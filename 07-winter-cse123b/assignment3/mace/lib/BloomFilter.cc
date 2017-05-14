/* 
 * BloomFilter.cc : part of the Mace toolkit for building distributed systems
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
#include "BloomFilter.h"
#include "MultiplicationHashFunction.h"
#include "stdio.h"
#include "math.h"

using std::list;
using std::priority_queue;

void debug (int level, char *format, ... )     ;
//we need reverse logic, since the priority queue is geared toward the maximum element
bool compare_integers (int first,int second)
{
  return first > second;
};

double factors[4] = {(sqrt(5)-1)/2.0,
  (sqrt(5)-1)/2.05,
  (sqrt(5)-1)/2.1,
  (sqrt(5)-1)/1.90};

//speed up computation, faster than func calls
#define USE_HASH_DIRECT 1
//  #define USE_HASH_INSERT 1
//precomputed values  make us pay the computation cost up front, not
//in the middle of the run,  when cpu might be scarce. of
int bloom_filter::precomputed = 0;
int bloom_filter::precomputed_table [bloom_filter::MAXIMUM_PRECOMPUTED][4];

bloom_filter::bloom_filter(int size,int count_functions, int ratio)
  : 
table(NULL), table_size (size), disable_precomputed (0), optimal_ratio( ratio)
{
  debug(0, "bloom_filter::bloom_filter: %d %d %d\n", table_size, count_functions, ratio);

  //    printf("table_size: %d\n", table_size);
  if (table_size % 8 != 0)
  {
    printf("table_size is not on multiple of 8, exiting: %d\n", table_size);
    exit(50);
  }
  initialize(size, count_functions);
}

void bloom_filter:: initialize(int size,int count_functions)
{
  table_size = size;
  table = new int [table_size+1];
  //    debug(0, "bloom_filter allocated table: %x %d\n", table, table_size);

  if (table_size != BLOOM_SIZE)
  {
    disable_precomputed = 1;
  }

#if !USE_HASH_DIRECT
  add_default_functions(count_functions);
#endif
  if (!precomputed)
  {
    add_default_functions(count_functions);

    for (int i = 0; i < bloom_filter::MAXIMUM_PRECOMPUTED ;i++ )
    {
      list<hash_function*>::iterator traverse; 
      int counter = 0;
      for(traverse=functions.begin();traverse!=functions.end();traverse++)
      {
        precomputed_table [i][counter] = (*traverse)->hash(i,  table_size);
        counter ++;
      }	
    }
    precomputed = 1;
  }
  reset ();
}
bloom_filter& bloom_filter::operator=(const bloom_filter&  from)
{
  //    printf("bloom_filter::bloom_filter: operator= %d\n", table_size);
  if (this != &from)
  {
    cleanup();
    copy_from( from);
  }
  return *this;
}
  bloom_filter::bloom_filter(const bloom_filter&  from)
:   table(NULL), disable_precomputed (0)

{
  //    printf("bloom_filter::bloom_filter: copy constructor %d\n", table_size);
  initialize(from.table_size, from.functions.size());
  copy_from( from);
}

void bloom_filter::copy_from(const bloom_filter&  from)
{
  //    printf("bloom_filter::bloom_filter: copy constructor %d\n", table_size);
  if ( table_size !=  from.table_size)
  {
    disable_precomputed = 1;
  }

  table_size=from.table_size;

  table = new int [table_size+1];
  contents = from.contents;
  heap = from.heap;
  for (uint i = 0; i < table_size ;i++ )
  {
    table[i]=from.table[i];
  }
}

bloom_filter::~bloom_filter()
{
  cleanup();
  list<hash_function*>::iterator traverse;  
  for(traverse=functions.begin();traverse!=functions.end();traverse++)
  {
    hash_function* f = *traverse;
    delete f; 
  }
}
// ---------------------------------------------- 
// contains
// ---------------------------------------------- 

bool  bloom_filter::contains (int item)
{
#if !USE_HASH_DIRECT
  list<hash_function*>::iterator traverse; 
  int counter = 0; 
  for(traverse=functions.begin();traverse!=functions.end();traverse++)
  {
    hash_function* function =*traverse;
    int index = -1;
    if (precomputed&&
        !disable_precomputed&&
        item < bloom_filter::MAXIMUM_PRECOMPUTED)
    {
      index =  precomputed_table [item][counter];
    }
    else
    {
      index = function->hash(item, table_size);
    }

    if (table[index]==0)
    {
      return false;
    }
    counter ++;
  }
#else

  /*   int value = 0; */
  /*   double factor = 0; */
  double intermediate = 0;
  for (int counter = 0; counter < 4 ;counter++ )
  {
    int index = -1;
    if (precomputed&&
        !disable_precomputed&&
        item < bloom_filter::MAXIMUM_PRECOMPUTED)
    {
      index =  precomputed_table [item][counter];
    }
    else
    {
      intermediate =item*factors[counter];
      intermediate -= floor( intermediate);
      index = (int)( table_size*intermediate);
    }
    if (table[index]==0)
    {
      return false;
    }
  }

