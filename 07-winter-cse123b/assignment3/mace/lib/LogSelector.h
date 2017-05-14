/* 
 * LogSelector.h : part of the Mace toolkit for building distributed systems
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
// #include <stdio.h>
#include <string>
#include "mhash_map.h"

#ifndef LOG_SELECTOR_H
#define LOG_SELECTOR_H

enum LogSelectorTimestamp { LOG_TIMESTAMP_DISABLED,
			    LOG_TIMESTAMP_EPOCH,
			    LOG_TIMESTAMP_HUMAN };
enum LogSelectorThreadId { LOG_THREADID_DISABLED, LOG_THREADID_ENABLED };
enum LogSelectorName { LOG_NAME_DISABLED, LOG_NAME_ENABLED };
enum LogSelectorOutput { LOG_FPRINTF, LOG_PIP };

class Sim;

class LogSelector {
  friend class Sim;

public:
  LogSelector(const std::string& name,
	      FILE* fp, 
	      LogSelectorTimestamp logTimestamp,
	      LogSelectorName logSelectorName,
	      LogSelectorThreadId logSelectorThreadId,
              LogSelectorOutput logSelectorOutput);
  ~LogSelector();

  std::string getSelectorHeader() const;
  std::string getName() const;
  FILE* getFile() const;
  LogSelectorTimestamp getLogTimestamp() const;
  LogSelectorName getLogName() const;
  LogSelectorThreadId getLogThreadId() const;
  LogSelectorOutput getLogSelectorOutput() const;

private:
  const std::string name;
  FILE* fp;
  LogSelectorTimestamp logTimestamp;
  LogSelectorName logName;
  LogSelectorThreadId logThreadId;
  LogSelectorOutput logOutput;

  typedef mace::hash_map<unsigned long, int> ThreadIdMap;
  static ThreadIdMap threadIds;
  static const std::string* prefix;
};

#endif // LOG_SELECTOR_H
