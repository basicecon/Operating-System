/* lfsControl.c - lfsControl */

#include <xinu.h>

/* --------------------------------------------------------------
 * lfsControl - Provide control functions for local file system
 * --------------------------------------------------------------
 */
devcall lfsControl (
	struct dentry	*devptr,	/* entry in device switch table */
	int32	func,			/* a control function		*/
	int32	arg1,			/* argument #1			*/
	int32	arg2			/* argument #2 			*/
	) 
{
	int32	retval;			/* return value from func. call	*/

	switch (func) {
	/*
		case LF_CTL_TO_HIERARCHY:
			retval = lfscreate(LF_DISK_DEV, arg1, arg2);
			return retval;
	*/
		case LF_CTL_MKDIR:
			retval = lfsmkdir(devptr, (char*)arg1);
			return retval;
		case LF_CTL_LS:
			retval = lflistdirh( LF_DISK_DEV, (char*)arg1 );
			return retval;
		default:
			return SYSERR;
	}

	return OK;
}
