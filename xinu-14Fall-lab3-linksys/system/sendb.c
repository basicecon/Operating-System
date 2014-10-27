#include <xinu.h>

/*
 * same as send()
 * except when blocking a send proc
 * put in sending state
 *
 * */

syscall sendb(
	  pid32		pid,
	  umsg32	msg
	) 
{
	intmask mask;
	struct procent *prptr;		// receive proc
	struct procent *sndprptr; 	// send proc

	mask = disable();
	if (isbadpid(pid)) {
		restore(mask);
		return SYSERR;
	}

	prptr = &proctab[pid];
	if (prptr->prstate == PR_FREE) {
		restore(mask);
		return SYSERR;
	}

	sndprptr = &proctab[currpid];
	if (prptr->prhasmsg) {
		//kprintf("------ enqueue sendb msg = %d\r\n", msg);
		sndprptr->sndmsg = msg;
		sndprptr->sndflag = TRUE;
		sndprptr->prstate = PR_SND;
		enqueue(currpid, prptr->sndqueue);
		resched();
	} else {
		//kprintf("------ successfully sent msg = %d\r\n", msg);
		prptr->prmsg = msg;
		prptr->prhasmsg = TRUE;
	}

	if (prptr->prstate == PR_RECV) {
		ready(pid, RESCHED_YES);
	} else if (prptr->prstate == PR_RECTIM) {
		unsleep(pid);
		ready(pid, RESCHED_YES);
	}
	restore(mask);
	return OK;
}
