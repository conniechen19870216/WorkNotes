/*
 * =====================================================================================
 *
 *       Filename:  print_errno.c
 *
 *    Description:  used to print errno meaning
 *
 *        Version:  1.0
 *        Created:  09/ 9/2014  2:18:17 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Kavon Ma (mn), kavon.ma@alcatel-lucent.com
 *        Company:  Alcatel-Lucent
 *
 * =====================================================================================
 */

#include <string.h>
#include <errno.h>  /* for strerror */
#include <stdio.h>

int main(int argc, char ** argv)
{
  int i = 0;
  for(i = 0; i < 256; i++)
  printf("errno.%02d is: %s\n", i, strerror(i));
  return 0;
}
