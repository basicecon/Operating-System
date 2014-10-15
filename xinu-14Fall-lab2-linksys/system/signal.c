/* signal.c - signal */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  signal  -  Signal a semaphore, releasing a process if one is waiting
 *------------------------------------------------------------------------
 */
syscall	signal(
	  sid32		sem		/* id of semaphore to signal	*/
	)
{
	intmask mask;			/* saved interrupt mask		*/
	struct	sentry *semptr;		/* ptr to sempahore table entry	*/

	mask = disable();
	if (isbadsem(sem)) {
		restore(mask);
		return SYSERR;
	}
	semptr= &semtab[sem];
	//kprintf("count=%d\r\n", semptr->scount);
	
	if (semptr->sstate == S_FREE) {
	    //kprintf("(FREE)\r\n");
		restore(mask);
		return SYSERR;
	}
	if ((semptr->scount++) < 0) {	/* release a waiting process */
		ready(dequeue(semptr->squeue), RESCHED_YES);
	}
	restore(mask);
	return OK;
}
