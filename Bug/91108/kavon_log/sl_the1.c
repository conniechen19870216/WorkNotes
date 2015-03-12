/**********************************************************************
**  Copyright (c) 2009 Alcatel-Lucent.  All rights reserved.
**
**  Name:  sl_the1.c
**
**  Desc:  This file contains SLAM TH E1 functions
**  
**  Modification History
**  Date         Prg   Description
**  ------------------------------------------------
**  4/19/2009   mcao001 Created
***********************************************************************/

/* header include files -- defines (.h) */

/* LynxOs */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <ctype.h>

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
#include "TsmColDefs.h"
#include "tb.h"
#include "tb_mgc.h"
#include "tb_util.h"
#include "tb_the1.h"

/* TL1/SM row/column includes */
#include "sm_cmds.h"

/* SLAM includes */
#include "lm.h"
#include "sl.h"
#include "sl_tl1.h"
#include "sl_unit.h"
#include "sl_smi.h"
#include "sl_brd.h"
#include "lry.h"
#include "lry.x"
#include "sl_clam.h"
#include "sl_ipfwd.h"

#include "log_global.h"
#include "proc_info.h"

#include "cm_tb.h"
#include "ldb.x" 
#include "ts.h"
#include "telica_pid.h"
#include "git_class.h"

static S16 slValidateThE1(ThE1 *the1);

S16 slTl1_Ed_ThE1_fsm(SlTl1Cb *context);
static S16 slTl1_Ed_ThE1_fsm_start(SlTl1Cb *context);
static S16 slTl1_Ed_ThE1_fsm_dbget(SlTl1Cb *context);
static S16 slTl1_Ed_ThE1_fsm_dbcfm(SlTl1Cb *context);

S16 slTl1_Rtrv_ThE1_fsm(SlTl1Cb *context);
static S16 slTl1_Rtrv_ThE1_fsm_start(SlTl1Cb *context);
static S16 slTl1_Rtrv_ThE1_fsm_dbget(SlTl1Cb *context);

S16 slThE1_bootKey_mod(SlClamCb *context, DbMngmtReq *dbMngmtReq);

enum 
{
    THE1_FSM_START,
    THE1_AWT_DB_GET,
    THE1_AWT_DB_SET,
    THE1_AWT_CLAM,
};

typedef struct thE1FsmCb 
{
    ThE1         *thE1;            /* incoming */
    ThE1         *dbThE1;          /* DB row */
    U8            numThE1;
} ThE1FsmCb;

/*****************************************************************************
 *
 *  SET-TH-E1
 *
 *****************************************************************************/
S16 slTl1_Ed_ThE1_fsm(SlTl1Cb *context)
{
    S16  ret = RFAILED;

    SLM_LOG(SLM_DBG, "context->userState:%ld\n", context->userState);
    
    switch (context->userState)
    {
    case THE1_FSM_START:
        ret = slTl1_Ed_ThE1_fsm_start(context);
        break;
    case THE1_AWT_DB_GET:
        ret = slTl1_Ed_ThE1_fsm_dbget(context);
        break;
    case THE1_AWT_DB_SET:
        ret = slTl1_Ed_ThE1_fsm_dbcfm(context);
        break;
    default:
        SLM_LOG(SLM_ERR, "unsupported userState:%ld\n", context->userState);
        break;
    }

    return (ret);
} /* slTl1_Ed_ThE1_fsm */

