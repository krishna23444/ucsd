#ifndef _SIM_NET__H_
#define _SIM_NET__H_

#include "Sim.h"
#include "m_map.h"
#include "mset.h"
#include "mvector.h"
#include "Iterator.h"
#include "MaceTypes.h"
#include "Util.h"
#include "SimulatorTransport.h"
#include "ReceiveDataHandler.h"
#include "mace-macros.h"

typedef mace::vector<SimulatorTransport*> NodeRecipientMap;
typedef mace::vector<NodeRecipientMap> PortNodeRecipientMap;
typedef mace::set<int> ErrorSet;
typedef mace::map<int, ErrorSet> ErrorMap;

class SimNetwork : public Sim {
  protected:
    static SimNetwork* _sim_inst;
    PortNodeRecipientMap simulatorTransportMap;
    uint32_t currentMessageId;
    int base_port;
    SimNetwork(int port) 
      : Sim(), 
        simulatorTransportMap(1, mace::vector<SimulatorTransport*>(Sim::getNumNodes())), 
        currentMessageId(0), 
        base_port(port) 
    { }
  public:
    uint32_t getMessageId() {
      return currentMessageId++;
    }
    uint32_t hashState() const {
      uint32_t hash = 0;
      int port;
      int node;
      PortNodeRecipientMap::const_iterator i;
      NodeRecipientMap::const_iterator j;
      for(i = simulatorTransportMap.begin(), port = base_port; i != simulatorTransportMap.end(); i++, port++) {
        for(j = i->begin(), node = 0; j != i->end(); j++, node++) {
          hash += port ^ node ^  (*j)->hashState();
        }
      }
      return hash;
    }
    void print(std::ostream& out) const {
      int port;
      int node;
      PortNodeRecipientMap::const_iterator i;
      NodeRecipientMap::const_iterator j;
      for(i = simulatorTransportMap.begin(), port = base_port; i != simulatorTransportMap.end(); i++, port++) {
        out << "[Port(" << port << ")]";
        for(j = i->begin(), node = 0; j != i->end(); j++, node++) {
          out << "[Node(" << node << ")]";
          (*j)->printNetwork(out);
          out << "[/Node(" << node << ")]";
        }
        out << "[/Port(" << port << ")]";
      }
    }
    void printState(std::ostream& out) const {
      int port;
      int node;
      PortNodeRecipientMap::const_iterator i;
      NodeRecipientMap::const_iterator j;
      for(i = simulatorTransportMap.begin(), port = base_port; i != simulatorTransportMap.end(); i++, port++) {
        out << "[Port(" << port << ")]";
        for(j = i->begin(), node = 0; j != i->end(); j++, node++) {
          out << "[Node(" << node << ")]";
          (*j)->printNetworkState(out);
          out << "[/Node(" << node << ")]";
        }
        out << "[/Port(" << port << ")]";
      }
    }
    void registerHandler(int port, SimulatorTransport& trans) {
      ADD_FUNC_SELECTORS;
      maceout << "registering handler for node " << Sim::getCurrentMaceKey() << " on port " << port << " for node " << getCurrentNode() << " with transport " << &trans << Log::endl;
      unsigned p2 = port;
      p2 -= (base_port-1);
      while(p2 > simulatorTransportMap.size()) { simulatorTransportMap.push_back(mace::vector<SimulatorTransport*>(Sim::getNumNodes())); }
      simulatorTransportMap[port-base_port][Sim::getCurrentNode()] = &trans;
    }
    SimulatorTransport* getTransport(int node, int port) {
      return simulatorTransportMap[port-base_port][node];
    }
    bool isListening(int node, int port) const {
      return simulatorTransportMap[port-base_port][node] != NULL && simulatorTransportMap[port-base_port][node]->isListening();
    }
    void reset() {
      for(PortNodeRecipientMap::iterator i = simulatorTransportMap.begin(); i != simulatorTransportMap.end(); i++) {
        for(NodeRecipientMap::iterator j = i->begin(); j != i->end(); j++) {
          delete *j;
          *j = NULL;
        }
      }
      currentMessageId = 0;
    }
    enum NetEventType { MESSAGE, DEST_NOT_READY, CTS, FLUSHED };
    void eventsWaiting(EventList& ev) const {
      ADD_SELECTORS("SimNetwork::eventsWaiting");

      int port;
      int node;
      int node2;
      PortNodeRecipientMap::const_iterator i;
      NodeRecipientMap::const_iterator j, k;
      for(i = simulatorTransportMap.begin(), port = base_port; i != simulatorTransportMap.end(); i++, port++) {
        maceout << "checking port " << port << Log::endl;
        for(j = i->begin(), node = 0; j != i->end(); j++, node++) {
	  if ((*j)->hasFlushed()) {
	    EventWeight ew;
	    ew.second = 40;
	    ew.first.node = node;
	    ew.first.type = Event::NETWORK;
	    ew.first.simulatorVector.push_back(SimNetwork::FLUSHED);
	    ew.first.simulatorVector.push_back(node);
	    ew.first.simulatorVector.push_back(port);
            ew.first.desc = "Flushed";
	    ev.push_back(ew);
	  }

          for(k = j, node2 = node; k != i->end(); k++, node2++) { 
	    if ((*j)->hasCTS(node2)) {
	      EventWeight ew;
	      ew.second = 40;
	      ew.first.node = node;
	      ew.first.type = Event::NETWORK;
	      ew.first.simulatorVector.push_back(SimNetwork::CTS);
	      ew.first.simulatorVector.push_back(node2);
	      ew.first.simulatorVector.push_back(port);
              ew.first.desc = "(CTS,dest node,port)";
	      ev.push_back(ew);
	    }
	    if ((*k)->hasCTS(node)) {
	      EventWeight ew;
	      ew.second = 40;
	      ew.first.node = node2;
	      ew.first.type = Event::NETWORK;
	      ew.first.simulatorVector.push_back(SimNetwork::CTS);
	      ew.first.simulatorVector.push_back(node);
	      ew.first.simulatorVector.push_back(port);
              ew.first.desc = "(CTS,dest node,port)";
	      ev.push_back(ew);
	    }

            bool jTok = (*j)->isAvailableMessage(node2);
            bool kToj = (*k)->isAvailableMessage(node);
            bool jListening = (*j)->isListening();
            bool kListening = (*k)->isListening();
            if( (jListening && kToj) ) {
              macedbg(0) << "Message from " << node2 << " to " << node << Log::endl;
              //Queue message from k to j
              EventWeight ew;
              ew.second = 8;
              ew.first.node = node;
              ew.first.type = Event::NETWORK;
              ew.first.simulatorVector.push_back(SimNetwork::MESSAGE);
              ew.first.simulatorVector.push_back(node2);
              ew.first.simulatorVector.push_back(port);
              ew.first.desc = "(MESSAGE EVENT,src,port)";
              ev.push_back(ew);
            }
            if( (kListening && jTok) ) {
              macedbg(0) << "Message from " << node << " to " << node2 << Log::endl;
              //Queue message from j to k
              EventWeight ew;
              ew.second = 8;
              ew.first.node = node2;
              ew.first.type = Event::NETWORK;
              ew.first.simulatorVector.push_back(SimNetwork::MESSAGE);
              ew.first.simulatorVector.push_back(node);
              ew.first.simulatorVector.push_back(port);
              ew.first.desc = "(MESSAGE EVENT,src,port)";
              ev.push_back(ew);
            }
            if( (!jListening && kToj) ) {
              macedbg(0) << "DEST_NOT_READY about " << node << " for " << node2 << Log::endl;
              //Queue dest_not_ready from j to k
              EventWeight ew;
              ew.second = 2;
              ew.first.node = node2;
              ew.first.type = Event::NETWORK;
              ew.first.simulatorVector.push_back(SimNetwork::DEST_NOT_READY);
              ew.first.simulatorVector.push_back(node);
              ew.first.simulatorVector.push_back(port);
              ew.first.desc = "(dest not ready,dest,port)";
              ev.push_back(ew);
            }
            if( (!kListening && jTok) ) {
              macedbg(0) << "DEST_NOT_READY about " << node2 << " for " << node << Log::endl;
              //Queue dest_not_ready from k to j
              EventWeight ew;
              ew.second = 2;
              ew.first.node = node;
              ew.first.type = Event::NETWORK;
              ew.first.simulatorVector.push_back(SimNetwork::DEST_NOT_READY);
              ew.first.simulatorVector.push_back(node2);
              ew.first.simulatorVector.push_back(port);
              ew.first.desc = "(dest not ready,dest,port)";
              ev.push_back(ew);
            }
          }
        }
      }
    }
    void signal(int source, int dest, int port, SimulatorTransport::TransportSignal sig) {
      simulatorTransportMap[port-base_port][dest]->signal(source, sig);
    }
    std::string simulateEvent(const Event& e) {
      ADD_SELECTORS("SimNetwork::simulateEvent");

      maceout << "Simulating network event for " << e.node << " from " << e.simulatorVector[1] << " type " << e.simulatorVector[0] << " on port " << e.simulatorVector[2] << Log::endl;

      SimulatorTransport* dest = getTransport(e.node, e.simulatorVector[2]);
      std::ostringstream r;

      switch(e.simulatorVector[0]) {
        case SimNetwork::MESSAGE: 
          {
            SimulatorMessage msg = getTransport(e.simulatorVector[1], e.simulatorVector[2])->getMessage(e.node);
            if(msg.messageType == SimulatorMessage::READ_ERROR) {
              maceout << "Delivering socket error from " << msg.source << " to " << msg.destination
                << " messageId " << msg.messageId << Log::endl;
              r << "error from " << msg.source << " on " << msg.destination << " id " << msg.messageId;
            } else {
              maceout << "Delivering messageId " << msg.messageId << " from "
                << msg.source << " to " << msg.destination << Log::endl;
              r << "id " << msg.messageId << " from " << msg.source << " to " << msg.destination;
            }
            r << " " << dest->simulateEvent(msg);
            break;
          }
        case SimNetwork::DEST_NOT_READY: 
          {
            SimulatorMessage msg(e.simulatorVector[1], e.node, getMessageId(),
                SimulatorMessage::DEST_NOT_READY, -1, "");
            maceout << "Event of " << msg.destination << " is a dest_not_ready error from node "
              << msg.source << Log::endl;
            std::string s = dest->simulateEvent(msg);
            r << s << " dest_not_ready error from " << msg.source << " on " << msg.destination;
            break;
          }
      case SimNetwork::CTS: {
	maceout << "simulating CTS on " << e.node << " for " << e.simulatorVector[1] << Log::endl;
	r << "CTS for " << e.simulatorVector[1] << " on " << e.node << " ";
	r << dest->simulateCTS(e.simulatorVector[1]);
	break;
      }
      case SimNetwork::FLUSHED: {
	maceout << "simulating flushed on " << e.simulatorVector[1] << Log::endl;
	r << "notifying flushed on " << e.node << " ";
	r << dest->simulateFlushed();
	break;
      }
        default: ASSERT(e.simulatorVector[0] == SimNetwork::MESSAGE || e.simulatorVector[0] == SimNetwork::DEST_NOT_READY);
      }
      return r.str();
    }
    static SimNetwork& SetInstance(int port) { 
      ASSERT(_sim_inst == NULL);
      _sim_inst = new SimNetwork(port);
      return *_sim_inst; 
    } 
    static SimNetwork& Instance() { 
      ASSERT(_sim_inst != NULL);
      return *_sim_inst; 
    } 
    virtual ~SimNetwork() { }
};

#endif
