/*  

Common Macedon lock routines.

Adolfo Rodriguez



*/
#include "macedon.h"
#include "macedon-macros.h"

#define MACEDON_TRACE_LOCK

#define MACEDON_LOCK_ON                       10
#define MACEDON_LOCK_OFF                     -10


/*
 * set and unset and test lock operations.
 */
void MACEDON_Agent::lock_on( int s )
{
  double curtime = Scheduler::instance().clock();
  lockstr_.lock_           = MACEDON_LOCK_ON;
  lockstr_.source_of_lock_ = s;
  lockstr_.time_of_lock_ = curtime;
  
#ifdef MACEDON_TRACE_LOCK
  debug_macro("Lock: locked by %x.\n", lockstr_.source_of_lock_);
#endif
}


void MACEDON_Agent::lock_off ( )
{
  // unlock myself
  lockstr_.lock_           = MACEDON_LOCK_OFF;
  
#ifdef MACEDON_TRACE_LOCK
  debug_macro("Lock: unlocked by %x\n", lockstr_.source_of_lock_ );
#endif
  
  lockstr_.source_of_lock_ = 0;
}



int MACEDON_Agent::test_lock()
{
  return ( lockstr_.lock_ );
}


int MACEDON_Agent::test_lock_source()
{
  return ( lockstr_.source_of_lock_ );
}
