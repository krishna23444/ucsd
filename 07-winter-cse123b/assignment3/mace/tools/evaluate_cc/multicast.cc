#define  USE_NS_ROUTING_TABLE 0
#define  USE_MODELNET 1

//#define FAST_RANDOMIZATION

#include "evaluate.cc"
// #include "enumerate.cc"
#include <math.h>

int main(int argc, char** argv)
{ 

  read_modelnet_network( );
 
 double bottleneck = multicast_tree();
  printf(" multicast bottleneck: %f\n", bottleneck);

}
