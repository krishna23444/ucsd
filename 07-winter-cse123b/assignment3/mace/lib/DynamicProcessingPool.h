#ifndef _DYNAMIC_PROCESSING_POOL_H
#define _DYNAMIC_PROCESSING_POOL_H

#include <cassert>
#include <deque>

#include "ThreadCreate.h"
#include "ScopedLock.h"
#include "mset.h"

namespace DynamicProcessingPoolCallbacks {

template<class P, class C, class B1>
class callback_cref {
  typedef void (C::*cb_t) (const B1&);
  P c;
  cb_t f;
public:
  callback_cref(const P &cc, cb_t ff)
    : c (cc), f (ff) {}
  void operator() (const B1& b1)
    { ((*c).*f) (b1); }
};

template<class P, class C, class B1>
class callback_copy {
  typedef void (C::*cb_t) (B1);
  P c;
  cb_t f;
public:
  callback_copy(const P &cc, cb_t ff)
    : c (cc), f (ff) {}
  void operator() (B1 b1)
    { ((*c).*f) (b1); }
};
}; // namespace DynamicProcessingPoolCallbacks


template <class C, class T>
class DynamicProcessingPool {
  
public:
  DynamicProcessingPool(C p, size_t numThreads, size_t maxThreads) :
    proc(p), threadCount(numThreads), maxThreads(maxThreads), stop(false) {

    assert(pthread_mutex_init(&poolMutex, 0) == 0);
    assert(pthread_cond_init(&signalv, 0) == 0);

    for (size_t i = 0; i < threadCount; i++) {
      createThread();
    }
  } // DynamicProcessingPool

  virtual ~DynamicProcessingPool() {
    halt();
  }

  void halt() {
    ScopedLock sl(poolMutex);
    stop = true;
    signal();
  }

  void execute(const T& v) {
    ScopedLock sl(poolMutex);

    tasks.push_back(v);
    if (sleeping.empty() && threads.size() < maxThreads) {
      createThread();
    }
    signal();
  } // execute

protected:
  static void* startThread(void* arg) {
    DynamicProcessingPool<C, T>* pq = (DynamicProcessingPool<C, T> *)arg;
    pq->run();
    return 0;
  } // startThread

private:
  void createThread() {
    pthread_t t;
    runNewThread(&t, DynamicProcessingPool::startThread, this, 0);
    threads.insert(t);
    sleeping.insert(t);
  }

  void lock() const {
    assert(pthread_mutex_lock(&poolMutex) == 0);
  } // lock

  void unlock() const {
    assert(pthread_mutex_unlock(&poolMutex) == 0);
  } // unlock

  void signal() {
    // assumes caller has lock
    pthread_cond_broadcast(&signalv);
  } // signal

  void wait() {
    assert(pthread_cond_wait(&signalv, &poolMutex) == 0);
  } // wait

  void run() {
    ScopedLock sl(poolMutex);

    while (!stop) {
      if (tasks.empty()) {
	wait();
	continue;
      }

      sleeping.erase(pthread_self());
      T t = tasks.front();
      tasks.pop_front();

      sl.unlock();

      proc(t);

      sl.lock();

      if (tasks.empty() && threads.size() > threadCount) {
	threads.erase(pthread_self());
	return;
      }

      sleeping.insert(pthread_self());
    }
  } // run


private:
  C proc;
  size_t threadCount;
  size_t maxThreads;
  bool stop;

  typedef mace::set<pthread_t> ThreadSet;
  ThreadSet threads;
  ThreadSet sleeping;

  mutable pthread_mutex_t poolMutex;
  pthread_cond_t signalv;

  std::deque<T> tasks;

}; // DynamicProcessingPool

#endif // _DYNAMIC_PROCESSING_POOL_H
