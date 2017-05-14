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

static int readMemTryTwice(int addr, int size, int* value) {
  if (machine->ReadMem(addr, size, value) == false)
    if (machine->ReadMem(addr, size, value) == false) {
      fprintf(stderr, "ERROR[readMemTryTwice] addr=%d, size=%d\n", addr, size);
      return false;
    }
  return true;
}

static int writeMemTryTwice(int addr, int size, int value) {
  if (machine->WriteMem(addr, size, value) == false)
    if (machine->WriteMem(addr, size, value) == false) {
      fprintf(stderr, "ERROR[writeMemTryTwice] addr=%d, size=%d, value=%d\n",
              addr, size, value);
      return false;
    }
  return true;
}

static bool readString(int readAddress, char* buffer) {
  int characterRead = 1;
  int writeAddress = 0;
  int endVirtualAddress = currentThread->space->getEndVirtualAddress();

  DEBUG('e', "addr: %d, end: %d\n", readAddress, endVirtualAddress);
  if (readAddress >= endVirtualAddress || readAddress < 0) {
    DEBUG('e', "Bad address to Exec\n");
    return false;
  }
        
 while (characterRead != 0 && readAddress < endVirtualAddress && writeAddress < MAX_STRING_LENGTH) {
    if (readMemTryTwice(readAddress, 1, &characterRead) == 0) {
      DEBUG('e', "Machine readMem failed at %d\n", readAddress);
      return false;
    }
    DEBUG('e', "Machine readMem returned %d at %d\n", characterRead, readAddress); 
    buffer[writeAddress] = (unsigned char)characterRead;
    readAddress++;
    writeAddress++;
  }
  if (characterRead != 0) {
    if (writeAddress == MAX_STRING_LENGTH) {
       DEBUG('e', "Filename too long: %d\n", writeAddress);
       return false;
    }
    
    if (readAddress >= endVirtualAddress) {
      DEBUG('e', "readAddress(%d) >= endVirtualAddress(%d)\n", readAddress,
            endVirtualAddress);
      return false;
    }
    
    if (buffer[writeAddress-1] != 0) {
      DEBUG('e', "Non-null terminating char:%c\n", buffer[writeAddress-1]);
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

static void kill(int status) {
  breakPipes();

  int spaceId = currentThread->space->getAddressSpaceId();
  joinAndAddressSpacesTable.SetIntValue(spaceId, status);
  if (currentThread->isJoinable() == false)
    joinAndAddressSpacesTable.Clear(spaceId);
  delete currentThread->space;
  fprintf(stdout, "Thread spaceID %d Exiting with value %d\n", spaceId, status);
  currentThread->Finish();
}

/* Write "size" bytes from "buffer" to the open file. */
static int kWrite(int buffer, int size, OpenFileId fileId) {
  if (fileId != ConsoleOutput) {
    DEBUG('e', "Don't know how to write to %d, expected %d\n",
          fileId, ConsoleOutput);
    return 0;
  }
  char *kernelBuffer = new char[size];
  int characterRead;
  int numBytesWritten;
  BoundedBuffer* writePipe = currentThread->getWritePipe();
  
  for (int i = 0; i < size; ++i) {
    readMemTryTwice(buffer + i, 1, &characterRead);
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
static int kRead(int buffer, int size, OpenFileId fileId) {
  if (fileId != ConsoleInput) {
    DEBUG('e', "Don't know how to read from %d, expected %d\n",
          fileId, ConsoleInput);
    return -1;
  }

  char *kernelBuffer = new char[size];
  int numBytesRead;

  BoundedBuffer* readPipe = currentThread->getReadPipe();
  if (readPipe != NULL)
    numBytesRead = readPipe->read(size, kernelBuffer);
  else
    numBytesRead = synchConsole->Read(size, kernelBuffer);

  for (int i = 0; i < numBytesRead; ++i)
    writeMemTryTwice(buffer + i, 1, kernelBuffer[i]);

  return numBytesRead;
}

struct ThreadArgs {
  int startOfArgPointersVirtualAddress;
  int startOfArgsVirtualAddress;
  int argc;
  char **argv;
};

static void writeArguments(ThreadArgs* pointerToThreadArgs) {
  int argPointerVirtualAddress = pointerToThreadArgs->startOfArgPointersVirtualAddress;
  int argVirtualAddress = pointerToThreadArgs->startOfArgsVirtualAddress;
  int argc = pointerToThreadArgs->argc;
  char **argv = pointerToThreadArgs->argv;

  // Since we append the arguments to main at the end of the virtual space,
  // when InitRegisters sets the stack pointer, the code itself will end up
  // corrupting our arguments. To solve this, we set the stack pointer where
  // the old stack ended, that is the end of the virtual memory before we
  // added space for arguments. This makes sure the running code won't
  // override our arguments when executing.
  currentThread->space->writeStackPointer(argPointerVirtualAddress);

  // copy arguments into newly allocated space
  for (int i = 0; i < argc; ++i) {
    DEBUG('e', "Writing string: %s\n", argv[i]);
    DEBUG('e', "Writing pointer %d to %d\n", argVirtualAddress, argPointerVirtualAddress);
    // write the pointer to the string
    writeMemTryTwice(argPointerVirtualAddress, 4, argVirtualAddress);
    
    // write the current argument string
    for (unsigned int j = 0; j < strlen(argv[i])+1; ++j) {
      DEBUG('e', "'%c' -> %d\n", argv[i][j], argVirtualAddress);
      writeMemTryTwice(argVirtualAddress, 1, argv[i][j]);
      argVirtualAddress++;
    }
    DEBUG('e', "\n");

    argPointerVirtualAddress += 4;
  }

  DEBUG('e', "Writing arguments: %d, %d\n", argc,
        pointerToThreadArgs->startOfArgPointersVirtualAddress);

  currentThread->WriteArguments(argc, pointerToThreadArgs->startOfArgPointersVirtualAddress);

  delete pointerToThreadArgs;
}

static void runUserCode(int pointerToThreadArgs) {
  if (currentThread == NULL) {
    fprintf(stderr, "ERROR in runUserCode, currentThread is NULL\n");
    return;
  }

  currentThread->space->InitRegisters();   // set the initial register values
  currentThread->space->RestoreState();    // load page table register

  writeArguments((ThreadArgs*)pointerToThreadArgs);

  machine->Run();     // jump to the user program
  ASSERT(FALSE);      // machine->Run never returns;
                      // the address space exits by doing the syscall "exit"
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
    readMemTryTwice(argInArrayVirtualAddress, 4, &argVirtualAddress);
    if (readString(argVirtualAddress, arrayBuffer[i]) == 0)
      return 0;
    argInArrayVirtualAddress += 4;
  }

  DEBUG('e', "Executing %s\n", arrayBuffer[0]);
  
  char* name = arrayBuffer[0];
  char** args = arrayBuffer;
  OpenFile *executable = fileSystem->Open(name);
  AddrSpace *space;
  SpaceId addressSpaceId;
  Thread *newProcess;

  bool isJoinable = willJoin & 0x1;
  bool isPipedWriter = willJoin & 0x2;
  bool isPipedReader = willJoin & 0x4;

  if (executable == NULL) {
    fprintf(stderr, "Unable to open file %s\n", name);
    return 0;
  }

  // create new address space
  space = new AddrSpace();

  // check if there is space available for the new process
  addressSpaceId = space->AllocateAndLoad(executable);
  if (addressSpaceId == 0) {
    DEBUG('e', "not enough memory for program\n");
    return 0;
  }

  newProcess = new Thread("exec'ed thread", isJoinable);
  newProcess->space = space;
  joinAndAddressSpacesTable.SetThread(addressSpaceId, newProcess);

  int spaceForArgPointers = argc * 4;

  int startOfArgPointersVirtualAddress = space->getEndVirtualAddress();
  int startOfArgsVirtualAddress = startOfArgPointersVirtualAddress + spaceForArgPointers;

  struct ThreadArgs* threadArguments = new struct ThreadArgs();
  threadArguments->argc = argc;
  threadArguments->argv = args;
  threadArguments->startOfArgPointersVirtualAddress = startOfArgPointersVirtualAddress;
  threadArguments->startOfArgsVirtualAddress = startOfArgsVirtualAddress;

  DEBUG('e', "Writing Args starting: %d\n", startOfArgsVirtualAddress);
  DEBUG('e', "Writing pointers starting: %d\n", startOfArgPointersVirtualAddress);
  // compute how many bytes we need to put all the arguments on the stack
  int additionalStackSpace = 0;
  // first add space for the actual array of character pointers
  additionalStackSpace += spaceForArgPointers;
  // now add space for each actual string
  for (int i = 0; i < argc; ++i) {
    additionalStackSpace += strlen(args[i]);
    // add another byte for NULL
    additionalStackSpace++;
  }

  DEBUG('e',"endVirtualMemory before expand: %d\n", space->getEndVirtualAddress());
  // ask address space for more space for arguments
  if (space->ExpandMemory(additionalStackSpace) == 0) {
    DEBUG('s', "could not allocate more memory for arguments\n");
    return 0;
  }
  DEBUG('e',"endVirtualMemory after expand: %d\n", space->getEndVirtualAddress());

  BoundedBuffer* pipelineWaitingWriter = currentThread->getPipelineWaitingWriter();
  if (isPipedReader) {
    if (pipelineWaitingWriter != NULL) {
      newProcess->setReadPipe(pipelineWaitingWriter);
      pipelineWaitingWriter->makePipe();
      currentThread->clearPipeline();
    }
  }

  if (isPipedWriter) {
    if (pipelineWaitingWriter != NULL)
      DEBUG('e', "creating a writer pipe when there's already one waiting\n");
    BoundedBuffer* newPipe = new BoundedBuffer();
    currentThread->setPipelineWaitingWriter(newPipe);
    newProcess->setWritePipe(newPipe);
  }

  newProcess->Fork(runUserCode, (int)threadArguments);

  return addressSpaceId;
}

static int kJoin(int spaceId) {
  const int JOIN_ERROR = -65535;
  Thread* thread_to_join;
  BoundedBuffer* pipelineWaitingWriter;

  if (joinAndAddressSpacesTable.isAllocated(spaceId) == false)
    return JOIN_ERROR;

  thread_to_join = joinAndAddressSpacesTable.GetThread(spaceId);
  if (thread_to_join == NULL)
    return JOIN_ERROR;
  if (thread_to_join == currentThread)
    return JOIN_ERROR;
  if (thread_to_join->isJoinable() == false)
    return JOIN_ERROR;
  
  joinAndAddressSpacesTable.SetThread(spaceId, NULL);
  pipelineWaitingWriter = currentThread->getPipelineWaitingWriter();
  if (pipelineWaitingWriter != NULL) {
    pipelineWaitingWriter->readerNotComing();
    currentThread->clearPipeline();
  }

  thread_to_join->Join();

  //Once the thread is done, get the return value
  int returnVal = joinAndAddressSpacesTable.GetIntValue(spaceId);
  joinAndAddressSpacesTable.Clear(spaceId);

  return returnVal;
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
  static const int RETURN_VALUE_REGISTER = 2;

  int type = machine->ReadRegister(2);
  int arg1 = machine->ReadRegister(4);
  int arg2 = machine->ReadRegister(5);
  int arg3 = machine->ReadRegister(6);
  int arg4 = machine->ReadRegister(7);
  int returnValue = 0;

  switch (which) {
    case SyscallException:
      switch (type) {
        case SC_Exec:
          returnValue = kExec(arg1, arg2, arg3, arg4);
          machine->WriteRegister(RETURN_VALUE_REGISTER, returnValue);
          break;
        case SC_Exit:
          kill(arg1);
          break;
        case SC_Halt:
          DEBUG('e', "Shutdown, initiated by user program.\n");
          interrupt->Halt();
          break;
        case SC_Join:
          returnValue = kJoin(arg1);
          machine->WriteRegister(RETURN_VALUE_REGISTER, returnValue);
          break;
        case SC_Read:
          returnValue = kRead(arg1, arg2, arg3);
          machine->WriteRegister(RETURN_VALUE_REGISTER, returnValue);
          break;
        case SC_Write:
          returnValue = kWrite(arg1, arg2, arg3);
          machine->WriteRegister(RETURN_VALUE_REGISTER, returnValue);
          break;
      }
      break;
    case PageFaultException:
      currentThread->space->loadPageIntoMemory(machine->registers[BadVAddrReg]);
      stats->numPageFaults++;
      break;
    case ReadOnlyException:
      fprintf(stderr, "Thread %d: Read Only Exception type %d\n",
              currentThread->space->getAddressSpaceId(), type);
      kill(which);
      break;
    case BusErrorException:
      fprintf(stderr, "Thread %d: Bus Error Exception type %d\n",
              currentThread->space->getAddressSpaceId(), type);
      kill(which);
      break;
    case AddressErrorException:
      fprintf(stderr, "Thread %d: Address Error Exception type %d\n",
              currentThread->space->getAddressSpaceId(), type);
      kill(which);
      break;
    case OverflowException:
      fprintf(stderr, "Thread %d: Overflow Exception type %d\n",
              currentThread->space->getAddressSpaceId(), type);
      kill(which);
      break;
    case IllegalInstrException:
      fprintf(stderr, "Thread %d: Illegal Exception type %d\n",
              currentThread->space->getAddressSpaceId(), type);
      kill(which);
      break;
    default:
      fprintf(stderr, "Thread %d: Unknown Exception type %d\n",
              currentThread->space->getAddressSpaceId(), type);
      kill(which);
      break;
  }
}
