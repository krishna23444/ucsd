/*
    FUSE: Filesystem in Userspace
    Copyright (C) 2001-2005  Miklos Szeredi <miklos@szeredi.hu>
    Copyright (C) 2007  Alex C. Snoeren <snoeren@cs.ucsd.edu>

    This program can be distributed under the terms of the GNU GPL.
    See the file COPYING.
*/

#ifdef linux
/* For pread()/pwrite() */
#define _XOPEN_SOURCE 500
#endif

#include <fuse.h>
#include <math.h>
#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <assert.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/statfs.h>
#ifdef HAVE_SETXATTR
#include <sys/xattr.h>
#endif

#include "../benchmark/benchmark.h"

#include "Inode.h"

#define DEBUG_ADD_TRAILING_DIRECTORY_SLASH
int imagefd = -1;

//Global functions - set at mymount
int _numBlocks;
int _inodeFreeListStart;
int _inodeFreeListEnd;   
int _dataFreeListStart;
int _dataFreeListEnd;
int _inodeStartBlock;
int _inodeEndBlock;
int _firstDirectoryBlock;
int blockNumsPerBlock;
int blocksPerIndirectBlock;
int blocksPerDoubleIndirectBlock;
int blocksPerTripleIndirectBlock;
int firstIndirectBlock;
int firstDoubleIndirectBlock;
int firstTripleIndirectBlock;
int maxBlocks;


void debug_Print_Free_Lists()
{
  unsigned char block[512];
  int i;
  int block_counter;
  printf("inodeFreeList(block %d to %d:\n", _inodeFreeListStart, _inodeFreeListEnd);
  for(block_counter = _inodeFreeListStart; block_counter <= _inodeFreeListEnd; block_counter++)
  {
    dread(imagefd, block_counter, block);
    for(i=0; i<128; i++)
    {
      printf("%.8x_", *(int*)(block+i*4));
    }
  }
  
  printf("\nDataFreeList:( block %d to %d) \n", _dataFreeListStart, _dataFreeListEnd);
  for(block_counter = _dataFreeListStart; block_counter <= _dataFreeListEnd; block_counter++)
  {
    dread(imagefd, block_counter, block);
    for(i=0; i<128; i++)
    {
      printf("%.8x_", *(int*)(block+i*4));
    }
  }

}

////////////////////////////////////////////////////////////
// Parse_Directory_Entry
//
// Given a block, will parse the directory entry into a buffer
// allocated
////////////////////////////////////////////////////////////
int Parse_Directory_Entry(Directory_Entry * entry, int entry_num, const char * data_block)
{
  assert( sizeof(Directory_Entry) == 36 );  
  //512 / 36
  assert( entry_num <= 14 );
  memcpy(entry, data_block+(entry_num*36), 36);
  return 0;
}


////////////////////////////////////////////////////////////
// Parse_Directory_Block
//
// Given a data block, will fill in a dir_block with the entries
// corresponding to tha directory block
////////////////////////////////////////////////////////////
int Parse_Directory_Block(Directory_Block * dir_block, const char * data_block)
{
  int i;

  for(i=0; i<14; i++)
  {
    Parse_Directory_Entry(&dir_block->entries[i], i, data_block);
  }
  dir_block->next_directory_block = *(int*)(data_block+504);

  return 0;
}


////////////////////////////////////////////////////////////
// is_Dir_Entry_Empty
//
//  returns 1 if directory is empty, 0 if not
////////////////////////////////////////////////////////////
int is_Dir_Entry_Empty(const Directory_Entry * entry)
{
  if(entry->first_data_block == -1)
    return 1;
  else
    return 0;
}

////////////////////////////////////////////////////////////
// is_Dir_Block_Empty
////////////////////////////////////////////////////////////
int is_Dir_Block_Empty(const Directory_Block * block)
{
  int i;

  for(i=0; i<14; i++)
  {
    if( !is_Dir_Entry_Empty(&block->entries[i]) )
      return 0;
  }
  
  return 1;
}
////////////////////////////////////////////////////////////
// Set_Empty_Dir_Block
//
// Fills in the provided memory address with empty
// directory information
////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////
// Set_Data_Direcotry_Block
//
// serializes a Directory_Block data structure into a 
// 512-byte block
////////////////////////////////////////////////////////////
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


////////////////////////////////////////////////////////////
// Set_Empty_Dir_Entry
//
//  Given a pointer to a directory entry, will set that
//  entry as "empty".  In this implementation empty means
//  that the first_data_block pointer is -1.  
//
//  This can never cause a problem (from unsigned integers)
//  since we have at most 2^32 / 512 blocks, we can never have
//  2^32-1 blocks so -1 will always be "free" to use for a reserved
//  value
////////////////////////////////////////////////////////////
void Set_Empty_Dir_Entry(Directory_Entry * dir_entry)
{
  dir_entry->first_data_block = -1;
  dir_entry->isDirectory = -1;
}

////////////////////////////////////////////////////////////
// Is_Directory
//
// returns 1 if the corresponding entry refers to a directory
//  (as opposed to an entry for a file)
// returns 0 otherwise
////////////////////////////////////////////////////////////
int Is_Directory(const Directory_Entry * entry)
{
  if(entry->isDirectory == 1)
    return 1;
  else
    return 0;
}

////////////////////////////////////////////////////////////
// Get_Directory_Entry_And_Block
//
// Given a starting block and a file name, will fill in retrieved_entry
// with the corresponding entry if such an entry existws.
//
// Also, returns the BLOCK that the entry was found in
////////////////////////////////////////////////////////////
int Get_Directory_Entry_And_Block (int dir_start_block, const char * file_name, 
				  Directory_Entry * retrieved_entry)
{
  int done;
  char dir_block_data[512];
  Directory_Block dir_block;
  int curr_block;
  int i;
  done = 0;

  dread(imagefd, dir_start_block, dir_block_data);
  Parse_Directory_Block(&dir_block, dir_block_data);

  curr_block = dir_start_block;
  while(!done)
  {
    if(!is_Dir_Block_Empty(&dir_block))
    {
      for(i=0; i<14; i++)
      {
	if( !is_Dir_Entry_Empty(&dir_block.entries[i]) && (strcmp(dir_block.entries[i].file_name, file_name) == 0) )
	{
	  memcpy(retrieved_entry, &dir_block.entries[i], sizeof(Directory_Entry));
	  done = 1;
	  break;
	}
      }
    }
    if(!done)
    {
      if(dir_block.next_directory_block == -1)
	return -1;
      curr_block = dir_block.next_directory_block;
      dread(imagefd, curr_block, dir_block_data);
      Parse_Directory_Block(&dir_block, dir_block_data);
    }
  }
  
  return curr_block;
}


////////////////////////////////////////////////////////////
// Add_Directory_Entry
//
// Helper function that adds a directory entry.
//
// Note, a directory entry is the tuple: <dir_name, size, firstBlock>
////////////////////////////////////////////////////////////
int Add_Directory_Entry(int dir_block_num, const Directory_Entry * dir_entry_to_add)
{
    int dir_entry_counter;
  Directory_Block dir_block, new_dir_block;
  char dirBlockData[BLOCKSIZE];
  int foundEmptyEntry;
  int previous_dir_block_num;
  int allocated_new_dir_block = 0;

  dread(imagefd,dir_block_num, dirBlockData);  
  Parse_Directory_Block(&dir_block, dirBlockData);  
  foundEmptyEntry = 0;
  
  previous_dir_block_num = dir_block_num;
  while(!foundEmptyEntry)
  {
    for(dir_entry_counter = 0; dir_entry_counter < 14; dir_entry_counter++)
    {
      if( is_Dir_Entry_Empty( &dir_block.entries[dir_entry_counter] ) )
      {
	foundEmptyEntry = 1;
	memcpy( &dir_block.entries[dir_entry_counter], dir_entry_to_add, sizeof(Directory_Entry));
	foundEmptyEntry = 1;
	break;
      }
    }

    //Went through entire directory block and did not find a free entry
    if( (dir_entry_counter == 14) && (!foundEmptyEntry) )
    {
      if(dir_block.next_directory_block == -1)
      {
	previous_dir_block_num = dir_block_num;
	//allocate a new directory block
	dir_block_num = Find_And_Mark_Free_Block();
	if(dir_block_num == -1)
	{
	  //Could not find a free directory block to allocate
	  return -ENOMEM;
	}
	dir_block.next_directory_block = dir_block_num;
	Set_Empty_Dir_Block(&new_dir_block);
	memcpy(&new_dir_block.entries[0], dir_entry_to_add, sizeof(Directory_Entry));
	foundEmptyEntry = 1;
	allocated_new_dir_block = 1;

      }else{
	previous_dir_block_num = dir_block_num;
	//read next directory block and continue
	dir_block_num = dir_block.next_directory_block;
	dread(imagefd, dir_block_num, dirBlockData);
	Parse_Directory_Block(&dir_block, dirBlockData);
      }
    }
  }

  if(!foundEmptyEntry)
    return -1;
  

  //This is the case where we had to allocate a new directory block and create a pointer
  //from the previous directory block
  if( allocated_new_dir_block && (previous_dir_block_num != dir_block_num))
  {
    //Write new directory block
    Set_Data_Directory_Block(dirBlockData, &new_dir_block);    
    dwrite(imagefd, dir_block_num, dirBlockData);

    Set_Data_Directory_Block(dirBlockData, &dir_block);
    dwrite(imagefd, previous_dir_block_num, dirBlockData);

    //write dir block
  }else{
    //simply write directory block
    Set_Data_Directory_Block(dirBlockData, &dir_block);
    dwrite(imagefd, dir_block_num, dirBlockData);
  }
  
  return 0;
}

////////////////////////////////////////////////////////////
// Remove_And_Clean_Dir_Entry
//
// Removes a directory entry described by entry which should appear 
// in the directory which starts at block directory_start_block_num
//
// Handles case where empty directory blocks are freed
////////////////////////////////////////////////////////////
int Remove_And_Clean_Dir_Entry(const Directory_Entry * entry, int directory_start_block_num)
{
  char dir_block_data[512];
  Directory_Block dir_block;
  int prev_dir_block_num;
  int curr_dir_block_num;
  int saved_next_dir_block_num;
  int i;
  int done;

  dread(imagefd, directory_start_block_num, dir_block_data);
  Parse_Directory_Block(&dir_block, dir_block_data);
  curr_dir_block_num = directory_start_block_num;
  prev_dir_block_num = directory_start_block_num;
  done = 0;
  while(!done)
  {
    for(i=0; i<14; i++)
    {
      if( (!is_Dir_Entry_Empty(&dir_block.entries[i])) && (strcmp(dir_block.entries[i].file_name, entry->file_name) == 0) )
      {
	//Found entry we want to delete!
	Set_Empty_Dir_Entry(&dir_block.entries[i]);
	Set_Data_Directory_Block(dir_block_data, &dir_block);
	dwrite(imagefd, curr_dir_block_num, dir_block_data);
	done = 1;
	break;
      }
    }
    if(!done)
    {
      //Might need to continue on to next block
      
      //if there is no block to continue on to & we have not found the entry to delete, 
      //then just return -1
      if(dir_block.next_directory_block == -1)
	return -1;
      prev_dir_block_num = curr_dir_block_num;
      curr_dir_block_num = dir_block.next_directory_block;
      dread(imagefd, curr_dir_block_num, dir_block_data);
      Parse_Directory_Block( &dir_block, dir_block_data);      
    }    
  }
  
  //Check if we have an empty directory block that needs to be freed
  if( (prev_dir_block_num != curr_dir_block_num) && (is_Dir_Block_Empty(&dir_block)) )
  {
    saved_next_dir_block_num = dir_block.next_directory_block;
    dread(imagefd, prev_dir_block_num, dir_block_data);
    Parse_Directory_Block(&dir_block, dir_block_data);
    dir_block.next_directory_block = saved_next_dir_block_num;
    Set_Data_Directory_Block(dir_block_data, &dir_block);
    dwrite(imagefd, prev_dir_block_num, dir_block_data);

    //update free list for freed directory block
    Mark_Block_Free(curr_dir_block_num);

  }
}

