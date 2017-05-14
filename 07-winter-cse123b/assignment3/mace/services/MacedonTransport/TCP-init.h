#ifndef TCP_init_h
#define TCP_init_h

#include "TCP.h"

namespace TCP_namespace {

TransportServiceClass& new_TCP_Transport(int queue_size = TCPService::MAX_QUEUE_SIZE, int port = -1, int ipaddr = INADDR_ANY);
BandwidthTransportServiceClass& new_TCP_BandwidthTransport(int queue_size = TCPService::MAX_QUEUE_SIZE, int port = -1, int ipaddr = INADDR_ANY);
BufferedTransportServiceClass& new_TCP_BufferedTransport(int queue_size = TCPService::MAX_QUEUE_SIZE, int port = -1, int ipaddr = INADDR_ANY);

}

#endif
