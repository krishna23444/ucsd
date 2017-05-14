/* 
 * macedon_transport.cc : part of the Mace toolkit for building distributed systems
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
#include "stdio.h"
#include "lib/mace.h"
#include "lib/Log.h"
#include "lib/SysUtil.h"
#include "lib/Util.h"
#include "macedon_transport.h"
#include "ThreadCreate.h"
#ifdef PIP_MESSAGING
#include "annotate.h"
#endif

int num_fds=0;

macedon_transport::macedon_transport(int ipaddr, int port1, 
		  int queue_size1)
  : initialized(0), lock_held(0), from_agent(0), here_addr_(ipaddr), port(port1), queue_size(queue_size1), 
  #ifdef PIP_MESSAGING
  msgUid(0),
  #endif
  receiver(NULL), receive_handler(NULL), error_handler(NULL), macedon_connections(0), threadsExit(false), macedonTransportLogId(Log::getId("macedon_transport::"+mace::convertToString(&port1, port1)))
{
  //pthread_mutexattr_t attr;
  //pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
  //pthread_mutex_init(&pool_lock, &attr);

  pthread_mutex_init(&pool_lock, NULL);

  //   int ret;

  Log::logf(macedonTransportLogId, "created; this = %p\n", this);
  // create the transport thread
  runNewThread(&worker_thread, worker_wrapper, this, NULL);
  SysUtil::signal ( SIGPIPE, SIG_IGN, false);
}
 

void macedon_transport::shutdown() 
{
  Log::log("macedon_transport::shutdown","macedon transport exiting\n");
  threadsExit = true;
}

macedon_transport::~macedon_transport()
{
}


//   API FUNCTIONS GO HERE

// ---------------------------------------------- 
// register_receiver
// ---------------------------------------------- 
int  macedon_transport::register_receiver (macedon_transport_receiver* receiver1, tr_handler handler1, er_handler handler2)
{
  lock();
  receive_handler = handler1;
  receiver = receiver1;
  error_handler = handler2;
  unlock();
  return 0;
}

// ---------------------------------------------- 
// send
// ---------------------------------------------- 
int  macedon_transport::send 
(
 int destination,
 const unsigned char *header_data, 
 int header_size,
 const unsigned char *data, 
 int size
 )
{
  return send(destination, 0, false, header_data, header_size, data, size);
}

int  macedon_transport::send 
(
 int destination,
 int index,
 bool always_send,
 const unsigned char *header_data, 
 int header_size,
 const unsigned char *data, 
 int size
 )
{
//        printf ("macedon_transport: send request destination  : %x %x %d %x %d queue %d port %d\n", destination, header_data, header_size, data,size,  queue_size, port);

  if (size<0 || header_size<0) {
    exit(89);
  }
//   if (destination == here_addr_) {
//     printf("Exception: trying to send to self %.8x\n", destination);
//     //     dump_hex (header_data, header_size);
//     //     dump_hex (data, size);
//     abort();
//     exit(99);
//   }

//    if (header_size + size > 4000) {
//      printf("Exception: what are you doing? %d %d\n", header_size, size);
//      exit(98);
//    }

#ifdef TRANSPORT_MASSIVE_TRACE
  {
    Log::logf("macedon_transport::massive_trace", "massive trace dump, send requested to %.8x:%d index %d hsize %d size %d\n", destination, port, index, header_size, size);
    Log::logf("macedon_transport::massive_trace", "header bytes %.8x:%d ", destination, port);
    dump_hex(header_data, header_size);
    Log::logf("macedon_transport::massive_trace", "data bytes %.8x:%d ", destination, port);
    dump_hex(data, size);
  }
#endif

  lock ();  
  from_agent = 1;
  macedon_connection* neighbor= find_macedon_connection (destination, index);
  if (!neighbor) {
    neighbor = new_macedon_connection (destination, htons(port), index);
  }
  
  if (queue_size !=0 &&
      neighbor->send_queue_items > queue_size)
    {
#ifdef TRANSPORT_TRACE
      Log::logf ("macedon_transport::trace", "send request destination rejected : %.8x %x %d %x %d items %d queue %d port %d\n", destination, header_data, header_size, data,size, neighbor->send_queue_items, queue_size, port);
#endif
  #ifdef PIP_MESSAGING
      ANNOTATE_NOTICE("macedon_transport::send", 0, "send request destination rejected : %.8x %p %d %p %d items %d queue %d port %d\n", destination, header_data, header_size, data,size, neighbor->send_queue_items, queue_size, port);
  #endif
      neighbor-> items_rejected++;
      neighbor->trans->send_more(neighbor);
      from_agent = 0;
      unlock();
      return 1;
    }

  //   if(TRANSPORT_TRACE) {
  #ifdef TRANSPORT_TRACE
      Log::logf ("macedon_transport::trace", "send request destination accepted : %.8x %x %d %x %d items %d queue %d port %d\n", destination, header_data, header_size, data,size, neighbor->send_queue_items, queue_size, port);
  #endif
      //   }

  size_t mysize =  header_size + size + 2*sizeof(int);
  #ifdef PIP_MESSAGING
  int psize;
  const void* pathId = ANNOTATE_GET_PATH_ID(&psize);
  mysize += sizeof(int) + psize; //For the path id
  mysize += sizeof(uint); //For the message id.
  #endif
  unsigned char *mydata = new unsigned char[mysize+100];
  if (mydata == NULL) {
    printf("Malloc exception 1 for size %p %zu\n", mydata, mysize);
    exit(72);
  }
  size_t currentPos = 0;
  *((int *)(mydata)) = header_size;
  currentPos += sizeof(int);
  *((int *)(mydata+currentPos)) = size;
  currentPos += sizeof(int);
  #ifdef PIP_MESSAGING
  *((int *)(mydata+currentPos)) = msgUid;
  currentPos += sizeof(int);
  *((int *)(mydata+currentPos)) = psize;
  currentPos += sizeof(int);
  bcopy(pathId, mydata+currentPos, psize);
  currentPos += psize;
  char msgId[36];
  sprintf(msgId, "%.8x-%.8x-%.8x-%.8x", here_addr_, destination, port, msgUid++);
  ANNOTATE_SEND("message_send"/*role*/, 0, msgId, 35, header_size+size);
