/* 
 * AddressCache.h : part of the Mace toolkit for building distributed systems
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
#ifndef MACEDON_CACHE_H

#define MACEDON_CACHE_H
#define MAX_CACHE_SIZE 100
#define MAX_CACHE_LIFE 10 * 1000 * 1000 //usec

#include "MaceTypes.h"
#include "NodeCollection.h"
#include "Printable.h"
#include "Util.h"

namespace mace {

class AddressCacheEntry : public mace::PrintPrintable {
  private:
    MaceKey _id;

  public:
    MaceKey start;
    MaceKey end;
    uint64_t time;

    void print(std::ostream&) const;
    MaceKey getId() const { return _id; }
    double getScore() const { return time; }
    AddressCacheEntry() : _id(), start(), end(), time() {}
    AddressCacheEntry(const MaceKey& id) : _id(id), start(), end(), time(Util::timeu()) {}
    AddressCacheEntry(const MaceKey& id, const MaceKey& b, const MaceKey& e) : _id(id), start(b), end(e), time(Util::timeu()) {}
    virtual ~AddressCacheEntry() {}
};

class AddressCache
{
public:
  AddressCache () : entries() {} 
  ~AddressCache () {}
  void insert(const MaceKey& start, const MaceKey& end, const MaceKey& address);
  void remove(const MaceKey& address);
  MaceKey query(const MaceKey&);
  void verify(const MaceKey& hash, const MaceKey& address);
private:
  typedef NodeCollection<AddressCacheEntry, MAX_CACHE_SIZE> cachemap;
  void consistency();
  cachemap entries;
};

}

#endif
