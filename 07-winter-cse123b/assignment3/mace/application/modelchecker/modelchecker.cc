#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <math.h>
#include "mace_constants.h"
#include <string>
#include <sstream>
// using std::string;
#include "NumberGen.h"
#include "ServiceInitializer.h"
#include "Log.h"
#include "pip_includer.h"
#include "mace.h"

#ifdef UseTritonShare
#include "TritonShare.h"
#endif

#ifdef UseBrokenTree
#include "BrokenTree.h"
#endif
#ifdef UseRandTree
#include "RandTree.h"
#endif
#ifdef UseOvercast
#include "Overcast.h"
#endif
#ifdef UsePastry
#include "Pastry.h"
#endif
#ifdef UseChord
#include "Chord.h"
#endif
#ifdef UseTransport
#include "MaceTransport.h"
#include "ProvisionalTransport.h"
#endif
#ifdef UseRanSub
#include "ReplayTree.h"
#include "RanSubAggregator.h"
#endif
#ifdef UseLynch
#include "AsynchLCR.h"
#endif
#ifdef UseMount
#include "Chord1.h"
#include "Chord5.h"
#include "MaceTransport6.h"
#endif
#ifdef UsePaxos
#include "Paxos.h"
#include "PaxosMembership.h"
#endif

#include "SimP2PApp.h"
#include "SimTreeApp.h"
#include "SimOverlayRouterApp.h"
#include "SimTransportApp.h"
#include "SimPTransportApp.h"
#include "SimAggregateApp.h"
#include "SimLCRApp.h"
#include "SimConsensusApp.h"
#include "MaceTime.h"

#include "Event.h"
#include "Sim.h"
#include "SimNetwork.h"
#include "SimulatorTCP.h"
using SimulatorTCP_namespace::SimulatorTCPService;
#include "SimulatorUDP.h"
using SimulatorUDP_namespace::SimulatorUDPService;
#include "RouteTransportWrapper.h"
typedef RouteTransportWrapper_namespace::RouteTransportWrapperService RouteTransportWrapper;
#include "SimScheduler.h"
#include "SimApplication.h"
#include "SearchRandomUtil.h"
#include "SearchStepRandomUtil.h"
//#include "BestFirstRandomUtil.h"
typedef SearchRandomUtil BestFirstRandomUtil;
#include "ReplayRandomUtil.h"
#include "LastNailRandomUtil.h"
#include "ThreadCreate.h"
#include "SysUtil.h"

#undef exit
// #undef ASSERT
// #define ASSERT(x)  if(!x) { Log::log("ERROR::ASSERT") << *randomUtil << Log::endl; std::ofstream out(("error"+lexical_cast<std::string>(error_path++)+".path").c_str()); randomUtil->printVSErrorPath(out); out.close(); abort(); }

class TestProperties {
  public:
    virtual bool testSafetyProperties() = 0;
    virtual bool testLivenessProperties() = 0;
    virtual ~TestProperties() {}
};

template<typename Service>
class SpecificTestProperties : public TestProperties {
  typedef mace::map<int, Service*> _NodeMap_;
  private:
    _NodeMap_ serviceNodes;
  public:
    SpecificTestProperties(const _NodeMap_& nodes) : serviceNodes(nodes) {
      ASSERT(nodes.size());
    }

    bool testSafetyProperties() {
      return Service::checkSafetyProperties(serviceNodes);
    }
    bool testLivenessProperties() {
      return Service::checkLivenessProperties(serviceNodes);
    }
};

class SimulatorEmptyProperty : public TestProperties {
public:
  SimulatorEmptyProperty(Sim& sim) : sim(sim) { }
  bool testSafetyProperties() { return true; }
  bool testLivenessProperties() {
    EventList ev;
    sim.eventsWaiting(ev);
    return ev.empty();
  }
private:
  Sim& sim;
};

const int APP_EVENT = 0;
const int NET_EVENT = 1;
const int SCHED_EVENT = 2;

const int BROKEN_TREE = 0;
const int RAND_TREE = 1;
const int OVERCAST = 2;
const int PASTRY = 3;
const int MACE_TRANSPORT = 4;
const int CHORD = 5;
const int RANSUB_AGGREGATOR = 6;
const int PROVISIONAL_TRANSPORT = 7;
const int SIM_TCP_TRANSPORT = 8;
const int CHORD1 = 9;
const int CHORD5 = 10;
const int MACE_TRANSPORT6 = 11;
const int ASYNCH_LCR = 12;
const int PAXOS = 13;
const int TRITONSHARE = 14;
int num_nodes;
int base_port;
std::string* nodeString;
uint64_t* mtime;

EventList getReadyEvents();
void simulateNode(const Event& e);
#if 0
bool noloops(int *numRoots = NULL, bool* nodesJoined = NULL, bool* kidsMatch = NULL);
bool pathendsatroot(int node, int root, int* parents);
#endif

using boost::lexical_cast;

bool waiting = false;
bool monitorOverride = false;
int checkedService = BROKEN_TREE;

class Stopwatch {
  private:
  uint64_t cumulativeTime;
  uint64_t startTime;
  uint64_t markTime;
  public:
  void start() {
    ASSERT(startTime == 0);
    startTime = Util::timeu();
  }
  void mark() {
    ASSERT(startTime != 0);
    markTime = Util::timeu();
  }
  void confirm() {
    ASSERT(markTime != 0);
    cumulativeTime += markTime - startTime;
    markTime = 0;
    startTime = 0;
  }
  void stop() {
    ASSERT(startTime != 0);
    mark();
    confirm();
  }
  uint64_t getCumulativeTime() { return cumulativeTime; }
};

class __Simulator__ {
  protected:
    static std::vector<uint32_t> nodeState;
    static std::vector<std::string> nodeStateStr;
    static std::vector<uint32_t> initialNodeState;
    static std::vector<std::string> initialNodeStateStr;
    static mace::hash_map<uint32_t, UIntList> systemStates;
    static uint32_t firstState;
    static hash_string hasher;
    static int error_path;

  public:
    static bool use_broken_hash;
    static bool use_hash;
    static bool assert_safety;
    static bool print_errors;
    static bool max_steps_error;
    static SimRandomUtil* randomUtil;
    typedef std::map<UIntList, char*> MemoryMap;
    static bool compareCMC;
    static bool randomAccessCMC;
    static bool runCMCmem;
    static MemoryMap cmcTestMap;
    static Stopwatch cmcTime;
    static Stopwatch macemcTime;
    static Stopwatch macemcTimeSim;
    static unsigned loggingStartStep;

    static void initState() {
      ADD_FUNC_SELECTORS;
      if(!__Simulator__::use_hash) { return; }
      static bool inited = false;
      if(!inited) {
        SimApplication& app = SimApplication::Instance();
        std::ostringstream out;
        inited = true;
        nodeState.resize(num_nodes);
        nodeStateStr.resize(num_nodes);
        firstState = 0;
        //XXX: update nodeState
        for(int i = 0; i < num_nodes; i++) {
          if(use_broken_hash) {
            nodeState[i] = app.hashNode(i);
          } else {
            nodeStateStr[i] = app.toStateString(i);
            out.write(nodeStateStr[i].data(), nodeStateStr[i].size());
            nodeState[i] = hasher(nodeStateStr[i]);
            firstState += nodeState[i];
          }
          maceout << "node " << i << " state " << nodeState[i] << Log::endl;
        }
        initialNodeState = nodeState;
        initialNodeStateStr = nodeStateStr;

        uint32_t tempState;

        Sim& net = SimNetwork::Instance();
        if(use_broken_hash) {
          tempState = net.hashState();
          maceout << "net state " << tempState << Log::endl;
          firstState += tempState;
        } else {
          net.print(out);
        }

        Sim& sched = SimScheduler::Instance();
        if(use_broken_hash) {
          tempState = sched.hashState();
          maceout << "sched state " << tempState << Log::endl;
          firstState += tempState;
        } else {
          sched.print(out);
        }

        if(!use_broken_hash) {
          firstState = hasher(out.str());
        }

        maceout << "firstState " << firstState << Log::endl;
        if(randomUtil->testSearchDepth() > -2) {
          systemStates[firstState] = randomUtil->getPath();
        }
      }
      else {
        nodeState = initialNodeState;
        nodeStateStr = initialNodeStateStr;
      }
    }

