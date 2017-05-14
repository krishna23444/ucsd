/*
 * Author: Nitay Joffe
 * Class: CSE 121 - Modern Operating Systems
 * Project: 1 - Filesystem
 */
#ifndef SUPERBLOCK_H_
#define SUPERBLOCK_H_

extern const unsigned int FILESYSTEM_ID;

struct Superblock {
  unsigned int disk_size_in_blocks;
  unsigned int first_free_inode;
  unsigned int first_free_data_block;
  unsigned int root_inode;
  unsigned int disk_is_corrupt;
  unsigned int filesystem_id;
} superblock;

void superblock_initialize(unsigned int size_in_blocks);
void superblock_read();
void superblock_write();

unsigned int allocate_data_block();
unsigned int allocate_inode();

void free_data_block(unsigned int block_number);
void free_inode(unsigned int inode_number);

unsigned int number_of_inode_blocks();
unsigned int number_of_inodes();
unsigned int last_inode_block_number();
unsigned int last_inode_number();

unsigned int first_data_block_number();
unsigned int number_of_data_blocks();
unsigned int last_data_block_number();

#endif
