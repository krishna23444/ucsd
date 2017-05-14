// synch.cc 
//  Routines for synchronizing threads.  Three kinds of
//  synchronization routines are defined here: semaphores, locks 
//     and condition variables (the implementation of the last two
//  are left to the reader).
//
// Any implementation of a synchronization routine needs some
// primitive atomic operation.  We assume Nachos is running on
// a uniprocessor, and thus atomicity can be provided by
// turning off interrupts.  While interrupts are disabled, no
// context switch can occur, and thus the current thread is guaranteed
// to hold the CPU throughout, until interrupts are reenabled.
//
// Because some of these routines might be called with interrupts
// already disabled (Semaphore::V for one), instead of turning
// on interrupts at the end of the atomic operation, we always simply
// re-set the interrupt state back to its original value (whether
// that be disabled or enabled).
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "synch.h"
#include "system.h"

//----------------------------------------------------------------------
// Semaphore::Semaphore
//   Initialize a semaphore, so that it can be used for synchronization.
//
//  "debugName" is an arbitrary name, useful for debugging.
//  "initialValue" is the initial value of the semaphore.
//----------------------------------------------------------------------

Semaphore::Semaphore(char* debugName, int initialValue)
{
    name = debugName;
    value = initialValue;
    queue = new List;
}

//----------------------------------------------------------------------
// Semaphore::Semaphore
//   De-allocate semaphore, when no longer needed.  Assume no one
//  is still waiting on the semaphore!
//----------------------------------------------------------------------

Semaphore::~Semaphore()
{
    delete queue;
}

//----------------------------------------------------------------------
// Semaphore::P
//   Wait until semaphore value > 0, then decrement.  Checking the
//  value and decrementing must be done atomically, so we
//  need to disable interrupts before checking the value.
//
//  Note that Thread::Sleep assumes that interrupts are disabled
//  when it is called.
//----------------------------------------------------------------------

void Semaphore::P()
{
  IntStatus oldLevel = interrupt->SetLevel(IntOff);  // disable interrupts

  while (value == 0) {       // semaphore not available
    // so go to sleep
    queue->SortedInsert((void *)currentThread, currentThread->getPriority());
    currentThread->Sleep();
  } 
  value--;           // semaphore available, consume its value

  (void) interrupt->SetLevel(oldLevel);  // re-enable interrupts
}

void Semaphore::Wait() {
  P();
}

//----------------------------------------------------------------------
// Semaphore::V
//   Increment semaphore value, waking up a waiter if necessary.
//  As with P(), this operation must be atomic, so we need to disable
//  interrupts.  Scheduler::ReadyToRun() assumes that threads
//  are disabled when it is called.
//----------------------------------------------------------------------

void Semaphore::V()
{
    Thread *thread;
    int priority;
    IntStatus oldLevel = interrupt->SetLevel(IntOff);

    thread = (Thread *)queue->SortedRemove(&priority);
    
    if (thread != NULL)     // make thread ready, consuming the V immediately
      scheduler->ReadyToRun(thread);
    value++;
    (void) interrupt->SetLevel(oldLevel);
}

void Semaphore::Signal() {
  V();
}

// Dummy functions -- so we can compile our later assignments 
// Note -- without a correct implementation of Condition::Wait(), 
// the test case in the network assignment won't work!
Lock::Lock(char* debugName) {
  name = debugName;
  held = false;
  wait_queue = new List();
  threadHoldingLock = NULL;
}

// Destructor for lock.
// Free memory taken up by the wait queue.
Lock::~Lock() {
  ASSERT(!held && wait_queue->IsEmpty());
  delete wait_queue;
}

//
void Lock::Acquire() {
  // check if the current thread already holds the lock.
  ASSERT(!isHeldByCurrentThread());

  // disable interrupts
  IntStatus oldLevel = interrupt->SetLevel(IntOff);

  while (held) {
    if (threadHoldingLock->getPriority() < currentThread->getPriority())
      threadHoldingLock->setPriority(currentThread->getPriority());
    wait_queue->SortedInsert(currentThread, currentThread->getPriority());
    currentThread->Sleep();
  }

  held = true;
  threadHoldingLock = currentThread;
  
  // set old priority
  threadHoldingLockPriority = currentThread->getPriority();

  // enable interrupt
  (void) interrupt->SetLevel(oldLevel);
}

