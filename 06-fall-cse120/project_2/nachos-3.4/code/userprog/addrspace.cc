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
#ifdef HOST_SPARC
#include <strings.h>
#endif

static BitMap memoryManager(NumPhysPages);

static const int NUM_MAX_ADDRESS_SPACES = 100;
Table joinAndAddressSpacesTable(NUM_MAX_ADDRESS_SPACES, true);

//----------------------------------------------------------------------
// SwapHeader
//   Do little endian to big endian conversion on the bytes in the 
//  object file header, in case the file was generated on a little
//  endian machine, and we're now running on a big endian machine.
//----------------------------------------------------------------------

static void SwapHeader (NoffHeader *noffH)
{
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

//----------------------------------------------------------------------
// AddrSpace::AddrSpace
//   Create an address space to run a user program.
//  Load the program from a file "executable", and set everything
//  up so that we can start executing user instructions.
//
//  Assumes that the object code file is in NOFF format.
//
//  First, set up the translation from program memory to physical 
//  memory.  For now, this is really simple (1:1), since we are
//  only uniprogramming, and we have a single unsegmented page table
//
//  "executable" is the file containing the object code to load into memory
//----------------------------------------------------------------------

AddrSpace::AddrSpace() {
}

//
int AddrSpace::AllocateAndLoad(OpenFile *executable) {
  NoffHeader noffH;
  unsigned int i, size;

  executable->ReadAt((char *)&noffH, sizeof(noffH), 0);
  if ((noffH.noffMagic != NOFFMAGIC) && (WordToHost(noffH.noffMagic) == NOFFMAGIC))
    SwapHeader(&noffH);
  ASSERT(noffH.noffMagic == NOFFMAGIC);

  // how big is address space?
  size = noffH.code.size + noffH.initData.size + noffH.uninitData.size
       + UserStackSize;
  endVirtualAddress = size;
      // we need to increase the size to leave room for the stack
  numPages = divRoundUp(size, PageSize);
  size = numPages * PageSize;

  // TODO deal with large programs i.e. WTF is this shit
  //ASSERT(numPages <= NumPhysPages);   // check we're not trying
                                      // to run anything too big --
                                      // at least until we have
                                      // virtual memory

  DEBUG('A', "Initializing address space, num pages %d, end %d, size %d\n", 
        numPages, endVirtualAddress, size);

  // check if there is memory available to hold this executable
  if (memoryManager.NumClear() < numPages) {
    DEBUG('A', "ran out of memory\n");
    return 0;
  }

  // first, set up the translation
  pageTable = new TranslationEntry[numPages];
  for (i = 0; i < numPages; i++) {
    pageTable[i].virtualPage = i;
    pageTable[i].physicalPage = memoryManager.Find();
    DEBUG('A', "virtual page number %d got physical %d\n",
          i, pageTable[i].physicalPage);
    pageTable[i].valid = TRUE;
    pageTable[i].use = FALSE;
    pageTable[i].dirty = FALSE;
    pageTable[i].readOnly = FALSE;  // if the code segment was entirely on
                                    // zero out bytes
    bzero(machine->mainMemory + pageTable[i].physicalPage*PageSize , PageSize); 
        // a separate page, we could set its pages to be read-only
  }

  DEBUG('A', "writing code section\n");
  // Read in the code section into memory
  if (noffH.code.size > 0)
    readSectionIntoMemory(noffH.code.inFileAddr, noffH.code.virtualAddr,
                          noffH.code.size, executable);
  DEBUG('A', "done writing data section\n");

  DEBUG('A', "writing data section\n");
  // Then, copy in the code and data segments into memory
  if (noffH.initData.size > 0)
    readSectionIntoMemory(noffH.initData.inFileAddr, noffH.initData.virtualAddr,
                          noffH.initData.size, executable);
  DEBUG('A', "done writing data section\n");

  int spaceId = joinAndAddressSpacesTable.Alloc(); 
  if (spaceId == -1) {
    DEBUG('A', "Cannot create more processes, out of spaceId's\n");
    Free();
    return 0;
  }

  addressSpaceId = spaceId;
  return addressSpaceId;
}

//
void AddrSpace::readSectionIntoMemory(int fileReadAddress, int beginningVirtualAddress,
                                      int sectionSize, OpenFile *executable) {
  int readSize;
  int pageNumber;
  int offset;
  int physicalAddress;
  int numberOfBytesLeft;

  // load first fragment until we reach a page boundary

  // compute start page and offset
  pageNumber = beginningVirtualAddress / PageSize;
  offset = beginningVirtualAddress % PageSize;

  DEBUG('A', "virtual %d, pageNum %d, offset %d\n",
          beginningVirtualAddress, pageNumber, offset);
  numberOfBytesLeft = sectionSize;
  if (offset > 0) {
    physicalAddress = pageTable[pageNumber].physicalPage * PageSize + offset;
    readSize = min(numberOfBytesLeft, PageSize - offset);
    
    DEBUG('A', "first page loop writing %d bytes to %d from %d in file\n",
          readSize, physicalAddress, fileReadAddress);
    executable->ReadAt(&(machine->mainMemory[physicalAddress]),
                       readSize, fileReadAddress);

    fileReadAddress += readSize;
    numberOfBytesLeft -= readSize;
    pageNumber++;
  }

  // load whole pages of data until there is less than a page worth to read
  while (numberOfBytesLeft >= PageSize) {
    // compute physical address
    physicalAddress = pageTable[pageNumber].physicalPage * PageSize;

    DEBUG('A', "page loop writing %d bytes to %d from %d in file\n",
          PageSize, physicalAddress, fileReadAddress);
    // write PageSize worth of data to PageSize
    executable->ReadAt(&(machine->mainMemory[physicalAddress]),
                       PageSize, fileReadAddress);

    fileReadAddress += PageSize;
    numberOfBytesLeft -= PageSize;
    pageNumber++;
  }

  // write remaining data that is less than a PageSize worth
  if (numberOfBytesLeft > 0) {
    physicalAddress = pageTable[pageNumber].physicalPage * PageSize;

    DEBUG('A', "last page writing %d bytes to %d from %d in file\n",
          numberOfBytesLeft, physicalAddress, fileReadAddress);
    executable->ReadAt(&(machine->mainMemory[physicalAddress]),
                       numberOfBytesLeft, fileReadAddress);
  }
}

//
bool AddrSpace::ExpandMemory(int howMuch) {
  //dumpPageTable();
  DEBUG('A', "Adding %d more bytes for arguments to address space\n", howMuch);
  int howMuchSpaceLeftoverInLastPage = PageSize - endVirtualAddress % PageSize;
  if (howMuchSpaceLeftoverInLastPage == PageSize)
    howMuchSpaceLeftoverInLastPage = 0;
  int spaceRequiringNewPages = howMuch - howMuchSpaceLeftoverInLastPage;

  if (spaceRequiringNewPages > 0) {
    int numNewPages = divRoundUp(spaceRequiringNewPages, PageSize);

    if (memoryManager.NumClear() < numNewPages) {
      DEBUG('A', "ran out of memory\n");
      Free();
      return false;
    }
 
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
      pageTable[i].physicalPage = memoryManager.Find();
      DEBUG('A', "virtual page number %d got physical %d\n",
            i, pageTable[i].physicalPage);
      pageTable[i].valid = TRUE;
      pageTable[i].use = FALSE;
      pageTable[i].dirty = FALSE;
      pageTable[i].readOnly = FALSE;  // if the code segment was entirely on
                                      // zero out bytes
      bzero(machine->mainMemory + pageTable[i].physicalPage*PageSize , PageSize); 
          // a separate page, we could set its 
          // pages to be read-only
    }
    numPages = numTotalPages;
  }
  endVirtualAddress += howMuch;
  //dumpPageTable();
  return true;
}

void AddrSpace::dumpPageTable() {
  DEBUG('A', "num pages = %d\n", numPages);
  TranslationEntry *entry;
  for (unsigned int i = 0; i < numPages; ++i) {
    entry = &pageTable[i];
    DEBUG('A', "virtual = %d, physical = %d, valid = %d, use = %d, dirty = %d, readOnly = %d\n",
          entry->virtualPage, entry->physicalPage, entry->valid, entry->use,
          entry->dirty, entry->readOnly);
  }
}

//
void AddrSpace::Free() {
  for (unsigned int i = 0; i < numPages; ++i) {
    DEBUG('A', "Thread %d::Freeing page: %d\n",
          currentThread->space->getAddressSpaceId(), pageTable[i].physicalPage);
    memoryManager.Clear(pageTable[i].physicalPage);
  }
}

//----------------------------------------------------------------------
// AddrSpace::~AddrSpace
//   Dealloate an address space.  Nothing for now!
//----------------------------------------------------------------------

AddrSpace::~AddrSpace()
{
 delete pageTable;
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
  DEBUG('A', "Initializing stack register to %d\n", numPages * PageSize - 16);
}


void AddrSpace::writeStackPointer(int location) {
  machine->WriteRegister(StackReg, location - 16);
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
