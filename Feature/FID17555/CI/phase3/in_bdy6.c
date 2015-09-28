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

     Name:     Q.93x - body 6

     Type:     C source file

     Desc:     C source code for Q.93x Upper Layer, Lower Layer,
               System Service and Layer Management.

               Part 6: Network Matrix (2 of 2)

     File:     qn_bdy6.c

     Sid:      qn_bdy6.c 3.11  -  09/29/99 14:37:02

     Prg:      bn

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
#ifdef ETSI 
      -14      ETSI
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


/*

System services are the functions required by the protocol layer for
buffer management, timer management, date/time management, resource
checking and initialization.

It is assumed that the following functions are provided in the system
services service provider file:

     SInitQueue     Initialize Queue
     SQueueFirst    Queue to First Place
     SQueueLast     Queue to Last Place
     SDequeueFirst  Dequeue from First Place
     SDequeueLast   Dequeue from Last Place
     SFlushQueue    Flush Queue
     SCatQueue      Concatenate Queue
     SFndLenQueue   Find Length of Queue

     SGetMsg        Get Message
     SPutMsg        Put Message
     SInitMsg       Initialize Message

     SAddPreMsg     Add Pre Message
     SAddPstMsg     Add Post Message
     SRemPreMsg     Remove Pre Message
     SRemPstMsg     Remove Post Message
     SExamMsg       Examine Message
     SFndLenMsg     Find Length of Message
     SCatMsg        Concatenate Message
     SSegMsg        Segment Message

     SChkRes        Check Resources
     SLogError         Error

     SRegTmr        Register Activate Task - timer

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


/* header include files (.h) */

#include "envopt.h"        /* environment options */  
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */

#include "gen.h"           /* general layer */
#include "ssi.h"           /* system services */
#include "lin.h"           /* layer management */
#include "cm5.h"           /* timers */
#include "qn_db.h"         /* message functions - isdn */
#include "mf.h"            /* message funcions */
#include "int.h"           /* isdn network layer */
#include "dat.h"           /* data link layer */
#include "cm_hash.h"       /* hash-list header */
#include "in.h"            /* Q.93x */
#include "in_err.h"        /* Q.93x error */
#include "cm_pftha.h"      /* common PSF */
#include "lzq.h"           /* ISDN PSF management */
#include "zq.h"            /* ISDN PSF */
#include "zq_err.h"        /* ISDN PSF error codes */
#include "cct.h"                /* Call Control Interface Header */
#include "rmt.h"                /* Resource Manager Interface defines */
#include "cm_atm.h"             /* general ATM layer */
#include "cm_ss7.h"
#include "cm_cc.h"              /* Common Call Control Hash Defs */
#include "lqw.h"
#include "qw.h"           /* general layer */

/* header/extern include files (.x) */

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
#include "cm_lib.x"

/* local defines */

/* local typedefs */
  
/* local externs */
extern Bool mfTwoByte;

extern U8 callDir;

/* forward references */

/* public variable declarations */


/* private variable declarations */


/*
*     support functions
*/


/*
 *
 *       Fun:   inNetE19S00
 *
 *       Desc:  Input: Information message 
 *              State: null
 *                     
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
         File:  qn_bdy6.c
 *
*/

#ifdef ANSI
PUBLIC S16 inNetE19S00
(
InNtc *ntc,
InCb *dCb 
)
#else
PUBLIC S16 inNetE19S00(ntc, dCb)
InNtc *ntc;
InCb *dCb;
#endif
{
   InPCB *pcb;

   TRC2(inNetE19S00);

   /* get physical link control block */
   if ((pcb = *(pcbLstPtr + dCb->suId)) == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      INLOGERROR(ERRCLS_DEBUG, EIN400, (ErrVal) dCb->rSuId,
                 "inNetE19S00() failed, unable to access DLSAP.");
#endif
      RETVALUE(RFAILED);
   }

   inNetMSGINV(ntc, dCb);
   RETVALUE(ROK);
} /* end of inNetE19S00 */

  
/*
 *
 *       Fun:   inNetE19SND
 *
 *       Desc:  Input: Information
 *              State: Any
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
         File:  qn_bdy6.c
 *
*/

#ifdef ANSI
PUBLIC S16 inNetE19SND
(
InNtc *ntc,
InCb *dCb 
)
#else
PUBLIC S16 inNetE19SND(ntc, dCb)
InNtc *ntc;
InCb *dCb;
#endif
{
   InCb *tCb;
   InPCB *pcb;
   AllSdus ev;
   S16 ret;

   TRC2(inNetE19SND);

   /* get physical link control block */
   if ((pcb = *(pcbLstPtr + dCb->suId)) == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      INLOGERROR(ERRCLS_DEBUG, EIN402, (ErrVal) dCb->rSuId,
                 "inNetE19SND() failed, unable to access DLSAP.");
#endif
      RETVALUE(RFAILED);
   }

   switch (pcb->cfg.swtch)
   {
#ifdef ETSI                   /* TELICA-dsolanki-INTMGCDEV */
      case SW_ETSI:          /* switch - etsi */
#ifdef NETSIDE
#ifdef ISDN_PTMPT
         IN_PTMPT_ISSUE_STATUS(pcb, ntc, dCb);
#endif /* ISDN_PTMPT */
#endif /* NETSIDE */
#endif                       /* TELICA-dsolanki-INTMGCDEV */ 

      case SW_CCITT:
         inUpdSts(&pcb->sts.infoRx, pcb);

         tCb = ntc->tCallCb;
         MFINITSDU(&tCb->mfMsgCtl, ret, (U8)MI_INFO, (U8) PM_CNSTREQ, 
                   (ElmtHdr *) &ntc->pduSp->m.info, (ElmtHdr *) &ev, 
                   (U8) PRSNT_NODEF, tCb->sapCfg.swtch, ntc->callType);

         /* Send Status Indication to Upper */
         InUiIntCnStInd(&tCb->pst, tCb->suId, ntc->suInstId, ntc->tCallRef, 
                        &ev.m.cnStEvnt, MI_INFO, pcb->suId,dCb->ces);
         RETVALUE(ROK);

      default:
#if (ERRCLASS & ERRCLS_DEBUG)
         INLOGERROR(ERRCLS_DEBUG, EIN403, (ErrVal) pcb->cfg.swtch,
                    "inNetE19SND() failed, invalid message.");
#endif
         inIgnore(ntc, dCb);
         break;
   }
   RETVALUE(ROK);
} /* end of inNetE19SND */

  
/*
 *
 *       Fun:   inNetE19S24
 *
 *       Desc:  Input: Information
 *              State: Permanent Signal
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
         File:  qn_bdy6.c
 *
*/

#ifdef ANSI
PUBLIC S16 inNetE19S24
(
InNtc *ntc,
InCb *dCb 
)
#else
PUBLIC S16 inNetE19S24(ntc, dCb)
InNtc *ntc;
InCb *dCb;
#endif
{


   TRC2(inNetE19S24);

   RETVALUE(ROK);
} /* end of inNetE19S24 */

  
/*
 *
 *       Fun:   inNetE19S12
 *
 *       Desc:  Input: Information
 *              State: Disconnect Indication
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
         File:  qn_bdy6.c
 *
*/

#ifdef ANSI
PUBLIC S16 inNetE19S12
(
InNtc *ntc,
InCb *dCb 
)
#else
PUBLIC S16 inNetE19S12(ntc, dCb)
InNtc *ntc;
InCb *dCb;
#endif
{
   InCb *tCb;
   InPCB *pcb;
   AllSdus ev;
   S16 ret;

   TRC2(inNetE19S12);

   /* get physical link control block */
   if ((pcb = *(pcbLstPtr + dCb->suId)) == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      INLOGERROR(ERRCLS_DEBUG, EIN406, (ErrVal) dCb->rSuId,
                 "inNetE19S12() failed, unable to access DLSAP.");
#endif
      RETVALUE(RFAILED);
 
   }


   switch (pcb->cfg.swtch)
   {
      case SW_CCITT:
#ifdef ETSI                  /* TELICA-dsolanki-INTMGCDEV */
      case SW_ETSI:         /* switch - etsi */
#endif                      /* TELICA-dsolanki-INTMGCDEV */
         inUpdSts(&pcb->sts.infoRx, pcb);

         tCb = ntc->tCallCb;
         MFINITSDU(&tCb->mfMsgCtl, ret, (U8)MI_INFO, (U8) PM_CNSTREQ, 
                   (ElmtHdr *) &ntc->pduSp->m.info, (ElmtHdr *) &ev, 
                   (U8) PRSNT_NODEF, tCb->sapCfg.swtch, ntc->callType);

         /* Send Status Indication to Upper */
         InUiIntCnStInd(&tCb->pst, tCb->suId, ntc->suInstId, ntc->tCallRef, 
                        &ev.m.cnStEvnt, MI_INFO, pcb->suId,dCb->ces);
         RETVALUE(ROK);

      default:
         inIgnore(ntc, dCb);
         break;
   }
   RETVALUE(ROK);
} /* end of inNetE19S12 */

  
/*
 *
 *       Fun:   inNetE19S02
 *
 *       Desc:  Input: Information
 *              State: Overlap Sending
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
         File:  qn_bdy6.c
 *
*/

#ifdef ANSI
PUBLIC S16 inNetE19S02
(
InNtc *ntc,
InCb *dCb 
)
#else
PUBLIC S16 inNetE19S02(ntc, dCb)
InNtc *ntc;
InCb *dCb;
#endif
{
   InCb *tCb = NULL;
   InPCB *pcb = NULL;
   AllSdus ev;
   PduHdr pduHdr;
   AllPdus allPdus;
   U8 tmrNum = 0;
   S16 ret = 0;
#ifdef ISDN_ROUTE
   U8 buf[ADRLEN];
#endif /* ISDN_ROUTE */
   CdPtyNmb adr;
   RoutEntry *rout = NULL;
   UConnId ref = 0;
   InBearCfg *profTbl = NULL;
   InBearChn *bearChn = NULL;
   Bool sndCmp;  /* 2111 overlap */
   U8 i = 0;

   TRC2(inNetE19S02);

   UNUSED(i);
   UNUSED(bearChn);
   UNUSED(profTbl);
   UNUSED(ref);
   UNUSED(rout);
   UNUSED(adr);
   UNUSED(ret);
   UNUSED(tmrNum);
   UNUSED(allPdus);
   UNUSED(pduHdr);
   UNUSED(ev);
   UNUSED(pcb);
   UNUSED(tCb);

   UNUSED(ntc);
   UNUSED(dCb);

   tCb = NULLP;

#ifdef ISDN_OVERLAP

   /* get physical link control block */
   if ((pcb = *(pcbLstPtr + dCb->suId)) == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      INLOGERROR(ERRCLS_DEBUG, EIN407, (ErrVal) dCb->rSuId,
                 "inNetE19S02() failed, unable to access DLSAP.");
#endif
      RETVALUE(RFAILED);
   }

#ifdef IN_NUMSCREEN /*BUG: Add CDPN screening in Information receiving procedure*/
   if (inScreenCdPtyNum(ntc->pduSp->m.info.cdPtyNmb, pcb) != ROK)
   {
      /* reject the call */
      INSETTOKVAL(ntc->causeDgn.causeVal, CCINVNMBFRM);
      INSETTOKVAL(ntc->causeDgn.location, LOC_PUBNETLU);
      ntc->causeDgn.dgnVal.pres = PRSNT_NODEF;
      ntc->causeDgn.dgnVal.len = 1;
      ntc->causeDgn.dgnVal.val[0] = ME_CDPTYNMB;

      inGenRelCLw(ntc, &ntc->causeDgn, dCb);
      RETVALUE(ROK);
   }
#endif

   inUpdSts(&pcb->sts.infoRx, pcb);

   /* FID 16779.0 Replace NPPL with digit map */
   if ((pcb->cfg.swtch == SW_ETSI) && (pcb->cfg.overlap) &&
       (pcb->cfg.digitMapName.length > 0) && (ntc->check_info == TRUE))
   {
      if ((ntc->pduSp->m.info.cdPtyNmb.eh.pres)
            && (ntc->pduSp->m.info.cdPtyNmb.nmbDigits.len > 0))
      {
         U16 i;   
         InDigitMap *pDigitMap;                                
         TknStrM *nmbDigits = NULLP;
         Bool int_call = FALSE; /* only used for Poland */
         U8   typeNmb0 = ntc->pduSp->m.info.cdPtyNmb.typeNmb0.val;

         /* Setup doesn't take any digits.
          * Only check Poland information message to see if
          * this is a international number
          */
         if ((pcb->cfg.numPL == NPPL) &&
             (ntc->cdAdr.nmbDigits.len == 0) && (typeNmb0 == TON_INT))
         {
            int_call = TRUE;
         }

         /* Append digits received in information 
          * message to ntc->cdAr 
          */
         for (i = 0; i < ntc->pduSp->m.info.cdPtyNmb.nmbDigits.len; i++)
         {
            ntc->cdAdr.nmbDigits.val[i + ntc->cdAdr.nmbDigits.len] 
                 = ntc->pduSp->m.info.cdPtyNmb.nmbDigits.val[i];
         }

         ntc->cdAdr.nmbDigits.len 
                += ntc->pduSp->m.info.cdPtyNmb.nmbDigits.len;

         /* Only check Poland international number if received 
          * digits are no less than 2
          */
         if (pcb->cfg.numPL == NPPL && ntc->cdAdr.nmbDigits.len >= 2)
         {
            if (typeNmb0 == TON_UNK)
            {
               int ret_val;

               ret_val = cmStrncmp(ntc->cdAdr.nmbDigits.val, "00", 2);

               if (ret_val == 0) 
               {
                  int_call = TRUE;
               }
            }
         }

         if ( int_call != TRUE)
         {
            /* Search the digit map table to get the 
             * corresponding maxDigits if match is found.
             */
#if 0 /* Old code */
            ntc->numPL_max = inDigMapSrch (ntc->cdAdr.nmbDigits.val,
                                           ntc->cdAdr.nmbDigits.len);
#else /* Start of code change */
            if ((ret = cmHashListFind(&inGlobalCb.digitMapHl, pcb->cfg.digitMapName.strg,
                         pcb->cfg.digitMapName.length, (U16)0, (PTR *) &pDigitMap)) == ROK)
            {
               if (ntc->cdAdr.nmbDigits.len == ntc->pduSp->m.info.cdPtyNmb.nmbDigits.len ||
                   ntc->lastTypeNmb != typeNmb0)
               {
                  /* Firstly, discard the previous digit map candidate info */        
                  inResetDigitMapInfo(ntc);

                  /* Create new digit map candidate info */        
                  inCreateDigitMapInfo(ntc, pDigitMap, typeNmb0);
                  nmbDigits = &ntc->cdAdr.nmbDigits;
               }
               else
               {
                  nmbDigits = &ntc->pduSp->m.info.cdPtyNmb.nmbDigits;
               }
               ntc->lastTypeNmb = typeNmb0;

               /* Search digit map candidiate lists */
               inSearchDigitMap(ntc, pDigitMap, nmbDigits, typeNmb0);

               /* Get max digits */
               ntc->numPL_max = inGetMaxDigits(ntc, pDigitMap, typeNmb0);
               INDBGP(DBGMASK_LI, "maxDigits of digit map is %d\n", ntc->numPL_max);
#endif /* End of code change */

               if (ntc->numPL_max > 0)
               {
                  /* If match is found, sequential information message
                   * don't need to checked again.
                   */
                  ntc->check_info = FALSE;
               }
            }
         }
         else
         {
            /* International call don't need to 
             * make further check on information message
             */
            ntc->check_info = FALSE;
         }
      }
   }

   /* 2111 overlap */
   if ((pcb->cfg.swtch == SW_ETSI) && (pcb->cfg.overlap))
   {
      STOP_NTC_TIMER(TMR_T302);
      STOP_NTC_TIMER(TMR_TMIN);

      sndCmp = FALSE;

      if((ntc->pduSp->m.info.cdPtyNmb.eh.pres) &&
         (ntc->pduSp->m.info.cdPtyNmb.nmbDigits.pres))
      {
          U8 len;

          len = ntc->pduSp->m.info.cdPtyNmb.nmbDigits.len;
          ntc->nmbCdPty += len;
          if ((len != 0) &&
              (ntc->pduSp->m.info.cdPtyNmb.nmbDigits.val[len - 1] == '#'))
              sndCmp = TRUE;
      }

      if ((ntc->pduSp->m.info.sndCmplt.eh.pres) || (sndCmp) ||
          (IN_GOT_MAXDIGITS(ntc->nmbCdPty, IN_GOTMAX(ntc, pcb))))
      {
          /* Stop waiting */
          IN_CLR_WAITINFO(ntc->waitForMoreInfo);

      }
      else
      {
          if(IN_GOT_MINDIGITS(ntc->nmbCdPty, pcb->cfg.minDigits))
          {
             inStartNtcTmr(TMR_T302, ntc, ntc->dCallCb);
          }
          else
          {
             inStartNtcTmr(TMR_TMIN, ntc, ntc->dCallCb);
          }
      }

      /* Send Status Indication to Upper */
      inUpdSts(&pcb->sts.infoRx, pcb);
      MFINITSDU(&ntc->tCallCb->mfMsgCtl, ret, (U8) MI_INFO, (U8) PM_CNSTREQ,
                 (ElmtHdr *) &ntc->pduSp->m.info, (ElmtHdr *) &ev,
                 (U8) PRSNT_NODEF, ntc->tCallCb->sapCfg.swtch, ntc->callType);

      ev.m.cnStEvnt.optElmt.waitForMoreInfo = ntc->waitForMoreInfo;
      InUiIntCnStInd(&ntc->tCallCb->pst, ntc->tCallCb->suId,
              ntc->suInstId, ntc->tCallRef, &ev.m.cnStEvnt, MI_INFO,
              pcb->suId, dCb->ces);

      RETVALUE(ROK);
   } /* 2111 overlap */

   switch (pcb->cfg.swtch)
   {
      case SW_CCITT:
#ifdef ETSI                  /* TELICA-dsolanki-INTMGCDEV */
      case SW_ETSI:         /* switch - etsi */
#endif                      /* TELICA-dsolanki-INTMGCDEV */
      case SW_NTDMS100B:     /* switch - nt dms100 */
      case SW_ATT5EB:
         /* Stop T_302 */

         STOP_NTC_TIMER(TMR_T302);
      
         if ((pcb->cfg.swtch == SW_NTDMS100B) || 
             (pcb->cfg.swtch == SW_ATT5EB))
         {
            if (!ntc->infoCnt) 
            {
               ntc->infoCnt = 1;
            
               /* prepare Pdu header */
               inInitPduHdr((U8) PD_Q931, ntc->origin, pcb->cfg.callRefLen, 
                            ntc->dCallRef,  (U8) M_INFO, &pduHdr);
            
               MFINITPDU(&ntc->dCallCb->mfMsgCtl, ret, (U8) MI_INFO, 
                         (U8) MI_INFO, (ElmtHdr *) &ntc->pduSp->m.info, 
                         (ElmtHdr *) &allPdus, (U8) PRSNT_NODEF, 
                         pcb->cfg.swtch, ntc->callType);

               allPdus.m.info.signal.eh.pres = PRSNT_NODEF;
               INSETTOKVAL(allPdus.m.info.signal.signal, SIG_TONESOFF);

               inGenPdu(dCb, &pduHdr, &allPdus, pcb->cfg.swtch, 
                        ntc->callType);
            }
         }  /* switch == SW_NTDMS100B */
      
         if ((ntc->cmpltAdr) || (pcb->cfg.tCbCfg))
         {
            tCb = ntc->tCallCb;
            /* Start T_302 */
            inStartNtcTmr(TMR_T302, ntc, ntc->dCallCb);
            MFINITSDU(&tCb->mfMsgCtl, ret, (U8)MI_INFO, (U8) PM_CNSTREQ, 
                      (ElmtHdr *) &ntc->pduSp->m.info, (ElmtHdr *) &ev, 
                      (U8) PRSNT_NODEF, tCb->sapCfg.swtch, ntc->callType);
            /* Send Status Indication to Upper */
            InUiIntCnStInd(&tCb->pst, tCb->suId, ntc->suInstId, ntc->tCallRef, 
                           &ev.m.cnStEvnt, MI_INFO, pcb->suId,dCb->ces);
         }
         else
         {
#ifdef ISDN_ROUTE
            if (pcb->cfg.keyPad || (pcb->cfg.swtch == SW_ATT5EB)) 
               /* keyPad conveys call info */
            {
               adr.nmbDigits.len = ntc->pduSp->m.info.keyFac.keyInfo.len;
               inAscAdrToBcd(&ntc->pduSp->m.info.keyFac.keyInfo.val[0], 
                             &adr.nmbDigits.val[0]);
            }
            else           /* not a keyPad */
            {
               if (!(inGetDstNmb(ntc->dCallCb, &ntc->pduSp->m.info.cdPtyNmb,
                                 &adr)))
               {
               
                  INSETTOKVAL(ntc->causeDgn.causeVal, CCNORTTODEST);
                  IN_SWNT100B_GEN_PRG;
               }
            }

            ntc->numAdrDig = inStoreAdr(&ntc->dstAdr.nmbDigits.val[0], 
                                        ntc->dstAdr.nmbDigits.len, 
                                        &adr.nmbDigits.val[0],
                                        adr.nmbDigits.len, &buf[0]);

            ntc->dstAdr.nmbDigits.len = ntc->numAdrDig;

            if (pcb->cfg.keyPad || (pcb->cfg.swtch == SW_ATT5EB))
            {
               INSETTOKARRAY(ntc->pduSp->m.info.keyFac.keyInfo,
                             ntc->numAdrDig, buf);
            }
            else           /* not a keyPad */
            {
               for (i = 0; i < ntc->numAdrDig; i++)
                  ntc->pduSp->m.info.cdPtyNmb.nmbDigits.val[i] = buf[i];  
               ntc->pduSp->m.info.cdPtyNmb.nmbDigits.len = ntc->numAdrDig;
            }
            for (i = 0; i < ntc->numAdrDig; i++)
               ntc->dstAdr.nmbDigits.val[i] = buf[i];        
            if ((rout = inRoutSearch(&ntc->dstAdr.nmbDigits.val[0], 
                         ntc->dstAdr.nmbDigits.len, 
                         &pcb->cfg.wcMask[0], TSAP)) == NULLP)
            {
               if (ntc->pduSp->m.info.sndCmplt.eh.pres)
               {
                  INSETTOKVAL(ntc->causeDgn.causeVal, CCINVNMBFRM);
                  IN_SWNT100B_GEN_PRG;
                  RETVALUE(ROK);
               }
               else
                  inStartNtcTmr(TMR_T302, ntc, ntc->dCallCb);

               RETVALUE(ROK);
            }  /* if rout is nullp. */
            else
            {
               if (!(tCb = inGetSapPtr(rout->sapId)))
               {
                  INSETTOKVAL(ntc->causeDgn.causeVal, CCNORTTODEST);
                  IN_SWNT100B_GEN_PRG;
                  RETVALUE(ROK);
               }


               if (tCb->sapCfg.swtch != pcb->cfg.swtch)
               {
                  INSETTOKVAL(ntc->causeDgn.causeVal, CCINCOMPDEST);
                  IN_SWNT100B_GEN_PRG;
                  RETVALUE(ROK);
               }

               if (inGetInstId(tCb, &ref) == RFAILED)
               {
                  INSETTOKVAL(ntc->causeDgn.causeVal, CCRESCUNAVAIL);
                  IN_SWNT100B_GEN_PRG;
                  RETVALUE(ROK);
               }
         
               /*
                * If snd complete has been received, transmit a CALL PROCEEDING
                * and send a connect indication to the call control.
                */
               if (tCb->state == CBST1XFER)
               {
                  ntc->tCallCb = tCb;
                  ntc->tCallRef = ref;
                  inInsCallRefEnt1(ntc->tCallCb, ntc);
               
                  if (ntc->pduSp->m.info.sndCmplt.eh.pres)
                  {
                     ntc->cmpltAdr = TRUE;
                     /* Change state to outgoing call proceeding */
                     ntc->state = ST_OUTPROC;

                     inUpdSts(&pcb->sts.callProcTx, pcb);
                  
                     /* prepare Pdu header */
                     inInitPduHdr((U8) PD_Q931, ntc->origin, 
                                  pcb->cfg.callRefLen, ntc->dCallRef,
                                  (U8) M_CALLPROC, &pduHdr);
                  
                     MFINITPDU(&ntc->dCallCb->mfMsgCtl, ret, (U8) 0, 
                               (U8) MI_CALLPROC, NULLP,  (ElmtHdr *) &allPdus, 
                               (U8) PRSNT_DEF,  pcb->cfg.swtch, ntc->callType);
                     /* insert channel Id  */
                     inInsrtChnlId(ntc, &allPdus.m.callProc.chanId, 
                                   ntc->ctldPcb);
                     inGenPdu(dCb, &pduHdr,&allPdus,pcb->cfg.swtch,
                              ntc->callType);
                  }
                  /* Send Setup Indication to Upper */
                  MFINITSDU(&tCb->mfMsgCtl, ret,(U8) MI_INFO,(U8) PM_CONREQ,
                            (ElmtHdr *) &ntc->pduSp->m.info, (ElmtHdr *) &ev, 
                            (U8) PRSNT_NODEF, pcb->cfg.swtch, ntc->callType);
                  IN_GET_BEARER(ntc->ctldPcb, ntc->bChanSel.bChan[0], 
                                ntc->ctldPcb->cfg.firstBChanNum, bearChn);
                  profTbl = inProfTbl + bearChn->profile;
                  inBldElmtBearCap(profTbl, &ev.m.conEvnt.bearCap[0]);
                  /* insert channel Id  */
                  inInsrtChnlId(ntc, &ev.m.conEvnt.chanId,ntc->ctldPcb);
                  InUiIntConInd(&tCb->pst, tCb->suId, ntc->suInstId,
                                ntc->tCallRef,&ev.m.conEvnt, pcb->suId,
                                dCb->ces);
               }
               else
               {
                  INSETTOKVAL(ntc->causeDgn.causeVal, CCDESTOUTORD);
                  IN_SWNT100B_GEN_PRG;
                  RETVALUE(ROK);
               }
            }
#endif
         }
         break;
         
      default:
#if (ERRCLASS & ERRCLS_DEBUG)
         INLOGERROR(ERRCLS_DEBUG, EIN408, (ErrVal) pcb->cfg.swtch,
                    "inNetE19S02() failed, invalid message received, dropping data");
#endif
         inIgnore(ntc, dCb);
         break;
   }
#endif
   RETVALUE(ROK);
} /* end of inNetE19S02 */

  
/*
 *
 *       Fun:   inNetE27S00
 *
 *       Desc:  Connect Request
 *              State Null
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
         File:  qn_bdy6.c
 *
*/

