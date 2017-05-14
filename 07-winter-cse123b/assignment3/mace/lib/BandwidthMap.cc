/* 
 * BandwidthMap.cc : part of the Mace toolkit for building distributed systems
 * 
 * Copyright (c) 2006, Charles Killian, Dejan Kostic, Ryan Braud, James W. Anderson, John Fisher-Ogden, Calvin Hubble, Duy Nguyen, Justin Burke, David Oppenheimer, Amin Vahdat, Adolfo Rodriguez, Sooraj Bhat
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in
 *      the documentation and/or other materials provided with the
 *      distribution.
 *    * Neither the names of Duke University nor The University of
 *      California, San Diego, nor the names of the authors or contributors
 *      may be used to endorse or promote products derived from
 *      this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * ----END-OF-LEGAL-STUFF---- */
#include "BandwidthMap.h"
#include "massert.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include "math.h"
#include "RandomUtil.h"

//  #define COMPILE_MAIN
#define NUM_IN_PEERS 10
#define NUM_OUT_PEERS 10 
#define NUM_IN_PEERS_SLOW 10
#define NUM_OUT_PEERS_SLOW 10
#define HIGH_BANDWIDTH 1200
#define LOW_BANDWIDTH 240
#define MAX_TIME 2000
#define ORACLE_THRESHOLD 500

#define RAWFILE "/usr/research/playground/dkostic/modelnet/samples-XML/full-5000-210-100-0/node_bandwidth-0-1400-100-80-0.50-0.2-0.2"
#define RAWFILE_SMALL "/usr/research/playground/dkostic/modelnet/samples-XML/full-5000-210-100-0/node_bandwidth-0-1400-100-80-0.50-0.2-0.2-save"
#define PRECOMPFILE "/usr/research/playground/dkostic/modelnet/samples-XML/full-5000-210-100-0/node_bandwidth-0-1400-100-80-0.50-0.2-0.2.mesh"

using namespace std;
using std::random_shuffle;

///////////
// ctors //
///////////

// rawfile is input, outputs to precompfile
bandwidth_map::bandwidth_map(const char* rawfile, const char* precompfile, int compute) { 

  parse_rawfile(rawfile);
  if (compute)
    {
      calculate_peers();
      output_precompfile(precompfile);
    }
  else
    {
	parse_precompfile(precompfile);	
    }

}

bandwidth_map::bandwidth_map(const char* rawfile) { 
  parse_rawfile(rawfile);
}

////////////////////
// simple lookups //
////////////////////

int bandwidth_map::in(int node, int time) {
	ASSERT(nodes[node]);
	time_bw_map_t::iterator lit = nodes[node]->lower_bound(time);
	ASSERT(lit != nodes[node]->end());
	return lit->second.in;
}

int bandwidth_map::out(int node, int time) {
	ASSERT(nodes[node]);
	time_bw_map_t::iterator lit = nodes[node]->lower_bound(time);
	ASSERT(lit != nodes[node]->end());
	return lit->second.out;
}

set<int>&  bandwidth_map::get_in_peers(int node, int time) {
	//cout << "get_in_peers called with " << node << " at time " << time << endl;
	ASSERT(nodes[node]);
	time_bw_map_t::iterator lit = nodes[node]->lower_bound(time);
	ASSERT(lit != nodes[node]->end());
	return lit->second.allowedin;
}

set<int>& bandwidth_map::get_out_peers(int node, int time) {
	//cout << "get_out_peers called with " << node << " at time " << time << endl;
	ASSERT(nodes[node]);
	time_bw_map_t::iterator lit = nodes[node]->lower_bound(time);
	ASSERT(lit != nodes[node]->end());
	return lit->second.allowedout;
}

////////////////////////////////
// writing to files / parsing //
////////////////////////////////

