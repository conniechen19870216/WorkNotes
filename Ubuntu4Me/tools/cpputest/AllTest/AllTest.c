#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTestExt/MockSupport.h"
#include "CppUTestExt/MockSupportPlugin.h"
#include "CppUTest/TestRegistry.h"
#include "AllTest.h"

int main(int ac, char** av)
{
	return CommandLineTestRunner::RunAllTests(ac, av);
}
