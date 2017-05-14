/* 
 * BandwidthFilter.cc : part of the Mace toolkit for building distributed systems
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
#include "BandwidthFilter.h"
#include "Util.h"

using std::list;

BandwidthFilter::BandwidthFilter() {
  history_count =0;
  value = 0.0;
  time_initial = Util::timeu();
}

BandwidthFilter::~BandwidthFilter() {
}

// ---------------------------------------------- 
// getValue
// ---------------------------------------------- 

double BandwidthFilter::getValue() {
  check();
  double result = value;
  return  result;
}


// ---------------------------------------------- 
// clear
// ---------------------------------------------- 

void BandwidthFilter::clear() {
  history.clear ();
  value = 0.0;
}


// ---------------------------------------------- 
// update
// ---------------------------------------------- 

void BandwidthFilter::update(int size) {
  uint64_t now = Util::timeu();

  history.push_back(BandwidthPair (size,now));
  history_count++;
  check();
}



// ---------------------------------------------- 
// check
// ---------------------------------------------- 

void BandwidthFilter::check() {
  uint64_t elapsed_time;
  uint64_t now = Util::timeu();

  while (history_count && (now - history.front().when) > BANDWIDTH_WINDOW) {
    history.pop_front();
    history_count--;
  }      
  
  if (history_count == 0) {
    value = 0.0;
  }
  else {
    if (now - time_initial < BANDWIDTH_WINDOW)
      elapsed_time = now - time_initial;
    else
      elapsed_time = BANDWIDTH_WINDOW;
    int total_bytes = 0;
    for(list<BandwidthPair>::iterator it = history.begin();
	it != history.end(); ++it) {
      BandwidthPair r=*it;
      total_bytes+=r.size;
    }
    value = total_bytes*8/((double)elapsed_time/1000000);
  }
}


