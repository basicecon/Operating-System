/* lfsmkdir.c - lfsmkdir */

#include <xinu.h>

/* ----------------------------------------------------------
 * lfsmkdir - Create a directory type file on disk
 * ---------------------------------------------------------
 */
int lfsmkdir (
	struct dentry	*devptr,	/* entry in device switch table	*/
	char *path 					/* path of the file to create 	*/
	)
{
	struct	lfdir	*dirptr;	/* ptr to in-memory directory 	*/
	int32		i;
	struct 	ldentry	*ldptr;		/* ptr to an entry in directory	*/
	struct 	ldentry	*root;		/* ptr to root					*/
	// but how?
	struct	lflcblk	*lfptr;		/* ptr to open file table entry	*/
	bool8		found;
	int32		retval;

	char paths[LF_PATH_DEPTH][LF_NAME_LEN];
	int depths = pathTokenize(path, paths);
	
	if (depths == SYSERR) {
		return SYSERR;
	}

	// not allow just create a '/'
	if (depths = 1 && paths[0][0] = '/') {
		return SYSERR;
	}

	// check length of path -> truncate the file name


	
	// search parent director to see if file exists
	//dirptr = &Lf_data.lf_dir;
	// start from root directory, check each layer whether dir exist
	/* eg: /a/b/c -> depth = 3 
		paths[0] = a
	 	paths[1] = b
	 	paths[2] = c
	*/
	found = FALSE;
	// initialize : get the ptr to root

	for (i = 0; i < depths; i ++) {
		// check whether file exist in current directory
		// if not, return SYSERR
	}




	// go thru all the entries in parent directory to find a free one
	// if none, find an entry at the end
	
	

	return 1;		
}
