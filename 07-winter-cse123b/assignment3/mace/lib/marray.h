/* 
 * marray.h : part of the Mace toolkit for building distributed systems
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
#include "StrUtil.h"
#include "mace_constants.h"

#ifndef _MACE_ARRAY_H
#define _MACE_ARRAY_H

namespace mace {

template<class T, int size, class Alloc = std::allocator<T> >
class array : public std::vector<T, Alloc>, public Serializable, public PrintPrintable {

public:
  typedef typename std::vector<T, Alloc> baseType;
  typedef typename baseType::iterator iterator;
  typedef typename baseType::const_iterator const_iterator;
  typedef typename baseType::size_type size_type;

  array() : baseType(size) {
  }

  array(const T& t) : baseType(size, t) {
  }

  array(const array<T, size, Alloc>& v) : baseType(v) {
  }

  virtual ~array() { }

  int deserialize(std::istream& in) throw(SerializationException) {
    int bytes = 0;
    
    //     this->clear();
    for(size_type i=0; i<size; i++) {
      bytes += mace::deserialize(in, &this->operator[](i), this->operator[](i));
    }
    
    return bytes;
  }

  void serialize(std::string &str) const {
    ASSERT(size == baseType::size());

    for(const_iterator i=this->begin(); i!=this->end(); i++) {
      mace::serialize(str, &(*i), *i);
    }
  }

  const std::string& getTypeName() const {
    const char* types[] = { "T", "int size", "Alloc", 0 };
    static const StrUtilNamespace::StdStringList myTypes = StrUtilNamespace::getTypeFromTemplate(__PRETTY_FUNCTION__, types);
    return myTypes[0];
  }
  
  void print(std::ostream& out) const {
    if(mace::PRINT_TYPE_NAMES) {
      out << "array<"<< this->getTypeName() << "," << size << ">";
    }
    printList(out, this->begin(), this->end());
  }
  void printState(std::ostream& out) const {
    if(mace::PRINT_TYPE_NAMES) {
      out << "array<"<< this->getTypeName() << "," << size << ">";
    }
    printListState(out, this->begin(), this->end());
  }
};

}
#endif // _MACE_ARRAY_H
