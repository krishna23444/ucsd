#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "StatisticalFilter.h"
#ifdef GCC3
#include <ext/hash_set>
using namespace __gnu_cxx;
#else
#include <hash_set>
#endif
const int PARTICIPANTS = 10000;
const int MAX_DURATION = 10000;
struct eqstr
{
  bool operator()(const char* s1, const char* s2) const
  {
    return strcmp(s1, s2) == 0;
  }
};

double total_bandwidth_time[MAX_DURATION][PARTICIPANTS];

statistical_filter bandwidth[MAX_DURATION];
statistical_filter total_bandwidth[MAX_DURATION];
statistical_filter raw_parent_bandwidth[MAX_DURATION];
statistical_filter useful_parent_bandwidth[MAX_DURATION];
statistical_filter stream_parent_bandwidth[MAX_DURATION];
hash_set<const char*, hash<const char*>, eqstr> seen;
//  vector <statistical_filter> bandwidth[1000]; 
double percentile =0.95;
int highest_node_seen = 0;

void printer ( FILE * file, statistical_filter& bandwidth)
{
  double average =bandwidth.getValue();
  double error =  bandwidth.get_deviation()/(double)bandwidth.get_count();
  double deviation =  bandwidth.get_deviation();
  double percentilex =  bandwidth.get_percentile(percentile);

  fprintf( file," %f %f %f %f %f  %f %d %d %d %d  ",
	   average, 
	   average - error, 
	   average + error,
	   average - deviation, 
	   average + deviation,
	   percentilex,
	   0, 0, 0, 0);

}

