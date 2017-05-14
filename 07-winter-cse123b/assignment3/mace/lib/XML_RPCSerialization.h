/* 
 * XML_RPCSerialization.h : part of the Mace toolkit for building distributed systems
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
#ifndef __XML_RPC__SERIALIZABLE_H
#define __XML_RPC__SERIALIZABLE_H

#include <netinet/in.h>
#include <string>
#include <sstream>
#include <iostream>
#include <typeinfo>
#include <netinet/in.h>
#include <ctype.h>
#include "Exception.h"
#include "massert.h"
#include "Serializable.h"
#include "mstring.h"

using std::istream;
using std::istringstream;
// using std::cerr;
// using std::endl;

namespace mace {

/* template<typename Key> */
/* bool isXML_RPCMapKeyType(const Key& k) { */
/*   const std::type_info& t = typeid(k); */
/*   return (t == typeid(int) || t == typeid(unsigned) || */
/* 	  t == typeid(long) || t == typeid(long long) || */
/* 	  t == typeid(short) || t == typeid(unsigned long) || */
/* 	  t == typeid(unsigned long long) || t == typeid(unsigned short) || */
/* 	  t == typeid(char) || t == typeid(unsigned char) ||  */
/* 	  t == typeid(bool) || t == typeid(std::string) || */
/* 	  t == typeid(mace::string)); */
/* } */

template<typename S>
void serializeXML_RPC(std::string& str, const void* pitem, const S& item)
  throw(SerializationException) {
//   throw SerializationException("Cannot serialize unsupported type");
  string s;
  serialize(s, pitem, item);
//   s.serialize(str);
  s.serializeXML_RPC(str);
}

template<typename S>
void serializeXML_RPC(std::string& str, const Serializable* pitem, 
		      const S& item) throw(SerializationException) {
  item.serializeXML_RPC(str);
}

template<typename S>
void serializeXML_RPC(std::string& str, const int* pitem, 
		      const S& item) {
  char buf[30];
  sprintf(buf, "%d", *pitem);

  str.append("<i4>");
  str.append(buf);
  str.append("</i4>");
}

template<typename S>
void serializeXML_RPC(std::string& str, const bool* pitem, 
		      const S& item) {
  str.append("<boolean>");
  if(*pitem)
    str.append("1");
  else
    str.append("0");
  str.append("</boolean>");
}

template<typename S>
void serializeXML_RPC(std::string& str, const double* pitem, 
		      const S& item) {
  char buf[30];
  snprintf(buf, 30, "%f", *pitem);
  str.append("<double>");
  str.append(buf);
  str.append("</double>");
}

template<typename S>
int deserializeXML_RPC(std::istream& in, void* pitem, S& item)
  throw(SerializationException) {
//   throw SerializationException("Cannot deserialize unsupported type");
  string s;
//   int r = s.deserialize(in);
  int r = s.deserializeXML_RPC(in);
  istringstream is(s);
  deserialize(is, pitem, item);
  return r;
}

template<typename S>
int deserializeXML_RPC(std::istream& in, Serializable* pitem, S& item) 
  throw(SerializationException) {
  return item.deserializeXML_RPC(in);
}

template<typename S>
int deserializeXML_RPC(std::istream& in, int* pitem, S& item) 
  throw(SerializationException) {
    std::istream::pos_type offset = in.tellg();
  std::string tag;

  tag = SerializationUtil::getTag(in);
  if(tag == "<i4>" || tag == "<int>") {
    SerializationUtil::getToken(in, *pitem);
    if(tag == "<i4>") {
      SerializationUtil::expectTag(in, "</i4>");
    }
    else {
      SerializationUtil::expectTag(in, "</int>");
    }
  }
  else
    throw SerializationException("error parsing integer tag: " + tag);
  return in.tellg() - offset;
}

template<typename S>
int deserializeXML_RPC(std::istream& in, bool* pitem, S& item) 
  throw(SerializationException) {
    std::istream::pos_type offset = in.tellg();
  char c;

  SerializationUtil::expectTag(in, "<boolean>");
  SerializationUtil::getToken(in, c);
  if(c == 1) {
    *pitem = true;
  }
  else if(c == 0) {
    *pitem = false;
  }
  else {
    throw SerializationException("Invalid value for boolean variable: " + c);
  }
  SerializationUtil::expectTag(in, "</boolean>");

  return in.tellg() - offset;
}

template<typename S>
int deserializeXML_RPC(std::istream& in, double* pitem, S& item)
  throw(SerializationException) {
    std::istream::pos_type offset = in.tellg();

  SerializationUtil::expectTag(in, "<double>");
  SerializationUtil::getToken(in, *pitem);
  SerializationUtil::expectTag(in, "</double>");

  return in.tellg() - offset;
}

template<typename S>
int deserializeXML_RPCParam(std::istream& in, S* pitem, S& item) 
  throw(SerializationException) {
    std::istream::pos_type offset = in.tellg();

  SerializationUtil::expectTag(in, "<param>");
  SerializationUtil::expectTag(in, "<value>");
  mace::deserializeXML_RPC(in, pitem, item);
  SerializationUtil::expectTag(in, "</value>");
  SerializationUtil::expectTag(in, "</param>");
  
  return in.tellg() - offset;
}
} // namespace mace
#endif