#ifdef ANSI
PUBLIC S16 inNetE27S00
(
InNtc *ntc,
InCb *dCb 
)
#else
PUBLIC S16 inNetE27S00(ntc, dCb)
InNtc *ntc;
InCb *dCb;
#endif
{
   TknU16 intId;
   InPCB *pcb;
   ConEvnt *conEvnt;
   PduHdr pduHdr;
   AllPdus allPdus;
   Buffer *mBuf;
   S16 ret;
   U8 i;
   U8 protDisc;
   Buffer *tmpBuf;

   TRC2(inNetE27S00);

   callDir = CALL_OUT;

   /* get physical link control block */
   if ((pcb = *(pcbLstPtr + dCb->suId)) == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      INLOGERROR(ERRCLS_DEBUG, EIN409, (ErrVal) dCb->rSuId,
                 "inNetE27S00() failed, unable to access DLSAP.");
#endif
      RETVALUE(RFAILED);
   }


   /*
    * Obtain the b channel required for this connection.
    * incase of error return with a release indication.
    */

   conEvnt = (ConEvnt *) ntc->sduSp;

   mfTwoByte = FALSE;
   if ((conEvnt->chanId.eh.pres) &&
       (conEvnt->chanId.chanMapType.pres) &&
       (conEvnt->chanId.chanMapType.val == CMT_ATMVCC))
   {
     mfTwoByte = TRUE; 
   }

   ntc->toBeAlctdBChanlDir = IN_BCHAN_OUTGOING;
   inGetBChan(ntc, &conEvnt->chanId, &intId, &conEvnt->bearCap[0],
              &ntc->causeDgn);
   
   
   if (ntc->causeDgn.causeVal.pres)
   {
      if (conEvnt->bearCap[1].eh.pres)
      {
         inGetBChan(ntc, &conEvnt->chanId, &intId, &conEvnt->bearCap[1],
                    &ntc->causeDgn);
      }
      else
      {
         inGenRelInd(ntc, &ntc->causeDgn);
         RETVALUE(ROK);
      }

      if (ntc->causeDgn.causeVal.pres)
      {
         inGenRelInd(ntc, &ntc->causeDgn);
         RETVALUE(ROK);
      }
   }


   /* Bug 51677 - start. Move the check to after getting b-channel */
 
   /* if physical link is not in transfer state clear the call attempt */

   if ((pcb->cfg.intCfg == INTCFG_PTPT) &&
       (((dCb->suId == ntc->ctldPcb->suId) && (ntc->ctldPcb->state != CBST1XFER)) ||
        ((pcb->cfg.nfasInt) && 
         (ntc->ctldPcb->suId != pcb->cfg.sigInt) &&
         ((!pcb->cfg.buIntPr) || (ntc->ctldPcb->suId != pcb->cfg.bupInt)) &&
         (ntc->ctldPcb->state != CBST1XFER))))
   {
      INSETTOKHDR(ntc->causeDgn);
      INSETTOKVAL(ntc->causeDgn.causeVal, CCPROTERR);
      INUNSETTOK(ntc->causeDgn.recommend);

      inGenRelInd(ntc, &ntc->causeDgn);
      RETVALUE(ROK);
   }
   /* Bug 51677 - end */

   ntc->causeDgn.eh.pres = PRSNT_NODEF;

   if (ntc->cmpltAdr)     /* not overlap */
   {
      if ((intId.pres) && (!pcb->cfg.nfasInt))
         if (intId.val != ntc->ctldPcb->cfg.intId)
         {
            INSETTOKVAL(ntc->causeDgn.causeVal, CCINVINFOEL);
            inGenRelInd(ntc, &ntc->causeDgn);
            RETVALUE(ROK);
         }
   } 

   /*
    * Change state to Call Present and prepare to send the 
    * setup packet to the other end.
    * Switch specific activities need to be performed before that.
    * 
    */

   ntc->exclFlg = conEvnt->chanId.prefExc.val;
   ntc->state = ST_CALLPRES;
   inStartNtcTmr(TMR_T303, ntc, ntc->dCallCb);

   /* network Specific facilities */ 
   IN_SWATT4E_SETNETFAC;

   inUpdSts(&pcb->sts.setupTx, pcb);

   /*
    *  set protocol discriminator 
    *  The protocol discriminator may have to be reset for some 
    *  switch variants like Belcore TR303CSC/TR303TMC.
    */

   protDisc = PD_Q931;

#if (ISDN_BC303TMC | ISDN_BC303CSC)
   if ((pcb->cfg.swtch == SW_BC303CSC) ||
       (pcb->cfg.swtch == SW_BC303TMC))
   {
      protDisc = (U8) PD_BC303;
   }
#endif

   /* 
    * prepare Pdu header 
    */
   inInitPduHdr((U8) protDisc, ntc->origin, pcb->cfg.callRefLen, ntc->dCallRef,
                (U8) M_SETUP, &pduHdr);

   MFINITPDU(&ntc->dCallCb->mfMsgCtl, ret, (U8) PM_CONREQ, (U8) MI_SETUP, 
             (ElmtHdr *) ntc->sduSp, (ElmtHdr *) &allPdus, (U8) PRSNT_NODEF, 
             pcb->cfg.swtch, ntc->callType);

   if ((pcb->cfg.swtch == SW_NTDMS100B) || 
       (pcb->cfg.swtch == SW_NTDMS100P))
   {
      if (pcb->cfg.ringOff)
      {
         INSETTOKHDR(allPdus.m.setup.signal);
         INSETTOKVAL(allPdus.m.setup.signal.signal, SIG_ALERTOFF);
      }
   }

   if (ntc->callRedir)
   {
      switch (pcb->cfg.swtch)
      {
         case SW_ATT5EB:
            break;
         case SW_NTDMS100B:     /* switch - nt dms100 */
         case SW_NTDMS100P:     /* switch - nt dms100 */
         case SW_NTDMS250:      /* switch - nt dms250 */
         case SW_ATT4E:
         case SW_ATT5EP:
            MFINITELMT(&ntc->dCallCb->mfMsgCtl, ret, NULLP, 
                       (ElmtHdr *) &allPdus.m.setup.redirNmb, &meRedirNmb, 
                       (U8) PRSNT_DEF, pcb->cfg.swtch, ntc->callType);
            MFINITELMT(&ntc->dCallCb->mfMsgCtl, ret, NULLP, 
                       (ElmtHdr *) &allPdus.m.setup.cdPtyNmb, 
                       &meCdPtyNmb, (U8) PRSNT_DEF, pcb->cfg.swtch, ntc->callType);

            INSETTOKVAL(allPdus.m.setup.redirNmb.rsnRedirect,ntc->redirReason);
  
            if (conEvnt->trnstNtw.eh.pres)
            {
               INSETTOKVAL(allPdus.m.setup.redirNmb.nmbPlanId, 
                           conEvnt->trnstNtw.netIdPlan.val);
               INSETTOKVAL(allPdus.m.setup.redirNmb.typeNmb,
                           conEvnt->trnstNtw.typeNetId.val);
               INSETTOKVAL(allPdus.m.setup.redirNmb.screenInd,
                           pcb->cfg.redirAdr.screenInd.val);
               INSETTOKVAL(allPdus.m.setup.redirNmb.presInd,
                           pcb->cfg.redirAdr.presInd.val);
               INSETTOKARRAY(allPdus.m.setup.redirNmb.nmbDigits,
                             conEvnt->trnstNtw.netId.len,
                             conEvnt->trnstNtw.netId.val);
            }
            else
               if (ntc->tCallCb->sapCfg.keyPad)
               {
                  INSETTOKVAL(allPdus.m.setup.redirNmb.nmbPlanId, NP_UNK);
                  INSETTOKVAL(allPdus.m.setup.redirNmb.typeNmb, TON_UNK);
                  INSETTOKVAL(allPdus.m.setup.redirNmb.screenInd,
                              pcb->cfg.redirAdr.screenInd.val);
                  INSETTOKVAL(allPdus.m.setup.redirNmb.presInd,
                              pcb->cfg.redirAdr.presInd.val);
                  INSETTOKARRAY(allPdus.m.setup.redirNmb.nmbDigits,
                                conEvnt->keyFac.keyInfo.len,
                                conEvnt->keyFac.keyInfo.val);
               }
               else
                  if (ntc->tCallCb->sapCfg.useSubAdr)
                  {
                     INSETTOKVAL(allPdus.m.setup.redirNmb.nmbPlanId, NP_UNK);
                     INSETTOKVAL(allPdus.m.setup.redirNmb.typeNmb,
                                 conEvnt->cdPtySad.typeSad.val);
                     INSETTOKVAL(allPdus.m.setup.redirNmb.screenInd, 
                                 pcb->cfg.redirAdr.screenInd.val);
                     INSETTOKVAL(allPdus.m.setup.redirNmb.presInd, 
                                 pcb->cfg.redirAdr.presInd.val);
                     INSETTOKARRAY(allPdus.m.setup.redirNmb.nmbDigits, 
                                   conEvnt->cdPtySad.sadInfo.len,
                                   conEvnt->cdPtySad.sadInfo.val);
                  }
                  else           /* not a subaddress or keyPad */
                  {
                     INSETTOKVAL(allPdus.m.setup.redirNmb.nmbPlanId,
                                 conEvnt->cdPtyNmb.nmbPlanId.val);
                     INSETTOKVAL(allPdus.m.setup.redirNmb.typeNmb, 
                                 conEvnt->cdPtyNmb.typeNmb0.val);
                     INSETTOKVAL(allPdus.m.setup.redirNmb.screenInd, 
                                 pcb->cfg.redirAdr.screenInd.val);
                     INSETTOKVAL(allPdus.m.setup.redirNmb.presInd, 
                                 pcb->cfg.redirAdr.presInd.val);
                     INSETTOKARRAY(allPdus.m.setup.redirNmb.nmbDigits, 
                                   conEvnt->cdPtyNmb.nmbDigits.len,
                                   conEvnt->cdPtyNmb.nmbDigits.val);
                  }
            
            INSETTOKVAL(allPdus.m.setup.cdPtyNmb.nmbPlanId, ntc->redirNmbPlan);
            INSETTOKVAL(allPdus.m.setup.cdPtyNmb.typeNmb0, ntc->redirTypeNmb);
            INSETTOKARRAY(allPdus.m.setup.cdPtyNmb.nmbDigits,
                          ntc->dstAdr.nmbDigits.len,
                          ntc->dstAdr.nmbDigits.val);
            break;
         default:
            break;
      }
   }  /* ntc->calRedir */


   /* switch specific acitivities*/

   switch (pcb->cfg.swtch)
   {
      case SW_VN2:
      case SW_NTDMS100B:
      case SW_ATT5EB:
         INSETTOKVAL(allPdus.m.setup.chanId.intIdentPres, IIP_IMPLICIT);
         allPdus.m.setup.chanId.intIdent.pres = NOTPRSNT;
         break;         
      default:
         break;
   }

   /* PTMPT012 */
   
   /*
    * Build the message, copy it into a temp. buffer and transmit
    * it.  Segmentation of broadcast message is not being allowed 
    * here.
    */

   inBldMsg(ntc->dCallCb, &pduHdr, &allPdus, ntc->dCallCb->pst.region, 
            ntc->dCallCb->pst.pool, &mBuf, pcb->cfg.swtch, ntc->callType);
   ntc->setUp = mBuf;
   if ((ret = SCpyMsgMsg(mBuf, ntc->dCallCb->pst.region, 
                         ntc->dCallCb->pst.pool,
                         &tmpBuf)) != ROK)
   {

#if (ERRCLASS & ERRCLS_DEBUG) /* addition : in014.310 */
      INLOGERROR(ERRCLS_DEBUG, EIN410, (ErrVal)ret,
                 "inNetE27S00() failed, unable to copy message.");
#endif /* addition : in014.310 */

      INSETTOKVAL(ntc->causeDgn.causeVal, CCINVINFOEL);
      inGenRelInd(ntc, &ntc->causeDgn);
      RETVALUE(RFAILED);
   }

#ifdef ISDN_PTMPT
   if (((pcb->cfg.intCfg == INTCFG_MULTI)
            && (pcb->cfg.intType == NETWORK))
           && (ntc->setupBCast == TRUE))
   {
      inStartNtcTmr(TMR_T312, ntc, ntc->dCallCb);
      IN_SEND_BCASTMSG(ntc->dCallCb, tmpBuf);

   } /* ((pcb->cfg.swtch == SW_ETSI) */
   else
   {
     if (dCb->state != CBST1XFER)
     {
       INSETTOKVAL(ntc->causeDgn.causeVal, CCDESTOUTORD);
       inGenRelInd(ntc, &ntc->causeDgn);
       inDropData(tmpBuf);
       RETVALUE(RFAILED);
     }

#ifdef ISDN_SEGMENT
      inXmitSegMsg(dCb, tmpBuf);
#else  /* No Segmentation required. */
      inSndMsg(dCb, tmpBuf);
#endif

   }
#else  /* Not ISDN_PTMPT */

   if (dCb->state != CBST1XFER)
   {
      INSETTOKVAL(ntc->causeDgn.causeVal, CCDESTOUTORD);
      inGenRelInd(ntc, &ntc->causeDgn);
      inDropData(tmpBuf);
      RETVALUE(RFAILED);
   }

#ifdef ISDN_SEGMENT
   inXmitSegMsg(dCb, tmpBuf);
#else  /* No Segmentation required. */
   inSndMsg(dCb, tmpBuf);
#endif

#endif  /* ISDN_PTMPT */
   inGetAdr(ntc, conEvnt);

   RETVALUE(ROK);
} /* end of inNetE27S00 */





  
/*
 *
 *       Fun:   inNetE27S11
 *
 *       Desc:  Connect Request
 *              State Null
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
         File:  qn_bdy6.c
 *
*/

