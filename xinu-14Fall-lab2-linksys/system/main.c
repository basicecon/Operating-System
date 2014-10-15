/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>
#include <stdlib.h>
/************************************************************************/
/*									*/
/* main - main program for testing Xinu					*/
/*									*/
/************************************************************************/

int main(int argc, char **argv)
{
    /*
	pipe_test();	
	return OK;
	*/
	
	pipid32 pip1 = pipcreate();

    pid32 pr01 = create( write_checker, 2000, 30, "Pipe writer", 1, pip1);
    pid32 pr02 = create( read_checker, 2000, 30, "Pipe reader", 1, pip1);

    pipconnect(pip1, pr01, pr02);

    resume(pr01);
    resume(pr02);

    sleep(10);

    kill(pr01);
    kill(pr02);
    
    pipdisconnect(pip1);
    pipdelete(pip1);
    
    return 0;
}

void write_checker(pipid32 wpip){
    char buf[1000];
    
    int i;
    for(i=0; i<1000; i++) {
        buf[i] = 'a';
    }
    sleep(1);
    int wlen = 0;
    while(wlen<1000){
        kprintf("Process try to write to pip %d\n\r", wpip);

        int len = pipwrite_block(wpip, &buf[wlen], 100);
        kprintf("Process have write %d bytes to pip %d\n\r", len, wpip);
        wlen += len;
    }

    while(1){
        sleep(1);
    }
}

void read_checker(pipid32 rpip){
    char buf[1000];
    sleep(1);
    while(1){
        kprintf("Process try to read from pip %d\n\r", rpip);

        int len = pipread_block(rpip, buf, 500);
        kprintf("Process have read %d bytes from pip %d\n\r", len, rpip);
    
    }

    while(1){
        sleep(1);
    }
}


void pipe_test() {
	/* pipe */
	pipid32 pipid;
	if ( (pipid = pipcreate()) == SYSERR ) {
		kprintf("ERROR WHEN CREATING PIPE!\n");
		return;
	} else {
		//kprintf("SUCCESSFULLY CREATED PIPE\n");
		kprintf("PIPE ID: %d...", pipid);
	}
	/* writer process */
	
	pid32 producer;
	if ( (producer = create(produce, 1024, 30, "producing", 2, pipid, 1)) == SYSERR ) {
		kprintf("ERROR WHEN CREATING PRODUCER!\n");
		return;
	} else {
		//kprintf("SUCCESSFULLY CREATED WRITER\n");
		kprintf("WRITER ID: %d...", producer);
	}
	/* reader process */
	pid32 consumer;
	if ( (consumer = create(consume, 1024, 30, "consuming", 2, pipid, 1)) == SYSERR ) {
		kprintf("ERROR WHEN CREATING CONSUMER!\n");	
		return;
	} else {
		//kprintf("SUCCESSFULLY CREATED READER\n");
		kprintf("READER ID: %d...", consumer);
	}
	/* connecting */
	
	if ( (pipconnect(pipid, producer, consumer) == SYSERR) ) {
		kprintf("ERROR WHEN CONNECTING!\n");
		return;
	} else {
		kprintf("SUCCESSFULLY CONNECTED...");
	}

	kprintf("Resuming writing...");
	resume(producer);
	kprintf("Resuming reading...");
	resume(consumer);

/*
    kill(consumer);
    kill(producer);
    
    pipdisconnect(pipid);
    pipdelete(pipid);
*/

}

void produce(pipid32 pipid, int block) {
    
	char *message = "aaaaaaaaaa\0";
	int len = 11;
	
	kprintf("Start writing...");
	int written_bytes = 0;
	if (block == 0) {
	    written_bytes = pipwrite_nonblock(pipid, message, len);
	} else {
	    written_bytes = pipwrite_block(pipid, message, len);
	}
	
	if ( written_bytes == SYSERR ) {
		kprintf("ERROR WHEN WRITING TO PIPE!...");
	} else {
		kprintf("WRITTEN %d BYTES!...", written_bytes);
	}
	kprintf("End writing...");
	return;	
}

void consume(pipid32 pipid, int block) {
	char *message = "\0\0\0\0\0\0\0\0\0\0\0";
	int len = 11;
	
	kprintf("Start reading...");
	int read_bytes = 0;
	
	if (block == 0) {
	    read_bytes = pipread_nonblock(pipid, message, len);
	} else {
	    read_bytes = pipread_block(pipid, message, len);
	}
	
	if ( read_bytes == SYSERR ) {
		kprintf("ERROR WHEN READING FROM PIPE!...");
	} else {
		kprintf("READ %d BYTES!...", read_bytes);
	}
	return;
}


