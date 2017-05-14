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

#include <stdlib.h>
#include <fuse.h>
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <dirent.h>
#include <errno.h>
#include <sys/statfs.h>
#ifdef HAVE_SETXATTR
#include <sys/xattr.h>
#endif

#include "../benchmark/benchmark.h"

#include "FAT.h"
//When defined, will add a "/" to the end of directory entries
//useful for debugging
#define DEBUG_ADD_TRAILING_DIRECTORY_SLASH

#define FAT_ALLOCATING -3
#define FAT_EOF -2
#define FAT_RESERVED -1
#define FAT_FREE 0

//In our case will be  512/4 = 128
#define FAT_ENTRIES_PER_BLOCK (BLOCKSIZE / 4)

//File descriptor for the image
int imagefd = -1;


//block of our file system that corresponds to the first FAT entry
int _FAT_begin_block_num;

//block of file system that corresponds to last FAT entry (inclusive)
int _FAT_end_block_num;

//Block of file system that corresponds to the base directory
int _baseDir_block_num;

//Number of blocks in our file system
int _num_blocks;

int Read_FAT_Entry(int);

////////////////////////////////////////////////////////////
// DEBUG_PRINT_FAT
//
//  hack used for debugging FAT table
////////////////////////////////////////////////////////////
void DEBUG_PRINT_FAT()
{
  int i;
  printf("DEBUG: HERE IS FAT\n");
  for(i=0; i<_num_blocks; i++)
  {
    printf("%d ", Read_FAT_Entry(i));
  }
  printf("\n");
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
  dir_entry->size = -1;
  dir_entry->first_data_block = -1;
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
    dir_block->entries[i].size = -1;
    dir_block->entries[i].first_data_block = -1;
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
    *(int*)(data_block+(i*36)+28) = dir_block->entries[i].size;
    *(int*)(data_block+(i*36)+32) = dir_block->entries[i].first_data_block;
  }
  *(int*)(data_block+504) = dir_block->next_directory_block;
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
  if((entry->size == -1) && (entry->first_data_block != -1))
    return 1;
  else
    return 0;
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
  memcpy(entry->file_name, data_block+(entry_num*36), 36);
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
}

void debug_Print_Dir_Entry(Directory_Entry * entry)
{
  printf("name: %s  ; size: %d  ; firstDataBlock: %d  ;", entry->file_name, entry->size, entry->first_data_block);
  
}
void debug_Print_Dir_Block(Directory_Block * block)
{
  int i;
  printf("Printing Directory block(14 entries): \n");
  for(i=0; i<14; i++)
  {
    printf("%d ", i);
    if(block->entries[i].first_data_block == -1)
    {
      printf("unallocated\n");
    }else{
      debug_Print_Dir_Entry(&block->entries[i]);
      printf("\n");
    }
  }
}


////////////////////////////////////////////////////////////
// Read_FAT_Entry - 
//
//  Helper function that, given a block num, will return
//  the FAT entry for that block
//
//  Each FAT entry is 4 bytes long and there are 128 
//  such entries per physical block (4*128 = 512)
//
////////////////////////////////////////////////////////////
int Read_FAT_Entry(int blocknum)
{
  //Read a fat table entry for a given blcok num.
  int fat_block_num = _FAT_begin_block_num + (blocknum / 128);
  int offset_in_block = 4*(blocknum % 128);
  char block[512];
  
  dread(imagefd, fat_block_num, block);
  return *(int*)(block+offset_in_block);
}

////////////////////////////////////////////////////////////
// Write_FAT_entry
//
// Helper function that, given a block number and a 32-bit entry
// will write those 4 bytes into the FAT entry for blocknum
////////////////////////////////////////////////////////////
void Write_FAT_Entry(int blocknum, int value)
{
  int fat_block_num = _FAT_begin_block_num + (blocknum / FAT_ENTRIES_PER_BLOCK);
  int offset_in_block = 4*(blocknum % FAT_ENTRIES_PER_BLOCK);
  char block[BLOCKSIZE];

  dread(imagefd, fat_block_num, block);
  *(int*)(block+offset_in_block) = value;
  dwrite(imagefd, fat_block_num, block);
}



