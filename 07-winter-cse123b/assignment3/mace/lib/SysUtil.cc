/* 
 * SysUtil.cc : part of the Mace toolkit for building distributed systems
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
#include <set>
#include "SysUtil.h"
#include "Util.h"
#include "Log.h"

int SysUtil::select(int max, fd_set* rfs, fd_set* wfs, fd_set* efs, uint64_t usec, bool restartOnEINTR) {
  if (usec) {
    struct timeval tv = { usec / 1000000, usec % 1000000 };
    return select(max, rfs, wfs, efs, &tv, restartOnEINTR);
  }
  else {
    return select(max, rfs, wfs, efs, (struct timeval*)0, restartOnEINTR);
  }
}

int SysUtil::select(int max, fd_set* rfs, fd_set* wfs, fd_set* efs,
		    struct timeval* timeout, bool restartOnEINTR) {
  int n = 0;
  uint64_t sleeptime = 0;
  if (timeout) {
    sleeptime = Util::timeu(*timeout);
  }

  do {
    uint64_t start = Util::timeu();
    n = ::select(max, rfs, wfs, efs, timeout);
    if (n < 0) {
      int err = errno;
      if (err == EINTR) {
        if (restartOnEINTR) {
          if (sleeptime != 0) {
            uint64_t now = Util::timeu();
            uint64_t diff = now - start;
            if (diff < sleeptime) {
              sleeptime -= diff;
              Util::fillTimeval(sleeptime, *timeout);
            }
            else {
              // this really should never happen
              Log::warn() << "WARNING: select was interrupted "
                "after it should have timed-out\n";
              return 0;
            }
          }
        } else {
          if (rfs) {
            FD_ZERO(rfs);
          }
          if (wfs) {
            FD_ZERO(wfs);
          }
          if (efs) {
            FD_ZERO(efs);
          }
          return 0;
        }
      }
      else if (err == EBADF) {
	Log::err() << "select returned EBADF\n" << Log::endl;
	if (rfs) {
	  FD_ZERO(rfs);
	}
	if (wfs) {
	  FD_ZERO(wfs);
	}
	if (efs) {
	  FD_ZERO(efs);
	}
	return 0;
      }
      else {
	Log::perror("select");
	assert(0);
      }
    }
    else if (n >= 0) {
      return n;
    }
  } while (true);
} // select

void SysUtil::sleepu(uint64_t usec) {
  sleep(usec / 1000000, usec % 1000000);
} // sleep

void SysUtil::sleep(time_t sec, useconds_t usec) {
  if (sec > 0 || usec > 0) {
    struct timeval tv = { sec, usec };
    select(0, 0, 0, 0, &tv);
  }
  else {
    select(0, 0, 0, 0, (struct timeval*)0);
  }
} // sleep


sighandler_t SysUtil::signal(int signum, sighandler_t handler, bool warn) {
  static std::set<int> registeredSignals;

  if (registeredSignals.find(signum) != registeredSignals.end()) {
    if (warn) {
      Log::warn() << "WARNING: signal handler already registered for " << signum
		  << Log::endl;
    }
    return handler;
  }
  
  registeredSignals.insert(signum);

  struct sigaction act;
  struct sigaction oact;
  memset(&act, 0, sizeof(act));
  memset(&oact, 0, sizeof(oact));

  sigemptyset(&act.sa_mask);
  act.sa_flags = SA_RESTART;
  act.sa_handler = handler;

  assert(sigaction(signum, &act, &oact) == 0);

  return oact.sa_handler;
} // signal
