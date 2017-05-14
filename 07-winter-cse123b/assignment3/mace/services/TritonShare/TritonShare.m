//Copyright (c) 2004, Charles Killian, Adolfo Rodriguez, Dejan Kostic, Sooraj Bhat, and Amin Vahdat
//All rights reserved.
//
//Redistribution and use in source and binary forms, with or without
//modification, are permitted provided that the following conditions are met:
//
//   * Redistributions of source code must retain the above copyright
//     notice, this list of conditions and the following disclaimer.
//   * Redistributions in binary form must reproduce the above copyright
//     notice, this list of conditions and the following disclaimer in
//     the documentation and/or other materials provided with the
//     distribution.
//   * Neither the names of Duke University nor The University of
//     California, San Diego, nor the names of its contributors
//     may be used to endorse or promote products derived from
//     this software without specific prior written permission.
//
//THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
//AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
//FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
//DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
//SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
//CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
//OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
//USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


#include "NodeCollection.h"
using mace::NodeCollection;
#include "lib/Iterator.h"

#include <algorithm>

#define OUTPUT (maceout)
//#define OUTPUT (std::cerr)
#define ENDL (Log::endl)
//#define ENDL (std::endl)

using mace::MapIterator;
using mace::ConstMapIterator;

using mace::string;


service TritonShare;

provides P2P;

trace = low;
//trace = med;


/* constants
 * In this block you will define all the constants
 * used by your program.  You can treat constants
 * as you would a normal c-style constant
 */
constants {
  // The maximum number of children any supernode can have. You may lower this
  // number for testing purposes.
  unsigned int MAX_SUPERNODE_CHILDREN = 5;

  // The maximum number of peers any supernode is connected to.
  unsigned int MAX_SUPERNODE_PEERS = 5;
  unsigned int MAX_SUPERNODE_PEERS_PLUSONE = 6;

  // Maximum number of join attempts a client makes before informing the node it
  // is joining to that we failed (taken as a hint to create more supernodes).
  int MAX_JOIN_ATTEMPTS = 3;
  
  //Number of seconds until we consider a join timed out
  uint64_t P2P_JOIN_TIMEOUT = 5*1000*1000;

  // Maximum number of hops a join message will take before failing.
  int MAX_JOIN_HOPS = 5;

  // Maximum number of hops a supernode election msg will take.
  // Used to find the "best" client among a random set of supernodes.
  int MAX_SUPERNODE_ELECTION_HOPS = 10;

  // Number of hops a random walk will take before failing.
  // Used to create a random subset of peers to connect to.
  int MAX_RANDOM_WALK_HOPS = MAX_SUPERNODE_PEERS*2;

  // Timeout for a random supernode walk when we are gathering peers.
  uint64_t GET_PEERS_TIMER_INTERVAL = 10*1000*1000;
  
  // Interval at which we poll the application for updated files.
  uint64_t UPDATE_FILES_INTERVAL = 60*1000*1000;

  // Maximum number of hops a flooded search message will take in the network.
  int MAX_SEARCH_HOPS = 10;
  
  uint64_t UPDATE_ACTIVE_INDEX_INTERVAL = 1*1000*1000;
  
  double SUPERNODE_ELECTION_ERRONEOUS_BANDWIDTH = -1;
}


/* Services
 * Here you define the services your protocol will use
 * In our case, we will simply route everythign over TCP
 * so TCP is the only service we require
 */
services {
  //For simplification, all msgs will be routed 
  // using the TCP service class
  // Route control_ = UdpTransport();
  Transport control_ = TcpTransport();
}


/* constructor_parameters
 * Here you will define paramters to your service
 * You do not need to worry about this since all your
 * parameters are defined as constants above
 */
constructor_parameters { }


/* states
 * In this block you define all the states your service can be in
 * The init state is implied and does not need to be explicitly defined
 */
states {
  pre_joining; 
  supernode_joined;
  client_joined;
  client_joining;
}


/* auto_types
 * Here you will define all the auto_types used by your program
 * Auto_types will be automatically serialized provided 
 * only use mace types and preivously defined auto types.
 *
 * If an auto type is specified as a node, it will have an id
 * implicity generated, which can be accessed using the function
 * getId()
 */
auto_types {
  //When we are supernode status, a supernode_peer will be one of the k peers
  //we know about
  supernode_peer __attribute((node())) {
  }

  //when we are a supernode, a supernode_client is one of our clients
  supernode_client __attribute((node())) {
    // For each client we maintain advertised bandwidth
    // and the files they are sharing
    double advertised_bandwidth;    
    int http_port;
    FileSet available_files;
  }

  // When we are a client, a client_supernode is the supernode we know about
  client_supernode __attribute((node())) {
  }
}


/* typedefs
 * Here you will define typedefs used by your program.
 * The notation is c-style
 */
typedefs {

  //MAX_SUPERNODE_PEERS+1 -> made implementation of behavior when receiving
  //add_sn_neighbor easier
  typedef NodeCollection<supernode_peer, MAX_SUPERNODE_PEERS_PLUSONE> supernode_peers;
  typedef NodeCollection<supernode_client, MAX_SUPERNODE_CHILDREN> supernode_clients;

  typedef mace::hash_map<MaceKey, uint> ClientKeyToSearchIdMap;
}

