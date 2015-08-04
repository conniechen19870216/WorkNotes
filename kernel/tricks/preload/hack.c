#include <stdio.h>

int strcmp(const char *s1, const char *s2)
{
    printf("hit hack function, s1 = %s, s2 = %s\n", s1, s2);
    return 0;
}
