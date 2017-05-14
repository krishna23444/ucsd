#include "boundedbuffer.h"
#include "system.h"
#include "syscall.h"
#include "synch.h"

struct ThreadArgs {
  int startOfArgPointersVirtualAddress;
  int startOfArgsVirtualAddress;
  int argc;
  char **argv;
};

//
void runUserCode(int pointerToThreadArgs) {
  if (currentThread != NULL) {
    currentThread->space->InitRegisters();   // set the initial register values
    currentThread->space->RestoreState();    // load page table register

    struct ThreadArgs *threadArgumentsStruct = (ThreadArgs*)pointerToThreadArgs;
    int argPointerVirtualAddress = threadArgumentsStruct->startOfArgPointersVirtualAddress;
    int argVirtualAddress = threadArgumentsStruct->startOfArgsVirtualAddress;
    int argc = threadArgumentsStruct->argc;
    char **argv = threadArgumentsStruct->argv;

    // Since we append the arguments to main at the end of the virtual space,
    // when InitRegisters sets the stack pointer, the code itself will end up
    // corrupting our arguments. To solve this, we set the stack pointer where
    // the old stack ended, that is the end of the virtual memory before we
    // added space for arguments. This makes sure the running code won't
    // override our arguments when executing.
    currentThread->space->writeStackPointer(argPointerVirtualAddress);

    // copy arguments into newly allocated space
    for (int i = 0; i < argc; ++i) {
      DEBUG('s', "Writing string: %s\n", argv[i]);
      DEBUG('s', "Writing pointer %d to %d\n", argVirtualAddress, argPointerVirtualAddress);
      // write the pointer to the string
      machine->WriteMem(argPointerVirtualAddress, 4, argVirtualAddress);
      
      // write the current argument string
      for (unsigned int j = 0; j < strlen(argv[i])+1; ++j) {
        DEBUG('s', "'%c' -> %d\n", argv[i][j], argVirtualAddress);
        machine->WriteMem(argVirtualAddress, 1, argv[i][j]);
        argVirtualAddress++;
      }
      DEBUG('s', "\n");
  
      argPointerVirtualAddress += 4;
    }

    DEBUG('s', "Writing arguments: %d, %d\n", argc,
          threadArgumentsStruct->startOfArgPointersVirtualAddress);

    currentThread->SaveArgs(argc, threadArgumentsStruct->startOfArgPointersVirtualAddress);
    currentThread->RestoreArgs();
    
    delete threadArgumentsStruct;

    machine->Run();     // jump to the user program
    ASSERT(FALSE);      // machine->Run never returns;
                        // the address space exits by doing the syscall "exit"
  }
}

//
SpaceId Exec(char *name, int argc, char **argv, int willJoin) {
  OpenFile *executable = fileSystem->Open(name);
  AddrSpace *space;
  SpaceId addressSpaceId;
  Thread *newProcess;

  bool isJoinable = willJoin & 0x1;
  bool isPipedWriter = willJoin & 0x2;
  bool isPipedReader = willJoin & 0x4;

  if (executable == NULL) {
    printf("Unable to open file %s\n", name);
    return 0;
  }

  // create new address space
  space = new AddrSpace();

  // check if there is space available for the new process
  addressSpaceId = space->AllocateAndLoad(executable);
  if (addressSpaceId == 0) {
    DEBUG('s', "not enough memory for program\n");
    return 0;
  }

  delete executable;      // close file

  newProcess = new Thread("exec'ed thread", isJoinable);
  newProcess->space = space;
  joinAndAddressSpacesTable.SetThread(addressSpaceId, newProcess);

  int spaceForArgPointers = argc * 4;

  int startOfArgPointersVirtualAddress = space->getEndVirtualAddress();
  int startOfArgsVirtualAddress = startOfArgPointersVirtualAddress + spaceForArgPointers;

  struct ThreadArgs* threadArguments = new struct ThreadArgs();
  threadArguments->argc = argc;
  threadArguments->argv = argv;
  threadArguments->startOfArgPointersVirtualAddress = startOfArgPointersVirtualAddress;
  threadArguments->startOfArgsVirtualAddress = startOfArgsVirtualAddress;

  DEBUG('s', "Writing Args starting: %d\n", startOfArgsVirtualAddress);
  DEBUG('s', "Writing pointers starting: %d\n", startOfArgPointersVirtualAddress);
  // compute how many bytes we need to put all the arguments on the stack
  int additionalStackSpace = 0;
  // first add space for the actual array of character pointers
  additionalStackSpace += spaceForArgPointers;
  // now add space for each actual string
  for (int i = 0; i < argc; ++i) {
    additionalStackSpace += strlen(argv[i]);
    // add another byte for NULL
    additionalStackSpace++;
  }

  DEBUG('s',"endVirtualMemory before expand: %d\n", space->getEndVirtualAddress());
  // ask address space for more space for arguments
  if (space->ExpandMemory(additionalStackSpace) == 0) {
    DEBUG('s', "could not allocate more memory for arguments\n");
    return 0;
  }
  DEBUG('s',"endVirtualMemory after expand: %d\n", space->getEndVirtualAddress());

  BoundedBuffer* pipelineWaitingWriter = currentThread->getPipelineWaitingWriter();
  if (isPipedReader) {
    if (pipelineWaitingWriter != NULL) {
      newProcess->setReadPipe(pipelineWaitingWriter);
      pipelineWaitingWriter->makePipe();
      currentThread->clearPipeline();
    }
  }

  if (isPipedWriter) {
    if (pipelineWaitingWriter != NULL) {
      DEBUG('s', "creating a writer pipe when there's already one waiting\n");
    }
    BoundedBuffer* newPipe = new BoundedBuffer();
    currentThread->setPipelineWaitingWriter(newPipe);
    newProcess->setWritePipe(newPipe);
  }

  newProcess->Fork(runUserCode, (int)threadArguments);

  return addressSpaceId;
}
