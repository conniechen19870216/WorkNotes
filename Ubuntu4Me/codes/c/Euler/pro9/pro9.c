/*
 * =====================================================================================
 *
 *       Filename:  pro9.c
 *
 *    Description:  Pythagorean triplet 
 *
 *        Version:  1.0
 *        Created:  2013年06月08日 10时52分46秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Kavon Ma (mn), kavon.ma@alcatel-lucent.com
 *        Company:  Alcatel-Lucent
 *
 * =====================================================================================
 */

#include <stdio.h>

int main()
{
	int i, j, k;

	for (i=1; i<1000; i++)
	{
		for (j=i+1; j<1000; j++)
		{
            k = 1000-i-j;

			if (i*i + j*j == k*k)
			{
				printf("i=%d, j=%d, k=%d\n", i, j, k);
				printf("i * j * k = %d\n", i*j*k);
			}
		}
	}

	return 0;
}
