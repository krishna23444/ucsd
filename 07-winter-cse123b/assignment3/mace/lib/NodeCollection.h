/* 
 * NodeCollection.h : part of the Mace toolkit for building distributed systems
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
// #include <algorithm>
#include <float.h>

#include "mace.h"
// #include "m_map.h"
#include <map>
#include "mset.h"
#include "XmlRpcCollection.h"
#include "Serializable.h"
#include "Iterator.h"
#include "MaceTypes.h"
#include "RandomUtil.h"
#include "StrUtil.h"
#include "Traits.h"

#ifndef _NEIGHBOR_SET_H
#define _NEIGHBOR_SET_H

namespace mace {

template <class NodeType, size_t MaxSize = UINT_MAX>
class NodeCollection : private std::map<MaceKey, NodeType>, public Serializable, public PrintPrintable
{
public: 
  typedef std::map<MaceKey, NodeType> mapType;
  typedef typename mapType::iterator iterator;
  typedef typename mapType::const_iterator const_iterator;
  typedef typename mapType::size_type size_type;
  typedef MapIterator<NodeCollection<NodeType, MaxSize> > map_iterator;
  typedef ConstMapIterator<NodeCollection<NodeType, MaxSize> > const_map_iterator;
  typedef NodeType mapped_type;

private: 
  //These are the things which map_iterator needs.
  friend class MapIterator<NodeCollection<NodeType, MaxSize> >;
  friend class ConstMapIterator<NodeCollection<NodeType, MaxSize> >;
    
  friend int XmlRpcMapDeserialize<NodeCollection<NodeType, MaxSize>, MaceKey, NodeType>(std:: istream& in, NodeCollection<NodeType, MaxSize>& obj);
    
  //     using mapType::erase;
  void erase(iterator i) { myNodeSet.erase(i->first); mapType::erase(i); }
  //     iterator begin() { return mapType::begin(); }
  //     const_iterator begin() const { return mapType::begin(); }
  //     iterator end() { return mapType::end(); }
  //     const_iterator end() const { return mapType::end(); }
    
  NodeSet myNodeSet;

public:
  using mapType::begin;
  using mapType::end;
  using mapType::find;
  using mapType::lower_bound;
  using mapType::upper_bound;

  inline size_t maxSize() const {
    return MaxSize;
  }

  inline size_t maxsize() const {
    return MaxSize;
  }

  NodeCollection() : mapType(), myNodeSet() { }
  virtual ~NodeCollection() {}

  bool contains(const MaceKey& who) const {
    //       return mapType::find(who) != end();
    return myNodeSet.contains(who);
  }
  bool containsKey(const MaceKey& who) const {
    return this->contains(who);
  }

  using mapType::size;
  using mapType::empty;

  NodeType& get(const MaceKey& who) {
    iterator i = mapType::find(who);
    if (i == end()) {
      ASSERT(false);
    }
    return i->second;
  }

  const NodeType& get(const MaceKey& who) const {
    const_iterator i = mapType::find(who);
    if (i == end()) {
      ASSERT(false);
    }
    return i->second;
  }

  NodeType& add(const MaceKey& who) {
    iterator i = mapType::find(who);
    if (i == end() && size() < MaxSize) {
      myNodeSet.insert(who);
      return (this->operator[](who) = NodeType(who));
    }
    else if (i == end()) {
      ASSERT(false && size() < MaxSize);
    }
    else {
      return (i->second);
    }
  }

  NodeType& add(const NodeType& nbr) {
    iterator i = mapType::find(nbr.getId());
    if (i == end() && size() < MaxSize) {
      myNodeSet.insert(nbr.getId());
      return (this->operator[](nbr.getId()) = nbr);
    }
    else if (i == end()) {
      ASSERT(false && size() < MaxSize); // die
    }
    else {
      return (i->second = nbr);
    }
  }

  void clear() {
    //This stub here so we can add stuff when a clear is called.
    if(!myNodeSet.empty()) {
      myNodeSet.clear();
      mapType::clear();
    }
  }

  size_type erase(const MaceKey& who) {
    //This stub here so we can add magic stuff.
    myNodeSet.erase(who);
    return mapType::erase(who);
  }

  size_type erase(const NodeType& who) {
    myNodeSet.erase(who.getId());
    return mapType::erase(who.getId());
  }

  template<typename S>
  bool query(S (NodeType::*val)(void), S v) {
    for(iterator i=begin(); i!=end(); i++) {
      if((i->second.*val)() == v) {
	return true;
      }
    }
    return false;
  }

  template<typename S>
  iterator find(S (NodeType::*val)(void), S v) {
    for(iterator i=begin(); i!=end(); i++) {
      if((i->second.*val)() == v) {
	return i;
      }
    }
    return end();
  }

  template<typename S>
  const_iterator find(S (NodeType::*val)(void), S v) const {
    for(iterator i=begin(); i!=end(); i++) {
      if((i->second.*val)() == v) {
	return i;
      }
    }
    return end();
  }

  template<typename ScoreType>
  NodeType& leastScore(ScoreType (NodeType::*sc)(void) const = &NodeType::getScore) {
    if(!mace::KeyTraits<ScoreType>::isDeterministic()) {
      return random();
    }
    ScoreType bestScore = ScoreType();
    NodeType *best_entry = NULL;
    ASSERT(!empty());
    bestScore = (begin()->second.*sc)();
    best_entry = &(begin()->second);
    //       printf("In leastScore\n");
    //       fflush(NULL);
    for(iterator i=begin(); i!=end(); i++) {
      //         printf("Considering %.8x %f\n", i->first, (i->second.*sc)());
      //         fflush(NULL);
      if ((i->second.*sc)() < bestScore) {
	//           printf("picking better score!\n");
	//           fflush(NULL);
	bestScore = (i->second.*sc)();
	best_entry = &(i->second);
      }
    }
    //       printf("returning %x\n", best_entry);
    //       fflush(NULL);
    return *best_entry;		
  }

  template<typename ScoreType, typename Compare>
  NodeType& leastScore(const Compare& c, ScoreType (NodeType::*sc)(void) const = &NodeType::getScore) {
    if(!mace::KeyTraits<ScoreType>::isDeterministic()) {
      return random();
    }
    ScoreType bestScore = ScoreType();
    NodeType *best_entry = NULL;
    ASSERT(!empty());
    bestScore = (begin()->second.*sc)();
    best_entry = &(begin()->second);
    //       printf("In leastScore\n");
    //       fflush(NULL);
    for(iterator i=begin(); i!=end(); i++) {
      //         printf("Considering %.8x %f\n", i->first, (i->second.*sc)());
      //         fflush(NULL);
      if (c((i->second.*sc)(),bestScore)) {
	//           printf("picking better score!\n");
	//           fflush(NULL);
	bestScore = (i->second.*sc)();
	best_entry = &(i->second);
      }
    }
    //       printf("returning %x\n", best_entry);
    //       fflush(NULL);
    return *best_entry;		
  }

  template<typename ScoreType>
  const NodeType& leastScore(ScoreType (NodeType::*sc)(void) const = &NodeType::getScore) const {
    if(!mace::KeyTraits<ScoreType>::isDeterministic()) {
      return random();
    }
    ScoreType bestScore = ScoreType();
    const NodeType *best_entry = NULL;
    ASSERT(!empty());
    bestScore = (begin()->second.*sc)();
    best_entry = &(begin()->second);
    //       printf("In leastScore\n");
    //       fflush(NULL);
    for(const_iterator i=begin(); i!=end(); i++) {
      //         printf("Considering %.8x %f\n", i->first, (i->second.*sc)());
      //         fflush(NULL);
      if ((i->second.*sc)() < bestScore) {
	//           printf("picking better score!\n");
	//           fflush(NULL);
	bestScore = (i->second.*sc)();
	best_entry = &(i->second);
      }
    }
    //       printf("returning %x\n", best_entry);
    //       fflush(NULL);
    return *best_entry;		
  }

  template<typename ScoreType, typename Compare>
  const NodeType& leastScore(const Compare& c, ScoreType (NodeType::*sc)(void) const = &NodeType::getScore) const {
    if(!mace::KeyTraits<ScoreType>::isDeterministic()) {
      return random();
    }
    ScoreType bestScore = ScoreType();
    const NodeType *best_entry = NULL;
    ASSERT(!empty());
    bestScore = (begin()->second.*sc)();
    best_entry = &(begin()->second);
    //       printf("In leastScore\n");
    //       fflush(NULL);
    for(const_iterator i=begin(); i!=end(); i++) {
      //         printf("Considering %.8x %f\n", i->first, (i->second.*sc)());
      //         fflush(NULL);
      if (c((i->second.*sc)(), bestScore)) {
	//           printf("picking better score!\n");
	//           fflush(NULL);
	bestScore = (i->second.*sc)();
	best_entry = &(i->second);
      }
    }
    //       printf("returning %x\n", best_entry);
    //       fflush(NULL);
    return *best_entry;		
  }

  template<typename ScoreType>
  NodeType& greatestScore(ScoreType (NodeType::*sc)(void) const = &NodeType::getScore) {
    if(!mace::KeyTraits<ScoreType>::isDeterministic()) {
      return random();
    }
    ScoreType worstScore = ScoreType();
    NodeType *worst_entry = NULL;

    ASSERT(!empty());
    worstScore = (begin()->second.*sc)();
    worst_entry = &(begin()->second);

    for(iterator i=begin(); i!=end(); i++) {
      if ((i->second.*sc)() > worstScore)
      {
	worstScore = (i->second.*sc)();
	worst_entry = &(i->second);
      }
    }
    return *worst_entry;		
  }

  template<typename ScoreType, typename Compare>
  NodeType& greatestScore(const Compare& c, ScoreType (NodeType::*sc)(void) const = &NodeType::getScore) {
    if(!mace::KeyTraits<ScoreType>::isDeterministic()) {
      return random();
    }
    ScoreType worstScore = ScoreType();
    NodeType *worst_entry = NULL;

    ASSERT(!empty());
    worstScore = (begin()->second.*sc)();
    worst_entry = &(begin()->second);

    for(iterator i=begin(); i!=end(); i++) {
      if (c(worstScore, (i->second.*sc)()))
      {
	worstScore = (i->second.*sc)();
	worst_entry = &(i->second);
      }
    }
    return *worst_entry;		
  }

  template<typename ScoreType>
  const NodeType& greatestScore(ScoreType (NodeType::*sc)(void) const = &NodeType::getScore) const {
    if(!mace::KeyTraits<ScoreType>::isDeterministic()) {
      return random();
    }
    ScoreType worstScore = ScoreType();
    const NodeType *worst_entry = NULL;

    ASSERT(!empty());
    worstScore = (begin()->second.*sc)();
    worst_entry = &(begin()->second);

    for(const_iterator i=begin(); i!=end(); i++) {
      if ((i->second.*sc)() > worstScore)
      {
	worstScore = (i->second.*sc)();
	worst_entry = &(i->second);
      }
    }
    return *worst_entry;		
  }

  template<typename ScoreType, typename Compare>
  const NodeType& greatestScore(const Compare& c, ScoreType (NodeType::*sc)(void) const = &NodeType::getScore) const {
    if(!mace::KeyTraits<ScoreType>::isDeterministic()) {
      return random();
    }
    ScoreType worstScore = ScoreType();
    const NodeType *worst_entry = NULL;

    ASSERT(!empty());
    worstScore = (begin()->second.*sc)();
    worst_entry = &(begin()->second);

    for(const_iterator i=begin(); i!=end(); i++) {
      if (c(worstScore, (i->second.*sc)()))
      {
	worstScore = (i->second.*sc)();
	worst_entry = &(i->second);
      }
    }
    return *worst_entry;		
  }

  NodeType& random() {
    ASSERT(!empty());
    size_type i = RandomUtil::randInt(size());
    ASSERT(i < size());
    iterator j;
    for(j=begin(); j!=end() && i > 0; j++, i--);
    if(j != end()) {
      return j->second;
    } else {
      ASSERT(false);
    }
  }

  const NodeType& random() const {
    ASSERT(!empty());
    size_type i = RandomUtil::randInt(size());
    ASSERT(i < size());
    const_iterator j;
    for(j=begin(); j!=end() && i > 0; j++, i--);
    if(j != end()) {
      return j->second;
    } else {
      ASSERT(false);
    }
  }

  //XXX: This object may change on you if you let it.
  const NodeSet& nodeSet() const {
    return myNodeSet;
  }

  bool space() const {
    return size() < MaxSize;
  }

  bool full() const {
    return size() >= MaxSize;
  }

//   iterator begin() {
//     return begin();
//   }

//   const_iterator begin() const {
//     return begin();
//   }

//   iterator end() {
//     return end();
//   }

//   const_iterator end() const {
//     return end();
//   }

  map_iterator mapIterator(const MaceKey* b = NULL, const MaceKey* e = NULL) {
    return map_iterator(*this, (b==NULL?begin():mapType::find(*b)), (e==NULL?end():mapType::find(*e)));
  }
  const_map_iterator mapIterator(const MaceKey* b = NULL, const MaceKey* e = NULL) const {
    return const_map_iterator(*this, (b==NULL?begin():mapType::find(*b)), (e==NULL?end():mapType::find(*e)));
  }

  //     MapIterator<MaceKey, NodeType> iterator() {
  //       return MapIterator<MaceKey, NodeType>((map<MaceKey, NodeType>)*this);
  //     }

  void serialize(std::string& str) const {
    int s = size();
    mace::serialize(str, &s, s);
    for(const_iterator i=mapType::begin(); i!=mapType::end(); i++) {
      mace::serialize(str, &(i->second), (i->second));
    }
  }

  int deserialize(std::istream& in) throw(SerializationException) {
    clear();
    int position = 0;
    int ret = 0;

    size_t s;

    ret = mace::deserialize (in, &s, s);
    if (ret < 0) { return -1; }
    else { position += ret; }
    
    ASSERT(s <= MaxSize);

    MaceKey m;

    for(size_t i=0; i<s; i++) {
      //         NodeType t;
      //         ret = mace::deserialize (in, &t, t);
      int pos = in.tellg();
      ret = mace::deserialize (in, &m, m);
      if (ret < 0) { return -1; }
      in.seekg(pos, std::ios::beg);
      NodeType& t = add(m);
      ret = mace::deserialize (in, &t, t);
      if (ret < 0) { return -1; }
    }
    return position;
  }

  void serializeXML_RPC(std::string& str) const throw(SerializationException) {
    XmlRpcMapSerialize<const_iterator, MaceKey>(str, begin(), end());
  }

  int deserializeXML_RPC(std::istream& in) throw(SerializationException) {    
    return XmlRpcMapDeserialize<NodeCollection<NodeType, MaxSize>, MaceKey, NodeType>(in, *this);
  }

  const std::string& getTypeName() const {
    const char* types[] = { "NodeType", "unsigned int MaxSize", 0 };
    static const StrUtilNamespace::StdStringList myTypes = StrUtilNamespace::getTypeFromTemplate(__PRETTY_FUNCTION__, types);
    return myTypes[0];
  }
  
  void print(std::ostream& out) const {
    if(mace::PRINT_TYPE_NAMES) {
      out << "NodeCollection<" << this->getTypeName() << "," << MaxSize << "> [";
      out << " cursize=" << size() << "]";
    }
    printList(out, setBegin(), setEnd());
  }

  void printState(std::ostream& out) const {
    if(mace::PRINT_TYPE_NAMES) {
      out << "NodeCollection<" << this->getTypeName() << "," << MaxSize << "> [";
      out << " cursize=" << size() << "]";
    }
    printListState(out, setBegin(), setEnd());
  }

  class set_iterator {
  private: 
    iterator iter;
    friend class const_set_iterator;
  public:
    //         set_iterator(NodeCollection<NodeType, MaxSize>::iterator i) : iter(i) { }
    set_iterator(iterator i) : iter(i) { }
    set_iterator(const set_iterator& i) : iter(i.iter) { }
    NodeType& operator*() { return iter->second; }
    set_iterator& operator++() { iter++; return *this; }
    void operator++(int) { iter++; }
    bool operator==(const set_iterator& right) { return iter == right.iter; }
    bool operator!=(const set_iterator& right) { return iter != right.iter; }
  };

  class const_set_iterator {
  private: 
    const_iterator iter;

  public:
    const_set_iterator(const_iterator i) : iter(i) { }
    const_set_iterator(const const_set_iterator& i) : iter(i.iter) { }
    const_set_iterator(const set_iterator& i) : iter(i.iter) { }
    const NodeType& operator*() { return iter->second; }
    const_set_iterator& operator++() { iter++; return *this; }
    void operator++(int) { iter++; }
    bool operator==(const const_set_iterator& right) { return iter == right.iter; }
    bool operator!=(const const_set_iterator& right) { return iter != right.iter; }
  };

  set_iterator setBegin() {
    return set_iterator(begin());
  }
  const_set_iterator setBegin() const {
    return const_set_iterator(begin());
  }
  set_iterator setEnd() {
    return set_iterator(end());
  }
  const_set_iterator setEnd() const {
    return const_set_iterator(end());
  }
  set_iterator setFind(const MaceKey& item) {
    return set_iterator(this->find(item));
  }
  const_set_iterator setFind(const MaceKey& item) const {
    return set_iterator(this->find(item));
  }
};

} // namespace mace

#endif // _NEIGHBOR_SET_H
