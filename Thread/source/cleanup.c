/*
 * =====================================================================================
 *
 *       Filename:  cleanup.c
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
 *           Note:  (1) pthread_exit (2) pthread_cancel (3) pthread_cleanup_pop(nonzero)
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void cleanup(void *arg)
{
	printf("cleanup: %s\n", (char *)arg);
}

void *thread1(void *arg)
{
	printf("enter thread 1\n");

	pthread_cleanup_push(cleanup, (void *)"thread 1 first");
	pthread_cleanup_push(cleanup, (void *)"thread 1 second");

	if (arg)
		return((void *)1);

	pthread_cleanup_pop(0);
	pthread_cleanup_pop(0);

    return((void *)2);
}

void *thread2(void *arg)
{
	printf("enter thread 2\n");
	pthread_cleanup_push(cleanup, (void *)"thread 2 first");
	pthread_cleanup_push(cleanup, (void *)"thread 2 second");

	if (arg)
		pthread_exit((void *)3);

	pthread_cleanup_pop(0);
	pthread_cleanup_pop(0);

    pthread_exit((void *)4);
}

int main()
{
	pthread_t tid1, tid2;
	void *tret;

	pthread_create(&tid1, NULL, thread1, (void *)1);
	pthread_create(&tid2, NULL, thread2, (void *)0);

	pthread_join(tid1, &tret);
	printf("thread 1 exit code is %d\n", (int)tret);

	pthread_join(tid2, &tret);
	printf("thread 2 exit code is %d\n", (int)tret);

	return 0;
}

