#ifndef P2P_JOIN_HANDLER_H
#define P2P_JOIN_HANDLER_H 1

#include <string>
#include "MaceTypes.h"
#include "lib/mace_constants.h"
#include "OverlayJoinHandler.h"

class P2PJoinHandler {
public:
  static P2PJoinHandler& NULL_;
  
  static const int P2P_DISCONNECTED = 0;
  static const int P2P_JOINING = 1;
  static const int P2P_CLIENT_JOINED = 2;
  static const int P2P_SUPERNODE_JOINED = 3;
  
  // called with the result of a join
  virtual void JoinResultP2P(join_status_t status, registration_uid_t registrationUid) { }

  //Moderate hack for application layer to know exactly what state we are in :
  //joining, client_joined or supernode_joined.
  //In reality, the application should not know whether we are a supernode or not
  //however, this will help students debug their proejct
  
  virtual void UpdateJoinStatusP2P(int p2pStatus, registration_uid_t registrationUid){ }
  
  virtual ~P2PJoinHandler() {}
}; // P2PJoinHandler

#endif // P2P_JOIN_HANDLER_H
