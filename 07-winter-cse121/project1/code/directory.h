/*
 * Author: Nitay Joffe
 * Class: CSE 121 - Modern Operating Systems
 * Project: 1 - Filesystem
 */
#ifndef DIRECTORY_H_
#define DIRECTORY_H_

#include "inode.h"
#include "path.h"

#define DIRECTORY_FILE_PATH_NUMBER_OF_NULL_PADDING_BYTES (4)

#define DIRECTORY_FILE_PATH_SIZE_IN_MEMORY \
  (PATH_MAX_LENGTH + DIRECTORY_FILE_PATH_NUMBER_OF_NULL_PADDING_BYTES)

#define DIRECTORY_ENTRY_SIZE_ON_DISK \
  (sizeof(struct DirectoryEntry) \
   - DIRECTORY_FILE_PATH_NUMBER_OF_NULL_PADDING_BYTES)

#define DIRECTORY_ENTRY_COUNT_PER_BLOCK \
  (BLOCKSIZE / DIRECTORY_ENTRY_SIZE_ON_DISK)

// the order of the items in this struct matter, in particular the file_path
// should be last so that memcpy can read/write only the first 28 bytes of it
struct DirectoryEntry {
  unsigned int inode_number;

  // only MAX_FILE_PATH_LENGTH bytes are written to disk, end is for null bytes
  // note that we add 4 extra bytes, not one, for padding purposes
  char file_path[DIRECTORY_FILE_PATH_SIZE_IN_MEMORY];
};

struct DirectoryBlock {
  struct DirectoryEntry entries[DIRECTORY_ENTRY_COUNT_PER_BLOCK];

  unsigned int block_number; 
};

struct Directory {
  struct Inode inode;
};

void directory_read_inode(unsigned int inode_number,
                          struct Directory *directory);

unsigned int directory_create_inode();

void directory_add_entry(const char *name, unsigned int inode,
                         struct Directory *directory);

unsigned int directory_delete_item(const char *name,
                                   struct Directory *directory);

unsigned int directory_find_item(const struct Directory *directory,
                                 const char *name);

void directory_block_read(unsigned int block_number,
                          struct DirectoryBlock *directory_block);

#endif
