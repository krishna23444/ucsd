/* 
 * CandidateMap.h : part of the Mace toolkit for building distributed systems
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
#include "mhash_map.h"
#include "RandomUtil.h"

#ifndef CANDIDATE_MAP_H
#define CANDIDATE_MAP_H 1

// extern int random_integer(int maximum);

namespace mace {

template<class Key, class Data, class HashFcn = __gnu_cxx::hash<Key>, class EqualKey = std::equal_to<Key>, class Alloc = std::allocator<Data> >

class CandidateMap : public mace::hash_map<Key, Data, HashFcn, EqualKey, Alloc> {

public:
  typedef typename hash_map<Key, Data, HashFcn, EqualKey, Alloc>::iterator iterator;

  CandidateMap() : hash_map<Key, Data, HashFcn, EqualKey, Alloc>(),
		   maxSize((size_t)-1), n(0) {
  }

  CandidateMap(size_t s) : hash_map<Key, Data, HashFcn, EqualKey, Alloc>(),
			   maxSize(s), n(0) {
  } // CandidateMap

  virtual ~CandidateMap() { }

  virtual void setMaxSize(size_t size) {
    maxSize = size;
  }

  void reset() {
    n = 0;
  } // reset

  size_t aggregateCount() {
    if (n < this->size()) {
      n = this->size();
    }
    return n;
  } // aggregateCount

  void compact(CandidateMap<Key, Data, HashFcn, EqualKey, Alloc> cs) {
    hash_map<Key, Data, HashFcn, EqualKey, Alloc> tmp;

    n += cs.size();

    // make sure that we are < maxSize
    while (this->size() > maxSize) {
      randErase();
    }

    while (cs.size() > cs.maxSize) {
      cs.randErase();
    }

    // adjust n
    if (n < this->size()) {
      n = this->size();
    }

    if (cs.n < cs.size()) {
      cs.n = cs.size();
    }

    size_t accum = 0;
    std::pair<Key, Data> v;
    double baseweight = (double)n;
    double weight = (double)cs.n;

    size_t total = this->size() + cs.size();
    if (total > maxSize) {
      total = maxSize;
    }

    while (accum < total) {
      double rnd = drand48();
      if (cs.empty() ||
	  (!this->empty() && (rnd < baseweight / (baseweight + weight)))) {
	v = randErase();
	baseweight--;
      }
      else {
	v = cs.randErase();
	weight--;
      }

      if (tmp.find(v.first) == tmp.end()) {
	accum++;
      }
      tmp[v.first] = v.second;
    }

    this->clear();

    for (iterator i = tmp.begin();
	 i != tmp.end(); i++) {
      operator[](i->first) = i->second;
    }
  } // compact

  void serialize(std::string &s) const throw(SerializationException) {
    hash_map<Key, Data, HashFcn, EqualKey, Alloc>::serialize(s);
    mace::serialize(s, &maxSize, maxSize);
    mace::serialize(s, &n, n);
  } // serialize

  int deserialize(std::istream& in) throw(SerializationException) {
    unsigned offset = hash_map<Key, Data, HashFcn, EqualKey, Alloc>::deserialize(in);
    offset += mace::deserialize(in, &maxSize, maxSize);
    offset += mace::deserialize(in, &n, n);
    return offset;
  } // deserialize

private:
  std::pair<Key, Data> randErase() {
    size_t pos = RandomUtil::randInt(this->size());
    iterator i = this->begin();
    for (size_t count = 0; count < pos; count++, i++);
    std::pair<Key, Data> p = *i;
    erase(i);
    return p;
  } // randErase
  
private:
  size_t maxSize;

  // total number of nodes processed so far
  size_t n;
};

}
#endif // CANDIDATE_MAP_H
