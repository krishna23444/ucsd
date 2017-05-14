#ifndef SIM_RANDOM_UTIL_H
#define SIM_RANDOM_UTIL_H

#include "RandomUtil.h"
#include "Printable.h"
#include "mvector.h"

//#define FAKE_GUSTO
#define DO_PREFIX_ASSERT true

typedef mace::vector<unsigned> UIntList;

class SimRandomUtil : public RandomUtil, public mace::PrintPrintable {
  public:
    static const unsigned MAX_RAND_INT = 1000000;
  protected:
    std::string phase;
  private: 
    unsigned _currentPos;
    bool localToggleRequest;
  protected:
    UIntList safeGustoDepthToggles;
    UIntList _prefixGustoDepthToggles;
    UIntList::iterator _currentPrefixGustoToggle;
    UIntList _pathInts;
    UIntList _outOf;
    UIntList _prefix;
    UIntList _prefixOutOf;
    UIntList _randomInts;
    UIntList _randomOutOf;

  protected:
    //returns value requested
    bool requestGustoToggle() {
      localToggleRequest = !localToggleRequest;

      return Sim::getGustoReq() ^ localToggleRequest; //Have not toggled yet, so this should invert the current Req.
    }

    bool requestGustoSet(bool req) {
      if(_currentPrefixGustoToggle != _prefixGustoDepthToggles.end()) {
        _prefixGustoDepthToggles.erase(_currentPrefixGustoToggle, _prefixGustoDepthToggles.end());
        _currentPrefixGustoToggle = _prefixGustoDepthToggles.end();
      }

      if(Sim::getGustoReq() != req) {
        _prefixGustoDepthToggles.push_back(_currentPos+1); //Gusto will happen at the end of randIntImplSub, so we need a +1.
        _currentPrefixGustoToggle = _prefixGustoDepthToggles.end()-1;
      }
      return Sim::getGustoReq() ^ localToggleRequest;
    }
    
