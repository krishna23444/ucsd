// exception.cc 
//  Entry point into the Nachos kernel from user programs.
//  There are two kinds of things that can cause control to
//  transfer back to here from user code:
//
//  syscall -- The user code explicitly requests to call a procedure
//  in the Nachos kernel.  Right now, the only function we support is
//  "Halt".
//
//  exceptions -- The user code does something that the CPU can't handle.
//  For instance, accessing memory that doesn't exist, arithmetic errors,
//  etc.  
//
//  Interrupts (which can also cause control to transfer from user
//  code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "boundedbuffer.h"
#include "copyright.h"
#include "synchconsole.h"
#include "system.h"
#include "syscall.h"

static const int MAX_STRING_LENGTH = 256;

static bool readString(int readAddress, char* buffer)
{
  int characterRead = 1;
  int writeAddress = 0;
  int endVirtualAddress = currentThread->space->getEndVirtualAddress();
  int counter = 0;

  DEBUG('e', "addr: %d, end: %d\n", readAddress, endVirtualAddress);
  if (readAddress >= endVirtualAddress || readAddress < 0)
  {
    DEBUG('e', "Bad address to Exec\n");
    return false;
  }
        
 while (characterRead != 0 && readAddress < endVirtualAddress
          && counter < MAX_STRING_LENGTH) {
    if( machine->ReadMem(readAddress, 1, &characterRead) == 0 ){
      DEBUG('e', "Machine readMem failed at %d\n", readAddress);
      return(false);
    }
    else{
      DEBUG('e', "Machine readMem returned %d at %d\n", characterRead, readAddress); 
    }
    buffer[writeAddress] = (unsigned char)characterRead;
    readAddress++;
    writeAddress++;
    counter ++;
  }
  if(characterRead != 0 ){
    if ( counter == MAX_STRING_LENGTH ){
         DEBUG('e', "Filename too long: %d\n", counter);
         return false;
    }
    
    if(readAddress >= endVirtualAddress){
      DEBUG('e', "readAddress(%d) >= endVirtualAddress(%d)\n", readAddress,
            endVirtualAddress);
      return false;
    }
    
    if( buffer[counter-1] != 0 ){
      DEBUG('e', "Non-null terminating char:%c\n", buffer[counter-1]);
      return false; 
    }
  }
  
  return true;
}

static SynchConsole *synchConsole = new SynchConsole();

static void breakPipes() {
  BoundedBuffer* readPipe = currentThread->getReadPipe();
  if (readPipe != NULL)
    readPipe->breakPipe();

  BoundedBuffer* writePipe = currentThread->getWritePipe();
  if (writePipe != NULL)
    writePipe->breakPipe();

  BoundedBuffer* pipelineWaitingWriter = currentThread->getPipelineWaitingWriter();
  if (pipelineWaitingWriter != NULL)
    pipelineWaitingWriter->readerNotComing();
}

//
void kill(int status) {
  breakPipes();

  int spaceId = currentThread->space->getAddressSpaceId();
  joinAndAddressSpacesTable.SetIntValue(spaceId, status);
  if (currentThread->isJoinable() == false)
    joinAndAddressSpacesTable.Clear(spaceId);
  currentThread->space->Free();
  fprintf(stdout, "Thread spaceID %d Exiting with value %d\n", spaceId, status);
  currentThread->Finish();
}

/* Write "size" bytes from "buffer" to the open file. */
static int kWrite(char *buffer, int size) {
  char *kernelBuffer = new char[size];
  int characterRead;
  int numBytesWritten;
  BoundedBuffer* writePipe = currentThread->getWritePipe();
  
  for (int i = 0; i < size; ++i) {
    machine->ReadMem((int)buffer+i, 1, &characterRead);
    kernelBuffer[i] = (char)characterRead;
  }

  if (writePipe != NULL)
    numBytesWritten = writePipe->write(kernelBuffer, size);
  else
    numBytesWritten = synchConsole->Write(kernelBuffer, size);

  return numBytesWritten;
}

/* Read "size" bytes from the open file into "buffer".  
 * Return the number of bytes actually read -- if the open file isn't
 * long enough, or if it is an I/O device, and there aren't enough 
 * characters to read, return whatever is available (for I/O devices, 
 * you should always wait until you can return at least one character).
 */
static int kRead(char *buffer, int size) {
  char *kernelBuffer = new char[size];
  int numBytesRead;

  BoundedBuffer* readPipe = currentThread->getReadPipe();
  if (readPipe != NULL)
    numBytesRead = readPipe->read(size, kernelBuffer);
  else
    numBytesRead = synchConsole->Read(size, kernelBuffer);

  for (int i = 0; i < numBytesRead; ++i)
    machine->WriteMem((int)buffer+i, 1, kernelBuffer[i]);

  return numBytesRead;
}

