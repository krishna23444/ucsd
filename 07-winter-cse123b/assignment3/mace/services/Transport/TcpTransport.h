/* 
 * TcpTransport.h : part of the Mace toolkit for building distributed systems
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
#ifndef TCP_TRANSPORT_H
#define TCP_TRANSPORT_H

#include "Accumulator.h"
#include "mdeque.h"
#include "mhash_map.h"
#include "MaceTypes.h"
#include "Collections.h"
#include "SockUtil.h"
#include "CircularUniqueQueue.h"

#include "TcpConnection.h"
#include "BaseTransport.h"

/**
 * Note: the queueSize parameter is now in bytes.  If a message is
 * larger than the maxQueueSize, then the transport will
 * unconditionally accept it to prevent starvation.
 */

class TcpTransport;

typedef boost::shared_ptr<TcpTransport> TcpTransportPtr;

class TcpTransport : public virtual BaseTransport {
//   typedef mace::hash_map<int, TcpConnectionPtr> ConnectionMap;
  typedef mace::map<int, TcpConnectionPtr> ConnectionMap;

public:
  static TcpTransportPtr create(TransportCrypto::type cryptofl = TransportCrypto::NONE,
				bool upcallMessageError = false,
				size_t queueSize = UINT_MAX,
				size_t threshold = UINT_MAX,
				int portoffset = INT_MAX,
				MaceAddr addr = SockUtil::NULL_MACEADDR,
				int bl = SOMAXCONN);
  virtual ~TcpTransport();

  virtual void requestToSend(const MaceKey& dest, registration_uid_t rid = -1) {
    ScopedLock sl(tlock);
    TcpConnectionPtr c = connect(dest);
    c->addRTS(dest, rid);
    if (c->canSend()) {
      sendable.push(c);
      signalDeliver();
    }
  } // requestToSend

  virtual bool canSend(const MaceKey& dest) const {
    ADD_SELECTORS("TcpTransport::canSend");
    ScopedLock sl(tlock);
    bool r = canSend(getNextHop(dest.getMaceAddr()));
    traceout << r << Log::end;
    return r;
  } // canSend

  virtual size_t availableBufferSize(const MaceKey& dest) const {
    ADD_SELECTORS("TcpTransport::availableBufferSize");
    ScopedLock sl(tlock);
    size_t r = availableBufferSize(getNextHop(dest.getMaceAddr()));
    traceout << r << Log::end;
    return r;
  } // availableBufferSize

  virtual size_t bufferedDataSize() const {
    ADD_SELECTORS("TcpTransport::bufferedDataSize");
    ScopedLock sl(tlock);
    size_t r = 0;
    for (ConnectionMap::const_iterator i = connections.begin();
	 i != connections.end(); i++) {
      r += i->second->size();
    }
    traceout << r << Log::end;
    return r;
  } // bufferedDataSize

  virtual bool hasBufferedData() const {
    ADD_SELECTORS("TcpTransport::hasBufferedData");
    bool r = (bufferedDataSize() != 0);
    traceout << r << Log::end;
    return r;
  } // hasBufferedData

  virtual bool hasOutgoingBufferedData() const {
    ADD_SELECTORS("TcpTransport::hasOutgoingBufferedData");
    bool r = (outgoingBufferedDataSize() != 0);
    traceout << r << Log::end;
    return r;
  } // hasOutgoingBufferedData

  virtual size_t outgoingBufferedDataSize() const {
    ADD_SELECTORS("TcpTransport::outgoingBufferedDataSize");
    ScopedLock sl(tlock);
    size_t r = 0;
    for (ConnectionMap::const_iterator i = connections.begin();
	 i != connections.end(); i++) {
      r += i->second->wsize();
    }
    traceout << r << Log::end;
    return r;
  } // outgoingBufferedDataSize

