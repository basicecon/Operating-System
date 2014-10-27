#include <xinu.h>

/*
 *
 * basically same as receive()
 * except that before the receiving proc returns, 
 * checks any blocked sending proc and 
 * unblock the one waiting the longest
 *
 * */

umsg32 receiveb(void) {
	intmask	mask;
	struct	procent *prptr; 		// receive proc
	struct	procent *sndprptr;		// send proc
	umsg32	msg;

	mask = disable();
	prptr = &proctab[currpid];
	
	if (prptr->prhasmsg == FALSE) {

		if (isempty(prptr->sndqueue)) {
			//kprintf("------ receiveb, sndqueue empty\r\n");
			prptr->prstate = PR_RECV;
			resched();
		} else {
			pid32 sndpid = dequeue(prptr->sndqueue);
			sndprptr = &proctab[sndpid];
			msg = sndprptr->sndmsg;		// set the send proc msg
			//kprintf("------ receiveb, dequeue msg = %d\r\n", msg);
			sndprptr->sndflag = FALSE;	// set the send proc flag
			ready(sndpid, RESCHED_YES);
		}
	}

	msg = prptr->prmsg;
	//kprintf("------ successfully received msg = %d\r\n", msg);
	prptr->prhasmsg = FALSE;
	restore(mask);
	return msg;

}

