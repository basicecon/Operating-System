
#include <xinu.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


// check whether two file paths are exactly the same
bool8 isEqualPath(char first[][LF_NAME_LEN], int depth1, char second[][LF_NAME_LEN], int depth2) {
	int i = 0;
	if (depth1 != depth2) {
		return 0;
	}
	for (i = 0; i < depth1; i ++) {
		if (!strcmp(first[i], second[i])) {
			return 0;
		}
	}
	return 1;
}