////////////////////////////////////////////////////////////
// GetDirPathHelper
////////////////////////////////////////////////////////////
//Given a path, will tokenize the string and return an array of pointers to strings
char ** GetDirPathHelper(const char * path, int *depth)
{ 
  char ** return_array;
  char * saved_path;
  char * next_token;
  char delim[] = "/";
  int i;
  

  /** Count number of '/' **/
  (*depth) = 0;
  saved_path = (char *)malloc(sizeof(path));
  strcpy(saved_path, path);    

  next_token = strtok(saved_path, delim);
  while(1)
  {
    if(next_token == NULL) break;
    (*depth)++;
    next_token = strtok(NULL, delim);
  }
  

  //** now create the string array

  return_array = (char **)malloc(sizeof(char*) * (*depth));

  if(!strcpy(saved_path, path))
  {
    fprintf(stderr, "GetDirPathHelper: error with strcpy - saved_path-path: %s (1)\n", path);
    fflush(stderr);
  }

  i=0;
  next_token = strtok(saved_path, delim);
  while(1)
  {
    if(next_token == NULL) return return_array;
    if(i == (*depth))
    {
      fprintf(stderr, "GetDirPathHelper: Miscaculation!  Probably coding error - next_token = %s\n", next_token);
      fflush(stderr);
      return NULL;
    }
    return_array[i] = (char *)malloc(28);
    if(strcpy(return_array[i], next_token) == NULL)
    {
      fprintf(stderr, "GetDirPathHelper: error with return_array->next_token\n");
      fflush(stderr);

    }

    i++;      
    next_token = strtok(NULL, delim);
  }
       
  free(saved_path);
  return return_array;
}

//Size is the number of entries
void Delete_Char_Array(char ** array, int size)
{
  int i;

  for(i=0; i<size; i++)
  {
    free(array[i]);
  }
  free(array);
}


////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
//  END OF DIRECTORY functions ... start of
//  Free List / Free Inode functions
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

int Find_And_Mark_Free_Block()
{
  //return -1;
  int foundFreeBlock;
  char blockData[512];
  int blockNum = _dataFreeListStart;
  int i;
  char byteEntry;
  int offset = 0;

  int saved_offset, saved_byte, saved_block;

  char full_block = 0x0;

  foundFreeBlock = 0;
  while(!foundFreeBlock)
  {
    if(blockNum > _dataFreeListEnd)
    {
      return -1;
    }
    dread(imagefd, blockNum, blockData);
    fflush(stdout);
    for(i=0; i<512; i++)
    {

      fflush(stdout);
      //See if we have a free entry
      if(blockData[i] != full_block) 
      {
	//there is a free entry in this block
	offset = 0;
	for(offset = 0; offset < 8; offset++)
	{

	  if( (blockData[i]>>offset) & 0x1 )
	  {
	    int return_block_num = 0;

	    foundFreeBlock = 1;
	    saved_offset = offset;
	    saved_byte = i;
	    saved_block = blockNum;
	    

	    blockData[i] = blockData[i] & ((0x1 << offset) ^ 0xff);

	    
	    dwrite(imagefd, blockNum, blockData);
	    
	    //return the block # that was allocated
	    return_block_num = ((blockNum-_dataFreeListStart)*512 + i)*8 + offset;

	    fflush(stdout);
	    return return_block_num;
	  }
	}//for(offset
	assert(0);
      }//if(blockData[i] != 0xff)
    }//for(i=0  
    blockNum++;
  }//while(!foundFreeBlock)  
  return -1;  
}




int Mark_Block_Used(int blockNum)
{
  int dataFreeListBlockNum = (blockNum / (512*8)) + _dataFreeListStart;
  int dataFreeListByteOffset = (blockNum / 8) % 512;
  int dataFreeListBitOffset = blockNum % 8;
  
  char blockData[512];

  dread(imagefd, dataFreeListBlockNum, blockData);
  blockData[dataFreeListByteOffset] = blockData[dataFreeListByteOffset] & ((0x1 << dataFreeListBitOffset) ^ 0xff);
  dwrite(imagefd, dataFreeListBlockNum, blockData);

  return 0;
}

int Mark_Block_Free(int blockNum)
{
  int dataFreeListBlockNum = (blockNum / (512*8)) + _dataFreeListStart;
  int dataFreeListByteOffset = (blockNum / 8) % 512;
  int dataFreeListBitOffset = blockNum % 8;

  char blockData[512];

  dread(imagefd, dataFreeListBlockNum, blockData);
  blockData[dataFreeListByteOffset] = blockData[dataFreeListByteOffset] | (0x1 << dataFreeListBitOffset);
  dwrite(imagefd, dataFreeListBlockNum, blockData);
}


int Find_And_Mark_Free_Inode()
{
  //return -1;
  int foundFreeBlock;
  char blockData[512];
  int blockNum = _inodeFreeListStart;
  int i;
  char byteEntry;
  int offset = 0;

  int saved_offset, saved_byte, saved_block;

  char full_block = 0x0;

  foundFreeBlock = 0;
  while(!foundFreeBlock)
  {
    if(blockNum > _inodeFreeListEnd)
    {

      return -ENOMEM;
    }
    dread(imagefd, blockNum, blockData);

    fflush(stdout);
    for(i=0; i<512; i++)
    {

      fflush(stdout);
      //See if we have a free entry
      if(blockData[i] != full_block) 
      {
	//there is a free entry in this block
	offset = 0;
	for(offset = 0; offset < 8; offset++)
	{

	  if( (blockData[i]>>offset) & 0x1 )
	  {
	    int return_block_num = 0;

	    foundFreeBlock = 1;
	    saved_offset = offset;
	    saved_byte = i;
	    saved_block = blockNum;
	    

	    blockData[i] = blockData[i] & ((0x1 << offset) ^ 0xff);

	    
	    dwrite(imagefd, blockNum, blockData);
	    

	    //return the block # that was allocated
	    return_block_num = (blockNum-_inodeFreeListStart)*512 + i*8 + offset;

	    fflush(stdout);
	    return return_block_num;
	  }
	}//for(offset
	assert(0);
      }//if(blockData[i] != 0xff)
    }//for(i=0  
    blockNum++;
  }//while(!foundFreeBlock)  
  return -1;  
}


////////////////////////////////////////////////////////////
// Start of indoe functions
////////////////////////////////////////////////////////////
//int Find_And_Mark_Free_Inode();
//int Mark_Inode_Used(int inodenum);
//int Is_Inode_Free(int inodenum);
//int Parse_Inode_From_Block(INode * node, int inode_num_offset, const char * dataBlock)



int Mark_Inode_Free(int inodenum)
{
  int dataFreeListBlockNum = (inodenum / (512*8)) + _inodeFreeListStart;
  int dataFreeListByteOffset = (inodenum / 8) % 512;
  int dataFreeListBitOffset = inodenum % 8;
  
  char blockData[512];

  dread(imagefd, dataFreeListBlockNum, blockData);
  blockData[dataFreeListByteOffset] = blockData[dataFreeListByteOffset] | (0x1 << dataFreeListBitOffset);
  dwrite(imagefd, dataFreeListBlockNum, blockData);
}
////////////////////////////////////////////////////////////
// Set_Data_Block_For_Inode
//
// Writes the 64-bytes of data that repesent an inode
////////////////////////////////////////////////////////////
int Set_Data_Block_For_Inode(char * data, const INode * node)
{
  int i;
  int entrynum = 0;

  *(int*)(data+(entrynum*4)) = node->size;
  entrynum++;

  for(i=0; i<7; i++)
  {
    *(int*)(data+(entrynum*4)) = node->direct_blocks[i];
    entrynum++;
  }
  
  for(i=0; i<3; i++)
  {
    *(int*)(data+(entrynum*4)) = node->indirect_blocks[i];
    entrynum++;
  }
  
  *(int*)(data+(entrynum*4)) = node->double_indirect_blocks[0];
  entrynum++;

  for(i=0; i<4; i++)
  {
    *(int*)(data+(entrynum*4)) = node->triple_indirect_blocks[i];
    entrynum++;
  }

  return 0;
}

