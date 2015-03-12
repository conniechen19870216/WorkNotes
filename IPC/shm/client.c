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
	printf("client ipcs after shmget\n");
	system("ipcs -m");

	exchg = (Exchange *)shmat(shmid, NULL, 0);
    if(exchg == (void *)-1)
	{
		perror("shmat failed");
	    exit(1);
	}

	printf("client ipcs after shmat\n");
	system("ipcs -m");

	while(running)
	{
		if(exchg->flag == 1)
		{
	        printf("client received text is %s\n", exchg->text);
		    exchg->flag = 0;

		    if(strncmp(exchg->text, "end", 3) == 0)
		    {
		    	running = 0;
		    }
		}
	}

	if(shmdt(exchg) == -1)
	{
		perror("client shmdt failed");
		exit(1);
	}
	printf("client ipcs after shmdt\n");
	system("ipcs -m");

	if(shmctl(shmid, IPC_RMID, NULL) == -1)
    {
        perror("producer shmctl failed");
        exit(1);
    }
    printf("producer ipcs after shmctl\n");
    system("ipcs -m");
    
	return 0;
}
