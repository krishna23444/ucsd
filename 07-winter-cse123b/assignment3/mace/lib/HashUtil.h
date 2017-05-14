/* 
 * HashUtil.h : part of the Mace toolkit for building distributed systems
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
#ifndef _HASH_UTIL_H
#define _HASH_UTIL_H

#include <string>
#include <openssl/sha.h>
#include <openssl/md2.h>
#include <openssl/evp.h>

#include "mstring.h"

typedef mace::string sha1;
typedef mace::string md5;

class HashUtil {

public:
  class Context {
  public:
    virtual ~Context();
    const mace::string& getHash();
    void update(const std::string& buf);
    void finalize();

  protected:
    Context(const EVP_MD* md);

  protected:
    const EVP_MD* md;
    EVP_MD_CTX* ctx;
    bool finalized;
    mace::string hash;
  }; // Context

  class MD5Context : public Context {
  public:
    MD5Context();
  }; // MD5Context

  class Sha1Context : public Context {
  public:
    Sha1Context();
  }; // Sha1Context

public:
  static void computeSha1(const std::string& buf, sha1& r);
  static void computeSha1(int buf, sha1& r);
  static void computeSha1(const void* buf, size_t size, sha1& r);
  static void computeMD5(const void* buf, size_t size, md5& r);
  static void computeFileSha1(const std::string& filePath, sha1& h, struct stat& sbuf,
			      bool dostat = true) throw (FileException);
  static void computeFileMD5(const std::string& filePath, md5& h, struct stat& sbuf,
			     bool dostat = true) throw (FileException);
  static size_t compute32bit(const std::string& buf) {
    static const hash_string hasher = hash_string();
    return hasher(buf);
  }
    
  static std::string compute32bitHex(const std::string& buf) {
    size_t hash = compute32bit(buf);
    std::string h((const char*)&hash, sizeof(hash));
    std::string r = "";
    for (size_t i = 0; i < h.size(); i++) {
      char c[3];
      sprintf(c, "%02hhx", (unsigned char)h[i]);
      r += c;
    }
    return r;
  }

  static void computeHash(const void* buf, size_t size, std::string& r,
			  const EVP_MD* md);
  static void computeFileHash(const std::string& path, std::string& h,
			      struct stat& sbuf, bool dostat,
			      const EVP_MD* md) throw(FileException);

  static const sha1& NULL_HASH;

protected:
  static EVP_MD_CTX* initCTX(const EVP_MD* md);
  static void finalizeCTX(EVP_MD_CTX* mdctx, std::string& r);
  static void freeCTX(EVP_MD_CTX* mdctx);
  static sha1& nullHash();


protected:
  static const size_t BLOCK_SIZE;
  static const size_t MAX_RETRY_COUNT;
}; // HashUtil


#endif // _HASH_UTIL_H
