/*
 * Author: Nitay Joffe
 * Class: CSE 121 - Modern Operating Systems
 * Project: 1 - Filesystem
 */
#include <string.h>
#include <strings.h>
#include "debug.h"
#include "directory.h"
#include "file.h"
#include "inode.h"
#include "io.h"
#include "superblock.h"

static void directory_dump_block(const struct DirectoryBlock *directory_block) {
  dbg("block_number: %u\n", directory_block->block_number);
  dbg("_%s_ -> %u, _%s_ -> %u\n", directory_block->entries[0].file_path,
      directory_block->entries[0].inode_number,
      directory_block->entries[1].file_path,
      directory_block->entries[1].inode_number);
  dbg("_%s_ -> %u, _%s_ -> %u\n", directory_block->entries[2].file_path,
      directory_block->entries[2].inode_number,
      directory_block->entries[3].file_path,
      directory_block->entries[3].inode_number);
  dbg("_%s_ -> %u, _%s_ -> %u\n", directory_block->entries[4].file_path,
      directory_block->entries[4].inode_number,
      directory_block->entries[5].file_path,
      directory_block->entries[5].inode_number);
  dbg("_%s_ -> %u, _%s_ -> %u\n", directory_block->entries[6].file_path,
      directory_block->entries[6].inode_number,
      directory_block->entries[7].file_path,
      directory_block->entries[7].inode_number);
  dbg("_%s_ -> %u, _%s_ -> %u\n", directory_block->entries[8].file_path,
      directory_block->entries[8].inode_number,
      directory_block->entries[9].file_path,
      directory_block->entries[9].inode_number);
  dbg("_%s_ -> %u, _%s_ -> %u\n", directory_block->entries[10].file_path,
      directory_block->entries[10].inode_number,
      directory_block->entries[11].file_path,
      directory_block->entries[11].inode_number);
  dbg("_%s_ -> %u, _%s_ -> %u\n", directory_block->entries[12].file_path,
      directory_block->entries[12].inode_number,
      directory_block->entries[13].file_path,
      directory_block->entries[13].inode_number);
  dbg("_%s_ -> %u, _%s_ -> %u\n", directory_block->entries[14].file_path,
      directory_block->entries[14].inode_number,
      directory_block->entries[15].file_path,
      directory_block->entries[15].inode_number);
}

// TODO: this should be static
void directory_block_read(unsigned int block_number,
                          struct DirectoryBlock *directory_block) {
  dbg_s("reading from block %u\n", block_number);

  bzero(directory_block, sizeof(struct DirectoryBlock));

  unsigned char buffer[BLOCKSIZE] = "";
  unsigned char *buffer_pointer = buffer;

  kread(block_number, buffer);

  int i;
  for (i = 0; i < DIRECTORY_ENTRY_COUNT_PER_BLOCK; ++i) {
    bzero(&directory_block->entries[i].file_path,
          DIRECTORY_FILE_PATH_SIZE_IN_MEMORY);
    
    memcpy(&directory_block->entries[i], buffer_pointer,
           DIRECTORY_ENTRY_SIZE_ON_DISK);
    
    buffer_pointer += DIRECTORY_ENTRY_SIZE_ON_DISK;
  }

  directory_block->block_number = block_number;
  
  directory_dump_block(directory_block);
  dbg_e("read from block %u\n", block_number);
}

static void directory_block_write(const struct DirectoryBlock *directory_block){
  dbg_s("writing to block %u\n", directory_block->block_number);
  directory_dump_block(directory_block);

  unsigned char buffer[BLOCKSIZE] = "";
  unsigned char *buffer_pointer = buffer;

  int i;
  for (i = 0; i < DIRECTORY_ENTRY_COUNT_PER_BLOCK; ++i) {
    memcpy(buffer_pointer, &directory_block->entries[i],
           DIRECTORY_ENTRY_SIZE_ON_DISK);

    buffer_pointer += DIRECTORY_ENTRY_SIZE_ON_DISK; 
  }
  
  kwrite(directory_block->block_number, buffer);

  dbg_e("wrote to block %u\n", directory_block->block_number);
}

static void directory_block_update(unsigned int index, const char *name,
                                   unsigned int inode,
                                   struct DirectoryBlock *directory_block) {
  dbg_s("updating entry %u: _%s_ -> %u\n", index, name, inode);

  bzero(directory_block->entries[index].file_path, DIRECTORY_FILE_PATH_SIZE_IN_MEMORY);
  strncpy(directory_block->entries[index].file_path, name, strlen(name));

  directory_block->entries[index].inode_number = inode;

  directory_block_write(directory_block);

  dbg_e("updated entry %u: _%s_ -> %u\n", index, name, inode);
}

static unsigned int directory_number_of_entries(const struct Directory *directory) {
  return directory->inode.on_disk.size_in_bytes / DIRECTORY_ENTRY_SIZE_ON_DISK;
}

static unsigned int directory_last_entry_index(const struct Directory *directory) {
  unsigned int number_of_entries = directory_number_of_entries(directory);

  return (number_of_entries - 1) % DIRECTORY_ENTRY_COUNT_PER_BLOCK;
}

