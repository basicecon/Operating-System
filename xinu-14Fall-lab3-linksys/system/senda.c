#include <xinu.h>

syscall senda(
	  pid32		pid,
	  umsg32	msg
	) 
{	
	//kprintf("entering senda\r\n");
	intmask mask;
	struct	procent *prptr;

	mask = disable();
	if (isbadpid(pid)) {
		//kprintf("senda SYSERR\r\n");
		restore(mask);
		return SYSERR;
	}
	
	prptr = &proctab[pid];
	//kprintf("pid = %d\r\n", pid);
	/*
	if ((prptr->prstate == PR_FREE) || prptr->prhasmsg) {
		restore(mask);
		return SYSERR;
	}*/	
	if (prptr->prstate == PR_FREE) {
		restore(mask);
		return SYSERR;
	}
	
	//kprintf("original msg = %d\r\n", msg);
	//*(prptr->cbbuff) = msg;
	*cbbuff = msg;
	//kprintf("msg = %d\r\n", msg);
	//prptr->cbfunc();
	cbfunc();
	return OK;
}
