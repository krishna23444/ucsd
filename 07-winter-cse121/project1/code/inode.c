/*
 * Author: Nitay Joffe
 * Class: CSE 121 - Modern Operating Systems
 * Project: 1 - Filesystem
 */
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include "debug.h"
#include "directory.h"
#include "inode.h"
#include "io.h"
#include "superblock.h"

enum InodePointerLevel { INODE_POINTER_DIRECT, INODE_POINTER_SINGLE,
                         INODE_POINTER_DOUBLE, INODE_POINTER_TRIPLE };

struct BlockNumberDetail {
  enum InodePointerLevel type;
  unsigned int offset_in_single;
  unsigned int offset_in_double;
  unsigned int offset_in_triple;
};

static void inode_copy(const struct Inode *from_inode, struct Inode *to_inode) {
  int i;
  for (i = 0; i < INODE_NUMBER_OF_DIRECT_POINTERS; ++i)
    to_inode->on_disk.direct[i] = from_inode->on_disk.direct[i];
  to_inode->on_disk.single_indirect = from_inode->on_disk.single_indirect;
  to_inode->on_disk.double_indirect = from_inode->on_disk.double_indirect;
  to_inode->on_disk.triple_indirect = from_inode->on_disk.triple_indirect;

  to_inode->number = from_inode->number;
  to_inode->on_disk.points_to_directory=from_inode->on_disk.points_to_directory;
  to_inode->on_disk.size_in_bytes = from_inode->on_disk.size_in_bytes;
}

static const char *inode_type_string(const struct Inode *inode) {
  return inode->on_disk.points_to_directory ? "directory" : "file";
}

static void inode_dump(const struct Inode *inode) {
  dbg("%s\n", inode_type_string(inode));
  dbg("size_in_bytes: %u\n", inode->on_disk.size_in_bytes);
  dbg("number: %u\n", inode->number);
  dbg("direct: %2u, %2u, %2u, %2u, %2u, %2u\n", inode->on_disk.direct[0],
      inode->on_disk.direct[1], inode->on_disk.direct[2],
      inode->on_disk.direct[3], inode->on_disk.direct[4],
      inode->on_disk.direct[5]);
  dbg("        %2u, %2u, %2u, %2u, %2u, %2u\n", inode->on_disk.direct[6],
      inode->on_disk.direct[7], inode->on_disk.direct[8],
      inode->on_disk.direct[9], inode->on_disk.direct[10],
      inode->on_disk.direct[11]);
  dbg("single: %u\n", inode->on_disk.single_indirect);
  dbg("double: %u\n", inode->on_disk.double_indirect);
  dbg("triple: %u\n", inode->on_disk.triple_indirect);
}

void free_inode_block_dump(const struct FreeInodeBlock *free_inode_block) {
  dbg("block_number: %u\n", free_inode_block->block_number);
  dbg("%u -> %u\n", free_inode_block->inodes[0].number,
      free_inode_block->inodes[0].on_disk.next_free_inode_number);
  dbg("%u -> %u\n", free_inode_block->inodes[1].number,
      free_inode_block->inodes[1].on_disk.next_free_inode_number);
  dbg("%u -> %u\n", free_inode_block->inodes[2].number,
      free_inode_block->inodes[2].on_disk.next_free_inode_number);
  dbg("%u -> %u\n", free_inode_block->inodes[3].number,
      free_inode_block->inodes[3].on_disk.next_free_inode_number);
  dbg("%u -> %u\n", free_inode_block->inodes[4].number,
      free_inode_block->inodes[4].on_disk.next_free_inode_number);
  dbg("%u -> %u\n", free_inode_block->inodes[5].number,
      free_inode_block->inodes[5].on_disk.next_free_inode_number);
  dbg("%u -> %u\n", free_inode_block->inodes[6].number,
      free_inode_block->inodes[6].on_disk.next_free_inode_number);
  dbg("%u -> %u\n", free_inode_block->inodes[7].number,
      free_inode_block->inodes[7].on_disk.next_free_inode_number);
}

