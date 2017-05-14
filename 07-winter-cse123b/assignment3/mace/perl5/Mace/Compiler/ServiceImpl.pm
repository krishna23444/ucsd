# 
# ServiceImpl.pm : part of the Mace toolkit for building distributed systems
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
package Mace::Compiler::ServiceImpl;

#TODO: hashState default implementation in ServiceClass!

use strict;
use Class::MakeMethods::Utility::Ref qw( ref_clone );
use Mace::Compiler::ClassCache;

use Class::MakeMethods::Template::Hash
    (
     'new' => 'new',

     #[[[The following structures are all storing state as its being parsed

     'string' => 'name',
     'string' => 'header',
     'array'  => 'provides',
     'string' => 'trace',
     'boolean' => 'macetime',
     'hash --get_set_items'   => 'selectors',
     'array_of_objects' => ["constants" => { class => "Mace::Compiler::Param" }],
     'array' => "constants_includes",
     'array_of_objects' => ["constructor_parameters" => { class => "Mace::Compiler::Param" }],
     'array_of_objects' => ["service_variables" => { class => "Mace::Compiler::ServiceVar" }],
     'array'  => 'states',
     'array_of_objects' => ['typedefs' => { class => "Mace::Compiler::TypeDef" }],
     'array_of_objects' => ["state_variables" => { class => "Mace::Compiler::Param" }],
     'array_of_objects' => ["timers" => { class => "Mace::Compiler::Timer" }],
     'array_of_objects' => ["auto_types" => { class => "Mace::Compiler::AutoType" }],
     'array_of_objects' => ["messages" => { class => "Mace::Compiler::AutoType" }],

     #These are the parsed Methods for remapping
     'array_of_objects' => ["usesDowncalls" => { class => "Mace::Compiler::Method" }],
     'array_of_objects' => ["usesUpcalls" => { class => "Mace::Compiler::Method" }],
     'array_of_objects' => ["implementsUpcalls" => { class => "Mace::Compiler::Method" }],
     'array_of_objects' => ["implementsDowncalls" => { class => "Mace::Compiler::Method" }],

     'array_of_objects' => ["routines" => { class => "Mace::Compiler::Method" }],
     'array_of_objects' => ["routineObjects" => { class => "Mace::Compiler::RoutineObject" }],
     'array_of_objects' => ["transitions" => { class => "Mace::Compiler::Transition" }],
     'number' => "transitionEnd",
     'array_of_objects' => ["safetyProperties" => { class => "Mace::Compiler::Properties::Property" }],
     'array_of_objects' => ["livenessProperties" => { class => "Mace::Compiler::Properties::Property" }],
     'string' => "filename",

     #End state from parse]]]

     'array' => 'ignores',

     'hash --get_set_items' => 'selectorVars',


     #These are the API methods this service is providing - upcalls and downcalls for transitions
     'array_of_objects' => ['providedMethods' => { class => "Mace::Compiler::Method" }],
     'array_of_objects' => ['usesHandlerMethods' => { class => "Mace::Compiler::Method" }],
     'array_of_objects' => ['timerMethods' => { class => "Mace::Compiler::Method" }],
     #These are the API methods this service is providing - public interface calls
     'array_of_objects' => ['providedMethodsAPI' => { class => "Mace::Compiler::Method" }],
     'array_of_objects' => ['usesHandlerMethodsAPI' => { class => "Mace::Compiler::Method" }],
     #These are the API methods this service is providing - private serial forms only
     'array_of_objects' => ['providedMethodsSerials' => { class => "Mace::Compiler::Method" }],
     'array_of_objects' => ['usesHandlerMethodsSerials' => { class => "Mace::Compiler::Method" }],

     #These are the aspect transition methods.
     'array_of_objects' => ["aspectMethods" => { class => "Mace::Compiler::Method" }],
     'array_of_objects' => ["onChangeVars" => { class => "Mace::Compiler::Param" }],

     'array' => 'usesHandlers',
     'hash_of_arrays' => ['usesHandlerNames' => 'string'],

     #These are the upcalls this service may make -- a unified list and a per-handler list
     'array_of_objects' => ['providedHandlerMethods' => { class => "Mace::Compiler::Method" }],
     'array_of_objects' => ['providedHandlers' => { class => "Mace::Compiler::ServiceClass" }],

     #These are the downcalls this service may make -- a unified list and a per-serviceclass list
     'array_of_objects' => ['usesClassMethods' => { class => "Mace::Compiler::Method" }],
     'hash_of_arrays' => ['usesClasses' => { class => "Mace::Compiler::Method" }],

     #This stack of guard methods is used during parsing to parse guard blocks
     'array_of_objects' => ['guards' => { class => 'Mace::Compiler::Guard' }],

     'object' => ['parser' => { class => "Parse::RecDescent" }],

     'string' => "origMacFile",
     'string' => "annotatedMacFile",
    );

sub toString {
    my $this = shift;
    my $s = 
	"Service: ".$this->name()."\n"
	    . "Provides: ".join(', ', $this->provides())."\n"
		. "Trace Level: ".$this->trace()."\n"
		    ;

    $s .= "Selectors { \n";
    while ( my ($selector, $selstring) = each(%{$this->selectors()}) ) {
	$s .= "  $selector = $selstring\n";
    }
    $s .= "}\n";

    $s .= "Constants { \n";
    $s .= join("", map { "  ".$_->toString().";\n" } $this->constants());
    $s .= "}\n";

    $s .= "Constructor Parameters { \n";
    $s .= join("", map { "  ".$_->toString().";\n" } $this->constructor_parameters());
    $s .= "}\n";

    $s .= "States { " . join(', ', $this->states()) . " }\n";

    $s .= "Service Variables { \n";
    $s .= join("", map { "  ".$_->toString().";\n" } $this->service_variables());
    $s .= "}\n";

    $s .= "Typedefs { \n";
    while ( my ($id, $type) = each(%{$this->typedefs()}) ) {
	$s .= "  $id => ".$type->toString()."\n";
    }
    $s .= "}\n";

    $s .= "State Variables { \n";
    $s .= join("", map { "  ".$_->toString(nodefaults => 1).";\n" } $this->state_variables());
    $s .= "}\n";

    $s .= "Auto Types { \n";
    $s .= join("", map { "  ".$_->toString().";\n" } $this->auto_types());
    $s .= "}\n";

    $s .= "Messages { \n";
    $s .= join("", map { "  ".$_->toString().";\n" } $this->messages());
    $s .= "}\n";

    $s .= "Uses Upcalls { \n";
    $s .= join("", map { "  ".$_->toString().";\n" } $this->usesUpcalls());
    $s .= "}\n";

    $s .= "Uses Downcalls { \n";
    $s .= join("", map { "  ".$_->toString().";\n" } $this->usesDowncalls());
    $s .= "}\n";

    $s .= "Implements Upcalls { \n";
    $s .= join("", map { "  ".$_->toString().";\n" } $this->implementsUpcalls());
    $s .= "}\n";

    $s .= "Implements Downcalls { \n";
    $s .= join("", map { "  ".$_->toString().";\n" } $this->implementsDowncalls());
    $s .= "}\n";

    $s .= "Routines { \n";
    $s .= join("", map { "  ".$_->toString().";\n" } $this->routines());
    $s .= "}\n";

    $s .= "Routine Objects { \n";
    $s .= join("", map { "  ".$_->toString().";\n" } $this->routineObjects());
    $s .= "}\n";

    $s .= "Transitions { \n";
    $s .= join("", map { "  ".$_->toString().";\n" } $this->transitions());
    $s .= "}\n";

    return $s;
}

sub printHFile {
    my $this = shift;
    my $outfile = shift;

    my $servicename = $this->name();

    my $r = Mace::Compiler::GeneratedOn::generatedOnHeader("$servicename header file");
    print $outfile $r;

    print $outfile <<END;

#ifndef ${servicename}_header
#define ${servicename}_header

END

    $this->printIncludesH($outfile);
    $this->printUsingH($outfile);
    $this->printIncludeBufH($outfile);

    print $outfile <<END;

    int ${servicename}_load_protocol();

END

    print $outfile <<END;
    namespace ${servicename}_namespace {
END
    #  $this->printConstantsH($outfile);
    $this->printSelectorConstantsH($outfile);
    $this->printAutoTypeForwardDeclares($outfile);
    $this->printTypeDefs($outfile);
    print $outfile "\nclass ${servicename}Service;\n";
    $this->printAutoTypes($outfile);
    $this->printService($outfile);
    print $outfile $this->generateMergeClasses();
    $this->printRoutineObjects($outfile);
    $this->printMessages($outfile);
    $this->printTimerClasses($outfile);
    print $outfile <<END;
    }
END

    print $outfile <<END;
#endif
END
	
}

