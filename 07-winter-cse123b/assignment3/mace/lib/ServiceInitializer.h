/* 
 * ServiceInitializer.h : part of the Mace toolkit for building distributed systems
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
#if __GNUC__ >= 3
#include <ext/hash_map>
#else
#include <hash_map>
#endif

#include "mstring.h"

#ifndef _SERVICE_INIT_H
#define _SERVICE_INIT_H

template<class T>
class ServiceInitializer {
public:
  typedef T (*init_function)();

private:
  __gnu_cxx::hash_map<mace::string, init_function, hash_string> registeredServices;
  
  static ServiceInitializer<T>* _inst;
  ServiceInitializer() {
  }

public:
  void registerService(mace::string name, init_function service) {
    //     printf("Registering function %x for protocol %d with this %x\n", (int)service, num, (int)this);
    registeredServices[name] = service;
  }
  

  T init(mace::string name) {
    T temp;
    if(registeredServices[name] != NULL) {
      temp = registeredServices[name]();
    } else {
      printf("No service registered for type %s with this %x!!!\n", name.c_str(), (int)this);
      exit(43);
    }
    return temp;
  }
  
  static ServiceInitializer<T>* Instance(){
    //     printf("Getting instance for ServiceInitializer<> _inst=%x\n", (int)_inst);
    if(_inst == NULL)
      _inst = new ServiceInitializer<T>;
    return _inst;
  }
};

template<class T>
ServiceInitializer<T>* ServiceInitializer<T>::_inst = NULL;

#endif // _SERVICE_INIT_H
