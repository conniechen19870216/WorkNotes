/*
 * =====================================================================================
 *
 *       Filename:  pro6.c
 *
 *    Description:  10001st prime
 *
 *        Version:  1.0
 *        Created:  2013年05月31日 13时58分21秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Kavon Ma (mn), kavon.ma@alcatel-lucent.com
 *        Company:  Alcatel-Lucent
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main()
{
	int index = 1;
	int num = 3;
	int i;

	while (index < 10001)
	{
		for (i=2; i<=sqrt(num); i++)
		{
			if (num % i == 0)
			{
				break;
			}
		}
		if (i > sqrt(num))
		{
			index++;
		}

		num += 2;
	}

	printf("10001st prime num is %d\n", num-2);

	return 0;
}
