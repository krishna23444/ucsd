#define  USE_NS_ROUTING_TABLE 0
#define  USE_MODELNET 1
#include "evaluate.cc"
// #include "enumerate.cc"
#include <math.h>

int main(int argc, char** argv)
{ 
  if (argc < 10)
    {
      printf("argc: %d\n\n usage total  initial   spacingminimum  spacing_maximum  length_minimum  length_maximum  percentage_minimum   percentage_maximum  magnitude_minimum magnitude_maximum \n", argc);
      exit( 1);
    }
      int initial=atoi(argv[1]); 
      int total=atoi(argv[2]); 
      int spacing_minimum=atoi(argv[3]); 
      int spacing_maximum=atoi(argv[4]); 
      int length_minimum=atoi(argv[5]); 
      int length_maximum=atoi(argv[6]); 
      double percentage_minimum =atof(argv[7]);
      double percentage_maximum =atof(argv[8]);
      double magnitude_minimum=atof(argv[9]); 
      double magnitude_maximum=atof(argv[10]);
      int  file_size_in_bytes = 5*1024*1024;
      BANDWIDTH_CHANGE type= BANDWIDTH_CHANGE_pulse;
      if (argc>11)
	{
	  type=(BANDWIDTH_CHANGE)atoi(argv[11]);
	}
      if (argc>12)
	{
	  file_size_in_bytes = atoi(argv[12])*1024*1024;
	}

  read_modelnet_network(1);
//    change_physical_bandwidth_partition(total, initial, spacing,percentage, magnitude);
  change_physical_edge_bandwidth_generalized(initial, total, spacing_minimum, spacing_maximum, length_minimum, length_maximum, percentage_minimum, percentage_maximum,  magnitude_minimum, magnitude_maximum, type, file_size_in_bytes);
}
