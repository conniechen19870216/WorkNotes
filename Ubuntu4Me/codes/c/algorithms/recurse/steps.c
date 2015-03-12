/*
 * =====================================================================================
 *
 *       Filename:  steps.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年07月14日 21时37分06秒
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
#include <string.h>

#define Maxsize 200

int getStepNum(int, int);
void printSum();
int *step;
int len;

int main() {
	int n = 10;
	int maxStep = 2;
	step = (int*)malloc(sizeof(int[n]));
	printf("方案数：%d\n",getStepNum(n, maxStep));

}

void printSum() {
	printf("走法:");
	int i = 0;
	while(i < len){
		printf("%d ", step[i]);
		i++;
	}
	printf("\n");
}

int getStepNum(int n, int m) {
	int sumStep = 0;

	// 总台阶数为0时，终止递归循环
	if (n == 0) {
		printSum();
		return 1;
	}

	if (n >= m) {
		// 如果n大于每步最大台阶数，则设置第一步为m之内的一个台阶数，然后递归循环
		int i = 1;
		while(i <= m){
			step[len] = i;
			len++;
			sumStep += getStepNum(n - i, m);
			len--;
			i++;
		}
	} else {
		// 如果n小于m，则将一步最大台阶数缩小为n，重新递归
		sumStep = getStepNum(n, n);
	}
	return sumStep;
}
