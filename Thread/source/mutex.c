/*
 * =====================================================================================
 *
 *       Filename:  mutex.c
 *
 *    Description:  pthread mutex
 *
 *        Version:  1.0
 *        Created:  2013年05月23日 22时51分24秒
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
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t count_lock;
pthread_cond_t  count_nonzero;

int count = 0;

void *decrement()
{
	printf("count (decrement) is %d\n", count);
	pthread_mutex_lock(&count_lock);

	if(count == 0)
	{
		printf("Here is Kavon!\n");
		pthread_cond_wait(&count_nonzero, &count_lock);
		printf("There is Colin!\n");
	}

	printf("count--(before): %d\n", count);
	count--;
	printf("count--(after): %d\n", count);

	pthread_mutex_unlock(&count_lock);

	pthread_exit((void *)1);
}

void *increment()
{
	printf("count (increment) is %d\n", count);
	pthread_mutex_lock(&count_lock);     /* necessary to lock in case of dead lock */

	if(count == 0)
	{
		pthread_cond_signal(&count_nonzero);
	}

    printf("count++(before): %d\n", count);
    count++;
    printf("count++(after): %d\n", count);
    
    pthread_mutex_unlock(&count_lock);   /* necessary to unlock, or thread will be locked */
    
    pthread_exit((void *)2);
}

int main()
{
    pthread_t tid1, tid2;
	int err;

	pthread_mutex_init(&count_lock, NULL);      /* count is not directly related with count_lock */
	pthread_cond_init(&count_nonzero, NULL);
    
	printf("count is %d\n", count);

	err = pthread_create(&tid1, NULL, decrement, NULL);
	if (err != 0)
	{
		printf("can not create thread 1: %s\n", strerror(err));
		exit(-1);
	}

	sleep(1);

	err = pthread_create(&tid2, NULL, increment, NULL);
	if (err != 0)
	{
		printf("can not create thread 2: %s\n", strerror(err));
		exit(-1);
	}

	pthread_join(tid1, NULL);
	/* execute uninterrupted */
	while (count != 5)
	{
		printf("count (main) is %d\n", count);
		sleep(1);
		count++;
	}

	return 0;
}
