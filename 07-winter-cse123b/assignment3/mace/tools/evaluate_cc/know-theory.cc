
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
/* #include "trace.h" */
/* #include "Random.h" */
#include <math.h>

#define NODES 1000
#define EPOCHS 400
#define CSSIZE 25

int learned[NODES];
int thisepoch[CSSIZE];
int wegot[NODES][EPOCHS];


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
  int i;
  int j;
  int k;
  int z;
  int randnode;
  
  for (j=0; j<NODES; j++)  // for each node
    {
      // first cleared learned list
      for (i=0; i<NODES; i++)
	learned[i]=0; 
      for (i=0; i<EPOCHS; i++)  // simulated number of epochs
	{  
	  for (k=0; k<CSSIZE; k++)  // simulate receiving CSSIZE random nodes
	    {
	      int foundnewnode=0;
	      thisepoch[k] = 0;
	      while (!foundnewnode) {
		randnode = random_integer(NODES); 
		foundnewnode = 1;
		for (z=0; z<k; z++)
		  {
		    if (thisepoch[z] == randnode)
		      foundnewnode = 0; 
		  }
	      }
	      thisepoch[k] = randnode;
	      learned[randnode] = 1;	      
	    }
	  wegot[j][i] = 0;
	  for (k=0; k<NODES; k++)  // count how many we have so far
	    if (learned[k])
	      wegot[j][i]++;
	  //	  if (i==0)
	  //  printf("%d %d\n", j, wegot[j][i]);

	}
    }

  for (i=0; i<EPOCHS; i++)  // simulated number of epochs
    {
      int cumulative=0;
      int cumulativesquare=0;
      for (j=0; j<NODES; j++)  // for each node
	{
	  cumulative+=wegot[j][i];
	  cumulativesquare+=wegot[j][i]*wegot[j][i];
	}
      double variance =  (cumulativesquare-((double)cumulative*(double)cumulative/(double)NODES))/(double)(NODES-1);
      double deviation = sqrt( variance);
      double average = (double)cumulative/(double)NODES;
      double up=average+deviation;
      if (up > NODES)
	up = (double)NODES;
      double down=average-deviation;
      if (down < 0)
	down = 0;
      
      if (i%10 == 0)
	printf("THEORETICAL %d %lf %lf %lf\n", i, average, up, down);
    }
}
