#include <unistd.h>
#include <stdio.h>

int main()
{
	int i;
	
	for(i = 0; i < 2; i++)
	{
		pid_t fpid = fork();
		if(fpid == 0)
		{
			printf("child i=%d, ppid=%d, pid=%d, fpid=%d\n", i, getppid(), getpid(), fpid);
		}
		else
		{
			printf("parent i=%d, ppid=%d, pid=%d, fpid=%d\n", i, getppid(), getpid(), fpid);
		}
	}

	sleep(1); /* in case that parent process is dead */

	return 0;
}
