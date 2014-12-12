#include <xinu.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


/*
	adding an entry for file/directory in the directory currently 
	loaded in lfltab[Nlfl + 1] and updates the size of the parent 
	directory.
	name = file/directory's name
	type = adding a file or directory ?
	isReplaced = whether we are creating an entry 
				or using en existing isOccupied = FALSE entry
	dir_ent = target file/directory

*/
status createDirEntry(char *name, byte type, struct ldentry *dir_ent, bool8 isReplaced) {
	// last entry is used for modifying parent directory
	struct lflcblk *dir_cblk = &lfltab[Nlfl+1];
	// second last entry is used for modifying grandparent directory
	struct lflcblk *pardir_cblk = &lfltab[Nlfl];
	struct dentry dev_ptr;
	struct dentry par_dev_ptr;

	dev_ptr.dvminor = Nlfl+1;
	par_dev_ptr.dvminor = Nlfl;
		
	// initialize the entry
	dir_ent->ld_size = 0;
	dir_ent->ld_ilist = LF_INULL;
	dir_ent->ld_type = type;
	dir_ent->isOccupied = (bool8)1; // currently occupied
	strcpy(dir_ent->ld_name, name);

	if (lflWrite(&dev_ptr, (char *)dir_ent, sizeof(struct ldentry)) == SYSERR) {
		dir_cblk->lfstate = LF_FREE;
		pardir_cblk->lfstate = LF_FREE;
		return SYSERR;
	}
	// close current directory / parent directory
	if (lfflush(dir_cblk) == SYSERR) {
		dir_cblk->lfstate = LF_FREE;
		pardir_cblk->lfstate = LF_FREE;
		return SYSERR;
	}
	// using an existing entry
	if (isReplaced) {
		dir_cblk->lfstate = LF_FREE;
		pardir_cblk->lfstate = LF_FREE;
		return OK;
	}
	// creaing an new entry
	// increment the size of the parent directory
	// special case: entry being added to the root
	if (pardir_cblk->lfstate == LF_FREE) {
		// one entry being added to the root
		dir_cblk->lfstate = LF_FREE;
		wait(Lf_data.lf_mutex);
		Lf_data.lf_dir.lfd_size += sizeof(struct ldentry);
		Lf_data.lf_dirdirty = TRUE;
		signal(Lf_data.lf_mutex);
		return OK;
	}

	// as we have created an new entry in a non-root directory 
	// need to update in the parent directory to reflect the
	// increased size of this directory
	// the lfPos of the parent directory is pointing to 
	// the loacation just after this directory's entry

	struct ldentry par_dir_ent;
	// move the pisition in grandparent at which 
	// the entry for the parent begins
	lflSeek(&par_dev_ptr, pardir_cblk->lfpos - sizeof(struct ldentry));
	if (lflRead(&par_dev_ptr, (char *)&par_dir_ent, sizeof(struct ldentry)) == SYSERR) {
		dir_cblk->lfstate = LF_FREE;
		pardir_cblk->lfstate = LF_FREE;
		return SYSERR;
	}
	// update that entry
	par_dir_ent.ld_size += sizeof(struct ldentry);
	par_dir_ent.ld_ilist = dir_cblk->lfibnum;
	// to overwrite that entry
	// moveback again since we have read it 
	lflSeek(&par_dev_ptr, pardir_cblk->lfpos - sizeof(struct ldentry));
	// write to grandparent
	if (lflWrite(&par_dev_ptr, (char *)&par_dir_ent, sizeof(struct ldentry)) == SYSERR) {
		dir_cblk->lfstate = LF_FREE;
		pardir_cblk->lfstate = LF_FREE;
		return SYSERR;
	}
	// close the parent directory
	if (lfflush(pardir_cblk) == SYSERR) {
		dir_cblk->lfstate = LF_FREE;
		pardir_cblk->lfstate = LF_FREE;
		return SYSERR;
	}
	dir_cblk->lfstate = LF_FREE;
	pardir_cblk->lfstate = LF_FREE;
	return OK;
}
