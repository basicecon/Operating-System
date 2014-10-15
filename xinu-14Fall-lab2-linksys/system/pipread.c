/* pipread.c - pipread_unblock pipread_block */

#include <xinu.h>

syscall pipread_nonblock(
	pipid32 	pip,
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
    
	if ( piptr->reader != (pid32) getpid() ) {
		restore(mask);
		return SYSERR;
	}

    //kprintf("index piptr->start=%d, piptr->end=%d\n", piptr->start, piptr->end);
    
	if ( (piptr->end == piptr->start) && (piptr->buffer[piptr->start] == '\0') ) {  
		/* the bounded buffer is empty */
		//kprintf("EMPTY bounded buffer!\n");
		restore(mask);
		return 0;
	}
	
	uint32 index;
	
	for ( index = 0; index < len; index ++ ) {
	    //kprintf("content piptr->start=%s\n", piptr->buffer[piptr->start]);
		buf[index] = piptr->buffer[piptr->start];
		piptr->buffer[piptr->start] = '\0';
		//kprintf("curr progress of reading... index=%d, buf=%s\n", index, buf[index]);
		piptr->start = (piptr->start + 1) % PIPE_SIZE;
    
		/* reached the end of the bounded buffer */	
		if ( piptr->start == piptr->end ) { 
			uint32 cnt = index + 1;
			restore(mask);
			return cnt;
		}
	}

	restore(mask);
	return index;

}

/* ----------------------------------------------------------------- */

syscall pipread_block(
	pipid32 	pip,
	char 		*buf,
	uint32		len
	) 
{
	intmask 	mask;
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

	if ( piptr->reader != (pid32) getpid() ) {
		restore(mask);
		return SYSERR;
	}

	uint32 index;
	
	//kprintf("reading pipid=%d\r\n", pip);
	for ( index = 0; index < len; index ++ ) {
		/* wait unitl bounded buffer is not empty */
		
		//kprintf("emptySem__(sem=%d)", piptr->emptySem);
		wait( piptr->emptySem );

		buf[index] = piptr->buffer[piptr->start];
		piptr->buffer[piptr->start] = '\0';
		piptr->start = (piptr->start + 1) % PIPE_SIZE;

		/* write_block will wait unitl buffer is not full */
		//kprintf("fullSem__(sem=%d)", piptr->fullSem);
		signal( piptr->fullSem );
		
		/* reached the end of the bounded buffer */    
        if ( piptr->start == piptr->end ) {
            //kprintf("READ REACHED END!\r\n");
            semreset(piptr->emptySem, 0);
            uint32 cnt = index + 1;
            restore(mask);
            return cnt;
        }
	}

	restore(mask);
	return index;
}

