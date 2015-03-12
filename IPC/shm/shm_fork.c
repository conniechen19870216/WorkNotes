/* function list: shmget, shmat, shmdt, shmctl */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

#define SHARED_MEM_SIZE 4096

int main()
{
	int   shmid;
	pid_t pid;
	char *str;

	shmid = shmget(IPC_PRIVATE, SHARED_MEM_SIZE, IPC_CREAT | 0644);
	if(shmid == -1)
	{
		perror("shmget error");
		exit(1);
	}
	printf("common ipcs after shmget\n");
	system("ipcs -m");

    pid = fork();
	if(pid == -1)
	{
		perror("fork failed");
		exit(1);
	}

	if(pid == 0)
	{
		str = shmat(shmid, NULL, 0);
        if(str == (void *)-1)
		{
			perror("child shmat failed");
		    exit(1);
		}

		strcpy(str, "kavon shared memory\n");
		printf("child ipcs after shmat and strcpy\n");
		system("ipcs -m");

		if(shmdt(str) == -1)
		{
			perror("child shmdt failed");
			exit(1);
		}
		printf("child ipcs after shmdt\n");
		system("ipcs -m");
	}
	else
	{
		sleep(1);

		str = shmat(shmid, NULL, 0);
		if(str == (void *)-1)
		{
			perror("parent shmat failed");
		    exit(1);
		}
		printf("parent ipcs after shmat\n");
		system("ipcs -m");

		printf("%s", str);
		if(shmdt(str) == -1)
		{
			perror("parent shmdt failed");
			exit(1);
		}
		printf("parent ipcs after shmdt\n");
		system("ipcs -m");

		if(shmctl(shmid, IPC_RMID, NULL) == -1)
		{
			perror("parent shmctl failed");
			exit(1);
		}
		printf("parent ipcs after shmctl\n");
		system("ipcs -m");
	}

	return 0;
}
