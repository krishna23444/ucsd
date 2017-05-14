# 
# RandWords.pm : part of the Mace toolkit for building distributed systems
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
package RandWords;

# Copyright 2005 James W. Anderson.  All rights reserved.
# This program is free software; you can redistribute it and/or
# modify it under the same terms as Perl itself.

use strict;

use Carp;
use Util qw(:all);

use Class::MakeMethods::Template::Hash
    (
     'new --and_then_init' => [qw(new)],
     'array' => [qw(words)],
#      'hash --get_set_items' => [qw(indexGalleryCount)],
     );

sub init {
    my $this = shift;
    $this->loadDictionary();
} # init

sub randWord {
    my $this = shift;
    return ${$this->words()}[irand($this->count_words())];
} # randWord

sub randWords {
    my $this = shift;
    my $n = shift;
    return $this->nrandWords(irand($n) + 1);
} # randWords

sub nrandWords {
    my $this = shift;
    my $n = shift;
    my $r = "";
    for my $i (1..$n) {
	my $w = $this->randWord() . " ";
	$r .= $w;
	if (($i % 10) == 0) {
	    $r .= "\n";
	}
    }
    return $r;
} # randWords

sub loadDictionary {
    my $this = shift;
    my @dict = readFile("/usr/share/dict/words");
    chomp(@dict);
    $this->push_words(@dict);
} # loadDictionary

1;
