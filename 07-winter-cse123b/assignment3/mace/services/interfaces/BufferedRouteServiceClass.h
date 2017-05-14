/*********************************************
 *  BufferedRouteServiceClass.h
 *  Mace generated on:
 *  Tue May 30 12:54:23 2006
 *********************************************/
#ifndef _BUFFEREDROUTESERVICECLASS_H
#define _BUFFEREDROUTESERVICECLASS_H
#include "RouteServiceClass.h"
#include "ServiceClass.h"
#include "ConnectionStatusHandler.h"

#include "RouteServiceClass.h"

struct TimeValueStat {
public:
  TimeValueStat() : time(0), value(0) { }
  TimeValueStat(uint64_t t, size_t v) : time(t), value(v) { }
  uint64_t time;
  size_t value;
};

struct BufferStatistics {
public:
  BufferStatistics(uint64_t w = 0, size_t s = 0, size_t ws = 0) :
    window(w), sum(s), windowSum(ws) { }
  typedef mace::deque<TimeValueStat> HistoryList;
  uint64_t window;
  size_t sum;
  size_t windowSum;
  HistoryList history;

  void append(size_t v) {
    uint64_t now = Util::timeu();
    while (!history.empty() && (now - window > history.front().time)) {
      windowSum -= history.front().value;
      history.pop_front();
    }
    history.push_back(TimeValueStat(now, v));
    sum += v;
    windowSum += v;
  }

};

typedef boost::shared_ptr<BufferStatistics> BufferStatisticsPtr;
class BufferedRouteServiceClass : public virtual RouteServiceClass, public virtual ServiceClass {
public:
  static BufferedRouteServiceClass& NULL_;



  virtual bool canSend(MaceKey const & peer, registration_uid_t rid = -1) { assert(0); }
  virtual size_t outgoingBufferedDataSize(MaceKey const & peer, registration_uid_t rid = -1) { assert(0); }
  virtual void setWindowSize(uint64_t micro, registration_uid_t rid = -1) { assert(0); }
  virtual BufferStatisticsPtr const  getStatistics(MaceKey const & peer, RouteServiceClass::Connection::type d, registration_uid_t rid = -1) { assert(0); }
  virtual void requestToSend(MaceKey const & peer, registration_uid_t rid = -1) { assert(0); }
  virtual size_t bufferedDataSize(registration_uid_t rid = -1) { assert(0); }
  virtual bool hasBufferedData(registration_uid_t rid = -1) { assert(0); }
  virtual bool hasOutgoingBufferedData(registration_uid_t rid = -1) { assert(0); }
  virtual size_t outgoingBufferedDataSize(registration_uid_t rid = -1) { assert(0); }
  virtual bool routeRTS(MaceKey const & dest, std::string const & s, registration_uid_t rid = -1) { assert(0); }
  virtual void requestFlushedNotification(registration_uid_t rid = -1) { assert(0); }
  virtual registration_uid_t registerHandler(ReceiveDataHandler& handler, registration_uid_t rid = -1) { assert(0); }
  virtual void unregisterHandler(ReceiveDataHandler& handler, registration_uid_t rid = -1) { assert(0); }
  void unregisterUniqueHandler(ReceiveDataHandler& handler) { unregisterHandler(handler); }
  virtual void registerUniqueHandler(ReceiveDataHandler& handler) { assert(0); }
  virtual registration_uid_t registerHandler(NetworkErrorHandler& handler, registration_uid_t rid = -1) { assert(0); }
  virtual void unregisterHandler(NetworkErrorHandler& handler, registration_uid_t rid = -1) { assert(0); }
  void unregisterUniqueHandler(NetworkErrorHandler& handler) { unregisterHandler(handler); }
  virtual void registerUniqueHandler(NetworkErrorHandler& handler) { assert(0); }
  virtual registration_uid_t registerHandler(ConnectionStatusHandler& handler, registration_uid_t rid = -1) { assert(0); }
  virtual void unregisterHandler(ConnectionStatusHandler& handler, registration_uid_t rid = -1) { assert(0); }
  void unregisterUniqueHandler(ConnectionStatusHandler& handler) { unregisterHandler(handler); }
  virtual void registerUniqueHandler(ConnectionStatusHandler& handler) { assert(0); }
  virtual ~BufferedRouteServiceClass() { }
};

#include "NumberGen.h"
#include "ScopedLock.h"
#include "mace.h"
#include "Serializable.h"
#include "XmlRpcCollection.h"

class SynchronousBufferedRoute : public ConnectionStatusHandler {

public:

private:
  BufferedRouteServiceClass& _sc;
  ConnectionStatusHandler& connectionStatusHandler;
  registration_uid_t rid;

  bool requestFlushedNotificationFlag;
  pthread_cond_t requestFlushedNotificationSignal;

public:
  SynchronousBufferedRoute(BufferedRouteServiceClass& sc, ConnectionStatusHandler& connectionStatusHandler) : _sc(sc), connectionStatusHandler(connectionStatusHandler), requestFlushedNotificationFlag(false) {
    rid = NumberGen::Instance(NumberGen::HANDLER_UID)->GetVal();
    _sc.registerHandler((ConnectionStatusHandler&)*this, rid);
    pthread_cond_init(&requestFlushedNotificationSignal, 0);
  }

  registration_uid_t getRegistrationUid() const {
    return rid;
  }

  bool canSend(MaceKey const & peer) {
    return _sc.canSend(peer, rid);
  }

  size_t outgoingBufferedDataSize(MaceKey const & peer) {
    return _sc.outgoingBufferedDataSize(peer, rid);
  }

  void setWindowSize(uint64_t micro) {
    _sc.setWindowSize(micro, rid);
  }

  BufferStatisticsPtr const  getStatistics(MaceKey const & peer, RouteServiceClass::Connection::type d) {
    return _sc.getStatistics(peer, d, rid);
  }

  void requestToSend(MaceKey const & peer) {
    _sc.requestToSend(peer, rid);
  }

  size_t bufferedDataSize() {
    return _sc.bufferedDataSize(rid);
  }

  bool hasBufferedData() {
    return _sc.hasBufferedData(rid);
  }

  bool hasOutgoingBufferedData() {
    return _sc.hasOutgoingBufferedData(rid);
  }

  size_t outgoingBufferedDataSize() {
    return _sc.outgoingBufferedDataSize(rid);
  }

  bool routeRTS(MaceKey const & dest, std::string const & s) {
    return _sc.routeRTS(dest, s, rid);
  }

  void requestFlushedNotification() {
    ScopedLock sl(BaseMaceService::agentlock);
    requestFlushedNotificationFlag = true;
    _sc.requestFlushedNotification(rid);
  }

  void clearToSend(MaceKey const & peer, registration_uid_t rid = -1) {
    connectionStatusHandler.clearToSend(peer, rid);
  }

  void flush() {
    ScopedLock sl(BaseMaceService::agentlock);

    _sc.requestFlushedNotification(rid);
    pthread_cond_wait(&requestFlushedNotificationSignal, &BaseMaceService::agentlock);
  } // flush

  void notifyFlushed(registration_uid_t rid = -1)  {
    ScopedLock sl(BaseMaceService::agentlock);

    if (requestFlushedNotificationFlag) {
      connectionStatusHandler.notifyFlushed(rid);
      requestFlushedNotificationFlag = false;
    }
    pthread_cond_broadcast(&requestFlushedNotificationSignal);
  } // notifyFlushed
};


#endif // _BUFFEREDROUTESERVICECLASS_H
