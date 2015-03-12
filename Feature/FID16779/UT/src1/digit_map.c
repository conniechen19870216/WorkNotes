/* header include files (.h) */
#include "gen.h"
#include "digit_map.h"

/* header/extern include files (.x) */
#include "gen.x"
#include "digit_map.x"

S16 InitDigitMap(DigitMap *pDigitMap, S8 rawDigitMapStr[][MAX_DIGITMAP_LEN], U16 num)
{
	assert(pDigitMap != NULL);
	assert(rawDigitMapStr != NULL);
	assert(num > 0);

	U8 ret = ROK;
	U8 len;
	U8 i;

	/* Allocate memories for num candidates */
	pDigitMap->numberCandidates = num;
	pDigitMap->candidates = (DigitMapCandidate *)malloc(pDigitMap->numberCandidates * sizeof(DigitMapCandidate));
	memset(pDigitMap->candidates, 0, pDigitMap->numberCandidates*sizeof(DigitMapCandidate));

	/* Initialize num candidates */
	for (i=0; i<pDigitMap->numberCandidates; i++)
	{
		/* Initialize candidate pattern */
		len = strlen(rawDigitMapStr[i]);
		assert(len > 0);
	    strncpy(pDigitMap->candidates[i].pattern.val, rawDigitMapStr[i], len);
		pDigitMap->candidates[i].pattern.pres = PRSNT_NODEF;
		pDigitMap->candidates[i].pattern.len = len;

		/* Initialize candidate index */
		pDigitMap->candidates[i].index = 0;

		/* Initialize candidate state */
		pDigitMap->candidates[i].state = DM_ACTIVE;
	}

	return ret;
}

U8 DigitMapCheck(DigitMap *pDigitMap, U16 index, U8 digit)
{
	assert(pDigitMap != NULL);
	assert(index >= 0);

	U8 ret = 0;

	return ret;
}
