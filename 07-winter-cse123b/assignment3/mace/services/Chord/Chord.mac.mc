/**
XXX: old comment
   Implementation of the Chord protocol
   Adolfo Rodriguez

   Notes:
   This implementation currently only supports:
   - 32-bit hash addresses
   - Fixed stabilize and fix-finger timeouts
   - Recursive forwarding
   
   Notes by Chip (4/25/06):
   - Naming of things largely based on SigComm paper, but protocol seems
     to be updated based on TON paper.

**/

using mace::MaceKeyDiff;

service Chord;

provides OverlayRouter, Overlay;

trace=med;

// time = MaceTime;

constants {
  int ADDRESS_FAMILY = SHA32;
  //   int ADDRESS_FAMILY = SHA160;
  int BITS = 32;  // Number of bits in the address
  //   int BITS = 160;  // Number of bits in the address
  uint64_t STABLE_TIMEOUT = 300*1000;  // stabilize timer
  uint64_t FIX_TIMEOUT = 1*1000*1000; // fix_fingers timer
  uint64_t JOIN_TIMEOUT = 10*1000*1000;  // initial join timer
  uint64_t RECOVERY_TIMEOUT = 60*1000*1000;  // initial join timer
  uint64_t PRINT_TIMEOUT = 2000000; // for debugging
}

states {
  joining;
  joined;
}

auto_types {
  finger __attribute((node())) {
    MaceKey hashId; //the node's hash id.
    MaceKey hashIdPlusOne __attribute((serialize(no), dump(no; state=no))) = MaceKey::null; //plus one, for range comparisons...
    //MaceKey start; //the beginning of the node's id space

    const MaceKey& getHashIdPlusOne() {
      if(hashIdPlusOne.isNullAddress()) {
        hashIdPlusOne = hashId + 1;
      }
      return hashIdPlusOne;
    }

    MaceKey getHashIdPlusOne() const {
      if(hashIdPlusOne.isNullAddress()) {
        return hashId + 1;
      }
      return hashIdPlusOne;
    }
  }
}

method_remappings {
  uses {
    downcall_route(const MaceKey&, const Message& -> const std::string&);
  }
  implements {
    upcalls {
      deliver(const MaceKey&, const MaceKey&, const Message& <- const std::string&);
    }
  }
}

services {
  Transport route = TcpTransport();
}

messages {
  //May wish to use the forwarding service for this message to make it "prettier"
  //used for joining, and for "fix fingers"
  find_pred {  // used to find the predecessor and successor of an ID (sometimes called find successor.  Here find predecessor, because it is responded to by the predecessor, with info about the successor
    MaceKey id;  // the hash in question (the one whose predecessor we want)
    MaceKey who; // who originated the query (IP)
  }
  //returns the successor of the id requested
  find_pred_reply {  // response to find_pred
    MaceKey predId;  // the hash of the predecessor
    MaceKey echoId;  // the hash finding the predecessor for.
    finger successor;  // my routing table
  }
  //used for stabilization
  get_pred { // queries a node for its predecessor and successor
  }	 
  get_pred_reply { // returns the requested information
    finger predecessor;
    finger successor; // XXX: Make this be a successor list.
  }
  update_pred { // instructs a node to potentially update its predecessor
    MaceKey id;  // hash of the sending node
  }
  remove { } //used to instruct a node to remove the sender from its routing tables.
}

state_variables {	 
  MaceKey source_;
  MaceKey me;
  MaceKey myhash;
  MaceKey myhashplusone;
  // disable failure detection for now
  //  fail_detect finger myfinger BITS;
  //  fail_detect finger predecessor;
  MaceKey fingerKeys[BITS]; //the hash id of the space the finger should manage
  finger myfinger[BITS]; // routing table
  finger predecessor; // predecessor entry
  finger successors_succ; // for error recovery
  timer stabilize __attribute((recur(STABLE_TIMEOUT)));
  timer fix_fingers __attribute((recur(FIX_TIMEOUT)));
  timer recovery __attribute((recur(RECOVERY_TIMEOUT)));
  timer printer __attribute((recur(PRINT_TIMEOUT)));
  timer join;
}	  

