/********************************************************************16**

        (c) COPYRIGHT 1989-1999 by Trillium Digital Systems, Inc.
                          All rights reserved.

     This software is confidential and proprietary to Trillium
     Digital Systems, Inc.  No part of this software may be reproduced,
     stored, transmitted, disclosed or used in any form or by any means
     other than as expressly provided by the written license agreement
     between Trillium and its licensee.

     Trillium warrants that for a period, as provided by the written
     license agreement between Trillium and its licensee, this
     software will perform substantially to Trillium specifications as
     published at the time of shipment and the media used for delivery
     of this software will be free from defects in materials and
     workmanship.

     TRILLIUM MAKES NO OTHER WARRANTIES, EXPRESS OR IMPLIED, INCLUDING
     WITHOUT LIMITATION WARRANTIES OF MERCHANTABILITY OR FITNESS FOR
     A PARTICULAR PURPOSE WITH REGARD TO THIS SOFTWARE OR ANY RELATED
     MATERIALS.

     IN NO EVENT SHALL TRILLIUM BE LIABLE FOR ANY INDIRECT, SPECIAL,
     OR CONSEQUENTIAL DAMAGES IN CONNECTION WITH OR ARISING OUT OF
     THE USE OF, OR INABILITY TO USE, THIS SOFTWARE, WHETHER BASED
     ON BREACH OF CONTRACT, TORT (INCLUDING NEGLIGENCE), PRODUCT
     LIABILITY, OR OTHERWISE, AND WHETHER OR NOT IT HAS BEEN ADVISED
     OF THE POSSIBILITY OF SUCH DAMAGE.

                       Restricted Rights Legend

     This software and all related materials licensed hereby are
     classified as "restricted computer software" as defined in clause
     52.227-19 of the Federal Acquisition Regulation ("FAR") and were
     developed entirely at private expense for nongovernmental purposes,
     are commercial in nature and have been regularly used for
     nongovernmental purposes, and, to the extent not published and
     copyrighted, are trade secrets and confidential and are provided
     with all rights reserved under the copyright laws of the United
     States.  The Government's rights to the software and related
     materials are limited and restricted as provided in clause
     52.227-19 of the FAR.

                    IMPORTANT LIMITATION(S) ON USE

     The use of this software is limited to the Use set
     forth in the written License Agreement between Trillium and
     its Licensee. Among other things, the Use of this software
     may be limited to a particular type of Designated Equipment.
     Before any installation, use or transfer of this software, please
     consult the written License Agreement or contact Trillium at
     the location set forth below in order to confirm that you are
     engaging in a permissible Use of the software.

                    Trillium Digital Systems, Inc.
                  12100 Wilshire Boulevard, suite 1800
                    Los Angeles, CA 90025-7118, USA

                        Tel: +1 (310) 442-9222
                        Fax: +1 (310) 442-1162

                   Email: tech_support@trillium.com
                     Web: http://www.trillium.com

*********************************************************************17*/

/********************************************************************20**

        Name:     Call Control - body 1

        Type:     C source file

        Desc:     C source code for Call Control Lower Layers (PSIFs), 
                  Resource Manager, Router, Switching Fabric Manager, 
                  System Service and Layer Management service user 
                  primitives supplied by TRILLIUM.

        File:     cc_bdy1.c

        Sid:      cc_bdy1.c 1.5  -  12/21/99 07:07:13

        Prg:      rs

*********************************************************************21*/


/************************************************************************

     Note:

     This file has been extracted to support the following options:

     Option             Description
     ------    ------------------------------


************************************************************************/


/*

----------------------------> Table of Content <-----------------------------

Chapter 0: interface functions to system services
Chapter 1: interface functions to layer management
Chapter 2: interface functions to PSIF Service Provider
Chapter 3: interface functions to Resource Manager Service Provider
Chapter 4: interface functions to Router Service Provider
Chapter 5: interface functions to Switching Fabric Manager Service Provider

----------------------------------------------------------------------------- 
The following functions for the CCT interface, are provided in this 
file (Chapter 2):

   CcLiCctConInd        - Connection Establishment Indication
   CcLiCctConCfm        - Connection Establishment Confirmation
   CcLiCctCnStInd       - Connection Status Indication
   CcLiCctRelInd        - Release Indication
   CcLiCctRelCfm        - Release Confirmation
   CcLiCctRscCfm        - Resource Confirmation
   CcLiCctStaInd        - Call Status Indication
   CcLiCctModInd        - Call Modification Indication
   CcLiCctModCfm        - Call Modification Confirmation
   CcLiCctAddrInd       - Address Indication
   CcLiCctMntStaInd     - Maintenance Status Indication
   CcLiCctBndCfm        - Bind Confirmation

It is assumed that the following functions are provided in the
Respective PSIF service provider file:

   CcLiCctBndReq        - Bind Request
   CcLiCctConReq        - Connection Establishment Request
   CcLiCctConRsp        - Connection Establishment Response
   CcLiCctCnStReq       - Connection Status Request
   CcLiCctRelReq        - Release Indication
   CcLiCctRelRsp        - Release Response
   CcLiCctRscRsp        - Resource Response
   CcLiCctModReq        - Modification Request
   CcLiCctModRsp        - Modification Response
   CcLiCctMntStaReq     - Maintenance Status Request

The following functions for the RMT interface, are provided in this 
file (Chapter 3):

   CcLiRmtAlocCfm       - Resource Allocation Confirmation
   CcLiRmtModCfm        - Resource Modification Confirmation
   CcLiRmtDealocCfm     - Reosorce Deallocation Cofirmation                
   CcLiRmtBndCfm        - Bind Confirmation

It is assumed that the following functions are provided in the
Resource Manager service provider file:

   CcLiRmtBndReq        - Bind Request
   CcLiRmtAlocReq       - Resource Allocation Request
   CcLiRmtModReq        - Resource Modification Request
   CcLiRmtDealocReq     - Reosorce Deallocation Reqeust                

The following functions for the RTT interface, are provided in this 
file (Chapter 4):

   CcLiRttRteCfm        - Connection Establishment Confirmation
   CcLiRttBndCfm        - Bind Confirmation
   CcLiRttMntStaInd     - Maintenance Status Indication

It is assumed that the following functions are provided in the
Router layer service provider file:

   CcLiRttMntStaReq     - Maintenance Status Request

The following functions for the SFT interface, are provided in this 
file (Chapter 4):

   CcLiSftConCfm        - Switching Connect Confirmation
   CcLiSftRelCfm        - Switching Release Confirmation
   CcLiSftBndCfm        - Bind Confirmation

It is assumed that the following functions are provided in the
Switching Fabric Manager layer service provider file:

   CcLiSftBndReq        - Bind Request
   CcLiSftConReq        - Switching Connect Request
   CcLiSftRelReq        - Switching Release Request
*/


/*
 
Layer management provides the necessary functions to control and
monitor the condition of each protocol layer.

The following functions for LCC interface, are provided in this file:

     CcMiLccCfgReq       Configure Request
     CcMiLccStaReq       Status Request
     CcMiLccStsReq       Statistics Request
     CcMiLccCntrlReq     Control Request

It is assumed that the following functions are provided in the
layer management service provider file.

     CcMiLccCfgInd       Configuration Indication
     CcMiLccStaInd       Status Indication
     CcMiLccStsInd       Statistics Indication
     CcMiLccTrcInd       Trace Indication

*/


/*
 *      This software may be combined with the following TRILLIUM
 *      software:
 *
 *      part no.                      description
 *      --------    ----------------------------------------------
 *
 */


  
/* header include files (.h) */

#include "envopt.h"             /* environment options */  
#include "envdep.h"             /* environment dependent */
#include "envind.h"             /* environment independent */
#include "unistd.h"             /* for unlink */
#include "gen.h"                /* general */
#include "ssi.h"                /* system services interface */
#include "cm_ss7.h"             /* general SS7 layer */
#include "cm_atm.h"             /* general ATM layer */
#include "cm_hash.h"            /* hash list structure */
#include "cm_llist.h"           /* linked list structure */
#include "cm_cc.h"              /* Common Call Control Hash Defs */
#include "sit.h"                /* PSIF interface*/
#include "int.h"                /* PSIF interface*/
#include "cst.h"                /* CAS  interface*/
#include "sipt.h"
#include "mgcpt.h"
#include "cct.h"                /* PSIF interface*/
#include "rtt.h"                /* router interface */
#include "rmt.h"                /* Resource Manager interface */
#include "sft.h"                /* Switching Fabric Manager interface */
#include "lcc.h"                /* layer management */
#include "mf.h"                 /* Message Decoding / Encoding Hdr */
#include "cm5.h"                /* timers */
#include "msg_enum_1299.h"      /* Common message types (CC and FIC) */
#include "fct.h"                /* FIC datastructures */
#include "fst.h"                /* FIC datastructures */
#include "cc.h"                 /* Call Control Private header file */
#include "cc_err.h"             /* Call Control Error Codes */
#ifdef ZC
#include "cm_pftha.h"      /* common PSF */
#include "lzc.h"           /* ZC LM defines */
#include "zc.h"            /* ZC defines */
#include "zc_err.h"        /* ZC error defines */
#endif /* ZC */
#include "cm_sdp.h"             /* SDP */
#include "tb_cccfg.h"

/* header/extern include files (.x) */

#include "gen.x"                /* general */
#include "ssi.x"                /* system services interface */
#include "cm_ss7.x"             /* general SS7 layer */
#include "cm_atm.x"             /* general ATM layer */
#include "cm_hash.x"            /* hash list structure */
#include "cm_llist.x"           /* linked list structure */
#include "cm_lib.x"             /* common library */
#include "cm_cc.x"              /* Common Call Control Typedefs */
#include "mgct.h"               /* GCC <-> MGI Related DataStructures */
#include "sit.x"                /* PSIF interface*/
#include "bicct.x"                /* PSIF interface*/
#include "int.x"                /* PSIF interface*/
#ifdef CCXM
#include "xmt.x"                /* PSIF Interface */
#endif /* CCXM */
#include "cst.x"                /* cas  Interface */
#include "sipt.x"
#include "mgcpt.x"
#include "cct.x"                /* PSIF Interface */
#include "rtt.x"                /* router interface */
#include "rmt.x"                /* Resource Manager interface */
#include "sft.x"                /* Switching Fabric Manager interface */

#include "lcc.x"                /* layer management */
#include "mf.x"                 /* Message Encoding/Decoding Hdr */
#include "cm5.x"                /* timers */
#include "fct.x"                /* FIC datastructures */
#include "cc_cache.x"                 /* Call Control Private header file */
#include "cc.x"                 /* Call Control Private header file */
#include "cct_geo.x"		/* Geo Audit */
#include "cc_geo.x"             /* MGC GR CC header file */
#ifdef ZC
#include "cm_pftha.x"      /* common PSF */
#include "lzc.x"           /* ZC LM */
#include "zc.x"            /* ZC typedefs */
#endif /* ZC */

#include "gdi_api.h"                 /* ON Switch Billing API */
#include "git_class.h"               /* for gitDebugPrint() */
#include "lsf.x"
#include "sf.x"

#include "tfsu_memmap.h"
#include "SoftSwitchDBApi.h"
#include "proc_info.h"
#include "cc_cdr_internal.h"
#include "cm_pcrcdr.h"
#include "tsmc_logutil.h"
#include "mplogUtils.h"
#include "telica_priorities.h"
#include "telica_abort.h"
#include "app_info.h"
#include "cm_log.h"
#include "cc_overlap.h"
#include "gets_dn.h"

#include "x2ct.x"
#include "cc_li.x"

#include "cc_asn.h"   /* FID16206.0 */
#include "cc_ccbs.x"   
#include "fst.x"
/* local defines */

/* local typedefs */

/* local externs */

/* forward references */

/* public variable declarations */

PUBLIC CcGenCp ccCp;             /* B-ISUP control point */
PUBLIC CcGenPrfl ccGenPrfl;
PUBLIC TskInit ccInit;           /* initialization */

PUBLIC DBContext   dbContext;

PUBLIC Pst ccFicPst;
PUBLIC Pst *pCcFicPst;

PUBLIC Pst ccGccPst;
PUBLIC Pst *pCcGccPst   = NULLP;

/* code trace */
PUBLIC U8 ccCodeTrcLvl=LOGERROR;

U32 obsoleteCallCounter;

PUBLIC U16 glLiConcurrX3Cnt = 0;   /* FID15083.1 - global parameter of number of concurrent X3 calls  */
PUBLIC U16 glLiConcurrX2Cnt = 0;   /* FID15083.1 - global parameter of number of concurrent X2 calls  */

PUBLIC CcAlarmCb_t ccAlarmCb;      /* FID16259.0 */      
PUBLIC U32 glFraudAlarmCnt  = 0;   /* FID16259.0 */
PUBLIC Bool glFraudAlarmCntChg  = FALSE;   /* FID16259.0 */
PUBLIC Bool glInterMgcSipTgExists = FALSE;   /* FID16766.0 */

/* SAPs' pointer lists */
PUBLIC CcPsSap  **ccPsSapLstPtr; /* pointer to list of Protocol Specific Saps*/
PUBLIC CcGenSap **ccRmSapLstPtr; /* pointer to list of Resource Manager SAP */
PUBLIC CcGenSap **ccRtSapLstPtr; /* pointer to list of Router SAP */
PUBLIC CcGenSap **ccSfSapLstPtr; /* pointer to list of SF Manager SAP */
PUBLIC CcProfCb **ccProfLstPtr; /* pointer to list of ATM Parameters Profile */
PUBLIC U8       **ccObsTablePtr;  /* pointer to the observation table */
PUBLIC CcGenSap **ccX2SapLstPtr;  /* FID15083.0 - pointer to list of X2SIG SAP */    

/* Hash Tables */
PUBLIC CmHashListCp ccSuInstTbl;   /* Connection table hashed on suConnId */
PUBLIC CmHashListCp ccSuOgInstTbl;   /* Connection table hashed on suConnId */
PUBLIC CmHashListCp ccGeoSuInstTbl;/* MGC GR Audit Term table hashed on suConnId */
PUBLIC CmHashListCp ccicBCMInsTbl; /* BCM structure hashed on transId */
PUBLIC CmHashListCp ccogBCMInsTbl; /* BCM structure hashed on transId */
PUBLIC CmHashListCp ccTgTbl;       /* Trunk Group table hashed on TGN */
PUBLIC CmHashListCp ccSdpTbl;      /* SDP Profile table hashed on sdpPrflId */
PUBLIC CmHashListCp ccBillingPrflTbl;  /* Billing Profile table hashed on prflId */
PUBLIC CmHashListCp ccMgctInsTbl;  /* MGI structure hashed on SuCtxId */
PUBLIC CmHashListCp ccGeoMgInsTbl; /* GeoCon hashed on CtxId for Geo Aud Calls*/
PUBLIC CmHashListCp ccGeoOnNetTbl; /* OnNet GeoCon hashed on Ingress suConnId */
PUBLIC CmHashListCp ccIntfcSAPTbl; /* SAP associated with an interface table */
PUBLIC CmHashListCp ccServMsgIdTbl; /* Conn table hashed on service msgId */
/* FID 15261.0 + */
PUBLIC CmHashListCp ccMgSapTbl;   /* FID15261.0 mgSapCb hased on mgSpId*/
PUBLIC CmHashListCp ccGeoConTbl;   /* FID 15261 ccGeoCon hashed on callId in geoRsc */
/* FID 15261.0 - */
PUBLIC CmHashListCp ccIvrAnncIdTbl; /* IVR announcement Ids */

PUBLIC CmHashListCp ccPrtclRelPrflTbl;   /* Protocol release profile table */
PUBLIC CmHashListCp ccProtRelTbl;
PUBLIC CmHashListCp ccFailCndPrflTbl;    /* Failure condition profile table */
PUBLIC CmHashListCp ccParamSuppPrflTbl;  /* Parameter suppression table */
PUBLIC CmHashListCp ccParamDfltPrflTbl;  /* Parameter default table */
PUBLIC CmHashListCp ccExtAnncIdTbl; /* External announcement table for use with Send_To_Resource */
PUBLIC CmHashListCp ccFraudPrflTbl; /* Fraud profile Table */
PUBLIC CmHashListCp ccFraudCallTbl; /* Contains info of the fraud calls */
PUBLIC CmHashListCp ccCasTermTbl; /* Context ID hashed on CAS-MGC termination ID*/
PUBLIC CmTreeCp     ccProtRelMapTbl; /* Protocol Release to FailCnd Map table */ 
PUBLIC CmTreeCp     ccRaProtRelTbl; /* Route Advance Protocol Rel table */
PUBLIC CmHashListCp ccFreedConTbl; /* Contains info of the fraud calls */
PUBLIC CmHashListCp ccSipPrflTbl; /* SIP profiles hashed on profile ID */
PUBLIC CmHashListCp ccSipTPrflTbl; /* SIPT profiles hashed on profile ID */
PUBLIC CmHashListCp ccMgNameTbl;  /*MG name hash table*/
PUBLIC CmHashListCp ccRealmNameTbl;  /*FID 14732 - Realm Name hash table*/
PUBLIC CmHashListCp ccMgMapAddrSdpOSdpCTbl; /*FID 14841.2 & 15261.0 - MG map addr hash table*/
PUBLIC TknU32       defaultRealmId4MgMapAddr; /*FID 14841.2 - MG map addr default realmId*/
PUBLIC TknU32       defaultMgListId4MgMapAddr; /*FID 15261.0 - MG map addr default listId*/
PUBLIC TknU8        defaultMgAdvance4MgMapAddr; /*FID 14457.0 - MG map addr default list MgAdvance */
PUBLIC CmHashListCp ccComNumTbl; /*FID 15083.0 - CIN duplicate key hash table */
PUBLIC CmHashListCp ccPrflCcc;    /*  FID 15083.0 - CCC hash list table */
PUBLIC CmHashListCp ccMcgCodeTbl; /* FID 16169.0 */
PUBLIC CmHashListCp ccMdPrflTbl;  /* FID15737.0 */
PUBLIC CmHashListCp ccDummyCallInfoTbl;  /* FID16206.0 */
PUBLIC CmHashListCp ccMrfListTbl;/* FID17174.0 */
PUBLIC CmHashListCp ccMrfAnncTbl;/* FID17174.0 */
CONSTANT Txt CCONT[] = "Call Control";

/* FID15169.0 */
PUBLIC CmHashListCp ccMgcPcTbl; 

EXTERN CmTreeCp ccGetsDnTbl; /* bug88508 */

/* Timers */
CmTqCp ccConTqCp;             /* cc connection timing queue control point */
CmTqType ccConTq[TQNUMENT];   /* cc connection timing queue */
CmTqCp ccSapTqCp;             /* cc SAP timing queue control point */
CmTqType ccSapTq[TQNUMENT];   /* cc SAP timing queue */
CmTqCp ccFraudTqCp;           /* cc fraud trap timing queue control point */
CmTqType ccFraudTq[TQNUMENT]; /* cc fraud trap timing queue */
CmTqCp   ccCon100TqCp;        /* cc 100 msec Timer control point    */
CmTqType ccCon100Tq[TQNUMENT]; /* cc 100 msec Timer Q                */


PUBLIC CcSwtCfg     ccSwitchCfg;
PUBLIC CcBillingSys ccBillingSystem;
PUBLIC CcIvrState   ccIVRState;
PUBLIC CcStSt       ccStSt;

Bool ainLoggingInd = 0;/* FID 171670 */
Bool iwLoggingInd  = 0;/* FID 171670 */
Bool billLoggingInd= 0;/* FID 171670 */
U32  ccOrigDbgMask = 0;

U8 inDbgMsk = 9;
U8 inOrigDbgMsk = 9;
U32 ccIwDbgMask = 0;
U32 ccOrigIwDbgMask = 0;
Bool ccCheckRscState = 1;
PUBLIC S32  gfrdLogfile;
PUBLIC U32 noOfCalls    = 0;
PUBLIC U32 noOfSuccCdrCalls = 0; 
PUBLIC U32 noOfCatOrMdpCalls = 0; /* Calls that failed to generate CDR because of CC_CAT_TEST or MdpTrunkGroup */ 
PUBLIC U32 noOfNOChrgCgCdCalls = 0; 
U8 ccCongestion = 0;

S32 ccLastExcessMgCtxts = 0; /* 42713 */

#if 1 /* Do not disable these counts. Consult Marlboro if needed  */
      /* These are sel-decrementing counts. They will turn-off automatically */
PUBLIC  U8  logFirstConInd = 5;
PUBLIC  U8  logFirstConReq = 5;
PUBLIC  U8  logFirstConCfm = 5;
PUBLIC  U8  logFirstConRsp = 5;
PUBLIC  U16  logFirstRelInd = 100;
PUBLIC  U16 logFirstRelReq = 100;
PUBLIC  U8  logRelCause = 0;
PUBLIC  U8  logRelType = 0;
PUBLIC  int  CcExtRelCause[256];
PUBLIC  int  CcIntRelCause[256];
#endif /* End */

PUBLIC  U16 logFailCndId = 0;  /* bug 69143 */
PUBLIC  U16 logFailcndNum = 0;

/* Bug 34323 - Error counters to prevent flooding of error logs */
PUBLIC CcErrorCounter ccErrorCounter[CC_ERR_MAX_ERRTYPE] =
{
    {0,"ERROR: Failed to find con suCtxId", 0},
    {0,"ERROR: AUDIT: suCtxId: not found in hashlist sending SubAll",0},
    {0,"ERROR: Received Error on TxnCfm txnCfm->status",0},
    {0,"ERROR: RmMgStatus if RM_OG_OOS: suCtxId",0}
};

PUBLIC Bool reEntryFlag = FALSE;

PUBLIC CcCongCb_t ccCongCb;

/* private variable declarations */
PRIVATE S16 CcInitPst( Pst *pst, Ent dstEnt, Selector selector);

/* public declarations */
S16 ccInitGDI(void);

/*
 *    Chapter 0: interface functions to System Services
 */
U8 ccGetXchgTypeForSipw(SuId	  suIdt, UConnId   suConnIdt, UConnId   spConnIdt);


#ifndef TELICA_MGC
/*
* Function Name:        cleanup
*
* Description:          Interrupt handler for termination signals.
*/
#ifdef __STDC__
static void cleanup
(
int sig
)
#else
static void cleanup ( sig )
int sig ;
#endif
{
        GDI_close();

        signal( sig, SIG_DFL);

        kill(getpid(), sig);
}
#endif

/*
*
*      Fun:   ccActvInit
*
*      Desc:  Invoked by system services to initialize Call Control. This
*             is an entry point used by Call Control to initialize its
*             global variables, before becoming operational.
*
*             The first and second parameters (entity, instance)
*             specify the entity and instance id of this Call Control task.
*
*             The third parameter (region) specifies the memory region
*             from which Call Control should allocate structures and buffers.
*
*             The fourth parameter (reason) specifies the reason for
*             calling this initialization function.
*
*             Allowable values for parameters are specified in ssi.h.
*
*      Ret:   ROK     - successful,
*
*      Notes: None.
*
*      File:  cc_bdy1.c
*
*/

#ifdef ANSI
PUBLIC S16 ccActvInit
(
Ent    ent,                 /* entity */
Inst   inst,                /* instance */
Region region,              /* region */
Reason reason               /* reason */
)
#else
PUBLIC S16 ccActvInit(ent, inst, region, reason)
Ent    ent;                 /* entity */
Inst   inst;                /* instance */
Region region;              /* region */
Reason reason;              /* reason */
#endif
{
   int retVal = ROK;

   TRC3(ccActvInit)

   /* initialize task configuration parameters */
   ccInit.ent     = ent;           /* entity */
   ccInit.inst    = inst;          /* instance */
   ccInit.region  = region;        /* static region */
   ccInit.pool    = 0;             /* static pool */
   ccInit.reason  = reason;        /* reason */
   ccInit.cfgDone = FALSE;         /* configuration done */
   ccInit.acnt    = TRUE;          /* enable accounting */
#ifdef CC_USTA
   ccInit.usta    = TRUE;          /* enable unsolicited status */
#else
   ccInit.usta    = FALSE;         /* disable unsolicited status */
#endif /* CC_USTA */
   ccInit.trc     = FALSE;         /* enable trace */

#if 1
   ccInit.dbgMask = 0;             /* disable all debug options */
#else
   ccInit.dbgMask = 0xffffffff;    /* enable all debug options */
#endif

   ccInit.procId  = SFndProcId();  /* processor id */

   /* initialize other lists */

   ccPsSapLstPtr = NULLP;    /* pointer to list of Protocol Specific Saps */
   ccRmSapLstPtr = NULLP;    /* pointer to list of Resource Manager SAP*/
   ccRtSapLstPtr = NULLP;    /* pointer to list of Router SAP */
   ccSfSapLstPtr = NULLP;    /* pointer to list of SF Manager SAP */
   ccProfLstPtr  = NULLP;    /* pointer to list of ATM Parameters */
   ccX2SapLstPtr = NULLP;    /* pointer to list of X2SIG SAP */

   /* FID 16794.0 */
   ccCp.raProtRelSipCnt = 0;
   ccCp.raProtRelSiptCnt = 0;
   ccCp.raProtRelSiCnt = 0;
   ccCp.raProtRelInCnt = 0;

   /* Initialize the Post Structure to the GCC */
   CcInitPst(&ccFicPst, ENTFIC, 1);
   pCcFicPst = &ccFicPst;

   CcInitPst(&ccGccPst, ENTCC, 0);
   pCcGccPst = &ccGccPst;

   /* call external function for intialization */
   ccInitExt();

#ifdef ZC
   zcActvInit(ent, inst, region, reason);
#endif /* ZC */

#ifndef TELICA_MGC
   retVal = ccDBInit(&dbContext, TELICA_SIG_DB_NAME);
   if(retVal == -1)
   {
      /* WEChin - BUG 14036 */
      procFailure("cc_bdy1.c: ccDBInit Failed");
      sleep(10);
      exit(1);
   }
#endif

#ifndef TELICA_MGC
#ifndef GDI_NO_SHM_IPC
   /* hxu 7/10/02, changed for SP2 */
   if(IS_SP2_INT)
   {
      ccInitGDI();
   }
#endif
#endif
#ifndef ATCA
   if(IS_CM())
   {
       /* Note, for the CM, need a different name and Replication not supported*/
       gfrdLogfile = cmMPLogInit( "signaling_fraud",
                         FRAUD_MAX_LOG_ENTRIES, FRAUD_MAX_LOG_QUEUE_ENTRIES,
                         MPLOG_DATE_FLAG,
                         SP_SIGNALING_MP_LOGGING_THREAD_PRIO, FALSE);
   }
   else
#endif
   {
       gfrdLogfile = MPlogInit( "/Telica/swCPU/CurrRel/log/signaling.fraud",
                         FRAUD_MAX_LOG_ENTRIES, FRAUD_MAX_LOG_QUEUE_ENTRIES,
                         MPLOG_DATE_FLAG, 
                         SP_SIGNALING_MP_LOGGING_THREAD_PRIO,
                         SP_SIGNALING_LOGMGR_THREAD_PRIO, FALSE);

   }
   if (gfrdLogfile < 0)
   {
     CCLOGERR ("ccActvInit: logInit returned:%ld\n",gfrdLogfile);
   }

   RETVALUE(retVal);
} /* end of ccActvInit */


/*
*     Chapter 1: interface functions to layer management
*/


/*
*
*      Fun:   CcMiLccCfgReq
*
*      Desc:  This function is used by the Layer Management to
*             configure the layer. The layer will only accept
*             a Connect Request on a SAP after it has been
*             configured.
*
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy1.c
*
*/

#ifdef ANSI
PUBLIC S16 CcMiLccCfgReq
(
Pst    *pst,                    /* post */
CcMngmt *cfg                    /* configuration */
)
#else
PUBLIC S16 CcMiLccCfgReq(pst, cfg)
Pst    *pst;                    /* post */
CcMngmt *cfg;                   /* configuration */
#endif
{
  S16 ret = RFAILED;

   TRC3(CcMiLccCfgReq)

   /* validate primitive parameters */

#if (ERRCLASS & ERRCLS_INT_PAR)
   if ( cfg->hdr.elmId.elmnt == STCCSDP )
   {
      if ((cfg == NULLP) ||
          (cfg->hdr.entId.ent != ccInit.ent)   ||   /* entity id */
          (cfg->hdr.entId.inst != ccInit.inst) ||   /* instance id */
          ( (cfg->hdr.msgType != TCFG) && (cfg->hdr.msgType != TUCFG) ))    /* message type */
      {
         CCLOGERROR(ERRCLS_INT_PAR, ECC233, (ErrVal) 0,
                   "CcMiLccCfgReq() Failed, invalid header");
         if (cfg == NULLP)
            ccGenCfm(pst, EVTLCCCFGCFM, cfg, LCM_PRIM_NOK, LCC_REASON_INVALID_HDRPARM);
         else
         {
            if (cfg->hdr.entId.ent != ccInit.ent)
            {
               ccGenCfm(pst, EVTLCCCFGCFM, cfg, LCM_PRIM_NOK, 
                        LCM_REASON_INVALID_ENTITY);
               RETVALUE(RFAILED);
            }
            if (cfg->hdr.entId.inst != ccInit.inst)
            {
               ccGenCfm(pst, EVTLCCCFGCFM, cfg, LCM_PRIM_NOK,
                        LCM_REASON_INVALID_INSTANCE);
               RETVALUE(RFAILED);
            }
            if (cfg->hdr.msgType != TCFG)
            {
               ccGenCfm(pst, EVTLCCCFGCFM, cfg, LCM_PRIM_NOK, 
                        LCM_REASON_INVALID_MSGTYPE);
               RETVALUE(RFAILED);
            }
         } 
      }
   }
   else if ((cfg == NULLP) ||
       (cfg->hdr.entId.ent != ccInit.ent)   ||   /* entity id */
       (cfg->hdr.entId.inst != ccInit.inst) ||   /* instance id */
       ((cfg->hdr.msgType != TCFG) && (cfg->hdr.msgType != TUCFG))) /* message type */
   {
      CCLOGERROR(ERRCLS_INT_PAR, ECC233, (ErrVal) 0,
                "CcMiLccCfgReq() Failed, invalid header");
      if (cfg == NULLP)
         ccGenCfm(pst, EVTLCCCFGCFM, cfg, LCM_PRIM_NOK, LCC_REASON_INVALID_HDRPARM);
      else
      {
         if (cfg->hdr.entId.ent != ccInit.ent)
         {
            ccGenCfm(pst, EVTLCCCFGCFM, cfg, LCM_PRIM_NOK, 
                     LCM_REASON_INVALID_ENTITY);
            RETVALUE(RFAILED);
         }
         if (cfg->hdr.entId.inst != ccInit.inst)
         {
            ccGenCfm(pst, EVTLCCCFGCFM, cfg, LCM_PRIM_NOK,
                     LCM_REASON_INVALID_INSTANCE);
            RETVALUE(RFAILED);
         }
         if (cfg->hdr.msgType != TCFG)
         {
            ccGenCfm(pst, EVTLCCCFGCFM, cfg, LCM_PRIM_NOK, 
                     LCM_REASON_INVALID_MSGTYPE);
            RETVALUE(RFAILED);
         }
      }
   }
#endif

   switch (cfg->hdr.elmId.elmnt)
   {
      case STGEN:    
         ret = ccGenCfg(pst, cfg);
#ifdef TELICA_MGI_MEGACO
         if (ret == ROK)
         {
             CcMngmt  myCfg;

             myCfg.t.cfg.s.ccSFSap.prior           = 0;
             myCfg.t.cfg.s.ccSFSap.route           = RTETRNQ;
             myCfg.t.cfg.s.ccSFSap.selector        = 0;
             myCfg.t.cfg.s.ccSFSap.mem.region      = 0;
             myCfg.t.cfg.s.ccSFSap.mem.pool        = 0;
             myCfg.t.cfg.s.ccSFSap.dstProcId       = 0;
             myCfg.t.cfg.s.ccSFSap.dstEnt          = ENTMGI;
             myCfg.t.cfg.s.ccSFSap.dstInst         = 0;
             myCfg.t.cfg.s.ccSFSap.spId            = 0;
             myCfg.t.cfg.s.ccSFSap.suId            = 0;
             myCfg.t.cfg.s.ccSFSap.sapType         = 0;
             myCfg.t.cfg.s.ccSFSap.tmr.tBNDCFM.enb = TRUE;
             myCfg.t.cfg.s.ccSFSap.tmr.tBNDCFM.val = 500;
             
             ret = ccSFSAPCfg(pst, &myCfg);

             DP ("CcMiLccCfgReq: ccGenCfg completes ROK --- SF Sap Cfg returns %d.\n",
                 ret);
         }
#endif /* TELICA_MGI_MEGACO */         
         /* BUG89777 clear GCC congestion/overload alarm for ccs suite restart */
         if (!CC_CONGESTION) 
         {
           ccGenAlarm(LCM_CATEGORY_RESOURCE, LCM_EVENT_GCC_CONG, LCM_CAUSE_GCC_CONG_CLR, NULL);
           ccGenAlarm(LCM_CATEGORY_RESOURCE, LCM_EVENT_OVERLOAD, LCM_CAUSE_OVERLOAD_CLR, NULL);
         }
         break;

      case STCCSDP:
         ret = ccSdpProfCfg(pst, cfg);
         break;

      case STCCBILLINGPRFL:
         ret = ccBillingPrflCfg(pst, cfg);
         break;

      case STCCPSSAP:
         ret = ccPSSAPCfg(pst, cfg);
         break;

      case STCCRMSAP:
         ret = ccRMSAPCfg(pst, cfg);
         break;

      case STCCRTSAP:
          gitDebugPrint (__FILE__, __LINE__, "RTSAP CFG got by cc. has to be removed \n");
          ccGenCfm(pst, EVTLCCCFGCFM, cfg, LCM_PRIM_OK, LCM_REASON_NOT_APPL);
          
         break;

      case STCCSFSAP:
         ret = ccSFSAPCfg(pst, cfg);
         break;

      case STCCINTFC:
         ret = ccINTFCSAPCfg(pst, cfg);
         break;

      case STCCPROF:
         ret = ccATMProfCfg(pst, cfg);
         break;

      case STCCVINTFC:        /* Virtual interface configuration */
         ret = ccVirIntfcCfg(pst, cfg);
         break;

      case STCCOBS:           /* Observation table configuration */
         ret = ccObsTableCfg(pst, cfg);
     break;

      case STCCSWTCFG:
         DP("Entering ccSwtCfg\n ");
         ret = ccSwtCfg(pst,cfg);    /* SWTCFG */
         break; 
      case STCCBILLINGSYS:            /*BILLIBG SYSTEM*/
     ret = ccBillingSys(pst,cfg);
         break;
      case STCCIVRSTATE:              /* IVRSTATE */
     ret = ccIvrState(pst,cfg);
         break;
      case STCCIVRANNCID:             /* IVR anncId */
          ret = ccIvrAnncIdCfg(pst,cfg);
          break;
      case STCCEXTANNCID:             /* IVR anncId */
          ret = ccExtAnncIdCfg(pst,cfg);
          break;
      case STCCSTST:                  /* STATSSTATE */
     ret = ccStatsState(pst,cfg);
         break;  

      case STCCPRTCLRELTBL:
         ret = ccPrtclRelCfg(pst, cfg);
         break;  

      case STCCFAILCNDTBL:
         ret = ccFailCndCfg(pst, cfg);
         break;  

      case STCCPARAMSUPPTBL:
         ret = ccParamSuppCfg(pst, cfg);
         break;  

      case STCCPARAMDFLTTBL:
         ret = ccParamDfltCfg(pst, cfg);
         break;  

      case STCCFRAUDPRFLTBL:
         ret = ccFraudPrflCfg(pst, cfg);
         break;

      case STCCSIPPRFL :
         ret = ccSipPrflCfg(pst, cfg);
         break;

      case STCCSIPTPRFL :
         ret = ccSipTPrflCfg(pst, cfg);
         break;

      case STCCPROTRELMAPTBL:
         ret = ccProtRelCfg (pst, cfg);
         break;

      case STCCRAPROTRELTBL:
         ret = ccRaProtRelCfg (pst, cfg);
         break;

      case ST_MGVARIANT :
         ret = ccMgVariantCfg(pst, cfg);
         break;

      case ST_MGOVERLOAD:
         ret = ccMgOverloadCfg(pst, cfg);
         break;
/*Bug:37702 -- Begin*/
      case STMGNAMECFG:
         ret = ccMgNameCfg(pst, cfg);
         break;
/*Bug:37702 -- End*/

      case STCCMGCMGAUDITCFG: /* Bug:45879 */
         ret = ccMgAuditCfg(pst, cfg);
         break;
/* + FID 14732 */
      case ST_REALMID:
         ret = ccRealmNameCfg(pst, cfg);
         break;

/* - FID 14732 */

      case STCCMGMAPADDR:
         ret = ccMgMapAddrCfg (pst, cfg);
         break;
      /* + FID15083.0 */
      case STCCCCCIPPRFL:
         ret = ccPrflCccIpCfg (pst, cfg);
         break;
      /* - FID15083.0 */
      /* FID15261.0 */
      case STCCMEGAABINDCNTRL:
         ret = ccMegaaBindCntrl(pst, cfg);
         break;

      /* FID 15169.0 */
      case STCCMGCPCCFG:
         ret = ccMgcPcCfg(pst, cfg);
          break;
      case STCCMCGCODE:
         ret = ccMcgCodeCfg(pst, cfg);
          break;
      case STCCHPCSYSCFG:
         ret = ccHpcSysCfg(pst, cfg);
          break;
          /* FID15737 + */
      case STCCLISYSCFG:
          ret = ccLiSysCfg(pst, cfg);
          break;
      case STCCMD:
          ret = ccMdPrflCfg(pst ,cfg);
          break;
      case STCCMDJRSDCTN:
          ret = ccJurisListCfg(pst, cfg);
          break;
          /* FID15737 -*/
      case STCCGRCNTRLCFG:  /* FID 14716.0 */
          DP("STCCGRCNTRLCFG\n");
          ccCp.grCntrl = cfg->t.cfg.s.ccGRCntrlCfg.grCntrl;
          ret=ROK;
          break;
      case STCCGETSDNCFG:
          ret = ccGetsDNCfg(pst, cfg);
          break;
      /* FID16584.0 */
      case STCCENUMDOMAIN:
          ret = ccEnumDomainCfg(pst, cfg);
          break;
      case STCCCCBSSYSCFG: /* FID: 14708.0 */
          ret = ccCcbsSysCfg(pst, cfg);
          break;
      case STCCMRFLISTCFG:              /* FID 17174.0 MRF-LIST */
          ret = ccMrfListCfg(pst, cfg);
          break;
      case STCCMRFANNCCFG:              /* FID 17174.0 MRF-ANNC */
          ret = ccMrfAnncTblCfg(pst, cfg);
          break;
      /* FID 17174.0 */
      case STCCSIPTGPST:
          ret = ccSipTgpStateCfg(pst, cfg);
          break;

      default:
#if (ERRCLASS & ERRCLS_INT_PAR)
         CCLOGERROR(ERRCLS_INT_PAR, ECC234, (ErrVal) cfg->hdr.elmId.elmnt,
                    "CcMiLccCfgReq() Failed, invalid elmnt");
#endif
         ccGenCfm(pst, EVTLCCCFGCFM, cfg, LCM_PRIM_NOK, 
                  LCM_REASON_INVALID_ELMNT);
         RETVALUE(RFAILED);
   } /* end switch (cfg->hdr.elmId.elmnt) */

   if (ret == ROK)
      ccGenCfm(pst, EVTLCCCFGCFM, cfg, LCM_PRIM_OK, LCM_REASON_NOT_APPL);

#ifdef ZC
   zcUpdPeer();
#endif /* ZC */
   RETVALUE(ROK);
} /* end of CcMiLccCfgReq */



/*
*
*      Fun:   CcMiLccStaReq
*
*      Desc:  
*
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy1.c
*
*/
 
#ifdef ANSI
PUBLIC S16 CcMiLccStaReq
(
Pst    *pst,                    /* post */
CcMngmt *sta                    /* status */
)
#else
PUBLIC S16 CcMiLccStaReq(pst, sta)
Pst    *pst;                    /* post */
CcMngmt *sta;                   /* status */
#endif
{

   TRC3(CcMiLccStaReq)

   if (!ccInit.cfgDone)
   {
      ccGenCfm(pst, EVTLCCSTACFM, sta, LCM_PRIM_NOK,LCM_REASON_GENCFG_NOT_DONE);
      RETVALUE(RFAILED);
   }

   /* validate message parameters */
#if (ERRCLASS & ERRCLS_INT_PAR)
   if ((sta == NULLP) ||
       (sta->hdr.entId.ent != ccInit.ent)   ||   /* entity id */
       (sta->hdr.entId.inst != ccInit.inst) ||   /* instance id */
       (sta->hdr.msgType != TSSTA))              /* message type */
   {
      CCLOGERROR(ERRCLS_INT_PAR, ECC235, (ErrVal) 0,
                 "CcMiLccStaReq() Failed, invalid header");
#if 0
      if (sta == NULLP)
         ccGenCfm(pst, EVTLCCSTACFM, sta, LCM_PRIM_NOK, LCC_REASON_INVALID_HDRPARM);
#endif
      if (sta != NULLP)
      {
         if (sta->hdr.entId.ent != ccInit.ent)
            ccGenCfm(pst, EVTLCCSTACFM, sta, LCM_PRIM_NOK, 
                     LCM_REASON_INVALID_ENTITY);
         if (sta->hdr.entId.inst != ccInit.inst)
            ccGenCfm(pst, EVTLCCSTACFM, sta, LCM_PRIM_NOK,
                     LCM_REASON_INVALID_INSTANCE);
         if (sta->hdr.msgType != TSSTA)
            ccGenCfm(pst, EVTLCCSTACFM, sta, LCM_PRIM_NOK, 
                     LCM_REASON_INVALID_MSGTYPE);
      }
      RETVALUE(RFAILED);
   }
#endif

   (Void) SGetDateTime(&sta->t.ssta.dt);

   switch (sta->hdr.elmId.elmnt)
   {
      case STSID:
         ccGetSId(&sta->t.ssta.s.sysId);
         break;
      case STCCINTFC:
      case STCCVINTFC:
         ccGetIntfcSta(pst, sta);
         break;
      case STCCOBS:
         ccGetObsTblSta(pst, sta);
         break;
      case STPSSAP:
         ccGetPsSapSta(pst, sta);
         break;
      case STCCRMSAP:
         ccGetPsSapSta(pst, sta);
         break;
      case STCCRTSAP:
         ccGetPsSapSta(pst, sta);
         break;
      case STCCSFSAP:
         ccGetPsSapSta(pst, sta);
         break;
      case STCCFRAUDCALLINFO:
         ccRtrvFraudCallInfo(pst, sta);
         RETVALUE(ROK); 
      default:
#if (ERRCLASS & ERRCLS_INT_PAR)
         CCLOGERROR(ERRCLS_INT_PAR, ECC236, (ErrVal) sta->hdr.elmId.elmnt,
                    "CcMiLccStaReq() Failed, invalid elmnt");
#endif
         ccGenCfm(pst, EVTLCCSTACFM, sta, LCM_PRIM_NOK, 
                  LCM_REASON_INVALID_ELMNT);
         RETVALUE(RFAILED);
   }

   ccGenCfm(pst, EVTLCCSTACFM, sta, LCM_PRIM_OK, LCM_REASON_NOT_APPL);
   RETVALUE(ROK);
} /* end of CcMiLccStaReq */
 


/*
*
*      Fun:   CcMiLccStsReq
*
*      Desc:  
*
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy1.c
*
*/
 
#ifdef ANSI
PUBLIC S16 CcMiLccStsReq
(
Pst    *pst,                    /* post */
Action action,                  /* action indicator */ 
CcMngmt *sts                    /* statistics */
)
#else
PUBLIC S16 CcMiLccStsReq(pst, action, sts)
Pst    *pst;                    /* post */
Action action;                  /* action indicator */
CcMngmt *sts;                   /* statistics */
#endif
{
   S16 ret;
   /* TELICA - Bugzilla-4321 - Bukucu - 10.25.01 */
   CcPsSap *cb;
   ret = ROK;
   /* end of Bugzilla-4321 */

   TRC3(CcMiLccStsReq)

   if (!ccInit.cfgDone)
   {
      ccGenCfm(pst, EVTLCCSTSCFM, sts, LCM_PRIM_NOK,LCM_REASON_GENCFG_NOT_DONE);
      RETVALUE(RFAILED);
   }

   /* validate message parameters */
#if (ERRCLASS & ERRCLS_INT_PAR)
   if ((sts == NULLP) ||
       (sts->hdr.entId.ent != ccInit.ent)   ||
       (sts->hdr.entId.inst != ccInit.inst) ||
       (sts->hdr.msgType != TSTS)           ||
       /* TELICA - Bugzilla-4321 - Bukucu - 10.25.01 */
       (action < ZEROSTS) ||
       (action > ZERO_RC_REL)) /* Bug14864 */
       /* Bugzilla-4321 */
   {
      CCLOGERROR(ERRCLS_INT_PAR, ECC237, (ErrVal) 0,
                 "CcMiLccStsReq() Failed, invalid header");
#if 0
      if (sts == NULLP)
         ccGenCfm(pst, EVTLCCSTSCFM, sts, LCM_PRIM_NOK, LCC_REASON_INVALID_HDRPARM);
#endif
      if (sts != NULLP)
      {
         if (sts->hdr.entId.ent != ccInit.ent)
            ccGenCfm(pst, EVTLCCSTSCFM, sts, LCM_PRIM_NOK, 
                     LCM_REASON_INVALID_ENTITY);
         if (sts->hdr.entId.inst != ccInit.inst)
            ccGenCfm(pst, EVTLCCSTSCFM, sts, LCM_PRIM_NOK,
                     LCM_REASON_INVALID_INSTANCE);
         if (sts->hdr.msgType != TSTS)
            ccGenCfm(pst, EVTLCCSTSCFM, sts, LCM_PRIM_NOK, 
                     LCM_REASON_INVALID_MSGTYPE);
      }
      RETVALUE(RFAILED);
   }
#endif
 
   SGetDateTime(&sts->t.sts.dt);
 
   switch (sts->hdr.elmId.elmnt)
   {
      case STGEN:      /* General Statistics */
         /* TELICA - Bugzilla-4321 - Bukucu - 10.25.01 - new stats handling
          * let ZEROSTS and NOZEROSTS do whatever they do before,
          * and return all the statistics for the new RTRV_XXX actions.
          * grouping will be done in tsmc.
          * previously ccGenSts() was called here.
         */
         switch(action)
         {
            case RTRV_NA_STS:
            case RTRV_ALL_STS:
            case RTRV_TRMT_STS:
            case RTRV_RCV_STS:
            case RTRV_UPDTHRSHLD:
            case NOZEROSTS:
               ccRtrvSts(sts, &ccCp, STGEN);
               break;
            case INIT_UPDTHRSHLD:
               ccCp.sts.updThrshld = sts->t.sts.s.ccGenSts.updThrshld;
               break;
            case INIT_TRMT_STS:
               ccInitTrmtSts(&ccCp, STGEN);
               break;
            case INIT_RCV_STS:
               ccInitRcvSts(&ccCp, STGEN);
               break;
            case INIT_NA_STS:
               ccInitNASts(&ccCp, STGEN);
               break;
            case ZEROSTS: /* fall thru */
               ccRtrvSts(sts, &ccCp, STGEN);
            case INIT_ALL_STS:
               ccInitTrmtSts(&ccCp, STGEN);
               ccInitRcvSts(&ccCp, STGEN);
               ccInitNASts(&ccCp, STGEN);
               break;
         }
         /* Bugzilla-4321 */
         break;

      case STPSSAP:    /* Protocol Specific SAP statistics */
         /* TELICA - Bugzilla-4321 - Bukucu - 10.25.01 - new stats handling */
#if (ERRCLASS & ERRCLS_INT_PAR)
         if (sts->t.sts.s.ccPsSapSts.suId >= (S16)ccCp.genCfg.maxPsSAP)
         {
            CCLOGERROR(ERRCLS_INT_PAR, ECC428,
                (ErrVal) sts->t.sts.s.ccPsSapSts.suId,
                "ccPsSAPSts() Failed, Invalid STCCPSSAP id");
            ccGenCfm(pst, EVTLCCSTSCFM, sts, LCM_PRIM_NOK,LCM_REASON_EXCEED_CONF_VAL);
            RETVALUE(RFAILED);
         }
#endif
         cb = *(ccPsSapLstPtr + sts->t.sts.s.ccPsSapSts.suId);
         CCDP(AIN_DBG_LEVEL_0, "%s - sts->t.sts.s.ccPsSapSts.suId:%d\n",__FUNCTION__,
              sts->t.sts.s.ccPsSapSts.suId);

         if (cb == NULLP)
         {
            ccGenCfm(pst, EVTLCCSTSCFM, sts, LCM_PRIM_NOK, LCM_REASON_INVALID_SAP);
            RETVALUE(RFAILED);
         }
         /* let ZEROSTS and NOZEROSTS do whatever they do before,
          * and return all the statistics for the new RTRV_XXX actions.
          * grouping will be done in tsmc.
          * previously ccPsSAPSts() was called here.
         */
         switch(action)
         {
            case RTRV_NA_STS:
            case RTRV_ALL_STS:
            case RTRV_TRMT_STS:
            case RTRV_RCV_STS:
            case NOZEROSTS:
               ccRtrvSts(sts, cb, STPSSAP);
               break;
            case INIT_TRMT_STS:
               ccInitTrmtSts(cb, STPSSAP);
               break;
            case INIT_RCV_STS:
               ccInitRcvSts(cb, STPSSAP);
               break;
            case INIT_NA_STS:
               ccInitNASts(cb, STPSSAP);
               break;
            case ZEROSTS: /* fall thru */
               ccRtrvSts(sts, cb, STPSSAP);
            case INIT_ALL_STS:
               ccInitTrmtSts(cb, STPSSAP);
               ccInitRcvSts(cb, STPSSAP);
               ccInitNASts(cb, STPSSAP);
               break;
         }
         break;


      case STCCINTFC:    /* Interface Specific statistics */
      case STCCVINTFC:   /* Virtual Interface Specific statistics */
         ret = ccIntfcSts(pst, action, sts);
         break;                  

/* TELICA - Bugzilla-14864 - */
      case STGENCALLFAIL:      /* GCC Call Fail Statistics  Bug 14864*/
         switch(action)
         {
            case RTRV_RC_REL:
               ccRtrvGccRelSts(sts, &ccCp, STGENCALLFAIL);
               break;
            case INIT_RC_REL:
               ccInitGccRelSts(&ccCp, STGENCALLFAIL);
               break;
            case ZERO_RC_REL:
               ccRtrvGccRelSts(sts, &ccCp, STGENCALLFAIL);
               ccInitGccRelSts(&ccCp, STGEN);
               break;
         }
         break;

      case STPSSAPCALLFAIL:    /* Protocol Specific GCC call rel statistics */
#if (ERRCLASS & ERRCLS_INT_PAR)
         if (sts->t.sts.s.ccPsSapSts.suId >= (S16)ccCp.genCfg.maxPsSAP)
         {
            CCLOGERROR(ERRCLS_INT_PAR, ECC428,
                (ErrVal) sts->t.sts.s.ccPsSapSts.suId,
                "ccPsSAPSts() Failed, Invalid STCCPSSAP id");
            ccGenCfm(pst, EVTLCCSTSCFM, sts, LCM_PRIM_NOK,LCM_REASON_EXCEED_CONF_VAL);
            RETVALUE(RFAILED);
         }
#endif
         cb = *(ccPsSapLstPtr + sts->t.sts.s.ccPsSapSts.suId);

         if (cb == NULLP)
         {
            ccGenCfm(pst, EVTLCCSTSCFM, sts, LCM_PRIM_NOK, LCM_REASON_INVALID_SAP);
            RETVALUE(RFAILED);
         }
         switch(action)
         {
            case RTRV_RC_REL:
               ccRtrvGccRelSts(sts, cb, STPSSAPCALLFAIL);
               break;
            case INIT_RC_REL:
               ccInitGccRelSts(cb, STPSSAPCALLFAIL);
               break;
            case ZERO_RC_REL:
               ccRtrvGccRelSts(sts, cb, STPSSAPCALLFAIL);
               ccInitGccRelSts(cb, STPSSAPCALLFAIL);
               break;
         }
         break;
/* end  14864 */

      case STOVERLOAD: /* FID 7804 */
          /* Copy the Overload Statistics from GCC data struture into the sts */
          sts->t.sts.s.ccGenSts.ccOverloadSts = 
                                          ccCp.sts.ccOverloadSts;
          CCDP(AIN_DBG_LEVEL_5, "FID: 7804: %s\n", __FUNCTION__);
          break;
      case STMGSAP:/*MG related sts*/
          switch(action)
          {
            case RTRV_ALL_STS:
              ccRtrvMgRelatedSts(sts, STMGSAP);
              break;
            case INIT_ALL_STS:
              ccInitMgRelatedSts(sts, STMGSAP);
              break;
          }
          break;
      case STGETS: /* FID 14457.0 */
          switch(action)
          {
            case RTRV_ALL_STS:
              ccRtrvGetsRelatedSts(sts, &ccCp.getsSts, STGETS);
              break;
            case INIT_ALL_STS:
              ccInitGetsRelatedSts(&ccCp.getsSts, STGETS);
              break;
          }
          break;
      case STCCBS: /* FID 16970.0 */
          switch(action)
          {
            case RTRV_ALL_STS:
              ccRtrvCcbsRelatedSts(sts, &ccCp.ccbsSts, STCCBS);
              break;
            case INIT_ALL_STS:
              ccInitCcbsRelatedSts(&ccCp.ccbsSts, STCCBS);
              break;
          }
          break;
      default:
#if (ERRCLASS & ERRCLS_INT_PAR)
         CCLOGERROR(ERRCLS_INT_PAR, ECC238, (ErrVal) sts->hdr.elmId.elmnt,
                    "CcMiLccStsReq() Failed, invalid elmnt");
#endif
         ccGenCfm(pst, EVTLCCSTSCFM, sts, LCM_PRIM_NOK, 
                  LCM_REASON_INVALID_ELMNT);
         RETVALUE(RFAILED);
   }

   if (ret == ROK)
      ccGenCfm(pst, EVTLCCSTSCFM, sts, LCM_PRIM_OK, LCM_REASON_NOT_APPL);
   /* bug 80565 */
   else
      ccGenCfm(pst, EVTLCCSTSCFM, sts, LCM_PRIM_NOK, LCM_REASON_NOT_APPL);
   /* bug 80565 */
   RETVALUE(ROK);
} /* end of CcMiLccStsReq */


/*
*
*      Fun:   CcMiLccCntrlReq
*
*      Desc:  This function is used by the Layer Management to
*             control asynchronously Call Control functions.
*
*      Ret:   ROK     - successful,
*
*      Notes: None.
*
*      File:  cc_bdy1.c
*
*/

#ifdef ANSI
PUBLIC S16 CcMiLccCntrlReq
(
Pst *pst,                    /* post */
CcMngmt *cntrl               /* control */
)
#else
PUBLIC S16 CcMiLccCntrlReq(pst, cntrl)
Pst *pst;                    /* post */
CcMngmt *cntrl;              /* control */
#endif
{
   S16 ret = ROK;
#ifdef ZC
   ZcUpdParam upd;
#endif

   TRC3(CcMiLccCntrlReq)

#ifndef TESTING
#ifdef ZC
   if (zcCb.protState != ACTIVE)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      CCLOGERROR(ERRCLS_DEBUG, ECCXXX, (ErrVal) zcCb.protState,
                 "CcMiLccCntrlReq() failed, CC is not active\n");
#endif
      ccGenCfm(pst, EVTLCCCNTRLCFM, cntrl, LCM_PRIM_NOK,
                  LCM_CAUSE_PROT_NOT_ACTIVE);
      RETVALUE(RFAILED);

   }
#endif /* ZC */
#endif /* TESTING */


   if (!ccInit.cfgDone)
   {
      ccGenCfm(pst, EVTLCCCNTRLCFM, cntrl, LCM_PRIM_NOK,
               LCM_REASON_GENCFG_NOT_DONE);
      RETVALUE(RFAILED);
   }
 
#if (ERRCLASS & ERRCLS_INT_PAR)
   if ((cntrl == NULLP) ||
       (cntrl->hdr.entId.ent != ccInit.ent)   ||   /* entity id */
       (cntrl->hdr.entId.inst != ccInit.inst) ||   /* instance id */
       (cntrl->hdr.msgType != TCNTRL))             /* message type */
   {
      CCLOGERROR(ERRCLS_INT_PAR, ECC239, (ErrVal) 0,
         "CcMiLccCntrlReq() Failed, invalid header");

#if 0
      if (cntrl == NULLP)
         ccGenCfm(pst, EVTLCCSTSCFM, cntrl, LCM_PRIM_NOK, LCC_REASON_INVALID_HDRPARM);
#endif
      if (cntrl != NULLP)
      {
         if (cntrl->hdr.entId.ent != ccInit.ent)
            ccGenCfm(pst, EVTLCCSTSCFM, cntrl, LCM_PRIM_NOK, 
                     LCM_REASON_INVALID_ENTITY);
         if (cntrl->hdr.entId.inst != ccInit.inst)
            ccGenCfm(pst, EVTLCCSTSCFM, cntrl, LCM_PRIM_NOK,
                     LCM_REASON_INVALID_INSTANCE);
         if (cntrl->hdr.msgType != TCNTRL)
            ccGenCfm(pst, EVTLCCSTSCFM, cntrl, LCM_PRIM_NOK, 
                     LCM_REASON_INVALID_MSGTYPE);
      }
      RETVALUE(RFAILED);
   }
#endif
 
   switch(cntrl->t.cntrl.type)
   {
      case SAUSTA:
         /* unsolicited status */
         switch (cntrl->t.cntrl.action)
         {
            case AENA:        /* enable */
               ccInit.usta = TRUE;
#ifdef ZC
               /* UPD INIT_CB */
               upd.suId = 0;
               zcRunTimeUpd(ZC_INIT_CB, CMPFTHA_UPD_REQ, upd);
#endif
               ret = ROK;
               break;
            case ADISIMM:     /* disable - immediately */
               ccInit.usta = FALSE;
#ifdef ZC
               /* UPD INIT_CB */
               upd.suId = 0;
               zcRunTimeUpd(ZC_INIT_CB, CMPFTHA_UPD_REQ, upd);
#endif
               ret = ROK;
               break;
#if (ERRCLASS & ERRCLS_INT_PAR)
            default:
               CCLOGERROR(ERRCLS_INT_PAR, ECC240,
                          (ErrVal) cntrl->t.cntrl.action,
                          "CcMiLccCntrlReq() Failed, invalid subaction");
               ccGenCfm(pst, EVTLCCCNTRLCFM, cntrl, LCM_PRIM_NOK,
                        LCM_REASON_INVALID_ACTION);
               RETVALUE(RFAILED);
#endif
         }
         break;

      case SAELMNT:     /* Specified element */
         switch(cntrl->hdr.elmId.elmnt)
         {
            case STCCPSSAP:
               ret = ccPSSapCntrl(pst, cntrl);
               break;

            case STCCRMSAP:
               ret = ccRMSapCntrl(pst, cntrl);
               break;

            case STCCSFSAP:
               ret = ccSFSapCntrl(pst, cntrl);
               break;

            case STCCPROF:
               ret = ccATMProfCntrl(pst, cntrl);
               break;

            case STCCBILLINGPRFL:
               ret = ccBillingPrflCntrl(pst, cntrl);
               break;

            case STCCINTFC:
            case STCCVINTFC:
               ret = ccIntfcCntrl(pst, cntrl);
               break;

            case STCCTSTCALL:
               switch (cntrl->t.cntrl.action)
               {
                 case AENA:
                    ret = ccInitiateTstCall(pst, cntrl);
                    if (ret == RFAILED)
                    {
                       ccGenCfm(pst, EVTLCCCNTRLCFM, cntrl, LCM_PRIM_NOK,
                                LCM_REASON_INVALID_ACTION);
                       RETVALUE(RFAILED);
                    }
                    break;
                 default:
                    ccGenCfm(pst, EVTLCCCNTRLCFM, cntrl, LCM_PRIM_NOK,
                             LCM_REASON_INVALID_ACTION);
                    RETVALUE(RFAILED);
               }
               break;

            case STGEN:
               switch (cntrl->t.cntrl.action)
               {
                  case ASHUTDOWN:
                    ccShutDown();
                    break;
                 default:
                    ccGenCfm(pst, EVTLCCCNTRLCFM, cntrl, LCM_PRIM_NOK,
                             LCM_REASON_INVALID_ACTION);
                    RETVALUE(RFAILED);
               }
               break;
           
            case STCCPRTCLRELTBL:
               ret = ccPrtclRelCntrl(pst, cntrl);
               break;  

         }
         break;

      case SAGR_DSTPROCID:     /* Specified group elements */
         switch(cntrl->hdr.elmId.elmnt)
         {
            case STCCGRPSSAP:
               ret = ccGrpPSSapCntrl(pst, cntrl);
               break;

            case STCCGRRMSAP:
               ret = ccGrpRMSapCntrl(pst, cntrl);
               break;

            case STCCGRSFSAP:
               ret = ccGrpSFSapCntrl(pst, cntrl);
               break;
            case STCCGRALL:
               ret = ccGrpPSSapCntrl(pst, cntrl);
               if (ret != ROK)
               {
                  break;
               }
               ret = ccGrpRMSapCntrl(pst, cntrl);
               if (ret != ROK)
               {
                  break;
               }
               ret = ccGrpSFSapCntrl(pst, cntrl);
         }
         break;

      case LCC_STKSTRT:
         ret = ccStartStack (pst, cntrl);
         break;

      case LCC_GEO_SLCCS_AUDIT_START:
         ret = ccGeoStartAudit(pst, cntrl);
         break;

      case LCC_GEO_SLCCS_AUDIT_ABORT:
         ret = ccGeoAbortAudit(pst, cntrl);
         break;

      case LCC_GEO_SLCCS_AUDIT_NOAUDIT:
         ret = ccGeoNoAudit(pst, cntrl);
         break;

      case LCC_GEO_SLCCS_AUDIT_DONE:
         ret = ccGeoAuditDone(pst, cntrl);
         break;
      /* FID 15261 Add the let GCC to prepare the Audit */
      case LCC_GEO_SLCCS_AUDIT_PREP:
         ret = ccGeoStartAudit(pst, cntrl);
         break;

      /* BUG# 81486 Add new event to process the Audit_Done after CM failover */
      case LCC_GEO_SLCCS_AUDIT_DONE_FAILOVER:
         ret = ccGeoAuditDoneFailover(pst, cntrl);
         break;

#ifdef TESTING
      case LCC_DISPCONS:
         ret = ccDispCons(pst, cntrl);
         break;
#endif /* TESTING */

      case LCC_CLEARCONN:
         ret = ccLMRelCon(pst, cntrl);
         break;

      case SADBG:       /* Debug */
#ifdef DEBUGP
         switch (cntrl->t.cntrl.action)
         {
            case AENA:
               ccInit.dbgMask |= cntrl->t.cntrl.c.dbgMask;
#ifdef ZC
               /* UPD INIT_CB */
               upd.suId = 0;
               zcRunTimeUpd(ZC_INIT_CB, CMPFTHA_UPD_REQ, upd);
#endif
               ret = ROK;
               break;
            case ADISIMM:
               ccInit.dbgMask &= ~(cntrl->t.cntrl.c.dbgMask);
#ifdef ZC
               /* UPD INIT_CB */
               upd.suId = 0;
               zcRunTimeUpd(ZC_INIT_CB, CMPFTHA_UPD_REQ, upd);
#endif
               ret = ROK;
               break;
#if 1 /* TELICA - Bugzilla-5520- Khan/Yakin - 08/01/2001 - code trace */
            case ACODETRC:
           if(cntrl->t.cntrl.c.dbgMask == 0){
            gitDebugPrint(__FILE__, __LINE__, "layer=cc level=%s\n",LearnLevelName(ccCodeTrcLvl));
           }
           else{
                    ccCodeTrcLvl = cntrl->t.cntrl.c.dbgMask; 
            gitDebugPrint(__FILE__, __LINE__, "layer=cc level=%s\n",LearnLevelName(ccCodeTrcLvl));
           }        
               break;
#endif /* Bugzilla-5520 */
#if (ERRCLASS & ERRCLS_INT_PAR)
            default:
               CCLOGERROR(ERRCLS_INT_PAR, ECC241,
                          (ErrVal) cntrl->t.cntrl.action,
                          "CcMiLccCntrlReq() Failed, invalid action for debug");
               ccGenCfm(pst, EVTLCCCNTRLCFM, cntrl, LCM_PRIM_NOK,
                        LCM_REASON_INVALID_ACTION);
               RETVALUE(RFAILED);
#endif
         }
#else
#if (ERRCLASS & ERRCLS_INT_PAR)
         CCLOGERROR(ERRCLS_INT_PAR, ECC242,
                    (ErrVal) cntrl->t.cntrl.type,
                    "CcMiLccCntrlReq() Failed, debug is not enabled");
#endif
         ccGenCfm(pst, EVTLCCCNTRLCFM, cntrl, LCM_PRIM_NOK,
                  LCM_REASON_INVALID_SUBACTION);
#endif /* DEBUGP */
         break;

      case SATRC:
         switch (cntrl->hdr.elmId.elmnt)
         {
            case STCCOBS:
               switch (cntrl->t.cntrl.action)
               {
                  case AENA:
                     ccInit.trc = TRUE; 
#ifdef ZC
               /* UPD INIT_CB */
               upd.suId = 0;
               zcRunTimeUpd(ZC_INIT_CB, CMPFTHA_UPD_REQ, upd);
#endif
                     ret = ROK;
                     break;
                  case ADISIMM:
                     ccInit.trc = FALSE; 
#ifdef ZC
               /* UPD INIT_CB */
               upd.suId = 0;
               zcRunTimeUpd(ZC_INIT_CB, CMPFTHA_UPD_REQ, upd);
#endif
                     ret = ROK;
                     break;
               }
               break;
         }
         break;
      case SAACNT:
         switch (cntrl->hdr.elmId.elmnt)
         {
            case STCCCDR:
               ret = ccAcntCntrl(pst, cntrl);
               break;
         }
         break;
      /* Bug: 14442 tyakin */
      case SASTS:
          ccUpdPeerSts();
          break;
      /* end Bug: 14442 */

      default:
#if (ERRCLASS & ERRCLS_INT_PAR)
         CCLOGERROR(ERRCLS_INT_PAR, ECC243, 
                    (ErrVal) cntrl->t.cntrl.type,
                    "CcMiLccCntrlReq() Failed, invalid control type");
#endif
         ccGenCfm(pst, EVTLCCCNTRLCFM, cntrl, LCM_PRIM_NOK, 
                  LCM_REASON_INVALID_SUBACTION);
         RETVALUE(RFAILED);
   }

   if (ret == ROK)
      ccGenCfm(pst, EVTLCCCNTRLCFM, cntrl, LCM_PRIM_OK, LCM_REASON_NOT_APPL);
   else
      ccGenCfm(pst, EVTLCCCNTRLCFM, cntrl, LCM_PRIM_NOK, LCM_REASON_NOT_APPL);
#ifdef ZC
   zcUpdPeer();
#endif /* ZC */
   RETVALUE(ROK);
} /* end of CcMiLccCntrlReq */


/*
*
*      Fun:   CcMiLccUnCfgReq
*
*      Desc:  This function is used by the Layer Management to
*             delete some Call Control tables.
*
*      Ret:   ROK     - successful,
*
*      Notes: None.
*
*      File:  cc_bdy1.c
*
*/

#ifdef ANSI
PUBLIC S16 CcMiLccUnCfgReq
(
Pst *pst,                    /* post */
CcMngmt *unCfg
)
#else
PUBLIC S16 CcMiLccUnCfgReq(pst, unCfg)
Pst *pst;                    /* post */
CcMngmt *unCfg;
#endif
{
   S16 ret = ROK;

   TRC3(CcMiLccUnCfgReq)

   if (!ccInit.cfgDone)
   {
      ccGenCfm(pst, EVTLCCUNCFGCFM, unCfg, LCM_PRIM_NOK,
               LCM_REASON_GENCFG_NOT_DONE);
      RETVALUE(RFAILED);
   }
 
#if (ERRCLASS & ERRCLS_INT_PAR)
   if ((unCfg == NULLP) ||
       (unCfg->hdr.entId.ent != ccInit.ent)   ||   /* entity id */
       (unCfg->hdr.entId.inst != ccInit.inst) ||   /* instance id */
       (unCfg->hdr.msgType != TUCFG))             /* message type */
   {
      DP("CcMiLccUnCfgReq() Failed, invalid header\n");

#if 0
      if (unCfg == NULLP)
         ccGenCfm(pst, EVTLCCUNCFGCFM, unCfg, LCM_PRIM_NOK,
             LCC_REASON_INVALID_HDRPARM);
#endif
      if (unCfg != NULLP)
      {
         DP(" ent - %d/%d, inst - %d/%d, msgType - %d/%d\n",
            unCfg->hdr.entId.ent, ccInit.ent, unCfg->hdr.entId.inst,
            ccInit.inst, unCfg->hdr.msgType, TUCFG);
         if (unCfg->hdr.entId.ent != ccInit.ent)
            ccGenCfm(pst, EVTLCCUNCFGCFM, unCfg, LCM_PRIM_NOK, 
                     LCM_REASON_INVALID_ENTITY);
         if (unCfg->hdr.entId.inst != ccInit.inst)
            ccGenCfm(pst, EVTLCCUNCFGCFM, unCfg, LCM_PRIM_NOK,
                     LCM_REASON_INVALID_INSTANCE);
         if (unCfg->hdr.msgType != TUCFG)
            ccGenCfm(pst, EVTLCCUNCFGCFM, unCfg, LCM_PRIM_NOK, 
                     LCM_REASON_INVALID_MSGTYPE);
      }
      RETVALUE(RFAILED);
   }
#endif

   switch (unCfg->hdr.elmId.elmnt)
   {
      case STCCFAILCNDTBL:
         ret = ccFailCndUnCfg(pst, unCfg);
         break;  

      case STCCPARAMSUPPTBL:
         ret = ccParamSuppUnCfg(pst, unCfg);
         break;  

      case STCCPARAMDFLTTBL:
         ret = ccParamDfltUnCfg(pst, unCfg);
         break;  

      case STCCFRAUDPRFLTBL:
         ret = ccFraudPrflUnCfg(pst, unCfg);
         break; 

      case STCCSIPPRFL :
         ret = ccSipPrflUnCfg(pst, unCfg);
         break; 

      case STCCSIPTPRFL :/* BUG: 51157 */
         ret = ccSipTPrflUnCfg(pst, unCfg);
         break;

      case STCCPROTRELMAPTBL:
         ret = ccProtRelUnCfg (pst, unCfg);
         break;

      case STCCRAPROTRELTBL:
         ret = ccRaProtRelUnCfg (pst, unCfg);
         break;

/* + FID 14732 */
      case ST_REALMID:
         ret = ccRealmNameUnCfg (pst, unCfg);
         break;
/* - FID 14732 */
      case STCCMGMAPADDR:
         ret = ccMgMapAddrUnCfg (pst, unCfg);
         break;
         /* FID 15261.0 + */
      case STCCMGCMGAUDITCFG:
         ret = ccMgAuditUnCfg (pst, unCfg);
         break;
      /* FID 15261.0 - */
      /* FID15169.0 */
      case STCCMGCPCCFG:
         ret = ccMgcPcUnCfg(pst, unCfg);
         break;

      case STCCCCCIPPRFL:
         ret = ccPrflCccIpUnCfg(pst, unCfg);
         break;
      case STCCMCGCODE: /* FID 16169.0 */
         ret = ccMcgCodeUnCfg(pst, unCfg);
         break;
         /* FID15737.0 + */
      case STCCMD:
         ret = ccPrflMdUnCfg(pst, unCfg);
         break;
      case STCCMDJRSDCTN:
         ret = ccJurisListUnCfg(pst, unCfg);
         break;
         /* FID15737.0 - */
      case STCCGETSDNCFG:
         ret = ccGetsDNUnCfg(pst, unCfg);
         break;
         /* FID16584.0 */
      case STCCENUMDOMAIN:
         ret = ccEnumDomainUnCfg(pst, unCfg);
         break;
      case STCCMRFLISTCFG:         /* FID17174.0 MRF-LIST */
         ret= ccMrfListCntrl(pst, unCfg);
         break;
      case STCCMRFANNCCFG:         /* FID17174.0 MRF-ANNC */
         ret = ccMrfAnncTblCntrl(pst, unCfg);
         break;
         
      default :
         DP("CcMiLccUnCfgReq() Failed, invalid element id %d",
             unCfg->hdr.elmId.elmnt);
         ccGenCfm(pst, EVTLCCUNCFGCFM, unCfg, LCM_PRIM_NOK, 
                  LCM_REASON_NOT_APPL);
         RETVALUE(RFAILED);
   }

   if (ret == ROK)
      ccGenCfm(pst, EVTLCCUNCFGCFM, unCfg, LCM_PRIM_OK, LCM_REASON_NOT_APPL);
   else
      ccGenCfm(pst, EVTLCCUNCFGCFM, unCfg, LCM_PRIM_NOK, LCM_REASON_NOT_APPL);

   RETVALUE(ROK);
} /* CcMiLccUnCfgReq */
 


/*
*     Chapter 2: interface functions to PSIF Service Provider
*/


/*
*
*      Fun:   CcLiCctAudCfm
*
*      Desc:  Confirmation from PSIF of the final audit request
*
*      Ret:   ROK     - successful
*
*      Notes: None.
*
*      File:  cc_bdy1.c
*
*/
#ifdef ANSI
PUBLIC S16 CcLiCctAudCfm
(
Pst *pst,                      /* post */
SuId suId,                     /* Service User SAP ID */
CcAudCfmEvnt *ccAudCfmEvent    /* Audit Confirm Event */
)
#else
PUBLIC S16 CcLiCctAudCfm(pst, suId, ccAudCfmEvent)
Pst *pst;                      /* post */
SuId suId;                     /* Service User SAP ID */
CcAudCfmEvnt *ccAudCfmEvent;   /* Audit Confirm Event */
#endif
{
   CcPsSap *pCb;

   TRC3(CcLiCctAudCfm)

#if (ERRCLASS & ERRCLS_INT_PAR)
   if ((ccInit.cfgDone != TRUE) || (suId >= (SuId) ccCp.genCfg.maxPsSAP))
   {
      CCLOGERROR(ERRCLS_INT_PAR, ECC244, (ErrVal) suId,
                 "CcLiCctAudCfm() Failed, invalid suId");
      RETVALUE(RFAILED);
   }
#endif

   if ((ccPsSapLstPtr == NULLP) || ((pCb = *(ccPsSapLstPtr + suId)) == NULLP))
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      CCLOGERROR(ERRCLS_INT_PAR, ECC245, (ErrVal) suId,
                 "CcLiCctAudCfm() Failed, invalid suId");
#endif
      ccGenAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT,
                 LCM_CAUSE_INV_SAP, (U8*) &suId);
      RETVALUE(RFAILED);
   }

   DP("CcLiCctAudCfm:  received status %s, from suId=%d\n",
       ((ccAudCfmEvent->status == CCT_AUD_OK)?"OK":"NAK"),
       suId);
/* todo - handle status */
   if (ccAudCfmEvent->status == CCT_AUD_OK)
   {
   }
   else
   {
   }

   RETVALUE(ROK);
}  /* end of CcLiCctAudCfm */


/*
*
*      Fun:   CcLiCctBndCfm
*
*      Desc:  Confirmation from PSIF of the Bind Request Issued
*
*      Ret:   ROK     - successful
*
*      Notes: None.
*
*      File:  cc_bdy1.c
*
*/
 
#ifdef ANSI
PUBLIC S16 CcLiCctBndCfm
(
Pst *pst,                      /* post */
SuId suId,                     /* Service User SAP ID */
U8   status                    /* Bind Status */
)
#else
PUBLIC S16 CcLiCctBndCfm(pst, suId, status)
Pst *pst;                      /* post */
SuId suId;                     /* Service User SAP ID */
U8   status;                   /* Bind Status */
#endif
{
   CcPsSap *pCb;
#ifdef ZC
   ZcUpdParam upd;
#endif /* ZC */

   TRC3(CcLiCctBndCfm)

#if (ERRCLASS & ERRCLS_INT_PAR)
   if ((ccInit.cfgDone != TRUE) || (suId >= (SuId) ccCp.genCfg.maxPsSAP))
   {
      CCLOGERROR(ERRCLS_INT_PAR, ECC244, (ErrVal) suId,
                 "CcLiCctBndCfm() Failed, invalid suId");
      RETVALUE(RFAILED);
   }
#endif

   if ((ccPsSapLstPtr == NULLP) || ((pCb = *(ccPsSapLstPtr + suId)) == NULLP))
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      CCLOGERROR(ERRCLS_INT_PAR, ECC245, (ErrVal) suId,
                 "CcLiCctBndCfm() Failed, invalid suId");
#endif
      ccGenAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT,
                 LCM_CAUSE_INV_SAP, (U8*) &suId);
      RETVALUE(RFAILED);
   }

   ccStopSapTmr((PTR)pCb, TMR_PSSAPBNDCFM);

   /* if waiting for Bind Confirm and Bind Successful */
   if ((pCb->state == CC_BND_CONT) && (status == CCT_BND_OK))
   {
      pCb->bndRetryCnt = 0;
      pCb->state = CC_BND;
#ifdef ZC
      /* UPD PSSAP */
      upd.pssap = pCb;
      zcRunTimeUpd(ZC_PSSAP_CB, CMPFTHA_UPD_REQ, upd);
#endif

          /* bug:26595 - need to send tgConfigInfo to CSMGC - aka ENTCS.
             If this BndCfm came from ENTCS, store this ccSap's pst for future easy use.*/
      if (pst->srcEnt == ENTCS)
      {
          ccCsmgcPstP = &pCb->pst ;
          DP("%s - initialized ccCsmgcPstP:0x%lx\n", __FUNCTION__, (U32)ccCsmgcPstP) ;
      }
   } /* Successful Bind Confirm */
   ccStoreSapPtrs(pCb);
   DP("%s: pst->srcEnt:%d, pst->dstEnt:%d, pCb->pst.srcEnt:%d,"
      " pCb->pst.dstEnt:%d, suId:%d, state:%d, status:%d\n",
      __FUNCTION__, pst->srcEnt, pst->dstEnt, pCb->pst.srcEnt,
      pCb->pst.dstEnt, suId, status, pCb->state);
   
   if (status == CCT_BND_NOK)
   {
      ccGenAlarm(LCM_CATEGORY_PROTOCOL, LCM_EVENT_BND_FAIL, 
                 LCM_CAUSE_UNKNOWN, NULLP);
      pCb->state = CC_UNBND;
   }
   
#ifdef ZC
   zcUpdPeer();
#endif /* ZC */
   RETVALUE(ROK);

} /* end of CcLiCctBndCfm */

/*
*
*      Fun:   CcLiCctConInd
*
*      Desc:  Process an incoming connection establishment
*             request.
*
*      Ret:   ROK     - successful,
*
*      Notes: None.
*
*      File:  cc_bdy1.c
*
*/

#ifdef ANSI
PUBLIC S16 CcLiCctConInd
(
Pst       *pst,               /* post */
SuId      suId,               /* service user identier */
UConnId   spConnId,           /* service provider instance identifier */
RmRsc     *rsc,               /* Incoming Resource  */
ProtType  protType,           /* incoming protocol type */
CcConEvnt *ccConEvnt,         /* connection event structure */
CcProfile *ccProfile,         /* connection profile structure */
Buffer    *uBuf               /* unrecognized parameters */
)
#else
PUBLIC S16 CcLiCctConInd(pst, suId, spConnId, rsc, protType, ccConEvnt, 
                         ccProfile, uBuf)
Pst       *pst;               /* post */
SuId      suId;               /* service user identier */
UConnId   spConnId;           /* service provider instance identifier */
RmRsc     *rsc;               /* Incoming Resource  */
ProtType  protType;           /* incoming protocol type */
CcConEvnt *ccConEvnt;         /* connection event structure */
CcProfile *ccProfile;         /* connection profile structure */
Buffer    *uBuf;              /* unrecognized parameters */
#endif
{
    CcPsSap *pCb;
    CcConCb *con;
    S16 ret;
    PTR dupEntry;             /* For checking duplicate item */
#ifdef ZC
    ZcUpdParam upd;
#endif
    const char *fn = __FUNCTION__ ;
    U32  logMask = 0; /* FID 17167.0 */

    TRC3( CcLiCctConInd)

    /* FID 17167.0 */
    CC_GET_LOGMASK(logMask, ccConEvnt, protType); 

#ifdef ZC
    /* 26928 */
    if (zcCb.protState != ACTIVE)
    {
        CCLOGERR("Ignoring %s in standby state.  protType=%d,"
            " spConnId=0x%08lX\n", __FUNCTION__, protType, spConnId);
        ccFreeSdpInTheEvnt(__FILE__, __LINE__, CCE_CONIND, 0, (PTR)ccConEvnt,
            protType);
        ccFreeIntBiccMBufInTheEvnt(__FILE__, __LINE__, CCE_CONIND, 0, (PTR)ccConEvnt,
            protType);
        ccFreeSipSipTMBufInTheEvnt(__FILE__, __LINE__, CCE_CONIND, 0, (PTR)ccConEvnt,
            protType); /* FID 14732 */
        ccFreeHistInfoEntriesInTheEvnt(__FILE__, __LINE__, CCE_CONIND, 0, (PTR)ccConEvnt,
            protType); /*Bug 89424 */
        RETVALUE(RFAILED);
    }
#endif /* ZC */

#if 1 /* TELICA - Bugzilla-5520- Khan/Yakin - 08/01/2001 - code trace */
        TELICACODETRC(LOGDEBUG, ccCodeTrcLvl, ccInit.lmPst, ("CcLiCctConInd::spConnId=%lx, protType=%d", spConnId, protType));
#endif /* Bugzilla-5520 */

#if (ERRCLASS & ERRCLS_INT_PAR)
    if ((ccInit.cfgDone != TRUE) || (suId >= (SuId) ccCp.genCfg.maxPsSAP))
    {
        CCLOGERROR(ERRCLS_INT_PAR, ECC246, (ErrVal) suId,
                   "CcLiCctConInd() Failed, invalid suId");
        if (uBuf != NULLP)
            ccDropUBuf(&uBuf);
        ccFreeSdpInTheEvnt(__FILE__, __LINE__, CCE_CONIND, 0, (PTR)ccConEvnt,
            protType);
        ccFreeIntBiccMBufInTheEvnt(__FILE__, __LINE__, CCE_CONIND, 0, (PTR)ccConEvnt,
            protType);
        ccFreeSipSipTMBufInTheEvnt(__FILE__, __LINE__, CCE_CONIND, 0, (PTR)ccConEvnt,
            protType); /* FID 14732 */
        ccFreeHistInfoEntriesInTheEvnt(__FILE__, __LINE__, CCE_CONIND, 0, (PTR)ccConEvnt,
            protType); /*Bug 89424 */
        RETVALUE(RFAILED);
    }
#endif

    if ((ccPsSapLstPtr == NULLP) || ((pCb = *(ccPsSapLstPtr + suId)) == NULLP))
    {
#if (ERRCLASS & ERRCLS_INT_PAR)
        CCLOGERROR(ERRCLS_INT_PAR, ECC247, (ErrVal) suId,
                   "CcLiCctConInd() Failed, invalid suId");
#endif

#if 1 /* TELICA - Bugzilla-5520- Khan/Yakin - 08/01/2001 - code trace */
        TELICACODETRC(LOGERROR, ccCodeTrcLvl, ccInit.lmPst, ("CcLiCctConInd::Invalid suId, spConnId=%lx, suId=%d", spConnId, suId));
#endif /* Bugzilla-5520 */

        ccGenAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT,
                   LCM_CAUSE_INV_SAP, (U8*) &suId);
        if (uBuf != NULLP)
            ccDropUBuf(&uBuf);
        ccFreeSdpInTheEvnt(__FILE__, __LINE__, CCE_CONIND, 0, (PTR)ccConEvnt,
            protType);
        ccFreeIntBiccMBufInTheEvnt(__FILE__, __LINE__, CCE_CONIND, 0, (PTR)ccConEvnt,
            protType);
        ccFreeSipSipTMBufInTheEvnt(__FILE__, __LINE__, CCE_CONIND, 0, (PTR)ccConEvnt,
            protType); /* FID 14732 */
        ccFreeHistInfoEntriesInTheEvnt(__FILE__, __LINE__, CCE_CONIND, 0, (PTR)ccConEvnt,
            protType); /*Bug 89424 */
        RETVALUE(RFAILED);
    }

    if (pCb->state != CC_BND)
    {
#if (ERRCLASS & ERRCLS_INT_PAR)
        CCLOGERROR(ERRCLS_INT_PAR, ECC248, (ErrVal) suId,
                   "CcLiCctConInd() Failed, SAP not bound");
#endif

#if 1 /* TELICA - Bugzilla-5520- Khan/Yakin - 08/01/2001 - code trace */
        TELICACODETRC(LOGERROR, ccCodeTrcLvl, ccInit.lmPst, ("CcLiCctConInd::spConnId=%lx, SAP not bound, pcbState=%d", spConnId, pCb->state));
#endif /* Bugzilla-5520 */

        ccGenAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_INV_STATE,
                   LCM_CAUSE_INV_SAP, (U8*) &suId);
        if (uBuf != NULLP)
            ccDropUBuf(&uBuf);
        ccFreeSdpInTheEvnt(__FILE__, __LINE__, CCE_CONIND, 0, (PTR)ccConEvnt,
            protType);
        ccFreeIntBiccMBufInTheEvnt(__FILE__, __LINE__, CCE_CONIND, 0, (PTR)ccConEvnt,
            protType);
        ccFreeSipSipTMBufInTheEvnt(__FILE__, __LINE__, CCE_CONIND, 0, (PTR)ccConEvnt,
            protType); /* FID 14732 */
        ccFreeHistInfoEntriesInTheEvnt(__FILE__, __LINE__, CCE_CONIND, 0, (PTR)ccConEvnt,
            protType); /*Bug 89424 */
        RETVALUE(RFAILED);
    }

    /* If we are in congestion, release the call */
    /* FID 14419.0 For incoming BICC, the congestion logic would be executed in 
     * ccConE00S00 just after extracting tunneled specialCall paramenter */
    if (CC_CONGESTION && ( CC_GET_PROTOCOL(protType) != CC_BICC))
    {
        U8   specialCall = 0; /* FID 14457.0 */
        /* FID 14457.2 changes */
        Bool relPrioCall = FALSE;
        Bool relNrmCall  = FALSE;

        U16        retVal = RFAILED;

         /* bug88508, there is a case that GCC is in congestion while SIPA is not.
            In the case, the GETS-DN is not matched. Need to match GETS-DN before
            release the call as normal call.
          */
        ccIsCallMarkedSpecialCall(protType, ccConEvnt, &specialCall);

        if((ccCp.getsEnabled) && (!specialCall) && 
              (CC_GET_PROTOCOL(protType) == CC_SIPT || 
               CC_GET_PROTOCOL(protType) == CC_SIP))
        {
           Addrs         cdpn;
           CmGetsDnEntry *getsDn = NULL;
           SiCdPtyNum    *cdPtyNum = NULLP;
           Bool          ret;
           int i;

           cmMemset((U8 *) &cdpn, 0, (Size)sizeof(Addrs));
           if(CC_GET_PROTOCOL(protType) == CC_SIP)
           {
              cdPtyNum = &(ccConEvnt->m.sipConEvnt.cdPtyNum);
           }
           else
           {
              cdPtyNum = &(ccConEvnt->m.siConEvnt.cdPtyNum);
           }

           ret = ccConvertSICdpn(&cdpn, cdPtyNum);

           if(ret != TRUE)
           {
              CCDP(AIN_DBG_LEVEL_2,"DN converting failed, must be invalid!\n");
              RETVALUE(RFAILED);
           }

           CCDP(AIN_DBG_LEVEL_2, "DN length %d \n", cdpn.length);
           for(i=0;i<cdpn.length;i++)
              CCDP(AIN_DBG_LEVEL_2,"DN str val %d \n", cdpn.strg[i]);
           retVal = cmGetsDnFind(&ccGetsDnTbl, &cdpn, &getsDn);

           if ((retVal == ROK) && (getsDn != NULL))
           {
              DP("find GETS call.\n");
              specialCall |= SPECIAL_CALL_GETS;
              if(CC_GET_PROTOCOL(protType) == CC_SIP)
              {
                 ccConEvnt->m.sipConEvnt.callPriority.pres = PRSNT_NODEF;
                 ccConEvnt->m.sipConEvnt.callPriority.val |= SPECIAL_CALL_GETS;
              }
              else
              {
                 ccConEvnt->m.siConEvnt.optElmnt.sipt.callPriority.pres = PRSNT_NODEF;
                 ccConEvnt->m.siConEvnt.optElmnt.sipt.callPriority.val |= SPECIAL_CALL_GETS;
              }
           }
        }

        /* FID 14419.0 check the call priority and peg the dropped call under overload */
        if (CC_CONGESTION_LEVEL4)
        {
          if (specialCall)
          {
            if (ccCp.getsEnabled && (specialCall & SPECIAL_CALL_GETS)) /* FID 14457.0 */
            {
               ccCp.getsSts.nsepLimitExceeded++;
               ccCp.sts.totalGetsRejOvldRx++;
            }
            else
              ccCp.sts.totalPrioRejOvldRx++;

            ccCongCb.ccDqExtCngCb.prioCallsDroppedSoFar++; /* FID 14457.2 */
            relPrioCall = TRUE;
          }
          else
          {
            ccCp.sts.totalNormRejOvldRx++;

            ccCongCb.ccDqExtCngCb.nrmCallsDroppedSoFar++; /* FID 14457.2 */
            relNrmCall = TRUE;
          }
          ccCongCb.ccDqExtCngCb.currBlockCnt++;
          ccCongCb.ccDqExtCngCb.currAttemptCnt++;
        }
        else
        {
          /* We come here for Level1, Level2, Level3 */
          if (!specialCall)
          {
            if ((!ccCongCb.ccDqExtCngCb.currBlockCnt && !ccCongCb.ccDqExtCngCb.currAttemptCnt) ||
                (((float)(ccCongCb.ccDqExtCngCb.currBlockCnt * 100)/ccCongCb.ccDqExtCngCb.currAttemptCnt) \
                <= ccDmndQCong_V2.callReleasePct[GET_GCC_ACTN_CONG_LEVEL]))
            {
              /* Release Normal Calls */
              ccCp.sts.totalNormRejOvldRx++;
              ccCongCb.ccDqExtCngCb.nrmCallsDroppedSoFar++; /* FID 14457.2 */

              ccCongCb.ccDqExtCngCb.currBlockCnt++;
              relNrmCall = TRUE;
            }
            ccCongCb.ccDqExtCngCb.currAttemptCnt++;
          }
        }

        /* FID 14457.2 */
        if (relPrioCall || relNrmCall)
        {
          ccSendRelease(NULLP, pCb, spConnId, 0, protType, CCSWTCHCONG, NULL);
          if (uBuf != NULLP)
              ccDropUBuf(&uBuf);

          /* FID 14457.2 */
          if (relPrioCall && !(ccCongCb.ccDqExtCngCb.prioCallsDroppedSoFar%100))
          {
            DP("%s():  In GCC CongLevel %d, released %ld PRIORITY calls so far\n",
               __FUNCTION__, GET_GCC_ACTN_CONG_LEVEL, 
               ccCongCb.ccDqExtCngCb.prioCallsDroppedSoFar);
          }
          else if (relNrmCall && !(ccCongCb.ccDqExtCngCb.nrmCallsDroppedSoFar%1000))
          {
            DP("%s():  In GCC CongLevel %d, released %ld NORMAL calls so far\n",
               __FUNCTION__, GET_GCC_ACTN_CONG_LEVEL, 
               ccCongCb.ccDqExtCngCb.nrmCallsDroppedSoFar);
          }

          ccFreeSdpInTheEvnt(__FILE__, __LINE__, CCE_CONIND, 0, (PTR)ccConEvnt,
              protType);
          ccFreeIntBiccMBufInTheEvnt(__FILE__, __LINE__, CCE_CONIND, 0, (PTR)ccConEvnt,
              protType);
          ccFreeSipSipTMBufInTheEvnt(__FILE__, __LINE__, CCE_CONIND, 0, (PTR)ccConEvnt,
              protType); /* FID 14732 */
          ccFreeHistInfoEntriesInTheEvnt(__FILE__, __LINE__, CCE_CONIND, 0, (PTR)ccConEvnt,
              protType); /*Bug 89424 */
          return(ROK);
        }
    }

    /* Allocate a connection block */
    if ((ret = ccInitCon(&con)) != ROK)
    {
#if (ERRCLASS & ERRCLS_ADD_RES)
        CCLOGERROR(ERRCLS_ADD_RES, ECC249, (ErrVal) ret,
                   "CcLiCctConInd() Failed, ccInitCon failed");
#endif

#if 1 /* TELICA - Bugzilla-5520- Khan/Yakin - 08/01/2001 - code trace */
        TELICACODETRC(LOGERROR, ccCodeTrcLvl, ccInit.lmPst, ("CcLiCctConInd::spConnId=%lx, ccInitCon failed", spConnId));
#endif /* Bugzilla-5520 */

        ccSendRelease(NULLP, pCb, spConnId, 0, protType, CCTMPFAIL, NULL);
        if (uBuf != NULLP)
            ccDropUBuf(&uBuf);
        ccFreeSdpInTheEvnt(__FILE__, __LINE__, CCE_CONIND, 0, (PTR)ccConEvnt,
            protType);
        ccFreeIntBiccMBufInTheEvnt(__FILE__, __LINE__, CCE_CONIND, 0, (PTR)ccConEvnt,
            protType);
        ccFreeSipSipTMBufInTheEvnt(__FILE__, __LINE__, CCE_CONIND, 0, (PTR)ccConEvnt,
            protType); /* FID 14732 */
        ccFreeHistInfoEntriesInTheEvnt(__FILE__, __LINE__, CCE_CONIND, 0, (PTR)ccConEvnt,
            protType); /*Bug 89424 */
        RETVALUE(ROK);
    }

#if 0 /* Fake SipMBuf */
   ccFakeAllocSipMBuf(__FILE__, __LINE__, CCE_CONIND, 0, (PTR)ccConEvnt, protType);
#endif
    con->logMask = logMask;
    con->icSuConnId   = ccGetNewSuConnId(); 
    con->origIcSuConnId = con->icSuConnId;

    con->ogSuConnId   = ccGetNewSuConnId();
    con->origOgSuConnId = con->ogSuConnId;

    con->icSpConnId = spConnId;
    con->icProtType = protType;
    con->icSapCb    = pCb;
    con->icSapCb->nmbActvConn++;
    con->uBuf       = uBuf;
    uBuf            = NULL; /* Bug 44218 */
    con->direction  = INCTOOUT;

    /* FID14485 the sip call-id GCC generated is within 32(CC_SIP_ID_LENGTH_OLD) bytes presently,
     * if the call-id GCC generated is longer than 32  bytes,should change to CC_SIP_ID_LENGTH(128)
     * or more
     */
    ret = ccGetNewVarSipCallId(&(con->callDtlInfo.un.icSipId),
          CC_SIP_ID_LENGTH,con->icSuConnId,FALSE);
    if(ret != ROK)
    {
       CCDP(AIN_DBG_LEVEL_0,"Error in getting new sip call-id!!!");
    }
#if 0 /* 55576: Moved to ccCleanUpOGForReRoute */
    ret = ccGetNewSipCallId (con->callDtlInfo.un.ogSipId, CC_SIP_ID_LENGTH,
                             con->ogSuConnId);
    if(ret == ROK) con->callDtlInfo.un.ogSipIdPres = PRSNT_NODEF;
#endif


    /* Bug#15674, rkhan, Lets fill up the incoming side here, Outgoing will be filled
     * up in cc state m/c */
    ccUpdSts(con->icSapCb, NULLP);

    /* FID16647.0 Mexico ISUP use FCI N Bit to indicate whether NPDB is queried or not.
       Here we will take the following actions: 
       1. Copy the the original transCallNInd to siConEvnt.optElmnt.isup(or bicc per ic intfType)
       2. Move the N bit to M bit and reuse the existing logic of processing M bit
       The M N bit will be restored when processing map (in function ccMapS02M00,ccSIP2SIMapS21M00)   
    */ 
    if (CC_IS_PROT_TYPE_MEXICO(con->icProtType) &&
        ((rsc->intfc.intfType == CC_SI_INTFC) || (rsc->intfc.intfType == CC_BICC_INTFC)))
    {
        if (ccConEvnt->m.siConEvnt.fwdCallInd.transCallNInd.pres == PRSNT_NODEF)
        {    
            if (rsc->intfc.intfType == CC_SI_INTFC) 
            {
                memcpy(&ccConEvnt->m.siConEvnt.optElmnt.isup.origTransCallNInd,
                       &ccConEvnt->m.siConEvnt.fwdCallInd.transCallNInd, 
                       sizeof(TknU8));
                CCDP(AIN_DBG_LEVEL_2, "%s:optElmnt.isup: transCallNInd.pres:%d,transCallNInd.val:%d\n",
                     __FUNCTION__,
                     ccConEvnt->m.siConEvnt.optElmnt.isup.origTransCallNInd.pres,
                     ccConEvnt->m.siConEvnt.optElmnt.isup.origTransCallNInd.val);
            }
            else if (rsc->intfc.intfType == CC_BICC_INTFC)
            {    
                memcpy(&ccConEvnt->m.siConEvnt.optElmnt.bicc.origTransCallNInd,
                       &ccConEvnt->m.siConEvnt.fwdCallInd.transCallNInd, 
                       sizeof(TknU8));
                CCDP(AIN_DBG_LEVEL_2, "%s:optElmnt.bicc: transCallNInd.pres:%d,transCallNInd.val:%d\n",
                     __FUNCTION__,
                     ccConEvnt->m.siConEvnt.optElmnt.bicc.origTransCallNInd.pres,
                     ccConEvnt->m.siConEvnt.optElmnt.bicc.origTransCallNInd.val);
            }
     
        } 
    
        ccConEvnt->m.siConEvnt.fwdCallInd.transCallNInd.val = CC_GET_FCI_BIT_N(ccConEvnt->m.siConEvnt.fwdCallInd.transCallNInd);
        CCDP(AIN_DBG_LEVEL_2, 
             "%s:Mexico ISUP move Bit N to Bit M: transCallNInd.pres:%d,transCallNInd.val:%d\n",
             __FUNCTION__,
             ccConEvnt->m.siConEvnt.fwdCallInd.transCallNInd.pres,
             ccConEvnt->m.siConEvnt.fwdCallInd.transCallNInd.val);
    }
    
    /* If this is ISUP or BICC, look up the trunk group before getting interface */
    if ((rsc->intfc.intfType == CC_SI_INTFC) || 
#if 0
        (rsc->intfc.intfType == CC_BICC_INTFC)) /* Not for internal BICC */
#endif
        (rsc->intfc.intfType == CC_EXT_BICC_INTFC))
    {
        if (rmGetNbTrkgrp(rsc) != ROK)
        {
            rsc->intfc.t.tgrpDesc.tgrpId = 0;
        }
#if 1 /* Bugzilla 3723:taraveti */
        /* Update the lnp status in the connection control block */
        if (ccConEvnt->m.siConEvnt.fwdCallInd.transCallNInd.pres == PRSNT_NODEF)
        {
            if (ccConEvnt->m.siConEvnt.fwdCallInd.transCallNInd.val == TRUE)
            {
                con->lnp_status = LNP_TRANS_SOMEWHERE;
            }
        }
#endif /* Bugzilla 3723:taraveti */
    }

    if ((ret = ccAssignSaps(con, &rsc->intfc)) != ROK)
    {
        SPutSBuf(ccInit.region, ccInit.pool, con->routeInfo,
                 (Size)sizeof(CcRouteInfo));

        TELICA_FREE_CONNECTION (con);

#if 1 /* TELICA - Bugzilla-5520- Khan/Yakin - 08/01/2001 - code trace */
        TELICACODETRC(LOGERROR, ccCodeTrcLvl, ccInit.lmPst, ("CcLiCctConInd::spConnId=%lx, ccAssignSaps failed", spConnId));
#endif /* Bugzilla-5520 */

        ccSendRelease(NULLP, pCb, spConnId, 0, protType, CCTMPFAIL, NULL);
        if (uBuf != NULLP)
            ccDropUBuf(&uBuf);
        ccFreeSdpInTheEvnt(__FILE__, __LINE__, CCE_CONIND, 0, (PTR)ccConEvnt,
            protType);
        ccFreeIntBiccMBufInTheEvnt(__FILE__, __LINE__, CCE_CONIND, 0, (PTR)ccConEvnt,
            protType);
        ccFreeSipSipTMBufInTheEvnt(__FILE__, __LINE__, CCE_CONIND, 0, (PTR)ccConEvnt,
            protType); /* FID 14732 */
        ccFreeHistInfoEntriesInTheEvnt(__FILE__, __LINE__, CCE_CONIND, 0, (PTR)ccConEvnt,
            protType); /*Bug 89424 */
        RETVALUE(RFAILED);
    }

    /* Increment the call attempt statistics */
    if (con->icIntfcCb) 
       con->icIntfcCb->sts.numIcCallAttempt++;

    cmMemcpy((U8 *) &con->icRsc, (U8 *)rsc, sizeof(RmRsc));
          /* bug:46451 */
       if (con->icRsc.intfc.intfType == CC_SIP_INTFC)
           CCDP(AIN_DBG_LEVEL_0, "%s - icRsc-intfcType:SIP, sipDesc.ipAddr:%s, "
                "sipDesc.tgId:0x%x\n", fn, ip_ntoa(&con->icRsc.intfc.t.sipDesc.ipAddr),
                (int)con->icRsc.intfc.t.sipDesc.tgrpId) ;

    if (ccProfile != NULLP)
        cmMemcpy((U8 *) con->icProfile, (U8 *) ccProfile, sizeof(CcProfile));   


    /* CAS has a null conEvnt */
    if(ccConEvnt != NULLP)  
    {
        if(protType == CC_SIP)
        {
            ccStoreEvnt(CCE_CONIND, 0, con, (U8 *) ccConEvnt, sizeof(SipConEvnt));
            /*
             * Netherlands ISUP: Initialize con->isCRGwithTariffGot and con->isInternationalCall.
             * These two fields only used for SIP to ISUP call without encapsulation.
             */
             con->isInternationalCall = FALSE;
             con->isCRGwithTariffGot = FALSE;
             /*End of Feature 1959*/
        }
        else
        {
            ccStoreEvnt(CCE_CONIND, 0, con, (U8 *) ccConEvnt, sizeof(CcConEvnt));
            /* Feature 1839 - compatibility: save the original data used for compatibility into con->siPduUPV */
            if(CC_IS_COMPAT_SUPPORTED(con->icIntfcCb,con->icProtType))
            {
                SiConEvnt  *pSiConEvnt;    /* points to the ISUP con ind */
                SiPduUPV   *pConUPV;       /* points to the ISUP con->siPduUPV */

                pSiConEvnt = NULLP;
                pConUPV = NULLP;

                pSiConEvnt = &con->ccEvntHold->m.ccConEvnt.m.siConEvnt;
                pConUPV = &con->siPduUPV;

                if(pSiConEvnt != NULLP && pConUPV != NULLP)
                {
#if 1 /* BUG: 79022 */
                    pConUPV->pres = PRSNT_NODEF;
#endif
                    cmMemcpy((U8*)(&pConUPV->fwdCallInd), (U8*)(&pSiConEvnt->fwdCallInd), sizeof(SiFwdCallInd));
                    cmMemcpy((U8*)(&pConUPV->cgPtyCat), (U8*)(&pSiConEvnt->cgPtyCat), sizeof(SiCgPtyCat));
                    cmMemcpy((U8*)(&pConUPV->cgPtyNum), (U8*)(&pSiConEvnt->cgPtyNum), sizeof(SiCgPtyNum));
                    cmMemcpy((U8*)(&pConUPV->opFwdCalInd), (U8*)(&pSiConEvnt->opFwdCalInd), sizeof(SiOpFwdCalInd));
                    cmMemcpy((U8*)(&pConUPV->origCdNum), (U8*)(&pSiConEvnt->origCdNum), sizeof(SiOrigCdNum));
                    cmMemcpy((U8*)(&pConUPV->redirgNum), (U8*)(&pSiConEvnt->redirgNum), sizeof(SiRedirNum));
                    cmMemcpy((U8*)(&pConUPV->redirInfo), (U8*)(&pSiConEvnt->redirInfo), sizeof(SiRedirInfo));
                    /*Feature 1959*/
                    cmMemcpy((U8*)(&pConUPV->usr2UsrInd), (U8*)(&pSiConEvnt->usr2UsrInd), sizeof(SiUsr2UsrInd));
                    cmMemcpy((U8*)(&pConUPV->cgPtyNum1), (U8*)(&pSiConEvnt->cgPtyNum1), sizeof(SiCgPtyNum));
                    /*End of feature 1959*/
#if 1 /* BUG: 62591 */
                    cmMemcpy((U8*)(&pConUPV->genNmb[0]),
                             (U8*)(&pSiConEvnt->genNmb[0]),
                             sizeof(SiGenNum) * MF_MAX_GNUM);
#endif
                }
            }
            /* end of compatibility */
	   
 	    /*
             * Initialize the con->waitForMoreInfo which lets GCC know if need to wait
             * for potential COT/INF/SAM.
             */
            if((CC_GET_PROTOCOL(con->icProtType) == CC_SI) ||
               (CC_GET_PROTOCOL(con->icProtType) == CC_EXT_BICC))
            {
                CC_SET_WAITMORE_INFO(con->ccEvntHold->m.ccConEvnt.m.siConEvnt.optElmnt.isup.waitForMoreInfo);
                if(CC_IS_WAITMORE_INFO(con->waitForMoreInfo))
                {
                    CCDP(AIN_DBG_LEVEL_0, "Need to await more INFO from ISUP/BICC, con->waitForMoreInfo=0x%02x\n", con->waitForMoreInfo);
                }
            }

	    /* 2111 --- ISDN overlap receiving */
            if(CC_GET_PROTOCOL(con->icProtType) == CC_IN)
            {
                CC_SET_IN_WAITMORE_INFO(con->ccEvntHold->m.ccConEvnt.m.inConEvnt.optElmt.waitForMoreInfo);
                /* Kavon */
                CCDP(AIN_DBG_LEVEL_0, "con->waitForMoreInfo=0x%02x\n", con->waitForMoreInfo);

                if(con->waitForMoreInfo & 0x02 )
                {
                    CCDP(AIN_DBG_LEVEL_0, "Need more INFO from ISDN\n");

                    if (con->waitForMoreInfo & 0x04)
                    {
                        CCDP(AIN_DBG_LEVEL_0, "Need to play dialing tone\n");
                    }
                }
            }
            /* end of 2111 */

            /*
             * Netherlands ISUP: Initialize con->isCOLPRequested and con->connNum;
             * For ISUP to SIP call without encapsulation, con->connNum store PAID received in latest 18x.
             */
            if( CC_IS_PROT_TYPE_KPNV2(con->icProtType) || CC_IS_PROT_TYPE_KPNNL2(con->icProtType))
            {
               con->isCOLPRequested = FALSE;
               con->connNum.eh.pres = NOTPRSNT;
            }
            /*End of Feature 1959*/
        }
    }
    
    /* Bug 89744 */
    if(con->ccEvntHold)
    {
        ccSetHistInfoEntriesFree(CCE_CONIND, 0, (PTR)ccConEvnt, protType);
    }

    switch (con->icRsc.intfc.intfType)
    {
    case CC_CS_LN_INTFC:
    case CC_CS_TG_INTFC:
    case CC_G3_INTFC:
    case CC_MGCP_LN_INTFC:
        /* IGNORE: no called number available yet */
        break;

    default:
        ccBillUpdCdPtyNum (__FILE__, __LINE__, con);
        break;
    }
        
    /* Insert Connection CB in suConnId Hash Table */
    /* Mask the least significant bit before inserting in the connection */
    /* Hash table as the outgoing and incoming connIds differ in LSB     */
    /* Find the icSuConnId in the ccSuOgInstTbl */
    ret = cmHashListFind(&ccSuOgInstTbl, (U8 *)&con->icSuConnId,
                         (U16)sizeof(UConnId), 0, &dupEntry);

    if (ret != ROK)
        ret = cmHashListInsert(&ccSuInstTbl, (PTR)con, (U8 *)&con->icSuConnId,
                               (U16)sizeof(UConnId));

    if (ret != ROK)
    {
        ccDropEvnt(con);
        SPutSBuf(ccInit.region, ccInit.pool, con->routeInfo,
                 (Size)sizeof(CcRouteInfo));
#if (ERRCLASS & ERRCLS_DEBUG)
        CCLOGERROR(ERRCLS_DEBUG, ECC251, (ErrVal) ret,
                   "CcLiCctConInd() Failed, cmHashListInsert failed in suInstTbl");
#endif

#if 1 /* TELICA - Bugzilla-5520- Khan/Yakin - 08/01/2001 - code trace */
        TELICACODETRC(LOGERROR, ccCodeTrcLvl, ccInit.lmPst, ("CcLiCctConInd::spConnId=%lx, icSuConnId=%lx, cmHashListInsert failed in suInstTbl", spConnId, con->icSuConnId));
#endif /* Bugzilla-5520 */

        ccGenAlarm(LCM_CATEGORY_INTERNAL, LCC_EVENT_HASHING_FAILED, 
                   LCC_CAUSE_SUINSTTBL_INS, (U8 *) &con->icSuConnId);
        ccSendRelease(NULLP, pCb, spConnId, 0, protType, CCTMPFAIL, NULL);
        TELICA_FREE_CONNECTION (con); /* Bug 78160 */
        if (uBuf != NULLP)
            ccDropUBuf(&uBuf);
        ccFreeSdpInTheEvnt(__FILE__, __LINE__, CCE_CONIND, 0, (PTR)ccConEvnt,
            protType);
        ccFreeIntBiccMBufInTheEvnt(__FILE__, __LINE__, CCE_CONIND, 0, (PTR)ccConEvnt,
            protType);
        ccFreeSipSipTMBufInTheEvnt(__FILE__, __LINE__, CCE_CONIND, 0, (PTR)ccConEvnt,
            protType); /* FID 14732 */
        ccFreeHistInfoEntriesInTheEvnt(__FILE__, __LINE__, CCE_CONIND, 0, (PTR)ccConEvnt,
            protType); /*Bug 89424 */
        RETVALUE(RFAILED);
    }

    /* Find the ogSuConnId in the ccSuInstTbl */
    ret = cmHashListFind(&ccSuInstTbl, (U8 *)&con->ogSuConnId,
                         (U16)sizeof(UConnId), 0, &dupEntry);
    if (ret != ROK)
        ret = cmHashListInsert(&ccSuOgInstTbl, (PTR)con, (U8 *)&con->ogSuConnId,
                               (U16)sizeof(UConnId));
    if (ret != ROK)
    {
        ccDropEvnt(con);
        SPutSBuf(ccInit.region, ccInit.pool, con->routeInfo,
                 (Size)sizeof(CcRouteInfo));
#if (ERRCLASS & ERRCLS_DEBUG)
        CCLOGERROR(ERRCLS_DEBUG, ECC251, (ErrVal) ret,
                   "CcLiCctConInd() Failed, cmHashListInsert failed in suOgInstTbl");
#endif

#if 1 /* TELICA - Bugzilla-5520- Khan/Yakin - 08/01/2001 - code trace */
        TELICACODETRC(LOGERROR, ccCodeTrcLvl, ccInit.lmPst, ("CcLiCctConInd::spConnId=%lx, ogSuConnId=%lx, "
                      "cmHashListInsert failed in suOgInstTbl", spConnId, con->ogSuConnId));
#endif /* Bugzilla-5520 */

        ccGenAlarm(LCM_CATEGORY_INTERNAL, LCC_EVENT_HASHING_FAILED,
                   LCC_CAUSE_SUINSTTBL_INS, (U8 *) &con->ogSuConnId);
        cmHashListDelete(&ccSuInstTbl, (PTR)con);
        ccSendRelease(NULLP, pCb, spConnId, 0, protType, CCTMPFAIL, NULL);
        TELICA_FREE_CONNECTION (con); /* Bug 78160 */
        if (uBuf != NULLP)
            ccDropUBuf(&uBuf);
        ccFreeSdpInTheEvnt(__FILE__, __LINE__, CCE_CONIND, 0, (PTR)ccConEvnt,
            protType);
        ccFreeIntBiccMBufInTheEvnt(__FILE__, __LINE__, CCE_CONIND, 0, (PTR)ccConEvnt,
            protType);
        ccFreeSipSipTMBufInTheEvnt(__FILE__, __LINE__, CCE_CONIND, 0, (PTR)ccConEvnt,
            protType); /* FID 14732 */
        ccFreeHistInfoEntriesInTheEvnt(__FILE__, __LINE__, CCE_CONIND, 0, (PTR)ccConEvnt,
            protType); /*Bug 89424 */
        RETVALUE(RFAILED);
    }

    /* Insert Connection CB in spInstId Hash Table */
    ret = cmHashListInsert(&pCb->spInstTbl, (PTR)con, (U8 *)&con->icSpConnId,
                           (U16)sizeof(UConnId));
    if (ret != ROK)
    {
#if 1 /* cc028.15 : addition */
        /* In case of spInstID insertion failure, it is possible to have
         * a control block already existing with that spInstId in case of
         * CRM followed by IAM in case of ISUP ANSI 92 or in case of forced
         * switchover of ISUP , in either case if we find a connection block
         * existing, we should remove the connection control block and delete
         * all resources without waiting for a response from other entities and
         * proceed with the current call */
        CcConCb *oldcon;
#if 1 /* TELICA - Bugzilla-5520- Khan/Yakin - 08/01/2001 - code trace */
        TELICACODETRC(LOGDEBUG, ccCodeTrcLvl, ccInit.lmPst, ("CcLiCctConInd::spConnId=%lx, Old  conn pres", spConnId));
#endif /* Bugzilla-5520 */

        ret = cmHashListFind (&pCb->spInstTbl, (U8 *)&spConnId, 
                              (U16)sizeof(UConnId), (U16)0, (PTR *) &oldcon);
        if (ret != ROK)
        {

#if 1 /* TELICA - Bugzilla-5520- Khan/Yakin - 08/01/2001 - code trace */
            TELICACODETRC(LOGERROR, ccCodeTrcLvl, ccInit.lmPst, ("CcLiCctConInd::spConnId=%lx, Blk unavail", spConnId));
#endif /* Bugzilla-5520 */

            /* fail the call as no other block with spInstId is available */
#if (ERRCLASS & ERRCLS_DEBUG)
            CCLOGERROR(ERRCLS_DEBUG, ECC252, (ErrVal) ret,
                       "CcLiCctConInd() Failed, cmHashListInsert failed in spInstTbl");
#endif
            ccGenAlarm(LCM_CATEGORY_INTERNAL, LCC_EVENT_HASHING_FAILED, 
                       LCC_CAUSE_SPINSTTBL_INS, (U8 *) &con->icSpConnId);
            cmHashListDelete(&ccSuInstTbl, (PTR)con);

            cmHashListDelete(&ccSuOgInstTbl, (PTR)con);

            ccDropEvnt(con);

            SPutSBuf(ccInit.region, ccInit.pool, con->icProfile,
                     (Size)(sizeof(CcProfile)));
            SPutSBuf(ccInit.region, ccInit.pool, con->ogProfile,
                     (Size)(sizeof(CcProfile)));

            SPutSBuf(ccInit.region, ccInit.pool, con->routeInfo,
                     (Size)sizeof(CcRouteInfo));

            TELICA_FREE_CONNECTION (con);

            ccSendRelease(NULLP, pCb, spConnId, 0, protType, CCTMPFAIL, NULL);
            if (uBuf != NULLP)
                ccDropUBuf(&uBuf);
            ccFreeSdpInTheEvnt(__FILE__, __LINE__, CCE_CONIND, 0,
                (PTR)ccConEvnt, protType);
            ccFreeIntBiccMBufInTheEvnt(__FILE__, __LINE__, CCE_CONIND, 0,
                (PTR)ccConEvnt, protType);
            ccFreeSipSipTMBufInTheEvnt(__FILE__, __LINE__, CCE_CONIND, 0, 
                (PTR)ccConEvnt, protType); /* FID 14732 */
            ccFreeHistInfoEntriesInTheEvnt(__FILE__, __LINE__, CCE_CONIND, 0,
                (PTR)ccConEvnt, protType); /*Bug 89424 */
            RETVALUE(RFAILED);
        }
        else /* another connection found using this icSpConnId */
        {
#if 0 /* bug 34874:taraveti */
            State oldState = oldcon->state;
#endif

            /* Check if the incoming protocol uses static spConnIds */
            if (((oldcon->icProtType == CC_CS_LN) || 
                 (oldcon->icProtType == CC_CS_TG) || 
                 (oldcon->icProtType == CC_MGCP_LN) || 
                 (oldcon->icProtType == CC_GR303)) && 
                (oldcon->icRscVal == FALSE) && (oldcon->ogRscVal == TRUE))
            {
                /* This is not an error.  These protocols just shouldn't be using static spConnIds */
                CCDP(AIN_DBG_LEVEL_2,"Warning:  duplicate icSp:  "
                     "icPt:%d, ogPt:%d, icSp:0x%08lX, icSu:0x%08lX, ogSp:0x%08lX, ogSu:0x%08lX\n",
                     oldcon->icProtType, oldcon->ogProtType,
                     oldcon->icSpConnId, oldcon->icSuConnId, 
                     oldcon->ogSpConnId, oldcon->ogSuConnId);
            }
#if 1 /*Bug: 37697-bcouto-06/10/2005*/
            /* Check if the old connection was responsible for Continuity Check of a segmented IAM */
            else if (oldcon->ccrIAMflg)
            {
                CCDP(AIN_DBG_LEVEL_2,"Warning:  duplicate icSp:  "
                     "icPt:%d, ogPt:%d, icSp:0x%08lX, icSu:0x%08lX, ogSp:0x%08lX, ogSu:0x%08lX\n",
                     oldcon->icProtType, oldcon->ogProtType,
                     oldcon->icSpConnId, oldcon->icSuConnId, 
                     oldcon->ogSpConnId, oldcon->ogSuConnId);
            }
#endif
            else
            {
                CCLOGERR("Error:  duplicate icSp:  "
                     "icPt:%d, ogPt:%d, icSp:0x%08lX, icSu:0x%08lX, ogSp:0x%08lX, ogSu:0x%08lX\n",
                     oldcon->icProtType, oldcon->ogProtType,
                     oldcon->icSpConnId, oldcon->icSuConnId, 
                     oldcon->ogSpConnId, oldcon->ogSuConnId);
                
                CC_TRACE_DUMP(oldcon);
            }

            if ((oldcon->icRscVal == FALSE) && (oldcon->ogRscVal == FALSE))
            {
                CC_STATE_CHANGE(oldcon, CCS_IDLE);
                ccRelCon(oldcon);
            }
            else if ((oldcon->icRscVal == FALSE) && (oldcon->ogRscVal == TRUE))
            {
                /* make the icSpConnId of the oldcon invalid so that, when the oldcon
                 * is released, it would't harm this newConnection */
                cmHashListDelete(&pCb->spInstTbl, (PTR)oldcon);
                oldcon->icSapCb->nmbActvConn--;
                oldcon->icSpConnId = 0;
                oldcon->icSapCb = NULLP;
            }
            else if ((oldcon->icRscVal == TRUE) && (oldcon->ogRscVal == FALSE))
            {
                /* Deallocate incoming resource and delete call */
                CC_STATE_CHANGE(oldcon, CCS_IDLE);
                ccDeallocateResource(oldcon, CC_INCOMING);
                oldcon->icRscVal = FALSE;
                ccRelCon(oldcon);
            }
            else /* ((oldcon->icRscVal == TRUE) && (oldcon->ogRscVal == TRUE)) */
            {
#if 0 /* bug 34874:taraveti */
                U8 tmrNum;
                Bool relTmr = FALSE;
#endif

                /* Deallocate incoming resource */
                CC_STATE_CHANGE(oldcon, CCS_IDLE);
                ccDeallocateResource(oldcon, CC_INCOMING);
                oldcon->icRscVal = FALSE;
                
                /* make the icSpConnId of the oldcon invalid so that, when 
                 * the oldcon is released, it would't harm this newConnection */
                cmHashListDelete(&pCb->spInstTbl, (PTR)oldcon);
                oldcon->icSapCb->nmbActvConn--;
                oldcon->icSpConnId = 0;
                oldcon->icSapCb = NULLP;

#if 0 /* bug 34874:taraveti */
                /* Check if release timer is running */
                for (tmrNum = 0; tmrNum < MAXCCCONCBTIMER; tmrNum++)
                {
                    if (oldcon->timers[tmrNum].tmrEvnt == TMR_RELEASE) 
                    {
                        relTmr = TRUE;
                        break;
                    }
                }

                if (relTmr == TRUE) /* releasing state */
                {
                    /* Change back to original state and let nature take its course */
                    CC_STATE_CHANGE(oldcon, oldState);
                }
                else  /* Setup or active state */
#endif
                {
                    /* blow off the mg ctx */
                    if (CC_IS_CONTEXT_PRESENT(oldcon))
                    {
                        cleanUpMgQ(oldcon);
                        ccCleanUpMGCtx (oldcon);
                    }

                    /* Release has not yet been sent to outgoing side.  Send one now */

#ifdef ZC
                    /* REL CON */
                    upd.icSuConnId = oldcon->icSuConnId;
                    zcRunTimeUpd(ZC_RELCON_CB, CMPFTHA_UPD_REQ, upd);
                    zcUpdPeer();
#endif
                    CC_STATE_CHANGE(oldcon, CCS_AWTRELCFM_OG);

                    ccStopConTmr(oldcon, TMR_SETUP);
                    ccStartConTmr(TMR_RELEASE, oldcon, (PTR)&ccCp.genCfg);
                    
                    ccSendRelease(oldcon,oldcon->ogSapCb, oldcon->ogSpConnId,
                                  oldcon->ogSuConnId, oldcon->ogProtType, CCTMPFAIL,NULL);
                }

            }
        }

        /* Insert Connection CB in spInstId Hash Table */
        ret = cmHashListInsert(&pCb->spInstTbl, (PTR)con,
            (U8 *)&con->icSpConnId, (U16)sizeof(UConnId));
        if (ret != ROK)
        {
#if (ERRCLASS & ERRCLS_DEBUG)
            CCLOGERROR(ERRCLS_DEBUG, ECC252, (ErrVal) ret,
                       "CcLiCctConInd() Failed, cmHashListInsert failed in spInstTbl");
#endif

#if 1 /* TELICA - Bugzilla-5520- Khan/Yakin - 08/01/2001 - code trace */
            TELICACODETRC(LOGERROR, ccCodeTrcLvl, ccInit.lmPst, ("CcLiCctConInd:spConnId=%lx, Insert spInstTbl failed", spConnId));
#endif /* Bugzilla-5520 */

            ccGenAlarm(LCM_CATEGORY_INTERNAL, LCC_EVENT_HASHING_FAILED, 
                       LCC_CAUSE_SPINSTTBL_INS, (U8 *) &con->icSpConnId);
            cmHashListDelete(&ccSuInstTbl, (PTR)con);

            cmHashListDelete(&ccSuOgInstTbl, (PTR)con);


            ccDropEvnt(con);

            SPutSBuf(ccInit.region, ccInit.pool, con->routeInfo,
                     (Size)sizeof(CcRouteInfo));
            TELICA_FREE_CONNECTION (con);

            ccSendRelease(NULLP, pCb, spConnId, 0, protType, CCTMPFAIL, NULL);
            if (uBuf != NULLP)
                ccDropUBuf(&uBuf);
            ccFreeSdpInTheEvnt(__FILE__, __LINE__, CCE_CONIND, 0,
                (PTR)ccConEvnt, protType);
            ccFreeIntBiccMBufInTheEvnt(__FILE__, __LINE__, CCE_CONIND, 0,
                (PTR)ccConEvnt, protType);
            ccFreeSipSipTMBufInTheEvnt(__FILE__, __LINE__, CCE_CONIND, 0, 
                (PTR)ccConEvnt, protType); /* FID 14732 */
            ccFreeHistInfoEntriesInTheEvnt(__FILE__, __LINE__, CCE_CONIND, 0,
                (PTR)ccConEvnt, protType); /*Bug 89424 */
            RETVALUE(RFAILED);
        }  
#endif /* cc028.15 : addition */
    }

#if 1 /* TELICA - Bugzilla-5520- Khan/Yakin - 08/01/2001 - code trace */
    TELICACODETRC(LOGDEBUG, ccCodeTrcLvl, ccInit.lmPst, ("CcLiCctConInd::ogSuConnId=%lx, icSuConnId=%lx",con->ogSuConnId, con->icSuConnId));
#endif /* Bugzilla-5520 */

#if 1 /* Do not turn this OFF, consult Marlboro Team! */
    CC_TRACE_EVENTS(con, logFirstConInd, CCE_CONIND);
#endif /* End Debug */

    /* Send to Call State machine */
    ccCallStateMachine (con, CCE_CONIND, (PTR)ccConEvnt, suId, spConnId,
                        (PTR) NULLP, (PTR) NULLP);
    ccFreeSdpInTheEvnt(__FILE__, __LINE__, CCE_CONIND, 0, (PTR)ccConEvnt,
        protType);
    ccFreeIntBiccMBufInTheEvnt(__FILE__, __LINE__, CCE_CONIND, 0, (PTR)ccConEvnt,
                               protType);
    ccFreeSipSipTMBufInTheEvnt(__FILE__, __LINE__, CCE_CONIND, 0, (PTR)ccConEvnt,
        protType); /* FID 14732 */
    ccFreeHistInfoEntriesInTheEvnt(__FILE__, __LINE__, CCE_CONIND, 0, (PTR)ccConEvnt,
        protType); /*Bug 89424 */
#ifdef ZC
    zcUpdPeer();
#endif /* ZC */
    RETVALUE(ROK);
}


/*
*
*      Fun:   CcLiCctConCfm
*
*      Desc:  notifies a successful N-ISUP connection establishment
*             signalling
*
*      Ret:   ROK     - successful,
*
*      Notes: None.
*
*      File:  cc_bdy1.c
*
*/

#ifdef ANSI
PUBLIC S16 CcLiCctConCfm
(
Pst       *pst,               /* post */
SuId      suId,               /* service user identier */
UConnId   suConnId,           /* service user instance identifier */
UConnId   spConnId,           /* service provider instance identifier */
ProtType  protType,           /* incoming protocol type */
CcConEvnt *ccConEvnt,         /* connection event structure */
Buffer    *uBuf               /* unrecognized parameters */
)
#else
PUBLIC S16 CcLiCctConCfm(pst, suId, suConnId, spConnId, protType, ccConEvnt, uBuf)
Pst       *pst;               /* post */
SuId      suId;               /* service user identier */
UConnId   suConnId;           /* service user instance identifier */
UConnId   spConnId;           /* service provider instance identifier */
ProtType  protType;           /* incoming protocol type */
CcConEvnt *ccConEvnt;         /* connection event structure */
Buffer    *uBuf;              /* unrecognized parameters */
#endif
{
   CcPsSap *pCb;
   CcConCb *con;
   S16 ret;
   UConnId tmpConnId;

   TRC3( CcLiCctConCfm)

#if 1 /* TELICA - Bugzilla-5520- Khan/Yakin - 08/01/2001 - code trace */
   TELICACODETRC(LOGDEBUG, ccCodeTrcLvl, ccInit.lmPst, ("CcLiCctConCfm::spConnId=%lx, suConnId=%lx", spConnId, suConnId));
#endif /* Bugzilla-5520 */


#if (ERRCLASS & ERRCLS_INT_PAR)
   if ((ccInit.cfgDone != TRUE) || (suId >= (SuId) ccCp.genCfg.maxPsSAP))
   {
      CCLOGERROR(ERRCLS_INT_PAR, ECC253, (ErrVal) suId,
                 "CcLiCctConCfm() Failed, invalid suId");
      if (uBuf != NULLP)
         ccDropUBuf(&uBuf);
      ccFreeSdpInTheEvnt(__FILE__, __LINE__, CCE_CONCFM, 0, (PTR)ccConEvnt,
        protType);
      ccFreeSipSipTMBufInTheEvnt(__FILE__, __LINE__, CCE_CONCFM, 0, 
        (PTR)ccConEvnt, protType); /* FID 14732 */
      ccFreeHistInfoEntriesInTheEvnt(__FILE__, __LINE__, CCE_CONCFM, 0,
        (PTR)ccConEvnt, protType); /*Bug 89424 */
      RETVALUE(RFAILED);
   }
#endif

   if ((ccPsSapLstPtr == NULLP) || ((pCb = *(ccPsSapLstPtr + suId)) == NULLP))
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      CCLOGERROR(ERRCLS_INT_PAR, ECC254, (ErrVal) suId,
                 "CcLiCctConCfm() Failed, invalid suId");
#endif
      ccGenAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT,
                 LCM_CAUSE_INV_SAP, (U8*) &suId);
      if (uBuf != NULLP)
         ccDropUBuf(&uBuf);
      ccFreeSdpInTheEvnt(__FILE__, __LINE__, CCE_CONCFM, 0, (PTR)ccConEvnt,
        protType);
      ccFreeSipSipTMBufInTheEvnt(__FILE__, __LINE__, CCE_CONCFM, 0, 
        (PTR)ccConEvnt, protType); /* FID 14732 */
      ccFreeHistInfoEntriesInTheEvnt(__FILE__, __LINE__, CCE_CONCFM, 0,
        (PTR)ccConEvnt, protType); /*Bug 89424 */
      RETVALUE(RFAILED);
   }

   if (pCb->state != CC_BND)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      CCLOGERROR(ERRCLS_INT_PAR, ECC255, (ErrVal) suId,
                 "CcLiCctConCfm() Failed, SAP not bound");
#endif
      ccGenAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_INV_STATE,
                 LCM_CAUSE_INV_SAP, (U8*) &suId);
      if (uBuf != NULLP)
         ccDropUBuf(&uBuf);
      ccFreeSdpInTheEvnt(__FILE__, __LINE__, CCE_CONCFM, 0, (PTR)ccConEvnt,
        protType);
      ccFreeSipSipTMBufInTheEvnt(__FILE__, __LINE__, CCE_CONCFM, 0, 
        (PTR)ccConEvnt, protType); /* FID 14732 */
      ccFreeHistInfoEntriesInTheEvnt(__FILE__, __LINE__, CCE_CONCFM, 0,
        (PTR)ccConEvnt, protType); /*Bug 89424 */
      RETVALUE(RFAILED);
   }

   /* Find connection */
#if 0 /* Fake SipMBuf */
   ccFakeAllocSipMBuf(__FILE__, __LINE__, CCE_CONCFM, 0, (PTR)ccConEvnt, protType);
#endif

   tmpConnId = suConnId;

   ret = cmHashListFind (&ccSuInstTbl, (U8 *)&tmpConnId, (U16)sizeof(UConnId),
                         (U16)0, (PTR *) &con);

   if (ret != ROK) 
      ret = cmHashListFind(&ccSuOgInstTbl, (U8 *)&tmpConnId,
                          (U16)sizeof(UConnId), (U16)0, (PTR *) &con);

   if (ret != ROK)
   {
     CCLOGERR("****** Alert : context not found : su:0x%lx, sp:0x%lx ***\n",
         tmpConnId, spConnId);

      if (uBuf != NULLP)
         ccDropUBuf(&uBuf);
      ccFreeSdpInTheEvnt(__FILE__, __LINE__, CCE_CONCFM, 0, (PTR)ccConEvnt,
        protType);
      ccFreeSipSipTMBufInTheEvnt(__FILE__, __LINE__, CCE_CONCFM, 0, 
        (PTR)ccConEvnt, protType); /* FID 14732 */
      ccFreeHistInfoEntriesInTheEvnt(__FILE__, __LINE__, CCE_CONCFM, 0,
        (PTR)ccConEvnt, protType); /*Bug 89424 */
      RETVALUE(RFAILED);
   }

/* TELICA-BUG:12976-mqin */
   ccChkCallTrc(con, CALLANS, 0);
/* BUG:12976 */

   con->direction = ccGetDirection(con, spConnId, suConnId, pCb);

   /* Store the spConnId on the outgoing interface */
   if (con->direction == OUTTOINC)
   {
       if (con->ogSpConnId == 0)
           con->ogSpConnId = spConnId;
   }
   else
   {
       if (con->icSpConnId == 0)
           con->icSpConnId = spConnId;
   }

   /* Bugzilla 10706:taraveti memory leak fix */
   ccDropUBuf(&con->uBuf);
   con->uBuf      = uBuf;
   uBuf           = NULL; /* Bug 44218 */

   if (ccInit.acnt == TRUE)
   {
       /* BUG:21485 - avoid resetting if present */
       if ((con->callDtlInfo.cm.tckOgConCfmRcvd.pres != PRSNT_NODEF)&&
           (con->keepGCCCallInfo == FALSE))
       {
           con->callDtlInfo.cm.tckOgConCfmRcvd.pres = PRSNT_NODEF;
           (Void) SGetSysTime(&con->callDtlInfo.cm.tckOgConCfmRcvd.val);
       }
       
       if (con->keepGCCCallInfo == FALSE)
       {
         con->callDtlInfo.cm.callState = LCC_CDR_ANSWERED;
       }
   }

   if (CC_EGRESS_INTER_CCS_CALL(con) && (con->direction == OUTTOINC))
   {
     ccProcessBkwdCallSetupInfoForInterCcsCall(con, CCE_CONCFM, 
                                               (PTR)ccConEvnt); 
   }

#if 1 /* Do not turn this OFF, consult Marlboro Team */
    CC_TRACE_EVENTS(con, logFirstConCfm, CCE_CONCFM);
#endif /* End Debug */

#if 1 /* BUG: 50880 */
  /* saves the msg type from SI to inform outgoing protocol if its the case */
  /* ESC23491-Bug86932 */
  if((CC_GET_PROTOCOL(protType) == CC_SI) ||
     (CC_GET_PROTOCOL(protType) == CC_EXT_BICC))
  {
      con->encapCONorANSmsg = ccConEvnt->m.siConEvnt.optElmnt.isup.encapCONorANSmsg;
      CCDP(AIN_DBG_LEVEL_2, "CcLiCctConCfm encapCONorANSmsg=%d\n", con->encapCONorANSmsg);
  }
#endif /* BUG: 50880 */

  /* BUG: 55296 - saves the msg type from BICC to inform outgoing protocol if its the case */
  if(CC_GET_PROTOCOL(protType) == CC_BICC)
  {
      con->encapCONorANSmsg = ccConEvnt->m.siConEvnt.optElmnt.bicc.encapCONorANSmsg;
      CCDP(AIN_DBG_LEVEL_2, "CcLiCctConCfm encapCONorANSmsg=%d bicc.encapCONorANSmsg=%d protType=%d\n", 
                con->encapCONorANSmsg, ccConEvnt->m.siConEvnt.optElmnt.bicc.encapCONorANSmsg, protType);
  }

   /* FID 14840 AoC */
   if ((CC_GET_PROTOCOL(protType) == CC_SIP) &&
                       (ccConEvnt != NULLP) &&
                       (ccConEvnt->m.sipConEvnt.sipXmlInfo.xmlBody.pres == PRSNT_NODEF) &&
                       (ccConEvnt->m.sipConEvnt.sipXmlInfo.xmlBody.val != NULLP))
   {
      if (ccCpyFreeSipXmlInfo(con, &(ccConEvnt->m.sipConEvnt.sipXmlInfo)) != ROK)
      {
          RETVALUE(RFAILED);
      }
      CCDP(AIN_DBG_LEVEL_2, "FID 14840 CcLiCctConCfm %s free and copy sipXmlInfo \n", __FUNCTION__);
   }
   /* End of FID 14840 */

   /* call state machine function */
   ccCallStateMachine (con, CCE_CONCFM, (PTR)ccConEvnt, suId, spConnId,
                       (PTR) NULLP, (PTR) NULLP);
   ccFreeSdpInTheEvnt(__FILE__, __LINE__, CCE_CONCFM, 0, (PTR)ccConEvnt,
        protType);
   ccFreeIntBiccMBufInTheEvnt(__FILE__, __LINE__, CCE_CONCFM, 0, 
                              (PTR)ccConEvnt, protType);
   ccFreeSipSipTMBufInTheEvnt(__FILE__, __LINE__, CCE_CONCFM, 0, (PTR)ccConEvnt,
            protType); /* FID 14732 */
   ccFreeHistInfoEntriesInTheEvnt(__FILE__, __LINE__, CCE_CONCFM, 0, (PTR)ccConEvnt,
            protType); /*Bug 89424 */
#ifdef ZC
   zcUpdPeer();
#endif /* ZC */
   RETVALUE(ROK);
}



/*
*
*      Fun:   CcLiCctCnStInd
*
*      Desc:  Process an connection establishment
*
*      Ret:   ROK     - successful,
*
*      Notes: None.
*
*      File:  cc_bdy1.c
*
*/

#ifdef ANSI
PUBLIC S16 CcLiCctCnStInd
(
Pst        *pst,               /* post */
SuId       suId,               /* service user identier */
UConnId    suConnId,           /* service user instance identifier */
UConnId    spConnId,           /* service provider instance identifier */
U8         evntType,           /* incoming event type */
ProtType   protType,           /* incoming protocol type */
CcCnStEvnt *ccCnStEvnt,        /* connection status event structure */
Buffer     *uBuf               /* unrecognized parameters */
)
#else
PUBLIC S16 CcLiCctCnStInd(pst, suId, suConnId, spConnId, evntType, protType, 
                         ccCnStEvnt, uBuf)
Pst        *pst;               /* post */
SuId       suId;               /* service user identier */
UConnId    suConnId;           /* service user instance identifier */
UConnId    spConnId;           /* service provider instance identifier */
U8         evntType;           /* incoming event type */
ProtType   protType;           /* incoming protocol type */
CcCnStEvnt *ccCnStEvnt;        /* connection status event structure */
Buffer     *uBuf;              /* unrecognized parameters */
#endif
{
   CcPsSap *pCb;
   CcConCb *con;
   S16 ret;
   UConnId tmpConnId;
   U8      tempEvntType = 0xFF; /* 27249 */
   Buffer  *saveUBuf = NULLP;		/* 44312 */
   U8      tmpDirection = DIR_UNKNOWN;  /* BUG: 71635 */
   ProtType egressCCSOgProtType = 0; /* 72947 */
   /* Bug 86734 ++ */
   ProtType ogProtType = 0;
   TknU8   *pCadPtyCatInd = NULLP;
   /* Bug 86734 --*/

   TRC3( CcLiCctCnStInd)

#if (ERRCLASS & ERRCLS_INT_PAR)
   if ((ccInit.cfgDone != TRUE) || (suId >= (SuId) ccCp.genCfg.maxPsSAP))
   {
      CCLOGERROR(ERRCLS_INT_PAR, ECC257, (ErrVal) suId,
                 "CcLiCctCnStInd() Failed, invalid suId");
      if (uBuf != NULLP)
         ccDropUBuf(&uBuf);
      ccFreeSdpInTheEvnt(__FILE__, __LINE__, CCE_CNSTIND, evntType, 
          (PTR)ccCnStEvnt, protType);
      ccFreeIntBiccMBufInTheEvnt(__FILE__, __LINE__, CCE_CNSTIND, evntType,
                                 (PTR)ccCnStEvnt, protType);
      ccFreeSipSipTMBufInTheEvnt(__FILE__, __LINE__, CCE_CNSTIND, evntType, 
                                 (PTR)ccCnStEvnt, protType); /* FID 14732 */
      ccFreeHistInfoEntriesInTheEvnt(__FILE__, __LINE__, CCE_CNSTIND, evntType,
                                     (PTR)ccCnStEvnt, protType); /*Bug 89424 */
      RETVALUE(RFAILED);
   }
#endif

   if ((ccPsSapLstPtr == NULLP) || ((pCb = *(ccPsSapLstPtr + suId)) == NULLP))
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      CCLOGERROR(ERRCLS_INT_PAR, ECC258, (ErrVal) suId,
                 "CcLiCctCnStInd() Failed, invalid suId");
#endif
      ccGenAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT,
                 LCM_CAUSE_INV_SAP, (U8*) &suId);
      if (uBuf != NULLP)
         ccDropUBuf(&uBuf);
      ccFreeSdpInTheEvnt(__FILE__, __LINE__, CCE_CNSTIND, evntType, 
          (PTR)ccCnStEvnt, protType);
      ccFreeIntBiccMBufInTheEvnt(__FILE__, __LINE__, CCE_CNSTIND, evntType,
                                 (PTR)ccCnStEvnt, protType);
      ccFreeSipSipTMBufInTheEvnt(__FILE__, __LINE__, CCE_CNSTIND, evntType, 
                                 (PTR)ccCnStEvnt, protType); /* FID 14732 */
      ccFreeHistInfoEntriesInTheEvnt(__FILE__, __LINE__, CCE_CNSTIND, evntType,
                                     (PTR)ccCnStEvnt, protType); /*Bug 89424 */
      RETVALUE(RFAILED);
   }

   if (pCb->state != CC_BND)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      CCLOGERROR(ERRCLS_INT_PAR, ECC259, (ErrVal) suId,
                 "CcLiCctCnStInd() Failed, SAP not bound");
#endif
      ccGenAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_INV_STATE,
                 LCM_CAUSE_INV_SAP, (U8*) &suId);
      if (uBuf != NULLP)
         ccDropUBuf(&uBuf);
      ccFreeSdpInTheEvnt(__FILE__, __LINE__, CCE_CNSTIND, evntType, 
          (PTR)ccCnStEvnt, protType);
      ccFreeIntBiccMBufInTheEvnt(__FILE__, __LINE__, CCE_CNSTIND, evntType,
                                 (PTR)ccCnStEvnt, protType);
      ccFreeSipSipTMBufInTheEvnt(__FILE__, __LINE__, CCE_CNSTIND, evntType, 
                                 (PTR)ccCnStEvnt, protType); /* FID 14732 */
      ccFreeHistInfoEntriesInTheEvnt(__FILE__, __LINE__, CCE_CNSTIND, evntType,
                                     (PTR)ccCnStEvnt, protType); /*Bug 89424 */
      RETVALUE(RFAILED);
   }

   /*
    * Check to see if this CnStInd is for a call Created Via Geo Audit or
    * normal call. Geo Audit calls have bit<28> of the suConnId set
    */
   if (suConnId & CC_GEO_AUD_CON_FLG)
   {
      if (uBuf != NULLP)
         ccDropUBuf(&uBuf);
      ccFreeSdpInTheEvnt(__FILE__, __LINE__, CCE_CNSTIND, evntType, 
          (PTR)ccCnStEvnt, protType);
      ccFreeIntBiccMBufInTheEvnt(__FILE__, __LINE__, CCE_CNSTIND, evntType,
                                 (PTR)ccCnStEvnt, protType);
      ccFreeSipSipTMBufInTheEvnt(__FILE__, __LINE__, CCE_CNSTIND, evntType, 
                                 (PTR)ccCnStEvnt, protType); /* FID 14732 */
      ccFreeHistInfoEntriesInTheEvnt(__FILE__, __LINE__, CCE_CNSTIND, evntType,
                                     (PTR)ccCnStEvnt, protType); /*Bug 89424 */
       if ((ret = CcGeoCnStInd (pst, suId, suConnId, spConnId, evntType,
               protType, ccCnStEvnt)) != ROK)
       {
           RETVALUE(RFAILED);
       }
       else
       {
           RETVALUE(ROK);
       }
   }

#if 0 /* Fake SipMBuf */
   ccFakeAllocSipMBuf(__FILE__, __LINE__, CCE_CNSTIND, evntType, (PTR)ccCnStEvnt, protType);
#endif

   /* Find connection */
   /* Mask the least significant bit of suConnId before searching  for */
   /* the connection */
   tmpConnId = suConnId;

   ret = cmHashListFind (&ccSuInstTbl, (U8 *)&tmpConnId, (U16)sizeof(UConnId),
                         (U16)0, (PTR *) &con);
   if (ret != ROK) 
      ret = cmHashListFind(&ccSuOgInstTbl, (U8 *)&tmpConnId,
                        (U16)sizeof(UConnId), (U16)0, (PTR *) &con);

   if (ret != ROK)
      ret = cmHashListFind (&pCb->spInstTbl, (U8 *)&spConnId, 
                            (U16)sizeof(UConnId), (U16)0, (PTR *) &con);

   if (ret != ROK)
   {

       /* Bug 43456: Request SIP Audits */
       if((suConnId == 0) && ((protType == CC_SIP)||(CC_GET_PROTOCOL(protType) == CC_SIPT)) && (evntType == CCSIP_ET_START_AUDIT))
       {
          CCDP(AIN_DBG_LEVEL_2, "Start GCC-SIP Audit\n");
          ccTriggerSIPAudit();
          RETVALUE(ROK);
       }

#if 1 /* Temp-hack to turn off ROH on PermBusy cas-lines */
       if((suConnId == 0) && ((protType == CC_CS_LN)||(protType == CC_GR303)) && (evntType == CS_CONN_STATUS_ON_HOOK))
       {
           RmRsc rsc;
           
           rsc.intfc.intfType = CC_CS_LN_INTFC;
           if (protType == CC_GR303)
              rsc.intfc.intfType = CC_G3_INTFC;
           rsc.intfc.t.intfcId = 0; /* Don't need this */
           rsc.rscPres = TRUE ;
#if 0
           rsc.t.ds0Rsc = spConnId;
           
           CCDP(AIN_DBG_LEVEL_2, "PERM SIGNAL:  turning off ROH on (%ld,%ld,%ld)\n",
                rsc.t.ds0Rsc>>19 &0x1F,
                rsc.t.ds0Rsc>>5 &0x3FF,
                rsc.t.ds0Rsc &0x1F);
#endif


           ccSendToneReq(&rsc, MS_IDLE);
           
           RETVALUE(ROK);
       }
#endif /* Temp-hack  END */

#if 0
       ccGenAlarm(LCM_CATEGORY_INTERNAL, LCC_EVENT_HASHING_FAILED, 
                  LCC_CAUSE_SPINSTTBL_FIND, (U8 *) &spConnId);
#else
       if ((protType == CC_CS_TG)||(protType == CC_CS_LN)||(protType == CC_GR303))
       {
#if 0
           CCLOGERR("%s: unexpected:  protType:%d, icSp:0x%08lX, icSu:0x%08lX, (%d,%d,%d)\n", 
                    __FUNCTION__, protType, spConnId, suConnId, 
                    ((CsChanId_t *)&spConnId)->parts.iomSlot,
                    ((CsChanId_t *)&spConnId)->parts.ds1Port,
                    ((CsChanId_t *)&spConnId)->parts.ds0Chan);
#endif
       }
       else
       {
           CCLOGERR("****** Alert : context not found : su:0x%lx, sp:0x%lx ***\n",
               tmpConnId, spConnId);

       }
#endif

       if (uBuf != NULLP)
           ccDropUBuf(&uBuf);
       ccFreeSdpInTheEvnt(__FILE__, __LINE__, CCE_CNSTIND, evntType, 
          (PTR)ccCnStEvnt, protType);
       ccFreeIntBiccMBufInTheEvnt(__FILE__, __LINE__, CCE_CNSTIND, evntType,
                                 (PTR)ccCnStEvnt, protType);
       ccFreeSipSipTMBufInTheEvnt(__FILE__, __LINE__, CCE_CNSTIND, evntType, 
                                 (PTR)ccCnStEvnt, protType); /* FID 14732 */
       ccFreeHistInfoEntriesInTheEvnt(__FILE__, __LINE__, CCE_CNSTIND, evntType,
                                      (PTR)ccCnStEvnt, protType); /*Bug 89424 */
       RETVALUE(RFAILED);
   }

   /* Debug code */
   if (0xFE == evntType)
   {
     CCLOGERR("****** Rcv DUMP CON from SIPA ******\n");
     CC_TRACE_DUMP(con);

      if (uBuf != NULLP)
           ccDropUBuf(&uBuf); /* Bug 44218 */
      
     RETVALUE(RFAILED);
   }
   /* Debug code */

   tmpDirection   = con->direction; /* BUG: 71635 */
   con->direction = ccGetDirection(con, spConnId, suConnId, pCb);

   /* Store the spConnId on the outgoing interface */
   if ((con->direction == OUTTOINC) && (con->ogSpConnId == 0))
      con->ogSpConnId = spConnId;
   else if((con->direction == INCTOOUT) && (con->icSpConnId == 0))
      con->icSpConnId = spConnId; /* added for bug#20323 */

   /* FID17523.0 There is an existing bug, if ISDN NOTIFY is received before call answer,
    * the SIP 200OK INVITE can't be mapped to ISDN CONNECT. The root cause is,
    * here con->icEvntType is set as CCIN_ET_NOTIFY(20). When SIP answers the call, 
    * con->icEvntType is not reset. This caused the call meet the following logic
    * in ccAnswerCallsCallBack(), because CCSIP_ET_SIP_INFO_DTMF_DET_ON is also defined as 20
    *   if ((con->ogProtType == CC_SIP) &&
         ((con->icEvntType == CCSIP_ET_SIP_INFO_DTMF_DET_ON) ||
          (con->icEvntType == CCSIP_ET_SIP_INFO_DTMF_DET_OFF)))
         {
            CC_STATE_CHANGE(con, CCS_ANSWERED);
            RETVALUE(ROK);
         }
    * Since this feature requests to ignore the ISDN NOTIFY message, we just jump out from here.
    */
   if(evntType == CCIN_ET_NOTIFY && protType == CC_INETSI)
   {
       CCDP(AIN_DBG_LEVEL_2, "CcLiCctCnStInd: ISDN NOTIFY is dropped\n");
       if (uBuf != NULLP)
           ccDropUBuf(&uBuf);
       ccFreeSdpInTheEvnt(__FILE__, __LINE__, CCE_CNSTIND, evntType, 
          (PTR)ccCnStEvnt, protType);
       ccFreeIntBiccMBufInTheEvnt(__FILE__, __LINE__, CCE_CNSTIND, evntType,
                                 (PTR)ccCnStEvnt, protType);
       ccFreeSipSipTMBufInTheEvnt(__FILE__, __LINE__, CCE_CNSTIND, evntType, 
                                 (PTR)ccCnStEvnt, protType); /* FID 14732 */
       ccFreeHistInfoEntriesInTheEvnt(__FILE__, __LINE__, CCE_CNSTIND, evntType,
                                      (PTR)ccCnStEvnt, protType); /*Bug 89424 */
       RETVALUE(ROK);
   }

   /* 27249 */
   /* Bug# 77814 - Save con->icEvntType for Msgs in both directions */
   tempEvntType = con->icEvntType;
   con->icEvntType = evntType;
   /* 27249 */

   /* Bugzilla 10706:taraveti memory leak fix */
#if 0	/* 44312+ */
   ccDropUBuf(&con->uBuf);
#else
   saveUBuf = con->uBuf;
#endif	/* 44312- */
   con->uBuf      = uBuf;
   uBuf           = NULL; /* Bug 44218 */

   /* FID 14677.0 ITU99 CC_ET_CFN for unknown Propagation Delay Count or Call History clear the stored value */
   if (evntType == CC_ET_CFN)
   {
      if ((con->icIntfcCb != NULL) && con->icIntfcCb->longDelay)
      {
          CCDP(AIN_DBG_LEVEL_2, "CcLiCctCnStInd: CC_ET_CFN received\n");

          /* Set totalPropDelay to NOTPRSNT_DELETED to indicates a CC_ET_ CFN has been received */
          con->echoCntrl.totalPropDelay.pres = NOTPRSNT_DELETED;
          con->echoCntrl.totalPropDelay.val = 0x0;
       }

       /* BUG:82657 */
       if (CC_GET_PROTOCOL(protType) == CC_SI)
       {
       /* CC_ET_CFN should always be ignored here to avoid for further processing */
          CCDP(AIN_DBG_LEVEL_2, "CcLiCctCnStInd: CC_ET_CFN on ISUP, ignore mapping\n");
       RETVALUE(ROK);
   }
   }
   /* End of FID 14677.0 ITU99 */

   /* Look for GLARE and Re-attempt from CAS and feed REATTEMPT-IND to state
    * machine */
   if(protType == CC_CS_TG)
   {
     switch (evntType)         
     {
       case CS_CONN_STATUS_GLARE:
       case CS_CONN_STATUS_REATTEMPT:
         ccCallStateMachine (con, CCE_REATIND, (PTR)NULLP, suId, spConnId,
                             (PTR) NULLP, (PTR) NULLP);
#ifdef ZC
         zcUpdPeer();
#endif /* ZC */
#if 1	/* 44312 */
	 ccDropUBuf(&saveUBuf); 
#endif		 
          RETVALUE(ROK);
         break;
#if 1 /*FGD*/
       /* On FGD trunk, ACK WINK to Tandem */
       case CS_CONN_STATUS_ADDR_COMPLETE:
          CCDP(AIN_DBG_LEVEL_2, "CcLiCctCnStInd:CS_CONN_STATUS_ADDR_COMPLETE sent to GCC\n");
          break;
       case CS_CONN_STATUS_START:
          CCDP(AIN_DBG_LEVEL_2, "CcLiCctCnStInd:CS_CONN_STATUS_START sent to GCC\n");
          break;
#endif /*FGD*/
       default:
         break;
     }
   }
   /* 72947 for AOC sip-sip inter ccs call, don't unpack the bkwd setup information */
   CC_GET_EGRESS_CCS_GENERIC_OGPROTTYPE(con, egressCCSOgProtType); /* 72947 */

/* Bukucu - Bug-14807 & Bug-14587 09.30.02  */
   if (ccInit.acnt == TRUE)
   {
       if((ccIsOnHookRcvd(con->icProtType, con->ogProtType, con->icEvntType,
                           con->direction)) &&
           ((con->callDtlInfo.cm.tckRelIndRcvd.pres == NOTPRSNT) ||
           isThisConInAOTCCall(con)))
       {
           ccSetRelIndTicks (con, NULLP);
       }
       if(ccIsAlertingIndRcvd(con))/*if GCC get the Alerting message, then save the tick*/
       {
           (void) SGetSysTime(&con->callDtlInfo.cm.tckOgAlertingIndRcvd.val);
           con->callDtlInfo.cm.tckOgAlertingIndRcvd.pres = PRSNT_NODEF;
           CCDP(AIN_DBG_LEVEL_0, "GCC received the Alerting message, the System tick is %ld\n", con->callDtlInfo.cm.tckOgAlertingIndRcvd.val);
           /* FID 15919.0 */
           if (CC_EGRESS_INTER_CCS_CALL(con))
           {
               ogProtType = egressCCSOgProtType;
           }
           else
           {
               ogProtType = CC_GET_PROTOCOL(con->ogProtType);
           }

           pCadPtyCatInd = (ccCnStEvnt != NULLP)?(&ccCnStEvnt->m.siCnStEvnt.bckCallInd.cadPtyStatInd):NULLP;

           if (con->callDtlInfo.cm.tckOgACM180IndRcvd.pres != PRSNT_NODEF)
           {
               if ((ogProtType == CC_SI) || (ogProtType == CC_EXT_BICC) || (ogProtType == CC_SIPT))
               {
                   if ((pCadPtyCatInd != NULLP) && (pCadPtyCatInd->pres == PRSNT_NODEF) && (pCadPtyCatInd->val == CADSTAT_SUBFREE))
                   {
                       con->callDtlInfo.cm.tckOgACM180IndRcvd.pres = con->callDtlInfo.cm.tckOgAlertingIndRcvd.pres;
                       con->callDtlInfo.cm.tckOgACM180IndRcvd.val  = con->callDtlInfo.cm.tckOgAlertingIndRcvd.val;
                       CCDP(AIN_DBG_LEVEL_0, "GCC received the ACM/180 message, the System tick is %ld, ogProtType is %d\n",
                            con->callDtlInfo.cm.tckOgACM180IndRcvd.val, ogProtType);
                   }
               }
               else if (ogProtType == CC_SIP)
               {
                   if (con->icEvntType == CCSIP_ET_180_RINGING)
                   {
                       con->callDtlInfo.cm.tckOgACM180IndRcvd.pres = con->callDtlInfo.cm.tckOgAlertingIndRcvd.pres;
                       con->callDtlInfo.cm.tckOgACM180IndRcvd.val  = con->callDtlInfo.cm.tckOgAlertingIndRcvd.val;
                       CCDP(AIN_DBG_LEVEL_0, "GCC received the ACM/180 message, the System tick is %ld, ogProtType is %d\n", 
                            con->callDtlInfo.cm.tckOgACM180IndRcvd.val, ogProtType);
                   }
               }          
           }
       }
   }

   if (CC_EGRESS_INTER_CCS_CALL(con) && (con->direction == OUTTOINC) &&
       !(CC_BOTH_TG_SUPPORT_SCI_XML(con) && 
         ((CC_GET_PROTOCOL(con->icProtType) == CC_SIP) ||
          (CC_GET_PROTOCOL(con->icProtType) == CC_SIPT)) && /* 73282 */
        (egressCCSOgProtType == CC_SIP) && (con->icEvntType == CC_ET_APPTRAN)))
   {
     ccProcessBkwdCallSetupInfoForInterCcsCall(con, CCE_CNSTIND,
                                               (PTR)ccCnStEvnt);
   }
/* + FID 14732 */
   else if (CC_INGRESS_INTER_CCS_CALL(con) && (con->direction == INCTOOUT))
   {
     ccProcessFwdCallSetupInfoForInterCcsCall(con, CCE_CNSTIND,
                                               (PTR)ccCnStEvnt);
   }
/* - FID 14732 */

#if 1 /* BUG 63343 */
   if ( (CC_GET_PROTOCOL(con->icProtType) == CC_SI) && (evntType == CC_ET_NMINFO) )
   {
      /*
       * Event CC_ET_NMINFO does not allow uBuf, i.e. uBuf is set to NULLP by protocol.
       * Revert con->uBuf prior to calling state machine.
       */
      CCDP(AIN_DBG_LEVEL_2, "CcLiCctCnStInd: con->uBuf %p revert to saveUBuf %p\n", con->uBuf, saveUBuf);

      /* If uBuf is not nullp by any chance, drop it. */
      if (con->uBuf != NULLP)
      {
         CCDP(AIN_DBG_LEVEL_2, "CcLiCctCnStInd: drop uBuf %p\n", con->uBuf);
         ccDropUBuf(&con->uBuf);
      }
      
      con->uBuf = saveUBuf;
      saveUBuf = NULLP;
   }
#endif

   /* Bug:94004
    * For normal-->inter-gateway call, store ORI in ACM immediately in case of ACM not mapping
    * If it is inter-ccs call,this is done in ingress ccs.
    */
    if ((con->direction == OUTTOINC) &&
        (ccCnStEvnt != NULLP) &&
        (!CC_INGRESS_INTER_CCS_CALL(con)) &&
        (con->icIntfcCb) && (con->ogIntfcCb) &&
        ((CC_GET_PROTOCOL(con->ogIntfcCb->protType) == CC_SI) || (CC_GET_PROTOCOL(con->ogIntfcCb->protType) == CC_SIPT)) &&
        (con->icEvntType == CC_ET_ACM) &&
        (CC_IS_OG_ORI_SUPPORTED(con)) &&
        (!CC_IS_IC_ORI_SUPPORTED(con)))
    {
       SiOgRteId *ori = NULLP;
       ori =  &(ccCnStEvnt->m.siCnStEvnt.ogRteId);

       if (CC_IS_ORI_MISS(ori))
       {
          CCDP(AIN_DBG_LEVEL_2, "CcLiCctCnStInd:Missing ORI,store default values \n");
          CC_STORE_ORI_TO_CDR_DEFAULT(con);
       } 
       else
       {
          CCDP(AIN_DBG_LEVEL_2, "CcLiCctCnStInd:Storing ORI \n");
          CC_STORE_ORI_TO_CDR(con, ori);
       }
    }

   /* FID 14840 AoC */
   if ((CC_GET_PROTOCOL(protType) == CC_SIP) &&
                       (ccCnStEvnt != NULLP) &&
                       (ccCnStEvnt->m.sipCnStEvnt.sipXmlInfo.xmlBody.pres == PRSNT_NODEF) &&
                       (ccCnStEvnt->m.sipCnStEvnt.sipXmlInfo.xmlBody.val != NULLP))
   {
      if (ccCpyFreeSipXmlInfo(con, &(ccCnStEvnt->m.sipCnStEvnt.sipXmlInfo)) != ROK)
      {
          RETVALUE(RFAILED);
      }
      CCDP(AIN_DBG_LEVEL_2, "FID 14840 CcLiCctCnStInd %s free and copy sipXmlInfo \n", __FUNCTION__);
   }
   /* End of FID 14840 AoC */

   /* FID 15174.0 */
   if ((CC_GET_PROTOCOL(protType) == CC_SIPT) && (evntType == CCSIP_ET_MULTIINVITE))
   {
      /* SIPT Multi-Invite Overlap */
      if (ccValidateMultiInvite(con, spConnId, ccCnStEvnt) != ROK)
      {
         /* Ignore the message */
         RETVALUE(ROK);
      }
   }

   /* FID15083.0 */
   ccChkSrvcForLi(con, ccCnStEvnt);

   /* call state machine function */
   ccCallStateMachine (con, CCE_CNSTIND, (PTR)ccCnStEvnt, suId, spConnId,
                       (PTR) NULLP, (PTR) NULLP);
   ccFreeSdpInTheEvnt(__FILE__, __LINE__, CCE_CNSTIND, evntType, 
          (PTR)ccCnStEvnt, protType);
   ccFreeIntBiccMBufInTheEvnt(__FILE__, __LINE__, CCE_CNSTIND, evntType, 
                             (PTR)ccCnStEvnt, protType); 
   ccFreeSipSipTMBufInTheEvnt(__FILE__, __LINE__, CCE_CNSTIND, evntType, 
                             (PTR)ccCnStEvnt, protType); /* FID 14732 */
   ccFreeHistInfoEntriesInTheEvnt(__FILE__, __LINE__, CCE_CNSTIND, evntType,
                                  (PTR)ccCnStEvnt, protType); /*Bug 89424 */
  if (con->bufferBlock.bufferedEvent != NULLP)
  {
    /* FID17373.0 do not revert the direction and evntType again as it revert to in cc_lib.c*/
    if ((CC_GET_PROTOCOL(con->icProtType) == CC_EXT_BICC) && 
        (con->cotBlock.contType == CC_IAM_INITIATED) &&
        (con->ogPrecndCb) && 
        (!PRECND_IS_CALL_FLAG2(con->ogPrecndCb, PRECND_FLAG_COT_RECVED)))
    {
      CCDP(AIN_DBG_LEVEL_2, "%s not revert as cc_lib.c done\n", __FUNCTION__);
    }
    else
    {
      if (tempEvntType != 0xFF)
      {
        /* This event is buffered. So, revert the icEvntType - 27249 */
        CCDP(AIN_DBG_LEVEL_2, "*** icEvntType is reverted from %d to %d\n",
             con->icEvntType, tempEvntType);
        con->icEvntType = tempEvntType;
      }
      if (tmpDirection != DIR_UNKNOWN)
      {
        CCDP(AIN_DBG_LEVEL_2, "***Direction is reverted from %d to %d****\n",
             con->direction, tmpDirection);
        con->direction  = tmpDirection; /* BUG: 71635 */
      }
#if 1	/* 44312+ */
      con->uBuf = saveUBuf;
    }
  } else {
    ccDropUBuf(&saveUBuf);
#endif	/* 44312- */    
  }

#ifdef ZC
   zcUpdPeer();
#endif /* ZC */
   RETVALUE(ROK);
}



/*
*
*      Fun:   CcLiCctRelInd
*
*      Desc:  process an incoming connection release request
*
*      Ret:   ROK     - successful,
*
*      Notes: None.
*
*      File:  cc_bdy1.c
*
*/

#ifdef ANSI
PUBLIC S16 CcLiCctRelInd
(
Pst       *pst,               /* post */
SuId      suId,               /* service user identier */
UConnId   suConnId,           /* service user instance identifier */
UConnId   spConnId,           /* service provider instance identifier */
U8        relType,            /* release type */
ProtType  protType,           /* incoming protocol type */
CcRelEvnt *ccRelEvnt,         /* connection release event structure */
Buffer    *uBuf               /* unrecognized parameters */
)
#else
PUBLIC S16 CcLiCctRelInd(pst, suId, suConnId, spConnId, relType, protType, 
                         ccRelEvnt, uBuf)
Pst       *pst;               /* post */
SuId      suId;               /* service user identier */
UConnId   suConnId;           /* service user instance identifier */
UConnId   spConnId;           /* service provider instance identifier */
U8        relType;            /* release type */
ProtType  protType;           /* incoming protocol type */
CcRelEvnt *ccRelEvnt;         /* connection release event structure */
Buffer    *uBuf;              /* unrecognized parameter */
#endif
{
   CcPsSap *pCb;
   CcConCb *con;
   CcConCb *tmpCon; /* bug 62895 */
   S16 ret;
   UConnId tmpConnId;
   /* bug#35825, rkhan */
   U8 tmpDirection = DIR_UNKNOWN;
   Buffer *saveUBuf = NULLP;	/* 44312 */
   Bool handle_acc = FALSE;
   TRC3( CcLiCctRelInd)

#if 1 /* TELICA - Bugzilla-5520- Khan/Yakin - 08/01/2001 - code trace */
   TELICACODETRC(LOGDEBUG, ccCodeTrcLvl, ccInit.lmPst, ("CcLiCctRelInd::suConnId=%lx, relType=%d",suConnId, relType));
#endif /* Bugzilla-5520 */


#if (ERRCLASS & ERRCLS_INT_PAR)
   if ((ccInit.cfgDone != TRUE) || (suId >= (SuId) ccCp.genCfg.maxPsSAP))
   {
      CCLOGERROR(ERRCLS_INT_PAR, ECC261, (ErrVal) suId,
                 "CcLiCctRelInd() Failed, invalid suId");
      if (uBuf != NULLP)
         ccDropUBuf(&uBuf);
      ccFreeIntBiccMBufInTheEvnt(__FILE__, __LINE__, CCE_RELIND, 0, (PTR)ccRelEvnt,
                              protType);
      ccFreeSipSipTMBufInTheEvnt(__FILE__, __LINE__, CCE_RELIND, 0, (PTR)ccRelEvnt,
          protType); /* FID 14732 */
      ccFreeHistInfoEntriesInTheEvnt(__FILE__, __LINE__, CCE_RELIND, 0, (PTR)ccRelEvnt,
          protType); /*Bug 89424 */
      RETVALUE(RFAILED);
   }
#endif

   if ((ccPsSapLstPtr == NULLP) || ((pCb = *(ccPsSapLstPtr + suId)) == NULLP))
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      CCLOGERROR(ERRCLS_INT_PAR, ECC262, (ErrVal) suId,
                 "CcLiCctRelInd() Failed, invalid suId");
#endif
      ccGenAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT,
                 LCM_CAUSE_INV_SAP, (U8*) &suId);
      if (uBuf != NULLP)
         ccDropUBuf(&uBuf);
      ccFreeIntBiccMBufInTheEvnt(__FILE__, __LINE__, CCE_RELIND, 0, (PTR)ccRelEvnt,
                              protType);
      ccFreeSipSipTMBufInTheEvnt(__FILE__, __LINE__, CCE_RELIND, 0, (PTR)ccRelEvnt,
          protType); /* FID 14732 */
      ccFreeHistInfoEntriesInTheEvnt(__FILE__, __LINE__, CCE_RELIND, 0, (PTR)ccRelEvnt,
          protType); /*Bug 89424 */
      RETVALUE(RFAILED);
   }

/* ravi -    if (pCb->state != CC_BND) */
   if (pCb->state == CC_UNBND)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      CCLOGERROR(ERRCLS_INT_PAR, ECC263, (ErrVal) suId,
                 "CcLiCctRelInd() Failed, SAP not bound");
#endif
      ccGenAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_INV_STATE,
                 LCM_CAUSE_INV_SAP, (U8*) &suId);
      if (uBuf != NULLP)
         ccDropUBuf(&uBuf);
      ccFreeIntBiccMBufInTheEvnt(__FILE__, __LINE__, CCE_RELIND, 0, (PTR)ccRelEvnt,
                              protType);
      ccFreeSipSipTMBufInTheEvnt(__FILE__, __LINE__, CCE_RELIND, 0, (PTR)ccRelEvnt,
          protType); /* FID 14732 */
      ccFreeHistInfoEntriesInTheEvnt(__FILE__, __LINE__, CCE_RELIND, 0, (PTR)ccRelEvnt,
          protType); /*Bug 89424 */
      RETVALUE(RFAILED);
   }

#if 0 /* Fake SipMBuf */
   ccFakeAllocSipMBuf(__FILE__, __LINE__, CCE_RELIND, 0, (PTR)ccRelEvnt, protType);
#endif

   /*
    * Check to see if this RelInd is for a call Created Via Geo Audit or normal call
    * Geo Audit calls have bit<28> of the suConnId set
    */
   if (suConnId & CC_GEO_AUD_CON_FLG)
   {
      if (uBuf != NULLP)
         ccDropUBuf(&uBuf);

       if ((ret = CcGeoRelInd (pst, suId, suConnId, spConnId, relType, protType, ccRelEvnt,NULLP)) != ROK)
       {
           RETVALUE(RFAILED);
       }
       else
       {
           RETVALUE(ROK);
       }
   }

   /* Find connection */
   /* Mask the least significant bit of suConnId before searching  for */
   /* the connection */
   tmpConnId = suConnId;

   ret = cmHashListFind (&ccSuInstTbl, (U8 *)&tmpConnId, (U16)sizeof(UConnId),
                         (U16)0, (PTR *) &con);
   if (ret != ROK) 
      ret = cmHashListFind(&ccSuOgInstTbl, (U8 *)&tmpConnId,
                        (U16)sizeof(UConnId), (U16)0, (PTR *) &con);

   if (ret != ROK)
      ret = cmHashListFind (&pCb->spInstTbl, (U8 *)&spConnId, 
                            (U16)sizeof(UConnId), (U16)0, (PTR *) &con);

   if (ret != ROK)
   {
      if (uBuf != NULLP)
         ccDropUBuf(&uBuf);
      ccFreeIntBiccMBufInTheEvnt(__FILE__, __LINE__, CCE_RELIND, 0, (PTR)ccRelEvnt,
                              protType);
      ccFreeSipSipTMBufInTheEvnt(__FILE__, __LINE__, CCE_RELIND, 0, (PTR)ccRelEvnt,
          protType); /* FID 14732 */
      ccFreeHistInfoEntriesInTheEvnt(__FILE__, __LINE__, CCE_RELIND, 0, (PTR)ccRelEvnt,
          protType); /*Bug 89424 */
      RETVALUE(RFAILED);
   }
   /* bug#35825, rkhan */
   tmpDirection = con->direction;

   con->direction = ccGetDirection(con, spConnId, suConnId, pCb);
   /* Store the spConnId on the outgoing interface */
   if ((con->direction == OUTTOINC) && (con->ogSpConnId == 0))
      con->ogSpConnId = spConnId;

   /* Bug 43938 */      
   if ((spConnId == 0) && ((CC_GET_PROTOCOL(protType) == CC_SIP) ||
                           (CC_GET_PROTOCOL(protType) == CC_SIPT)))
   {
      /* euysal - 02/07/2006 - Bug: 44374 Begin */
     if (suConnId == con->icSuConnId)
     {
       cmHashListDelete(&con->icSapCb->spInstTbl, (PTR)con);
       if ((con->icSapCb) && (con->icSapCb->nmbActvConn))
         con->icSapCb->nmbActvConn--;
       con->icSpConnId = 0;
     }
     else
     {
       con->ogSpConnId = 0;
     }
     /* Bug 44374 End */
   }
   /* Bug 43938 */      

   /* Bug 79058 */
   if ((ccRelEvnt) && ((CC_GET_PROTOCOL(protType) == CC_SI) || (CC_GET_PROTOCOL(protType) == CC_EXT_BICC)))
   {
       handle_acc = ccRelEvnt->m.siRelEvnt.optElmnt.m.isup.handle_acc;
   }
   if (reEntryFlag && CC_IS_REMOTE_REL(relType) && handle_acc == FALSE)
   { /* In field flip the relType to local, else ABORT for tracking the Layer-3 issue */
     ABORT_DEBUG; 
     relType = CC_REL_NRML_LOC;
     if (logFirstRelInd)
     {    
        DP("** relType : %d **\n", relType);
     }     
   }

   /* FID 14886 */
   if((CC_GET_PROTOCOL(protType) == CC_SI) &&
       (ccRelEvnt->m.siRelEvnt.causeDgn.location.pres))
   {
       ccConvertIcLocation(con, &ccRelEvnt->m.siRelEvnt);
   }
   
   /* Bug 26758: mntRelProc is no more BOOL, it is U8, 
    * copy the reltype coming from Release indication and store it in mntRelProc
    * to be used later in the FSM to find out if the call is release by FORCE
    *
    * Also update the CDR to indicate the release location (relOrign) if the
    * release is happening for the first time
    *
    * RV Suresh, copy relType in mntRelProc 
    */
   CCDP(AIN_DBG_LEVEL_0, "CcLiCctRelInd: Received RelType = %d\n",relType);
   con->mntRelProc = relType;
   ccUpdateCallDtlInfoRelOrign(con,con->direction,relType);

   if (ccInit.acnt == TRUE)
   {
       ccUpdateCallDtlInfoRelCause (con, ccRelEvnt, relType);
   }
 
   /* Bugzilla 10706:taraveti memory leak fix */

#if 0	/* 44312+ */
   ccDropUBuf(&con->uBuf);
#else
   saveUBuf = con->uBuf;
#endif	/* 44312- */
   con->uBuf = uBuf;
   uBuf      = NULL; /* Bug 44218 */

   if (ccInit.acnt == TRUE)
   {
      /* Bug# 21586 */
      if ((con->callDtlInfo.cm.tckRelIndRcvd.pres == NOTPRSNT) ||
          ((con->callDtlInfo.cm.tckRelIndRcvd.pres == PRSNT_NODEF) &&
          isThisConInAOTCCall(con)))
      {
          ccSetRelIndTicks (con, ccRelEvnt);
      }
   }

/* TELICA-BUG:12976-mqin */
   ccChkCallTrc(con, CALLREL, con->callDtlInfo.cm.relCause.causeVal.val);
/* BUG:12976 */

   /* Internal BICC call processing */
   /* FID 14732 */
   if (CC_INGRESS_INTER_CCS_CALL(con) && (con->direction == INCTOOUT))
   {
      if (ccUnPkFwdRelInfoForInterCcsCall(con, ccCp.genCfg.ownCcsId,
                                                  (PTR)ccRelEvnt) != ROK)
      {
        CCLOGERR("Error: ccUnPkReleaseInfoForInterCcsCall failed\n");
      }
   }
   else if (CC_EGRESS_INTER_CCS_CALL(con) && (con->direction == OUTTOINC))
   {
      if (ccUnPkBkwdRelInfoForInterCcsCall(con, ccCp.genCfg.ownCcsId,
                             (PTR)ccRelEvnt, &con->interCcsLocalBusy) != ROK)
      {
        CCDP(AIN_DBG_LEVEL_0, "Error: ccUnPkReleaseInfoForInterCcsCall failed\n");
      }
   }

   /* Bug# 71626 - Save the real ic/og leg protocol of an inter-CCS call
                   for REL mapping purpose */
   CC_POPULATE_REAL_LEG_PROTTYPE(con);

   /* BUG:79251 set mntRelProc to local if internal error */
   if (CC_EGRESS_INTER_CCS_CALL(con) && (con->direction == OUTTOINC))
   {
      if (con->callDtlInfo.cm.relOrign == LCC_RELORGN_INTERNAL)
      {
         relType = CC_REL_NRML_LOC;
         con->mntRelProc = relType;
         CCDP(AIN_DBG_LEVEL_0, "CcLiCctRelInd: set relType %d\n", relType);
      }
   }    

#if 1 /* Debug */
   {
     CcCause cause;
     TknU16    sipCause;
     TknU16    sipStatus;

/* euysal - 09/19/2005 - Bug:40643 Use the stored protocol type */
#if 1
     ProtType       protocolType;

     if (con->direction == INCTOOUT)
        protocolType = con->icProtType;
     else
        protocolType = con->ogProtType;
     ccGetRelCause(con, protocolType, ccRelEvnt, &cause, &sipCause, &sipStatus);
#else
     ccGetRelCause(con, protType, ccRelEvnt, &cause, &sipCause, &sipStatus);
#endif     
     if ((cause.eh.pres == PRSNT_NODEF) &&
         (cause.causeVal.pres == PRSNT_NODEF) &&
         (cause.causeVal.val != CCCALLCLR) &&
         (cause.causeVal.val != CCNORMUNSPEC) && 
         ((logRelCause == 0) || ((logRelCause != 0) &&
          (cause.causeVal.val == logRelCause) && ((logRelType-1) == (relType >>4)))))
     {
      if (logFirstRelInd)
        DP("** Cause in : %d **\n", cause.causeVal.val);
      CC_TRACE_EVENTS(con, logFirstRelInd, CCE_RELIND);
     }  
     if (!logFirstRelInd)
     {
        logRelCause = 0;
        logRelType = 0;
     }
     CC_INC_REL_CAUSE_CNTR(cause.causeVal.val, relType);
     /* BUG 86074: update ingressSipStatusCode for reroute call*/
     if (con->callDtlInfo.cm.callState == LCC_CDR_UNANSWERED)
     {
         if(con->direction == INCTOOUT)
             con->callDtlInfo.cm.ingressSipStatusCode = sipStatus;
         else if(con->direction == OUTTOINC)
             con->callDtlInfo.cm.egressSipStatusCode = sipStatus;
         CCDP(AIN_DBG_LEVEL_0, "BUG86074:ingressSipStatusCode=%d\n", con->callDtlInfo.cm.ingressSipStatusCode.val);

         /* FID 14457.0 + */
         /* The getsSts.abandoned count is for both caller abandone and no answer from user  */
         /* Peg abandoned GETS calls counter if GETS is enabled and the call has not reached */
         /* the answer stage, either a SIP CANCEL from ingress, or ISUP REL from either side */
         /* with cause code of 18 (no user responding) or 19 (no answer from user).          */
         if((ccCp.getsEnabled) && (IS_THIS_CALL_GETS(con)))
         {
             if (((CC_GET_PROTOCOL(protocolType) == CC_SIP) ||
                  (CC_GET_PROTOCOL(protocolType) == CC_SIPT)) &&
                 (con->direction == INCTOOUT))
             {
                 ccCp.getsSts.abandoned++;
             }
             else if ((CC_GET_PROTOCOL(protocolType) == CC_SI)&&
                      (cause.eh.pres == PRSNT_NODEF) &&
                      (cause.causeVal.pres == PRSNT_NODEF) &&
                      ((cause.causeVal.val==CCNOUSRRSP)||(cause.causeVal.val==CCNOANSWR)))
             {
                 ccCp.getsSts.abandoned++;
             }
 
             
             if (con->direction == OUTTOINC)
             {
               /* Peg PM-GETS BUSY, ServiceUnavailableRx, TimeOutOfQueueRx counts*/
               switch (CC_GET_PROTOCOL(protocolType))
               {
                case CC_SIP:
                case CC_SIPT:
                  if (sipStatus.val < 400)
                  break;

                  switch (sipStatus.val)
                  {
                   case 486:
                   case 600:
                      ccCp.getsSts.busy++;
                      break;
                   case 408:
                      ccCp.getsSts.timedOutOfQueueRx++;
                      ccCp.getsSts.serviceUnavailRx++;
                      break;
                   default:
                      ccCp.getsSts.serviceUnavailRx++;
                      break;
                  }
                  break;

               case CC_SI:
                 if ( CC_CAUSE_IS_VAL(&cause, CCUSRBSY, CSTD_CCITT))
                 {
                   ccCp.getsSts.busy++;  /* cause: 17 */
                 }
                 else if ( !CC_CAUSE_IS_VAL(&cause, CCNOUSRRSP, CSTD_CCITT) &&
                           !CC_CAUSE_IS_VAL(&cause, CCNOANSWR, CSTD_CCITT) )
                 {
                   if (!handle_acc) /* Don't pegg it if released by ACC */
                      ccCp.getsSts.serviceUnavailRx++;  /* cause NOT 17,18,19 */
                 }
                 if ( CC_CAUSE_IS_VAL(&cause, CCNOCIRCUIT, CSTD_CCITT))
                 {
                   if (!handle_acc) /* Don't pegg it if released by ACC */
                      ccCp.getsSts.timedOutOfQueueRx++;  /* cause 34 */
                 }
                 break;
              default:
                break;
             }
           }
         }
         /* FID 14457.0 - */
     }
   }
#endif /* End Debug */
   /* Bug 44409 */
   if (con->direction == OUTTOINC && ((CC_GET_PROTOCOL(con->ogProtType) == CC_SIP) || 
       (CC_GET_PROTOCOL(con->ogProtType) == CC_SIPT)))
   {
       /* Bug# 87003 - Need to consider the scenario of SIPT redirect server */
       TknU16 *sipStatus = NULL;
       U8     *sip3xxHandling = NULL;
       

       if (CC_GET_PROTOCOL(con->ogProtType) == CC_SIP)
       {
         sipStatus = &ccRelEvnt->m.sipRelEvnt.sipStatus;

         if ((con->ogIntfcCb) && (con->ogIntfcCb->ccTgCb) && 
             (con->ogIntfcCb->ccTgCb->tgAtt.sipPrflPtr))
         {
           sip3xxHandling = &con->ogIntfcCb->ccTgCb->tgAtt.sipPrflPtr->sip3xxHandling;
         }
       }
       else
       {
         sipStatus = &ccRelEvnt->m.siRelEvnt.optElmnt.m.sipt.sipStatus;

         if ((con->ogIntfcCb) && (con->ogIntfcCb->ccTgCb) && 
             (con->ogIntfcCb->ccTgCb->tgAtt.sipTPrflPtr))
         {
           sip3xxHandling = &con->ogIntfcCb->ccTgCb->tgAtt.sipTPrflPtr->sip3xxHandling;
         }
       }

       /* For SIP to SIP call 302 release handling, if the SIP PROFILE
          configure to release , change the sip release status to 410 */
       if (sipStatus && (sipStatus->pres == PRSNT_NODEF) &&
           ((sipStatus->val == SIPCAUSE_301_MOVEDP) ||
            (sipStatus->val == SIPCAUSE_302_MOVEDT)) &&
            (sip3xxHandling && (*sip3xxHandling == LCC_3XX_RELEASE)))
       {
           sipStatus->val = SIPCAUSE_410_GONE;
           DP("** sipStatus changed to: %d for SIP 302 Handling **\n", sipStatus->val);
       }
   }
   /* End of Bug 44409 */
   /*Bug 58746: To identify "local SIPT 408 without reason header" and "remote SIPT 408 without reason header" in protrel table, we add a reason header with cause value 102 to local 408.*/ 
   if(relType==CC_REL_NRML_LOC)
   {
       if ( con->direction == OUTTOINC 
           && CC_GET_PROTOCOL(con->ogProtType) == CC_SIPT)
       {
           if (ccRelEvnt->m.siRelEvnt.optElmnt.m.sipt.sipStatus.pres == PRSNT_NODEF &&
               ccRelEvnt->m.siRelEvnt.optElmnt.m.sipt.sipStatus.val == SIPCAUSE_408_TIMEOUT)
           {
               if(ccRelEvnt->m.siRelEvnt.optElmnt.m.sipt.sipReason.pres == NOTPRSNT)
               {
                   ccRelEvnt->m.siRelEvnt.optElmnt.m.sipt.sipReason.pres = PRSNT_NODEF;
                   ccRelEvnt->m.siRelEvnt.optElmnt.m.sipt.sipReason.cause = CCTMRRECOV;
                   ccRelEvnt->m.siRelEvnt.optElmnt.m.sipt.sipReason.prtcl = SIP_REASON_PRTCL_Q850;
               }
           }
       }
   }
   /*End of Bug 58746*/

   /* FID 16706 AoC + */
   if ((CC_GET_PROTOCOL(protType) == CC_SIP) &&
        (con->direction == OUTTOINC) &&
                       (ccRelEvnt != NULLP) &&
                       (ccRelEvnt->m.sipRelEvnt.sipXmlInfo.xmlBody.pres == PRSNT_NODEF) &&
                       (ccRelEvnt->m.sipRelEvnt.sipXmlInfo.xmlBody.val != NULLP))
   {
      if (ccCpyFreeSipXmlInfo(con, &(ccRelEvnt->m.sipRelEvnt.sipXmlInfo)) != ROK)
      {
          RETVALUE(RFAILED);
      }
      CCDP(AIN_DBG_LEVEL_2, "FID 16706 CcLiCctRelInd %s free and copy sipXmlInfo \n", __FUNCTION__);
   }
   /* FID 16706 AOC - */

   /* call state machine function */
   CCDP(AIN_DBG_LEVEL_2, "%s:ccRelEvnt facility pres:%d.len:%d.\n",__FUNCTION__,
         ccRelEvnt->m.inDiscEvnt.facilityStr1.eh.pres,
         ccRelEvnt->m.inDiscEvnt.facilityStr1.facilityStr.len);

   ccCallStateMachine (con, CCE_RELIND, (PTR)ccRelEvnt, suId, spConnId,
                       (PTR) NULLP, (PTR) NULLP);
   ccFreeIntBiccMBufInTheEvnt(__FILE__, __LINE__, CCE_RELIND, 0, (PTR)ccRelEvnt,
                              protType);
   ccFreeSipSipTMBufInTheEvnt(__FILE__, __LINE__, CCE_RELIND, 0, (PTR)ccRelEvnt,
        protType); /* FID 14732 */
   ccFreeHistInfoEntriesInTheEvnt(__FILE__, __LINE__, CCE_RELIND, 0, (PTR)ccRelEvnt,
        protType); /*Bug 89424 */

   /* Bug 62895 During ISUP to ISUP call load test, core here due to accessing con.
    * The core is because the "con" is freed from hash list during ccCallStateMachine().
    * Now we have to check that and avoid using freed "con" again.
    */
   tmpCon = ccFindConFromSuConnId(tmpConnId);

   if(tmpCon == NULLP)
   {
       ret = cmHashListFind(&pCb->spInstTbl, (U8 *)&spConnId,
                            (U16)sizeof(UConnId), (U16)0, (PTR *) &tmpCon);
   }
   if( (tmpCon != NULLP) && (tmpCon == con) )
   {
       /* bug#35825, rkhan */
       if ((con->bufferBlock.bufferedEvent != NULLP) && (tmpDirection != DIR_UNKNOWN))
       {
           /* This event is buffered. So, revert the direction */
           DP("*** direction is reverted from %d to %d ****\n", con->direction,
              tmpDirection);
           con->direction = tmpDirection;
           con->uBuf = saveUBuf;
       }
       else
       {
           ccDropUBuf(&saveUBuf);
       }
   }
   else
   {
#if 1	/* 44312+ */
       ccDropUBuf(&saveUBuf);
#endif	/* 44312- */    
   }
   
#ifdef ZC
   zcUpdPeer();
#endif /* ZC */
   RETVALUE(ROK);
}


/*
*
*      Fun:   CcLiCctRelCfm
*
*      Desc:  process an incoming connection release confirm
*
*      Ret:   ROK     - successful,
*
*      Notes: None.
*
*      File:  cc_bdy1.c
*
*/

#ifdef ANSI
PUBLIC S16 CcLiCctRelCfm
(
Pst       *pst,               /* post */
SuId      suId,               /* service user identier */
UConnId   suConnId,           /* service user instance identifier */
UConnId   spConnId,           /* service provider instance identifier */
ProtType  protType,           /* incoming protocol type */
CcRelEvnt *ccRelEvnt,         /* connection release event structure */
Buffer    *uBuf               /* unrecognized parameters */
)
#else
PUBLIC S16 CcLiCctRelCfm(pst, suId, suConnId, spConnId, protType, 
                         ccRelEvnt, uBuf)
Pst       *pst;               /* post */
SuId      suId;               /* service user identier */
UConnId   suConnId;           /* service user instance identifier */
UConnId   spConnId;           /* service provider instance identifier */
ProtType  protType;           /* incoming protocol type */
CcRelEvnt *ccRelEvnt;         /* connection release event structure */
Buffer    *uBuf;              /* unrecognized parameter */
#endif
{
   CcPsSap *pCb;
   CcConCb *con;
   S16 ret;
   UConnId tmpConnId;

   TRC3( CcLiCctRelCfm)

#if 1 /* TELICA - Bugzilla-5520- Khan/Yakin - 08/01/2001 - code trace */
   TELICACODETRC(LOGDEBUG, ccCodeTrcLvl, ccInit.lmPst, ("CcLiCctRelCfm::spConnId=%lx, suConnId=%lx, protType=%d",spConnId, suConnId, protType));
#endif /* Bugzilla-5520 */


#if (ERRCLASS & ERRCLS_INT_PAR)
   if ((ccInit.cfgDone != TRUE) || (suId >= (SuId) ccCp.genCfg.maxPsSAP))
   {
      CCLOGERROR(ERRCLS_INT_PAR, ECC265, (ErrVal) suId,
                 "CcLiCctRelCfm() Failed, invalid suId");
      if (uBuf != NULLP)
         ccDropUBuf(&uBuf);
      ccFreeIntBiccMBufInTheEvnt(__FILE__, __LINE__, CCE_RELCFM, 0, (PTR)ccRelEvnt,
                              protType); 
      ccFreeSipSipTMBufInTheEvnt(__FILE__, __LINE__, CCE_RELIND, 0, (PTR)ccRelEvnt,
          protType); /* FID 14732 */
      RETVALUE(RFAILED);
   }
#endif

   if ((ccPsSapLstPtr == NULLP) || ((pCb = *(ccPsSapLstPtr + suId)) == NULLP))
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      CCLOGERROR(ERRCLS_INT_PAR, ECC266, (ErrVal) suId,
                 "CcLiCctRelCfm() Failed, invalid suId");
#endif
      ccGenAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT,
                 LCM_CAUSE_INV_SAP, (U8*) &suId);
      if (uBuf != NULLP)
         ccDropUBuf(&uBuf);
      ccFreeIntBiccMBufInTheEvnt(__FILE__, __LINE__, CCE_RELCFM, 0, (PTR)ccRelEvnt,
                              protType); 
      ccFreeSipSipTMBufInTheEvnt(__FILE__, __LINE__, CCE_RELIND, 0, (PTR)ccRelEvnt,
          protType); /* FID 14732 */
      RETVALUE(RFAILED);
   }

#if 0 /* cc028.15 : Change-old code */
   if (pCb->state != CC_BND)
#else /* cc028.15 : Change-new code */
   if (pCb->state == CC_UNBND)
#endif /* cc028.15 : Change */
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      CCLOGERROR(ERRCLS_INT_PAR, ECC267, (ErrVal) suId,
                 "CcLiCctRelCfm() Failed, SAP not bound");
#endif
      ccGenAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_INV_STATE,
                 LCM_CAUSE_INV_SAP, (U8*) &suId);
      if (uBuf != NULLP)
         ccDropUBuf(&uBuf);
      ccFreeIntBiccMBufInTheEvnt(__FILE__, __LINE__, CCE_RELCFM, 0, (PTR)ccRelEvnt,
                              protType); 
      ccFreeSipSipTMBufInTheEvnt(__FILE__, __LINE__, CCE_RELIND, 0, (PTR)ccRelEvnt,
          protType); /* FID 14732 */
      RETVALUE(RFAILED);
   }

   /*
    * Check to see if this RelInd is for a call Created Via Geo Audit or normal call
    * Geo Audit calls have bit<28> of the suConnId set
    */
   if (suConnId & CC_GEO_AUD_CON_FLG)
   {
      if (uBuf != NULLP)
         ccDropUBuf(&uBuf);

       if ((ret = CcGeoRelCfm (pst, suId, suConnId, spConnId,  protType, ccRelEvnt,uBuf)) != ROK)
       {
           RETVALUE(RFAILED);
       }
       else
       {
           RETVALUE(ROK);
       }
   }

   /* Find connection */
   /* Mask the least significant bit of suConnId before searching  for */
   /* the connection */

   tmpConnId = suConnId;

   ret = cmHashListFind (&ccSuInstTbl, (U8 *)&tmpConnId, (U16)sizeof(UConnId),
                         (U16)0, (PTR *) &con);

   if (ret != ROK) 
      ret = cmHashListFind(&ccSuOgInstTbl, (U8 *)&tmpConnId,
                        (U16)sizeof(UConnId), (U16)0, (PTR *) &con);

   if (ret != ROK)
      ret = cmHashListFind (&pCb->spInstTbl, (U8 *)&spConnId, 
                            (U16)sizeof(UConnId), (U16)0, (PTR *) &con);

   if (ret != ROK)
   {
      if (uBuf != NULLP)
         ccDropUBuf(&uBuf);
      ccFreeIntBiccMBufInTheEvnt(__FILE__, __LINE__, CCE_RELCFM, 0, (PTR)ccRelEvnt,
                              protType); 
      ccFreeSipSipTMBufInTheEvnt(__FILE__, __LINE__, CCE_RELIND, 0, (PTR)ccRelEvnt,
          protType); /* FID 14732 */
      RETVALUE(RFAILED);
   }
   /* call state machine function */
   con->direction = ccGetDirection(con, spConnId, suConnId, pCb);
   /* Bugzilla 10706:taraveti memory leak fix */
   ccDropUBuf(&con->uBuf);
   con->uBuf      = uBuf;
   uBuf           = NULL; /* Bug 44218 */

#if 1 /* TELICA-BUG:12976-mqin */
   ccChkCallTrc(con, CALLREL, con->callDtlInfo.cm.relCause.causeVal.val);
#endif /* BUG:12976 */

   /* Internal BICC call processing */
#if 0
   if (CC_INGRESS_INTER_CCS_CALL(con))
   {
      if (ccUnPkFwdRelInfoForInterCcsCall(con, ccCp.genCfg.ownCcsId, 
                                                  (PTR)ccRelEvnt) != ROK)
      {
        CCLOGERR("Error: ccUnPkReleaseInfoForInterCcsCall failed\n"); 
      } 
   }
#else
   if (CC_EGRESS_INTER_CCS_CALL(con) && (con->direction == OUTTOINC))
   {
      if (ccUnPkBkwdRelInfoForInterCcsCall(con, ccCp.genCfg.ownCcsId, 
                                                  (PTR)ccRelEvnt, NULLP) != ROK)
      {
        CCLOGERR("Error: ccUnPkReleaseInfoForInterCcsCall failed\n"); 
      } 
   }
#endif

   /* FID 16706 AoC + */
   if ((CC_GET_PROTOCOL(protType) == CC_SIP) &&
        (con->direction == OUTTOINC) &&
                       (ccRelEvnt != NULLP) &&
                       (ccRelEvnt->m.sipRelEvnt.sipXmlInfo.xmlBody.pres == PRSNT_NODEF) &&
                       (ccRelEvnt->m.sipRelEvnt.sipXmlInfo.xmlBody.val != NULLP))
   {
      if (ccCpyFreeSipXmlInfo(con, &(ccRelEvnt->m.sipRelEvnt.sipXmlInfo)) != ROK)
      {
          RETVALUE(RFAILED);
      }
      CCDP(AIN_DBG_LEVEL_2, "FID 16706 CcLiCctRelCfm %s free and copy sipXmlInfo \n", __FUNCTION__);
   }

   /* FID 16706 AOC - */
   
   ccCallStateMachine (con, CCE_RELCFM, (PTR)ccRelEvnt, suId, spConnId,
                       (PTR) NULLP, (PTR) NULLP);
   ccFreeIntBiccMBufInTheEvnt(__FILE__, __LINE__, CCE_RELCFM, 0, (PTR)ccRelEvnt,
                              protType); 
   ccFreeSipSipTMBufInTheEvnt(__FILE__, __LINE__, CCE_RELCFM, 0, (PTR)ccRelEvnt,
        protType); /* FID 14732 */
   
#ifdef ZC
   zcUpdPeer();
#endif /* ZC */
   RETVALUE(ROK);
}


/*
*
*       Fun:   CcLiCctStaInd                  
*
*       Desc:  This function is used to Indicate Network layer status.
*
*       Ret:   ROK   - ok
*
*       Notes: None
*
*       File:  cc_bdy1.c
*
*/
PUBLIC S16 CcLiCctStaInd
(
Pst       *pst,               /* post */
SuId      suId,               /* service user identier */
UConnId   suConnId,           /* service user instance identifier */
UConnId   spConnId,           /* service provider instance identifier */
U8        evntType,           /* Event Type */
RmRsc     *resource,          /* Resource information */
CcStaEvnt *staEvnt,           /* status Event */
Buffer    *uBuf
)
{
   CcPsSap            *pCb         = NULLP;
   CcConCb            *con         = NULLP;
   S16                 ret         = RFAILED;
   UConnId             tmpConnId   = 0;
   ProtType            protType    = CC_SIITU92;

#if 1 /* TELICA - Bugzilla-5520- Khan/Yakin - 08/01/2001 - code trace */
   TELICACODETRC(LOGDEBUG, ccCodeTrcLvl, ccInit.lmPst,
              ("CcLiCctStaInd::spConnId=%lx, protType=%x", spConnId, protType));
#endif /* Bugzilla-5520 */


#ifdef ZC
    /* 26928 */
    if (zcCb.protState != ACTIVE)
    {
        CCLOGERR("Ignoring %s in standby state.  evntType=%d,"
            " spConnId=0x%08lX\n", __FUNCTION__, evntType, spConnId);
        RETVALUE(RFAILED);
    }
#endif /* ZC */

#if (ERRCLASS & ERRCLS_INT_PAR)
   if ((ccInit.cfgDone != TRUE) || (suId >= (SuId) ccCp.genCfg.maxPsSAP))
   {
      CCLOGERROR(ERRCLS_INT_PAR, ECC269, (ErrVal) suId,
                 "CcLiCctStaInd() Failed, invalid suId");
      RETVALUE(RFAILED);
   }
#endif

   if ((ccPsSapLstPtr == NULLP) || ((pCb = *(ccPsSapLstPtr + suId)) == NULLP))
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      CCLOGERROR(ERRCLS_INT_PAR, ECC270, (ErrVal) suId,
                 "CcLiCctStaInd() Failed, invalid suId");
#endif
      ccGenAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT,
                 LCM_CAUSE_INV_SAP, (U8*) &suId);
      RETVALUE(RFAILED);
   }

   if (pCb->state != CC_BND)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      CCLOGERROR(ERRCLS_INT_PAR, ECC271, (ErrVal) suId,
                 "CcLiCctStaInd() Failed, SAP not bound");
#endif
      ccGenAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_INV_STATE,
                 LCM_CAUSE_INV_SAP, (U8*) &suId);
      RETVALUE(RFAILED);
   }

   /* Find connection */
   tmpConnId = suConnId;

   ret = cmHashListFind (&ccSuInstTbl, (U8 *)&tmpConnId, (U16)sizeof(UConnId),
                         (U16)0, (PTR *) &con);
   if (ret != ROK)
      ret = cmHashListFind(&ccSuOgInstTbl, (U8 *)&tmpConnId,
                        (U16)sizeof(UConnId), (U16)0, (PTR *) &con);

   if (ret != ROK)
      ret = cmHashListFind (&pCb->spInstTbl, (U8 *)&spConnId, 
                            (U16)sizeof(UConnId), (U16)0, (PTR *) &con);

   if (ret != ROK)
   {
      if ((evntType == CC_EVT_CONTCHK) || (evntType == CC_EVT_CONTCHKIAM) || (evntType == CC_EVT_CIRRSRVREQ) ||
          (evntType == CC_EVT_LOOPBACKACK))
      {
         if ((ret = ccInitCon(&con)) != ROK)
         {
#if (ERRCLASS & ERRCLS_ADD_RES)
            CCLOGERROR(ERRCLS_ADD_RES, ECC272, (ErrVal) ret,
                 "CcLiCctStaInd() Failed, ccInitCon failed");
#endif
            ccSendRelease(NULLP, pCb, spConnId, 0, protType, CCTMPFAIL, NULL);
            if (uBuf != NULLP)
              ccDropUBuf(&uBuf);
            RETVALUE(ROK);
         }

         con->icSuConnId   = ccGetNewSuConnId(); 
         con->origIcSuConnId = con->icSuConnId;

         con->ogSuConnId   = ccGetNewSuConnId();
         con->origOgSuConnId = con->ogSuConnId; 

         con->icSpConnId = spConnId;
         con->icProtType = protType;
         con->icSapCb    = pCb;
         con->icSapCb->nmbActvConn++; /* ravi */
         con->direction  = INCTOOUT;
         /* Bugzilla 10706:taraveti memory leak fix */
         ccDropUBuf(&con->uBuf);
         con->uBuf       = uBuf;


         /* FID14485 the sip call-id GCC generated is within 32(CC_SIP_ID_LENGTH_OLD) bytes presently,
          * if the call-id GCC generated is longer than 32  bytes,should change this to CC_SIP_ID_LENGTH(128) 
          * or more
          */
         ret = ccGetNewVarSipCallId(&(con->callDtlInfo.un.icSipId),
               CC_SIP_ID_LENGTH, con->icSuConnId, FALSE);
         if(ret != ROK) 
            CCDP(AIN_DBG_LEVEL_0,"Error in getting new sip call-id!!!");
         ret = ccGetNewVarSipCallId(&(con->callDtlInfo.un.ogSipId),
               CC_SIP_ID_LENGTH, con->ogSuConnId,TRUE);
         if(ret != ROK) 
            CCDP(AIN_DBG_LEVEL_0,"Error in getting new sip call-id!!!");


         /* If this is ISUP or BICC, look up the trunk group before getting interface */
         if ((resource->intfc.intfType == CC_SI_INTFC) || 
#if 0
             (resource->intfc.intfType == CC_BICC_INTFC))
#endif
             (resource->intfc.intfType == CC_EXT_BICC_INTFC))
         {
             if (rmGetNbTrkgrp(resource) != ROK)
             {
                 resource->intfc.t.tgrpDesc.tgrpId = 0;
             }
         }
         if ((ret = ccAssignSaps(con, &resource->intfc)) != ROK)
         {
           /* Added this check for #20582 */
           SPutSBuf(ccInit.region, ccInit.pool, con->routeInfo,
                 (Size)sizeof(CcRouteInfo));

           TELICA_FREE_CONNECTION (con);

           TELICACODETRC(LOGERROR, ccCodeTrcLvl, ccInit.lmPst,
               ("CcLiCctStaInd::spConnId=%lx, ccAssignSaps failed", spConnId));

           ccSendRelease(NULLP, pCb, spConnId, 0, protType, AM_CV_TMPFAIL,
               NULL);
           if (uBuf != NULLP)
             ccDropUBuf(&uBuf);
           RETVALUE(RFAILED);
         }

         cmMemcpy((U8 *) &con->icRsc, (U8 *)resource, sizeof(RmRsc));   
         /* Insert Connection CB in suConnId Hash Table */
         ret = cmHashListInsert(&ccSuInstTbl, (PTR)con, (U8 *)&con->icSuConnId,
                               (U16)sizeof(UConnId));
         if (ret != ROK)
         {
            SPutSBuf(ccInit.region, ccInit.pool, con->routeInfo,
                     (Size)sizeof(CcRouteInfo));
#if (ERRCLASS & ERRCLS_DEBUG)
            CCLOGERROR(ERRCLS_DEBUG, ECC273, (ErrVal) ret,
               "CcLiCctStaInd() Failed, cmHashListInsert failed in suInstTbl");
#endif
             ccGenAlarm(LCM_CATEGORY_INTERNAL, LCC_EVENT_HASHING_FAILED, 
                 LCC_CAUSE_SUINSTTBL_INS, (U8 *) &con->icSuConnId);
             ccSendRelease(NULLP, pCb, spConnId, 0, protType, CCTMPFAIL, NULL);
             TELICA_FREE_CONNECTION (con); /* Bug 78160 */
             if (uBuf != NULLP)
                ccDropUBuf(&uBuf);
             RETVALUE(RFAILED);
          }

          /* Insert Connection CB in spInstId Hash Table */
          ret = cmHashListInsert(&pCb->spInstTbl, (PTR)con, 
                                 (U8 *)&con->icSpConnId, (U16)sizeof(UConnId));
          if (ret != ROK)
          {
#if (ERRCLASS & ERRCLS_DEBUG)
             CCLOGERROR(ERRCLS_DEBUG, ECC274, (ErrVal) ret,
               "CcLiCctStaInd() Failed, cmHashListInsert failed in spInstTbl");
#endif
             ccGenAlarm(LCM_CATEGORY_INTERNAL, LCC_EVENT_HASHING_FAILED, 
                     LCC_CAUSE_SPINSTTBL_INS, (U8 *) &con->icSpConnId);
             cmHashListDelete(&ccSuInstTbl, (PTR)con);

             cmHashListDelete(&ccSuOgInstTbl, (PTR)con);



             SPutSBuf(ccInit.region, ccInit.pool, con->icProfile,
                      (Size)(sizeof(CcProfile)));
             SPutSBuf(ccInit.region, ccInit.pool, con->ogProfile,
                      (Size)(sizeof(CcProfile)));

             SPutSBuf(ccInit.region, ccInit.pool, con->routeInfo,
                      (Size)sizeof(CcRouteInfo));

             TELICA_FREE_CONNECTION (con);

             ccSendRelease(NULLP, pCb, spConnId, 0, protType, CCTMPFAIL, NULL);

             if (uBuf != NULLP)
               ccDropUBuf(&uBuf);

             RETVALUE(RFAILED);
          }
      } /* Evnt == CC_EVT_CONTCHK */
      else
      {
         ccGenAlarm(LCM_CATEGORY_INTERNAL, LCC_EVENT_HASHING_FAILED, 
                    LCC_CAUSE_SPINSTTBL_FIND, (U8 *) &spConnId);
         RETVALUE(RFAILED);
      }
   }

#if 1 /*BUG:20945*/
   con->icEvntType   = evntType;
   con->bkIcEvntType = evntType;
   con->direction = ccGetDirection(con, spConnId, suConnId, pCb);
   /* Store the spConnId on the outgoing interface */
   if ((con->direction == OUTTOINC) && (con->ogSpConnId == 0))
      con->ogSpConnId = spConnId;
#endif /*BUG:20945*/

#if 1 /* TELICA - Bugzilla-5520- Khan/Yakin - 08/01/2001 - code trace */
         TELICACODETRC(LOGDEBUG, ccCodeTrcLvl, ccInit.lmPst,
                       ("CcLiCctConInd::spConnId=%lx, suConnId=%lx, "
                        "evntType=%d", spConnId, suConnId, evntType));
#endif /* Bugzilla-5520 */

   CCDP(AIN_DBG_LEVEL_0, "Received EventType = %d for suId = 0x%x, "
        "spConnId = 0x%lx\n", evntType, suId, spConnId);
   switch (evntType)
   {
      /* FID 14382.15 */
      case CC_EVT_ICGLARE:
          pCb->sts.icglare++;
          con->callDtlInfo.cm.icglare++;
          CCDP(AIN_DBG_LEVEL_3,"CcLiCctStaInd:incoming glare pegged\n");
          break;
      case CC_EVT_OGGLARE:
          pCb->sts.ogglare++;
          con->callDtlInfo.cm.ogglare++;
          CCDP(AIN_DBG_LEVEL_3,"CcLiCctStaInd:outgoing glare pegged\n");
          break;
      /* FID 14382.15 */
      case CC_EVT_REATTEMPT:
         /* 26751 */
         ccDropUBuf(&con->uBuf);
         con->uBuf = uBuf;
         ccCallStateMachine (con, CCE_REATIND, (PTR)staEvnt, suId, spConnId,
                             (PTR) NULLP, (PTR) NULLP);
         break;
      case CC_EVT_RELCALL:
         /* Bug 26758: In case ISDN sends CC_EVT_RELCALL, it is because of internal error,
          * in this case update the CallDtlInfo if possible, so that the CDR reflects the 
          * correct Release Location.
          *
          * In case of CC_EVT_RELCALL, fake to the update function as if it is FORCE_LOC relType */

         ccUpdateCallDtlInfoRelOrign(con,con->direction,CC_REL_FORCE_LOC);
         ccCallStateMachine (con, CCE_INITRELIND, (PTR)staEvnt, suId, spConnId,
                             (PTR) NULLP, (PTR) NULLP);
         break;
#if 1 /*Bug: 37697-bcouto-06/10/2005*/
      case CC_EVT_CONTCHKIAM:
         /* This prevents duplicate icSp error during ConInd */
         con->ccrIAMflg = TRUE; 
#endif
      case CC_EVT_CONTCHK:
         ccCallStateMachine (con, CCE_CCR, (PTR)staEvnt, suId, spConnId,
                             (PTR) NULLP, (PTR) NULLP);
         break;
      case CC_EVT_CONTREP:
         ccCallStateMachine (con, CCE_CONTREP, (PTR)staEvnt, suId, spConnId,
                             (PTR) NULLP, (PTR) NULLP);
         break;
      case CC_EVT_STPCONTIN:
         ccCallStateMachine (con, CCE_STA_STPCONTIN, (PTR)staEvnt, suId,
                 spConnId, (PTR) NULLP, (PTR) NULLP);
         break;
      case CC_EVT_LOOPBACKACK:
         ccCallStateMachine (con, CCE_LOOPBACKACK, (PTR)staEvnt, suId,
                 spConnId, (PTR) NULLP, (PTR) NULLP);
         break;
      case CC_EVT_CIRRSRVACK:
         ccCallStateMachine (con, CCE_CRA, (PTR)staEvnt, suId, spConnId,
                             (PTR) NULLP, (PTR) NULLP);
         break;
      case CC_EVT_CIRRSRVREQ:
         ccCallStateMachine (con, CCE_STA_CRM, (PTR)staEvnt, suId, spConnId,
                             (PTR) NULLP, (PTR) NULLP);
         break;
      case CC_EVT_SETUP_AFTER_CRM:
      {
         CcConEvnt       *ccConEvnt = NULLP;         /* connection event structure */

         CCDP(AIN_DBG_LEVEL_2, "CcLiCctStaInd: Received IAM after CRM from peer, suId:%08X, spConnId:%08lX\n", suId, spConnId);

         /*
          * SETUP (IAM) after CRM forms InfoCollected and sends it to FIC.
          * Incoming resource has been allocated by CRM already.
          *
          * Incorporate CcLiCctConInd and ccConE00S00 code where necessary before 
          * calling state machine. This is because IAM after CRM does not go thru 
          * CcLiCctConInd. Instead, the IAM is sent by ISUP in a StaInd to GCC.
          * The SETUP is handled by ccConE00S27 (state COT_IC_DONE).
          */

         /* Incorporate CcLiCctConInd. */
         ccUpdSts(con->icSapCb, NULLP);

         /* Increment the call attempt statistics */
         if (con->icIntfcCb)
         {
            con->icIntfcCb->sts.numIcCallAttempt++;
         }
         if (staEvnt != NULLP)
         {
            ccConEvnt = (CcConEvnt *) &(staEvnt->siConEvnt);
            ccStoreEvnt(CCE_CONIND, 0, con, (U8 *) ccConEvnt, sizeof(CcConEvnt));
         }
	 /* Bug 45275 + */
	 con->icProtType = CC_SI | ccConEvnt->m.siConEvnt.optElmnt.isup.swtch.val;
	 CCDP(AIN_DBG_LEVEL_2, "CcLiCctStaInd: icProtType=0x%x => 0x%x\n", ccConEvnt->m.siConEvnt.optElmnt.isup.swtch.val, con->icProtType);

         ccBillUpdCdPtyNum (__FILE__, __LINE__, con);

         /* Update the lnp status in the connection control block */
         if (ccConEvnt->m.siConEvnt.fwdCallInd.transCallNInd.pres == PRSNT_NODEF)
         {
            if (ccConEvnt->m.siConEvnt.fwdCallInd.transCallNInd.val == TRUE)
            {
               con->lnp_status = LNP_TRANS_SOMEWHERE;
            }
         }

         ret = cmHashListInsert(&ccSuOgInstTbl, (PTR)con, (U8 *)&con->ogSuConnId,
                                (U16)sizeof(UConnId));
         if (ret != ROK)
         {
             ccDropEvnt(con);
             SPutSBuf(ccInit.region, ccInit.pool, con->routeInfo,
                      (Size)sizeof(CcRouteInfo));
#if (ERRCLASS & ERRCLS_DEBUG)
             CCLOGERROR(ERRCLS_DEBUG, ECC251, (ErrVal) ret,
                        "CcLiCctStaInd() Failed, cmHashListInsert failed in suOgInstTbl");
#endif

#if 1 /* TELICA - Bugzilla-5520- Khan/Yakin - 08/01/2001 - code trace */
             TELICACODETRC(LOGERROR, ccCodeTrcLvl, ccInit.lmPst, ("CcLiCctConInd::spConnId=%lx, ogSuConnId=%lx, cmHashListInsert failed in suOgInstTbl", spConnId, con->ogSuConnId));
#endif /* Bugzilla-5520 */

             ccGenAlarm(LCM_CATEGORY_INTERNAL, LCC_EVENT_HASHING_FAILED,
                        LCC_CAUSE_SUINSTTBL_INS, (U8 *) &con->ogSuConnId);
             cmHashListDelete(&ccSuInstTbl, (PTR)con);
             ccSendRelease(NULLP, pCb, spConnId, 0, protType, CCTMPFAIL, NULL);
             TELICA_FREE_CONNECTION (con); /* Bug 78160 */
             if (uBuf != NULLP)
                 ccDropUBuf(&uBuf);
#if 0
             ccFreeSdpInTheEvnt(__FILE__, __LINE__, CCE_CONIND, 0, (PTR)ccConEvnt,
                           protType);
#endif
             RETVALUE(RFAILED);
         }

         ccDropUBuf(&con->uBuf);
         con->uBuf = uBuf;

         TELICACODETRC(LOGDEBUG, ccCodeTrcLvl, ccInit.lmPst, ("CcLiCctConInd::ogSuConnId=%lx, icSuConnId=%lx",con->ogSuConnId, con->icSuConnId));

         /* Send to Call State machine */
         ccCallStateMachine (con, CCE_CONIND, (PTR)ccConEvnt, suId, spConnId,
                             (PTR) NULLP, (PTR) NULLP);

         break;
      }

      default:
         break;
   }

#ifdef ZC
   zcUpdPeer();
#endif /* ZC */
   RETVALUE(ROK);
}


/*
*
*       Fun:   CcLiCctRscCfm                  
*
*       Desc:  This function is used to indicated the outgoing resource
*              to Call Control if it is received in a backward message.
*
*       Ret:   ROK   - ok
*
*       Notes: None
*
*       File:  cc_bdy1.c
*
*/
 
#ifdef ANSI
PUBLIC S16 CcLiCctRscCfm
(
Pst       *pst,               /* post */
SuId      suId,               /* service user identier */
UConnId   suConnId,           /* service user instance identifier */
UConnId   spConnId,           /* service provider instance identifier */
RmRsc     *rsc                /* Resource  */
)
#else
PUBLIC S16 CcLiCctRscCfm(pst, suId, suConnId, spConnId, rsc)
Pst       *pst;               /* post */
SuId      suId;               /* service user identier */
UConnId   suConnId;           /* service user instance identifier */
UConnId   spConnId;           /* service provider instance identifier */
RmRsc     *rsc;               /* Resource  */
#endif
{
   CcPsSap *pCb;
   CcConCb *con;
   S16 ret;
   UConnId tmpConnId;

   TRC3( CcLiCctRscCfm)

#if 1 /* TELICA - Bugzilla-5520- Khan/Yakin - 08/01/2001 - code trace */
   TELICACODETRC(LOGDEBUG, ccCodeTrcLvl, ccInit.lmPst, ("CcLiCctRscCfm::spConnId=%lx, suConnId=%lx",spConnId, suConnId));
#endif /* Bugzilla-5520 */


#if (ERRCLASS & ERRCLS_INT_PAR)
   if ((ccInit.cfgDone != TRUE) || (suId >= (SuId) ccCp.genCfg.maxPsSAP))
   {
      CCLOGERROR(ERRCLS_INT_PAR, ECC276, (ErrVal) suId,
                 "CcLiCctRscCfm() Failed, invalid suId");
      RETVALUE(RFAILED);
   }
#endif

   if ((ccPsSapLstPtr == NULLP) || ((pCb = *(ccPsSapLstPtr + suId)) == NULLP))
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      CCLOGERROR(ERRCLS_INT_PAR, ECC277, (ErrVal) suId,
                 "CcLiCctRscCfm() Failed, invalid suId");
#endif
      ccGenAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT,
                 LCM_CAUSE_INV_SAP, (U8*) &suId);
      RETVALUE(RFAILED);
   }

   if (pCb->state != CC_BND)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      CCLOGERROR(ERRCLS_INT_PAR, ECC278, (ErrVal) suId,
                 "CcLiCctRscCfm() Failed, SAP not bound");
#endif
      ccGenAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_INV_STATE,
                 LCM_CAUSE_INV_SAP, (U8*) &suId);
      RETVALUE(RFAILED);
   }

   /* Find connection */
   /* Mask the least significant bit of suConnId before searching  for */
   /* the connection */

  tmpConnId = suConnId;


   ret = cmHashListFind (&ccSuInstTbl, (U8 *)&tmpConnId, (U16)sizeof(UConnId),
                         (U16)0, (PTR *) &con);

   if (ret != ROK) 
      ret = cmHashListFind(&ccSuOgInstTbl, (U8 *)&tmpConnId,
                        (U16)sizeof(UConnId), (U16)0, (PTR *) &con);

   if (ret != ROK)
   {
#if 0 /* cc017.15 : deletion */
#if (ERRCLASS & ERRCLS_DEBUG)
      CCLOGERROR(ERRCLS_DEBUG, ECC279, (ErrVal) ret,
         "CcLiCctRscCfm() Failed, cmHashListFind failed in suInstTbl");
#endif
#endif /* cc017.15 : deletion */
      ccGenAlarm(LCM_CATEGORY_INTERNAL, LCC_EVENT_HASHING_FAILED, 
                 LCC_CAUSE_SUINSTTBL_FIND, (U8 *) &suConnId);
      RETVALUE(RFAILED);
   }
   /* Store the spConnId on the outgoing interface */
   if (con->ogSpConnId == 0)
      con->ogSpConnId = spConnId;
   con->direction = OUTTOINC;
   /* call state machine function */
   ccCallStateMachine (con, CCE_RSCCFM, (PTR)rsc, suId, spConnId,
                       (PTR) NULLP, (PTR) NULLP);

#ifdef ZC
   zcUpdPeer();
#endif /* ZC */
   RETVALUE(ROK);
}



/*
*
*      Fun:   CcLiCctAddrInd
*
*      Desc:  Indication of subsequent address digits  
*
*      Ret:   ROK     - successful,
*
*      Notes: None.
*
*      File:  cc_bdy1.c
*
*/

#ifdef ANSI
PUBLIC S16 CcLiCctAddrInd
(
Pst        *pst,               /* post */
SuId       suId,               /* service user identier */
UConnId    suConnId,           /* service user instance identifier */
UConnId    spConnId,           /* service provider instance identifier */
ProtType   protType,           /* incoming protocol type */
CcCnStEvnt *ccCnStEvnt,        /* connection status event structure */
Buffer     *uBuf               /* unrecognized parameters */
)
#else
PUBLIC S16 CcLiCctAddrInd(pst, suId, suConnId, spConnId, protType, ccCnStEvnt, 
                          uBuf)
Pst        *pst;               /* post */
SuId       suId;               /* service user identier */
UConnId    suConnId;           /* service user instance identifier */
UConnId    spConnId;           /* service provider instance identifier */
ProtType   protType;           /* incoming protocol type */
CcCnStEvnt *ccCnStEvnt;        /* connection status event structure */
Buffer     *uBuf;              /* unrecognized parameter */
#endif
{
   CcPsSap *pCb;
   CcConCb *con;
   S16 ret;
   UConnId tmpConnId;
   ProtType genProtType;

   TRC3( CcLiCctAddrInd)

#if (ERRCLASS & ERRCLS_INT_PAR)
   if ((ccInit.cfgDone != TRUE) || (suId >= (SuId) ccCp.genCfg.maxPsSAP))
   {
      CCLOGERROR(ERRCLS_INT_PAR, ECC280, (ErrVal) suId,
                 "CcLiCctAddrInd() Failed, invalid suId");
      if (uBuf != NULLP)
         ccDropUBuf(&uBuf);
      RETVALUE(RFAILED);
   }
#endif

   if ((ccPsSapLstPtr == NULLP) || ((pCb = *(ccPsSapLstPtr + suId)) == NULLP))
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      CCLOGERROR(ERRCLS_INT_PAR, ECC281, (ErrVal) suId,
                 "CcLiCctAddrInd() Failed, invalid suId");
#endif
      ccGenAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT,
                 LCM_CAUSE_INV_SAP, (U8*) &suId);
      if (uBuf != NULLP)
         ccDropUBuf(&uBuf);
      RETVALUE(RFAILED);
   }

   if (pCb->state != CC_BND)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      CCLOGERROR(ERRCLS_INT_PAR, ECC282, (ErrVal) suId,
                 "CcLiCctAddrInd() Failed, SAP not bound");
#endif
      ccGenAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_INV_STATE,
                 LCM_CAUSE_INV_SAP, (U8*) &suId);
      if (uBuf != NULLP)
         ccDropUBuf(&uBuf);
      RETVALUE(RFAILED);
   }

   /* Find connection */
   /* Mask the least significant bit of suConnId before searching  for */
   /* the connection */

   tmpConnId = suConnId;


   ret = cmHashListFind (&ccSuInstTbl, (U8 *)&tmpConnId, (U16)sizeof(UConnId),
                         (U16)0, (PTR *) &con);

   if (ret != ROK) 
      ret = cmHashListFind(&ccSuOgInstTbl, (U8 *)&tmpConnId,
                        (U16)sizeof(UConnId), (U16)0, (PTR *) &con);

   if (ret != ROK)
      ret = cmHashListFind (&pCb->spInstTbl, (U8 *)&spConnId, 
                            (U16)sizeof(UConnId), (U16)0, (PTR *) &con);

   if (ret != ROK)
   {
#if 0 /* cc017.15 : deletion */
#if (ERRCLASS & ERRCLS_DEBUG)
      CCLOGERROR(ERRCLS_DEBUG, ECC283, (ErrVal) ret,
         "CcLiCctAddrInd() Failed, cmHashListFind failed in spInstTbl");
#endif
#endif /* cc017.15 : deletion */
      ccGenAlarm(LCM_CATEGORY_INTERNAL, LCC_EVENT_HASHING_FAILED, 
                 LCC_CAUSE_SPINSTTBL_FIND, (U8 *) &spConnId);
      if (uBuf != NULLP)
         ccDropUBuf(&uBuf);
      RETVALUE(RFAILED);
   }
   /* call state machine function */
   con->direction = INCTOOUT;
   /* Bugzilla 10706:taraveti memory leak fix */

   /* Bug: 31978 - Don't overwrite the Ubuf in case of ISUP SAM messages. because SAM cannot have Ubuf and it will overwrite
    * the Un recognised message in the IAM, Problem found during Telstra SQA testing
    *
    * RV Suresh, Bug: 31978 */
   genProtType = CC_GET_PROTOCOL(protType);
   if((genProtType == CC_SI) || (genProtType == CC_EXT_BICC) || (genProtType == CC_BICC))
   {
       /* don't overwrite the Ubuf for anything for ISUP - SAM case */
   }
   else
   {
       ccDropUBuf(&con->uBuf);
       con->uBuf      = uBuf;
   }


#if 1 /* TELICA - Bugzilla-5520- Khan/Yakin - 08/01/2001 - code trace */
   TELICACODETRC(LOGDEBUG, ccCodeTrcLvl, ccInit.lmPst, ("CcLiCctAddrInd::spConnId=%lx, suConnId=%lx, protType=%d", spConnId, suConnId, protType));
#endif /* Bugzilla-5520 */

   ccCallStateMachine (con, CCE_ADDRIND, (PTR)ccCnStEvnt, suId, spConnId,
                       (PTR) NULLP, (PTR) NULLP);
   
#ifdef ZC
   zcUpdPeer();
#endif /* ZC */
   RETVALUE(ROK);
}


/*
*
*       Fun:   CcLiCctMntStaInd                  
*
*       Desc:  This function is used to request Call control to forward 
*              the maintenance status to the given interface.
*       Ret:   ROK   - ok
*
*       Notes: None
*
*       File:  cc_bdy1.c
*
*/
 
#ifdef ANSI
PUBLIC S16 CcLiCctMntStaInd
(
Pst          *pst,               /* post */
SuId         suId,               /* service user identier */
U8           mntEvntType,        /* maintenance Event type */
CcMntStaEvnt *mntEvnt,           /* maintenance Event */
RmInterface  *rmIntfc            /* Associated interface */
)
#else
PUBLIC S16 CcLiCctMntStaInd(pst, suId, mntEvntType, mntEvnt, rmIntfc)
Pst          *pst;               /* post */
SuId         suId;               /* service user identier */
U8           mntEvntType;        /* maintenance Event type */
CcMntStaEvnt *mntEvnt;           /* maintenance Event */
RmInterface  *rmIntfc;           /* Associated interface */
#endif
{
   CcPsSap *pCb;
   CcIntfcSAPCb *icb;
   U8 intfcKey[CCMAXINTFCKEYLEN]; /* Key for the Hash list */
   S16 ret;

   TRC3( CcLiCctMntStaInd)

#if (ERRCLASS & ERRCLS_INT_PAR)
   if ((ccInit.cfgDone != TRUE) || (suId >= (SuId) ccCp.genCfg.maxPsSAP))
   {
      CCLOGERROR(ERRCLS_INT_PAR, ECC284, (ErrVal) suId,
                 "CcLiCctMntStaInd() Failed, invalid suId");
      RETVALUE(RFAILED);
   }
#endif

   if ((ccPsSapLstPtr == NULLP) || ((pCb = *(ccPsSapLstPtr + suId)) == NULLP))
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      CCLOGERROR(ERRCLS_INT_PAR, ECC285, (ErrVal) suId,
                 "CcLiCctMntStaInd() Failed, invalid suId");
#endif
      ccGenAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT,
                 LCM_CAUSE_INV_SAP, (U8*) &suId);
      RETVALUE(RFAILED);
   }

   if (pCb->state != CC_BND)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      CCLOGERROR(ERRCLS_INT_PAR, ECC286, (ErrVal) suId,
                 "CcLiCctMntStaInd() Failed, SAP not bound");
#endif
      ccGenAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_INV_STATE,
                 LCM_CAUSE_INV_SAP, (U8*) &suId);
      RETVALUE(RFAILED);
   }

   switch (ccGetMntEvntClass(mntEvntType))
   {
      case CC_SNGL_MNTEVNT:
      case CC_GRP_MNTEVNT:
         /* prepare key for searching for the associted interface  */
         switch (rmIntfc->intfType)
         {
            case  CC_SI_INTFC:

#if 1 /* TELICA - Bugzilla-5520- Khan/Yakin - 08/01/2001 - code trace */
               TELICACODETRC(LOGDEBUG, ccCodeTrcLvl, ccInit.lmPst, ("CcLiCctMntStaInd::mntEvntType=%d, IntfType=SS7, IntfcId=%ld-%ld-%ld", mntEvntType, (rmIntfc->t.intfcId & 0x00ff0000) >> 16, (rmIntfc->t.intfcId & 0x0000ff00) >> 8, rmIntfc->t.intfcId & 0x000000ff ));
#endif /* Bugzilla-5520 */

               CCGET_INTFCKEY(intfcKey, rmIntfc->intfType, rmIntfc->t.tgrpDesc.tgrpId);
               break;
            case CC_IN_INTFC:

#if 1 /* TELICA - Bugzilla-5520- Khan/Yakin - 08/01/2001 - code trace */
               TELICACODETRC(LOGDEBUG, ccCodeTrcLvl, ccInit.lmPst, ("CcLiCctMntStaInd::mntEvntType=%d, IntfType=ISDN, IntfcId=%ld", mntEvntType, rmIntfc->t.intfcId));
#endif /* Bugzilla-5520 */

               CCGET_INTFCKEY(intfcKey, rmIntfc->intfType, rmIntfc->t.intfcId);
               break;
            default:
               CCGET_INTFCKEY(intfcKey, rmIntfc->intfType, rmIntfc->t.intfcId);
               break;
         }

         /* Find connection */
         ret = cmHashListFind (&ccIntfcSAPTbl, (U8 *)&intfcKey[0], 
                               CCMAXINTFCKEYLEN, (U16)0, (PTR *) &icb);
         if (ret != ROK)
         {
            ccGenAlarm(LCM_CATEGORY_INTERNAL, LCC_EVENT_HASHING_FAILED, 
                       LCC_CAUSE_INTFCSAPTBL_FIND, (U8 *)rmIntfc);
            RETVALUE(RFAILED);
         }

         /* find the sap control block for the associated interface */
         if (((pCb = *(ccPsSapLstPtr + icb->destSAPId)) == NULLP) || 
              (pCb->state != CC_BND))
         {
#if (ERRCLASS & ERRCLS_DEBUG)
            CCLOGERROR(ERRCLS_DEBUG, ECC288, (ErrVal) ret,
                       "CcLiCctMntStaInd() Failed, invalid Destination SAP Id");
#endif
            ccGenAlarm(LCM_CATEGORY_INTERNAL, LCC_EVENT_INV_DESTSAP, 
                       LCM_CAUSE_INV_SAP, (U8 *) &icb->destSAPId);
            RETVALUE(RFAILED);
         }
   
         /* forward the maintenance indication to associated interface */
         CcLiCctMntStaReq (&pCb->pst, pCb->spId, mntEvntType, mntEvnt); 
   
         break;
      case CC_RT_AUDEVNT:
         CCLOGERR("%s: ** This functionality is not supported *****\n", __FUNCTION__);
         RETVALUE(ROK);
         break;
      case CC_OTHER_MNTEVNT :
         if((mntEvntType != CC_ME_INTFC_UNAVAIL) &&
            (mntEvntType != CC_ME_INTFC_AVAIL))
           CCLOGERR("%s: ** This functionality is not supported *****\n",
               __FUNCTION__);
         break;
      default:
#if (ERRCLASS & ERRCLS_DEBUG)
         CCLOGERROR(ERRCLS_DEBUG, ECC290, (ErrVal) mntEvntType,
                   "CcLiCctMntStaInd() Failed, invalid Maintenance Event Type");
#endif
         break;

   }

#ifdef ZC
   zcUpdPeer();
#endif /* ZC */
   RETVALUE(ROK);
}

/*
*
*      Fun:   CcLiCctAddrCfm
*
*      Desc:  Indication of subsequent address digits  
*
*      Ret:   ROK     - successful,
*
*      Notes: None.
*
*      File:  cc_bdy1.c
*
*/

#ifdef ANSI
PUBLIC S16 CcLiCctAddrCfm
(
Pst        *pst,               /* post */
SuId       suId,               /* service user identier */
UConnId    suConnId,           /* service user instance identifier */
UConnId    spConnId,           /* service provider instance identifier */
ProtType   protType,           /* incoming protocol type */
CcAddrCfmEvnt *ccAddrCfmEvnt   /* address event structure */
)
#else
PUBLIC S16 CcLiCctAddrCfm(pst, suId, suConnId, spConnId, protType, ccAddrCfmEvnt)
Pst        *pst;               /* post */
SuId       suId;               /* service user identier */
UConnId    suConnId;           /* service user instance identifier */
UConnId    spConnId;           /* service provider instance identifier */
ProtType   protType;           /* incoming protocol type */
CcAddrCfmEvnt *ccAddrCfmEvnt;  /* address event structure */
#endif
{
   CcPsSap *pCb;
   CcConCb *con;
   S16 ret;
   UConnId tmpConnId;

   TRC3( CcLiCctAddrCfm)

#if (ERRCLASS & ERRCLS_INT_PAR)
   if ((ccInit.cfgDone != TRUE) || (suId >= (SuId) ccCp.genCfg.maxPsSAP))
   {
      CCLOGERROR(ERRCLS_INT_PAR, ECC984, (ErrVal) suId,
                 "CcLiCctAddrCfm() Failed, invalid suId");
      ccFreeSdpInTheEvnt(__FILE__, __LINE__, CCE_ADDRCFM, 0,
          (PTR)ccAddrCfmEvnt, protType);
      RETVALUE(RFAILED);
   }
#endif

   if ((ccPsSapLstPtr == NULLP) || ((pCb = *(ccPsSapLstPtr + suId)) == NULLP))
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      CCLOGERROR(ERRCLS_INT_PAR, ECC985, (ErrVal) suId,
                 "CcLiCctAddrCfm() Failed, invalid suId");
#endif
      ccGenAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT,
                 LCM_CAUSE_INV_SAP, (U8*) &suId);
      ccFreeSdpInTheEvnt(__FILE__, __LINE__, CCE_ADDRCFM, 0,
          (PTR)ccAddrCfmEvnt, protType);
      RETVALUE(RFAILED);
   }

   if (pCb->state != CC_BND)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      CCLOGERROR(ERRCLS_INT_PAR, ECC986, (ErrVal) suId,
                 "CcLiCctAddrCfm() Failed, SAP not bound");
#endif
      ccGenAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_INV_STATE,
                 LCM_CAUSE_INV_SAP, (U8*) &suId);
      ccFreeSdpInTheEvnt(__FILE__, __LINE__, CCE_ADDRCFM, 0,
          (PTR)ccAddrCfmEvnt, protType);
      RETVALUE(RFAILED);
   }
     
   /* Find connection */
   /* Mask the least significant bit of suConnId before searching  for */
   /* the connection */

  tmpConnId = suConnId;


   ret = cmHashListFind (&ccSuInstTbl, (U8 *)&tmpConnId, (U16)sizeof(UConnId),
                         (U16)0, (PTR *) &con);

   if (ret != ROK) 
      ret = cmHashListFind(&ccSuOgInstTbl, (U8 *)&tmpConnId,
                        (U16)sizeof(UConnId), (U16)0, (PTR *) &con);

   if (ret != ROK)
      ret = cmHashListFind (&pCb->spInstTbl, (U8 *)&spConnId, 
                            (U16)sizeof(UConnId), (U16)0, (PTR *) &con);

   if (ret != ROK)
   {
      ccGenAlarm(LCM_CATEGORY_INTERNAL, LCC_EVENT_HASHING_FAILED, 
                 LCC_CAUSE_SPINSTTBL_FIND, (U8 *) &spConnId);
      ccFreeSdpInTheEvnt(__FILE__, __LINE__, CCE_ADDRCFM, 0,
          (PTR)ccAddrCfmEvnt, protType);
      RETVALUE(RFAILED);
   }
  
   if(ccAddrCfmEvnt == NULLP)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      CCLOGERROR(ERRCLS_INT_PAR, ECC986, (ErrVal) suId,
                 "CcLiCctAddrCfm() Failed, ccAddrCfmEvnt is NULL\n");
#endif
     RETVALUE(RFAILED);
   }

   /* copy addrCfm to evntHold */
   ccStoreEvnt(CCE_ADDRCFM, 0, con, (U8 *)ccAddrCfmEvnt, sizeof(CcAddrCfmEvnt));

   /* call state machine function */
   con->direction = INCTOOUT;
   ccCallStateMachine (con, CCE_ADDRCFM, (PTR)ccAddrCfmEvnt, suId, spConnId,
                       (PTR) NULLP, (PTR) NULLP);
   ccFreeSdpInTheEvnt(__FILE__, __LINE__, CCE_ADDRCFM, 0,
          (PTR)ccAddrCfmEvnt, protType);
#ifdef ZC
   zcUpdPeer();
#endif /* ZC */
   RETVALUE(ROK);
}  /* end of CcLiCctAddrCfm */


/*
*
*      Fun:   CcLiCctMidCallCfm
*
*      Desc:  Indication of subsequent address digits  
*
*      Ret:   ROK     - successful,
*
*      Notes: None.
*
*      File:  cc_bdy1.c
*
*/

#ifdef ANSI
PUBLIC S16 CcLiCctMidCallCfm
(
Pst        *pst,               /* post */
SuId       suId,               /* service user identier */
UConnId    suConnId,           /* service user instance identifier */
UConnId    spConnId,           /* service provider instance identifier */
ProtType   protType,           /* incoming protocol type */
CcMidCallCfmEvnt *ccMidCallCfmEvnt   /* midcall event structure */
)
#else
PUBLIC S16 CcLiCctMidCallCfm(pst, suId, suConnId, spConnId, protType, ccMidCallCfmEvnt)
Pst        *pst;               /* post */
SuId       suId;               /* service user identier */
UConnId    suConnId;           /* service user instance identifier */
UConnId    spConnId;           /* service provider instance identifier */
ProtType   protType;           /* incoming protocol type */
CcMidCallCfmEvnt *ccMidCallCfmEvnt;  /* midcall event structure */
#endif
{
   CcPsSap *pCb;
   CcConCb *con;
   S16 ret;
   UConnId tmpConnId;

   TRC3( CcLiCctMidCallCfm)

#if (ERRCLASS & ERRCLS_INT_PAR)
   if ((ccInit.cfgDone != TRUE) || (suId >= (SuId) ccCp.genCfg.maxPsSAP))
   {
      CCLOGERROR(ERRCLS_INT_PAR, ECC987, (ErrVal) suId,
                 "CcLiCctMidCallCfm() Failed, invalid suId");
      RETVALUE(RFAILED);
   }
#endif

   if ((ccPsSapLstPtr == NULLP) || ((pCb = *(ccPsSapLstPtr + suId)) == NULLP))
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      CCLOGERROR(ERRCLS_INT_PAR, ECC988, (ErrVal) suId,
                 "CcLiCctMidCallCfm() Failed, invalid suId");
#endif
      ccGenAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT,
                 LCM_CAUSE_INV_SAP, (U8*) &suId);
      RETVALUE(RFAILED);
   }

   if (pCb->state != CC_BND)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      CCLOGERROR(ERRCLS_INT_PAR, ECC989, (ErrVal) suId,
                 "CcLiCctMidCallCfm() Failed, SAP not bound");
#endif
      ccGenAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_INV_STATE,
                 LCM_CAUSE_INV_SAP, (U8*) &suId);
      RETVALUE(RFAILED);
   }
     
   /* Find connection */
   /* Mask the least significant bit of suConnId before searching  for */
   /* the connection */

   tmpConnId = suConnId;

   ret = cmHashListFind (&ccSuInstTbl, (U8 *)&tmpConnId, (U16)sizeof(UConnId),
                         (U16)0, (PTR *) &con);

   if (ret != ROK) 
      ret = cmHashListFind(&ccSuOgInstTbl, (U8 *)&tmpConnId,
                        (U16)sizeof(UConnId), (U16)0, (PTR *) &con);

   if (ret != ROK)
      ret = cmHashListFind (&pCb->spInstTbl, (U8 *)&spConnId, 
                            (U16)sizeof(UConnId), (U16)0, (PTR *) &con);

   if (ret != ROK)
   {
      ccGenAlarm(LCM_CATEGORY_INTERNAL, LCC_EVENT_HASHING_FAILED, 
                 LCC_CAUSE_SPINSTTBL_FIND, (U8 *) &spConnId);
      RETVALUE(RFAILED);
   }

   if(ccMidCallCfmEvnt == NULLP)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      CCLOGERROR(ERRCLS_INT_PAR, ECC986, (ErrVal) suId,
                 "CcLiCctMidCallCfm() Failed, ccMidCallCfmEvnt is NULL\n");
#endif
     RETVALUE(RFAILED);
   }
   
   /* call state machine function */
   con->direction = INCTOOUT;
   ccCallStateMachine (con, CCE_MIDCALLCFM, (PTR)ccMidCallCfmEvnt, suId, 
           spConnId, (PTR) NULLP, (PTR) NULLP);

#ifdef ZC
   zcUpdPeer();
#endif /* ZC */
   RETVALUE(ROK);
}  /* end of CcLiCctMidCallCfm */

  
/*
*     Chapter 3: interface functions to Resource Manager Service Provider
*/


/*
*
*      Fun:   CcLiRmtBndCfm
*
*      Desc:  Confirmation from Resource Manager of the Bind Request Issued
*
*      Ret:   ROK     - successful
*
*      Notes: None.
*
*      File:  cc_bdy1.c
*
*/
 
#ifdef ANSI
PUBLIC S16 CcLiRmtBndCfm
(
Pst *pst,                      /* post */
SuId suId,                     /* Service User SAP ID */
U8   status                    /* Bind Status */
)
#else
PUBLIC S16 CcLiRmtBndCfm(pst, suId, status)
Pst *pst;                      /* post */
SuId suId;                     /* Service User SAP ID */
U8   status;                   /* Bind Status */
#endif
{
   CcGenSap *rmCb;
#ifdef ZC
   ZcUpdParam upd;
#endif /* ZC */

   TRC3(CcLiRmtBndCfm)

#if (ERRCLASS & ERRCLS_INT_PAR)
   if ((ccInit.cfgDone != TRUE) || (suId >= (SuId) ccCp.genCfg.maxRmSAP))
   {
      CCLOGERROR(ERRCLS_INT_PAR, ECC291, (ErrVal) suId,
                 "CcLiRmtBndCfm() Failed, invalid suId");
      RETVALUE(RFAILED);
   }
#endif

   if ((ccRmSapLstPtr == NULLP) || ((rmCb = *(ccRmSapLstPtr + suId)) == NULLP))
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      CCLOGERROR(ERRCLS_INT_PAR, ECC292, (ErrVal) suId,
                 "CcLiRmtBndCfm() Failed, invalid suId");
#endif
      ccGenAlarm(LCM_CATEGORY_INTERNAL, LCC_EVENT_INV_RMSAP, 
                 LCM_CAUSE_INV_SAP, (U8 *) &suId);
      RETVALUE(RFAILED);
   }

   ccStopSapTmr((PTR)rmCb, TMR_RMSAPBNDCFM);

   /* if waiting for Bind Confirm and Bind Successful */
   if ((rmCb->state == CC_BND_CONT) && (status == RMT_BND_OK))
   {
      rmCb->bndRetryCnt = 0;
      rmCb->state = CC_BND;
#ifdef ZC
      /* UPD RMSAP */
      upd.gensap = rmCb;
      zcRunTimeUpd(ZC_RMSAP_CB, CMPFTHA_UPD_REQ, upd);
#endif
      
   } /* Successful Bind Confirm */
   
   /* AAG Sip Normalization */
   /* 
    * Save the SAP data for later use when sending the MonitorForChgCfm
    * back to FIC
    */
   ccStsChgSap = rmCb;

   if (status == RMT_BND_FAILED)
   {
      ccGenAlarm(LCM_CATEGORY_PROTOCOL, LCM_EVENT_BND_FAIL, 
                 LCM_CAUSE_UNKNOWN, NULLP);
      rmCb->state = CC_UNBND;
   }
   
#ifdef ZC
   zcUpdPeer();
#endif /* ZC */
   RETVALUE(ROK);

} /* end of CcLiRmtBndCfm */


/*
*
*      Fun:   CcLiRmtAlocCfm
*
*      Desc:  Returns the Resource managers output
*
*      Ret:   ROK     - successful,
*
*      Notes: None.
*
*      File:  cc_bdy1.c
*
*/

#ifdef ANSI
PUBLIC S16 CcLiRmtAlocCfm
(
Pst    *pst,             /* post */
SuId   suId,             /* service user id */
UConnId suConnId,         /* Connection id */
U8     rscSta1,          /* status of resource 1 */
RmRsc  *rsc1,            /* resource 1 */
U8     rscSta2,          /* status of resource 2 */
RmRsc  *rsc2,            /* resource 2 */
U8     staticBnd,        /* Static Binding */
RmTfcDesc   *tfcDsc,     /* Traffic Descriptor */
CmProfile *profile,
RmAddnlRscInfo *addnlRscInfo,
Buffer *mBuf
)
#else
PUBLIC S16 CcLiRmtAlocCfm(pst, suId, suConnId, rscSta1, rsc1, rscSta2, rsc2,
                          staticBnd, tfcDsc, profile, addnlRscInfo, mBuf)
Pst    *pst;             /* post */
SuId   suId;             /* service user id */
UConnId suConnId;         /* Connection id */
U8     rscSta1;          /* status of resource 1 */
RmRsc  *rsc1;            /* resource 1 */
U8     rscSta2;          /* status of resource 2 */
RmRsc  *rsc2;            /* resource 2 */
U8     staticBnd;        /* Static Binding */
RmTfcDesc   *tfcDsc;     /* Traffic Descriptor */
CmProfile *profile;
RmAddnlRscInfo *addnlRscInfo;
Buffer *mBuf;
#endif
{
   S16 ret;
   CcGenSap *rmCb;
   CcConCb  *con;
   RmAlocCfmEvnt alocCfmEvnt;
   UConnId tmpConnId;
   CcGeoRscEntry_t *geoRsc=NULL;
   CcGeoCbEvent_t geoAlocCfmEvnt;


   TRC3(CcLiRmtAlocCfm)

#if 1 /* TELICA - Bugzilla-5520- Khan/Yakin - 08/01/2001 - code trace */
   TELICACODETRC(LOGDEBUG, ccCodeTrcLvl, ccInit.lmPst, ("CcLiRmtAlocCfm::suConnId=%lx",suConnId));
#endif /* Bugzilla-5520 */


   /* this is a Geo Audit resource - find the context block */
   if (suConnId & CC_GEO_AUD_CON_FLG)
   { 
        ret = cmHashListFind (&ccGeoSuInstTbl, (U8 *)&suConnId, 
                (U16)sizeof(UConnId), (U16)0, (PTR *) &geoRsc);

        if (ret != ROK)
        {
            ccGenAlarm(LCM_CATEGORY_INTERNAL, LCC_EVENT_HASHING_FAILED, 
                       LCC_CAUSE_SUINSTTBL_FIND, (U8 *) &suConnId);
            RETVALUE(RFAILED);
        }
        geoAlocCfmEvnt.type = GCE_RMT_ALOC_CFM;
        geoAlocCfmEvnt.u.rmAloc.rsc = geoRsc;
        geoAlocCfmEvnt.u.rmAloc.status = rscSta1;
        if (rsc1)
          geoRsc->rsc = *rsc1;
        
        ccGeoCallStateMachine(geoRsc->con, &geoAlocCfmEvnt);
        RETVALUE(ROK);
   }
   else /* proceed with the generic resource alloc confirm */
   {
       if ((ret = ccGetRmSap(&rmCb, NULLP,  suId)) != ROK)
       {
#if (ERRCLASS & ERRCLS_DEBUG)
          CCLOGERROR(ERRCLS_DEBUG, ECC293, (ErrVal) ret,
                     "CcLiRmtAlocCfm() Failed, ccGetRmSap failed");
#endif
          ccGenAlarm(LCM_CATEGORY_INTERNAL, LCC_EVENT_INV_RMSAP, 
                    LCM_CAUSE_INV_SAP, (U8 *) &suId);
          RETVALUE(RFAILED);
       }
    
       /* Find connection */
       /* Mask the least significant bit of suConnId before searching  for */
       /* the connection */
    
       tmpConnId = suConnId;
    
       ret = cmHashListFind (&ccSuInstTbl, (U8 *)&tmpConnId, (U16)sizeof(UConnId),
                             (U16)0, (PTR *) &con);
       if (ret != ROK) 
          ret = cmHashListFind(&ccSuOgInstTbl, (U8 *)&tmpConnId,
                            (U16)sizeof(UConnId), (U16)0, (PTR *) &con);
    
       if (ret != ROK)
       {
#if 0 /* cc017.15 : deletion */
#if (ERRCLASS & ERRCLS_DEBUG)
          CCLOGERROR(ERRCLS_DEBUG, ECC294, (ErrVal) ret,
             "CcLiRmtAlocCfm() Failed, cmHashListFind failed in suInstTbl");
#endif
#endif /* cc017.15 : deletion */
          ccGenAlarm(LCM_CATEGORY_INTERNAL, LCC_EVENT_HASHING_FAILED, 
                     LCC_CAUSE_SUINSTTBL_FIND, (U8 *) &suConnId);
          RETVALUE(RFAILED);
       }
    
       /*Prepare routCfm event and call state machine function */
       alocCfmEvnt.rscSta1 = rscSta1;
       alocCfmEvnt.rscSta2 = rscSta2;
       alocCfmEvnt.rsc1 = rsc1;
       alocCfmEvnt.rsc2 = rsc2;
       alocCfmEvnt.tfcDesc = tfcDsc;
    
       alocCfmEvnt.addnlRscInfo = addnlRscInfo;
       alocCfmEvnt.profile      = profile;
       alocCfmEvnt.mBuf         = mBuf;
    
       /* Following parametrs make no sense in regular RmtAlocCfm but 2wayPriAloc */
       alocCfmEvnt.ogPrtclType = NULLP;
#if 0
       alocCfmEvnt.dgtStrp = NULLP;
       alocCfmEvnt.dgtsPrefix = NULLP;
#endif
    
       /* Send to Call State machine */
       ccCallStateMachine (con, CCE_ALOCCFM, (PTR) &alocCfmEvnt, suId, suConnId,
                           (PTR) NULLP, (PTR) NULLP);
    
       if (alocCfmEvnt.mBuf)
       {
          SPutMsg(alocCfmEvnt.mBuf);
       }
    
#ifdef ZC
       zcUpdPeer();
#endif /* ZC */
   }

   RETVALUE(ROK);

} /* end of CcLiRmtAlocCfm */


/*
*
*      Fun:   CcLiRmtAloc2WayPriCfm
*
*      Desc:  Returns the Resource managers output -- 2 WAY PRI specific 
*
*      Ret:   ROK     - successful,
*
*      Notes: None.
*
*      File:  cc_bdy1.c
*
*/

#ifdef ANSI
PUBLIC S16 CcLiRmtAloc2WayPriCfm
(
Pst    *pst,             /* post */
SuId   suId,             /* service user id */
UConnId suConnId,         /* Connection id */
U8     rscSta1,          /* status of resource 1 */
RmRsc  *rsc1,            /* resource 1 */
U8     staticBnd,        /* Static Binding */
RmTfcDesc   *tfcDsc,     /* Traffic Descriptor */
CmProfile *profile,
ProtType  *ogPrtclType,
RmAddnlRscInfo *addnlRscInfo
)
#else
PUBLIC S16 CcLiRmtAloc2WayPriCfm(pst, suId, suConnId, rscSta1, rsc1,
                          staticBnd, tfcDsc, profile,
                          ogPrtclType, dgtStrp, dgtsPrefix, addnlRscInfo)
Pst    *pst;             /* post */
SuId   suId;             /* service user id */
UConnId suConnId;         /* Connection id */
U8     rscSta1;          /* status of resource 1 */
RmRsc  *rsc1;            /* resource 1 */
U8     staticBnd;        /* Static Binding */
RmTfcDesc   *tfcDsc;     /* Traffic Descriptor */
CmProfile *profile;
ProtType  *ogPrtclType;
RtDgtsStrpInfo    *dgtStrp;
RtDgtsPrefixInfo  *dgtsPrefix;
RmAddnlRscInfo *addnlRscInfo;
#endif
{
   S16 ret;
   CcGenSap *rmCb;
   CcConCb  *con;
   RmAlocCfmEvnt alocCfmEvnt;
   UConnId tmpConnId;

   TRC3(CcLiRmtAloc2WayPriCfm)

#if 1 
   TELICACODETRC(LOGDEBUG, ccCodeTrcLvl, ccInit.lmPst,
         ("CcLiRmtAloc2WayPriCfm::suConnId=%lx",suConnId));
#endif

   if ((ret = ccGetRmSap(&rmCb, NULLP,  suId)) != ROK)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      CCLOGERROR(ERRCLS_DEBUG, ECC293, (ErrVal) ret,
                 "CcLiRmtAloc2WayPriCfm() Failed, ccGetRmSap failed");
#endif
      ccGenAlarm(LCM_CATEGORY_INTERNAL, LCC_EVENT_INV_RMSAP, 
                 LCM_CAUSE_INV_SAP, (U8 *) &suId);
      RETVALUE(RFAILED);
   }

   /* Find connection */
   /* Mask the least significant bit of suConnId before searching  for */
   /* the connection */
   tmpConnId = suConnId;

   ret = cmHashListFind (&ccSuInstTbl, (U8 *)&tmpConnId, (U16)sizeof(UConnId),
                         (U16)0, (PTR *) &con);

   if (ret != ROK)
      ret = cmHashListFind(&ccSuOgInstTbl, (U8 *)&tmpConnId,
                          (U16)sizeof(UConnId), (U16)0, (PTR *) &con);

   if (ret != ROK)
   {
      ccGenAlarm(LCM_CATEGORY_INTERNAL, LCC_EVENT_HASHING_FAILED, 
                 LCC_CAUSE_SUINSTTBL_FIND, (U8 *) &suConnId);
      RETVALUE(RFAILED);
   }

   /*Prepare routCfm event and call state machine function */
   alocCfmEvnt.rscSta1 = rscSta1;
   alocCfmEvnt.rscSta2 = RMT_RES_INVALID;
   alocCfmEvnt.rsc1 = rsc1;
   alocCfmEvnt.rsc2 = NULLP; 
   alocCfmEvnt.tfcDesc = tfcDsc;

   alocCfmEvnt.addnlRscInfo = addnlRscInfo;
   alocCfmEvnt.profile      = profile;
   alocCfmEvnt.mBuf         = NULLP;

   alocCfmEvnt.ogPrtclType = ogPrtclType;
#if 0
   alocCfmEvnt.dgtStrp = dgtStrp;
   alocCfmEvnt.dgtsPrefix = dgtsPrefix;
#endif
   /* Send to Call State machine */
   ccCallStateMachine (con, CCE_ALOCCFM, (PTR) &alocCfmEvnt, suId, suConnId,
                       (PTR) NULLP, (PTR) NULLP);

#ifdef ZC
   zcUpdPeer();
#endif /* ZC */
   RETVALUE(ROK);

} /* end of CcLiRmtAloc2WayPriCfm */


/*
*
*      Fun:   CcLiRmtDealocCfm
*
*      Desc:  Returns the Resource managers output
*
*      Ret:   ROK     - successful,
*
*      Notes: None.
*
*      File:  cc_bdy1.c
*
*/

#ifdef ANSI
PUBLIC S16 CcLiRmtDealocCfm
(
Pst    *pst,             /* post */
SuId   suId,             /* service user id */
UConnId suConnId,         /* Connection id */
U8      rscCount         /* resource count */
)
#else
PUBLIC S16 CcLiRmtDealocCfm(pst, suId, suConnId, rscCount)
Pst    *pst;             /* post */
SuId   suId;             /* service user id */
UConnId suConnId;         /* Connection id */
U8      rscCount;        /* resource count */
#endif
{
   S16 ret;
#if 0 /* cc028.15 : deletion */
   CcGenSap *rmCb;
#endif /* cc028.15 : deletion */
   CcConCb  *con;
   UConnId tmpConnId;

   CcRmDealocCfmEvnt dealocCfmEvnt;

   TRC3(CcLiRmtDealocCfm)

#if 1 /* TELICA - Bugzilla-5520- Khan/Yakin - 08/01/2001 - code trace */ 
   TELICACODETRC(LOGDEBUG, ccCodeTrcLvl, ccInit.lmPst, ("CcLiRmtDealocCfm::suConnId=%lx",suConnId)); 
#endif /* Bugzilla-5520 */


#if 0 /* cc028.15 : deletion */
   if ((ret = ccGetRmSap(&rmCb, NULLP, suId)) != ROK)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      CCLOGERROR(ERRCLS_DEBUG, ECC295, (ErrVal) ret,
                 "CcLiRmtDealocCfm() Failed, ccGetRmSap failed");
#endif
      ccGenAlarm(LCM_CATEGORY_INTERNAL, LCC_EVENT_INV_RMSAP, 
                 LCM_CAUSE_INV_SAP, (U8 *) &suId);
      RETVALUE(RFAILED);
   }
#endif /* cc028.15 : deletion */

   /* this is a Geo Audit resource - find the context block */
   if (suConnId & CC_GEO_AUD_CON_FLG)
   { 
      CcGeoRscEntry_t *geoRsc=NULL;                                            
      CcGeoCbEvent_t geoDealocCfmEvnt;                                         

        ret = cmHashListFind (&ccGeoSuInstTbl, (U8 *)&suConnId, 
                (U16)sizeof(UConnId), (U16)0, (PTR *) &geoRsc);

        if (ret != ROK)
        {
            ccGenAlarm(LCM_CATEGORY_INTERNAL, LCC_EVENT_HASHING_FAILED, 
                       LCC_CAUSE_SUINSTTBL_FIND, (U8 *) &suConnId);
            RETVALUE(RFAILED);
        }
        geoDealocCfmEvnt.type = GCE_RMT_DEALOC_CFM;
        geoDealocCfmEvnt.u.rmDeAloc.rsc = geoRsc;

        ccGeoCallStateMachine(geoRsc->con, &geoDealocCfmEvnt);
        RETVALUE(ROK);
   }
   else /* proceed with the generic resource dealloc confirm */
   {
        /* Find connection */
        /* Mask the least significant bit of suConnId before searching  for */
        /* the connection */
        tmpConnId = suConnId;

        ret = cmHashListFind (&ccSuInstTbl, (U8 *)&tmpConnId, (U16)sizeof(UConnId),
                         (U16)0, (PTR *) &con);
        if (ret != ROK) 
            ret = cmHashListFind(&ccSuOgInstTbl, (U8 *)&tmpConnId,
                                 (U16)sizeof(UConnId), (U16)0, (PTR *) &con);

        if (ret != ROK)
        {
#if 0 /* cc017.15 : deletion */
#if (ERRCLASS & ERRCLS_DEBUG)
      CCLOGERROR(ERRCLS_DEBUG, ECC296, (ErrVal) ret,
         "CcLiRmtDealocCfm() Failed, cmHashListFind failed in suInstTbl");
#endif
#endif /* cc017.15 : deletion */
            ccGenAlarm(LCM_CATEGORY_INTERNAL, LCC_EVENT_HASHING_FAILED, 
                       LCC_CAUSE_SUINSTTBL_FIND, (U8 *) &suConnId);
            RETVALUE(RFAILED);
        }
   
        /*Prepare DealocCfm event and call state machine function */
        dealocCfmEvnt.rscCount = rscCount;
        dealocCfmEvnt.suConnId = suConnId;

        if (con->dealocPend > 0) con->dealocPend--;
   
        /* Send to Call State machine */
        ccCallStateMachine (con, CCE_DEALOCCFM, (PTR) &dealocCfmEvnt, suId,
                           suConnId, (PTR) NULLP, (PTR) NULLP);

#ifdef ZC
        zcUpdPeer();
#endif /* ZC */
   }

   RETVALUE(ROK);

} /* end of CcLiRmtDealocCfm */



/*
*
*      Fun:   CcLiRmtDealocInd
*
*      Desc:  Returns the Resource managers output
*
*      Ret:   ROK     - successful,
*
*      Notes: None.
*
*      File:  cc_bdy1.c
*
*/

#ifdef ANSI
PUBLIC S16 CcLiRmtDealocInd
(
Pst    *pst,             /* post */
SuId   suId,             /* service user id */
UConnId suConnId,        /* Connection id */
RmRsc  *resource1        /* resource 1 */
)
#else
PUBLIC S16 CcLiRmtDealocInd(pst, suId, suConnId, resource1)
Pst    *pst;             /* post */
SuId   suId;             /* service user id */
UConnId suConnId;        /* Connection id */
RmRsc  *resource1;       /* resource 1 */
#endif
{
   S16 ret;
   CcGenSap *rmCb;
   CcConCb  *con;
   UConnId tmpConnId;
   CcRmDealocIndEvnt dealocIndEvnt;

   TRC3(CcLiRmtDealocInd)

   if ((ret = ccGetRmSap(&rmCb, NULLP, suId)) != ROK)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      CCLOGERROR(ERRCLS_DEBUG, ECC297, (ErrVal) ret,
                 "CcLiRmtDealocInd() Failed, ccGetRmSap failed");
#endif
      ccGenAlarm(LCM_CATEGORY_INTERNAL, LCC_EVENT_INV_RMSAP, 
                 LCM_CAUSE_INV_SAP, (U8 *) &suId);
      RETVALUE(RFAILED);
   }

   /* Find connection */
   /* Mask the least significant bit of suConnId before searching  for */
   /* the connection */

  tmpConnId = suConnId;

   ret = cmHashListFind (&ccSuInstTbl, (U8 *)&tmpConnId, (U16)sizeof(UConnId),
                         (U16)0, (PTR *) &con);

   if (ret != ROK) 
      ret = cmHashListFind(&ccSuOgInstTbl, (U8 *)&tmpConnId,
                        (U16)sizeof(UConnId), (U16)0, (PTR *) &con);


   if (ret != ROK)
   {
#if 0 /* cc017.15 : deletion */
#if (ERRCLASS & ERRCLS_DEBUG)
      CCLOGERROR(ERRCLS_DEBUG, ECC298, (ErrVal) ret,
         "CcLiRmtDealocInd() Failed, cmHashListFind failed in suInstTbl");
#endif
#endif /* cc017.15 : deletion */
      ccGenAlarm(LCM_CATEGORY_INTERNAL, LCC_EVENT_HASHING_FAILED, 
                 LCC_CAUSE_SUINSTTBL_FIND, (U8 *) &suConnId);
      RETVALUE(RFAILED);
   }

   /*Prepare DealocCfm event and call state machine function */
   dealocIndEvnt.suConnId = suConnId;
   dealocIndEvnt.resource1 = resource1;
   
   /* Send to Call State machine */
   ccCallStateMachine (con, CCE_DEALOCIND, (PTR) &dealocIndEvnt, suId, suConnId,
                       (PTR) NULLP, (PTR) NULLP);

#ifdef ZC
   zcUpdPeer();
#endif /* ZC */
   RETVALUE(ROK);

} /* end of CcLiRmtDealocInd */

#if 0
  
/*
*     Chapter 4: interface functions to Router Service Provider
*/

/*
*
*      Fun:   CcLiRttMntStaInd
*
*      Desc:  Maintenance Status Ind for the purposes of router audit
*
*      Ret:   ROK     - successful,
*
*      Notes: None.
*
*      File:  cc_bdy1.c
*
*/

#ifdef ANSI
PUBLIC S16 CcLiRttMntStaInd
(
Pst *pst,                      /* post */
SuId suId,                     /* service user identifier */
U8   mntEvntType,
CcMntStaEvnt *mntEvnt,
RmInterface *intfc
)
#else
PUBLIC S16 CcLiRttMntStaInd(pst, suId, mntEvntType, mntEvnt, intfc)
Pst *pst;                      /* post */
SuId suId;                     /* service user identifier */
U8   mntEvntType;
CcMntStaEvnt *mntEvnt;
RmInterface *intfc;
#endif
{
   CcPsSap     *pssap;
   S16 ret;

   TRC3(CcLiRttMntStaInd)

   UNUSED(intfc);

   ret = ccGetPsSap(&pssap, &(mntEvnt->t.rtAudEvnt.intfc));

   if (ret != ROK)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      CCLOGERROR(ERRCLS_DEBUG, ECCXXX, (ErrVal) ret,
         "CcLiRttMntStaInd() Failed, Cannot find PsSap");
#endif
      ccGenAlarm(LCM_CATEGORY_INTERNAL, LCC_EVENT_INV_DESTSAP,
                 LCC_CAUSE_INV_INTERFACE, (U8 *) &suId);
      RETVALUE(RFAILED);
   }

   CcLiCctMntStaReq(&pssap->pst, pssap->spId, mntEvntType, mntEvnt);
   RETVALUE(ROK);
}
#endif

/*
*
*      Fun:   MgiLiMgctBndCfm
*
*      Desc:  Confirmation from MGI Manager of the Bind Request Issued
*
*      Ret:   ROK     - successful
*
*      Notes: None.
*
*      File:  cc_bdy1.c
*
*/
PUBLIC Void MgiLiMgctBndCfm
(
MgctBndCfm *bndCfm
)
{
   CcGenSap *sfCb;
   CcMgSapCb *pMgSapCb = NULLP;

   if (bndCfm == NULLP)
   {
     CCLOGERR("Error: bndCfm is NULLP\n");
     RETVOID;
   }
   CCDP(AIN_DBG_LEVEL_1,"%s received Bind Confirm from MGI suId %d spId %d status %d major %d minor%d\n",
       __FUNCTION__, bndCfm->suId, bndCfm->spId, bndCfm->status, bndCfm->version.major, bndCfm->version.minor);

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (ccInit.cfgDone != TRUE)
   {
      CCLOGERROR(ERRCLS_INT_PAR, ECC303, (ErrVal) bndCfm->suId,
                 "CcLiMgitBndCfm() Failed, invalid suId");
      RETVOID;
   }
#endif

   /* Need first to find out the corresponding MG SAP */
   if(cmHashListFind(&ccMgSapTbl, (U8*)&(bndCfm->spId),
                (U16)sizeof(SpId), (U16)0, (PTR*)&pMgSapCb) != ROK)
   {
     CCLOGERR("%s: corresponding MG SAP not found!\n", __FUNCTION__);
     RETVOID;
   }

   if (pMgSapCb->ccsId == ccCp.genCfg.ownCcsId)
   {
     if ((sfCb = *ccSfSapLstPtr) == NULLP)
     {
#if (ERRCLASS & ERRCLS_INT_PAR)
       CCLOGERROR(ERRCLS_INT_PAR, ECC304, (ErrVal) bndCfm->suId,
                  "CcLiMgitBndCfm() Failed, invalid suId");
#endif
       ccGenAlarm(LCM_CATEGORY_INTERNAL, LCC_EVENT_INV_SFSAP,
                  LCM_CAUSE_INV_SAP, (U8 *) &(bndCfm->suId));
       RETVOID;
     }
     ccStopSapTmr((PTR)sfCb, TMR_SFSAPBNDCFM);
   }
   else
   {
     ccStopSapTmr((PTR)&(pMgSapCb->cb), TMR_SFSAPBNDCFM);
   }

   
   CCDP(AIN_DBG_LEVEL_1, "state %d\n", pMgSapCb->cb.state);
   
   /* if waiting for Bind Confirm and Bind Successful */
   if (pMgSapCb->ccsId == ccCp.genCfg.ownCcsId)
   {
     CCDP(AIN_DBG_LEVEL_1, "sfCb->state%d\n", sfCb->state);
     
     if ((sfCb->state == CC_BND_CONT) && (bndCfm->status == MGCT_BND_OK))
     {
       sfCb->bndRetryCnt = 0;
       sfCb->state = CC_BND;

#ifdef ZC
       ZcUpdParam upd;
       upd.gensap = sfCb;
       zcRunTimeUpd(ZC_SFSAP_CB, CMPFTHA_UPD_REQ, upd);
#endif

      pMgSapCb->cb.bndRetryCnt = 0;
      pMgSapCb->cb.state       = CC_BND;
      
      cmMemcpy((U8*) &(pMgSapCb->version),
                  (U8*) &(bndCfm->version), sizeof(MgctVersion));
     }
     else if((sfCb->state == CC_BND) && (bndCfm->status == MGCT_BND_OK))
     {
       /* For CM failover cases:
        * 1. For local MG:
        *  the sfSap->state in standby side is replicated from active side and it is CC_BND
        *  for this case only update the local MG Sap state in ccMgSapTbl
        * 2. For the global MG:
        *  as we do not issue another bind on the standby side, we will not have this case
        */
       sfCb->bndRetryCnt = 0;

       pMgSapCb->cb.bndRetryCnt = 0;
       pMgSapCb->cb.state       = CC_BND;
       
       cmMemcpy((U8*) &(pMgSapCb->version),
                   (U8*) &(bndCfm->version), sizeof(MgctVersion));
     }

   }
   else if ((pMgSapCb->cb.state == CC_BND_CONT) && (bndCfm->status == MGCT_BND_OK))
   {
      pMgSapCb->cb.bndRetryCnt = 0;
      pMgSapCb->cb.state       = CC_BND;

      cmMemcpy((U8*) &(pMgSapCb->version),
                  (U8*) &(bndCfm->version), sizeof(MgctVersion));
   }
   else if((pMgSapCb->cb.state == CC_BND) && (bndCfm->status == MGCT_BND_OK))
   {
     pMgSapCb->cb.bndRetryCnt = 0;
     pMgSapCb->cb.state       = CC_BND;

     cmMemcpy((U8*) &(pMgSapCb->version),
                 (U8*) &(bndCfm->version), sizeof(MgctVersion));
   }/* Successful Bind Confirm */

   if ((bndCfm->status == MGCT_BND_BAD_VERSION) ||
       (bndCfm->status == MGCT_BND_NO_SAP_CFG) ||
       (bndCfm->status == MGCT_BND_OTHER_ERROR))
   {
      ccGenAlarm(LCM_CATEGORY_PROTOCOL, LCM_EVENT_BND_FAIL,
                 LCM_CAUSE_UNKNOWN, NULLP);

      pMgSapCb->cb.state = CC_UNBND;

      if (pMgSapCb->ccsId == ccCp.genCfg.ownCcsId)
      {
        sfCb->state = CC_UNBND;
      }
   }

#ifdef ZC
   zcUpdPeer();
#endif /* ZC */
   RETVOID;
} /* end of MgiLiMgctBndCfm */


/*
*
*      Fun:   CcLiCctHldInd
*
*      Desc:  Process an connection Hold
*
*      Ret:   ROK     - successful,
*
*      Notes: None.
*
*      File:  cc_bdy1.c
*
*/

#ifdef ANSI
PUBLIC S16 CcLiCctHldInd
(
Pst        *pst,               /* post */
SuId       suId,               /* service user identier */
UConnId    suConnId,           /* service user instance identifier */
UConnId    spConnId,           /* service provider instance identifier */
U8         evntType,           /* incoming event type */
ProtType   protType,           /* incoming protocol type */
CcHldEvnt *ccHldEvnt,          /* connection Hold event structure */
Buffer     *uBuf               /* unrecognized parameters */
)
#else
PUBLIC S16 CcLiCctHldInd(pst, suId, suConnId, spConnId, evntType, protType, 
                         ccHldEvnt, uBuf)
Pst        *pst;               /* post */
SuId       suId;               /* service user identier */
UConnId    suConnId;           /* service user instance identifier */
UConnId    spConnId;           /* service provider instance identifier */
U8         evntType;           /* incoming event type */
ProtType   protType;           /* incoming protocol type */
CcHldEvnt *ccHldEvnt;          /* connection Hold event structure */
Buffer     *uBuf;              /* unrecognized parameters */
#endif
{
   CcPsSap *pCb;
   CcConCb *con;
   S16 ret;
   UConnId tmpConnId;

   TRC3( CcLiCctHldInd)

#if (ERRCLASS & ERRCLS_INT_PAR)
   if ((ccInit.cfgDone != TRUE) || (suId >= (SuId) ccCp.genCfg.maxPsSAP))
   {
      CCLOGERROR(ERRCLS_INT_PAR, ECC313, (ErrVal) suId,
                 "CcLiCctHldInd() Failed, invalid suId");
      if (uBuf != NULLP)
         ccDropUBuf(&uBuf);
      RETVALUE(RFAILED);
   }
#endif

   if ((ccPsSapLstPtr == NULLP) || ((pCb = *(ccPsSapLstPtr + suId)) == NULLP))
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      CCLOGERROR(ERRCLS_INT_PAR, ECC314, (ErrVal) suId,
                 "CcLiCctHldInd() Failed, invalid suId");
#endif
      ccGenAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT,
                 LCM_CAUSE_INV_SAP, (U8*) &suId);
      if (uBuf != NULLP)
         ccDropUBuf(&uBuf);
      RETVALUE(RFAILED);
   }

   if (pCb->state != CC_BND)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      CCLOGERROR(ERRCLS_INT_PAR, ECC315, (ErrVal) suId,
                 "CcLiCctHldInd() Failed, SAP not bound");
#endif
      ccGenAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_INV_STATE,
                 LCM_CAUSE_INV_SAP, (U8*) &suId);
      if (uBuf != NULLP)
         ccDropUBuf(&uBuf);
      RETVALUE(RFAILED);
   }
     
   /* Find connection */
   /* Mask the least significant bit of suConnId before searching  for */
   /* the connection */
  tmpConnId = suConnId;

   ret = cmHashListFind (&ccSuInstTbl, (U8 *)&tmpConnId, (U16)sizeof(UConnId),
                         (U16)0, (PTR *) &con);
   if (ret != ROK) 
      ret = cmHashListFind(&ccSuOgInstTbl, (U8 *)&tmpConnId,
                        (U16)sizeof(UConnId), (U16)0, (PTR *) &con);

   if (ret != ROK)
      ret = cmHashListFind (&pCb->spInstTbl, (U8 *)&spConnId, 
                            (U16)sizeof(UConnId), (U16)0, (PTR *) &con);

   if (ret != ROK)
   {
#if 0 /* cc017.15 : deletion */
#if (ERRCLASS & ERRCLS_DEBUG)
      CCLOGERROR(ERRCLS_DEBUG, ECC316, (ErrVal) ret,
         "CcLiCctHldInd() Failed, cmHashListFind failed in spInstTbl");
#endif
#endif /* cc017.15 : deletion */
      ccGenAlarm(LCM_CATEGORY_INTERNAL, LCC_EVENT_HASHING_FAILED, 
                 LCC_CAUSE_SPINSTTBL_FIND, (U8 *) &spConnId);
      if (uBuf != NULLP)
         ccDropUBuf(&uBuf);
      RETVALUE(RFAILED);
   }

   con->icEvntType = evntType;
   con->direction = ccGetDirection(con, spConnId, suConnId, pCb);
   /* Store the spConnId on the outgoing interface */
   if ((con->direction == OUTTOINC) && (con->ogSpConnId == 0))
      con->ogSpConnId = spConnId;
   /* Bugzilla 10706:taraveti memory leak fix */
   ccDropUBuf(&con->uBuf);
   con->uBuf      = uBuf;
   /* call state machine function */
   ccCallStateMachine (con, CCE_HLDIND, (PTR)ccHldEvnt, suId, spConnId,
                       (PTR) NULLP, (PTR) NULLP);
   
#ifdef ZC
   zcUpdPeer();
#endif /* ZC */
   RETVALUE(ROK);
}


/*
*
*      Fun:   CcLiCctRtrInd
*
*      Desc:  Process an connection Retrieve Indication
*
*      Ret:   ROK     - successful,
*
*      Notes: None.
*
*      File:  cc_bdy1.c
*
*/

#ifdef ANSI
PUBLIC S16 CcLiCctRtrInd
(
Pst        *pst,               /* post */
SuId       suId,               /* service user identier */
UConnId    suConnId,           /* service user instance identifier */
UConnId    spConnId,           /* service provider instance identifier */
U8         evntType,           /* incoming event type */
ProtType   protType,           /* incoming protocol type */
CcRtrEvnt *ccRtrEvnt,          /* connection Retrieve event structure */
Buffer     *uBuf               /* unrecognized parameters */
)
#else
PUBLIC S16 CcLiCctRtrInd(pst, suId, suConnId, spConnId, evntType, protType, 
                         ccRtrEvnt, uBuf)
Pst        *pst;               /* post */
SuId       suId;               /* service user identier */
UConnId    suConnId;           /* service user instance identifier */
UConnId    spConnId;           /* service provider instance identifier */
U8         evntType;           /* incoming event type */
ProtType   protType;           /* incoming protocol type */
CcRtrEvnt *ccRtrEvnt;          /* connection Retrieve event structure */
Buffer     *uBuf;              /* unrecognized parameters */
#endif
{
   CcPsSap *pCb;
   CcConCb *con;
   S16 ret;
   UConnId tmpConnId;

   TRC3( CcLiCctRtrInd)

#if (ERRCLASS & ERRCLS_INT_PAR)
   if ((ccInit.cfgDone != TRUE) || (suId >= (SuId) ccCp.genCfg.maxPsSAP))
   {
      CCLOGERROR(ERRCLS_INT_PAR, ECC317, (ErrVal) suId,
                 "CcLiCctRtrInd() Failed, invalid suId");
      if (uBuf != NULLP)
         ccDropUBuf(&uBuf);
      RETVALUE(RFAILED);
   }
#endif

   if ((ccPsSapLstPtr == NULLP) || ((pCb = *(ccPsSapLstPtr + suId)) == NULLP))
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      CCLOGERROR(ERRCLS_INT_PAR, ECC318, (ErrVal) suId,
                 "CcLiCctRtrInd() Failed, invalid suId");
#endif
      ccGenAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT,
                 LCM_CAUSE_INV_SAP, (U8*) &suId);
      if (uBuf != NULLP)
         ccDropUBuf(&uBuf);
      RETVALUE(RFAILED);
   }

   if (pCb->state != CC_BND)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      CCLOGERROR(ERRCLS_INT_PAR, ECC319, (ErrVal) suId,
                 "CcLiCctRtrInd() Failed, SAP not bound");
#endif
      ccGenAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_INV_STATE,
                 LCM_CAUSE_INV_SAP, (U8*) &suId);
      if (uBuf != NULLP)
         ccDropUBuf(&uBuf);
      RETVALUE(RFAILED);
   }
     
   /* Find connection */
   /* Mask the least significant bit of suConnId before searching  for */
   /* the connection */
  tmpConnId = suConnId;

   ret = cmHashListFind (&ccSuInstTbl, (U8 *)&tmpConnId, (U16)sizeof(UConnId),
                         (U16)0, (PTR *) &con);
   if (ret != ROK) 
      ret = cmHashListFind(&ccSuOgInstTbl, (U8 *)&tmpConnId,
                        (U16)sizeof(UConnId), (U16)0, (PTR *) &con);

   if (ret != ROK)
      ret = cmHashListFind (&pCb->spInstTbl, (U8 *)&spConnId, 
                            (U16)sizeof(UConnId), (U16)0, (PTR *) &con);

   if (ret != ROK)
   {
      ccGenAlarm(LCM_CATEGORY_INTERNAL, LCC_EVENT_HASHING_FAILED, 
                 LCC_CAUSE_SPINSTTBL_FIND, (U8 *) &spConnId);
      if (uBuf != NULLP)
         ccDropUBuf(&uBuf);
      RETVALUE(RFAILED);
   }
   con->icEvntType = evntType;
   con->direction = ccGetDirection(con, spConnId, suConnId, pCb);
   /* Store the spConnId on the outgoing interface */
   if ((con->direction == OUTTOINC) && (con->ogSpConnId == 0))
      con->ogSpConnId = spConnId;
   /* Bugzilla 10706:taraveti memory leak fix */
   ccDropUBuf(&con->uBuf);
   con->uBuf      = uBuf;
   /* call state machine function */
   ccCallStateMachine (con, CCE_RTRIND, (PTR)ccRtrEvnt, suId, spConnId,
                       (PTR) NULLP, (PTR) NULLP);
   
#ifdef ZC
   zcUpdPeer();
#endif /* ZC */
   RETVALUE(ROK);
}


/*
*
*      Fun:   CcLiCctProfInd
*
*      Desc:  process an incoming connection release request
*
*      Ret:   ROK     - successful,
*
*      Notes: None.
*
*      File:  cc_bdy1.c
*
*/

#ifdef ANSI
PUBLIC S16 CcLiCctProfInd
(
Pst       *pst,               /* post */
SuId      suId,               /* service user identier */
UConnId   suConnId,           /* service user instance identifier */
UConnId   spConnId,           /* service provider instance identifier */
CcProfile *ccProfile          /* connection profile structure */
)
#else
PUBLIC S16 CcLiCctProfInd(pst, suId, suConnId, spConnId, ccProfile)
Pst       *pst;               /* post */
SuId      suId;               /* service user identier */
UConnId   suConnId;           /* service user instance identifier */
UConnId   spConnId;           /* service provider instance identifier */
CcProfile *ccProfile;         /* connection profile structure */
#endif
{
   CcPsSap *pCb;
   CcConCb *con;
   S16 ret;
   UConnId tmpConnId;

   TRC3( CcLiCctProfInd)

#if (ERRCLASS & ERRCLS_INT_PAR)
   if ((ccInit.cfgDone != TRUE) || (suId >= (SuId) ccCp.genCfg.maxPsSAP))
   {
      CCLOGERROR(ERRCLS_INT_PAR, ECC321, (ErrVal) suId,
                 "CcLiCctProfInd() Failed, invalid suId");
      RETVALUE(RFAILED);
   }
#endif

   if ((ccPsSapLstPtr == NULLP) || ((pCb = *(ccPsSapLstPtr + suId)) == NULLP))
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      CCLOGERROR(ERRCLS_INT_PAR, ECC322, (ErrVal) suId,
                 "CcLiCctProfInd() Failed, invalid suId");
#endif
      ccGenAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT,
                 LCM_CAUSE_INV_SAP, (U8*) &suId);
      RETVALUE(RFAILED);
   }

   if (pCb->state != CC_BND)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      CCLOGERROR(ERRCLS_INT_PAR, ECC323, (ErrVal) suId,
                 "CcLiCctProfInd() Failed, SAP not bound");
#endif
      ccGenAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_INV_STATE,
                 LCM_CAUSE_INV_SAP, (U8*) &suId);
      RETVALUE(RFAILED);
   }

   /* Find connection */
   /* Mask the least significant bit of suConnId before searching  for */
   /* the connection */
  tmpConnId = suConnId;

   ret = cmHashListFind (&ccSuInstTbl, (U8 *)&tmpConnId, (U16)sizeof(UConnId),
                         (U16)0, (PTR *) &con);
   if (ret != ROK) 
      ret = cmHashListFind(&ccSuOgInstTbl, (U8 *)&tmpConnId,
                        (U16)sizeof(UConnId), (U16)0, (PTR *) &con);

   if (ret != ROK)
      ret = cmHashListFind (&pCb->spInstTbl, (U8 *)&spConnId, 
                            (U16)sizeof(UConnId), (U16)0, (PTR *) &con);

   if (ret != ROK)
   {
#if 0 /* cc017.15 : deletion */
#if (ERRCLASS & ERRCLS_DEBUG)
      CCLOGERROR(ERRCLS_DEBUG, ECC324, (ErrVal) ret,
         "CcLiCctProfInd() Failed, cmHashListFind failed in spInstTbl");
#endif
#endif /* cc017.15 : deletion */
      ccGenAlarm(LCM_CATEGORY_INTERNAL, LCC_EVENT_HASHING_FAILED, 
                 LCC_CAUSE_SPINSTTBL_FIND, (U8 *) &spConnId);
      RETVALUE(RFAILED);
   }
   con->direction = ccGetDirection(con, spConnId, suConnId, pCb);
   /* Store the spConnId on the outgoing interface */
   if ((con->direction == OUTTOINC) && (con->ogSpConnId == 0))
      con->ogSpConnId = spConnId;

   /* Copy connection profile */
   if (con->direction == INCTOOUT)
   {
      cmMemcpy((U8 *) con->icProfile, (U8 *) ccProfile, sizeof(CcProfile));   
   }
   else if (con->direction == OUTTOINC)
   {
      cmMemcpy((U8 *) con->ogProfile, (U8 *) ccProfile, sizeof(CcProfile));   
   }

#ifdef ZC
   zcUpdPeer();
#endif /* ZC */
   RETVALUE(ROK);
}


/*
*
*      Fun:   CcLiCctSvcInd
*
*      Desc:  Service Indication from Layer-3
*
*      Ret:   ROK     - successful,
*
*      Notes: None.
*
*      File:  cc_bdy1.c
*
*/
PUBLIC S16 CcLiCctSvcInd
(
Pst        *pst,               /* post */
SuId       suId,               /* service user identier */
UConnId    suConnId,           /* service user instance identifier */
UConnId    spConnId,           /* service provider instance identifier */
U8         evntType,           /* incoming event type */
ProtType   protType,           /* incoming protocol type */
CcSvcEvnt  *ccSvcEvnt,         /* Service event structure */
Buffer     *uBuf               /* unrecognized parameters */
)
{
   CcPsSap *pCb;
   CcConCb *con;
   S16 ret;
   U8  tempEvntType = 0xFF; /* 27249 */
   Buffer  *saveUBuf = NULLP;		/* 44312 */

   TRC3(CcLiCctSvcInd)

   if ((ccInit.cfgDone != TRUE) || (suId >= (SuId) ccCp.genCfg.maxPsSAP))
   {
      DP("CcLiCctSvcInd() Failed, invalid suId=%d or cfg NotDone\n", suId);
      if (uBuf != NULLP)
         ccDropUBuf(&uBuf);
      ccFreeSdpInTheEvnt(__FILE__, __LINE__, CCE_SVC_IND, evntType,
          (PTR)ccSvcEvnt, protType);
      ccFreeSipSipTMBufInTheEvnt(__FILE__, __LINE__, CCE_SVC_IND, 
           evntType, (PTR)ccSvcEvnt, protType); /* FID 14732 */
      RETVALUE(RFAILED);
   }

   if ((ccPsSapLstPtr == NULLP) || ((pCb = *(ccPsSapLstPtr + suId)) == NULLP))
   {
     DP("CcLiCctSvcInd() Failed, invalid suId=%d\n", suId);

      ccGenAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT,
                 LCM_CAUSE_INV_SAP, (U8*) &suId);
      if (uBuf != NULLP)
         ccDropUBuf(&uBuf);
      ccFreeSdpInTheEvnt(__FILE__, __LINE__, CCE_SVC_IND, evntType,
          (PTR)ccSvcEvnt, protType);
      ccFreeSipSipTMBufInTheEvnt(__FILE__, __LINE__, CCE_SVC_IND, evntType, 
          (PTR)ccSvcEvnt, protType); /* FID 14732 */
      RETVALUE(RFAILED);
   }

   if (pCb->state != CC_BND)
   {
      DP("CcLiCctSvcInd() Failed, SAP not bound\n");
      ccGenAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_INV_STATE,
                 LCM_CAUSE_INV_SAP, (U8*) &suId);
      if (uBuf != NULLP)
         ccDropUBuf(&uBuf);
      ccFreeSdpInTheEvnt(__FILE__, __LINE__, CCE_SVC_IND, evntType,
          (PTR)ccSvcEvnt, protType);
      ccFreeSipSipTMBufInTheEvnt(__FILE__, __LINE__, CCE_SVC_IND, evntType, 
              (PTR)ccSvcEvnt,
            protType); /* FID 14732 */
      RETVALUE(RFAILED);
   }

   /* Find connection */
   ret = cmHashListFind (&ccSuInstTbl, (U8 *)&suConnId, (U16)sizeof(UConnId),
                         (U16)0, (PTR *) &con);
   if (ret != ROK) 
      ret = cmHashListFind(&ccSuOgInstTbl, (U8 *)&suConnId,
                        (U16)sizeof(UConnId), (U16)0, (PTR *) &con);

   if (ret != ROK)
      ret = cmHashListFind (&pCb->spInstTbl, (U8 *)&spConnId, 
                            (U16)sizeof(UConnId), (U16)0, (PTR *) &con);

   if (ret != ROK)
   {
      DP("****** Alert : context not found : su:0x%lx, sp:0x%lx ***\n",
               suConnId, spConnId);
       if (uBuf != NULLP)
           ccDropUBuf(&uBuf);
       ccFreeSdpInTheEvnt(__FILE__, __LINE__, CCE_SVC_IND, evntType,
          (PTR)ccSvcEvnt, protType);
       ccFreeSipSipTMBufInTheEvnt(__FILE__, __LINE__, CCE_SVC_IND, evntType, 
          (PTR)ccSvcEvnt, protType); /* FID 14732 */
       RETVALUE(RFAILED);
   }
   con->direction = ccGetDirection(con, spConnId, suConnId, pCb);
   /* Store the spConnId on the outgoing interface */
   if ((con->direction == OUTTOINC) && (con->ogSpConnId == 0))
      con->ogSpConnId = spConnId;
   else if((con->direction == INCTOOUT) && (con->icSpConnId == 0))
      con->icSpConnId = spConnId; /* added for bug#20323 */

   /* 27249 */
   if(con->direction == OUTTOINC) tempEvntType = con->icEvntType;
   con->icEvntType = evntType;
   /* 27249 */

   /*FID 14732 */
   ccExtractSipwMBufFromSvcInd(con, (PTR)ccSvcEvnt);
   /*FID 14732 */

   /* Bugzilla 10706:taraveti memory leak fix */
#if 0	/* 44312+ */
   ccDropUBuf(&con->uBuf);
#else
   saveUBuf = con->uBuf;
#endif	/* 44312- */
   con->uBuf      = uBuf;

   /* call state machine function */
   ccCallStateMachine (con, CCE_SVC_IND, (PTR)ccSvcEvnt, suId, spConnId,
                       (PTR) NULLP, (PTR) NULLP);
   ccFreeSdpInTheEvnt(__FILE__, __LINE__, CCE_SVC_IND, evntType,
       (PTR)ccSvcEvnt, protType);
   ccFreeSipSipTMBufInTheEvnt(__FILE__, __LINE__, CCE_SVC_IND, evntType, 
       (PTR)ccSvcEvnt, protType); /* FID 14732 */

  if ((con->bufferBlock.bufferedEvent != NULLP) && (tempEvntType != 0xFF))
  {
    /* This event is buffered. So, revert the icEvntType - 27249 */
    DP("*** icEvntType is reverted from %d to %d ****\n", con->icEvntType,
        tempEvntType);
    con->icEvntType = tempEvntType;
#if 1	/* 44312+ */
    con->uBuf = saveUBuf;
  } else {
    ccDropUBuf(&saveUBuf);
#endif	/* 44312- */    
  }

#ifdef ZC
   zcUpdPeer();
#endif /* ZC */
   RETVALUE(ROK);
}

/*
*
*       Fun:    CcLiRmtAudRsp
*
*       Desc:   Audit response received from RM
*
*       Ret:    ROK     - ok
*
*       Notes:  None
*
*       File:   cc_bdy1.c
*
*/
#ifdef ANSI
PUBLIC S16 CcLiRmtAudRsp
(
Pst *pst,
SuId suId
)
#else
PUBLIC S16 CcLiRmtAudRsp(pst, suId)
Pst *pst;
SuId suId;
#endif
{
   CcGenSap *rmsap;
   S16      ret;

   if ((ret = ccGetRmSap(&rmsap, NULLP,  suId)) != ROK)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      CCLOGERROR(ERRCLS_DEBUG, ECC294, (ErrVal) ret,
                 "CcLiRmtAudRsp() Failed, ccGetRmSap failed");
#endif
      ccGenAlarm(LCM_CATEGORY_INTERNAL, LCC_EVENT_INV_RMSAP,
                 LCM_CAUSE_INV_SAP, (U8 *) &suId);
      RETVALUE(RFAILED);
   }

   /* Stop the timer that was waiting on this AudRsp from RM */
   ccStopSapTmr((PTR)rmsap, TMR_RMAUDRSP);

   RETVALUE(ROK);
} /* end of CcLiRmtAudRsp() */

/*
*
*       Fun:    CcLiRmtAudReq
*
*       Desc:   Audit request received from RM
*
*       Ret:    ROK     - ok
*
*       Notes:  None
*
*       File:   cc_bdy1.c
*
*/
#ifdef ANSI
PUBLIC S16 CcLiRmtAudReq
(
Pst *pst, 
SuId suId, 
RmAudReqEvnt *rmAudReqEvnt
)
#else
PUBLIC S16 CcLiRmtAudReq(pst, suId, rmAudReqEvnt)
Pst *pst;
SuId suId; 
RmAudReqEvnt *rmAudReqEvnt;
#endif
{
   S16 ret = ROK;
   U16 i;
   CcGenSap *rmsap;
   CcConCb *con;

   if ((ret = ccGetRmSap(&rmsap, NULLP,  suId)) != ROK)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      CCLOGERROR(ERRCLS_DEBUG, ECC294, (ErrVal) ret,
                 "CcLiRmtAudReq() Failed, ccGetRmSap failed");
#endif
      ccGenAlarm(LCM_CATEGORY_INTERNAL, LCC_EVENT_INV_RMSAP,
                 LCM_CAUSE_INV_SAP, (U8 *) &suId);
      RETVALUE(RFAILED);
   }

   DP("%s: rcvd AudReq from suId=%d\n", __FUNCTION__, suId);

   switch(rmsap->audState)
   {
      case RMAUD_AWT_RMAUDREQ:
         if ((rmAudReqEvnt->t.rmGAPAudReqEvnt.sequence == 1) || 
             (rmAudReqEvnt->t.rmGAPAudReqEvnt.sequence != 
              rmsap->audCfmEvnt.t.rmGAPAudCfmEvnt.sequence))
         {
            rmsap->audCfmEvnt.t.rmGAPAudCfmEvnt.sequence = 
                                      rmAudReqEvnt->t.rmGAPAudReqEvnt.sequence;

            rmsap->audCfmEvnt.t.rmGAPAudCfmEvnt.numSuConnId = 0;
            rmsap->audCfmEvnt.audType = rmAudReqEvnt->audType;

            if(rmAudReqEvnt->t.rmGAPAudReqEvnt.numSuConnId == 0)
            {
              CcConCb *first = NULLP;

              /* This is the final GAP Aud Req from RM */
              DP("%s: final GAP Aud Req received in GCC\n", __FUNCTION__);
              rmsap->audCfmEvnt.returnStatus = RMT_AUD_STATUS_OK;

              CcLiRmtAudCfm(&rmsap->pst, rmsap->spId, &rmsap->audCfmEvnt);

              ret = cmHashListGetNext(&ccSuInstTbl, (PTR) NULLP, (PTR *)&first);
              if (ret != ROK || !first)
              {
                DP("AUDIT: no standing call found.\n");
                return (ROK);
              }

              /* initialize obsolete call counter */
              obsoleteCallCounter = 0;
              ccClearObsoleteCalls(first->icSuConnId);
              return (ROK);
            }

            rmsap->audCfmEvnt.returnStatus = RMT_AUD_STATUS_OK_NDONE;

            for(i=0; i<rmAudReqEvnt->t.rmGAPAudReqEvnt.numSuConnId; i++)
            {
               ret = cmHashListFind(&ccSuInstTbl, 
                            (U8 *)&rmAudReqEvnt->t.rmGAPAudReqEvnt.suConnId[i], 
                            (U16)sizeof(UConnId), (U16)0, (PTR *) &con);
               if (ret != ROK)
                  ret = cmHashListFind(&ccSuOgInstTbl,
                           (U8 *)&rmAudReqEvnt->t.rmGAPAudReqEvnt.suConnId[i],
                           (U16)sizeof(UConnId), (U16)0, (PTR *) &con); 
               if (ret != ROK)
               {
                 UConnId connId = rmAudReqEvnt->t.rmGAPAudReqEvnt.suConnId[i];
                 CcGeoRscEntry_t *gRsc =  ccGeoFindRscFromConnId(connId);

                 if (gRsc)
                 {
                   DP("GAP Audit found con in geoCon 0x%ld\n", connId);
                   continue;
                 }

                  CCDP(AIN_DBG_LEVEL_0,
                      "%s: audit found incomplete connection 0x%ld\n",
                      __FUNCTION__,
                      rmAudReqEvnt->t.rmGAPAudReqEvnt.suConnId[i]);
                  rmsap->audCfmEvnt.t.rmGAPAudCfmEvnt.suConnId
                          [rmsap->audCfmEvnt.t.rmGAPAudCfmEvnt.numSuConnId] = 
                                    rmAudReqEvnt->t.rmGAPAudReqEvnt.suConnId[i];
                  rmsap->audCfmEvnt.t.rmGAPAudCfmEvnt.numSuConnId++;
               }
               else 
                 con->GAPauditdone = TRUE;
            }
            DP("%s: sending AudCfm : spId=%d, numSuConnId=%d\n",
                __FUNCTION__, rmsap->spId,
                rmsap->audCfmEvnt.t.rmGAPAudCfmEvnt.numSuConnId);
            CcLiRmtAudCfm(&rmsap->pst, rmsap->spId, &rmsap->audCfmEvnt);

            RETVALUE(ret);
         }
         else
         {
            DP("%s: sending AudCfm : spId=%d, numSuConnId=%d\n",
                __FUNCTION__, rmsap->spId,
                rmsap->audCfmEvnt.t.rmGAPAudCfmEvnt.numSuConnId);
            /* This must be a duplicate request. Resend the last AudCfm */
            CcLiRmtAudCfm(&rmsap->pst, rmsap->spId, &rmsap->audCfmEvnt);
         }
         break;

      case RMAUD_AWT_SFAUDCFM:
         if (rmAudReqEvnt->t.rmGAPAudReqEvnt.sequence ==
                        rmsap->audCfmEvnt.t.rmGAPAudCfmEvnt.sequence)
         {
            /* message resent by RM - ignore this as we have already forwarded 
               the AudReq to SF */
            break;
         }
      CCLOGERR("%s() Failed, AudReq received while waiting for SftAudCfm\n",
          __FUNCTION__);
      ccGenAlarm(LCM_CATEGORY_INTERNAL, LCC_EVENT_FLOWCONTRL_ERR,
                 LCM_CAUSE_INV_STATE, (U8 *) &suId);
      break;
   }

   RETVALUE(ret);
} /* End of CcLiRmtAudReq() */

/*
*
*       Fun:    CcLiSftAudCfm
*
*       Desc:   Audit confirm received from SF
*
*       Ret:    ROK     - ok
*
*       Notes:  None
*
*       File:   cc_bdy1.c
*
*/
#ifdef ANSI
PUBLIC S16 CcLiSftAudCfm
(
Pst *pst,
SuId suId,
SuId rmsapId
)
#else
PUBLIC S16 CcLiSftAudCfm(pst, suId, rmsapId)
Pst *pst;
SuId suId;
SuId rmsapId;
#endif
{
   CcGenSap *rmsap;
   S16      ret;

   if ((ret = ccGetRmSap(&rmsap, NULLP,  rmsapId)) != ROK)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      CCLOGERROR(ERRCLS_DEBUG, ECC294, (ErrVal) ret,
                 "CcLiRmtAudReq() Failed, ccGetRmSap failed");
#endif
      ccGenAlarm(LCM_CATEGORY_INTERNAL, LCC_EVENT_INV_RMSAP,
                 LCM_CAUSE_INV_SAP, (U8 *) &suId);
      RETVALUE(RFAILED);
   }

   if (suId != 0)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      CCLOGERROR(ERRCLS_DEBUG, ECC294, (ErrVal) ret,
                 "CcLiRmtAudReq() Failed, invalid suId for SFSAP");
#endif
      ccGenAlarm(LCM_CATEGORY_INTERNAL, LCC_EVENT_INV_SFSAP,
                 LCM_CAUSE_INV_SAP, (U8 *) &suId);
      RETVALUE(RFAILED);
   }

   /* Stop the timer that was waiting on this AudCfm from SF */
   ccStopSapTmr((PTR)rmsap, TMR_RM_SFAUDCFM);

   switch (rmsap->audState)
   {
   case RMAUD_AWT_RMAUDREQ:
      /* It could be a confirm for a resent SftAudReq - ignore it */
      break;
   case RMAUD_AWT_SFAUDCFM:
      /* Cfm from SF received - confirm back to RM and change the state */

      rmsap->audState = RMAUD_AWT_RMAUDREQ;

      CcLiRmtAudCfm(&rmsap->pst, rmsap->spId, &rmsap->audCfmEvnt);

      break;
   }
  
#ifdef ZC
   zcUpdPeer();
#endif /* ZC */
   RETVALUE(ROK);

} /* end of CcLiSftAudCfm() */

/* TELICA-jwh-04/24/2001:  Function to compute Call Control memory needs. */
#if 1   /* Start of code change */
#include "tsmc_memsize.h"
int tsmcMemsizeCc(TsmcMemsizeEnt_t *cb)
{
    int i;

    if (cb->ent != ENTCC)
    {
        GDP(__FILE__, __LINE__, "Error:  ent=%0x02X\n",  cb->ent);
        return(-1);
    }

    cb->genCfgSize = 3*(sizeof(CcGenSap *) + sizeof(CcGenSap)) + sizeof(CcPsSap *) + (100+255)*sizeof(CmListEnt);

    for(i=0; i<cb->numProt; i++)
    {
        TsmcMemsizeProt_t *protCb = &cb->protList[i];
        TsmcMemsizeParams_t *protParams = protCb->protParams;

        switch (protCb->protType)
        {
        case TSMC_MEMSIZE_PROT_ISDN:

            /* Memory allocation that is independent of user configuration */
            cb->genCfgSize += protParams->maxProtVariant*(sizeof(CcPsSap) + (255)*sizeof(CmListEnt));

            /* Memory allocation that is dependent on user configuration */
            protCb->perCicCfg = 0;
            protCb->perIfCfg = sizeof(CcIntfcSAPCb);

            /* Per call, per direction memory allocation */
            protCb->perCallperDir = sizeof(CcConCb)/2 + sizeof(CcRouteInfo)/2;

            break;

        case TSMC_MEMSIZE_PROT_SS7:

            /* Memory allocation that is independent of user configuration */
            cb->genCfgSize += protParams->maxProtVariant*(sizeof(CcPsSap) + (255)*sizeof(CmListEnt));

            /* Memory allocation that is dependent on user configuration */
            protCb->perCicCfg = 0;
            protCb->perIfCfg = sizeof(CcIntfcSAPCb);

            /* Per call, per direction memory allocation */
            protCb->perCallperDir = sizeof(CcConCb)/2 + sizeof(CcRouteInfo)/2;
            break;
        default:
            return(-1);
        }

    }
   return(0);
}
#endif  /* End of code change */

/* TELICA - Bugzilla-4321 - Bukucu - 10.25.01 - new stats handling funcs */
void ccRtrvSts(CcMngmt *sts, void *cb, int elmnt)
{
   CcGenCp *genCb;
   CcPsSap *sapCb;

   switch(elmnt)
   {
      case STGEN:
         genCb = (CcGenCp *) cb;


         sts->t.sts.s.ccGenSts.totalCalls = genCb->sts.totalCalls;
         sts->t.sts.s.ccGenSts.answered = genCb->sts.answered;
         /* Bug 59761 */
         sts->t.sts.s.ccGenSts.unAnswered = genCb->sts.unAnswered;
         sts->t.sts.s.ccGenSts.inInternalUnAnswered = genCb->sts.inInternalUnAnswered; /*Bug 68186*/
         sts->t.sts.s.ccGenSts.fRoutUnavail = genCb->sts.fRoutUnavail;
         sts->t.sts.s.ccGenSts.fRoutUnavailForDisabledDpc = genCb->sts.fRoutUnavailForDisabledDpc;
         sts->t.sts.s.ccGenSts.fResUnavail = genCb->sts.fResUnavail;
         sts->t.sts.s.ccGenSts.standing = ccGetStandingCalls (1, &(sts->t.sts.s.ccGenSts.inInternalStdgCalls),
                                                              &(sts->t.sts.s.ccGenSts.outInternalStdgCalls));
         sts->t.sts.s.ccGenSts.lstInitNA = genCb->sts.lstInitNA;
         sts->t.sts.s.ccGenSts.lstInitRx = genCb->sts.lstInitRx;
         sts->t.sts.s.ccGenSts.lstInitTx = genCb->sts.lstInitTx;
         sts->t.sts.s.ccGenSts.updThrshld = genCb->sts.updThrshld;
         sts->t.sts.s.ccGenSts.totalLnpQueries = genCb->sts.totalLnpQueries;
         sts->t.sts.s.ccGenSts.totalLnpFail = genCb->sts.totalLnpFail;
         sts->t.sts.s.ccGenSts.totalLnpVldResp = genCb->sts.totalLnpVldResp;
         sts->t.sts.s.ccGenSts.totalLnpMisRtdCalls = genCb->sts.totalLnpMisRtdCalls;
         sts->t.sts.s.ccGenSts.totalLnpClsRelByDonSwch = genCb->sts.totalLnpClsRelByDonSwch;
         sts->t.sts.s.ccGenSts.totalLnpClsRelByOurSwch = genCb->sts.totalLnpClsRelByOurSwch;
         /* FID 14382.8 */
         sts->t.sts.s.ccGenSts.totalLnpCallForPortSub = genCb->sts.totalLnpCallForPortSub;
         sts->t.sts.s.ccGenSts.totalLnpCallSubPortedInNet = genCb->sts.totalLnpCallSubPortedInNet;

         /* added for bug:9458, nsv */
         sts->t.sts.s.ccGenSts.totalLnpClsRelwCC41 = genCb->sts.totalLnpClsRelwCC41;
         /* Bug# 28150 */
         sts->t.sts.s.ccGenSts.totalBillingCalls = genCb->sts.totalBillingCalls;
         sts->t.sts.s.ccGenSts.noCdrCalls = genCb->sts.noCdrCalls;
         sts->t.sts.s.ccGenSts.inInternalAnsCalls = genCb->sts.inInternalAnsCalls;

         /*Bug:28882 : new counters for RLT */
         sts->t.sts.s.ccGenSts.rmtXferIn = genCb->sts.rmtXferIn;
         sts->t.sts.s.ccGenSts.rmtXferOut = genCb->sts.rmtXferOut;
         sts->t.sts.s.ccGenSts.tromboneOut = genCb->sts.tromboneOut;
         /* FID 7803 +*/
         sts->t.sts.s.ccGenSts.totalForkedCalls = genCb->sts.totalForkedCalls;
         /* FID 7803 -*/
         
         /* FID-2593: Transcoding */
         sts->t.sts.s.ccGenSts.sip2SipPermMg = genCb->sts.sip2SipPermMg;
         /* FID-2575: LMSD */
         sts->t.sts.s.ccGenSts.sip2SipTransMg = genCb->sts.sip2SipTransMg;
         
         /* FID 14234.0 */
         sts->t.sts.s.ccGenSts.totalRBWFVirtualCall = genCb->sts.totalRBWFVirtualCall;
         sts->t.sts.s.ccGenSts.totalRBWFRealCall    = genCb->sts.totalRBWFRealCall;

         /* FID 14419.0 */
         sts->t.sts.s.ccGenSts.totalNormRejOvldRx = genCb->sts.totalNormRejOvldRx;
         sts->t.sts.s.ccGenSts.totalNormRejOvldTx = genCb->sts.totalNormRejOvldTx;
         sts->t.sts.s.ccGenSts.totalPrioRejOvldRx = genCb->sts.totalPrioRejOvldRx;
         sts->t.sts.s.ccGenSts.totalPrioRejOvldTx = genCb->sts.totalPrioRejOvldTx;

         /* FID 14382.15 */                                                          
         sts->t.sts.s.ccGenSts.caleaAttempt = genCb->sts.caleaAttempt;
         sts->t.sts.s.ccGenSts.caleaAnswered = genCb->sts.caleaAnswered;

         sts->t.sts.s.ccGenSts.InVideo = genCb->sts.InVideo;
         sts->t.sts.s.ccGenSts.OutVideo = genCb->sts.OutVideo;
         sts->t.sts.s.ccGenSts.InMSRP = genCb->sts.InMSRP;
         sts->t.sts.s.ccGenSts.OutMSRP = genCb->sts.OutMSRP;

         /* FID 15319.0 */
         sts->t.sts.s.ccGenSts.totalInExtBicc  = genCb->sts.totalInExtBicc;
         sts->t.sts.s.ccGenSts.totalOutExtBicc = genCb->sts.totalOutExtBicc;

         /* FID 15801.0 (LGP-384) */
         sts->t.sts.s.ccGenSts.ccSip3xxSts.noOfSip3xxCalls          = genCb->sts.ccSip3xxSts.noOfSip3xxCalls;
         sts->t.sts.s.ccGenSts.ccSip3xxSts.noOfSip3xxFailUnAnsCalls = genCb->sts.ccSip3xxSts.noOfSip3xxFailUnAnsCalls;
         sts->t.sts.s.ccGenSts.ccSip3xxSts.noOfSip3xxURIProcessed   = genCb->sts.ccSip3xxSts.noOfSip3xxURIProcessed;
         sts->t.sts.s.ccGenSts.ccSip3xxSts.noOfSip3xxAns1URI        = genCb->sts.ccSip3xxSts.noOfSip3xxAns1URI;
         sts->t.sts.s.ccGenSts.ccSip3xxSts.noOfSip3xxAns2URI        = genCb->sts.ccSip3xxSts.noOfSip3xxAns2URI;
         sts->t.sts.s.ccGenSts.ccSip3xxSts.noOfSip3xxAnsMore2URI    = genCb->sts.ccSip3xxSts.noOfSip3xxAnsMore2URI;

         /* Bug 59761 */
         /* Check for Bad answered count */
         if (genCb->sts.answered > genCb->sts.totalCalls)
         {
            CCLOGERR("ccRtrvSts - WARNING -  answered count =%ld is invalid,"
                   " totalCalls=%ld \n", genCb->sts.answered, genCb->sts.totalCalls);
         }

         /* Check for Bad unAnswered count */
         if(genCb->sts.totalCalls - genCb->sts.answered != sts->t.sts.s.ccGenSts.unAnswered)
         {
            CCLOGERR("ccRtrvSts - WARNING - UnAnswered calls Not "
                       "accounted for totalCalls:%ld, answered:%ld, unAnswered:%ld\n",
                        genCb->sts.totalCalls, genCb->sts.answered,
                        sts->t.sts.s.ccGenSts.unAnswered);
         }

         CCDP(AIN_DBG_LEVEL_0, "ccRtrvSts - totalCalls:%ld, answered:%ld, unAnswered:%ld,"
              "inIntUnAnswered:%ld, standing:%ld, inIntStdgCalls:%ld, outIntStdgCalls:%ld, totalBillingCalls:%ld, "
              "noCdrCalls:%ld, inInternalAnsCalls:%ld, totalForkedCalls:%ld, "
              "InVideo=%ld, OutVideo=%ld, totalInExtBicc:%ld, totalOutExtBicc:%ld\n",
              genCb->sts.totalCalls, genCb->sts.answered,
              sts->t.sts.s.ccGenSts.unAnswered, 
              sts->t.sts.s.ccGenSts.inInternalUnAnswered, /*Bug 68186*/ 
	      sts->t.sts.s.ccGenSts.standing,
              sts->t.sts.s.ccGenSts.inInternalStdgCalls,
              sts->t.sts.s.ccGenSts.outInternalStdgCalls, genCb->sts.totalBillingCalls,
              genCb->sts.noCdrCalls, genCb->sts.inInternalAnsCalls,
              genCb->sts.totalForkedCalls,
              sts->t.sts.s.ccGenSts.InVideo, sts->t.sts.s.ccGenSts.OutVideo,
              sts->t.sts.s.ccGenSts.totalInExtBicc,sts->t.sts.s.ccGenSts.totalOutExtBicc);

         CCDP(AIN_DBG_LEVEL_0, "InMSRP=%ld, OutMSRP=%ld, sip2SipPermMg %ld, sip2SipTransMg %ld\n",
              sts->t.sts.s.ccGenSts.InMSRP, sts->t.sts.s.ccGenSts.OutMSRP,
              sts->t.sts.s.ccGenSts.sip2SipPermMg, sts->t.sts.s.ccGenSts.sip2SipTransMg);

         CCDP(AIN_DBG_LEVEL_0,"%s()\n "
              "noOfSip3xxCalls          = %ld\n"
              "noOfSip3xxFailUnAnsCalls = %ld\n"
              "noOfSip3xxURIProcessed   = %ld\n"
              "noOfSip3xxAns1URI        = %ld\n"
              "noOfSip3xxAns2URI        = %ld\n"
              "noOfSip3xxAnsMore2URI    = %ld\n", __FUNCTION__,  
              sts->t.sts.s.ccGenSts.ccSip3xxSts.noOfSip3xxCalls,
              sts->t.sts.s.ccGenSts.ccSip3xxSts.noOfSip3xxFailUnAnsCalls,
              sts->t.sts.s.ccGenSts.ccSip3xxSts.noOfSip3xxURIProcessed,
              sts->t.sts.s.ccGenSts.ccSip3xxSts.noOfSip3xxAns1URI,
              sts->t.sts.s.ccGenSts.ccSip3xxSts.noOfSip3xxAns2URI,
              sts->t.sts.s.ccGenSts.ccSip3xxSts.noOfSip3xxAnsMore2URI);

         /* FID 14457.0 */
         sts->t.sts.s.ccGenSts.totalGetsRejOvldRx = genCb->sts.totalGetsRejOvldRx;
         sts->t.sts.s.ccGenSts.totalGetsRejOvldTx = genCb->sts.totalGetsRejOvldTx;
         /* FID 16970.0 */
         sts->t.sts.s.ccGenSts.totalCCBSVirtual = genCb->sts.totalCCBSVirtual;
         sts->t.sts.s.ccGenSts.totalCCBSReal = genCb->sts.totalCCBSReal;
         break;
      case STPSSAP:
         sapCb = (CcPsSap *) cb;
         sts->t.sts.s.ccPsSapSts.incoming = sapCb->sts.incoming;
         sts->t.sts.s.ccPsSapSts.outgoing = sapCb->sts.outgoing;
         /* FID 14382.15 */
         sts->t.sts.s.ccPsSapSts.icglare = sapCb->sts.icglare;
         sts->t.sts.s.ccPsSapSts.ogglare = sapCb->sts.ogglare;

      default:
         break;
   }
}

/* Bug: 14442 tyakin */
S16 ccUpdPeerSts(void)
{
int i = 0;
#ifdef ZC
   ZcUpdParam upd;
   CcPsSap *cb;

   ccCp.sts.updThrshldCurval = 0; /* reset thrshld cntr */

   /* Bug#16149, rkhan, if not ACTIVE just return */
   if (zcCb.protState != ACTIVE)
   {
      RETVALUE(ROK);
   } 

#if 0 /* RRK TEST */
   gitDebugPrint (__FILE__, __LINE__,"ccUpdPeerSts: maxPsSAP=%d\n",ccCp.genCfg.maxPsSAP);
#endif

   for(; i < ccCp.genCfg.maxPsSAP ;i++){
     cb = *(ccPsSapLstPtr + i);
     if(cb == NULLP)
        continue;

#if 0 /* RRK TEST */
     gitDebugPrint (__FILE__, __LINE__,"ccUpdPeerSts: Updating for suid=%d\n",i);
#endif

     upd.suId = i;
     zcRunTimeUpd(ZC_STATS, CMPFTHA_UPD_REQ, upd);
   }
   zcUpdPeer(); /* Bug#16149, rkhan, changed psf to send all sts update in one attempt */
#endif /* ZC */

   RETVALUE(ROK);
}

void ccInitTrmtSts(void *cb, int elmnt)
{
   CcGenCp *genCb;
   CcPsSap *sapCb;

   switch(elmnt)
   {
      case STGEN:
         genCb = (CcGenCp *) cb;
         /* bug#18354, rkhan, changed to SGetSysUpTime */
     /* Bukucu Bug-19182 changed to SGetSysTime again */
         SGetSysTime(&genCb->sts.lstInitTx);
         break;
      case STPSSAP:
         sapCb = (CcPsSap *) cb;
         sapCb->sts.outgoing = NULLD;
         /* FID 14382.15 */
         sapCb->sts.ogglare = NULLD;
         break;

      default:
         break;
   }
}

void ccInitRcvSts(void *cb, int elmnt)
{
   CcGenCp *genCb;
   CcPsSap *sapCb;

   switch(elmnt)
   {
      case STGEN:
         genCb = (CcGenCp *) cb;
         /* bug#18354, rkhan, changed to SGetSysUpTime */
         /* Bukucu Bug-19182 changed to SGetSysTime again */
         SGetSysTime(&genCb->sts.lstInitRx);
         break;

      case STPSSAP:
         sapCb = (CcPsSap *) cb;
         sapCb->sts.incoming = NULLD;
         /* FID 14382.15 */
         sapCb->sts.icglare = NULLD;
         break;

      default:
         break;
   }
}
void ccInitNASts(void *cb, int elmnt)
{
   CcGenCp *genCb;

   switch(elmnt)
   {
      case STGEN:
         genCb = (CcGenCp *) cb;
         genCb->sts.totalCalls = NULLD;
         genCb->sts.answered   = NULLD;
         genCb->sts.unAnswered   = NULLD;
         genCb->sts.inInternalUnAnswered   = NULLD; /*Bug68186*/
         genCb->sts.fRoutUnavail = NULLD;
         genCb->sts.fRoutUnavailForDisabledDpc = NULLD;
         genCb->sts.fResUnavail = NULLD;
         genCb->sts.standing = NULLD;
         genCb->sts.totalLnpQueries = NULLD;
         genCb->sts.totalLnpFail = NULLD;
         genCb->sts.totalLnpVldResp = NULLD;
         genCb->sts.totalLnpMisRtdCalls = NULLD;
         genCb->sts.totalLnpClsRelByDonSwch = NULLD;
         genCb->sts.totalLnpClsRelByOurSwch = NULLD;
         /* FID 14382.8 */
         genCb->sts.totalLnpCallForPortSub = NULLD;
         genCb->sts.totalLnpCallSubPortedInNet = NULLD;

         /* added for bug:9458 , nsv */
         genCb->sts.totalLnpClsRelwCC41 = NULLD;

         /* Bug# 28150 */
         genCb->sts.totalBillingCalls = NULLD;
         genCb->sts.noCdrCalls = NULLD;
         genCb->sts.inInternalStdgCalls  = NULLD;
         genCb->sts.outInternalStdgCalls = NULLD;
         genCb->sts.inInternalAnsCalls   = NULLD;
         /*Bug:28882 : new counters for RLT */
         genCb->sts.rmtXferIn = NULLD;
         genCb->sts.rmtXferOut = NULLD;
         genCb->sts.tromboneOut = NULLD;
         /* FID 7803 +*/
         genCb->sts.totalForkedCalls     = NULLD;
         /* FID 7803 -*/

         /* FID 7804 */
         cmMemset((U8 *)&genCb->sts.ccOverloadSts, 0, sizeof(CcOverloadSts));

         /* FID-2593: Transcoding */
         genCb->sts.sip2SipPermMg = NULLD;
         /* FID-2575: LMSD */
         genCb->sts.sip2SipTransMg = NULLD;

         /* FID 14234.0 */
         genCb->sts.totalRBWFVirtualCall = NULLD;
         genCb->sts.totalRBWFRealCall    = NULLD;

         /* FID 14419.0 */
         genCb->sts.totalNormRejOvldRx  = NULLD;
         genCb->sts.totalNormRejOvldTx  = NULLD;
         genCb->sts.totalPrioRejOvldRx  = NULLD;
         genCb->sts.totalPrioRejOvldTx  = NULLD;
         
         /* FID 14382.15 */
         genCb->sts.caleaAttempt = NULLD;
         genCb->sts.caleaAnswered = NULLD;

         genCb->sts.InVideo = NULLD;
         genCb->sts.OutVideo = NULLD;
         genCb->sts.InMSRP = NULLD;
         genCb->sts.OutMSRP = NULLD;

         /* FID 15319.0 */
         genCb->sts.totalInExtBicc = NULLD;
         genCb->sts.totalOutExtBicc = NULLD;

         /* FID 15801.0 */
         genCb->sts.ccSip3xxSts.noOfSip3xxCalls          = NULLD;
         genCb->sts.ccSip3xxSts.noOfSip3xxFailUnAnsCalls = NULLD;
         genCb->sts.ccSip3xxSts.noOfSip3xxURIProcessed   = NULLD;
         genCb->sts.ccSip3xxSts.noOfSip3xxAns1URI        = NULLD;
         genCb->sts.ccSip3xxSts.noOfSip3xxAns2URI        = NULLD;
         genCb->sts.ccSip3xxSts.noOfSip3xxAnsMore2URI    = NULLD;

         /* FID 14457.0 */
         genCb->sts.totalGetsRejOvldRx  = NULLD;
         genCb->sts.totalGetsRejOvldTx  = NULLD;
    
         /* FID 15461.0 */
         genCb->sts.mgcfStdgCalls = NULLD;
         genCb->sts.ibcfStdgCalls = NULLD;

         /* FID 16970.0 */
         genCb->sts.totalCCBSVirtual = NULLD;
         genCb->sts.totalCCBSReal = NULLD;

         /* bug#18354, rkhan, changed to SGetSysUpTime */
         /* Bukucu Bug-19182 changed to SGetSysTime again */
         SGetSysTime(&genCb->sts.lstInitNA);
         break;

      default:
         break;
   }
}
/* Bugzilla-4321 */

/* Bugzilla-14864 */
void ccRtrvGccRelSts(CcMngmt *sts, void *cb, int elmnt)
{
   CcGenCp *genCb; 
   CcPsSap *sapCb; 
   int i;  
   Ticks  currTime;

   SGetSysTime(&currTime);

   switch(elmnt)
   {
      case STGENCALLFAIL:
         genCb = (CcGenCp *) cb;
         for(i = 0; i< FC_LAST; i++)
         {
           sts->t.sts.s.ccGenSts.relCondition[i] = genCb->sts.relCondition[i];
           if(genCb->sts.relCondition[i] > 0)
/*               CCDP(CCDP_RELCOND,"CCDP_RECOND : relCondition[%d] count[%ld] \n",i, */
/*                genCb->sts.relCondition[i]); */
             gitDebugPrint(__FILE__, __LINE__, "CCDP_RECOND : relCondition[%d] count[%ld] \n",
                           i, genCb->sts.relCondition[i]);
         }
         sts->t.sts.s.ccGenSts.relConditionTicks = genCb->sts.relConditionTicks;
         break;

      case STPSSAPCALLFAIL:
         sapCb = (CcPsSap *) cb;
         for(i = 0; i< 256; i++)
         {
           sts->t.sts.s.ccPsSapSts.relCause[i] = sapCb->sts.relCause[i];
           if(sapCb->sts.relCause[i] > 0)
/*               CCDP(CCDP_RELCOND,"CCDP_RECAUSE : relCausecode[%d] count[%ld] \n",i, */
/*                sapCb->sts.relCause[i]); */
             gitDebugPrint(__FILE__, __LINE__, "CCDP_RECAUSE : relCausecode[%d] count[%ld] \n",i, 
                           sapCb->sts.relCause[i]);
                           
         }
         sts->t.sts.s.ccPsSapSts.relCauseTicks =( currTime - sapCb->sts.relCauseTicks);
            break;
      default:
         break;
   }
}
void ccInitGccRelSts(void *cb, int elmnt)
{
   CcGenCp *genCb; 
   CcPsSap *sapCb; 
   int i;  

   switch(elmnt)
   {
      case STGENCALLFAIL:
         genCb = (CcGenCp *) cb;
         for(i = 0; i< FC_LAST; i++)
         {
             genCb->sts.relCondition[i] = NULLD;
         }
         /* bug#18354, rkhan, changed to SGetSysUpTime */
         /* Bukucu Bug-19182 changed to SGetSysTime again */
         SGetSysTime(&genCb->sts.relConditionTicks);
         break;

      case STPSSAPCALLFAIL:
         sapCb = (CcPsSap *) cb;
         for(i = 0; i< 256; i++)
         {
             sapCb->sts.relCause[i] = NULLD;
         }
         SGetSysTime(&sapCb->sts.relCauseTicks);
         break;

      default:
         break;
   }
}

/*end  Bugzilla-14864 */




/************************************************************************
 FcInitPst()

 This is the initialization of the post structure used at FIC.

 ************************************************************************/

S16 CcInitPst(
Pst *pst,               /* Post   */
Ent dstEnt,             /* entity */
Selector selector       /* jump Table Index */
)
{
    pst->dstProcId = SFndProcId();
    pst->dstEnt    = dstEnt;
    pst->dstInst   = 0;
    pst->srcProcId = SFndProcId();
    pst->srcEnt    = ENTCC;        /* CC  Interface user entity */
    pst->srcInst   = 0;
    pst->selector  = selector;
    pst->region    = DFLT_REGION;
    pst->pool      = DFLT_POOL;
    pst->prior     = 0;
    pst->route     = RTESPEC;

    return ROK;
}


#ifndef TELICA_MGC
/************************************************************************
 ccInitGDI(void)

 This is the initialization of the post structure used at FIC.

 ************************************************************************/
S16 ccInitGDI(void)
{
   enum gdi_return gret;
   struct gdi_stat status;
   int retVal = ROK;

   /* fixed bug 16370, init ccInit */
   ccInit.ent     = ENTCC;           /* entity */
   ccInit.inst    = 0;               /* instance */
   ccInit.cfgDone = FALSE;

   if(IS_SP2_MASTER)
   {
       retVal = ccDBInit(&dbContext, TELICA_SIG_DB_NAME);
       if(retVal == -1)
       {
           /* WEChin - BUG 14036 */
           procFailure("ccDBInit Failed");
           sleep(10);
           exit(1);
       }
   }

   // Initialize the on switch Billing system
   signal( SIGTERM, cleanup);
   signal( SIGINT, cleanup);

   CdrInitAlarmEnviron();   // initialize billing alarms code

   gret = GDI_open( &status);
   if( gret == GDI_FAIL )
      CCLOGERR("GDI_OPEN Fails !! \n");
   else
      CCDP(AIN_DBG_LEVEL_3, "GDI_OPEN Done  !! \n");

   return (retVal);
}
#endif

CcConCb *ccFindConFromSuConnId(UConnId suConnId)
{
  CcConCb *con = NULLP;
  S16 ret = RFAILED;

  ret = cmHashListFind(&ccSuInstTbl,
                       (U8 *)&suConnId, (U16)sizeof(UConnId),
                       (U16)0, (PTR *) &con);
  if (ret != ROK)
     ret = cmHashListFind(&ccSuOgInstTbl,
                          (U8 *)&suConnId,
                          (U16)sizeof(UConnId), (U16)0, (PTR *) &con); 
  if (ret != ROK)
  {
    CCDP(AIN_DBG_LEVEL_0, "CON Not found %ld \n", suConnId);
  }

  return (con);
}



/* AAG */
/* START: NON-CALL RELATED API's */
/*
*
*      Fun:   CcLiRmtStsChgCfm
*
*      Desc:  This is returned when status change is detected for a requested
*             status change service.
*
*      Ret:   ROK     - successful,
*
*      Notes: None.
*
*      File:  cc_bdy1.c
*
*/
PUBLIC S16 CcLiRmtStsChgCfm
(
Pst    *pst,             /* post                                    */
SuId   suId,             /* service user id                         */
U16    srvId,            /* service ID                              */
ProtType protType,       /* protocol Type                           */
RmRsc *resource,         /* Resource                                */
TknU8  facSts,           /* echo parameter that came in the request */
TknU8  failCs,		 /* Fail Cause 				    */
TknU8  stsCs,            /* Status Cause                            */
TknU8  facState          /* Facility state change                   */
)
{
    /* AAG: Sip Normalization */

    MonitorForChgCfm_t pMonChgCfm;
    SvcHeader_t        pHeader;

    TRC3(CcLiRmtStsChgCfm)

    CCDP(AIN_DBG_LEVEL_2, "%s: srvId=%d, suId=%d, protType=%d, failCs.pres=%d val=%d\n",
         __FUNCTION__,  srvId, suId, protType, failCs.pres, failCs.val);

    stsCs.pres = NOTPRSNT;

    if (failCs.pres == PRSNT_NODEF)
    {
        switch (failCs.val)
        {
        case ClcSuccess:
            /* 
             * If the fail cause is ClsSuccess,
             * reply back with status cause StatusCauseMatch
             */
            stsCs.pres = PRSNT_NODEF;
            stsCs.val = StatusCauseMatch;
            break;

        case ClcResourceNotAvailable:
            /* 
             * If the fail cause is ClcResourceNotAvailable,
             * reply back with no status cause 
             */
            stsCs.pres = NOTPRSNT;
            break;
    
        default:
            /* 
             * If the fail cause is others,
             * reply back with status cause StatusCauseError 
             */
            stsCs.pres = PRSNT_NODEF;
            stsCs.val = StatusCauseError;
            break;

        }
    } 

    cmMemset((U8 *)&pMonChgCfm, 0, sizeof(MonitorForChgCfm_t));
    cmMemset((U8 *)&pHeader, 0, sizeof(SvcHeader_t));

    /* Map the information from resource to usrId to send back to FIC */

    if (resource->intfc.intfType == CC_G3_INTFC)
    {
        /* GR303 interface */
        pMonChgCfm.usrId.protType = FIC_GR303;
    }
    else if (resource->intfc.intfType == CC_CS_LN_INTFC)
    {
        /* CAS line interface */
        pMonChgCfm.usrId.protType = FIC_CAS;
    }

    pMonChgCfm.usrId.u.sub.intfcId.pres = PRSNT_NODEF;
    pMonChgCfm.usrId.u.sub.intfcId.val = resource->intfc.t.intfcId;

    pMonChgCfm.usrId.pres = PRSNT_NODEF;

    /* Echo back the service ID data */
    pMonChgCfm.srvcId = srvId;

    /* Echo back the facility status data */
    pMonChgCfm.facilityStatus.pres = facSts.pres;
    pMonChgCfm.facilityStatus.val = facSts.val;

    /* Fill the fail cause data */
    pMonChgCfm.failCause.pres = failCs.pres;
    pMonChgCfm.failCause.val = failCs.val;

    /* Fill the status cause data */
    pMonChgCfm.statusCause.pres = stsCs.pres;
    pMonChgCfm.statusCause.val = stsCs.val;

    /* Bug 46255, fill the facility state change data */
    pMonChgCfm.facilityState.pres = facState.pres;
    pMonChgCfm.facilityState.val = facState.val;

    CCDP(AIN_DBG_LEVEL_2, "%s: CcUiFctMonitorForChgCfm with intfcId:%ld, protType=%d, facSts.pres=%d val=%d, stsCs.pres=%d val=%d, facState.pres=%d, val=%d\n",
         __FUNCTION__, pMonChgCfm.usrId.u.sub.intfcId.val,
         pMonChgCfm.usrId.protType,
         pMonChgCfm.facilityStatus.pres, pMonChgCfm.facilityStatus.val,
         pMonChgCfm.statusCause.pres, pMonChgCfm.statusCause.val,
         pMonChgCfm.facilityState.pres, pMonChgCfm.facilityState.val);

     /* Call the API to send the confirmation back to FIC */
    CcUiFctMonitorForChgCfm(&pMonChgCfm, &pHeader);

    RETVALUE(ROK);

} /* end of CcLiRmtStsChgCfm */

/* 
*       
*      Fun:   CcLiCctMsgInd
*       
*      Desc:  process an incoming Message Indication from PSIF Layer.
*
*      Ret:   ROK     - successful, 
*       
*      Notes: None.
*
*      File:  cc_bdy1.c
*
*/
PUBLIC S16 CcLiCctMsgInd 
(
Pst       *pst,               /* post                                         */
SuId      suId,               /* service user identier                        */
UConnId   suConnId,           /* service user instance identifier             */ 
UConnId   spConnId,           /* service provider instance identifier         */
ProtType  protType,           /* incoming protocol type                       */
CcMsgEvnt *msgEvnt            /* connection release event structure           */
)
{
    CcPsSap        *pCb        = NULLP;
    SvcHeader_t     svcHeader;
    /* FID16206.0 + */
    CcConCb *con = NULLP;
    DummyCallInfo *dummyCallInfo = NULLP;
    S16 ret = RFAILED;
    PTR dupEntry = NULLP;             /* For checking duplicate item */
    RmInterface intfc;
    /* FID16206.0 -*/
    CCDP(AIN_DBG_LEVEL_1, "%s:\n", __FUNCTION__);

    if (zcCb.protState != ACTIVE)
    {
        CCLOGERR("Ignoring for standby:%s: spConnId:%ld, protType:%d\n",
                 __FUNCTION__, spConnId, protType);
        RETVALUE(RFAILED);
    }

#if 1 /* TELICA - Bugzilla-5520- Khan/Yakin - 08/01/2001 - code trace */
    TELICACODETRC(LOGDEBUG, ccCodeTrcLvl, ccInit.lmPst, ("CcLiCctMsgInd::"
                  "spConnId=%lx, protType=%d", spConnId, protType));
#endif /* Bugzilla-5520 */

    if ((ccInit.cfgDone != TRUE) || (suId >= (SuId) ccCp.genCfg.maxPsSAP))
    {
        CCLOGERROR(ERRCLS_INT_PAR, ECC246, (ErrVal) suId,
                   "CcLiCctMsgInd() Failed, invalid suId");
        RETVALUE(RFAILED);
    }

    if ((ccPsSapLstPtr == NULLP) || ((pCb = *(ccPsSapLstPtr + suId)) == NULLP))
    {
        CCLOGERROR(ERRCLS_INT_PAR, ECC247, (ErrVal) suId,
                   "CcLiCctMsgInd() Failed, invalid suId");

#if 1 /* TELICA - Bugzilla-5520- Khan/Yakin - 08/01/2001 - code trace */
        TELICACODETRC(LOGERROR, ccCodeTrcLvl, ccInit.lmPst, ("CcLiCctMsgInd::"
                      "Invalid suId, spConnId=%lx, suId=%d", spConnId, suId));
#endif /* Bugzilla-5520 */

        ccGenAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT,
                   LCM_CAUSE_INV_SAP, (U8*) &suId);
        RETVALUE(RFAILED);
    }

    if (pCb->state != CC_BND)
    {
        CCLOGERROR(ERRCLS_INT_PAR, ECC248, (ErrVal) suId,
                   "CcLiCctMsgInd() Failed, SAP not bound");

#if 1 /* TELICA - Bugzilla-5520- Khan/Yakin - 08/01/2001 - code trace */
        TELICACODETRC(LOGERROR, ccCodeTrcLvl, ccInit.lmPst, ("CcLiCctMsgInd::"
                      "spConnId=%lx, SAP not bound, pcbState=%d",
                      spConnId, pCb->state));
#endif /* Bugzilla-5520 */

        ccGenAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_INV_STATE,
                   LCM_CAUSE_INV_SAP, (U8*) &suId);
        RETVALUE(RFAILED);
    }
    /*FID16206.0 +*/
    /*FID 16970.0 inter-ccs*/
    if((protType == CC_BICCETSI) && (msgEvnt->m.biccMsgEvnt.callIndpMsgType.eh.pres) &&
      (msgEvnt->m.biccMsgEvnt.callIndpMsgType.callIndpMsgType.pres) && 
      (msgEvnt->m.biccMsgEvnt.callIndpMsgType.callIndpMsgType.val == BICC_REGISTER_REQ) ) /* FID16970.0 */
    {
        ret = ccbsEgressCcsInitCall(con,pCb,suId,suConnId,spConnId,protType,msgEvnt);
        if(ret != ROK)
        {
            CCLOGERR("%s: ccbsEgressCcsInitCall failed.\n",__FUNCTION__);
            RETVALUE(RFAILED);
        }
        RETVALUE(ROK);
    }

    if((msgEvnt->m.biccMsgEvnt.callIndpMsgType.eh.pres) &&
      (msgEvnt->m.biccMsgEvnt.callIndpMsgType.callIndpMsgType.pres) && 
      ( (msgEvnt->m.biccMsgEvnt.callIndpMsgType.callIndpMsgType.val == FACILITY_IND) ||
        (msgEvnt->m.biccMsgEvnt.callIndpMsgType.callIndpMsgType.val == FACILITY_SUSPEND) ||
        (msgEvnt->m.biccMsgEvnt.callIndpMsgType.callIndpMsgType.val == FACILITY_RESUME) ) ) /* FID16970.0 */
    {
        ret = cmHashListFind (&ccSuInstTbl, (U8 *)&suConnId, (U16)sizeof(UConnId),
                              (U16)0, (PTR *) &con);

        if (ret != ROK) 
        {    
            ret = cmHashListFind(&ccSuOgInstTbl, (U8 *)&suConnId,
                                 (U16)sizeof(UConnId), (U16)0, (PTR *) &con); 
        }    

        if (ret != ROK) 
        {    
            CCLOGERR("****** Alert : context not found : su:0x%lx, sp:0x%lx ***\n",
                         suConnId, spConnId);
            RETVALUE(RFAILED);
        }    

        if (con->ccbsIsdnCallInfo == NULL)
        {    
            CCDP(AIN_DBG_LEVEL_2, "ccbsIsdnCallInfo is NULL.\n");
            RETVALUE(RFAILED);
        }    

        con->direction = ccGetDirection(con, spConnId, suConnId, pCb);

        CCDP(AIN_DBG_LEVEL_2, "%s. receive BICC FAC IND CCBSINFO. enter CC FSM.\n", __FUNCTION__);

        con->icEvntType = msgEvnt->m.biccMsgEvnt.callIndpMsgType.callIndpMsgType.val;
        /*ccConE99S152*/
        ccCallStateMachine (con, CCE_NCIND, (PTR)msgEvnt, suId, spConnId,
                             (PTR) NULLP, (PTR) NULLP);

        RETVALUE(ROK);
    }


    if(((protType == CC_INETSI) && (msgEvnt->m.dcEvnt.type == DUMMYFAC_IND)) ||
        ((CC_GET_PROTOCOL(protType) == CC_IN) && (msgEvnt->m.dcEvnt.type == IN_REGISTER_IND)) ) /* FID16970.0 */
    {

        if (CC_CONGESTION)
        {
            if (ccCongestion == 0)
            {
                ccCongestion = 2;
                ccGenAlarm(LCM_CATEGORY_RESOURCE, LCM_EVENT_GCC_CONG, LCM_CAUSE_GCC_CONG_LVL2, NULL);
            }
            return(ROK);
        }
        else if (ccCongestion != 0)
        {
            ccCongestion = 0;
            ccGenAlarm(LCM_CATEGORY_RESOURCE, LCM_EVENT_GCC_CONG, LCM_CAUSE_GCC_CONG_CLR, NULL);
        }

        if ((ret = ccInitCon(&con)) != ROK)
        {
#if (ERRCLASS & ERRCLS_ADD_RES)
            CCLOGERROR(ERRCLS_ADD_RES, ECC249, (ErrVal) ret,
                          "CcLiCctConInd() Failed, ccInitCon failed");
#endif

#if 1 /* TELICA - Bugzilla-5520- Khan/Yakin - 08/01/2001 - code trace */
            TELICACODETRC(LOGERROR, ccCodeTrcLvl, ccInit.lmPst, ("CcLiCctConInd::spConnId=%lx, ccInitCon failed", spConnId));
#endif
            RETVALUE(ROK);
        }
        con->icSuConnId   = ccGetNewSuConnId();
        con->origIcSuConnId = con->icSuConnId;

        con->ogSuConnId   = ccGetNewSuConnId();
        con->origOgSuConnId = con->ogSuConnId;
        con->icSpConnId = spConnId;
        con->icProtType = protType;
        con->icSapCb    = pCb;
        con->icSapCb->nmbActvConn++;

        if ((msgEvnt->m.dcEvnt.type == IN_REGISTER_IND) &&
           (msgEvnt->m.dcEvnt.u.rInd.inRegEvnt->facilityStr.eh.pres == PRSNT_NODEF) && 
             (msgEvnt->m.dcEvnt.u.rInd.inRegEvnt->facilityStr.facilityStr.val[0] != PROT_PRFL_RMT_OPRA) )
        {
            CCDP(AIN_DBG_LEVEL_2, "invalid protocol id in register message, send release complete back.\n");
            CcRelEvnt ccRelEvnt;
            INITEVENT(&ccRelEvnt, sizeof(CcRelEvnt));
            ccRelEvnt.m.inDiscEvnt.causeDgn[0].causeVal.pres = PRSNT_NODEF;
            ccRelEvnt.m.inDiscEvnt.causeDgn[0].causeVal.val   = CCINVINFOEL;
            ccRelEvnt.m.inDiscEvnt.causeDgn[0].eh.pres = PRSNT_NODEF;
            ccRelEvnt.m.inDiscEvnt.causeDgn[0].location.pres = PRSNT_NODEF;
            ccRelEvnt.m.inDiscEvnt.causeDgn[0].location.val = LOC_PUBNETLU; 
            ccRelEvnt.m.inDiscEvnt.causeDgn[0].codeStand3.pres = PRSNT_NODEF;
            ccRelEvnt.m.inDiscEvnt.causeDgn[0].codeStand3.val = CSTD_CCITT;
            ccRelEvnt.m.inDiscEvnt.causeDgn[0].recommend.pres = NOTPRSNT;
            ccRelEvnt.m.inDiscEvnt.causeDgn[0].dgnVal.pres = NOTPRSNT;
			ccRelEvnt.m.inDiscEvnt.checkRelComp = TRUE;
            CcLiCctRelRsp(&con->icSapCb->pst, con->icSapCb->spId, con->icSpConnId,
                                      con->icSuConnId, con->icProtType, &ccRelEvnt, NULLP);
            ccRelCon(con);
            RETVALUE(ROK);
        }

        con->direction  = ccGetDirection(con, spConnId, suConnId, pCb);;
        intfc.intfType = CC_IN_INTFC;

        if(msgEvnt->m.dcEvnt.type == DUMMYFAC_IND)
           intfc.t.intfcId = msgEvnt->m.dcEvnt.u.dfInd.intfcId;
        else
           intfc.t.intfcId = msgEvnt->m.dcEvnt.u.rInd.intfcId;
        con->icEvntType = msgEvnt->m.dcEvnt.type;

        CCDP(AIN_DBG_LEVEL_2, "MsgInd intfcId %ld \n", intfc.t.intfcId);
        if ((ret = ccGetIntfcSap (&con->icIntfcCb, &intfc)) != ROK)
        {
#if (ERRCLASS & ERRCLS_DEBUG)
            CCLOGERROR(ERRCLS_DEBUG, ECC461, (ErrVal) ret,
                         "ccAssignSaps() Failed, ccGetIntfcSap failed");
#endif
            TELICA_FREE_CONNECTION (con);
            RETVALUE(RFAILED);
        }

        con->icRsc.intfc = intfc;

        ret = cmHashListFind(&ccSuOgInstTbl, (U8 *)&con->icSuConnId,
                              (U16)sizeof(UConnId), 0, &dupEntry);
        if (ret != ROK)
            ret = cmHashListInsert(&ccSuInstTbl, (PTR)con, (U8 *)&con->icSuConnId,
                                    (U16)sizeof(UConnId));
        if (ret != ROK)
        {
            ccDropEvnt(con);
            SPutSBuf(ccInit.region, ccInit.pool, con->routeInfo,
                     (Size)sizeof(CcRouteInfo));
#if (ERRCLASS & ERRCLS_DEBUG)
            CCLOGERROR(ERRCLS_DEBUG, ECC251, (ErrVal) ret,
                       "CcLiCctIndpFacInd() Failed, cmHashListInsert failed in suInstTbl");
#endif

#if 1 /* TELICA - Bugzilla-5520- Khan/Yakin - 08/01/2001 - code trace */
            TELICACODETRC(LOGERROR, ccCodeTrcLvl, ccInit.lmPst, ("CcLiCctIndpFacInd::spConnId=%lx, icSuConnId=%lx, cmHashListInsert failed in suInstTbl", spConnId, con->icSuConnId));
#endif /* Bugzilla-5520 */

            ccGenAlarm(LCM_CATEGORY_INTERNAL, LCC_EVENT_HASHING_FAILED,
                       LCC_CAUSE_SUINSTTBL_INS, (U8 *) &con->icSuConnId);
            TELICA_FREE_CONNECTION (con); /* Bug 78160 */
            RETVALUE(RFAILED);
        }

    /* Find the ogSuConnId in the ccSuInstTbl */
        ret = cmHashListFind(&ccSuInstTbl, (U8 *)&con->ogSuConnId,
                             (U16)sizeof(UConnId), 0, &dupEntry);
        if (ret != ROK)
            ret = cmHashListInsert(&ccSuOgInstTbl, (PTR)con, (U8 *)&con->ogSuConnId,
                                    (U16)sizeof(UConnId));
        if (ret != ROK)
        {
            ccDropEvnt(con);
            SPutSBuf(ccInit.region, ccInit.pool, con->routeInfo,
                     (Size)sizeof(CcRouteInfo));
#if (ERRCLASS & ERRCLS_DEBUG)
            CCLOGERROR(ERRCLS_DEBUG, ECC251, (ErrVal) ret,
                       "CcLiCctIndpFacInd() Failed, cmHashListInsert failed in suOgInstTbl");
#endif

#if 1 /* TELICA - Bugzilla-5520- Khan/Yakin - 08/01/2001 - code trace */
            TELICACODETRC(LOGERROR, ccCodeTrcLvl, ccInit.lmPst, ("CcLiCctIndpFacInd::spConnId=%lx, ogSuConnId=%lx, "
                          "cmHashListInsert failed in suOgInstTbl", spConnId, con->ogSuConnId));
#endif /* Bugzilla-5520 */

            ccGenAlarm(LCM_CATEGORY_INTERNAL, LCC_EVENT_HASHING_FAILED,
                       LCC_CAUSE_SUINSTTBL_INS, (U8 *) &con->ogSuConnId);
            cmHashListDelete(&ccSuInstTbl, (PTR)con);
            TELICA_FREE_CONNECTION (con);
            RETVALUE(RFAILED);
        }

#if 1 /* TELICA - Bugzilla-5520- Khan/Yakin - 08/01/2001 - code trace */
        TELICACODETRC(LOGDEBUG, ccCodeTrcLvl, ccInit.lmPst, ("CcLiCctIndpInd::ogSuConnId=%lx, icSuConnId=%lx",con->ogSuConnId, con->icSuConnId));
#endif /* Bugzilla-5520 */
        ccCallStateMachine (con, CCE_NCIND, (PTR)msgEvnt, suId, spConnId,
                        (PTR) NULLP, (PTR) NULLP);
        RETVALUE(ROK);
    }

    if((CC_GET_PROTOCOL(protType) == CC_SIP) && (msgEvnt->m.ncEvnt.type == MESSAGE_IND))
    {
        CcMsgEvnt rspEvnt;
        cmMemset((U8 *)&rspEvnt, 0, sizeof(CcMsgEvnt));
        rspEvnt.m.ncEvnt.type = MESSAGE_RSP;
        rspEvnt.m.ncEvnt.protType = protType;
        rspEvnt.m.ncEvnt.u.msgRsp.statusCode.pres = PRSNT_NODEF;

        ret = cmHashListFind (&ccDummyCallInfoTbl, (U8 *)msgEvnt->m.ncEvnt.u.msgInd.inReplyTo.val,
                                   (U16)msgEvnt->m.ncEvnt.u.msgInd.inReplyTo.len, (U16)0, (PTR *) &dummyCallInfo);
        if(ret != ROK)
        {
           CCDP(AIN_DBG_LEVEL_2, "%s: can't find the dummyCallInfo\n", __FUNCTION__);
           rspEvnt.m.ncEvnt.u.msgRsp.statusCode.val = SIPCAUSE_405_METHD;
           CcLiCctMsgRsp(&pCb->pst, 0, spConnId, suConnId, protType, &rspEvnt);
           RETVALUE(RFAILED);
        }

        con = dummyCallInfo->con;   
        if(con == NULLP)
        {
           CCDP(AIN_DBG_LEVEL_2, "%s: the connection block is NULLP\n", __FUNCTION__);
           rspEvnt.m.ncEvnt.u.msgRsp.statusCode.val = SIPCAUSE_405_METHD;
           CcLiCctMsgRsp(&pCb->pst, 0, spConnId, suConnId, protType, &rspEvnt);
           RETVALUE(RFAILED);
        }

        rspEvnt.m.ncEvnt.u.msgRsp.statusCode.val = SIPCNSTS_200_OK;
        CcLiCctMsgRsp(&pCb->pst, 0, spConnId, suConnId, protType, &rspEvnt);

        con->icEvntType = msgEvnt->m.ncEvnt.type;
        con->direction = OUTTOINC;
        ccCallStateMachine (con, CCE_NCIND, (PTR)msgEvnt, suId, spConnId,
                             (PTR) NULLP, (PTR) NULLP);
        RETVALUE(ROK);
    }
    /*FID16206.0 -*/


    /* FID 16970.0 + */
    if ( (protType == CC_INETSI) && (msgEvnt->m.dcEvnt.type == FACILITY_IND) ) 
    {
        ret = cmHashListFind (&ccSuInstTbl, (U8 *)&suConnId, (U16)sizeof(UConnId),
                              (U16)0, (PTR *) &con);

        if (ret != ROK) 
        {    
            ret = cmHashListFind(&ccSuOgInstTbl, (U8 *)&suConnId,
                                 (U16)sizeof(UConnId), (U16)0, (PTR *) &con); 
        }    

        if (ret != ROK) 
        {    
            CCLOGERR("****** Alert : context not found : su:0x%lx, sp:0x%lx ***\n",
                         suConnId, spConnId);
            RETVALUE(RFAILED);
        }    

        if (con->ccbsIsdnCallInfo == NULL)
        {    
            CCDP(AIN_DBG_LEVEL_2, "ccbsIsdnCallInfo is NULL.\n");
            RETVALUE(RFAILED);
        }    

        con->direction = ccGetDirection(con, spConnId, suConnId, pCb);
        CCDP(AIN_DBG_LEVEL_2, "%s. receive FACILITY IND. enter CC FSM.\n", __FUNCTION__);

        con->icEvntType = msgEvnt->m.dcEvnt.type;
        ccCallStateMachine (con, CCE_NCIND, (PTR)msgEvnt, suId, spConnId,
                             (PTR) NULLP, (PTR) NULLP);
        RETVALUE(ROK);
    }
    /*FID 16970.0 R8068+R8074*/
	else if( (CC_GET_PROTOCOL(protType) == CC_SIP) && 
			 (msgEvnt->m.ncEvnt.type == SUBSCRIBE_IND) ) 
    {
        if (msgEvnt->m.ncEvnt.serviceId.pres == NOTPRSNT)
        {
            CCDP(AIN_DBG_LEVEL_2, "%s. initial SUBSCRIBE message: su:0x%lx, sp:0x%lx, ccbsVirualCall:%ld.\n", 
                     __FUNCTION__, suConnId, spConnId, ccCp.sts.totalCCBSVirtual);
            goto EXIT_LABEL;
        }
        else
        {
            CCDP(AIN_DBG_LEVEL_2, "%s, refresh SUBSCRIBE message: su:0x%lx, sp:0x%lx. \n",
                     __FUNCTION__, suConnId, spConnId); 

            if ( (msgEvnt->m.ncEvnt.serviceId.pres == PRSNT_NODEF) &&
                 (msgEvnt->m.ncEvnt.serviceId.val == srvc_CCBSIW) )
            {
                 
                ret = cmHashListFind (&ccSuInstTbl, (U8 *)&suConnId, (U16)sizeof(UConnId),
                              (U16)0, (PTR *) &con);

                if (ret != ROK) 
                {    
                    ret = cmHashListFind(&ccSuOgInstTbl, (U8 *)&suConnId,
                                 (U16)sizeof(UConnId), (U16)0, (PTR *) &con); 
                }

                if (ret != ROK)
                {
                    /* add spConnId hash list search */
                    ret = cmHashListFind (&pCb->spInstTbl, (U8 *)&spConnId,
                       (U16)sizeof(UConnId), (U16)0, (PTR *) &con);
                }

                if (ret != ROK)
                {
                    CCDP(AIN_DBG_LEVEL_0, "%s: refresh call doesn't have con.\n", __FUNCTION__);
                    RETVALUE(RFAILED);
                }

				if (CC_GET_PROTOCOL(con->ogProtType) == CC_IN || 
                    CC_GET_PROTOCOL(con->ogProtType) == CC_BICC) 
		        {
                    if (con->ccbsIsdnCallInfo == NULL)
                    {
                        CCDP(AIN_DBG_LEVEL_0, "%s: ccbs isdn call doesn't have ccbsIsdnCallInfo\n", __FUNCTION__);        
                        RETVALUE(RFAILED);
                    }

                    CCDP(AIN_DBG_LEVEL_2, "%s, refresh SUBSCRIBE message for 16970: su:0x%lx, sp:0x%lx. \n",
                        __FUNCTION__, suConnId, spConnId);

                    if (suConnId == 0)
                        CCDP(AIN_DBG_LEVEL_2, "%s: refresh sub before initial sub return result arrive: %ld\n",
                                  __FUNCTION__, suConnId);
    
                    con->icEvntType = msgEvnt->m.ncEvnt.type;
                    con->direction = ccGetDirection(con, spConnId, suConnId, pCb);
                    ccCallStateMachine (con, CCE_NCIND, (PTR)msgEvnt, suId, spConnId,
                                (PTR) NULLP, (PTR) NULLP);
                    RETVALUE(ROK);
			    }
                else
                {
                    CCDP(AIN_DBG_LEVEL_2, "%s: RBWF refresh subscribe. send to FIC.\n", __FUNCTION__);
                    goto EXIT_LABEL;
                }
            }
            else
            {
				CCDP(AIN_DBG_LEVEL_2, "%s CCBS TCAP refressh SUBSCRIBE, : su:0x%lx, sp:0x%lx. \n",
						__FUNCTION__, suConnId, spConnId);
                goto EXIT_LABEL;
            }
        }
    }
	else if( (CC_GET_PROTOCOL(protType) == CC_SIP) && 
			 (msgEvnt->m.ncEvnt.type == NOTIFY_IND) &&
			 (msgEvnt->m.ncEvnt.serviceId.pres == PRSNT_NODEF) &&
			 (msgEvnt->m.ncEvnt.serviceId.val == srvc_CCBSIW) ) 
    {
        ret = cmHashListFind (&ccSuInstTbl, (U8 *)&suConnId, (U16)sizeof(UConnId),
                              (U16)0, (PTR *) &con);

        if (ret != ROK) 
        {
            ret = cmHashListFind(&ccSuOgInstTbl, (U8 *)&suConnId,
                                 (U16)sizeof(UConnId), (U16)0, (PTR *) &con); 
        }    

        if (ret != ROK) 
        {    
			CCDP(AIN_DBG_LEVEL_2, "%s, context not found, don't return: su:0x%lx, sp:0x%lx. \n",
					__FUNCTION__, suConnId, spConnId);
            goto EXIT_LABEL;
        }    

		if ( (con != NULLP) && 
		     (CC_GET_PROTOCOL(con->icProtType) == CC_IN) )
		{
            if (con->ccbsIsdnCallInfo == NULL)
            {
                CCDP(AIN_DBG_LEVEL_2, "%s: ccbsIsdnCallInfo is NULL."
                     "maybe TUFS's app conid overlap with con's. sent to FIC.\n",
                     __FUNCTION__);
                goto EXIT_LABEL;
            }
            con->icEvntType = msgEvnt->m.ncEvnt.type;
            con->direction = ccGetDirection(con, spConnId, suConnId, pCb);
            ccCallStateMachine (con, CCE_NCIND, (PTR)msgEvnt, suId, spConnId,
                             (PTR) NULLP, (PTR) NULLP);
            RETVALUE(ROK);
		}
    }
    /* FID 16970.0 - */

EXIT_LABEL:
/* + FID15083.0 */
    if ((msgEvnt->m.ncEvnt.type == IRI_REPORT))
    {
       ccGenerateIriReportX2SIG(&(msgEvnt->m.ncEvnt.u.iriReport));
    }
/* - FID15083.0 */
    else 
    {
       cmMemset((U8 *)&svcHeader, 0, sizeof(SvcHeader_t));
       svcHeader.suTransId = suConnId;
       svcHeader.spTransId = spConnId;
       CcUiFctSrvcInd(&(msgEvnt->m.ncEvnt), &svcHeader);
    }

    RETVALUE(ROK);
}

/* 
*       
*      Fun:   CcLiCctMsgCfm
*       
*      Desc:  process an incoming Message Confirm from PSIF layer.
*
*      Ret:   ROK     - successful, 
*       
*      Notes: None.
*
*      File:  cc_bdy1.c
*
*/
PUBLIC S16 CcLiCctMsgCfm
(
Pst       *pst,               /* post                                         */
SuId      suId,               /* service user identier                        */
UConnId   suConnId,           /* service user instance identifier             */
UConnId   spConnId,           /* service provider instance identifier         */
ProtType  protType,           /* incoming protocol type                       */
CcMsgEvnt *msgEvnt            /* connection release event structure           */
)
{
    CcPsSap        *pCb        = NULLP;
    SvcHeader_t     svcHeader;

    S16             ret = ROK;
    CcConCb        *con = NULLP;
    CCDP(AIN_DBG_LEVEL_1, "%s:\n", __FUNCTION__);

    if (zcCb.protState != ACTIVE)
    {
        CCLOGERR("Ignoring for standby:%s: suConnId:%ld, spConnId:%ld,"
                 "protType:%d\n",__FUNCTION__, suConnId, spConnId, protType);
        RETVALUE(RFAILED);
    }

#if 1 /* TELICA - Bugzilla-5520- Khan/Yakin - 08/01/2001 - code trace */
    TELICACODETRC(LOGDEBUG, ccCodeTrcLvl, ccInit.lmPst, ("CcLiCctMsgCfm::"
                  "spConnId=%lx, protType=%d", spConnId, protType));
#endif /* Bugzilla-5520 */

    if ((ccInit.cfgDone != TRUE) || (suId >= (SuId) ccCp.genCfg.maxPsSAP))
    {
        CCLOGERROR(ERRCLS_INT_PAR, ECC246, (ErrVal) suId,
                   "CcLiCctMsgCfm() Failed, invalid suId");
        RETVALUE(RFAILED);
    }

    if ((ccPsSapLstPtr == NULLP) || ((pCb = *(ccPsSapLstPtr + suId)) == NULLP))
    {
        CCLOGERROR(ERRCLS_INT_PAR, ECC247, (ErrVal) suId,
                   "CcLiCctMsgCfm() Failed, invalid suId");

#if 1 /* TELICA - Bugzilla-5520- Khan/Yakin - 08/01/2001 - code trace */
        TELICACODETRC(LOGERROR, ccCodeTrcLvl, ccInit.lmPst, ("CcLiCctMsgCfm::"
                      "Invalid suId, spConnId=%lx, suId=%d", spConnId, suId));
#endif /* Bugzilla-5520 */

        ccGenAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT,
                   LCM_CAUSE_INV_SAP, (U8*) &suId);
        RETVALUE(RFAILED);
    }

    if (pCb->state != CC_BND)
    {
        CCLOGERROR(ERRCLS_INT_PAR, ECC248, (ErrVal) suId,
                   "CcLiCctMsgCfm() Failed, SAP not bound");

#if 1 /* TELICA - Bugzilla-5520- Khan/Yakin - 08/01/2001 - code trace */
        TELICACODETRC(LOGERROR, ccCodeTrcLvl, ccInit.lmPst, ("CcLiCctMsgCfm::"
                      "spConnId=%lx, SAP not bound, pcbState=%d",
                      spConnId, pCb->state));
#endif /* Bugzilla-5520 */

        ccGenAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_INV_STATE,
                   LCM_CAUSE_INV_SAP, (U8*) &suId);
        RETVALUE(RFAILED);
    }
    /* FID16206.0 +*/
    if((CC_GET_PROTOCOL(protType) == CC_SIP) && (msgEvnt->m.ncEvnt.type == MESSAGE_CFM))
    {
        ret = cmHashListFind (&ccSuInstTbl, (U8 *)&suConnId, (U16)sizeof(UConnId),
                                 (U16)0, (PTR *) &con);

        if (ret != ROK)
            ret = cmHashListFind(&ccSuOgInstTbl, (U8 *)&suConnId,
                                    (U16)sizeof(UConnId), (U16)0, (PTR *) &con);

        if (ret != ROK)
        {
            CCLOGERR("****** Alert : context not found : su:0x%lx, sp:0x%lx ***\n",
                         suConnId, spConnId);
            DP("Error\n");
            RETVALUE(RFAILED);
        }
        if (con->dummyCallInfo == NULL || con->dummyCallInfo->dummyCallType == NONDUMMYCALL)
        {
            CCDP(AIN_DBG_LEVEL_2, "This is not dummy call\n");
            RETVALUE(RFAILED);
        }
        con->direction = ccGetDirection(con, spConnId, suConnId, pCb);
        con->icEvntType = msgEvnt->m.ncEvnt.type;
        ccCallStateMachine (con,CCE_NCCFM, (PTR)msgEvnt, suId, spConnId,
                       (PTR) NULLP, (PTR) NULLP);
        RETVALUE(ROK);
    }
    /* FID16206.0 -*/

    /* FID 16970.0 + */ 
    if (((CC_GET_PROTOCOL(protType) == CC_SIP) && 
		(msgEvnt->m.ncEvnt.serviceId.pres == PRSNT_NODEF) &&
		(msgEvnt->m.ncEvnt.serviceId.val == srvc_CCBSIW) &&
	    ((msgEvnt->m.ncEvnt.type == NOTIFY_CFM) ||
        (msgEvnt->m.ncEvnt.type == SUBSCRIBE_CFM))) ||
        ((protType == CC_INETSI) && (msgEvnt->m.dcEvnt.type == FACILITY_CFM)) ||
        ((msgEvnt->m.biccMsgEvnt.callIndpMsgType.eh.pres)&&
        (msgEvnt->m.biccMsgEvnt.callIndpMsgType.callIndpMsgType.pres) && 
        (msgEvnt->m.biccMsgEvnt.callIndpMsgType.callIndpMsgType.val == FACILITY_CFM)))
    {    
        if (suConnId != 0)
        {
            ret = cmHashListFind (&ccSuInstTbl, (U8 *)&suConnId, (U16)sizeof(UConnId),
                              (U16)0, (PTR *) &con);

            if (ret != ROK) 
            {    
                ret = cmHashListFind(&ccSuOgInstTbl, (U8 *)&suConnId,
                                 (U16)sizeof(UConnId), (U16)0, (PTR *) &con); 
            }    

            if (ret != ROK) 
            {    
				CCDP(AIN_DBG_LEVEL_2, "context not found: su:0x%lx, sp:0x%lx ***\n",
						 suConnId, spConnId);

                if (msgEvnt->m.ncEvnt.type == NOTIFY_CFM)
                {
                    CCDP(AIN_DBG_LEVEL_2, "%s: NOTIFY_CFM suppose have con. return fail.\n", __FUNCTION__);
                    RETVALUE(RFAILED);
                }
                else if (msgEvnt->m.ncEvnt.type == SUBSCRIBE_CFM)
                {
                    CCDP(AIN_DBG_LEVEL_2, "%s: SUBSCRIBE_CFM may have no con, send to FIC.\n", __FUNCTION__);
                    goto EXIT_LABEL;
                }
            }    
        }
        else
        {
            CCDP(AIN_DBG_LEVEL_0, "%s: suConnId is 0.\n", __FUNCTION__);
            RETVALUE(RFAILED);
        }

        if (con->ccbsIsdnCallInfo == NULL)
        {    
            CCDP(AIN_DBG_LEVEL_2, "%s: ccbsIsdnCallInfo is NULL."
                 "maybe TUFS's app conid overlap with con's. sent to FIC.\n",
                 __FUNCTION__);
            goto EXIT_LABEL; 
        }    

        con->direction = ccGetDirection(con, spConnId, suConnId, pCb);

        if(CC_GET_PROTOCOL(protType) == CC_SIP)
        {
		    if (msgEvnt->m.ncEvnt.type == NOTIFY_CFM) 
            {
				if ((con != NULL) && 
			   	    ((CC_GET_PROTOCOL(con->ogProtType) == CC_IN) || (CC_GET_PROTOCOL(con->ogProtType) == CC_BICC)))
			    {
			   	    CCDP(AIN_DBG_LEVEL_2, "16970 NOTIFY CFM.\n");
			    }
			    else
			    {
			   	    CCDP(AIN_DBG_LEVEL_2, "None 16970 NOTIFY CFM.\n");
			        goto EXIT_LABEL;
			    }
            }
            else if (msgEvnt->m.ncEvnt.type == SUBSCRIBE_CFM)
            {
			    if ((con != NULL) && 
				    (CC_GET_PROTOCOL(con->icProtType) == CC_IN))
			    {
			    	CCDP(AIN_DBG_LEVEL_2, "16970 SUBSCRIBE CFM.\n");
			    }
			    else
			    {
                    CCDP(AIN_DBG_LEVEL_2, "None 16970 SUBSCRIBE CFM.\n");
                    goto EXIT_LABEL;
                }
            }

            con->icEvntType = msgEvnt->m.ncEvnt.type;
            if ((con->direction == OUTTOINC) && (con->ogSpConnId == 0) && (msgEvnt->m.ncEvnt.type == SUBSCRIBE_CFM))
            {
               CCDP(AIN_DBG_LEVEL_1, "%s: update spConnId for SIP\n", __FUNCTION__);
               con->ogSpConnId = spConnId;
            }
        }
        else
        {
            if(CC_EGRESS_INTER_CCS_CALL(con))
            {
                con->icEvntType = FACILITY_CFM;
            }
            else
            {
                con->icEvntType = msgEvnt->m.dcEvnt.type;
            }
            if ((con->direction == OUTTOINC) && (con->ogSpConnId == 0))
            {
                con->ogSpConnId = spConnId;
                CCDP(AIN_DBG_LEVEL_1, "%s: update spConnId %ld for ISDN\n", __FUNCTION__,con->ogSpConnId);
            }
        }

        ccCallStateMachine (con,CCE_NCCFM, (PTR)msgEvnt, suId, spConnId,
                       (PTR) NULLP, (PTR) NULLP);
        RETVALUE(ROK);
    }
    /* FID16970.0 -*/

EXIT_LABEL:
    cmMemset((U8 *)&svcHeader, 0, sizeof(SvcHeader_t));
    svcHeader.suTransId = suConnId;
    svcHeader.spTransId = spConnId;
    CcUiFctSrvcCfm(&(msgEvnt->m.ncEvnt), &svcHeader);

    RETVALUE(ROK);
}

void ccRtrvGetsRelatedSts(CcMngmt *sts, void *cb, int elmnt)
{
   CcGetsSts *getsSts;

   CCDP(AIN_DBG_LEVEL_1, "%s\n", __FUNCTION__);
   
   getsSts = (CcGetsSts *) cb;
   if (getsSts)
   {
      sts->t.sts.s.ccGetsSts = * getsSts;
      sts->t.sts.s.ccGetsSts.totalIn  = getsSts->sipIn  + getsSts->isupIn;
      sts->t.sts.s.ccGetsSts.totalOut = getsSts->sipOut + getsSts->isupOut;
   }

   RETVOID;
}

void ccRtrvCcbsRelatedSts(CcMngmt *sts, void *cb, int elmnt)
{
   CcCcbsSts *ccbsSts;

   CCDP(AIN_DBG_LEVEL_1, "%s\n", __FUNCTION__);
   
   ccbsSts = (CcCcbsSts *) cb;
   if (ccbsSts)
   {
      sts->t.sts.s.ccCcbsSts = * ccbsSts;
      sts->t.sts.s.ccCcbsSts.praCCBSrequestRx        = ccbsSts->praCCBSrequestRx;  
      sts->t.sts.s.ccCcbsSts.praCCBSrequestTx        = ccbsSts->praCCBSrequestTx;
      sts->t.sts.s.ccCcbsSts.praCCBSreturnResultRx   = ccbsSts->praCCBSreturnResultRx;
      sts->t.sts.s.ccCcbsSts.praCCBSreturnResultTx   = ccbsSts->praCCBSreturnResultTx;
      sts->t.sts.s.ccCcbsSts.praCCBSreturnErrorRx    = ccbsSts->praCCBSreturnErrorRx;
      sts->t.sts.s.ccCcbsSts.praCCBSreturnErrorTx    = ccbsSts->praCCBSreturnErrorTx;
      sts->t.sts.s.ccCcbsSts.praCCBSremoteUserFreeRx = ccbsSts->praCCBSremoteUserFreeRx;
      sts->t.sts.s.ccCcbsSts.praCCBSremoteUserFreeTx = ccbsSts->praCCBSremoteUserFreeTx;
      sts->t.sts.s.ccCcbsSts.praCCBSsuspendRx        = ccbsSts->praCCBSsuspendRx;
      sts->t.sts.s.ccCcbsSts.praCCBSsuspendTx        = ccbsSts->praCCBSsuspendTx;
      sts->t.sts.s.ccCcbsSts.praCCBSresumeRx         = ccbsSts->praCCBSresumeRx;
      sts->t.sts.s.ccCcbsSts.praCCBSresumeTx         = ccbsSts->praCCBSresumeTx;
      sts->t.sts.s.ccCcbsSts.praCCBSavailableRx      = ccbsSts->praCCBSavailableRx; 
      sts->t.sts.s.ccCcbsSts.praCCBSavailableTx      = ccbsSts->praCCBSavailableTx;
   }

   RETVOID;
}

void ccInitGetsRelatedSts(void *cb, int elmnt)
{
   CcGetsSts *getsSts = NULLP;;

   CCDP(AIN_DBG_LEVEL_1, "%s\n", __FUNCTION__);

   getsSts = (CcGetsSts *) cb;
   if (getsSts) 
      cmMemset((U8 *)getsSts, 0, sizeof(CcGetsSts));
   
   RETVOID;
}

void ccInitCcbsRelatedSts(void *cb, int elmnt)
{
   CcCcbsSts *ccbsSts = NULLP;;

   CCDP(AIN_DBG_LEVEL_1, "%s\n", __FUNCTION__);

   ccbsSts = (CcCcbsSts *) cb;
   if (ccbsSts) 
      cmMemset((U8 *)ccbsSts, 0, sizeof(CcCcbsSts));
   
   RETVOID;
}

void ccRtrvMgRelatedSts(CcMngmt *sts, int elmnt)
{
   CcMgNameCfg_t *mgName = NULLP;
   CcMgNameCfg_t *last    = NULLP;
   CcMgSapCb     *mgSapCb = NULLP;
   S16            ret     = ROK;
   Bool           matchFound = FALSE;
   Bool           allMg      = FALSE;
   CcMgSapCb     *prev    = NULLP;
   CcMgSapCb     *next    = NULLP;
   
   CCDP(AIN_DBG_LEVEL_1, "%s--%s \n", __FUNCTION__, sts->t.sts.s.ccGenMgSts.mgName.val);
   
   /* Initialize the counter here */
   sts->t.sts.s.ccGenMgSts.sip2SipPermMg = 0;
   sts->t.sts.s.ccGenMgSts.sip2SipTransMg =  0;
   
   if(strncmp(sts->t.sts.s.ccGenMgSts.mgName.val, "COM", 3) ==0)
   {
     /* this is for COM -all MGs */
     CCDP(AIN_DBG_LEVEL_1, "The mgName is COM!\n");
     allMg = TRUE;
   }
   else
   {
     /* Need to find out the corresponding ccMgSapCb */
     if(sts->t.sts.s.ccGenMgSts.mgName.pres != PRSNT_NODEF)
     {
       CCDP(AIN_DBG_LEVEL_1, "Specify MG name not exists from LM!\n");
       RETVOID;
     }
     /* Need to find out the corresponding ccMgSapCb */
     while((ret=cmHashListGetNext (&ccMgNameTbl, (PTR)last, (PTR *)&mgName))==ROK)
     {
       /*bug90428: find the exact MG name*/     
       if (strlen(mgName->mgName) == sts->t.sts.s.ccGenMgSts.mgName.len)   
       {
          if (strncmp(mgName->mgName, sts->t.sts.s.ccGenMgSts.mgName.val,sts->t.sts.s.ccGenMgSts.mgName.len) == 0)  
             matchFound = TRUE;  
       }
       
       if(matchFound)
       {
         CCDP(AIN_DBG_LEVEL_1, "Specified MG found:%s!\n", sts->t.sts.s.ccGenMgSts.mgName.val);
         
         ret = cmHashListFind(&ccMgSapTbl, (U8 *)&mgName->spId, (U16)sizeof(SpId), (U16)0, (PTR *)&mgSapCb);

         if((ret != ROK) || (mgSapCb == NULLP))
         {
           CCLOGERR("ERROR, did not find MG SAP entry for spId = %d\n",mgName->spId);
           RETVOID;
         }
         break;
       }
       last = mgName;
       continue;
     }
   }
   
   switch(elmnt)
   {
      case STMGSAP:
         if(allMg)
         {
           CCDP(AIN_DBG_LEVEL_1, "Send all the global MG params back to LM\n");
           
           while((ret=cmHashListGetNext (&ccMgSapTbl, (PTR)prev, (PTR *)&next))==ROK)
           {
             if(next != NULLP)
             {
               CCDP(AIN_DBG_LEVEL_1, "PermMg:%ld,TransMg:%ld\n", next->sip2SipPermMg, next->sip2SipTransMg);
               sts->t.sts.s.ccGenMgSts.sip2SipPermMg  += next->sip2SipPermMg;
               sts->t.sts.s.ccGenMgSts.sip2SipTransMg += next->sip2SipTransMg;
              CCDP(AIN_DBG_LEVEL_1, "PermMg:%ld,TransMg:%ld\n", 
                   sts->t.sts.s.ccGenMgSts.sip2SipPermMg, sts->t.sts.s.ccGenMgSts.sip2SipTransMg);
               prev = next;
               continue;
             }
             break;
           }
           CCDP(AIN_DBG_LEVEL_1, "total num:sip2SipPermMg:%ld, sip2SipTransMg:%ld\n", 
                sts->t.sts.s.ccGenMgSts.sip2SipPermMg, sts->t.sts.s.ccGenMgSts.sip2SipTransMg);
         }
         else
         { 
           if(mgSapCb == NULLP)
           {
             CCDP(AIN_DBG_LEVEL_1, "Specify mgName:%s's mgSapCb not found!\n", sts->t.sts.s.ccGenMgSts.mgName.val);
             break;
           }
           sts->t.sts.s.ccGenMgSts.sip2SipPermMg = mgSapCb->sip2SipPermMg;
           sts->t.sts.s.ccGenMgSts.sip2SipTransMg = mgSapCb->sip2SipTransMg;
           CCDP(AIN_DBG_LEVEL_1, "ccRtrvSts - sip2SipPermMg:%ld, sip2SipTransMg;%ld\n", 
             mgSapCb->sip2SipPermMg, mgSapCb->sip2SipTransMg);
         }
         break;
      default:
         CCDP(AIN_DBG_LEVEL_1, "elmnt%d not supported!\n", elmnt);
         break;
   }
   RETVOID;
}

void ccInitMgRelatedSts(CcMngmt *sts, int elmnt)
{
   CcMgNameCfg_t *last       = NULLP;
   CcMgNameCfg_t *mgName     = NULLP;
   Bool           matchFound = FALSE;
   Bool           allMg      = FALSE;
   CcMgSapCb     *mgSapCb    = NULLP;
   S16            ret        = RFAILED;
   CcMgSapCb     *prev       = NULLP;
   CcMgSapCb     *next       = NULLP;
   
   CCDP(AIN_DBG_LEVEL_1, "%s--%s\n", __FUNCTION__, sts->t.sts.s.ccGenMgSts.mgName.val);

   if(strncmp(sts->t.sts.s.ccGenMgSts.mgName.val, "COM", 3) ==0)
   {
     /* this is for COM -all MGs */
     CCDP(AIN_DBG_LEVEL_1, "The mgName is COM!\n");
     allMg = TRUE;
   }
   else
   {
     if(sts->t.sts.s.ccGenMgSts.mgName.pres != PRSNT_NODEF)
     {
       CCDP(AIN_DBG_LEVEL_1, "Specify MG name not exists from LM!\n");
       RETVOID;
     }
     CCDP(AIN_DBG_LEVEL_1, "Specify mgName:%s\n", sts->t.sts.s.ccGenMgSts.mgName.val);
     /* Need to find out the corresponding ccMgSapCb */
     while((ret=cmHashListGetNext (&ccMgNameTbl, (PTR)last, (PTR *)&mgName))==ROK)
     {
       /*bug90428: find the exact MG name*/     
       if (strlen(mgName->mgName) == sts->t.sts.s.ccGenMgSts.mgName.len)   
       {
          if (strncmp(mgName->mgName, sts->t.sts.s.ccGenMgSts.mgName.val,sts->t.sts.s.ccGenMgSts.mgName.len) == 0)  
             matchFound = TRUE;  
       }
       
       if(matchFound)
       {
        CCDP(AIN_DBG_LEVEL_1, "Specified MG found:%s!\n", sts->t.sts.s.ccGenMgSts.mgName.val);
         
         ret = cmHashListFind(&ccMgSapTbl, (U8 *)&mgName->spId, (U16)sizeof(SpId), (U16)0, (PTR *)&mgSapCb);

         if((ret != ROK) || (mgSapCb == NULLP))
         {
           CCLOGERR("ERROR, did not find MG SAP entry for spId = %d\n",mgName->spId);
           RETVOID;
         }
         break;
       }
       last = mgName;
       continue;
     }
   }
   
   switch(elmnt)
   {
      case STMGSAP:
        {
          if(allMg)
          {
            while((ret=cmHashListGetNext (&ccMgSapTbl, (PTR)prev, (PTR *)&next))==ROK)
            {
              if(next != NULLP)
              {
                next->sip2SipPermMg = 0;
                next->sip2SipTransMg = 0;
                prev = next;
                continue;
              }
              break;
            }
            
          }
          else
          {
            if(matchFound)
            {
              CCDP(AIN_DBG_LEVEL_1, "Reset the counter for 0x%lx\n", (U32)mgSapCb);
              mgSapCb->sip2SipPermMg = 0;
              mgSapCb->sip2SipTransMg = 0;
            }
            else
            {
              CCDP(AIN_DBG_LEVEL_1, "Specified mg name %s mgsapCb not found\n", sts->t.sts.s.ccGenMgSts.mgName.val);
            }
          }
        }
         break;

      default:
         CCDP(AIN_DBG_LEVEL_1, "element:%d not supported!\n", elmnt);
         break;
   }
   
   RETVOID;
}
/* END: NON-CALL RELATED API's */
/* AAG */

/* 
*       
*      Fun:   CcLiCctIndiagMsgInd
*       
*      Desc:  
*
*      Ret:   ROK     - successful, 
*       
*      Notes: None.
*
*      File:  cc_bdy1.c
*
*/
PUBLIC S16 CcLiCctIndiagMsgInd 
(
Pst *pst, 
SuId suId, 
UConnId suConnId, 
UConnId spConnId, 
ProtType protType, 
CcSvcEvnt *ccSvcEvnt
)
{

   CcPsSap *pCb = NULLP;
   CcConCb *con = NULLP;
   S16 ret = RFAILED;
   U8  direction = DIR_UNKNOWN;

   TRC3(CcLiCctIndiagMsgInd)
       
   if ((ccInit.cfgDone != TRUE) || (suId >= (SuId) ccCp.genCfg.maxPsSAP))
   {
      DP("CcLiCctIndiagMsgInd() Failed, invalid suId=%d or cfg NotDone\n", suId);
      
      ccFreeSipSipTMBufInTheEvnt(__FILE__, __LINE__, CCE_SVC_IND, 
                                 0, (PTR)ccSvcEvnt, protType);
      RETVALUE(RFAILED);
   }

   if ((ccPsSapLstPtr == NULLP) || ((pCb = *(ccPsSapLstPtr + suId)) == NULLP))
   {
     DP("CcLiCctIndiagMsgInd() Failed, invalid suId=%d\n", suId);

      ccGenAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT,
                 LCM_CAUSE_INV_SAP, (U8*) &suId);
      ccFreeSipSipTMBufInTheEvnt(__FILE__, __LINE__, CCE_SVC_IND, 0, 
                                 (PTR)ccSvcEvnt, protType);
      RETVALUE(RFAILED);
   }

   if (pCb->state != CC_BND)
   {
      DP("CcLiCctSvcInd() Failed, SAP not bound\n");
      ccGenAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_INV_STATE,
                 LCM_CAUSE_INV_SAP, (U8*) &suId);
      ccFreeSipSipTMBufInTheEvnt(__FILE__, __LINE__, CCE_SVC_IND, 0, 
                                (PTR)ccSvcEvnt, protType);
      RETVALUE(RFAILED);
   }

   /* Find connection */
   ret = cmHashListFind (&ccSuInstTbl, (U8 *)&suConnId, (U16)sizeof(UConnId),
                         (U16)0, (PTR *) &con);
   if (ret != ROK) 
      ret = cmHashListFind(&ccSuOgInstTbl, (U8 *)&suConnId,
                        (U16)sizeof(UConnId), (U16)0, (PTR *) &con);

   if (ret != ROK)
      ret = cmHashListFind (&pCb->spInstTbl, (U8 *)&spConnId, 
                            (U16)sizeof(UConnId), (U16)0, (PTR *) &con);

   if (ret != ROK)
   {
     DP("****** Alert : context not found : su:0x%lx, sp:0x%lx ***\n",
              suConnId, spConnId);
     ccFreeSipSipTMBufInTheEvnt(__FILE__, __LINE__, CCE_SVC_IND, 0, 
         (PTR)ccSvcEvnt, protType);
     RETVALUE(RFAILED);
   }
   
   direction = ccGetDirection(con, spConnId, suConnId, pCb);
   
   if ((direction == OUTTOINC) && (con->ogSpConnId == 0))
      con->ogSpConnId = spConnId;
   else if((direction == INCTOOUT) && (con->icSpConnId == 0))
      con->icSpConnId = spConnId;
   
   if (con->state == CCS_ANSWERED)
   {
     if (((CC_GET_PROTOCOL(con->icProtType) == CC_SIP)||
          (CC_GET_PROTOCOL(con->icProtType) == CC_SIPT))&&
         ((CC_GET_PROTOCOL(con->ogProtType) == CC_SIP)||
          (CC_GET_PROTOCOL(con->ogProtType) == CC_SIPT)))
     {
       if (direction == INCTOOUT)
       {
         if ((con->ogIntfcCb)&&
             (con->ogIntfcCb->ccTgCb)&&
             ((CC_GET_PROTOCOL(con->ogProtType) == CC_SIP)?
               ((con->ogIntfcCb->ccTgCb->tgAtt.sipPrflPtr)&&
                (con->ogIntfcCb->ccTgCb->tgAtt.sipPrflPtr->transitInDialogueRefer)):
               ((con->ogIntfcCb->ccTgCb->tgAtt.sipTPrflPtr)&&
                (con->ogIntfcCb->ccTgCb->tgAtt.sipTPrflPtr->transitInDialogueRefer)))) 
         {
           CcLiCctIndiagMsgReq(&con->ogSapCb->pst, con->ogSapCb->spId, con->ogSpConnId,
                               con->ogSuConnId, con->ogProtType, ccSvcEvnt);
         }
         else
         {
           /* + BUG 84954- responding back with same msg and response code  */
           /* Build response code in In Dialog message and respond back to same side */
           ccSvcEvnt->m.sipSvcEvnt.m.msgTrans.responseCode.pres = PRSNT_NODEF;
           ccSvcEvnt->m.sipSvcEvnt.m.msgTrans.responseCode.val = SIP_405_METHOD_NOT_ALLOWED;
           ccSvcEvnt->m.sipSvcEvnt.m.msgTrans.hdrPass.pres = NOTPRSNT;
           ccFreeSipSipTMBufInTheEvnt(__FILE__, __LINE__, CCE_SVC_IND, 0, 
                              (PTR)ccSvcEvnt, protType);
           CcLiCctIndiagMsgReq(&con->icSapCb->pst, con->icSapCb->spId, con->icSpConnId,
                               con->icSuConnId, con->icProtType, ccSvcEvnt);
         }
         RETVALUE(ROK);
       }
       else if (direction == OUTTOINC)
       {
         if ((con->icIntfcCb)&&
             (con->icIntfcCb->ccTgCb)&&
             ((CC_GET_PROTOCOL(con->icProtType) == CC_SIP)?
               ((con->icIntfcCb->ccTgCb->tgAtt.sipPrflPtr)&&
                (con->icIntfcCb->ccTgCb->tgAtt.sipPrflPtr->transitInDialogueRefer)):
               ((con->icIntfcCb->ccTgCb->tgAtt.sipTPrflPtr)&&
                (con->icIntfcCb->ccTgCb->tgAtt.sipTPrflPtr->transitInDialogueRefer)))) 
         {
           CcLiCctIndiagMsgReq(&con->icSapCb->pst, con->icSapCb->spId, con->icSpConnId,
                               con->icSuConnId, con->icProtType, ccSvcEvnt);
         }
         else
         {
           /* + BUG 84954- responding back with same msg and response code  */
           /* Build response code in In Dialog message and respond back to same side */
           ccSvcEvnt->m.sipSvcEvnt.m.msgTrans.responseCode.pres = PRSNT_NODEF;
           ccSvcEvnt->m.sipSvcEvnt.m.msgTrans.responseCode.val = SIP_405_METHOD_NOT_ALLOWED;
           ccSvcEvnt->m.sipSvcEvnt.m.msgTrans.hdrPass.pres = NOTPRSNT;
           ccFreeSipSipTMBufInTheEvnt(__FILE__, __LINE__, CCE_SVC_IND, 0, 
                              (PTR)ccSvcEvnt, protType);
           CcLiCctIndiagMsgReq(&con->ogSapCb->pst, con->ogSapCb->spId, con->ogSpConnId,
                               con->ogSuConnId, con->ogProtType, ccSvcEvnt);
         }
         RETVALUE(ROK);
       }
       else
         ABORT_DEBUG;
     }
   }
      
   ccFreeSipSipTMBufInTheEvnt(__FILE__, __LINE__, CCE_SVC_IND, 0, 
                              (PTR)ccSvcEvnt, protType);
   RETVALUE(ROK);
}



/*
*
*      Fun:   ccGetXchgTypeForSipw
*
*      Desc:  Get the exchange type for compatibility
*
*      Ret:   exchange type
*
*      Notes: Always return an exchange type, typeA or typeB
*
*      File:  
*
*/

#ifdef ANSI
U8 ccGetXchgTypeForSipw
(
	SuId	  suIdt, 			  /* service user identier */
	UConnId   suConnIdt, 		  /* service user instance identifier */
	UConnId   spConnIdt 		  /* service provider instance identifier */
)
#else
U8 ccGetXchgTypeForSipw(suIdt, suConnIdt, spConnIdt)
	SuId	  suIdt; 			  /* service user identier */
	UConnId   suConnIdt; 		  /* service user instance identifier */
	UConnId   spConnIdt; 		  /* service provider instance identifier */
#endif
{
   CcPsSap *pCb = NULL;
   CcConCb *con = NULL;
   U8     xchgType = TOXA;  /* For SIP-T we default to Exchange type A */
   SuId 	  suId = suIdt; 			  /* service user identier */
   UConnId	  suConnId = suConnIdt; 		  /* service user instance identifier */
   UConnId	  spConnId = spConnIdt; 		  /* service provider instance identifier */
   S16            ret = RFAILED;

   CCDP(AIN_DBG_LEVEL_2, "%s process\n",__FUNCTION__);

   if ((ccPsSapLstPtr == NULLP) || ((pCb = *(ccPsSapLstPtr + suId)) == NULLP))
   {
      CCDP(AIN_DBG_LEVEL_2,"%s: SIP-T  xchgType = %d\n", __FUNCTION__, xchgType);
      return (U8)xchgType; /* use type=A here */
   }

   if (pCb->state != CC_BND)
   {
      CCDP(AIN_DBG_LEVEL_2,"%s: SIP-T  xchgType = %d\n", __FUNCTION__, xchgType);
	  return (U8)xchgType; /* use type=A here */
   }

   CCDP(AIN_DBG_LEVEL_2,"%s: SIP-T	suId=%d suConnId=%ld, sipInstId=0x%08lx\n",
					  __FUNCTION__, suId, suConnId, spConnId);    

   ret = cmHashListFind (&ccSuInstTbl, (U8 *)&suConnId, (U16)sizeof(UConnId), (U16)0, (PTR *) &con);
   CCDP(AIN_DBG_LEVEL_2,"%s: SIP-T  ret = %d\n", __FUNCTION__, ret);

   if (ret != ROK) 
   {
      ret = cmHashListFind(&ccSuOgInstTbl, (U8 *)&suConnId, (U16)sizeof(UConnId), (U16)0, (PTR *) &con);
      CCDP(AIN_DBG_LEVEL_2,"%s: SIP-T  ret = %d\n", __FUNCTION__, ret); 

	   if (ret != ROK)
	   {
		   ret = cmHashListFind (&pCb->spInstTbl, (U8 *)&spConnId, (U16)sizeof(UConnId), (U16)0, (PTR *) &con);
	 CCDP(AIN_DBG_LEVEL_2,"%s: SIP-T  ret = %d\n", __FUNCTION__, ret);

		   if (ret != ROK)
		   {
            CCDP(AIN_DBG_LEVEL_2,"%s: SIP-T  xchgType = %d\n", __FUNCTION__, xchgType);
			   return (U8)xchgType;/* Use type A here */
		   }
	   }
   }

   if((con != NULL) 
   	&& ((con->typeOfXchg == TOXA) || (con->typeOfXchg == TOXB)))
   {
        CCDP(AIN_DBG_LEVEL_2,"%s: SIP-T  xchgType = %d\n", __FUNCTION__, con->typeOfXchg);
	   	return (U8)con->typeOfXchg;
   }
   else
   {
       CCDP(AIN_DBG_LEVEL_2,"%s: SIP-T  xchgType = %d\n", __FUNCTION__, xchgType);
	   return (U8)xchgType;
   }
}

/* FID 14457.2 */
PUBLIC void ccCcCongInit(void)
{
  S16 i=0;
  CcRlyCngCb_t *ccRlyCngCb = NULL;
  CcMegaCngCb_t *ccMegaCngCb = &ccCongCb.ccMegaCngCb;

  DP("%s()\n", __FUNCTION__); 
  if(SRegTmr(ccInit.ent, ccInit.inst, CC_CONG_TIMER_RES, ccPrcCcCongTmr) != ROK)
  {
    CCLOGERR("ERROR:%s: SRegTmr failed to register CC_CONG_TIMER\n", __FUNCTION__);
  }

  cmInitTimers(ccCongCb.ccCongTimer, CC_CONG_TIMER_MAX);

  ccCongCb.ccCongTqCp.tmrLen = TQNUMENT;
  ccCongCb.ccCongTqCp.nxtEnt = NULLP;

  for (i = 0; i < TQNUMENT; i++)
     ccCongCb.ccCongTqType[i].first = NULLP;

  /* Intializing Control Block for DQ, Router, GDI and Memory congestion */
  ccCongCb.ccDqExtCngCb.ccCongActnLvl = CCCONG_LEVEL_NORMAL;
  ccCongCb.ccDqExtCngCb.currCCIntCngLvl = CCCONG_LEVEL_NORMAL;

  ccCongCb.ccDqExtCngCb.currAttemptCnt = 0;
  ccCongCb.ccDqExtCngCb.currBlockCnt = 0;
  ccCongCb.ccDqExtCngCb.nrmCallsDroppedSoFar = 0;
  ccCongCb.ccDqExtCngCb.prioCallsDroppedSoFar = 0;


  /* Initializing Control Block for GCC to SIPA relay congestion */
  for (i=0; i < CCMAXRLYCHNLMON;i++)
  {
    ccRlyCngCb = &ccCongCb.ccRlyCngCb[i];
    CC_INIT_RLY_CONG_CTRL_BLOCK(ccRlyCngCb);
  }

  /* Initializing Control Block for local Megaa congestion */
  CC_INIT_MEGAA_CONG_CTRL_BLOCK(ccMegaCngCb);

  return;

}

/* FID 16259.0 */
PUBLIC void ccCcAlarmInit(void)
{
  S16 i=0;

  if(SRegTmr(ccInit.ent, ccInit.inst, CC_ALARM_TIMER_RES, ccPrcCcAlarmTmr) != ROK)
  {
    CCLOGERR("ERROR:%s: SRegTmr failed to register CC_ALARM_TIMER\n", __FUNCTION__);
  }

  cmInitTimers(ccAlarmCb.ccAlarmTimer, CC_ALARM_TIMER_MAX);

  ccAlarmCb.ccAlarmTqCp.tmrLen = TQNUMENT;
  ccAlarmCb.ccAlarmTqCp.nxtEnt = NULLP;

  for (i = 0; i < TQNUMENT; i++)
     ccAlarmCb.ccAlarmTqType[i].first = NULLP;

  return;

}



/********************************************************************30**
  
         End of file: cc_bdy1.c 1.5  -  12/21/99 07:07:13
  
*********************************************************************31*/


/********************************************************************40**
  
        Notes:
  
*********************************************************************41*/

/********************************************************************50**

*********************************************************************51*/

   
/********************************************************************60**
  
        Revision history:
  
*********************************************************************61*/

/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
1.1          ---      rs   1. initial release.
  
1.2          ---      rs   1. Changes for the ICC release 1.2
1.3          ---      rs   1. Changes for the ICC release 1.3
1.3+         cc001.13 mg   1. Corrected the generation of interfaceId key
             cc011.13 rs   4. Corrected processing of the SftRelCfm primitive  

1.4          ---      mg   1. Changes for ICC 1.4 release. 

1.5          ---      zy   1. Changes for ICC 1.5 release. 
             cc001.15 zy   1. Add process for RT Auditing.
             cc017.15 zy   1. Delete generate a SLogError, when cmHashListFind
                              fails to find connection based on suInstId, or
                              spInstId.
             cc020.15 tej  1. Changes for SFT Version 3
                           2. Fixes the Audit Request processing routine
                              so that it resends the last response if this
                              is a duplicate audit request. 
             cc022.15 zy   1. changes for compilation without SFT_VER_2 flag
             cc028.15 dw   1. Changes to handle IAM after CRM in ANSI 92 case
                      zy   1. Changes for handling call release for disable SAP.
             cc031.15 dw   1. Modification in case of CRM to prevent accessing
                              uninitialised pointer
1.6         ---       ak   1. Added a CcInitPst Structure for creating a post
                              structure for sending messages to FIC.
1.7          ---      ak   1. Added fct.x in the include list.
1.8         ---       ak   1. Added cmHashListFind for ccSuOgInstTbl
                              Relation between IC and OG ConnId is 
                              broken down. Both the ConnId's are
                              independent.
*********************************************************************91*/