static int kExec(int filename, int argc, int argv, int willJoin) {
  int argInArrayVirtualAddress = argv;
  int argVirtualAddress;
  char **arrayBuffer = new char*[argc];

  // add location for filename in beginning of args array
  argc++;

  for (int i = 0; i < argc; ++i) {
    arrayBuffer[i] = new char[MAX_STRING_LENGTH];
    bzero(arrayBuffer[i], MAX_STRING_LENGTH);
  }

  DEBUG('e', "Exec, initiated by user program.\n");

  // copy filename to kernel memory
  if (readString(filename, arrayBuffer[0]) == 0)
    return 0;

  // copy arguments passed in to kernel memory
  argInArrayVirtualAddress = argv;
  for(int i = 1; i < argc; i++) {
    machine->ReadMem(argInArrayVirtualAddress, 4, &argVirtualAddress);
    if (readString(argVirtualAddress, arrayBuffer[i]) == 0)
      return 0;
    argInArrayVirtualAddress += 4;
  }

  DEBUG('e', "Executing %s\n", arrayBuffer[0]);

  return Exec(arrayBuffer[0], argc, (char **)arrayBuffer, willJoin);
}

//----------------------------------------------------------------------
// ExceptionHandler
//   Entry point into the Nachos kernel.  Called when a user program
//  is executing, and either does a syscall, or generates an addressing
//  or arithmetic exception.
//
//   For system calls, the following is the calling convention:
//
//   system call code -- r2
//    arg1 -- r4
//    arg2 -- r5
//    arg3 -- r6
//    arg4 -- r7
//
//  The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//  "which" is the kind of exception.  The list of possible exceptions 
//  are in machine.h.
//----------------------------------------------------------------------
void ExceptionHandler(ExceptionType which)
{
  int type = machine->ReadRegister(2);
  int arg1 = machine->ReadRegister(4);
  int arg2 = machine->ReadRegister(5);
  int arg3 = machine->ReadRegister(6);
  int arg4 = machine->ReadRegister(7);
  int returnVal;

  char *buffer = (char *)arg1;
  int size = arg2;
  int id = arg3;
  int spaceId = arg1;
  Thread* scjoin_Thread;
  BoundedBuffer* pipelineWaitingWriter;

  DEBUG('e', "System call %d type = %d, arg1 = %d, arg2 = %d, arg3 = %d, arg4 = %d\n",
        which, type, arg1, arg2, arg3, arg4);
  if (which == SyscallException) {
    switch (type) {
      case SC_Join:
        if (joinAndAddressSpacesTable.isAllocated(spaceId) == false){
          returnVal = -65535;
          break;
        }
        //Get thread pointer
        scjoin_Thread = joinAndAddressSpacesTable.GetThread(spaceId);
        if (scjoin_Thread == NULL || scjoin_Thread == currentThread
            || scjoin_Thread->isJoinable() == false) {
          returnVal = -65535;
          break;
        }
        
        joinAndAddressSpacesTable.SetThread(spaceId, NULL);
        pipelineWaitingWriter = currentThread->getPipelineWaitingWriter();
        if (pipelineWaitingWriter != NULL) {
          pipelineWaitingWriter->readerNotComing();
          currentThread->clearPipeline();
        }
        scjoin_Thread->Join();
        //Once the thread is done, get the return value
        returnVal = joinAndAddressSpacesTable.GetIntValue(spaceId);
        joinAndAddressSpacesTable.Clear(spaceId);
        break;
      case SC_Write:
        if (id != ConsoleOutput) {
          DEBUG('s', "Don't know how to write to %d, expected %d\n", id, ConsoleOutput);
          returnVal = 0;
          break;
        }
        returnVal = kWrite(buffer, size);
        break;
      case SC_Read:
        if (id != ConsoleInput) {
          DEBUG('s', "Don't know how to read from %d, expected %d\n", id, ConsoleInput);
          returnVal = -1;
          break;
        }
        returnVal = kRead(buffer, size);
        break;
      case SC_Halt:
        DEBUG('e', "Shutdown, initiated by user program.\n");
        interrupt->Halt();
        break;       
      case SC_Exit:
        kill(arg1);
        break;
      case SC_Exec:
        returnVal = kExec(arg1, arg2, arg3, arg4);
        break;
    }
  } else {
    switch (which) { 
    case PageFaultException:
      fprintf(stderr, "Thread %d: Page Fault Exception type %d\n",
              currentThread->space->getAddressSpaceId(), type);
      break;
    case ReadOnlyException:
      fprintf(stderr, "Thread %d: Read Only Exception type %d\n",
              currentThread->space->getAddressSpaceId(), type);
      break;
    case BusErrorException:
      fprintf(stderr, "Thread %d: Bus Error Exception type %d\n",
              currentThread->space->getAddressSpaceId(), type);
      break;
    case AddressErrorException:
      fprintf(stderr, "Thread %d: Address Error Exception type %d\n",
              currentThread->space->getAddressSpaceId(), type);
      break;
    case OverflowException:
      fprintf(stderr, "Thread %d: Overflow Exception type %d\n",
              currentThread->space->getAddressSpaceId(), type);
      break;
    case IllegalInstrException:
      fprintf(stderr, "Thread %d: Illegal Exception type %d\n",
              currentThread->space->getAddressSpaceId(), type);
      break;
    default:
      fprintf(stderr, "Thread %d: Unknown Exception type %d\n",
              currentThread->space->getAddressSpaceId(), type);
      break;
    }
    kill(which);
  }
  machine->WriteRegister(2, returnVal);
  
  // TODO Check if this is always supposed to be incremented by 4...
  // Increment Program Counter by 4 (one instruction)
  //machine->WriteRegister(PCReg, machine->ReadRegister(PCReg) + 4);
}
