#ifndef __EVENT_H
#define __EVENT_H

#include "mpair.h"
#include "mvector.h"

struct Event : public mace::PrintPrintable {
  public:
    enum EventType { APPLICATION, NETWORK, SCHEDULER };

  public:
    int node;
    EventType type;
    mace::string desc;
  
    mace::vector<int> simulatorVector;
    mace::vector<int> subVector;

    const char* getType(EventType t) const {
      switch (t) {
        case APPLICATION:
          return "APPLICATION";
        case NETWORK:
          return "NETWORK";
        case SCHEDULER:
          return "SCHEDULER";
        default:
          ASSERT(0);
      }
    }
    void print(std::ostream& out) const {
      out << "Event(node="<<node<<",desc="<<desc<<",type="<<getType(type)<<",simulatorVector="<<simulatorVector<<",subVector="<<subVector<<")";
    }

};

typedef mace::pair<Event, unsigned> EventWeight;
class EventList : public mace::vector<EventWeight> {
  public:
    typedef Event key_type;
    typedef unsigned mapped_type;

    void print(std::ostream& out) const {
      unsigned weight = 0;
      unsigned position = 0;
      for(EventList::const_iterator i = this->begin(); i != this->end(); i++) {
        out << "Item " << position << "[" << weight << "," << (weight+i->second) << "): " << i->first << " wt: " << i->second << std::endl;
        position++;
        weight += i->second;
      }
    }
};

#endif //__EVENT_H