#ifdef TRANSPORT_TRACE
  printf("pip_msg send: msgId %s header_size %d data_size %d msgUid %d psize %d\n", msgId, header_size, size, msgUid-1, psize);
#endif
  #endif
  if (header_size) {
    bcopy(header_data, mydata+currentPos, header_size);
    currentPos += header_size;
  }
  if (size) {
    bcopy( data, mydata+currentPos, size);
    currentPos += size;
  }
  
  try {
    macedon_transport_send_work* work = new macedon_transport_send_work ();
    work->always_send = always_send;
    work->tot_size =  mysize;
    work->running_size =  mysize;
    work->stuff = mydata;
    work->running_buffer = mydata;

//     if (work->tot_size >= 20*sizeof(int)) {
//       int * dude = (int *) (work->stuff+19*sizeof(int));
//       if (*dude == 900) // trace scribe anycast msgs 
// 	{
// 	  printf("sending anycast msg with sizes %d %d\n", header_size, size);
// 	  dump_hex((void *)work->stuff, work->tot_size);
// 	}
//     }
    
    if(always_send) {
      neighbor->queue_back_always(work);
    } else {
      neighbor->queue_back(work);
    }
  }
  catch (...) {
    printf("Exception: out of mem allocing new send work\n");
    exit(97);
  }

  neighbor->trans->send_more(neighbor);
  from_agent = 0;
  unlock ();
  
  return 0; //0 here means simply that the transport will handle the message from here out.
}


// ---------------------------------------------- 
// close
// ---------------------------------------------- 
int  macedon_transport::close (int destination)
{
  return close(destination, 0);
}

int  macedon_transport::close (int destination, int index)
{
  //  printf("macedon_transport: close destination: %x\n", destination);
  lock ();
  macedon_connection* neighbor= find_macedon_connection (destination, index);
  if (!neighbor) {
    Log::logf (macedonTransportLogId, "cannot find neighbor : %.8x index %d\n", destination, index);
    unlock ();
    return -1;
  }
  
  transport_disconnect(neighbor);
  unlock ();
  return 0;
}



