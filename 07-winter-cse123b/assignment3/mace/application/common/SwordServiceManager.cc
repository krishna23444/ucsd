#include <assert.h>
#include <sys/time.h>
#include <errno.h>

#include "SwordServiceManager.h"

using mace::string;

const int Sword_RESPONSE_TIMEOUT_SEC = 15;
const int Sword_RESPONSE_TIMEOUT_NSEC = 0;

SwordServiceManager::SwordServiceManager(SwordServiceClass& dhtsc) : dht(dhtsc) {
  rid = dht.registerHandler(*this);

  pthread_mutex_init(&mapLock, 0);
} // SwordServiceManager

SwordServiceManager::~SwordServiceManager() {
  pthread_mutex_destroy(&mapLock);
} // ~SwordServiceManager

void SwordServiceManager::j2m_mvu(const DMap& dmap, const SMap& smap, uint key[5]) {
  dht.update(dmap, smap, key);
}

// note: despite the name, this is the proper function
void SwordServiceManager::dhtGetResult(const int& seqno, AppUpcallMap& result, registration_uid_t rid) {
  std::cerr << "entered SwordServiceManager::dhtGetResult, seqno " << seqno << " rid " << rid << std::endl;
  results[seqno] = result;
  if (!signalResult(seqno)) {
    std::cerr << "!signalResult(seqno)" << std::endl;
    results.erase(seqno);
  }
} // dhtGetResult

void SwordServiceManager::j2m_query(const DQ& dq, const SQ& sq, const int& seqno, uint lo[5], uint hi[5], /* int[] picked, */ AppUpcallMap& result ) {
  acquireLock(seqno);
  bool waitForCallback;
  bool retry = false;
  do {
    dht.query(dq, sq, seqno, lo, hi, /* picked, */ result, waitForCallback, rid);
    if (waitForCallback) {
      if (waitForSignal(seqno)) {
	result = results[seqno];
	results.erase(seqno);
	retry = false;
      }
      else {
	retry = true;
      }
    }
  } while (retry);
  releaseLock(seqno);
} // get

void SwordServiceManager::acquireLock(const int& k) {
  pthread_mutex_lock(&mapLock);
  pthread_mutex_t* l = 0;
  if (locks.containsKey(k)) {
    counts[k]++;
    l = locks[k];
    assert(l);
  }
  else {
    assert(counts[k] == 0);
    counts[k]++;
    l = new pthread_mutex_t;
    pthread_mutex_init(l, 0);
    pthread_cond_t* s = new pthread_cond_t;
    pthread_cond_init(s, 0);
    locks[k] = l;
    signals[k] = s;
  }
  pthread_mutex_unlock(&mapLock);
  assert(pthread_mutex_lock(l) == 0);
} // acquireLock

bool SwordServiceManager::waitForSignal(const int& k) {
  // assumes that acquireLock was already called for k
  pthread_mutex_lock(&mapLock);

  assert(locks.containsKey(k));
  assert(signals.containsKey(k));
  pthread_mutex_t* l = locks[k];
  assert(l);
  pthread_cond_t* s = signals[k];
  assert(s);

  pthread_mutex_unlock(&mapLock);

  struct timespec timeout;
  struct timeval now;
  gettimeofday(&now, 0);
  timeout.tv_sec = now.tv_sec + Sword_RESPONSE_TIMEOUT_SEC;
  timeout.tv_nsec = (now.tv_usec * 1000) + Sword_RESPONSE_TIMEOUT_NSEC;
  int r = pthread_cond_timedwait(s, l, &timeout);
  if (r == ETIMEDOUT) {
    return false;
  }
  assert(r == 0);
  return true;
} // waitForSignal

bool SwordServiceManager::signalResult(const int& k) {
  pthread_mutex_lock(&mapLock);

  bool sentSignal = false;
  if (signals.containsKey(k)) {
    pthread_cond_t* s = signals[k];

    assert(s);
    assert(pthread_cond_broadcast(s) == 0);

    sentSignal = true;
  }

  pthread_mutex_unlock(&mapLock);
  return sentSignal;
} // signalResult

void SwordServiceManager::releaseLock(const int& k) {
  pthread_mutex_lock(&mapLock);
  pthread_mutex_t* l = locks[k];
  assert(l);
  pthread_cond_t* s = signals[k];
  assert(s);

  assert(pthread_mutex_unlock(l) == 0);

  counts[k]--;
  if (counts[k] == 0) {
    pthread_mutex_destroy(l);
    pthread_cond_destroy(s);
    locks.erase(k);
    signals.erase(k);
    delete l;
    delete s;
    l = 0;
    s = 0;
  }
  
  pthread_mutex_unlock(&mapLock);
} // releaseLock
