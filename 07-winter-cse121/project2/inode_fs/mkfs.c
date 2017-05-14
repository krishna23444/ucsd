/*
 * CSE121 Project 1: File System
 *
 * Copyright(c) 2005 Alex C. Snoeren <snoeren@cs.ucsd.edu>
 * Calvin Hubble
 * Kiran Tati <ktati@cs.ucsd.edu>
 *
 */

#include <math.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "../benchmark/benchmark.h"

#include "Inode.h"

/* Sets up the directory structure like the following :
|Superblock| inodeFreeList | dataFreeList | inodes | firstDirBlock|       rest of data |

Superblock has the following form:
0           4          8        12     
|# of blocks|inodefreelist_start|inodefreelist_end|memfreelist_start|mem_freelistend|inodeStart|inodeEnd|1stDir|
*/

int
myformat(const char *filename, int size)
{
  //fprintf(stderr, "NEED TO IMPLEMENT.\n");
  //return -1;
  int i;
  int FD;
  int numBlocks = size; //size is in terms of blocks
  char superBlock[512];
 
  //How many blocks are part of the inode
  int numInodeBlocks = (int) ceil( (double)numBlocks * (double) .05 );
  
  //Inode size is 64, (512/64) = 8 inodes per block
  int numInodes = numInodeBlocks * 8; 

  //int inodeFreeListSize = ((numInodeBlocks * 2) / 512) + 1;  
  
  //8 inode free bitscan fit into 1 byte, with 512 bytes per block
  //inodeFreeListSize = # inodes / (8*512)
  int inodeFreeListSize = (int) ceil ( (double)numInodes / (double)(512*8) );

  //8 data free bits can fit into 1 byte, 512 bits fit into a block
  //8*512 free list bits can fit into a block
  int dataFreeListSize = (int) ceil ( (double) numBlocks  /  (double) (512*8) );
  
  
  int inodeFreeListStart = 1;
  int inodeFreeListEnd = inodeFreeListStart+inodeFreeListSize - 1;
  int dataFreeListStart = inodeFreeListEnd + 1;
  int dataFreeListEnd = dataFreeListStart + dataFreeListSize - 1;
  
  char * dataFreeList;
  char * inodeFreeList;
  
  int inodeStartBlock = dataFreeListEnd+1;
  int inodeEndBlock = inodeStartBlock+numInodeBlocks-1;

  
  int firstDirectoryBlock = inodeEndBlock+1;
  
  Directory_Block base_dir;
  char base_dir_data[512];

  int blockCnt;
  FD = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0777);  
  if(FD == -1)
    fprintf(stderr, "mymount: Could not open file image %s\n", filename);

  /** Write the super block **/
  if(numInodeBlocks == 0)
    numInodeBlocks = 1;

  memset(superBlock, 0, 512);
  
  *(int*)(superBlock) = numBlocks;
  *(int*)(superBlock+4) = inodeFreeListStart;
  *(int*)(superBlock+8) = inodeFreeListEnd;   
  *(int*)(superBlock+12) = dataFreeListStart;
  *(int*)(superBlock+16) = dataFreeListEnd;
  *(int*)(superBlock+20) = inodeStartBlock;
  *(int*)(superBlock+24) = inodeEndBlock;
  *(int*)(superBlock+28) = firstDirectoryBlock;

 
  
  dwrite(FD, 0, superBlock);


  /** Write inode free list **/
  //Just write all inodes as unallocated
  inodeFreeList = (char *)malloc(512);
    
  for(i=0; i<512; i++)
  {
    inodeFreeList[i] = 0xff;
  }
  
  for(i = inodeFreeListStart; i<= inodeFreeListEnd; i++)
  {
    dwrite(FD, i, inodeFreeList);
  }
  
  
  
  /** Write data free list **/
  
  //1 bit per each block ... 8 bits in a byte

  dataFreeList = (char *)malloc( dataFreeListSize*512 );
  memset(dataFreeList, 0, dataFreeListSize*512 );

  for(i=0; i<numBlocks/8; i++)
  {
    dataFreeList[i] = 0xff;
  }

  //Mark block 0->first Dir Block Num (inclusive)
  //as MARKED, then write dataFreeList
  for(i=0; i<=firstDirectoryBlock; i++)
  {
    int byteNum;
    int offset;
    unsigned char reserved_byte = 1;
    //Mark i'th data block as reserved
    byteNum = i / 8;
    offset =  i % 8;
    
    reserved_byte = reserved_byte<<offset;
    reserved_byte = reserved_byte ^ 0xff; //flip bits, so all 1s except for a 0
    dataFreeList[byteNum] = dataFreeList[byteNum] & reserved_byte;

  }
  
  blockCnt = 0;
  for(i=dataFreeListStart; i<=dataFreeListEnd; i++)
  {
    dwrite(FD, i, dataFreeList+(blockCnt*512));
    blockCnt++;
  }
  
  
  /** Write "/" directory entry **/
  memset(base_dir_data, 0, 512);
  Set_Empty_Dir_Block(&base_dir);
  Set_Data_Directory_Block(base_dir_data, &base_dir);
  dwrite(FD, firstDirectoryBlock, base_dir_data);


  free(inodeFreeList);
  free(dataFreeList);

  int  blockNum;
  char block[512];
  memset(block, 0, 512);
  for(blockNum = firstDirectoryBlock + 1; blockNum < size; blockNum++) {
    dwrite(FD, blockNum, block);
  }

 // we need to write all inode to zero

  close(FD);



}

int usage() {
    printf("usage: mkfs diskSizeInBlockSize\n");
}

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("Invalid number of arguments \n");
    usage();
    return 1;
  }

  unsigned long size = atoi(argv[1]);

  benchmark_start(BENCHMARK_FILE);
  benchmark_set_disk_size_in_blocks(size);

  printf("Formatting the %s with size %lu \n", DISKFILE, size);
  myformat(DISKFILE, size);
  
  benchmark_end();
}

/* Read a block from disk */
int dread(int fd, int blocknum, char *buf) {
  benchmark_read(blocknum);
  
  lseek(fd, blocknum*BLOCKSIZE, SEEK_SET);
  if (read(fd, buf, BLOCKSIZE) != BLOCKSIZE)
    perror("dread");
  return BLOCKSIZE;
}

/* Write a block to disk */
int dwrite(int fd, int blocknum, const char *buf)
{
  benchmark_write(blocknum);
  
  int i;
  lseek(fd, blocknum*BLOCKSIZE, SEEK_SET);
  if (write(fd, buf, BLOCKSIZE) != BLOCKSIZE)
    perror("dwrite");

  return BLOCKSIZE;
}


void Set_Empty_Dir_Block(Directory_Block * dir_block)
{
  int i;
  for(i=0; i<14; i++)
  {
    dir_block->entries[i].first_data_block = -1;
    dir_block->entries[i].isDirectory = -1;
  }
  dir_block->next_directory_block = -1;
}

void Set_Data_Directory_Block(char * data_block, const Directory_Block * dir_block)
{
  int i;
  
  for(i=0; i<14; i++)
  {
    memcpy(data_block+(i*36), dir_block->entries[i].file_name, 28);
    *(int*)(data_block+(i*36)+28) = dir_block->entries[i].isDirectory;
    *(int*)(data_block+(i*36)+32) = dir_block->entries[i].first_data_block;
  }
  *(int*)(data_block+504) = dir_block->next_directory_block;
}
