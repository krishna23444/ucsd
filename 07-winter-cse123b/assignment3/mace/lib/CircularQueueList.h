/* 
 * CircularQueueList.h : part of the Mace toolkit for building distributed systems
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
#ifndef CIRCULAR_QUEUE_LIST_H
#define CIRCULAR_QUEUE_LIST_H

#include <boost/shared_ptr.hpp>
#include "FIFOLinkedList.h"
#include "CircularQueue.h"

template <class T>
class CircularQueueList {
  typedef CircularQueue<T> CQ;
  typedef boost::shared_ptr<CQ> CQPtr;


public:
  CircularQueueList(size_t len = CQ::DEFAULT_MAX_SIZE) :
    MAX_SIZE(len), l(CQPtr(new CQ(len))), first(0), last(0), pushc(0), popc(0) {

  } // CircularQueueList
  
  virtual ~CircularQueueList() {

  } // ~CircularQueueList

  virtual uint64_t size() const {
//     uint64_t r = l.front()->size();
//     std::cout << "l.size()=" << l.size() << " r=" << r << " ";
//     if (l.canPop()) {
//       r += l.back()->size();
//       std::cout << "r=" << r << " ";
//       r += (l.size() - 2) * (MAX_SIZE - 1);
//       std::cout << "r=" << r << std::endl;
//     }
//     return r;
    return pushc - popc;
  } // size

  virtual bool empty() const {
    return (!l.canPop() && l.front()->empty());
  } // empty

  virtual T& front() const {
    assert(!empty());
    return l.front()->front();
  } // front

  virtual void pop() {
    assert(!empty());
    l.front()->pop();
    if (l.front()->empty() && l.canPop()) {
      l.pop();
    }
    popc++;
  } // pop

  virtual void push(const T& v) {
    if (l.back()->isFull()) {
      l.push(CQPtr(new CQ(MAX_SIZE)));
    }
    l.back()->push(v);
    pushc++;
  } // push

  virtual void clear() {
    while (!empty()) {
      pop();
    }
  } // clear

public:
  const size_t MAX_SIZE;

private:
  FIFOLinkedList<CQPtr> l;
  uint64_t first;
  uint64_t last;
  uint64_t pushc;
  uint64_t popc;

}; // CircularQueueList

#endif // CIRCULAR_QUEUE_LIST_H
