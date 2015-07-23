#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>

int main()
{
    int resource = RLIMIT_STACK;
    struct rlimit rlim, rlim_new;

    if (getrlimit(resource, &rlim) == 0)
    {
        printf("got limit of %d\n", resource);
        if (rlim.rlim_max == RLIM_INFINITY)
        {
            printf("existing %d.rlim_max is RLIM_INFINITY\n", resource);
        }
        else
        {
            printf("existing %d.rlim_max is not RLIM_INFINITY but %d\n", resource, rlim.rlim_max);
        }

        printf("existing %d.rlim_cur is %d\n", resource, rlim.rlim_cur);

        rlim_new.rlim_cur = rlim_new.rlim_max = RLIM_INFINITY;
        if (setrlimit(resource, &rlim_new) != 0)
        {
            printf("disallow setting unlimited for %d\n", resource);
            rlim_new.rlim_cur = rlim_new.rlim_max = rlim.rlim_max;
            setrlimit(resource, &rlim_new);
        }
        else
        {
            printf("set to RLIM_INFINITY successfully\n");
        }
    }

    return 0;
}
