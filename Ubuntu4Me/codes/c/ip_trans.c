#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main (void)
{
  char IPdotdec[20];
  struct in_addr s;
  printf("Please input IP address: ");
  scanf("%s", &IPdotdec);
  printf("inet_pton: 0x%x\n", s.s_addr);
  printf("inet_ntop: %s\n", IPdotdec);
}
