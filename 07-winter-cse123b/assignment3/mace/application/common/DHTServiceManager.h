/* 
 * DHTServiceManager.h : part of the Mace toolkit for building distributed systems
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
#ifndef DHT_SERVICE_MANAGER_H
#define DHT_SERVICE_MANAGER_H

#include <pthread.h>
#include "DHTServiceClass.h"
#include "mhash_map.h"

class DHTServiceManager : public DHTDataHandler {

  typedef mace::hash_map<MaceKey, pthread_mutex_t*> KeyLockMap;
  typedef mace::hash_map<MaceKey, pthread_cond_t*> KeySignalMap;
  typedef mace::hash_map<MaceKey, uint> KeyCountMap;
  typedef mace::hash_map<MaceKey, bool> KeyBoolMap;
  typedef mace::hash_map<MaceKey, mace::string> KeyStringMap;

public:
  DHTServiceManager(DHTServiceClass& dhtsc);
  virtual ~DHTServiceManager();
  virtual bool containsKey(const MaceKey& key);
  virtual void get(const MaceKey& key, mace::string& value, bool& found);
  virtual void put(const MaceKey& key, const mace::string& value);
  virtual void remove(const MaceKey& key);
  virtual void dhtContainsKeyResult(const MaceKey& key, bool result,
				    registration_uid_t rid);
  virtual void dhtGetResult(const MaceKey& key, const mace::string& value, bool found,
			    registration_uid_t rid);

private:
  void acquireLock(const MaceKey& key);
  void releaseLock(const MaceKey& key);
  // returns false if timed out, true if signaled
  bool waitForSignal(const MaceKey& key);
  // returns true if the result was signaled
  bool signalResult(const MaceKey& key);

private:
  DHTServiceClass& dht;
  registration_uid_t rid;
  KeyLockMap locks;
  KeySignalMap signals;
  KeyCountMap counts;

  KeyBoolMap lookups;
  KeyStringMap values;

  pthread_mutex_t mapLock;
}; // DHTServiceManager

#endif // DHT_SERVICE_MANAGER_H
