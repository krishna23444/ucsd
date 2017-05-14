/* 
 * sping.cc : part of the Mace toolkit for building distributed systems
 * 
 * Copyright (c) 2006, James W. Anderson, Charles Killian, Ryan Braud, Amin Vahdat
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
#include "SysUtil.h"
#include "Util.h"
#include "PingServiceClass.h"
#include "Ping-init.h"
#include "TcpTransport-init.h"
#include "UdpTransport-init.h"

using namespace std;

class PingLatencyHandler : public PingDataHandler {
  void hostResponseReceived(const MaceKey& host, uint64_t timeSent, uint64_t timeReceived,
			    uint64_t remoteTime, registration_uid_t rid) {
    cout << host << " one-way latency is " << (remoteTime - timeSent) << " usec" << endl;
  } // hostResponseReceived
}; // PingLatencyHandler

class PingResponseHandler : public PingDataHandler {

  void hostResponseReceived(const MaceKey& host, uint64_t timeSent, uint64_t timeReceived,
			    uint64_t remoteTime, registration_uid_t rid) {
    cout << host << " responded in " << (timeReceived - timeSent) << " usec" << endl;
  } // hostResponseReceived

  void hostResponseMissed(const MaceKey& host, uint64_t timeSent, registration_uid_t rid) {
    const time_t t = timeSent / 1000000;
    cout << "did not receive response from " << host << " pinged at " << ctime(&t);
  } // hostResponseMissed

}; // PingResponseHandler

class ErrorHandler : public NetworkErrorHandler {
public:
  void error(const MaceKey& k, TransportError::type error, const std::string& m,
            registration_uid_t rid) {
    cerr << "received error " << error << " for " << k << ": " << m << endl;
  }
}; // ErrorHandler

int main(int argc, char* argv[]) {
//   Log::autoAdd("Ping::");
  Log::autoAddAll();

  if (argc < 2 || !FileUtil::fileExists(argv[1])) {
    cerr << "usage: " << argv[0] << " config-file" << endl;
    exit(-1);
  }
  Params::Instance()->loadfile(argv[1], true);

  PingResponseHandler prh;
  PingLatencyHandler plh;
  ErrorHandler eh;

//   TransportServiceClass& router = UdpTransport_namespace::new_UdpTransport_Transport();
  TransportServiceClass& router = TcpTransport_namespace::new_TcpTransport_Transport(TransportCrypto::TLS);
  router.registerHandler(eh);

  PingServiceClass& ping = Ping_namespace::new_Ping_Ping(router, 1*1000*1000);
  ping.maceInit();
  registration_uid_t rid = ping.registerHandler(prh);
  registration_uid_t lrid = ping.registerHandler(plh);

  for (int i = 2; i < argc; i++) {
    ping.monitor(MaceKey(ipv4, argv[i]), rid);
    ping.monitor(MaceKey(ipv4, argv[i]), lrid);
  }

  SysUtil::sleep();

  return 0;
} // main
