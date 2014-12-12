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
	char*	path,				/* complete path	*/
	char 	**paths				/* separated paths	*/
	) 
{
	kprintf("entering pathTokenize\r\n");
	int curr_depth = 0;

	int i, j = 0;
	paths = malloc(LF_PATH_DEPTH*sizeof(char *));

	for (i = 0; i < LF_PATH_DEPTH; i ++) {
		paths[i] = (char *)malloc(LF_NAME_LEN);
	}
	// only contains a sigle char '/'
	if (path[0] == '/' && path[1] == '\0') {
		// segmentation fault
		paths[0][0] = '/';
		paths[0][1] = '\0';
		return 1;
	}
	kprintf("path = %s\r\n", path);
	while (curr_depth < LF_PATH_DEPTH) {

		if (path[i] == '/') {
			i ++;
		}
		for (j = 0; path[i] && path[i] != '/' && j < LF_NAME_LEN - 1; j ++, i ++) {
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
	return curr_depth;
}