//
void Lock::Release() {
  IntStatus oldLevel = interrupt->SetLevel(IntOff);
  int priority;
  Thread *waiting_thread;

  // make sure the current thread own the lock.
  ASSERT(isHeldByCurrentThread());

  // wake up a thread that is waiting on the lock.
  if (!wait_queue->IsEmpty()) {
    waiting_thread = (Thread *)wait_queue->SortedRemove(&priority);
    scheduler->ReadyToRun(waiting_thread);
  }
  
  currentThread->setPriority(threadHoldingLockPriority);

  held = false;
  threadHoldingLock = NULL;

  (void) interrupt->SetLevel(oldLevel);
}

// Check if current thread is holding the lock.
//
// Returns:
//   true if lock is being held by the current running thread.
bool Lock::isHeldByCurrentThread() {
  return held && threadHoldingLock == currentThread;
}


Condition::Condition(char* debugName) {
  name = debugName;
  threadsWaitingToBeSignaledQueue = new List();
}

Condition::~Condition() { }

//
void Condition::Wait(Lock* conditionLock) {
  IntStatus oldLevel = interrupt->SetLevel(IntOff);

  // Make sure the current thread holds the lock.
  ASSERT(conditionLock->isHeldByCurrentThread());

  // Release the lock.
  conditionLock->Release();

  // Add to wait queue and wait for signal.
  threadsWaitingToBeSignaledQueue->SortedInsert(currentThread,
                                                currentThread->getPriority());

  // Relinquish the CPU until we're signaled.
  currentThread->Sleep();

  // Re-acquire the lock after return.
  conditionLock->Acquire();

  (void) interrupt->SetLevel(oldLevel);
}

void Condition::Signal(Lock* conditionLock) {
  int priority;
  ASSERT(conditionLock->isHeldByCurrentThread());

  if (!threadsWaitingToBeSignaledQueue->IsEmpty()) 
    scheduler->ReadyToRun((Thread *)threadsWaitingToBeSignaledQueue->SortedRemove(&priority));
}

void Condition::Broadcast(Lock* conditionLock) {
  int priority;
  ASSERT(conditionLock->isHeldByCurrentThread());

  while (!threadsWaitingToBeSignaledQueue->IsEmpty())
    scheduler->ReadyToRun((Thread *)threadsWaitingToBeSignaledQueue->SortedRemove(&priority));
}

// Constructor for mailbox
Mailbox::Mailbox() {
  message = 0;
  hasMessage = false;
  receiverLock = new Lock("mailbox receiver lock");
  senderLock = new Lock("mailbox sender lock");
  senderSemaphore = new Semaphore("mailbox sender semaphore", 0);
  receiverSemaphore = new Semaphore("mailbox receiver semaphore", 0);
}

// Reclaim memory taken up by associated locks and semaphores.
Mailbox::~Mailbox() {
  delete receiverLock;
  delete senderLock;
  delete receiverSemaphore;
  delete senderSemaphore;
}

// Atomically wait until another thread calls Receive, and copy the message into
// their space.
//
// Args:
// - newMessage: integer, message to send.
//
void Mailbox::Send(int newMessage)
{
  senderLock->Acquire();

  receiverSemaphore->Wait();

  // Send the message
  message = newMessage;
  hasMessage = true;

  senderSemaphore->Signal();
  
  senderLock->Release();
}

// Atomically wait until another thread calls Send, and copy their message into
// the pointer passed in.
//
// Args:
// - newMessage: pointer to space to copy message into.
//
void Mailbox::Receive(int *newMessage)
{
  receiverLock->Acquire();

  receiverSemaphore->Signal();
  senderSemaphore->Wait();

  // Receive the message
  *newMessage = message;
  hasMessage = false;

  receiverLock->Release();
}
