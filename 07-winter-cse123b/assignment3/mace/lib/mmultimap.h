/* 
 * mmultimap.h : part of the Mace toolkit for building distributed systems
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
#include "mdeque.h"
#include "mpair.h"
#include "XmlRpcCollection.h"
#include "Serializable.h"
#include "RandomUtil.h"
#include "mace_constants.h"
#include "StrUtil.h"

#ifndef _MACE_MULTIMAP_H
#define _MACE_MULTIMAP_H

namespace mace {

  template<class Key, class Data, class Compare = std::less<Key>, class Alloc = std::allocator<std::pair<const Key, Data> > >
class multimap : public std::multimap<Key, Data, Compare, Alloc>, public Serializable, public PrintPrintable {

public:
  typedef typename std::multimap<Key, Data, Compare, Alloc> baseType;
  typedef typename baseType::iterator iterator;
  typedef typename baseType::const_iterator const_iterator;
  typedef typename baseType::size_type size_type;
  typedef typename baseType::key_compare key_compare;

  multimap() : baseType() {
  }

  multimap(const key_compare& comp) : baseType(comp) {
  }

  virtual ~multimap() { }

  bool containsKey(const Key& k) const {
    return find(k) != this->end();
  } // containsKey

  deque<Data> get(const Key& k) const {
    deque<Data> l;
    for (const_iterator i = find(k); i != this->end() && i->first == k; i++) {
      l.push_back(i->second);
    }
    return l;
  } // get
  
  int deserialize(std::istream& in) throw(SerializationException) {
    size_type sz;
    int bytes = sizeof(sz);

    this->clear();
    mace::deserialize(in, &sz, sz);
    for(size_type i = 0; i < sz; i++) {
      Key T;
      Data d;
      bytes += mace::deserialize(in, &T, T);
      bytes += mace::deserialize(in, &d, d);
      insert(std::pair<Key, Data>(T, d));
    }
    return bytes;
  }

  // will not work with duplicate keys
//   int deserializeXML_RPC(std::istream& in) throw(SerializationException) {    
//     return XmlRpcMapDeserialize<map<Key, Data, Compare, Alloc>, Key, Data>(in, *this);
//   }

  void serialize(std::string &str) const {
    size_type sz = this->size();
    mace::serialize(str, &sz, sz);
    for(const_iterator i=this->begin(); i!=this->end(); i++) {
      mace::serialize(str, &(i->first), i->first);
      mace::serialize(str, &(i->second), i->second);
    }
  }

  // will not work with duplicate keys
//   void serializeXML_RPC(std::string& str) const throw(SerializationException) {
//     if(!KeyTraits<Key>::isString() && !KeyTraits<Key>::isInt()) {
//       throw SerializationException("Bad key type for hash map serialization");
//     }
//     XmlRpcMapSerialize<const_iterator, Key>(str, this->begin(), this->end());
//   }

  const_iterator random() const {
    if(this->size()==0) { return this->end(); }
    int rand = RandomUtil::randInt(this->size());
    const_iterator i;
    int position;
    for(i = this->begin(), position=0; i != this->end() && position < rand; i++, position++);
    return i;
  }

  const std::string& getTypeName() const {
    const char* types[] = { "Key", "Data", "Compare", "Alloc", 0 };
    static const StrUtilNamespace::StdStringList myTypes = StrUtilNamespace::getTypeFromTemplate(__PRETTY_FUNCTION__, types);
    static string ret = myTypes[0]+"->"+myTypes[1];
    return ret;
  }

  void print(std::ostream& out) const {
    if(mace::PRINT_TYPE_NAMES) {
      out << "mace::MultiMap<" << getTypeName() << ">";
    }
    mace::printMap(out, this->begin(), this->end());
  }
  void printState(std::ostream& out) const {
    if(mace::PRINT_TYPE_NAMES) {
      out << "mace::MultiMap<" << getTypeName() << ">";
    }
    mace::printMapState(out, this->begin(), this->end());
  }

//   std::string toString() const {
//     std::ostringstream out;
//     out << "mace::Map{ ";
//     for(const_iterator i = this->begin(); i != this->end() ; i++) {
//       out << " (" << mace::convertToString(&(i->first), i->first) << " -> "
// 	  << mace::convertToString(&(i->second), i->second) << ")";
//     }
//     out << " }";
//     return out.str();
//   }
};

}
#endif // _MACE_MULTIMAP_H
