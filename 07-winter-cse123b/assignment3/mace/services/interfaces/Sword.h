typedef mace::hash_map<MaceKey, uint64_t> TimeMap;
typedef mace::hash_map<mace::string, double> DMap;
typedef mace::hash_map<mace::string, mace::string> SMap;
typedef mace::hash_map<mace::string, mace::pair<double, double> > DQ;
typedef mace::hash_map<mace::string, mace::hash_set<mace::string> > SQ; 
typedef mace::pair<DMap, SMap> TDSMap;
typedef mace::hash_map<int, TDSMap> AppUpcallMap;
// don't put UT here!
