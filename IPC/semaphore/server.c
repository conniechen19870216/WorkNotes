#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

int semid;

typedef struct people{
       char name[10];
       int age;
} People;

void p()
{
       struct sembuf sem_p;
       sem_p.sem_num=0;
       sem_p.sem_op=-1;
       if(semop(semid,&sem_p,1)==-1)
       printf("p operation is fail\n");             
}

void v()
{
       struct sembuf sem_v;
       sem_v.sem_num=0;
       sem_v.sem_op=1;
       if(semop(semid,&sem_v,1)==-1)
       printf("v operation is fail\n");
}

int main()
{
        int shmid;

        semid=semget((key_t)1234,1,0666|IPC_CREAT);
        if(semid==-1)
        printf("creat sem is fail\n");

        shmid=shmget((key_t)5678,4096,0666|IPC_CREAT);
        if(shmid==-1)
        printf("creat shm is fail\n");

        union semun{
               int val;
               struct semid_ds *buf;
               ushort *array;
        }sem_u;

        sem_u.val=1;
        semctl(semid,0,SETVAL,sem_u);


        People *addr;
		addr = (People *)shmat(shmid,NULL,0);
        if(addr == (void *)-1)
        printf("shm shmat is fail\n");

        p();
        addr->age=10;
        strcpy(addr->name, "xiaoming");
		sleep(10);
        v();

        if(shmdt(addr)==-1)
        printf("shmdt is fail\n");    
}
