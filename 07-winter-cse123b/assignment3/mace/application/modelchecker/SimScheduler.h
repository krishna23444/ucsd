#ifndef _SIM_SCHED__H_
#define _SIM_SCHED__H_

#include "Sim.h"
#include "m_map.h"
#include "mvector.h"
#include "Iterator.h"
#include "MaceTypes.h"
#include "Util.h"
#include "mace-macros.h"
#include "Scheduler.h"
#include "SimulatorTransport.h"
#include "ReceiveDataHandler.h"

// typedef mace::vector<TimerHandler*> TimerList;
typedef mace::vector<EventWeight> TimerList;

class SimScheduler : public Sim, public Scheduler {
  protected:
    static SimScheduler* _sim_inst;
    TimerList scheduledTimers;
    SimScheduler() : Sim(), Scheduler() { }
    void runSchedulerThread() {}
  public:
    uint32_t hashState() const { return 0; }
    void print(std::ostream& out) const { }
    void reset() {
      scheduledTimers.clear();
    }
    uint64_t schedule(TimerHandler& timer, uint64_t time, bool abs = false) {
      ADD_FUNC_SELECTORS;
      if (!abs) {
        time += Util::timeu();
      }
      int current = getCurrentNode();
      EventWeight ew;
      ew.first.node = current;
      ew.first.type = Event::SCHEDULER;
      ew.first.simulatorVector.push_back((int)&timer);
      ew.first.desc = timer.getDescription();
      ew.second = 1;
      maceout << "Scheduling timer " << &timer << " for node " << current << Log::endl;
      scheduledTimers.push_back(ew);
      return time;
    }
    void cancel(TimerHandler& timer) {
      TimerList::iterator i;
      for(i = scheduledTimers.begin(); i != scheduledTimers.end(); i++) {
        if(i->first.simulatorVector[0] == (int)&timer) {
          break;
        }
      }
      if(i != scheduledTimers.end()) {
        scheduledTimers.erase(i);
      }
    }
    bool isSimulated() { return true; }

    void eventsWaiting(EventList& ev) const {
      ADD_SELECTORS("SimScheduler::eventWaiting");
      ev.insert(ev.end(), scheduledTimers.begin(), scheduledTimers.end());
      maceout << "Scheduler returning " << scheduledTimers.size() << " events" << Log::endl;
    }
    std::string simulateEvent(const Event& e) {
      ADD_FUNC_SELECTORS;
      maceLog("simulating timer for node %d\n", e.node);
      TimerList::iterator pos;
      for(pos = scheduledTimers.begin(); pos != scheduledTimers.end(); pos++) {
        if(e.simulatorVector[0] == pos->first.simulatorVector[0]) { break; }
      }
      if(pos != scheduledTimers.end()) {
        scheduledTimers.erase(pos);
      }
      TimerHandler* timer = (TimerHandler*)e.simulatorVector[0];
      timer->fire();
      std::ostringstream r;
      r << timer->getDescription() << "(" << timer->getId() << ")";
      return r.str();
    }
    static SimScheduler& Instance() { 
      if(_sim_inst == NULL) {
        _sim_inst = new SimScheduler();
      }
      if(scheduler == NULL) {
        scheduler = _sim_inst;
      }
      return *_sim_inst; 
    } 
    virtual ~SimScheduler() { }
};

#endif