int Get_All_Data_Blocks_For_Inode(int * data_block_array, int * metaData_array, int * metadata_counter, const INode * inode)
{
  int total_size = inode->size;
  int total_blocks = (int) ceil( (double)total_size / (double) 512 );
  int blocks_read = 0;
  
  int direct_counter = 0;
  int indirect_counter = 0;
  int double_indirect_counter = 0;;
  int triple_indirect_counter = 0;

  char indirectPointer_block[512];
  char doubleIndirectPointer_block[512];
  char tripleIndirectPointer_block[512];
  
  int which_pointer = 0;

  *metadata_counter = 0;
  while(blocks_read < total_blocks)
  {
    if(which_pointer == 0)
    {
      data_block_array[blocks_read] = inode->direct_blocks[direct_counter];
      direct_counter++;
      blocks_read++;
      
      if(blocks_read >= total_blocks)
	return 0;

      if(direct_counter == 7)
      {
	direct_counter = 0;
	which_pointer = 1;
	dread(imagefd, inode->indirect_blocks[indirect_counter], indirectPointer_block);
	metaData_array[(*metadata_counter)++] = inode->indirect_blocks[indirect_counter];
      }         
    
    }else if(which_pointer == 1) {
      data_block_array[blocks_read] = *(int*)(indirectPointer_block+(direct_counter*4));
      direct_counter++;
      blocks_read++;
      
      if( blocks_read >= total_blocks )
	return 0;

      if(direct_counter == 128)
      {
	indirect_counter++;
	direct_counter = 0;
	
	if(indirect_counter < 3)
	{
	  dread(imagefd, inode->indirect_blocks[indirect_counter], indirectPointer_block);
	  metaData_array[(*metadata_counter)++] = inode->indirect_blocks[indirect_counter];
	}else{
	  which_pointer = 2;
	  indirect_counter = 0;
	  double_indirect_counter = 0;
	  
	  dread(imagefd, inode->double_indirect_blocks[double_indirect_counter],doubleIndirectPointer_block);
	  metaData_array[(*metadata_counter)++] = inode->double_indirect_blocks[double_indirect_counter];

	  dread(imagefd, *(int*)(doubleIndirectPointer_block+(indirect_counter*4)), indirectPointer_block);
	  metaData_array[(*metadata_counter)++] = *(int*)(doubleIndirectPointer_block+(indirect_counter*4));
	}
      }
    
    }else if(which_pointer == 2) {
      data_block_array[blocks_read] = *(int*)(indirectPointer_block+(direct_counter*4));
      direct_counter++;
      blocks_read++;

      if( blocks_read >= total_blocks )
	return 0;
      
      if(direct_counter == 128)
      {
	direct_counter = 0;
	indirect_counter++;
	
	if( indirect_counter < 128 )
	{
	  dread(imagefd, *(int*)(doubleIndirectPointer_block+(indirect_counter*4)), indirectPointer_block);
	  metaData_array[(*metadata_counter)++] = *(int*)(doubleIndirectPointer_block+(indirect_counter*4));

	}else{
	  which_pointer = 3;
	  direct_counter = 0;
	  indirect_counter = 0;
	  double_indirect_counter = 0;
	  triple_indirect_counter = 0;

	  dread(imagefd, inode->triple_indirect_blocks[triple_indirect_counter], tripleIndirectPointer_block);
	  metaData_array[(*metadata_counter)++] = inode->triple_indirect_blocks[triple_indirect_counter];

	  dread(imagefd, *(int*)(tripleIndirectPointer_block+(double_indirect_counter*4)), doubleIndirectPointer_block);
	  metaData_array[(*metadata_counter)++] = *(int*)(tripleIndirectPointer_block+(double_indirect_counter*4));

	  dread(imagefd, *(int*)(doubleIndirectPointer_block+(indirect_counter*4)), indirectPointer_block);
	  metaData_array[(*metadata_counter)++] = *(int*)(doubleIndirectPointer_block+(indirect_counter*4));

	}
      }
    }else if(which_pointer == 3) {
      data_block_array[blocks_read] = *(int*)(indirectPointer_block+(direct_counter*4));
      direct_counter++;
      blocks_read++;

      if( blocks_read >= total_blocks )
	return 0;
      
      if(direct_counter == 128)
      {
	direct_counter = 0;
	indirect_counter++;

	if(indirect_counter < 128)
	{
	  dread(imagefd, *(int*)(doubleIndirectPointer_block+(indirect_counter*4)), indirectPointer_block);
	  metaData_array[(*metadata_counter)++] = *(int*)(doubleIndirectPointer_block+(indirect_counter*4));
	}else{
	  indirect_counter = 0;
	  double_indirect_counter++;
	  
	  if( double_indirect_counter < 128 )
	  {
	    dread(imagefd, *(int*)(tripleIndirectPointer_block+(double_indirect_counter*4)), doubleIndirectPointer_block);	    
	    metaData_array[(*metadata_counter)++] = *(int*)(tripleIndirectPointer_block+(double_indirect_counter*4));

	  }else{
	    double_indirect_counter = 0;
	    triple_indirect_counter++;
	    if(triple_indirect_counter >= 4)
	    {
	      //ran out of space
	      assert(0);
	      return -1;
	    }
	    dread(imagefd, inode->triple_indirect_blocks[triple_indirect_counter], tripleIndirectPointer_block);
	    metaData_array[(*metadata_counter)++] = inode->triple_indirect_blocks[triple_indirect_counter];

	    dread(imagefd, *(int*)(tripleIndirectPointer_block+(double_indirect_counter*4)), doubleIndirectPointer_block);
	    metaData_array[(*metadata_counter)++] = *(int*)(tripleIndirectPointer_block+(double_indirect_counter*4));

	    dread(imagefd, *(int*)(doubleIndirectPointer_block+(indirect_counter*4)), indirectPointer_block);
	    metaData_array[(*metadata_counter)++] = *(int*)(doubleIndirectPointer_block+(indirect_counter*4));
	    
	  }
	}
      }
    }
  }//while(blocks_read < total_blocks)
}


////////////////////////////////////////////////////////////
// Allocate_Inode_Pointers
//
// Given a file of size 'size', Allocate_Inode_Pointers will allocate all the necessary metadata
// (direct/indirect pointers etc.) to write tehf ile to disk.  THis function assumes
// all the necessary data blocks have already been allocated and data SHOULD be written
// before calling this function.
//
// Fills in "node" with all the allocated pointers
////////////////////////////////////////////////////////////
int Allocate_Inode_Pointers(INode * node, int inode_num, unsigned int size_bytes, int * allocated_data_block_array)
{
  
  int num_data_blocks = (int) ceil( (double)size_bytes / (double)512 );
  int direct_counter = 0;
  int indirect_counter = 0;
  int double_indirect_counter = 0;
  int triple_indirect_counter = 0;
  int data_blocks_allocated = 0;
  
  //0 means currently on direct, 1 means on indirect, 2 means on doubly-indirect, 3 means on triply-indirect
  int which_pointer = 0;

  //char doubleIndirectPointer_block[512];
  char indirectPointer_block[512];
  char doubleIndirectPointer_block[512];
  char tripleIndirectPointer_block[512];
  node->size = size_bytes;

  while(data_blocks_allocated < num_data_blocks)
  {
    if(which_pointer == 0)
    {
      node->direct_blocks[direct_counter] = allocated_data_block_array[data_blocks_allocated];

      direct_counter++;
      data_blocks_allocated++;
      
      if(data_blocks_allocated >= num_data_blocks)
      {
	//nothing to flush to disk. ... return
	return 0;
      }
      if(direct_counter == 7)
      {
	//time to move on to indirect blocks
	direct_counter = 0;
	which_pointer = 1;
	
	//Allocate a block for indirect pointers
	node->indirect_blocks[indirect_counter] = Find_And_Mark_Free_Block();
	if(node->indirect_blocks[indirect_counter] == -1)
	{

	  return -1;
	}

      }
      
    }else if(which_pointer == 1) {
      *(int*)(indirectPointer_block+(direct_counter*4)) = allocated_data_block_array[data_blocks_allocated];


      direct_counter++;
      data_blocks_allocated++;
      
      //Check if we are done writing!!
      if( data_blocks_allocated >= num_data_blocks)
      {
	//Done writing blocks .... write changes to our indirect block & break out
	dwrite(imagefd, node->indirect_blocks[indirect_counter], indirectPointer_block);
	return 0;
      }
      else if(direct_counter == 128)
      {
	//Write back the indirect block we have filled in & move on to (1) next indirect block or 
	//(2) double indirect blocks
	dwrite(imagefd, node->indirect_blocks[indirect_counter], indirectPointer_block);
	indirect_counter++;
	direct_counter = 0;
	if(indirect_counter < 3)
	{
	  //allocate a new indirect block and move on
	  node->indirect_blocks[indirect_counter] = Find_And_Mark_Free_Block();
	  if(node->indirect_blocks[indirect_counter] == -1)
	  {
	    return -1;
	  }

	}else{
	  which_pointer = 2;
	  //move on to double indirect blocks
	  indirect_counter = 0;
	  double_indirect_counter = 0;
	  node->double_indirect_blocks[double_indirect_counter] = Find_And_Mark_Free_Block();
	  if(node->double_indirect_blocks[double_indirect_counter] == -1)
	  {

	    return -1;
	  }

	  *(int*)(doubleIndirectPointer_block+(indirect_counter*4)) = Find_And_Mark_Free_Block();

	  if(*(int*)(doubleIndirectPointer_block+(indirect_counter*4)) == -1)
	  {

	    return -1;
	  }

	}	
      }

    }else if(which_pointer == 2) {
      //In double indirect block here
      *(int*)(indirectPointer_block+(direct_counter*4)) = allocated_data_block_array[data_blocks_allocated];


      direct_counter++;
      data_blocks_allocated++;
      
      //if we are done writing
      if( data_blocks_allocated >= num_data_blocks)
      {
	//done writing!  write our double indirect pointer to disk and our indirect pointer to disk
	dwrite(imagefd, node->double_indirect_blocks[double_indirect_counter], doubleIndirectPointer_block);
	dwrite(imagefd, *(int*)(doubleIndirectPointer_block+(4*indirect_counter)), indirectPointer_block);
	return 0;
      }
      
      if(direct_counter == 128)
      {
	//Finished with this direct block

	dwrite(imagefd, *(int*)(doubleIndirectPointer_block+(4*indirect_counter)), indirectPointer_block);	

	direct_counter = 0;
	indirect_counter++;
	
	if( indirect_counter < 128 )
	{
	  //still have room in current double-indirect block
	  *(int*)(doubleIndirectPointer_block+(indirect_counter*4)) = Find_And_Mark_Free_Block(); 
	  if(*(int*)(doubleIndirectPointer_block+(indirect_counter*4)) == -1)
	  {

	    return -1;
	  }


	}else{
	  //in this implementation, only have 1 double-indirect block ... time to move on
	  //time to move into triple-indirectness

	  which_pointer = 3;
	  dwrite(imagefd, node->double_indirect_blocks[double_indirect_counter], doubleIndirectPointer_block);
	  
	  direct_counter = 0;
	  indirect_counter = 0;
	  double_indirect_counter = 0;
	  triple_indirect_counter = 0;

	  node->triple_indirect_blocks[triple_indirect_counter] = Find_And_Mark_Free_Block();
	  if(node->triple_indirect_blocks[triple_indirect_counter] == -1)
	  {

	    return -1;
	  }

	  *(int*)(tripleIndirectPointer_block+(double_indirect_counter*4)) = Find_And_Mark_Free_Block();
	  if( *(int*)(tripleIndirectPointer_block+(double_indirect_counter*4)) == -1)
	  {

	    return -1;
	  }
	  	  
	  *(int*)(doubleIndirectPointer_block+(indirect_counter*4)) = Find_And_Mark_Free_Block();	  
	  if( *(int*)(doubleIndirectPointer_block+(indirect_counter*4)) == -1 )
	  {

	    return -1;
	  }

	}
      }

          
    }else if(which_pointer == 3){
      //Doing triple-indirect blocks


      *(int*)(indirectPointer_block+(direct_counter*4)) = allocated_data_block_array[data_blocks_allocated];
      direct_counter++;
      data_blocks_allocated++;

      //if we are done writing
      if( data_blocks_allocated >= num_data_blocks)
      {
	//Flush necessary data structures to disk & roll out
	dwrite(imagefd, node->triple_indirect_blocks[triple_indirect_counter], tripleIndirectPointer_block);
	dwrite(imagefd, *(int*)(tripleIndirectPointer_block+(4*double_indirect_counter)), doubleIndirectPointer_block);
	dwrite(imagefd, *(int*)(doubleIndirectPointer_block+(4*indirect_counter)), indirectPointer_block);

	return 0;
      }
      
      if(direct_counter == 128)
      {
	//move on to next indirect block ... might need to change a bunch of pointers along the way
	
	dwrite(imagefd, *(int*)(doubleIndirectPointer_block+(4*indirect_counter)), indirectPointer_block);

	direct_counter = 0;
	indirect_counter++;

	if( indirect_counter < 128 )
	{
	  //current double-indirect block still has room
	  *(int*)(doubleIndirectPointer_block+(indirect_counter*4)) = Find_And_Mark_Free_Block();
	  if(*(int*)(doubleIndirectPointer_block+(indirect_counter*4)) == -1)
	  {

	    return -1;
	  }

	}else{
	  //We maxed out the double-indirect block ... write it to disk and try to allocate another
	  
	  dwrite(imagefd, *(int*)(tripleIndirectPointer_block+(4*double_indirect_counter)), doubleIndirectPointer_block);	  
	  indirect_counter = 0;
	  double_indirect_counter++;
	  
	  if( double_indirect_counter < 128 )
	  {
	    //We still have room in triple-indirect block
	    *(int*)(tripleIndirectPointer_block+(double_indirect_counter*4)) = Find_And_Mark_Free_Block();
	    if(*(int*)(tripleIndirectPointer_block+(double_indirect_counter*4)) == -1 )
	    {

	      return -1;
	    }
	    
	  }else{
	    //Current triple indirect block is maxed out!  write it to disk
	    dwrite(imagefd, node->triple_indirect_blocks[triple_indirect_counter], tripleIndirectPointer_block);
	    double_indirect_counter = 0;
	    triple_indirect_counter++;
	    if(triple_indirect_counter >= 4)
	    {
	      //ran out of space ... this shouldn't happen though
	      assert(0);
	      return -1;
	    }else{
	      node->triple_indirect_blocks[triple_indirect_counter] = Find_And_Mark_Free_Block();
	      if(node->triple_indirect_blocks[triple_indirect_counter] == -1)
	      {

		return -1;
	      }

	      *(int*)(tripleIndirectPointer_block+(double_indirect_counter*4)) = Find_And_Mark_Free_Block();
	      if( *(int*)(tripleIndirectPointer_block+(double_indirect_counter*4)) == -1 )
	      {

		return -1;
	      }

	      
	      *(int*)(doubleIndirectPointer_block+(indirect_counter*4)) = Find_And_Mark_Free_Block();
	      if( *(int*)(doubleIndirectPointer_block+(indirect_counter*4)) == -1 )
	      {

		return -1;
	      }
	    }
	  }	  
	}
      }
      
    }else{
      assert(0);
    }
  }

}

