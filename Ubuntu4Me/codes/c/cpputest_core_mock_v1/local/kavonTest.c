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

#define BOOL 0
int i = 0;
long a = 1;
long b = 2;

TEST_GROUP(FirstTestGroup)
{
	void setup()
	{
	}

	void teardown()
	{
	}
};

TEST(FirstTestGroup, 1stTest)
{
	hello();
}

TEST(FirstTestGroup, 2ndTest)
{
	FAIL("Fail me!");
}

TEST(FirstTestGroup, 3rdTest)
{
	STRCMP_EQUAL("hello", "world");
}

TEST(FirstTestGroup, 4thTest)
{
	CHECK(BOOL);
}

TEST(FirstTestGroup, 5thTest)
{
	CHECK_EQUAL(BOOL, 1);
}

TEST(FirstTestGroup, 6thTest)
{
	CHECK_EQUAL(i, 0);
}

TEST(FirstTestGroup, 7thTest)
{
	LONGS_EQUAL(a, b);
}