int  macedon_transport::abort_destination (int destination)
{
  //  printf("macedon_transport: close destination: %x\n", destination);
  lock ();
  macedon_connection* neighbor= find_macedon_connection (destination, 0);
  if (!neighbor) {
    Log::logf (macedonTransportLogId, "cannot find neighbor : %.8x\n", destination);
    unlock ();
    return -1;
  }
  
  transport_abort(neighbor);
  unlock ();
  return 0;
}

int  macedon_transport::transport_abort (macedon_connection *neighbor)
{
  Log::logf("macedon_transport::transport_abort", "empty method\n");
  return 0;
}

// END OF API FUNCTIONS

// ---------------------------------------------- 
// try_lock
// ---------------------------------------------- 
int macedon_transport::try_lock ()
{
  ASSERT(lock_held != pthread_self());
  //   int old_type = 0;
  //   int old_state = 0;
#ifdef TRANSPORT_TRACE_LOCK
  Log::logf("macedon_transport::lock", "%d transport trylock %x for %x in held_by %d\n",pthread_self(),&pool_lock,this, lock_held);
#endif
  int ret = pthread_mutex_trylock( &pool_lock );
  if (ret == 0) {
    //pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, & old_type);
    //pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, & old_state);
    ASSERT(!lock_held);
    lock_held = pthread_self();
#ifdef TRANSPORT_TRACE_LOCK
    Log::logf("macedon_transport::lock", "%d transport trylock %x for %x out (held) held_by %d\n",pthread_self(),&pool_lock,this, lock_held);
#endif
  } else {
#ifdef TRANSPORT_TRACE_LOCK
    Log::logf("macedon_transport::lock", "%d transport trylock %x for %x out (not held) held_by %d\n",pthread_self(),&pool_lock,this,lock_held);
#endif
  }
  return ret;
}


// ---------------------------------------------- 
// lock
// ---------------------------------------------- 
int macedon_transport::lock ()
{
  ASSERT(lock_held != pthread_self());
  //   int old_type = 0;
  //   int old_state = 0;
#ifdef TRANSPORT_TRACE_LOCK
  Log::logf("macedon_transport::lock", "%d transport lock %x for %x held_by %d in\n",pthread_self(),&pool_lock,this, lock_held);
#endif
  pthread_mutex_lock( &pool_lock );
  //pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, & old_type);
  //pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, & old_state);
  ASSERT(!lock_held);
  ASSERT(!from_agent);
  lock_held = pthread_self();
#ifdef TRANSPORT_TRACE_LOCK
  Log::logf("macedon_transport::lock", "%d transport lock %x for %x held_by %d out\n",pthread_self(),&pool_lock,this, lock_held);
#endif
  return 1;
}


// ---------------------------------------------- 
// unlock
// ---------------------------------------------- 
void macedon_transport::unlock ()
{
#ifdef TRANSPORT_TRACE_LOCK
  Log::logf("macedon_transport::lock", "%d transport unlock %x for %x held_by %d in\n",pthread_self(),&pool_lock,this,lock_held);
#endif
  ASSERT(lock_held == pthread_self());
  ASSERT(!from_agent);
  //   int old_type = 0;
  //   int old_state = 0;
  lock_held = 0;
  pthread_mutex_unlock( &pool_lock );
#ifdef TRANSPORT_TRACE_LOCK
  Log::logf("macedon_transport::lock", "%d transport unlock %x for %x held_by %d out\n",pthread_self(),&pool_lock,this,lock_held);
#endif
  //pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, & old_type);
  //pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, & old_state);
  //pthread_testcancel();
}


// ---------------------------------------------- 
// create_connection_if_needed
// ---------------------------------------------- 
void macedon_transport::create_connection_if_needed (int destination, int index)
{
  lock ();  
  macedon_connection* neighbor= find_macedon_connection (destination, index);
  if (!neighbor) {
    neighbor = new_macedon_connection (destination, htons(port), index);
    transport_connect(neighbor); // CK: Ryan, is that what you wanted?
  }
  unlock ();  
}

