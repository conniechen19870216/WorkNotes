/*
 * =====================================================================================
 *
 *       Filename:  AllTest.c
 *
 *    Description:  Test Cases
 *
 *        Version:  1.0
 *        Created:  2013年01月20日 20时56分38秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Kavon Ma (mn), kavon.ma@alcatel-lucent.com
 *        Company:  Alcatel-Lucent
 *
 * =====================================================================================
 */
#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTestExt/MockSupport.h"
#include "CppUTestExt/MockSupportPlugin.h"
#include "CppUTest/TestRegistry.h"
#include "AllTest.h"

int main(int ac, char** av)
{
	return CommandLineTestRunner::RunAllTests(ac, av);
}
