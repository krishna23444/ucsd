#include "Util.h"
#include "MaceTime.h"

#ifndef app_getmtime_h
#define app_getmtime_h

//Include this file only in applications which are defining getmtime.
//Elsewhere this function is declared by lib/MaceTime.h, so it can be defined
//separately by modelchecker.cc

namespace mace {

  MonotoneTime getmtime() {
    return Util::timeu();
  }

}

#endif

