/* head.c - head */

#include <xinu.h>

/*-------------------------------------------------------------------------
 * head  -- read the first 100 bytes from the file
 *-------------------------------------------------------------------------
 */
int32 	head (
	  did32		fd,	 		/* the file descriptor 		*/ 
	  char 		*buff			/* the pointer to the buffer with the result */
	) 
{
	int32 		retval;	
	intmask		mask;
	retval = read(fd, buff, 100);
	if (retval == SYSERR) {
		retval = 0;
	}
	buff[retval] = '\0';
	restore(mask);
	return retval;
}

