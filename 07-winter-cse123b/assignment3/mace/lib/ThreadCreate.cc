/* 
 * ThreadCreate.cc : part of the Mace toolkit for building distributed systems
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
// #include <errno.h>

#include <sys/types.h>
#include <unistd.h>
#include "ThreadCreate.h"
#include "Log.h"
#include "massert.h"
#include "pip_includer.h"
#include "Util.h"
#include "Scheduler.h"

// _syscall0(pid_t,gettid)

struct FuncArg {
  func f;
  RunThreadClass* c;
  classfunc cf;
  void* arg;
  pthread_t pp;
  std::string fname;
  bool joinThread;
  FuncArg(func tf, RunThreadClass* tc, classfunc tcf, void* targ, 
	  pthread_t tpp, const char* fname, bool join) : f(tf), c(tc), cf(tcf),
							 arg(targ), pp(tpp),
							 fname(fname),
							 joinThread(join) {}
};

void logThread(pthread_t p, const std::string& fname, bool ending) {
//   Log::logf("logThread", "%s :: pid = %u :: ppid = %u :: pthread_id = %u :: tid = %u :: parent_pthread_id = %u %s\n", fname.c_str(), getpid(), getppid(), pthread_self(), gettid(), p, ending ? "ending" : "starting");
#ifndef __CYGWIN__
  Log::logf("logThread", "%s :: pid = %u :: ppid = %u :: pthread_id = %lu :: parent_pthread_id = %lu %s\n", fname.c_str(), getpid(), getppid(), pthread_self(), p, ending ? "ending" : "starting");
#else
  Log::logf("logThread", "%s :: pid = %u :: ppid = %u :: pthread_id = %lu :: parent_pthread_id = %lu %s\n", fname.c_str(), getpid(), getppid(), (uint32_t)pthread_self(), (uint32_t)p, ending ? "ending" : "starting");
#endif
}

void* threadStart(void* vfa) {
  FuncArg* fa = (FuncArg*)vfa;
  #ifdef PIP_MESSAGING
  ANNOTATE_SET_PATH_ID_STR(NULL, 0, "thread-%s-%d", Util::getAddrString(Util::getMaceAddr()).c_str(), (int)pthread_self());
  #endif
  Log::traceNewThread(fa->fname);
  logThread(fa->pp, fa->fname);
  //NOTE: Here would be a great place to add exception handling code, etc.
  if(fa->f != NULL) {
    (*(fa->f))(fa->arg);
  } else {
    ASSERT(fa->c != NULL);
    ASSERT(fa->cf != NULL);
    (fa->c->*(fa->cf))(fa->arg);
  }
  logThread(fa->pp, fa->fname, true);

  if (fa->joinThread) {
    Scheduler::Instance().joinThread(pthread_self());
  }

  delete fa;

  return 0;
}

void _runNewThread(pthread_t* t, func f, void* arg, pthread_attr_t* attr, 
		   const char* fname, bool joinThread) {
  int ret;
  FuncArg *fa = new FuncArg(f, NULL, NULL, arg, pthread_self(), fname, 
			    joinThread);
  if((ret = pthread_create(t, attr, threadStart, fa)) != 0) {
    perror("pthread_create");
    Log::err() << "Error " << ret << " in creating thread!" << Log::endl;
    abort();
  }
  if (joinThread) {
    Scheduler::Instance().shutdownJoinThread(*t);
  }
}

void _runNewThreadClass(pthread_t* t, RunThreadClass* c, classfunc f, 
			void* arg, pthread_attr_t* attr, const char* fname,
			bool joinThread) {
  int ret;
  FuncArg *fa = new FuncArg(NULL, c, f, arg, pthread_self(), fname, 
			    joinThread);
  if((ret = pthread_create(t, attr, threadStart, fa)) != 0) {
    perror("pthread_create");
    Log::err() << "Error " << ret << " in creating thread!" << Log::endl;
    abort();
  }
  if (joinThread) {
    Scheduler::Instance().shutdownJoinThread(*t);
  }
}
