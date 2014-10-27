#include <xinu.h>
syscall sendq(
	  pid32		pid,
	  umsg32	msg
	) 
{
	intmask mask;
	struct procent *prptr;		// receive proc
	struct procent *sndprptr; 	// (send) msg proc

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
		int qsize = 0;
		if (qsize < 4) {
		//if (prptr->msgqsize < 4) { // have space in msgqueue
			sndprptr->msg = msg;
			kprintf("enqueue MSG currpid=%d, pid=%d, qid=%d, msg=%d\r\n", currpid, enqueue(currpid, prptr->msgqueue), prptr->msgqueue, msg);
			prptr->msgqsize = 1 + prptr->msgqsize;
		} else { // dont have enough space, sendq blocks
			sndprptr->sndmsg = msg;
			sndprptr->sndflag = TRUE;
			sndprptr->prstate = PR_SND;
			//enqueue(currpid, prptr->sndqueue);
			kprintf("enqueue SND currpid=%d, pid=%d, qid=%d, msg=%d\r\n", currpid, enqueue(currpid, prptr->sndqueue), prptr->sndqueue, msg);
			resched();
		}
	
	} else {
		kprintf("successfully sent msg = %d\r\n", msg);
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
