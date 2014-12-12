#include <xinu.h>
#include <stdio.h>

void test_root(void)
{
	// open the file system
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
	//kprintf("fd = %d\r\n", fd);
	/*
	char	buff[90];
	int32	i = 0;
	while (i < 89) {
		buff[i] = 'a';
		i ++;
	}
	buff[89] = '\0';
	write(fd, buff, 50);
	close(fd);
	// initialized the new buffer
	char	new_buff[101];
	i = 0;
	while (i < 100) {
		new_buff[i] = 'b';
		i ++;
	}
	//new_buff[100] = '\0';
	fd = open(LFILESYS, "1", "rw");
	int32	ret = head(fd, new_buff);
	//kprintf("new_buff -> %s\r\n", new_buff);
	*/
	close(fd);
	//kprintf("how many bytes were read = %d\r\n", ret);
	//kprintf("first %d characters of the file: %s\n", ret, new_buff);
}
