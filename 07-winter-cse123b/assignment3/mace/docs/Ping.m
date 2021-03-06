#include "mmultimap.h"
#include "mlist.h"

service;

provides Ping;

services {
  Transport router = UdpTransport();
  //   Transport router = TcpTransport();
} // services

constants {
  uint64_t DEFAULT_PING_INTERVAL = 1*1000*1000; // useconds
  uint64_t DEFAULT_PING_TIMEOUT = (DEFAULT_PING_INTERVAL + 5*100*1000); // useconds
} // constants

constructor_parameters {
  uint64_t PING_INTERVAL = DEFAULT_PING_INTERVAL;
  uint64_t PING_TIMEOUT = DEFAULT_PING_TIMEOUT;
} // constructor_parameters

states {
  ready;
} // states

typedefs {
  typedef mace::list<uint64_t> PingTimeList;
  typedef mace::hash_map<MaceKey, PingTimeList> PingListMap;
  typedef mace::hash_map<MaceKey, uint64_t> TimeMap;
  typedef mace::multimap<MaceKey, registration_uid_t> RegistrationMap;
} // typedefs

method_remappings {
  implements {
    upcalls {
      messageError(MaceKey const & k, TransportError::type code, std::string const & m);
    }
  }
}

messages {
  Ping { 
    uint64_t t;
  }
  PingReply {
    uint64_t techo;
    uint64_t t;
  }
} // messages

state_variables {
  PingListMap sentTimes;
  TimeMap responseTimes;
  RegistrationMap rids;
  timer sendtimer __attribute((recur(PING_INTERVAL)));
} // state_variables

transitions {
  downcall (state == init) maceInit() {
    state = ready;
    sendtimer.schedule(PING_INTERVAL);
  } // downcall maceInit

  (state == ready) {
    upcall deliver(const MaceKey& src, const MaceKey& dest, const Ping& msg) {
      downcall_route(src, PingReply(msg.t, curtime));
    } // deliver Ping

    upcall deliver(const MaceKey& src, const MaceKey& dest, const PingReply& msg) {
      if(sentTimes[src].empty() || sentTimes[src].front() > msg.techo) {
        return; // ping already notified as failure
      }
      while(!sentTimes[src].empty() && sentTimes[src].front() <= msg.techo) {
        sentTimes[src].pop_front();
      }
      responseTimes[src] = curtime;
      if(curtime-msg.techo <= PING_TIMEOUT) {
        upcallSuccess(src, msg.techo, responseTimes[src], msg.t);
      } else {
        upcallFailure(src, msg.techo);
      }
    } // deliver PingReply

    downcall monitor(const MaceKey& host, registration_uid_t rid) {
      if (rids.find(host) == rids.end()) {
	ping(host);
      }
      rids.insert(std::make_pair(host, rid));
    } // downcall monitor

    downcall getLastResponseTime(const MaceKey& host) {
      return responseTimes[host];
    } // downcall getLastResponseTime

    upcall messageError(const MaceKey& k, TransportError::type code,
			const std::string& m) {
      maceout << "timeout to " << k << " (" << code << ") " << Log::toHex(m) << Log::endl;
    }
  } // state == ready

  scheduler sendtimer() {
    for (PingListMap::iterator i = sentTimes.begin(); i != sentTimes.end(); i++) {
      if(!i->second.empty() && curtime - i->second.front() > PING_TIMEOUT) {
	upcallFailure(i->first, i->second.front());
        while(!i->second.empty() && curtime - i->second.front() > PING_TIMEOUT) {
          i->second.pop_front();
        }
      }
      ping(i->first);
    }
  }
} // transitions

routines {
  void ping(const MaceKey& host) {
    downcall_route(host, Ping(curtime));
    sentTimes[host].push_back(curtime);
  } // ping

  void upcallSuccess(const MaceKey& k, uint64_t ts, uint64_t tr, uint64_t rt) {
    for (RegistrationMap::iterator i = rids.find(k); i != rids.end() && i->first == k; i++) {
      upcall_hostResponseReceived(k, ts, tr, rt, i->second);
    }
  } // upcallSuccess

  void upcallFailure(const MaceKey& k, uint64_t ts) {
    for (RegistrationMap::iterator i = rids.find(k); i != rids.end() && i->first == k; i++) {
      upcall_hostResponseMissed(k, ts, i->second);
    }
  } // upcallFailure
} // routines
