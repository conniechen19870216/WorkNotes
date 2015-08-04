#include <mcheck.h>

void _init()
{
    mtrace();
}

void _fini()
{
    muntrace();
}
