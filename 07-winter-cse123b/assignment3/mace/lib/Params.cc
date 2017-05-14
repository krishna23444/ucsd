/* 
 * Params.cc : part of the Mace toolkit for building distributed systems
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
#include "Params.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "mace-macros.h"

using namespace std;

const string Params::MACE_LOCAL_ADDRESS = "MACE_LOCAL_ADDRESS";
const string Params::MACE_PORT = "MACE_PORT";
const string Params::MACE_NO_VERIFY_HOSTNAMES = "MACE_NO_VERIFY_HOSTNAMES";
const string Params::MACE_BIND_LOCAL_ADDRESS = "MACE_BIND_LOCAL_ADDRESS";
const string Params::MACE_ALL_HOSTS_REACHABLE = "MACE_ALL_HOSTS_REACHABLE";
const string Params::MACE_CA_FILE = "MACE_CA_FILE";
const string Params::MACE_CERT_FILE = "MACE_CERT_FILE";
const string Params::MACE_PRIVATE_KEY_FILE = "MACE_PRIVATE_KEY_FILE";
const string Params::MACE_PRINT_HOSTNAME_ONLY = "MACE_PRINT_HOSTNAME_ONLY";
const string Params::MACE_LOG_AUTO_SELECTORS = "MACE_LOG_AUTO_SELECTORS";
const string Params::MACE_LOG_TIMESTAMP_HUMAN = "MACE_LOG_TIMESTAMP_HUMAN";
const string Params::MACE_LOG_LEVEL = "MACE_LOG_LEVEL";
const string Params::MACE_LOG_AUTO_ALL = "MACE_LOG_AUTO_ALL";
const string Params::MACE_LOG_FILE = "MACE_LOG_FILE";
const string Params::MACE_LOG_APPEND = "MACE_LOG_APPEND";
const string Params::MACE_LOAD_MONITOR_FREQUENCY_MS = "MACE_LOAD_MONITOR_FREQUENCY_MS";
const string Params::MACE_SWAP_MONITOR_FREQUENCY_MS = "MACE_SWAP_MONITOR_FREQUENCY_MS";
const string Params::MACE_VMRSSLIMIT_FREQUENCY_MS = "MACE_VMRSSLIMIT_FREQUENCY_MS";
const string Params::MACE_TRACE_FILE = "MACE_TRACE_FILE";
const string Params::MACE_BOOTSTRAP_PEERS = "MACE_BOOTSTRAP_PEERS";
const string Params::MACE_AUTO_BOOTSTRAP_PEERS = "MACE_AUTO_BOOTSTRAP_PEERS";

// int param::print(FILE* ostream) {
//   return fprintf(ostream,"REPLAY PARAMETER: %s = \"%s\"\n", key, strvalue.c_str());
// }

param* Params::find(const string& key) {
  ParamMap::iterator iter;
  if ((iter=data.find(key)) != data.end()) {
    return iter->second;
  }
  return NULL;
}

bool Params::containsKey(const string& key) {
  param* found = find(key);
  return found != NULL;
}

int Params::getInt(const string& key) {
  return get<int>(key, 0);
}

double Params::getDouble(const string& key) {
  return get<double>(key, 0.0);
}

const char* Params::getStr(const string& key) {
  param* found = find(key);
  return (found != NULL)?found->getstr():NULL;
}

const char* Params::set(const string& key, const string& value) {
  //printf("params::set(%s, %s)\n",key.c_str(),value.c_str());
  param* found = find(key);
  if(found) {
    //printf("params::set(%s, %s) -- FOUND\n",key.c_str(),value.c_str());
    return found->set(value);
  } else {
    //printf("params::set(%s, %s) -- NOT FOUND\n",key.c_str(),value.c_str());
    data[key] = new param(value);
    return NULL;
  }
}

void Params::setAppend(const string& key, const string& value) {
  param* found = find(key);
  if (found) {
    found->append(value);
  }
  else {
    set(key, value);
  }
} // setAppend

void Params::addRequired(const string& name, const string& desc) {
  required[name] = desc;
} // addRequired

void Params::verifyRequired() {
  // verify we have all the required args
  bool error = false;
  for (RequiredMap::iterator i = required.begin(); i != required.end(); i++) {
    if (!containsKey(i->first)) {
      fprintf(stderr, "required argument: %s - %s\n", i->first.c_str(), i->second.c_str());
      error = true;
    }
  }
  if (error) {
    exit(-1);
  }
} // verifyRequired

int Params::loadparams(int& argc, char**& argv, bool requireFile) {
//   printf("loading params, argc=%d\n", argc);
//   for (int i = 0; i < argc; i++) {
//     printf("%s ", argv[i]);
//   }
//   printf("\n");

  int startarg = 1;
  if(argc==1 || argv[1][0] == '-') {
//     printf("REPLAY PARAMETER FILE: params.default\n");
    loadfile("params.default");
  } else if (argc > 1) {
    startarg = 2;
//     printf("REPLAY PARAMETER FILE: %s\n",argv[1]);
    struct stat sbuf;
    if ((stat(argv[1], &sbuf) == 0) && S_ISREG(sbuf.st_mode)) {
      loadfile(argv[1]);
      argv[1] = 0;
    }
    else if (requireFile) {
      std::cerr << "could not open params file " << argv[1] << std::endl;
      exit(-1);
    }
  }
  else {
    return 0;
  }

  for(int i = startarg; i<argc; i++) {
    if (!argv[i]) {
      continue;
    }
    char* arg = *(argv+i);
    if(arg[0] != '-' || strlen(arg) < 2) continue;
    arg++;
    if(arg[0] == '-') {
      arg++;
    }
//     printf("Node REPLAY_PARM: %s %s\n",argv[i],argv[i+1]);
    //NOTE: For now assuming no '='
    if(i < argc-1) {
      set(arg,argv[i+1]);
      argv[i] = 0;
      argv[i+1] = 0;
    } else {
      argv[i] = 0;
      set(arg, "");
    }
  }

  int found = 0;
  for (int i = 1; i < argc; i++) {
    if (!argv[i]) {
      found++;
      for (int j = i + 1; j < argc; j++) {
	if (argv[j]) {
	  argv[i] = argv[j];
	  break;
	}
      }
    }
  }
  argc -= found;

//   printf("done loading params, argc=%d\n", argc);
//   for (int i = 0; i < argc; i++) {
//     printf("%s ", argv[i]);
//   }
//   printf("\n");

  verifyRequired();
  return 0;
}

string Params::trim(const string& s) {
  int start = 0;
  while (s[start] == ' ' || s[start] == '\t') {
    start++;
  }

  int end = s.size();
  while (s[end - 1] == ' ' || s[end - 1] == '\t') {
    end--;
  }
    
  if (end < start) {
    return "";
  }

  return s.substr(start, end - start);
} // trim

int Params::loadfile(const char* filename, bool allRequired) {
  ifstream f(filename);
  if (!f.is_open()) {
//     cout << "could not open file " << filename << endl;
    return 0;
  }

  uint count = 0;
  while (!f.eof()) {
    count++;
    const int BUF_SIZE = 2048;
    char buf[BUF_SIZE];
    f.getline(buf, BUF_SIZE);

    string s(buf);
    if (s.find('#') != string::npos) {
      string::size_type openPos = s.find('"');
      string::size_type closePos = s.find_last_of('"');
      string::size_type commentPos = s.find('#');
      if (!(openPos != string::npos && closePos != string::npos &&
	    openPos < commentPos && commentPos < closePos)) {
	s = s.substr(0, commentPos);
      }
    }
    s = trim(s);
    if (s.empty()) {
      continue;
    }

    string::size_type i = s.find('=');
    if ((i == string::npos) || (i == 0) || (i == (s.size() - 1))) {
      cerr << "error in parameters file " << filename << endl
	   << "line " << count << ": " << s << endl
	   << "parameters format is key=\"value\"" << endl;
    }
    string key = s.substr(0, i);
    key = trim(key);
    string value = s.substr(i + 1);
    value = trim(value);
    if (value[0] == '"') {
      value = value.substr(1);
    }
    if (value[value.size() - 1] == '"') {
      value = value.substr(0, value.size() - 1);
    }

    setAppend(key, value);
  }
  f.close();

  if (allRequired) {
    verifyRequired();
  }

  return 0;
} // loadfile

// int params::loadfile(const char* filename) {
//   FILE *infile = fopen(filename, "r");
//   if(!infile) {
//     perror("loadfile file open failed");
//     return 0;
//   }
//   char* buffer = new char[81];
//   char* key = new char[81];
//   char* value = new char[81];
//   char* temp;
//   /*   char* begincomment; */
//   /*   char* beginquote; */
//   /*   char* endquote; */
  
