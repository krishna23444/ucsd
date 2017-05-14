/*********************************************
 *  ReplicatedMapServiceClass.h
 *  Mace generated on:
 *  Tue May 30 12:54:33 2006
 *********************************************/
#ifndef _REPLICATEDMAPSERVICECLASS_H
#define _REPLICATEDMAPSERVICECLASS_H
#include "ServiceClass.h"
#include "ReplicatedMapHandler.h"


class ReplicatedMapServiceClass : public virtual ServiceClass {
public:
  static ReplicatedMapServiceClass& NULL_;



  virtual void setServers(NodeSet const & servers, registration_uid_t rid = -1) { assert(0); }
  virtual void exists(std::string const & key, registration_uid_t rid = -1) { assert(0); }
  virtual void erase(std::string const & key, registration_uid_t rid = -1) { assert(0); }
  virtual void put(std::string const & key, std::string const & value, bool overwrite = false, registration_uid_t rid = -1) { assert(0); }
  virtual void get(std::string const & key, registration_uid_t rid = -1) { assert(0); }
  virtual registration_uid_t registerHandler(ReplicatedMapHandler& handler, registration_uid_t rid = -1) { assert(0); }
  virtual void unregisterHandler(ReplicatedMapHandler& handler, registration_uid_t rid = -1) { assert(0); }
  void unregisterUniqueHandler(ReplicatedMapHandler& handler) { unregisterHandler(handler); }
  virtual void registerUniqueHandler(ReplicatedMapHandler& handler) { assert(0); }
  virtual ~ReplicatedMapServiceClass() { }
};

#include "NumberGen.h"
#include "ScopedLock.h"
#include "mace.h"
#include "Serializable.h"
#include "XmlRpcCollection.h"

class SynchronousReplicatedMap : public ReplicatedMapHandler {

public:
class SyncGetResult : public mace::Serializable {
public:
  SyncGetResult() { }
  SyncGetResult(std::string const & key, std::string const & value, bool success) : key(key), value(value), success(success) { }
  std::string key;
  std::string value;
  bool success;

  void serialize(std::string& str) const { assert(0); }
  int deserialize(std::istream& in) throw(mace::SerializationException) { assert(0); }
  void serializeXML_RPC(std::string& s) const throw(mace::SerializationException) {
    s.append("<struct>");
    s.append("<member><name>key</name><value>");
    mace::serializeXML_RPC(s, &key, key);
    s.append("</value></member>");
    s.append("<member><name>value</name><value>");
    mace::serializeXML_RPC(s, &value, value);
    s.append("</value></member>");
    s.append("<member><name>success</name><value>");
    mace::serializeXML_RPC(s, &success, success);
    s.append("</value></member>");

    s.append("</struct>");
  }
  int deserializeXML_RPC(std::istream& in) throw(mace::SerializationException) {
    std::istream::pos_type offset = in.tellg();
    in >> skipws;
    mace::SerializationUtil::expect(in, "<struct>");
    in >> skipws;
    for (size_t _i = 0; _i < 3; _i++) {
      mace::SerializationUtil::expect(in, "<member>");
      in >> skipws;
      mace::SerializationUtil::expect(in, "<name>");
      std::string _k = mace::SerializationUtil::get(in, '<');
      mace::SerializationUtil::expect(in, "</name>");
      in >> skipws;
      mace::SerializationUtil::expect(in, "<value>");
      if (_k == "key") {
        mace::deserializeXML_RPC(in, &key, key);
      }
      else if (_k == "value") {
        mace::deserializeXML_RPC(in, &value, value);
      }
      else if (_k == "success") {
        mace::deserializeXML_RPC(in, &success, success);
      }
      else {
        throw mace::SerializationException("unknown member: " + _k);
      }
      mace::SerializationUtil::expect(in, "</value>");
      in >> skipws;
      mace::SerializationUtil::expect(in, "</member>");
      in >> skipws;
    }

    mace::SerializationUtil::expect(in, "</struct>");
    return in.tellg() - offset;
  }

};


private:
  ReplicatedMapServiceClass& _sc;
  ReplicatedMapHandler& replicatedMapHandler;
  registration_uid_t rid;

  typedef mace::map<std::string, boost::shared_ptr<SyncGetResult> > SyncGetResultMap;
  typedef mace::map<std::string, bool> SyncGetResultAsyncMap;
  SyncGetResultMap getResults;
  SyncGetResultAsyncMap getFlags;
  pthread_cond_t getSignal;

public:
  SynchronousReplicatedMap(ReplicatedMapServiceClass& sc, ReplicatedMapHandler& replicatedMapHandler) : _sc(sc), replicatedMapHandler(replicatedMapHandler) {
    rid = NumberGen::Instance(NumberGen::HANDLER_UID)->GetVal();
    _sc.registerHandler((ReplicatedMapHandler&)*this, rid);
    pthread_cond_init(&getSignal, 0);
  }

  registration_uid_t getRegistrationUid() const {
    return rid;
  }

  void setServers(NodeSet const & servers) {
    _sc.setServers(servers, rid);
  }

  void exists(std::string const & key) {
    _sc.exists(key, rid);
  }

  void erase(std::string const & key) {
    _sc.erase(key, rid);
  }

  void put(std::string const & key, std::string const & value, bool overwrite = false) {
    _sc.put(key, value, overwrite, rid);
  }

  void get(std::string const & key) {
    ScopedLock sl(BaseMaceService::agentlock);

    assert(!getFlags.containsKey(key));
    getFlags[key] = true;
    _sc.get(key, rid);
  }

  void replicatedMapExistsResult(std::string const & key, bool exists, registration_uid_t rid = -1) {
    replicatedMapHandler.replicatedMapExistsResult(key, exists, rid);
  }

  void replicatedMapEraseResult(std::string const & key, bool success, registration_uid_t rid = -1) {
    replicatedMapHandler.replicatedMapEraseResult(key, success, rid);
  }

  void replicatedMapPutResult(std::string const & key, bool success, registration_uid_t rid = -1) {
    replicatedMapHandler.replicatedMapPutResult(key, success, rid);
  }

  boost::shared_ptr<SyncGetResult> syncGet(std::string const & key) {
    ScopedLock sl(BaseMaceService::agentlock);

    while (getFlags.containsKey(key)) {
      pthread_cond_wait(&getSignal, &BaseMaceService::agentlock);
    }
    getFlags[key] = false;
    _sc.get(key, rid);
    while (!getResults.containsKey(key)) {
      pthread_cond_wait(&getSignal, &BaseMaceService::agentlock);
    }
    boost::shared_ptr<SyncGetResult> p = getResults[key];
    getResults.erase(key);
    getFlags.erase(key);
    pthread_cond_broadcast(&getSignal);
    return p;
  } // syncGet

  void replicatedMapGetResult(std::string const & key, std::string const & value, bool success, registration_uid_t rid = -1)  {
    ScopedLock sl(BaseMaceService::agentlock);

    SyncGetResultAsyncMap::iterator i = getFlags.find(key);
    if (i == getFlags.end()) {
      assert(0);
    }
    if (i->second) {
      replicatedMapHandler.replicatedMapGetResult(key, value, success, rid);
      getFlags.erase(i);
    }
    else {
      getResults[key] = boost::shared_ptr<SyncGetResult>(new SyncGetResult(key, value, success));
      pthread_cond_broadcast(&getSignal);
    }
  } // replicatedMapGetResult
};


#endif // _REPLICATEDMAPSERVICECLASS_H
