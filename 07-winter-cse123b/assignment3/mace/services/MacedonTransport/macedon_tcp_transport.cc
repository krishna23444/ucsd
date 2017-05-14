/* 
 * macedon_tcp_transport.cc : part of the Mace toolkit for building distributed systems
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
#include <poll.h>
#include <fcntl.h>
#include <signal.h>
#include <stdarg.h>   
#include <math.h>   
#include <stdio.h>   
#include <strings.h>
#include <sys/time.h>    
#include <sys/uio.h>
#include <pthread.h>
#include <inttypes.h>
#include <sstream>

#include "lib/mace.h"
#include "string.h"
#include "mstring.h"
#include <netinet/tcp.h>
#include "lib/Scheduler.h"
#include "macedon_tcp_transport.h"

#include "NetworkErrorHandler.h"
#include "tcp_auto_ext.h"
#include "Log.h"
#include "Util.h"
#include "Accumulator.h"
#include "pip_includer.h"

extern int num_fds;

using std::string;

void macedon_tcp_transport::signal_error(int tcp_error, int ipaddr, int port, int msg_count) {
  ASSERT(lock_held==pthread_self());

  if(from_agent) {
    //CK: We could include "deferred" as a signal.
    macedon_tcp_error* deferred_error = new macedon_tcp_error(tcp_error, ipaddr, port, msg_count);
    if(last_error == NULL) {
      ASSERT(first_error == NULL);
      last_error = deferred_error;
      first_error = deferred_error;
    } else {
      last_error->next_error = deferred_error;
      last_error = last_error->next_error;
    }
    return;
  }
  #ifdef PIP_MESSAGING
  static int error_notice = 0;
  ANNOTATE_SET_PATH_ID_STR("macedon_tcp_transport::tcp_error", 0, "tcp_err_%.8x_%u_%d", here_addr_, port, error_notice++);
  ANNOTATE_NOTICE("macedon_tcp_transport", 1, "macedon_tcp_transport:: signal_error %d on %.8x:%d msg_count=%d at %f\n",
      tcp_error, ipaddr, port, msg_count, Util::timed());
  #endif
  //   int did_lock = 0;
  macedon_transport_receiver* r = receiver;
  er_handler h = error_handler;
  unlock();
  Log::logf("macedon_tcp_transport::signal_error", "signal_error %d on %.8x:%d msg_count=%d at %f\n",
      tcp_error, ipaddr, port, msg_count, Util::timed());

  struct transport_error_struct arg;
  arg.transport_error = tcp_error;
  arg.dest_addr = ipaddr;
  arg.port = port;
  arg.known_lost = msg_count;

  if (r && h) {
#ifdef PIP_MESSAGING
    ANNOTATE_START_TASK(NULL, 0, "macedon_tcp_transport::error");
#endif
    ( r->*h)
      (0, (void*)&arg);
#ifdef PIP_MESSAGING
    ANNOTATE_END_TASK(NULL, 0, "macedon_tcp_transport::error");
#endif
  }
  lock();

}

void debug (int level, char *format, ... );

macedon_tcp_transport::macedon_tcp_transport(int ipaddr, int port, int queue_size)
  : macedon_transport(ipaddr, port, queue_size), server_descriptor(- 1), waiting_counter(0), time_last_cleanup(0.0), first_error(NULL), last_error(NULL)
{
  bzero(&server_socket, sizeof(server_socket));
  bzero(&waiting_structure, sizeof(waiting_structure));
  server_socket.sin_family = AF_INET;
  server_socket.sin_port = htons(port);
  server_socket.sin_addr.s_addr = here_addr_;

  if ( (server_descriptor = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    fprintf(stderr, "macedon_tcp_transport: failure creating server socket port %d: %s\n", port, strerror(errno));
    terminate(31);
  }
  int yes = 1;	  
  setsockopt(server_descriptor, SOL_SOCKET, SO_REUSEADDR,
      (int *)&yes, sizeof(int));

  // printf("setsockopt TCP_NODELAY!\n");
  // setsockopt(server_descriptor, IPPROTO_TCP, TCP_NODELAY, &yes,sizeof(int));
  if (bind(server_descriptor, (struct sockaddr *) & server_socket,
        sizeof (server_socket)) < 0) {
    fprintf(stderr, "macedon_tcp_transport: failure binding server port %d: %s\n",
	    port, strerror(errno));
    terminate(32);
  }
  num_fds++;

  if (listen (server_descriptor, 1000) != 0) {
    fprintf(stderr, "macedon_tcp_transport: failure listening on server port %d\n", port);
    terminate(33);
  }

//   keep_connecting = true;
//   pthread_cond_init(&connect_cond, NULL);
//   for(int i=0; i<NUM_CONNECT_THREADS; i++ ) {
//     pthread_t ctid;
//     if (pthread_create(&ctid, NULL, connecter, this) != 0) {
//       fprintf(stderr, "macedon_tcp_transport: pthread_create port %d %s", port, strerror(errno));
//       Log::logf("macedon_tcp_transport","Error in creating connect thread. port %d\n", port);
//       abort();
//       exit(81);
//     }
//   }

  initialized = 1;
  Log::logf("macedon_tcp_transport", "listening on port %d, fd %d this %p\n", port, server_descriptor, this);
}

macedon_tcp_transport::~macedon_tcp_transport()
{
}


// ---------------------------------------------- 
// terminate
// ---------------------------------------------- 
void  macedon_tcp_transport::terminate (int code)
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
  while( first_error) {
    macedon_tcp_error* delErr = first_error;
    first_error = first_error->next_error;
    delete delErr;
  }
  first_error = NULL;
  last_error = NULL;
  if(did_lock) { unlock(); }

  if (code > 0)
    exit(code);
  else if(code == 0)
    pthread_exit(NULL);
}


// ---------------------------------------------- 
// reset_connection
// ---------------------------------------------- 
int macedon_tcp_transport::reset_connection (macedon_connection *neighbor, int tcp_error)
{
  ASSERT(lock_held==pthread_self());
  int msg_count = 0;
  if (neighbor->status == MACEDON_CONNECTION_STATUS_disconnected)
    return 1;

#ifdef TRANSPORT_TRACE_CONNECT
  Log::logf("macedon_tcp_transport::trace_connect", "%f resetting %.8x port %d fd %d index %d status %d fds %d\n", Util::timed(), neighbor->get_destination(), port, neighbor->descriptor, neighbor->index, neighbor->status, num_fds);
#endif
  if(neighbor->descriptor >= 0) {
    num_fds--;
  }
  neighbor->close_descriptor();
  neighbor->status = MACEDON_CONNECTION_STATUS_disconnected;
  if (neighbor->send_queue) {
    //     msg_count + 1; //TODO: May need more here.
    neighbor->send_queue->running_buffer = neighbor->send_queue->stuff;  
    neighbor->send_queue->running_size = neighbor->send_queue->tot_size;
  }
  if (neighbor->recv_in_progress) {
    delete neighbor->recv_in_progress;
    neighbor->recv_in_progress = 0;
  }
  if(tcp_error && tcp_error != TCP_CLEANUP_KILLING_OLDEST && neighbor->index == 0) {
    signal_error(tcp_error, (int)neighbor->get_destination(), port, msg_count);
  }
  return 0;
}

// ---------------------------------------------- 
// abort_connection
// ---------------------------------------------- 
int macedon_tcp_transport::transport_abort (macedon_connection *neighbor)
{
  return reset_connection(neighbor, 0);
}


// ---------------------------------------------- 
// transport_disconnect
// ---------------------------------------------- 
int macedon_tcp_transport::transport_disconnect (macedon_connection *neighbor)
{
  ASSERT(lock_held==pthread_self());
#if 0
  reset_connection (neighbor);

  printf ("macedon_tcp_transport: %f disconnected %.8x fd %d fds %d\n", Util::timed(), neighbor->get_destination(), neighbor->descriptor, num_fds);
  return 0;
#endif

  if (neighbor->status == MACEDON_CONNECTION_STATUS_disconnected ) {
#ifdef TRANSPORT_TRACE_CONNECT
    Log::logf("macedon_tcp_transport::trace_connect", "%f %.8x %d index %d disconnecting but already disconnected %p fd %d fds %d\n", Util::timed(), neighbor->get_destination(), port, neighbor->index, neighbor, neighbor->descriptor, num_fds);
#endif
    return 0;
  }

  if (!neighbor->send_queue && !neighbor->recv_in_progress) {
    // Adolfo: could also delete the neighbor here
    reset_connection (neighbor, TransportError::NO_ERROR);
#ifdef TRANSPORT_TRACE_CONNECT
    Log::logf("macedon_tcp_transport::trace_connect", "%f %.8x %d index %d disconnected %p fd %d fds %d\n", Util::timed(), neighbor->get_destination(), port, neighbor->index, neighbor, neighbor->descriptor, num_fds);
#endif
  }
  else  {  // work still needs to be done
    neighbor->status = MACEDON_CONNECTION_STATUS_disconnecting;
#ifdef TRANSPORT_TRACE_CONNECT
    Log::logf ("macedon_tcp_transport::trace_connect", "%f %.8x %d index %d disconnecting %p fd %d fds %d\n", Util::timed(), neighbor->get_destination(), port, neighbor->index, neighbor, neighbor->descriptor, num_fds);
#endif
  }

  return 0;
}


// ---------------------------------------------- 
// transport_connect
// ---------------------------------------------- 
int macedon_tcp_transport::transport_connect (macedon_connection *neighbor)
{
  ASSERT(lock_held == pthread_self());
  //lock();
#ifdef TRANSPORT_TRACE
  Log::logf("macedon_tcp_transport::trace", "transport_connect called for destination %.8x at %f\n", neighbor->get_destination(),
	    Util::timed());
#endif
  neighbor->status  = MACEDON_CONNECTION_STATUS_connecting;
  neighbor->time_connect_initiated = Util::timeu();
//   pthread_cond_signal(&connect_cond);
  //unlock();

  neighbor->descriptor = socket(AF_INET, SOCK_STREAM, 0);
  if (neighbor->descriptor < 0) {
    std::cerr << "socket failed:" << std::endl;
    perror("socket");
    terminate(50);
  }

  int flags = 0;
  if (-1 == (flags = fcntl(neighbor->descriptor, F_GETFL, 0))) {
    perror("fcntl");
    terminate(51);
  }
  if (-1 == fcntl(neighbor->descriptor, F_SETFL, flags | O_NONBLOCK)) {
    perror("fcntl");
    terminate(52);
  }

  struct sockaddr_in sa;
  bzero(&sa, sizeof(sa));
  sa.sin_family = AF_INET;
  sa.sin_port = htons(port);
  sa.sin_addr.s_addr = neighbor->get_destination();
  num_fds++;
#ifdef TRANSPORT_TRACE_CONNECT
  Log::logf("macedon_tcp_transport::trace_connect", "%f trying to connect to %.8x %d %p %d fds %d\n", Util::timed(),
	    (int)sa.sin_addr.s_addr, port, neighbor, neighbor->descriptor, num_fds);
#endif
  ::connect(neighbor->descriptor, (struct sockaddr*)&sa, sizeof(sa));

  return 0;
}

// ---------------------------------------------- 
// connecter    
// separate thread to do the connect
// ---------------------------------------------- 
// void *connecter (void *input)
// {
//   macedon_tcp_transport *trans = (macedon_tcp_transport *)input;
//   trans->do_connect();
//   return NULL;
// }

// ---------------------------------------------- 
// do_connect
// ---------------------------------------------- 
// void macedon_tcp_transport::do_connect()
// {
//   lock();
//   while (keep_connecting) {
//     int check_time = -1;
//     macedon_connection* neighbor;
//     while ( (neighbor = find_macedon_connection_by_vtime(check_time)) != NULL) {
//       check_time = neighbor->virtual_time_initiated;
//       uint64_t entry=Util::timeu();
//       struct sockaddr_in socket_address;
//       bzero(&socket_address, sizeof(socket_address));
//       socket_address.sin_family = AF_INET;
//       neighbor->neigh_socket_address.sin_port = htons(port);
//       socket_address.sin_port = htons(port);
//       socket_address.sin_addr.s_addr = neighbor->get_destination();
//       if (neighbor->descriptor < 0 ) {
//         if ( (neighbor->descriptor = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
//           fprintf (stderr, "failure creating connect socket: %s\n", strerror(errno));
//           exit(90);
//         }
//       }
//       int descriptor =  neighbor->descriptor;
//       neighbor->status = MACEDON_CONNECTION_STATUS_connecting;
//       if (num_fds > MAX_FDS)
//         cleanup();
//       pthread_cond_signal(&connect_cond);
//       unlock();
// #ifdef TRANSPORT_TRACE_CONNECT
//         Log::logf("macedon_tcp_transport::trace_connect", "%f trying to connect to %.8x %d %x %d fds %d\n", Util::timed(), (int )socket_address.sin_addr.s_addr, ntohs((int )socket_address.sin_port), neighbor, descriptor, num_fds);
// #endif
//       int myret = ::connect(descriptor, (struct sockaddr *)  &socket_address,
//           sizeof(socket_address));
//       lock();
//       if (myret != 0) {
//         if ( neighbor->status != MACEDON_CONNECTION_STATUS_connecting ) {
// #ifdef TRANSPORT_TRACE_CONNECT
//             Log::logf ("macedon_tcp_transport::trace_connect", "connect failed to %.8x:%d  descriptor  %d %s time %f but neighbor status changed, so ignoring\n",	 
//                 (int )socket_address.sin_addr.s_addr,
//                 ntohs((int )socket_address.sin_port),
//                 descriptor,
//                 strerror(errno),
//                 Util::timed());
// #endif

//           continue;         
//         }
// #ifdef TRANSPORT_TRACE_CONNECT
//           Log::logf ("macedon_tcp_transport::trace_connect", "connect failed to %.8x:%d  descriptor  %d %s time %f\n",	 
//               (int )socket_address.sin_addr.s_addr,
//               ntohs((int )socket_address.sin_port),
//               descriptor,
//               strerror(errno),
//               Util::timed());
// #endif
//         neighbor->status = MACEDON_CONNECTION_STATUS_disconnected; //Since connrefused, now discconnected.
//         neighbor->time_connect_initiated = entry; //Set initiated time to now to move to end of queue.
//         neighbor->virtual_time_initiated = virtual_time++;
//         //Clear out the send queue on a connect failed.
//         int msg_count = 0;
//         while (neighbor->send_queue) {
//           msg_count++;
//           macedon_transport_send_work* work = neighbor->send_queue;
//           neighbor->pop_front();
//           delete work;
//         }
//         signal_error(TCP_CONNECT_FAILED, (int )socket_address.sin_addr.s_addr, port, msg_count);
//       }
//       else {
//         if (descriptor == neighbor->descriptor 
//             || neighbor->descriptor == -1) {
//           num_fds++;
//           neighbor->descriptor = descriptor;
//           int yes = 1;
//           setsockopt(neighbor->descriptor, SOL_SOCKET, SO_REUSEADDR,
//               (int *)&yes, sizeof(int));  
//           int minimum = 2*sizeof(int);
//           setsockopt(neighbor->descriptor, SOL_SOCKET, SO_RCVLOWAT,
//               (int *)&minimum, sizeof(int));  

//           //            int bs = 0;
//           //            setsockopt(neighbor->descriptor, SOL_SOCKET, SO_LINGER,
//           //                (int *)&bs, sizeof(bs));

//           //            int bs = 16384;
//           //            setsockopt(neighbor->descriptor, SOL_SOCKET, SO_SNDBUF,
//           //                (int *)&bs, sizeof(bs));
//           //  	  int new_len; 
//           //  	  int arglen = sizeof(new_len); 

//           //  	  if (-1 == getsockopt(neighbor->descriptor, SOL_SOCKET, SO_SNDBUF, &new_len,(socklen_t*)&arglen)) 
//           //  	    perror("getsockopt"); 


//           //  	  printf("setsb %d\n", new_len); 

//           // setsockopt(neighbor->descriptor, IPPROTO_TCP, TCP_NODELAY, &yes,sizeof(int));      
// #ifdef TRANSPORT_TRACE_CONNECT
//             Log::logf ("macedon_tcp_transport::trace_connect", "%f connected to %.8x:%d descriptor %d transport port %d time %f fds %d overall %f\n", Util::timed(), neighbor->get_destination(), ntohs((int)socket_address.sin_port), neighbor->descriptor, port, Util::timed()-entry, num_fds, Util::timed() - neighbor->time_connect_initiated);
// #endif
//           // shouldn't need to, but clean queues
//           if (neighbor->send_queue) {
//             neighbor->send_queue->running_buffer = neighbor->send_queue->stuff;  
//             neighbor->send_queue->running_size = neighbor->send_queue->tot_size;
//           }
//           if (neighbor->recv_in_progress) {
//             delete neighbor->recv_in_progress;
//             neighbor->recv_in_progress = 0;
//           }
//           neighbor->status = MACEDON_CONNECTION_STATUS_connected;
//         }
//         else {   // must have been accepted in the meantime
//           ::shutdown(descriptor,  SHUT_RDWR);
//           ::close( descriptor);
//         }
//       }
//     }
// #ifdef TRANSPORT_TRACE_LOCK
//     Log::logf("macedon_tcp_transport::lock", "%x cond_wait (release lock) on %x by transport %x held_by %d\n",pthread_self(),&pool_lock,this,lock_held);
// #endif
//     ASSERT(lock_held == pthread_self());
//     lock_held = 0;
//     pthread_cond_wait(&connect_cond, &pool_lock);
// #ifdef TRANSPORT_TRACE_LOCK
//     Log::logf("macedon_tcp_transport::lock", "%x cond_wait done (acquire lock) on %x by transport %x held_by %d\n",pthread_self(),&pool_lock,this,lock_held);
// #endif
//     lock_held = pthread_self();
//   }
//   unlock();
//   return;
// }


// ---------------------------------------------- 
// send_more
// ---------------------------------------------- 
int macedon_tcp_transport::send_more (macedon_connection *neighbor)
{
  ASSERT(lock_held == pthread_self());
#ifdef TRANSPORT_TRACE
  Log::logf ("macedon_tcp_transport::trace", "send destination %.8x status %d port %d index %d\n", neighbor->get_destination(), neighbor->status, port, neighbor->index);
  fflush(stdout);
#endif

  if (!neighbor->send_queue) {
    return 1;
  }
  if (neighbor->status == MACEDON_CONNECTION_STATUS_disconnecting) {
    neighbor->status = MACEDON_CONNECTION_STATUS_connected;
  }
  else if (neighbor->status == MACEDON_CONNECTION_STATUS_disconnected ) {
    transport_connect (neighbor);
    return 1;
  }
  else if (neighbor->status == MACEDON_CONNECTION_STATUS_connecting) {
    return 1;
  }
  //if(from_agent) return 0;

  int sent = 0;
  int descriptor = neighbor->descriptor;

  int keep_sending=1;
  while (keep_sending && neighbor->send_queue && (neighbor->status == MACEDON_CONNECTION_STATUS_connected || neighbor->send_queue->always_send) ) {
    macedon_transport_send_work* work = neighbor->send_queue;

#ifdef TRANSPORT_TRACE
    Log::logf ("macedon_tcp_transport::trace", "send going for %d bytes to %.8x port %d index %d\n", work-> running_size, neighbor->get_destination(), port, neighbor->index);
    //    dump_hex(work->running_buffer, work->running_size);
#endif

    int flags = 0;
    if (-1 == (flags = fcntl (descriptor, F_GETFL, 0)))
      flags = 0;
    fcntl (descriptor, F_SETFL, flags | O_NONBLOCK);    
    //     int * dude = (int *) (work->stuff);
    //      if (*dude > 4000) {
    //        printf("Problem with header size when sending %d\n", dude);
    //        exit(86);
    //      }
    sent = write (descriptor, work->running_buffer, work->running_size); 
    neighbor->time_last_used = Util::timeu();
    if (sent < 0 && errno!=EWOULDBLOCK) {
      Log::logf ("macedon_tcp_transport", "send error on socket to %.8x port %d index %d fd %d: %s\n", neighbor->get_destination(), port, neighbor->index, neighbor->descriptor, strerror(errno));
      reset_connection (neighbor, TransportError::WRITE_ERROR);
      //transport_connect(neighbor);
      return 1;
    }
    else if (sent < 0 && errno==EWOULDBLOCK)
      keep_sending = 0;

#ifdef TRANSPORT_TRACE
    Log::logf ("macedon_tcp_transport::trace", "send sent %d bytes to %.8x %d index %d\n", sent, neighbor->get_destination(), port, index);
#endif
    if (sent > 0) {
      Accumulator::Instance(Accumulator::TCP_WRITE)->accumulate(sent);
      neighbor->bytes_sent += sent;

      neighbor->outgoingBW.update(sent);
      neighbor->bufStatsOut->append(sent);

      work->running_buffer += sent;
      work->running_size -= sent;
      if (work->running_size == 0) {  // done sending this work
        neighbor->items_sent ++;
        neighbor->pop_front();
        delete work;
      }
    }
  }
#ifdef TRANSPORT_TRACE
  Log::logf ("macedon_tcp_transport::trace", "send done destination %.8x port %d index %d items left %d\n", neighbor->get_destination(), port, neighbor->index, neighbor->send_queue_items);
#endif

  return 0;
}



// ---------------------------------------------- 
// wait_for_work 
// ---------------------------------------------- 
void macedon_tcp_transport::wait_for_work  ()
{
//   printf("macedon_tcp_transport: waiting for work\n");

#ifdef TRANSPORT_TRACE
  //  printf("macedon_tcp_transport: waiting for work\n");
#endif
  // fflush(stdout);
  lock ();
  //we reserve slot zero for the listening socket
  waiting_counter = 0;
  waiting_structure[waiting_counter].fd = server_descriptor;
  waiting_structure[waiting_counter].events =  POLLIN;
  waiting_structure[waiting_counter].revents = 0;
  
  macedon_connection* sample2 = macedon_connections;
  while (sample2) {  
    if (sample2->status == MACEDON_CONNECTION_STATUS_connected ||
        sample2->status == MACEDON_CONNECTION_STATUS_disconnecting) {
      waiting_counter ++;
      if (waiting_counter > sizeof( waiting_structure)/sizeof(struct pollfd )) {
        fprintf (stderr, "counter exceeding maximum number of file descriptors: %d\n", waiting_counter);
        terminate(100);
      }
#ifdef TRANSPORT_TRACE
      Log::logf ("macedon_tcp_transport::trace", "counter %d sample2->descriptor %d destination %.8x\n", waiting_counter,sample2->descriptor, sample2->neigh_socket_address.sin_addr.s_addr);
#endif
      waiting_structure[waiting_counter].fd =  sample2->descriptor;
      if (sample2->send_queue)   // I have stuff to send for him
        waiting_structure[waiting_counter].events = POLLIN | POLLOUT | POLLPRI;
      else
        waiting_structure[waiting_counter].events = POLLIN | POLLPRI;
      if (waiting_structure[waiting_counter].fd >= MAXIMUM_DESCRIPTORS) {
        printf("Exception: too big a file descriptor %d\n", waiting_structure[waiting_counter].fd);
        exit(91);
      }
    }
    else if (sample2->status == MACEDON_CONNECTION_STATUS_connecting) {
      waiting_counter++;
      if (waiting_counter > sizeof( waiting_structure)/sizeof(struct pollfd )) {
        fprintf (stderr, "counter exceeding maximum number of file descriptors: %d\n", waiting_counter);
        terminate(100);
      }
#ifdef TRANSPORT_TRACE
      Log::logf ("macedon_tcp_transport::trace", "counter %d sample2->descriptor %d destination %.8x connecting\n", waiting_counter,sample2->descriptor, sample2->neigh_socket_address.sin_addr.s_addr);
#endif
      waiting_structure[waiting_counter].fd =  sample2->descriptor;
      waiting_structure[waiting_counter].events = POLLOUT;
      if (waiting_structure[waiting_counter].fd >= MAXIMUM_DESCRIPTORS) {
        printf("Exception: too big a file descriptor %d\n", waiting_structure[waiting_counter].fd);
        exit(91);
      }
    }
    else {
#ifdef TRANSPORT_TRACE
      Log::logf("macedon_tcp_transport::trace", "skip %.8x %d %d\n", sample2->get_destination(), sample2->status, sample2->descriptor);
#endif
    }
    sample2 = sample2->next;
  }
  unlock ();
#ifdef TRANSPORT_TRACE
  Log::logf("macedon_tcp_transport::trace", "macedon_tcp_transport: polling on %d descriptors\n", waiting_counter+1);
#endif
  //  fflush(stdout);
  int status = poll (waiting_structure, waiting_counter+1, 500);
  if (status < 0 && errno != EINTR ) {
    perror("Polling error: \n");
    terminate(101);
  }
}


// ---------------------------------------------- 
// accept_connections
// ---------------------------------------------- 
void macedon_tcp_transport::accept_connections ()
{
  int keep_accepting = 1;
  struct sockaddr_in client;
  socklen_t client_length = sizeof(sockaddr_in);
  int flags = 0;
  if (-1 == (flags = fcntl (server_descriptor, F_GETFL, 0)))
    flags = 0;
  fcntl (server_descriptor, F_SETFL, flags | O_NONBLOCK);  

  while (keep_accepting) {
    if (num_fds > MAX_FDS)
      cleanup();
    int descriptor = ::accept( server_descriptor, (sockaddr *) & client, &client_length);
    if (descriptor < 0) {
      if (errno != EWOULDBLOCK) {    // error
        printf ("Failure accepting a macedon_connection: %s\n", strerror(errno));
        exit(93);
      }
      else   // no harm no foul
        keep_accepting = 0;
    }
    else {
      //       int yes = 1;
      //  setsockopt(descriptor, IPPROTO_TCP, TCP_NODELAY, &yes,sizeof(int));
      int minimum = 4;	      
      setsockopt(descriptor, SOL_SOCKET, SO_RCVLOWAT,
          (int *)&minimum, sizeof(int));  

      //       int bs = 0;
      //       setsockopt(descriptor, SOL_SOCKET, SO_LINGER,
      //  		 (int *)&bs, sizeof(bs));

      //        int bs = 16384;
      //        setsockopt(descriptor, SOL_SOCKET, SO_SNDBUF,
      //  		 (int *)&bs, sizeof(bs));  

      int side = (int)client.sin_addr.s_addr;
      if ((side & htonl(0xff000000)) == htonl(0x0a000000))
      {
        //this is the Modelnet hack
        side &= ~(htonl(0x00800000)); 
      }
      lock ();

      macedon_connection* neighbor = find_macedon_connection (side, 0);
      if (neighbor && neighbor->status != MACEDON_CONNECTION_STATUS_disconnected) {
        if (here_addr_ < side) { 
          // HACK this is probably a bug, what happens if the connection is in connecting state, this is a potential race condition
          // deterministically pick which connection to keep so we don't bounce back and forth
#ifdef TRANSPORT_TRACE_CONNECT
          Log::logf ("macedon_tcp_transport::trace_connect", "%f peer already exists to %.8x %d, state %d, reusing connection %p, entry %p fds %d\n", Util::timed(), side, port, neighbor->status, this, neighbor, num_fds);
#endif
//           reset_connection(neighbor, TCP_PEER_ALREADY_EXISTS);
//           signal_error(TCP_PEER_ALREADY_EXISTS, (int)neighbor->get_destination(), port, 0);
	  if(neighbor->status == MACEDON_CONNECTION_STATUS_connecting) {
            //If it is connecting, we need to close the descriptor
            neighbor->close_descriptor();
#ifdef TRANSPORT_TRACE
            Log::logf("macedon_tcp_transport::trace", "peer %.8x %d was in connecting state, close connecting socket.\n", side, port);
#endif
          } else {
            //In this case -- shift this neighbor to a new index to finish delivering data.
            int index = find_available_index(neighbor->get_destination());
            neighbor->index = index;
#ifdef TRANSPORT_TRACE
            Log::logf("macedon_tcp_transport::trace", "peer %.8x %d was in connected state, assign new index %d and disconnect transport.\n", side, port, index);
#endif
            transport_disconnect(neighbor);
#ifdef NEW_BW_FILTER
            BandwidthTimeFilter& ibw = neighbor->get_filter(TransportServiceClass::Connection::IN);
            BandwidthTimeFilter& obw = neighbor->get_filter(TransportServiceClass::Connection::OUT);
#else
            BandwidthFilter& ibw = neighbor->get_filter(TransportServiceClass::Connection::IN);
            BandwidthFilter& obw = neighbor->get_filter(TransportServiceClass::Connection::OUT);
#endif
            neighbor = new_macedon_connection (side, (int)client.sin_port, 0);

            neighbor->set_filter(ibw, TransportServiceClass::Connection::IN);
            neighbor->set_filter(obw, TransportServiceClass::Connection::OUT);
          }
        }
        else if (neighbor->descriptor != -1 
            && neighbor->status != MACEDON_CONNECTION_STATUS_disconnected) {
#ifdef TRANSPORT_TRACE_CONNECT
          Log::logf ("macedon_tcp_transport::trace_connect", "%f peer already exists to %.8x %d, keeping the old one %p entry %p fds %d\n", Util::timed(), side, port, this, neighbor, num_fds);
#endif
          int index;
          macedon_connection* error_connection = new_macedon_connection_available_index(side, (int)client.sin_port, &index);
          num_fds++;
          Log::logf("macedon_tcp_transport", "creating error connection to %.8x, index=%d fd=%d\n", neighbor->neigh_socket_address.sin_addr.s_addr, index, descriptor);
          error_connection->status = MACEDON_CONNECTION_STATUS_connected;
          error_connection->descriptor = descriptor;
          //           signal_error(TCP_PEER_ALREADY_EXISTS_OTHER, (int)neighbor->get_destination(), port, 0); //we have no idea how much data could have been lost
          //::shutdown(descriptor,  SHUT_RDWR);
          //::close( descriptor);
          unlock(); //NOTE: Unlock before send, because send will re-lock
#ifdef PIP_MESSAGING
          static int instance = 0;
          ANNOTATE_SET_PATH_ID_STR("macedon_tcp_transport::connect error", 1, "error_connection-%.8x-%.8x-%.8x-%.8x", here_addr_, neighbor->get_destination(), port, instance++);
#endif
          send(side, index, false, NULL, 0, NULL, 0);
          continue;
        }
      }
      else if (!neighbor) {
        neighbor = new_macedon_connection (side, (int)client.sin_port, 0);
      }
      num_fds++;
      neighbor->descriptor = descriptor;
      // shouldn't need to, but clean queues
      if (neighbor->send_queue) {
        neighbor->send_queue->running_buffer = neighbor->send_queue->stuff;  
        neighbor->send_queue->running_size = neighbor->send_queue->tot_size;
      }
      if (neighbor->recv_in_progress) {
        delete neighbor->recv_in_progress;
        neighbor->recv_in_progress = 0;
      }
      neighbor->status = MACEDON_CONNECTION_STATUS_connected;

      unlock ();
#ifdef TRANSPORT_TRACE_CONNECT
        Log::logf ("macedon_tcp_transport::trace_connect", "accepted a macedon_connection from %.8x:%d fd %d my port %d this %p entry %p fds %d\n", side, client.sin_port, descriptor, port, this, neighbor, num_fds);
#endif
      fflush(stdout);
    }

  }
}


// ---------------------------------------------- 
// do_work 
// ---------------------------------------------- 
void macedon_tcp_transport::do_work ()
{

  lock ();	    
  while(first_error != NULL) {
    ASSERT(from_agent == 0);
    signal_error(first_error->tcp_error, first_error->ipaddr, first_error->port, first_error->msg_count);
    macedon_tcp_error* temp_error = first_error;
    first_error = first_error->next_error;
    delete temp_error;
    if(first_error == NULL) {
      last_error = NULL;
    }
  }
  unlock();

  //REB: what's up with this??

  if (Util::timed() - time_last_cleanup > CLEANUP_TIME) {
    cleanup();
    time_last_cleanup = Util::timed();
    return;
  }

  for (uint examine = 0; examine  <= waiting_counter ; examine++ ) {
    if (examine == 0) {
      //did we get any new macedon_connections?
      if (waiting_structure[examine].revents & POLLIN) 
      {
        accept_connections();
      }
      else if ((waiting_structure[examine].revents & POLLERR) ||
          (waiting_structure[examine].revents & POLLHUP)) {
        printf("Exception: TCP main server socket has a problem!\n");
        exit(94);
      }
    }
    else {
      if ((waiting_structure[examine].revents & POLLERR) ||
          (waiting_structure[examine].revents & POLLNVAL) ||
          (waiting_structure[examine].revents & POLLHUP)) {
#ifdef TRANSPORT_TRACE
        Log::logf ("macedon_tcp_transport::trace", "error/hup on fd %d waiting_structure[%d].revents: %x\n", waiting_structure[examine].fd, examine, waiting_structure[examine].revents);
#endif
        lock ();	    
        macedon_connection*neighbor = find_macedon_connection_by_fd(waiting_structure[examine].fd);
        if (neighbor) {
#ifdef TRANSPORT_TRACE
          Log::logf ("macedon_tcp_transport::trace", "poll error on %.8x fd %d\n", neighbor->get_destination(), neighbor->descriptor);
#endif
          if(neighbor->status == MACEDON_CONNECTION_STATUS_connecting) {
#ifdef TRANSPORT_TRACE_CONNECT
            Log::logf ("macedon_tcp_transport::trace_connect", "connect failed to %.8x:%d  descriptor  %d %s time %f\n",	 
                neighbor->get_destination(),
                port,
                neighbor->descriptor,
                strerror(errno),
                Util::timed());
#endif
            neighbor->status = MACEDON_CONNECTION_STATUS_disconnected; //Since connrefused, now discconnected.
            //Clear out the send queue on a connect failed.
            int msg_count = 0;
            while (neighbor->send_queue) {
              msg_count++;
              macedon_transport_send_work* work = neighbor->send_queue;
              neighbor->pop_front();
              delete work;
            }
            signal_error(TransportError::CONNECT_ERROR, neighbor->get_destination(), port, msg_count);
            num_fds--;
          } else {
            reset_connection(neighbor, TCP_POLL_ERROR);
          }
          //            transport_connect(neighbor);
        }
        else {
          num_fds--;
          ::shutdown( waiting_structure[examine].fd, SHUT_RDWR);
          ::close( waiting_structure[examine].fd );
        }
        unlock ();
      } 
      else if (waiting_structure[examine].revents & POLLIN ||
          waiting_structure[examine].revents & POLLPRI ||
          waiting_structure[examine].revents & POLLOUT) {
        //         if(TRANSPORT_MASSIVE_TRACE) {
        #ifdef TRANSPORT_MASSIVE_TRACE
          Log::logf("%f Poll called: IN: %d PRI: %d OUT: %d fd: %d\n", 
              Util::timed(),
              waiting_structure[examine].revents & POLLIN,
              waiting_structure[examine].revents & POLLPRI,
              waiting_structure[examine].revents & POLLOUT,
              waiting_structure[examine].fd);
        #endif
          //       }
        actual_work(waiting_structure[examine].fd, waiting_structure[examine].revents);
      }	
      else  if (waiting_structure[examine].revents ) {
        fprintf (stderr, "fd %d waiting_structure[%d].revents: %x\n", waiting_structure[examine].fd, examine, waiting_structure[examine].revents);
      }	
      else {
        //         if(TRANSPORT_MASSIVE_TRACE) {
        #ifdef TRANSPORT_MASSIVE_TRACE
          Log::logf("macedon_tcp_transport::massive_trace", "NO EVENTS for fd %d waiting_structure[%d]", waiting_structure[examine].fd, examine);
        #endif
          //         }
      }
    }
  }
}


// ---------------------------------------------- 
// cleanup
// ---------------------------------------------- 
void macedon_tcp_transport::cleanup()
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
      Log::logf("macedon_tcp_transport", "%f nuking %.8x %d index %d %p %p %" PRIu64 " fds %d\n", Util::timed(), sample4->get_destination(), port, sample4->index, sample4->send_queue, sample4->recv_in_progress, sample4->time_last_used, num_fds);
      // #endif
      reset_connection(sample4, TransportError::NO_ERROR);
    }
    sample4 = sample4->next;
  }

  if (num_fds > MAX_FDS) {
    Log::logf("macedon_tcp_transport", "%f nuking oldest %.8x %d index %d %f %p fds %d\n",  Util::timed(), oldest->get_destination(), port, oldest->index, oldest_time, oldest, num_fds);
    reset_connection (oldest, TCP_CLEANUP_KILLING_OLDEST);
  }

  macedon_connection* sample2 = macedon_connections;
  while (sample2) {  
    if (sample2->status == MACEDON_CONNECTION_STATUS_connected 
        || sample2->status == MACEDON_CONNECTION_STATUS_disconnecting) {
      send_more(sample2);
      if (sample2->status == MACEDON_CONNECTION_STATUS_connected 
          || sample2->status == MACEDON_CONNECTION_STATUS_disconnecting) 
        recv_more(sample2);
    }
    sample2 = sample2->next;
  }
  unlock();
}

// ---------------------------------------------- 
// actual_work
// ---------------------------------------------- 
void macedon_tcp_transport::actual_work (int desc, int pollflags)
{
  lock ();
  macedon_connection* receiver = find_macedon_connection_by_fd(desc);
  if (! receiver ) {
    printf ("Exception: macedon_tcp_transport: %f null peer in select %d\n", Util::timed(), desc);
    num_fds--;
    ::shutdown( desc, SHUT_RDWR);
    ::close( desc );
    unlock();
    return;
  }
  if (receiver->status == MACEDON_CONNECTION_STATUS_connecting) {
    int err;
    socklen_t size = sizeof(int);
    /*int r =*/ getsockopt(desc, SOL_SOCKET, SO_ERROR, &err, &size);
    if (err == 0) {
      if(TRANSPORT_TRACE_CONNECT) {
	Log::logf("macedon_tcp_transport::trace_connect::connect", "Connect accepted. Now connected to %.8x fd %d port %d\n", receiver->get_destination(), desc, port);
      }
      receiver->status = MACEDON_CONNECTION_STATUS_connected;
    }
    else {
      // error connecting

#ifdef TRANSPORT_TRACE_CONNECT
      Log::logf ("macedon_tcp_transport::trace_connect", "connect failed to %.8x:%d  descriptor  %d %s time %f\n",	 
		 receiver->get_destination(),
		 port,
		 desc,
		 strerror(errno),
		 Util::timed());
#endif
      receiver->status = MACEDON_CONNECTION_STATUS_disconnected; //Since connrefused, now discconnected.
      //Clear out the send queue on a connect failed.
      int msg_count = 0;
      while (receiver->send_queue) {
	msg_count++;
	macedon_transport_send_work* work = receiver->send_queue;
	receiver->pop_front();
	delete work;
      }
      signal_error(TransportError::CONNECT_ERROR, receiver->get_destination(), port, msg_count);
      num_fds--;
      unlock();
      return;
    }
  }
  if (pollflags & POLLOUT) {
    send_more (receiver);
  }
  if ((pollflags & POLLIN)) { /* && (receiver->status == MACEDON_CONNECTION_STATUS_connected 
			       || receiver->status == MACEDON_CONNECTION_STATUS_disconnecting)) {*/
    recv_more (receiver);
  }
  if (!receiver->send_queue &&
      !receiver->recv_in_progress &&
      receiver->status == MACEDON_CONNECTION_STATUS_disconnecting) {
    reset_connection(receiver, TransportError::NO_ERROR);
  }
  unlock ();
  return;
}

