#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char passwd[] = "password";
    if (argc < 2)
    {
        printf("at least one argument\n");
        return 1;
    }

    if (!strcmp(passwd, argv[1]))
    {
        printf("correct password\n");
        return 0;
    }

    printf("incorrect password\n");
    return 0;
}
