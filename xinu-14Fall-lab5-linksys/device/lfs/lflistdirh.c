#include <xinu.h>

/* ----------------------------------------------------------
 * lflistdirh - List contents of a directory
 * ---------------------------------------------------------
 */

int lflistdirh(
		did32		disk,		/* ID of an open disk device	*/
		char 		*path 		/* path */
		)
{
	//kprintf("entering lflistdirh...\r\n");

	char paths[LF_PATH_DEPTH][LF_NAME_LEN];
	
	//wait(cblkmutex);
	int depths = pathTokenize(path, paths);
	//signal(cblkmutex);

	if (depths == SYSERR) {
		return SYSERR;
	}

	wait(cblkmutex);
	// initialize lfltab[Nlfl+1] -> parent direcotry (current)
	// initizlize lfltab[Nlfl] -> grandparent directory
	if (updateDir(paths, depths-1) == SYSERR) {
		//signal(Lf_data.lf_mutex);
		//kprintf("error when allocating directory\r\n");
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
	int 	found = 0; // if found the target directory

	// similar to updateDir
	
	// case1: (target is root)
	/*
	if (depths == 1 && paths[0][0] == '/' && paths[0][1] == '\0') {
		kprintf("target is root\r\n");
	}*/

	int readcnt = 0;
	// case2: (target is not root) find the entry of the target directory from its parent directory	
	while ((readcnt = lflRead(&dev_ptr, (char*)curr_dir, sizeof(struct ldentry))) != SYSERR) {
		//
		//kprintf("readcnt = %d\r\n", readcnt);
		// if the target is root
		if (depths == 1 && paths[0][0] == '/' && paths[0][1] == '\0') {
			if (readcnt != sizeof(struct ldentry) || !curr_dir->isOccupied) {
				break;
			}
			found = 1;
			//kprintf("LS:\r\n");
			kprintf("%s\r\n", curr_dir->ld_name);
			//kprintf("\r\n");

		} else {
			// not root: go to parent and check curr's existance

			if (readcnt != sizeof(struct ldentry)) {
				//kprintf("readcnt != sizeof(struct ldentry)\r\n");
				break;
			}
			if (! curr_dir->isOccupied) {
				if (! isInitialized) {
					pos = dir_cblk->lfpos - sizeof(struct ldentry);
					isInitialized = 1;
				}
				continue;
			}

			// when found the current target file/directory
			if (strcmp(curr_dir->ld_name, curr_name) && curr_dir->isOccupied) {
					
				// make sure it is a directory
				//kprintf("compare: %s == %s\r\n", curr_dir->ld_name, curr_name);
				if (curr_dir->ld_type == LF_TYPE_FILE) {
					//kprintf("target is a file -> %s\r\n", curr_dir->ld_name);
					
					dir_cblk->lfstate = LF_FREE;
					pardir_cblk->lfstate = LF_FREE;
					signal(cblkmutex);
					return SYSERR;

				} else {
					//kprintf("target is a directory -> %s\r\n", curr_dir->ld_name);
				}
				// target is a directory: do the list work
				found = 1;
				
				// TODO
				// find the children of the target directory
				int tmpcnt = 0;
				while ((tmpcnt = lflRead(&dev_ptr, (char*)curr_dir, sizeof(struct ldentry))) != SYSERR) {
					if (tmpcnt != sizeof(struct ldentry)) {
						break;
					}
					if (! curr_dir->isOccupied) {
						continue;
					}
					//kprintf("LS:\r\n");
					//kprintf("%s\r\n", curr_dir->ld_name);
					//kprintf("\r\n");
						
				}

				dir_cblk->lfstate = LF_FREE;
				pardir_cblk->lfstate = LF_FREE;
				signal(cblkmutex);
				break;
			}
		}
	}

	if(!found) // didnt find target
	{
		//kprintf("no such directory \r\n");

		dir_cblk->lfstate = LF_FREE;
		pardir_cblk->lfstate = LF_FREE;

		signal(cblkmutex);
		return SYSERR;
	}

	signal(cblkmutex);
	return OK;		
}
