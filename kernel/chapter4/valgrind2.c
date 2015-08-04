#include <stdlib.h>
#include <stdio.h>

int main()
{
    char *chptr;
    char *chptr1;
    int i  = 1;
    chptr  = (char *) malloc(512);
    chptr1 = (char *) malloc (512);
    for (i; i <= 513; i++)  
    {
        chptr[i] = '?';
        chptr1[i] = chptr[i];
    }
    
    free(chptr1);
    free(chptr);

    return 0;
}
