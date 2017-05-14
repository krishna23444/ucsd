/* 
 * RandTree.m : part of the Mace toolkit for building distributed systems
 * 
 * Copyright (c) 2006, James W. Anderson, John Fisher-Ogden, Ryan Braud, Charles Killian
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
#include "lib/mstring.h"
#include "lib/Log.h"
#include "lib/MaceTypes.h"
#include "lib/Iterator.h"

using mace::string;
using std::ostringstream;

service RandTree;

provides Tree, Overlay;
// trace=med;


constants {       //The set of [global]  constants available to the protocol.
  uint RANDTREE_MAX_CHILDREN = 12;
  //   uint RANDTREE_MAX_CHILDREN = 2;
  uint64_t RANDTREE_JOIN_TIMEOUT = 5 * 1000 * 1000;
  uint64_t RANDTREE_RECOVERY_TIMEOUT = 10 * 1000 * 1000 * 60;
  uint64_t DEBUG_INTERVAL = 10 * 1000 * 1000;

  int JOIN_ACCEPTED = 0;
  int JOIN_UNREACHABLE = 1;
  int JOIN_UNAUTHORIZED = 2;
  int JOIN_ROOT_FORCE = 3;
  int JOIN_LOOP_ERROR = 4;
}

services {
  Transport control_ = TcpTransport();
}

constructor_parameters {
  uint MAX_CHILDREN = RANDTREE_MAX_CHILDREN;
}

states {
  joining;
  joined;
}

auto_types {
}

typedefs {
  //   typedef NodeCollection<child, RANDTREE_MAX_CHILDREN> children;
  typedef NodeSet::iterator NodeSetIter;
  typedef mace::hash_set<registration_uid_t> HandlerSet;
}

method_remappings {
  uses {
    upcall_verifyJoinOverlay(const MaceKey&, registration_uid_t regId = authoritativeJoinHandler); 
  }
}

messages {
  Join {
    MaceKey from;
    MaceKey redirectedBy;
  }

  JoinReply {
    int overlayStatus;
    MaceKey root;
    NodeSet siblings;
  }

  remove {
  }

  rejoin {
  }

  update_siblings {
    MaceKey root;
    NodeSet siblings;
  }

  probe {
    MaceKey root;
  }

  probe_join {
    MaceKey root;
  }

  new_root {
    MaceKey root;
  }
}

state_variables {    
  MaceKey bootstrap;
  //   MaceKey root __attribute((onChange(function=upcallAllVoid(notifyIsRootChanged, MaceKey(), root==meIPV4))));
  MaceKey root;
  MaceKey parent;
  // MaceKey parent __attribute((fail_detect(error=parentFailed)));
  // MaceKey parent __attribute((fail_detect(error=parentFailed; suppression_window=5 SEC; retry_interval=1 SEC; fail_timeout=10 SEC; probe=yes|no; message=probe(root);)));

  MaceKey lastProbed;

  NodeSet children;
  // NodeSet children __attribute((fail_detect(error=childFailed)));
  NodeSet siblings;
  NodeSet peers;
  // other type is concurrent
  // NodeSet peers __attribute((auto_message(message=probe(root); type=sequential; send_interval=5 SEC; state=joined; filter=!(children.contains(i) || i == parent || i == root); transport=_control)));

  timer join;
  timer recovery __attribute((recur(RANDTREE_RECOVERY_TIMEOUT)));
  timer printer __attribute((recur(DEBUG_INTERVAL)));

  registration_uid_t authoritativeJoinHandler;

  MaceKey meIPV4;
}     

transitions {
  aspect<root> whenRootChanges(oldroot) {
    if( (oldroot==meIPV4) != (root==meIPV4) ) {
      upcallAllVoid(notifyIsRootChanged, MaceKey::null, root==meIPV4);
    }
  }

  downcall (state == init) maceInit() {
    meIPV4 = downcall_getLocalAddress();
    root = meIPV4;
    parent = MaceKey::null;
    lastProbed = MaceKey::null;
    authoritativeJoinHandler = -1;
    printer.reschedule(DEBUG_INTERVAL);
  } // maceInit

  downcall isMember(const MaceKey& groupId) {
    return (state != init);
  }

  scheduler (state == joining) join() {
    // Timer fired, retry join
    notifyJoinResult(bootstrap, JOIN_UNREACHABLE);
    if (true) {
      root = meIPV4; //possible fix for RT15

      if (!peers.empty()) {
        do {
          maceout << "finding next in peers " << bootstrap << Log::endl;
          bootstrap = findNextInSet(peers, bootstrap);
          maceout << "checking bootstrap " << bootstrap << Log::endl;
        } while (children.find(bootstrap) != children.end());
      }

      maceout << "attempting to join " << bootstrap.toString() << Log::endl;
      downcall_route(bootstrap, Join(meIPV4, MaceKey()));
      join.reschedule(RANDTREE_JOIN_TIMEOUT);
    }
    else {
      join.cancel();
      state = init;
    }
  } // joining timer join

  upcall (state == joining || state == joined) 
    deliver(const MaceKey& source, const MaceKey& destination, 
	    const Join& msg) {
    maceout<< "received join request from: " 
					<< msg.from << Log::endl;

    if ((root != meIPV4) && (msg.from != root) && (msg.redirectedBy != root)) {
      maceout<< "redirecting join msg from " << msg.from 
        << " to root " << root << Log::endl;
      downcall_route(root, Join(msg.from, MaceKey()));
      return;
    }

    if (msg.from == root) {
      maceout<< "root (" << root 
        << ") tried to join a descendant, tell root he's the root" << Log::endl;
      downcall_route(msg.from, JoinReply(JOIN_LOOP_ERROR, root, NodeSet()));
      return;
    }

    if ((msg.from < meIPV4) && (root == meIPV4)) {
      if (upcall_verifyJoinOverlay(msg.from)) {
        ASSERT(parent.isNullAddress());
        // we thought we were the root, but the node that tried to
        // join with us should be
        maceout<< "received join request from " << msg.from << " < me (" << meIPV4 << ") "
            << "attempting to join" << Log::endl;
        downcall_route(msg.from, JoinReply(JOIN_ROOT_FORCE, msg.from, NodeSet()));
        state = joining;
        peers.insert(msg.from); //XXX Temp fix for bug 1 -- add node to peer set.
        bootstrap = msg.from;
        // this could cause multiple join replies if we are currently joining

        maceout<< meIPV4 << " sending join msg to " << msg.from << Log::endl;
        downcall_route(msg.from, Join(meIPV4, MaceKey()));
        join.reschedule(RANDTREE_JOIN_TIMEOUT);
        return;
      }
      else {
        maceout<< "new parent " << msg.from << " not verified" << Log::endl;
        downcall_route(msg.from, JoinReply(JOIN_UNAUTHORIZED, MaceKey(), NodeSet()));
      }
    }

    ASSERT(msg.from > root);

    if ((!children.contains(msg.from)) &&
        (children.size() < MAX_CHILDREN)) {
      if (upcall_verifyJoinOverlay(msg.from)) {
        children.insert(msg.from);
        maceout<< meIPV4 << " accepting " << msg.from 
          << " as child, root=" << root << Log::endl;
        downcall_route(msg.from, JoinReply(JOIN_ACCEPTED, root, children));

        upcallAllVoid(notifyChildren, MaceKey()/*gId ignored*/, children);
        upcallAllVoid(peerJoinedOverlay, msg.from);

        if (root == meIPV4) {
          maceout<< meIPV4 << " updating children, I'm root" << Log::endl;
          //NOTE: This is to handle the case where root does joinResult because just discovered am root.
          notifyJoinResult(source, JOIN_ACCEPTED);
          state = joined;
          join.cancel();
          updateChildSiblings();
        }
      }
      else {
        maceout<< "child " << msg.from << " not verified" << Log::endl;
        downcall_route(msg.from, JoinReply(JOIN_UNAUTHORIZED, MaceKey(), NodeSet()));
      }
    }
    else if (children.find(msg.from) != children.end()) {
      // already our child, do nothing
      maceout<< "!!!received Join from " << msg.from << ", "
          << "already child" << Log::endl;
    }
    else {   // choose random child to redirect join
      int pos = randint(children.size());
      MaceKey c = getChildIndex(pos);

      maceout<< "Redirecting from=" << msg.from << " to " << c << Log::endl;
      ASSERT(root == meIPV4 || msg.redirectedBy == root);
      downcall_route(c, Join(msg.from, root));
    }
  } // joined recv Join

  
  upcall (state == joining) 
    deliver(const MaceKey& source, const MaceKey& destination, 
	    const JoinReply& msg) {
    maceout
      << "JoinReply from: " 
      << source << ", root=" << root 
      << " status=" << getStatus(msg.overlayStatus) << Log::endl;
    
    switch (msg.overlayStatus) {

    case JOIN_ACCEPTED:
      ASSERT(parent.isNullAddress());
      root = msg.root;
      siblings = msg.siblings;
      parent = source;

      notifyJoinResult(source, JOIN_ACCEPTED);
      upcallAllVoid(notifyParent, source /*gId ignored*/, source);

      state = joined;
      join.cancel();
      multicastNewRoot();
      updateChildSiblings();

      break;
    
    case JOIN_LOOP_ERROR:
//       ASSERT(0);
      ASSERT(parent.isNullAddress());
      ASSERT(root == meIPV4);
      notifyJoinResult(source, JOIN_ACCEPTED);
      state = joined;
      join.cancel();
      break;

    case JOIN_ROOT_FORCE:
      // we are now the root
      ASSERT(parent.isNullAddress());
      ASSERT(root == msg.root);
      maceout << source << " told me (" << meIPV4 << ") to be the root" << Log::endl;
      notifyJoinResult(source, JOIN_ACCEPTED);
      state = joined;
      join.cancel();	
      break;
      
    case JOIN_UNAUTHORIZED:
	maceout << "authentication failed trying to join " << source << Log::endl;
        notifyJoinResult(source, JOIN_UNAUTHORIZED);
	break;
    case JOIN_UNREACHABLE:
      // no one should send a JoinReply with unreachable status since
      // they're unreachable so how are they sending the JoinReply
      ASSERT(0);
      break;
    default:
      ASSERT(0);
    }
  } // joining recv JoinReply

  
  //   upcall (state == init) deliver(const MaceKey& source, 
  // 				     const MaceKey& destination, 
  // 				     const JoinReply& msg, 
  // 				     comm_type_t communicationType) {
  //     macewarn << "received early JoinReply from: " << source << Log::endl;

  //     if (msg.overlayStatus == JOIN_ACCEPTED) {