transitions {

  downcall (state == init) maceInit() {  // called to initialize protocol
    me = downcall_getLocalAddress();
    // derive bytes for sha from bamboo
    std::string buf;
    uint16_t p = htons(me.getMaceAddr().local.port);
    buf.append((const char*)&p, sizeof(p));
    char len = 4;
    buf.append((const char*)&len, sizeof(len));
    int i = me.getMaceAddr().local.addr;
    buf.append((const char*)&i, sizeof(i));
    maceout << "hashing " << Log::toHex(buf) << Log::endl;
    if(ADDRESS_FAMILY == SHA160) {
      myhash = MaceKey(sha160, buf);

      // myhash = MaceKey(sha160, me.addressString());
    } else if(ADDRESS_FAMILY == SHA32) {
      myhash = MaceKey(sha32, buf);

      // myhash = MaceKey(sha32, me.addressString());
    } else {
      printf("Exception: ADDRESS_FAMILY must either be SHA32 or SHA160\n");
      abort();
    }
    myhashplusone = myhash + 1;
    maceout << "myhash=" << myhash << Log::endl;
    ASSERT(!myhash.isNullAddress());

    //set up the fingerKeys
    MaceKeyDiff diff = myhashplusone - myhash; //diff = 1
    for(int i = 0; i < BITS; i++) {
      fingerKeys[i] = myhash + diff;
      diff = diff + diff; //doubling diff has the effect of creating the next biggest offset
    }

    predecessor = finger(me, myhash, myhashplusone);

    for (i=0; i<BITS; i++) {  // initializes all finger entries to me
      myfinger[i] = predecessor;
      //       neighbor_add(myfinger[i],me);
      //       myent = neighbor_entry(myfinger[i],me);
      //       myent->hashf = myhash;
      //       start = 1 << i; //(int)pow(2,i);  
      //       adjusted = start + myhash;
      //       myent->start = adjusted;  // the start address of this finger
    }
    //     printer.reschedule(PRINT_TIMEOUT); //for debugging
  }

  downcall (state == init) joinOverlay(const NodeSet& peerSet) {
    ASSERT(!peerSet.empty());
    source_ = *peerSet.begin();
    if (source_ == me) { // i am the bootstrap
      state = joined; // consider self joined
      stabilize.reschedule(STABLE_TIMEOUT); //schedule maintenance
      fix_fingers.reschedule(FIX_TIMEOUT);
    }
    else { // not the bootstrap
      state = joining;  // just joining
      downcall_route(source_, find_pred(myhash/*plusone*/, me));
      join.reschedule(JOIN_TIMEOUT); // my request could get lost
    }
    //     upcall_notify(predecessor,NBR_TYPE_PEER);
  }

  scheduler (state == joining) join() { // retry if my request was lost
    downcall_route(source_, find_pred(myhash/*plusone*/, me));
    join.reschedule(JOIN_TIMEOUT); // my request could get lost
  }

  scheduler (state == joined && source_ != me) recovery() {
    downcall_route(source_, find_pred(myhash, me));
  }

  upcall (state == joining && msg.who != src) deliver(const MaceKey& src, const MaceKey& dest, const find_pred& msg) {
    downcall_route(src, remove());
  }

  upcall (state == joined) deliver(const MaceKey& src, const MaceKey& dest, const find_pred& msg) {
    //consider using forward...
    MaceKey pred = nexthop(msg.id, true);
    if (pred == me) { // arrived at destination
      if (msg.who != me) { // i did not originate request
        downcall_route(msg.who, find_pred_reply(myhash, msg.id, myfinger[0]));
      }
      // Note that this node's successor field is updated via stabilize timers
      //      else { // update entries if needed
      //	register_knowledge(succ, succ_hash);
      //      }  Adolfo: Probably not needed.
      return;
    }
    // route the find_pred
    if(!pred.isNullAddress()) {
      downcall_route(pred, msg);
    }
  }

  upcall deliver(const MaceKey& src, const MaceKey& dest, const remove& m) {
    error(src, -1, "Asked to be removed", -1);
  }

  upcall (state == joining && !msg.predId.isNullAddress()) deliver(const MaceKey& src, const MaceKey& dest, const find_pred_reply& msg) {
    // my predecessor's old successor may suit me as route entry
    register_knowledge(msg.successor);
    // update p's old successor
    downcall_route(msg.successor.getId(), update_pred(myhash)); 
    ASSERT(!msg.predId.isNullAddress());
    predecessor = finger(src, msg.predId, MaceKey::null);
    register_knowledge(predecessor); //Chord8 -- add predecessor to state, instead of just as predecessor.
    state = joined; //now joined
    recovery.reschedule(RECOVERY_TIMEOUT); //for fixing partitions
    stabilize.reschedule(STABLE_TIMEOUT);// schedule maintenance
    fix_fingers.reschedule(FIX_TIMEOUT);
    //     upcall_notify(predecessor,NBR_TYPE_PEER);
  }

  scheduler (state == joined && getSuccessor().getId() != me) stabilize() { // stabilize timer
    // responsible for correcting predecessor and successor values
    downcall_route(myfinger[0].getId(), get_pred());  // query successor for predecessor
  }

  upcall (state == joined) deliver(const MaceKey& src, const MaceKey& dest, const get_pred& msg) {
    downcall_route(src, get_pred_reply(predecessor, myfinger[0])); // let querying node know whom my successor
    // and predecessor are
  }

  upcall (state == joined && src == getSuccessor().getId()) deliver(const MaceKey& src, const MaceKey& dest, const get_pred_reply& msg) {
    successors_succ = msg.successor;
    if (msg.predecessor.getId() == me) { //FYI: Used to check the hash, not the IP (CK)
      return;  // if he thinks i am his predecessor, we are done
    }

    if(!msg.predecessor.hashId.isNullAddress()) {
      register_knowledge(msg.predecessor); // update fingers
    }
    // successor has likely changed: either old successor doesn't likely know of me yet, or my successor has changed.
    downcall_route(myfinger[0].getId(), update_pred(myhash)); // let him know of me
  }

  // check to see if sending node should be our predecessor
  upcall (state == joined && KeyRange::containsKey(msg.id, predecessor.getHashIdPlusOne(), myhash)) deliver(const MaceKey& src, const MaceKey& dest, const update_pred& msg) {
    maceout << "change predecessor: old " << predecessor << " ";
    finger f(src, msg.id);
    predecessor = f; //Chord2
    register_knowledge(f); /* Broken initially -- based on update to TON protocol */
    maceout << "new " << predecessor << Log::endl;
    //       upcall_notify(predecessor,NBR_TYPE_PEER);
  }

  scheduler (state == joined && !(predecessor.getId() == me || getSuccessor().getId() == me)) fix_fingers() {
    int myrand = randint(BITS); // select a random finger entry
    MaceKey pred = nexthop(fingerKeys[myrand], true);

    EXPECT(pred != me);
    if(pred != me && ! pred.isNullAddress()) {
      downcall_route(pred, find_pred(fingerKeys[myrand], me));
    }
  }

  upcall (state == joined) deliver(const MaceKey& src, const MaceKey& dest, const find_pred_reply& msg) {
    //Moved for Chord13
    if(!msg.predId.isNullAddress() && KeyRange::containsKey(msg.predId, predecessor.getHashIdPlusOne(), myhash/*Chord14: plusone*/)) { //Chord9
      predecessor = finger(src, msg.predId, MaceKey::null);
      register_knowledge(predecessor); //Chord11
    }
    if(msg.echoId == myhash) {
      if(msg.successor.hashId == myhash) {
        return; // All's well.  Did a lookup for my key, and found myself.
      } else {
        downcall_route(src, find_pred_reply(MaceKey::null,myhash,finger(me, myhash)));
      }
    }
    else if(KeyRange::containsKey(msg.successor.hashId, predecessor.getHashIdPlusOne(), myhash/*Chord14: plusone*/)) { //Chord9
      predecessor = msg.successor;
    }
    // when attempting to fix my finger table, I will get replies from nodes
    // to consider
    register_knowledge(msg.successor); // update fingers
  }

  scheduler printer() {
    dumpState(maceout);
  }

  upcall (true) error(const MaceKey& nodeId, TransportError::type error_code, const std::string& m, registration_uid_t registrationUid) {
    int loopbit;
    maceout << "Neighbor " << nodeId << " died! " << m << Log::endl;
    if(0) {
      //believed broken code!!!

      if (nodeId == predecessor.getId()) { // predecessor died
        predecessor = myfinger[BITS-1];
        //       upcall_notify(predecessor,NBR_TYPE_PEER);
      }

      for (loopbit = BITS-1; loopbit>=1; loopbit--) {
        if (nodeId == myfinger[loopbit].getId()) {
          myfinger[loopbit] = myfinger[loopbit-1];
        }
      }
      if (nodeId == myfinger[0].getId()) { // successor died
        if(!successors_succ.getId().isNullAddress()) {
          myfinger[0] = successors_succ;
        }
        else {
          myfinger[0] = myfinger[1];
        }
      }
    }
    //Instead, I think the code should be:
    else {
      bool registerPredecessor = true;
      if(predecessor.getId() == nodeId) {
        predecessor = finger();
        registerPredecessor = false;
      }
      if(myfinger[BITS-1].getId() == nodeId) {
        myfinger[BITS-1] = finger(me, myhash, myhashplusone);
      } else if (predecessor.getId().isNullAddress() && myfinger[BITS-1].getId() != me) {
        predecessor = myfinger[BITS-1];
      }
      for (loopbit = BITS-2; loopbit>=0; loopbit--) {
        if (nodeId == myfinger[loopbit].getId()) {
          myfinger[loopbit] = myfinger[loopbit+1];
        } else if (predecessor.getId().isNullAddress() && myfinger[loopbit].getId() != me) {
          predecessor = myfinger[loopbit];
        }
      }
      if(registerPredecessor) {
        register_knowledge(predecessor);
      }
      if(successors_succ.getId() == nodeId) {
        successors_succ = finger();
      } else if(!successors_succ.hashId.isNullAddress()) {
        register_knowledge(successors_succ);
        if(predecessor.hashId.isNullAddress()) {
          predecessor = successors_succ;
        }
      }
      if(predecessor.hashId.isNullAddress()) {
        predecessor = finger(me, myhash, myhashplusone);
      }
    }
  }

  downcall getNextHop(const MaceKey& id) {
    if(state == joined) {
      return nexthop(id, false);
    }
    else {
      return me;
    }
  }

  downcall getNextHop(const MaceKey& id, MaceKey& ovId) {
    if(state == joined) {
      return nexthop(id, false, &ovId);
    }
    else {
      ovId = myhash;
      return me;
    }
  }
}

