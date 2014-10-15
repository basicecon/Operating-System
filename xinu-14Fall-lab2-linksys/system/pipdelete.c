/* pipdelete.c */

#include <xinu.h>

syscall pipdelete(pipid32 pipid) {
	intmask mask;
	struct pipent *piptr;

	mask = disable();

	if (isbadpipid(pipid)) {
		restore(mask);
		return SYSERR;
	}

	piptr = &piptab[pipid];

	if (piptr->owner != (pid32) getpid()) {
		restore(mask);
		return SYSERR;
	}
	
	pipcount --;
	piptr->pipstate = PIPE_FREE;
	memset(piptr->buffer, '\0', PIPE_SIZE);
	piptr->owner = -1;
	piptr->writer = -1;
	piptr->reader = -1;
	
	//semdelete(piptr->pipsem);
	semdelete(piptr->emptySem);
	semdelete(piptr->fullSem);

	restore(mask);
	return OK;
	
}


