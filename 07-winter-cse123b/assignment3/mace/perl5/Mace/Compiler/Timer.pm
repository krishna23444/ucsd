# 
# Timer.pm : part of the Mace toolkit for building distributed systems
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
package Mace::Compiler::Timer;

use strict;

use Mace::Compiler::Type;
use Mace::Util qw(:all);

use Class::MakeMethods::Template::Hash 
    (
     'new' => 'new',
     'string' => "name",
     'array_of_objects' => ["types" => { class => "Mace::Compiler::Type"}],

     'string' => "scheduleSelector",
     'string' => "cancelSelector",
     'string' => "expireSelector",

      #from type options
     'scalar' => "recur",
     'boolean' => "dump",
     'boolean' => "multi", #implies the same timer may be scheduled more than once
     'boolean' => "random",

      #for error messages
     'string' => 'filename',
     'number' => 'line',

     'array_of_objects' => ["params" => { class => "Mace::Compiler::Param"}],
     'array_of_objects' => ["typeOptions" => { class => "Mace::Compiler::TypeOption" }],
     );

sub type {
  my $this = shift;
  return Mace::Compiler::Type->new(type=>$this->name."_MaceTimer");
}

sub toString {
    my $this = shift;
    my $name = shift;
    my %args = @_;
    my $r = "";
    my $n = $this->name();
    my $maptype = "mace::map<uint64_t, TimerData*>";
    my $scheduleFields = "";
    my $noCommaScheduleFields = "";
    my $params = "";
    my $commaparams = "";
    my $timerDataBody = "";
    my $callParams = "";
    my $traceLevel = $args{traceLevel};

    my $traceg1 = ($traceLevel > 1)?'true':'false';
    my $trace = ($traceLevel > 0)?'true':'false';
    my $printFields = "";
    my $printFieldState = "";
    my $maceoutPrintFields = "";
    my $maceoutPrintFieldsTd = "";

    my $macetime = $Mace::Compiler::Globals::MACE_TIME;
    my $prep = "PREPARE_FUNCTION";
    my $expirePrep = "ANNOTATE_SET_PATH_ID(NULL, 0, timerData->pipPathId, timerData->pipPathIdLen);";
    my $timeType = "uint64_t";
    my $realtime = "";
    if ($macetime) {
#	$prep = "MaceTime curtime = MaceTime::currentTime();";
	$timeType = "MaceTime";
	$realtime = ".time()";
    }

    my $compareFields = "";

    $timerDataBody .= qq{
      public:
      char* pipPathId;
      int pipPathIdLen;
      TimerData() : pipPathId(NULL), pipPathIdLen(0) { }
      ~TimerData() { ::free(pipPathId); }
    };

    if($this->count_params()) {
      $scheduleFields = join("", map{", ".$_->toString(paramconst => 1, paramref => 1)} $this->params());
      $noCommaScheduleFields = join(", ", map { $_->toString(paramconst => 1, paramref => 1) } $this->params());
      $params = join(", ", map{"temptd->".$_->name()} $this->params());
      $commaparams = ", $params";
      $compareFields = join(" && ", map{"(temptd->".$_->name()." == ".$_->name().")"} $this->params());
      $timerDataBody .= ("TimerData(" .
			 join(", ", map{$_->toString(paramconst => 1, paramref=> 1)}
			      $this->params()) .") : " .
			 join(", ", map{$_->name()."(".$_->name().")"} $this->params()) .
			 "{ }\n");
      $timerDataBody .= join("\n", map{$_->toString().";"} $this->params());
      $callParams = join(", ", map{$_->name()} $this->params());
      $printFields = join("", map{qq/", ${\$_->name()}="; mace::printItem(__out, &td->${\$_->name()}, td->${\$_->name()}); __out << /} $this->params());
      $maceoutPrintFields = join("", map{qq/", ${\$_->name()}="; mace::printItem(maceout, &${\$_->name()}, ${\$_->name()}); maceout << /} $this->params());
      $maceoutPrintFieldsTd = join("", map{qq/", ${\$_->name()}="; mace::printItem(maceout, &temptd->${\$_->name()}, temptd->${\$_->name()}); maceout << /} $this->params());
      $printFieldState = join("", map{qq/", ${\$_->name()}="; mace::printState(__out, &td->${\$_->name()}, td->${\$_->name()}); __out << /} $this->params());
    }

    my $schedulePrep = qq{
	timerData = new TimerData($callParams);
	timerData->pipPathId = annotate_get_path_id_copy(&timerData->pipPathIdLen);
    };

    my $printBody = qq{
      TimerData* td __attribute((unused)) = timerData;
      __out << "timer<${\$this->name}>(scheduled=" << nextScheduledTime << $printFields ")";
    };
    my $printStateBody = qq/
      if(nextScheduledTime) {
        TimerData* td __attribute((unused)) = timerData;
        __out << "timer<${\$this->name}>(scheduled" << $printFieldState ")";
      } else {
        __out << "timer<${\$this->name}>(not scheduled)";   
      }
    /;

    my $timerDataObject = ($this->multi()?"$maptype timerData":"TimerData *timerData");
    my $multiIsScheduled = "";
    my $nextScheduled = "return nextScheduledTime";
    my $nextScheduledTime = "uint64_t nextScheduledTime;";
    my $nextScheduledConstructor = ", nextScheduledTime()";
    my $rescheduleMethod = "$timeType reschedule(const $timeType& interval $scheduleFields) {
      cancel();
      return schedule(interval $commaparams);
    }";
    my $scheduleBody = qq{
      ASSERT(!TimerHandler::isScheduled());
      nextScheduledTime = TimerHandler::schedule(interval$realtime, false);
      return nextScheduledTime;
      };
    my $multiCancel = "";
    my $multiNumScheduled = "";
    my $cancelMethod = qq{
      if (TimerHandler::isScheduled()) {
	TimerHandler::cancel();
	nextScheduledTime = 0; 
	delete timerData;
	timerData = 0;
      }
    };
    my $expireFunction = qq{
      TimerData* temptd = timerData; 
      timerData = 0;
      agent_->expire_$n($params);
      delete temptd;
    };

    if($this->multi()) {
	$multiNumScheduled = "size_t scheduledCount() const { return timerData.size(); }";
	$multiIsScheduled = "bool isScheduled(const $timeType& expireTime) const { return timerData.containsKey(expireTime$realtime); }";
	$nextScheduled = "return timerData.empty()?0:timerData.begin()->first";
	$nextScheduledTime = "";
	$schedulePrep = qq{
	    TimerData* td = new TimerData($callParams);
	    td->pipPathId = annotate_get_path_id_copy(&td->pipPathIdLen);
	};
	$expirePrep = "";
	$nextScheduledConstructor = "";
	$printBody = qq/
        __out << "timer<${\$this->name}>(";
        for(${maptype}::const_iterator i = timerData.begin(); i != timerData.end(); i++) {
          TimerData* td __attribute((unused)) = i->second;
          __out << "[scheduled=" << i->first << $printFields "]";
        }
        __out << ")";
	/;
	$printStateBody = qq/
	    __out << "timer<${\$this->name}>(";
        for(${maptype}::const_iterator i = timerData.begin(); i != timerData.end(); i++) {
	    TimerData* td __attribute((unused)) = i->second;
	    __out << "[scheduled" << $printFieldState "]";
        }
        __out << ")";
	/;
	my $random = $this->random() . " && Scheduler::Instance().isSimulated()";
	$scheduleBody = qq{
//	  TimerData* td = new TimerData($callParams);
	  uint64_t t = Util::timeu() + interval$realtime;
	  if ($random) {
	    t = Util::timeu();
	  }
	  // maceout << "interval=" << interval << " t=" << t << Log::endl;
	  ${maptype}::iterator i = timerData.find(t);
	  while(i != timerData.end() && i->first == t) {
	    // maceout << "found duplicate timer scheduled for " << t << Log::endl;
	    i++;
	    t++;
	  }
// 	  std::cerr << "assigning timer to " << t << std::endl;
	  timerData[t] = td;
	  if (timerData.begin()->first == t) {
	    TimerHandler::cancel();
	    maceout << "calling schedule for " << t << Log::endl;
	    ASSERT(TimerHandler::schedule(t, true) == t);
	  }
	  return t;
	};

	$multiCancel = qq{
	  void cancel(const $timeType& expireTime) {
	    #define selector selector_cancel$n
	    #define selectorId selectorId_cancel$n
	    $prep
	    ADD_LOG_BACKING
	    ScopedLog __scopedLog(selector, 0, selectorId->compiler, true, $traceg1, $trace, PIP);

	    maceout << "canceling timer at " << expireTime << Log::endl;

	    ${maptype}::iterator i = timerData.find(expireTime$realtime);
	    if(i == timerData.end()) {
	      maceout << "timer not found, returning" << Log::endl;
	      return;
	    }
	    delete i->second;
	    if(i == timerData.begin()) {
	      maceout << "calling TimerHandler::cancel on " << getId() << Log::endl;
	      TimerHandler::cancel();
	      timerData.erase(i);
	      if(!timerData.empty()) {
		maceout << "calling schedule for " << timerData.begin()->first << Log::endl;
		TimerHandler::schedule(timerData.begin()->first, true);
	      }
	    }
	    else {
	      maceout << "erasing from timerData " << i->first << Log::endl;
	      timerData.erase(i);
	    }

	    #undef selector
	    #undef selectorId
	  }
			   
	  void cancel($noCommaScheduleFields) {
	    #define selector selector_cancel$n
	    #define selectorId selectorId_cancel$n
	    $prep
	    ADD_LOG_BACKING
	    ScopedLog __scopedLog(selector, 0, selectorId->compiler, true, $traceg1, $trace, PIP);

	    maceout << "canceling timer for " << $maceoutPrintFields Log::endl;

	    ${maptype}::iterator i = timerData.begin();
	    bool reschedule = false;
	    while (i != timerData.end()) {
	      TimerData* temptd = i->second;
	      if ($compareFields) {
		maceout << "erasing " << i->first << " : " << $maceoutPrintFieldsTd Log::endl;
		delete i->second;
		timerData.erase(i++);
		reschedule = true;
	      }
	      else {
		i++;
	      }
	    }
	    if (reschedule) {
	      maceout << "calling TimerHandler::cancel on " << getId() << Log::endl;
	      TimerHandler::cancel();
	      if(!timerData.empty()) {
		maceout << "calling schedule for " << timerData.begin()->first << Log::endl;
		TimerHandler::schedule(timerData.begin()->first, true);
	      }
	    }

	    #undef selector
	    #undef selectorId
	  }
			   
		       };
	$cancelMethod = qq{TimerHandler::cancel();
			   maceout << "canceling all timers" << Log::endl;
                       for(${maptype}::iterator i = timerData.begin(); i != timerData.end(); i++) {
                         delete i->second;
                       }
			 // maceout << "clearing timerData" << Log::endl;
                       timerData.clear();
        };
	$rescheduleMethod = "";
	
	my $loopCondition = "(i->first < (curtime + Scheduler::CLOCK_RESOLUTION))";
	if ($macetime) {
	    $loopCondition = "MaceTime(i->first - Scheduler::CLOCK_RESOLUTION).lessThan(curtime)";
	}

	$expireFunction = qq{
	ASSERT(!timerData.empty());
	if ($random) {
	  unsigned ntimers = Util::randInt(timerData.size()) + 1;
	  maceout << "firing " << ntimers << " random timers" << Log::endl;
	  std::vector<std::pair<uint64_t, TimerData*> > toFire;
	  $maptype copy = timerData;
	  for (unsigned i = 0; i < ntimers; i++) {
	    unsigned which = Util::randInt(copy.size());
	    ${maptype}::iterator mi = copy.begin();
	    for (unsigned j = 0; j < which; j++) {
	      mi++;
	    }
	    toFire.push_back(*mi);
	    maceout << "selecting " << mi->first << Log::endl;
	    copy.erase(mi);
	  }
	  for (std::vector<std::pair<uint64_t, TimerData*> >::iterator i = toFire.begin();
	       i != toFire.end(); i++) {
	    if (timerData.containsKey(i->first)) {
	      TimerData* temptd = i->second;
	      ANNOTATE_SET_PATH_ID(NULL, 0, temptd->pipPathId, temptd->pipPathIdLen);
	      ScopedLog __scopedLog(selector, 0, selectorId->compiler, true, $traceg1, $trace, PIP);
	      timerData.erase(i->first);
	      maceout << "firing " << i->first << Log::endl;
	      agent_->expire_$n($params);
	      delete temptd;
	    }
	    else {
	      maceout << i->first << " canceled!" << Log::endl;
	    }
	  }
	}
	else {
	  ${maptype}::iterator i = timerData.begin();
	  while((i != timerData.end()) && $loopCondition) {
	    TimerData* temptd = i->second;
	    timerData.erase(i);
	    maceout << "calling expire into service for $n " << i->first <<  Log::endl;
	    agent_->expire_$n($params);
	    delete temptd;
	    i = timerData.begin();
	  }
	}
	if (!timerData.empty() && !TimerHandler::isScheduled()) {
	  maceout << "calling resched for " << timerData.begin()->first << Log::endl;
	  TimerHandler::schedule(timerData.begin()->first, true);
	}
	else if (timerData.empty() && TimerHandler::isScheduled()) {
	  maceout << "canceling already expired timer" << Log::endl;
	  TimerHandler::cancel();
	}
      };
    }
    $r .= qq{class ${name}::${n}_MaceTimer : private TimerHandler, public mace::PrintPrintable {
            public:
              ${n}_MaceTimer($name *a)
                : TimerHandler("${name}::${n}"), agent_(a)$nextScheduledConstructor
              {
	      }

	      virtual ~${n}_MaceTimer() {
		cancel();
	      }
              
	      using TimerHandler::isScheduled;
              $timeType schedule(const $timeType& interval $scheduleFields) {
                #define selector selector_schedule$n
                #define selectorId selectorId_schedule$n
                $prep
                $schedulePrep
                ADD_LOG_BACKING
                ScopedLog __scopedLog(selector, 0, selectorId->compiler, true, $traceg1, $trace, PIP);

                $scheduleBody
                #undef selector
                #undef selectorId
              }
              $rescheduleMethod
              void cancel() {
                #define selector selector_cancel$n
                #define selectorId selectorId_cancel$n
                $prep
                ADD_LOG_BACKING
                ScopedLog __scopedLog(selector, 0, selectorId->compiler, true, $traceg1, $trace, PIP);
                $cancelMethod
                #undef selector
                #undef selectorId
              }
              $multiCancel
              $timeType nextScheduled() const { //not scheduled == 0
                $nextScheduled;
              }
              $multiIsScheduled
	      $multiNumScheduled

              void print(std::ostream& __out) const {
                $printBody
              }
              void printState(std::ostream& __out) const {
                $printStateBody
              }

	    protected:
              void expire() {
                #define selector selector_expire$n
                #define selectorId selectorId_expire$n
                $prep
                $expirePrep
                ADD_LOG_BACKING
		ScopedLock __scopedLock(BaseMaceService::agentlock);
                ScopedLog __scopedLog(selector, 0, selectorId->compiler, true, $traceg1,
				      $trace, PIP && !${\$this->multi()});
		// maceout << "executing" << Log::endl;
                if (!isRunning()) {
		  // maceout << "timer not running, returning" << Log::endl;
                  return;
                }
                
                $expireFunction
                #undef selector
                #undef selectorId
              }
              
              
            protected:
              $name *agent_;
              $nextScheduledTime

              class TimerData {
                $timerDataBody
              };

              $timerDataObject;
            };
          };
    return $r;
} # toString

                              #uint sched_count_$t;
                              #void* pip_path_id_$t;
                              #int pip_path_len_$t;
                              #,\nsched_count_$timer(), pip_path_id_$timer(), pip_path_len_$timer()
