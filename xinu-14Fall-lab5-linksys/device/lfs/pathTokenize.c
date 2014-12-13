/* pathTokenize.c - pathTokenize */

#include <xinu.h>
#include <stdlib.h>

/* ------------------------------------------------------
 * pathTokenize - Split whole path into tokens and return depth
 * eg: "a/b/c/d" -> [a], [b], [c], [d] -> return 3
 * ------------------------------------------------------
 */
void *malloc(unsigned int);

int pathTokenize (
	char*	path,								/* complete path	*/
	char 	paths[][LF_NAME_LEN]				/* separated paths	*/
	) 
{
	kprintf("entering pathTokenize\r\n");
	int curr_depth = 0;

	int i, j = 0;

	//paths = malloc(LF_PATH_DEPTH*sizeof(char *));

	//for (i = 0; i < LF_PATH_DEPTH; i ++) {
	//	paths[i] = (char *)malloc(LF_NAME_LEN);
	//}

	// only contains a sigle char '/'
	if (path[0] == '/' && path[1] == '\0') {
		// segmentation fault
		paths[0][0] = '/';
		paths[0][1] = '\0';
		return 1;
	}
	kprintf("path = %s\r\n", path);
	i = 0;
	while (curr_depth < LF_PATH_DEPTH) {
		//kprintf("while... i = %d\r\n", i);
		if (path[i] == '/') {
			//kprintf(" i = %d\r\n", i);
			i ++;
		}
		for (j = 0; path[i] != NULLCH && path[i] != '/' && j < LF_NAME_LEN - 1; j ++, i ++) {
			//kprintf("i=%d, j=%d\r\n", i, j);
			paths[curr_depth][j] = path[i];
		}
		if (j) {
			if (j >= LF_NAME_LEN - 1) {
				return SYSERR;
			}
			paths[curr_depth][j] = '\0';
			curr_depth ++;
		}
		if (path[i] == NULLCH || (path[i] == '/' && path[i+1] == NULLCH)) {
			break;
		}
	}

	//kprintf("pathTokenize -> depth = %d\r\n", curr_depth);
	//kprintf("%s\r\n", paths[0]);
	return curr_depth;
}