sub printCCFile {
    my $this = shift;
    my $outfile = shift;

    my $servicename = $this->name();

    my $serviceVariableString = '';
    #  my $serviceVariableString = join("\n", map{my $sv=$_->service; qq{#include "$sv.h"
    #                                                                 using ${sv}_namespace::${sv}Service;}} $this->service_variables());
    my $timerDelete = join("\n", map{my $t = $_->name(); "delete &$t;"} $this->timers());
    my $modelCheckSafety = join("\n", map{"// ${\$_->toString()}
      ${\$_->toMethod(nostatic=>1, methodprefix=>$servicename.qq/Service::/)}"    } $this->safetyProperties);
    my $modelCheckLiveness = join("\n", map{"// ${\$_->toString()}
      ${\$_->toMethod(nostatic=>1, methodprefix=>$servicename.qq/Service::/)}"    } $this->livenessProperties);

    my $r = Mace::Compiler::GeneratedOn::generatedOnHeader("$servicename main source file");
    print $outfile $r;
    print $outfile <<END;
    //BEGIN Mace::Compiler::ServiceImpl::printCCFile
#include "mace.h"
#include "NumberGen.h"
#include "$servicename.h"
#include "$servicename-macros.h"
#include "Log.h"
#include "ServiceInitializer.h"
#include "ScopedLock.h"
#include "ScopedLog.h"
#include "ScopedSerialize.h"
#include "pip_includer.h"
#include "lib/MaceTime.h"

	bool operator==(const mace::map<int, mace::map<int, ${servicename}_namespace::${servicename}Service*>::const_iterator>::const_iterator& lhs, const mace::map<int, ${servicename}_namespace::${servicename}Service*>::const_iterator& rhs) {
	    return lhs->second == rhs;
	}
    bool operator==(const mace::map<int, ${servicename}_namespace::${servicename}Service*>::const_iterator& lhs, const mace::map<int, mace::map<int, ${servicename}_namespace::${servicename}Service*>::const_iterator>::const_iterator& rhs) {
	return lhs == rhs->second;
    }
    //bool operator==(const mace::map<MaceKey, mace::map<MaceKey, ${servicename}_namespace::${servicename}Service*>::const_iterator>::const_iterator& lhs, const MaceKey& rhs) {
	//  return lhs->first == rhs;
	//}
    //bool operator==(const MaceKey& lhs, const mace::map<MaceKey, mace::map<MaceKey, ${servicename}_namespace::${servicename}Service*>::const_iterator>::const_iterator& rhs) {
	//  return lhs == rhs->first;
	//}
    //bool operator==(const mace::map<MaceKey, ${servicename}_namespace::${servicename}Service*>::const_iterator& lhs, const MaceKey& rhs) {
	//  return lhs->first == rhs;
	//}
    //bool operator==(const MaceKey& lhs, const mace::map<MaceKey, ${servicename}_namespace::${servicename}Service*>::const_iterator& rhs) {
	//  return lhs == rhs->first;
	//}

    namespace ${servicename}_namespace {

        //Selector Constants
END
	    
    $this->printSelectorConstantsCC($outfile);

    print $outfile <<END;
	//Change Tracker (optional)
END
    $this->printChangeTracker($outfile);

    print $outfile <<END;

	//service variable includes and uses
	    $serviceVariableString

	    //Transition and Guard Implementations
END
	    
    $this->printTransitions($outfile);

    print $outfile <<END;

	//Auto Type Methods
END
	    
    $this->defineAutoTypeMethods($outfile);

    print $outfile <<END;

	//Routines
END
	    
    $this->printRoutines($outfile);

    print $outfile <<END;

	//Timer Demux (and utility timer)
END
	    
    $this->printTimerDemux($outfile);

    my $getMessageNameCases = join("\n", map{my $n = $_->name; qq{case ${n}::messageType: return "${servicename}::$n";}} $this->messages());
    my $getStateNameCases = join("\n", map{qq/case $_: return "${servicename}::$_";/} $this->states()); 
    my $traceStateChange = ($this->traceLevel() > 0)? q{Log::logf(selectorId, "FSM: state changed to %s\n", getStateName(state));}:"";
    my $printStateVars = join("\n", map { $_ . " __out << std::endl;" } (grep(/./, map { $_->toPrint("__out") } $this->state_variables())));
    my $printState_StateVars = join("\n", grep(/./, map { $_->toPrintState("__out") } $this->state_variables()));
    my $printScheduledTimers = join("\n", map { $_->toPrint("__out")." __out << std::endl;" } $this->timers());
    my $printState_ScheduledTimers = join("\n", map { $_->toPrintState("__out") } $this->timers());
    my $printLowerServices = join("\n", map { unless($_->intermediate()) {" << _".$_->name()} else {""} } $this->service_variables());
    my $printState_LowerServices = join("\n", map { $_->toPrintState("__out") } $this->service_variables());

    print $outfile <<END;

	//Constructors
END
	    
    $this->printConstructor($outfile);

    print $outfile <<END;

	//Destructor
	    ${servicename}Service::~${servicename}Service() {
		$timerDelete
		}

	//Auxiliary Methods (check_neighbors, dumpState, print, getMessageName, changeState, getStateName)
	    //TODO: check_neighbors
	    void ${servicename}Service::check_neighbors(uint64_t t) { }
	void ${servicename}Service::dumpState(LOGLOGTYPE& logger) const {
	    logger << "state_dump: " << *this << std::endl;
	    return;
	}
	void ${servicename}Service::print(std::ostream& __out) const {
	    __out << "state=" << getStateName(state) << std::endl;
	    $printStateVars
		$printScheduledTimers
		__out << std::endl;
	    if(_printLower) {
		__out $printLowerServices << std::endl;
	    }
	    return;
	}
	void ${servicename}Service::printState(std::ostream& __out) const {
	    __out << "state=" << getStateName(state) << std::endl;
	    $printState_StateVars
		$printState_ScheduledTimers
		__out << std::endl;
	    if(_printLower) {
		$printState_LowerServices
		}
	    return;
	}
	const char* ${servicename}Service::getMessageName(uint8_t messageType) const {
	    switch(messageType) {
		$getMessageNameCases
		  default: ASSERT(false); return "INVALID MESSAGE NUMBER";
	    }
	}
	void ${servicename}Service::changeState(state_type stateNum, int selectorId) {
	    _actual_state = stateNum;
	    $traceStateChange
	    }
	const char* ${servicename}Service::getStateName(state_type state) const {
	    switch(state) {
		$getStateNameCases
		  default: ASSERT(false); return "INVALID STATE NUMBER";
	    }
	}

	//API demux (provides -- registration methods, maceInit/maceExit special handling)
END
	    
    $this->printAPIDemux($outfile);
    $this->printAspectDemux($outfile);
    $this->printHandlerRegistrations($outfile);

    print $outfile <<END;

	//Handler demux (uses handlers)
END
	    
    $this->printHandlerDemux($outfile);

    print $outfile <<END;

	//Downcall helpers (uses)
END
	    
    $this->printDowncallHelpers($outfile);

    print $outfile <<END;

	//Upcall helpers (provides handlers)
END
	    
    $this->printUpcallHelpers($outfile);

    print $outfile <<END;

	//Serial Helper Demux
END
	    
    $this->printSerialHelperDemux($outfile);

    print $outfile <<END;

	//Model checking safety methods
	    $modelCheckSafety

	    //Model checking liveness methods
	    $modelCheckLiveness

	} // end namespace

	//END Mace::Compiler::ServiceImpl::printCCFile
END
}

sub printConstantsFile {
    my $this = shift;
    my $outfile = shift;

    my $servicename = $this->name();

    my $r = Mace::Compiler::GeneratedOn::generatedOnHeader("$servicename constants header file");
    print $outfile $r;

    my $constantsIncludes = join("", $this->constants_includes());

    print $outfile <<END;
#ifndef ${servicename}_constants_h
#define ${servicename}_constants_h

#include "MaceBasics.h"
$constantsIncludes

    namespace ${servicename}_namespace {
END

    $this->printConstantsH($outfile);

    print $outfile <<END;

    } //end namespace ${servicename}_namespace

#endif // ${servicename}_constants_h
END
}

sub printConstantsH {
    my $this = shift;
    my $outfile = shift;

    my $servicename = $this->name();

    print $outfile <<END;
    //BEGIN: Mace::Compiler::ServiceImpl::printConstantsH 
END

    foreach my $constant ($this->constants()) {
	my $conststr = $constant->toString();
	print $outfile <<END;
	    static const $conststr;
END
    }

    print $outfile <<END;
    //END: Mace::Compiler::ServiceImpl::printConstantsH 
END
}

sub printSelectorConstantsH {
    my $this = shift;
    my $outfile = shift;

    my $servicename = $this->name();

    print $outfile <<END;
    //BEGIN: Mace::Compiler::ServiceImpl::printSelectorConstantsH 
END

    while (my ($sv, $s) = each(%{$this->selectorVars()})) {
	#        const char selector_${sv}[] = $s;
	print $outfile <<END;
	    const std::string selector_${sv} = $s;
END
    }

    print $outfile <<END;
    //END: Mace::Compiler::ServiceImpl::printSelectorConstantsH 
END
}

sub printSelectorConstantsCC {
    my $this = shift;
    my $outfile = shift;

    my $servicename = $this->name();

    print $outfile <<END;
    //BEGIN: Mace::Compiler::ServiceImpl::printSelectorConstantsCC 
END

    while (my ($sv, $s) = each(%{$this->selectorVars()})) {
	#        const char selector_${sv}[] = $s;
	print $outfile <<END;
	    //const std::string selector_${sv};
	    const LogIdSet* ${servicename}Service::selectorId_${sv};
END
    }

    print $outfile <<END;
    //END: Mace::Compiler::ServiceImpl::printSelectorConstantsCC 
END
}

sub printIncludesH {
    my $this = shift;
    my $outfile = shift;

    my $servicename = $this->name();

    print $outfile <<END;
    //BEGIN: Mace::Compiler::ServiceImpl::printIncludesH 
END

    my $undefCurtime = "";
    #  if ($this->macetime()) {
    #      $undefCurtime = '#undef curtime';
    #  }

    print $outfile <<END;
#include "lib/mace.h"
#include "lib/mace_constants.h"
#include "lib/Util.h"
#include <map> //only include if can make upcalls
#include "lib/mace-macros.h"
    $undefCurtime
#include "lib/Log.h"
#include "lib/ScopedLog.h"
#include "${servicename}-constants.h"
END

    if (scalar(@{$this->auto_types()}) || scalar(@{$this->messages()})) {
	print $outfile <<END;
#include "lib/Serializable.h"
END
	if (scalar(@{$this->messages()})) {
	    print $outfile <<END;
#include "lib/Message.h" 
END
	}
    }

    if ($this->macetime()) {
	print $outfile <<END;
#include "lib/MaceTime.h"
END
    }

    foreach my $scProvided ($this->provides()) {
	print $outfile <<END;
#include "${scProvided}ServiceClass.h"
END
    }
    foreach my $scUsed ($this->service_variables()) {
	print $outfile $scUsed->returnSCInclude();
    }

    print $outfile <<END;
    //END: Mace::Compiler::ServiceImpl::printIncludesH 
END
}

sub printUsingH {
    my $this = shift;
    my $outfile = shift;

    print $outfile <<END;
    //BEGIN: Mace::Compiler::ServiceImpl::printUsingH 
END

    if (scalar(@{$this->auto_types()}) || scalar(@{$this->messages()})) {
	print $outfile <<END;
	    using mace::Serializable;
	    using mace::SerializationException;
END
	if (scalar(@{$this->messages()})) {
	    print $outfile <<END;
		    using mace::Message;
END
	}
    }

    if ($this->macetime()) {
	print $outfile <<END;
	using mace::MaceTime;
END
    }

    print $outfile <<END;
    //END: Mace::Compiler::ServiceImpl::printUsingH 
END

}

sub printIncludeBufH {
    my $this = shift;
    my $outfile = shift;

    print $outfile <<END;
    //BEGIN: Mace::Compiler::ServiceImpl::printIncludeBufH 
END

    print $outfile $this->header()."\n";

    print $outfile <<END;
    //END: Mace::Compiler::ServiceImpl::printIncludeBufH 
END

}

sub printAutoTypeForwardDeclares {
    my $this = shift;
    my $outfile = shift;

    print $outfile <<END;
    //BEGIN: Mace::Compiler::ServiceImpl::printAutoTypeForwardDeclares 
END

    foreach my $at ($this->auto_types()) {
	print $outfile "  ".$at->toForwardDeclare();
    }

    print $outfile <<END;
    //END: Mace::Compiler::ServiceImpl::printAutoTypeForwardDeclares 
END

}

sub printTypeDefs {
    my $this = shift;
    my $outfile = shift;

    print $outfile <<END;
    //BEGIN: Mace::Compiler::ServiceImpl::printTypeDefs 
END

    foreach my $td ($this->typedefs()) {
	print $outfile "  ".$td->toString()."\n";
    }

    print $outfile <<END;
    //END: Mace::Compiler::ServiceImpl::printTypeDefs 
END

}

sub printRoutineObjects {
    my $this = shift;
    my $outfile = shift;

    print $outfile <<END;
    //BEGIN: Mace::Compiler::ServiceImpl::printRoutineObjects 
END

    map { print $outfile $_->toString.";\n"; } $this->routineObjects();

    print $outfile <<END;
    //END: Mace::Compiler::ServiceImpl::printRoutineObjects 
END

}

sub printAutoTypes {
    my $this = shift;
    my $outfile = shift;

    print $outfile <<END;
    //BEGIN: Mace::Compiler::ServiceImpl::printAutoTypes 
END

    foreach my $at ($this->auto_types()) {
	print $outfile "  ".$at->toAutoTypeString(tracelevel=>$this->traceLevel())."\n";
    }

    print $outfile <<END;
    //END: Mace::Compiler::ServiceImpl::printAutoTypes 
END

}

sub defineAutoTypeMethods {
    my $this = shift;
    my $outfile = shift;

    print $outfile <<END;
    //BEGIN: Mace::Compiler::ServiceImpl::defineAutoTypeMethods 
END

    foreach my $at ($this->auto_types()) {
	print $outfile "  ".$at->defineAutoTypeMethods(scopedlog=>$this->traceLevel(), logparams=>$this->traceLevel())."\n";
    }

    print $outfile <<END;
    //END: Mace::Compiler::ServiceImpl::defineAutoTypeMethods 
END

}

sub printMessages {
    my $this = shift;
    my $outfile = shift;

    print $outfile <<END;
    //BEGIN: Mace::Compiler::ServiceImpl::printMessages 
END

    my $messagenum = 0;
    foreach my $at ($this->messages()) {
        $at->setNumber(\$messagenum);
	print $outfile "  ".$at->toMessageString($this->name())."\n";
    }

    print $outfile <<END;
    //END: Mace::Compiler::ServiceImpl::printMessages 
END

}

sub printTimerClasses {
    my $this = shift;
    my $outfile = shift;

    print $outfile <<END;
    //BEGIN: Mace::Compiler::ServiceImpl::printTimerClasses 
END

    foreach my $timer ($this->timers()) {
	print $outfile $timer->toString($this->name()."Service",
					traceLevel => $this->traceLevel())."\n";
    }

    print $outfile <<END;
    //END: Mace::Compiler::ServiceImpl::printTimerClasses 
END

}

sub matchStateChange {
    my $this = shift;
    my $l = shift;
    $$l =~ s|\bstate\s*=\s*(\w+)\s*;|state_change($1);|g;
    return $l;
}				# matchStateChange

sub printService {
    my $this = shift;
    my $outfile = shift;

    my $name = $this->name();
    my $statestring = 'enum state_type { '.join(',', $this->states())."};\n";
    my $transitionDeclarations = join("\n", map{$_->toTransitionDeclaration().";\n".$_->toGuardDeclaration().";"} $this->transitions());
    my $routineDeclarations = join("\n", map{$_->toString().";"} $this->routines());
    if ($this->traceLevel() > 1) {
      $routineDeclarations .= "\n".join("\n", grep(/./, map{if($_->returnType()->type() ne "void") { $_->toString(methodprefix=>"__mace_log_").";"}} $this->routines()));
    }
    my $stateVariables = join("\n", map{$_->toString(nodefaults => 1).";"} $this->state_variables(), $this->onChangeVars()); #nonTimer -> state_var
    my $messageDeclares = join("", map{$_->toForwardDeclare()} $this->messages());
    my $providedMethodDeclares = join("\n", map{$_->toString('nodefaults' => 1).";"} $this->providedMethodsAPI());
    my $usedHandlerDeclares = join("\n", map{$_->toString('nodefaults' => 1).";"} $this->usesHandlerMethodsAPI());
    my $serviceVars = join("\n", map{$_->toServiceVarDeclares()} $this->service_variables());
    my $constructorParams = join("\n", map{$_->toString('nodefaults' => 1).';'} $this->constructor_parameters());
    my $timerDeclares = join("\n", map{my $t = $_->name(); qq/ class ${t}_MaceTimer;\n${t}_MaceTimer &$t; /;} $this->timers());
    my $timerMethods .= join("\n", map{$_->toString().";"} $this->timerMethods());
    my $providesSerialDeclares = join("\n", map{$_->toString("noid" => 0).";"} $this->providedMethodsSerials());
    my $usesHandlersSerialDeclares = join("\n", map{$_->toString("noid" => 0).";"} $this->usesHandlerMethodsSerials());
    my $downcallHelperMethods = join("\n", map{$_->toString("methodprefix"=>'downcall_', "noid" => 0, "novirtual" => 1).";"} $this->usesClassMethods());
    my $upcallHelperMethods = join("\n", map{$_->toString('methodprefix'=>'upcall_', "noid" => 0, "novirtual" => 1).";"} $this->providedHandlerMethods());
    my $derives = join(", ", map{"public virtual $_"} (map{"${_}ServiceClass"} $this->provides() ), ($this->usesHandlers()) );
    my $constructor = $name."Service(".join(", ", (map{$_->serviceclass."ServiceClass& __".$_->name} grep(not($_->intermediate()), $this->service_variables)), (map{$_->type->toString()." _".$_->name} $this->constructor_parameters()) ).");";
    my $registrationDeclares = join("\n", map{my $n = $_->name(); "typedef std::map<int, $n* > maptype_$n;
                                                                 maptype_$n map_$n;"} $this->providedHandlers);
    my $changeTrackerDeclare = ($this->count_onChangeVars())?"class __ChangeTracker__;":"";
    my $changeTrackerFriend = ($this->count_onChangeVars())?"friend class __ChangeTracker__;":"";
    my $mergeDeclare = join("\n", map { "class $_;" } $this->mergeClasses());
    my $mergeFriend = join("\n", map { "friend class $_;" } $this->mergeClasses());
    my $autoTypeFriend = join("\n", map { my $n = $_->name(); "friend class $n;"} $this->auto_types());
    my $mergeMethods = $this->declareMergeMethods();
    my $onChangeDeclares = join("\n", map{$_->toString('nodefaults' => 1).";"} $this->aspectMethods());

    my $modelCheckSafety = join("\n", map{"// ${\$_->toString()}
                                         ${\$_->toMethodDeclare()}"    } $this->safetyProperties);
    my $modelCheckLiveness = join("\n", map{"// ${\$_->toString()}
                                           ${\$_->toMethodDeclare()}"    } $this->livenessProperties);
    my $callSafetyProperties = join("", map{"if(!${\$_->toMethodCall()}) { maceerr << \"Safety property ${\$_->name}: failed\" << Log::endl; return false; } else\n"} $this->safetyProperties()) . "{ maceout << \"Safety Properties: check\" << Log::endl; return true; }";
    my $callLivenessProperties = join("", map{"if(!${\$_->toMethodCall()}) { maceout << \"Liveness property ${\$_->name}: failed\" << Log::endl; return false; } else\n"} $this->livenessProperties()) . "{ maceout << \"Liveness Properties: check\" << Log::endl; return true; }";

    my $selectorIdInits = "";
    while (my ($sv, $s) = each(%{$this->selectorVars()})) {
	$selectorIdInits .= qq/
	    static const LogIdSet* selectorId_${sv}; /;
    }

    print $outfile <<END;
    //BEGIN: Mace::Compiler::ServiceImpl::printService 
END

    print $outfile <<END;
    $changeTrackerDeclare
	class ${name}Service : public BaseMaceService, public virtual mace::PrintPrintable, $derives
{
  private:
    $changeTrackerFriend
    $mergeFriend
    $autoTypeFriend
	int __inited;
  protected:
    $statestring

END
	
    print $outfile <<END;
  public:
    //Constructor
	$constructor
	//Destructor
	virtual ~${name}Service();

    $selectorIdInits

      protected:
	//XXX: Add CheckNeighbors where needed
	void check_neighbors(uint64_t t);
    void dumpState(LOGLOGTYPE& logger) const;
    const char* getStateName(state_type state) const;
    const char* getMessageName(uint8_t msgNum) const;
    void changeState(state_type stateNum, int selectorId);

  public:
    void print(std::ostream& logger) const;
    void printState(std::ostream& logger) const;

    //Provided Methods (calls into this service from a higher layer)
	$providedMethodDeclares

	//Used Handlers (calls into this service from a lower layer)
	$usedHandlerDeclares

      protected:
	state_type _actual_state;
    const state_type& state;
    //XXX: Do we still need fsm_hint?
	//XXX: Generate utility_timer_ as needed.

	//ServiceVariables and Registration UIDs
	$serviceVars

	//Aspect Methods
	$onChangeDeclares

	//Constructor Parameter (Variables)
	$constructorParams
	
	//Registration Typedefs and Maps
	$registrationDeclares

	//State Variables
	$stateVariables
	
	//Timer Vars
	$timerDeclares

	//Timer Methods
	$timerMethods

	//Message Declarations
	$messageDeclares
	
        //Merge Class Declarations
        $mergeDeclare

	//Downcall and Upcall helper methods
	$downcallHelperMethods
	$upcallHelperMethods

	//Serialized form Method Helpers
	$providesSerialDeclares
	$usesHandlersSerialDeclares

	//Transition and Guard Method Declarations
	$transitionDeclarations

	//Routines
	$routineDeclarations

        //Pre and Post Transition Demux Method Declarations
        $mergeMethods

      public:
	static bool checkSafetyProperties(const mace::map<int, ${name}Service*>& _nodes_) {
	    ADD_SELECTORS("${name}::checkSafetyProperties");
	    maceout << "Testing safety properties" << Log::endl;
	    $callSafetyProperties
	    }
    
    static bool checkLivenessProperties(const mace::map<int, ${name}Service*>& _nodes_) {
	ADD_SELECTORS("${name}::checkLivenessProperties");
	maceout << "Testing liveness properties" << Log::endl;
	$callLivenessProperties
	}
    
  protected:
    static mace::map<int, ${name}Service*>::const_iterator castNode(const mace::MaceKey& key, const mace::map<int, ${name}Service*>& _nodes_) {
	if(key.isNullAddress()) { return _nodes_.end(); }
	return _nodes_.find(key.getMaceAddr().local.addr-1);
    }

    static mace::map<int, ${name}Service*>::const_iterator castNode(const NodeSet::const_iterator& iter, const mace::map<int, ${name}Service*>& _nodes_) {
	if((*iter).isNullAddress()) { return _nodes_.end(); }
	return castNode(*iter, _nodes_);
    }

    static mace::map<int, ${name}Service*>::const_iterator castNode(const mace::map<int, mace::map<int, ${name}Service*>::const_iterator>::const_iterator& iter, const mace::map<int, ${name}Service*>& _nodes_) {
	return iter->second;
    }

    static mace::map<int, ${name}Service*>::const_iterator castNode(const mace::map<int, ${name}Service*>::const_iterator& iter, const mace::map<int, ${name}Service*>& _nodes_) {
	return iter;
    }

    //Model Checking Safety Properties
	$modelCheckSafety

	//Model Checking Liveness Properties
	$modelCheckLiveness
    };
END

    print $outfile <<END;
//END: Mace::Compiler::ServiceImpl::printService 
END

}

sub validate {
    my $this = shift;

    my $name = $this->name();
    $Mace::Compiler::Globals::MACE_TIME = $this->macetime();

    $this->push_states('init');
    $this->push_states('exited');
    $this->push_ignores('hashState');
    $this->push_ignores('maceReset');

    while (my ($t,$selector) = each (%{$this->selectors()})) {
	$selector =~ s/\$service/$name/g;
	$this->selectors($t, $selector);
    }
    if (! exists($this->selectors->{'default'})) {
	$this->selectors('default', qq/"${name}::\$function::\$state"/);
    }
    ;
    if (! exists($this->selectors->{'message'})) {
	$this->selectors('message', qq/"${name}::\$function::\$message::\$state"/);
    }
    ;

    my @provides = Mace::Compiler::ClassCache::getServiceClasses($this->provides());
    my %providesHash;
    for my $p (@provides) {
	if ($p->maceLiteral() and not $providesHash{$p->maceLiteral()}) {
            $providesHash{$p->maceLiteral()} = 1;
	    $Mace::Compiler::Grammar::text = $p->maceLiteral();
	    $this->parser()->Update($p->maceLiteral(), 0, "type" => "provides");
	}
    }

    my @providesMethods = map {$_->isVirtual(0); $_} (grep {$_->isVirtual()} Mace::Compiler::ClassCache::unionMethods(@provides));
    for my $m (@providesMethods) {
	if ($m->name eq "hashState") {
	    $m->body(qq{{
		static hash_string hasher;
		std::string s = toString();  
		macedbg(0) << s << Log::endl;
		return hasher(s);
	    }});
	    last;
	}
    }
    $this->providedMethods(@providesMethods);
    $this->providedMethodsAPI(@providesMethods);

    my @providesHandlers = Mace::Compiler::ClassCache::getHandlers($this->provides());
    my @providesHandlersMethods = Mace::Compiler::ClassCache::unionMethods(@providesHandlers);
    $this->providedHandlers(@providesHandlers);
    $this->providedHandlerMethods(@providesHandlersMethods);

    my %svClassHash;
    my %usesHandlersMap = ();
    for my $sv ($this->service_variables()) {
	my $sc = $sv->serviceclass();
	unless($sv->intermediate()) {
	    unless (defined($svClassHash{$sc})) {
		my @h = Mace::Compiler::ClassCache::getHandlers($sc);
		for my $h (@h) {
		    if ($sv->doRegister($h->name())) {
			$usesHandlersMap{$h->name()} = $h;
		    }
		}
		$svClassHash{$sc}=1; # XXX: track handlers!
	    }
	}
    }
    my @usesHandlers = values(%usesHandlersMap);
    
    my @serviceVarClasses = keys(%svClassHash);

    my @uses = Mace::Compiler::ClassCache::getServiceClasses(@serviceVarClasses);
    my %usesHash;
    for my $u (@uses) {
	if ($u->maceLiteral() and not $usesHash{$u->maceLiteral()}) {
            $usesHash{$u->maceLiteral()} = 1;
	    $Mace::Compiler::Grammar::text = $u->maceLiteral();
	    $this->parser()->Update($u->maceLiteral(), 0, "type" => "services");
	}
    }

    foreach my $message ($this->messages()) {
	$message->validateMessageOptions();
    }

    $this->sortByLine("typedefs");
    $this->sortByLine("auto_types");
#     my $atref = $this->auto_types();
#     my @sortedat = sort {
# 	if ($a->includedline() == $b->includedline()) {
# 	    return $a->line() <=> $b->line();
# 	}
# 	else {
# 	    return $a->includedline() <=> $b->includedline();
# 	}
#     } @$atref;
#     $this->auto_types(@sortedat);

    foreach my $autotype ($this->auto_types()) {
	$autotype->validateAutoTypeOptions();
    }

    foreach my $var ($this->state_variables()) {
	$var->validateTypeOptions({'serialize' => 0, 'recur' => 1});
    }

    for my $m ($this->constructor_parameters()) {
	$m->type()->set_isConst();
    }

    for my $timer ($this->timers()) {
	$timer->validateTypeOptions($this);
	my $v = Mace::Compiler::Type->new('type'=>'void');
	my $m = Mace::Compiler::Method->new('name' => "expire_".$timer->name, 'body' => '{ }', 'returnType' => $v);
	my $i = 0;
	for my $t ($timer->types()) {
	    my $dupet = ref_clone($t);
	    $dupet->set_isRef();
	    my $p = Mace::Compiler::Param->new(name=>"p$i", type=>$dupet);
	    $m->push_params($p);
	    $i++;
	}
	#$m->options('timer' => $timer->name, 'timerRecur' => $timer->recur(), 'transitions' => []);
	$m->options('timer' => $timer->name, 'timerRecur' => $timer->recur());

	$this->push_timerMethods($m);
    }
    
    #This portion reads in the methods as declared by the Provides and Services blocks

    #   my @usesHandlers = Mace::Compiler::ClassCache::getHandlers(@serviceVarClasses);
    $this->usesHandlers(map{$_->name} @usesHandlers);
    my @usesHandlersMethods = map {$_->isVirtual(0); $_} (grep {$_->isVirtual()} Mace::Compiler::ClassCache::unionMethods(@usesHandlers));
    $this->usesHandlerMethods(@usesHandlersMethods);
    $this->usesHandlerMethodsAPI(@usesHandlersMethods);

    my @usesMethods = grep(!($_->name =~ /^((un)?register.*Handler)|(mace(Init|Exit|Reset))|hashState$/), Mace::Compiler::ClassCache::unionMethods(@uses));
    $this->usesClassMethods(@usesMethods);

    for my $sv (@serviceVarClasses) {
	my @svc = Mace::Compiler::ClassCache::getServiceClasses($sv);
	my @svcm = Mace::Compiler::ClassCache::unionMethods(@svc);
	my @svcn = map { $_->name(); } @svc;
	$this->usesClasses_push($sv, @svcm);
	my @svch = Mace::Compiler::ClassCache::getHandlers($sv);
	my @svchn = map { $_->name() } @svch;
	$this->usesHandlerNames_push($sv, @svchn);
    }

    $Mace::Compiler::Globals::filename = $this->filename();

    #   print "dumping everything\n\n";
    #   for my $m ($this->usesDowncalls(), $this->usesUpcalls(), $this->implementsUpcalls(), $this->implementsDowncalls()) {
    #       print $m->toString(noline => 1) . "\n";
    #   }
    #   die "ayeee!!!";
    
    #This portion handles the method remappings block by removing pristine methods from state, 
    #placing them in a remapping list, and replacing them with ones using remapped types
    my $doGrep = 0;
    for my $method ($this->usesDowncalls()) {
	my $origmethod;
	unless(ref ($origmethod = Mace::Compiler::Method::containsTransition($method, $this->usesClassMethods()))) {
            if($method->options("mhdefaults")) {
              $method->options("delete", 1);
              $doGrep = 1;
            }
            else {
              Mace::Compiler::Globals::error("bad_method_remap", $method->line(), $origmethod);
            }
	    next;
	}
	$method->returnType($origmethod->returnType);
	$method->body($origmethod->body);
	$method->isConst($origmethod->isConst);
	my @op = $origmethod->params();
	for my $p ($method->params()) {
	    my $op = shift(@op);
	    unless($p->hasDefault) {
		$p->hasDefault($op->hasDefault);
		$p->default($op->default);
	    }
	    if ($p->hasDefault() 
		&& (!$op->hasDefault() 
		    || ($p->default ne $op->default) 
		   ) 
	       ) {
		$origmethod->options('remapDefault', 1);
		$op->flags('remapDefault', $p->default);
		if ($method->serialRemap) {
		    $method->options('remapDefault', 1);
		    $p->flags('remapDefault', $p->default);
		    $p->default($op->default);
		}
	    }
	    if ($p->name =~ /noname_(\d+)/) {
		$p->name($op->name);
	    }
	}
	$method->push_params(@op);
	if ($method->serialRemap) {
            my @m = (grep { $origmethod != $_ } @{$this->usesClassMethods()});
            if(scalar(@m)) {
              $this->usesClassMethods(@m);
            } else {
              $this->clear_usesClassMethods();
            }
	    my @serialForms = $method->getSerialForms("Message", map{$_->name()} $this->messages());
	    $this->push_usesClassMethods(@serialForms);
	}
    }
    if($doGrep) {
      my @m = grep(!$_->options("delete"), @{$this->usesDowncalls});
      if(scalar(@m)) {
        $this->usesDowncalls(@m);
      } else {
        $this->clear_usesDowncalls(@m);
      }
      $doGrep = 0;
    }
    for my $method ($this->usesUpcalls()) {
	my $origmethod;
	unless(ref ($origmethod = Mace::Compiler::Method::containsTransition($method, $this->providedHandlerMethods()))) {
            if($method->options("mhdefaults")) {
              $method->options("delete", 1);
              $doGrep = 1;
            }
            else {
              Mace::Compiler::Globals::error("bad_method_remap", $method->line(), $origmethod);
            }
	    next;
	}
	$method->returnType($origmethod->returnType);
	$method->body($origmethod->body);
	$method->isConst($origmethod->isConst);
	my @op = $origmethod->params();
	my $n = 0;
	for my $p ($method->params()) {
	    my $op = shift(@op);
	    unless($p->hasDefault) {
		$p->hasDefault($op->hasDefault);
		$p->default($op->default);
	    }
	    if ($p->hasDefault()) {
		unless ($op->hasDefault()) {
		    my $pstr = $p->toString(noline => 1);
		    my $ostr = $op->toString(noline => 1);
		    Mace::Compiler::Globals::error("bad_method_remap", $p->line(), "Cannot assign new default to arg $n [$pstr] because API [$ostr] has no default (see Mace/Compiler/ServiceImpl.pm [1])");
		    #[1] : Because we allow and regularly use non-const items as defaults
		    #here, we do not allow you to define default values for parameters
		    #which do not have default values in the API.  The actual
		    #implementation looks to see if the API default parameter is
		    #supplied, and if so, the default specified here is evaluated.  A
		    #"smarter" compiler could "trust" that you know what you are doing,
		    #and if there is no default, just use the one you supply (which will
		    #work as long as its a constant expression).
		    $n++;
		    next;
		}
		if ($p->default ne $op->default) {
		    $origmethod->options('remapDefault', 1);
		    $op->flags('remapDefault', $p->default);
		    if ($method->serialRemap) {
			$method->options('remapDefault', 1);
			$p->flags('remapDefault', $p->default);
			$p->default($op->default);
		    }
		}
	    }
	    if ($p->name =~ /noname_(\d+)/) {
		$p->name($op->name);
	    }
	    $n++;
	}
	$method->push_params(@op);
	if ($method->serialRemap) {
            my @m = (grep { $origmethod != $_ } @{$this->providedHandlerMethods()});
            if(scalar(@m)) {
              $this->providedHandlerMethods(@m);
            } else {
              $this->clear_providedHandlerMethods();
            }
	    my @serialForms = $method->getSerialForms("Message", map{$_->name()} $this->messages());
	    $this->push_providedHandlerMethods(@serialForms);
	}
    }
    if($doGrep) {
      my @m = grep(!$_->options("delete"), @{$this->usesUpcalls});
      if(scalar(@m)) {
        $this->usesUpcalls(@m);
      } else {
        $this->clear_usesUpcalls(@m);
      }
      $doGrep = 0;
    }
    for my $method ($this->implementsUpcalls()) {
	my $origmethod;
	unless(ref ($origmethod = Mace::Compiler::Method::containsTransition($method, $this->usesHandlerMethods()))) {
            if($method->options("mhdefaults")) {
              $method->options("delete", 1);
              $doGrep = 1;
            }
            else {
              Mace::Compiler::Globals::error("bad_method_remap", $method->line(), $origmethod);
            }
	    next;
	}
	$method->returnType($origmethod->returnType);
	$method->body($origmethod->body);
	$method->isConst($origmethod->isConst);
	my @op = $origmethod->params();
	for my $p ($method->params()) {
	    my $op = shift(@op);
	    unless($p->hasDefault) {
		$p->hasDefault($op->hasDefault);
		$p->default($op->default);
	    }
	    if ($p->name =~ /noname_(\d+)/) {
		$p->name($op->name);
	    }
	}
	if (scalar(@op)) {
	    $method->push_params(@op);
	}
	if ($method->serialRemap) {
            my @m = (grep { $origmethod != $_ } @{$this->usesHandlerMethods()});
            if(scalar(@m)) {
              $this->usesHandlerMethods(@m);
            } else {
              $this->clear_usesHandlerMethods();
            }
	    my @serialForms = $method->getSerialForms(map{$_->name()} $this->messages());
	    $this->push_usesHandlerMethods(@serialForms);
	    $this->push_usesHandlerMethodsSerials(@serialForms);
	}
    }
    if($doGrep) {
      my @m = grep(!$_->options("delete"), @{$this->implementsUpcalls});
      if(scalar(@m)) {
        $this->implementsUpcalls(@m);
      } else {
        $this->clear_implementsUpcalls(@m);
      }
      $doGrep = 0;
    }
    for my $method ($this->implementsDowncalls()) {
	my $origmethod;
	unless(ref ($origmethod = Mace::Compiler::Method::containsTransition($method, $this->providedMethods()))) {
            if($method->options("mhdefaults")) {
              $method->options("delete", 1);
              $doGrep = 1;
            }
            else {
              Mace::Compiler::Globals::error("bad_method_remap", $method->line(), $origmethod);
            }
	    next;
	}
	$method->returnType($origmethod->returnType);
	$method->body($origmethod->body);
	$method->isConst($origmethod->isConst);
	my @op = $origmethod->params();
	for my $p ($method->params()) {
	    my $op = shift(@op);
	    unless($p->hasDefault) {
		$p->hasDefault($op->hasDefault);
		$p->default($op->default);
	    }
	    if ($p->name =~ /noname_(\d+)/) {
		$p->name($op->name);
	    }
	}
	if ($method->serialRemap) {
            my @m = (grep { $origmethod != $_ } @{$this->providedMethods()});
            if(scalar(@m)) {
              $this->providedMethods(@m);
            } else {
              $this->clear_providedMethods();
            }
	    my @serialForms = $method->getSerialForms(map{$_->name()} $this->messages());
	    $this->push_providedMethods(@serialForms);
	    $this->push_providedMethodsSerials(@serialForms);
	}
    }
    if($doGrep) {
      my @m = grep(!$_->options("delete"), @{$this->implementsDowncalls});
      if(scalar(@m)) {
        $this->implementsDowncalls(@m);
      } else {
        $this->clear_implementsDowncalls(@m);
      }
    }

    for my $routine ($this->routines()) {
	my $selector = $this->selectors('default');
	my $fnName = $routine->name();
	$selector =~ s/\$function/$fnName/;
	$selector =~ s/\$state/(routine)/;
	$this->selectorVars("${fnName}_routine",$selector);
	$routine->options('selector', $selector);
	$routine->options('selectorVar', "${fnName}_routine");
	my $t = $routine->body();
	$this->matchStateChange(\$t);
	$routine->body($t);
    }

    #this code handles selectors and selectorVars for methods passed to demuxFunction
    my $demuxNum = 0;
    for my $method ($this->usesHandlerMethods(), $this->providedMethods(), $this->timerMethods(), $this->implementsUpcalls(), $this->implementsDowncalls()) {
	my $selector;
	if ($method->options('message')) {
	    $selector = $this->selectors('message');
	    my $msg = $method->options('message');
	    $selector =~ s/\$message/$msg/g;
	} else {
	    $selector = $this->selectors('default');
	}
	my $fnName = $method->name();
	$selector =~ s/\$function/$fnName/;
	$selector =~ s/\$state/(demux)/;
	my $selectorVar = "${fnName}_demux_$demuxNum";
	$this->selectorVars($selectorVar,$selector);
	$method->options('selector', $selector);
	$method->options('selectorVar', $selectorVar);
	$demuxNum++;
    }

    #this code handles selectors and selectorVars for methods passed to demuxFunction
    my $upcallNum = 0;
    for my $method ($this->providedHandlerMethods(), $this->usesUpcalls()) {
	my $selector;
	if ($method->options('message')) {
	    $selector = $this->selectors('message');
	    my $msg = $method->options('message');
	    $selector =~ s/\$message/$msg/g;
	} else {
	    $selector = $this->selectors('default');
	}
	my $fnName = $method->name();
	$selector =~ s/\$function/$fnName/;
	$selector =~ s/\$state/(upcall)/;
	my $selectorVar = "${fnName}_upcall_$upcallNum";
	$this->selectorVars($selectorVar,$selector);
	$method->options('selector', $selector);
	$method->options('selectorVar', $selectorVar);
	$upcallNum++;
    }

    #this code handles selectors and selectorVars for methods passed to demuxFunction
    my $downcallNum = 0;
    for my $method ($this->usesClassMethods(), $this->usesDowncalls()) {
	my $selector;
	if ($method->options('message')) {
	    $selector = $this->selectors('message');
	    my $msg = $method->options('message');
	    $selector =~ s/\$message/$msg/g;
	} else {
	    $selector = $this->selectors('default');
	}
	my $fnName = $method->name();
	$selector =~ s/\$function/$fnName/;
	$selector =~ s/\$state/(downcall)/;
	my $selectorVar = "${fnName}_downcall_$downcallNum";
	$this->selectorVars($selectorVar,$selector);
	$method->options('selector', $selector);
	$method->options('selectorVar', $selectorVar);
	$downcallNum++;
    }

    #This portion validates that transitions match some legal API -- must determine list of timer names before this block.
    my $transitionNum = 0;
    my $filepos = 0;
    foreach my $transition ($this->transitions()) {
	if ($transition->type() eq 'downcall') {
	    my $origmethod;
	    unless(ref ($origmethod = Mace::Compiler::Method::containsTransition($transition->method, $this->providedMethods()))) {
		Mace::Compiler::Globals::error("bad_transition", $transition->method->line(), $origmethod);
		next;
	    }
	    $this->fillTransition($transition, $origmethod);
	}
	elsif ($transition->type() eq 'upcall') {
	    my $origmethod;
	    unless(ref ($origmethod = Mace::Compiler::Method::containsTransition($transition->method, $this->usesHandlerMethods()))) {
		Mace::Compiler::Globals::error("bad_transition", $transition->method->line(), $origmethod);
		next;
	    }
	    $this->fillTransition($transition, $origmethod);
	}
	elsif ($transition->type() eq 'scheduler') {
	    my $origmethod;
	    $transition->method->name("expire_".$transition->method->name());
	    unless(ref ($origmethod = Mace::Compiler::Method::containsTransition($transition->method, $this->timerMethods()))) {
		Mace::Compiler::Globals::error("bad_transition", $transition->method->line(), $origmethod);
		next;
	    }
	    $this->fillTransition($transition, $origmethod);
	}
        elsif ($transition->type() eq 'aspect') {
	    my $origmethod;
	    $transition->method->name('aspect_'.$transition->method->name());
            unless(scalar(@{$transition->options('monitor')}) == scalar(@{$transition->method->params()})) {
              Mace::Compiler::Globals::error("bad_transition", $transition->method->line(), "aspect transition parameter mismatch -- same parameters as those to monitor (different number)"); #comparing aspect template count against function count
              next;
            }
            my $err = 0;
            my $pi = 0;
            my $mvars = $transition->options('monitor');
            my @mpars;
            for my $monitorVar (@$mvars) {
              #Find state variable for each montiored name
              my $stateType = Mace::Compiler::Type->new(type=>"state_type");
              my $stateParam = Mace::Compiler::Param->new(name=>"state", type=>$stateType);
              my @svar = grep($monitorVar eq $_->name, @{$this->state_variables()}, $stateParam);
              my $fsvar = $svar[0];
              if(scalar(@svar) != 1) {
                Mace::Compiler::Globals::error("bad_transition", $transition->method->line(), "aspect transition: variable $monitorVar cannot be found in state variables");
                $err = 1;
                last;
              } else {
                #put state variable on the monitor/shadow list
                push(@mpars, $fsvar);
                my @shvar = grep("_MA_".$transition->method->name()."_".$monitorVar eq $_->name, @{$this->onChangeVars()});
                unless(scalar(@shvar)) {
                  my $sv = ref_clone($fsvar);
                  $sv->name("_MA_".$transition->method->name()."_".$sv->name());
                  $sv->flags("originalVar", $fsvar);
                  $this->push_onChangeVars($sv);
                }
              }
              if($err) { $err=0; next; }
              #test found state variable against type from the method
              if($transition->method->params()->[$pi]->type()) {
                if($transition->method->params()->[$pi]->type->type ne $fsvar->type->type) {
                  Mace::Compiler::Globals::error("bad_transition", $transition->method->line(), "aspect transition: state variable $monitorVar has type ".$fsvar->type->type." and does not match parameter $pi: ".$transition->method->params()->[$pi]->toString());
                  next;
                }
                if(!$transition->method->params()->[$pi]->type->isConst() or !$transition->method->params()->[$pi]->type->isRef()) {
                  Mace::Compiler::Globals::error("bad_transition", $transition->method->line(), "aspect transition parameters must be const and ref parameters (parameter $pi: ".$transition->method->params()->[$pi]->toString().")");
                  next;
                }
              }
              $pi++;
            }
            
            if($transition->method->returnType->toString()) {
              unless($transition->method->returnType->type eq "void") {
		Mace::Compiler::Globals::error("bad_transition", $transition->method->line(), "aspect transitions must return 'void'");
		next;
              }
            } else {
              $transition->method->returnType(Mace::Compiler::Type->new(type => "void"));
            }
	    unless(ref ($origmethod = Mace::Compiler::Method::containsTransition($transition->method, $this->aspectMethods()))) {
              #add transition.
              my @othermatches;
              if(scalar(@othermatches = grep($_->name eq $transition->method->name, $this->aspectMethods))) {
                Mace::Compiler::Globals::error("bad_transition", $transition->method->line(), "aspect transition ".$transition->method->toString()." does not match other aspects with the same name.  Options are:\n".join("\n", @othermatches->toString()));
                next;
              }
              $origmethod = ref_clone($transition->method);
              $origmethod->options('monitor', $transition->options('monitor'));
              $origmethod->body("{ }\n");
              my $i = 0;
              for my $p ($origmethod->params()) {
                unless($p->type()) {
                  $p->type(ref_clone($mpars[$i]->type()));
                  $p->type->isConst(1);
                  $p->type->isRef(1);
                }
                $i++;
              }

              my $selector = $this->selectors('default');
              my $fnName = $origmethod->name();
              $selector =~ s/\$function/$fnName/;
              $selector =~ s/\$state/(demux)/;
              my $selectorVar = "${fnName}_aspect";
              $this->selectorVars($selectorVar,$selector);
              $origmethod->options('selector', $selector);
              $origmethod->options('selectorVar', $selectorVar);
        
              #$a->body("{ return; }"); #XXX What goes here?
              # XXX: This allows mutiple aspects of same name with different
              # parameter lists to be created, and one of each will execute.
              # If that's bad we need to do some extra checking.
              $this->push_aspectMethods($origmethod);
            }
	    $this->fillTransition($transition, $origmethod);
        }
	else {
	    my $ttype = $transition->type();
	    Mace::Compiler::Globals::error("bad_transition", $transition->method->line(), "Transition type '$ttype' invalid.  Expecting upcall/downcall/scheduler");
	}
	$transition->validate($transitionNum++, $this->selectors());
	$this->selectorVars($transition->getSelectorVar(), $transition->getSelector());

        if($transition->startFilePos() >= 0) {
          $this->annotatedMacFile($this->annotatedMacFile() . substr($this->origMacFile(), $filepos, $transition->startFilePos()-$filepos) . "//" . $transition->toString(noline=>1) . "\n" . (" " x ($transition->columnStart()-1)));
          $filepos = $transition->startFilePos();
        }

	my $t = $transition->method->body();
	$this->matchStateChange(\$t);
	$transition->method->body($t);

	if ($transition->isOnce()) {
	    my $once = "once_".$transition->getTransitionMethodName();
	    $this->push_state_variables(Mace::Compiler::Param->new(name => $once,
								   type => Mace::Compiler::Type->new(type => "bool"),
								   hasDefault => 1,
								   default => "false"));
	    $transition->unshift_guards(Mace::Compiler::Guard->new(guardStr => "(!$once)"));
	}
	
    }

    $this->annotatedMacFile($this->annotatedMacFile() . substr($this->origMacFile(), $filepos));

    for my $p (@{$this->safetyProperties()}, @{$this->livenessProperties()}) {
	$p->validate($this);
    }
}

sub fillTransition {
    my $this = shift;
    my $transition = shift;
    my $origmethod = shift;
    if ($transition->type() eq 'scheduler') {
	$transition->method->options('timer', $transition->method->name());
    }
    else {
	while (my ($k, $v) = each(%{$origmethod->options})) {
	    $transition->method->options($k, $v);
	}
    }

    my $merge = $transition->getMergeType();

    $transition->method->returnType($origmethod->returnType);
    $transition->method->isConst($origmethod->isConst);
    my $i = 0;
    for my $p ($transition->method->params()) {
      unless($p->type) {
        $p->type($origmethod->params()->[$i]->type());
      }
      $i++;
    }
    $origmethod->options($merge.'transitions', []) unless($origmethod->options($merge.'transitions'));

    push(@{$origmethod->options($merge.'transitions')}, $transition);
}

sub printTransitions {
    my $this = shift;
    my $outfile = shift;

    my $name = $this->name();

    print $outfile "//BEGIN Mace::Compiler::ServiceImpl::printTransitions\n";
    for my $t ($this->transitions()) {
	$t->printGuardFunction($outfile, "methodprefix" => "${name}Service::");
        my $onChangeVarsRef = $this->onChangeVars();
	$t->printTransitionFunction($outfile, "methodprefix" => "${name}Service::", onChangeVars => $onChangeVarsRef);
    }
    print $outfile "//END Mace::Compiler::ServiceImpl::printTransitions\n";
}

sub printRoutines {
    my $this = shift;
    my $outfile = shift;

    my $name = $this->name();

    print $outfile "//BEGIN Mace::Compiler::ServiceImpl::printRoutines\n";
    for my $r ($this->routines()) {
        my $under = "";
	my $selectorVar = $r->options('selectorVar');
	my $fnName = $r->name();
        my $shimroutine = "";
        if ($r->returnType->type() ne "void" and $this->traceLevel() > 1) {
          $shimroutine = $r->toString("methodprefix" => "${name}Service::", nodefaults => 1);
          my $paramlist = join(",", map{ $_->toString(notype=>1, nodefaults => 1) } $r->params());
          $shimroutine .= qq{
            {
              #define selector selector_$selectorVar
              #define selectorId selectorId_$selectorVar
              ADD_LOG_BACKING

              return mace::logVal(__mace_log_$fnName($paramlist), macecompiler(0), "Return value: ");

              #undef selector
              #undef selectorId
            }
          };
          $under = "__mace_log_";
        }
	my $routine = $r->toString("methodprefix" => "${name}Service::${under}", nodefaults => 1);
	my $body = $r->body;
	my $prep = "PREPARE_FUNCTION";
	if ($this->macetime()) {
	    $prep = "MaceTime _curtime = 0;";
	}
        my $logRoutine = "ADD_LOG_BACKING";
        if($this->traceLevel() > 1) {
          $logRoutine .= "\nScopedLog __scoped_log(selector, 0, selectorId->compiler, true, true, true, PIP);";
        }
        if ($this->traceLevel() > 1) {
            $logRoutine .= qq/\nif(!macecompiler(0).isNoop()) {/;
            $logRoutine .= qq/macecompiler(0) << "$fnName(" /;
            for my $p ($r->params()) {
                my $pname = $p->name();
                if (not $p->flags('message')) {
                    $logRoutine .= qq/<< "[$pname=";
                                                                   mace::printItem(macecompiler(0), &$pname, $pname);
                                                                   macecompiler(0) << "]" /;
                } elsif ($this->traceLevel() > 1) {
                    $logRoutine .= qq/<< "[$pname=" << $pname << "]" /;
                } else {
                    my $ptype = $p->type()->type();
                    $logRoutine .= qq/<< "[$pname=$ptype]" /;
                }
            }
            $logRoutine .= qq/<< ")" << Log::endl;
                                                       }
            /;
        }
	print $outfile <<END
            $shimroutine
	    $routine {
		#define selector selector_$selectorVar
		#define selectorId selectorId_$selectorVar
		$prep
		    $logRoutine
		    $body
		    #undef selector
		    #undef selectorId
		}
END
    }
    print $outfile "//END Mace::Compiler::ServiceImpl::printRoutines\n";
}

sub demuxMethod {
    my $this = shift;
    my $outfile = shift;
    my $m = shift;
    my $transitionType = shift;
    my $name = $this->name();

    my $routine = $m->toString("methodprefix" => "${name}Service::", "nodefaults" => 1);
    my $fnName = $m->name();

    my $selectorVar = $m->options('selectorVar');
    my $apiBody = qq{    #define selector selector_$selectorVar
	#define selectorId selectorId_$selectorVar
	ADD_LOG_BACKING
	};
    my $hasTransOptions = (defined($m->options("transitions")) or defined($m->options("pretransitions")) or defined($m->options("posttransitions")));
    if ($hasTransOptions || $m->name eq 'maceInit' || $m->name eq 'maceExit' || $m->name eq 'hashState' || $m->name eq 'maceReset') {
	$apiBody .= qq"ScopedLock __lock(agentlock);
	";
    }
    if ($hasTransOptions && $this->trace ne 'off') {
	my $traceg1 = ($this->traceLevel() > 1)?'true':'false';
	my $trace = ($this->traceLevel() > 0)?'true':'false';
	$apiBody .= qq{ScopedLog __scoped_log(selector, 0, selectorId->compiler, true, $traceg1, $trace, PIP);
		   };
    }
    if ($this->traceLevel() > 0) {
	my $logMethod = "";
	$logMethod = qq/if(!macecompiler(0).isNoop()) {/;
	$logMethod .= qq/macecompiler(0) << "$fnName(" /;
	for my $p ($m->params()) {
	    my $pname = $p->name();
	    if (not $p->flags('message')) {
		$logMethod .= qq/<< "[$pname=";
							       mace::printItem(macecompiler(0), &$pname, $pname);
							       macecompiler(0) << "]" /;
	    } elsif ($this->traceLevel() > 1) {
		$logMethod .= qq/<< "[$pname=" << $pname << "]" /;
	    } else {
		my $ptype = $p->type()->type();
		$logMethod .= qq/<< "[$pname=$ptype]" /;
	    }
	}
	$logMethod .= qq/<< ")" << Log::endl;
						   }
	/;
	$apiBody .= $logMethod;
    }
    my $apiTail = "";
    if ($m->name eq 'maceInit') {
	my $initServiceVars = join("\n", map{my $n = $_->name(); qq/
	    _$n.maceInit();
	    if ($n == -1) {
		$n  = NumberGen::Instance(NumberGen::HANDLER_UID)->GetVal();
	    }
					     /;
					 } grep(not($_->intermediate()), $this->service_variables()));
	my $registerHandlers = "";
	for my $sv ($this->service_variables()) {
	    my $svn = $sv->name();
	    for my $h ($this->usesHandlerNames($sv->serviceclass)) {
		if ($sv->doRegister($h)) {
		    if ($this->traceLevel() > 0) {
			$registerHandlers .= qq{macecompiler(0) << "Registering handler with regId " << $svn << " and type $h for service variable $svn" << Log::endl;
					    };
		    }
		    $registerHandlers .= qq{_$svn.registerHandler(($h&)*this, $svn);
					};
		}
	    }
	}

	$apiBody .= "
	if(__inited++ == 0) {
	    //TODO: start utility timer as necessary
		$initServiceVars
		$registerHandlers
		";
    }
    elsif ($m->name eq 'maceExit') {
	my $stopTimers = join("\n", map{my $t = $_->name(); "$t.cancel();"} $this->timers());
	my $exitServiceVars = join("\n", map{my $n = $_->name(); qq{_$n.maceExit();}} grep(not($_->intermediate()), $this->service_variables()));
	my $unregisterHandlers = "";
	for my $sv ($this->service_variables()) {
	    my $svn = $sv->name();
	    for my $h ($this->usesHandlerNames($sv->serviceclass)) {
		if ($sv->doRegister($h)) {
		    $unregisterHandlers .= qq{_$svn.unregisterHandler(($h&)*this, $svn);
					  };
		}
	    }
	}
	
	$apiBody .= "
	if(--__inited == 0) {
        ";
	$apiTail .= "
	    //TODO: stop utility timer as necessary
	    _actual_state = exited;
  	    $stopTimers
	    $unregisterHandlers
	    $exitServiceVars
            ";
    }
    elsif ($m->name eq 'maceReset') {
	my $stopTimers = join("\n", map{my $t = $_->name(); "$t.cancel();"} $this->timers());
	my $resetServiceVars = join("\n", map{my $n = $_->name(); qq{_$n.maceReset();}} grep(not($_->intermediate()), $this->service_variables()));
	my $clearHandlers = "";
	for my $h ($this->providedHandlers()) {
	    my $hname = $h->name();
	    $clearHandlers .= "map_${hname}.clear();\n";
	}

	my $resetVars = "";
	for my $var ($this->state_variables(), $this->onChangeVars()) {
	    if (!$var->flags("reset")) {
		next;
	    }
	    my $head = "";
	    my $tail = "";
	    my $init = $var->name();
	    my $depth = 0;
	    for my $size ($var->arraySizes()) {
		$head .= "for(int i$depth = 0; i$depth < $size; i$depth++) {\n";
		$init .= "[i$depth]";
		$tail .= "}\n";
	    }
	    $init .= " = " . $var->getDefault() . ";\n";
	    $resetVars .= "$head $init $tail";
	}

	$apiTail .= "
	//TODO: stop utility timer as necessary
	    _actual_state = init;
	    $stopTimers
	    $clearHandlers
	    $resetServiceVars
	    $resetVars
	    __inited = 0;
        ";
	
    }

    if (defined($m->options("pretransitions")) || defined($m->options("posttransitions"))) {
	$apiBody .= "Merge_" . $m->options("selectorVar") . " __merge(" .
	    join(", ", ("this", map{$_->name()} $m->params())) . ");\n";
    }
    
    if (defined $m->options('transitions')) {
	$apiBody .= qq/ if(state == exited) {
	    ${\$m->body()}
	} else 
	    /;
	$apiBody .= $this->checkGuardFireTransition($m, "transitions", "else");
	#TODO: Fell Through No Processing
    } elsif (!scalar(grep {$_ eq $m->name} $this->ignores() )) {
	$Mace::Compiler::Globals::filename = $this->filename();
        my $tname = $m->name;
        if($transitionType eq "scheduler") {
          $tname = substr($tname, 7);
        }
	Mace::Compiler::Globals::warning('undefined', $this->transitionEnd(), "Transition $transitionType ".$tname." not defined!");
        $this->annotatedMacFile($this->annotatedMacFile . "\n//Undefined transition: $transitionType ".$m->toString(noline=>1, nodefaults=>1, methodname=>$tname)." { }\n");
	my $mn = $m->name;
	if ($this->traceLevel() > 0) {
	    $apiBody .= qq{macecompiler(1) << "COMPILER RUNTIME NOTICE: $mn called, but not implemented" << Log::endl;\n};
	}
    }
    my $resched = "";
    my $reschedEnd = "";
    if ($m->options('timer')) {
	my $timer = $m->options('timer');
	#TODO Pip Stuff
	if ($m->options('timerRecur')) {
	    my $recur = $m->options('timerRecur');
	    my $pstring = join("", map{", ".$_->name} $m->params());
	    $resched = "{\n";
	    $resched .= qq~ $timer.reschedule($recur$pstring);
		      ~;
	    $reschedEnd = "\n}\n";
	}
    }
    $apiBody .= $resched;
    $apiBody .= $m->body();
    $apiBody .= $reschedEnd;
    $apiBody .= "\n";
    $apiBody .= $apiTail;
    if ($m->name eq 'maceInit' || $m->name eq 'maceExit') {
	$apiBody .= "\n}\n";
    }
    print $outfile "
	$routine {
	    $apiBody
		#undef selector
		#undef selectorId
	    }
    ";
    for my $el ("pre", "post") {
	if ($m->options($el . "transitions")) {
	    my $r = $m->toString("methodprefix" => "${name}Service::_${el}_", "nodefaults" => 1);
	    print $outfile "$r {\n";
	    print $outfile $this->checkGuardFireTransition($m, "${el}transitions");
	    print $outfile "}\n";
	}
    }
}

sub mergeClasses {
    my $this = shift;
    my @r = ();
    for my $m ($this->providedMethods(), $this->usesHandlerMethods(), $this->timerMethods()) {
	if ($m->options("pretransitions") or $m->options("posttransitions")) {
	    my $name = "Merge_" . $m->options("selectorVar");
	    push(@r, $name);
	}
    }
    return @r;
}

sub generateMergeClasses {
    my $this = shift;
    my $c = "//BEGIN Mace::Compiler::ServiceImpl::generateMergeClasses\n";
    for my $m ($this->providedMethods(), $this->usesHandlerMethods(), $this->timerMethods()) {
	if ($m->options("pretransitions") or $m->options("posttransitions")) {
            my $svName = $this->name();
	    my $name = "Merge_" . $m->options("selectorVar");
	    $c .= "class ${svName}Service::$name {\n";
	    $c .= "private:\n";
	    $c .= $this->name() . "Service* sv;\n";
	    $c .= join("\n", map {$_->toString(nodefaults=>1) . ";"} $m->params()) . "\n";
	    $c .= "public:\n";
	    $c .= "$name(" . join(", ", ($this->name() . "Service* sv", map{$_->toString} $m->params())) . ") : ";
	    $c .= join(", ", ("sv(sv)", map{$_->name . "(" . $_->name . ")"} $m->params())) . " {\n";
	    if ($m->options("pretransitions")) {
		$c .= "sv->_pre_" . $m->name() . "(" . join(",", map{$_->name()} $m->params()) . ");\n";
	    }
	    $c .= "}\n";
	    $c .= "virtual ~$name() {\n";
	    if ($m->options("posttransitions")) {
		$c .= "sv->_post_" . $m->name() . "(" . join(",", map{$_->name()} $m->params()) . ");\n";
	    }
	    $c .= "}\n";
	    $c .= "}; // class $name\n\n";
	}
    }
    $c .= "//END Mace::Compiler::ServiceImpl::generateMergeClasses\n";
    return $c;
}

sub declareMergeMethods {
    my $this = shift;
    my $r = "";
    for my $m ($this->providedMethods(), $this->usesHandlerMethods(), $this->timerMethods()) {
	for my $el ("pre", "post") {
	    if ($m->options("${el}transitions")) {
		$r .= $m->toString("methodprefix" => "_${el}_", "nodefaults" => 1) . ";\n";
	    }
	}
    }
    return $r;
}

sub checkGuardFireTransition {
    my $this = shift;
    my $m = shift;
    my $key = shift;
    my $else = shift || "";

    my $r = "";
    map {
	my $guardCheck = $_->getGuardMethodName()."(".join(",", map{$_->name} $m->matchedParams($_->method)).")";
	my $transitionCall = $_->getTransitionMethodName()."(".join(",", map{$_->name} $m->matchedParams($_->method)).")";
	my $setOnce = "";
	if ($_->isOnce()) {
	    $setOnce = "once_".$_->getTransitionMethodName()." = true;";
	}
	my $return = '';
	my $returnend = '';
	if (!$m->returnType->isVoid()) {
            if($this->traceLevel > 1) {
              $return = 'return mace::logVal(';
              $returnend = ', macecompiler(0), "Return Value: ")';
            }
            else {
              $return = 'return';
            }
	}

	my $resched = "";
	if ($m->options('timer')) {
	    my $timer = $m->options('timer');
	    #TODO Pip Stuff
	    if ($m->options('timerRecur')) {
		my $recur = $m->options('timerRecur');
		my $pstring = join("", map{", ".$_->name} $m->params());
		$resched = qq{$timer.reschedule($recur$pstring);
			   };
	    }
	}

	$r .= qq{ if($guardCheck) {
		$setOnce
		    $resched
		    $return $transitionCall$returnend;
	    } $else
	    };
    } @{$m->options($key)};

    return $r;
}

sub printAPIDemux {
    my $this = shift;
    my $outfile = shift;

    my $name = $this->name();

    print $outfile "//BEGIN Mace::Compiler::ServiceImpl::printAPIDemux\n";
    for my $m (grep(!($_->name =~ /^(un)?register.*Handler$/), $this->providedMethods())) {
	$this->demuxMethod($outfile, $m, "downcall");
    }
    print $outfile "//END Mace::Compiler::ServiceImpl::printAPIDemux\n";
}

sub printAspectDemux {
    my $this = shift;
    my $outfile = shift;

    my $name = $this->name();

    print $outfile "//BEGIN Mace::Compiler::ServiceImpl::printAspectDemux\n";
    for my $m ($this->aspectMethods()) {
	$this->demuxMethod($outfile, $m, "aspect");
    }
    print $outfile "//END Mace::Compiler::ServiceImpl::printAspectDemux\n";
}

sub printHandlerRegistrations {
    my $this = shift;
    my $outfile = shift;

    my $name = $this->name();

    print $outfile "//BEGIN Mace::Compiler::ServiceImpl::printHandlerRegistrations\n";
    for my $h ($this->providedHandlers()) {
	my $hname = $h->name();
	print $outfile <<END;
	registration_uid_t ${name}Service::registerHandler(${hname}& handler, registration_uid_t regId) {
	    if(regId == -1) { regId = NumberGen::Instance(NumberGen::HANDLER_UID)->GetVal(); }
            ASSERT(map_${hname}.find(regId) == map_${hname}.end());
	    map_${hname}[regId] = &handler;
	    return regId;
	}

	void ${name}Service::registerUniqueHandler(${hname}& handler) {
	    map_${hname}[-1] = &handler;
	}

	void ${name}Service::unregisterHandler(${hname}& handler, registration_uid_t regId) {
	    ASSERT(map_${hname}[regId] == &handler);
	    map_${hname}.erase(regId);
	}
END
    }
    print $outfile "//END Mace::Compiler::ServiceImpl::printHandlerRegistrations\n";
}

sub printHandlerDemux {
    my $this = shift;
    my $outfile = shift;

    my $name = $this->name();

    print $outfile "//BEGIN Mace::Compiler::ServiceImpl::printHandlerDemux\n";
    for my $m ($this->usesHandlerMethods()) {
	$this->demuxMethod($outfile, $m, "upcall");
    }
    print $outfile "//END Mace::Compiler::ServiceImpl::printHandlerDemux\n";
}

sub printTimerDemux {
    my $this = shift;
    my $outfile = shift;

    my $name = $this->name();

    print $outfile "//BEGIN Mace::Compiler::ServiceImpl::printTimerDemux\n";
    for my $m ($this->timerMethods()) {
	$this->demuxMethod($outfile, $m, "scheduler");
    }
    print $outfile "//END Mace::Compiler::ServiceImpl::printTimerDemux\n";
}

sub printChangeTracker {
    my $this = shift;
    my $outfile = shift;

    print $outfile "//BEGIN Mace::Compiler::ServiceImpl::printChangeTracker\n";
    if ($this->count_onChangeVars()) {

	my $name = $this->name();
        my $ctVarsCheck = "";
        for my $asp ($this->aspectMethods()) {
          my $aname = $asp->name();
          $ctVarsCheck .= "if(" . join("||", map{ "(sv->$_ != sv->_MA_${aname}_$_)" } @{$asp->options('monitor')}) . ") {
                            somethingChanged = true;
                            sv->$aname(".join(",", map { "sv->_MA_${aname}_$_" } @{$asp->options('monitor')}).");
                            ".join("\n", map{ "if(sv->$_ != sv->_MA_${aname}_$_) { sv->_MA_${aname}_$_ = sv->$_; } " } @{$asp->options('monitor')})."
                           }
                           ";
        }


	print $outfile <<END;
	class __ChangeTracker__ {
	  private:
	    //Pointer to the service for before/after inspection
		${name}Service* sv;
	    //Copies of the variables initial conditions for 'var' method variables
	      public:
		__ChangeTracker__(${name}Service* service) : sv(service) {}
	    ~__ChangeTracker__() {
              bool somethingChanged = false;
              do { 
                somethingChanged = false;
		$ctVarsCheck
              } while(somethingChanged);
		}
	};
END
    }
    print $outfile "//END Mace::Compiler::ServiceImpl::printChangeTracker\n";
}

sub printSerialHelperDemux {
    my $this = shift;
    my $outfile = shift;

    my $name = $this->name();

    print $outfile "//BEGIN Mace::Compiler::ServiceImpl::printSerialHelperDemux\n";
    for my $m ($this->implementsUpcalls(), $this->implementsDowncalls()) {
	if ($m->serialRemap) {
	    $this->demuxSerial($outfile, $m);
	}
    }
    print $outfile "//END Mace::Compiler::ServiceImpl::printSerialHelperDemux\n";
}

sub demuxSerial {
    my $this = shift;
    my $outfile = shift;
    my $m = shift;
    my $name = $this->name();

    my $routine = $m->toString("methodprefix" => "${name}Service::", "nodefaults" => 1);
    my $fnName = $m->name();
    my $selectorVar = $m->options('selectorVar');

    my $return = '';
    if (!$m->returnType->isVoid()) {
	$return = 'return';
    }

    my $apiBody = qq{//TODO: try/catch Serialize
		     };
    if (defined $this->trace ne 'off') {
	#TODO: LESS LOCKING!
        $apiBody .= qq{#ifdef PIP_MESSAGING\nScopedLock __lock(agentlock);\n#endif
        };
	my $traceg1 = ($this->traceLevel() > 1)?'true':'false';
	my $trace = ($this->traceLevel() > 0)?'true':'false';
	$apiBody .= qq{ScopedLog __scoped_log(selector, 0, selectorId->compiler, true, $traceg1, $trace, PIP);
		   };
    }
    for my $p ($m->params()) {
	if ($p->typeSerial and $p->typeSerial->type ne 'Message') {
	    my $typeSerial = $p->typeSerial()->type();
	    my $pname = $p->name();
	    $apiBody .= qq{ $typeSerial ${pname}_deserialized;
			    mace::deserializeStr(${pname}, &${pname}_deserialized, ${pname}_deserialized);
			};
	}
    }
    my $msgDeserialize = "$return $fnName(".join(",", map{$_->name.(($_->typeSerial)?"_deserialized":"")}$m->params()).");\n";
    for my $p ($m->params()) {
	if ($p->typeSerial and $p->typeSerial->type eq 'Message') {
	    my $pname = $p->name();
	    my $msgTraceNum = "//TODO -- trace num\n";
	    my $msgTrace = "//TODO -- trace msg\n";
	    my $body = $m->body();
	    my $msgDeserializeTmp = " std::istringstream in_$pname($pname);
					uint8_t msgNum_$pname;
					mace::deserialize(in_$pname, &msgNum_$pname, msgNum_$pname);
					in_$pname.seekg(0); 
					$msgTraceNum
					switch(msgNum_$pname) {
					";
	    for my $msg ($this->messages()) {
		my $msgName = $msg->name;
		$msgDeserializeTmp .= qq{
		    case ${msgName}::messageType: {
			${msgName} ${pname}_deserialized;
			mace::deserialize(in_$pname, &${pname}_deserialized, ${pname}_deserialized);
			$msgTrace 
			    $msgDeserialize
			}
		    break;
		};
	    }
	    $msgDeserializeTmp .= qq{ default: {
		macecompiler(1) << "FELL THROUGH NO PROCESSING -- INVALID MESSAGE NUMBER" << Log::endl;
		$body
		    ASSERT(0);
	    }
	      break;
	  };
	    $msgDeserializeTmp .= "}\n";
	    $msgDeserialize = $msgDeserializeTmp;
	}
    }
    $apiBody .= $msgDeserialize;
    unless($m->returnType->isVoid()) {
	$apiBody .= "\nASSERT(0);\n";
    }
    
    print $outfile "
	$routine {
	    #define selector selector_$selectorVar
	    #define selectorId selectorId_$selectorVar
	    ADD_LOG_BACKING
		$apiBody
		#undef selector
		#undef selectorId
	    }
        ";
}

sub printDowncallHelpers {
    my $this = shift;
    my $outfile = shift;

    my $name = $this->name();

    print $outfile "//BEGIN Mace::Compiler::ServiceImpl::printDowncallHelpers\n";
    for my $m ($this->usesClassMethods()) {
	my $origmethod = $m;
	my $routine = $m->toString("methodprefix"=>"${name}Service::downcall_", "noid" => 0, "novirtual" => 1, "nodefaults" => 1);
	my $serialize = "";
	my $defaults = "";
	if ($m->options('original')) {
	    #TODO: try/catch Serialization
	    $origmethod = $m->options('original');
	    my @oparams = $origmethod->params();
	    for my $p ($m->params()) {
		my $op = shift(@oparams);
		if (! $op->type->eq($p->type)) {
		    my $optype = $op->type->type();
		    my $opname = $op->name;
                    my $ptype = $p->type->type();
		    my $pname = $p->name;
		    $serialize .= qq{ $optype $opname;                                    
                                      ScopedSerialize<$optype, $ptype> ss($opname, $pname);
				  };
		}
	    }
	}
	if ($m->options('remapDefault')) {
	    for my $p ($m->params()) {
		if ($p->flags('remapDefault')) {
		    my $pn = $p->name();
		    my $pd = $p->default();
		    my $pd2 = $p->flags('remapDefault');
		    $defaults .= qq{ if($pn == $pd) { $pn = $pd2; }
				 };
		}
	    }
	}
	my @matchedServiceVars;
	for my $sv ($this->service_variables) {
	    if (not $sv->intermediate() and ref Mace::Compiler::Method::containsTransition($origmethod, $this->usesClasses($sv->serviceclass))) {
		push(@matchedServiceVars, $sv);
	    }
	}
	if (scalar(@matchedServiceVars) == 1) {
	    my $rid = $m->params()->[-1]->name();
	    my $svn = $matchedServiceVars[0]->name();
	    $defaults .= qq{ if($rid == -1) { $rid = $svn; }
			 };
	}
	my $callString = "";
	for my $sv (@matchedServiceVars) {
	    my $rid = $m->params()->[-1]->name();
	    my $callm = $origmethod->name."(".join(",", map{$_->name} $origmethod->params()).")";
	    my $svname = $sv->name;
	    my $return = (!$m->returnType->isVoid())?"return":"";
	    $callString .= qq{if($rid == $svname) {
		$return _$svname.$callm;
	    } else 
	    };
	}
	my $selectorVar = $m->options('selectorVar');
	#TODO: Logging, etc.
	$callString .= qq{{ ASSERT(0); }};
	my $trace = $this->traceLevel() > 0 ? "true" : "false";
	my $traceg1 = $this->traceLevel() > 1 ? "true" : "false";
	my $logMethod = "";
	if ($this->traceLevel() > 0) {
	    my $fnName = $origmethod->name();
	    $logMethod = qq/if(!macecompiler(0).isNoop()) {/;
	    $logMethod .= qq/macecompiler(0) << "$fnName(" /;
	    for my $p ($m->params()) {
		my $pname = $p->name();
		if (not $p->flags('message')) {
		    $logMethod .= qq/<< "[$pname=";
								   mace::printItem(macecompiler(0), &$pname, $pname);
								   macecompiler(0) << "]" /;
		} elsif ($this->traceLevel() > 1) {
		    $logMethod .= qq/<< "[$pname=" << $pname << "]" /;
		} else {
		    my $ptype = $p->type()->type();
		    $logMethod .= qq/<< "[$pname=$ptype]" /;
		}
	    }
	    $logMethod .= qq/<< ")" << Log::endl;
						       }
	    /;
	}
	print $outfile <<END;
	$routine {
	    #define selector selector_$selectorVar
	    #define selectorId selectorId_$selectorVar
	    ADD_LOG_BACKING
		ScopedLog __scoped_log(selector, 0, selectorId->compiler, true, $traceg1, $trace, PIP);
	    $logMethod
		$serialize
		$defaults
		$callString
		#undef selector
		#undef selectorId
	    }
END
    }
    print $outfile "//END Mace::Compiler::ServiceImpl::printDowncallHelpers\n";
}

sub printUpcallHelpers {
    my $this = shift;
    my $outfile = shift;

    my $name = $this->name();

    print $outfile "//BEGIN Mace::Compiler::ServiceImpl::printUpcallHelpers\n";
    for my $m ($this->providedHandlerMethods()) {
	my $origmethod = $m;
	my $routine = $m->toString("methodprefix"=>"${name}Service::upcall_", "noid" => 0, "novirtual" => 1, "nodefaults" => 1);
	my $serialize = "";
	my $defaults = "";
	if ($m->options('original')) {
	    #TODO: try/catch Serialization
	    $origmethod = $m->options('original');
	    my @oparams = $origmethod->params();
	    for my $p ($m->params()) {
		my $op = shift(@oparams);
		if (! $op->type->eq($p->type)) {
		    my $optype = $op->type->toString();
		    my $opname = $op->name;
		    my $pname = $p->name;
		    $serialize .= qq{ $optype $opname;                                    
				      mace::serialize($opname, &$pname, $pname);
				  };
		}
	    }
	}
	if ($m->options('remapDefault')) {
	    for my $p ($m->params()) {
		if ($p->flags('remapDefault')) {
		    my $pn = $p->name();
		    my $pd = $p->default();
		    my $pd2 = $p->flags('remapDefault');
		    $defaults .= qq{ if($pn == $pd) { $pn = $pd2; }
				 };
		}
	    }
	}
	my $callString = "";
	my @handlerArr = @{$origmethod->options('class')};
	unless(scalar(@handlerArr) == 1) {
	    Mace::Compiler::Globals::error("ambiguous_upcall", $m->line(), "Too many possible Handler types for this method (see Mace::Compiler::ServiceImpl [2])");
	    #[2] : In the present implementation, if an upcall could map to more than
	    #one handler type, we do not support it.  In theory, we could have a
	    #stratgegy where we search the upcall maps in a given order, upcalling to
	    #the first one we find.  Especially since if they had the same rid, they
	    #should refer to the same bond at least.  However, for simplicity, for
	    #now we just drop it as an error.
	    next;
	}
	my $handler = shift(@handlerArr);
	my $hname = $handler->name;
	my $mname = $m->name;
	my $body = $m->body;
	my $rid = $m->params()->[-1]->name();
	my $return = '';
	if (!$m->returnType->isVoid()) {
	    $return = 'return';
	}
	my $callm = $origmethod->name."(".join(",", map{$_->name} $origmethod->params()).")";
	my $iterator = "iterator";
	if ($m->isConst()) {
	    $iterator = "const_iterator"
	}
	$callString .= qq{maptype_${hname}::$iterator iter = map_${hname}.find($rid);
			  if(iter == map_${hname}.end()) {
			      maceWarn("No $hname registered with uid %d for upcall $mname!\\n", $rid);
			      $body
			      }
			  else {
			      $return iter->second->$callm;
			  }
		      };
	#TODO: Logging, etc.
	my $selectorVar = $m->options('selectorVar');
	my $logMethod = "";
	if ($this->traceLevel() > 0) {
	    my $fnName = $origmethod->name();
	    $logMethod = qq/if(!macecompiler(0).isNoop()) {
		/;
	    $logMethod .= qq/macecompiler(0) << "$fnName(" /;
	    for my $p ($m->params()) {
		my $pname = $p->name();
		if (not $p->flags('message')) {
		    $logMethod .= qq/<< "[$pname=";
			mace::printItem(macecompiler(0), &$pname, $pname);
			macecompiler(0) << "]" /;
		} elsif ($this->traceLevel() > 1) {
		    $logMethod .= qq/<< "[$pname=" << $pname << "]" /;
		} else {
		    my $ptype = $p->type()->type();
		    $logMethod .= qq/<< "[$pname=$ptype]" /;
		}
	    }
	    $logMethod .= qq/<< ")" << Log::endl;
	    }
	    /;
	}
	print $outfile <<END;
	$routine {
	    #define selector selector_$selectorVar
	    #define selectorId selectorId_$selectorVar
	    ADD_LOG_BACKING
		$logMethod
		$serialize
		$defaults
		$callString
		#undef selector
		#undef selectorId
	    }
END
    }
    print $outfile "//END Mace::Compiler::ServiceImpl::printUpcallHelpers\n";
}

sub printConstructor {
    my $this = shift;
    my $outfile = shift;

    my $name = $this->name();
    my @svo = grep( not($_->intermediate), $this->service_variables());
    my @svp = grep( not($_->intermediate or $_->final), $this->service_variables());

    print $outfile "//BEGIN Mace::Compiler::ServiceImpl::printConstructor\n";
    foreach my $scProvided ($this->provides()) {
	my $realMethod = "real_new_${name}_$scProvided";
	print $outfile "${scProvided}ServiceClass& $realMethod(".join(", ", (map{$_->serviceclass."ServiceClass& ".$_->name} @svo), (map{$_->toString("nodefaults" => 1)} $this->constructor_parameters()) ).") {\n";
	print $outfile "return *(new ${name}Service(".join(",", (map{$_->name} @svp, $this->constructor_parameters)).") );\n";
	print $outfile "}\n";
    }
    

    #TODO: utility_timer
    my $constructors = "${name}Service::${name}Service(".join(", ", (map{$_->serviceclass."ServiceClass& __".$_->name} @svo), (map{$_->type->toString()." _".$_->name} $this->constructor_parameters()) ).") : \n//(\nBaseMaceService(), __inited(0)";
    $constructors .= ", _actual_state(init), state(_actual_state)";
    my $initSelectors = "";
    for my $sv (keys(%{$this->selectorVars()})) {
	$initSelectors .= qq{${name}Service::selectorId_${sv} = new LogIdSet(selector_${sv});
			 };
    }
    map{ my $n = $_->name(); my $rid = $_->registrationUid(); $constructors .= ",\n_$n(__$n), $n($rid)"; } @svo;
    map{ my $n = $_->name(); $constructors .= ",\n$n(_$n)"; } $this->constructor_parameters();
    map{
	my $n = $_->name();
	my $default = "";
	if ($_->hasDefault()) {
	    $default = $_->default();
	}
	$constructors .= ",\n$n($default)";
    } $this->state_variables(), $this->onChangeVars(); #nonTimer => state_Var
    map{ my $timer = $_->name(); $constructors .= ",\n$timer(*(new ${timer}_MaceTimer(this)))"; } $this->timers();
    $constructors .= qq|{
	static bool initedSelectors __attribute((unused)) = initializeSelectors();
    }
    |;
    print $outfile qq|
	static bool initializeSelectors() {
	    $initSelectors
		return true;
	}
    |;
    print $outfile $constructors;
    print $outfile "//END Mace::Compiler::ServiceImpl::printConstructor\n";
}
sub traceLevel {
    my $this = shift;
    if ($this->trace eq 'off') {
	return -1;
    }
    if ($this->trace eq 'manual') {
	return 0;
    }
    if ($this->trace eq 'low') {
	return 1;
    }
    if ($this->trace eq 'med') {
	return 2;
    }
    if ($this->trace eq 'high') {
	return 3;
    }
}

sub printInitHFile {
    my $this = shift;
    my $outfile = shift;
    my $name = $this->name();

    my @svp = grep( not($_->intermediate or $_->final), $this->service_variables());

    my $r = Mace::Compiler::GeneratedOn::generatedOnHeader("$name init declaration file");
    print $outfile $r;

    print $outfile <<END;
#ifndef ${name}_init_h
#define ${name}_init_h

    #include "${name}-constants.h"
END
    $this->printIncludeBufH($outfile);

    foreach my $scProvided ($this->provides()) {
	print $outfile <<END;
#include "${scProvided}ServiceClass.h"
END
    }
    foreach my $scUsed ($this->service_variables()) {
	print $outfile $scUsed->returnSCInclude();
    }
    print $outfile <<END;

    namespace ${name}_namespace {
END
    foreach my $scProvided ($this->provides()) {
	print $outfile "${scProvided}ServiceClass& new_${name}_$scProvided(".join(", ", (map{my $svline = $_->line(); qq{\n#line $svline "$Mace::Compiler::Globals::filename"\n}.$_->serviceclass."ServiceClass& ".$_->name." = ".$_->serviceclass."ServiceClass::NULL_\n// __INSERT_LINE_HERE__\n"} @svp), (map{$_->toString()} $this->constructor_parameters()) ).");\n";

	if ($this->count_constructor_parameters() and scalar(@svp)) {
	    print $outfile "${scProvided}ServiceClass& new_${name}_$scProvided(";
	    my @p = $this->constructor_parameters();
	    my $p1 = shift(@p);
	    print $outfile $p1->type->toString()." ".$p1->name.", ";
	    print $outfile join(", ", (map{$_->toString()} @p), (map{my $svline = $_->line(); qq{\n#line $svline "$Mace::Compiler::Globals::filename"\n}.$_->serviceclass."ServiceClass& ".$_->name." = ".$_->serviceclass."ServiceClass::NULL_\n// __INSERT_LINE_HERE__\n"} @svp) );
	    print $outfile ");\n";
	}
    }

    print $outfile <<END;
																						      } //end namespace
#endif // ${name}_init_h
END
}

sub printInitCCFile {
    my $this = shift;
    my $outfile = shift;
    my $name = $this->name();
    my @svo = grep( not($_->intermediate), $this->service_variables());
    my @svp = grep( not($_->intermediate or $_->final), $this->service_variables());

    my $r = Mace::Compiler::GeneratedOn::generatedOnHeader("$name init source file");
    print $outfile $r;

    print $outfile <<END;
  #include "${name}-init.h"
END
    print $outfile join("", map{my $sv=$_->service; qq{#include "$sv-init.h"\n}} grep($_->service, $this->service_variables()));
    print $outfile <<END;
  namespace ${name}_namespace {
END
    foreach my $scProvided ($this->provides()) {
	my $realMethod = "real_new_${name}_$scProvided";
	print $outfile "${scProvided}ServiceClass& $realMethod(".join(", ", (map{$_->serviceclass."ServiceClass& ".$_->name} @svo), (map{$_->toString("nodefaults" => 1)} $this->constructor_parameters()) ).");\n";
	print $outfile "${scProvided}ServiceClass& new_${name}_$scProvided(".join(", ", (map{$_->serviceclass."ServiceClass& _".$_->name} @svp), (map{$_->toString("nodefaults" => 1)} $this->constructor_parameters()) ).") {\n";
	$this->printInitStuff($outfile, $realMethod);
	print $outfile "}\n";

	if ($this->count_constructor_parameters() and scalar(@svp)) {
	    print $outfile "${scProvided}ServiceClass& new_${name}_$scProvided(";
	    my @p = $this->constructor_parameters();
	    my $p1 = shift(@p);
	    print $outfile $p1->type->toString()." ".$p1->name.", ";
	    print $outfile join(", ", (map{$_->toString("nodefaults"=>1)} @p), (map{$_->serviceclass."ServiceClass& _".$_->name} @svp) );
	    print $outfile ") {\n";
	    $this->printInitStuff($outfile, $realMethod);
	    print $outfile "}\n";
	}
    }
    print $outfile <<END;
  } //end namespace
END
}

sub printInitStuff {
    my $this = shift;
    my $outfile = shift;
    my $realMethod = shift;
    my $name = $this->name();
    my @svo = grep( not($_->intermediate), $this->service_variables());
    my @svp = grep( not($_->intermediate or $_->final), $this->service_variables());

    for (my $i = $this->count_service_variables()-2; $i >= 0; $i--) {
	my $s1 = $this->service_variables()->[$i];
	my $s1n = $this->service_variables()->[$i]->name();
	my @joins;
	for (my $j = $this->count_service_variables()-1; $j > $i; $j--) {
	    my $s2 = $this->service_variables()->[$j];
	    my $s2n = $this->service_variables()->[$j]->name();
	    if ($s2->service and grep($s1n eq $_, $s2->constructionparams())) {
		if ($s2->final) {
		    @joins = [ 'true' ];
		    last;
		}
		elsif ($s2->intermediate) {
		    push @joins, "later_dep_$s2n";
		}
		else {
		    push @joins, "(&_".$s2n." == &".$s2->serviceclass."ServiceClass::NULL_)";
		}
	    }
	}
	if ($s1->intermediate()) {
	    if (@joins) {
		print $outfile "const bool later_dep_$s1n = ". join(' || ', @joins) . ";\n";
	    }
	    else {
		Mace::Compiler::Globals::error("invalid_service", $s1->line(), "'intermediate' may only be applied to services which are used later in the services block; $s1n is not used later!");
	    }
	}
    }

    for my $sv ($this->service_variables()) {
	my $svline = $sv->line();
	if ($sv->service() or $sv->doDynamicCast()) {
	    print $outfile qq{\n#line $svline "$Mace::Compiler::Globals::filename"\n};
	    if ($sv->intermediate) {
		print $outfile $sv->serviceclass."ServiceClass& ${\$sv->name} = (later_dep_${\$sv->name}) ? ".$sv->toNewMethodCall()." : ".$sv->serviceclass."ServiceClass::NULL_;\n";
	    } elsif ($sv->final) {
		print $outfile $sv->serviceclass."ServiceClass& ${\$sv->name} = ${\$sv->toNewMethodCall()};\n";
	    } else {
		print $outfile $sv->serviceclass."ServiceClass& ${\$sv->name} = (&_${\$sv->name} == &".$sv->serviceclass."ServiceClass::NULL_) ? ".$sv->toNewMethodCall()." : _${\$sv->name};\n";
	    }
	    print $outfile qq{\n// __INSERT_LINE_HERE__\n};
	}
	else {
	    print $outfile qq{\n#line $svline "$Mace::Compiler::Globals::filename"\n};
	    print $outfile $sv->serviceclass."ServiceClass& ${\$sv->name} = _${\$sv->name};\n";
	    print $outfile qq{\n// __INSERT_LINE_HERE__\n};
	}
    }
    print $outfile "return $realMethod(".join(", ", (map{ $_->name } (@svo, $this->constructor_parameters()))).");\n";
}

sub printMacrosFile {
    my $this = shift;
    my $outfile = shift;
    my $name = $this->name();

    my $timerDebug = ($this->traceLevel() > 1)?q{maceCompiler(1, "Timer: Resched timer \"#x\" for time \"#y\"\n");}:"";

    my $r = Mace::Compiler::GeneratedOn::generatedOnHeader("$name macros file");
    print $outfile $r;

    my $undefCurtime = "";
    if ($this->macetime()) {
	#      $undefCurtime = '#undef curtime';
    }

    print $outfile <<END;
#ifndef ${name}_macros_h
#define ${name}_macros_h

#include "lib/mace-macros.h"
$undefCurtime

#define state_change(s) changeState(s, selectorId->log)
END

    for my $m ($this->providedHandlerMethods()) {
	my $fnName = $m->name;
	my $clName = $m->options('class')->[0]->name();
	print $outfile <<END;
    #define typeof_upcall_$fnName $clName
    #define map_typeof_upcall_$fnName map_$clName
END
    }

    print $outfile <<END;
  
#endif //${name}_macros_h
END
}

sub sortByLine {
    my $this = shift;
    my $func = shift;
    my $ref = $this->$func();
    my @sorted = sort {
	if ($a->includedline() == $b->includedline()) {
	    return $a->line() <=> $b->line();
	}
	else {
	    return $a->includedline() <=> $b->includedline();
	}
    } @$ref;
    $this->$func(@sorted);
}

1;
