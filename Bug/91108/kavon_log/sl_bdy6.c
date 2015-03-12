/**********************************************************************
**  Copyright (c) 1999 Telica.  All rights reserved.
**
** $Id: 
**
**  Name:  sl_bdy6.c
**
**  Desc:  This file contains the SLAM utility functions
**  
**  Modification History
**  Date         Prg   Description
**  ------------------------------------------------
**  06/19/2001   tg   Created
**
***********************************************************************/
/***** include files **************************************************/
/* LynxOs */
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <ctype.h>

#include "telica_abort.h"    /* for ABORT macro */

#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */

#include "gen.h"           /* general */
#include "ssi.h"           /* system services */
#include "cm_hash.h"       /* common hash functions */
#include "cm_err.h"        /* common functions error */
#include "ts.h"

/* header include -- typedef structs (.x) */

#include "gen.x"           /* general */
#include "ssi.x"           /* system services */
#include "cm_lib.x"        /* common library functions */
#include "cm_hash.x"       /* common hash functions */

#include "cm_queue.h"      /* common queue functions */
#include "cm_fsm.h"        /* common queue functions */

/* Telica table defintions */
#define TELICA_TABLE_IDS  0x1000
#include "tb.h"

/* TL1/SM row/column includes */
#include "sm_cmds.h"


/* SLAM includes */
#include "lm.h"
#include "sl.h"
#include "sl_tl1.h"
#include "sl_blk.h"
#include "sl_brd.h"
#include "log_global.h"
#include "proc_info.h"

#include "sl_smi.h"
#include "sl_iomau.h"

#ifdef ATCA
#include "lxr.h"
#include "sl_bkpm.h"
#include "tb_stgdmtp3pm.h"
#include "tb_stgdmtp2.h"
#include "tb_mtppm.h"
#endif

#include "cm_llist.x"
#include "trcSrv.h"
#include "sl_calltrc_fsm.h"
#include "sfed_evts.h"

/* for the getmulti requests */
extern int tsmcGetMultiBusy;

/* to support the getnextmulti */
extern int tsmcGetNextMultiFlag;
extern void tsmcGetNextMultiCleanContext ( void);
extern S16 isIomAuTL1Trans(SlTl1Cb *slTl1Cb);

S16 sendRespToEmf(SlTl1Cb *slTl1Cb, TsSmiErrorCode errorCode,
               TsSmiErrorIndex errorIndex, S16 tl1Error);

S16 sendRespToEmfNoTl1Cb(SFED_CMD_BUF_t *sfedCmdRcv, S16 tl1Error);

