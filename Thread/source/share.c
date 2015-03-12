/*
 * =====================================================================================
 *
 *       Filename:  share.c
 *
 *    Description:  share heap and stack with global variable
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
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

pthread_once_t once = PTHREAD_ONCE_INIT;
char *gstr = NULL;
int  *gnum = NULL;

void once_run()
{
	printf("Once run in thread %u\n", (unsigned int)pthread_self());
}

void *thread1()
{
	printf("entering thread 1\n");
	printf("entering thread %u\n", (unsigned int)pthread_self());
	pthread_once(&once, once_run);

	char *p1 = malloc(1024);
	memset(p1, 0, 1024);
	strcpy(p1, "HelloThread1");
	gstr = p1;
    
	int num1 = 20;
	gnum = &num1;

	sleep(20);
	pthread_exit ((void *)1);
}

void *thread2()
{
	printf("entering thread 2\n");
	printf("entering thread %u\n", (unsigned int)pthread_self());
	pthread_once(&once, once_run);

	/* make sure that thread1 is over at this time */
	sleep(15);
	char *p2 = gstr;
	printf("string from 1st thread is %s\n", p2);

	int *pnum = gnum;
	printf("integer from 1st thread is %d\n", *pnum);

	pthread_exit ((void *)2);
}

int main()
{
	pthread_t tid1, tid2;
	int err;
	void *tret;

	char *p = malloc(1024);
	memset(p, 0, 1024);
	strcpy(p, "HelloKavon");
	gstr = p;
    
	int num = 10;
	gnum = &num;
	
	printf("string from main thread is %s\n", gstr);
	printf("integer from main thread is %d\n", *gnum);

	err = pthread_create(&tid1, NULL, thread1, NULL);
	if (err != 0)
	{
		printf("1st thread create error: %s\n", strerror(err));
        exit(-1);	
	}
	err = pthread_create(&tid2, NULL, thread2, NULL);
	if (err != 0)
	{
		printf("2nd thread create error: %s\n", strerror(err));
        exit(-1);	
	}

	err = pthread_join(tid1, &tret);
	if (err != 0)
	{
		printf("can not join 1st thread: %s\n", strerror(err));
        exit(-1);	
	}
    printf("1st thread exit code is %d\n", (int)tret);
	err = pthread_join(tid2, &tret);
	if (err != 0)
	{
		printf("can not join 2nd thread: %s\n", strerror(err));
        exit(-1);	
	}
    printf("2nd thread exit code is %d\n", (int)tret);

	return 0;
}
