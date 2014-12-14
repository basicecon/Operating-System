/* lflRead.c  -  lfRead */

#include <xinu.h>

/*------------------------------------------------------------------------
 * lflRead  --  read from a previously opened local file
 *------------------------------------------------------------------------
 */
devcall	lflRead (
	  struct dentry *devptr,	/* entry in device switch table */
	  char	*buff,			/* buffer to hold bytes		*/
	  int32	count			/* max bytes to read		*/
	)
{
	uint32	numread;		/* number of bytes read		*/
	int32	nxtbyte;		/* character or SYSERR/EOF	*/

	//kprintf("entering lflRead...\r\n");
	if (count < 0) {
		return SYSERR;
	}

	for (numread=0 ; numread < count ; numread++) {
		//kprintf("numread = %d\r\n", numread);
		nxtbyte = lflGetc(devptr);
		//kprintf("nxtbyte = %d\r\n", nxtbyte);

		if (nxtbyte == SYSERR) {
			return SYSERR;
		} else if (nxtbyte == EOF) {	/* EOF before finished */
		    if (numread == 0) {
		    	//kprintf("EOF\r\n");
		    	return 0;
				//return EOF;
		    } else {
			return numread;
		    }
		} else {
			*buff++ = (char) (0xff & nxtbyte);
		}
	}
	return numread;
}