static S16 slTl1_Ed_ThE1_fsm_start(SlTl1Cb *context)
{
    S16           ret = RFAILED;
    ThE1FsmCb     *cb = NULL;
    U32           numRows = 0;
    DbMngmtReq    dbMngmtReq = {{0}};
    Pst           pst;
    U32           transId;
    U8            shelfId = 0;
    U16           pSlotId = 0;
	U16           errorCol1;
    U8            state;

    SLM_LOG(SLM_DBG, "context->userState:%ld\n", context->userState);
    
    /* allocate memory for userVar */ 
    ret = SGetSBuf(0, 0, (Data**)&context->userVar, sizeof(ThE1FsmCb));
    if (ret != ROK)
    {
        SLM_LOG(SLM_ERR, "SGetSBuf fail ret:%d\n", ret);
        slSetError(context, TSSMI_OUT_OF_RESRC,
                   TSSMI_ERR_IDX_UNSPECIFIED, TSSMI_OUT_OF_RESRC);
        goto ed_thE1_start_err;
    }
    
    cmMemset((U8 *)context->userVar, 0, sizeof(ThE1FsmCb));    
    cb = (ThE1FsmCb *)context->userVar;
    
    /* allocate memory for TL1 row */
    ret = SGetSBuf(0, 0, (Data**)&cb->thE1, sizeof(ThE1));
    if (ret != ROK)
    {
        SLM_LOG(SLM_ERR, "SGetSBuf ret:%d\n", ret);
        slSetError(context, TSSMI_OUT_OF_RESRC,
                   TSSMI_ERR_IDX_UNSPECIFIED, TSSMI_OUT_OF_RESRC);
        goto ed_thE1_start_err;
    }

    cmMemset((U8 *)cb->thE1, 0, sizeof(ThE1));
    cb->thE1->tableId = THE1_TAB_ID;
    
    /* setup rowKey */
    if (context->rowKey->numIndices != 4)
    {
        SLM_LOG(SLM_ERR, "Invalid numIndices:%d\n", context->rowKey->numIndices);
        slSetError(context, TSSMI_BAD_INDEX,
                   TSSMI_ERR_IDX_UNSPECIFIED, TSSMI_BAD_INDEX);
        goto ed_thE1_start_err; 
    }

    cb->thE1->shelfId.pres       = TRUE;
    cb->thE1->shelfId.val        = context->rowKey->rowIndex[0].value.longVal;
    cb->thE1->atcaSlot.pres      = TRUE;
    cb->thE1->atcaSlot.val       = context->rowKey->rowIndex[1].value.longVal;
    cb->thE1->slotNum.pres       = TRUE;
    shelfId = context->rowKey->rowIndex[0].value.longVal;
    pSlotId = context->rowKey->rowIndex[1].value.longVal;

    ret = SL_GET_VIRT_SLOT(shelfId, pSlotId, (int*)(&cb->thE1->slotNum.val));
    if (ret != ROK)
    {
       SLM_LOG(SLM_ERR,"Failed to map to virtual slot %d: %d-%d\n", cb->thE1->slotNum.val, shelfId, pSlotId);
       slSetError(context, TSSMI_BAD_INDEX,-1, TSSMI_BAD_INDEX);
       goto ed_thE1_start_err;
    }    

    cb->thE1->amcSlot.pres       = TRUE;
    cb->thE1->amcSlot.val        = context->rowKey->rowIndex[2].value.longVal;
    cb->thE1->portId.pres        = TRUE;
    cb->thE1->portId.val         = context->rowKey->rowIndex[3].value.longVal;    

    SLM_LOG(SLM_DBG, "slotNum:%d, amcSlot:%d, portId:%d\n",
                      cb->thE1->slotNum.val,
                      cb->thE1->amcSlot.val, cb->thE1->portId.val);
    
    /* Reject this command if destination m2uasig is OOS */
    ret=slClamTl1State(TELICA_GET_CLAM_ID(cb->thE1->slotNum.val, 1, SLCLM_AXCM_MGSIG_CLMID), &state);
    if((state == FALSE) || (ret != ROK))
    {
        SLM_LOG(SLM_ERR,"m2uasig of Slot %d is OOS\n", cb->thE1->slotNum.val);
        slSetError(context, TSSMI_NOT_VALID_STATE, -1, TSSMI_NOT_VALID_STATE);
        ret = RFAILED;
        goto ed_thE1_start_err;
    }
    
    DP("kavon slTl1_Ed_ThE1_fsm_start: just before call slDecodeSmiCols\n");
    /* decode TL1 request */
    ret = slDecodeSmiCols(cb->thE1->tableId, context->colArray, cb->thE1, NULL, (U16*) &errorCol1, UPDATE_REQD);
    if (ret != ROK)
    {
        SLM_LOG(SLM_ERR, "slDecodeSmiCols returned ret:%d\n", ret);
        slSetError(context, TSSMI_COL_BAD_VALUE, TSSMI_ERR_IDX_UNSPECIFIED, TSSMI_COL_BAD_VALUE);
        goto ed_thE1_start_err;
    }
    SLM_LOG(SLM_DBG, "slDecodeSmiCols done for tableId:%d\n", cb->thE1->tableId);

    if(cb->thE1->thlev.pres == TRUE)
    {
        cb->thE1->thlev.pres = FALSE;
    }

    if(cb->thE1->locn.pres == TRUE)
    {
        cb->thE1->locn.pres = FALSE;
    }

    if(cb->thE1->dirn.pres == TRUE)
    {
        cb->thE1->dirn.pres = FALSE;
    }

    if(cb->thE1->tmper.pres == FALSE)
    {
        cb->thE1->tmper.pres = TRUE;
        cb->thE1->tmper.val = THE1_TMPER_15MIN;
    }

    /* getNewTransId */
    ret = slTl1GetTransId(context, &transId);
    if (ret != ROK)
    {
        slSetError(context, TSSMI_OUT_OF_RESRC,
                   TSSMI_ERR_IDX_UNSPECIFIED, TSSMI_OUT_OF_RESRC);
        goto ed_thE1_start_err;
    }
    SLM_LOG(SLM_DBG, "transId:%ld\n", transId);
    
    /* setup hdr and pst for DBproxy */ 
    ret = slInitPstDbpMngmt(&pst, &dbMngmtReq, THE1_TAB_ID, DB_GET_ROWS, TRUE, 
                            DB_SET, transId);
    if (ret != ROK)
    {
        SLM_LOG(SLM_ERR, "slInitPstDbpMngmt ret:%d\n", ret);
        slSetError(context, TSSMI_GEN_ERROR, TSSMI_ERR_IDX_UNSPECIFIED, TSSMI_GEN_ERROR);
        goto ed_thE1_start_err;
    }
    
    SLM_LOG(SLM_DBG, "slInitPstDbpMngmt done\n");
    
    numRows = 1;
    ret = cmTbAddTbl(&dbMngmtReq.dbReq.tables, cb->thE1->tableId, numRows, 
                     cb->thE1, CM_TB_USER_ALLOCATED);
    if (ret != ROK)
    {
        SLM_LOG(SLM_ERR, "cmTbAddTbl failed, ret:%d\n", ret);
        slSetError(context, TSSMI_GEN_ERROR, TSSMI_ERR_IDX_UNSPECIFIED, TSSMI_GEN_ERROR);

        /*Free the TbNode and row*/
        cmTbDeInit(&dbMngmtReq.dbReq.tables);
        goto ed_thE1_start_err;
    } 
    SLM_LOG(SLM_DBG, "cmTbAddTbl done\n"); 
    
    /* pack and send GET request to DBproxy */
    ret = cmPkDbMngmtReq(&dbMngmtReq, &pst);
    if (ret != ROK)
    {
        SLM_LOG(SLM_ERR, "cmPkDbMngmtReq failed, ret:%d\n", ret);
        slSetError(context, TSSMI_GEN_ERROR, TSSMI_ERR_IDX_UNSPECIFIED, TSSMI_GEN_ERROR);

        /*Free the TbNode and row*/
        cmTbDeInit(&dbMngmtReq.dbReq.tables);
        goto ed_thE1_start_err;
    }  
    SLM_LOG(SLM_DBG, "cmPkDbMngmtReq done\n"); 
        
    cmTbDeInit(&dbMngmtReq.dbReq.tables);
    
    context->userState = THE1_AWT_DB_GET;
    
ed_thE1_start_err:
    if (ret != ROK)
    {
        SLM_LOG(SLM_ERR, "ed_thE1_start error\n");
        if (cb)
        {
            if (cb->thE1)
            {
                /* free cb->thE1 */
                ret = SPutSBuf(slCb.region, slCb.pool, cb->thE1, sizeof(ThE1));
                if (ret != ROK)
                {
                   SLM_LOG(SLM_ERR, "SPutSBuf failed\n");
                }
            }
            /* free context->userVar */
            ret = SPutSBuf(slCb.region, slCb.pool, cb, sizeof(ThE1FsmCb));
            if (ret != ROK)
            {
                SLM_LOG(SLM_ERR, "SPutSBuf failed\n");
            }
        }
  
        /* return control to TL1 FSM */        
        ret = slTl1Fsm(context, SLTL1_EV_X10_DONE);
        if (ret != ROK)
        {
            SLM_LOG(SLM_ERR, "slTl1Fsm failed, ret:%d\n", ret);
            slSetError(context, TSSMI_GEN_ERROR, TSSMI_ERR_IDX_UNSPECIFIED, TSSMI_GEN_ERROR);
        }   
    }

    SLM_LOG(SLM_DBG, "return:%d\n", ret);
    return (ret);
} /* slTl1_Ed_ThE1_fsm_start */

