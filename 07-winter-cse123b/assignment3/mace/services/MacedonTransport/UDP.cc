/* 
 * UDP.cc : part of the Mace toolkit for building distributed systems
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
#include "lib/Log.h"
#include "lib/Util.h"
#include "UDP.h"
#include "lib/NumberGen.h"
#include "lib/ServiceInitializer.h"
#include "lib/mace_constants.h"

using std::string;


TransportServiceClass *UDP_init_function() {
  return new UDP_namespace::UDPService();
}

int UDP_load_protocol()
{
  //     UDPService::PT_UDP = protocol_num;
  ServiceInitializer < TransportServiceClass * >::Instance()->registerService("UDPService", UDP_init_function);
  return 0;
}

namespace UDP_namespace {

// int UDPService::PT_UDP;
TransportServiceClass& new_UDP_Transport(int ipaddr, int port, int queue_size) {
  return *(new UDPService(ipaddr, port, queue_size));
}

UDPService::UDPService(int ipaddr, int port, int queue_size) : inited(0) {

  pthread_mutex_init(&mapLock, 0);
  if(ipaddr == INADDR_ANY) {
    ipaddr = Util::getAddr();
  }
  localIP = ipaddr;
  localKey = MaceKey(ipv4, localIP);
  if(port < 0) {
    port = Util::getPort() + NumberGen::Instance(NumberGen::PORT)->GetVal();
  }
  trans = new macedon_udp_transport(ipaddr, port, queue_size);
  trans->register_receiver(this, (tr_handler)&UDPService::recv_from_pipe, (er_handler)&UDPService::transport_error);
  //contents: do I need to initialize it somehow?
  Log::logf("UDPService::UDPService", "local_address=%.8x port=%d this=%p\n",
	    ipaddr, port, this);
}

void UDPService::maceInit() { 
  inited++;
}

void UDPService::maceExit() { 
  inited--;
  if(inited == 0 && trans != NULL) {
    //cleanup macedon_udp_transport
    trans->register_receiver(NULL, NULL, NULL);
    trans->shutdown();
    trans = NULL;
  }
}

registration_uid_t UDPService::registerHandler(ReceiveDataHandler& handler, registration_uid_t handlerUid) {
  if(handlerUid == -1) {
    handlerUid = NumberGen::Instance(NumberGen::HANDLER_UID)->GetVal();
  }
  lock();
  handlers[handlerUid] = &handler;
  unlock();
  return handlerUid;
}

registration_uid_t UDPService::registerHandler(NetworkErrorHandler& handler, registration_uid_t handlerUid) {
  //What if it was -1?
  return handlerUid;
}

void UDPService::registerUniqueHandler(ReceiveDataHandler& handler) {
  lock();
  handlers[-1] = &handler;
  unlock();
}

void UDPService::registerUniqueHandler(NetworkErrorHandler& handler) {
}

void UDPService::unregisterHandler(ReceiveDataHandler& handler, registration_uid_t handlerUid) {
  lock();
  handlers.erase(handlerUid);
  unlock();
}

void UDPService::unregisterHandler(NetworkErrorHandler& handler, registration_uid_t handlerUid) {
}

bool UDPService::route(const MaceKey& dest, const std::string& s, registration_uid_t handlerUid) {
  ADD_SELECTORS("UDPService::route");
  maceLog("dest=%s size=%zu priority=%d handlerUid=%d\n", dest.toString().c_str(), s.size(), 0, handlerUid);
  lock();
  if(handlers.find(handlerUid) == handlers.end()) { unlock(); return false; }
  unlock();
  mace_udp_header hdr;
  hdr.registrationUid = handlerUid;
  maceout << "Forwarding to nexthop " << dest << Log::endl;
  return !(maceRoute(dest.getMaceAddr().local.addr, hdr, s.data(), s.size()));
}

int UDPService::maceRoute(int dest, mace_udp_header& hdr, const char* str, int size) {
  if(trans == NULL) { return 1; }
  return trans->send(dest, (const unsigned char*)&hdr, sizeof(mace_udp_header), (const unsigned char*)str, size);
}

void UDPService::recv_from_pipe(int from, const unsigned char * hdata, int hsize, const unsigned char * data, int size) {
  ADD_SELECTORS("UDPService::recv_from_pipe");
  string msg((char*)data, size);
  MaceKey fr(ipv4, from);
  maceLog("from=%s hsize=%d dsize=%d\n", fr.toString().c_str(), hsize, size);
  mace_udp_header hdr = *(mace_udp_header*)hdata;
  lock();
  if(handlers.find(hdr.registrationUid) == handlers.end()) { maceWarn("Could not find handler with id %d : dropping packet\n",hdr.registrationUid); unlock(); return; }
  ReceiveDataHandler* handler = handlers[hdr.registrationUid];
  unlock();
  MaceKey source(ipv4, from);
  handler->deliver(source, localKey, msg, hdr.registrationUid);
}

int UDPService::transport_error(int operation, void* arg) {
  return 0;
}

void UDPService::lock() const { assert(pthread_mutex_lock(&mapLock) == 0); }
void UDPService::unlock() const { assert(pthread_mutex_unlock(&mapLock) == 0); }
}
