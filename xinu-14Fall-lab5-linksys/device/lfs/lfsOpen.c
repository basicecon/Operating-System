/* lfsOpen.c  -  lfsOpen */

#include <xinu.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
/*------------------------------------------------------------------------
 * lfsOpen - open a file and allocate a local file pseudo-device
 *------------------------------------------------------------------------
 */

devcall	lfsOpen (
	 struct	dentry	*devptr,	/* entry in device switch table	*/
	 char	*name,			/* name of file to open		*/
	 char	*mode			/* mode chars: 'r' 'w' 'o' 'n'	*/
	)
{
	kprintf("entering lfsOpen\r\n");
	//struct	lfdir	*dirptr;	/* ptr to in-memory directory	*/
	char		*from;				/* ptrs used during copy		*/
	//char		*nam, *cmp;			/* ptrs used during comparison	*/
	int32		i;					/* general loop index			*/
	did32		lfnext;				/* minor number of an unused	*/
									/*    file pseudo-device		*/
	//struct	ldentry	*ldptr;		/* ptr to an entry in directory	*/
	struct		lflcblk	*lfptr;		/* ptr to open file table entry	*/
	//bool8		found;				/* was the name found?			*/
	//int32		retval;				/* value returned from function	*/
	int32		mbits;				/* mode bits					*/

/*
	char	**paths = (char **)malloc(LF_PATH_DEPTH*sizeof(char *));
	for (i = 0; i < LF_PATH_DEPTH; i ++) {
		paths[i] = (char *)malloc(LF_NAME_LEN);
	}
*/
	char paths[LF_PATH_DEPTH][LF_NAME_LEN];
	//char	*path = name;		/* complete path of file to open*/
	//int 	depth = pathTokenize(path, (char **)paths);
	
	// hard code
	// "/a/b/c"
	paths[0][0] = '/'; // root directory
	paths[0][1] = '\0';
	paths[1][0] = 'a';	// grandparent directory
	paths[1][1] = '\0';
	paths[2][0] = 'b';  // parent directory
	paths[2][1] = '\0';
	paths[3][0] = 'c';  // current directory
	paths[3][1] = '\0';
	int depth = 4;
	char *tmp_path = "\\a\\b\\c";

	// is 'path' valid?
	if (depth == SYSERR) {
		return SYSERR;
	}
	if (depth == 1 && paths[0][0] == '/') {
		kprintf("you cannot open an root directory\r\n");
		return SYSERR;
	}

	/* Check length of name file (leaving space for NULLCH */
	from = name;
	for (i=0; i< LF_NAME_LEN; i++) {
		if (*from++ == NULLCH) {
			break;
		}
	}
	if (i >= LF_NAME_LEN) {		/* name is too long */
		return SYSERR;
	}

	/* Parse mode argument and convert to binary */

	mbits = lfgetmode(mode);
	if (mbits == SYSERR) {
		return SYSERR;
	}

	/* If named file is already open, return SYSERR */

	lfnext = SYSERR;
	wait(Lf_data.lf_mutex);
	// file is alreayd open
	// need implement isFileOpen
	if (isFileOpen(paths, depth, &lfnext) == TRUE) {
		signal(Lf_data.lf_mutex);
		return SYSERR;
	} else {
		kprintf("no such open file \"%s\" exists, and found a free slot at position = %d\r\n", tmp_path, (int)lfnext);
	}
	// no slave devices are available
	if (lfnext == SYSERR) {
		signal(Lf_data.lf_mutex);
		return SYSERR;
	}

	struct ldentry fileInfo;
	signal(Lf_data.lf_mutex);
	// Initialize lfltab[Nlfl+1] and lfltab[Nlfl] 
	// to parent(current directory not current target) and grandparent of the target file
	if (updateDir(paths, depth) == SYSERR) {
		signal(Lf_data.lf_mutex);
		return SYSERR;
	}
	// either create a file or open an already existing file
	if (lfsOpenHelper(paths[depth-1], &fileInfo, mbits) == SYSERR) {
		signal(Lf_data.lf_mutex);
		return SYSERR;
	}
	/*
	for (i = 0; i < depth; i ++) {
		char *tmp_dir = paths[i];
		// check tmp_dir existance
		if (i == 0 && tmp_dir[0] == '/' && tmp_dir[1] == '\0') {
			// root directory
			//dirptr = 
			continue;
		}
		if (// exist) {
			//dirptr = 
		} else {
			return SYSERR;
		}
	} */

	/* Initialize the local file pseudo-device */

	lfptr = &lfltab[lfnext];
	lfptr->lfstate = LF_USED;
	lfptr->lfmode = mbits & LF_MODE_RW;

	/* File starts at position 0 */

	lfptr->lfpos     = 0;

	/* Neither index block nor data block are initially valid	*/

	lfptr->lfinum    = LF_INULL;
	lfptr->lfdnum    = LF_DNULL;

	/* Initialize byte pointer to address beyond the end of the	*/
	/*	buffer (i.e., invalid pointer triggers setup)		*/

	lfptr->lfbyte = &lfptr->lfdblock[LF_BLKSIZ];
	lfptr->lfibdirty = FALSE;
	lfptr->lfdbdirty = FALSE;

	lfptr->lfsize = fileInfo.ld_size;
	lfptr->lfibnum = fileInfo.ld_ilist;
	memcpy(lfptr->lfpath,paths,LF_NAME_LEN * LF_PATH_DEPTH);
	lfptr->lfdepth = depth;

	signal(Lf_data.lf_mutex);

	return lfptr->lfdev;
}



