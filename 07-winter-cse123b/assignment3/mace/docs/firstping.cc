#include "SysUtil.h"
#include "Util.h"
#include "PingServiceClass.h"
#include "Ping-init.h"

using namespace std;

class PingResponseHandler : public PingDataHandler {

  void hostResponseReceived(const MaceKey& host, uint64_t timeSent, uint64_t timeReceived,
			    uint64_t remoteTime, registration_uid_t rid) {
    cout << host << " responded in " << (timeReceived - timeSent) << " usec" << endl;
    exit(0);
  } // hostResponseReceived

  void hostResponseMissed(const MaceKey& host, uint64_t timeSent, registration_uid_t rid) {
    const time_t t = timeSent / 1000000;
    cout << "did not receive response from " << host << " pinged at " << ctime(&t);
    exit(0);
  } // hostResponseMissed

}; // PingResponseHandler

int main(int argc, char* argv[]) {
//   Log::autoAdd("Ping::");

  PingResponseHandler prh;

  PingServiceClass& ping = FirstPing_namespace::new_FirstPing_Ping();
  ping.maceInit();
  ping.registerUniqueHandler(prh);
  if (argc > 1) {
    ping.monitor(MaceKey(ipv4, argv[1]));
  }

  SysUtil::sleep();

  return 0;
} // main
