/* 
 * PipedSignal.h : part of the Mace toolkit for building distributed systems
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
#ifndef _PIPED_SIGNAL_H
#define _PIPED_SIGNAL_H

// #include "mace-macros.h"
#include "ScopedLock.h"
#include "SockUtil.h"
#include "SysUtil.h"
#include "FileUtil.h"
#include "Log.h"

class PipedSignal {
public:
  PipedSignal() {
    int des[2];
    if(pipe(des) < 0) {
      Log::perror("pipe");
      ASSERT(0);
    }
    r = des[0];
    w = des[1];
    SockUtil::setNonblock(r);
    SockUtil::setNonblock(w);
    FD_ZERO(&rset);
  }
  
  virtual ~PipedSignal() {
    close();
  }

  void addToSet(fd_set& s, int& selectMax) const {
    FD_SET(r, &s);
    selectMax = std::max(selectMax, r);
  }

  bool clear(fd_set& s) {
    bool ret = FD_ISSET(r, &s);
    if (ret) {
      FileUtil::read(r);
    }
    FD_CLR(r, &s);
    return ret;
  }

  void signal() {
    ::write(w, " ", 1);
  }

  int wait(uint64_t usec = 0) {
    FD_SET(r, &rset);
    int n = SysUtil::select(r + 1, &rset, 0, 0, usec);
    if (n) {
      ASSERT(FD_ISSET(r, &rset));
      FileUtil::read(r);
    }
    return n;
  }

  void close() {
    ::close(r);
    ::close(w);
  }

private:
  int r;
  int w;
  fd_set rset;
}; // PipedSignal

#endif // _PIPED_SIGNAL_H
