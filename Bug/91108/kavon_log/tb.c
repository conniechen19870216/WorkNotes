/**********************************************************************
**  Copyright (c) 2001 Telica.  All rights reserved.
**
**  $Id: tb.c,v 1.186 2012/09/30 02:31:59 alphaz Exp $
**
**  Name:  tb.c
**
**  Desc:  This file contains functions for validation and defaulting
**  
***********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "git_class.h"
#include "TsmColDefs.h"
#include "tb.h"
#include "tb_sipt.h"
#include "envopt.h"
#include "envdep.h"
#include "envind.h"
#include "gen.h"
#include "ssi.h"
#include "gen.x"
#include "ssi.x"
#include "cm_lib.x"
#include "api_ver.h"
#include "tl1_util.h"
/* PLM_IN */
#include "string.h"
#include "tb_sorm.h"

#if 0
#define ASM_TABLE_DEBUG 1
#endif

/* new function added in 6.3 for hotupgrade */
static int tbDirectMapTabId(unsigned short srcIdx, unsigned short dstIdx, int srcTableId, int *dstTableId);

int tkStrLen[MAX_WIDTH_ENUM]=
{
    [STR8]   9,
    [STR16]  17,
    [STR32]  33,
    [STR64]  65,
    [STR128] 129,
    [STR256] 257,
};

int tkTypeLen[MAX_WIDTH_ENUM]=
{
    [U8BITS]  sizeof(TkU8),
    [S8BITS]  sizeof(TkS8),
    [U16BITS] sizeof(TkU16),
    [S16BITS] sizeof(TkS16),
    [U32BITS] sizeof(TkU32),
    [S32BITS] sizeof(TkS32),
    [STR8]    sizeof(TkStr8),
    [STR16]   sizeof(TkStr16),
    [STR32]   sizeof(TkStr32),
    [STR64]   sizeof(TkStr64),
    [STR128]  sizeof(TkStr128),
    [STR256]  sizeof(TkStr256),
};


/* NOTE: table definitions have moved to tb_defs.c */

/* 
 */
int tbTabIdMap_5_2_and_4_3[][2] =
{
    /* {EXAMPLE_TABLE, EXAMPLE_TABLE_4_3},*/
    /* add more */
    {MG_SCTPM2UA_PM_TAB_ID, M2UASCTP_PM_TAB_ID_v_4_3},
    {SG_SCTPM2UA_PM_TAB_ID, M2UASGSCTP_PM_TAB_ID_v_4_3},
    {SG_SCTPM3UA_PM_TAB_ID, M3UASGSCTP_PM_TAB_ID_v_4_3},
    {MGC_SCTPM3UA_PM_TAB_ID, M3UAMGCSCTP_PM_TAB_ID_v_4_3},
    {MGC_SCTPIUA_PM_TAB_ID, IUAMGCSCTP_PM_TAB_ID_v_4_3},
    {MG_SCTPIUA_PM_TAB_ID, IUAMGSCTP_PM_TAB_ID_v_4_3},
    {SG_SCTPM2PA_PM_TAB_ID, M2PASGSCTP_PM_TAB_ID_v_4_3},
    {0, 0},/* end delimiter */
};      


/* Note that for tbMapLst, the HIGHER version has the to-from map list  */

TbVerCb tbVerCb[TB_MAX_VERSIONS] =
{
/* Currently there is no plan to support 5.1.2->6.3.1 hotupgrade and this
   and the v_1 files are not quite up-to-date which can cause problems so
   Null out it for now until it's decided that we will support 5.1.2. 
    {0x101, tableDefs_v_1, TABLE_ID_OFFSET_v_1, TELICA_MAX_TAB_ID_v_1},
*/
    {TB_VER_512,  NULL, 0, 0},
    {TB_VER_53,   NULL, 0, 0},
/* Currently there is no plan to support 6.2.0->6.3.1 hotupgrade and this
   and the v_3 files are not quite up-to-date which causes problems so
   Null out it for now until it's decided that we will support 6.2.0. 
    {0x301, tableDefs_v_3, TABLE_ID_OFFSET_v_3, TELICA_MAX_TAB_ID_v_3},
*/
    {TB_VER_620,  NULL, 0, 0},
/* Currently, there is no plan to support 6.2.1.2->6.3.1 hotupgrade */
    {TB_VER_621,  NULL, 0, 0},
/* 6.2.1.3 and 6.2.1.4 have the same version number */
    {TB_VER_6213, tableDefs_v_4_3, TABLE_ID_OFFSET_v_4_3, TELICA_MAX_TAB_ID_v_4_3},
    {TB_VER_63,   tableDefs_v_5_1, TABLE_ID_OFFSET_v_5_1, TELICA_MAX_TAB_ID_v_5_1},
    {TB_VER_631,  tableDefs_v_5_2, TABLE_ID_OFFSET_v_5_2, TELICA_MAX_TAB_ID_v_5_2},
    {TB_VER_64,   tableDefs_v_6_1, TABLE_ID_OFFSET_v_6_1, TELICA_MAX_TAB_ID_v_6_1, {{0x403, tbTabIdMap_5_2_and_4_3}}},
    {TB_VER_641,  tableDefs_v_6_2, TABLE_ID_OFFSET_v_6_2, TELICA_MAX_TAB_ID_v_6_2}, 
    {TB_VER_70,   NULL, 0, 0},
    {TB_VER_701,  NULL, 0, 0},
    {TB_VER_71,   NULL, 0, 0},
    {TB_VER_801,  NULL, 0, 0},
    {TB_VER_802,  NULL, 0, 0},
    {TB_VER_803,  NULL, 0, 0},
    {TB_VER_804,  NULL, 0, 0},
    {TB_VER_81,   tableDefs_v_8_a, TABLE_ID_OFFSET_v_8_a, TELICA_MAX_TAB_ID_v_8_a}, 
    {TB_VER_8103, tableDefs_v_8_d, TABLE_ID_OFFSET_v_8_d, TELICA_MAX_TAB_ID_v_8_d}, 
    {TB_VER_8104, tableDefs_v_8_e, TABLE_ID_OFFSET_v_8_e, TELICA_MAX_TAB_ID_v_8_e}, 
    {TB_VER_82  , NULL, 0, 0},
    {TB_VER_821,  tableDefs_v_a_1,       TABLE_ID_OFFSET_v_a_1,       TELICA_MAX_TAB_ID_v_a_1}, 
    {TB_VER_83, tableDefs_v_b_1, TABLE_ID_OFFSET_v_b_1, TELICA_MAX_TAB_ID_v_b_1},
    {TB_VER_831, tableDefs_v_b_a, TABLE_ID_OFFSET_v_b_a,TELICA_MAX_TAB_ID_v_b_a},
	 {TB_VER_84, tableDefs_v_c_1, TABLE_ID_OFFSET_v_c_1,TELICA_MAX_TAB_ID_v_c_1},
    {TB_VER_8402, tableDefs_v_c_3, TABLE_ID_OFFSET_v_c_3, TELICA_MAX_TAB_ID_v_c_3},
    {TB_VER_841, tableDefs_v_c_a, TABLE_ID_OFFSET_v_c_a, TELICA_MAX_TAB_ID_v_c_a},
    {TB_VER_91, tableDefs, TABLE_ID_OFFSET, TELICA_MAX_TAB_ID},
};

/*****************************************************************/
/** DB encryption/decryption, introduced by FID 15083 SORM.      */ 
/** If the new table need to be encrypted, please add rules below*/
/**                                                              */
/** NOTE: if one table needs encrypt, the DB column type must be */
/**       string type in mySQL, and the length needs to be       */
/**       considered.                                            */
/*****************************************************************/
#define SORMENCRYPTKEY "etsili"