/**********************************************************************
** slDecodeSmiCols
**
**     Desc:  Decode row to table structure 
**
**     Notes: 1. undecodedCols can be NULL
**            2. input numCols in undecodedCols is the maximum number of 
**               columns that undecodedCols can hold
**            3. numCols in undecodedCols is set to the number of actual
**               undecoded columns
**            4. flags - indicates different treatment based on flag value
**     Ret:    0  - OK
**            >0 - see tb.h for a list of error codes
**
**********************************************************************/
S16
slDecodeSmiCols(U16 tableId, TsSmiColArray *colArray, void *row, 
                TsSmiColArray *undecodedCols, U16 *column, U8 flags)
{
    S16 colId, ret;
    U8 *ptr;
    S16 i, maxUndecodedCols;
    DbTableDef *tabDef;
    
    if (!(colArray && row))
    {
        return (E_TB_NULLPTR);
    }

    *(int *)row = tableId;

    if (tableId >= TELICA_MAX_TAB_ID)
    {
        SLM_LOG(SLM_ERR, "invalid tableId:0x%x\n", tableId);
        return (E_TB_TABLEID);
    }
    
    tabDef = tableDefs[tableId-TABLE_ID_OFFSET]; 
    if (!tabDef)
    {
        SLM_LOG(SLM_ERR, "table:%d not defined\n", tableId);
        return(E_TB_NULLPTR);
    }
    if (undecodedCols)
    {
        maxUndecodedCols = undecodedCols->numCols;
        undecodedCols->numCols = 0;
    }
    else
    {
        maxUndecodedCols = 0;
    }

    SLM_LOG(SLM_DBG, "flags:%d\n", flags);

    DP("kavon numCols in SlTl1Cb->colArray is %d\n", colArray->numCols); 
    for (i=0; i<colArray->numCols; i++)
    {
       S16 tmpColId = colArray->colArray[i].columnId;
       DP("kavon col id: %d\n", tmpColId);
       DP("kavon col name: %s\n", tabDef->dbColDef[tmpColId]->name);
       DP("kavon col type: %d\n", colArray->colArray[i].value.colType);
    }

    for (i=0; i<colArray->numCols; i++)
    {
        colId = colArray->colArray[i].columnId;
        if (colId >= tabDef->numCols)
        {
            if (maxUndecodedCols && (undecodedCols->numCols < maxUndecodedCols))
            {
                slCpySmiCol(&undecodedCols->colArray[undecodedCols->numCols++], &colArray->colArray[i]);
            }
            else
            {
                SLM_LOG(SLM_NFY, "lost colId:0x%x, no space left in undecoded colArray(%d)\n", colId, maxUndecodedCols);
            }
            continue;
        }
        if(tabDef->dbColDef[colId] == NULL)
        {
            /* missing column definition, skip decoding this column */
            continue;
        }
        ptr = (U8*)row+tabDef->dbColDef[colId]->offset; 
        if ((tabDef->dbColDef[colId]->width >= STR8) && 
            (colArray->colArray[i].value.colType != TSSMI_RESET)) /* bug:14388 - add check for TSSMI_RESET */
        {
            char debugStr[257];

            /* bug 64376 The maximun value of U8 is less than 257 *//* 
            if (colArray->colArray[i].value.value.stringVal.len < 257)
            {
            */
                strncpy(debugStr, colArray->colArray[i].value.value.stringVal.ptr,           
                        colArray->colArray[i].value.value.stringVal.len);
                debugStr[colArray->colArray[i].value.value.stringVal.len]='\0';
            /*
            }
            else
            {
                strncpy(debugStr, colArray->colArray[i].value.value.stringVal.ptr, 256);
                debugStr[256]='\0';
            }
             *//*Bug 64376 end */
#if 1 /* BUG 11499: vpochira: */
            if((strcmp(debugStr,"\"\"") == 0) ||
               (strcmp(debugStr,"NULL") == 0)) 
            {
/* 56314 - zeroing out string without freeing memory */
#if 1
                SLM_LOG(SLM_DBG, "converting stringVal:%s, len:%d to stringVal.len:0\n", 
                        debugStr, colArray->colArray[i].value.value.stringVal.len);
                ret = SPutSBuf(LM_REGION, LM_POOL,
                         colArray->colArray[i].value.value.stringVal.ptr,
                         colArray->colArray[i].value.value.stringVal.len);
                if (ret)
                {
                    SLM_LOG(SLM_ERR, "SPutSBuf ret:%d\n",ret);
                }
                colArray->colArray[i].value.value.stringVal.ptr = NULL;
#endif

                colArray->colArray[i].value.value.stringVal.len = 0;
	    }	
#endif /* BUG: 11499: end-chng */
            SLM_LOG(SLM_NFY, "decoded %s, type:%d, len:%d, stringVal:%s\n", 
                              tabDef->dbColDef[colId]->name, tabDef->dbColDef[colId]->width, 
                              colArray->colArray[i].value.value.stringVal.len, debugStr);
        }
        else
        {
#if 0
            SLM_LOG(SLM_NFY, "decoded %s, type:%d, longVal:%d\n", 
                              tabDef->dbColDef[colId]->name, tabDef->dbColDef[colId]->width, 
                              colArray->colArray[i].value.value.longVal);
#endif
        }
        if ((tabDef->dbColDef[colId]->width < STR8) 
            && (colArray->colArray[i].value.colType == TSSMI_OCTET_STRING))
        {
            SLM_LOG(SLM_NFY, "skipping %s, colId %d, mismatched type\n",
                    tabDef->dbColDef[colId]->name, colId);
            if (maxUndecodedCols && (undecodedCols->numCols < maxUndecodedCols))
            {
                slCpySmiCol(&undecodedCols->colArray[undecodedCols->numCols++], &colArray->colArray[i]);
            }
            else
            {
                SLM_LOG(SLM_NFY, "lost colId:0x%x, no space left in undecoded colArray(%d)\n", colId, maxUndecodedCols);
            }
            continue;
        }

        
        switch(tabDef->dbColDef[colId]->width)
        {
        case S8BITS:
            if (colArray->colArray[i].value.colType == TSSMI_RESET)
            {
                if(flags == DEFAULT)
                {
                    ((TkS8*)ptr)->pres = 0;
                }
                else if(flags == UPDATE_REQD)
                {
                    ((TkS8*)ptr)->pres = DB_RESET; /* MGC change: set pres flag to DB_RESET for TSSMI_RESET */
                }
            }
            else if(colArray->colArray[i].value.value.longVal & 0xFFFFFF00)
            {
                *column = colId;
                 SLM_LOG(SLM_ERR, "error in decode %d\n", colId);
                return (E_TB_COLTYPE); 
            }
            else
            {
            ((TkS8*)ptr)->pres = 1;
                ((TkS8*)ptr)->val = 
                    (S8)colArray->colArray[i].value.value.longVal;
            
            }
            break;
        case U8BITS:
            if (colArray->colArray[i].value.colType == TSSMI_RESET)
            {
                if(flags == DEFAULT)
                {
                    ((TkU8*)ptr)->pres = 0;
                }
                else if(flags == UPDATE_REQD)
                {
                    ((TkU8*)ptr)->pres = DB_RESET; /* MGC change: set pres flag to DB_RESET for TSSMI_RESET */
                }
            }
            else if(colArray->colArray[i].value.value.longVal & 0xFFFFFF00)
            {
                *column = colId;
                 SLM_LOG(SLM_ERR, "error in decode %d\n", colId);
                 SLM_LOG(SLM_NFY, "decoded %s, type:%d, longVal:%d\n", tabDef->dbColDef[colId]->name, 
                 tabDef->dbColDef[colId]->width, colArray->colArray[i].value.value.longVal);
                return (E_TB_COLTYPE); 
            }
            else
            {
            ((TkU8*)ptr)->pres = 1;
                ((TkU8*)ptr)->val = 
                    (U8)colArray->colArray[i].value.value.longVal;
            
             }
            break;     
        case S16BITS:
            if (colArray->colArray[i].value.colType == TSSMI_RESET)
            {
                if(flags == DEFAULT)
                {
                    ((TkS16*)ptr)->pres = 0;
                }
                else if(flags == UPDATE_REQD)
                {
                    ((TkS16 *)ptr)->pres = DB_RESET; /* MGC change: set pres flag to DB_RESET for TSSMI_RESET */
                }
            }
            else if(colArray->colArray[i].value.value.longVal & 0xFFFF0000)
            {
                *column = colId;
                 SLM_LOG(SLM_ERR, "error in decode %d\n", colId);
                return (E_TB_COLTYPE); 
            }
            else
            {
            ((TkS16*)ptr)->pres = 1;
                ((TkS16*)ptr)->val =
                    (S16)colArray->colArray[i].value.value.longVal;
            
            }
            break;
        case U16BITS:
            if (colArray->colArray[i].value.colType == TSSMI_RESET)
            {
                if(flags == DEFAULT)
                {
                    ((TkU16*)ptr)->pres = 0;
                }
                else if(flags == UPDATE_REQD)
                {
                    ((TkU16*)ptr)->pres = DB_RESET; /* MGC change: set pres flag to DB_RESET for TSSMI_RESET */
                }
            }
            else if(colArray->colArray[i].value.value.longVal & 0xFFFF0000)
            {
                *column = colId;
                 SLM_LOG(SLM_ERR, "error in decode %d\n", colId);
                return (E_TB_COLTYPE); 
            }
            else
            {
            ((TkU16*)ptr)->pres = 1;
                ((TkU16*)ptr)->val =
                    (U16)colArray->colArray[i].value.value.longVal;
            
            }
            break;
        case S32BITS:
            if (colArray->colArray[i].value.colType == TSSMI_RESET)
            {
                if(flags == DEFAULT)
                {
                    ((TkS32*)ptr)->pres = 0;
                }
                else if(flags == UPDATE_REQD)
                {
                    ((TkS32*)ptr)->pres = DB_RESET; /* MGC change: set pres flag to DB_RESET for TSSMI_RESET */
                }
            }
            else 
            {
            ((TkS32*)ptr)->pres = 1;
                ((TkS32*)ptr)->val =
                    (S32)colArray->colArray[i].value.value.longVal;
            
            }
            break;
        case U32BITS:
            if (colArray->colArray[i].value.colType == TSSMI_RESET)
            {
                if(flags == DEFAULT)
                {
                    ((TkU32*)ptr)->pres = 0;
                }
                else if(flags == UPDATE_REQD)
                {
                    ((TkU32*)ptr)->pres = DB_RESET; /* MGC change: set pres flag to DB_RESET for TSSMI_RESET */
                }
            }
            else 
            {
            ((TkU32*)ptr)->pres = 1;
                ((TkU32*)ptr)->val =
                    (U32)colArray->colArray[i].value.value.longVal;
            
            }
            break;
        case STR8:
#if 1 /* BUG 11499: vpochira: */
            if (colArray->colArray[i].value.value.stringVal.len == 0) {
               colArray->colArray[i].value.colType = TSSMI_RESET; 
	    }
#endif /* BUG: 11499: end-chng */
            if (colArray->colArray[i].value.colType == TSSMI_RESET)
            {
                if(flags == DEFAULT)
                {
                    ((TkStr8*)ptr)->pres = 0;
                }
                else if(flags == UPDATE_REQD)
                {
                    ((TkStr8*)ptr)->pres = DB_RESET; /* MGC change: set pres flag to DB_RESET for TSSMI_RESET */
                }
                /* Bug 15705 - support NULL value for TSSMI_RESET */
                colArray->colArray[i].value.value.stringVal.len = 0;
                ((TkStr8*)ptr)->val[colArray->colArray[i].value.value.stringVal.len] = '\0'; 
            }
            else if(colArray->colArray[i].value.value.stringVal.len > 8)
            {
                *column = colId;
                 SLM_LOG(SLM_ERR, "error in decode %d\n", colId);
                return (E_TB_COLTYPE);
            }
            else
            {
             ((TkStr8*)ptr)->pres = 1;
             strncpy(((TkStr8*)ptr)->val, 
                     colArray->colArray[i].value.value.stringVal.ptr,
                     colArray->colArray[i].value.value.stringVal.len);    
                ((TkStr8*)ptr)->val[colArray->colArray[i].value.value.stringVal.len] = 0;
            }
            break;
        case STR16:
#if 1 /* BUG 11499: vpochira: */
            if (colArray->colArray[i].value.value.stringVal.len == 0) {
               colArray->colArray[i].value.colType = TSSMI_RESET; 
	    }
#endif /* BUG: 11499: end-chng */
            if (colArray->colArray[i].value.colType == TSSMI_RESET)
            {
                if(flags == DEFAULT)
                {
                    ((TkStr16*)ptr)->pres = 0;
                }
                else if(flags == UPDATE_REQD)
                {
                    ((TkStr16*)ptr)->pres = DB_RESET; /* MGC change: set pres flag to DB_RESET for TSSMI_RESET */
                }
                /* Bug 15705 - support NULL value for TSSMI_RESET */
                colArray->colArray[i].value.value.stringVal.len = 0;
                ((TkStr16*)ptr)->val[colArray->colArray[i].value.value.stringVal.len] = '\0';
            }
            else  if(colArray->colArray[i].value.value.stringVal.len > 16)
            {
                *column = colId;
                 SLM_LOG(SLM_ERR, "error in decode %d\n", colId);
                return (E_TB_COLTYPE);
            }
            else
            {
            ((TkStr16*)ptr)->pres = 1;
            strncpy(((TkStr16*)ptr)->val, 
                    colArray->colArray[i].value.value.stringVal.ptr,
                    colArray->colArray[i].value.value.stringVal.len);   
                ((TkStr16*)ptr)->val [colArray->colArray[i].value.value.stringVal.len] = 0;
             
            }
            break;
        case STR32:
#if 1 /* BUG 11499: vpochira: */
            if (colArray->colArray[i].value.value.stringVal.len == 0) {
               colArray->colArray[i].value.colType = TSSMI_RESET; 
	    }
#endif /* BUG: 11499: end-chng */
            if (colArray->colArray[i].value.colType == TSSMI_RESET)
            {
                if(flags == DEFAULT)
                {
                    ((TkStr32*)ptr)->pres = 0;
                }
                else if(flags == UPDATE_REQD)
                {
                    ((TkStr32*)ptr)->pres = DB_RESET; /* MGC change: set pres flag to DB_RESET for TSSMI_RESET */
                }
                /* Bug 15705 - support NULL value for TSSMI_RESET */
                colArray->colArray[i].value.value.stringVal.len = 0;
                ((TkStr32*)ptr)->val[colArray->colArray[i].value.value.stringVal.len] = '\0';
            }
            else if(colArray->colArray[i].value.value.stringVal.len > 32)
            {
                *column = colId;
                 SLM_LOG(SLM_ERR, "error in decode %d\n", colId);
                return (E_TB_COLTYPE);
            }
            else
            {
            ((TkStr32*)ptr)->pres = 1;
            strncpy(((TkStr32*)ptr)->val, 
                    colArray->colArray[i].value.value.stringVal.ptr,
                    colArray->colArray[i].value.value.stringVal.len);  
                ((TkStr32*)ptr)->val [colArray->colArray[i].value.value.stringVal.len] = 0;
            }
            break;
        case STR64:
#if 1 /* BUG 11499: vpochira: */
            if (colArray->colArray[i].value.value.stringVal.len == 0) {
               colArray->colArray[i].value.colType = TSSMI_RESET; 
	    }
#endif /* BUG: 11499: end-chng */
            if (colArray->colArray[i].value.colType == TSSMI_RESET)
            {
                if(flags == DEFAULT)
                {
                    ((TkStr64*)ptr)->pres = 0;
                }
                else if(flags == UPDATE_REQD)
                {
                    ((TkStr64*)ptr)->pres = DB_RESET; /* MGC change: set pres flag to DB_RESET for TSSMI_RESET */
                }
                /* Bug 15705 - support NULL value for TSSMI_RESET */
                colArray->colArray[i].value.value.stringVal.len = 0;
                ((TkStr64*)ptr)->val[colArray->colArray[i].value.value.stringVal.len] = '\0';
            }
            else  if(colArray->colArray[i].value.value.stringVal.len > 64)
            {
                *column = colId;
                 SLM_LOG(SLM_ERR, "error in decode %d\n", colId);
                return (E_TB_COLTYPE);
            }
            else
            {
            ((TkStr64*)ptr)->pres = 1;
            strncpy(((TkStr64*)ptr)->val, 
                    colArray->colArray[i].value.value.stringVal.ptr,
                    colArray->colArray[i].value.value.stringVal.len);   
                ((TkStr64*)ptr)->val [colArray->colArray[i].value.value.stringVal.len] = 0;
            }
            break;
        case STR128:
#if 1 /* BUG 11499: vpochira: */
            if (colArray->colArray[i].value.value.stringVal.len == 0) {
               colArray->colArray[i].value.colType = TSSMI_RESET; 
	    }
#endif /* BUG: 11499: end-chng */
            if (colArray->colArray[i].value.colType == TSSMI_RESET)
            {
                if(flags == DEFAULT)
                {
                    ((TkStr128*)ptr)->pres = 0;
                }
                else if(flags == UPDATE_REQD)
                {
                    ((TkStr128*)ptr)->pres = DB_RESET; /* MGC change: set pres flag to DB_RESET for TSSMI_RESET */
                }
                /* Bug 15705 - support NULL value for TSSMI_RESET */
                colArray->colArray[i].value.value.stringVal.len = 0;
                ((TkStr128*)ptr)->val[colArray->colArray[i].value.value.stringVal.len] = '\0';
            }
            else  if(colArray->colArray[i].value.value.stringVal.len > 128)
            {
                *column = colId;
                 SLM_LOG(SLM_ERR, "error in decode %d\n", colId);
                return (E_TB_COLTYPE);
            }
            else
            {
                ((TkStr128*)ptr)->pres = 1;
                strncpy(((TkStr128*)ptr)->val, 
                        colArray->colArray[i].value.value.stringVal.ptr,
                        colArray->colArray[i].value.value.stringVal.len);   
                ((TkStr128*)ptr)->val [colArray->colArray[i].value.value.stringVal.len] = 0;
            }
            break;
        case STR256:
            if (colArray->colArray[i].value.colType == TSSMI_RESET)
            {
                if(flags == DEFAULT)
                {
                    ((TkStr256*)ptr)->pres = 0;
                }
                else if(flags == UPDATE_REQD)
                {
                    ((TkStr256*)ptr)->pres = DB_RESET; /* MGC change: set pres flag to DB_RESET for TSSMI_RESET */
                }
                /* Bug 15705 - support NULL value for TSSMI_RESET */
                colArray->colArray[i].value.value.stringVal.len = 0;
                ((TkStr256*)ptr)->val[colArray->colArray[i].value.value.stringVal.len] = '\0';
            }
/* len will never biger than 256 due to the type limit. 
            else  if(colArray->colArray[i].value.value.stringVal.len > 256)
            {
                *column = colId;
                 SLM_LOG(SLM_ERR, "error in decode %d\n", colId);
                return (E_TB_COLTYPE);
            }
*/
            else
            {
                ((TkStr256*)ptr)->pres = 1;
                strncpy(((TkStr256*)ptr)->val, 
                        colArray->colArray[i].value.value.stringVal.ptr,
                        colArray->colArray[i].value.value.stringVal.len);   
                ((TkStr256*)ptr)->val [colArray->colArray[i].value.value.stringVal.len] = 0;
            }
            break;
	    
        default:
            *column = colId; 
            SLM_LOG(SLM_ERR, "default error in decode %d\n", colId);
            return (E_TB_COLTYPE);
            break;
        }
    }
   
    return (0);
} /* slDecodeSmiCols() */