    static void dumpState() {
      ADD_SELECTORS("__Simulator__::dumpState");
      if(! maceout.isNoop()) {
        SimApplication& app = SimApplication::Instance();
        maceout << "Application State: " << app << std::endl;
        Sim& net = SimNetwork::Instance();
        maceout << "Network State: " << net << std::endl;
        Sim& sched = SimScheduler::Instance();
        maceout << "Scheduler State: " << sched << Log::endl;
      }
    }

    static bool isDuplicateState() {
      ADD_FUNC_SELECTORS;
      if(!__Simulator__::use_hash) { return false; }
      int depthTest = randomUtil->testSearchDepth();
      if(depthTest > 0) { return false; } //If search is past the interesting range, state is not considered duplicate
      uint32_t tempState = 0;
      SimApplication& app = SimApplication::Instance();
      nodeStateStr[Sim::getCurrentNode()] = app.toStateString(Sim::getCurrentNode());
      if(use_broken_hash) {
        nodeState[Sim::getCurrentNode()] = app.hashNode(Sim::getCurrentNode());
      } else {
        nodeState[Sim::getCurrentNode()] = hasher( nodeStateStr[Sim::getCurrentNode()]);
      }
      macedbg(1) << "current node " << Sim::getCurrentNode() << " state " << nodeState[Sim::getCurrentNode()] << Log::endl;
      // XXX - fix this for best first
//       if(depthTest < 0) { return false; } //If depth is before the search portion, don't bother noticing duplicate state
      if(depthTest < -1) { return false; } //If depth during the prefix portion, don't bother noticing duplicate state
      std::ostringstream out;
      for(int i = 0; i < num_nodes; i++) {
        if(use_broken_hash) {
          tempState += nodeState[i];
        }
        out <<  nodeStateStr[i];
      }

      Sim& net = SimNetwork::Instance();
      if(use_broken_hash) {
        tempState += net.hashState();
      }
      net.printState(out);
      Sim& sched = SimScheduler::Instance();
      if(use_broken_hash) {
        tempState += sched.hashState();
      }
      out << sched;

      if(!use_broken_hash) {
        tempState = hasher(out.str());
      }
      
      macedbg(1) << "current path " << *randomUtil << Log::endl;
      macedbg(1) << "current state " << tempState << " known states " << systemStates << Log::endl;
      if(!macedbg(1).isNoop()) {
        macedbg(1) << out.str() << Log::endl;
      }

      if(systemStates.containsKey(tempState)) { 
        UIntList& selections = systemStates[tempState];
        UIntList current = randomUtil->getPath();
        if(current.size() < selections.size()) {
          macedbg(1) << "Affirming shorter path with same state" << Log::endl;
          systemStates[tempState] = current;
          return false;
        } else if(selections == current) {
          return false;
        }
        macedbg(0) << "State Duplicated!" << Log::endl;

        return true;
        }
      #ifdef COMPARE_CMC
      if(__Simulator__::compareCMC) {
        if(__Simulator__::runCMCmem) {
          cmcTime.start();
          static char memory[40000];
          char* cur = new char[40000];
          memcpy(cur,memory,40000);
          cmcTestMap[randomUtil->getPath()] = cur;
          cmcTime.stop();
        }
      }
      #endif

      systemStates[tempState] = randomUtil->getPath();
      Sim::markUniqueState();
      return false;
    }

    static void Halt() {
      monitorOverride = true;
      Sim::printStats(__Simulator__::randomUtil->getPhase() + "::ERROR");
      monitorOverride = false;
      if(__Simulator__::print_errors) {
        Log::log("HALT") << "Output paths errorN.path for 0 < N < " << error_path << Log::endl;
      }
      raise(11);
    }
    static void Error(const std::string& type, bool exit = true) {
      if(__Simulator__::print_errors) {
        Log::log("ERROR::SIM::"+type) << *__Simulator__::randomUtil << Log::endl;
        static std::string error_tag = Params::Instance()->get<std::string>("ERROR_PATH_FILE_TAG", "");
        std::ofstream out(("error"+error_tag+lexical_cast<std::string>(error_path++)+".path").c_str());
        monitorOverride = true;
        randomUtil->printPath(out);
        monitorOverride = false;
        out.close();
      } else {
        Log::log("ERROR::SIM::"+type) << "error occured of type " << type << Log::endl;
      }
      if(exit) {
        Log::log("ERROR::SIM::"+type) << "exiting via signal 11" << Log::endl;
        Halt();
      } else {
        Log::log("ERROR::SIM::"+type) << "continuing as requested, " << error_path << " paths so far" << Log::endl;
      }
    }

    static void logAddresses() {
      static const log_id_t printAddress = Log::getId("NodeAddress");
      for(int i = 0; i < Sim::getNumNodes(); i++) {
        Log::log(printAddress) << "Node " << i << " address " << Sim::getMaceKey(i) << Log::endl;
      }
    }

};

void* monitor(void* dud) {
  ADD_FUNC_SELECTORS;
  bool divergence_assert = Params::Instance()->containsKey("divergence_assert");
  maceout << "monitor begun" << Log::endl;
  int timeout = Params::Instance()->get("divergence_timeout", 5);
  while(true) {
    waiting = true;
    SysUtil::sleep(timeout);
    if(waiting && !monitorOverride) {
      Log::enableLogging();
      __Simulator__::Error("DIVERGENCE", divergence_assert);
    }
  }
}

void sigHandler(int sig) {
//   std::cerr << "received signal " << sig << " my pid=" << getpid() << std::endl;
  if(sig == SIGINT && __Simulator__::randomUtil->handleInt()) {
    return;
  }
  Log::enableLogging();
  Log::log("ERROR::SIM::SIGNAL") << "Caught Signal " << sig << ", terminating" << Log::endl;
  __Simulator__::Error("SIGNAL");
}

std::vector<uint32_t> __Simulator__::nodeState;
std::vector<std::string> __Simulator__::nodeStateStr;
std::vector<uint32_t> __Simulator__::initialNodeState;
std::vector<std::string> __Simulator__::initialNodeStateStr;
mace::hash_map<uint32_t, UIntList> __Simulator__::systemStates;
uint32_t __Simulator__::firstState;
bool __Simulator__::use_broken_hash;
bool __Simulator__::use_hash;
bool __Simulator__::assert_safety;
bool __Simulator__::max_steps_error;
bool __Simulator__::print_errors;
int __Simulator__::error_path = 1;
SimRandomUtil* __Simulator__::randomUtil;
hash_string __Simulator__::hasher;
bool __Simulator__::compareCMC;
bool __Simulator__::runCMCmem;
bool __Simulator__::randomAccessCMC;
__Simulator__::MemoryMap __Simulator__::cmcTestMap;
Stopwatch __Simulator__::cmcTime;
Stopwatch __Simulator__::macemcTime;
Stopwatch __Simulator__::macemcTimeSim;
unsigned __Simulator__::loggingStartStep = 0;