int main (int argc, char** argv)
{
  int c;
  double rate=0;
  double hold_time=0.0;
  char*input_file= NULL;
  char*output_file= NULL;
  char*node_file= NULL;
  char*solution_file= NULL;
//  if (argc>2)solution_file = strdup(argv[2]);
//  if (argc>1)input_file = strdup(argv[1]);
  while ((c = getopt(argc, argv, "s:i:o:r:p:h:n:")) != EOF)
    switch (c) {
    case 'p':
      percentile = atof(optarg)/100.0;
      break;
    case 'r':
      rate = atof(optarg)*1000.0;
      break;
    case 's':
      solution_file = strdup(optarg);
      break;
    case 'n':
      node_file = strdup(optarg);
      break;
    case 'i':
      input_file = strdup(optarg);
      break;
    case 'h':
      hold_time = atof(optarg);
      break;
    case 'o':
      output_file = strdup(optarg);
      break;
    }

  double time_index= 0;
  double dump_time_index= 0;
  double counter_time_index= 0;
  double starting_time_index= 0;
  int starting_set = 0;
  int counter = 0;
  int second_counter = 0;

 FILE *myfile;
 FILE *outfile;
  int i=0;
  char name[80];
  char command[80];
  char status[80];
  char rest[512];
  char whole_line[512];
  double time_when;
  int tid;
  int initialized = 0;
  myfile = (FILE *)fopen(input_file, "r");
  if (!myfile)
    {
      printf("nonexistent replay file  %s\n", 
	     input_file);
	  exit( 1);

    }
  if (output_file)
    {
      outfile = (FILE *)freopen( output_file, "w",stdout);
    }
  else
    {
      outfile = stdout;
    }

  if (!outfile)
    {
      printf("nonexistent  output file  %s\n", 
	     input_file);
	  exit( 1);
    }
  i=0;

  while (fgets(whole_line, 512, myfile)!=NULL)
    {
//        cout << whole_line << endl;
//        fgetc(myfile);whole_line[ strlen( whole_line)]='\n';
      int successful = 0;
      if ((successful =sscanf(whole_line, "%s %lf %d %s %[^\n]s", 
		 &name,  &time_when, &tid,&command, &rest))!= 5)
	{
	  continue;
	}
//  printf("got %d, %s %f %d : %s %s\n", 
//         successful,name,  time_when, command, rest);
//  printf("got %f %f %f\n", 
//         time_when,starting_time_index,dump_time_index);

      if (strstr (command,"REPLAY_") != NULL && strstr(command, "_BANDWIDTH") != NULL)
	{	
	  if (0 == atoi(name+6))
	    {
	      // skip the root
	      continue;
	    }


	  if (highest_node_seen < atoi(name+6))
	    {
	      highest_node_seen = atoi(name+6);
	    }

	  int p_total_bandwidth;
	  int p_bandwidth;
	  int p_useful_bandwidth;
	  int t_bandwidth;
	  int useful_bandwidth;
	  int smooth_bandwidth;

	  if (sscanf(rest, "%d %d %d %d %d %d", 
		     &p_total_bandwidth,&p_bandwidth,&p_useful_bandwidth,&t_bandwidth, &useful_bandwidth, &smooth_bandwidth)!= 6)
	    continue;
	  //  	  printf("smooth_bandwidth: %d\n", smooth_bandwidth);
	  hash_set<const char*, hash<const char*>, eqstr>::const_iterator it = seen.find(name);
	      total_bandwidth_time[second_counter][atoi(name+6)]=useful_bandwidth;	  
	  if (it == seen.end())
	    {
	      bandwidth[counter].update(useful_bandwidth);
	      total_bandwidth[counter].update(t_bandwidth);
	      stream_parent_bandwidth[counter].update(p_bandwidth);
	      raw_parent_bandwidth[counter].update(p_total_bandwidth);
	      useful_parent_bandwidth[counter].update(p_useful_bandwidth);
	      seen.insert(name);
	    }
	}
      if (!starting_set)
	{
	  starting_set = 1;
	  starting_time_index =time_when;
	}
      //NOTE: I'm abusing stream_parent_bandwidth for control bandwidth
      if (!counter_time_index||
	  time_when> counter_time_index+ 1)
	{
	  counter_time_index=time_when;

	  second_counter ++;
//  	  seen.clear();
	}

      if (!dump_time_index||
	  time_when> dump_time_index+ 5)
	{
	  dump_time_index=time_when;

	  fprintf(outfile,"%f %d %d %d %d %d %d %d %d %d",
		 time_when-starting_time_index, 
		 0, 0, 0, 0, 0, 0, 0, 0, 0
		 );
	  printer( outfile, bandwidth[counter]);
	  printer( outfile, raw_parent_bandwidth[counter]);
	  printer( outfile, stream_parent_bandwidth[counter]);
	  printer( outfile, useful_parent_bandwidth[counter]);
	  printer( outfile, total_bandwidth[counter]);

	  fprintf(outfile,"\n");
	  counter ++;
	  seen.clear();
	}
    }
  fclose(myfile); 

  if (solution_file!=NULL)
    {
      myfile = (FILE *)fopen(solution_file, "w");
      if (!myfile)
	{
	  printf(" cannot open outputfile  %s\n", 
		 solution_file);
	  exit( 1);
	}

      int exclude = 5; 
      int back = 20;
      int earliest =counter - exclude - back;
      if (earliest < 0)
	{
	  earliest =0;
	}
      statistical_filter solution_bandwidth;
      statistical_filter solution_total_bandwidth;
      statistical_filter solution_raw_parent_bandwidth;
      statistical_filter solution_useful_parent_bandwidth;
      statistical_filter solution_stream_parent_bandwidth;

      for (int sample =  counter - exclude; sample >  earliest ;sample-- )
	{
	  solution_bandwidth.update(bandwidth[sample].getValue());
	  solution_total_bandwidth.update(total_bandwidth[sample].getValue());
	  solution_stream_parent_bandwidth.update(stream_parent_bandwidth[sample].getValue());
	  solution_raw_parent_bandwidth.update(raw_parent_bandwidth[sample].getValue());
	  solution_useful_parent_bandwidth.update(useful_parent_bandwidth[sample].getValue());
	}

      fprintf( myfile,"%f %f %f %f %f %f %d %d %d %d",
	       rate, 
	       solution_bandwidth.getValue(), 
	       solution_bandwidth.getValue()/rate, 
	       percentile, 
	       solution_bandwidth.get_percentile(percentile), 
	       hold_time,
	       0, 
	       0, 
	       0,
	       0
	     );
      printer(  myfile,solution_bandwidth);
      printer(  myfile,solution_raw_parent_bandwidth);
      printer(  myfile,solution_stream_parent_bandwidth);
      printer(  myfile,solution_useful_parent_bandwidth);
      printer(  myfile,solution_total_bandwidth);
      fprintf(myfile,"\n");
      fclose(myfile); 

    }

  if (node_file!=NULL)
    {
      myfile = (FILE *)fopen(node_file, "w");
      if (!myfile)
	{
	  printf(" cannot open outputfile  %s\n", 
		 node_file);
	  exit( 1);
	}

      int exclude = 20; 
      int back = 120;
      int earliest =second_counter - exclude;
      if (earliest < 0)
	{
	  earliest =0;
	}

      for (int sample = 0; sample <=highest_node_seen;sample++ )
	{
	  statistical_filter average;
	  for (int Index = earliest-back; Index < earliest ;Index++ )
	{
	  average.update(total_bandwidth_time[ Index][sample]);
	}

	  int position = earliest;
	  double value = average.getValue();

	  fprintf( myfile, "%d %f\n", sample, value);
	}

      fclose(myfile); 

    }


  return 0;
}


