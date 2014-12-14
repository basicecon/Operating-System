
#include <xinu.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

bool8 strcmp(char *, char *);
// check whether two file paths are exactly the same (if the target is already open)
int isEqualPath(char first[][LF_NAME_LEN], int depth1, char second[][LF_NAME_LEN], int depth2) {
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
/*
bool8 strcmp(char *first, char *second) {
	while (*first != NULLCH && *first == *second) {
		first ++;
		second ++;
	}
	return (*first == *second) && (*first == NULLCH);
}*/
