
define int display_data(addr, length)
unsigned long * addr;
unsigned int length;
{
	int i;
	unsigned long * temp_addr;
	for ( i = 0; i < length ; i++)
	{
		temp_addr = (unsigned long *)((unsigned char*) addr + i) ;
   		$ fprintf 20, "%x\n", byte(temp_addr)$;
	}
		
   	return(1);   	/*  0 to stop simulation, 1 to continue */
}
.

define int end_application()
{
	$ fprintf 20, "APPLICATION FINISHED \n" $;
	return(0); 	/*  0 to stop simulation, 1 to continue */
}
.


load ../sw/MAC_test.x
//load ../sw/a946.x
mode a

bi TestMain
bi WriteDebuggerWindow; display_data(WriteDebuggerWindow\address, WriteDebuggerWindow\size)
bi _sys_exit; end_application()

