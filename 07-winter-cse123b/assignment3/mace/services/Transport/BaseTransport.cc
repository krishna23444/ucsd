/* 
 * BaseTransport.cc : part of the Mace toolkit for building distributed systems
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
#include "Log.h"
#include "Accumulator.h"
#include "NumberGen.h"
#include "BaseTransport.h"
#include "ThreadCreate.h"

const int BaseTransport::TRANSPORT_TRACE_DELIVER;
const int BaseTransport::TRANSPORT_TRACE_ERROR;
const int BaseTransport::TRANSPORT_TRACE_CTS;
const int BaseTransport::TRANSPORT_TRACE_FLUSH;

BaseTransport::BaseTransport(int portoff, MaceAddr maddr, int bl) :
  initCount(0),
  backlog(bl),
  transportSocket(0),
  starting(true),
  running(false),
  shuttingDown(false),
  doClose(false),
  proxying(false) {

  ADD_SELECTORS("BaseTransport::BaseTransport");

  if (maddr == SockUtil::NULL_MACEADDR) {
    maddr = Util::getMaceAddr();
  }

  if (portoff == INT_MAX) {
    portOffset = NumberGen::Instance(NumberGen::PORT)->GetVal();
  }
  else {
    portOffset = portoff;
  }
  maceout << "port offset=" << portOffset << Log::endl;

  saddr = maddr.local.addr;
  port = maddr.local.port + portOffset;

  localAddr = maddr;
  localKey = MaceKey(ipv4, localAddr);

  maceout << "listening on " << Util::getAddrString(localAddr) << Log::endl;

  pthread_mutex_init(&tlock, 0);
  pthread_mutex_init(&dlock, 0);
} // BaseTransport

BaseTransport::~BaseTransport() {
  pthread_mutex_destroy(&tlock);
  pthread_mutex_destroy(&dlock);
} // ~BaseTransport

void* BaseTransport::startDeliverThread(void* arg) {
  BaseTransport* transport = (BaseTransport*)arg;
  transport->runDeliverThread();
  return 0;
} // startDeliverThread

void BaseTransport::run() throw(SocketException) {
  ScopedLock sl(tlock);
  if (running) {
    return;
  }

  // if the transport is not starting, then it needs to be re-added to
  // the scheduler...for now, do not allow transport to be re-started
  assert(starting);

  setupSocket();

  running = true;
  starting = false;
  shuttingDown = false;

  assert(transportSocket);

  if (getSockType() == SOCK_STREAM) {
    if (listen(transportSocket, backlog) < 0) {
      Log::perror("listen");
      assert(0);
    }
  }

  runNewThread(&deliverThread, BaseTransport::startDeliverThread, this, 0);
} // run

void BaseTransport::shutdown() {
  ADD_SELECTORS("BaseTransport::shutdown");
  ScopedLock sl(tlock);
  if (shuttingDown || !running) {
    return;
  }

  maceout << "transport shutting down" << Log::endl;
  shuttingDown = true;
  signalDeliver();
} // shutdown

void BaseTransport::maceInit() {
  initCount++;
  run();
} // maceInit

void BaseTransport::maceExit() {
  initCount--;
  if (initCount == 0) {
    shutdown();
  }
} // maceExit

void BaseTransport::setupSocket() {
  ADD_SELECTORS("BaseTransport::setupSocket");

  transportSocket = socket(AF_INET, getSockType(), 0);
  if (transportSocket < 0) {
    Log::perror("socket");
    assert(0);
  }

  int n = 1;
  if (setsockopt(transportSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&n, sizeof(n)) < 0) {
    Log::perror("setsockopt");
    assert(0);
  }

  struct sockaddr_in sa;
  uint32_t bindaddr = INADDR_ANY;
  if (Params::Instance()->containsKey(Params::MACE_BIND_LOCAL_ADDRESS)) {
    if (Params::Instance()->getInt(Params::MACE_BIND_LOCAL_ADDRESS)) {
      bindaddr = saddr;
    }
  }

  maceout << "binding to " << bindaddr << ":" << port << Log::endl;

  SockUtil::fillSockAddr(bindaddr, port, sa);
  if (bind(transportSocket, (struct sockaddr*)&sa, sizeof(sa)) < 0) {
    Log::perror("bind");
    ::close(transportSocket);
    std::ostringstream m;
    m << "BaseTransport::setupSocket: error binding to " << bindaddr << ":" << port
      << ": " << Util::getErrorString(errno);
    throw BindException(m.str());
  }

  SockUtil::setNonblock(transportSocket);
} // setupSocket

void BaseTransport::closeConnections() {
  ADD_SELECTORS("BaseTransport::closeConnections");
  ScopedLock sl(tlock);

  if (!doClose) {
    return;
  }

  doClose = false;

  if (transportSocket) {
    ::close(transportSocket);
    transportSocket = 0;
  }
  maceout << "halted transport" << Log::endl;
} // closeConnections

bool BaseTransport::deliverData(const std::string& shdr, const mace::string& s,
				MaceKey* srcp, NodeSet* suspended) {
  ADD_SELECTORS("BaseTransport::deliverData");

  static Accumulator* recvaccum = Accumulator::Instance(Accumulator::TRANSPORT_RECV);

  try {
    istringstream in(shdr);
    hdr.deserialize(in);
  }
  catch (const Exception& e) {
    Log::err() << "Transport deliver deserialization exception: " << e << Log::endl;
    return true;
  }

  MaceKey src(ipv4, hdr.src);

  if (suspended && suspended->contains(src)) {
    return false;
  }

  if (srcp) {
    *srcp = src;
  }
  
  if (hdr.dest.proxy == localAddr.local) {
    // this is the proxy node, forward the message
      
    sendData(hdr.src, MaceKey(ipv4, hdr.dest), hdr.dest, hdr.rid, s, false, false);
    return true;
  }

  DataHandlerMap::iterator i = dataHandlers.find(hdr.rid);
  if (i == dataHandlers.end()) {
    Log::err() << "BaseTransport::deliverData: no handler registered with "
	       << hdr.rid << Log::endl;
    return true;
  }

  if (hdr.dest != localAddr) {
    lock();
    translations[getNextHop(hdr.src)] = hdr.dest;
    unlock();
  }

  ReceiveDataHandler* h = i->second;
  MaceKey dest(ipv4, hdr.dest);

  recvaccum->accumulate(s.size());
  // XXX !!!
  //       sha1 hash;
  //       HashUtil::computeSha1(s, hash);
  //   maceout << "delivering " << Log::toHex(s) << " from " << src << Log::endl;
  macedbg(1) << "delivering message from " << src << " size " << s.size() << Log::endl;
//   maceout << "delivering " << s.size() << Log::endl;
  traceout << TRANSPORT_TRACE_ERROR << src << dest << s << hdr.rid << Log::end;
  h->deliver(src, dest, s, hdr.rid);

  return true;
} // deliverData

registration_uid_t BaseTransport::registerHandler(ReceiveDataHandler& h,
						  registration_uid_t rid) {
  if (rid == -1) {
    rid = NumberGen::Instance(NumberGen::HANDLER_UID)->GetVal();
  }
  dataHandlers[rid] = &h;
  return rid;
} // registerHandler

void BaseTransport::registerUniqueHandler(ReceiveDataHandler& h) {
  dataHandlers[-1] = &h;
} // registerUniqueHandler

void BaseTransport::unregisterHandler(ReceiveDataHandler& h, registration_uid_t rid) {
  dataUnreg.push(rid);
}


registration_uid_t BaseTransport::registerHandler(NetworkErrorHandler& h,
						  registration_uid_t rid) {
  if (rid == -1) {
    rid = NumberGen::Instance(NumberGen::HANDLER_UID)->GetVal();
  }
  errorHandlers[rid] = &h;
  return rid;
} // registerHandler

void BaseTransport::registerUniqueHandler(NetworkErrorHandler& h) {
  errorHandlers[-1] = &h;
} // registerUniqueHandler

void BaseTransport::unregisterHandler(NetworkErrorHandler& h, registration_uid_t rid) {
  errorUnreg.push(rid);
}

registration_uid_t BaseTransport::registerHandler(ConnectionStatusHandler& h,
						 registration_uid_t rid) {
  if (rid == -1) {
    rid = NumberGen::Instance(NumberGen::HANDLER_UID)->GetVal();
  }
  connectionHandlers[rid] = &h;
  return rid;
}

void BaseTransport::registerUniqueHandler(ConnectionStatusHandler& h) {
  connectionHandlers[-1] = &h;
}

void BaseTransport::unregisterHandler(ConnectionStatusHandler& h, registration_uid_t rid) {
  connectionUnreg.push(rid);
}

void BaseTransport::unregisterHandlers() {
  while (!dataUnreg.empty()) {
    dataHandlers.erase(dataUnreg.front());
    dataUnreg.pop();
  }
  while (!errorUnreg.empty()) {
    errorHandlers.erase(errorUnreg.front());
    errorUnreg.pop();
  }
  while (!connectionUnreg.empty()) {
    connectionHandlers.erase(connectionUnreg.front());
    connectionUnreg.pop();
  }
  
} // unregisterHandlers

