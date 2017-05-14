#include "VersionedMap.h"
#include "QuorumCollection.h"

using mace::string;
using mace::QuorumCollection;

typedef uint32_t seq_id_t;

service ReplicatedMap;

provides ReplicatedMap;

constants {
  int QUORUM_EXISTS_TYPE = 0;
  int QUORUM_GET_TYPE = 1;
  int QUORUM_PREPARE_PUT_TYPE = 2;
  uint64_t DEBUG_INTERVAL = 10*1000*1000;
}

services {
  Transport router = TcpTransport();
}

states {
  ready;
}

auto_types {
  QuorumData __attribute((node(score=seq))) {
    seq_id_t seq;
  }

  QuorumState {
    QuorumCollection<QuorumData> q;
    int type;
    registration_uid_t rid;
    uint64_t timestamp;
    string key;
    string value;
    bool overwrite;
    seq_id_t seq;
  }

  PreparedData {
    string key;
    string value;
    seq_id_t seq;
    uint64_t timestamp;
  }
}

typedefs {
  typedef mace::VersionedMap<std::string, std::string, seq_id_t> VMap;
  typedef mace::hash_map<seq_id_t, QuorumState> QuorumMap;
  typedef mace::hash_map<seq_id_t, PreparedData> SeqPreparedDataMap;
  typedef mace::hash_map<MaceKey, SeqPreparedDataMap> PreparedDataMap;
  typedef QuorumCollection<QuorumData> QuorumDataCollection;
}

messages {
  Lookup {
    string key;
    bool get;
    seq_id_t mseq;
  }

  LookupReply {
    seq_id_t seq;
    bool get;
    string value;
    seq_id_t mseq;
  }

  PreparePut {
    string key;
    string value;
    seq_id_t seq;
    seq_id_t mseq;
  }

  PreparePutReply {
    seq_id_t seq;
    seq_id_t mseq;
  }

  CommitPut {
    seq_id_t mseq;
  }

  AbortPut {
    seq_id_t mseq;
  }

//   PrepareErase {
//     string key;
//     seq_id_t seq;
//   }

//   PrepareEraseReply {
//     seq_id_t seq;
//   }

//   CommitErase {
//     string key;
//   }
} // messages

state_variables {
  MaceKey laddr;
  NodeSet allServers;
  NodeSet servers;
  VMap data;
  QuorumMap quorums;
  PreparedDataMap prepared;
  seq_id_t mseq;
  timer printer __attribute((recur(DEBUG_INTERVAL)));
}