static S16 slTl1_Ed_ThE1_fsm_dbget(SlTl1Cb *context)
{
    S16           ret = RFAILED;
    U16           errorCol1;
    int           errorCol2;
    ThE1FsmCb     *cb;
    U32           numRows=0;
    DbMngmtReq    dbMngmtReq = {{0}};
    DbMngmtCfm    dbMngmtCfm = {{0}}; 
    Pst           pst;
    U32           transId;
    S16           tl1err = TSSMI_NO_ERROR;
 
    SLM_LOG(SLM_DBG, "context->userState:%ld\n", context->userState);
    
    cb = (ThE1FsmCb *)context->userVar;

    /* unpack DBproxy cfm */
    cmMemset((U8*) &dbMngmtCfm, 0, sizeof(DbMngmtCfm));
    ret = cmUnpkDbMngmtCfm(&dbMngmtCfm, &context->fsmEnt.pst, context->fsmEnt.mBuf);
    if (ret != ROK)
    {
        SLM_LOG(SLM_ERR, "cmUnpkDbMngmtCfm failed, ret:%d\n", ret);
        slSetError(context, TSSMI_GEN_ERROR, TSSMI_ERR_IDX_UNSPECIFIED, TSSMI_GEN_ERROR);
        goto ed_thE1_dbGet_err;
    } 
    
    /* check for DB error */
    if (dbMngmtCfm.cfm.status != ROK)
    {
        /* Map DB Error to ts.h(TL1) ErrorCode */
        tl1err = slGetRtrvErr (dbMngmtCfm.dbConf.u.error.errorCode.val);
        SLM_LOG(SLM_DBG, "get request FAILED, status:%d, tl1err%d\n", 
                          dbMngmtCfm.cfm.status, tl1err);
        slSetError(context, tl1err, TSSMI_ERR_IDX_UNSPECIFIED, tl1err);
        goto ed_thE1_dbGet_err;
    }
    
    /* unload data */
    ret = cmTbFndDlnkTbl(&dbMngmtCfm.dbConf.u.tables, THE1_TAB_ID, 
                         0, &numRows, (void **)&cb->dbThE1, TRUE);
    if (ret != ROK || (numRows == 0))
    {
        SLM_LOG(SLM_ERR, "cmTbFndDlnkTbl ret:%d\n", ret);
        slSetError(context, TSSMI_NO_SUCH_ROW, TSSMI_ERR_IDX_UNSPECIFIED, TSSMI_NO_SUCH_ROW);
        /* free memory allocated for DB row */
        goto ed_thE1_dbGet_err;
    } 
    
    SLM_LOG(SLM_DBG, "cmTbFndDlnkTbl done\n"); 
  
    /* DB doesn't set this for now; do this till that is fixed */
    cb->dbThE1->tableId = THE1_TAB_ID;

    DP("kavon slTl1_Ed_ThE1_fsm_dbget: just before call slDecodeSmiCols\n");
    /* decode TL1 request */
    ret = slDecodeSmiCols(cb->thE1->tableId, context->colArray, cb->thE1, 
                          NULL, (U16*) &errorCol1, UPDATE_REQD);
    if (ret != ROK)
    {
        SLM_LOG(SLM_ERR, "slDecodeSmiCols failed, returned ret:%d\n", ret);
        slSetError(context, TSSMI_COL_BAD_VALUE,
                   TSSMI_ERR_IDX_UNSPECIFIED, TSSMI_COL_BAD_VALUE);
        goto ed_thE1_dbGet_err;
    } 
    SLM_LOG(SLM_DBG, "slDecodeSmiCols done for tableId:%d\n", cb->thE1->tableId);
    
    /* update table */
    ret = tbUpdateRow(cb->thE1, cb->dbThE1); 
    if (ret != ROK)
    {
        SLM_LOG(SLM_ERR, "tbUpdateRow returned:%d\n", ret);
        slSetError(context, TSSMI_COL_BAD_VALUE,
                   TSSMI_ERR_IDX_UNSPECIFIED, TSSMI_COL_BAD_VALUE);

        goto ed_thE1_dbGet_err;
    }  

    SLM_LOG(SLM_DBG, "tbUpdateRow done\n");  

    SLM_LOG(SLM_DBG, "slotNum:%d, amcSlot:%d, portId:%d, montype:%d, thlev:%d, locn:%d, dirn:%d, tmper:%d\n",
                      cb->dbThE1->slotNum.val,
                      cb->dbThE1->amcSlot.val, cb->dbThE1->portId.val,
                      cb->dbThE1->montype.val, cb->dbThE1->thlev.val,
                      cb->dbThE1->locn.val, cb->dbThE1->dirn.val,
                      cb->dbThE1->tmper.val);    
    
    /* tbValidate() */
    ret = tbValidateRow(cb->dbThE1, DB_UPDATE, &errorCol2);
    if (ret != ROK)
    {
        SLM_LOG(SLM_ERR, "tbValidateRow returned:%d\n", ret);
        slSetError(context, TSSMI_COL_BAD_VALUE,
                   TSSMI_ERR_IDX_UNSPECIFIED, TSSMI_COL_BAD_VALUE);
        goto ed_thE1_dbGet_err;
    } 
    SLM_LOG(SLM_DBG, "tbValidateRow done\n"); 

    /* basic validation */
    ret = slValidateThE1(cb->dbThE1);
    if (ret != ROK)
    {
        SLM_LOG(SLM_ERR, "slValidateThE1 failed, ret:%d\n", ret);
        slSetError(context, TSSMI_COL_BAD_VALUE,
                   TSSMI_ERR_IDX_UNSPECIFIED, TSSMI_COL_BAD_VALUE);
        goto ed_thE1_dbGet_err;        
    }
    SLM_LOG(SLM_DBG, "slValidateThE1 done\n");        

    /* getNewTransId */
    ret = slTl1GetTransId(context, &transId);
    if (ret != ROK)
    {
        slSetError(context, TSSMI_GEN_ERROR,
                   TSSMI_ERR_IDX_UNSPECIFIED, TSSMI_GEN_ERROR);
        goto ed_thE1_dbGet_err;
    }
    SLM_LOG(SLM_DBG, "transId:%ld\n", transId);
    
    /* setup hdr and pst for DBproxy */ 
    ret = slInitPstDbpMngmt(&pst, &dbMngmtReq, THE1_TAB_ID, DB_UPDATE, 0, 
                          DB_SET, transId);
    if (ret != ROK)
    {
        SLM_LOG(SLM_ERR, "slInitPstDbpMngmt ret:%d\n", ret);
        slSetError(context, TSSMI_GEN_ERROR,
                   TSSMI_ERR_IDX_UNSPECIFIED, TSSMI_GEN_ERROR);
        goto ed_thE1_dbGet_err;
    }
    SLM_LOG(SLM_DBG, "slInitPstDbpMngmt done\n");
    
    numRows = 1;
    ret = cmTbAddTbl(&dbMngmtReq.dbReq.tables, cb->dbThE1->tableId, 
                     numRows, cb->dbThE1, CM_TB_USER_ALLOCATED);
    if (ret != ROK)
    {
        SLM_LOG(SLM_ERR, "cmTbAddTbl ret:%d\n", ret);
        slSetError(context, TSSMI_GEN_ERROR,
                   TSSMI_ERR_IDX_UNSPECIFIED, TSSMI_GEN_ERROR);
        cmTbDeInit(&dbMngmtReq.dbReq.tables);
        goto ed_thE1_dbGet_err;
    } 
    SLM_LOG(SLM_DBG, "cmTbAddTbl done\n"); 
    
    /* pack and send SET request to DBproxy */
    ret = cmPkDbMngmtReq(&dbMngmtReq, &pst);
    if (ret != ROK)
    {
        SLM_LOG(SLM_ERR, "cmPkDbMngmtReq ret:%d\n", ret);
        slSetError(context, TSSMI_GEN_ERROR,
                   TSSMI_ERR_IDX_UNSPECIFIED, TSSMI_GEN_ERROR);
        cmTbDeInit(&dbMngmtReq.dbReq.tables);
        goto ed_thE1_dbGet_err;
    }  
    SLM_LOG(SLM_DBG, "cmPkDbMngmtReq for SET done\n"); 
    
    cmTbDeInit(&dbMngmtReq.dbReq.tables);
    context->userState = THE1_AWT_DB_SET;
    
ed_thE1_dbGet_err:

    cmTbDeInit(&dbMngmtCfm.dbConf.u.tables);

    if (cb->thE1)
    {
        /* cb->thE1 will not be used, free cb->thE1 */
        if(SPutSBuf(slCb.region, slCb.pool, cb->thE1, sizeof(ThE1)) != ROK)
        {
            SLM_LOG(SLM_ERR, "SPutSBuf failed\n");
        }
        cb->thE1 = NULL;
    }
    
    if (ret != ROK)
    {
        SLM_LOG(SLM_ERR, "ed_thE1_dbGet error\n");
        if (cb)
        {
            if (cb->dbThE1)
            {
                /* free cb->dbThE1 */
                ret = SPutSBuf(slCb.region, slCb.pool, cb->dbThE1, numRows * sizeof(ThE1));
                if (ret != ROK)
                {
                   SLM_LOG(SLM_ERR, "SPutSBuf failed\n");
                }
                cb->dbThE1 = NULL;
            }

           /* free context->userVar */
            if(SPutSBuf(slCb.region, slCb.pool, cb, sizeof(ThE1FsmCb)) != ROK)
            {
                SLM_LOG(SLM_ERR, "SPutSBuf failed\n");
            }
            cb = NULL;
        }
  
        /* return control to TL1 FSM */        
        ret = slTl1Fsm(context, SLTL1_EV_X10_DONE);
        if (ret != ROK)
        {
            SLM_LOG(SLM_ERR, "slTl1Fsm ret:%d\n", ret);
            slSetError(context, TSSMI_GEN_ERROR,
                       TSSMI_ERR_IDX_UNSPECIFIED, TSSMI_GEN_ERROR);
        }   
    }

    SLM_LOG(SLM_DBG, "return:%d\n", ret);
    return (ret);
} /* slTl1_Ed_ThE1_fsm_dbget */

