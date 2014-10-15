/* pipdisconnect - pipdisconnect */

#include <xinu.h>

syscall pipdisconnect(
	pipid32 	pipid
	) 
{
	intmask mask;
	struct pipent	*piptr;

	mask = disable();

	if ( isbadpipid(pipid) ) {
		restore(mask);
		return SYSERR;
	}

	piptr = &piptab[pipid];

	if ( piptr->pipstate != PIPE_CONNECTED ) {
		restore(mask);
		return SYSERR;
	}

	if ( piptr->owner != (pid32) getpid() ) {
		restore(mask);
		return SYSERR;
	}

	piptr->pipstate = PIPE_USED;
	piptr->writer = -1;
	piptr->reader = -1;

	semdelete( piptr->emptySem );
	semdelete( piptr->fullSem );

	restore(mask);
	return OK;
}


