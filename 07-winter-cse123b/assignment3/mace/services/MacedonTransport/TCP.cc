/* 
 * TCP.cc : part of the Mace toolkit for building distributed systems
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
#include "Util.h"
#include "Log.h"
#include "NumberGen.h"
#include "ServiceInitializer.h"
#include "Accumulator.h"
#include "mace_constants.h"
#include "MaceTypes.h"

#include "tcp_auto_ext.h"
#include "TCP.h"

using std::string;

TransportServiceClass *tcp_init_function() {
  return new TCP_namespace::TCPService();
}

int TCP_load_protocol()
{
  //     TCPService::PT_tcp = protocol_num;
  ServiceInitializer < TransportServiceClass * >::Instance()->registerService("TCPService", tcp_init_function);
  return 0;
}

namespace TCP_namespace {

// int TCPService::PT_tcp;
TransportServiceClass& new_TCP_Transport(int queue_size, int port, int ipaddr)
{
  return (*new TCPService(queue_size, port, ipaddr));
}
BandwidthTransportServiceClass& new_TCP_BandwidthTransport(int queue_size, int port, int ipaddr)
{
  return (*new TCPService(queue_size, port, ipaddr));
}
BufferedTransportServiceClass& new_TCP_BufferedTransport(int queue_size, int port, int ipaddr)
{
  return (*new TCPService(queue_size, port, ipaddr));
}

TCPService::TCPService(int queue_size, int port, int ipaddr) : inited(0) {

  pthread_mutex_init(&mapLock, 0);
  if(ipaddr == INADDR_ANY) {
    ipaddr = Util::getAddr();
  }
  localIP = ipaddr;
  localKey = MaceKey(ipv4, localIP);
  if(port < 0) {
    port = Util::getPort() + NumberGen::Instance(NumberGen::PORT)->GetVal();
  }
  //contents: do I need to initialize it somehow?
  Log::logf("TCPService::TCPService", "local_address=%.8x port=%d this=%p\n", 
	    ipaddr, port, this);
  Log::log("TCPService::MaceKey") << localKey << Log::endl;
  trans = new macedon_tcp_transport(ipaddr, port, queue_size);
  trans->register_receiver(this, (tr_handler)&TCPService::recv_from_pipe, (er_handler)&TCPService::transport_error);
}

void TCPService::maceInit() { 
  inited++;
}

void TCPService::maceExit() { 
  inited--;
  if(inited == 0 && trans != NULL) {
    //cleanup macedon_tcp_transport
    trans->register_receiver(NULL, NULL, NULL);
    trans->shutdown();
    trans = NULL;
  }
}

registration_uid_t TCPService::registerHandler(ReceiveDataHandler& handler, registration_uid_t handlerUid) {
  Log::logf("TCPService::registerHandler", "Registering handler with id %d and memory %p on this %p\n", handlerUid, &handler, this);
  if(handlerUid == -1) {
    handlerUid = NumberGen::Instance(NumberGen::HANDLER_UID)->GetVal();
  }
  lock();
  handlers[handlerUid] = &handler;
  unlock();
  return handlerUid;
}

registration_uid_t TCPService::registerHandler(NetworkErrorHandler& handler, registration_uid_t handlerUid) {
  if(handlerUid == -1) {
    handlerUid = NumberGen::Instance(NumberGen::HANDLER_UID)->GetVal();
  }
  lock();
  network_handlers[handlerUid] = &handler;
  unlock();
  return handlerUid;
}

registration_uid_t TCPService::registerHandler(ConnectionStatusHandler& h,
					       registration_uid_t handlerUid) {
  // XXX: unimplemented
  return 0;
}

void TCPService::registerUniqueHandler(ReceiveDataHandler& handler) {
  Log::logf("TCPService::registerHandler", "Registering unique handler with id -1 and memory %p on this %p\n", &handler, this);
  lock();
  handlers[-1] = &handler;
  unlock();
}

void TCPService::registerUniqueHandler(NetworkErrorHandler& handler) {
  lock();
  network_handlers[-1] = &handler;
  unlock();
}

void TCPService::registerUniqueHandler(ConnectionStatusHandler& handler) {
  // XXX: unimplemented
}

void TCPService::unregisterHandler(ReceiveDataHandler& handler, registration_uid_t handlerUid) {
  Log::logf("TCPService::unregisterHandler", "UnRegistering handler with id %d and memory %p on this %p\n", handlerUid, &handler, this);
  lock();
  handlers.erase(handlerUid);
  unlock();
}

void TCPService::unregisterHandler(NetworkErrorHandler& handler, registration_uid_t handlerUid) {
  lock();
  network_handlers.erase(handlerUid);
  unlock();
}

void TCPService::unregisterHandler(ConnectionStatusHandler& h, 
				   registration_uid_t handlerUid) {
  // XXX: unimplemented
}

bool TCPService::route(const MaceKey& dest, const string& s, registration_uid_t handlerUid) {
  ADD_SELECTORS("TCPService::route");
  //   #warning "The MACEDON Transports do not support multiple-port experiments"
  ASSERT(localKey.getMaceAddr().local.port == dest.getMaceAddr().local.port);
  maceDebug(1, "dest=%s size=%zu priority=%d handlerUid=%d\n", dest.toString().c_str(), s.size(), 0, handlerUid);
  lock();
  if(handlers.find(handlerUid) == handlers.end()) { maceWarn("No handler registered with that uid, ignoring route\n"); unlock(); return false; }
  unlock();
  mace_tcp_header hdr;
  hdr.registrationUid = handlerUid;
  return !(maceRoute(dest.getMaceAddr().local.addr, hdr, s.data(), s.size()));
}

int TCPService::maceRoute(int nextHop, mace_tcp_header& hdr, const char* str, int size) {
  int retval = (trans == NULL?1:trans->send(nextHop, (const unsigned char*)&hdr, sizeof(mace_tcp_header), (const unsigned char*)str, size));
  if(retval == 0) {
    //Only accumulate actually successfully "sent" data
    Accumulator::Instance(Accumulator::TRANSPORT_SEND)->accumulate(size); //data + handlerUid + macedon_tcp_transport header ints
  }
  return retval;
}

void TCPService::lock() const { assert(pthread_mutex_lock(&mapLock) == 0); }
void TCPService::unlock() const { assert(pthread_mutex_unlock(&mapLock) == 0); }

void TCPService::recv_from_pipe(int from, const unsigned char * hdata, int hsize, const unsigned char * data, int size) {
  ADD_SELECTORS("TCPService::recv_from_pipe");
  string msg((char*)data, size);
  MaceKey fr(ipv4, from);
  maceDebug(1, "from=%s hsize=%d dsize=%d\n", fr.toString().c_str(), hsize, size);
  Accumulator::Instance(Accumulator::TRANSPORT_RECV)->accumulate(size); //data+ header + macedon_tcp_transport header ints
  mace_tcp_header hdr = *(mace_tcp_header*)hdata;
  lock();
  if(handlers.find(hdr.registrationUid) == handlers.end()) { maceWarn("Could not find handler with id %d : dropping packet\n", hdr.registrationUid); unlock(); return; }
  ReceiveDataHandler* handler = handlers[hdr.registrationUid];
  unlock();
  MaceKey source = MaceKey(ipv4, from);
  handler->deliver(source, localKey, msg, hdr.registrationUid);
  maceDebug(1, "ending.\n");
}

int TCPService::transport_error(int operation, void* arg) {
  mace::map<int, NetworkErrorHandler*>::iterator i;
  struct transport_error_struct *targ = (transport_error_struct*)arg;
  
  lock();
  for(i = network_handlers.begin(); i != network_handlers.end(); ) {
    NetworkErrorHandler* handler = i->second;
    registration_uid_t regId = i->first;
    i++;
    unlock();
    handler->error(MaceKey(ipv4, targ->dest_addr), targ->transport_error, "", regId);
    lock();
  }
  unlock();
  return 0;
}


void TCPService::startSegment(const MaceKey& peer, BandwidthDirection bd, 
			      registration_uid_t rid) {
  if(trans == NULL) { return; }
#ifdef NEW_BW_FILTER
  trans->start_bw_segment(peer.getMaceAddr().local.addr, bd);
#endif
}

void TCPService::endSegment(const MaceKey& peer, BandwidthDirection bd, 
			    registration_uid_t rid) {
  if(trans == NULL) { return; }
#ifdef NEW_BW_FILTER
  trans->finish_bw_segment(peer.getMaceAddr().local.addr, bd);
#endif
}

double TCPService::getBandwidth(const MaceKey& peer, BandwidthDirection bd, 
				registration_uid_t rid) {
  if(trans == NULL) { return 0.0; }
  return trans->get_bandwidth(peer.getMaceAddr().local.addr, bd);
}

const BufferStatisticsPtr TCPService::getStatistics(const MaceKey& peer,
						    TransportServiceClass::Connection::type d,
						    registration_uid_t rid) {
  assert(trans != NULL);
  return trans->getStatistics(peer.getMaceAddr().local.addr, d);
}
						      
size_t TCPService::outgoingBufferedDataSize(const MaceKey& peer, 
					    registration_uid_t rid) {
  if (trans == NULL) {
    return 0;
  }
  return trans->outgoingBufferedDataSize(peer.getMaceAddr().local.addr);
}

bool TCPService::hasRoom(const MaceKey& peer, registration_uid_t rid) {
  if(trans == NULL) { return false; }
  return trans->has_room(peer.getMaceAddr().local.addr, 0);
}

int TCPService::queued(const MaceKey& peer, registration_uid_t rid) {
  if(trans == NULL) { return 0; }
  return trans->queued(peer.getMaceAddr().local.addr, 0);
}


void TCPService::setWindowSize(const MaceKey& peer, registration_uid_t rid) {

}

}
