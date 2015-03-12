#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>

#define NAME_SIZE 128

struct filename{               //定义一个结构体，用于存储文件名称
	char name[NAME_SIZE];
};

void output_info(struct filename *head);           //输出所有信息
void output_mode(mode_t mode);			 //输出文件权限信息
void output_user_group(uid_t uid,gid_t gid);          //输出文件所属用户名和组名
void output_mytime(time_t mytime );                 //输出文件最后一次被修改的时间

int main(int argc,char *argv[])
{
    if (argc < 2)       //判断参数个数，少于两个参数报错
    {
        fprintf(stderr,"usage:%s <pathname>\n",argv[0]);
        exit(EXIT_FAILURE);
    }

    int i;
    struct stat stat_info;             //文件属性

	for (i = 1;i < argc;i ++)          //分别对每一个参数的文件属性进行判断
	{
		if (stat(argv[i],&stat_info) == -1)
		{
			printf("%s is wrong\n",argv[i]);         //参数有错误，报错，判断下一个参数属性
			continue;
		}

		if (S_ISREG(stat_info.st_mode))       //文件属性是文件类型
		{
			struct filename *temp = malloc(sizeof(struct filename));      //申明一个指针，给他分配一个struct filename大小的空间
			if (temp == NULL)
			{
				perror("malloc in regular file\n");
				exit(EXIT_FAILURE);
			}

			memset(temp->name,'\0',NAME_SIZE);        //初始化
			memcpy(temp->name,argv[i],strlen(argv[i]));    //将参数名称拷贝到name中
			output_info(temp);      //输出文件信息
			free(temp);		
		}
		else if (S_ISDIR(stat_info.st_mode))           //文件属性是文件夹
		{
			DIR *dirp = NULL;
			if ((dirp = opendir(argv[i])) == NULL)        //打开文件夹
			{
				perror("opendir");
				exit(EXIT_FAILURE);
			}

			struct dirent *entp;            
			while ((entp = readdir(dirp)) != NULL)        //依次读取文件夹内部的文件
			{
				if (strcmp(entp->d_name,".") == 0 || strcmp(entp->d_name,"..") == 0)   //如果是“.”和“..”文件，跳过
					continue;
				struct filename *temp = malloc(sizeof(struct filename));
				if (temp == NULL)
				{
					perror("malloc in directory");
					exit(EXIT_FAILURE);
				}

				char *name_dir = malloc(strlen(argv[i]) + 1 + strlen(entp->d_name) + 1);    
				sprintf(name_dir,"%s/%s\0",argv[i],entp->d_name);     //将文件夹内文件的路径保存在name_dir中

				memset(temp->name,'\0',NAME_SIZE);
				memcpy(temp->name,name_dir,strlen(argv[i]) + 1 + strlen(entp->d_name) + 1);
				output_info(temp);
				free(temp);
				free(name_dir);
			}
		}
	}

	return 0;
}

void output_info(struct filename *temp)
{
	struct stat mystat;
	if(stat(temp->name,&mystat) == -1)
	{
		perror("mystat temp->name");
		exit(EXIT_FAILURE);
	}

	output_mode(mystat.st_mode);
	printf(" %d",mystat.st_nlink);
	output_user_group(mystat.st_uid,mystat.st_gid);
	printf(" %ld",mystat.st_size );
	output_mytime(mystat.st_mtime);
	printf(" %s\n",temp->name );
}

void output_mode(mode_t mode)        
{
	char type[7] = {'p','c','d','b','-','l','s'};
	int index = (mode >>12 & 0xF) / 2;
	printf("%c",type[index] );

	char *perm[8] = {"---","--x","-w-","-wx","r--","r-x","rw-","rwx"}; //按照000，001，010，011...111排列
	printf("%s",perm[mode >> 6 & 07] ); //root用户权限
	printf("%s",perm[mode >> 3 & 07] );	//组用户权限
	printf("%s",perm[mode >> 0 & 07] );	//其他用户权限
}

void output_user_group(uid_t uid,gid_t gid)
{
	struct passwd *user;
	user = getpwuid(uid);     //通过用户ID查看用户信息
	printf(" %s",user->pw_name );        //打印用户名信息

	struct group *group;
	group = getgrgid(gid);    //通过组ID查看用户信息
	printf(" %s",group->gr_name );            //打印组名信息
}

void output_mytime(time_t mytime )
{
	char buf[128];
	memset(buf,'\0',sizeof(buf));
	memcpy(buf,ctime(&mytime),strlen(ctime(&mytime)) -1);   //打印文件最后一次被修改的时间，保存在buf中，ctime将时间转换为字符串形式
	printf(" %s",buf );
}
