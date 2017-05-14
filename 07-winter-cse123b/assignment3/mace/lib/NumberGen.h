/* 
 * NumberGen.h : part of the Mace toolkit for building distributed systems
 * 
 * Copyright (c) 2006, Charles Killian, Dejan Kostic, Ryan Braud, James W. Anderson, John Fisher-Ogden, Calvin Hubble, Duy Nguyen, Justin Burke, David Oppenheimer, Amin Vahdat, Adolfo Rodriguez, Sooraj Bhat
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in
 *      the documentation and/or other materials provided with the
 *      distribution.
 *    * Neither the names of Duke University nor The University of
 *      California, San Diego, nor the names of the authors or contributors
 *      may be used to endorse or promote products derived from
 *      this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * ----END-OF-LEGAL-STUFF---- */
#ifndef _NUMBER_GEN_H
#define _NUMBER_GEN_H

#include <map>
// #include "mstring.h"
#include "Log.h"
#include "LogIdSet.h"
#include "Params.h"
#include "mace-macros.h"

namespace mace {
  class string;
}

class NumberGen {
private:
  static const int DEFAULT_BASE = 0;
//   static const int DEFAULT_PORT_BASE = 5377;

  unsigned val;
  typedef std::map<mace::string, NumberGen*> NumberGenMap;
  static NumberGenMap instances;

  NumberGen(int base) {
    val = base;
  }

public:
  static const mace::string HANDLER_UID;
  static const mace::string PORT;

  static void Reset() {
    for(NumberGenMap::iterator i = instances.begin(); i != instances.end(); i++) {
      delete i->second;
    } 
    instances.clear();
  }

  static void Reset(const mace::string& index) {
    NumberGenMap::iterator i = instances.find(index);
    if (i != instances.end()) {
      delete i->second;
      instances.erase(i);
    }
  }
  
  static NumberGen* Instance(mace::string index) {
    ADD_FUNC_SELECTORS;
    if(Params::Instance()->containsKey("MACE_SIMULATE_NODES")) {
      index += Params::Instance()->getStr("SIM_CURRENT_NODE");
    }
    if(instances.find(index) == instances.end()) {
      int base = DEFAULT_BASE;
      if(Params::Instance()->containsKey(index)) {
        base = Params::Instance()->getInt(index);
        maceLog("Using params base for index %s %d\n", index.c_str(), base);
      }
//       else if(index == "port") {
//         Log::logf("NumberGen::Instance", "Using default port base for index port %d\n", DEFAULT_PORT_BASE);
//         //port is a special case
//         base = DEFAULT_PORT_BASE;
//       }
      else {
        maceLog("Using default base for index %s %d\n", index.c_str(), DEFAULT_BASE);
      }
      instances[index] = new NumberGen(base);
    }
    return instances[index];
  }
  unsigned GetVal() {
    return val++;
  }
};

#endif // _NUMBER_GEN_H