#ifdef ANSI
PUBLIC S16 inNetE27S11
(
InNtc *ntc,
InCb *dCb 
)
#else
PUBLIC S16 inNetE27S11(ntc, dCb)
InNtc *ntc;
InCb *dCb;
#endif
{



   TRC2(inNetE27S11);

      inNetEVTINV(ntc, dCb);
   RETVALUE(ROK);
} /* end of inNetE27S11 */

  
/*
 *
 *       Fun:   inNetE28S01
 *
 *       Desc:  Connect Response
 *              State Call Initiated
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
         File:  qn_bdy6.c
 *
*/

#ifdef ANSI
PUBLIC S16 inNetE28S01
(
InNtc *ntc,
InCb *dCb 
)
#else
PUBLIC S16 inNetE28S01(ntc, dCb)
InNtc *ntc;
InCb *dCb;
#endif
{
   InPCB *pcb;
   CnStEvnt *cnStEvnt;
   U8 tmrNum;
   PduHdr pduHdr;
   AllPdus allPdus;
   S16 ret;
   U8 protDisc;

   TRC2(inNetE28S01);

   mfTwoByte = FALSE;
  
   /* get physical link control block */
   if ((pcb = *(pcbLstPtr + dCb->suId)) == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      INLOGERROR(ERRCLS_DEBUG, EIN412, (ErrVal) dCb->rSuId,
                 "inNetE28S01() failed, unable to access DLSAP.");
#endif
      RETVALUE(RFAILED);
   }


   /* if physical link is restarting, ignore request,
    *  upper layer call will
    *  be cleared by restart procedures 
    */

   if (pcb->cfg.swtch == SW_NI2)
   {
#if (ERRCLASS & ERRCLS_INT_PAR) /* addition : in014.310 */
      INLOGERROR(ERRCLS_INT_PAR, EIN413, (ErrVal)ntc->evntType,
                 "InNetE28S01 failed, Incorrect Event Type issued. \n");
#endif /* addition : in014.310 */
      INSETTOKVAL(ntc->causeDgn.causeVal, CCPROTERR);
      inGenStaCfm(ntc, &ntc->causeDgn);
      RETVALUE(ROK);
   }

   if (ntc->ctldPcb->state == CBST2RST)
   {
     inGenAlarm(LCM_CATEGORY_INTERNAL, LCM_EVENT_INV_STATE, 
                LCM_CAUSE_INV_PCB_STATE, ntc->ctldPcb->suId);
     RETVALUE(ROK);
   }

   cnStEvnt = (CnStEvnt *) ntc->sduSp;
   /* Stop timers */

   STOP_NTC_TIMERS;

   ntc->tmrCnt = 0;
   if (cnStEvnt->chanId.eh.pres)
   {
      if ((cnStEvnt->chanId.chanMapType.pres) &&
          (cnStEvnt->chanId.chanMapType.val == CMT_ATMVCC))
      {
         mfTwoByte = TRUE; 
      }
    
      inNegChan(ntc, &cnStEvnt->chanId, &ntc->causeDgn);
      if (ntc->causeDgn.causeVal.pres)
      {
         inGenRelUpCLw(ntc, &ntc->causeDgn, dCb);
         RETVALUE(ROK);
      }
      ntc->causeDgn.eh.pres = PRSNT_NODEF;
      INSETTOKVAL(cnStEvnt->chanId.prefExc, PE_EXCLSVE);
   }
   else
   {
      switch (pcb->cfg.swtch)
      {
         default:
            inInsrtChnlId(ntc, &cnStEvnt->chanId, ntc->ctldPcb);
            break;
      }
   }

   ntc->state = ST_ACTVE;

   if (ntc->state == ST_ACTVE)
   {
      pcb->sts.cons++;
      pcb->sts.aveCalls++;

      if (ntc->updStatus == NTC_NOTCREATED)
      {
         ret = zqRunTimeUpd(ZQ_NTC_CB, CMPFTHA_CREATE_REQ, (PTR)ntc);
         if (ret == ROK)
            ntc->updStatus = NTC_CREATED;
      }
      else
         zqRunTimeUpd(ZQ_NTC_CB, CMPFTHA_UPD_REQ, (PTR)ntc);
   }

   /* Generate Connect Response message to lower layer */
   inUpdSts(&pcb->sts.connTx, pcb);

   protDisc = PD_Q931;

#if (ISDN_BC303TMC | ISDN_BC303CSC)
   if ((pcb->cfg.swtch == SW_BC303CSC) ||
       (pcb->cfg.swtch == SW_BC303TMC))
   {
      protDisc = (U8) PD_BC303;
   }
#endif

   /* FID 17524.0 */
   inIsFirstChnlId(ntc, &cnStEvnt->chanId, pcb->cfg.channelId); 

   /* prepare Pdu header */
   inInitPduHdr((U8) protDisc, ntc->origin, pcb->cfg.callRefLen, ntc->dCallRef,
                (U8) M_CONNECT, &pduHdr);

   MFINITPDU(&ntc->dCallCb->mfMsgCtl, ret, (U8) PM_CNSTREQ, (U8) MI_CONNECT, 
             (ElmtHdr *) ntc->sduSp, (ElmtHdr *) &allPdus, (U8) PRSNT_NODEF, 
             pcb->cfg.swtch, ntc->callType);

   IN_SWNT100B_TONES;

   inGenPdu(dCb, &pduHdr, &allPdus, pcb->cfg.swtch, ntc->callType);

   zqUpdPeer();

   RETVALUE(ROK);
} /* end of inNetE28S01 */

  
/*
 *
 *       Fun:   inNetE28S03
 *
 *       Desc:  Connect Response
 *              State Outgoing Call Proceeding
 *              State Call Delivered
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
         File:  qn_bdy6.c
 *
*/

#ifdef ANSI
PUBLIC S16 inNetE28S03
(
InNtc *ntc,
InCb *dCb 
)
#else
PUBLIC S16 inNetE28S03(ntc, dCb)
InNtc *ntc;
InCb *dCb;
#endif
{
   InPCB *pcb;
   CnStEvnt *cnStEvnt;
   U8 tmrNum;
   PduHdr pduHdr;
   AllPdus allPdus;
   S16 ret;

   TRC2(inNetE28S03);

   mfTwoByte = FALSE;

   /* get physical link control block */
   if ((pcb = *(pcbLstPtr + dCb->suId)) == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      INLOGERROR(ERRCLS_DEBUG, EIN414, (ErrVal) dCb->rSuId,
                 "inNetE28S03() failed, unable to access DLSAP.");
#endif
      RETVALUE(RFAILED);
   }

   /* if phisycal link is restarting, ignore request, upper layer call will
      be cleared by restart procedures */
   if (ntc->ctldPcb->state == CBST2RST)
   {
     inGenAlarm(LCM_CATEGORY_INTERNAL, LCM_EVENT_INV_STATE, 
                LCM_CAUSE_INV_PCB_STATE, ntc->ctldPcb->suId);
     RETVALUE(ROK);
   }

   cnStEvnt = (CnStEvnt *) ntc->sduSp;
   
   /* Stop timers */

   STOP_NTC_TIMERS;

   ntc->tmrCnt = 0;
   
   /* FID 17524.0 */
   inIsFirstChnlId(ntc, &cnStEvnt->chanId, pcb->cfg.channelId);

   mfTwoByte = FALSE;
   if ((cnStEvnt->chanId.eh.pres) &&
       ((cnStEvnt->chanId.chanMapType.pres) &&
       (cnStEvnt->chanId.chanMapType.val == CMT_ATMVCC)))
   {
     mfTwoByte = TRUE; 
   }

   if ((ret = inValChnlId(ntc, &cnStEvnt->chanId)) != ROK)
   {
      inGenRelUpLw(ntc, &ntc->causeDgn, dCb);
      RETVALUE(ROK);
   }

   pcb->sts.cons++;
   pcb->sts.aveCalls++;

   /* Change State to Active */
   ntc->state = ST_ACTVE;

   if (ntc->updStatus == NTC_NOTCREATED)
   {
      ret = zqRunTimeUpd(ZQ_NTC_CB, CMPFTHA_CREATE_REQ, (PTR)ntc);
      if (ret == ROK)
         ntc->updStatus = NTC_CREATED;
   }
   else
      zqRunTimeUpd(ZQ_NTC_CB, CMPFTHA_UPD_REQ, (PTR)ntc);

   /* Generate Connect Response message to lower layer */
   inUpdSts(&pcb->sts.connTx, pcb);

   /* prepare Pdu header */
   inInitPduHdr((U8) PD_Q931, ntc->origin, pcb->cfg.callRefLen, ntc->dCallRef,
                (U8) M_CONNECT, &pduHdr);

   MFINITPDU(&ntc->dCallCb->mfMsgCtl, ret, (U8) PM_CNSTREQ, (U8) MI_CONNECT, 
             (ElmtHdr *) ntc->sduSp, (ElmtHdr *) &allPdus, (U8) PRSNT_NODEF, 
             pcb->cfg.swtch, ntc->callType);

   IN_SWNT100B_TONES;
   inGenPdu(dCb, &pduHdr, &allPdus, pcb->cfg.swtch, ntc->callType);

   zqUpdPeer();

   RETVALUE(ROK);
} /* end of inNetE28S03 */





  
/*
 *
 *       Fun:   inNetE28S02
 *
 *       Desc:  Connect Response
 *              State: overlap sending
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
         File:  qn_bdy6.c
 *
*/

#ifdef ANSI
PUBLIC S16 inNetE28S02
(
InNtc *ntc,
InCb *dCb 
)
#else
PUBLIC S16 inNetE28S02(ntc, dCb)
InNtc *ntc;
InCb *dCb;
#endif
{
   InPCB *pcb = NULL;
   CnStEvnt *cnStEvnt = NULL;
   PduHdr pduHdr;
   AllPdus allPdus;
   U8 tmrNum = 0;
   S16 ret = 0;

   TRC2(inNetE28S02);
   UNUSED(ret);
   UNUSED(tmrNum);
   UNUSED(allPdus);
   UNUSED(pduHdr);
   UNUSED(cnStEvnt);
   UNUSED(pcb);
   UNUSED(ntc);
   UNUSED(dCb);

#ifdef ISDN_OVERLAP

    mfTwoByte = FALSE;

   /* get physical link control block */
   if ((pcb = *(pcbLstPtr + dCb->suId)) == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      INLOGERROR(ERRCLS_DEBUG, EIN415, (ErrVal) dCb->rSuId,
                 "inNetE28S02() failed, unable to access DLSAP.");
#endif
      RETVALUE(RFAILED);
   }

   /* if phisycal link is restarting, ignore request, upper layer call will
      be cleared by restart procedures */

   if (ntc->ctldPcb->state == CBST2RST)
   {
     inGenAlarm(LCM_CATEGORY_INTERNAL, LCM_EVENT_INV_STATE, 
                LCM_CAUSE_INV_PCB_STATE, ntc->ctldPcb->suId);
     RETVALUE(ROK);
   }

   switch (pcb->cfg.swtch)
   {
      case SW_NTDMS100B:     /* switch - nt dms100 */
      case SW_ATT5EB:
      case SW_CCITT:
#ifdef ETSI                 /* TELICA-dsolanki-INTMGCDEV */
      case SW_ETSI:         /* switch - etsi */
#endif                      /* TELICA-dsolanki-INTMGCDEV */
         cnStEvnt = (CnStEvnt *) ntc->sduSp;
         /* Stop T302 */
         STOP_NTC_TIMER(TMR_T302);

         mfTwoByte = FALSE;
         /* FID 17524.0 */
         inIsFirstChnlId(ntc, &cnStEvnt->chanId, pcb->cfg.channelId);

         if (cnStEvnt->chanId.eh.pres)
         {
           if ((cnStEvnt->chanId.chanMapType.pres) &&
               (cnStEvnt->chanId.chanMapType.val == CMT_ATMVCC))
           {
             mfTwoByte = TRUE; 
           }
         
           if ((ret = inValChnlId(ntc, &cnStEvnt->chanId)) != ROK)
           {
             INSETTOKVAL(ntc->causeDgn.causeVal, CCCHANUNACPT);
             inGenRelUpLw(ntc, &ntc->causeDgn, dCb);
             RETVALUE(ROK);
           }
           INSETTOKVAL(cnStEvnt->chanId.prefExc, PE_EXCLSVE);
         }
         
         /* Change State to Active */
         ntc->state = ST_ACTVE;

         if (ntc->updStatus == NTC_NOTCREATED)
         {
            ret = zqRunTimeUpd(ZQ_NTC_CB, CMPFTHA_CREATE_REQ, (PTR)ntc);
            if (ret == ROK)
               ntc->updStatus = NTC_CREATED;
         }
         else
            zqRunTimeUpd(ZQ_NTC_CB, CMPFTHA_UPD_REQ, (PTR)ntc);

         inUpdSts(&pcb->sts.connTx, pcb);
         pcb->sts.cons++;
         pcb->sts.aveCalls++;
         
         /* prepare Pdu header */
         inInitPduHdr((U8) PD_Q931, ntc->origin, pcb->cfg.callRefLen, 
                      ntc->dCallRef, (U8) M_CONNECT, &pduHdr);
         
         MFINITPDU(&ntc->dCallCb->mfMsgCtl,ret, (U8)PM_CNSTREQ,(U8) MI_CONNECT,
                   (ElmtHdr *) ntc->sduSp, (ElmtHdr *) &allPdus, (U8) PRSNT_NODEF, 
                   pcb->cfg.swtch, ntc->callType);

         IN_SWNT100B_TONES;
         inGenPdu(dCb, &pduHdr, &allPdus, pcb->cfg.swtch, 
                  ntc->callType);
         break;

      default:
#if (ERRCLASS & ERRCLS_INT_PAR) /* addition : in014.310 */
         INLOGERROR(ERRCLS_INT_PAR, EIN416, (ErrVal) pcb->cfg.swtch,
                    "inNetE28S02() failed, invalid event.");
#endif
         inNetEVTINV(ntc, dCb);
         break;
   }
#endif
   RETVALUE(ROK);
} /* end of inNetE28S02 */



/*
*       Fun:   inNetE29S01
*
*       Desc:  Connection Status Request First Reply To Setup Message
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  qn_bdy6.c
*
*/

