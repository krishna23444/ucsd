/* 
 * CgiWebObject.h : part of the Mace toolkit for building distributed systems
 * 
 * Copyright (c) 2006, James W. Anderson, Ryan Braud, Charles Killian
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
#ifndef CGI_WEB_OBJECT_H
#define CGI_WEB_OBJECT_H

#include "HttpRequestContext.h"
#include "WebObject.h"

class CgiWebObject : public WebObject {
public:
  CgiWebObject(const std::string& path, const std::string& root,
	       const std::string& pathInfo, const std::string& queryString,
	       const HttpRequestContext& req, bool headRequest = false);
  virtual ~CgiWebObject();
  virtual void close();
  virtual bool hasMoreChunks();
  virtual bool isChunkReady();
  virtual std::string readChunk() throw (HttpServerException);
  virtual pid_t getChildPid() const;

protected:
  virtual void exec(const HttpRequestContext& req);

private:
  void writeBlock();
  
private:
  std::string path;
  std::string root;
  std::string pathInfo;
  std::string queryString;
  pid_t childpid;
  bool isOpen;
  bool parseHeader;
  int rpipe;
  int wpipe;
  std::string inputBuf;
  std::string chunk;
  static const size_t BLOCK_SIZE = 8192; 
}; // CgiWebObject

#endif // CGI_WEB_OBJECT_H
