/* 
 * BufferedBlockManager.cc : part of the Mace toolkit for building distributed systems
 * 
 * Copyright (c) 2006, Charles Killian, Dejan Kostic, Ryan Braud, James W. Anderson, John Fisher-Ogden, Calvin Hubble, Duy Nguyen, Justin Burke, David Oppenheimer, Amin Vahdat, Adolfo Rodriguez, Sooraj Bhat
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in
 *      the documentation and/or other materials provided with the
 *      distribution.
 *    * Neither the names of Duke University nor The University of
 *      California, San Diego, nor the names of the authors or contributors
 *      may be used to endorse or promote products derived from
 *      this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * ----END-OF-LEGAL-STUFF---- */
#include <assert.h>
#include <inttypes.h>
#include "lib/Log.h"
#include "BufferedBlockManager.h"
#include "ThreadCreate.h"

using namespace std;

BufferedBlockManager::BufferedBlockManager(FileBlockManager& m, uint capacity)
  : fbm(&m), bufferCache(capacity), isOpenFlag(false), 
    processQueuesFlag(false) {
  
} // BufferedBlockManager

BufferedBlockManager::~BufferedBlockManager() {
  close();
  pthread_cond_destroy(&queueCond);
  pthread_mutex_destroy(&queueLock);
} // ~BufferedBlockManager

void BufferedBlockManager::prefetchBlock(uint64_t index) {
  Log::logf("BufferedBlockManager::prefetchBlock", 2, "enqueuing %" PRIu64 
	    "\n", index);
  acquireQueueLock();
  readQueue.push_back(index);
  signalQueueData();
  releaseQueueLock();
} // prefetchBlock

string BufferedBlockManager::getBlock(uint64_t index) {
  assert(isOpen());
  Log::logf("BufferedBlockManager::getBlock", 2, "index=%" PRIu64 "\n", index);
  acquireQueueLock();
  readIntoCache(index);
  string tmp = bufferCache.get(index);
  releaseQueueLock();
  return tmp;
} // getBlock

size_t BufferedBlockManager::setBlock(uint64_t index, const string& buf) {
  assert(isOpen());
  Log::logf("BufferedBlockManager::setBlock", 2, "index=%" PRIu64 "\n", index);
  acquireQueueLock();
  while (bufferCache.isFullDirty()) {
    // need to loop because writeLastDirty() temporarily releases the lock
    writeLastDirty();
  }
  bufferCache.add(index, buf, true);
  signalQueueData();
  releaseQueueLock();
  return buf.size();
} // setBlock

bool BufferedBlockManager::isOpen() const {
  return isOpenFlag;
} // isOpen

const string& BufferedBlockManager::getPath() const {
  return fbm->getPath();
}

std::string BufferedBlockManager::getFileName() const {
  return fbm->getFileName();
}

int BufferedBlockManager::open(const std::string& p, const char* mode) {
  if (isOpen()) {
    return 0;
  }

  int openrc = fbm->open(p, mode);
  if (openrc != 0) {
    fprintf(stderr, "BufferedBlockManager::open: error opening FileBlockManager\n");
    return openrc;
  }

  isOpenFlag = true;

  Log::logf("BufferedBlockManager::open", 1,
	    "blockSize=%zu blockCount=%" PRIu64 " size=%" PRIu64 "\n",
	    getBlockSize(), getBlockCount(), getSize());

  // create our thread
  processQueuesFlag = true;

  pthread_attr_t threadAttr;
  pthread_attr_init(&threadAttr);
  pthread_attr_setdetachstate(&threadAttr, PTHREAD_CREATE_JOINABLE);

  pthread_mutex_init(&queueLock, 0);
  pthread_cond_init(&queueCond, 0);

  runNewThread(&queueThread, &BufferedBlockManager::startQueueThread, this, &threadAttr);
  pthread_attr_destroy(&threadAttr);

  return 0;
} // open

