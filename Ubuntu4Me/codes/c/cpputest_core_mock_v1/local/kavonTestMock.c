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
#include "hello.h"
}

int mock_add(int a, int b)
{
	mock().actualCall("printSum");
    int c = a + b + 100;	
	return c;
}

TEST_GROUP(MockTestGroup)
{
	void setup()
	{
		UT_PTR_SET(add_ptr, &mock_add);
	}

	void teardown()
	{
	    mock().checkExpectations();
		mock().clear();
	}
};

TEST(MockTestGroup, SimpleScenario)
{
	mock().expectOneCall("printSum");
	printSum();
}
