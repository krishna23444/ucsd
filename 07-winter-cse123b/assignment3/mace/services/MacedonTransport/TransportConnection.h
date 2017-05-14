/* 
 * TransportConnection.h : part of the Mace toolkit for building distributed systems
 * 
 * Copyright (c) 2005, Charles Killian, Dejan Kostic, Ryan Braud, James W. Anderson, John Fisher-Ogden, Calvin Hubble, Duy Nguyen, Justin Burke, Amin Vahdat, Adolfo Rodriguez, Sooraj Bhat
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
#ifndef _TRANSPORT_CONNECTION
#define _TRANSPORT_CONNECTION

#define NEW_BW_FILTER

#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#ifdef NEW_BW_FILTER
#include "BandwidthTimeFilter.h"
#else
#include "BandwidthFilter.h"
#endif
#include "pip_includer.h"
#include "BufferedTransportServiceClass.h"
#include "BandwidthTransportServiceClass.h"
// Adolfo Rodriguez
// transport-independent macedon "connection", one per neighbor

class macedon_transport;
class macedon_connection;

class macedon_transport_work
{
public:
  macedon_transport_work ( );
  virtual ~macedon_transport_work ( );
  macedon_connection *neighbor;
  unsigned char * stuff;
  int tot_size;
  unsigned char * running_buffer;
  int running_size;
};

class macedon_transport_send_work : public macedon_transport_work
{ 
public:
  macedon_transport_send_work	( );
  ~macedon_transport_send_work	( );
  class macedon_transport_send_work *next;
  bool always_send;
};

class macedon_transport_recv_work : public macedon_transport_work
{	
public:
  macedon_transport_recv_work	( );
  ~macedon_transport_recv_work	( );
  int field_bytes;
  // the following two fields must remain in order always
  int header_size;
  int data_size;
  #ifdef PIP_MESSAGING
  int msgUid;
  int psize;
  char* pip_path_id;
  #endif
};

enum MACEDON_CONNECTION_STATUS
{
  MACEDON_CONNECTION_STATUS_connecting,
  MACEDON_CONNECTION_STATUS_connected,
  MACEDON_CONNECTION_STATUS_disconnecting,
  MACEDON_CONNECTION_STATUS_disconnected,
}
;

class macedon_connection
{	
public:
  macedon_connection (int destination, int index, int port, macedon_transport *intrans);
  virtual ~macedon_connection( ); 
  int get_destination () {
    return neigh_socket_address.sin_addr.s_addr;
  }
  int close_descriptor ();
  void queue_back(macedon_transport_send_work *);
  void queue_back_always(macedon_transport_send_work *);
  void queue_front(macedon_transport_send_work *);
  macedon_transport_send_work *pop_front();

  macedon_transport *trans;
  class macedon_connection *next;
  MACEDON_CONNECTION_STATUS status;
  int index;
  int descriptor;
  uint64_t time_last_used;
  uint64_t time_connect_initiated;
  int virtual_time_initiated;
  struct sockaddr_in neigh_socket_address;  
  int send_queue_items;
  macedon_transport_send_work * send_queue;
  macedon_transport_send_work * send_queue_last;
  macedon_transport_recv_work *recv_in_progress;

  // statistics
  int items_sent;
  int items_received;
  int items_rejected;
  int items_queued;
  int bytes_sent;
  int bytes_queued;
  int bytes_received;
#ifdef NEW_BW_FILTER
  BandwidthTimeFilter incomingBW;
  BandwidthTimeFilter outgoingBW;
  void set_filter(const BandwidthTimeFilter& filter, BandwidthDirection bd);
  BandwidthTimeFilter& get_filter(BandwidthDirection bd);
#else
  BandwidthFilter incomingBW;
  BandwidthFilter outgoingBW;
  void set_filter(const BandwidthFilter& filter, BandwidthDirection bd);
  BandwidthFilter& get_filter(BandwidthDirection bd);
#endif
  BufferStatisticsPtr bufStatsIn;
  BufferStatisticsPtr bufStatsOut;
};
#endif //_TRANSPORT_CONNECTION
