// threadtest.cc 
//  Simple test case for the threads assignment.
//
//  Create two threads, and have them context switch
//  back and forth between themselves by calling Thread::Yield, 
//  to illustratethe inner workings of the thread system.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "synch.h"
//#include <stdlib.h>
#include <time.h>

// testnum is set in main.cc
int testnum = 1;

// global variables used by most tests.
Lock *lock;
Semaphore *semaphore;
Condition *condition;
Mailbox *mailbox;

//----------------------------------------------------------------------
// SimpleThread
//   Loop 5 times, yielding the CPU to another ready thread 
//  each iteration.
//
//  "which" is simply a number identifying the thread, for debugging
//  purposes.
//----------------------------------------------------------------------
void SimpleThread(int which)
{
  int num;
  for (num = 0; num < 5; num++) {
    printf("*** thread %d looped %d times\n", which, num);
    currentThread->Yield();
  }
}

//----------------------------------------------------------------------
// ThreadTest1
//   Set up a ping-pong between two threads, by forking a thread 
//  to call SimpleThread, and then calling SimpleThread ourselves.
//----------------------------------------------------------------------
void ThreadTest1()
{
  DEBUG('t', "Entering ThreadTest1");
  Thread *t = new Thread("forked thread");
  t->Fork(SimpleThread, 1);
  SimpleThread(0);
}

//------------------------------------------------------------------------------
// Lock Tests 
//------------------------------------------------------------------------------

void LockAcquireTwiceShouldErrorTest() {
  lock = new Lock("testing lock");
  lock->Acquire();
  fprintf(stderr, ">> This test should now abort ../threads/synch.cc << \n");
  lock->Acquire();
  printf("TEST FAILED\n");
}

void ReleasingLockThatIsntHeldShouldErrorTest() {
  lock = new Lock("testing lock");
  fprintf(stderr, ">> This test should now abort in ../threads/synch.cc << \n");
  lock->Release();
  printf("TEST FAILED\n");
}

void DeletingLockThatIsBeingHeldShouldErrorTest() {
  lock = new Lock("testing lock");
  lock->Acquire();
  fprintf(stderr, ">> This test should now abort in ../threads/synch.cc <<\n");
  delete lock;
  printf("TEST FAILED\n");
}

void ThreadAcquireLockMethod(int notUsedParameter) {
  lock->Acquire();
}

void DeletingALockWhenThereAreThreadsWaitingToAcquireItTest() {
  lock = new Lock("testing lock");
  lock->Acquire();

  (new Thread("one"))->Fork(ThreadAcquireLockMethod, 0);
  (new Thread("two"))->Fork(ThreadAcquireLockMethod, 0);
  (new Thread("three"))->Fork(ThreadAcquireLockMethod, 0);

  currentThread->Yield();
  lock->Release();
  fprintf(stderr, ">> This test should now abort in ../threads/synch.cc <<\n");
  delete lock;
  printf("TEST FAILED\n");
}

void LockThread1(int param) {
  fprintf(stderr, "L1:0\n");
  lock->Acquire();
  fprintf(stderr, "L1:1\n");
  currentThread->Yield();
  fprintf(stderr, "L1:2\n");
  lock->Release();
  fprintf(stderr, "L1:3\n");
}

void LockThread2(int param) {
  fprintf(stderr, "L2:0\n");
  lock->Acquire();
  fprintf(stderr, "L2:1\n");
  currentThread->Yield();
  fprintf(stderr, "L2:2\n");
  lock->Release();
  fprintf(stderr, "L2:3\n");
}

void LockTestFromClassWebsite() {
  lock = new Lock("testing lock");
  (new Thread("one"))->Fork(LockThread1, 0);
  (new Thread("one"))->Fork(LockThread2, 0);
}

//----------------------------------------------------------------------
// ConditionVariableTest1
//   Wait on condition variable without having a lock.
//----------------------------------------------------------------------
//------------------------------------------------------------------------------
// Condition Variable Tests.
//------------------------------------------------------------------------------
void ConditionVariableTest1() {
  fprintf(stderr, ">> This test should now abort in ../threads/synch.cc\n");
  lock = new Lock("test lock");
  Condition *cv = new Condition("test cv");
  cv->Wait(lock);
  printf("TEST FAILED\n");
}

//----------------------------------------------------------------------
// ConditionVariableTest2
//  Waiting on a condition variable should free the associated lock.  
//----------------------------------------------------------------------
Condition *conditionVariable = new Condition("cv");

void ThreadAcquireLock(int param) {
  lock->Acquire();
  conditionVariable->Wait(lock);
}

void ConditionVariableTest2() {
  lock = new Lock("test lock");
  Thread *thread1 = new Thread("one");
  thread1->Fork(ThreadAcquireLock, 0);

  currentThread->Yield();
  lock->Acquire();
  printf("TEST PASSED\n");
}

