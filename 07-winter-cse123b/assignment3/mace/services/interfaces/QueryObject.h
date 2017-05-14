#ifndef _QUERY_OBJECT_H
#define _QUERY_OBJECT_H 1

#include <string>
#include <set>
#include "MaceTypes.h"
#include "lib/mace_constants.h"


//typedef mace::hash_set<mace::string> ClientQueryResult;
typedef mace::hash_set<mace::string> FileSet;

//typedef mace::hash_map<MaceKey, FileSet> QueryResultObject;

class AppQueryResult {
 public:
  int port;
  FileSet files;

  AppQueryResult(int p, FileSet f)
  :port(p), files(f)
  {
  }

  AppQueryResult()
  {
  }
};

//query result for the application - this is NOT serializable
typedef mace::hash_map<MaceKey, AppQueryResult> AppQueryResultSet;

#endif
