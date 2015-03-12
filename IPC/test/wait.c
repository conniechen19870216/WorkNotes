/*
 * =====================================================================================
 *
 *       Filename:  wait.c
 *
 *    Description:  test wait function
 *
 *        Version:  1.0
 *        Created:  09/11/2014  2:07:16 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Kavon Ma (mn), kavon.ma@alcatel-lucent.com
 *        Company:  Alcatel-Lucent
 *
 * =====================================================================================
 */

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main ()
{
	int flag = 1;
    pid_t pid, pid_caught;
    switch (fork())
	{
		case -1:
			perror("fork failed");
			break;
		case 0:
			flag = 0;
			printf("This is child process with pid %d, flag = %d\n", getpid(), flag);
			sleep(5);
			break;
		default:
			pid_caught = wait(NULL);
			printf("I caught a child process with pid %d, flag = %d\n", pid_caught, flag);
			break;
    }

	return 0;
}
