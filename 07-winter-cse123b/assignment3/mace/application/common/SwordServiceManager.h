#ifndef Sword_SERVICE_MANAGER_H
#define Sword_SERVICE_MANAGER_H

#include <pthread.h>
#include "SwordServiceClass.h"
#include "mhash_map.h"

class SwordServiceManager : public SwordDataHandler {

  typedef mace::hash_map<int, pthread_mutex_t*> KeyLockMap;
  typedef mace::hash_map<int, pthread_cond_t*> KeySignalMap;
  typedef mace::hash_map<int, uint> KeyCountMap;
//  typedef mace::hash_map<MaceKey, bool> KeyBoolMap;
//  typedef mace::hash_map<MaceKey, mace::string> KeyStringMap;
  typedef mace::hash_map<int, AppUpcallMap> SwordResultMap; // seqno -> UT

public:
  SwordServiceManager(SwordServiceClass& dhtsc);
  virtual ~SwordServiceManager();
  virtual void j2m_mvu(const DMap& dmap, const SMap& smap, uint key[5]);
  virtual void j2m_query(const DQ& dq, const SQ& sq, const int& seqno, uint lo[5], uint hi[5], AppUpcallMap& result);
  virtual void dhtGetResult(const int& seqno, AppUpcallMap& result, registration_uid_t rid);

private:
  void acquireLock(const int& k);
  void releaseLock(const int& k);
  // returns false if timed out, true if signaled
  bool waitForSignal(const int& k);
  // returns true if the result was signaled
  bool signalResult(const int& k);

private:
  SwordServiceClass& dht;
  registration_uid_t rid;
  KeyLockMap locks;
  KeySignalMap signals;
  KeyCountMap counts;
  SwordResultMap results;
  pthread_mutex_t mapLock;
}; // SwordServiceManager

#endif // Sword_SERVICE_MANAGER_H
