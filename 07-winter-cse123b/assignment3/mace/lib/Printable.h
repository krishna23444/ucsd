/* 
 * Printable.h : part of the Mace toolkit for building distributed systems
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
#ifndef __PRINTABLE_H
#define __PRINTABLE_H

#include <string>
#include <sstream>
#include <iomanip>
#include <ctype.h>
#include <boost/shared_ptr.hpp>
#include <map>

#include "hash_string.h"
#include "Base64.h"

namespace mace {

class Printable {
  public:
    virtual std::string toString() const = 0;
    virtual std::string toStateString() const = 0;
    virtual void print(std::ostream& printer) const = 0;
    virtual void printState(std::ostream& printer) const = 0;
    virtual ~Printable() {}
};

class PrintPrintable : virtual public Printable {
  public:
    std::string toString() const {
      std::ostringstream out;
      print(out);
      return out.str();
    }
    std::string toStateString() const {
      std::ostringstream out;
      printState(out);
      return out.str();
    }
    virtual void printState(std::ostream& printer) const {
      print(printer);
    }
    virtual ~PrintPrintable() {}
};

class ToStringPrintable : virtual public Printable {
  public:
    void print(std::ostream& printer) const {
      printer << toString();
    }
    void printState(std::ostream& printer) const {
      printer << toStateString();
    }
    virtual std::string toStateString() const {
      return toString();
    }
    virtual ~ToStringPrintable() {}
};

template<typename S>
const int noConvertToString(const S* dud) { return 0; }

template<typename S> 
void printItem(std::ostream& out, const void* pitem, const S& item) {
  //   out << "[NON-PRINTABLE: address=" << std::hex << (int)pitem << " size=" << sizeof(S) << /*" typeid=" << typeid(S) <<  " hex=" << toHex(std::string((const char*)pitem, sizeof(S))) << */ "]";
  std::ios::fmtflags f(out.flags());
  out << std::hex;
  out << "[NON-PRINTABLE: ptr=" << pitem << " func=" << __PRETTY_FUNCTION__ << "]";
  out.flags(f);
}

template<typename S> 
void printItem(std::ostream& out, const bool* pitem, const S& item) {
  out << (item?"true":"false");
}

template<typename S> 
void printItem(std::ostream& out, const double* pitem, const S& item) {
  out << item;
}

template<typename S> 
void printItem(std::ostream& out, const int* pitem, const S& item) {
  out << item;
}

template<typename S> 
void printItem(std::ostream& out, const uint* pitem, const S& item) {
  out << item;
}

template<typename S> 
void printItem(std::ostream& out, const char* pitem, const S& item) {
  out << (int)item;
  if(isprint(item)) {
    out << " '" << item << "'";
  }
}

template<typename S> 
void printItem(std::ostream& out, const uint8_t* pitem, const S& item) {
  out << item;
}

template<typename S> 
void printItem(std::ostream& out, const long int* pitem, const S& item) {
  out << item;
}

template<typename S> 
void printItem(std::ostream& out, const short* pitem, const S& item) {
  out << item;
}

template<typename S> 
void printItem(std::ostream& out, const unsigned short* pitem, const S& item) {
  out << item;
}

template<typename S>
void printItem(std::ostream& out, const unsigned long long* pitem, const S& item) {
  out << item;
}

template<typename S> 
void printItem(std::ostream& out, const long long* pitem, const S& item) {
  out << item;
}

template<typename S> 
void printItem(std::ostream& out, const std::string* pitem, const S& item) {
  if (Base64::isPrintable(item)) {
    out << item;
  } else {
    static const hash_string hasher = hash_string();
    //if(longStrings) {
    //     out << Log::toHex(item);
    //} else {
    out << hasher(item);
    //}
  }
}

template<typename S> 
void printItem(std::ostream& out, const char** pitem, const S& item) {
  out << item;
}

template<typename S> 
void printItem(std::ostream& out, const Printable* pitem, const S& item) {
  item.print(out);
}

template<typename S> 
void printItem(std::ostream& out, const Printable** pitem, const S& item) {
  // warning --- if pitem is an array, then only the first element will get printed
  item->print(out);
}

template<typename S> 
void printItem(std::ostream& out, const boost::shared_ptr<S>* pitem, const boost::shared_ptr<S>& item) {
  out << "shared_ptr(";
  mace::printItem(out, item.get(), *item);
  out << ")";
}

