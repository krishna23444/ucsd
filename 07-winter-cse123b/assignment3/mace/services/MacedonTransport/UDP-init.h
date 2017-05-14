#ifndef UDP_init_h
#define UDP_init_h

#include "UDP.h"

namespace UDP_namespace {

TransportServiceClass& new_UDP_Transport(int ipaddr = INADDR_ANY, int port = -1, int queue_size = UDP_namespace::UDPService::MAX_QUEUE_SIZE);

}

#endif
