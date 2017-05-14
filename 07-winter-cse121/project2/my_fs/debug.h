/*
 * Author: Nitay Joffe
 * Class: CSE 121 - Modern Operating Systems
 * Project: 1 - Filesystem
 */
#ifndef DEBUG_H_
#define DEBUG_H_

//#define DEBUG

enum DebugLevel { NONE, COMMAND, PATH, SUPERBLOCK, DIRECTORY, FILES,
                  INODE, IO, ALL };

extern enum DebugLevel debug_on[ALL + 1];

void debug_start(const char *file, unsigned int line, const char *format, ...);
#define dbg_s(format, ...) \
  (debug_start(__FILE__, __LINE__, format, ##__VA_ARGS__))

void debug(const char *file, unsigned int line, const char *format, ...);
#define dbg(format, ...) \
  (debug(__FILE__, __LINE__, format, ##__VA_ARGS__))

void debug_end(const char *file, unsigned int line, const char *format, ...);
#define dbg_e(format, ...) \
  (debug_end(__FILE__, __LINE__, format, ##__VA_ARGS__))

#endif