#ifdef ANSI
PUBLIC S16 inNetE29S01
(
InNtc *ntc,
InCb *dCb
)
#else
PUBLIC S16 inNetE29S01(ntc, dCb)
InNtc *ntc;
InCb *dCb;
#endif
{
   InPCB *pcb;
   CnStEvnt *cnStEvnt;
   PduHdr pduHdr;
   AllPdus allPdus;
   U8 tmrNum;
   S16 ret;
   U8 protDisc;

   TRC2(inNetE29S01);

   mfTwoByte = FALSE;

   /* get physical link control block */

   if ((pcb = *(pcbLstPtr + dCb->suId)) == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      INLOGERROR(ERRCLS_DEBUG, EIN417, (ErrVal) dCb->rSuId,
                 "inNetE29S01() failed, unable to access DLSAP.");
#endif
      RETVALUE(RFAILED);
   }

   if ((ntc->evntType != MI_CALLPROC)&&(ntc->evntType != MI_STAT)
        &&(pcb->cfg.swtch == SW_NI2))
   {
#if (ERRCLASS & ERRCLS_INT_PAR) /* addition : in009.310 */
      INLOGERROR(ERRCLS_INT_PAR, EIN419, (ErrVal)ntc->evntType,
                 "InNetE29S01 failed, Incorrect Event Type issued. \n");
#endif /* (ERRCLASS & ERRCLS_INT_PAR) */ /* addition : in009.310 */
      INSETTOKVAL(ntc->causeDgn.causeVal, CCPROTERR);
      inGenStaCfm(ntc, &ntc->causeDgn);
      RETVALUE(ROK);
   }


   /* if phisycal link is restarting, ignore request, 
    * upper layer call will
    * be cleared by restart procedures 
    */

   if (ntc->ctldPcb->state == CBST2RST)
   {
     inGenAlarm(LCM_CATEGORY_INTERNAL, LCM_EVENT_INV_STATE, 
                LCM_CAUSE_INV_PCB_STATE, ntc->ctldPcb->suId);
      RETVALUE(ROK);
   }

   /* TELICA-BUG:15265-mqin */
   if (ntc->evntType == MI_STAT)
   {
      if (pcb->cfg.swtch == SW_NI2)
      {
         inGenStatusReq(ntc, dCb);
      }
      RETVALUE(ROK);
   }

   cnStEvnt = (CnStEvnt *) ntc->sduSp;

   mfTwoByte = FALSE;
   /* FID 17524.0 */
   if ((inIsFirstChnlId(ntc, &cnStEvnt->chanId, pcb->cfg.channelId)) == TRUE)
   {
       if (cnStEvnt->chanId.eh.pres)
       {
           if ((cnStEvnt->chanId.chanMapType.pres) &&
               (cnStEvnt->chanId.chanMapType.val == CMT_ATMVCC))
           {
               mfTwoByte = TRUE; 
           }
     
           switch (pcb->cfg.swtch)
           {
               case SW_NTDMS100B:     /* switch - nt dms100 */
               case SW_NTDMS100P:     /* switch - nt dms100 */
                   /* validate Channel */
                   if ((ret = inValChnlId(ntc, &cnStEvnt->chanId)) != ROK)
                   {
                      inGenStaCfm(ntc, &ntc->causeDgn);
                      RETVALUE(ROK);
                   }
                   break;
               default:
                   inNegChan(ntc, &cnStEvnt->chanId, &ntc->causeDgn);
                   if (ntc->causeDgn.causeVal.pres)
                   {
                      inGenRelUpCLw(ntc, &ntc->causeDgn, dCb);
                      RETVALUE(ROK);
                   }
                   ntc->causeDgn.eh.pres = PRSNT_NODEF;
                   cnStEvnt->chanId.prefExc.val = PE_EXCLSVE;
                   break;
           }
       }  /* if (cnStEvnt->chanId.eh.pres) */
       else
           inInsrtChnlId(ntc, &cnStEvnt->chanId, ntc->ctldPcb);
   }


   if (((pcb->cfg.swtch == SW_NTNI) || 
        (pcb->cfg.swtch == SW_NTDMS250)) &&
       (ntc->evntType != MI_CALLPROC))
   {
      INSETTOKVAL(ntc->causeDgn.causeVal, CCPROTERR);
      inGenStaCfm(ntc, &ntc->causeDgn);
      RETVALUE(ROK);
   }

   switch (ntc->evntType)
   {
      case MI_ALERTING:           /* Alerting, c, p, u */

         STOP_NTC_TIMER(TMR_T302);
         ntc->state = ST_CALLDEL;

         if (ntc->updStatus == NTC_NOTCREATED)
         {
            ret = zqRunTimeUpd(ZQ_NTC_CB, CMPFTHA_CREATE_REQ, (PTR)ntc);
            if (ret == ROK)
               ntc->updStatus = NTC_CREATED;
         }
         else
            zqRunTimeUpd(ZQ_NTC_CB, CMPFTHA_UPD_REQ, (PTR)ntc);

         inUpdSts(&pcb->sts.alertingTx, pcb);
         protDisc = (U8) PD_Q931;
         inInitPduHdr(protDisc, ntc->origin, pcb->cfg.callRefLen,ntc->dCallRef,
                      (U8) M_ALERTING, &pduHdr);
      
         MFINITPDU(&ntc->dCallCb->mfMsgCtl, ret, (U8) PM_CNSTREQ, 
                   (U8) MI_ALERTING, (ElmtHdr *) cnStEvnt, (ElmtHdr *)&allPdus, 
                   (U8) PRSNT_NODEF, pcb->cfg.swtch, ntc->callType);

         if (pcb->cfg.swtch == SW_NTDMS100B)     /* switch - nt dms100 */
         {
            if ((ntc->bearCap == ITC_SPEECH) || /* Speech */
                (ntc->bearCap == ITC_A31KHZ) || /* 3.1 Audio */
                (ntc->bearCap == ITC_A7KHZ))    /* 7 Audio */
            { 
               /* set progress indicator to 8 */
               INSETTOKHDR(allPdus.m.alerting.progInd);
               INSETTOKVAL(allPdus.m.alerting.progInd.location,LOC_PUBNETLU);
               INSETTOKVAL(allPdus.m.alerting.progInd.codeStand0,CSTD_CCITT);
               INSETTOKVAL(allPdus.m.alerting.progInd.progDesc, PD_IBAVAIL);
            }
            else
            {
               INSETTOKHDR(allPdus.m.alerting.signal);
               INSETTOKVAL(allPdus.m.alerting.signal.signal, SIG_RNGBCK);
            }
         }
         inGenPdu(dCb, &pduHdr, &allPdus, pcb->cfg.swtch, 
                  ntc->callType);
         break;

      case MI_CALLPROC:           /* Call Proceeding, c, p, u */
         /* Stop T302 */
      
         STOP_NTC_TIMER(TMR_T302);

         ntc->state = ST_OUTPROC;

         inUpdSts(&pcb->sts.callProcTx, pcb);
      
         /* prepare Pdu header */
         protDisc = (U8) PD_Q931;
         inInitPduHdr(protDisc, ntc->origin, pcb->cfg.callRefLen,ntc->dCallRef,
                      (U8) M_CALLPROC, &pduHdr);
      
         MFINITPDU(&ntc->dCallCb->mfMsgCtl, ret, (U8) PM_CNSTREQ, 
                   (U8) MI_CALLPROC, (ElmtHdr *) cnStEvnt, 
                   (ElmtHdr *) &allPdus, 
                   (U8) PRSNT_NODEF, pcb->cfg.swtch, ntc->callType);
         inGenPdu(dCb, &pduHdr, &allPdus, pcb->cfg.swtch, 
                  ntc->callType);
         break;

      case MI_PROGRESS:           /* Progress, c, p, u */
         STOP_NTC_TIMER(TMR_T302);
      
         switch (pcb->cfg.swtch)
         {
            case SW_ATT4E:
            case SW_ATT5EP:
            case SW_ATT5EB:
              ntc->state = ST_CALLDEL;
              if (ntc->updStatus == NTC_NOTCREATED)
              {
                 ret = zqRunTimeUpd(ZQ_NTC_CB, CMPFTHA_CREATE_REQ, (PTR)ntc);
                 if (ret == ROK)
                    ntc->updStatus = NTC_CREATED;
              }
              else
                 zqRunTimeUpd(ZQ_NTC_CB, CMPFTHA_UPD_REQ, (PTR)ntc);
              zqUpdPeer();

               break;
            case SW_CCITT:
#ifdef ETSI                       /* TELICA-dsolanki-INTMGCDEV */
            case SW_ETSI:         /* switch - etsi */
#endif                             /* TELICA-dsolanki-INTMGCDEV */
            case SW_NTDMS100B:     /* switch - nt dms100 */
            case SW_NTDMS100P:     /* switch - nt dms100 */
               break;
            default:
#if (ERRCLASS & ERRCLS_INT_PAR) /* addition : in014.310 */
               INLOGERROR(ERRCLS_INT_PAR, EIN420, (ErrVal)pcb->cfg.swtch,
                          "inNetE29S01() failed, invalid event.");
#endif
               inNetEVTINV(ntc, dCb);
         }
  
         inUpdSts(&pcb->sts.progressTx, pcb);
 
         /* prepare Pdu header */
         inInitPduHdr((U8) PD_Q931, ntc->origin, pcb->cfg.callRefLen, 
                      ntc->dCallRef, (U8) M_PROGRESS, &pduHdr);
      
         MFINITPDU(&ntc->dCallCb->mfMsgCtl, ret, (U8) PM_CNSTREQ, 
                   (U8) MI_PROGRESS, (ElmtHdr *) cnStEvnt, (ElmtHdr *) &allPdus, 
                   (U8) PRSNT_NODEF, pcb->cfg.swtch, ntc->callType);
         inGenPdu(dCb, &pduHdr, &allPdus, pcb->cfg.swtch, 
                  ntc->callType);
         break;
      

      case MI_SETUPACK:           /* Setup Acknowledge, c, p, u */
         if (pcb->cfg.swtch == SW_NTDMS100P)
         {
#if (ERRCLASS & ERRCLS_INT_PAR)
            INLOGERROR(ERRCLS_INT_PAR, EIN421, (ErrVal) pcb->cfg.swtch,
                       "inNetE29ES01() failed, invalid message.");
#endif
            inNetEVTINV(ntc, dCb);
         }

         if ((pcb->cfg.swtch == SW_ATT4E) || (pcb->cfg.swtch == SW_ATT5EP))
         {
#if (ERRCLASS & ERRCLS_INT_PAR)
            INLOGERROR(ERRCLS_INT_PAR, EIN422, (ErrVal) pcb->cfg.swtch,
                       "inNetE29ES01() failed, invalid message.");
#endif
            inNetEVTINV(ntc, dCb);
         }
      
         STOP_NTC_TIMER(TMR_T302);
      
         ntc->state = ST_OVLAPSND;
      
         /* start T302, where applicable */
         switch (pcb->cfg.swtch)
         {
            /* 2111 overlap */
            case SW_ETSI:
               if((pcb->cfg.overlap) &&
                  (!IN_GOT_MINDIGITS(ntc->nmbCdPty, pcb->cfg.minDigits)))
                  inStartNtcTmr(TMR_TMIN, ntc, ntc->dCallCb);
               else
                  inStartNtcTmr(TMR_T302, ntc, ntc->dCallCb);
               break;
 
            default:
               inStartNtcTmr(TMR_T302, ntc, ntc->dCallCb);
               break;
         }
      
         inUpdSts(&pcb->sts.setupAckTx, pcb);
         /* Generate Setup Ack message to Lower layer */
      
         /* prepare Pdu header */
         switch (pcb->cfg.swtch)
         {
            default:
               protDisc = (U8) PD_Q931;
               break;
         }

         inInitPduHdr(protDisc, ntc->origin, pcb->cfg.callRefLen,ntc->dCallRef,
                      (U8) M_SETUPACK, &pduHdr);
      
         MFINITPDU(&ntc->dCallCb->mfMsgCtl, ret, (U8) PM_CNSTREQ, 
                   (U8) MI_SETUPACK, (ElmtHdr *) cnStEvnt, (ElmtHdr *) &allPdus,
                   (U8) PRSNT_NODEF, pcb->cfg.swtch, ntc->callType);

         if (pcb->cfg.swtch == SW_NTDMS100B)
         {
            /* set signal to tones on */
            INSETTOKHDR(allPdus.m.setupAck.signal);
            INSETTOKVAL(allPdus.m.setupAck.signal.signal, SIG_DIAL);
         }

         /* FID17555.0 Kavon */
         if (ntc->bearCap == ITC_A31KHZ &&
             IN_CK_PLAYDIALTONE(ntc->waitForMoreInfo))
         { 
            INSETTOKHDR(allPdus.m.setupAck.progInd);
            INSETTOKVAL(allPdus.m.setupAck.progInd.location,LOC_PUBNETLU);
            INSETTOKVAL(allPdus.m.setupAck.progInd.codeStand0,CSTD_CCITT);
            INSETTOKVAL(allPdus.m.setupAck.progInd.progDesc, PD_IBAVAIL);
         }
      
         inGenPdu(dCb, &pduHdr, &allPdus, pcb->cfg.swtch, 
                  ntc->callType);
         break;
      
      default:
         inNetEVTINV(ntc, dCb);
         break;
   }

   zqUpdPeer();

   RETVALUE(ROK);
} /* end of inNetE29ES01 */






/*
 *
 *       Fun:   inNetE29S04
 *
 *       Desc:  Input: Connect Status Event 
 *              State: Call Delivered
 *                     
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
         File:  qn_bdy6.c
 *
*/

#ifdef ANSI
PUBLIC S16 inNetE29S04
(
InNtc *ntc,
InCb *dCb 
)
#else
PUBLIC S16 inNetE29S04(ntc, dCb)
InNtc *ntc;
InCb *dCb;
#endif
{
   InPCB *pcb;
   PduHdr pduHdr;
   AllPdus allPdus;
   S16 ret;
#ifdef NT
   U8 tmrNum;
#endif /* NT */


   TRC2(inNetE29S04);

   mfTwoByte = FALSE;

   /* get physical link control block */
   if ((pcb = *(pcbLstPtr + dCb->suId)) == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      INLOGERROR(ERRCLS_DEBUG, EIN423, (ErrVal) dCb->rSuId,
                 "inNetE29S04() failed, unable to access DLSAP.");
#endif
      RETVALUE(RFAILED);
   }

   /* if phisycal link is restarting, ignore request, upper layer call will
      be cleared by restart procedures */
   if (ntc->ctldPcb->state == CBST2RST)
   {
     inGenAlarm(LCM_CATEGORY_INTERNAL, LCM_EVENT_INV_STATE, 
                LCM_CAUSE_INV_PCB_STATE, ntc->ctldPcb->suId);
     RETVALUE(ROK);
   }

   /* FID 17524.0 */
   inIsFirstChnlId(ntc, &ntc->sduSp->m.cnStEvnt.chanId, pcb->cfg.channelId);

   if ((ntc->sduSp->m.cnStEvnt.chanId.eh.pres) &&
       ((ntc->sduSp->m.cnStEvnt.chanId.chanMapType.pres) &&
       (ntc->sduSp->m.cnStEvnt.chanId.chanMapType.val == CMT_ATMVCC)))
   {
     mfTwoByte = TRUE; 
   }

   /* TELICA-BUG:15265-mqin */
   if (ntc->evntType == MI_STAT)
   {
      if (pcb->cfg.swtch == SW_NI2)
      {
         inGenStatusReq(ntc, dCb);
      }
      RETVALUE(ROK);
   }

   /* Init Cause Element */
   ntc->causeDgn.causeVal.pres = PRSNT_NODEF;
   switch (pcb->cfg.swtch)
   {
      case SW_ATT4E:
      case SW_ATT5EP:
      case SW_ATT5EB:
         /* MCAO: RIO bug 9485 */
         switch (ntc->evntType)
         {
            case MI_PROGRESS:           /* Progress, c, p, u */
               STOP_NTC_TIMER(TMR_T302);
           
               inUpdSts(&pcb->sts.progressTx, pcb);
           
               /* prepare Pdu header */
               inInitPduHdr((U8) PD_Q931, ntc->origin, pcb->cfg.callRefLen, 
                            ntc->dCallRef, (U8) M_PROGRESS, &pduHdr);
           
               MFINITPDU(&ntc->dCallCb->mfMsgCtl, ret, (U8) PM_CNSTREQ, 
                         (U8) MI_PROGRESS, (ElmtHdr *) ntc->sduSp, 
                         (ElmtHdr *) &allPdus, (U8) PRSNT_NODEF, pcb->cfg.swtch, 
                         ntc->callType);

               inGenPdu(dCb, &pduHdr, &allPdus, pcb->cfg.swtch, 
                        ntc->callType);
               RETVALUE(ROK);
           
            case MI_ALERTING:           /* Alerting */
               inUpdSts(&pcb->sts.alertingTx, pcb);
           
               /* prepare Pdu header */
               inInitPduHdr((U8) PD_Q931, ntc->origin, pcb->cfg.callRefLen, 
                            ntc->dCallRef, (U8) M_ALERTING, &pduHdr);
           
               MFINITPDU(&ntc->dCallCb->mfMsgCtl, ret, (U8) PM_CNSTREQ, 
                         (U8) MI_ALERTING, (ElmtHdr *) ntc->sduSp, 
                         (ElmtHdr *) &allPdus, (U8) PRSNT_NODEF, pcb->cfg.swtch,
                         ntc->callType);
               inGenPdu(dCb, &pduHdr, &allPdus, pcb->cfg.swtch, 
                        ntc->callType);
               RETVALUE(ROK);
           
            default:
               inNetEVTINV(ntc, dCb);
               RETVALUE(ROK);
         }
         break;

      case SW_NI2:
      case SW_NTDMS250:      /* switch - nt dms250 */
      case SW_NTDMS100P:     /* switch - nt dms100 */
      case SW_CCITT:
#ifdef ETSI                  /* TELICA-dsolanki-INTMGCDEV */
      case SW_ETSI:         /* switch - etsi */
#endif                      /* TELICA-dsolanki-INTMGCDEV */
         switch (ntc->evntType)
         {
            case MI_PROGRESS:           /* Progress, c, p, u */
               inUpdSts(&pcb->sts.progressTx, pcb);
           
               /* prepare Pdu header */
               inInitPduHdr((U8) PD_Q931, ntc->origin, pcb->cfg.callRefLen, 
                            ntc->dCallRef, (U8) M_PROGRESS, &pduHdr);
           
               MFINITPDU(&ntc->dCallCb->mfMsgCtl, ret, (U8) PM_CNSTREQ, 
                         (U8) MI_PROGRESS, (ElmtHdr *) ntc->sduSp, 
                         (ElmtHdr *) &allPdus, (U8) PRSNT_NODEF,
                         pcb->cfg.swtch, ntc->callType);
               inGenPdu(dCb, &pduHdr, &allPdus, pcb->cfg.swtch, 
                        ntc->callType);
               break;

           case MI_INFO:               /* Information, c, p, u */
               if ((pcb->cfg.swtch == SW_NTMCI) ||
                   (pcb->cfg.swtch == SW_NTNI) ||
                   (pcb->cfg.swtch == SW_NI2))
               {
#if (ERRCLASS & ERRCLS_INT_PAR) /* addition : in014.310 */
                  INLOGERROR(ERRCLS_INT_PAR, EIN424, (ErrVal)ntc->evntType,
                             "InNetE29S04 failed, Incorrect Event Type issued. \n");
#endif /* addition : in014.310 */
                  INSETTOKVAL(ntc->causeDgn.causeVal, CCPROTERR);
                  inGenStaCfm(ntc, &ntc->causeDgn);
                  RETVALUE(ROK);
               }

               inUpdSts(&pcb->sts.infoTx, pcb);

               if (pcb->cfg.swtch == SW_NTDMS250)
#if (ERRCLASS & ERRCLS_DEBUG)
               {
                 INLOGERROR(ERRCLS_INT_PAR, EIN425, (ErrVal)ntc->evntType,
                            "InNetE29S04 failed, Incorrect Event Type issued. \n");
                 INSETTOKVAL(ntc->causeDgn.causeVal, CCPROTERR);
                 inGenStaCfm(ntc, &ntc->causeDgn);
                 RETVALUE(ROK);
               }
#else
               {
                 inGenAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT, 
                            LCM_CAUSE_INV_EVENT, ntc->tCallCb->spId);
                 RETVALUE(ROK);
               }
#endif /* (ERRCLASS & ERRCLS_DEBUG) */
               
               /* prepare Pdu header */
               inInitPduHdr((U8) PD_Q931, ntc->origin, pcb->cfg.callRefLen, 
                            ntc->dCallRef, (U8) M_INFO, &pduHdr);
               
               MFINITPDU(&ntc->dCallCb->mfMsgCtl, ret, (U8) PM_CNSTREQ, 
                         (U8) MI_INFO, (ElmtHdr *) ntc->sduSp, 
                         (ElmtHdr *) &allPdus, (U8) PRSNT_NODEF, 
                         pcb->cfg.swtch, ntc->callType);
               inGenPdu(dCb, &pduHdr, &allPdus, pcb->cfg.swtch, 
                        ntc->callType);
               break;
           case MI_NOTIFY:           /* Progress, c, p, u */
             /* prepare Pdu header */
             inInitPduHdr((U8) PD_Q931, ntc->origin, pcb->cfg.callRefLen, 
                          ntc->dCallRef, (U8) M_NOTIFY, &pduHdr);
             
             /* initialize Notify Message */
             MFINITPDU(&ntc->dCallCb->mfMsgCtl, ret, (U8) PM_CNSTREQ, 
                       (U8) MI_NOTIFY, (ElmtHdr *) ntc->sduSp, 
                       (ElmtHdr *) &allPdus, (U8) PRSNT_NODEF, pcb->cfg.swtch,
                       ntc->callType);
             
             inUpdSts(&pcb->sts.notifyTx, pcb);
             
             inGenPdu(dCb, &pduHdr, &allPdus, pcb->cfg.swtch, 
                      ntc->callType);                          
            break;   
       
           default:
               inNetEVTINV(ntc, dCb);
               break;
         }
         RETVALUE(ROK);

      case SW_NTDMS100B:     /* switch - nt dms100 */  
         if (pcb->cfg.swtch == SW_NTDMS100B)
         { 
            if ((ret = inValChnlId(ntc, &ntc->sduSp->m.cnStEvnt.chanId)) 
                != ROK)
            {
               INSETTOKVAL(ntc->causeDgn.causeVal, CCCHANUNACPT);
               break;
            }
         }

         switch (ntc->evntType)
         {
            case MI_PROGRESS:           /* Progress, c, p, u */
               STOP_NTC_TIMER(TMR_T302);
           
               inUpdSts(&pcb->sts.progressTx, pcb);
           
               /* prepare Pdu header */
               inInitPduHdr((U8) PD_Q931, ntc->origin, pcb->cfg.callRefLen, 
                            ntc->dCallRef, (U8) M_PROGRESS, &pduHdr);
           
               MFINITPDU(&ntc->dCallCb->mfMsgCtl, ret, (U8) PM_CNSTREQ, 
                         (U8) MI_PROGRESS, (ElmtHdr *) ntc->sduSp, 
                         (ElmtHdr *) &allPdus, (U8) PRSNT_NODEF, pcb->cfg.swtch, 
                         ntc->callType);

               if (pcb->cfg.swtch == SW_NTDMS100B)
               {
                  INSETTOKHDR(allPdus.m.progress.progInd);
                  INSETTOKVAL(allPdus.m.progress.progInd.location,
                              LOC_PUBNETLU);
                  INSETTOKVAL(allPdus.m.progress.progInd.codeStand0, 
                              CSTD_CCITT);
                  INSETTOKVAL(allPdus.m.progress.progInd.progDesc, 
                              PD_NOTETEISDN);
               }

               inGenPdu(dCb, &pduHdr, &allPdus, pcb->cfg.swtch, 
                        ntc->callType);
               RETVALUE(ROK);
           
            case MI_ALERTING:           /* Alerting */
               inUpdSts(&pcb->sts.alertingTx, pcb);
           
               /* prepare Pdu header */
               inInitPduHdr((U8) PD_Q931, ntc->origin, pcb->cfg.callRefLen, 
                            ntc->dCallRef, (U8) M_ALERTING, &pduHdr);
           
               MFINITPDU(&ntc->dCallCb->mfMsgCtl, ret, (U8) PM_CNSTREQ, 
                         (U8) MI_ALERTING, (ElmtHdr *) ntc->sduSp, 
                         (ElmtHdr *) &allPdus, (U8) PRSNT_NODEF, pcb->cfg.swtch,
                         ntc->callType);
               inGenPdu(dCb, &pduHdr, &allPdus, pcb->cfg.swtch, 
                        ntc->callType);
               RETVALUE(ROK);
           
            default:
               inNetEVTINV(ntc, dCb);
               RETVALUE(ROK);
         }

      default:
         /*
          * We depend on tht Status Confirm to do the trick here.
          */
#if (ERRCLASS & ERRCLS_INT_PAR)
         INLOGERROR(ERRCLS_INT_PAR, EIN426, (ErrVal) pcb->cfg.swtch,
                    "inNetE29S04() failed, invalid event.");
#endif
         break;
   }
   inGenStaCfm(ntc, &ntc->causeDgn);
   RETVALUE(ROK);
} 