//  //   tell the sender to remove us
    //       downcall_route(source, remove());
    //     }
    //   } // joined recv JoinReply

  upcall (state == joined) deliver(const MaceKey& source, 
				     const MaceKey& destination, 
				     const JoinReply& msg) {
    maceout << "received duplicate JoinReply from: " << source << Log::endl;

    if (msg.overlayStatus == JOIN_ACCEPTED) {
      if (parent != source) {
	// tell the sender to remove us
	downcall_route(source, remove());
      }
    }
  } // joined recv JoinReply

  downcall (true) setAuthoritativeJoinHandler(registration_uid_t regUid) {
    //authoritativeJoinHandler = handlerUid;
    authoritativeJoinHandler = regUid;
  }


  upcall (children.contains(source)) deliver(const MaceKey& source, const MaceKey& destination,  //Fix for RT10 - used to be state == joined
				     const remove& msg) {
    // child is requesting to be removed
    NodeSet::iterator i = children.find(source);
    if (i != children.end()) {
      maceout << source << " asked to be removed" << Log::endl;

      children.erase(source);
      notifyChildChange();

      if (root == meIPV4) {
	updateChildSiblings();
      }
    }
    else {
      ASSERT(0);
    }
  } // joined recv remove

  
  upcall (state == joined) deliver(const MaceKey& source, const MaceKey& destination, 
				     const update_siblings& msg) {
    if (source == parent) {
      root = msg.root;
      siblings = msg.siblings;
    }
    else {
      maceout << "got a message from " << source << " my parent is " 
        << parent << Log::endl;
//       ASSERT(0);
    }
  } // recv update_peers


