#include <xinu.h>
#include <stdio.h>

#define PRI 30

// blocking message send test

static sid32 sema;
static int score = 0;

static int test0_success = 0;
static int test0_sender_returned = 0;

static void receiver0(void){
  sleep(1);
  kprintf("Test0: call receive()\r\n");
  if( test0_sender_returned ){
    test0_success = 1;
  }else{
    kprintf("Test0: fail: timeout, the first sendb() should return immediately.\r\n");
  }
  uint32 msg = receiveb();
  kprintf("Test0: receive %d\n", msg);
  if( test0_success ){
    score+= 10;
    kprintf("Test0: Pass\r\n");
  }
}
static void sender0(int pid){
  sendb( pid, 1 );
  kprintf("Test0: sender returned\r\n");
  test0_sender_returned = 1;
}

#define TEST1_MESSAGE 5566
static void _receiver1(void){
  uint32 msg = receiveb();
  if( msg == TEST1_MESSAGE ){
    kprintf("Test1: Pass\r\n");
    score+=10;
  }else{
    kprintf("Test1: fail: received incorrect value from the sender. received %d, expect %d\r\n", msg, TEST1_MESSAGE);
  }
  signal( sema );
}
static void _sender1(int pid){
  sendb( pid, TEST1_MESSAGE );
}

#define TEST2_OFFSET 1024
static void _receiver2(void){
  int n;
  for(n = 0; n< 100; n++ ){
    uint32 msg = receiveb();
    if( msg != n+TEST2_OFFSET ){
      kprintf("Test2: fail: did not receive in sending order at %d th message.\r\n", n);
      signal( sema );
      return;
    }
  }
  kprintf("Test2: pass\r\n");
  score+=10;
  signal( sema );
}
static void _sender2(int pid){
  int n;
  for(n = 0; n< 100; n++ ){
    sendb( pid, n+TEST2_OFFSET );
  }
}

void test0(void){
  kprintf("Test 0 -- One sender one receiver. the sender should return immediately. 10 points\r\n");

  int pid = create(receiver0, 1024, PRI, "receiver0", 0);
  resume(pid);
  int senderpid = create(sender0, 1024, PRI, "sender0", 1, pid);
  resume(senderpid);

  sleep(3);
  if( kill( pid ) == OK ){
    kprintf("test 0 failed: receiver did not return\r\n");
  }
  kill( senderpid );
  kprintf("Test0 finished\r\n");
  sleep(1);
}
void test1(void){
  kprintf("Test 1 -- One sender one receiver. one message per sender. verify receiver gets the right message. 10 points\r\n");
  int pid = create(_receiver1, 1024, PRI, "receiver1", 0);
  resume(pid);
  
  resume(create(_sender1, 1024, PRI, "sender1", 1, pid));

  wait( sema );
  kprintf("Test 1 finished\r\n");
  sleep(1);
}
void test2(void){
  kprintf("Test 2 -- One sender one receiver. multiple messages per sender. 10 points\r\n");
  int pid = create(_receiver2, 1024, PRI, "receiver2", 0);
  resume(pid);
  
  resume(create(_sender2, 1024, PRI, "sender2", 1, pid));

  wait( sema );
  kprintf("Test 2 finished\r\n");
  sleep(1);
}

void blocksending_test(void) {

  sema = semcreate(0);

  kprintf("======================Start Testing=====================\r\n");
  test0();

  test1();

  test2();

  kprintf("Total Score: %d\r\n", score );
  kprintf("======================End of Test=====================\r\n");
  return ;
}

// Asynchronous message receive test
umsg32 recvbuf;             
int myrecvhandler(void) {
    kprintf("msg received = %d\n\r", recvbuf);
    return(OK);
}
void test_sender01(pid32 pid, umsg32 msg)	{
	senda(pid, msg);
}
void test_receiver(void)	{
	while (1)
		;
}

void areceive_test01(void)	{
	if (registercb(&recvbuf, myrecvhandler) != OK) {
		kprintf("recv handler registration failed\r\n");
		return ;
	}
	pid32 pid;
	pid=create(test_receiver, 1024, 20, "receive", 0);
	resume(pid);
	resume(create(test_sender01, 1024, 20, "senda", 2, pid, 10));
	sleep(5);
	if (recvbuf != 10) 
	    kprintf("test01 fail: received incorrect value");
	else
	    kprintf("Pass.");
	kill(pid);
}

void areceive_test(void)    {
	kprintf("Test1:\n\r");
	recvbuf = 0;
	areceive_test01();	//1 send, 1 receiver
}

int main(int argc, char **argv)
{
	
	blocksending_test();
	areceive_test();

	return OK;
}


