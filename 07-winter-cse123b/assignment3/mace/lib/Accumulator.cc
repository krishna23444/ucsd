/* 
 * Accumulator.cc : part of the Mace toolkit for building distributed systems
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
#include <inttypes.h>
#include "Accumulator.h"
#include "Util.h"
#include "SysUtil.h"
#include "ThreadCreate.h"

using std::cerr;
using std::endl;

Accumulator::AccumulatorMap Accumulator::instances;
bool Accumulator::isLogging = false;
pthread_t Accumulator::athread;
pthread_mutex_t Accumulator::alock = PTHREAD_MUTEX_INITIALIZER;
const std::string Accumulator::NETWORK_READ = "NETWORK_READ";
const std::string Accumulator::NETWORK_READ_SELECTOR = "Accumulator::NETWORK_READ";
const std::string Accumulator::NETWORK_WRITE = "NETWORK_WRITE";
const std::string Accumulator::NETWORK_WRITE_SELECTOR = "Accumulator::NETWORK_WRITE";
const std::string Accumulator::TCP_READ = "TCP_READ";
const std::string Accumulator::TCP_READ_SELECTOR = "Accumulator::TCP_READ";
const std::string Accumulator::TCP_WRITE = "TCP_WRITE";
const std::string Accumulator::TCP_WRITE_SELECTOR = "Accumulator::TCP_WRITE";
const std::string Accumulator::UDP_READ = "UDP_READ";
const std::string Accumulator::UDP_READ_SELECTOR = "Accumulator::UDP_READ";
const std::string Accumulator::UDP_WRITE = "UDP_WRITE";
const std::string Accumulator::UDP_WRITE_SELECTOR = "Accumulator::UDP_WRITE";
const std::string Accumulator::HTTP_CLIENT_READ = "HTTP_CLIENT_READ";
const std::string Accumulator::HTTP_CLIENT_READ_SELECTOR =
"Accumulator::HTTP_CLIENT_READ";
const std::string Accumulator::HTTP_CLIENT_WRITE = "HTTP_CLIENT_WRITE";
const std::string Accumulator::HTTP_CLIENT_WRITE_SELECTOR =
"Accumulator::HTTP_CLIENT_WRITE";
const std::string Accumulator::HTTP_SERVER_READ = "HTTP_SERVER_READ";
const std::string Accumulator::HTTP_SERVER_READ_SELECTOR =
"Accumulator::HTTP_SERVER_READ";
const std::string Accumulator::HTTP_SERVER_WRITE = "HTTP_SERVER_WRITE";
const std::string Accumulator::HTTP_SERVER_WRITE_SELECTOR =
"Accumulator::HTTP_SERVER_WRITE";
const std::string Accumulator::FILE_WRITE = "FILE_WRITE";
const std::string Accumulator::FILE_WRITE_SELECTOR = "Accumulator::FILE_WRITE";
const std::string Accumulator::TRANSPORT_SEND = "TRANSPORT_SEND";
const std::string Accumulator::TRANSPORT_SEND_SELECTOR = "Accumulator::TRANSPORT_SEND";
const std::string Accumulator::TRANSPORT_RECV = "TRANSPORT_RECV";
const std::string Accumulator::TRANSPORT_RECV_SELECTOR = "Accumulator::TRANSPORT_RECV";
const std::string Accumulator::TRANSPORT_RECV_CANCELED = "TRANSPORT_RECV_CANCELED";
const std::string Accumulator::TRANSPORT_RECV_CANCELED_SELECTOR = "Accumulator::TRANSPORT_RECV_CANCELED";
const std::string Accumulator::APPLICATION_SEND = "APPLICATION_SEND";
const std::string Accumulator::APPLICATION_SEND_SELECTOR = "Accumulator::APPLICATION_SEND";
const std::string Accumulator::APPLICATION_RECV = "APPLICATION_RECV";
const std::string Accumulator::APPLICATION_RECV_SELECTOR = "Accumulator::APPLICATION_RECV";

void* Accumulator::startLoggingThread(void* arg) {
  uint64_t interval = *(uint64_t*)arg;

  while (isLogging) {
    uint64_t now = Util::timeu();
    uint64_t sleepTime = interval;
    if (interval == 1*1000*1000) {
      sleepTime -= (now % (1000 * 1000));
      if (sleepTime < 500*1000) {
	sleepTime += interval;
      }
    }
    SysUtil::sleepu(sleepTime);
    lock();
    for (AccumulatorMap::iterator i = instances.begin();
	 i != instances.end(); i++) {
      Accumulator* a = i->second;
      Log::log(a->logId)
	<< a->getAmount() << " " << a->resetDiff() << Log::endl;
    }
    unlock();
  }

  return 0;
} // startLoggingThread

void Accumulator::dumpAll() {
  for(AccumulatorMap::const_iterator i = instances.begin();
      i != instances.end(); i++) {
    Log::logf("Accumulator::dumpAll", "accumulator %s amount %" PRIu64 "\n",
	      i->first.c_str(), i->second->getAmount());
  }
}

void Accumulator::logAll() {
  for (AccumulatorMap::const_iterator i = instances.begin();
       i != instances.end(); i++) {
    const Accumulator* a = i->second;
    Log::log(a->logId)
      << a->getAmount() << " " << a->getDiff() << Log::endl;
  }
} // logAll

void Accumulator::startLogging(uint64_t interval) {
  static uint64_t sint = 0;
  if (!isLogging) {
    isLogging = true;
    sint = interval;
    pthread_attr_t threadAttr;
    pthread_attr_init(&threadAttr);
    pthread_attr_setdetachstate(&threadAttr, PTHREAD_CREATE_JOINABLE);

    runNewThread(&athread, Accumulator::startLoggingThread, &sint, &threadAttr);
  }
  else {
    Log::err() << "cannot start Accumulator logging more than once" << Log::endl;
  }
} // log

void Accumulator::stopLogging() {
  isLogging = false;
}