//Search_Directory_List
// Started at a given directory block: curr_dir_block, will search for an entry for a given name
// It basically return an entry for that file - filling in size, first data block and also the 
// directory block number where entry where the entry was located (not necessarily curr_dir_block)
int 
Search_Directory_List( int curr_dir_block, const char * name, int * isDirectory, int * firstDataBlockNum,
		       int * final_directory_block)
{
  char dirBlockData[512];
  int i;
  Directory_Block dirBlock;

  dread(imagefd, curr_dir_block, dirBlockData);
  Parse_Directory_Block(&dirBlock, dirBlockData);

  //14 entries of size 36 in a directory block .... the 504th, 5ht, 6th and 7th byte hold a pointer
  //to the next directory block


  i = 0;
  while(1)
  {
    //Got to an invalid entry!  no more directory blocks will exist after here
    //if( *(int*)(dirBlock+(i*36)+32) == -1)
    if( is_Dir_Entry_Empty(&dirBlock.entries[i]) )
    {
      //return -1;
    }else{
      //Found the data block
      if(strcmp(name, dirBlock.entries[i].file_name) == 0)
      {
	//*size = *(int*)(dirBlock+(i*36)+28);
	//*firstDataBlockNum = *(int*)(dirBlock+(i*36)+32);
	//*final_directory_block = curr_dir_block;
	*isDirectory = dirBlock.entries[i].isDirectory;
	*firstDataBlockNum = dirBlock.entries[i].first_data_block;
	*final_directory_block = curr_dir_block;
	return 0;
      }
    }

    //Do we need to read another directory block
    if(i == 14)
    {
      //curr_dir_block = *(int*)(dirBlock+(14*36));
      curr_dir_block = dirBlock.next_directory_block;
      i = 0;
      if(curr_dir_block == -1)
      {
	return -1;
      }
      dread(imagefd, curr_dir_block, dirBlockData);
      Parse_Directory_Block(&dirBlock, dirBlockData);
    }else{
      i++;
    }
  }
  //COuld not find directory listing
  return -1;
}

int GetDirectoryEntry(
    int              block,
    const char*      file,
    Directory_Block* dirBlock,
    int*             fileEntryNum)
{
    do {
        int  i;
        char blockData[512];

        if ((i = dread(imagefd, block, blockData)) < 0) 
            return i;

        Parse_Directory_Block(dirBlock, blockData);
        for(i = 0; i < 14; i++) {
	        if (is_Dir_Entry_Empty(&dirBlock->entries[i]))         continue;
            if (strcmp(dirBlock->entries[i].file_name, file) != 0) continue;

            if (fileEntryNum != NULL) 
                *fileEntryNum = i;

            return block;
        }

        block = dirBlock->next_directory_block;
    } while (block != -1);

    return -1;
}

void PrintIndirectBlock(int level, int blockNum) {
    if (blockNum == 0) return ;
    if (level == 0) {
        fprintf(stderr, " %d ", blockNum);
        return ;
    }

    fprintf(stderr, "Level(%d) : Block Num %d\n", level, blockNum);

    char buffer[512];
    int* blocks = (int *) buffer;
    if (dread(imagefd, blockNum, buffer) < 0) {
        fprintf(stderr, "PrintIndirectBlock: unable to read block ", blockNum);
        fprintf(stderr, " at level (%d)", level);
        return ;
    }

    int i;
    level--;
    for(i = 0; i < blockNumsPerBlock; i++) {
        PrintIndirectBlock(level, *blocks++);
    }
    
    fprintf(stderr, "\n");
}

int PrintInode(INode *inode, int inodeNum) {
    int  i;
    
    fprintf(stderr, "Inode = %d : size = %d \n", inodeNum, inode->size);
    fprintf(stderr, "Direct Blocks: ");
    for(i = 0; i < 7; i++) 
        PrintIndirectBlock(0, inode->direct_blocks[i]);
    fprintf(stderr, "\n");

    for(i = 0; i < 3; i++) {
        if (inode->indirect_blocks[i])
            fprintf(stderr, "Indirect Block %d\n", i);

        PrintIndirectBlock(1, inode->indirect_blocks[i]);
    }

    if (inode->double_indirect_blocks[0])
        fprintf(stderr, "Double Indirect Block \n");
    PrintIndirectBlock(2, inode->double_indirect_blocks[0]);

    for(i = 0; i < 4; i++) {
        if (inode->triple_indirect_blocks[i])
            fprintf(stderr, "Triple Indirect Block %d\n", i);

        PrintIndirectBlock(3, inode->triple_indirect_blocks[i]);
    }

    return 0;
}

int ReadInode(INode *inode, int inodeNum) {
    int result;
    int offset       = inodeNum % 8;
    int dataBlockNum = _inodeStartBlock + (inodeNum / 8);

    char inodeDataBlock[512];
    if ((result = dread(imagefd, dataBlockNum, inodeDataBlock)) < 0) {
        fprintf(stderr, "ReadInode: Unable to read the inode %d\n", inodeNum);   
        return result;
    }

    int  i;
    int* buf = (int*)(inodeDataBlock + (offset * 64));
    
    inode->size = *buf;
    for(i = 0; i < 7; i++)
        inode->direct_blocks[i] = *++buf;

    for(i = 0; i < 3; i++)
        inode->indirect_blocks[i] = *++buf;

    inode->double_indirect_blocks[0] = *++buf;
    for(i = 0; i < 4; i++)
        inode->triple_indirect_blocks[i] = *++buf;

    return 0;
}

int WriteInode(const INode* inode, int inodeNum) {
    int result;
    int offset       = inodeNum % 8;
    int dataBlockNum = _inodeStartBlock + (inodeNum / 8);

    fprintf(stderr, "WriteInode: inodeNum = %d : dataBlockNum = ", inodeNum);
    fprintf(stderr, "%d \n", dataBlockNum);

    char inodeDataBlock[512];
    if ((result = dread(imagefd, dataBlockNum, inodeDataBlock)) < 0) {
        fprintf(stderr, "WriteInode: Unable to read the inode %d\n", inodeNum);   
        return result;
    }

    //Let's update the inode in the block
    int  i;
    int* buf = (int*)(inodeDataBlock + (offset * 64));

    *buf = inode->size;
    for(i = 0; i < 7; i++) {
        *++buf = inode->direct_blocks[i];
    }

    for(i = 0; i < 3; i++) {
        *++buf = inode->indirect_blocks[i];
    }

    *++buf = inode->double_indirect_blocks[0];
    for(i = 0; i < 4; i++) {
        *++buf = inode->triple_indirect_blocks[i];
    }
  
    if ((result = dwrite(imagefd, dataBlockNum, inodeDataBlock)) < 0) {
        fprintf(stderr, "WriteInode: Unable to write the inode %d\n", inodeNum);   
        return result;
    }

    return 0;
}

int DeleteInode(int inodeNum, INode* inode) {
    int blockNum   = (inodeNum / (512*8)) + _inodeFreeListStart;
    int bitOffset  = inodeNum % 8;
    int byteOffset = (inodeNum / 8) % 512;
  
    char blockData[512];
    
    int result;
    if ((result = dread(imagefd, blockNum, blockData)) < 0) {
        fprintf(stderr, "DeleteInode: Unable to read the free inode list ");
        fprintf(stderr, "block %d\n", inodeNum);   
        return result;
    }

    blockData[byteOffset] = blockData[byteOffset] | (0x1 << bitOffset);

    if ((result = dwrite(imagefd, blockNum, blockData)) < 0) {
        fprintf(stderr, "DeleteInode: Unable to write the free inode list ");
        fprintf(stderr, "block %d\n", inodeNum);   
        return result;
    }
    
    int inodeOffset   = inodeNum % 8;
    int inodeBlockNum = _inodeStartBlock + (inodeNum / 8);

    char inodeDataBlock[512];
    if ((result = dread(imagefd, inodeBlockNum, inodeDataBlock)) < 0) {
        fprintf(stderr, "DeleteInode: Unable to read the inode %d\n", inodeNum);   
        return result;
    }

    //Let's update the inode in the block
    int  i;
    int* buf = (int*)(inodeDataBlock + (inodeOffset * 64));

    *buf = 0;
    for(i = 0; i < 7; i++) {
        *++buf = 0;
        DeleteBlocks(0, inode->direct_blocks[i]);
    }

    for(i = 0; i < 3; i++) {
        *++buf = 0;
        DeleteBlocks(1, inode->indirect_blocks[i]);
    }

    *++buf = 0;
    DeleteBlocks(2, inode->double_indirect_blocks[i]);

    for(i = 0; i < 4; i++) {
        *++buf = 0;
        DeleteBlocks(3, inode->triple_indirect_blocks[i]);
    }
  
    if ((result = dwrite(imagefd, inodeBlockNum, inodeDataBlock)) < 0) {
        fprintf(stderr, "DeleteInode: Unable to write the inode %d\n", inodeNum);   
        return result;
    }

    return 0;
}

