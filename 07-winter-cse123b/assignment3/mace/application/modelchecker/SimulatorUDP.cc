#include "Util.h"
#include "Log.h"
#include "SimulatorUDP.h"
#include "NumberGen.h"
#include "ServiceInitializer.h"
#include "Accumulator.h"
#include "mace_constants.h"
#include "MaceTypes.h"
#include "mace-macros.h"

using std::string;

int SimulatorUDP_load_protocol()
{
  return 0;
}

namespace SimulatorUDP_namespace {

  SimulatorUDPService::SimulatorUDPService(int port, int node) : localPort(port), localNode(node), net(SimNetwork::Instance()), listening(false), queuedMessages(Sim::getNumNodes()) {
    ADD_SELECTORS("UDP::constructor");

    maceout << "local_address " << Sim::getMaceKey(node) << " port " << port << Log::endl;
    net.registerHandler(port, *this);
  }

  void SimulatorUDPService::maceInit() { 
    listening = true;
  }

  bool SimulatorUDPService::isListening() {
    return listening;
  }
  
  void SimulatorUDPService::maceReset() {
    listening = false;
    handlers.clear();
  }

  registration_uid_t SimulatorUDPService::registerHandler(ReceiveDataHandler& handler, registration_uid_t handlerUid) {
    if(handlerUid == -1) {
      handlerUid = NumberGen::Instance(NumberGen::HANDLER_UID)->GetVal();
    }
    handlers[handlerUid] = &handler;
    return handlerUid;
  }

  registration_uid_t SimulatorUDPService::registerHandler(NetworkErrorHandler& handler, registration_uid_t handlerUid) {
    if(handlerUid == -1) {
      handlerUid = NumberGen::Instance(NumberGen::HANDLER_UID)->GetVal();
    }
    return handlerUid;
  }

  bool SimulatorUDPService::route(const MaceKey& dest, const string& s, registration_uid_t handlerUid) {
    static bool allowErrors = Params::Instance()->get("USE_UDP_ERRORS", false);
    ADD_SELECTORS("SIMULATOR::UDP::route::pkt::(downcall)");
    maceLog("dest=%s size=%d priority=%d handlerUid=%d\n", dest.toString().c_str(), s.size(), 0, handlerUid);
    uint32_t msgId = net.getMessageId();
    int destNode = Sim::getNode(dest);
    MessageQueue& mqueue = queuedMessages[destNode];
    maceout << "drop message routed from " << localNode << " to " << destNode << "? (0-no 1-yes) " << Log::endl;
    if(allowErrors && RandomUtil::randInt(2, 1, 0)) { //In search mode, either error or non-error may occur.  In gusto mode, no errors may occur
      maceout << "route([dest=" << dest << "][s_deserialized=pkt(id=" << msgId
        << ", SIM_DROP=1)])" << Log::endl;
    } else {
      maceout << "route([dest=" << dest << "][s_deserialized=pkt(id=" << msgId << ")])"
        << Log::endl;
      mqueue.push_back(SimulatorMessage(localNode, destNode, msgId, SimulatorMessage::MESSAGE, handlerUid, s));
    }
    return true; //UDP always appears to send message
  }

  bool SimulatorUDPService::isAvailableMessage(int destNode) {
    std::deque<SimulatorMessage>& deque = queuedMessages[destNode];
    return !deque.empty();
  }

  SimulatorMessage SimulatorUDPService::getMessage(int destNode) {
    static bool allowReorder = Params::Instance()->get("USE_UDP_REORDER", true);
    ADD_SELECTORS("SIMULATOR::UDP::getMessage");
    std::deque<SimulatorMessage>& deque = queuedMessages[destNode];
    ASSERT(isAvailableMessage(destNode));
    int msg = 0;
    //if (allowReorder && Sim::gusto) -- original, buggy code -- only allows reordering during gusto phase
    //if (allowReorder || Sim::gusto) -- possible "correct" code for practical model checking -- always allow reordering during gusto
    if (allowReorder && !Sim::getGusto()) {
      maceout << "Which message in queue to deliver?" << std::endl;
      if(! maceout.isNoop()) {
        int position = 0;
        for(MessageQueue::const_iterator j = deque.begin(); j != deque.end(); j++, position++) {
          maceout << position << ": message " << j->messageId << std::endl;
        }
      }
      maceout << Log::endl;
      msg = RandomUtil::randInt(deque.size());
    }
    MessageQueue::iterator i = deque.begin()+msg;
    SimulatorMessage str = *(i);
    deque.erase(i);
    return str;
  }

  int SimulatorUDPService::getPort() { return localPort; }
  MaceKey SimulatorUDPService::getLocalAddress(registration_uid_t regId) const { 
    ADD_SELECTORS("UDP::getLocalAddress");
    maceLog("called getLocalAddress\n");
    return Sim::getMaceKey(localNode); 
  }

