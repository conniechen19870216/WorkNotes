/*
 * =====================================================================================
 *
 *       Filename:  id.c
 *
 *    Description:  distinct id
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

void print_ids(char *str)
{
	pid_t pid;
	pthread_t tid;
	pid = getpid();
	tid = pthread_self();

	printf("%s\n pid: %u tid: %u (0x%x)\n",
			str,
			(unsigned int)pid,
			(unsigned int)tid,
			(unsigned int)tid);
}

void *thread()
{
	print_ids("new thread:");

	pthread_exit((void *)1);
}

int main()
{
	pthread_t main_tid;
	int err;
	void *tret;
	
	err = pthread_create(&main_tid, NULL, thread, NULL);
	if (err != 0)
	{
		printf("thread create error: %s\n", strerror(err));
        exit(-1);	
	}

	printf("main thread:\n pid: %u tid: %u (0x%x)\n",
			(unsigned int)getpid(),
			(unsigned int)pthread_self(),
			(unsigned int)pthread_self());

	print_ids("main thread again:");

	err = pthread_join(main_tid, &tret);
	if (err != 0)
	{
		printf("can not join thread: %s\n", strerror(err));
        exit(-1);	
	}
    printf("thread exit code is %d\n", (int)tret);

	return 0;
}
