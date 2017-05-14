/* This file implements a few low level library functions and some other 
   functions so that C programs can execute in the Altera+ARM environment. 
   We've to do this since we do not have semihosting SWI support (with XRAY). 
   I've implemented:
   a) _sys_exit() and _ttywrch() to handle program exit and errors
   b) __user_initial_stackheap() to initialize the run-time memory model
   c) WriteDebuggerWindow()      This call is handled by XRAY and prints data 
				 in XRAY window
*/

#include <rt_misc.h>

void _sys_exit(int return_code)
{
	label: goto label; /* endless loop. XRAY has a macro to terminate program */
			   /* when _sys_exit is called */
}

void _ttywrch(int ch) 	   /* Do nothing */
{
}	

__value_in_regs struct __initial_stackheap __user_initial_stackheap(
	unsigned R0, unsigned SP, unsigned R2, unsigned SL)
{
	struct __initial_stackheap config;
	//extern unsigned int Image$$SRAM$$ZI$$Limit;
	extern unsigned int Image$$EXEC_SRAM$$ZI$$Limit;
	
	config.heap_base  = (unsigned int)&Image$$EXEC_SRAM$$ZI$$Limit;
	config.stack_base = SP;

	return config;
}

void WriteDebuggerWindow(unsigned long* address, unsigned int size)  	/* XRAY debugger will handle this call */
					       	/* and print the data at this address in the debugger window */
{
}