//----------------------------------------------------------------------
// ConditionVariableTest3
//  Cannot signal a condition variable when you do not own the associated lock.
//----------------------------------------------------------------------
void ConditionVariableTest3() {
  lock = new Lock("test lock");
  fprintf(stderr, ">> This test should now abort in ../threads/synch.cc\n");
  conditionVariable->Signal(lock);
  printf("TEST FAILED\n");
}

//----------------------------------------------------------------------
// ConditionVariableTest4
//   Make sure Wait() actually waits... doesn't return until signaled.
//----------------------------------------------------------------------
bool signaled = false;

void ThreadWaiter(int param) {
  lock->Acquire();
  conditionVariable->Wait(lock);

  ASSERT(signaled);
  printf("TEST PASSED\n");
  lock->Release();
}

void SignalThread() {
  lock->Acquire();
  signaled = true;
  conditionVariable->Signal(lock);
  lock->Release();
}

void ConditionVariableTest4() {
  lock = new Lock("test lock");
  Thread *thread1 = new Thread("one");
  thread1->Fork(ThreadWaiter, 0);

  currentThread->Yield();
  SignalThread();
}

//----------------------------------------------------------------------
// ConditionVariableTest5
//   Make sure Broadcast() wakes up all the threads Waiting().
//----------------------------------------------------------------------
int count = 0;

void ConditionVariableTest5ThreadWaiter(int param) {
  lock->Acquire();
  conditionVariable->Wait(lock);
  count++;
  lock->Release();
}

void ConditionVariableTest5Broadcast() {
  lock->Acquire();
  signaled = true;
  conditionVariable->Broadcast(lock);
  lock->Release();
}

void ConditionVariableTest5() {
  lock = new Lock("test lock");
  Thread *thread1 = new Thread("one");
  thread1->Fork(ConditionVariableTest5ThreadWaiter, 0);
  Thread *thread2 = new Thread("two");
  thread2->Fork(ConditionVariableTest5ThreadWaiter, 0);
  Thread *thread3 = new Thread("three");
  thread3->Fork(ConditionVariableTest5ThreadWaiter, 0);
  Thread *thread4 = new Thread("four");
  thread4->Fork(ConditionVariableTest5ThreadWaiter, 0);

  currentThread->Yield();
  currentThread->Yield();
  currentThread->Yield();
  currentThread->Yield();
  currentThread->Yield();
  currentThread->Yield();
  ConditionVariableTest5Broadcast();
  currentThread->Yield();
  currentThread->Yield();
  currentThread->Yield();
  currentThread->Yield();
  currentThread->Yield();
  currentThread->Yield();

  ASSERT(count == 4);
  printf("TEST PASSED\n");
}

//----------------------------------------------------------------------
// ConditionVariableTest6
//   Make sure Signal() when no one is waiting does not do anything.
//----------------------------------------------------------------------
void ConditionVariableTest6ThreadWaiter(int param) {
  lock->Acquire();
  conditionVariable->Wait(lock);
  printf("TEST FAILED\n");
  lock->Release();
}

void ConditionVariableTest6ThreadSignal(int param) {
  lock->Acquire();
  conditionVariable->Signal(lock);
  lock->Release();
}

void ConditionVariableTest6() {
  lock = new Lock("test lock");
  Thread *thread1 = new Thread("one");
  thread1->Fork(ConditionVariableTest6ThreadSignal, 0);

  currentThread->Yield();

  Thread *thread2 = new Thread("two");
  thread2->Fork(ConditionVariableTest6ThreadWaiter, 0);

  currentThread->Yield();
  currentThread->Yield();
  printf("TEST PASSED\n");
}

//----------------------------------------------------------------------
// ConditionVariableTest7
//   Make sure Signal() only wakes up one Waiting() thread.
//----------------------------------------------------------------------
void ConditionVariableTest7ThreadWaiter(int param) {
  lock->Acquire();
  conditionVariable->Wait(lock);
  count++;
  lock->Release();
}

void ConditionVariableTest7Signal() {
  lock->Acquire();
  signaled = true;
  conditionVariable->Signal(lock);
  lock->Release();
}

void ConditionVariableTest7() {
  count = 0;
  lock = new Lock("test lock");
  Thread *thread1 = new Thread("one");
  thread1->Fork(ConditionVariableTest7ThreadWaiter, 0);
  Thread *thread2 = new Thread("two");
  thread2->Fork(ConditionVariableTest7ThreadWaiter, 0);
  Thread *thread3 = new Thread("three");
  thread3->Fork(ConditionVariableTest7ThreadWaiter, 0);
  Thread *thread4 = new Thread("four");
  thread4->Fork(ConditionVariableTest7ThreadWaiter, 0);

  currentThread->Yield();
  currentThread->Yield();
  currentThread->Yield();
  currentThread->Yield();
  currentThread->Yield();
  currentThread->Yield();
  ConditionVariableTest7Signal();
  currentThread->Yield();
  currentThread->Yield();
  currentThread->Yield();
  currentThread->Yield();
  currentThread->Yield();
  currentThread->Yield();

  ASSERT(count == 1);
  printf("TEST PASSED\n");
}