int DeleteBlocks(int level, int blockNum) {
    if (blockNum == 0) 
        return 0;

    if (level == 0) {
        Mark_Block_Free(blockNum);
        return 0;
    }

    int  result;
    char buffer[512];

    if ((result = dread(imagefd, blockNum, buffer)) < 0)
        return result;

    int  i;
    int* blocks = (int *)buffer;

    level--;
    for(i = 0; i < blockNumsPerBlock; i++) {
        DeleteBlocks(level, *blocks++);
    }
}


/* converts the path to the inode information */
int namei(const char* Path, int* isDir, int* firstDataBlockNum, int checkLastComponent) {
    int finalDir, ir;

    int curDir      = _firstDirectoryBlock;
    int firstData   = _firstDirectoryBlock; 
    int isDirectory = 1;

    char tmp[4096];
    char* path  = tmp;

    if (strlen(Path) >= 4095) {
        return -ENAMETOOLONG;
    }

    if (path == NULL) {
        fprintf(stderr, "namei: Unable to allocate memory for path \n");
        return -ENOMEM;
    }

    strcpy(path, Path);
    if (*path != '/') {
        fprintf(stderr, "namei: Expecting a full path, got %s\n", path);
        return -EINVAL;
    }

    int lastComponent = 0;
    path++;
    while (*path != 0) {
        
        //Let find the next component
        char* fileName = path;
        while (*path && (*path != '/'))
            path++;

        // is it the last component?
        if ((*path == '\0') || ( (*path == '/') && (path[1] == '\0') ) ) {
            lastComponent = 1;
            if (!checkLastComponent) break;
        }

        *path = '\0';
        if (strlen(fileName) >= 28) {
            return -ENAMETOOLONG;
        }

        ir = Search_Directory_List(curDir, fileName, &isDirectory, 
                                   &firstData, &finalDir);
        if (ir == -1) {
            return -ENOENT;
        }

        if (lastComponent) break;

        //not a directory entry
        if (isDirectory != 1) {
            return -ENOTDIR;
        }

        path++; // move '/'
        curDir = firstData;
    }

    if (isDir != NULL) *isDir = isDirectory;
    if (firstDataBlockNum != NULL) *firstDataBlockNum = firstData;

    return 0;

/*
    char* path  = (char*)malloc(strlen(Path));
    char* start = path;

    if (path == NULL) {
        fprintf(stderr, "namei: Unable to allocate memory for path \n");
        free(start);
        return -ENOMEM;
    }

    strcpy(path, Path);
    if (*path != '/') {
        fprintf(stderr, "namei: Expecting a full path, got %s\n", path);
        free(start);
        return -EINVAL;
    }

    int lastComponent = 0;
    path++;
    while (*path != 0) {
        
        //Let find the next component
        char* fileName = path;
        while (*path && (*path != '/'))
            path++;

        // is it the last component?
        if ((*path == '\0') || ( (*path == '/') && (path[1] == '\0') ) ) {
            lastComponent = 1;
            if (!checkLastComponent) break;
        }

        *path = '\0';
        if (strlen(fileName) >= 28) {
            free(start);
            return -ENAMETOOLONG;
        }

        ir = Search_Directory_List(curDir, fileName, &isDirectory, 
                                   &firstData, &finalDir);
        if (ir == -1) {
            free(start);
            return -ENOENT;
        }

        if (lastComponent) break;

        //not a directory entry
        if (isDirectory != 1) {
            free(start);
            return -ENOTDIR;
        }

        path++; // move '/'
        curDir = firstData;
    }

    if (isDir != NULL) *isDir = isDirectory;
    if (firstDataBlockNum != NULL) *firstDataBlockNum = firstData;

    free(start);
    return 0;
*/
}

/**
 * Initialize filesystem
 *
 * You need the filename that you are using as disk to be passed to this 
 * function, since it is not taking any parameters, hard code the file name 
 * (we defined this file name in cse121fs.h as DISKFILE). This function will 
 * use that file as the disk image to mount the application-level filesystem.  
 * You also need to open this disk file and store the file discriptor in a
 * global variable or super block to read and write from this file later (or
 * you can open this file whenever you needed and close it after that).
 * Once a mount occurs, that file will be used as the disk for all further 
 * file system commands you will implement (until an xmp_unmount occurs).  
 * Remember that this is an application level file system.  From the point 
 * of view of a user of the application, it will appear like a structured 
 * file system.  However, in reality the disk image is merely one contiguous 
 * file in the operating system.
 * 
 * This is also when you will need to check the integrity of your file 
 * system.  For example, if a crash occurred during a file create after an
 * inode was allocated but before a directory entry is updated, such an error 
 * should be found and fixed in xmp_mount.
 *
 * The return value will passed in the private_data field of
 * fuse_context to all file operations and as a parameter to the
 * xmp_unmount() method.
 *
 */
static void* xmp_mount(void) {

    if((imagefd = open(DISKFILE, O_RDWR)) == -1) {
        perror("Mount:");
        fprintf(stderr, "Mount: Unable to open the disk file %s\n", DISKFILE);
        fprintf(stderr, "Mount Failed \n");
        exit(100);
    }

    char superBlock[512];
    if (dread(imagefd, 0, superBlock) < 0) {
        perror("Mount:");
        fprintf(stderr, "Mount: Unable to read the disk file %s\n", DISKFILE);
        fprintf(stderr, "Mount Failed \n");
        exit(100);
    }

     _numBlocks           = *(int*)(superBlock);
     _inodeFreeListStart  = *(int*)(superBlock+4);
     _inodeFreeListEnd    = *(int*)(superBlock+8);
     _dataFreeListStart   = *(int*)(superBlock+12);
     _dataFreeListEnd     = *(int*)(superBlock+16);
     _inodeStartBlock     = *(int*)(superBlock+20);
     _inodeEndBlock       = *(int*)(superBlock+24);
     _firstDirectoryBlock = *(int*)(superBlock+28);

    benchmark_set_disk_size_in_blocks(_numBlocks);

    blockNumsPerBlock            = 512 / sizeof(int);
    blocksPerIndirectBlock       = blockNumsPerBlock;
    blocksPerDoubleIndirectBlock = blockNumsPerBlock * blockNumsPerBlock;
    blocksPerTripleIndirectBlock = blockNumsPerBlock * blockNumsPerBlock * blockNumsPerBlock;

    firstIndirectBlock           = 7;
    firstDoubleIndirectBlock     = firstIndirectBlock + (3 * blocksPerIndirectBlock);
    firstTripleIndirectBlock     = firstDoubleIndirectBlock + (1 * blocksPerDoubleIndirectBlock);
    maxBlocks                    = firstTripleIndirectBlock + (4 * blocksPerTripleIndirectBlock);

    fprintf(stderr, "Mount is success\n");
    fprintf(stderr, "Size Of Inode = %d", sizeof(INode));
    return NULL;
}

/*
 *
 * Unmount is responsible for unmounting the file system. In addition to 
 * closing the disk image file descriptor, unmount will need to write out 
 * any necessary meta data that might be required the next time the file 
 * system is mounted. For instance, it might note that the filesystem was 
 * cleanly unmounted, speeding up the integrity check the next time the 
 * file system is mounted.
 *
 */
static void xmp_unmount (void *fsData) {
    /* You may need to write some final metadata here... */
    close(imagefd);
}

/** 
 *
 * Given an absolute path to a file/directory 
 * (i.e., /foo/bar ---all paths will start with the root directory, "/"),
 * you need to return the file attributes that is similar stat system call. 
 *
 * However you don't need to fill all the attributes except file size, 
 * file size in blocks and block size of file system. 
 *
 * You can ignore the 'st_dev', 'st_blksize' and 'st_ino' fields 
 * (FUSE ignore these values and for more information look into fuse.h file).
 * 
 * For st_mode you have to send the S_IFDIR if the entry is directory 
 * or S_IFREG if it is a simple file
 *
 * Since we are not implementing permissions, 
 * we pass fixed values to st_uid (5), st_gid(500) entries.
 *
 * We are also not implementing hard links,
 * so set the st_nlinks to 3 (just as safe side). 
 *
 * Its up to you to implemet the time fields (st_atime, st_mtime, st_ctime), 
 * if didn't impement them just return current time.
 * 
 * The only thing you would be returning correctly is the file size and 
 * size in blocks (st_size, st_blocks) and file system block size (st_blksize)
 *
 * The following are common steps to implement this function:
 * 1. Resolve the directory path -- this
 *    will probably involve reading each directory in the path to get
 *    the location of the file entry (or location of data block with
 *    the file's inode) of the specified file.
 * 
 * 2. Fill the file size, file size in blocks, block size information
 *
 * You should return 0 on success or -1 on error (e.g., the path doesn't
 * exist).
 */
static int xmp_getattr(const char *path, struct stat *stbuf) {

    /* 
     * I am setting some pluasible values but you can change the following to 
     * other values.
     */
    stbuf->st_nlink   = 3;
    stbuf->st_uid     = 5;
    stbuf->st_gid     = 500;
    stbuf->st_rdev    = 0;
    stbuf->st_mode    = 0777;
    stbuf->st_atime   = time(NULL);
    stbuf->st_mtime   = time(NULL);
    stbuf->st_ctime   = time(NULL);
    stbuf->st_blksize = 512;

    /* We will handle the  root dir as a special case */
    if (strcmp(path, "/") == 0) {
        stbuf->st_mode    = 0777 | S_IFDIR;
        stbuf->st_size    = 1024; // file size
        stbuf->st_blocks  = 2;    // file size in blocks
        stbuf->st_blksize = 512;  // block size of you file system
        return 0;
    }

    int isDir, inodeNum;
    int res = namei(path, &isDir, &inodeNum, 1);
    if (res < 0) return res;

    if (isDir != 0) {
        stbuf->st_mode  |= S_IFDIR;
        stbuf->st_size   = 1024; 
        stbuf->st_blocks = 2;
    } else {
        int   res;
        INode inode;

        if ((res = ReadInode(&inode, inodeNum)) < 0) {
            fprintf(stderr, "GetAttr: Unable to read inode (%d) for path %s \n", inodeNum, path);
            return res;
        }

        stbuf->st_mode  |= S_IFREG;
        stbuf->st_size   = inode.size; 
        stbuf->st_blocks = (int) ceil( (double) inode.size / (double) 512 );
    }

    return 0;
}

/**
 * Given an absolute path to a directory (which may or may not end in
 * '/'), xmp_mkdir will create a new directory named dirname in that
 * directory. Ignore the mode parameter as we are not implementing 
 * the permissions. The steps your implementation will do to make a new
 * directory will be the following: 
 *
 * 1. Resolve the directory path
 *
 * 2. Allocate and initialize a new directory block  
 *
 * 3. Update the directory block for 'path' by adding an entry for 
 *    the newly created directory. 
 *
 * 4. You also need to create appropriate entries for "." and ".." in the 
 *    New directory
 *
 * You should return 0 on success or -1 on error (e.g., the path doesn't
 * exist, or dirname already does).
 *
 */
