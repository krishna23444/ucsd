/* 
 * msparse_array.h : part of the Mace toolkit for building distributed systems
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
#include <vector>
#include "Serializable.h"
#include "RandomUtil.h"
#include "StrUtil.h"

#ifndef _MACE_SPARSE_ARRAY_H
#define _MACE_SPARSE_ARRAY_H

namespace mace {

template<class T, int size>
class sparse_array : public Serializable, public PrintPrintable {

public:
  typedef int size_type;
  T* data[size];

  sparse_array() : data() { 
  }

  sparse_array(const sparse_array<T, size>& v) {
    for(int i=0; i < size; i++) {
      if(v.data[i] != NULL) {
        data[i] = new T(*v.data[i]);
      } else {
        data[i] = NULL;
      }
    }
  }

  sparse_array<T, size>& operator=(const sparse_array<T, size>& v) {
    if(this == &v) { return *this; }
    for(int i=0; i < size; i++) {
      delete data[i];
      if(v.data[i] != NULL) {
        data[i] = new T(*v.data[i]);
      } else {
        data[i] = NULL;
      }
    }
    return *this;
  }

  virtual ~sparse_array() { 
    for(int i=0; i < size; i++) {
      if(data[i] != NULL) { delete data[i]; data[i] = NULL; }
    }
  }

  int deserialize(std::istream& in) throw(SerializationException) {
    int bytes = 0;

    int count = 0;
    bytes += mace::deserialize(in, &count, count);
    
    ASSERT(count <= size);
    for(int i=0; i < size; i++) {
      if(data[i] != NULL) { delete data[i]; data[i] = NULL; }
    }

    for(size_type i=0; i<count; i++) {
      int place = 0;
      bytes += mace::deserialize(in, &place, place);
      bytes += mace::deserialize(in, &this->operator[](place), this->operator[](place));
    }
    
    return bytes;
  }

  void serialize(std::string &str) const {
    int count = 0;
    for(int i = 0; i < size; i++) { if(data[i] != NULL) { count++; } }
    mace::serialize(str, &count, count);

    for(int i = 0; i < size; i++) { 
      if(data[i] != NULL) {
        mace::serialize(str, &i, i);
        mace::serialize(str, data[i], *data[i]);
      }
    }
  }

  const std::string& getTypeName() const {
    const char* types[] = { "T", "int size", 0 };
    static const StrUtilNamespace::StdStringList myTypes = StrUtilNamespace::getTypeFromTemplate(__PRETTY_FUNCTION__, types);
    return myTypes[0];
  }

  void print(std::ostream& out) const {
    if(mace::PRINT_TYPE_NAMES) {
      out << "mace::sparse_array<" << getTypeName() << ">";
    }
    out << "[";
    for(int i = 0; i < size; i++) { 
      if(data[i] != NULL) {
        out << "(" << i << " => ";
        mace::printItem(out, data[i], *data[i]);
        out << ")";
      }
    }
    out << "]";
  }
  void printState(std::ostream& out) const {
    if(mace::PRINT_TYPE_NAMES) {
      out << "mace::sparse_array<" << getTypeName() << ">[";
    }
    for(int i = 0; i < size; i++) { 
      if(data[i] != NULL) {
        out << "(" << i << " => ";
        mace::printState(out, data[i], *data[i]);
        out << ")";
      }
    }
    out << "]";
  }

  bool contains(size_type i) const { return data[i] != NULL; }
  T& operator[] (size_type place) {
    check(place);
    return *data[place];
  }
  const T& operator[] (size_type place) const {
    if(!this->contains(place)) { throw(new Exception("place is empty in sparse array when indexing data!")); }
    return *data[place];
  }
  void erase(size_type i) {
    delete data[i];
    data[i] = NULL;
  }

  size_type count() const {
    size_type count = 0;
    for(size_type i = 0; i < size; i++) {
      if(this->contains(i)) {
        count++;
      }
    }
    return count;
  }

  T& random(int *p = NULL) {
    return (*this)[(p?*p=Util::randInt(this->count()):Util::randInt(this->count()))];
  }
  const T& random(int *p = NULL) const {
    return (*this)[(p?*p=Util::randInt(this->count()):Util::randInt(this->count()))];
  }
    

private:
  inline void check(int i) {
    if(data[i] == NULL) { data[i] = new T(); }
  }
    
};

}
#endif // _MACE_SPARSE_ARRAY_H
