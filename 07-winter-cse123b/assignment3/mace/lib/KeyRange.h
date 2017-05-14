/* 
 * KeyRange.h : part of the Mace toolkit for building distributed systems
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
#ifndef KEY_RANGE_H
#define KEY_RANGE_H

#include "mpair.h"
#include "MaceKey.h"

namespace mace {
class KeyRange : public mace::pair<MaceKey, MaceKey> {
public:
  KeyRange() : mace::pair<MaceKey, MaceKey>() { }
  KeyRange(const KeyRange& copy) : mace::pair<MaceKey, MaceKey>(copy) { }
  KeyRange(MaceKey f, MaceKey s) : mace::pair<MaceKey, MaceKey>(f, s) { }
  bool containsKey(const MaceKey& k) const {
    return KeyRange::containsKey(k, first, second);
  } // containsKey

  inline static bool containsKey(const MaceKey& id, const MaceKey& start, const MaceKey& end) {
    if (id.isNullAddress() || start.isNullAddress() || end.isNullAddress()) {
      return false;
    }
    if (id.addressFamily() != start.addressFamily() || start.addressFamily() != end.addressFamily()) {
      throw(InvalidMaceKeyException("Cannot perform KeyRange::containsKey on keys of different families"));
    }

    if (start == end) {
      return true;
    }
    else if (start < end) {
      return ((start <= id) && (id < end));
    }
    else {
      return ((start <= id) || (id < end));
    }
  }

  void print(std::ostream& out) const {
    out << "KeyRange["<<first<<","<<second<<")";
  }

  void printState(std::ostream& out) const {
    out << "KeyRange[";
    mace::printState(out, &first, first);
    out << ",";
    mace::printState(out, &second, second);
    out <<")";
  }

  static const KeyRange null;

}; // KeyRange

} // namespace mace

#endif // KEY_RANGE_H
