/* 
 * Log.cc : part of the Mace toolkit for building distributed systems
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
#include "Log.h"
#include "StrUtil.h"
#include "Util.h"
#include "pip_includer.h"
#include "Params.h"

#include <fstream>
#include <assert.h>

using namespace std;

bool Log::enabled = true;
bool Log::autoAll = false;
bool Log::errorSelected = true;
bool Log::warningSelected = true;
const bool Log::FLUSH = true;
const log_level_t Log::DEFAULT_LEVEL = 0;
const log_level_t Log::MAX_LEVEL = UINT_MAX;
log_level_t Log::logLevel = DEFAULT_LEVEL;
Log::SelectorMap Log::selectors;
Log::IdSelectorList Log::selectorLists(1);
Log::LogFileMap Log::logFiles;
// Log::LogStreamList Log::streams(1);
Log::SelectorList Log::autoSelectors;
Log::SelectorIdMap Log::ids;
LogSelector* Log::autoAllDefault = 0;
pthread_mutex_t Log::llock = PTHREAD_MUTEX_INITIALIZER;
uint32_t Log::idCount = 1;
const log_id_t Log::NULL_ID = 0;
const size_t Log::MAX_MESSAGE_SIZE = 4096;
Log::NullOutputType Log::nullOutputVar;
Log::LogFlushManipulator Log::end;
Log::LogFlushLineManipulator Log::endl;
std::ifstream Log::replayStream;

pthread_key_t Log::ThreadSpecific::pkey;
pthread_once_t Log::ThreadSpecific::keyOnce = PTHREAD_ONCE_INIT;
unsigned int Log::ThreadSpecific::count = 0;

Log::ThreadSpecific::ThreadSpecific() {
  count++;
  vtid = count;
} // ThreadSpecific

Log::ThreadSpecific::~ThreadSpecific() {
} // ~ThreadSpecific

Log::ThreadSpecific* Log::ThreadSpecific::init() {
  pthread_once(&keyOnce, Log::ThreadSpecific::initKey);
  ThreadSpecific* t = (ThreadSpecific*)pthread_getspecific(pkey);
  if (t == 0) {
    t = new ThreadSpecific();
    assert(pthread_setspecific(pkey, t) == 0);
  }

  return t;
} // init

void Log::ThreadSpecific::initKey() {
  assert(pthread_key_create(&pkey, NULL) == 0);
} // initKey

unsigned int Log::ThreadSpecific::getVtid() {
  ThreadSpecific* t = (ThreadSpecific*)init();
  return t->vtid;
} // getVtid

Log::StreamMap& Log::ThreadSpecific::getStreams() {
  ThreadSpecific* t = (ThreadSpecific*)init();
  return t->streams;
} // getStreams

Log::TraceStreamMap& Log::ThreadSpecific::getTraceStreams() {
  ThreadSpecific* t = (ThreadSpecific*)init();
  return t->traceStreams;
} // getStreams

log_id_t Log::getId(const std::string& selector) {
  log_id_t r = NULL_ID;
  lock();
  SelectorIdMap::iterator i = ids.find(selector);
  if (i == ids.end()) {
    if (isSelected(selector)) {
      SelectorList* l = selectors[selector];
      r = idCount;
      idCount++;
      selectorLists.push_back(l);
//       streams.push_back(new ThreadStreamMap());
    }
    ids[selector] = r;
  }
  else {
    r = i->second;
  }

  unlock();
  return r;
} // getId

Log::MaceOutputStream& Log::log(const std::string& selector, log_level_t level) {
  return log(getId(selector), level);
} // log

void Log::log(log_id_t id, log_id_t level, const std::string& m) {
  if (!enabled || id == NULL_ID || level > logLevel) {
    return;
  }

  log(id, level, m.c_str());
} // log

void Log::log(log_id_t id, log_id_t level, const char* m) {
  if (!enabled || id == NULL_ID || level > logLevel) {
    return;
  }

  // WARNING - assumes selector list exists for id
  // does not need to be locked because l is a linked list
  SelectorList* l = selectorLists[id];
  for (SelectorList::const_iterator i = l->begin(); i != l->end(); i++) {
    const LogSelector* sel = *i;
    if(sel->getLogSelectorOutput() == LOG_FPRINTF) {
      fprintf(sel->getFile(), "%s%s", sel->getSelectorHeader().c_str(), m);
    } else {
#ifdef PIP_MESSAGING
      ANNOTATE_NOTICE(sel->getSelectorHeader().c_str(), 4 + level, "%s", m);
      #else
      fprintf(sel->getFile(), "%s%s", sel->getSelectorHeader().c_str(), m);
#endif
    }
    if (FLUSH) {
      fflush(sel->getFile());
//       fdatasync(fileno(sel->getFile()));
    }
  }
} // log

void Log::logf(log_id_t id, log_level_t level, const char* f ...) {
  if (!enabled || id == NULL_ID || level > logLevel) {
    return;
  }

  va_list ap;
  va_start(ap, f);
  char tmp[MAX_MESSAGE_SIZE];
  vsnprintf(tmp, MAX_MESSAGE_SIZE - 1, f, ap);
  va_end(ap);
  log(id, level, tmp);
} // logf

void Log::logf(log_id_t id, const char* f ...) {
  if (!enabled || id == NULL_ID) {
    return;
  }

  va_list ap;
  va_start(ap, f);
  char tmp[MAX_MESSAGE_SIZE];
  vsnprintf(tmp, MAX_MESSAGE_SIZE - 1, f, ap);
  va_end(ap);
  log(id, tmp);
} // logf

Log::MaceOutputStream& Log::log(log_id_t id, log_level_t level) {
  static MaceOutputStream* nullOutStr = new MaceOutputStream();
  if (id == NULL_ID || level > logLevel) { 
    return *nullOutStr;
  }
  StreamMap& m = ThreadSpecific::getStreams();

  // this does not need to be locked because this is thread-specific data
  StreamMap::iterator i = m.find(id);
  MaceOutputStream* r = 0;
  if (i == m.end()) {
    r = new MaceOutputStream(id);
    m[id] = r;
  }
  else {
    r = i->second;
  }
  
//   pthread_t tid = pthread_self();
//   lock();
//   MaceOutputStream* r = findStream(id, tid);
//   assert(r);
//   unlock();

  return *r;
} // log

// Log::MaceOutputStream* Log::findStream(log_id_t id, pthread_t tid) {
//   assert(id != NULL_ID);
//   ThreadStreamMap& tsm  = *(streams[id]);
//   ThreadStreamMap::iterator i = tsm.find(tid);
//   MaceOutputStream* s = NULL;
//   if(i == tsm.end()) {
//     s = new MaceOutputStream(id);
//     tsm[tid] = s;
//   } else {
//     s = i->second;
//   }
//   return s;
// } // findStream

void Log::flush(MaceOutputStream& str) {
  // WARNING - assumes that str is registered
  if (str.stream != NULL) {
    log(str.id, str.stream->str().c_str());
    str.stream->str("");
  }
} // flush

void Log::autoAddAll(FILE* f,
		     LogSelectorTimestamp lt,
		     LogSelectorName ln,
		     LogSelectorThreadId ltid,
                     LogSelectorOutput lso) {

  if (autoAllDefault != 0) {
    delete autoAllDefault;
  }

  autoAll = true;
  autoAllDefault = new LogSelector("", f, lt, ln, ltid, lso);
} // autoAddAll

void Log::autoAdd(const string& s,
		  FILE* f,
		  LogSelectorTimestamp lt,
		  LogSelectorName ln,
		  LogSelectorThreadId ltid,
                  LogSelectorOutput lso) {

  LogSelector* sel = new LogSelector(s, f, lt, ln, ltid, lso);
  autoSelectors.push_back(sel);
} // autoAdd

void Log::add(const string& s,
	      FILE* f,
	      LogSelectorTimestamp lt,
	      LogSelectorName ln,
	      LogSelectorThreadId ltid,
              LogSelectorOutput lso) {
  lock();

  // this will add any auto selectors
  isSelected(s);

  addl(s, f, lt, ln, ltid, lso);
  unlock();
} // add

void Log::addl(const string& s,
	       FILE* f,
	       LogSelectorTimestamp lt,
	       LogSelectorName ln,
	       LogSelectorThreadId ltid,
               LogSelectorOutput lso) {

  // actually updates the data structures; assumes caller holds lock

  SelectorList* l;
  SelectorMap::iterator i = selectors.find(s);
  if (i != selectors.end()) {
    l = i->second;
  }
  else {
    l = new SelectorList();
    selectors[s] = l;
  }

  // delete the selector if it exists
  remove(l, f);

  LogSelector* sel = new LogSelector(s, f, lt, ln, ltid, lso);
  l->push_back(sel);
} // addl

void Log::remove(const string& s, FILE* f) {
  if (selectors.containsKey(s)) {
    SelectorList* l = selectors[s];
    remove(l, f);
  }
} // remove

void Log::remove(SelectorList* l, FILE* f) {
  SelectorList::iterator i = l->begin();
  while (i != l->end()) {
    if ((*i)->getFile() == f) {
      delete *i;
      *i = 0;
      i = l->erase(i);
    }
    else {
      i++;
    }
  }
} // remove

void Log::removeAll(const string& s) {
  if (selectors.find(s) != selectors.end()) {
    SelectorList* l = selectors[s];
    for (SelectorList::iterator i = l->begin(); i != l->end(); i++) {
      delete *i;
    }
    delete l;
    selectors.erase(s);
  }
} // removeAll

void Log::logToFile(const string& path, const string& sel, const char* mode,
		    LogSelectorTimestamp sts, LogSelectorName sn,
		    LogSelectorThreadId stid) {
  FILE* f = openLogFile(path, mode);
  add(sel, f, sts, sn, stid);
} // logToFile

FILE* Log::openLogFile(const string& path, const char* mode) {
  if (logFiles.find(path) != logFiles.end()) {
    return logFiles[path];
  }
  
  FILE* f = fopen(path.c_str(), mode);
  if (f == 0) {
    fprintf(stderr, "could not open log file %s, aborting\n", path.c_str());
    ::perror("open");
    exit(-1);
  }

  logFiles[path] = f;
  return f;
} // openLogFile

void Log::closeLogFiles() {
  for (LogFileMap::iterator i = logFiles.begin(); i != logFiles.end(); i++) {
    FILE* f = i->second;
    fclose(f);
  }
} // closeLogFiles

void Log::enableLogging() {
  enabled = true;
} // enableLogging

void Log::disableLogging() {
  enabled = false;
} // disalbleLogging

void Log::log(const std::string& selector, log_level_t level, const string& message) {
  if (!enabled || level > logLevel) {
    return;
  }

  log(selector, message.c_str());
} // log

bool Log::isSelected(const string& s) {
  if (!selectors.containsKey(s)) {
    if (autoAll) {
      assert(autoAllDefault != 0);
      addl(s,
	   autoAllDefault->getFile(),
	   autoAllDefault->getLogTimestamp(),
	   autoAllDefault->getLogName(),
	   autoAllDefault->getLogThreadId(),
           autoAllDefault->getLogSelectorOutput());
      return true;
    }
    else if (!autoSelectors.empty()) {
      bool r = false;
      if(errorSelected && s.find("ERROR") != string::npos) {
	  addl(s,
	       stderr,
	       LOG_TIMESTAMP_DISABLED,
               LOG_NAME_ENABLED,
               LOG_THREADID_DISABLED,
               LOG_FPRINTF);
	  r = true;
      }
      else if(warningSelected && s.find("WARNING") != string::npos) {
	  addl(s,
	       stdout,
	       LOG_TIMESTAMP_DISABLED,
               LOG_NAME_ENABLED,
               LOG_THREADID_DISABLED,
               LOG_FPRINTF);
	  r = true;
      }
      for (SelectorList::iterator i = autoSelectors.begin();
          i != autoSelectors.end(); i++) {
        LogSelector* sel = *i;
        if (s.find(sel->getName()) != string::npos) {
          addl(s,
              sel->getFile(),
              sel->getLogTimestamp(),
              sel->getLogName(),
              sel->getLogThreadId(),
              sel->getLogSelectorOutput());
          r = true;
        }
      }
      return r;
    }
    return false;
  }
  return true;
} // isSelected

void Log::log(const string& s, log_level_t level, const char* m) {
  if (!enabled || level > logLevel) {
    return;
  }

  log(getId(s), level, m);
} // log

void Log::logf(const string& s, log_level_t level, const char* f ...) {
  if (!enabled || level > logLevel) {
    return;
  }

  va_list ap;
  va_start(ap, f);
  char tmp[MAX_MESSAGE_SIZE];
  vsnprintf(tmp, MAX_MESSAGE_SIZE - 1, f, ap);
  va_end(ap);
  log(s, level, tmp);
} // logf

void Log::logf(const string& s, const char* f ...) {
  if (!enabled) {
    return;
  }

  va_list ap;
  va_start(ap, f);
  char tmp[MAX_MESSAGE_SIZE];
  vsnprintf(tmp, MAX_MESSAGE_SIZE - 1, f, ap);
  va_end(ap);
  log(s, tmp);
} // logf

string Log::toHex(const string& h) {
  string r = "";
  for (size_t i = 0; i < h.size(); i++) {
    char c[3];
    sprintf(c, "%02hhx", (unsigned char)h[i]);
    r += c;
  }
  return r;
} // toHex

void Log::configure(Params* p) {
  LogSelectorTimestamp lts = LOG_TIMESTAMP_EPOCH;
  if (p->containsKey(Params::MACE_LOG_TIMESTAMP_HUMAN)) {
    if (p->getInt(Params::MACE_LOG_TIMESTAMP_HUMAN)) {
      lts = LOG_TIMESTAMP_HUMAN;
    }
  }

  if (p->containsKey(Params::MACE_LOG_LEVEL)) {
    int l = p->getInt(Params::MACE_LOG_LEVEL);
    setLevel(l == -1 ? MAX_LEVEL : l);
  }

  string filemode = "w";
  if (p->containsKey(Params::MACE_LOG_APPEND) && p->getInt(Params::MACE_LOG_APPEND)) {
    filemode = "a";
  }

  FILE* fp = stdout;
  if (p->containsKey(Params::MACE_LOG_FILE)) {
    StringList s = StrUtil::split("\n", p->getStr(Params::MACE_LOG_FILE));
    for (StringList::const_iterator i = s.begin(); i != s.end(); i++) {
      StringList l = StrUtil::split(" ", *i);
      if (l.size() == 1) {
// 	cerr << "logging auto selectors to " << l[0] << std::endl;
	fp = openLogFile(l[0], filemode.c_str());
      }
      else {
	string path = l.front();
	l.pop_front();
	for (StringList::const_iterator li = l.begin(); li != l.end(); li++) {
// 	  cerr << "logging " << *li << " to " << path << std::endl;
	  logToFile(path, *li, filemode.c_str(), lts);
	}
      }
    }
    
  }

  if (p->containsKey(Params::MACE_LOG_AUTO_ALL) && p->getInt(Params::MACE_LOG_AUTO_ALL)) {
    autoAddAll(fp, lts);
    return;
  }

  if (p->containsKey(Params::MACE_LOG_AUTO_SELECTORS)) {
    string tmp = p->getStr(Params::MACE_LOG_AUTO_SELECTORS);

    StringList s = StrUtil::split(" ", tmp);
    for (StringList::const_iterator i = s.begin(); i != s.end(); i++) {
      autoAdd(*i, fp, lts);
    }
  }
} // configure

void Log::perror(const std::string& s) {
  err() << s << ": " << Util::getErrorString(errno) << Log::endl;
} // Log::perror

void Log::lock() {
  assert(pthread_mutex_lock(&llock) == 0);
} // lock

void Log::unlock() {
  assert(pthread_mutex_unlock(&llock) == 0);
} // unlock

log_id_t Log::getTraceId(const std::string& selector) {
  static const bool isTrace = Params::Instance()->containsKey(Params::MACE_TRACE_FILE);
  if (isTrace) {
    return UINT_MAX;
  }
  else {
    return NULL_ID;
  }
} // getTraceId

Log::MaceTraceStream& Log::trace(log_id_t id) {
  TraceStreamMap& m = ThreadSpecific::getTraceStreams();

  // this does not need to be locked because this is thread-specific data
  TraceStreamMap::iterator i = m.find(id);
  MaceTraceStream* r = 0;
  if (i == m.end()) {
    r = new MaceTraceStream(id);
    m[id] = r;
  }
  else {
    r = i->second;
  }

  return *r;
} // trace

void Log::flush(Log::MaceTraceStream& s) {
  if (s.isNoop()) {
    return;
  }
  static FILE* out = openLogFile(Params::Instance()->get<std::string>(Params::MACE_TRACE_FILE), "wb");

  unsigned int vtid = ThreadSpecific::getVtid();
  vtid = htonl(vtid);
  fwrite(&vtid, sizeof(vtid), 1, out);
  fwrite(s.stream->str().data(), 1, s.stream->str().size(), out);
  fflush(out);
  s.stream->str("");
} // flush

void Log::traceNewThread(const mace::string& fname) {
  if (Util::REPLAY) {
    scheduleThread(UINT_MAX, fname);
  }
  else {
    if (!Params::Instance()->containsKey(Params::MACE_TRACE_FILE)) {
      return;
    }
    static FILE* out = openLogFile(Params::Instance()->get<std::string>(Params::MACE_TRACE_FILE), "wb");

    unsigned int vtid = htonl(UINT_MAX);
    fwrite(&vtid, sizeof(vtid), 1, out);
    std::string s;
    fname.serialize(s);
    fwrite(s.data(), 1, s.size(), out);
    fflush(out);
  }
} // traceNewThread

std::istream& Log::replay(log_id_t id) {
  scheduleThread(ThreadSpecific::getVtid(), "");
  return replayStream;
} // replay

void Log::scheduleThread(unsigned int vtid, const mace::string& desc) {
  static pthread_cond_t sig = PTHREAD_COND_INITIALIZER;
  static uint curId = UINT_MAX;
  static mace::string curDesc = desc;

  ASSERT(Util::REPLAY);

  lock();
  if (!replayStream.is_open()) {
    replayStream.open(Params::Instance()->get<std::string>(Params::MACE_TRACE_FILE).c_str());
  }
  ASSERT(replayStream);

  if (vtid != UINT_MAX) {
    mace::deserialize(replayStream, &curId, curId);
    if (curId == UINT_MAX) {
      curDesc.deserialize(replayStream);
    }
    pthread_cond_broadcast(&sig);
  }

  while (vtid != curId || (vtid == UINT_MAX && desc != curDesc)) {
    pthread_cond_wait(&sig, &llock);
  }
  unlock();
} // scheduleThread
