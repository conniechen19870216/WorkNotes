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

     Name:     Q.93x - body 1

     Type:     C source file

     Desc:     C source code for Q.93x 

     File:     qn_bdy11.c

     Sid:      qn_bdy11.c 1.1  -  09/29/99 18:02:10

     Prg:      pk

*********************************************************************21*/


/************************************************************************

     Note: 

     This file has been extracted to support the following options:

     Option             Description
     ------    ------------------------------
#ifdef CCITT
               CCITT
#endif
#ifdef ATT
      -11      AT&T 4ESS and 5ESS
#endif
#ifdef NT
      -12      Northern Telecom DMS-100
#endif
#ifdef ATT5E
      -19      AT&T 5ESS
#endif
#ifdef ATT4E
      -20      AT&T 4ESS
#endif
#ifdef NTDMS250
      -23      Northern Telecom DMS-250
#endif
#ifdef NI2
      -28      National ISDN 2
#endif

************************************************************************/




/* This file contains the following Congiguration Functions 

 */


/*
*     this software may be combined with the following TRILLIUM
*     software:
*
*     part no.             description
*     --------    ----------------------------------------------
*     1000002     Data Link Layer - LAPB
*     1000003     Data Link Layer - LAPD
*     1000004     Data Link Layer - Multilink
*     1000005     Data Link Layer - LAPB & LAPD
*     1000011     Multiprocessor Operating System
*     1000023     Network Layer - X.31
*
*/

/* Header include files (.h) */
#ifdef LINUX
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#endif
#include "mlregs.h"
#include "app_info.h" /* application information */
#include <sycor_ioctl.h> /* system information */
#include "telica_std.h" /* for ABORT macros and procInfo variable */
#include "telica_sysinfo_ioctl.h" /* system information */
#include "envopt.h"        /* environment options */  
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */

#include "gen.h"           /* general layer */
#include "ssi.h"           /* system services */
#include "lin.h"           /* layer management */
#include "cm5.h"           /* timers */
#include "mf.h"            /* message functions */
#include "int.h"           /* isdn network layer */
#include "dat.h"           /* data link layer */
#include "qn_db.h"         /* message functions - isdn */
#include "cm_hash.h"       /* hash-list header */
#include "in.h"            /* Q.93x */
#include "in_err.h"        /* Q.93x error */
#include "cm_pftha.h"      /* common PSF */
#include "lzq.h"           /* ISDN PSF management */
#include "zq.h"            /* ISDN PSF */
#include "zq_err.h"        /* ISDN PSF error codes */
#include "rmt.h"                /* Resource Manager Interface defines */
#include "cm_atm.h"             /* general ATM layer */
#include "cm_ss7.h"
#include "cm_cc.h"              /* Common Call Control Hash Defs */
#include "cct.h"                /* Call Control Interface Header */
#include "lqw.h"
#include "qw.h"           /* general layer */

#ifdef TST_DRIVER
#include "in_acc.h"
#endif /* TST_DRIVER */

/* Header/extern include files (.x) */

#include "gen.x"           /* general layer */
#include "ssi.x"           /* system services */
#include "lin.x"           /* layer management */
#include "cm5.x"           /* timers */
#include "mf.x"            /* message functions */
#include "int.x"           /* isdn network layer */
#include "qn_db.x"         /* message functions - isdn */
#include "dat.x"           /* data link layer */
#include "cm_hash.x"       /* hash-list header */
#include "in.x"            /* Q.93x */
#include "cm_pftha.x"      /* common PSF */
#include "lzq.x"           /* ISDN PSF management */
#include "zq.x"            /* ISDN PSF */

#include "cm_atm.x"             /* general ATM layer */
#include "cm_ss7.x"
#include "cm_cc.x"              /* Common Call Control Typedefs */
#include "cm_cas.x"
#include "aag.x"
#include "cct.x"                /* Call Control Interface */
#include "rmt.x"                /* Resource Manager Interface */
#include "lqw.x"
#include "qw.x"
#include "api_ver.h" 
#ifdef TST_DRIVER
#include "in_acc.x"
#endif /* TST_DRIVER */

#include "git_class.h"

/* bug#15960, rkhan */
#include <stdlib.h>
#include <sys/ioctl.h>

/* FID 14984.10 */
#include "cm_tree.x"
#include "trc_call.h"
#ifndef LINUX
#include <time.h>
#else
#include <sys/time.h>
#endif



/* Local typedefs */

/* Local externs */
extern TskInit inInit;               /* initialization */
extern Bool inSwtchValid [MF_MAX_SWITCH];
extern CmTreeCp inCallTraceLeft; /* FID 14984.10 trace Id left tree */
extern CmTreeCp inCallTraceRight;/* FID 14984.10 trace Id right tree */
extern U8       inNumOfActTrcRules; /* FID 14984.10 active trace number */

/* Public variable declarations */

extern InGenCfg inCp;              /* ISDN General Configuration */
extern InCb **inSapLstPtr;         /* pointer to list of SAP structures */
extern InPCB **pcbLstPtr;          /* pointer to list of Control Blocks */
extern InCtldPcb ** ctldPcbLstPtr;
InGlobalCb inGlobalCb;
#ifdef PLM_IN
#include "cm_lib.x"
#endif
/* Bug#15960, rkhan */
char *dms100_011internationalnumber;
/* bug#16247, jjohn */
char *bCapacityInfoXfr;

 
/* Bug#15520, mcao */
char *status_message_handler;

/* Monmouth Progress error */
char *monmouth_progress_handler;

/* Bug#18709, mcao */
char *monmouth_bearCap_adjust;

U16 oldSigInt;
U16 oldBkupInt;

#ifdef TELICA
int isdnPcbAllocSize = sizeof(InCtldPcb);
#include "queue.h"     /* bug 47964 - ngc */
extern QUEUE_t stsUpdIsdnIfCbQ;
#endif


#define IN_UPDSTS_WAIT 100 /* TELICA-Bug#16149, rkhan, 10 secs */
void inStopAllBchanTimers(InPCB *pcb);

void print_ctldPcb(int index, InCtldPcb *ctldPcb, int moreDebug);

/* CONFIGURATION FUNCTIONS */


/*
*
*      Fun:   inGenCfgHandler
*
*      Desc: 
*
*      Ret:   ROK   - ok
*
*      Notes: None
*
*      File:  qn_bdy11.c
*
*/

#ifdef ANSI
PUBLIC S16 inGenCfgHandler
(
 InMngmt *cfg
 )
#else
     PUBLIC S16 inGenCfgHandler(cfg)
     InMngmt *cfg;
#endif
{
   Size inSMemSz;           /* memory size */
   Size profSize;           /* size of profile table */
   S16 ret;
   S16 i;
/* bug 54753, litovchick 05/29/07 */
/* congestion handling improvements */
   QLen cpuCapacity=CM1_CPU_CAPACITY;
   QLen lvl1EnterThreshold;
   QLen lvl1ExitThreshold;
   QLen lvl2EnterThreshold;
   QLen lvl2ExitThreshold;
   S16           version;
   S16           peerVersion;

   TRC3(inGenCfgHandler);

   if (inInit.cfgDone == TRUE)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      INLOGERROR(ERRCLS_DEBUG,EIN675,(ErrVal)inInit.cfgDone,
                 "ingenCfgHandler() failed, configuration already done");
#endif
    
      RETVALUE(LCM_REASON_RECONFIG_FAIL);
   }
    
   /* bug#15960, rkhan */
   dms100_011internationalnumber = getenv("DMS100_011INTERNATIONALNUMBER");
   if (dms100_011internationalnumber == NULL)
   {
      gitDebugPrint(__FILE__, __LINE__, "RRK, dms100_011internationalnumber == NULL \n");
   }
   else
   {
      gitDebugPrint(__FILE__, __LINE__, "RRK, dms100_011internationalnumber has a value \n");
   }
   /* bug#16247, jjohn */
   bCapacityInfoXfr = getenv("BCAPACITY_INFOXFR");
   if (bCapacityInfoXfr == NULL)
   {
      gitDebugPrint(__FILE__, __LINE__, "JJ, bCapacityInfoXfr == NULL \n");
   }
   else
   {
      gitDebugPrint(__FILE__, __LINE__, "JJ, bCapacityInfoXfr has a value \n");
   }

   /* Bug 16749: Monmouth Progress error */
   monmouth_progress_handler = getenv("monmouth_progress_handler");
   if ( monmouth_progress_handler == NULL)
   {
      gitDebugPrint(__FILE__, __LINE__, "MCAO, bug 16749: monmouth_progress_handler == NULL \n");
   }
   else
   {
      gitDebugPrint(__FILE__, __LINE__, "MCAO,bug 16749: monmouth_progress_handler has a value \n");
   }


   /* bug#15520, mcao */
   status_message_handler = getenv("status_message_handler");
   if (status_message_handler == NULL)
   {
      gitDebugPrint(__FILE__, __LINE__, "MCAO, status_message_handler == NULL \n");
   }
   else
   {
      gitDebugPrint(__FILE__, __LINE__, "MCAO, status_message_handler has a value \n");
   }
   /* bug#18709, mcao */
   monmouth_bearCap_adjust = getenv("monmouth_bearCap_adjust");
   if (monmouth_bearCap_adjust == NULL)
   {
      gitDebugPrint(__FILE__, __LINE__, "MCAO, monmouth_bearCap_adjust == NULL \n");
   }
   else
   {
      gitDebugPrint(__FILE__, __LINE__, "MCAO, monmouth_bearCap_adjust has a value \n");
   }

/* bug 54753, litovchick 04/02/07 */
/* congestion handling improvements */

#ifndef ATCA
      if (IS_CM())
      {
         cpuCapacity = CM1_CPU_CAPACITY;
      }
      else if (IS_SP())
      {
         int driverFd = -1;
         telica_sysinfo_cpu_info_t tsi;

         /* Attempt to find ouf if this is an SP2 or an SP3.
          * If this fails, assume SP2 since it has the lowest capacity.
          */
         cpuCapacity = SP2_CPU_CAPACITY;

         driverFd = open(TELICA_SYSINFO_DRIVER_DEVICE_PATHNAME, O_RDWR);

         if (driverFd >= 0)
         {
            tsi.common.version = TELICA_SYSINFO_IOCTL_VERSION_1;
            tsi.common.structure_length = sizeof(tsi);
	    if (ioctl(driverFd, telica_sysinfo_ioctl_cpu_info, &tsi) >= 0)
            {
               if (tsi.cpu_type == sycor_cpu_type_sp3)
               {
                  cpuCapacity = SP3_CPU_CAPACITY;
               }
            }
	    
         }

      } /* end if (IS_SP()) */
#else
/* STEVEN - Need redefine the capacity for ATCA. 
   Since we have no performance data for now, 
   just make it same on both ACCM and ADCM */
      if(IS_SC_SLOT(procInfo.SlotNum)) /* ACCM */
      {
         cpuCapacity = CM1_CPU_CAPACITY;
      }
      else /* ADCM */
      {
         cpuCapacity = CM1_CPU_CAPACITY;
      }
#endif

#ifndef ATCA
      /* bug88508 */
      lvl1EnterThreshold = (cpuCapacity * SW_VERSION_MULTIPLIER) / 2;
      lvl1ExitThreshold = (lvl1EnterThreshold * 6) / 10;
      lvl2EnterThreshold = lvl1EnterThreshold * 1.6;
      lvl2ExitThreshold = (lvl2EnterThreshold * 8) / 10;
#else
      lvl1EnterThreshold = (cpuCapacity * SW_VERSION_MULTIPLIER) / 3;
      lvl1ExitThreshold = (lvl1EnterThreshold * 6) / 8;
      lvl2EnterThreshold = lvl1EnterThreshold * 1.25;
      lvl2ExitThreshold = (lvl2EnterThreshold * 9) / 10;
#endif



#ifdef IN_ACC /* TELICA-BUG:18735-mqin */
#if 0 
   SDmndQCongInit(&inDmndQCong, DFLT_LVL2_DQCONG_ENTER,
                  DFLT_LVL2_DQCONG_EXIT, DFLT_LVL1_DQCONG_ENTER,
                  DFLT_LVL1_DQCONG_EXIT, q931Name);
#else /* new code */
      /* bug 54753, litovchick 05/29/07 */

   SDmndQCongInit_SHORT(&inDmndQCong, q931Name, 
					   lvl1EnterThreshold, lvl1ExitThreshold,
						lvl2EnterThreshold, lvl2ExitThreshold,
						FALSE );  

#endif /* end new code */
#endif /* BUG:18735 */

   inCp.nmbSaps = cfg->t.cfg.s.inGen.nmbSaps;
   inCp.nmbLnks = cfg->t.cfg.s.inGen.nmbLnks;
   inCp.nmbSigLnks = cfg->t.cfg.s.inGen.nmbSigLnks;
   inCp.nmbCes = cfg->t.cfg.s.inGen.nmbCes;
   inCp.nmbCalRef = (cfg->t.cfg.s.inGen.nmbCalRef + inCp.nmbSaps + 
                     inCp.nmbLnks);
   inCp.nmbBearer = cfg->t.cfg.s.inGen.nmbBearer;
   inCp.nmbRouts = cfg->t.cfg.s.inGen.nmbRouts;
   inCp.nmbProfiles = cfg->t.cfg.s.inGen.nmbProfiles;
   inCp.poolTrUpper = cfg->t.cfg.s.inGen.poolTrUpper;
   inCp.timeRes = cfg->t.cfg.s.inGen.timeRes;

   profSize = (Size) inCp.nmbProfiles * (Size) (sizeof(InBearCfg)); 
  
   inSMemSz = ((Size) inCp.nmbSigLnks * (Size) (sizeof(InPCB))) +
      ((Size) (inCp.nmbLnks) * (Size) (sizeof(InCtldPcb))) +
      ((Size) (inCp.nmbLnks) * (Size) (sizeof(InPCB *))) +
      ((Size) (inCp.nmbLnks) * (Size) (sizeof(InCtldPcb *))) +
      ((Size) (inCp.nmbBearer) * (Size) (sizeof(InBearChn))) +
      ((Size) inCp.nmbSaps * (Size) (sizeof(InCb))) +
      ((Size) inCp.nmbSaps * (Size) (sizeof(InCb *))) +
      ((Size) inCp.nmbCes * (Size) (sizeof(InCb))) +
      ((Size) inCp.nmbCes * (Size) (sizeof(InCb *))) +
      ((Size) inCp.nmbRouts * (Size) 
       (sizeof(RoutEntry))) + profSize +  
      ((Size) inCp.nmbCalRef * (Size) (sizeof(CalId))) + 
      ((Size) inCp.nmbCalRef * (Size) (sizeof(InNtc))) + 
      ((Size) inCp.nmbCalRef * (Size) (sizeof(CalRefTbl))) +
      ((Size) inCp.nmbCalRef * (Size) (sizeof(CmListEnt)));


   if ((ret=SGetSMem(inInit.region, (Size)inSMemSz,&inInit.pool)) != ROK)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      INLOGERROR(ERRCLS_DEBUG, EIN676, (ErrVal) inSMemSz,
                 "InMiLinCfgReq() failed, unable to allocate memory.");
#endif
      RETVALUE(LCM_REASON_MEM_NOAVAIL);
   }


   if ((ret = SGetSBuf(inInit.region, inInit.pool, 
                       (Data **) &ctldPcbLstPtr, 
                       (Size)((inCp.nmbLnks) * 
                              (sizeof(InCtldPcb *))))) != ROK)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      INLOGERROR(ERRCLS_DEBUG, EIN677, (ErrVal) inSMemSz,
                 "InMiLinCfgReq() failed, unable to allocate memory.");
#endif
      RETVALUE(LCM_REASON_MEM_NOAVAIL);
   }

   if ((ret = SGetSBuf(inInit.region, inInit.pool, 
                       (Data **) &pcbLstPtr, 
                       (Size)((inCp.nmbLnks) * 
                              (sizeof(InPCB *))))) != ROK)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      INLOGERROR(ERRCLS_DEBUG, EIN678, (ErrVal) inSMemSz,
                 "InMiLinCfgReq() failed, unable to allocate memory.");
#endif
      RETVALUE(LCM_REASON_MEM_NOAVAIL);
   }


   if ((ret = SGetSBuf(inInit.region, inInit.pool, 
                       (Data **) &inSapLstPtr, (Size)
                       (inCp.nmbSaps * (sizeof(InCb *))))) != ROK)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      INLOGERROR(ERRCLS_DEBUG, EIN679, (ErrVal) inSMemSz,
                 "InMiLinCfgReq() failed, unable to allocate memory.");
#endif
      RETVALUE(LCM_REASON_MEM_NOAVAIL);
   }
  
   if (inCp.nmbProfiles)
      if ((ret = SGetSBuf(inInit.region, inInit.pool, 
                          (Data **) &inProfTbl, (Size) profSize)) != ROK)
      {
#if (ERRCLASS & ERRCLS_DEBUG)
         INLOGERROR(ERRCLS_DEBUG, EIN680, (ErrVal) inSMemSz,
                    "InMiLinCfgReq() failed, unable to allocate memory.");
#endif
         RETVALUE(LCM_REASON_MEM_NOAVAIL);
      }

   SRegTmr(inInit.ent, inInit.inst, inCp.timeRes, inPrcCbTq);
   SRegTmr(inInit.ent, inInit.inst, inCp.timeRes, inPrcNtcTq);
   SRegTmr(inInit.ent, inInit.inst, inCp.timeRes, inPrcBChTq);
   SRegTmr(inInit.ent, inInit.inst, inCp.timeRes, inPrcPcbTq);
   SRegTmr(inInit.ent, inInit.inst, inCp.timeRes, inPrcCtldPcbTq);

  /* TELICA-Bug#16149, rkhan */
   SRegTmr(inInit.ent, inInit.inst, IN_UPDSTS_WAIT, inUpdPeerSts);

   /* Initialize timing queues */
   ntcTqCp.tmrLen   = TQNUMENT;
   cbTqCp.tmrLen    = TQNUMENT;
   bChTqCp.tmrLen   = TQNUMENT;
   ctldPcbTqCp.tmrLen = TQNUMENT;
   pcbTqCp.tmrLen   = TQNUMENT;
   cbIntTqCp.tmrLen = TQNUMENT;

   for (i = 0; i < (S16) inCp.nmbSaps; i++)
      *(inSapLstPtr + i) = NULLP;

   for (i = 0; i < (S16) inCp.nmbLnks; i++)
      *(pcbLstPtr + i) = NULLP;

   for (i = 0; i < (S16) inCp.nmbLnks; i++)
      *(ctldPcbLstPtr + i) = NULLP;

   inFillMgmtPst(cfg);

#ifdef ATCA /* FID 14984.10 */
   (void)cmTreeInit(&inCallTraceLeft, 0, 0, TRC_CALL_MAX_LEAFS);
   (void)cmTreeInit(&inCallTraceRight, 0, 0, TRC_CALL_MAX_LEAFS);
   inNumOfActTrcRules = 0;
#endif

   /* HUG code for FID 16537.0.
    * Init hash pcb and ctldPcb hash table only in case of
    * hug to 9.0.1. 3808 is old value of ISDNIF_TAB_ID and 
    * ISDNLNK_TAB_ID. 
    */
   version =  GET_API_VER (API_PSF_Q931_VER_IDX);
   peerVersion =  GET_PEER_API_VER (API_PSF_Q931_VER_IDX);

   if((GET_MAJOR_VER(peerVersion) == API_83_MAJOR)&&
      (GET_MAJOR_VER(version)==API_91_MAJOR)&&(GET_MINOR_VER(version)==0x1))
   {
      ret = cmHashListInit(&inGlobalCb.pcbHl, 3808, \
                        0x0, FALSE,CM_HASH_KEYTYPE_DEF,\
                        inInit.region, inInit.pool);
      if (ret != ROK)
      {
         RETVALUE (LCM_REASON_HASHING_FAILED);
      }

      ret = cmHashListInit(&inGlobalCb.ctldPcbHl, 3808, \
                        0x0, FALSE,CM_HASH_KEYTYPE_DEF,\
                        inInit.region, inInit.pool);
      if (ret != ROK)
      {
         RETVALUE (LCM_REASON_HASHING_FAILED);
      }
   }
   else if((GET_MAJOR_VER(peerVersion) == API_84_MAJOR)&&(GET_MINOR_VER(peerVersion)<0xa)&&
           (GET_MAJOR_VER(version)==API_91_MAJOR)&&(GET_MINOR_VER(version)==0x1))
   {
      ret = cmHashListInit(&inGlobalCb.pcbHl, 3808, \
                        0x0, FALSE,CM_HASH_KEYTYPE_DEF,\
                        inInit.region, inInit.pool);
      if (ret != ROK)
      {
         RETVALUE (LCM_REASON_HASHING_FAILED);
      }

      ret = cmHashListInit(&inGlobalCb.ctldPcbHl, 3808, \
                        0x0, FALSE,CM_HASH_KEYTYPE_DEF,\
                        inInit.region, inInit.pool);
      if (ret != ROK)
      {
         RETVALUE (LCM_REASON_HASHING_FAILED);
      }
   }

   /* FID 16779.0 Initialize digit map hash list */
   ret = cmHashListInit(&inGlobalCb.digitMapHl, 512, \
                     0x0, FALSE, CM_HASH_KEYTYPE_STR,\
                     inInit.region, inInit.pool);
   if (ret != ROK)
   {
      RETVALUE(LCM_REASON_HASHING_FAILED);
   }

   RETVALUE(LCM_REASON_NOT_APPL);
  
} /* End : inGenCfgHandler */




/*
*
*      Fun:   inInitTCb
*
*      Desc: 
*
*      Ret:   ROK   - ok
*
*      Notes: None
*
*      File:  qn_bdy11.c
*
*/

PUBLIC S16 inInitTCb (InCb *cb, InMngmt *cfg, SpId spId)
{
   S16 i;
   U8  callId[32];

   TRC3(inInitTCb);

   cb->sapCfg.useSubAdr = cfg->t.cfg.s.inTSAP.useSubAdr;
   cb->sapCfg.keyPad = cfg->t.cfg.s.inTSAP.keyPad;
   cb->sapCfg.minAdrDig = cfg->t.cfg.s.inTSAP.minAdrDig;
  
   for (i = 0; i < ADRLEN; i++)
      cb->sapCfg.wcMask[i] = 0;
  
#ifdef ISDN_ROUTE
   if ((cb->sapCfg.wcRout = cfg->t.cfg.s.inTSAP.wcRout) != FALSE)
      inAscMaskToBcd(&cfg->t.cfg.s.inTSAP.wcMask[0], 
                     &cb->sapCfg.wcMask[0]);
   else
   {
      for (i = 0; i < ADRLEN; i++)
         cb->sapCfg.wcMask[i] = 0xff;
   }
   if ((cb->sapCfg.adrPref = cfg->t.cfg.s.inTSAP.adrPref) != FALSE)
   {
      cb->sapCfg.nmbPrefDig = cfg->t.cfg.s.inTSAP.nmbPrefDig;
      for (i = 0; i < MAXPREFDIG; i++)
         cb->sapCfg.prefix[i] = cfg->t.cfg.s.inTSAP.prefix[i];
      inAscAdrToBcd(&cfg->t.cfg.s.inTSAP.prefix[0], &cb->prefix[0]);
   }
   else
#endif /* ISDN_ROUTE*/
   {
      cb->sapCfg.nmbPrefDig = 0;
      for (i = 0; i < MAXPREFDIG; i++)
         cb->sapCfg.prefix[i] = 0;
   }
  
   sprintf(callId, "%dLSM%d", 0, spId);
   cb->sapCfg.callId.len = (U8)cmStrlen(callId);
   cmMemcpy(cb->sapCfg.callId.val, callId, cmStrlen(callId));

   for (i = 0; i < ADRLEN; i++)
      cb->sapCfg.sid.strg[i] = 0;
  
   cb->sapCfg.sidIns = cfg->t.cfg.s.inTSAP.sidIns;
   cb->sapCfg.sidTon = cfg->t.cfg.s.inTSAP.sidTon;
   cb->sapCfg.sidNPlan = cfg->t.cfg.s.inTSAP.sidNPlan;
   cb->sapCfg.sidPresInd = cfg->t.cfg.s.inTSAP.sidPresInd;
   cb->sapCfg.sid.length = cfg->t.cfg.s.inTSAP.sid.length;
   for (i = 0; i < ADRLEN; i++)
      cb->sapCfg.sid.strg[i] = cfg->t.cfg.s.inTSAP.sid.strg[i];
  
   cb->sapCfg.comptChck = cfg->t.cfg.s.inTSAP.comptChck;
   cb->sapCfg.nmbApplProf = cfg->t.cfg.s.inTSAP.nmbApplProf;
   for (i = 0; i < MAX_NMB_PROF; i++)
      cb->sapCfg.profNmb[i] = cfg->t.cfg.s.inTSAP.profNmb[i];
  
   RETVALUE(ROK);
} /* end of inInitTCb */



/*
*
*      Fun:   inTSAPCfgHandler
*
*      Desc: 
*
*      Ret:   ROK   - ok
*
*      Notes: None
*
*      File:  qn_bdy11.c
*
*/

PUBLIC S16 inTSAPCfgHandler(InMngmt *cfg)
{
   S16 ret;
   InCb *cb;
   SuId sapId;
   int i;

   TRC3(inTSAPCfgHandler);

   INDBGP(DBGMASK_MI, "\n**VAR**:-- BEGIN to set all TSAP --\n");

   for (i=0; i< PLM_MAX_ISDN_VARIANT_TYPES; i++)
   {
      /*
        INDBGP(DBGMASK_MI,"PLM_IN:cfg->t.cfg.s.inTSAP.spIdLst[i] = %d, "
        "cfg->t.cfg.s.inTSAP.swtchLst[i] = %d \n",
        cfg->t.cfg.s.inTSAP.spIdLst[i], cfg->t.cfg.s.inTSAP.swtchLst[i]);
      */
      sapId = cfg->t.cfg.s.inTSAP.spIdLst[i];

      if (sapId == PLM_SWTCH_NOT_SUPPORT)
         continue;

      if ((cb = *(inSapLstPtr + sapId)) != NULLP)
      {
#if (ERRCLASS & ERRCLS_DEBUG)
         INLOGERROR(ERRCLS_DEBUG, EIN681, (ErrVal) sapId,
                    "InMiLinCfgReq() failed, sap already configured.");
#endif
         RETVALUE(LCM_REASON_MEM_NOAVAIL);

      }
 

      if ((ret = SGetSBuf(inInit.region, inInit.pool, 
                          (Data **) &cb, (Size) sizeof(InCb))) != ROK)
      {
#if (ERRCLASS & ERRCLS_DEBUG)
         INLOGERROR(ERRCLS_DEBUG, EIN681, (ErrVal) sizeof(InCb),
                    "InMiLinCfgReq() failed, unable to allocate memory.");
#endif
         RETVALUE(LCM_REASON_MEM_NOAVAIL);
      }

      /* bug 18804: initiate memory of control block */
      cmMemset((U8 *)cb, 0, sizeof(InCb));

      *(inSapLstPtr + sapId) = cb;
    

      if ((cfg->t.cfg.s.inTSAP.swtchLst[i] >= MF_MIN_SWITCH) &&
          (cfg->t.cfg.s.inTSAP.swtchLst[i] < MF_MAX_SWITCH)  &&
          (inSwtchValid [cfg->t.cfg.s.inTSAP.swtchLst[i]] == TRUE))
      {
         cb->sapCfg.swtch = cfg->t.cfg.s.inTSAP.swtchLst[i];
      }
      else
      {
#if (ERRCLASS & ERRCLS_INT_PAR)
         INLOGERROR(ERRCLS_INT_PAR, EIN682, 
                    (ErrVal) cfg->t.cfg.s.inTSAP.swtchLst[i],
                    "InMiLinCfgReq() failed, invalid switch value.");
#endif
         RETVALUE(LCM_REASON_INVALID_PAR_VAL);
      }

      cb->spId = (SuId)sapId;


      INDBGP(DBGMASK_MI,"**VAR**:Setting  TSAP spId=%d\n", cb->spId);
      switch(cb->sapCfg.swtch)
      {
         case SW_ATT5EP: INDBGP(DBGMASK_MI,"SwitchType=SW_ATT5EP\n");
            break;
         case SW_NI2: INDBGP(DBGMASK_MI,"SwitchType=SW_NI2\n");
            break;
         case SW_NTDMS100P: INDBGP(DBGMASK_MI,"SwitchType=SW_NTDMS100P\n");
            break;
         case SW_ETSI: INDBGP(DBGMASK_MI,"SwitchType=SW_ETSI\n");
            break;
         case SW_CCITT: INDBGP(DBGMASK_MI,"SwitchType=SW_CCITT\n");
            break;
         default:
            INDBGP(DBGMASK_MI,"SwitchType=%d (NOT SUPPORTED)\n",cb->sapCfg.swtch);
      }

      cb->ces = 0;
      cb->sapType = TSAP;
      cb->state = CBST3DOWN;
      cb->flc = FALSE;
      cb->rstCnt = 0;
      cb->intType = 0;
      cb->numCalId = 0;
      cb->numCalRef = 0;
      cb->callIdTbl = NULLP;
      cb->lstRefUsd = 0;
    
      cb->pst.selector = cfg->t.cfg.s.inTSAP.selector;
      cb->pst.region = cfg->t.cfg.s.inTSAP.mem.region;
      cb->pst.pool = cfg->t.cfg.s.inTSAP.mem.pool;
      cb->pst.prior = cfg->t.cfg.s.inTSAP.prior;
      cb->pst.route = cfg->t.cfg.s.inTSAP.route;
      cb->pst.dstProcId = PROCIDNC;
      cb->pst.dstEnt    = ENTNC;
      cb->pst.dstInst   = INSTNC;
      cb->pst.srcProcId = inInit.procId;
      cb->pst.srcEnt    = inInit.ent;
      cb->pst.srcInst   = inInit.inst;
      cb->pst.event     = EVTNONE;
      cb->mBuf = NULLP;
      cb->inNtcPtr = NULLP;
    
      cb->tmrCnt = 0;
    
      /* Initialize timer structure */
      cmInitTimers(cb->timers, MAXSIMTIMER);
    
      /* Initialize call reference's hash table */
      /* the maximum hash index is 4 X 2^8 = 1024 */
      /* we sum all the hash keys bytes by bytes up to 4 bytes */
      /* and mod it with the bin number to obtain the hash index */
      if (inCp.nmbCalRef > 0x400)
         cmHashListInit(&cb->tcrTbl, 0x400, 0, (Bool)FALSE, 
                        (U16)CM_HASH_KEYTYPE_DEF, 
                        inInit.region, inInit.pool); 
      else
         cmHashListInit(&cb->tcrTbl, inCp.nmbCalRef, 0, (Bool)FALSE, 
                        (U16)CM_HASH_KEYTYPE_DEF, 
                        inInit.region, inInit.pool); 
    
      /* Initialize message control */  
      MFINITPROF(&cb->mfCfgProf, ret, NMB_ALLMSG, NMB_PRIM, allPduDefs, 
                 allSduDefs, MF_MAX_REPELMT, 
                 (MF_IGNORE | MF_ORDERED), NULLP);
      MFINITMSGCTL(&cb->mfMsgCtl, ret, &cb->mfCfgProf, 0);


      inInitTCb(cb, cfg, sapId);
   } /* for */


   RETVALUE(LCM_REASON_NOT_APPL);

} /* End : inTSAPCfgHandler */



/*
*
*      Fun:   inDLSAPCfgHandler
*
*      Desc: 
*
*      Ret:   ROK   - ok
*
*      Notes: None
*
*      File:  qn_bdy11.c
*
*/

#ifdef ANSI
PUBLIC S16 inDLSAPCfgHandler
(
 InMngmt *cfg
)
#else
PUBLIC S16 inDLSAPCfgHandler(cfg)
  InMngmt *cfg;
#endif
{
   InPCB *pcb;
   InPCB *sigPcb;
   InCtldPcb *ctldPcb;
   Bool initCfg;
   SuId sapId;
   S16 ret;
   S16 version;
   S16 peerVersion;
   InPCB *tmpPcb = NULLP;
#ifdef IN_OLD_BIND
   S16 i;
   InCb *tCb;
#endif

   TRC3(inDLSAPCfgHandler);


#ifdef IN_LMINT3
   sapId = cfg->t.cfg.s.inDLSAP.sapId;
#else
   sapId = cfg->hdr.elmId.elmntInst1;
#endif /* IN_LMINT3 */

   version =  GET_API_VER (API_PSF_Q931_VER_IDX);
   peerVersion =  GET_PEER_API_VER (API_PSF_Q931_VER_IDX);

   if ((ctldPcb = *(ctldPcbLstPtr + sapId)) != NULLP)
   {
      initCfg = FALSE;
   }
   else
   {
      if ((ret = SGetSBuf(inInit.region, inInit.pool, (Data **) &ctldPcb,
                          (Size) sizeof(InCtldPcb))) != ROK)
      {
#if (ERRCLASS & ERRCLS_DEBUG) /* addition : in014.310 */
         INLOGERROR(ERRCLS_DEBUG, EIN683, (ErrVal) sizeof(InCtldPcb),
                    "InMiLinCfgReq() failed, unable to allocate memory.");
#endif /* addition : in014.310 */
         RETVALUE(LCM_REASON_MEM_NOAVAIL);
      }

      /* bug 18804: initiate memory of control block */
      cmMemset((U8 *)ctldPcb, 0, sizeof(InCtldPcb));

      *(ctldPcbLstPtr + sapId) = ctldPcb;
      initCfg = TRUE;
   }


   if (initCfg)
   {
      if (!(cfg->t.cfg.s.inDLSAP.nfasInt) || 
          ((cfg->t.cfg.s.inDLSAP.nfasInt) &&
           ((sapId == cfg->t.cfg.s.inDLSAP.sigInt) ||
            ((cfg->t.cfg.s.inDLSAP.buIntPr) &&
             (sapId == cfg->t.cfg.s.inDLSAP.bupInt)))))
      {
         if ((ret = SGetSBuf(inInit.region, inInit.pool, (Data **) &pcb,
                             (Size) sizeof(InPCB))) != ROK)
         {
#if (ERRCLASS & ERRCLS_DEBUG) /* addition : in014.310 */
            INLOGERROR(ERRCLS_DEBUG, EIN684, (ErrVal) sizeof(InPCB),
                       "InMiLinCfgReq() failed, unable to allocate memory.");
#endif /* addition : in014.310 */
            RETVALUE(LCM_REASON_MEM_NOAVAIL);
         }
         /* bug 18804: initiate memory of control block */
         cmMemset((U8 *)pcb, 0, sizeof(InPCB));

         *(pcbLstPtr + sapId) = pcb;              
      }
      else
         pcb = NULLP;
   } /* initCfg */
   else
   {
      /* TELICA-jwh-07/25/2000: Allow reconfig of NFAS B links */
      if ((cfg->t.cfg.s.inDLSAP.nfasInt) &&
          (sapId != cfg->t.cfg.s.inDLSAP.sigInt) &&
          ((cfg->t.cfg.s.inDLSAP.buIntPr != TRUE) || (sapId != cfg->t.cfg.s.inDLSAP.bupInt)))
      {
         return(LCM_REASON_NOT_APPL);
      }

      pcb = *(pcbLstPtr + sapId);

      if (pcb == NULLP)
      {
         if (ctldPcb->cfg.nfasInt)
         {
            if (ctldPcb->suId == cfg->t.cfg.s.inDLSAP.sigInt)
            {
               pcb = *(pcbLstPtr + ctldPcb->cfg.sigInt);
            }
            else
               if ((cfg->t.cfg.s.inDLSAP.buIntPr) &&
                   (ctldPcb->suId == cfg->t.cfg.s.inDLSAP.bupInt))
               {
                  sigPcb = *(pcbLstPtr + ctldPcb->cfg.sigInt);
                  if (sigPcb)
                     pcb = *(pcbLstPtr + sigPcb->cfg.bupInt);
               }

            if (pcb == NULLP)
            {
#if (ERRCLASS & ERRCLS_INT_PAR)
               INLOGERROR(ERRCLS_INT_PAR, EIN734, (ErrVal) sapId,
                          "inDLCCfgHandler() failed, incorrect instance value.");
#endif
               RETVALUE(LCM_REASON_INVALID_SAP);
            }

            *(pcbLstPtr + ctldPcb->suId) = pcb;
        
         } /* nfas */
         else
         {
#if (ERRCLASS & ERRCLS_INT_PAR)
            INLOGERROR(ERRCLS_INT_PAR, EIN735, (ErrVal) sapId,
                       "inDLCCfgHandler() failed, incorrect instance value.");
#endif
            RETVALUE(LCM_REASON_INVALID_SAP);
         } 
      } /* Null pcb */

      /* TELICA-jwh-03/02/2001:  If this is a reconfig, call inPartialRecfgDLSAP.  */
      ret = inPartialRecfgDLSAP(pcb, ctldPcb, cfg);
      RETVALUE(ret);
   } /* else */


   if (pcb != NULLP)
   {
      if ((ret = inSigPcbCfgHandler(cfg, ctldPcb, pcb, initCfg, sapId)) 
          != LCM_REASON_NOT_APPL)
         RETVALUE(ret);
      else
      {
         /* HUG code for FID 16537.0. Insert PCB to hash list */
         if((GET_MAJOR_VER(peerVersion) == API_83_MAJOR)&&
            (GET_MAJOR_VER(version)==API_91_MAJOR)&&(GET_MINOR_VER(version)==0x1))
         {
            ret = cmHashListFind(&inGlobalCb.pcbHl, (U8 *)&pcb->rSuId, (U16)sizeof(U32),
                                   (U16)0, (PTR *)&tmpPcb);
            if(ret != ROK)
            {
               /* Insert new entry in hash list */
               if (cmHashListInsert(&inGlobalCb.pcbHl, (PTR)pcb,
                        (U8 *)&pcb->rSuId,
                        sizeof(U32)) != ROK)
              {
                 RETVALUE(ret);
              }
           }
         }
         else if((GET_MAJOR_VER(peerVersion) == API_84_MAJOR)&&(GET_MINOR_VER(peerVersion)<0xa)&&
                 (GET_MAJOR_VER(version)==API_91_MAJOR)&&(GET_MINOR_VER(version)==0x1))
         {
            ret = cmHashListFind(&inGlobalCb.pcbHl, (U8 *)&pcb->rSuId, (U16)sizeof(U32),
                                   (U16)0, (PTR *)&tmpPcb);
            if(ret != ROK)
            {
               /* Insert new entry in hash list */
               if (cmHashListInsert(&inGlobalCb.pcbHl, (PTR)pcb,
                        (U8 *)&pcb->rSuId,
                        sizeof(U32)) != ROK)
              {
                 RETVALUE(ret);
              }
           }
         }
      }
   }
   else
   {
      ctldPcb->suId = sapId;
      ret = inCtldPcbCfgHandler(cfg, ctldPcb, initCfg);
  
      RETVALUE(ret);
   }

   if (zqCb.protState == ACTIVE)
   {
#ifdef IN_OLD_BIND
#error IN_OLD_BIND
      /* Issue a Bind Request if:
         - If non NFAS and the interface has CES OR
         - If NFAS interface and the interface is the signalling
         interface OR
         - If NFAS, Backup Present and Interface is the backup.          
         - If any of upper sap using this link is configured */
  
      if (((pcb->cfg.nmbCes) && (!(pcb->cfg.nfasInt))) 
          || ((pcb->cfg.nmbCes) && (pcb->cfg.nfasInt)
              && ((pcb->suId == (S16) pcb->cfg.sigInt) 
                  || ((pcb->cfg.buIntPr) && 
                      (pcb->suId == (S16)pcb->cfg.bupInt)))))
      {
         for (i = 0; i < (S16) inCp.nmbSaps; i++)
         {
            tCb = *(inSapLstPtr + i);
            if ((tCb != NULLP) && (tCb->sapCfg.swtch == pcb->cfg.swtch) 
                && (tCb->state == CBST1XFER))
            {
#ifdef DAT2
               pcb->bndState = IN_WAIT_BNDCFM;
               inStartPcbTmr(TMR_TINT, pcb);
#endif /* DAT2 */
               InLiDatBndReq(&pcb->pst, pcb->suId, pcb->spId, Q_930);
#ifndef DAT2
               pcb->bndState = IN_BND;
               zqRunTimeUpd(ZQ_PSAP_CB, CMPFTHA_UPD_REQ, (PTR)pcb);
               zqUpdPeer();
#endif /* ifndef DAT2 */          
            }
         }
      }
#endif /*  IN_OLD_BIND */
   }
   RETVALUE(LCM_REASON_NOT_APPL);
} /* end of inDLSAPCfgHandler */







/*
 *
 *      Fun:   inCtldSapCfgHandler
 *
 *      Desc: 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None
 *
 *      File:  qn_bdy11.c
 *
*/

#ifdef ANSI
PUBLIC S16 inCtldSapCfgHandler
(
 InMngmt *cfg
 )
#else
     PUBLIC S16 inCtldSapCfgHandler(cfg)
     InMngmt *cfg;
#endif
{
   InCtldPcb *ctldPcb;
   Bool initCfg;
   SuId sapId;
   S16 ret;

   TRC3(inCtldSapCfgHandler);

#ifdef IN_LMINT3
   sapId = cfg->t.cfg.s.inDLSAP.sapId;
#else
   sapId = cfg->hdr.elmId.elmntInst1;
#endif /* IN_LMINT3 */ 

   if ((ctldPcb = *(ctldPcbLstPtr + sapId)) != NULLP)
   {
      initCfg = FALSE;
   }
   else
   {
      if ((ret = SGetSBuf(inInit.region, inInit.pool, (Data **) &ctldPcb,
                          (Size) sizeof(InCtldPcb))) != ROK)
      {
#if (ERRCLASS & ERRCLS_DEBUG)
         INLOGERROR(ERRCLS_DEBUG, EIN685, (ErrVal) sizeof(InCtldPcb),
                    "InMiLinCfgReq() failed, unable to allocate memory.");
#endif
         RETVALUE(LCM_REASON_MEM_NOAVAIL);
      }

      /* bug 18804: initiate memory of control block */
      cmMemset((U8 *)ctldPcb, 0, sizeof(InCtldPcb));

      *(ctldPcbLstPtr + sapId) = ctldPcb;
      initCfg = TRUE;
   }   

   /* ignore the configuration request if the ctldPcb is in transfer state */
   if (initCfg == FALSE)
      /* TELICA-jwh-03/14/2001:  Just return if this is a reconfiguration. */
      RETVALUE(LCM_REASON_NOT_APPL);

   ctldPcb->suId = sapId;
   ctldPcb->rSuId = cfg->t.cfg.s.inDLSAP.rSuId;
   ret = inCtldPcbCfgHandler(cfg, ctldPcb, initCfg);
   RETVALUE(ret);
}/* end of inCtldSapCfgHandler */

/*
*
*      Fun:   inDLCCfgHandler
*
*      Desc: 
*
*      Ret:   ROK   - ok
*
*      Notes: None
*
*      File:  qn_bdy11.c
*
*/

#ifdef ANSI
PUBLIC S16 inDLCCfgHandler
(
 InMngmt *cfg
 )
#else
     PUBLIC S16 inDLCCfgHandler(cfg)
     InMngmt *cfg;
#endif
{

   SuId sapId;
   Ces ces;
   InPCB *pcb;
   InCb *cb;
   S16 ret;

   TRC3(inDLCCfgHandler);

#ifdef IN_LMINT3
   sapId = cfg->t.cfg.s.inLCe.sapId;
   ces = cfg->t.cfg.s.inLCe.ces;
#else
   sapId = cfg->hdr.elmId.elmntInst1;
   ces = cfg->hdr.elmId.elmntInst2;
#endif /* IN_LMINT3 */ 

   /* get physical link Control Block */
   if ((pcb = *(pcbLstPtr + sapId)) == NULLP)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      INLOGERROR(ERRCLS_INT_PAR, EIN686, (ErrVal) sapId,
                 "inDLCCfgHandler() failed, incorrect instance value.");
#endif
      RETVALUE(LCM_REASON_INVALID_SAP);
   }
     
 
   if ((cb = *(pcb->cesLst + ces)) == NULLP)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      INLOGERROR(ERRCLS_INT_PAR, EIN687, (ErrVal) ces,
                 "inDLCCfgHandler() failed, incorrect instance value.");
#endif
      RETVALUE(LCM_REASON_INVALID_SAP);
   }

   if ((ret = inCesCfg(cfg,pcb,cb)) != LCM_REASON_NOT_APPL)
      RETVALUE(ret);

   RETVALUE(LCM_REASON_NOT_APPL);

} /* end of inDLCCfgHandler */

#if 1 /* DEV_MGCBCH */
/* should only be called while add or delete B channels */
PUBLIC S16 inBchCfgHandler
(
   InMngmt *cfg
)
{
   S16 i;
   InBearChn *bearChn;
   InCtldPcb *ctldPcb;
   SuId sapId;

   TRC3(inBchCfgHandler);

   sapId = cfg->t.cfg.s.inDLSAP.sapId;

   if ((ctldPcb = *(ctldPcbLstPtr + sapId)) == NULLP)
   {
      INLOGERROR(ERRCLS_INT_PAR, EINXXX, (ErrVal)sapId,
                 "inBchCfgHandler() failed, unable to access ctldPcb.");
      RETVALUE(LCM_REASON_INVALID_SAP);
   }

   for (i = 1; i < 32; i++)
   {
      /* only act on those intended and valid channels */
      if (cfg->t.cfg.s.inDLSAP.bProf[i].valid && (i >= ctldPcb->cfg.firstBChanNum)
          && (i < ctldPcb->cfg.firstBChanNum + ctldPcb->cfg.nmbBearChan))
      {
         IN_GET_BEARER (ctldPcb, i, ctldPcb->cfg.firstBChanNum, bearChn);

         /* FID17233.0 Configure ISDN B channel direction */
         bearChn->chnDir = cfg->t.cfg.s.inDLSAP.dirBearChan[i];

         switch(cfg->t.cfg.s.inDLSAP.bProf[i].state)
         {
            case PROV:
               bearChn->status = OUT_SERVICE_NE;
               break;
           
            case NOT_PROV:
            {
               /* release call on this channel, only on active side */
               if ((bearChn->status != NOTPROVISIONED) && (zqCb.protState == ACTIVE))
               {
                  U8 tmrNum;

                  STOP_BCHAN_TIMERS;
                  
                  if (bearChn->ntc)
                  {
                     InPCB *sigPcb = NULLP;
                     InCb  *dCb = NULLP;

                     if ((sigPcb = *(pcbLstPtr + ctldPcb->cfg.sigInt)) == NULLP)
                     {
#if (ERRCLASS & ERRCLS_INT_PAR)
                        INLOGERROR(ERRCLS_INT_PAR, EIN705, 
                                   (ErrVal)ctldPcb->cfg.rSigInt,
                               "inBchCfgHandler() failed,Unable to Access Signalling Interface");
                        RETVALUE(LCM_REASON_NOT_APPL);
#endif
                     }
                        
                     /* get Cb */
                     if (!(dCb = *(sigPcb->cesLst + CONN_CES)))
                     {
#if (ERRCLASS & ERRCLS_INT_PAR)
                         INLOGERROR(ERRCLS_INT_PAR, EIN710, (ErrVal)sigPcb->suId,
                                    "inBchCfgHandler() failed, unable to access SAP.");
#endif
                         RETVALUE(LCM_REASON_INVALID_SAP);                      
                     }

                     /* NTC_DELETED is only used when ZQ_CTLDPSAP_DELNTC_CB or ZQ_CES_DELNTC_CB followed */
                     //bearChn->ntc->updStatus = NTC_DELETED;
                     INSETTOKVAL(bearChn->ntc->causeDgn.causeVal,CCTMPFAIL);
                     inGenRelUpCLw(bearChn->ntc, &bearChn->ntc->causeDgn, dCb);
                  }       
               }
               bearChn->status = NOTPROVISIONED;
               break;
            }
            case PROV_AVAIL:
            default:
               bearChn->status = AVAIL;
               break;
         }
      }
   }
   /* no Bchan update StaInd should send */
   RETVALUE(LCM_REASON_NOT_APPL);
}
#endif



/*
*
*      Fun:   inDigitMapCfgHandler
*
*      Desc: 
*
*      Ret:   ROK   - ok
*
*      Notes: None
*
*      File:  qn_bdy11.c
*
*/

PUBLIC S16 inDigitMapCfgHandler(InMngmt *cfg)
{
   S16         ret;
   LngAddrs    digitMapKey;
   U16         candidateId;
   U8          typeNmb;
   U8          listIdx;
   LngAddrs    digitMapStr;
   InDigitMap *pDigitMap;
   DigitMapCandidate *pCandidate;

   TRC3(inDigitMapCfgHandler);

   digitMapKey = cfg->t.cfg.s.inDgMap.digitMapName;
   candidateId = cfg->t.cfg.s.inDgMap.id;
   digitMapStr = cfg->t.cfg.s.inDgMap.digitMap;
   typeNmb     = cfg->t.cfg.s.inDgMap.noa;
   IN_CONVERT_TYPENMB_TO_LIST_IDX(typeNmb, listIdx);

   if ((ret = cmHashListFind(&inGlobalCb.digitMapHl, digitMapKey.strg, 
                             digitMapKey.length, (U16)0, (PTR *) &pDigitMap)) != ROK)
   {
      if ((ret = SGetSBuf(inInit.region, inInit.pool, (Data **) &pDigitMap,
                         (Size) sizeof(InDigitMap))) != ROK)
      {
         INLOGERROR(ERRCLS_DEBUG, EINXXX, (ErrVal) sizeof(InDigitMap),
                    "inDigitMapCfgHandler() failed, unable to allocate memory.");
         RETVALUE(LCM_REASON_MEM_NOAVAIL);
      }

      /* init a new digit map */
      inInitDigitMap(pDigitMap, &digitMapKey);

      /* insert one candidate to list */
      if ((ret = inAddCandidateToList(pDigitMap, &digitMapStr, candidateId, typeNmb)) != ROK)
      {
         INLOGERROR(ERRCLS_DEBUG, EINXXX, (ErrVal) sizeof(DigitMapCandidate),
                    "inDigitMapCfgHandler() failed, unable to allocate memory.");
         RETVALUE(LCM_REASON_MEM_NOAVAIL);
      }

      /* insert into hash list */
      if ((ret = cmHashListInsert(&inGlobalCb.digitMapHl, (PTR) pDigitMap,
                 pDigitMap->digitMapName.strg, pDigitMap->digitMapName.length)) != ROK)
      {
         /* free the memory allocated for digit map candidate */
         SPutSBuf(inInit.region, inInit.pool, pDigitMap->candidateList[listIdx].next,
                  (Size) sizeof(DigitMapCandidate));
        
         /* free the memory allocated for pDigitMap */
         SPutSBuf(inInit.region, inInit.pool, pDigitMap, (Size) sizeof(InDigitMap));

         RETVALUE(LCM_REASON_HASHING_FAILED); 
      }
   }
   else
   {
      /* continue to find digit map candidate */
      if ((ret = inFindCandidateInList(pDigitMap, candidateId, &pCandidate)) == ROK)
      {
         /* candidate found and update */
         cmMemset((U8 *)&pCandidate->pattern, 0, sizeof(TknStr));
         cmMemcpy(pCandidate->pattern.val, digitMapStr.strg, digitMapStr.length);
         pCandidate->pattern.pres = PRSNT_NODEF;
         pCandidate->pattern.len  = digitMapStr.length;

         if (pCandidate->typeNmb != typeNmb)
         {
            pCandidate->typeNmb = typeNmb;
            cmListDelete(&pCandidate->list);
            cmListInsert(pDigitMap->candidateList[listIdx].prev, &pCandidate->list);
         }
      }
      else
      {
         /* insert one candidate to list */
         if ((ret = inAddCandidateToList(pDigitMap, &digitMapStr, candidateId, typeNmb)) != ROK)
         {
            INLOGERROR(ERRCLS_DEBUG, EINXXX, (ErrVal) sizeof(DigitMapCandidate),
                       "inDigitMapCfgHandler() failed, unable to allocate memory.");
            RETVALUE(LCM_REASON_MEM_NOAVAIL);
         }
      }
   }

   RETVALUE(LCM_REASON_NOT_APPL);
} /* end of inDigitMapCfgHandler */



#ifdef ISDN_ROUTE

/*
 *
 *      Fun:   inRouteCfgHandler
 *
 *      Desc: 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None
 *
 *      File:  qn_bdy11.c
 *
*/

#ifdef ANSI
PUBLIC S16 inRouteCfgHandler
(
 InMngmt *cfg
 )
#else
     PUBLIC S16 inRouteCfgHandler(cfg)
     InMngmt *cfg;
#endif
{
   U8 i;
   S16 ret;
   InRoutCfg routTbl;

   TRC3(inRouteCfgHandler);

   if ((U16) (inTRoutRoot.entries + inLRoutRoot.entries + 1)
       > inCp.nmbRouts)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      INLOGERROR(ERRCLS_INT_PAR, EIN688, (ErrVal)inCp.nmbRouts,
                 "inRouteCfgHandler() failed, incorrect route configuration.");
#endif
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }
        
   /* copy configuration parameters */
   routTbl.addr.length = cfg->t.cfg.s.inRout.addr.length;
   for (i = 0; i < ADRLEN; i++)
      routTbl.addr.strg[i] = 0;

   inAscAdrToBcd(&cfg->t.cfg.s.inRout.addr.strg[0], 
                 &routTbl.addr.strg[0]);
   routTbl.sapId = cfg->t.cfg.s.inRout.sapId;
   routTbl.sapType = cfg->t.cfg.s.inRout.sapType;
   routTbl.pLink.lSap = cfg->t.cfg.s.inRout.pLink.lSap;
   routTbl.pLink.cEs = cfg->t.cfg.s.inRout.pLink.cEs;
        
   /* insert into routing table */
   if ((ret = inInsRout(&routTbl)) != ROK)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      INLOGERROR(ERRCLS_DEBUG, EIN690, (ErrVal) ret,
                 "InMiLinCfgReq() failed, unable to insert route.");
#endif
      RETVALUE(LCM_REASON_MEM_NOAVAIL);
   }

   RETVALUE(LCM_REASON_NOT_APPL);
} /* end of inRouteCfgHandler */

#endif




/*
 *
 *      Fun:   inBearCfgHandler
 *
 *      Desc: 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None
 *
 *      File:  qn_bdy11.c
 *
*/

#ifdef ANSI
PUBLIC S16 inBearCfgHandler
(
 InMngmt *cfg
 )
#else
     PUBLIC S16 inBearCfgHandler(cfg)
     InMngmt *cfg;
#endif
{
   InBearCfg *profTbl;
   TRC3(inBearCfgHandler);

   if (cfg->t.cfg.s.inBear.profNum >= inCp.nmbProfiles)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      INLOGERROR(ERRCLS_INT_PAR, EIN691, 
                 (ErrVal) cfg->t.cfg.s.inBear.profNum,
                 "inBearCfgHandler() failed, incorrect bearer profile number.");
#endif
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

   profTbl = inProfTbl + cfg->t.cfg.s.inBear.profNum;
   profTbl->profNum = cfg->t.cfg.s.inBear.profNum;
   profTbl->infoTranCap.enb = cfg->t.cfg.s.inBear.infoTranCap.enb;
   profTbl->infoTranCap.val = cfg->t.cfg.s.inBear.infoTranCap.val;
   profTbl->codeStand0.enb = cfg->t.cfg.s.inBear.codeStand0.enb;
   profTbl->codeStand0.val = cfg->t.cfg.s.inBear.codeStand0.val;
   profTbl->infoTranRate0.enb = cfg->t.cfg.s.inBear.infoTranRate0.enb;
   profTbl->infoTranRate0.val = cfg->t.cfg.s.inBear.infoTranRate0.val;
   profTbl->tranMode.enb = cfg->t.cfg.s.inBear.tranMode.enb;
   profTbl->tranMode.val = cfg->t.cfg.s.inBear.tranMode.val;

#ifdef ITUT88
   profTbl->establish.enb = cfg->t.cfg.s.inBear.establish.enb;
   profTbl->establish.val = cfg->t.cfg.s.inBear.establish.val;
   profTbl->cfg.enb = cfg->t.cfg.s.inBear.cfg.enb;
   profTbl->cfg.val = cfg->t.cfg.s.inBear.cfg.val;
   profTbl->chanStruct.enb = cfg->t.cfg.s.inBear.chanStruct.enb;
   profTbl->chanStruct.val = cfg->t.cfg.s.inBear.chanStruct.val;
   profTbl->infoTranRate1.enb = cfg->t.cfg.s.inBear.infoTranRate1.enb;
   profTbl->infoTranRate1.val = cfg->t.cfg.s.inBear.infoTranRate1.val;
   profTbl->symmetry.enb = cfg->t.cfg.s.inBear.symmetry.enb;
   profTbl->symmetry.val = cfg->t.cfg.s.inBear.symmetry.val;
#else
   profTbl->rateMultiplier.enb = cfg->t.cfg.s.inBear.rateMultiplier.enb;
   profTbl->rateMultiplier.val = cfg->t.cfg.s.inBear.rateMultiplier.val;
#endif /* ITUT88 */

   profTbl->negot.enb = cfg->t.cfg.s.inBear.negot.enb;
   profTbl->negot.val = cfg->t.cfg.s.inBear.negot.val;
   profTbl->syncAsync.enb = cfg->t.cfg.s.inBear.syncAsync.enb;
   profTbl->syncAsync.val = cfg->t.cfg.s.inBear.syncAsync.val;
   profTbl->interRate.enb = cfg->t.cfg.s.inBear.interRate.enb;
   profTbl->interRate.val = cfg->t.cfg.s.inBear.interRate.val;
   profTbl->inOutBandNegot.enb = cfg->t.cfg.s.inBear.inOutBandNegot.enb;
   profTbl->inOutBandNegot.val = cfg->t.cfg.s.inBear.inOutBandNegot.val;
   profTbl->asgnrAsgne.enb = cfg->t.cfg.s.inBear.asgnrAsgne.enb;
   profTbl->asgnrAsgne.val = cfg->t.cfg.s.inBear.asgnrAsgne.val;
   profTbl->logLnkNegot.enb = cfg->t.cfg.s.inBear.logLnkNegot.enb;
   profTbl->logLnkNegot.val = cfg->t.cfg.s.inBear.logLnkNegot.val;
   profTbl->mode.enb = cfg->t.cfg.s.inBear.mode.enb;
   profTbl->mode.val = cfg->t.cfg.s.inBear.mode.val;
   profTbl->multiFrm.enb = cfg->t.cfg.s.inBear.multiFrm.enb;
   profTbl->multiFrm.val = cfg->t.cfg.s.inBear.multiFrm.val;
   profTbl->parity.enb = cfg->t.cfg.s.inBear.parity.enb;
   profTbl->parity.val = cfg->t.cfg.s.inBear.parity.val;
   profTbl->nmbDatBits.enb = cfg->t.cfg.s.inBear.nmbDatBits.enb;
   profTbl->nmbDatBits.val = cfg->t.cfg.s.inBear.nmbDatBits.val;
   profTbl->nmbStopBits.enb = cfg->t.cfg.s.inBear.nmbStopBits.enb;
   profTbl->nmbStopBits.val = cfg->t.cfg.s.inBear.nmbStopBits.val;
   profTbl->duplexMode.enb = cfg->t.cfg.s.inBear.duplexMode.enb;
   profTbl->duplexMode.val = cfg->t.cfg.s.inBear.duplexMode.val;
   profTbl->usrInfoLyr2Prot0.enb = 
      cfg->t.cfg.s.inBear.usrInfoLyr2Prot0.enb;
   profTbl->usrInfoLyr2Prot0.val = 
      cfg->t.cfg.s.inBear.usrInfoLyr2Prot0.val;
   profTbl->usrInfoLyr3Prot0.enb = 
      cfg->t.cfg.s.inBear.usrInfoLyr3Prot0.enb;
   profTbl->usrInfoLyr3Prot0.val = 
      cfg->t.cfg.s.inBear.usrInfoLyr3Prot0.val;
   profTbl->codeStand1.enb = cfg->t.cfg.s.inBear.codeStand1.enb;
   profTbl->codeStand1.val = cfg->t.cfg.s.inBear.codeStand1.val;
   profTbl->presMethod.enb = cfg->t.cfg.s.inBear.presMethod.enb;
   profTbl->presMethod.val = cfg->t.cfg.s.inBear.presMethod.val;
   profTbl->interpret.enb = cfg->t.cfg.s.inBear.interpret.enb;
   profTbl->interpret.val = cfg->t.cfg.s.inBear.interpret.val;
   profTbl->highLyrCharId.enb = cfg->t.cfg.s.inBear.highLyrCharId.enb;
   profTbl->highLyrCharId.val = cfg->t.cfg.s.inBear.highLyrCharId.val;
   RETVALUE(LCM_REASON_NOT_APPL);
} /* end of inBearCfgHandler */


/*
*
*      Fun:   inPrcChanCfg
*
*      Desc: 
*
*      Ret:   ROK   - ok
*
*      Notes: None
*
*      File:  qn_bdy11.c
*
*/
PUBLIC S16 inPrcChanCfg (InMngmt *cfg, InCtldPcb *ctldPcb, Bool initCfg)
{
   S16 i;
   InBearChn *bBearer; 
   InPCB *sigPcb;
   S16 oldDChnNum;
   U8 dChanNum;
   Bool sigLnkCfged = TRUE;

   TRC3(inPrcChanCfg);

   oldDChnNum = 0;
   dChanNum = 0;
   /* if signaling link not configured yet, we need to differentiate. */

   if ((cfg->t.cfg.s.inDLSAP.nfasInt) && (cfg->t.cfg.s.inDLSAP.sigInt == 0) &&
       (cfg->t.cfg.s.inDLSAP.bupInt == 0))
      sigLnkCfged = FALSE;

   if (!sigLnkCfged)
   { /* only B link could reach here */
      if (initCfg)
      {
         for (i = ctldPcb->cfg.firstBChanNum; 
              i < (ctldPcb->cfg.nmbBearChan +ctldPcb->cfg.firstBChanNum);
              i++)
         {
            IN_GET_BEARER(ctldPcb, i, ctldPcb->cfg.firstBChanNum, bBearer);
            if ((ctldPcb->cfg.nmbBearChan == PRIM_EUR_RATE) 
                && (i == 0))
            {
               bBearer->status = IN_SERVICE;
               bBearer->dChan = TRUE;
            }
            else
            {
#if 1 /* DEV_MGCBCH : mark all B channels NOTPROVISIONED initially */
               bBearer->status = NOTPROVISIONED;
#else
               bBearer->status = ctldPcb->cfg.bProf[i].state;
               switch(ctldPcb->cfg.bProf[i].state)
               {
                  case PROV:
                     bBearer->status = OUT_SERVICE_NE;
                     break;
           
                  case NOT_PROV:
                     bBearer->status = NOTPROVISIONED;
                     break;
            
                  case PROV_AVAIL:
                  default:
                     bBearer->status = AVAIL;
                     break;
           
               }
#endif
               bBearer->dChan = FALSE;
            }

            bBearer->chnNmb = (U8) i;
            bBearer->chnDir = IN_BCHAN_BOTHWAY;
            bBearer->profile = ctldPcb->cfg.bProf[i].profNmb;

            /* ashish */
            bBearer->intId = ctldPcb->suId;
            bBearer->validate = ctldPcb->cfg.bProf[i].valid;
            bBearer->waitSrvAck = FALSE;
            bBearer->pendStat = 0;   
            bBearer->rst = FALSE;
            bBearer->suIdPres = TRUE;
            bBearer->suId = 0; /* temp set as 0, should be sigPcb->cfg.tCbId;*/
            bBearer->ntc = NULLP;
            bBearer->rstCnt = 0;
            /* init timer structure */
            cmInitTimers(bBearer->timers, MAXBCHTIMER);
         } /* End of for(i=..) */
      } /* InitCfg */
      /* nothing could be changed after first configuration */
      RETVALUE(LCM_REASON_NOT_APPL);
   }

   sigPcb = *(pcbLstPtr+ctldPcb->suId);

   if (cfg->t.cfg.s.inDLSAP.nfasInt)
   {
      dChanNum = 0xff;
      
      if (sigPcb != NULLP)
      {
         oldDChnNum = sigPcb->cfg.dChannelNum;
      }
   
      if ((sigPcb = *(pcbLstPtr+ctldPcb->cfg.sigInt)) == NULLP)
      {
         INLOGERROR(ERRCLS_INT_PAR, EIN692, 
                    (ErrVal)ctldPcb->cfg.rSigInt,
                    "inPrcChanCfg() failed, Unable to Access Signalling Interface.");
         RETVALUE(LCM_REASON_INVALID_SAP);
      }
   }
   else
   {
      if (sigPcb == NULLP)
      {
         INLOGERROR(ERRCLS_INT_PAR, EIN693, 
                    (ErrVal)ctldPcb->suId,
                    "inPrcChanCfg() failed, Unable to Access Signalling Interface.");
         RETVALUE(LCM_REASON_INVALID_SAP);
      } 
   }
   if ((!cfg->t.cfg.s.inDLSAP.nfasInt) ||
       ((cfg->t.cfg.s.inDLSAP.nfasInt) &&
        ((ctldPcb->suId == sigPcb->cfg.sigInt) ||
         ((sigPcb->cfg.buIntPr) && (ctldPcb->suId == sigPcb->cfg.bupInt)))))
   {
      if ((cfg->t.cfg.s.inDLSAP.nfasInt) && (sigPcb->cfg.buIntPr) && (ctldPcb->suId == sigPcb->cfg.bupInt))
      {
         if ((sigPcb = *(pcbLstPtr+sigPcb->cfg.bupInt)) == NULLP)
         {
            INLOGERROR(ERRCLS_INT_PAR, EIN692, 
                       (ErrVal)sigPcb->cfg.rBupInt,
                       "inPrcChanCfg() failed, Unable to Access Backup Signalling Interface.");
            RETVALUE(LCM_REASON_INVALID_SAP);
         }
      }

      oldDChnNum = sigPcb->cfg.dChannelNum;
      dChanNum = sigPcb->cfg.dChannelNum = cfg->t.cfg.s.inDLSAP.dChannelNum;
    
      switch (ctldPcb->cfg.nmbBearChan)
      {
         case BASICRATE:
            sigPcb->cfg.dChannelNum = 0;
            break;
         case PRIM_US_RATE:
            break;
         case PRIM_EUR_RATE:
            break;
         default:
            break;
      }   
   } /* end if */

   if (initCfg)
   {
      for (i = ctldPcb->cfg.firstBChanNum; 
           i < (ctldPcb->cfg.nmbBearChan +ctldPcb->cfg.firstBChanNum);
           i++)
      {
         IN_GET_BEARER(ctldPcb, i, ctldPcb->cfg.firstBChanNum, bBearer);

         if ((i == dChanNum) && (i > BASICRATE) && 
             (!(sigPcb->cfg.nfasInt) ||
              ((ctldPcb->suId == (S16) sigPcb->cfg.sigInt) || 
               ((sigPcb->cfg.buIntPr) && 
                (ctldPcb->suId == (S16) sigPcb->cfg.bupInt)))))
         {
            bBearer->status = IN_SERVICE;
            bBearer->dChan = TRUE;
         }
         else
         {
            if ((ctldPcb->cfg.nmbBearChan == PRIM_EUR_RATE) 
                && (i == 0))
            {
               bBearer->status = IN_SERVICE;
               bBearer->dChan = TRUE;
            }
            else
            {
#if 1 /* DEV_MGCBCH : mark all B channels NOTPROVISIONED initially */
               bBearer->status = NOTPROVISIONED;
#else
               bBearer->status = ctldPcb->cfg.bProf[i].state;
               switch(ctldPcb->cfg.bProf[i].state)
               {
                  case PROV:
                     bBearer->status = OUT_SERVICE_NE;
                     break;
              
                  case NOT_PROV:
                     bBearer->status = NOTPROVISIONED;
                     break;
              
                  case PROV_AVAIL:
                  default:
                     bBearer->status = AVAIL;
                     break;
              
               }
#endif
               bBearer->dChan = FALSE;
            }
         }
         
         bBearer->chnNmb = (U8) i;
         bBearer->profile = ctldPcb->cfg.bProf[i].profNmb;

         /* ashish */
         bBearer->intId = ctldPcb->suId;
         bBearer->validate = ctldPcb->cfg.bProf[i].valid;
         bBearer->waitSrvAck = FALSE;
         bBearer->pendStat = 0;   
         bBearer->rst = FALSE;
         bBearer->suIdPres = TRUE;
         bBearer->suId = sigPcb->cfg.tCbId;
         bBearer->ntc = NULLP;
         bBearer->rstCnt = 0;
         /* init timer structure */
         cmInitTimers(bBearer->timers, MAXBCHTIMER);
      } /* End of for(i=..) */
   } /* InitCfg */
   else
   {  
      if (!(sigPcb->cfg.nfasInt) ||
          ((ctldPcb->suId == (S16) sigPcb->cfg.sigInt) || 
           ((sigPcb->cfg.buIntPr) && 
            (ctldPcb->suId == (S16) sigPcb->cfg.bupInt))))
      {
         IN_GET_BEARER(ctldPcb,dChanNum, 
                       ctldPcb->cfg.firstBChanNum, bBearer);
         bBearer->status = IN_SERVICE;
         bBearer->dChan = TRUE;
         if (oldDChnNum != sigPcb->cfg.dChannelNum)
         {
            IN_GET_BEARER(ctldPcb,oldDChnNum, ctldPcb->cfg.firstBChanNum, bBearer);
            bBearer->status = OUT_SERVICE_NE;
            bBearer->dChan = FALSE;
         }
      }
      else
      {
         if ((oldSigInt == ctldPcb->suId) || 
             (oldBkupInt == ctldPcb->suId))
         {
            IN_GET_BEARER(ctldPcb,oldDChnNum, ctldPcb->cfg.firstBChanNum, 
                          bBearer);
            bBearer->status = OUT_SERVICE_NE;
            bBearer->dChan = FALSE;
         }
      }
   } /* else */

   RETVALUE(LCM_REASON_NOT_APPL);
} /* end of inPrcChanCfg */







/*
 *
 *      Fun:   inInitTmr
 *
 *      Desc: 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None
 *
 *      File:  qn_bdy11.c
 *
*/

#ifdef ANSI
PUBLIC S16 inInitTmr
(
 InMngmt *cfg,
 InPCB *pcb
 )
#else
     PUBLIC S16 inInitTmr(cfg, pcb)
     InMngmt *cfg;
     InPCB *pcb;
#endif
{
  TRC3(inInitTmr);

  INSETTMRVAL(t301);
  INSETTMRVAL(t302);
  INSETTMRVAL(t303);
  INSETTMRVAL(t304);
  INSETTMRVAL(t305);
  INSETTMRVAL(t306);
  INSETTMRVAL(t307);
  INSETTMRVAL(t308);
  INSETTMRVAL(t310);
  INSETTMRVAL(t312);
  INSETTMRVAL(t313);
  INSETTMRVAL(t316c);
  INSETTMRVAL(t316);
  INSETTMRVAL(t318);
  INSETTMRVAL(t319);
  INSETTMRVAL(t322);
  INSETTMRVAL(t330);
  INSETTMRVAL(t331);
  INSETTMRVAL(t332);
  INSETTMRVAL(tRst);
  INSETTMRVAL(tAns);
  INSETTMRVAL(t396);
  INSETTMRVAL(t397);
  INSETTMRVAL(tProg);
  INSETTMRVAL(tRest);
#ifdef DAT2
  INSETTMRVAL(tIntTmr);
#endif /* DAT2 */
#ifdef ETSI             /* 2111 overlap */
  INSETTMRVAL(tMinDigits);
#endif
  RETVALUE (ROK);

} /* end of inInitTmr */







/*
 *
 *      Fun:   inFindOldInt
 *
 *      Desc: 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None
 *
 *      File:  qn_bdy11.c
 *
*/

#ifdef ANSI
PUBLIC S16 inFindOldInt
(
 InMngmt *cfg,
 Bool initCfg,
 SuId sapId
 )
#else
     PUBLIC S16 inFindOldInt(cfg, initCfg, sapId)
     InMngmt *cfg;
     Bool initCfg;
     SuId sapId;
#endif
{
   InCtldPcb *ctldPcb;
   InPCB *tmpPcb;
   InPCB *sigPcb;
   S16 i;

   TRC3(inFindOldInt);

   ctldPcb = *(ctldPcbLstPtr + sapId);

   if (ctldPcb == NULLP)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      INLOGERROR(ERRCLS_INT_PAR, EINXXX, 
                 (ErrVal)sapId,
                 "inFindOldInt() failed, Unable to Access Controled Interface");
#endif
      RETVALUE(LCM_REASON_INVALID_SAP);
   }

   if ((sigPcb = *(pcbLstPtr + cfg->t.cfg.s.inDLSAP.sigInt)) == NULLP)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      INLOGERROR(ERRCLS_INT_PAR, EIN694, 
                 (ErrVal)cfg->t.cfg.s.inDLSAP.rSigInt,
                 "inFindOldInt() failed, Unable to Access Signalling Interface");
#endif
      RETVALUE(LCM_REASON_INVALID_SAP);
   }


   if (initCfg)
   {
      oldSigInt = INT_INV;
      oldBkupInt = INT_INV;
    
      /* Identify old signalling/ backup interface if a new sig/backup
         interface is being configured. */
      if ((ctldPcb->suId == cfg->t.cfg.s.inDLSAP.sigInt) ||
          (ctldPcb->suId == cfg->t.cfg.s.inDLSAP.bupInt))
      {
         for (i = 0; i < sigPcb->cfg.maxNmbNfasInt; i++)
         {
            /* TELICA-jwh-06/07/2001:  Make sure ctldInt is valid */
            if ((*(sigPcb->ctldInt+i) != INT_NOT_CFGD) &&
                ((tmpPcb =  *(pcbLstPtr + *(sigPcb->ctldInt + i))) != NULLP))
            {
               if ((ctldPcb->suId == cfg->t.cfg.s.inDLSAP.sigInt) &&
                   (tmpPcb->suId == (S16)tmpPcb->cfg.sigInt) &&
                   (tmpPcb->suId != ctldPcb->suId))
               {
                  oldSigInt = tmpPcb->suId;
                  break;
               }
               if ((ctldPcb->suId == cfg->t.cfg.s.inDLSAP.bupInt) &&
                   ((tmpPcb->suId == (S16)tmpPcb->cfg.bupInt) &&
                    (tmpPcb->cfg.buIntPr))&&
                   (tmpPcb->suId != ctldPcb->suId))
               {
                  oldSigInt = tmpPcb->cfg.sigInt;
                  oldBkupInt = tmpPcb->cfg.bupInt;
                  break;
               }
            } /* if tmpCtldPcb */
         } /* End for */
      } /* Signalling / Backup Interface */      
   } /* InitCfg */
   else 
   {
      oldSigInt = ctldPcb->cfg.sigInt;

      if (oldSigInt != INT_INV)
      {
         tmpPcb = *(pcbLstPtr + oldSigInt);

         if ((tmpPcb) && (tmpPcb->cfg.buIntPr))
            oldBkupInt = tmpPcb->cfg.bupInt;
         else
            oldBkupInt = INT_INV;
      }
      else
         oldBkupInt = INT_INV;
   }

   RETVALUE(LCM_REASON_NOT_APPL);
} /* end of inFindOldInt */



/*
 *
 *      Fun:   inCtldPcbCfgHandler
 *
 *      Desc: 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None
 *
 *      File:  qn_bdy11.c
 *
*/

#ifdef ANSI
PUBLIC S16 inCtldPcbCfgHandler
(
 InMngmt *cfg,
 InCtldPcb *ctldPcb,
 Bool initCfg
 )
#else
     PUBLIC S16 inCtldPcbCfgHandler(cfg, ctldPcb, initCfg)
     InMngmt *cfg;
     InCtldPcb *ctldPcb;
     Bool initCfg;
#endif
{
   InPCB *sigPcb;
   S16 i;
   S16 ret;
   S16 version;
   S16 peerVersion;
   InCtldPcb *tmpCtldPcb = NULLP;
   TRC3(inCtldPcbCfgHandler);


   ctldPcb->spId = (SpId) cfg->t.cfg.s.inDLSAP.spId;
   ctldPcb->rSuId = cfg->t.cfg.s.inDLSAP.rSuId;
   ctldPcb->state = CBST1XFER;
   ctldPcb->rstCnt = 0;

   ctldPcb->cfg.clrGlr = cfg->t.cfg.s.inDLSAP.clrGlr;

   oldSigInt = INT_INV;
   oldBkupInt = INT_INV;

   if ((cfg->t.cfg.s.inDLSAP.nfasInt) && (!initCfg))
   {
      if ((ret = inFindOldInt(cfg,initCfg, ctldPcb->suId)) != 
          LCM_REASON_NOT_APPL)
         RETVALUE(ret);
   }

   if (initCfg)
   {
      ctldPcb->cfg.firstBChanNum = cfg->t.cfg.s.inDLSAP.firstBChanNum;
      ctldPcb->cfg.nmbBearChan = cfg->t.cfg.s.inDLSAP.nmbBearChan;
    
      for (i = ctldPcb->cfg.firstBChanNum; 
           i < (ctldPcb->cfg.nmbBearChan + ctldPcb->cfg.firstBChanNum); i++)
      {
         ctldPcb->cfg.bProf[i].valid = cfg->t.cfg.s.inDLSAP.bProf[i].valid;
         ctldPcb->cfg.bProf[i].profNmb = cfg->t.cfg.s.inDLSAP.bProf[i].profNmb;
         ctldPcb->cfg.bProf[i].state = cfg->t.cfg.s.inDLSAP.bProf[i].state;   
      }

      /* Bearer Channel */
      if((ret = SGetSBuf(inInit.region, inInit.pool, (Data **) &ctldPcb->bBearer,
                         (Size) ((ctldPcb->cfg.nmbBearChan + ctldPcb->cfg.firstBChanNum) * 
                                 sizeof(InBearChn)))) != ROK)
      {
         INLOGERROR(ERRCLS_DEBUG, EINXXX, (ErrVal) sizeof(InBearChn),
                    "InMiLinCfgReq() failed, unable to allocate memory.");
         RETVALUE(LCM_REASON_MEM_NOAVAIL);
      }
   }
    
   ctldPcb->cfg.nfasInt = cfg->t.cfg.s.inDLSAP.nfasInt;
   ctldPcb->cfg.intId = cfg->t.cfg.s.inDLSAP.intId;
   ctldPcb->cfg.sigInt = cfg->t.cfg.s.inDLSAP.sigInt;
   ctldPcb->cfg.swtch = cfg->t.cfg.s.inDLSAP.swtch;
   ctldPcb->cfg.rSigInt = cfg->t.cfg.s.inDLSAP.rSigInt;

   version =  GET_API_VER (API_PSF_Q931_VER_IDX);
   peerVersion =  GET_PEER_API_VER (API_PSF_Q931_VER_IDX);
   /* HUG code for FID 16537.0. In sert ctldPcb to hash list */
    if((GET_MAJOR_VER(peerVersion) == API_83_MAJOR)&&
       (GET_MAJOR_VER(version)==API_91_MAJOR)&&(GET_MINOR_VER(version)==0x1))
   {
      ret = cmHashListFind(&inGlobalCb.ctldPcbHl, (U8 *)&ctldPcb->rSuId, (U16)sizeof(U32),
                                   (U16)0, (PTR *)&tmpCtldPcb);
      if(ret != ROK)
      {
         /* Insert new entry in hash list */
         if (cmHashListInsert(&inGlobalCb.ctldPcbHl, (PTR)ctldPcb,
                     (U8 *)&ctldPcb->rSuId,
                     sizeof(U32)) != ROK)
         {
            RETVALUE(ret); 
         }
      }
   }
   else if((GET_MAJOR_VER(peerVersion) == API_84_MAJOR)&&(GET_MINOR_VER(peerVersion)<0xa)&&
           (GET_MAJOR_VER(version)==API_91_MAJOR)&&(GET_MINOR_VER(version)==0x1))
   {
      ret = cmHashListFind(&inGlobalCb.ctldPcbHl, (U8 *)&ctldPcb->rSuId, (U16)sizeof(U32),
                                   (U16)0, (PTR *)&tmpCtldPcb);
      if(ret != ROK)
      {
         /* Insert new entry in hash list */
         if (cmHashListInsert(&inGlobalCb.ctldPcbHl, (PTR)ctldPcb,
                     (U8 *)&ctldPcb->rSuId,
                     sizeof(U32)) != ROK)
         {
            RETVALUE(ret); 
         }
      }
   }

  
   if (ctldPcb->cfg.nfasInt)
   {
      if ((cfg->t.cfg.s.inDLSAP.buIntPr == TRUE) && 
          (cfg->t.cfg.s.inDLSAP.bupInt == ctldPcb->suId))
      {
         if ((sigPcb = *(pcbLstPtr + ctldPcb->cfg.sigInt)) == NULLP)
         {
#if (ERRCLASS & ERRCLS_INT_PAR)
            INLOGERROR(ERRCLS_INT_PAR, EIN887, (ErrVal)ctldPcb->cfg.rSigInt,
                       "inCtldPcbCfgHandler failed, invalid sap id");
            RETVALUE(LCM_REASON_INVALID_SAP);
#endif
         }
         if (!sigPcb->cfg.buIntPr)
         {
            sigPcb->cfg.buIntPr = TRUE;
            sigPcb->cfg.bupInt = ctldPcb->suId;
         }
      }

      if ((ret = inPrcCtldPcbCfg(ctldPcb)) != LCM_REASON_NOT_APPL)
         RETVALUE(ret);
   }
      
   /* Initialize timer structure */
   cmInitTimers(ctldPcb->timers, MAXBCHTIMER);

   /* Handle Bearer Channel Initialization */
   ret = inPrcChanCfg(cfg, ctldPcb, initCfg);
   RETVALUE(ret);
} /* end of inCtldPcbCfgHandler */




/*
*
*      Fun:   inInitTCb
*
*      Desc: 
*
*      Ret:   ROK   - ok
*
*      Notes: None
*
*      File:  qn_bdy11.c
*
*/
#ifdef ANSI
PUBLIC S16 inInitPcb
(
 InPCB *pcb,
 InMngmt *cfg,
 Bool initCfg
 )
#else
     PUBLIC S16 inInitPcb(pcb, cfg, initCfg)
     InPCB *pcb;
     InMngmt *cfg;
     Bool initCfg;
#endif
{
   S16 i;

   TRC3(inInitPcb);
  
   pcb->spId = (SpId) cfg->t.cfg.s.inDLSAP.spId;
   if (initCfg)
      pcb->bndState = IN_UNBND;

   /* use NETWORK even if configured as USER for interface type,
    * store the original intType in svcType - BUG:26000-mqin
    */
   pcb->cfg.intType = NETWORK;
   pcb->cfg.svcType = cfg->t.cfg.s.inDLSAP.intType;
   pcb->cfg.intCfg = cfg->t.cfg.s.inDLSAP.intCfg;
   pcb->cfg.n201 = cfg->t.cfg.s.inDLSAP.n201;
   pcb->cfg.nmbRst = cfg->t.cfg.s.inDLSAP.nmbRst;
   pcb->cfg.numRstInd = cfg->t.cfg.s.inDLSAP.numRstInd;
   pcb->cfg.ackOpt = cfg->t.cfg.s.inDLSAP.ackOpt;
   pcb->cfg.relOpt = cfg->t.cfg.s.inDLSAP.relOpt;
   pcb->cfg.facilityHandling = cfg->t.cfg.s.inDLSAP.facilityHandling;
   pcb->cfg.statEnqOpt = cfg->t.cfg.s.inDLSAP.statEnqOpt;
   pcb->cfg.nmbCes = cfg->t.cfg.s.inDLSAP.nmbCes;
   pcb->cfg.callRefLen = cfg->t.cfg.s.inDLSAP.callRefLen;
   pcb->cfg.teiAlloc = cfg->t.cfg.s.inDLSAP.teiAlloc;
   pcb->cfg.minAdrDig = cfg->t.cfg.s.inDLSAP.minAdrDig;
   pcb->cfg.useSubAdr = cfg->t.cfg.s.inDLSAP.useSubAdr;
   pcb->cfg.keyPad = cfg->t.cfg.s.inDLSAP.keyPad;
   pcb->cfg.tCbCfg = cfg->t.cfg.s.inDLSAP.tCbCfg;
   pcb->cfg.tCbId = cfg->t.cfg.s.inDLSAP.tCbId;

   for (i = 0; i < ADRLEN; i++)
      pcb->cfg.wcMask[i] = 0;

#ifdef ISDN_ROUTE
   if ((pcb->cfg.wcRout = cfg->t.cfg.s.inDLSAP.wcRout) != FALSE)
      inAscMaskToBcd(&cfg->t.cfg.s.inDLSAP.wcMask[0], 
                     &pcb->cfg.wcMask[0]);
   else
      for (i = 0; i < ADRLEN; i++)
         pcb->cfg.wcMask[i] = 0xff;

   if ((pcb->cfg.adrPref = cfg->t.cfg.s.inDLSAP.adrPref) != FALSE)
   {
      pcb->cfg.nmbPrefDig = cfg->t.cfg.s.inDLSAP.nmbPrefDig;
      for (i = 0; i < MAXPREFDIG; i++)
         pcb->cfg.prefix[i] = cfg->t.cfg.s.inDLSAP.prefix[i];
      inAscAdrToBcd(&cfg->t.cfg.s.inDLSAP.prefix[0], 
                    &pcb->cfg.prefix[0]);
   }
   else
#endif /*ISDN_ROUTE*/
   {
      pcb->cfg.nmbPrefDig = 0;
      for (i = 0; i < MAXPREFDIG; i++)
         pcb->cfg.prefix[i] = 0;
   }
  
   if ((pcb->cfg.callId.len = cfg->t.cfg.s.inDLSAP.callId.len) != 0)
      for (i = 0; i < MF_SIZE_TKNSTRS; i++)
         pcb->cfg.callId.val[i] = cfg->t.cfg.s.inDLSAP.callId.val[i];
   else
      for (i = 0; i < MF_SIZE_TKNSTRS; i++)
         pcb->cfg.callId.val[i] = 0;
  
   for (i = 0; i < ADRLEN; i++)
      pcb->cfg.sid.strg[i] = 0;

   pcb->cfg.sidIns = cfg->t.cfg.s.inDLSAP.sidIns;
   pcb->cfg.sidTon = cfg->t.cfg.s.inDLSAP.sidTon;
   pcb->cfg.sidNPlan = cfg->t.cfg.s.inDLSAP.sidNPlan;
   pcb->cfg.sidPresInd = cfg->t.cfg.s.inDLSAP.sidPresInd;
   pcb->cfg.sid.length = cfg->t.cfg.s.inDLSAP.sid.length;

   for (i = 0; i < ADRLEN; i++)
      pcb->cfg.sid.strg[i] = cfg->t.cfg.s.inDLSAP.sid.strg[i];

   pcb->cfg.cndSubsc = cfg->t.cfg.s.inDLSAP.cndSubsc;
   pcb->cfg.redirSubsc = cfg->t.cfg.s.inDLSAP.redirSubsc;
   pcb->cfg.forwSubsc = cfg->t.cfg.s.inDLSAP.forwSubsc;
   if (pcb->cfg.redirSubsc || pcb->cfg.forwSubsc)
   {
      pcb->cfg.redirAdr.eh.pres = PRSNT_NODEF;
      pcb->cfg.redirAdr.nmbPlanId.pres = PRSNT_NODEF;
      pcb->cfg.redirAdr.nmbPlanId.val = 
         cfg->t.cfg.s.inDLSAP.redirAdr.nmbPlanId.val;
      pcb->cfg.redirAdr.typeNmb.pres = PRSNT_NODEF;
      pcb->cfg.redirAdr.typeNmb.val = 
         cfg->t.cfg.s.inDLSAP.redirAdr.typeNmb.val;
      pcb->cfg.redirAdr.screenInd.pres = PRSNT_NODEF;
      pcb->cfg.redirAdr.screenInd.val = 
         cfg->t.cfg.s.inDLSAP.redirAdr.screenInd.val;
      pcb->cfg.redirAdr.presInd.pres = PRSNT_NODEF;
      pcb->cfg.redirAdr.presInd.val = 
         cfg->t.cfg.s.inDLSAP.redirAdr.presInd.val;
      pcb->cfg.redirAdr.rsnRedirect.pres = NOTPRSNT;
      pcb->cfg.redirAdr.rsnRedirect.val = 0;
      pcb->cfg.redirAdr.nmbDigits.pres = PRSNT_NODEF;
      pcb->cfg.redirAdr.nmbDigits.len = 
         cfg->t.cfg.s.inDLSAP.redirAdr.nmbDigits.len;
      for (i = 0; (U8) i < pcb->cfg.redirAdr.nmbDigits.len; i++)
         pcb->cfg.redirAdr.nmbDigits.val[i] = 
            cfg->t.cfg.s.inDLSAP.redirAdr.nmbDigits.val[i];
   }


   if ((pcb->cfg.srvOpt = cfg->t.cfg.s.inDLSAP.srvOpt) != FALSE)
   {
      if ((cfg->t.cfg.s.inDLSAP.intType != NETWORK) &&
          (pcb->cfg.swtch != SW_NTDMS100P) &&
          (pcb->cfg.swtch != SW_NTDMS250) &&
          (pcb->cfg.swtch != SW_ATT4E) && 
          (pcb->cfg.swtch != SW_ATT5EP) &&
          (pcb->cfg.swtch != SW_NTMCI) &&
          (pcb->cfg.swtch != SW_NTNI) &&
          (pcb->cfg.swtch != SW_QSIG))
      {
#if (ERRCLASS & ERRCLS_INT_PAR)
         INLOGERROR(ERRCLS_INT_PAR, EIN695, (ErrVal)cfg->t.cfg.s.inDLSAP.srvOpt,
                    "InMiLinCfgReq() failed, invalid option.");
#endif
         RETVALUE(LCM_REASON_INVALID_PAR_VAL);
      }
   }

   pcb->cfg.rstOpt = cfg->t.cfg.s.inDLSAP.rstOpt;

  /* TELICA-MCAO-PRJ-NI2-SRV_MSG-- begin of code change */
   pcb->cfg.bcasOpt = cfg->t.cfg.s.inDLSAP.bcasOpt;
   pcb->cfg.mimOpt = cfg->t.cfg.s.inDLSAP.mimOpt;
   pcb->cfg.ringOff = cfg->t.cfg.s.inDLSAP.ringOff;
 

#ifdef ISDN_PTMPT
   /* Initialize the PCB structure array list.
   * This field needs to be initialized only if its 
   * network side point to multipoint interface.
   * No need to check for signalling interface since the 
   * configuration should not be INTCFG_MULTI for NFAS/PRI
   * interfaces.
   */

   if ((pcb->cfg.intType == NETWORK) 
       && (pcb->cfg.intCfg == INTCFG_MULTI))
   {
      for (i = 0; i < MAXNTCNUM; i++)
      {
         pcb->mptCallList[i] = NULLP;
      }  /* End of for loop */
   } /* ((pcb->cfg.intType == NETWORK) */

#endif /* ISDN_PTMPT */

   if (initCfg)
   {
      pcb->pst.selector = cfg->t.cfg.s.inDLSAP.selector;
      pcb->pst.region = cfg->t.cfg.s.inDLSAP.mem.region;
      pcb->pst.pool = cfg->t.cfg.s.inDLSAP.mem.pool;
      pcb->pst.prior = cfg->t.cfg.s.inDLSAP.prior;
      pcb->pst.route = cfg->t.cfg.s.inDLSAP.route;
      pcb->pst.dstProcId = cfg->t.cfg.s.inDLSAP.dstProcId;
      pcb->pst.dstEnt = cfg->t.cfg.s.inDLSAP.dstEnt;
      pcb->pst.dstInst = cfg->t.cfg.s.inDLSAP.dstInst;
      pcb->pst.srcProcId = inInit.procId;
      pcb->pst.srcEnt = inInit.ent;
      pcb->pst.srcInst = inInit.inst;
   }

   pcb->flc = FALSE;
   pcb->rstAll = FALSE;
   pcb->cfg.nfasInt = cfg->t.cfg.s.inDLSAP.nfasInt;
   pcb->cfg.buIntPr = cfg->t.cfg.s.inDLSAP.buIntPr;
   pcb->cfg.sigInt = cfg->t.cfg.s.inDLSAP.sigInt;
   pcb->cfg.bupInt = cfg->t.cfg.s.inDLSAP.bupInt;
   pcb->cfg.rSigInt = cfg->t.cfg.s.inDLSAP.rSigInt;
   pcb->cfg.rBupInt = cfg->t.cfg.s.inDLSAP.rBupInt;

   /* TELICA-jwh-10/23/2000:  maxNmbNfasInt and nmbCtldInt should be set only during initial configuration */
   if (initCfg)
   {
      pcb->cfg.maxNmbNfasInt = cfg->t.cfg.s.inDLSAP.nmbNfasInt;
      pcb->nmbCtldInt = 0;
   }
   /* 2111 overlap */
   pcb->cfg.overlap = cfg->t.cfg.s.inDLSAP.overlap;
   pcb->cfg.minDigits = cfg->t.cfg.s.inDLSAP.minDigits;
   pcb->cfg.maxDigits = cfg->t.cfg.s.inDLSAP.maxDigits;

   /* FID:15070 */
   pcb->cfg.numPL = NONENP;
   /* FID 17412.0  */
   pcb->cfg.connectDateTimeIE = IN_CONNECT_WO_DATE_TIME;
   /* FID 17524.0 */
   pcb->cfg.channelId = FALSE;
   /* FID 17555.0 Kavon */
   pcb->cfg.playDialTone = FALSE;
 
   if(cfg->t.cfg.s.inDLSAP.remark.length > 0)
   {
      if (cmStrcmp("NPPL", cfg->t.cfg.s.inDLSAP.remark.strg) == 0)
      {
         pcb->cfg.numPL = NPPL;
      }
      else if (cmStrcmp("LOCALWOSECS", cfg->t.cfg.s.inDLSAP.remark.strg) == 0)
      {
         pcb->cfg.connectDateTimeIE = IN_CONNECT_W_DATE_TIME_LOCALWOSECS;
         pcb->cfg.channelId = TRUE;
         pcb->cfg.playDialTone = TRUE;
      }
      else if (cmStrcmp("UTCWOSECS", cfg->t.cfg.s.inDLSAP.remark.strg) == 0)
      {
         pcb->cfg.connectDateTimeIE = IN_CONNECT_W_DATE_TIME_UTCWOSECS;
         pcb->cfg.channelId = TRUE;
         pcb->cfg.playDialTone = TRUE;
      }
      else if (cmStrcmp("LOCAL", cfg->t.cfg.s.inDLSAP.remark.strg) == 0)
      {
         pcb->cfg.connectDateTimeIE = IN_CONNECT_W_DATE_TIME_LOCAL;
         pcb->cfg.channelId = TRUE;
         pcb->cfg.playDialTone = TRUE;
      }
      else if (cmStrcmp("UTC", cfg->t.cfg.s.inDLSAP.remark.strg) == 0)
      {
         pcb->cfg.connectDateTimeIE = IN_CONNECT_W_DATE_TIME_UTC;
         pcb->cfg.channelId = TRUE;
         pcb->cfg.playDialTone = TRUE;
      }
   }

   INDBGP(DBGMASK_MI,"%s: numPL=%d, connectDateTimeIE=%d, channelId=%d, playDialTone=%d\n", __FUNCTION__,
                      pcb->cfg.numPL,     pcb->cfg.connectDateTimeIE, pcb->cfg.channelId, pcb->cfg.playDialTone);

   /* FID 16206.0 */
   pcb->cfg.cfCfg = cfg->t.cfg.s.inDLSAP.cfCfg;
   pcb->gr_bnd = FALSE;

   pcb->tConRestarted = FALSE;

   /* FID 16706.0 */
   pcb->cfg.aoc = cfg->t.cfg.s.inDLSAP.aoc;

   /* FID 16779.0 */
   pcb->cfg.digitMapName = cfg->t.cfg.s.inDLSAP.digitMapName;

   RETVALUE(LCM_REASON_NOT_APPL);
} /* end */

/*
*
*      Fun:   inPartialRecfgDLSAP
*
*      Desc: 
*
*      Ret:   ROK   - ok
*
*      Notes: None
*
*      File:  qn_bdy11.c
*
*/

#ifdef ANSI
PUBLIC S16 inPartialRecfgDLSAP
(
 InPCB *pcb,
 InCtldPcb *ctldPcb,
 InMngmt *cfg
 )
#else
     PUBLIC S16 inPartialRecfgDLSAP(pcb, ctldPcb, cfg)
     InPCB *pcb;
     InCtldPcb *ctldPcb;
     InMngmt *cfg;
#endif
{
   S16 i;

   TRC3(inPartialRecfgDLSAP);
  
   ctldPcb->cfg.clrGlr = cfg->t.cfg.s.inDLSAP.clrGlr;
   if ( ctldPcb->suId != pcb->suId)
      RETVALUE(LCM_REASON_NOT_APPL);

   pcb->cfg.n201 = cfg->t.cfg.s.inDLSAP.n201;
   pcb->cfg.nmbRst = cfg->t.cfg.s.inDLSAP.nmbRst;
   pcb->cfg.numRstInd = cfg->t.cfg.s.inDLSAP.numRstInd;
   pcb->cfg.ackOpt = cfg->t.cfg.s.inDLSAP.ackOpt;
   pcb->cfg.relOpt = cfg->t.cfg.s.inDLSAP.relOpt;
   pcb->cfg.facilityHandling = cfg->t.cfg.s.inDLSAP.facilityHandling;
   pcb->cfg.statEnqOpt = cfg->t.cfg.s.inDLSAP.statEnqOpt;

   pcb->cfg.teiAlloc = cfg->t.cfg.s.inDLSAP.teiAlloc;
   pcb->cfg.minAdrDig = cfg->t.cfg.s.inDLSAP.minAdrDig;
   pcb->cfg.useSubAdr = cfg->t.cfg.s.inDLSAP.useSubAdr;
   pcb->cfg.keyPad = cfg->t.cfg.s.inDLSAP.keyPad;

   if (pcb->cfg.swtch != cfg->t.cfg.s.inDLSAP.swtch)
   {
      INDBGP(DBGMASK_MI,"changing interface switch from %d to %d\n", pcb->cfg.swtch,
          cfg->t.cfg.s.inDLSAP.swtch);
      pcb->cfg.swtch = cfg->t.cfg.s.inDLSAP.swtch;
      ctldPcb->cfg.swtch = cfg->t.cfg.s.inDLSAP.swtch;
   }
   pcb->cfg.tCbCfg = cfg->t.cfg.s.inDLSAP.tCbCfg;
   pcb->cfg.tCbId = cfg->t.cfg.s.inDLSAP.tCbId;

   for (i = 0; i < ADRLEN; i++)
      pcb->cfg.wcMask[i] = 0;

#ifdef ISDN_ROUTE
   if ((pcb->cfg.wcRout = cfg->t.cfg.s.inDLSAP.wcRout) != FALSE)
      inAscMaskToBcd(&cfg->t.cfg.s.inDLSAP.wcMask[0], 
                     &pcb->cfg.wcMask[0]);
   else
      for (i = 0; i < ADRLEN; i++)
         pcb->cfg.wcMask[i] = 0xff;

   if ((pcb->cfg.adrPref = cfg->t.cfg.s.inDLSAP.adrPref) != FALSE)
   {
      pcb->cfg.nmbPrefDig = cfg->t.cfg.s.inDLSAP.nmbPrefDig;
      for (i = 0; i < MAXPREFDIG; i++)
         pcb->cfg.prefix[i] = cfg->t.cfg.s.inDLSAP.prefix[i];
      inAscAdrToBcd(&cfg->t.cfg.s.inDLSAP.prefix[0], 
                    &pcb->cfg.prefix[0]);
   }
   else
#endif /*ISDN_ROUTE*/
   {
      pcb->cfg.nmbPrefDig = 0;
      for (i = 0; i < MAXPREFDIG; i++)
         pcb->cfg.prefix[i] = 0;
   }
  
   if ((pcb->cfg.callId.len = cfg->t.cfg.s.inDLSAP.callId.len) != 0)
      for (i = 0; i < MF_SIZE_TKNSTRS; i++)
         pcb->cfg.callId.val[i] = cfg->t.cfg.s.inDLSAP.callId.val[i];
   else
      for (i = 0; i < MF_SIZE_TKNSTRS; i++)
         pcb->cfg.callId.val[i] = 0;
  
   for (i = 0; i < ADRLEN; i++)
      pcb->cfg.sid.strg[i] = 0;

   pcb->cfg.sidIns = cfg->t.cfg.s.inDLSAP.sidIns;
   pcb->cfg.sidTon = cfg->t.cfg.s.inDLSAP.sidTon;
   pcb->cfg.sidNPlan = cfg->t.cfg.s.inDLSAP.sidNPlan;
   pcb->cfg.sidPresInd = cfg->t.cfg.s.inDLSAP.sidPresInd;
   pcb->cfg.sid.length = cfg->t.cfg.s.inDLSAP.sid.length;

   for (i = 0; i < ADRLEN; i++)
      pcb->cfg.sid.strg[i] = cfg->t.cfg.s.inDLSAP.sid.strg[i];

   pcb->cfg.cndSubsc = cfg->t.cfg.s.inDLSAP.cndSubsc;
   pcb->cfg.redirSubsc = cfg->t.cfg.s.inDLSAP.redirSubsc;
   pcb->cfg.forwSubsc = cfg->t.cfg.s.inDLSAP.forwSubsc;
   if (pcb->cfg.redirSubsc || pcb->cfg.forwSubsc)
   {
      pcb->cfg.redirAdr.eh.pres = PRSNT_NODEF;
      pcb->cfg.redirAdr.nmbPlanId.pres = PRSNT_NODEF;
      pcb->cfg.redirAdr.nmbPlanId.val = 
         cfg->t.cfg.s.inDLSAP.redirAdr.nmbPlanId.val;
      pcb->cfg.redirAdr.typeNmb.pres = PRSNT_NODEF;
      pcb->cfg.redirAdr.typeNmb.val = 
         cfg->t.cfg.s.inDLSAP.redirAdr.typeNmb.val;
      pcb->cfg.redirAdr.screenInd.pres = PRSNT_NODEF;
      pcb->cfg.redirAdr.screenInd.val = 
         cfg->t.cfg.s.inDLSAP.redirAdr.screenInd.val;
      pcb->cfg.redirAdr.presInd.pres = PRSNT_NODEF;
      pcb->cfg.redirAdr.presInd.val = 
         cfg->t.cfg.s.inDLSAP.redirAdr.presInd.val;
      pcb->cfg.redirAdr.rsnRedirect.pres = NOTPRSNT;
      pcb->cfg.redirAdr.rsnRedirect.val = 0;
      pcb->cfg.redirAdr.nmbDigits.pres = PRSNT_NODEF;
      pcb->cfg.redirAdr.nmbDigits.len = 
         cfg->t.cfg.s.inDLSAP.redirAdr.nmbDigits.len;
      for (i = 0; (U8) i < pcb->cfg.redirAdr.nmbDigits.len; i++)
         pcb->cfg.redirAdr.nmbDigits.val[i] = 
            cfg->t.cfg.s.inDLSAP.redirAdr.nmbDigits.val[i];
   }

   if ((pcb->cfg.srvOpt = cfg->t.cfg.s.inDLSAP.srvOpt) != FALSE)
   {
      if ((cfg->t.cfg.s.inDLSAP.intType != NETWORK) &&
          (pcb->cfg.swtch != SW_NTDMS100P) &&
          (pcb->cfg.swtch != SW_NTDMS250) &&
          (pcb->cfg.swtch != SW_ATT4E) && 
          (pcb->cfg.swtch != SW_ATT5EP) &&
          (pcb->cfg.swtch != SW_NTMCI) &&
          (pcb->cfg.swtch != SW_NTNI) &&
          (pcb->cfg.swtch != SW_QSIG))
      {
#if (ERRCLASS & ERRCLS_INT_PAR)
         INLOGERROR(ERRCLS_INT_PAR, EIN695, (ErrVal)cfg->t.cfg.s.inDLSAP.srvOpt,
                    "inPartialRecfgDLSAP() failed, invalid option.");
#endif
         RETVALUE(LCM_REASON_INVALID_PAR_VAL);
      }
   }

   pcb->cfg.rstOpt = cfg->t.cfg.s.inDLSAP.rstOpt;


  /* TELICA-MCAO-PRJ-NI2-SRV_MSG-- begin of code change */
   pcb->cfg.bcasOpt = cfg->t.cfg.s.inDLSAP.bcasOpt; 
   pcb->cfg.mimOpt = cfg->t.cfg.s.inDLSAP.mimOpt;
   pcb->cfg.ringOff = cfg->t.cfg.s.inDLSAP.ringOff;

   /* 2111 overlap */
   pcb->cfg.overlap = cfg->t.cfg.s.inDLSAP.overlap;
   pcb->cfg.minDigits = cfg->t.cfg.s.inDLSAP.minDigits;
   pcb->cfg.maxDigits = cfg->t.cfg.s.inDLSAP.maxDigits;

   /* FID:15070 */
   pcb->cfg.numPL = NONENP;
   /* FID 17412.0  */
   pcb->cfg.connectDateTimeIE = IN_CONNECT_WO_DATE_TIME; 
   /* FID 17524.0 */
   pcb->cfg.channelId = FALSE;
   /* FID 17555.0 Kavon */
   pcb->cfg.playDialTone = FALSE;
 
   if(cfg->t.cfg.s.inDLSAP.remark.length > 0)
   {
      if (cmStrcmp("NPPL", cfg->t.cfg.s.inDLSAP.remark.strg) == 0)
      {
         pcb->cfg.numPL = NPPL;
      }
      else if (cmStrcmp("LOCALWOSECS", cfg->t.cfg.s.inDLSAP.remark.strg) == 0)
      {
         pcb->cfg.connectDateTimeIE = IN_CONNECT_W_DATE_TIME_LOCALWOSECS;
         pcb->cfg.channelId = TRUE;
         pcb->cfg.playDialTone = TRUE;
      }
      else if (cmStrcmp("UTCWOSECS", cfg->t.cfg.s.inDLSAP.remark.strg) == 0)
      {
         pcb->cfg.connectDateTimeIE = IN_CONNECT_W_DATE_TIME_UTCWOSECS;
         pcb->cfg.channelId = TRUE;
         pcb->cfg.playDialTone = TRUE;
      }
      else if (cmStrcmp("LOCAL", cfg->t.cfg.s.inDLSAP.remark.strg) == 0)
      {
         pcb->cfg.connectDateTimeIE = IN_CONNECT_W_DATE_TIME_LOCAL;
         pcb->cfg.channelId = TRUE;
         pcb->cfg.playDialTone = TRUE;
      }
      else if (cmStrcmp("UTC", cfg->t.cfg.s.inDLSAP.remark.strg) == 0)
      {
         pcb->cfg.connectDateTimeIE = IN_CONNECT_W_DATE_TIME_UTC;
         pcb->cfg.channelId = TRUE;
         pcb->cfg.playDialTone = TRUE;
      }
   }

   INDBGP(DBGMASK_MI,"%s: numPL=%d, connectDateTimeIE=%d, channelId=%d, playDialTone=%d\n", __FUNCTION__,
                      pcb->cfg.numPL,     pcb->cfg.connectDateTimeIE, pcb->cfg.channelId, pcb->cfg.playDialTone);

   /* FID 16206.0 */
   pcb->cfg.cfCfg = cfg->t.cfg.s.inDLSAP.cfCfg;
 
   /* FID 16706.0 */
   pcb->cfg.aoc = cfg->t.cfg.s.inDLSAP.aoc;

   /* FID 16779.0 */
   pcb->cfg.digitMapName = cfg->t.cfg.s.inDLSAP.digitMapName;

   /* TELICA-jwh-03/02/2001:  Don't change operational states during reconfig. */
   /* Configure Timers */
   inInitTmr(cfg, pcb);

   RETVALUE(LCM_REASON_NOT_APPL);
} /* end of inPartialRecfgDLSAP */


/*
 *
 *      Fun:   inSigPcbCfgHandler
 *
 *      Desc: 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None
 *
 *      File:  qn_bdy11.c
 *
*/

#ifdef ANSI
PUBLIC S16 inSigPcbCfgHandler
(
 InMngmt *cfg,
 InCtldPcb *ctldPcb,
 InPCB *pcb,
 Bool initCfg,
 SuId sapId
 )
#else
     PUBLIC S16 inSigPcbCfgHandler(cfg,ctldPcb, pcb, initCfg, sapId)
     InMngmt *cfg;
     InCtldPcb *ctldPcb;
     InPCB *pcb;
     Bool initCfg;
     SuId sapId;
#endif
{
   S16 ret;
   InPCB *sigPcb;
   SpId oldSpId;
   S16 i;

   TRC3(inSigPcbCfgHandler);


   if (initCfg) /* New PCB */
   {
      if ((cfg->t.cfg.s.inDLSAP.swtch >= MF_MIN_SWITCH) &&
          (cfg->t.cfg.s.inDLSAP.swtch < MF_MAX_SWITCH)  &&
          (inSwtchValid [cfg->t.cfg.s.inDLSAP.swtch] == TRUE))
      {
         pcb->cfg.swtch = cfg->t.cfg.s.inDLSAP.swtch;
      }
      else
      {
#if (ERRCLASS & ERRCLS_INT_PAR)
         INLOGERROR(ERRCLS_INT_PAR, EIN696, (ErrVal)cfg->t.cfg.s.inDLSAP.swtch,
                    "inDLSAPCfgHandler() failed, Incorrect Switch value.");
#endif
         RETVALUE(LCM_REASON_INVALID_PAR_VAL);      
      }
   }
   else /* Reconfiguration */
   {
      if((pcb->cfg.buIntPr) && (pcb->suId == pcb->cfg.bupInt))
      {
         oldSpId = pcb->spId;
      }
   }


   if ((cfg->t.cfg.s.inDLSAP.intType != USER) &&
       (cfg->t.cfg.s.inDLSAP.intType != NETWORK) &&
       (cfg->t.cfg.s.inDLSAP.intType != SYM_USER))
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      INLOGERROR(ERRCLS_INT_PAR, EIN698, (ErrVal)cfg->t.cfg.s.inDLSAP.intType,
                 "inDLSAPCfgHandler() failed, incorrect interface type.");
#endif
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

  
   if ((cfg->t.cfg.s.inDLSAP.intCfg != INTCFG_PTPT) &&
       (cfg->t.cfg.s.inDLSAP.intCfg != INTCFG_MULTI))
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      INLOGERROR(ERRCLS_INT_PAR, EIN699, (ErrVal)cfg->t.cfg.s.inDLSAP.intCfg,
                 "inDLSAPCfgHandler() failed, incorrect interface configuration.");
#endif
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }


   if (cfg->t.cfg.s.inDLSAP.nmbBearChan > (U16) inCp.nmbBearer)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      INLOGERROR(ERRCLS_INT_PAR, EIN700, 
                 (ErrVal)cfg->t.cfg.s.inDLSAP.nmbBearChan,
                 "inDLSAPCfgHandler()failed, incorrect bearer number configuration");
#endif
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }
   /* Initialize PCB parameters */
   pcb->suId = (SuId) sapId;
   pcb->rSuId = cfg->t.cfg.s.inDLSAP.rSuId;
   if ((ret = inInitPcb(pcb, cfg, initCfg)) != LCM_REASON_NOT_APPL)
      RETVALUE(ret);

  /* TELICA-mg-11/21/00: Just printf for debugging */      
   INDBGP(DBGMASK_MI,"**VAR**:Setting  DLSAP pcb->spId=%d pcb->suId=%d, pcb->rSuId=%ld, initCfg=%d, pcb->bndState= %d\n", 
      pcb->spId, pcb->suId, pcb->rSuId, initCfg, pcb->bndState);
   switch(pcb->cfg.swtch)
   {
      case SW_ATT5EP: INDBGP(DBGMASK_MI,"SwitchType=SW_ATT5EP\n");
         break;
      case SW_NI2: INDBGP(DBGMASK_MI,"SwitchType=SW_NI2\n");
         break;
      case SW_ETSI: INDBGP(DBGMASK_MI,"SwitchType=SW_ETSI\n");
         break;
      case SW_NTDMS100P: INDBGP(DBGMASK_MI,"SwitchType=SW_NTDMS100P\n");
         break;
      default:
         DP("SwitchType=%d (NOT SUPPORTED)\n",pcb->cfg.swtch );
   }

   if (initCfg) /* New PCB */
   {
      /* TELICA-Bug#16149, rkhan, suId is copied athe top and also
       * it is not recfg so put it in a queue */
      addIsdnIfCbQ(pcb);

      pcb->dChan.numCon = 0;
      for (i = 0; i < MAXDCHANCON; i++)
         pcb->dChan.ntc[i] = NULLP;  
   }

   /* Init timer structure */
   cmInitTimers(pcb->timers, MAXBCHTIMER);

   /* Configure Timers */
   inInitTmr(cfg, pcb);

   oldSigInt = INT_INV;
   oldBkupInt = INT_INV;

   if ((cfg->t.cfg.s.inDLSAP.nfasInt) && (!initCfg))
   {
      if ((ret=inFindOldInt(cfg,initCfg, pcb->suId)) != LCM_REASON_NOT_APPL)
         RETVALUE(ret);
   }
    
   if ((ret = inPrcSigPcbCfg(cfg, pcb, initCfg)) != LCM_REASON_NOT_APPL)
      RETVALUE(ret);

   ctldPcb->suId = sapId;
   ctldPcb->rSuId = cfg->t.cfg.s.inDLSAP.rSuId;

   if ((ret = inCtldPcbCfgHandler(cfg, ctldPcb, initCfg)) != 
       LCM_REASON_NOT_APPL)
      RETVALUE(ret);
 

    
  /*  For a NFAS Interface, if the pcb being configured is the backup
      interface and the original backup interface was a different one
      or if the backup is being removed,then the oldBkupInt needs to
      be unbound */
  
   if (!initCfg)
   {
      if ((pcb->cfg.nfasInt) && 
          ((pcb->suId == (S16) pcb->cfg.bupInt) &&
           (pcb->cfg.buIntPr)) && ((oldBkupInt != pcb->suId) &&
                                   (oldBkupInt!= INT_INV)))
      {
         if ((sigPcb =*(pcbLstPtr + oldBkupInt)) != NULLP)
         {
            /* Unbind the link */
            InLiDatUbndReq(&sigPcb->pst, sigPcb->spId, Q_930, 0);
            /* TELICA-MCAO-01/29/02: bug 8009 need update bndState */
            sigPcb->bndState = IN_UNBND;
            zqRunTimeUpd(ZQ_PSAP_CB, CMPFTHA_UPD_REQ, (PTR)sigPcb);
            zqUpdPeer();
            *(pcbLstPtr + oldBkupInt) = NULLP;
         }
      }
   }
 
   RETVALUE(LCM_REASON_NOT_APPL);
} /* end of inSigPcbCfgHandler */



/*
 *
 *      Fun:   inResetStsCntr
 *
 *      Desc: 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None
 *
 *      File:  qn_bdy11.c
 *
*/

#ifdef ANSI
PUBLIC S16 inResetStsCntr
(
 InPCB *pcb
 )
#else
     PUBLIC S16 inResetStsCntr(pcb)
     InPCB *pcb;
#endif
{
   if (!pcb)
   {
      DP("[PLM_IN]: pcb is not configured yet!\n");
      RETVALUE(ROK);
   }
  
  pcb->sts.alertingTx = NULLD;
  pcb->sts.callProcTx = NULLD;
  pcb->sts.connTx = NULLD;
  pcb->sts.connAckTx = NULLD;
  pcb->sts.progressTx = NULLD;
  pcb->sts.setupTx = NULLD;
  pcb->sts.setupAckTx = NULLD;
  pcb->sts.rsmTx = NULLD;
  pcb->sts.rsmAckTx = NULLD;
  pcb->sts.rsmRejTx = NULLD;
  pcb->sts.suspTx = NULLD;
  pcb->sts.suspAckTx = NULLD;
  pcb->sts.suspRejTx = NULLD;
  pcb->sts.usrInfoTx = NULLD;
  pcb->sts.holdTx = NULLD;
  pcb->sts.holdAckTx = NULLD;
  pcb->sts.holdRejTx = NULLD;
  pcb->sts.retTx = NULLD;
  pcb->sts.retAckTx = NULLD;
  pcb->sts.retRejTx = NULLD;
  pcb->sts.discTx = NULLD;
  pcb->sts.relTx = NULLD;
  pcb->sts.relCmpltTx = NULLD;
  pcb->sts.rstTx = NULLD;
  pcb->sts.rstAckTx = NULLD;
  pcb->sts.segmentTx = NULLD;
  pcb->sts.congCntrlTx = NULLD;
  pcb->sts.infoTx = NULLD;
  pcb->sts.notifyTx = NULLD;
  pcb->sts.statTx = NULLD;
  pcb->sts.statEnqTx = NULLD;
  pcb->sts.servTx = NULLD;
  pcb->sts.servAckTx = NULLD;
  pcb->sts.facilTx = NULLD;
  pcb->sts.facilAckTx = NULLD;
  pcb->sts.facilRejTx = NULLD;
  pcb->sts.regTx = NULLD;
  
  pcb->sts.alertingRx = NULLD;
  pcb->sts.callProcRx = NULLD;
  pcb->sts.connRx = NULLD;
  pcb->sts.connAckRx = NULLD;
  pcb->sts.progressRx = NULLD;
  pcb->sts.setupRx = NULLD;
  pcb->sts.setupAckRx = NULLD;
  pcb->sts.rsmRx = NULLD;
  pcb->sts.rsmAckRx = NULLD;
  pcb->sts.rsmRejRx = NULLD;
  pcb->sts.suspRx = NULLD;
  pcb->sts.suspAckRx = NULLD;
  pcb->sts.suspRejRx = NULLD;
  pcb->sts.usrInfoRx = NULLD;
  pcb->sts.holdRx = NULLD;
  pcb->sts.holdAckRx = NULLD;
  pcb->sts.holdRejRx = NULLD;
  pcb->sts.retRx = NULLD;
  pcb->sts.retAckRx = NULLD;
  pcb->sts.retRejRx = NULLD;
  pcb->sts.discRx = NULLD;
  pcb->sts.relRx = NULLD;
  pcb->sts.relCmpltRx = NULLD;
  pcb->sts.rstRx = NULLD;
  pcb->sts.rstAckRx = NULLD;
  pcb->sts.segmentRx = NULLD;
  pcb->sts.congCntrlRx = NULLD;
  pcb->sts.infoRx = NULLD;
  pcb->sts.notifyRx = NULLD;
  pcb->sts.statRx = NULLD;
  pcb->sts.statEnqRx = NULLD;
  pcb->sts.servRx = NULLD;
  pcb->sts.servAckRx = NULLD;
  pcb->sts.facilRx = NULLD;
  pcb->sts.regRx = NULLD;
  pcb->sts.facilAckRx = NULLD;
  pcb->sts.facilRejRx = NULLD;
  
  pcb->sts.cons = NULLD;
  pcb->sts.discs = NULLD;
  pcb->sts.aveCalls = 0;

  pcb->sts.lstTxCause = NULLD;
  pcb->sts.lstTxDgn = NULLD;
  pcb->sts.lstRxCause = NULLD;
  pcb->sts.lstRxDgn = NULLD;

  /* TELICA - Bugzilla-4321 - Bukucu - 10.25.01 - */
  /* bug#18354, rkhan, changed to SGetSysUpTime */
  /* Bukucu Bug-19182 changed to SGetSysTime again */
  SGetSysTime(&pcb->sts.lstInitTx);
  SGetSysTime(&pcb->sts.lstInitRx);
  SGetSysTime(&pcb->sts.lstInitNA);
  pcb->sts.updThrshldCurval = NULLD;
  pcb->sts.updThrshld = ISDNPM_DFLT_UPDTHRSHLD_VAL;

  RETVALUE(ROK);
}/* end of inResetStsCntr */




/*
 *
 *      Fun:   inInitCb
 *
 *      Desc: 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None
 *
 *      File:  qn_bdy11.c
 *
*/

#ifdef ANSI
PUBLIC S16 inInitCb
(
 InCb *cb,
 InPCB *pcb
 )
#else
     PUBLIC S16 inInitCb(cb, pcb)
     InCb *cb;
     InPCB *pcb;
#endif
{
   S16 ret;
#ifdef ISDN_PTMPT
   S16 index;
#endif

   TRC3(inInitCb);
  
   cb->suId = (SuId) pcb->suId;
   cb->rSuId = pcb->rSuId;
   cb->spId = (SpId) pcb->spId;
   cb->sapType = DLSAP;
   cb->state = CBST3DOWN;
   cb->mBuf = NULLP;
   cb->msg = NULLP;
   cb->facMBuf = NULLP;
   cb->flc = FALSE;
   cb->rstCnt = 0;
   cb->inUnBndFlg = FALSE;
   if (pcb->cfg.intType == NETWORK)
      cb->intType = NETWORK;
   else
      cb->intType = USER;
   cb->numCalId = 0;
   cb->numCalRef = 0;
   cb->callIdTbl = NULLP;
#ifdef ISDN_SEGMENT
   cb->segCallRefVal = 0;
   cb->numSegs = 0;
   cb->remSegs = 0;
   cb->segMsg = NULLP;
   cb->segFlg = FALSE;
#endif
   /* TELICA-mg-01/08/2001: Surprise! Trildos did not initialize a pointer */
   cb->facMBuf = NULLP;
   cb->rstCfmSnt = FALSE;
   cb->trc = FALSE;
   /* Start in OOS state */
   if (pcb->cfg.nfasInt)
   {
      /* For NTNA and NTNI variants start with ST_MOOS as the 
       * initial state */
      if ((pcb->cfg.swtch == SW_NTNI) ||
          (pcb->cfg.swtch == SW_NTDMS250) ||
          (pcb->cfg.swtch == SW_NTDMS100P) ||
          (pcb->cfg.swtch == SW_NTMCI))
      {
         cb->sigState = ST_MOOS;
      }
      else
         cb->sigState = ST_OOS;
   }
   else
      /* TELICA-jwh-06/04/2001:  Remove LCM_EVENT_INSERVICE alarm AGAIN. */
      cb->sigState = ST_IS;

   /* bn */
   cb->usid = 0;
   cb->tid = 0;
   cb->lstRefUsd = 0;
   switch (pcb->cfg.swtch)
   {
      case SW_ATT5EB:
         if(pcb->cfg.mimOpt)
            cb->endPointState = NOT_INIT;
         else
         {
            cb->endPointState = P_INIT;
            cb->srvState = IN_SERVICE;
         }
         break;
      case SW_NTDMS100B:
         if(pcb->cfg.teiAlloc != STATIC)
            cb->endPointState = NOT_INIT;
         else
            cb->endPointState = E_INIT;
         break;
      default:
         cb->endPointState = P_INIT;
         break;
   }
  
#ifdef ISDN_ROUTE
   inAscAdrToBcd(&pcb->cfg.prefix[0],&cb->prefix[0]);
#endif
   cb->pst.selector = pcb->pst.selector;
   cb->pst.region = pcb->pst.region;
   cb->pst.pool = pcb->pst.pool;
   cb->pst.prior = pcb->pst.prior;
   cb->pst.route = pcb->pst.route;
   cb->pst.dstProcId = pcb->pst.dstProcId;
   cb->pst.dstEnt = pcb->pst.dstEnt;
   cb->pst.dstInst = pcb->pst.dstInst;
   cb->pst.srcProcId = inInit.procId;
   cb->pst.srcEnt = inInit.ent;
   cb->pst.srcInst = inInit.inst;
  
   cb->inNtcPtr = NULLP;
   cb->rel_force = FALSE;

   cb->nfas_gr = FALSE;
   cb->gr_lnkup = FALSE; /*Bug 81522*/ 
   cb->gr_relnk = FALSE; 
#ifdef ISDN_PTMPT
   for (index = 0; index < MAXCBTIMERBLOCK; index++)
   {
      cb->cesTimerCb[index].index = index;
      cb->cesTimerCb[index].ntc = (PTR)NULLP;
      cb->cesTimerCb[index].dCb = (PTR)cb;
      cb->cesTimerCb[index].timerCnt = 0;
      cmInitTimers(cb->cesTimerCb[index].timers, 
                   MAXSIMINTTIMER);
    
   }
#endif /* ISDN_PTMPT */
  
   cb->tmrCnt = 0;
  
   /* TELICA-jwh-08/07/2000:  Detect and remove stranded timers. */
   {
      U8 tmrNum;
      for (tmrNum = 0; tmrNum < MAXSIMTIMER; tmrNum++)
      {
         if (cb->timers[tmrNum].tmrEvnt != TMR_NONE)
         {
            INDBGP(DBGMASK_MI,"Clearing stranded ISDN timer.  rSuId=%ld, spid=%d, tmrEvnt=%d\n",
cb->rSuId, cb->spId, cb->timers[tmrNum].tmrEvnt);
            inRmvCbTq(cb, tmrNum);
         }
      }
   }

   /* initialize message control */  
   MFINITPROF(&cb->mfCfgProf, ret, NMB_ALLMSG, NMB_PRIM, 
              allPduDefs, allSduDefs,  MF_MAX_REPELMT, 
              (MF_IGNORE | MF_ORDERED), NULLP);
  
   MFINITMSGCTL(&cb->mfMsgCtl, ret, &cb->mfCfgProf, 0);
   RETVALUE(ROK);
} /* end of inInitCb */



/*
 *
 *      Fun:   inPrcSigPcbCfg
 *
 *      Desc: 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None
 *
 *      File:  qn_bdy11.c
 *
*/

#ifdef ANSI
PUBLIC S16 inPrcSigPcbCfg
(
 InMngmt *cfg,
 InPCB *pcb,
 Bool initCfg
 )
#else
     PUBLIC S16 inPrcSigPcbCfg(cfg, pcb, initCfg)
     InMngmt *cfg;
     InPCB *pcb;
     Bool initCfg;
#endif
{
   S16 i;
   S16 ret;
   InCb *cb;

   TRC3(inPrcSigPcbCfg);

   if (((initCfg) && 
        (oldSigInt == INT_INV)) ||
       ((pcb->suId == (S16) pcb->cfg.bupInt) &&
        (oldBkupInt == INT_INV) &&
        (pcb->cfg.buIntPr)))
   {
      if ((ret = SGetSBuf(inInit.region, inInit.pool, 
                          (Data **) &pcb->cesLst,
                          (Size) (cfg->t.cfg.s.inDLSAP.nmbCes * 
                                  sizeof(InCb *)))) != ROK)
      {
#if (ERRCLASS & ERRCLS_DEBUG)
         INLOGERROR(ERRCLS_DEBUG, EIN702, (ErrVal) sizeof(InCb *),
                    "InMiLinCfgReq() failed, unable to allocate memory.");
#endif
         RETVALUE(LCM_REASON_MEM_NOAVAIL);
      }
   }
 
   for (i = 0; i < pcb->cfg.nmbCes; i++)
   {
      if (((initCfg) && (oldSigInt == INT_INV)) || 
          ((pcb->suId == (S16) pcb->cfg.bupInt)&&
           (oldBkupInt == INT_INV) && (pcb->cfg.buIntPr)))
      {
         if ((ret= SGetSBuf(inInit.region, inInit.pool, (Data **) &cb,
                            (Size) sizeof(InCb))) != ROK)
         {
#if (ERRCLASS & ERRCLS_DEBUG)
            INLOGERROR(ERRCLS_DEBUG, EIN703, (ErrVal) sizeof(InCb *),
                       "InMiLinCfgReq() failed,unable to allocate memory.");
#endif
            RETVALUE(LCM_REASON_MEM_NOAVAIL);
         }
         /* bug 18804: initiate memory of control block */
         cmMemset((U8 *)cb, 0, sizeof(InCb));

         *(pcb->cesLst + i) = cb;

         /* TELICA-jwh-08/07/2000:  Initialize timers here so that stranded timers can be detected and removed later. */
         cmInitTimers(cb->timers, MAXSIMTIMER);
 
      }
      else
      {
         if ((!pcb->cfg.nfasInt) ||
             (((oldSigInt != pcb->cfg.sigInt) &&
               (pcb->cfg.sigInt == pcb->suId)) ||
              (((oldBkupInt != pcb->cfg.bupInt) &&
                (pcb->cfg.bupInt == pcb->suId)) && 
               (oldBkupInt != INT_INV))))
         {
            cb = *(pcb->cesLst + i);
         }
         else
         {
            break;
         }
      }
    
      cb->ces = (Ces) i;
      inInitCb(cb, pcb);

   } /* end for */

   if (pcb->cfg.nfasInt)
   {
      Bool isSigLnk = FALSE;
      /* need update links ctldInt list, in all case */
      if (pcb->cfg.sigInt == pcb->suId)
         isSigLnk = TRUE;
      else if ((pcb->cfg.buIntPr) && (pcb->suId == (S16) pcb->cfg.bupInt))
         isSigLnk = FALSE;
      else 
      {
#if (ERRCLASS & ERRCLS_INT_PAR)
         INLOGERROR(ERRCLS_INT_PAR, EIN705, 
                    (ErrVal)pcb->cfg.rSigInt,
                    "inPrcCtldPcbCfg failed, invalid combination of singaling suId");
         RETVALUE(LCM_REASON_INVALID_SAP);
#endif
      }

      if ((initCfg) || (pcb->ctldInt == NULLP))
      {
         if ((ret = SGetSBuf(inInit.region, inInit.pool, 
                             (Data **) &pcb->ctldInt,
                             (Size) (pcb->cfg.maxNmbNfasInt * 
                                     sizeof(U16)))) != ROK)
         {
#if (ERRCLASS & ERRCLS_DEBUG)
            INLOGERROR(ERRCLS_DEBUG, EIN704, (ErrVal) sizeof(U16),
                       "InMiLinCfgReq() failed, unable to allocate memory.");
#endif
            RETVALUE(LCM_REASON_MEM_NOAVAIL);
         }

         for (i = 0; i < (S16) pcb->cfg.maxNmbNfasInt; i++)
         {
            *(pcb->ctldInt + i) = INT_NOT_CFGD;
         }
         pcb->nmbCtldInt = 0;
 
         /* PLM_IN enable it */
         for (i = 0; i < (S16) cfg->t.cfg.s.inDLSAP.nmbNfasInt; i++)
         {
            /* TELICA-jwh-01/31/2001:  Only load the valid ctldInts */
            if (cfg->t.cfg.s.inDLSAP.ctldInt[i] != (S16)INT_NOT_CFGD)
            {
               DP_PLMIN(DBGMASK_L1, "pcb->rSuId= %ld, ctldInt[%d]= %d\n", pcb->rSuId, i, cfg->t.cfg.s.inDLSAP.ctldInt[i]);
               *(pcb->ctldInt + i) = cfg->t.cfg.s.inDLSAP.ctldInt[i];

               /* need update links ctldInt list, in all case */
               if (isSigLnk)
                  inAssociateCtldPcb(cfg->t.cfg.s.inDLSAP.ctldInt[i], pcb->suId, pcb->rSuId, 0, 0);
               else 
                  inAssociateCtldPcb(cfg->t.cfg.s.inDLSAP.ctldInt[i], 0, 0, pcb->suId, pcb->rSuId);
               pcb->nmbCtldInt ++;
            }
         }
      }
   }
   else
   {
      pcb->ctldInt = NULLP;
      pcb->nmbCtldInt = 0;
   }
    
   /* initialize statistics counters */
   inResetStsCntr(pcb);

   RETVALUE(LCM_REASON_NOT_APPL);
} /* end of inPrcSigPcbCfg */





/*
 *
 *      Fun:   inPrcCtldPcbCfg
 *
 *      Desc: 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None
 *
 *      File:  qn_bdy11.c
 *
*/

#ifdef ANSI
PUBLIC S16 inPrcCtldPcbCfg
(
 InCtldPcb *ctldPcb
 )
#else
     PUBLIC S16 inPrcCtldPcbCfg(ctldPcb)
     InCtldPcb *ctldPcb;
#endif
{
   InPCB *sigPcb;
   InPCB *tmpPcb;
   InPCB *bkupPcb=NULLP;
   S16 i;
   S16 j;
   Bool found;
   InCb *cb;
   S16 ret;
  

   TRC3(inPrcCtldPcbCfg);
  
   if (ctldPcb->cfg.sigInt == 0)
   {
      DP("signaling link of rSuId %ld not configured yet \n", ctldPcb->rSuId); 
      RETVALUE(LCM_REASON_NOT_APPL);
   }
   if ((sigPcb = *(pcbLstPtr + ctldPcb->cfg.sigInt)) == NULLP)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      INLOGERROR(ERRCLS_INT_PAR, EIN705, 
                 (ErrVal)ctldPcb->cfg.rSigInt,
                 "inPrcCtldPcbCfg failed,Unable to Access Signalling Interface");
      RETVALUE(LCM_REASON_INVALID_SAP);
#endif
   }
  
 
   if (ctldPcb->suId != ctldPcb->cfg.sigInt)
   {
      found = FALSE;
      for (i= 0; i < sigPcb->cfg.maxNmbNfasInt; i++)
      {
         if (*(sigPcb->ctldInt+i) == ctldPcb->suId)
         {
            found = TRUE;
            break;
         }
      }
    
      if (!found)
      {
         if ( sigPcb->nmbCtldInt < sigPcb->cfg.maxNmbNfasInt)
         {
            for (i = 0; i < (S16) sigPcb->cfg.maxNmbNfasInt; i++)
            {
               if ( *(sigPcb->ctldInt + i) == INT_NOT_CFGD)
               {
                  *(sigPcb->ctldInt + i) = ctldPcb->suId;
                  sigPcb->nmbCtldInt ++;
                  break;
               }
            }
            if (i ==  (S16) sigPcb->cfg.maxNmbNfasInt)
            {
#if (ERRCLASS & ERRCLS_INT_PAR)
               INLOGERROR(ERRCLS_INT_PAR, EINXXX, 
                          (U32)ctldPcb->rSuId,
                          "inPrcCtldPcbCfg failed,Unable to Configure CtldPcb");
               RETVALUE(LCM_REASON_INVALID_SAP);
#endif
            }
         }
         else
         {
#if (ERRCLASS & ERRCLS_INT_PAR)
            INLOGERROR(ERRCLS_INT_PAR, EINXXX, 
                       (U32)ctldPcb->rSuId,
                       "inPrcCtldPcbCfg failed,Unable to Configure CtldPcb");
            RETVALUE(LCM_REASON_INVALID_SAP);
#endif
         }
      }
   }

   for (i = 0; i < (S16)sigPcb ->cfg.maxNmbNfasInt; i++)
   {
      if ( *(sigPcb->ctldInt + i) != INT_NOT_CFGD)
      {
         DP_PLMIN(DBGMASK_L1, "rSuId = %ld,  sigPcb->ctldInt[%d] = %d;\n",
                  sigPcb->rSuId, i,sigPcb->ctldInt[i]);
      }
   }

   if ((sigPcb->cfg.buIntPr) && (sigPcb->cfg.sigInt != ctldPcb->suId))
   {
      bkupPcb = *(pcbLstPtr + sigPcb->cfg.bupInt);

      if (bkupPcb == NULLP)
      {
#if (ERRCLASS & ERRCLS_INT_PAR)
         INLOGERROR(ERRCLS_INT_PAR, EIN888, (ErrVal)sigPcb->cfg.rBupInt,
                    "inPrcCtldPcbCfg failed,Unable to Access bkup Interface");
         RETVALUE(LCM_REASON_INVALID_SAP);
#endif
      }
  
      if (sigPcb->cfg.bupInt == ctldPcb->suId)
      {
#if 0 /* bug#26711 */
         *(bkupPcb->ctldInt + bkupPcb->nmbCtldInt) = sigPcb->suId;
         bkupPcb->nmbCtldInt ++;
#endif
         for (i= 0; i < sigPcb->cfg.maxNmbNfasInt; i++)
         {
            found = FALSE;

            for (j= 0; j < bkupPcb->cfg.maxNmbNfasInt; j++)
            {
               if (*(bkupPcb->ctldInt + j) == *(sigPcb->ctldInt + i))
               {
                  found = TRUE;
                  break;
               }
            }
    
            if (!found)
            {
               if ( bkupPcb->nmbCtldInt < bkupPcb->cfg.maxNmbNfasInt)
               {
                  for (j = 0; j < (S16) bkupPcb->cfg.maxNmbNfasInt; j++)
                  {
                     if ( *(bkupPcb->ctldInt + j) == INT_NOT_CFGD)
                     {
                        *(bkupPcb->ctldInt + j) = *(sigPcb->ctldInt + i);
                        bkupPcb->nmbCtldInt ++;
                        break;
                     }
                  }
                  if (j ==  (S16) bkupPcb->cfg.maxNmbNfasInt)
                  {
#if (ERRCLASS & ERRCLS_INT_PAR)
                     INLOGERROR(ERRCLS_INT_PAR, EINXXX, 
                                (ErrVal)ctldPcb->rSuId,
                                "inPrcCtldPcbCfg failed,Unable to Configure CtldPcb");
                     RETVALUE(LCM_REASON_INVALID_SAP);
#endif
                  }
               }
               else
               {
#if (ERRCLASS & ERRCLS_INT_PAR)
                  INLOGERROR(ERRCLS_INT_PAR, EINXXX, 
                             (ErrVal)ctldPcb->rSuId,
                             "inPrcCtldPcbCfg failed,Unable to Configure CtldPcb");
                  RETVALUE(LCM_REASON_INVALID_SAP);
#endif
               }
            }
         }
      }

      if (ctldPcb->suId != bkupPcb->suId)
      {
         found = FALSE;
         for (i= 0; i < bkupPcb->cfg.maxNmbNfasInt; i++)
         {
            if (*(bkupPcb->ctldInt+i) == ctldPcb->suId)
            {
               found = TRUE;
               break;
            }
         }

         if (!found)
         {
            if ( bkupPcb->nmbCtldInt < bkupPcb->cfg.maxNmbNfasInt)
            {
               for (j = 0; j < (S16) bkupPcb->cfg.maxNmbNfasInt; j++)
               {
                  if ( *(bkupPcb->ctldInt + j) == INT_NOT_CFGD)
                  {
                     *(bkupPcb->ctldInt + j) = ctldPcb->suId;
                     bkupPcb->nmbCtldInt ++;
                     break;
                  }
               }
               if (j ==  (S16) bkupPcb->cfg.maxNmbNfasInt)
               {
#if (ERRCLASS & ERRCLS_INT_PAR)
                  INLOGERROR(ERRCLS_INT_PAR, EINXXX, 
                             (ErrVal)ctldPcb->rSuId,
                             "inPrcCtldPcbCfg failed,Unable to Configure CtldPcb");
                  RETVALUE(LCM_REASON_INVALID_SAP);
#endif
               }
            }
            else
            {
#if (ERRCLASS & ERRCLS_INT_PAR)
               INLOGERROR(ERRCLS_INT_PAR, EINXXX, 
                          (ErrVal)ctldPcb->rSuId,
                          "inPrcCtldPcbCfg failed,Unable to Configure CtldPcb");
               RETVALUE(LCM_REASON_INVALID_SAP);
#endif
            }
         }
      } /* ctldPcb->suId != bkupPcb->suId) */
   } /* bkup prsnt */

   if((sigPcb->cfg.buIntPr) && bkupPcb)
   {
      for (i = 0; i < (S16) bkupPcb->cfg.maxNmbNfasInt; i++)
      {
         if ( *(bkupPcb->ctldInt + i) != INT_NOT_CFGD)
         {
            DP_PLMIN(DBGMASK_L1, "PLM_IN: rSuId = %ld,  sigPcb->ctldInt[%d] = %d;\n",
                     bkupPcb->rSuId, i, bkupPcb->ctldInt[i]);
         }
      }
   }

   if (ctldPcb->suId == oldSigInt)
   {
      if (((tmpPcb= *(pcbLstPtr + oldSigInt)) != sigPcb) &&
          (tmpPcb != NULLP))
      {
         for (i = 0; i < tmpPcb->cfg.nmbCes; i++)
         {
            if ((cb = *(tmpPcb->cesLst + i)) != NULLP)
            {
               /* TELICA-jwh-09/06/2001:  Make sure all timers are stopped before freeing memory */
               STOP_TIMERS(cb, MAXSIMTIMER, inRmvCbTq);
               if ((ret = SPutSBuf(inInit.region, inInit.pool,
                                   cb, (Size) sizeof(InCb))) != ROK)
               {
#if (ERRCLASS & ERRCLS_ADD_RES)
                  INLOGERROR(ERRCLS_ADD_RES, EIN736, ERRZERO,
                             "inPrcCtldPcbCfg() failed, unable to release memory.");
#endif
               } 
            }
         }
         InLiDatUbndReq(&tmpPcb->pst, tmpPcb->spId, Q_930, 0);
         /* TELICA-MCAO-01/29/02: bug 8009 need update bndState */
         tmpPcb->bndState = IN_UNBND;
         zqRunTimeUpd(ZQ_PSAP_CB, CMPFTHA_UPD_REQ, (PTR)tmpPcb);
         zqUpdPeer();

         if ((ret = SPutSBuf(inInit.region, inInit.pool, 
                             tmpPcb->cesLst, 
                             (Size) sizeof(InCb *))) != ROK)
         {
#if (ERRCLASS & ERRCLS_ADD_RES)
            INLOGERROR(ERRCLS_ADD_RES, EIN737, ERRZERO,
                       "inPrcCtldPcbCfg() failed, unable to release memory.");
#endif
         }


         /* TELICA-jwh-01/31/2001:  Must free ctldInt[maxNmbNfasInt] when freeing pcb */
         /* Start of code change */
         if (tmpPcb->cfg.nfasInt)
         {        
            if (tmpPcb->ctldInt != NULLP)
            {
               if ((ret = SPutSBuf(inInit.region, inInit.pool, tmpPcb->ctldInt, 
                                   (Size) (tmpPcb->cfg.maxNmbNfasInt * 
                                           sizeof(U16)))) != ROK)
               {
#if (ERRCLASS & ERRCLS_ADD_RES)
                  INLOGERROR(ERRCLS_ADD_RES, EIN737, ERRZERO,
                             "inPrcCtldPcbCfg() failed, unable to release memory.");
#endif
               }
               tmpPcb->ctldInt = NULLP;
            }
         }

         /* TELICA-jwh-09/06/2001:  Make sure all timers are stopped before freeing memory */
         STOP_TIMERS(tmpPcb, MAXBCHTIMER, inRmvPcbTq);

         /* TELICA-Bug#16149, rkhan */
         remIsdnIfCbQ(tmpPcb);

         if ((ret = SPutSBuf(inInit.region, inInit.pool, tmpPcb, 
                             sizeof(InPCB))) != ROK)
         {
#if (ERRCLASS & ERRCLS_ADD_RES)
            INLOGERROR(ERRCLS_ADD_RES, EIN738, ERRZERO,
                       "inPrcCtldPcbCfg() failed, unable to release memory.");
        
#endif
            RETVALUE(RFAILED);
         }
         *(pcbLstPtr + oldSigInt) = NULLP;
      }
   }


   if ((sigPcb->cfg.nfasInt) && !(sigPcb->cfg.buIntPr) &&
       (oldBkupInt == ctldPcb->suId))
   {
      if ((tmpPcb =*(pcbLstPtr + oldBkupInt)) != NULLP)
      {      
         InLiDatUbndReq(&sigPcb->pst,tmpPcb->spId, Q_930, 0);
         /* TELICA-MCAO-01/29/02: bug 8009 need update bndState */
         tmpPcb->bndState = IN_UNBND;
         zqRunTimeUpd(ZQ_PSAP_CB, CMPFTHA_UPD_REQ, (PTR)tmpPcb);
         zqUpdPeer();
    
         *(pcbLstPtr + oldBkupInt) = NULLP;
      }
   }

   if (sigPcb->cfg.teiAlloc != STATIC)
      ctldPcb->state = CBST3DOWN;

   RETVALUE(LCM_REASON_NOT_APPL);
} /* end of inPrcCtldPcbCfg */





/*
*
*      Fun:   inCesCfg
*
*      Desc: 
*
*      Ret:   ROK   - ok
*
*      Notes: None
*
*      File:  qn_bdy11.c
*
*/

#ifdef ANSI
PUBLIC S16 inCesCfg
(
 InMngmt *cfg,
 InPCB *pcb,
 InCb *cb
 )
#else
     PUBLIC S16 inCesCfg(cfg, pcb, cb)
     InMngmt *cfg;
     InPCB *pcb;
     InCb *cb;
#endif
{
   S16 i;

   TRC3(inCesCfg);

   cb->cbCfg.lnkUpDwnInd = cfg->t.cfg.s.inLCe.lnkUpDwnInd;
   cb->cbCfg.tCon.enb = cfg->t.cfg.s.inLCe.tCon.enb;
   cb->cbCfg.tCon.val = cfg->t.cfg.s.inLCe.tCon.val;
   /* TELICA-BUG:21878-mqin */
   if (!cb->cbCfg.tCon.val)
      cb->cbCfg.tCon.val = 1;
   cb->cbCfg.tDisc.enb = cfg->t.cfg.s.inLCe.tDisc.enb;
   cb->cbCfg.tDisc.val = cfg->t.cfg.s.inLCe.tDisc.val;
   cb->cbCfg.t314.enb = cfg->t.cfg.s.inLCe.t314.enb;
   cb->cbCfg.t314.val = cfg->t.cfg.s.inLCe.t314.val;
   cb->cbCfg.t332i.enb = cfg->t.cfg.s.inLCe.t332i.enb;
   cb->cbCfg.t332i.val = cfg->t.cfg.s.inLCe.t332i.val;
   cb->cbCfg.tRstAck.enb = cfg->t.cfg.s.inLCe.tRstAck.enb;
   cb->cbCfg.tRstAck.val = cfg->t.cfg.s.inLCe.tRstAck.val;


   if((pcb->cfg.swtch == SW_NI1) ||
      (pcb->cfg.swtch == SW_ATT5EB) || 
      (pcb->cfg.swtch == SW_NTDMS100B))
   {
      cb->cbCfg.tSpid.enb = cfg->t.cfg.s.inLCe.tSpid.enb;
      cb->cbCfg.tSpid.val = cfg->t.cfg.s.inLCe.tSpid.val;
      for (i = 0; i < ADRLEN; i++)
         cb->cbCfg.spid.val[i] = '0';
   }

   cb->cbCfg.spid.pres = cfg->t.cfg.s.inLCe.spid.pres;
  
   if (cb->cbCfg.spid.pres)
   {
      cb->cbCfg.spid.len = cfg->t.cfg.s.inLCe.spid.len;
      for (i = 0; (U8) i < cb->cbCfg.spid.len; i++)
         cb->cbCfg.spid.val[i] = cfg->t.cfg.s.inLCe.spid.val[i];
   }
   else
      cb->cbCfg.spid.len = 0;
  
   if ((cb->cbCfg.usid = cfg->t.cfg.s.inLCe.usid) != 0)
      if (cb->intType == USER)
      {
#if (ERRCLASS & ERRCLS_INT_PAR)
         INLOGERROR(ERRCLS_INT_PAR, EIN706, (ErrVal) cb->intType,
                    "InMiLinCfgReq() failed, invalid configuration.");
#endif
         RETVALUE(LCM_REASON_INVALID_PAR_VAL);
      }


   if ((cb->cbCfg.tid = cfg->t.cfg.s.inLCe.tid) != 0)
      if (cb->intType == USER)
      {
#if (ERRCLASS & ERRCLS_INT_PAR)
         INLOGERROR(ERRCLS_INT_PAR, EIN707, (ErrVal) cb->intType,
                    "InMiLinCfgReq() failed, invalid configuration.");
#endif
         RETVALUE(LCM_REASON_INVALID_PAR_VAL);
      }

#ifdef ISDN_BRI
   if((pcb->cfg.swtch == SW_ATT5EB))
   {
      /* transmit Initialization Request Message to the Network */
      if (cb->state == CBST1XFER)
      {
         cb->rstCnt = 0;
         inGenMimInitReq(cb);
      }
   }
#endif

   RETVALUE(LCM_REASON_NOT_APPL);
}/* end of inCesCfg */




/***********************************************************************/
/*                Control Request Procedures                           */
/***********************************************************************/




/*
*
*      Fun:   inGetPcb
*
*      Desc: 
*
*      Ret:   ROK   - ok
*
*      Notes: None
*
*      File:  qn_bdy11.c
*
*/

#ifdef ANSI
PUBLIC S16 inGetPcb
(
 SuId suId,
 InCtldPcb **ctldPcb,
 InPCB **pcb
 )
#else
     PUBLIC S16 inGetPcb(suId, ctldPcb, pcb)
     SuId suId;
     InCtldPcb **ctldPcb;
     InPCB **pcb;
#endif
{

   TRC3(inGetPcb);

   /* initialize the defaults */
   *pcb = NULLP;
   *ctldPcb = NULLP;
   
  /* TELICA-MCAO-02/25/02: bug 9328: check sapId range */
   if ((suId < 0) || (suId >= inCp.nmbLnks))
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      INLOGERROR(ERRCLS_INT_PAR, EIN708, (ErrVal)suId,
                 "inGetPcb() failed, unable to access DLSAP.");
#endif
      RETVALUE(LCM_REASON_INVALID_SAP);    
   } 

   if (((*ctldPcb) = *(ctldPcbLstPtr + suId)) == NULLP)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      INLOGERROR(ERRCLS_INT_PAR, EIN708, (ErrVal)suId ,
                 "inGetPcb failed,unable to access DLSAP.");
#endif
      RETVALUE(LCM_REASON_INVALID_SAP);    
   }

   if ((*ctldPcb)->cfg.nfasInt)
   {
      if((*ctldPcb)->cfg.sigInt == 0)  
         RETVALUE(LCM_REASON_INVALID_SAP);

      *pcb = *(pcbLstPtr + (SuId) (*ctldPcb)->cfg.sigInt);

      if (*pcb)
      {
         /* If the Interface is the Backup return Backup PCB */
         if (((*pcb)->cfg.buIntPr) && 
             ((*ctldPcb)->suId == ((*pcb)->cfg.bupInt)))
         {
            *pcb = *(pcbLstPtr + (SuId) (*pcb)->cfg.bupInt);
         }
      }
   }
   else
   {
      *pcb = *(pcbLstPtr + (SuId) (*ctldPcb)->suId);
   }
  
   RETVALUE(LCM_REASON_NOT_APPL);
}/* end of inGetPcb */ 




/*
 *
 *      Fun:   inGetCb
 *
 *      Desc: 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None
 *
 *      File:  qn_bdy11.c
 *
*/

#ifdef ANSI
PUBLIC S16 inGetCb
(
 InCtldPcb *ctldPcb,
 InPCB *sigPcb,
 S16 ces,
 InCb **cb
 )
#else
     PUBLIC S16 inGetCb(ctldPcb, sigPcb, ces, cb)
     InCtldPcb *ctldPcb;
     InPCB *sigPcb;
     S16 ces;
     InCb **cb;
#endif
{
   TRC3(inGetCb);

   if (!sigPcb)
   {
      RETVALUE(LCM_REASON_INVALID_SAP);                      
   }
   
   if (sigPcb->cfg.nfasInt)
   {
      if (ctldPcb->suId != sigPcb->suId)
      {
         *cb = NULLP;
         /* Bug 85635: NFAS/NFASBACKUP will
          * have chance to have different suId
          * between ctldPcb and sigPcb.
          */
         RETVALUE(LCM_REASON_NOT_APPL);
      }
   }
      
   if (!(*cb = *(sigPcb->cesLst + (SuId) ces)))
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      INLOGERROR(ERRCLS_INT_PAR, EIN710, (ErrVal)ces,
                 "InGetCb() failed, unable to access SAP.");
#endif
      RETVALUE(LCM_REASON_INVALID_SAP);                      
   }

   RETVALUE(LCM_REASON_NOT_APPL);
} /* end of inGetCb */



/*
 *
 *      Fun:   inGetBkup
 *
 *      Desc: 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None
 *
 *      File:  qn_bdy11.c
 *
*/

#ifdef ANSI
PUBLIC S16 inGetBkup
(
 InPCB *pcb,
 InCb **cb
 )
#else
     PUBLIC S16 inGetBkup(pcb, cb)
     InPCB *pcb;
     InCb **cb;
#endif
{
   SuId buInt;

   TRC3(inGetBkup);

   if ((pcb->cfg.nfasInt) && (pcb->cfg.buIntPr))
   {
      /* TELICA-jwh-10/24/2000:  Remove (U8) cast of suId */
      if (pcb->cfg.sigInt == pcb->suId)
         buInt = pcb->cfg.bupInt;
      else
         buInt = pcb->cfg.sigInt;

      if (!(*cb = inGetCBPtr(buInt, CONN_CES)))
      {
#if (ERRCLASS & ERRCLS_INT_PAR)
         INLOGERROR(ERRCLS_INT_PAR, EIN711, (ErrVal) buInt,
                    "InMiLinCntrlReq() failed, invalid ces.");
#endif
         RETVALUE(LCM_REASON_INVALID_SAP);
      }
   }
   else
      RETVALUE(LCM_REASON_INVALID_SAP);

   RETVALUE(LCM_REASON_NOT_APPL);
} /* end of inGetBkup */




/*
 *
 *      Fun:   inClearCb
 *
 *      Desc: 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None
 *
 *      File:  qn_bdy11.c
 *
*/
#ifdef ANSI
PUBLIC S16 inClearCb
(
 InPCB *sigPcb,
 InCtldPcb *ctldPcb,
 U8 clrCalls
 )
#else
     PUBLIC S16 inClearCb(sigPcb, ctldPcb, clrCalls)
     InPCB *sigPcb;
     InCtldPcb *ctldPcb;
     U8 clrCalls;
#endif
{  
   U8 tmrNum;
   CalId *ent;
   InCb *cb;
   S16 j;
   S16 ret;
   InCesRst inCesRst;

   TRC3(inClearCb);

   for (j=0; j < sigPcb->cfg.nmbCes; j++)
   {
      if ((ret = inGetCb(ctldPcb, sigPcb, j, &cb)) != LCM_REASON_NOT_APPL)
      {
#if (ERRCLASS & ERRCLS_INT_PAR)
         INLOGERROR(ERRCLS_INT_PAR, EIN886, (ErrVal)j,
                    "inClearCb() failed, unable to access SAP.");
#endif
         continue;
      }
     
      if (cb == NULLP)
      {
         continue;
      }

      if (sigPcb->cfg.nfasInt)
         cb->sigState = ST_OOS;
    
      /* Stop All B Channel Timers for the signalling ctldPcb */
      inStopAllBchanTimers(sigPcb);

      /* Timers on all other B links will be stopped in inFreeCtldPcb() or
         inFreePcb() */

      switch (cb->state)
      {
    
         case CBST1XFER:

            STOP_SPID_TIMERS;
      
            /* clear all suspended calls */
            if (cb->numCalId)
               while (cb->numCalId)
               {
                  ent = inGetCalEntry(cb, 0);
                  if (ent->ntc->state == ST_NULL)
                  {
                     ent->ntc->updStatus = NTC_DELETED;
                     inRelChan(ent->ntc);
                     inRelCalRef(ent->ntc);                                   
                  }
               }

         case CBST0CON:
            /* Clear Calls */
            if (clrCalls == FTHA_CLEAR)
            {
               inClearAllCalls(cb, NON_STABLE_NOIND);
            }
            /* TELICA-jwh-09/06/2001:  Only pace when a slot goes down */
            else if (clrCalls == NORMAL_CLEAR_PACE)
               inClearAllCalls(cb, NORMAL_CLEAR_PACE);
            else
               inClearAllCalls(cb, ALL_CALLS);

            break;

         case CBST2RST:
            if (!cb->inRstJobFlg)
            {
               /* Stop T_316 */
               if (cb->state == CBST2RST)
                  for (tmrNum = 0; tmrNum< MAXBCHTIMER;tmrNum++)
                     if (ctldPcb->timers[tmrNum].tmrEvnt==TMR_T316)
                        inRmvCtldPcbTq(ctldPcb, tmrNum);
            }
      
            break;
      
         default:
            break;
      } /* switch (cb->state) */

      cb->state = CBST3DOWN;
      inCesRst.intId = ctldPcb->suId;
      inCesRst.ces = cb->ces;
      if (clrCalls == FTHA_CLEAR)
         inCesRst.calls = NON_ACTIVE_CALLS;
      else
         inCesRst.calls = ALL_CALLS;
      zqRunTimeUpd(ZQ_CES_DELNTC_CB, CMPFTHA_UPD_REQ, (PTR)&inCesRst);
      zqRunTimeUpd(ZQ_CES_CB, CMPFTHA_UPD_REQ, (PTR)cb);
   } /* for loop for sigPcb->cfg.nmbCes */

   RETVALUE(LCM_REASON_NOT_APPL);
} /* end of inClearCb */

/*
*
*      Fun:   inClearCtldCb
*
*      Desc: 
*
*      Ret:   ROK   - ok
*
*      Notes: None
*
*      File:  qn_bdy11.c
*
*/
PUBLIC S16 inClearCtldCb
(
 InPCB *sigPcb,
 InCtldPcb *ctldPcb,
 U8 clrCalls
 )
{  
   InCb *dCb;

   TRC3(inClearCtldCb);
   /* get active D link */
   if ((dCb = inGetSigCb(sigPcb)) == NULLP)
   {
      DP("inClearCtldCb can't find DCB \n");
      ctldPcb->state = CBST3DOWN;     
      RETVALUE(LCM_REASON_NOT_APPL);
   }

   /* Stop All B Channel Timers for the signalling pcb */
   inClearBChanCalls(ctldPcb, NULLP, dCb, ADISGRC);
   ctldPcb->state = CBST3DOWN;

   RETVALUE(LCM_REASON_NOT_APPL);
} /* end of inClearCtldCb */


/*
 *
 *      Fun:   inCntrlDLSAP
 *
 *      Desc: 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None
 *
 *      File:  qn_bdy11.c
 *
*/

#ifdef ANSI
PUBLIC S16 inCntrlDLSAP
(
 InMngmt *cntrl,           /* control */
 SuId sapId,
 Ces ces
 )
#else
     PUBLIC S16 inCntrlDLSAP(cntrl, sapId, ces)
     InMngmt *cntrl;          /* control */
     SuId sapId;
     Ces ces;
#endif
{

   InCtldPcb *ctldPcb;
   InPCB *sigPcb;
   S16 ret;
   InCb *cb;
   U8 clrCalls;

  /* Bug:30909 block SATRC replicaton ZqRtUpdCntrlReq zqCntrlReqStruct; */

   TRC3(inCntrlDLSAP);

   if ((zqCb.protState != ACTIVE) && 
       (cntrl->t.cntrl.subAction != SATRC))
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      INLOGERROR(ERRCLS_DEBUG, EIN739, (ErrVal)zqCb.protState,
                 "Event received in invalid fault tolerant state ");
#endif 
      inGenAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_MI_INV_EVT,
                 LCM_CAUSE_PROT_NOT_ACTIVE, sapId);
      RETVALUE(RFAILED);
   }

#ifdef IN_LMINT3  
   clrCalls = cntrl->t.cntrl.clrCalls;
#else
   clrCalls = NORMAL_CLEAR;
#endif /* IN_LMINT3 */

   if ((ret = inGetPcb(sapId, &ctldPcb, &sigPcb)) 
       != LCM_REASON_NOT_APPL)
      RETVALUE(ret);

   if (!sigPcb)
   {
      DP("[Q931]: can't find DlSAP with sapID = %d, subAction = %d \n", sapId,
         cntrl->t.cntrl.subAction);
      /* MCAO_DBG: may need turn it to ROK when goes to customer!!! */
      RETVALUE(LCM_REASON_INVALID_SAP);
   }
  
   switch(cntrl->t.cntrl.subAction)
   {
      case SATRC:
         if ((ret = inGetCb(ctldPcb, sigPcb,ces,
                            &cb)) != LCM_REASON_NOT_APPL)
            break;
      
         /* Bugzilla 7867:taraveti */
         if (cb == NULL)
            break;

         if (cntrl->t.cntrl.action == AENA)
            cb->trc = TRUE;
         else if (cntrl->t.cntrl.action == ADISIMM)
            cb->trc = FALSE;
         INDBGP(DBGMASK_MI,"Q931 sigtrace: suId=%d, action=%d, rSuId=%ld\n", sapId,
cntrl->t.cntrl.action, cntrl->t.cntrl.rSuId);

#if 0 /* Bug:30909 replication is not required */
         zqCntrlReqStruct.action = cntrl->t.cntrl.action;
         zqCntrlReqStruct.blockType = cntrl->t.cntrl.subAction;
         zqCntrlReqStruct.id.trcSapId.ces = ces;
         zqCntrlReqStruct.id.trcSapId.suId = sapId;
         /* in005.310-change to remove typing error in in003.310 */
         zqRunTimeUpd(ZQ_CNTRLREQ_CB, CMPFTHA_CREATE_REQ, (PTR)(&zqCntrlReqStruct));
         zqUpdPeer();
#endif
         break;
      case SADBG:
         if ((ret = inGetCb(ctldPcb, sigPcb, ces,
                            &cb)) != LCM_REASON_NOT_APPL)
         {
            DP("warning: no InCb found for suId %d\n", sapId);
            break;
         }
         sigPcb->dbgMask = cntrl->t.cntrl.maintBChanStat;
         cb->dbgMask = cntrl->t.cntrl.maintBChanStat;
         INDBGP(DBGMASK_MI,"set dbgMask = %#08lx for rSuId %ld\n", cb->dbgMask, cntrl->t.cntrl.rSuId);  
         /* need populate it to standby later */
         break;
      case SAELMNT:
         switch (cntrl->t.cntrl.action)
         {
            /* TELICA-jwh-01/24/2001:  Add Q931 action to gracefully disable an interface. */
            case ADISGRC:  /* Action to clear all calls on interface and then disable entire interface */
               ret = inDisableInterface(ctldPcb, sigPcb, ces, cntrl->t.cntrl.action);
               break;
            case ADISIMM:
            case AMOOS:
            case ARST:
               ret = inDisableDLSAP(ctldPcb, sigPcb,ces,
                                    cntrl->t.cntrl.action); 
               break;

            case AENA:
               ret = inEnableSap(ctldPcb, sigPcb,ces);
               break;

            case ADEL:
               ret = inDeleteDLSAP(ctldPcb, sigPcb,ces); 
               break;

            case ABND_ENA:
            case ABND:
               ret = inBndEnableSap(sigPcb, cntrl->t.cntrl.action);
               /* BUG 43595 mpralat 2006-01-19 */
               if ((ret == LCM_PRIM_OK_NDONE) && (cntrl->t.cntrl.action == ABND_ENA))
               {
                   /* LCM_PRIM_OK_NDONE indentifies that
                    * the links is bound, but not necessarily enabled.
                    */
                   ret = inEnableSap(ctldPcb, sigPcb,ces);
               }
               break;
            case AUBND_DIS:
               ret = inDisableInterface(ctldPcb, sigPcb, ces, cntrl->t.cntrl.action);
               break;
            case ASRVMSG_ON:
               sigPcb->cfg.srvOpt |= SRV_MSG_ON;
               ret = LCM_REASON_NOT_APPL;
               break;

            case ASRVMSG_OFF:
               sigPcb->cfg.srvOpt &= ~SRV_MSG_ON;
               ret = LCM_REASON_NOT_APPL;
               break;

            case ARESTART:
               if (sigPcb->cfg.swtch == SW_ATT5EP)  
               {
#if (ERRCLASS & ERRCLS_INT_PAR)
                  INLOGERROR(ERRCLS_INT_PAR, EINXXX, (ErrVal) cntrl->t.cntrl.action,
                             "inCntrlDLSAP() failed, invalid Action.");
#endif
                  ret = LCM_REASON_INVALID_ACTION;
               }
               else
                  ret = inRestartInt(sigPcb, ctldPcb, ces);
               break;
               /* switch over Dchan request */
            case ASYNCHRONIZE:  
               ret = inCtrldDChanSwtchOver(sigPcb, ctldPcb, ces);
               break;

            default:
#if (ERRCLASS & ERRCLS_INT_PAR)
               INLOGERROR(ERRCLS_INT_PAR, EIN712, (ErrVal) cntrl->t.cntrl.action,
                          "inCntrlDLSAP() failed, invalid Action.");
#endif
               ret = LCM_REASON_INVALID_ACTION;
         }
         break;
      default:
#if (ERRCLASS & ERRCLS_INT_PAR)
         INLOGERROR(ERRCLS_INT_PAR, EIN713, (ErrVal)cntrl->t.cntrl.subAction,
                    "inCntrlDLSAP() failed, invalid config request.");
#endif
         ret = LCM_REASON_INVALID_SUBACTION;
   }  

   /* TELICA-jwh-01/31/2001:  Flush replication queue. */
   zqUpdPeer();

   RETVALUE(ret);
} /* end of inCntrlDLSAP */


/*
*
*      Fun:   inCntrlCTRLDSAP
*
*      Desc: 
*
*      Ret:   ROK   - ok
*
*      Notes: None
*
*      File:  qn_bdy11.c
*
*/

PUBLIC S16 inCntrlCTRLDSAP
(
 InMngmt *cntrl,           /* control */
 SuId sapId,
 Ces ces
)
{

   InCtldPcb *ctldPcb;
   InPCB *sigPcb = NULLP;
   S16 ret;
   U8 clrCalls;

   TRC3(inCntrlCTRLDSAP);

   if ((zqCb.protState != ACTIVE) && 
       (cntrl->t.cntrl.subAction != SATRC))
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      INLOGERROR(ERRCLS_DEBUG, EIN739, (ErrVal)zqCb.protState,
                 "Event received in invalid fault tolerant state ");
#endif 
      inGenAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_MI_INV_EVT,
                 LCM_CAUSE_PROT_NOT_ACTIVE, sapId);
      RETVALUE(RFAILED);
   }

#ifdef IN_LMINT3  
   clrCalls = cntrl->t.cntrl.clrCalls;
#else
   clrCalls = NORMAL_CLEAR;
#endif /* IN_LMINT3 */

   if ((ret = inGetPcb(sapId, &ctldPcb, &sigPcb)) 
       != LCM_REASON_NOT_APPL)
      RETVALUE(ret);
  
   switch(cntrl->t.cntrl.subAction)
   {
      case SAELMNT:
         switch (cntrl->t.cntrl.action)
         {
            case ADISGRC:  /* Action to clear all calls on interface and then disable entire interface */
               ret = inDisableLink(ctldPcb, sigPcb, ces, cntrl->t.cntrl.action);
               break;

            case ADEL:
               ret = inDeleteLink(ctldPcb, sigPcb, ces); 
               break;

            case AENA:
               ret = inEnableLink(ctldPcb, sigPcb, ces, cntrl);
               break;
            default:
#if (ERRCLASS & ERRCLS_INT_PAR)
               INLOGERROR(ERRCLS_INT_PAR, EIN712, (ErrVal) cntrl->t.cntrl.action,
                          "inCntrlCTRLDSAP() failed, invalid Action.");
#endif
               ret = LCM_REASON_INVALID_ACTION;
         }
         break;
      default:
#if (ERRCLASS & ERRCLS_INT_PAR)
         INLOGERROR(ERRCLS_INT_PAR, EIN713, (ErrVal)cntrl->t.cntrl.subAction,
                    "inCntrlCTRLDSAP() failed, invalid config request.");
#endif
         ret = LCM_REASON_INVALID_SUBACTION;
   }  

   zqUpdPeer();

   RETVALUE(ret);
} /* end of inCntrlCTRLDSAP */




/*
 *
 *      Fun:   inCntrlTSAP
 *
 *      Desc: 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None
 *
 *      File:  qn_bdy11.c
 *
*/

#ifdef ANSI
PUBLIC S16 inCntrlTSAP
(
 InMngmt *cntrl,           /* control */
 SuId sapId
 )
#else
     PUBLIC S16 inCntrlTSAP(cntrl, sapId)
     InMngmt *cntrl;          /* control */
     SuId sapId;
#endif
{
   S16 ret;

   TRC3(inCntrlTSAP);

   if (zqCb.protState != ACTIVE)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      INLOGERROR(ERRCLS_DEBUG, EIN740, (ErrVal)zqCb.protState,
                 "Event received in invalid fault tolerant state ");
#endif 
      inGenAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_MI_INV_EVT,
                 LCM_CAUSE_PROT_NOT_ACTIVE, sapId);
      RETVALUE(RFAILED);
   }

   if (cntrl->t.cntrl.subAction != SAELMNT)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      INLOGERROR(ERRCLS_INT_PAR, EIN714, (ErrVal)cntrl->t.cntrl.subAction,
                 "inCntrlDLSAP() failed, invalid config request.");
#endif
      RETVALUE (LCM_REASON_INVALID_SUBACTION); 
   }

   switch (cntrl->t.cntrl.action)
   {
      case ADEL:
         ret = inDeleteTSAP(sapId);
         break;
      case AUBND_DIS:
         ret = inUBndDisTSAP(sapId);
         break;
      default:
#if (ERRCLASS & ERRCLS_INT_PAR)
         INLOGERROR(ERRCLS_INT_PAR, EIN715, (ErrVal) cntrl->t.cntrl.action,
                    "inCntrlDLSAP() failed, invalid Action.");
#endif
         ret = LCM_REASON_INVALID_ACTION;
      
   }

   RETVALUE(ret);
} /* end of inCntrlTSAP */





/*
 *
 *      Fun:   inCntrlGen
 *
 *      Desc: 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None
 *
 *      File:  qn_bdy11.c
 *
*/

#ifdef ANSI
PUBLIC S16 inCntrlGen
(
 InMngmt *cntrl           /* control */
 )
#else
     PUBLIC S16 inCntrlGen(cntrl)
     InMngmt *cntrl;          /* control */
#endif
{
   ZqRtUpdCntrlReq zqCntrlReqStruct;

   TRC3(inCntrlGen);

   if (cntrl->t.cntrl.action == ASHUTDOWN)
   {
      inShutDown();
      RETVALUE(LCM_REASON_NOT_APPL);
   }
   else
   {
      switch(cntrl->t.cntrl.subAction)
      {
         case SAUSTA:
            if (cntrl->t.cntrl.action == AENA)
               inInit.usta = TRUE;
            else if (cntrl->t.cntrl.action == ADISIMM)
               inInit.usta = FALSE;
            break;
        
         case SAACNT:
            if (cntrl->t.cntrl.action == AENA)
               inInit.acnt = TRUE;
            else if (cntrl->t.cntrl.action == ADISIMM)
               inInit.acnt = FALSE;
            break;
         case SAELMNT:
            if (cntrl->t.cntrl.action == ACONDLYR)
            {
               DP_PLMIN(DBGMASK_L1,"inCntlGen: Q931 Received Cntl to Init L3 Action = ACONDLYR\n");
               inGeoHandleBChannel(cntrl);
            }
            break;
        
         default:
#if (ERRCLASS & ERRCLS_INT_PAR)
            INLOGERROR(ERRCLS_INT_PAR, EIN716, (ErrVal) cntrl->t.cntrl.action,
                       "inCntrlDLSAP() failed, invalid Action.");
#endif
            RETVALUE(LCM_REASON_INVALID_ACTION);      
      }
   }

   /* ngc - SAELMNT not supported in zqCntrlReqStruct upd - bug 47964 */
   if( cntrl->t.cntrl.subAction == SAELMNT) 
   {
      zqUpdPeer();
      RETVALUE(LCM_REASON_NOT_APPL);
   }

   zqCntrlReqStruct.action = cntrl->t.cntrl.action;
   zqCntrlReqStruct.blockType = cntrl->t.cntrl.subAction;
   if (cntrl->t.cntrl.subAction == SAUSTA)
      zqCntrlReqStruct.blockType = SAUSTA_IN;
   /* in005.310-change to remove typing error in in003.310 */
   zqRunTimeUpd(ZQ_CNTRLREQ_CB, CMPFTHA_CREATE_REQ, (PTR)(&zqCntrlReqStruct));
   zqUpdPeer();
   RETVALUE(LCM_REASON_NOT_APPL);
} /* end of inCntrlGen */


/*
*
*      Fun:   inCntrlBChan
*
*      Desc: 
*
*      Ret:   ROK   - ok
*      Notes: None
*
*      File:  qn_bdy11.c
*
*/

#ifdef ANSI
PUBLIC S16 inCntrlBChan
(
 InMngmt *cntrl,           /* control */
 SuId sapId,
 Ces ces,
 U8 chn
 )
#else
     PUBLIC S16 inCntrlBChan(cntrl, sapId, ces, chn)
     InMngmt *cntrl;          /* control */
     SuId sapId;
     Ces ces;
     U8 chn;
#endif
{
   InCtldPcb *ctldPcb = NULLP;
   InPCB *sigPcb = NULLP;
   S16 ret = ROK;
   InCb *cb = NULLP;
   InBearChn *bearChn;

   TRC3(inCntrlBChan);

   /* TELICA-MCAO-02/27/02: don't execute and cntrlreq at standby */
   if (zqCb.protState != ACTIVE)  
      RETVALUE(LCM_REASON_NOT_APPL); 

   if ((cntrl->t.cntrl.subAction != SAELMNT)
      && (cntrl->t.cntrl.subAction != SAPORTDOWN))
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      INLOGERROR(ERRCLS_INT_PAR, EIN717, (ErrVal)cntrl->t.cntrl.subAction,
                 "inCntrlDLSAP() failed, invalid config request.");
#endif
      RETVALUE(LCM_REASON_INVALID_SUBACTION); 
   }

   if ((ret = inGetPcb(sapId, &ctldPcb, &sigPcb)) 
       != LCM_REASON_NOT_APPL)
      RETVALUE(ret);

   if (!sigPcb)
   {
      DP("signaling link of suId %d not configured yet \n", sapId); 
      RETVALUE(LCM_REASON_NOT_APPL);
   }
   
   if ((ret = inGetCb(ctldPcb, sigPcb,ces,
                      &cb)) != LCM_REASON_NOT_APPL)
      RETVALUE(ret);

   /* FID14382.8 */
   if (cb != NULLP)
   {
      if ((cntrl->t.cntrl.action == AINH)
         && (cntrl->t.cntrl.subAction == SAPORTDOWN))
      {
         cb->rel_force = TRUE;
      }
      else if (cntrl->t.cntrl.action == AUNINH)
      {
         cb->rel_force = FALSE;
      }
   }

   /* FID17233.0 Modify ISDN B Channel Direction */
   if (cntrl->t.cntrl.action == AENA    ||
       cntrl->t.cntrl.action == ADISIMM ||
       cntrl->t.cntrl.action == ARESTART)
   {
       IN_GET_BEARER(ctldPcb, chn,
            ctldPcb->cfg.firstBChanNum, bearChn);
       bearChn->chnDir = cntrl->t.cntrl.chanDir;
   }

   switch (cntrl->t.cntrl.action)
   {
      /* TELICA-jwh-01/04/2001: BCHANSYNC:  Add bchan actions so maint blocking can be distinguished from hardware blocking */
      case AINH:
      case AUNINH:
      case ADISIMM: 
      case ADISGRC:    /* Inhibit gracefully */
      case AENA:

         /* ISDN Persistent B/D Channel Status, H.Kim */
         if(zqCb.protState == ACTIVE)
         {
            ret = inDisEnbChan(ctldPcb, sigPcb,chn,
                               cntrl->t.cntrl.action, cntrl->t.cntrl.maintBChanStat);
         }
         else
         {
            ret = LCM_REASON_NOT_APPL;
         }
         break;

      case AADD_BCHAN:
      case ADEL_BCHAN:
         ret = inDelAddChan(ctldPcb,chn, 
                            cntrl->t.cntrl.action);
         break;

      case ARESTART:

         /* TELICA-jwh-01/25/2001:  Let's send the restarts out of the ACTIVE D channel. */
         if (!(cb = inGetSigCb(sigPcb)))
            RETVALUE(LCM_REASON_INVALID_SAP);

         /* TELICA-MCAO-10/12/01: send restart out on PRI only when it's connected */
         if ((cb->state != CBST1XFER) && (cb->state != CBST2RST))
         {
            /* Bug32244: mcao-08/10/04 special handling here: bring channel out
               from MAINT_NE without sending restart messages, but still
               print a notice */
            if (bearChn->status == NOTPROVISIONED)
               RETVALUE(LCM_REASON_NOT_APPL);

            INDBGP(DBGMASK_MI,"channel reset failed to sending restart messages, ctldPcb->rSuId=%ld, chn=%d, cb->state=%d ",
               ctldPcb->rSuId, chn, cb->state);

            if (bearChn->status == MAINT_NE)
               bearChn->status = OUT_SERVICE_NE;
            
            RETVALUE(LCM_REASON_NOT_APPL); 
         }
         else
            ret = inRestartChan(sigPcb, ctldPcb, cb, chn);
            if (ret == ROKIGNORE)
            {
               INDBGP(DBGMASK_MI,"BChan being ignored due to something already in reset\n");
               ret = ROK;
            }
         break;

      default:
#if (ERRCLASS & ERRCLS_INT_PAR)
         INLOGERROR(ERRCLS_INT_PAR, EIN718, (ErrVal) cntrl->t.cntrl.action,
                    "inCntrlDLSAP() failed, invalid Action.");
#endif
         ret = LCM_REASON_INVALID_ACTION;
         break;
   }

   RETVALUE(ret);
} /* end of inCntrlBChan */





/*
 *
 *      Fun:   inCntrlGrpDLSAP
 *
 *      Desc: 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None
 *
 *      File:  qn_bdy11.c
 *
*/

#ifdef ANSI
PUBLIC S16 inCntrlGrpDLSAP
(
 InMngmt *cntrl           /* control */
 )
#else
     PUBLIC S16 inCntrlGrpDLSAP(cntrl)
     InMngmt *cntrl;          /* control */
#endif
{

   S16 ret;
   S16 sapId;
   U8 clrCalls;

   TRC3(inCntrlGrpDLSAP);

#ifdef IN_LMINT3 
   sapId = cntrl->t.cntrl.sapId;
   /* TELICA-jwh-09/06/2001:  Only pace when a slot goes down */
   clrCalls = NORMAL_CLEAR_PACE;
#else
   sapId = cntrl->hdr.elmId.elmntInst1;
   clrCalls = NORMAL_CLEAR;
#endif 

   if (zqCb.protState != ACTIVE)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      INLOGERROR(ERRCLS_DEBUG, EIN742, (ErrVal)zqCb.protState,
                 "Event received in invalid fault tolerant state ");
#endif 
      inGenAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_MI_INV_EVT,
                 LCM_CAUSE_PROT_NOT_ACTIVE, sapId);
      RETVALUE(RFAILED);
   }

   if (cntrl->t.cntrl.subAction != SAGR_DSTPROCID)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      INLOGERROR(ERRCLS_INT_PAR, EIN719, (ErrVal)cntrl->t.cntrl.subAction,
                 "inCntrlGrpDLSAP() failed, invalid config request.");
#endif
      RETVALUE (LCM_REASON_INVALID_SUBACTION); 
   }


   switch (cntrl->t.cntrl.action)
   {
      case ABND_ENA:
      case AUBND_DIS:
         ret = inHandleGrpDLSAP(cntrl->t.cntrl.par.dstProcId,
                                cntrl->t.cntrl.action, clrCalls);
         break;

      default:
#if (ERRCLASS & ERRCLS_INT_PAR)
         INLOGERROR(ERRCLS_INT_PAR, EIN720, (ErrVal) cntrl->t.cntrl.action,
                    "inCntrlDLSAP() failed, invalid Action.");
#endif
         ret = LCM_REASON_INVALID_ACTION;
   }
   RETVALUE(ret);
} /* end of inCntrlGrpDLSAP */






/*
 *
 *      Fun:   inCntrlGrpTSAP
 *
 *      Desc: 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None
 *
 *      File:  qn_bdy11.c
 *
*/

#ifdef ANSI
PUBLIC S16 inCntrlGrpTSAP
(
 InMngmt *cntrl           /* control */
 )
#else
     PUBLIC S16 inCntrlGrpTSAP(cntrl)
     InMngmt *cntrl;          /* control */
#endif
{
   S16 ret;
   S16 sapId;

   TRC3(inCntrlGrpTSAP);

#ifdef IN_LMINT3 
   sapId = cntrl->t.cntrl.sapId;
#else
   sapId = cntrl->hdr.elmId.elmntInst1;
#endif 

   if (zqCb.protState != ACTIVE)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      INLOGERROR(ERRCLS_DEBUG, EIN743, (ErrVal)zqCb.protState,
                 "Event received in invalid fault tolerant state ");
#endif 
      inGenAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_MI_INV_EVT,
                 LCM_CAUSE_PROT_NOT_ACTIVE, sapId);
      RETVALUE(RFAILED);
   }

   if (cntrl->t.cntrl.subAction != SAGR_DSTPROCID)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      INLOGERROR(ERRCLS_INT_PAR, EIN721, (ErrVal)cntrl->t.cntrl.subAction,
                 "inCntrlGrpTSAP() failed, invalid config request.");
#endif
      RETVALUE(LCM_REASON_INVALID_SUBACTION); 
   }

   if (cntrl->t.cntrl.action == AUBND_DIS)
   {
      ret = inDisableGrpTSAP(cntrl->t.cntrl.par.dstProcId);
   }
   else
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      INLOGERROR(ERRCLS_INT_PAR, EIN722, (ErrVal) cntrl->t.cntrl.action,
                 "inCntrlGrpTSAP() failed, invalid Action.");
#endif
      ret = LCM_REASON_INVALID_ACTION;
   }

   RETVALUE(ret);
} /* end of inCntrlGrpTSAP */


/*
 *
 *      Fun:   inCntrlAllSaps
 *
 *      Desc: 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None
 *
 *      File:  qn_bdy11.c
 *
*/

#ifdef ANSI
PUBLIC S16 inCntrlAllSaps
(
 InMngmt *cntrl           /* control */
 )
#else
     PUBLIC S16 inCntrlAllSaps(cntrl)
     InMngmt *cntrl;          /* control */
#endif
{
   S16 ret;
   SuId sapId;
   Ces ces;
   U8 clrCalls;

   TRC3(inCntrlAllSaps);

   if (zqCb.protState != ACTIVE)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      INLOGERROR(ERRCLS_DEBUG, EIN744, (ErrVal)zqCb.protState,
                 "Event received in invalid fault tolerant state ");
#endif 
      inGenAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_MI_INV_EVT,
                 LCM_CAUSE_PROT_NOT_ACTIVE, 0);
      RETVALUE(RFAILED);
   }

   switch(cntrl->t.cntrl.subAction)
   {
      case SAELMNT:
         switch (cntrl->t.cntrl.action)
         {
            case ABND_ENA:
               ret = inBndEnableAllSaps();
               break;

            case AUBND_DIS:

#ifdef IN_LMINT3  
               clrCalls = cntrl->t.cntrl.clrCalls;
#else
               clrCalls = NORMAL_CLEAR;
#endif /* IN_LMINT3 */
               ret = inUBndDisableAllSaps(clrCalls);
               break;
            case ARESTART:
#ifdef IN_LMINT3
               sapId = cntrl->t.cntrl.sapId;
               ces = cntrl->t.cntrl.ces;
#else
               sapId = cntrl->hdr.elmId.elmntInst1;
               ces = cntrl->hdr.elmId.elmntInst2;
#endif   /* IN_LMINT3 */
               ret = inRestartAllInt(sapId, ces);
               break;
            default:
#if (ERRCLASS & ERRCLS_INT_PAR)
               INLOGERROR(ERRCLS_INT_PAR, EIN723, (ErrVal) cntrl->t.cntrl.action,
                          "inCntrlAllSaps() failed, invalid Action.");
#endif
               ret = LCM_REASON_INVALID_ACTION;
         }
         break;

      case SAGR_DSTPROCID:
         ret = LCM_REASON_NOT_APPL;
         switch (cntrl->t.cntrl.action)
         {
            case ABND_ENA:
               break;

            case AUBND_DIS:
               break;

            default:
#if (ERRCLASS & ERRCLS_INT_PAR)
               INLOGERROR(ERRCLS_INT_PAR, EIN724, (ErrVal) cntrl->t.cntrl.action,
                          "inCntrlGrpTSAP() failed, invalid Action.");
#endif
               ret = LCM_REASON_INVALID_ACTION;
         }
         break;

      default:
#if (ERRCLASS & ERRCLS_INT_PAR)
         INLOGERROR(ERRCLS_INT_PAR, EIN725, (ErrVal)cntrl->t.cntrl.subAction,
                    "inCntrlGrpTSAP() failed, invalid config request.");
#endif
         ret = LCM_REASON_INVALID_SUBACTION;
         break;
   }
   RETVALUE(ret);
} /* end of inCntrlAllSaps */




/*
 *
 *      Fun:   inCntrlDigitMap
 *
 *      Desc: 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None
 *
 *      File:  qn_bdy11.c
 *
*/

#ifdef ANSI
PUBLIC S16 inCntrlDigitMap
(
 InMngmt *cntrl               /* control */
 )
#else
     PUBLIC S16 inCntrlDigitMap(cntrl)
     InMngmt *cntrl;          /* control */
#endif
{
   S16         ret;
   LngAddrs    digitMapKey;
   U16         candidateId;
   InDigitMap  *pDigitMap;
   DigitMapCandidate *pCandidate;

   TRC3(inCntrlDigitMap);

   digitMapKey = cntrl->t.cntrl.inDgMap.digitMapName;
   candidateId = cntrl->t.cntrl.inDgMap.id;

   if (cntrl->t.cntrl.subAction != SAELMNT)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      INLOGERROR(ERRCLS_INT_PAR, EINXXX, (ErrVal)cntrl->t.cntrl.subAction,
                 "inCntrlDigitMap() failed, invalid config request.");
#endif
      RETVALUE (LCM_REASON_INVALID_SUBACTION); 
   }

   if ((ret = cmHashListFind(&inGlobalCb.digitMapHl, digitMapKey.strg, 
                             digitMapKey.length, (U16)0, (PTR *) &pDigitMap)) != ROK)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      INLOGERROR(ERRCLS_INT_PAR, EINXXX, (ErrVal)ERRZERO,
                 "inCntrlDigitMap() failed, no such digit map name.");
#endif
      RETVALUE(LCM_REASON_NOT_APPL);
   }

   if ((ret = inFindCandidateInList(pDigitMap, candidateId, &pCandidate)) != ROK)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      INLOGERROR(ERRCLS_INT_PAR, EINXXX, (ErrVal)candidateId,
                 "inCntrlDigitMap() failed, no such candidate id.");
#endif
      RETVALUE(LCM_REASON_NOT_APPL);
   }

   switch (cntrl->t.cntrl.action)
   {
      case ADEL:
         ret = inDelCandidateFromList(pDigitMap, pCandidate);
         break;
      default:
#if (ERRCLASS & ERRCLS_INT_PAR)
         INLOGERROR(ERRCLS_INT_PAR, EINXXX, (ErrVal) cntrl->t.cntrl.action,
                    "inCntrlDigitMap() failed, invalid Action.");
#endif
         ret = LCM_REASON_INVALID_ACTION;
      
   }

   RETVALUE(ret);
} /* end of inCntrlDigitMap */


/*
 *
 *      Fun:   inEnableSap
 *
 *      Desc: 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None
 *
 *      File:  qn_bdy11.c
 *
*/

#ifdef ANSI
PUBLIC S16 inEnableSap
(
 InCtldPcb *ctldPcb,
 InPCB *sigPcb,
 S16 ces
 )
#else
     PUBLIC S16 inEnableSap(ctldPcb, sigPcb, ces)
     InCtldPcb *ctldPcb;
     InPCB *sigPcb;
     S16 ces;
#endif
{
   InCb *cb;
   S16 ret;
   U8 tmrNum;
   Bool tConRunning = FALSE;  /* bug 94296 */

   TRC3(inEnableSap);
  
   if ((ret = inGetCb(ctldPcb, sigPcb, ces, &cb)) != LCM_REASON_NOT_APPL)
      RETVALUE(ret);  

   INDBGP(DBGMASK_MI,"Q931 got EnableSap cntrl req for rSuId=%ld, spId=%d\n", cb->rSuId, cb->spId);

   ctldPcb->state = CBST1XFER;

   zqRunTimeUpd(ZQ_CTLDPSAP_CB, CMPFTHA_UPD_REQ, (PTR)ctldPcb);
   zqUpdPeer();

   if (cb == NULLP)
      RETVALUE(LCM_REASON_NOT_APPL);

   if (cb->state == CBST3DOWN ||
       cb->state == CBST0CON)   /* bug 94296 */
   {
      if (!cb->inRstJobFlg)
         if ((sigPcb->cfg.intType == NETWORK) || 
             (sigPcb->cfg.teiAlloc == STATIC))
         {
            /* Stop Disconnect Timer */
            for (tmrNum = 0;tmrNum < MAXSIMTIMER;tmrNum++)
               if (cb->timers[tmrNum].tmrEvnt == TMR_DISC)
                  inRmvCbTq(cb, tmrNum);

            /* Check if TMR_CON is running */
            for (tmrNum = 0; tmrNum < MAXSIMTIMER; tmrNum++)
            {
                if (cb->timers[tmrNum].tmrEvnt == TMR_CON)
                {
                    tConRunning = TRUE;
                    break;
                }
            }

            cb->state = CBST0CON;

            if (sigPcb->cfg.nfasInt)
                cb->sigState = ST_MB;

            if (tConRunning)
            {
                INDBGP(DBGMASK_MI,"TMR_CON has already run.\n");
            }
            else
            {
                inStartCbTmr(TMR_CON, cb);
                InLiDatConReq(&cb->pst, cb->spId, Q_930,
                        cb->ces, cb->rSuId);
            }
         }
   }
   RETVALUE(LCM_REASON_NOT_APPL);
} /* end of inEnableSap */


/* TELICA-jwh-01/24/2001:  Add Q931 action to gracefully disable an interface. */
/*
 *
 *      Fun:   inDisableInterface
 *
 *      Desc: 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None
 *
 *      File:  qn_bdy11.c
 *
*/

#ifdef ANSI
PUBLIC S16 inDisableInterface
(
 InCtldPcb *ctldPcb,
 InPCB *sigPcb,
 S16 ces,
 U8 action
 )
#else
     PUBLIC S16 inDisableInterface(ctldPcb, sigPcb, ces, action)
     InCtldPcb *ctldPcb;
     InPCB *sigPcb;
     S16 ces;
     U8 action;
#endif
{
   S16 ret;
   InCtldPcb *actvCtldPcb=NULLP;
   InPCB *actvPcb=NULLP;
   InCb *actvCb=NULLP;
   InCb *cb;
   InCb *tmpCb;
   int numDChans = 1;
   int i;
   U8 sigState, lnkState;
   U8 actvSigState;
   U8 tmrNum;
   Bool reportIsReqed = FALSE;

   TRC3(inDisableInterface);

  /* Make sure the suId indicated by layer manager is a D channel for this interface */
   if ((sigPcb->cfg.nfasInt) && (ctldPcb->suId != sigPcb->suId))
   {
      INDBGP(DBGMASK_MI,"Received invalid DLSAP cntl req for action ADISGRC: sigPcb->rSuId=%ld, ctldPcb->rSuId=%ld\n", sigPcb->rSuId, ctldPcb->rSuId);
      return(LCM_REASON_INVALID_SAP);
   }

   /* Get cb */
   if ((ret = inGetCb(ctldPcb, sigPcb, ces, &cb)) != LCM_REASON_NOT_APPL)
   {
      return(ret);
   }
  
   /* If this is an NFAS with backup, we want to disable current backup first. */
   if ((sigPcb->cfg.nfasInt) && (sigPcb->cfg.buIntPr))
   {
      /* TELICA-MCAO-12/18/01: don't repeat disable funcs if it's already done, it's related to 
         ed-isdn-if state feature and only happen in NFAS w/ BACKUP */
      if ((sigPcb->bndState == IN_UNBND) && (action == AUBND_DIS))
      {
         INDBGP(DBGMASK_MI,"inDisableInterface:  rSuId=%ld, sigState=%d was already unbound. quit...\n",cb->rSuId, cb->sigState);
         return(LCM_REASON_NOT_APPL);      
      }
      if (inGetBkup(sigPcb, &tmpCb) == LCM_REASON_NOT_APPL)
      {
         /* Both D1 and D2 are configured */
         numDChans = 2;

         /* Primary and backup D channels are configured. */
         if (cb->sigState == ST_IS)
         {
            actvCb = cb;
            cb = tmpCb;
            ctldPcb = *(ctldPcbLstPtr + cb->suId);
            sigPcb = *(pcbLstPtr + cb->suId);
         }
         else
         {
            actvCb = tmpCb;
         }

         actvCtldPcb = *(ctldPcbLstPtr + actvCb->suId);
         actvPcb = *(pcbLstPtr + actvCb->suId);

         actvSigState = actvCb->sigState;
      }
   }

   for (i=0; i<numDChans; i++)
   {

      if (sigPcb->bndState == IN_UNBND)
      {
         INDBGP(DBGMASK_MI,"inDisableInterface:  rSuId=%ld, sigState=%d was already unbound.  Proceeding...\n",cb->rSuId, cb->sigState);
      }

      /* only D link which was up need to report down */
      sigState = cb->sigState;
      lnkState = cb->state;
      if (sigPcb->cfg.nfasInt)
      {
         if (((lnkState == CBST1XFER) ||(lnkState == CBST2RST)) &&
             ((sigState == ST_IS) ||(sigState == ST_STB)))
            reportIsReqed = TRUE;
      }
      else if ((lnkState == CBST1XFER) ||(lnkState == CBST2RST))
         reportIsReqed = TRUE;
    
      /* Clear all calls */
      if ((ret = inClearCb(sigPcb, ctldPcb, NORMAL_CLEAR) != LCM_REASON_NOT_APPL))
      {
         INLOGERROR(ERRCLS_INT_PAR, EIN883, (ErrVal)ret,
                    "inDisableInterface() failed, Failed to Clear Cb");
         RETVALUE(ret);
      }

      /* Disable signaling on this link */
      if (sigPcb->cfg.nfasInt)
      {
         cb->sigState = ST_MOOS;
         cb->state = CBST3DOWN;
         zqRunTimeUpd(ZQ_CES_CB, CMPFTHA_UPD_REQ, (PTR)cb);
      }

      inDisableCtldPcb(sigPcb);

      /* MCAO-9312  stop all Cb timer */
      /* MCAO: don't start timer DISC */
      STOP_CB_TIMERS;
      /*  only report link down when lnk state is not down */
      if (reportIsReqed)
         inNmReport(LCM_CATEGORY_INTERFACE, LCM_EVENT_DOWN,
                    LCM_CAUSE_DCHAN,(U8) DCHAN, cb->suId, (U8) 0);
      cb->rstCfmSnt = TRUE; /* MCAO-9312 set flag to avoid redundant staInd */
      zqRunTimeUpd(ZQ_CES_CB, CMPFTHA_UPD_REQ, (PTR)cb);

      if (sigPcb->bndState != IN_UNBND)
      {
         /* Disconnect layer 2 */
         InLiDatDiscReq(&cb->pst, cb->spId, Q_930, cb->ces, DAT_DM_RLS);
         /* Unbind layer 2 */
         InLiDatUbndReq(&sigPcb->pst,sigPcb->spId, Q_930, 0);
      }
      sigPcb->bndState = IN_UNBND;

      zqRunTimeUpd(ZQ_PSAP_CB, CMPFTHA_UPD_REQ, (PTR)sigPcb);
      zqUpdPeer();

      /* Now disable the active DChan */
      cb = actvCb;
      ctldPcb = actvCtldPcb;
      sigPcb = actvPcb;
      sigState = actvSigState;
      /*Bug84708: set reportIsReqed back to FALSE*/
      reportIsReqed = FALSE;
   }

   return(LCM_REASON_NOT_APPL);
} /* inDisableInterface */


PUBLIC S16 inEnableLink
(
 InCtldPcb *ctldPcb,
 InPCB *sigPcb,
 S16 ces,
 InMngmt *cntrl
 )
{
   S16 ret = LCM_REASON_NOT_APPL;

   TRC3(inEnableLink);

   if (zqCb.protState != ACTIVE)
   {
      RETVALUE(LCM_REASON_NOT_APPL);
   }
   /* non B link or no signaling link case */
   if (!ctldPcb->cfg.nfasInt || !sigPcb)
   {
      INDBGP(DBGMASK_MI,"PLM_IN: not nfas B link or no signaling link exist, returning..."
         "ctldPcb->cfg.nfasInt = %d, sigPcb= %p \n", ctldPcb->cfg.nfasInt, sigPcb);
      RETVALUE(LCM_REASON_NOT_APPL);
   }

   /* 20558:inDisEnbChan will find active D link (InCb) */
   ret = inDisEnbChan(ctldPcb, sigPcb, cntrl->t.cntrl.chanNum, AUNINH, cntrl->t.cntrl.maintBChanStat);
   if (ret)
   {
      INDBGP(DBGMASK_MI,"Warning:inDisEnbChan got abnormal return ret= %d\n",ret);
   }
   /* we don't expect errors here, even there is return error due to isdn restart glaring */
   RETVALUE(LCM_REASON_NOT_APPL);
}

PUBLIC S16 inDisableLink
(
 InCtldPcb *ctldPcb,
 InPCB *sigPcb,
 S16 ces,
 U8 action
 )
{
   S16 ret;
   InCb *cb;
   InCb *tmpCb;
   U8 linkType = IN_LNK_TYPE_UNKNOWN;
   U8 callTreatment = IN_CALL_TREATMENT_NONE;
   U8 tmrNum;
   Bool needDchanSwitch = FALSE;
   U8 sigState, lnkState;
   Bool reportIsReqed = FALSE;

   TRC3(inDisableLink);

   /* B link w/o signaling link case */
   if (ctldPcb->cfg.nfasInt && !sigPcb)
   {
      INDBGP(DBGMASK_MI,"PLM_IN: no signaling link exist, returning...\n");
      RETVALUE(LCM_REASON_NOT_APPL);
   }

   if (!sigPcb)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      INLOGERROR(ERRCLS_INT_PAR, EIN708, (ErrVal)ctldPcb->rSuId,
                 "inDisableLink() failed, invalid DLSAP.");
#endif
      RETVALUE(LCM_REASON_INVALID_SAP);  
   }

   /* Get cb */
   if (!(cb = *(sigPcb->cesLst + (SuId) ces)))
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      INLOGERROR(ERRCLS_INT_PAR, EIN710, (ErrVal)ces,
                 "InGetCb() failed, unable to access SAP.");
#endif
      RETVALUE(LCM_REASON_INVALID_SAP);                      
   }

   /* identify type of link */
   if (sigPcb->cfg.nfasInt) 
   { 
      if (sigPcb->cfg.buIntPr)
      {
         if (sigPcb->cfg.sigInt == ctldPcb->suId)
            linkType = IN_LNK_TYPE_D1;
         else if (sigPcb->cfg.bupInt == ctldPcb->suId)
            linkType = IN_LNK_TYPE_D2;
         else 
            linkType =  IN_LNK_TYPE_B;
      }
      else 
      {
         if (sigPcb->cfg.sigInt == ctldPcb->suId)
            linkType = IN_LNK_TYPE_D_NFAS;
         else
            linkType =  IN_LNK_TYPE_B;
      } 
   }     
   else
      linkType = IN_LNK_TYPE_D_PRI;
  
   INDBGP(DBGMASK_MI,"PLM_IN: inDisableLink() found linkType = %d\n", linkType);
   /* clear calls on this link */
   /* switch type of link */
   switch (linkType)
   {
      case IN_LNK_TYPE_D_PRI:
      case IN_LNK_TYPE_D_NFAS:
         callTreatment = IN_CALL_TREATMENT_CLEAR_ALL;
         break;
      case IN_LNK_TYPE_D1:
      case IN_LNK_TYPE_D2:
         /* get status of counterpart */
         if (inGetBkup(sigPcb, &tmpCb) == LCM_REASON_NOT_APPL)
         {
            /* Primary and backup D channels are configured. */
            if (cb->sigState == ST_IS)
            {
               if (tmpCb->sigState == ST_STB)
               {
                  callTreatment = IN_CALL_TREATMENT_CLEAR_SINGLE;
                  /* failover in this case */
                  needDchanSwitch = TRUE;
               } 
               else
               {
                  /* Clear all calls */
                  callTreatment = IN_CALL_TREATMENT_CLEAR_ALL;
               }
            }
            else if (tmpCb->sigState == ST_IS)
               /* only clear calls on this link */
               callTreatment = IN_CALL_TREATMENT_CLEAR_SINGLE;
            else 
               callTreatment = IN_CALL_TREATMENT_CLEAR_SINGLE; /* maybe IN_CALL_TREATMENT_NONE is enough */
         }
         else
         {
            INDBGP(DBGMASK_MI,"inDisableLink: can't find backup link, current rSuId=%ld\n",
ctldPcb->rSuId);
            RETVALUE(LCM_REASON_INVALID_SAP);
         }

         break;
      case IN_LNK_TYPE_B:  
         callTreatment = IN_CALL_TREATMENT_CLEAR_SINGLE;
         break;
      default:
         INDBGP(DBGMASK_MI,"inDisableLink: invalid link type: %d\n", linkType);
         RETVALUE(LCM_REASON_INVALID_SAP);
   }

   /* only D link which was up need to report down */
   if (linkType != IN_LNK_TYPE_B)
   {
      sigState = cb->sigState;
      lnkState = cb->state;
      if (sigPcb->cfg.nfasInt)
      {
         if (((lnkState == CBST1XFER) ||(lnkState == CBST2RST)) &&
             ((sigState == ST_IS) ||(sigState == ST_STB)))
            reportIsReqed = TRUE;
      }
      else if ((lnkState == CBST1XFER) ||(lnkState == CBST2RST))
         reportIsReqed = TRUE;
   }

   switch(callTreatment)
   {
#if 0 /* Coverity fix - can never hit this value */
      case IN_CALL_TREATMENT_NONE:
         break;
#endif
      case IN_CALL_TREATMENT_CLEAR_SINGLE:
         /* just clear calls on this link */       
         if ((ret = inClearCtldCb(sigPcb, ctldPcb, NORMAL_CLEAR) != LCM_REASON_NOT_APPL))
         {
#if (ERRCLASS & ERRCLS_INT_PAR)
            INLOGERROR(ERRCLS_INT_PAR, EIN883, (ErrVal)ret,
                       "inDisableInterface() failed, Failed to Clear Cb");
#endif
            RETVALUE(ret);
         }   
         STOP_CTLDPCB_TIMERS;
         break;
      case IN_CALL_TREATMENT_CLEAR_ALL:
         /* Clear all calls */
         if ((ret = inClearCb(sigPcb, ctldPcb, NORMAL_CLEAR) != LCM_REASON_NOT_APPL))
         {
#if (ERRCLASS & ERRCLS_INT_PAR)
            INLOGERROR(ERRCLS_INT_PAR, EIN883, (ErrVal)ret,
                       "inDisableInterface() failed, Failed to Clear Cb");
#endif
            RETVALUE(ret);
         }
         break;
#if 0 /* Coverity fix - can never hit this value */
      default:
         INDBGP(DBGMASK_MI,"inDisableLink: invalid call treatment: %d, linkType = %d\n", callTreatment, linkType);
         RETVALUE(LCM_REASON_INVALID_SAP);
#endif
   }

   if (needDchanSwitch) /* only works when sigPcb->suId == ctldPcb->suId */
   {
      if (sigPcb->suId != ctldPcb->suId)
      {
         INDBGP(DBGMASK_MI,"inDisableLink: sigPcb->rSuId = %ld, ctldPcb->rSuId = %ld\n",
            sigPcb->rSuId, ctldPcb->rSuId);
      }
      inCtrldDChanSwtchOver(sigPcb, ctldPcb, 0);
   }
   else 
   {
      /* it must be a unique D link's disabling,
         need stop all related ctldPcb and pcb timers */
      if (callTreatment == IN_CALL_TREATMENT_CLEAR_ALL)
      {
         /* stop ctldPcb timers */
         inDisableCtldPcb(sigPcb);
      }
   }
   /* disconnect and unbind LAPD */
   if (linkType != IN_LNK_TYPE_B)
   {
      if (sigPcb->bndState == IN_UNBND)
      {
         INDBGP(DBGMASK_MI,"inDisableLink: rSuId=%ld, sigState=%d was already unbound.  Proceeding...\n",
            cb->rSuId, cb->sigState);
      }

      /* Disable signaling on this link */
      cb->state = CBST3DOWN;
      if (sigPcb->cfg.nfasInt)
         cb->sigState = ST_MOOS;
     
      /* MCAO-9312  stop all Cb timer */
      /* MCAO: don't start timer DISC */
      STOP_CB_TIMERS;
      if (reportIsReqed)
         inNmReport(LCM_CATEGORY_INTERFACE, LCM_EVENT_DOWN,
                    LCM_CAUSE_DCHAN,(U8) DCHAN, cb->suId, (U8) 0);
      /* MCAO-9312 set flag to avoid redundant staInd */
      cb->rstCfmSnt = TRUE; 
      zqRunTimeUpd(ZQ_CES_CB, CMPFTHA_UPD_REQ, (PTR)cb);

      if (sigPcb->bndState != IN_UNBND)
      {
         /* Disconnect layer 2 */
         InLiDatDiscReq(&cb->pst, cb->spId, Q_930, cb->ces, DAT_DM_RLS);
         /* Unbind layer 2 */
         InLiDatUbndReq(&sigPcb->pst,sigPcb->spId, Q_930, 0);
      }
      sigPcb->bndState = IN_UNBND;
      zqRunTimeUpd(ZQ_PSAP_CB, CMPFTHA_UPD_REQ, (PTR)sigPcb);
      zqUpdPeer();     
   }

   return(LCM_REASON_NOT_APPL);
} /* inDisableLink */



/*
*
*      Fun:   inDisableDLSAP
*
*      Desc: 
*
*      Ret:   ROK   - ok
*
*      Notes: None
*
*      File:  qn_bdy11.c
*
*/

#ifdef ANSI
PUBLIC S16 inDisableDLSAP
(
 InCtldPcb *ctldPcb,
 InPCB *sigPcb,
 S16 ces,
 U8 action
 )
#else
     PUBLIC S16 inDisableDLSAP(ctldPcb, sigPcb, ces, action)
     InCtldPcb *ctldPcb;
     InPCB *sigPcb;
     S16 ces;
     U8 action;
#endif
{
   S16 ret;
   InCb *cb;
   U8 tmrNum;
   CalId *ent;
   InCesRst inCesRst;

   TRC3(inDisbleDLSAP);
  
   ret = LCM_REASON_NOT_APPL;

   if ((ret = inGetCb(ctldPcb, sigPcb, ces, &cb)) != LCM_REASON_NOT_APPL)
      RETVALUE(ret);  

   if (sigPcb->cfg.nfasInt)
   {
      if ((ret = inDisableNfasSap(sigPcb, ctldPcb, cb, action))
          != LCM_REASON_NOT_APPL)
      {
         /* if not error */
         if (ret != LCM_REASON_MISC)
            RETVALUE(ret);
      }

      for (tmrNum = 0; tmrNum < MAXBCHTIMER; tmrNum++)
         if (sigPcb->timers[tmrNum].tmrEvnt == TMR_TREST)
            inRmvPcbTq(sigPcb, tmrNum);

      if (cb == NULLP)
         RETVALUE(LCM_REASON_NOT_APPL);
   }
   else
   {
      if (action != ADISIMM)
      {
         RETVALUE(LCM_REASON_INVALID_ACTION);
      }

      /* cb not found */
      if (cb == NULLP)
         RETVALUE(LCM_REASON_INVALID_SAP);

      /* Stop all BCHAN timers  */
      inStopAllBchanTimers(sigPcb);
   }
    
  
   for (tmrNum = 0; tmrNum < MAXSIMTIMER; tmrNum++)
      if (cb->timers[tmrNum].tmrEvnt == TMR_TRSTACK)
         inRmvCbTq(cb, tmrNum);

   if (ret == LCM_REASON_MISC)
   {
      RETVALUE(LCM_REASON_NOT_APPL);
   }

   switch (cb->state)
   {
      case CBST1XFER:

         STOP_SPID_TIMERS;
      
         /* clear all suspended calls */
         if (cb->numCalId)
            while (cb->numCalId)
            {
               ent = inGetCalEntry(cb, 0);
               if (ent->ntc->state == ST_NULL)
               {
                  inRelChan(ent->ntc);
                  inRelCalRef(ent->ntc);                                   
               }
            }
         
      case CBST0CON:
         /* Clear Calls */
         inClearAllCalls(cb, ALL_CALLS);
         inCesRst.intId = sigPcb->suId;
         inCesRst.ces = cb->ces;
         inCesRst.calls = ALL_CALLS;
         zqRunTimeUpd(ZQ_CES_DELNTC_CB, CMPFTHA_UPD_REQ, (PTR)&inCesRst);
         break;
      case CBST2RST:
         if (!cb->inRstJobFlg)
         {
            /* Stop T_316 */
            for (tmrNum = 0; tmrNum< MAXBCHTIMER;tmrNum++)
               if (ctldPcb->timers[tmrNum].tmrEvnt==TMR_T316)
                  inRmvCtldPcbTq(ctldPcb, tmrNum);
         }
      
         break;
      
      default:
         break;
   } /* switch (cb->state) */


   /* Don't Start Timer in Case of ARST */
   if (cb->state != CBST3DOWN)
   {
      if (action != ARST)
      {
         inStartCbTmr(TMR_DISC, cb);
         cb->tmrCnt = 1; 
      }
   }
   InLiDatDiscReq(&cb->pst, cb->spId, Q_930, cb->ces, DAT_DM_RLS);

   cb->state = CBST3DOWN;

   /* TELICA-BUG:9203-mqin */
   inNmReport(LCM_CATEGORY_INTERFACE, LCM_EVENT_DOWN,
              LCM_CAUSE_DCHAN,(U8) DCHAN, cb->suId, (U8) 0);

   if (action == ARST)
   {
      sigPcb->bndState = IN_UNBND;
      InLiDatUbndReq(&sigPcb->pst,sigPcb->spId, Q_930, 0);
   }

   ctldPcb->state = CBST3DOWN;
 
   zqRunTimeUpd(ZQ_CTLDPSAP_CB, CMPFTHA_UPD_REQ, (PTR)ctldPcb);
   zqRunTimeUpd(ZQ_PSAP_CB, CMPFTHA_UPD_REQ, (PTR)sigPcb);
   zqRunTimeUpd(ZQ_CES_CB, CMPFTHA_UPD_REQ, (PTR)cb);
   zqUpdPeer();

   RETVALUE(LCM_REASON_NOT_APPL);
} /* inDisableDLSAP */





/*
 *
 *      Fun:   inDeleteDLSAP
 *
 *      Desc: 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None
 *
 *      File:  qn_bdy11.c
 *
*/

#ifdef ANSI
PUBLIC S16 inDeleteDLSAP
(
 InCtldPcb *ctldPcb,
 InPCB *sigPcb,
 S16 ces
 )
#else
     PUBLIC S16 inDeleteDLSAP(ctldPcb, sigPcb, ces)
     InCtldPcb *ctldPcb;
     InPCB *sigPcb;
     S16 ces;
#endif
{
   InCb *cb;
   S16 ret;
   S16 i;
   InPCB *bkupPcb;
   U8 tmrNum;
   InPCB *pcb;
   /*in039.310-change, use U16 instead of S16 */
   U16 tmpInt;

   TRC3(inDeleteDLSAP);
  
   if ((ret = inGetCb(ctldPcb, sigPcb, ces, &cb)) != LCM_REASON_NOT_APPL)
      RETVALUE(ret);  

   if (cb == NULLP)
   {
      if (sigPcb->cfg.nfasInt)
      {
         for (i = 0; i < (S16) sigPcb->cfg.maxNmbNfasInt; i++)
            if (*(sigPcb->ctldInt+i) == ctldPcb->suId)
            {
               *(sigPcb->ctldInt+i) = INT_NOT_CFGD;
               sigPcb->nmbCtldInt--;
               break;
            }

         if (sigPcb->cfg.buIntPr)
         {
            if (sigPcb->suId == sigPcb->cfg.bupInt) 
               tmpInt = sigPcb->cfg.sigInt;
            else
               tmpInt = sigPcb->cfg.bupInt;

            /* get physical link control block of backup chnnl*/
            if ((bkupPcb = *(pcbLstPtr + tmpInt))== NULLP)
            {
#if (ERRCLASS & ERRCLS_INT_PAR)
               INLOGERROR(ERRCLS_INT_PAR, EIN726, (ErrVal)tmpInt,
                          "InMiLinCntrlReq() failed, unable to access DLSAP");
#endif
               RETVALUE(LCM_REASON_INVALID_SAP);                           
            }

            for (i = 0; i < (S16) bkupPcb->cfg.maxNmbNfasInt; i++)
               if (*(bkupPcb->ctldInt+i) == ctldPcb->suId)
               {
                  *(bkupPcb->ctldInt+i) = INT_NOT_CFGD;
                  bkupPcb->nmbCtldInt--;
                  break;
               }
         }

         if ((ret = inFreeCtldPcb(ctldPcb)) != ROK)
         {
            RETVALUE(LCM_REASON_MISC_FAILURE);
         }
      }
   }
   else
   {
      if (sigPcb->cfg.nfasInt)
         cb->sigState = ST_MOOS;

      if ((cb->numCalRef) || (cb->numCalId))
      {
         cb->inUnBndFlg = TRUE;
         RETVALUE(LCM_REASON_NOT_APPL);
      }

      if (cb->mBuf)
         inDropData(cb->mBuf);
  
      for (tmrNum = 0; tmrNum< MAXSIMTIMER;tmrNum++)
         if (cb->timers[tmrNum].tmrEvnt == TMR_DISC)
         {
            /* Set Flag to unbind later */
            cb->inUnBndFlg = TRUE;
            RETVALUE(LCM_REASON_NOT_APPL);
         }

      /* Stop all timers */
      STOP_CB_TIMERS;

      pcb = sigPcb;
      STOP_PCB_TIMERS;
    
      /* Calls inFreePcb if required */
      if ((ret= inUnBndCb(sigPcb,cb)) != ROK)
      {
         RETVALUE(LCM_REASON_MISC_FAILURE);
      }
   }
   RETVALUE(LCM_REASON_NOT_APPL);
} /* end of inDeleteDLSAP */



/*
*
*      Fun:   inDeleteLink
*
*      Desc: 
*
*      Ret:   ROK   - ok
*
*      Notes: None
*
*      File:  qn_bdy11.c
*
*/

PUBLIC S16 inDeleteLink
(
 InCtldPcb *ctldPcb,
 InPCB *sigPcb,
 S16 ces
 )
{
   InCb *cb;
   S16 ret;
   S16 i;
   U8 tmrNum;
   InPCB *pcb;
   U8 linkType = IN_LNK_TYPE_UNKNOWN;

   TRC3(inDeleteLink);

   /* B link w/o signaling link case */
   if (ctldPcb->cfg.nfasInt && !sigPcb)
   {
      INDBGP(DBGMASK_MI,"PLM_IN: no signaling link exist, removed ctldPcb...\n");
      if ((ret = inFreeCtldPcb(ctldPcb)) != ROK)
      {
         RETVALUE(LCM_REASON_MISC_FAILURE);
      }
    
      RETVALUE(LCM_REASON_NOT_APPL);
   }

   if (!sigPcb)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      INLOGERROR(ERRCLS_INT_PAR, EIN708, (ErrVal)ctldPcb->rSuId,
                 "inDisableLink() failed, invalid DLSAP.");
#endif
      RETVALUE(LCM_REASON_INVALID_SAP);  
   }

   /* Get cb */
   if (!(cb = *(sigPcb->cesLst + (SuId) ces)))
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      INLOGERROR(ERRCLS_INT_PAR, EIN710, (ErrVal)ces,
                 "InGetCb() failed, unable to access SAP.");
#endif
      RETVALUE(LCM_REASON_INVALID_SAP);                      
   }

   /* identify type of link */
   if (sigPcb->cfg.nfasInt) 
   { 
      if (sigPcb->cfg.buIntPr)
      {
         if (sigPcb->cfg.sigInt == ctldPcb->suId)
            linkType = IN_LNK_TYPE_D1;
         else if (sigPcb->cfg.bupInt == ctldPcb->suId)
            linkType = IN_LNK_TYPE_D2;
         else 
            linkType =  IN_LNK_TYPE_B;
      }
      else 
      {
         if (sigPcb->cfg.sigInt == ctldPcb->suId)
            linkType = IN_LNK_TYPE_D_NFAS;
         else
            linkType =  IN_LNK_TYPE_B;
      } 
   }     
   else
      linkType = IN_LNK_TYPE_D_PRI;

  /* deassociate ctldInt[] list */
   inDeAssociateCtldPcb(ctldPcb);

   /* remove control blocks */
   switch(linkType)
   {
      case IN_LNK_TYPE_D_PRI:
      case IN_LNK_TYPE_D_NFAS:
      case IN_LNK_TYPE_D1:
      case IN_LNK_TYPE_D2:
         if ((cb->numCalRef) || (cb->numCalId))
         {
            cb->inUnBndFlg = TRUE;
#if (ERRCLASS & ERRCLS_ADD_RES)
            INLOGERROR(ERRCLS_ADD_RES, EIN839, (cb->numCalRef),
                       "inDeleteLink() failed, found active calls.");
#endif
            RETVALUE(LCM_REASON_NOT_APPL);
         }
      
         if (cb->mBuf)
            inDropData(cb->mBuf);
  
         /* Stop all timers */
         STOP_CB_TIMERS;

         pcb = sigPcb;
         STOP_PCB_TIMERS;

         /* release cb */
         if ((ret = SPutSBuf(inInit.region, inInit.pool,
                             cb, (Size) sizeof(InCb))) != ROK)
         {
#if (ERRCLASS & ERRCLS_ADD_RES)
            INLOGERROR(ERRCLS_ADD_RES, EIN839, ERRZERO,
                       "inUnBndCb() failed, unable to release memory.");
#endif
         }
         *(sigPcb->cesLst + ces) = NULLP;

         /* double check whether there are live cb attached  */
         for (i = 0; i < sigPcb->cfg.nmbCes; i++)
            if ((cb = *(sigPcb->cesLst + i)) != NULLP)
            {
#if (ERRCLASS & ERRCLS_ADD_RES)
               INLOGERROR(ERRCLS_ADD_RES, EIN839, ERRZERO,
                          "inUnBndCb() failed, unable to release memory.");
#endif 
            }
         /* may need double check whether pcb unbound, refer to inUnBndCb() */

         /* release pointers */
         if ((ret = SPutSBuf(inInit.region, inInit.pool, 
                             sigPcb->cesLst, (Size) sizeof(InCb *))) != ROK)
         {
#if (ERRCLASS & ERRCLS_ADD_RES)
            INLOGERROR(ERRCLS_ADD_RES, EIN840, ERRZERO,
                       "inUnBndCb() failed, unable to release memory.");
#endif
         }

         /* Calls inFreePcb if required */
         for (tmrNum = 0; tmrNum < MAXBCHTIMER; tmrNum++)
            if (ctldPcb->timers[tmrNum].tmrEvnt != TMR_NONE)
               inRmvCtldPcbTq(ctldPcb, tmrNum);

         inFreePcb(pcb);
      
         break;
      case IN_LNK_TYPE_B:
         if ((ret = inFreeCtldPcb(ctldPcb)) != ROK)
         {
            RETVALUE(LCM_REASON_MISC_FAILURE);
         }

         break;
      default:
         INDBGP(DBGMASK_MI,"inDisableLink: invalid link type: %d\n", linkType);
         RETVALUE(LCM_REASON_INVALID_SAP);
   }

   RETVALUE(LCM_REASON_NOT_APPL);
} /* end of inDeleteLink */



/*
 *
 *      Fun:   inBndEnableSap
 *
 *      Desc: 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None
 *
 *      File:  qn_bdy11.c
 *
*/
PUBLIC S16 inBndEnableSap
(
 InPCB *pcb,
 U8 action
 )
{
#ifndef DAT2
   InCb *cb;
   S16 j;
   U8 tmrNum;
#endif
   /* PLM_IN_UPG */
   U8 oldBndState;
   InCtldPcb *ctldPcb;

   TRC3(inBndEnableSap);

   INDBGP(DBGMASK_MI,"inBndEnableSap rSuId=%ld, spId=%d, bndState=%d, srcProcId=%04X, srcEnt=%d, dstProcId=%04X, dstEnt=%d, route=%d, action = %d\n",
      pcb->rSuId, pcb->spId, pcb->bndState, pcb->pst.srcProcId, pcb->pst.srcEnt, pcb->pst.dstProcId, pcb->pst.dstEnt, pcb->pst.route, action);

   /* Bug 90003: Durign GR failover, it needs to force ISDN link bind one time. */
   if((qwCp.geoAuditState == CCT_GEO_AUD_STATE_INPROGRESS) && (pcb->gr_bnd == FALSE))
   {
      pcb->bndState = IN_UNBND;
      pcb->gr_bnd = TRUE;
   }

   /* PLM_IN_UPG */
   oldBndState = pcb->bndState;
   if (action != ABND)
   {
      if (pcb->bndState == IN_BND)
      {
          /* BUG 43595 mpralat 2006-01-19 */
          /* Bound but maybe not enabled */
          RETVALUE(LCM_PRIM_OK_NDONE);
      }
  
#ifdef DAT2
#ifdef TELICA
      if (pcb->bndState == IN_WAIT_BNDCFM)
      {
         pcb->bndRetryCnt = 0;  /* start over */
         RETVALUE(LCM_REASON_NOT_APPL);
      }
#else
      if (pcb->bndState == IN_WAIT_BNDCFM)
         RETVALUE(LCM_REASON_NOT_APPL);
#endif
#endif
   }

   if ((ctldPcb = *(ctldPcbLstPtr + pcb->suId)) == NULLP)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      INLOGERROR(ERRCLS_INT_PAR, EIN727, 
                 (ErrVal)pcb->rSuId,
                 "InBndEnable() failed, unable to access SAP.");
#endif
      RETVALUE(LCM_REASON_INVALID_SAP);
   }


#ifdef DAT2
   pcb->bndState = IN_WAIT_BNDCFM;
   inStartPcbTmr(TMR_TINT, pcb);
#else
   pcb->bndState = IN_BND;
   zqRunTimeUpd(ZQ_PSAP_CB, CMPFTHA_UPD_REQ, (PTR)pcb);
#endif /* DAT2 */

   InLiDatBndReq(&pcb->pst, pcb->suId, pcb->spId, Q_930);

#ifndef DAT2
   for (j=0; j < pcb->cfg.nmbCes; j++)
   {
      if (!(cb = *(pcb->cesLst + (SuId) j)))
      {
#if (ERRCLASS & ERRCLS_INT_PAR)
         INLOGERROR(ERRCLS_INT_PAR, EIN728, 
                    (ErrVal)j,
                    "InBndEnable() failed, unable to access SAP.");
#endif
         continue;
      }
      /* PLM_IN_UPG: don't need reconnect this link if it's already bound */
      if (oldBndState == IN_BND)
         continue;

      if (cb->state != CBST3DOWN)
      {
#if (ERRCLASS & ERRCLS_INT_PAR)
         INLOGERROR(ERRCLS_INT_PAR, EIN729, (ErrVal)cb->state ,
                    "InBndEnable() failed, Link Not in Down State.");
#endif       
         continue;
      }
    
    
      for (tmrNum = 0;tmrNum < MAXSIMTIMER;tmrNum++)
         if (cb->timers[tmrNum].tmrEvnt == TMR_DISC)
            inRmvCbTq(cb, tmrNum);
    
      inStartCbTmr(TMR_CON, cb);
      cb->state = CBST0CON;    
      cb->rstCfmSnt = FALSE;

      if (pcb->cfg.nfasInt)
         cb->sigState = ST_MB;

      InLiDatConReq(&cb->pst, cb->spId, Q_930, cb->ces, cb->rSuId);
   }

   ctldPcb->state = CBST1XFER;
   zqRunTimeUpd(ZQ_CTLDPSAP_CB, CMPFTHA_UPD_REQ, (PTR)ctldPcb);
   zqUpdPeer();
#endif 

   RETVALUE(LCM_REASON_NOT_APPL);
} /* end of inBndEnableSap */



/*
 *
 *      Fun:   inUBndDisDLSAP
 *
 *      Desc: 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None
 *
 *      File:  qn_bdy11.c
 *
*/

#ifdef ANSI
PUBLIC S16 inUBndDisDLSAP
(
 InPCB *sigPcb,
 InCtldPcb *ctldPcb,
 U8 clrCalls
 )
#else
     PUBLIC S16 inUBndDisDLSAP(sigPcb, ctldPcb, clrCalls)
     InPCB *sigPcb;
     InCtldPcb *ctldPcb;
     U8 clrCalls;
#endif
{
   S16 ret;
   InCb *cb;
   S16 i;
   Bool clrAllCtldPcb;
   InCb *bkupCb;
   InCtldPcb *tmpCtldPcb;
   U8 svdSigStat;           /* saved cb->sigState */
   U8 svdBkpStat;           /* saved backup link sigState */
   InCtldPSapRst inCtldPcbRst;
   U8 tmrNum;

   TRC3(inUBndDisDLSAP);

   if (sigPcb->bndState == IN_UNBND)
      RETVALUE(LCM_REASON_NOT_APPL);

   if (ctldPcb->suId != sigPcb->suId)
      RETVALUE(LCM_REASON_INVALID_ACTION);

  /* get Cb */
   if ((ret = inGetCb(ctldPcb, sigPcb, 0, &cb)) != LCM_REASON_NOT_APPL)
      RETVALUE(ret);

   svdSigStat = cb->sigState;
   svdBkpStat = ST_OOS;

  /* This part of the code i.e. the check for clrCalls 
     is specifically for FT/HA architecture. 
     During the conventional unbind-disable all calls are lost. To 
     prevent this in the FT/HA switchover case the LM can set the 
     clrCalls parameter in the control request to NON_ACTIVE_CALLS 
     so that only non active calls are lost in this case. */

  /* TELICA-jwh-09/06/2001:  Only pace when a slot goes down */
   if ((clrCalls != FTHA_CLEAR) && (clrCalls != NORMAL_CLEAR_PACE))
      clrCalls = NORMAL_CLEAR;
  
   if ( (clrCalls == FTHA_CLEAR) || !(sigPcb->cfg.nfasInt) || 
        ( (sigPcb->suId == sigPcb->cfg.sigInt) && (!(sigPcb->cfg.buIntPr)) ) )
   { 
      if ((ret = inClearCb(sigPcb, ctldPcb, clrCalls) 
           != LCM_REASON_NOT_APPL))
      {
#if (ERRCLASS & ERRCLS_INT_PAR)
         INLOGERROR(ERRCLS_INT_PAR, EIN883, (ErrVal)ret,
                    "inUBndDisDLSAP() failed, Failed to Clear Cb");
#endif
         RETVALUE(ret);
      }
   }
   else
   {
      /* NFAS with Backup */
      /* Get Backup */
      if ((ret = inGetBkup(sigPcb, &bkupCb)) != LCM_REASON_NOT_APPL)
         RETVALUE(ret);
      svdBkpStat = bkupCb->sigState;

      if (cb->sigState == ST_IS)
      {
         if (bkupCb->sigState == ST_STB)
         {
            if ((ret = inActivateBkup(cb, bkupCb, sigPcb, ctldPcb)) 
                != LCM_REASON_NOT_APPL)
            {
#if (ERRCLASS & ERRCLS_INT_PAR)
               INLOGERROR(ERRCLS_INT_PAR, EIN884, (ErrVal)ret,
                          "inUBndDisDLSAP() failed, failed to activate bkup ");
#endif
               RETVALUE(ret);
            }    
         }
         else
         {
            if ((ret = inClearCb(sigPcb, ctldPcb, clrCalls) 
                 != LCM_REASON_NOT_APPL))
            {
#if (ERRCLASS & ERRCLS_INT_PAR)
               INLOGERROR(ERRCLS_INT_PAR, EIN885, (ErrVal)ret,
                          "inUBndDisDLSAP() failed, failed to clear cb");
#endif
               RETVALUE(ret);
            }    
         } /* end if bkup is ST_STB */
      } /* cb->sigState == ST_IS */
      else
      {
         inClearBChanCalls(ctldPcb, NULLP, cb, AUBND_DIS);
         cb->sigState = ST_OOS;
         cb->state = CBST3DOWN;
         inCtldPcbRst.suId = ctldPcb->suId;
         inCtldPcbRst.calls = ALL_CALLS;
         zqRunTimeUpd(ZQ_CTLDPSAP_DELNTC_CB, CMPFTHA_UPD_REQ, 
                      (PTR)&inCtldPcbRst);
         zqRunTimeUpd(ZQ_CTLDPSAP_CB, CMPFTHA_UPD_REQ, (PTR)ctldPcb);    
      }
   } /* nfas with back up */

   if ((sigPcb->cfg.nfasInt) && (svdSigStat == ST_IS))
   {
      /* If the signalling interface is getting disabled and there is
         no backup, reset state of  controlled interfaces */

      clrAllCtldPcb = FALSE;
    
      if (clrCalls == FTHA_CLEAR) 
         clrAllCtldPcb = TRUE;
      else
      {
         if ( (sigPcb->suId == sigPcb->cfg.sigInt) && 
              (!(sigPcb->cfg.buIntPr)) )
            clrAllCtldPcb = TRUE;
         else 
         {
            if (svdBkpStat != ST_STB)
               clrAllCtldPcb = TRUE;
            else 
            {
               if (cb->pst.dstProcId == bkupCb->pst.dstProcId)
                  clrAllCtldPcb = TRUE;
            }
         }
      } /* not FTHA_CLEAR */

      if (clrAllCtldPcb == TRUE)
      {
         for (i = 0; i < sigPcb->cfg.maxNmbNfasInt; i++)
         {
            if ((*(sigPcb->ctldInt+i) != INT_NOT_CFGD) &&
                (( tmpCtldPcb = *(ctldPcbLstPtr + 
                                  *(sigPcb->ctldInt+i)) ) != NULLP) &&
                (tmpCtldPcb->suId != ctldPcb->suId))
            {
               if ( tmpCtldPcb->state != CBST3DOWN)
               {
                  tmpCtldPcb->state = CBST3DOWN;
                  zqRunTimeUpd(ZQ_CTLDPSAP_CB, CMPFTHA_UPD_REQ, (PTR)tmpCtldPcb);
               }
            }
         } /* for */
      } /* if sigInt */
   } /* nfas && ST_IS */

   ctldPcb->state = CBST3DOWN;
   sigPcb->bndState = IN_UNBND;
   for (i=0; i < sigPcb->cfg.nmbCes; i++)
   {
      STOP_CB_TIMERS;
   }
   for(tmrNum=0;tmrNum<MAXBCHTIMER;tmrNum++)
   {
      if (sigPcb->timers[tmrNum].tmrEvnt != TMR_NONE) 
      {
         inRmvPcbTq(sigPcb, tmrNum);
      }
   }
   STOP_CTLDPCB_TIMERS;

   zqRunTimeUpd(ZQ_CTLDPSAP_CB, CMPFTHA_UPD_REQ, (PTR)ctldPcb);
   zqRunTimeUpd(ZQ_PSAP_CB, CMPFTHA_UPD_REQ, (PTR)sigPcb);
   zqRunTimeUpd(ZQ_CES_CB, CMPFTHA_UPD_REQ, (PTR)cb);
   zqUpdPeer();

   RETVALUE(LCM_REASON_NOT_APPL);
} /* end of inUBndDisDLSAP */




/*
 *
 *      Fun:   inDeleteTSAP
 *
 *      Desc: 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None
 *
 *      File:  qn_bdy11.c
 *
*/

#ifdef ANSI
PUBLIC S16 inDeleteTSAP
(
 SpId spId
 )
#else
     PUBLIC S16 inDeleteTSAP(spId)
     SpId spId;
#endif
{
   InCb *tCb;
   S16 ret;
  
   TRC3(inDeleteTSAP);

   if ((tCb = inGetSapPtr(spId)) == NULLP)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      INLOGERROR(ERRCLS_INT_PAR, EIN730, (ErrVal) spId,
                 "inDeleteTSAP() failed, invalid parameter.");
#endif
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

   if (tCb->state != CBST3DOWN)
   {
      if ((ret = inUnBndTSAP(spId)) != LCM_REASON_NOT_APPL)
         RETVALUE(ret);
   }

   *(inSapLstPtr + tCb->spId) = NULLP;
   SPutSBuf(inInit.region, inInit.pool, tCb, (Size) sizeof(InCb));

   RETVALUE(LCM_REASON_NOT_APPL);
} /* end of inDeleteTSAP */





/*
 *
 *      Fun:   inUBndDisTSAP
 *
 *      Desc: 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None
 *
 *      File:  qn_bdy11.c
 *
*/

#ifdef ANSI
PUBLIC S16 inUBndDisTSAP
(
 SpId spId
 )
#else
     PUBLIC S16 inUBndDisTSAP(spId)
     SpId spId;
#endif
{
   InCb *tCb;
   S16 ret;

   TRC3(inUnBndDisTSAP);

   if ((tCb = inGetSapPtr(spId)) == NULLP)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      INLOGERROR(ERRCLS_INT_PAR, EIN731, (ErrVal) spId,
                 "inDeleteTSAP() failed, invalid parameter.");
#endif
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

   if (tCb->state == CBST3DOWN)
      RETVALUE(LCM_REASON_NOT_APPL);
  
   if ((ret = inUnBndTSAP(spId)) != LCM_REASON_NOT_APPL)
      RETVALUE(ret);

   RETVALUE(LCM_REASON_NOT_APPL);
} /* end of inUBndDisTSAP */

/* TELICA-jwh-01/04/2001:  BCHANSYNC:  Add logic to handle B channel maintenance request. */
/**********************************************************************
 ** inHandleBChanReq
 **
 **     Desc:  This function 
 **
 **     Ret:   ROK     - successful
 **            RFAILED - failed
 **    
**********************************************************************/
#ifdef IN_PBD /* ISDN Persistent B/D Channel Status, H.Kim */

#ifdef ANSI
PUBLIC void inHandleBChanReq
(
 InCtldPcb *ctldPcb,
 InPCB *sigPcb,
 InBearChn *bearChn,
 U8 action,
 U8 mntStat
 )
#else
     PUBLIC void inHandleBChanReq(cb, tCb, ctldPcb, sigPcb, bearChn, action, mntStat)
     InCtldPcb *ctldPcb;
     InPCB *sigPcb;
     InBearChn *bearChn;
     U8 action;
     U8 mntStat;
#endif

#else /* original code */

#ifdef ANSI
     PUBLIC void inHandleBChanReq
     (
      InCtldPcb *ctldPcb,
      InPCB *sigPcb,
      InBearChn *bearChn,
      U8 action
      )
#else
     PUBLIC void inHandleBChanReq(cb, tCb, ctldPcb, sigPcb, bearChn, action)
     InCtldPcb *ctldPcb;
     InPCB *sigPcb;
     InBearChn *bearChn;
     U8 action;
#endif

#endif /* IN_PBD */
{
   int actRst = FALSE;
   int actLog = FALSE;
   int callPresent = ((bearChn->ntc == NULL)?FALSE:TRUE);

   TRC2(inHandleBChanReq)

   switch (action)
   {
      case ADISIMM:  /* Maintenance block */
         switch(bearChn->status)
         {
            case AVAIL:
            case OUT_SERVICE_FE:
            case MAINT_FE:
               if (callPresent)
               {
                  actLog = TRUE;
               }
               bearChn->status = MAINT_NE;
               break;

            case IN_SERVICE:
               if (!callPresent)
               {
                  actLog = TRUE;
               }
               bearChn->status = MAINT_NE;
               break;

            case OUT_SERVICE_NE:
            case MAINT_NE:
               break;
          
            default: /* NOTPROVISIONED */
               gitDebugPrint(__FILE__, __LINE__, "inHandleBChanReq:  Invalid B channel state=%s, chan=%d, suId=%d, ntc=%p\n",
                             bChanStatusStr(bearChn->status), bearChn->chnNmb, bearChn->intId, bearChn->ntc);
               break;
         }
         break;

      case AENA:  /* Maintenance unblock */
         switch(bearChn->status)
         {
            case AVAIL:
               if (callPresent)
               {
                  actLog = TRUE;
               }
               break;

            case IN_SERVICE:
               if (!callPresent)
               {
                  actRst = TRUE;
                  actLog = TRUE;
               }
               break;

            case OUT_SERVICE_NE:
               break;

            case OUT_SERVICE_FE:
            case MAINT_FE:
               actRst = TRUE;
               if (callPresent)
               {
                  actLog = TRUE;
               }
               break;

            case MAINT_NE:
               if (callPresent)
               {
                  bearChn->status = IN_SERVICE;
               }
               else
               {
                  actRst = TRUE;
               }
               break;
          
            default: /* NOTPROVISIONED */
               gitDebugPrint(__FILE__, __LINE__, "inHandleBChanReq:  Invalid B channel state=%s, chan=%d, suId=%d, ntc=%p\n",
                             bChanStatusStr(bearChn->status), bearChn->chnNmb, bearChn->intId, bearChn->ntc);
               break;
         }
         break;

      case AINH:   /* Hardware block */
         switch(bearChn->status)
         {
            case AVAIL:
            case OUT_SERVICE_FE:
            case MAINT_FE:
               if (callPresent)
               {
                  actLog = TRUE;
               }
               bearChn->status = OUT_SERVICE_NE;
               break;

            case IN_SERVICE:
               if (!callPresent)
               {
                  actLog = TRUE;
               }
               bearChn->status = OUT_SERVICE_NE;
               break;

            case OUT_SERVICE_NE:
               if (callPresent)
               {
                  actLog = TRUE;
               }
               break;

            case MAINT_NE:
               bearChn->status = OUT_SERVICE_NE;
               break;
          
            default: /* NOTPROVISIONED */
               gitDebugPrint(__FILE__, __LINE__, "inHandleBChanReq:  Invalid B channel state=%s, chan=%d, suId=%d, ntc=%p\n",
                             bChanStatusStr(bearChn->status), bearChn->chnNmb, bearChn->intId, bearChn->ntc);
               break;
         }
         break;

      case AUNINH:   /* Hardware unblock */
         switch(bearChn->status)
         {
            case AVAIL:
               if (callPresent)
               {
                  actRst = TRUE;
                  actLog = TRUE;
               }
               break;

            case IN_SERVICE:
               if (!callPresent)
               {
                  actRst = TRUE;
                  actLog = TRUE;
               }
               break;

            case OUT_SERVICE_NE:
               if (!callPresent)
               {
                  if(mntStat)
                  {
                     actRst = FALSE;
                     bearChn->status = MAINT_NE;
                  }
                  else
                  {
                     actRst = TRUE;
                  }
               }
               else
               {
                  bearChn->status = IN_SERVICE;
               }
               break;

            case MAINT_NE:
               if(mntStat)
               {
                  /* gitDebugPrint(__FILE__,__LINE__,"HKIM : instead of reset, set it to MAINT_NE\n"); */
               }
               else
               {
                  actRst = FALSE;
               }
               break;

            case OUT_SERVICE_FE:
            case MAINT_FE:
               actRst = TRUE;
               break;
          
            default: /* NOTPROVISIONED */
               gitDebugPrint(__FILE__, __LINE__, "inHandleBChanReq:  Invalid B channel state=%s, chan=%d, suId=%d, ntc=%p\n",
                             bChanStatusStr(bearChn->status), bearChn->chnNmb, bearChn->intId, bearChn->ntc);
         }
         break;

      default:
         gitDebugPrint(__FILE__, __LINE__, "inHandleBChanReq:  Invalid action=%d state=%s, chan=%d, suId=%d, ntc=%p\n",
                       action, bChanStatusStr(bearChn->status), bearChn->chnNmb, bearChn->intId, bearChn->ntc);
   }

   /* Stop B channel restart timer, if any, when we are blocking a B channel */
   if ((action == AINH) || (action == ADISIMM))
   {
      U8 tmrNum;
   
      for (tmrNum = 0; tmrNum < MAXBCHTIMER; tmrNum++)
         if (bearChn->timers[tmrNum].tmrEvnt == TMR_T316C)
            inRmvBChTq(bearChn, tmrNum);

      bearChn->rst = FALSE;
   }

   if (actLog == TRUE)  /* Uexpected state.  Log message. */
   {
      gitDebugPrint(__FILE__, __LINE__, "Warning:  unexpected state, action=%d state=%s, chan=%d, suId=%d, ntc=%p\n"
                    "     sigPcb->rSuId=%ld, ctldPcb->rSuId=%ld, dChannelNum=%d\n",
                    action, bChanStatusStr(bearChn->status), bearChn->chnNmb, bearChn->intId, bearChn->ntc,
                    sigPcb->rSuId, ctldPcb->rSuId, sigPcb->cfg.dChannelNum);
   }

   if (actRst == TRUE)  /* Send restart request for this B channel */
   {
      if (!sigPcb->cfg.rstOpt)
      {
         InCb *cb;

         /* Bug 70294 */
         cb = inGetSigCb(sigPcb);

         if (cb != NULLP)
         {
            if (cb->state == CBST1XFER)
               bearChn->status = AVAIL;
         }
      }
      else
      {
         InCb *cb;
         S16 ret = LCM_REASON_NOT_APPL;

         /* Find currently active D channel */
         cb = inGetSigCb(sigPcb);
         if (cb != NULLP)
         {
            switch (sigPcb->cfg.swtch)
            {
               /* NOTE:  Put variant specific action here Max */
                  
               case SW_NI2:
               case SW_ETSI:
               default:
                  ret = inRestartChan(sigPcb, ctldPcb, cb, bearChn->chnNmb);
            }
         }
          
         if (ret != LCM_REASON_NOT_APPL)
         {
            INDBGP(DBGMASK_MI,"inHandleBChanReq: restart %s, ret=%d, action=%d state=%s, chan=%d, suId=%d, ntc=%p, rSuId=%ld\n",
               (ret == ROKIGNORE)? "ignored":"failed", ret, action, bChanStatusStr(bearChn->status),
               bearChn->chnNmb, bearChn->intId, bearChn->ntc, sigPcb->rSuId);

         }
      }
   }
}    /* End of inHandleBChanReq */



/**********************************************************************
** inHandleBChanReqNoSrv
**
**     Desc:  This function 
**
**     Ret:   ROK     - successful
**            RFAILED - failed
**    
**********************************************************************/
#ifdef ANSI
PUBLIC void inHandleBChanReqNoSrv
(
 InCb *cb,
 InCb *tCb,
 InCtldPcb *ctldPcb,
 InPCB *sigPcb,
 InBearChn *bearChn,
 U8 action
 )
#else
     PUBLIC void inHandleBChanReqNoSrv(cb, tCb, ctldPcb, sigPcb, bearChn, action)
     InCb *cb,
  InCb *tCb,
  InCtldPcb *ctldPcb;
     InPCB *sigPcb;
     InBearChn *bearChn;
     U8 action;
#endif
{
#ifndef IN_PBD
  inHandleBChanReq (ctldPcb, sigPcb, bearChn, action);
#endif
}



/**********************************************************************
** inHandleBChanReq
**
**     Desc:  This function 
**
**     Ret:   ROK     - successful
**            RFAILED - failed
**    
**********************************************************************/
#ifdef IN_PBD /* ISDN Persistent B/D Channel, H.Kim */

#ifdef ANSI
PUBLIC void inHandleBChanReqSrv
(
 InCb *cb,
 InCb *tCb,
 InCtldPcb *ctldPcb,
 InPCB *sigPcb,
 InBearChn *bearChn,
 U8 action,
 U8 mntStat
 )
#else
     PUBLIC void inHandleBChanReqSrv(cb, tCb, ctldPcb, sigPcb, bearChn, action, mntStat)
     InCb *cb,
  InCb *tCb,
  InCtldPcb *ctldPcb;
     InPCB *sigPcb;
     InBearChn *bearChn;
     U8 action;
     U8 mntStat;
#endif

#else /* original code */

#ifdef ANSI
     PUBLIC void inHandleBChanReqSrv
     (
      InCb *cb,
      InCb *tCb,
      InCtldPcb *ctldPcb,
      InPCB *sigPcb,
      InBearChn *bearChn,
      U8 action
      )
#else
     PUBLIC void inHandleBChanReqSrv(cb, tCb, ctldPcb, sigPcb, bearChn, action)
     InCb *cb,
       InCb *tCb,
       InCtldPcb *ctldPcb;
     InPCB *sigPcb;
     InBearChn *bearChn;
     U8 action;
#endif

#endif /* IN_PBD */
{
   int actRst = FALSE;
   int actLog = FALSE;
   int callPresent = ((bearChn->ntc == NULL)?FALSE:TRUE);
   U8 tmrNum;
   ChanId chanId;

   TRC3(inHandleBChanReqSrv);
  

   if (bearChn->waitSrvAck)
   {
      /* Stop T_332 */
      for (tmrNum = 0; tmrNum < MAXBCHTIMER; tmrNum++)
         if (bearChn->timers[tmrNum].tmrEvnt == TMR_T332)
            inRmvBChTq(bearChn, tmrNum);

      bearChn->waitSrvAck = FALSE;
   }
  
   switch (action)
   {
      case ADISIMM:  /* Maintenance block */
         switch(bearChn->status)
         {
            case AVAIL:
            case IN_SERVICE:
            case OUT_SERVICE_FE:
            case MAINT_NE: /* more check needed */
               if (((bearChn->status == AVAIL) && callPresent)
                   || ((bearChn->status == IN_SERVICE) && !callPresent)
                   || ((bearChn->status == OUT_SERVICE_FE) && callPresent))
               {
                  actLog = TRUE;
               }
               /* don't send service message if bchan already in MAINT_NE status */
      
               if (bearChn->status == MAINT_NE) 
                  break;

               /* TELICA-MCAO-PRJ-NI2-SRV_MSG-- Q: Shall we stop T332 individually? */  
               bearChn->suId = tCb->spId;
               bearChn->suIdPres = TRUE;
               bearChn->pendStat = NS_MAINT;
               bearChn->status = MAINT_NE;
      
               if (cb == NULLP)
                  break;
      
               inFillChanId (cb, ctldPcb, &chanId);
               chanId.chanNmbSlotMap.val[0] = bearChn->chnNmb;
               bearChn->waitSrvAck = TRUE;
               inGenSrvReq (cb, &chanId, NS_OUTSRV);
               break;

            case OUT_SERVICE_NE:
               break;
        
            case  MAINT_FE:
               actRst = TRUE;
               if (callPresent)
               {
                  actLog = TRUE;
               }
               break;

            default: /* NOTPROVISIONED */
               gitDebugPrint(__FILE__, __LINE__, "inHandleBChanReq:  Invalid B channel state=%s, chan=%d, suId=%d, ntc=%p\n",
                             bChanStatusStr(bearChn->status), bearChn->chnNmb, bearChn->intId, bearChn->ntc);
               break;
         }
         break;

      case AINH:   /* Hardware block */
         switch(bearChn->status)
         {
            case AVAIL:
            case OUT_SERVICE_FE:
               if (callPresent)
               {
                  actLog = TRUE;
               }

               bearChn->suId = tCb->spId;
               bearChn->suIdPres = TRUE;
               bearChn->pendStat = NS_OUTSRV;
               bearChn->status = OUT_SERVICE_NE;
        
               if (cb == NULLP)
                  break;

               inFillChanId (cb, ctldPcb, &chanId);
               chanId.chanNmbSlotMap.val[0] = bearChn->chnNmb;
               inGenSrvReq (cb, &chanId, NS_OUTSRV);
               break;

            case IN_SERVICE:
               if (!callPresent)
               {
                  actLog = TRUE;
               }

               bearChn->suId = tCb->spId;
               bearChn->suIdPres = TRUE;
               bearChn->pendStat = NS_OUTSRV;
               bearChn->status = OUT_SERVICE_NE;
        
               if (cb == NULLP)
                  break;

               inFillChanId (cb, ctldPcb, &chanId);
               chanId.chanNmbSlotMap.val[0] = bearChn->chnNmb;
               inGenSrvReq (cb, &chanId, NS_OUTSRV);
               break;

            case OUT_SERVICE_NE:
               if (callPresent)
               {
                  actLog = TRUE;
               }
               break;

            case MAINT_NE:
               /* Do not send OOS ServiceMsg if this B-channel is already OOS */
               bearChn->status = OUT_SERVICE_NE;
               bearChn->pendStat = NS_OUTSRV;
               break; 
      
            case MAINT_FE:
               actRst = TRUE;
               if (callPresent)
               {
                  actLog = TRUE;
               }  
               break;
            default: /* NOTPROVISIONED */
               gitDebugPrint(__FILE__, __LINE__, "inHandleBChanReq:  Invalid B channel state=%s, chan=%d, suId=%d, ntc=%p\n",
                             bChanStatusStr(bearChn->status), bearChn->chnNmb, bearChn->intId, bearChn->ntc);
               break;
         }
         break;

      case AENA:  /* Maintenance unblock */
         switch(bearChn->status)
         {
            case AVAIL:
               if (callPresent)
               {
                  actLog = TRUE;
               }
               break;

            case IN_SERVICE:
               if (!callPresent)
               {
                  actRst = TRUE;
                  actLog = TRUE;
               }
               break;

            case OUT_SERVICE_NE:
            case OUT_SERVICE_FE:

               if (callPresent)
               {
                  actLog = TRUE;
               }
               bearChn->suId = tCb->spId;
               bearChn->suIdPres = TRUE;
               bearChn->pendStat = NS_INSRV;
               bearChn->status = OUT_SERVICE_FE;
      
               if (cb == NULLP)
                  break;
      
               inFillChanId (cb, ctldPcb, &chanId);
               chanId.chanNmbSlotMap.val[0] = bearChn->chnNmb;
               inGenSrvReq (cb, &chanId, NS_INSRV);
               break;

            case MAINT_FE:
               actRst = TRUE;
               if (callPresent)
               {
                  actLog = TRUE;
               }
               break;

            case MAINT_NE:

               bearChn->suId = tCb->spId;
               bearChn->suIdPres = TRUE;
               bearChn->pendStat = NS_INSRV;
        
               if (callPresent)
                  bearChn->status = IN_SERVICE;
               else
                  bearChn->status = OUT_SERVICE_FE;
      
               if (cb == NULLP)
                  break;

               inFillChanId (cb, ctldPcb, &chanId);
               chanId.chanNmbSlotMap.val[0] = bearChn->chnNmb;
               inGenSrvReq (cb, &chanId, NS_INSRV);
               break;
          
            default: /* NOTPROVISIONED */
               gitDebugPrint(__FILE__, __LINE__, "inHandleBChanReq:  Invalid B channel state=%s, chan=%d, suId=%d, ntc=%p\n",
                             bChanStatusStr(bearChn->status), bearChn->chnNmb, bearChn->intId, bearChn->ntc);
               break;
         }
         break;

      case AUNINH:   /* Hardware unblock */
         switch(bearChn->status)
         {
            case AVAIL:
               if (callPresent)
               {
                  actRst = TRUE;
                  actLog = TRUE;
               }
               break;

            case IN_SERVICE:
               if (!callPresent)
               {
                  actRst = TRUE;
                  actLog = TRUE;
               }
               break;

            case OUT_SERVICE_FE:
               if (callPresent)
                  actLog = TRUE;

               bearChn->suId = tCb->spId;
               bearChn->suIdPres = TRUE;
               bearChn->pendStat = NS_INSRV;
      
               if (cb == NULLP)
                  break;
      
               inFillChanId (cb, ctldPcb, &chanId);
               chanId.chanNmbSlotMap.val[0] = bearChn->chnNmb;
               inGenSrvReq (cb, &chanId, NS_INSRV);

               break;

            case OUT_SERVICE_NE:
            case MAINT_NE:
               if (callPresent)
                  actLog = TRUE;

               if(mntStat)
               {
                  actRst = FALSE;

                  /* send service msg only in case of T1 bouncing */
                  if(bearChn->status != MAINT_NE)
                  {
                     bearChn->suId = tCb->spId;
                     bearChn->suIdPres = TRUE;
                     bearChn->pendStat = NS_MAINT;
                     bearChn->waitSrvAck = TRUE;
                     bearChn->status = MAINT_NE;

                     if(cb == NULLP)
                        break;

                     /* send out-of-service service message */
                     inFillChanId (cb, ctldPcb, &chanId);
                     chanId.chanNmbSlotMap.val[0] = bearChn->chnNmb;
                     inGenSrvReq (cb, &chanId, NS_OUTSRV);
                  }
               }
               else
               {
                  /* bug#17517, rkhan */
                  if (!sigPcb->cfg.rstOpt)
                  {
                     actRst = FALSE;
                     bearChn->suId = tCb->spId;
                     bearChn->suIdPres = TRUE;
                     bearChn->pendStat = NS_INSRV;
                     bearChn->status = OUT_SERVICE_FE;

                     if (cb == NULLP)
                        break;

                     inFillChanId (cb, ctldPcb, &chanId);
                     chanId.chanNmbSlotMap.val[0] = bearChn->chnNmb;
                     inGenSrvReq (cb, &chanId, NS_INSRV);
                  }
                  else
                     actRst = TRUE;
               }
               break;

            case MAINT_FE:
               actLog = TRUE;
               actRst = TRUE;
               break;
          
            default: /* NOTPROVISIONED */
               gitDebugPrint(__FILE__, __LINE__, "inHandleBChanReq:  Invalid B channel state=%s, chan=%d, suId=%d, ntc=%p\n",
                             bChanStatusStr(bearChn->status), bearChn->chnNmb, bearChn->intId, bearChn->ntc);
         }
         break;

      default:
         gitDebugPrint(__FILE__, __LINE__, "inHandleBChanReq:  Invalid action=%d state=%s, chan=%d, suId=%d, ntc=%p\n",
                       action, bChanStatusStr(bearChn->status), bearChn->chnNmb, bearChn->intId, bearChn->ntc);
   }

   /* Stop B channel restart timer, if any, when we are blocking a B channel */
   if ((action == AINH) || (action == ADISIMM))
   {
      U8 tmrNum;
   
      for (tmrNum = 0; tmrNum < MAXBCHTIMER; tmrNum++)
         if (bearChn->timers[tmrNum].tmrEvnt == TMR_T316C)
            inRmvBChTq(bearChn, tmrNum);

      bearChn->rst = FALSE;
   }


   /* TELICA-MCAO-PRJ-NI2-SRV_MSG-- begin of code change */
   inNmReport(LCM_CATEGORY_INTERFACE, LCM_EVENT_DISABLED, LCM_CAUSE_BCHAN,
              (U8) BCHAN, ctldPcb->suId,bearChn->chnNmb);
  
   zqRunTimeUpd(ZQ_BCHNL_CB, CMPFTHA_UPD_REQ, (PTR)bearChn);

   if (actLog == TRUE)  /* Uexpected state.  Log message. */
   {
      gitDebugPrint(__FILE__, __LINE__, "Warning:  unexpected state, action=%d state=%s, chan=%d, suId=%d, ntc=%p\n"
                    "     sigPcb->rSuId=%ld, ctldPcb->rSuId=%ld, dChannelNum=%d\n",
                    action, bChanStatusStr(bearChn->status), bearChn->chnNmb, bearChn->intId, bearChn->ntc,
                    sigPcb->rSuId, ctldPcb->rSuId, sigPcb->cfg.dChannelNum);
   }

   if (actRst == TRUE)  /* Send restart request for this B channel */
   {
      if (!sigPcb->cfg.rstOpt)
      {
         bearChn->status = AVAIL;
         zqRunTimeUpd(ZQ_BCHNL_CB, CMPFTHA_UPD_REQ, (PTR)bearChn);
      }
      else
      {
         InCb *cb;
         S16 ret = LCM_REASON_NOT_APPL;

         /* Find currently active D channel */
         cb = inGetSigCb(sigPcb);
          
         if (cb != NULLP)
         {
            switch (sigPcb->cfg.swtch)
            {
               /* NOTE:  Put variant specific action here Max */
                  
               case SW_NI2:
               default:
                  ret = inRestartChan(sigPcb, ctldPcb, cb, bearChn->chnNmb);
            }
         }
          
         if (ret != LCM_REASON_NOT_APPL)
         {
            gitDebugPrint(__FILE__, __LINE__, "inHandleBChanReq:  restart failed, ret=%d, action=%d state=%s,"
                          " chan=%d, suId=%d, ntc=%p, rSuId=%ld\n",
                          ret, action, bChanStatusStr(bearChn->status), bearChn->chnNmb,
bearChn->intId, bearChn->ntc, sigPcb->rSuId);
         }
      }
   }
}    /* End of inHandleBChanReqSrv */


/*
*
*      Fun:   inNI2AuditbChan
*
*      Desc:  NI2 Bchannel Status Audit 
*
*      Ret:   ROK   - ok
*
*      Notes: None
*
*      File:  qn_bdy11.c
*
*/

#ifdef ANSI
PUBLIC S16 inNI2AuditbChan
(
 InCb  *tCb,
 InPCB *pcb,
 InNtc *ntc
 )
#else
     PUBLIC S16 inNI2AuditbChan(tCb, pcb, ntc)
     InCb *tCb;
     InPCB *pcb;
     InNtc *ntc;
#endif
{
   InCtldPcb *ctldPcb;
   InBearChn *bearChn;
   S16 i;

   TRC3(inNI2AuditbChan);

   if (ntc->dChanAs)
      RETVALUE(ROK);


   if (ntc->bChanAs)
   {
      if (((ctldPcb = ntc->ctldPcb) != NULLP) 
          && (ntc->ctldPcb->bBearer != NULLP))
      {
         InCb *cb;
         ChanId chanId;
      
         cb = inGetSigCb(pcb);

         for ( i = 0; i < (S16) ntc->bChanSel.nmbOfChn; i++)
         {
            IN_GET_BEARER(ctldPcb, ntc->bChanSel.bChan[i], 
                          ctldPcb->cfg.firstBChanNum, bearChn);
          
            if (bearChn->status != IN_SERVICE) /* it's not a glare condition */
            {
               if ((bearChn->status == AVAIL) || 
                   (bearChn->status == OUT_SERVICE_FE) ||
                   (bearChn->status == MAINT_FE))
               {
    
                  bearChn->suId = tCb->spId;
                  bearChn->suIdPres = TRUE;
                  bearChn->pendStat = NS_INSRV;
                 
                  if (cb == NULLP)
                     break;
                 
                  inFillChanId (cb, ctldPcb, &chanId);
                  chanId.chanNmbSlotMap.val[0] = bearChn->chnNmb;
                  inGenSrvReq (cb, &chanId, NS_INSRV);           
               }
               else if  ((bearChn->status == MAINT_NE) || 
                         (bearChn->status == OUT_SERVICE_NE))
               {
                  bearChn->suId = tCb->spId;
                  bearChn->suIdPres = TRUE;
                  bearChn->pendStat = NS_OUTSRV;
                 
                  if (cb == NULLP)
                     break;
                 
                  inFillChanId (cb, ctldPcb, &chanId);
                  chanId.chanNmbSlotMap.val[0] = bearChn->chnNmb;
                  inGenSrvReq (cb, &chanId, NS_OUTSRV);                  
               }
            }
         }
      }
      else
      {
         gitDebugPrint(__FILE__, __LINE__, "ctldPcb=%p, ctldPcb->bBearer=%p\n", ctldPcb, (ctldPcb?ctldPcb->bBearer:0));
      }
   }
   RETVALUE(ROK);    
}

/*
*
*      Fun:   inDisEnbChan
*
*      Desc: 
*
*      Ret:   ROK   - ok
*
*      Notes: None
*
*      File:  qn_bdy11.c
*
*/
#ifdef ANSI
PUBLIC S16 inDisEnbChan
(
 InCtldPcb *ctldPcb,
 InPCB *sigPcb,
 U8 chn,
 U8 action,
 U32 maintStat
 )
#else
     PUBLIC S16 inDisEnbChan(ctldPcb, sigPcb, chn, action, maintStat)
     InCtldPcb *ctldPcb;
     InPCB *sigPcb;
     U8 chn;
     U8 action;
     U32 maintStat;
#endif
{

   S16 i;
   InBearChn *bearChn;
   U8 tmrNum;
   InCb *cb, *sigCb;
   InCb *tCb;
   ChanId chanId;

   TRC3(inDisEnbChan);

   bearChn = NULLP;
   cb = inGetSigCb(sigPcb);
   sigCb = inGetCBPtr(sigPcb->suId, CONN_CES);
   tCb = inGetSapPtr(sigPcb->cfg.tCbId);

   INDBGP_CB(DBGMASK_MI, "rSuId = %ld action=%d chan=%d\n", ctldPcb->rSuId, action, chn);

   /* TELICA-jwh-02/26/2001:  Add logic to determine if DS1 or single B channel restart is apppropriate  */
   if ((sigPcb->cfg.swtch == SW_NI2) && (sigPcb->cfg.rstOpt) && (chn == ALL_CHAN))
   {
      U8 tmrNum;

      if (action == AUNINH)  /* Hardware unblock */
      {
         int actDs1Rst = TRUE;
         int actNone = TRUE;
         int callPresent;
         int rstInProgress;

         /* If this DS1 is already restarting, our job here is done */
         if (ctldPcb->state == CBST2RST)
         {
            return(LCM_REASON_NOT_APPL);
         }

         for (i = ctldPcb->cfg.firstBChanNum; 
              i < (ctldPcb->cfg.nmbBearChan + 
                   ctldPcb->cfg.firstBChanNum); i++)
         {
            IN_GET_BEARER(ctldPcb, i, ctldPcb->cfg.firstBChanNum, bearChn);
            if (bearChn->status == NOTPROVISIONED) continue;
            

            INDBGP_CB(DBGMASK_MI, "rSuId = %ld bch->intId =%d chan=%d, dChan=%d action=%d state=%s ntc=0x%lx\n",
              ctldPcb->rSuId, bearChn->intId, bearChn->chnNmb, bearChn->dChan, action,
              bChanStatusStr(bearChn->status), (U32)(bearChn->ntc));

            if (!bearChn->dChan)
            {
               for (tmrNum = 0, rstInProgress = FALSE; tmrNum < MAXBCHTIMER; tmrNum++)
               {
                  if (bearChn->timers[tmrNum].tmrEvnt == TMR_T316C)
                  {
                     rstInProgress = TRUE;
                     INDBGP_CB(DBGMASK_MI, "rstInProgress = TRUE\n");
                     break;
                  }
               }

               callPresent = ((bearChn->ntc == NULL)?FALSE:TRUE);

               if ((rstInProgress == TRUE) || (callPresent == TRUE))
               {
                  /* We found a B channel that should NOT be restarted */
                  actDs1Rst = FALSE;
               }

               if ((bearChn->status != NOTPROVISIONED) && 
                   ((bearChn->status != AVAIL) || (callPresent == TRUE)) &&
                   ((bearChn->status != IN_SERVICE) || (callPresent != TRUE))) 
               {
                  /* We found a B channel that should be restarted */
                  actNone = FALSE;
               }

               /* if any of channle's maint. stat is maint busy,
                * we cannot initiate DSS1 level restart.
                */
               if(IN_IS_MNT_BLOCKED(maintStat, i))
               {
                  actDs1Rst = FALSE;
               }
            }
         }

         if (actNone)
         {
            return(LCM_REASON_NOT_APPL);
         }

         if (actDs1Rst)
         { 
            S16 ret = LCM_REASON_NOT_APPL;

            /* If a DS1 or PRI restart is already outstanding, don't send another */
            for (tmrNum = 0; tmrNum < MAXBCHTIMER; tmrNum++)
            {
               if (ctldPcb->timers[tmrNum].tmrEvnt == TMR_T316)
               {
                  return(LCM_REASON_NOT_APPL);
               }
            }

            switch (sigPcb->cfg.swtch)
            {
               /* NOTE:  Put variant specific action here Max */

               case SW_NI2:
               default:
                  ret = inRestartInt(sigPcb, ctldPcb, CONN_CES);
            }

            if (ret != LCM_REASON_NOT_APPL)
            {
               gitDebugPrint(__FILE__, __LINE__, "inRestartInt:  DS1 restart failed, ret=%d, action=%d state=%s,"
                             " chan=%d, suId=%d, ntc=0x%lx, rSuId=%ld\n",
                             ret, action, bChanStatusStr(bearChn->status), bearChn->chnNmb, bearChn->intId, (U32)(bearChn->ntc), sigPcb->rSuId);
            }

            return(LCM_REASON_NOT_APPL);
         }
      }
      else if (action == AINH) /* Hardware block */ 
      {
         /* If a DS1 or PRI restart timer is running, stop it to prevent more restarts from being sent */
         for (tmrNum = 0; tmrNum < MAXBCHTIMER; tmrNum++)
         {
            if (ctldPcb->timers[tmrNum].tmrEvnt == TMR_T316)
            {
               /* Stop restart timer */
               inRmvCtldPcbTq(ctldPcb, tmrNum);
            }
         }

         /* If this is a PRI, all B channels are now blocked.  Stop TREST if it is running. */
         if (!sigPcb->cfg.nfasInt)
         {
            for (tmrNum = 0; tmrNum < MAXBCHTIMER; tmrNum++)
            {
               if (sigPcb->timers[tmrNum].tmrEvnt == TMR_TREST)
               {
                  /* Stop t-rest restart timer */
                  inRmvPcbTq(sigPcb, tmrNum);
               }
            }
         }

         /* Update ctldPcb state */
         if (ctldPcb->state == CBST2RST)
         {
            ctldPcb->state = CBST1XFER;
            zqRunTimeUpd(ZQ_CTLDPSAP_CB, CMPFTHA_UPD_REQ, (PTR)ctldPcb);
         }

         /* Update cb state */
         if (cb != NULLP)
         {
            if (cb->state == CBST2RST)
            {
               cb->state = CBST1XFER;
               zqRunTimeUpd(ZQ_CES_CB, CMPFTHA_UPD_REQ, (PTR)cb);
            }
         }
      }
   } /* SW_NI2 */

   if ((chn == ALL_CHAN) || (chn == ALL_CHAN_BCHANONLY_RSTOPT))
   {
      int callPresent;

      for (i = ctldPcb->cfg.firstBChanNum; 
           i < (ctldPcb->cfg.nmbBearChan + 
                ctldPcb->cfg.firstBChanNum); i++)
      {
         IN_GET_BEARER(ctldPcb, i, ctldPcb->cfg.firstBChanNum, bearChn);
         if (bearChn->status == NOTPROVISIONED) continue;
         INDBGP_CB(DBGMASK_MI, "rSuId = %ld bch->intId =%d chan=%d, action=%d state=%s ntc=0x%lx\n",
                   ctldPcb->rSuId, bearChn->intId, bearChn->chnNmb, action, bChanStatusStr(bearChn->status), (U32)(bearChn->ntc));
 
         if (action == ADISGRC)
         {
             callPresent = (bearChn->ntc == NULL)?FALSE:TRUE;
             if (callPresent && (bearChn->status == IN_SERVICE))
             {
                 /* skip the action if active call exist with action ADISGRC */
                 INDBGP_CB(DBGMASK_MI, "ADISGRC: call exists, skip action on chan=%d\n",
                                        bearChn->chnNmb);
                 continue; 
             }
             /* no active call on bearChan, continue with action AINH */
             INDBGP_CB(DBGMASK_MI, "ADISGRC => AINH: no call found on chan=%d\n",
                                   bearChn->chnNmb);
             action = AINH;
         }
         
         if ((sigPcb->suId == ctldPcb->suId) && (i == sigPcb->cfg.dChannelNum))
         {
            /* NFASBACK: send alarm if backup link is taken down or brought up */
            if(sigPcb->cfg.nfasInt && sigPcb->cfg.buIntPr
               && (cb != NULLP) && (sigCb != NULLP))
            {
                if ((action == AINH) && (bearChn->status == IN_SERVICE)
                     && (sigCb->sigState == ST_STB))
                {
                    INDBGP_CB(DBGMASK_MI, "rSuId=%ld, rSigInt=%ld, state=%d\n",
                              sigPcb->rSuId, sigPcb->cfg.rSigInt, ctldPcb->state);
                    
                    sigCb->state = CBST3DOWN;
                    sigCb->sigState = ST_BSY;
                    inNmReport(LCM_CATEGORY_INTERFACE, LCM_EVENT_DOWN,
                               LCM_CAUSE_DCHAN, (U8) DCHAN, sigPcb->suId, (U8) 0);

                    inStartCbTmr(TMR_DISC, sigCb);
                    InLiDatDiscReq(&sigCb->pst, sigCb->spId, Q_930, sigCb->ces, 0);

                    bearChn->status = OUT_SERVICE_NE;

                }else if ((action == AUNINH) && (bearChn->status != IN_SERVICE)
                          && (sigCb->sigState == ST_BSY))
                {
                    INDBGP_CB(DBGMASK_MI, "rSuId=%ld, rSigInt=%ld, state=%d\n",
                              sigPcb->rSuId, sigPcb->cfg.rSigInt, ctldPcb->state);

                    sigCb->state = CBST1XFER;
                    sigCb->sigState = ST_STB;
                    inNmReport(LCM_CATEGORY_INTERFACE, LCM_EVENT_UP,
                                LCM_CAUSE_DCHAN, (U8) DCHAN, sigPcb->suId, (U8) 0);

                    if (sigPcb->bndState != IN_BND)
                    {
                         sigPcb->bndState = IN_WAIT_BNDCFM;
                         InLiDatBndReq(&sigCb->pst, sigCb->suId, sigPcb->spId, Q_930);
                         inStartPcbTmr(TMR_TINT, sigPcb);
                    }

                    if (sigPcb->cfg.intType == NETWORK)
                    {
                       inStartCbTmr(TMR_CON, sigCb);
                       InLiDatConReq(&sigCb->pst, sigCb->spId, Q_930, sigCb->ces, sigCb->rSuId);
                    }
                    
                    bearChn->status = IN_SERVICE;   
                }
            }else
            {
                bearChn->status = IN_SERVICE;
                INDBGP_CB(DBGMASK_MI,"%s: old status: %d new status: %d\n",
                                 __FUNCTION__, bearChn->status, IN_SERVICE);
            }

         }
         else
	 {
            /* TELICA-jwh-03/14/2001:  Add hardware unblock action. */
            if ((action == AENA) || (action == AUNINH))
            {
               switch (sigPcb->cfg.swtch)
               {
                  case SW_ATT5EB:
                  case SW_ATT5EP:
                  case SW_ATT4E:
                  case SW_NTDMS250:
                  case SW_NTMCI:
                  case SW_NTNI:
                     /* TELICA-mg-03/20/01: Do not send IS ServiceMsg if this B-channel is already IS */
                     if ((bearChn->status == AVAIL) || (bearChn->status == IN_SERVICE))
                        break;
                     for (tmrNum = 0; tmrNum < MAXBCHTIMER; tmrNum++)
                        if (bearChn->timers[tmrNum].tmrEvnt == TMR_T332)
                           inRmvBChTq (bearChn, tmrNum);

                     bearChn->suId = tCb->spId;
                     bearChn->suIdPres = TRUE;
                     bearChn->pendStat = NS_INSRV;
                     bearChn->waitSrvAck = TRUE;

                     if(action == AUNINH && IN_IS_MNT_BLOCKED(maintStat, i))
                     {
                        /* send service msg only in case of T1 bouncing */
                        if(bearChn->status != MAINT_NE)
                        {
                           bearChn->status = MAINT_NE;
                           bearChn->pendStat = NS_MAINT;

                           if(cb != NULLP)
                           {
                              inFillChanId (cb, ctldPcb, &chanId);
                              chanId.chanNmbSlotMap.val[0] = bearChn->chnNmb;
                              inGenSrvReq (cb, &chanId, NS_MAINT);
                           }
                        }
                        break;
                     }

                     if (bearChn->status == OUT_SERVICE_NE)
                        bearChn->status = OUT_SERVICE_FE;
                     else
                        if (bearChn->status == MAINT_NE)
                           bearChn->status = MAINT_FE;

                     /* TELICA-jwh-03/21/2001:  If cb is NULL we're done */
                     if (cb == NULLP)
                        break;

                     inFillChanId (cb, ctldPcb, &chanId);
                     chanId.chanNmbSlotMap.val[0] = bearChn->chnNmb;
                     inGenSrvReq (cb, &chanId, NS_INSRV);
                     break;
                  case SW_NTDMS100P:
                     /* Bug 64925 : According to DMS100 spec: NIS-A211-1, chapter 4-8:
                      * When Layer 3 is subsequently established, all of its B-channels
                      * automatically change to the In service state, without any
                      * exchange of B-channel maintenance messages. B-channels
                      * which were not in the In service state previous to a Layer
                      * 3 failure, typically as a result of manual maintenance
                      * intervention, do not change state when Layer 3 is established.
                      */
                     if (bearChn->status == OUT_SERVICE_NE)
                     {
                        if (cb == NULLP)
                           break;

                        if (cb->state == CBST1XFER)
                        {
                           bearChn->status = AVAIL;
                        }
                     }
                     else
                     {
                        INDBGP_CB(DBGMASK_MI, "rSuId = %ld state = %d\n", ctldPcb->rSuId, bearChn->status);
                     }

                     if (IN_IS_MNT_BLOCKED(maintStat, i))
                     {
                        if(bearChn->status != MAINT_NE)
                        {
                           bearChn->status = MAINT_NE;
                        }
                     }
                     break;
                     /* TELICA-jwh-03/14/2001:  BCHANSYNC:  Add logic to handle B channel maintenance request. */
                  case SW_ETSI:
                     inHandleBChanReq( ctldPcb, sigPcb, bearChn, action, IN_IS_MNT_BLOCKED( maintStat, i));
                     break;
                  case SW_NI2:
                     /* IN_PBD: ISDN Persistent B/D Channel , H.Kim*/
                     if(sigPcb->cfg.bcasOpt)
                     {
                        inHandleBChanReqSrv(cb, tCb, ctldPcb, sigPcb, bearChn, action,
                                            IN_IS_MNT_BLOCKED(maintStat,i));
                     }
                     else
                     {
                        inHandleBChanReq( ctldPcb, sigPcb, bearChn, action, IN_IS_MNT_BLOCKED( maintStat, i));
                     }
                     break;
                  default:
                     bearChn->status = AVAIL;
                     break;
               }
            }
            else
            {
               switch (sigPcb->cfg.swtch)
               {
                  case SW_ATT5EB:
                  case SW_ATT5EP:
                  case SW_ATT4E:
                  case SW_NTDMS100P:
                  case SW_NTDMS250:
                  case SW_NTMCI:
                  case SW_NTNI:
                     /* TELICA-jwh-04/10/01: Do not send OOS ServiceMsg if this B-channel is already OOS */
                     if (bearChn->status == OUT_SERVICE_NE)
                        break;

                     for (tmrNum = 0; tmrNum < MAXBCHTIMER; tmrNum++)
                        if (bearChn->timers[tmrNum].tmrEvnt == TMR_T332)
                        {
                           inRmvBChTq (bearChn, tmrNum);
                        }

                     bearChn->suId = tCb->spId;
                     bearChn->suIdPres = TRUE;
                     /* mcao bug #3878 begin of code change */ 
                     if (action == ADISIMM) 
                     {
                        bearChn->pendStat = NS_MAINT;
                        bearChn->waitSrvAck = TRUE;
                        bearChn->status = MAINT_NE;
                     }
                     else
                     {
                        bearChn->pendStat = NS_OUTSRV;
                        bearChn->waitSrvAck = TRUE;
                        bearChn->status = OUT_SERVICE_NE;
                     } 

                     /* TELICA-jwh-03/21/2001:  If cb is NULL we're done */
                     if (cb == NULLP)
                        break;

                     inFillChanId (cb, ctldPcb, &chanId);
                     chanId.chanNmbSlotMap.val[0] = bearChn->chnNmb;

                     /* mcao bug #3878 begin of code change */ 
                     inGenSrvReq (cb, &chanId, bearChn->pendStat);
                     break;

                     /* TELICA-jwh-03/14/2001:  BCHANSYNC:  Add logic to handle B channel maintenance request. */
                     /* Start of code change */
                  case SW_ETSI:
                     inHandleBChanReq( ctldPcb, sigPcb, bearChn, action, IN_IS_MNT_BLOCKED( maintStat, i));
                     break;
                  case SW_NI2:
                     /* TELICA-MCAO-PRJ-NI2-SRV_MSG-- begin of code change */
                     if(sigPcb->cfg.bcasOpt)
                     {
                        DP_PLMIN(DBGMASK_L1,"%s: calling inHandleBChanReqSrv()\n",__FUNCTION__);
                        inHandleBChanReqSrv(cb, tCb, ctldPcb, sigPcb, bearChn, action,
                                            IN_IS_MNT_BLOCKED( maintStat, i));
                     }
                     else
                     {
                        DP_PLMIN(DBGMASK_L1,"%s: calling inHandleBChanReq()\n",__FUNCTION__);
                        inHandleBChanReq( ctldPcb, sigPcb, bearChn, action, IN_IS_MNT_BLOCKED( maintStat, i));
                     }
                     break;
                  default:
                     bearChn->status = IN_SERVICE;
                     break;
               }
            }
         }
         zqRunTimeUpd(ZQ_BCHNL_CB, CMPFTHA_UPD_REQ, (PTR)bearChn);
      }
   }
   else if ((chn >= ctldPcb->cfg.firstBChanNum) &&
            (chn < (U8)(ctldPcb->cfg.firstBChanNum + 
                        ctldPcb->cfg.nmbBearChan)))
   {
      int callPresent;

      IN_GET_BEARER(ctldPcb, chn, ctldPcb->cfg.firstBChanNum, bearChn);
      if (bearChn->status == NOTPROVISIONED)
         RETVALUE(LCM_REASON_NOT_APPL);
      INDBGP_CB(DBGMASK_MI, "rSuId = %ld bch->intId =%d chan=%d, action=%d state=%s ntc=0x%lx\n",
                ctldPcb->rSuId, bearChn->intId, bearChn->chnNmb, action, bChanStatusStr(bearChn->status), (U32) (bearChn->ntc));
    
      if (action == ADISGRC)
      {
          callPresent = (bearChn->ntc == NULL)?FALSE:TRUE;
          if (callPresent && (bearChn->status == IN_SERVICE))
          {
              /* skip the action if active call exist with action ADISGRC */
              INDBGP_CB(DBGMASK_MI, "ADISGRC: call exists, skip action on chan=%d\n",
                                     bearChn->chnNmb);
              RETVALUE(LCM_REASON_NOT_APPL);
          }
          /* no active call on bearChan, continue with action AINH */
          INDBGP_CB(DBGMASK_MI, "ADISGRC => AINH: no call found on chan=%d\n",
                                bearChn->chnNmb);
          action = AINH;
      }

      /* TELICA-jwh-03/14/2001:  Add hardware unblock action. */
      if ((action == AENA) || (action == AUNINH))
      {
         switch (sigPcb->cfg.swtch)
         {
            case SW_ATT5EB:
            case SW_ATT5EP:
            case SW_ATT4E:
            case SW_NTDMS100P:
            case SW_NTDMS250:
            case SW_NTMCI:
            case SW_NTNI:
               /* TELICA-mg-03/20/01: Do not send IS ServiceMsg if this B-channel is already IS */
               if ((bearChn->status == AVAIL) || (bearChn->status == IN_SERVICE))
                  break;
               for (tmrNum = 0; tmrNum < MAXBCHTIMER; tmrNum++)
                  if (bearChn->timers[tmrNum].tmrEvnt == TMR_T332)
                  {
                     inRmvBChTq (bearChn, tmrNum);
                  }

               bearChn->suId = tCb->spId;
               bearChn->suIdPres = TRUE;
               bearChn->pendStat = NS_INSRV;
               bearChn->waitSrvAck = TRUE;

               if(action == AUNINH && IN_IS_MNT_BLOCKED(maintStat, chn))
               {
                  /* send service msg only in case of T1 bouncing */
                  if(bearChn->status != MAINT_NE)
                  {
                     bearChn->status = MAINT_NE;
                     bearChn->pendStat = NS_MAINT;

                     if(cb != NULLP)
                     {
                        inFillChanId (cb, ctldPcb, &chanId);
                        chanId.chanNmbSlotMap.val[0] = bearChn->chnNmb;
                        inGenSrvReq (cb, &chanId, NS_MAINT);
                     }
                  }
                  break;
               }

               if (bearChn->status == OUT_SERVICE_NE)
                  bearChn->status = OUT_SERVICE_FE;
               else
                  if (bearChn->status == MAINT_NE)
                     bearChn->status = MAINT_FE;

               /* TELICA-jwh-03/21/2001:  If cb is NULL we're done */
               if (cb == NULLP)
                  break;

               inFillChanId (cb, ctldPcb, &chanId);
               chanId.chanNmbSlotMap.val[0] = bearChn->chnNmb;
               inGenSrvReq (cb, &chanId, NS_INSRV);
               break;

               /* TELICA-jwh-03/14/2001:  BCHANSYNC:  Add logic to handle B channel maintenance request. */
            case SW_ETSI:
               inHandleBChanReq( ctldPcb, sigPcb, bearChn, action,IN_IS_MNT_BLOCKED(maintStat,chn));
               break;
            case SW_NI2:
               /* TELICA-MCAO-PRJ-NI2-SRV_MSG-- begin of code change */
               if(sigPcb->cfg.bcasOpt)
               {
                  inHandleBChanReqSrv(cb, tCb, ctldPcb, sigPcb, bearChn, action,
                                      IN_IS_MNT_BLOCKED(maintStat,chn));
               }
               else
               {
                  inHandleBChanReq( ctldPcb, sigPcb, bearChn, action,IN_IS_MNT_BLOCKED(maintStat,chn));
               }
       
               /* TELICA-MCAO-PRJ-NI2-SRV_MSG-- end of code change */
               break;
            default: 
               bearChn->status = AVAIL;
               break;
         }
         inNmReport(LCM_CATEGORY_INTERFACE, LCM_EVENT_ENABLED, LCM_CAUSE_BCHAN,
                    (U8) BCHAN, ctldPcb->suId,bearChn->chnNmb);
      }
      else
      {
         switch (sigPcb->cfg.swtch)
         {
            case SW_ATT5EB:
            case SW_ATT5EP:
            case SW_ATT4E:
            case SW_NTDMS100P:
            case SW_NTDMS250:
            case SW_NTMCI:
            case SW_NTNI:
               /* TELICA-jwh-04/10/01: Do not send OOS ServiceMsg if this B-channel is already OOS */
               if (bearChn->status == OUT_SERVICE_NE)
                  break;

               for (tmrNum = 0; tmrNum < MAXBCHTIMER; tmrNum++)
                  if (bearChn->timers[tmrNum].tmrEvnt == TMR_T332)
                  {
                     inRmvBChTq (bearChn, tmrNum);
                  }

               bearChn->suId = tCb->spId;
               bearChn->suIdPres = TRUE;
               /* mcao bug #3878 begin of code change */ 
               if (action == ADISIMM) 
               {
                  bearChn->pendStat = NS_MAINT;
                  bearChn->waitSrvAck = TRUE;
                  bearChn->status = MAINT_NE;
               }
               else
               {
                  bearChn->pendStat = NS_OUTSRV;
                  bearChn->waitSrvAck = TRUE;
                  bearChn->status = OUT_SERVICE_NE;
               } 

               /* TELICA-jwh-03/21/2001:  If cb is NULL we're done */
               if (cb == NULLP)
                  break;

               inFillChanId (cb, ctldPcb, &chanId);
               chanId.chanNmbSlotMap.val[0] = bearChn->chnNmb;
               inGenSrvReq (cb, &chanId, bearChn->pendStat);
               break;

               /* TELICA-jwh-03/14/2001:  BCHANSYNC:  Add logic to handle B channel maintenance request. */
            case SW_ETSI:
               inHandleBChanReq( ctldPcb, sigPcb, bearChn,action,IN_IS_MNT_BLOCKED(maintStat,chn));
               break;
            case SW_NI2:
               /* TELICA-MCAO-PRJ-NI2-SRV_MSG-- begin of code change */
               if(sigPcb->cfg.bcasOpt)
               {
                  inHandleBChanReqSrv(cb, tCb, ctldPcb, sigPcb, bearChn, action,
                                      IN_IS_MNT_BLOCKED(maintStat,chn));
               }
               else
               {
                  inHandleBChanReq( ctldPcb, sigPcb, bearChn,action,IN_IS_MNT_BLOCKED(maintStat,chn));
               }

               break;

            default:
               bearChn->status = IN_SERVICE;
               break;
         }
         inNmReport(LCM_CATEGORY_INTERFACE, LCM_EVENT_DISABLED, LCM_CAUSE_BCHAN,
                    (U8) BCHAN, ctldPcb->suId,bearChn->chnNmb);
      }
      zqRunTimeUpd(ZQ_BCHNL_CB, CMPFTHA_UPD_REQ, (PTR)bearChn);
   }

   else
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      INLOGERROR(ERRCLS_INT_PAR, EIN732, (ErrVal)chn ,
                 "InMiLinCntrlReq() failed, unable to access BChannel.");
#endif
      RETVALUE(LCM_REASON_INVALID_CHAN);
   }
   RETVALUE(LCM_REASON_NOT_APPL);
} /* end of inDisEnbChan */





/*
 *
 *      Fun:   inDelAddChan
 *
 *      Desc: 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None
 *
 *      File:  qn_bdy11.c
 *
*/

#ifdef ANSI
PUBLIC S16 inDelAddChan
(
 InCtldPcb *ctldPcb,
 U8 chn,
 U8 action
 )
#else
PUBLIC S16 inDelAddChan(ctldPcb, chn, action)
InCtldPcb *ctldPcb;
U8 chn;
U8 action;
#endif
{
   InPCB *sigPcb;
   InCb  *cb;
   
   TRC3(inDelAddChan);

   /* DEV_MGCBCH only trigger the channel update */

   /* get status of Pcb and inCb */
   if (ctldPcb->cfg.sigInt == 0)
   {
      DP("WARNING: signaling link hasn't been configured, keep bchan state as OOS_NE \n");
      RETVALUE(LCM_REASON_NOT_APPL);
   }
   
   if ((sigPcb = *(pcbLstPtr + ctldPcb->cfg.sigInt)) == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
         INLOGERROR(ERRCLS_DEBUG, EIN681, (ErrVal) ctldPcb->cfg.rSigInt,
                    "inDelAddChan() failed, sap not configured.");
#endif
       RETVALUE(LCM_REASON_INVALID_SAP);
   }

   /* get InCb state */
   if (!(cb = *(sigPcb->cesLst + 0)))
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      INLOGERROR(ERRCLS_INT_PAR, EIN710, (ErrVal)0,
                 "() failed, unable to access SAP.");
#endif
      RETVALUE(LCM_REASON_INVALID_SAP);                      
   }
   
   /* decide what state to report, only one signaling link is enough, just need a trigger to
      CLAM FSM */
   switch (cb->state)
   {
      case CBST2RST :
      case CBST1XFER:
         inNmReport(LCM_CATEGORY_LYR_SPECIFIC, LCM_EVENT_UP,
                    LCM_CAUSE_DCHAN,(U8) DCHAN, ctldPcb->suId, (U8) 0);
         break;
      case CBST3DOWN:
      case CBST0CON:
         inNmReport(LCM_CATEGORY_LYR_SPECIFIC, LCM_EVENT_DOWN,
                    LCM_CAUSE_DCHAN,(U8) DCHAN, ctldPcb->suId, (U8) 0);
         break;
   }

   /* while it returns it won't require a transId ! CLAM doesn't like a response */
   RETVALUE(LCM_REASON_NOT_APPL);
} /* end of inDelAddChan */





/*
 *
 *      Fun:   inRestartInt
 *
 *      Desc: 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None
 *
 *      File:  qn_bdy11.c
 *
*/

#ifdef ANSI
PUBLIC S16 inRestartInt
(
 InPCB *pcb,
 InCtldPcb *ctldPcb,
 Ces ces
 )
#else
     PUBLIC S16 inRestartInt(pcb, ctldPcb, ces)
     InPCB *pcb;
     InCtldPcb *ctldPcb;
     Ces ces;
#endif
{
   AllPdus allPdus;
   PduHdr pduHdr;
   Rst rst;
   InCb *cb;
   S16 ret;

   TRC3(inRestartInt);

   if ((ret = inGetCb(ctldPcb, pcb,ces,
                      &cb)) != LCM_REASON_NOT_APPL)
      RETVALUE(ret);

   if ((cb == NULLP) || ((cb->sigState != ST_IS) && pcb->cfg.nfasInt))
   {
      if ((cb = inGetSigCb(pcb)) == NULLP)
      {
         inGenAlarm(LCM_CATEGORY_INTERNAL, LCM_EVENT_INV_EVT, 
                    LCM_CAUSE_INV_CES, pcb->suId);
         RETVALUE(RFAILED);
      }
   }

   rst.rstInd.rstClass.val = 0;
   ret = inRstReqSngInt(ctldPcb, pcb, cb, SND_RST, &rst);
   if ( ret == ROKIGNORE )
      RETVALUE(ret);

   MFINITPDU(&cb->mfMsgCtl, ret, (U8) 0, (U8) MI_RST, 
             NULLP, (ElmtHdr *) &allPdus, (U8) NOTPRSNT, 
             pcb->cfg.swtch, (U32) MF_NA);

   allPdus.m.rst.chanId.eh.pres = PRSNT_NODEF;
   allPdus.m.rst.chanId.intType.pres = PRSNT_NODEF;

   if (ctldPcb->cfg.nmbBearChan > BASICRATE)
      allPdus.m.rst.chanId.intType.val = IT_OTHER;
   else
      allPdus.m.rst.chanId.intType.val = IT_BASIC;

  /* TELICA-jwh-03/07/2001:  For NI2 single DS1 restarts, chan sel must be set to "As indicated in the following octets (0x01)" */
   switch (pcb->cfg.swtch)
   {
      case SW_NI2:
         allPdus.m.rst.chanId.infoChanSel.pres = PRSNT_NODEF;
         allPdus.m.rst.chanId.infoChanSel.val = ICS_B1CHAN;
         break;
      default:
         allPdus.m.rst.chanId.infoChanSel.pres = PRSNT_NODEF;
         allPdus.m.rst.chanId.infoChanSel.val = ICS_NOCHAN;
         break;
   }

   allPdus.m.rst.chanId.prefExc.pres = PRSNT_NODEF;

  /* TELICA-jwh-03/01/2001:  TR-NWT-001268: Rev 4, 3/98, p 5-64:  exclusive bit must be set in chanid for Restart */
   allPdus.m.rst.chanId.prefExc.val = PE_EXCLSVE;
   allPdus.m.rst.chanId.dChanInd.pres = PRSNT_NODEF;
   allPdus.m.rst.chanId.dChanInd.val = DSI_NOTDCHAN;
   allPdus.m.rst.chanId.chanMapType.pres = NOTPRSNT;
   allPdus.m.rst.chanId.chanNmbSlotMap.pres = NOTPRSNT;

   if (pcb->cfg.nfasInt)
   {
      allPdus.m.rst.chanId.intIdentPres.pres = PRSNT_NODEF;
      allPdus.m.rst.chanId.intIdentPres.val = IIP_EXPLICIT;
      allPdus.m.rst.chanId.intIdent.pres = PRSNT_NODEF;
      allPdus.m.rst.chanId.intIdent.val = ctldPcb->cfg.intId;
   }
   else
   {
      allPdus.m.rst.chanId.intIdentPres.pres = PRSNT_NODEF;
      allPdus.m.rst.chanId.intIdentPres.val = IIP_IMPLICIT;
      allPdus.m.rst.chanId.intIdent.pres = NOTPRSNT;
   }

   allPdus.m.rst.chanId.nmbMap.pres = NOTPRSNT;
   allPdus.m.rst.chanId.codeStand1.pres = NOTPRSNT;
   allPdus.m.rst.chanId.chanNmbSlotMap.pres = NOTPRSNT;
    
   allPdus.m.rst.rstInd.eh.pres = PRSNT_NODEF;
   allPdus.m.rst.rstInd.rstClass.pres = PRSNT_NODEF;

   if (rst.rstInd.rstClass.val)
   {
      allPdus.m.rst.rstInd.rstClass.val = rst.rstInd.rstClass.val;
   }
   else
   {
      allPdus.m.rst.rstInd.rstClass.val = CL_SNGINT;
   }

   inInitPduHdr(PD_Q931, ORG_0, pcb->cfg.callRefLen, GLOBAL, (U8) M_RST, 
                &pduHdr);

   /* TELICA - Bugzilla-4321 - Bukucu - 10.25.01 - new imp. of cntrs */
   inUpdSts(&pcb->sts.rstTx, pcb);

   inChkRstMsg(pcb, ctldPcb, &allPdus.m.rst.rstInd, 
               &allPdus.m.rst.chanId);

   /* send restart message */
   inGenPdu(cb, &pduHdr, &allPdus, pcb->cfg.swtch, (U32) MF_NA);

   /* Inform layer 4 that a restart message has been sent to
   * far end */
   inGenRstInd(cb, pcb, RSTSND, 0, &allPdus.m.rst);

   RETVALUE(LCM_REASON_NOT_APPL);
}





/*
 *
 *      Fun:   inRestartChan
 *
 *      Desc: 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None
 *
 *      File:  qn_bdy11.c
 *
*/

PUBLIC S16 inRestartChan
(
 InPCB *pcb,
 InCtldPcb *ctldPcb,
 InCb *cb,
 U8 chn
 )
{
   InBearChn *bearChn;
   AllPdus allPdus;
   PduHdr pduHdr;
   Bool flagBasic;
   InCb *tCb;
   S16 ret;
  
   TRC3(inRestartChan);
  
   IN_GET_BEARER(ctldPcb, chn,
                 ctldPcb->cfg.firstBChanNum, bearChn);
   if (bearChn->status == NOTPROVISIONED)
      RETVALUE(LCM_REASON_NOT_APPL);

   if (pcb->cfg.intType == IT_BASIC)
      flagBasic = TRUE;
   else
      flagBasic = FALSE;
  
   if (!pcb->cfg.tCbCfg)
      RETVALUE(LCM_REASON_INVALID_SAP);
   else
      if ((tCb = inGetSapPtr(pcb->cfg.tCbId)) == NULLP)
         RETVALUE(LCM_REASON_INVALID_SAP);
   DP_PLMIN(DBGMASK_L1,"inRestartChan: Bchan = %d, BChanStatus = %d, intType = %d \n",
                bearChn->chnNmb, bearChn->status, pcb->cfg.intType);

   if ((cb == NULLP) || ((cb->sigState != ST_IS) && pcb->cfg.nfasInt))
   {
      if ((cb = inGetSigCb(pcb)) == NULLP)
      {
         inGenAlarm(LCM_CATEGORY_INTERNAL, LCM_EVENT_INV_EVT, 
                    LCM_CAUSE_INV_CES, pcb->suId);
         RETVALUE(RFAILED);
      }
   }

   /* 20558: get the right Pcb, cb must be an active one now */
   if (cb->suId != pcb->suId)
   {
      if ((pcb = *(pcbLstPtr + cb->suId))== NULLP)
      {
#if (ERRCLASS & ERRCLS_DEBUG)
         INLOGERROR(ERRCLS_DEBUG, EIN813, (ErrVal) cb->rSuId,
                    "inRestartChan() failed, unable to access DLSAP.");
#endif
         RETVALUE(RFAILED);
      }
   }
  
   ret = inRstReqBChan (bearChn, cb, pcb, tCb, ctldPcb, SND_RST, flagBasic);
   if ( ret == ROKIGNORE )
      RETVALUE(ROKIGNORE );

  /* Send out Restart Message */
   inInitPduHdr(PD_Q931, ORG_0, pcb->cfg.callRefLen, GLOBAL, (U8) M_RST, 
                &pduHdr);

   MFINITPDU(&cb->mfMsgCtl, ret, (U8) 0, (U8) MI_RST, 
             NULLP, (ElmtHdr *) &allPdus, (U8) NOTPRSNT, 
             pcb->cfg.swtch, (U32) MF_NA);

   inFillChanId(cb, ctldPcb, &allPdus.m.rst.chanId);

   allPdus.m.rst.chanId.chanNmbSlotMap.val[0] = chn;

   if ((!ctldPcb->cfg.nfasInt) || (cb->suId == ctldPcb->suId))
   {
      allPdus.m.rstAck.chanId.intIdentPres.val = IIP_IMPLICIT;
      allPdus.m.rstAck.chanId.intIdent.pres = NOTPRSNT;
   }

   allPdus.m.rst.rstInd.eh.pres = PRSNT_NODEF;
   allPdus.m.rst.rstInd.rstClass.pres = PRSNT_NODEF;
   allPdus.m.rst.rstInd.rstClass.val = CL_INDCHAN;
  
  /* TELICA - Bugzilla-4321 - Bukucu - 10.25.01 - new imp. of cntrs */
   inUpdSts(&pcb->sts.rstTx, pcb);

   inChkRstMsg(pcb, ctldPcb, &allPdus.m.rst.rstInd,
               &allPdus.m.rst.chanId);
   /* ngc - bug 47964 */
   INDBGP(DBGMASK_MI,"inRestartChan: Sending RST for channel = %d \n", bearChn->chnNmb);
   /* send restart message */
   inGenPdu(cb, &pduHdr, &allPdus, pcb->cfg.swtch, (U32) MF_NA);
 
   /* Inform layer 4 that a restart message has been sent to
   * far end
   */
   inGenRstInd(cb, pcb, RSTSND, 0, &allPdus.m.rst);

   RETVALUE(LCM_REASON_NOT_APPL);
}






/*
 *
 *      Fun:   inRestartAllInt
 *
 *      Desc: 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None
 *
 *      File:  qn_bdy11.c
 *
*/

#ifdef ANSI
PUBLIC S16 inRestartAllInt
(
 SuId sapId,
 Ces ces
 )
#else
     PUBLIC S16 inRestartAllInt(sapId, ces)
     SuId sapId;
     Ces ces;
#endif
{
   AllPdus allPdus;
   PduHdr pduHdr;
   InCb *cb;
   InPCB *pcb;
   InCtldPcb *ctldPcb;
   S16 ret;

   TRC3(inRestartAllInt);

   if ((ret = inGetPcb(sapId, &ctldPcb, &pcb)) 
       != LCM_REASON_NOT_APPL)
      RETVALUE(ret);

   if ((ret = inGetCb(ctldPcb, pcb,ces,
                      &cb)) != LCM_REASON_NOT_APPL)
      RETVALUE(ret);
  
   if ((ret = inRstReqAllInt(pcb, cb, SND_RST)) != ROK)
   {
      /* If a global restart was sent already or if not NFAS 
         then ignore */
      RETVALUE(LCM_REASON_NOT_APPL);
   }
  
   inInitPduHdr(PD_Q931, ORG_0, pcb->cfg.callRefLen, GLOBAL, (U8) M_RST, 
                &pduHdr);

   MFINITPDU(&cb->mfMsgCtl, ret, (U8) 0, (U8) MI_RST, 
             NULLP, (ElmtHdr *) &allPdus, (U8) NOTPRSNT, 
             pcb->cfg.swtch, (U32) MF_NA);

   allPdus.m.rst.rstInd.eh.pres = PRSNT_NODEF;
   allPdus.m.rst.rstInd.rstClass.pres = PRSNT_NODEF;
   allPdus.m.rst.rstInd.rstClass.val = CL_ALLINT;
   allPdus.m.rst.chanId.eh.pres = NOTPRSNT;

   /* TELICA - Bugzilla-4321 - Bukucu - 10.25.01 - new imp. of cntrs */
   inUpdSts(&pcb->sts.rstTx, pcb);
   /* send restart message */
   inGenPdu(cb, &pduHdr, &allPdus, pcb->cfg.swtch, (U32) MF_NA);

  
   /* Inform layer 4 that a restart message has been sent to
   * far end
   */
   inGenRstInd(cb, pcb, RSTSND, 0, &allPdus.m.rst);

   RETVALUE(LCM_REASON_NOT_APPL);
}






/*
 *
 *      Fun:   inBndEnableAllSaps
 *
 *      Desc: 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None
 *
 *      File:  qn_bdy11.c
 *
*/

#ifdef ANSI
PUBLIC S16 inBndEnableAllSaps
(
 void
 )
#else
     PUBLIC S16 inBndEnableAllSaps()
#endif
{
   S16 i;
   InPCB *pcb;
   S16 ret;
   InCtldPcb *ctldPcb;

   TRC3(inBndEnableAllSaps);

   i =0;

   for (i=0; i < inCp.nmbLnks; i++)
   {
      if ((pcb = *(pcbLstPtr + i)) == NULLP)
      {
         /* For Non Signalling Links the SAP is enabled */
         if ((ctldPcb = *(ctldPcbLstPtr + i)) != NULLP)
         {
            if ((inGetPcb(i, &ctldPcb, &pcb) == LCM_REASON_NOT_APPL) && pcb)
            {
               if ((ret =inEnableSap(ctldPcb, pcb, i)) != LCM_REASON_NOT_APPL)
               {
                  inGenAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_MI_INV_EVT,
                             LCM_CAUSE_BNDENA_FAIL, i);
               }
            }
            else
               inGenAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_MI_INV_EVT,
                          LCM_CAUSE_BNDENA_FAIL, i);
         }
      }
      else /* Signalling Link */
      {
         if ((ret =inBndEnableSap(pcb, ABND_ENA)) != LCM_REASON_NOT_APPL)
            inGenAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_MI_INV_EVT,
                       LCM_CAUSE_BNDENA_FAIL, i);
      } /* else */
   } 
   RETVALUE(LCM_REASON_NOT_APPL);
} /* end of inBndEnableAllSaps */




/*
 *
 *      Fun:   inUbndDisableAllSaps
 *
 *      Desc: 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None
 *
 *      File:  qn_bdy11.c
 *
*/

#ifdef ANSI
PUBLIC S16 inUBndDisableAllSaps
(
 U8 clrCalls
 )
#else
     PUBLIC S16 inUBndDisableAllSaps(clrCalls)
     U8 clrCalls;
#endif
{
   S16 i;
   InPCB *pcb;
   S16 ret;
   InCb *tCb;
   InCtldPcb *ctldPcb;
   InCb *dCb;
   InCtldPSapRst inCtldPcbRst;

   TRC3(inUBndDisableAllSaps);

   /* Disable Lower Saps */
   for (i=0; i < inCp.nmbLnks; i++)
   {
      /* For Signalling Interface */
      if ((pcb = *(pcbLstPtr + i)) != NULLP)
      {
         if ((ctldPcb = *(ctldPcbLstPtr + i)) == NULLP)
            inGenAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_MI_INV_EVT,
                       LCM_CAUSE_UBNDDIS_DLSAP_FAIL, i);
         else
            if ((ret = inUBndDisDLSAP(pcb, ctldPcb, clrCalls))
                != LCM_REASON_NOT_APPL)
               inGenAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_MI_INV_EVT,
                          LCM_CAUSE_UBNDDIS_DLSAP_FAIL, i);
      }
      else
      {
         if ((ctldPcb = *(ctldPcbLstPtr + i)) != NULLP)
         {
            if (((pcb = *(pcbLstPtr + ctldPcb->cfg.sigInt)) == NULLP) ||
                ((dCb = inGetSigCb(pcb)) == NULLP))
            {
               inGenAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_MI_INV_EVT,
                          LCM_CAUSE_UBNDDIS_DLSAP_FAIL, i);
            }
            else
            {
               inClearBChanCalls(ctldPcb, NULLP, dCb, AUBND_DIS);
               ctldPcb->state = CBST3DOWN;
               inCtldPcbRst.suId = ctldPcb->suId;
               inCtldPcbRst.calls = ALL_CALLS;
               zqRunTimeUpd(ZQ_CTLDPSAP_DELNTC_CB, CMPFTHA_UPD_REQ, 
                            (PTR)&inCtldPcbRst);
               zqRunTimeUpd(ZQ_CTLDPSAP_CB, CMPFTHA_UPD_REQ, (PTR)ctldPcb);    
            }
         }
      }
   }

  
   /* Disable Upper Saps */
   for (i=0; i < inCp.nmbSaps; i++)
   {
      if ((tCb = *(inSapLstPtr + i)) != NULLP)
      {
         if ((ret =inUBndDisTSAP(i)) != ROK)
            inGenAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_MI_INV_EVT,
                       LCM_CAUSE_UBNDDIS_TSAP_FAIL, i);
      } /* else */
   }

   zqUpdPeer();

   RETVALUE(ROK);
} /* end of inUbndDisableAllSaps */




/*
 *
 *      Fun:   inHandleGrpDLSAP
 *
 *      Desc: 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None
 *
 *      File:  qn_bdy11.c
 *
*/

#ifdef ANSI
PUBLIC S16 inHandleGrpDLSAP
(
 ProcId dstProcId,
 U8 action,
 U8 clrCalls
 )
#else
     PUBLIC S16 inHandleGrpDLSAP(dstProcId, action, clrCalls)
     ProcId dstProcId;
     U8 action;
     U8 clrCalls;
#endif
{
   S16 i;
   InPCB *pcb;
   S16 ret;
   InCtldPcb *ctldPcb;

   TRC3(inHandleGrpDLSAP);
   /* MCAO_DBG */
   INDBGP(DBGMASK_MI,"inHandleGrpDLSAP: action = %d\n", action);
  
   for (i = 0; i < inCp.nmbLnks; i++)
   {
      /* Signalling Interfaces / Non Nfas */
      if ((pcb = *(pcbLstPtr + i)) != NULLP)
      {    
         if  (pcb->pst.dstProcId == dstProcId)
         {
            if (action == ABND_ENA)
               if ((ret =inBndEnableSap(pcb, ABND_ENA)) != LCM_REASON_NOT_APPL)
                  inGenAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_MI_INV_EVT,
                             LCM_CAUSE_BNDENA_FAIL, i);

            if (action == AUBND_DIS)

            {
               if ((ctldPcb = *(ctldPcbLstPtr + i)) == NULLP)
                  inGenAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_MI_INV_EVT,
                             LCM_CAUSE_UBNDDIS_DLSAP_FAIL, i);
               else
                  if ((ret = inUBndDisDLSAP(pcb, ctldPcb, clrCalls))
                      != LCM_REASON_NOT_APPL)
                     inGenAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_MI_INV_EVT,
                                LCM_CAUSE_UBNDDIS_DLSAP_FAIL, i);
               /* TELICA-jwh-08/17/2001:  Inform LM that interface is down */
                  else
                     inNmReport(LCM_CATEGORY_INTERFACE, LCM_EVENT_DOWN,
                                LCM_CAUSE_DCHAN,(U8) DCHAN, pcb->suId, (U8) 0);
            }
         }
      }    
      else 
      {
         if ((ctldPcb = *(ctldPcbLstPtr + i)) != NULLP)
         {   
            if ((inGetPcb(i, &ctldPcb, &pcb)) == 
                LCM_REASON_NOT_APPL)
            {
               if  (pcb && (pcb->pst.dstProcId == dstProcId))
               {
                  if (action == ABND_ENA)
                  {
                     if ((ret =inEnableSap(ctldPcb, pcb, i)) != LCM_REASON_NOT_APPL)
                     {
                        inGenAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_MI_INV_EVT,
                                   LCM_CAUSE_BNDENA_FAIL, i);
                     }
                  }
               } /* if dstProcId */
            }        
            else /* getPcb */
               inGenAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_MI_INV_EVT,
                          LCM_CAUSE_UNKNOWN, i);
         } /* ctldPcb */
      } /* else */
   } /* for */

   qwGrpRmSapUnBndDis(dstProcId);

   RETVALUE(LCM_REASON_NOT_APPL);
} /* end of inHandleGrpDLSAP */




/*
 *
 *      Fun:   inDisableGrpTSAP
 *
 *      Desc: 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None
 *
 *      File:  qn_bdy11.c
 *
*/

#ifdef ANSI
PUBLIC S16 inDisableGrpTSAP
(
 ProcId dstProcId
 )
#else
     PUBLIC S16 inDisableGrpTSAP(dstProcId)
     ProcId dstProcId;
#endif
{
   S16 i;
   InCb *tCb;
   S16 ret;

   TRC3(inDisableGrpTSAP);
  
   qwGrpCcSapUnBndDis(dstProcId);

   for (i = 0; i < inCp.nmbSaps; i++)
   {
      tCb = *(inSapLstPtr + i);
      if (tCb != NULLP)
      {
         if (tCb->pst.dstProcId == dstProcId)
         {
            if ((ret = inUnBndTSAP(i)) != LCM_REASON_NOT_APPL)
               inGenAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_MI_INV_EVT,
                          LCM_CAUSE_UBNDDIS_TSAP_FAIL, i);
         }
      }
   }
   RETVALUE(LCM_REASON_NOT_APPL);
} /* end of inDisableGrpTSAP */







/*
 *
 *      Fun:   inShutDown
 *
 *      Desc: 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None
 *
 *      File:  qn_bdy11.c
 *
*/

#ifdef ANSI
PUBLIC S16 inShutDown
(
 void
 )
#else
     PUBLIC S16 inShutDown()
#endif
{
   InCb *tCb;
   S16 i;
   S16 j;
   S16 cntr;
   S16 ret;
   S16 tmrNum;
   InPCB *pcb;
   InCtldPcb *ctldPcb;
   InCb *cb;
   CalRefTbl *entry;
   CalId *ent;
   U8 numCalls;
   InNtc *ntc;
   InNtc *tmpNtc;
   QwLwSap *bCb;

   TRC3(inShutDown);

  /* Clear All Upper Saps */
   for (i = 0; i < inCp.nmbSaps; i++)
   {
      tCb = *(inSapLstPtr + i);
      if (tCb != NULLP)
         *(inSapLstPtr + tCb->spId) = NULLP;
      else
         continue;
    
      if ((bCb = qwGetBCbPtr(tCb->sapCfg.swtch)) != NULLP)
      {
         if (bCb->state == QW_BND)
         {
            bCb->state = QW_UNBND;
         }
      }

      numCalls = tCb->tcrTbl.nmbEnt;
      ntc = NULLP;
      tmpNtc = NULLP;

      while (numCalls)
      {
         if (cmHashListGetNext (&tCb->tcrTbl, (PTR) tmpNtc, 
                                (PTR *)&ntc) == ROK)
         {
            cmHashListDelete (&tCb->tcrTbl, (PTR)ntc);
            /* TELICA-jwh-12/19/2000:  Clear the hash list insertion flag after removing ntc from the hash list. */
            ntc->hlInsrt =0;
         }
         numCalls--;
      }

      cmHashListDeinit(&tCb->tcrTbl);
      SPutSBuf(inInit.region, inInit.pool, tCb, 
               (Size) sizeof(InCb));
   }

   SPutSBuf(inInit.region, inInit.pool, inSapLstPtr,  
            (inCp.nmbSaps * (sizeof(InCb *))));


   /* Clear All Lower Saps */
   for (i=0; i < inCp.nmbLnks; i++)
   {
      /* For Signalling Interface */
      if ((pcb = *(pcbLstPtr + i)) != NULLP)
      {
         /* Clear All Ces */
         for (j=0; j < pcb->cfg.nmbCes; j++)
         {
            cb = *(pcb->cesLst + (SuId)j);
        
            if (cb != NULLP)
            {

               STOP_CB_TIMERS;

               /* Delete  NTC */
               cntr = cb->numCalRef;
               while (cntr)
               {
                  if ((entry = inGetCalRefEntry(cb,0)) != NULLP)
                  {
                     if (entry->ntc)
                     {
                        inClnNtc(entry->ntc);
                        cb->numCalRef--;
                        cb->inNtcPtr = entry->next;

                        if ((ret = SPutSBuf(inInit.region, inInit.pool, 
                                            entry->ntc,
                                            (Size) (sizeof(InNtc) + sizeof (CalRefTbl)))!= ROK))
                        {
#if (ERRCLASS & ERRCLS_ADD_RES)
                           INLOGERROR(ERRCLS_ADD_RES, EIN201, (ErrVal) ret,
                                      "inShutDown() failed, unable to release memory.");
#endif
                        }
                     }
                  }
                  cntr--;
               }        
          

               /*  Clear Suspended Calls */
               if (cb->numCalId)
               {
                  while (cb->numCalId)
                  {
                     if ((ent = inGetCalEntry(cb, 0)) != NULLP)
                     {
                        cb->numCalId--;
                        inClnNtc(ent->ntc);
                        if ((ret = SPutSBuf(inInit.region, inInit.pool, 
                                            ent, (Size)sizeof(CalId))) 
                            != ROK)
                        {
#if (ERRCLASS & ERRCLS_ADD_RES)
                           INLOGERROR(ERRCLS_ADD_RES, EIN748, (ErrVal) ret,
                                      "inShutDown() failed, unable to release memory.");
#endif
                           RETVALUE(LCM_REASON_MEM_NOAVAIL);
                        }
                     }
                  }
                  cb->callIdTbl = NULLP;
               }

               /* Clear CB */
               if ((ret = SPutSBuf(inInit.region, inInit.pool, cb, 
                                   sizeof(InCb))) != ROK)
               {
#if (ERRCLASS & ERRCLS_ADD_RES)
                  INLOGERROR(ERRCLS_ADD_RES, EIN749, ERRZERO,
                             "inShutDown() failed, unable to release memory.");
        
#endif
                  RETVALUE(LCM_REASON_MEM_NOAVAIL);
               }     

            } /* if cb */
         } /* for */
      
         STOP_PCB_TIMERS;
         *(pcbLstPtr + pcb->suId) = NULLP;


         if ((ret = SPutSBuf(inInit.region, inInit.pool, 
                             pcb->cesLst, 
                             (Size)((sizeof(InCb *))*pcb->cfg.nmbCes))) != ROK)
         {
#if (ERRCLASS & ERRCLS_ADD_RES)
            INLOGERROR(ERRCLS_ADD_RES, EIN747, ERRZERO,
                       "inShutDown() failed, unable to release memory.");
#endif
         }

         if (pcb->cfg.nfasInt)
         {        
            if ((ret = SPutSBuf(inInit.region, inInit.pool, pcb->ctldInt, 
                                (Size) (pcb->cfg.maxNmbNfasInt * 
                                        sizeof(U16)))) != ROK)
            {
#if (ERRCLASS & ERRCLS_ADD_RES)
               INLOGERROR(ERRCLS_ADD_RES, EIN876, ERRZERO,
                          "inShutDown() failed, unable to release memory.");
          
#endif
               RETVALUE(LCM_REASON_MEM_NOAVAIL);
            }
         } /* if nfas */

         /* TELICA-Bug#16149, rkhan */
         remIsdnIfCbQ(pcb);

         if ((ret = SPutSBuf(inInit.region, inInit.pool, pcb, 
                             sizeof(InPCB))) != ROK)
         {
#if (ERRCLASS & ERRCLS_ADD_RES)
            INLOGERROR(ERRCLS_ADD_RES, EIN877, ERRZERO,
                       "inShutDown() failed, unable to release memory.");
        
#endif
            RETVALUE(LCM_REASON_MEM_NOAVAIL);
         }
      } /* if pcb */
    

      if ((ctldPcb = *(ctldPcbLstPtr + i)) != NULLP)
      {
         STOP_CTLDPCB_TIMERS;
         if ((ret = SPutSBuf(inInit.region, inInit.pool, 
                             ctldPcb->bBearer,
                             (Size) ((ctldPcb->cfg.nmbBearChan + 
                                      ctldPcb->cfg.firstBChanNum) * 
                                     sizeof(InBearChn)))) != ROK)
         {
#if (ERRCLASS & ERRCLS_ADD_RES)
            INLOGERROR(ERRCLS_ADD_RES, EIN878, ERRZERO,
                       "inShutDown() failed, unable to release memory.");
#endif
            RETVALUE(LCM_REASON_MEM_NOAVAIL);
         }

         if ((ret = SPutSBuf(inInit.region, inInit.pool, ctldPcb, 
                             sizeof(InCtldPcb))) != ROK)
         {
#if (ERRCLASS & ERRCLS_ADD_RES)
            INLOGERROR(ERRCLS_ADD_RES, EIN879, ERRZERO,
                       "inShutDown() failed, unable to release memory.");
#endif
            RETVALUE(LCM_REASON_MEM_NOAVAIL);
         }

      } /* if ctldPcb */   
   } /* for nmbLnks */

   SDeregTmr(inInit.ent, inInit.inst, inCp.timeRes,inPrcCbTq);
   SDeregTmr(inInit.ent, inInit.inst, inCp.timeRes,inPrcNtcTq);
   SDeregTmr(inInit.ent, inInit.inst, inCp.timeRes,inPrcBChTq);
   SDeregTmr(inInit.ent, inInit.inst, inCp.timeRes,inPrcPcbTq);
   SDeregTmr(inInit.ent, inInit.inst, inCp.timeRes,inPrcCtldPcbTq);


   if ((ret = SPutSBuf(inInit.region, inInit.pool, inProfTbl, 
                       ((Size)inCp.nmbProfiles * 
                        (Size) (sizeof(InBearCfg))))) != ROK)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      INLOGERROR(ERRCLS_ADD_RES, EIN880, (ErrVal) ret,
                 "inShutDown() failed, unable to release memory.");
#endif
      RETVALUE(LCM_REASON_MEM_NOAVAIL);
   }

   if ((ret = SPutSBuf(inInit.region, inInit.pool, ctldPcbLstPtr, 
                       ((Size)((inCp.nmbLnks) * 
                               (sizeof(InCtldPcb *)))))) != ROK)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      INLOGERROR(ERRCLS_ADD_RES, EIN881, (ErrVal) ret,
                 "inShutDown() failed, unable to release memory.");
#endif
      RETVALUE(LCM_REASON_MEM_NOAVAIL);
   }


   if ((ret = SPutSBuf(inInit.region, inInit.pool, pcbLstPtr, 
                       ((Size)((inCp.nmbLnks) * 
                               (sizeof(InPCB *)))))) != ROK)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      INLOGERROR(ERRCLS_ADD_RES, EIN882, (ErrVal) ret,
                 "inShutDown() failed, unable to release memory.");
#endif
      RETVALUE(LCM_REASON_MEM_NOAVAIL);
   }

   /* return static memory */
   SPutSMem(inInit.region, inInit.pool); 

   /* Reinitialize inInit */
   inInitialize(inInit.ent, inInit.inst, inInit.region, inInit.reason);

   RETVALUE(LCM_REASON_NOT_APPL);
} /* end of inShutDown */




/*
 *
 *      Fun:   inDisableNfasSap
 *
 *      Desc: 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None
 *
 *      File:  qn_bdy11.c
 *
*/

#ifdef ANSI
PUBLIC S16 inDisableNfasSap
(
 InPCB *sigPcb,
 InCtldPcb *ctldPcb,
 InCb *cb,
 U8 action
 )
#else
     PUBLIC S16 inDisableNfasSap(sigPcb, ctldPcb, cb, action)
     InPCB *sigPcb;
     InCtldPcb *ctldPcb;
     InCb *cb;
     U8 action;
#endif
{
   InCb *bkupCb;
   InCb *dCb;
   S16 ret;
   U8 cbState;
   InCtldPSapRst inCtldPcbRst;

   TRC3(inDisableNfasSap);

   /* Controlled Interface */
   if (cb == NULLP)
   {
      if ((dCb = inGetSigCb(sigPcb)) == NULLP)
      {
         inClearBChanCalls(ctldPcb, NULLP, NULLP, AUBND_DIS);
      }
      else
         inClearBChanCalls(ctldPcb, NULLP, dCb, ADISIMM);

      ctldPcb->state = CBST3DOWN;
      inCtldPcbRst.suId =  ctldPcb->suId;
      inCtldPcbRst.calls = ALL_CALLS;
      zqRunTimeUpd(ZQ_CTLDPSAP_DELNTC_CB, CMPFTHA_UPD_REQ, (PTR)&inCtldPcbRst);
      zqRunTimeUpd(ZQ_CTLDPSAP_CB, CMPFTHA_UPD_REQ, (PTR)ctldPcb);
      zqUpdPeer();
      RETVALUE(LCM_REASON_MISC);
   }

   /* Backup Interface */

  /* If backup is getting disabled the calls on the backup need to get
     cleared */
  /* TELICA-jwh-12/08/2000:  Trildos weren't checking whether this NFAS has a backup */
   if (!sigPcb->cfg.buIntPr)
   {
      return(LCM_REASON_NOT_APPL);
   }
   else if (cb->sigState != ST_IS)
   {   
      if ((ret = inGetBkup(sigPcb, &bkupCb)) != LCM_REASON_NOT_APPL)
         RETVALUE(ret);
    
      if (bkupCb->sigState == ST_IS)
      {
         inClearBChanCalls(ctldPcb, NULLP, cb, ADISIMM);
         inCtldPcbRst.suId =  ctldPcb->suId;
         inCtldPcbRst.calls = ALL_CALLS;
         zqRunTimeUpd(ZQ_CTLDPSAP_DELNTC_CB, CMPFTHA_UPD_REQ, 
                      (PTR)&inCtldPcbRst);
      }
      /* clear all calls if both of themwill be MOOS */
      else if (bkupCb->sigState == ST_MOOS)
         inClearAllCalls(bkupCb, ALL_CALLS);
   } /* if not IS */
  
  

   cbState = cb->sigState;

   if (action == AMOOS)
      cb->sigState = ST_MOOS;
   else
      cb->sigState = ST_OOS;

  /* For Signalling Interface or for active bkup*/
   if ((sigPcb->cfg.buIntPr) && (cbState == ST_IS))
   {

      inClearBChanCalls(ctldPcb, NULLP, cb, ADISIMM);

      switch (action)
      {
         case ARST:
            break;
         case ADISIMM:
            if ((ret = inGetBkup(sigPcb, &bkupCb)) != LCM_REASON_NOT_APPL)
               RETVALUE(ret);
    
            if (bkupCb->sigState == ST_STB)
            {
               if (cb->state == CBST1XFER)
               {
                  cb->sigState= ST_BSY;
                  ctldPcb->state = CBST3DOWN;
                  cb->state = CBST3DOWN;
                  inStartCbTmr(TMR_DISC, cb);

                  /* TELICA-BUG:9203-mqin */
                  inNmReport(LCM_CATEGORY_INTERFACE, LCM_EVENT_DOWN,
                             LCM_CAUSE_DCHAN,(U8) DCHAN, cb->suId, (U8) 0);

                  InLiDatDiscReq(&cb->pst, cb->spId, Q_930, cb->ces, DAT_DM_RLS);
                  zqRunTimeUpd(ZQ_CTLDPSAP_CB, CMPFTHA_UPD_REQ, (PTR)ctldPcb);
                  zqRunTimeUpd(ZQ_CES_CB, CMPFTHA_UPD_REQ, (PTR)cb);
                  zqUpdPeer();
                  /* Need to stop processing at this pt  */
                  RETVALUE(LCM_REASON_MISC);
               }
            }
            break;

         case AMOOS:
            if ((ret = inGetBkup(sigPcb, &bkupCb)) != LCM_REASON_NOT_APPL)
               RETVALUE(ret);
            /* For NFAS network side, if backup is present try to connect link
               again */
            if ((sigPcb->cfg.intType == NETWORK) &&
                (bkupCb->sigState == ST_OOS))
            {
               bkupCb->sigState = ST_MB;
               bkupCb->state = CBST0CON;
               inStartCbTmr(TMR_CON, bkupCb);
               InLiDatConReq(&bkupCb->pst, bkupCb->spId, Q_930, bkupCb->ces, bkupCb->rSuId);
            }      

            /* If bkup is not in MOOS then we donot have to clear calls
               on the signalling, because they will get transferred to the 
               bkup */
            if ((cb->state == CBST1XFER) && 
                (bkupCb->sigState != ST_MOOS)) 
            {
               inStartCbTmr(TMR_DISC, cb);
               InLiDatDiscReq(&cb->pst, cb->spId, Q_930, cb->ces, DAT_DM_RLS);
               ctldPcb->state = CBST3DOWN;
               cb->state = CBST3DOWN;

               /* TELICA-BUG:9203-mqin */
               inNmReport(LCM_CATEGORY_INTERFACE, LCM_EVENT_DOWN,
                          LCM_CAUSE_DCHAN,(U8) DCHAN, cb->suId, (U8) 0);

               zqRunTimeUpd(ZQ_CTLDPSAP_CB, CMPFTHA_UPD_REQ, (PTR)ctldPcb);
               zqRunTimeUpd(ZQ_CES_CB, CMPFTHA_UPD_REQ, (PTR)cb);
               zqUpdPeer();
               RETVALUE(LCM_REASON_MISC);
            }
            break;
         default:
            RETVALUE(LCM_REASON_INVALID_ACTION);
      } /* switch action */
   }/* if signalling */


   RETVALUE(LCM_REASON_NOT_APPL);
}



/*
 *
 *      Fun:   inActivateBkup
 *
 *      Desc: 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None
 *
 *      File:  qn_bdy11.c
 *
*/

#ifdef ANSI
PUBLIC S16 inActivateBkup
(
 InCb *cb,
 InCb *bkupCb,
 InPCB *sigPcb,
 InCtldPcb *ctldPcb
 )
#else
     PUBLIC S16 inActivateBkup(cb, bkupCb, sigPcb, ctldPcb)
     InCb *cb;
     InCb *bkupCb;
     InPCB *sigPcb;
     InCtldPcb *ctldPcb;
#endif
{
   S16 ret;

   TRC3(inActivateBkup);
   if (cb->pst.dstProcId == bkupCb->pst.dstProcId)
   {
      if ( (ret = inClearCb(sigPcb, ctldPcb, NORMAL_CLEAR) 
            != LCM_REASON_NOT_APPL))
         RETVALUE(ret);
   }
   else
   {
      U8 oldBkupSigSt;
      
      inClearBChanCalls(ctldPcb, NULLP, cb, AUBND_DIS);
      if (cb->numCalRef)
      {
         inClearAllCalls(cb, NON_ACTIVE_CALLS);
         inXferActvCalls(bkupCb, cb);
      }
      cb->sigState = ST_OOS;
      oldBkupSigSt = bkupCb->sigState;
      bkupCb->sigState= ST_IS;
      
      if (oldBkupSigSt != ST_IS)
         inNmReport(LCM_CATEGORY_INTERFACE, LCM_EVENT_INSERVICE,
                    LCM_CAUSE_DCHAN,(U8) DCHAN, bkupCb->suId, (U8) 0);

      switch (sigPcb->cfg.swtch)
      {
         case SW_ATT4E:
         case SW_NI2:
         case SW_NTDMS100P:
         case SW_NTDMS250:
         case SW_NTNI:
         case SW_NTMCI:
            inActvInt(bkupCb);
            break;
         default:
            break;
      }

      cb->state = CBST3DOWN;
   }
   RETVALUE(LCM_REASON_NOT_APPL);
} /* inActivateBkup */


/***********************************************************************/
/*                Statistics Request Procedures                           */
/***********************************************************************/

/*
*
*      Fun:   inStsReqHandler
*
*      Desc: 
*
*      Ret:   ROK   - ok
*
*      Notes: None
*
*      File:  qn_bdy11.c
*
*/

#ifdef ANSI
PUBLIC S16 inStsReqHandler
(
 InMngmt *sts,
 Action action
 )
#else
     PUBLIC S16 inStsReqHandler(sts, action)
     InMngmt *sts;
     Action action;
#endif
{
   SuId sapId;
   S16 ret;
   InPCB *pcb;
   InCtldPcb *ctldPcb;

   TRC3(inStsReqHandler);

#ifdef IN_LMINT3
   sapId = sts->t.sts.suId;
#else
   sapId = sts->hdr.elmId.elmntInst1;
#endif /* IN_LMINT3 */

   if ((ret = inGetPcb(sapId, &ctldPcb, &pcb))
       != LCM_REASON_NOT_APPL)
      RETVALUE(ret);

   inSetStsCntr(sts, pcb);
  
   if (action == ZEROSTS)
   {
      inResetStsCntr(pcb);
   }

   RETVALUE(LCM_REASON_NOT_APPL);
} /* end of inStsReqHandler */



/*
 *
 *      Fun:   inSetStsCntr
 *
 *      Desc: 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None
 *
 *      File:  qn_bdy11.c
 *
*/

#ifdef ANSI
PUBLIC S16 inSetStsCntr
(
 InMngmt *sts,
 InPCB *pcb
 )
#else
     PUBLIC S16 inSetStsCntr(sts, pcb)
     InMngmt *sts;
     InPCB *pcb;
#endif
{
  TRC3(inSetStsCntr);
  
  if (!sts || !pcb)
  {
     INDBGP(DBGMASK_MI,"[PLM_IN]: sts(%p) or pcb(%p) is not configured yet!\n", sts, pcb);
     RETVALUE(ROK);
  }

  sts->t.sts.s.inDLSAP.alertingTx = pcb->sts.alertingTx;
  sts->t.sts.s.inDLSAP.callProcTx = pcb->sts.callProcTx;
  sts->t.sts.s.inDLSAP.connTx = pcb->sts.connTx;
  sts->t.sts.s.inDLSAP.connAckTx = pcb->sts.connAckTx;
  sts->t.sts.s.inDLSAP.progressTx = pcb->sts.progressTx;
  sts->t.sts.s.inDLSAP.setupTx = pcb->sts.setupTx;
  sts->t.sts.s.inDLSAP.setupAckTx = pcb->sts.setupAckTx;
  sts->t.sts.s.inDLSAP.rsmTx = pcb->sts.rsmTx;
  sts->t.sts.s.inDLSAP.rsmAckTx = pcb->sts.rsmAckTx;
  sts->t.sts.s.inDLSAP.rsmRejTx = pcb->sts.rsmRejTx;
  sts->t.sts.s.inDLSAP.suspTx = pcb->sts.suspTx;
  sts->t.sts.s.inDLSAP.suspAckTx = pcb->sts.suspAckTx;
  sts->t.sts.s.inDLSAP.suspRejTx = pcb->sts.suspRejTx;
  sts->t.sts.s.inDLSAP.usrInfoTx = pcb->sts.usrInfoTx;
  sts->t.sts.s.inDLSAP.holdTx = pcb->sts.holdTx;
  sts->t.sts.s.inDLSAP.holdAckTx = pcb->sts.holdAckTx;
  sts->t.sts.s.inDLSAP.holdRejTx = pcb->sts.holdRejTx;
  sts->t.sts.s.inDLSAP.retTx = pcb->sts.retTx;
  sts->t.sts.s.inDLSAP.retAckTx = pcb->sts.retAckTx;
  sts->t.sts.s.inDLSAP.retRejTx = pcb->sts.retRejTx;
  sts->t.sts.s.inDLSAP.discTx = pcb->sts.discTx;
  sts->t.sts.s.inDLSAP.relTx = pcb->sts.relTx;
  sts->t.sts.s.inDLSAP.relCmpltTx = pcb->sts.relCmpltTx;
  sts->t.sts.s.inDLSAP.rstTx = pcb->sts.rstTx;
  sts->t.sts.s.inDLSAP.rstAckTx = pcb->sts.rstAckTx;
  sts->t.sts.s.inDLSAP.segmentTx = pcb->sts.segmentTx;
  sts->t.sts.s.inDLSAP.congCntrlTx = pcb->sts.congCntrlTx;
  sts->t.sts.s.inDLSAP.infoTx = pcb->sts.infoTx;
  sts->t.sts.s.inDLSAP.notifyTx = pcb->sts.notifyTx;
  sts->t.sts.s.inDLSAP.statTx = pcb->sts.statTx;
  sts->t.sts.s.inDLSAP.statEnqTx = pcb->sts.statEnqTx;
  sts->t.sts.s.inDLSAP.servTx = pcb->sts.servTx;
  sts->t.sts.s.inDLSAP.servAckTx = pcb->sts.servAckTx;
  sts->t.sts.s.inDLSAP.facilTx = pcb->sts.facilTx;
  sts->t.sts.s.inDLSAP.facilAckTx = pcb->sts.facilAckTx;
  sts->t.sts.s.inDLSAP.facilRejTx = pcb->sts.facilRejTx;
  sts->t.sts.s.inDLSAP.regTx = pcb->sts.regTx;
  sts->t.sts.s.inDLSAP.alertingRx = pcb->sts.alertingRx;
  sts->t.sts.s.inDLSAP.callProcRx = pcb->sts.callProcRx;
  sts->t.sts.s.inDLSAP.connRx = pcb->sts.connRx;
  sts->t.sts.s.inDLSAP.connAckRx = pcb->sts.connAckRx;
  sts->t.sts.s.inDLSAP.progressRx = pcb->sts.progressRx;
  sts->t.sts.s.inDLSAP.setupRx = pcb->sts.setupRx;
  sts->t.sts.s.inDLSAP.setupAckRx = pcb->sts.setupAckRx;
  sts->t.sts.s.inDLSAP.rsmRx = pcb->sts.rsmRx;
  sts->t.sts.s.inDLSAP.rsmAckRx = pcb->sts.rsmAckRx;
  sts->t.sts.s.inDLSAP.rsmRejRx = pcb->sts.rsmRejRx;
  sts->t.sts.s.inDLSAP.suspRx = pcb->sts.suspRx;
  sts->t.sts.s.inDLSAP.suspAckRx = pcb->sts.suspAckRx;
  sts->t.sts.s.inDLSAP.suspRejRx = pcb->sts.suspRejRx;
  sts->t.sts.s.inDLSAP.usrInfoRx = pcb->sts.usrInfoRx;
  sts->t.sts.s.inDLSAP.holdRx = pcb->sts.holdRx;
  sts->t.sts.s.inDLSAP.holdAckRx = pcb->sts.holdAckRx;
  sts->t.sts.s.inDLSAP.holdRejRx = pcb->sts.holdRejRx;
  sts->t.sts.s.inDLSAP.retRx = pcb->sts.retRx;
  sts->t.sts.s.inDLSAP.retAckRx = pcb->sts.retAckRx;
  sts->t.sts.s.inDLSAP.retRejRx = pcb->sts.retRejRx;
  sts->t.sts.s.inDLSAP.discRx = pcb->sts.discRx;
  sts->t.sts.s.inDLSAP.relRx = pcb->sts.relRx;
  sts->t.sts.s.inDLSAP.relCmpltRx = pcb->sts.relCmpltRx;
  sts->t.sts.s.inDLSAP.rstRx = pcb->sts.rstRx;
  sts->t.sts.s.inDLSAP.rstAckRx = pcb->sts.rstAckRx;
  sts->t.sts.s.inDLSAP.segmentRx = pcb->sts.segmentRx;
  sts->t.sts.s.inDLSAP.congCntrlRx = pcb->sts.congCntrlRx;
  sts->t.sts.s.inDLSAP.infoRx = pcb->sts.infoRx;
  sts->t.sts.s.inDLSAP.notifyRx = pcb->sts.notifyRx;
  sts->t.sts.s.inDLSAP.statRx = pcb->sts.statRx;
  sts->t.sts.s.inDLSAP.statEnqRx = pcb->sts.statEnqRx;
  sts->t.sts.s.inDLSAP.servRx = pcb->sts.servRx;
  sts->t.sts.s.inDLSAP.servAckRx = pcb->sts.servAckRx;
  sts->t.sts.s.inDLSAP.facilRx = pcb->sts.facilRx;
  sts->t.sts.s.inDLSAP.facilAckRx = pcb->sts.facilAckRx;
  sts->t.sts.s.inDLSAP.facilRejRx = pcb->sts.facilRejRx;
  sts->t.sts.s.inDLSAP.regRx = pcb->sts.regRx;
  sts->t.sts.s.inDLSAP.cons = pcb->sts.cons;
  sts->t.sts.s.inDLSAP.discs = pcb->sts.discs;
  sts->t.sts.s.inDLSAP.aveCalls = pcb->sts.aveCalls;
  sts->t.sts.s.inDLSAP.lstTxCause = pcb->sts.lstTxCause;
  sts->t.sts.s.inDLSAP.lstTxDgn = pcb->sts.lstTxDgn;
  sts->t.sts.s.inDLSAP.lstRxCause = pcb->sts.lstRxCause;
  sts->t.sts.s.inDLSAP.lstRxDgn = pcb->sts.lstRxDgn;
  RETVALUE(ROK);
} /* end of inSetStsCntr */


PUBLIC void inAssociateCtldPcb(SuId ctldInt, SuId sigInt, U32 rSigInt,  SuId bupInt, U32 rBupInt)
{
   InCtldPcb *ctldPcb;
   InPCB *pcb;
   /* shouldn't be both non-zero or zero */
   if ((sigInt && bupInt) || (!sigInt && !bupInt))
   {
      INDBGP(DBGMASK_MI,"inAssociateCtldPcb: invalid suId combination: sigInt= %d, rSigInt =%ld, bupInt = %d, rBupInt=%ld\n",
         sigInt, rSigInt, bupInt, rBupInt);
      return;
   }

   /* get ctldPcb */
   ctldPcb = *(ctldPcbLstPtr + ctldInt);

   if (ctldPcb == NULLP)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      INLOGERROR(ERRCLS_INT_PAR, EIN734, (ErrVal) ctldInt,
                 "inAssociateCtldPcb() failed, incorrect instance value.");
#endif
      return;
   }

   if (sigInt)
   {
      DP_PLMIN(DBGMASK_L1, "inAssociateCtldPcb: updating sigInt: ctldInt = %d, sigInt = %d, rSigInt = %ld.\n",
      ctldInt, sigInt, rSigInt);
      ctldPcb->cfg.sigInt = sigInt;
      ctldPcb->cfg.rSigInt = rSigInt;
   }

   /* get Pcb if it's a D link*/
   pcb = *(pcbLstPtr + ctldInt);
   if (pcb == NULLP)
      return;

   if (bupInt)
   {
      pcb->cfg.buIntPr = TRUE;
      DP_PLMIN(DBGMASK_L1, "inAssociateCtldPcb: updating bupInt: ctldInt = %d, bupInt = %d, rBupInt = %ld.\n", ctldInt, bupInt, rBupInt);
      pcb->cfg.bupInt = bupInt;
      pcb->cfg.rBupInt = rBupInt;
   }
}

PUBLIC void inDeAssociateCtldPcb(InCtldPcb *ctldPcb)
{
   S16 i;
   InPCB *sigPcb;
   U8 linkType = IN_LNK_TYPE_UNKNOWN;
   InPCB *bkSigPcb = NULLP;

   /* sig Lnk not configured yet */
   if (ctldPcb->cfg.sigInt == 0)
      return;

  /* PRI doesn't require a update */
   if (!ctldPcb->cfg.nfasInt)
      return;

  /* NFAS (w or w/o backup) should check sigLnk first, then bkLnk;
     if sigPcb disappeared, bkSigPcb should already gone too */
   sigPcb = *(pcbLstPtr + ctldPcb->cfg.sigInt);
  
   if (sigPcb == NULLP)
   {
      INDBGP(DBGMASK_MI,"zqDeAssociateCtldPcb() found Pcb is already deleted for rSuId = %ld,"
         "go on delete ctldPcb ...\n", ctldPcb->cfg.rSigInt);
      return;
   }

   /* identify link type */
   if (sigPcb->cfg.buIntPr)
   {
      if (sigPcb->cfg.sigInt == ctldPcb->suId)
         linkType = IN_LNK_TYPE_D1;
      else if (sigPcb->cfg.bupInt == ctldPcb->suId)
         linkType = IN_LNK_TYPE_D2;
      else 
         linkType =  IN_LNK_TYPE_B;
   }
   else 
   {
      if (sigPcb->cfg.sigInt == ctldPcb->suId)
         linkType = IN_LNK_TYPE_D_NFAS;
      else
         linkType = IN_LNK_TYPE_B;
   } 

   /* modify ctldInt list of signaling link */
   for (i = 0; i < (S16) sigPcb->cfg.maxNmbNfasInt; i++)
      if (*(sigPcb->ctldInt+i) == ctldPcb->suId)
      {
         *(sigPcb->ctldInt+i) = INT_NOT_CFGD;
         sigPcb->nmbCtldInt--;
         break;
      }

   /* backup link update */
   if (sigPcb->cfg.buIntPr)
   {   
      bkSigPcb = *(pcbLstPtr + sigPcb->cfg.bupInt);
      if (bkSigPcb != NULLP)
      {
         /* modify ctldInt list of backup  signaling link */
         for (i = 0; i < (S16) bkSigPcb->cfg.maxNmbNfasInt; i++)
            if (*(bkSigPcb->ctldInt+i) == ctldPcb->suId)
            {
               *(bkSigPcb->ctldInt+i) = INT_NOT_CFGD;
               bkSigPcb->nmbCtldInt--;
               break;
            }
      }
      else
      {
         INDBGP(DBGMASK_MI,"zqDeAssociateCtldPcb() found bkup pcb is already deleted for rSuId = %ld."
            "linktype = %d , go on delete ctldPcb ...\n", sigPcb->cfg.rBupInt, linkType);
         sigPcb->cfg.buIntPr = FALSE;
         sigPcb->cfg.bupInt = 0;
      }
   }
  
   switch(linkType)
   {
      case IN_LNK_TYPE_D_NFAS:
      case IN_LNK_TYPE_D1:
      {
         if(bkSigPcb) /* should not have backup existing when  deleting primary */
         {
#if (ERRCLASS & ERRCLS_INT_PAR)
            INLOGERROR(ERRCLS_INT_PAR, EIN708, (ErrVal)bkSigPcb->rSuId,
                       "zqDeAssociateCtldPcb() failed, invalid DLSAP.");
#endif
            return;
         }

         /* reset sigInt of all ctldPcb */
         for (i = 0; i < (S16) sigPcb->cfg.maxNmbNfasInt; i++)
            if (*(sigPcb->ctldInt+i) != INT_NOT_CFGD)
            {
               InCtldPcb *tempCtldPcb;

               /* get ctldPcb */
               tempCtldPcb = *(ctldPcbLstPtr + (*(sigPcb->ctldInt+i)));     
               if (tempCtldPcb == NULLP)
               {
#if (ERRCLASS & ERRCLS_INT_PAR)
                  INLOGERROR(ERRCLS_INT_PAR, EIN734, (ErrVal) *(sigPcb->ctldInt+i),
                             "zqDeAssociateCtldPcb() failed, incorrect instance value.");
#endif
               }
               else /* Bug 88902 */
               {
                  tempCtldPcb->cfg.sigInt = 0;
               }

            }  

         break;
      }
      case IN_LNK_TYPE_D2:
         INDBGP(DBGMASK_MI,"zqDeAssociateCtldPcb() linktype is D2, go on delete ctldPcb ...\n");
         sigPcb->cfg.buIntPr = FALSE;
         sigPcb->cfg.bupInt = 0;
         return;
      case IN_LNK_TYPE_B:
      default:
         break;
   }
  
   return;
}

void inStopAllBchanTimers(InPCB *pcb)
{
   InCtldPcb *ctldPcb;
   SuId suId = 0;
   InBearChn *bearChn = NULL;
   U16  i, j;
   U8 tmrNum;

   if (pcb->cfg.nfasInt)
   {
      for (i = 0; i < (S16) pcb->cfg.maxNmbNfasInt; i++)
      {
         if (*(pcb->ctldInt+i) != INT_NOT_CFGD)
         {
            suId = *(pcb->ctldInt+i);

            if ((ctldPcb = *(ctldPcbLstPtr + suId)) == NULLP)
            {
#if (ERRCLASS & ERRCLS_DEBUG)
               INLOGERROR(ERRCLS_DEBUG, EIN838, (ErrVal) suId,
                          "inUnBndCb() failed, unable to access lower SAP.");
#endif
               continue;
            }

            for (j = ctldPcb->cfg.firstBChanNum;
                 j < (ctldPcb->cfg.nmbBearChan + ctldPcb->cfg.firstBChanNum);
                 j++)
            {
               IN_GET_BEARER(ctldPcb, j, ctldPcb->cfg.firstBChanNum, bearChn);

               /* stop b channel timers */
               STOP_BCHAN_TIMERS;
            }
         }
      }
   }
   else
   {
      if ((ctldPcb = *(ctldPcbLstPtr + pcb->suId)) != NULLP)
      {
         for (i = ctldPcb->cfg.firstBChanNum;
              i < (ctldPcb->cfg.nmbBearChan + ctldPcb->cfg.firstBChanNum);
              i++)
         {
            IN_GET_BEARER(ctldPcb, i, ctldPcb->cfg.firstBChanNum, bearChn);

            /* stop b channel timers */
            STOP_BCHAN_TIMERS;
         }
      }
   }
}



/*
 * Function: inGeoHandleBChannel(cntrl)
 *
 * Description: This function will set all the B-channel status to
 * to AVAIL and all D-Chan state to IS.
 * 
 */

S16
inGeoHandleBChannel(InMngmt *cntrl)
{
   InPCB *pcb;
   InCtldPcb *ctldPcb;
   InCb      *cb;
   ISDNIFSTS_QUEUE_t *ifQCb;
   U32 dChanCntr = 0;   /* number of D-chan intf configured */

   pcb = NULLP;
   ctldPcb = NULLP;
   cb = NULLP;
   ifQCb = NULLP;

   if ( cntrl == NULLP ) 
   {
       DP("%s: Rx Invalid Cntrl msg from LM. No preconditioning of D/B channels done.\n",
	                 __FUNCTION__);
       return ROK;
   }


   DP_PLMIN(DBGMASK_L1,"%s: Rx Cntrl msg from LM to precondition all  D/B-Chan state before Geo Audit\n",__FUNCTION__);

   /*
    * get pcb->suId from link list "stsUpdIsdnIfCbQ" first. This will be used to
    * get the pcb and ctldPcb ptrs.
    */

   /* loop thru all Isdn Ifc's stored in stsUpdIsdnIfCbQ link list  */
   DP("%s: found %ld Isdn Infc configured in stsUpdIsdnIfCbQ\n", 
          __FUNCTION__, stsUpdIsdnIfCbQ.Count);

   for (dChanCntr = 1; dChanCntr <= stsUpdIsdnIfCbQ.Count; dChanCntr++)
   {
      DEQUEUE(&stsUpdIsdnIfCbQ,  ifQCb, ISDNIFSTS_QUEUE_t *);
      if (ifQCb == NULLP)
      {
         gitDebugPrint(__FILE__, __LINE__,
                       "ISDN:NGC, NULL pointer ifQCb found in stsUpdIsdnIfCbQ\n");
         continue;
      }

      /* get pcb and ctldPcb using the suId from the link list */
      if (inGetPcb(ifQCb->suId, &ctldPcb, &pcb) == LCM_REASON_NOT_APPL)
      {
         INDBGP(DBGMASK_MI,"[%ld] dump_isdn_pcb:: rSuId: %ld, spId: %d, bndState: %u,"
              "dChan.numCon: %d, nmbCtldInt: %u, rstAll: %s\n",
              dChanCntr, pcb->rSuId, pcb->spId, pcb->bndState, pcb->dChan.numCon, pcb->nmbCtldInt,
              pcb->rstAll ? "TRUE":"FALSE");

         /* get Q.931 Control Block - cb */
	 if ((cb = inGetCBPtr(pcb->suId, CONN_CES)) != NULLP)
	 {
	    /*set D-chan state and update Peer */

            INDBGP_CB(DBGMASK_MI,"%s: Setting dChanLnk %ld to cb->state = CBST1XFER, "
	                         "cb->sigState = ST_IS\n", __FUNCTION__, cb->rSuId);

            cb->state = CBST1XFER;
            /*Bug81892: for the nfasbackup case, if suId is for the bk link
             *set the sigState into ST_STB
             */
            if((pcb->cfg.nfasInt) && (pcb->cfg.buIntPr) && (pcb->cfg.bupInt == pcb->suId))
            {
               cb->sigState = ST_STB;
            }
            else
            {
               cb->sigState = ST_IS;
            }

            /* Bug 71341: set flag for NFASBACKUP */
            /*if ((pcb->cfg.nfasInt) && (pcb->cfg.buIntPr))*/
	    /* Bug 81522: set flg both for NFAS & NFASBACKUP */
	    if (pcb->cfg.nfasInt)
            {
               cb->nfas_gr = TRUE;
            }

            zqRunTimeUpd(ZQ_CES_CB, CMPFTHA_UPD_REQ, (PTR)cb);
	    
	    /* Now precondition  status of B-Channels for this D-Link */
	    
	    DP("%s: Preconditioning B-Channels for Isdn dChanLink = %ld\n", __FUNCTION__, pcb->rSuId);
	    inGeoPreCondBChannel(cb, pcb);
         }
	 else
	 {
	    DP("%s: inGetCPPtr return cb with NULLP for dChanLink = %ld\n", __FUNCTION__, pcb->rSuId);
	 }

      }
      else
      {
         gitDebugPrint(__FILE__, __LINE__,
                       "ISDN:NGC, NULL pointer found in stsUpdIsdnIfCbQ\n");
      }

      /* Put this entry back in the queue */
      ENQUEUE(&stsUpdIsdnIfCbQ, ifQCb, ISDNIFSTS_QUEUE_t *);
      ifQCb = NULLP;

   } /* for loop */

   RETVALUE(ROK);
}



PUBLIC void 
inGeoPreCondBChannel(InCb *cb, InPCB *pcb)
{
   InCtldPcb *ctldPcb;
   int j, bChan;
   InBearChn *bearChn;

   /* Loop thru all  B channels and set the status to AVAIL if the  B-Channel
    * is PROVISIONED and not MTCE BSY
    */

   if (pcb->cfg.nfasInt) /* NFAS */
   {
      for (j = 0; j < pcb->cfg.maxNmbNfasInt; j++)
      {
         if ((*(pcb->ctldInt+j) != INT_NOT_CFGD) &&
             (( ctldPcb = *(ctldPcbLstPtr + *(pcb->ctldInt+j)) ) != NULLP)) 
         {
            /* Set B channel status to AVAIL if PROVISIONED and not MTCE_BSY  */
            for (bChan = ctldPcb->cfg.firstBChanNum;
                 bChan < (ctldPcb->cfg.nmbBearChan + ctldPcb->cfg.firstBChanNum); bChan++)
            {
               IN_GET_BEARER(ctldPcb, bChan, ctldPcb->cfg.firstBChanNum, bearChn);

               INDBGP_CB(DBGMASK_MI,"%s:: rSuId = %ld bch->intId=%d chan=%d,"
	                "dChan=%d  state=%s   ntc=0x%lx\n",
                        __FUNCTION__, ctldPcb->rSuId, bearChn->intId, bearChn->chnNmb,
			bearChn->dChan, bChanStatusStr(bearChn->status),
			(U32)(bearChn->ntc));

               /* check that B-Chans are PROVISIONED and not MTCE_BSY */
               if ( (bearChn->status == NOTPROVISIONED) ||
	            (bearChn->status == MAINT_NE) || (bearChn->status == MAINT_FE) )
	       {
	          continue;
               }

               bearChn->status = AVAIL;
	       INDBGP_CB(DBGMASK_MI,"%s:: setting status of B-Chan %d to %s for infId = %d\n",
                   __FUNCTION__, bearChn->chnNmb, bChanStatusStr(bearChn->status), bearChn->intId);

               /* Inform RM of B-Channel State and update STBY CM  */
               zqRunTimeUpd(ZQ_BCHNL_CB, CMPFTHA_UPD_REQ, (PTR)bearChn);
            }
         }
      } /* for */
   }
   else /* PRI */
   {
      if ((ctldPcb = *(ctldPcbLstPtr + pcb->suId)) != NULLP)
      {
         for (bChan = ctldPcb->cfg.firstBChanNum;
              bChan < (ctldPcb->cfg.nmbBearChan + ctldPcb->cfg.firstBChanNum);bChan++)
         {
            IN_GET_BEARER(ctldPcb, bChan, ctldPcb->cfg.firstBChanNum,bearChn);

            INDBGP_CB(DBGMASK_MI,"%s:: rSuId = %ld bch->intId=%d chan=%d,"
	                "dChan=%d  state=%s ntc=0x%lx\n",
                        __FUNCTION__, ctldPcb->rSuId, bearChn->intId, bearChn->chnNmb,
			bearChn->dChan, bChanStatusStr(bearChn->status), 
			(U32)(bearChn->ntc));

            /* check that B-Chans are PROVISIONED and not MTCE_BSY */
            if ( (bearChn->status == NOTPROVISIONED) ||
                     (bearChn->status == MAINT_NE) || (bearChn->status == MAINT_FE) )
	    {
	       continue;
            }

            bearChn->status = AVAIL;
	    INDBGP_CB(DBGMASK_MI,"%s:: setting status of B-Chan %d to %s for infId = %d\n",
                   __FUNCTION__, bearChn->chnNmb, bChanStatusStr(bearChn->status), bearChn->intId);

            /* Inform RM of B-Channel State and update STBY CM  */
            zqRunTimeUpd(ZQ_BCHNL_CB, CMPFTHA_UPD_REQ, (PTR)bearChn);

         } /* end of for */

      } /* if */
   } /* end of else */
}

/*
 *      Fun:   inModReverse
 *
 *      Desc:  reverse digit string
 *
 *      Ret:   ROK   - void
 *
 *      Notes: None
 *
 *      File:  in_bdy11.c
 *
*/
PUBLIC void inModReverse (U8 *ptr, U8 len)
{           
    U8 tempVal = 0;
    U8 i       = 0;

    if(len > 1)
    {
        for(i=0; i<len/2; i++)
        {
            tempVal = *(ptr+i);
            *(ptr+i) = *(ptr+len-1-i);
            *(ptr+len-1-i) = tempVal;
        }
    } 

    return;
}

/*
 *      Fun:   inCallTraceCfgHandler
 *
 *      Desc:  ENT-TRC-CALL
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None
 *
 *      File:  in_bdy11.c
 *
 */
#ifdef ANSI
PUBLIC S16 inCallTraceCfgHandler 
(
 InMngmt *cfg
 )
#else
PUBLIC S16 inCallTraceCfgHandler(cfg)
     InMngmt *cfg;
#endif
{
    InCallTraceCfg  *inCallTraceCfg=NULLP;
    CmCallTraceCfg  cmCallTraceCfg;
    CmTreeCp        *traceIdTree=NULLP;

    if (!inInit.cfgDone)
    {
#if (ERRCLASS & ERRCLS_DEBUG)
        INLOGERROR(ERRCLS_DEBUG,EIN675,(ErrVal)inInit.cfgDone,
                    "inCallTraceCfgHandler() failed, configuration already done");
#endif

        RETVALUE(LCM_REASON_RECONFIG_FAIL);
    }

    inCallTraceCfg = &cfg->t.cfg.s.inCallTraceCfg;
    cmMemset((U8 *)&cmCallTraceCfg, 0, sizeof(CmCallTraceCfg));

    if (inCallTraceCfg->telNumDgts.length > SHRTADRLEN)
    {
#if (ERRCLASS & ERRCLS_INT_PAR)
        INLOGERROR(ERRCLS_INT_PAR, EIN688, (ErrVal)inCallTraceCfg->telNumDgts.length,
                 "inCallTraceCfgHandler() failed, incorrect telnum length.");
#endif
        RETVALUE(LCM_REASON_INVALID_PAR_VAL);
    }

    cmCallTraceCfg.telNumDgts = inCallTraceCfg->telNumDgts;
    cmCallTraceCfg.traceId    = inCallTraceCfg->traceId;

    /* FID 17167.0 */
    cmCallTraceCfg.logMask    = inCallTraceCfg->logMask;


   if(inCallTraceCfg->matchDirn == TRC_CALL_TELNUM_MATCHDIRN_LEFT)
   {
      traceIdTree = &inCallTraceLeft;
   }
   else if (inCallTraceCfg->matchDirn == TRC_CALL_TELNUM_MATCHDIRN_RIGHT)
   {
      traceIdTree = &inCallTraceRight;
      inModReverse(cmCallTraceCfg.telNumDgts.strg, cmCallTraceCfg.telNumDgts.length);
   }
   else
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
        INLOGERROR(ERRCLS_INT_PAR, EIN688, (ErrVal)inCallTraceCfg->matchDirn,
                 "inCallTraceCfgHandler() failed, incorrect matchDirn.");
#endif
        RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

   if (cmCallTraceAdd(traceIdTree,&cmCallTraceCfg) != ROK)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
        INLOGERROR(ERRCLS_INT_PAR, EIN688, (ErrVal)inCallTraceCfg->matchDirn,
                 "inCallTraceCfgHandler(), cmCallTraceAdd Error.");
#endif
        RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

   inNumOfActTrcRules++;

   INDBGP(DBGMASK_MI,"%s: traceId=%ld, telNumDgts.length=%d, telNumDgts.strg=%s, matchDirn=%d,activeRuls=%d, logMask=%ld\n",
                  __FUNCTION__,
                  inCallTraceCfg->traceId, 
                  inCallTraceCfg->telNumDgts.length, 
                  inCallTraceCfg->telNumDgts.strg,
                  inCallTraceCfg->matchDirn,
                  inNumOfActTrcRules,
                  inCallTraceCfg->logMask); 

   RETVALUE(LCM_REASON_NOT_APPL);
}

/*
 *      Fun:   inCntrlCallTrace
 *
 *      Desc:  DLT-TRC-CALL
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None
 *
 *      File:  in_bdy11.c
 *
 */
#ifdef ANSI
PUBLIC S16 inCntrlCallTrace
(
InMngmt *cntrl
)
#else
PUBLIC S16 inCntrlCallTrace(cntrl)
       InMngmt *cntrl;
#endif
{
    InCallTraceCfg  *inCallTraceCfg=NULLP;
    CmCallTraceCfg  cmCallTraceCfg;
    CmTreeCp        *traceIdTree=NULLP;

    if (!inInit.cfgDone)
    {
#if (ERRCLASS & ERRCLS_DEBUG)
        INLOGERROR(ERRCLS_DEBUG,EIN675,(ErrVal)inInit.cfgDone,
                    "inCntrlCallTrace() failed, configuration already done");
#endif

        RETVALUE(LCM_REASON_RECONFIG_FAIL);
    }

    inCallTraceCfg = &cntrl->t.cntrl.inCallTrace;

    cmMemset((U8 *)&cmCallTraceCfg, 0, sizeof(CmCallTraceCfg));

    if (inCallTraceCfg->telNumDgts.length > SHRTADRLEN)
    {
#if (ERRCLASS & ERRCLS_INT_PAR)
        INLOGERROR(ERRCLS_INT_PAR, EIN688, (ErrVal)inCallTraceCfg->telNumDgts.length,
                 "inCntrlCallTrace() failed, incorrect telnum length.");
#endif
        RETVALUE(LCM_REASON_INVALID_PAR_VAL);
    }

    cmCallTraceCfg.telNumDgts = inCallTraceCfg->telNumDgts;
    cmCallTraceCfg.traceId    = inCallTraceCfg->traceId;

   if(inCallTraceCfg->matchDirn == TRC_CALL_TELNUM_MATCHDIRN_LEFT)
      traceIdTree = &inCallTraceLeft;
   else if (inCallTraceCfg->matchDirn == TRC_CALL_TELNUM_MATCHDIRN_RIGHT)
   {
      traceIdTree = &inCallTraceRight;
      inModReverse(cmCallTraceCfg.telNumDgts.strg, cmCallTraceCfg.telNumDgts.length);
   }
   else
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
        INLOGERROR(ERRCLS_INT_PAR, EIN688, (ErrVal)inCallTraceCfg->matchDirn,
                 "inCntrlCallTrace() failed, incorrect matchDirn.");
#endif
        RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

   if (cmCallTraceRmv(traceIdTree,&cmCallTraceCfg) != ROK)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
        INLOGERROR(ERRCLS_INT_PAR, EIN688, (ErrVal)inCallTraceCfg->matchDirn,
                 "inCntrlCallTrace(), cmCallTraceRmv Error.");
#endif
        RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

   if(inNumOfActTrcRules != 0)
   {
      /* Decrease the number of active trace call */
       inNumOfActTrcRules--;
   }

   INDBGP(DBGMASK_MI,"%s: traceId=%ld, telNumDgts.length=%d,"
         " telNumDgts.strg=%s, matchDirn=%d,activeRuls=%d logMask=%ld\n",
                  __FUNCTION__,
                  inCallTraceCfg->traceId, 
                  inCallTraceCfg->telNumDgts.length, 
                  inCallTraceCfg->telNumDgts.strg,
                  inCallTraceCfg->matchDirn,
                  inNumOfActTrcRules,
                  inCallTraceCfg->logMask);

   RETVALUE(LCM_REASON_NOT_APPL);
}

/*
 *       Fun:  inMatchCallTraceId 
 *
 *       Desc:  Return List of trace Id matched CdPN & CgPN 
 *
 *       Ret:   void
 * 
 *       Notes: none
 *
 *       File:  in_bdy11.c
 *
 */
PUBLIC void inMatchCallTraceId(CdPtyNmb *cdPtyNmb, CgPtyNmb *cgPtyNmb, InNtc *ntc)
{
   ShrtAddrs cgpn;
   ShrtAddrs cdpn;
   CmCallTraceEntry *callTraceEntry[TRC_CALL_MAX_DUP_ENTRY];
   U32        traceIdArray[TRC_CALL_MAX_DUP_ENTRY]={0};
   CallTrcId *pTraceId=NULLP;
   U8        numLeft=0, numRight=0, numOfTraceId=0;
   U8        i, j;
   S16       ret=ROK;

   cmMemset((U8 *)&cgpn, 0, sizeof(ShrtAddrs));
   cmMemset((U8 *)&cdpn, 0, sizeof(ShrtAddrs));

   if(ntc == NULLP)
   {
      INDBGP(DBGMASK_MI,"%s:Null pointer ntc is passed in\n", __FUNCTION__);
      return;
   }
   pTraceId = &ntc->callTrcId;
   if(pTraceId->idList)
   {
      INDBGP(DBGMASK_MI,"%s: call trace is present for current call!!!",__FUNCTION__);
      return;
   }

   if(cdPtyNmb && cdPtyNmb->eh.pres && cdPtyNmb->nmbDigits.pres &&
         (cdPtyNmb->nmbDigits.len <= SHRTADRLEN))
   {
      cdpn.length = cdPtyNmb->nmbDigits.len;
      cmMemcpy(cdpn.strg, cdPtyNmb->nmbDigits.val, cdPtyNmb->nmbDigits.len);

      INDBGP(DBGMASK_MI,"%s: cdpn.length=%d, cdpn.strg=%s\n",
            __FUNCTION__,cdpn.length, cdpn.strg);

      if(cmCallTraceFind(&inCallTraceLeft,&cdpn, callTraceEntry,&numLeft) == ROK)
      {
         if((numLeft > 0) && (numLeft <= TRC_CALL_MAX_DUP_ENTRY) )
         {
            for(i=0; i<numLeft; i++)
            {
               for(j=0; j<callTraceEntry[i]->numTrace; j++)
               {
                  /* FID 17167.0 */
                  ntc->logMask |= callTraceEntry[i]->logMask[j];

                  traceIdArray[numOfTraceId]=callTraceEntry[i]->traceId[j];
                  INDBGP(DBGMASK_MI,"%s: trace id=%ld is found]\n", __FUNCTION__,traceIdArray[numOfTraceId]);
                  numOfTraceId++;
               }
            }
         }
      }

      inModReverse(cdpn.strg, cdpn.length);

      INDBGP(DBGMASK_MI,"%s: reversed cdpn.length=%d, cdpn.strg=%s\n",
            __FUNCTION__,cdpn.length, cdpn.strg);

      if(cmCallTraceFind(&inCallTraceRight, &cdpn, callTraceEntry,&numRight) == ROK)
      {
         if((numRight> 0) && (numRight<= TRC_CALL_MAX_DUP_ENTRY) )
         {
            for(i=0; i<numRight; i++)
            {
               for(j=0; j<callTraceEntry[i]->numTrace; j++)
               {
                  /* FID 17167.0 */
                  ntc->logMask |= callTraceEntry[i]->logMask[j];

                  traceIdArray[numOfTraceId]=callTraceEntry[i]->traceId[j];
                  INDBGP(DBGMASK_MI,"%s: trace id=%ld is found]\n", __FUNCTION__,traceIdArray[numOfTraceId]);
                  numOfTraceId++;
               }
            }
         }
      }
   }

   if(cgPtyNmb && cgPtyNmb->eh.pres && cgPtyNmb->nmbDigits.pres &&
       (cgPtyNmb->nmbDigits.len <= SHRTADRLEN))
   {
      numLeft=0;
      numRight=0;

      cgpn.length = cgPtyNmb->nmbDigits.len;
      cmMemcpy(cgpn.strg, cgPtyNmb->nmbDigits.val, cgPtyNmb->nmbDigits.len);

      INDBGP(DBGMASK_MI,"%s: cgpn.length=%d, cgpn.strg=%s\n",
            __FUNCTION__,cgpn.length, cgpn.strg);

      if(cmCallTraceFind(&inCallTraceLeft,&cgpn, callTraceEntry,&numLeft) == ROK)
      {
         if((numLeft > 0) && (numLeft <= TRC_CALL_MAX_DUP_ENTRY) )
         {
            for(i=0; i<numLeft; i++)
            {
               for(j=0; j<callTraceEntry[i]->numTrace; j++)
               {
                  /* FID 17167.0 */
                  ntc->logMask |= callTraceEntry[i]->logMask[j];

                  traceIdArray[numOfTraceId]=callTraceEntry[i]->traceId[j];
                  INDBGP(DBGMASK_MI,"%s: trace id=%ld is found]\n", __FUNCTION__,traceIdArray[numOfTraceId]);
                  numOfTraceId++;
               }
            }
         }
      }

      inModReverse(cgpn.strg, cgpn.length);
      INDBGP(DBGMASK_MI,"%s: reversed cgpn.length=%d, cgpn.strg=%s\n",
            __FUNCTION__, cgpn.length,cgpn.strg);

      if(cmCallTraceFind(&inCallTraceRight, &cgpn, callTraceEntry, &numRight) == ROK)
      {
         if((numRight> 0) && (numRight<= TRC_CALL_MAX_DUP_ENTRY) )
         {
            for(i=0; i<numRight; i++)
            {
               for(j=0; j<callTraceEntry[i]->numTrace; j++)
               {
                  /* FID 17167.0 */
                  ntc->logMask |= callTraceEntry[i]->logMask[j];

                  traceIdArray[numOfTraceId]=callTraceEntry[i]->traceId[j];
                  INDBGP(DBGMASK_MI,"%s: trace id=%ld is found]\n", __FUNCTION__,traceIdArray[numOfTraceId]);
                  numOfTraceId++;
               }
            }
         }
      }

   }

   if(numOfTraceId == 0)
   {
      INDBGP(DBGMASK_MI,"%s: no trace is found\n", __FUNCTION__);
      return;
   }

   for(i=0; i<numOfTraceId; i++)
   {
      pTraceId->count = numOfTraceId;

      ret = SGetSBuf(inInit.region, inInit.pool, (Data **)&pTraceId->idList, (Size)sizeof(U32)*pTraceId->count);
      if((ret != ROK) || (pTraceId->idList == NULLP))
      {
#if (ERRCLASS & ERRCLS_DEBUG)
          INLOGERROR(ERRCLS_DEBUG, EIN704, (ErrVal) sizeof(U16),
                       "inMatchCallTraceId() failed, unable to allocate memory.");
#endif
          pTraceId->count = 0;
          return;

      }
      else
      {
         pTraceId->region = inInit.region;
         pTraceId->pool   = inInit.pool;
         cmMemset((U8 *)pTraceId->idList, 0,(Size)sizeof(U32)*pTraceId->count);
         cmMemcpy((U8 *)pTraceId->idList, (U8 *)traceIdArray, (Size)sizeof(U32)*pTraceId->count);
      }
   }
   return;
}

/*
 *       Fun:   inSendCallTraceInfo
 *
 *       Desc:  build UDP packet for ISDN raw message in pcap format
 *              and send it to SLAM
 *
 *       Ret:   void
 * 
 *       Notes: none
 *
 *       File:  in_bdy11.c
 *
 */
PUBLIC void inSendCallTraceInfo(CallTrcId *pTraceId, Buffer *mBuf, U8 msgtype)
{
    char         trcHdr[128];
    char         pcapRaw[CALL_TRACE_MAX_RAW_MSG + 177];
    U8           isdnRaw[CALL_TRACE_MAX_RAW_MSG-4];
    DateTime     dt;
    struct timeval tm;
    int          tmpMsgSize = 0;
    int          trcHeaderLen = 0;
    int          pcapRawLen = 0;
    U16          checksum = 0;
    U32          tempsum=0;
    char         *tmpMsgPtr = NULLP;
    U16          sctpChunkLen=0;
    U16          isdnRawLen=0;
    U16          iuaProtocolLen=0;
    U16          iuaMsgLen=0;
    U8           paddinglen = 0;  /* IUA padding byte length */
    U8           i;
    U8           iuaMsgType=(msgtype==EVTDATDATREQ)?0x01:0x02;

    if((!pTraceId) || (pTraceId->count == 0) || (pTraceId->idList == NULLP))
    {
       INDBGP(DBGMASK_MI,"%s: no trace Id for this call\n", __FUNCTION__);
       return;
    }

    cmMemset((U8 *)trcHdr, 0,(Size)sizeof(trcHdr));
    cmMemset((U8 *)pcapRaw, 0,(Size)sizeof(pcapRaw));
    cmMemset((U8 *)isdnRaw, 0,(Size)sizeof(isdnRaw));

    /*************************************************** 
     * Format ISDN raw as below format to send it    | *
     * | to SLAM via UDP                               | *
     * |                                             | *
     * |---------------------------------------------| *
     * | TAG: 05 for ISDN  2 hex                     | *
     * |---------------------------------------------| *
     * | Trace ID count: 2 hex                       | *
     * |---------------------------------------------| *
     * | Trace IDs: 8 hex * Id count = max 80        | *
     * |---------------------------------------------| *
     * | PCAP data length: 4 hex                     | *
     * |---------------------------------------------| *
     * | PCAP Data| Time: 2012:12:31:18:23:58.568    | *
     * |          |----------------------------------| *
     * |          | IP header: 40 hex                | *
     * |          |----------------------------------| *
     * |          | SCTP header: 56 hex              | *
     * |          |----------------------------------| *
     * |          | IUA header: 52+padding hex       | *
     * |          |----------------------------------| *
     * |          | Q.931 raw start with protocol    | *
     * |          | discriminator 08(Q931)           | *
     * |          |----------------------------------| *
     * |          | possible IUA padding data        | *
     * |          |----------------------------------| *
     * *************************************************
     */
    tmpMsgPtr = trcHdr;

    /* TAG 0x05 for ISDN */
    tmpMsgSize = sprintf(tmpMsgPtr, "%02x", 0x05);
    tmpMsgPtr += tmpMsgSize;
    trcHeaderLen += tmpMsgSize;

    /* Trace Id count */
    tmpMsgSize = sprintf(tmpMsgPtr, "%02x", pTraceId->count);
    tmpMsgPtr += tmpMsgSize;
    trcHeaderLen += tmpMsgSize;

    /* TraceID list */
    for(i = 0; i < pTraceId->count; i++)
    {
        tmpMsgSize = sprintf(tmpMsgPtr, "%08x", (unsigned int)pTraceId->idList[i]);
        tmpMsgPtr += tmpMsgSize;
        trcHeaderLen += tmpMsgSize;
    }

    /* End of trace message header part */
    *tmpMsgPtr = '\0';

    INDBGP(DBGMASK_MI,"%s: ISDN trcHdr=%s\n", __FUNCTION__,trcHdr);

    /* Start to format raw ISDN to PCAP format */
    tmpMsgPtr = pcapRaw;

    /*#########################################*
     *            Time stamp                   * 
     *#########################################*/
    (Void) SGetDateTime(&dt); 
    gettimeofday(&tm, (struct timezone *)NULLP);
    tmpMsgSize = sprintf(tmpMsgPtr, "%4d:%02d:%02d:%02d:%02d:%02d.%03d %04x ",
                                    dt.year+1900,
                                    dt.month,
                                    dt.day,
                                    dt.hour,
                                    dt.min,
                                    dt.sec,
                                    (unsigned int)(tm.tv_usec/1000),
                                    0x0000);
    tmpMsgPtr += tmpMsgSize;
    pcapRawLen += tmpMsgSize;

    /*#############################################################*
     *            IP header                                        * 
     *#############################################################*
     * IP header is hardcoded as fake info except for total length *
     * and checksum                                                *
     *                                                             *
     *   Header version + header length: 0x45                      * 
     *   Service field:  0x00                                      * 
     *   Total length:   4 hex = 20(IP) + 28(SCTP) + iuaMsgLen     *
     *   Identification:  0x0000                                   *
     *   Flags:           0x00                                     *
     *   Fragment offset: 0x00                                     *
     *   Time to live:    0x40                                     *
     *   Protocol:        0x84 (SCTP)                              *
     *   Checksum:        4 hex base on calculation                *
     *   Source IP:       0x00000000                               *
     *   Destination IP:  0x00000000                               *
     ***************************************************************/
    SFndLenMsg(mBuf, &isdnRawLen);
    if(isdnRawLen > CALL_TRACE_MAX_RAW_MSG)
    {
       /* minus the max padding string length */
       isdnRawLen=CALL_TRACE_MAX_RAW_MSG - 4;
    }
    INDBGP(DBGMASK_MI,"%s: ISDN msglen=%d\n", __FUNCTION__,isdnRawLen);

    if(isdnRawLen%4 == 0)
    {
        paddinglen = 0;
    }
    else
    {
        paddinglen = 4 - (isdnRawLen%4);
    }
    INDBGP(DBGMASK_MI,"%s: paddinglen=%d\n", __FUNCTION__,paddinglen);

    /* message length in IUA header */
    iuaMsgLen = 28 + isdnRawLen + paddinglen;
    INDBGP(DBGMASK_MI,"%s: iuaMsgLen=%d\n", __FUNCTION__,iuaMsgLen);

    /* Protocol length in IUA header */
    iuaProtocolLen = 4 + isdnRawLen;
    INDBGP(DBGMASK_MI,"%s: iuaProtocolLen=%d\n", __FUNCTION__,iuaProtocolLen);

    /* Chunk length in SCTP header */
    sctpChunkLen = 16 + iuaMsgLen;
    INDBGP(DBGMASK_MI,"%s: sctpChunkLen=%d\n", __FUNCTION__,sctpChunkLen);

    /* checksum of IP header fields with 0x0000 will be ignored */
    tempsum = (U32)0x4500 + (U32)(20+28+iuaMsgLen) + (U32)0x4084; 
    checksum = (U16)~((tempsum & 0xffff) + (tempsum>>16));

    for(i=0; i<isdnRawLen; i++)
    {
        SExamMsg(&isdnRaw[i],mBuf,i);
        INDBGP(DBGMASK_MI,"%s: isdnRaw[%d]=%02x\n", __FUNCTION__,i, isdnRaw[i]);
    }
    isdnRaw[i] = '\0'; 

    tmpMsgSize = sprintf(tmpMsgPtr, "%4x%04x%04x%04x%04x%04x%08x%08x",
                            0x4500,
                            (20 + 28 + iuaMsgLen),
                            0x0,
                            0x0,
                            0x4084,
                            checksum,
                            0x0,
                            0x0);
    tmpMsgPtr  += tmpMsgSize;
    pcapRawLen += tmpMsgSize;


    /*#######################################################*
     *            SCTP header                                * 
     *#######################################################*
     * We hardcode a fake SCTP with variable chunk lenght only 
     * Source port: 0x0000
     * Dest port:   0x0000
     * Verification tag: 0x00000000
     * Checksum:    0x00000000
     * Data chunk
     *    chunk type: 0x00
     *    chunk flags: 0x03
     *    chunk length: sctpChunkLen
     *    TSN: fixed 0x00000001
     *    Stream identifier: 0x0001
     *    Stream sequence num: 0x0001
     *    Payload protocol identifier: 0x00000001
     */
    tmpMsgSize = sprintf(tmpMsgPtr, "%08x%08x%08x%04x%04x%08x%08x%08x",
                         0x0,
                         0x0,
                         0x0,
                         0x3,
                         sctpChunkLen,
                         0x1,
                         0x10001,
                         0x1);
    tmpMsgPtr  += tmpMsgSize;
    pcapRawLen += tmpMsgSize;

    /*#######################################################*
     *             IUA header                                * 
     *#######################################################*
     * Version: 0x01
     * Reserved: 0x00
     * Message class: 0x05
     * Message type: 0x02 (reserved)
     * Message length: iuaMsgLen
     * Integer interface identifier parameter
     *     Parameter tag: 0x0001
     *     Parameter length: 0x0008
     *     Integer interface identifier: 0x00000001
     *
     * DLCI 
     *     Parameter tag: 0x0005
     *     Parameter length: 0x0008
     *     SAPI: 0x00(call control procedure)
     *     TEI: 0x40
     *     Spare: 0x0000
     *
     * Protocol data parameter
     *     Parameter tag: 0x000e
     *     Parameter length: iuaProtocolLen
     *     Parameter padding: paddinglen (number of 00) 
     */
    tmpMsgSize = sprintf(tmpMsgPtr, "%06x%02x%08x%08x%08x%08x%08x%04x%04x",
                         0x010005,
                         iuaMsgType,
                         iuaMsgLen,
                         0x00010008,
                         0x1,
                         0x00050008,
                         0x00400000,
                         0x000e,
                         iuaProtocolLen);
    tmpMsgPtr  += tmpMsgSize;
    pcapRawLen += tmpMsgSize;

    /*#######################################################*
     *             ISDN raw message                          * 
     *#######################################################*/
    for(i=0; i<isdnRawLen; i++)
    {
        tmpMsgSize = sprintf(tmpMsgPtr,"%02x",isdnRaw[i]);
        tmpMsgPtr  += tmpMsgSize;
        pcapRawLen += tmpMsgSize;
    }

    /* Fill the padding data */
    for(i=0; i<paddinglen; i++)
    {
        tmpMsgSize = sprintf(tmpMsgPtr,"%02x", 0x00);
        tmpMsgPtr  += tmpMsgSize;
        pcapRawLen += tmpMsgSize;
    }

    /* End of Trace msg */                   
    *tmpMsgPtr = '\0';

    INDBGP(DBGMASK_MI, "%s: pcapRawLen=%d pcapRaw=%s\n", __FUNCTION__, pcapRawLen, pcapRaw);

    /*#############################################################*
     * Whole ISDN trace msg:                                       *
     *   Send to logger thread via log level PLOG_TRCINFO          *
     *   Trace msg includes 3 parts: trace header, pcapraw length  *
     *                             and pcapraw                     *
     *#############################################################*/
     MPprocLogMsg(0, PLOG_TRCINFO, 0, "%s%04x%s", trcHdr, pcapRawLen, pcapRaw);
    return;
}

/********************************************************************30**
                                                                     
         End of file: qn_bdy11.c 1.1  -  09/29/99 18:02:10
                                                                     
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
1.1            ----     pk  1.   Initial Release

1.2       in001.310     pk  1. Added clearing of inNtc in function
                               inShutDown.
                2. Added STOP_CTLDPCBTMR in function
                   inShutDown.
                3. Removed Break statement within if 
                   in function inCesCfg.
          in003.310     pk  4. Modifications for QW and ZQ.
          in005.310     sam 5. bugfixing for updating PSF during cntrlReq
          in008.310     pk  6. Added Initialization of ctldPcb->cfg.swtch.
                        cy  7. Added initialization for suId field in 
                               pcb->bBearer when a config request was called
                               for DLSAP.
          in009.310     cy  8. Added a correction in function 
                               inStsReqHandler.
                            9. Added counters in inSetStsCntr and 
                               inResetStsCntr.
                           10. initialized bBearer->rstCnt to 0 during 
                               configuration.
                           11. Initialized cb->tmrCnt in inDisableDLSAP.
          in010.310     cy 12. Instead of using inGetSapPtr, got the tCb 
                               pointer from inSapLstPtr directly in case of
                               the tCb might not be configured before.
          in011.310     pk 13. Fixed resetting of old signalling interface
                               in function inPrcCtldPcbCfg.
                           14. Added a new functionality to the unbind-
                               disable control request. The layer manager
                               can now issue an unbind-disable to the layer
                               in FT/HA scenarios to clear only the 
                               non-active calls by filling up a field 
                               called clearCalls in the control structure.
                           15. Modified functions inClearCb and 
                               inUBndDisDLSAP, inCntrlGrpDLSAP, 
                               inCntrlAllSaps, inUBndDisableAllSaps
                               inHandleGrpDLSAP to work for the new 
                               Unbind-Disable functionality.
                           16. Added Function inActivateBkup to activate
                               the backup link when the primry link is
                               disabled.
          in012.310     cy 17. Added code to sopport partial DLSAP 
                               reconfiguration.
                           18. Corrected inInitPcb. In reconfiguration, 
                               do not change pcb->bndState and the pst
                               structure.
          in013.310     cy 19. Removed a check for multipoint 
                               configuration.
                           20. Changed inCtldPcbCfgHandler and 
                               inPrcCtldPcbCfg to allow config the backup 
                               interface during run time for a NFAS case.
                           21. Added check for chanId IE before sending 
                               out a RESTART/RESTART message.
          in014.310     cy 22. Ignore the configuration request for
                               controlled DLSAP if that CntldSap is in
                               transfer state.
                           23. Removed compiling time warning.
                           24. Corrected ZQ update parameters for 
                               ZQ_CNTRLREQ_CB.
                           25. Corrected compile time flag for INLOGERR.
          in014.310     mm 26. Added tRest timer in Timer initialization
                               function.
          in015.310     cy 27. Called inClearAllCalls with flag 
                               NON_STABLE_NOIND for FTHA_CLEAR in 
                               inClearCb.
          in016.310     cy 28. Added a check for inDLSAP.buIntPr in 
                               inCtldPcbCfgHandler before updating 
                               NFAS parameters.
          in017.310     bb 29. Deleted the bCb state change. Now it is done
                               in inUnBndTSAP function.
                        cy 30. Changed nmbCalRef and nmbBearer to U32 
                               value in InGenCfg.
          in019.310     bb 31. Called InLiDatDiscReq with DAT_DM_RLS on
                               receiving a Control Request to disable the 
                               DLSAP in inDisbleDLSAP and inDisableNfasSap.
          in020.310     cy 32. Initialized allPdus in inRestartChan.
                           33. Reconfigure timers in inPartialRecfgDLSAP.
                           34. Put all corresponding QW lower SAP to 
                               unbound state in inShutDown.
          in021.310     cy 35. Send alarm to the layer manager when the
                               D channel becomes IN_SERVICE in NFAS case.
          in023.310     km 36. Changed link's parameter type in inNmReport 
                               and associated parameters.
          in025.310     bb 37. Initialised ctldPcb->rstCnt.
                           38. Initialized cb in inRestartInt if cb is 
                               NULLP.
          in026.310     bb 39. Stopped Timer TREST in case of Disable or 
                           unbind disable DLSAP.
                           40. Added checks to not to initiate Interface or 
                               All interface Restart procedure in case of 
                               ATT5EP.
          in028.310     cy 41. Initialized cb->facMBuf in inInitCb.
                        bb 42. Changed code to add ctldPcb->suId in Pcb at
                               a correct position.
                           43. Added checks so that addition of ctldPcb->suId
                               does not go beyond the pcb->ctldPcb array. 
          in029.310     yz 44. Added NULLP check in inFindOldInt.
                           45. Corrected reported ErrVal to avoid wrong report.
                       scc 46. initialize variables
                       scc 47. reduce unnecessary hash bins allocation.
          in030.310     yz 48. Added check return of SGetSBuf() to avoid NULLP.
                           49. Use maxNmbNfasInt instead of nmbCtldInt to avoid
                           missing interface.
                        yz 50. Modified code to get signal interface.
                        km 51. Modified Enable and Disable channel procedure 
                               to send service message to inform the peer if
                               service prodedure supported.
          in031.310     yz 52. Added check initCfg for initialing CtrlSap.
                           53. Modified code to ignore B-Channel restart
                               if there are outstanding restart.
          in033.310     yz 54. Added restart msg check in inRestartChan().
                        bb 55. Stopped timers running over  Cb, Pcb and
                               controlled Pcb in inUBndDisDLSAP. 
          in034.310     yz 56. Added get signal Cb in inRestartChan.
          in035.310     yz 57. Corrected an error in inRestartChan.
          in036.310     bb 58. Added changes for CORE2 support
                               (TELICA: won't work, removed).
          in039.310     ql 59. Change tmpInt define to U16.
                           60. Cast suId to U16 instead of U8.
*********************************************************************91*/

