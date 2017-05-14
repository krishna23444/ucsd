/* 
 * mace.h : part of the Mace toolkit for building distributed systems
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
/*


*/
#ifndef _MACE_H
#define _MACE_H


// #include <string>
// #include "MaceTypes.h"
// #include "massert.h"
// #include "pip_includer.h"

#include "Scheduler.h"

#define MSEC *1000
#define SEC *1000 MSEC
#define ORACLE_COST 0
#define HEARTBEAT_SEND 4 SEC
#define HEARTBEAT_TIMEOUT 10 SEC

#define MACEDON_SANITY_CHECKS
#define MACEDON_MAX_TRACE_SIZE                   50000

#define MACEDON_MAX_PACKET_SIZE 1472

// Utility timer pops this often
#define MACEDON_UTIL_TIMER_VALUE  100 MSEC

// MACEDON engine packets are always LESS THAN 100
// #define MH_TYPE_HIGHER                    100

class MaceTimer;
class BaseMaceService;
class TimerEventHandler {};
  
// lock stuff
struct macedon_lock_info
{
  int lock_;
  int source_of_lock_;
  uint64_t time_of_lock_;
};

typedef void ( TimerEventHandler::*TimerCallbackFunction)();

class BaseMaceService : public TimerEventHandler
{

public:
  //Agent Stuff
  // general stuff
  static pthread_t childtid;
  static pthread_mutex_t agentlock;
//   static int lock_initialized;
  static int locked_for_writing;
  static int lock_count;
  static  uint64_t lock_taken;
  static pthread_t lock_owner;
  static bool _printLower;

  static void Lock();
  static void Unlock();

  //MACEDON_Agent stuff
  BaseMaceService();
  virtual ~BaseMaceService() {}

  /*   virtual int command(int argc, const char*const *argv)=0; */
  virtual void check_neighbors(uint64_t ctime)=0;

  // general stuff
  int num_nodes_;
  uint64_t time_booted;
  int topo_;
  int topo_nodes_;
  int seed_;
  int degree_;
  //   int macedon_sendret;

  // lock functions
  /*   void lock_on( int ); */
  /*   void lock_off(); */
  /*   int  test_lock(); */
  /*   int  test_lock_source(); */
  /*   void Lock(); */
  /*   void Unlock(); */

  // neighbor stuff
  // TODO: Is this still necessary?
  /*   macedon_neighbor_entry *get_neighbor(int); */
  /*   void queue_pkt(macedon_neighbor_entry *, macedon_packet_qent *); */

  // trace functions
  //   virtual void trace_print();

  //   int my_address()
  //   {
  //     return (here_.addr_);
  //   }

  // thread stuff
  struct macedon_thread_qent *threads;
  
  // join stuff
  /*   struct macedon_packet_qent *queued_join_; */

  // reliable delivery
  /*   class macedon_neighbor_entry *neighbors_; */
  int packet_count_;  
  
#ifdef MACEDON_SANITY_CHECKS
  /*   int mem_pkts_recvd_; */
  /*   int mem_pkts_sent_; */
  /*   int mem_pkts_alloced_; */
  /*   int mem_pkts_freed_; */
#endif
 
  struct macedon_lock_info lockstr_;
};

#endif // _MACE_H
