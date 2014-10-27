#include <xinu.h>

/*
 * first process msg in msgqueue
 * then unblock a sender from sndqueue and put the msg into msgqueue
 *
 * */

umsg32 receiveq(void) {
	intmask	mask;
	struct	procent *prptr; 		// receive proc
	struct	procent *sndprptr;		// send proc
	struct 	procent *msgprptr;		// msg proc
	umsg32	msg;

	mask = disable();
	prptr = &proctab[currpid];
	
	if (prptr->prhasmsg == FALSE) {
	
		if (!isempty(prptr->msgqueue)) {
			pid32 msgpid = dequeue(prptr->msgqueue);
			prptr->msgqsize = prptr->msgqsize - 1;

			msgprptr = &proctab[msgpid];
			kprintf("dequeue msgqueue msg = %d\r\n", msgprptr->msg);
			
			//while (!isempty(prptr->sndqueue) && qsize < 3) {
			if (!isempty(prptr->sndqueue)) {

				pid32 sndpid = dequeue(prptr->sndqueue);
				sndprptr = &proctab[sndpid];
				sndprptr->msg = sndprptr->sndmsg;
				enqueue(sndpid, prptr->msgqueue);
				prptr->msgqsize = 1 + prptr->msgqsize;
				kprintf("enqueue msgqueue msg = %d\r\n", sndprptr->msg);
				
			}
			//prptr->prmsg = msg;
			msg = msgprptr->msg;
			prptr->prhasmsg = FALSE;
			kprintf("received msg = %d\r\n", msg);
			restore(mask);
			return msg;
		} else {
			prptr->prstate = PR_RECV;
			resched();
		}
			
	}

	msg = prptr->prmsg;
	kprintf("received msg = %d\r\n", msg);
	prptr->prhasmsg = FALSE;
	restore(mask);
	return msg;

}

