#define  USE_NS_ROUTING_TABLE 0
#define  USE_MODELNET 1

//#define FAST_RANDOMIZATION

#include "evaluate.cc"
// #include "enumerate.cc"
#include <math.h>
class stressing
{
public:
  stressing(node source, node destination, int sequence)
{
  this->source = source;
  this->destination = destination;
  this->sequence = sequence;
}

  node source;
  node destination;
  int sequence;
};

void replay_stress( char* replay_file,
		    list<stressing*>&receipts)
{
  //matters start
  read_modelnet_network( );

  FILE *myfile;
  int i=0;
  char intro[80];
  char name[80];
  char ip[80];
  char rest[512];
  char command[80];
  char status[80];
  char parent[80];
  char child[80];
  char whole_line[512];
  double time_when;
  int tid;
  int initialized = 0;
  int sequence = 0;
  
  myfile = (FILE *)fopen(replay_file, "r");
  if (!myfile)
    {
      printf("nonexistent replay file  %s\n", 
	     replay_file);
      return;  
    }
  
  i=0;
//    while (fscanf(myfile, "%[^\n]s", whole_line)!=EOF)
  while (fgets(whole_line, 512, myfile)!=NULL)
    {
//        cout << whole_line << endl;
//        fgetc(myfile);whole_line[ strlen( whole_line)]='\n';
      if (strncmp (whole_line,"Node", 4))
	{
	  continue;
	}
      int successful = 0;
      if ((successful =sscanf(whole_line, "%s %s %s %lf %d %s %[^\n]s", 
		 &intro, &ip, &name,  &time_when, &tid,&command, &rest))!= 7)
	{
	  continue;
	}
//  printf("got %d, %s %s %s %f %d : %s %s\n", 
//         successful,intro,ip, name,  time_when, command, rest);
//  printf("got %f %f %f\n", 
//         time_when,starting_time_index,dump_time_index);


      if (!strcmp (command,"REPLAY_PACKET"))
	{
	  //  printf("got %s %s\n", 
	  //         command, rest);

	  if (sscanf(rest, "%s %s %d", 
		     &parent, &child, &sequence)!= 3)
	    continue;
	  stressing* packet = new stressing (get_node_from_name(parent),
					     get_node_from_name(child),
					     sequence); 
	  receipts. append(packet);
	  printf("REPLAY_PACKET %s %s %d\n", 
		 parent, child, sequence);
	}
    }
}
double compute_stress (list<edge>&physical_edges,
		       edge_array<int>&is_physical,
			  double &  overall_average,
			 list<stressing*>&receipts)
{
  map<int, edge_array<int> > stress_map;
  map<int, int >  average;
  map<int, int >   max;
  stressing*sample;
  edge_array<int> empty(G,0);
  forall(sample,receipts)
    {
      if (1)
      cout << "  packet from "  
	   <<G[sample->source]<<" -> "<<G[sample->destination]  
	   << " # " << sample-> sequence << endl;   

      if (! stress_map.defined (sample-> sequence))
	{
	  stress_map[sample-> sequence]=empty;
	}

      edge p;
      list < edge > *constituents =modelnet_overlay_matrix(sample->source, sample->destination);

      forall(p,*constituents)
	{
	  stress_map[sample-> sequence][p] ++;
		 if ( 0)  cout << "  crossing "  
		       <<edge_number[p]<<" " 
		       << stress_map[sample-> sequence][p] << endl;   
	}

    }

  edge_array<int>stress(G,0);
  edge e,o;
  double overall_cumulative=0.0;
  int  sequence=0;
  int   overall_maximum=0;
  int  countsequence=0;

  forall_defined(sequence,stress_map)
	{
countsequence ++;
	  double cumulative=0.0;
	  int count=0;
	  int maximum = 0;
	  forall(e,physical_edges)
	    {
	      if (stress_map[sequence][e])
		{
		  count+=1;
//  		  cout << " stress "  
//  		       <<G[source (e)]<<" -> "<<G[target (e)]  
//  		       << " " << e << " " << stress[e]<< endl;   
		}

	      if (stress_map[sequence][e]>maximum)
		{
		  maximum=stress_map[sequence][e];
		}
	      cumulative+=stress_map[sequence][e];
	    } 
	  double average_stress;
	  if (count>0)
	    average_stress=cumulative/count;
	  else
	    average_stress=0;
	  average[sequence]=average_stress;
	  max[sequence]=maximum;
          cout << "average_stress for " << sequence << " " <<   average_stress << endl;
	  overall_cumulative +=average_stress;
	      if (maximum>overall_maximum)
		{
		  overall_maximum=maximum;
		}
	}
  overall_average =overall_cumulative/countsequence;
  printf("average_STRESS: %f\n",  overall_average);
  printf("maximum_STRESS: %d\n",  overall_maximum);
    return overall_maximum; 
} 


int main(int argc, char** argv)
{ 
  if (argc < 2)
    {
      printf("\nError in args.\n");
      exit(4);
    }

//    read_modelnet_network( );
  
  list<stressing*>receipts;
//    int sequence = 0;
//    node first = nil;
//    node sample;
//    node before;
//    forall(sample, participants)
//      {
//        if (! first)
//  	{
//  	  first = sample;
//  	  before = sample;
//  	  continue;
//  	}
//        stressing* packet = new stressing ( before, sample, sequence);
//        receipts. append(packet);
//        before = sample;
//      }
  replay_stress( argv[1], receipts);
  double average = 0;
  double  maximum = compute_stress (physical_edges,is_physical,
				    average, receipts);

}