#define SL_SET_ROW_COL(tabDef, row, colId, type, rowKey, colArry)                              \
{                                                                                               \
    U8 *ptr = (U8*)(row)+(tabDef)->dbColDef[(colId)]->offset;                                   \
    if (((type *)ptr)->pres)                                                                    \
    {                                                                                           \
        if ((colId)<(tabDef)->numKeys)                                                          \
        {                                                                                       \
            (rowKey)->rowIndex[(rowKey)->numIndices].value.longVal = ((type *)ptr)->val;        \
            (rowKey)->rowIndex[(rowKey)->numIndices++].colType = TSSMI_LONG;                    \
        }                                                                                       \
        else                                                                                    \
        {                                                                                       \
            (colArry)->colArray[(colArry)->numCols].value.value.longVal = ((type *)ptr)->val;   \
            (colArry)->colArray[(colArry)->numCols].columnId = (colId);                         \
            (colArry)->colArray[(colArry)->numCols++].value.colType = TSSMI_LONG;               \
        }                                                                                       \
    }                                                                                           \
}


/**********************************************************************
** slRtrvMultiEncodeSmiCols
**
**     Desc:  Encode table structure rows into SMI columns 
**            for getnextmutli cases
**********************************************************************/
S16
slRtrvMultiEncodeSmiCols(SlTl1Cb *slTl1Cb, void *rows, U16 numRows, void **respPtr, 
                          U32 *respSize, U32 stringObjSize, U8 **stringObjects)
{
    TsSmiRowKey *rowKey;
    TsSmiColArray *colArray;
    TsSmiColObj *colObjArray;
    U8 j, *rowPtr;
    U16 tableId, i;
    DbTableDef *tabDef;
    S16 ret;
    int strSize;
    U8 *strCols;

    tableId = *(int *)(rows);
    if ((tableId >= TELICA_MAX_TAB_ID) || (tableId < TABLE_ID_OFFSET))
    {
        SLM_LOG(SLM_ERR, "invalid tableId:0x%x\n", tableId);
        return (E_TB_TABLEID);
    }
    tabDef = tableDefs[tableId-TABLE_ID_OFFSET];
    if (!tabDef)
    {
        SLM_LOG(SLM_ERR, "table not defined\n");
        return(E_TB_NULLPTR);
    }

    /* calculate size for string columns */
    strSize = 0;    
    for (i=0;i<numRows;i++)
    {
        rowPtr = (U8 *)rows + i*tabDef->rowSize;
        for (j=0;j<tabDef->numCols;j++)
        {
            if (!tabDef->dbColDef[j])
            {
                /* missing column definition, skip */
                continue;
            }
            if ((tabDef->dbColDef[j]->width>=STR8) && (tabDef->dbColDef[j]->width<MAX_WIDTH_ENUM))
            {
                /* string column, allocate enough memory to store this */
                U8 *ptr = (U8*)(rowPtr)+(tabDef)->dbColDef[(j)]->offset;   
                if (((TkStr8 *) ptr)->pres)
                {
                    /* make sure string is NULL terminated */
                    ((TkStr8 *)ptr)->val[tkStrLen[tabDef->dbColDef[j]->width]-1]='\0';
                    /* note, for 0 length strings, we may not pack the '\0' at all, 
                       so we'll waste 1 byte, but we don't care */
                    strSize += (1+strlen(&((TkStr8 *)ptr)->val[0]));
                }
            }
        } /* for all columns */
    } /* for all rows */

    *respSize = numRows*(sizeof(TsSmiRowKey*)+sizeof(TsSmiColArray*)+sizeof(TsSmiRowKey)+
                         sizeof(TsSmiColArray)+ tabDef->numCols*sizeof(TsSmiColObj)) + strSize + stringObjSize;
    
    ret = SGetSBuf(0, SLM_POOL, (Data**)respPtr, (Size)(*respSize));

#if 0
    SLM_LOG(SLM_NFY, "ASMASM:slRtrvMultiEncodeSmiCols: Obtained %d bytes\n", *respSize);
    SLM_LOG(SLM_NFY, "%d (%d + %d + %d + %d + %d*%d) + %d + %d\n", numRows, sizeof(TsSmiRowKey*),
            sizeof(TsSmiColArray*), sizeof(TsSmiRowKey), sizeof(TsSmiColArray), tabDef->numCols,
            sizeof(TsSmiColObj), strSize, stringObjSize);
#endif
    if (ret != ROK)
    {
        SLM_LOG(SLM_ERR, "SGetSBuf failed to alloc %ld bytes, ret:%d\n",*respSize, ret);
        *respPtr=NULL;
        *respSize = 0;
        return (RFAILED);
    }
    cmMemset((U8 *)*respPtr, 0,*respSize);

    slTl1Cb->respRowKey = (TsSmiRowKey**)*respPtr;
    slTl1Cb->respColArray = (TsSmiColArray**)((U8*)*respPtr+numRows*sizeof(TsSmiRowKey*));
    rowKey = (TsSmiRowKey*)((U8*)(slTl1Cb->respColArray)+numRows*sizeof(TsSmiColArray*));
    colArray = (TsSmiColArray*)((U8*)rowKey+numRows*sizeof(TsSmiRowKey));
    colObjArray = (TsSmiColObj*)((U8 *)colArray+numRows*sizeof(TsSmiColArray));
    strCols = (U8 *)((U8 *)colObjArray + numRows*(tabDef->numCols*sizeof(TsSmiColObj)));
    if (stringObjSize && stringObjects)
    {
        *stringObjects = strCols + strSize;
    }

    for (i=0;i<numRows;i++)
    {
        slTl1Cb->respRowKey[i]= &rowKey[i];
        slTl1Cb->respColArray[i] = &colArray[i];
        colArray[i].colArray = (TsSmiColObj *)((U8*)colObjArray
                                                +i*tabDef->numCols*sizeof(TsSmiColObj));

#ifdef ATCA
        if (isBulkPmTL1Trans(slTl1Cb->transId) == ROK)
        {
            MtpPmSlkHwm  *mtpPmSlkHwm  = NULL;
            PmStgSlkL2   *pmStgSlkL2   = NULL;
            StgSlkAal5Pm *stgSlkAal5Pm = NULL;
            Dmtp3PmSlkl3 *dmtp3PmSlkl3 = NULL;

            switch(tableId)
            {
               case MTPSLKHWM_PM_TAB_ID:
                  mtpPmSlkHwm = (MtpPmSlkHwm *)rows;
                  rowKey[i].rowIndex[0].value.longVal = mtpPmSlkHwm[i].lnkId.val;
                  rowKey[i].rowIndex[0].colType = TSSMI_LONG;
                  rowKey[i].numIndices = 1;
                  SLM_LOG(SLM_DBG, "PM-SLK lnkId[%d]:%d\n", i, rowKey[i].rowIndex[0].value.longVal);
                  break; 
               case STGSLKL2_PM_TAB_ID:
                  pmStgSlkL2 = (PmStgSlkL2 *)rows;
                  rowKey[i].rowIndex[0].value.longVal = pmStgSlkL2[i].ss7LnkId.val;
                  rowKey[i].rowIndex[0].colType = TSSMI_LONG;
                  rowKey[i].numIndices = 1;
                  SLM_LOG(SLM_DBG, "PM-STGSLKL2 lnkId[%d]:%d\n", i, rowKey[i].rowIndex[0].value.longVal);                  
                  break;
               case STGSLKAAL5_PM_TAB_ID:
                  stgSlkAal5Pm =(StgSlkAal5Pm *)rows;
                  rowKey[i].rowIndex[0].value.longVal = stgSlkAal5Pm[i].ss7LnkId.val;
                  rowKey[i].rowIndex[0].colType = TSSMI_LONG;
                  rowKey[i].numIndices = 1;
                  SLM_LOG(SLM_DBG, "PM-STGSLKAAL5 slkId[%d]:%d\n", i, rowKey[i].rowIndex[0].value.longVal);
                  break;
               case STG_TABLE_SLKL3_PM:      
                  dmtp3PmSlkl3 = (Dmtp3PmSlkl3 *)rows;
                  rowKey[i].rowIndex[0].value.longVal = dmtp3PmSlkl3[i].lnkId.val;
                  rowKey[i].rowIndex[0].colType = TSSMI_LONG;
                  rowKey[i].numIndices = 1;
                  SLM_LOG(SLM_DBG, "PM-STGSLKL3 lnkId[%d]:%d\n", i, rowKey[i].rowIndex[0].value.longVal);
                  break;
               default:
                  return(RFAILED);
             }
        }
#endif
    
        for (j=0;j<tabDef->numCols;j++)
        {
            rowPtr = (U8 *)rows + i*tabDef->rowSize;
            if (!tabDef->dbColDef[j])
            {
                /* missing column definition, skip this column */
                continue;
            }
            switch(tabDef->dbColDef[j]->width)
            {
            case S8BITS:
                SL_SET_ROW_COL(tabDef, rowPtr, j, TkS8, &rowKey[i], &colArray[i]);
                break;
            case U8BITS:
                SL_SET_ROW_COL(tabDef, rowPtr, j, TkU8, &rowKey[i], &colArray[i]);
                break;
            case S16BITS:
                SL_SET_ROW_COL(tabDef, rowPtr, j, TkS16, &rowKey[i], &colArray[i]);
                break;
            case U16BITS:
                SL_SET_ROW_COL(tabDef, rowPtr, j, TkU16, &rowKey[i], &colArray[i]);
                break;
            case S32BITS:
                SL_SET_ROW_COL(tabDef, rowPtr, j, TkS32, &rowKey[i], &colArray[i]);
                break;
            case U32BITS:
                SL_SET_ROW_COL(tabDef, rowPtr, j, TkU32, &rowKey[i], &colArray[i]);
                break;
            case STR8:
            case STR16:
            case STR32:
            case STR64:
            case STR128:
            case STR256:
            {
                U8 *ptr = (U8*)(rowPtr)+(tabDef)->dbColDef[j]->offset;   
                if (((TkStr8 *) ptr)->pres)
                {
                    if (!strlen(&((TkStr8 *)ptr)->val[0]))
                    {
                        /* shouldn't happen unless the DB returned pres for a NULL string,
                           skip NULL string column */
                        continue;
                    }
                    
                    if (j<(tabDef)->numKeys)
                    {                                         
                        rowKey[i].rowIndex[rowKey[i].numIndices].value.stringVal.ptr 
                            = strCols;
                        /* do unprotected copy because we already made sure the column is NULL terminated */
                        strcpy(strCols, &((TkStr8 *)ptr)->val[0]);
                        strCols += (1+strlen (&((TkStr8 *)ptr)->val[0]));
                        rowKey[i].rowIndex[rowKey[i].numIndices].value.stringVal.len
                            = strlen (&((TkStr8 *)ptr)->val[0]);
                        rowKey[i].rowIndex[rowKey[i].numIndices++].colType = TSSMI_OCTET_STRING;
                    }
                    else                     
                    {                            
                        colArray[i].colArray[colArray[i].numCols].value.value.stringVal.ptr 
                            = strCols;
                        /* do unprotected copy because we already made sure the column is NULL terminated */
                        strcpy(strCols, &((TkStr8 *)ptr)->val[0]);
                        strCols += (1+strlen (&((TkStr8 *)ptr)->val[0]));
                        colArray[i].colArray[colArray[i].numCols].value.value.stringVal.len
                            = strlen (&((TkStr8 *)ptr)->val[0]);
                        colArray[i].colArray[colArray[i].numCols].columnId = j;
                        colArray[i].colArray[colArray[i].numCols++].value.colType = TSSMI_OCTET_STRING; 
                    }
                }
            }
            break;
            default:
                {
                    U8 *ptr = (U8*)(rowPtr)+(tabDef)->dbColDef[(j)]->offset;   
                    if (*ptr)
                    {
                        SLM_LOG(SLM_ERR, "unknown type:%d, strings not handled here yet\n", tabDef->dbColDef[j]->width);
                    }
                }
                break;
            } /* switch */
        } /* for all cols */
    } /* for all records */

    slTl1Cb->multiStruct.nmbRecords = numRows;
    slTl1Cb->totalRecords = numRows;
    slTl1Cb->totalRecSize = *respSize;

    return (ROK);

} /* slRtrvMultiEncodeSmiCols() */