routines {
  MaceKey nexthop (const MaceKey& target, bool strictlyPred = false, MaceKey* ovId = NULL) const {
    //Rem: KeyRange::containsKey is left inclusive, right exclusive -- so the need for plusone
    if(KeyRange::containsKey(target, predecessor.getHashIdPlusOne(), myhashplusone)) {
      //If its in my range, then I'm the destination
      if(ovId != NULL) { *ovId = myhash; }
      if(strictlyPred) {
        if(!predecessor.getId().isNullAddress()) {
          return predecessor.getId();
        } else {
          for(int i = BITS-1; i >= 0; i--) {
            if(myfinger[i].getId() != me) {
              return myfinger[i].getId();
            }
          }
        }
        return MaceKey::null;
      }
      //Note -- this case is kinda funny for Chord -- which is really a node location service not a routing service...
      return me;
    }
    else if(KeyRange::containsKey(target, myhashplusone, myfinger[0].getHashIdPlusOne())) {
      if(ovId != NULL) { *ovId = myfinger[0].hashId; }
      if(strictlyPred) {
        return me;
      }
      return myfinger[0].getId();
    }
  
    //else it must be in our finger table somewhere.
    KeyRange keyRange(myhashplusone, target); //Note: using target gives us strictly a predecessor, since an exact match would not match due to right exclusivity.  What's the right thing to do?  Chord status will nearly require that the predecessor be found, but still...
    //NOTE: MUCH Simpler than prior MACEDON code.
    for(int i = BITS-1; i >= 0; i--) {
      if(keyRange.containsKey(myfinger[i].hashId)) {
        if(ovId) { *ovId = myfinger[i].hashId; }
        return myfinger[i].getId();
      }
    }
    ASSERT(false);
    if(ovId) { *ovId = myhash; }
    return me;
  }

  int register_knowledge (const finger& peer) {
    // updates the appropriate finger table entr(ies)
    // returns 1 if an update occurred
    bool change=false;
    KeyRange keyRange(myhashplusone, peer.getHashIdPlusOne());
    for (int loopbit = 0; loopbit < BITS && keyRange.containsKey(fingerKeys[loopbit]); loopbit++) {
      if(KeyRange::containsKey(peer.hashId, fingerKeys[loopbit], myfinger[loopbit].hashId)) { //Found while looking for Chord14, don't replace a node with itself. (was plusone())
        //then peer.hashId is a better match than myfinger[loopbit]
        maceout << "change finger " << loopbit << ": old " << myfinger[loopbit];
        myfinger[loopbit] = peer;
        maceout << " new " << myfinger[loopbit] << Log::endl;
        change = true;
      }
    }
    return change;
  } 

  const finger& getSuccessor() const {
    return myfinger[0];
  }
}

properties
{
  safety {
    predecessorNonNullSafety: \forall n \in \nodes: { n.state = init \or \not n.predecessor.hashId.isNullAddress() }; //Take away when move to "current"
    successorAndPredecessor: \forall n \in \nodes: { n.predecessor.getId() = n.me \iff n.getSuccessor().getId() = n.me }; //pred = me if and only if succ = me
  }
  liveness {
    nodesJoined : \forall n \in \nodes : { n.state = joined };
    allNodes : \forall n \in \nodes : { n.(getSuccessor().getId())* \eq \nodes };
    predecessorNonNull: \forall n \in \nodes : \not n.predecessor.getId().isNullAddress();
    successorReflexive : \forall n \in \nodes : n.getSuccessor().getId().predecessor.getId() = n;
    allNodesReachable : \forall n \in \nodes : \forall m \in \nodes : { m \in n.(getNextHop(m.myhash, -1))* };
    //     logPath : \forall n \in \nodes : \forall m \in \nodes : { |n.(getNextHop(m.myhash, -1))*| \leq MAX_ROUTE_DIST };
  }
}