static S16 slTl1_Ed_ThE1_fsm_dbcfm(SlTl1Cb *context)
{
    S16           ret = RFAILED;
    ThE1FsmCb     *cb = NULL;
    U32           numRows = 0;
    DbMngmtCfm    dbMngmtCfm = {{0}}; 
    Pst           pst;
    SlclMngmt     *slClMngmt = NULL;
    S16           tl1err = TSSMI_NO_ERROR;

    cb = (ThE1FsmCb *)context->userVar;
    SLM_LOG(SLM_DBG, "context->userState:%ld\n", context->userState);
    
    /* unpack DBproxy cfm */
    cmMemset((U8*) &dbMngmtCfm, 0, sizeof(DbMngmtCfm));
    cmUnpkDbMngmtCfm(&dbMngmtCfm, &context->fsmEnt.pst, context->fsmEnt.mBuf);
    
    /* check for DB error */
    if (dbMngmtCfm.cfm.status == ROK)
    {
        SLM_LOG(SLM_DBG, "***set request succeeded***\n");
    }
    else 
    {
        if (dbMngmtCfm.dbConf.u.error.result.pres)
        {
            tl1err = dbMngmtCfm.dbConf.u.error.result.val;
        } 
        else 
        {
            /* Map DB Error to ts.h(TL1) ErrorCode */
            tl1err = slGetUpdErr (dbMngmtCfm.dbConf.u.error.errorCode.val);
            SLM_LOG(SLM_DBG, "set request FAILED, status:%d, tl1err:%d\n", 
                              dbMngmtCfm.cfm.status, tl1err);
        }
        slSetError(context, tl1err, TSSMI_ERR_IDX_UNSPECIFIED, tl1err);
     
        goto ed_thE1_dbcfm_err;
    }
    
    /* pack data to send to CLAM */
    /* allocate memory for SlclMngmt data to send to CLAM, TL1 fsm will free this */
    ret = SGetSBuf(slCb.region, slCb.pool, (Data**)&slClMngmt, sizeof(SlclMngmt));
    if (ret != ROK)
    {
        SLM_LOG(SLM_ERR, "SGetSBuf ret:%d\n", ret);
        slSetError(context, TSSMI_OUT_OF_RESRC, TSSMI_ERR_IDX_UNSPECIFIED,
                   TSSMI_OUT_OF_RESRC);

        goto ed_thE1_dbcfm_err;
    }
    
    /* setup hdr and the pst */
    ret = slInitPstClmMngmt(&pst, slClMngmt, THE1_TAB_ID);
    if (ret != ROK)
    {
        SLM_LOG(SLM_ERR, "slInitPstClmMngmt failed, ret:%d\n", ret);
        slSetError(context, TSSMI_GEN_ERROR,
                   TSSMI_ERR_IDX_UNSPECIFIED, TSSMI_GEN_ERROR);

        if (slClMngmt)
        {
            SPutSBuf(slCb.region, slCb.pool, slClMngmt, sizeof(SlclMngmt));
            slClMngmt = NULL;
        }
        goto ed_thE1_dbcfm_err;
    }
    SLM_LOG(SLM_DBG, "slInitPstClmMngmt done\n");

    numRows = 1;
    ret = cmTbAddTbl(&slClMngmt->t.r.tl1.tblLst, THE1_TAB_ID, numRows, 
                     cb->dbThE1, CM_TB_USER_ALLOCATED);
    if (ret != ROK)
    {
        SLM_LOG(SLM_ERR, "cmTbAddTbl failed, ret:%d\n", ret);
        slSetError(context, TSSMI_GEN_ERROR,
                   TSSMI_ERR_IDX_UNSPECIFIED, TSSMI_GEN_ERROR);
        cmTbDeInit(&slClMngmt->t.r.tl1.tblLst);
        if (slClMngmt)
        {
            SPutSBuf(slCb.region, slCb.pool, slClMngmt, sizeof(SlclMngmt));
            slClMngmt = NULL;
        }
        goto ed_thE1_dbcfm_err;
    } 
    SLM_LOG(SLM_DBG, "cmTbAddTbl done\n"); 
    
    /* generate CcsId */
    ret = slAddClamId(context, slClMngmt, TELICA_GET_CLAM_ID(cb->dbThE1->slotNum.val,1,SLCLM_AXCM_MGSIG_CLMID));
    if (ret != ROK)
    {
        SLM_LOG(SLM_ERR, "slAddClamId ret:%d\n", ret);
        slSetError(context, TSSMI_GEN_ERROR,
                   TSSMI_ERR_IDX_UNSPECIFIED, TSSMI_GEN_ERROR);

        /* cleanup allocated memory */
        cmTbDeInit(&slClMngmt->t.r.tl1.tblLst);
        if (slClMngmt)
        {
            SPutSBuf(slCb.region, slCb.pool, slClMngmt, sizeof(SlclMngmt));
            slClMngmt = NULL;
        }
        goto ed_thE1_dbcfm_err; 
    }  

ed_thE1_dbcfm_err:
    /* de-init the DB cfm tables */
    cmTbDeInit(&dbMngmtCfm.dbConf.u.tables);

    if (cb->dbThE1)
    {
        /* free cb->dbThE1 */
        SPutSBuf(slCb.region, slCb.pool, cb->dbThE1, numRows * sizeof(ThE1));
        cb->dbThE1 = NULL;
    }

    /* free userVar */
    ret = SPutSBuf(slCb.region, slCb.pool, cb, sizeof(ThE1FsmCb));
    if (ret != ROK)
    {
        SLM_LOG(SLM_ERR, "SPutSBuf failed\n");
    }
    
    /* send response to TL1 FSM */        
    ret = slTl1Fsm(context, SLTL1_EV_X10_DONE);
    if (ret != ROK)
    {
        SLM_LOG(SLM_ERR, "slTl1Fsm ret:%d\n", ret);
        slSetError(context, TSSMI_GEN_ERROR,
                   TSSMI_ERR_IDX_UNSPECIFIED, TSSMI_GEN_ERROR);
    }   

    SLM_LOG(SLM_DBG, "return:%d\n", ret);
    
    return (ret);
} /* slTl1_Ed_ThE1_fsm_dbcfm */

