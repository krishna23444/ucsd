/*********************************************
 *  ReplicatedMapHandler.h
 *  Mace generated on:
 *  Tue May 30 12:54:48 2006
 *********************************************/
#ifndef _REPLICATEDMAPHANDLER_H
#define _REPLICATEDMAPHANDLER_H
#include <assert.h>
#include "MaceTypes.h"


class ReplicatedMapHandler  {
public:
  static ReplicatedMapHandler& NULL_;



  virtual void replicatedMapExistsResult(std::string const & key, bool exists, registration_uid_t rid = -1) { assert(0); }
  virtual void replicatedMapEraseResult(std::string const & key, bool success, registration_uid_t rid = -1) { assert(0); }
  virtual void replicatedMapPutResult(std::string const & key, bool success, registration_uid_t rid = -1) { assert(0); }
  virtual void replicatedMapGetResult(std::string const & key, std::string const & value, bool success, registration_uid_t rid = -1) { assert(0); }
  virtual ~ReplicatedMapHandler() { }
};

#endif // _REPLICATEDMAPHANDLER_H
