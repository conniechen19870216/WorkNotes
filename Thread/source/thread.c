/*
 * =====================================================================================
 *
 *       Filename:  thread.c
 *
 *    Description:  1st thread source file
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

int num = 0;

void *add()
{
	printf("entering thread\n");
	int i, tmp;
	for (i=0; i<10; i++)
	{
		tmp=num+1;
		num=tmp;
		printf("add+1, result is %d\n", num);
	}
	pthread_exit ((void *)5);
}

int main()
{
	pthread_t tid;
	int err;
	void *tret;
	
	err = pthread_create(&tid, NULL, add, NULL);
	if (err != 0)
	{
		printf("thread create error: %s\n", strerror(err));
        exit(-1);	
	}

	printf("main thread\n");
	err = pthread_join(tid, &tret);
	if (err != 0)
	{
		printf("can not join thread: %s\n", strerror(err));
        exit(-1);	
	}
    printf("thread exit code is %d\n", (int)tret);

	return 0;
}