// ---------------------------------------------- 
// has_room
// ---------------------------------------------- 
bool macedon_transport::has_room (int destination, int index)
{
  bool room = true;
  lock ();  
  macedon_connection* neighbor= find_macedon_connection (destination, index);
  if ((queue_size !=0 &&
       neighbor && neighbor->send_queue_items >= queue_size)) {
      room = false;
  }
  unlock ();  
  return room;
}

// ---------------------------------------------- 
// queued
// ---------------------------------------------- 
int macedon_transport::queued (int destination, int index)
{
  int status = 0;
  lock ();  
  macedon_connection* neighbor= find_macedon_connection (destination, index);
  if (neighbor)
    {
      status = neighbor->send_queue_items;
    }
  unlock ();  
  return status;
}

// ---------------------------------------------- 
// new_macedon_connection
// ---------------------------------------------- 
macedon_connection*  macedon_transport::new_macedon_connection (int destination, int useport, int index)
{
  macedon_connection* sample;
  //lock();
  ASSERT(lock_held==pthread_self());
  try {

    Log::logf(macedonTransportLogId, "created new connection for %s %d transport_port %d %p index %d\n", Util::getHostByAddr(destination).c_str(), ntohs(useport), port, this, index);
    
    sample = new macedon_connection(destination, index, useport, this);
    sample->next = macedon_connections;
    macedon_connections = sample;
  }
  catch (...) {
    printf("Exception: out of mem allocating new conn\n");
    exit(97);
  }

  //  hash_macedon_connections[destination]=(sample);
  //unlock();
  return sample;
}

int macedon_transport::find_available_index(int destination)
{
  macedon_connection *one=NULL, *two=NULL, *three=NULL;
  int last = 0;
  int index = -1;
  //lock();
  ASSERT(lock_held==pthread_self());
#if 1
  macedon_connection* sample2 = macedon_connections;
  while (sample2) {
    if (sample2-> get_destination() == destination && sample2->index == 1) {
      one = sample2;
    }
    else if (sample2-> get_destination() == destination && sample2->index == 2) {
      two = sample2;
    }
    else if (sample2-> get_destination() == destination && sample2->index == 3) {
      three = sample2;
    }
    if (sample2-> get_destination() == destination && sample2->index > last) {
      last = sample2->index;
    }
    sample2 = sample2->next;
    if (one == NULL) { index = 1; }
    else if (two == NULL) { index = 2; }
    else if (three == NULL) { index = 3; }
    else if (last < INT_MAX) { index = last+1; }
    else {
      printf("macedon_transport: Did not find valid transport index!\n");
      terminate(3421);
    }
  }
#else
  //was hash stuff for find_connection
  terminate(3452);
#endif
  //unlock();
  return index;
}

// ---------------------------------------------- 
// new_macedon_connection_available_index
//
// Note: Implementation not perfect.
// ---------------------------------------------- 
macedon_connection*  macedon_transport::new_macedon_connection_available_index (int destination, int useport, int *index)
{
  ASSERT(lock_held==pthread_self());
  //lock();
  *index = find_available_index(destination);
  macedon_connection *sample = new_macedon_connection(destination, useport, *index);
  //unlock();
  return sample;
}

// ---------------------------------------------- 
// find_macedon_connection
//
// Note: index is used to separate the "real" macedon_connection from ones created just to
// send an error message.  The only current use for this is to handle criss-cross error
// conditions properly.
// Note: An index of '0' is presently used to indicate the "real" macedon_connection.
// Note: In as much as is possible, the index should be hidden from the developer for now.
// ---------------------------------------------- 
macedon_connection*  macedon_transport::find_macedon_connection (int destination, int index)
{
  ASSERT(lock_held==pthread_self());
  //lock();
  macedon_connection* sample = NULL;
#if 1
  macedon_connection* sample2 = macedon_connections;
  while (sample2) {
    if (sample2-> get_destination() == destination && sample2->index == index) {
      sample = sample2;
      break; //CK added -- won't this be more efficient?
    }
    sample2 = sample2->next;
  }
#else
  sample = hash_macedon_connections[destination];
#endif
  //unlock();
  return sample;
}
#ifdef NEW_BW_FILTER

