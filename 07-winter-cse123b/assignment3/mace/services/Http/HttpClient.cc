/* 
 * HttpClient.cc : part of the Mace toolkit for building distributed systems
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
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <getopt.h>
#include <cassert>
#include <sstream>
#include <fstream>
#include <iostream>
#include <errno.h>
#include <boost/lexical_cast.hpp>

#include "Accumulator.h"
#include "Log.h"
#include "StrUtil.h"
#include "Util.h"
#include "SysUtil.h"
#include "SockUtil.h"
#include "FileUtil.h"
#include "mace-macros.h"

#include "HttpParser.h"
#include "HttpClient.h"

using namespace std;
using boost::lexical_cast;

const string HttpClient::DEFAULT_USER_AGENT = "MaceHttpClient/1.0";

HttpClient::HttpClient(bool async, const string& userAgent) :
  userAgent(userAgent), s(0) {
  init(async);
} // HttpClient

HttpClient::HttpClient(const string& host, uint16_t p, bool async,
		       const string& userAgent) : userAgent(userAgent), s(0) {
  
  init(async);
  setServer(host, p);
} // HttpClient

void HttpClient::init(bool async) {
  SysUtil::signal(SIGPIPE, SIG_IGN, false);

  if (async) {
    requests = new HttpClientRequestQueue(
      HttpClientRequestCallback(this, &HttpClient::execute));
  }
  else {
    requests = 0;
  }
} // init

HttpClient::~HttpClient() {
  closeConnection();

  if (requests) {
    delete requests;
    requests = 0;
  }
} // ~HttpClient

void HttpClient::setServer(const string& host, uint16_t p) {
  if ((server == host) && (port == p)) {
    return;
  }

  closeConnection();

  server = host;
  port = p;
} // setServer

std::string HttpClient::setServerFromUrl(std::string url) {
  string http = "http://";
  if (url.find(http) == 0) {
    url = url.substr(http.size());
  }

  StringList m = StrUtil::match("([^/]+)/(.*)", url);
  if (m.empty()) {
    Log::err() << "bad url " << url << Log::endl;
    assert(0);
  }

  string host = m[0];
  string path = "/" + m[1];

  int p = 80;
  m = StrUtil::match("([^:]+):(\\d+)", host);
  if (!m.empty()) {
    host = m[0];
    p = lexical_cast<int>(m[1]);
  }

  setServer(host, p);

  return path;
} // setServerFromUrl

HttpResponse HttpClient::get(const std::string& url, const std::string& httpVersion)
  throw (HttpClientException) {

  string path = setServerFromUrl(url);
  return getUrl(path, httpVersion);
} // get

HttpResponse HttpClient::post(const std::string& url, const std::string& contentType,
			      const std::string& content, const std::string& httpVersion)
  throw (HttpClientException) {

  string path = setServerFromUrl(url);
  return postUrl(path, contentType, content, httpVersion);
} // post

HttpResponse HttpClient::getUrl(const string& url, const string& httpVersion,
				bool keepAlive) throw (HttpClientException) {


  string req = getRequest("GET", url, httpVersion, keepAlive);
  sendRequest(req);

  HttpResponse response(server, port, url);
  readResponse(response);

  return response;
} // getUrl

void HttpClient::getUrlAsync(const string& url, const string& httpVersion,
			     bool keepAlive,
			     HttpClientResponseHandler* c,
			     HttpClientResponseCallback f) {
  assert(requests);
  // we are calling this to clear the queue of async callback results
  hasAsyncResponse();
  HttpClientResponseState* rs = new HttpClientResponseState(server, port, url, c, f);
  rs->request = getRequest("GET", url, httpVersion, keepAlive);
  requests->enqueue(rs);
} // getUrlAsync

void HttpClient::postUrlAsync(const std::string& url, const std::string& contentType,
			      const std::string& content, const std::string& httpVersion,
			      bool keepAlive,
			      HttpClientResponseHandler* c,
			      HttpClientResponseCallback f) {
  assert(requests);
  // we are calling this to clear the queue of async callback results
  hasAsyncResponse();
  HttpClientResponseState* rs = new HttpClientResponseState(server, port, url, c, f);
  rs->request = getRequest("POST", url, httpVersion, keepAlive, contentType, content);
  requests->enqueue(rs);
} // postUrlAsync

bool HttpClient::hasAsyncResponse() {
  assert(requests);
  if (!requests->empty()) {
    HttpClientResponseState* r = requests->front();
    while (r->obj && r->callback) {
      r = requests->dequeue();
      delete r;
      if (requests->empty()) {
	break;
      }
      r = requests->front();
    }
  }
  return !requests->empty();
} // hasAsyncResponse

HttpResponse HttpClient::getAsyncResponse() throw (HttpClientException) {
  assert(requests);
  HttpClientResponseState* r = 0;
  do {
    delete r;
    r = requests->dequeue();
  } while (r->obj && r->callback);

  HttpResponse rr;
  HttpClientException e("");
  bool thr = r->hasException;
  if (thr) {
    e = r->exception;
  }
  else {
    rr = r->response;
  }

  delete r;
  r = 0;

  if (thr) {
    closeConnection();
    throw e;
  }

  return rr;
} // getAsyncResponse

HttpResponse HttpClient::postUrl(const string& url, const string& contentType,
				 const string& content, const string& httpVersion,
				 bool keepAlive) throw (HttpClientException) {
  string req = getRequest("POST", url, httpVersion, keepAlive, contentType, content);
  sendRequest(req);

  HttpResponse response(server, port, url);
  readResponse(response);

  return response;
} // postUrl

void HttpClient::sendRequest(const string& req) throw (HttpClientException) {
  try {
    connectToServer();

    size_t sendCount = 0;
    bool retrySend = false;
    do {
      sendCount++;
      try {
	FileUtil::write(s, req);
	Accumulator::Instance(Accumulator::HTTP_CLIENT_WRITE)->accumulate(req.size());
	retrySend = false;
      } catch (const PipeClosedException& e) {
	// server closed connection, try to reconnect
	closeConnection();
	if (sendCount > 3) {
	  throw HttpClientException("could not send request to server " +
				    server + ": " + e.toString());
	}
	
	connectToServer();
	retrySend = true;
      }
    } while (retrySend);

  } catch (const WriteException& e) {
    closeConnection();
    throw HttpClientException("could not send request to server " + server + ": " +
			      e.toString());
  }
} // sendRequest

void HttpClient::readResponse(HttpResponse& response) throw (HttpClientException) {
  ADD_SELECTORS("HttpClient::readResponse");
  Accumulator* accum = Accumulator::Instance(Accumulator::HTTP_CLIENT_READ);
  try {
    mace::string rbuf;
    StringList headerList;
    size_t lengthUsed = 0;
    mace::string buf = readBuffer;
    readBuffer.clear();
    do {
      size_t r = 0;
      try {
	r = FileUtil::read(s, buf);
	accum->accumulate(r);
      } catch (const ReadException& e) {
	closeConnection();
	throw HttpClientException(e.toString());
      }

      if (r == 0) {
	closeConnection();
	throw HttpClientException("server " + server + " closed connection");
      }

      rbuf = buf;
      lengthUsed = 0;
    } while (!HttpParser::readHeader(rbuf, headerList, lengthUsed));

    if (!HttpParser::parseResponseHeaderLine(headerList, response)) {
      closeConnection();
      throw HttpClientException("invalid response from " + server);
    }

    HttpRequestContext context;
    // this is needed so that we can correctly parse content-length
    context.method = "POST";
    size_t contentLength = 0;
    if (!HttpParser::parseHeaders(headerList, context, contentLength)) {
      closeConnection();
      throw HttpClientException("invalid response from " + server);
    }

    response.headers = context.headers;
    if (context.headers.containsKey("Connection")) {
      response.keepAlive = context.keepAlive;
    }

    if (context.chunked) {
      bool valid = false;
      bool done = false;
      size_t r = 0;
      do {
	HttpParser::readChunked(rbuf, response.content, lengthUsed, valid, done);

	if (!done) {
	  r = FileUtil::read(s, rbuf);
	  accum->accumulate(r);
	}
      } while (!done && (r > 0));

      if (!valid) {
	closeConnection();
	throw HttpClientException("incomplete response: did not receive valid header");
      }

      readBuffer = rbuf;
    }
    else if (0 == contentLength) {
      size_t r = 0;
      response.content = rbuf;
      do {
	r = FileUtil::read(s, response.content);
	accum->accumulate(r);
      } while (r > 0);
    }
    else {
      // read the rest of the content, accounting for data we already have in rbuf
      if (rbuf.size() == contentLength) {
	response.content = rbuf;
      }
      else if (rbuf.size() > contentLength) {
	response.content = rbuf.substr(0, contentLength);
	readBuffer.append(rbuf.substr(contentLength));
      }
      else {
	size_t remaining = contentLength - rbuf.size();
	response.content.append(rbuf);
	size_t r = FileUtil::bufferedRead(s, rbuf, remaining);
	accum->accumulate(r);
	if (r != remaining) {
	  closeConnection();
	  throw HttpClientException("incomplete response: expected " +
				    lexical_cast<std::string>(contentLength) +
				    " but read only " +
				    lexical_cast<std::string>(rbuf.size()) +
				    " r=" + lexical_cast<std::string>(r) +
				    " remaining=" + lexical_cast<std::string>(remaining));
	}
	response.content.append(rbuf);
      }
    }

  } catch (const ReadException& e) {
    closeConnection();
    throw HttpClientException("error reading response from server " + server + ": " +
			      e.toString());
  }

  if (!response.keepAlive) {
    closeConnection();
  }
} // readResponse

string HttpClient::getRequest(const string& method, const string& path,
			      const string& version, bool keepAlive,
			      const string& contentType, const string& content) {

  string req = method + " " + path + " HTTP/" + version + "\r\n";
  if (version == "1.1") {
    req += "Host: " + server + "\r\n";
  }
  if (!userAgent.empty()) {
    req += "User-Agent: " + userAgent + "\r\n";
  }
  if (keepAlive) {
    req += "Connection: keep-alive\r\n";
  }
  if (!contentType.empty()) {
    req += "Content-Type: " + contentType + "\r\n";
  }
  if (!content.empty()) {
    req += "Content-Length: " + lexical_cast<std::string>(content.size()) + "\r\n";
  }
  
  req += "\r\n";

  if (!content.empty()) {
    req += content;
  }

  return req;
} // getRequest

void HttpClient::closeConnection() {
  readBuffer.clear();
  if (s) {
    shutdown(s, SHUT_RDWR);
    close(s);
    s = 0;
  }
} // closeConnection

void HttpClient::setResponseSignal(pthread_cond_t* sig, pthread_mutex_t* siglock) {
  assert(requests);
  requests->setResultSignal(sig, siglock);
} // setResponseSignal

void HttpClient::clearResponseSignal() {
  assert(requests);
  requests->clearResultSignal();
} // clearResponseSignal

HttpClientResponseState* HttpClient::execute(HttpClientResponseState* r) {
  try {
    sendRequest(r->request);
    readResponse(r->response);
  } catch (const HttpClientException& e) {
    r->hasException = true;
    r->exception = e;
  }

  if (r->obj && r->callback) {
    ((r->obj)->*(r->callback))(*r);
  }
  
  return r;
} // execute

void HttpClient::connectToServer() throw (HttpClientException) {
  if (!s) {
    struct sockaddr_in sa;
    SockUtil::fillSockAddr(server, port, sa);

    s = connectToServer(sa);
  }
} // connectToServer

int HttpClient::connectToServer(const struct sockaddr_in& sin)
  throw (HttpClientException) {
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0) {
    Log::perror("socket");
    assert(0);
  }

  int ret = connect(fd, (struct sockaddr*)&sin, sizeof(sin));

  if (ret == 0) {
    // in progress
  }
  else {
    string errstr = strerror(errno);
    ostringstream os;
    os << "connect: " << errstr << " on " << server << ":" << port;
    closeConnection();
    throw HttpClientException(os.str());
  }

  return fd;
} // connectToServer
