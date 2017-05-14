// usage:   cost-oracle 

#define  USE_NS_ROUTING_TABLE 0
#define  USE_MODELNET 1

//#define FAST_RANDOMIZATION

#include "evaluate.cc"
// #include "enumerate.cc"
#include <math.h>

int main(int argc, char** argv)
{ 
  int num;
  char *name = (char *) malloc (40);
  char *name2 = (char *) malloc (40);
  int i, j;

  char dir_name[256];
  char file_name[256];
  FILE *myfile;

  node s,v;
  if (argc != 2)
    {
      printf("\nError in args.\n");
      exit(4);
    }
  
  num = atoi(argv[1]);

  printf("about to read network, clients is %d\n", num);
  read_modelnet_network( );
  read_modelnet_hosts ( );
  printf("done reading network\n");

  char * mn_mdb = getenv("MN_MDB");
  if (!mn_mdb)
    {
      error("MN_MDB undefined");
    }

  strcpy(dir_name, mn_mdb);
  strcat(dir_name,"/oracle/");

  for (i=0; i<num; i++)
    {
      sprintf (name, "client%d", i);
      s=get_node_from_name(name);
      printf("processing node %s %d %x\n", name, get_node_number(s), IP_addresses[s]);
      strcpy(file_name, dir_name);
      strcat(file_name, name);
      myfile = (FILE *)fopen(file_name, "w");
      for (j=0; j<num; j++)
	{
	  if (i!=j) 
	    {
	      sprintf (name2, "client%d", j);
	      v=get_node_from_name(name2);
	      fprintf(myfile, "%x %x %lf\n", IP_addresses[v], IP_addresses[s], overlay_cost_distance(v,s));
	    }
	}
      fclose(myfile);
    }
  return 0;
}
