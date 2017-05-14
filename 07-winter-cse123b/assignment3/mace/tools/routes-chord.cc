/*

Usage:  routes-chord node-list route-list


*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define NODES 10000
#define BITS 32

typedef struct node_entry {
  unsigned int addr;
  unsigned int start[BITS];   // start value for the route entry
  unsigned int correct_routes[BITS];
  unsigned int test_routes[BITS];
} entry;

entry nodes[NODES];
int num_nodes=0;
FILE *node_list;
FILE *route_list;
FILE *legend;

void read_nodes(FILE *file)
{
  unsigned int node;
  unsigned int notused;
  char notused2[100];

  while (fscanf(file, "%x %x %s\n", &node, &notused, notused2)!=EOF)
    {
      nodes[num_nodes].addr = node;
      for (int i=0; i<BITS; i++)
	{
	  nodes[num_nodes].start[i] = node + (int)pow(2, i);
	  nodes[num_nodes].correct_routes[i] = 0;
	  nodes[num_nodes].test_routes[i] = 0;
	}
      num_nodes++;
      if (num_nodes > NODES) {
	printf("Too many nodes mate %d.\n", num_nodes);
	exit(6);
      }
    }
}


void read_routes(FILE *file)
{
  unsigned int mynode;
  unsigned int mystart;
  unsigned int myroute;

  if (!file)
    return;
  while (fscanf(file, "%x %x %x\n", &mynode, &mystart, &myroute)!=EOF)
    {      
      int i, j;
      for (i=0; i<num_nodes; i++)
	{
	  if (nodes[i].addr == mynode)
	    break;
	}
      if (i==BITS)
	{
	  printf("I could not find the node %x in the node list you gave me\n", mynode);
	  exit(8);
	}
      for (j=0; j<BITS; j++)
	{
	  if (nodes[i].start[j] == mystart)
	    nodes[i].test_routes[j] = myroute;
	}
    }
}


int succ(int addr)
{
  int i;

  if (addr > nodes[num_nodes-1].addr)
    return nodes[0].addr;


  for (i=0; i<num_nodes; i++)
    if (addr < nodes[i].addr)
      return nodes[i].addr;
}

void check_routes()
{
  int i, j;
  char *legend_name = (char *) malloc(40);
  sprintf(legend_name, "legend.%d",num_nodes);
  legend = (FILE *)fopen(legend_name, "w");
	  
  for (i=0; i<num_nodes; i++)
    {      
      for (j=0; j<BITS; j++)
	{
	  nodes[i].correct_routes[j] = succ(nodes[i].start[j]);
	  if (legend==0) 
	    {
	      printf("Cannot open legend file %s for writing.\n", legend_name);
	      exit(9);
	    }

	  fprintf(legend, "legend %.8x %.8x %.8x\n", nodes[i].addr, nodes[i].start[j], nodes[i].correct_routes[j]);
	  if (route_list) {
	    if (nodes[i].correct_routes[j] != nodes[i].test_routes[j])
	      {
		printf("Wrong: %.8x %.8x %.8x %.8x %d\n", nodes[i].addr, nodes[i].start[j], nodes[i].correct_routes[j], nodes[i].test_routes[j], j);
	      }
	    else
	      {
		printf("Right: %.8x %.8x %.8x %.8x %d\n", nodes[i].addr, nodes[i].start[j], nodes[i].correct_routes[j], nodes[i].test_routes[j], j);
	      }
	  }
	}
    }
}

int main(int argc, char** argv)
{ 
  if (argc != 2 && argc !=3) {
    printf("Usage:  routes-chord node-list [route-list]\n");
    exit (4);
  }

  node_list = (FILE *)fopen(argv[1], "r");
  if (argc == 3)
    route_list = (FILE *)fopen(argv[2], "r");
  else
    route_list = 0;
  if (node_list==0) {
    printf("Error opening file %s.\n", argv[1]);
    exit(5);
  }

  read_nodes(node_list);
  read_routes(route_list);
  check_routes();
  return 0;
}
