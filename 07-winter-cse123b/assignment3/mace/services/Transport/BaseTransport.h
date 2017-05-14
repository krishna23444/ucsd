/* 
 * BaseTransport.h : part of the Mace toolkit for building distributed systems
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
#ifndef BASE_TRANSPORT_H
#define BASE_TRANSPORT_H

#include <boost/shared_ptr.hpp>

#include "SysUtil.h"
#include "PipedSignal.h"
#include "SockUtil.h"
#include "ScopedLock.h"
#include "CircularQueue.h"

#include "TransportHeader.h"
#include "BufferedTransportServiceClass.h"

#include "mace-macros.h"

class BaseTransport : public virtual TransportServiceClass {
public:
  class SocketException : public Exception {
  public:
    SocketException(const std::string& m) : Exception(m) { }
    void rethrow() const { throw *this; }
  }; // SocketException

  class BindException : public SocketException {
  public:
    BindException(const std::string& m) : SocketException(m) { }
    void rethrow() const { throw *this; }
  }; // BindException

public:
  typedef mace::hash_map<registration_uid_t, ReceiveDataHandler*> DataHandlerMap;
  typedef mace::hash_map<registration_uid_t, NetworkErrorHandler*> NetworkHandlerMap;
  typedef mace::hash_map<registration_uid_t, ConnectionStatusHandler*> ConnectionHandlerMap;
  typedef CircularQueue<registration_uid_t> RegIdQueue;

protected:
  typedef mace::hash_map<SockAddr, MaceAddr> SourceTranslationMap;
  enum transport_upcall_t { UPCALL_DELIVER, UPCALL_ERROR, UPCALL_CTS };
  static const int TRANSPORT_TRACE_DELIVER = 1;
  static const int TRANSPORT_TRACE_ERROR = 2;
  static const int TRANSPORT_TRACE_CTS = 3;
  static const int TRANSPORT_TRACE_FLUSH = 4;

public:
  BaseTransport(int portoff, MaceAddr addr = SockUtil::NULL_MACEADDR,
		int backlog = SOMAXCONN);
  virtual ~BaseTransport();

  virtual bool isRunning() {
    if (!running && doClose) {
      closeConnections();
    }
    return starting || running;
  } // isRunning

  virtual void addSockets(fd_set& rset, fd_set& wset, int& selectMax) {
    FD_SET(transportSocket, &rset);
    selectMax = std::max(transportSocket, selectMax);
  } // addSockets


  virtual const SockAddr& getNextHop(const MaceAddr& dest) const {
    if ((dest.proxy != SockUtil::NULL_MSOCKADDR) &&
	(dest.proxy != localAddr.local)) {
      proxying = true;
      // send it to the proxy
      return dest.proxy;
    }
    return dest.local;
  } // getNextHop

  virtual bool route(const MaceKey& dest, const std::string& s, bool rts = false,
		     registration_uid_t rid = -1) {
    ADD_SELECTORS("BaseTransport::route");
    macedbg(1) << "Routing Message of size " << s.size() << " from " << localKey << " to " << dest << Log::endl;
    DataHandlerMap::iterator i = dataHandlers.find(rid);
    if (i == dataHandlers.end()) {
      maceerr << "no handler registered with " << rid << Log::endl;
      return false;
    }
  
    ASSERT(running);

    bool r = true;

    r = sendData(localAddr, dest, dest.getMaceAddr(), rid, s, true, rts);
    traceout << r << Log::end;
    return r;
  } // route

  virtual void doIO(const fd_set& rset, const fd_set& wset, uint64_t selectTime) = 0;

  virtual void run() throw(SocketException);
  virtual void shutdown();
  virtual void maceInit();
  virtual void maceExit();

  virtual registration_uid_t registerHandler(ReceiveDataHandler& h,
					     registration_uid_t rid = -1);
  virtual registration_uid_t registerHandler(NetworkErrorHandler& h,
					     registration_uid_t rid = -1);
  virtual registration_uid_t registerHandler(ConnectionStatusHandler& h,
					     registration_uid_t rid = -1);
  virtual void unregisterHandler(ReceiveDataHandler& h, registration_uid_t rid = -1);
  virtual void unregisterHandler(NetworkErrorHandler& h, registration_uid_t rid = -1);
  virtual void unregisterHandler(ConnectionStatusHandler& h, registration_uid_t rid = -1);
  virtual void registerUniqueHandler(ReceiveDataHandler& h);
  virtual void registerUniqueHandler(NetworkErrorHandler& h);
  virtual void registerUniqueHandler(ConnectionStatusHandler& h);

public:
  static const uint64_t DEFAULT_WINDOW_SIZE = 5*1000*1000;

protected:
  static void* startDeliverThread(void* arg);
  virtual int getSockType() = 0;
  virtual void runDeliverThread() = 0;
  virtual void closeConnections();
  virtual bool deliverData(const std::string& shdr, const mace::string& s,
			   MaceKey* src = 0, NodeSet* suspended = 0);
  virtual bool sendData(const MaceAddr& src, const MaceKey& dest,
			const MaceAddr& nextHop, registration_uid_t rid,
			const std::string& s, bool checkQueueSize, bool rts) = 0;
  virtual void unregisterHandlers();

  void lock() const { ASSERT(pthread_mutex_lock(&tlock) == 0); }
  void unlock() const { ASSERT(pthread_mutex_unlock(&tlock) == 0); }
  void lockd() { ASSERT(pthread_mutex_lock(&dlock) == 0); }
  void unlockd() { ASSERT(pthread_mutex_unlock(&dlock) == 0); }
  void waitForDeliverSignal() { dsignal.wait(); }
  void signalDeliver() { dsignal.signal(); }

private:
  void setupSocket();

private:
  uint initCount;

protected:
  uint16_t port; // baseport
  uint32_t saddr;
  uint16_t portOffset;
  MaceAddr localAddr;
  MaceKey localKey;

  int backlog;
  int transportSocket;

  bool starting;
  bool running;
  bool shuttingDown;
  bool doClose;
  mutable bool proxying;

  mutable pthread_mutex_t tlock;

  PipedSignal dsignal;
  pthread_t deliverThread;
  pthread_mutex_t dlock;

  SourceTranslationMap translations;

  DataHandlerMap dataHandlers;
  RegIdQueue dataUnreg;
  NetworkHandlerMap errorHandlers;
  RegIdQueue errorUnreg;
  ConnectionHandlerMap connectionHandlers;
  RegIdQueue connectionUnreg;

  TransportHeader hdr;

}; // BaseTransport

typedef boost::shared_ptr<BaseTransport> BaseTransportPtr;

#endif // BASE_TRANSPORT_H
