/*
 * =====================================================================================
 *
 *       Filename:  kmp.c
 *
 *    Description:  KMP Algorithm
 *
 *        Version:  1.0
 *        Created:  2013年11月12日 10时32分13秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Kavon Ma (mn), kavon.ma@alcatel-lucent.com
 *        Company:  Alcatel-Lucent
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <assert.h>
#include <string.h>

void getNext(const char* T, int* next);
int KMP(const char* S, const char* T);

int main()
{
	int pos;
	char *S = "ababcabcacbab";
	char *T = "abcac";

	pos = KMP(S, T);
#ifdef DEBUG
    printf("pos = %d\n", pos);
#endif
	return 0;
}

void getNext(const char* T, int* next)
{
    assert(T != NULL);
    assert(strlen(T) > 0);
    assert(next != NULL);

	next[0] = -1;
	int j = 0;
	int k = -1;

	while (j < strlen(T)-1)
	{
		if (k == -1 || T[j] == T[k])
		{
			j++;
			k++;
			next[j] = k;
		}
		else
		{
			k = next[k];
		}
	}

	return;
}

int KMP(const char* S, const char* T)
{
    assert(S != NULL);
    assert(strlen(S) > 0);
    assert(T != NULL);
    assert(strlen(T) > 0);

	int i = 0;
	int j = 0;
	int next[100];

	getNext(T, next);
#ifdef DEBUG
	int index;
    for (index = 0; index < strlen(T); index++)
	{
		printf("next[%d] = %d\n", index, next[index]);
	}
#endif
	while (i < strlen(S))
	{
		if (j == -1 || S[i] == T[j])
		{
			i++;
			j++;
		}
		else
		{
			j = next[j];
		}

		if (j == strlen(T))
		{
			return i-strlen(T);
		}
	}

	return -1;
}
