/*
 * =====================================================================================
 *
 *       Filename:  kavonTest.c
 *
 *    Description:  1st cpputest test case
 *
 *        Version:  1.0
 *        Created:  2013年01月20日 20时52分33秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Kavon Ma (mn), kavon.ma@alcatel-lucent.com
 *        Company:  Alcatel-Lucent
 *
 * =====================================================================================
 */
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "CppUTestExt/MockSupportPlugin.h"
#include "CppUTest/MemoryLeakDetectorMallocMacros.h"
#include "CppUTest/MemoryLeakDetectorNewMacros.h"

extern "C"
{
#include "kavon.h"
}

TEST_GROUP(FirstTestGroup)
{
	void setup()
	{
	}

	void teardown()
	{
	}
};

TEST(FirstTestGroup, FirstTest)
{
	hello();
}
