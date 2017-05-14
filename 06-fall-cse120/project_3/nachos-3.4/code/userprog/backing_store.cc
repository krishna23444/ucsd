#include <stdio.h>
#include "system.h"
#include "backing_store.h"

BackingStore::BackingStore(int pageSize, int numPages, int fileNumber) {
  this->pageSize = pageSize;
  this->numPages = numPages;

  pagedOut = new bool[numPages];
  for (int i = 0; i < numPages; ++i)
    pagedOut[i] = false;

  filename = new char[BUFSIZ];
  snprintf(filename, BUFSIZ, "swap.%d", fileNumber);

  fileIsOpen = false;
}

BackingStore::~BackingStore() {
  if (fileIsOpen) {
    delete openFile;
    fileSystem->Remove(filename);
  }
  delete filename;
  delete pagedOut;
}

bool BackingStore::isPagedOut(int virtualPageNumber) {
  return pagedOut[virtualPageNumber];
}

int BackingStore::locatePageInFile(int pageNumber) {
  return pageNumber * pageSize;
}

bool BackingStore::pageIn(int virtualPageNumber, int physicalPageNumber) {
  DEBUG('b', "Process %d, paging in virtual page %d, physical page %d\n",
      currentThread->space->getAddressSpaceId(),
      virtualPageNumber, physicalPageNumber);

  if (!fileIsOpen) {
    fprintf(stderr, "Trying to page in %d, but file not open\n",
            virtualPageNumber);
    return false;
  }
  if (!isPagedOut(virtualPageNumber)) {
    fprintf(stderr, "UH OH: pageIn virtual page %d not paged out\n",
            virtualPageNumber);
    return false;
  }

  int readAtPosition = locatePageInFile(virtualPageNumber);
  char* physicalAddress = machine->mainMemory + physicalPageNumber * pageSize;

  int numRead = openFile->ReadAt(physicalAddress, pageSize, readAtPosition);

  // TODO: do we need a loop around ReadAt till we write everything?  
  if (numRead != pageSize) {
    fprintf(stderr, "pageIn read %d bytes instead of %d\n", numRead, pageSize);
    return false;
  }

  return true;
}

bool BackingStore::pageOut(int virtualPageNumber, int physicalPageNumber) {
  DEBUG('b', "Process %d, paging out virtual page %d, physical page %d\n",
        currentThread->space->getAddressSpaceId(),
        virtualPageNumber, physicalPageNumber);
  if (!fileIsOpen) {
    if (fileSystem->Create(filename, pageSize * numPages) == false) {
      fprintf(stderr, "BackingStore failed to create file %s\n", filename);
      return false;
    }
    if ((openFile = fileSystem->Open(filename)) == NULL) {
      fprintf(stderr, "BackingStore failed to open file %s\n", filename);
      return false;
    }

    fileIsOpen = true;
  }

  int writeAtPosition = locatePageInFile(virtualPageNumber);
  char* physicalAddress = machine->mainMemory + physicalPageNumber * pageSize;

  int numWritten = openFile->WriteAt(physicalAddress, pageSize, writeAtPosition);

  // TODO: do we need a loop around WriteAt till we write everything?  
  if (numWritten != pageSize) {
    fprintf(stderr, "pageOut wrote %d bytes instead of %d\n", numWritten, pageSize);
    return false;
  }

  stats->numPageOuts++;
  pagedOut[virtualPageNumber] = true;
  return true;
}
