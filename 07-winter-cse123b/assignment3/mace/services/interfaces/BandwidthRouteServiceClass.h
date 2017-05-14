/*********************************************
 *  BandwidthRouteServiceClass.h
 *  Mace generated on:
 *  Tue May 30 12:54:22 2006
 *********************************************/
#ifndef _BANDWIDTHROUTESERVICECLASS_H
#define _BANDWIDTHROUTESERVICECLASS_H
#include "RouteServiceClass.h"
#include "ServiceClass.h"

typedef RouteServiceClass::Connection::type BandwidthDirection;
class BandwidthRouteServiceClass : public virtual RouteServiceClass, public virtual ServiceClass {
public:
  static BandwidthRouteServiceClass& NULL_;



  virtual void startSegment(MaceKey const & peer, BandwidthDirection bd, registration_uid_t rid = -1) { assert(0); }
  virtual void endSegment(MaceKey const & peer, BandwidthDirection bd, registration_uid_t rid = -1) { assert(0); }
  virtual double getBandwidth(MaceKey const & peer, BandwidthDirection bd, registration_uid_t rid = -1) { assert(0); }
  virtual bool hasRoom(MaceKey const & peer, registration_uid_t rid = -1) { assert(0); }
  virtual int queued(MaceKey const & peer, registration_uid_t rid = -1) { assert(0); }
  virtual void setWindowSize(MaceKey const & peer, registration_uid_t rid = -1) { assert(0); }
  virtual registration_uid_t registerHandler(ReceiveDataHandler& handler, registration_uid_t rid = -1) { assert(0); }
  virtual void unregisterHandler(ReceiveDataHandler& handler, registration_uid_t rid = -1) { assert(0); }
  void unregisterUniqueHandler(ReceiveDataHandler& handler) { unregisterHandler(handler); }
  virtual void registerUniqueHandler(ReceiveDataHandler& handler) { assert(0); }
  virtual registration_uid_t registerHandler(NetworkErrorHandler& handler, registration_uid_t rid = -1) { assert(0); }
  virtual void unregisterHandler(NetworkErrorHandler& handler, registration_uid_t rid = -1) { assert(0); }
  void unregisterUniqueHandler(NetworkErrorHandler& handler) { unregisterHandler(handler); }
  virtual void registerUniqueHandler(NetworkErrorHandler& handler) { assert(0); }
  virtual ~BandwidthRouteServiceClass() { }
};

#endif // _BANDWIDTHROUTESERVICECLASS_H
