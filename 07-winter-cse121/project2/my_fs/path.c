/*
 * Author: Nitay Joffe
 * Class: CSE 121 - Modern Operating Systems
 * Project: 1 - Filesystem
 */
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include "debug.h"
#include "directory.h"
#include "file.h"
#include "inode.h"
#include "path.h"
#include "superblock.h"

const char PATH_DELIMITER_CHARACTER = '/';
const char *PATH_DELIMITER_STRING = "/";

static unsigned int path_lookup_inode_of(const char *path) {
//  dbg_s("looking up _%s_\n", path);

  if (*path != PATH_DELIMITER_CHARACTER) {
//    dbg_e("_%s_ doesn't begin with a %c\n", path,  PATH_DELIMITER_CHARACTER);
    return INVALID;
  }

  char path_buffer[strlen(path) + 1];
  strncpy(path_buffer, path, sizeof(path_buffer)/sizeof(path_buffer[0]));

  const char *last_path_item = "/";
  const char *next_path_item = strtok(path_buffer, PATH_DELIMITER_STRING);

  unsigned int inode_number = superblock.root_inode;
  struct Directory current_directory;

  while (next_path_item != NULL) {
    directory_read_inode(inode_number, &current_directory);

//    dbg("looking for _%s_ in _%s_\n", next_path_item, last_path_item);
    inode_number = directory_find_item(&current_directory, next_path_item);

    if (inode_number == INVALID) {
//      dbg_e("_%s_ not found in _%s_\n", next_path_item, last_path_item);
      return INVALID;
    }

//    dbg("_%s_ -> %u\n", next_path_item, inode_number);
    last_path_item = next_path_item;
    next_path_item = strtok(NULL, PATH_DELIMITER_STRING);
  }

//  dbg_e("_%s_ -> %u\n", path, inode_number);
  return inode_number;
}

static unsigned int path_lookup_parent_inode_of(const char *path) {
  char path_buffer[100] = "";
  strncpy(path_buffer, path, strlen(path));

  char *filename_first_character
    = strrchr(path_buffer, PATH_DELIMITER_CHARACTER) + 1;
  *filename_first_character = '\0';

  return path_lookup_inode_of(path_buffer);
}

unsigned int path_get_attributes(const char *path, struct stat *stbuf) {
  dbg_s("filling stat of _%s_\n", path);

  unsigned int inode_number = path_lookup_inode_of(path);

  if (inode_number == INVALID) {
    dbg_e("_%s_ does not exist\n", path);
    return -ENOENT;
  }

  struct Inode inode;
  inode_read(inode_number, &inode);

  inode_fill_stat(&inode, stbuf);
  
  dbg_e("filled stat of _%s_\n", path);
  return 0;
}

enum PathType { PATH_FILE, PATH_DIRECTORY };

static const char *path_type_to_string(enum PathType type) {
  switch (type) {
    case PATH_FILE:
      return "file";
    case PATH_DIRECTORY:
      return "directory";
  }
  return "";
}

static unsigned int path_create(const char *path, enum PathType type) {
  dbg_s("creating %s _%s_\n", path, path_type_to_string(type));
  
  unsigned int parent_inode = path_lookup_parent_inode_of(path);
  
  if (parent_inode == INVALID) {
    dbg_e("parent of _%s_ not found\n", path);
    return -ENOENT;
  }

  struct Directory parent_directory;
  directory_read_inode(parent_inode, &parent_directory);  

  char *name = strrchr(path, PATH_DELIMITER_CHARACTER) + 1;

  if (directory_find_item(&parent_directory, name) != INVALID) {
    dbg_e("_%s_ exists\n", path);
    return -ENOENT;
  }

  unsigned int inode_number;
  switch (type) {
    case PATH_DIRECTORY:
      inode_number = directory_create_inode();
      break;
    case PATH_FILE:
      inode_number = file_create_inode();
      break;
    default:
      dbg_e("unknown type: %u\n", type);
      return -ENOENT;
  }

  directory_add_entry(name, inode_number, &parent_directory);

  dbg_e("created %s _%s_ -> %u\n", path_type_to_string(type),path,inode_number);
  return 0;
}

unsigned int path_create_directory(const char *path) {
  return path_create(path, PATH_DIRECTORY);
}

unsigned int path_create_file(const char *path) {
  return path_create(path, PATH_FILE);
}