//----------------------------------------------------------------------
// MailboxTest1
//   Can't receive without sender
//----------------------------------------------------------------------
void MailboxReceiveUnanswered(int notUsed)
{
  int message;
  mailbox->Receive(&message);
  fprintf(stderr, "TEST FAILED\n");
  ASSERT(false);
}

void MailboxTest1()
{
  mailbox = new Mailbox();
  Thread *thread1 = new Thread("one");
  thread1->Fork(MailboxReceiveUnanswered, 0);

  currentThread->Yield();
  currentThread->Yield();
  currentThread->Yield();
  fprintf(stderr, "TEST PASSED\n");
}

//----------------------------------------------------------------------
// MailboxTest2
//   Can't send without receiver
//----------------------------------------------------------------------
void MailboxSendUnanswered(int message)
{
  mailbox->Send(message);
  fprintf(stderr, "TEST FAILED\n");
  ASSERT(false);
}

void MailboxTest2()
{
  mailbox = new Mailbox();
  Thread *thread1 = new Thread("one");
  thread1->Fork(MailboxSendUnanswered, 0);

  currentThread->Yield();
  currentThread->Yield();
  currentThread->Yield();
  fprintf(stderr, "TEST PASSED\n");
}

//----------------------------------------------------------------------

int mailboxReceiveCount = 0;
Lock mailboxReceiveLock("mailbox receive lock");
const int MAIL_VALUE = 42;

void MailboxReceive(int param) {
//  fprintf(stderr, "trying to receive mail\n");
  mailbox->Receive(&param);
//  fprintf(stderr, "mail received: %d\n", param);
  ASSERT(param == MAIL_VALUE);
  mailboxReceiveLock.Acquire();
  mailboxReceiveCount++;
  mailboxReceiveLock.Release();  
}

void MailboxSend(int param) {
//  fprintf(stderr, "sending mail: %d\n", param);
  mailbox->Send(MAIL_VALUE);
//  fprintf(stderr, "mail sent: %d\n", param);
}

void MailboxTest(int numReceivers, int numSenders, bool receiversFirst) {
  mailbox = new Mailbox();

  mailboxReceiveLock.Acquire();
  mailboxReceiveCount = 0;
  mailboxReceiveLock.Release();

  if (receiversFirst)
    for (int i = 0; i < numReceivers; ++i) {
//      fprintf(stderr, "forking receiver before sender\n");
      (new Thread("mailbox receiver"))->Fork(MailboxReceive, MAIL_VALUE);
      currentThread->Yield();
    }

  for (int i = 0; i < numSenders; ++i) {
//    fprintf(stderr, "forking sender\n");
    (new Thread("mailbox sender"))->Fork(MailboxSend, MAIL_VALUE);
    currentThread->Yield();
  }
  
  if (!receiversFirst)
    for (int i = 0; i < numReceivers; ++i) {
//      fprintf(stderr, "forking receiver after sender\n");
      (new Thread("mailbox receiver"))->Fork(MailboxReceive, MAIL_VALUE);
      currentThread->Yield();
    }

  for (int i = 0; i < 3 * (numReceivers + numSenders); ++i)
    currentThread->Yield();
}

void MailboxTestArray(int howManyOfEach[][2], int length) {
  for (int i = 0; i < length; ++i) {
    int numReceivers = howManyOfEach[i][0];
    int numSenders = howManyOfEach[i][1];

    MailboxTest(numReceivers, numSenders, false);
    mailboxReceiveLock.Acquire();
//    fprintf(stderr, "r = %d, s = %d\n", numReceivers, numSenders);
//    fprintf(stderr, "min = %d\n", min(numReceivers, numSenders));
//    fprintf(stderr, "count = %d\n", mailboxReceiveCount);
    ASSERT(mailboxReceiveCount == min(numReceivers, numSenders));
    fprintf(stderr, "%d,%d receivers first passed\n", numReceivers, numSenders);
    mailboxReceiveCount = 0;
    mailboxReceiveLock.Release();

    MailboxTest(numReceivers, numSenders, true);
    mailboxReceiveLock.Acquire();
    ASSERT(mailboxReceiveCount == min(numReceivers, numSenders));
    fprintf(stderr, "%d,%d receivers first passed\n", numReceivers, numSenders);
    mailboxReceiveCount = 0;
    mailboxReceiveLock.Release();
  }

  fprintf(stderr, "TEST PASSED\n");
}

