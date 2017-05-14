#include "SysUtil.h"
#include "Util.h"
#include "PingServiceClass.h"
#include "Ping-init.h"
#include "TcpTransport-init.h"
#include "UdpTransport-init.h"

using namespace std;

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

  void messageTimeout(const MaceKey& k, TransportError::type error, const std::string& m,
		      registration_uid_t rid) {
    cerr << "message timeout " << error << " for " << k << ": " << Log::toHex(m) << endl;
  }
}; // ErrorHandler

static bool shutdownPing = false;

void shutdownHandler(int sig) {
  shutdownPing = true;
} // shutdownHandler

int main(int argc, char* argv[]) {
  Log::autoAdd("Ping::");
//   Log::autoAddAll();
//   Log::setLevel(2);

  SysUtil::signal(SIGINT, &shutdownHandler);
  SysUtil::signal(SIGTERM, &shutdownHandler);

  Params::Instance()->loadparams(argc, argv);

  PingResponseHandler prh;
  ErrorHandler eh;

  BufferedTransportServiceClass& router = TcpTransport_namespace::new_TcpTransport_BufferedTransport(TransportCrypto::NONE, false);

  router.registerHandler(eh);

  PingServiceClass& ping = Ping_namespace::new_Ping_Ping(router, 5*1000*1000, 6*1000*1000);

  ping.maceInit();
  registration_uid_t rid = ping.registerHandler(prh);

  for (int i = 1; i < argc; i++) {
    ping.monitor(MaceKey(ipv4, argv[i]), rid);
  }

  while (!shutdownPing) {
    SysUtil::sleepm(500);
  }

  ping.maceExit();
  return 0;
} // main
