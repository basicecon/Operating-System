/* getitem.c - getfirst, getlast, getitem */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  getfirst  -  Remove a process from the front of a queue
 *------------------------------------------------------------------------
 */
pid32	getfirst(
	  qid16		q		/* ID of queue from which to	*/
	)				/* remove a process (assumed	*/
					/* valid with no check)		*/
{
	pid32	head;

	if (isempty(q)) {
		return EMPTY;
	}

	head = queuehead(q);
	return getitem(queuetab[head].qnext);
}

/*------------------------------------------------------------------------
 *  getlast  -  Remove a process from end of queue
 *------------------------------------------------------------------------
 */
pid32	getlast(
	  qid16		q		/* ID of queue from which to	*/
	)				/* remove a process (assumed	*/
					/* valid with no check)		*/
{
	pid32 tail;

	if (isempty(q)) {
		return EMPTY;
	}

	tail = queuetail(q);
	return getitem(queuetab[tail].qprev);
}

/*------------------------------------------------------------------------
 *  getitem  -  Remove a process from an arbitrary point in a queue
 *------------------------------------------------------------------------
 */

pid32 getitemfromqueue(qid16 q, pid32 p) {
	pid32 head, tail, curr;
	if (isempty(q)) {
		return EMPTY;
	}
	head = queuehead(q);
	tail = queuetail(q);
	if (p == head) {
		return getfirst(q);
	}
	if (p == tail) {
		return getlast(q);
	}
		

	curr = queuetab[head].qnext;
	while (curr != queuetab[tail].qprev) {
		if (curr == p) {
			return getitem(p);
		}
		curr = queuetab[curr].qnext;
	}
	return p;
}

pid32	getitem(
	  pid32		pid		/* ID of process to remove	*/
	)
{
	pid32	prev, next;

	next = queuetab[pid].qnext;	/* following node in list	*/
	prev = queuetab[pid].qprev;	/* previous node in list	*/
	queuetab[prev].qnext = next;
	queuetab[next].qprev = prev;
	return pid;
}
