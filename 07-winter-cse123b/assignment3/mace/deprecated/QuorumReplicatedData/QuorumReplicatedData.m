#include "VersionedMap.h"

using mace::string;

provides ReplicatedData;

services {
  Transport router = TcpTransport();
}

states {
  ready;
}

messages {
}

state_variables {
  mace::VersionedMap<uint32_t, std::string, uint32_t> data;
}

transitions {
//   upcall deliver(const MaceKey& src, const MaceKey& dest, MessageType t,
// 		 comm_type_t ct) {
//   }

  downcall maceInit() {

  } // maceInit

  downcall setServers(const NodeSet& servers) {
  } // setServers

  downcall exists(const std::string& key) {
  } // exists

  downcall open(const std::string& key) {
    // 
  } // open

  downcall erase(const std::string& key) {
  } // erase

  downcall size(data_id_t id) {
    return 0;
  } // size

  downcall read(data_id_t id, off_t offset, std::string& buf, size_t count) {
    return 0;
  } // read

  downcall write(data_id_t id, off_t offset, const std::string& buf) {
    return 0;
  }
} // transitions

routines {
} // routines