/**********************************************************************
** slRuleIpfltrRuleGenEncodeSmiCols
**
**     Desc:  Encode table structure rows into SMI columns 
**            must use slAllocCopySmiTrans() style allocation, since
**            the result is going into rowKey/colArray instead of
**            respRowKey/respColArray
**********************************************************************/
S16
slRuleIpfltrRuleGenEncodeSmiCols(SlTl1Cb *slTl1Cb, void *row)
{
    TsSmiRowKey *rowKey = NULL;
    TsSmiColArray *colArray = NULL;
    U8 j, *rowPtr = (U8*)row;
    U16 tableId, rowIndexCount = 0, colArrayCount = 0;
    DbTableDef *tabDef;
    S16 ret;


    tableId = *(int *)(row);
    if ((tableId >= TELICA_MAX_TAB_ID) || (tableId < TABLE_ID_OFFSET))
    {
        SLM_LOG(SLM_ERR, "invalid tableId:0x%x\n", tableId);
        return (E_TB_TABLEID);
    }
    tabDef = tableDefs[tableId-TABLE_ID_OFFSET];
    if (!tabDef)
    {
        SLM_LOG(SLM_ERR, "table not defined\n");
        return(E_TB_NULLPTR);
    }

    /* get memory set up the same way slAllocCopySmiTrans does it */

    ret = SGetSBuf(slCb.region, slCb.pool, (Data **)&rowKey,
                   sizeof(TsSmiRowKey));
    if (ret != ROK)
    {
        SLM_LOG(SLM_ERR, "SGetSBuf failed\n");
        goto ruleIpfltrRuleGen_exit;
    }

    ret = SGetSBuf(slCb.region, slCb.pool, (Data **)&colArray,
                   sizeof(TsSmiColArray));
    if (ret != ROK)
    {
        SLM_LOG(SLM_ERR, "SGetSBuf failed\n");
        goto ruleIpfltrRuleGen_exit;
    }

    cmMemset((U8*) rowKey, 0, sizeof(TsSmiRowKey));
    cmMemset((U8*) colArray, 0, sizeof(TsSmiColArray));
    for (j = 0, rowIndexCount = 0; j < tabDef->numKeys; j++)
    {
        U8 *fldptr;
        if (tabDef->dbColDef[j] == NULL)
        {
            /* missing column, skip it */
            continue;
        }

        if (tabDef->dbColDef[j]->width >= MAX_WIDTH_ENUM)
        {
            SLM_LOG(SLM_ERR, "%s: bad width enum %d, col %d\n", __FUNCTION__,
                    tabDef->dbColDef[j]->width, j);
            ret = RFAILED;
            goto ruleIpfltrRuleGen_exit;
        }

        fldptr = (U8*)row + tabDef->dbColDef[j]->offset;

#define COUNT_COL(typ, dataptr, counter)   \
    {  typ *tmpdataptr = (typ *) dataptr;  \
        if (tmpdataptr->pres) (counter)++; }

        switch (tabDef->dbColDef[j]->width)
        {
        case S8BITS:
            COUNT_COL(TkS8, fldptr, rowIndexCount);
            break;
        case U8BITS:
            COUNT_COL(TkU8, fldptr, rowIndexCount);
            break;
        case S16BITS:
            COUNT_COL(TkS16, fldptr, rowIndexCount);
            break;
        case U16BITS:
            COUNT_COL(TkU16, fldptr, rowIndexCount);
            break;
        case S32BITS:
            COUNT_COL(TkS32, fldptr, rowIndexCount);
            break;
        case U32BITS:
            COUNT_COL(TkU32, fldptr, rowIndexCount);
            break;
        case STR16:
        {
            TkStr16 *tstr = (TkStr16 *) fldptr;
            if (tstr->pres)
            {
                int len = strlen(tstr->val) + 1;
                rowIndexCount++;

                ret = SGetSBuf(slCb.region, slCb.pool,
                               (Data **)&(rowKey->rowIndex[j].value.stringVal.ptr),
                               len);
                if (ret != ROK)
                {
                    SLM_LOG(SLM_ERR, "%s: SGetSBuf failed\n", __FUNCTION__);
                    /* haven't alloc'd anything extra yet, just go to exit */
                    goto ruleIpfltrRuleGen_exit;
                }
                rowKey->rowIndex[j].value.stringVal.len = len;
                rowKey->rowIndex[j].colType = TSSMI_OCTET_STRING;
            }
            break;
        }
        default:
        { U8 *prsptr = (U8 *)fldptr;
            SLM_LOG(SLM_ERR,"%s: unrecognized width for row %d, %d - prs %d\n",
                    __FUNCTION__, j, tabDef->dbColDef[j]->width,*prsptr);
        }
            break;
        }
    }

    /* count columns we'll need, to get colArray started */
    for (j = tabDef->numKeys; j < tabDef->numCols; j++)
    {
        U8 *fldptr;
        if (tabDef->dbColDef[j] == NULL)
        {
            /* missing column, skip it */
            continue;
        }

        if (tabDef->dbColDef[j]->width >= MAX_WIDTH_ENUM)
        {
            SLM_LOG(SLM_ERR, "%s: bad width enum %d, col %d\n", __FUNCTION__,
                    tabDef->dbColDef[j]->width, j);
            ret = RFAILED;
            goto ruleIpfltrRuleGen_exit;
        }

        fldptr = (U8*)row + tabDef->dbColDef[j]->offset;
        switch (tabDef->dbColDef[j]->width)
        {
        case S8BITS:
            COUNT_COL(TkS8, fldptr, colArrayCount);
            break;
        case U8BITS:
            COUNT_COL(TkU8, fldptr, colArrayCount);
            break;
        case S16BITS:
            COUNT_COL(TkS16, fldptr, colArrayCount);
            break;
        case U16BITS:
            COUNT_COL(TkU16, fldptr, colArrayCount);
            break;
        case S32BITS:
            COUNT_COL(TkS32, fldptr, colArrayCount);
            break;
        case U32BITS:
            COUNT_COL(TkU32, fldptr, colArrayCount);
            break;
        case STR8:
            COUNT_COL(TkStr8, fldptr, colArrayCount);
            break;
        case STR16:
            COUNT_COL(TkStr16, fldptr, colArrayCount);
            break;
        case STR64:
            COUNT_COL(TkStr64, fldptr, colArrayCount);
            break;  /* IPv6 IP format */ 
            /* no other strings */
        default:
            SLM_LOG(SLM_ERR, "%s: RuleIpFltr: unrecog col size %d, col %d\n",
                    __FUNCTION__, tabDef->dbColDef[j]->width, j);
            break;
        }
    }

    if (colArrayCount > 0)
    {
        U16 colIdx = 0;
        ret = SGetSBuf(slCb.region, slCb.pool, (Data **)&(colArray->colArray),
                       colArrayCount * sizeof(TsSmiColObj));

        if (ret != ROK)
        {
            SLM_LOG(SLM_ERR, "%s: SGetSBuf failed\n", __FUNCTION__);
            goto ruleIpfltrRuleGen_exit;
        }

        cmMemset((U8*) colArray->colArray, 0,
                 colArrayCount * sizeof(TsSmiColObj));
        for (j = tabDef->numKeys; j < tabDef->numCols; j++)
        {
            U8 *fldptr;
            if (tabDef->dbColDef[j] == NULL)
            {
                /* missing column, skip it */
                continue;
            }

            fldptr = (U8*)row + tabDef->dbColDef[j]->offset;
            switch (tabDef->dbColDef[j]->width)
            {
            case S8BITS:
                COUNT_COL(TkS8, fldptr, colIdx);
                break;
            case U8BITS:
                COUNT_COL(TkU8, fldptr, colIdx);
                break;
            case S16BITS:
                COUNT_COL(TkS16, fldptr, colIdx);
                break;
            case U16BITS:
                COUNT_COL(TkU16, fldptr, colIdx);
                break;
            case S32BITS:
                COUNT_COL(TkS32, fldptr, colIdx);
                break;
            case U32BITS:
                COUNT_COL(TkU32, fldptr, colIdx);
                break;
            case STR8:
            case STR16:
            case STR32:
            case STR64:
            case STR128:
            case STR256:
            {
                TkStr8 *tstr = (TkStr8 *) fldptr;
                if (tstr->pres)
                {
                    int len = strlen(tstr->val) + 1;
                    
                    ret = SGetSBuf(slCb.region, slCb.pool,
                                   (Data **)&(colArray->colArray[colIdx].value.value.stringVal.ptr),
                                   len);
                    if (ret != ROK)
                    {
                        SLM_LOG(SLM_ERR, "%s: SGetSBuf failed\n", __FUNCTION__);
                        goto ruleIpfltrRuleGen_exit;
                    }
                    colArray->colArray[colIdx].columnId = j;
                    colArray->colArray[colIdx].value.value.stringVal.len = len;
                    colArray->colArray[colIdx].value.colType = TSSMI_OCTET_STRING;
                    colIdx++;
                }
            }
            break;
            default:
                SLM_LOG(SLM_ERR, "%s: new str enum in RuleIpfltr %d, col %d\n",
                        __FUNCTION__, tabDef->dbColDef[j]->width, j);
                ret = RFAILED;
                goto ruleIpfltrRuleGen_exit;
                break;
            }
        }

        if (colIdx != colArrayCount)
        {
            SLM_LOG(SLM_ERR, "%s: counts don't match! colIdx %d, "
                    "colArrayCount %d\n", __FUNCTION__, colIdx, colArrayCount);
            ret = RFAILED;
            goto ruleIpfltrRuleGen_exit;
        }
    }

    for (j=0;j<tabDef->numCols;j++)
    {
        if (!tabDef->dbColDef[j])
        {
            /* missing column definition, skip this column */
            continue;
        }

        switch(tabDef->dbColDef[j]->width)
        {
        case S8BITS:
            SL_SET_ROW_COL(tabDef, rowPtr, j, TkS8, rowKey, colArray);
            break;
        case U8BITS:
            SL_SET_ROW_COL(tabDef, rowPtr, j, TkU8, rowKey, colArray);
            break;
        case S16BITS:
            SL_SET_ROW_COL(tabDef, rowPtr, j, TkS16, rowKey, colArray);
            break;
        case U16BITS:
            SL_SET_ROW_COL(tabDef, rowPtr, j, TkU16, rowKey, colArray);
            break;
        case S32BITS:
            SL_SET_ROW_COL(tabDef, rowPtr, j, TkS32, rowKey, colArray);
            break;
        case U32BITS:
            SL_SET_ROW_COL(tabDef, rowPtr, j, TkU32, rowKey, colArray);
            break;
        case STR8:
        case STR16:
        case STR32:
        case STR64:
        case STR128:
        case STR256:
        {
            U8 *ptr = (U8*)(rowPtr)+(tabDef)->dbColDef[j]->offset;
            if (((TkStr8 *) ptr)->pres)
            {
                u8 isrow = (j < (tabDef)->numKeys);
                u8 *idxCnt = isrow ?
                        &rowKey->numIndices : &colArray->numCols;
                TsSmiOctetString *strval = isrow ?
                        &rowKey->rowIndex[*idxCnt].value.stringVal :
                        &colArray->colArray[*idxCnt].value.value.stringVal;

                if (!strlen(&((TkStr8 *)ptr)->val[0]))
                {
                    /* shouldn't happen unless the DB returned pres for a
                     * NULL string, skip NULL string column */
                    continue;
                }

                if (NULL == strval->ptr)
                {
                    SLM_LOG(SLM_ERR, "%s: allocated %s str memory "
                            "missing! col %d rowIdx %d\n", __FUNCTION__,
                            isrow ? "rowKey" : "colArray", j, *idxCnt);
                    ret = RFAILED;
                    goto ruleIpfltrRuleGen_exit;
                }

                if (!isrow && colArray->colArray[*idxCnt].columnId != j)
                {
                    SLM_LOG(SLM_ERR, "%s: str col not matching! col %d, "
                            "saved col %d\n", __FUNCTION__, j,
                            colArray->colArray[*idxCnt].columnId);
                    ABORT_DEBUG;
                }


                /* do unprotected copy because we already made sure the
                 * column is NULL terminated */
                strcpy(strval->ptr, &((TkStr8 *)ptr)->val[0]);

                /* in case it wasn't == but not running with ABORT_DEBUG */
                if (!isrow)
                {
                    colArray->colArray[*idxCnt].columnId = j;
                }

                (*idxCnt)++;
            }
        }
        break;
        default:
        {
            U8 *ptr = (U8*)(rowPtr)+(tabDef)->dbColDef[(j)]->offset;   
            if (*ptr)
            {
                SLM_LOG(SLM_ERR, "unknown type:%d\n", tabDef->dbColDef[j]->width);
            }
        }
        break;
        } /* switch */
    } /* for all cols */

    slTl1Cb->rowKey = rowKey;
    slTl1Cb->colArray = colArray;
    rowKey = NULL;
    colArray = NULL;

 ruleIpfltrRuleGen_exit:
    if (rowKey != NULL)
    {
        for (j = 0; j < rowIndexCount; j++)
        {
            if (rowKey->rowIndex[j].colType == TSSMI_OCTET_STRING &&
                rowKey->rowIndex[j].value.stringVal.len != 0)
            {
                SPutSBuf(slCb.region, slCb.pool,
                         rowKey->rowIndex[j].value.stringVal.ptr,
                         rowKey->rowIndex[j].value.stringVal.len);
            }

        }
        SPutSBuf(slCb.region, slCb.pool, rowKey, sizeof(TsSmiRowKey));
    }

    if (colArray != NULL)
    {
        if (colArray->colArray != NULL)
        {
            for (j = 0; j < colArrayCount; j++)
            {
                if (colArray->colArray[j].value.colType == TSSMI_OCTET_STRING &&
                    colArray->colArray[j].value.value.stringVal.len != 0)
                {
                    SPutSBuf(slCb.region, slCb.pool,
                             colArray->colArray[j].value.value.stringVal.ptr,
                             colArray->colArray[j].value.value.stringVal.len);
                }
            }
        }

        if (colArrayCount > 0)
        {
            SPutSBuf(slCb.region, slCb.pool, colArray->colArray,
                     colArrayCount * sizeof(TsSmiColObj));
        }

        SPutSBuf(slCb.region, slCb.pool, colArray,
                 sizeof(TsSmiColArray));
    }

    return ret;

} /* slRuleIpfltrRuleGenEncodeSmiCols() */



