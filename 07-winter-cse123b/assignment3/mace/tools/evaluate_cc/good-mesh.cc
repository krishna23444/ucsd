#define  USE_NS_ROUTING_TABLE 0
#define  USE_MODELNET 1

//#define FAST_RANDOMIZATION

#include "evaluate.cc"
// #include "enumerate.cc"
#include <math.h>

int main(int argc, char** argv)
{ 

  read_modelnet_network( );
  int strategy = 0;
  int algorithm = 0;
  if (argc > 1) strategy = atoi(argv[1]);
  if (argc > 2) algorithm = atoi(argv[2]);
  if (algorithm == 0)
    {
      best_mesh(strategy);
    }
  else if (algorithm == 1)
    {
      good_mesh(strategy);
      
   }
  else if (algorithm == 2)
    {
      good_color_mesh(strategy);
      
   }
}