//   while(fgets(buffer,80,infile)) {
//     if(sscanf(buffer," %[#]",key)) continue;
//     if(sscanf(buffer," %[^=]",key) != 1) continue;
//     temp = index(buffer, '=')+1;
//     if(temp == NULL) continue;
//     if(sscanf(temp," %[a-zA-Z._0-9-]",value) != 1) {
//       if(sscanf(temp, " '%[^']'",value) != 1) {
//         if(sscanf(temp, " \"%[^\"]\"",value) != 1) {
//           continue;
//         }
//       }
//     }
//     set(key,value);
//     key = new char[81];
//     value = new char[81];
//   }
//   return 0; 
// }

int Params::print(FILE* ostream) {
  int total=fprintf(ostream,"REPLAY PARAMETERS:\n");
  for (ParamMap::iterator i = data.begin(); i != data.end(); i++) {
    total += fprintf(ostream, "REPLAY PARAMETER: %s = \"%s\"\n",
		     i->first.c_str(), i->second->getstr());
  }
  return total;
}

void Params::printUnusedParams(FILE* ostream) {
  fprintf(ostream, "UNUSED PARAMETERS:\n");
  for (ParamMap::iterator i = data.begin(); i != data.end(); i++) {
    if (! i->second->isTouched()) {
      fprintf(ostream, "UNUSED PARAMETER: %s = \"%s\"\n", i->first.c_str(), i->second->getstr());
    }
  }
}

Params::Params() {
}

void Params::getBootstrapPeers(const MaceKey& laddr, NodeSet& peers, bool& autoBootstrap) {
  ADD_SELECTORS("Params::getBootstrapPeers");
  autoBootstrap = containsKey(MACE_AUTO_BOOTSTRAP_PEERS);
  string s;
  if (autoBootstrap) {
    s = get<string>(MACE_AUTO_BOOTSTRAP_PEERS, "");
  }
  else {
    s = get<string>(MACE_BOOTSTRAP_PEERS, "");
  }

  if(s.size()) {
    istringstream is(s);
    while (!is.eof()) {
      string h;
      is >> h;
      MaceKey k(ipv4, h);
      if (k != laddr) {
        peers.insert(k);
      }
    }
  }

  if (autoBootstrap && peers.empty()) {
    maceerr << "No MACE_AUTO_BOOTSTRAP_PEERS defined" << Log::endl;
    ASSERT(0);
  }
}


Params* Params::_inst;
