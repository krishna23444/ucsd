
#include <stdlib.h>
#include <stdio.h>

//#define SANITY

FILE* open(const char *name)
{
    FILE *f = fopen(name,"r");
    if (f) return f;
    fprintf(stderr,"Error opening file: %s\n",name);
    exit(1);
}

#define findit(i, arr, len, what) \
for (int k=0; k<len; k++) \
    if (arr[k].what == what) \
	i = k;

const int MAX_NODES = 30;
const int MAX_LEAFSET = 16;
const int BITS = 32;
const int B = 4;
const int ROWS = BITS/B;
const int COLS = 1<<B;
const int MASK = COLS-1;

struct node {
    int addr;
    int id;
    node() : addr(0), id(0) {}
};

int nnodes = 0;
int nleaves = 0;
int me = -1;
node nodes[MAX_NODES];
node leafset[MAX_LEAFSET];
node table[ROWS][COLS];

void load_nodes(FILE *f)
{
    char name[16]; // not used
    int addr, id, i=0;

    while ( fscanf(f,"%x %x %s",&id,&addr,name) == 3 )
    {
	nodes[nnodes].id = id;
	nodes[nnodes].addr = addr;
#ifdef SANITY
	printf("%.8x/%.8x\n",id,addr);
#endif
	if (++nnodes > MAX_NODES) {
	    fprintf(stderr,"Too many nodes mate.\n");
	    exit(4);
	}
    }
#ifdef SANITY
    printf("nnodes = %d\n",nnodes);
#endif   
}

void load_leafset(FILE *f)
{
    int id, addr; // not used
    while ( fscanf(f,"%x [%d]:",&addr,&nleaves) == 2 )
    {
	if (me == -1)
	{
	    //findit(me,nodes,nnodes,addr);
	    for (int i=0; i<nnodes; i++)
		if (nodes[i].addr == addr)
		    me = i;

#ifdef SANITY
	    printf("me = %d %.8x/%.8x\n",me,nodes[me].addr,nodes[me].id);	    
#endif 
	}
	
	if (nleaves > MAX_LEAFSET)
	{
	    fprintf(stderr,"Too many leaves mate.\n");
	    exit(5);
	}
	for (int i=0; i<nleaves; i++)
	{
	    if ( fscanf(f," %x",&id) == 1 )
	    {
		leafset[i].id = id;
#ifdef SANITY
		printf("leaf[%d] =\t%.8x\n",i,id);
#endif
	    }
	    else
		exit(3);
	}
    }
#ifdef SANITY
    printf("nleaves = %d\n", nleaves);
#endif
}

void load_table(FILE *f)
{
    int addr; // not used
    int row,col,id;
    if ( fscanf(f,"%x",&addr) == 1 )
    {
#ifdef SANITY
	printf("Good table format, addr = %.8x.\n",addr);
#endif
	while ( fscanf(f," mytable[%d][%d] %x",&row,&col,&id) == 3 ) 
	{  
	    if ( 0<=row && row<ROWS && 0<=col && col<COLS )
	    { 
		table[row][col].id = id;
#ifdef SANITY
		printf("table[%d][%d].id = %.8x\n", row, col, id);
#endif 
	    }
	    else
	    {
		fprintf(stderr,"Bad row/col indices: row = %d, col = %d\n",row,col);
		exit(8);
	    } 
	}
    }
}

int shared_prefix_length(int a, int b)
{
    unsigned int c = a ^ b;
    int shift = BITS, count = 0;

    for (int i=0; i<BITS/B; i++)
    {
	shift -= B;
	if ( c>>shift == 0 )
	    count++;
	else
	    break;
    }
    return count;
}


// indexed from n=0,1,..., left to right
int nth_digit(int id, int n)
{
    int shift = BITS - (n+1)*B;
    return (id>>shift) & MASK;
}


bool is_real(int id)
{
    for (int i=0; i<nnodes; i++)
	if (nodes[i].id == id)
	    return true;
    return false;
}

bool in_leafset(int id)
{
    for (int i=0; i<nleaves; i++)
	if (leafset[i].id == id)
	    return true;
    return false;
}

void check_leafset()
{
    int id, half = MAX_LEAFSET/2;
    int start = (me-half+nnodes)%nnodes;
    int finish = (me+half+1)%nnodes;
    
    int right=0,wrong=0,missing=0,bogus=0;
    
    for (int i=start; i != finish; i=(i+1)%nnodes)
    {
	if (i!=me) 
	{
	    if (in_leafset(nodes[i].id))
		right++;
	    else
		missing++;
	}
    }
    for (int i=finish; i != start; i=(i+1)%nnodes)
    {
	if (in_leafset(nodes[i].id))
	    wrong++;
    }
    for (int i=0; i<nleaves; i++)
    {
	if (!is_real(leafset[i].id))
	    bogus++;	
    }

    printf("Leafs (%d/%d) %d/%d/%d/%d ", MAX_LEAFSET, nleaves, right, missing, wrong, bogus);
    if (right+wrong != nleaves)	printf("-");
}

bool someone_satisfies(int r,int c)
{
    int id,l,Dl,Dr;
    for (int i=0; i<nnodes; i++)
    {
	id = nodes[i].id;
	if (id == nodes[me].id)
	{
	    Dr = nth_digit(id,r);
	    if (c==Dr) return true;
	}
	else
	{
	    l = shared_prefix_length(id, nodes[me].id);
	    Dl = nth_digit(id,l);
	    if (r==l && c==Dl) return true;		
	}
    }
    return false;
}

void check_table()
{    
    int id,l,Dl,Dr;
    int right=0, wrong=0, bogus=0, ewrong=0, eright=0;
    for (int r=0; r<ROWS; r++)
	for (int c=0; c<COLS; c++)
	{
	    id = table[r][c].id;
	    if (is_real(id))
	    {
		if (id == nodes[me].id) // me
		{
		    Dr = nth_digit(id,r);
		    if (c==Dr)	right++;
		    else	wrong++;
		}
		else // someone else
		{
		    l = shared_prefix_length(id,nodes[me].id);
		    Dl = nth_digit(id,l);		    
		    if (r==l && c==Dl)	right++;
		    else		wrong++;
		}
	    }
	    else // empty
	    {
		if (someone_satisfies(r,c)) ewrong++;
		else			    eright++;

		if (id != 0) bogus++;
	    }
	}

    printf("Table (%d) %d/%d/%d/%d/%d ",ROWS*COLS, right,wrong,eright,ewrong,bogus);
    if (right+wrong+eright+ewrong != ROWS*COLS)	printf("-");
}

int main(int argc, char **argv)
{
    if (argc != 4)
    {
	fprintf(stderr,"usage: %s <nodes> <leafset> <table>\n",argv[0]);
	fprintf(stderr,"note: <nodes> must be sorted by node id (hash value).\n");
	exit(11);
    }
    
    FILE *fnodes = open(argv[1]);
    FILE *fleafs = open(argv[2]);
    FILE *ftable = open(argv[3]);

    load_nodes(fnodes);
    load_leafset(fleafs);
    load_table(ftable);    
    
    check_table();    printf("\t");
    check_leafset();  printf("\n");
    
    return 0;
}
