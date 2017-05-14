/*
 * Author: Nitay Joffe
 * Class: CSE 121 - Modern Operating Systems
 * Project: 1 - Filesystem
 */
#ifndef FILE_H_
#define FILE_H_

#include <sys/types.h>
#include "inode.h"
#include "io.h"

struct FileBlockOnDisk {
  char data[BLOCKSIZE];
};

struct FileBlock {
  struct FileBlockOnDisk on_disk;

  unsigned int block_number;
};

struct File {
  struct Inode inode;
};

unsigned int file_create_inode();

void file_read_inode(unsigned int inode_number, struct File *file);

unsigned int file_read(const struct File *file, size_t size, off_t offset,
                       char *buffer);

unsigned int file_write(size_t size, off_t offset, const char *buffer,
                        struct File *file);

#endif