/* NetworkErrorHandler::error */
  upcall (true) error(const MaceKey& nodeId, TransportError::type error_code,
		      const std::string& message, registration_uid_t h) { 

    maceerr << "received error " << error_code << " for " << nodeId << Log::endl;

    ASSERT(error_code != TransportError::NO_ERROR);

    NodeSet::iterator i = children.find(nodeId);
    if (i != children.end()) {
      maceerr << "child " << nodeId << " died" << Log::endl;
      children.erase(nodeId);
      if (root == meIPV4) {
	updateChildSiblings();
      }
      notifyChildChange();
      return;
    }
      
    if (nodeId == root && nodeId == parent) {
      // root failover
      maceerr << "parent (root) " << parent << " died, attempting to join peer..."
	      << Log::endl;

      MaceKey d = meIPV4;
      for (NodeSet::iterator i = siblings.begin(); i != siblings.end(); i++) {
	d = std::min(d, *i);
      }

      if (d == meIPV4) {
	root = meIPV4;
	multicastNewRoot();
	maceerr << "I (" << meIPV4 << ") am now the root" << Log::endl;
	siblings.clear();
	notifyParentFailed();
	parent = MaceKey();
	notifyParent();
      }
      else {
	state = joining;
	peers.insert(d);
	bootstrap = d;
	maceerr << "attempting to join " << bootstrap << Log::endl;
	notifyParentFailed();
	parent = MaceKey();
	downcall_route(bootstrap, Join(meIPV4, MaceKey()));
	join.reschedule(RANDTREE_JOIN_TIMEOUT);
      }
      root = meIPV4;
    }
    else if (nodeId == parent) {
      maceerr << "parent " << parent << " died, attempting to join root " 
	      << root << Log::endl;

      state = joining;
      notifyParentFailed();
      parent = MaceKey();

      peers.insert(root);
      bootstrap = root;
      downcall_route(bootstrap, Join(meIPV4, MaceKey()));
      join.reschedule(RANDTREE_JOIN_TIMEOUT);
      root = meIPV4;
    }
    return;
  } // API error

  
  scheduler (state == joined) recovery() {
    ASSERT(!peers.empty());
//     maceout << "finding next in peers, " << lastProbed << Log::endl;
    uint count = 0;
    lastProbed = findNextInSet(peers, lastProbed);
    while ((count < peers.size()) &&
	   ((children.find(lastProbed) != children.end()) ||
	    (lastProbed == parent) ||
	    (lastProbed == root))) {
      lastProbed = findNextInSet(peers, lastProbed);
      count++;
    }

    if ((children.find(lastProbed) == children.end()) &&
	(lastProbed != parent) &&
	(lastProbed != root)) {
      maceout << "sending probe with root " << root << " to " << lastProbed
        << Log::endl;
      downcall_route(lastProbed, probe(root));
    }

  } // timer recovery

  
  upcall (state != init) deliver(const MaceKey& source, const MaceKey& destination, 
			  const probe& msg) { 
    if (root < msg.root) 
    {
      maceout << "sending probe_join with my root " << root << " to " 
        << msg.root << Log::endl;
      downcall_route(msg.root, probe_join(root));
    }
    else if (root > msg.root) 
    {
      if (root == meIPV4) 
      {
	state = joining;
        peers.insert(msg.root);
	bootstrap = msg.root;
	maceout << "joining root " << msg.root << " < me (" << meIPV4 << ")" << Log::endl;
	downcall_route(bootstrap, Join(meIPV4, MaceKey()));
	join.reschedule(RANDTREE_JOIN_TIMEOUT);
	return;
      }

      maceout << "sending probe_join to my root " << root << " with " << msg.root << Log::endl;
      downcall_route(root, probe_join(msg.root));
    }
  } // recv probe


  upcall (state != init) deliver(const MaceKey& source, const MaceKey& destination, 
			  const probe_join& msg) { 
    if ((root != meIPV4) || (root < msg.root)) {
      return;
    }

    ASSERT(parent.isNullAddress());

    state = joining;

    peers.insert(msg.root);
    bootstrap = msg.root;
    maceout << meIPV4 << " sending join request to "
      << msg.root << Log::endl;
    downcall_route(bootstrap, Join(meIPV4, MaceKey()));
    join.reschedule(RANDTREE_JOIN_TIMEOUT);
  } // probe_join


  //XXX Need to check the sender!
  //   upcall (true) deliver(const MaceKey& source, const MaceKey& destination, 
  upcall (source == parent) deliver(const MaceKey& source, const MaceKey& destination, 
			  const new_root& msg) { 
    root = msg.root;
    multicastNewRoot();
  } // recv new_root
  