/*
 * Declaration of state variables (note the difference between state
 * variables and the protocol's FSM state).
 *
 * These variables are declared according to normal conventions.
 */
state_variables {
  // If we are connected as a client, then my_supernode is the supernode we are
  // connected to, to which we direct all search requests.
  // Otherwise it is not used.
  client_supernode my_supernode;

  // If we are connected as a supernode, my_peers is the set of peers to which
  // we will flood all search requests. Otherwise it is not used
  supernode_peers my_peers;
  
  // If we are connected as a supernode, my_clients will refer to the clients
  // connected to us
  supernode_clients my_clients;

  // Timer used when joining the network
  timer join_timer;

  // Timer used when doing a walk to collect a random set of peers
  timer get_sn_peers_timer;

  // Timer used when polling the application layer for a list of files
  timer update_files_timer __attribute((recur(UPDATE_FILES_INTERVAL)));

  // My key in the network
  MaceKey my_ipv4;

  // My http port I am hosting files from
  short my_http_port;
  
  // The initial peer that we will connected to when specified in P2PConnect
  MaceKey initial_peer;
  
  // A counter used to keep track of number of times we have attempted to join
  int num_join_attemps;

  // The bandwidth we advertise - used for supernode selection
  // This is specified in the P2PJoin downcall
  double my_advertised_bandwidth_kbps;

  // UID for the p2p handler
  registration_uid_t my_p2pHandler_uid;

  // The set of files that we currently have available
  FileSet my_available_files;

  // put the rest of your state variables here
  uint my_query_sequence_number;
  
  ClientKeyToSearchIdMap my_client_search_id_map;

  timer update_active_index_timer;
}


/* method_remappings
 * These are method remappings that will be used by your protocol
 * We have included all the method remappings you will need so you
 * should not have to add to this block
 */
method_remappings {
  uses {
    upcall_joinStatusChanged(join_status_t newstaus);
    
  }
  // This is how we would add an error handler:
  implements {
    upcalls {
      messageError(MaceKey const &key, TransportError::type code,
                   std::string const & message);
    }
  }
}


/* messages
 * In this block you will define all the messages used by your protocol
 */
messages {
  /* client_join:
   * This message is sent by a client wishing to join the network
   * The message is randomly directed to supernodes until it reaches a
   * Supernode that can take it, or until the hop counter runs out
   */
  client_join {
    MaceKey source; // source of initiator for join
    int hop_counter;       // count - decremented at each hop
    double advertised_bandwidth;
    short http_port;
  }

  /* accept_client
   * this message is sent from a supernode to a client informing the
   * client that it has been accepted to the network.  The sending supernode
   * is the client's supernode
   */
  accept_client {    
  }

  accept_client_failure {
  }

  /* new_supernode_election
   * This message is used to find a client to 
   * to be the new supernode
   */
  new_supernode_election {    
    MaceKey best_recorded_client;
    double best_recorded_bandwidth;
    MaceKey sn_with_best_client;
    int hop_counter;
  }

  /* upgrade - a messaged used to inform a connected client that he is
   * upgraded to supernode status.
   */
  upgrade {
  }

  /* join failed
   *
   * A message sent to a supernode from a neighbor attempting to join
   * which signifiese that the client has failed MAX_JOIN_ATTEMPTS
   * attempts to join and failed.  The supernode takes this as a hint
   * to add a new supernode to the network
   */  
  join_failed {
    MaceKey source;
    double advertised_bandwidth;
    int http_port;
  }

  /* random_supernode_walk
   * 
   * This message is for collecting a random subset of supernodes
   */
  random_supernode_walk {
    MaceKey source;
    supernode_peers peers;
    unsigned int hop_counter;
    unsigned int max_size_of_peerSet;
  }

  /* add_sn_neighbor
   *
   * Informs a supernode that he must add the sender of this message
   * as one of his peers
   */
  add_sn_neighbor {
  }

  /* Search
   * A search message that is flooded to all supernodes
   */
  search {
    MaceKey source;
    uint sequence_number;
    mace::string query;
    int hop_counter;
  }
  
  /* Search result
   */
  search_result {
    uint sequence_number;    
    QueryResultSet result;
  }

  /* supernode_leave
   * A message sent by supernodes to all of its clients when it
   * wishes to leave the network
   */
  supernode_leave {
    supernode_peers alternate_peers; 
  }

  /* client_leave
   * A message sent by a client to its supernode informing the supernode
   * that it wishes to leave the network
   */
  client_leave {
  }

  /* update_active_index
   * A message sent by a client to its supernode informing the supernode
   * that the client wishes to upgrade its active index
   */
  update_active_index {
    //FileIndex new_active_index;
    FileSet available_files;
  }

  /* req_active_index
   * A message sent by a supernode to a client informing the client
   * to send it it's most recent index of files
   */
  req_active_index {
  }
}


/*
 * Definition of the protocol transitions. That is, the code which is
 * triggered when events occur. This wholly codifies the entry-points
 * into the protocol execution.
 */
