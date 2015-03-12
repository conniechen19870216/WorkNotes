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
  inet_pton(AF_INET, IPdotdec, (void *)&s);
  printf("inet_pton: 0x%x\n", s.s_addr);
  inet_ntop(AF_INET, (void *)&s, IPdotdec);
  printf("inet_ntop: %s\n", IPdotdec);
}