  std::string SimulatorUDPService::simulateEvent(const SimulatorMessage& msg) {
    ASSERT(msg.destination == localNode);
    if(msg.messageType == SimulatorMessage::MESSAGE) {
      recv(msg);
      return "";
    } else if(msg.messageType == SimulatorMessage::DEST_NOT_READY) {
      ASSERT(isAvailableMessage(msg.source));
      SimulatorMessage m = getMessage(msg.source);
      return "id " + boost::lexical_cast<std::string>(m.messageId);
    } else { //msg.messageType == READ_ERROR
      ASSERT(0);
    }
  }
  void SimulatorUDPService::recv(const SimulatorMessage& msg) {
    ADD_SELECTORS("UDP::recv");
    maceLog("from=%s dsize=%d\n", Sim::getMaceKey(msg.source).toString().c_str(), msg.msg.size());
    ReceiveHandlerMap::iterator i = handlers.find(msg.regId);
    if(i == handlers.end()) { maceWarn("Could not find handler with id %d for msgId %u : dropping packet\n", msg.regId, msg.messageId); return; }
    i->second->deliver(Sim::getMaceKey(msg.source), Sim::getMaceKey(msg.destination), msg.msg, msg.regId);
  }

  uint32_t SimulatorUDPService::hashState() const {
    static uint32_t hashAnd = (localPort) * localNode;
    static hash_string strHasher;
    uint32_t hashWork = hashAnd;
    for(mace::map < registration_uid_t, ReceiveDataHandler*>::const_iterator i = handlers.begin(); i != handlers.end(); i++) {
      hashWork = (hashWork << 1) ^ i->first;
    }
    int node;
    MessageQueueMap::const_iterator i;
    for(i = queuedMessages.begin(), node = 0; i != queuedMessages.end(); i++, node++) {
      hashWork += node;
      for(MessageQueue::const_iterator j = i->begin(); j != i->end(); j++) {
        hashWork ^= strHasher((*j).msg);
      }
    }
    return hashWork;
  }

  void SimulatorUDPService::printState(std::ostream& out) const {
    out << "[SimulatorUDP]" << std::endl;
    out << "[localPort = " << localPort << "][localKey = " << Sim::getMaceKey(localNode) << "]";
    out << "[RDH] ";
    for(mace::map < registration_uid_t, ReceiveDataHandler*>::const_iterator i = handlers.begin(); i != handlers.end(); i++) {
      out << i->first << " ";
    }
    out << "[/RDH]";
    out << "[/SimulatorUDP]" << std::endl;
  }

  void SimulatorUDPService::print(std::ostream& out) const {
    out << "[localPort = " << localPort << "][localKey = " << Sim::getMaceKey(localNode) << "]";
    out << "[RDH] ";
    for(mace::map < registration_uid_t, ReceiveDataHandler*>::const_iterator i = handlers.begin(); i != handlers.end(); i++) {
      out << i->first << " ";
    }
    out << "[/RDH]";
    //This is getting printed by printNetwork();
    //     out << "[messages]";
    //     int node;
    //     MessageQueueMap::const_iterator i;
    //     for(i = queuedMessages.begin(), node=0; i != queuedMessages.end(); i++, node++) {
    //       out << "[toDest(" << node << ")]";
    //       for(MessageQueue::const_iterator j = i->begin(); j != i->end(); j++) {
    //         out << "[msg]" << (*j).messageId << "::" << (*j).messageType << "[/msg]";
    //       }
    //       out << "[/toDest(" << node << ")]";
    //     }
    //     out << "[/messages]";
  }

  void SimulatorUDPService::printNetwork(std::ostream& out) const {
    out << "[messages]";
    int node;
    MessageQueueMap::const_iterator i;
    for(i = queuedMessages.begin(), node=0; i != queuedMessages.end(); i++, node++) {
      if(i->size()) {
        out << "[toDest(" << node << ")] ";
        for(MessageQueue::const_iterator j = i->begin(); j != i->end(); j++) {
          out << (*j).messageId << "::" << (*j).messageType << " ";
        }
        out << "[/toDest(" << node << ")]";
      }
    }
    out << "[/messages]";
  }
  void SimulatorUDPService::printNetworkState(std::ostream& out) const {
    out << "[messages]";
    int node;
    MessageQueueMap::const_iterator i;
    for(i = queuedMessages.begin(), node=0; i != queuedMessages.end(); i++, node++) {
      if(i->size()) {
        out << "[toDest(" << node << ")]";
        for(MessageQueue::const_iterator j = i->begin(); j != i->end(); j++) {
          out << " " << (*j).regId << " " << (*j).messageType << " " << Base64::encode((*j).msg) << " ";
        }
        out << "[/toDest(" << node << ")]";
      }
    }
    out << "[/messages]";
  }

}