#endif
  return true;
}

// ---------------------------------------------- 
// insert
// ---------------------------------------------- 

int  bloom_filter::insert (int item, int suppress)
{

  //    debug(1,"%x bloom_filter::insert %d at the size %d\n", this, item, get_items());
  heap_key top_key;
#if USE_HASH_INSERT
  if (_contains(item))
  {
    debug(0,"%x bloom_filter::insert %d contains\n", this, item);
    top_key= heap.top();
    return top_key.key;
  }
#endif	
#if !USE_HASH_DIRECT
  list<hash_function*>::iterator traverse;  
  int counter = 0;
  for(traverse=functions.begin();traverse!=functions.end();traverse++)
  {
    hash_function* function =*traverse;

    int index = -1;
    if (precomputed&&
        !disable_precomputed&&
        item < bloom_filter::MAXIMUM_PRECOMPUTED)
    {
      index =  precomputed_table [item][counter];
    }
    else
    {
      index = function->hash(item, table_size);
    }

    table[index] ++; 
    counter ++;
  }
#else
  int value = 0;
  /*   double factor = 0; */
  double intermediate = 0;
  for (int counter = 0; counter < 4 ;counter++ )
  {
    uint index = 0;
    if (precomputed&&
        !disable_precomputed&&
        item < bloom_filter::MAXIMUM_PRECOMPUTED)
    {
      index =  precomputed_table [item][counter];
    }
    else
    {
      intermediate =item*factors[counter];
      intermediate -= floor( intermediate);
      index = (int)( table_size*intermediate);
    }

    if (index > table_size) //dmk
    {
      debug(0, "index: %d\n", index);
      abort();
    }

    table[index] ++;
  }

#endif
#if USE_HASH_INSERT
  contents.insert( item);
#endif	
  heap.push( heap_key(item));

#if 1

  if (!suppress && heap.size()>= (double) (table_size/(double)optimal_ratio))
  {
    top_key= heap.top();
    heap.pop();
    value = top_key.key;
    _remove( value);
    debug(1,"dropping filter value %d at the size %d\n", value, get_items());
  }
#endif


  top_key= heap.top();
  return top_key.key;
}

// ---------------------------------------------- 
// _remove
// ---------------------------------------------- 

void  bloom_filter::_remove (int item)
{
  //    debug(2,"%x bloom_filter::remove %d at the size %d\n", this, item, get_items());
#if USE_HASH_INSERT
  if (!_contains(item))
  {
    debug(1,"%x bloom_filter::remove %d !contains\n", this, item);
    return;
  }
#endif	
#if !USE_HASH_DIRECT
  list<hash_function*>::iterator traverse;  
  int counter = 0;
  for(traverse=functions.begin();traverse!=functions.end();traverse++)
  {
    hash_function* function =*traverse;

    int index = -1;
    if (precomputed&&
        !disable_precomputed&&
        item < bloom_filter::MAXIMUM_PRECOMPUTED)
    {
      index =  precomputed_table [item][counter];
    }
    else
    {
      index = function->hash(item, table_size);
    }

    table[index] --; 
    if ( table[index]< 0)
    {
      debug(0, "Bloom filters entry negative while inserting %d\n", 
          item);
      abort();
      //  	  exit( 61);
    }
    counter ++;
  }
#else
  /*   int value = 0; */
  /*   double factor = 0; */
  double intermediate = 0;
  for (int counter = 0; counter < 4 ;counter++ )
  {
    int index = -1;
    if (precomputed&&
        !disable_precomputed&&
        item < bloom_filter::MAXIMUM_PRECOMPUTED)
    {
      index =  precomputed_table [item][counter];
    }
    else
    {
      intermediate =item*factors[counter];
      intermediate -= floor( intermediate);
      index = (int)( table_size*intermediate);
    }
    table[index] --;
    if ( table[index]< 0)
    {
      debug(1, "Bloom filters entry negative while inserting %d\n", 
          item);
      abort();

      //  	  exit( 61);
    }
  }

#endif
#if USE_HASH_INSERT
  contents.erase(item);
#endif	

}
// ---------------------------------------------- 
// _contains
// ---------------------------------------------- 

bool  bloom_filter::_contains (int item)
{
  __gnu_cxx::hash_set<int, __gnu_cxx::hash< int>,  equal_int2>::const_iterator   element
    = contents.find( item);
  if (element !=  contents.end())
  {
    return true;
  }

  return false;
}

// ---------------------------------------------- 
// add_function
// ---------------------------------------------- 

void  bloom_filter::add_function (hash_function* function)
{
  functions.push_front(function);
}

// ---------------------------------------------- 
// add_default_functions
// ---------------------------------------------- 