//This version of stoppingCondition will continue search as usual
// bool stoppingCondition() {
//   return __Simulator__::randomUtil && __Simulator__::randomUtil->pathIsDone();
// }

typedef mace::vector<TestProperties*> TestPropertyList;

bool stoppingCondition(bool& isLive, bool& isSafe, const TestPropertyList& properties) {
  static int stopping = 0;
  static bool printPrefix = Params::Instance()->get("PRINT_SEARCH_PREFIX", false);
  static bool useRandomWalks = Params::Instance()->get("USE_RANDOM_WALKS", false);
  static bool testEarly = Params::Instance()->get("TEST_PROPERTIES_EARLY", true);

  ADD_SELECTORS("stoppingCondition");

  //Causes problems if the properties consult the randomUtil
  //   if(__Simulator__::randomUtil->testSearchDepth() < 0) {
  //     maceout << "Not testing properties since we have not yet reached search depth!" << Log::endl;
  //     isSafe = true;
  //     isLive = false; //could cache this?
  //     return false;
  //   }

  int searchDepthTest = __Simulator__::randomUtil->testSearchDepth();

  isSafe = true;

  if(testEarly || searchDepthTest >= 0) {
    for(TestPropertyList::const_iterator i = properties.begin(); i != properties.end(); i++) {
      isSafe = (*i)->testSafetyProperties();
      ASSERT(isSafe || !__Simulator__::assert_safety);
      if(!isSafe) {
        __Simulator__::Error("PROPERTY_FAILED");
      }
    } 

    if(!isSafe) { 
      isLive = false; // An unsafe path is definitely not a live path
      return true; // Unsafe paths definitely mean 
    }

    isLive = true;

    for(TestPropertyList::const_iterator i = properties.begin(); i != properties.end() && isLive; i++) {
      isLive &= (*i)->testLivenessProperties();
    }
  } else {
    isLive = false;
  }

  if(printPrefix && isLive && __Simulator__::randomUtil->implementsPrintPath()) {
    std::ofstream out(("prefix"+lexical_cast<std::string>(stopping++)+".path").c_str());
    __Simulator__::randomUtil->printPath(out);
    out.close();
  }

  return (searchDepthTest >= 0 && __Simulator__::randomUtil->pathIsDone() && (!useRandomWalks || isLive));
}

