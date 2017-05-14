#ifndef _SEARCH_RANDOM_UTIL_
#define _SEARCH_RANDOM_UTIL_

#include "mvector.h"
#include "SimRandomUtil.h"
#include "Printable.h"
#include "Params.h"
#include <sstream>
#include <fstream>

class SearchRandomUtil : public SimRandomUtil {
  protected:
    UIntList currentSelections;
    UIntList currentOutOf;

    unsigned depth;
    int depthIncrement;
    bool doneEarly; //True when every path searched at this depth so far finished before the depth.
    bool verbose;

    SearchRandomUtil(bool v = false) : currentSelections(), currentOutOf(), depth(0), depthIncrement(Params::Instance()->containsKey("SEARCH_DEPTH")?Params::Instance()->getInt("SEARCH_DEPTH"):5), verbose(v)
    {
      UIntList i;
      i.push_back(0);
      setNext(i);
    }


  public:
    unsigned randIntImplSub(unsigned max) {
      ADD_SELECTORS("SearchRandomUtil::randInt");
      ASSERT(max > 0);
      macedbg(0) << " depth=" << depth << " currentSelections.size()=" << currentSelections.size() << " currentOutOf.size()=" << currentOutOf.size() << Log::endl;
      ASSERT(depth != 0);
      unsigned offset = _pathInts.size();
      if(offset < depth) {
        if(offset >= currentOutOf.size()) {
          currentOutOf.push_back(max);
          if(offset >= currentSelections.size()) {
            currentSelections.push_back(0);
          }
        }
        macedbg(0) << "currentSelections[offset]=" << currentSelections[offset] << " currentOutOf[offset]="<<currentOutOf[offset] << " offset " << offset << Log::endl;
        ASSERT(currentSelections[offset] < max);
        if(max != currentOutOf[offset]) {
          maceerr << "max: " << max << " currentOutOf[ " << offset << " ]: " << currentOutOf[offset] << Log::endl;
          ASSERT(max == currentOutOf[offset]);
        }
        if(_pathInts.size() == depth - 1) { 
          doneEarly = false; 
        }
        return currentSelections[offset];
      }
      doneEarly = false;
      return max;
    }
    void setNext(const UIntList& selections) {
      currentSelections = selections;
      while(depth < currentSelections.size()) { depth += depthIncrement; }
      currentOutOf.resize(0);
    }
    bool hasNextSub(bool isLive) { 
      static unsigned maxSearchDepth = Params::Instance()->get("MAX_SEARCH_DEPTH", UINT_MAX);
      for(uint i = 0; i < depth && i < _pathInts.size(); i++) {
        if(currentSelections[i] + 1 < currentOutOf[i]) { return true; }
      }
      if(doneEarly) {
        return false;
      } else {
        return depth < maxSearchDepth;
      }
    }
    bool nextSub() { //advance to the next search sequence, returns true when the depth increases
      ADD_SELECTORS("SearchRandomUtil::next");
      bool ret = false;
      if(currentOutOf.size() != 0) { 
        int newDepth = std::min(depth, (unsigned int)_pathInts.size());
        for(; newDepth > 0; newDepth--) {
          currentSelections[newDepth-1]++;
          if(currentSelections[newDepth-1] < currentOutOf[newDepth-1]) { break; }
        }
        currentSelections.resize(newDepth);
        currentOutOf.resize(newDepth);
        bool ret = false;
        if(newDepth == 0) {
          ASSERT(!doneEarly /*failed assertion means search is done!*/);
          doneEarly = true;
          depth += depthIncrement;
          currentSelections.reserve(depth);
          currentOutOf.reserve(depth);
          maceout << "Searching up to depths of " << depth << ", " << (Sim::getPathNum()-1) << " paths searched so far." << Log::endl;
          phase = "SearchRandom:depth "+boost::lexical_cast<std::string>(depth);
          ret = true;
        }
      }
      resetGustoToggles();
      addGustoToggle(depth+_prefix.size());
      return ret;
    }
    void print(std::ostream& out) const {
      out << "Path<sdepth=" << depth << ">";
      SimRandomUtil::print(out);
      out << " search sequence " << currentSelections;
      if(verbose) {
        out << " Out Of " << currentOutOf;
      }
    }
    bool implementsPrintPath() { return true; }
    bool pathIsDone() { return _pathInts.size() == depth;}
    void dumpState() {}
    int testSearchDepth() { 
      if(_pathInts.size() >= currentOutOf.size()) {
        if(_randomInts.empty()) {
          return 0;
        } else {
          return 1;
        }
      } else if(pathDepth() < _prefix.size()) {
        return -2;
      } else {
        return -1;
      }
    }

    static SearchRandomUtil* SetInstance() { 
      ADD_SELECTORS("SearchRandomUtil::SetInstance");
      SearchRandomUtil* t = NULL; 
      if(_inst == NULL) { 
        _inst = t = new SearchRandomUtil(); 
        if(Params::Instance()->containsKey("RANDOM_UTIL_PREFIX_FILE")) {
          std::ifstream in(Params::Instance()->getStr("RANDOM_UTIL_PREFIX_FILE"));
          UIntList initial;
          uint a;
          in >> a;
          while(!in.eof()) {
            macedbg(1) << "Pushing back " << a << " onto initial." << Log::endl;
            initial.push_back(a);
            in >> a;
          }
          macedbg(0) << "initial.size: " << initial.size() << " initial: " << initial << Log::endl;
          t->setPrefix(initial);
        }
        //if(Params::Instance()->containsKey("RANDOM_UTIL_START_SEARCH")) {
        //  std::string s = Params::Instance()->getStr("RANDOM_UTIL_START_SEARCH");
        //  std::istringstream in(s);
        //  UIntList initial;
        //  uint a;
        //  in >> a;
        //  while(!in.eof()) {
        //    initial.push_back(a);
        //    in >> a;
        //  }
        //  t->setNext(initial);
        //}
      }
      return t;
    }
};

#endif 
