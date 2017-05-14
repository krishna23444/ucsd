/* 
 * mace.cc : part of the Mace toolkit for building distributed systems
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
#include <errno.h>
#include "mace.h"
// #include "Log.h"
// #include "Params.h"
#include "Util.h"

pthread_t BaseMaceService::childtid;
// pthread_mutex_t BaseMaceService::agentlock = PTHREAD_ERRORCHECK_MUTEX_INITIALIZER_NP; 
pthread_mutex_t BaseMaceService::agentlock = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
// int BaseMaceService::lock_initialized = 1;

int BaseMaceService::lock_count = 0;
uint64_t BaseMaceService::lock_taken;
int BaseMaceService::locked_for_writing;
pthread_t BaseMaceService::lock_owner = (pthread_t)0;
bool BaseMaceService::_printLower = false;

BaseMaceService::BaseMaceService() 
{
//   if(!BaseMaceService::lock_initialized) {
//     lock_count= 0;
//     lock_owner = (pthread_t)0;

//     BaseMaceService::lock_initialized = 1;
//     /*     pthread_attr_t attr; */

//     // the rest of the stuff is only done for the top-level agent
//     pthread_mutexattr_t    attrm;
//     pthread_mutexattr_settype(&attrm, PTHREAD_MUTEX_ERRORCHECK);
//     //    pthread_mutexattr_settype(&attrm, PTHREAD_MUTEX_RECURSIVE);
//     pthread_mutex_init(&(BaseMaceService::agentlock), &attrm);
//   }


#ifndef __CYGWIN__
  if(sysconf(_SC_THREADS)==-1){

    printf("Threads not supported\n");
    exit(69); 
  }
#endif

  time_booted= Util::timeu();

}

#define AGENT_LOCK_DEBUG 0
#define AGENT_LOCK_TIME 0
// ---------------------------------------------- 
// lock functions, read and base(write)
// ---------------------------------------------- 

void  BaseMaceService::Lock ()
{
#if AGENT_LOCK_DEBUG > 1
  printf("%d Agent_lock in : %d %d %x %d %x %d \n", pthread_self(),
      agentlock.__m_reserved,
      agentlock.__m_count,
      agentlock.__m_owner,
      agentlock.__m_kind,
      agentlock.__m_lock.__status,
      agentlock.__m_lock.__spinlock
      );
  //        fflush(stdout);
#endif
  pthread_mutex_lock(&(agentlock));
#if AGENT_LOCK_DEBUG > 0
  printf("%d Agent_lock out %d\n" , pthread_self(),lock_count);
#endif
}

// ---------------------------------------------- 
// unlock
// ---------------------------------------------- 

void  BaseMaceService::Unlock ()
{
#if AGENT_LOCK_DEBUG > 1
  printf("%d Agent_unlock in : %d %d %x %d %x %d \n", pthread_self(),
      agentlock.__m_reserved,
      agentlock.__m_count,
      agentlock.__m_owner,
      agentlock.__m_kind,
      agentlock.__m_lock.__status,
      agentlock.__m_lock.__spinlock
      );
#endif
  pthread_mutex_unlock(&(agentlock));
#if AGENT_LOCK_DEBUG > 0
  printf("%d Agent_unlock out %d %d\n", pthread_self(), lock_count,lock_owner );
#endif
}

