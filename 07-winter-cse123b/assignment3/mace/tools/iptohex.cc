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
    printf("iptohex dotted_quad\n");
    printf("e.g.: iptohex 127.0.0.1\n");
    exit(5);
  }

  struct in_addr in;
  inet_aton( argv[1], &in );
  printf("%.8x\n", in.s_addr);
  return 0;
}


