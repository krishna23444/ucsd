// addrspace.cc 
//  Routines to manage address spaces (executing user programs).
//
//  In order to run a user program, you must:
//
//  1. link with the -N -T 0 option 
//  2. run coff2noff to convert the object file to Nachos format
//    (Nachos object code format is essentially just a simpler
//    version of the UNIX executable object code format)
//  3. load the NOFF file into the Nachos file system
//    (if you haven't implemented the file system yet, you
//    don't need to do this last step)
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "bitmap.h"
#include "addrspace.h"
#include "noff.h"
#include "syscall.h"
#include "synch.h"
#include "table.h"
#include "backing_store.h"
#ifdef HOST_SPARC
#include <strings.h>
#endif

static BitMap memoryManager(NumPhysPages);

static const unsigned int MAXIMUM_NUMBER_OF_ADDRESS_SPACES = 1000;
Table joinAndAddressSpacesTable(MAXIMUM_NUMBER_OF_ADDRESS_SPACES, true);

enum PageReplacementAlgorithm pageReplacementAlgorithm = RANDOM;

AddrSpace** AddrSpace::allAddressSpaces = NULL;
bool AddrSpace::needToCreateAllAddressSpaces = true;
unsigned int AddrSpace::maximumAddressSpaceId = 0;

bool AddrSpace::needToCreatePagesInMemoryInformation = true;
struct pageInformation* AddrSpace::pagesInMemoryInformation = NULL;

//----------------------------------------------------------------------
// SwapHeader
//   Do little endian to big endian conversion on the bytes in the 
//  object file header, in case the file was generated on a little
//  endian machine, and we're now running on a big endian machine.
//----------------------------------------------------------------------
static void SwapHeader (NoffHeader *noffH) {
  noffH->noffMagic = WordToHost(noffH->noffMagic);
  noffH->code.size = WordToHost(noffH->code.size);
  noffH->code.virtualAddr = WordToHost(noffH->code.virtualAddr);
  noffH->code.inFileAddr = WordToHost(noffH->code.inFileAddr);
  noffH->initData.size = WordToHost(noffH->initData.size);
  noffH->initData.virtualAddr = WordToHost(noffH->initData.virtualAddr);
  noffH->initData.inFileAddr = WordToHost(noffH->initData.inFileAddr);
  noffH->uninitData.size = WordToHost(noffH->uninitData.size);
  noffH->uninitData.virtualAddr = WordToHost(noffH->uninitData.virtualAddr);
  noffH->uninitData.inFileAddr = WordToHost(noffH->uninitData.inFileAddr);
}

AddrSpace::AddrSpace() {
  if (needToCreateAllAddressSpaces) {
    allAddressSpaces = new AddrSpace*[MAXIMUM_NUMBER_OF_ADDRESS_SPACES];
    for (unsigned int i = 0; i < MAXIMUM_NUMBER_OF_ADDRESS_SPACES; ++i)
      allAddressSpaces[i] = NULL;
    needToCreateAllAddressSpaces = false;
  }
  if (needToCreatePagesInMemoryInformation) {
    pagesInMemoryInformation = new struct pageInformation[NumPhysPages];
    for (int i = 0; i < NumPhysPages; ++i) {
       pagesInMemoryInformation[i].addressSpaceId = -1;
       pagesInMemoryInformation[i].counter = 0;
    }
  }
}

