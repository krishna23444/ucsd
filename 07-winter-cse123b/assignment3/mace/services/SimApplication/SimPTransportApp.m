#include "NodeCollection.h"

using mace::NodeCollection;

service SimPTransportApp;
provides SimApplication;

trace = med;

states {
  needToSend;
  done;
}

services {
  BufferedTransport router;
  ProvisionalTransport pt = ProvisionalTransport(1, 1, router);
}

typedefs {
  typedef NodeCollection<PeerCounters> CountMap;
//   typedef NodeCollection<StaticCounters> StaticCountMap;
  typedef mace::set<message_id_t> IdSet;
}

auto_types {
//   StaticCounters __attribute((node())) {
//     int allReceived = 0;
//   }

  PeerCounters __attribute((node())) {
    IdSet inProgress;
    size_t sent = 0;
    size_t received = 0;
    size_t canceled = 0;
//     int incarnation = 0;
//     size_t inError = 0;
//     size_t outError = 0;
//     bool isWaitingForCTS = false;

//     int getIncarnation() const {
//       return incarnation;
//     }
  };
}

state_variables {
  size_t messageCount;
  size_t messageSize;
//   bool useRTS;
  CountMap counts;
  mace::string message __attribute((dump(no)));
//   StaticCountMap staticCounts __attribute((reset(no)));
  bool isFlushed = false;
  MaceKey me = MaceKey::null;
  int numNodes = -1;
  size_t maxCancel = 3;
//   int incarnation = -1;
}

messages {
  Data {
    mace::string payload;
  };
}

transitions {
  downcall (state == init) maceInit() {
    me = downcall_getLocalAddress();

    numNodes = upcall_getNodeCount();
//     incarnation = upcall_getIncarnationNumber();
    state = needToSend;
    Params* p = Params::Instance();
    messageCount = p->get<size_t>("messageCount");
    messageSize = p->get<size_t>("messageSize");
    ASSERT(messageSize);
    for (int i = 0; i < numNodes; i++) {
      if (i == upcall_getNodeNumber()) {
	continue;
      }
      const MaceKey& k = upcall_getMaceKey(i);
      counts.add(k);
//       if (!staticCounts.containsKey(k)) {
// 	staticCounts.add(k).allReceived = 0;
//       }
    }

    message.append(messageSize, 0);
  }

  downcall eventsWaiting() {
    if (state != done) {
//       if (useRTS) {
// 	size_t c = 0;
// 	for (CountMap::const_iterator i = counts.begin(); i != counts.end(); i++) {
// 	  if (i->second.curMessage == messageCount || i->second.isWaitingForCTS) {
// 	    c++;
// 	  }
// 	}
// 	if (c == counts.size()) {
// 	  return false;
// 	}
//       }
      return true;
    }
    return false;
  }

  downcall simulateEvent() {
    if (state == init) {
      maceInit();
      return "maceInit()";
    }
    else if (state == needToSend) {
      mace::map<MaceKey, unsigned> dests;
      for (CountMap::const_iterator i = counts.begin(); i != counts.end(); i++) {
// 	if (i->second.sent < messageCount || !i->second.inProgress.empty()) {
	if (i->second.sent < messageCount) {
	  dests[i->first] = 1;
	}
      }
      const MaceKey& dest = RandomUtil::random(dests);

      PeerCounters& c = counts.get(dest);
//       if (!c.inflight.empty()) {
// 	if (RandomUtil::randInt(2, 10, 1)) {
// 	  c.curMessage--;
// 	  message_id_t id = *(c.inflight.begin());
// 	  c.inflight.erase(id);
// 	  downcall_cancel(id);
// 	  return "cancel()";
// 	}
//       }

      if (downcall_route(dest, Data(message), router)) {
	c.sent++;
	checkAndSetDone();
      }

      return "route()";
    }
    ASSERT(0);
  }

  upcall deliver(const MaceKey& src, const MaceKey& dest, const Data& m) {
    PeerCounters& c = counts.get(src);
    c.received++;
//     if (m.incarnation < c.incarnation) {
//       maceout << "ingnoring message with old incarnation " << c.incarnation << Log::endl;
//       return;
//     }
//     if (m.incarnation > c.incarnation) {
//       ASSERT(m.seq == 0);
//       c.incarnation = m.incarnation;
//       c.inError = m.error;
//     }
//     if (m.error > c.inError) {
//       maceout << "m.error=" << m.error << " c.inError=" << c.inError << Log::endl;
//       c.inError = m.error;
//       c.expectedMessage = 0;
//     }
//     if (m.incarnation > c.incarnation || m.error > c.inError) {
//       ASSERT(m.seq == 0);
//       c.incarnation = m.incarnation;
//       c.inError = m.error;
//       c.expectedMessage = 0;
//     }
//     if () {
//       maceout << "m.error=" << m.error << " c.inError=" << c.inError << Log::endl;
//       c.inError = m.error;
//     }

//     if (m.seq != c.expectedMessage) {
//       maceerr << "m.seq=" << m.seq << " exp=" << c.expectedMessage
// 	      << " m.inc=" << m.incarnation << " c.inc=" << c.incarnation << Log::endl;
//     }
//     ASSERT(m.seq == c.expectedMessage);
//     c.expectedMessage++;
//     if (c.expectedMessage == messageCount) {
//       staticCounts.get(c.getId()).allReceived = 1;
//     }

  }

//   upcall clearToSend(const MaceKey& dest) {
//     counts.get(dest).isWaitingForCTS = false;
//     if (state == needToSend) {
//       routeRTS(dest);
//     }
//   }

  upcall notifyFlushed() {
    isFlushed = true;
  }

//   upcall error(const MaceKey& dest, TransportError::type ec,
// 	       const std::string& mesasge) {
//     PeerCounters& c = counts.get(dest);
//     c.outError++;
// //     if (ec != TransportError::WRITE_ERROR) {
// //       c.expectedMessage = 0;
// //     }
//     c.curMessage = 0;
//     state = needToSend;
//     isFlushed = false;
//     c.isWaitingForCTS = false;
//   }

//   upcall messageError(const MaceKey& dest, TransportError::type ec, const Data& m) {
//     if (ec == TransportError::READ_ERROR) {
//       if (!downcall_route(dest, m)) {
// 	state = needToSend;
// 	counts.get(dest).curMessage = std::min(m.seq, counts.get(dest).curMessage);
// 	isFlushed = false;
//       }
//     }
//   }
}

