/*
 * Author: Nitay Joffe
 * Class: CSE 121 - Modern Operating Systems
 * Project: 1 - Filesystem
 *
 * This program is intended to format your disk file, and should be executed
 * BEFORE any attempt is made to mount your file system.  It will not, however
 * be called before every mount.
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../benchmark/benchmark.h"

#include "debug.h"
#include "directory.h"
#include "inode.h"
#include "io.h"
#include "superblock.h"

static void usage(const char *program_name) {  
  printf("USAGE: %s diskSizeInBlockSize\n", program_name);
  printf("  diskSizeInBlockSize < %u\n", INVALID);
}

static void data_block_free_list_initialize() {
  dbg_s("initializing data block free list\n");
  unsigned char buffer[BLOCKSIZE] = "";
  
  unsigned int block_number;
  unsigned int next_block_number;

  for (block_number = first_data_block_number();
       block_number < last_data_block_number(); ++block_number) {
    next_block_number = block_number + 1;
    memcpy(buffer, &next_block_number, sizeof(next_block_number));
    dbg("writing free data block %u -> %u\n", block_number, next_block_number);
    kwrite(block_number, buffer);
  }

  next_block_number = INVALID;
  memcpy(buffer, &next_block_number, sizeof(next_block_number));
  dbg("writing free data block %u -> %u\n", last_data_block_number(),
      next_block_number);
  kwrite(last_data_block_number(), buffer);
  dbg_e("initialized data block free list\n");
}

static void inode_free_list_initialize() {
  dbg_s("initializing inode free list\n");
  unsigned int inode_block_number = FIRST_INODE_BLOCK_NUMBER;

  while (inode_block_number <= last_inode_block_number()) {
    struct FreeInodeBlock free_inode_block;

    unsigned int inode_number
      = (inode_block_number  - FIRST_INODE_BLOCK_NUMBER) *INODE_COUNT_PER_BLOCK;
    unsigned int last_inode_number = inode_number + INODE_COUNT_PER_BLOCK - 1;

    while (inode_number <= last_inode_number) {
      struct FreeInode free_inode;
      free_inode.number = inode_number;
      
      if (inode_block_number == last_inode_block_number() &&
          inode_number == last_inode_number)
        free_inode.on_disk.next_free_inode_number = INVALID;
      else
        free_inode.on_disk.next_free_inode_number = inode_number + 1;

      free_inode_block.block_number = inode_block_number;
      
      free_inode_block.inodes[inode_number %INODE_COUNT_PER_BLOCK] = free_inode;
      
      ++inode_number;
    }
    ++inode_block_number;

    free_inode_block_write(&free_inode_block);
  }
  dbg_e("initalized inode free list\n");
}

int main(int argc, char **argv) {
  debug_on[NONE] = 1;

  if (argc != 2) {
    printf("ERROR: Invalid number of arguments\n");
    usage(argv[0]);
    return -1;
  }

  unsigned long size = atoi(argv[1]);

  benchmark_start(BENCHMARK_FILE);
  benchmark_set_disk_size_in_blocks(size);

  if (size >= INVALID) {
    printf("ERROR: diskSizeInBlocksSize >= %u\n", INVALID);
    usage(argv[0]);
    return -1;
  }

  printf("Formatting the %s with size %lu \n", DISKFILE, size);

  create_disk_file(DISKFILE);

  superblock_initialize(size);

  inode_free_list_initialize();
  data_block_free_list_initialize();
  
  superblock.root_inode = directory_create_inode();
  superblock_write();
  
  close_disk_file();
  
  benchmark_end();

  return 0;
}
