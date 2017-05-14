/* 
 * TransportHeader.h : part of the Mace toolkit for building distributed systems
 * 
 * Copyright (c) 2006, James W. Anderson, Charles Killian
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
#ifndef TRANSPORT_HEADER_H
#define TRANSPORT_HEADER_H

#include <deque>

#include "pip_includer.h"
#include "SockUtil.h"
#include "Serializable.h"

class TransportHeader : public mace::Serializable {
public:
  typedef std::deque<TransportHeader> HeaderList;

public:
  TransportHeader(const MaceAddr& src = SockUtil::NULL_MACEADDR,
		  const MaceAddr& dest = SockUtil::NULL_MACEADDR,
		  registration_uid_t rid = 0, uint32_t size = 0, 
                  uint32_t pip_size = 0) :
    src(src), dest(dest), rid(rid), size(size), pip_size(pip_size)
    { }
  virtual ~TransportHeader() { }

  void serialize(std::string& s) const {
    serialize(s, src, dest, rid, size, pip_size);
  } // serialize

  static void serialize(std::string& s, const MaceAddr& src, const MaceAddr& dest,
			registration_uid_t rid, uint32_t size, uint32_t pip_size) {
    mace::serialize(s, &src, src);
    mace::serialize(s, &dest, dest);
    mace::serialize(s, &rid, rid);
    mace::serialize(s, &size, size);
    if(PIP)
    {
      mace::serialize(s, &pip_size, pip_size);
    }
  } // serialize

  int deserialize(std::istream& in) throw (mace::SerializationException) {
    int o = 0;
    o += mace::deserialize(in, &src, src);
    o += mace::deserialize(in, &dest, dest);
    o += mace::deserialize(in, &rid, rid);
    o += mace::deserialize(in, &size, size);
    if(PIP)
    {
      o += mace::deserialize(in, &pip_size, pip_size);
    } else {
      pip_size = 0;
    }
    return o;
  } // deserialize

  // return the serialized size in bytes
  static size_t ssize() {
    static size_t ths = 0;
    if (ths == 0) {
      TransportHeader t;
      std::string s;
      t.serialize(s);
      ths = s.size();
    }
    return ths;
  } // ssize

  static uint32_t deserializeSize(const std::string& s) { //total size past the transport header
    uint32_t ns = 0;
    memcpy(&ns, s.data() + ssize() - sizeof(ns), sizeof(ns));
    if(PIP)
    {
      uint32_t ns2;
      memcpy(&ns2, s.data() + ssize() - sizeof(ns) - sizeof(ns2), sizeof(ns2));
      return ntohl(ns) + ntohl(ns2);
    }
    return ntohl(ns);
  } // deserializeSize

  static uint32_t deserializePipSize(const std::string& s) {
    if(PIP) {
      uint32_t ns;
      memcpy(&ns, s.data() + ssize() - sizeof(ns), sizeof(ns));
      return ntohl(ns);
    } else {
      return 0;
    }
  }

  static void serialize(const MaceAddr& a, std::string& s) {
    a.serialize(s);
  } // serialize

  MaceAddr src;
  MaceAddr dest;
  registration_uid_t rid;
  uint32_t size;
  uint32_t pip_size;
}; // TransportHeader

class PipTransportHeader {
public:
  PipTransportHeader(const std::string& pathId = "", int mn = 0) :
    pathId(pathId), mnum(mn) { }
  virtual ~PipTransportHeader() { }

  void serialize(std::string& s) const {
    mace::serialize(s, &pathId, pathId);
    mace::serialize(s, &mnum, mnum);
  } // serialize

  int deserialize(std::istream& in) throw (mace::SerializationException) {
    int o = 0;
    o += mace::deserialize(in, &pathId, pathId);
    o += mace::deserialize(in, &mnum, mnum);
    return o;
  } // deserialize

  mace::string pathId;
  int mnum;
}; // PipTransportHeader

#endif // TRANSPORT_HEADER_H
