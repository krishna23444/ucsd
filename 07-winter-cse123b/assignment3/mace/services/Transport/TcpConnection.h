/* 
 * TcpConnection.h : part of the Mace toolkit for building distributed systems
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
#ifndef TCP_CONNECTION_H
#define TCP_CONNECTION_H

#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include <pthread.h>
#include <boost/shared_ptr.hpp>

#include "pip_includer.h"
#include "mvector.h"
#include "HashUtil.h"
#include "SockUtil.h"
#include "Collections.h"
#include "CircularQueue.h"
#include "CircularQueueList.h"
#include "mace-macros.h"

#include "BufferedTransportServiceClass.h"
#include "BaseTransport.h"
#include "NetworkErrorHandler.h"
#include "TransportHeader.h"
#include "TransportCrypto.h"

class TcpConnection {
public:
  typedef boost::shared_ptr<std::string> StringPtr;
  typedef CircularQueueList<StringPtr> StringPtrQueue;
  typedef CircularQueueList<std::string> StringQueue;

  struct StatusCallbackArgs {
    StatusCallbackArgs() : rid(0), ts(Util::timeu()) { }
    StatusCallbackArgs(const MaceKey& dest, registration_uid_t rid) :
      dest(dest), rid(rid), ts(Util::timeu()) { }

    MaceKey dest;
    registration_uid_t rid;
    uint64_t ts;
  };

  typedef CircularQueueList<StatusCallbackArgs> StatusCallbackArgsQueue;

public:
  TcpConnection(int s, TransportServiceClass::Connection::type t,
		TransportCrypto::type cryptoFlags, SSL_CTX* ctx,
		size_t queueSize = 0, size_t thresholdSize = 0,
		const SockAddr& id = SockUtil::NULL_MSOCKADDR, 
		uint16_t port = 0, uint64_t wsize = BaseTransport::DEFAULT_WINDOW_SIZE);
  virtual ~TcpConnection();
  TransportServiceClass::Connection::type directionType() const { return direction; }
  bool isReadable() const {
    return (open && (direction == TransportServiceClass::Connection::IN) && !suspended);
  }
  bool isWriteable() const {
    return (open && (direction == TransportServiceClass::Connection::OUT) &&
	    (wqueued > wsent));
  }
  bool isOpen() const { return open; }
  bool isConnecting() const { return connecting || (idsa == SockUtil::NULL_MSOCKADDR); }
  bool hasError() const { return (error != TransportError::NO_ERROR); }
  TransportError::type getError() const { return error; }
  const std::string& getErrorString() const { return errstr; }
  void read();
  void write();
  void close(TransportError::type err = TransportError::NO_ERROR,
	     const std::string& m = "", bool reset = false);
  void setQueueSize(size_t qs, size_t thresh) {
    qsize = qs;
    threshold = thresh;
  }
  void setWindowSize(uint64_t microsec) {
    bufStats->window = microsec;
  }
  size_t queueSize() const { return qsize; }
  size_t size() const { return wqueued - wsent + rqueued; }
  size_t rsize() const { return rqueued; }
  size_t wsize() const { return wqueued - wsent; }
  bool rqempty() const { return rq.empty(); }
  void enqueue(TransportHeader& h, const std::string& buf);
  void dequeue(StringPtr& h, StringPtr& buf);
  uint32_t localAddr() const { return laddr; }
  uint16_t localPort() const { return ntohs(lport); }
  uint32_t remoteAddr() const { return raddr; }
  uint16_t remotePort() const { return ntohs(rport); }
  const NodeSet& remoteKeys() const { return rkeys; }
  void addRemoteKey(const MaceKey& k) { rkeys.insert(k); }
  const SockAddr& id() const { return idsa; }
  int sockfd() const { return c; }
  const BufferStatisticsPtr stats() { return bufStats; }
  void addRTS(const MaceKey& dest, registration_uid_t rid) {
    rtsList.push(StatusCallbackArgs(dest, rid));
  }
  bool hasRTS() { return !rtsList.empty(); }
  StatusCallbackArgsQueue& getRTS() { return rtsList; }
  bool canSend() const { return sendable; }
  uint64_t lastActivityTime() const { return lastActivity; }
  uint64_t idleTime() const { return (Util::timeu() - lastActivity); }
  bool idle() const { return (rqempty() && !isWriteable() && (rqueued == 0)); }
  uint64_t timeOpened() const { return start; }
  void notifyMessageErrors(BaseTransport::NetworkHandlerMap& handlers);
  void suspend() {
    ASSERT(direction == TransportServiceClass::Connection::IN);
    suspended = true;
  }
  void resume() {
    ASSERT(direction == TransportServiceClass::Connection::IN);
    suspended = false;
  }
  bool isSuspended() const { return suspended; }

protected:
  void initSSL();
  bool connectSSL();
  ssize_t read(int s, void* buf, size_t count);
  ssize_t write(int s, const void* buf, size_t count);
  void connect();
  bool noVerifyHostname(const char* host);
  void lock() { assert(pthread_mutex_lock(&tlock) == 0); lockcount++; }
  void unlock() { assert(pthread_mutex_unlock(&tlock) == 0); }

  void wqRemove() {
    ADD_SELECTORS("TcpConnection::wqRemove");
    buffed.push(wq.front());
    // XXX !!!
//     sha1 hash;
//     HashUtil::computeSha1(wq.front(), hash);
//     maceout << "added " << Log::toHex(hash) << " to buffed"
// 	    << " wbuffed=" << wbuffed << " wsent=" << wsent << Log::endl;
    wq.pop();
  } // wqRemove

  void updateBuffed() {
    ADD_SELECTORS("TcpConnection::updateBuffed");
    size_t sz = (TransportHeader::ssize() +
		 TransportHeader::deserializeSize(buffed.front()));

    // XXX !!!
//     sha1 hash;
    while ((wbuffed + sz) <= wsent) {
      wbuffed += sz;
//       HashUtil::computeSha1(buffed.front(), hash);
//       maceout << "removed " << Log::toHex(hash) << " from buffed"
// 	      << " wbuffed=" << wbuffed << " wsent=" << wsent << Log::endl;
      buffed.pop();
      if(PIP)
      {
        buffed.pop();
      }
//       HashUtil::computeSha1(buffed.front(), hash);
//       maceout << "removed " << Log::toHex(hash) << " from buffed"
// 	      << " wbuffed=" << wbuffed << " wsent=" << wsent << Log::endl;
      buffed.pop();
      if (buffed.empty()) {
	break;
      }
      sz = TransportHeader::ssize() + TransportHeader::deserializeSize(buffed.front());
    }
  }

private:
  int c;
  TransportServiceClass::Connection::type direction;
  int cryptoFlags;
  SSL_CTX* ctx;
  SSL* ssl;
  BufferStatisticsPtr bufStats;
  SockAddr idsa;
  bool open;
  bool connecting;
  bool waitingForHeader;
  bool sendable;
  bool suspended;
  TransportError::type error;
  size_t qsize;
  size_t threshold;
  uint64_t byteCount;
  uint64_t wsent;
  uint64_t wqueued;
  uint64_t rqueued;
  uint64_t lastActivity;

  uint32_t laddr;
  uint16_t lport;
  uint32_t raddr;
  uint16_t rport;
  NodeSet rkeys;

  pthread_mutex_t tlock;
  uint64_t start;

  std::string errstr;

  StringPtrQueue rhq;
  StringPtrQueue rq;
  size_t rhdrsize;
  StringPtr rhdrstr;
  StringQueue wq;
  std::string rbuf;
  std::string wbuf;
  static const size_t QUEUE_SIZE;
  static const size_t BLOCK_SIZE;
  char* tmpBlock;
  size_t wchunki;
  bool isChunking;

  StringQueue buffed;
  bool buffedhdr;
  uint64_t wbuffed;

  uint64_t maxwqlen;
  uint64_t wqcount;
  uint64_t maxrqlen;
  uint64_t rqcount;
  size_t bufcopies;
  size_t chunkcopies;
  size_t lockcount;

  StatusCallbackArgsQueue rtsList;

  //The following are only used when Pip is being used
  bool waitingForPipHeader;
  uint32_t pipHeaderSize;
  StringPtr phdrstr;
  typedef CircularQueueList<StringPtr> PipHeaderList;
  PipHeaderList pq;
}; // TcpConnection

typedef boost::shared_ptr<TcpConnection> TcpConnectionPtr;

#endif // TCP_CONNECTION_H
