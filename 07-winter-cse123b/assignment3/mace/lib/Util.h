/* 
 * Util.h : part of the Mace toolkit for building distributed systems
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
#ifndef __UTIL_H
#define __UTIL_H

#include <pthread.h>
#include <sys/time.h>
#include <errno.h>
#include <string>

#include "MaceTypes.h"
#include "Exception.h"
#include "RandomUtil.h"

typedef int64_t m_suseconds_t;

class Util {
public:
  static const uint16_t DEFAULT_MACE_PORT = 5377;
  static const bool REPLAY;

public:
  static const MaceAddr& getMaceAddr() throw (AddressException);
  static int getAddr() throw (AddressException);
  // this will return the packed ip address of the hostname, which
  // could also be an ip address in dotted notation
  static int getAddr(const std::string& hostname) throw (AddressException);
  static uint16_t getPort();
  static int getHostByName(const std::string& hostname) throw (AddressException);
  static MaceAddr getMaceAddr(const std::string& hostname) throw (AddressException);
  static std::string getHostname();
  static bool isPrivateAddr(uint32_t a);
  static bool isHostReachable(const MaceKey& k);
  // returns the hostname of the key if the host can be directly
  // routed to.  throws an UnreachablePrivateAddressException if the
  // host can only be reached through the proxy address
  static std::string getHostname(const MaceKey& k) throw (AddressException);
  static std::string getHostname(const std::string& ipaddr) throw (AddressException);
  static std::string getHostname(int ipaddr);
  static std::string getHostByAddr(int ipaddr);
  static std::string getAddrString(int ipaddr);
  static std::string getAddrString(const std::string& hostname) throw (AddressException);
  static std::string getAddrString(const MaceAddr& addr, bool resolve = true);
  static std::string getAddrString(const SockAddr& addr, bool resolve = true);
  static void printAddrString(std::ostream& out, const MaceAddr& addr, bool resolve = true);
  static void printAddrString(std::ostream& out, const SockAddr& addr, bool resolve = true);
  static void printAddrString(std::ostream& out, const std::string& hostname) throw (AddressException);
  static void printAddrString(std::ostream& out, int ipaddr);
  static NodeSet getNodeSet(const std::string& hostlist);
  static m_suseconds_t timediff(const timeval& start, const timeval& end);
  // randInt will call srand automatically if needed
  static unsigned randInt() { return RandomUtil::randInt(); }
  static unsigned randInt(unsigned max) {
    return RandomUtil::randInt(max);
  }
  static unsigned randInt(unsigned max, unsigned first ...);
  static double timed();
  static time_t time();
  static uint64_t timeu();
  static uint64_t timeu(const struct timeval& tv);
  static void fillTimeval(uint64_t t, struct timeval& tv);
  static std::string getErrorString(int err);
  static std::string getSslErrorString();

private:
  static void lock();
  static void unlock();
  
private:
  static const size_t DNS_CACHE_TIMEOUT = 10 * 60; // 10 minutes;
  static const unsigned DNS_CACHE_SIZE = 64;
  static uint16_t baseport;
  static pthread_mutex_t ulock;
};

#endif // __UTIL_H
