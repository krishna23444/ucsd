/* 
 * MaceTime.h : part of the Mace toolkit for building distributed systems
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
#ifndef _MACE_TIME_H
#define _MACE_TIME_H

#include <inttypes.h>

#include "Util.h"
#include "Printable.h"
#include "Serializable.h"
#include "hash_string.h"
#include "Traits.h"

class Sim;

namespace mace {
  typedef uint64_t MonotoneTime;

  // Defined by modelchecker.cc or app-getmtime.h (include in application)
  MonotoneTime getmtime() __attribute__ ((used, noinline)) ;

  class MaceTime : public PrintPrintable, public Serializable {
    friend class ::Sim;
    friend class mace::KeyTraits<MaceTime>;

  public:
    MaceTime() : realtime(0), tainted(true) {
    }

    MaceTime(uint64_t offset) : realtime(offset), tainted(true) {
    }

    MaceTime(const MaceTime& other) : realtime(other.realtime), tainted(other.tainted) {
    }

    MaceTime& operator=(const MaceTime& other) {
      realtime = other.realtime;
      tainted = other.tainted;
      return *this;
    }

    MaceTime& operator=(uint64_t offset) {
      realtime = offset;
      tainted = true;
      return *this;
    }

    MaceTime operator+(const MaceTime& other) const {
      return plus(other);
    }

    MaceTime operator-(const MaceTime& other) const {
      return minus(other);
    }

    bool operator==(const MaceTime& other) const {
      return realtime == other.realtime;
    }

    bool operator==(const uint64_t& other) const {
      return realtime == other;
    }

    bool operator!=(const MaceTime& other) const {
      return realtime != other.realtime;
    }

    bool operator!=(const uint64_t& other) const {
      return realtime != other;
    }

//     operator bool() const {
//       return realtime != 0;
//     }

    bool lessThan(const MaceTime& other, uint trueWeight = 1, uint falseWeight = 1) const {
      if (simulated && (tainted || other.tainted)) {
	return Util::randInt(2, falseWeight, trueWeight);
      }
      else {
	return realtime < other.realtime;
      }
    }

    bool greaterThan(const MaceTime& other,
		     uint trueWeight = 1, uint falseWeight = 1) const {
      return !lessThanOrEqual(other, falseWeight, trueWeight);
    }

    bool lessThanOrEqual(const MaceTime& other,
			 uint trueWeight = 1,  uint falseWeight = 1) const {
      if (simulated && (tainted || other.tainted)) {
	return Util::randInt(2, falseWeight, trueWeight);
      }
      else {
	return realtime <= other.realtime;
      }
    }

    bool greaterThanOrEqual(const MaceTime& other,
			    uint trueWeight = 1,  uint falseWeight = 1) const {
      return !lessThan(other, falseWeight, trueWeight);
    }
  
    bool equals(const MaceTime& other, uint trueWeight = 1, uint falseWeight = 1) const {
      if (simulated && (tainted || other.tainted)) {
	return Util::randInt(2, falseWeight, trueWeight);
      }
      else {
	return realtime == other.realtime;
      }
    }

    bool notEquals(const MaceTime& other,
		   uint trueWeight = 1, uint falseWeight = 1) const {
      return !equals(other, falseWeight, trueWeight);
    }

    MaceTime plus(const MaceTime& other) const {
      MaceTime t(*this);
      t.realtime += other.realtime;
      t.tainted |= other.tainted;
      return t;
    }

    MaceTime minus(const MaceTime& other) const {
      MaceTime t(*this);
      t.realtime -= other.realtime;
      t.tainted |= other.tainted;
      return t;
    }

    MaceTime scaleBy(uint amount) const {
      MaceTime t(*this);
      t.realtime *= amount;
      // clear tainted if amount == 0 ?
      return t;
    }

    MaceTime scaleBy(double amount) const {
      MaceTime t(*this);
      t.realtime = (uint64_t)((double)t.realtime * amount);
      // clear tainted if amount == 0 ?
      return t;
    }

    void serialize(std::string& s) const {
      mace::serialize(s, &realtime, realtime);
    }

    int deserialize(istream& in) throw(SerializationException) {
      int off = 0;
      off += mace::deserialize(in, &realtime, realtime);
      tainted = true;
      return off;
    }

    void print(std::ostream& out) const {
      out << realtime;
    }

    void printState(std::ostream& out) const {
      if(simulated && tainted) {
        out << "NON-DET";
      } else {
        out << realtime;
      }
    }

    size_t hashOf() const {
      static const hash_bytes<uint64_t> h = hash_bytes<uint64_t>();
      return h(realtime);
    }

    uint64_t time() const {
      return realtime;
    }

    bool isZero() const {
      return realtime == 0;
    }

    static const MaceTime& min(const MaceTime& l, const MaceTime& r) {
      if (l.realtime < r.realtime) {
	return l;
      }
      return r;
    }

    static MaceTime currentTime() {
      MaceTime t(Util::timeu());
      t.tainted = true;
      return t;
    }


//     static uint64_t timeu() {
// //       if (simulated) {
// // 	timeofday += 1000;
// // 	return timeofday;
// //       }
//       return Util::timeu();
//     }

  private:
    uint64_t realtime;
    bool tainted;
    static bool simulated;
//     static uint64_t timeofday;
  }; // MaceTime

  class MaceTimeComparitor {
  public:
    bool operator()(const MaceTime& l, const MaceTime& r) const {
      return l.time() < r.time();
    }
  };

  class MaceTimeEquality {
  public:
    bool operator()(const MaceTime& l, const MaceTime& r) const {
      return l.time() == r.time();
    }
  };

  class MaceTimeHash {
  public:
    size_t operator()(const MaceTime& x) const {
      return x.hashOf();
    }
  };

  template<>
  class KeyTraits<MaceTime> : public KeyTraits<unsigned long long> {
    public:
    static void append(std::string& str, const MaceTime& key) {
      char buf[30];

      sprintf(buf, "%" PRIu64, key.time());
      str += buf;
    }
    static bool isDeterministic() {
      return !MaceTime::simulated;
    }
    static MaceTime extract(std::istream& in) 
    throw(SerializationException) {
      std::string token = SerializationUtil::get(in, '<');
      return strtoull(token.c_str(), NULL, 10);
    }
  };

}; // mace

namespace __gnu_cxx {
  template<> struct hash<mace::MaceTime> {
    size_t operator()(const mace::MaceTime& x) const {
      return x.hashOf();
    }
  };
}

#endif // _MACE_TIME_H