  virtual size_t outgoingBufferedDataSize(const MaceKey& peer) const {
    ADD_SELECTORS("TcpTransport::outgoingBufferedDataSize");
    ScopedLock l(tlock);
    DestinationMap::const_iterator i = out.find(getNextHop(peer.getMaceAddr()));
    size_t r = 0;
    if (i != out.end()) {
      r = i->second->wsize();
    }
    traceout << r << Log::end;
    return r;
  } // outgoingBufferedDataSize

  virtual const BufferStatisticsPtr getStatistics(const MaceKey& peer,
						  TransportServiceClass::Connection::type d) const {
    ADD_SELECTORS("TcpTransport::getStatistics");
    ScopedLock l(tlock);
    if (d == TransportServiceClass::Connection::OUT) {
      DestinationMap::const_iterator i = out.find(getNextHop(peer.getMaceAddr()));
      if (i != out.end()) {
	traceout << true << *(i->second->stats()) << Log::end;
	return i->second->stats();
      }
    }
    else {
      assert(d == TransportServiceClass::Connection::IN);
      DestinationMap::const_iterator i = in.find(getNextHop(peer.getMaceAddr()));
      if (i != in.end()) {
	traceout << true << *(i->second->stats()) << Log::end;
	return i->second->stats();
      }
    }
    traceout << false << Log::end;
    return BufferStatisticsPtr();
  } // getStatistics

  virtual void setQueueSize(const MaceKey& peer, size_t size,
			    size_t threshold = UINT_MAX) {
    ScopedLock sl(tlock);
    TcpConnectionPtr c = connect(peer);
    if (threshold == UINT_MAX) {
      threshold = ((size / 2) ? (size / 2) : 1);
    }
    c->setQueueSize(size, threshold);
  } // setQueueSize

  virtual void setWindowSize(const MaceKey& peer, uint64_t microsec) {
    ScopedLock l(tlock);
    TcpConnectionPtr c = connect(peer);
    c->setWindowSize(microsec);
  } // setWindowSize

  virtual void requestFlushedNotification(registration_uid_t rid = -1) {
    flushedNotificationRequests.push(rid);
  } // requestFlushedNotification

  virtual void suspendDeliver(const MaceKey& peer, registration_uid_t rid) {
    ASSERT(dataHandlers.size() <= 1);
    ScopedLock l(tlock);
    DestinationMap::const_iterator i = in.find(getNextHop(peer.getMaceAddr()));
    if (i != in.end()) {
      i->second->suspend();
    }
  } // suspendDeliver

  virtual void resumeDeliver(const MaceKey& peer, registration_uid_t rid) {
    ASSERT(dataHandlers.size() <= 1);
    ScopedLock l(tlock);
    DestinationMap::const_iterator i = in.find(getNextHop(peer.getMaceAddr()));
    if (i != in.end()) {
      TcpConnectionPtr c = i->second;
      if (c->isSuspended()) {
	c->resume();
	for (ConnectionQueue::iterator q = suspended.begin(); q != suspended.end(); q++) {
	  if (*q == c) {
	    deliverable.push(c);
	    suspended.erase(q);
	    signalDeliver();
	    return;
	  }
	}
      }
    }
  } // resumeDeliver

  virtual void close(const MaceKey& dest);

  virtual void addSockets(fd_set& rset, fd_set& wset, int& selectMax) {
    //     ADD_SELECTORS("TcpTransport::addSockets");
    ScopedLock sl(tlock);
    // macedbg(1) << "addSockets() called" << Log::endl;
    if (!running) {
      return;
    }
    BaseTransport::addSockets(rset, wset, selectMax);
    uint64_t buffered = 0;
    for (ConnectionMap::const_iterator i = connections.begin();
	 i != connections.end(); i++) {
      int s = i->first;
      //       macedbg(1) << "Considering socket: " << s << " idsa: " << i->second->id();
      selectMax = std::max(s, selectMax);

      if (i->second->isReadable()) {
	FD_SET(s, &rset);
        //         macedbg(1) << " READABLE ";
      }

      if (i->second->isWriteable()) {
	FD_SET(s, &wset);
        //         macedbg(1) << " WRITEABLE ";
      }

      buffered += i->second->wsize();
      //       macedbg(1) << " bytes: " << i->second->wsize() << Log::endl;
    }

    if (buffered == 0) {
      while (!flushedNotificationRequests.empty()) {
	pendingFlushedNotifications.push(flushedNotificationRequests.front());
	flushedNotificationRequests.pop();
      }
      signalDeliver();
    }
  } // addSockets

