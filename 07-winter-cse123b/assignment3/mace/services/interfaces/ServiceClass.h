/* 
 * ServiceClass.h : part of the Mace toolkit for building distributed systems
 * 
 * Copyright (c) 2006, Charles Killian, James W. Anderson, Ryan Braud
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
#ifndef SERVICE_CLASS_H
#define SERVICE_CLASS_H

#include <cassert>
#include "lib/MaceTypes.h"
#include "lib/Util.h"
#include "NullPrintable.h"

class ServiceClass : public virtual PrintPrintableVoid {
  public:
  virtual void maceInit() { }
  virtual void maceExit() { } //Called when exiting.
  virtual void maceReset() { }
  virtual MaceKey getLocalAddress(registration_uid_t regId = -1) const { 
    static const MaceKey local(ipv4, Util::getMaceAddr());
    return local;
  }
  virtual uint32_t hashState() const { return (unsigned long)this & 0xFFFFFFFF; }

  virtual ~ServiceClass() {}

  static ServiceClass& NULL_;
};

// class service_state_hash {
//   size_t operator() (const ServiceClass& __x) const {
//     return __x.hashState();
//   }
//   size_t operator() (const ServiceClass* __x) const {
//     return __x->hashState();
//   }
// };

#endif