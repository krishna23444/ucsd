#include "macedon_bitmap.h"
#include "macedon.h"
#include "stdio.h"
#include "math.h"

macedon_bitmap::macedon_bitmap()
{
  bm.tab_size = BITMAP_SIZE;
  bm.start_bit = 0;
  bm.start_seq = 0;
  bm.last_seq = 0;
  for (int i=0; i<bm.tab_size; i++) {
    set_bit(i, 0);
  }
}

macedon_bitmap::~macedon_bitmap()
{
}

int macedon_bitmap::offset(int seq)
{
  if (seq < bm.start_seq)
    return -1;
  else if (seq-bm.start_seq > BITMAP_SIZE)
    return -1;
  else {
    int starter = bm.start_bit / 8;
    int this_one = (seq-bm.start_seq) / 8;
    return (starter+this_one)%(BITMAP_SIZE/8);
  }
}

void macedon_bitmap::set_bit(int seq, int set_value) 
{
  unsigned char value = 0x01;
  int which_bit = 7 - ((seq-bm.start_seq) % 8);
  value = value<<which_bit;
  if (set_value) {
    //      printf("%d inserted into bitmap\n", seq);
      bm.tab[offset(seq)] = (bm.tab[offset(seq)]|value);
    }
  else {
    bm.tab[offset(seq)] = (bm.tab[offset(seq)]&(~value));
  }
}

bool macedon_bitmap::contains(int seq) 
{
  unsigned char value = bm.tab[offset(seq)];
  int which_bit = 7 - ((seq-bm.start_seq) % 8);
  //  printf("contains %d %d %d %x %x\n", seq, offset(seq), bm.start_seq, value, value>>which_bit);
  value = value>>which_bit;
  return (value&0x01);
}

unsigned char *macedon_bitmap::get_message(int item, int &size)
{
  unsigned char *temp;
  if (contains(item)) {
    int cache_index = item % BITMAP_SIZE;
    size = cache_size[cache_index];
    temp = (unsigned char *)malloc (size);
    bcopy(cache[cache_index], temp, size);
    return temp;
  }
  else
    return 0;
}

int macedon_bitmap::insert (int item, unsigned char *message, int size)
{
  if (contains(item)) {
    //    printf("%d already in bitmap\n", item);
    return 0;
  }
  if (item > bm.last_seq)
    bm.last_seq = item;
  if (bm.last_seq-bm.start_seq > BITMAP_SIZE) {
    int old_start = bm.start_seq;
    bm.start_seq = bm.last_seq-BITMAP_SIZE;
    bm.start_bit = (bm.start_bit+bm.start_seq-old_start)%BITMAP_SIZE;
  }
  contents.insert(item);
  if (message) {
    int cache_index = item % BITMAP_SIZE;
    cache[cache_index] = (unsigned char *)malloc (size);
    bcopy(message, cache[cache_index], size);
    cache_size[cache_index] = size;
  }
  if (contents.size()>BITMAP_SIZE) {
    set<int, less<int> >::const_iterator element
      = contents.begin();
    contents. erase(*element);
    int cache_index = *element % BITMAP_SIZE;
    ::free(cache[cache_index]);
    cache[cache_index]=0;
    cache_size[cache_index]=0;
  }
  set_bit(item, 1);
  return 1;
}

int macedon_bitmap::get_earliest()
{
  return bm.start_seq;
}

int macedon_bitmap::get_last()
{
  return bm.last_seq;
}

void macedon_bitmap::get_modulo_keys (list <int> & result,
				     macedon_bitmap& src, int &low, int &high, 
				     int &mod, int &mod_max)
{
  int max_stories=2000;
  int story[max_stories];
  int num_stories=0;
  int chosen;
  int last = -1;
  
  set<int, less<int> >::const_iterator element;  
  for(element=contents.begin();element!=contents.end();element++) {
    int value =*element;
    if (!src.contains(value) && 
	  //	  value < high && 
	  value > low && 
	  mod_max !=0 && 
	  ((value % mod_max) == mod) ) {
      if (num_stories<max_stories) {
	story[num_stories] = value;
	num_stories++;
      }
      else {
	chosen = random_integer(num_stories);
	story[chosen] = value;
      }
    }
  }
  int back = 99999;
  while (num_stories && back >0) {
    chosen = random_integer(num_stories);
    result.push_back(story[chosen]);
    if (chosen != num_stories-1)
      story[chosen] = story[num_stories-1];
    num_stories--;
    back--;
  }
  
  return;
}
