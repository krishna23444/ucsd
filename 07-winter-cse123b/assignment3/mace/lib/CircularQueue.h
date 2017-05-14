/* 
 * CircularQueue.h : part of the Mace toolkit for building distributed systems
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
#ifndef CIRCULAR_QUEUE_H
#define CIRCULAR_QUEUE_H

template<class T>
class CircularQueue {
public:
  static const size_t DEFAULT_MAX_SIZE = 4096;

public:
  CircularQueue(size_t len = DEFAULT_MAX_SIZE) :
    MAX_SIZE(len), data(0), first(0), last(0) {
    data = new T*[MAX_SIZE];
  } // CircularQueue

  virtual ~CircularQueue() {
    while (!empty()) {
      pop();
    }
    delete [] data;
  } // ~CircularQueue

  virtual uint64_t size() const {
    if (last >= first) {
      return last - first;
    }
    else {
      return MAX_SIZE - first + last;
    }
  } // size

  virtual bool isFull() const {
    return (((last + 1) % MAX_SIZE) == first);
  } // isFull

  virtual bool empty() const {
    return (first == last);
  } // empty

  virtual T& front() const {
    assert(!empty());
    return *(data[first]);
  } // front

  virtual void pop() {
    assert(!empty());
    T* v = data[first];
    first = (first + 1) % MAX_SIZE;
    delete v;
  } // pop

  virtual void push(const T& v) {
    assert(!isFull());
    data[last] = new T(v);
    last = (last + 1) % MAX_SIZE;
  } // push

  virtual void clear() {
    while (!empty()) {
      pop();
    }
  } // clear

public:
  const size_t MAX_SIZE;

protected:
  T** data;
  size_t first;
  size_t last;

}; // CircularQueue

#endif // CIRCULAR_QUEUE_H