int main(int argc, char **argv)
{
  BaseMaceService::_printLower = true;
  SysUtil::signal(SIGABRT, sigHandler);
  SysUtil::signal(SIGINT, sigHandler);
  //   SysUtil::signal(SIGINT, sigHandler);


  // First load running parameters 
  Params::Instance()->addRequired("num_nodes", "Number of nodes to simulate");
  Params::Instance()->loadparams(argc, argv);
  Params::Instance()->set("MACE_SIMULATE_NODES", "MaceMC");
  Params::Instance()->print(stdout);

  Log::configure(Params::Instance());
  Log::disableDefaultWarning();
  Log::disableDefaultError();
//   LogSelectorTimestamp printTime = LOG_TIMESTAMP_HUMAN;
  LogSelectorTimestamp printTime = LOG_TIMESTAMP_DISABLED;
  LogSelectorThreadId printThreadId = LOG_THREADID_DISABLED;
  Log::add("ERROR",stdout,printTime,LOG_NAME_ENABLED,printThreadId);
  Log::autoAdd("ERROR::SIM",stdout,printTime,LOG_NAME_ENABLED,printThreadId);
  Log::add("SearchRandomUtil::next",stdout,printTime,LOG_NAME_ENABLED,printThreadId);
  Log::add("SearchStepRandomUtil::next",stdout,printTime,LOG_NAME_ENABLED,printThreadId);
  Log::add("BestFirstRandomUtil::next",stdout,printTime,LOG_NAME_ENABLED,printThreadId);
  Log::add("HALT",stdout,printTime,LOG_NAME_ENABLED,printThreadId);
  Log::autoAdd("monitor",stdout,printTime,LOG_NAME_ENABLED,printThreadId);
  Log::add("Sim::pathComplete",stdout,printTime,LOG_NAME_ENABLED);
  Log::add("Sim::printStats",stdout,printTime,LOG_NAME_ENABLED);
//   Log::add("DEBUG::static bool __Simulator__::isDuplicateState()",stdout,printTime,LOG_NAME_ENABLED);
  Log::autoAdd("LastNailRandomUtil::hasNext",stdout,printTime,LOG_NAME_ENABLED,printThreadId);
  Log::add("LastNailRandomUtil::next",stdout,printTime,LOG_NAME_ENABLED,printThreadId);
  Log::autoAdd("LastNailRandomUtil::dumpState",stdout,printTime,LOG_NAME_ENABLED,printThreadId);
  if(Params::Instance()->containsKey("TRACE_ALL")) {
    Log::autoAddAll(stdout,printTime,LOG_NAME_ENABLED,printThreadId);
  }
  if(Params::Instance()->containsKey("TRACE_SIMULATOR")) {
    Log::add("BestFirstRandomUtil::hasNext",stdout,printTime,LOG_NAME_ENABLED,printThreadId);
    Log::add("main",stdout,printTime,LOG_NAME_ENABLED,printThreadId);
    //     Log::autoAdd("Simulator");
    Log::add("Sim::getReadyEvents",stdout,printTime,LOG_NAME_ENABLED,printThreadId);
    Log::autoAdd("__Simulator__::isDuplicateState",stdout,printTime,LOG_NAME_ENABLED,printThreadId);
    Log::autoAdd("__Simulator__::initState",stdout,printTime,LOG_NAME_ENABLED,printThreadId);
    Log::autoAdd("simulateNode",stdout,printTime,LOG_NAME_ENABLED,printThreadId);
    Log::add("SearchRandomUtil::randInt",stdout,printTime,LOG_NAME_ENABLED,printThreadId);
    Log::add("SearchStepRandomUtil::randInt",stdout,printTime,LOG_NAME_ENABLED,printThreadId);
    Log::add("BestFirstRandomUtil::randInt",stdout,printTime,LOG_NAME_ENABLED,printThreadId);
    Log::add("LastNailRandomUtil::randInt",stdout,printTime,LOG_NAME_ENABLED,printThreadId);
    Log::add("ReplayRandomUtil::randInt",stdout,printTime,LOG_NAME_ENABLED,printThreadId);
  }
  if(Params::Instance()->containsKey("TRACE_SUBST")) {
    std::istringstream in(Params::Instance()->getStr("TRACE_SUBST"));
    while(in) {
      std::string s;
      in >> s;
      if(s.length() == 0) { break; }
      Log::autoAdd(s,stdout,printTime,LOG_NAME_ENABLED,printThreadId);
    }
  }
  if(Params::Instance()->containsKey("TRACE_STATE")) {
    Log::autoAdd("__Simulator__::dumpState");
  }
  if(Params::Instance()->containsKey("TRACE_STEP")) {
    Log::add("main",stdout,printTime,LOG_NAME_ENABLED,printThreadId);
  }

  if(Params::Instance()->containsKey("CHECKED_SERVICE")) {
    std::string svstr = Params::Instance()->getStr("CHECKED_SERVICE");
    if(svstr == "RandTree") {
      checkedService = RAND_TREE;
    }
    else if(svstr == "TritonShare") {
	checkedService = TRITONSHARE;
    }
    else if(svstr == "Overcast") {
      checkedService = OVERCAST;
    }
    else if(svstr == "BrokenTree") {
      checkedService = BROKEN_TREE;
    }
    else if(svstr == "Pastry") {
      checkedService = PASTRY;
    }
    else if(svstr == "Chord") {
      checkedService = CHORD;
    }
    else if(svstr == "ChordAssert") {
      checkedService = CHORD1;
    }
    else if(svstr == "ChordPartition") {
      checkedService = CHORD5;
    }
    else if(svstr == "MaceTransport") {
      checkedService = MACE_TRANSPORT;
    }
    else if (svstr == "MaceTransportLiveness") {
      checkedService = MACE_TRANSPORT6;
    }
    else if (svstr == "ProvisionalTransport") {
      checkedService = PROVISIONAL_TRANSPORT;
    }
    else if (svstr == "SimTcpTransport") {
      checkedService = SIM_TCP_TRANSPORT;
    }
    else if(svstr == "RanSubAggregator") {
      checkedService = RANSUB_AGGREGATOR;
    }
    else if(svstr == "AsynchLCR") {
      checkedService = ASYNCH_LCR;
    }
    else if(svstr == "Paxos") {
      checkedService = PAXOS;
    }
    else {
      Log::err() << "Invalid checked service type " << svstr << " specified!" << Log::endl;
      ASSERT(0);
    }
  }
  
  __Simulator__::use_hash = Params::Instance()->get("USE_STATE_HASHES", false);
  __Simulator__::use_broken_hash = Params::Instance()->get("USE_BROKEN_HASH", false);
  __Simulator__::compareCMC = Params::Instance()->get("COMPARE_CMC", false);
  __Simulator__::runCMCmem = Params::Instance()->get("RUN_CMC_MEM", false);
  __Simulator__::randomAccessCMC = Params::Instance()->get("RANDOM_ACCESS_CMC", false);
  __Simulator__::loggingStartStep = Param::get<unsigned>("LOGGING_START_STEP", 0);

  // Make sure the running scheduler is our simulator one.
  SimScheduler::Instance();
  bool divergenceMonitor = true;
  int max_dead_paths = Params::Instance()->get("MAX_DEAD_PATHS", 1);
  // Make sure our random number generator is the verisoft one
  if(Params::Instance()->containsKey("RANDOM_REPLAY_FILE")) {
    __Simulator__::use_hash = false;
    __Simulator__::assert_safety = true;
    if(Params::Instance()->get<std::string>("RANDOM_REPLAY_FILE") == "-") {
      __Simulator__::print_errors = true;
      srand48(Util::timeu());
    } else {
      __Simulator__::print_errors = false;
      srand48(0);
    }
    __Simulator__::max_steps_error = true;
    __Simulator__::randomUtil = ReplayRandomUtil::SetInstance();
    divergenceMonitor = Params::Instance()->get("RUN_DIVERGENCE_MONITOR",false);
  } else if(Params::Instance()->containsKey("LAST_NAIL_REPLAY_FILE")) {
    srand48(0);
    max_dead_paths = INT_MAX;
    __Simulator__::use_hash = false;
    __Simulator__::assert_safety = false;
    //     __Simulator__::print_errors = false;
    __Simulator__::print_errors = true;
    __Simulator__::max_steps_error = false;
    __Simulator__::randomUtil = LastNailRandomUtil::SetInstance();
    divergenceMonitor = Params::Instance()->get("RUN_DIVERGENCE_MONITOR",true);
    std::string statfilestr = "lastnail_" + Params::Instance()->get<std::string>("STAT_FILE", "stats.log");
    Params::Instance()->set("ERROR_PATH_FILE_TAG", "-lastnail-");
    FILE* statfile = fopen(statfilestr.c_str(), "w");
    if(statfile == NULL) {
      Log::perror("Could not open stat file");
      exit(1);
    }
    Log::add("Sim::printStats",statfile,LOG_TIMESTAMP_DISABLED,LOG_NAME_DISABLED,LOG_THREADID_DISABLED);
    Log::add("DEBUG::Sim::printStats",statfile,LOG_TIMESTAMP_DISABLED,LOG_NAME_DISABLED,LOG_THREADID_DISABLED);
  } else {
    srand48(0);
    __Simulator__::print_errors = true;
    __Simulator__::assert_safety = false;
    __Simulator__::max_steps_error = true;
    if (Params::Instance()->get("USE_BEST_FIRST", true)) {
      __Simulator__::randomUtil = BestFirstRandomUtil::SetInstance();
    }
    else if(Params::Instance()->get("USE_STEP_SEARCH", false)) {
      __Simulator__::randomUtil = SearchStepRandomUtil::SetInstance();
    }
    else {
      __Simulator__::randomUtil = SearchRandomUtil::SetInstance();
    }
    divergenceMonitor = Params::Instance()->get("RUN_DIVERGENCE_MONITOR",true);
    FILE* statfile = fopen(Params::Instance()->get<std::string>("STAT_FILE", "stats.log").c_str(), "w");
    if(statfile == NULL) {
      Log::perror("Could not open stat file");
      exit(1);
    }
    Log::add("Sim::printStats",statfile,LOG_TIMESTAMP_DISABLED,LOG_NAME_DISABLED,LOG_THREADID_DISABLED);
    Log::add("DEBUG::Sim::printStats",statfile,LOG_TIMESTAMP_DISABLED,LOG_NAME_DISABLED,LOG_THREADID_DISABLED);
  }
  if(divergenceMonitor) {
    pthread_t tid;
    runNewThread(&tid, monitor, NULL, NULL);
  }

  base_port = Params::Instance()->get("MACE_PORT", 5377);
  num_nodes = Params::Instance()->getInt("num_nodes");
  Sim::init(num_nodes);
  nodeString = new std::string[num_nodes];
  Sim::maxStep = Params::Instance()->get("max_num_steps", UINT_MAX);
  int queue_size = Params::Instance()->get("queue_size", 20);
  SimNetwork::SetInstance(base_port);

  MaceKey root(ipv4, 1);
  NodeSet allNodes;

  for(int i = 0; i < num_nodes; i++) {
    nodeString[i] = boost::lexical_cast<std::string>(i);
    allNodes.insert(Sim::getMaceKey(i));
  }

  __Simulator__::logAddresses();
  Log::log("main") << "Starting simulation" << Log::endl;

  int num_dead_paths = 0;
  unsigned maxPaths = (Params::Instance()->containsKey("MAX_PATHS")?Params::Instance()->getInt("MAX_PATHS"):UINT_MAX);
  do {
    ADD_SELECTORS("main");

    if(__Simulator__::randomUtil->next()) {
      monitorOverride = true;
      Sim::printStats(__Simulator__::randomUtil->getPhase());
      monitorOverride = false;
    }

    Sim::clearGusto();

    #ifdef COMPARE_CMC
    if(__Simulator__::compareCMC) {
      if(__Simulator__::runCMCmem) {
        __Simulator__::cmcTime.start();
        static char memory[40000];
        if(!__Simulator__::cmcTestMap.empty()) {
          UIntList i = __Simulator__::randomUtil->getPath();
          i.pop_back();
          __Simulator__::MemoryMap::iterator j = __Simulator__::cmcTestMap.find(i);
          if(j == __Simulator__::cmcTestMap.end()) { 
            //prefix may not match perfectly.
            int pos = 0;
            if(__Simulator__::randomAccessCMC) {
              pos = (int)(drand48() * __Simulator__::cmcTestMap.size());
            }
            for(j = __Simulator__::cmcTestMap.begin(); pos > 0; j++, pos--);
          }
          char* start = j->second;
          memcpy(memory, start, 40000);
        }
        __Simulator__::cmcTime.stop();
      }
      __Simulator__::macemcTime.start();
    }
    #endif

    mace::vector<TestProperties*> propertiesToTest;
    mace::vector<BaseMaceService*> servicesToDelete;
    SimApplicationServiceClass** appNodes = new SimApplicationServiceClass*[num_nodes];

    mtime = new uint64_t[num_nodes];
    for (int i = 0; i < num_nodes; i++) {
      mtime[i] = 0;
    }

    #ifdef UseBrokenTree
    if(checkedService == BROKEN_TREE) {
      mace::map<int, BrokenTree_namespace::BrokenTreeService*> brokenTreeNodes;
      mace::map<int, SimTreeApp_namespace::SimTreeAppService*> simTreeNodes;
      for(int i = 0; i < num_nodes; i++) {
        waiting = false;
        Sim::setCurrentNode(i);
        MaceKey key = Sim::getCurrentMaceKey();
        SimulatorTCPService* tcp = new SimulatorTCPService(queue_size, base_port, i);
        RouteTransportWrapper* rtw = new RouteTransportWrapper(*tcp);
	servicesToDelete.push_back(rtw);
        BrokenTree_namespace::BrokenTreeService* tree = new BrokenTree_namespace::BrokenTreeService(*rtw, root, num_nodes);
	servicesToDelete.push_back(tree);
        brokenTreeNodes[i] = tree;
        SimTreeApp_namespace::SimTreeAppService* app = new SimTreeApp_namespace::SimTreeAppService(*tree, *tree, SimTreeApp_namespace::ROOT_ONLY, 0, key, num_nodes);
	servicesToDelete.push_back(app);
        simTreeNodes[i] = app;
        appNodes[i] = app;
      }
      propertiesToTest.push_back(new SpecificTestProperties<BrokenTree_namespace::BrokenTreeService>(brokenTreeNodes));
      propertiesToTest.push_back(new SpecificTestProperties<SimTreeApp_namespace::SimTreeAppService>(simTreeNodes));
    }
    else 
    #endif
    #ifdef UseRandTree
    if(checkedService == RAND_TREE) {
      mace::map<int, RandTree_namespace::RandTreeService*> randTreeNodes;
      mace::map<int, SimTreeApp_namespace::SimTreeAppService*> simTreeNodes;
      mace::map<int, SimulatorTCPService*> tcpNodes;
      for(int i = 0; i < num_nodes; i++) {
        waiting = false;
        Sim::setCurrentNode(i);
        MaceKey key = Sim::getCurrentMaceKey();
        SimulatorTCPService* tcp = new SimulatorTCPService(queue_size, base_port, i);
        tcpNodes[i] = tcp;
        RandTree_namespace::RandTreeService* tree = new RandTree_namespace::RandTreeService(*tcp, 2);
	servicesToDelete.push_back(tree);
        randTreeNodes[i] = tree;
        SimTreeApp_namespace::SimTreeAppService* app = new SimTreeApp_namespace::SimTreeAppService(*tree, *tree, SimTreeApp_namespace::NEXT_NODE, 1, key, num_nodes);
	servicesToDelete.push_back(app);
        simTreeNodes[i] = app;
        appNodes[i] = app;
      }
      propertiesToTest.push_back(new SpecificTestProperties<RandTree_namespace::RandTreeService>(randTreeNodes));
      propertiesToTest.push_back(new SpecificTestProperties<SimTreeApp_namespace::SimTreeAppService>(simTreeNodes));
      propertiesToTest.push_back(new SpecificTestProperties<SimulatorTCPService>(tcpNodes));
    }
    else 
    #endif
    #ifdef UseTritonShare
    if(checkedService == TRITONSHARE) {
      mace::map<int, TritonShare_namespace::TritonShareService*> randTreeNodes;
      mace::map<int, SimP2PApp_namespace::SimP2PAppService*> simTreeNodes;
      mace::map<int, SimulatorTCPService*> tcpNodes;
      for(int i = 0; i < num_nodes; i++) {
        waiting = false;
        Sim::setCurrentNode(i);
        MaceKey key = Sim::getCurrentMaceKey();
        SimulatorTCPService* tcp = new SimulatorTCPService(queue_size, base_port, i);
        tcpNodes[i] = tcp;
        //TritonShare_namespace::TritonShareService* tree = new TritonShare_namespace::TritonShareService(*tcp, 2);
	TritonShare_namespace::TritonShareService* tree = new TritonShare_namespace::TritonShareService(*tcp);
	servicesToDelete.push_back(tree);
        randTreeNodes[i] = tree;
        //SimP2PApp_namespace::SimP2PAppService* app = new SimP2PApp_namespace::SimP2PAppService(*tree, *tree, SimP2PApp_namespace::NEXT_NODE, 1, key, num_nodes);
	//SimP2PApp_namespace::SimP2PAppService* app = new SimP2PApp_namespace::SimP2PAppService(*tree, *tree, 0, 1, key, num_nodes);
	SimP2PApp_namespace::SimP2PAppService* app = new SimP2PApp_namespace::SimP2PAppService(*tree);
	servicesToDelete.push_back(app);
        simTreeNodes[i] = app;
        appNodes[i] = app;
      }
      propertiesToTest.push_back(new SpecificTestProperties<TritonShare_namespace::TritonShareService>(randTreeNodes));
      propertiesToTest.push_back(new SpecificTestProperties<SimP2PApp_namespace::SimP2PAppService>(simTreeNodes));
      propertiesToTest.push_back(new SpecificTestProperties<SimulatorTCPService>(tcpNodes));
    }
    else 
    #endif
    #ifdef UseOvercast  
    if(checkedService == OVERCAST) {
      mace::map<int, Overcast_namespace::OvercastService*> overcastNodes;
      mace::map<int, SimTreeApp_namespace::SimTreeAppService*> simTreeNodes;
      for(int i = 0; i < num_nodes; i++) {
        waiting = false;
        Sim::setCurrentNode(i);
        MaceKey key = Sim::getCurrentMaceKey();
        SimulatorTCPService* tcp = new SimulatorTCPService(queue_size, base_port, i);
        Overcast_namespace::OvercastService* tree = new Overcast_namespace::OvercastService(*tcp);
	servicesToDelete.push_back(tree);
        overcastNodes[i] = tree;
        SimTreeApp_namespace::SimTreeAppService* app = new SimTreeApp_namespace::SimTreeAppService(*tree, *tree, SimTreeApp_namespace::ROOT_ONLY, 1, key, num_nodes);
	servicesToDelete.push_back(app);
        simTreeNodes[i] = app;
        appNodes[i] = app;
      }
      propertiesToTest.push_back(new SpecificTestProperties<Overcast_namespace::OvercastService>(overcastNodes));
      propertiesToTest.push_back(new SpecificTestProperties<SimTreeApp_namespace::SimTreeAppService>(simTreeNodes));
    }
    else 
    #endif
    #ifdef UsePastry
    if(checkedService == PASTRY) {
      mace::map<int, Pastry_namespace::PastryService*> pastryNodes;
      mace::map<int, SimulatorTCPService*> tcpNodes;
      for(int i = 0; i < num_nodes; i++) {
        waiting = false;
        Sim::setCurrentNode(i);
        MaceKey key = Sim::getCurrentMaceKey();
        SimulatorTCPService* tcp = new SimulatorTCPService(queue_size, base_port, i);
        tcpNodes[i] = tcp;
        SimulatorUDPService* udp = new SimulatorUDPService(base_port+1, i);
        Pastry_namespace::PastryService* tree = new Pastry_namespace::PastryService(*tcp, *udp);
	servicesToDelete.push_back(tree);
        pastryNodes[i] = tree;
        SimOverlayRouterApp_namespace::SimOverlayRouterAppService* app = new SimOverlayRouterApp_namespace::SimOverlayRouterAppService(*tree, SimOverlayRouterApp_namespace::ROOT_ONLY, 1, key, num_nodes);
	servicesToDelete.push_back(app);
        appNodes[i] = app;
      }
      propertiesToTest.push_back(new SpecificTestProperties<Pastry_namespace::PastryService>(pastryNodes));
      propertiesToTest.push_back(new SpecificTestProperties<SimulatorTCPService>(tcpNodes));
    }
    else
    #endif
    #ifdef UseChord
    if(checkedService == CHORD) {
      mace::map<int, Chord_namespace::ChordService*> chordNodes;
      mace::map<int, SimulatorTCPService*> tcpNodes;
      for(int i = 0; i < num_nodes; i++) {
        waiting = false;
        Sim::setCurrentNode(i);
        MaceKey key = Sim::getCurrentMaceKey();
        SimulatorTCPService* tcp = new SimulatorTCPService(queue_size, base_port, i);
        tcpNodes[i] = tcp;
        Chord_namespace::ChordService* ch = new Chord_namespace::ChordService(*tcp);
	servicesToDelete.push_back(ch);
        chordNodes[i] = ch;
        SimOverlayRouterApp_namespace::SimOverlayRouterAppService* app = new SimOverlayRouterApp_namespace::SimOverlayRouterAppService(*ch, SimOverlayRouterApp_namespace::/*ROOT_ONLY*/RANDOM_LESSER, 1, key, num_nodes);
	servicesToDelete.push_back(app);
        appNodes[i] = app;
      }
      propertiesToTest.push_back(new SpecificTestProperties<Chord_namespace::ChordService>(chordNodes));
      propertiesToTest.push_back(new SpecificTestProperties<SimulatorTCPService>(tcpNodes));
    }
    else
    #endif
    #ifdef UseLynch
    if(checkedService == ASYNCH_LCR) {
      mace::map<int, AsynchLCR_namespace::AsynchLCRService*> lcrNodes;
      mace::map<int, SimLCRApp_namespace::SimLCRAppService*> simAppNodes;
      for(int i = 0; i < num_nodes; i++) {
        waiting = false;
        Sim::setCurrentNode(i);
        MaceKey key = Sim::getCurrentMaceKey();
        MaceKey next = Sim::getMaceKey((i+1)%num_nodes);
        SimulatorTCPService* tcp = new SimulatorTCPService(queue_size, base_port, i);
        AsynchLCR_namespace::AsynchLCRService* ch = new AsynchLCR_namespace::AsynchLCRService(*tcp, next, key);
	servicesToDelete.push_back(ch);
        lcrNodes[i] = ch;
        SimLCRApp_namespace::SimLCRAppService* app = new SimLCRApp_namespace::SimLCRAppService(*ch, i == (num_nodes -1));
	servicesToDelete.push_back(app);
        simAppNodes[i] = app;
        appNodes[i] = app;
        app->maceInit();
      }
      propertiesToTest.push_back(new SpecificTestProperties<SimLCRApp_namespace::SimLCRAppService>(simAppNodes));
    }
    else
    #endif
    #ifdef UsePaxos
    if(checkedService == PAXOS) {
      mace::map<int, SimConsensusApp_namespace::SimConsensusAppService*> simAppNodes;
      for(int i = 0; i < num_nodes; i++) {
        waiting = false;
        Sim::setCurrentNode(i);
        MaceKey key = Sim::getCurrentMaceKey();
        SimulatorUDPService* udp = new SimulatorUDPService(base_port, i);
        PaxosMembership_namespace::PaxosMembershipService* membership = new PaxosMembership_namespace::PaxosMembershipService(*udp);
        membership->maceInit();
        membership->setMembership(MaceKey::null, allNodes, -1);
	servicesToDelete.push_back(membership);
        Paxos_namespace::PaxosService* ch = new Paxos_namespace::PaxosService(*membership,*membership,*udp);
	servicesToDelete.push_back(ch);
        static int numProposers = Param::get("NODES_TO_PROPOSE", allNodes.size());
        SimConsensusApp_namespace::SimConsensusAppService* app = new SimConsensusApp_namespace::SimConsensusAppService(*ch, numProposers);
	app->maceInit();
	servicesToDelete.push_back(app);
        simAppNodes[i] = app;
        appNodes[i] = app;
      }
      propertiesToTest.push_back(new SpecificTestProperties<SimConsensusApp_namespace::SimConsensusAppService>(simAppNodes));
      propertiesToTest.push_back(new SimulatorEmptyProperty(SimNetwork::Instance()));
    }
    else
    #endif
    #ifdef UseMount
    if(checkedService == CHORD1) {
      mace::map<int, Chord1_namespace::Chord1Service*> chordNodes;
      mace::map<int, SimulatorTCPService*> tcpNodes;
      for(int i = 0; i < num_nodes; i++) {
        waiting = false;
        Sim::setCurrentNode(i);
        MaceKey key = Sim::getCurrentMaceKey();
        SimulatorTCPService* tcp = new SimulatorTCPService(queue_size, base_port, i);
        tcpNodes[i] = tcp;
        Chord1_namespace::Chord1Service* ch = new Chord1_namespace::Chord1Service(*tcp);
	servicesToDelete.push_back(ch);
        chordNodes[i] = ch;
        SimOverlayRouterApp_namespace::SimOverlayRouterAppService* app = new SimOverlayRouterApp_namespace::SimOverlayRouterAppService(*ch, SimOverlayRouterApp_namespace::/*ROOT_ONLY*/RANDOM_LESSER, 1, key, num_nodes);
	servicesToDelete.push_back(app);
        appNodes[i] = app;
      }
      propertiesToTest.push_back(new SpecificTestProperties<Chord1_namespace::Chord1Service>(chordNodes));
      propertiesToTest.push_back(new SpecificTestProperties<SimulatorTCPService>(tcpNodes));
    }
    else
    if(checkedService == CHORD5) {
      mace::map<int, Chord5_namespace::Chord5Service*> chordNodes;
      mace::map<int, SimulatorTCPService*> tcpNodes;
      for(int i = 0; i < num_nodes; i++) {
        waiting = false;
        Sim::setCurrentNode(i);
        MaceKey key = Sim::getCurrentMaceKey();
        SimulatorTCPService* tcp = new SimulatorTCPService(queue_size, base_port, i);
        tcpNodes[i] = tcp;
        Chord5_namespace::Chord5Service* ch = new Chord5_namespace::Chord5Service(*tcp);
	servicesToDelete.push_back(ch);
        chordNodes[i] = ch;
        SimOverlayRouterApp_namespace::SimOverlayRouterAppService* app = new SimOverlayRouterApp_namespace::SimOverlayRouterAppService(*ch, SimOverlayRouterApp_namespace::/*ROOT_ONLY*/RANDOM_LESSER, 1, key, num_nodes);
	servicesToDelete.push_back(app);
        appNodes[i] = app;
      }
      propertiesToTest.push_back(new SpecificTestProperties<Chord5_namespace::Chord5Service>(chordNodes));
      propertiesToTest.push_back(new SpecificTestProperties<SimulatorTCPService>(tcpNodes));
    }
    else 
    if(checkedService == MACE_TRANSPORT6) {
      mace::map<int, MaceTransport6_namespace::MaceTransport6Service*> transportNodes;
      mace::map<int, SimTransportApp_namespace::SimTransportAppService*> simTransportNodes;
      for (int i = 0; i < num_nodes; i++) {
        waiting = false;
        Sim::setCurrentNode(i);
        SimulatorUDPService* udp = new SimulatorUDPService(base_port, i);
	MaceTransport6_namespace::MaceTransport6Service* t = new MaceTransport6_namespace::MaceTransport6Service(*udp, true, queue_size, UINT_MAX);
	servicesToDelete.push_back(t);
	SimTransportApp_namespace::SimTransportAppService* app = new SimTransportApp_namespace::SimTransportAppService(*t);
	servicesToDelete.push_back(app);
	appNodes[i] = app;
	transportNodes[i] = t;
	simTransportNodes[i] = app;
      }
      propertiesToTest.push_back(new SpecificTestProperties<MaceTransport6_namespace::MaceTransport6Service>(transportNodes));
      propertiesToTest.push_back(new SpecificTestProperties<SimTransportApp_namespace::SimTransportAppService>(simTransportNodes));
      propertiesToTest.push_back(new SimulatorEmptyProperty(SimNetwork::Instance()));
      propertiesToTest.push_back(new SimulatorEmptyProperty(SimScheduler::Instance()));
    }
    else
    #endif
    #ifdef UseTransport
    if(checkedService == MACE_TRANSPORT) {
      mace::map<int, MaceTransport_namespace::MaceTransportService*> transportNodes;
      mace::map<int, SimTransportApp_namespace::SimTransportAppService*> simTransportNodes;
      for (int i = 0; i < num_nodes; i++) {
        waiting = false;
        Sim::setCurrentNode(i);
        SimulatorUDPService* udp = new SimulatorUDPService(base_port, i);
	MaceTransport_namespace::MaceTransportService* t = new MaceTransport_namespace::MaceTransportService(*udp, true, queue_size, UINT_MAX);
	servicesToDelete.push_back(t);
	SimTransportApp_namespace::SimTransportAppService* app = new SimTransportApp_namespace::SimTransportAppService(*t);
	servicesToDelete.push_back(app);
	appNodes[i] = app;
	transportNodes[i] = t;
	simTransportNodes[i] = app;
      }
      propertiesToTest.push_back(new SpecificTestProperties<MaceTransport_namespace::MaceTransportService>(transportNodes));
      propertiesToTest.push_back(new SpecificTestProperties<SimTransportApp_namespace::SimTransportAppService>(simTransportNodes));
      propertiesToTest.push_back(new SimulatorEmptyProperty(SimNetwork::Instance()));
      propertiesToTest.push_back(new SimulatorEmptyProperty(SimScheduler::Instance()));
    }
    else if (checkedService == PROVISIONAL_TRANSPORT) {
      mace::map<int, ProvisionalTransport_namespace::ProvisionalTransportService*> transportNodes;
      mace::map<int, SimPTransportApp_namespace::SimPTransportAppService*> simTransportNodes;
      for (int i = 0; i < num_nodes; i++) {
        waiting = false;
        Sim::setCurrentNode(i);
        SimulatorTCPService* tcp = new SimulatorTCPService(1, base_port, i);
	ProvisionalTransport_namespace::ProvisionalTransportService* t = new ProvisionalTransport_namespace::ProvisionalTransportService(*tcp, 1, 1);
	servicesToDelete.push_back(t);
	SimPTransportApp_namespace::SimPTransportAppService* app = new SimPTransportApp_namespace::SimPTransportAppService(*tcp, *t);
	servicesToDelete.push_back(app);
	appNodes[i] = app;
	transportNodes[i] = t;
	simTransportNodes[i] = app;
      }
//       propertiesToTest.push_back(new SpecificTestProperties<ProvisionalTransport_namespace::ProvisionalTransportService>(transportNodes));
      propertiesToTest.push_back(new SpecificTestProperties<SimPTransportApp_namespace::SimPTransportAppService>(simTransportNodes));
      propertiesToTest.push_back(new SimulatorEmptyProperty(SimNetwork::Instance()));
//       propertiesToTest.push_back(new SimulatorEmptyProperty(SimScheduler::Instance()));
    }
    else 
    #endif
    #ifdef UseRanSub
    if(checkedService == RANSUB_AGGREGATOR) {
      mace::map<int, RanSubAggregator_namespace::RanSubAggregatorService*> aggregateNodes;
      mace::map<int, SimAggregateApp_namespace::SimAggregateAppService*> simAggregateNodes;
      Params::Instance()->set("root", "1.0.0.0");
      for (int i = 0; i < num_nodes; i++) {
        waiting = false;
        Sim::setCurrentNode(i);
        MaceKey key = Sim::getCurrentMaceKey();
        SimulatorTCPService* tcp = new SimulatorTCPService(queue_size, base_port, i);
	ReplayTree_namespace::ReplayTreeService* t = new ReplayTree_namespace::ReplayTreeService(*tcp);
	servicesToDelete.push_back(t);
	RanSubAggregator_namespace::RanSubAggregatorService* rsagg = new RanSubAggregator_namespace::RanSubAggregatorService(*t, *tcp, 5*1000*1000);
	servicesToDelete.push_back(rsagg);
	SimAggregateApp_namespace::SimAggregateAppService* app = new SimAggregateApp_namespace::SimAggregateAppService(*t, *rsagg, SimAggregateApp_namespace::ROOT_ONLY, 1, key);
	servicesToDelete.push_back(app);
	appNodes[i] = app;
	aggregateNodes[i] = rsagg;
	simAggregateNodes[i] = app;
      }
      propertiesToTest.push_back(new SpecificTestProperties<RanSubAggregator_namespace::RanSubAggregatorService>(aggregateNodes));
      propertiesToTest.push_back(new SpecificTestProperties<SimAggregateApp_namespace::SimAggregateAppService>(simAggregateNodes));
    }
    else 
    #endif
    if(checkedService == SIM_TCP_TRANSPORT) {
      mace::map<int, SimTransportApp_namespace::SimTransportAppService*> simTransportNodes;
      mace::map<int, SimulatorTCPService*> tcpNodes;
      for (int i = 0; i < num_nodes; i++) {
        waiting = false;
        Sim::setCurrentNode(i);
        SimulatorTCPService* t = new SimulatorTCPService(queue_size, base_port, i);
        tcpNodes[i] = t;
	SimTransportApp_namespace::SimTransportAppService* app = new SimTransportApp_namespace::SimTransportAppService(*t);
	servicesToDelete.push_back(app);
	appNodes[i] = app;
	simTransportNodes[i] = app;
      }
      propertiesToTest.push_back(new SpecificTestProperties<SimTransportApp_namespace::SimTransportAppService>(simTransportNodes));
      propertiesToTest.push_back(new SpecificTestProperties<SimulatorTCPService>(tcpNodes));
      propertiesToTest.push_back(new SimulatorEmptyProperty(SimNetwork::Instance()));
      propertiesToTest.push_back(new SimulatorEmptyProperty(SimScheduler::Instance()));
    }
    else
    {
      assert(0);
    }

    SimApplication::SetInstance(appNodes);

    if(__Simulator__::use_hash) {
      __Simulator__::initState();
    }

    #ifdef COMPARE_CMC
    if(__Simulator__::compareCMC) {
      __Simulator__::macemcTime.stop();
      __Simulator__::macemcTimeSim.start();
      __Simulator__::macemcTimeSim.mark();
    }
    
    bool macemcTimerRunning = true; //only needed for compareCMC
    #endif
    if(__Simulator__::loggingStartStep > 0) {
      Log::disableLogging();
    }

    Sim::PathEndCause cause = Sim::TOO_MANY_STEPS;
    for(Sim::step = 0; Sim::step < Sim::maxStep; Sim::step++) {
      #ifdef COMPARE_CMC
      if(__Simulator__::compareCMC && macemcTimerRunning) {
        int tests = __Simulator__::randomUtil->testSearchDepth();
        if(tests < 0) {
          __Simulator__::macemcTimeSim.mark();
        }
        else if(tests >= 0) {
          __Simulator__::macemcTimeSim.confirm();
          macemcTimerRunning = false;
        }
      }
      #endif
      if(Sim::step == __Simulator__::loggingStartStep) {
        Log::enableLogging();
      }
      maceout << "Now on simulator step " << Sim::step << " (Rem, maxStep = " << Sim::maxStep << ")" << Log::endl;
      waiting = false;
      EventList readyEvents = getReadyEvents();
      macedbg(0) << "Ready Events--size=" << readyEvents.size();
      if(readyEvents.size()) {
        macedbg(0) << std::endl << readyEvents;
      }  
      macedbg(0) << Log::endl;
      if(readyEvents.size() > 0) {
        Event e = RandomUtil::random(readyEvents);
        try {
          simulateNode(e);
        } catch(const Exception& e) {
          maceerr << "Uncaught Mace Exception " << e << Log::endl;
          ASSERT(0);
        } catch(const std::exception& se) {
          maceerr << "Uncaught std::exception " << se.what() << Log::endl;
          ASSERT(0);
        } catch(...) {
          maceerr << "Uncaught unknown throw" << Log::endl;
          ASSERT(0);
        }
        __Simulator__::dumpState();
        Sim::updateGusto();
        if(__Simulator__::isDuplicateState()) {
          cause = Sim::DUPLICATE_STATE;
          break;
        }
        else if(stoppingCondition(Sim::isLive, Sim::isSafe, propertiesToTest)) {
          cause = Sim::STOPPING_CONDITION;
          break;
        }
      } else {
        stoppingCondition(Sim::isLive, Sim::isSafe, propertiesToTest);
        cause = Sim::NO_MORE_EVENTS;
        break;
      }
    } 
    #ifdef COMPARE_CMC
    if(__Simulator__::compareCMC && macemcTimerRunning) {
      __Simulator__::macemcTimeSim.confirm();
    }
    #endif

    Sim::pathComplete(cause, Sim::isLive, Sim::isSafe, Sim::step, __Simulator__::randomUtil->pathDepth(), __Simulator__::randomUtil);

    //this could certainly move to Sim
    if(cause == Sim::NO_MORE_EVENTS && !Sim::isLive) {
      __Simulator__::Error("NO_MORE_STEPS", ++num_dead_paths >= max_dead_paths);
    } else if( cause == Sim::TOO_MANY_STEPS && __Simulator__::max_steps_error && !Sim::isLive ) {
      __Simulator__::Error("MAX_STEPS", ++num_dead_paths >= max_dead_paths);
    }

    //     XXX: delete nodes
    //     for(int i = 0; i < num_nodes; i++) {
    //       delete nodes[i];
    //       nodes[i] = NULL;
    //     }

    for (size_t i = 0; i < servicesToDelete.size(); i++) {
      delete servicesToDelete[i];
    }

    SimScheduler::Instance().reset();
    SimNetwork::Instance().reset();
    SimApplication::reset();
    NumberGen::Reset();

    delete[] appNodes;

    for (size_t i = 0; i < propertiesToTest.size(); i++) {
      delete propertiesToTest[i];
    }

    delete[] mtime;
    mtime = 0;
    
  } while(Sim::getPathNum() < maxPaths && __Simulator__::randomUtil->hasNext(Sim::isLive));
  Log::enableLogging();
  Log::log("HALT") << "Simulation complete!" << Log::endl;
  monitorOverride = true;
  Sim::printStats(__Simulator__::randomUtil->getPhase() + " -- possibly incomplete");
  monitorOverride = false;
  Log::log("HALT") << "Final RandomUtil State: " << *__Simulator__::randomUtil << Log::endl;
  __Simulator__::randomUtil->dumpState();
  #ifdef COMPARE_CMC
  if(__Simulator__::compareCMC) {
    Log::log("HALT") << "macemcTime " << __Simulator__::macemcTime.getCumulativeTime() << Log::endl;
    Log::log("HALT") << "macemcTimeSim " << __Simulator__::macemcTimeSim.getCumulativeTime() << Log::endl;
    if(__Simulator__::runCMCmem) {
      Log::log("HALT") << "cmcTime " << __Simulator__::cmcTime.getCumulativeTime() << Log::endl;
    }
  }
  #endif

  printf("***falling out of main***\n");
  return 2;

}

