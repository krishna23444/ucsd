#include "TcpTransport.h"
#include "NullBlockManager.h"
#include "Scheduler.h"

TcpTransportPtr transport;

class NullDataHandler : public ReceiveDataHandler {
  
}; // NullDataHandler

void shutdownHandler(int sig) {
  transport->maceExit();
  Scheduler::haltScheduler();
  exit(0);
} // shutdownHandler

int main(int argc, char* argv[]) {
  Log::autoAddAll();
  SysUtil::signal(SIGINT, &shutdownHandler);
  SysUtil::signal(SIGTERM, &shutdownHandler);

  NullBlockManager bm(1);
  NullDataHandler ndh;

  MaceKey dest;
  if (argc == 2) {
    dest = MaceKey(ipv4, argv[1]);
  }

  transport = TcpTransport::create();

  transport->maceInit();
  transport->registerUniqueHandler(ndh);
  if (dest != MaceKey()) {
    Log::log("main") << "sending to " << dest << Log::endl;
    transport->route(dest, bm.getBlock(0));
  }
  else {
    Log::log("main") << "starting as receiver" << Log::endl;
  }

  SysUtil::sleep();

  return 0;
} // main