////////////////////////////////////////////////////////////
// Locate_And_Mark_Free_Block
//
//  Will locate a free block and allocate that block from the FAT
//  table writing "marker" for that entry in the table
////////////////////////////////////////////////////////////
int Find_And_Mark_Free_Block(int marker)
{
  int currBlockNum = _FAT_begin_block_num;
  int entry_pointer = 0;
  char block[BLOCKSIZE];
  int i;

  dread(imagefd, currBlockNum, block);
  
  i = 0;
  while(1)
  {
    //check if FS is full
    if(  ((currBlockNum-_FAT_begin_block_num)*128 + i) >= _num_blocks )
    {
      return -1;
    }

    if(  *(int*)(block+i*4) == FAT_FREE)
    {
      *(int*)(block+i*4) = marker;
      dwrite(imagefd, currBlockNum, block);
      //printf("find_and_mark_free_block: returning block %d\n", (currBlockNum*128 + i));
      //printf("retunring block %d\n", (currBlockNum-_FAT_begin_block_num)*128 + i);
      return ( (currBlockNum-_FAT_begin_block_num)*128 + i );
    }
    i++;
    
    //We have read all the entries in the current FAT block
    //Move on to the next block of FAT
    if(i == FAT_ENTRIES_PER_BLOCK)
    {
      //0 free space!
      if(currBlockNum == _FAT_end_block_num)
      {
	return -1;
      }
      currBlockNum++;
      dread(imagefd, currBlockNum, block);
      i = 0;
    }        
  }
  fprintf(stderr, "find_and_mark_free_block: shouldnt get here\n");
  return -1;
}

////////////////////////////////////////////////////////////
// Get_Directory_Entry
//
//  Addition - returns the BLOCk that the given entry 
//  can be found in!  Or -1 on error
////////////////////////////////////////////////////////////
int GetDirectoryEntry(int block, const char *file, Directory_Block* dirBlock, int* fileEntryNum)
{
    do {
        int  i;
        char blockData[512];

        dread(imagefd, block, blockData);
        Parse_Directory_Block(dirBlock, blockData);
        for(i = 0; i < 14; i++) {
	        if (is_Dir_Entry_Empty(&dirBlock->entries[i]))         continue;
            if (strcmp(dirBlock->entries[i].file_name, file) != 0) continue;

            *fileEntryNum = i;
            return block;
        }

        block = dirBlock->next_directory_block;
    } while (block != -1);

    return -1;
}

/*
    curr_block = block;
    while(!done)
    {
        if(!is_Dir_Block_Empty(dirBlock))
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
*/

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

    //update FAT to notify free'd block entry
    Write_FAT_Entry(curr_dir_block_num, FAT_FREE);

  }
}


