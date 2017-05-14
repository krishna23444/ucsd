#ifndef TABLE_H__
#define TABLE_H__

class BitMap;
class Lock;
class Thread;

/**
 * Thread-safe table
 */
class Table {
 public:
  /* Create a table to hold at most "size" entries. */
  Table(int size, bool startAtZero); 

  ~Table();

  /* Allocate a table slot for "object", returning the 
   "index" of the allocated entry; otherwise, return -1
    if no free slots are available. */
  int Alloc();

  /* Retrieve the object from table slot at "index", or NULL
     if that slot has not been allocated. */
  Thread * GetThread(int index);
  int GetIntValue(int index);
  

  void SetThread(int index, Thread * in);
  void SetIntValue(int index, int value);
  bool isAllocated(int index);
  
  void Clear(int index);

  /* Free the table slot at index. */
  void Release(int index);

 private:
  int maxSize;
  Lock* writeLock;
  BitMap* bitmap;
  int* intArray;
  Thread** threadArray;
};

#endif /* TABLE_H__ */
