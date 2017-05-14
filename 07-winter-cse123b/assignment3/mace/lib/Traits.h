/* 
 * Traits.h : part of the Mace toolkit for building distributed systems
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
#include <string>
#include <istream>
#include <iostream>
#include "Serializable.h"
#include "mstring.h"

#ifndef _MACE_TRAITS_H
#define _MACE_TRAITS_H

namespace mace {

  class string;
  class MaceTime;

template<class T>
class KeyTraits {
  public:
  static bool isString() {
    return false;
  }
  static bool isInt() {
    return false;
  }
  static bool isDeterministic() {
    return true;
  }
  static void append(std::string& str, const T& key) {
  }
  static T extract(std::istream& in) throw(SerializationException) {
    throw SerializationException("Cannot extract unsupported type");
  }
  static void set(T& k, const std::string& s) {
    deserialize(s, &k, k);
  }
  static std::string toString(const T& key) {
    return serialize(&key, key);
  }
};

template<>
class KeyTraits<std::string> {
 public:
  static bool isString() {
    return true;
  }
  static bool isInt() {
    return false;
  }
  static bool isDeterministic() {
    return true;
  }
  static void append(std::string& str, const std::string& key) {
    str.append(key);
  }
  static std::string extract(std::istream& in) throw(SerializationException) {
    return SerializationUtil::get(in, '<');
  }
};

template<>
class KeyTraits<mace::string> : public KeyTraits<std::string> {
};

template<>
class KeyTraits<long long> {
public:
  static bool isString() {
    return false;
  }
  static bool isInt() {
    return true;
  }
  static bool isDeterministic() {
    return true;
  }
  static void append(std::string& str, const long long& key) {
    char buf[30];
    
    sprintf(buf, "%lld", key);
    str += buf;
  }
  static unsigned long long extract(std::istream& in) 
    throw(SerializationException) {
    std::string token = SerializationUtil::get(in, '<');
    return strtoull(token.c_str(), NULL, 10);
  }
};

template<>
class KeyTraits<unsigned long long> : public KeyTraits<long long> {
public:
  static void append(std::string& str, const unsigned long long& key) {
    char buf[30];
    
    sprintf(buf, "%llu", key);
    str += buf;
  }
};

template<>
class KeyTraits<int> : public KeyTraits<long long> {
};

template<>
class KeyTraits<unsigned> : public KeyTraits<unsigned long long> {
};

template<>
class KeyTraits<short> : public KeyTraits<long long> {
};

template<>
class KeyTraits<unsigned short> : public KeyTraits<unsigned long long> {
};

template<>
class KeyTraits<char> : public KeyTraits<long long> {
};

template<>
class KeyTraits<unsigned char> : public KeyTraits<unsigned long long> {
};


} // namespace mace
#endif // _TRAITS_H