sub validateTypeOptions {
    my $this = shift;
    my $sv = shift;
    my $name;
    my $value;
    my %fieldoptions;
    $this->dump(1);
    $this->multi(0);
    $this->recur(0);
    foreach my $option ($this->typeOptions()) {
	Mace::Compiler::Globals::error('bad_type_option', $option->line(), "Cannot define option ".$option->name()." for field ".$this->name()." more than once!") unless(++$fieldoptions{$option->name()} == 1);

	if ($option->name() eq 'dump') {
	    Mace::Compiler::Globals::error('bad_type_option', $option->line(), "option dump must have exactly one sub-option") unless (scalar(keys(%{$option->options()})) == 1);
	    while (($name, $value) = each(%{$option->options()})) {
		if ($name eq 'no') {
		    $this->dump(0);
		}
		elsif (! $name eq 'yes') {
		    Mace::Compiler::Globals::error('bad_type_option', $option->line(), "Invalid option with name $name and value $value");
		}
	    }
	}
	elsif ($option->name() eq 'recur') {
	    Mace::Compiler::Globals::error('bad_type_option', $option->line(), "option recur must have exactly one sub-option") unless(scalar(keys(%{$option->options()})) == 1);
	    $this->recur((keys(%{$option->options()}))[0]);
	    Mace::Compiler::Globals::error('bad_type_option', $option->line(), "options multi and recur cannot be used together") if($this->recur() and $this->multi());
	}
	elsif ($option->name() eq 'multi') {
# 	    Mace::Compiler::Globals::error('bad_type_option', $option->line(), "option multi must have exactly one sub-option") unless(scalar(keys(%{$option->options()})) == 1);
	    $this->random(0);
	    if (defined($option->options()->{"random"})) {
		if ($option->options("random") =~ m/^(random|yes)$/) {
		    $this->random(1);
		}
		elsif ($option->options("random") !~ m/^no$/) {
		    Mace::Compiler::Globals::error('bad_type_option', $option->line(),
						   "random cannot have value " .
						   $option->options("random"));
		}
		delete($option->options()->{"random"});
	    }

	    while (($name, $value) = each(%{$option->options()})) {
		if ($name eq 'yes') {
		    $this->multi(1);
		}
		elsif (! $name eq 'no') {
		    Mace::Compiler::Globals::error('bad_type_option', $option->line(), "Invalid option with name $name and value $value");
		}
	    }
	    Mace::Compiler::Globals::error('bad_type_option', $option->line(), "options multi and recur cannot be used together") if($this->recur() and $this->multi());
	}
	else {
	    Mace::Compiler::Globals::error('bad_type_option', $option->line(), "Invalid option ".$option->name());
	}
    }

    my $count = 0;
    for my $t ($this->types()) {
	my $p = Mace::Compiler::Param->new(name => "p$count", type => $t);
	$this->push_params($p);
	$count++;
    }

    my $selector = $sv->selectors('message');
    $selector =~ s/\$function/schedule/;
    $selector =~ s/\$state/(timer)/;
    my $n = $this->name();
    $selector =~ s/\$message/$n/;
    $this->scheduleSelector($selector);
    $sv->selectorVars("schedule$n", $selector);
    $selector =~ s/schedule/cancel/;
    $this->cancelSelector($selector);
    $sv->selectorVars("cancel$n", $selector);
    $selector =~ s/cancel/expire/;
    $this->expireSelector($selector);
    $sv->selectorVars("expire$n", $selector);
}

sub toPrint {
  my $this = shift;
  my $out = shift;
  if($this->dump()) {
#    my $s = qq{<< "$name=" << (${name}.nextScheduled()?"scheduled":"unscheduled")};
    my $s = qq{$out << ${\$this->name};};
    return $s;
  }
  return '';
}

sub toPrintState {
  my $this = shift;
  my $out = shift;
  if($this->dump()) {
#    my $s = qq{<< "$name=" << (${name}.nextScheduled()?"scheduled":"unscheduled")};
    my $s = qq{${\$this->name}.printState($out);};
    return $s;
  }
  return '';
}

1;
