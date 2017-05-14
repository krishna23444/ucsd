/* 
 * TcpTransport.cc : part of the Mace toolkit for building distributed systems
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
#include <cassert>
#include <algorithm>
#include <sys/socket.h>
#include <stdexcept>

#include <arpa/inet.h>

#include "HashUtil.h"
#include "ServiceInitializer.h"
#include "SysUtil.h"
#include "NumberGen.h"
#include "Accumulator.h"
#include "TcpTransport.h"
#include "TransportScheduler.h"
#include "ThreadCreate.h"
#include "pip_includer.h"
#include "mace.h"

#undef MAX_LOG
#define MAX_LOG 5

using std::max;
using std::cout;
using std::cerr;
using std::endl;
using std::ostream;


namespace TcpTransport_namespace {

  TransportServiceClass& new_TcpTransport_Transport(TransportCrypto::type cryptoFlags,
						    bool merror,
						    size_t queueSize,
						    size_t threshold,
						    int portoffset,
						    MaceAddr addr,
						    int bl)
  {
    return *(new TcpTransportService(cryptoFlags, merror, queueSize, threshold, portoffset, addr, bl));
  }
  BufferedTransportServiceClass& new_TcpTransport_BufferedTransport(TransportCrypto::type cryptoFlags,
								    bool merror,
								    size_t queueSize,
								    size_t threshold,
								    int portoffset,
								    MaceAddr addr,
								    int bl)
  {
    return *(new TcpTransportService(cryptoFlags, merror, queueSize, threshold, portoffset, addr, bl));
  }

}

TcpTransportPtr TcpTransport::create(TransportCrypto::type cryptoFlags, bool merror,
				     size_t queueSize, size_t threshold, int portoffset,
				     MaceAddr maddr, int bl) {
  TcpTransportPtr p(new TcpTransport(cryptoFlags, merror, queueSize, threshold,
				     portoffset, maddr, bl));
  TransportScheduler::add(p);
  return p;
} // create

TcpTransport::TcpTransport(TransportCrypto::type cryptoFlags, bool merror,
			   size_t queueSize, size_t threshold, int portoffset,
			   MaceAddr maddr, int bl) :
  BaseTransport(portoffset, maddr, bl),
  upcallMessageError(merror),
  cryptoFlags(cryptoFlags),
  maxQueueSize(queueSize),
  queueThreshold((threshold == UINT_MAX) ?
		 ((maxQueueSize / 2) ? (maxQueueSize / 2) : 1) : threshold),
  ctx(0) {

  if (cryptoFlags & TransportCrypto::TLS) {
    initSSL();
  }

//   pthread_cond_init(&dsignal, 0);
} // TcpTransport

TcpTransport::~TcpTransport() {
  shutdown();
  SSL_CTX_free(ctx);
} // ~TcpTransport

void TcpTransport::initSSL() {
  ADD_SELECTORS("TcpTransport::initSSL");
  static bool loadSSL = true;
  if (loadSSL) {
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    ERR_load_BIO_strings();
    loadSSL = false;
  }

  SSL_METHOD* method = TLSv1_method();
  ctx = SSL_CTX_new(method);
  if (!ctx) {
    ERR_print_errors_fp(stderr);
    Log::err() << "TcpTransport::initSSL " << Util::getSslErrorString() << Log::endl;
    assert(0);
  }

  SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER | SSL_VERIFY_CLIENT_ONCE, 0);

  Params* params = Params::Instance();
  if (!params->containsKey(Params::MACE_CERT_FILE)) {
    Log::err() << "parameter MACE_CERT_FILE not set" << Log::endl;
    exit(-1);
  }

  std::string certFile = params->getStr(Params::MACE_CERT_FILE);
  if (SSL_CTX_use_certificate_file(ctx, certFile.c_str(), SSL_FILETYPE_PEM) <= 0) {
    Log::err() << "error loading certificate " << certFile << Log::endl;
    ERR_print_errors_fp(stderr);
    Log::err() << "TcpTransport::initSSL " << Util::getSslErrorString() << Log::endl;
    exit(-1);
  }

  if (!params->containsKey(Params::MACE_CERT_FILE)) {
    Log::err() << "parameter MACE_PRIVATE_KEY_FILE not set" << Log::endl;
    exit(-1);
  }

  std::string keyFile = params->getStr(Params::MACE_PRIVATE_KEY_FILE);
  if (SSL_CTX_use_PrivateKey_file(ctx, keyFile.c_str(), SSL_FILETYPE_PEM) <= 0) {
    Log::err() << "error loading private key " << keyFile << Log::endl;
    ERR_print_errors_fp(stderr);
    Log::err() << "TcpTransport::initSSL " << Util::getSslErrorString() << Log::endl;
    exit(-1);
  }

  if (!SSL_CTX_check_private_key(ctx)) {
    Log::err() << "private key " << keyFile
	       << " does not match the public certificate " << certFile << Log::endl;
    exit(-1);
  }

  if (!params->containsKey(Params::MACE_CA_FILE)) {
    Log::err() << "parameter MACE_CA_FILE not set" << Log::endl;
    exit(-1);
  }

  std::string caFile = params->getStr(Params::MACE_CA_FILE);
  STACK_OF(X509_NAME) *certNames = SSL_load_client_CA_file(caFile.c_str());
  if (certNames != NULL) {
    SSL_CTX_set_client_CA_list(ctx, certNames);
  }
  else {
    Log::err() << "could not load client CA file " << caFile << Log::endl;
    exit(-1);
  }

  if (!SSL_CTX_load_verify_locations(ctx, caFile.c_str(), 0)) {
    Log::err() << "could not load and verify CA file " << caFile << Log::endl;
    exit(-1);
  }

//   maceout << "loaded and verified keys" << Log::endl;

  assert(SSL_CTX_set_cipher_list(ctx, "AES128-SHA"));
} // initSSL

void TcpTransport::notifyError(TcpConnectionPtr c) {
  ADD_SELECTORS("TcpTransport::notifyError");
  const NodeSet& ns = c->remoteKeys();
  TransportError::type err = c->getError();
  const std::string& m = c->getErrorString();

  traceout << TRANSPORT_TRACE_ERROR << ns << err << m;
  for (NetworkHandlerMap::iterator i = errorHandlers.begin();
       i != errorHandlers.end(); i++) {
    for (NodeSet::const_iterator n = ns.begin(); n != ns.end(); n++) {
      i->second->error(*n, err, m, i->first);
    }
  }

  if (upcallMessageError && (c->directionType() == TransportServiceClass::Connection::OUT)) {
    traceout << true;
    c->notifyMessageErrors(errorHandlers);
  }
  else {
    traceout << false;
  }
  traceout << Log::end;
} // notifyError

void TcpTransport::clearToSend(const MaceKey& dest, registration_uid_t rid) {
  ADD_SELECTORS("TcpTransport::clearToSend");
  ConnectionHandlerMap::iterator i = connectionHandlers.find(rid);
  if (i == connectionHandlers.end()) {
    Log::err() << "TcpTransport::clearToSend: no handler registered with "
	       << rid << Log::endl;
    return;
  }

  ConnectionStatusHandler* h = i->second;
  traceout << TRANSPORT_TRACE_CTS << dest << rid << Log::end;
  h->clearToSend(dest, rid);
} // clearToSend

void TcpTransport::notifyFlushed(registration_uid_t rid) {
  ADD_SELECTORS("TcpTransport::notifyFlushed");
  ConnectionHandlerMap::iterator i = connectionHandlers.find(rid);
  if (i == connectionHandlers.end()) {
    Log::err() << "TcpTransport::notifyFlushed: no handler registered with "
	       << rid << Log::endl;
    return;
  }

  ConnectionStatusHandler* h = i->second;
  traceout << TRANSPORT_TRACE_FLUSH << rid << Log::endl;
  h->notifyFlushed(rid);
} // notifyFlushed

void TcpTransport::flush() {
  while (hasOutgoingBufferedData()) {
    SysUtil::sleepm(500);
  }
} // flush

void TcpTransport::closeConnections() {
  ScopedLock sl(tlock);
  if (!doClose) {
    return;
  }

  doClose = false;
  for (ConnectionMap::const_iterator i = connections.begin();
       i != connections.end(); i++) {
    TcpConnectionPtr c = i->second;
    c->close();
  }

  connections.clear();
  in.clear();
  out.clear();

  sl.unlock();
  BaseTransport::closeConnections();
} // closeConnections

void TcpTransport::runDeliverThread() {
  typedef std::vector<TcpConnectionPtr> ConnectionVector;
  MaceKey esrc;
  ConnectionVector resend;
  while (!shuttingDown || !deliverable.empty() || !errors.empty() ||
	 !pendingFlushedNotifications.empty()) {
    unregisterHandlers();
    for (ConnectionVector::const_iterator i = resend.begin(); i != resend.end(); i++) {
      sendable.push(*i);
    }
    resend.clear();
    
    if (deliverable.empty() && sendable.empty() && errors.empty() &&
	pendingFlushedNotifications.empty()) {
//       cerr << "waiting for deliver signal" << endl;
      waitForDeliverSignal();
//       cerr << "received deliver signal deliverable.size=" << deliverable.size()
// 	   << " sendable.size=" << sendable.size()
// 	   << " errors.size=" << errors.size()
// 	   << " shuttingDown=" << shuttingDown
// 	   << endl;
      continue;
    }

//     cerr << "deliver thread sendable.size=" << sendable.size() << endl;

    if (!deliverable.empty()) {
      TcpConnectionPtr c = deliverable.front();
      
      while (!c->rqempty() && !c->isSuspended()) {
	TcpConnection::StringPtr shdr;
	TcpConnection::StringPtr sbuf;
	c->dequeue(shdr, sbuf);

	if (c->remoteKeys().empty() || proxying) {
	  deliverData(*shdr, *sbuf, &esrc);
	  c->addRemoteKey(esrc);
	}
	else {
	  deliverData(*shdr, *sbuf);
	}
      }
      if (c->isSuspended() && !c->rqempty()) {
	suspended.push_back(c);
      }
      deliverable.pop();
    }
    if (!sendable.empty()) {
      TcpConnectionPtr c = sendable.front();
      uint64_t start = Util::timeu();

      TcpConnection::StatusCallbackArgsQueue& rts = c->getRTS();
      while (c->isOpen() && !rts.empty()) {
	const TcpConnection::StatusCallbackArgs& p = rts.front();
	if ((p.ts < start) && c->canSend()) {
	  clearToSend(p.dest, p.rid);
	  rts.pop();
	}
	else {
	  if ((p.ts >= start) && c->canSend()) {
	    resend.push_back(c);
	  }
	  break;
	}
      }

      sendable.pop();
    }
    if (!errors.empty()) {
      TcpConnectionPtr c = errors.front();
      notifyError(c);
//       const ErrorNotification& e = errors.front();
//       notifyError(e.nodes, e.error, e.message);

      errors.pop();
    }
    if (!pendingFlushedNotifications.empty()) {
      notifyFlushed(pendingFlushedNotifications.front());
      pendingFlushedNotifications.pop();
    }
  }
  flush();
  running = false;
  doClose = true;
//   Log::log("TcpTransport::runDeliverThread")
//     << "exiting connections=" << connections.size() << " in=" << in.size()
//     << " out=" << out.size() << Log::endl;
} // runDeliverThread

void TcpTransport::doIO(const fd_set& rset, const fd_set& wset, uint64_t selectTime) {
  ADD_SELECTORS("TcpTransport::doIO");
//   static uint count = 0;
  ScopedLock sl(tlock);
//   cerr << "connections.size=" << connections.size() << endl;
  if (!running) {
    return;
  }

  if (FD_ISSET(transportSocket, &rset)) {
    accept();
  }

  for (ConnectionMap::const_iterator i = connections.begin();
       i != connections.end(); i++) {
    int s = i->first;
    TcpConnectionPtr c = i->second;

    if (selectTime < c->timeOpened()) {
      continue;
    }

//     if (count % 1000 == 0) {
//       bool r = FD_ISSET(s, &rset);
//       bool w = FD_ISSET(s, &wset);
//       cerr << s << " isOpen=" << c->isOpen() << " r=" << r
// 	   << " w=" << w << endl;
//     }

    if (c->isOpen() && FD_ISSET(s, &rset)) {
      bool connecting = c->isConnecting();
      try {
	c->read();
      }
      catch (const std::length_error& e) {
	maceerr << e.what() << Log::endl;
	c->close();
      }
      catch (const std::bad_alloc& e) {
	maceerr << e.what() << Log::endl;
	c->close();
      }
      catch (const Exception& e) {
	maceerr << e << Log::endl;
	c->close();
      }
      if (connecting && c->isOpen() && !c->isConnecting()) {
	if (in.containsKey(c->id())) {
	  TcpConnectionPtr old = in[c->id()];
	  old->close(TransportError::READ_ERROR, "peer reset");
	  closeBidirectionalConnection(old);
// 	  maceout << "completed connection to " << c->id() << " " << c->sockfd()
// 		  << " in[" << c->id() << "]=" << in[c->id()]->sockfd() << Log::endl;
// 	  assert(0);
	}
// 	assert(!in.containsKey(c->id()));
	duplicateIncomingConnections.push_back(c);
// 	maceout << "connected in[" << c->id() << "]=" << c->sockfd() << Log::endl;
      }
    }

    if (c->isOpen() && FD_ISSET(s, &wset)) {
      c->write();
      if (c->isOpen() && c->hasRTS() && c->canSend()) {
	sendable.push(c);
	signalDeliver();
      }
    }

    if (!c->rqempty()) {
      deliverable.push(c);
      signalDeliver();
    }

    if (!c->isOpen()) {
// 	log << "connection " << s << " closed "
// 	    << Util::getAddrString(c->id()) << Log::endl;
      closeBidirectionalConnection(c);
    }
  }

  removeClosedSockets();

  for (ConnectionQueue::const_iterator i = duplicateIncomingConnections.begin();
       i != duplicateIncomingConnections.end(); i++) {
    in[(*i)->id()]= *i;
  }
  duplicateIncomingConnections.clear();

//   count++;
} // doIO

void TcpTransport::garbageCollectSockets() {
  uint64_t now = Util::timeu();

  typedef std::vector<uint64_t> TimeVector;
  TimeVector idles;

  for (ConnectionMap::const_iterator i = connections.begin();
       i != connections.end(); i++) {
    TcpConnectionPtr c = i->second;

    idles.push_back(now - c->lastActivityTime());
  }

  std::stable_sort(idles.begin(), idles.end());
  uint64_t median = idles[idles.size() / 2];

  for (ConnectionMap::const_iterator i = connections.begin();
       i != connections.end(); i++) {
//     int s = i->first;
    TcpConnectionPtr c = i->second;

    if (!c->isOpen()) {
      closeBidirectionalConnection(c);
    }
    else if (c->idle() && ((now - c->lastActivityTime()) >= median)) {
      c->close(TransportError::NO_ERROR, "connection idle", true);
      closeBidirectionalConnection(c);
    }
  }

  removeClosedSockets();
} // garbageCollectSockets

void TcpTransport::closeBidirectionalConnection(TcpConnectionPtr c) {
  ADD_SELECTORS("TcpTransport::closeBidirectionalConnection");
  closed.insert(c->sockfd());
//   maceout << "added " << c->sockfd() << " " << c->id() << " to closed" << Log::endl;
//   maceout << "in.contains=" << in.containsKey(c->id())
// 	  << " out.contains=" << out.containsKey(c->id()) << Log::endl;
  if (in.containsKey(c->id()) && out.containsKey(c->id())) {
// 	  log << "closing both directions of " << s << Log::endl;
// 	assert(connections.containsKey(out.get(c->id())));
    TcpConnectionPtr tmp = out.get(c->id());
    if (tmp == c) {
// 	  assert(connections.containsKey(in.get(c->id())));
      tmp = in.get(c->id());
      assert(tmp != c);
    }
// 	cerr << "transport closing connection with error " << c->getErrorString() << endl;
//     maceout << "closing " << tmp->sockfd() << " (" << c->sockfd() << ")"
// 	    << " with " << c->getError() << " " << c->getErrorString()
// 	    << Log::endl;
    tmp->close(c->getError(), c->getErrorString(), true);
    closed.insert(tmp->sockfd());
  }
}

void TcpTransport::removeClosedSockets() {
  for (SocketSet::const_iterator i = closed.begin(); i != closed.end(); i++) {
    int s = *i;
    TcpConnectionPtr c = connections[s];
    
    if (c->getError() != TransportError::NO_ERROR) {
      errors.push(c);
//       const NodeSet& es = c->remoteKeys();
//       errors.push(ErrorNotification(es, c->getError(), c->getErrorString()));
    }

    in.erase(c->id());
    out.erase(c->id());
    translations.erase(c->id());
    connections.erase(s);
  }

  if (!closed.empty()) {
    signalDeliver();
    closed.clear();
  }
} // removeClosedSockets

void TcpTransport::close(const MaceKey& destk) {
  ScopedLock sl(tlock);
  const SockAddr& dest = getNextHop(destk.getMaceAddr());
  DestinationMap::iterator i = out.find(dest);
  if (i != out.end()) {
    i->second->close();
  }
  i = in.find(dest);
  if (i != in.end()) {
    i->second->close();
  }
} // close

void TcpTransport::accept() {
  ADD_SELECTORS("TcpTransport::accept");

  do {
    sockaddr_in sin;
    socklen_t sinlen = sizeof(sin);
    memset(&sin, 0, sinlen); 

    int s = 0;
    if ((s = ::accept(transportSocket, (struct sockaddr*)&sin, &sinlen)) < 0) {
      int err = errno;
      if (err == EAGAIN || err == EWOULDBLOCK || err == ECONNABORTED ||
	  err == EPROTO || err == EINTR) {
	return;
      }
      else if (err == ENFILE || err == EMFILE || err == ENOBUFS || err == ENOMEM) {
	maceout << "error on socket(): " << Util::getErrorString(err) << Log::endl;
	garbageCollectSockets();
	s = ::accept(transportSocket, (struct sockaddr*)&sin, &sinlen);
	if (s < 0) {
	  if (errno == EAGAIN) {
	    return;
	  }
	  Log::perror("socket");
	  assert(0);
	}
      }
      else {
	Log::perror("accept");
	assert(0);
      }
    }

    char hostname[200];
    maceout << "Accepted connection from " << inet_ntop(AF_INET, &(sin.sin_addr), hostname, 200) << ":" << ntohs(sin.sin_port) << Log::endl;

    SockUtil::setNonblock(s);
    TcpConnectionPtr c(new TcpConnection(s, TransportServiceClass::Connection::IN, 
					 cryptoFlags, ctx));
    connections[s] = c;
  } while (1);
} // handleAccept

bool TcpTransport::sendData(const MaceAddr& src, const MaceKey& dest,
			    const MaceAddr& nextHop, registration_uid_t rid,
			    const std::string& s, bool checkQueueSize, bool rts) {
  ADD_SELECTORS("TcpTransport::sendData");
  static Accumulator* sendaccum = Accumulator::Instance(Accumulator::TRANSPORT_SEND);
  const SockAddr& ma = getNextHop(nextHop);

  lock();
  TcpConnectionPtr c = connect(dest, ma);

//   const MaceAddr* tsrc = &src;
  SourceTranslationMap::const_iterator i = translations.find(ma);
//   if (i != translations.end()) {
//     tsrc = &(i->second);
//   } 
  bool doEnqueue = !checkQueueSize || canSend(ma);
  if (!doEnqueue && rts) {
    c->addRTS(dest, rid);
  }
  unlock();

  if (doEnqueue) {
    //     std::string h;
    //     TransportHeader::serialize(h, ((i == translations.end()) ? src : i->second),
    // 			       dest.getMaceAddr(), rid, s.size());
    TransportHeader th(((i == translations.end()) ? src : i->second), dest.getMaceAddr(), rid, s.size());
    c->enqueue(th, s);
    
    // XXX !!!!
//     sha1 hash;
//     HashUtil::computeSha1(h, hash);
//     maceout << "enqueued " << Log::toHex(hash) << " to " << dest << Log::endl;
//     HashUtil::computeSha1(s, hash);
//     maceout << "enqueued " << Log::toHex(hash) << " to " << dest << Log::endl;

    sendaccum->accumulate(s.size());
  }

  return doEnqueue;
} // sendData

TcpConnectionPtr TcpTransport::connect(const SockAddr& dest, bool& newConnection) {
  ADD_SELECTORS("TcpTransport::connect");
  DestinationMap::const_iterator i = out.find(dest);
  if (i != out.end()) {
    TcpConnectionPtr c = i->second;
    if (c->isOpen()) {
      newConnection = false;
      return c;
    }
    else {
      removeClosedSockets();
    }
  }

  newConnection = true;
  int s = socket(AF_INET, SOCK_STREAM, 0);
  if (s < 0) {
    int err = errno;
    if (err == ENFILE || err == EMFILE || err == ENOBUFS || err == ENOMEM) {
      maceout << "error on socket(): " << Util::getErrorString(err) << Log::endl;
      garbageCollectSockets();
      do {
	s = socket(AF_INET, SOCK_STREAM, 0);
	if (s < 0) {
	  err = errno;
	  if (err != EAGAIN) {
	    Log::perror("socket");
	    assert(0);
	  }
	}
	SysUtil::sleepm(5);
      } while (s < 0);
    }
    else {
      Log::perror("socket");
      assert(0);
    }
  }

  SockUtil::setNonblock(s);

  struct sockaddr_in sa;
  SockUtil::fillSockAddr(dest.addr, dest.port + portOffset, sa);
  macedbg(1) << "connecting to " << Util::getHostname(dest.addr) << ":"
    << dest.port + portOffset << Log::endl;

  int r = 0;
  if ((r = ::connect(s, (struct sockaddr*)&sa, sizeof(sa))) < 0) {
    if (errno != EINPROGRESS) {
      Log::perror("connect");
      assert(0);
    }
  }

  TcpConnectionPtr c(new TcpConnection(s, TransportServiceClass::Connection::OUT,
				       cryptoFlags, ctx, maxQueueSize, queueThreshold,
				       dest, port));
  connections[s] = c;
  
//   maceout << "connected: out[" << dest << "]=" << s << Log::endl;
  out[dest] = c;

  TransportScheduler::signal();
  return c;
} // connect

TransportServiceClass* tcpTransportInitFunction() {
  return new TcpTransport_namespace::TcpTransportService();
} // tcpTransportInitFunction

int TcpTransport_load_protocol() {
  ServiceInitializer<TransportServiceClass*>::Instance()->
    registerService("TcpTransportService", tcpTransportInitFunction);
  return 0;
} // TcpTransport_load_protocol
