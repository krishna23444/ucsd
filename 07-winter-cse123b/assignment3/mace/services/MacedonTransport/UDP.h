/* 
 * UDP.h : part of the Mace toolkit for building distributed systems
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
#ifndef UDP_SERVICE_H
#define UDP_SERVICE_H

#include "lib/mace.h"
#include "services/interfaces/TransportServiceClass.h"
#include "macedon_udp_transport.h"

#if __GNUC__ >= 3
#include <ext/hash_map>
using namespace __gnu_cxx;
#else
#include <hash_map>
#endif

namespace UDP_namespace {

struct mace_udp_header {
  registration_uid_t registrationUid;
};

class UDPService : public TransportServiceClass, public macedon_transport_receiver {
public:
  static const int MAX_QUEUE_SIZE = 100;
  //   static int PT_UDP;

public:
  UDPService(int ipaddr = INADDR_ANY, int port = -1, 
	     int queue_size = MAX_QUEUE_SIZE);
  virtual ~UDPService() { }

  void maceInit();
  void maceExit();
  registration_uid_t registerHandler(ReceiveDataHandler& handler, registration_uid_t handlerUid = -1);
  registration_uid_t registerHandler(NetworkErrorHandler& handler, registration_uid_t handlerUid = -1);
  void registerUniqueHandler(ReceiveDataHandler& handler);
  void registerUniqueHandler(NetworkErrorHandler& handler);
  void unregisterHandler(ReceiveDataHandler& handler, registration_uid_t handlerUid = -1);
  void unregisterHandler(NetworkErrorHandler& handler, registration_uid_t handlerUid = -1);
  bool route(const MaceKey& dest, const std::string& s, registration_uid_t handlerUid);

  int maceRoute(int dest, mace_udp_header& hdr, const char* str, int size);

  //For callback from the UDP transport.
  void recv_from_pipe(int from, const unsigned char * hdata, int hsize, const unsigned char * data, int size);
  int transport_error(int operation, void* arg); 

private:

  macedon_udp_transport *trans;
  uint localIP;
  MaceKey localKey;
  hash_map < registration_uid_t, ReceiveDataHandler*> handlers;
  int inited;
  mutable pthread_mutex_t mapLock;
  void lock() const;
  void unlock() const;
};

}

int UDP_load_protocol();
UDP_namespace::UDPService *UDPService_init_function();

#endif
