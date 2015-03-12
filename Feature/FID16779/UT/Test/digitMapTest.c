#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "CppUTestExt/MockSupportPlugin.h"
#include "CppUTest/MemoryLeakDetectorMallocMacros.h"
#include "CppUTest/MemoryLeakDetectorNewMacros.h"

extern "C"
{
/* header include files (.h) */
#include "gen.h"
#include "digit_map.h"

/* header/extern include files (.x) */
#include "gen.x"
#include "digit_map.x"                
}

S8 rawDigitMapStr[NUM_DIGITMAP][MAX_DIGITMAP_LEN] = {
    "12xxx",
    "34xx[1-9]", 
    "ABxx01xx[A-F]", 
    "[13-6]xx89xx[5-9A-F]"
};  

TEST_GROUP(digitMapTestGroup)
{
	int i;
    DigitMap *pDigitMap;
	void setup()
	{
        pDigitMap = (DigitMap *)malloc(sizeof(DigitMap));
        memset(pDigitMap, 0, sizeof(DigitMap));
	}

	void teardown()
	{
        free(pDigitMap);
		pDigitMap = NULL;
	}
};

TEST(digitMapTestGroup, InitDigitMapTest)
{
	InitDigitMap(pDigitMap, rawDigitMapStr, NUM_DIGITMAP);

    CHECK_EQUAL(NUM_DIGITMAP, pDigitMap->numberCandidates);
	for (i=0; i<pDigitMap->numberCandidates; i++)
	{
		CHECK_EQUAL(DM_ACTIVE, pDigitMap->candidates[i].state);
		CHECK_EQUAL(0, pDigitMap->candidates[i].index);
		
		CHECK_EQUAL(PRSNT_NODEF, pDigitMap->candidates[i].pattern.pres);
		CHECK_EQUAL(strlen(rawDigitMapStr[i]), pDigitMap->candidates[i].pattern.len);
		STRCMP_EQUAL(rawDigitMapStr[i], pDigitMap->candidates[i].pattern.val);
	}
}
