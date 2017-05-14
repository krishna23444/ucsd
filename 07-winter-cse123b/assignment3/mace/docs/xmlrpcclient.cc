/* 
 * xmlrpcclient.cc : part of the Mace toolkit for building distributed systems
 * 
 * Copyright (c) 2006, James W. Anderson, Charles Killian, Ryan Braud, Amin Vahdat
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
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <getopt.h>
#include <cassert>
#include <sstream>
#include <fstream>
#include <iostream>

#include <string>

#include "Util.h"
#include "StrUtil.h"
#include "SysUtil.h"

#include "MathServiceXmlRpcClient.h"
#include "HttpClient.h"

using namespace std;

string version = "1.1";
bool quiet = false;
bool profile = false;

void fetch(string host, int port, string path, int count) {

  HttpClient c(host, port);

  long long totaltime = 0;
  int reqCount = count;

//   do {
//     timeval starttime;
//     gettimeofday(&starttime, 0);

//     HttpResponse r = c.getUrl(path, version, false);
//     if (!quiet) {
//       if (r.headers.containsKey("Content-Type")) {
// 	if (r.headers["Content-Type"].find("text/") == 0) {
// 	  cout << r.content << endl;
// 	}
// 	else {
// 	  cout << "unsupported content type: " << r.headers["Content-Type"] << endl;
// 	}
//       }
//     }

//     timeval end;
//     gettimeofday(&end, 0);
//     totaltime += Util::timediff(starttime, end);

//     count--;
//   } while (count);

  int ccount = count;
  do {
    c.getUrlAsync(path, version, version == "1.1");

    count--;
  } while (count);

  count = ccount;
  do {
    timeval starttime;
    gettimeofday(&starttime, 0);

    HttpResponse r;
    if (c.hasAsyncResponse()) {
      r = c.getAsyncResponse();
      count--;
    }
    else {
      struct timeval tv = { 0, 50 * 1000 };
      SysUtil::select(0, 0, 0, 0, &tv);
    }
    
    if (!quiet) {
      if (r.headers.containsKey("Content-Type")) {
	if (r.headers["Content-Type"].find("text/") == 0) {
	  cout << r.content << endl;
	}
	else {
	  cout << "unsupported content type: " << r.headers["Content-Type"] << endl;
	}
      }
    }

    timeval end;
    gettimeofday(&end, 0);
    totaltime += Util::timediff(starttime, end);

  } while (count);

  totaltime /= reqCount;

  if (profile) {
    cerr << totaltime << "\t" << totaltime / 1000000 << endl;
  }
} // fetch

int main(int argc, char* argv[]) {
  int port = 6666;

  try {
    MathServiceXmlRpcClient c("localhost", port, "/xmlrpc");
    mace::deque<int> addv;
    for (int i = 1; i < argc; i++) {
      addv.push_back(atoi(argv[i]));
    }
    int sum = c.add(addv);
    cout << sum << endl;

    StringIntHMap sdm = c.sumAndDifference(atoi(argv[1]), atoi(argv[2]));
    cout << sdm["sum"] << " " << sdm["difference"] << endl;

//     IntMap m;
//     for (int i = 10; i > 0; i--) {
//       m[i] = "foo" + StrUtil::intAsString(i);
//     }
//     string s = c.printMap(m);
//     cerr << s << endl;

//     StringIntHMap sm = c.reverseMap(m);
//     for (StringIntHMap::iterator i = sm.begin(); i != sm.end(); i++) {
//       cerr << "sm[" << i->first << "]=" << i->second << endl;
//     }
    
  } catch (const Exception& e) {
    cerr << e << endl;
  }

//   string usage = "usage: ";
//   usage += argv[0];
//   usage += " url [--version 1.0|1.1] [--count count] [--quiet] [--profile]\n";

//   while (1) {
//     int option_index = 0;
//     static struct option long_options[] = {
//       {"count", required_argument, 0, 'c'},
//       {"version", required_argument, 0, 'v'},
//       {"quiet", no_argument, 0, 'q'},
//       {"profile", no_argument, 0, 'p'},
//       {"help", no_argument, 0, 'h'},
//       {0, 0, 0, 0}
//     };

//     int c = getopt_long(argc, argv, "c:v:qhp", long_options, &option_index);
//     if (c == -1) {
//       break;
//     }

//     string sarg;

//     switch (c) {

//     case 'c':
//       sarg = optarg;
//       assert(StrUtil::parseInt(sarg, count));
//       break;
//     case 'v':
//       version = optarg;
//       assert(version == "1.1" || version == "1.0");
//       break;
//     case 'q':
//       quiet = !quiet;
//       break;
//     case 'p':
//       profile = !profile;
//       break;
//     case 'h':
//       cerr << usage;
//       exit(-1);

//     default:
//       cerr << "unknown option\n";
//       cerr << usage;
//       exit(-1);
//     }
//   }

//   if (argc <= optind) {
//     cerr << usage;
//     exit(-1);
//   }
  
//   string url = argv[optind];

//   string http = "http://";
//   if (url.find(http) == 0) {
//     url = url.substr(http.size());
//   }

//   StringList m = StrUtil::match("([^/]+)/(.*)", url);
//   if (m.empty()) {
//     cerr << "bad url " << url << endl;
//     exit(-1);
//   }

//   string host = m[0];
//   string path = "/" + m[1];

//   m = StrUtil::match("([^:]+):(\\d+)", host);
//   if (!m.empty()) {
//     host = m[0];
//     assert(StrUtil::parseInt(m[1], port));
//   }

//   try {
//     fetch(host, port, path, count);
//   } catch (const Exception& e) {
//     cerr << e << endl;
//   }

  return 0;
} // main