/* end of inNetE29S04 */





/*
*
*       Fun:   inNetE29S07
*
*       Desc:  Input: Connect Status Event 
*              State: Call Received
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  qn_bdy6.c
*
*/

#ifdef ANSI
PUBLIC S16 inNetE29S07
(
InNtc *ntc,
InCb *dCb 
)
#else
PUBLIC S16 inNetE29S07(ntc, dCb)
InNtc *ntc;
InCb *dCb;
#endif
{
   InPCB *pcb;
   PduHdr pduHdr;
   AllPdus allPdus;
   S16 ret;
   Buffer *mBuf;

   TRC2(inNetE29S07);

   mfTwoByte = FALSE;

   /* get physical link control block */
   if ((pcb = *(pcbLstPtr + dCb->suId)) == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      INLOGERROR(ERRCLS_DEBUG, EIN427, (ErrVal) dCb->rSuId,
                 "inNetE27S00() failed, unable to access DLSAP.");
#endif
      RETVALUE(RFAILED);
   }

   /*
    * if phisycal link is restarting, 
    * ignore request, upper layer call will
    * be cleared by restart procedures 
    */
   if (ntc->ctldPcb->state == CBST2RST)
   {
      inGenAlarm(LCM_CATEGORY_INTERNAL, LCM_EVENT_INV_STATE, 
                 LCM_CAUSE_INV_PCB_STATE, ntc->ctldPcb->suId);
      RETVALUE(ROK);
   }

   /* FID 17524.0 */
   inIsFirstChnlId(ntc, &ntc->sduSp->m.cnStEvnt.chanId, pcb->cfg.channelId);

   if ((ntc->sduSp->m.cnStEvnt.chanId.eh.pres) &&
       ((ntc->sduSp->m.cnStEvnt.chanId.chanMapType.pres) &&
       (ntc->sduSp->m.cnStEvnt.chanId.chanMapType.val == CMT_ATMVCC)))
   {
      mfTwoByte = TRUE; 
   }

   /* TELICA-BUG:15265-mqin */
   if (ntc->evntType == MI_STAT)
   {
      if (pcb->cfg.swtch == SW_NI2)
      {
         inGenStatusReq(ntc, dCb);
      }
      RETVALUE(ROK);
   }

   switch (pcb->cfg.swtch)
   {
      case SW_NTDMS250:
      case SW_NTDMS100P:
      case SW_CCITT:
#ifdef ETSI                  /* TELICA-dsolanki-INTMGCDEV */
      case SW_ETSI:         /* switch - etsi */
#endif                      /* TELICA-dsolanki-INTMGCDEV */
         switch (ntc->evntType)
         {
            case MI_INFO:           /* info, c, p, u */
               inUpdSts(&pcb->sts.infoTx, pcb);

               /* prepare Pdu header */
               inInitPduHdr((U8) PD_Q931, ntc->origin, pcb->cfg.callRefLen, 
                            ntc->dCallRef, (U8) M_INFO, &pduHdr);
         
               /* initialize Info Message */
               MFINITPDU(&ntc->dCallCb->mfMsgCtl, ret, (U8) PM_CNSTREQ, 
                         (U8) MI_INFO, (ElmtHdr *) ntc->sduSp, 
                         (ElmtHdr *) &allPdus, (U8) PRSNT_NODEF, pcb->cfg.swtch,
                         ntc->callType);
               inBldMsg(dCb, &pduHdr, &allPdus, ntc->dCallCb->pst.region, 
                        ntc->dCallCb->pst.pool, &mBuf, pcb->cfg.swtch, ntc->callType);
               /* PTMPT028 */
#ifdef ISDN_PTMPT
               if ((pcb->cfg.intType == NETWORK)
                   && ((pcb->cfg.intCfg == INTCFG_MULTI)
                       && (ntc->setupBCast == TRUE)))
               {
                  S16 i;
                  for (i=0; (U8)i< (U8)(ntc->nmbValidCes); i++) 
                  {
                     /*
                      * The function will copy the message before transmitting.
                      */
                     inIntPrimInfoReq(ntc, dCb, mBuf);
                  }
                  inDropData(mBuf);
                  RETVALUE(ROK);
               }
#endif /* ISDN_PTMPT */

#ifdef ISDN_SEGMENT
               inXmitSegMsg(dCb, mBuf);
#else  /* No Segmentation. */
               inSndMsg(dCb, mBuf);
#endif 
               break;
           case MI_NOTIFY:           /* Progress, c, p, u */
             /* prepare Pdu header */
             inInitPduHdr((U8) PD_Q931, ntc->origin, pcb->cfg.callRefLen, 
                          ntc->dCallRef, (U8) M_NOTIFY, &pduHdr);
             
             /* initialize Notify Message */
             MFINITPDU(&ntc->dCallCb->mfMsgCtl, ret, (U8) PM_CNSTREQ, 
                       (U8) MI_NOTIFY, (ElmtHdr *) ntc->sduSp, 
                       (ElmtHdr *) &allPdus, (U8) PRSNT_NODEF, pcb->cfg.swtch,
                       ntc->callType);
             
             inUpdSts(&pcb->sts.notifyTx, pcb);
             
             inGenPdu(dCb, &pduHdr, &allPdus, pcb->cfg.swtch, 
                      ntc->callType);
             
             
             break;        
           default:
             inNetEVTINV(ntc, dCb);
             break;
         }
         break;
      
      default:
         /*
          * Ignore the event, it changes nothing.
          */
#if (ERRCLASS & ERRCLS_INT_PAR)
         INLOGERROR(ERRCLS_INT_PAR, EIN428, (ErrVal) pcb->cfg.swtch,
                    "inNetE29S07() failed, invalid event.");
#endif
         inIgnore(ntc, dCb);
         break;
   }
   RETVALUE(ROK);
} /* end of inNetE29S07 */







/*
 *
 *       Fun:   inNetE29SND
 *
 *       Desc:  Connection Status Request
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
         File:  qn_bdy6.c
 *
*/

#ifdef ANSI
PUBLIC S16 inNetE29SND
(
InNtc *ntc,
InCb *dCb 
)
#else
PUBLIC S16 inNetE29SND(ntc,dCb)
InNtc *ntc;
InCb *dCb;
#endif
{
   InPCB *pcb;
   CnStEvnt *cnStEvnt;
   PduHdr pduHdr;
   AllPdus allPdus;
   S16 ret;
   U8 tmrNum;
   U8 protDisc;
   Buffer *mBuf;

   TRC2(inNetE29SND);


   mfTwoByte = FALSE;

   /* get physical link control block */
   if ((pcb = *(pcbLstPtr + dCb->suId)) == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      INLOGERROR(ERRCLS_DEBUG, EIN429, (ErrVal) dCb->rSuId,
                 "inNetE29SND() failed, unable to access DLSAP.");
#endif
      RETVALUE(RFAILED);
   }



   /* if physical link is restarting, ignore request, upper layer call will
      be cleared by restart procedures */
   if (ntc->ctldPcb->state == CBST2RST)
   {
     inGenAlarm(LCM_CATEGORY_INTERNAL, LCM_EVENT_INV_STATE, 
                LCM_CAUSE_INV_PCB_STATE, ntc->ctldPcb->suId);
     RETVALUE(ROK);
   }


   cnStEvnt = (CnStEvnt *) ntc->sduSp;

   /* FID 17524.0 */
   inIsFirstChnlId(ntc, &cnStEvnt->chanId, pcb->cfg.channelId);

   mfTwoByte = FALSE;
   if ((cnStEvnt->chanId.eh.pres) &&
       ((cnStEvnt->chanId.chanMapType.pres) &&
       (cnStEvnt->chanId.chanMapType.val == CMT_ATMVCC)))
   {
     mfTwoByte = TRUE; 
   }


   if (((pcb->cfg.swtch == SW_NTNI) || 
        (pcb->cfg.swtch == SW_NTMCI) ||
        (pcb->cfg.swtch == SW_NI2))&&
       ((ntc->evntType != MI_ALERTING)
        && (ntc->evntType != MI_PROGRESS)
        && (ntc->evntType != MI_STAT)))
   {
      /* 
       * If event other than Alerting or Progress 
       * return an error 
       */
#if (ERRCLASS & ERRCLS_INT_PAR) /* addition : in014.310 */
      INLOGERROR(ERRCLS_INT_PAR, EIN430, (ErrVal)ntc->evntType,
                 "InNetE29SND failed, Incorrect Event Type issued. \n");
      INSETTOKVAL(ntc->causeDgn.causeVal, CCPROTERR);
#endif /*(ERRCLASS & ERRCLS_DEBUG)*/

      inGenStaCfm(ntc, &ntc->causeDgn);
      RETVALUE(ROK);
   }

   /* TELICA-BUG:15265-mqin */
   if (ntc->evntType == MI_STAT)
   {
      if (pcb->cfg.swtch == SW_NI2)
      {
         inGenStatusReq(ntc, dCb);
      }
      RETVALUE(ROK);
   }

   switch (ntc->evntType)
   {
      case MI_ALERTING:           /* Alerting, c, p, u */
         if ((ntc->ctldPcb->cfg.swtch == SW_NTDMS100P) || 
             (ntc->ctldPcb->cfg.swtch == SW_NTDMS250) || 
             (ntc->ctldPcb->cfg.swtch == SW_NTMCI) || 
             (ntc->ctldPcb->cfg.swtch == SW_NTNI) || 
             (ntc->ctldPcb->cfg.swtch == SW_ATT5EP) || 
             (ntc->ctldPcb->cfg.swtch == SW_ATT4E))
         {
            if ((ntc->origin) && (ntc->ctldPcb->cfg.nfasInt))
            {
               /* Set octet 3.2 and 3.3 of chanId to NOTPRSNT for H11 channels */
               if ( (cnStEvnt->chanId.eh.pres)
                    && (cnStEvnt->chanId.intIdent.pres == PRSNT_NODEF)
                    && (cnStEvnt->chanId.chanNmbSlotMap.pres == PRSNT_NODEF) 
                    && (cnStEvnt->chanId.nmbMap.pres == PRSNT_NODEF)
                    && (cnStEvnt->chanId.nmbMap.val == NM_CHNMAP) )
               {
                  if ( (cnStEvnt->chanId.chanNmbSlotMap.len == 3) && 
                       (cnStEvnt->chanId.chanNmbSlotMap.val[0] == 0xff ) &&
                       (cnStEvnt->chanId.chanNmbSlotMap.val[1] == 0xff ) &&
                       (cnStEvnt->chanId.chanNmbSlotMap.val[2] == 0xff ) )
                  {
                     cnStEvnt->chanId.chanMapType.pres = NOTPRSNT;
                     cnStEvnt->chanId.nmbMap.pres = NOTPRSNT;
                     cnStEvnt->chanId.codeStand1.pres = NOTPRSNT;
                     cnStEvnt->chanId.chanNmbSlotMap.pres = NOTPRSNT;
                  }
               }  
            } /* CALL_OUT */
         } /* DMS100P / DMS250 / NTMCI / NTNI / ATT5EP / ATT4E */

         for (tmrNum = 0; tmrNum < MAXSIMTIMER; tmrNum++)
            if (ntc->timers[tmrNum].tmrEvnt == TMR_T302) 
               inRmvNtcTq(ntc, tmrNum);
      
         /* Change state to call delivered */
         ntc->state = ST_CALLDEL;

         if (ntc->updStatus == NTC_NOTCREATED)
         {
            ret = zqRunTimeUpd(ZQ_NTC_CB, CMPFTHA_CREATE_REQ, (PTR)ntc);
            if (ret == ROK)
               ntc->updStatus = NTC_CREATED;
         }
         else
            zqRunTimeUpd(ZQ_NTC_CB, CMPFTHA_UPD_REQ, (PTR)ntc);

         inUpdSts(&pcb->sts.alertingTx, pcb);
         /* Generate Connect status request message to Lower layer */
      
         /* prepare Pdu header */
         protDisc = (U8) PD_Q931;
         inInitPduHdr(protDisc, ntc->origin, pcb->cfg.callRefLen,ntc->dCallRef,
                      (U8) M_ALERTING, &pduHdr);
      
         MFINITPDU(&ntc->dCallCb->mfMsgCtl, ret, (U8) PM_CNSTREQ, 
                   (U8) MI_ALERTING, (ElmtHdr *) cnStEvnt, (ElmtHdr *) &allPdus, 
                   (U8) PRSNT_NODEF, pcb->cfg.swtch, ntc->callType);

         if (pcb->cfg.swtch == SW_NTDMS100B)     /* switch - nt dms100 */
            switch (ntc->bearCap)
            { 
               case ITC_SPEECH:                  /* speech */
               case ITC_A31KHZ:                  /* 3.1 audio */
               case ITC_A7KHZ:                   /* 7 audio */
                  INSETTOKHDR(allPdus.m.alerting.progInd);
                  INSETTOKVAL(allPdus.m.alerting.progInd.location,LOC_PUBNETLU);
                  INSETTOKVAL(allPdus.m.alerting.progInd.codeStand0, CSTD_CCITT);
                  INSETTOKVAL(allPdus.m.alerting.progInd.progDesc, PD_IBAVAIL);
               default:
                  /* set signal to 1 */
                  INSETTOKHDR(allPdus.m.alerting.signal);
                  INSETTOKVAL(allPdus.m.alerting.signal.signal,SIG_RNGBCK);
                  break;
            }

         inGenPdu(dCb, &pduHdr, &allPdus, pcb->cfg.swtch, 
                  ntc->callType);
         break;
      
      case MI_CALLPROC:           /* Call Proceeding, c, p, u */
         /* Stop T302 */

         STOP_NTC_TIMER(TMR_T302);
         if (ntc->state != ST_OUTPROC)
         {
            /* Change state to outgoing call proceeding */
            ntc->state = ST_OUTPROC;

            inUpdSts(&pcb->sts.callProcTx, pcb);
         
            /* prepare Pdu header */
            protDisc = (U8) PD_Q931;
            inInitPduHdr(protDisc, ntc->origin, pcb->cfg.callRefLen, 
                         ntc->dCallRef, (U8) M_CALLPROC, &pduHdr);
         
            MFINITPDU(&ntc->dCallCb->mfMsgCtl, ret, (U8) PM_CNSTREQ, 
                      (U8) MI_CALLPROC, (ElmtHdr *) cnStEvnt,
                      (ElmtHdr *) &allPdus, (U8) PRSNT_NODEF, 
                      pcb->cfg.swtch, ntc->callType);
            inGenPdu(dCb, &pduHdr, &allPdus, pcb->cfg.swtch, 
                     ntc->callType);
         }
         break;

      case MI_PROGRESS:           /* Progress, c, p, u */
         switch (pcb->cfg.swtch)
         {
            case SW_ATT4E:
            case SW_ATT5EP:
            case SW_ATT5EB:
               ntc->state = ST_CALLDEL;
               if (ntc->updStatus == NTC_NOTCREATED)
               {
                  ret = zqRunTimeUpd(ZQ_NTC_CB, CMPFTHA_CREATE_REQ, (PTR)ntc);
                  if (ret == ROK)
                     ntc->updStatus = NTC_CREATED;
               }
               else
                  zqRunTimeUpd(ZQ_NTC_CB, CMPFTHA_UPD_REQ, (PTR)ntc);

               break;
            case SW_CCITT:
#ifdef ETSI                       /* TELICA-dsolanki-INTMGCDEV */
           case SW_ETSI:         /* switch - etsi */
#endif                           /* TELICA-dsolanki-INTMGCDEV */
            case SW_NTDMS100B:     /* switch - nt dms100 */
            case SW_NTDMS100P:     /* switch - nt dms100 */
            case SW_NTDMS250:      /* switch - nt dms250 */
            case SW_NI2:             /* switch - NI2 */
               break;
                
            default:
#if (ERRCLASS & ERRCLS_INT_PAR) /* addition : in014.310 */
               INLOGERROR(ERRCLS_INT_PAR, EIN431, (ErrVal)pcb->cfg.swtch,
                          "inNetE29SND() failed, invalid event.");
#endif
               RETVALUE(RFAILED);
               break;
         }

     
         /* Stop T302 */
         STOP_NTC_TIMER(TMR_T302);
         inUpdSts(&pcb->sts.progressTx, pcb);
     
         /* prepare Pdu header */
         inInitPduHdr((U8) PD_Q931, ntc->origin, pcb->cfg.callRefLen, 
                      ntc->dCallRef, (U8) M_PROGRESS, &pduHdr);
     
         MFINITPDU(&ntc->dCallCb->mfMsgCtl, ret, (U8) PM_CNSTREQ, 
                   (U8) MI_PROGRESS, (ElmtHdr *) cnStEvnt, (ElmtHdr *) &allPdus,
                   (U8) PRSNT_NODEF, pcb->cfg.swtch, ntc->callType);
         inGenPdu(dCb, &pduHdr, &allPdus, pcb->cfg.swtch, 
                  ntc->callType);
         break;
   
      case MI_INFO:               /* Information, c, p, u */
         inUpdSts(&pcb->sts.infoTx, pcb);

         /* Stop T304 */
         STOP_NTC_TIMER(TMR_T304);

         /* Start T304 */
         inStartNtcTmr(TMR_T304, ntc, ntc->dCallCb);
      
         /* Generate Connect Status Request message to Lower layer */
         /* prepare Pdu header */
         protDisc = PD_Q931;

         inInitPduHdr(protDisc, ntc->origin, pcb->cfg.callRefLen, 
                      ntc->dCallRef, (U8) M_INFO, &pduHdr);
      
         MFINITPDU(&ntc->dCallCb->mfMsgCtl, ret, (U8) PM_CNSTREQ, (U8)MI_INFO, 
                   (ElmtHdr *) cnStEvnt, (ElmtHdr *) &allPdus, (U8) PRSNT_NODEF, 
                   pcb->cfg.swtch, ntc->callType);
         inBldMsg(dCb, &pduHdr, &allPdus, ntc->dCallCb->pst.region, 
                  ntc->dCallCb->pst.pool, &mBuf, pcb->cfg.swtch, ntc->callType);
#ifdef ISDN_PTMPT
         if ((pcb->cfg.intType == NETWORK)
             && ((pcb->cfg.intCfg == INTCFG_MULTI)
                 && (ntc->setupBCast == TRUE)))
         {
            S16 i;
            for (i=0; i< ntc->nmbValidCes; i++) 
            {
               /*
                * The function will copy the message before transmitting.
                */
               inIntPrimInfoReq(ntc, dCb, mBuf);
            }
            inDropData(mBuf);
            RETVALUE(ROK);
         }
#endif /* ISDN_PTMPT */

#ifdef ISDN_SEGMENT
         inXmitSegMsg(dCb, mBuf);
#else  /* No Segmentation. */
         inSndMsg(dCb, mBuf);
#endif 
         break;
     case MI_NOTIFY:           /* Progress, c, p, u */
       protDisc = (U8) PD_Q931;
       
       /* prepare Pdu header */
       inInitPduHdr((U8) protDisc, ntc->origin, pcb->cfg.callRefLen, 
                    ntc->dCallRef, (U8) M_NOTIFY, &pduHdr);
       
       /* initialize Notify Message */
       MFINITPDU(&ntc->dCallCb->mfMsgCtl, ret, (U8) PM_CNSTREQ, 
                 (U8) MI_NOTIFY, (ElmtHdr *) ntc->sduSp, 
                 (ElmtHdr *) &allPdus, (U8) PRSNT_NODEF, pcb->cfg.swtch,
                 ntc->callType);
       
       inUpdSts(&pcb->sts.notifyTx, pcb);
       
       inGenPdu(dCb, &pduHdr, &allPdus, pcb->cfg.swtch, 
                ntc->callType);
       break;    
     default:
       inNetEVTINV(ntc, dCb);
       RETVALUE(ROK);
   }

   zqUpdPeer();

   RETVALUE(ROK);
} /* end of inNetE29SND */