int AddrSpace::AllocateAndLoad(OpenFile *executable) {
  unsigned int i, size;

  executableFile = executable;

  executable->ReadAt((char *)&noffHeader, sizeof(noffHeader), 0);
  if ((noffHeader.noffMagic != NOFFMAGIC) && (WordToHost(noffHeader.noffMagic) == NOFFMAGIC))
    SwapHeader(&noffHeader);
  ASSERT(noffHeader.noffMagic == NOFFMAGIC);

  // how big is address space?
  size = noffHeader.code.size + noffHeader.initData.size
         + noffHeader.uninitData.size + UserStackSize;
  endVirtualAddress = size;
      // we need to increase the size to leave room for the stack
  numPages = divRoundUp(size, PageSize);
  size = numPages * PageSize;

  DEBUG('A', "Initializing address space, num pages %d, end %d, size %d\n", 
        numPages, endVirtualAddress, size);

  // first, set up the translation
  pageTable = new TranslationEntry[numPages];
  for (i = 0; i < numPages; i++) {
    pageTable[i].virtualPage = i;
    pageTable[i].valid = false;
    pageTable[i].use = false;
    pageTable[i].dirty = false;
    pageTable[i].readOnly = false;
  }

  int spaceId = joinAndAddressSpacesTable.Allocate(); 
  if (spaceId == -1) {
    DEBUG('A', "Cannot create more processes, out of spaceId's\n");
    return 0;
  }

  addressSpaceId = spaceId;

  allAddressSpaces[addressSpaceId] = this;

  backingStore = new BackingStore(PageSize, numPages, addressSpaceId);

  if (addressSpaceId > maximumAddressSpaceId)
    maximumAddressSpaceId = addressSpaceId;

  return addressSpaceId;
}

int AddrSpace::readTillEndOfSectionOrPage(int virtualAddress, Segment* fileSegment) {
  int segmentBegin = fileSegment->virtualAddr;
  int segmentSize = fileSegment->size;
  int segmentEnd = segmentBegin + segmentSize;
  int segmentInFileAddress = fileSegment->inFileAddr;

  int pageNumber = virtualAddress / PageSize;
  int offset = virtualAddress % PageSize;
  int physicalAddress = pageTable[pageNumber].physicalPage * PageSize + offset;

  int endOfPageAddress = (pageNumber+1) * PageSize;
  int numberOfBytesTillEndOfPage = endOfPageAddress - virtualAddress;

  int offsetFromSegmentBegin = virtualAddress - segmentBegin;

  if (segmentSize <= 0)
    return virtualAddress;

  // the segment begins after our virtual address
  if (offsetFromSegmentBegin < 0) {
    fprintf(stderr, "Segment begins after virtual address %d\n", virtualAddress);
//    offsetFromSegmentBegin *= -1;
//    if (offsetFromSegmentBegin > numberOfBytesTillEndOfPage)
//      return virtualAddress;
//
//    virtualAddress += offsetFromSegmentBegin;
//    physicalAddress += offsetFromSegmentBegin;
//    numberOfBytesTillEndOfPage -= offsetFromSegmentBegin;
//    offsetFromSegmentBegin = 0;
  }

  int offsetFromSegmentEnd = segmentEnd - virtualAddress;
  int fileReadAddress = segmentInFileAddress + offsetFromSegmentBegin;
  int amountToRead = min(offsetFromSegmentEnd, numberOfBytesTillEndOfPage);

  if (offsetFromSegmentEnd > 0 && amountToRead > 0) {
    executableFile->ReadAt(&(machine->mainMemory[physicalAddress]),
                           amountToRead, fileReadAddress);

    virtualAddress += amountToRead;
  }

  return virtualAddress;
}

void AddrSpace::readPageFromExecutableFile(int pageNumber) {
  int startOfPageAddress = pageNumber * PageSize;

  int virtualAddress = readTillEndOfSectionOrPage(startOfPageAddress, &noffHeader.code);

  int endOfPageAddress = startOfPageAddress + PageSize;

  if (virtualAddress < endOfPageAddress)
    virtualAddress = readTillEndOfSectionOrPage(virtualAddress, &noffHeader.initData);

  // If we read something (if address was in text or data) this was a page in
  if (virtualAddress > startOfPageAddress)
  {
    stats->numPageIns++;
  }
}

void AddrSpace::evictPage(AddrSpace* addressSpace, int virtualPageNumber) {
  if (addressSpace == NULL) {
    DEBUG('b', "ERROR: evictPage got NULL page\n");
    return;
  }

  TranslationEntry* pageEntry = &addressSpace->pageTable[virtualPageNumber];
  if (pageEntry->dirty) {
    addressSpace->backingStore->pageOut(pageEntry->virtualPage, pageEntry->physicalPage);
    pageEntry->dirty = false;
  }

  DEBUG('b', "Evicting virtual page %d, physical page %d from address space %d\n",
        pageEntry->virtualPage, pageEntry->physicalPage, addressSpace->addressSpaceId);
  memoryManager.Clear(pageEntry->physicalPage);
  pageEntry->use = false;
  pageEntry->valid = false;
}

