#include <stdio.h>

int sum(int, int);

int main()
{
    int c = sum(1, 2);
    printf("c=%d\n", c);
    return 0;
}

int sum(int a, int b)
{
    int c = a + b;
    return c;
}