routines {

  void checkAndSetDone() {
    size_t count = 0;
    for (CountMap::const_iterator i = counts.begin(); i != counts.end(); i++) {
      if (i->second.sent == messageCount) {
	count++;
      }
    }
    if (count == counts.size()) {
      downcall_requestFlushedNotification();
      state = done;
    }
  } // checkAndSetDone

//   void routeRTS(const MaceKey& dest) {
//     PeerCounters& c = counts.get(dest);
//     if (downcall_routeRTS(dest, Data(c.curMessage, incarnation, c.outError, message))) {
//       c.curMessage++;
//       checkAndSetDone();
//     }
//     else {
//       c.isWaitingForCTS = true;
//     }
//   } // routeRTS

}

properties {
  safety {
//     incarnationOrdering : {
//       \forall n \in \nodes : 
// 	\forall c \in n.counts :
// 	(c->first.state = init) \or
// 	(c->first.incarnation \geq c->second.getIncarnation())
//     };

//     sequenceOrdering : {
//       \forall n \in \nodes : 
// 	\forall c \in n.counts :
// 	(c->first.state = init) \or
// 	(c->first.incarnation > c->second.getIncarnation()) \or
// 	(c->first.counts.get(n).outError > c->second.inError) \or
// 	(c->first.counts.get(n).curMessage \geq c->second.expectedMessage)
//     };

    messageCountCheck : \forall n \in \nodes :
      \forall c \in n.counts : c->second.sent \leq n.messageCount;
  }

  liveness {
//     allMessagesDelivered : {
//       \forall n \in \nodes : 
// 	\forall c \in n.counts :
// 	((n.counts.add(c->first).expectedMessage = 0 \and n.staticCounts.get(c->first).allReceived) \or
// 	 ((n.counts.contains(c->first) \and
// 	   (n.messageCount = n.counts.get(c->first).expectedMessage))))
//     };
    allMessagesDelivered : {
      \forall n \in \nodes : 
	\forall c \in n.counts :
	(n.counts.contains(c->first) \and
	 (n.messageCount = n.counts.get(c->first).received))
    };

    allMessagesSent : \forall n \in \nodes : 
      \forall c \in n.counts : c->second.sent = n.messageCount;

    allFlushed : \forall n \in \nodes : n.isFlushed;
  }
}
