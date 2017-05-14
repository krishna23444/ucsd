/* 
 * UdpTransport.cc : part of the Mace toolkit for building distributed systems
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
#include "massert.h"
#include "Accumulator.h"

#include "TransportScheduler.h"
#include "UdpTransport.h"

using std::max;
using std::cout;
using std::cerr;
using std::endl;
using std::ostream;

const size_t UdpTransport::MAX_MESSAGE_SIZE = 65507;
const size_t UdpTransport::MAX_PAYLOAD_SIZE = MAX_MESSAGE_SIZE - TransportHeader::ssize();

namespace UdpTransport_namespace {

  TransportServiceClass& new_UdpTransport_Transport(int portOffset) {
    return *(new UdpTransportService(portOffset));
  }

} // UdpTransport_namespace

UdpTransport::UdpTransport(int portOffset) : BaseTransport(portOffset), rc(0), wc(0) {
  rbuf = new char[MAX_MESSAGE_SIZE];
} // UdpTransport

UdpTransport::~UdpTransport() {
  delete rbuf;
} // ~UdpTransport

UdpTransportPtr UdpTransport::create(int portOffset) {
  UdpTransportPtr p(new UdpTransport(portOffset));
  TransportScheduler::add(p);
  return p;
} // create

void UdpTransport::doIO(const fd_set& rset, const fd_set& wset, uint64_t st) {
  ADD_SELECTORS("UdpTransport::doIO");
  static Accumulator* readaccum = Accumulator::Instance(Accumulator::UDP_READ);
  static Accumulator* netaccum = Accumulator::Instance(Accumulator::NETWORK_READ);

  if (!running) {
    return;
  }

  if (FD_ISSET(transportSocket, &rset)) {
    sockaddr_in sa;
    while (true) {
      socklen_t salen = sizeof(sa);
      int r = recvfrom(transportSocket, rbuf, MAX_MESSAGE_SIZE, 0,
		       (struct sockaddr*)&sa, &salen);
      if (r <= 0) {
	if (errno == EAGAIN) {
	  return;
	}
	else {
	  Log::perror("recvfrom");
	  // XXX
	  ASSERT(0);
	}
      }

      //       macedbg(0) << "read " << r << Log::endl;

      ASSERT(r > (int)TransportHeader::ssize());

      readaccum->accumulate(r);
      netaccum->accumulate(r);
      rc += r;

      StringPtr hdr = StringPtr(new std::string(rbuf, TransportHeader::ssize()));
      size_t pipsize = 0;
      //       macedbg(1) << "receiving hdr(" << hdr->size() << ")=" << Log::toHex(*hdr) << Log::endl;

//       std::cerr << "read hdr(" << hdr->size() << ")=" << Log::toHex(*hdr) << std::endl;
      if (PIP) {
        pipsize = TransportHeader::deserializePipSize(*hdr);
        StringPtr pipHdr = StringPtr(new std::string(rbuf + TransportHeader::ssize(), pipsize));
        //         macedbg(1) << "receiving pipHdr(" << pipHdr->size() << ")=" << Log::toHex(*pipHdr) << Log::endl;
        pipq.push(pipHdr);
      }
      size_t rhdrsize = TransportHeader::deserializeSize(*hdr) - pipsize;
      ASSERT(rhdrsize < MAX_PAYLOAD_SIZE);
      ASSERT(rhdrsize == r - TransportHeader::ssize() - pipsize);

      StringPtr buf = StringPtr(new std::string(rbuf + TransportHeader::ssize() + pipsize,
						rhdrsize));
      //       macedbg(2) << "receiving message(" << buf->size() << ")=" << Log::toHex(*buf) << Log::endl;
//       std::cerr << "buf=" << Log::toHex(*buf) << std::endl;

      rq.push(buf);
      rhq.push(hdr);
      signalDeliver();
    }
  }
} // doIO

void UdpTransport::runDeliverThread() {
  TransportHeader th;
  while (!shuttingDown || !rhq.empty()) {
    unregisterHandlers();
    if (rhq.empty()) {
      waitForDeliverSignal();
      continue;
    }

    while (!rhq.empty()) {
      StringPtr shdr = rhq.front();
      rhq.pop();
      StringPtr sbuf = rq.front();
      rq.pop();
      if (PIP) {
        std::istringstream tin(*shdr);
        TransportHeader th;
        th.deserialize(tin);
        StringPtr& ps = pipq.front();
        PipTransportHeader h;
        std::istringstream in(*ps);
        h.deserialize(in);
        ANNOTATE_SET_PATH_ID(NULL, 0, h.pathId.data(), h.pathId.size());
        ANNOTATE_RECEIVE_STR(NULL, 0, sbuf->size(), "%s:%d-%d", th.src.toString().c_str(), portOffset, h.mnum);
        pipq.pop();
      }

      deliverData(*shdr, *sbuf, 0, &suspended);
    }
  }
  running = false;
  doClose = true;
} // runDeliverThread

bool UdpTransport::sendData(const MaceAddr& src, const MaceKey& dest,
			    const MaceAddr& nextHop, registration_uid_t rid,
			    const std::string& s, bool checkQueueSize, bool rts) {
  ADD_SELECTORS("UdpTransport::sendData");
  static Accumulator* sendaccum = Accumulator::Instance(Accumulator::TRANSPORT_SEND);
  static Accumulator* writeaccum = Accumulator::Instance(Accumulator::UDP_WRITE);
  static Accumulator* netaccum = Accumulator::Instance(Accumulator::NETWORK_WRITE);

  ASSERT(s.size() <= MAX_PAYLOAD_SIZE);

  const SockAddr& ma = getNextHop(nextHop);

  lock();
  SourceTranslationMap::const_iterator i = translations.find(ma);
  unlock();

  std::string phs;
  if(PIP) {
    static uint32_t pipSeq = 0;
    int size;
    const void* b;
    b = ANNOTATE_GET_PATH_ID(&size);
    ANNOTATE_SEND_STR(NULL, 0, s.size(), "%s:%d-%d", src.toString().c_str(), portOffset, pipSeq);
    mace::string pathId((const char*)b, size);
    //     maceLog("size %d sendstr %.8x:%d-%d path %s\n", buf.size(), laddr, lport, pipSeq, pathId.toString().c_str());
    PipTransportHeader ph(pathId, pipSeq);
    pipSeq++;
    ph.serialize(phs);
  }

  std::string m;
  TransportHeader::serialize(m, ((i == translations.end()) ? src : i->second),
			     dest.getMaceAddr(), rid, s.size(), phs.size());

  //   macedbg(0) << "sending src: " << src << " dest: " << dest << " ma=" << ma << Log::endl;
  //   macedbg(1) << "sending hdr(" << m.size() << ")=" << Log::toHex(m) << Log::endl;
//   std::cerr << "sending src=" << src << " dest=" << dest << " ma=" << ma << std::endl;
//   std::cerr << "sending hdr(" << m.size() << ")=" << Log::toHex(m) << std::endl;
  m.append(phs); //Empty string when not using Pip
  //   macedbg(1) << "sending pipHdr(" << phs.size() << ")=" << Log::toHex(phs) << Log::endl;
  m.append(s);
  //   macedbg(2) << "sending message(" << s.size() << ")=" << Log::toHex(s) << Log::endl;
  //   macedbg(3) << "sending total message(" << m.size() << ")=" << Log::toHex(m) << Log::endl;
//   std::cerr << "s=" << Log::toHex(s) << std::endl;

  bool success = true;

  sockaddr_in sa;
  SockUtil::fillSockAddr(ma.addr, ma.port + portOffset, sa);

  ASSERT(m.size() <= MAX_MESSAGE_SIZE);

  int r = sendto(transportSocket, m.data(), m.size(), 0,
		 (const struct sockaddr*)&sa, sizeof(sa));

  if (r < 0) {
    Log::perror("sendto");
    success = false;
    // XXX
    ASSERT(0);
  }

  writeaccum->accumulate(m.size());
  netaccum->accumulate(m.size());
  sendaccum->accumulate(s.size());
  wc += m.size();
  return success;
} // sendData
