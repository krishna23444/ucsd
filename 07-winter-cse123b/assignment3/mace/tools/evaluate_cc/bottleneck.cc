#define  USE_NS_ROUTING_TABLE 0
#define  USE_MODELNET 1

//#define FAST_RANDOMIZATION

#include "evaluate.cc"
// #include "enumerate.cc"
#include <math.h>

int main(int argc, char** argv)
{ 

  read_modelnet_network( );
  
  int degree = 10;
  int height = 10*ceil(log( participants.size())/log(10));
  if (argc > 1)
    {
      height=atoi(argv[1]); 
    } 
  if (argc > 2)
    {
      degree=atoi(argv[2]); 
    }   

  printf("calculating bottleneck: with degree %d and height %d \n", degree, height);

  double bottleneck = bottleneck_tree(degree, height);
  printf("bottleneck: %f\n", bottleneck);

}
