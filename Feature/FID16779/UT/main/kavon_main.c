/* header include files (.h) */
#include "gen.h"
#include "digit_map.h"

/* header/extern include files (.x) */
#include "gen.x"
#include "digit_map.x"

S8 rawDigitMapStr[NUM_DIGITMAP][MAX_DIGITMAP_LEN] = {
	"12xxx", 
	"34xx[1-9]", 
	"ABxx01xx[A-F]", 
	"[13-6]xx89xx[5-9A-F]"
};

S32 main()
{
    S16 ret = RFAILED;
    DigitMap *pDigitMap = NULL;

	pDigitMap = (DigitMap *)malloc(sizeof(DigitMap));
	memset(pDigitMap, 0, sizeof(DigitMap));

	ret = InitDigitMap(pDigitMap, rawDigitMapStr, NUM_DIGITMAP);
	if (ROK == ret)
	{
		DP("InitDigitMap Success!\n");
	}
	else
	{
		DP("InitDigitMap Failure!\n");
	}

	return ROK;
}
