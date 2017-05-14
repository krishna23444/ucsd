/* 
 * Util.cc : part of the Mace toolkit for building distributed systems
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
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <boost/lexical_cast.hpp>

#include <math.h>
#include "Util.h"
#include "RandomUtil.h"
#include "hash_string.h"
#include "LRUCache.h"
#include "Params.h"
#include "StrUtil.h"
#include "SockUtil.h"

using std::cerr;
using std::endl;
using std::string;
using boost::lexical_cast;

const size_t Util::DNS_CACHE_TIMEOUT;
const unsigned Util::DNS_CACHE_SIZE;
const uint16_t Util::DEFAULT_MACE_PORT;
const bool Util::REPLAY = false;

static const int SUPPRESS_REVERSE_DNS = 0;
// static const int SUPPRESS_REVERSE_DNS = 1;

uint16_t Util::baseport = 0;
pthread_mutex_t Util::ulock = PTHREAD_MUTEX_INITIALIZER;

void Util::lock() {
  assert(pthread_mutex_lock(&ulock) == 0);
} // lock

void Util::unlock() {
  assert(pthread_mutex_unlock(&ulock) == 0);
} // unlock

std::string Util::getErrorString(int err) {
  char* errbuf = strerror(err);
  return std::string(errbuf);
} // getErrorString

std::string Util::getSslErrorString() {
  unsigned long e = ERR_get_error();
  if (e) {
    size_t len = 1024;
    char buf[len];
    ERR_error_string_n(e, buf, len);
    return string(buf);
  }
  else {
    return "";
  }
} // getSslErrorString

MaceAddr Util::getMaceAddr(const std::string& hostname) throw (AddressException) {
  size_t i = hostname.find('/');
  std::string pub = hostname;
  SockAddr proxyaddr;
  if (i != std::string::npos) {
    proxyaddr = SockUtil::getSockAddr(hostname.substr(i + 1));
    pub = hostname.substr(0, i);
  }
  return MaceAddr(SockUtil::getSockAddr(pub), proxyaddr);
} // getMaceAddr

uint16_t Util::getPort() {
  if (baseport == 0) {
    lock();
    if (Params::Instance()->containsKey(Params::MACE_LOCAL_ADDRESS)) {
      std::string hostname = Params::Instance()->getStr(Params::MACE_LOCAL_ADDRESS);
      if (hostname.find('/') != std::string::npos) {
	hostname = hostname.substr(0, hostname.find('/'));
      }
      size_t i = hostname.find(':');
      if (i != std::string::npos) {
	baseport = lexical_cast<int16_t>(hostname.substr(i + 1));
// 	if (!StrUtil::parseInt(hostname.substr(i + 1), iport)) {
// 	  throw AddressException("bad hostname:port: " + hostname);
// 	}
// 	baseport = (uint16_t)iport;
      }
    }
    if (baseport == 0) {
      baseport = Params::Instance()->get(Params::MACE_PORT, DEFAULT_MACE_PORT);
    }
    unlock();
  }
  return baseport;
} // getPort

const MaceAddr& Util::getMaceAddr() throw (AddressException) {
  static MaceAddr maddr = SockUtil::NULL_MACEADDR;

  if (maddr != SockUtil::NULL_MACEADDR) {
    return maddr;
  }

  if (Params::Instance()->containsKey(Params::MACE_LOCAL_ADDRESS)) {
    maddr = getMaceAddr(Params::Instance()->getStr(Params::MACE_LOCAL_ADDRESS));
    baseport = maddr.local.port;
  }
  else {
    maddr.local.addr = getAddr();
    maddr.local.port = getPort();
  }

  return maddr;
} // getMaceAddr

int Util::getAddr() throw (AddressException) {
  static int ipaddr = 0;
  if (ipaddr) {
    return ipaddr;
  }

  ipaddr = getAddr(getHostname());
  return ipaddr;
} // getAddr

// uint16_t Util::getPort() {
//   static uint16_t port = 0;
//   if (port) {
//     return port;
//   }
//   if (Params::Instance()->isset("MACE_PORT")) {
//     port = (uint16_t)Params::Instance()->getint("MACE_PORT");
//   }
//   else {
//     port = DEFAULT_MACE_PORT;
//   }
//   return port;
// } // getPort

int Util::getAddr(const std::string& hostname) throw (AddressException) {
  typedef mace::LRUCache<std::string, int, hash_string> DnsCache;
  static DnsCache dnsCache(DNS_CACHE_SIZE, DNS_CACHE_TIMEOUT);
  
  int r = 0;
  lock();
  if (dnsCache.containsKey(hostname)) {
    r = dnsCache.get(hostname);
  }
  else {
    hostent* he = gethostbyname(hostname.c_str());
    if (!he || he->h_length != sizeof(struct in_addr)) {
      if (hostname.find(':') != std::string::npos) {
	throw AddressException("Util::getAddr: bad hostname: " + hostname);
      }
      throw AddressException("gethostbyname " + hostname + ": " + getErrorString(h_errno));
    }

    r = ((in_addr*)(he->h_addr_list[0]))->s_addr;
    dnsCache.add(hostname, r);
  }
  unlock();
  return r;
} // getAddr

std::string Util::getAddrString(const MaceAddr& addr, bool resolve) {
  std::ostringstream out;
  printAddrString(out, addr, resolve);
  return out.str();
} // getAddrString

void Util::printAddrString(std::ostream& out, const MaceAddr& addr, bool resolve) {
  printAddrString(out, addr.local, resolve);
  if(addr.proxy != SockUtil::NULL_MSOCKADDR) {
    out << "/";
    printAddrString(out, addr.proxy, resolve);
  };
} // printAddrString

std::string Util::getAddrString(const SockAddr& addr, bool resolve) {
  std::ostringstream out;
  printAddrString(out, addr, resolve);
  return out.str();
} // getAddrString

void Util::printAddrString(std::ostream& r, const SockAddr& addr, bool resolve) {
  if (resolve) {
    try {
      r << getHostname(addr.addr);
    }
    catch (const AddressException& e) {
      printAddrString(r, addr.addr);
    }
  }
  else {
    printAddrString(r, addr.addr);
  }
  r << ":" << addr.port;
} // getAddrString

std::string Util::getAddrString(const std::string& hostname) throw (AddressException) {
  std::ostringstream out;
  printAddrString(out, hostname);
  return out.str();
} // getAddrString

void Util::printAddrString(std::ostream& out, const std::string& hostname) throw (AddressException) {
  printAddrString(out, getAddr(hostname));
} // printAddrString

std::string Util::getAddrString(int ipaddr) {
  struct sockaddr_in in;
  in.sin_addr.s_addr = ipaddr;
  std::string r(inet_ntoa(in.sin_addr));
  return r; 
} // getAddrString

void Util::printAddrString(std::ostream& out, int ipaddr) {
  struct sockaddr_in in;
  in.sin_addr.s_addr = ipaddr;
  out << inet_ntoa(in.sin_addr);
} // getAddrString

int Util::getHostByName(const std::string& hostname) throw (AddressException) {
  return getAddr(hostname);
} // getHostByName

std::string Util::getHostByAddr(int ipaddr) {
  return getHostname(ipaddr);
} // getHostByAddr

std::string Util::getHostname() {
  static std::string s;
  if (s.empty()) {
    lock();
    char hostname[512];
    assert(gethostname(hostname, sizeof(hostname)) == 0);
    s = hostname;
    unlock();
  }
  return s;
} // getHostname

bool Util::isPrivateAddr(uint32_t a) {
//      10.0.0.0        -   10.255.255.255  (10/8 prefix)
//      172.16.0.0      -   172.31.255.255  (172.16/12 prefix)
//      192.168.0.0     -   192.168.255.255 (192.168/16 prefix)
  static uint32_t s8 = ntohl(inet_addr("10.0.0.1"));
  static uint32_t s12b = ntohl(inet_addr("172.16.0.0"));
  static uint32_t s12e = ntohl(inet_addr("172.31.255.255"));
  static uint32_t s16 = ntohl(inet_addr("192.168.0.0"));

  a = ntohl(a);

  uint32_t t = a & s8;
  if (t == s8) {
    return true;
  }

  t = a & s16;
  if (t == s16) {
    return true;
  }

  t = a;
  if ((t >= s12b) && (t <= s12e)) {
    return true;
  }
  
  return false;
} // isPrivateAddr

bool Util::isHostReachable(const MaceKey& k) {
  static bool isReachableSet = false;
  static bool isReachable = false;

  if (!isReachableSet &&
      Params::Instance()->containsKey(Params::MACE_ALL_HOSTS_REACHABLE)) {
    isReachable = Params::Instance()->getInt(Params::MACE_ALL_HOSTS_REACHABLE);
    isReachableSet = true;
  }

  if (isReachableSet && isReachable) {
    return true;
  }
  
  const MaceAddr& ma = k.getMaceAddr();
  if ((ma.proxy != SockUtil::NULL_MSOCKADDR) && (ma.proxy != getMaceAddr().local)) {
    // a proxy is set but we are not the proxy, so we cannot directly reach this host
    return false;
  }
  return true;
} // isHostReachable

std::string Util::getHostname(const MaceKey& k) throw (AddressException) {
  const MaceAddr& ma = k.getMaceAddr();
  uint32_t h = ma.local.addr;
  if (isHostReachable(k)) {
    if ((ma.proxy != SockUtil::NULL_MSOCKADDR) && (ma.proxy != getMaceAddr().local)) {
      // a proxy is set and we are not the proxy, so go to the proxy
      h = ma.proxy.addr;
    }
  }
  else {
    throw UnreachablePrivateAddressException(k.addressString());
  }

  return getHostname(h);
} // getHostname

std::string Util::getHostname(const std::string& ipaddr) throw (AddressException) {
  in_addr_t a = inet_addr(ipaddr.c_str());
  if (a == INADDR_NONE) {
    throw AddressException("getHostname passed bad address: " + ipaddr);
  }

  return Util::getHostname(a);
} // getHostname

std::string Util::getHostname(int ipaddr) {
  typedef mace::LRUCache<int, std::string> ReverseDnsCache;
  static ReverseDnsCache dnsCache(DNS_CACHE_SIZE, DNS_CACHE_TIMEOUT);

  string r;
  lock();

  if (dnsCache.containsKey(ipaddr)) {
    r = dnsCache.get(ipaddr);
  }
  else {
    struct hostent* he = (SUPPRESS_REVERSE_DNS ? 0 : gethostbyaddr((char*)&ipaddr, sizeof(ipaddr), AF_INET));
    if (he == 0) {
      struct sockaddr_in in;
      in.sin_addr.s_addr = ipaddr;
      r.append(inet_ntoa(in.sin_addr)); 
      dnsCache.add(ipaddr, r);
    }
    else {
      r.append(he->h_name);
      dnsCache.add(ipaddr, r);
    }
  }

  unlock();
  return r;
} // getHostname

NodeSet Util::getNodeSet(const std::string& hostlist) {
  //FIXME
  //I'd like for this function to read in a node set from XML from the params file, and return the generated node set.
  return NodeSet();
} //getNodeSet

m_suseconds_t Util::timediff(const timeval& start, const timeval& end) {
  return ((int64_t)end.tv_usec - start.tv_usec) + (((int64_t)end.tv_sec - start.tv_sec) * 1000000);
} // timediff

double Util::timed() {
  return timeu() / 1000000.0;
} // timed

time_t Util::time() {
  return ::time(0);
}

uint64_t Util::timeu() {
  struct timeval tv;
  gettimeofday(&tv, 0);
  return timeu(tv);
} // timel

uint64_t Util::timeu(const struct timeval& tv) {
  return (((uint64_t)tv.tv_sec * 1000000) + tv.tv_usec);  
} // timel

void Util::fillTimeval(uint64_t t, struct timeval& tv) {
  tv.tv_sec = t / 1000000;
  tv.tv_usec = t % 1000000;
} // fillTimeval

unsigned Util::randInt(unsigned max, unsigned first ...) {
  va_list ap;
  va_start(ap, first);
  unsigned r = RandomUtil::vrandInt(max, first, ap);
  va_end(ap);
  return r;
}
