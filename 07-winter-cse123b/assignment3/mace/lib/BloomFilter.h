/* 
 * BloomFilter.h : part of the Mace toolkit for building distributed systems
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
#include <list>
#include "HashFunction.h"
#include <queue>

#if __GNUC__ >= 3
#include <ext/hash_set>
#else
#include <hash_set>
#endif
#include "Digest.h"

#ifndef __bloom_filter
#define __bloom_filter
const static uint BLOOM_SIZE = 1024*22;
const static uint BLOOM_SIZE_IN_BYTES = BLOOM_SIZE/8;
const static uint BLOOM_SIZE_SMALL = 3216;//2048; //1024*22/8;
const static uint BLOOM_SIZE_IN_BYTES_SMALL = BLOOM_SIZE_SMALL/8;

typedef struct bloomy {
  int tab_size;
  unsigned char tab[BLOOM_SIZE_IN_BYTES];
} flat_bloom;

typedef struct bloomy_small {
  int tab_size;
  unsigned char tab[BLOOM_SIZE_IN_BYTES_SMALL];
} flat_bloom_small;

class  heap_key {
public:

 heap_key (int s = -1)
 : key (s) {}
 
 bool operator< (const heap_key & right) const {
  return key > right.key; 
 }
 int key;
};

class bloom_filter  : public digest
{
struct equal_int2
{
  bool operator()(int  i1, int i2) const
  {
    return i1 == i2;
  }
};

	
 public:
  static const int MAXIMUM_PRECOMPUTED = 50000;
  //look at  http://www.cs.wisc.edu/~cao/papers/summary-cache/node8.html
  const static int OPTIMAL_RATIO =    10;
  const static int NUMBER_OF_FUNCTIONS = 4;
  bloom_filter	( int size = BLOOM_SIZE, int count_functions =
		  NUMBER_OF_FUNCTIONS, int ratio = OPTIMAL_RATIO );
  bloom_filter(const bloom_filter&  from);
  bloom_filter&  operator=(const bloom_filter&  from);
  ~bloom_filter	( );
  bool  contains (int item);
  //returns the lowest sequence number remaining in the filter even after truncation
  int  insert (int item, int suppress = 1);
  void  add_default_functions (int count_functions);

  void  add_function (hash_function* function);
  void  dump_state ();
  void   serialize (unsigned char* buffer);
  void  import (unsigned char* buffer);
  int  size_compacted_in_bytes ();
  /*   int  reset (); */
  void  reset ();
  int  get_items ();
  int  get_lowest_sequence ();
  bool  _contains (int item);
void  prune_up_to (int key);

 protected:
   void initialize( int size, int count_functions);
 void  copy_from(const bloom_filter&  from); 
  void  _remove (int item); // does NOT remove from the heap!

  int * table;
  std::list < hash_function* > functions; 
  __gnu_cxx::hash_set < int, __gnu_cxx::hash < int >, equal_int2> contents;
  std::priority_queue < heap_key > heap; 
  void  cleanup ();

  uint table_size;
  static int precomputed;
  static int precomputed_table [bloom_filter::MAXIMUM_PRECOMPUTED][4];
  int disable_precomputed;
  int optimal_ratio;
 private:
};
#endif //__bloom_filter