/**********************************************************/
/**  LI-TARGET column encryption definition               */
/**********************************************************/
EnDecryptRuleDef liTarget_en_rule =
{
    SORMENCRYPTKEY,   /* Encrpytion Key              */
    ENCRYPT_DEFAULT,  /* Column Encryption Algorithm */
};

EnDecryptRuleDef * liTargetEnRules[] =
{
    &liTarget_en_rule, /* LITARGET_COL_TARGETDNTYPE */
    &liTarget_en_rule, /* LITARGET_COL_TARGETDN     */
    &liTarget_en_rule, /* LITARGET_COL_DIRECTION    */
    &liTarget_en_rule, /* LITARGET_COL_INTERCEPTTYPE*/
    &liTarget_en_rule, /* LITARGET_COL_CCCIPPRFLID  */
    &liTarget_en_rule, /* LITARGET_COL_CDCIPPRFLID  */
    &liTarget_en_rule, /* LITARGET_COL_TL1USER      */
    &liTarget_en_rule, /* LITARGET_COL_MDID*/
};

/***********************************************************/
/** The Encryptioin Table definition List Definition       */
/** This array will be used in Common DB API Functions     */
/***********************************************************/
TbEnDecryptDef EnDecryptLst[] =
{
    {LITARGET_TAB_ID, liTargetEnRules},
};

/***********************************************************/
/** isEncryptedDB                                          */
/**                                                        */
/**     Desc:   Determine if one table needs encrypte      */
/**                                                        */
/**     Ret:    index -  yes, it needs to be encypted      */
/**                      Return the index of EnDecryptLst  */
/**             -1    -  The table is not encrypted        */
/**                                                        */
/***********************************************************/
int isEncryptedDB(int tableId)
{
    int i = 0;
    for (i = 0; i < (sizeof(EnDecryptLst)/sizeof(TbEnDecryptDef)); i++)
    {
        if (tableId == EnDecryptLst[i].tableId)
            return i;
    }
    return -1;
}


/**********************************************************************
** tbFndRowVer
**
**     Desc:   find the row version
**
**     Notes:  1. This function assumes that the table row is atleast 8
**                (tableId + version) or tableId + tokens
**             2. If no version header is found, the row is assumed to be
**                old style
**
**     Ret:    - 16 bits version if found
**             - 0 if no version found
**             - TB_INVALID_VER if error
**
**********************************************************************/
unsigned short
tbFndRowVer(void *row)
{
    int intVer;
    unsigned short ver;
    

    if (!row)
    {
        return (TB_INVALID_VER);
    }

    intVer = *((int *)row + 1);

    /* check for version header */
    if ((intVer & 0xff000000) == 0xff000000)
    {
        /* version present */
        ver = (unsigned short)(intVer & 0x00ffffff);
    }
    else
    {
        ver = 0;
    }
    
    return (ver);
} /* tbFndRowVer() */




/**********************************************************************
** tbFndVerIdx
**
**     Desc:   find the version index used to index the jumper table
**
**     Notes:  1. the version is typically comprised of a major and minor
**                number, which make this number unwieldly to use as a
**                direct index.
**             2. Currently, this function uses a linear search to determine
**                the index.  For number of versions exceeding 10, we should
**                change this to a binary search (oe a hash list if number
**                of versions exceed 100)
**
**     Ret:    - 16 bits index if match found
**             - TB_INVALID_VER if error or no match found
**
**********************************************************************/
unsigned short
tbFndVerIdx(unsigned short ver)
{
    unsigned short i, verIdx = TB_INVALID_VER;

    // DP("tbFndVer: ver = %d\n",ver);

    for (i=0; i < TB_MAX_VERSIONS; i++)
    {
        if (ver == tbVerCb[i].version)
        {
            verIdx = i;
            break;
        }
    }
    
    // DP("Exiting tbFndVerIdx - returning verIdx = %d\n",verIdx);
    return (verIdx);

} /* tbFndVerIdx() */
    


/**********************************************************************
** tbFndVerDef
**
**     Desc:   find the table definition corresponding to a version and
**             tableId
**
**     Ret:    - 0         no error
**             - E_TB_GEN  error
**
**********************************************************************/
int
tbFndVerDef(unsigned short ver, int tableId, DbTableDef **tbDef)
{
    unsigned short verIdx;
    DbTableDef **tbDefLst;
        
    /* find the jumber table index corresponding to this version */
    verIdx = tbFndVerIdx(ver);
    if (verIdx == TB_INVALID_VER)
    {
        return (E_TB_GEN);
    }

    /* get the definition list for this tableId */    
    tbDefLst = tbVerCb[verIdx].tbDefLst;
    
    /* check min, max for this version */
    *tbDef = tbDefLst[tableId-tbVerCb[verIdx].startTableId];

    return (0);
} /* tbFndVerDef() */

int
tbFndRowTableId(void *row)
{
    int tableId;
    
    if (!row)
    {
        return (TB_INVALID_TABLE_ID);
    }

    tableId = *(int *)row;
    return (tableId);
} /* tbFndRowTableId() */



/**********************************************************************
** tbFndRowSize
**
**     Desc:   find the row size for the specified version of a
**             table definition.  since row must contain data,
**             error return is 0
**
**     Ret:    - 0         error
**             - size of table row for ver
**
**********************************************************************/
int
tbFndRowSize(unsigned short ver, int tableId)
{
    DbTableDef *tbDef;

    if (tbFndVerDef(ver, tableId, &tbDef) != 0)
    {
        return (0);
    }

    return tbDef->rowSize;
}

/**********************************************************************
** tbFndRowDef
**
**     Desc:   find the table definition corresponding to a table row
**
**     Notes:  
**
**     Ret:    ROK     - successful
**             RFAILED - failed
**
**********************************************************************/
int
tbFndRowDef(void *row, DbTableDef **tbDef)
{
    int ret, tableId;
    unsigned short ver;
    
    if (!row)
    {
        return (E_TB_NULLPTR);
    }

    ver = tbFndRowVer(row);
    if (ver == TB_INVALID_VER)
    {
        return (E_TB_GEN);
    }
    
    tableId = tbFndRowTableId(row);
    if (tableId == TB_INVALID_TABLE_ID)
    {
        return (E_TB_GEN);
    }
    
    ret = tbFndVerDef(ver, tableId, tbDef);
    if (ret != 0)
    {
        return (ret);
    }
    
    return (0);
    
} /* tbFndRowDef() */

/**********************************************************************
** tbDirectMapTabId
**
**     Desc:   Find the dstTableId from the srcTableId given 
**
**     Notes:  New function added in 6.3 to cover the cases where
**             a tableId in the srvVersion was removed in the dstVersion. 
**
**     Ret:    0  - successful
**             >0 - failed
**
**********************************************************************/
static int
tbDirectMapTabId(unsigned short srcIdx, unsigned short dstIdx, int srcTableId, int *dstTableId)
{
    int i, j, k;
    unsigned short highIdx = (srcIdx > dstIdx)? srcIdx: dstIdx;
    unsigned short lowIdx = (dstIdx < srcIdx)? dstIdx: srcIdx;
    
    if (highIdx == srcIdx)
    {
        k = 0;
    }
    else
    {
        k = 1;
    }    

#if 0    
    DP("%s: srcTableId:%d, highIdx:%d, lowIdx:%d, k=%d\n",
           __FUNCTION__,srcTableId, highIdx, lowIdx, k);
#endif

    /* look for a map list in the higher version's definition */
    for (i=0; i< TB_MAX_VERSIONS; i++)
    {
        if (tbVerCb[highIdx].tableIdMapLst[i].lowVer)
        {
            if (tbVerCb[highIdx].tableIdMapLst[i].lowVer == tbVerCb[lowIdx].version)
            {
                /* found direct map list; look for map */
                for (j=0; TRUE; j++)
                {
                    if (!tbVerCb[highIdx].tableIdMapLst[i].mapLst[j][0])
                    {
                        /* we hit the end */
                        return (1);
                    }
                    if (tbVerCb[highIdx].tableIdMapLst[i].mapLst[j][k] == srcTableId)
                    {
                        /* found match, return dstTableId */
                        *dstTableId = tbVerCb[highIdx].tableIdMapLst[i].mapLst[j][k ^ 1];
                        return (0);
                    }
                }
            }
        }
    }
    return (E_TB_TABLEID);
} /* tbDirectMapTabId() */


