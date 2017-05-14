#ifndef _SIM_H_
#define _SIM_H_

#include "MaceTypes.h"
#include "Log.h"
#include "Param.h"
#include "MaceTime.h"
#include "Event.h"
#include "mdeque.h"
#include "mace-macros.h"

class Sim : public mace::PrintPrintable {
  protected:
    static int runningNode;
    static int numNodes;
    static std::string** logPrefixes;
    static MaceKey** keys;
    static std::string* nodeString;
    static bool inited;

    static bool gusto;
    static bool gustoReq;
    static bool useGusto;

  public: //TODO: make non-public
    static bool isLive;
    static bool isSafe;
    static unsigned step;
    static unsigned maxStep;

  protected:
    //stats
    static unsigned phaseNum;
    static unsigned pathCount;
    static unsigned cumulativePathCount;
    static unsigned uniqueStateCount;
    static unsigned cumulativeUniqueStateCount;
    static unsigned countTMS;
    static unsigned cumulativeTmsCount;
    static unsigned countOOE;
    static unsigned cumulativeOoeCount;
    static unsigned countSC;
    static unsigned cumulativeScCount;
    static unsigned countDuplicate;
    static unsigned cumulativeDuplicateCount;
    static unsigned liveCount;
    static unsigned cumulativeLiveCount;
    static mace::deque<unsigned> simStepsDuplicate;
    static mace::deque<unsigned> randStepsDuplicate;
    static mace::deque<unsigned> simStepsTMS;
    static mace::deque<unsigned> randStepsTMS;
    static mace::deque<unsigned> simStepsOOE;
    static mace::deque<unsigned> randStepsOOE;
    static mace::deque<unsigned> simStepsSC;
    static mace::deque<unsigned> randStepsSC;
    static uint64_t startTime;
    static uint64_t lastPhaseEnd;

  public:
    enum PathEndCause { NO_MORE_EVENTS, DUPLICATE_STATE, STOPPING_CONDITION, TOO_MANY_STEPS };
    static void init(int nn) {
      ASSERT(!inited);
      inited = true;
      mace::MaceTime::simulated = true;
      startTime = Util::timeu();
      lastPhaseEnd = startTime;
      useGusto = Param::get("USE_GUSTO", false);

      numNodes = nn;
      logPrefixes = new std::string*[numNodes];
      keys = new MaceKey*[numNodes];
      nodeString = new std::string[numNodes];
      for(int i = 0; i < numNodes; i++) {
        keys[i] = new MaceKey(ipv4, i+1);
        logPrefixes[i] = new std::string(keys[i]->toString()+" ");
        nodeString[i] = boost::lexical_cast<std::string>(i);
      }
    }
    static int getNumNodes() { return numNodes; }
    static int getCurrentNode() { return runningNode; }
    static const MaceKey& getCurrentMaceKey() { return getMaceKey(runningNode); }
    static const MaceKey& getMaceKey(int node) { ASSERT(node < numNodes); return *keys[node]; }
    static int getNode(const MaceKey& key) {
      return key.getMaceAddr().local.addr-1;
    }
    //     static int getNode(const MaceKey& key) {
    //       for(int i = 0; i < numNodes; i++) {
    //         if(*(keys[i]) == key) {
    //           return i;
    //         }
    //       }
    //     }
    static void setCurrentNode(const MaceKey& key) {
      setCurrentNode(getNode(key));
    }
    static void setCurrentNode(int currentNode) {
      ASSERT(currentNode < numNodes);
      runningNode = currentNode;
      LogSelector::prefix = logPrefixes[runningNode];
      Params::Instance()->set("SIM_CURRENT_NODE", nodeString[runningNode]);
    }
#define FLUSH_LIST(x) macedbg(0) << #x "_path_steps " << simSteps##x << Log::endl; simSteps##x.clear(); macedbg(0) << #x "_path_rand " << randSteps##x << Log::endl; randSteps##x.clear();
    static void printStats(const std::string& nextPhaseStr) {
      ADD_SELECTORS("Sim::printStats");
      uint64_t now = Util::timeu();
      cumulativeUniqueStateCount += uniqueStateCount;
      cumulativeDuplicateCount += countDuplicate;
      cumulativeOoeCount += countOOE;
      cumulativeScCount += countSC;
      cumulativeTmsCount += countTMS;
      cumulativeLiveCount += liveCount;
      phaseNum++;
      if(!maceout.isNoop()) {
        FLUSH_LIST(Duplicate);
        FLUSH_LIST(OOE);
        FLUSH_LIST(SC);
        FLUSH_LIST(TMS);
        maceout << "Phase time " << (now - lastPhaseEnd) << " (cumulative) " << (now - startTime) << Log::endl;
        maceout << "Unique state count: " << uniqueStateCount << " (cumulative) " << cumulativeUniqueStateCount << Log::endl;
        maceout << "Paths Searched: " << pathCount << " (cumulative) " << cumulativePathCount << Log::endl;
        maceout << "Live path count: " << liveCount << " (cumulative) " << cumulativeLiveCount << Log::endl;
        maceout << "Abandonded duplicates: " << countDuplicate << " (cumulative) " << cumulativeDuplicateCount << Log::endl;
        maceout << "Out of events count: " << countOOE << " (cumulative) " << cumulativeOoeCount << Log::endl;
        maceout << "Stopping Condition count: " << countSC << " (cumulative) " << cumulativeScCount << Log::endl;
        maceout << "Too Many Steps count: " << countTMS << " (cumulative) " << cumulativeTmsCount << Log::endl;
        maceout << "Now on phase " << phaseNum << " : " << nextPhaseStr << Log::endl;
      }
      lastPhaseEnd = now;
      simStepsDuplicate.clear();
      randStepsDuplicate.clear();
      simStepsOOE.clear();
      randStepsOOE.clear();
      simStepsSC.clear();
      randStepsSC.clear();
      simStepsTMS.clear();
      randStepsTMS.clear();
      uniqueStateCount = 0;
      pathCount = 0;
      countDuplicate = 0;
      countOOE = 0;
      countSC = 0;
      countTMS = 0;
      liveCount = 0;
    }