void MailboxTestMany() {
  int values[][2] = {{1,1}, {2,2}, {5,3}, {4,12}};
  MailboxTestArray(values, sizeof(values)/sizeof(values[0]));
}

//
// MailboxSimpleTest
//

void MailboxTest3() {
  mailbox = new Mailbox();

  Thread *t = new Thread("b");
  t->Fork(MailboxSend, MAIL_VALUE);
  
  t = new Thread("b");
  t->Fork(MailboxReceive, MAIL_VALUE);

  (new Thread("b"))->Fork(MailboxSend, MAIL_VALUE);
  (new Thread("b"))->Fork(MailboxReceive, MAIL_VALUE);

  for (int i = 0; i < 20; ++i)
    currentThread->Yield();

  ASSERT(mailboxReceiveCount == 2);
  fprintf(stderr, "TEST PASSED\n");
}

//----------------------------------------------------------------------
// ThreadJoinTest1
//----------------------------------------------------------------------
void Joiner(Thread *joinee)
{
  printf("Waiting for the Joinee to finish executing.\n");

  // Note that, in this program, the "joinee" has not finished
  // when the "joiner" calls Join().  You will also need to handle
  // and test for the case when the "joinee" _has_ finished when
  // the "joiner" calls Join().

  joinee->Join();

  printf("Joinee has finished executing, we can continue.\n");
}

void JoinerWithYield(Thread *joinee)
{
  currentThread->Yield();
  currentThread->Yield();

  printf("Waiting for the Joinee to finish executing.\n");

  currentThread->Yield();
  currentThread->Yield();

  // Note that, in this program, the "joinee" has not finished
  // when the "joiner" calls Join().  You will also need to handle
  // and test for the case when the "joinee" _has_ finished when
  // the "joiner" calls Join().

  joinee->Join();

  currentThread->Yield();
  currentThread->Yield();

  printf("Joinee has finished executing, we can continue.\n");

  currentThread->Yield();
  currentThread->Yield();
}

void Joinee()
{
  for (int i = 0; i < 5; i++)
    printf("Smell the roses.\n");

  printf("Done smelling the roses!\n");
}

void JoineeWithYield()
{
  for (int i = 0; i < 5; i++) {
    printf("Smell the roses.\n");
    //currentThread->Yield();
  }

  //currentThread->Yield();
  printf("Done smelling the roses!\n");
  //currentThread->Yield();
}

void ForkerThread()
{
  Thread *joiner = new Thread("joiner", 0);  // will not be joined
  Thread *joinee = new Thread("joinee", 1);  // WILL be joined

  // fork off the two threads and let them do their business
  joiner->Fork((VoidFunctionPtr) JoinerWithYield, (int) joinee);
  joinee->Fork((VoidFunctionPtr) JoineeWithYield, 0);

  // this thread is done and can go on its merry way
  printf("Forked off the joiner and joinee threads.\n");
}

void testRosesWithJoineeYield() {
  Thread *joiner = new Thread("joiner", 0);  // will not be joined
  Thread *joinee = new Thread("joinee", 1);  // WILL be joined

  // fork off the two threads and let them do their business
  joiner->Fork((VoidFunctionPtr) Joiner, (int) joinee);
  joinee->Fork((VoidFunctionPtr) JoineeWithYield, 0);

  // this thread is done and can go on its merry way
  printf("Forked off the joiner and joinee threads.\n");
}

void testRosesWithJoinerYield() {
  Thread *joiner = new Thread("joiner", 0);  // will not be joined
  Thread *joinee = new Thread("joinee", 1);  // WILL be joined

  // fork off the two threads and let them do their business
  joiner->Fork((VoidFunctionPtr) JoinerWithYield, (int) joinee);
  joinee->Fork((VoidFunctionPtr) Joinee, 0);

  // this thread is done and can go on its merry way
  printf("Forked off the joiner and joinee threads.\n");
}

//------------------------------------------------------------------------------
// Thread Join Tests
//------------------------------------------------------------------------------
Thread *thread;

void threadJoinSelf(int param) {
  fprintf(stderr, ">> This test should now abort in ../threads/thread.cc <<\n");
  currentThread->Join();
  fprintf(stderr, "TEST FAILED\n");
}

void testThreadDoesNotCallJoinOnItself() {
  thread = new Thread("thread expecting join", 1);
  thread->Fork(threadJoinSelf, 0);
}


void threadSpinLoop(int paramNotUsed) {
  while (true)
    currentThread->Yield();
}

void testJoinOnlyInvokedOnThreadsThatExpectIt() {
  thread = new Thread("thread not expecting join", 0);
  thread->Fork(threadSpinLoop, 0);

  fprintf(stderr, ">> This test should now abort in ../threads/thread.cc <<\n");
  thread->Join();
  fprintf(stderr, "TEST FAILED\n");
}