int
tbVerMapTableId(unsigned short srcVersion,
                int srcTableId,
                unsigned short dstVersion,
                int *dstTableId)
{
    unsigned short dstIdx, srcIdx;
    DbTableDef *srcDef, **dstTbDefLst;
    int ret, i, dstStartTabId, dstEndTableId;

    if (srcVersion == dstVersion)
    {
        *dstTableId = srcTableId;
        return (0);
    }

    /* initialize *dstTableId */
    *dstTableId = 0;
        
    /* find the src table definition */
    ret = tbFndVerDef(srcVersion, srcTableId, &srcDef);
    if ((ret != 0) || !srcDef || !(srcDef->tableIdStr))
    {
        DP("%s:error:tbFndVerDef() failed, ret:%d, srcDef:%p\n", __FUNCTION__, ret, srcDef);
        return (E_TB_GEN); /* CID 14073 */
    }
    
    /* find the jumper table index corresponding to the destination version */
    dstIdx = tbFndVerIdx(dstVersion);
    if (dstIdx == TB_INVALID_VER)
    {
        return (E_TB_GEN);
    }

    /* find the jumper table index corresponding to the source version */
    srcIdx = tbFndVerIdx(srcVersion);
    if (srcIdx == TB_INVALID_VER)
    {
        return (E_TB_GEN);
    }
        
    if ((ret = tbDirectMapTabId(srcIdx, dstIdx, srcTableId, dstTableId)) == 0)
    {
        /* found direct mapping */
        DP("%s:DIRECT match found for srcTableId:%s, srcVer:0x%x, dstVer:0x%x\n",
           __FUNCTION__,srcDef->tableIdStr, srcVersion, dstVersion);
        return (0);
    }

#if 0    
    DP("%s:error:DIRECT match not found for srcTableId:%s, srcVer:0x%x, dstVer:0x%x - trying strcmp method\n",
           __FUNCTION__,srcDef->tableIdStr, srcVersion, dstVersion);
#endif
    
    
    /* get src and dst  definition lists */    
    dstTbDefLst = tbVerCb[dstIdx].tbDefLst;
    dstStartTabId = tbVerCb[dstIdx].startTableId;
    dstEndTableId = tbVerCb[dstIdx].endTableId;
    
    /* loop through dst table definitions, looking for matching tableId string */
    for (i = 0; i < dstEndTableId-dstStartTabId; i++)
    {
        if (dstTbDefLst[i] && dstTbDefLst[i]->tableIdStr)
        {            
            if (strcmp(srcDef->tableIdStr, dstTbDefLst[i]->tableIdStr)==0)
            {
                *dstTableId = i + dstStartTabId;
                break;
            }
        }
    }
    
    if (!(*dstTableId))
    {
        DP("%s:error:match not found for srcTableId:%s, srcVer:0x%x, dstVer:0x%x\n",
           __FUNCTION__,srcDef->tableIdStr, srcVersion, dstVersion);
        return (E_TB_TABLEID);
    }
    
    return (0);
}  /* tbVerMapTableId() */



#define TB_CPY_TOKEN(dstTk, srcTk)              \
{                                               \
    copied = FALSE;                             \
    if (srcTk->pres)                            \
    {                                           \
        if (!dstTk->pres)                       \
        {                                       \
            *dstTk = *srcTk;                      \
            copied = TRUE;                      \
        }                                       \
        else                                    \
        {                                       \
            /* skip, do not overwrite */        \
        }                                       \
    }                                           \
    else                                        \
    {                                           \
        /* skip */                              \
    }                                           \
}

