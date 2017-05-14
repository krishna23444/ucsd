#include <set>
#include <iostream>
#include <time.h>
#include <math.h>
#include <sys/time.h>

//#define TRACE

int random_integer( int maximum)
{
  // returns a random int in the range {0, maximum-1}
  
  int retval;
  double value = drand48();
  value*= maximum;
  retval = (int) floor(value);
  if (retval == maximum)
    retval--;
  return retval;
}

int main(int argc, char** argv)
{ 
  int total= atoi(argv[1]);
  int degree = 10;
  if (argc == 1) {
    printf("Total number of nodes required\n");
    return 1;
  }  
  if (argc > 2) {
    degree =atoi(argv[2]);
  }
#ifdef TRACE
  printf ("total %d, degree %d\n", total, degree);
#endif
  
  int* parent = new int[total];
  int* available = new int[total];
  int* remaining = new int[total];
  int* children = new int[total];
  int count = total - 1;
  int remaining_counter = count;
  int available_counter = 0;
  int placed_counter = 1;
  for (int index = 0; index < total ;index++ ) {
    available[index] = 0;
    children[index] = 0;
    parent[index] = 0;
    if(index<(count))
      remaining[index]= index + 1;
  }
  while (count > 0) {
    int target = random_integer(count);
    int node = remaining[target];
#ifdef TRACE
    printf ("count %d target %d node %d\n", count, target, node);
#endif
    remaining[target] = remaining[count-1];
    count --;
    int found = 0;
    while (found == 0 && available_counter<=(placed_counter-1)) {
      if (children[available[available_counter]]< degree) {
	found = 1;
	break;
      }
      else {
	available_counter++;
      }
    }
    if (found == 0) {
      printf ("Exception: found: %d\n", found);
      exit( 1);
    }
    
    int chosen = available[available_counter];
#ifdef TRACE
    printf ("chosen: %d\n", chosen);
#endif
    parent[node]=chosen;
    available[placed_counter]=node;
    placed_counter ++;
    
    children[chosen]++;
  }
  for (int index = 0; index < total ;index++ )
      printf("client%d client%d\n", index, parent[index]);
}