  public:
    bool hasNext(bool isLive) {
      ADD_SELECTORS("SimRandomUtil::hasNext");
      bool r = hasNextSub(isLive);
      static int mask = (Params::Instance()->containsKey("search_print_mask")?Params::Instance()->getInt("search_print_mask"):0x00000fff);
      if((Sim::getPathNum() & mask) == 0) {
        maceout << "More Paths: " << r << ", Just finished path " << Sim::getPathNum() << ", " << *this << Log::endl;
      }
      return r;
    }
    virtual bool hasNextSub(bool isLive) = 0;
    virtual bool handleInt() {
      return false;
    }
    const std::string& getPhase() {
      return phase;
    }
    virtual bool next() {
      ADD_SELECTORS("SimRandomUtil::next");
      static int mask = (Params::Instance()->containsKey("search_print_mask")?Params::Instance()->getInt("search_print_mask"):0x00000fff);
      localToggleRequest = false;

      bool b = nextSub();

      if((Sim::getPathNum() & mask) == 0) {
        maceout << "Now searching path " << Sim::getPathNum() << ", " << *this << Log::endl;
      }
      _currentPrefixGustoToggle = _prefixGustoDepthToggles.begin();
      _currentPos = 0;
      _pathInts.clear();
      _outOf.clear();
      _randomInts.clear();
      _randomOutOf.clear();
      return b;
    }
    virtual bool nextSub() = 0; //returns true on a phase change -- to print stats.
    virtual bool implementsPrintPath() = 0;
    virtual void print(std::ostream& out) const {
      out << "SimRandUtil<" << "prefixlen="<<_prefix.size()<<",position="<<_currentPos<<",searchlen="<<_pathInts.size()<<",walklen="<<_randomInts.size()<<">";
    }
    void printPath(std::ostream& out) const {
      for(UIntList::const_iterator i = _prefixGustoDepthToggles.begin();
          i != _prefixGustoDepthToggles.end() && *i <= _currentPos; i++) {
        out << *i << std::endl;
      }
      //if(_prefixGustoDepthToggles.size() % 2 != 0) {
      //  out << _prefix.size() << std::endl;
      //}
      //if(!_pathInts.empty()) {
      //  out << (_pathInts.size() + _prefix.size()) << std::endl; //Print out the gusto depth
      //}
      out << UINT_MAX << std::endl;
      if(_prefix.size() != _prefixOutOf.size()) {
        Log::err() << "ERROR: size mismatch on prefix and prefixOutOf (SimRandomUtil.h) prefix " << _prefix << " prefixOutOf " << _prefixOutOf << Log::endl;
      }
      for(UIntList::const_iterator i = _prefix.begin(), j = _prefixOutOf.begin(); i != _prefix.end() && j != _prefix.end(); i++, j++) {
        out << *j << " " << *i << std::endl;
      }

      if(_pathInts.size() != _outOf.size()) {
        Log::err() << "ERROR: size mismatch on pathInts and outOf (SimRandomUtil.h) pathInts " << _pathInts << " outOf " << _outOf << Log::endl;
      }
      for(UIntList::const_iterator i = _pathInts.begin(), j = _outOf.begin(); i != _pathInts.end() && j != _outOf.end(); i++, j++) {
        out << *j << " " << *i << std::endl;
      }

      if(_randomInts.size() != _randomOutOf.size()) {
        Log::err() << "ERROR: size mismatch on randomInts and randomOutOf (SimRandomUtil.h) randomInts " << _randomInts << " randomOutOf " << _randomOutOf << Log::endl;
      }
      for(UIntList::const_iterator i = _randomInts.begin(), j = _randomOutOf.begin(); i != _randomInts.end() && j != _randomOutOf.end(); i++, j++) {
        out << *j << " " << *i << std::endl;
      }
    }
    virtual bool pathIsDone() = 0;
    virtual void dumpState() = 0;
    virtual int testSearchDepth() { return 1; } 
    virtual UIntList getPath() const { return _pathInts; }
    unsigned randIntImpl(unsigned max) {
      ADD_SELECTORS("SimRandomUtil::randInt");
      ASSERT(max > 0 && max < MAX_RAND_INT);
      maceout << "max " << max << " currentPos " << _currentPos << Log::endl;
      macedbg(0) << " _pathInts.size()=" << _pathInts.size() << " _outOf.size()=" << _outOf.size() << Log::endl;
      unsigned b = 0;
      const char* method = "(empty)";
      if(max <= 1) { 
        maceout << "returning deterministic 0" << Log::endl;
        return 0; 
      }
      else if(!_prefix.empty() && _currentPos < _prefix.size()) {
        ASSERT(!DO_PREFIX_ASSERT || max == _prefixOutOf[_currentPos]);
        method = "prefix";
        b = _prefix[_currentPos];
      }
      else {
        b = randIntImplSub(max);
        if(b != max) {
          method = "search";
          _pathInts.push_back(b);
          _outOf.push_back(max);
        } else {
          method = "random";
          b = RandomUtil::randIntImpl(max);
          _randomInts.push_back(b);
          _randomOutOf.push_back(max);
        }
      }
      maceout << "returning " << b << " currentPos " << _currentPos << " max " << max << " method " << method << Log::endl;
      _currentPos++;
      if(localToggleRequest) {
        if(_currentPrefixGustoToggle != _prefixGustoDepthToggles.end()) {
          _prefixGustoDepthToggles.erase(_currentPrefixGustoToggle, _prefixGustoDepthToggles.end());
        }
        _prefixGustoDepthToggles.push_back(_currentPos); //Gusto will happen at the end of randIntImplSub, so we need a +1.
        _currentPrefixGustoToggle = _prefixGustoDepthToggles.end()-1;
        localToggleRequest = false;
      }
      if(_currentPrefixGustoToggle != _prefixGustoDepthToggles.end() && _currentPos == *_currentPrefixGustoToggle) {
        Sim::reqGustoToggle();
        _currentPrefixGustoToggle++;
      }
      return b;
    }
    virtual unsigned randIntImplSub(unsigned max) = 0;
    unsigned pathDepth() const { return _currentPos; }
    unsigned randIntImpl(const std::vector<unsigned>& weights) {
      if (Sim::getGusto()) {
#ifndef FAKE_GUSTO
        return RandomUtil::randIntImpl(weights);
#else
        mace::vector<unsigned> w2;
        for(unsigned i = 0 ; i < weights.size(); i++) {
          //           printf("weights[%u]=%u\n", i, weights[i]);
          if(weights[i] != 0) {
            w2.push_back(i);
          }
        }
        ASSERT(!w2.empty());
        return *(w2.random());
#endif
      }
      else {
	return randIntImpl(weights.size());
      }
    }
    unsigned randIntImpl() { 
      ASSERT(false);
      return randIntImpl(UINT_MAX); 
    }
    unsigned randIntImpl(unsigned max, unsigned first, va_list ap) {
      if (Sim::getGusto()) {
#ifndef FAKE_GUSTO
        unsigned r = RandomUtil::randIntImpl(max, first, ap);
	return r;
#else
        va_list copy;
        va_copy(copy, ap);

        unsigned newMax = first;
        for (size_t i = 0; i < max - 1; i++) {
          unsigned t = va_arg(ap, unsigned);
          newMax += (t != 0);
        }

        unsigned rand = randIntImpl(newMax);
        unsigned r = 0;
        while (r < max - 1 && rand >= first) {
          r++;
          first += (va_arg(copy, unsigned) != 0);
        }
        va_end(copy);

        return r;
#endif
      }
      else {
	return randIntImpl(max);
      }
    }