int
tbVerDfltMapRow(void *dst, DbTableDef *dstDef, void *src, DbTableDef *srcDef)
{
    int i, j, copied, numCopied=0, numSkipped1=0, numSkipped2=0, numSkipped3=0;
    unsigned char *srcPtr, *dstPtr;
    TkS8    *srcTkS8, *dstTkS8;
    TkU8    *srcTkU8, *dstTkU8;
    TkS16   *srcTkS16, *dstTkS16; 
    TkU16   *srcTkU16, *dstTkU16;
    TkS32   *srcTkS32, *dstTkS32;
    TkU32   *srcTkU32, *dstTkU32;
    TkStr8  *srcTkStr8, *dstTkStr8;
    TkStr16 *srcTkStr16, *dstTkStr16;
    TkStr32 *srcTkStr32, *dstTkStr32;
    TkStr64 *srcTkStr64, *dstTkStr64;
    TkStr128 *srcTkStr128, *dstTkStr128;
    TkStr256 *srcTkStr256, *dstTkStr256;

    if (src && dst && srcDef && dstDef)
    {
        if(srcDef==dstDef) /* skip column mapping if their definitions are the same */
        {
             unsigned short offset = sizeof(int)*2; /* skip header: int tableId && int version */
             memcpy((unsigned char *)dst+offset, (unsigned char *)src+offset, srcDef->rowSize-offset);
             return (0);
        }

        for (i=0; i<srcDef->numCols; i++)
        {
            if(srcDef->dbColDef[i] == NULL)
            {
                /* increment the match not found counter */
                numSkipped3++;
                continue;
            }
            srcPtr = (unsigned char *)src+srcDef->dbColDef[i]->offset;
            if (srcDef->dbColDef[i]->width >= MAX_WIDTH_ENUM)
            {
                DP("invalid columnType:0x%x\n", srcDef->dbColDef[i]->width);
                return (E_TB_COLTYPE);
            }
            
            for (j=0; j<dstDef->numCols; j++)
            {
                if (dstDef->dbColDef[j] == NULL)
                {
                    continue;
                }
                if (strcmp(srcDef->dbColDef[i]->name, dstDef->dbColDef[j]->name))
                {
                    continue;
                }
                dstPtr = (unsigned char *)dst+dstDef->dbColDef[j]->offset;
                if (srcDef->dbColDef[i]->width != dstDef->dbColDef[j]->width)
                {
                    DP("type for %s, src(%d), dst(%d) does not match\n", 
                       srcDef->dbColDef[i]->name, srcDef->dbColDef[i]->width, 
                       dstDef->dbColDef[j]->width);
                    /* -tg- account for size changes, compatibility change */
                    /* increment the match found but not copied count */
                    numSkipped1++;
                    break;
                }
                /* match found */
                switch(srcDef->dbColDef[i]->width)
                {
                case S8BITS:
                    srcTkS8 = (TkS8*)srcPtr;
                    dstTkS8 = (TkS8*)dstPtr;
                    TB_CPY_TOKEN(dstTkS8, srcTkS8);
                    break;
                case U8BITS:
                    srcTkU8 = (TkU8*)srcPtr;
                    dstTkU8 = (TkU8*)dstPtr;
                    TB_CPY_TOKEN(dstTkU8, srcTkU8);
                    break;
                case S16BITS:
                    srcTkS16 = (TkS16*)srcPtr;
                    dstTkS16 = (TkS16*)dstPtr;
                    TB_CPY_TOKEN(dstTkS16, srcTkS16);
                    break;
                case U16BITS:
                    srcTkU16 = (TkU16*)srcPtr;
                    dstTkU16 = (TkU16*)dstPtr;
                    TB_CPY_TOKEN(dstTkU16, srcTkU16);
                    break;
                case S32BITS:
                    srcTkS32 = (TkS32*)srcPtr;
                    dstTkS32 = (TkS32*)dstPtr;
                    TB_CPY_TOKEN(dstTkS32, srcTkS32);
                    break;
                case U32BITS:
                    srcTkU32 = (TkU32*)srcPtr;
                    dstTkU32 = (TkU32*)dstPtr;
                    TB_CPY_TOKEN(dstTkU32, srcTkU32);
                    break;
                case STR8:
                    srcTkStr8 = (TkStr8*)srcPtr;
                    dstTkStr8 = (TkStr8*)dstPtr;
                    TB_CPY_TOKEN(dstTkStr8, srcTkStr8); 
                    break;
                case STR16:
                    srcTkStr16 = (TkStr16*)srcPtr;
                    dstTkStr16 = (TkStr16*)dstPtr;
                    TB_CPY_TOKEN(dstTkStr16, srcTkStr16); 
                    break;
                case STR32:
                    srcTkStr32 = (TkStr32*)srcPtr;
                    dstTkStr32 = (TkStr32*)dstPtr;
                    TB_CPY_TOKEN(dstTkStr32, srcTkStr32); 
                    break;
                case STR64:
                    srcTkStr64 = (TkStr64*)srcPtr;
                    dstTkStr64 = (TkStr64*)dstPtr;
                    TB_CPY_TOKEN(dstTkStr64, srcTkStr64); 
                    break;
                case STR128:
                    srcTkStr128 = (TkStr128*)srcPtr;
                    dstTkStr128 = (TkStr128*)dstPtr;
                    TB_CPY_TOKEN(dstTkStr128, srcTkStr128); 
                    break;
                case STR256:
                    srcTkStr256 = (TkStr256*)srcPtr;
                    dstTkStr256 = (TkStr256*)dstPtr;
                    TB_CPY_TOKEN(dstTkStr256, srcTkStr256); 
                    break;
                default:
                    return(E_TB_COLTYPE);
                    break;
                }
                if (copied)
                {
                    numCopied++;
                }
                else
                {
                    /* increment the match found but not copied count */
                    numSkipped1++;
                }
#if 0
                DP("match found, %s col:%s\n", copied?"copied":"skipped", 
                   srcDef->dbColDef[i]->name);
#endif
                break;
            } /* search in dst */
            if (j==dstDef->numCols)
            {
                /* increment the match not found counter */
                numSkipped2++;
            }
        } /* for all cols in src */
        DP("mapped %d columns, match found but not copied:%d, no match:%d, missing src def:%d, table:%s\n",
           numCopied, numSkipped1, numSkipped2, numSkipped3, dstDef->tableIdStr);
    }
    else
    {
        return (E_TB_NULLPTR);
    }
    
    return (0);
} /* tbVerDfltMapRow() */



/* PLM_IN */


#define TB_CMP_STR(tk1, tk2, size) \
            if (tk2->pres) \
            { \
	      if (!(tk1->pres)) \
		*((U8 *)ptr_diff) = 1; \
	      else \
		{ \
		  U8 len1 = strlen(tk1->val); \
		  U8 len2 = strlen(tk2->val); \
  \
		  if (len1 != len2) \
		    *((U8 *)ptr_diff) = 1; \
		  else if (len1 && (len1 <= size)) \
		    { \
		      if (memcmp((U8 *)(tk1->val), (U8 *)(tk2->val), len1) == 0) \
			*((U8 *)ptr_diff) = 0; \
		      else \
			*((U8 *)ptr_diff) = 1; \
		    } \
		  else \
		    *((U8 *)ptr_diff) = 0; \
		} \
	    } \
	    else if (tk1->pres) \
              *((U8 *)ptr_diff) = 1; \
            else \
	      *((U8 *)ptr_diff) = 0;



#define TB_VALIDATE_STR(token) \
{\
    if (token->pres)\
    {\
        if (tabDef->dbColDef[i]->validationType==DB_RANGE)\
        {\
            if (tabDef->dbColDef[i]->width >= STR8)\
            {\
                if ((strlen((char *)token->val)<tabDef->dbColDef[i]->min)\
                    || (strlen((char *)token->val)>tabDef->dbColDef[i]->max)) \
                {\
                    DP("column:%s, intput val %s is out of range(%d,%d)\n", \
                        tabDef->dbColDef[i]->name, \
                        token->val, \
                        tabDef->dbColDef[i]->min, \
                        tabDef->dbColDef[i]->max); \
                    error = E_TB_RANGE;\
                    break;\
                }\
            }\
        }\
        else if ((tabDef->dbColDef[i]->validationType==DB_LIST)\
                 && (tabDef->dbColDef[i]->width >= STR8))\
        {\
            if ((tabDef->dbColDef[i]->numLst==0)\
                || (tabDef->dbColDef[i]->list==NULL))\
            {\
                error = E_TB_LIST;\
                break;\
            }\
            for (j=0;\
                 (j<tabDef->dbColDef[i]->numLst \
                  && (token->val != /* GR303 */\
                      ((typeof(token->val) *)tabDef->dbColDef[i]->list)[j]));\
                 j++);\
            \
            if (j==(tabDef->dbColDef[i]->numLst)) \
            {\
                error = E_TB_LIST;\
                break;\
            }\
        }\
    } /* present */\
    else if (tabDef->dbColDef[i]->optFlgs==DB_MAND)\
    {\
        error = E_TB_MISSING_MAND;\
        break;\
    }\
    else if (tabDef->dbColDef[i]->optFlgs==DB_OPT_DEF)\
    {\
        if (tabDef->dbColDef[i]->defaultVal)\
        {\
            strncpy(&token->val[0], (char *)&tabDef->dbColDef[i]->defaultVal,\
                    tkStrLen[tabDef->dbColDef[i]->width]-1);\
                    token->val[tabDef->dbColDef[i]->width] = '\0';\
                    token->pres = TRUE;\
            DP("defaulted %d to %s\n", i, token->val); \
        }\
    }\
}


#define TB_VALIDATE(token) \
{\
    if (token->pres)\
    {\
        if (tabDef->dbColDef[i]->validationType==DB_RANGE)\
        {\
            if (tabDef->dbColDef[i]->width < STR8)\
            {\
                if ((token->val<tabDef->dbColDef[i]->min)\
                    || (token->val>tabDef->dbColDef[i]->max)) \
                {\
                    DP("column:%s, input val %d is out of range(%d,%d)\n", \
                        tabDef->dbColDef[i]->name, \
                        token->val, \
                        tabDef->dbColDef[i]->min, \
                        tabDef->dbColDef[i]->max); \
                    error = E_TB_RANGE;\
                    break;\
                }\
            }\
        }\
        else if ((tabDef->dbColDef[i]->validationType==DB_LIST)\
                 && (tabDef->dbColDef[i]->width < STR8))\
        {\
            if ((tabDef->dbColDef[i]->numLst==0)\
                || (tabDef->dbColDef[i]->list==NULL))\
            {\
                error = E_TB_LIST;\
                break;\
            }\
            for (j=0;\
                 (j<tabDef->dbColDef[i]->numLst \
                  && (token->val != /* GR303 */\
                      ((typeof(token->val) *)tabDef->dbColDef[i]->list)[j]));\
                 j++);\
            \
            if (j==tabDef->dbColDef[i]->numLst)\
            {\
                error = E_TB_LIST;\
                break;\
            }\
        }\
    } /* present */\
    else if (tabDef->dbColDef[i]->optFlgs==DB_MAND)\
    {\
        error = E_TB_MISSING_MAND;\
        break;\
    }\
    else if (tabDef->dbColDef[i]->optFlgs==DB_OPT_DEF)\
    {\
        token->val = (typeof(token->val)) tabDef->dbColDef[i]->defaultVal;\
        token->pres = TRUE;\
    }\
}


