/* 
 * Scheduler.h : part of the Mace toolkit for building distributed systems
 * 
 * Copyright (c) 2006, Charles Killian, Dejan Kostic, Ryan Braud, James W. Anderson, John Fisher-Ogden, Calvin Hubble, Duy Nguyen, Justin Burke, David Oppenheimer, Amin Vahdat, Adolfo Rodriguez, Sooraj Bhat
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in
 *      the documentation and/or other materials provided with the
 *      distribution.
 *    * Neither the names of Duke University nor The University of
 *      California, San Diego, nor the names of the authors or contributors
 *      may be used to endorse or promote products derived from
 *      this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * ----END-OF-LEGAL-STUFF---- */
#ifndef _SCHEDULER_H
#define _SCHEDULER_H

#include <sys/types.h>       
#include <pthread.h>
#include <sys/time.h>       
#include <unistd.h>
#include <stdint.h>
#include "mmultimap.h"
#include "mhash_set.h"
#include "ThreadCreate.h"
#include "NumberGen.h"
#include "PipedSignal.h"

class TimerHandler;

#ifdef __CYGWIN__
namespace __gnu_cxx {
  template<> struct hash<pthread_t>  {
    size_t operator()(const pthread_t& x) const { return (uint32_t)x; }
  };
}
#endif

class Scheduler : public RunThreadClass {

  friend void _runNewThread(pthread_t* t, func f, void* arg, 
			    pthread_attr_t* attr, const char* fname, bool joinThread);
  friend void _runNewThreadClass(pthread_t* t, RunThreadClass* c, classfunc f,
				 void* arg, pthread_attr_t* attr, 
				 const char* fname, bool joinThread);
  friend void* threadStart(void* vfa);

public:
  virtual ~Scheduler();
  static Scheduler& Instance() {
    if (scheduler == 0) {
      scheduler = new Scheduler();
      _runNewThread(&scheduler->schedulerThread, 
		    Scheduler::startSchedulerThread, scheduler, 0, 
		    "Scheduler::startSchedulerThread", false);
    }
    return *scheduler;
  }
  static void haltScheduler();
  virtual uint64_t schedule(TimerHandler& timer, uint64_t time, bool abs = false);
  virtual void cancel(TimerHandler& timer);
  virtual bool isSimulated() { return false; }

  static void* startSchedulerThread(void* arg);

  static const uint64_t CLOCK_RESOLUTION = 1000; // us == 1ms

protected:
  typedef mace::hash_set<pthread_t> ThreadIdSet;
  virtual void runSchedulerThread();
  virtual void fireTimer(bool locked);
  virtual void joinThread(pthread_t tid);
  virtual void shutdownJoinThread(pthread_t tid);
  size_t joinThreads(ThreadIdSet& s);
  void lock() { assert(pthread_mutex_lock(&slock) == 0); }
  void unlock() { assert(pthread_mutex_unlock(&slock) == 0); }


protected:
  bool running;
  uint64_t next;

  PipedSignal sig;

  typedef mace::multimap<uint64_t, TimerHandler*> TimerMap;
  TimerMap timers;

  ThreadIdSet joinSet;
  ThreadIdSet shutdownJoinSet;

  pthread_mutex_t slock;

  static Scheduler* scheduler;
  Scheduler(); //This is protected so the SimScheduler may use it.

private:  
  pthread_t schedulerThread;
}; // Scheduler

class TimerHandler {
public:
  TimerHandler(std::string desc = "") :
    description(desc), scheduled(false), running(false),
    id(NumberGen::Instance("TimerHandler")->GetVal()) {
  }

  virtual ~TimerHandler() { }

  void fire() {
    scheduled = false;
    expire();
  }

  uint64_t schedule(uint64_t time, bool abs = false, bool align = false);

  void cancel() {
    if (scheduled) {
      Scheduler::Instance().cancel(*this);
    }
    scheduled = false;
    running = false;
  }
  const std::string& getDescription() const { return description; }
  unsigned getId() const { return id; }
  bool isScheduled() const { return scheduled; }
  bool isRunning() const { return running; }
  bool operator==(const TimerHandler& other) const { return getId() == other.getId(); }

protected:
  virtual void expire() = 0;

private:
  std::string description;
  bool scheduled;
  bool running;
  unsigned id;
}; // TimerHandler

#endif // _SCHEDULER_H
