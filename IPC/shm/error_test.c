#include <stdio.h>
#include <string.h>
#include <errno.h>

int main()
{
    perror("kavon");
	printf("errno is %d (%s)\n", errno, strerror(errno));

	return 0;
}