void bandwidth_map::parse_rawfile(const char* rawfile) {
	cout << "Parsing rawfile..." << endl;
	ASSERT(rawfile);
	
	// parse file
	ifstream infile(rawfile);
	ASSERT(infile.is_open());

	while(!infile.eof()) {
		// read in this line of data
		int node, time, in, out;
		infile >> node;
		infile >> time;
		infile >> in;
		infile >> out;

		times.insert(time);
		
		// populate data structures keyed by node
		time_bw_map_t* bwmap;
		if(nodes.find(node) == nodes.end()) {
			bwmap = new time_bw_map_t();
		}
		else {
			bwmap = nodes[node];	
		}
		time_bw_map_t::iterator find_it = bwmap->find(time);
		if(find_it != bwmap->end()) bwmap->erase(find_it);
		
		// populate data structures keyed by time
		bw bandwidth(in,out);
		pair<int,bw> pbw(time,bandwidth);
		bwmap->insert(pbw);
		nodes[node] = bwmap;
	}
	infile.close();
	cout << "Parsed rawfile " << rawfile << endl;
}


void bandwidth_map::output_precompfile(const char* precompfile) {
	cerr << "Outputting precompfile..." << endl;
	ASSERT(precompfile);
	
	ofstream outfile(precompfile);
	ASSERT(outfile);

	for(node_hashmap_t::iterator nodeit = nodes.begin(); nodeit != nodes.end(); ++nodeit) {
		int node = nodeit->first;
		time_bw_map_t* bwmap = nodeit->second;
		
		outfile << node << endl; // 1
//  		cout << "node: " << node << endl; 
		outfile << bwmap->size() << endl; // 2
//  		cout << "number of time entries: " << bwmap->size() << endl;
		for(time_bw_map_t::iterator it = bwmap->begin(); it != bwmap->end(); ++it) {
			set<int> &allowedin = it->second.allowedin;
			set<int> &allowedout = it->second.allowedout;
//  			ASSERT(allowedout.size() > 0);
			
			outfile << it->first << endl; // 3
//  			cout << "time: " << it->first << endl;
			
			outfile << allowedin.size() << endl; // 4
//  			cout << "size of allowed in: " << allowedin.size() << endl;
//  			cout << "allowed in: [ ";
			for(set<int>::iterator in_it = allowedin.begin(); in_it != allowedin.end(); ++in_it) {
			  outfile << *in_it << " "; // 5
//  			  cout << *in_it << " ";
			}
			outfile << endl;
//  			cout << "]" << endl;
			outfile << allowedout.size() << endl; // 6
//  			cout << "size of allowed out: " << allowedout.size() << endl;
//  			cout << "allowed out: [ ";
			for(set<int>::iterator out_it = allowedout.begin(); out_it != allowedout.end(); ++out_it) {
				outfile << *out_it << " "; // 7
//  				cout << *out_it << " ";
			}
			outfile << endl;
//  			cout << "]" << endl;
		}
	}	
	outfile.close();
	cerr << "Precompfile outputted to " << precompfile << endl;
}

void bandwidth_map::parse_precompfile(const char* precompfile) {
	cerr << "Parsing precomp file " << endl;
	
	ifstream infile(precompfile);
	ASSERT(infile.is_open());

	while(infile.peek() != -1) {
		int node;
		int bwmap_size;

		infile >> node; // 1
		infile >> bwmap_size; // 2

//  		cout << "node: " << node << endl; 
//  		cout << "number of time entries: " << bwmap_size << endl;
		
		for(int i=0; i<bwmap_size; ++i) {
			int time;
			infile >> time; // 3
		
			times.insert(time);
//  			cout << "time: " << time << endl;
			
			set<int>& inpeers = get_in_peers(node, time);
			int allowedin_size;
			infile >> allowedin_size; // 4

//  			cout << "size of allowed in: " << allowedin_size << endl;
//  			cout << "allowed in: [ ";
			
			for(int j=0; j<allowedin_size; ++j) {
				int inpeer;
				infile >> inpeer; // 5
				inpeers.insert(inpeer);
//  				cout << inpeer << " ";
			}
//  			cout << "]" << endl;
			
			set<int>& outpeers = get_out_peers(node, time);
			int allowedout_size;
			infile >> allowedout_size; // 6
			
//  			cout << "size of allowed out: " << allowedout_size << endl;
//  			cout << "allowed out: [ ";
			
			for(int j=0; j<allowedout_size; ++j) {
				int outpeer;
				infile >> outpeer; // 7
				outpeers.insert(outpeer);
//  				cout << outpeer << " ";
			}
//  			cout << "]" << endl;
		}
		infile.ignore();
		infile.ignore();
	}
	infile.close();
	cerr << "Precomp file parsed " << precompfile << endl;
}

