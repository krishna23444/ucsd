#include "lib/mstring.h"

#include <boost/lexical_cast.hpp>

using mace::string;

service SimP2PApp;
provides SimApplication;

trace=med;

states {
  needToJoin;
  sentJoin;
  needToPublish;
  needToSearch;
  waitingForSearch;
  done;
  bad;
}


services {
  P2P p2p_;
}

state_variables {
  // 0 denotes we have not heard from
  // 1 denotes we are awaiting a result
  // 2 means we got the result
  mace::hash_map<int, int> searchResultMap;
  timer searchTimer;
}

transitions {
  downcall (state == init) maceInit() {
    state = needToJoin;
  }

  downcall (state == done) eventsWaiting() {
    return false;
  }
  
  downcall (state == needToPublish) eventsWaiting() {
    return false;
  }

  downcall (state == sentJoin) eventsWaiting() {
    return false;
  }

  downcall (state == waitingForSearch) eventsWaiting() {
    return false;
  }
  downcall (true) eventsWaiting() {
    return true;
  }

  // Do a join first.  We always join to incarnation 0 
  downcall (state == needToJoin) simulateEvent() {

    int incarnationNum = upcall_getNodeNumber();
    MaceKey rootKey = upcall_getMaceKey(0);
    double bw = 10.0 * incarnationNum;
    short httpPort = 6666 + incarnationNum;
    string identity = "Node " + 
      upcall_getMaceKey(incarnationNum).addressString();

    state = sentJoin;
    downcall_joinP2P(rootKey, bw, httpPort, identity, p2p_);
    return "simulateEvent(needToJoin)";
  }
  

  upcall (state == sentJoin) joinStatusChanged(join_status_t newstatus) {
    if ((newstatus == P2P_CLIENT_JOINED) || 
	(newstatus == P2P_SUPERNODE_JOINED)) {
      state = needToPublish;
    } else if (newstatus == P2P_DISCONNECTED) {
      state = needToJoin;
    }
  }

  upcall (true) joinStatusChanged(join_status_t newstatus) {
    maceout << "MY JOIN STATUS CHANGED TO " << newstatus << Log::endl;
    maceout << "I am in state " << state << Log::endl;
//    ASSERT(0);
  }
  // upcall (state != needToPublish) getAvailableFiles() {
//     FileSet fs;
//     maceout << "chip was right!" << Log::endl;
//     return fs;
//   }
  
//   upcall (state == needToPublish) getAvailableFiles() {
  upcall (true) getAvailableFiles() {
    FileSet fs;
    char fname[1024];
    sprintf(fname, "commonstring%d", upcall_getNodeNumber());
    fs.insert( std::string(fname) );

    if (state == needToPublish)
      state = needToSearch;
    return fs;
  }

  

  // Wait until we've gotten an upcall to publish before searching
  downcall (state == needToSearch) simulateEvent() {
    // XXX will the incarnation count change over time?
    for (int i=0; i < upcall_getNodeCount(); i++) {
      searchResultMap[i] = 1;
    }
    // XXX ask Chip - if i want to wait a specified # of cycles before
    // sending teh search string, should I do a timeout or just wait
    // until we get n downcalls for simulateEvent ??

    state = waitingForSearch;
    searchTimer.reschedule(1 * 1000 * 1000);
    downcall_queryP2P("commonstring");
    return "simulateEvent()";
  }

  
  upcall (state == waitingForSearch) 
    handleQueryResult(uint sequence_num, const QueryResultSet& results) {
    QueryResultSet::const_iterator i;
    int cnt;
    int fileNumber;

    string number_str;
    for(i = results.begin(); i != results.end(); i++)
    {
      FileSet::const_iterator j;

      for(j = i->files.begin(); 
	  j != i->files.end(); 
	  j++)
      {	
	maceout << "String: " << *j << Log::endl;
	maceout << "size : " << j->size() << Log::endl;

	// The string "commonstring" is 12 characters long
	ASSERT(j->size() >= 13);
	number_str = j->substr(12, string::npos);
	maceout << "number_str " << number_str << Log::endl;

	//fileNumber = atoi(j->substr(12, string::npos).c_str());
	fileNumber = boost::lexical_cast<int>(number_str);
	
//	if (fileNumber == 2)
//	    ASSERT(0);
	
	maceout << "FileNumber " << fileNumber << Log::endl;
	maceout << "Get node count " << upcall_getNodeCount() << Log::endl;
	ASSERT( (fileNumber >= 0) && (fileNumber < upcall_getNodeCount()) );
	maceout << "searchResult[" << fileNumber << "] = " << searchResultMap[fileNumber] 
		<< " (expecting 1)" << Log::endl;
	
        // XXX would we get 2x results from the same person!?
	ASSERT((searchResultMap[fileNumber]) == 1 || (searchResultMap[fileNumber] == 2));
	maceout << " Before setting searchResultmap[" << fileNumber << "] = 2" << Log::endl;
	searchResultMap[fileNumber] = 2;
	
      }
    }
    maceout << "looping through " << Log::endl;
    // Check if we have gotten everything
    for (cnt=0; cnt < upcall_getNodeCount(); cnt++) {
      if (searchResultMap[cnt] != 2)
	break;
    }
    maceout << "after loop cnt = " << cnt << Log::endl;

    if (cnt == upcall_getNodeCount())
      state = done;
  }
  
  // all other handleQueryResult upcalls
  upcall (true) handleQueryResult(uint sequence_num, const QueryResultSet& results) {
    // Do nothing
  }
  

  scheduler (true) searchTimer() {
    if (state == waitingForSearch) {
      downcall_queryP2P("commonstring");
      searchTimer.reschedule(1 * 1000 * 1000);
    }
  }

  downcall (state == init) simulateEvent() {
    maceInit();
    return "maceInit()";
  }
	    
}

properties
{
  safety {
  }
  liveness {
    // All nodes have received data from everyone 
    // For now let's check that all nodes have joined (specified in Cal_A_Lum)
    testDone : \forall n \in \nodes : { n.state = done };

  }
}