void testJoinOnlyCalledOnThreadThatHasForked() {
  thread = new Thread("thread not gonna fork", 1);
  fprintf(stderr, ">> This test should now abort in ../threads/thread.cc <<\n");
  thread->Join();
}


void ThreadYieldAndExit(int param) {
  currentThread->Yield();
  currentThread->Yield();
}

void testJoinCalledTwiceOnAThread() {
  thread = new Thread("thread gonna be joined twice", 1);
  thread->Fork(ThreadYieldAndExit, 0);

  thread->Join();
  currentThread->Yield();
  currentThread->Yield();
  currentThread->Yield();
  fprintf(stderr, "\nThe thread variable should have been deleted, meaning we");
  fprintf(stderr, "\nshould segfault next, but for some reason the finished\n");
  fprintf(stderr, "thread does not get deleted yet, so we should abort...\n\n");
  fprintf(stderr, ">> This test should now abort in ../threads/thread.cc <<\n");
  thread->Join();
  fprintf(stderr, "TEST FAILED\n");
}

//------------------------------------------------------------------------------
// Priority Scheduling Tests
//------------------------------------------------------------------------------
void yieldAndPrintPriority(int param)
{
  currentThread->Yield();
  currentThread->Yield();
  fprintf(stderr, "This is my priority: %d\n", currentThread->getPriority());
  currentThread->Yield();
  currentThread->Yield();
}

void printPriority(int param)
{
  fprintf(stderr, "This is my priority: %d\n", currentThread->getPriority());
}

void threadLoop(int paramNotUsed)
{
  while (true);
}

void testHighestPrioritiesRunFirst(int param)
{
  fprintf(stderr, "Should see priorities in decreasing order\n");
  if(param == 1)
  {
    currentThread->setPriority(200);
    thread = new Thread("thread p = 70", 0);
    thread->setPriority(70);
    thread->Fork(yieldAndPrintPriority, 0);
  
    Thread *thread1 = new Thread("thread p = 80", 0);
    thread1->setPriority(80);
    thread1->Fork(yieldAndPrintPriority, 0);
  
    Thread *thread2 = new Thread("thread p = 90", 0);
    thread2->setPriority(90);
    thread2->Fork(yieldAndPrintPriority, 0);
  
    Thread *thread3 = new Thread("thread p = 100", 0);
    thread3->setPriority(100);
    thread3->Fork(yieldAndPrintPriority, 0);
  }
  else if(param == 2)
  {
    currentThread->setPriority(200);
    thread = new Thread("thread p = 70", 0);
    thread->setPriority(70);
    thread->Fork(threadLoop, 0);
  
    Thread *thread1 = new Thread("thread p = 80", 0);
    thread1->setPriority(80);
    thread1->Fork(printPriority, 0);
  }
}

void threadAcquireLock(int param) {
  lock->Acquire();
  fprintf(stderr, "thread with priority %d acquired lock\n", currentThread->getPriority());
  currentThread->Yield();
  currentThread->Yield();
  currentThread->Yield();
  currentThread->Yield();
  lock->Release();
}

void threadAcquireSemaphore(int param) {
  semaphore->Wait();
  fprintf(stderr, "thread with priority %d got semaphore\n", currentThread->getPriority());
  currentThread->Yield();
  currentThread->Yield();
  currentThread->Yield();
  currentThread->Yield();
  semaphore->Signal();
}

void testHigherPriorityWithSynchronizationVariables(int param) {
  lock = new Lock("test lock");
  semaphore = new Semaphore("test semaphore", 1);
  condition = new Condition("test condition");
  Thread *t1 = new Thread("thread priority ten");
  t1->setPriority(10);
  Thread *t2 = new Thread("thread priority twenty");
  t2->setPriority(20);
  Thread *t3 = new Thread("thread priority forty");
  t3->setPriority(40);
  Thread *t4 = new Thread("thread priority thirty");
  t4->setPriority(30);

  switch (param) {
    case 0:
      fprintf(stderr, "NOTE: priority 10 should get the lock, after that\n");
      fprintf(stderr, "should be sorted in decending order by priority\n\n");
      t1->Fork(threadAcquireLock, 0);
      t2->Fork(threadAcquireLock, 0);
      t3->Fork(threadAcquireLock, 0);
      t4->Fork(threadAcquireLock, 0);
      break;
    case 1:
      fprintf(stderr, "NOTE: priority 10 should get semaphore, after that\n");
      fprintf(stderr, "should be sorted in decending order by priority\n\n");
      t1->Fork(threadAcquireSemaphore, 0);
      t2->Fork(threadAcquireSemaphore, 0);
      t3->Fork(threadAcquireSemaphore, 0);
      t4->Fork(threadAcquireSemaphore, 0);
      break;
  }
}

