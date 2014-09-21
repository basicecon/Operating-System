/* resched.c - resched */

#include <xinu.h>
#include <string.h>

/*------------------------------------------------------------------------
 *  resched  -  Reschedule processor to highest priority eligible process
 *------------------------------------------------------------------------
 *
 *
 */

_Bool ROUND_ROBIN;
struct procent *ptold;
struct procent *ptnew;

void	resched(void)		/* assumes interrupts are disabled	*/
{
	//struct procent *ptold;	/* ptr to table entry for old process	*/
	//struct procent *ptnew;	/* ptr to table entry for new process	*/

	/* If rescheduling is deferred, record attempt and return */

	if (Defer.ndefers > 0) {
		Defer.attempt = TRUE;
		return;
	}

	/* Point to process table entry for the current (old) process */

	ptold = &proctab[currpid];
	
	if (!ROUND_ROBIN) {  
		if (ptold->prstate == PR_CURR) {  
			if (ptold->prprio > firstkey(readylist)) {
				return;
			}

			ptold->prstate = PR_READY;
			insert(currpid, readylist, ptold->prprio);
		}
		currpid = dequeue(readylist);

	} else { 

		roundrobin();
	}

	ptnew = &proctab[currpid];
	ptnew->prstate = PR_CURR;
	preempt = QUANTUM;		/* reset time slice for process	*/
	ctxsw(&ptold->prstkptr, &ptnew->prstkptr);

	/* Old process returns here when resumed */

	return;
}

void roundrobin (void) {

	if (ptold->prstate == PR_CURR) {
		ptold->prstate = PR_READY;
		enqueue(currpid, readylist);
	}

	char *oldprocgn = proctab[currpid].prgname; // old process group name
	//char *oldprocn = proctab[currpid].prname;
	//kprintf("\n(%s,%s) ", oldprocn, oldprocgn);
	pid32 curr = queuehead(readylist);

	pid32 tmp;
	struct procent *tmpp;
	char *tmpgn;
	char *tmpn;

	int num = 0; 
	// number of processes in the readylist needs to be dequeue/enqueue
	//kprintf("\nBEFORE: ");
	while (queuetab[curr].qnext != EMPTY) {
		tmp = queuetab[curr].qnext;
		tmpp = &proctab[tmp];
		//tmpn = tmpp->prname;
		tmpgn = tmpp->prgname;
		if (strncmp(tmpgn, oldprocgn, 20) == 0) {
			num ++;	
		}
		//kprintf("_%s_", tmpn);
		curr = queuetab[curr].qnext;
	}
	//kprintf("\n");
	//kprintf(" NUM=%d ", num);


	curr = queuehead(readylist);
	/*	
	kprintf("BEFORE!!!!");
	while (queuetab[curr].qnext != EMPTY) {
		tmp = queuetab[curr].qnext;
		tmpp = &proctab[tmp];
		tmpn = tmpp->prname;
			
		kprintf("(%s)", tmpn);
		curr = queuetab[curr].qnext;
	}
	kprintf("\n");
	*/

	if (num > 1) {
		int cnt = 0;
		curr = queuehead(readylist);
 		//pid32 next = curr;
		/*
		while (queuetab[curr].qnext != EMPTY && cnt < num) {
 			tmp = queuetab[curr].qnext;
			tmpp = &proctab[tmp];
			tmpgn = tmpp->prgname;
			tmpn = tmpp->prname; 
		
			//next = queuetab[tmp].qnext;
			kprintf("currProcessIs_%s  ", tmpn);
				
			if(strncmp(tmpgn, oldprocgn, 20) == 0) {
				kprintf("_FOUND(%s)WITH(%d)_", tmpn, curr);
				getitemfromqueue(readylist, tmp);
				enqueue(tmp, readylist);

				pid32 tmp_curr = queuehead(readylist);
				kprintf("AFTER REMOVE/ENQUEUE~~~~~");
				while (queuetab[tmp_curr].qnext != EMPTY) {
					pid32 a = queuetab[tmp_curr].qnext;
					struct procent *b = &proctab[a];
					//char *c = b->prgname;	
					char *d = b->prname;
					kprintf("_%s_", d);
					tmp_curr = queuetab[tmp_curr].qnext;

				}
					
				cnt ++;
			}
			curr = queuetab[curr].qnext;
		}*/

		while (cnt < num) {
			curr = queuehead(readylist);
			while (queuetab[curr].qnext != EMPTY) {
				tmp = queuetab[curr].qnext;
				tmpp = &proctab[tmp];
				tmpgn = tmpp->prgname;
				tmpn = tmpp->prname; 

				if(strncmp(tmpgn, oldprocgn, 20) == 0) {
					//kprintf("_FOUND(%s)WITH(%d)_", tmpn, curr);
					getitemfromqueue(readylist, tmp);
					enqueue(tmp, readylist);

					/*
					pid32 tmp_curr = queuehead(readylist);
					kprintf("AFTER REMOVE/ENQUEUE~~~~~");
					while (queuetab[tmp_curr].qnext != EMPTY) {
						pid32 a = queuetab[tmp_curr].qnext;
						struct procent *b = &proctab[a];
						char *c = b->prgname;	
						char *d = b->prname;
						kprintf("_%s_", d);
						tmp_curr = queuetab[tmp_curr].qnext;

					}
						*/
					cnt ++;
					break;

				}
				curr = queuetab[curr].qnext;

			}
		}

		//kprintf("_cnt=%d_", cnt);
	}

	/*
	curr = queuehead(readylist);
	kprintf("\nAFTER: ");
	while (queuetab[curr].qnext != EMPTY) {
		tmp = queuetab[curr].qnext;
		tmpp = &proctab[tmp];
		tmpn = tmpp->prname;
		kprintf("(%s)", tmpn);
		curr = queuetab[curr].qnext;
	}
	kprintf("\n");
	*/		
		

	currpid = getfirst(readylist);
}