/*
 *
 *       Fun:   inNetE29S10
 *
 *       Desc:  Input: Connect Status Event 
 *              State: Active
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
         File:  qn_bdy6.c
 *
*/

#ifdef ANSI
PUBLIC S16 inNetE29S10
(
InNtc *ntc,
InCb *dCb 
)
#else
PUBLIC S16 inNetE29S10(ntc, dCb)
InNtc *ntc;
InCb *dCb;
#endif
{
   InPCB *pcb;
   PduHdr pduHdr;
   AllPdus allPdus;
   U8 protDisc;
   S16 ret;

   TRC2(inNetE29S10);

   mfTwoByte = FALSE;

   /* get physical link control block */
   if ((pcb = *(pcbLstPtr + dCb->suId)) == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      INLOGERROR(ERRCLS_DEBUG, EIN433, (ErrVal)  dCb->rSuId,
                 "inNetE29S10() failed, unable to access DLSAP.");
#endif
      RETVALUE(RFAILED);
   }

   /* if phisycal link is restarting, ignore request, 
    * upper layer call will
    * be cleared by restart procedures 
    */

   if (ntc->ctldPcb->state == CBST2RST)
      RETVALUE(ROK);

   /* FID 17524.0 */
   inIsFirstChnlId(ntc, &ntc->sduSp->m.cnStEvnt.chanId, pcb->cfg.channelId);

   if ((ntc->sduSp->m.cnStEvnt.chanId.eh.pres) &&
       ((ntc->sduSp->m.cnStEvnt.chanId.chanMapType.pres) &&
       (ntc->sduSp->m.cnStEvnt.chanId.chanMapType.val == CMT_ATMVCC)))
   {
     mfTwoByte = TRUE; 
   }

   /* TELICA-BUG:15265-mqin */
   if (ntc->evntType == MI_STAT)
   {
      if (pcb->cfg.swtch == SW_NI2)
      {
         inGenStatusReq(ntc, dCb);
      }
      RETVALUE(ROK);
   }

   switch (pcb->cfg.swtch)
   {
      case SW_NTDMS100B:     /* switch - nt dms100 */
      case SW_NTDMS100P:     /* switch - nt dms100 */
      case SW_NTDMS250:      /* switch - nt dms250 */
      case SW_CCITT:
#ifdef ETSI                 /* TELICA-dsolanki-INTMGCDEV */
      case SW_ETSI:         /* switch - etsi */
#endif                      /* TELICA-dsolanki-INTMGCDEV */
         switch (ntc->evntType)
         {
            case MI_NOTIFY:           /* notify, c, p, u */
               protDisc = (U8) PD_Q931;

               /* prepare Pdu header */
               inInitPduHdr((U8) protDisc, ntc->origin, pcb->cfg.callRefLen, 
                            ntc->dCallRef, (U8) M_NOTIFY, &pduHdr);

               /* initialize Notify Message */
               MFINITPDU(&ntc->dCallCb->mfMsgCtl, ret, (U8) PM_CNSTREQ, 
                         (U8) MI_NOTIFY, (ElmtHdr *) ntc->sduSp, 
                         (ElmtHdr *) &allPdus, (U8) PRSNT_NODEF, pcb->cfg.swtch,
                         ntc->callType);

               inUpdSts(&pcb->sts.notifyTx, pcb);
               inGenPdu(dCb, &pduHdr, &allPdus, pcb->cfg.swtch, 
                        ntc->callType);
               break;

            case MI_INFO:           /* info, c, p, u */
               switch (pcb->cfg.swtch)
               {
                  case SW_CCITT:
#ifdef ETSI                             /* TELICA-dsolanki-INTMGCDEV */
                  case SW_ETSI:         /* switch - etsi */
#endif                                  /* TELICA-dsolanki-INTMGCDEV */
                     break;

                  default:
                     inNetEVTINV(ntc, dCb);
                     RETVALUE(ROK);
               }
               inUpdSts(&pcb->sts.infoTx, pcb);

               /* prepare Pdu header */
               inInitPduHdr((U8) PD_Q931, ntc->origin, pcb->cfg.callRefLen, 
                            ntc->dCallRef, (U8) M_INFO, &pduHdr);

               /* initialize Info Message */
               MFINITPDU(&ntc->dCallCb->mfMsgCtl, ret, (U8) PM_CNSTREQ, 
                         (U8) MI_INFO, (ElmtHdr *)ntc->sduSp, (ElmtHdr *) &allPdus,
                         (U8) PRSNT_NODEF, pcb->cfg.swtch, ntc->callType);

               inGenPdu(dCb, &pduHdr, &allPdus, pcb->cfg.swtch, 
                        ntc->callType);
               break;

            default:
               inNetEVTINV(ntc, dCb);
               break;
         }
         break;
      case SW_ATT4E:
      case SW_ATT5EP:
      case SW_ATT5EB:
         break;
      default:
#if (ERRCLASS & ERRCLS_DEBUG)
         INLOGERROR(ERRCLS_DEBUG, EIN434, (ErrVal) pcb->cfg.swtch,
                    "inNetE29SND() failed, invalid event.");
#endif
         inNetEVTINV(ntc, dCb);
         break;
   }
   RETVALUE(ROK);
} /* end of inNetE29S10 */






/*
 *
 *       Fun:   inNetE29S15
 *
 *       Desc:  Input: Connect Status Event 
 *              State: Suspend Request
 *              Allows Notify message to be sent
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
         File:  qn_bdy6.c
 *
*/

#ifdef ANSI
PUBLIC S16 inNetE29S15
(
InNtc *ntc,
InCb *dCb 
)
#else
PUBLIC S16 inNetE29S15(ntc, dCb)
InNtc *ntc;
InCb *dCb;
#endif
{
   InPCB *pcb;
   PduHdr pduHdr;
   AllPdus allPdus;
   S16 ret;

   TRC2(inNetE29S15);

   /* get physical link control block */
   if ((pcb = *(pcbLstPtr + dCb->suId)) == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      INLOGERROR(ERRCLS_DEBUG, EIN435, (ErrVal)dCb->rSuId,
                 "inNetE29S15() failed, unable to access DLSAP.");
#endif
      RETVALUE(RFAILED);
   }

   /* if physical link is restarting, ignore request, upper layer call will
      be cleared by restart procedures */
   if (ntc->ctldPcb->state == CBST2RST)
   {
     inGenAlarm(LCM_CATEGORY_INTERNAL, LCM_EVENT_INV_STATE, 
                LCM_CAUSE_INV_PCB_STATE, ntc->ctldPcb->suId);
     RETVALUE(ROK);
   }


   switch (pcb->cfg.swtch)
   {
#ifdef ETSI                   /* TELICA-dsolanki-INTMGCDEV */
      case SW_ETSI:          /* switch - etsi */
         break;
#endif                        /* TELICA-dsolanki-INTMGCDEV */
      default:
#if (ERRCLASS & ERRCLS_INT_PAR)
         INLOGERROR(ERRCLS_INT_PAR, EIN436, (ErrVal)pcb->cfg.swtch,
                    "inNetE29S15() failed, invalid event.");
#endif
         inIgnore (ntc, dCb);
         RETVALUE(RFAILED);
         break;
   }

   switch (ntc->evntType)
   {
      case MI_NOTIFY:           /* notify, c, p, u */
         inUpdSts(&pcb->sts.notifyTx, pcb);

         /* prepare Pdu header */
         inInitPduHdr((U8) PD_Q931, ntc->origin, pcb->cfg.callRefLen, 
                      ntc->dCallRef, (U8) M_NOTIFY, &pduHdr);

         /* initialize Notify Message */
         MFINITPDU(&ntc->dCallCb->mfMsgCtl, ret, (U8) PM_CNSTREQ, 
                   (U8) MI_NOTIFY, (ElmtHdr *) ntc->sduSp, (ElmtHdr *) &allPdus, 
                   (U8) PRSNT_NODEF, pcb->cfg.swtch, ntc->callType);

         inGenPdu(dCb, &pduHdr, &allPdus, pcb->cfg.swtch, 
                  ntc->callType);
         break;

      default:
         inNetEVTINV(ntc, dCb);
         RETVALUE(ROK);
   }
   RETVALUE(ROK);
} /* end of inNetE29S15 */




  
/*
 *
 *       Fun:   inNetE30SND
 *
 *       Desc:  Disconnect Request
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
         File:  qn_bdy6.c
 *
*/

#ifdef ANSI
PUBLIC S16 inNetE30SND
(
InNtc *ntc,
InCb *dCb 
)
#else
PUBLIC S16 inNetE30SND(ntc, dCb)
InNtc *ntc;
InCb *dCb;
#endif
{
   InPCB *pcb;
   PduHdr pduHdr;
   AllPdus allPdus;
   S16 ret;
   U8 protDisc;
   U8 tmrNum;
   char fn[] = "inNetE30SND";

   TRC2(inNetE30SND);
   /* get physical link control block */
   if ((pcb = *(pcbLstPtr + dCb->suId)) == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      INLOGERROR(ERRCLS_DEBUG, EIN437, (ErrVal) dCb->rSuId,
                 "inNetE30SND() failed, unable to access DLSAP.");
#endif
      DP("%s -  failed, unable to access DLSAP dCb->rSuId = 0x%lx\n", fn, dCb->rSuId);
      RETVALUE(RFAILED);
   }

   /*
    * if physical link is restarting, ignore request, 
    * upper layer call will be cleared by restart procedures 
    */
   if (ntc->ctldPcb->state == CBST2RST)
   {
     inGenAlarm(LCM_CATEGORY_INTERNAL, LCM_EVENT_INV_STATE, 
                LCM_CAUSE_INV_PCB_STATE, ntc->ctldPcb->suId);
     RETVALUE(ROK);
   }
    
   if ((ntc->state == ST_ACTVE) && !(ntc->origin))
      inGenBillNm(ntc);

   /*
    * Stop all Timers 
    */

   STOP_NTC_TIMERS;

   ntc->tmrCnt = 0;
   pcb->sts.discs++;
    /* Bukucu - Bug-12959 decrement it only if it's greater than 0.
     * even if we count this counter correctly, we may have a case
     * where it goes below zero, after a failover
    */
   if ((ntc->state == ST_ACTVE) && (pcb->sts.aveCalls > 0))
      pcb->sts.aveCalls--;

#ifdef ISDN_PTMPT
   if ((pcb->cfg.intCfg == INTCFG_MULTI)
       && ((pcb->cfg.intType == NETWORK)
           && (ntc->setupBCast == TRUE)))
   {
      if (ntc->state == ST_CALLPRES)
      {
         IN_TST_ASSERT("Disconnect Request in Call Present State.");
         STOP_NTC_TIMER(TMR_T303);
         inPrcCallAbort(ntc); 
         RETVALUE(ROK);
      } /* if (ntc->state == ST_CALLPRES) */
      
      if ((ntc->state == ST_CALLRECV)
          || ((ntc->state == ST_CONNRQ)
              || ((ntc->state == ST_INPROC)
                  ||(ntc->state == ST_OVLAPRCV))))
      {
         /* Stop appropriate timers */
         switch (ntc->state)
         {
            case ST_CALLRECV:
               STOP_NTC_TIMER(TMR_T301);
               break;
            case ST_CONNRQ:
               break;
            case ST_INPROC:
               STOP_NTC_TIMER(TMR_T310);
               break;
            case ST_OVLAPRCV:
               STOP_NTC_TIMER(TMR_T304);
               break;
 
         }
         /* 
          * Cause, if present is initialized in the
          * primitive
          */

         inPrcReleaseAllCes(ntc);
         RETVALUE(ROK);
      }
   }
#endif /* ISDN_PTMPT */

   /* 
    * Change state to disconnect indication 
    */
   ntc->state = ST_DISCIN;

   /* 
    * Start Timer T305 
    */
   inStartNtcTmr(TMR_T305, ntc, ntc->dCallCb);

   /*
    * FID 16706.0 - If the Facility IE is present store it in case T305 expires.
    */
   if ((pcb->cfg.aoc) && (ntc->sduSp->m.discEvnt.facilityStr1.eh.pres))
   {
      INDBGP(DBGMASK_LI, "%s storing the facilityStr1 in NTC\n",__FUNCTION__);

      /* Allocate space if it's not already there */
      if (ntc->facilityStr1 == NULLP)
      {
         if ((ret = SGetSBuf(DFLT_REGION, DFLT_POOL, (Data **)&(ntc->facilityStr1),
               (Size) sizeof(FacilityStr))) != ROK)
         {
#if (ERRCLASS & ERRCLS_DEBUG)
            INLOGERROR(ERRCLS_DEBUG, EIN250, (ErrVal) sizeof(FacilityStr),
                       "inNetE30SND failed, unable to allocate memory.");
#endif
            RETVALUE(RFAILED);
         }
      }
      cmMemcpy((U8 *)(ntc->facilityStr1), (U8 *)&ntc->sduSp->m.discEvnt.facilityStr1,
                 sizeof(FacilityStr));
   }

   inUpdSts(&pcb->sts.discTx, pcb);

   /* 
    * Generate Disconnect Request message to Lower layer.
    * Set protocol discriminator 
    */
   protDisc = PD_Q931;
   
#if (ISDN_BC303TMC || ISDN_BC303CSC)
   if ((pcb->cfg.swtch == SW_BC303TMC) ||
       (pcb->cfg.swtch == SW_BC303CSC))
   {
      protDisc = (U8) PD_BC303;
   }
#endif

   /* 
    * prepare Pdu header 
    */
   inInitPduHdr((U8) protDisc, ntc->origin, pcb->cfg.callRefLen, ntc->dCallRef,
                (U8) M_DISC, &pduHdr);

   MFINITPDU(&ntc->dCallCb->mfMsgCtl, ret, (U8) PM_DISCREQ, (U8) MI_DISC, 
             (ElmtHdr *) ntc->sduSp, (ElmtHdr *) &allPdus, (U8) PRSNT_NODEF, 
             pcb->cfg.swtch, ntc->callType);

   inGenPdu(dCb, &pduHdr, &allPdus, pcb->cfg.swtch, ntc->callType);
   RETVALUE(ROK);

} /* end of inNetE30SND */



/*
*
*       Fun:   inNetE30S01
*
*       Desc:  Disconnect Request primitive
               State: call initiated
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  qn_bdy6.c
*
*/