static void free_inode_dump(const struct FreeInode *free_inode) {
  dbg("inode_number: %u\n", free_inode->number);
  dbg("next_free_inode: %u\n", free_inode->on_disk.next_free_inode_number);
}

static void inode_block_read(unsigned int inode_block_number,
                             struct InodeBlock *inode_block) {
  bzero(inode_block, sizeof(struct InodeBlock));
  
  inode_block->block_number = inode_block_number + FIRST_INODE_BLOCK_NUMBER;

  unsigned char buffer[BLOCKSIZE] = "";
  kread(inode_block->block_number, buffer);

  unsigned char *buffer_pointer = buffer;
  int i;
  for (i = 0; i < INODE_COUNT_PER_BLOCK; ++i) {
    memcpy(&inode_block->inodes[i].on_disk, buffer_pointer,
           sizeof(struct InodeOnDisk));
    buffer_pointer += sizeof(struct InodeOnDisk);
  }
}

static void inode_block_write(const struct InodeBlock *inode_block) {
  unsigned char buffer[BLOCKSIZE] = "";
  unsigned char *buffer_pointer = buffer;
  int i;
  for (i = 0; i < INODE_COUNT_PER_BLOCK; ++i) {
    memcpy(buffer_pointer, &inode_block->inodes[i].on_disk,
           sizeof(struct InodeOnDisk));
    buffer_pointer += sizeof(struct InodeOnDisk);
  }
  
  kwrite(inode_block->block_number, buffer);
}

void inode_read(unsigned int inode_number, struct Inode *inode) {
  dbg_s("reading inode number %u\n", inode_number);

  bzero(inode, sizeof(struct Inode));

  int inode_block_number = inode_number / INODE_COUNT_PER_BLOCK;
  int inode_offset_in_block = inode_number % INODE_COUNT_PER_BLOCK;

  struct InodeBlock inode_block;
  inode_block_read(inode_block_number, &inode_block);

  inode_copy(&inode_block.inodes[inode_offset_in_block], inode);
  inode->number = inode_number;

  inode_dump(inode);
  dbg_e("read inode number %u\n", inode_number);
}

void inode_write(const struct Inode *inode) {
  dbg_s("writing inode number %u\n", inode->number);
  inode_dump(inode);

  if (inode->number == INVALID) {
    dbg_e("ERROR Inode number invalid, not writing to disk\n");
    return;
  }

  unsigned int inode_block_number = inode->number / INODE_COUNT_PER_BLOCK;
  unsigned int inode_offset_in_block = inode->number % INODE_COUNT_PER_BLOCK;

  struct InodeBlock inode_block;
  inode_block_read(inode_block_number, &inode_block);

  inode_copy(inode, &inode_block.inodes[inode_offset_in_block]);

  inode_block_write(&inode_block);

  dbg_e("wrote inode number %u\n", inode->number);
}

unsigned int inode_size_in_blocks(const struct Inode *inode) {
  int file_size_in_blocks = inode->on_disk.size_in_bytes / BLOCKSIZE;

  if (inode->on_disk.size_in_bytes % BLOCKSIZE > 0)
    file_size_in_blocks++;

  return file_size_in_blocks;
}

unsigned int inode_points_to_file(const struct Inode *inode) {
  return !inode_points_to_directory(inode);
}

unsigned int inode_points_to_directory(const struct Inode *inode) {
  return inode->on_disk.points_to_directory;
}

unsigned int inode_bytes_in_last_block(const struct Inode *inode) {
  unsigned int number_of_bytes = inode->on_disk.size_in_bytes % BLOCKSIZE;
  if (number_of_bytes == 0)
    return BLOCKSIZE;
  return number_of_bytes;
}

