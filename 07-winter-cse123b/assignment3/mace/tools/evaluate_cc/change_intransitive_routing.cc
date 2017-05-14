#define  USE_NS_ROUTING_TABLE 0
#define  USE_MODELNET 1
#include "evaluate.cc"
// #include "enumerate.cc"
#include <math.h>

int main(int argc, char** argv)
{ 
  if (argc < 11)
    {
      printf("argc: %d\n\n usage number_participants total  initial   spacingminimum  spacing_maximum  length_minimum  length_maximum  percentage_minimum   percentage_maximum  magnitude_minimum magnitude_maximum \n", argc);
      exit( 1);
    }
      int number_participants=atoi(argv[1]); 
      int initial=atoi(argv[2]); 
      int total=atoi(argv[3]); 
      int spacing_minimum=atoi(argv[4]); 
      int spacing_maximum=atoi(argv[5]); 
      int length_minimum=atoi(argv[6]); 
      int length_maximum=atoi(argv[7]); 
      double percentage_minimum =atof(argv[8]);
      double percentage_maximum =atof(argv[9]);
      double magnitude_minimum=atof(argv[10]); 
      double magnitude_maximum=atof(argv[11]); 
//    read_modelnet_network(1);
//    change_physical_bandwidth_partition(total, initial, spacing,percentage, magnitude);
  change_intransitive_routing( number_participants,initial, total, spacing_minimum, spacing_maximum, length_minimum, length_maximum, percentage_minimum, percentage_maximum,  magnitude_minimum, magnitude_maximum);
}
