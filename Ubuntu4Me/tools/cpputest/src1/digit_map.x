#ifndef DIGIT_MAP_X_
#define DIGIT_MAP_X_

typedef enum DigitMapState_Tag
{
	DM_INACTIVE = 0,
	DM_ACTIVE,
	DM_FAILED_MATCH,
	DM_PARTIAL_MATCH
} DigitMapState;

typedef struct digitMapCandidate
{
	DigitMapState       state;
	TknStr              pattern;
	U8                  index;
} DigitMapCandidate;

typedef struct digitMap
{
	DigitMapCandidate   *candidates;
	U16                 numberCandidates;
} DigitMap;

extern S16 InitDigitMap(DigitMap *pDigitMap, S8 rawDigitMapStr[][MAX_DIGITMAP_LEN], U16 num);
extern U8 DigitMapCheck(DigitMap *pDigitMap, U16 index, U8 digit);

#endif /*DIGIT_MAP_X_*/
