#include <xinu.h>
#include <stdio.h>

void test_root(void)
{
	// open the file system

/*
	int32	retval = open(RDISK, "XinuDisk", "rw");
	if (retval == SYSERR)
		kprintf("Error opening the Xinu File System\r\n");
	// initialize the file system
	lfscreate(RDISK, 1000, 1024000);

	did32	fd = 0;
	char path[] = "/a/b/c";
	fd = open(LFILESYS, path, "rwn");
	if (fd == SYSERR) {
		kprintf("error");
	} else {
		kprintf("pass");
	}
*/

	int32 retval;
	//int32 fd[5];
	int32 i;

	kprintf("Open remote disk\r\n");
	retval = open(RDISK,"XinuDisk","rw");
	if (retval == SYSERR){
		panic("Error: could not open the remote disk, check the server\n\r");
	}

	kprintf("Initializing file system\r\n");
	//retval = lfscreate ( RDISK, 1000, 1024000);
	retval = control(LFILESYS, LF_CTL_TO_HIERARCHY, 1024*6, 8192*512);
	if( retval == SYSERR )
		panic("Initialize file system failed");

	
	// Create 5 directories
	char dirname [] = {"/DIR_1"};
	kprintf("\r\n");
	for( i=0; i<5; i++){
		dirname[5] = '1'+i;
		kprintf("Creating directory %s\r\n", dirname );
		retval = control(LFILESYS, LF_CTL_MKDIR, (int)dirname, 0);
		if( retval == SYSERR )
			kprintf("....failed");
		kprintf("\r\n");
	}
}