EventList getReadyEvents() {
  ADD_SELECTORS("Sim::getReadyEvents");
  maceLog("getting ready nodes\n");
  EventList ev;
  Sim& net = SimNetwork::Instance();
  net.eventsWaiting(ev);
  Sim& sched = SimScheduler::Instance();
  sched.eventsWaiting(ev);
  Sim& app = SimApplication::Instance();
  app.eventsWaiting(ev);
  return ev;
}

void simulateNode(const Event& e) {
  ADD_FUNC_SELECTORS;
  static log_id_t bid = Log::getId("SimulateEventBegin");
  static log_id_t eid = Log::getId("SimulateEventEnd");
  //Step 1: Load the node's context (parameters & environment)
  Sim::setCurrentNode(e.node);

  maceLog("simulating node %d\n", e.node);

  const char* eventType; 

  switch(e.type) {
  case Event::APPLICATION:
    eventType = "APP_EVENT";
    break;
  case Event::NETWORK:
    eventType = "NET_EVENT";
    break;
  case Event::SCHEDULER:
    eventType = "SCHED_EVENT";
    break;
  default: ASSERT(0);
  }

  maceout << eventType << Log::endl;
  Log::log(bid) << Sim::step << " " << e.node << " " << eventType << Log::endl;

  std::string r;
  switch(e.type) {  
    case Event::APPLICATION:
      r = SimApplication::Instance().simulateEvent(e);
      break;
    case Event::NETWORK:
      r = SimNetwork::Instance().simulateEvent(e);
      break;
    case Event::SCHEDULER:
      r = SimScheduler::Instance().simulateEvent(e);
      break;
  }
  Log::log(eid) << Sim::step << " " << e.node << " " << eventType << " " << r << Log::endl;
}

namespace mace {
  MonotoneTime getmtime() {
    return mtime[Sim::getCurrentNode()]++;
  }
}
