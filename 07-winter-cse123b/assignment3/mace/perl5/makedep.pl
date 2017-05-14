#!/usr/bin/perl -w
# 
# makedep.pl : part of the Mace toolkit for building distributed systems
# 
# Copyright (c) 2006, Charles Killian, James W. Anderson, Ryan Braud, Adolfo Rodriguez
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

use Mace::Compiler::GeneratedOn;
use strict;

my @all = <*>;
my @perl = ();
my %dep = ();
my @makedep = ('$(MACEPATH)/perl5');

for my $f (@all) {
    if (-T $f) {
	my $h = `head -1 $f`;
	chomp($h);
	if ($h =~ m|perl| && -x $f) {
	    push(@perl, $f);
	    $dep{$f} = [];
	}
    }
}

my %mod = ();
my @mods = ();
for my $f (@perl) {
    my ($u, $r) = readMods($f);
    for my $m (@$u) {
	addDep(\%dep, $f, $m);
	if (!grep(/$m/, @mods)) {
	    push(@mods, $m);
	}
    }
    for my $m (@$r) {
	if (!grep(/$m/, @mods)) {
	    push(@mods, $m);
	}
    }
}

my @visited = ();
while (my $f = shift(@mods)) {
    push(@visited, $f);
    $f =~ s|::|/|g;
    $f .= ".pm";
    my ($u, $r) = readMods($f);
    addDep(\%mod, $f);
    for my $m (@$u) {
	addDep(\%mod, $f, $m);
	if (!grep(/$m/, @visited)) {
	    push(@mods, $m);
	}
    }
    for my $m (@$r) {
	if (!grep(/$m/, @visited)) {
	    push(@mods, $m);
	}
    }
}

# use Data::Dumper;

print Mace::Compiler::GeneratedOn::generatedOnHeader("Perl5 Makefile Variables", "commentLinePrefix" => "#");

while (my ($k, $v) = each(%mod)) {
    my $t = makeVar($k);
    print "$t = \$(MACEPATH)/perl5/$k " . join(" ", map { '$(' . makeVar($_) . ')' } @$v) . "\n";
}

while (my ($k, $v) = each(%dep)) {
    print "${k}_dep = \$(MACEPATH)/perl5/$k " . join(" ", map { '$(' . makeVar($_) . ')' } @$v) . "\n";
    print "$k = \$(MACEPATH)/perl5/$k\n";
}

print "PERLBINS = ".join(' ', keys(%dep))."\n";


#print "\$(PERLMAKE) : ".join(" ", @makedep)."\n";

sub makeVar {
    my $t = shift;
    if ($t =~ m|/|) {
	$t =~ s|/|_|g;
	$t =~ s|\.pm|_dep|;
    }
    else {
	$t =~ s|::|_|g;
	$t .= "_dep";
    }
    return $t;
}

sub readMods {
    my $f = shift;
    my @u = ();
    my @r = ();
    if (not -e $f) {
	warn "WARNING: cannot read file $f\n";
	return ();
    }
    push(@makedep, "\$(MACEPATH)/perl5/$f");
#     die "cannot open $f: $!" unless (-e $f);
    my @l = `cat $f`;
    for my $l (@l) {
	chomp($l);
	if ($l =~ m|^use\s+(Mace::[\w:]+);$|) {
	    push(@u, $1);
	}
	elsif ($l =~ m|^require\s+(Mace::[\w:]+);$|) {
	    push(@r, $1);
	}
    }
    return (\@u, \@r);
}

sub addDep {
    my ($r, $k, $v) = @_;
    if (defined($r->{$k})) {
	my $ar = $r->{$k};
	if (defined($v) && !grep(/$v/, @$ar)) {
	    push(@$ar, $v);
	}
    }
    else {
	if (defined($v)) {
	    $r->{$k} = [$v];
	}
	else {
	    $r->{$k} = [];
	}
    }
}