////////////////////////////////////////////////////////////
// Add_Directory_Entry
//
// Helper function that adds a directory entry.
//
// Note, a directory entry is the tuple: <dir_name, size, firstBlock>
////////////////////////////////////////////////////////////
//int
//Add_Directory_Entry(int dir_block_num, const char * dir_name, int size, int firstBlock)
int
Add_Directory_Entry(int dir_block_num, const Directory_Entry * dir_entry_to_add)
{
  /*int i, j;
    char dirBlock[512];

    int added_dir_block_num;
    char newDirBlock[512];*/
  
  
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
    for(dir_entry_counter = 0; dir_entry_counter < 14; dir_entry_counter++) {
        fprintf(stderr, "Entry[%d] : ", dir_entry_counter);
        if( is_Dir_Entry_Empty( &dir_block.entries[dir_entry_counter] ) ) {
            fprintf(stderr, "Not Free so we are allocating it to new entry \n");
	        foundEmptyEntry = 1;
	        memcpy( &dir_block.entries[dir_entry_counter], dir_entry_to_add, sizeof(Directory_Entry));
	        foundEmptyEntry = 1;
	        break;
        }
        
        fprintf(stderr, "%s :", dir_block.entries[dir_entry_counter].file_name);
        fprintf(stderr, "Size = %d :", dir_block.entries[dir_entry_counter].size);
        fprintf(stderr, "block = %d \n", dir_block.entries[dir_entry_counter].first_data_block);
    }

    //Went through entire directory block and did not find a free entry
    if( (dir_entry_counter == 14) && (!foundEmptyEntry) )
    {
      if(dir_block.next_directory_block == -1)
      {
	previous_dir_block_num = dir_block_num;
	//allocate a new directory block
	dir_block_num = Find_And_Mark_Free_Block(FAT_RESERVED);
	if(dir_block_num == -1)
	{
	  //Could not find a free directory block to allocate
	  return -1;
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

//Search_Directory_List
// Started at a given directory block: curr_dir_block, will search for an entry for a given name
// It basically return an entry for that file - filling in size, first data block and also the 
// directory block number where entry where the entry was located (not necessarily curr_dir_block)
int 
Search_Directory_List( int curr_dir_block, const char * name, int * size, int * firstDataBlockNum,
		       int * final_directory_block)
{
  char dirBlock[512];
  int i;
  dread(imagefd, curr_dir_block, dirBlock);
  
  //14 entries of size 36 in a directory block .... the 504th, 5ht, 6th and 7th byte hold a pointer
  //to the next directory block



  i = 0;
  while(1)
  {
    //Got to an invalid entry!  no more directory blocks will exist after here
    if( *(int*)(dirBlock+(i*36)+32) == -1)
    {
      //return -1;
    }else{
      //Found the data block
      if(strcmp(name, dirBlock+(i*36)) == 0)
      {
	*size = *(int*)(dirBlock+(i*36)+28);
	*firstDataBlockNum = *(int*)(dirBlock+(i*36)+32);
	*final_directory_block = curr_dir_block;
	return 0;
      }
    }

    //Do we need to read another directory block
    if(i == 14)
    {
      curr_dir_block = *(int*)(dirBlock+(14*36));
      i = 0;
      if(curr_dir_block == -1)
      {
	return -1;
      }
      dread(imagefd, curr_dir_block, dirBlock);
    }else{
      i++;
    }
  }
  //COuld not find directory listing
  return -1;
}

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
  saved_path = (char *)malloc(strlen(path)+1);
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

/* converts the path to the inode information */
int namei(const char* Path, int* Size, int* firstDataBlockNum, int checkLastComponent) {
    int finalDir, ir;

    int size      = -1;
    int curDir    = _baseDir_block_num;
    int firstData = _baseDir_block_num; 

    char tmp[4096];
    char *path = tmp;

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

        ir = Search_Directory_List(curDir, fileName, &size, &firstData, &finalDir);
        if (ir == -1) {
            return -ENOENT;
        }

        if (lastComponent) break;

        //not a directory entry
        if (size != -1) {
            return -ENOTDIR;
        }

        path++; // move '/'
        curDir = firstData;
    }

    if (Size != NULL) *Size = size;
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

        ir = Search_Directory_List(curDir, fileName, &size, &firstData, &finalDir);
        if (ir == -1) {
            free(start);
            return -ENOENT;
        }

        if (lastComponent) break;

        //not a directory entry
        if (size != -1) {
            free(start);
            return -ENOTDIR;
        }

        path++; // move '/'
        curDir = firstData;
    }

    if (Size != NULL) *Size = size;
    if (firstDataBlockNum != NULL) *firstDataBlockNum = firstData;

    free(start);
    return 0;
*/
}

int DeallocateAll(int blockNum) {
    int next = Read_FAT_Entry(blockNum);
    int totalBlocks = 0;

    if (next != FAT_EOF) 
        totalBlocks = DeallocateAll(next);

    Write_FAT_Entry(blockNum, FAT_FREE);
    return ++totalBlocks;

    //fprintf(stderr, "Freed %d : Total Freed = %d \n", blockNum, totalBlocks);
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

    char superblock_block[512];

    if (dread(imagefd, 0, superblock_block) < 0) {
        perror("Mount:");
        fprintf(stderr, "Mount: Unable to read the disk file %s\n", DISKFILE);
        fprintf(stderr, "Mount Failed \n");
        exit(100);
    }

    _num_blocks          = *(int*)(superblock_block);
    _FAT_end_block_num   = *(int*)(superblock_block+8);
    _baseDir_block_num   = *(int*)(superblock_block+12);
    _FAT_begin_block_num = *(int*)(superblock_block+4);
  
    /* You may need to read some info from your filesystem to prepare
     to read... */

    benchmark_set_disk_size_in_blocks(_num_blocks);

    fprintf(stderr, "Mount is success\n");
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
static int xmp_getattr(const char *path, struct stat *stbuf)
{

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

    int size;
    int res = namei(path, &size, NULL, 1);
    if (res < 0) return res;

    if (size < 0) {
        stbuf->st_mode  |= S_IFDIR;
        stbuf->st_size   = 1024; 
        stbuf->st_blocks = 2;
    } else {
        stbuf->st_mode  |= S_IFREG;
        stbuf->st_size   = size; 
        stbuf->st_blocks = (int) ceil( (double) size / (double) 512 );
    }


    /* you have to implement the following fields correctly 

    if (The path represents the directory)
        stbuf->st_mode  = 0777 | S_IFDIR;
    else 
        stbuf->st_mode  = 0777 | S_IFREG;

    stbuf->st_size    =  // file size
    stbuf->st_blocks  =  // file size in blocks
    stbuf->st_blksize =  // block size of you file system

    */
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
    int size, firstDataBlock;
    int result = namei(path, &size, &firstDataBlock, 0);

    if (result < 0) return result;
    if (size != -1) return -ENOTDIR;

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
    int  freeDirBlock = Find_And_Mark_Free_Block(FAT_RESERVED);
    if (freeDirBlock == -1 ) {
        fprintf(stderr, "mkdir: Unable to find a free block to %s\n", newDirBlock);
        return -EIO;
    }

    //Write directory information to new directory block
    for(i = 0; i < 14; i++)
    {
        *(int*)(newDirBlock+(i*36+28)) = -1;
        *(int*)(newDirBlock+(i*36+32)) = -1;
        *(int*)(newDirBlock+504) = -1;
    }
    dwrite(imagefd, freeDirBlock, newDirBlock);      
	    
    Directory_Entry dir_entry;
    memcpy(dir_entry.file_name, newDirName, nameLen + 1);
    dir_entry.size = -1;
    dir_entry.first_data_block = freeDirBlock;
    if (Add_Directory_Entry(firstDataBlock, &dir_entry) < 0) {
        fprintf(stderr, "mkdir: Unable to add (%s) new dir\n", newDirName);
        return -EIO;
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
    int size, curBlock;
    int result = namei(path, &size, &curBlock, 1);

    if (result < 0) return result;
    if (size != -1) return -ENOTDIR;

    filler(buf, ".", NULL, 0);
    filler(buf, "..", NULL, 0);

    char dirBlockData[512];
    Directory_Block dirBlock;
    while(1) {
        dread(imagefd, curBlock, dirBlockData);
        Parse_Directory_Block(&dirBlock, dirBlockData);
 
        int i;
        for(i = 0; i < 14; i++) {
            // Is the entry empty?
            if (dirBlock.entries[i].first_data_block != -1) {
                struct stat st;
                memset(&st, 0, sizeof(st));
                st.st_mode  = 0777;
                st.st_mode |= (dirBlock.entries[i].size == -1) ? S_IFDIR : S_IFREG;
                if (filler(buf, dirBlock.entries[i].file_name,  &st, 0))
                    break;
            } 
        }

        curBlock = dirBlock.next_directory_block;
        if (dirBlock.next_directory_block == -1) break;
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
    int size, firstDataBlock;
    int result = namei(path, &size, &firstDataBlock, 0);

    if (result < 0) return result;
    if (size != -1) return -ENOTDIR;

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
    int newFileFirstDataBlock = Find_And_Mark_Free_Block(FAT_EOF);
    if (newFileFirstDataBlock < 0) {
        fprintf(stderr, "Create: Unable to allocate a new block to %s", newFileName);
        return -EIO;
    }
    
    Directory_Entry dir_entry;
    memcpy(dir_entry.file_name, newFileName, nameLen + 1);
    dir_entry.size = 0;
    dir_entry.first_data_block = newFileFirstDataBlock;
    if (Add_Directory_Entry(firstDataBlock, &dir_entry) < 0) {
        fprintf(stderr, "Create: Unable to add (%s) new entry\n", newFileName);
        return -EIO;
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
static int xmp_read(const char *path, char *buf, size_t bufSize, off_t offset,
                    struct fuse_file_info *fi)
{
    int fileSize, curBlock;
    int result = namei(path, &fileSize, &curBlock, 1);

    if (result < 0)     return result;
    if (fileSize == -1) return -EISDIR;

    int firstBlock = offset / 512;
    int lastBlock  = offset + bufSize;

    if (lastBlock > fileSize) // We are going beyond the file size
        lastBlock = fileSize;

    lastBlock = (int) ceil( lastBlock / 512.0);

    //Lets skip through the FAT to the first block; cache would be very helpful here
    int skippedBlocks;
    for(skippedBlocks = 0; skippedBlocks < firstBlock; skippedBlocks++) {
        if (curBlock < 0) {
            fprintf(stderr, "Read: We got a invalid block no \n");
            fprintf(stderr, "Reading from %d offset of size %d\n", offset, bufSize);
            fprintf(stderr, "FirstBlock %d : lastBlock %d : skippedBlocks %d\n",
                    firstBlock, lastBlock, skippedBlocks);

            return -EIO; // we made a mistake
        }
        curBlock = Read_FAT_Entry(curBlock);
    }

    int  left;
    int  redBytes  = 0;
    int  curOffset = offset;
    char block[512];
    while (firstBlock++ < lastBlock) {

        int ret = dread(imagefd, curBlock, block);
        if (ret < 0) return ret;

        offset = curOffset % 512;
        left   = 512 - offset;

        if ((curOffset + left) >  fileSize)
            left = fileSize - curOffset;
            
        memcpy(buf, block + offset, left);

        buf       += left;
        bufSize   -= left;
        redBytes  += left;
        curOffset += left;

        if (curOffset == fileSize) 
            return redBytes;
        
        curBlock = Read_FAT_Entry(curBlock);

        if (curBlock < 0) {
            fprintf(stderr, "Read: We got a invalid block no \n");
            fprintf(stderr, "Reading from %d offset of size %d\n", offset, bufSize);
            fprintf(stderr, "FirstBlock %d : lastBlock %d \n", firstBlock, lastBlock);
            fprintf(stderr, "redBytes %d : curOffset %d \n", redBytes, curOffset);

            return -EIO; // we made a mistake
        }
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
static int xmp_write(const char *path, const char *buf, size_t bufSize,
                     off_t offset, struct fuse_file_info *fi)
{
    int  dirSize, dirBlockNum;
    int  result = namei(path, &dirSize, &dirBlockNum, 0);
    char fileName[28];

    if (result < 0)    return result;
    if (dirSize != -1) return -ENOTDIR;

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
    if (dirEntryBlockNum < 0) return -ENOENT;
    if ((entryNum < 0) || (entryNum > 14)) {
        fprintf(stderr, "Look like we messed up GetDirectoryEntry function ");
        fprintf(stderr, " Entry Num (%d) should > 0 and < 14", entryNum);
        exit(1);
    }

    int fileSize   = dirBlock.entries[entryNum].size;
    int curBlock   = dirBlock.entries[entryNum].first_data_block;
    if (curBlock < 0) return -EISDIR;

    int firstBlock = (offset < fileSize) ? offset : fileSize;
    int lastBlock  = offset + bufSize;

    firstBlock = firstBlock / 512;
    lastBlock  = (int) ceil(lastBlock / 512.0);

    // oppurtunity to improve the FS performace using cache
    int skippedBlocks;
    for(skippedBlocks = 0; skippedBlocks < firstBlock; skippedBlocks++) {
        if (curBlock < 0) {
            fprintf(stderr, "Write: We got a invalid block no \n");
            fprintf(stderr, "Reading from %d offset of size %d\n", offset, bufSize);
            fprintf(stderr, "FirstBlock %d : lastBlock %d : skippedBlocks \n",
                    firstBlock, lastBlock, skippedBlocks);

            return -EIO; // we made a mistake
        }

        curBlock  = Read_FAT_Entry(curBlock);
    }

    int blocks = lastBlock - firstBlock;
    if (offset <= fileSize) {
        result = OverWrite(buf, bufSize, offset, curBlock, blocks, fileSize);
    } else {
        result = WriteWithZeros(buf, bufSize, offset, curBlock, blocks, fileSize);
    }

    if (result < 0) return result;
    if ((offset + bufSize) > fileSize) {
        char dataBlock[512];

        dirBlock.entries[entryNum].size = offset + bufSize;
        Set_Data_Directory_Block(dataBlock, &dirBlock);
        if ((result = dwrite(imagefd, dirEntryBlockNum, dataBlock)) < 0) 
            return result;
    }
    return bufSize;
}

int OverWrite(const char* buf, int bufSize, int offset, int curBlock, int blocks, int fileSize) 
{
    int  curOffset = offset;
    int  allocated = ((fileSize / 512) * 512) + 512;
    char block[512];

    while (blocks--) {

        // do we need to read the old block or complitly overwritting?
        if ((curOffset % 512) == 0) {

            int   bufOffset = curOffset - offset;
            int   remaining = bufSize - bufOffset;
            char* buffer = block;

            if (remaining >= 512) {

                buffer    = (char *)buf + bufOffset; // we should copy, if compiler complains
                remaining = 512;

            } else if (curOffset >= fileSize) {

                memset(block, 0, 512);
                memcpy(block, buf + bufOffset, remaining);

            } else {
                // we have to read this cur block
                memset(block, 0, 512);

                if (dread(imagefd, curBlock, block) < 0) 
                    return -EIO;

                memcpy(block, buf + bufOffset, remaining);
            }

            if (dwrite(imagefd, curBlock, buffer) < 0)
                return -EIO;

            curOffset += remaining;

        } else {
            if (curOffset != offset) {
                fprintf(stderr, "OverWrite: current offset (%d) is not ", curOffset);
                fprintf(stderr, "equal to offset \n", offset);
                exit(3);
            }

            memset(block, 0, 512);

            if (dread(imagefd, curBlock, block) < 0)
                return -EIO;

            int skip      = curOffset % 512;
            int remaining = 512 - skip;

            if (remaining > bufSize) 
                remaining = bufSize;

            memcpy(block + skip, buf, remaining);

            if (dwrite(imagefd, curBlock, block) < 0) 
                return -EIO;

            curOffset += remaining;
        }

        // should we allocate new block?
        if (curOffset >= allocated)  {

            int newBlock = Find_And_Mark_Free_Block(FAT_EOF);
            if (newBlock < 0) 
                return -EIO;
   
            Write_FAT_Entry(curBlock, newBlock);

            allocated += 512;    
            curBlock   = newBlock;

        } else {
            curBlock  = Read_FAT_Entry(curBlock);
            if ((curBlock < 0)  && (curOffset != (offset + bufSize))) {
                fprintf(stderr, "OverWrite: We got a invalid block no \n");
                fprintf(stderr, "Writing from %d offset of size %d\n", offset, bufSize);
                exit(2);
            }
        }
    }
    return bufSize;
}

int WriteWithZeros(const char* buf, int bufSize, int offset, int curBlock, int blocks, int fileSize) 
{
    int  result;
    int  curOffset = fileSize;
    int  allocated = ((fileSize / 512) * 512) + 512;
    char block[512];

    while (blocks--) {
        int  newDataSize;
        int  startOffset;

        // Do we need to read the last block?
        if ((curOffset % 512) != 0) {
            memset(block, 0, 512);
            if ((result = dread(imagefd, curBlock, block)) < 0) return result;
            
            startOffset = 0;
            newDataSize = allocated - curOffset;
            if (allocated > offset) {
                startOffset = 512 - newDataSize;
                newDataSize = allocated - offset;

                if (newDataSize > bufSize) newDataSize = bufSize;
                memcpy(block + startOffset, buf, newDataSize);
            }

            if ((result = dwrite(imagefd, curBlock, block)) < 0) return result;
            curOffset += (newDataSize + startOffset);
        } else if (curOffset < offset) {
            startOffset = 0;
            newDataSize = 512;
            memset(block, 0, 512);
            if (allocated > offset) {
                newDataSize = allocated - offset;
                startOffset = 512 - newDataSize;
        
                if (newDataSize > bufSize) newDataSize = bufSize;
                memcpy(block + startOffset, buf, newDataSize);
            }

            if ((result = dwrite(imagefd, curBlock, block)) < 0) return result;
            curOffset += (startOffset + newDataSize);
        } else {
            startOffset = curOffset - offset;
            newDataSize = 512;

            if (allocated > (offset + bufSize)) 
                newDataSize = offset + bufSize - curOffset;
        
            memset(block, 0, 512);
            memcpy(block, buf + startOffset, newDataSize);
            if ((result = dwrite(imagefd, curBlock, block)) < 0) return result;
            curOffset += newDataSize;
        }

        if (curOffset >= allocated)  {
            int newBlock = Find_And_Mark_Free_Block(FAT_EOF);
            if (newBlock < 0) return -EIO;
   
            Write_FAT_Entry(curBlock, newBlock);
            allocated += 512;    
            curBlock = newBlock;
        }
    }

    return bufSize;
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
    int  dirSize, dirBlockNum;
    int  result = namei(path, &dirSize, &dirBlockNum, 0);
    char fileName[28];

    if (result < 0)    return result;
    if (dirSize != -1) return -ENOTDIR;

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
    if (dirEntryBlockNum < 0) return -ENOENT;
    if ((entryNum < 0) || (entryNum > 14)) {
        fprintf(stderr, "Look like we messed up GetDirectoryEntry function ");
        fprintf(stderr, " Entry Num (%d) should > 0 and < 14", entryNum);
        exit(1);
    }

    int fileSize   = dirBlock.entries[entryNum].size;
    int firstBlock = dirBlock.entries[entryNum].first_data_block;
    if (firstBlock < 0) return -EISDIR;

    dirBlock.entries[entryNum].size             = -1;
    dirBlock.entries[entryNum].first_data_block = -1;

    char dataBlock[512];
    Set_Data_Directory_Block(dataBlock, &dirBlock);
    if ((result = dwrite(imagefd, dirEntryBlockNum, dataBlock)) < 0) 
        return result;

    // Now we need to clean the FAT entries;
    int allocated   = (int)ceil(fileSize / 512.0) + 1; // We allocate an extra block
    int freedBlocks = DeallocateAll(firstBlock);
    if (allocated != freedBlocks) {
        fprintf(stderr, "Allocated(%d) didn't matched with freed Blocks(%d)\n",
                        allocated, freedBlocks);
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
    int  dirSize, dirBlockNum;
    int  result = namei(from, &dirSize, &dirBlockNum, 0);
    char oldFileName[28];

    if (result < 0)    return result;
    if (dirSize != -1) return -ENOTDIR;

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
    int oldDirBlockNum = GetDirectoryEntry(dirBlockNum, oldFileName, &oldDirBlock,
                                            &oldEntryNum);
    if (oldDirBlockNum < 0) return -ENOENT;
    if ((oldEntryNum < 0) || (oldEntryNum > 14)) {
        fprintf(stderr, "Look like we messed up GetDirectoryEntry function ");
        fprintf(stderr, " Entry Num (%d) should > 0 and < 14", oldEntryNum);
        exit(1);
    }

    int firstBlock = oldDirBlock.entries[oldEntryNum].first_data_block;
    if (firstBlock < 0) return -EISDIR;

    // Let first create new entry
    int i, j, k;
    int size, firstDataBlock;

    result = namei(to, &size, &firstDataBlock, 0);
    if (result < 0) return result;
    if (size != -1) return -ENOTDIR;

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
    if (firstDataBlock == dirBlockNum) {
        char dataBlock[512];

        strcpy(oldDirBlock.entries[oldEntryNum].file_name, newFileName);
        Set_Data_Directory_Block(dataBlock, &oldDirBlock);

        if ((result = dwrite(imagefd, oldDirBlockNum, dataBlock)) < 0) 
            return result;

        return 0;
    }

    Directory_Entry newEntry;

    newEntry.size             = oldDirBlock.entries[oldEntryNum].size;
    newEntry.first_data_block = oldDirBlock.entries[oldEntryNum].first_data_block;
    strcpy(newEntry.file_name, newFileName);

    if (Add_Directory_Entry(firstDataBlock, &newEntry) < 0) {
        fprintf(stderr, "Unable to add new entry(%s) to the dir (%s)\n", newFileName, to);
        return -EIO;
    }

    //Now lets delete the old entry
    oldDirBlock.entries[oldEntryNum].size             = -1;
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
