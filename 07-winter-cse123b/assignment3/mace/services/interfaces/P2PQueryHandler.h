#ifndef P2P_QUERY_HANDLER_H
#define P2P_QUERY_HANDLER_H 1

#include <string>
#include <set>
#include "MaceTypes.h"
#include "lib/mace_constants.h"
#include "QueryObject.h"

class P2PQueryHandler {
public:
  static P2PQueryHandler& NULL_;

  virtual void Handle_Query_Result( uint sequence_num, const AppQueryResultSet& results, registration_uid_t registrationUid ) {ASSERT(0);}

  virtual ~P2PQueryHandler() {}

}; // P2PQueryHandler

#endif // P2P_QUERY_HANDLER_H