/**********************************************************************
** slDumpSmiCols
**
**     Desc:  pretty prints a SMI row into a string
**
**     Notes: 1. undecodedCols can be NULL
**            2. input numCols in undecodedCols is the maximum number of 
**               columns that undecodedCols can hold
**            3. numCols in undecodedCols is set to the number of actual
**               undecoded columns
**     Ret:    0  - OK
**            >0 - see tb.h for a list of error codes
**
**********************************************************************/
S16
slDumpSmiCols(U16 tableId, TsSmiRowKey *rowKey, TsSmiColArray *colArray, U8 *str)
{
    S16 colId = 0; /* PLM_IN */
    S16 i;
    DbTableDef *tabDef = NULLP;
    char colName[20], valStr[65], colStr[100];
    
    str[0]='\0';

    if (!(rowKey && colArray))
    {
        return (E_TB_NULLPTR);
    }
    
    if (tableId >= TELICA_MAX_TAB_ID)
    {
        SLM_LOG(SLM_ERR, "invalid tableId:0x%x\n", tableId);
        return (E_TB_TABLEID);
    }
    
    tabDef = tableDefs[tableId-TABLE_ID_OFFSET];
    if (!tabDef)
    {
        SLM_LOG(SLM_ERR, "table not defined\n");
        return(E_TB_NULLPTR);
    }

    sprintf(str, "%d:", tableId);

    for (i=0; i<rowKey->numIndices; i++)
    {
        /* modified for PLM_IN, assume colId always take first bunch of index */
        colId =  i;
        sprintf(colName, "%s", tabDef->dbColDef[colId]->name);
        if (tabDef->dbColDef[colId]->width >= STR8)
        {
            strncpy(valStr, rowKey->rowIndex[i].value.stringVal.ptr, rowKey->rowIndex[i].value.stringVal.len);
            valStr[rowKey->rowIndex[i].value.stringVal.len]='\0';
        }
        else
        {
            sprintf(valStr, "%d", (int)rowKey->rowIndex[i].value.longVal);
        }
        if (i)
        {
            sprintf(colStr, ",%s:%s", colName, valStr);
        }
        else
        {
            sprintf(colStr, "%s:%s", colName, valStr);
        }
        
        strcat(str, colStr);
    }
    
    for (i=0; i<colArray->numCols; i++)
    {
        colId = colArray->colArray[i].columnId;
        if ((colId < tabDef->numCols) && tabDef->dbColDef[colId])
        {
            sprintf(colName, "%s", tabDef->dbColDef[colId]->name);
        }
        else
        {
            sprintf(colName, "(%d)", colId);
        }
        
        if (tabDef->dbColDef[colId]->width >= STR8)
        {
            strncpy(valStr, colArray->colArray[i].value.value.stringVal.ptr, colArray->colArray[i].value.value.stringVal.len);
            valStr[colArray->colArray[i].value.value.stringVal.len]='\0';
        }
        else
        {
            sprintf(valStr, "%d", (int)colArray->colArray[i].value.value.longVal);
        }
        sprintf(colStr, ",%s:%s", colName, valStr);
        strcat(str, colStr);
    }
    
    return (0);
} /* slDumpSmiCols() */



