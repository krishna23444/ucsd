#include "synchconsole.h"
#include "system.h"
#include "console.h"
#include "machine.h"
#include "addrspace.h"
#include "synch.h"

static void SynchConsoleReadAvail(int c) 
{ SynchConsole *sc = (SynchConsole *)c; sc->ReadAvail(); }
static void SynchConsoleWriteDone(int c)
{ SynchConsole *sc = (SynchConsole *)c; sc->WriteDone(); }



SynchConsole::SynchConsole(){
   consoleHasBeenCreated = false;
   readSemaphore = new Semaphore("read avail", 0);
   writeSemaphore = new Semaphore("write done", 1);
   lock = new Lock("synchConsole lock");
}

SynchConsole::~SynchConsole() {
  delete readSemaphore;
  delete writeSemaphore;
  delete lock;
  if (console != NULL)
    delete console;
}

int SynchConsole::Write(char *buffer, int size) {
  lock->Acquire();  
  if(consoleHasBeenCreated == false) {
      console = new Console(NULL, NULL,SynchConsoleReadAvail, SynchConsoleWriteDone, (int)this);
      consoleHasBeenCreated = true;
   }  
    // only one disk I/O at a time
  for (int i = 0; i < size; ++i) {
    writeSemaphore->Wait();      // wait for interrupt
    console->PutChar(buffer[i]);
  }
  lock->Release();
  return size;
}

int SynchConsole::Read(int size, char *buffer) {
  lock->Acquire();
  if(consoleHasBeenCreated == false) {
      console = new Console(NULL, NULL,SynchConsoleReadAvail, SynchConsoleWriteDone, (int)this);
      consoleHasBeenCreated = true;
  }
  int i;
  for (i = 0; i < size; i++) {
    readSemaphore->Wait();
    buffer[i] = console->GetChar();
    DEBUG('c', "Character read: %c\n", buffer[i]);
    if(buffer[i] == EOF)
      break;
  }
  lock->Release();
  return i;
}

void SynchConsole::ReadAvail() {
  readSemaphore->Signal(); 
}

void SynchConsole::WriteDone() {
  writeSemaphore->Signal(); 
}
