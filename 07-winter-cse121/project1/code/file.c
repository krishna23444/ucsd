/*
 * Author: Nitay Joffe
 * Class: CSE 121 - Modern Operating Systems
 * Project: 1 - Filesystem
 */
#include <string.h>
#include <strings.h>
#include "debug.h"
#include "file.h"
#include "inode.h"
#include "io.h"
#include "superblock.h"

unsigned int file_create_inode() {
  dbg_s("creating file inode\n");
  struct Inode inode;
  bzero(&inode, sizeof(struct Inode));
  inode.number = allocate_inode();
  inode.on_disk.points_to_directory = 0;
  inode_write(&inode);

  dbg_e("created file inode %u\n", inode.number);
  return inode.number;
}

void file_read_inode(unsigned int inode_number, struct File *file) {
  inode_read(inode_number, &file->inode);
}

unsigned int file_read(const struct File *file, size_t size, off_t offset,
                       char *buffer) {
  if (size <= 0)
    return 0;

  if (offset >= file->inode.on_disk.size_in_bytes)
    return 0;

  if (size + offset > file->inode.on_disk.size_in_bytes)
    size = file->inode.on_disk.size_in_bytes - offset;

  unsigned int number_of_bytes_to_read = size;

  unsigned int offset_in_block = offset % BLOCKSIZE;
  unsigned int number_of_bytes_till_next_block = BLOCKSIZE - offset_in_block;

  if (number_of_bytes_to_read > number_of_bytes_till_next_block)
    number_of_bytes_to_read = number_of_bytes_till_next_block;

  unsigned int number_of_bytes_read = 0;  

  while (number_of_bytes_read < size) {
    unsigned char block_buffer[BLOCKSIZE] = "";
    
    unsigned int block_number = offset / BLOCKSIZE;
    unsigned int block_address = inode_lookup_block(&file->inode, block_number);
    
    kread(block_address, block_buffer);
    
    offset_in_block = offset % BLOCKSIZE;
    
    memcpy(buffer, block_buffer + offset_in_block, number_of_bytes_to_read);
    
    number_of_bytes_read += number_of_bytes_to_read;
    offset += number_of_bytes_to_read;
    buffer += number_of_bytes_to_read;
    
    number_of_bytes_to_read = size - number_of_bytes_read;
    if (number_of_bytes_to_read > BLOCKSIZE)
      number_of_bytes_to_read = BLOCKSIZE;
  }

  return number_of_bytes_read;
}

unsigned int file_write(size_t size, off_t offset, const char *buffer,
                        struct File *file) {
  dbg_s("writing %u bytes at %u to inode %u\n", size, offset, file->inode.number);

  if (size <= 0)
    return 0;

  int number_of_bytes_to_allocate = offset + size - file->inode.on_disk.size_in_bytes;

  if (number_of_bytes_to_allocate > 0) {
    dbg("allocating %u bytes\n", number_of_bytes_to_allocate);
    if (inode_increase_size(number_of_bytes_to_allocate, &file->inode) == INVALID) {
      dbg_e("file reached maximum size\n");
      return 0;
    }
  }
 
  unsigned int number_of_bytes_to_write = size;

  unsigned int offset_in_block = offset % BLOCKSIZE;
  unsigned int number_of_bytes_till_next_block = BLOCKSIZE - offset_in_block;

  if (number_of_bytes_to_write > number_of_bytes_till_next_block)
    number_of_bytes_to_write = number_of_bytes_till_next_block;

  unsigned int number_of_bytes_written = 0;  

  while (number_of_bytes_written < size) {
    unsigned char block_buffer[BLOCKSIZE] = "";
    
    unsigned int block_number = offset / BLOCKSIZE;
    unsigned int block_address = inode_lookup_block(&file->inode, block_number);
    
    if (number_of_bytes_to_write < BLOCKSIZE) {
      dbg("partial block, reading first\n");
      kread(block_address, block_buffer);
    }

    offset_in_block = offset % BLOCKSIZE;
    
    dbg("writing %u bytes to block number %u, address %u, offset %u\n",
        number_of_bytes_to_write, block_number, block_address, offset_in_block);
    memcpy(block_buffer + offset_in_block, buffer, number_of_bytes_to_write);
    
    kwrite(block_address, block_buffer);

    number_of_bytes_written += number_of_bytes_to_write;
    offset += number_of_bytes_to_write;
    buffer += number_of_bytes_to_write;
    
    number_of_bytes_to_write = size - number_of_bytes_written;
    if (number_of_bytes_to_write > BLOCKSIZE)
      number_of_bytes_to_write = BLOCKSIZE;
  }

  dbg_e("wrote %u bytes at %u to inode %u", number_of_bytes_written, offset,
        file->inode.number);
  return number_of_bytes_written;
}