void AddrSpace::evictAPageRandomly() {
  int addressSpaceIdToEvictFrom;
  AddrSpace* addressSpace;
  int pageNumberToEvict;

  do {
    do {
      addressSpaceIdToEvictFrom = Random() % (maximumAddressSpaceId+1);
      addressSpace = allAddressSpaces[addressSpaceIdToEvictFrom];
    } while (addressSpace == NULL);
    pageNumberToEvict = Random() % addressSpace->numPages;
  } while (!addressSpace->pageTable[pageNumberToEvict].valid);

  evictPage(addressSpace, pageNumberToEvict);
}

void AddrSpace::evictLeastRecentlyUsedPage() {
  int maxCounter = -1;
  int maxCounterIndex = -1;
  AddrSpace* addressSpace;
  TranslationEntry* pageEntry;
  
  for (int i = 0; i < NumPhysPages; ++i) {
    if (pagesInMemoryInformation[i].addressSpaceId == -1) {
      DEBUG('b', "ERROR: pagesInMemoryInformation at %d invalid\n", i);
      continue;
    }
    addressSpace = allAddressSpaces[pagesInMemoryInformation[i].addressSpaceId];
    if (addressSpace == NULL) {
      DEBUG('b', "ERROR: addressSpace %d invalid\n", pagesInMemoryInformation[i].addressSpaceId);
      continue;
    }
    pageEntry = &addressSpace->pageTable[pagesInMemoryInformation[i].virtualPageNumber];
    if (!pageEntry->use) {
      pagesInMemoryInformation[i].counter++;
      if (pagesInMemoryInformation[i].counter > maxCounter) {
        maxCounter = pagesInMemoryInformation[i].counter;
        maxCounterIndex = i;
      } else if (pagesInMemoryInformation[i].counter == maxCounter) {
        if (Random() % 2 == 1) {
          maxCounter = pagesInMemoryInformation[i].counter;
          maxCounterIndex = i;
        }
      }
    }
    pageEntry->use = false;
  }
  
  if (maxCounterIndex == -1) {
    DEBUG('b', "ERROR: Couldn't find a page to evict\n");
    maxCounterIndex = 0;
  }
  addressSpace = allAddressSpaces[pagesInMemoryInformation[maxCounterIndex].addressSpaceId];
  evictPage(addressSpace, pagesInMemoryInformation[maxCounterIndex].virtualPageNumber);
}

void AddrSpace::loadPageIntoMemory(int virtualAddress) {
  int virtualPageNumber = virtualAddress / PageSize;
  TranslationEntry* pageEntry = &pageTable[virtualPageNumber];

  DEBUG('b', "Allocating virtual page %d for virtual address %d, address space %d\n",
        virtualPageNumber, virtualAddress, addressSpaceId);

  if (memoryManager.NumClear() == 0)
    switch (pageReplacementAlgorithm) {
    case RANDOM:
      evictAPageRandomly();
      break;
    case LRU:
      evictLeastRecentlyUsedPage();
      break;
    }

  pageTable[virtualPageNumber].physicalPage = memoryManager.Find();

  bzero(machine->mainMemory + pageEntry->physicalPage * PageSize , PageSize);

  if (backingStore->isPagedOut(virtualPageNumber)){
    backingStore->pageIn(virtualPageNumber, pageEntry->physicalPage);
    stats->numPageIns++;
  }
  else {
    // Only counts as a page in if it's from text or data
    readPageFromExecutableFile(virtualPageNumber);
    pageEntry->dirty = true;
  }

  pagesInMemoryInformation[pageEntry->physicalPage].addressSpaceId = addressSpaceId;
  pagesInMemoryInformation[pageEntry->physicalPage].virtualPageNumber = virtualPageNumber;
  pagesInMemoryInformation[pageEntry->physicalPage].counter = 0;
  pageEntry->valid = true;
}

