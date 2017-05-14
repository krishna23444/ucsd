
service SimTreeApp;

provides SimApplication;
trace=med;

constants {
  int INIT = 0;
  int JOINED = 1;
  int ROOT_ONLY = 0;
  int NEXT_NODE = 1;
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
  Tree tree_;
  Overlay treeo_ = tree_;
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
      peers.insert(MaceKey(ipv4, ((me.getMaceAddr().local.addr)%num_nodes)+1));
    } else {
      peers.insert(MaceKey(ipv4, 1)); //BrokenTree and Overcast expect the root to be passed in.
    }
    downcall_joinOverlay(peers);
    return "joinOverlay(" + peers.toString() + ")";
  }

  downcall (state == init) simulateEvent() {
    maceInit();
    return "maceInit()";
  }

}

properties {
  safety {
    //     joinedImpNotNullParent : \forall n \in \nodes : { n.downcall_isJoinedOverlay() \iff ( n.downcall_getParent(MaceKey::null) \neq \null ) };
    //     isRootEqSelfParent : \forall n \in \nodes : { n.downcall_isRoot(MaceKey::null) \iff ( n.downcall_getParent(MaceKey::null) = n ) };
    noLoops : \forall n \in \nodes : { \exists m \in n.(downcall_getParent(MaceKey::null))* : { m.downcall_isRoot(MaceKey::null) \or \not m.downcall_isJoinedOverlay() } };
  }
  liveness {
    allJoined : \forall n \in \nodes : n.downcall_isJoinedOverlay();
    oneRoot : \for{=}{1} n \in \nodes : n.downcall_isRoot(MaceKey::null);
    kidsMatch : \forall n \in \nodes :
            {
              (n.downcall_isRoot(MaceKey::null) \or n \in n.downcall_getParent(MaceKey::null).downcall_getChildren(MaceKey::null))
                \and (\forall m \in n.downcall_getChildren(MaceKey::null) : n = m.downcall_getParent(MaceKey::null))
            };
  }
}
