#ifndef BOUNDEDBUFFER_H_
#define BOUNDEDBUFFER_H_
#include "syscall.h"

class Semaphore;
class Thread;

class BoundedBuffer
{
  public:
    BoundedBuffer(int bufferSize = DEFAULT_BOUNDED_BUFFER_SIZE);
    ~BoundedBuffer();

    int write(char* message, int size);
    
    int read(int size, char* message);
    
    void readerComes();
    void readerNotComing();
    
    void makePipe(); // Reader matching with a writer
    void breakPipe(); // When either member quits
  
  private: 
    char *buffer;
    int bufferSize;
    
    bool isBroken;
    bool readerHasCome;
    
    int writeLocation, readLocation;
    
    // Readers/writers signal once per character read/written
    Semaphore *spaceAvailableSemaphore;
    Semaphore *dataAvailableSemaphore;
    Semaphore *readerHasComeSemaphore;
};

#endif /*BOUNDEDBUFFER_H_*/