void threadAcquireLockPrintPriority(int expectedPriority)
{
  lock->Acquire();
  while (true) {
    currentThread->Yield();
    if (currentThread->getPriority() == expectedPriority)
      break;
  }
  lock->Release();
}

void testLockInversion()
{
  int invertedPriorityValue = 30;
  lock = new Lock("test lock");
  fprintf(stderr, "\n");
  thread = new Thread("foo");
  thread->setPriority(10);
  thread->Fork(threadAcquireLockPrintPriority, invertedPriorityValue);
  
  currentThread->Yield();
  currentThread->Yield();
  currentThread->Yield();
  currentThread->setPriority(invertedPriorityValue);
  
  fprintf(stderr, "NOTE: this test should finish quickly if it succeeds,\n");
  fprintf(stderr, "  and you should receive a nice test passed message\n");
  lock->Acquire();
  fprintf(stderr, "TEST PASSED\n");
}

void threadSpinUntilReachPriority(int expectedPriority) {
  while (true) {
    currentThread->Yield();
    fprintf(stderr, "p = %d\n", currentThread->getPriority());
    if (currentThread->getPriority() == expectedPriority)
      break;
  }
}

void threadJoiner(int param) {
  currentThread->Yield();
  thread->Join();
}

void testJoinInversion()
{
  int expectedPriority = 30;
  thread = new Thread("thread spin until reach priority", 1);
  thread->Fork(threadSpinUntilReachPriority, expectedPriority);
  
  currentThread->Yield();
  currentThread->Yield();
  
  Thread *t = new Thread("joiner with low priority");
  t->setPriority(20);
  t->Fork(threadJoiner, 0);
  
  currentThread->Yield();
  currentThread->Yield();
  
  currentThread->setPriority(expectedPriority);
  thread->Join();
  fprintf(stderr, "TEST PASSED\n");
}

//------------------------------------------------------------------------------
// Whale fornication functions. (No support for homosexuality)
//------------------------------------------------------------------------------
Semaphore maleWhaleSemaphore("maleWhaleSemaphore", 0);
Semaphore femaleWhaleSemaphore("femaleWhaleSemaphore", 0);
Semaphore matchmakerMaleSemaphore("matchmakerMaleSemaphore", 0);
Semaphore matchmakerFemaleSemaphore("matchmakerFemaleSemaphore", 0);
Semaphore matchedWhalePrintSemaphore("matchedWhalePrintSemaphore", 0);

Lock maleLock("maleLock");
Lock femaleLock("femaleLock");
Lock matchLock("matchmakerLock");

void Male()
{
  maleWhaleSemaphore.Signal();
  //fprintf(stderr, "Male signaled, waiting for lock\n");
  maleLock.Acquire();
  matchmakerMaleSemaphore.Wait();
  fprintf(stderr, "Male Id: %p\n", currentThread);
  matchedWhalePrintSemaphore.Signal();
  maleLock.Release();
}

void Female()
{
  femaleWhaleSemaphore.Signal();
  //fprintf(stderr, "Female signaled, waiting for lock\n");
  femaleLock.Acquire();
  matchmakerFemaleSemaphore.Wait();
  fprintf(stderr, "Female Id: %p\n", currentThread);
  matchedWhalePrintSemaphore.Signal();
  femaleLock.Release();
}

void Matchmaker()
{
  maleWhaleSemaphore.Wait();
  femaleWhaleSemaphore.Wait();
  //fprintf(stderr, "Matchmaker signaled by male and female, waiting for lock\n");
  matchLock.Acquire();
  fprintf(stderr, "I, Matchmaker, have found two whales.\nMatchmaker ID: %p\n", currentThread);
  matchmakerMaleSemaphore.Signal();
  matchmakerFemaleSemaphore.Signal();
  matchedWhalePrintSemaphore.Wait();
  matchedWhalePrintSemaphore.Wait();
  matchLock.Release();
}

int malesReturned = 0,
    femalesReturned = 0,
    matchmakersReturned = 0;

void MaleFork(int howMany)
{
  while (howMany-- > 0) {
    Male();
    malesReturned++;
    currentThread->Yield();
  }
}
void FemaleFork(int howMany)
{
  while (howMany-- > 0) {
    Female();
    femalesReturned++;
    currentThread->Yield();
  }
}
void MatchmakerFork(int howMany)
{
  while (howMany-- > 0) {
    Matchmaker();
    matchmakersReturned++;
    currentThread->Yield();
  }
}

void WhaleBasicTest1()
{
  (new Thread("whale"))->Fork(MaleFork, 1);
  (new Thread("whale"))->Fork(MatchmakerFork, 1);
  (new Thread("whale"))->Fork(FemaleFork, 1);

  for (int i = 0; i < 3; i++)
    currentThread->Yield();

  fprintf(stderr, "Should have made one match by now\n");

}

