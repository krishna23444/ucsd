/* 
 * BitmapDigest.h : part of the Mace toolkit for building distributed systems
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
#include "Digest.h"

#ifndef __bitmap_digest
#define __bitmap_digest
//const static int BITMAP_SIZE = 1024*22;
const static int BITMAP_SIZE = 1024*6; //was *4

typedef struct bitmapy {
  int tab_size;
  int start_bit;
  int start_seq;
  int last_seq;
#if VARIABLE_BITMAP
  unsigned char* tab;
  bitmapy():tab( 0) {}
  int  size_core_in_bytes (){sizeof(*this)-sizeof(unsigned char* );};
#else
  unsigned char tab[BITMAP_SIZE/8]; 
#endif
} flat_bitmap;

class bitmap_digest : public digest
	{
	
public:
 bitmap_digest	(int size = BITMAP_SIZE);
~bitmap_digest	( );
public:
  bool  contains (int item);
  //insert returns the lowest sequence number remaining in the  digest even after truncation
  int  insert (int item, int suppress = 1);
  //returns 1 if it actually cleared the bit (i.e. it was in range)
  int  remove_item (int item);
  void   serialize (unsigned char* buffer);
  void  import (unsigned char* buffer);
  int  size_compacted_in_bytes ();
  /*   int  reset (); */
  void  reset ();
  int  get_lowest_sequence ();
  int  get_highest_sequence ();

  flat_bitmap bm;
 protected:
  int in_table (int seq);
  int  which_byte  (int seq);
  int  which_bit  (int seq);
  void set_bit (int seq, int set_value);

 private:
 	};
#endif //__bitmap_digest
