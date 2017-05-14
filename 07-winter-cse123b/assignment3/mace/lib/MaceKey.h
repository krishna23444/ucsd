/* 
 * MaceKey.h : part of the Mace toolkit for building distributed systems
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
//
// Included in this file are the new MaceKey classes.
// First is the virtual interface a MaceKey provides.
//
// class MaceKey_interface : public Serializable {
//   public:
//    //common: all should implement
//    virtual bool isNullAddress() = 0;
//    virtual string toHex() = 0;
//    virtual string toString() = 0;
//    virtual void print(std::ostream&) = 0;
//    virtual bool isBitArrMaceKey() = 0;
//
//    //ipv4
//     virtual const MaceAddr& getMaceAddr() = 0;
//    
//    //bitarr: e.g. sha160 and sha32
//    virtual int bitLength() = 0;
//    // digitBits are the number of bits per digit -- commonly 4
//    // dititBits > 32 will produce an exception (?)
//    // digitBits which don't divide 32 will produce an exception (?)
//    virtual uint getNthDigit(int position, uint digitBits = 4) = 0;
//    virtual int sharedPrefixLength(MaceKey key, uint digitBits = 4) = 0;
// };
//
// Then there are the address family constants:
// const int8_t NO_ADDRESS = 0;
// const int8_t IPV4 = 1;
// const int8_t SHA160 = 2;
// const int8_t SHA32 = 3;
// const int8_t STRING_ADDRESS = 4;
//
// There are 3 defined exceptions:
// InvalidMaceKeyException -- thrown when a function is called on a 
//                            MaceKey which does not support the operation
// IndexOutOfBoundsException -- thrown when trying to index a digit which is out of bounds of the
//                              MaceKey
// DigitBaseException -- thrown when a digit base is offered which does not divide 32.
//
// Finally, there are constructors to create the different MaceKey's
// 1. Default constructors:
// MaceKey()
// MaceKey(ipv4_type)
// MaceKey(sha160_type)
// MaceKey(sha32_type)
// [These should create addresses which return true for isNullAddress()]
// 
// 2. IPV4 constructors
// MaceKey(ipv4_type, int) -- the int is the packed IP address
// MaceKey(ipv4_type, string) -- the string is an address, either dotted or hostname
//
// 3. SHA160/32 constructors
// MaceKey(sha###_type, int) -- ### is the number 160 or 32, the int will be SHA hashed, and value truncated as necessary, or salt added
// MaceKey(sha###_type, string) -- the string will be hashed, and the value truncated or salted as necessary
// 
// Q: Should we add SHA128 ids? 
// TODO: This is a first-cut.  We may adapt this as appropriate.
// 
// Usage:
// There are static structs and instances of each of the xxx_type classes.
// Therefore, to create an IPV4 address from 10.0.0.1, you could do:
// MaceKey mykey = MaceKey(MaceKey::ipv4, "10.0.0.1");
//
// Or, if you wanted a SHA32 address from hashing your packed IP address,
// as was used in the original MACEDON, you could do:
// int me = Util::getAddr();
// MaceKey myhash = MaceKey(MaceKey::sha32, me);
//
// NOTE: MaceKey and all its pieces except address family constants are in the 'mace' namespace.
// Implementation Note: IPV4 addresses have been tested, the SHA addresses will
// not work as we need to plug in the hash function still.  Also, the helper
// classes are all privately enclosed in MaceKey, to prevent outside code from
// ever creating or referencing them.  Before adding new MaceKey helper
// classes, take a good look at this code.  You'll have to add a new address
// family, both here and in the compiler, and there are a number of places code
// should be added.  In particular, you'll want to note the bitarr_MaceKey,
// MaceKey_exception, and intarr_MaceKey classes as they may be helpful.

#include <sys/types.h>
#include <netinet/in.h>
// #include <ext/hash_map>
#include <iomanip>
#include <cassert>
#include <boost/shared_ptr.hpp>
#include <string>

// #include "mstring.h"
// #include "mpair.h"
#include "Serializable.h"
#include "Exception.h"
#include "HashUtil.h"
#include "Log.h"
#include "MaceBasics.h"
#include "Collections.h"

#ifndef __MACEKEY_H
#define __MACEKEY_H

#define MACE_KEY_USE_SHARED_PTR

//NOTE: All MaceKey serializations MUST serialize 
//      as the first thing on the wire.

// typedef mace::pair<uint32_t, uint16_t> msockaddr;
// typedef mace::pair<msockaddr, msockaddr> MaceAddr;

namespace mace {

// typedef mace::pair<uint32_t, uint16_t> SockAddrPair;

class SockAddr : public mace::PrintPrintable, public mace::Serializable {
public:
  SockAddr() : addr(INADDR_NONE), port(0) { }
  SockAddr(const SockAddr& s) : addr(s.addr), port(s.port) { }
  SockAddr(uint32_t a, uint16_t p) :  addr(a), port(p) { }
  void print(std::ostream& out) const;
  void serialize(std::string& s) const {
    mace::serialize(s, &addr, addr);
    mace::serialize(s, &port, port);
  } // serialize
  int deserialize(std::istream& in) throw (mace::SerializationException) {
    int o = 0;
    o += mace::deserialize(in, &addr, addr);
    o += mace::deserialize(in, &port, port);
    return o;
  } // deserialize
  bool operator==(const SockAddr& other) const {
    return ((other.addr == addr) && (other.port == port));
  }
  bool operator!=(const SockAddr& other) const {
    return (!(*this == other));
  }
  bool operator<(const SockAddr& other) const {
    if (addr < other.addr) {
      return true;
    }
    else if (addr == other.addr) {
      return port < other.port;
    }
    else {
      return false;
    }
  }

public:
  uint32_t addr;
  uint16_t port;
}; // SockAddr

// typedef mace::pair<SockAddr, SockAddr> MaceAddrPair;

class MaceAddr : public mace::PrintPrintable, public mace::Serializable {
public:
  MaceAddr() : local(SockAddr()), proxy(SockAddr()) { }
  MaceAddr(const MaceAddr& s) : local(s.local), proxy(s.proxy) { }
  MaceAddr(const SockAddr& l, const SockAddr& p) : local(l), proxy(p) { }
  void print(std::ostream& out) const;
  void serialize(std::string& s) const {
    local.serialize(s);
    proxy.serialize(s);
  } // serialize
  int deserialize(std::istream& in) throw (mace::SerializationException) {
    int o = 0;
    o += local.deserialize(in);
    o += proxy.deserialize(in);
    return o;
  } // deserialize
  bool operator==(const MaceAddr& other) const {
    return ((other.local == local) && (other.proxy == proxy));
  }
  bool operator!=(const MaceAddr& other) const {
    return (!(*this == other));
  }
  bool operator<(const MaceAddr& other) const {
    if (local < other.local) {
      return true;
    }
    else if (local == other.local) {
      return proxy < other.proxy;
    }
    else {
      return false;
    }
  }

public:
  SockAddr local;
  SockAddr proxy;
}; // MaceAddr

} // namespace mace

namespace __gnu_cxx {

template<> struct hash<mace::SockAddr> {
  uint32_t operator()(const mace::SockAddr& x) const {
    return hash<uint32_t>()(x.addr * x.port);
  }
};

template<> struct hash<mace::MaceAddr> {
  uint32_t operator()(const mace::MaceAddr& x) const {
    return hash<uint32_t>()((x.local.addr * x.local.port)^
			    (x.proxy.addr * x.proxy.port));
  }
};

} // namespace __gnu_cxx

namespace mace {

class InvalidMaceKeyException : public SerializationException {
public:
  InvalidMaceKeyException(const std::string& m) : SerializationException(m) { }
  void rethrow() const { throw *this; }
};

class IndexOutOfBoundsException : public Exception {
public:
  IndexOutOfBoundsException(const std::string& m) : Exception(m) { }
  void rethrow() const { throw *this; }
};

class DigitBaseException : public Exception {
public:
  DigitBaseException(const std::string& m) : Exception(m) { }
  void rethrow() const { throw *this; }
};

class MaceKey;

class MaceKey_interface : public Serializable, virtual public Printable {
    friend class MaceKey;
  public:
    //common: all should implement
    virtual bool isNullAddress() const = 0;
    virtual std::string toHex() const = 0;
    //     virtual std::string toString() const = 0;
    //     virtual void print(std::ostream&) const = 0;
    virtual std::string addressString() const = 0;
    virtual bool isBitArrMaceKey() const = 0;
    virtual size_t hashOf() const = 0;
    virtual bool operator==(const MaceKey& right) const = 0;
    virtual bool operator<(const MaceKey& right) const = 0;

    //ipv4
    virtual const MaceAddr& getMaceAddr() const = 0;
    
    //bitarr: e.g. sha160 and sha32
    virtual int bitLength() const = 0;
    // digitBits are the number of bits per digit -- commonly 4
    // dititBits > 32 will produce an exception (?)
    // digitBits which don't divide 32 will produce an exception (?)
    virtual uint getNthDigit(int position, uint digitBits = 4) const = 0;
    virtual void setNthDigit(uint value, int position, uint digitBits = 4) = 0;
    virtual int sharedPrefixLength(const MaceKey& key, uint digitBits = 4) const = 0;
  protected:
    virtual uint getNthDigitSafe(int position, uint digitBits) const = 0;
};

class MaceKey : public MaceKey_interface, virtual public PrintPrintable {
  public:
    static const MaceKey null; //for returning, NOT for comparison
  private:
    class MaceKey_interface_helper : public MaceKey_interface {
      public:
        virtual MaceKey_interface_helper* clone() const = 0; //memory to be created by clone function -- needs to be deleted.
    };
#ifndef MACE_KEY_USE_SHARED_PTR
    typedef MaceKey_interface_helper* HelperPtr;
#else
    typedef boost::shared_ptr<MaceKey_interface_helper> HelperPtr;
#endif
    HelperPtr helper;
    int8_t address_family;

  protected:
    // Caller promises helper is not null and that no exceptions will be thrown
    uint getNthDigitSafe(int position, uint digitBits) const { return helper->getNthDigitSafe(position, digitBits); }

  public:
    static bool getHostnameOnly();

    int8_t addressFamily() const { return address_family; }
    bool isNullAddress() const { return (helper==NULL?true:helper->isNullAddress()); }
    std::string toHex() const { 
      std::string hexStr = Log::toHex(std::string((char*)&address_family, sizeof(int8_t )));
      if(helper != NULL) { hexStr += helper->toHex(); }
      return hexStr;
    }
    void print(std::ostream& out) const {
      static bool hostnameOnly = getHostnameOnly();
      if (helper != NULL) {
	if (!hostnameOnly) {
          out << addressFamilyName(address_family) << "/";
	}
        out << (*helper);
      }
      else {
        out << addressFamilyName(address_family) << "/";
      }
    }
    std::string addressString() const {
      return addressString(true);
    }
    std::string addressString(bool printFamily) const {
      if (printFamily) {
        std::ostringstream out;
        out << addressFamilyName(address_family) << "/";
        if (helper != 0) {
          out << helper->addressString();
        }
        return out.str();
      }
      if (helper == 0) {
	throw InvalidMaceKeyException("addressString() called with no helper defined");
      }
      return helper->addressString();
    }
    bool isBitArrMaceKey() const { return (helper==NULL?false:helper->isBitArrMaceKey()); }
    size_t hashOf() const { return (helper==NULL?0:helper->hashOf()); }

    //ipv4
    const MaceAddr& getMaceAddr() const throw(InvalidMaceKeyException) { 
      if(helper==NULL) { 
        throw(InvalidMaceKeyException("No Helper Defined for getMaceAddr")); 
      } else { 
        return helper->getMaceAddr(); 
      } 
    }

    //bitarr
    int bitLength() const throw(InvalidMaceKeyException) { if(helper==NULL) { throw(InvalidMaceKeyException("No Helper Defined")); } else { return helper->bitLength(); } }
    uint getNthDigit(int position, uint digitBits = 4) const throw(Exception) 
      { if(helper==NULL) { throw(InvalidMaceKeyException("No helper defined")); } else { return helper->getNthDigit(position, digitBits); } }
    void setNthDigit(uint digit, int position, uint digitBits = 4) throw(Exception) { 
      if(helper==NULL) { 
        throw(InvalidMaceKeyException("No helper defined")); 
      } else { 
#ifdef MACE_KEY_USE_SHARED_PTR
        if(!helper.unique()) {
          helper = HelperPtr(helper->clone());
        }
#endif
        helper->setNthDigit(digit, position, digitBits); 
      } 
    }
    int sharedPrefixLength(const MaceKey& key, uint digitBits = 4) const throw(InvalidMaceKeyException)
      { if(helper==NULL) { throw(InvalidMaceKeyException("No helper defined")); } else { return helper->sharedPrefixLength(key, digitBits); } }

    virtual void serialize(std::string& str) const {
      mace::serialize(str, &address_family, address_family);
      if(address_family != 0 && helper != NULL) {
        helper->serialize(str);
      }
    }
    virtual int deserialize(std::istream& in) throw(SerializationException) {
      int count = 0;
      count += mace::deserialize(in, &address_family, address_family);
#ifndef MACE_KEY_USE_SHARED_PTR
      delete helper;
#endif
      helper = HelperPtr();
      switch(address_family) {
      case UNDEFINED_ADDRESS: return count;
      case IPV4: helper = HelperPtr(new ipv4_MaceKey()); break;
      case SHA160: helper = HelperPtr(new sha160_MaceKey()); break;
      case SHA32: helper = HelperPtr(new sha32_MaceKey()); break;
      case STRING_ADDRESS: helper = HelperPtr(new string_MaceKey()); break;
      default: throw(InvalidMaceKeyException("Deserializing bad address family "+boost::lexical_cast<std::string>(address_family)+"!"));
      }
      count += helper->deserialize(in); 
      return count;
    }

  virtual void serializeXML_RPC(std::string& str) const throw(SerializationException) {
    StringMap m;
    switch (address_family) {
    case IPV4:
      m["type"] = "ipv4";
      break;
    case STRING_ADDRESS:
      m["type"] = "str";
      break;
    default: throw(InvalidMaceKeyException("Deserializing bad address family "+boost::lexical_cast<std::string>(address_family)+"!"));
    }
    mace::string s;
    helper->serializeXML_RPC(s);
    m["addr"] = s;
    m.serializeXML_RPC(str);
  }

  virtual int deserializeXML_RPC(std::istream& in) throw(SerializationException) {
    StringMap m;
    int r = m.deserializeXML_RPC(in);
    const StringMap::iterator i = m.find("type");
    if (i == m.end()) {
      throw InvalidMaceKeyException("deserialized MaceKey without type");
    }
    const StringMap::iterator ai = m.find("addr");
    if (ai == m.end()) {
      throw InvalidMaceKeyException("deserialized MaceKey without address");
    }
    const mace::string& t = i->second;
    if (t == "ipv4") {
      address_family = IPV4;
      helper = HelperPtr(new ipv4_MaceKey());
    }
    else if (t == "str") {
      address_family = STRING_ADDRESS;
      helper = HelperPtr(new string_MaceKey());
    }      
    else {
      throw InvalidMaceKeyException("deserialized MaceKey with unknown type: " + t);
    }
    std::istringstream is(ai->second);
    helper->deserializeXML_RPC(is);
    return r;
  }


    MaceKey() : helper(HelperPtr()), address_family(UNDEFINED_ADDRESS) {}
    virtual ~MaceKey() { 
#ifndef MACE_KEY_USE_SHARED_PTR
      delete helper; 
#endif
    }

    //operators
    bool operator==(const MaceKey& right) const {
      if(helper == right.helper) { return true; }
      if(addressFamily() != right.addressFamily()) { return false; }
      else if(addressFamily() == 0) { return true; } //XXX: Should this be true or false?  cf. in SQL, NULL != NULL
      else { try {return (*helper) == right;} catch (InvalidMaceKeyException e) { ASSERT(false); return false; } }
    }

    bool operator!=(const MaceKey& right) const {
      return !((*this)==right);
    }

    bool operator<(const MaceKey& right) const {
      if(addressFamily() != right.addressFamily()) { return addressFamily() < right.addressFamily(); }
      else if(addressFamily() == 0) { return false; }
      else { try {return (*helper) < right;} catch (InvalidMaceKeyException e) { ASSERT(false); return false; } }
    }

    bool operator<=(const MaceKey& right) const {
      return ( (*this) < right || (*this) == right);
    }
    bool operator>(const MaceKey& right) const {
      return !( (*this) <= right );
    }
    bool operator>=(const MaceKey& right) const {
      return !( (*this) < right );
    }

    //An assignment operator
    MaceKey& operator=(const MaceKey& right) {
#ifndef MACE_KEY_USE_SHARED_PTR
      if(this != &right) {
	delete helper;
	if(right.helper == NULL)
        {
	  helper = NULL;
          address_family = 0;
        }
	else
        {
	  helper = right.helper->clone();
          address_family = right.address_family;
        }
      }
#else
      helper = right.helper;
      address_family = right.address_family;
#endif
      return *this;
    }

    MaceKey(const MaceKey& right) : helper(), address_family(right.address_family) {
#ifndef MACE_KEY_USE_SHARED_PTR
      if(right.helper != NULL)
      {
	helper = right.helper->clone();
      }
#else
      helper = right.helper;
#endif
    }

    //static functions
    static const char* addressFamilyName(int addressFamily) {
      switch(addressFamily) {
        case UNDEFINED_ADDRESS: return "NONE";
        case IPV4: return "IPV4";
        case SHA160: return "SHA160";
        case SHA32: return "SHA32";
        case STRING_ADDRESS: return "STRING";
        default: return "HUH?";
      }
    }

    static const char* addressFamilyVar(int addressFamily) {
      switch(addressFamily) {
        case UNDEFINED_ADDRESS: return "UNDEFINED_ADDRESS";
        case IPV4: return "IPV4";
        case SHA160: return "SHA160";
        case SHA32: return "SHA32";
        case STRING_ADDRESS: return "STRING_ADDRESS";
        default: return "HUH?";
      }
    }

  private:

    //provides a class which throws an exception for non-common functions
    class MaceKey_exception : public MaceKey_interface_helper {
      protected:
        virtual uint getNthDigitSafe(int position, uint digitBits) const { abort(); }
      public: 
        //ipv4
        virtual const MaceAddr& getMaceAddr() const throw(InvalidMaceKeyException) { throw(InvalidMaceKeyException("Not Implemented For This MaceKey Family")); }

        //bitarr: e.g. sha160 and sha32
        virtual int bitLength() const throw(InvalidMaceKeyException) { throw(InvalidMaceKeyException("Not Implemented For This MaceKey Family")); }
        // digitBits are the number of bits per digit -- commonly 4
        // dititBits > 32 will produce an exception (?)
        // digitBits which don't divide 32 will produce an exception (?)
        virtual uint getNthDigit(int position, uint digitBits = 4) const throw(Exception) { throw(InvalidMaceKeyException("Not Implemented For This MaceKey Family")); }
        virtual void setNthDigit(uint digit, int position, uint digitBits = 4) throw(Exception) { throw(InvalidMaceKeyException("Not Implemented For This MaceKey Family")); }
        virtual int sharedPrefixLength(const MaceKey& key, uint digitBits = 4) const throw(Exception) { throw(InvalidMaceKeyException("Not Implemented For This MaceKey Family")); }
    };

    class ipv4_MaceKey : public MaceKey_exception, virtual public PrintPrintable {
      protected:
        MaceAddr addr;

      public:
        virtual bool isNullAddress() const { return INADDR_NONE == addr.local.addr; }
        virtual std::string toHex() const { return Log::toHex(std::string((char*)&(addr.local.addr), sizeof(uint))); } //Note, currently only does the IP address
        virtual std::string addressString() const;
        virtual void print(std::ostream& out) const;
        virtual bool isBitArrMaceKey() const { return false; }
        virtual size_t hashOf() const { return __gnu_cxx::hash<MaceAddr>()(addr); }

        //ipv4
        virtual const MaceAddr& getMaceAddr() const throw(InvalidMaceKeyException) { return addr; }

        //Constructs an ipv4_MaceKey which would return true for "isNullAddress"
        ipv4_MaceKey(): addr() { }
        ipv4_MaceKey(uint32_t ipaddr, uint16_t port = 0, uint32_t proxyIp = INADDR_NONE, uint16_t proxyPort = 0);
        ipv4_MaceKey(const MaceAddr& ma) {
	  addr = ma;
	}
        //address may be either a dotted-string or a hostname
        ipv4_MaceKey(const std::string& address);
        virtual ~ipv4_MaceKey() { } 

        virtual void serialize(std::string& str) const {
          mace::serialize(str, &addr, addr);
        }
        virtual int deserialize(std::istream& in) throw(SerializationException) {
          return mace::deserialize(in, &addr, addr);
        }
        virtual void serializeXML_RPC(std::string& str) const throw(SerializationException);
        virtual int deserializeXML_RPC(std::istream& in) throw(SerializationException);

        bool operator==(const ipv4_MaceKey& right) const {
          return addr == right.addr;
        }
        bool operator==(const MaceKey& right) const {
          return addr == right.getMaceAddr();
        }
        bool operator<(const MaceKey& right) const {
          return addr < right.getMaceAddr();
        }
        
        MaceKey_interface_helper* clone() const {
          ipv4_MaceKey* key = new ipv4_MaceKey();
          key->addr = addr;
          return key;
        }
    };

    class string_MaceKey : public MaceKey_exception, virtual public PrintPrintable {
      protected:
        mace::string s;

      public:
        virtual bool isNullAddress() const { return s == ""; }
        virtual std::string toHex() const { return Log::toHex(s); } //Note, currently only does the IP address
        virtual std::string addressString() const { return s; }
        virtual void print(std::ostream& out) const { out << "[" << s << "]"; }
        virtual bool isBitArrMaceKey() const { return false; }
        virtual size_t hashOf() const { return __gnu_cxx::hash<mace::string>()(s); }

        //Constructs an ipv4_MaceKey which would return true for "isNullAddress"
        string_MaceKey(): s() { }
        string_MaceKey(const mace::string& t) : s(t) { }
        virtual ~string_MaceKey() { } 

        virtual void serialize(std::string& str) const {
          mace::serialize(str, &s, s);
        }
        virtual int deserialize(std::istream& in) throw(SerializationException) {
          return mace::deserialize(in, &s, s);
        }
      virtual void serializeXML_RPC(std::string& str) const throw(SerializationException) {
	ASSERT(str.empty());
	str = s;
      }
      virtual int deserializeXML_RPC(std::istream& in) throw(SerializationException) {
	in >> s;
	ASSERT(in.eof());
	return s.size();
      }
        bool operator==(const string_MaceKey& right) const {
          return s == right.s;
        }
        bool operator==(const MaceKey& right) const {
          return s == right.addressString(); //assume the types match
        }
        bool operator<(const MaceKey& right) const {
          return s < right.addressString();
        }
        
        MaceKey_interface_helper* clone() const {
          string_MaceKey* key = new string_MaceKey();
          key->s = s;
          return key;
        }
    };

    //These two classes are intended as a helper for creating address types from int or bit arrays,
    //  such as the 160 bit hash key.  They do not define addressFamily() and thus are virtual classes only.
    class bitarr_MaceKey : public MaceKey_exception {
      public:
        //common functions
        bool isBitArrMaceKey() const { return true; }
    };

    //NOTE: "null" addresses will return all 0's for bits, and are only 
    //      distinguishable from the address of all 0's by checking for 
    //      isNullAddress()
    template<int SIZE>
    class intarr_MaceKey : public bitarr_MaceKey, virtual public PrintPrintable {
      protected:
        uint data[SIZE];
        bool null;

        void exceptDivides32(uint digitBits) const throw(DigitBaseException) {
          switch(digitBits) {
            case 4:
            case 32: 
            case 1:
            case 2:
            case 8:
            case 16: return;
          }
          throw(DigitBaseException("DigitBits does not divide 32"));
        }
        void exceptDivides32Bounds(int digit, uint digitBits) const throw(DigitBaseException,IndexOutOfBoundsException) {
          if(digit < 0) {
            throw(IndexOutOfBoundsException("Digit is out of bounds (<= 0)"));
          }
          switch(digitBits) {
            case 4:
              if((uint)digit < SIZE << 3) { return; }
              throw(IndexOutOfBoundsException("Digit is out of bounds"));
            case 32: 
              if((uint)digit < SIZE) { return; }
              throw(IndexOutOfBoundsException("Digit is out of bounds"));
            case 1: 
              if((uint)digit < SIZE << 5) { return; }
              throw(IndexOutOfBoundsException("Digit is out of bounds"));
            case 2:
              if((uint)digit < SIZE << 4) { return; }
              throw(IndexOutOfBoundsException("Digit is out of bounds"));
            case 8:
              if((uint)digit < SIZE << 2) { return; }
              throw(IndexOutOfBoundsException("Digit is out of bounds"));
            case 16:
              if((uint)digit < SIZE << 1) { return; }
              throw(IndexOutOfBoundsException("Digit is out of bounds"));
          }
          throw(DigitBaseException("DigitBits does not divide 32"));
        }

        int getArrPos(int position, uint digitBits) const {
          switch(digitBits) {
            case 4:  return position >> 3;
            case 32: return position;
            case 1:  return position >> 5;
            case 2:  return position >> 4;
            case 8:  return position >> 2;
            case 16: return position >> 1;
          }
          abort();
        }
        uint getDigitMask(int digitBits) const {
          switch(digitBits) {
            case 4:  return 0xf;
            case 1:  return 0x1;
            case 2:  return 0x3;
            case 8:  return 0xff;
            case 16: return 0xffff;
            case 32: return 0xffffffff;
          }
          abort();
        }
        int getShift(int position, uint digitBits) const {
          switch(digitBits) {
            case 4:  return (~(position & 0x7)) << 2;
            case 1:  return (~(position & 0x1f));
            case 2:  return (~(position & 0xf)) << 1;
            case 8:  return (~(position & 0x3)) << 3;
            case 16: return (~(position & 0x1)) << 4;
            case 32: return 0;
          }
	  assert(0);
        }
        uint getNthDigitSafe(int position, uint digitBits) const {
          if(null) { return 0; }
          else if(digitBits == sizeof(int)*8) {
            return data[position];
          } else {
            return (data[getArrPos(position, digitBits)] >> getShift(position, digitBits)) & getDigitMask(digitBits);
          }
        }

      public:
        //common functions
        bool isNullAddress() const { return null; }
        void printHex(std::ostream& out) const {
          std::ios::fmtflags f(out.flags());
          out << std::hex << std::setfill('0');
          for(int i=0; i<SIZE; i++) {
            out << std::setw(sizeof(uint)*2) << data[i];
          }
          out.flags(f);
        }
        std::string toHex() const { 
          std::ostringstream out;
          printHex(out);
          return out.str();
        }
        std::string addressString() const { return toHex(); }
        void print(std::ostream& out) const { printHex(out); }
        virtual size_t hashOf() const {
          size_t hash = 0;
          for(int i = 0; i < SIZE; i++) {
            hash += __gnu_cxx::hash<uint>()(data[i]);
          }
          return hash;
        }
        //bitarr functions
        int bitLength() const throw(InvalidMaceKeyException) { return SIZE * sizeof(uint) * 8; }
        uint getNthDigit(int position, uint digitBits = 4) const throw(Exception) {
          //throw exception if digitBits doesn't divide 32 (sizeof(int)*8)
          //throw exception if position is too big
          //throw exception if position is too small
          exceptDivides32Bounds(position, digitBits);
          return getNthDigitSafe(position, digitBits);
        }
        void setNthDigit(uint digit, int position, uint digitBits = 4) throw(Exception) {
          //throw exception if digitBits doesn't divide 32 (sizeof(int)*8)
          //throw exception if position is too big
          //throw exception if position is too small
          exceptDivides32Bounds(position, digitBits);
          if(null) { throw(InvalidMaceKeyException("Mace Key Array is void")); }
          else if(digitBits == sizeof(int)*8) {
            data[position] = digit;
          } else {
            int arrPos = getArrPos(position, digitBits);
            int shift = getShift(position, digitBits);
            uint DIGIT_MASK = getDigitMask(digitBits) << shift;
            digit &= (1<<digitBits)-1;
            data[arrPos] &= ~DIGIT_MASK;
            data[arrPos] |= (digit << shift);
          }
        }
        int sharedPrefixLength(const MaceKey& key, uint digitBits = 4) const throw(Exception) {
          //throw exception if digitBits doesn't divide 32 (sizeof(int)*8)
          exceptDivides32(digitBits);
          if(null) { return 0; }
          int common;
          unsigned myBitLength, hisBitLength;
          for(common = 0, myBitLength = this->bitLength(), hisBitLength = key.bitLength();
              myBitLength > 0 && hisBitLength > 0 &&
              getNthDigitSafe(common, digitBits) == key.getNthDigitSafe(common, digitBits);
              common++, myBitLength -= digitBits, hisBitLength -= digitBits
              );
          return common;
        }

        virtual void serialize(std::string& str) const {
          mace::serialize(str, &null, null);
          if(null) {
            return;
          }
          for(int i = 0; i < SIZE; i++) {
            mace::serialize(str, (data+i), data[i]);
          }
        }
        //XXX Back things out if deserialization fails.
        virtual int deserialize(std::istream& in) throw(SerializationException) {
          int count = 0;
          count += mace::deserialize(in, &null, null);
          if(!null) {
            for(int i = 0; i < SIZE; i++) {
              count += mace::deserialize(in, (data+i), data[i]);
            }
          }
          return count;
        }

        bool operator==(const intarr_MaceKey& right) const {
          if(bitLength() != right.bitLength()) { return false; }
          for(int i = 0; i < SIZE; i++) {
            if(data[i] != right.data[i]) {
              return false;
            }
          }
          return true;
        }

        bool operator==(const MaceKey& right) const {
          if(bitLength() != right.bitLength()) { return false; }
          return sharedPrefixLength(right, 32) == SIZE;
        }
        //XXX: Does byte order screw this up?
        bool operator<(const MaceKey& right) const {
          int common = sharedPrefixLength(right, 32);
          if(common == SIZE && bitLength() != right.bitLength()) {
            return true; //shorter string is less.
          } else if(common == SIZE) {
            return false; //they are equal
          } else if(common*8 == right.bitLength()) {
            return false; //this string is longer
          }
          return data[common] < right.getNthDigit(common, 32);
        }

        void setArr(const char carr[SIZE*4]) {
          uint* arr = (uint*)carr;
          if(arr == NULL) { 
            null = true; 
          }
          else { 
            null = false;
            for(uint i = 0; i < SIZE; i++) {
              data[i] = ntohl(arr[i]);
            }
          }
        }

        void setArr(const uint arr[SIZE]) {
          if(arr == NULL) { 
            null = true; 
          }
          else { 
            null = false;
            memcpy(data, arr, SIZE*sizeof(uint));
          }
        }

        intarr_MaceKey(): null(true) { }
        intarr_MaceKey(const char carr[SIZE*4]): null(carr==NULL) { 
          const uint* arr = (uint*)carr;
          if(arr != NULL) { 
            for(uint i = 0; i < SIZE; i++) {
              data[i] = ntohl(arr[i]);
            }
          } 
        }
        intarr_MaceKey(const uint arr[SIZE]): null(arr==NULL) { 
          if(arr != NULL) { 
            memcpy(data, arr, SIZE*sizeof(uint));
          } 
        }
    };

    //shaXXX_MaceKey classes get functionality from the intarr_MaceKey classes.
    //By definition, when being constructed, the take the sha hash of something passed
    //in, and 

    class sha160_MaceKey : public intarr_MaceKey<5> {
      public:
        MaceKey_interface_helper* clone() const {
          sha160_MaceKey* key = new sha160_MaceKey();
          memcpy(key->data, data, sizeof(uint)*5);
          key->null = null;
          return key;
        }

        sha160_MaceKey() : intarr_MaceKey<5>() {
// 	  Log::logf("sha160_MaceKey", "empty constructor\n");
	}
        sha160_MaceKey(int toHash) {
	  sha1 buf;
	  HashUtil::computeSha1(toHash, buf);
// 	  Log::logf("sha160_MaceKey", "sha1(%d)=%s\n", toHash, Log::toHex(buf).c_str());
	  setArr(buf.data());
	}
        sha160_MaceKey(const uint arr[5]) : intarr_MaceKey<5>(arr) {
// 	  Log::logf("sha160_MaceKey", "initialized with array\n");
	}
        sha160_MaceKey(const char arr[5*4]) : intarr_MaceKey<5>(arr) {
// 	  Log::logf("sha160_MaceKey", "initialized with array\n");
	}
        sha160_MaceKey(const std::string& toHash) {
	  sha1 buf;
	  HashUtil::computeSha1(toHash, buf);
          //           Log::logf("sha160_MaceKey", "sha1('%s')=%s\n", toHash.c_str(), Log::toHex(buf).c_str());
	  setArr(buf.data());
	}
    };

    class sha32_MaceKey : public intarr_MaceKey<1> {
      public:
        MaceKey_interface_helper* clone() const {
          sha32_MaceKey* key = new sha32_MaceKey();
          memcpy(key->data, data, sizeof(uint));
          key->null = null;
          return key;
        }

        sha32_MaceKey() : intarr_MaceKey<1>() {}
        sha32_MaceKey(int toHash) { sha1 buf; HashUtil::computeSha1(toHash, buf); setArr((uint*)buf.data()); }
        sha32_MaceKey(const uint arr[1]) : intarr_MaceKey<1>(arr) { }
        sha32_MaceKey(const char arr[4]) : intarr_MaceKey<1>(arr) { }
        sha32_MaceKey(const std::string& toHash) { sha1 buf; HashUtil::computeSha1(toHash, buf); setArr(buf.data()); }
    };

  public:

    struct ipv4_type {};
    struct sha160_type {};
    struct sha32_type {};
    struct string_type {};

    //ipv4
    MaceKey(ipv4_type t) : helper(new ipv4_MaceKey()), address_family(IPV4) { }
    MaceKey(ipv4_type t, uint32_t ipaddr, uint16_t port = 0, uint32_t pIp = INADDR_NONE, uint16_t pport = 0) : helper(new ipv4_MaceKey(ipaddr, port, pIp, pport)), address_family(IPV4) { }
    MaceKey(ipv4_type t, const std::string& addr) : helper(new ipv4_MaceKey(addr)), address_family(IPV4) { }
    MaceKey(ipv4_type t, const MaceAddr& maddr) : helper(new ipv4_MaceKey(maddr)), address_family(IPV4) { }

    //sha160
    MaceKey(sha160_type t) : helper(new sha160_MaceKey()), address_family(SHA160) { }
    MaceKey(sha160_type t, int toHash) : helper(new sha160_MaceKey(toHash)), address_family(SHA160) { }
    MaceKey(sha160_type t, const uint arr[5]) : helper(new sha160_MaceKey(arr)), address_family(SHA160) { }
    MaceKey(sha160_type t, const char arr[5*4]) : helper(new sha160_MaceKey(arr)), address_family(SHA160) { }
    MaceKey(sha160_type t, const std::string& toHash) : helper(new sha160_MaceKey(toHash)), address_family(SHA160) { }

    //sha32
    MaceKey(sha32_type t) : helper(new sha32_MaceKey()), address_family(SHA32) { }
    MaceKey(sha32_type t, int toHash) : helper(new sha32_MaceKey(toHash)), address_family(SHA32) { }
    MaceKey(sha32_type t, const uint arr[1]) : helper(new sha32_MaceKey(arr)), address_family(SHA32) { }
    MaceKey(sha32_type t, const char arr[4]) : helper(new sha32_MaceKey(arr)), address_family(SHA32) { }
    MaceKey(sha32_type t, const std::string& toHash) : helper(new sha32_MaceKey(toHash)), address_family(SHA32) { }

    //string
    MaceKey(string_type t) : helper(new string_MaceKey()), address_family(STRING_ADDRESS) { }
    MaceKey(string_type t, const mace::string& u) : helper(new string_MaceKey(u)), address_family(STRING_ADDRESS) { }

    MaceKey(const mace::string& a) {
      if(a.substr(0, 5) == "IPV4/") {
        address_family = IPV4;
        helper = HelperPtr(new ipv4_MaceKey(a.substr(5)));
      } 
      else if(a.substr(0,6) == "SHA160/") {
        if(a.size() != 40+6) {
          throw(InvalidMaceKeyException("SHA160 Address must have 40 hexidecmal characters"));
        }
        address_family = SHA160;
        char arr[5*4];
        for (int i = 0; i < 20; i++) {
          arr[i] = a[6+2*i]*16+a[7+2*i];
        }
        helper = HelperPtr(new sha160_MaceKey(arr));
      }
      else if(a.substr(0,5) == "SHA32/") {
        if(a.size() != 8+5) {
          throw(InvalidMaceKeyException("SHA32 Address must have 8 hexidecmal characters"));
        }
        address_family = SHA32;
        char arr[4];
        for (int i = 0; i < 4; i++) {
          arr[i] = a[5+2*i]*16+a[6+2*i];
        }
        helper = HelperPtr(new sha32_MaceKey(arr));
      }
      else if(a.substr(0,7) == "STRING/") {
        address_family = STRING_ADDRESS;
        helper = HelperPtr(new string_MaceKey(a.substr(7)));
      }
    }
};

const mace::MaceKey::ipv4_type ipv4 = mace::MaceKey::ipv4_type();
const mace::MaceKey::sha160_type sha160 = mace::MaceKey::sha160_type();
const mace::MaceKey::sha32_type sha32 = mace::MaceKey::sha32_type();
const mace::MaceKey::string_type string_key = mace::MaceKey::string_type();

//Defined in utils.cc
// bool isinrange(const MaceKey&, const MaceKey&, const MaceKey&) throw(InvalidMaceKeyException);
// bool isinrangeright(const MaceKey&, const MaceKey&, const MaceKey&) throw(InvalidMaceKeyException);

//Works only with BitArrMaceKey's
class MaceKeyDiff : public PrintPrintable {
  friend MaceKey operator-(const MaceKey& to, const MaceKeyDiff& from) throw(InvalidMaceKeyException);
  friend MaceKey operator+(const MaceKey& to, const MaceKeyDiff& from) throw(InvalidMaceKeyException);
  public:
    enum type_t { ZERO, ___SIGNED, ___UNSIGNED, _INFINITY, _NEG_INFINITY };
  private:
    type_t type;
    int size;
    uint* data;
  public:
    MaceKeyDiff() : type(ZERO), size(0), data(NULL) {}
    MaceKeyDiff(type_t t, int s = 0, uint* d = NULL) : type(t), size(s), data(d) {}
    MaceKeyDiff(const MaceKeyDiff&);
    virtual ~MaceKeyDiff() { delete[] data; }
    void print(std::ostream&) const;
    int sign();
    bool isPositive() { return sign() > 0; }
    bool isNegative() { return sign() < 0; }
    bool isZero() { return sign() == 0; }
    MaceKeyDiff& abs();
    MaceKeyDiff& toSigned() {
      if(type == ___UNSIGNED) { type = ___SIGNED; }
      return *this;
    }
    MaceKeyDiff& toUnsigned() {
      if(type == ___SIGNED) { type = ___UNSIGNED; }
      return *this;
    }
    static MaceKeyDiff NEG_INF;
    static MaceKeyDiff INF;
    MaceKeyDiff& operator=(const MaceKeyDiff&);
    bool operator<(const MaceKeyDiff&) const;
    bool operator>(const MaceKeyDiff& right) const { return !(*this<=right); }
    bool operator==(const MaceKeyDiff&) const;
    bool operator<=(const MaceKeyDiff& right) const;
    bool operator>=(const MaceKeyDiff& right) const { return !(*this<right); }
    MaceKeyDiff operator>>(int bits) const;
    MaceKeyDiff operator+(const MaceKeyDiff&) const;
    MaceKeyDiff operator-(const MaceKeyDiff&) const;
    MaceKeyDiff operator+(uint) const;
    MaceKeyDiff operator-(uint) const;
};

MaceKey operator+(const MaceKey&, const MaceKeyDiff&) throw(InvalidMaceKeyException);
MaceKey operator-(const MaceKey&, const MaceKeyDiff&) throw(InvalidMaceKeyException);
MaceKey operator+(const MaceKey&, uint) throw(InvalidMaceKeyException);
MaceKey operator-(const MaceKey&, uint) throw(InvalidMaceKeyException);
MaceKeyDiff operator-(const MaceKey&, const MaceKey&) throw(InvalidMaceKeyException);
} // namespace mace

//Defining hash template structs for MaceKey
namespace __gnu_cxx {

template<> struct hash<mace::MaceKey>  {
  size_t operator()(const mace::MaceKey& x) const { return x.hashOf(); }
};

template<> struct hash<mace::MaceKey*>  {
  size_t operator()(const mace::MaceKey*& x) {
    return (x==NULL?0:x->hashOf());
  }
};

} // namespace __gnu_cxx

#endif //__MACEKEY_H