void whaleFornication1()
{
  Thread *thread1 = new Thread("male1");
  thread1->Fork(MaleFork, 1);
  Thread *thread2 = new Thread("matchmaker1");
  thread2->Fork(MatchmakerFork, 1);
  Thread *thread3 = new Thread("female1");
  thread3->Fork(FemaleFork, 1);
  Thread *thread4 = new Thread("male1");
  thread4->Fork(MaleFork, 1);
  Thread *thread5 = new Thread("female1");
  thread5->Fork(FemaleFork, 1);
  Thread *thread6 = new Thread("male2");
  thread6->Fork(MaleFork, 1);
  Thread *thread7 = new Thread("matchmaker1");
  thread7->Fork(MatchmakerFork, 1);
  Thread *thread8 = new Thread("female1");
  thread8->Fork(FemaleFork, 1);
  Thread *thread9 = new Thread("matchmaker1");
  thread9->Fork(MatchmakerFork, 1);
  Thread *thread10 = new Thread("male1");
  thread10->Fork(MaleFork, 1);
  Thread *thread11 = new Thread("male1");
  thread11->Fork(MaleFork, 1);
  Thread *thread12 = new Thread("female1");
  thread12->Fork(FemaleFork, 1);
  Thread *thread13 = new Thread("matchmaker1");
  thread13->Fork(MatchmakerFork, 1);
  Thread *thread14 = new Thread("female1");
  thread14->Fork(FemaleFork, 1);
  Thread *thread15 = new Thread("matchmaker2");
  thread15->Fork(MatchmakerFork, 1);
  Thread *thread16 = new Thread("male1");
  thread16->Fork(MaleFork, 1);
  Thread *thread17 = new Thread("female2");
  thread17->Fork(FemaleFork, 1);
  Thread *thread18 = new Thread("matchmaker1");
  thread18->Fork(MatchmakerFork, 1);
  Thread *thread19 = new Thread("female1");
  thread19->Fork(FemaleFork, 1);
  
  //fprintf(stderr, "TEST PASSED\n");
}

void whaleFornicationBroken()
{
  int males = 0, females = 0, matchmakers = 0;
  int seconds = 0;
  int random = 0;
  int totalwhales = 97;
  
  int input[] = {0, 1, 0, 1, 0, 3, 1, 0, 1, 1, 0, 1, 2, 2, 0, 3, 3, 0, \
                 2, 0, 0, 1, 0, 3, 1, 2, 0, 0, 2, 1, 1, 2, 2, 1, 0, 2, 0, 1, \
                 2, 2, 2, 2, 3, 3, 3, 3, 3, 3};

  for (int whale_count = 0; whale_count < totalwhales; whale_count++)
  {
    random = input[whale_count % 45];

    if (random == 0) {
      (new Thread("whale"))->Fork(MaleFork, 1);
      males++;
    }
    if (random == 1) {
      (new Thread("whale"))->Fork(MatchmakerFork, 1);
      matchmakers++;
    }
    if (random == 2) {
      (new Thread("whale"))->Fork(FemaleFork, 1);
      females++;
    }
    if (random == 3) {
      for (int i = 0; i < 250; i++)
        currentThread->Yield();

      ASSERT((malesReturned == femalesReturned) &&
             (malesReturned == matchmakersReturned) &&
             (malesReturned == min(min(males, females), matchmakers)));
    }
  }

  for (int i = 0; i < 25; i++)
    currentThread->Yield();

  bool passed1 = (malesReturned == femalesReturned) &&
         (malesReturned == matchmakersReturned) &&
         (malesReturned == min(min(males, females), matchmakers));

  if (passed1)
    fprintf(stderr, "\nPhase 1 passed! Now match them up!\n\n");
  else
  {
    fprintf(stderr, "\nTEST FAILED!\n");
    return;
  }

  int matches = max(max(males, females), matchmakers);

  while (males < matches) {
    (new Thread("whale"))->Fork(MaleFork, 1);
    males++;
  }
  while (matchmakers < matches) {
    (new Thread("whale"))->Fork(MatchmakerFork, 1);
    matchmakers++;
  }
  while (females < matches) {
    (new Thread("whale"))->Fork(FemaleFork, 1);
    females++;
  }

  for (int i = 0; i < 4*totalwhales; i++)
    currentThread->Yield();

  bool passed2 = (malesReturned == femalesReturned) &&
         (malesReturned == matchmakersReturned) &&
         (malesReturned == min(min(males, females), matchmakers)) &&
         (malesReturned == matches);

  if (passed2)
    fprintf(stderr, "\nTEST PASSED!\n");
  else
    fprintf(stderr, "\nTEST FAILED!  The matching up didn't work.\n");
}

