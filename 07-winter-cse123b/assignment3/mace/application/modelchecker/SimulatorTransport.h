#ifndef _SIM_TRANSPORT__H_
#define _SIM_TRANSPORT__H_

#include <string>
#include "MaceTypes.h"
#include "TransportServiceClass.h"

struct SimulatorMessage {
  public:
  int source;
  int destination;
  uint32_t messageId;
  registration_uid_t regId;
  enum MessageType { MESSAGE, READ_ERROR, DEST_NOT_READY } messageType;
  std::string msg;
  SimulatorMessage(const int& s, const int& d, uint32_t id, const MessageType& t, registration_uid_t rid, const std::string& m)
    : source(s), destination(d), messageId(id), regId(rid), messageType(t), msg(m)
  {}
};

class SimulatorTransport : public virtual TransportServiceClass {
  public:
    enum TransportSignal { BIDI_READ_ERROR, SOURCE_RESET };
    virtual bool isAvailableMessage(int destNode) = 0;
    virtual SimulatorMessage getMessage(int destNode) = 0;
    virtual int getPort() = 0;
    virtual std::string simulateEvent(const SimulatorMessage& msg) = 0;
    virtual bool isListening() = 0;
    virtual void printNetwork(std::ostream& out) const = 0;
    virtual void printNetworkState(std::ostream& out) const = 0;
    virtual void signal(int source, TransportSignal sig) = 0;
  virtual std::string simulateFlushed() { ASSERT(0); }
  virtual std::string simulateCTS(int node) { ASSERT(0); }
  virtual bool hasFlushed() { return false; }
  virtual bool hasCTS(int node) { return false; }
};

#endif
