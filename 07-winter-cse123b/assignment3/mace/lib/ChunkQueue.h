/* 
 * ChunkQueue.h : part of the Mace toolkit for building distributed systems
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
#ifndef CHUNK_QUEUE_H
#define CHUNK_QUEUE_H

#include "Collections.h"

class ChunkQueue {
public:
//   static const size_t DEFAULT_BLOCK_SIZE = 32768;
  static const size_t DEFAULT_BLOCK_SIZE = 65536;
//   static const size_t DEFAULT_CHUNK_SIZE = 524288;
  static const size_t DEFAULT_CHUNK_SIZE = 1048576;
//   static const size_t DEFAULT_CHUNK_SIZE = 4194304;
  static const size_t FREE_BUFS = 16;

public:
  ChunkQueue(uint32_t blockSize = DEFAULT_BLOCK_SIZE,
	     uint32_t chunkSize = DEFAULT_CHUNK_SIZE) :
    BLOCK_SIZE(blockSize), CHUNK_SIZE(chunkSize), MAX_BUF_COUNT(UINT_MAX / CHUNK_SIZE),
    nbuf(0), bufs(0), boffset(0), firstbuf(0), lastbuf(0), wsize(0), rsize(0),
    bufcount(0), maxsize(0) {

    bufs = new char*[MAX_BUF_COUNT];
    buflen = new size_t[MAX_BUF_COUNT];
    memset(bufs, 0, sizeof(char*) * MAX_BUF_COUNT);

    pthread_mutex_init(&qlock, 0);
  } // ChunkQueue

  virtual ~ChunkQueue() {
    for (size_t i = 0; i < MAX_BUF_COUNT; i++) {
      if (bufs[i] != 0) {
	delete[] bufs[i];
	bufs[i] = 0;
      }
    }

    delete[] bufs;
    delete[] buflen;

    pthread_mutex_destroy(&qlock);
  } // ~ChunkQueue

  void append(const std::string& s) {
    wsize += s.size();
    maxsize = std::max(maxsize, size());
    size_t i = 0;
    while (i < s.size()) {
      if (bufs[lastbuf] == 0) {
	bufs[lastbuf] = new char[CHUNK_SIZE];
	buflen[lastbuf] = 0;
	bufcount++;
	nbuf++;
      }
      size_t c = std::min(s.size() - i, CHUNK_SIZE - buflen[lastbuf]);
      memcpy(bufs[lastbuf] + buflen[lastbuf], s.data() + i, c);
      buflen[lastbuf] += c;
      i += c;
      assert(buflen[lastbuf] <= CHUNK_SIZE);
      if (buflen[lastbuf] == CHUNK_SIZE) {
	lastbuf = (lastbuf + 1) % MAX_BUF_COUNT;
	assert(lastbuf != firstbuf);
      }
    }
  } // append

  const char* read(size_t& len) {
    len = std::min(buflen[firstbuf] - boffset, BLOCK_SIZE);
    return bufs[firstbuf] + boffset;
  } // read

  void advance(size_t len) {
    boffset += len;
    rsize += len;

    if (boffset == CHUNK_SIZE) {
      delete[] bufs[firstbuf];
      nbuf--;
      buflen[firstbuf] = 0;
      bufs[firstbuf] = 0;
      firstbuf = (firstbuf + 1) % MAX_BUF_COUNT;
      boffset = 0;
    }
  } // advance

  size_t size() const {
    return wsize - rsize;
  } // size

  std::string stats() const {
    std::ostringstream r;
    r << " bufcount=" << bufcount << " maxsize=" << maxsize << " nbuf=" << nbuf;
    return r.str();
  } // stats

private:
  void lock() {
    assert(pthread_mutex_lock(&qlock) == 0);
  } // lock

  void unlock() {
    assert(pthread_mutex_unlock(&qlock) == 0);
  } // unlock

public:
  const size_t BLOCK_SIZE;
  const size_t CHUNK_SIZE;
  const size_t MAX_BUF_COUNT;

protected:
  size_t nbuf;
  char** bufs;
  size_t* buflen;
  size_t boffset;
  size_t firstbuf;
  size_t lastbuf;
  size_t wsize;
  size_t rsize;

  // stats
  size_t bufcount;
  size_t maxsize;
  
private:
  pthread_mutex_t qlock;
}; // ChunkQueue

#endif // CHUNK_QUEUE_H
