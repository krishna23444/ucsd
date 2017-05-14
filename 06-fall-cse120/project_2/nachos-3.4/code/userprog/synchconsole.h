#ifndef SYNCHCONSOLE_H_
#define SYNCHCONSOLE_H_

class Console;
class Semaphore;
class Lock;

class SynchConsole
{
private:
    bool consoleHasBeenCreated;
    Console *console;
    Semaphore *readSemaphore;
    Semaphore *writeSemaphore;
    Lock *lock;

public:
    SynchConsole();
    ~SynchConsole();
    void WriteDone();
    void ReadAvail();
    int Write(char *buffer, int size);
    int Read(int size, char *buffer);
};

#endif /*SYNCHCONSOLE_H_*/
