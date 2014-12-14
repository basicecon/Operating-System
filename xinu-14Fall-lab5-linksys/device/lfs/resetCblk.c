#include <xinu.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// initialize the two allocated control block 

void resetCblk(struct lflcblk *dir_cblk) {
	dir_cblk->lfstate = LF_FREE;
	dir_cblk->lfpos = 0;

	dir_cblk->lfinum = LF_INULL;
	dir_cblk->lfdnum = LF_DNULL;

	dir_cblk->lfsize = 0; // 0?
	dir_cblk->lffibnum = LF_INULL;
	
	dir_cblk->lfbyte = &dir_cblk->lfdblock[LF_BLKSIZ];

	dir_cblk->lfibdirty = FALSE;
	dir_cblk->lfdbdirty = FALSE;
	
	memset((char *)dir_cblk->lfpath, NULLCH, LF_PATH_DEPTH*LF_NAME_LEN);
	dir_cblk->lfdepth = 0;
}
