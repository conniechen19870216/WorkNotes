#include <unistd.h>
#include <stdio.h>

int main()
{
	int fd[2];
	char buf[80];
	pid_t pid;

    pipe(fd);
	printf("fd[0]=%d\n",fd[0]);
	printf("fd[1]=%d\n",fd[1]);

    pid = fork();
	if(pid == 0)
	{
		printf("This is child process\n");
		char s[] = "pipe communication";
		write(fd[1], s, sizeof(s));
		close(fd[0]);
		close(fd[1]);
	}
	else
	{
		waitpid(pid, NULL, 0);
		printf("This is parent process\n");
		read(fd[0], buf, sizeof(buf));
		printf("%s\n", buf);
		close(fd[0]);
		close(fd[1]);
	}

	return 0;
}
