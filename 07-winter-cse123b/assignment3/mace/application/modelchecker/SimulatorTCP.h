#ifndef SimulatorTCP_SERVICE_H
#define SimulatorTCP_SERVICE_H

#include "mace.h"
#include "RouteServiceClass.h"
#include "BufferedTransportServiceClass.h"
#include "m_map.h"
#include "SimNetwork.h"
#include <deque>
#include "Serializable.h"

namespace SimulatorTCP_namespace {

class SimulatorTCPService : public virtual BufferedTransportServiceClass, public virtual SimulatorTransport {
public:
  typedef std::deque<SimulatorMessage> MessageQueue;
  typedef mace::vector<MessageQueue> MessageQueueMap;
  typedef mace::vector<int> SocketState;
  static const int MAX_QUEUE_SIZE = 100000;
  //   static int PT_tcp;

public:
  SimulatorTCPService(int queue_size, int port, int node);
  virtual ~SimulatorTCPService() { }

  void maceInit();
  void maceExit() {}
  void maceReset();
  uint32_t hashState() const;
  void print(std::ostream& out) const;
  void printState(std::ostream& out) const;
  void printNetwork(std::ostream& out) const;
  void printNetworkState(std::ostream& out) const;
  registration_uid_t registerHandler(ReceiveDataHandler& handler, registration_uid_t registrationUid = -1);
  registration_uid_t registerHandler(NetworkErrorHandler& handler, registration_uid_t registrationUid = -1);
  registration_uid_t registerHandler(ConnectionStatusHandler& h, registration_uid_t rid = -1);
  bool route(const MaceKey& dest, const std::string& s, registration_uid_t registrationUid);
  void requestToSend(const MaceKey& peer, registration_uid_t rid);
  bool routeRTS(const MaceKey& dest, const std::string& s, registration_uid_t rid);
  void requestFlushedNotification(registration_uid_t rid);

  void signal(int source, TransportSignal sig);

  std::string simulateEvent(const SimulatorMessage& msg);
  void recv(const SimulatorMessage& m);
  void transport_error(const int& destAddr, int transportError);

  std::string simulateFlushed();
  std::string simulateCTS(int node);
  bool hasFlushed();
  bool hasCTS(int node);

  bool isAvailableMessage(int dest);
  SimulatorMessage getMessage(int dest);
  int getPort();
  MaceKey getLocalAddress(registration_uid_t regId = -1) const;

  bool isListening();

  typedef mace::map<int, SimulatorTCPService*> _NodeMap_;
  static bool checkSafetyProperties(const _NodeMap_& nodes);
  static bool checkLivenessProperties(const _NodeMap_& nodes) {
    return true; //could do other perhaps, but not so general.
  }

protected:
  void clearQueue(int destNode, bool upcallError);

private:
  uint queueSize;
  int localPort;
  int localNode;
  SimNetwork& net;
  bool listening;
  typedef mace::deque<registration_uid_t> RegList;
  typedef mace::map<int, RegList> RtsMap;
  RtsMap rtsRequests;
  typedef mace::set<registration_uid_t> RegSet;
  RegSet flushRequests;
  typedef mace::map < registration_uid_t, ConnectionStatusHandler*> ConnectionHandlerMap;
  ConnectionHandlerMap connectionHandlers;
  typedef mace::map < registration_uid_t, ReceiveDataHandler*> ReceiveHandlerMap;
  ReceiveHandlerMap handlers;
  typedef mace::map < registration_uid_t, NetworkErrorHandler*> NetworkHandlerMap;
  NetworkHandlerMap network_handlers;

  MessageQueueMap queuedMessages;
  SocketState openSockets; // true if we have delivered a message to the dest, or if we have received a message
  SocketState remoteErrors; // true if there is an error message coming locally (controlled by signals)
  SocketState localErrors; // true if we have a queued error message
};

}

int SimulatorTCP_load_protocol();
SimulatorTCP_namespace::SimulatorTCPService *SimulatorTCPService_init_function();

#endif