int
tbValRow(void *row, int flags, int *column, char **errStr)
{
    int i, j, tableId, error = 0;
    unsigned char *ptr;
    DbTableDef *tabDef;
    TkS8    *tkS8;
    TkU8    *tkU8;
    TkS16   *tkS16; 
    TkU16   *tkU16;
    TkS32   *tkS32;
    TkU32   *tkU32;
    TkStr8  *tkStr8;
    TkStr16 *tkStr16;
    TkStr32 *tkStr32;
    TkStr64 *tkStr64;
    TkStr128 *tkStr128;
    TkStr256 *tkStr256;
    char     *locErrStr = NULL;

    *column = 0;
    if (errStr)
    {
        *errStr = NULL;
    }
    if (row)
    {
        tableId = *((int*) row);
        if ((tableId < TABLE_ID_OFFSET) || (tableId >= TELICA_MAX_TAB_ID))
        {
            DP("invalid tableId:0x%x\n", tableId);
            *column = tableId;
            error = E_TB_TABLEID;
            goto val_error;
        }

        tabDef = tableDefs[tableId-TABLE_ID_OFFSET];
        // DP("tableinput tableId %d,%d\n",tableId,TELICA_MAX_TAB_ID);
        if (!tabDef)
        {
            DP("table not defined\n");
            error = E_TB_NULLPTR;
            goto val_error;
        }
        for (i=0; i<tabDef->numCols; i++)
        {
            if ((flags==DB_DELETE) && (tabDef->numKeys>=i))
            {
                /* only check for keys for delete */
                break;
            }
            error = 0;
            if(tabDef->dbColDef[i] == NULL)
            {
                continue;
            }
            ptr = (unsigned char *)row+tabDef->dbColDef[i]->offset;
            if (tabDef->dbColDef[i]->width >= MAX_WIDTH_ENUM)
            {
                DP("invalid columnType:0x%x\n", tabDef->dbColDef[i]->width);
                error = E_TB_COLTYPE;
                goto val_error;
            }
            
            switch(tabDef->dbColDef[i]->width)
            {
            case S8BITS:
                tkS8 = (TkS8*)ptr;
                TB_VALIDATE(tkS8);
                break;
            case U8BITS:
                tkU8 = (TkU8*)ptr;
                TB_VALIDATE(tkU8);
                break;
            case S16BITS:
                tkS16 = (TkS16*)ptr;
                TB_VALIDATE(tkS16);
                break;
            case U16BITS:
                tkU16 = (TkU16*)ptr;
                TB_VALIDATE(tkU16);
                break;
            case S32BITS:
                tkS32 = (TkS32*)ptr;
                TB_VALIDATE(tkS32);
                break;
            case U32BITS:
                tkU32 = (TkU32*)ptr;
                TB_VALIDATE(tkU32);
                break;
            case STR8:
                tkStr8 = (TkStr8*)ptr;
                TB_VALIDATE_STR(tkStr8); 
                break;
            case STR16:
                tkStr16 = (TkStr16*)ptr;
                TB_VALIDATE_STR(tkStr16); 
                break;
            case STR32:
                tkStr32 = (TkStr32*)ptr;
                TB_VALIDATE_STR(tkStr32); 
                break;
            case STR64:
                tkStr64 = (TkStr64*)ptr;
                TB_VALIDATE_STR(tkStr64); 
                break;
            case STR128:
                tkStr128 = (TkStr128*)ptr;
                TB_VALIDATE_STR(tkStr128); 
                break;
            case STR256:
                tkStr256 = (TkStr256*)ptr;
                TB_VALIDATE_STR(tkStr256); 
                break;
            default:
                error = E_TB_COLTYPE;
                break;
            }

            if (error)
            {


                if ( i < tabDef->numKeys)
                {
                    DP ("Error in key =  %d %s \n", 
                        error, tabDef->dbColDef[i]->name);
                    error = E_TB_KEY;
                    goto val_error;
                }

                DP("error:%d, column:%s\n", error, tabDef->dbColDef[i]->name);
                *column = i;
                goto val_error;
            }
        }
    }
    else
    {
        error = E_TB_NULLPTR;
        goto val_error;
    }

    /* call extra check functions for each column */
    for (i=0; i<tabDef->numCols; i++)
    {
        if ((flags==DB_DELETE) && (tabDef->numKeys>=i))
        {
            /* only check for keys for delete */
            break;
        }
        error = 0;
        if(tabDef->dbColDef[i] == NULL)
        {
            continue;
        }
        ptr = (unsigned char *)row+tabDef->dbColDef[i]->offset;
        if (tabDef->dbColDef[i]->extraVal)
        {
            error=(*tabDef->dbColDef[i]->extraVal)(row, flags, &i, &locErrStr);
            if (error)
            {
                /* check if the column validation function did not set the error column */
                if (!*column)
                {
                    *column = i;
                }
                /* check if the column validation function did not set the error string */
                if (!*locErrStr)
                {
                    locErrStr = "extra column validation function failed";
                }
                goto val_error;
            }
        }
    }
    
    return (0);

val_error:
    if (errStr)
    {
        *errStr = locErrStr;
    }
    return (error);

} /* tbValRow() */
                    


