#include <xinu.h>

umsg32 *cbbuff;
int (* cbfunc) (void);

syscall registercb(umsg32 *abuf, int (* func) (void)) {
/*
	proctab[currpid].cbfunc = func;
	proctab[currpid].cbbuff = abuf;
*/
	cbbuff = abuf;
	cbfunc = func;
	return OK;

}