static int xmp_mkdir(const char *path, mode_t mode) {
    int i, j, k;
    int isDir, firstDataBlock;
    int result = namei(path, &isDir, &firstDataBlock, 0);

    if (result < 0) return result;
    if (isDir != 1) return -ENOTDIR;

    const char* end = path + strlen(path);
    if (*--end == '/') end--; // skip the last '/' 

    //Lets find out the last component;
    int nameLen = 0; 
    while (end != path && *end != '/') {
        end--;
        nameLen++;
    }

    if (nameLen ==  0) return -EINVAL;
    if (nameLen >= 28) return -ENAMETOOLONG;

    char newDirName[28];
    strncpy(newDirName, ++end, nameLen);
    newDirName[nameLen] = '\0';
    result = Search_Directory_List(firstDataBlock, newDirName, &i, &j, &k);
    if (result == 0) return -EEXIST;

    //Find free block from FAT and mark it as used
    char newDirBlock[512];
    int  freeDirBlock = Find_And_Mark_Free_Block();
    if (freeDirBlock == -1 ) {
        fprintf(stderr, "mkdir: Unable to find a free block to %s\n", newDirBlock);
        return -ENOMEM;
    }

    //Write directory information to new directory block
    for(i = 0; i < 14; i++)
    {
        *(int*)(newDirBlock+(i*36+28)) = -1;
        *(int*)(newDirBlock+(i*36+32)) = -1;
        *(int*)(newDirBlock+504) = -1;
    }

    if ((result = dwrite(imagefd, freeDirBlock, newDirBlock)) < 0) {
        fprintf(stderr, "mkdir: Unable to write the new dir first block \n");
        return result;
    }
	    
    Directory_Entry dir_entry;
    dir_entry.isDirectory      = 1;
    dir_entry.first_data_block = freeDirBlock;
    memcpy(dir_entry.file_name, newDirName, nameLen + 1);
    if ((result = Add_Directory_Entry(firstDataBlock, &dir_entry)) < 0) {
        fprintf(stderr, "mkdir: Unable to add (%s) new dir\n", newDirName);
        return result;
    }

    return 0;
}

/** Read directory
 *
 * Given an absolute path to a directory, xmp_readdir will return 
 * all the files and directories in that directory. The steps you 
 * will take will be the following:
 *
 * 1. Resolve the directory entry from its absolute path to get 
 *    the address of the directory block that corresponds to "path"
 *
 * 2. For each valid entry in the directory block, copy the file 
 *    name into an array (buf) using the function filler. The filler
 *    is already implemeted in fuse and its sample use is shown in 
 *    fusexmp.c file and you should pass the zero to the offest 
 *    paramter of filler function. fuse.h file has some more infomration
 *    about how to implement this function and another way of implementing it.
 *
 * For time being ignore the fuse_file_info parameter. If you would 
 * like to use please see the open function in fuse.h file for more 
 * detials.
 *
 * You should return 0 on success or -1 on error (e.g., the directory 
 * does not exist).
 */
static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                       off_t offset, struct fuse_file_info *fi)
{
    int isDir, curBlock;
    int result = namei(path, &isDir, &curBlock, 1);

    if (result < 0) return result;
    if (isDir != 1) return -ENOTDIR;

    char dirBlockData[512];
    Directory_Block dirBlock;
    
    while (curBlock != -1) {
        int result = dread(imagefd, curBlock, dirBlockData);
        if (result < 0) {
            fprintf(stderr, "ReadDir: Unable to read directroy block(%d) \n", curBlock);
            return result;
        }

	filler(buf, ".", NULL, 0);
	filler(buf, "..", NULL, 0);

        Parse_Directory_Block(&dirBlock, dirBlockData);
 
        int i;
        for(i = 0; i < 14; i++) {
            // Is the entry empty?
            if (dirBlock.entries[i].first_data_block != -1) {
                struct stat st;
                memset(&st, 0, sizeof(st));
                st.st_mode  = 0777;
                st.st_mode |= (dirBlock.entries[i].isDirectory == -1) ? S_IFDIR : S_IFREG;
                if (filler(buf, dirBlock.entries[i].file_name,  &st, 0))
                    break;
            }
        }

        curBlock = dirBlock.next_directory_block;
    }
    return 0;
}

/*
 * Given an absolute path to a file (for example /a/b/myFile), xmp_create 
 * will create a new file named myFile in the /a/b directory. 
 * Ignore the mode parameter as we are not implementing 
 * the permissions. Also ignore the rdev parameter. 
 * The steps your implementation will do to make a new
 * file will be the following: (similar to the xmp_mkdir except the foruth step)
 *
 * 1. Resolve the directory path
 *
 * 2. Allocate and initialize a new directory entry  
 *
 * 3. Update the directory entry for 'path' by adding an entry for 
 *    the newly created file. 
 *
 *
 * You should return 0 on success or -1 on error (e.g., the path doesn't
 * exist, or file name already exists).
 */
static int xmp_create(const char *path, mode_t mode, dev_t rdev) {
    int i, j, k;
    int isDir, firstDataBlock;
    int result = namei(path, &isDir, &firstDataBlock, 0);

    if (result < 0) return result;
    if (isDir != 1) return -ENOTDIR;

    const char* end = path + strlen(path);
    if (*--end == '/') end--; // skip the last '/' 

    //Lets find out the last component;
    int nameLen = 0; 
    while (end != path && *end != '/') {
        end--;
        nameLen++;
    }

    if (nameLen ==  0) return -EINVAL;
    if (nameLen >= 28) return -ENAMETOOLONG;

    char newFileName[28];
    strncpy(newFileName, ++end, nameLen);
    newFileName[nameLen] = '\0';

    result = Search_Directory_List(firstDataBlock, newFileName, &i, &j, &k);
    if (result == 0) return -EEXIST;

    // this is make my job easy but we might be wasting a block in the worst case
    int inodeNum = Find_And_Mark_Free_Inode();
    if (inodeNum < 0) {
        fprintf(stderr, "Create: Unable to allocate inode to %s", newFileName);
        return inodeNum;
    }

    //Lets first write the inode
    INode inode;

    memset((char *)&inode, 0, sizeof(inode));
    if ((result = WriteInode(&inode, inodeNum)) < 0) {
        fprintf(stderr, "Create: Unable to write inode %d \n", inodeNum);
        return result;
    }
    
    Directory_Entry dir_entry;
    dir_entry.isDirectory      = 0;
    dir_entry.first_data_block = inodeNum;
    memcpy(dir_entry.file_name, newFileName, nameLen + 1);
    if ((result = Add_Directory_Entry(firstDataBlock, &dir_entry)) < 0) {
        fprintf(stderr, "Create: Unable to add (%s) new entry\n", newFileName);
        return result;
    }

    return 0;
}

/**
 * The function xmp_read provides the ability to read data from 
 * an absolute path 'path,' which should specify an existing file.
 * It will attempt to read 'size' bytes from the specified file 
 * on your filesystem into the memory address 'buf'. The return 
 * value is the amount of bytes actually read; if the file is 
 * smaller than size, xmp_read will simply return the most amount
 * of bytes it could read. 
 * 
 * For time being ignore the fuse_file_info parameter. If you would 
 * like to use please see the open function in fuse.h file for more 
 * detials.
 *
 * On error, xmp_read will return -1.  The actual implementation 
 * of xmp_read is dependent on how files are allocated.
 *
 */
int ReadBlock(int diskBlockNum, char* buffer) {
    if (diskBlockNum > _numBlocks) {
        fprintf(stderr, "ReadBlock: disk block number (%d) is ", diskBlockNum);
        fprintf(stderr, "larger than the disk capacity (%d) \n", _numBlocks);
        return -ENOMEM;
    }

    if (diskBlockNum == 0) {
        memset(buffer, 0, 512);
        return 512;
    }
        
    return dread(imagefd, diskBlockNum, buffer);
}

int ReadLogicalBlock(INode* inode, int blockNum, char* block) {
    int result;

    // is the block is direct block?
    if (blockNum < firstIndirectBlock) {

        return ReadBlock(inode->direct_blocks[blockNum], block);

    } else if (blockNum < firstDoubleIndirectBlock) { //double indirect block?
        int indirectBlock;
        
        blockNum      -= firstIndirectBlock;
        indirectBlock  = blockNum / blockNumsPerBlock;
        blockNum      %= blockNumsPerBlock;

        if ((result = ReadBlock(inode->indirect_blocks[indirectBlock], block)) < 0)
            return result;

        blockNum = ((int *)block)[blockNum];

        if ((result = ReadBlock(blockNum, block)) < 0)
            return result;

    } else if (blockNum < firstTripleIndirectBlock) { //Triple indirect block?
        int indirectBlock;
        int doubleIndirectBlock;

        blockNum            -= firstDoubleIndirectBlock;
        doubleIndirectBlock  = blockNum / blocksPerDoubleIndirectBlock;
        indirectBlock        = blockNum % blocksPerDoubleIndirectBlock;
        blockNum             = indirectBlock % blocksPerIndirectBlock;
        indirectBlock       /= blocksPerIndirectBlock;
        
        if (doubleIndirectBlock != 0) {
            fprintf(stderr, "ReadLogicalBlock: Error in calculating double indirect ");
            fprintf(stderr, "block number (%d) should be \n", doubleIndirectBlock);
            fprintf(stderr, "less than 1 \n");
            return -EIO;
        }

        //Lets read the double indirect block first
        doubleIndirectBlock = inode->double_indirect_blocks[doubleIndirectBlock];

        if ((result = ReadBlock(doubleIndirectBlock, block)) < 0)
            return result;

        indirectBlock = ((int *)block)[indirectBlock];

        if ((result = ReadBlock(indirectBlock, block)) < 0) 
            return result;

        blockNum = ((int *)block)[blockNum];

        if ((result = ReadBlock(blockNum, block)) < 0) 
            return result;
        
    } else if (blockNum < maxBlocks) {
        int indirectBlock;
        int doubleIndirectBlock;
        int tripleIndirectBlock;

        blockNum            -= firstTripleIndirectBlock;
        tripleIndirectBlock  = blockNum / blocksPerTripleIndirectBlock;

        if ((tripleIndirectBlock < 0) || (tripleIndirectBlock >= 4)) {
            fprintf(stderr, "ReadLogicalBlock: Error in calculating triple indirect ");
            fprintf(stderr, "block number (%d) should be \n", tripleIndirectBlock);
            fprintf(stderr, "greater than 0 and less than 4 \n");
            return -EIO;
        }

        tripleIndirectBlock = inode->triple_indirect_blocks[tripleIndirectBlock];
        if ((result = ReadBlock(tripleIndirectBlock, block)) < 0) 
            return result;

        blockNum            %= blocksPerTripleIndirectBlock;
        doubleIndirectBlock  = blockNum / blocksPerDoubleIndirectBlock;
        doubleIndirectBlock  = ((int *)block)[doubleIndirectBlock];
        if ((result = ReadBlock(doubleIndirectBlock, block)) < 0)
            return result;

        blockNum      %= blocksPerDoubleIndirectBlock;
        indirectBlock  = blockNum / blocksPerIndirectBlock;
        indirectBlock  = ((int *)block)[indirectBlock];
        if ((result = ReadBlock(indirectBlock, block)) < 0) 
            return result;
        
        blockNum %= blocksPerIndirectBlock;
        blockNum  = ((int *)block)[blockNum];
        if ((result = ReadBlock(blockNum, block)) < 0) 
            return result;

    } else {
        fprintf(stderr, "ReadLogicalBlock: block number(%d) is ");
        fprintf(stderr, "bigger than max allowed(%d)\n", blockNum, maxBlocks);
        return -EFBIG;
    }

    return 0;
}

