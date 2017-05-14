/* 
 * LRUCache.h : part of the Mace toolkit for building distributed systems
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
#include <cassert>
#include <pthread.h>

#include "mstring.h"
#include "mhash_map.h"

#include "Log.h"

#ifndef LRUCACHE_H
#define LRUCACHE_H

// Provides a cache with a least recently used replacement policy and
// a map like interface.
//
// template parameters: Key Value HashFunction EqualKey
//
// API:
//
// LRUCache(capacity, timeout) - create a new cache with the specified
// capacity.  If a timeout is given, then entries will be expired
// after the specified timeout.
//
// The add methods will evict the least recently used non-dirty key.
// If all the indices are dirty, then the least recently used dirty
// key will be evicted.
//
// Use timeouts with caution---if a dirty entry expires, it will be
// deleted the next time containsKey is called for its entry.  Because
// dirty entries may be deleted before they have a chance to be
// cleared, it is advised to not use a timeout when storing dirty data
// in the cache.
//
// void addDirty(key, value) - add a new entry to the cache, mark as dirty
// void add(key, value, dirty) - add a new entry to the cache, set dirty as specified
// void remove(key) - removes the entry from the cache
// bool containsKey(key) - true if the cache contains the key

// * NB: you MUST call containsKey and check that the result is true
//       before calling get, operator[], or obtain the key by getLastDirtyKey

// value& get(key) / operator[](key) - lookup the value for the given key
// bool isFullDirty() - true if the cache is full, and all entries are dirty
// bool hasDirty() - true if any entries are dirty
// key getLastDirtyKey() - return the least recently accessed dirty key
// value& getLastDirty() - return the least recently accessed dirty value
// void clearDirty(key) - clear the dirty bit for the given key
// value& getDirty(key) - get the value of the given key, but do not
//   adjust access status: the key remains in its current position with
//   respect to how recently it was used

namespace mace {

template <typename K, typename D, typename HashFcn = __gnu_cxx::hash<K>,
	  typename EqualKey = std::equal_to<K> >

class LRUCache {

  class CacheEntry {
  public:
    CacheEntry(const K& index, const D& d, bool dirty, time_t t) :
      index(index), data(d), dirty(dirty), init(t), prev(0), next(0) {
    } // CacheEntry

    virtual ~CacheEntry() {
    }

  public:
    K index;
    D data;
    bool dirty;
    time_t init;
    CacheEntry* prev;
    CacheEntry* next;
  }; // CacheEntry

  typedef hash_map<K, CacheEntry*, HashFcn, EqualKey> LRUCacheIndexMap;

public:
  LRUCache(unsigned capacity = 32, time_t timeout = 0) :
    capacity(capacity), timeout(timeout), size(0), dirtyCount(0), cache(0) {

    pthread_mutexattr_t ma;
    assert(pthread_mutexattr_init(&ma) == 0);
    assert(pthread_mutexattr_settype(&ma, PTHREAD_MUTEX_RECURSIVE) == 0);
    pthread_mutex_init(&lrulock, &ma);
    assert(pthread_mutexattr_destroy(&ma) == 0);
  } // LRUCache

  virtual ~LRUCache() {
    lock();
    while (cache != 0) {
      CacheEntry* tmp = cache;
      cache = cache->next;
      indexMap.erase(tmp->index);
      delete tmp;
      tmp = 0;
    }
    unlock();
    pthread_mutex_destroy(&lrulock);
  } // ~LRUCache

  void addDirty(const K& index, const D& buf) {
    add(index, buf, true);
  } // addDirty

  void add(const K& index, const D& buf, bool dirty = false) {
    lock();
    if (indexMap.containsKey(index)) {
      // If you add a block that is already in the cache, you overwrite
      // it unconditionally.  If the old block was dirty and the new one
      // is not, then you lose the dirty data
      CacheEntry* e = indexMap[index];
      e->data = buf;
      if (e->dirty && !dirty) {
	dirtyCount--;
      }
      else if (!e->dirty && dirty) {
	dirtyCount++;
      }
      e->dirty = dirty;
      moveToHead(e);
      unlock();
      return;
    }

    if (isFullDirty()) {
      clearDirty(getLastDirtyKey());
    }
    
    assert(!isFullDirty());

    if (size == capacity) {
      // evict the lru non-dirty block
      CacheEntry* last = cache->prev;

      while (last->dirty) {
	last = last->prev;
      }

      remove(last->index);

    }

    assert(size < capacity);

    size++;
    if (dirty) {
      dirtyCount++;
    }

    time_t now = time(0);
    CacheEntry* e = new CacheEntry(index, buf, dirty, now);
    if (cache == 0) {
      e->prev = e;
    }
    else {
      e->next = cache;
      e->prev = cache->prev;
      cache->prev = e;
    }
    cache = e;
    indexMap[index] = e;
    unlock();
  } // add

  void remove(const K& index) {
    lock();
    typename LRUCacheIndexMap::iterator i = indexMap.find(index);
    if (i != indexMap.end()) {
      CacheEntry* e = i->second;

      clearDirty(e);

      if (e->next) {
	e->next->prev = e->prev;
      }
      else {
	cache->prev = e->prev;
      }

      if (cache == e) {
	cache = e->next;
      }
      else {
        //Before, this would complete the loop if the first element were removed.
        e->prev->next = e->next;
      }

      delete e;
      e = 0;
      indexMap.erase(i);
      size--;

      ASSERT(indexMap.size() == size);
      if (size == 0) {
	assert(cache == 0);
      }
    }
    unlock();
  } // remove
    
  D& get(const K& index) {
    lock();
    assert(indexMap.containsKey(index));

    CacheEntry* e = indexMap[index];
    moveToHead(e);
    unlock();
    return e->data;
  } // get

  D& operator[](const K& index) {
    return get(index);
  } // operator[]

  bool containsKey(const K& index) {
    lock();
    bool c = indexMap.containsKey(index);
    if (c && (timeout != 0)) {
      time_t now = time(0);
      CacheEntry* e = indexMap[index];
      if ((now - e->init) > timeout) {
	remove(index);
	unlock();
	return false;
      }
    }

    unlock();
    return c;
  } // containsKey

  bool isFullDirty() const {
    return (dirtyCount == capacity);
  } // isFullDirty

  bool hasDirty() const {
    return (dirtyCount > 0);
  } // hasDirty

  K getLastDirtyKey() const {
    lock();
    assert(hasDirty());

    CacheEntry* last = cache->prev;
    while (!last->dirty) {
      last = last->prev;
    }

    unlock();
    return last->index;
  } // getLastDirtyKey

  D& getLastDirty() const {
    lock();

    assert(hasDirty());

    CacheEntry* last = cache->prev;
    while (!last->dirty) {
      last = last->prev;
    }

    unlock();
    return last->data;
  } // getLastDirty

  void clearDirty(const K& index) {
    lock();
    assert(indexMap.containsKey(index));
    
    CacheEntry* e = indexMap[index];
    clearDirty(e);
    unlock();
  } // clearDirty

  // this does not move the index to the head of the cache
  D& getDirty(const K& index) {
    lock();
    assert(indexMap.containsKey(index));

    CacheEntry* e = indexMap[index];
    unlock();
    return e->data;
  } // getDirty

private:
  void clearDirty(CacheEntry* e) {
    if (e->dirty) {
      dirtyCount--;
      e->dirty = false;
    }
  }
    
  void moveToHead(CacheEntry* e) {
    if (e->index == cache->index) {
      assert(cache == e);
      return;
    }
    e->prev->next = e->next;
    if (e->next) {
      e->next->prev = e->prev;
      e->prev = cache->prev;
    }
    e->next = cache;
    cache->prev = e;
    cache = e;
  } // moveToHead

  void lock() const {
    assert(pthread_mutex_lock(&lrulock) == 0);
  } // lock

  void unlock() const {
    assert(pthread_mutex_unlock(&lrulock) == 0);
  } // unlock
  

private:
  unsigned capacity;
  time_t timeout;
  unsigned size;
  unsigned dirtyCount;
  CacheEntry* cache;
  LRUCacheIndexMap indexMap;
  mutable pthread_mutex_t lrulock;
};


} // namespace mace

#endif // LRUCACHE_H
