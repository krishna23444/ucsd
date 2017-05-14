/* 
 * ScopedLog.h : part of the Mace toolkit for building distributed systems
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
#ifndef _SCOPED_LOG_H
#define _SCOPED_LOG_H

#include <string>
#include "Log.h"
#include "pip_includer.h"

class ScopedLog {
public:
  ScopedLog(const std::string& _note, log_level_t _level, log_id_t _logId, bool _prefix, bool _suffix, bool _log, bool _annotate) : 
    note(_note), level(_level), logId(_logId), prefix(_prefix), suffix(_suffix), doLog(_log), annotate(_annotate)
  {
    if(prefix) {
      if(suffix) {
        if(doLog) {
          Log::logf(logId, level, "STARTING( %s )\n", note.c_str());
        }
        if(annotate) {
          ANNOTATE_START_TASK(NULL, 0, note.c_str());
          int __pip_path_len__;
          const void* tp = ANNOTATE_GET_PATH_ID(&__pip_path_len__);
          ANNOTATE_PUSH_PATH_ID(NULL, 0, tp, __pip_path_len__);
        }
      }
      else {
        if(doLog) {
          Log::logf(logId, level, "CALLED( %s )\n", note.c_str());
        }
        if(annotate) {
          ANNOTATE_NOTICE(NULL, 0, "%s", note.c_str());
        }
      }
    }
  }

  virtual ~ScopedLog() {
    if(suffix) {
      if(prefix) {
        if(doLog) {
          Log::logf(logId, level, "ENDING( %s )\n", note.c_str());
        }
        if(annotate) {
          ANNOTATE_POP_PATH_ID(NULL, 0);
          ANNOTATE_END_TASK(NULL, 0, note.c_str());
        }
      }
      else {
        if(doLog) {
          Log::logf(logId, level, "ENDED( %s )\n", note.c_str());
        }
        if(annotate) {
          ANNOTATE_NOTICE(NULL, 0, "%s", note.c_str());
        }
      }
    }
  }

protected:
  const std::string& note;
  const log_level_t level;
  const log_id_t logId;
  const bool prefix;
  const bool suffix;
  const bool doLog;
  const bool annotate;
  
}; // ScopedLog

#endif // _SCOPED_LOG_H
