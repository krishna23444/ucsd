#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <list>

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
enum CHURN_SCHEDULE
{
  CHURN_SCHEDULE_from_to_exponential_min = 0,
  CHURN_SCHEDULE_from_to_exponential_min_start_with_downtime,
  CHURN_SCHEDULE_from_to_exponential_linger_and_downtime
};
class completion
{
public:
  completion(double howlong1, double when1) :howlong(howlong1), when( when1) {}
  double howlong;
  double when;
};

std::vector <int> inbound_capacity(1000);
std::vector <std::list<completion> > completions(1000);
int file_size_in_bytes = 5*1024*1024;
double overhead=0;
void read_capacity_file (char * capacity_filename) 
{
  FILE *myfile;

  int i=0, inbound, outbound, ilink, olink,who;
  char rest[1024];
  char whole_line[1024];
  printf("forced  capacity_filename: %s\n", capacity_filename);

  myfile = (FILE *)fopen( capacity_filename, "r");
  if (!myfile) {
      printf("nonexistent forced  capacity_file  %s\n",  capacity_filename);
    return;
  }

  while (fgets(whole_line, 512, myfile)!=NULL) {
    int successful = 0;
    successful =sscanf(whole_line, "%d %d %d d %d %[^\n]s", 
		       &who,&inbound, & outbound, &ilink, &olink ,&rest);
    //  printf("whole_line: %s\n", whole_line);
    if (successful<5) 
      {
	//          continue;
      }
    inbound_capacity[who] = inbound;
//  printf("inbound for: %d : %d\n", who,inbound);

  }
  fclose(myfile);
}

void check_download( int index, double current_time, double total_uptime)
{
  if (index != 0)
    {
//        printf("check_download: %d %d %d\n", index, current_time,  total_uptime);

      double required_time =  (1.0+overhead)*file_size_in_bytes*8/(double)(inbound_capacity[index]*1000.0);
//        printf("required_time: %d\n", required_time);
      if (total_uptime>required_time)
	{
//  	      printf("f_download: %d %d %d\n", index,   (int)required_time, (int) current_time + required_time);
	  completions[ index].push_back( completion(required_time, current_time + required_time));
	}

    }
}

void print_completions(int from, int to_inclusive)
{
  for (int index = from; index <= to_inclusive ;index++ )
    {
      for(std::list<completion>::iterator it = completions[ index].begin();
	  it != completions[ index].end(); ++it)
	{
	  completion c = *it;
	  printf("download %d client%d %d\n", (int)c.howlong, index, (int)c.when);
	}
    }
}

int main(int argc, char** argv)
{ 
  if (argc < 10)
    {
      printf("\nError in args.\n");
      printf("\nusage: type initial_time total_time type from_node to_node exponential_mean_uptime_seconds exponential_mean_downtime min_uptime min_downtime\n");
      exit(4);
    }
  int from_node = 0;
  int to_node = 0;
  int mean_uptime = 0;
  int min_uptime = 0;
  int mean_downtime = 0;
  int min_downtime = 0;
  int type=atoi(argv[1]); 
  int initial=atoi(argv[2]); 
  int total_time=atoi(argv[3]); 

  if (type==CHURN_SCHEDULE_from_to_exponential_min ||
      type==CHURN_SCHEDULE_from_to_exponential_min_start_with_downtime)
    {
      from_node =atoi(argv[4]);
      to_node =atoi(argv[5]);
      mean_uptime =atoi(argv[6]);
      mean_downtime =atoi(argv[7]);
      min_uptime =atoi(argv[8]);
      min_downtime =atoi(argv[9]);
      if (argc>10)
	{
	  file_size_in_bytes = atoi(argv[10])*1024*1024;
//  	  printf(" file_size_in_bytes: %d\n",  file_size_in_bytes);

	}
      if (argc>11)
	{
	  read_capacity_file(argv[11]); 
	}
      if (argc>12)
	{
	  overhead=atof(argv[12]); 
	}

      for (int index = 0; index < from_node ;index++ )
	{
	  printf("client%d: 0 %d\n", index, total_time);
	  check_download( index, 0, total_time);
	}
      for (int index = from_node; index <= to_node ;index++ )
	{
	  printf("client%d: ", index);
	  if (type==CHURN_SCHEDULE_from_to_exponential_min)
	    printf("0", index);

	  double starting_time = 0;
	  double current = 0;
	  while (current - starting_time < total_time)
	    {
	      int chosen_uptime = (int) ceil (-mean_uptime*(log(1-drand48())));
	      if (chosen_uptime < min_uptime)
		{
		  chosen_uptime= min_uptime;
		}
	      check_download( index, current, chosen_uptime);

	      int chosen_downtime = (int) ceil (-mean_downtime*(log(1-drand48())));
	      if (chosen_downtime < min_downtime)
		{
		  chosen_downtime= min_downtime;
		}
	  
	      if (type==CHURN_SCHEDULE_from_to_exponential_min)
		printf(" %d %d", chosen_uptime, chosen_downtime);
	      else if (type==CHURN_SCHEDULE_from_to_exponential_min_start_with_downtime)
		printf(" %d %d", chosen_downtime, chosen_uptime);

	      current += chosen_uptime + chosen_downtime;
	    }
	  printf("\n");
	}
      print_completions(1, to_node);
    }
  else if (type==CHURN_SCHEDULE_from_to_exponential_linger_and_downtime)
    {
      from_node =atoi(argv[4]);
      to_node =atoi(argv[5]);
      int mean_linger =atoi(argv[6]);
      int min_linger =atoi(argv[7]);
      int max_linger =atoi(argv[8]);
      mean_downtime =atoi(argv[9]);
      min_downtime =atoi(argv[10]);
      int max_downtime =atoi(argv[11]);

//        for (int index = 0; index < from_node ;index++ )
//  	{
//  	  printf("client%d: 0 %d\n", index, total_time);
//  	}
      for (int index = from_node; index <= to_node ;index++ )
	{
	  printf("client%d: ", index);

	  double starting_time = 0;
	  double current = 0;
	  while (current - starting_time < total_time)
	    {
	      int chosen_linger = (int) ceil (-mean_uptime*(log(1-drand48())));
	      if (chosen_linger < min_linger)
		{
		  chosen_linger= min_linger;
		}

	      if (max_linger && chosen_linger > max_linger)
		{
		  chosen_linger= max_linger;
		}
	      int chosen_downtime = (int) ceil (-mean_downtime*(log(1-drand48())));
	      if (chosen_downtime < min_downtime)
		{
		  chosen_downtime= min_downtime;
		}
	      if (max_downtime && chosen_downtime > max_downtime)
		{
		  chosen_downtime= max_downtime;
		}
	  
	      printf(" %d %d", chosen_linger, chosen_downtime);
	      current += chosen_linger + chosen_downtime;
	    }
	  printf("\n");
	}
    }
  else
    {
      printf("unknown type: %d\n", type);
      exit(1);
    }

}