void  bloom_filter::add_default_functions (int count_functions)
{
#if 1

  //plug-in some multiplicative factors based on Knuth pretty good value  //of (sqrt(5)-1)/2.0

  multiplication_hash_function* f;
  f = new multiplication_hash_function ((sqrt(5)-1)/2.0);
  add_function(f);

  f = new multiplication_hash_function ((sqrt(5)-1)/2.05);
  add_function(f);

  f = new multiplication_hash_function ((sqrt(5)-1)/2.1);
  add_function(f);
  f = new multiplication_hash_function ((sqrt(5)-1)/1.90);
  add_function(f);

  return;
#else
  for (int i = 0; i <  count_functions ;i++ )
  {
    double index =i+1;
    index/=(double)  count_functions+1;
    //  	multiplication_hash_function* f = new multiplication_hash_function ((int)((( (double)i+1)/(double)  count_functions)*count_functions));
    multiplication_hash_function* f = new multiplication_hash_function (index);
    add_function(f);
  }
#endif
}

// ---------------------------------------------- 
// dump_state
// ---------------------------------------------- 

void  bloom_filter::dump_state ()
{
  for (uint i = 0; i < table_size ;i++ )
  {
    if (((i) % 8) == 0)
    {
      printf(" ");
    }
    printf("%d",table[i]!=0);

  }

  printf("\n");

}

// ---------------------------------------------- 
// cleanup
// ---------------------------------------------- 

void  bloom_filter::cleanup ()
{
  if (table != NULL)
  {
    delete [] table;
    table = NULL;
  }

}

// ---------------------------------------------- 
// serialize
// ---------------------------------------------- 

void  bloom_filter::serialize (unsigned char* buffer)
{
  unsigned int*size_pointer = (unsigned int*)buffer;
  *size_pointer = table_size;
  unsigned char* iterator =(unsigned char*)++size_pointer;
  unsigned char value = 0;
  int counter = 0;
  for (uint i = 0; i < table_size ;i++ )
  {
    //we care if the field is only > 0
    value |= ((table[i]!=0)*0x80);
    if (counter == 7)
    {
      *iterator++ = value;
      //  	  printf("svalue: %x %d\n", value, (unsigned int) iterator - (unsigned int)buffer);

      counter = 0;
      value = 0;
    }
    else
    {
      counter++ ;
      value=value>>1;
    }
  }

  //    dump_state ();
}

// ---------------------------------------------- 
// import
// ---------------------------------------------- 

void  bloom_filter::import (unsigned char* buffer)
{
  //    
  unsigned int*size_pointer =  (unsigned int*)buffer;

  if (*size_pointer != table_size)
  {
    disable_precomputed = 1;
    printf("bloom_filter::import changing size from %d to %d \n", table_size, *size_pointer);

    table_size = *size_pointer;
    if (table != NULL)// && *size_pointer > table_size)
    {
      cleanup();
      table = new int [table_size+1];
      printf("allocated table: %p size %d\n", table, table_size);
    }
  }  
  reset();

  unsigned char* iterator =(unsigned char*)++size_pointer;
  unsigned char value = 0;
  uint seek = 0;
  /*   int counter = 0; */
  while (seek < table_size )
  {
    if ((seek % 8) == 0)
    {
      value=  *iterator++ ;
      //  	  printf("ivalue: %x %d\n", value, (unsigned int) iterator - (unsigned int)buffer);
    }
    int bit = (value&0x01);
    table[seek]= bit;
    value= value>>1;
    seek++;
  }

}

// ---------------------------------------------- 
// size_compacted_in_bytes 
// ---------------------------------------------- 

int  bloom_filter::size_compacted_in_bytes ()
{
  return sizeof( int)+ ( int)ceil(table_size/8);
}

// ---------------------------------------------- 
// get_lowest_sequence
// ---------------------------------------------- 

int  bloom_filter::get_lowest_sequence ()
{
  if (get_items() == 0)
  {
    return 0; 
  }

  heap_key top_key;
  top_key= heap.top();
  return top_key.key;
}

// ---------------------------------------------- 
// reset
// ---------------------------------------------- 

void  bloom_filter::reset ()
{
  //    bzero(table, sizeof( table));
  for (uint i = 0; i < table_size ;i++ )
  {
    table[i]=0;
  }
#if USE_HASH_INSERT
  contents.clear();
#endif	
  while (heap. size ())
  {
    heap. pop ();
  }
}

// ---------------------------------------------- 
// get_items
// ---------------------------------------------- 

int  bloom_filter::get_items ()
{
  return heap.size();
}

// ---------------------------------------------- 
// prune_up_to
// ---------------------------------------------- 

void  bloom_filter::prune_up_to (int max_key)
{
  if (get_items() == 0)
  {
    return;
  }

  heap_key top_key;
  top_key= heap.top();
  while (top_key.key < max_key)
  {
    heap.pop();
    //        debug(1,"pruning filter value %d at the size %d\n", top_key.key , get_items());
    _remove( top_key.key );
    if (get_items() == 0)
    {
      break;
    }

    top_key= heap.top(); 
  }

}
