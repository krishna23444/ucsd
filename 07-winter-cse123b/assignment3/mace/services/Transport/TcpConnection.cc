/* 
 * TcpConnection.cc : part of the Mace toolkit for building distributed systems
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
#include <errno.h>
#include <sys/socket.h>

#include <boost/format.hpp>

#include "mace-macros.h"
#include "Params.h"
#include "pip_includer.h"
#include "Accumulator.h"
#include "Log.h"
#include "Util.h"
#include "StrUtil.h"
#include "TransportScheduler.h"

#include "TcpConnection.h"

using std::min;
using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::ostream;
using std::istringstream;
using std::ostringstream;

const size_t TcpConnection::QUEUE_SIZE = 65536;
// const size_t TcpConnection::BLOCK_SIZE = 16384;
const size_t TcpConnection::BLOCK_SIZE = 32768;
// const size_t TcpConnection::BLOCK_SIZE = 65536;
// const size_t TcpConnection::BLOCK_SIZE = 131072;

TcpConnection::TcpConnection(int s, TransportServiceClass::Connection::type d,
			     TransportCrypto::type cryptoFlags, SSL_CTX* sctl, 
			     size_t queueSize, size_t thresholdSize,
			     const SockAddr& id, uint16_t port, uint64_t wsize) :
  c(s),
  direction(d),
  cryptoFlags(cryptoFlags),
  ctx(sctl),
  ssl(0),
  bufStats(new BufferStatistics(wsize)),
  idsa(id),
  open(true),
  connecting(true),
  waitingForHeader(true),
  sendable(true),
  suspended(false),
  error(TransportError::NO_ERROR),
  qsize(queueSize),
  threshold(thresholdSize),
  byteCount(0),
  wsent(0),
  wqueued(0),
  rqueued(0),
  lastActivity(Util::timeu()),
  raddr(INADDR_NONE),
  rport(0),
  rhq(QUEUE_SIZE),
  rq(QUEUE_SIZE),
  wq(QUEUE_SIZE),
  tmpBlock(0),
  wchunki(0),
  isChunking(false),
  wbuffed(0),
  maxwqlen(0),
  wqcount(0),
  maxrqlen(0),
  rqcount(0),
  bufcopies(0),
  chunkcopies(0),
  lockcount(0),
  waitingForPipHeader(true),
  pipHeaderSize(0),
  pq(QUEUE_SIZE) 
{

  struct sockaddr_in sa;
  memset(&sa, 0, sizeof(sa));
  socklen_t slen = sizeof(sa);
  if (getsockname(c, (sockaddr*)&sa, &slen) < 0) {
    Log::perror("getsockname");
    ASSERT(0);
  }

  laddr = sa.sin_addr.s_addr;
  lport = sa.sin_port;

  pthread_mutex_init(&tlock, 0);

  start = Util::timeu();

  if (cryptoFlags & TransportCrypto::TLS) {
    initSSL();
  }

  if (direction == TransportServiceClass::Connection::IN) {
    tmpBlock = new char[BLOCK_SIZE];
  }
  else {
    ASSERT(direction == TransportServiceClass::Connection::OUT);
  }

} // TcpConnection

TcpConnection::~TcpConnection() {
//   Log::log("TcpConnection::~TcpConnection") << "closing" << Log::endl;
  close();
  delete [] tmpBlock;
  tmpBlock = 0;
  SSL_free(ssl);
  ssl = 0;
} // ~TcpConnection

void TcpConnection::initSSL() {
//   ADD_SELECTORS("TcpConnection::initSSL");
  if (ssl) {
    return;
  }
  
  ssl = SSL_new(ctx);
  SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);
  SSL_set_fd(ssl, c);
//   maceout << "set " << c << " with " << ssl << Log::endl;
} // initSSL

bool TcpConnection::connectSSL() {
  ADD_SELECTORS("TcpConnection::connectSSL");
  static bool dieOnError = Params::Instance()->get<bool>("TRANSPORT_ASSERT_SSL");

  int r = 0;
  if (direction == TransportServiceClass::Connection::IN) {
//     maceout << "accepting ssl connection" << Log::endl;
    r = SSL_accept(ssl);
  }
  else {
    ASSERT(direction == TransportServiceClass::Connection::OUT);
//     maceout << "connecting ssl connection" << Log::endl;
    r = SSL_connect(ssl);
  }
  if (r <= 0) {
    switch (SSL_get_error(ssl, r)) {
    case SSL_ERROR_WANT_READ:
      break;
    case SSL_ERROR_WANT_WRITE:
      break;
    default:
      close(TransportError::TLS_ERROR, Util::getSslErrorString() + (errno ? " " +
	    Util::getErrorString(errno) : ""));
      // XXX
      ASSERT(!dieOnError);
// 	maceout << "unknown accept error" << Log::endl;
// 	ERR_print_errors_fp(stderr);
// 	ASSERT(0);
    }
//       Log::err() << "SSL_accept error" << Util::getSslErrorString() << Log::endl;
    return false;
  }

//   maceout << "ssl connection established" << Log::endl;

  string peername = Util::getHostname(raddr);

  // perform verification
  if (SSL_get_verify_result(ssl) != X509_V_OK) {
    string m = "certificate for " + peername + " does not verify";
    maceerr << m << Log::endl;
    close(TransportError::TLS_ERROR, m);
    ASSERT(!dieOnError);
  }


  // check the common name
  X509* peer = SSL_get_peer_certificate(ssl);
  if (peer == NULL) {
    string m = "no peer certificate for " + peername;
    maceerr << m << Log::endl;
    close(TransportError::TLS_ERROR, m);
    ASSERT(!dieOnError);
  }
  size_t len = 512;
  char peerCN[len];
  X509_NAME_get_text_by_NID(X509_get_subject_name(peer), NID_commonName, peerCN, len);
  if (strcasecmp(peerCN, peername.c_str()) && !noVerifyHostname(peerCN)) {
    ostringstream m;
    m << "common name " << peerCN << " does not match host name " << peername;
    maceerr << m.str() << Log::endl;
    close(TransportError::TLS_ERROR, m.str());
    ASSERT(!dieOnError);
  }

  return true;
} // connectSSL

void TcpConnection::connect() {
  ADD_SELECTORS("TcpConnection::connect");
  ASSERT(connecting);
  lastActivity = Util::timeu();

  if ((raddr == INADDR_NONE) && (rport == 0)) {
    struct sockaddr_in sa;
    memset(&sa, 0, sizeof(sa));
    socklen_t slen = sizeof(sa);
    if (getpeername(c, (sockaddr*)&sa, &slen) < 0) {
      if (errno == ENOTCONN) {
	// connect failed
	int err;
	socklen_t len = sizeof(err);
	if (getsockopt(c, SOL_SOCKET, SO_ERROR, &err, &len) < 0) {
	  Log::perror("getsockopt");
	  ASSERT(0);
	}
	close(TransportError::CONNECT_ERROR, Util::getErrorString(err));
	return;
      }
      Log::perror("getpeername");
      ASSERT(0);
    }
    {
      int err;
      socklen_t len = sizeof(err);
      ASSERT(getsockopt(c, SOL_SOCKET, SO_ERROR, &err, &len) == 0);
    }

    raddr = sa.sin_addr.s_addr;
    rport = sa.sin_port;

//     if (idsa.addr == INADDR_NONE) {
//       idsa.addr = raddr;
//       idsa.port = rport;
//     }
  }

  if (cryptoFlags & TransportCrypto::TLS) {
    if (!connectSSL()) {
      return;
    }
  }

  macedbg(1) << "Finished connecting to " << idsa << " socket " << c << Log::endl;

  connecting = false;
} // connect

ssize_t TcpConnection::read(int s, void* buf, size_t count) {
//   ADD_SELECTORS("TcpConnection::read");
  lastActivity = Util::timeu();
  int r = 0;
  if (cryptoFlags & TransportCrypto::TLS) {
    r = SSL_read(ssl, buf, count);
//     maceout << "SSL_read " << r << Log::endl;
    if (r <= 0) {
      switch (SSL_get_error(ssl, r)) {
      case SSL_ERROR_WANT_READ:
	break;
//       case SSL_ERROR_NONE:
//       case SSL_ERROR_ZERO_RETURN:
//       case SSL_ERROR_WANT_CONNECT:
//       case SSL_ERROR_WANT_ACCEPT:
//       case SSL_ERROR_SYSCALL:
//       case SSL_ERROR_SSL:
      default:
// 	maceout << "closing connection on SSL error" << Log::endl;
	close(TransportError::READ_ERROR, Util::getSslErrorString()  + " " +
	      Util::getErrorString(errno));
      }
    }
//     else {
//       maceout << Log::toHex(string((const char*)buf, r)) << Log::endl;
//     }
  }
  else {
    r = ::read(s, buf, count);
    int err = errno;
    if (r == 0) {
      close(TransportError::READ_ERROR, "socket closed");
    }
    else if ((r < 0) && (err != EAGAIN)) {
      close(TransportError::READ_ERROR, Util::getErrorString(err));
    }
  }
  return r;
} // read

ssize_t TcpConnection::write(int s, const void* buf, size_t count) {
//   ADD_SELECTORS("TcpConnection::write");
  lastActivity = Util::timeu();
  int r = 0;
  if (cryptoFlags & TransportCrypto::TLS) {
    r = SSL_write(ssl, buf, count);
//     maceout << "SSL_write " << r << Log::endl;
    if (r <= 0) {
      switch (SSL_get_error(ssl, r)) {
      case SSL_ERROR_WANT_WRITE:
	break;
//       case SSL_ERROR_NONE:
// 	ASSERT(0);
// 	break;
//       case SSL_ERROR_ZERO_RETURN:
// 	maceout << "closing connection on SSL_ERROR_ZERO_RETURN" << Log::endl;
// 	close(TransportError::WRITE_ERROR, Util::getSslErrorString());
// 	break;
//       case SSL_ERROR_WANT_CONNECT:
// 	Log::err() << "want connect error" << Log::endl;
// 	ASSERT(0);
//       case SSL_ERROR_WANT_ACCEPT:
// 	Log::err() << "want accept error" << Log::endl;
// 	ASSERT(0);
//       case SSL_ERROR_SYSCALL:
// 	Log::err() << "syscall error" << Log::endl;
// 	ASSERT(0);
//       case SSL_ERROR_SSL:
// 	Log::err() << "ssl error" << Log::endl;
// 	ASSERT(0);
//       default:
// 	ERR_print_errors_fp(stderr);
// 	Log::err() << "TcpConnection::write: SSL write error "
// 		   << Util::getSslErrorString() << Log::endl;
// 	ASSERT(0);
      default:
// 	maceout << "closing connection on SSL error" << Log::endl;
	close(TransportError::WRITE_ERROR, Util::getSslErrorString()  + " " +
	      Util::getErrorString(errno));
      }
    }
//     else {
//       maceout << Log::toHex(string((const char*)buf, r)) << Log::endl;
//     }
  }
  else {
    r = ::write(s, buf, count);
    if (r < 0) {
      close(TransportError::WRITE_ERROR, Util::getErrorString(errno));
    }
  }
  return r;
} // write

void TcpConnection::read() {
  ADD_SELECTORS("TcpConnection::read");
  static Accumulator* readaccum = Accumulator::Instance(Accumulator::TCP_READ);
  static Accumulator* netaccum = Accumulator::Instance(Accumulator::NETWORK_READ);
  ASSERT(open);

  if (connecting) {
    connect();
    if (connecting) {
      return;
    }
  }

  int r = read(c, tmpBlock, BLOCK_SIZE);

  if (r <= 0) {
    return;
  }

  readaccum->accumulate(r);
  netaccum->accumulate(r);
  bufStats->append(r);
  byteCount += r;
  rqueued += r;

  rbuf.append(tmpBlock, r);

  while (open) {
    if (waitingForHeader) {
      if (rbuf.size() < TransportHeader::ssize()) {
	return;
      }

//       cerr << "transportheader ssize=" << TransportHeader::ssize() << endl;
//       cerr << "rbuf.size=" << rbuf.size() << endl;

      rhdrstr = StringPtr(new std::string(rbuf.substr(0, TransportHeader::ssize())));
      rbuf = rbuf.substr(TransportHeader::ssize());
      rqueued -= TransportHeader::ssize();

//       cerr << Log::toHex(*rhdrstr) << endl;
      pipHeaderSize = TransportHeader::deserializePipSize(*rhdrstr); //returns 0 when !PIP
      //         maceout << "pipHeaderSize " << pipHeaderSize << " rhdrsize " << rhdrsize << Log::endl;
      rhdrsize = TransportHeader::deserializeSize(*rhdrstr) - pipHeaderSize;
//       cerr << "read rhdrsize=" << rhdrsize << endl;

      waitingForHeader = false;

      rbuf.reserve(rbuf.size() + rhdrsize);
    }

    if(PIP)
    {
      if(waitingForPipHeader) {
        if(rbuf.size() < pipHeaderSize) {
          return;
        }
        phdrstr = StringPtr(new std::string(rbuf.substr(0, pipHeaderSize)));
        rbuf = rbuf.substr(pipHeaderSize);
        rqueued -= pipHeaderSize;

        //           istringstream in(s);
        //           phdr.deserialize(in);
        waitingForPipHeader = false;
      }
    }
  
    if (rbuf.size() < rhdrsize) {
      return;
    }

    StringPtr buf = StringPtr(new std::string(rbuf.substr(0, rhdrsize)));
    rbuf = rbuf.substr(rhdrsize);
    rqueued -= rhdrsize;

    waitingForHeader = true;

    if(PIP)
    {
      pipHeaderSize = 0;
      waitingForPipHeader = true;
      pq.push(phdrstr);
    }

    rhq.push(rhdrstr);

    if (idsa.addr == INADDR_NONE) {
      TransportHeader thdr;
      istringstream in(*rhdrstr);
      thdr.deserialize(in);

      if (thdr.src.proxy != SockUtil::NULL_MSOCKADDR) {
	idsa = thdr.src.proxy;
      }
      else {
	idsa = thdr.src.local;
      }
    }
    
    rhdrstr = StringPtr();
    if(PIP)
    {
      phdrstr = StringPtr();
    } 
//     cerr << "adding message to rq" << endl;
    rq.push(buf);
//     cerr << "rq.size=" << rq.size() << " rhq.size=" << rhq.size() << endl;
    maxrqlen = std::max(maxrqlen, rq.size());
    rqcount++;
  }
} // read

void TcpConnection::write() {
  static Accumulator* writeaccum = Accumulator::Instance(Accumulator::TCP_WRITE);
  static Accumulator* netaccum = Accumulator::Instance(Accumulator::NETWORK_WRITE);
  ASSERT(open);
  ASSERT(wqueued > wsent);
  if (connecting) {
    connect();
    if (connecting || !open) {
      return;
    }
  }

  while ((wbuf.size() < BLOCK_SIZE) && !wq.empty()) {
    string& s = wq.front();
    size_t fsz = s.size();
    if (fsz > BLOCK_SIZE) {
      isChunking = true;
    }
    if (isChunking) {
      chunkcopies++;
      size_t sz = std::min(fsz - wchunki, BLOCK_SIZE);
      wbuf.append(s.data() + wchunki, sz);
      wchunki += sz;
      if (wchunki == fsz) {
	isChunking = false;
	wchunki = 0;
	wqRemove();
      }
    }
    else {
      bufcopies++;
      wbuf.append(s);
      wqRemove();
    }
  }

  int r = write(c, wbuf.data(), wbuf.size());
  if (r == 0) {
    Log::warn() << "TcpConnection::write wrote 0 bytes"
		<< " wbuf.size=" << wbuf.size()
		<< " wqueued=" << wqueued << " wsent=" << wsent
		<< " wq=" << wq.size() << Log::endl;
  }

  if (r <= 0) {
    return;
  }

  writeaccum->accumulate(r);
  netaccum->accumulate(r);
  bufStats->append(r);

  byteCount += r;
  wsent += r;

  updateBuffed();

  wbuf = wbuf.substr(r);

  if (!sendable && (wsize() < std::min(threshold, qsize))) {
    sendable = true;
  }
} // write

void TcpConnection::close(TransportError::type err, const std::string& m, bool reset) {
  ADD_SELECTORS("TcpConnection::close");
  ScopedLock sl(tlock);

  if (open) {
    open = false;

    error = err;
    errstr = m;

    if (reset) {
      maceout << "sending reset on " << c << Log::endl;
      struct linger ling;
      ling.l_onoff = 1;
      ling.l_linger = 0;
      if (setsockopt(c, SOL_SOCKET, SO_LINGER, &ling, sizeof(ling)) < 0) {
	Log::perror("setsockopt");
      }
    }

    if (cryptoFlags & TransportCrypto::TLS) {
      SSL_shutdown(ssl);
    }
    
    ::shutdown(c, SHUT_RDWR);
    ::close(c);
    uint64_t end = Util::timeu();
    uint64_t t = end - start;
    uint64_t idle = end - lastActivity;

    string unit = "B";
    double bw = byteCount * 1000 * 1000 / t;
    if (bw > 1024) {
      unit = "KB";
      bw /= 1024;
    }
    if (bw > 1024) {
      unit = "MB";
      bw /= 1024;
    }
    
    maceout << c << " open for " << (t / 1000) << " ms "
	    << "idle " << (idle / 1000) << " ms "
	    << byteCount << " bytes "
	    << boost::format("%.3f") % bw << " " << unit << "/sec" << Log::endl;
    maceout << "  wbuf=" << wbuf.size() << " rbuf=" << rbuf.size()
	    << " rq=" << rq.size() << " wq=" << wq.size() << " wsent=" << wsent
	    << " wqd=" << wqueued << " rqd=" << rqueued
	    << " wqc=" << wqcount << " rqc=" << rqcount
	    << " rts=" << rtsList.size() << Log::endl;
    maceout << "  maxwqlen=" << maxwqlen << " maxrqlen=" << maxrqlen
	    << " bc=" << bufcopies
	    << " cc=" << chunkcopies
	    << " conn=" << connecting
	    << " err=" << err << " " << m << Log::endl;
    maceout << "raddr=" << Util::getAddrString(raddr) << ":" << ntohs(rport)
	    << " id=" << idsa;
    for (NodeSet::const_iterator i = rkeys.begin(); i != rkeys.end(); i++) {
      maceout << " " << *i;
    }
    maceout << Log::endl;

  }
} // close

// void TcpConnection::enqueue(std::string& hdr, const std::string& buf, size_t max) {}
void TcpConnection::enqueue(TransportHeader& th, const std::string& buf) {
  ADD_SELECTORS("TcpConnection::enqueue");
//   ASSERT(open);

//   bool combineBuf = ((buf.size() < BLOCK_SIZE * 2) &&
// 		     !(cryptoFlags & TransportCrypto::ENCRYPTION) &&
// 		     !PIP);
  bool combineBuf = false;
  bool doSignal = (wsize() == 0);
  string phs;
  if(PIP) {
    static uint32_t pipSeq = 0;
    int size;
    const void* b;
    b = ANNOTATE_GET_PATH_ID(&size);
    ANNOTATE_SEND_STR(NULL, 0, buf.size(), "%.8x:%d-%d", laddr, lport, pipSeq);
    mace::string pathId((const char*)b, size);
    //     maceLog("size %d sendstr %.8x:%d-%d path %s\n", buf.size(), laddr, lport, pipSeq, pathId.toString().c_str());
    PipTransportHeader ph(pathId, pipSeq);
    pipSeq++;
    ph.serialize(phs);
  }
  th.pip_size = phs.size();
  std::string hdr;
  th.serialize(hdr);
  if (combineBuf) {
    hdr.append(phs); //NOTE: Empty string when not using Pip
    hdr.append(buf);
  }
  wq.push(hdr);
  wqcount++;
  //   cerr << Log::toHex(hdr) << endl;
  wqueued += hdr.size();

  if(PIP && !combineBuf) {
    wq.push(phs);
    wqueued += phs.size();
    wqcount++;
    //     maceout << "hdr.size " << hdr.size() << " phs.size " << phs.size() << " buf.size " << buf.size() << Log::endl;
  }

  if (!combineBuf) {
    wq.push(buf);
    wqueued += buf.size();
    wqcount ++;
  }
  maxwqlen = std::max(wq.size(), maxwqlen);
  if (doSignal) {
    TransportScheduler::signal();
  }

  if (sendable && (wsize() > qsize)) {
    sendable = false;
  }
} // enqueue

void TcpConnection::dequeue(StringPtr& hdr, StringPtr& buf) {
  ASSERT(!rq.empty() && !rhq.empty());

  buf = rq.front();
  rq.pop();
  hdr = rhq.front();
  rhq.pop();

  if(PIP)
  {
    ASSERT(!pq.empty());
    StringPtr& ps = pq.front();
    PipTransportHeader h;
    std::istringstream in(*ps);
    h.deserialize(in);
    ANNOTATE_SET_PATH_ID(NULL, 0, h.pathId.data(), h.pathId.size());
    ANNOTATE_RECEIVE_STR(NULL, 0, buf->size(), "%.8x:%d-%d",
			 raddr, rport, h.mnum);
    pq.pop();
  }
} // dequeue

bool TcpConnection::noVerifyHostname(const char* host) {
  static bool set = false;
  static bool check = false;
  static mace::deque<mace::string> allowed;
  static Params* params = 0;
  if (!set) {
    set = true;
    params = Params::Instance();
    if (params->containsKey(Params::MACE_NO_VERIFY_HOSTNAMES)) {
      std::string tmp = params->getStr(Params::MACE_NO_VERIFY_HOSTNAMES);
      allowed = StrUtil::split(" ", tmp);
      check = true;
    }
  }

  if (check) {
    for (size_t i = 0; i < allowed.size(); i++) {
      if (strcasecmp(host, allowed[i].c_str()) == 0) {
	return true;
      }
    }
  }

  return false;
} // noVerifyHostname

void TcpConnection::notifyMessageErrors(BaseTransport::NetworkHandlerMap& handlers) {
  ADD_SELECTORS("TcpConnection::notifyMessageErrors");

  while (!wq.empty()) {
    buffed.push(wq.front());
    wq.pop();
  }

  TransportHeader hdr;
  while (!buffed.empty()) {
    istringstream in(buffed.front());
    hdr.deserialize(in);

    buffed.pop();

    if(PIP)
    {
      std::string& phs = buffed.front();

      std::istringstream in(phs);
      PipTransportHeader ph;
      ph.deserialize(in);

      ANNOTATE_SET_PATH_ID(NULL, 0, ph.pathId.data(), ph.pathId.size());
      ANNOTATE_NOTICE(NULL, 0, "MessageError %s error %d", hdr.dest.toString().c_str(), error);
      //       ANNOTATE_RECEIVE_STR(NULL, 0, buf.size(), "%.8x:%d-%d", laddr, lport, pipSeq); FIXME

      buffed.pop();
    } 

    std::string& m = buffed.front();
    BaseTransport::NetworkHandlerMap::iterator i = handlers.find(hdr.rid);
    ASSERT (i != handlers.end());
    MaceKey dest(ipv4, hdr.dest);
//     sha1 hash;
//     HashUtil::computeSha1(in.str(), hash);
//     maceout << "upcalling " << hash << " " << dest << Log::endl;
//     HashUtil::computeSha1(m, hash);
//     maceout << "upcalling " << Log::toHex(hash) << " " << dest << Log::endl;
    traceout << true << dest << error << m << hdr.rid << Log::end;
    i->second->messageError(dest, error, m, hdr.rid);
    buffed.pop();
  }
  traceout << false << Log::end;
  
}
