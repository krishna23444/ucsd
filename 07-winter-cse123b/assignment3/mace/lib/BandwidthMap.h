/* 
 * BandwidthMap.h : part of the Mace toolkit for building distributed systems
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
#include <map>
#include <set>

#if __GNUC__ >= 3
#include <ext/hash_map>
#else
#include <hash_map>
#endif

#ifndef __bandwidth_map
#define __bandwidth_map

// represents a single data point
struct bw {
	bw(int in, int out) { this->in = in; this->out = out; }
	void print();
	int in;
	int out;
	std::set<int>  allowedin;
	std::set<int> allowedout;
};

struct bw_time_compare {
	bool operator()(const int& t1, const int& t2) {
		return t2 < t1; // reverse order
	}
};

typedef std::map<int,bw,bw_time_compare> time_bw_map_t;
typedef __gnu_cxx::hash_map<int,time_bw_map_t*> node_hashmap_t;


class bandwidth_map {
  public:
	 
	bandwidth_map(const char* rawfile, const char* precompfile, int compute); 
	bandwidth_map(const char* rawfile); 

	bool attempt_to_peer(int node, int peer, int time);
	void calculate_peers();
	void print();	
	void parse_rawfile(const char* rawfile);
	void output_precompfile(const char* precompfile);
	void parse_precompfile(const char* precompfile);
	int in(int node, int time);
	int out(int node, int time);

	void get_in_peers(int node, int time, std::set<int>& inpeers);
	void get_out_peers(int node, int time, std::set<int>& outpeers);
	std::set<int>& get_in_peers(int node, int time);
	std::set<int>& get_out_peers(int node, int time);	
	bool allow_in(int giver, int getter, int time);
	
	// node -->[time --> bw]
        __gnu_cxx::hash_map<int,std::map<int,bw,bw_time_compare>*> nodes;
	std::set<int> times; // stores unique times, just so we can iterate conveniently
	
};

#endif
