/*
 * CSE121 Project 2: File System
 *
 * Copyright(c) 2005 Alex C. Snoeren <snoeren@cs.ucsd.edu>
 * Calvin Hubble
 * Kiran Tati <ktati@cs.ucsd.edu>
 *
 */

#define BLOCKSIZE 512
#define DISKFILE "/home/linux/ieng6/oce/66/njoffe/inode_fs/disk.txt"
#define BENCHMARK_FILE "/home/linux/ieng6/oce/66/njoffe/inode_fs/benchmark.txt"

/* You must use the following two calls to read from your "disk" */
/* Read a block from disk */
int dread(int fd, int blocknum, char *buf);

/* Write a block to disk */
int dwrite(int fd, int blocknum, const char *buf);


/* You must implement the following calls on your filesystem */

/* Create a file to serve as the logical disk for the file system */
int myformat(const char *filename, int size);

/* Mount a previously-created filesystem */
int mymount(const char *filename);

/* Unmount a mounted file system */
int myunmount();

/* Make a directory.
 * 
 * Pathnames must be absolute, starting with '/'.
 * You may assume all directory names are at most 32 char long.
 */
int mymkdir(const char *path, const char *dirname);

/* Remove a file.  You don't have to support removing directories. */
int mydelete(const char *path);

/* Rename a file. */
int myrename(const char *oldpath, const char *newpath);

/* List the contents of a directory.  You must dynamically allocate the
   array of strings.  The caller will free them. */
char ** mygetdir(const char *path);

/* Read a buffer from a file */
int mywrite(const char *path, const void *buf, int size);

/* Write a buffer to a file */
int myread(const char *path, void *buf, int *size);

/*////////////////////////////////////////////////////////////
// API used for directories
//
// NOTE: This implementation is dependent on BLOCKSIZE being 512
////////////////////////////////////////////////////////////*/

struct Directory_Entry_struct
{
  char file_name[28];

  //Value of 1 if entry is a directory, value of 0 if not a directory
  //value of -1 if entry is empty
  int isDirectory;


  //First_data_block refers to an inode (if entry is a file)
  //or a block # (if entry is a directory
  //or value is -1 if entry is empty
  int first_data_block; 
  

};
typedef struct Directory_Entry_struct Directory_Entry;

//a directory block is "empty" if all the entries are empty
//note a directory block CAN be empty yet have a pointer to a next 
//non-empty block
struct Directory_Block_struct
{
  Directory_Entry entries[14];
  int next_directory_block;
};

typedef struct Directory_Block_struct Directory_Block;

////////////////////////////////////////////////////////////
//Directory API
////////////////////////////////////////////////////////////
char ** GetDirPathHelper(const char * path, int *depth);

int Parse_Directory_Entry(Directory_Entry * entry, int entry_num, const char *data_block);
int Parse_Directory_Block(Directory_Block * dir_block, const char * data_block);
int is_Dir_Entry_Empty(const Directory_Entry * entry);
int is_Dir_Block_Empty(const Directory_Block * block);
void Set_Empty_Dir_Block(Directory_Block * dir_block);
void Set_Data_Directory_Block(char * data_block, const Directory_Block * dir_block);
void Set_Empty_Dir_Entry(Directory_Entry * dir_entry);
int Is_Directory(const Directory_Entry * entry);

//Gets a directory entry and returns the final BLOCk that entry was located in
int Get_Directory_Entry_and_Block(int dir_start_block, const char * file_name, 
				  Directory_Entry * retrieved_entry);

int Add_Directory_Entry(int dir_block_num, const Directory_Entry * dir_entry_to_add);

int Remove_And_Clean_Dir_Entry(const Directory_Entry * entry, int directory_start_block_num);
int 
Search_Directory_List( int curr_dir_block, const char * name, int * isDirectory, int * firstDataBlockNum,
		       int * final_directory_block);
////////////////////////////////////////////////////////////
// iNode List API
////////////////////////////////////////////////////////////
//
// Each inode should be of size 16*4 = 64 bytes
// and (512/64)=8 inode entries can fit into 1 block of data.
//
// Because of the small block size, we had to create this funky
// Inode structure in order to be able to address 2^32 bytes of 
// data with one inode
//
////////////////////////////////////////////////////////////
struct inode_struct
{
  int size;
  int direct_blocks[7];
  int indirect_blocks[3];
  int double_indirect_blocks[1];
  int triple_indirect_blocks[4];
};
typedef struct inode_struct INode;

//Locates a free inode, marks it as used, and returns the inode #
int Find_And_Mark_Free_Inode();
//int Mark_Inode_Used(int inodenum); (not needed)
int Mark_Inode_Free(int inodenum);
//int Is_Inode_Free(int inodenum); (not needeD)

int Parse_Inode_From_Block(INode * node, int inode_num_offset, const char * dataBlock);


// Given a file of size 'size', Allocate_Inode_Pointers will allocate all the necessary metadata
// (direct/indirect pointers etc.) to write tehf ile to disk.  THis function assumes
// all the necessary data blocks have already been allocated and data SHOULD be written
// before calling this function.
//
// Fils in "node" with all the allocated pointers
int Allocate_Inode_Pointers(INode * node, int inode_num, unsigned int size_bytes, int * allocated_data_block_array);

int Set_Data_Block_For_Inode(char * data, const INode * node);
int Write_Inode_To_Disk(const INode * node, int inode_num);

//Sorta the inverse of Allocate_Inode_Pointers
int Get_All_Data_Blocks_For_Inode(int * data_block_array, int * metaData_array, int * metadata_counter, const INode * inode);

int Delete_And_Free_Inode(int inodenum);
//API for free blocks
int Find_And_Mark_Free_Block();
int Mark_Block_Used(int blockNum);
int Mark_Block_Free(int blockNum);

int  ReadInode(INode *node, int inodeNum);
int  WriteInode(const INode* node, int inodeNum);
int  DeleteInode(int inodenum, INode* inode);
void PrintIndirectBlock(int level, int blockNum);
int  PrintInode(INode *inode, int inodeNum);
int  namei(const char* Path, int* isDir, int* firstDataBlockNum, int checkLastComponent);
int  GetDirectoryEntry(int block, const char* file, Directory_Block* dirBlock,
                       int* fileEntryNum);

int ReadBlock(int diskBlockNum, char* buffer);
int WriteBlock(int diskBlockNum, char* buffer);
int AssignAndWriteBlock(char* buffer);
int RecursiveWrite(int level, int blockNum, char *bucket, char* block,
                   int bucketBlockNum);
int ReadLogicalBlock(INode* inode, int blockNum, char* block);
int WriteLogicalBlock(INode* inode, int blockNum, char* block, int* inodeUpdated);
int DeleteBlocks(int level, int blockNum);
