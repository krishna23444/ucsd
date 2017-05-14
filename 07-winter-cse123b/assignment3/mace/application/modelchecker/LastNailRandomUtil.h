#ifndef _LAST_NAIL_RANDOM_UTIL_
#define _LAST_NAIL_RANDOM_UTIL_

#include "mvector.h"
#include "SimRandomUtil.h"
#include "Printable.h"
#include "Params.h"
#include "boost/lexical_cast.hpp"

using boost::lexical_cast;

typedef mace::map<unsigned, UIntList> UIntListMap;
// typedef mace::vector<UIntList> IntListList;

class LastNailRandomUtil : public SimRandomUtil {
  protected:
    //These are set based on the path file input
    UIntList badPath;
    UIntList badPathOutOf;
    UIntListMap goodPaths;
    UIntListMap goodPathsOutOf;

    //These are for the current run
    unsigned prefixLength; //prefixLength is how many numbers to read from badPath before switching to random/extras.
    unsigned prefixLengthMod;
    unsigned pathCount; //tracks the number of paths tested at each prefixLength
    unsigned pathCountMax; //pronounce dead if pathCountMax paths are tested with no successes. 
    bool rampUp;

    bool verbose;
    bool printLivePaths;
    bool outputLivePaths;
    std::istream* infile;

    LastNailRandomUtil(bool v = false) : 
      prefixLength(Params::Instance()->get("LAST_NAIL_STARTING_PREFIX", 1)), 
      prefixLengthMod(1), 
      pathCount(0), 
      pathCountMax(Params::Instance()->get("NUM_RANDOM_PATHS", 20)), 
      rampUp(true), 
      verbose(v), 
      printLivePaths(Params::Instance()->get("LAST_NAIL_PRINT_PATHS", false)), 
      outputLivePaths(Params::Instance()->get("LAST_NAIL_OUTPUT_PATHS", true)), 
      infile((Params::Instance()->getStr("LAST_NAIL_REPLAY_FILE")==std::string("-")?&std::cin:new std::ifstream(Params::Instance()->getStr("LAST_NAIL_REPLAY_FILE"))))
    {
      ADD_FUNC_SELECTORS
      UIntList toggles;
      unsigned currentGusto = 0;
      do {
        (*infile) >> currentGusto; 
        maceout << "GustoDepth: " << currentGusto << Log::endl; 
        toggles.push_back(currentGusto);
        if(currentGusto == UINT_MAX) { break; }
      } while(true);
      setPrefix(toggles);
      unsigned input;
      (*infile) >> input;
      while(*infile) {
        badPathOutOf.push_back(input);
        (*infile) >> input;
        badPath.push_back(input);
        (*infile) >> input;
      }
      maceout << "Loaded a path of length: " << badPath.size() << Log::endl;
    }


  public:
    unsigned randIntImplSub(unsigned max) { 
      ADD_SELECTORS("LastNailRandomUtil::randInt");
      if(pathDepth() < prefixLength) {
        ASSERT(!DO_PREFIX_ASSERT || max == badPathOutOf[pathDepth()]);
        unsigned r = badPath[pathDepth()];
        maceout << "prefix: returning " << r << Log::endl;
        return r;
      }
      return max;
    }
    bool hasNextSub(bool isLive) { 
      ADD_FUNC_SELECTORS;
      //Note -- hasNext is called only at end of path
      if(isLive) {
        UIntList t;
        UIntList t2;
        for (unsigned i = 0; i < pathDepth(); i++) {
          t.push_back((i<prefixLength ? badPath[i] : _randomInts[i-prefixLength]));
          t2.push_back((i<prefixLength ? badPathOutOf[i] : _randomOutOf[i-prefixLength]));
        }
        goodPaths[prefixLength] = t;
        goodPathsOutOf[prefixLength] = t2;
        pathCount = pathCountMax; //done at this level
      }

      if(pathCount < pathCountMax) { return true; }
      pathCount = 0;

      //Adjust rampUp
      if(!isLive && rampUp) {
        rampUp = false;
        prefixLengthMod = prefixLengthMod >> 2;
      }

      //Adjust prefixLength
      if(isLive) {
        prefixLength += prefixLengthMod;
      }
      else {
        prefixLength -= prefixLengthMod;
      }

      bool ret = prefixLengthMod != 0;

      //Adjust prefixLengthMod
      if(rampUp) {
        prefixLengthMod = prefixLengthMod << 1;
      }
      else {
        prefixLengthMod = prefixLengthMod >> 1;
      }

      maceout << "New prefix length: " << prefixLength << " prefixLengthMod: " << prefixLengthMod << Log::endl;
      if (prefixLength + prefixLengthMod > badPath.size()) {
	maceerr << "last nail search inconclusive; try increasing max_num_steps" << Log::endl;
	return false;
      }
      return ret; //note, rampUp implies prefixLengthMod > 0
    }
    bool nextSub() { //advance to the next search sequence, returns true when the depth increases
      resetGustoToggles();
      addGustoToggle(prefixLength, true);

      pathCount++;
      if(pathCount == 1) {
        phase = "LastNail:prefix "+boost::lexical_cast<std::string>(prefixLength);
      }
      return pathCount == 1;
    }
    void print(std::ostream& out) const {
      out << "Path<prefixlen=" << prefixLength << ">";
      SimRandomUtil::print(out);
    }
    bool implementsPrintPath() { return true; }
    bool pathIsDone() { return pathDepth() > prefixLength; /* in theory, this would be used to skip by earlier live portions (e.g. starting frm a live point).  For now, we just make this be whether or not we are in the random portion of the search. */ }
    void dumpState() {
      ADD_FUNC_SELECTORS;
      unsigned lastLivePrefix = 0;
      resetGustoToggles();
      UIntList toggles = getGustoToggles();
      for(UIntListMap::const_iterator i = goodPaths.begin(), i2 = goodPathsOutOf.begin(); i != goodPaths.end() && i2 != goodPathsOutOf.end(); i++, i2++) {
        lastLivePrefix = i->first;
        if(outputLivePaths) {
          std::ofstream out(("live"+lexical_cast<std::string>(i->first)+".path").c_str());
          bool gustoOn = false;
          for(UIntList::const_iterator j = toggles.begin(); j != toggles.end() && i->first > *j; j++) {
            out << *j << std::endl;
            gustoOn = !gustoOn;
          }
          if(!gustoOn) {
            out << i->first << std::endl;
          }
          out << UINT_MAX << std::endl;
          for(UIntList::const_iterator j = i->second.begin(), j2 = i2->second.begin(); j != i->second.end() && j2 != i2->second.end(); j++, j2++) {
            out << *j2 << " " << *j << std::endl;
          }
          out.close();
        }
        if(printLivePaths) {
          maceout << "Live Path<gustoDepths=" << toggles << " prefixlen=" << i->first << " randomwalk=" << i->second.size() << "> path " << i->second << Log::endl;
        } else {
          maceout << "Live Path<gustoDepths=" << toggles << " prefixlen=" << i->first << " randomwalk=" << i->second.size() << ">" << Log::endl;
        }
      }
      if (prefixLength + prefixLengthMod < badPath.size()) {
	maceout << "Last live path has prefixlen " << lastLivePrefix << " so last nail was random number position " << (lastLivePrefix+1) << " taken" << Log::endl;
      }
    }

    static LastNailRandomUtil* SetInstance() { LastNailRandomUtil* t = NULL; if(_inst == NULL) { _inst = t = new LastNailRandomUtil(); } return t;}
};

#endif 
