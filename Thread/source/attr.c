/*
 * =====================================================================================
 *
 *       Filename:  attr.c
 *
 *    Description:  1. detach state; 2. scope; 3. policy; 4. priority; 5. inherit
 *
 *        Version:  1.0
 *        Created:  2013年05月21日 17时31分34秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Kavon Ma (mn), kavon.ma@alcatel-lucent.com
 *        Company:  Alcatel-Lucent
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *thread1()
{
	int i;
	for(i=0; i<10; i++)
	{
	    printf("thread 1\n");
	}

	printf("start sleep 2 seconds\n");
	sleep(2);
	printf("already sleep 2 seconds\n");
    pthread_exit ((void *)1);
}

void *thread2()
{
    int i;

	for(i=0; i<3; i++)
	{
	    printf("thread 2\n");
	}

	sleep(3);
	pthread_exit ((void *)2);
}

int main()
{
    pthread_attr_t attr;
	pthread_t tid1, tid2;

	int err;

	pthread_attr_init(&attr);
	pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	
	err = pthread_create(&tid1, &attr, thread1, NULL);
	if (err != 0)
	{
		printf("1st thread create error: %s\n", (char *)strerror(err));
        return(-1);	
	}
	err = pthread_create(&tid2, NULL, thread2, NULL);
	if (err != 0)
	{
		printf("2nd thread create error: %s\n", (char *)strerror(err));
        return(-1);	
	}

	err = pthread_join(tid2, NULL);
	if (err != 0)
	{
		printf("can not join 2nd thread: %s\n", (char *)strerror(err));
        return(-1);	
	}

	printf("Bye main\n");

	return 0;
}
