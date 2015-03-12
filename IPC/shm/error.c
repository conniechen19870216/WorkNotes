#include <stdio.h>
#include <string.h>
#include <errno.h>

int main()
{
	int i;
	for(i=0; i<256; i++)
	{
		printf("errno.%02d is %s\n", i, strerror(i));
	}

	return 0;
}
