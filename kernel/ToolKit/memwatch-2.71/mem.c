#include <stdlib.h>
#include <stdio.h>
#include "memwatch.h"

int main()
{
    char *ptr1;
    char *ptr2;

    ptr1 = malloc(512);
    ptr2 = malloc(512);

    ptr2 = ptr1;
    free(ptr2);
    free(ptr1);
}
