/* 
 * AdolfoFilter.h : part of the Mace toolkit for building distributed systems
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
#include "GenericFilter.h"
#include <list>

#ifndef __adolfo_filter
#define __adolfo_filter
class adolfo_filter:public GenericFilter 
{
 
 public:

  static const int HISTORY_VALUE=4000;
  static const int BANDWIDTH_WINDOW_SBITS=2; //how long to keep average (seconds) in log of 2: 2^2 = 4 seconds
  static const uint64_t BANDWIDTH_WINDOW= (1<<BANDWIDTH_WINDOW_SBITS) * 1000*1000; // how long to keep average in micro-seconds

  adolfo_filter	();
  ~adolfo_filter	( );
  void  update ();
  void remove_entry(int);
  void  clear ();
  double getValue();
  void initialize(); // REB: replaces constructor initialization
 public:
 protected:
  int  lock ();
  void  unlock ();
  void check ();
  std::list < uint64_t > history; 
  int data_packet_size;
};
#endif //__adolfo_filter
