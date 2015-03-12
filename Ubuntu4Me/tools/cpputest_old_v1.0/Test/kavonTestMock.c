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
#include <stdio.h>
#include "hello.h"
}

int mock_add(int a, int b)
{
	mock().actualCall("printSum").withParameter("a", a);
    int c = a + b + 100;	
	return c;
}

level mock_add_level(level *a, level *b)
{
	mock().actualCall("printSumLevel").withParameter("level_a", a);
    level c;
	c.points = a->points + b->points + 100;	
	c.money = a->money + b->money + 100;	
	return c;
}

TEST_GROUP(MockTestGroup)
{
	void setup()
	{
		UT_PTR_SET(add_ptr, &mock_add);
		UT_PTR_SET(add_level_ptr, &mock_add_level);
	}

	void teardown()
	{
	    mock().checkExpectations();
		mock().clear();
	}
};

TEST(MockTestGroup, Mock_Basic)
{
	mock().expectOneCall("printSum").withParameter("a", 2);
	printSum();
}

TEST(MockTestGroup, Mock_Add_Struct)
{
	mock().expectOneCall("printSumLevel").ignoreOtherParameters();
	printSumLevel();
}

TEST(MockTestGroup, Mock_Data)
{
	mock().setData("Kavon", 1);
	CHECK_EQUAL(2, mock().getData("Kavon").getIntValue())

	if (mock().hasData("Kavon"))
	{
		printf("Kavon is ready.\n");
	}
	else
	{
		printf("Kavon is not ready.\n");
	}
}