/**********************************************************************
** slTl1_Rtrv_ThE1_fsm
**
**     Desc:   wrapper function called from SLAM TL1 FSM for RTRV
**     Notes:
**
**     Ret:    ROK     - successful
**             RFAILED - failed
**
**********************************************************************/
S16 slTl1_Rtrv_ThE1_fsm(SlTl1Cb *context)
{
    S16   ret = RFAILED;

    SLM_LOG(SLM_DBG, "context->userState:%ld\n", context->userState);

    switch (context->userState)
    {
    case THE1_FSM_START:
        ret = slTl1_Rtrv_ThE1_fsm_start(context);
        break;
    case THE1_AWT_DB_GET:
        ret = slTl1_Rtrv_ThE1_fsm_dbget(context);
        break;
    default:
        /* error */
        SLM_LOG(SLM_ERR, "unsupported userState:%ld\n", context->userState);
        break;
    }

    return (ret);
} /* slTl1_Rtrv_ThE1_fsm */

/**********************************************************************
** slTl1_Rtrv_ThE1_fsm_start
**
**     Desc:   Sends request to DBP to retrieve the E1 table
**     Notes:  
**      
**     Ret:    ROK     - successful
**             RFAILED - failed
**
**********************************************************************/
static S16 slTl1_Rtrv_ThE1_fsm_start(SlTl1Cb *context)
{
    S16                  ret = RFAILED;
    ThE1FsmCb            *cb = NULLP;
    U32                  numRows=0;
    DbMngmtReq           dbMngmtReq = {{0}};
    Pst                  pst;
    U32                  transId;
    U8                   shelfId = 0;
    U16                  pSlotId = 0;
	U16                  errorCol1;

    SLM_LOG(SLM_DBG, "context->userState:%ld\n", context->userState);
    
    /* allocate memory for userVar */ 
    ret = SGetSBuf(0, 0, (Data**)&context->userVar, sizeof(ThE1FsmCb));
    if(ret != ROK)
    {
        SLM_LOG(SLM_ERR, "SGetSBuf ret:%d\n", ret);
        slSetError(context, TSSMI_OUT_OF_RESRC, TSSMI_ERR_IDX_UNSPECIFIED,
                   TSSMI_OUT_OF_RESRC);
        goto rtrv_thE1_start_err;
    }
    cmMemset((U8 *)context->userVar, 0, sizeof(ThE1FsmCb));
    cb = (ThE1FsmCb *)context->userVar;

    /* allocate memory for start and end rows */
    ret = SGetSBuf(slCb.region, slCb.pool, (Data**)&cb->thE1, sizeof(ThE1));
    if(ret != ROK)
    {
        SLM_LOG(SLM_ERR, "SGetSBuf ret:%d\n", ret);
        slSetError(context, TSSMI_OUT_OF_RESRC, TSSMI_ERR_IDX_UNSPECIFIED,
                   TSSMI_OUT_OF_RESRC);
        goto rtrv_thE1_start_err;
    }
    /* init the allocated buffer to zero. */
    /* Uninitialized values would create problems in DB with new generic API */
    cmMemset((U8 *)cb->thE1, 0, sizeof(ThE1));
    cb->thE1->tableId = THE1_TAB_ID;         

    /* setup rowKey */
    if (context->rowKey->numIndices != 4)
    {
        SLM_LOG(SLM_ERR, "Invalid numIndices:%d\n", context->rowKey->numIndices);
        slSetError(context, TSSMI_BAD_INDEX, -1, TSSMI_BAD_INDEX);
        goto rtrv_thE1_start_err;
    }

    cb->thE1->shelfId.pres       = TRUE;
    cb->thE1->shelfId.val        = context->rowKey->rowIndex[0].value.longVal;
    cb->thE1->atcaSlot.pres      = TRUE;
    cb->thE1->atcaSlot.val       = context->rowKey->rowIndex[1].value.longVal;
    cb->thE1->slotNum.pres       = TRUE;
    shelfId = context->rowKey->rowIndex[0].value.longVal;
    pSlotId = context->rowKey->rowIndex[1].value.longVal;

    ret = SL_GET_VIRT_SLOT(shelfId, pSlotId, (int*)(&cb->thE1->slotNum.val));
    if (ret != ROK)
    {
       SLM_LOG(SLM_ERR,"Failed to map to virtual slot %d: %d-%d\n", cb->thE1->slotNum.val, shelfId, pSlotId);
       slSetError(context, TSSMI_BAD_INDEX,-1, TSSMI_BAD_INDEX);
       goto rtrv_thE1_start_err;
    }
    cb->thE1->amcSlot.pres       = TRUE;
    cb->thE1->amcSlot.val        = context->rowKey->rowIndex[2].value.longVal;
    cb->thE1->portId.pres        = TRUE;
    cb->thE1->portId.val         = context->rowKey->rowIndex[3].value.longVal;    

    SLM_LOG(SLM_DBG, "slotNum:%d, amcSlot:%d, portId:%d\n",
                      cb->thE1->slotNum.val,
                      cb->thE1->amcSlot.val, cb->thE1->portId.val);

    /* decode TL1 request */
    ret = slDecodeSmiCols(cb->thE1->tableId, context->colArray, cb->thE1, NULL, (U16*) &errorCol1, UPDATE_REQD);
    if (ret != ROK)
    {
        SLM_LOG(SLM_ERR, "slDecodeSmiCols returned ret:%d\n", ret);
        slSetError(context, TSSMI_COL_BAD_VALUE, TSSMI_ERR_IDX_UNSPECIFIED, TSSMI_COL_BAD_VALUE);
        goto rtrv_thE1_start_err;
    }
    SLM_LOG(SLM_DBG, "slDecodeSmiCols done for tableId:%d\n", cb->thE1->tableId);

    /* read in table from Mysql */
    cmMemset((U8 *)&dbMngmtReq, 0, sizeof(DbMngmtReq));

    /* get new transId to send request to DBP */
    ret=slTl1GetTransId(context, &transId);
    if (ret!=ROK)
    {
        SLM_LOG(SLM_ERR, "slTl1GetTransId() ret:%d\n", ret);
        slSetError(context, TSSMI_OUT_OF_RESRC, TSSMI_ERR_IDX_UNSPECIFIED,
                   TSSMI_OUT_OF_RESRC);
        goto rtrv_thE1_start_err;
    }
    SLM_LOG(SLM_DBG, "context->transId:%d\n", context->transId);

    /* setup hdr and pst for DBproxy */ 
    ret=slInitPstDbpMngmt(&pst, &dbMngmtReq, THE1_TAB_ID, DB_GET_ROWS, 0, DB_SET, transId);
    if(ret != ROK)
    {
         SLM_LOG(SLM_ERR, "slInitPstHdr ret:%d\n", ret);
         slSetError(context, TSSMI_OUT_OF_RESRC, TSSMI_ERR_IDX_UNSPECIFIED,
                    TSSMI_GEN_ERROR);
         goto rtrv_thE1_start_err;
    }
    SLM_LOG(SLM_DBG, "slInitPstDbpMngmt done\n");

    numRows = 1;
    ret = cmTbAddTbl(&dbMngmtReq.dbReq.tables, cb->thE1->tableId, numRows, 
                     cb->thE1, CM_TB_USER_ALLOCATED);
    if(ret != ROK)
    {
        SLM_LOG(SLM_ERR, "cmTbAddTbl ret:%d\n", ret);
        slSetError(context, TSSMI_GEN_ERROR, TSSMI_ERR_IDX_UNSPECIFIED,
                   TSSMI_GEN_ERROR);
        goto rtrv_thE1_start_err;
    } 

    SLM_LOG(SLM_DBG, "cmTbAddTbl done\n"); 

    ret = cmPkDbMngmtReq(&dbMngmtReq, &pst);
    if(ret != ROK)
    {
        SLM_LOG(SLM_ERR, "cmPkDbMngmtReq ret:%d\n", ret);
        slSetError(context, TSSMI_GEN_ERROR, TSSMI_ERR_IDX_UNSPECIFIED,
                   TSSMI_GEN_ERROR);
        goto rtrv_thE1_start_err;
    }  

    context->userState = THE1_AWT_DB_GET;
    SLM_LOG(SLM_DBG, "context->userState:%ld\n", context->userState);

rtrv_thE1_start_err:
    /* free the DB tables, we can do this even if cmTbInit() wasn't called as long as the
     *        structure has been zeroed out */
    cmTbDeInit(&dbMngmtReq.dbReq.tables);
           
    if ((ret != ROK) && cb)
    { 
        /* free thE1 rows for the whole range for error */
        if (cb->thE1)
        {
            ret = SPutSBuf(slCb.region, slCb.pool, cb->thE1, sizeof(ThE1));
        }
        cb->thE1 = NULL;
        /* free userVar */
        SPutSBuf(slCb.region, slCb.pool, cb, sizeof(ThE1FsmCb));
        cb = NULL;
        /* return control to TL1 FSM; for ret!=ROK, slSetError should have been called already */
        ret = slTl1Fsm(context, SLTL1_EV_X10_DONE);
        if(ret != ROK)
        {
            SLM_LOG(SLM_ERR, "slTl1Fsm ret:%d\n", ret);
        }
    }

    return (ret);
} /* slTl1_Rtrv_ThE1_fsm_start */

