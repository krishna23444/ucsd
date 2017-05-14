/* 
 * AdolfoFilter.cc : part of the Mace toolkit for building distributed systems
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
#include "Util.h"
#include "AdolfoFilter.h"
// #include <stdio.h>
// #include "Scheduler.h"
// #include "mace.h"
#include "Params.h"

using std::list;

//const int adolfo_filter::HISTORY_VALUE = 4000;
adolfo_filter::adolfo_filter( )
{
  data_packet_size = Params::Instance()->getInt("data_packet_size");
  initialize();
}

adolfo_filter::~adolfo_filter()
{
}

void adolfo_filter::initialize()
{
  value = 0.0;
}

// ---------------------------------------------- 
// getValue
// ---------------------------------------------- 

double  adolfo_filter::getValue ()
{
  check();
  return value;
}


// ---------------------------------------------- 
// clear
// ---------------------------------------------- 

void  adolfo_filter::clear ()
{
  history.clear ();
  value = 0.0;
}


// ---------------------------------------------- 
// update
// ---------------------------------------------- 

void  adolfo_filter::update ()
{
  history.push_back (Util::timeu());
  //   check();
}



// ---------------------------------------------- 
// check
// ---------------------------------------------- 

void adolfo_filter::check ()
{
  uint64_t end = Util::timeu() - BANDWIDTH_WINDOW;

  while (history.size() > 0 && (history.front())< end) {
    history.pop_front();
  }      
  
  if (history.size() == 0 ) {
    value = 0.0;
  }
  else {
    value = (double)(((data_packet_size*history.size())>>(BANDWIDTH_WINDOW_SBITS))<<3); //-3 to multiply by 8 for the number of bits per byte
  }
}


