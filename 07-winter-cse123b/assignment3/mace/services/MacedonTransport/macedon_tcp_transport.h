/* 
 * macedon_tcp_transport.h : part of the Mace toolkit for building distributed systems
 * 
 * Copyright (c) 2005, Charles Killian, Adolfo Rodriguez, Dejan Kostic, James W. Anderson, Ryan Braud
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
#ifndef __tcp_transport
#define __tcp_transport
#include <stdlib.h>
#include <stdio.h>

#include <sys/types.h>
//#include <stropts.h>
#include <limits.h>
#include <poll.h>

#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <pthread.h>
#include <list>

#include "macedon_transport.h"
#include "TransportConnection.h"

#define CLEANUP_TIME 5.0
#define TRANSPORT_TIMEOUT 100.0

// #define NUM_CONNECT_THREADS 5

// void *connecter (void *);
struct macedon_tcp_error;

class macedon_tcp_transport : public macedon_transport
{
public:
  static const int MAXIMUM_DESCRIPTORS = 3000;	
  static const int PORT = 5377;
  static const double MAX_QUEUE_SIZE = 100;
  
  macedon_tcp_transport	( int ipaddr, int port, int queue_size );
  virtual ~macedon_tcp_transport ( );
public:
//   void do_connect();  
protected:
  struct sockaddr_in server_socket;  
  int server_descriptor;
  uint waiting_counter;
  double time_last_cleanup;
  macedon_tcp_error* first_error;
  macedon_tcp_error* last_error;
  struct pollfd waiting_structure[MAXIMUM_DESCRIPTORS];
  void actual_work(int desc, int pollflags);
  void cleanup ();
  int reset_connection (macedon_connection *, int);
  int connect_connection (macedon_connection *);
  void accept_connections();
  void signal_error(int, int, int, int);

  // virtual functions
  void wait_for_work ();
  void do_work ();
  void terminate (int code = 0);
  int send_more (macedon_connection *);
  int recv_more (macedon_connection *);
  virtual int transport_connect(macedon_connection *);
  virtual int transport_disconnect(macedon_connection *);
  virtual int transport_abort (macedon_connection *);
};

struct macedon_tcp_error {

  public: 
    int tcp_error;
    int ipaddr;
    int port;
    int msg_count;
    macedon_tcp_error* next_error;
    macedon_tcp_error(int t, int i, int p, int m) : tcp_error(t), ipaddr(i), port(p), msg_count(m), next_error(NULL) {}
};

#endif //__macedon_tcp_transport
