#ifndef P2P_FILE_HANDLER_H
#define P2P_FILE_HANDLER_H 1

#include <string>
#include <set>
#include "MaceTypes.h"
#include "lib/mace_constants.h"

#include "QueryObject.h"

class P2PFileHandler {
public:
  static P2PFileHandler& NULL_;

  virtual FileSet Get_Available_Files( registration_uid_t registrationUid ) {ASSERT(0);}
  //virtual void peerJoinedOverlay(const MaceKey& peer, registration_uid_t registrationUid) { }

  // called with the result of a join
  //virtual void joinResultOverlay(const MaceKey& source, join_status_t status, registration_uid_t registrationUid) { }

  virtual ~P2PFileHandler() {}
}; // P2PFileHandler

#endif // P2P_FILE_HANDLER_H
