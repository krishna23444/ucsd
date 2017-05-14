#include <stdio.h>
#include <sys/stat.h>

mode_t parse_permissions(char *str);
int do_permissions(int argc, char *argv[], mode_t file_perms, mode_t dir_perms);


/*
 *
 */
int main(int argc, char *argv[]) {
  mode_t file_permissions, directory_permissions;

  switch (argc) {
    case 0:
    case 1:
    case 2:
      printf("USAGE: %s <file_perms> <dir_perms> <file> [file]...\n", argv[0]);
      return 0;
    case 3:
      file_permissions = parse_permissions(argv[1]);
    case 4:
      file_permissions = parse_permissions(argv[1]);
      directory_permissions = parse_permissions(argv[2]);
      do_permissions(argc-3, argv+3, file_permissions, directory_permissions);
    default:
  }

  return 0;
}
