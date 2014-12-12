/* lfscreate.c  -  lfscreate */

#include <xinu.h>

/*------------------------------------------------------------------------
 * lfscreate  --  Create an initially-empty file system on a disk
 *------------------------------------------------------------------------
 */
status	lfscreate (
	  did32		disk,		/* ID of an open disk device	*/
	  ibid32	lfiblks,	/* num. of index blocks on disk	*/
	  uint32	dsiz		/* total size of disk in bytes	*/
	)
{
	uint32	sectors;		/* number of sectors to use	*/
	uint32	ibsectors;		/* number of sectors of i-blocks*/
	uint32	ibpersector;		/* number of i-blocks per sector*/
	struct	lfdir	dir;		/* Buffer to hold the directory	*/
	uint32	dblks;			/* total free data blocks	*/
	struct	lfiblk	iblock;		/* space for one i-block	*/
	struct	lfdbfree dblock;	/* data block on the free list	*/
	dbid32	dbindex;		/* index for data blocks	*/
	int32	retval;			/* return value from func call	*/
	int32	i;			/* loop index			*/

	/* Compute total sectors on disk */

	sectors = dsiz	/ LF_BLKSIZ;	/* truncate to full sector */

	/* Compute number of sectors comprising i-blocks */

	ibpersector = LF_BLKSIZ / sizeof(struct lfiblk);
	ibsectors = (lfiblks+(ibpersector-1)) / ibpersector; /* round up*/
	lfiblks = ibsectors * ibpersector;
	if (ibsectors > sectors/2) {	/* invalid arguments */
		return SYSERR;
	}

	/* Create the sector 0, which holds a dir that contains a list of files */

	memset((char *)&dir, NULLCH, sizeof(struct lfdir));
	//dir.lfd_nfiles = 0;
	dbindex= (dbid32)(ibsectors + 1);
	dir.lfd_dfree = dbindex;
	dir.lfd_ifree = 1; // first i-block point to root
	dblks = sectors - ibsectors - 1;
	
	/* Initialize the root dir */
	/*
	dir.lfd_root.ld_ilist = lfiballoc();
	dir.lfd_ifree = dir.lfd_root.ld_ilist; // pt to root i-block

	dir.lfd_root.ld_size = 0;
	dir.lfd_root.ld_name[0] = '/';
	dir.lfd_root.ld_name[1] = NULLCH; 
	dir.lfd_root.ld_type = LF_TYPE_DIR; // root is a dir
	
	//retval = write(disk,(char *)&dir, LF_AREA_DIR);
	*/

	dir.lfd_ifirst = 0; // list of file names
	dir.lfd_size = 0; // file size

	retval = write(disk,(char *)&dir, LF_AREA_ROOT); // mapping to root area

	if (retval == SYSERR) {
		return SYSERR;
	}

	/* Create list of free i-blocks on disk */

	lfibclear(&iblock, 0);
	// allocate the first i-block to root dir
	iblock.ib_next = LF_INULL;
	lfibput(disk, 0, &iblock);
	// put the rest i-block in free list
	for (i=1; i<lfiblks-1; i++) {
		iblock.ib_next = (ibid32)(i + 1);
		lfibput(disk, i, &iblock);
	}
	iblock.ib_next = LF_INULL;
	lfibput(disk, i, &iblock);

	/* Create list of free data blocks on disk */

	memset((char*)&dblock, NULLCH, LF_BLKSIZ);
	for (i=0; i<dblks-1; i++) {
		dblock.lf_nextdb = dbindex + 1;	
		write(disk, (char *)&dblock, dbindex);
		dbindex++;
	}
	dblock.lf_nextdb = LF_DNULL;
	write(disk, (char *)&dblock, dbindex);
	//close(disk);
	return OK;
}
