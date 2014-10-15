/* pipconnect.c - pipconnect */

#include <xinu.h>

syscall pipconnect(
	pipid32 	pip,
	pid32 		pid1,
	pid32 		pid2
	) 
{	
	intmask mask;
	struct pipent 	*piptr;

	mask = disable();

	if ( isbadpipid(pip) ) {
		restore(mask);
		return SYSERR;
	}
	
	piptr = &piptab[pip];

	if ( piptr->pipstate == PIPE_CONNECTED ) {
		restore(mask);
		return SYSERR;
	}

	if ( piptr->owner != (pid32) getpid() ) {			
		restore(mask);
		return SYSERR;
	}

	if ( isbadpid(pid1) || isbadpid(pid2) || pid1 == NULLPROC || pid2 == NULLPROC ) {
		restore(mask);
		return SYSERR;
	}

	//prptr1 = &proctab[pid1];
	//prptr2 = &proctab[pid2];

	piptr->pipstate = PIPE_CONNECTED;
	piptr->writer = pid1;
	piptr->reader = pid2;
	restore(mask);
	return OK;
}