unsigned int inode_bytes_left_in_last_block(const struct Inode *inode) {
  return BLOCKSIZE - inode_bytes_in_last_block(inode);
}

static unsigned int inode_lookup_single(unsigned int single_indirect_block_number,
                                         unsigned int relative_target_block_number) {
//  dbg_s(" \n");

  unsigned char buffer[BLOCKSIZE] = "";
  
  kread(single_indirect_block_number, buffer);

  unsigned int offset_in_direct_segment
    = relative_target_block_number / INODE_SIZEOF_DIRECT_IN_BLOCKS;

  unsigned int block_address = INVALID;

  memcpy(&block_address, buffer + offset_in_direct_segment
         * INODE_SIZEOF_BLOCK_POINTER, INODE_SIZEOF_BLOCK_POINTER);
  
//  dbg_e("-> %u\n", block_address);

  return block_address;
}

static unsigned int inode_lookup_double(unsigned int double_indirect_block_number,
                                         unsigned int relative_target_block_number) {
//  dbg_s(" \n");
  
  unsigned char buffer[BLOCKSIZE] = "";
  
  kread(double_indirect_block_number, buffer);

  unsigned int offset_in_single_indirect_segment
    = relative_target_block_number / INODE_SIZEOF_SINGLE_INDIRECT_IN_BLOCKS;

  unsigned int single_indirect_block_address = INVALID;

  memcpy(&single_indirect_block_address,
         buffer + offset_in_single_indirect_segment
         * INODE_SIZEOF_BLOCK_POINTER, INODE_SIZEOF_BLOCK_POINTER);
  
  unsigned int new_target_relative_block_number
    = relative_target_block_number % INODE_SIZEOF_SINGLE_INDIRECT_IN_BLOCKS;
  
//  dbg_e("-> %u\n", single_indirect_block_address);

  unsigned int block_address
    = inode_lookup_single(single_indirect_block_address,
                          new_target_relative_block_number);

  return block_address;
}

static unsigned int inode_lookup_triple(unsigned int triple_indirect_block_number,
                                         unsigned int relative_target_block_number) {
//  dbg_s(" \n");
  
  unsigned char buffer[BLOCKSIZE] = "";

  kread(triple_indirect_block_number, buffer);

  unsigned int offset_in_double_indirect_segment
    = relative_target_block_number / INODE_SIZEOF_DOUBLE_INDIRECT_IN_BLOCKS;

  unsigned int double_indirect_block_address = INVALID;
  
  memcpy(&double_indirect_block_address,
         buffer + offset_in_double_indirect_segment
         * INODE_SIZEOF_BLOCK_POINTER, INODE_SIZEOF_BLOCK_POINTER);
  
  unsigned int new_target_relative_block_number
    = relative_target_block_number % INODE_SIZEOF_DOUBLE_INDIRECT_IN_BLOCKS;

//  dbg_e("    -> %u\n", double_indirect_block_address);
  
  unsigned int block_address
    = inode_lookup_double(double_indirect_block_address,
                          new_target_relative_block_number);

  return block_address;
}

void inode_fill_stat(const struct Inode *inode, struct stat *stbuf) {
  dbg_s("filling stat for inode %u\n", inode->number);

  if (inode_points_to_directory(inode))
    stbuf->st_mode = 0777 | S_IFDIR;
  else
    stbuf->st_mode = 0777 | S_IFREG;

  stbuf->st_nlink = 3;
  stbuf->st_rdev = 0;

  stbuf->st_uid = getuid();

  stbuf->st_gid = getgid();


  stbuf->st_ino = inode->number;

  stbuf->st_atime = time(NULL);
  stbuf->st_mtime = time(NULL);
  stbuf->st_ctime = time(NULL);

  stbuf->st_size = inode->on_disk.size_in_bytes;

  stbuf->st_blocks = inode_size_in_blocks(inode);
  
  stbuf->st_blksize = BLOCKSIZE;

  dbg("%s\n", inode_type_string(inode));
  dbg("inode_number: %u\n", stbuf->st_ino);
  dbg("uid: %u\n", stbuf->st_uid);
  dbg("gid: %u\n", stbuf->st_gid);
  dbg("size_in_bytes: %u\n", inode->on_disk.size_in_bytes);
  dbg("size_in_blocks: %u\n", stbuf->st_blocks);

  dbg_e(" \n");
}