int
tbMapRow(void *dst, void *src)
{
    int i, j, srcTblId, dstTblId, copied;
    unsigned char *srcPtr, *dstPtr;
    DbTableDef *srcDef, *dstDef;
    TkS8    *srcTkS8, *dstTkS8;
    TkU8    *srcTkU8, *dstTkU8;
    TkS16   *srcTkS16, *dstTkS16; 
    TkU16   *srcTkU16, *dstTkU16;
    TkS32   *srcTkS32, *dstTkS32;
    TkU32   *srcTkU32, *dstTkU32;
    TkStr8  *srcTkStr8, *dstTkStr8;
    TkStr16 *srcTkStr16, *dstTkStr16;
    TkStr32 *srcTkStr32, *dstTkStr32;
    TkStr64 *srcTkStr64, *dstTkStr64;
    TkStr128 *srcTkStr128, *dstTkStr128;
    TkStr256 *srcTkStr256, *dstTkStr256;

    if (src && dst)
    {
        srcTblId = *((int*) src);
        if (srcTblId >= TELICA_MAX_TAB_ID)
        {
            DP("invalid tableId:0x%x\n", srcTblId);
            return (E_TB_TABLEID);
        }
        dstTblId = *((int*) dst);
        if (dstTblId >= TELICA_MAX_TAB_ID)
        {
            DP("invalid tableId:0x%x\n", dstTblId);
            return (E_TB_TABLEID);
        }
        srcDef = tableDefs[srcTblId-TABLE_ID_OFFSET];
        dstDef = tableDefs[dstTblId-TABLE_ID_OFFSET];
        DP("tableinput src:%d dst:%d\n", srcTblId, dstTblId);
        if (!srcDef || !dstDef)
        {
            DP("table not defined, srcDef:0x%x, dstDef:0x%x\n", (int)srcDef, (int)dstDef);
            return(E_TB_NULLPTR);
        }
        for (i=0; i<srcDef->numCols; i++)
        {
            if(srcDef->dbColDef[i] == NULL)
            {
                continue;
            }
            srcPtr = (unsigned char *)src+srcDef->dbColDef[i]->offset;
            if (srcDef->dbColDef[i]->width >= MAX_WIDTH_ENUM)
            {
                DP("invalid columnType:0x%x\n", srcDef->dbColDef[i]->width);
                return (E_TB_COLTYPE);
            }
            
            for (j=0; j<dstDef->numCols; j++)
            {
                if (strcmp(srcDef->dbColDef[i]->name, dstDef->dbColDef[j]->name))
                {
                    continue;
                }
                dstPtr = (unsigned char *)dst+dstDef->dbColDef[j]->offset;
                if (srcDef->dbColDef[i]->width != dstDef->dbColDef[j]->width)
                {
                    DP("type for %s, src(%d), dst(%d) does not match\n", 
                       srcDef->dbColDef[i]->name, srcDef->dbColDef[i]->width, 
                       dstDef->dbColDef[j]->width);
                    break;
                }
                /* match found */
                switch(srcDef->dbColDef[i]->width)
                {
                case S8BITS:
                    srcTkS8 = (TkS8*)srcPtr;
                    dstTkS8 = (TkS8*)dstPtr;
                    TB_CPY_TOKEN(dstTkS8, srcTkS8);
                    break;
                case U8BITS:
                    srcTkU8 = (TkU8*)srcPtr;
                    dstTkU8 = (TkU8*)dstPtr;
                    TB_CPY_TOKEN(dstTkU8, srcTkU8);
                    break;
                case S16BITS:
                    srcTkS16 = (TkS16*)srcPtr;
                    dstTkS16 = (TkS16*)dstPtr;
                    TB_CPY_TOKEN(dstTkS16, srcTkS16);
                    break;
                case U16BITS:
                    srcTkU16 = (TkU16*)srcPtr;
                    dstTkU16 = (TkU16*)dstPtr;
                    TB_CPY_TOKEN(dstTkU16, srcTkU16);
                    break;
                case S32BITS:
                    srcTkS32 = (TkS32*)srcPtr;
                    dstTkS32 = (TkS32*)dstPtr;
                    TB_CPY_TOKEN(dstTkS32, srcTkS32);
                    break;
                case U32BITS:
                    srcTkU32 = (TkU32*)srcPtr;
                    dstTkU32 = (TkU32*)dstPtr;
                    TB_CPY_TOKEN(dstTkU32, srcTkU32);
                    break;
                case STR8:
                    srcTkStr8 = (TkStr8*)srcPtr;
                    dstTkStr8 = (TkStr8*)dstPtr;
                    TB_CPY_TOKEN(dstTkStr8, srcTkStr8); 
                    break;
                case STR16:
                    srcTkStr16 = (TkStr16*)srcPtr;
                    dstTkStr16 = (TkStr16*)dstPtr;
                    TB_CPY_TOKEN(dstTkStr16, srcTkStr16); 
                    break;
                case STR32:
                    srcTkStr32 = (TkStr32*)srcPtr;
                    dstTkStr32 = (TkStr32*)dstPtr;
                    TB_CPY_TOKEN(dstTkStr32, srcTkStr32); 
                    break;
                case STR64:
                    srcTkStr64 = (TkStr64*)srcPtr;
                    dstTkStr64 = (TkStr64*)dstPtr;
                    TB_CPY_TOKEN(dstTkStr64, srcTkStr64); 
                    break;
                case STR128:
                    srcTkStr128 = (TkStr128*)srcPtr;
                    dstTkStr128 = (TkStr128*)dstPtr;
                    TB_CPY_TOKEN(dstTkStr128, srcTkStr128); 
                    break;
                case STR256:
                    srcTkStr256 = (TkStr256*)srcPtr;
                    dstTkStr256 = (TkStr256*)dstPtr;
                    TB_CPY_TOKEN(dstTkStr256, srcTkStr256); 
                    break;
                default:
                    return(E_TB_COLTYPE);
                    break;
                }
                DP("match found, %s col:%s\n", copied?"copied":"skipped", 
                   srcDef->dbColDef[i]->name);                
                break;
            } /* search in dst */
        } /* for all cols in src */
    }
    else
    {
        return (E_TB_NULLPTR);
    }

    return (0);
} /* tbMapRow() */

/* initially designed for ED-ISDN-IF */
int
tbSortRowUpdate(void *old, void *new, void *diff, int *column)
{
    int ret=0;
    int i, tableId, error = 0;
    DbTableDef *tabDef;
    U8    *ptr_old, *ptr_new, *ptr_diff;
    TkStr8  *tkStr_old, *tkStr_new;
    U8      token_is_str = 0;
    int     size = 0;

    *column = 0;

    if (!old || !new)
      {      
	DP("Error: NULL pointers :old= 0x%x, new=0x%x\n", (int) old, (int) new);
	*column = 0;
	return (E_TB_NULLPTR);
      }

    if (*((int*) old) != *((int*) new))
      {      
	DP("unmatched old tableId:0x%x, new tableId:0x%x\n",
	   *((int*) old),  *((int*) new));
	*column = *((int*) old);
	return (E_TB_TABLEID);
      }

    tableId = *((int*) old);
    if (tableId >= TELICA_MAX_TAB_ID)
      {
	DP("invalid tableId:0x%x\n", tableId);
	*column = tableId;
	return (E_TB_TABLEID);
      }

    tabDef = tableDefs[tableId-TABLE_ID_OFFSET];
    if (!tabDef)
      {
	DP("table not defined.\n");
	return(E_TB_NULLPTR);
      }
    for (i=0; i<tabDef->numCols; i++)
        {
	  /* reset variables */
	    error = 0;
	    token_is_str = 0; 

            if(tabDef->dbColDef[i] == NULL)
            {
                continue;
            }
            ptr_old = (unsigned char *)old+tabDef->dbColDef[i]->offset;
	    ptr_new = (unsigned char *)new+tabDef->dbColDef[i]->offset;
	    ptr_diff = (unsigned char *)diff+tabDef->dbColDef[i]->offset;
            if (tabDef->dbColDef[i]->width >= MAX_WIDTH_ENUM)
            {
                DP("invalid columnType:0x%x\n", tabDef->dbColDef[i]->width);
                error = E_TB_COLTYPE;
                return (error);
            }
            switch(tabDef->dbColDef[i]->width)
            {
            case S8BITS:
            case U8BITS:
	      size =  sizeof(TkS8);
	      break;
            case S16BITS:
            case U16BITS:
	      size =  sizeof(TkS16);
	      break;
	    case S32BITS:
	    case U32BITS:
	      size =  sizeof(TkS32);
	      break;
            case STR8:
	      token_is_str = 1;
	      size = sizeof(TkStr8) -1;
	      break;
            case STR16:
	      token_is_str = 1;
	      size = sizeof(TkStr16) -1;
	      break;
            case STR32:
	      token_is_str = 1;
	      size = sizeof(TkStr32) -1;
	      break;
            case STR64:
	      token_is_str = 1;
	      size = sizeof(TkStr64) -1;  
	      break;
            case STR128:
	      token_is_str = 1;
	      size = sizeof(TkStr128) -1;  
	      break;
            case STR256:
	      token_is_str = 1;
	      size = sizeof(TkStr256) -1;  
	      break;
            default:
                error = E_TB_COLTYPE;
                break;
            }

            if (error)
            {
                DP("error:%d, column:%s\n", error, tabDef->dbColDef[i]->name);
                *column = i;
                return (error);
            }
	    if (token_is_str)
	      {
		tkStr_old = (TkStr8*)ptr_old;
		tkStr_new = (TkStr8*)ptr_new;
		TB_CMP_STR(tkStr_old, tkStr_new, size);
	      }
	    else
	      {
		ret = memcmp((void *)ptr_old, (void *)ptr_new, size) ;
		if (ret && (*ptr_new)) /* check pres */
		  *ptr_diff = 1;
		else 
		  *ptr_diff = 0;
	      }
#if 1 /* debug only */
	    DP("colId: %s, type: 0x%x,token_is_str= %d, old = %d, new = %d, size = %d, ret = %d, diff= %d\n", tabDef->dbColDef[i]->name, 
	       tabDef->dbColDef[i]->width, token_is_str, *(ptr_old + 1), *(ptr_new + 1), size, ret, *ptr_diff);
#endif
        }
   
    return (0);
} /* tbSortRowUpdate() */



