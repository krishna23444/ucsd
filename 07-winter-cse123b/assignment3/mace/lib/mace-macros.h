/* 
 * mace-macros.h : part of the Mace toolkit for building distributed systems
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
/**
 * General-purpose macros for all MACEDON_Agents
 */

#ifndef _MACEDON_MACROS_H
#define _MACEDON_MACROS_H

#ifndef MAX_LOG
#define MAX_LOG UINT_MAX
#endif

#ifndef SELECTOR_TAG
#define SELECTOR_TAG
#endif

#include "RandomUtil.h"
#include "LogIdSet.h"

// #define logf(args...) nologf()
// #define log(args...) nolog()
// #define convertToString(args...) noConvertToString("")
// #define LOGLOGTYPE const Log::NullOutputType
#define LOGLOGTYPE Log::MaceOutputStream

inline int randint(int x)		{ return RandomUtil::randInt(x); }
  
#define upcallAllVoid(func, args...) \
  for(std::map<int, typeof_upcall_##func*>::iterator _m_i = map_typeof_upcall_##func.begin(); \
      _m_i != map_typeof_upcall_##func.end(); _m_i++) { \
      upcall_##func(args, _m_i->first); \
  }

#define upcallAll(var, ag, func, args...) \
  for(std::map<int, typeof_upcall_##func*>::iterator _m_i = map_typeof_upcall_##func.begin(); \
      _m_i != map_typeof_upcall_##func.end(); _m_i++) { \
      ag(var, upcall_##func(args, _m_i->first)); \
  }

#define andEq(x, y) x = x && y
    
#define orEq(x, y) x = x || y

#define ADD_LOG_BACKING \
  LOGLOGTYPE* _maceout __attribute((unused)) = NULL;\
  LOGLOGTYPE* _macedbg[10] __attribute((unused)) = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };\
  LOGLOGTYPE* _macewarn __attribute((unused)) = NULL;\
  LOGLOGTYPE* _maceerr __attribute((unused)) = NULL;\
  LOGLOGTYPE* _macecompiler[5] __attribute((unused)) = { NULL, NULL, NULL, NULL, NULL };\

#define ADD_SELECTORS(x) \
  static const std::string selector = x; \
  static const LogIdSet* const selectorId __attribute((unused)) = new LogIdSet(selector); \
  static const log_id_t __trace_selector __attribute((unused)) = Log::getTraceId(x); \
  ADD_LOG_BACKING
    
#define ADD_FUNC_SELECTORS ADD_SELECTORS(__PRETTY_FUNCTION__)

#define PREPARE_FUNCTION \
  uint64_t _curtime __attribute__((unused)) = 0; \

//printf style
#define maceDebug(pri, args...) do { if(pri < MAX_LOG) { Log::logf(selectorId->debug, pri, args); } } while(0)
#if MAX_LOG != 0
#define maceLog(args...) Log::logf(selectorId->log, args)
#define maceWarn(args...) Log::logf(selectorId->warn, args)
#define maceError(args...) Log::logf(selectorId->error, args)
#else 
#define maceLog(args...)
#define maceWarn(args...)
#define maceError(args...)
#endif
#define maceCompiler(pri,args...) do { if(pri < MAX_LOG) { Log::logf(selectorId->compiler, pri, args); } } while(0)

//ostream style
#define traceout Log::trace(__trace_selector)
#define tracein Log::replay(__trace_selector)

#define maceout (_maceout == NULL? *(_maceout = &Log::log((0 != MAX_LOG?selectorId->log:Log::NULL_ID))): *_maceout)
#define macedbg(pri) (pri < 10? (_macedbg[pri] == NULL? *(_macedbg[pri] = &Log::log((pri < MAX_LOG?selectorId->debug:Log::NULL_ID), (log_level_t)pri)): *_macedbg[pri]): Log::log((pri < MAX_LOG?selectorId->debug:Log::NULL_ID), (log_level_t)pri))
#define macewarn (_macewarn == NULL? *(_macewarn = &Log::log((0 != MAX_LOG?selectorId->warn:Log::NULL_ID))): *_macewarn)
#define maceerr (_maceerr == NULL? *(_maceerr = &Log::log((0 != MAX_LOG?selectorId->error:Log::NULL_ID))): *_maceerr)
#define macecompiler(pri) (pri < 5? (_macecompiler[pri] == NULL? *(_macecompiler[pri] = &Log::log((pri < MAX_LOG?selectorId->compiler:Log::NULL_ID), (log_level_t)pri)): *_macecompiler[pri]): Log::log((pri < MAX_LOG?selectorId->compiler:Log::NULL_ID), (log_level_t)pri))

#define curtime (_curtime==0?(_curtime=Util::timeu()):_curtime)

#define EXPECT(x) if(!(x)) { maceerr << "EXPECT_FAILURE: Condition " << #x << " failed!" << Log::endl; } else 

#endif // _MACEDON_MACROS_H
