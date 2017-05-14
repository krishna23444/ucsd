/* 
 * TransportConnection.cc : part of the Mace toolkit for building distributed systems
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
#include <unistd.h>
#include <stdio.h>
// #include "mace.h"
#include "TransportConnection.h"
#include "Util.h"

macedon_transport_work::macedon_transport_work()
  : neighbor(NULL), stuff(0), tot_size(0), running_buffer(0), running_size(0)
{
}

macedon_transport_work::~macedon_transport_work()
{
  if (stuff == (unsigned char *)0xeeeeeeee) {
    printf("Something hosed with stuff\n");
    exit(97);
  }
  if (stuff) {
    delete[] stuff;
    stuff = 0;
  }
}

macedon_transport_send_work::macedon_transport_send_work()
  : next(NULL)
{
}

macedon_transport_send_work::~macedon_transport_send_work()
{
}

macedon_transport_recv_work::macedon_transport_recv_work()
  : field_bytes(0), header_size(0), data_size(0)
{
}

macedon_transport_recv_work::~macedon_transport_recv_work()
{
  //  printf ("recv work freeing %x in destruct\n", this);
  // fflush(stdout);
}


macedon_connection::macedon_connection(int destination, int in_index, int port, macedon_transport *intrans)
  : 
  trans(intrans),
  next(NULL),
  status(MACEDON_CONNECTION_STATUS_disconnected), 
  index(in_index),
  descriptor(-1), 
  time_last_used(Util::timeu()),
  time_connect_initiated (0),
  virtual_time_initiated(0),
  send_queue_items(0),
  send_queue(NULL),
  send_queue_last(NULL),
  recv_in_progress(NULL), 
  items_sent(0),
  items_received(0),
  items_rejected(0),
  items_queued(0), 
  bytes_sent(0),
  bytes_queued(0),
  bytes_received(0),
  bufStatsIn(new BufferStatistics(5*1000*1000)),
  bufStatsOut(new BufferStatistics(5*1000*1000))
{
  bzero(&neigh_socket_address, sizeof(neigh_socket_address));
  neigh_socket_address.sin_family = AF_INET;
  neigh_socket_address.sin_port = port;
  neigh_socket_address.sin_addr.s_addr = destination;
}

macedon_connection::~macedon_connection()
{
  close_descriptor ();
  status = MACEDON_CONNECTION_STATUS_disconnected;
  while (send_queue) {
    macedon_transport_work* unit = pop_front();
    delete unit;
  }
  if (recv_in_progress)
    delete recv_in_progress;
  recv_in_progress = 0;
}


// ---------------------------------------------- 
// close_descriptor
// ---------------------------------------------- 
int  macedon_connection::close_descriptor ()
{
  if (descriptor !=  - 1) {
    ::shutdown( descriptor, SHUT_RDWR);
    ::close( descriptor);
    descriptor =  - 1; 
    return 0;
  }
  return 1;
}

void macedon_connection::queue_back(macedon_transport_send_work *in) 
{
  if (send_queue) {
    send_queue_last->next = in;
  }
  else {
    send_queue = in;
  }
  macedon_transport_send_work *temp = in;
  send_queue_items++;
  bytes_queued += in->tot_size;
  while (temp->next) {
    send_queue_items++;
    temp = temp->next;
  }
  send_queue_last = temp;
}

void macedon_connection::queue_back_always(macedon_transport_send_work *in) 
{
  macedon_transport_send_work *temp = in;
  send_queue_items++;
  bytes_queued += temp->tot_size;
  while (temp->next) {
    send_queue_items++;
    bytes_queued += temp->tot_size;
    temp = temp->next;
  }
  if (send_queue) {
    if(send_queue->always_send) {
      temp = send_queue;
      while(temp->next && temp->next->always_send) { temp = temp->next; }
      in->next = temp->next;
      temp->next = in;
    }
    else {
      in->next = send_queue;
      send_queue = in;
    }
  }
  else {
    send_queue = in;
  }
  temp = in;
  while (temp->next) {
    temp = temp->next;
  }
  send_queue_last = temp;
}

void macedon_connection::queue_front(macedon_transport_send_work *in)
{
  macedon_transport_send_work *temp = in;
  send_queue_items++;
  while (temp->next) {
    send_queue_items++;
    bytes_queued += in->tot_size;
    temp = temp->next;
  }
  temp->next = send_queue;
  send_queue = in;
}

macedon_transport_send_work *macedon_connection::pop_front() 
{
  macedon_transport_send_work *temp = NULL;

  if (send_queue) {
    temp = send_queue;
    send_queue = send_queue->next;
    send_queue_items--;
  }

  return temp;
}

void macedon_connection::set_filter(
#ifdef NEW_BW_FILTER
const BandwidthTimeFilter& filter
#else
const BandwidthFilter& filter
#endif
, BandwidthDirection bd) {
  if (bd == TransportServiceClass::Connection::IN) {
    incomingBW = filter;
  }
  else {
    outgoingBW = filter;
  }
}

#ifdef NEW_BW_FILTER
BandwidthTimeFilter&
#else
BandwidthFilter&
#endif
macedon_connection::get_filter(BandwidthDirection bd) {
  return (bd == TransportServiceClass::Connection::IN ? incomingBW : outgoingBW);
}
