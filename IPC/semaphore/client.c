#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
 
struct People{
       char name[10];
       int age;
};

int main()
{
        int semid;
        int shmid;

        semid=semget((key_t)1234,0,0666);
        if(semid==-1)
        printf("creat sem is fail\n");

        shmid=shmget((key_t)5678,0,0666);
        if(shmid==-1)
        printf("creat shm is fail\n");

        struct People *addr = (struct People *) shmat(shmid,0,0);
        if(addr==(struct People*)-1)
        printf("shm shmat is fail\n");

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


        p();
        printf("name:%s\n",addr->name);
        printf("age:%d\n",addr->age);
        v();

        if(shmdt(addr)==-1)
        printf("shmdt is fail\n");

        if(semctl(semid,0,IPC_RMID,NULL)==-1)
        printf("semctl delete error\n");
        if(shmctl(shmid,IPC_RMID,NULL)==-1)
        printf("shmctl delete error\n");
}
