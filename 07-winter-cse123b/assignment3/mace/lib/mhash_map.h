/* 
 * mhash_map.h : part of the Mace toolkit for building distributed systems
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
#if __GNUC__ >= 3
#include <ext/hash_map>
#else
#include <hash_map>
#endif

#include "Serializable.h"
#include "Traits.h"
#include "XmlRpcCollection.h"
#include "RandomUtil.h"
#include "mace_constants.h"
#include "StrUtilNamespace.h"

#ifndef _MACE_HASH_MAP_H
#define _MACE_HASH_MAP_H

namespace mace {

template<class Key, class Data,
	 class HashFcn = __gnu_cxx::hash<Key>,
	 class EqualKey = std::equal_to<Key>,
	 class Alloc = std::allocator<Data> >
class hash_map : public __gnu_cxx::hash_map<Key, Data, HashFcn, EqualKey, Alloc>, public Serializable, public PrintPrintable {

public:
  typedef typename __gnu_cxx::hash_map<Key, Data, HashFcn, EqualKey, Alloc> baseType;
  typedef typename baseType::iterator iterator;
  typedef typename baseType::const_iterator const_iterator;
  typedef typename baseType::size_type size_type;

  virtual ~hash_map() { }

  bool containsKey(const Key& k) const {
    return find(k) != this->end();
  } // containsKey

  const Data& get(const Key& k) const {
//     static const Data defaultValue = Data();
    const_iterator i = find(k);
    if (i != this->end()) {
      return i->second;
    }
    ASSERT(0);
//     return defaultValue;
  } // get

  int deserialize(std::istream& in) throw(SerializationException) {
    Key T;
    size_type sz;
    int bytes = sizeof(sz);

    this->clear();
    mace::deserialize(in, &sz, sz);
    for(size_type i = 0; i < sz; i++) {
      bytes += mace::deserialize(in, &T, T);
      bytes += mace::deserialize(in, &operator[](T), operator[](T));
    }
    return bytes;
  }

  int deserializeXML_RPC(std::istream& in) throw(SerializationException) {
    return XmlRpcMapDeserialize<hash_map<Key, Data, HashFcn, EqualKey, Alloc>, Key, Data>(in, *this);
  }

  void serialize(std::string &str) const {
    size_type sz = this->size();
    mace::serialize(str, &sz, sz);
    for(const_iterator i=this->begin(); i!=this->end(); i++) {
      mace::serialize(str, &(i->first), i->first);
      mace::serialize(str, &(i->second), i->second);
    }
  }

  void serializeXML_RPC(std::string& str) const throw(SerializationException) {
    if(!KeyTraits<Key>::isString() && !KeyTraits<Key>::isInt()) {
      throw SerializationException("Bad key type for hash map serialization");
    }
    XmlRpcMapSerialize<const_iterator, Key>(str, this->begin(), this->end());
  }

  const_iterator random() const {
    if(this->size()==0) { return this->end(); }
    int rand = RandomUtil::randInt(this->size());
    const_iterator i;
    int position;
    for(i = this->begin(), position=0; i != this->end() && position < rand; i++, position++);
    return i;
  }

  const std::string& getTypeName() const {
    const char* types[] = { "Key", "Data", "HashFcn", "EqualKey", "Alloc", 0 };
    static const StrUtilNamespace::StdStringList myTypes = StrUtilNamespace::getTypeFromTemplate(__PRETTY_FUNCTION__, types);
    static string ret = myTypes[0]+"->"+myTypes[1];
    return ret;
  }

  void print(std::ostream& out) const {
    if(mace::PRINT_TYPE_NAMES) {
      out << "mace::HashMap<" << getTypeName() << ">";
    }
    mace::printMap(out, this->begin(), this->end());
  }
  void printState(std::ostream& out) const {
    if(mace::PRINT_TYPE_NAMES) {
      out << "mace::HashMap<" << getTypeName() << ">";
    }
    mace::printMapState(out, this->begin(), this->end());
  }
};

}
#endif // _MACE_HASH_MAP_H
