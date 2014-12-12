#include <xinu.h>
#include <stdio.h>

void test_mkdir(void) 
{
	// open the file system
	int32	retval = open(RDISK, "XinuDisk", "rw");
	if (retval == SYSERR)
		kprintf("Error opening the Xinu File System\r\n");
	// initialize the file system
	lfscreate(RDISK, 1000, 1024000);
	
	did32	fd = 0;
	fd = open();
}
