#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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

    /* bind connection address */
    ret = bind(fd, (struct sockaddr *)&addr, sizeof(addr));
    if(ret == -1)
    {
    	perror("bind");
    	close(fd);
    	exit(1);
    }
    printf("bind address successful!\n");

    /* prepare for receiving */
    char buf[255];
    struct sockaddr_in from;
    socklen_t len;
    len = sizeof(from);

	while(1)
	{
        ret = recvfrom(fd, buf, sizeof(buf)-1, 0, (struct sockaddr *)&from, &len);
        if(ret > 0)
        {
            buf[ret] = '\0';
            printf("received message from %s is %s\n", inet_ntoa(from.sin_addr), buf);
        }
        else
        {
            printf("stop receiving\n");
            break;
        }
	}

    close(fd);
    return 0;
}
