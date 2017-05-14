/* 
 * candidate_set.h : part of the Mace toolkit for building distributed systems
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
#include "Scheduler.h"
#include "RandomUtil.h"

#ifndef __candidate_set
#define __candidate_set
template <class T>
class candidate_set
{
public:
  static const int LAST_MAX_CANDIDATES = 10; 
  candidate_set<T>(int size = LAST_MAX_CANDIDATES)
    : number_candidates (0), max_size(size){}

  int addj( T from)
  {
    
    int i;
    
    for(i=0; i<number_candidates; i++) {
      if (candidates[i] == from) {
	return 0;
      }
    }
    
    if (number_candidates < max_size) {
	candidates [number_candidates++]= from;
    }
    else {
      printf("Exception: adding too many candidates (%d >= %d)!\n", number_candidates, max_size);
      exit(5);
    }
  }
  
  int add_unconditionally( T from)
  {
    
    int i;
    
    for(i=0; i<number_candidates; i++) {
      if (candidates[i] == from) {
	return 0;
      }
    }
    
    if (number_candidates < LAST_MAX_CANDIDATES ) {
      candidates [number_candidates++]= from;
    }
  }
  
  T throw_one_out()
  {
    int i;
    T which;
    int pick = RandomUtil::randInt(number_candidates);
    
    if (number_candidates < 1) {
      printf("Exception: too few candidates to throw any out %d\n", number_candidates);
      exit(23);
    }
    
    which = candidates[pick];
    if (pick < number_candidates-1) {
      for(i=pick; i<number_candidates-1; i++) {
	candidates[i] = candidates[i+1];
      }
    }
    else if (pick > number_candidates-1) {
      printf("Exception: something is wrong with your random number generation dude %d %d\n", pick, number_candidates);
      exit(5);
    }
    number_candidates--;
    return which;
  }
  
  void clear() {}

  void dumpem(char *buf)
  {
    int i;
    
    for(i=0; i<number_candidates; i++) {
      sprintf(buf, "%x ", candidates[i]);
      buf = buf + strlen(buf);
    }
    return;
  }
 

  void printem(int addr)
  {
    int i;
    
    for(i=0; i<number_candidates; i++) {
      printf("Node %x (%f): candidate[%d] = %x\n",          
	     addr, Scheduler::instance().clock(), i, candidates[i]);
    }
    
    return;
  }
 

  void compact(candidate_set which, int which_represents, int sofar)
  {
    int total = number_candidates + which.number_candidates;
    double myrand;
    candidate_set<T> temp(max_size);
    
    if (sofar < number_candidates) {
      sofar = number_candidates;
    }
    
    if (which_represents < which.number_candidates) {
      which_represents = which.number_candidates;
    }
    
    int accum = 0;
    T toadd;
    double baseweight = ((double)sofar);
    double whichweight = ((double)which_represents);
    if (total > max_size) {
      total = max_size;
    }
    while (accum < total) {
      if ( (which.number_candidates < 1) ||
	   ( (number_candidates > 0 ) &&
	     ((myrand = drand48()) < baseweight/(baseweight+whichweight)))) {
	if (number_candidates < 1) {
	  printf("Exception: houston we have a problem with base %d %d %d %d %d\n", accum, total, max_size, number_candidates, which.number_candidates);
	  exit(51);
	}
	toadd = throw_one_out();
	baseweight=baseweight-1;
      }
      else {
	if (which.number_candidates < 1) {
	  printf("Exception: houston we have a problem with which %d %d %d  %d %d\n", accum, total, max_size, number_candidates, which.number_candidates);
	  exit(51);
	}
	toadd = which.throw_one_out();
	whichweight = whichweight-1;
      }
      temp.addj(toadd);
      accum++;
    }
    
    while (number_candidates > 0) {
      throw_one_out();
    }
    
    while (temp.number_candidates > 0) {
      toadd = temp.throw_one_out();
      addj(toadd);
    }
    
  }
  
  int max_size;
  int number_candidates;
  T candidates[LAST_MAX_CANDIDATES];
};	

#endif //__candidate_set