unsigned int inode_last_block_number(const struct Inode *inode) {
  return inode_size_in_blocks(inode) - 1;
}

static void inode_fill_block_details(unsigned int block_number,
                                      struct BlockNumberDetail *details) {
  if (block_number < INODE_DIRECT_SEGMENT_END)
    details->type = INODE_POINTER_DIRECT;

  else if (block_number < INODE_SINGLE_INDIRECT_SEGMENT_END) {
    details->type = INODE_POINTER_SINGLE;

    details->offset_in_single = block_number - INODE_DIRECT_SEGMENT_END;    
  }

  else if (block_number < INODE_DOUBLE_INDIRECT_SEGMENT_END) {
    details->type = INODE_POINTER_DOUBLE;

    unsigned int relative_number = block_number - INODE_SINGLE_INDIRECT_SEGMENT_END;

    details->offset_in_double = relative_number / INODE_SIZEOF_SINGLE_INDIRECT_IN_BLOCKS;

    details->offset_in_single = relative_number % INODE_SIZEOF_SINGLE_INDIRECT_IN_BLOCKS;
  }

  else if (block_number < INODE_TRIPLE_INDIRECT_SEGMENT_END) {
    details->type = INODE_POINTER_TRIPLE;
    
    unsigned int triple_relative = block_number - INODE_DOUBLE_INDIRECT_SEGMENT_END;
    
    details->offset_in_triple = triple_relative / INODE_SIZEOF_DOUBLE_INDIRECT_IN_BLOCKS;
    
    unsigned int double_relative = triple_relative % INODE_SIZEOF_DOUBLE_INDIRECT_IN_BLOCKS;
    
    details->offset_in_double = double_relative / INODE_SIZEOF_SINGLE_INDIRECT_IN_BLOCKS;
    
    details->offset_in_single = double_relative % INODE_SIZEOF_SINGLE_INDIRECT_IN_BLOCKS;
  }
  else
    dbg("ERROR block number %u out of range\n", block_number);
}