  virtual void doIO(const fd_set& rset, const fd_set& wset, uint64_t selectTime);
  
protected:
  int getSockType() { return SOCK_STREAM; }
  virtual void runDeliverThread();
  virtual bool sendData(const MaceAddr& src, const MaceKey& dest,
			const MaceAddr& nextHop, registration_uid_t rid,
			const std::string& s, bool checkQueueSize, bool rts);
  virtual void notifyError(TcpConnectionPtr c);
  virtual void clearToSend(const MaceKey& dest, registration_uid_t rid);
  virtual void notifyFlushed(registration_uid_t rid);
  virtual bool canSend(const SockAddr& dest) const {
    DestinationMap::const_iterator i = out.find(dest);
    if (i != out.end()) {
      return i->second->canSend();
    }
    return true;
  } // canSend

  virtual size_t availableBufferSize(const SockAddr& dest) const {
    DestinationMap::const_iterator i = out.find(dest);
    if (i == out.end()) {
      return maxQueueSize;
    }
    else if (!i->second->canSend()) {
      return 0;
    }
    return i->second->queueSize() - i->second->wsize();
  } // availableBufferSize

private:
  TcpTransport(TransportCrypto::type cryptoFlags = TransportCrypto::NONE,
	       bool upcallMessageErrors = false,
	       size_t queueSize = UINT_MAX,
	       size_t threshold = UINT_MAX,
	       int portoffset = INT_MAX,
	       MaceAddr addr = SockUtil::NULL_MACEADDR, int bl = SOMAXCONN);
  void initSSL();
  void closeConnections();
  void accept();
  void flush();

  TcpConnectionPtr connect(const MaceKey& dest) {
    const SockAddr& ma = getNextHop(dest.getMaceAddr());
    return connect(dest, ma);
  }

  TcpConnectionPtr connect(const MaceKey& dest, const SockAddr& ma) {
    bool newConnection = false;
    TcpConnectionPtr c = connect(ma, newConnection);
    if (proxying || newConnection) {
      c->addRemoteKey(dest);
    }
    return c;
  }

  TcpConnectionPtr connect(const SockAddr& dest, bool& newConnection);

  void garbageCollectSockets();
  void removeClosedSockets();
  void closeBidirectionalConnection(TcpConnectionPtr c);

// private:
//   class ErrorNotification {
//   public:
//     ErrorNotification() { }
//     ErrorNotification(const NodeSet& ns, TransportError::type e, const std::string& m)
//       : nodes(ns), error(e), message(m) { }
//     NodeSet nodes;
//     TransportError::type error;
//     std::string message;
//   };

private:
  bool upcallMessageError;
  TransportCrypto::type cryptoFlags;
  size_t maxQueueSize;
  size_t queueThreshold;

  SSL_CTX* ctx;

  typedef CircularUniqueQueue<TcpConnectionPtr> ConnectionList;
  ConnectionList deliverable;
  ConnectionList sendable;
  ConnectionList errors;


  typedef mace::deque<TcpConnectionPtr> ConnectionQueue;
  ConnectionQueue duplicateIncomingConnections;
  ConnectionQueue suspended;

  typedef CircularUniqueQueue<registration_uid_t> RegIdList;
  RegIdList flushedNotificationRequests;
  RegIdList pendingFlushedNotifications;

  typedef mace::hash_map<SockAddr, TcpConnectionPtr> DestinationMap;
  DestinationMap out;
  DestinationMap in;

  ConnectionMap connections;

  typedef mace::hash_set<int> SocketSet;
  SocketSet closed;

}; // TcpTransport

