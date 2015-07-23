#include <stdio.h>

void function1()
{
    int i;
    for (i=0;i<1000000;i++);
}

void function2()
{
    int i;
    for (i=0;i<1000000;i++);
    function1();
    for (i=0;i<2000000;i++);
}

void function3()
{
    int i;
    function1();
    function2();
    for (i=0;i<3000000;i++);
    function1();
   
}

int main()
{
    int i;
    for (i=0;i<10;i++)
    {
        function1();
    }

    for (i=0;i<5000000;i++);

    for (i=0;i<10;i++)
    {
       function2();
    }

    for (i=0; i<13;i++);
    function3();
    function2();
    function1();

    return 0;
}