bool AddrSpace::ExpandMemory(int howMuch) {
  DEBUG('A', "Adding %d more bytes for arguments to address space\n", howMuch);
  int howMuchSpaceLeftoverInLastPage = PageSize - endVirtualAddress % PageSize;
  if (howMuchSpaceLeftoverInLastPage == PageSize)
    howMuchSpaceLeftoverInLastPage = 0;
  int spaceRequiringNewPages = howMuch - howMuchSpaceLeftoverInLastPage;

  if (spaceRequiringNewPages > 0) {
    int numNewPages = divRoundUp(spaceRequiringNewPages, PageSize);
 
    int numTotalPages = numPages + numNewPages;
    // create grown page table
    TranslationEntry *newPageTable = new TranslationEntry[numTotalPages];
    for (unsigned int i = 0; i < numPages; ++i) {
      newPageTable[i].virtualPage = pageTable[i].virtualPage;
      newPageTable[i].physicalPage = pageTable[i].physicalPage;
      newPageTable[i].valid = pageTable[i].valid;
      newPageTable[i].use = pageTable[i].use;
      newPageTable[i].dirty = pageTable[i].dirty;
      newPageTable[i].readOnly = pageTable[i].readOnly;
    }
    // copy over all the old information
    //memcpy(newPageTable, pageTable, numPages*sizeof(TranslationEntry));
    // delete old table and set pointer to new one
    delete pageTable;
    pageTable = newPageTable;

    for (int i = numPages; i < numTotalPages; i++) {
      pageTable[i].virtualPage = i;
      pageTable[i].valid = false;
      pageTable[i].use = false;
      pageTable[i].dirty = false;
      pageTable[i].readOnly = false;
    }
    numPages = numTotalPages;
  }
  endVirtualAddress += howMuch;

  backingStore = new BackingStore(PageSize, numPages, addressSpaceId);

  return true;
}

AddrSpace::~AddrSpace() {
  allAddressSpaces[addressSpaceId] = NULL;
  while (allAddressSpaces[maximumAddressSpaceId] == NULL)
    maximumAddressSpaceId--;
  for (unsigned int i = 0; i < numPages; ++i)
    if (pageTable[i].valid)
      memoryManager.Clear(pageTable[i].physicalPage);
  delete pageTable;
  delete backingStore;
}

//----------------------------------------------------------------------
// AddrSpace::InitRegisters
//   Set the initial values for the user-level register set.
//
//   We write these directly into the "machine" registers, so
//  that we can immediately jump to user code.  Note that these
//  will be saved/restored into the currentThread->userRegisters
//  when this thread is context switched out.
//----------------------------------------------------------------------
void AddrSpace::InitRegisters()
{
  int i;

  for (i = 0; i < NumTotalRegs; i++)
    machine->WriteRegister(i, 0);

  // Initial program counter -- must be location of "Start"
  machine->WriteRegister(PCReg, 0);  

  // Need to also tell MIPS where next instruction is, because
  // of branch delay possibility
  machine->WriteRegister(NextPCReg, 4);

  // Set the stack register to the end of the address space, where we
  // allocated the stack; but subtract off a bit, to make sure we don't
  // accidentally reference off the end!
  writeStackPointer(numPages * PageSize);
}

void AddrSpace::writeStackPointer(int location) {
  machine->WriteRegister(StackReg, location - 16);
  DEBUG('A', "Initializing stack register to %d\n", location - 16);
}

//----------------------------------------------------------------------
// AddrSpace::SaveState
//   On a context switch, save any machine state, specific
//  to this address space, that needs saving.
//
//  For now, nothing!
//----------------------------------------------------------------------
void AddrSpace::SaveState() 
{
}

//----------------------------------------------------------------------
// AddrSpace::RestoreState
//   On a context switch, restore the machine state so that
//  this address space can run.
//
//      For now, tell the machine where to find the page table.
//----------------------------------------------------------------------
void AddrSpace::RestoreState() 
{
  machine->pageTable = pageTable;
  machine->pageTableSize = numPages;
}