namespace TcpTransport_namespace {

class TcpTransportService : public virtual BufferedTransportServiceClass {
public:
  TcpTransportService(TransportCrypto::type cryptoFlags = TransportCrypto::NONE,
		      bool upcallMessageError = false,
		      size_t queueSize = UINT_MAX,
		      size_t threshold = UINT_MAX,
		      int portoffset = INT_MAX,
		      MaceAddr addr = SockUtil::NULL_MACEADDR,
		      int bl = SOMAXCONN) {
    t = TcpTransport::create(cryptoFlags, upcallMessageError, queueSize, threshold, portoffset, addr, bl);
  }
  virtual ~TcpTransportService() { }
  bool route(const MaceKey& dest, const std::string& s, registration_uid_t rid = -1) {
    return t->route(dest, s, false, rid);
  }
  void requestToSend(const MaceKey& peer, registration_uid_t rid = -1) {
    t->requestToSend(peer, rid);
  }
  bool routeRTS(const MaceKey& dest, const std::string& s, registration_uid_t rid = -1) {
    return t->route(dest, s, true, rid);
  }
  bool canSend(const MaceKey& peer, registration_uid_t rid = -1) {
    return t->canSend(peer);
  }
  size_t availableBufferSize(const MaceKey& peer, registration_uid_t rid = -1) {
    return t->availableBufferSize(peer);
  }
  size_t outgoingBufferedDataSize(const MaceKey& peer, 
				  registration_uid_t rid = -1) {
    return t->outgoingBufferedDataSize(peer);
  }
  const BufferStatisticsPtr getStatistics(const MaceKey& peer, Connection::type d,
					  registration_uid_t rid = -1) {
    return t->getStatistics(peer, d);
  }
  size_t bufferedDataSize(registration_uid_t rid = -1) {
    return t->bufferedDataSize();
  }
  bool hasBufferedData(registration_uid_t rid = -1) {
    return t->hasBufferedData();
  }
  bool hasOutgoingBufferedData(registration_uid_t rid = -1) {
    return t->hasOutgoingBufferedData();
  }
  size_t outgoingBufferedDataSize(registration_uid_t rid = -1) {
    return t->outgoingBufferedDataSize();
  }
  void requestFlushedNotification(registration_uid_t rid = -1) {
    t->requestFlushedNotification(rid);
  }
  void suspendDeliver(MaceKey const & dest, registration_uid_t rid = -1) {
    t->suspendDeliver(dest, rid);
  }
  void resumeDeliver(MaceKey const & dest, registration_uid_t rid = -1) {
    t->resumeDeliver(dest, rid);
  }
  registration_uid_t registerHandler(ReceiveDataHandler& h, registration_uid_t rid = -1) {
    return t->registerHandler(h, rid);
  }
  registration_uid_t registerHandler(NetworkErrorHandler& h,
				     registration_uid_t rid = -1) {
    return t->registerHandler(h, rid);
  }
  registration_uid_t registerHandler(ConnectionStatusHandler& h,
				     registration_uid_t rid = -1) {
    return t->registerHandler(h, rid);
  }
  void unregisterHandler(ReceiveDataHandler& h, registration_uid_t rid = -1) {
    t->unregisterHandler(h, rid);
  }
  void unregisterHandler(NetworkErrorHandler& h, registration_uid_t rid = -1) {
    t->unregisterHandler(h, rid);
  }
  void unregisterHandler(ConnectionStatusHandler& h, registration_uid_t rid = -1) {
    t->unregisterHandler(h, rid);
  }
  void registerUniqueHandler(ReceiveDataHandler& h) { t->registerUniqueHandler(h); }
  void registerUniqueHandler(NetworkErrorHandler& h) { t->registerUniqueHandler(h); }
  void registerUniqueHandler(ConnectionStatusHandler& h) { t->registerUniqueHandler(h); }
  void maceInit() { t->maceInit(); }
  void maceExit() { t->maceExit(); }

private:
  TcpTransportPtr t;

}; // TcpTransportService

} // TcpTransport_namespace
  
#endif // TCP_TRANSPORT_H
