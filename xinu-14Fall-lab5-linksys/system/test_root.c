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
	int32 fd[5];
	int32 i;

	char wbuff[] = {"While disks provide a convenient means of permanent storage, they support only a simple and limited naming mechanism of the data they hold (the block number). File systems are used to overcoming this limitation by providing a more elaborate and useful naming mechanism for the data blocks on a disk. This original Xinu File System is a mini-file system with an elegant design that has a limited numbers of files, short file names, and no hierarchical directory structure; however, it allows files to grow dynamically. \r\nThe objective of this lab is to enhance the existing file system for Xinu. You will extend the functionality of the Xinu File System on top of the remote disk abstraction. You are going to transform the current flat file system into a new file system with hierarchical directory structure. \r\n"};
	int32 wbufflen = sizeof( wbuff );
	//char rbuff[ wbufflen * 10 ];
	//int32 rbufflen = sizeof( rbuff );
	

	kprintf("Open remote disk\r\n");
	retval = open(RDISK,"XinuDisk","rw");
	if (retval == SYSERR){
		panic("Error: could not open the remote disk, check the server\n\r");
	}

	kprintf("Initializing file system\r\n");
	retval = lfscreate ( RDISK, 1000, 1024000);
	//retval = control(LFILESYS, LF_CTL_TO_HIERARCHY, 1000, 1024000);
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
	
	//Open a file in each directory
	char filename [] = {"/DIR_1/file"};
	kprintf("\r\n");
	for( i=0; i<5; i++)
	{ 
		filename[5] = '1'+i;
		kprintf("Opening file at %s\r\n", filename);
		fd[i] = open(LFILESYS, filename, "rwn");
		kprintf("  Got the sudo device : %d \r\n", fd[i]);
	}

	// --------------------------------------------------------------------------------
	// Write to first file
	kprintf("\r\nWriting %d characters to /DIR_1/file .... ", wbufflen );
	retval = write( fd[0], wbuff, wbufflen );
	if( retval == SYSERR )
		kprintf("Write failed \r\b");
	else
		kprintf("Write returned %d \r\n", retval );

	// Write to last file
	kprintf("\r\nWriting %d characters to /DIR_5/file \r\n", wbufflen*10 );
	int total = 0;
	for( i=0; i<10; i++ ){
		retval = write( fd[4], wbuff, wbufflen );
		if( retval == SYSERR )
			kprintf("Write failed \r\b");
		else {
			kprintf("Write returned %d \r\n", retval );
			total += retval;
		}
	}
	if( total != 0 )
		kprintf("Wrote a total of %d characters\r\n", total );

	
	//list files
	kprintf("\r\nList root directory:\r\n");
	retval = lflistdirh( LFILESYS, "/" );
	if( SYSERR == retval ) 
		kprintf("  Listing failed\r\n");
	for( i=0; i<5; i++ ){
		dirname[5] = '1'+i;
		kprintf("\r\nList directory %s:\r\n", dirname);
		retval = lflistdirh( LFILESYS, dirname );
		if( SYSERR == retval ) 
			kprintf("  Listing failed\r\n");
	}

	//return OK;
}
