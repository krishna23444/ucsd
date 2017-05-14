/* 
 * Serializable.h : part of the Mace toolkit for building distributed systems
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
#ifndef __SERIALIZABLE_H
#define __SERIALIZABLE_H

#include <netinet/in.h>
#include <string>
#include <sstream>
#include <iostream>
#include <typeinfo>
#include <netinet/in.h>
#include <ctype.h>
#include <boost/lexical_cast.hpp>
#include "Exception.h"
#include "massert.h"

using std::istream;
using std::istringstream;
using std::skipws;
// using std::cerr;
// using std::endl;

namespace mace {

class string;

class SerializationException : public Exception {
public:
  SerializationException(const std::string& m) : Exception(m) { }
  void rethrow() const { throw *this; }
};

class SerializationUtil {
public:
  static std::string getTag(istream& in, bool strict = true)
    throw(SerializationException) {
    std::string ret("<");
    char ch;

    expect(in, '<');
    in.get(ch);
    while(in && ch != '>' && (!strict || (isalnum(ch) || ch == '/' || ch == '_'))) {
//       cerr << "got " << ch << endl;
      ret.push_back(ch);
      in.get(ch);
    }
    if(!in) {
      throw SerializationException("EOF while deserializing tag: " + ret);
    }
    if (ch != '>') {
      std::string err = "Invalid character in tag " + ret + ": ";
      err.push_back(ch);
      throw SerializationException(err);
    }
    ret.push_back('>');

//     cerr << "returning " << ret << endl;
    return ret;
  }

  static void expectTag(istream& in, const char* tag) 
    throw(SerializationException) {

    while (in && isspace(in.peek())) {
      in.get();
    }

    expect(in, tag);
  }

  static std::string get(istream& in, char delim) throw(SerializationException) {
    std::string ret = "";
    char ch;

    while(in) {
      in.get(ch);
      if(ch != delim)
	ret += ch;
      else {
	in.unget();
	return ret;
      }
    }
    throw SerializationException("EOF while looking for delimeter " + delim);
  }

  static void expect(istream& in, const char* str) 
    throw(SerializationException) {
    unsigned len = strlen(str);
//     std::cerr << "expect: " << str << " " << len << " " << in.tellg() << std::endl;
//     char* buf = new char[len + 1];
    char* buf = new char[len];
    in.read(buf, len);
//     buf[len] = 0;
//     std::cerr << buf << " " << in.tellg() << std::endl;
    if(!in || strncmp(buf, str, len) != 0) {
      delete [] buf;
      std::string errstr = "error parsing string: ";
      if (!in) {
	errstr += "could not read " + boost::lexical_cast<std::string>(len) +
	  " bytes, expecting ";
	errstr.append(str);
      }
      else {
	errstr.append(str);
	errstr += " != ";
	errstr.append(buf, len);
      }
      throw SerializationException(errstr);
    }
    delete [] buf;
  }

  static void expect(istream& in, const std::string& str) 
    throw(SerializationException) {
    expect(in, str.c_str());
  }

  template<typename S>
  static void expect(istream& in, const S& val) throw(SerializationException) {
    S v;

    in >> v;
    if (!in || v != val) {
      std::string errstr = "error parsing token " +
	boost::lexical_cast<std::string>(val) + ": ";
      if (!in) {
	errstr += "EOF";
      }
      else {
	errstr += "parsed " + boost::lexical_cast<std::string>(v);
      }
	
      throw SerializationException(errstr);
    }
  }

  template<typename S>
  static void getToken(istream& in, S& val) throw(SerializationException) {
    in >> val;
    if (!in) {
      throw SerializationException("error parsing token " +
				   boost::lexical_cast<std::string>(val));
    }
  }

}; // SerializationUtil

class Serializable {
public:
  virtual int deserialize(std::istream& in) throw(SerializationException) = 0;
  virtual void serialize(std::string& str) const = 0;
  virtual std::string serializeStr() const {
    std::string s;
    serialize(s);
    return s;
  }
  virtual void deserializeStr(const std::string& s) throw(SerializationException) {
    std::istringstream is(s);
    deserialize(is);
  }

  virtual int deserializeXML_RPC(std::istream& in) throw(SerializationException);
  virtual void serializeXML_RPC(std::string& str) const throw(SerializationException);

  virtual ~Serializable() {}
};

template<typename S> 
void serialize(std::string& str, const void* pitem, const S& item) {
  //   printf("serialize void*\n");
  //   fflush(NULL);
  str.append((char*)&item, sizeof(S));
}

template<typename S> 
void serialize(std::string& str, const Serializable* pitem, const S& item) {
  //   printf("serialize Serializable*\n");
  //   fflush(NULL);
  item.serialize(str);
}

template<typename S> 
void serialize(std::string& str, const uint32_t* pitem, const S& item) {
  //   printf("serialize Serializable*\n");
  //   printf("serialize::htonl\n");
  //   fflush(NULL);

  uint32_t tmp = htonl(item);
  str.append((char*)&tmp, sizeof(S));
}

template<typename S> 
void serialize(std::string& str, const int32_t* pitem, const S& item) {
  //   printf("serialize Serializable*\n");
  //   printf("serialize::htonl\n");
  //   fflush(NULL);

  uint32_t tmp = htonl(item);
  str.append((char*)&tmp, sizeof(S));
}

template<typename S> 
void serialize(std::string& str, const uint16_t* pitem, const S& item) {
  //   printf("serialize Serializable*\n");
  //   printf("serialize::htonl\n");
  //   fflush(NULL);

  uint16_t tmp = htons(item);
  str.append((char*)&tmp, sizeof(S));
}

template<typename S> 
void serialize(std::string& str, const int16_t* pitem, const S& item) {
  //   printf("serialize Serializable*\n");
  //   printf("serialize::htonl\n");
  //   fflush(NULL);

  uint16_t tmp = htons(item);
  str.append((char*)&tmp, sizeof(S));
}

template<typename S> 
int deserialize(std::istream& in, void* pitem, S& item) 
  throw(SerializationException) {
  //   printf("deserialize void*\n");
  //   fflush(NULL);
  in.read((char *)pitem, sizeof(S));
  if (!in) {
    throw SerializationException("could not read " +
				 boost::lexical_cast<std::string>(sizeof(S)) + " bytes");
  }
  //item = *(S*)(str.data()+start);
  return sizeof(S);
}

template<typename S> 
int deserialize(std::istream& in, Serializable* pitem, S& item)
  throw(SerializationException) {
  //   printf("deserialize Serializable*\n");
  //   fflush(NULL);
  return item.deserialize(in);
}

template<typename S> 
int deserialize(std::istream& in, uint32_t* pitem, S& item)
  throw(SerializationException) {
  in.read((char *)pitem, sizeof(S));
  if (!in) {
    throw SerializationException("could not read " +
				 boost::lexical_cast<std::string>(sizeof(S)) + " bytes");
  }
  item = ntohl(item);
  return sizeof(S);
}

template<typename S> 
int deserialize(std::istream& in, int32_t* pitem, S& item) 
  throw(SerializationException) {
  in.read((char *)pitem, sizeof(S));
  if (!in) {
    throw SerializationException("could not read " +
				 boost::lexical_cast<std::string>(sizeof(S)) + " bytes");
  }
  item = ntohl(item);
  return sizeof(S);
}

template<typename S> 
int deserialize(std::istream& in, uint16_t* pitem, S& item) 
  throw(SerializationException) {
  in.read((char *)pitem, sizeof(S));
  if (!in) {
    throw SerializationException("could not read " +
				 boost::lexical_cast<std::string>(sizeof(S)) + " bytes");
  }
  item = ntohs(item);
  return sizeof(S);
}

template<typename S> 
int deserialize(std::istream& in, int16_t* pitem, S& item) 
  throw(SerializationException) {
  in.read((char *)pitem, sizeof(S));
  if (!in) {
    throw SerializationException("could not read " +
				 boost::lexical_cast<std::string>(sizeof(S)) + " bytes");
  }
  item = ntohs(item);
  return sizeof(S);
}

template<typename S>
int deserializeStr(const std::string& str, S* pitem, S& item)
{
  std::istringstream in(str);
  return mace::deserialize(in, pitem, item);
}

template<typename S> 
std::string serialize(const void* pitem, const S& item) {
  std::string str;
  serialize(str, pitem, item);
  return str;
}

template<typename S> 
std::string serialize(const Serializable* pitem, const S& item) {
  return item.serializeStr();
}

template<typename S> 
void deserialize(const std::string& s, void* pitem, S& item) 
  throw(SerializationException) {
  deserializeStr(s, pitem, item);
}

template<typename S> 
void deserialize(const std::string& s, Serializable* pitem, S& item)
  throw(SerializationException) {
  item.deserializeStr(s);
}



} // namespace mace
#endif