static void inode_allocate_block(struct Inode *inode) {
  unsigned int block_number_to_add = inode_size_in_blocks(inode);

  struct BlockNumberDetail block_details;
  inode_fill_block_details(block_number_to_add, &block_details);

  unsigned char buffer[BLOCKSIZE] = "";
  unsigned int block_address;
  unsigned int new_block_address;
  unsigned int offset_in_buffer;

  switch (block_details.type) {
    case INODE_POINTER_DIRECT:
      inode->on_disk.direct[block_number_to_add] = allocate_data_block();
      break;

    case INODE_POINTER_SINGLE:
      if (block_details.offset_in_single == 0)
        inode->on_disk.single_indirect = allocate_data_block();
      
      kread(inode->on_disk.single_indirect, buffer);
      
      new_block_address = allocate_data_block();
      offset_in_buffer = block_details.offset_in_single * sizeof(block_address);
      
      memcpy(buffer + offset_in_buffer, &new_block_address, sizeof(block_address));
      kwrite(inode->on_disk.single_indirect, buffer);
      break;

    case INODE_POINTER_DOUBLE:
      if (block_details.offset_in_double == 0
          && block_details.offset_in_single == 0)
        inode->on_disk.double_indirect = allocate_data_block();

      if (block_details.offset_in_single == 0) {
        kread(inode->on_disk.double_indirect, buffer);
        
        new_block_address = allocate_data_block();
        offset_in_buffer = block_details.offset_in_double * sizeof(block_address);
        
        memcpy(buffer + offset_in_buffer, &new_block_address, sizeof(block_address));
        
        kwrite(inode->on_disk.double_indirect, buffer);
      }
      
      kread(inode->on_disk.double_indirect, buffer);
      
      offset_in_buffer = block_details.offset_in_double * sizeof(block_address);
      memcpy(&block_address, buffer + offset_in_buffer, sizeof(block_address));
      
      kread(block_address, buffer);
      
      new_block_address = allocate_data_block();
      offset_in_buffer = block_details.offset_in_single * sizeof(block_address);
      memcpy(buffer + offset_in_buffer, &new_block_address, sizeof(block_address));
      
      kwrite(block_address, buffer);
      break;

    case INODE_POINTER_TRIPLE:
      if (block_details.offset_in_triple == 0
          && block_details.offset_in_double == 0
          && block_details.offset_in_single == 0)
        inode->on_disk.triple_indirect = allocate_data_block();

      if (block_details.offset_in_double == 0
          && block_details.offset_in_single == 0) {
        kread(inode->on_disk.triple_indirect, buffer);
        
        new_block_address = allocate_data_block();
        offset_in_buffer = block_details.offset_in_triple * sizeof(block_address);
        memcpy(buffer + offset_in_buffer, &new_block_address, sizeof(block_address));
        
        kwrite(block_address, buffer);
      }

      if (block_details.offset_in_single == 0) {
        kread(inode->on_disk.triple_indirect, buffer);

        offset_in_buffer = block_details.offset_in_triple * sizeof(block_address);
        memcpy(&block_address, buffer + offset_in_buffer, sizeof(block_address));

        kread(block_address, buffer);

        new_block_address = allocate_data_block();
        offset_in_buffer = block_details.offset_in_double * sizeof(block_address);
        memcpy(buffer + offset_in_buffer, &new_block_address, sizeof(block_address));
        
        kwrite(block_address, buffer);
      }

      kread(inode->on_disk.triple_indirect, buffer);
      
      offset_in_buffer = block_details.offset_in_triple * sizeof(block_address);
      memcpy(&block_address, buffer + offset_in_buffer, sizeof(block_address));

      kread(block_address, buffer);
      
      offset_in_buffer = block_details.offset_in_double * sizeof(block_address);
      memcpy(&block_address, buffer + offset_in_buffer, sizeof(block_address));
      
      kread(block_address, buffer);
      
      new_block_address = allocate_data_block();
      offset_in_buffer = block_details.offset_in_single * sizeof(block_address);
      memcpy(buffer + offset_in_buffer, &new_block_address, sizeof(block_address));
      
      kwrite(block_address, buffer);
      break;
  }
}

int inode_increase_size(unsigned int size_in_bytes, struct Inode *inode) {
  dbg_s("adding %u bytes to inode %u\n", size_in_bytes, inode->number);

  if (size_in_bytes <= inode_bytes_left_in_last_block(inode)) {
    dbg("everything fits in last block (%u bytes left)\n",
        inode_bytes_left_in_last_block(inode));
    
    inode->on_disk.size_in_bytes += size_in_bytes;
    inode_write(inode);
    
    dbg_e("");
    return 0;
  }
  
  inode->on_disk.size_in_bytes += inode_bytes_left_in_last_block(inode);
  dbg("added %u bytes to end of last block\n",
      inode_bytes_left_in_last_block(inode));
  size_in_bytes -= inode_bytes_left_in_last_block(inode);
  
  while (size_in_bytes > 0) {
    dbg("need another data block\n");
    
    if (inode_size_in_blocks(inode) == INODE_MAXIMUM_FILE_SIZE_IN_BLOCKS) {
      dbg_e("cannot allocate anymore blocks for inode %u\n", inode->number);
      inode_write(inode);
      return INVALID;
    }

    inode_allocate_block(inode);
    
    unsigned int number_of_bytes_to_add = size_in_bytes;
    if (number_of_bytes_to_add > BLOCKSIZE)
      number_of_bytes_to_add = BLOCKSIZE;

    inode->on_disk.size_in_bytes += number_of_bytes_to_add;
    size_in_bytes -= number_of_bytes_to_add;
  }

  inode_write(inode);
  dbg_e("");
  return 0;
}  

