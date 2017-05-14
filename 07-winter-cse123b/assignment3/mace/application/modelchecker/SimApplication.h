#ifndef _SIM_APP_H_
#define _SIM_APP_H_

#include "MaceTypes.h"
#include "mvector.h"
#include "OverlayServiceClass.h"
#include "Sim.h"
#include "SimApplicationServiceClass.h"
#include "SimHandler.h"
#include "mace-macros.h"

class SimApplication : public Sim {
private:
  class SimNodeHandler : public SimHandler, public PrintPrintable {
  protected:
    int nodeNum;
    int incarnationNumber;
    SimApplicationServiceClass* node;
    bool eventSimulated;

  public:
    SimNodeHandler(int nn, SimApplicationServiceClass* no) : nodeNum(nn), incarnationNumber(0), node(no), eventSimulated(false) {
      node->registerUniqueHandler(*this);
    }

    const MaceKey& getMaceKey(int n, registration_uid_t rid = -1) const {
      return Sim::getMaceKey(n);
    }
    int getNodeNumber(const MaceKey& node, registration_uid_t rid = -1) const {
      return Sim::getNode(node);
    }
    int getNodeNumber(registration_uid_t rid = -1) const {
      return nodeNum;
    }
    int getNodeCount(registration_uid_t rid = -1) const {
      return Sim::getNumNodes();
    }
    int getIncarnationNumber(registration_uid_t rid = -1) const {
      return incarnationNumber;
    }
    int eventsWaiting(EventList& ev) const {
      static int appWeight = Params::Instance()->get("APPLICATION_EVENT_WEIGHT", 40);
      static int failWeight = Params::Instance()->get("EVENT_FAILURE_WEIGHT", 0);
      static bool simulateFailure = Params::Instance()->get("SIM_NODE_FAILURE", false);

      int eventsAdded = 0;
      if(!Sim::gusto && simulateFailure && eventSimulated) {
        //push back failure event
        EventWeight ew;
        Event& e = ew.first;
        e.node = nodeNum;
        e.type = Event::APPLICATION;
        e.simulatorVector.push_back(SimApplication::RESET); 
        e.desc = "(Node Reset)";
        ew.second = failWeight;
        ev.push_back(ew);
        eventsAdded++;
      }
      if(node->eventsWaiting()) {
        //push back app event
        EventWeight ew;
        Event& e = ew.first;
        e.node = nodeNum;
        e.type = Event::APPLICATION;
        e.simulatorVector.push_back(SimApplication::APP); 
        e.desc = "(SimApp)";
        ew.second = appWeight;
        ev.push_back(ew);
        eventsAdded++;
      }
      return eventsAdded;
    }
    std::string simulateEvent(const Event& e) {
      ADD_SELECTORS("SimApplication::simulateEvent");
      eventSimulated = true;
      switch(e.simulatorVector[0]) {
        case SimApplication::RESET:
          maceout << "simulating failure of " << e.node << Log::endl;
          this->reincarnate();
          return "FAILURE";
        case SimApplication::APP:
          return node->simulateEvent();
        default: ASSERT(e.simulatorVector[0] == SimApplication::RESET || e.simulatorVector[0] == SimApplication::APP);
      }
      ASSERT(0);
    }
    void reincarnate() {
      eventSimulated = false;
      node->maceReset();
      node->registerUniqueHandler(*this);
      incarnationNumber++;
    }
    uint32_t hashState() const {
      return nodeNum * incarnationNumber + incarnationNumber;
    }
    void reset() {
      reincarnate();
      incarnationNumber = 0;
    }
    void print(std::ostream& out) const {
      out << "[nodeHandler " << nodeNum << " incarnation " << incarnationNumber << "]";
    }
  };

protected:
  static SimApplication* _sim_inst;
  SimApplication(SimApplicationServiceClass** tnodes) : Sim(), nodes(tnodes), nodeHandlers(new SimNodeHandler*[Sim::getNumNodes()]) { 
    for(int i = 0; i < Sim::getNumNodes(); i++) {
      nodeHandlers[i] = new SimNodeHandler(i, nodes[i]);
    }
  }
  static bool CheckInstance() { 
    ASSERT(_sim_inst != NULL);
    return true;
  }
  
  SimApplicationServiceClass** nodes;
  SimNodeHandler** nodeHandlers;

public:
  static SimApplication& Instance() { static bool dud __attribute((unused)) = CheckInstance(); return *_sim_inst; }
  static void SetInstance(SimApplicationServiceClass** nodes) {
    ASSERT(_sim_inst == NULL);
    ASSERT(nodes != NULL);
    _sim_inst = new SimApplication(nodes);
  }

  uint32_t hashState() const {
    uint32_t hash = 0;
    for(int i = 0; i < Sim::getNumNodes(); i++) {
      hash = (hash << 1) ^ nodes[i]->hashState();
    }
    return hash;
  }
  void print(std::ostream& out) const {
    for(int i = 0; i < Sim::getNumNodes(); i++) {
      out << " [ Node " << i << ": " << *(nodes[i]) << " ] ";
    }
  }
  void printState(std::ostream& out) const {
    for(int i = 0; i < Sim::getNumNodes(); i++) {
      out << " [ Node " << i << ": ";
      nodes[i]->printState(out);
      out << " ] ";
    }
  }
  enum AppEventType { APP, RESET };
  EventList getNodeFailureEvents() const {
    EventList ev;
    for(int i = 0; i < Sim::getNumNodes(); i++) {
    }
    return ev;
  }
  void eventsWaiting(EventList& ev) const {
    ADD_SELECTORS("SimApplication::eventsWaiting");
    int eventsAdded = 0;
    for(int i = 0; i < Sim::getNumNodes(); i++) {
      eventsAdded += nodeHandlers[i]->eventsWaiting(ev);
    }
    maceout << "Added " << eventsAdded << " events to the event list" << Log::endl;
  }
  std::string simulateEvent(const Event& e) {
    ADD_SELECTORS("SimApplication::simulateEvent");
    ASSERT(e.node < Sim::getNumNodes());
    ASSERT(nodes != NULL);
    return nodeHandlers[e.node]->simulateEvent(e);
  }
  uint32_t hashNode(int node) const {
    ASSERT(node < Sim::getNumNodes());
    ASSERT(nodes != NULL);
    return nodes[node]->hashState()^nodeHandlers[node]->hashState();
  }
  std::string toString(int node) const {
    ASSERT(node < Sim::getNumNodes());
    ASSERT(nodes != NULL);
    return nodes[node]->toString()+nodeHandlers[node]->toString();
  }
  std::string toStateString(int node) const {
    ASSERT(node < Sim::getNumNodes());
    ASSERT(nodes != NULL);
    return nodes[node]->toStateString()+nodeHandlers[node]->toStateString();
  }
  static void reset() {
    delete _sim_inst;
    _sim_inst = NULL;
  }
  virtual ~SimApplication() {
    for(int i = 0; i < Sim::getNumNodes(); i++) {
      delete nodeHandlers[i];
    }
    delete[] nodeHandlers;
    nodeHandlers = NULL;
  }
};

#endif
