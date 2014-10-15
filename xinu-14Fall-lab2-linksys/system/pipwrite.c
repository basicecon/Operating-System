/* pipwrite.c - pipwrite_nonblock pipwrite_block */

#include <xinu.h>

syscall pipwrite_nonblock(
	pipid32 	pip,
	char		*buf,
	uint32 		len	/* length of the data to be written */
	) 
{
	intmask		mask;
	struct pipent 	*piptr;

	mask = disable();

	if ( isbadpipid(pip) ) {
		restore(mask);
		return SYSERR;
	}

	piptr = &piptab[pip];

	if ( piptr->pipstate != PIPE_CONNECTED ) {
		restore(mask);
		return SYSERR;
	}
	
	if ( piptr->writer != (pid32) getpid() ) {
		restore(mask);
		return SYSERR;
	}

	// check not empty
	if ( (piptr->end == piptr->start) && (piptr->buffer[piptr->start] != '\0') ) { 
		/* the bounded buffer is full */
		kprintf("FULL bounded bufffer!\n");
		restore(mask);
		return 0;
	}
	uint32 index;
	//kprintf("buf len=%d, ", len);
	for ( index = 0; index < len; index ++ ) {
		piptr->buffer[piptr->end] = buf[index];
		//kprintf("wrote__(index=%d, content=%s\n)", piptr->end, buf[index]);
		piptr->end = (piptr->end + 1) % PIPE_SIZE;

		if ( piptr->end == piptr->start ) {
			/* return the actual number of bytes written */
			uint32 cnt = index + 1;
			restore(mask);
			return cnt;
		}
	}
	
	restore(mask);
	return index;
}

/* ------------------------------------------------------------ */

syscall pipwrite_block(
	pipid32		pip,
	char 		*buf,
	uint32		len
	) 
{
	intmask 	mask;
	struct pipent	*piptr;

	mask = disable();

	if ( isbadpipid(pip) ) {
		restore(mask);
		return SYSERR;
	}

	piptr = &piptab[pip];

	if ( piptr->pipstate != PIPE_CONNECTED ) {
		restore(mask);
		return SYSERR;
	}

	if ( piptr->writer != (pid32) getpid() ) {
		restore(mask);
		return SYSERR;
	}
 	
	uint32 index;
	//kprintf("writing pipid=%d\r\n", pip);
	for ( index = 0; index < len; index ++ ) {
		/* wait until buffer is not full */
		
		//kprintf("fullSem__(sem=%d)", piptr->fullSem);
		wait( piptr->fullSem );
		piptr->buffer[piptr->end] = buf[index];
		piptr->end = (piptr->end + 1) % PIPE_SIZE;

		/* read_block will wait until buffer is not empty */
		//kprintf("emptySem__(sem=%d)", piptr->emptySem);
		signal( piptr->emptySem );
		
		if ( piptr->end == piptr->start ) {
            /* return the actual number of bytes written */
            
            //kprintf("WRITE REACHED END!\r\n");
            uint32 cnt = index + 1;
            restore(mask);
            return cnt;
        }
	}

	restore(mask);
	return index;
}

