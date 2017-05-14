/*
 * Author: Nitay Joffe
 * Class: CSE 121 - Modern Operating Systems
 * Project: 1 - Filesystem
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "debug.h"
#include "inode.h"
#include "io.h"
#include "superblock.h"

const unsigned int FILESYSTEM_ID = 314159265u;

static const unsigned int SUPERBLOCK_ADDRESS = 0;
static const unsigned int FIRST_INODE_NUMBER = 0;

static void superblock_dump() {
  dbg("size_in_blocks: %u\n", superblock.disk_size_in_blocks);
  dbg("first_free_inode: %u\n", superblock.first_free_inode);
  dbg("first_free_data_block: %u\n", superblock.first_free_data_block);
  dbg("root_inode: %u\n", superblock.root_inode);
  dbg("disk_corrupt: %u\n", superblock.disk_is_corrupt);
  dbg("filesystem_id: %u\n", superblock.filesystem_id);
}

void superblock_initialize(unsigned int size_in_blocks) {
  dbg_s("initializing with size in blocks %u\n", size_in_blocks);

  superblock.disk_size_in_blocks = size_in_blocks;
  superblock.first_free_inode = FIRST_INODE_NUMBER;
  superblock.first_free_data_block = first_data_block_number();
  superblock.root_inode = INVALID;
  superblock.disk_is_corrupt = 1;
  superblock.filesystem_id = FILESYSTEM_ID;

  dbg_e("initialized with size in blocks %u\n", size_in_blocks);
}

void superblock_read() {
  dbg_s("reading\n");
  
  unsigned char buffer[BLOCKSIZE] = "";

  kread(SUPERBLOCK_ADDRESS, buffer);
  memcpy(&superblock, buffer, sizeof(struct Superblock));

  superblock_dump();
  dbg_e("read\n");
}

void superblock_write() {
  dbg_s("writing\n");
  superblock_dump();

  unsigned char buffer[BLOCKSIZE] = ""; 

  memcpy(buffer, &superblock, sizeof(struct Superblock));
  kwrite(SUPERBLOCK_ADDRESS, buffer);

  dbg_e("wrote\n");
}

unsigned int number_of_inode_blocks() {
  // FFS allocates one inode per 2048 bytes = 4 blocks in this filesystem.
  // Since there are 8 inodes per block, this means one inode block per 32
  // data blocks, therefore our divisor for the number of inode blocks is 32.
  return superblock.disk_size_in_blocks / 32;
}

unsigned int number_of_inodes() {
  return number_of_inode_blocks() * INODE_COUNT_PER_BLOCK;
}

unsigned int last_inode_block_number() {
  return FIRST_INODE_BLOCK_NUMBER + number_of_inode_blocks() - 1;
}

unsigned int last_inode_number() {
  return number_of_inodes() - 1;
}

unsigned int first_data_block_number() {
  return FIRST_INODE_BLOCK_NUMBER + number_of_inode_blocks();
}

unsigned int number_of_data_blocks() {
  return superblock.disk_size_in_blocks - number_of_inode_blocks() - 1;
}

unsigned int last_data_block_number() {
  return first_data_block_number() + number_of_data_blocks() - 1;
}

unsigned int allocate_data_block() {
  dbg_s("allocating data block\n");
  if (superblock.first_free_data_block == INVALID) {
    fprintf(stderr, "ERROR no more data blocks available\n");
    close_disk_file();
    exit(-1);
  }

  unsigned int next_free_data_block = superblock.first_free_data_block;

  unsigned char buffer[BLOCKSIZE] = "";
  kread(superblock.first_free_data_block, buffer);
  memcpy(&superblock.first_free_data_block, buffer,
         sizeof(superblock.first_free_data_block));

  superblock_write();

//  bzero(buffer, BLOCKSIZE);
//  kwrite(next_free_data_block, buffer);

  dbg_e("allocated data block %u, next = %u\n", next_free_data_block,
        superblock.first_free_data_block);
  return next_free_data_block;
}

unsigned int allocate_inode() {
  dbg_s("allocating inode\n");
  if (superblock.first_free_inode == INVALID) {
    fprintf(stderr, "ERROR no more inodes available\n");
    close_disk_file();
    exit(-1);
  }

  struct FreeInode free_inode;
  free_inode_read(superblock.first_free_inode, &free_inode);

  superblock.first_free_inode = free_inode.on_disk.next_free_inode_number;
  superblock_write();

  dbg_e("allocated inode %u, next = %u\n", free_inode.number,
        superblock.first_free_inode);
  return free_inode.number;
}

void free_data_block(unsigned int block_number) {
  unsigned char buffer[BLOCKSIZE] = "";
  bzero(buffer, sizeof(buffer)/sizeof(buffer[0]));
  
  memcpy(buffer, &superblock.first_free_data_block,
         sizeof(superblock.first_free_data_block));
  
  kwrite(block_number, buffer);
  
  superblock.first_free_data_block = block_number;
  superblock_write();
}

void free_inode(unsigned int inode_number) {
  struct FreeInode next_free_inode;
  next_free_inode.number = inode_number;
  next_free_inode.on_disk.next_free_inode_number = superblock.first_free_inode;
  free_inode_write(&next_free_inode);
  
  superblock.first_free_inode = inode_number;
  superblock_write();
}