transitions {

  downcall maceInit() {
    mseq = 0;
    printer.schedule(DEBUG_INTERVAL);
  } // maceInit

  downcall setServers(const NodeSet& s) {
    laddr = downcall_getLocalAddress();
    allServers = s;
    servers = s;
    servers.erase(laddr);
    state = ready;
  } // setServers

  downcall exists(const std::string& key, registration_uid_t rid) {
    createQuorum(QUORUM_EXISTS_TYPE, key, rid);
    for (NodeSet::const_iterator i = servers.begin(); i != servers.end(); i++) {
      downcall_route(*i, Lookup(key, false, mseq));
    }
    // XXX - schedule a timer
  } // exists

  upcall deliver(const MaceKey& src, const MaceKey& dest, const Lookup& m) {
    const VMap::Element& el = data.get(m.key);
    downcall_route(src, LookupReply(el.contained ? el.version : 0, m.get,
				    (el.contained && m.get) ? el.value : string(),
				    m.mseq));
  } // deliver Lookup

  upcall deliver(const MaceKey& src, const MaceKey& dest, const LookupReply& m) {

    QuorumMap::iterator i = quorums.find(m.mseq);
    if (i == quorums.end()) {
      return;
    }

    QuorumState& qs = i->second;
    qs.q.add(src).seq = m.seq;

    // XXX - this is if put failed because of old sequence number
//     if (qs.type == QUORUM_PREPARE_PUT_TYPE) {
//       data.put(qs.key, m.value, m.seq);
//       quorums.erase(i);
//       return;
//     }

    if (m.get) {
      data.put(qs.key, m.value, m.seq);
    }
  
    if (qs.q.hasQuorum()) {
      bool success = tabulateQuorumResult(qs.q);
      if (!success) {
	// XXX - need to regenerate values somehow...
	assert(0);
      }

      if (qs.type == QUORUM_EXISTS_TYPE) {
	upcall_replicatedMapExistsResult(qs.key, success, qs.rid);
	quorums.erase(i);
      }
      else if (qs.type == QUORUM_GET_TYPE) {
	QuorumData& maxqd = qs.q.greatestScore<seq_id_t>();
	const VMap::Element& el = data.get(qs.key);
	if (maxqd.seq > el.version) {
	  downcall_route(maxqd.getId(), Lookup(qs.key, true, m.mseq));
	  // XXX - schedule a timer in case if this node fails
	}
	else {
	  upcall_replicatedMapGetResult(qs.key, el.value, success, qs.rid);
	  quorums.erase(i);
	}
      }
      else {
	assert(0);
      }
    }
  } // deliver LookupReply

  downcall erase(const std::string& key, registration_uid_t rid) {
    
  } // erase

  downcall put(const std::string& key, const std::string& value, bool overwrite,
	       registration_uid_t rid) {

    const VMap::Element& el = data.get(key);
    seq_id_t seq = el.contained ? el.version : 0;
    seq++;

    createQuorum(QUORUM_PREPARE_PUT_TYPE, key, value, overwrite, seq, rid);

    for (NodeSet::const_iterator i = servers.begin(); i != servers.end(); i++) {
      downcall_route(*i, PreparePut(key, value, seq, mseq));
    }
    // XXX - schedule timer to check for timeout
  } // put

  upcall deliver(const MaceKey& src, const MaceKey& dest, const PreparePut& m) {

    seq_id_t maxseq = 0;
    
    for (PreparedDataMap::const_iterator i = prepared.begin(); i != prepared.end(); i++) {
      const MaceKey& cursrc = i->first;
      const SeqPreparedDataMap& sp = i->second;
      for (SeqPreparedDataMap::const_iterator si = sp.begin(); si != sp.end(); si++) {
	const PreparedData& d = si->second;
	if ((d.key == m.key) && !((m.mseq == si->first) && (src == cursrc))) {
	  maxseq = std::max(maxseq, d.seq);
	}
      }
    }

    const VMap::Element& el = data.get(m.key);
    if (!el.contained || ((m.seq > maxseq) && (m.seq > el.version))) {
      PreparedData& p = prepared[src][m.mseq];
      p.key = m.key;
      p.value = m.value;
      p.seq = m.seq;
      p.timestamp = Util::timeu();
    }

    if (el.contained) {
      maxseq = std::max(maxseq, el.version);
    }

    downcall_route(src, PreparePutReply(maxseq, m.mseq));
    // XXX - check for timeout
  } // deliver PreparePut

  upcall deliver(const MaceKey& src, const MaceKey& dest, const CommitPut& m) {
    SeqPreparedDataMap& sm = prepared[src];
    SeqPreparedDataMap::iterator i = sm.find(m.mseq);
    if (i == sm.end()) {
      return;
    }
    else {
      const PreparedData& p = i->second;
      const VMap::Element& el = data.get(p.key);
      if (!el.contained || (el.version < p.seq)) {
	data.put(p.key, p.value, p.seq);
      }
      sm.erase(i);
    }
  } // deliver CommitPut

  upcall deliver(const MaceKey& src, const MaceKey& dest, const AbortPut& m) {
    prepared[src].erase(m.mseq);
  } // deliver AbortPut

  upcall deliver(const MaceKey& src, const MaceKey& dest, const PreparePutReply& m) {

    QuorumMap::iterator i = quorums.find(m.mseq);
    if (i == quorums.end()) {
      return;
    }

    QuorumState& qs = i->second;
    qs.q.add(src).seq = m.seq;

    if (qs.q.hasQuorum()) {
      QuorumData& maxqd = qs.q.greatestScore<seq_id_t>();
      if (maxqd.seq > qs.seq) {
	// XXX
	assert(0);
      }
      else {
	for (NodeSet::const_iterator i = servers.begin(); i != servers.end(); i++) {
	  downcall_route(*i, CommitPut(m.mseq));
	}
      }
      quorums.erase(i);
    }
  } // deliver PreparePutReply

  downcall get(const std::string& key, registration_uid_t rid) {
    createQuorum(QUORUM_GET_TYPE, key, rid);
    for (NodeSet::const_iterator i = servers.begin(); i != servers.end(); i++) {
      downcall_route(*i, Lookup(key, false, mseq));
    }
    // XXX schedule a timer to check if the quorum times-out
  } // get

  scheduler printer() {
    maceout << data << Log::endl;
  } // timer printer

} // transitions

routines {
  bool tabulateQuorumResult(const QuorumDataCollection& c) {
    size_t count = 0;
    for (QuorumDataCollection::const_iterator i = c.begin(); i != c.end(); i++) {
      if (i->second.seq != 0) {
	count++;
      }
    }
    return (count >= c.quorumSize());
  } // tabulateQuorumResult

  QuorumState& createQuorum(int type, const std::string& key, registration_uid_t rid) {
    mseq++;

    QuorumState& q = quorums[mseq];
    q.q.setServers(allServers);
    const VMap::Element& el = data.get(key);
    if (el.contained) {
      q.q.add(laddr).seq = el.version;
    }
    else {
      q.q.add(laddr).seq = 0;
    }
    q.rid = rid;
    q.key = key;
    q.type = QUORUM_EXISTS_TYPE;
    q.timestamp = Util::timeu();
    return q;
  } // createQuorum

  QuorumState& createQuorum(int type, const std::string& key, const std::string& value,
			    bool overwrite, seq_id_t seq, registration_uid_t rid) {
    QuorumState& q = createQuorum(type, key, rid);
    q.value = value;
    q.seq = seq;
    q.overwrite = overwrite;
    return q;
  } // createQuorum

} // routines
