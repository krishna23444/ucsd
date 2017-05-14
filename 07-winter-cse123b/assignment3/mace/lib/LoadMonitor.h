/* 
 * LoadMonitor.h : part of the Mace toolkit for building distributed systems
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
#ifndef __LOAD_MONITOR__H
#define __LOAD_MONITOR__H
// #include <pthread.h>
#include <fstream>
#include <cassert>
#include "Scheduler.h"

class HiResLoadMonitor : public TimerHandler {
public:
  class ProcStat {
  public:
    ProcStat() : utime(0), stime(0), vsize(0), rss(0) { }
    uint utime;
    uint stime;
    uint vsize;
    int rss;
  }; // ProcStat

  void expire();
  static void start(uint64_t freq = DEFAULT_FREQUENCY);
  static void stop();

public:
  static const uint64_t DEFAULT_FREQUENCY = 1*1000*1000;

private:
  HiResLoadMonitor(uint64_t freq);
  void readProcStat(ProcStat& stat);

private:
  static HiResLoadMonitor* instance;
  bool halt;
  uint64_t frequency;
  uint64_t prevTime;
  pid_t pid;
  ProcStat stat;
  std::ifstream statfile;
  std::string path;
}; // HiResLoadMonitor

class LoadMonitor : public TimerHandler {
public:
  LoadMonitor();
  void expire();
  static void runLoadMonitor();
  static void stopLoadMonitor();
  static double getLoad() { return instance ? instance->curLoad[0] : 0; }

public:
  static const uint64_t DEFAULT_FREQUENCY = 5*1000*1000;

private:
  static LoadMonitor* instance;
  bool halt;
  uint64_t frequency;
  std::ifstream loadfile;
  std::ifstream cpufile;
  double load;
  double curLoad[3];
  uint32_t lastCPU[4];
  uint32_t curCPU[4];
  double peakload[3];
  uint32_t lastCPU_sum;

  
};
#endif
