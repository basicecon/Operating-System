#include <xinu.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// check whether the file indentified by paths is open
// at the meantime, initialize firstfreeslot to a free entry in lfltab

bool8 isFileOpen(char paths[][LF_NAME_LEN], int depth, int* firstfreeslot) {
	struct lflcblk *lfptr;
	int i = 0;
	//kprintf("entering isFileOpen...\r\n");
	//kprintf("Nlfl = %d\r\n", Nlfl);

	for (i = 0; i < Nlfl; i ++) {
		lfptr = &lfltab[i];
		if (lfptr->lfstate == LF_FREE) {
			// first free slot in lfltab
			if (*firstfreeslot == SYSERR) {
				//kprintf("find the first free slot in lfltab at position %d\r\n", i);
				*firstfreeslot = i;
			} else {
				//kprintf("other free slots at position = %d\r\n", i);	
			}
		} else { // the slot is currently occupied by an open file
			// compare name of paths and the name of open file
			//kprintf("compare \"%s\" and \"%s\"\r\n", lfptr->lfpath, paths);
			if (isEqualPath(lfptr->lfpath, lfptr->lfdepth, paths, depth)) {
				// file is already open
				signal(Lf_data.lf_mutex);
				return 1;
			}
		}
	}
	return 0;
}
