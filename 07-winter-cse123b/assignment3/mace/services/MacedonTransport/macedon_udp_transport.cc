/* 
 * macedon_udp_transport.cc : part of the Mace toolkit for building distributed systems
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
#include <poll.h>
#include <fcntl.h>
#include <signal.h>
#include <stdarg.h>   
#include <math.h>   
#include <stdio.h>
#include <inttypes.h>
#include <strings.h>
#include <sys/time.h>    
#include <sys/uio.h>
#include <pthread.h>
#include "lib/Log.h"

#include "Accumulator.h"
#include "lib/mace.h"
#include "lib/Util.h"
#include "string.h"
#include <netinet/tcp.h>
#include "lib/Scheduler.h"
#include "macedon_udp_transport.h"

#ifdef PIP_MESSAGING
#include "annotate.h"
#endif

extern int num_fds;

macedon_udp_transport::macedon_udp_transport(int ipaddr, int port, 
			     int queue_size)
  : macedon_transport(ipaddr, port, queue_size), server_descriptor(- 1), time_last_cleanup(0.0)
{
  bzero(&server_socket, sizeof(server_socket));
  server_socket.sin_family = AF_INET;
  server_socket.sin_port = htons(port);
  server_socket.sin_addr.s_addr = ipaddr;

  if ( (server_descriptor = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    fprintf(stderr, "macedon_udp_transport: failure creating server socket port %d %s\n", port, strerror(errno));
    terminate(31);
  }
  int yes = 1;	  
  setsockopt(server_descriptor, SOL_SOCKET, SO_REUSEADDR,
	     (int *)&yes, sizeof(int));
  
  if (bind(server_descriptor, (struct sockaddr *) & server_socket,
	   sizeof (server_socket)) < 0) {
    fprintf(stderr, "macedon_udp_transport: failure binding server port %d %s\n", port, strerror(errno));
    terminate(32);
  }
  num_fds++;

  initialized = 1;
  Log::logf("macedon_udp_transport", "macedon_udp_transport bound on port %x, fd %d this %p\n", port, server_descriptor, this);
}

macedon_udp_transport::~macedon_udp_transport()
{
}


// ---------------------------------------------- 
// terminate
// ---------------------------------------------- 
void  macedon_udp_transport::terminate (int code)
{
  int did_lock = 0;
  if(lock_held != pthread_self()) { lock(); did_lock = 1;}
  if (server_descriptor != -1) {
    num_fds--;
    ::shutdown( server_descriptor, SHUT_RDWR);
    ::close(server_descriptor);
    server_descriptor = -1;
  }
  macedon_connection* sample = macedon_connections;
  macedon_connection* toDel;
  while (sample) {
    toDel = sample;
    sample = sample->next;
    delete toDel;
  }
  if(did_lock) { unlock(); }
  
  fflush(NULL);
  if (code > 0)
    exit(code);
  else if(code == 0)
    pthread_exit(NULL);
}


// ---------------------------------------------- 
// transport_disconnect
// ---------------------------------------------- 
int macedon_udp_transport::transport_disconnect (macedon_connection *neighbor)
{
  lock();
#ifdef TRANSPORT_TRACE
  Log::logf("macedon_udp_transport::trace", "transport_disconnect called for destination %.8x\n", neighbor->get_destination());
#endif
  neighbor->status = MACEDON_CONNECTION_STATUS_disconnected;
  unlock();
  return 0;
}


// ---------------------------------------------- 
// transport_connect
// ---------------------------------------------- 
int macedon_udp_transport::transport_connect (macedon_connection *neighbor)
{
  ASSERT(lock_held == pthread_self());
  //lock();
#ifdef TRANSPORT_TRACE
  Log::logf("macedon_udp_transport::trace", "transport_connect called for destination %.8x\n", neighbor->get_destination());
#endif
  neighbor->status  = MACEDON_CONNECTION_STATUS_connected;
  neighbor->time_last_used = Util::timeu();
  //unlock();
  return 0;
}



// ---------------------------------------------- 
// send_more
// ---------------------------------------------- 
int macedon_udp_transport::send_more (macedon_connection *neighbor)
{
  ASSERT(lock_held == pthread_self());
#ifdef TRANSPORT_TRACE
  Log::logf ("macedon_udp_transport::transport trace", "send destination %x status %d \n", neighbor->get_destination(), neighbor->status);
#endif

  if (!neighbor->send_queue) {
    return 1;
  }
  
  if (neighbor->status != MACEDON_CONNECTION_STATUS_connected) {
    transport_connect(neighbor);
  }
  
  int sent = 0;
  int descriptor = server_descriptor;

  while (neighbor->send_queue) {
    macedon_transport_send_work* work = neighbor->send_queue;

#ifdef TRANSPORT_TRACE
    Log::logf ("macedon_udp_transport::transport trace", "send going for %d bytes to %x \n", work-> running_size, neighbor->get_destination());
#endif
    //    dump_hex(work->running_buffer, work->running_size);

    //     int * dude = (int *) (work->stuff);
//      if (*dude > 4000) {
//        printf("Problem with header size when sending %d\n", dude);
//        exit(86);
//      }
    ASSERT(work->running_size < MACEDON_MAX_UDP_SIZE);
    server_socket.sin_addr.s_addr = neighbor->get_destination();
    sent = sendto (descriptor, work->running_buffer, work->running_size, 0, 
		   (const sockaddr *)&server_socket, sizeof(server_socket)); 
    int error = errno;
    neighbor->time_last_used = Util::timeu();
    if (sent < 0 && error != EAGAIN) {
      //perror("macedon_udp_transport: ");
      printf ("macedon_udp_transport: send error on socket to %x fd %d buf_addr %p size %d sockaddr %p :%s\n", neighbor->get_destination(), descriptor, work->running_buffer, work->running_size, &server_socket, strerror(error));
      terminate (46);
    }

#ifdef TRANSPORT_TRACE
   Log::logf ("macedon_udp_transport::transport trace", "send sent %d bytes to %x \n", sent, neighbor->get_destination());
#endif
    if (sent > 0) {
      neighbor->bytes_sent += sent;
      work->running_buffer += sent;
      work->running_size -= sent;
      if (work->running_size == 0) {  // done sending this work
        neighbor->items_sent ++;
        neighbor->pop_front();
        delete work;
      }
      Accumulator::Instance(Accumulator::NETWORK_WRITE)->accumulate(sent);
      Accumulator::Instance(Accumulator::UDP_WRITE)->accumulate(sent);
    } else if(error == EAGAIN) {
#ifdef TRANSPORT_TRACE
     Log::logf ("macedon_udp_transport::transport trace", "discarding message to %x \n", sent, neighbor->get_destination());
#endif
      if (work->running_size == 0) {  // done sending this work
        neighbor->items_sent ++;
        neighbor->pop_front();
        delete work;
      }
    }
  }
#ifdef TRANSPORT_TRACE
  Log::logf ("macedon_udp_transport::transport trace", "send done destination %x items left %d\n", neighbor->get_destination(), neighbor->send_queue_items);
#endif
  
  return 0;
}



// ---------------------------------------------- 
// wait_for_work 
// ---------------------------------------------- 
void macedon_udp_transport::wait_for_work  ()
{
 
#ifdef TRANSPORT_TRACE
  //  printf("macedon_udp_transport: waiting for work\n");
#endif
  // fflush(stdout);
  lock ();
  waiting_structure[0].fd = server_descriptor;
  waiting_structure[0].events =  POLLIN | POLLPRI;
  waiting_structure[0].revents = 0;
  unlock ();
#ifdef TRANSPORT_TRACE
  //  printf("macedon_udp_transport: polling on 1 descriptor\n");
#endif
  //  fflush(stdout);
  int status = poll (waiting_structure, 1, 500);
  if (status < 0 && errno != EINTR ) {
    perror("Polling error: \n");
    terminate(101);
  }
}



// ---------------------------------------------- 
// do_work 
// ---------------------------------------------- 
void macedon_udp_transport::do_work ()
{
  if (Util::timed() - time_last_cleanup > CLEANUP_TIME) {
    cleanup();
    time_last_cleanup = Util::timed();
    return;
  }

  if ((waiting_structure[0].revents & POLLERR) ||
      (waiting_structure[0].revents & POLLNVAL) ||
      (waiting_structure[0].revents & POLLHUP)) {
#ifdef TRANSPORT_TRACE
    printf ("macedon_udp_transport: error/hup on fd %d revents: %x\n", waiting_structure[0].fd, waiting_structure[0].revents);
#endif
    terminate(49);
  }
  else if (waiting_structure[0].revents & POLLIN ||
	   waiting_structure[0].revents & POLLPRI ||
	   waiting_structure[0].revents & POLLOUT) {
    actual_work();
  }	
  else  if (waiting_structure[0].revents ) {
    fprintf (stderr, "fd %d revents: %x\n", waiting_structure[0].fd, waiting_structure[0].revents);
  }	
}


// ---------------------------------------------- 
// cleanup
// ---------------------------------------------- 
void macedon_udp_transport::cleanup()
{
  lock();
  
  double now = Util::timed();
  macedon_connection* sample4 = macedon_connections;
  macedon_connection* oldest = sample4;
  double oldest_time = 0.0;
  if (sample4)
    oldest_time = sample4->time_last_used;
  while (sample4) {  
    int nuke=0;
    if (sample4->time_last_used < oldest_time) {
      oldest = sample4;
      oldest_time = sample4->time_last_used;
    }
    if ( now > sample4->time_last_used + TRANSPORT_TIMEOUT ) {
      if (sample4->status == MACEDON_CONNECTION_STATUS_connected) {
	if (!sample4->send_queue) {
	  if (!sample4->recv_in_progress) {
	    // nuke = 1;
	  }
	  else {
	    if (sample4->recv_in_progress->field_bytes ==0) {
	      // disable nuking for now
	      //	      nuke = 1;
	      delete sample4->recv_in_progress;
	      sample4->recv_in_progress = 0;
	    }
// 	    else 
// 	      printf("Wanted to nuke %.8x but could not cause recv queue %x\n", sample4->get_destination(), sample4->recv_in_progress);
	  }
	}
// 	else
//	  printf("Wanted to nuke %.8x but could not cause send queue %d %x\n", sample4->get_destination(), sample4->send_queue_items, sample4->send_queue);
      }
    }
    if (nuke) {
// #ifdef TRANSPORT_TRACE_CONNECT
      Log::logf("macedon_udp_transport::trace_connect", "%f nuking %.8x %x %p %p %" PRIu64 " fds %d\n", Util::timed(), sample4->get_destination(), port, sample4->send_queue, sample4->recv_in_progress, sample4->time_last_used, num_fds);
// #endif
    }
    sample4 = sample4->next;
  }
  
  if (num_fds > MAX_FDS) {
    Log::logf("macedon_udp_transport::trace_connect", "%f nuking oldest %.8x %f %p fds %d\n",  Util::timed(), oldest->get_destination(), oldest_time, oldest, num_fds);
  }

  unlock();
}

// ---------------------------------------------- 
// actual_work
// ---------------------------------------------- 
void macedon_udp_transport::actual_work ()
{
  lock ();
  recv_more (NULL);
  unlock ();
  return;
}

// ---------------------------------------------- 
// recv_more
// ---------------------------------------------- 
int macedon_udp_transport::recv_more (macedon_connection *neighbor)
{
  ASSERT(lock_held == pthread_self());
  // neighbor passed in is NULL always for UDP
  static int inst_count=0;
  inst_count ++;
  macedon_transport_recv_work *recv_work;

  int descriptor = server_descriptor;
  try {
    recv_work = new macedon_transport_recv_work();
    //	printf("alloced recv work element %x\n", recv_work);
  }
  catch (...) {
    printf("Exception: out of mem allocing new recv work\n");
    exit(97);
  }
  int flags;
  if (-1 == (flags = fcntl (descriptor, F_GETFL, 0)))
    flags = 0;
  fcntl (descriptor, F_SETFL, flags | O_NONBLOCK);

  recv_work->stuff = new unsigned char[MACEDON_MAX_UDP_SIZE];
  socklen_t arg = sizeof(server_socket);
  int bread = (int)::recvfrom(descriptor, ((char*)recv_work->stuff), MACEDON_MAX_UDP_SIZE, 0, (struct sockaddr *) &server_socket, &arg);
  if (bread <= 0 && errno != EWOULDBLOCK) { // there was nothing to read
    delete recv_work;
    return 1;
  }
  #ifdef PIP_MESSAGING
  int hsize = 4 * sizeof(int);
  #else
  int hsize = 2 * sizeof(int);
  #endif
  if (bread < hsize) {  // read error or did not read a complete header
    printf("macedon_udp_transport: receive error on fd %d %.8x %s\n", neighbor->descriptor, neighbor->get_destination(), strerror(errno));
    delete recv_work;
    terminate(50);
    return 1;
  }
  recv_work->header_size = *((int *)recv_work->stuff);
  recv_work->data_size = *((int *)(recv_work->stuff+sizeof(int)));
  int offset = 0;
  #ifdef PIP_MESSAGING
  recv_work->msgUid = *((int *)(recv_work->stuff+2*sizeof(int)));
  recv_work->psize =  *((int *)(recv_work->stuff+3*sizeof(int)));
  ANNOTATE_SET_PATH_ID(NULL, 0, recv_work->stuff + 4*sizeof(int), recv_work->psize);
  char msgId[36];
  sprintf(msgId, "%.8x-%.8x-%.8x-%.8x", server_socket.sin_addr.s_addr, here_addr_, port, recv_work->msgUid);
  ANNOTATE_RECEIVE(NULL, 0, msgId, 35, recv_work->header_size+recv_work->data_size);
  offset += 2*sizeof(int) + recv_work->psize;
  #endif
  recv_work->tot_size = recv_work->header_size + recv_work->data_size;

  unsigned char *temp = new unsigned char[recv_work->tot_size];
  bcopy(recv_work->stuff+2*sizeof(int)+offset, temp, recv_work->tot_size);
  delete[] recv_work->stuff;
  recv_work->stuff = temp;

#ifdef TRANSPORT_TRACE
  Log::logf("macedon_udp_transport::trace", "read all the data, %d bytes with %d header (%d this time), buff is %x\n", recv_work->tot_size, recv_work->header_size, bread, recv_work->stuff);
  fflush(stdout);
#endif
  neighbor = find_macedon_connection (server_socket.sin_addr.s_addr, 0);
  if (!neighbor)
    neighbor = new_macedon_connection (server_socket.sin_addr.s_addr, (int)server_socket.sin_port, 0);
  neighbor->items_received++;
  int mydest = neighbor->get_destination();
  neighbor->time_last_used = Util::timeu();
  macedon_transport_receiver* r = receiver;
  tr_handler h = receive_handler;
  if (receiver && receive_handler && recv_work->tot_size) {
    unlock();
#ifdef PIP_MESSAGING
    ANNOTATE_START_TASK(NULL, 0, "macedon_udp_transport::deliver");
#endif
    ( r->*h)
      (mydest,
       recv_work->stuff, recv_work->header_size,
       recv_work->stuff+recv_work->header_size, recv_work->tot_size-recv_work->header_size);
#ifdef PIP_MESSAGING
    ANNOTATE_END_TASK(NULL, 0, "macedon_udp_transport::deliver");
#endif
    lock();
  }
  delete recv_work;
  return 0;
}