static void inode_free_block(struct Inode *inode) {
  // TODO: fill in code similar to inode_allocate_block
  unsigned int last_block_address = inode_lookup_last_block(inode);
  free_data_block(last_block_address);
}

void inode_decrease_size(unsigned int size_in_bytes, struct Inode *inode) {
  dbg_s("removing %u bytes from %u\n", size_in_bytes, inode->number);

  if (size_in_bytes <= inode_bytes_in_last_block(inode)) {
    dbg_e("everything fits in last block (%u bytes used)\n",
          inode_bytes_in_last_block(inode));
    inode->on_disk.size_in_bytes -= size_in_bytes;
    inode_write(inode);
    return;
  }
  
  inode->on_disk.size_in_bytes -= inode_bytes_in_last_block(inode);
  size_in_bytes -= inode_bytes_in_last_block(inode);

  while (size_in_bytes > 0) {
    dbg("removing last block\n");
    inode_free_block(inode);

    unsigned int number_of_bytes_to_delete = size_in_bytes;
    if (number_of_bytes_to_delete > BLOCKSIZE)
      number_of_bytes_to_delete = BLOCKSIZE;
    
    inode->on_disk.size_in_bytes -= number_of_bytes_to_delete;
    size_in_bytes -= number_of_bytes_to_delete;
  }

  dbg_e("");
  inode_write(inode);
}

unsigned int inode_lookup_block(const struct Inode *inode,
                                unsigned int block_number) {
//  dbg_s("looking up block %u\n", block_number);
//  
//  if (block_number >= inode_size_in_blocks(inode))
//    dbg("warning block %u > size in blocks %u\n", block_number,
//          inode_size_in_blocks(inode));

  unsigned int block_address = INVALID;

  if (block_number < INODE_DIRECT_SEGMENT_END) {
//    dbg("looking up direct block %u\n", block_number);
    block_address = inode->on_disk.direct[block_number];
  }

  else if (block_number < INODE_SINGLE_INDIRECT_SEGMENT_END) {
//    dbg("looking up single indirect block %u\n", block_number);
//    dbg("      -> %u \n", inode->on_disk.single_indirect);
    block_address
      = inode_lookup_single(inode->on_disk.single_indirect,
                            block_number -INODE_DIRECT_SEGMENT_END);
  }

  else if (block_number < INODE_DOUBLE_INDIRECT_SEGMENT_END) {
//    dbg("looking up double indirect block %u\n", block_number);
//    dbg("    -> %u\n", inode->on_disk.double_indirect);
    block_address
      = inode_lookup_double(inode->on_disk.double_indirect,
                            block_number - INODE_SINGLE_INDIRECT_SEGMENT_END);
  }

  else if (block_number < INODE_TRIPLE_INDIRECT_SEGMENT_END) {
//    dbg("looking up triple indirect block %u\n", block_number);
//    dbg("  -> %u\n", inode->on_disk.triple_indirect);
    block_address
      = inode_lookup_triple(inode->on_disk.triple_indirect,
                            block_number - INODE_DOUBLE_INDIRECT_SEGMENT_END);
  }

//  dbg_e("");
  return block_address;
}

unsigned int inode_lookup_last_block(const struct Inode *inode) {
  return inode_lookup_block(inode, inode_last_block_number(inode));
}

