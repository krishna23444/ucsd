/*********************************************
 *  ReplicatedDataHandler.h
 *  Mace generated on:
 *  Tue May 30 12:54:48 2006
 *********************************************/
#ifndef _REPLICATEDDATAHANDLER_H
#define _REPLICATEDDATAHANDLER_H
#include <assert.h>
#include "MaceTypes.h"

typedef uint32_t data_id_t;
class ReplicatedDataHandler  {
public:
  static ReplicatedDataHandler& NULL_;



  virtual void replicatedDataExistsResult(std::string const & key, bool exists, registration_uid_t rid = -1) { assert(0); }
  virtual void replicatedDataEraseResult(std::string const & key, bool success, registration_uid_t rid = -1) { assert(0); }
  virtual void replicatedDataOpenResult(std::string const & key, data_id_t id, bool success, registration_uid_t rid = -1) { assert(0); }
  virtual void replicatedDataSizeResult(data_id_t id, int sizeId, size_t size, registration_uid_t rid = -1) { assert(0); }
  virtual void replicatedDataReadResult(data_id_t id, int readId, off_t offset, std::string const & buf, bool success, registration_uid_t rid = -1) { assert(0); }
  virtual void replicatedDataWriteResult(data_id_t id, int writeId, off_t offset, bool success, registration_uid_t rid = -1) { assert(0); }
  virtual ~ReplicatedDataHandler() { }
};

#endif // _REPLICATEDDATAHANDLER_H
