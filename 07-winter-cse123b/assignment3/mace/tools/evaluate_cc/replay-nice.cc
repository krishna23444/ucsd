#define  USE_NS_ROUTING_TABLE 0
#define  USE_MODELNET 1
#define  NICE_VALIDATION 1

//#define FAST_RANDOMIZATION

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

  if (argc >= 3 && !strcmp(argv[2], "generic"))
    {
    replay_generic_modelnet_file(argv[1]);
    nice_validation();
    }
  else
    replay_modelnet_file(argv[1]);
  
  //   replay_file(argv[1],argv[2]);
}
