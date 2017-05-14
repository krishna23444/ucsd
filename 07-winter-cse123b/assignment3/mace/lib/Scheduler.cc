/* 
 * Scheduler.cc : part of the Mace toolkit for building distributed systems
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
#include "Scheduler.h"
#include "Util.h"

Scheduler* Scheduler::scheduler = 0;

Scheduler::Scheduler() : running(true), next(0) {
  pthread_mutex_init(&slock, 0);
} // Scheduler

Scheduler::~Scheduler() {
  haltScheduler();
} // ~Scheduler

void Scheduler::haltScheduler() {
  assert(scheduler);
  scheduler->lock();
  bool r = scheduler->running;
  scheduler->running = false;
  scheduler->unlock();
  if (r) {
    scheduler->sig.signal();
    assert(pthread_join(scheduler->schedulerThread, NULL) == 0);
  }
} // haltScheduler

uint64_t Scheduler::schedule(TimerHandler& timer, uint64_t time, bool abs) {
  ADD_SELECTORS("Scheduler::schedule");
  ScopedLock sl(slock);
  if (!abs) {
    time += Util::timeu();
  }

//   maceout << "scheduling " << timer.getId() << " for " << time << Log::endl;
  timers.insert(std::make_pair(time, &timer));

  if (time < next || next == 0) {
//     maceout << "signaling" << Log::endl;
    sig.signal();
  }

  return time;
} // schedule

void Scheduler::cancel(TimerHandler& timer) {
  ADD_SELECTORS("Scheduler::cancel");
  ScopedLock sl(slock);

//   maceout << "canceling " << timer.getId() << Log::endl;

  TimerMap::iterator i = timers.begin();
  while (i != timers.end()) {
//     maceout << "checking " << i->second->getId() << Log::endl;
    if (i->second->getId() == timer.getId()) {
      timers.erase(i);
      if (!timers.empty()) {
	next = timers.begin()->first;
// 	maceout << "set next=" << next << Log::endl;
      }

//       maceout << "returning" << Log::endl;
      return;
    }
    i++;
  }
} // cancel
  
void Scheduler::runSchedulerThread() {
  ADD_SELECTORS("Scheduler::runSchedulerThread");

  while (running) {
    uint64_t pending = 0;

    lock();
    joinThreads(joinSet);
    
    if (!timers.empty()) {
      pending = timers.begin()->first;
      next = pending;
//       maceout << "got pending=" << pending << " next=" << next << Log::endl;
    }
    else {
      next = 0;
      unlock();
//       maceout << "no timers, waiting for signal" << Log::endl;
      sig.wait();
      continue;
    }
    uint64_t now = Util::timeu();
    uint64_t sleeptime = next - now;
//     maceout << "now=" << now << " sleeptime=" << sleeptime << " next=" << next << Log::endl;
    if ((now + CLOCK_RESOLUTION) > next) {
//       maceout << (now + CLOCK_RESOLUTION) << " > " << next << ", firing" << Log::endl;
      fireTimer(true);
    }
    else {
      unlock();
//       maceout << "sleeping for " << sleeptime << Log::endl;
      int n = sig.wait(sleeptime);
//       maceout << "sig returned " << n << Log::endl;
      if (n) {
	// another timer has been scheduled before pending
	continue;
      }
      if (pending == next) {
	// the pending timer has not been canceled
// 	maceout << "pending == next = " << next << ", firing" << Log::endl;
	fireTimer(false);
      }
    }
  }

  lock();
  joinThreads(joinSet);
  unlock();
  joinThreads(shutdownJoinSet);
} // runSchedulerThread

void Scheduler::fireTimer(bool locked) {
  ADD_SELECTORS("Scheduler::fireTimer");

  if (!locked) {
    lock();
  }
  if (timers.empty()) {
    unlock();
//     maceout << "no timer to fire, returning" << Log::endl;
    return;
  }
  TimerMap::iterator i = timers.begin();
  TimerHandler* t = i->second;
  timers.erase(i);
  unlock();
//   maceout << "firing " << t->getId() << Log::endl;
  t->fire();
} // fireTimer

size_t Scheduler::joinThreads(ThreadIdSet& s) {
  size_t joinCount = 0;
  for (ThreadIdSet::const_iterator i = s.begin(); i != s.end(); i++) {
    assert(pthread_join(*i, NULL) == 0);
    joinCount++;
  }
  if (joinCount) {
    s.clear();
  }
  return joinCount;
} // joinThreads

void Scheduler::joinThread(pthread_t tid) {
  ScopedLock sl(slock);
  if (running) {
    shutdownJoinSet.erase(tid);
    joinSet.insert(tid);
  }
} // joinThread

void Scheduler::shutdownJoinThread(pthread_t tid) {
  ScopedLock sl(slock);

  if (running) {
    shutdownJoinSet.insert(tid);
  }
} // shutdownJoinThread

void* Scheduler::startSchedulerThread(void* arg) {
  Scheduler* s = (Scheduler*)arg;
  s->runSchedulerThread();
  return 0;
} // startSchedulerThread

uint64_t TimerHandler::schedule(uint64_t time, bool abs, bool align) {
  assert(!scheduled);
  scheduled = true;
  running = true;

  if (align && (time % (1000*1000) == 0)) {
    uint64_t now = Util::timeu();
    uint sec = time / (1000*1000);
    uint64_t sleep = 1000*1000;
    sleep -= (now % (1000 * 1000));
    if (sleep < 500*1000) {
      sleep += 1000*1000;
    }
    time = (sec - 1) * 1000*1000 + sleep;
  }
    
  return Scheduler::Instance().schedule(*this, time, abs);
} // TimerHandler::schedule
