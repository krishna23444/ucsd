/* 
 * Params.h : part of the Mace toolkit for building distributed systems
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
#ifndef params_h
#define params_h

// #include <stdio.h>
#include <string>
#if __GNUC__ >= 3
#include <ext/hash_map>
#else
#include <hash_map>
#endif

#include <iostream>
#include <boost/lexical_cast.hpp>
#include "hash_string.h"
#include "massert.h"
#include "MaceTypes.h"

//For internal use from within params.  See params below.
class param {
  std::string strvalue;
  bool touched;

public:
  param(const std::string& value) :
    strvalue(value), touched(false) { }
  param() : touched(false) { }
  ~param() { }
  const char* set(const std::string& value) { const char* old = strvalue.c_str(); strvalue = value; return old; }
  const char* getstr() { touched = true; return strvalue.c_str(); }
  const std::string& getStr() { touched = true; return strvalue; }
  bool isTouched() const { return touched; }
  void append(const std::string& value) {
    strvalue.append("\n");
    strvalue.append(value);
  }
//   int print(FILE* ostream);
};

typedef __gnu_cxx::hash_map<std::string, param*, hash_string> ParamMap;

class Params {
public:
  static const std::string MACE_LOCAL_ADDRESS;
  static const std::string MACE_PORT;
  static const std::string MACE_BIND_LOCAL_ADDRESS;
  static const std::string MACE_ALL_HOSTS_REACHABLE;
  static const std::string MACE_CERT_FILE;
  static const std::string MACE_PRIVATE_KEY_FILE;
  static const std::string MACE_CA_FILE;
  static const std::string MACE_NO_VERIFY_HOSTNAMES;
  static const std::string MACE_PRINT_HOSTNAME_ONLY;
  static const std::string MACE_LOG_AUTO_SELECTORS;
  static const std::string MACE_LOG_TIMESTAMP_HUMAN;
  static const std::string MACE_LOG_LEVEL;
  static const std::string MACE_LOG_AUTO_ALL;
  static const std::string MACE_LOG_FILE;
  static const std::string MACE_LOG_APPEND;
  static const std::string MACE_LOAD_MONITOR_FREQUENCY_MS;
  static const std::string MACE_SWAP_MONITOR_FREQUENCY_MS;
  static const std::string MACE_VMRSSLIMIT_FREQUENCY_MS;
  static const std::string MACE_TRACE_FILE;
  static const std::string MACE_BOOTSTRAP_PEERS;
  static const std::string MACE_AUTO_BOOTSTRAP_PEERS;

public:
  bool containsKey(const std::string& key); //returns true if the parameter is set.
  int getInt(const std::string& key); //returns the value of the parameter as an int.
  double getDouble(const std::string& key); //returns the value of the parameter as a double.
  const char* getStr(const std::string& key); //returns the value of the parameter as a string.

  //returns the old value of the param.    
  const char* set(const std::string& key, const std::string& value); 
  void setAppend(const std::string& key, const std::string& value);

  template <typename T>
  T get(const std::string& key) {
    param* p = find(key); 
   if (p != NULL) {
      return boost::lexical_cast<T>(p->getStr());
    }
    else {
      std::cerr << "no param defined for " << key << std::endl;
      ASSERT(0);
    }
  }

  template <typename T>
  T get(const std::string& key, const T& def) {
    param* p = find(key);
    if (p != NULL) {
      return boost::lexical_cast<T>(p->getStr());
    }
    else {
      return def;
    }
  }

  /** 
   * Loads the parameters as though from the command line.  Follows this method:
   * 1 - Ignores argv[0], as this is the program executable
   * 2 - If no option file is specified, defaults are read in from params.default
   * 3 - If argv[1] is a file -- parameters are read in from it
   * 4 - Remaining arguments are read and parsed as command-line [potentially] overriding values.
   * 5 - Returns the number of parameters set.
   */
  int loadparams(int& argc, char**& argv, bool requireFile = false);

  /**
   * Files are read in as follows:
   * # is the comment delimiter.  Lines beginning with it are ignored.  
   *   The remainder of any line containing it, unless a quoted string are ignored.
   * parameters are set by parameter = value, or parameter = "value"
   * allRequired specifies whether all required parameters must be in the file
   */
  int loadfile(const char* filename, bool allRequired = false);

  void addRequired(const std::string& name, const std::string& desc = "");
    
  int print(FILE* ostream); // Prints the parameters to the FILE specified.

  void getBootstrapPeers(const mace::MaceKey& laddr, NodeSet& peers, bool& autoBootstrap);

  void printUnusedParams(FILE* ostream);

  Params();

  static Params* Instance() { 
    if (_inst == NULL) {
      _inst = new Params();
    }
    return _inst;
  }
  
private:  
  static Params *_inst;

  ParamMap data;
  typedef __gnu_cxx::hash_map<std::string, std::string, hash_string> RequiredMap;
  RequiredMap required;

  param* find(const std::string& key);
  std::string trim(const std::string& s);
  void verifyRequired();

};

#endif