#ifdef ANSI
PUBLIC S16 inNetE30S01
(
InNtc *ntc,
InCb *dCb 
)
#else
PUBLIC S16 inNetE30S01(ntc, dCb)
InNtc *ntc;
InCb *dCb;
#endif
{
   InPCB *pcb;
   PduHdr pduHdr;
   AllPdus allPdus;
   S16 ret;

   TRC2(inNetE30S01);

   /* get physical link control block */
   if ((pcb = *(pcbLstPtr + dCb->suId)) == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      INLOGERROR(ERRCLS_DEBUG, EIN438, (ErrVal) dCb->rSuId,
                 "inNetE30S01() failed, unable to access DLSAP.");
#endif
      RETVALUE(RFAILED);
   }

   /* if physical link is restarting, ignore request, upper layer call will
      be cleared by restart procedures */
   if (ntc->ctldPcb->state == CBST2RST)
   {
     inGenAlarm(LCM_CATEGORY_INTERNAL, LCM_EVENT_INV_STATE, 
                LCM_CAUSE_INV_PCB_STATE, ntc->ctldPcb->suId);
     RETVALUE(ROK);
   } 

#if (ISDN_BC303TMC || ISDN_BC303CSC)
   if ( ((pcb->cfg.swtch == SW_BC303TMC) || (pcb->cfg.swtch == SW_BC303CSC))
        && (ntc->state == ST_CALLINIT) )
   {
      ret = inNetE30SND(ntc, dCb);
      RETVALUE(ret);
   }
#endif /* ISDN_BC303TMC || ISDN_BC303CSC */

   switch (pcb->cfg.swtch)
   {
      case SW_NTDMS100B:     /* switch - nt dms100 */
      case SW_NTDMS100P:     /* switch - nt dms100 */
      case SW_NTDMS250:      /* switch - nt dms250 */
      case SW_NI2:           /* switch -  ni2 */
      case SW_ATT4E:
      case SW_ATT5EP:
      case SW_ATT5EB:
      case SW_CCITT:
#ifdef ETSI                 /* TELICA-dsolanki-INTMGCDEV */
      case SW_ETSI:         /* switch - etsi */
#endif                      /* TELICA-dsolanki-INTMGCDEV */

         /* TELICA-MCAO-12/07/01: CC is waiting for a relind in this case */
         {
            InCb *tCb;
            AllSdus ev;

            tCb = ntc->tCallCb;
            if (tCb)
            {
               /* Send Release Indication to Upper */
               MFINITSDU(&tCb->mfMsgCtl, ret, (U8) PM_DISCREQ, (U8) PM_RELREQ, 
                         (ElmtHdr *) &ntc->sduSp->m.discEvnt, (ElmtHdr *)  &ev,
                         (U8) PRSNT_NODEF, pcb->cfg.swtch, ntc->callType);
                
               InUiIntRelInd(&tCb->pst, tCb->suId, (InInstId)ntc->suInstId, 
                             (InInstId)ntc->tCallRef,&ev.m.relEvnt);
            }
         }

         /* Release the B-channel */
         inRelChan(ntc);
         inUpdSts(&pcb->sts.relCmpltTx, pcb);

         /* Send Release Complete to Lower */
         /* prepare Pdu header */
#if (ISDN_BC303TMC || ISDN_BC303CSC)
         if ((pcb->cfg.swtch == SW_BC303TMC) ||
             (pcb->cfg.swtch == SW_BC303CSC))
         {
            inInitPduHdr((U8) PD_BC303, ntc->origin, pcb->cfg.callRefLen, 
                         ntc->dCallRef,  (U8) M_RELCMPLT, &pduHdr);
         }
         else
#endif /* ISDN_BC303TMC || ISDN_BC303CSC */
         inInitPduHdr((U8) PD_Q931, ntc->origin, pcb->cfg.callRefLen,
                      ntc->dCallRef, (U8) M_RELCMPLT, &pduHdr);

         MFINITPDU(&ntc->dCallCb->mfMsgCtl, ret, (U8) PM_DISCREQ,
                   (U8) MI_RELCMPLT, (ElmtHdr *) ntc->sduSp, (ElmtHdr *) &allPdus,
                   (U8) PRSNT_NODEF, pcb->cfg.swtch, ntc->callType);
         inGenPdu(dCb, &pduHdr, &allPdus, pcb->cfg.swtch, 
                  ntc->callType);

         /* Release CR */
         inRelCalRef(ntc);
         break;
      default:
#if (ERRCLASS & ERRCLS_INT_PAR)
         INLOGERROR(ERRCLS_INT_PAR, EIN439, (ErrVal) pcb->cfg.swtch,
                    "inNetE30S01() failed, invalid event.");
#endif
         inNetEVTINV(ntc, dCb);
         break;
   }
   RETVALUE(ROK);
} /* end of inNetE30S01 */
  

/*
*
*       Fun:   inNetE30S03
*
*       Desc:  Disconnect Request
*              state outgoing call proceeding
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  qn_bdy6.c
*
*/

#ifdef ANSI
PUBLIC S16 inNetE30S03
(
InNtc *ntc,
InCb *dCb 
)
#else
PUBLIC S16 inNetE30S03(ntc, dCb)
InNtc *ntc;
InCb *dCb;
#endif
{
   InPCB *pcb;
   PduHdr pduHdr;
   AllPdus allPdus;
   S16 ret;
   InCb *tCb;
   U8 tmrNum;

   TRC2(inNetE30S03);
   
   tCb = ntc->tCallCb;
   /* get physical link control block */
   if ((pcb = *(pcbLstPtr + dCb->suId)) == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      INLOGERROR(ERRCLS_DEBUG, EIN440, (ErrVal) dCb->rSuId,
                 "inNetE30S03() failed, unable to access DLSAP.");
#endif
      RETVALUE(RFAILED);
   }

   /* 
    * If physical link is restarting, ignore request, 
    * upper layer call will be cleared by restart procedures 
    */
   if (ntc->ctldPcb->state == CBST2RST)
   {
     inGenAlarm(LCM_CATEGORY_INTERNAL, LCM_EVENT_INV_STATE, 
                LCM_CAUSE_INV_PCB_STATE, ntc->ctldPcb->suId);
     RETVALUE(ROK);
   }    

   STOP_NTC_TIMERS;

   ntc->tmrCnt = 0;

   switch (pcb->cfg.swtch)
   {
      case SW_NTDMS100B:     /* switch - nt dms100 */
      case SW_NTDMS100P:     /* switch - nt dms100 */
      case SW_NTDMS250:      /* switch - nt dms250 */
      case SW_NI2:            /* switch - NI2 */
      case SW_CCITT:
#ifdef ETSI                  /* TELICA-dsolanki-INTMGCDEV */
      case SW_ETSI:         /* switch - etsi */
#endif                     /* TELICA-dsolanki-INTMGCDEV */
         /* 
          * Change state to disconnect indication 
          */
         ntc->state = ST_DISCIN;
         pcb->sts.discs++;

         /* 
          * Start Timer T305 
          */
         inStartNtcTmr(TMR_T305, ntc, ntc->dCallCb);
         inUpdSts(&pcb->sts.discTx, pcb);

         /* 
          * Generate Disconnect Request message to Lower layer 
          */

         /* prepare Pdu header */
         inInitPduHdr((U8) PD_Q931, ntc->origin, pcb->cfg.callRefLen, 
                      ntc->dCallRef, (U8) M_DISC, &pduHdr);

         MFINITPDU(&ntc->dCallCb->mfMsgCtl, ret, (U8) PM_DISCREQ, (U8) MI_DISC,
                   (ElmtHdr *) ntc->sduSp, (ElmtHdr *) &allPdus, (U8) PRSNT_NODEF, 
                   pcb->cfg.swtch, ntc->callType);
         inGenPdu(dCb, &pduHdr, &allPdus, pcb->cfg.swtch, 
                  ntc->callType);
         break;
      case SW_ATT4E:
      case SW_ATT5EP:
      case SW_ATT5EB:
         /* Change state to disconnect indication */
         ntc->state = ST_DISCIN;
         pcb->sts.discs++;

         /* Start T305 */
         inStartNtcTmr(TMR_T305, ntc, ntc->dCallCb);
         inUpdSts(&pcb->sts.discTx, pcb);

         /* prepare Pdu header */
         inInitPduHdr((U8) PD_Q931, ntc->origin, pcb->cfg.callRefLen, 
                      ntc->dCallRef, (U8) M_DISC, &pduHdr);

         /* generate disconnect request message to lower layer */
         MFINITPDU(&ntc->dCallCb->mfMsgCtl, ret, (U8) PM_DISCREQ, 
                   (U8) MI_DISC, (ElmtHdr *) ntc->sduSp, (ElmtHdr *) &allPdus, 
                   (U8) PRSNT_NODEF, pcb->cfg.swtch, ntc->callType);
         inGenPdu(dCb, &pduHdr, &allPdus, pcb->cfg.swtch, 
                     ntc->callType);

         break;
      default:

#if (ERRCLASS & ERRCLS_INT_PAR)
         INLOGERROR(ERRCLS_INT_PAR, EIN441, (ErrVal) pcb->cfg.swtch,
                    "inNetE30S03() failed, invalid event.");
#endif
         inNetEVTINV(ntc, dCb);
         break;
   }
   RETVALUE(ROK);
} /* end of inNetE30S03 */

  
/*
 *
 *       Fun:   inNetE30S11
 *
 *       Desc:  Disconnect Request primitive
 State: disconnect request
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
         File:  qn_bdy6.c
 *
*/

#ifdef ANSI
PUBLIC S16 inNetE30S11
(
InNtc *ntc,
InCb *dCb 
)
#else
PUBLIC S16 inNetE30S11(ntc, dCb)
InNtc *ntc;
InCb *dCb;
#endif
{
   InCb *tCb;
   InPCB *pcb;
   PduHdr pduHdr;
   AllPdus allPdus;
   AllSdus ev;
   S16 ret;

   TRC2(inNetE30S11);
   /* get physical link control block */
   if ((pcb = *(pcbLstPtr + dCb->suId)) == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      INLOGERROR(ERRCLS_DEBUG, EIN442, (ErrVal) dCb->rSuId,
                 "inNetE30S11() failed, unable to access DLSAP.");
#endif
      RETVALUE(RFAILED);

   }
   /* 
    * If physical link is restarting, ignore request, 
    * upper layer call will be cleared by restart procedures 
    */
   if (ntc->ctldPcb->state == CBST2RST)
   {
     inGenAlarm(LCM_CATEGORY_INTERNAL, LCM_EVENT_INV_STATE, 
                LCM_CAUSE_INV_PCB_STATE, ntc->ctldPcb->suId);
     RETVALUE(ROK);
   }


   tCb = ntc->tCallCb;


   if (tCb->sapCfg.swtch == SW_NTDMS250)
   {
      inNetEVTINV(ntc, dCb);
      RETVALUE(ROK);
   }

   /* Initialize state to NULL before sending release indication */ 
   ntc->state = ST_NULL;

   /* Send Release Indication to Upper */
   MFINITSDU(&tCb->mfMsgCtl, ret, (U8) PM_DISCREQ, (U8) PM_RELREQ, 
             (ElmtHdr *) &ntc->sduSp->m.discEvnt, (ElmtHdr *)  &ev,
             (U8) PRSNT_NODEF, pcb->cfg.swtch, ntc->callType);

   InUiIntRelInd(&tCb->pst, tCb->suId, (InInstId)ntc->suInstId, 
                 (InInstId)ntc->tCallRef,&ev.m.relEvnt);

   /* Release the B-channel */
   inRelChan(ntc);
   inUpdSts(&pcb->sts.relCmpltTx, pcb);

   /* Send Release Complete to Lower */

   /* prepare Pdu header */
   inInitPduHdr((U8) PD_Q931, ntc->origin, pcb->cfg.callRefLen, ntc->dCallRef,
                (U8) M_RELCMPLT, &pduHdr);

   MFINITPDU(&ntc->dCallCb->mfMsgCtl, ret, (U8) PM_DISCREQ, (U8) MI_RELCMPLT, 
             (ElmtHdr *) ntc->sduSp, (ElmtHdr *) &allPdus, (U8) PRSNT_NODEF, 
             pcb->cfg.swtch, ntc->callType);
   inGenPdu(dCb, &pduHdr, &allPdus, pcb->cfg.swtch, ntc->callType);

   /* Release CR */
   inRelCalRef(ntc);
   RETVALUE(ROK);
} /* end of inNetE30S11 */

  
/*
 *
 *       Fun:   inNetE31S00
 *
 *       Desc:  Input: Release request
 *              State: Null
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
         File:  qn_bdy6.c
 *
*/

#ifdef ANSI
PUBLIC S16 inNetE31S00
(
InNtc *ntc,
InCb *dCb 
)
#else
PUBLIC S16 inNetE31S00(ntc, dCb)
InNtc *ntc;
InCb *dCb;
#endif
{
   TRC2(inNetE31S00);
#ifdef ISDN_SUSPEND
   if (ntc->susFlg)
   {
     inGenRelInd(ntc, &ntc->causeDgn);
     RETVALUE(ROK);
   }
#endif
#if  0
#ifdef Q932
#else
   inNetINES00(ntc, dCb);
#endif
#else
   inNetINES00(ntc, dCb);
#endif
   RETVALUE(ROK);
} /* end of inNetE31S00 */

  
/*
 *
 *       Fun:   inNetE31S06
 *
 *       Desc:  Input: Release request
 *              State: call present
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
         File:  qn_bdy6.c
 *
*/

#ifdef ANSI
PUBLIC S16 inNetE31S06
(
InNtc *ntc,
InCb *dCb 
)
#else
PUBLIC S16 inNetE31S06(ntc, dCb)
InNtc *ntc;
InCb *dCb;
#endif
{
   InPCB *pcb;
   U8 tmrNum;
   S16 ret;

   TRC2(inNetE31S06);

   /* get physical link control block */
   if ((pcb = *(pcbLstPtr + dCb->suId)) == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      INLOGERROR(ERRCLS_DEBUG, EIN444, (ErrVal) dCb->rSuId,
                 "inNetE27S00() failed, unable to access DLSAP.");
#endif
      RETVALUE(RFAILED);
   }

   /* 
    * Stop Timer T303 
    */
   STOP_NTC_TIMER(TMR_T303);

   IN_TST_ASSERT("Release Request in State 6.\n");
   /* Init cause in Ntc to the cause received from layer4 */
   if (ntc->sduSp->m.relEvnt.causeDgn[0].eh.pres)
   {
      MFINITELMT(&ntc->tCallCb->mfMsgCtl, ret,
                 (ElmtHdr *)&ntc->sduSp->m.relEvnt.causeDgn[0],
                 (ElmtHdr *) &ntc->causeDgn, &meCauseDgn,
                 (U8) PRSNT_NODEF, ntc->tCallCb->sapCfg.swtch, ntc->callType);
   }

   /* 
    * Call common disconnect function.
    */
   inNetE30SND(ntc, dCb);

   RETVALUE(ROK);
} /* end of inNetE31S06 */




  
/*
 *
 *       Fun:   inNetE34S15
 *
 *       Desc:  Suspend Response
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
         File:  qn_bdy6.c
 *
*/

#ifdef ANSI
PUBLIC S16 inNetE34S15
(
InNtc *ntc,
InCb *dCb 
)
#else
PUBLIC S16 inNetE34S15(ntc, dCb)
InNtc *ntc;
InCb *dCb;
#endif
{
   InPCB *pcb = NULL;
   PduHdr pduHdr;
   AllPdus allPdus;
   CalId *entry = NULL;
   U8 tmrNum = 0;
   S16 ret = 0;
#ifdef ISDN_SUSPEND
   U8  causVal;
#endif

   TRC2(inNetE34S15);

   UNUSED(ret);
   UNUSED(tmrNum);
   UNUSED(entry);
   UNUSED(allPdus);
   UNUSED(pduHdr);
   UNUSED(pcb);

#ifdef ISDN_SUSPEND
   /* get physical link control block */

   if ((pcb = *(pcbLstPtr + dCb->suId)) == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      INLOGERROR(ERRCLS_DEBUG, EIN445, (ErrVal) dCb->rSuId,
                 "inNetE34S15() failed, unable to access DLSAP.");
#endif
      RETVALUE(RFAILED);
   }
   /* if phisycal link is restarting, ignore request, upper layer call will
      be cleared by restart procedures */
   if (ntc->ctldPcb->state == CBST2RST)
   {
     inGenAlarm(LCM_CATEGORY_INTERNAL, LCM_EVENT_INV_STATE, 
                LCM_CAUSE_INV_PCB_STATE, ntc->ctldPcb->suId);
     RETVALUE(ROK);
   } 
     

   switch (pcb->cfg.swtch)
   {
      case SW_CCITT:
#ifdef ETSI                  /* TELICA-dsolanki-INTMGCDEV */
      case SW_ETSI:         /* switch - etsi */
#endif                       /* TELICA-dsolanki-INTMGCDEV */
         if (ntc->sduSp->m.ssHlEvnt.causeDgn[0].eh.pres)  /* Suspend Reject */
         {
            ntc->susFlg = 0;
            ntc->state = ST_ACTVE;

            if ((entry = inCallIdSearch(ntc->dCallCb, &ntc->callId,
                                        &causVal)))
               inRemCallId(ntc->dCallCb, entry);
            ntc->callId.len = 0;
            inUpdSts(&pcb->sts.suspRejTx, pcb);
            
            /* prepare Pdu header */
            inInitPduHdr((U8) PD_Q931, ntc->origin, pcb->cfg.callRefLen, 
                         ntc->dCallRef,  (U8) M_SUSPREJ, &pduHdr);

            MFINITPDU(&ntc->dCallCb->mfMsgCtl, ret, (U8) PM_SSHLRSP, 
                      (U8) MI_SUSPREJ, (ElmtHdr *) ntc->sduSp, (ElmtHdr *) &allPdus,
                      (U8) PRSNT_NODEF, pcb->cfg.swtch, ntc->callType);
         }
         else
         {
           U8 causVal;
           CalId *entry;

           if ((entry = inCallIdSearch(ntc->dCallCb, &ntc->callId, &causVal)))
           {
             zqRunTimeUpd(ZQ_CALLID_CB, CMPFTHA_CREATE_REQ, (PTR)entry);
           }
           
           /* Remove call reference from both the upper and
            * lower saps. */
           
           inRemCallRef(ntc->dCallCb, ntc->dCallRef);
           
           STOP_NTC_TIMERS;
           
           ntc->tmrCnt = 0;
           
           /* Start T307 */
           inStartNtcTmr(TMR_T307, ntc, ntc->dCallCb);
           
           /* Change state to null */
           ntc->state = ST_NULL;
           
           inUpdSts(&pcb->sts.suspAckTx, pcb);

           /* prepare Pdu header */
           inInitPduHdr((U8) PD_Q931, ntc->origin, pcb->cfg.callRefLen, 
                        ntc->dCallRef,  (U8) M_SUSPACK, &pduHdr);

           MFINITPDU(&ntc->dCallCb->mfMsgCtl, ret, (U8) PM_SSHLRSP,
                     (U8) MI_SUSPACK, (ElmtHdr *) ntc->sduSp, 
                     (ElmtHdr *) &allPdus,
                     (U8) PRSNT_NODEF, pcb->cfg.swtch, ntc->callType);
           
           ntc->dCallRef = 0;
         }

         /* Send Suspend Ack or Reject */
         inGenPdu(dCb, &pduHdr, &allPdus, pcb->cfg.swtch, 
                  ntc->callType);
         break;

      default:
#if (ERRCLASS & ERRCLS_INT_PAR)
         INLOGERROR(ERRCLS_INT_PAR, EIN446, (ErrVal) pcb->cfg.swtch,
                    "inNetE34S15() failed, invalid event.");
#endif
         inNetEVTINV(ntc, dCb);
 
         break;
   }

   zqUpdPeer();

#endif
   RETVALUE(ROK);
} /* end of inNetE34S15 */




  
/*
 *
 *       Fun:   inNetE36S17
 *
 *       Desc:  Resume Response
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
         File:  qn_bdy6.c
 *
*/

