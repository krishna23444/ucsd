/* 
 * ThreadPool.h : part of the Mace toolkit for building distributed systems
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
#ifndef _MACE_THREAD_POOL_H
#define _MACE_THREAD_POOL_H

#include <list>
#include <pthread.h>

#include "mvector.h"
#include "mhash_set.h"
#include "mace-macros.h"

#include "ScopedLock.h"
#include "ScopedLog.h"
#include "Util.h"
#include "ThreadCreate.h"
#include "Scheduler.h"

namespace mace {

  template<class C, class D>
  class ThreadPool {

  public:
    typedef bool (C::*ConditionFP)(uint);
    typedef void (C::*WorkFP)(uint);

  private:
    struct ThreadArg {
      ThreadPool* p;
      uint i;
    };

  public:
    ThreadPool(C& o,
	       ConditionFP cond,
	       WorkFP process,
	       WorkFP setup = 0,
	       WorkFP finish = 0,
	       uint16_t numThreads = 1) :
      obj(o), dstore(0), cond(cond), process(process), setup(setup), finish(finish),
      threadCount(numThreads), sleeping(0), stop(false) {

      dstore = new D[threadCount];
      sleeping = new uint[threadCount];
      for (uint i = 0; i < threadCount; i++) {
	sleeping[i] = 0;
      }

      assert(pthread_key_create(&key, 0) == 0);
      assert(pthread_mutex_init(&poolMutex, 0) == 0);
      assert(pthread_cond_init(&signalv, 0) == 0);
      
      for (uint i = 0; i < threadCount; i++) {
// 	pthread_cond_t sig;
// 	assert(pthread_cond_init(&sig, 0) == 0);
// 	signals.push_back(sig);
	pthread_t t;
	ThreadArg* ta = new ThreadArg;
	ta->p = this;
	ta->i = i;
	runNewThread(&t, ThreadPool::startThread, ta, 0);
	threads.push_back(t);
      }
      assert(threadCount == threads.size());
    } // ThreadPool

    virtual ~ThreadPool() {
      halt();

//       for (uint i = 0; i < threads.size(); i++) {
// 	assert(pthread_cond_destroy(&(signals[i])) == 0);
//       }

      assert(pthread_mutex_destroy(&poolMutex) == 0);
      assert(pthread_cond_destroy(&signalv) == 0);
      assert(pthread_key_delete(key) == 0);

      delete [] dstore;
      delete [] sleeping;
    } // ~ThreadPool

    uint getThreadCount() const {
      return threadCount;
    }

    size_t size() const {
      return threadCount;
    } // poolSize

    size_t sleepingSize() const {
      size_t r = 0;
      for (uint i = 0; i < threadCount; i++) {
	r += sleeping[i];
      }
      return r;
    } // sleepingSize

    bool isDone() const {
      return (sleepingSize() == threadCount);
    } // isDone

    void halt() {
      stop = true;
      signal();
    } // halt

    void signal() {
      lock();
//       for (uint i = 0; i < signals.size(); i++) {
// 	assert(pthread_cond_signal(&(signals[i])) == 0);
//       }
      pthread_cond_broadcast(&signalv);
      unlock();
    } // signal

    D& data(uint i) const {
      assert(i < threadCount);
      return dstore[i];
    } // data

    void* getSpecific() {
      return pthread_getspecific(key);
    } // getSpecific

    void setSpecific(const void* v) {
      assert(pthread_setspecific(key, v) == 0);
    } // setSpecific

    void lock() const {
      assert(pthread_mutex_lock(&poolMutex) == 0);
    } // lock

    void unlock() const {
      assert(pthread_mutex_unlock(&poolMutex) == 0);
    } // unlock

  protected:
    static void* startThread(void* arg) {
      ThreadArg* a = (ThreadArg*)arg;
      ThreadPool* t = (ThreadPool*)(a->p);
      t->run(a->i);
      delete a;
      return 0;
    } // startThread

//     void wait(uint index, pthread_mutex_t& l) {
    void wait(uint index) {
//       struct timeval now;
//       gettimeofday(&now, 0);
//       struct timespec timeout;
//       timeout.tv_sec = now.tv_sec;
//       timeout.tv_nsec = ((now.tv_usec * 1000) + (5 * 1000 * 1000) +
// 			 Util::randInt(5 * 1000 * 1000));
//       assert(pthread_cond_wait(&(signals[index]), &poolMutex) == 0);
      assert(pthread_cond_wait(&signalv, &poolMutex) == 0);
//       pthread_cond_timedwait(&(signals[index]), &poolMutex, &timeout);
    } // wait

  private:
    void run(uint index) {
      assert(index < threadCount);
      ScopedLock sl(poolMutex);

      while (!stop) {
	if (!(obj.*cond)(index)) {
	  sleeping[index] = 1;
	  wait(index);
	  continue;
	}

	sleeping[index] = 0;

	if (setup) {
	  (obj.*setup)(index);
	}
	sl.unlock();
	(obj.*process)(index);
	sl.lock();
	if (finish) {
	  (obj.*finish)(index);
	}
      }
    } // run

  private:
    C& obj;
    D* dstore;
    ConditionFP cond;
    WorkFP process;
    WorkFP setup;
    WorkFP finish;
    uint threadCount;
    uint* sleeping;
    bool stop;
    pthread_key_t key;
    mutable pthread_mutex_t poolMutex;
    pthread_cond_t signalv;

    typedef vector<pthread_t> ThreadList;
    ThreadList threads;
//     typedef vector<pthread_cond_t> SignalList;
//     SignalList signals;
  }; // ThreadPool

} // namespace mace

#endif // _MACE_THREAD_POOL_H