int BufferedBlockManager::close() {
  if (!isOpen()) {
    return 0;
  }

  isOpenFlag = false;

  flush();

  acquireQueueLock();
  processQueuesFlag = false;
  signalQueueData();
  releaseQueueLock();

  Log::log("BufferedBlockManager::close", 1, "all data written, closing\n");
  return fbm->close();
} // close

void BufferedBlockManager::flush() {
  Log::log("BufferedBlockManager::flush", 2, "flushing cache\n");
  acquireQueueLock();
  while (bufferCache.hasDirty()) {
    writeLastDirty();
  }
  releaseQueueLock();
} // flush

size_t BufferedBlockManager::getBlockSize() const {
//    assert(isOpen());
  return fbm->getBlockSize();
} // getBlockSize

void BufferedBlockManager::setBlockSize(size_t s) {
  fbm->setBlockSize(s);
} // setBlockSize

void BufferedBlockManager::processQueues() {
  while (processQueuesFlag) {
    acquireQueueLock();
    while (!bufferCache.hasDirty() && readQueue.empty() && processQueuesFlag) {
//       Log::log("BufferedBlockManager::processQueues", "waiting for data\n");
      waitForQueueData();
      //Log::log("BufferedBlockManager::processQueues", "received signal\n");
    }

    if (processQueuesFlag) {
      if (bufferCache.hasDirty()) {
	Log::log("BufferedBlockManager::processQueues", 3, "writing last dirty block\n");
	writeLastDirty();
      }
      else {
	assert(!readQueue.empty());
	int index;
	do {
	  index = readQueue.front();
	  Log::logf("BufferedBlockManager::processQueues", 2, "dequeued %d\n", index);
	  readQueue.pop_front();
	} while (!bufferCache.containsKey(index) && !readQueue.empty());
// 	Log::logf("BufferedBlockManager::processQueues", "reading block %d\n", index);
	readIntoCache(index);
      }
    }
    
    releaseQueueLock();
  }
//   pthread_exit((void*)0);
} // processQueues

void BufferedBlockManager::readIntoCache(uint index) {
  // assume caller has lock

  if (bufferCache.containsKey(index)) {
    return;
  }

  releaseQueueLock();
  Log::logf("BufferedBlockManager::readIntoCache", 2, "reading block %d\n", index);
  string buf = fbm->getBlock(index);
  acquireQueueLock();
  while (bufferCache.isFullDirty()) {
    // need to loop because writeLastDirty() temporarily releases the lock
    writeLastDirty();
  }
  bufferCache.add(index, buf, false);
} // readIntoCache

void BufferedBlockManager::writeLastDirty() {
  // assume caller has lock

  int index = bufferCache.getLastDirtyKey();
  Log::logf("BufferedBlockManager::writeLastDirty", 2, "writing block %d\n", index);
  string buf = bufferCache.getDirty(index);
  bufferCache.clearDirty(index);
//   releaseQueueLock();
  assert(fbm->setBlock(index, buf) == buf.size());
  Log::logf("BufferedBlockManager::writeLastDirty", 2, "wrote %zu bytes\n", buf.size());
//   acquireQueueLock();
} // writeLastDirty

void BufferedBlockManager::acquireQueueLock() {
  assert(pthread_mutex_lock(&queueLock) == 0);
} // acquireQueueLock

void BufferedBlockManager::signalQueueData() {
  assert(pthread_cond_signal(&queueCond) == 0);
} // signalQueueData

void BufferedBlockManager::waitForQueueData() {
  // assume caller has lock
  assert(pthread_cond_wait(&queueCond, &queueLock) == 0);
} // waitForQueueData

void BufferedBlockManager::releaseQueueLock() {
  assert(pthread_mutex_unlock(&queueLock) == 0);
} // releaseQueueLock

void* BufferedBlockManager::startQueueThread(void* t) {
  BufferedBlockManager* self = (BufferedBlockManager*)t;
  assert(self);
  self->processQueues();
  //XXX: What should this return?
  return t;
} // startThread

