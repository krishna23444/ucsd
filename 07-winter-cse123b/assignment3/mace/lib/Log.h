/* 
 * Log.h : part of the Mace toolkit for building distributed systems
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
#ifndef _LOG_H
#define _LOG_H

#include <stdio.h>
#include <list>
#include <vector>
#include <cstdarg>
#include <ostream>
#include <sstream>
#include <pthread.h>

// #include "mvector.h"
#include "mhash_map.h"
// #include "hash_string.h"
#include "Serializable.h"
#include "LogSelector.h"
// #include "Params.h"
class Params;
class hash_string;

typedef uint32_t log_id_t;
typedef uint32_t log_level_t;

class Log {
public:
  static class LogFlushManipulator {} end;
  static class LogFlushLineManipulator {} endl;

  class MaceTraceStream {
  private:
    friend class Log;
    std::ostringstream* stream;
    log_id_t id;

  public:
    MaceTraceStream& operator<<(const LogFlushManipulator& l) {
      if(stream != NULL) {
	Log::flush(*this);
      }
      return *this;
    }

    template<typename T>
    MaceTraceStream& operator<<(const T* r) { 
      if(stream != NULL) {
	std::string tmp;
	mace::serialize(tmp, r, *r);
	(*stream) << tmp;
      }
      return *this;
    }

    template<typename T>
    MaceTraceStream& operator<<(const T& r) { 
      if(stream != NULL) {
	std::string tmp;
	mace::serialize(tmp, &r, r);
	(*stream) << tmp;
      }
      return *this;
    }

    MaceTraceStream(log_id_t lid = NULL_ID) :
      stream(lid==NULL_ID ? NULL : new std::ostringstream()), id(lid) {}
    bool isNoop() const { return stream == NULL; }
  }; // MaceTraceStream

  class MaceOutputStream {
  private:
    friend class Log;
    std::ostringstream* stream;
    log_id_t id;
  public:
    MaceOutputStream& operator<<(const LogFlushManipulator& l) {
      if(stream != NULL) {
	Log::flush(*this);
      }
      return *this;
    }
    MaceOutputStream& operator<<(const LogFlushLineManipulator& l) {
      if(stream != NULL) {
	(*stream) << std::endl;
	Log::flush(*this);
      }
      return *this;
    }
    MaceOutputStream& operator<<(std::ostream::__ios_type& (*__pf)(std::ostream::__ios_type&)) {
      if(stream != NULL) {
	(*stream) << __pf;
      }
      return *this;
    }
    MaceOutputStream& operator<<(std::ostream::ios_base& (*__pf)(std::ostream::ios_base&)) {
      if(stream != NULL) {
	(*stream) << __pf;
      }
      return *this;
    }
    MaceOutputStream& operator<<(std::ostream::__ostream_type& (*__pf)(std::ostream::__ostream_type&)) {
      if(stream != NULL) {
	(*stream) << __pf;
      }
      return *this;
    }
    template<typename T>
    MaceOutputStream& operator<<(const T* r) { 
      if(stream != NULL) {
	(*stream) << r;
      }
      return *this;
    }
    template<typename T>
    MaceOutputStream& operator<<(const T& r) { 
      if(stream != NULL) {
	(*stream) << r;
      }
      return *this;
    }
    template<typename T>
    void printItem(const T* pitem, const T& item) {
      if(stream != NULL) {
        mace::printItem(*stream, pitem, item);
      }
    }
    MaceOutputStream(log_id_t lid = NULL_ID) :
      stream(lid==NULL_ID ? NULL : new std::ostringstream()), id(lid) {}
    bool isNoop() const { return stream == NULL; }
  }; // MaceOutputStream

  class NullOutputType {
    public:
      template<typename T>
      const NullOutputType& operator<<(const T* r) const { return *this; }
      template<typename T>
      const NullOutputType& operator<<(const T& r) const { return *this; }

    const NullOutputType& operator<<(std::ostream::__ios_type& (*__pf)(std::ostream::__ios_type&)) const {
      return *this;
    }
    const NullOutputType& operator<<(std::ostream::ios_base& (*__pf)(std::ostream::ios_base&)) const {
      return *this;
    }
    const NullOutputType& operator<<(std::ostream::__ostream_type& (*__pf)(std::ostream::__ostream_type&)) const {
      return *this;
    }
  };

  typedef mace::hash_map<log_id_t, MaceOutputStream*> StreamMap;
  typedef mace::hash_map<log_id_t, MaceTraceStream*> TraceStreamMap;

  class ThreadSpecific {
    friend class Log;

  public:
    ThreadSpecific();
    virtual ~ThreadSpecific();
    static ThreadSpecific* init();
    static unsigned int getVtid();
    static StreamMap& getStreams();
    static TraceStreamMap& getTraceStreams();
    
  private:
    static void initKey();

  protected:
    unsigned int vtid;
    StreamMap streams;
    TraceStreamMap traceStreams;

  private:
    static pthread_key_t pkey;
    static pthread_once_t keyOnce;
    static unsigned int count;
  }; // ThreadSpecific


public:
  static log_id_t getId(const std::string& selector);
  static void add(const std::string& selector,
		  FILE* fp = stdout,
		  LogSelectorTimestamp = LOG_TIMESTAMP_DISABLED,
		  LogSelectorName = LOG_NAME_DISABLED,
		  LogSelectorThreadId = LOG_THREADID_DISABLED,
                  LogSelectorOutput = LOG_FPRINTF);
  static void autoAddAll(FILE* fp = stdout,
			 LogSelectorTimestamp = LOG_TIMESTAMP_EPOCH,
			 LogSelectorName = LOG_NAME_ENABLED,
			 LogSelectorThreadId = LOG_THREADID_ENABLED,
                         LogSelectorOutput = LOG_FPRINTF);
  static void autoAdd(const std::string& subselector,
		      FILE* fp = stdout,
		      LogSelectorTimestamp = LOG_TIMESTAMP_EPOCH,
		      LogSelectorName = LOG_NAME_ENABLED,
		      LogSelectorThreadId = LOG_THREADID_ENABLED,
                      LogSelectorOutput = LOG_FPRINTF);
  static void remove(const std::string& selector, FILE* fp = stdout);
  static void removeAll(const std::string& selector);
  static FILE* openLogFile(const std::string& path, const char* mode = "a");
  static void logToFile(const std::string& path, const std::string& selector,
			const char* mode = "w",
			LogSelectorTimestamp sts = LOG_TIMESTAMP_EPOCH,
			LogSelectorName sn = LOG_NAME_DISABLED,
			LogSelectorThreadId stid = LOG_THREADID_DISABLED);
  static void closeLogFiles();
  static void enableLogging();
  static void disableLogging();
  static void disableDefaultError() { errorSelected = false; }
  static void disableDefaultWarning() { warningSelected = false; }
  static void perror(const std::string& s);
  static MaceOutputStream& err() { return log("ERROR"); }
  static MaceOutputStream& warn() { return log("WARNING"); }
  static MaceOutputStream& info() { return log("INFO"); }
  static MaceOutputStream& log(log_id_t id, log_level_t level = DEFAULT_LEVEL);
  static MaceOutputStream& log(const std::string& selector,
			       log_level_t level = DEFAULT_LEVEL);
  static void log(log_id_t id, const std::string& message) {
    log(id, DEFAULT_LEVEL, message);
  }
  static void log(log_id_t id, log_level_t level, const std::string& message);
  static void log(const std::string& selector, const std::string& message) {
    log(selector, DEFAULT_LEVEL, message);
  }
  static void log(const std::string& selector, log_level_t level,
		  const std::string& message);
  static void log(log_id_t id, const char* message) {
    log(id, DEFAULT_LEVEL, message);
  }
  static void log(log_id_t id, log_level_t level, const char* message);
  static void log(const std::string& selector, const char* message) {
    log(selector, DEFAULT_LEVEL, message);
  }
  static void log(const std::string& selector, log_level_t level, const char* message);
  static void logf(log_id_t id, const char* format ...) __attribute__((format(printf,2,3)));
  static void logf(log_id_t id, log_level_t level, const char* format ...) __attribute__((format(printf,3,4)));
  static void logf(const std::string& selector, const char* format ...) __attribute__((format(printf,2,3)));
  static void logf(const std::string& selector, log_level_t level, const char* format ...) __attribute__((format(printf,3,4)));
  static std::string toHex(const std::string& s);
  static void flush(MaceOutputStream& s);
  static log_level_t getLevel() { return logLevel; }
  static void setLevel(log_level_t l = DEFAULT_LEVEL) { logLevel = l; }

  static void configure(Params* p);

  static void nologf() {}
  static const NullOutputType& nolog() { return nullOutputVar; }

  static log_id_t getTraceId(const std::string& selector);
  static MaceTraceStream& trace(log_id_t id);
  static void flush(MaceTraceStream& s);
  static void traceNewThread(const mace::string& fname);
  static std::istream& replay(log_id_t id);

public:
  static const log_level_t DEFAULT_LEVEL;
  static const log_level_t MAX_LEVEL;
  static const log_id_t NULL_ID;
  
private:
  typedef std::list<LogSelector*> SelectorList;

protected:
  static bool isSelected(const std::string& selector);
  static void remove(SelectorList* l, FILE* f);
  static void addl(const std::string& selector,
		   FILE* fp,
		   LogSelectorTimestamp,
		   LogSelectorName,
		   LogSelectorThreadId,
                   LogSelectorOutput);
  static void scheduleThread(unsigned int id, const mace::string& description);

private:
//   static MaceOutputStream* findStream(log_id_t id, pthread_t tid);
  static void lock();
  static void unlock();
  Log() { }

protected:
  static bool enabled;
  static bool autoAll;
  static const bool FLUSH;

  typedef mace::hash_map<std::string, SelectorList*, hash_string> SelectorMap;
  typedef mace::hash_map<std::string, FILE*, hash_string> LogFileMap;
  typedef mace::hash_map<std::string, log_id_t, hash_string> SelectorIdMap;
  typedef std::vector<SelectorList*> IdSelectorList; //log_id_t index
//   typedef mace::hash_map<pthread_t, MaceOutputStream*> ThreadStreamMap;
//   typedef mace::vector<ThreadStreamMap*> LogStreamList; //log_id_t index

  static SelectorMap selectors;
  static IdSelectorList selectorLists;
  static LogFileMap logFiles;
//   static LogStreamList streams;
  static SelectorList autoSelectors;
  static LogSelector* autoAllDefault;
  static pthread_mutex_t llock; // lock call to fprintf
  static SelectorIdMap ids;
  static uint32_t idCount;
  static const size_t MAX_MESSAGE_SIZE;
  static NullOutputType nullOutputVar;
  static log_level_t logLevel;
  static bool errorSelected;
  static bool warningSelected;
  static MaceTraceStream traceStream;
  static std::ifstream replayStream;
}; // Log

// Log::MaceOutputStream& operator<<(Log::MaceOutputStream& o, Log::LogFlushManipulator l);
// Log::MaceOutputStream& operator<<(Log::MaceOutputStream& o, Log::LogFlushLineManipulator l);

namespace mace {
  template<typename S> 
    void printItem(Log::MaceOutputStream& out, const S* pitem, const S& item) {
      out.printItem(pitem, item);
    }

  template<typename T> T const & logVal(T const & a, Log::MaceOutputStream& out, const mace::string& prefix) {
    out << prefix << a << Log::endl;
    return a;
  }

  template<typename T> T& logVal(T& a, Log::MaceOutputStream& out, const mace::string& prefix) {
    out << prefix << a << Log::endl;
    return a;
  }

}

#define __LOG_H_DONE
#include "massert.h"
#endif // _LOG_H
