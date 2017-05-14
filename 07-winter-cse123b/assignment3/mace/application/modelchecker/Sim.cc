#include "Sim.h"

int Sim::runningNode = 0;
bool Sim::gusto = false;
bool Sim::useGusto = false;
bool Sim::gustoReq = false;
bool Sim::isLive = false;
bool Sim::isSafe = true;
unsigned Sim::step = 0;
unsigned Sim::maxStep = 0;
int Sim::numNodes = 0;
std::string** Sim::logPrefixes = NULL;
MaceKey** Sim::keys = NULL;
std::string* Sim::nodeString = NULL;
bool Sim::inited = false;
unsigned Sim::phaseNum = 0;
unsigned Sim::pathCount = 0;
unsigned Sim::cumulativePathCount = 0;
unsigned Sim::uniqueStateCount = 0;
unsigned Sim::cumulativeUniqueStateCount = 0;
unsigned Sim::countTMS = 0;
unsigned Sim::cumulativeTmsCount = 0;
unsigned Sim::countOOE = 0;
unsigned Sim::cumulativeOoeCount = 0;
unsigned Sim::countSC = 0;
unsigned Sim::cumulativeScCount = 0;
unsigned Sim::countDuplicate = 0;
unsigned Sim::cumulativeDuplicateCount = 0;
unsigned Sim::liveCount = 0;
unsigned Sim::cumulativeLiveCount = 0;
mace::deque<unsigned> Sim::simStepsDuplicate;
mace::deque<unsigned> Sim::randStepsDuplicate;
mace::deque<unsigned> Sim::simStepsTMS;
mace::deque<unsigned> Sim::randStepsTMS;
mace::deque<unsigned> Sim::simStepsOOE;
mace::deque<unsigned> Sim::randStepsOOE;
mace::deque<unsigned> Sim::simStepsSC;
mace::deque<unsigned> Sim::randStepsSC;
uint64_t Sim::startTime;
uint64_t Sim::lastPhaseEnd;
