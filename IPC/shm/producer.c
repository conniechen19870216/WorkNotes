#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include "shared.h"

int main()
{
	int  running = 1;
	int  shmid;
	char *str;
	Exchange *exchg;

	shmid = shmget(IPC_KEY, SHARED_MEM_SIZE, IPC_CREAT | 0644);
	if(shmid == -1)
	{
		perror("shmget error");
		exit(1);
	}
	printf("producer ipcs after shmget\n");
	system("ipcs -m");

	exchg = (Exchange *)shmat(shmid, NULL, 0);
    if(exchg == (void *)-1)
	{
		perror("shmat failed");
	    exit(1);
	}

	printf("producer ipcs after shmat\n");
	system("ipcs -m");
    printf("initially, exchg->flag = %d\n", exchg->flag);

	while(running)
	{
		while(exchg->flag == 1)
		{
			sleep(2);
			printf("waiting for the client\n");
		}

		scanf("%s", str);
		strcpy(exchg->text, str);
		exchg->flag = 1;

		if(strncmp(exchg->text, "end", 3) == 0)
		{
			running = 0;
		}
	}

	if(shmdt(exchg) == -1)
	{
		perror("producer shmdt failed");
		exit(1);
	}
	printf("producer ipcs after shmdt\n");
	system("ipcs -m");

	return 0;
}