//////////////
// strategy //
//////////////
void permute(vector<int>& vec) {
  for(uint x=0; x<10*vec.size(); x++) {
    int rand1 = RandomUtil::randInt(vec.size());
    int rand2 = RandomUtil::randInt(vec.size());
    int temp = vec[rand1];
    vec[rand1] = vec[rand2];
    vec[rand2] = temp;
  }
}

void bandwidth_map::calculate_peers() {
	cerr << "Calculating peers..." << endl;
#if 1
	//dmk: assuming that for every node we have the same times mentioned in the bandwidth file
	node_hashmap_t::iterator nodeitc = nodes.begin();
	int nodec = nodeitc->first;
	cout << "reference node " << nodec << endl;

	for(set<int>::iterator timeit = times.begin(); timeit != times.end(); ++timeit) {
	  int time = *timeit;
	  cout << "reference time " << time << endl;
	  
	  vector<int> normal_nodes;
	  int counter = 0;
	  multimap<int, int, less<int> > ord_nodes;
	  normal_nodes.resize (nodes.size());

	  for(node_hashmap_t::iterator nodeit = nodes.begin(); nodeit != nodes.end(); ++nodeit, ++counter) {
	  cout << "inserted node " << nodeit->first << endl;

	    normal_nodes[counter]= nodeit->first;
	    ord_nodes.insert(pair<int, int>(0, nodeit->first));
	  }
	  while(ord_nodes.size() > 0) {
	    bool success = false;
	    int val, node;
	    // while nodes still need incoming connections
	    multimap<int, int, less<int> >::iterator i=ord_nodes.begin();
	    val = i->first;
	    node = i->second;
	    permute(normal_nodes);
	    for(uint cnt=0; cnt<normal_nodes.size(); cnt++) {
	      // REB: time of 0 used just to calculate a random, fixed peering
	      if(attempt_to_peer(i->second, normal_nodes[cnt], time)) {
		success = true;
		cout << "Node " << i->second << " connected to " << normal_nodes[cnt] << "time " << time <<endl;
		break;
	      }
	    }

	    ord_nodes.erase(i);
	    if(success)
	      ord_nodes.insert(pair<int, int>(val+1, node));
	    else {
	      cout << "Node " << node << " is done with neighbors:";
	      set<int>& out = get_out_peers(node, time);
	      for(set<int>::iterator it = out.begin(); it!=out.end(); it++) {
		cout << " " << *it;
	      }
	      cout << endl;
	    }
	  }
	}

#else
	for(node_hashmap_t::iterator nodeit = nodes.begin(); nodeit != nodes.end(); ++nodeit) {
	  int node = nodeit->first;

		cerr << "Calculating peers for node " << node << endl;
		for(set<int>::iterator timeit = times.begin(); timeit != times.end(); ++timeit) {
			int time = *timeit;
			int me_fast = 0;
			if (in ( node, time) > ORACLE_THRESHOLD)
			  me_fast = 1;
			// prefer to peer with my previous peers first
#if 0
			if(time > 0) {
				set<int>& previous_outpeers = get_out_peers(node, time-1);
				for(set<int>::iterator it = previous_outpeers.begin(); it != previous_outpeers.end(); ++it) {
					int peernode = *it;
					attempt_to_peer(node, peernode, time);
					set<int>& allowedout = get_out_peers(node, time);
					if((me_fast && allowedout.size() == NUM_OUT_PEERS)||
					   (!me_fast && allowedout.size() == NUM_OUT_PEERS_SLOW))
					  break; // done
				}
			}
#endif		
			// then, try everyone else
			for(node_hashmap_t::iterator it = nodes.begin(); it != nodes.end(); ++it) {
				int peernode = it->first;
				attempt_to_peer(node, peernode, time);
				set<int>& allowedout = get_out_peers(node, time);
				if((me_fast && allowedout.size() == NUM_OUT_PEERS)||
				   (!me_fast && allowedout.size() == NUM_OUT_PEERS_SLOW))
				  break; // done
			}
		}
	}
#endif
}

