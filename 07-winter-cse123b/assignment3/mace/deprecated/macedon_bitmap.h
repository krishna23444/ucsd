#ifndef __macedon_bitmap
#define __macedon_bitmap
#include <list>
#include <vector>
#include <set>

using namespace std;

const static int BITMAP_SIZE = 1024*22;

typedef struct bitmapy {
  int tab_size;
  int start_bit;
  int start_seq;
  int last_seq;
  unsigned char tab[BITMAP_SIZE/8];
} flat_bitmap;

class macedon_bitmap
{	
public:
  macedon_bitmap();
  ~macedon_bitmap();
  bool contains (int item);
  int insert (int item, unsigned char *message=0, int size=0);
  void remove (int item);
  unsigned char *get_message(int item, int &size);
  void get_modulo_keys (list < int> & result, macedon_bitmap &src, int &low, int &high, int &mod, int &mod_max);
  int get_last();
  int get_earliest();
  flat_bitmap bm;
  set<int, less<int> > contents;
private:
  unsigned char *cache[BITMAP_SIZE];
  int cache_size[BITMAP_SIZE];
  int offset (int seq);
  void set_bit (int seq, int set_value);
};
#endif //__macedon_bitmap