//   API create_group {
//     // For now, do nothing. In a real protocol, we would register the session.
//   }

  downcall (state == init) joinOverlay(const NodeSet& peerSet, 
				       registration_uid_t rid) {
    peers = peerSet;
    //       ASSERT(!peers.empty());
    parent = MaceKey();
    peers.erase(meIPV4);
    if (peers.empty()) {
      state = joined;
      maceout << "joinOverlay called and there were no peers other than me " << Log::endl;
    }
    else {
      state = joining;

      bootstrap = *(peers.begin());
      lastProbed = bootstrap;

      maceout << "attempting to join " << bootstrap << Log::endl;
      downcall_route(bootstrap, Join(meIPV4, MaceKey()));
      join.reschedule(RANDTREE_JOIN_TIMEOUT);
      recovery.reschedule(RANDTREE_RECOVERY_TIMEOUT);
    }
  }


  downcall (state == joining || state == joined) 
    joinOverlay(const NodeSet& peerSet, registration_uid_t rid) {
      bool empty = peers.empty();

      peers.insert(peerSet.begin(), peerSet.end());
      peers.erase(meIPV4);

      if(empty && !peers.empty()) {
        // If we didn't have any peers before, but we have some now,
        // we have to start the join timer
        bootstrap = *(peers.begin());
        lastProbed = bootstrap;

        maceout << "attempting to join " 
          << bootstrap << Log::endl;
        downcall_route(bootstrap, Join(meIPV4, MaceKey()));
        join.reschedule(RANDTREE_JOIN_TIMEOUT);
        recovery.reschedule(RANDTREE_RECOVERY_TIMEOUT);
        return;
      }

      //XXX: Implement maceDefer
      //       maceDefer(true, upcall_joinResultOverlay, meIPV4, 0/*JOIN_SUCCESS*/, handlerUid); //true refers to the condition which allows this to fire
      upcall_joinResultOverlay(meIPV4, JOIN_ACCEPTED, rid); //true refers to the condition which allows this to fire
      //XXX: notify children
  }


  downcall (true) getOverlayJoinStatus() {
    switch(state) {
      case joining: return JOINING;
      case joined: return JOINED;
      default: return NONE;
    }
  }


  downcall (true) isRoot(const MaceKey& groupId) {
    return (root == meIPV4);
  }

  //   API leave {
  //     // For now, ignore it. 
  //   }

  
  upcall (state == joined) deliver(const MaceKey& source, const MaceKey& destination, 
				     const rejoin& msg) {
    if (parent == source) {
      maceout << "received rejoin from " << source << "!" << Log::endl;
      parent = MaceKey();
      state = joining; 
      // retry joining at the root
      ASSERT(!root.isNullAddress());
      ASSERT(parent != root);
      peers.insert(root);
      bootstrap = root;
      downcall_route(root, Join(meIPV4, MaceKey()));
      join.reschedule(RANDTREE_JOIN_TIMEOUT);
    }
  } // joined recv rejoin

  
  scheduler (true) printer() {
    ostringstream os;
    for (NodeSet::iterator i = children.begin(); i != children.end(); i++) {
      os << *i << " ";
    }
    string cs = os.str();
    os.str("");
    for (NodeSet::iterator i = siblings.begin(); i != siblings.end(); i++) {
      os << *i << " ";
    }
    string ss = os.str();
    os.str("");
    for (NodeSet::iterator i = peers.begin(); i != peers.end(); i++) {
      os << *i << " ";
    }
    string ps = os.str();
    os.str("");
    
    maceout
      << "parent=" << parent << " bootstrap="  << bootstrap << " root=" << root
      //       << std::endl << "  children: " << cs
      << "\n  children: " << cs
      << "\tsiblings: " << ss
      << "\tpeers: " << ps << Log::endl;
  } // timer printer

  downcall getParent(const MaceKey& channelId) {
    return (root == meIPV4? meIPV4: parent); 
  }
  downcall getChildren(const MaceKey& channelId) {
    return children;
  }
  downcall getLocalAddress() { return meIPV4; }
} // transitions

