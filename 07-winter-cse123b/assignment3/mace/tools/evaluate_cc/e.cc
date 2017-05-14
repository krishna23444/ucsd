#define  USE_NS_ROUTING_TABLE 0
#include "evaluate.cc"
// #include "enumerate.cc"
#include <math.h>

int main(int argc, char** argv)
{ 
  if (argc < 2)
    {
      printf("\nError in args.\n");
      exit(4);
    }
  
  read_network(argv[1]);
  readnetwork = true;
//        printf("\ndumping network1.\n");

  dump_network1(NULL);
//        printf("\ndumping network2.\n");
  dump_network2(NULL);
//          printf("\n.\n");

  //  dump_network1(NULL);

}
