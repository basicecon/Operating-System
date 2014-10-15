/* pipcreate.c - pipcreate, newpipid */

#include <xinu.h>
#include <mips.h>

static pipid32	newpipid(void);

syscall pipcreate(void) {
//pipid32 pipcreate(void) {
	intmask mask;
	struct pipent *piptr;	/* ptr to pipe' table entry */
	pipid32 pipid;		/* ID of newly created pipe */

	mask = disable();

	if ( (((int32)(pipid = newpipid())) == (int32) SYSERR) ) {
		restore(mask);
		return SYSERR;
	}

	pipcount ++;
	piptr = &piptab[pipid];

	/* Initialize pipe table entry for new pipe */

	piptr->pipstate = PIPE_USED;	/* a pipe has been successfully created */
	piptr->owner = (pid32) getpid();

	restore(mask);
	return pipid;
}

local pipid32 newpipid(void) {
	uint32 i;
	static pipid32 nextpipid = 1;

	/* Check all NPIPE slots */

	for (i = 0; i < NPIPE; i ++) {
		nextpipid %= NPIPE;
		if (piptab[nextpipid].pipstate == PIPE_FREE) {
			return nextpipid++;
		} else {
			nextpipid++;
		}
	}
	return (pipid32) SYSERR;
}

