
service SimOverlayRouterApp;

provides SimApplication;
trace=low;

constants {
  int INIT = 0;
  int JOINED = 1;
  int ROOT_ONLY = 0;
  int NEXT_NODE = 1;
  int RANDOM_LESSER = 2;
}

constructor_parameters {
  int PEERSET_STYLE = ROOT_ONLY;
  int SHOULD_JOIN = 1;
  MaceKey me = MaceKey::null;
  int num_nodes = -1;
}

states {
  needJoin;
  done;
}

services {
  OverlayRouter tree_;
}

transitions {

  downcall (state == init) maceInit() {
    state = needJoin;
  }

  downcall (state == done) eventsWaiting() {
    return false;
  }
  downcall eventsWaiting() {
    return true;
  }
  
  downcall (state == needJoin) simulateEvent() {
    state = done;
    NodeSet peers;
    if(PEERSET_STYLE == NEXT_NODE) {
      peers.insert(upcall_getMaceKey((upcall_getNodeNumber() + 1) % num_nodes));
    } else if(PEERSET_STYLE == RANDOM_LESSER && upcall_getNodeNumber() != 0) {
      peers.insert(upcall_getMaceKey(Util::randInt(upcall_getNodeNumber()))); // get a random node below you (if you're not node 0)
    } else {
      peers.insert(upcall_getMaceKey(0)); // pass in the same node to everyone
    }
    downcall_joinOverlay(peers);
    return "joinOverlay(" + peers.toString() + ")";
  }

  downcall (state == init) simulateEvent() {
    maceInit();
    return "maceInit()";
  }

}

