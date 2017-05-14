/* 
 * RouteTransportWrapper.m : part of the Mace toolkit for building distributed systems
 * 
 * Copyright (c) 2006, James W. Anderson, Charles Killian
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
service RouteTransportWrapper;

provides Route;

trace=med;

services 
{
  Transport trans = TcpTransport();
}

messages 
{
  Data {
    MaceKey source;
    MaceKey dest;
    registration_uid_t regId;
    mace::string data;
  }
}

state_variables {
  MaceKey me;
}

transitions 
{
  downcall maceInit() {
    me = downcall_getLocalAddress();
  }
  downcall route(const MaceKey& dest, const std::string& msg, registration_uid_t regId) {
    MaceKey nextHop = dest;
    if(upcall_forward(me, dest, nextHop, msg, regId)) {
      return downcall_route(nextHop, Data(me, dest, regId, msg));
    }
    return true;
  }
  upcall deliver(const MaceKey& src, const MaceKey& dest, const Data& msg) {
    MaceKey nextHop = me;
    if(upcall_forward(msg.source, msg.dest, nextHop, msg.data, msg.regId)) {
      if(nextHop == me) {
        upcall_deliver(msg.source, msg.dest, msg.data, msg.regId);
      } else {
        downcall_route(nextHop, msg);
      }
    }
  }
}
