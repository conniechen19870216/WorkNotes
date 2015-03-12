/*
 * =====================================================================================
 *
 *       Filename:  cancel.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年05月28日 21时50分54秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Kavon Ma (mn), kavon.ma@alcatel-lucent.com
 *        Company:  Alcatel-Lucent
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_t tid1, tid2;

void *thread1()
{
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
	//pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	printf("ready for cancel\n");

	int i;
	for(i=1; i<1e9; i++);
	printf("before cancel\n");
	pthread_testcancel();
	printf("after cancel\n");

    pthread_exit ((void *)1);
}

void *thread2()
{
	pthread_cancel(tid1);
	printf("cancel signal sent\n");
	pthread_exit ((void *)2);
}

int main()
{
	pthread_create(&tid1, NULL, thread1, NULL);
	pthread_create(&tid2, NULL, thread2, NULL);

	pthread_join(tid1, NULL);
	printf("Bye main\n");

	return 0;
}

