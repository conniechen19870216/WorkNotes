/*
 * =====================================================================================
 *
 *       Filename:  pro4.c
 *
 *    Description:  Largest palindrome product
 *
 *        Version:  1.0
 *        Created:  2013年05月31日 13时28分38秒
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
	int temp_result, mul_result;
	unsigned char temp[6];
	int flag = 0;

	for (i=999; i>900; i--)
	{
		for (j=999; j>900; j--)
		{
			mul_result = i*j;
			temp_result = mul_result;

			for (k=0; k<6; k++)
			{
				temp[k] = mul_result % 10;
				mul_result /= 10;
			}

			if ((temp[0] == temp[5]) &&
				(temp[1] == temp[4]) &&	
				(temp[2] == temp[3]))
			{
				printf("found the value %d = %d * %d\n", temp_result, i, j);
				flag = 1;
				break;
			}
		}
		if (flag != 0)
		{
			break;
		}
	}
	if (flag == 0)
	{
		printf("There is no such number\n");
	}

	return 0;
}
