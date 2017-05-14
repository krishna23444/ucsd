# 
# CommonGrammar.pm : part of the Mace toolkit for building distributed systems
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
package Mace::Compiler::CommonGrammar;

use strict;
use Mace::Compiler::Grammar;

use constant COMMON => q{

Word : /\S*/
LookaheadWord : ...!<matchrule: $arg{rule}>[%arg] Word
LookaheadString : LookaheadWord[%arg](s?)
CopyLookaheadString : StartPos LookaheadString[%arg] EndPos
{
  $return = substr($Mace::Compiler::Grammar::text, $item{StartPos},
      1 + $item{EndPos} - $item{StartPos});
}
ToEnd : Word(s?)

StartPos : // { $thisoffset }
EndPos : { $prevoffset }
Line : // { $thisline }
Column : { $thiscolumn }

Id : /[_a-zA-Z][a-zA-Z0-9_]*/

SemiStatementToken : m|[^;{}][^;{}/]*|

SemiStatementBegin : SemiStatementToken(s)

BraceBlock : '{' SemiStatement(s?) '}'

Enum : 'enum' Id '{' SemiStatementBegin '}'

SemiStatement : Enum ';'
| SemiStatementBegin BraceBlock(?) (';')(?)
| <defer: Mace::Compiler::Globals::warning('unusual', $thisline, "Bare Brace Block Found")> BraceBlock (';')(?)
| <error>

MethodDecl : VirtualMethod | Method

VirtualMethod : 'virtual' Method
{
    $item{Method}->isVirtual(1);
    $return = $item{Method};
}

MethodTerm : StartPos Line BraceBlock EndPos
{
    my $startline = "";
    my $endline = "";
    if(defined($Mace::Compiler::Globals::filename) and $Mace::Compiler::Globals::filename ne '') {
      $startline = "\n#line ".$item{'Line'}." \"$Mace::Compiler::Globals::filename\"\n";
      $endline = "\n// __INSERT_LINE_HERE__\n";
    }
    $return = $startline.substr($Mace::Compiler::Grammar::text, $item{StartPos},
		     1 + $item{EndPos} - $item{StartPos}).$endline;
}
| '=' '0' ';' { $return = "0" }
| ';' { $return = "" }

Expression : StartPos Expression1 EndPos
{
    $return = substr($Mace::Compiler::Grammar::text, $item{StartPos},
		     1 + $item{EndPos} - $item{StartPos});
}

BinaryOp : '*' | '/' | '+' | '<<' | '>>' | '!=' | '==' | '<=' | '>=' | '<' | '>' | '||' | '|' | '&&' | '&' | '^' | '.' | '->' | '-' | <error>

UnaryOp : '!' | '~' | '*' | '&' | 'new' | 'delete' | <error>

#Changed to Expression1 to allow unary operators before things like my.foo
Expression1 : UnaryOp <commit> Expression1
| Expression2 BinaryOp <commit> Expression1
| Expression2
| <error>

QuotedString : <skip:'\s*'> 
               /"           #Opening Quote
                [^"\\\\]*     #Any number of characters not a quote or slash
                (           #Group: 1
                  \\\\        #Followed by a slash
                  .         #Any character
                  [^"\\\\]*   #Any number of characters not a quote or slash
                )*          #1: Repeated any number of times
                "           #Closing quote
               /sx

Number : /0x[a-fA-F0-9]+/ | /-?\d+(\.\d+)?/

Expression2 : Number
| ScopedId '(' <commit> Expression1(s? /,/) ')'
| ..."'" <commit> "'" /[^\']*/ "'"
| ...'"' <commit> QuotedString
| '(' <commit> Expression1 ')'
| ScopedId
| <error>

TypeOptions : <reject: !$arg{typeopt}> '__attribute((' <commit> TypeOption(s /,/) '))' 
{ 
  $return = $item[4];
} | { $return = []; }
TypeOption : <rulevar: %opt>
TypeOption : Line Id '(' TypeOptionParamList['options'=>\%opt] ')' 
{
  $return = Mace::Compiler::TypeOption->new(name => $item{Id}, line => $item{Line});
  $return->options(%opt);
} | <error>
TypeOptionParamList : 
( 
  Id '=' <commit> Expression 
  { $arg{options}->{$item{Id}} = $item{Expression} }
| Expression 
  { $arg{options}->{$item{Expression}} = $item{Expression} } 
)(s? /;/) (';')(?) ...')' | <error>

ArraySizes : <reject: !$arg{arrayok}> ArraySize(s) | { $return = []; }
ArraySize : '[' Expression ']' { $return = $item{Expression}; } | <error>

CheckSemi : ';' <commit> <reject: !$arg{semi}> | <reject: $arg{semi}> | <error>

Parameter : <reject: $arg{declareonly}> Line Type Id ArraySizes[%arg] TypeOptions[%arg] '=' <commit> Expression CheckSemi[%arg]
{ 
    my $p = Mace::Compiler::Param->new(name => $item{Id},
                                       type => $item{Type},
                                       hasDefault => 1,
                                       filename => $Mace::Compiler::Globals::filename,
                                       line => $item{Line},
                                       default => $item{Expression});
    $p->typeOptions(@{$item{TypeOptions}});
    $p->arraySizes(@{$item{ArraySizes}});
    $return = $p;
}
| <reject: !$arg{mustinit}> <commit> <error>
| Line Type Id <commit> ArraySizes[%arg] TypeOptions[%arg] CheckSemi 
{
    #print "Param1 type ".$item{Type}->toString()."\n";
    my $p = Mace::Compiler::Param->new(name => $item{Id},
				       type => $item{Type},
                                       filename => $Mace::Compiler::Globals::filename,
                                       line => $item{Line},
				       hasDefault => 0);
    $p->typeOptions(@{$item{TypeOptions}});
    $p->arraySizes(@{$item{ArraySizes}});

    $return = $p;
}
| <reject: !$arg{declareonly}> <commit> <error>
| Line Type DirArrow[direction => $arg{mapOk}] Type <reject:!defined($arg{mapOk})>
{
    #print "Param2 type ".$item{Type}->toString()."\n";
    my $p = Mace::Compiler::Param->new(name => "noname_".$thisrule->{'local'}{'paramnum'}++,
				       type => $item[4],
                                       typeSerial => $item[2],
                                       filename => $Mace::Compiler::Globals::filename,
                                       line => $item{Line},
				       hasDefault => 0);

    $return = $p;
}
| Line Type ('=' Expression)(?) <reject:!defined($arg{noIdOk})>
{
    #print "Param2 type ".$item{Type}->toString()."\n";
    my $p = Mace::Compiler::Param->new(name => "noname_".$thisrule->{'local'}{'paramnum'}++,
				       type => $item{Type},
                                       filename => $Mace::Compiler::Globals::filename,
                                       line => $item{Line},
				       hasDefault => scalar(@{$item[3]}));

    if ($p->hasDefault()) {
	$p->default(${$item[3]}[0]);
    }
    $return = $p;
}
| Line Id <reject:!defined($arg{typeOptional})>
{
    #print "Param2 type ".$item{Type}->toString()."\n";
    my $p = Mace::Compiler::Param->new(name => $item{Id},
				       #type => $item{Type},
                                       filename => $Mace::Compiler::Globals::filename,
                                       line => $item{Line},
				       hasDefault => 0);

    if ($p->hasDefault()) {
	$p->default(${$item[3]}[0]);
    }
    $return = $p;
}
| <error>

AutoType : Line Id TypeOptions[typeopt => 1] '{' Parameter[typeopt => 1, semi => 1](s?) Constructor[className => $item{Id}](s?) Method(s?) '}' (';')(?) 
{
  my $at = Mace::Compiler::AutoType->new(name => $item{Id},
					 line => $item{Line},
					 includedline => defined($thisparser->{'local'}{update}) ? $thisparser->{'local'}{update} : $item{Line},
					 );
  $at->typeOptions(@{$item{TypeOptions}});
  $at->fields(@{$item[5]});
  $at->constructors(@{$item[6]});
  $at->methods(@{$item[7]});
  for my $m (@{$item[6]}) {
    if($m->name ne $item{Id}) {
      Mace::Compiler::Globals::error("bad_auto_type",  $item{Line}, "Constructor name does not match auto_type name");
    }
  }
  my $key = "service";
  if (defined($arg{key})) {
      $key = $arg{key};
  }
  $thisparser->{'local'}{$key}->push_auto_types($at);
}
| <error>

#XXX: reject
DirArrow : '<-' | '->'

KeyEqVal : Id '=' Id { $return = [ $item[1], $item[-1] ] }

MethodOptions : '[' KeyEqVal(s /;/) (';')(?) ']' { $return = $item[2] }

InitializerItem : ScopedId '(' Expression(s? /,/) ')'
InitializerList : StartPos Line ':' InitializerItem(s /,/) EndPos 
{
    my $startline = "";
    my $endline = "";
    if(defined($Mace::Compiler::Globals::filename) and $Mace::Compiler::Globals::filename ne '') {
      $startline = "\n#line ".$item{'Line'}." \"$Mace::Compiler::Globals::filename\"\n";
      $endline = "\n// __INSERT_LINE_HERE__\n";
    }
    $return = $startline.substr($Mace::Compiler::Grammar::text, $item{StartPos},
		     1 + $item{EndPos} - $item{StartPos}).$endline;
}
| {$return = ""}

Constructor : <reject:defined($arg{className})> <commit> /\b$thisparser->{'local'}{'classname'}\b/ '(' Parameter(s? /,/) ')' InitializerList MethodTerm
{
    my $t = Mace::Compiler::Type->new(type => "");
    my $m = Mace::Compiler::Method->new(name => $thisparser->{'local'}{'classname'},
					returnType => $t,
					body => $item{InitializerList}.$item{MethodTerm});

    if (scalar(@{$item[5]})) {
	$m->params(@{$item[5]});
    }

    $return = $m;
}
| <reject:!defined($arg{className})> <commit> /\b$arg{className}\b/ '(' Parameter(s? /,/) ')' InitializerList MethodTerm
{
    my $t = Mace::Compiler::Type->new(type => "");
    my $m = Mace::Compiler::Method->new(name => $arg{className},
					returnType => $t,
					body => $item{InitializerList}.$item{MethodTerm});

    if (scalar(@{$item[5]})) {
	$m->params(@{$item[5]});
    }

    $return = $m;
}
| <error>


Destructor : ('virtual')(?) '~' /\b$thisparser->{'local'}{'classname'}\b/ '(' ')' MethodTerm
{
    my $t = Mace::Compiler::Type->new(type => "");
    my $m = Mace::Compiler::Method->new(name => '~' . $thisparser->{'local'}{'classname'},
					returnType => $t,
					body => $item{MethodTerm});
    if (defined($item[1])) {
	$m->isVirtual(1);
    }
    $return = $m;
}

#MethodReturnType : <reject:$arg{noReturn}> Type | <reject:!$arg{noReturn}> { $return = Mace::Compiler::Type->new(); } | <error>
MethodReturnType : Type ...!'(' { $return = $item{Type} } | <reject:!$arg{noReturn}> { $return = Mace::Compiler::Type->new(); } | <error>

MethodOperator : '==' | '<=' | '>=' | '<' | '>' | '=' | '!=' | '+' | '*' | '/' | '->' | '-'
MethodName : 'operator' <commit> MethodOperator { $return = "operator".$item{MethodOperator}; } | Id | <error>

Method : MethodReturnType[%arg] Line MethodName '(' Parameter[%arg](s? /,/) ')' ConstToken(?) MethodOptions(?) MethodTerm
{
    my $m = Mace::Compiler::Method->new(name => $item{MethodName},
					returnType => $item{MethodReturnType},
					isConst => scalar(@{$item[-3]}),
                                        line => $item{Line},
					body => $item{MethodTerm});
    if (scalar(@{$item[5]})) {
	$m->params(@{$item[5]});
    }

    if (scalar(@{$item[-2]})) {
	my $ref = ${$item[-2]}[0];
	for my $el (@$ref) {
	    $m->options(@$el);
	}
    }

    $return = $m;
}
| <error>

PointerType : NonPointerType ConstToken ('*')(s) | NonPointerType ('*')(s) ConstToken ('*')(s) | NonPointerType ('*')(s?)

NonPointerType : BasicType | ScopedType 

#XXX-CK: Shouldn't this really be a recursion on Type, not Id?  After all, you can have std::map<> or map<>::iterator . . . 
ScopedId : StartPos TemplateTypeId ('::' TemplateTypeId)(s?) EndPos { $return = substr($Mace::Compiler::Grammar::text, $item{StartPos}, 1 + $item{EndPos} - $item{StartPos}); }
ScopedType : TemplateType ('::' TemplateType)(s?) 

#NOTE: CK -- added ScopedType to Template type to allow ::
#      CK -- Added | Expression since template parameters could be constant expressions
#      CK -- BasicType added to parse primitive types and their modifiers
TemplateTypeId : Id '<' ( ConstToken(?) (PointerType | Number) )(s /,/) '>' | Id | <error>
TemplateType : Id '<' <commit> ( ConstToken(?) (PointerType | Number) )(s /,/) '>' | Id | <error>

ConstToken : 'const'

RefToken : '&'

#NOTE: CK -- added \b to avoid problems parsing things like int8_t.  Also -- removed 'long long'
TypeModifier : /\blong\b/ | /\bsigned\b/ | /\bunsigned\b/ | /\bshort\b/ | <error>
PrimitiveType : /\bint\b/ | /\bdouble\b/ | /\bfloat\b/ | /\bchar\b/ | <error>
BasicType : TypeModifier(0..3) PrimitiveType | TypeModifier(1..3) | <error>

Type : ConstToken(?) StartPos PointerType EndPos ConstToken(?) RefToken(?)
{
    my $type = substr($Mace::Compiler::Grammar::text, $item{StartPos},
		      1 + $item{EndPos} - $item{StartPos});

    $return = Mace::Compiler::Type->new(type => Mace::Util::trim($type),
 					isConst => (scalar(@{$item[1]}) or scalar(@{$item[-2]})),
 					isRef => scalar(@{$item[-1]}));
}

ProtectionToken : 'public' | 'private' | 'protected'

EOFile : /^\Z/

}; # COMMON grammar

1;
