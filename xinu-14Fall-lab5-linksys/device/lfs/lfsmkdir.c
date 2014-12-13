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
	//struct	lfdir	*dirptr;	/* ptr to in-memory directory 	*/
	//int32		i;
	//struct 	ldentry	*ldptr;		/* ptr to an entry in directory	*/
	//struct 	ldentry	*root;		/* ptr to root					*/
	// but how?
	//struct	lflcblk	*lfptr;		/* ptr to open file table entry	*/
	//bool8		found;
	//int32		retval;

	kprintf("entering lfsmkdir...\r\n");

	char paths[LF_PATH_DEPTH][LF_NAME_LEN];
	
	wait(cblkmutex);
	int depths = pathTokenize(path, paths);
	signal(cblkmutex);

	if (depths == SYSERR) {
		return SYSERR;
	}

	// not allow just create a '/'
	if (depths == 1 && paths[0][0] == '/') {
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
	//wait(Lf_data.lf_mutex);
	wait(cblkmutex);
	// initialize lfltab[Nlfl+1] -> parent direcotry (current)
	// initizlize lfltab[Nlfl] -> grandparent directory
	if (updateDir(paths, depths-1) == SYSERR) {
		//signal(Lf_data.lf_mutex);
		signal(cblkmutex);
		return SYSERR;
	}

	// last entry -> parent directory (current directory)
	struct	lflcblk	*dir_cblk = &lfltab[Nlfl + 1];
	// second last entry -> grandparent directory 
	struct	lflcblk	*pardir_cblk = &lfltab[Nlfl];
	struct 	dentry	dev_ptr;
	struct 	dentry 	par_dev_ptr;

	struct	ldentry	curr_dir_entry;
	struct	ldentry* curr_dir = &curr_dir_entry;

	dev_ptr.dvminor = Nlfl + 1;
	par_dev_ptr.dvminor = Nlfl;

	char 	*curr_name = paths[depths-1];
	uint32	pos = 0;
	bool8 	isInitialized = 0;

	// similar to updateDir
	while (lflRead(&dev_ptr, (char*)curr_dir, sizeof(struct ldentry)) != SYSERR) {
		if (! curr_dir->isOccupied) {
			if (! isInitialized) {
				pos = dir_cblk->lfpos - sizeof(struct ldentry);
				isInitialized = 1;
			}
			continue;
		}

		// when found the current target file/directory
		if (strcmp(curr_dir->ld_name, curr_name) && curr_dir->isOccupied) {
			// return error if it is a file instead of a directory
			if (curr_dir->ld_type == LF_TYPE_DIR) {
				//kprintf("found a file %s which supposed to be a directory \r\n", curr_dir->ld_name);
				//return SYSERR;
			} else {

			}
			dir_cblk->lfstate = LF_FREE;
			pardir_cblk->lfstate = LF_FREE;
			signal(Lf_data.lf_mutex);
			return SYSERR;
		}
	}

	// the file doesnt exist and mode bits = LF_MODE_N
	// create the file
	if (isInitialized) {
		// reuse an existing directory entry to 
		// create a new file
		if (lflSeek(&dev_ptr, pos) == SYSERR) {

		}
	}
	// create the file
	if (createDirEntry(curr_name, LF_TYPE_DIR, curr_dir, isInitialized) == SYSERR) {
		signal(Lf_data.lf_mutex);
		return SYSERR;
	}
	signal(Lf_data.lf_mutex);
	return OK;		
}