#define TB_UPDATE_TOKEN(updateTk, dbTk)    \
{                                          \
    updated = FALSE;                       \
    if (updateTk->pres == DB_TRUE)         \
    {                                      \
        *dbTk = *updateTk;                 \
        updated = TRUE;                    \
    }                                      \
    else if(updateTk->pres == DB_RESET)    \
    {                                      \
        dbTk->pres = DB_FALSE;            \
        updated = TRUE;                    \
    }                                      \
}

/**********************************************************************
 **
 **   Func: tbUpdateRow
 **
 **   Desc: This function is added for SLAM functionality in MGC
 **         For TL1 ED command, updateRow decoded from TL1 command
 **         is copied over the existing record in DB.  
 ** 
 *********************************************************************/

int
tbUpdateRow(void *updateRow, void *dbRow)
{
    int            i, updateTblId, dbTblId, updated=FALSE;
    unsigned char  *updatePtr, *dbPtr;
    DbTableDef     *updateDef, *dbDef;
    TkS8           *updateTkS8, *dbTkS8;
    TkU8           *updateTkU8, *dbTkU8;
    TkS16          *updateTkS16, *dbTkS16; 
    TkU16          *updateTkU16, *dbTkU16;
    TkS32          *updateTkS32, *dbTkS32;
    TkU32          *updateTkU32, *dbTkU32;
    TkStr8         *updateTkStr8, *dbTkStr8;
    TkStr16        *updateTkStr16, *dbTkStr16;
    TkStr32        *updateTkStr32, *dbTkStr32;
    TkStr64        *updateTkStr64, *dbTkStr64;
    TkStr128       *updateTkStr128, *dbTkStr128;
    TkStr256       *updateTkStr256, *dbTkStr256;
    


    if (updateRow && dbRow)
    {
        updateTblId = *((int*) updateRow);
        if (updateTblId >= TELICA_MAX_TAB_ID)
        {
            DP("invalid tableId:0x%x\n", updateTblId);
            return (E_TB_TABLEID);
        }
        dbTblId = *((int*) dbRow);
        if (dbTblId >= TELICA_MAX_TAB_ID)
        {
            DP("invalid tableId:0x%x\n", dbTblId);
            return (E_TB_TABLEID);
        }
        updateDef = tableDefs[updateTblId-TABLE_ID_OFFSET];
        dbDef = tableDefs[dbTblId-TABLE_ID_OFFSET];
        DP("tableinput update:%d db:%d\n", updateTblId, dbTblId);
        if (!updateDef || !dbDef)
        {
            DP("table not defined, updateDef:0x%x, dbDef:0x%x\n", (int)updateDef, (int)dbDef);
            return(E_TB_NULLPTR);
        }

        DP("kavon updateDef->numCols = %d\n", updateDef->numCols);
        for (i=0; i<updateDef->numCols; i++)
         {
            if(updateDef->dbColDef[i] == NULL)
            {
                continue;
            }
            updatePtr = (unsigned char *)updateRow+updateDef->dbColDef[i]->offset;
            if (updateDef->dbColDef[i]->width >= MAX_WIDTH_ENUM)
            {
                DP("invalid columnType:0x%x\n", updateDef->dbColDef[i]->width);
                return (E_TB_COLTYPE);
            }
            dbPtr = (unsigned char *)dbRow+dbDef->dbColDef[i]->offset;
            if (dbDef->dbColDef[i]->width >= MAX_WIDTH_ENUM)
            {
                DP("invalid columnType:0x%x\n", dbDef->dbColDef[i]->width);
                return (E_TB_COLTYPE);
            }

            switch(updateDef->dbColDef[i]->width)
            {
            case S8BITS:
                updateTkS8 = (TkS8*)updatePtr;
                dbTkS8 = (TkS8*)dbPtr;
                TB_UPDATE_TOKEN(updateTkS8, dbTkS8);
                break;
            case U8BITS:
                updateTkU8 = (TkU8*)updatePtr;
                dbTkU8 = (TkU8*)dbPtr;
                DP("kavon col-%d %s U8BITS: updateTkU8->pres = %d\n", i, updateDef->dbColDef[i]->name, updateTkU8->pres);
                DP("kavon col-%d %s U8BITS: updateTkU8->val = %d\n", i, updateDef->dbColDef[i]->name, updateTkU8->val);
                TB_UPDATE_TOKEN(updateTkU8, dbTkU8);
                break;
            case S16BITS:
                updateTkS16 = (TkS16*)updatePtr;
                dbTkS16 = (TkS16*)dbPtr;
                TB_UPDATE_TOKEN(updateTkS16, dbTkS16);
                break;
            case U16BITS:
                updateTkU16 = (TkU16*)updatePtr;
                dbTkU16 = (TkU16*)dbPtr;
                DP("kavon col-%d %s U16BITS: updateTkU16->pres = %d\n", i, updateDef->dbColDef[i]->name, updateTkU16->pres);
                DP("kavon col-%d %s U16BITS: updateTkU16->val = %d\n", i, updateDef->dbColDef[i]->name, updateTkU16->val);
                TB_UPDATE_TOKEN(updateTkU16, dbTkU16);
                break;
            case S32BITS:
                updateTkS32 = (TkS32*)updatePtr;
                dbTkS32 = (TkS32*)dbPtr;
                TB_UPDATE_TOKEN(updateTkS32, dbTkS32);
                break;
            case U32BITS:
                updateTkU32 = (TkU32*)updatePtr;
                dbTkU32 = (TkU32*)dbPtr;
                DP("kavon col-%d %s update pre: updateTkU32->pres = %d\n", i, updateDef->dbColDef[i]->name, updateTkU32->pres);
                DP("kavon col-%d %s update val: updateTkU32->val = %d\n", i, updateDef->dbColDef[i]->name, updateTkU32->val);
                DP("kavon col-%d %s db: dbTkU32->val = %d\n", i, updateDef->dbColDef[i]->name, dbTkU32->val);
                TB_UPDATE_TOKEN(updateTkU32, dbTkU32);
                break;
            case STR8:
                updateTkStr8 = (TkStr8*)updatePtr;
                dbTkStr8 = (TkStr8*)dbPtr;
                TB_UPDATE_TOKEN(updateTkStr8, dbTkStr8);
                break;
            case STR16:
                updateTkStr16 = (TkStr16*)updatePtr;
                dbTkStr16 = (TkStr16*)dbPtr;
                TB_UPDATE_TOKEN(updateTkStr16, dbTkStr16);
                break;
            case STR32:
                updateTkStr32 = (TkStr32*)updatePtr;
                dbTkStr32 = (TkStr32*)dbPtr;
                TB_UPDATE_TOKEN(updateTkStr32, dbTkStr32);
                break;
            case STR64:
                updateTkStr64 = (TkStr64*)updatePtr;
                dbTkStr64 = (TkStr64*)dbPtr;
                TB_UPDATE_TOKEN(updateTkStr64, dbTkStr64);
                break;
            case STR128:
                updateTkStr128 = (TkStr128*)updatePtr;
                dbTkStr128 = (TkStr128*)dbPtr;
                TB_UPDATE_TOKEN(updateTkStr128, dbTkStr128);
                break;
            case STR256:
                updateTkStr256 = (TkStr256*)updatePtr;
                dbTkStr256 = (TkStr256*)dbPtr;
                TB_UPDATE_TOKEN(updateTkStr256, dbTkStr256);
                break;
            default:
                return(E_TB_COLTYPE);
                break;
            }

#if 1                           /* just debug purpose, clean up skipped logs */
            if(updated)
            {
                DP("updated col:%s\n",updateDef->dbColDef[i]->name);                
            }
#endif
        }
    }
    else
    {
        return (E_TB_NULLPTR);
    }

    return (0);

} /* tbUpdateRow */



