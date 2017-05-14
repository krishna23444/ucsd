#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main (int argc, char** argv)
{
  if (argc != 2) {
    printf("Usage\n");
    printf("hextoip ip_in_hex\n");
    exit(5);
  }
  struct in_addr in;
  sscanf(argv[1], "%x", &in.s_addr);
  printf("%.8x %s\n", in.s_addr, inet_ntoa(in));
  return 0;
}


