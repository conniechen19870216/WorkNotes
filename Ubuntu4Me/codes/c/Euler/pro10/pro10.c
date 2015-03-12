/*
 * =====================================================================================
 *
 *       Filename:  pro10.c
 *
 *    Description:  sum of all the primes below 2 million
 *
 *        Version:  1.0
 *        Created:  2013年06月08日 11时08分00秒
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
	int i, number;
    long long sum=2;

	for (number=3; number<2000000; number+=2)
	{
		for (i=2; i<=sqrt(number); i++)
		{
			if (number % i == 0)
			{
				break;
			}
		}
		if (i > sqrt(number))
		{
			sum += number;
		}
	}

	printf("sum is %lld\n", sum);

	return 0;
}
