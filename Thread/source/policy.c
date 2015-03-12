/*
 * =====================================================================================
 *
 *       Filename:  policy.c
 *
 *    Description:  policy and priority with root permission
 *
 *        Version:  1.0
 *        Created:  2013年05月22日 22时00分13秒
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
#include <string.h>
#include <unistd.h>

void *thread()
{
	int policy, priority;

	pthread_getschedparam(pthread_self(), &policy, (struct sched_param *)&priority);

	printf("child thread: policy = %d, priority = %d\n", policy, priority);

	pthread_exit((void *)1);
}

int main()
{
	pthread_t tid;
	pthread_attr_t attr;
	int err;
	void *tret;
	int priority = 8;
	
	pthread_attr_init(&attr);
	pthread_attr_setschedpolicy(&attr, SCHED_RR);
	pthread_attr_setschedparam(&attr, (struct sched_param *)&priority);
	pthread_attr_setinheritsched(&attr, PTHREAD_INHERIT_SCHED);
	//pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);

	err = pthread_create(&tid, &attr, thread, NULL);
	if (err != 0)
	{
		printf("thread create error: %s\n", strerror(err));
        exit(-1);	
	}

	err = pthread_join(tid, &tret);
	if (err != 0)
	{
		printf("can not join thread: %s\n", strerror(err));
        exit(-1);	
	}
    printf("thread exit code is %d\n", (int)tret);

	return 0;
}
