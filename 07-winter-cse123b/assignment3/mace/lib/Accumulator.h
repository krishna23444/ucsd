/* 
 * Accumulator.h : part of the Mace toolkit for building distributed systems
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
#ifndef ACCUMULATOR_H
#define ACCUMULATOR_H

#include <cassert>
#include <string>
#include "mhash_map.h"
#include "Log.h"
// #include "SysUtil.h"
#include "Util.h"
#include "ScopedLock.h"

//NOTE: This could build off of the statistical filter, and compute a wide variety of statistics.
class Accumulator {
public:
  static const std::string NETWORK_READ;
  static const std::string NETWORK_READ_SELECTOR;
  static const std::string NETWORK_WRITE;
  static const std::string NETWORK_WRITE_SELECTOR;
  static const std::string TCP_READ;
  static const std::string TCP_READ_SELECTOR;
  static const std::string TCP_WRITE;
  static const std::string TCP_WRITE_SELECTOR;
  static const std::string UDP_READ;
  static const std::string UDP_READ_SELECTOR;
  static const std::string UDP_WRITE;
  static const std::string UDP_WRITE_SELECTOR;
  static const std::string HTTP_CLIENT_READ;
  static const std::string HTTP_CLIENT_READ_SELECTOR;
  static const std::string HTTP_CLIENT_WRITE;
  static const std::string HTTP_CLIENT_WRITE_SELECTOR;
  static const std::string HTTP_SERVER_READ;
  static const std::string HTTP_SERVER_READ_SELECTOR;
  static const std::string HTTP_SERVER_WRITE;
  static const std::string HTTP_SERVER_WRITE_SELECTOR;
  static const std::string TRANSPORT_SEND;
  static const std::string TRANSPORT_SEND_SELECTOR;
  static const std::string TRANSPORT_RECV;
  static const std::string TRANSPORT_RECV_SELECTOR;
  static const std::string TRANSPORT_RECV_CANCELED;
  static const std::string TRANSPORT_RECV_CANCELED_SELECTOR;
  static const std::string FILE_WRITE;
  static const std::string FILE_WRITE_SELECTOR;
  static const std::string APPLICATION_RECV;
  static const std::string APPLICATION_RECV_SELECTOR;
  static const std::string APPLICATION_SEND;
  static const std::string APPLICATION_SEND_SELECTOR;

public:
  static Accumulator* Instance(const std::string& counter) {
    ScopedLock sl(alock);
    Accumulator* r = 0;
    AccumulatorMap::iterator i = instances.find(counter);
    if (i == instances.end()) {
      r = new Accumulator();
      r->logId = Log::getId("Accumulator::" + counter);
      instances[counter] = r;
    }
    else {
      r = i->second;
    }
    return r;
  }

  void startTimer() {
    startTime = 0;
    useTimer = true;
  }

  void stopTimer() { useTimer = false; }

  void accumulate(uint amount) {
    ScopedLock sl(alock);
    accumulateUnlocked(amount);
  }

  void accumulateUnlocked(uint amount) {
    totalBytes += amount;
    if (useTimer) {
      lastTime = Util::timeu();
      if (startTime == 0) {
	startTime = lastTime;
      }
    }
  }

  uint64_t getAmount() const { return totalBytes; }
  uint64_t getDiff() const { return totalBytes - diff; }
  uint64_t resetDiff() {
    // leave this unlocked so that we do not need a recursive lock
    uint64_t r = totalBytes - diff;
    diff = totalBytes;
    return r;
  }

  uint64_t getStartTime() { return startTime; }
  uint64_t getLastTime() { return lastTime; }

  static void dumpAll();
  static void logAll();
  static void startLogging(uint64_t interval = 1*1000*1000);
  static void stopLogging();

protected:
  static void* startLoggingThread(void* arg);

private:
  static void lock() {
    assert(pthread_mutex_lock(&alock) == 0);
  }
  static void unlock() {
    assert(pthread_mutex_unlock(&alock) == 0);
  }


private:
  typedef mace::hash_map<std::string, Accumulator*, hash_string> AccumulatorMap;
  static AccumulatorMap instances;
  static bool isLogging;
  static pthread_t athread;
  static pthread_mutex_t alock;

  uint64_t totalBytes;
  uint64_t diff;
  bool useTimer;
  uint64_t startTime;
  uint64_t lastTime;
  log_id_t logId;
  
  Accumulator() : totalBytes(0), diff(0), useTimer(false), startTime(0), lastTime(0) {}
}; // class Accumulator

#endif // ACCUMULATOR_H