unsigned int path_read_directory(const char *path, void *buf,
                                 fuse_fill_dir_t filler) {
  dbg_s("reading directory _%s_\n", path);

  filler(buf, ".", NULL, 0);
  filler(buf, "..", NULL, 0);
  
  unsigned int inode_number = path_lookup_inode_of(path);
  if (inode_number == INVALID) {
    dbg("_%s_ not found\n", path);
    return -ENOENT;
  }
  
  struct Inode inode;
  inode_read(inode_number, &inode);

  struct DirectoryBlock directory_block;

  dbg("directory contains %u blocks\n", inode_size_in_blocks(&inode));

  int i;
  for (i = 0; i < inode_size_in_blocks(&inode); ++i) {
    unsigned int block_number = inode_lookup_block(&inode, i);

    dbg("reading directory block number %u -> %u\n", i, block_number);
    directory_block_read(block_number, &directory_block);

    struct Inode entry_inode;
    struct stat stbuf;

    unsigned int number_of_entries = DIRECTORY_ENTRY_COUNT_PER_BLOCK;
    if (i == inode_last_block_number(&inode))
      number_of_entries = inode_bytes_in_last_block(&inode) / DIRECTORY_ENTRY_SIZE_ON_DISK;
    
    dbg("number of entries in directory block = %u\n", number_of_entries);

    int j;
    for (j = 0; j < number_of_entries; ++j) {
      inode_read(directory_block.entries[j].inode_number, &entry_inode);
      inode_fill_stat(&inode, &stbuf);

      filler(buf, directory_block.entries[j].file_path, &stbuf, 0);
    }
  }

  dbg_e("read directory _%s_\n", path);
  return 0;
}

unsigned int path_read(const char *path, char *buf, size_t size, off_t offset) {
  dbg_s("reading %u bytes from _%s_ starting at %u\n", size, path, offset);
  
  unsigned int inode_number = path_lookup_inode_of(path);
  if (inode_number == INVALID) {
    dbg_e("file at _%s_ not found\n");
    return -ENOENT;
  }
  
  struct File file;
  file_read_inode(inode_number, &file);
  
  unsigned int byte_count = file_read(&file, size, offset, buf);
  
  dbg_e("read %u bytes from _%s_ starting at %u\n", byte_count, path, offset);
  
  return byte_count;
}

unsigned int path_write(const char *path, const char *buf, size_t size,
                        off_t offset) {
  dbg_s("writing %u bytes to _%s_ starting at %u\n", size, path, offset);
  
  unsigned int inode_number = path_lookup_inode_of(path);
  if (inode_number == INVALID) {
    dbg_e("file at _%s_ not found\n");
    return -ENOENT;
  }
  
  struct File file;
  file_read_inode(inode_number, &file);
  
  unsigned int byte_count = file_write(size, offset, buf, &file);
//  unsigned int byte_count = 0;
  
  dbg_e("wrote %u bytes to _%s_ starting at %u\n", byte_count, path, offset);
  
  return byte_count;
}

unsigned int path_delete(const char *path) {
  dbg_s("deleting _%s_\n", path);
  
  unsigned int parent_inode_number = path_lookup_parent_inode_of(path);
  if (parent_inode_number == INVALID) {
    dbg("parent of _%s_ not found\n", path);
    return -ENOENT;
  }

  struct Directory parent_directory;
  directory_read_inode(parent_inode_number, &parent_directory);

  const char *filename = strrchr(path, PATH_DELIMITER_CHARACTER) + 1;
  unsigned int inode_number = directory_delete_item(filename,&parent_directory);
  
  if (inode_number == INVALID)
    return -ENOENT;

  struct Inode inode;
  inode_read(inode_number, &inode);
  
  dbg("removing _%s_ inode %u\n", path, inode_number);
  inode_delete(&inode);

  dbg_e("deleted _%s_\n", path);
  return 0;
}

unsigned int path_rename(const char *from, const char *to) {
  dbg_s("renaming _%s_ to _%s_\n", from, to);

  unsigned int from_parent_inode = path_lookup_parent_inode_of(from);
  if (from_parent_inode == INVALID) {
    dbg_e("parent of _%s_ does not exist\n", from);
    return -ENOENT;
  }
  
  unsigned int to_parent_inode = path_lookup_parent_inode_of(to);
  if (to_parent_inode == INVALID) {
    dbg_e("parent of _%s_ does not exist\n", to);
    return -ENOENT;
  }

  const char *to_name = strrchr(to, PATH_DELIMITER_CHARACTER) + 1;

  struct Directory to_parent_directory;
  directory_read_inode(to_parent_inode, &to_parent_directory);

  if (directory_find_item(&to_parent_directory, to_name) != INVALID)
    return INVALID;

  const char *from_name = strrchr(from, PATH_DELIMITER_CHARACTER) + 1;

  struct Directory from_parent_directory;
  directory_read_inode(from_parent_inode, &from_parent_directory);

  unsigned int from_inode = directory_delete_item(from_name,
                                                  &from_parent_directory);
  
  if (from_inode == INVALID) {
    dbg_e("item _%s_ not found\n", from);
    return -ENOENT;
  }

  directory_add_entry(to_name, from_inode, &to_parent_directory);

  dbg_e("renamed _%s_ to _%s_\n", from, to);
  return 0;
}