void macedon_transport::start_bw_segment(int destination, BandwidthDirection bd)
{
  lock();
  macedon_connection* conn = find_macedon_connection(destination, 0);
  Log::logf(macedonTransportLogId, "start_bw_segment called on %s, conn=%p\n", 
	    Util::getHostByAddr(destination).c_str(), conn);
  ASSERT(conn != NULL);
  if (bd == TransportServiceClass::Connection::IN) {
    conn->incomingBW.startUpdate();    
  }
  else {
    conn->outgoingBW.startUpdate();
  }
  unlock();
}

void macedon_transport::finish_bw_segment(int destination, BandwidthDirection bd)
{
  lock();
  macedon_connection* conn = find_macedon_connection(destination, 0);
  ASSERT(conn != NULL);
  if (bd == TransportServiceClass::Connection::IN) {
    conn->incomingBW.finishUpdate();    
  }
  else {
    conn->outgoingBW.finishUpdate();
  }
  unlock();
}

#endif
// ---------------------------------------------- 
// get_bandwidth
// ---------------------------------------------- 
double macedon_transport::get_bandwidth(int destination, BandwidthDirection bd)
{
  double result = 0;

  lock();
  macedon_connection* conn = find_macedon_connection(destination, 0);
// printf("%f get_bandwidth called on %.8x, conn=%.8x\n", wall_clock(), destination, conn);
  if(conn != NULL) {
    if (bd == TransportServiceClass::Connection::IN) {
      result = conn->incomingBW.getValue();    
    }
    else {
      result = conn->outgoingBW.getValue();
    }
    unlock();
    return result;
  }
  else {
    unlock();
    return -1;
  }
}

const BufferStatisticsPtr macedon_transport::getStatistics(int dest,
							   TransportServiceClass::Connection::type d) {
  lock();
  macedon_connection* conn = find_macedon_connection(dest, 0);
  BufferStatisticsPtr r;
  if (conn != NULL) {
    if (d == TransportServiceClass::Connection::IN) {
      r = conn->bufStatsIn;
    }
    else {
      assert(d == TransportServiceClass::Connection::OUT);
      r = conn->bufStatsOut;
    }
  }
  unlock();
  return r;
}

size_t macedon_transport::outgoingBufferedDataSize(int destination) {
  lock();
  size_t r = 0;
  macedon_connection* conn = find_macedon_connection(destination, 0);
  if (conn != NULL) {
    r = conn->bytes_queued - conn->bytes_sent;
    ASSERT(r >= 0);
  }
  unlock();
  return r;
}

// ---------------------------------------------- 
// find_macedon_connection_by_fd
// ---------------------------------------------- 
macedon_connection* macedon_transport::find_macedon_connection_by_fd (int descriptor)
{
  ASSERT(lock_held==pthread_self());
  //lock();
  macedon_connection* sample2 = macedon_connections;
  while (sample2) {
    if ((int)sample2-> descriptor == descriptor) {
      //unlock();
      return sample2;
    }
    sample2 = sample2->next;
  }
  //unlock();
  return NULL;
}

// ---------------------------------------------- 
// worker_wrapper
// ---------------------------------------------- 
void* worker_wrapper(void *temp)
{
  macedon_transport *this_transport = (macedon_transport *)temp;
  this_transport->worker();
  delete this_transport;
  pthread_exit(NULL);
  return NULL;
}


// ---------------------------------------------- 
// worker
// ---------------------------------------------- 
void* macedon_transport::worker()
{
  thread_id = pthread_self();
  Log::logf("macedon_transport::trace", "created macedon transport thread %lu\n", pthread_self());
  while (!initialized)
    usleep(100000);
  while(threadsExit == false )
    {
      wait_for_work ();

      if (threadsExit == false)
	do_work ();
    }
  terminate(-1);
  return NULL;
}
