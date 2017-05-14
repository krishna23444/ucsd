# 
# MaceGrammar.pm : part of the Mace toolkit for building distributed systems
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
package Mace::Compiler::MaceGrammar;

use strict;
use Mace::Compiler::Grammar;
use Mace::Compiler::CommonGrammar;

use constant MACE => Mace::Compiler::CommonGrammar::COMMON . q[

Preamble : Line CopyLookaheadString[rule=>'ServiceName'] 
{ $thisparser->{'local'}{'service'}->header("#line ".$item{'Line'}." \"$Mace::Compiler::Globals::filename\"\n".$item{CopyLookaheadString}."\n// __INSERT_LINE_HERE__\n"); }

PrintError : MFile 
| {
    my $lastLine = "";
    my $lastMsg = "";
    for my $i (@{$thisparser->{errors}}) {
	if ($i->[1] ne $lastLine || $i->[0] ne $lastMsg) {
	    Mace::Compiler::Globals::error('invalid syntax', $i->[1], $i->[0]);
	    $lastLine = $i->[1];
	    $lastMsg = $i->[0];
	}
    }
    $thisparser->{errors} = undef;
}
MFile : <skip: qr{\s* ((/[*] .*? [*]/|(//[^\n]*\n)) \s*)*}sx> 
        { 
          $thisparser->{'local'}{'service'} = Mace::Compiler::ServiceImpl->new(); 
          $thisparser->{'local'}{'service'}->filename($Mace::Compiler::Globals::filename);
          $thisparser->{local}{'mincludes'} = {};
        } 
        Preamble Service EOFile 
        { 
          $return = $thisparser->{'local'}{'service'}; 
        }
        | <error>

Service : ServiceName Provides TraceLevel MaceTime ServiceBlock(s) ...EOFile 
{ 
  $thisparser->{'local'}{'service'}->name($item{ServiceName}); 
  $thisparser->{'local'}{'service'}->provides(@{$item{Provides}}); 
  $thisparser->{'local'}{'service'}->trace($item{TraceLevel});
  $thisparser->{'local'}{'service'}->macetime($item{MaceTime});
}
        | <error>

ServiceName : 'service' Id <commit> ';' 
                  { $return = $item{Id}; }
            | 'service' <commit> ';'
                  { 
                    my $svName = $Mace::Compiler::Globals::filename;
                    $svName =~ s|^.*/||;
                    $svName =~ s|[.].*||;
                    $return = $svName;
                  }

Provides : 'provides' <commit> Id(s /,/) ';'
                  { $return = $item[3]; }
            | 
                  { $return = [ "Null" ]; }

Level : 'off' | 'manual' | 'low' | 'med' | 'high' | <error>
TraceLevel : 'trace' <commit> '=' Level ';' { $return = $item{Level}; } | { $return = 'off'; }

MaceTimeLevel : 'uint64_t' | 'MaceTime' | <error>
MaceTime : 'time' <commit> '=' MaceTimeLevel ';' { $return = $item{MaceTimeLevel} eq "MaceTime"; } | { $return = 0; }

ServiceBlockName : 'log_selectors' | 'constants' | 'services'|
                   'constructor_parameters' | 'transitions' | 'routines' |
                   'states' | 'auto_types' | 'typedefs' | 'state_variables' |
                   'method_remappings' | 'messages' | 'properties' | '#minclude' | <error>

ServiceBlock : ServiceBlockName Block[rule => $item{'ServiceBlockName'}] 

Filename : /[^"\n\t]+/
#" -- ignore this line!
Block : <reject: $arg{'rule'} eq '#minclude'> <commit> { $thisrule->{'local'}{$arg{'rule'}}++; } <reject: do{$thisrule->{'local'}{$arg{'rule'}} > 1 && !$thisparser->{local}{update} }> <uncommit> '{' <matchrule: $arg{'rule'}> '}' (';')(?)
        { print "Matched block $arg{'rule'}\n"; $thisparser->{local}{$arg{'rule'}} = $item[5]; }
      | <error?: Cannot define block '$arg{'rule'}' more than once> <reject>
      | <reject: $arg{'rule'} ne '#minclude'> <commit> Line /"/ Filename /"\s/ { $thisparser->{local}{'mincludes'}->{$item{Line}} = $item{Filename} }
      | <error>
#      | <error?: Could not parse '#minclude' statement> <reject> <error>

log_selectors : <reject: do{$thisparser->{local}{update}}> Selector(s?) ...'}' 
SelectorName : 'default' | 'message' | <error>
Selector : <rulevar: $selname = ""> 
Selector : SelectorName { $selname = $item{SelectorName}; } '=' QuotedString ';' { $thisrule->{'local'}{'selector_'.$item{'SelectorName'}}++; } <commit> <reject: do{$thisrule->{'local'}{'selector_'.$item{'SelectorName'}} > 1}>
{
  $thisparser->{'local'}{'service'}->selectors($item{SelectorName}, $item{QuotedString});
}
      | <error?: Cannot define selector string for "$selname" more than once> <error>

ConstantsIncludes : '#include' /.*\n/ { $return = $item[1]." ".$item[2]; }
constants : ConstantsIncludes(s?) Parameter[mustinit => 1, semi => 1](s?) ...'}' 
{
    $thisparser->{'local'}{'service'}->unshift_constants_includes(@{$item[1]});
    $thisparser->{'local'}{'service'}->unshift_constants(@{$item[2]});
}
constructor_parameters : <reject: do{$thisparser->{local}{update}}> Parameter[mustinit => 1, semi => 1](s?) ...'}' 
{ 
  $thisparser->{'local'}{'service'}->constructor_parameters(@{$item[2]});
}

states : <reject: do{$thisparser->{local}{update}}> StateName(s?) ...'}' 
{
  $thisparser->{'local'}{'service'}->states(@{$item[2]});
}
StateName : <rulevar: $id = ""> 
          | Id <commit> { $id = $item{'Id'}; $thisrule->{'local'}{$item{'Id'}}++; } <reject: do{$item{'Id'} eq "init" ||  $thisrule->{'local'}{$item{'Id'}} > 1}> <uncommit> Id(?) ';' { $return = $item{Id}; }
          | <error?: Invalid state '$id'.  States cannot be reused nor be 'init'.> <error>

services : <reject: do{$thisparser->{local}{update}}> ServiceUsed(s?) ...'}'
{
  $thisparser->{'local'}{'service'}->service_variables(@{$item[2]});
}
InlineFinal : 'inline' { $return = 2; } | 'final' { $return = 1; } | { $return = 0; }
HandlerList : '[' Id(s? /,/) ']' { $return = $item[2]; } | '[' '*' ']' { $return = -1; } | { $return = -1; }
RegistrationUid : '::' <commit> (ScopedId|Number) { $return = $item[3]; } | <error?> <reject> | { $return = "-1"; }
#XXX: Use more intelligent service name checking? -- as in existance of file
ServiceUsed : Line InlineFinal Id HandlerList Id RegistrationUid '=' Line Id '(' <commit> Expression(s? /,/) ')' ';' 
{ 
  $return = Mace::Compiler::ServiceVar->new(name => $item[5], serviceclass => $item[3], service => $item[9], defineLine => $item[8], line => $item[1], intermediate => ($item{InlineFinal}==2?1:0), final => ($item{InlineFinal}==1?1:0), registrationUid => $item{RegistrationUid});
  $return->constructionparams(@{$item[12]});
  if(scalar($item{HandlerList}) == -1) {
    $return->allHandlers(1);
  } else {
    $return->allHandlers(0);
    $return->handlerList(@{$item{HandlerList}});
  }
}
        | Line InlineFinal Id HandlerList Id RegistrationUid '=' <commit> Line Id ';' 
{ 
  $return = Mace::Compiler::ServiceVar->new(name => $item[5], serviceclass => $item[3], dynamicCastSource => $item[10], defineLine => $item[9], line => $item[1], intermediate => ($item{InlineFinal}==2?1:0), final => ($item{InlineFinal}==1?1:0), registrationUid => $item{RegistrationUid}, doDynamicCast => 1);
  if(scalar($item{HandlerList}) == -1) {
    $return->allHandlers(1);
  } else {
    $return->allHandlers(0);
    $return->handlerList(@{$item{HandlerList}});
  }
}
        | Line InlineFinal Id HandlerList Id RegistrationUid ';' <commit> 
{ 
  $return = Mace::Compiler::ServiceVar->new(name => $item[5], serviceclass => $item[3], line => $item[1], intermediate => ($item{InlineFinal}==2?1:0), final => ($item{InlineFinal}==1?1:0), registrationUid => $item{RegistrationUid});
  if(scalar($item{HandlerList}) == -1) {
    $return->allHandlers(1);
  } else {
    $return->allHandlers(0);
    $return->handlerList(@{$item{HandlerList}});
  }
}
        | <error>

typedefs : TypeDef(s?) ...'}'
        | <error>

TypeDef : 'typedef' Line Type Id ';'
{
    my $incl = $item{Line};
    if (defined($thisparser->{'local'}{'update'})) {
	$incl = defined($thisparser->{'local'}{'update'});
    }
    $thisparser->{'local'}{'service'}->push_typedefs(Mace::Compiler::TypeDef->new(name=>$item{Id}, type=>$item{Type}, line => $item{Line}, includedline => $incl));
}
        | <error>

#XXX: Add in framework for checking specific options

state_variables : Variable(s?) ...'}' 

Variable : ...'timer' <commit> Timer | StateVar | <error>

Timer : 'timer' TimerTypes Id TypeOptions[typeopt => 1] ';'
{
  my $timer = Mace::Compiler::Timer->new(name => $item{Id});
  $timer->typeOptions(@{$item{TypeOptions}});
  $timer->types(@{$item{TimerTypes}});
  $thisparser->{'local'}{'service'}->push_timers($timer);
}
TimerTypes : '<' <commit> Type(s /,/) '>' { $return = $item[3]; } | { $return = []; }

StateVar : Parameter[typeopt => 1, arrayok => 1, semi => 1]
{
  $thisparser->{'local'}{'service'}->push_state_variables($item{Parameter});
}

auto_types : AutoType(s?) ...'}' | <error>

messages : Message(s?) ...'}' | <error>
Message : Line Id TypeOptions[typeopt => 1] '{' Parameter[declareonly => 1, typeopt => 1, semi => 1](s?) '}' (';')(?) 
{ 
  my $at = Mace::Compiler::AutoType->new(name => $item{Id}, line => $item{Line});
  $at->typeOptions(@{$item{TypeOptions}});
  $at->fields(@{$item[5]});
  $thisparser->{'local'}{'service'}->push_messages($at);
}
| <error>

method_remappings : RemappingsSection(s?) ...'}' | <error>
RemappingsSection : RemappingsSectionNames Block[rule=>$item{RemappingsSectionNames}] | <error>
RemappingsSectionNames : 'uses' | 'implements' | <error>

ParseMethodRemapping : '(' Parameter[%arg](s? /,/) ')' ';' | <error>

uses : UsesMethod(s?) ...'}'

UsesMethod : UsesToken Method[noReturn => 1, noIdOk => 1, mapOk => $item{UsesToken}]
{
#     print "from UsesMethod: parsing " . $item{Method}->toString(noline => 1) . "\n";
    if($item{UsesToken} eq "up") {
	if (grep { $item{Method}->eq($_, 1) } $thisparser->{'local'}{'service'}->usesUpcalls()) {
	    unless ($thisparser->{local}{update}) {
		die "duplicate remapping";
	    }
	}
	else {
            if($thisparser->{'local'}{'update'}) {
              $item{Method}->options('mhdefaults',1);
              my $arr = $thisparser->{'local'}{'service'}->usesUpcalls();
              my $i = scalar(grep($_->options('mhdefaults'), @$arr));
              splice(@$arr, $i, 0, $item{Method});
              1;
            } else {
              $thisparser->{'local'}{'service'}->push_usesUpcalls($item{Method});
            }
	}
    } else {
	if (grep { $item{Method}->eq($_, 1) } $thisparser->{'local'}{'service'}->usesDowncalls()) {
	    unless ($thisparser->{local}{update}) {
		die "duplicate remapping";
	    }
	}
	else {
            if($thisparser->{'local'}{'update'}) {
              $item{Method}->options('mhdefaults',1);
              my $arr = $thisparser->{'local'}{'service'}->usesDowncalls();
              my $i = scalar(grep($_->options('mhdefaults'), @$arr));
              splice(@$arr, $i, 0, $item{Method});
              1;
            } else {
              $thisparser->{'local'}{'service'}->push_usesDowncalls($item{Method});
            }
	}
    }
}
           | <error>
UsesToken : 'upcall_' { $return = "up" } | 'downcall_' { $return = "down" }

implements : ImplementsSection(s?) ...'}' | <error>
ImplementsSection : ImplementsBlockNames Block[rule=>$item{ImplementsBlockNames}] | <error>
ImplementsBlockNames : "upcalls" | "downcalls" | <error>

upcalls : Upcall(s?) ...'}' | <error>
Upcall : Method[noReturn => 1, noIdOk => 1, mapOk => "up"] 
{
    if (grep { $item{Method}->eq($_, 1) } $thisparser->{'local'}{'service'}->implementsUpcalls()) {
	unless ($thisparser->{local}{update}) {
	    die "duplicate remapping";
	}
    }
    else {
        if($thisparser->{'local'}{'update'}) {
          $item{Method}->options('mhdefaults',1);
          my $arr = $thisparser->{'local'}{'service'}->implementsUpcalls();
          my $i = scalar(grep($_->options('mhdefaults'), @$arr));
          splice(@$arr, $i, 0, $item{Method});
          1;
        } else {
          $thisparser->{'local'}{'service'}->push_implementsUpcalls($item{Method});
        }
    }
}
| <error>
downcalls : Downcall(s?) ...'}' | <error>
Downcall : Method[noReturn => 1, noIdOk => 1, mapOk => "down"] 
{
    if (grep { $item{Method}->eq($_, 1) } $thisparser->{'local'}{'service'}->implementsDowncalls()) {
	unless ($thisparser->{local}{update}) {
	    die "duplicate remapping";
	}
    }
    else {
        if($thisparser->{'local'}{'update'}) {
          $item{Method}->options('mhdefaults',1);
          my $arr = $thisparser->{'local'}{'service'}->implementsDowncalls();
          my $i = scalar(grep($_->options('mhdefaults'), @$arr));
          splice(@$arr, $i, 0, $item{Method});
          1;
        } else {
          $thisparser->{'local'}{'service'}->push_implementsDowncalls($item{Method});
        }
    }
}
| <error>

routines : ( 
              Method { $thisparser->{'local'}{'service'}->push_routines($item{Method}); }
            | RoutineObject 
           )(s?) ...'}' | <error>
RoutineObject : <defer: Mace::Compiler::Globals::warning('deprecated', $thisline, "Objects in routines blocks deprecated.  Use the new (not-yet-implemented) types block")> ObjectType Id MethodTerm ';' 
{
  #XXX -- MethodTerm instead of braceblock since it returns the string.
  my $o = Mace::Compiler::RoutineObject->new(name => $item{Id}, type => $item{ObjectType}, braceblock => $item{MethodTerm});
  $thisparser->{'local'}{'service'}->push_routineObjects($o);
}
| <error>
ObjectType : 'class' | 'struct' | <error>

transitions : '}' <commit> <reject> 
| ( 
    ...TransitionType <commit> Transition 
  | ...'(' <commit> GuardBlock 
  | <error> 
  )(s) ...'}' Line { $thisparser->{'local'}{'service'}->transitionEnd($item{Line}) }
| <error?:Services must contain at least one transition!> <error>
GuardBlock : <commit> 
             <defer: Mace::Compiler::Globals::warning('deprecated', $thisline, "Bare block state expressions are deprecated!  Use as-yet-unimplemented 'guard' blocks instead!")> 
             '(' Line Expression ')' <uncommit> { $thisparser->{'local'}{'service'}->push_guards(Mace::Compiler::Guard->new('guardStr' => $item{Expression},
                                                                                                                       'line' => $item{Line},
                                                                                                                       'file' => $Mace::Compiler::Globals::filename,
                                                                                                                       )) } 
             '{' transitions '}' { $thisparser->{'local'}{'service'}->pop_guards($item{Expression}) }
           | <error?> { $thisparser->{'local'}{'service'}->pop_guards(); } <error>
StartCol : // { $return = $thiscolumn; }
Transition : StartPos StartCol TransitionType Line StateExpression Method[noReturn => 1, typeOptional => 1] 
{ 
  my $transitionType = $item{TransitionType};
  if(ref ($item{TransitionType})) {
    $transitionType = "aspect";
    #TODO: Resolve how to handle aspects with same name but different parameters, and same parameters but different name.
  }
  my $t = Mace::Compiler::Transition->new(name => $item{Method}->name(), startFilePos => ($thisparser->{local}{update} ? -1 : $item{StartPos}), columnStart => $item{StartCol}, type => $transitionType, method => $item{Method} );
  $t->guards(@{$thisparser->{'local'}{'service'}->guards()});
  $t->push_guards(Mace::Compiler::Guard->new('guardStr' => $item{StateExpression},
                                             'line' => $item{Line},
                                             'file' => $Mace::Compiler::Globals::filename,)); #Deprecated
  if(ref ($item{TransitionType})) {
    $t->options('monitor',$item{TransitionType});
  }
  $thisparser->{'local'}{'service'}->push_transitions($t);
} 
| <error>
TransitionType : 'downcall' | 'upcall' | 'scheduler' | 'aspect' <commit> '<' Id(s /,/) '>' { $return = $item[4] } | <error>
StateExpression : <defer: Mace::Compiler::Globals::warning('deprecated', $thisline, "Inline state expressions are deprecated!  Use as-yet-unimplemented 'guard' blocks instead!")> 
              '(' <commit> Expression ')' 
                { $return = $item{Expression} } 
              | <error?> <reject> 
              | { $return = "true"; }


Update : { $thisparser->{local}{update} = 1; } MaceBlock[%arg](s)
UpdateWithErrors : <skip: qr{\s* ((/[*] .*? [*]/|(//[^\n]*\n)) \s*)*}sx> { $thisparser->{local}{update} = $arg[1]; } ServiceBlock(s?) EOFile { 1 }
| {
    print "HERE\n";
    my $lastLine = "";
    my $lastMsg = "";
    for my $i (@{$thisparser->{errors}}) {
	if ($i->[1] ne $lastLine || $i->[0] ne $lastMsg) {
	    Mace::Compiler::Globals::error('invalid syntax', $i->[1], $i->[0]);
	    $lastLine = $i->[1];
	    $lastMsg = $i->[0];
	}
    }
    $thisparser->{errors} = undef;
}

MaceBlock : 'mace' "$arg{type}" '{' MaceBlockBody '}' | 'mace' ('service'|'provides') BraceBlock

MaceBlockBody : ServiceBlock(s) ...'}'

################################################################################
# Properties
################################################################################

properties : PropertiesSection(s?) ...'}' | <error>
PropertiesSection : PropertiesSectionNames Block[rule=>$item{PropertiesSectionNames}] | <error>
PropertiesSectionNames : 'safety' | 'liveness' | <error>

safety : Property(s?) ...'}' {
  $thisparser->{'local'}{'service'}->push_safetyProperties(@{$item[1]});
  $return = 1;
} | <error>
liveness : Property(s?) ...'}' {
  $thisparser->{'local'}{'service'}->push_livenessProperties(@{$item[1]});
  $return = 1;
} | <error>

Property : <rulevar: $propname = "">
Property : Id ':' <commit>
{ 
  $propname = $item{Id};
#  print "Begun property $item{Id}\n";
  $thisparser->{local}{'propertyName'} = $item{Id}; 
  $thisparser->{local}{'count'} = 0; 
} 
GrandBExpression[lookahead=>';'] ';'
{
#  print "Finished property $item{Id}\n";
  $return = Mace::Compiler::Properties::Property->new(name=>$item{Id}, property=>$item{GrandBExpression});
}
| <error?:Invalid syntax in property $propname, see detailed error> <error>

GrandBExpression : <leftop: BExpression Join BExpression> ..."$arg{lookahead}"
  {
    my $lhs = shift(@{$item[1]});
    while(@{$item[1]}) {
      my $op = shift(@{$item[1]});
      my $rhs = shift(@{$item[1]});
      $lhs = Mace::Compiler::Properties::JoinExpression->new(lhs=>$lhs, rhs=>$rhs, type=>$op, methodName=>"$thisparser->{local}{'propertyName'}_JoinExpression_$thisparser->{local}{'count'}");
      $thisparser->{local}{'count'}++;
    }
    $return = $lhs;
  }
Join : '\or' {$return=0} 
  | '\and' {$return=1}
  | '\xor' {$return=2} 
  | '\implies' {$return=3}
  | '\iff' {$return=4}
  | <error>
BExpression : ...EquationLookahead <commit> Equation 
| ...BinaryOpLookahead <commit> BinaryBExpression 
| ...BBlockDelim <commit> BBlock 
| ...Quantifier <commit> Quantification 
| '\not' <commit> GrandBExpression[lookahead=>''] 
  {
    $return = Mace::Compiler::Properties::Equation->new(lhs=>Mace::Compiler::Properties::PropertyItem->new(null=>1), type=>"!", rhs=>$item{GrandBExpression}, methodName=>"$thisparser->{local}{'propertyName'}_Equation_$thisparser->{local}{'count'}");
    $thisparser->{local}{'count'}++;
  }
| SetVariable[boolean=>1, varType=>2] 
#| <error?> <error>

EquationLookahead : NonBExpression[varType=>2] EquationBinOp 
BinaryOpLookahead : SetVariable[varType=>2] (SetSetBinOp | ElementSetBinOp )
BinaryBExpression : ElementSetExpression | SetSetExpression

Equation : NonBExpression[varType=>2] Equality <commit> NonBExpression[varType=>2]
  {
    $return = Mace::Compiler::Properties::Equation->new(lhs=>$item[1], type=>$item[2], rhs=>$item[4], methodName=>"$thisparser->{local}{'propertyName'}_Equation_$thisparser->{local}{'count'}");
    $thisparser->{local}{'count'}++;
  }
| <error?> <reject> 

EquationBinOp : '==' | '>=' | '<=' | '!=' | '=' | '\neq' | '\leq' | '\geq' | '<' | '>' | <error>
SetSetBinOp : '\subset' | '\propersubset' | '\eq' | <error>
ElementSetBinOp : '\in' | '\notin' | <error>

NEquality : '==' <commit> <error: $item[1] not allowed, use = for equality>
| '>=' <commit> <error: $item[1] not allowed, use \\\geq> 
| '<=' <commit> <error: $item[1] not allowed, use \\\leq>
| '!=' <commit> <error: $item[1] not allowed, use \\\neq>
| '=' {$return=2}
| '\neq' {$return=5}
| '\leq' {$return=3}
| '\geq' {$return=1} 
| '<' {$return=4}   
| '>' {$return=0}

Equality : '==' <commit> <error: $item[1] not allowed, use = for equality>
| '>=' <commit> <error: $item[1] not allowed, use \\\geq>
| '<=' <commit> <error: $item[1] not allowed, use \\\leq>
| '!=' <commit> <error: $item[1] not allowed, use \\\neq>
| '=' {$return="=="}
| '\neq' {$return="!="}
| '\leq' {$return="<="}
| '\geq' {$return=">="} 
| '<'    
| '>'   

ElementSetExpression : SetVariable[noSet=>1, varType=>1] SetOp <commit> SetVariable[varType=>0]
{
  $return = Mace::Compiler::Properties::ElementSetExpression->new(lhs=>$item[1], type=>$item[2], rhs=>$item[4], methodName=>"$thisparser->{local}{'propertyName'}_ElementSetExpression_$thisparser->{local}{'count'}");
  $thisparser->{local}{'count'}++;
}
| <error?><error>
SetOp : '\in' {$return=1}
| '\notin' {$return=0}
| <error>
SetSetExpression : SetVariable[varType=>0] SetComparisons <commit> SetVariable[varType=>0]
{
  $return = Mace::Compiler::Properties::SetSetExpression->new(lhs=>$item[1], type=>$item[2], rhs=>$item[4], methodName=>"$thisparser->{local}{'propertyName'}_SetSetExpression_$thisparser->{local}{'count'}");
  $thisparser->{local}{'count'}++;
}
| <error?><error>
SetComparisons : '\subset' {$return=1}
| '\propersubset' {$return=2}
| '\eq' {$return=0}
| <error>

BBlock : BBlockDelim GrandBExpression[lookahead=>$item{BBlockDelim}] <commit> "$item{BBlockDelim}" { $return = $item{GrandBExpression} } | <error?> <reject>
BBlockDelim : '(' { $return = ')' } | '{' {$return = '}'}
Quantification : <commit> Quantifier Id '\in' SetVariable[varType=>0] ':' <uncommit> GrandBExpression[lookahead=>""] 
    {
      $return = $item{Quantifier};
      $return->varname($item{Id});
      $return->set($item{SetVariable});
      $return->expression($item{GrandBExpression});
    }
  | <error?> <reject>

Quantifier : '\forall' 
    {
      $return = Mace::Compiler::Properties::Quantification->new(type=>2,quantity=>'all', methodName=>"$thisparser->{local}{'propertyName'}_Quantification_$thisparser->{local}{'count'}");
      $thisparser->{local}{'count'}++;
    }
  | '\exists' 
    {
      $return = Mace::Compiler::Properties::Quantification->new(type=>1,quantity=>1, methodName=>"$thisparser->{local}{'propertyName'}_Quantification_$thisparser->{local}{'count'}");
      $thisparser->{local}{'count'}++;
    }
  | '\for' <commit> '{' NEquality '}' '{' Number '}' 
    {
      $return = Mace::Compiler::Properties::Quantification->new(type=>$item{NEquality},quantity=>$item{Number}, methodName=>"$thisparser->{local}{'propertyName'}_Quantification_$thisparser->{local}{'count'}");
      $thisparser->{local}{'count'}++;
    }
  | <error>

NonBExpressionOp : '+' | '-' | <error>

NonBExpression : <leftop: ParenNonBExpression[%arg] NonBExpressionOp ParenNonBExpression[%arg]> 
  {
    my $lhs = shift(@{$item[1]});
    while(@{$item[1]}) {
      my $op = shift(@{$item[1]});
      my $rhs = shift(@{$item[1]});
      $lhs = Mace::Compiler::Properties::NonBExpression->new(lhs=>$lhs, rhs=>$rhs, op=>$op);
      $thisparser->{local}{'count'}++;
    }
    $return = $lhs;
  }
| <error>

ParenNonBExpression : '(' <commit> NonBExpression[%arg] ')' { $return = $item{NonBExpression}; } | SetVariable[%arg] | <error>

SetVariable : '\nodes' <commit> <reject:$arg{varType}==1 or $arg{boolean}>
    {
      $return = Mace::Compiler::Properties::SetVariable->new(variable=>'\nodes', varType=>0);
    }
  | <error?> <reject>
#  | <reject:$arg{varType}==1 or $arg{boolean}> Var '*'
#    {
#      $return = Mace::Compiler::Properties::SetVariable->new(variable=>$item{Var}, varType=>0, closure=>1, methodName=>"$thisparser->{local}{'propertyName'}_Closure_$thisparser->{local}{'count'}");
#      $thisparser->{local}{'count'}++;
#    } 
  | Var[%arg] 
    {
      $return = $item{Var};
      #Mace::Compiler::Properties::SetVariable->new(variable=>$item{Var}, varType=>$arg{varType});
    } 
  | <reject:$arg{varType}==0 or $arg{boolean}> '|' <commit> SetVariable[varType=>0] '|'
    { #note varType doesn't totally capture it -- we in fact know it is neither a set nor a set element.
#      $return = Mace::Compiler::Properties::SetVariable->new(variable=>$item{Var}, varType=>2, cardinality=>1);
      $return = $item{SetVariable};
      $return->cardinality(1); #varType=>2
    } 
  | <error?> <reject>
  | '\null' <commit> <reject:$arg{varType}==0 or $arg{boolean}>
    {
      $return = Mace::Compiler::Properties::SetVariable->new(variable=>'\null', varType=>1);
    } 
  | <error>

MethodParams : '(' Var[varType=>2](s? /,/) ')' { for my $v (@{$item[2]}) { $v->inMethodParameter(1); }; $return = $item[2]; } | { $return = "NOT_METHOD"; }
SepTok : '.' | '->' | <error>
SubVar : SepTok '(' <commit> Var[varType=>1] ')' '*' 
  {
    $return = $item{Var};
    $return->closure(1);
    $return->prefix($item{SepTok});
    $return->methodName("$thisparser->{local}{'propertyName'}_Closure_$thisparser->{local}{'count'}");
    $thisparser->{local}{'count'}++;
  }
| SepTok Var[varType=>2] 
  {
    $return = $item{Var};
    $return->prefix($item{SepTok});
  }
| { $return = "NOT_SUBVAR" }
Var : Line Column ScopedId MethodParams SubVar
{
  my $r = Mace::Compiler::Properties::SetVariable->new(variable=>$item{ScopedId}, varType=>$arg{varType});
  if($item{SubVar} ne "NOT_SUBVAR") {
    $r->subvar($item{SubVar});
  }
  if($item{MethodParams} ne "NOT_METHOD") {
    $r->isMethodCall(1);
    $r->push_params(@{$item{MethodParams}});
  }
#  print "Line: $item{Line} Column: $item{Column} SetVariable: $item{ScopedId} varType: $arg{varType}\n";
  $return = $r;
}
| Number <reject:$arg{boolean}>
    {
      $return = Mace::Compiler::Properties::SetVariable->new(variable=>$item{Number}, varType=>$arg{varType});
    } 
| <error>

]; # MACE grammar

1;