routines {  

  void multicastNewRoot() {
    for (NodeSet::iterator i = children.begin(); i != children.end(); i++) {
      downcall_route(*i, new_root(root));
    }
  } // multicastNewRoot
  
  MaceKey findNextInSet(const NodeSet& s, MaceKey k) {
    NodeSet::const_iterator i = s.find(k);
    if (i == s.end()) {
      return *(s.begin());
    }
//     ASSERT(i != s.end());
    i++;
    if (i == s.end()) {
      return *(s.begin());
    }
    else {
      return *i;
    }
  } // findNextInSet

  MaceKey getChildIndex(uint pos) {
    MaceKey c = MaceKey();
    uint i = 0;
    for (NodeSet::iterator it = children.begin(); it != children.end(); it++) {
      if (i == pos) {
	c = *it;
	break;
      }
      i++;
    }
    return c;
  } // getChildIndex

  const char* getStatus(int jr) {
    static char* arr[] = { "JOIN_ACCEPTED", "JOIN_UNREACHABLE", "JOIN_UNAUTHORIZED",
		     "JOIN_ROOT_FORCE", "JOIN_LOOP_ERROR", ""};
    if (jr < 0 || jr > 4) {
      return arr[5];
    }
    else {
      return arr[jr];
    }
  }

  void updateChildSiblings() {
    for (NodeSet::iterator i = children.begin(); i != children.end(); i++) {
      downcall_route(*i, update_siblings(root, children));
    }
  } // updateChildSiblings

  //     Alternately, we can provide a new mechanism for notifying handlers
  void notifyChildChange() {
    upcallAllVoid(notifyChildren, MaceKey(), children);
  } // notifyChildChange

  void notifyJoinResult(MaceKey from, int jr) {
    //NOTE: This is used at the root -- all registered handlers are marked verified when I am root.
    upcallAllVoid(joinResultOverlay, from, jr);
    upcallAllVoid(notifyIsMemberChanged, MaceKey::null, true);
  } // notifyJoinResult

  void notifyParent() {
    upcallAllVoid(notifyParent, MaceKey(), parent);
  } // notifyParent

  void notifyParentFailed() {
    maceout << "making upcalls that " << parent << " failed" << Log::endl;
    upcallAllVoid(notifyParentFailed, MaceKey(), parent);
  } // notifyParentFailed

} // routines

