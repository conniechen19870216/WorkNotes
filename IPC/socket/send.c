#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
    /* create socket */
    int fd;
	int ret;
    struct sockaddr_in addr;

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(fd == -1)
    {
        perror("socket");
        exit(1);
    }
    printf("socket is %d\n", fd);

    /* build connection address */
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(5060);

	/* send the message */
	char buf[255];
	while(1)
	{
		ret = read(0, buf, sizeof(buf)-1);
		buf[ret] = '\0';
		if(ret > 0)
		{
			sendto(fd, buf, ret, 0, (struct sockaddr *)&addr, sizeof(addr));
		}
		else
		{
			printf("stop sending\n");
			break;
		}
	}

    close(fd);
    return 0;
}