/**********************************************************************
** slTl1_Rtrv_ThE1_fsm_dbget
**
**     Desc:   Receives from DBP the ThE1 table
**     Notes:  
**      
**     Ret:    ROK     - successful
**             RFAILED - failed
**
**********************************************************************/
static S16 slTl1_Rtrv_ThE1_fsm_dbget(SlTl1Cb *context)
{
    S16                  ret;
    ThE1FsmCb            *cb = NULLP;
    U32                  numRows=0;
    DbMngmtCfm           dbMngmtCfm = {{0}}; 
    S16                  tl1err = TSSMI_NO_ERROR;
    U32                  respSize = 0;
    void                 *respPtr = NULL;
	int                  i = 0;

    cb = (ThE1FsmCb *)context->userVar;

    /* check for error in userEvent */
    if (context->userEvent != SLTL1_USREV_DBP_CFM)
    {
        ret = RFAILED;
        SLM_LOG(SLM_ERR, "DB request failed, userEvent:%ld\n", context->userEvent);
        slSetError(context, TSSMI_GEN_ERROR, TSSMI_ERR_IDX_UNSPECIFIED,
                   TSSMI_GEN_ERROR);
        goto rtrv_thE1_db_cfm_err;
    }

    /* unpack database proxy confirm */
    cmMemset((U8*) &dbMngmtCfm, 0, sizeof(DbMngmtCfm));

    ret = cmUnpkDbMngmtCfm(&dbMngmtCfm, &context->fsmEnt.pst, context->fsmEnt.mBuf);

    /* check for DB error */
    if (dbMngmtCfm.cfm.status == ROK)
    {
        SLM_LOG(SLM_DBG, "***get request succeeded***\n");
    }
    else
    {
        SLM_LOG(SLM_ERR, "***get request FAILED, status:%d***\n", dbMngmtCfm.cfm.status);
        tl1err = slGetRtrvErr(dbMngmtCfm.dbConf.u.error.errorCode.val);
        slSetError(context, tl1err, TSSMI_ERR_IDX_UNSPECIFIED, tl1err);
        goto rtrv_thE1_db_cfm_err;
    }

    /* unload data */
    ret = cmTbFndDlnkTbl(&dbMngmtCfm.dbConf.u.tables, THE1_TAB_ID, 0, 
                         &numRows, (void **)&cb->dbThE1, TRUE);
    if(ret != ROK)
    {
        SLM_LOG(SLM_ERR, "cmTbFndTbl ret:%d\n", ret);
        slSetError(context, tl1err, TSSMI_ERR_IDX_UNSPECIFIED, tl1err);
        goto rtrv_thE1_db_cfm_err;
    }
    SLM_LOG(SLM_DBG, "cmTbFndTbl done, numRows:%ld\n", numRows); 

    for (i = 0 ; i < numRows; i++)
    {
        cb->dbThE1[i].tableId = THE1_TAB_ID;
        cb->dbThE1[i].shelfId.pres  = TRUE;
        cb->dbThE1[i].shelfId.val   = context->rowKey->rowIndex[0].value.longVal;
        cb->dbThE1[i].atcaSlot.pres = TRUE;
        cb->dbThE1[i].atcaSlot.val  = context->rowKey->rowIndex[1].value.longVal;

        SLM_LOG(SLM_DBG, "shelfId:%d, atcaSlot:%d, amcSlot:%d, portId:%d,"
                         "monType:%d, thLev:%d, locn:%d, dirn:%d, tmPer:%d\n",
                         cb->dbThE1[i].shelfId.val, cb->dbThE1[i].atcaSlot.val,
                         cb->dbThE1[i].amcSlot.val, cb->dbThE1[i].portId.val,
                         cb->dbThE1[i].montype.val, cb->dbThE1[i].thlev.val,
                         cb->dbThE1[i].locn.val, cb->dbThE1[i].dirn.val,
                         cb->dbThE1[i].tmper.val);
    }

    ret = slEncodeSmiCols(context, cb->dbThE1, numRows, &respPtr, &respSize);
    if (ret!=ROK)
    {
        SLM_LOG(SLM_ERR, "slEncodeSmiCols() returned %d\n", ret);
        slSetError(context, TSSMI_GEN_ERROR, TSSMI_ERR_IDX_UNSPECIFIED, TSSMI_GEN_ERROR);
        goto rtrv_thE1_db_cfm_err;
    }

rtrv_thE1_db_cfm_err:
   
    /* Free the memory allocated for TL1 row */
    if (cb && cb->thE1)
    {
        SPutSBuf(slCb.region, slCb.pool, cb->thE1, sizeof(ThE1));
        cb->thE1 = NULL;
    }
   
    /* free the DB cfm table, we can do this even if cmTbInit() 
     * wasn't called as long as the structure has been zeroed out */
    cmTbDeInit(&dbMngmtCfm.dbConf.u.tables);

    if(cb && cb->dbThE1)
    {
        SPutSBuf(slCb.region, slCb.pool, cb->dbThE1, sizeof(ThE1) * numRows);
        cb->dbThE1 = NULL;
    }

    if (cb)
    {
        SPutSBuf(slCb.region, slCb.pool, cb, sizeof(ThE1FsmCb));
        cb = NULL;	
    }

    /* return control to TL1 FSM */
    ret = slTl1Fsm(context, SLTL1_EV_X10_DONE);
    if(ret != ROK)
    {
        SLM_LOG(SLM_ERR, "slTl1Fsm ret:%d\n", ret);
    }

    return(ret);
} /* slTl1_Rtrv_ThE1_fsm_dbget */

static S16 slValidateThE1(ThE1 *thE1)
{
    return(ROK);
} /* slValidateThE1 */

S16
slThE1_bootKey_mod(SlClamCb *context, DbMngmtReq *dbMngmtReq)
{
    ThE1     *thE1;
    U32      numRows=0;
    S16      ret;

    ret = cmTbFndTbl(&dbMngmtReq->dbReq.tables, THE1_TAB_ID,
                     0, &numRows, (void **)&thE1);
    if (ret != ROK)
    {
        SLM_LOG(SLM_ERR, "%s: THE1: SLAM boot access failed, ret:%d\n",
                __FUNCTION__,ret);
        return (RFAILED);
    }

    SLM_LOG(SLM_NFY, "Modifying DB key for TH-E1 on slot:%d\n", context->slot);

    thE1->slotNum.pres   = TRUE;
    thE1->slotNum.val    = context->slot;

    return(ROK);
}