properties {
  safety {
    meIPV4 : \forall n \in \nodes : n.state \neq init \implies n.meIPV4 = n;
    joinedOrNullParent : \forall n \in \nodes : { n.state = joined
                         \implies (n.root = n \or n.parent \neq \null) };
    noLoops : \forall n \in \nodes : { \exists m \in n.(parent)* : { m.root = m \or m.state \neq joined } };
                         //     timers : \forall n \in \nodes : { (n.state = init) \xor ( (n.join.nextScheduled() \neq 0) \xor (n.recovery.nextScheduled() \neq 0) ) };
    timers : \forall n \in \nodes : { (n.state = init) \or (n.recovery.nextScheduled() \neq 0)  };
  }
  liveness {
    allJoined : \forall n \in \nodes : (n.state = joined);
    oneRoot : \for{=}{1} n \in \nodes : (n.root = n);
    kidsMatch : \forall n \in \nodes :
              {
                (n.root = n \or n \in n.parent.children)
                  \and (\forall m \in n.children : n = m.parent)
              };
    //Inefficient
    rootMatch : \for{=}{1} n \in \nodes : { \forall m \in \nodes : n = m.root };
    siblingsMatch : \forall n \in \nodes : { n.root \neq n.parent \or \forall m \in n.siblings : { n \in m.siblings \and n.parent = m.parent } };
  }
}
