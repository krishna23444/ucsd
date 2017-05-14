/**
 * Authors: Erik Buchanan, Nitay Joffe
 * Class: CSE 123B - Networking
 * Assignment: 1 - Web Server
 * Date: 1/21/07
 */
#include <stdarg.h>
#include <stdio.h>

#include "debug.h"

static unsigned int debug_is_enabled = 0;

void debug_disable() {
  debug_is_enabled = 0;
}

void debug_enable() {
  debug_is_enabled = 1;
}

void dbg(const char* format, ...) {
  if (debug_is_enabled) {
    va_list ap;
    va_start(ap, format);
    vprintf(format, ap);
    va_end(ap);
  }
}
