/*
 * Author: Nitay Joffe
 * Class: CSE 121 - Modern Operating Systems
 * Project: 1 - Filesystem
 */
#ifndef PATH_H_
#define PATH_H_

#define _GNU_SOURCE

#define FUSE_USE_VERSION 26
#include <fuse.h>

#define PATH_MAX_LENGTH (28)

extern const char PATH_DELIMITER_CHARACTER;
extern const char *PATH_DELIMITER_STRING;

unsigned int path_get_attributes(const char *path, struct stat *stbuf);

unsigned int path_create_directory(const char *path);
unsigned int path_create_file(const char *path);

unsigned int path_read_directory(const char *path, void *buf,
                                 fuse_fill_dir_t filler);

unsigned int path_read(const char *path, char *buf, size_t size, off_t offset);
unsigned int path_write(const char *path, const char *buf, size_t size,
                        off_t offset);

unsigned int path_delete(const char *path);

unsigned int path_rename(const char *from, const char *to);

#endif
