#include <xinu.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


int lfsOpenHelper(char *fileName, struct ldentry *dir_ent, int32 mbits)
{
	// last entry -> current directory the file is being created
	struct lflcblk *dir_cblk = &lfltab[Nlfl+1];
	// second entry -> parent of parent directory 
	struct lflcblk *pardir_cblk = &lfltab[Nlfl];
	struct dentry devptr;
	struct dentry pardevptr;
	devptr.dvminor = Nlfl + 1;
	pardevptr.dvminor = Nlfl;
	uint32 pos = 0;
	bool8 isInitialized = 0;

	// keep reading on parent directory unless find a match
	int readcnt = 0;
	while ((readcnt = lflRead(&devptr, (char *)dir_ent, sizeof(struct ldentry))) != SYSERR) {
		kprintf("lfsOpenHelper: reading name %s comparing with %s\r\n", dir_ent->ld_name, fileName);

		// if found a deleted entry -> reuse it 
		// when we create a new file
		if (readcnt != sizeof(struct ldentry)) {
			break;
		}
		if (!dir_ent->isOccupied) {
			if (!isInitialized) {
				pos = dir_cblk->lfpos - sizeof(struct ldentry);
				isInitialized = 1;
			}
			continue;
		}
		// there is a match
		if (strcmp(dir_ent->ld_name, fileName) && dir_ent->isOccupied) {
			// its a directory
			if (dir_ent->ld_type == LF_TYPE_DIR) {
				// try to open a directory?
				dir_cblk->lfstate = LF_FREE;
				pardir_cblk->lfstate = LF_FREE;
				return SYSERR;
			}
			if (mbits & LF_MODE_N) {
				// file not exist?
				dir_cblk->lfstate = LF_FREE;
				pardir_cblk->lfstate = LF_FREE;
				return SYSERR;
			}
			dir_cblk->lfstate = LF_FREE;
			pardir_cblk->lfstate = LF_FREE;
			return OK;
		}
	}
	// the file doesnt exist and mode bits = LF_MODE_O
	if (mbits & LF_MODE_O) {
		dir_cblk->lfstate = LF_FREE;
		pardir_cblk->lfstate = LF_FREE;
		return SYSERR;
	}
	// the file doesnt exist and mode bits = LF_MODE_N
	// create the file
	if (isInitialized) {
		// reuse an existing directory entry to 
		// create a new file
		if (lflSeek(&devptr, pos) == SYSERR) {

		}
	}
	// create the file
	if (createDirEntry(fileName, LF_TYPE_FILE, dir_ent, isInitialized)) {
		dir_cblk->lfstate = LF_FREE;
		pardir_cblk->lfstate = LF_FREE;
		return SYSERR;
	}
	return OK;


}
