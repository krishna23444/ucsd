/* 
 * massert.h : part of the Mace toolkit for building distributed systems
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

#ifndef ASSERT_ONCE
#define ASSERT_ONCE

// #define ASSERT_EXIT_MECHANISM char* __a = NULL; *__a = 5; //segfault
#define ASSERT_EXIT_MECHANISM abort(); //abort

#define OUTPUT_FORMAT(x) "Assert(%s) failed in file %s (included from %s), line %d method %s\n",#x,__FILE__,__BASE_FILE__,__LINE__,__PRETTY_FUNCTION__
#define OUTPUT_STREAM(x) "Assert(" #x ") failed in file " __FILE__ " (included from " __BASE_FILE__ "), line " << __LINE__ << " method " << __PRETTY_FUNCTION__ 

#endif //ASSERT_ONCE

#ifdef __LOG_H_DONE
#ifdef ASSERT
#undef ASSERT
#endif //ASSERT
#define ASSERT(x) \
   { \
     if(x) {}\
     else {\
       printf(OUTPUT_FORMAT(x));\
       fprintf(stderr, OUTPUT_FORMAT(x));\
       Log::err() << OUTPUT_STREAM(x) << Log::endl;\
       ASSERT_EXIT_MECHANISM \
     }\
   }
#else //__LOG_H_DONE
#ifndef ASSERT
#define ASSERT(x) \
   { \
     if(x) {}\
     else {\
       printf(OUTPUT_FORMAT(x));\
       fprintf(stderr, OUTPUT_FORMAT(x));\
       ASSERT_EXIT_MECHANISM \
     }\
   }
#endif //ASSERT
#endif //__LOG_H_DONE