/**********************************************************************
 **
 **   Func: tbInit
 **
 **   Desc: checks all table defintions
 ** 
 *********************************************************************/
int
tbInit(void)
{ 
    int          i, ret=0, tableId, startTabId, endTableId, currTableId, retval=0;
    unsigned short ver;
    DbTableDef  *tbDef;
    
    for (i=0; i< TB_MAX_VERSIONS; i++)
    {
        /* get src and dst  definition lists */    
        startTabId = tbVerCb[i].startTableId;
        endTableId = tbVerCb[i].endTableId;
        ver = tbVerCb[i].version;
        DP("tbInit: ver = 0x%x, i = %d\n",ver,i);

        
        for (tableId=startTabId;tableId<endTableId;tableId++)
        {
            tbFndVerDef(ver, tableId, &tbDef);
                
            if ((!tbDef) || (tbDef->dbColDef == 0))
            {
                DP("**warning:NULL defintion for table:%d : 0x%x\n", tableId, tableId);
                continue;
            }
            
            /* check index */
            /* First convert the tableId release to the tableId of the
               current release before comparing the index */
            retval = tbVerMapTableId(ver,tableId,TB_CUR_VER,&currTableId);
            if (retval != 0)
            {
                DP("**error:tbVerMapTableId() failed for version:%d,table:%d : 0x%x\n", ver, tableId, tableId);
                ret=3;
            }

            if (tbDef->tableId!=currTableId)
            {
                DP("**error:definition for tableId:%d : 0x%x is in bad index:%d\n", tbDef->tableId,tbDef->tableId,
                   tableId-TABLE_ID_OFFSET);
                ret=1;
            }
            
            /* check sizeof */
            if (!tbDef->rowSize)
            {
                DP("**error:rowSize is not set for tableId:%d : 0x%x\n", tableId, tableId);
                ret=2;
            }
        
            /* check column types to match up with the row struct -later- */
            /* Call another function here to validate the dbColDef structures */
            tbValidateCols(tbDef, (void **)tbDef->dbColDef, tbDef->numCols);
        }
    }
    
    return (ret);
} /* tbInit() */



int tbValidateCols(DbTableDef *tbDef, void ** colDef, unsigned short numCols)
{

    int i;
    /* int ctr=sizeof(int);*/
    /* int size;*/
    /* int idx; */
    int rowSize = 0;
    
    DbColDef **colDefPtr = (DbColDef **)colDef;
    rowSize = tbDef->rowSize;
    
    if (colDefPtr[0] == NULL)
    {
        DP("ColDefPtr NULL entry\n");
        return(0);
    }
    
    /* Verify first DbColDef entry's offset is 4 (tableId) */
    if ((colDefPtr[0] == NULL) && (colDefPtr[0]->offset != 2*sizeof(int)))
    {
        DP("error:First ColDefPtr offset not zero, but %d\n", colDefPtr[0]->offset);
    }
        
    for (i=0; i< numCols; i++)
    {
        if(colDefPtr[i] == NULL)
        {
            DP("   Col %d: Contains a NULL entry\n", i);
        }
        else
        {
#if ASM_TABLE_DEBUG
            DP("   Col %d: %s\n", i, colDefPtr[i]->name);
#endif
            if (colDefPtr[i]->offset >= rowSize)
            {
                DP("error:table:%s, offset:%d for column:%d,%s greater than rowSize:%d\n",
                   (tbDef->sqlTabStr?tbDef->sqlTabStr:"NOT SET"), colDefPtr[i]->offset,
                   i, colDefPtr[i]->name, rowSize);
            }
        }
    }
    
#if 0    
    /* Test the DbColDef arrays */
    for (i=1; i<numCols; i++)
    {
        if (colDefPtr[i] == NULL)
            continue;
        if (colDefPtr[i-1] == NULL)
            idx = i-2;
        else
            idx = i-1;

        switch(colDefPtr[idx]->width)
        {
        case U8BITS:
        case S8BITS:
            size = sizeof(TkS8);
            break;
        case U16BITS:
        case S16BITS:
            size = sizeof(TkS16);
            break;
        case U32BITS:
        case S32BITS:
            size = sizeof(TkS32);
            break;
        case STR8:
            size = sizeof(TkStr8);
            break;
        case STR16:
            size = sizeof(TkStr16);
            break;
        case STR32:
            size = sizeof(TkStr32);
            break;
        case STR64:
            size = sizeof(TkStr64);
            break;
        case STR128:
            size = sizeof(TkStr128);
            break;
        case STR256:
            size = sizeof(TkStr256);
            break;
        }
#if ASM_TABLE_DEBUG
        DP("       adding size %d to ctr\n", size);
#endif
        ctr += size;

#if ASM_TABLE_DEBUG

        if(colDefPtr[i]->offset != ctr)
        {
            DP("MISMATCH in DbColDef[%d] field %s  : offset = %d  ctr = %d\n",
               i, colDefPtr[i]->name, colDefPtr[i]->offset, ctr);  
        }
        else
        {
            DP("  MATCH in DbColDef[%d]  field %s  : offset = %d  ctr = %d\n",
               i, colDefPtr[i]->name, colDefPtr[i]->offset, ctr);    
        }
#endif        

        
        
    }
#endif
    return(0);
}

#if 1 /* 7.0.1: ISUP DDI dabao-Jan/2009 */
/* Enum to String */
char* tbEnum2Str(int val, TB_ENUM_NAME_LIST *enumlist)

{
    int i;

    if(!enumlist) return NULL;

    for (i = 0; i < enumlist->length; i++)
    {
        if(enumlist->list[i].enum_val == val)
        {
            return enumlist->list[i].name;
        }
    }
    return NULL;
}

/* String to Enum */
int tbStr2Enum(char *str,TB_ENUM_NAME_LIST *enumlist)
{
    int i;

    if ((!str) || (!enumlist)) return -1;

    for (i = 0; i < enumlist->length; i++)
    {
        if (strcmp(enumlist->list[i].name,str) == 0)
        {
            return enumlist->list[i].enum_val;
        }
    }
    return -1;
}


#endif