transitions {
  /////////////////////////////////////////////////////////////////////////////
  // The init API call will be called at the protocol's start time, so the
  // system will be in the init state. 
  /////////////////////////////////////////////////////////////////////////////
  downcall (state == init) maceInit() {
    OUTPUT << ENDL;
    OUTPUT << "(downcall) [init] MACE_INIT" << ENDL;
    
    // my_ipv4 = MaceKey(ipv4, Util::getAddr());
    my_ipv4 = downcall_getLocalAddress();
    state = pre_joining;

    my_supernode = client_supernode();
    my_peers = supernode_peers();
    my_clients = supernode_clients();
    
    num_join_attemps = 0;
    
    my_query_sequence_number = 0;
    my_client_search_id_map = ClientKeyToSearchIdMap();
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // Timer for updating files timed out. Make an upcall to app to get files.
  /////////////////////////////////////////////////////////////////////////////
  scheduler (true) update_files_timer() {
    OUTPUT << ENDL;
    OUTPUT << "(timer) [any] UPDATE_FILES" << ENDL;
    
    my_available_files = upcall_getAvailableFiles(my_p2pHandler_uid);   
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // Timer for updating a supernode's index timed out
  /////////////////////////////////////////////////////////////////////////////
  scheduler (state == supernode_joined) update_active_index_timer() {
    OUTPUT << ENDL;
    OUTPUT << "(timer) [supernode] UPDATE_ACTIVE_INDEX" << ENDL;

    supernode_clients::const_iterator client_iter = my_clients.begin();
    for (; client_iter != my_clients.end(); ++client_iter)
      downcall_route(client_iter->second.getId(), req_active_index());

    update_active_index_timer.reschedule(UPDATE_ACTIVE_INDEX_INTERVAL);
  }

  /////////////////////////////////////////////////////////////////////////////
  // Message from tcp layer requesting list of files
  /////////////////////////////////////////////////////////////////////////////
  upcall (stateHasJoined())
      deliver(const MaceKey& source, const MaceKey& dest, 
              const req_active_index& message,
              registration_uid_t p2pHandler_uid) {
    OUTPUT << ENDL;
    OUTPUT << "(upcall) [joined] REQ_ACTIVE_INDEX" << ENDL;

    downcall_route(source, update_active_index(my_available_files)); 
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // Message from tcp layer to supernode with an updated list of files.
  /////////////////////////////////////////////////////////////////////////////
  upcall (state == supernode_joined)
      deliver(const MaceKey& source, const MaceKey& dest, 
              const update_active_index &message,
              registration_uid_t p2pHandler_uid) {
    OUTPUT << ENDL;
    OUTPUT << "(upcall) [supernode] UPDATE_ACTIVE_INDEX" << ENDL;
    
    OUTPUT << "got an file set update for " << source << ENDL;

    supernode_clients::iterator client_iterator = my_clients.find(source);

    if (client_iterator == my_clients.end())
      return;

    supernode_client &client = client_iterator->second;
    client.available_files = message.available_files; 
  }

  /////////////////////////////////////////////////////////////////////////////
  // request by application layer to join a P2P network
  /////////////////////////////////////////////////////////////////////////////
  downcall (state == pre_joining) 
      joinP2P(const MaceKey& peerToJoin, double advertisedBandwidth_kbps, 
              short http_port, const std::string& identity,
              registration_uid_t p2pHandler_uid) {
    OUTPUT << ENDL;
    OUTPUT << "(downcall) [pre_joining] JOIN_P2P" << ENDL;
    
    OUTPUT << "Im joinP2P, joining peer " << peerToJoin << ENDL;
    // Send a join message to the peer and set the timeout

    initial_peer = peerToJoin;
    my_advertised_bandwidth_kbps = advertisedBandwidth_kbps;
    my_http_port = http_port;
    
    my_p2pHandler_uid = p2pHandler_uid;
    
    my_available_files = upcall_getAvailableFiles(my_p2pHandler_uid);
    update_files_timer.reschedule(UPDATE_FILES_INTERVAL);
    
    OUTPUT << "I am " << my_ipv4 << ENDL;
    OUTPUT << "The bootstrap is " << peerToJoin << ENDL;

    // If i am specified as the bootstrap, then I am a supernode
    if (initial_peer == my_ipv4) {
      OUTPUT << "I am the bootstrap" << ENDL;
      becomeSupernode();
    }
    else {
      OUTPUT << "I am not the bootstrap" << ENDL;
      joinTheNetwork(initial_peer);
    }
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // message from tcp layer that client has been accepted
  /////////////////////////////////////////////////////////////////////////////
  upcall (state == client_joining)
      deliver(const MaceKey& source, const MaceKey& dest, 
              const accept_client& Msg, registration_uid_t p2pHandler_uid) {
    OUTPUT << ENDL;
    OUTPUT << "(upcall) [joining] ACCEPT_CLIENT" << ENDL;
    
    OUTPUT << "I joined the network" << ENDL;

    my_supernode = client_supernode(source);
    
    state = client_joined;
    upcallAllVoid(joinStatusChanged, P2P_CLIENT_JOINED);
    
    join_timer.cancel();
    
    update_active_index update_active_index_message(my_available_files);
    downcall_route(my_supernode.getId(), update_active_index_message);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // message from tcp layer that client is joining and we're not a supernode
  /////////////////////////////////////////////////////////////////////////////
  upcall (state == client_joined)
      deliver(const MaceKey& source, const MaceKey& dest,
              const client_join &message, registration_uid_t p2pHandler_uid) {
    OUTPUT << ENDL;
    OUTPUT << "(upcall) [client] CLIENT_JOIN" << ENDL;
    
    OUTPUT << "not a supernode, forwarding to my supernode: "
              << my_supernode.getId() << ENDL;
    
    downcall_route(my_supernode.getId(), message);
  }

  /////////////////////////////////////////////////////////////////////////////
  // message from tcp layer that client is joining and we are a supernode
  /////////////////////////////////////////////////////////////////////////////
  upcall (state == supernode_joined)
      deliver(const MaceKey& source, const MaceKey& dest,
              const client_join &message, registration_uid_t p2pHandler_uid) {
    OUTPUT << ENDL;
    OUTPUT << "(upcall) [supernode] CLIENT_JOIN" << ENDL;
    
    OUTPUT << "received client join request" << ENDL;

    if (message.hop_counter == 0) {
      OUTPUT << "hop_count is 0, sending accept failure" << ENDL;
      
      downcall_route(message.source, accept_client_failure());
      return;
    }

    if (my_clients.full()) {
      OUTPUT << "no space for client" << ENDL;

      if (my_peers.empty()) {
        OUTPUT << "no peers, sending accept failure" << ENDL;
        
        downcall_route(message.source, accept_client_failure());
      }
      else {
        int new_hop_counter = message.hop_counter - 1;
        client_join new_message(message.source, new_hop_counter,
                                message.advertised_bandwidth, message.http_port);
  
        const MaceKey &random_peer_id = my_peers.random().getId();
        
        OUTPUT << "forwarding to random peer " << random_peer_id << ENDL;
        downcall_route(random_peer_id, new_message);
      }

      return;
    }

    OUTPUT << "accepting client" << ENDL;

    FileSet empty_file_set;
    my_clients.add(supernode_client(message.source, message.advertised_bandwidth,
                                    message.http_port, empty_file_set));
    
    downcall_route(message.source, accept_client());
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // message from tcp layer that client was not accepted
  /////////////////////////////////////////////////////////////////////////////
  upcall (state == client_joining)
    deliver(const MaceKey& source, const MaceKey& dest,
            const accept_client_failure &message,
            registration_uid_t p2pHandler_uid) {
    OUTPUT << ENDL;
    OUTPUT << "(upcall) [joining] ACCEPT_CLIENT_FAILURE" << ENDL;
    
    OUTPUT << "got client accept failure message" << ENDL;

    // Why do we reschedule here?  Why not just wait for the timer to go off? -Erik
    join_timer.reschedule(P2P_JOIN_TIMEOUT);
  }

  /////////////////////////////////////////////////////////////////////////////
  // join timer timed out, retry connection and increment count
  /////////////////////////////////////////////////////////////////////////////
  scheduler (state == client_joining) join_timer() {
    OUTPUT << "(timer) [joining] JOIN" << ENDL;
    
    join_timer.reschedule(P2P_JOIN_TIMEOUT);
    
    if (num_join_attemps < MAX_JOIN_ATTEMPTS) {
      OUTPUT << "join timer timed out, retrying" << ENDL;
      
      client_join join_message(my_ipv4, MAX_JOIN_HOPS,
                               my_advertised_bandwidth_kbps, my_http_port);

      downcall_route(initial_peer, join_message);
      num_join_attemps++;
      return;
    }

    OUTPUT << "reached " << MAX_JOIN_ATTEMPTS << " join attempts, "
           << "sending join failed message to supernode" << ENDL;

    join_failed join_failed_message(my_ipv4, my_advertised_bandwidth_kbps,
                                    my_http_port);

    downcall_route(initial_peer, join_failed_message);
    num_join_attemps = 0;
  }

  /////////////////////////////////////////////////////////////////////////////
  // new_supernode_election message: Find my best bandwidth client
  // and update the message if it's better, and then pass it on.
  /////////////////////////////////////////////////////////////////////////////
  upcall (state == supernode_joined)
    deliver(const MaceKey& source, const MaceKey& dest,
            const new_supernode_election &message,
            registration_uid_t p2pHandler_uid) {
    OUTPUT << ENDL;
    OUTPUT << "(upcall) [supernode] NEW_SUPERNODE_ELECTION" << ENDL;

    double best_bandwidth = message.best_recorded_bandwidth;
    MaceKey best_client = message.best_recorded_client;
    MaceKey best_supernode = message.sn_with_best_client;

    // Find the client with the best bandwidth
    supernode_clients::const_iterator client_iter = my_clients.begin();
    for (; client_iter != my_clients.end(); ++client_iter)
      if (client_iter->second.advertised_bandwidth > best_bandwidth) {
        best_bandwidth = client_iter->second.advertised_bandwidth;
        best_client = client_iter->second.getId();
        best_supernode = my_ipv4;
      }

    // If hop_counter is 0 or I have no peers, send the upgrade message
    if (my_peers.empty() || message.hop_counter == 0) {
      if (best_bandwidth == SUPERNODE_ELECTION_ERRONEOUS_BANDWIDTH)
        OUTPUT << "I have no peers, and there is no best client, "
               << "election coming to an end" << ENDL;
      else {
        OUTPUT << "sending upgrade to " << best_client << ENDL;
        downcall_route(best_client, upgrade());
      }
      
      return;
    }

    // Otherwise pass on the message
    int new_hop_counter = message.hop_counter - 1;
    new_supernode_election election_message(best_client, best_bandwidth,
                                            best_supernode, new_hop_counter);

    // Send this message
    downcall_route(my_peers.random().getId(), election_message);
  }

  /////////////////////////////////////////////////////////////////////////////
  // join_failed message and I'm a client: forward to my_supernode
  /////////////////////////////////////////////////////////////////////////////
  upcall (state == client_joined)
      deliver(const MaceKey &source, const MaceKey &destination,
              const join_failed &message, registration_uid_t p2pHandler_uid) {
    OUTPUT << ENDL;
    OUTPUT << "(upcall) [client] JOIN_FAILED" << ENDL;
    
    downcall_route(my_supernode.getId(), message);
  }

  /////////////////////////////////////////////////////////////////////////////
  // join_failed message and I'm a supernode: start new_supernode_election
  /////////////////////////////////////////////////////////////////////////////
  upcall (state == supernode_joined)
      deliver(const MaceKey& source, const MaceKey& dest,
              const join_failed &message, registration_uid_t p2pHandler_uid) {
    OUTPUT << ENDL;
    OUTPUT << "(upcall) [supernode] JOIN_FAILED" << ENDL;

    double best_bandwidth = SUPERNODE_ELECTION_ERRONEOUS_BANDWIDTH;
    const MaceKey *best_client = &my_ipv4;

    // Find the client with the best bandwidth
    supernode_clients::const_iterator client_iter = my_clients.begin();
    for (; client_iter != my_clients.end(); ++client_iter)
      if (client_iter->second.advertised_bandwidth > best_bandwidth) {
        best_bandwidth = client_iter->second.advertised_bandwidth;
        best_client = &(client_iter->second.getId());
      }

    // If there are no peers, send the upgrade message to the best client.
    if (my_peers.empty()) {
      OUTPUT << "I have no peers; telling my best client to upgrade" << ENDL;
      
      if (best_client != &my_ipv4)
        downcall_route(*best_client, upgrade());
      return;
    }

    new_supernode_election election_message(*best_client, best_bandwidth,
                                            my_ipv4, MAX_SUPERNODE_ELECTION_HOPS);

    // Send this message
    downcall_route(my_peers.random().getId(), election_message);
  }

  /////////////////////////////////////////////////////////////////////////////
  // maceExit() call.  I don't know what this does.
  /////////////////////////////////////////////////////////////////////////////
  downcall (true) maceExit() {
    OUTPUT << ENDL;
    OUTPUT << "(downcall) [any] MACE_EXIT" << ENDL;
  }

  /////////////////////////////////////////////////////////////////////////////
  // request by application layer to leave the p2p network for non supernode
  /////////////////////////////////////////////////////////////////////////////
  downcall (state == client_joined)
      leaveP2P(registration_uid_t p2pHandler_uid) {
    OUTPUT << ENDL;
    OUTPUT << "(downcall) [client] LEAVE_P2P" << ENDL;
    
    OUTPUT << "non-supernode leaving the network" << ENDL;
    
    downcall_route(my_supernode.getId(), client_leave());

    state = pre_joining;
    upcallAllVoid(joinStatusChanged, P2P_JOINING);
  }

  /////////////////////////////////////////////////////////////////////////////
  // message from tcp layer that a client is leaving
  /////////////////////////////////////////////////////////////////////////////
  upcall (state == supernode_joined)
      deliver(const MaceKey& source, const MaceKey& dest,
              const client_leave &message, registration_uid_t p2pHandler_uid) {
    OUTPUT << ENDL;
    OUTPUT << "(upcall) [supernode] CLIENT_LEAVE" << ENDL;
    
    OUTPUT << "removing " << source << " from client list" << ENDL;

    my_clients.erase(source);
    
    OUTPUT << "client " << source << " removed" << ENDL;
  }

  /////////////////////////////////////////////////////////////////////////////
  // request by application layer to leave the p2p network for supernode
  /////////////////////////////////////////////////////////////////////////////
  downcall (state == supernode_joined)
      leaveP2P(registration_uid_t p2pHandler_uid) {
    OUTPUT << ENDL;
    OUTPUT << "(downcall) [supernode] LEAVE_P2P" << ENDL;
    
    OUTPUT << "supernode leaving the network" << ENDL;
    
    supernode_clients::const_iterator iter;
    for (iter = my_clients.begin(); iter != my_clients.end(); iter++)
      downcall_route(iter->second.getId(), supernode_leave(my_peers));

    state = pre_joining;
    upcallAllVoid(joinStatusChanged, P2P_JOINING);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // message from tcp layer that a supernode is leaving
  /////////////////////////////////////////////////////////////////////////////
  upcall (state == client_joined)
      deliver(const MaceKey& source, const MaceKey& dest,
              const supernode_leave &message,
              registration_uid_t p2pHandler_uid) {
    OUTPUT << ENDL;
    OUTPUT << "(upcall) [client] SUPERNODE_LEAVE" << ENDL;
    
    // Was that my supernode?
    if (source == my_supernode.getId()) {
      OUTPUT << "my supernode left" << ENDL;
 
      // We got disconnected :(
      state = pre_joining;

      if (!message.alternate_peers.empty())
        joinTheNetwork(message.alternate_peers.random().getId());
      else { // The bootstrap supernode just left, I'm all alone
        becomeSupernode();
      }
    }
  }

  /////////////////////////////////////////////////////////////////////////////
  // message from tcp layer that node should upgrade to a supernode
  /////////////////////////////////////////////////////////////////////////////
  upcall (state == client_joined)
      deliver(const MaceKey& source, const MaceKey& dest,
              const upgrade &message, registration_uid_t p2pHandler_uid) {
    OUTPUT << ENDL;
    OUTPUT << "(upcall) [client] UPGRADE" << ENDL;
    
    OUTPUT << "upgrading to supernode" << ENDL;

    becomeSupernode();
    
    random_supernode_walk walk_msg(my_ipv4, my_peers, MAX_RANDOM_WALK_HOPS,
                                   MAX_SUPERNODE_PEERS);

    downcall_route(my_supernode.getId(), walk_msg);
    
    get_sn_peers_timer.reschedule(GET_PEERS_TIMER_INTERVAL);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // message from tcp layer for random supernode walk
  /////////////////////////////////////////////////////////////////////////////
  upcall (state == supernode_joined)
      deliver(const MaceKey& source, const MaceKey& dest,
              const random_supernode_walk &msg, registration_uid_t foo) {
    OUTPUT << ENDL;
    OUTPUT << "(upcall) [supernode] RANDOM_SUPERNODE_WALK" << ENDL;
    
    OUTPUT << "random walk from " << msg.source << ENDL;
    
    if (msg.source == my_ipv4) {
      OUTPUT << "walk message came back to me" << ENDL;
      
      supernode_peers::const_iterator iter;
      for (iter = msg.peers.begin(); iter != msg.peers.end(); iter++) {
        if (isFull(my_peers))
          break;
        
        const supernode_peer &peer = iter->second;
        if (my_peers.contains(peer.getId()))
          continue;
        
        OUTPUT << "adding " << peer.getId() << " to my peers" << ENDL;
        
        my_peers.add(peer);
        OUTPUT << "My new list of peers: " << my_peers << ENDL;
        
        downcall_route(peer.getId(), add_sn_neighbor());
      }

      get_sn_peers_timer.cancel();
      return;
    }

    if (msg.hop_counter == 0) {
      OUTPUT << "hop_count is 0, sending back to source" << ENDL;
      
      downcall_route(msg.source, msg);
      return;
    }

    supernode_peers new_peers = msg.peers;
    
    if (!isFull(new_peers) && !new_peers.contains(my_ipv4)) {
      OUTPUT << "Adding myself to list of peers" << ENDL;
      
      new_peers.add(my_ipv4);
    }

    if (isFull(new_peers)) {
      OUTPUT << "peers list full, sending back to source" << ENDL;
      
      random_supernode_walk new_walk_msg(msg.source, new_peers, msg.hop_counter,
                                        msg.max_size_of_peerSet);

      downcall_route(msg.source, new_walk_msg);
      return;
    }
    
    // If we have no peers we can't send this to a random one
    if (my_peers.empty()) {
      OUTPUT << "I have no peers!! Sending back to source" << ENDL;
      
      random_supernode_walk new_walk_msg(msg.source, new_peers, msg.hop_counter,
                                         msg.max_size_of_peerSet);
                                        
      downcall_route(msg.source, new_walk_msg);
      return;
    }

    OUTPUT << "checking if we have a peer that is not in the list" << ENDL;

    supernode_peers::const_iterator iter;
    for (iter = my_peers.begin(); iter != my_peers.end(); iter++)
      if (!msg.peers.contains(iter->second.getId()))
        break;

    
    supernode_peer &peer = my_peers.random();

    if (iter != my_peers.end()) {
      OUTPUT << "some of my peers are not in the list, finding random one" << ENDL;

      while (new_peers.contains(peer.getId()))
        peer = my_peers.random();
      new_peers.add(peer);
    }

    OUTPUT << "sending walk message to random node " << peer.getId() << ENDL;
    
    int new_hop_counter = msg.hop_counter - 1;
    random_supernode_walk new_walk_msg(msg.source, new_peers, new_hop_counter,
                                       msg.max_size_of_peerSet);

    downcall_route(peer.getId(), new_walk_msg);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // random supernode walk timer timed out
  /////////////////////////////////////////////////////////////////////////////
  scheduler (state == supernode_joined) get_sn_peers_timer() {
    OUTPUT << ENDL;
    OUTPUT << "(timer) [supernode] GET_SN_PEERS" << ENDL;
    
    OUTPUT << "random supernode walk timer timed out" << ENDL;

    if (isFull(my_peers))
      return;

    random_supernode_walk walk_msg(my_ipv4, my_peers, MAX_RANDOM_WALK_HOPS,
                                   MAX_SUPERNODE_PEERS);
    downcall_route(my_supernode.getId(), walk_msg);

    get_sn_peers_timer.reschedule(GET_PEERS_TIMER_INTERVAL);
  }

  /////////////////////////////////////////////////////////////////////////////
  // message from tcp layer to add a supernode as a peer
  /////////////////////////////////////////////////////////////////////////////
  upcall (state == supernode_joined)
      deliver(const MaceKey& source, const MaceKey& dest,
              const add_sn_neighbor &message, registration_uid_t foo) {
    OUTPUT << ENDL;
    OUTPUT << "(upcall) [supernode] ADD_SN_NEIGHBOR" << ENDL;
    
    OUTPUT << "adding supernode " << source << " as peer" << ENDL;
    
    my_peers.add(source);
    
    OUTPUT << "My new list of peers: " << my_peers << ENDL;
    
    // Only remove a peer if our list has too many peers
    if (my_peers.full()) {
      supernode_peer &peer_to_forget = my_peers.random();
      OUTPUT << "Now we have too many peers, randomly removing this one: " 
             << peer_to_forget << ENDL;
      my_peers.erase(peer_to_forget.getId());
    }
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // Request by application layer to search for a file.
  /////////////////////////////////////////////////////////////////////////////
  downcall (stateHasJoined())
      queryP2P(const std::string& searchString,
               registration_uid_t p2pHandler_uid) {
    OUTPUT << ENDL;
    OUTPUT << "(downcall) [joined] QUERY_P2P" << ENDL;
    
    OUTPUT << "querying network for " << searchString << ENDL;

    ++my_query_sequence_number;
    mace::string str = mace::string(searchString);
    search search_message(my_ipv4, my_query_sequence_number, str, MAX_SEARCH_HOPS);
    
    // If we're a client, ask our supernode
    if (state == client_joined)
      downcall_route(my_supernode.getId(), search_message);
    else // Search my own clients, and forward the message to my peers
      downcall_route(my_ipv4, search_message);
    
    return my_query_sequence_number;
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // Message from tcp layer to search for a file.
  /////////////////////////////////////////////////////////////////////////////
  upcall (state == supernode_joined)
      deliver(const MaceKey& source, const MaceKey& dest,
              const search &message, registration_uid_t foo) {
    OUTPUT << ENDL;
    OUTPUT << "(upcall) [supernode] SEARCH" << ENDL;
    
    OUTPUT << "search initiated by " << message.source << " for file "
        << message.query << ENDL;

    uint search_id = message.sequence_number;

    ClientKeyToSearchIdMap::iterator client_search_id_iterator;
    client_search_id_iterator = my_client_search_id_map.find(message.source);

    // Sequence number stuff
    if (client_search_id_iterator == my_client_search_id_map.end()) {
      ClientKeyToSearchIdMap::value_type value(message.source, search_id);
      OUTPUT << "adding " << message.source << " -> " << search_id
             << " to search/sequence# map" << ENDL;
      my_client_search_id_map.insert(value);
    }
    else {
      uint &client_search_id = client_search_id_iterator->second;
      if (search_id <= client_search_id) {
        OUTPUT << "Ignoring this search, it's too old ("
            << search_id << "<=" << client_search_id << ")" << ENDL;
        return;
      }
      client_search_id = search_id;
    }
    
    QueryResultSet results;

    // Search through my clients
    supernode_clients::const_iterator client_iterator = my_clients.begin();
    for (; client_iterator != my_clients.end(); ++client_iterator) {
      const supernode_client &client = client_iterator->second;
      const FileSet &client_files = client.available_files;
      
      FileSet client_matching_files;

      FileSet::const_iterator file_iterator = client_files.begin();
      for (; file_iterator != client_files.end(); ++file_iterator) {
        const mace::string &file_string = *file_iterator;
        if (file_string.find(message.query) != mace::string::npos)
          client_matching_files.push_back(file_string);
      }
      
      if (!client_matching_files.empty()) {
        QueryResult result(client.getId(), client.http_port,
                           client_matching_files);
        results.push_back(result);
      }
    }

    // Also check my own files
    FileSet my_matching_files;

    FileSet::const_iterator file_iterator = my_available_files.begin();
    for (; file_iterator != my_available_files.end(); ++file_iterator) {
      const mace::string &file_string = *file_iterator;
      if (file_string.find(message.query) != mace::string::npos)
        my_matching_files.push_back(file_string);
    }
    
    if (!my_matching_files.empty()) {
      QueryResult result(my_ipv4, my_http_port, my_matching_files);
      results.push_back(result);
    }
    
    // If there were matching files, send the results back
    if (!results.empty()) {
      OUTPUT << "Found one or more matching files from my clients! Sending "
          << "search_result" << ENDL;
      search_result result_message(search_id, results);
      downcall_route(message.source, result_message);
    }
    else {
      OUTPUT << "None of my clients have matching files" << ENDL;
    }
    
    // Forward the message on if it hasn't expired
    if (message.hop_counter > 0) {
      int new_hop_counter = message.hop_counter - 1;
      search new_message(message.source, search_id,
                         message.query, new_hop_counter);
      
      supernode_peers::const_iterator peer_iterator = my_peers.begin();
      for (; peer_iterator != my_peers.end(); ++peer_iterator)
        downcall_route(peer_iterator->second.getId(), new_message);
    }
  }

  /////////////////////////////////////////////////////////////////////////////
  // Message from tcp layer with the results of a search.
  /////////////////////////////////////////////////////////////////////////////
  upcall (stateHasJoined())
      deliver(const MaceKey& source, const MaceKey& dest,
              const search_result &message, registration_uid_t foo) {
    OUTPUT << ENDL;
    OUTPUT << "(upcall) [joined] SEARCH_RESULT" << ENDL;
    
    OUTPUT << "got result: " << message << ENDL;
    
    upcallAllVoid(handleQueryResult, message.sequence_number, message.result);
  }

  /////////////////////////////////////////////////////////////////////////////
  // network error and we are a supernode
  /////////////////////////////////////////////////////////////////////////////
  upcall (state == supernode_joined)
      error(const MaceKey& nodeId, TransportError::type error_code,
            const std::string& message) {
    OUTPUT << ENDL;
    OUTPUT << "(upcall) [supernode] TRANSPORT_ERROR" << ENDL;
    OUTPUT << "from: " << nodeId << ENDL;
    OUTPUT << "error_code: " << error_code << ENDL;
    OUTPUT << "message: " << message << ENDL;
    
    // If I knew about this guy, I don't any more
    if (my_peers.contains(nodeId)) {
      // We detected an error with one of our peers
      my_peers.erase(nodeId);
      OUTPUT << "Got error " << error_code << " from peer " << nodeId
          << ": " << message << ". Removing from peer list." << ENDL;
    }
    else if (my_clients.contains(nodeId)) {
      // We detected an error with one of our clients
      my_clients.erase(nodeId);
      OUTPUT << "Got error " << error_code << " from client " << nodeId
          << ": " << message << ". Removing from client list." << ENDL;
    }
  }

  /////////////////////////////////////////////////////////////////////////////
  // network error and we are not a supernode
  /////////////////////////////////////////////////////////////////////////////
  upcall (state == client_joined)
      error(const MaceKey& nodeId, TransportError::type error_code,
            const std::string& message) {
    OUTPUT << ENDL;
    OUTPUT << "(upcall) [client] TRANSPORT_ERROR" << ENDL;
    OUTPUT << "from: " << nodeId << ENDL;
    OUTPUT << "error_code: " << error_code << ENDL;
    OUTPUT << "message: " << message << ENDL;
    
    // Error came from my supernode. I have no one to talk to anymore,
    // so I'm going to leave the network.
    if (my_supernode.getId() == nodeId) {
      OUTPUT << "Got error " << error_code << " from my supernode " << nodeId
             << ": " << message << ". Leaving the network." << ENDL;
      
      state = pre_joining;
      upcallAllVoid(joinStatusChanged, P2P_DISCONNECTED);

      return;
    }
  }

  /////////////////////////////////////////////////////////////////////////////
  // network message error
  /////////////////////////////////////////////////////////////////////////////
  upcall (true)
      messageError(MaceKey const &key, TransportError::type code,
                   std::string const & message) {
    OUTPUT << ENDL;
    OUTPUT << "(upcall) [any] MESSAGE_ERROR" << ENDL;
    
    OUTPUT << "-----FIX ME: Got message error " << code << ": " << message << ENDL;
  }
}


/* routines
 * Here you may define any routines used by your program.
 * These are simply c-functions, however thye have access to all
 * the auto-types, typedefs and messages you have defined.  Also
 * you may access any of your state variables from these routines
 */
routines {
  void becomeSupernode() {
    // If I was a client, tell my supernode I'm not his client any more
    if (state == client_joined)
      downcall_route(my_supernode.getId(), client_leave());
  
    state = supernode_joined; 
    upcallAllVoid(joinStatusChanged, P2P_SUPERNODE_JOINED);
    update_active_index_timer.reschedule(UPDATE_ACTIVE_INDEX_INTERVAL);
  }

  void joinTheNetwork(const MaceKey& peerToJoin) {
    state = client_joining;
    upcallAllVoid(joinStatusChanged, P2P_JOINING);
    
    client_join join_message(my_ipv4, MAX_JOIN_HOPS,
                             my_advertised_bandwidth_kbps, my_http_port);
    downcall_route(peerToJoin, join_message);

    num_join_attemps = 1;

    join_timer.reschedule(P2P_JOIN_TIMEOUT);
  }
  
  bool stateHasJoined() const {
    return state == client_joined || state == supernode_joined;
  }

  bool isFull(supernode_peers peers) {
    return peers.size() >= MAX_SUPERNODE_PEERS;
  }
  
  const char *currentStateString() {
    switch (state) {
    case (pre_joining):
      return "disconnected";
    case (client_joining):
      return "connecting";
    case (client_joined):
      return "client";
    case (supernode_joined):
      return "supernode";
    default:
      return "unknown";
    }
  }
}


/* Modelchecking properties ... do not worry about this */
properties
{
  safety {
  }

  liveness {
    // All nodes are clients or supernodes eventually
    nodesJoined : \forall n \in \nodes : { n.state = client_joined \or n.state = supernode_joined };    
  }
}