unsigned int directory_create_inode() {
  dbg_s("creating directory inode\n");

  struct Inode inode;
  bzero(&inode, sizeof(struct Inode));
  inode.number = allocate_inode();
  inode.on_disk.points_to_directory = 1;
  inode_write(&inode);

  dbg_e("created directory inode %u\n", inode.number);
  return inode.number;
}

static unsigned int directory_block_find_index(const struct DirectoryBlock *directory_block,
                                                const char *name,
                                                unsigned int number_of_entries) {
  int i;
  for (i = 0; i < number_of_entries; ++i) {
    const struct DirectoryEntry *entry = &directory_block->entries[i];
    if (strncmp(name, entry->file_path, PATH_MAX_LENGTH) == 0)
      return i;
  }

  return INVALID;
}

static unsigned int directory_block_find(const struct DirectoryBlock *directory_block,
                                          const char *name,
                                          unsigned int number_of_entries) {
  unsigned int item_index
    = directory_block_find_index(directory_block, name, number_of_entries);

  if (item_index == INVALID)
    return INVALID;

  return directory_block->entries[item_index].inode_number;
}

static unsigned int directory_block_number_of_entries(const struct Directory *directory,
                                                      unsigned int index) {
  unsigned int last_block_number = inode_last_block_number(&directory->inode);
  if (index > last_block_number)
    return 0;
  
  if (index < last_block_number)
    return DIRECTORY_ENTRY_COUNT_PER_BLOCK;
  
  return directory_last_entry_index(directory) + 1;
}

unsigned int directory_find_item(const struct Directory *directory,
                                 const char *name) {
  dbg_s("looking up _%s_\n", name);

  if (strlen(name) > PATH_MAX_LENGTH) {
    dbg_e("_%s_ invalid: name longer than %u\n", name, PATH_MAX_LENGTH);
    return INVALID;
  }

  int i;
  for (i = 0; i < inode_size_in_blocks(&directory->inode); ++i) {
    unsigned int block_number = inode_lookup_block(&directory->inode, i);

    struct DirectoryBlock directory_block;
    directory_block_read(block_number, &directory_block);

    unsigned int number_of_entries = directory_block_number_of_entries(directory, i);

    unsigned int inode_number = directory_block_find(&directory_block, name, number_of_entries);
    if (inode_number != INVALID) {
      dbg_e("found _%s_ in block %u at %u on disk\n", name, i, inode_number);
      return inode_number;
    }
  }

  dbg_e("_%s_ not found\n", name);
  return INVALID;
}

unsigned int directory_delete_item(const char *name,
                                   struct Directory *directory) {
  dbg_s("deleting _%s_\n", name);

  if (strlen(name) > PATH_MAX_LENGTH) {
    dbg_e("_%s_ invalid: name longer than %u\n", name, PATH_MAX_LENGTH);
    return INVALID;
  }

  dbg("looking for _%s_ in %u blocks, %u entries\n", name,
      inode_size_in_blocks(&directory->inode),
      directory_number_of_entries(directory));

  int i;
  for (i = 0; i < inode_size_in_blocks(&directory->inode); ++i) {
    unsigned int block_number = inode_lookup_block(&directory->inode, i);
    
    struct DirectoryBlock directory_block;
    directory_block_read(block_number, &directory_block);

    unsigned int number_of_entries = directory_block_number_of_entries(directory, i);

    unsigned int item_index = directory_block_find_index(&directory_block, name, number_of_entries);
    if (item_index != INVALID) {
      dbg("found _%s_ in index %u of block %u\n", name, item_index, i);
      unsigned int inode = directory_block.entries[item_index].inode_number;

      struct DirectoryBlock last_directory_block;
      unsigned int last_block_number=inode_lookup_last_block(&directory->inode);
      directory_block_read(last_block_number, &last_directory_block);

      unsigned int last_entry_index = directory_last_entry_index(directory);
      struct DirectoryEntry *last_entry
        = &last_directory_block.entries[last_entry_index];

      dbg("copying last block %u index %u to block %u index %u\n",
          last_block_number, last_entry_index, block_number, item_index);

      directory_block_update(item_index, last_entry->file_path,
                             last_entry->inode_number, &directory_block);

      inode_decrease_size(DIRECTORY_ENTRY_SIZE_ON_DISK, &directory->inode);

      return inode;
    }
  }

  dbg_e("file _%s_ not found in directory\n", name);
  return INVALID;
}

void directory_read_inode(unsigned int inode_number,
                          struct Directory *directory) {
  inode_read(inode_number, &directory->inode);
}

void directory_add_entry(const char *name, unsigned int inode,
                         struct Directory *directory) {
  dbg_s("adding _%s_ -> %u to directory inode %u\n", name, inode,
        directory->inode.number);

  if (inode_increase_size(DIRECTORY_ENTRY_SIZE_ON_DISK, &directory->inode) == INVALID) {
    dbg_e("failed to increase inode %u size\n", directory->inode.number);
    return;
  }

  struct DirectoryBlock directory_block;
  unsigned int last_block_address = inode_lookup_last_block(&directory->inode);
  directory_block_read(last_block_address, &directory_block);

  unsigned int last_entry_index = directory_last_entry_index(directory);
  directory_block_update(last_entry_index, name, inode, &directory_block);
  
  dbg_e("added _%s_ -> %u to directory inode %u\n", name, inode,
        directory->inode.number);
}
