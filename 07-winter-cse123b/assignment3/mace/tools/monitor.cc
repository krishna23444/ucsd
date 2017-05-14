#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>

#include <signal.h>

double now_time()
{
  double ret;
  timeval clock_;
  struct timezone tz_;
  gettimeofday(&clock_, &tz_);
  ret = (clock_.tv_sec+(double)clock_.tv_usec/1000000.0);
  return ret;
}

double sure_sleep( double amount)
  {
#if NSPORT_DEBUG > 1
      printf("asked to sleep for %f seconds\n",  amount);
#endif

    struct timeval timeout;
    double now, original;
    double left = amount;
    original = now_time();
    do 
      {
      timeout.tv_sec= (int)left;
      timeout.tv_usec= (int)((left - timeout.tv_sec)*1000000);
      int code =select(0,NULL,NULL,NULL,&timeout);
      if (code < 0)
	{
#if NSPORT_DEBUG > 0
	  printf("error in select %s \n",strerror(errno));
#endif
	}

      now = now_time();
      left -= now - original;
#if NSPORT_DEBUG > 1
      printf("slept for %f seconds, left %f \n", now - original, left);
#endif

      original = now;
    }
 
    while (left > 0.02);

      return left;
}

int main(int argc, char** argv)
{ 
  if (argc < 2)
    {
      printf("\nError in args.\n");
      exit(4);
    }
  int length = atoi(argv[1]);
  int child = 0;
  int st = 0;

  printf("monitor running '%s' '%s'  '%s'  '%s'  '%s'  '%s'  '%s' for : %d\n", argv[2], argv[3], argv[4],argv[5], argv[6], argv[7], argv[8], length);
  if ((child =fork()) ==0)
    {

      if (execl(argv[2], argv[2], argv[3], argv[4],argv[5], argv[6], argv[7], argv[8], NULL) < 0)
	{
	  fprintf(stderr,"monitor failed: %s\n",strerror(errno));
	  _exit (1);
	} 
    }
  else
    {
      printf("child: %d\n", child);

      sure_sleep(atoi(argv[1]));
      kill(child, SIGKILL);
      wait (&st);
      printf("monitor exiting\n" );
    }
}
