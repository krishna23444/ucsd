/* 
 * macedon_transport.h : part of the Mace toolkit for building distributed systems
 * 
 * Copyright (c) 2005, Charles Killian, Adolfo Rodriguez, Dejan Kostic
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
#ifndef __macedon_transport
#define __macedon_transport
#include "TransportConnection.h"
#include "mace.h"
#include "Log.h"

// #define TRANSPORT_TRACE 1
// #define TRANSPORT_TRACE_LOCK 0
#define TRANSPORT_TRACE_CONNECT 1
// #define TRANSPORT_MASSIVE_TRACE 0

#define MAX_FDS 1010

class  macedon_transport_receiver
{
};

void *worker_wrapper (void *);
 
// typedef  void ( macedon_transport_receiver::*macedon_recv_handler)
//   ( int, 
//     unsigned char *, int, 
//     unsigned char *, int);

typedef  void  ( macedon_transport_receiver::*tr_handler)
      ( int, 
       const unsigned char *, int, 
       const unsigned char *, int);

typedef  void  ( macedon_transport_receiver::*er_handler)
      ( int, 
        void* 
       );

class macedon_transport 
{
public:

  macedon_transport (int ipaddr, int port, 
		     int queue_size);
  virtual ~macedon_transport ( );

  // API functions go here
  int register_receiver (macedon_transport_receiver* receiver, 
			 tr_handler handler, er_handler er_handler_p);
  int send (int destination, 
	    const unsigned char *header_data, 
	    int header_size,
	    const unsigned char *data, 
	    int size);
  int send (int destination, 
      int index,
      bool always_send,
	    const unsigned char *header_data, 
	    int header_size,
	    const unsigned char *data, 
	    int size);
  int abort_destination (int destination);
  int close (int destination);
  int close (int destination, int index);
  void create_connection_if_needed (int destination, int index);
  bool has_room (int destination, int index);
  int queued (int destination, int index);
protected:
  int lock (); 
  int try_lock (); 
  void unlock ();
  macedon_connection* find_macedon_connection (int destination, int index);
  macedon_connection* find_macedon_connection_by_fd (int destination);
  int find_available_index(int destination);
  macedon_connection* new_macedon_connection (int destination, int useport, int index);
  macedon_connection* new_macedon_connection_available_index (int destination, int useport, int* index);
public:
  void *worker ();
  virtual int send_more (macedon_connection *) = 0;
  pthread_t thread_id;
  double get_bandwidth (int destination, BandwidthDirection bd);
  const BufferStatisticsPtr getStatistics(int dest,
					  TransportServiceClass::Connection::type d);
  size_t outgoingBufferedDataSize(int destination);

  void shutdown();
#ifdef NEW_BW_FILTER
  void start_bw_segment(int destination, BandwidthDirection bd);
  void finish_bw_segment(int destination, BandwidthDirection bd);
#endif

protected:
  // expected from the upper class
  virtual void wait_for_work () = 0;
  virtual void do_work () = 0;
  virtual int recv_more (macedon_connection *) = 0;
  virtual void terminate (int code = 0) = 0;
  virtual int transport_connect(macedon_connection *) = 0;
  virtual int transport_disconnect(macedon_connection *) = 0;
  virtual int transport_abort (macedon_connection *); //default method is empty

  // general stuff
  int initialized;
  pthread_mutex_t pool_lock; 
  pthread_t worker_thread;
  pthread_t lock_held; //the thread id of the holding thread
  int from_agent; //set to 1 when the calling thread is a thread from the agent.  (i.e. TCP defers sends)
  int here_addr_;       //bind address
  int port;        // sockets port
  int queue_size;   // 0 means unlimited queued pkts
  #ifdef PIP_MESSAGING
  int msgUid;
  #endif

  // receiver knowledge - make compatible with transport for now
  macedon_transport_receiver* receiver;
  tr_handler receive_handler;
  er_handler error_handler;

  // connection stuff
  macedon_connection *macedon_connections; 
  bool threadsExit;
  log_id_t macedonTransportLogId;
  //  list < macedon_connection*> macedon_connections; 
  //  hash_map < int, macedon_connection*, hash < int >, equal_integer> hash_macedon_connections;

};
#endif //__macedon_transport



