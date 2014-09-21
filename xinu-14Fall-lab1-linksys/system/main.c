/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>

/************************************************************************/
/*									*/
/* main - main program for testing Xinu					*/
/*									*/
/************************************************************************/

int main(int argc, char **argv)
{
	kprintf("Hello World!\n");

	//starvation();
	
	rr_test();
	while (1) {
		sleep(1);
	}

	return OK;
}

void starvation() {
	//resume( create(prGen, 1024, 30, "proc 1", 1, 'A') );
	//resume( create(prGen, 1024, 20, "proc 2", 1, 'B') );
}

void rr_test() {
	ROUND_ROBIN = 1;

	pid32 pra = create(prGen, 1024, 20, "group 1", "proc 1", 1, 'A');
	pid32 prb = create(prGen, 1024, 25, "group 2", "proc 2", 1, 'B');
	pid32 prc = create(prGen, 1024, 30, "group 1", "proc 3", 1, 'C');
	//pid32 prd = create(prD, 1024, 10, "group 1", "proc 4", 1, 'D');
	pid32 prd = create(prGen, 1024, 10, "group 1", "proc 4", 1, 'D');
	pid32 pre = create(prGen, 1024, 40, "group 3", "proc 5", 1, 'E');	
	pid32 prf = create(prD, 1024, 30, "group 2", "proc 6", 1, 'F');
 	
	// A, B, E -> C, F, E -> D, B, E -> A, B, E -> C, E

	resume(pra);
	resume(prb);
	resume(prc);
	resume(prd);
	resume(pre);
	resume(prf);
}

int prGen(char c) {
	while (1) {
		kprintf("%c", c);
	}
}

void prD(char c) {
	int i;
	for (i = 0; i < 10; i ++) {
		kprintf("%c", c);
	}	
}

