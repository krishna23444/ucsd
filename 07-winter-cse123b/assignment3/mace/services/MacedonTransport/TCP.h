/* 
 * TCP.h : part of the Mace toolkit for building distributed systems
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
#ifndef TCP_SERVICE_H
#define TCP_SERVICE_H

#include "lib/mace.h"
#include "services/interfaces/BandwidthTransportServiceClass.h"
#include "macedon_tcp_transport.h"
#include "mhash_map.h"

namespace TCP_namespace {

struct mace_tcp_header {
  registration_uid_t registrationUid;
};

class TCPService : public BandwidthTransportServiceClass,
		   public BufferedTransportServiceClass,
		   public macedon_transport_receiver {
public:
  static const int MAX_QUEUE_SIZE = 100;
  //   static int PT_tcp;

public:
  TCPService(int queue_size = MAX_QUEUE_SIZE, int port = -1, int ipaddr = INADDR_ANY);
  virtual ~TCPService() { }

  void maceInit();
  void maceExit();
  registration_uid_t registerHandler(ReceiveDataHandler& handler, registration_uid_t registrationUid = -1);
  void registerUniqueHandler(ReceiveDataHandler& handler);
  registration_uid_t registerHandler(NetworkErrorHandler& handler, registration_uid_t registrationUid = -1);
  void registerUniqueHandler(NetworkErrorHandler& handler);
  registration_uid_t registerHandler(ConnectionStatusHandler& h,
				     registration_uid_t rid = -1);
  void registerUniqueHandler(ConnectionStatusHandler& h);

  void unregisterHandler(ReceiveDataHandler& handler, registration_uid_t registrationUid = -1);
  void unregisterHandler(NetworkErrorHandler& handler, registration_uid_t registrationUid = -1);
  void unregisterHandler(ConnectionStatusHandler& h, registration_uid_t rid = -1);
  bool route(const MaceKey& dest, const std::string& s, registration_uid_t registrationUid);

  int maceRoute(int nextHop, mace_tcp_header& hdr, const char* str, int size);

  void recv_from_pipe(int from, const unsigned char * hdata, int hsize, const unsigned char * data, int size);
  int transport_error(int operation, void* arg); 

  void startSegment(const MaceKey& peer, BandwidthDirection bd, registration_uid_t rid);
  void endSegment(const MaceKey& peer, BandwidthDirection bd, registration_uid_t rid);
  double getBandwidth(const MaceKey& peer, BandwidthDirection bd, registration_uid_t rid);
  bool hasRoom(const MaceKey& peer, registration_uid_t rid);
  int queued(const MaceKey& peer, registration_uid_t rid);
  void setWindowSize(const MaceKey& peer, registration_uid_t rid);

  const BufferStatisticsPtr getStatistics(const MaceKey& peer,
					  TransportServiceClass::Connection::type d,
					  registration_uid_t rid);
  size_t outgoingBufferedDataSize(const MaceKey& peer, registration_uid_t rid);


private:
  macedon_tcp_transport *trans;
  uint localIP;
  MaceKey localKey;
  mace::map < registration_uid_t, ReceiveDataHandler*> handlers;
  mace::map < registration_uid_t, NetworkErrorHandler*> network_handlers;
  int inited;
  mutable pthread_mutex_t mapLock;
  void lock() const;
  void unlock() const;
};

}

int TCP_load_protocol();
TCP_namespace::TCPService *TCPService_init_function();

#endif
