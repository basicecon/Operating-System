/* pipe.h - isbadpipid */

/* Maximum number of pipes in the system */

#ifndef NPIPE 
#define NPIPE		10
#endif 

/* Pipe state constants */

#define PIPE_FREE	0
#define PIPE_USED 	1
#define PIPE_CONNECTED 	2

/* Default fixed-size buffer */

#define PIPE_SIZE	256

/* Inline code to check pipe ID (assumes interrupts are disabled) */

#define isbadpipid(x) 	( ((pipid32)(x) < 0) || \
			  ((pipid32)(x) >= NPIPE) || \
			  (piptab[(x)].pipstate == PIPE_FREE) )


/* Definition of the pipe table (mulitple of 32 bits) */

struct pipent {				/* entry in the pipe table */
	uint16 	pipstate;		/* pipe state */
	pid32	owner;  		/* who creates the pipe */
	pid32 	writer; 		/* who can write to the pipe */
	pid32 	reader; 		/* who can read from the pipe */
	byte 	buffer[PIPE_SIZE];	/* pipe buffer */
	sid32 	emptySem;		/* read will call wait(emptySem) and 
					   it will block until 
					   write calls post(emptySem) */
	sid32 	fullSem;		/* write will call wait(fullSem) and
				  	   it will block until
					   read calls post(fullSem) */
	int32 	start;			/* start index of buffer 
					   buffer[start] contains data */
	int32	end;			/* end index of buffer 
				 	   buffer[end] doesnot contain data */ 	
};

extern	struct	pipent	piptab[];
extern	int32 	pipcount;		/* currently active pipes */

