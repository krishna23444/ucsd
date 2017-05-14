/* 
 * BlockManager.h : part of the Mace toolkit for building distributed systems
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
#include <stdint.h>
#include <sys/types.h>
#include <string>

#ifndef BLOCK_MANAGER_H
#define BLOCK_MANAGER_H

class BlockManager {
public:
  BlockManager();
  virtual std::string getBlock(uint64_t index) = 0;
  virtual size_t setBlock(uint64_t index, const std::string& buffer) = 0;
  virtual size_t getBlockSize() const { return blockSize; }
  virtual void setBlockSize(size_t s) { blockSize = s; }
  virtual uint32_t getMinimumInterested() const { return minimumInterested; }
  virtual uint32_t getMaximumInterested() const { return maximumInterested; }
  virtual uint64_t getBlockCount() const { return blockCount; }
  virtual uint64_t getSize() const { return size; }
  virtual int close() = 0;
  virtual ~BlockManager() {}

public:
  static const size_t DEFAULT_BLOCK_SIZE;

protected:
  size_t blockSize;
  uint64_t blockCount;
  uint64_t size;
public:
  uint32_t minimumInterested;
  uint32_t maximumInterested;
}; // BlockManager

#endif // BLOCK_MANAGER_H