/**********************************************************************
** slCpySmiCol
**
**     Desc:  copy an SMI column
**
**     Notes: 1. This function needs to allocate memory for string copies
**
**     Ret: 
**
**********************************************************************/
S16 slCpySmiCol(TsSmiColObj *dstCol, TsSmiColObj *srcCol)
{
    cmMemcpy((U8 *)dstCol, (U8 *)srcCol, sizeof(TsSmiColObj));
    return (ROK);
}



/**********************************************************************
** slRemapSmiCols
**
**     Desc:  remaps colId to another set of colIds
**
**     Notes: 
**     Ret:    0  - OK
**            >0 - see tb.h for a list of error codes
**
**********************************************************************/
S16
slRemapSmiCols(TsSmiColArray *colArray, SlMapSmiColTbl *mapTbl, S16 max, U16 invalid, U8 reverse)
{
    S16 colId;
    S16 i, j, cnt=0;
    U8 src, dst;

    if (reverse==FALSE)
    {
        src=0;
        dst=1;
    }
    else
    {
        src=1;
        dst=0;
    }

    if (!colArray)
    {
        return (E_TB_NULLPTR);
    }
    
    for (i=0; i<colArray->numCols; i++)
    {
        colId = colArray->colArray[i].columnId;
        
        for (j=0;j<max;j++)
        {
            if (mapTbl[j].list[src]==colId)
            {
                if (mapTbl[j].list[dst]==invalid)
                {
                    /* no mapping exists, delete this column TBD */
                    break;
                }
                colArray->colArray[i].columnId = mapTbl[j].list[dst];
                cnt++;
                break;
            }
        }
    }

    SLM_LOG(SLM_NFY, "re-mapped %d columns\n", cnt);
    
    return (0);
} /* slRemapSmiCols() */

