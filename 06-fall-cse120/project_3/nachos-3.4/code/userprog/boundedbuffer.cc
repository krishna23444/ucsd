#include "boundedbuffer.h"
#include "synch.h"


// Constructed in the Writer exec
BoundedBuffer::BoundedBuffer(int size)
{
  bufferSize = size;
  buffer = new char[size];
  bzero(buffer, size);

  writeLocation = 0;
  readLocation = 0;

  isBroken = true;
  readerHasCome = false;

  spaceAvailableSemaphore = new Semaphore("Bounded Buffer space available semaphore", size);
  dataAvailableSemaphore = new Semaphore("Bounded Buffer data available semaphore", 0);
  readerHasComeSemaphore = new Semaphore("Bounded Buffer reader has come semaphore", 0);
}

BoundedBuffer::~BoundedBuffer()
{
  delete buffer;
  delete spaceAvailableSemaphore;
  delete dataAvailableSemaphore;
  delete readerHasComeSemaphore;
}

int BoundedBuffer::write(char* message, int size)
{
  if (!readerHasCome)
    readerHasComeSemaphore->Wait();

  for (int i = 0; i < size; i++) {
    if (isBroken)
      return i;

    spaceAvailableSemaphore->Wait();
    buffer[writeLocation] = message[i];
    writeLocation = (writeLocation+1)%bufferSize;
    dataAvailableSemaphore->Signal();
  }
  return size;
}
    
int BoundedBuffer::read(int size, char* message)
{
  for (int i = 0; i < size; i++) {
    // If it's broken, read until it's empty
    if (isBroken && readLocation == writeLocation)
      return i;

    dataAvailableSemaphore->Wait();
    message[i] = buffer[readLocation];
    readLocation = (readLocation+1)%bufferSize;
    spaceAvailableSemaphore->Signal();
  }
  return size;
}

void BoundedBuffer::readerComes()
{
  readerHasCome = true;
  readerHasComeSemaphore->Signal();
}

void BoundedBuffer::readerNotComing()
{
  readerComes();
}

void BoundedBuffer::makePipe()
{ 
  isBroken = false;
  readerComes();
}

void BoundedBuffer::breakPipe()
{
  isBroken = true;
  
  // Go signal crazy, who cares?  We broke the pipe.
  dataAvailableSemaphore->Signal();
  spaceAvailableSemaphore->Signal();
}