void whaleFornication2(int totalwhales)
{
  int males = 0, females = 0, matchmakers = 0;
  int seconds = 0;
  int random = 0;
  
  for (int whale_count = 0; whale_count < totalwhales; whale_count++)
  {
    if (seconds == 0 || whale_count % 16 == 15)
      seconds = (int)time(NULL);

    random = seconds % 4;
    seconds = seconds >> 2;

    fprintf(stderr, "Seconds: %d\t Rand: %d\n", seconds, random);

    if (random == 0) {
      (new Thread("whale"))->Fork(MaleFork, 1);
      males++;
    }
    if (random == 1) {
      (new Thread("whale"))->Fork(MatchmakerFork, 1);
      matchmakers++;
    }
    if (random == 2) {
      (new Thread("whale"))->Fork(FemaleFork, 1);
      females++;
    }
    if (random == 3) {
      for (int i = 0; i < 25; i++)
        currentThread->Yield();

      ASSERT((malesReturned == femalesReturned) &&
             (malesReturned == matchmakersReturned) &&
             (malesReturned == min(min(males, females), matchmakers)));
    }
  }

  for (int i = 0; i < 25; i++)
    currentThread->Yield();

  bool passed1 = (malesReturned == femalesReturned) &&
         (malesReturned == matchmakersReturned) &&
         (malesReturned == min(min(males, females), matchmakers));

  if (passed1)
    fprintf(stderr, "\nPhase 1 passed! Now match them up!\n\n");
  else
  {
    fprintf(stderr, "\nTEST FAILED!\n");
    return;
  }

  int matches = max(max(males, females), matchmakers);

  while (males < matches) {
    (new Thread("whale"))->Fork(MaleFork, 1);
    males++;
  }
  while (matchmakers < matches) {
    (new Thread("whale"))->Fork(MatchmakerFork, 1);
    matchmakers++;
  }
  while (females < matches) {
    (new Thread("whale"))->Fork(FemaleFork, 1);
    females++;
  }

  for (int i = 0; i < 4*totalwhales; i++)
    currentThread->Yield();

  bool passed2 = (malesReturned == femalesReturned) &&
         (malesReturned == matchmakersReturned) &&
         (malesReturned == min(min(males, females), matchmakers)) &&
         (malesReturned == matches);

  if (passed2)
    fprintf(stderr, "\nTEST PASSED!\n");
  else
    fprintf(stderr, "\nTEST FAILED!  The matching up didn't work.\n");
}

//----------------------------------------------------------------------
// ThreadTest
//   Invoke a test routine.
//----------------------------------------------------------------------
void ThreadTest() {
  switch (testnum) {
  case 0:
    break;
  case 1:
    ThreadTest1();
    break;
  case 2:
    LockAcquireTwiceShouldErrorTest();
    break;
  case 3:
    ReleasingLockThatIsntHeldShouldErrorTest();
    break;
  case 4:
    DeletingLockThatIsBeingHeldShouldErrorTest();
    break;
  case 5:
    DeletingALockWhenThereAreThreadsWaitingToAcquireItTest();
    break;
  case 6:
    LockTestFromClassWebsite();
    break;
  case 7:
    ConditionVariableTest1();
    break;
  case 8:
    ConditionVariableTest2();
    break;
  case 9:
    ConditionVariableTest3();
    break;
  case 10:
    ConditionVariableTest4();
    break;
  case 11:
    ConditionVariableTest5();
    break;
  case 12:
    ConditionVariableTest6();
    break;
  case 13:
    ConditionVariableTest7();
    break;
  case 14:
    MailboxTest1();
    break;
  case 15:
    MailboxTest2();
    break;
  case 16:
    ForkerThread();
    break;
  case 17:
    MailboxTestMany();
    break;
  case 18:
    MailboxTest3();
    break;
  case 19:
    WhaleBasicTest1();
    break;
  case 20:
    whaleFornication1();
    break;
  case 21:
    whaleFornication2(100);
    break;
  case 22:
    testRosesWithJoinerYield();
    break;
  case 23:
    testRosesWithJoineeYield();
    break;
  case 24:
    testThreadDoesNotCallJoinOnItself();
    break;
  case 25:
    testJoinOnlyInvokedOnThreadsThatExpectIt();
    break;
  case 26:
    testJoinOnlyCalledOnThreadThatHasForked();
    break;
  case 27:
    testJoinCalledTwiceOnAThread();
    break;
  case 28:
    testHighestPrioritiesRunFirst(1);
    break;
  case 29:
    testHigherPriorityWithSynchronizationVariables(0);
    break;
  case 30:
    testHigherPriorityWithSynchronizationVariables(1);
    break;
  case 31:
    testLockInversion();
    break;
  case 32:
    testJoinInversion();
    break;
  case 33:
    whaleFornicationBroken();
    break;
  default:
    printf("No test specified.\n");
    break;
  }
}