/**********************************************************************
** slSendTl1Resp
**
**     Desc:   send TL1 response
**
**     Notes:  modules call this function directly to send error responses
**             LM calls this function to send OK responses after all modules 
**             have confirmed.
**             **This function should check if a response has already been 
**             sent for a particular LM TL1 event.
**
**     Ret:    ROK     - successful
**             RFAILED - failed
**    
**********************************************************************/
S16
slSendTl1Resp(SlTl1Cb *slTl1Cb, 
               TsSmiErrorCode errorCode,
               TsSmiErrorIndex errorIndex, 
               S16 tl1Error)
{
#if 0                           /* bug 89870 */    
    S16 ret;
    TsTransId  transId;
    SlTl1TransIdCb *retTransIdCb;
#endif
    
    if (!slTl1Cb)
    {
        SLM_LOG(SLM_DBG, "SendTl1Resp: Failed to send TL1 response because no SlTl1Cb\n");
        return (RFAILED);
    }

    if ((slTl1Cb->lmTl1Evt!=LM_TL1_ENT_EVT) && (slTl1Cb->lmTl1Evt != LM_BOOT_EVT) &&
        (slTl1Cb->lmTl1Evt!=LM_TL1_ED_EVT) && (slTl1Cb->lmTl1Evt!=LM_TL1_DLT_EVT) &&
        (slTl1Cb->lmTl1Evt!=LM_TL1_RTRV_EVT))
    {
            
        SLM_LOG(LM_ERR, "ignoring TL1 resp for lm event:%d\n",slTl1Cb->lmTl1Evt);
        return (ROK);
    }

    if (!slTl1TrueTl1Trans(slTl1Cb))
    {
        SLM_LOG(LM_NFY, "skipping TL1 resp: IPFILTER ntfy transaction\n");
 
        /* current EMF cmds are added to this category  */
        if(slTl1Cb->nonTl1)
        {
            sendRespToEmf(slTl1Cb, errorCode, errorIndex, tl1Error);
        }
     
    }
#ifdef ATCA
    else if (isBulkPmTL1Trans(slTl1Cb->transId)== ROK) /* this tl1 request is sent by bulk pm  */
    {
        bkpmGetResp(slTl1Cb, errorCode);
    }
#endif
    else if(isIomAuTL1Trans(slTl1Cb))
    {
        /* let's set the tl1result */
        if((TSSMI_NO_ERROR == errorCode)&&(TSSMI_NO_ERROR == tl1Error))
        {
            setAuTl1Result(ROK);
        }
        else
        {
            setAuTl1Result(RFAILED);
        }
    }
    else
    {
        SLM_LOG(LM_DBG, "SendTl1Resp: Returning  TransId 0x%x:  ErrorCode = %d\n     ErrorIndex = %d:  Tl1Error = %d\n", slTl1Cb->transId, errorCode, errorIndex, tl1Error);

        if (isTrcTL1Trans(slTl1Cb->transId) == ROK) /* this tl1 request is sent by call trace */
        {
            trcGetState(slTl1Cb, errorCode);            
        }

        if (!slTl1Cb->blkFlg)
        {            
            emfPkSmiSetResp(&slTl1Cb->tl1Cmd,
                            slTl1Cb->transId, 
                            slTl1Cb->tableId, 
                            slTl1Cb->rowKey, 
                            slTl1Cb->colArray, 
                            errorCode, errorIndex, tl1Error);
        }
        else
        {
            slBlkSendTl1Resp(slTl1Cb, tl1Error);
        }
    }

#if 0                           /* bug 89870 */    
    transId = slTl1Cb->transId;
    ret = cmHashListFind(&slCb.tl1TransIdCp,
                         (U8 *)&transId,
                         (U16)sizeof(TsTransId), 0,
                         (PTR *)&retTransIdCb);
    if (ret != ROK)
    {
        SLM_LOG(LM_DBG, "SendTl1Resp: HashListFind returned 0x%x\n", ret);
        return(RFAILED);
    }

    ret = cmHashListDelete(&slCb.tl1TransIdCp, (PTR)retTransIdCb);
    if (ret != ROK)
    {
        SLM_LOG(LM_ERR, "cmHashListDelete failed, ret = 0x%x\n", ret);
        return(RFAILED);
    }

    SPutSBuf(LM_REGION, LM_POOL,
             retTransIdCb, sizeof(SlTl1TransIdCb));
#endif    
    
    return (ROK);
}

