#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_NUM (pow(2,31)-1)
#define MAX_NAME 100

#define MAX_TOPO_SIZE 10000
void genrand(int *n, int numnodes, int seed);
int randnum(int);
int initialized=0;
int toposize;

/* Initialize an array and pass it in to initstate(). */
static long state1[32] = {
  3,
  0x3a319039, 0x32d9c024, 0x9b663182, 0x5da1f342,
  0x7449e56b, 0xb3b1dbb0, 0xab5c5918, 0x946554fd,
  0x8c2e680f, 0xeb3d799f, 0xb11ee0b7, 0x2d436b86,
  0x3a672e2a, 0x1588ca88, 0xe369535d, 0x904f35f7,
  0x17158fd6, 0x6fa6f051, 0x616e6b96, 0xac94fd6c,
  0xfe3b81e0, 0xdf0a6fb5, 0xf103bc02, 0x48f330fb,
  0x36413f93, 0xc6d3c298, 0xf5a42ab8, 0x8a82d77b,
  0xf5ad9d0e, 0x8939220b, 0x27fb47b9
};

int main(int argc, char **argv)
{
  int i;
  int numnodes;
  unsigned seed;
  int n[MAX_TOPO_SIZE];
  char nsname[MAX_NAME];
  char listname[MAX_NAME];
  FILE *nsfile;
  FILE *listfile;

  if (argc!=4)
    {
      printf("\nIncorrect number of args.");
      printf("\nUsage:");
      printf("\n  genrand toposize numnodes seed\n");
      exit(3);
    }
  toposize = atoi(argv[1]);
  numnodes = atoi(argv[2]);
  seed = atoi(argv[3]);
  
  sprintf(nsname, "assign-%d-%d-%d.ns", toposize, numnodes, seed);
  sprintf(listname, "assign-%d-%d-%d.list", toposize, numnodes, seed);
  
  nsfile = (FILE *)fopen(nsname, "w");
  listfile = (FILE *)fopen(listname, "w");
  
  fprintf(nsfile, "set numnodes %d\n", numnodes);
  fprintf(nsfile, "set seed %d\n", seed);

  genrand(n, numnodes, seed);
 
  for (i=0; i<numnodes; i++)
   {
     fprintf(nsfile, "set n(%d) $realn(%d)\n",i,n[i]);
     fprintf(listfile, "%d\n", n[i]);
   }
}

void genrand(int *n, int numnodes, int seed)
{
  int i, j;
  int notgood;

  for (i=0; i<numnodes; i++)
   {
     notgood = 1;
     while(notgood)
       {
	 notgood = 0;
	 n[i] = randnum(seed);
	 for (j=0; j<i; j++)
	   if (n[i]==n[j])     // same rand number, can't have that
	     notgood = 1;
       }
   }
  
}


int randnum(int seed) 
{
  int n;
  double mynum;
  int x;

  if (!initialized)
    {
      n = 128;
      initstate(seed, (char *)state1, n);
      setstate((char *)state1);
      initialized=1;
    }
  mynum=(double)random();
  mynum=mynum/MAX_NUM;
  mynum=mynum*toposize;
  x=(int)mynum;
  return x;
}
