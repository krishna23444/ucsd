/* 
 * XmlRpcServer.cc : part of the Mace toolkit for building distributed systems
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
#include "XmlRpcServer.h"

using namespace std;

XmlRpcServer::XmlRpcServer(const string& url, uint16_t port, int backlog) :
  deleteServer(true) {

  httpd = new HttpServer(port, backlog);
  setupServer(url);
} // XmlRpcServer

XmlRpcServer::XmlRpcServer(const string& url, HttpServer* s) : deleteServer(false) {
  httpd = s;
  setupServer(url);
} // XmlRpcServer

XmlRpcServer::~XmlRpcServer() {
  shutdown();
  if (deleteServer) {
    delete httpd;
    httpd = 0;
  }
} // ~XmlRpcServer

void XmlRpcServer::registerHandler(const string& className, XmlRpcHandler* handler,
				   SStringSet* methods) {

  xh.registerHandler(className, handler, methods);
} // registerHandler

void XmlRpcServer::run() {
  httpd->run();
} // run

void XmlRpcServer::shutdown() {
  httpd->shutdown();
  xh.shutdown();
} // shutdown

void XmlRpcServer::setupServer(const string& url) {
  httpd->registerUrlHandler(url, &xh);
} // setupServer
