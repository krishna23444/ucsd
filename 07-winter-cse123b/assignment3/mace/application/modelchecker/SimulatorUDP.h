#ifndef SimulatorUDP_SERVICE_H
#define SimulatorUDP_SERVICE_H

#include "mace.h"
#include "TransportServiceClass.h"
#include "m_map.h"
#include "SimNetwork.h"
#include <deque>
#include "Serializable.h"

namespace SimulatorUDP_namespace {

class SimulatorUDPService : public virtual TransportServiceClass, public virtual SimulatorTransport {
public:
  typedef std::deque<SimulatorMessage> MessageQueue;
  typedef mace::vector<MessageQueue> MessageQueueMap;

public:
  SimulatorUDPService(int port, int node);
  virtual ~SimulatorUDPService() { }

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
  bool route(const MaceKey& dest, const std::string& s, registration_uid_t registrationUid);
  void signal(int source, TransportSignal sig) {}

  std::string simulateEvent(const SimulatorMessage& msg);
  void recv(const SimulatorMessage& msg);

  bool isAvailableMessage(int dest);
  SimulatorMessage getMessage(int dest);
  int getPort();
  MaceKey getLocalAddress(registration_uid_t regId = -1) const;

  bool isListening();

private:
  int localPort;
  int localNode;
  SimNetwork& net;
  bool listening;
  typedef mace::map < registration_uid_t, ReceiveDataHandler*> ReceiveHandlerMap;
  ReceiveHandlerMap handlers;

  MessageQueueMap queuedMessages;
};

}

int SimulatorUDP_load_protocol();
SimulatorUDP_namespace::SimulatorUDPService *SimulatorUDPService_init_function();

#endif
