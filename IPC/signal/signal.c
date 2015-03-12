/*
 * 1. SIG_IGN, SIG_DFL, void handler(int sig)
 * 2. 当一个信号的信号处理函数执行时，如果进程又接收到了该信号，该信号会自动被储存而不会中断信号处理函数的执行，
 *    直到信号处理函数执行完毕再重新调用相应的处理函数
 * 3. 如果在信号处理函数执行时进程收到了其它类型的信号，该函数的执行就会被中断
 */
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void signal_handle(int sig)
{
    printf("received signal: %d\n", sig);
    signal(SIGINT, SIG_DFL);
}

int main()
{
    signal(SIGINT, signal_handle);
   
    while(1)
    {
    	printf("waiting SIGINT(CTRL+C)\n");
    	sleep(2);
    }

    return 0;
}