    static bool reqGustoToggle() {
      gustoReq = !gustoReq;
      return gustoReq;
    }

    static void reqGusto(bool req) {
      gustoReq = req;
    }

    static void updateGusto() {
      gusto = gustoReq && useGusto;
    }

    static void clearGusto() {
      gusto = false;
      gustoReq = false;
    }

    static bool getGusto() {
      return gusto;
    }
    
    static bool getGustoReq() {
      return gustoReq;
    }
    
    static void markUniqueState() {
      uniqueStateCount++;
    }

#define ADD_TO_LIST(x) { count##x++; simSteps##x.push_back(simSteps); randSteps##x.push_back(randSteps); if(simSteps##x.size() >= 100) { FLUSH_LIST(x) } }

    static char* getCauseStr(PathEndCause cause) {
        switch(cause) {
          case NO_MORE_EVENTS: return "NO_MORE_EVENTS";
          case DUPLICATE_STATE: return "DUPLICATE_STATE";
          case STOPPING_CONDITION: return "STOPPING_CONDITION";
          case TOO_MANY_STEPS: return "TOO_MANY_STEPS";
          default: ASSERT(0);
        }
    }
    static void pathComplete(PathEndCause cause, bool isLive, bool isSafe, uint32_t simSteps, uint32_t randSteps, const mace::Printable* const randomUtil) {
      ADD_SELECTORS("Sim::pathComplete");
      unsigned mask = Param::get("search_print_mask",0x00000fff);
      ASSERT(isSafe);
      cumulativePathCount++;
      pathCount++;
      liveCount++;
      if((cumulativePathCount & mask) == 0) {
        maceout << "Path " << cumulativePathCount << " ( " << pathCount << " in phase) ended at simulator step: " << simSteps << " random step: " << randSteps << " cause: " << getCauseStr(cause) << " " << (*randomUtil) << Log::endl;
      }
      {
        ADD_SELECTORS("Sim::printStats");
        switch(cause) {
          case NO_MORE_EVENTS: ADD_TO_LIST(OOE); break;
          case DUPLICATE_STATE: ADD_TO_LIST(Duplicate); break;
          case STOPPING_CONDITION: ADD_TO_LIST(SC); break;
          case TOO_MANY_STEPS: ADD_TO_LIST(TMS); break;
          default: ASSERT(0);
        }
      }
    }

#undef FLUSH_LIST
#undef ADD_TO_LIST

    static unsigned getPathNum() { return cumulativePathCount; }

    virtual void  eventsWaiting(EventList& ev) const = 0;
    virtual std::string simulateEvent(const Event& e) = 0;
    virtual uint32_t hashState() const = 0;
    virtual ~Sim() {}
};

#endif