    void setPrefix(const UIntList& selections) {
      static bool setNoGusto = Param::get("START_SEARCH_WITHOUT_GUSTO",true);
      ADD_SELECTORS("SimRandomUtil::setPrefix");
      maceout << "Input: selections size: " << selections.size() << Log::endl;

      UIntList::const_iterator i = selections.begin();
  
      unsigned maxToggle = UINT_MAX;

      //Find UINT_MAX
      while(i != selections.end() && *i != UINT_MAX) {
        maceout << "maxToggle: " << maxToggle << " i: " << *i << Log::endl;
        ASSERT(maxToggle == UINT_MAX || *i > maxToggle);
        maxToggle = *i;
        macedbg(0) << "Toggle at depth " << *i << Log::endl;
        i++;
      }

      //Must find!
      ASSERT(i != selections.end());

      //Assign prefix values
      _prefix.clear();
      for(UIntList::const_iterator j = i+1; j != selections.end(); j++) {
        _prefixOutOf.push_back(*j);
        j++;
        ASSERT(j != selections.end());
        _prefix.push_back(*j);
      }

      //Two cases:
      //  1. toggles end before _prefix.size(), if so, make sure gusto off.
      //  2. toggles end after _prefix.size(), in which case do not mess with.

      _prefixGustoDepthToggles.assign(selections.begin(), i);
      if(maxToggle < _prefix.size() && setNoGusto) {
        if(_prefixGustoDepthToggles.size() % 2 != 0) {
          _prefixGustoDepthToggles.push_back(_prefix.size());
        }
      }
      safeGustoDepthToggles = _prefixGustoDepthToggles;

      _currentPrefixGustoToggle = _prefixGustoDepthToggles.begin();
      maceout << "toggle size: " << _prefixGustoDepthToggles.size() << " prefix size: " << _prefix.size() << Log::endl;
    }

    UIntList getGustoToggles() const {
      return _prefixGustoDepthToggles;
    }

    void resetGustoToggles() {
      _prefixGustoDepthToggles = safeGustoDepthToggles;
    }

    void addGustoToggle(unsigned at) {
      _prefixGustoDepthToggles.push_back(at);
    }

    void addGustoToggle(unsigned at, bool setTo) {
      bool gustoOn = false;
      UIntList::iterator i;

      //Find place to insert
      for(i = _prefixGustoDepthToggles.begin(); i != _prefixGustoDepthToggles.end() && *i < at; i++, gustoOn = !gustoOn) { }

      if(i != _prefixGustoDepthToggles.end()) {
        _prefixGustoDepthToggles.erase(i, _prefixGustoDepthToggles.end());
      }

      if(gustoOn != setTo) {
        _prefixGustoDepthToggles.push_back(at);
      }
    }
};

#endif //SIM_RANDOM_UTIL_H
