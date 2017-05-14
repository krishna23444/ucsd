#include "table.h"
#include "bitmap.h"
#include "synch.h"

Table::Table(int size, bool startAtOne=false) {
  writeLock = new Lock("table lock");
  bitmap = new BitMap(size);
  intArray = new int[size];
  threadArray = new Thread*[size];
  maxSize = size;
  if (startAtOne)
    bitmap->MarkNotAtomic(0);
}

Table::~Table() {
  delete writeLock;
  delete bitmap;
  delete intArray;
  delete threadArray;
}

int Table::Allocate() {
  return bitmap->Find();
}

Thread * Table::GetThread(int index){
  if( index > maxSize ){
    DEBUG('t', "Array out of bounds error in GetThread %d\n", index);
    return NULL;
  }
  return threadArray[index];
}

int Table::GetIntValue(int index) {
  if( index > maxSize ){
    DEBUG('t', "Array out of bounds error in GetIntValue %d\n", index);
    return 0;
  }
  return intArray[index];
}
 

void Table::SetThread(int index, Thread * in){
  if( index > maxSize ){
    DEBUG('t', "Array out of bounds error in SetThread %d\n", index);
    return;
  }
  writeLock->Acquire();
  threadArray[index] = in;
  writeLock->Release();
}


void Table::SetIntValue(int index, int value) {
  if( index > maxSize ){
    DEBUG('t', "Array out of bounds error in SetIntVal %d\n", index);
    return;
  }
  writeLock->Acquire();
  intArray[index] = value;
  writeLock->Release();
}

bool Table::isAllocated(int index){
  return bitmap->Test(index);
  
}

void Table::Clear(int index){
  if( index > maxSize ){
    DEBUG('t', "Array out of bounds error in SetIntVal %d\n", index);
    return;
  }
  bitmap->Clear(index);
  writeLock->Acquire();
  threadArray[index]= 0;
  intArray[index] = 0;
  writeLock->Release(); 
}

void Table::Release(int index) {
  bitmap->Clear(index);
}