void free_inode_block_read(unsigned int block_number,
                           struct FreeInodeBlock *free_inode_block) {
  bzero(free_inode_block, sizeof(struct FreeInodeBlock));
  free_inode_block->block_number = block_number + FIRST_INODE_BLOCK_NUMBER;
  
  dbg_s("reading free block %u\n", free_inode_block->block_number);
  
  unsigned char buffer[BLOCKSIZE] = "";
  kread(free_inode_block->block_number, buffer);

  unsigned char *buffer_pointer = buffer;
  int i;
  for (i = 0; i < INODE_COUNT_PER_BLOCK; ++i) {
    struct FreeInode *free_inode = &free_inode_block->inodes[i];
 
    memcpy(&free_inode->on_disk, buffer_pointer,
           sizeof(struct FreeInodeOnDisk));

    buffer_pointer += sizeof(struct FreeInodeOnDisk);

    free_inode->number = block_number * INODE_COUNT_PER_BLOCK+i;
  }
  
  free_inode_block_dump(free_inode_block);
  dbg_e("read free block %u\n", free_inode_block->block_number);
}

void free_inode_block_write(const struct FreeInodeBlock *free_inode_block) {
  dbg_s("writing free block %u\n", free_inode_block->block_number);
  free_inode_block_dump(free_inode_block);
  
  unsigned char buffer[BLOCKSIZE] = "";
  unsigned char *buffer_pointer = buffer;
  int i;
  for (i = 0; i < INODE_COUNT_PER_BLOCK; ++i) {
    struct FreeInode *free_inode = &free_inode_block->inodes[i];

    memcpy(buffer_pointer, &free_inode->on_disk,
           sizeof(struct FreeInodeOnDisk));

    buffer_pointer += sizeof(struct InodeOnDisk);
  }
  
  kwrite(free_inode_block->block_number, buffer);
  
  dbg_e("wrote free block %u\n", free_inode_block->block_number);
}

void free_inode_read(unsigned int inode_number, struct FreeInode *free_inode) {
//  dbg_s("reading free inode number %u\n", inode_number);

  bzero(free_inode, sizeof(struct FreeInode));

  int inode_block_number = inode_number / INODE_COUNT_PER_BLOCK;
  int inode_offset_in_block = inode_number % INODE_COUNT_PER_BLOCK;

  struct FreeInodeBlock free_inode_block;
  free_inode_block_read(inode_block_number, &free_inode_block);

  free_inode->on_disk.next_free_inode_number
    = free_inode_block.inodes[inode_offset_in_block].on_disk.next_free_inode_number;
  free_inode->number = inode_number;

//  free_inode_dump(free_inode);
//  dbg_e("read free inode number %u -> %u\n", inode_number,
//        free_inode->on_disk.next_free_inode_number);
}

void free_inode_write(const struct FreeInode *free_inode) {
//  dbg_s("writing free inode number %u -> %u\n", free_inode->number,
//        free_inode->on_disk.next_free_inode_number);
//  free_inode_dump(free_inode);

  if (free_inode->number == INVALID) {
//    dbg_e("ERROR Inode number invalid, not writing to disk\n");
    return;
  }

  unsigned int inode_block_number = free_inode->number / INODE_COUNT_PER_BLOCK;
  unsigned int inode_offset_in_block = free_inode->number%INODE_COUNT_PER_BLOCK;

  struct FreeInodeBlock free_inode_block;
  free_inode_block_read(inode_block_number, &free_inode_block);

  free_inode_block.inodes[inode_offset_in_block].on_disk.next_free_inode_number
    = free_inode->on_disk.next_free_inode_number;

  free_inode_block_write(&free_inode_block);

//  dbg_e("wrote free inode number %u -> %u\n", free_inode->number,
//        free_inode->on_disk.next_free_inode_number);
}

void inode_delete(struct Inode *inode) {
  free_inode(inode->number);
  
  unsigned int block_index = 0;
  while (block_index < inode_size_in_blocks(inode)) {
    unsigned int block_address = inode_lookup_block(inode, block_index);
    free_data_block(block_address);
    block_index++;
  }
}
