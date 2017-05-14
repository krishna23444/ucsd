/* 
 * Iterator.h : part of the Mace toolkit for building distributed systems
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
#include <map>
// #define DEBUG_ITER
#ifdef DEBUG_ITER
#include "Log.h"
#endif

#ifndef _MACE_ITERATOR_H
#define _MACE_ITERATOR_H

namespace mace {

  // template <typename K, typename V>
  template <typename M>
class MapIterator
{
private:
  //   map<K, V>::iterator iter;
  //   map<K, V>::iterator *last;
  typedef typename M::iterator iterator;
  typedef typename M::const_iterator const_iterator;
  typedef typename M::mapped_type V;
  typedef typename M::key_type K;
  iterator iter;
  iterator last;
  //   map<K, V>& set;
  M& set;
  const iterator begin;
  const iterator end;

public:
  typedef typename M::mapped_type mapped_type;
  typedef typename M::key_type key_type;

  //   MapIterator(map<K, V> foo): set(foo) {}
  MapIterator(M& s): iter(s.begin()), set(s), begin(s.begin()), end(s.end()) {
    last = end;
#ifdef DEBUG_ITER
    Log::logf("MAP_ITER_DEBUG", "size: %d\n", set.size());
#endif
  }
  
  MapIterator(M& s, iterator b): set(s), begin(b), end(s.end()), iter(b) {
    last = end;
#ifdef DEBUG_ITER
    Log::logf("MAP_ITER_DEBUG", "size: %d\n", set.size());
#endif
  }
  
  MapIterator(M& s, iterator b, iterator e): iter(b), set(s), begin(b), end(e) {
    last = end;
#ifdef DEBUG_ITER
    Log::logf("MAP_ITER_DEBUG", "size: %d\n", set.size());
#endif
  }

  bool hasNext() {
    return iter != end && iter != set.end();
  }

  V& next() {
    ASSERT(hasNext());
#ifdef DEBUG_ITER
    Log::logf("MAP_ITER_DEBUG", "next(): iter %x\n", iter->first);
#endif
    last = iter;
    iter++;
    return last->second;
  }

  V& next(K &key) {
    ASSERT(hasNext());
#ifdef DEBUG_ITER
    Log::logf("MAP_ITER_DEBUG", "next(): iter %x\n", iter->first);
#endif
    last = iter;
    iter++;
    key = last->first;
    return last->second;
  }

  void remove() {
#ifdef DEBUG_ITER
    Log::logf("MAP_ITER_DEBUG", "remove(): last %x\n", last->first);
#endif
    if(last != end) {
      set.erase(last);
      last = end;
    } else {
      ASSERT(false);
    }
  }

};

  template <typename M>
class ConstMapIterator
{
private:
  //   map<K, V>::iterator iter;
  //   map<K, V>::iterator *last;
  typedef typename M::iterator iterator;
  typedef typename M::const_iterator const_iterator;
  typedef typename M::mapped_type V;
  typedef typename M::key_type K;
  const_iterator iter;
  const_iterator last;
  //   map<K, V>& set;
  const M& set;
  const const_iterator begin;
  const const_iterator end;

public:
  typedef typename M::mapped_type mapped_type;
  typedef typename M::key_type key_type;
  //   MapIterator(map<K, V> foo): set(foo) {}
  ConstMapIterator(const M& s): set(s), begin(s.begin()), end(s.end()) {
    iter = begin;
    last = end;
#ifdef DEBUG_ITER
    Log::logf("MAP_ITER_DEBUG::const", "size: %d\n", set.size());
#endif
  }
  
  ConstMapIterator(const M& s, const_iterator b): set(s), begin(b), end(s.end()) {
    iter = begin;
    last = end;
#ifdef DEBUG_ITER
    Log::logf("MAP_ITER_DEBUG::const", "size: %d\n", set.size());
#endif
  }
  
  ConstMapIterator(const M& s, const_iterator b, const_iterator e): set(s), begin(b), end(e) {
    iter = begin;
    last = end;
#ifdef DEBUG_ITER
    Log::logf("MAP_ITER_DEBUG::const", "size: %d\n", set.size());
#endif
  }

  bool hasNext() {
    return iter != end && iter != set.end();
  }

  const V& next() {
    ASSERT(hasNext());
#ifdef DEBUG_ITER
    Log::logf("MAP_ITER_DEBUG::const", "next(): iter %x\n", iter->first);
#endif
    last = iter;
    iter++;
    return last->second;
  }

  const V& next(K &key) {
    ASSERT(hasNext());
#ifdef DEBUG_ITER
    Log::logf("MAP_ITER_DEBUG::const", "next(): iter %x\n", iter->first);
#endif
    last = iter;
    iter++;
    key = last->first;
    return last->second;
  }

};

}

#endif //_MACE_ITERATOR_H
