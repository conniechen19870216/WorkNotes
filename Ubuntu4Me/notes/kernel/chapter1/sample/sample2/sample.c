#include <stdlib.h>
#include <stdio.h>
#include <math.h>

void test ()
{
    int i;
    for (i=0; i<10000; i++);
}

int prime (int num);

int main()
{
    int i;
    int colcnt = 0;
    for (i=2; i <= 50000; i++)
    {
        if (prime(i)) 
        {
            colcnt++;
            if (colcnt%9 == 0) 
            {
                printf("%5d\n",i);
                colcnt = 0;
            }
            else
            {
                printf("%5d ", i);
            }
        }
    }

    putchar('\n');
    return 0;
 }

int prime (int num) 
{
    /* check to see if the number is a prime? */
    test();
    int i;
    for (i=2; i<=sqrt(num); i++)
    if (num %i == 0)
    {
        return 0;
    }

    return 1;
}