static int xmp_read(const char *path, char *buf, size_t bufSize, off_t offset,
                    struct fuse_file_info *fi)
{
    int isDir, inodeNum;
    int result = namei(path, &isDir, &inodeNum, 1);

    if (result < 0) return result;
    if (isDir == 1) return -EISDIR;

    // Lets read the inode now
    INode inode;

    if ((result = ReadInode(&inode, inodeNum)) < 0) {
        fprintf(stderr, "Read: Unable to read Inode (%d) for path %s\n", inodeNum, path);
        return result;
    }

    int fileSize   = inode.size;
    int lastBlock  = offset + bufSize;
    int firstBlock = offset / 512;

    if (lastBlock > fileSize) // We are going beyond the file size
        lastBlock = fileSize;

    lastBlock = (int) ceil( lastBlock / 512.0);

    int  left;
    int  redBytes  = 0;
    int  curOffset = offset;
    char block[512];

    for(; firstBlock < lastBlock; firstBlock++) {
        int ret = ReadLogicalBlock(&inode, firstBlock, block);

        if (ret < 0) {
            fprintf(stderr, "Read: Unable to read block(%d) for file %s\n", firstBlock, path);
            if (ret == -EFBIG) return redBytes; //special case
            return ret;
        }

        offset = curOffset % 512;
        left   = 512 - offset;

        if (left > bufSize) left = bufSize;
        memcpy(buf, block + offset, left);

        buf       += left;
        bufSize   -= left;
        redBytes  += left;
        curOffset += left;
    }

    return redBytes;
}

/**
 * The function xmp_write will attempt to write 'size' bytes from 
 * memory address 'buf' into a file specified by an absolute 'path'
 *
 * For time being ignore the fuse_file_info parameter. If you would 
 * like to use please see the open function in fuse.h file for more 
 * detials.
 *
 * On error (e.g., the path does not exist) xmp_write will return -1, 
 * otherwise xmp_write should return the number of bytes written. 
 * 
 * Note size is not necessarily an integral number of blocks. 
 * Similar to xmp_read, the actual implementation of xmp_write will 
 * depend on how you decide to allocate file space.
 *
 */
int WriteBlock(int diskBlockNum, char* buffer) {
    if (diskBlockNum > _numBlocks) {
        fprintf(stderr, "WriteBlock: disk block number (%d) is ", diskBlockNum);
        fprintf(stderr, "larger than the disk capacity (%d) \n", _numBlocks);
        return -ENOMEM;
    }

    return dwrite(imagefd, diskBlockNum, buffer);
}

int AssignAndWriteBlock(char* buffer) {
    int blockNum = Find_And_Mark_Free_Block();

    if (blockNum < 0) {
        fprintf(stderr, "AssignAndWriteBlock: Unable to assign a new block\n");
        return -ENOMEM;
    }

    int result = WriteBlock(blockNum, buffer);

    if (result < 0) return result;

    return blockNum;
}

int RecursiveWrite(int level, int blockNum, char *bucket, char* block, int bucketBlockNum)
{
    int result;
    int blocksInThisGroup = 1;

    if (level == 0) {
        result = AssignAndWriteBlock(block);
    } else {
        int i = level;

        while (i--)
            blocksInThisGroup *= blocksPerIndirectBlock;
            
        result = RecursiveWrite(level - 1, blockNum % blocksInThisGroup, NULL, block, 0);
    }
    
    if (result < 0) return result;

    char blockPointers[512];
    if (bucket == NULL) {
        bucketBlockNum = Find_And_Mark_Free_Block();

        if (bucketBlockNum < 0) {
            fprintf(stderr, "RecursiveWrite: Unable to assign a new block\n");
            return -ENOMEM;
        }

        memset(blockPointers, 0, 512);
        bucket = blockPointers;
    } 

    int offset              = blockNum / blocksInThisGroup;
    ((int *)bucket)[offset] = result;

    if ((result = WriteBlock(bucketBlockNum, bucket)) < 0)
        return result;

    return bucketBlockNum;
}

/*
 * This function write the logical block (blockNum) and allocates new blocks as
 * necessary
 */
int WriteLogicalBlock(INode* inode, int blockNum, char* block, int* inodeUpdated) {
    int result;

    // is the block is direct block?
    if (blockNum < firstIndirectBlock) {

        int iblock = inode->direct_blocks[blockNum];
        result = (iblock == 0) ?  AssignAndWriteBlock(block) : WriteBlock(iblock, block);

        if (result < 0) return result;
        if (iblock == 0) {
            inode->direct_blocks[blockNum] = result;
            if (inodeUpdated != NULL) *inodeUpdated = 1;
        }

    } else if (blockNum < firstDoubleIndirectBlock) { //double indirect block?
        int  indirectBlock;
        char buffer[512];
        
        blockNum      -= firstIndirectBlock;
        indirectBlock  = blockNum / blockNumsPerBlock;
        blockNum      %= blockNumsPerBlock;

        int iblock = inode->indirect_blocks[indirectBlock];

        if (iblock == 0) {
            if ((result = RecursiveWrite(0, blockNum, NULL, block, 0)) < 0)
                return result;

            inode->indirect_blocks[indirectBlock] = result;
            if (inodeUpdated != NULL) *inodeUpdated = 1;

            return 0;
        } 

        if ((result = ReadBlock(iblock, buffer)) < 0)
            return result;

        int directBlock = ((int *)buffer)[blockNum];

        if (directBlock == 0) {
            if ((result = RecursiveWrite(0, blockNum, buffer, block, iblock)) < 0)
                return result;
            
            return 0;
        }

        if ((result = WriteBlock(directBlock, block)) < 0)
            return result;

    } else if (blockNum < firstTripleIndirectBlock) { //Triple indirect block?
        int  iblock;
        int  indirectBlock;
        int  doubleIndirectBlock;
        char buffer[512];

        blockNum            -= firstDoubleIndirectBlock;
        doubleIndirectBlock  = blockNum / blocksPerDoubleIndirectBlock;
        
        if (doubleIndirectBlock != 0) {
            fprintf(stderr, "ReadLogicalBlock: Error in calculating double indirect ");
            fprintf(stderr, "block number (%d) should be \n", doubleIndirectBlock);
            fprintf(stderr, "less than 1 \n");
            return -EIO;
        }

        //Lets read the double indirect block first
        iblock = inode->double_indirect_blocks[doubleIndirectBlock];

        if (iblock == 0) {
            if ((result = RecursiveWrite(1, blockNum, NULL, block, 0)) < 0)
                return result;

            inode->double_indirect_blocks[doubleIndirectBlock] = result;
            if (inodeUpdated != NULL) *inodeUpdated = 1;

            return 0;
        }

        if ((result = ReadBlock(iblock, buffer)) < 0)
            return result;

        indirectBlock  = blockNum % blocksPerDoubleIndirectBlock;
        indirectBlock /= blocksPerIndirectBlock;
        indirectBlock  = ((int *)buffer)[indirectBlock];

        if (indirectBlock == 0) {
            if ((result = RecursiveWrite(1, blockNum, buffer, block, iblock)) < 0)
                return result;

            return 0;
        }

        if ((result = ReadBlock(indirectBlock, buffer)) < 0) 
            return result;

        blockNum = blockNum % blocksPerIndirectBlock;
        iblock   = ((int *)buffer)[blockNum];

        if (iblock == 0) {
            if ((result = RecursiveWrite(0, blockNum, buffer, block, indirectBlock)) < 0)
                return result;

            return 0;
        }

        if ((result = WriteBlock(iblock, block)) < 0)
            return result;

    } else if (blockNum < maxBlocks) {
        int  iblock;
        int  indirectBlock;
        int  doubleIndirectBlock;
        int  tripleIndirectBlock;
        char buffer[512];

        blockNum            -= firstTripleIndirectBlock;
        tripleIndirectBlock  = blockNum / blocksPerTripleIndirectBlock;
        blockNum            %= blocksPerTripleIndirectBlock;

        if ((tripleIndirectBlock < 0) || (tripleIndirectBlock >= 4)) {
            fprintf(stderr, "ReadLogicalBlock: Error in calculating triple indirect ");
            fprintf(stderr, "block number (%d) should be \n", tripleIndirectBlock);
            fprintf(stderr, "greater than 0 and less than 4 \n");
            return -EIO;
        }

        iblock = inode->triple_indirect_blocks[tripleIndirectBlock];

        if (iblock == 0) {
            if ((result = RecursiveWrite(2, blockNum, NULL, block, 0)) < 0)
                return result;

            inode->triple_indirect_blocks[tripleIndirectBlock] = result;
            if (inodeUpdated != NULL) *inodeUpdated = 1;

            return 0;

        }

        if ((result = ReadBlock(iblock, buffer)) < 0) 
            return result;

        doubleIndirectBlock  = blockNum / blocksPerDoubleIndirectBlock;
        doubleIndirectBlock  = ((int *)buffer)[doubleIndirectBlock];

        if (doubleIndirectBlock == 0) {
            if ((result = RecursiveWrite(2, blockNum, buffer, block, tripleIndirectBlock)) < 0)
                return result;

            return 0;
        }

        if ((result = ReadBlock(doubleIndirectBlock, buffer)) < 0)
            return result;

        blockNum      %= blocksPerDoubleIndirectBlock;
        indirectBlock  = blockNum / blocksPerIndirectBlock;
        indirectBlock  = ((int *)buffer)[indirectBlock];

        if (indirectBlock == 0) {
            if ((result = RecursiveWrite(1, blockNum, buffer, block, doubleIndirectBlock)) < 0)
                return result;

            return 0;
        }

        if ((result = ReadBlock(indirectBlock, buffer)) < 0) 
            return result;
        
        blockNum %= blocksPerIndirectBlock;
        iblock    = ((int *)buffer)[blockNum];

        if (iblock == 0) {
            if ((result = RecursiveWrite(0, blockNum, buffer, block, indirectBlock)) < 0)
                return result;

            return 0;
        }

        if ((result = WriteBlock(iblock, block)) < 0) 
            return result;

    } else {
        fprintf(stderr, "ReadLogicalBlock: block number(%d) is ");
        fprintf(stderr, "bigger than max allowed(%d)\n", blockNum, maxBlocks);
        return -EFBIG;
    }

    return 0;
}

