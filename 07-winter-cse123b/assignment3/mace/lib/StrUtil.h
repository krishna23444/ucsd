/* 
 * StrUtil.h : part of the Mace toolkit for building distributed systems
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
#include <sys/types.h>
#include <regex.h>
#include <ctype.h>
#include <string>
#include <sstream>

#include "LRUCache.h"
#include "mhash_map.h"
#include "mdeque.h"
#include "hash_string.h"
#include "Exception.h"
#include "Collections.h"
#include "StrUtilNamespace.h"

#ifndef STRUTIL_H
#define STRUTIL_H

typedef mace::LRUCache<std::string, regex_t*, hash_string> RegexCache;

class RegexException : public Exception {
public:
  RegexException(const std::string& m) : Exception(m) { }
  void rethrow() const { throw *this; }
}; // RegexException

class StrUtil {
public:
  static std::string trim(const std::string& s);
  static std::string trimFront(const std::string& s);
  static std::string toLower(std::string s);
  static std::string toUpper(std::string s);

  // replace all occurrences of target in src with replacement
  static std::string replaceAll(std::string src, const std::string& target,
				const std::string& replacement);

  // reads a line terminated with \n or \r\n from src into dst, returns
  // number of bytes extracted from src
  static size_t readLine(std::string& src, std::string& dst);

  static size_t read(std::string& src, std::string& dst, size_t n);

  static bool matches(const std::string& regexp, const std::string& s,
		      bool ignoreCase = false, bool matchNewline = false)
    throw (RegexException);

  static StringList match(const std::string& regexp, const std::string& s,
			  bool ignoreCase = false, bool matchNewline = false)
    throw (RegexException);

  static std::string translatePerlRE(std::string re);

  static void toCStr(const StringList& l, const char* a[]);

  static StringList split(const std::string& delim, std::string s, bool returnEmpty = false);
  static std::string join(const std::string& delim, const StringList& l);

  static bool isPrintable(const std::string& s);
  static std::string spaces(size_t w, size_t l);

private:
  static void throwRegexException(const std::string& re, int error, regex_t* rex)
    throw (RegexException);
  static regex_t* compileRegex(std::string re, bool ignoreCase, bool matchNewline,
			       bool nosub) throw (RegexException);
  static void freeRegex(const std::string& re, regex_t& rex);

private:

  static StringIntHMap cflagMap;
  static RegexCache regexCache;

};

#endif // STRUTIL_H
