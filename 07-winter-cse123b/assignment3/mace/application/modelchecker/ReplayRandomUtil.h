#ifndef _Replay_RANDOM_UTIL_
#define _Replay_RANDOM_UTIL_

#include <iostream>
#include <fstream>
#include "SimRandomUtil.h"
#include "lib/Params.h"

class ReplayRandomUtil : public SimRandomUtil {
  protected:
    static ReplayRandomUtil* realRandomUtil;
    bool interactive;
    std::istream* infile;

    //Used in interactive mode only.
    mace::vector<unsigned> simStepToPathStep;
    unsigned lastStepNum;
    bool runUntilLiveness;
    bool stopRunUntilLiveness;
    bool doPrompt;
    bool postRunGusto;

  public:
    unsigned randIntImplSub(unsigned foo) {
      ADD_SELECTORS("ReplayRandomUtil::randInt");
      if(runUntilLiveness && (stopRunUntilLiveness || Sim::isLive || Sim::step >= Sim::maxStep)) {
        runUntilLiveness = false;
        stopRunUntilLiveness = false;
        requestGustoSet(postRunGusto);
      }
      while(Sim::step > lastStepNum) {
        simStepToPathStep.push_back(pathDepth());
        lastStepNum++;
      }
      unsigned b;
      do {
        if(interactive && !runUntilLiveness && doPrompt) {
          if(foo <= 1) { return 0; }
          maceout << "Random number requested between 0 and " << foo << " (exclusive)." << Log::endl;
        }
        else {
          maceout << "max " << foo << " currentPos " << pathDepth() << Log::endl;
          if(foo <= 1) { 
            maceout << "returning deterministic 0" << Log::endl;
            return 0; 
          }
        }
        if(!runUntilLiveness) {
          bool skipB = false;
          if(!interactive || !doPrompt) {
            unsigned a;
            (*infile) >> a;
            ASSERT(!infile->eof());
            if(a >= 1000000) {
              b = a;
              skipB = true;
            } else {
              ASSERT(a == foo);
            }
          }
          ASSERT(!infile->eof());
          if(!skipB) {
            (*infile) >> b;
            ASSERT(!infile->eof());
          } else {
            skipB = false;
          }
        }
        if(b == 1000000 || runUntilLiveness) {
          //Dealer's Choice
          b = RandomUtil::randIntImpl(foo);
        }
        if(b == 1000001) {
          //Toggle Gusto
          bool req = requestGustoToggle();
          maceout << "Set gusto to: " << req << " (takes effect next step)" << Log::endl;
        } else if(b == 1000002) {
          //Save sequence to file <f>
          std::string filename;
          std::cin >> filename;
          std::ofstream out(filename.c_str());
          SimRandomUtil::printPath(out);
	  out.close();
          maceout << "Saved sequence as: " << filename << Log::endl;
        } else if(b == 1000003) {
          //Run until isLive
          runUntilLiveness = true;
          stopRunUntilLiveness = false;
          postRunGusto = requestGustoSet(true);
        } else if(b == 1000004) {
          //Toggle prompt
          doPrompt = !doPrompt;
          maceout << "Set doPrompt to: " << doPrompt << Log::endl;
        } else if(b == 1000005) {
          //Save sequence up to step <s> to file <f>
          unsigned stepNum;
          std::cin >> stepNum;
          std::string filename;
          std::cin >> filename;
          std::ofstream out(filename.c_str());
          printPath(out, stepNum);
          out.close();
        } else if(b == 1000006) {
          exit(0);
        } else {
          //b is kosher.
        }
      } while(b >= 1000000);
      ASSERT(b < foo);
      return b;
    }
    //Note: inherit randIntImpl(...) from the base class.  It will call our impl by inheritance.

    bool handleInt() {
      if(runUntilLiveness) {
        stopRunUntilLiveness = true;
        return true;
      }
      else {
        return false;
      }
    }

    static ReplayRandomUtil* SetInstance() { 
      ReplayRandomUtil* realRandomUtil = NULL;
      if(_inst == NULL) { _inst = realRandomUtil = new ReplayRandomUtil(); } 
      return realRandomUtil; 
    }
    ReplayRandomUtil() : 
      interactive((Params::Instance()->getStr("RANDOM_REPLAY_FILE")==std::string("-")?true:false)),infile((interactive ? &std::cin : new std::ifstream(Params::Instance()->getStr("RANDOM_REPLAY_FILE")))), 
      lastStepNum(0), runUntilLiveness(false), doPrompt(false), postRunGusto(false)
    { 
      ADD_FUNC_SELECTORS;
      maceout << "Using REPLAY_RANDOM_FILE \"" << Params::Instance()->getStr("RANDOM_REPLAY_FILE") << "\"" << " and therefore " << (interactive ? "std::cin" : "new ifstream") << Log::endl;
      unsigned currentGusto = 0;
      if(interactive) {
        simStepToPathStep.push_back(0);
      }
      UIntList toggles;
      do {
        maceout << "Please provide gusto toggle depth (" << UINT_MAX << " when no more toggles)" << Log::endl; 
        (*infile) >> currentGusto; 
        maceout << "Gusto Toggle at: " << currentGusto << Log::endl;
        toggles.push_back(currentGusto);
        if(currentGusto == UINT_MAX) { break; }
      } while(true);
      setPrefix(toggles);
    }
    virtual ~ReplayRandomUtil() {}
    bool hasNextSub(bool isLive) { return false; }
    bool nextSub() { return false; }
    bool pathIsDone() { return false; }
    void dumpState() {
      if(interactive) {
        ADD_SELECTORS("ReplayRandomUtil::dumpState");
        std::ofstream out("final-sequence.path");
        maceout << "Saving sequence as: final-sequence.path" << Log::endl;
        SimRandomUtil::printPath(out);
        out.close();
      }
    }
    void print(std::ostream& out) const { }

    bool implementsPrintPath() { return interactive; }
    void printPath(std::ostream& out, unsigned stepNum) const {
      if(stepNum > simStepToPathStep.size()) { 
        SimRandomUtil::printPath(out);
        return;
      }
      UIntList toggles = getGustoToggles();
      for(UIntList::const_iterator i = toggles.begin(); i != toggles.end() && *i <= simStepToPathStep[stepNum]; i++) {
        out << *i << std::endl;
      }
      //if(toggleCount % 2 != 0) {
      //  out << _pathInts.size() << std::endl; //Print out the gusto depth
      //}
      out << UINT_MAX << std::endl;
      unsigned position = 0;
      for(UIntList::const_iterator i = _pathInts.begin(), j = _outOf.begin(); i != _pathInts.end() && j != _outOf.end() && position < simStepToPathStep[stepNum]; i++, j++, position++) {
        out << *j << " " << *i << std::endl;
      }
    }
};

#endif 
