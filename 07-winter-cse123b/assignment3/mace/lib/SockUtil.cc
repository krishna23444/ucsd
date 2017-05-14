/* 
 * SockUtil.cc : part of the Mace toolkit for building distributed systems
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
#include <boost/lexical_cast.hpp>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cassert>
#include <iostream>
#include <ext/hash_map>
#include <errno.h>
#include <fcntl.h>

#include "StrUtil.h"
#include "SockUtil.h"
#include "Util.h"
#include "Log.h"

// const msockaddr SockUtil::NULL_MSOCKADDR = msockaddr(INADDR_NONE, 0);
// const MaceAddr SockUtil::NULL_MACEADDR = MaceAddr(NULL_MSOCKADDR, NULL_MSOCKADDR);
const SockAddr SockUtil::NULL_MSOCKADDR = SockAddr();
const MaceAddr SockUtil::NULL_MACEADDR = MaceAddr();

using std::cerr;
using std::endl;

void SockUtil::setNonblock(int s) {
  int f;
  if (((f = fcntl(s, F_GETFL)) < 0) ||
      (fcntl(s, F_SETFL, f | O_NONBLOCK) < 0)) {
    Log::perror("fcntl");
    assert(false);
  }
} // setNonblock

void SockUtil::fillSockAddr(const std::string& host, uint16_t port,
			    struct sockaddr_in& sa) throw (AddressException) {
  fillSockAddr((u_int32_t)Util::getAddr(host), port, sa);
} // fillSockAddr

void SockUtil::fillSockAddr(uint32_t addr, uint16_t port, struct sockaddr_in& sa) {
  memset(&sa, 0, sizeof(sa));
  struct in_addr ina;
  ina.s_addr = addr;
  sa.sin_family = AF_INET;
  sa.sin_port = htons(port);
  sa.sin_addr = ina;
} // fillSockAddr

void SockUtil::fillSockAddr(const SockAddr& addr, struct sockaddr_in& sa) {
  fillSockAddr(addr.addr, addr.port, sa);
} // fillSockAddr

SockAddr SockUtil::getSockAddr(const std::string& hostname) throw (AddressException) {
  uint16_t port = Util::getPort();
  size_t i = hostname.find(':');
  std::string host = hostname;
  if (i != std::string::npos) {
    port = boost::lexical_cast<uint16_t>(hostname.substr(i + 1));
    host = hostname.substr(0, i);
  }

  uint32_t ip = Util::getAddr(host);
  return SockAddr(ip, port);
} // getSockAddr