template<typename S, typename T>
void printItem(std::ostream& out, const typename std::map<S, T>::const_iterator* i, const typename std::map<S,T>::const_iterator& item) {
  mace::printItem(out, (*i)->first, *((*i)->first));
  out << "->";
  mace::printItem(out, (*i)->second, *((*i)->second));
}

template<typename S>
std::string mapToString(const S& m, bool newlines = false) {
  std::ostringstream os;
  for (typename S::const_iterator i = m.begin(); i != m.end(); i++) {
    os << "( " << convertToString(&(i->first), i->first) << " -> " << convertToString(&(i->second), i->second) << " )";
    if (newlines) {
      os << std::endl;
    }
    else {
      os << " ";
    }
  }
  return os.str();
}

template<typename S> 
void printMap(std::ostream& os, const S& b, const S& e, bool newlines = false) {
  os << "[";
  if(newlines) { os << std::endl; } else { os << " "; }
  for (S i = b; i != e; i++) {
    os << "( ";
    printItem(os, &(i->first), i->first);
    os << " -> ";
    printItem(os, &(i->second), i->second);
    os << " )";
    if (newlines) {
      os << std::endl;
    }
    else {
      os << " ";
    }
  }
  os << "]";
  if (newlines) {
    os << std::endl;
  }
}

template<typename S> 
void printMapState(std::ostream& os, const S& b, const S& e, bool newlines = false) {
  os << "[";
  if(newlines) { os << std::endl; } else { os << " "; }
  for (S i = b; i != e; i++) {
    os << "( ";
    printState(os, &(i->first), i->first);
    os << " -> ";
    printState(os, &(i->second), i->second);
    os << " )";
    if (newlines) {
      os << std::endl;
    }
    else {
      os << " ";
    }
  }
  os << "]";
  if (newlines) {
    os << std::endl;
  }
}

template<typename S> 
std::string listToString(const S& l, bool newlines = false) {
  std::ostringstream os;
  for (typename S::const_iterator i = l.begin(); i != l.end(); i++) {
    os << convertToString(&(*i), *i);
    if (newlines) {
      os << std::endl;
    }
    else {
      os << " ";
    }
  }
  return os.str();
} // listToString

template<typename S> 
std::string listToString(const S& b, const S& e, bool newlines = false) {
  std::ostringstream os;
  for (S i = b; i != e; i++) {
    os << convertToString(&(*i), *i);
    if (newlines) {
      os << std::endl;
    }
    else {
      os << " ";
    }
  }
  return os.str();
} // listToString

template<typename S> 
void printList(std::ostream& os, const S& b, const S& e, bool newlines = false) {
  os << "[";
  if(newlines) { os << std::endl; } else { os << " "; }
  for (S i = b; i != e; i++) {
    printItem(os, &(*i), *i);
    if (newlines) {
      os << std::endl;
    }
    else {
      os << " ";
    }
  }
  os << "]";
  if (newlines) {
    os << std::endl;
  }
} // listToString

template<typename S>
std::string pointerListToString(const S& l, bool newlines = false) {
  std::ostringstream os;
  for (typename S::const_iterator i = l.begin(); i != l.end(); i++) {
    os << convertToString(&(**i), **i);
    if (newlines) {
      os << std::endl;
    }
    else {
      os << " ";
    }
  }
  return os.str();
} // pointerListToString

template<typename S> 
std::string convertToString(const S* pitem, const S& item) {
  std::ostringstream out;
  mace::printItem(out, pitem, item);
  return out.str();
}

template<typename S>
void printState(std::ostream& out, const void* pitem, const S& item) {
  mace::printItem(out, &item, item);
}

template<typename S>
void printState(std::ostream& out, const Printable* pitem, const S& item) {
  item.printState(out); 
}

template<typename S> 
void printState(std::ostream& out, const boost::shared_ptr<S>* pitem, const boost::shared_ptr<S>& item) {
  out << "shared_ptr(";
  mace::printState(out, item.get(), *item);
  out << ")";
}

template<typename S> 
void printListState(std::ostream& os, const S& b, const S& e, bool newlines = false) {
  os << "[";
  if(newlines) { os << std::endl; } else { os << " "; }
  for (S i = b; i != e; i++) {
    printState(os, &(*i), *i);
    if (newlines) {
      os << std::endl;
    }
    else {
      os << " ";
    }
  }
  os << "]";
  if (newlines) {
    os << std::endl;
  }
} 

} // namespace mace

inline std::ostream& operator<<(std::ostream& o, const mace::Printable& p) {
  p.print(o);
  return o;
}

#endif //__PRINTABLE_H

