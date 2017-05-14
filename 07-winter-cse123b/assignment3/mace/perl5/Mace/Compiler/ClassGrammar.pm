# 
# ClassGrammar.pm : part of the Mace toolkit for building distributed systems
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
package Mace::Compiler::ClassGrammar;

use strict;
use Mace::Compiler::Grammar;
use Mace::Compiler::ServiceClassCommonGrammar;

use constant CLASS => Mace::Compiler::ServiceClassCommonGrammar::SERVICE_CLASS_COMMON . q[

File : <skip: qr{\s* ((/[*] .*? [*]/|(//[^\n]*\n)) \s*)*}sx> Reset LookaheadString[name=>"", rule=>'ClassBegin'] Class ToEnd EOFile
{ $return = $thisparser->{'local'}{'serviceclass'} } | <error>

NamedFile : <skip: qr{\s* ((/[*] .*? [*]/|(//[^\n]*\n)) \s*)*}sx> Reset LookaheadString[name=>$arg[0], rule=>'ClassBegin'] NamedClass[name=>$arg[0]] ToEnd EOFile
{ $return = $thisparser->{'local'}{'serviceclass'} } | <error>

ClassBegin : ('class' | 'struct') /\b$arg{name}\b/ { $return = $item[-1] }

Class : ('class' | 'struct') Id 
{
    $thisparser->{'local'}{'classname'} = $item{Id};
}
ClassExt(?) '{' ClassBody MaceVoodooBlock '}' ';'
{
    my $classname = $item{Id};
    $thisparser->{'local'}{'serviceclass'}->name($classname);
    if ($classname =~ m|ServiceClass$|) {
	$thisparser->{'local'}{'serviceclass'}->isServiceClass(1);
    }
    elsif ($classname =~ m|Handler$|) {
	$thisparser->{'local'}{'serviceclass'}->isHandler(1);
    }
} | <error>

NamedClass : ClassBegin[%arg] 
{
    $thisparser->{'local'}{'classname'} = $item{ClassBegin};
}
ClassExt(?) '{' ClassBody MaceVoodooBlock '}' ';'
{
    my $classname = $item{ClassBegin};
    $thisparser->{'local'}{'serviceclass'}->name($classname);
    if ($classname =~ m|ServiceClass$|) {
	$thisparser->{'local'}{'serviceclass'}->isServiceClass(1);
    }
    elsif ($classname =~ m|Handler$|) {
	$thisparser->{'local'}{'serviceclass'}->isHandler(1);
    }
} | <error>

ClassExtMod : 'virtual' | ProtectionToken

ClassExt : ':' (ClassExtMod(s?) Id)(s /,/)
{ $thisparser->{'local'}{'serviceclass'}->superclasses(@{$item[2]}) }

# ClassBody : ...!('}'|'#ifdef MACE_VOODOO') <commit> Statement ClassBody | <error?> <reject> |
BraceTokenOrMaceVoodooBegin : '}' | '#ifdef MACE_VOODOO'

ClassBody : ...!BraceTokenOrMaceVoodooBegin <commit> Statement ClassBody | <error?> <reject> |

Statement : ProtectionToken ':' | MethodDecl
{ $thisparser->{'local'}{'serviceclass'}->push_methods($item{MethodDecl}) }
| Constructor
{ $thisparser->{'local'}{'serviceclass'}->push_constructors($item{Constructor}) }
| Destructor
{ $thisparser->{'local'}{'serviceclass'}->destructor($item{Destructor}) }
| SemiStatement

MaceBlock : StartPos 'mace' <commit> ('provides'|'services') BraceBlock EndPos
{
    my $maceLit = substr($Mace::Compiler::Grammar::text, $item{StartPos},
			 1 + $item{EndPos} - $item{StartPos});
    $thisparser->{local}{serviceclass}->maceLiteral($maceLit);
}

MaceVoodooBlock : '#ifdef MACE_VOODOO' <commit> MaceBlock '#endif // MACE_VOODOO' |

]; # CLASS grammar

1;
