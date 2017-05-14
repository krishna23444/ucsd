# 
# RunLog.pm : part of the Mace toolkit for building distributed systems
# 
# Copyright (c) 2005, Charles Killian, Dejan Kostic, Ryan Braud, James W. Anderson, John Fisher-Ogden, Calvin Hubble, Duy Nguyen, Justin Burke, Amin Vahdat, Adolfo Rodriguez, Sooraj Bhat
# All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
# 
#    * Redistributions of source code must retain the above copyright
#      notice, this list of conditions and the following disclaimer.
#    * Redistributions in binary form must reproduce the above copyright
#      notice, this list of conditions and the following disclaimer in
#      the documentation and/or other materials provided with the
#      distribution.
#    * Neither the names of Duke University nor The University of
#      California, San Diego, nor the names of the authors or contributors
#      may be used to endorse or promote products derived from
#      this software without specific prior written permission.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
# USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
# 
# ----END-OF-LEGAL-STUFF----
package RunLog;

# Copyright 2005 James W. Anderson.  All rights reserved.
# This program is free software; you can redistribute it and/or
# modify it under the same terms as Perl itself.

use strict;

use Symbol;
use FileHandle;
use Date::Manip;
use Time::HiRes qw(gettimeofday);
use Carp;
use MIME::Base64;

use Util qw(:all);

use Class::MakeMethods::Template::Hash
    (
     'new --and_then_init' => [qw(new)],
     'string' => [qw(filename)],
     'scalar' => [qw(file)],
     'scalar' => [qw(encode)],
     );


sub init {
    my $this = shift;
    $this->encode(0);
} # init

sub open {
    my $this = shift;
    my $filename = shift || die "required for open: filename";
    my $mode = shift || "> ";
    
    my $fh = FileHandle->new($mode . $filename) or die "cannot open $filename: $!\n";
    $this->filename($filename);
    $this->file($fh);
} # open    

sub close {
    my $this = shift;
    $this->file()->close();
} # close

sub replay {
    my $this = shift;
    my ($file, $obj) = @_;

    CORE::open(IN, $file) or die "cannot open $file: $!\n";
    my $t = [0, 0];
    while (my $l = <IN>) {
	chomp($l);
	$t = $this->replayLine($obj, @$t, $l);
    }
    CORE::close(IN);
} # replay

sub dump {
    my $this = shift;
    my ($file, $fp) = @_;
    CORE::open(IN, $file) or die "cannot open $file: $!\n";
    while (my $l = <IN>) {
	chomp($l);
	my $ts;
	my $method;
	my @a;
	$this->parseLine($l, \$ts, \$method, \@a);
	if ($method =~ m/(write|createFile)/) {
	    pop(@a);
	}
	$ts = substr($ts, 0, -7);
	my $date = UnixDate(ParseDate("epoch $ts"), '%u');
	print $fp "[$date] $method @a\n";
    }
    CORE::close(IN);
} # dump

sub replayLine {
    my $this = shift;
    my ($obj, $prevlogtime, $prevtime, $l) = @_;

    my $ts;
    my $method;
    my @a;
    $this->parseLine($l, \$ts, \$method, \@a);

    if ($prevlogtime) {
	my $now = $this->timeusec();
	die "$prevlogtime > $ts" if $prevlogtime > $ts;
	my $diff = $ts - $prevlogtime;
	my $realdiff = $now - $prevtime;
	if ($diff > $realdiff) {
	    my $sleepTime = $diff - $realdiff;
	    Time::HiRes::sleep($sleepTime);
	}
    }

    print "calling $method with @a\n";
#     $obj->$method(@a);
    
    return [$ts, $this->timeusec()];
} # replayLine

sub parseLine {
    my $this = shift;
    my ($l, $ts, $method, $aref) = @_;

    my @a = split(/ /, $l);
    $$ts = shift(@a);
    $$method = shift(@a);
    @$aref = map { decode_base64($_) } @a;
} # parseLine
    
sub log {
    my $this = shift;
    my ($method, @args) = @_;

    unless ($this->file()) {
	die "cannot log on unopened file\n";
    }

    my $now = $this->timeusec();

    my $s = $now . " " . $method;
    for my $el (@args) {
	$s .= " " . ($this->encode() ? encode_base64($el, "") : $el);
    }
    $s .= "\n";

    my $fh = $this->file();
    (syswrite($fh, $s) == length($s)) or die("error writing " .
					     $this->filename() . ": $!\n");
} # log

sub timeusec() {
    my $this = shift;
    my ($seconds, $microseconds) = gettimeofday();
    my $frac = sprintf("%.6f", ($microseconds / 10**6));
    $frac = substr($frac, 2);
    my $time = $seconds . "." . $frac;
#     $seconds *= 10**6;
#     $seconds += $microseconds;
#     return $seconds;
    return $time;
} # timeusec

1;
