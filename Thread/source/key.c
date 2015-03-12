/*
 * =====================================================================================
 *
 *       Filename:  key.c
 *
 *    Description:  Thread specific data
 *
 *        Version:  1.0
 *        Created:  2013年05月25日 22时58分51秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Kavon Ma (mn), kavon.ma@alcatel-lucent.com
 *        Company:  Alcatel-Lucent
 *           Note:  pthread_key_t; pthread_setspecific; pthread_getspecific;
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

pthread_key_t thread_log_key;

void write_to_thread_log(char *message)
{
	FILE *thread_log = (FILE *)pthread_getspecific(thread_log_key);

	fprintf(thread_log, "%s\n", message);
}

void close_thread_log(void *thread_log)
{
	fclose((FILE *)thread_log);
}

void *thread_function(void *arg)
{
	printf("entering %s\n", (char *)arg);
	char thread_log_filename[20];
	FILE *thread_log;

	sprintf(thread_log_filename, "thread%d.log", (unsigned int)pthread_self());
	thread_log = fopen(thread_log_filename, "w");

	pthread_setspecific(thread_log_key, thread_log);
    write_to_thread_log("Kavon Thread");

	printf("exit %s\n", (char *)arg);
	pthread_exit((void *)2);
}

int main()
{
	int i;
	pthread_t threads[2];

    pthread_key_create(&thread_log_key, close_thread_log);

	pthread_create(&threads[0], NULL, thread_function, (void *)"thread 0");
	pthread_create(&threads[1], NULL, thread_function, (void *)"thread 1");

	pthread_join(threads[0], NULL);
	pthread_join(threads[1], NULL);

	return 0;
}