#ifdef ANSI
PUBLIC S16 inNetE36S17
(
InNtc *ntc,
InCb *dCb 
)
#else
PUBLIC S16 inNetE36S17(ntc, dCb)
InNtc *ntc;
InCb *dCb;
#endif
{
   InCb *tCb = NULL;
   CalId *entry = NULL;
#ifdef ISDN_SUSPEND
   InPCB *pcb = NULL;
   U8 causVal = 0;
   U8 tmrNum = 0;
   S16 ret = 0;
   U8 i = 0;
#endif
   AllPdus allPdus;
   PduHdr pduHdr;

   TRC2(inNetE36S17);

 
   UNUSED(entry);
   UNUSED(tCb);
   UNUSED(allPdus);
   UNUSED(pduHdr);


#ifdef ISDN_SUSPEND

   mfTwoByte = FALSE;

   tCb = ntc->tCallCb;

   /* get physical link control block */
   if ((pcb = *(pcbLstPtr + dCb->suId)) == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      INLOGERROR(ERRCLS_DEBUG, EIN447, (ErrVal) dCb->rSuId,
                 "inNetE36S17() failed, unable to access DLSAP.");
#endif
      RETVALUE(RFAILED);
   }


   /* if phisycal link is restarting, ignore request, upper layer call will
      be cleared by restart procedures */
   if (ntc->ctldPcb->state == CBST2RST)
   {
     inGenAlarm(LCM_CATEGORY_INTERNAL, LCM_EVENT_INV_STATE, 
                LCM_CAUSE_INV_PCB_STATE, ntc->ctldPcb->suId);
     RETVALUE(ROK);
   } 
   

   switch (pcb->cfg.swtch)
   {
      case SW_CCITT:
#ifdef ETSI                 /* TELICA-dsolanki-INTMGCDEV */
      case SW_ETSI:         /* switch - etsi */
#endif                      /* TELICA-dsolanki-INTMGCDEV */
         if (!ntc->sduSp->m.rmRtEvnt.causeDgn[0].eh.pres) /* resume Ack */
         {
            /* Stop T_307 */
            STOP_NTC_TIMER(TMR_T307);

            if ((entry = inCallIdSearch(ntc->dCallCb, &ntc->callId,
                                        &causVal)))
               inRemCallId(ntc->dCallCb, entry);
            ntc->susFlg = FALSE;
            ntc->callId.len = 0;
            for (i = 0; i < ADRLEN; i++)
               ntc->callId.val[i] = 0;
            ntc->state = ST_ACTVE;
            zqRunTimeUpd(ZQ_CALLID_CB, CMPFTHA_DELETE_REQ, (PTR)entry);


            ntc->causeDgn.eh.pres = PRSNT_NODEF;

            inUpdSts(&pcb->sts.rsmAckTx, pcb);

            /* prepare Pdu header */
            inInitPduHdr((U8) PD_Q931, ntc->origin, pcb->cfg.callRefLen, 
                         ntc->dCallRef,  (U8) M_RSMACK, &pduHdr);

            MFINITPDU(&ntc->dCallCb->mfMsgCtl, ret, (U8) PM_RMRTRSP, 
                      (U8) MI_RSMACK, (ElmtHdr *) ntc->sduSp, 
                      (ElmtHdr *) &allPdus,
                      (U8) PRSNT_NODEF, pcb->cfg.swtch, ntc->callType);
            if (!allPdus.m.rsmAck.chanId.eh.pres)
               /* insert channel Id  */
               inInsrtChnlId(ntc, &allPdus.m.rsmAck.chanId,ntc->ctldPcb);

            if ((allPdus.m.rsmAck.chanId.eh.pres) &&
                (allPdus.m.rsmAck.chanId.chanMapType.pres) &&
                (allPdus.m.rsmAck.chanId.chanMapType.val == CMT_ATMVCC))
            {
              mfTwoByte = TRUE; 
            }

            inGenPdu(dCb, &pduHdr, &allPdus, pcb->cfg.swtch, 
                     ntc->callType);
         }
         else     /* resume Rej */
         {
            inUpdSts(&pcb->sts.rsmRejTx, pcb);
            inRemCallRef(ntc->dCallCb, ntc->dCallRef);

            /* Change state to null */
            ntc->state = ST_NULL;

            /* prepare Pdu header */
            inInitPduHdr((U8) PD_Q931, ntc->origin, pcb->cfg.callRefLen, 
                         ntc->dCallRef, (U8) M_RSMREJ, &pduHdr);

            MFINITPDU(&ntc->dCallCb->mfMsgCtl, ret, (U8) PM_RMRTRSP, 
                      (U8) MI_RSMREJ, (ElmtHdr *) ntc->sduSp, (ElmtHdr *) &allPdus,
                      (U8) PRSNT_NODEF, pcb->cfg.swtch, ntc->callType);

             ntc->dCallRef = 0;

            inGenPdu(dCb, &pduHdr, &allPdus, pcb->cfg.swtch, 
                     ntc->callType);
         }
         break;

      default:
#if (ERRCLASS & ERRCLS_INT_PAR)
         INLOGERROR(ERRCLS_INT_PAR, EIN448, (ErrVal) pcb->cfg.swtch,
                    "inNetE36S17() failed, invalid event.");
#endif
         inNetEVTINV(ntc, dCb);
 
         break;
   }

   zqUpdPeer();

#endif
   RETVALUE(ROK);
} /* end of inNetE36S17 */

/* FID 16970.0 + */
#ifdef ANSI
PUBLIC S16 inNetE39S00
(
InNtc *ntc,
InCb *dCb
)
#else
PUBLIC S16 inNetE39S00(ntc, dCb)
InNtc *ntc;
InCb *dCb;
#endif
{
   RegEvnt   *regEvnt;
   InPCB *pcb;
   PduHdr pduHdr;
   AllPdus allPdus;
   Buffer *mBuf;
   U8 protDisc;
   S16 ret;

   TRC2(inNetE39S00);
   /* get physical link control block */
   if ((pcb = *(pcbLstPtr + dCb->suId)) == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
       INLOGERROR(ERRCLS_DEBUG, EIN449, (ErrVal) dCb->rSuId,
                 "inNetE39S00() failed, unable to access DLSAP.");
#endif
       RETVALUE(RFAILED);
   }

   regEvnt = (RegEvnt *) ntc->sduSp;
   ntc->state = ST_CALLINDEP;
   ntc->checkRegCfm = IN_REG_CFM;
   inUpdSts(&pcb->sts.regTx, pcb);
   /* set protocol discriminator */
   protDisc = PD_Q931; 
   /* prepare Pdu header */
   inInitPduHdr((U8) protDisc, ntc->origin, pcb->cfg.callRefLen, ntc->dCallRef,
                (U8) M_REG, &pduHdr);

   MFINITPDU(&ntc->dCallCb->mfMsgCtl, ret, (U8) PM_REG, (U8) MI_REG, 
             (ElmtHdr *) ntc->sduSp, (ElmtHdr *) &allPdus, (U8) PRSNT_NODEF, 
             pcb->cfg.swtch, ntc->callType);

   inBldMsg(ntc->dCallCb, &pduHdr, &allPdus, ntc->dCallCb->pst.region, 
            ntc->dCallCb->pst.pool, &mBuf, pcb->cfg.swtch, ntc->callType);
  
   if (dCb->state != CBST1XFER)
   {
       INSETTOKVAL(ntc->causeDgn.causeVal, CCDESTOUTORD);
       inGenRelInd(ntc, &ntc->causeDgn);
       inDropData(mBuf);
       RETVALUE(RFAILED);
   }
   inSndMsg(dCb, mBuf);

   zqUpdPeer();   
   RETVALUE(ROK);
} /* end of inNetE39S00 */
/* FID16970 - */

  
/********************************************************************30**
  
         End of file: qn_bdy6.c 3.11  -  09/29/99 14:37:02
  
*********************************************************************31*/


/********************************************************************40**

   Notes:

*********************************************************************41*/

/********************************************************************50**

*********************************************************************51*/


/********************************************************************60**

   Revision history:

*********************************************************************61*/

/********************************************************************70**

  version    initials         description
-----------  ---------   ------------------------------------------------
1.1.0.0         lc       1. initial release.

1.2.0.0         lc       1. add att variant
                         2. text changes

1.3.0.0         lc       1. reorganize to unbundle variants
                         2. text changes

1.4.0.0         lc       1. miscellaneous changes

1.5.0.0         lc       1. add callRefLen to MFINITPDU function call

1.6.0.0         lc       1. change inGenDiscInd to inGenRelInd
                            in inNetE27S00, inNetE27S00 functions
                         2. or MF_USR flag into ntc->callType instead
                            of just setting in inNetE27S00 function
                         3. change ConEvnt typedef to CnStEvnt typdef in
                            inNetE28S01, inNetE28S02 functions
                         4. initialize sdu as connect status request
                            instead of connect response in inNetE28S01
                            function
                         5. set ntc pointer for d and t call references
                            to NULLP in inNetE34S15 function
                         6. change d and b channel assignment in
                            inNetE34S15 function
                         7. zero tCallRef and dCallRef in ntc in
                            inNetE34S15 function

1.7.0.0         lc       1. error codes were changed       

1.8             lc       1. added nt-dms100 variants for basic and primary
                            interfaces 
                         2. added att variant for 5ess switch 
                         3. added vn-3 variant.
                         4. added management/maintanence protocol 
                            for 5ess switch.
                         5. changed call reference and call reference flag
                            allocation.
                         6. added call redirection (forwarding) logic
                         7. added inGetRedirDst function to get redirecting 
                            destination.
                         8. added forward subscription, redirection and
                            calling party number insertion and screening 
                         9. added ie errors in global messages processing. 
            
                        10. added new configuration parameter for non ringing
                            terminal for nt-dms100.
                        11. allow allocation of B and H channels on the same
                            physical interface.
                        12. changed NConInd and NConCfm primitives to pass
                            D channel number.
                        13. added new configuration parameter restart option.
                        14. new error codes added                         
                        15. changes to conform with ATT4E switch
               jrl      16. trillium development system checkpoint (dvs)
                            at version: 1.7.0.0

2.1            lc       1. changed calling convention for upper and lower
                           interfaces primitives.
                        2. conversion to second release of message generating
                           and processing functions universal to Trillium's 
                           implementation of communication protocols.
                        3. added INSNET variant.
                        4. added dChan parametr to CnStInd primitive.
                        5. add ANSI function declarations
                        6. add envopt.h include file
                        7. text changes

2.2            jrl      1. remove MSCDOS

2.3             bn      1. text changes.
 
*********************************************************************71*/

/********************************************************************80**

  version    pat  init                   description
----------- ----- ----  ------------------------------------------------
2.4          ---  jrl   1. text changes

2.5          ---  bn    1. add inNetE29S10.
                        2. added support for nfas.

2.6          ---  rg    1. added etsi variant.
                        2. text changes.
                        3. changed inNetE29S04 to allow Progress & Info
                           messages to be sent, for etsi & ccitt.
                        4. added inNetE19S12, inNetE19SND, inNetE29S07 and 
                           inNetE29S15.
                        5. changed inNetE29S10 to allow Info message to
                           be sent, for etsi & ccitt.

2.7          ---  bn    1. text changes

2.8          ---  bn    1. text changes
             ---  bn    2. change SW_VN3 define to SW_VN2

2.9          ---  bn    1. corrected typo in inNetE30S11.

2.10         ---  bn    1. add include cm5.?

2.11         ---  bn    1. replaced return with RETVALUE.
             ---  bn    2. add nt dms-250.

2.12         ---  bn    1. added inNetE19S00 and inNetE31S00 for Q932.

2.13         ---  bn    1. surrounded the code pertaining to specific options
                           by corresponding ifdef's

2.14         ---  bn    1. changed #ifdef SUSPEND to #if ISDN_SUSPEND.
             ---  bn    2. replaced ss_??.? by ssi.?

3.1          ---  bn    1. Changed interfaces to include Pst structure
                           instead of BndCfg.
             ---  bn    2. changed layer manager interface.
             ---  bn    3. miscellanious error correctios.
             ---  bn    4. added BC303.

3.2          ---  bn    1. corrected parm 3 in inGenPdu in inNetE19S24.

3.3          ---  bn    1. in025.31, in040.31      
             ---  krp   2. Added #ifdef ROUTE around portions of the code
                           used only when Routing is enabled.

3.4          ---  bn    1. misceleneous changes.
*********************************************************************81*/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
3.5          ---      sb   1. Miscellaneous changes.
3.6          ---      sb   1. Modified function inNetE27S00 to issue
                              Broadcast setup if necessary.
             ---      sb   2. Modified functions to taken in the extra 
                              parameter.
3.7          ---      sam  1. Modified function inNetE29S01 to check
                              for correct response from network for
                              Australia Telecom. Also to insert 
                              channel id. if necessary.
             ---      sam  2. Added check for channel id for AUSTEL
                              case in inNetE29SND. Also made changes
                              for the appropriate valid inputs in 
                              ST_OUTPROC state for Austel. Added case
                              MI_CONNECT.
             ---      sam  3. Added check for Info request for AUSTEL
                              in ST_OVLPSND in inNetE29SND.
             ---      sb   4. Modified functions using inRemCallRef to
                              remove upper call references to use
                              cmHashListDelete instead.

3.8          ---      pk   1. Added check to ensure that call proceeding
                              is the only valid connect status message 
                              in the call initiated state for NI2 in 
                              inNetE29S01
             ---      pk   2. Added check for validity of incoming 
                              message for NI2 in inNetE29SND
             ---      pk   3. Added case for NI2 when a progress request
                              is received in function inNetE29SND
             ---      pk   4. Added a check for valid message in case of 
                              NI2 in function inNetE29S04
             ---      pk   5. Added a check for NI2 in Function 
                              inNetE28S01 to verify that connect request 
                              is not received in the Call Initiated state
             ---      pk   6. Added case for NI2 when a progress is 
                              received in the Call delivered state in
                              function inNetE29S04.
             ---      pk   7. Added a check for the switch type in function
                              inNetE28S01 inorder to issue an error for 
                              switch type NI2.
             ---      pk   8. Added case for NI2 when a disconnect request 
                              is received in the outgoing call proceeding 
                              state in function inNetE30S03 
             ---      sb   9. Moved initialization of cnStEvnt in inNetE29SND
                              to the top of function.
             ---      sam  10.Added case for SW_AUSP in  inNetE28S02.
             ---      sam  11.Added case for SW_AUSP (to accept Info
                              mesg.) in inNetE19S02.

3.9          ---      pk   1. Modified parameters for inNetE29SND for non 
                              ANSI case.

3.10         ---      sam  1. Changed causeDgn.eh.pres to have PRSNT_NODEF
                              Most places it is probably redundant, but was
                              retained, incase it is being used later in
             ---      asa  2. Changed to fill up ces and pcb->suId in
                              the last parameter in InUiIntConInd
             ---      asa  3. Fill up Ces value in the last parameter in
                              InUiIntCnStInd and InUiIntConInd prim.
             ---      asa  4. Initialize cause in Ntc before calling
                              inNetE30SND (func : inNetE31S06)
                           5. Correct the parameters passed to function
                              inCallIdSearch
                           6. Initialize ntc->hlInsrt to 0 after
                              deleting Ntc entry from hash list.
                           7. Changed PM_SSHLREQ to PM_SSHLRSP in
                              MFINITPDU call in inNetE34S15
             ---      sam  8. Added variant NTMCI and NTNI for all state
                              matrix processing functions.
             ---      sam  9. Removed #ifdef cm_gen.h and cm_gen.x.
             ---      sb   10.Text changes 

3.11         ---      rsk  1. Change  ntc's call state before issuing
                              a InUiIntRelInd to the upper layer

3.12         in001.310 pk  1. Allowed Channel Negotiation for NTDMS250.
                           2. Changed in inNetE27S00 function due to calling 
                              number IE problem.
             in003.310 bn  3. Added case for NI2 in inNetE30S01
                       pk  4. Addition of QW header files
             in005.310 cy  5. Correction in inNetE30S01 for BC303TMC and 
                              BC303CSC variants. Call function inNetE30SND
                              if the ntc->state is ST_CALLINIT.
                           6. Set protocol discriminator to PD_BC303 (0x4f) 
                              in inNetE30S01 for BC303TMC and BC303CSC 
                              variants.
                           7. Changed inNetE31S06 to release the call 
                              completely for BC303TMC variant when a RelReq 
                              from call control was received in state 
                              ST_CALLPRES.
             in007.310 cy  8. Added correction for counter pcb->sts.aveCalls.
             in009.310 cy  9. Checked ntc->updStatus before calling
                              funciton zqRunTimeUpd for update/create ntc.
                       cy 10. Removed initialization of ntc->cmpltAdr to 
                              TRUE in inNetE29S01 in case of event 
                              MI_SETUPACK.
             in010.310 cy 11. Fix for ZQ. Set ntc->updStatus flag to 
                              NTC_CREATED after sending create request  
                              to the standby.
                       bn 12. Added generation of release indication from 
                              inNetE27S00 in case of PRI when data link 
                              layer is down.
             in011.310 pk 13. Removed Check for ETSI while checking for
                              PTMPT.
                       bb 14. Changed to send a RELEASE COMPLETE message
                              instead of a RELEASE message in case of 
                              channel negotiation fails for an incoming 
                              SETUP message.
             in014.310 cy 15. Corrected compile time flag for INLOGERR.
             in018.310 cy 16. Fix for ZQ. Checked return value from 
                              zqRunTimeUpd before setting ntc->updStatus  
                              to NTC_CREATED.
             in021.310 cy 17. Changed a typing error in inNetE30S01.
             in029.310 yz 18. Corrected reported ErrVal to avoid wrong report.
             in029.310 scc 19. Remove define for ISDN_PRI as it should be 
                               defined in envopt.h
             in036.310 bb 20. Added changes for CORE2 support
                              (TELICA:won't work, removed).
*********************************************************************91*/