// ---------------------------------------------- 
// recv_more
// ---------------------------------------------- 
int macedon_tcp_transport::recv_more (macedon_connection *neighbor)
{
  ASSERT(lock_held == pthread_self());
  static int inst_count=0;
  inst_count ++;
  macedon_transport_recv_work *recv_work;

  int descriptor = neighbor->descriptor;
  int flags;
  if (-1 == (flags = fcntl (descriptor, F_GETFL, 0)))
    flags = 0;
  fcntl (descriptor, F_SETFL, flags | O_NONBLOCK);

  int keep_receiving=1;
  while (keep_receiving) {
    if (neighbor->recv_in_progress) {
      recv_work = neighbor->recv_in_progress;
      //      printf("recv was in progress %x %d %x %.8x\n", recv_work, inst_count, neighbor, neighbor->get_destination());
      neighbor->recv_in_progress = 0;
    }
    else {
      try {
        recv_work = new macedon_transport_recv_work();
        //	printf("alloced recv work element %x\n", recv_work);
      }
      catch (...) {
        printf("Exception: out of mem allocing new recv work\n");
        exit(97);
      }
    }

    #ifdef PIP_MESSAGING
    int sizesHeader = 4 * sizeof(int);
    #else
    int sizesHeader = 2 * sizeof(int);
    #endif

    if (recv_work->field_bytes < sizesHeader) { // sizes not read completely
      errno = 0;
      //printf("Trying to read %d bytes from %.8x on fd %d\n",2*sizeof(int)-recv_work->field_bytes, neighbor->neigh_socket_address.sin_addr.s_addr, descriptor);
      errno = 0;
      int bread = (int)::read(descriptor, ((char*)&recv_work->header_size)+recv_work->field_bytes, sizesHeader-recv_work->field_bytes);
      //printf("Read %d bytes\n", bread);
      if ((bread < 0 && errno != EWOULDBLOCK && errno != EAGAIN)
          || bread == 0) {  // read error
        Log::logf("macedon_tcp_transport", "receive error on fd %d %.8x port %d %s bread=%d errno=%d index=%d\n", neighbor->descriptor, neighbor->get_destination(), port, strerror(errno), bread, errno, neighbor->index);
        reset_connection(neighbor, TransportError::READ_ERROR);
        if(neighbor->index == 0) {
          //            transport_connect(neighbor);
        }
        delete recv_work;
        return 1;
      }
      else if (bread < 0) {  // must be EWOULDBLOCK , try later 
        //	printf("queueing recv 2 %x %d %x %.8x\n", recv_work, inst_count, neighbor, neighbor->get_destination()); 
        keep_receiving = 0;
        neighbor->recv_in_progress = recv_work;
      }
      else {    // read something 
        neighbor->incomingBW.update(bread);
	neighbor->bufStatsIn->append(bread);
        // Try to update the bytes on the zero index connection also
        if(neighbor->index != 0) {
          macedon_connection* index_zero = find_macedon_connection(neighbor->get_destination(), 0);
          if(index_zero != NULL) {
            index_zero->incomingBW.update(bread);
	    index_zero->bufStatsIn->append(bread);
	  }
        }
        if (bread > sizesHeader-recv_work->field_bytes) {
          printf ("Read in too much data to be headers %d\n", bread);
          exit(92);
        }
        recv_work->field_bytes+= bread;
        if (recv_work->field_bytes < sizesHeader) {
          // still not done
          //	  printf("queueing recv 3 %x %d \n", recv_work, inst_count, neighbor, neighbor->get_destination()); 
          neighbor->recv_in_progress = recv_work;
          continue;
        }
        else {  // read in the sizes completely
          neighbor->bytes_received +=sizesHeader;
          recv_work->tot_size = recv_work->header_size + recv_work->data_size;
          #ifdef PIP_MESSAGING
          recv_work->tot_size += recv_work->psize;
          #endif
          if (recv_work->tot_size != 0) {
            //             int trytoread = recv_work->tot_size-recv_work->running_size;
            //	    if (trytoread < 4000) 
            {
              recv_work->stuff = new unsigned char[recv_work->tot_size+100];  
              if (recv_work->stuff == 0) {
                Log::logf("macedon_tcp_transport", "Malloc exception 2 for size %p %d!\n", recv_work->stuff, recv_work->tot_size);
                reset_connection(neighbor, TransportError::READ_ERROR);
                /*                 abort(); */
                /*                 exit(72); */
              }
              recv_work->running_buffer = recv_work->stuff;
            }
          } else {
            if(neighbor->index == 0) {
              Log::logf ("macedon_tcp_transport", "%f peer socket to %.8x %d closed, expecting incoming connection.  transport %p entry %p fds %d\n", Util::timed(), neighbor->get_destination(), port, this, neighbor, num_fds);
              int index = find_available_index(neighbor->get_destination());
              //               signal_error(TCP_PEER_ALREADY_EXISTS, (int)neighbor->get_destination(), port, 0);
              neighbor->index = index;
              transport_disconnect(neighbor);
              new_macedon_connection(neighbor->get_destination(), htons(port), 0);
              //reset_connection(neighbor,TCP_PEER_ALREADY_EXISTS); 
            } else {
              transport_disconnect(neighbor);
            }
            keep_receiving = 0;
          }
        }
      }
    }

    if (keep_receiving) {    // try to read the rest of the data
      int trytoread = recv_work->tot_size-recv_work->running_size;
      //       if (trytoread > 4000) {
      // 	if (recv_work->running_size)
      // 	  dump_hex((void *)recv_work->stuff, recv_work->running_size);
      // 	printf("Exception: macedon_tcp_transport: %f trying to read way too much %d, sizes are %d %d %d %d %.8x\n", Util::timed(), trytoread, recv_work->header_size, recv_work->data_size, neighbor->descriptor, descriptor, neighbor->get_destination());
      // 	fflush(stdout);
      // 	terminate (110);
      //       }
      int bread = 0;
      if (trytoread) {
        //printf("Trying to read %d bytes from %.8x on fd %d\n", trytoread, neighbor->neigh_socket_address.sin_addr.s_addr, descriptor);
        errno = 0;
        bread =(int)::read(descriptor, recv_work->running_buffer, trytoread); 
        //printf("Read %d bytes\n", bread);
      }
      //    dump_hex((void *)recv_work->running_buffer, bread);
      if (bread > 0) {
        recv_work->running_buffer += bread;
        recv_work->running_size += bread;
        neighbor->bytes_received += (int)bread;
        neighbor->incomingBW.update(bread);
	neighbor->bufStatsIn->append(bread);
        // Also update the zero index neighbor also
        if(neighbor->index != 0) {
          macedon_connection* index_zero = find_macedon_connection(neighbor->get_destination(), 0);
          if(index_zero != NULL) {
            index_zero->incomingBW.update(bread);
	    neighbor->bufStatsIn->append(bread);
	  }
        }
      }
      else {  // read error
        if (bread < 0 && (errno == EWOULDBLOCK || errno == EAGAIN)) {  // try later
          keep_receiving = 0;
          bread = 0;
        }
        else {   // real error
          Log::logf("macedon_tcp_transport", "receive error on fd %d %.8x port %d %s bread=%d trytoread=%d errno=%d index=%d\n", neighbor->descriptor, neighbor->get_destination(), port, strerror(errno), bread, trytoread, errno, neighbor->index);
          // 	  printf("data received error on fd %d %.8x %s\n", neighbor->descriptor, neighbor->get_destination(), strerror(errno));
          reset_connection(neighbor, TransportError::READ_ERROR);
          //            transport_connect(neighbor);
          delete recv_work;
          return 1;
        }
      }
      if (recv_work->running_size != recv_work->tot_size) {
        keep_receiving = 0;
#ifdef TRANSPORT_TRACE
        Log::logf ("macedon_tcp_transport::trace", "did not read all the data, %d bytes so far (%d this time), %d were promised, queueing work %x %.8x:%d index %d fd %d\n", recv_work->running_size, bread, recv_work->tot_size, recv_work, neighbor->get_destination(), port, neighbor->index, descriptor);
        fflush(stdout);
#endif
        neighbor->recv_in_progress = recv_work;
      }	
      else {
        //         int trace=0;
        // 	if (recv_work->tot_size >= 18*sizeof(int)) {
        // 	  int * dude = (int *) (recv_work->stuff+17*sizeof(int));
        // 	  if (*dude == 900) // trace scribe anycast msgs
        // 	    dump_hex((void *)recv_work->stuff, recv_work->tot_size);
        // 	}
#ifdef TRANSPORT_TRACE
        Log::logf ("macedon_tcp_transport::trace", "read all the data, %d bytes with %d header (%d this time), buff is %x %x %.8x:%d index %d fd %d\n", recv_work->running_size, recv_work->header_size, bread, recv_work->stuff, recv_work->stuff+recv_work->header_size, neighbor->get_destination(), port, neighbor->index, descriptor);
#ifdef PIP_MESSAGING
        printf ("pip_msg: header_size %d data_size %d msgUid %d psize %d\n", recv_work->header_size, recv_work->data_size, recv_work->msgUid, recv_work->psize);
#endif
        fflush(stdout);
#endif
#ifdef TRANSPORT_MASSIVE_TRACE
        {
          int destination = neighbor->get_destination();
          int header_size = recv_work->header_size;
          int data_size = recv_work->running_size - recv_work->header_size;
#ifdef PIP_MESSAGING
          data_size -= recv_work->psize;
          int offset = recv_work->psize;
          printf("pip: pathId bytes %d ", recv_work->psize);
          dump_hex(recv_work->stuff, recv_work->psize);
#else
          int offset = 0;
#endif
          Log::logf("macedon_tcp_transport::massive_trace", "massive trace dump, recv from %.8x:%d hsize %d size %d\n", destination, port, header_size, data_size);
          Log::logf("macedon_tcp_transport::massive_trace", "header bytes %.8x:%d ", destination, port);
          dump_hex(recv_work->stuff, recv_work->header_size);
          Log::logf("macedon_tcp_transport::massive_trace", " data bytes %.8x:%d ", destination, port);
          dump_hex(recv_work->stuff + recv_work->header_size, recv_work->running_size - recv_work->header_size);
        }
#endif
        neighbor->items_received++;
        int mydest = neighbor->get_destination();
        neighbor->time_last_used = Util::timeu();
        if (receiver && recv_work->tot_size) {
          int offset = 0;
#ifdef PIP_MESSAGING
          ANNOTATE_SET_PATH_ID("macedon_tcp_transport::trace", 0, recv_work->stuff, recv_work->psize);
          char msgId[36];
          sprintf(msgId, "%.8x-%.8x-%.8x-%.8x", neighbor->get_destination(), here_addr_, port, recv_work->msgUid);
          ANNOTATE_RECEIVE(NULL, 0, msgId, 35, recv_work->header_size+recv_work->data_size);
          offset += recv_work->psize;
#endif
          if(recv_work->tot_size - offset <= 0) {
            if(neighbor->index == 0) {
              Log::logf("macedon_tcp_transport::trace_connect", "macedon_tcp_transport: %f peer socket to %.8x %d closed, expecting incoming connection.  transport %p entry %p fds %d\n", Util::timed(), neighbor->get_destination(), port, this, neighbor, num_fds);
              int index = find_available_index(neighbor->get_destination());
              //               signal_error(TCP_PEER_ALREADY_EXISTS, (int)neighbor->get_destination(), port, 0);
              neighbor->index = index;
              transport_disconnect(neighbor);
              new_macedon_connection(neighbor->get_destination(), htons(port), 0);
              //reset_connection(neighbor,TCP_PEER_ALREADY_EXISTS); 
            } else {
              transport_disconnect(neighbor);
            }
          } else {
            if (neighbor->status == MACEDON_CONNECTION_STATUS_connected) {
              macedon_transport_receiver* r = receiver;
              tr_handler h = receive_handler;
              if(receiver != NULL) {
                unlock();
#ifdef PIP_MESSAGING
		ANNOTATE_START_TASK(NULL, 0, "macedon_tcp_transport::deliver");
#endif
		( r->*h)
		  (mydest,
		   recv_work->stuff+offset, recv_work->header_size,
		   recv_work->stuff+offset+recv_work->header_size, recv_work->tot_size-recv_work->header_size-offset);
#ifdef PIP_MESSAGING
		ANNOTATE_END_TASK(NULL, 0, "macedon_tcp_transport::deliver");
#endif
                lock();
              }
            }
          }
        }
        //printf ("macedon_tcp_transport: timings (base->lock: %f, base->recv: %f, tcp->lock: %f)\n",(b-a),(c-b),(d-c));
        delete recv_work;
      }
    }
  }
  return 0;
}