static int xmp_write(const char *path, const char *buf, size_t bufSize,
                     off_t offset, struct fuse_file_info *fi)
{
    int isDir, inodeNum;
    int result = namei(path, &isDir, &inodeNum, 1);

    if (result < 0) return result;
    if (isDir == 1) return -EISDIR;

    // Lets read the inode now
    INode inode;

    if ((result = ReadInode(&inode, inodeNum)) < 0) {
        fprintf(stderr, "Write: Unable to read Inode (%d) for path %s\n", inodeNum, path);
        return result;
    }

    int  written      = 0;
    int  curOffset    = offset;
    int  lastOffset   = offset + bufSize;
    int  firstBlock   = offset / 512;
    int  lastBlock    = (lastOffset - 1) / 512;
    int  inodeChanged = 0;
    char block[512];

    for(; firstBlock <= lastBlock; firstBlock++) {
        int tmp;
        int wrote = 512;

        if ((curOffset % 512) != 0) {
            int startOffset = curOffset % 512;

            wrote = 512 - startOffset;
            if (wrote > bufSize)
                wrote = bufSize;

            if ((result = ReadLogicalBlock(&inode, firstBlock, block)) < 0) {
                fprintf(stderr, "Write(%s): Unable to read the previous data ", path);
                fprintf(stderr, "of block (%d) \n", firstBlock);
                return result;
            }
            
            memcpy(block + startOffset, buf, wrote);
        } else {

            if (bufSize < 512) {
                if ((result = ReadLogicalBlock(&inode, firstBlock, block)) < 0) {
                    fprintf(stderr, "Write(%s): Unable to read the previous data ", path);
                    fprintf(stderr, "of block (%d) \n", firstBlock);
                    return result;
                }

                wrote = bufSize;
                memcpy(block, buf, bufSize);

            } else {
                memcpy(block, buf, 512); 
            }
        }

        result = WriteLogicalBlock(&inode, firstBlock, block, &tmp);
        if (result  < 0) {
            fprintf(stderr, "Write(%s): Unable to write ", path);
            fprintf(stderr, "the block(%d)\n", firstBlock);
            return result;
        }

        if (tmp == 1) inodeChanged = 1;

        buf       += wrote;
        bufSize   -= wrote;
        written   += wrote;
        curOffset += wrote;
    }

    if (inode.size < lastOffset) {
        inode.size   = lastOffset;
        inodeChanged = 1;
    }

    if (inodeChanged == 1) {
        if ((result = WriteInode(&inode, inodeNum)) < 0) {
            fprintf(stderr, "Write(%s): Unable to update ", path);
            fprintf(stderr, "the inode (%d)\n", inodeNum);
            return result;
        }
    }

    return written;
}

/**
 * This function deletes the last component of the path (e.g., /a/b/c you 
 * need to remove the file 'c' from the directory /a/b). The steps you 
 * will take will be the following:
 *
 * 1. Locate the directory entry for the file by resolving the path - this
 *    will probably involve reading each directory in the path to get
 *    the location of the first data block (or location of data block with
 *    the file's inode) of the specified file.
 *
 * 2. Update the directory entry for the deleted file
 *
 * 3. Free any data blocks used by the file
 *
 * Again, these steps are very general and the actual logistics of how 
 * to locate data blocks of a file, how to free data blocks and how to update
 * directory entries are dependent on your implementation of directories
 * and file allocation.
 *
 * For time being ignore the fuse_file_info parameter. If you would 
 * like to use please see the open function in fuse.h file for more 
 * detials.
 *
 * On error (e.g., the path does not exist) xmp_delete will return -1, 
 * otherwise xmp_delete should return the number of bytes written. 
 *
 */
static int xmp_delete(const char *path) {
    int  isDir, dirBlockNum;
    int  result = namei(path, &isDir, &dirBlockNum, 0);
    char fileName[28];

    if (result < 0) return result;
    if (isDir != 1) return -ENOTDIR;

    const char* end = path + strlen(path);
    if (*--end == '/') end--; // skip the last '/' 

    //Lets find out the last component;
    int nameLen = 0; 
    while (end != path && *end != '/') {
        end--;
        nameLen++;
    }

    if (nameLen ==  0) return -EINVAL;
    if (nameLen >= 28) return -ENAMETOOLONG;

    strncpy(fileName, ++end, nameLen);
    fileName[nameLen] = '\0';

    Directory_Block dirBlock; 
    int entryNum = -1;
    int dirEntryBlockNum = GetDirectoryEntry(dirBlockNum, fileName, &dirBlock, &entryNum);
    if (dirEntryBlockNum < 0) {
        if (dirEntryBlockNum == -1)
            return -ENOENT;

        fprintf(stderr, "Delete: Unable to parse the last dir (%s) \n", path);
        return dirEntryBlockNum;
    }

    if ((entryNum < 0) || (entryNum > 14)) {
        fprintf(stderr, "Delete: we messed up GetDirectoryEntry function ");
        fprintf(stderr, "For file %s ", path);
        fprintf(stderr, "Entry Num (%d) should be > 0 and < 14 ", entryNum);
        exit(1);
    }

    if (dirBlock.entries[entryNum].isDirectory == 1)
        return -EISDIR;

    int inodeNum   = dirBlock.entries[entryNum].first_data_block;

    dirBlock.entries[entryNum].isDirectory      = -1;
    dirBlock.entries[entryNum].first_data_block = -1;

    char dataBlock[512];
    Set_Data_Directory_Block(dataBlock, &dirBlock);
    if ((result = dwrite(imagefd, dirEntryBlockNum, dataBlock)) < 0) {
        fprintf(stderr, "Delete: file %s failed, unable to write dir entry \n", path);
        return result;
    }

    // Lets read the node and then free it
    INode inode;

    if ((result = ReadInode(&inode, inodeNum)) < 0) {
        fprintf(stderr, "Delete: Unable to read Inode(%d) for path %s\n", inodeNum, path);
        return result;
    }

    if ((result = DeleteInode(inodeNum, &inode)) < 0) {
        fprintf(stderr, "Unable to delete Inode for file %s \n", path);
        fprintf(stderr, "But file is deleted, needs to run fsck \n");
    } 

    return 0;
}

/**
 * The function rename will rename a file or directory named by the
 * string 'oldpath' and rename it to the file name specified by 'newpath'.
 *
 * As usual, return 0 on success, -1 on failure.
 *
 */
static int xmp_rename(const char *from, const char *to) {
    int  isDir, oldDirFirstDataBlock;
    int  result = namei(from, &isDir, &oldDirFirstDataBlock, 0);
    char oldFileName[28];

    if (result < 0) return result;
    if (isDir != 1) return -ENOTDIR;

    const char* end = from + strlen(from);
    if (*--end == '/') end--; // skip the last '/' 

    //Lets find out the last component;
    int nameLen = 0; 
    while (end != from && *end != '/') {
        end--;
        nameLen++;
    }

    if (nameLen ==  0) return -EINVAL;
    if (nameLen >= 28) return -ENAMETOOLONG;

    strncpy(oldFileName, ++end, nameLen);
    oldFileName[nameLen] = '\0';

    Directory_Block oldDirBlock;
    int oldEntryNum = -1;
    int oldDirBlockNum = GetDirectoryEntry(oldDirFirstDataBlock, oldFileName, 
                                            &oldDirBlock, &oldEntryNum);
    if (oldDirBlockNum < 0) return -ENOENT;
    if ((oldEntryNum < 0) || (oldEntryNum > 14)) {
        fprintf(stderr, "Look like we messed up GetDirectoryEntry function ");
        fprintf(stderr, " Entry Num (%d) should > 0 and < 14", oldEntryNum);
        exit(1);
    }

    int firstBlock = oldDirBlock.entries[oldEntryNum].first_data_block;
    //if (firstBlock < 0) return -EISDIR; //We are allowing to move directories

    // Let first create new entry
    int i, j, k;
    int size, firstDataBlock;

    result = namei(to, &size, &firstDataBlock, 0);
    if (result < 0) return result;
    if (size != 1)  return -ENOTDIR;

    end = to + strlen(to);
    if (*--end == '/') end--; // skip the last '/' 

    //Lets find out the last component;
    nameLen = 0; 
    while (end != to && *end != '/') {
        end--;
        nameLen++;
    }

    if (nameLen ==  0) return -EINVAL;
    if (nameLen >= 28) return -ENAMETOOLONG;

    char newFileName[28];
    strncpy(newFileName, ++end, nameLen);
    newFileName[nameLen] = '\0';
    result = Search_Directory_List(firstDataBlock, newFileName, &i, &j, &k);
    if (result == 0) return -EEXIST;

    // are we changing the name in same directory
    if (firstDataBlock == oldDirFirstDataBlock) {
        char dataBlock[512];

        strcpy(oldDirBlock.entries[oldEntryNum].file_name, newFileName);
        Set_Data_Directory_Block(dataBlock, &oldDirBlock);

        if ((result = dwrite(imagefd, oldDirBlockNum, dataBlock)) < 0) 
            return result;

        return 0;
    }

    Directory_Entry newDirEntry;

    newDirEntry.isDirectory      = oldDirBlock.entries[oldEntryNum].isDirectory;
    newDirEntry.first_data_block = oldDirBlock.entries[oldEntryNum].first_data_block;
    strcpy(newDirEntry.file_name, newFileName);

    if (Add_Directory_Entry(firstDataBlock, &newDirEntry) < 0) {
        fprintf(stderr, "Unable to add new entry(%s) to the dir (%s)\n", newFileName, to);
        return -EIO;
    }

    //Now lets delete the old entry
    oldDirBlock.entries[oldEntryNum].isDirectory      = -1;
    oldDirBlock.entries[oldEntryNum].first_data_block = -1;

    char dataBlock[512];
    Set_Data_Directory_Block(dataBlock, &oldDirBlock);
    if ((result = dwrite(imagefd, oldDirBlockNum, dataBlock)) < 0) 
        return result;

    return 0;
}

static struct fuse_operations xmp_oper = {
    .init    = xmp_mount,
    .destroy = xmp_unmount,
    .getattr = xmp_getattr,
    .mkdir	 = xmp_mkdir,
    .readdir = xmp_readdir,
    .mknod	 = xmp_create,
    .read	 = xmp_read,
    .write	 = xmp_write,
    .unlink	 = xmp_delete,
    .rename	 = xmp_rename,
};

int main(int argc, char *argv[])
{
    umask(0);
    benchmark_start(BENCHMARK_FILE);

    int result = fuse_main(argc, argv, &xmp_oper);
    
    benchmark_end();
    
    return result;
}

/***************** DO NOT MODIFY THE FUNCTIONS BELOW ***************/
/*
 * before calling these function you need to open the file that is being
 * used for disk (DISKFILE) and pass the file discriptor to these functions
 */

/* Read a block from disk */
int dread(int fd, int blocknum, char *buf) {
    benchmark_read(blocknum);
    
    if (lseek(fd, blocknum*BLOCKSIZE, SEEK_SET) < 0) {
        perror("dread-seek: ");
        return -errno;
    }

    if (read(fd, buf, BLOCKSIZE) != BLOCKSIZE) {
        perror("dread: ");
        return -errno;
    }

    return BLOCKSIZE;
}

/* Write a block to disk */
int dwrite(int fd, int blocknum, const char *buf)
{
    benchmark_write(blocknum);
    
    if (lseek(fd, blocknum*BLOCKSIZE, SEEK_SET) < 0) {
        perror("dwrite-seek: ");
        return -errno;
    }

    if (write(fd, buf, BLOCKSIZE) != BLOCKSIZE) {
        perror("dwrite: ");
        return -errno;
    }

    return BLOCKSIZE;
}
