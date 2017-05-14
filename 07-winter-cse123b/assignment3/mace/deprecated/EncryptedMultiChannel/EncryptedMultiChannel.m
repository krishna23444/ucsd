/* 
 * EncryptedMultiChannel.m : part of the Mace toolkit for building distributed systems
 * 
 * Copyright (c) 2005, James W. Anderson, Charles Killian
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
#include <openssl/evp.h>

using std::endl;
using mace::string;

provides Tree, HierarchicalMulticast, Transport, Overlay;

services {
  Transport ro_ =TCP();
  Tree tr_ =RandTree(ro_);
  HierarchicalMulticast mc_ =GenericTreeMulticast(ro_, tr_);
  Crypto cr_ =Crypto();
} // services

states {
  preJoining unready;
  joining unready;
  key_received unready;
  waiting_for_key unready;
  ready ready;
} // states

messages {
  key_msg {
    string encSymKey;
    string encIV;
    string encSymKeyHash;
  }

  data {
    registration_uid_t rid; //This is the handler we deliver the message to.
    string payload __attribute((dump(no)));
  }
} // messeages

state_variables {
  MaceKey source_;
  string myIdentity;
} // state_variables

method_remappings {
  uses {
    downcall_route(const MaceKey&, const Message& -> const std::string&);
    downcall_routeDirect(const MaceKey&, const Message& -> const std::string&);
    downcall_distribute(const MaceKey&, const Message& -> const string&);
    downcall_collect(const MaceKey&, const Message& -> const string&);
    downcall_anycast(const MaceKey&, const Message& -> const string&);
    downcall_multicast(const MaceKey&, const Message& -> const string&);
  } // uses

  implements {
    upcalls {
      deliver(const MaceKey&, const MaceKey&, const Message& <- const std::string&);
    } // upcalls
  } // implements
} // method_remappings

transitions {

  downcall (state == init) maceInit() {
    OpenSSL_add_all_digests(); //Did this go here?
    ASSERT(Params::Instance()->isset("key1fp") && Params::Instance()->isset("key2fp"));
    ASSERT(downcall_loadKeys(Params::Instance()->getstr("key1fp"),
			     Params::Instance()->getstr("key2fp")));
    myIdentity = string("");
    string pubkey = downcall_getPubKey();
    string sig = downcall_sign(pubkey);
    pubkey.serialize(myIdentity);
    sig.serialize(myIdentity);
    //This is to prevent multiple maceInit calls.  preJoining is to
    //wait for the joinOverlay downcall.
    state_change(preJoining);
    downcall_setActiveAuthenticator();
  } // maceInit

  /*   !ready API verifyJoinOverlay { */
  /*     //NOTE: wouldn't it be nice if we could avoid defining this?  The reason it's here is that I'm not certain the */
  /*     //      default return value would work. */
  /*     //UPDATE: Indeed, the default return value can work -- however, the default is actually to verify join requests. */
  /*     return false; */
  /*   } */

  //TODO: Is it okay to verify joins before I am "ready"
  /*ready*/ 
  upcall verifyJoinOverlay(const MaceKey& source, const std::string& ident) {
    string theirpub;
    string theirsig;
    string mypub = downcall_getPubKey();

    istringstream in(ident);
    theirpub.deserialize(in);
    theirsig.deserialize(in);
    bool r = (downcall_verifySignature(theirpub, theirsig) && (mypub == theirpub));
    if (r) {
      //TODO: Will this overlay service support a higher level authenticator?
      //       upcallAll(r, andEq, verifyJoinOverlay, source, ident);
    }
    return r;
  } // verifyJoinOverlay

  //TODO: What to do here if not ready yet?
  /*ready*/
  upcall peerJoinedOverlay(const MaceKey& peer) {
    maceout << "key=" << Log::toHex(downcall_getEncryptedSymKey()) << endl
	    << "iv=" << Log::toHex(downcall_getEncryptedIV()) << endl
	    << "hash=" << Log::toHex(downcall_getSymKeyHash()) << Log::endl;

    downcall_route(peer, key_msg(downcall_getEncryptedSymKey(),
				 downcall_getEncryptedIV(),
				 downcall_getSymKeyHash()));

    upcallAllVoid(peerJoinedOverlay, peer);
  } // peerJoinedOverlay

  (state == ready) {
    downcall multicast(const MaceKey& dest, const string& message,
		       registration_uid_t rid) {
      maceout << "multicasting message of size " << message.size() << " to " << dest
	      << Log::endl;
      string m = encryptMessage(message);
      return downcall_multicast(dest, data(rid, m));
    } // multicast

    downcall anycast(const MaceKey& dest, const string& message,
		     registration_uid_t rid) {
      maceout << "anycasting message of size " << message.size() << " to " << dest
	      << Log::endl;
      string m = encryptMessage(message);
      return downcall_anycast(dest, data(rid, m));
    } // anycast

    downcall collect(const MaceKey& dest, const string& message,
		     registration_uid_t rid) {
      maceout << "collecting message of size " << message.size() << " to " << dest
	      << Log::endl;
      string m = encryptMessage(message);
      return downcall_collect(dest, data(rid, m));
    } // collect

    downcall distribute(const MaceKey& dest, const string& message,
			registration_uid_t rid) {
      maceout << "distributing message of size " << message.size() << " to " << dest
	      << Log::endl;
      string m = encryptMessage(message);
      return downcall_distribute(dest, data(rid, m));
    } // distribute

    downcall route(const MaceKey& dest, const std::string& message,
		   registration_uid_t rid) {
      maceout << "routing message of size " << message.size() << " to " << dest
	      << Log::endl;
      string m = encryptMessage(message);
      return downcall_route(dest, data(rid, m)); 
    } // route

    downcall routeDirect(const MaceKey& dest, const std::string& message,
			 registration_uid_t rid) {
      maceout << "routing direct message of size " << message.size() << " to " << dest
	      << Log::endl;
      string m = encryptMessage(message);
      return downcall_routeDirect(dest, data(rid, m)); 
    } // routeDirect

  } // state == ready

  //OPEN QUESTION: Does encryptor.mac need to do anything other than provide its own identity?
  //If not, imagine a syntax like: API create_group => create_group(groupId, myIdentity, mc_); instead of a full transition def.
  downcall (state == preJoining) joinOverlay(const NodeSet& peerSet,
					     const std::string& identity) {
    if (!identity.empty()) {
      std::cerr << "EncryptedMultiChannel::joinOverlay: WARNING - non-empty identity "
		<< "passed as argument: " << Log::toHex(identity) << endl;
    }
    
    state_change(joining);
    // note that we discard the identity passed to us, using the
    // identity computed using our Crypto service
    downcall_joinOverlay(peerSet, myIdentity);
  } // joinOverlay

  downcall (state != preJoining) joinOverlay(const NodeSet& peerSet,
					     const std::string& identity) {
    if (!identity.empty()) {
      std::cerr << "EncryptedMultiChannel::joinOverlay: WARNING - non-empty identity "
		<< "passed as argument: " << Log::toHex(identity) << endl;
    }

    downcall_joinOverlay(peerSet, myIdentity);
  } // joinOverlay

  /*ready*/
  downcall leaveOverlay() {
    downcall_leaveOverlay();
    state_change(preJoining);
  } // leaveOverlay

  upcall (state != init) getIdentity() {
    //TODO: Is this okay?  Do we need to do some kind of verification?
    return myIdentity;
  } // getIdentity

  upcall (state == key_received) joinResultOverlay(const MaceKey& source,
						     join_status_t status) {
    upcallAllVoid(joinResultOverlay, source, status);
    if(status == JOIN_ACCEPTED) {
      state_change(ready);
    }
  } // joinResultOverlay

  upcall (state == joining) joinResultOverlay(const MaceKey& source,
						join_status_t status) {
    if(status == JOIN_ACCEPTED) {
      state_change(waiting_for_key);
      source_ = source;
    }
    else {
      upcallAllVoid(joinResultOverlay, source, status)
    }
  } // joinResultOverlay

  //   (ready|waiting_for_key) API joinResult {
  //     //for now -- do nothing.
  //   }

  //TODO: Need to also handle change-of-keys -- can we reuse this code -- perhaps with a ready forward key_msg?
  upcall (state == joining || state == key_received)
    deliver(const MaceKey& source, const MaceKey& dest, const key_msg& msg, registration_uid_t rid) {
    maceout << "received a key message from " << source << Log::endl;
    maceout << "key=" << Log::toHex(msg.encSymKey) << endl
	    << "iv=" << Log::toHex(msg.encIV) << endl
	    << "hash=" << Log::toHex(msg.encSymKeyHash) << Log::endl;
    // XXX: probably should check whether the message came from our
    // parent in randtree, is this possible?
    // OK -- I'm thinking that instead we should protect it with a nonce.  This avoids needing to realize we are in a tree
    // Alternately we could use the joinResult as a guide here
    // When we get a new key -- we probably need to notify our down-line.  But what's the right way?
    ASSERT(downcall_setSymKey(msg.encSymKey, msg.encSymKeyHash, msg.encIV));
    if(/*msg.commType == COMM_TYPE_UNICAST*/ rid == ro_) {
      //XXX: But what if the new key came from your child?  Not presently allowed.
      downcall_distribute(MaceKey(), key_msg(msg.encSymKey, msg.encIV, msg.encSymKeyHash));
    }
    state_change(key_received);
  } // deliver key_msg

  upcall (state == ready)
    deliver(const MaceKey& source, const MaceKey& dest, const key_msg& msg, registration_uid_t rid) {
    maceout << "received a key message from " << source << Log::endl;
    maceout << "key=" << Log::toHex(msg.encSymKey) << endl
	    << "iv=" << Log::toHex(msg.encIV) << endl
	    << "hash=" << Log::toHex(msg.encSymKeyHash) << Log::endl;
    // XXX: probably should check whether the message came from our
    // parent in randtree, is this possible?
    // OK -- I'm thinking that instead we should protect it with a nonce.  This avoids needing to realize we are in a tree
    // Alternately we could use the joinResult as a guide here
    // When we get a new key -- we probably need to notify our down-line.  But what's the right way?
    ASSERT(downcall_setSymKey(msg.encSymKey, msg.encSymKeyHash, msg.encIV));
    if(rid == ro_ /*msg.commType == COMM_TYPE_UNICAST*/) {
      //XXX: But what if the new key came from your child?  Not presently allowed.
      downcall_distribute(MaceKey(), key_msg(msg.encSymKey, msg.encIV, msg.encSymKeyHash));
    }
  } // deliver key_msg 

  upcall (state == waiting_for_key)
    deliver(const MaceKey& source, const MaceKey& dest, const key_msg& msg, registration_uid_t rid) {
    maceout << "received a key message from " << source << Log::endl;
    maceout << "key=" << Log::toHex(msg.encSymKey) << endl
	    << "iv=" << Log::toHex(msg.encIV) << endl
	    << "hash=" << Log::toHex(msg.encSymKeyHash) << Log::endl;
    // XXX: probably should check whether the message came from our
    // parent in randtree, is this possible?
    // OK -- I'm thinking that instead we should protect it with a nonce.  This avoids needing to realize we are in a tree
    // Alternately we could use the joinResult as a guide here
    // When we get a new key -- we probably need to notify our down-line.  But what's the right way?
    ASSERT(downcall_setSymKey(msg.encSymKey, msg.encSymKeyHash, msg.encIV));
    if(rid == ro_ /*msg.commType == COMM_TYPE_UNICAST*/) {
      //XXX: But what if the new key came from your child?  Not presently allowed.
      downcall_distribute(MaceKey(), key_msg(msg.encSymKey, msg.encIV,
					     msg.encSymKeyHash));
    }
    upcallAllVoid(joinResultOverlay, source_, JOIN_ACCEPTED);
  } // deliver key_msg

  // XXX --- these are disabled because
  // HierarchicalMulticastServiceClass is not a Tree, so it does not
  // have the appropriate tree handlers

  // tree
  downcall isRoot(const MaceKey& groupId) {
    return downcall_isRoot(groupId);
  } // isRoot

  upcall notifyChildren(const MaceKey& groupId, NodeSet children) {
    upcallAllVoid(notifyChildren, groupId, children);
  } // notifyChildren

  upcall notifyParent(const MaceKey& groupId, const MaceKey& parent) {
    upcallAllVoid(notifyParent, groupId, parent);
  } // notifyParent

  upcall notifyParentFailed(const MaceKey& groupId, const MaceKey& formerParent) {
    upcallAllVoid(notifyParentFailed, groupId, formerParent);
  } // notifyParentFailed

  upcall (state == ready) deliver(const MaceKey& source, const MaceKey& dest,
				    const data& msg) {
    //NOTE: Works for all communication types!
    //NOTE 2: Does the mirror of encryptMessage (below)
    //     debug_macro("encrypted data: %s\n", toHex(msg.payload).c_str());
    string m;
    m = downcall_decrypt(msg.payload);
    if(m.size() == 0) {
      maceout << "WARNING -- decrypt returned 0 length data." << Log::endl;
      return;
    }
    //     debug_macro("unencrypted data: %s\n", toHex(m).c_str());
    string skh;
    istringstream in(m);
    skh.deserialize(in);
    string d;
    d.deserialize(in);
    
    if(skh == downcall_getSymKeyHash()) {
      maceout << "Message decrypted, received size " << d.size() << Log::endl;
      upcall_deliver(source, dest, d, msg.commType, msg.rid);
    }
    else {
      maceout << "Could not decrypt message" << Log::endl;
    }
  } // deliver data

} // transition

routines {

  //NOTE: Does more than just call "encrypt."  Also prepends the sym
  //key hash for verification.
  string encryptMessage(const mace::string& message) {
    //s is the unencrypted string we will use
    string s;

    //Here we construct s
    downcall_getSymKeyHash().serialize(s);
    message.serialize(s);

    //Now we encrypt s 
    return downcall_encrypt(s);
  }

} // routines

