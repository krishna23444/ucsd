/* 
 * DHTServiceManager.cc : part of the Mace toolkit for building distributed systems
 * 
 * Copyright (c) 2005, James W. Anderson, Ryan Braud, Charles Killian, John Fisher-Ogden, Calvin Hubble
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
#include <assert.h>
#include <sys/time.h>
#include <errno.h>

#include "DHTServiceManager.h"

using mace::string;

const int DHT_RESPONSE_TIMEOUT_SEC = 5;
const int DHT_RESPONSE_TIMEOUT_NSEC = 0;

DHTServiceManager::DHTServiceManager(DHTServiceClass& dhtsc) : dht(dhtsc) {
  rid = dht.registerHandler(*this);

  pthread_mutex_init(&mapLock, 0);
} // DHTServiceManager

DHTServiceManager::~DHTServiceManager() {
  pthread_mutex_destroy(&mapLock);
} // ~DHTServiceManager

void DHTServiceManager::dhtContainsKeyResult(const MaceKey& k, bool result,
					     registration_uid_t rid) {
  lookups[k] = result;
  if (!signalResult(k)) {
    lookups.erase(k);
  }
} // dhtContainsKeyResult

void DHTServiceManager::dhtGetResult(const MaceKey& k, const mace::string& v, bool found,
				     registration_uid_t rid) {
  lookups[k] = found;
  values[k] = v;
  if (!signalResult(k)) {
    lookups.erase(k);
    values.erase(k);
  }
} // dhtGetResult

bool DHTServiceManager::containsKey(const MaceKey& k) {
  acquireLock(k);
  bool result;
  bool waitForCallback;
  bool retry = false;
  do {
    dht.containsKey(k, result, waitForCallback, rid);
    if (waitForCallback) {
      if (waitForSignal(k)) {
	result = lookups[k];
	lookups.erase(k);
	retry = false;
      }
      else {
	retry = true;
      }
    }
  } while (retry);
  releaseLock(k);
  return result;
} // containsKey

void DHTServiceManager::get(const MaceKey& k, string& v, bool& found) {
  acquireLock(k);
  bool waitForCallback;
  bool retry = false;
  do {
    dht.get(k, v, found, waitForCallback, rid);
    if (waitForCallback) {
      if (waitForSignal(k)) {
	v = values[k];
	found = lookups[k];
	values.erase(k);
	lookups.erase(k);
	retry = false;
      }
      else {
	retry = true;
      }
    }
  } while (retry);
  releaseLock(k);
} // get

void DHTServiceManager::put(const MaceKey& k, const string& v) {
  dht.put(k, v);
} // put

void DHTServiceManager::remove(const MaceKey& k) {
  dht.remove(k);
} // remove

void DHTServiceManager::acquireLock(const MaceKey& k) {
  pthread_mutex_lock(&mapLock);
  pthread_mutex_t* l = 0;
  if (locks.containsKey(k)) {
    counts[k]++;
    l = locks[k];
    assert(l);
  }
  else {
    assert(counts[k] == 0);
    counts[k]++;
    l = new pthread_mutex_t;
    pthread_mutex_init(l, 0);
    pthread_cond_t* s = new pthread_cond_t;
    pthread_cond_init(s, 0);
    locks[k] = l;
    signals[k] = s;
  }
  pthread_mutex_unlock(&mapLock);
  assert(pthread_mutex_lock(l) == 0);
} // acquireLock

bool DHTServiceManager::waitForSignal(const MaceKey& k) {
  // assumes that acquireLock was already called for k
  pthread_mutex_lock(&mapLock);

  assert(locks.containsKey(k));
  assert(signals.containsKey(k));
  pthread_mutex_t* l = locks[k];
  assert(l);
  pthread_cond_t* s = signals[k];
  assert(s);

  pthread_mutex_unlock(&mapLock);

  struct timespec timeout;
  struct timeval now;
  gettimeofday(&now, 0);
  timeout.tv_sec = now.tv_sec + DHT_RESPONSE_TIMEOUT_SEC;
  timeout.tv_nsec = (now.tv_usec * 1000) + DHT_RESPONSE_TIMEOUT_NSEC;
  int r = pthread_cond_timedwait(s, l, &timeout);
  if (r == ETIMEDOUT) {
    return false;
  }
  assert(r == 0);
  return true;
} // waitForSignal

bool DHTServiceManager::signalResult(const MaceKey& k) {
  pthread_mutex_lock(&mapLock);

  bool sentSignal = false;
  if (signals.containsKey(k)) {
    pthread_cond_t* s = signals[k];

    assert(s);
    assert(pthread_cond_broadcast(s) == 0);

    sentSignal = true;
  }

  pthread_mutex_unlock(&mapLock);
  return sentSignal;
} // signalResult

void DHTServiceManager::releaseLock(const MaceKey& k) {
  pthread_mutex_lock(&mapLock);
  pthread_mutex_t* l = locks[k];
  assert(l);
  pthread_cond_t* s = signals[k];
  assert(s);

  assert(pthread_mutex_unlock(l) == 0);

  counts[k]--;
  if (counts[k] == 0) {
    pthread_mutex_destroy(l);
    pthread_cond_destroy(s);
    locks.erase(k);
    signals.erase(k);
    delete l;
    delete s;
    l = 0;
    s = 0;
  }
  
  pthread_mutex_unlock(&mapLock);
} // releaseLock