bool bandwidth_map::attempt_to_peer(int node, int peernode, int time) {
	set<int>& allowedout = get_out_peers(node, time);
	if(peernode != node && allowedout.size() < NUM_OUT_PEERS) {
		if(allow_in(peernode, node, time)) { // try to connect
			allowedout.insert(peernode);
			ASSERT(allowedout.size() <= NUM_OUT_PEERS);
			set<int>& allowedin = get_in_peers(peernode, time);
			allowedin.insert(node);
			ASSERT(allowedin.size() <= NUM_IN_PEERS);
			return true;
		}
	}
	return false;
}

bool bandwidth_map::allow_in(int giver, int getter, int time) {
   int me_fast = 0;
  if (in ( giver, time) > 500)
    me_fast = 1;
 
  set<int>& allowedin = get_in_peers(giver, time);
  if(	(me_fast&&allowedin.size() >= NUM_IN_PEERS) || // i don't have space for this guy at this time
	(!me_fast&&allowedin.size() >= NUM_IN_PEERS_SLOW) ||
	allowedin.find(getter) != allowedin.end()) { // i have already let him in
		return false;
  }
  int fast = 0;
  int slow = 0;
  int consider_fast = 0;
  if (in (getter, time) > 500)
    consider_fast = 1;
  for(set<int>::iterator element = allowedin.begin(); 
      element != allowedin.end(); ++element) 
    {
      int chosen = *element;
      if (out(chosen, time) > 500)
	  fast ++;
      else
	  slow ++;
    } 
#if 0
  if (allowedin.size() > 5 && drand48() > 1/(double)allowedin.size())
    {
      return false;
    }
#endif
#if 1
  if (me_fast)
    {
      if (consider_fast)
	  return true;
//        else  if (!consider_fast && slow==0)
//  	  return true;
    }
  else//I am slow
    {
      if (!consider_fast)
	return true;
    }

#else
  if (me_fast)
    {
      if (consider_fast && fast < NUM_IN_PEERS- 2)
	  return true;
      else  if (!consider_fast && slow<=2)
	  return true;

    }
  else//I am slow
    {
      if (consider_fast && fast <= 2)
	return true;
      else  if (!consider_fast && slow < NUM_IN_PEERS- 2)
	return true;
    }
#endif
  return false;
}

///////////
// print //
///////////

void bw::print() {
	cout << "Allowedin: [ ";
	for(set<int>::iterator it = allowedin.begin(); it != allowedin.end(); ++it) {
		cout << *it << " ";
	}
	cout << "]" << endl;
	
	cout << "Allowedout: [ ";
	for(set<int>::iterator it = allowedout.begin(); it != allowedout.end(); ++it) {
		cout << *it << " ";
	}
	cout << "]" << endl;
}

void bandwidth_map::print() {

	for(node_hashmap_t::iterator it = nodes.begin(); it != nodes.end(); ++it) {
		cout << "For node " << it->first << ":" << endl;
		time_bw_map_t* bwmap = it->second;
		for(time_bw_map_t::iterator bit = bwmap->begin(); bit != bwmap->end(); ++bit) {
			cout << "Time " << bit->first << ":" << endl; 
			bit->second.print();
		}
	}
}


#ifdef COMPILE_MAIN

int main(int argc, char** argv) {
	ASSERT(argc == 2);
	bandwidth_map bwmap(RAWFILE, PRECOMPFILE, atoi(argv[1]));

	return 0;
}

#endif


