/* 
 * TransportScheduler.cc : part of the Mace toolkit for building distributed systems
 * 
 * Copyright (c) 2006, James W. Anderson, Charles Killian
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
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include <cassert>
#include <list>
#include <inttypes.h>

#include "ServiceInitializer.h"
#include "SysUtil.h"
#include "NumberGen.h"
#include "Accumulator.h"
#include "TransportScheduler.h"
#include "ThreadCreate.h"
#include "mvector.h"
//#include "mace-macros.h"

using std::max;
using std::cerr;
using std::endl;
using std::ostream;


TransportScheduler::TransportList TransportScheduler::transports;
TransportScheduler::PendingTransportList TransportScheduler::pendingTransports;
pthread_t TransportScheduler::schedulerThread;
bool TransportScheduler::running = false;
fd_set TransportScheduler::rset;
fd_set TransportScheduler::wset;
const time_t TransportScheduler::SELECT_TIMEOUT_MICRO = 500*1000;
const uint32_t TransportScheduler::CHECK_RUNNING_COUNT = 1000*1000 / SELECT_TIMEOUT_MICRO;
pthread_mutex_t TransportScheduler::slock = PTHREAD_MUTEX_INITIALIZER;
PipedSignal TransportScheduler::sig;
bool TransportScheduler::inSelect;

TransportScheduler::TransportScheduler() {
} // TransportScheduler

TransportScheduler::~TransportScheduler() {
} // ~TransportScheduler

void TransportScheduler::add(BaseTransportPtr t) {
  ScopedLock sl(slock);
  pendingTransports.push(t);
//   Log::log("TransportScheduler::add") << "added transport, count=" << transports.size()
// 				      << Log::endl;
  run();
} // add

void TransportScheduler::run() {
  if (running) {
    return;
  }

  running = true;
//   Log::log("TransportScheduler::run") << "creating schedulerThread" << Log::endl;
  runNewThread(&schedulerThread, TransportScheduler::startSchedulerThread, 0, 0);
} // run

void TransportScheduler::halt() {
  ADD_SELECTORS("TransportScheduler::halt");
  maceout << "halting" << Log::endl;
  running = false;
} // halt

void* TransportScheduler::startSchedulerThread(void* arg) {
  runSchedulerThread();
  return 0;
} // startSchedulerThread

void TransportScheduler::runSchedulerThread() {
  ADD_SELECTORS("TransportScheduler::runSchedulerThread");
  SysUtil::signal(SIGPIPE, SIG_IGN);
  uint64_t cumulativeSelect = 0;
  uint64_t cumulativeSelectCount = 0;
  uint64_t shortestSelect = UINT_MAX;
  uint64_t longestSelect = 0;

  uint32_t count = 0;

  while (running) {

    while(!pendingTransports.empty()) {
      BaseTransportPtr t = pendingTransports.front();
      transports.push_back(t);
      pendingTransports.pop();
    }
  
    FD_ZERO(&rset);
    FD_ZERO(&wset);

    int selectMax = 0;
    sig.addToSet(rset, selectMax);

    int n = 0;

    if (count % CHECK_RUNNING_COUNT == 0) {
      lock();
      TransportList::iterator rem = transports.begin();
      while (rem != transports.end()) {
	if (!(*rem)->isRunning()) {
	  rem = transports.erase(rem);
	}
	else {
	  rem++;
	}
      }
      unlock();
    }

    if (transports.empty()) {
      halt();
      continue;
    }

    for (size_t i = 0; i < transports.size(); i++) {
      transports[i]->addSockets(rset, wset, selectMax);
    }

    selectMax++;
    count++;

    struct timeval polltv = { 0, SELECT_TIMEOUT_MICRO };
    uint64_t before = Util::timeu();
    inSelect = true;
    n = SysUtil::select(selectMax, &rset, &wset, 0, &polltv);
    inSelect = false;
    uint64_t diff = Util::timeu() - before;
    cumulativeSelect += (diff);
    cumulativeSelectCount++;
    if(diff < shortestSelect) { shortestSelect = diff; }
    if(diff > longestSelect) { longestSelect = diff; }

    if (n < 0) {
      Log::perror("select");
      assert(0);
    }
    else if (n == 0) {
      continue;
    }

    sig.clear(rset);

    // eventually replace this with a thread pool
    for (size_t i = 0; i < transports.size(); i++) {
      transports[i]->doIO(rset, wset, before);
    }
  }

  transports.clear();
  maceout << "exiting" << Log::endl;
  maceLog("select stats: cumulative = %" PRIu64 " -- count = %" PRIu64 
	  " -- avg = %" PRIu64 "-- min = %" PRIu64 " -- max = %" PRIu64 "\n",
	  cumulativeSelect, cumulativeSelectCount, 
	  (cumulativeSelect/cumulativeSelectCount), shortestSelect, 
	  longestSelect);

} // runSchedulerThread

void TransportScheduler::signal() {
  if (inSelect) {
    inSelect = false;
    sig.signal();
  }
}