S16 sendRespToEmf(SlTl1Cb *slTl1Cb, TsSmiErrorCode errorCode,
               TsSmiErrorIndex errorIndex, S16 tl1Error)
{
    SFED_CMD_BUF_t sfedAck;

    memset(&sfedAck, 0, sizeof(SFED_CMD_BUF_t));
    SLM_LOG(LM_DBG, "%s: lmTl1Evt=%d, tl1Error=0x%x, errorCode=0x%x, SeqNo=0x%x\n",
            __FUNCTION__,slTl1Cb->lmTl1Evt,tl1Error,errorCode, slTl1Cb->transId);

    switch(slTl1Cb->lmTl1Evt)
    {
    case LM_TL1_ENT_EVT:
        sfedAck.Command = SFED_CREATED;
        break;

    case LM_TL1_ED_EVT:
        sfedAck.Command = SFED_EDITED;
        break;

    case LM_TL1_DLT_EVT:
        sfedAck.Command = SFED_DELETE_QUERY;
        sfedAck.CmdInfo.SfedDeleteQuery.OkToDelete = tl1Error;
        break;

    default:
        SLM_LOG(SLM_NFY, "unknown SFED command (%d) received\n", slTl1Cb->lmTl1Evt);
        break;
    }

    sfedAck.SeqNo = slTl1Cb->transId;
    sfedAck.Status = tl1Error;

    slSlToGaPstTsk(NULL_EVENT_INT, SFED_ACK_CLASS_INT, (void *)&sfedAck, sizeof(SFED_CMD_BUF_t));
    return(ROK);
}

S16 sendRespToEmfNoTl1Cb(SFED_CMD_BUF_t *sfedCmdRcv, S16 tl1Error)
{
    SlTl1Cb  ErrorSlTl1Cb;

    if (!sfedCmdRcv)
    {
       SLM_LOG(SLM_NFY, "%s: Null Pointer\n", __FUNCTION__);
       return(ROK);
    }

    memset(&ErrorSlTl1Cb, 0, sizeof(SlTl1Cb));
    SLM_LOG(SLM_NFY, "%s: Error processing SFED Command:%d\n", __FUNCTION__,sfedCmdRcv->Command);

    switch(sfedCmdRcv->Command)
    {
    case SFED_CREATED:
        ErrorSlTl1Cb.lmTl1Evt = LM_TL1_ENT_EVT;
        break;

    case SFED_EDITED:
        ErrorSlTl1Cb.lmTl1Evt = LM_TL1_ED_EVT;
        break;

    case SFED_DELETE_QUERY:
        ErrorSlTl1Cb.lmTl1Evt = LM_TL1_DLT_EVT;
        break;

    default:
        SLM_LOG(SLM_NFY, "unknown SFED command (%d) received\n", sfedCmdRcv->Command);
        return(ROK);
        break;
    }
    ErrorSlTl1Cb.transId = sfedCmdRcv->SeqNo;

    sendRespToEmf(&ErrorSlTl1Cb,0,0,tl1Error);
    
    return(ROK);
}
