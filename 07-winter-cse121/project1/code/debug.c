/*
 * Author: Nitay Joffe
 * Class: CSE 121 - Modern Operating Systems
 * Project: 1 - Filesystem
 */
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "config.h"
#include "debug.h"

enum DebugLevel debug_on[ALL + 1] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };

static const unsigned int SPACING = 2;
static unsigned int space = 0;

static void print_space() {
  int i;
  for (i = 0; i < space; ++i)
    fprintf(stderr, " ");
}

static void print_info(const char *file, unsigned int line) {
  print_space();
  fprintf(stderr, "[%s:%d] ", file, line);
}

static unsigned int debug_level_for_file(const char *file) {
  if (strncmp("cse121fs.c",  file, strlen(file)) == 0)
    return COMMAND;
  else if (strncmp("directory.c", file, strlen(file)) == 0)
    return DIRECTORY;
  else if (strncmp("file.c", file, strlen(file)) == 0)
    return FILES;
  else if (strncmp("inode.c", file, strlen(file)) == 0)
    return INODE;
  else if (strncmp("io.c", file, strlen(file)) == 0)
    return IO;
  else if (strncmp("mkcse121fs.c", file, strlen(file)) == 0)
    return COMMAND;
  else if (strncmp("path.c", file, strlen(file)) == 0)
    return PATH;
  else if (strncmp("superblock.c", file, strlen(file)) == 0)
    return SUPERBLOCK;
  else
    return NONE;
}

int debug_enabled_for(const char *file) {
  if (debug_on[NONE])
    return 0;

  return debug_on[debug_level_for_file(file)] || debug_on[ALL];
}

void debug_start(const char *file, unsigned int line, const char *format, ...) {
#ifdef DEBUG
  if (!debug_enabled_for(file))
    return;

  for (; format != NULL && *format == '\n'; format++)
    fprintf(stderr, "\n");

  va_list ap;
  va_start(ap, format);

  print_info(file, line);
  vfprintf(stderr, format, ap);

  va_end(ap);
  
  space += SPACING;
#endif
}

void debug(const char *file, unsigned int line, const char *format, ...) {
#ifdef DEBUG
  if (!debug_enabled_for(file))
    return;

  for (; format != NULL && *format == '\n'; format++)
    fprintf(stderr, "\n");

  va_list ap;
  va_start(ap, format);

  print_info(file, line);
  vfprintf(stderr, format, ap);

  va_end(ap);
#endif
}

void debug_end(const char *file, unsigned int line, const char *format, ...) {
#ifdef DEBUG
  if (!debug_enabled_for(file))
    return;

  for (; format != NULL && *format == '\n'; format++)
    fprintf(stderr, "\n");

  va_list ap;
  va_start(ap, format);

  print_info(file, line);
  vfprintf(stderr, format, ap);

  va_end(ap);
  
  space -= SPACING;
#endif
}
