#define  USE_NS_ROUTING_TABLE 0
#define  USE_MODELNET 1
#include "evaluate.cc"
// #include "enumerate.cc"
#include <math.h>

int main(int argc, char** argv)
{ 
  double links = 0.10;
  double magnitude = 0.15;
  int spacing = 25;
  int total= 1000;
  int initial= 600;
  if (argc > 1)
    {
      total=atoi(argv[1]); 
    } 
  if (argc > 2)
    {
      initial=atoi(argv[2]); 
    } 
  if (argc > 3)
    {
      spacing=atoi(argv[3]); 
    }
  if (argc > 4)
    {
      links =atof(argv[4]);
      //  	  network_name =argv[1]; 
    }
  if (argc > 5)
    {
      magnitude=atof(argv[5]); 
    }
  read_modelnet_network(1);
  change_physical_delays(total, initial, spacing,links, magnitude);
}
