/* 
 * MaceTransport.m : part of the Mace toolkit for building distributed systems
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
#include <boost/shared_ptr.hpp>
#include "NodeCollection.h"
#include "UdpTransport.h"
#include "MaceTime.h"

using mace::string;
using mace::NodeCollection;
// using mace::MaceTimeEquality;
// using mace::MaceTimeComparitor;
// using mace::MaceTimeHash;
using mace::MonotoneTime;
using std::endl;
using mace::getmtime;

// namespace MaceTransport_namespace {
//   static const size_t MAX_DATA_MESSAGE_SIZE = UdpTransport::MAX_MESSAGE_SIZE - 14;
//   static const size_t MAX_DATA_FRAGMENT_MESSAGE_SIZE = UdpTransport::MAX_MESSAGE_SIZE - 18;
//   static const size_t MAX_FRAGMENT_MESSAGE_SIZE = UdpTransport::MAX_MESSAGE_SIZE - 14;
// }

service MaceTransport;

provides BufferedTransport;

trace = med;

time = MaceTime;

services {
  Transport router = UdpTransport();
}

constants {

  // these values are set to be aggressive
  // more conservative values would be INIT_RTO = 3sec, MAX_RTO = 5sec
  int64_t INIT_RTO = 1*1000*1000; // us
  int64_t MAX_RTO = 3*1000*1000; // us
  int64_t MIN_RTO = 15000; // us
  int64_t BW_STATS_PERIOD = 5*1000*1000; // us
  double MAX_WND = 1000.0*1000.0; // packets
  uint16_t FAST_RETRANSMIT_DUP = 3;
  uint16_t FAST_RESET_DUP = 3;
  size_t UDP_MAX_MESSAGE_SIZE = 1468;
  size_t MAX_DATA_MESSAGE_SIZE = UDP_MAX_MESSAGE_SIZE - 30;
  size_t MAX_DATA_FRAGMENT_MESSAGE_SIZE = UDP_MAX_MESSAGE_SIZE - 34;
  size_t MAX_FRAGMENT_MESSAGE_SIZE = UDP_MAX_MESSAGE_SIZE - 14;
  uint64_t DEFAULT_WINDOW_SIZE = 5*1000*1000;
}

constructor_parameters {
  bool UPCALL_MESSAGE_ERROR = false;
  size_t maxQueueSize = UINT_MAX;
  size_t queueThresholdArg = UINT_MAX;
}

typedefs {
  typedef NodeCollection<OutgoingConnection> OutgoingConnectionMap;
  typedef NodeCollection<IncomingConnection> IncomingConnectionMap;
  typedef uint32_t seq_t;
//   typedef mace::hash_map<MaceTime, std::string, MaceTimeHash, MaceTimeEquality> IdMessageMap;
  typedef mace::hash_map<MaceTime, std::string> IdMessageMap;
  typedef mace::hash_set<registration_uid_t> RegIdSet;
//   typedef mace::deque<RtsData> RtsList;
  typedef mace::deque<registration_uid_t> RtsList;
  typedef boost::shared_ptr<MessageInfo> MessageInfoPtr;
  typedef mace::map<seq_t, MessageInfoPtr> MessagePtrMap;
  typedef mace::deque<MessageInfoPtr> MessagePtrList;
//   typedef mace::hash_set<MaceTime, MaceTimeHash, MaceTimeEquality> IdSet;
  typedef mace::hash_set<MonotoneTime, hash_bytes<MonotoneTime> > IdSet;
  typedef mace::map<MaceKey, MonotoneTime> MonotoneIdMap;
}

auto_types {
//   RtsData {
//     MonotoneTime syn = 0;
//     registration_uid_t rid = 0;
//   };

  MessageInfo {
    seq_t seq;
    string payload;
    registration_uid_t rid;
    uint32_t frag = 0;
    uint32_t nfrag = 0;
    uint32_t fragrecv = 0;
    MonotoneTime syn = 0;
    MaceTime starttime __attribute((dump(state = no))) = 0;
    MaceTime ts __attribute((dump(state = no))) = 0;
    MaceTime timer __attribute((dump(state = no))) = 0;
    uint retries = 0;
    bool timeout = true;
    MonotoneTime id = 0;
  };

  ConnectionInfo {
    int64_t sa __attribute((dump(no; state = no))) = -1;
    int64_t sv __attribute((dump(no; state = no))) = 0;
    int64_t rto __attribute((dump(state = no))) = INIT_RTO;
    uint consecutiveTimeouts = 0;
//     MaceTime lastHeard __attribute((dump(no))) = MaceTime::currentTime();
    double cwnd = 1.0;
    double ssthresh = MAX_WND;
    uint32_t packetsSent = 0;
    uint32_t acksReceived = 0;
    uint32_t packetsRetransmitted = 0;
    uint32_t timeoutCount = 0;

    void updateRTO(const MaceTime& ts) {
      int64_t m = curtime.time() - ts.time();
      if (sa == -1) {
	sa = m << 3;
	sv = 0;
	rto = m << 2;
      }
      else {
	m -= (sa >> 3);
	sa += m;
	if (m < 0) {
	  m = -1 * m;
	}
	m -= (sv >> 2);
	sv += m;
	rto = (sa >> 3) + sv;
      }
      rto += MIN_RTO;

      if (rto > MAX_RTO) {
	rto = MAX_RTO;
      }
      else if (rto < MIN_RTO) {
	rto += MIN_RTO;
      }

      consecutiveTimeouts = 0;
//       lastHeard = ts;
      acksReceived++;
    }

    void increaseCWND() {
      if (cwnd < ssthresh) {
	cwnd += 1.0;
      }
      else {
	cwnd += 1.0 / cwnd;
      }
      if (cwnd > MAX_WND) {
	cwnd = MAX_WND;
      }
      ASSERT(cwnd >= 1);
    }

    void timeout() {
      rto <<= 1;

      if (rto > MAX_RTO) {
	rto = MAX_RTO;
      }

      ssthresh = cwnd / 2.0;
      cwnd = 1.0;

      consecutiveTimeouts++;
      timeoutCount++;
    }

  }; // ConnectionInfo

  OutgoingConnection __attribute((node())) {
    ConnectionInfo connection;
    MessagePtrMap inflight;
    MessagePtrMap rtbuf; // retransmission queue
    MessagePtrList sendbuf; // transmission queue
    IdMessageMap unfragged;
    RtsList rts;

//     seq_t curseq = (Util::timeu() & 0xfffffff);
//     MonotoneTime syn = Util::timeu();
    seq_t curseq = getmtime() * 1000*1000;
    MonotoneTime syn = getmtime();
    bool returnSyn = true;
    size_t dupacks = 0; // number of duplicate acks
    seq_t last = 0; // last acknowledged seq
    uint64_t queued = 0;
    uint64_t sent = 0;
    uint64_t acked = 0;
    bool flushed = false;
    bool sendable = true;
    BufferStatisticsPtr bufStats __attribute((dump(no; state = no))) = BufferStatisticsPtr(new BufferStatistics(DEFAULT_WINDOW_SIZE));

    bool canSend() const {
      return sendable;
    }

    void updateCanSendOnWrite(uint64_t maxsize, uint64_t threshold) {
      if (!sendable && (bufferedDataSize() < std::min(maxsize, threshold))) {
	sendable = true;
      }
    }

    void updateCanSendOnEnqueue(uint64_t maxsize) {
      if (sendable && (bufferedDataSize() > maxsize)) {
	sendable = false;
      }
    }

    size_t bufferedDataSize() const {
      return ((queued - sent) + (sent - acked));
    }

    bool hasQueuedMessages() const {
      return !rtbuf.empty() || !sendbuf.empty();
    }

    size_t queuedMessagesSize() const {
      return rtbuf.size() + sendbuf.size();
    }

    seq_t nextSeq() {
      curseq++;
      return curseq;
    }

    MonotoneTime getSyn() {
      if (returnSyn) {
	returnSyn = false;
	return syn;
      }
      return 0;
    }

    void setLast(seq_t s) {
      if (last == s) {
	dupacks++;
      }
      else {
	last = s;
	dupacks = 0;
      }
    }

    bool fastRetransmit() const {
      return (dupacks >= FAST_RETRANSMIT_DUP);
    }
  };

  IncomingConnection __attribute((node())) {
    MessagePtrMap m; // buffered received messages
//     seq_t synSeq; // fix for MT bug # 1
//     MonotoneTime syn; // fix for MT bug # 1 and bug # 3
    seq_t next;
    seq_t nextDeliver;
    bool expectSyn = true;
    uint32_t discardedPackets = 0;
    BufferStatisticsPtr bufStats __attribute((dump(no; state = no))) = BufferStatisticsPtr(new BufferStatistics(DEFAULT_WINDOW_SIZE));
  };
}

state_variables {
  MaceKey me;
  OutgoingConnectionMap out;
  IncomingConnectionMap in;
  MonotoneIdMap lastSyn; // fix for MT bug # 1 and bug # 3
  RegIdSet notifyFlushed;
  size_t flushedCount = 0;
  size_t queueThreshold = queueThresholdArg;

  timer<MaceKey, MessageInfoPtr> retransmissionTimer __attribute((multi(yes; random)));
  timer<std::string, registration_uid_t> localTimer __attribute((multi(yes)));
  timer<registration_uid_t> notifyFlushedTimer __attribute((multi(yes)));
  // fix for MT bug # 10 - added MonotoneTime
  timer<MaceKey> ctsTimer __attribute((multi(yes)));
}

messages {

  Data {
    seq_t seq;
    registration_uid_t rid;
    MonotoneTime syn; // fix for MT bug # 3 (changed from bool)
    MonotoneTime synecho;
    string payload;
  };

  DataFragment {
    seq_t seq;
    registration_uid_t rid;
    MonotoneTime syn; // fix for MT bug # 3 (changed from bool)
    MonotoneTime synecho;
    uint32_t nfrag; // number of fragments
    string payload;
  };

  Fragment {
    seq_t seq;
    uint32_t frag; // fragment offset
    string payload;
  };

  Ack {
    // cumulative acknowledgments are sent for the last received sequence number
    seq_t seq;
  };

  Reset {
    MonotoneTime syn; // fix for MT bug # 7
    seq_t seq;
  };
  
}

transitions {
  downcall maceInit() {
    me = downcall_getLocalAddress();
    queueThreshold = ((queueThresholdArg == UINT_MAX) ? maxQueueSize / 2 : queueThresholdArg);
    if (queueThreshold == 0) {
      queueThreshold = 1;
    }
  } // maceInit

  downcall getLocalAddress() {
    return me;
  } // getLocalAddress

  downcall route(const MaceKey& dest, const std::string& s, registration_uid_t rid) {
    if (dest == me) {
      maceout << "scheduling localTimer for " << HashUtil::compute32bitHex(s) << Log::endl;
      localTimer.schedule(1, s, rid);
      return true;
    }
    if (dest != me) {
      OutgoingConnection& c = out.add(dest);
      if (c.canSend()) {
	enqueue(c, s, rid);
	sendPackets(c);
	return true;
      }
      return false;
    }
    ASSERT(0);
  } // route

  downcall requestFlushedNotification(registration_uid_t rid) {
    if (flushedCount == out.size()) {
      notifyFlushedTimer.schedule(1, rid);
    }
    else {
      notifyFlushed.insert(rid);
    }
  } // requestFlushedNotification

  downcall hasOutgoingBufferedData() {
    return (outgoingBufferedDataSize(-1) != 0);
  } // hasOutgoingBufferedData

  downcall outgoingBufferedDataSize() {
    uint64_t r = 0;
    for (OutgoingConnectionMap::const_iterator i = out.begin(); i != out.end(); i++) {
      r += i->second.bufferedDataSize();
    }
    return r;
  } // outgoingBufferedDataSize

  downcall setWindowSize(const MaceKey& peer, uint64_t size) {
    ASSERT(0);
  } // setWindowSize

  downcall getStatistics(const MaceKey& peer,
			 TransportServiceClass::Connection::type d) {
    if (d == TransportServiceClass::Connection::OUT) {
    }
    else {
      ASSERT(d == TransportServiceClass::Connection::IN);
    }
    return BufferStatisticsPtr();
  } // getStatistics
  
  downcall outgoingBufferedDataSize(const MaceKey& peer) {
    OutgoingConnectionMap::const_iterator i = out.find(peer);
    if (i != out.end()) {
      return i->second.bufferedDataSize();
    }
    else {
      return 0;
    }
  } // outgoingBufferedDataSize

  downcall hasBufferedData(registration_uid_t rid) {
    if (hasOutgoingBufferedData(rid)) {
      return true;
    }
    for (IncomingConnectionMap::const_iterator i = in.begin(); i != in.end(); i++) {
      if (!i->second.m.empty()) {
	return true;
      }
    }
    return false;
  } // hasBufferedData

  downcall canSend(const MaceKey& peer) {
    OutgoingConnectionMap::const_iterator i = out.find(peer);
    if (i != out.end()) {
      return i->second.canSend();
    }
    return true;
  } // canSend

  downcall requestToSend(const MaceKey& peer, registration_uid_t rid) {
    OutgoingConnection& c = out.add(peer);
//     c.rts.push_back(RtsData(c.syn, rid));
    c.rts.push_back(rid);
//     maceout << "pushed " << c.syn << " " << rid << " onto rts for " << peer << Log::endl;
    if (!c.canSend()) {
      return;
    }
    ctsTimer.schedule(1, peer);
  } // requestToSend

  downcall routeRTS(const MaceKey& dest, const std::string& s, registration_uid_t rid) {
    if (canSend(dest, rid)) { 
      route(dest, s, rid);
      return true;
    }
    else {
      requestToSend(dest, rid);
      return false;
    }
  } // routeRTS

  upcall deliver(const MaceKey& src, const MaceKey& dest, const Data& m) {
    processDeliver(src, dest, m.seq, m.payload, m.getSerializedSize(), m.rid, m.syn,
		   m.synecho);
  } // deliver Data

  upcall deliver(const MaceKey& src, const MaceKey& dest, const DataFragment& m) {
    processDeliver(src, dest, m.seq, m.payload, m.getSerializedSize(), m.rid, m.syn,
		   m.synecho, m.nfrag);
  } // deliver DataFragment

  upcall deliver(const MaceKey& src, const MaceKey& dest, const Fragment& m) {
    processDeliver(src, dest, m.seq, m.payload, m.getSerializedSize(), 0, 0, 0, 0,
		   m.frag);
  } // deliver Fragment

  upcall deliver(const MaceKey& src, const MaceKey& dest, const Ack& ack) {

    OutgoingConnectionMap::iterator ci = out.find(src);

    if (ci != out.end()) {
      maceout << "received ack from " << src << " for " << ack.seq << Log::endl;

      OutgoingConnection& c = ci->second;
      printConnection(c, "deliver");
      MessagePtrMap::iterator rti = c.rtbuf.begin();

//       if (rti != ci->second.rtbuf.end()) {
// 	maceout << "rtbuf.front=" << rti->first << " ack.seq=" << ack.seq << Log::endl;
//       }

//       if ((rti != ci->second.rtbuf.end()) && (rti->first <= ack.seq)) {
      bool nowarn = false;
      if (rti != c.rtbuf.end()) {
	maceout << "rtbuf.front=" << rti->first << " ack.seq=" << ack.seq << Log::endl;
	while ((rti != c.rtbuf.end()) && (rti->first <= ack.seq)) {
	  MessageInfoPtr m = rti->second;
	  maceout << "erasing " << *m << " from rtbuf" << Log::endl;
	  c.connection.updateRTO(m->ts);
	  c.connection.increaseCWND();
	  incrementAcked(c, m->payload.size());
	  c.rtbuf.erase(rti++);
	  rti = c.rtbuf.begin();
	  nowarn = true;
	}
      }

      if (!c.inflight.empty()) {
	MessagePtrMap::iterator mi = c.inflight.find(ack.seq);
	if ((mi == c.inflight.end()) && (c.inflight.rbegin()->first < ack.seq)) {
	  // fix for MT bug # 6
	  maceout << "clearing inflight" << Log::endl;
	  mi = c.inflight.begin();
	}

	if (mi != c.inflight.end()) {
	  MessageInfoPtr m = mi->second;
	  c.connection.updateRTO(m->ts);

	  mi = c.inflight.begin();
	  while ((mi != c.inflight.end()) && (mi->first <= ack.seq)) {
	    m = mi->second;
	    maceout << "erasing " << *m << " from inflight" << Log::endl;
	    retransmissionTimer.cancel(m->timer);
	    c.inflight.erase(mi);
	    c.connection.increaseCWND();
	    incrementAcked(c, m->payload.size());
	    mi = c.inflight.begin();
	  }

	  c.setLast(ack.seq);
	  sendPackets(c);
	  updateFlushed(c);
	  return;
	}
      }

      if (ack.seq == c.last) {
	maceout << "received duplicate ack from " << src << " for " << ack.seq
		<< Log::endl;

	c.connection.increaseCWND();
	c.setLast(ack.seq);
	if (c.fastRetransmit()) {
	  MessagePtrMap::iterator minext = c.inflight.find(ack.seq + 1);
	  if (minext != c.inflight.end()) {
	    c.connection.timeout();
	    MessageInfoPtr m = minext->second;
	    maceout << "fast retransmit for " << m->seq << Log::endl;
	    retransmissionTimer.cancel(m->timer);
	    c.inflight.erase(minext);
	    m->retries++;
	    c.rtbuf[m->seq] = m;
	  }
	}

	sendPackets(c);
	return;
      }

      if (nowarn) {
	return;
      }
    }

    macewarn << "received unknown ack from " << src << " for " << ack.seq << Log::endl;
//     ASSERT(0); // XXX
  } // deliver Ack

  upcall deliver(const MaceKey& src, const MaceKey& dest, const Reset& rst) {
    
    OutgoingConnectionMap::iterator ci = out.find(src);
    if (ci != out.end()) {
      OutgoingConnection& c = ci->second;
      maceout << "received reset from " << src << " with syn=" << rst.syn
	      << " seq=" << rst.seq << Log::endl;
      if (!rst.syn ||
	  (rst.syn >= c.syn) ||	// fix for MT bug # 7
	  c.inflight.containsKey(rst.seq) ||
	  c.rtbuf.containsKey(rst.seq)) {
	closeOutgoingConnection(src, c, TransportError::WRITE_ERROR, "connection reset");
      }
      else {
	maceout << "ignoring reset with syn " << rst.syn << " c.syn=" << c.syn
		<< Log::endl;
      }
    }
  } // deliver Reset

  scheduler retransmissionTimer(MaceKey& dest, MessageInfoPtr& m) {
    OutgoingConnectionMap::iterator ci = out.find(dest);
    if (ci != out.end()) {
      maceout << "dest=" << dest << " " << *m << Log::endl;
      OutgoingConnection& c = ci->second;

      printConnection(c, "retransmissionTimer begin");

      bool clearTimeout = false;
      if (m->timeout) {
	c.connection.timeout();
	maceout << "connection.timeout() " << c.connection << Log::endl;
	clearTimeout = true;
      }

      MaceTime oldestInflight = curtime;
      for (MessagePtrMap::iterator i = c.inflight.begin(); i != c.inflight.end(); i++) {
	oldestInflight = MaceTime::min(oldestInflight, i->second->starttime);
	if (clearTimeout) {
	  i->second->timeout = false;
	}
      }

      if (curtime.greaterThan(oldestInflight + (uint64_t)MAX_RTO, 0, 1)) {
	// peer failed
	closeBidirectionalConnection(dest, c, TransportError::WRITE_ERROR,
				     "send timeout");
	return;
      }

      c.inflight.erase(m->seq);

      m->retries++;
      c.rtbuf[m->seq] = m;
      maceout << "queued " << *m << " for retransmission" << Log::endl;

      MessagePtrMap::iterator i = c.inflight.begin();
      while (i != c.inflight.end() && (i->first < m->seq) &&
	     (i->second->ts.lessThan(m->ts))) {
	MessageInfoPtr mp = i->second;
	maceout << "queueing " << *mp << " for retransmission with older timeout"
		<< Log::endl;
	mp->retries++;
	c.rtbuf[mp->seq] = mp;
	retransmissionTimer.cancel(mp->timer);
	c.inflight.erase(i);
	i = c.inflight.begin();
      }

      printConnection(c, "retransmissionTimer end");
      sendPackets(c);
    }
  } // scheduler retransmissionTimer

  scheduler localTimer(std::string& s, registration_uid_t& rid) {
    maceout << "delivering " << HashUtil::compute32bitHex(s) << Log::endl;
    upcall_deliver(me, me, s, rid);
  } // scheduler localTimer

  scheduler notifyFlushedTimer(registration_uid_t& rid) {
    upcall_notifyFlushed(rid);
  } // notifyFlushedTimer

  scheduler ctsTimer(MaceKey& dest) {
    OutgoingConnectionMap::iterator i = out.find(dest);
    if (i != out.end()) {
      OutgoingConnection& c = i->second;
      while (!c.rts.empty() && c.canSend()) {
// 	maceout << "c.rts.size=" << c.rts.size() << Log::endl;
// 	const RtsData& d = c.rts.front();
// 	maceout << "d.syn=" << d.syn << " c.syn=" << c.syn << " rid=" << d.rid
// 		<< " dest=" << dest << Log::endl;
// 	if (d.syn == c.syn) {
//     	  upcall_clearToSend(dest, d.rid);
// 	}
	upcall_clearToSend(dest, c.rts.front());
	c.rts.pop_front();
      }
    }
  } // ctsTimer

} // transitions

routines {

  void incrementAcked(OutgoingConnection& c, size_t size) {
    c.acked += size;
    c.updateCanSendOnWrite(maxQueueSize, queueThreshold);
    if (!c.rts.empty() && c.canSend()) {
      ctsTimer.schedule(1, c.getId());
    }
  } // incrementAcked

  void closeIncomingConnection(const MaceKey& src, TransportError::type error,
			       const std::string& errstr, bool upcallError = true) {

    maceout << "closing incoming connection to " << src << Log::endl;
//     if (closeOut) {
//       OutgoingConnectionMap::iterator o = out.find(src);
//       if (o != out.end()) {
// 	closeConnection(src, o->second, error, errstr);
//       }
//       else {
// 	closeOut = false;
//       }
//     }
//     if (!closeOut) {
    if (upcallError) {
      // fix for MT10
      ctsTimer.cancel(src);
      if (out.containsKey(src)) {
	out.get(src).rts.clear();
      }
      upcallAllVoid(error, src, error, errstr);
    }
//     }

    in.erase(src);
    MonotoneIdMap::iterator i = lastSyn.find(src);
    if (i != lastSyn.end()) {
      i->second++;
    }

  } // closeIncomingConnection

  void closeOutgoingConnection(const MaceKey& dest, OutgoingConnection& c,
			       TransportError::type error, const std::string& errstr,
			       bool upcallError = true) {
    maceout << "closing outdoing connection to " << dest << Log::endl;
    if (c.flushed) {
      flushedCount--;
    }

    // fix for MT bug 2
    for (MessagePtrMap::const_iterator i = c.inflight.begin();
	 i != c.inflight.end(); i++) {
      retransmissionTimer.cancel(i->second->timer);
    }

    OutgoingConnection cc = c;
    out.erase(dest);

    if (upcallError) {
      // fix for MT10
      ctsTimer.cancel(dest);
      upcallAllVoid(error, dest, error, errstr);
    }

    if (UPCALL_MESSAGE_ERROR) {

      maceout << "upcalling message errors for " << cc << Log::endl;
      IdSet seen;
      upcallMessageErrors(cc.inflight.begin(), cc.inflight.end(), dest, cc, seen, error);
      upcallMessageErrors(cc.rtbuf.begin(), cc.rtbuf.end(), dest, cc, seen, error);
      upcallMessageErrors(cc.sendbuf.begin(), cc.sendbuf.end(), dest, cc, seen, error);
    }

    checkAndNotifyFlushed();
  } // closeOutgoingConnection

  void closeBidirectionalConnection(const MaceKey& dest, OutgoingConnection& c,
				    TransportError::type error,
				    const std::string& errstr) {
    maceout << "closing bidirectional connection to " << dest << Log::endl;
    // bug fix for MT # 7
    closeOutgoingConnection(dest, c, error, errstr);
    closeIncomingConnection(dest, error, errstr, false);
  } // closeBidirectionalConnection

  void deliverBuffered(IncomingConnection& c, const MaceKey& src, const MaceKey& dest) {
    MessagePtrMap::iterator i = c.m.begin();
    if (i != c.m.end()) {
//       maceout << "seq=" << i->second->seq << " nextDeliver=" << c.nextDeliver << Log::endl;
    }
    while (i != c.m.end() && i->second->seq == c.nextDeliver) {
      MessageInfoPtr m = i->second;

      maceout << "seq=" << m->seq << " nfrag=" << m->nfrag
	      << " fragrecv=" << m->fragrecv << Log::endl;

      if (m->nfrag) {
	if (m->nfrag == m->fragrecv) {
// 	  maceout << "erasing " << i->second->seq << Log::endl;
	  c.m.erase(i);
	  i = c.m.begin();
	  for (size_t j = 1; j < m->nfrag; j++) {
	    ASSERT(i != c.m.end());
// 	    maceout << "checking that " << i->first << " == " << m->seq + j
// 		    << " m->seq=" << m->seq << " j=" << j << Log::endl;
	    ASSERT(i->first == m->seq + j);
	    ASSERT(i->second->seq == m->seq + j);
	    m->payload.append(i->second->payload);
	    c.m.erase(i);
// 	    maceout << "erasing " << i->second->seq << Log::endl;
	    i = c.m.begin();
	  }
	  c.nextDeliver += (m->nfrag - 1);
	}
	else {
	  return;
	}
      }
      else {
	c.m.erase(i);
	i = c.m.begin();
      }
      maceout << "delivering " << m->seq << Log::endl;
      upcallDeliver(c, src, dest, m->payload, m->rid);
    }
  } // deliverBuffered

  void upcallDeliver(IncomingConnection& c, const MaceKey& src, const MaceKey& dest,
		     const std::string& payload, registration_uid_t rid) {
    upcall_deliver(src, dest, payload, rid);
    c.nextDeliver++;
  } // upcallDeliver

  void upcallMessageErrors(MessagePtrMap::iterator i, MessagePtrMap::iterator end,
			   const MaceKey& dest, OutgoingConnection& c, IdSet& seen,
			   TransportError::type error) {
    while (i != end) {
      upcallMessageError(dest, i->second, c, seen, error);
      i++;
    }
  } // upcallMessageErrors

  void upcallMessageErrors(MessagePtrList::iterator i, MessagePtrList::iterator end,
			   const MaceKey& dest, OutgoingConnection& c, IdSet& seen,
			   TransportError::type error) {
    while (i != end) {
      upcallMessageError(dest, *i, c, seen, error);
      i++;
    }
  } // upcallMessageErrors

  void upcallMessageError(const MaceKey& dest, MessageInfoPtr m, OutgoingConnection& c,
			  IdSet& seen, TransportError::type error) {
    if (!m->nfrag) {
      upcallMessageError(dest, m, c, error);
    }
    else if (!seen.contains(m->id)) {
      upcallMessageError(dest, m, c, error);
      seen.insert(m->id);
    }
  } // upcallMessageError

  void upcallMessageError(const MaceKey& dest, MessageInfoPtr m, OutgoingConnection& c,
			  TransportError::type error) {
    if (m->nfrag) {
      const std::string& data = c.unfragged.get(m->id);
      maceout << "upcalling message error for " << HashUtil::compute32bitHex(data)
	      << Log::endl;
      upcall_messageError(dest, error, data, m->rid);
    }
    else {
      maceout << "upcalling message error for " << HashUtil::compute32bitHex(m->payload)
	      << Log::endl;
      upcall_messageError(dest, error, m->payload, m->rid);
    }
  } // upcallMessageError

  void processDeliver(const MaceKey& src, const MaceKey& dest,
		      seq_t seq, string payload, uint32_t ssize,
		      registration_uid_t rid = 0, MonotoneTime syn = 0,
		      MonotoneTime synecho = 0,
		      uint32_t nfrag = 0, uint32_t frag = 0) {
    ASSERT(src != me);
    seq_t ackseq = 0;
    bool discard = false;
    IncomingConnection& c = sendAck(src, seq, syn, synecho, ssize, ackseq, discard);
    if (discard) {
      return;
    }

    maceout << src << " seq=" << seq << " ackseq=" << ackseq << " nextDeliver="
	    << c.nextDeliver << " syn=" << syn << " nfrag=" << nfrag
	    << " frag=" << frag << Log::endl;

    if (c.m.containsKey(seq)) {
      MessageInfoPtr minfo = c.m[seq];
      // fix for MT bug # 11
      minfo->retries++;
      if (minfo->retries >= FAST_RESET_DUP) {
	maceout << "performing fast reset on connection from " << src << Log::endl;
	sendReset(src, 0, seq);
	closeIncomingConnection(src, TransportError::READ_ERROR,
				"connection reset");
      }
      else {
	// fix for MT bug # 8
	maceout << "ignoring duplicate buffered message " << seq << " with "
		<< minfo->retries << " retries" << Log::endl;
      }
      return;
    }

    if (frag || nfrag) {
      MessageInfoPtr minfo = MessageInfoPtr(new MessageInfo(seq, payload, rid,
							    frag, nfrag,
							    (nfrag ? 1 : 0)));
      c.m[seq] = minfo;

      if (nfrag) {
	// see if we already have fragments for this message
	seq_t last = seq + nfrag;
	MessagePtrMap::iterator i = c.m.find(seq);
	i++;
	while ((i != c.m.end()) && (i->second->seq < last)) {
	  minfo->fragrecv++;
	  maceout << "already received " << i->second->seq
		  << ", incremented fragrecv to " << minfo->fragrecv << Log::endl;
	  i++;
	}
	maceout << "received DataFragment seq=" << minfo->seq << " nfrag=" << minfo->nfrag
		<< " last=" << last << " fragrecv=" << minfo->fragrecv << Log::endl;
      }
      else {
	ASSERT(frag);
	seq_t first = seq - frag;
	MessagePtrMap::iterator i = c.m.find(first);
	if (i != c.m.end()) {
	  i->second->fragrecv++;
	  maceout << "received Fragment seq=" << minfo->seq << " frag=" << minfo->frag
		  << ", incremented fragrev=" << i->second->fragrecv << Log::endl;
	}
	else {
	  maceout << "received Fragment seq=" << minfo->seq << " frag=" << minfo->frag
		  << ", did not yet receive correpsonding data frag" << Log::endl;
	}
      }
    }
    else {
      if (ackseq < seq) {
	c.m[seq] = MessageInfoPtr(new MessageInfo(seq, payload, rid));
	return;
      }

//       ASSERT(ackseq == seq); 

//       MaceKey nexthop = me;
//       bool f = upcall_forward(src, dest, nexthop, payload, COMM_TYPE_UNICAST, rid);
//       if (!f) {
// 	c.nextDeliver++;
//       }
//       else {
      ASSERT(seq == c.nextDeliver);
      upcallDeliver(c, src, dest, payload, rid);
//       }
    }

    deliverBuffered(c, src, dest);
  } // processDeliver

  IncomingConnection& sendAck(const MaceKey& src, seq_t seq, MonotoneTime syn,
			      MonotoneTime synecho, size_t sz, seq_t& ackseq,
			      bool& discard) {
    discard = false;
    if (syn) {
      maceout << "received syn=" << syn << " synecho=" << synecho
	      << " from " << src << Log::endl;
      MonotoneIdMap::iterator i = lastSyn.find(src);
      if (i != lastSyn.end()) {
	if (syn < i->second) {
	  maceout << "received syn with old time" << syn << " < " << i->second << Log::endl;
	  discard = true;
	  ackseq = 0;
	  sendReset(src, syn, seq);
	}
	else if (i->second == syn) {
	  maceout << "received duplicate syn " << syn << Log::endl;
	}
	else {
	  OutgoingConnectionMap::iterator o = out.find(src);
	  bool closeIn = true;
	  if (o != out.end()) {
	    OutgoingConnection& c = o->second;
	    if (c.syn != synecho) {
	      maceout << "closing bidirectional connection synecho=" << synecho
		      << " syn=" << c.syn << Log::endl;
	      closeBidirectionalConnection(src, c, TransportError::READ_ERROR,
					   "connection reset");
	      closeIn = false;
	    }
	    if (closeIn) {
	      maceout << "closing incoming connection on unexpected syn" << Log::endl;
	      closeIncomingConnection(src, TransportError::READ_ERROR,
				      "connection reset");
	    }
	  }
	}
      }
    }
    IncomingConnection& c = in.add(src);
    if (discard) {
      return c;
    }
    c.bufStats->append(sz);
    if (c.expectSyn) {
      if (syn) {
	c.expectSyn = false;
	c.next = seq;
	c.nextDeliver = seq;
// 	c.synSeq = seq;
// 	c.syn = syn; // fix for bugs # 1 and # 3
	lastSyn[src] = syn; // fix for bugs # 1 and # 3
      }
      else {
	discard = true;
	sendReset(src, 0, seq);
	return c;
      }
    }

    maceout << "src=" << src << " seq=" << seq << " c.next=" << c.next << Log::endl;

    if (c.next == seq) {
      c.next++;
      MessagePtrMap::const_iterator i = c.m.begin();
      while (i != c.m.end() && i->second->seq <= c.next) {
	if (i->second->seq == c.next) {
	  maceout << "incrementing c.next to " << c.next << Log::endl;
	  c.next++; // fix for MT bug # 4
	}
	i++;
      }
    }
    else {
      if (seq > c.next) {
	macewarn << "received out-of-order message with seq " << seq
		 << " > next " << c.next << Log::endl;
      }
      else {
	macewarn << "discarding message with seq=" << seq << " next=" << c.next
		 << Log::endl;
	discard = true;
	c.discardedPackets++;
      }
//       ackseq--; // fix for bug # 5 (moved from seq > c.next to both cases)
    }
    ackseq = c.next - 1;

    maceout << "sending ack to " << src << " for " << ackseq << Log::endl;
    downcall_route(src, Ack(ackseq));

    return c;
  } // sendAck

  void sendReset(const MaceKey& dest, MonotoneTime id, seq_t seq) {
    if (!id) {
      MonotoneIdMap::const_iterator i = lastSyn.find(dest);
      if (i != lastSyn.end()) {
	id = i->second;
      }
    }
    maceout << "sending reset to " << dest << " with syn=" << id << Log::endl;
    downcall_route(dest, Reset(id, seq));
  } // sendReset

  void sendPackets(OutgoingConnection& c) {
    printConnection(c, "sendPackets");
    MonotoneTime synecho = 0;
    MonotoneIdMap::const_iterator ii = lastSyn.find(c.getId());
    if (ii != lastSyn.end()) {
      synecho = ii->second;
    }
    while (c.hasQueuedMessages() && (c.inflight.size() < (size_t)c.connection.cwnd)) {
      MessageInfoPtr m;
      if (!c.rtbuf.empty()) {
	m = c.rtbuf.begin()->second;
	c.rtbuf.erase(c.rtbuf.begin());
	maceout << "got message from rtbuf, size remaining=" << c.rtbuf.size()
		<< Log::endl;
	c.connection.packetsRetransmitted++;
      }
      else if (!c.sendbuf.empty()) {
	m = c.sendbuf.front();
	c.sendbuf.pop_front();
	maceout << "got message from sendbuf, size remaining=" << c.sendbuf.size()
		<< Log::endl;
	c.sent += m->payload.size();
	c.connection.packetsSent++;
	c.updateCanSendOnWrite(maxQueueSize, queueThreshold);
      }
      else {
	ASSERT(0);
      }

      if (m) {
	ASSERT(!c.inflight.containsKey(m->seq));
	c.inflight[m->seq] = m;
	const MaceKey& dest = c.getId();

	m->ts = curtime;
	if (m->starttime == 0) {
	  m->starttime = curtime;
	}

	maceout << "scheduling timer for " << c.connection.rto << " " << dest << " " << *m
		<< Log::endl;
	m->timer = retransmissionTimer.schedule(c.connection.rto, dest, m);
	m->timeout = true;

	maceout << "sending " << *m << " payload size=" << m->payload.size() << Log::endl;

	size_t bytes = 0;
	if (m->nfrag) {	
	  if (m->frag) {
	    bytes = downcall_send(dest, Fragment(m->seq, m->frag, m->payload));
	  }
	  else {
	    bytes = downcall_send(dest, DataFragment(m->seq, m->rid, m->syn, synecho,
						     m->nfrag, m->payload));
	  }
	}
	else {
	  bytes = downcall_send(dest, Data(m->seq, m->rid, m->syn, synecho, m->payload));
	}
	c.bufStats->append(bytes);
      }
    }

    if (!c.hasQueuedMessages()) {
      maceout << "no more queued packets to " << c.getId() << Log::endl;
    }
    else {
      maceout << "inflight " << c.inflight.size() << " >= " << (size_t)c.connection.cwnd
	      << " cwnd to " << c.getId() << Log::endl;
    }
  } // sendPackets

  void enqueue(OutgoingConnection& c, const std::string& s, registration_uid_t rid) {
    maceout << "s.size=" << s.size()
	    << " MAX_DATA_FRAGMENT_MESSAGE_SIZE=" << MAX_DATA_FRAGMENT_MESSAGE_SIZE
	    << " MAX_FRAGMENT_MESSAGE_SIZE=" << MAX_FRAGMENT_MESSAGE_SIZE
	    << " UdpTransport::MAX_MESSAGE_SIZE=" << UdpTransport::MAX_MESSAGE_SIZE
	    << Log::endl;
    if (s.size() <= MAX_DATA_MESSAGE_SIZE) {
      MessageInfoPtr m = MessageInfoPtr(new MessageInfo(c.nextSeq(), s, rid, 0, 0, 0,
							c.getSyn()));
      c.sendbuf.push_back(m);
      maceout << "enqueued " << *m << Log::endl;
    }
    else {
      size_t sz = s.size() - MAX_DATA_FRAGMENT_MESSAGE_SIZE;
      uint32_t nfrag = 1;
      nfrag += sz / MAX_FRAGMENT_MESSAGE_SIZE;
      if (sz % MAX_FRAGMENT_MESSAGE_SIZE) {
	nfrag++;
      }
      maceout << "nfrag=" << nfrag << Log::endl;
      size_t off = 0;
      MonotoneTime mid = getmtime();
      for (uint32_t i = 0; i < nfrag; i++) {
	size_t len = std::min(MAX_FRAGMENT_MESSAGE_SIZE, s.size() - off);
	if (i == 0) {
	  len = MAX_DATA_FRAGMENT_MESSAGE_SIZE;
	}
	maceout << "enqueueing frag " << i << " off=" << off << " len=" << len
		<< Log::endl;
	MessageInfoPtr m = MessageInfoPtr(new MessageInfo(c.nextSeq(),
							  s.substr(off, len),
							  rid, i, nfrag, 0, c.getSyn()));
	c.sendbuf.push_back(m);
// 	m->id = curtime.time();
	m->id = mid;
	maceout << "enqueued " << *m << Log::endl;
	off += len;
      }
      c.unfragged[mid] = s;
    }
    c.queued += s.size();
    if (c.flushed) {
      flushedCount--;
      c.flushed = false;
    }
//     printConnection(c, "enqueue");
    c.updateCanSendOnEnqueue(maxQueueSize);
  } // enqueue

  void updateFlushed(OutgoingConnection& c) {
    ASSERT(!c.flushed);
    if (c.bufferedDataSize() == 0) {
      c.flushed = true;
      flushedCount++;
      checkAndNotifyFlushed();
    }
  } // updateFlushed

  void checkAndNotifyFlushed() {
    if (flushedCount == out.size()) {
      for (RegIdSet::const_iterator i = notifyFlushed.begin();
	   i != notifyFlushed.end(); i++) {
	upcall_notifyFlushed(*i);
      }
      notifyFlushed.clear();
    }
  } // checkAndNotifyFlushed

  void printConnection(const OutgoingConnection& c, const std::string& s) const {
    if (maceout.isNoop()) {
      return;
    }

    if (!s.empty()) {
      maceout << s << ":" << endl;
    }
    maceout << c.connection << endl;
    maceout << "inflight=" << c.inflight.size() << " rtbuf=" << c.rtbuf.size()
	    << " sendbuf=" << c.sendbuf.size()
	    << " queued=" << c.queued << " sent=" << c.sent << " acked=" << c.acked
	    << endl;
    if (1) {
      if (!c.inflight.empty()) {
	maceout << "inflight: " << c.inflight.begin()->first << " "
		<< c.inflight.rbegin()->first << endl;
      }
      //       maceout << "inflight:" << endl;
      //       for (MessagePtrMap::const_iterator i = c.inflight.begin();
      // 	   i != c.inflight.end(); i++) {
      // // 	maceout << i->first << "=" << *(i->second) << endl;
      // 	maceout << i->first << endl;
      //       }
      if (!c.rtbuf.empty()) {
	maceout << "rtbuf: " << c.rtbuf.begin()->first << " "
		<< c.rtbuf.rbegin()->first << endl;
      }
      //       maceout << "rtbuf:" << endl;
      //       for (MessagePtrMap::const_iterator i = c.rtbuf.begin(); i != c.rtbuf.end(); i++) {
      // // 	maceout << i->first << "=" << *(i->second) << endl;
      // 	maceout << i->first << endl;
      //       }
      if (!c.sendbuf.empty()) {
	maceout << "sendbuf: " << (*c.sendbuf.begin())->seq << " "
		<< (*c.sendbuf.rbegin())->seq << endl;
      }
      //       maceout << "sendbuf:" << endl;
      //       for (MessagePtrList::const_iterator i = c.sendbuf.begin(); i != c.sendbuf.end(); i++) {
      // 	maceout << **i << endl;
      //       }
    }

    //     maceout << "unfragged: " << endl;
    //     for (IdMessageMap::const_iterator i = c.unfragged.begin(); i != c.unfragged.end(); i++) {
    //       maceout << i->first << "=" << i->second << endl;
    //     }
    //     maceout << Log::endl;
//     maceout << "unfragged: " << endl;
//     for (IdMessageMap::const_iterator i = c.unfragged.begin(); i != c.unfragged.end(); i++) {
//       maceout << i->first << "=" << i->second << endl;
//     }
    maceout << Log::endl;
  } // printConnection

  /********************************************************************************
   * methods for properties
   ********************************************************************************/

  size_t inflightSize() const {
    size_t r = 0;
    for (OutgoingConnectionMap::const_iterator i = out.begin(); i != out.end(); i++) {
      r += i->second.inflight.size();
    }
    return r;
  }

  bool checkRetransmissionTimer() const {
    for (OutgoingConnectionMap::const_iterator i = out.begin(); i != out.end(); i++) {
      const OutgoingConnection& c = i->second;
      for (MessagePtrMap::const_iterator mi = c.inflight.begin();
	   mi != c.inflight.end(); mi++) {
	if (!retransmissionTimer.isScheduled(mi->second->timer)) {
	  return false;
	}
      }
    }
    return true;
  }

  bool checkRetransmissionTimerCount() const {
    size_t count = 0;
    for (OutgoingConnectionMap::const_iterator i = out.begin(); i != out.end(); i++) {
      const OutgoingConnection& c = i->second;
      count += c.inflight.size();
    }
    return count == retransmissionTimer.scheduledCount();
  } // checkRetransmissionTimerCount

  uint64_t outgoingBufferedDataSize() const {
    uint64_t r = 0;
    for (OutgoingConnectionMap::const_iterator i = out.begin(); i != out.end(); i++) {
      r += i->second.bufferedDataSize();
    }
    return r;
  }

} // routines

properties {
  safety {
    retransmissionTimerScheduled : \forall n \in \nodes : n.checkRetransmissionTimer();
    retransmissionTimerScheduledCount :
      \forall n \in \nodes : n.checkRetransmissionTimerCount();

  } // safety

  liveness {
    inflightEmpty : \forall n \in \nodes : n.inflightSize() = 0;
    bufferedDataEmpty : \forall n \in \nodes : n.outgoingBufferedDataSize() = 0;
    sequenceNumbersMatch : {
      \forall n \in \nodes :
	\forall c \in n.out :
	((\not c->first.in.contains(n)) \or
	 (c->second.curseq + 1 = c->first.in.get(n).nextDeliver))
    };
  } // liveness
} // properties
