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
  
        Name:     Call Control
    
        Type:     C include file
 
        Desc:     Defines required by the Call Control.
 
        File:     cc.h

        Sid:      cc.h 1.5  -  12/21/99 09:49:59
  
        Prg:      rs
  
*********************************************************************21*/
 
  
#ifndef __CCH__
#define __CCH__

#define SAVE_PRFLID

/* TELICA:spamidi:06/05/2001:included as part of bug 4523 */
#include <SoftSwitchDBApi.h>
#include <mgi_congestion.h>

#if 1 /*FGD*/
/* 
 * Bit Map values for Far/Own/Class of service
 * Bits 0,1,2 mapped to Far end class
 * Bits 3,4 mapped to Own class
 * Bits 5,6,7 mapped to Class of service
 */
#define TGP_FAREND_AT            0x00   /* Far End Class = AT */
#define TGP_FAREND_EO            0x01   /* Far End Class = EO */
#define TGP_FAREND_INC           0x02   /* Far End Class = INC */
#define TGP_FAREND_BLV           0x03   /* Far End Class = BLV */
#define TGP_FAREND_IXC           0x04   /* Far End Class = IXC */

#define TGP_NEAREND_EO           0x08   /* Near End = EO */
#define TGP_NEAREND_AT           0x10   /* Near End = AT */
#define TGP_NEAREND_IXC          0x18   /* Near End = IXC */

#define TGP_COS_911              0x20   /* Class Of Svc = 911 */
#define TGP_COS_AIS              0x40   /* Class Of Svc = AIS */
#define TGP_COS_BLV              0x60   /* Class Of Svc = BLV */
#define TGP_COS_FGD              0x80   /* Class Of Svc = FGD */
#define TGP_COS_MDP              0xA0   /* Class Of Svc = MDP */
#define TGP_COS_NULL             0xC0   /* Class Of Svc = NULL */
#define TGP_COS_WTNDM            0xE0   /* Class Of Svc = WTNDM */

#if 1  /* bug: 19674 */
#define TGP_DIR_OUT              0x100
#define TGP_DIR_IN               0x200
#define TGP_DIR_BOTH             0x400

#define MAP_TGP_DIR(con, tgAtt) \
{ \
   switch(tgAtt & 0xF00) \
   { \
      case TGP_DIR_OUT: \
         con = TGP_DIR_OUTGOING; \
         break; \
      case TGP_DIR_IN: \
         con = TGP_DIR_INCOMING; \
         break; \
      case TGP_DIR_BOTH: \
         con = TGP_DIR_BOTHWAY; \
         break; \
      default: \
         con = TGP_DIR_OUTGOING; \
         break; \
   } \
}
#endif /* bug: 19674 */


#define MAP_TGP_COS(con, tgAtt) \
{ \
   switch(tgAtt & 0xE0) \
   { \
      case TGP_COS_911: \
         con = CoS_911; \
         break; \
      case TGP_COS_AIS: \
         con = CoS_AIS; \
         break; \
      case TGP_COS_BLV: \
         con = CoS_BLV; \
         break; \
      case TGP_COS_FGD: \
         con = CoS_FGD; \
         break; \
      case TGP_COS_MDP: \
         con = Cos_MDP; \
         break; \
      case TGP_COS_WTNDM: \
         con = CoS_WTNDM; \
         break; \
      case TGP_COS_NULL: \
         con = CoS_NULL; \
         break; \
      default: \
         con = CoS_NULL; \
         break; \
   } \
}

#define MAP_TGP_FEC(con, tgAtt) \
{ \
   switch(tgAtt & 0x07) \
   { \
      case TGP_FAREND_AT: \
         con = TGP_CLASS_AT; \
         break; \
      case TGP_FAREND_EO: \
         con = TGP_CLASS_EO; \
         break; \
      case TGP_FAREND_INC: \
         con = TGP_CLASS_INC; \
         break; \
      case TGP_FAREND_BLV: \
         con = TGP_CLASS_BLV; \
         break; \
      case TGP_FAREND_IXC: \
         con = TGP_CLASS_IXC; \
         break; \
      default: \
         con = TGP_CLASS_AT; \
         break; \
   } \
}

#define MAP_TGP_OWN(con, tgAtt) \
{ \
   switch(tgAtt & 0x18) \
   { \
      case TGP_NEAREND_EO: \
         con = TGP_OWN_CLASS_EO; \
         break; \
      case TGP_NEAREND_AT: \
         con = TGP_OWN_CLASS_AT; \
         break; \
      case TGP_NEAREND_IXC: \
         con = TGP_OWN_CLASS_IXC; \
         break; \
      default: \
         con = TGP_OWN_CLASS_EO; \
         break; \
   } \
}

#endif /*FGD*/

#if 1 /*BUG 20443*/
#define MAP_3_DGT_CARR_ID(carrId, tns) \
{ \
   carrId[0] = tns[0] & 0x0f; \
   carrId[1] = (tns[0] >> 4) & 0x0f; \
   carrId[2] = tns[1] & 0x0f; \
   carrId[3] = 0; \
}

#define MAP_4_DGT_CARR_ID(carrId, tns) \
{ \
   carrId[0] = tns[0] & 0x0f; \
   carrId[1] = (tns[0] >> 4) & 0x0f; \
   carrId[2] = tns[1] & 0x0f; \
   carrId[3] = (tns[1] >> 4) & 0x0f; \
}

#define MAP_TNS_TO_BCD(tns, carr, ozz) \
{ \
   tns[0] = carr[0] | (carr[1] << 4); \
   tns[1] = carr[2] | (carr[3] << 4); \
   tns[2] = ozz[0] | (ozz[1] << 4); \
   tns[3] = ozz[2] | 0; \
}

#define MAP_0ZZ_DEC(tns, ozz) \
{ \
   ozz[0] = (tns / 100); \
   ozz[1] = ((tns % 100) / 10); \
   ozz[2] = ((tns % 100) % 10); \
}
#endif /*BUG 20443*/

#if 1 /*BUG:20251*/
#define MAP_TRKGRP(u16Val, strVal) \
{ \
   strVal[0] = (u16Val / 1000) | (((u16Val % 1000) / 100) << 4); \
   strVal[1] = ((u16Val % 1000) % 100) / 10 | ((((u16Val % 1000) % 100) % 10) << 4); \
}
#endif

#define CAS_INTL_CKT_CODE       138
#define CAS_INTL_OP_CKT_CODE    158

#define IW_MF_SS7_SUSP_RECD     0x01 /* SS7 SUSP message recd */
#define IW_MF_SS7_RESU_RECD     0x02 /* SS7 RESUME message recd */
#define isSuspRecd(con) (con->iwMfSs7_SuspResuRecd & IW_MF_SS7_SUSP_RECD)

/* Criteria for observation trace */
#define NUM_IC_CRITERIA  3        /* Criteria based on incoming parameters */
#define NUM_OG_CRITERIA  3        /* Criteria based on outgoing parameters */

/* Used to specify any action with a leg of the connection */
#define CC_INCOMING      0        /* Incoming leg of the connection */
#define CC_OUTGOING      1        /* Outgoing leg of the connection */
#define CC_BOTH          2        /* Both leg of the connection */
#if 0 /* CC_ALL will not be used anymore #32748 */
#define CC_ALL           3        /* Used in Deallocation. Specifies to free
                                   * up any resources allocated for the 
                                   * connection */
#endif /* 0 CC_ALL will not be used anymore #32748 */
#ifdef GUARD_TIMER /* aambika - Bugzilla 3187 - Guard Timer */
#define CC_INCOMING_GUARD      4        /* guard Incoming leg of the connection */
#define CC_OUTGOING_GUARD      5        /* guard Outgoing leg of the connection */
#endif

/* FID 2001 */
#define CC_IC_COT_ATTEMPT      1  /* Incoming COT Attempt */
#define CC_OG_COT_ATTEMPT      2  /* Outgoing COT Attempt */
#define CC_IC_COT_SUCCESS      3  /* Incoming COT Success */
#define CC_OG_COT_SUCCESS      4  /* Outgoing COT Success */
#define CC_IC_COT_FAIL         5  /* Incoming COT Failure */
#define CC_OG_COT_FAIL         6  /* Ourgoing COT Failure */

#define CC_TONE_SETUP_ATTEMPT  1  /* Tone Setup Attempt   */
#define CC_TONE_SETUP_SUCCESS  2  /* Tone Setup Success   */
#define CC_ANNC_SETUP_ATTEMPT  3  /* Announcement Setup Attempt */
#define CC_ANNC_SETUP_SUCCESS  4  /* Announcement Setup Success */

#define CC_ACTION_INCREMENT    1  /* Increment */
#define CC_ACTION_DECREASE     2  /* Decrease  */

/* Internal Allocation Status used to determine the allocation need for
 * the call
 */
#define RIC_RES_ALOC                  23 /* Incoming Resource is allocated */
#define ROG_RES_ALOC                  24 /* Outgoing Resource is allocated */
#define RIC_RES_DEALOC_OG_PENDING     25 /* Incoming Resource is Deallocated 
                                          * Outgoing dealocation is pending
                                          */
#define ROG_RES_DEALOC_IC_PENDING     26 /* Outgoing Resource is Deallocated 
                                          * Incoming dealocation is pending
                                          */
#define RIC_RES_DEALOC                27 /* Incoming Resource is Deallocated */
#define ROG_RES_DEALOC                28 /* Outgoing Resource is Deallocated */

/* Allocation type value for internal use */
#define RMT_DONTCARE  RMT_INVALID    /* Allocation Type for internal use */

/* Internal Events State Machine: Events */
#define CCMAXINTEVENTS      2        /* Maximum number of Connection Events */
#define CCE_INTOGALOCREQ    0        /* Resource allocation request for outgoing
                                      * side 
                                      */
#define CCE_INTSWTCONREQ    1        /* Switch connect request */


/* Observation Trace Event Category */
#define CC_STATE_TRAN                 1  /* State Transition Event */
#define CC_STATE_EVNT                 2  /* Event to/from one of the entities
                                          * like RT, RM, SF, PSIF, TIMER
                                          */
/* flag used in CcConCb  - Generic purpose flag to indicate
 * the status of call 
 *
 * new field flag ( used as BITMASK )defined in Connnection block
 */
#define CC_FLAG_HOLDSENT 0x01
#define CC_FLAG_SENDCPGINUPDATE 0x02 /* FID-2112: Bug#55567 */
#define CC_FLAG_HOLDINFOSAVED 0x04 /* FID 7803 */
#define CC_FLAG_TRANSCODING 0x08  /* FID-2593 */
#define CC_FLAG_CLEARMODE 0x10  /* FID-2593 */
#define CC_FLAG_EARLYMEDIA_IW 0x20  /* FID-2575 */
#define CC_FLAG_CCCONCBREP 0x40  /* FID-7908 */
#define CC_FLAG_IW_GAP 0x80  /* FID-14209 */
#define CC_FLAG_NAT    0x0100  /* FID-14732 */
#define CC_FLAG_BTS_REALM 0x200 /* FID 14732 */
#define CC_FLAG_IN_DIALOG 0x400 /* FID 15174.1 */
#define CC_FLAG_IC_VBD_NEGOTIATED 0x800 /* FID 14844 */
#define CC_FLAG_OG_VBD_NEGOTIATED 0x1000 /* FID 14844 */
#define CC_FLAG_CANCEL 0x2000 /* FID 15045.1 */
#define CC_FLAG_RELAY_MODE 0x4000 /* FID 15045.1 */
#define CC_FLAG_IN_DIALOG_STOP 0x8000 /* FID 15174.0 */
#define CC_FLAG_SUCONNID_SENT4OL 0x10000 /* FID 15174.0 */
#define CC_FLAG_484_RCVD 0x20000 /* FID 15174.0 */
#define CC_FLAG_DEFER_ANS_SDP_IC  0x40000 /* FID 14844.1 */
#define CC_FLAG_IPV_CONVERSION 0x80000 /* FID 14341.0 -IPV6 */
#define CC_FLAG_ICA            0x100000 /* FID 15310.1 - ICA calls*/
#define CC_FLAG_NEED_PSEUDO_VBD  0x200000 /* FID 15913.0 */
#define CC_FLAG_AGNOSTIC_NO_OFFER_CANDIDATE 0x400000 /* FID 16118.0 BUG: 86355 */
#define CC_FLAG_CALLQUEUED_DELAY 0x800000  /* FID 14457.0 */
#define CC_FLAG_CALLQUEUED       0x1000000 /* FID 14457.0 */
#define CC_FLAG_IP_CALL_NO_INITIAL_SDP 0x2000000 /* BUG:86958 for FID 16118.0 */
#define CC_FLAG_IC_HOLD_BY_ZERO_IP 0x4000000 /* FID 14841.0 */
#define CC_FLAG_OG_HOLD_BY_ZERO_IP 0x8000000 /* FID 14841.0 */
#define CC_FLAG_SENT_INTERNAL_DELAY 0x10000000 /* BUG: 88025 */
#define CC_FLAG_HPCQ_AR             0x20000000 /* BUG: 88130 */
#define CC_FLAG_IBCF_CALL           0x40000000 /* FID 15461.0 */
#define CC_FLAG_GROVRFLW_SKIP       0x80000000 /* FID 14716.0 */

/* new field flag2 (used as BITMASK) defined in Connection block */
#define SET_SCR_ALARM_FLAG          0x01       /* FID16259.0 */
#define CC_SCR_ALARM_GENERATED      0x02       /* FID16259.0 */
#define CC_FRAUDCALL_EXPIRED        0x04       /* FID16259.0 */
#define CC_RMTDESC_BUFFERED         0x08       /* BUG90380  */
#define CC_MARK_OVERLOAD_IND        0x10       /* FID16784.0 */
#define CC_FLAG_VBDFALLBACKNEEDED   0x20       /* FID16883.0 */
#define CC_FLAG_L3CRI_RECV_BICC     0x40       /* BUG: 91793 */
#define CC_FLAG_SAVE_L3CRI          0x80       /* BUG: 91793 */
#define CC_FLAG_FREESDPNEEDED       0x100      /* BUG: 91848 */
#define CC_FLAG_PRECONDITION        0x200      /* FID15068.0 */
#define CC_FLAG_CRBT                0x400      /* FID15068.0 */
#define CC_FLAG_TRANSIENT_MG        0x800      /* FID16692.0 */
#define CC_FLAG_SUPPORTNSS          0x1000     /* FID16690.0 */
#define CC_FLAG_ACTTCPMERGE         0x2000     /* FID15682.0 */
#define CC_FLAG_RELATCH             0x4000     /* FID15682.0 */
#define CC_FLAG_ICS_REL             0x8000     /* FID17378.0 */
#define CC_FLAG_ICS_MODIFY          0x10000    /* FID17378.0 */
#define CC_FLAG_ICS_RETRIEVE        0x20000    /* FID17378.0 */
#define CC_FLAG_NOT_ALLOW_SRTP_MS1  0x40000    /* FID17066.0 */
#define CC_FLAG_NOT_ALLOW_SRTP_MS2  0x80000    /* FID17066.0 */
#define CC_FLAG_NOT_ALLOW_SRTP_MS3  0x100000   /* FID17066.0 */
#define CC_FLAG_NOT_ALLOW_SRTP_MS4  0x200000   /* FID17066.0 */


/* holdFlag used in CcConCb  - flag to indicate either HOLD/XFER event
 */
#define CC_EVENT_HOLD   1
#define CC_EVENT_XFER   2

/* FID 14972.0 & FID 15261.0 */
#define CC_PASSTHRU_APPL_LOC_SDP    0x01  /* application provides LocSdp */
#define CC_PASSTHRU_IC_SDP_DONE     0x02  /* incoming reorder done due to CAC */
#define CC_PASSTHRU_DONE_ON_FOFFER  0x04  /* PassThru done for Filtered Offer */
#define CC_PASSTHRU_DONE_ON_NFOFFER 0x08  /* PassThru done for No Filter Offer */

#define CC_CLR_PASSTHRU_FLAG(con,flag)                     \
  (con->passThruInfo) &= ~flag
#define CC_SET_PASSTHRU_FLAG(con,flag)                     \
  (con->passThruInfo) |= flag
#define CC_IS_PASSTHRU_FLAG_SET(con,flag)                  \
  ((con->passThruInfo) & flag)

#define CC_GET_PASSTHRU_PTIME(con, bcmType)     (((bcmType)==BtOBcm)?((con)->icPassThruPtime): \
                                                                     ((con)->ogPassThruPtime))
#define CC_SET_PASSTHRU_PTIME(con, bcmType, ptime)      \
                                            ({if((bcmType)==BtOBcm) \
                                               (con)->icPassThruPtime = (ptime); \
                                              else \
                                               (con)->ogPassThruPtime = (ptime);})



/* Type of Release Pending */
#define INRELPEND   1     /* Incoming Release Pending */
#define OGRELPEND   2     /* Outgoing Release Pending */
#define BOTHRELPEND 3     /* Both Release Pending */
#define LMRELPEND   4     /* LM initiated Release Pending */
#define INITRELPEND 5     /* INITREL Pending */
#define ICRELONLY   6     /* Release IC leg of call */
#define BOTHREL     7     /* Release IC and OG side */
#define OGRELONLY          8     /* Release OG leg of call */
#define INRELRSPANDOUTREL  9     /* Release OG after Switching Release */
#define OUTRELRSPANDINREL  10    /* Release IC after Switching Release */
#define INRELRSPANDNOREL   11    /* Release Rsp to IC No relReq to OG */
#define OUTRELRSPANDNOREL  12    /* Release Rsp to OG No relReq to IC ?? */
#define INRELRSPANDOUTRELRSP 13  /* Release Rsp to OG and to IC */
#define OUTRELRSPANDINTONE 14    /* Release Rsp to ISUP and Play tone to CAS */
#define NORELANDNORELRSP   15    /* No Release sent on any side, cleanup the con */
#define ICRELONLYKEEPCON   16    /* Send IC Release and dont clean Con */
#define OGRELONLYKEEPCON   17    /* Send OG Release and dont clean Con */
#define OUTRELRSPISDNANDINTONE 18/* Used for cas-isdn call for tone playing */
#define OUTRELRSPANDROUTE      19/* Used for Analyze Route                  */
#define OUTRELRSPANDPLAYANNC   20/* Used for SendToResource                 */
#define OUTRELANDROUTE         21/* Used for Analyze Route                  */
#define OUTRELANDPLAYANNC      22/* Used for SendToResource                 */
#define OUTRELANDINFAILCOND    23/* Send Rel to Og and Exception Ind to IC  */
#define OUTRELRSPANDINFAILCOND 24 /* Send RelRsp to Og and Exception to IC  */
/* CPDI */
#define PALYINGANNC 25 /* Used for sending MGI TnxReq after receiving ConnectToResource */
#define PALYEDANNC  26 /* Used for receiving MGI TnxCfm after TnxReq */
#define OUTRELANDANSINFAILCOND 27 /* Send Rel to Og and Exception Ind to IC due to Ans Fraud */
#define OUTRELRSPANDANSINFAILCOND 28 /* Send RelRsp to Og and Exception Ind to IC due to Ans Fraud */
#define OUTRELANDSCRINFAILCOND 29 /* Send Rel to Og and Exception Ind to IC due to SCR Fraud */
#define OUTRELRSPANDSCRINFAILCOND 30 /* Send RelRsp to Og and Exception Ind to IC due to SCR Fraud */
#define NORELANDPLAYANNC       31 /* Used for SendToResource                 */
#define TRY_SUBADD_ON_ERR_IC   32 /* 24322 Try to do SUB and ADD incase Modify fails */
#define TRY_SUBADD_ON_ERR_OG   33 /* 24322 Try to do SUB and ADD incase Modify fails */
#define NORELANDINFAILCOND     34 /* Used after E11S28 */
#define INTONEFORSUBANDRELFORTRK 35 /* Context is present case - Bug# 30944 */
/* euysal - 05/26/2005 - ACC Support */
#define ABANDONEDANNC_IC       36 /* Received a release indication during announcement from incoming side */
#define ABANDONEDANNC_OG       37 /* Received a release indication during announcement from outgoing side */
/* euysal - 03/29/2005 - ACC Support */
#define PLAYINGANNCFORCONNECTTORSC 38 /* Used for sending announcement for ACC Support */
/* Start SIP Refer/subscribe */
#define IC_RLT_AFTER_MGIT_CFM       39 /* RLT/REFER: IC -> OG */
#define IC_RMT_XFER_AFTER_MGIT_CFM  40 /* Rmt Xfer:Ingress initiated */
#if 0
#define SUBSCRIBE_RCVD_FROM_OG 41 /* Support for SUBSCRIBE from Egress */
#define SUBSCRIBE_RCVD_FROM_IC 42 /* Support for SUBSCRIBE from Ingress */
#define SUBSCRIBE_EXPIRED_IC   43 /* Support for SUBSCRIBE */
#define SUBSCRIBE_EXPIRED_OG   44 /* Support for SUBSCRIBE */
#endif
#define SEND2RSC_RCVD_FROM_IC  45 /* Support for Send2Rsc coming from Layer-3 */
#define OG_RLT_AFTER_MGIT_CFM       46 /* RLT/REFER: OG -> IC */
#define OG_RMT_XFER_AFTER_MGIT_CFM  47 /* Rmt Xfer:Egress initiated */
#define NORELCONTINUE_ANNC     48 /* Support for SIP/BICC Annc      */
#define INTONE                 49 /* Support for SI/BICC Annc       */
#define IC_RLT_FAILURE         50 /* RLT/REFER failed, trying ReRouting */
#define OG_RLT_FAILURE         51 /* RLT/REFER failed, trying ReRouting */
#define IC_AWT_RLT             52 /* Waiting for FAA/FRJ/Notify from Ingress */
#define OG_AWT_RLT             53 /* Waiting for FAA/FRJ/Notify from Egress */
#define OUTRELRSPANDROUTEFORMGADVANCE                                         \
                               54 /* FID 14732.0: For MGW reroute after MG rsp*/
#define CONTINUEWITHCALLWITHOUTLI                                             \
                               55 /* FID 15083.0 */
/* End SIP Refer/subscribe */
#define ONLYINFAILCND          56 /* FID 15261.0 BUG:80926 */
#define CONTINUEWITHREATTEMPT  57 /* FID 15083.0 BUG:83045 */
#define CONTINUEWITHREATTEMPT1 58 /* FID 15083.0 BUG:83045 */
#define OUTRELRSPANDADVANCEURI 59 /* FID 15801.0 LGP-74 */
#define OUTRELANDADVANCEURI    60 /* FID 15801.0 LGP-74 */
#define OUTRELRSPANDROUTEFORICMGADVANCE                                       \
                               61 /* FID 14457.0: For Ingress MG Advance reroute after MG rsp*/
#define OUTRELHPCMGRETRY       62 /* FID 14457.0: MG retry for single MG on list or Local MG */
/*BUG:90302 */
#define OUTRSCSUBANDCLR        63 /* BUG:90302 OgRsc needs to be cleaned   */
#define OUTRSCADDANDALOC       64 /* BUG:90302 new OgRsc needs to be alloc */
#define INRELRSPDELAYANDOUTREL 65 /* FID 16706 BUG 91702 */
#define CC_SIP_ID_LENGTH_OLD   32
#define CC_SIP_ID_LENGTH       128 /* FID 14485.0 :128 Char SIP Call-Id */
/* euysal - 06/01/2005 - ACC Support */

#define CC_RAND1_LEGNTH  64  /* FID 15189.0 */
#define CC_RAND2_LEGNTH  31  /* FID 17044.0 for short callID*/

#define CC_CAC_NAME_LENGTH 20

#define ANNC_NONE   0x00
#define ANNC_IC     0x01
#define ANNC_OG     0x02
#define ANNC_BOTH   0x03

#ifdef CC_CALEA
/* #define values for the field con->mgiPendAddReq */
#define ORGSUBADDREQ           0x00000001
#define ORGSUBCALEALEG1ADDREQ  0x00000002
#define ORGSUBCALEALEG2ADDREQ  0x00000004
#define ORGSUBCALEALEG3ADDREQ  0x00000008
#define ORGSUBCALEALEG4ADDREQ  0x00000010
#define ORGSUBCALEALEG5ADDREQ  0x00000020
#define TERMSUBADDREQ          0x00000040
#define TERMSUBCALEALEG1ADDREQ 0x00000080
#define TERMSUBCALEALEG2ADDREQ 0x00000100
#define TERMSUBCALEALEG3ADDREQ 0x00000200
#define TERMSUBCALEALEG4ADDREQ 0x00000400
#define TERMSUBCALEALEG5ADDREQ 0x00000800
#define SURROGATE1ADDREQ       0x00001000 
#define SURROGATE2ADDREQ       0x00002000 
#define SURROGATE3ADDREQ       0x00004000 
#define SURROGATE4ADDREQ       0x00008000 
#define SURROGATE5ADDREQ       0x00010000 

/* #define value for the field con->mgiPendAddRsp */
#define ORGSUBADDRSP           0x00000001
#define ORGSUBCALEALEG1ADDRSP  0x00000002
#define ORGSUBCALEALEG2ADDRSP  0x00000004
#define ORGSUBCALEALEG3ADDRSP  0x00000008
#define ORGSUBCALEALEG4ADDRSP  0x00000010
#define ORGSUBCALEALEG5ADDRSP  0x00000020
#define TERMSUBADDRSP          0x00000040  
#define TERMSUBCALEALEG1ADDRSP 0x00000080
#define TERMSUBCALEALEG2ADDRSP 0x00000100
#define TERMSUBCALEALEG3ADDRSP 0x00000200
#define TERMSUBCALEALEG4ADDRSP 0x00000400
#define TERMSUBCALEALEG5ADDRSP 0x00000800
#define SURROGATE1ADDRSP       0x00001000 
#define SURROGATE2ADDRSP       0x00002000 
#define SURROGATE3ADDRSP       0x00004000 
#define SURROGATE4ADDRSP       0x00008000 
#define SURROGATE5ADDRSP       0x00010000 

/* #define value for the field con->surrogate->mgiPendAddReq */
#define SURRCALEALEG1ADDREQ    0x01
#define SURRCALEALEG2ADDREQ    0x02
#define SURRCALEALEG3ADDREQ    0x04
#define SURRCALEALEG4ADDREQ    0x08
#define SURRCALEALEG5ADDREQ    0x10

/* #define value for the field con->surrogate->mgiPendAddRsp */
#define SURRCALEALEG1ADDRSP    0x01
#define SURRCALEALEG2ADDRSP    0x02
#define SURRCALEALEG3ADDRSP    0x04
#define SURRCALEALEG4ADDRSP    0x08
#define SURRCALEALEG5ADDRSP    0x10

/* #define values for the field con->mgiPendRelReq */
#define ORGSUBRELREQ           0x00000001
#define ORGSUBCALEALEG1RELREQ  0x00000002
#define ORGSUBCALEALEG2RELREQ  0x00000004
#define ORGSUBCALEALEG3RELREQ  0x00000008
#define ORGSUBCALEALEG4RELREQ  0x00000010
#define ORGSUBCALEALEG5RELREQ  0x00000020
#define TERMSUBRELREQ          0x00000040
#define TERMSUBCALEALEG1RELREQ 0x00000080
#define TERMSUBCALEALEG2RELREQ 0x00000100
#define TERMSUBCALEALEG3RELREQ 0x00000200
#define TERMSUBCALEALEG4RELREQ 0x00000400
#define TERMSUBCALEALEG5RELREQ 0x00000800
#define SURROGATE1RELREQ       0x00001000 
#define SURROGATE2RELREQ       0x00002000 
#define SURROGATE3RELREQ       0x00004000 
#define SURROGATE4RELREQ       0x00008000 
#define SURROGATE5RELREQ       0x00010000 

/* #define value for the field con->mgiPendRelRsp */
#define ORGSUBRELRSP           0x00000001
#define ORGSUBCALEALEG1RELRSP  0x00000002
#define ORGSUBCALEALEG2RELRSP  0x00000004
#define ORGSUBCALEALEG3RELRSP  0x00000008
#define ORGSUBCALEALEG4RELRSP  0x00000010
#define ORGSUBCALEALEG5RELRSP  0x00000020
#define TERMSUBRELRSP          0x00000040  
#define TERMSUBCALEALEG1RELRSP 0x00000080
#define TERMSUBCALEALEG2RELRSP 0x00000100
#define TERMSUBCALEALEG3RELRSP 0x00000200
#define TERMSUBCALEALEG4RELRSP 0x00000400
#define TERMSUBCALEALEG5RELRSP 0x00000800
#define SURROGATE1RELRSP       0x00001000 
#define SURROGATE2RELRSP       0x00002000 
#define SURROGATE3RELRSP       0x00004000 
#define SURROGATE4RELRSP       0x00008000 
#define SURROGATE5RELRSP       0x00010000 

/* #define value for the field con->surrogate->mgiPendRelReq */
#define SURRCALEALEG1RELREQ    0x01
#define SURRCALEALEG2RELREQ    0x02
#define SURRCALEALEG3RELREQ    0x04
#define SURRCALEALEG4RELREQ    0x08
#define SURRCALEALEG5RELREQ    0x10

/* #define value for the field con->surrogate->mgiPendRelRsp */
#define SURRCALEALEG1RELRSP    0x01
#define SURRCALEALEG2RELRSP    0x02
#define SURRCALEALEG3RELRSP    0x04
#define SURRCALEALEG4RELRSP    0x08
#define SURRCALEALEG5RELRSP    0x10

/* #define for indexing surrogate */
#define SURROGATE1             0x01 
#define SURROGATE2             0x02 
#define SURROGATE3             0x03 
#define SURROGATE4             0x04 
#define SURROGATE5             0x05 

#endif

/* COT Number of wire types */
#define TWOWIRE     2     /* Two Wire COT  */
#define FOURWIRE    4     /* Four Wire COT */

#define ICRETRIEVE  1     /* Incoming Retrieve in progress */
#define OGRETRIEVE  2     /* Incoming Retrieve in progress */
/* Connection timer events */

/* CHECK FOR VACANT Timer re-use before adding new timer values */
#define TMR_INTERDGT    0x01          /* timer waiting for Inter Digit */
#define TMR_SETUP       0x02          /* Connection Setup timer */
#define TMR_RELEASE     0x03          /* Connection Release timer */
#define TMR_PSSAPBNDCFM 0x04         /* PSAP Bind Confirm timer */
#define TMR_RMSAPBNDCFM 0x05         /* RMSAP Bind Confirm timer */
#define TMR_RTSAPBNDCFM 0x06         /* RTSAP Bind Confirm timer */
#define TMR_SFSAPBNDCFM 0x07         /* SFSAP Bind Confirm timer */
#define CC_TMR_CALLDTL  0x08         /* Call Detail record timer */
#define CC_TMR_REINVITE 0x09         /* Tmr started when non-owner of CallId */
#define CC_TMR_VACANT1  0x0a
#define CC_TMR_VACANT2  0x0b
#define CC_TMR_BUFFER   0x0c
#define TMR_RM_SFAUDCFM 0x0d
#define TMR_RMAUDRSP    0x0e
#define CC_TMR_CALLDTL_STATS  0x0f   /* Stats Call Detail record timer */

#define CC_TMR_SUSP    0x10
#define CC_TMR_TMDREL  0x11

#define AIN_T_NO_ANS_TMR 0x12
#define AIN_O_NO_ANS_TMR 0x13
#define TMR_BUSY_TONE    0x14
#define TMR_RECONNECT    0x15
#ifdef CC_CALEA
#define CC_CALEA_LEG_TMR 0x16 
#endif
#define TMR_NOTIFYCC     0x17
#define CC_CODEC_NEG_TMR 0x18 /* For Bearer state m/c: Codec Negotiation */
#define TMR_VACANT0      0x19 /* EMPTY                                   */
#define SCR_FRAUD_TMR    0x1a /* Timer for SCREEN digit Fraud Call */ 
#define ANS_FRAUD_TMR    0x1b /* Timer for Answer Supervision Fraud Call */
#define CC_TMR_TIMEOUT   0x1c /* timeout timer */
#define CC_PAUSETIMER    0x1d /* Used for Annoucements */

/* International Development: Overlap Support */
#define CC_TMR_MINDIGIT   0x1e /* mindigit interdigit timeout timer */
#define CC_TMR_MAXDIGIT   0x1f /* maxdigit interdigit timeout timer */
/* International Development: Awaiting Answer Timer Support */
#define CC_TMR_AWAITANS   0x20 /* Await Answer timeout timer */
#define CC_TMR_EARLYACM   0x21 /* Early ACM timer */ 

/* Feature 1789 - German ISUP: Toiw2 support*/
#define CC_TMR_TOIW2      0x22 /* ISUP-SIP interworking timer: Toiw2 */

/* End of German ISUP Definition */

/* FID 15174.0 */
#define CC_TMR_TOIW3      0x23 /* ISUP-SIP interworking timer: Toiw3 */

/* Feature 1839 - compatibility */
#define CC_TMR_AWAIT_COMPAT_ACK 0x24 /* wait for compatibility ACK timer */

/* CRBT */
#define CC_TMR_PREANM 0x25 /* Pre ANM timer */

/* FID-2112: SIP UPDATE */
#define CC_TMR_CPG_RETRY 0x26 /* CPG Retry timer */

/* FID 1915 - Test Line */
#define CC_TMR_TST_LN_DSC  0x27 /* Test line disconnect timer */

/* FID14457.0 GETS HPC MG Retry Timer */
#define CC_TMR_HPC_MG_RETRY  0x28 /* HPC MG Retry Timer */
/* FID16206.0 Call Independent Timer of CFx*/
#define CC_TMR_CFXCALLINDP    0x29 /* CFx Call Independent Timer*/

/* 100 mSec Connection timer events */
#define CC_TST_100_SLNC_TMR    0x1  /* Test Line 100 silence 300 msec timer */
#define CC_TST_102_SLNC1_TMR   0x2  /* Test Line 102 silence 300 msec timer */
#define CC_TST_102_SLNC2_TMR   0x3  /* Test Line 102 silence 1 sec timer */
#define CC_TMR_TST_LN_FAILOVER 0x4  /* Test line disconnect timer for failover */

/* Test Line defines */
#define CC_TST_100_SILENCE       0x1  /* Line Test 100 silence */
#define CC_TST_100_TONE          0x2  /* Line Test 100 tone */
#define CC_TST_102_SILENCE       0x3  /* Line Test 102 silence */
#define CC_TST_102_TONE          0x4  /* Line Test 102 tone */
#define CC_TST_108_LOOPBACK      0x5  /* Line Test 108 loopback */

#define CC_MAX_TSTLN_FO_TMR_EXP 10 /* Max Test line failover disconnect timer expiries */

/* International Development: Simple Echo Control support */
#define CC_FORCE_ECHO        0x0  /* apply echo as per plexus Rsc */
#define CC_PLEXUS_ALT        0x1  /* Plexus echo control procedure */
#define CC_SIMPLE_ECHO_CNTRL 0x2  /* simple echo control procedure */
#define CC_ENH_ECHO_CNTRL_TYPE1 0x3 /* Enhanced echo control type1 */
#define CC_ENH_ECHO_CNTRL_TYPE2 0x4 /* Enhanced echo control type2 */


#define CC_MAX_RETRY    10           /* Maximum Bind Request Retries */
#define CC_PERCENT_TRANSIENT_CALL  2 /* Percent call in transient phase */
#define CC_MAX_MSG_QUEUED_PERCALL  4 /* Maximum messages required to be 
                                      * queued per call 
                                      */
/**************************
/**************************
 Some bit defines for mapping the progress Descriptors for ease of use */
#define CC_PROG_DESC_1      0x01    /* 1 Call is not end 2 end ISDN */
#define CC_PROG_DESC_2      0x02    /* 2 Called eqpt is not ISDN    */
#define CC_PROG_DESC_3      0x03    /* 3 CAlling eqpt is not ISDN   */
#define CC_PROG_DESC_4      0x04    /* 4 call has returned to isdn */ 
#define CC_PROG_DESC_8      0x08    /* 8 Inband info or appropriate pattern now avail */
#define CC_PROG_DESC_10     0x10    /* 10 Delay in response at called interface. national coding standard only */

#define MAP2PROG_DESC(val) ({ U8 x = 0;         \
switch((val)) {                     \
case 1: x = (U8)CC_PROG_DESC_1; break;          \
case 2: x = (U8)CC_PROG_DESC_2; break;          \
case 3: x = (U8)CC_PROG_DESC_3; break;          \
case 4: x = (U8)CC_PROG_DESC_4; break;          \
case 8: x = (U8)CC_PROG_DESC_8; break;          \
case 10: x = (U8)CC_PROG_DESC_10; break;        \
default: x = 0;} x; } )

#define IS_1010              0x01  
#define TMR_SVC_DURATION     0x01
#define TMR_SVC_GAP_INTERVAL 0x02
/* TELICA:spamidi: Modified as part of bug 3528 fix */
#define MAXSVCTIMER          255   /* need to reconsider this value */

#define IVRMSG_INVALID_CALLED_NUMBER   "551"
#define IVRMSG_SCP_ERROR               "552"

#define MAXCCMTIMER       3          /* maximum number of simultaneous timers */
#define MAXCCCONCBTIMER   7          /* maximum number of simultaneous timers
                                      * for ConCb of CC */

#define CC_CON100_TIMER_RES  1       /* 100msec resolution Connection Control timer */
#define MAXCCCONCB100TIMER   1       /* maximum number of simultaneous 100 msec timers
                                      * for ConCb of CC */
#define CC_UPDSTS_WAIT 100 /* Bug#16149, rkhan, 10 secs */

#define TQNUMENT        512          /* timing queue number of timers entries */
#define BUFFER_TIMER_EXPIRY_DURATION  15
                                     /* Max Duration an event can sit in a Call
                                      * connection block */
#define DEFAULT_EARLY_ACM_TIMER 10  /* Default early acm timer  */

/* + FID 16970.0 + */
#define CCBS_ISDN_MAX_RUN_TMR       2
#define CCBS_ISDN_SUBSCRIPTION_TMR  2
#define CCBS_ISDN_GUARD_TMR         1 

#define CCBS_ISDN_INTNTY_TMR_VAL    32   /* 32 seconds */
#define CCBS_ISDN_WAIT_RSP_TMR_VAL  270  /* 4 minute 30 seconds */

#define CCBS_ISDN_MAX_DIALOGS       12800 /* Max number of CCBS TCAP dialogs */

#define CCBS_ISDN_RETENTION_SUPPORT_MASK      0x00000001  /* 1'st bit: retetion */
#define CCBS_ISDN_SET_RETENTION_SUPPORT_MASK(A) ((A) |= CCBS_ISDN_RETENTION_SUPPORT_MASK)
#define CCBS_ISDN_RESET_RETENTION_SUPPORT_MASK(A) ((A) &= ~CCBS_ISDN_RETENTION_SUPPORT_MASK)
#define CCBS_ISDN_IS_RETENTION_SUPPORTED(A) ((A) & CCBS_ISDN_RETENTION_SUPPORT_MASK)

#define CCBS_ISDN_CLEANUP_DONE_MASK      0x00000002  /* 2'st bit: cleanup */
#define CCBS_ISDN_SET_CLEANUP_DONE_MASK(A) ((A) |= CCBS_ISDN_CLEANUP_DONE_MASK)
#define CCBS_ISDN_RESET_CLEANUP_DONE_MASK(A) ((A) &= ~CCBS_ISDN_CLEANUP_DONE_MASK)
#define CCBS_ISDN_IS_CLEANUP_DONE(A) ((A) & CCBS_ISDN_CLEANUP_DONE_MASK)

#define CCBS_ISDN_TIME_OUT_MASK      0x00000004  /* 3'st bit: timeout */
#define CCBS_ISDN_SET_TIME_OUT_MASK(A) ((A) |= CCBS_ISDN_TIME_OUT_MASK)
#define CCBS_ISDN_RESET_TIME_OUT_MASK(A) ((A) &= ~CCBS_ISDN_TIME_OUT_MASK)
#define CCBS_ISDN_IS_TIME_OUT(A) ((A) & CCBS_ISDN_TIME_OUT_MASK)

#define CCBS_ISDN_SUBRSP_RCVED_MASK      0x00000008  /* 4'st bit: indicated terminate subscibe response received */
#define CCBS_ISDN_SET_SUBRSP_RCVED_MASK(A) ((A) |= CCBS_ISDN_SUBRSP_RCVED_MASK)
#define CCBS_ISDN_RESET_SUBRSP_RCVED_MASK(A) ((A) &= ~CCBS_ISDN_SUBRSP_RCVED_MASK)
#define CCBS_ISDN_IS_SUBRSP_RCVED(A) ((A) & CCBS_ISDN_SUBRSP_RCVED_MASK)

#define CCBS_ISDN_REFRESH_SUB_SENT_MASK      0x00000010  /* 5'st bit: refresh subscribe sent */
#define CCBS_ISDN_SET_REFRESH_SUB_SENT_MASK(A) ((A) |= CCBS_ISDN_REFRESH_SUB_SENT_MASK)
#define CCBS_ISDN_RESET_REFRESH_SUB_SENT_MASK(A) ((A) &= ~CCBS_ISDN_REFRESH_SUB_SENT_MASK)
#define CCBS_ISDN_IS_REFRESH_SUB_SENT(A) ((A) & CCBS_ISDN_REFRESH_SUB_SENT_MASK)

#define CCBS_ISDN_IC_RELEASED_MASK      0x00000020  /* 6'st bit: incoming released */
#define CCBS_ISDN_SET_IC_RELEASED_MASK(A) ((A) |= CCBS_ISDN_IC_RELEASED_MASK)
#define CCBS_ISDN_RESET_IC_RELEASED_MASK(A) ((A) &= ~CCBS_ISDN_IC_RELEASED_MASK)
#define CCBS_ISDN_IS_IC_RELEASED(A) ((A) & CCBS_ISDN_IC_RELEASED_MASK)

#define CCBS_ISDN_1st_NOTIFY_MASK      0x00000040  /* 7'st bit: first NOTIFY request-queued sent/received */
#define CCBS_ISDN_SET_1st_NOTIFY_MASK(A) ((A) |= CCBS_ISDN_1st_NOTIFY_MASK)
#define CCBS_ISDN_RESET_1st_NOTIFY_MASK(A) ((A) &= ~CCBS_ISDN_1st_NOTIFY_MASK)
#define CCBS_ISDN_IS_1st_NOTIFY(A) ((A) & CCBS_ISDN_1st_NOTIFY_MASK)

#define CCBS_ISDN_UPD_SUB_TMR      0x00000080  /* 8'st bit: first NOTIFY request-queued sent/received */
#define CCBS_ISDN_SET_UPD_SUB_TMR(A) ((A) |= CCBS_ISDN_UPD_SUB_TMR)
#define CCBS_ISDN_RESET_UPD_SUB_TMR(A) ((A) &= ~CCBS_ISDN_UPD_SUB_TMR)
#define CCBS_ISDN_IS_UPD_SUB_TMR(A) ((A) & CCBS_ISDN_UPD_SUB_TMR)

#define CCBS_ISDN_T_REQ_RR_SENT      0x00000100  /* 9'st bit: CCBS_T_REQUEST rr has been sent out */
#define CCBS_ISDN_SET_T_REQ_RR_SENT(A) ((A) |= CCBS_ISDN_T_REQ_RR_SENT)
#define CCBS_ISDN_RESET_T_REQ_RR_SENT(A) ((A) &= ~CCBS_ISDN_T_REQ_RR_SENT)
#define CCBS_ISDN_IS_T_REQ_RR_SENT(A) ((A) & CCBS_ISDN_T_REQ_RR_SENT)
/* - FID 16970.0 - */


/* Mapping direction */
#define DIR_UNKNOWN       0          /* MCAO_DEV WILTEL */
#define INCTOOUT          1          /* incoming -> outgoing */
#define OUTTOINC          2          /* outgoing -> incoming */

/* FID 14972.0, specify whether to get algorithm from Bcm's own interface or the other */
#define CC_OWN_INTF       1
#define CC_OTH_INTF       2

/* Send To direction for type of exchange information */
#define TOINCOMING        1        /* Send the xChange to call incoming side */
#define TOOUTGOING        2        /* Send the xChange to call outgoing side */


#define NO_DIR            0          /* FID 7804 - for overload Processing */
#define OVR_UP            1          /* FID 7804 - for overload Processing */
#define OVR_DOWN          2          /* FID 7804 - for overload Processing */
#define CC_CALLPRIO_0     0          /* FID 7804 - call priority 0         */
#define CC_CALLPRIO_1     1          /* FID 7804 - call priority 1         */

#define OBCM              1          /* Originating BCM      */
#define TBCM              2          /* Terminating BCM      */

#define CCSOURCE          1          /* Used for Move Rsc    */
#define CCDESTINATION     2          /* Used for Move Rsc    */

/* mgBlockDirection in CcConCb uses this for associating mgBlocks */
#define LEGID1            1          /* LegId 1 for a CC     */
#define LEGID2            2          /* LegId 2 for a CC     */

/* PsSap Disable flags, these two flags must be "OR-able" independently */
#define IC_PSSAP_DIS      1
#define OG_PSSAP_DIS      2

/* Audit States for RMSAP */
#define RMAUD_AWT_RMAUDREQ  0
#define RMAUD_AWT_SFAUDCFM  1

/* interface interworking */


/* Always add 2 because XMTOAM and AMTOXM are used twice in ccIWSwtchCb */
#define CC_MAX_SWITCH    (74 + 2)         /* Maximum number of elements in the array.
                                           * Actual number of interworking switches
                                           * may be less as more than one type of 
                                           * interfaces resolve to the same entity.
                                           * This happens in case of XM. */

#define SITOBI            0          /* ISUP -> BISUP */
#define BITOSI            1          /* BISUP -> ISUP */
#define SITOSI            2          /* ISUP -> ISUP */
#define BITOBI            3          /* BISUP -> BISUP */
#define INTOIN            4          /* ISDN -> ISDN */
#define SITOIN            5          /* ISUP -> ISDN */
#define INTOSI            6          /* ISUP -> ISDN */
#define AMTOAM            7          /* Q93B -> Q93B */
#define AMTOSI            8          /* Q93B -> ISDN */
#define SITOAM            9          /* ISUP -> Q93B */
#define XMTOIN           10          /* Connection Manager -> ISDN */
#define XMTOSI           11          /* Connection Manager -> ISUP */
#define XMTOAM           12          /* Connection Manager -> Q93B */
#define INTOXM           13          /* ISDN -> Connection Manager */
#define SITOXM           14          /* ISDN -> Connection Manager */
#define AMTOXM           15          /* ISDN -> Connection Manager */
#define SITOCSLN         16          /* ISUP -> CAS line */
#define SITOCSTG         17          /* ISUP -> CAS trunk */
#define INTOCSLN         18          /* ISDN -> CAS line */
#define INTOCSTG         19          /* ISDN -> CAS trunk */
#define CSLNTOSI         20          /* CAS line -> ISUP */
#define CSTGTOSI         21          /* CAS trunk -> ISUP */
#define CSLNTOIN         22          /* CAS line -> ISDN */
#define CSTGTOIN         23          /* CAS trunk -> ISDN */
#define CSLNTOCSTG       24          /* CAS line -> CAS trunk */
#define CSTGTOCSLN       25          /* CAS trunk -> CAS line */
#define CSLNTOCSLN       26          /* CAS line -> CAS line */
#define CSTGTOCSTG       27          /* CAS trunk -> CAS trunk */
#define SITOSIP          28          /* ISUP -> SIP      */
#define SIPTOSI          29          /* SIP  -> ISUP     */
#define INTOSIP          30          /* ISDN -> SIP      */
#define SIPTOIN          31          /* SIP  -> ISDN     */
#define CSLNTOSIP        32          /* CAS line -> SIP  */
#define SIPTOCSLN        33          /* SIP -> CAS line  */
#define CSTGTOSIP        34          /* CAS trunk -> SIP */
#define SIPTOCSTG        35          /* SIP -> CAS trunk */
#define SIPTOSIP         36          /* SIP -> SIP       */
#ifdef CC_CALEA
#define LCLSWTTOSI       37          /* LOCAL SWT to ISUP */ 
#define LCLSWTTOIN       38          /* LOCAL SWT to ISDN */
#define LCLSWTTOCSLN     39          /* LOCAL SWT to CAS LN */
#define LCLSWTTOCSTG     40          /* LOCAL SWT to CAS TG */ 
#define LCLSWTTOSIP      41          /* LOCAL SWT to SIP */ 
#define SITOLCLSWT       42          /* ISUP to LOCAL SWT */
#define INTOLCLSWT       43          /* ISDN to LOCAL SWT */
#define CSLNTOLCLSWT     44          /* CAS LN to LOCAL SWT */ 
#define CSTGTOLCLSWT     45          /* CAS TG to LOCAL SWT */
#define SIPTOLCLSWT      46          /* SIP to LOCAL SWT */
#endif


#define MGLNTOSI       47          /* MGCP LN to ISUP */ 
#define MGLNTOIN       48          /* MGCP LN to ISDN */
#define MGLNTOCSLN     49          /* MGCP LN to CAS LN */
#define MGLNTOCSTG     50          /* MGCP LN to CAS TG */ 
#define MGLNTOSIP      51          /* MGCP LN to SIP */ 
#define MGLNTOLCLSWT   52          /* MGCP LN to LOCAL SWT */ 
#define SITOMGLN       53          /* ISUP to MGCP LN */
#define INTOMGLN       54          /* ISDN to MGCP LN */
#define CSLNTOMGLN     55          /* CAS LN to MGCP LN */ 
#define CSTGTOMGLN     56          /* CAS TG to MGCP LN */
#define SIPTOMGLN      57          /* SIP to MGCP LN */
#define LCLSWTTOMGLN   58          /* LOCAL SWT to MGCP LN */
#define MGLNTOMGLN     59          /* MGCP LN to MGCP LN */
/* Begin: FID 14221 */
#define SITOIVT        60          /* SI to IVT          */
#define SIPTOIVT       61          /* SIP to IVT         */
#define IVTTOSI        62          /* IVT to SI          */
#define IVTTOSIP       63          /* IVT to SIP         */
/* End: FID 14221 */

/* FID 15310.1 + */
#define ICATOSI        64          /* ICA to SI          */
#define ICATOSIP       65          /* ICA to SIP         */
#define ICATOIN        66          /* ICA to ISDN        */
#define SIPTOICA       67          /* SIP to ICA         */
/* FID 15310.1 - */

#if 1 /* cc018.15 : addition */
#define INV_IF_SWTCH     0xFFFF      /* Invalid Interface switch */
#endif /* cc018.15 : addition */
#define CCOFFSETOF(X, Y)   (U32)( &(((X *)NULLP)->Y))
#define MINTRANSID       1      /* Min TransID between FIC and CC */

#define MINSUCTXID       1      /* Min CtxId between MGI and CC */

/* .................Protocol interworking  Begins.......................
 *
 *
 * All Protocol Interworking Codes should be added below this. For the sake 
 * of clarity the #defines for interworking is organized as follows
 *
 * ISUP ANSI 92
 * ISUP ANSI 95
 * ISUP ITU 92
 * ISUP ITU 97
 * ISUP ETSI
 * ISUP ETSIV3
 * ISUP Q767
 * BISUP 
 * ISDN
 *
 */

#ifdef CC_FID_1950_1773
/* ..........      ISUP ANSI 92 <-> Other Protocols  begin     ......... */

/* ISUP-ANSI92 -> BISUP-ITU */
#define CC_IW_SIANS92_TO_BIITU (((CC_SIANS92) << 16) | (CC_BIITU))
#define CC_IW_BIITU_TO_SIANS92 (((CC_BIITU) << 16) | (CC_SIANS92))

/* ISUP-ANSI92 -> ISDN-NI2 */
#define CC_IW_SIANS92_TO_INNI2 (((CC_SIANS92) << 16) | (CC_INNI2))
#define CC_IW_INNI2_TO_SIANS92 (((CC_INNI2) << 16) | (CC_SIANS92))

/* ISUP-ANSI92 -> ISDN ETSI */
#define CC_IW_SIANS92_TO_INETSI (((CC_SIANS92) << 16) | (CC_INETSI))
#define CC_IW_INETSI_TO_SIANS92 (((CC_INETSI) << 16) | (CC_SIANS92))

/* ISUP-ANSI92 -> ISUP-ITU92 */
#define CC_IW_SIANS92_TO_SIITU92 (((CC_SIANS92) << 16) | (CC_SIITU92))
#define CC_IW_SIITU92_TO_SIANS92 (((CC_SIITU92) << 16) | (CC_SIANS92))

/* ISUP-ANSI92 -> ISUP-ITU97 */
#define CC_IW_SIANS92_TO_SIITU97 (((CC_SIANS92) << 16) | (CC_SIITU97))
#define CC_IW_SIITU97_TO_SIANS92 (((CC_SIITU97) << 16) | (CC_SIANS92))

/* ISUP-ANSI92 -> ISUP-Q767 */
#define CC_IW_SIANS92_TO_SI767 (((CC_SIANS92) << 16) | (CC_SI76792))
#define CC_IW_SI767_TO_SIANS92 (((CC_SI76792) << 16) | (CC_SIANS92))

/* ..........      ISUP ANSI 92 <-> Other Protocols  end       ......... */

/* ..........      ISUP ANSI 95 <-> Other Protocols  begin     ......... */

/* ISUP-ANSI95 -> BISUP-ITU */
#define CC_IW_SIANS95_TO_BIITU (((CC_SIANS95) << 16) | (CC_BIITU))
#define CC_IW_BIITU_TO_SIANS95 (((CC_BIITU) << 16) | (CC_SIANS95))

/* ISUP-ANSI95 -> ISDN-NI2 */
#define CC_IW_SIANS95_TO_INNI2 (((CC_SIANS95) << 16) | (CC_INNI2))
#define CC_IW_INNI2_TO_SIANS95 (((CC_INNI2) << 16) | (CC_SIANS95))

/* ISUP-ANSI95 -> ISDN-ETSI */
#define CC_IW_SIANS95_TO_INETSI (((CC_SIANS95) << 8) | (CC_INETSI))
#define CC_IW_INETSI_TO_SIANS95 (((CC_INETSI) << 8) | (CC_SIANS95))

/* ISUP-ANSI95 -> ISUP-ITU92 */
#define CC_IW_SIANS95_TO_SIITU (((CC_SIANS95) << 16) | (CC_SIITU92))
#define CC_IW_SIITU_TO_SIANS95 (((CC_SIITU92) << 16) | (CC_SIANS95))

/* ISUP-ANSI95 -> ISUP-ITU97 */
#define CC_IW_SIANS95_TO_SIITU97 (((CC_SIANS95) << 16) | (CC_SIITU97))
#define CC_IW_SIITU97_TO_SIANS95 (((CC_SIITU97) << 16) | (CC_SIANS95))

/* ISUP-ANSI95 -> ISUP-Q767 */
#define CC_IW_SIANS95_TO_SI767 (((CC_SIANS95) << 16) | (CC_SI76792))
#define CC_IW_SI767_TO_SIANS95 (((CC_SI76792) << 16) | (CC_SIANS95))

/* ..........      ISUP ANSI 95 <-> Other Protocols  end       ......... */

/* ..........      ISUP ITU 92  <-> Other Protocols  begin     ......... */

/* ISUP-ITU92 -> BISUP-ITU */
#define CC_IW_SIITU92_TO_BIITU (((CC_SIITU92) << 16) | (CC_BIITU))
#define CC_IW_BIITU_TO_SIITU92 (((CC_BIITU) << 16) | (CC_SIITU92))

/* ISUP-ITU92 -> ISDN-ETSI */
#define CC_IW_SIITU92_TO_INETSI (((CC_SIITU92) << 16) | (CC_INETSI))
#define CC_IW_INETSI_TO_SIITU92 (((CC_INETSI) << 16) | (CC_SIITU92))

/* ISUP-ITU92 -> ISDN-ETSIV3 */
#define CC_IW_SIITU92_TO_INETSIV3 (((CC_SIITU92) << 16) | (CC_INETSIV3))
#define CC_IW_INETSIV3_TO_SIITU92 (((CC_INETSIV3) << 16) | (CC_SIITU92))

/* ISUP-ITU92 -> ISDN-QSIG */
#define CC_IW_SIITU92_TO_INQSIG (((CC_SIITU92) << 16) | (CC_INQSIG))
#define CC_IW_INQSIG_TO_SIITU92 (((CC_INQSIG) << 16) | (CC_SIITU92))

/* ISUP-ITU92 -> ISDN-NI2 */
#define CC_IW_SIITU92_TO_INNI2 (((CC_SIITU92) << 16) | (CC_INNI2))
#define CC_IW_INNI2_TO_SIITU92 (((CC_INNI2) << 16) | (CC_SIITU92))

/* ISUP-ITU92 -> Q.93B-PNNI */
#define CC_IW_SIITU92_TO_AMPNNI (((CC_SIITU92) << 16) | (CC_AM_SIG_PNNI))
#define CC_IW_AMPNNI_TO_SIITU92 (((CC_AM_SIG_PNNI) << 16) | (CC_SIITU92))

/* ISUP-767 -> ISUP-ITU92 */
#define CC_IW_SIITU92_TO_SI767 (((CC_SIITU92) << 16) | (CC_SI76792))
#define CC_IW_SI767_TO_SIITU92 (((CC_SI76792) << 16) | (CC_SIITU92))

/* ..........      ISUP ITU 92  <-> Other Protocols  end       ......... */

/* ..........      ISUP ITU 97  <-> Other Protocols  begin     ......... */

/* ISUP-ITU97 -> BISUP-ITU */
#define CC_IW_SIITU97_TO_BIITU (((CC_SIITU97) << 16) | (CC_BIITU))
#define CC_IW_BIITU_TO_SIITU97 (((CC_BIITU) << 16) | (CC_SIITU97))

/* ISUP-ITU97 -> ISDN-ETSI */
#define CC_IW_SIITU97_TO_INETSI (((CC_SIITU97) << 16) | (CC_INETSI))
#define CC_IW_INETSI_TO_SIITU97 (((CC_INETSI) << 16) | (CC_SIITU97))

/* ISUP-ITU97 -> ISDN-ETSIV3 */
#define CC_IW_SIITU97_TO_INETSIV3 (((CC_SIITU97) << 16) | (CC_INETSIV3))
#define CC_IW_INETSIV3_TO_SIITU97 (((CC_INETSIV3) << 16) | (CC_SIITU97))

/* ISUP-ITU97 -> ISDN-QSIG */
#define CC_IW_SIITU97_TO_INQSIG (((CC_SIITU97) << 16) | (CC_INQSIG))
#define CC_IW_INQSIG_TO_SIITU97 (((CC_INQSIG) << 16) | (CC_SIITU97))

/* ISUP-ITU97 -> ISDN-NI2 */
#define CC_IW_SIITU97_TO_INNI2 (((CC_SIITU97) << 16) | (CC_INNI2))
#define CC_IW_INNI2_TO_SIITU97 (((CC_INNI2) << 16) | (CC_SIITU97))

/* ISUP-ITU97 -> Q.93B-PNNI */
#define CC_IW_SIITU97_TO_AMPNNI (((CC_SIITU97) << 16) | (CC_AM_SIG_PNNI))
#define CC_IW_AMPNNI_TO_SIITU97 (((CC_AM_SIG_PNNI) << 16) | (CC_SIITU97))

/* ISUP-767 -> ISUP-ITU97 */
#define CC_IW_SI767_TO_SIITU97 (((CC_SI76792) << 16) | (CC_SIITU97))
#define CC_IW_SIITU97_TO_SI767 (((CC_SIITU97) << 16) | (CC_SI76792))

/* ..........      ISUP ITU 97  <-> Other Protocols  end       ......... */

/* ..........      ISUP ETSI    <-> Other Protocols  begin     ......... */

/* ISUP-ETSI -> ISDN-QSIG */
#define CC_IW_SIETSI_TO_INQSIG (((CC_SIETSI) << 16) | (CC_INQSIG))
#define CC_IW_INQSIG_TO_SIETSI (((CC_INQSIG) << 16) | (CC_SIETSI))

/* ISUP-ETSI -> ISDN-ITU */
#define CC_IW_SIETSI_TO_INITU (((CC_SIETSI) << 16) | (CC_INITU))
#define CC_IW_INITU_TO_SIETSI (((CC_INITU) << 16) | (CC_SIETSI))

/* ISUP-ETSI -> ISDN-ETSI */
#define CC_IW_SIETSI_TO_INETSI (((CC_SIETSI) << 16) | (CC_INETSI))
#define CC_IW_INETSI_TO_SIETSI (((CC_INETSI) << 16) | (CC_SIETSI))

/* ISUP-767 -> ISUP-ETSI */
#define CC_IW_SI767_TO_SIETSI (((CC_SI76792) << 16) | (CC_SIETSI))
#define CC_IW_SIETSI_TO_SI767 (((CC_SIETSI) << 16) | (CC_SI76792))

/* ..........      ISUP ETSI    <-> Other Protocols  end       ......... */

/* ..........      ISUP ETSIV3    <-> Other Protocols  begin     ......... */

/* ISUP-ETSIV3 -> ISDN-QSIG */
#define CC_IW_SIETSIV3_TO_INQSIG (((CC_SIETSIV3) << 16) | (CC_INQSIG))
#define CC_IW_INQSIG_TO_SIETSIV3 (((CC_INQSIG) << 16) | (CC_SIETSIV3))

/* ISUP-ETSIV3 -> ISDN-ITU */
#define CC_IW_SIETSIV3_TO_INITU (((CC_SIETSIV3) << 16) | (CC_INITU))
#define CC_IW_INITU_TO_SIETSIV3 (((CC_INITU) << 16) | (CC_SIETSIV3))

/* ISUP-ETSIV3 -> ISDN-ETSI */
#define CC_IW_SIETSIV3_TO_INETSI (((CC_SIETSIV3) << 16) | (CC_INETSI))
#define CC_IW_INETSI_TO_SIETSIV3 (((CC_INETSI) << 16) | (CC_SIETSIV3))

/* ISUP-767 -> ISUP-ETSIV3 */
#define CC_IW_SI767_TO_SIETSIV3 (((CC_SI76792) << 16) | (CC_SIETSIV3))
#define CC_IW_SIETSIV3_TO_SI767 (((CC_SIETSIV3) << 16) | (CC_SI76792))

/* ..........      ISUP ETSIV3    <-> Other Protocols  end       ......... */

/* ..........      ISUP Q767      <-> Other Protocols  begin     ......... */

/* ISUP-Q767 -> BISUP-ITU */
#define CC_IW_SI767_TO_BIITU (((CC_SI76792) << 16) | (CC_BIITU))
#define CC_IW_BIITU_TO_SI767 (((CC_BIITU) << 16) | (CC_SI76792))

/* ISUP-767 -> ISDN-QSIG */
#define CC_IW_SI767_TO_INQSIG (((CC_SI76792) << 16) | (CC_INQSIG))
#define CC_IW_INQSIG_TO_SI767 (((CC_INQSIG) << 16) | (CC_SI76792))

/* ISUP-767 -> ISDN-ITU */
#define CC_IW_SI767_TO_INITU (((CC_SI76792) << 16) | (CC_INITU))
#define CC_IW_INITU_TO_SI767 (((CC_INITU) << 16) | (CC_SI76792))

/* ISDN-ETSI -> ISUP-Q767 */
#define CC_IW_SI767_TO_INETSI (((CC_SI76792) << 16) | (CC_INETSI))
#define CC_IW_INETSI_TO_SI767 (((CC_INETSI) << 16) | (CC_SI76792))

/* ISUP-INDIA <-> ISUP-767 */
#define CC_IW_SI767_TO_SIINDIA (((CC_SI76792) << 16) | (CC_SIINDIA))
#define CC_IW_SIINDIA_TO_SI767 (((CC_SIINDIA) << 16) | (CC_SI76792))                                       

/* ISUP-KPNV2 <-> ISUP-767 */                                                                              
#define CC_IW_SIKPNV2_TO_SI767 (((CC_SIKPNV2) << 16) | (CC_SI76792))
#define CC_IW_SI767_TO_SIKPNV2 (((CC_SI76792) << 16) | (CC_SIKPNV2))

/* ISUP-KPNNL2 <-> ISUP-767 */
#define CC_IW_SIKPNNL2_TO_SI767 (((CC_SIKPNNL2) << 16) | (CC_SI76792))
#define CC_IW_SI767_TO_SIKPNNL2 (((CC_SI76792) << 16) | (CC_SIKPNNL2))

/* ISUP-UK <-> ISUP-767 */
#define CC_IW_SI767_TO_SIUK     (((CC_SI76792) << 16) | (CC_SIUK))
#define CC_IW_SIUK_TO_SI767     (((CC_SIUK) << 16) | (CC_SI76792))                                       

/* ISUP-RUSSIA <-> ISUP-767 */
#define CC_IW_SIRUSSIA_TO_SI767 (((CC_SIRUSSIA) << 16) | (CC_SI76792))
#define CC_IW_SI767_TO_SIRUSSIA (((CC_SI76792) << 16) | (CC_SIRUSSIA))

/* ISUP-THAILAND <-> ISUP-767 */
#define CC_IW_SITHAILAND_TO_SI767 (((CC_SITHAILAND) << 16) | (CC_SI76792))
#define CC_IW_SI767_TO_SITHAILAND (((CC_SI76792) << 16) | (CC_SITHAILAND))

/* ISUP-SPAIN <-> ISUP-767 */
#define CC_IW_SISPAIN_TO_SI767 (((CC_SISPAIN) << 16) | (CC_SI76792))
#define CC_IW_SI767_TO_SISPAIN (((CC_SI76792) << 16) | (CC_SISPAIN))

/* ISUP-NICARAGUA <-> ISUP-767 */
#define CC_IW_SINICARAGUA_TO_SI767 (((CC_SINICARAGUA) << 16) | (CC_SI76792))
#define CC_IW_SI767_TO_SINICARAGUA (((CC_SI76792) << 16) | (CC_SINICARAGUA))

/* ISUP-HK <-> ISUP-767 */
#define CC_IW_SI767_TO_SIHK      (((CC_SI76792) << 16) | (CC_SIHK))
#define CC_IW_SIHK_TO_SI767     (((CC_SIHK) << 16) | (CC_SI76792)) 

/* ISUP-PE <-> ISUP-767 */
#define CC_IW_SI767_TO_SIPE      (((CC_SI76792) << 16) | (CC_SIPE))
#define CC_IW_SIPE_TO_SI767     (((CC_SIPE) << 16) | (CC_SI76792))

/* ISUP-UA <-> ISUP-767 */
#define CC_IW_SI767_TO_SIUA     (((CC_SI76792) << 16) | (CC_SIUA))
#define CC_IW_SIUA_TO_SI767     (((CC_SIUA) << 16) | (CC_SI76792))

/* ISUP-ITU88 <-> ISUP-767 */
#define CC_IW_SI767_TO_SIITU88     (((CC_SI76792) << 16) | (CC_SIITU88))
#define CC_IW_SIITU88_TO_SI767     (((CC_SIITU88) << 16) | (CC_SI76792))

/* ISUP-VN <-> ISUP-767 */
#define CC_IW_SI767_TO_SIVN     (((CC_SI76792) << 16) | (CC_SIVN))
#define CC_IW_SIVN_TO_SI767     (((CC_SIVN) << 16) | (CC_SI76792))

/* ISUP-ITU99 <-> ISUP-767 */
#define CC_IW_SI767_TO_SIITU99     (((CC_SI76792) << 16) | (CC_SIITU99))
#define CC_IW_SIITU99_TO_SI767     (((CC_SIITU99) << 16) | (CC_SI76792))

/* ISUP-CHINA <-> ISUP-767 */
#define CC_IW_SI767_TO_SICHINA     (((CC_SI76792) << 16) | (CC_SICHINA))
#define CC_IW_SICHINA_TO_SI767     (((CC_SICHINA) << 16) | (CC_SI76792))

/* ..........      ISUP Q767      <-> Other Protocols  end       ......... */

/* ISDN-ETSI -> ISUP-FTZ */
#define CC_IW_SIFTZ_TO_INETSI (((CC_SIFTZ) << 16) | (CC_INETSI))
#define CC_IW_INETSI_TO_SIFTZ (((CC_INETSI) << 16) | (CC_SIFTZ))

/* ISDN-ETSI -> ISUP-SIGTEL */
#define CC_IW_SISIGTEL_TO_INETSI (((CC_SISIGTEL) << 16) | (CC_INETSI))
#define CC_IW_INETSI_TO_SISIGTEL (((CC_INETSI) << 16) | (CC_SISIGTEL))

/* ISDN-ETSI -> ISUP-767IT */
#define CC_IW_SI767IT_TO_INETSI (((CC_SI767IT) << 16) | (CC_INETSI))
#define CC_IW_INETSI_TO_SI767IT (((CC_INETSI) << 16) | (CC_SI767IT))

/* ISDN-ETSI -> ISUP-MEXICO */
#define CC_IW_SIMEXICO_TO_INETSI (((CC_SIMEXICO) << 16) | (CC_INETSI))
#define CC_IW_INETSI_TO_SIMEXICO (((CC_INETSI) << 16) | (CC_SIMEXICO))

/* ISDN-ETSI -> ISUP-TELSTRA */
#define CC_IW_SITELSTRA_TO_INETSI (((CC_SITELSTRA) << 16) | (CC_INETSI))
#define CC_IW_INETSI_TO_SITELSTRA (((CC_INETSI) << 16) | (CC_SITELSTRA))

/* ISDN-ETSI -> ISUP-G500V2 */
#define CC_IW_SIG500V2_TO_INETSI (((CC_SIG500V2) << 16) | (CC_INETSI))
#define CC_IW_INETSI_TO_SIG500V2 (((CC_INETSI) << 16) | (CC_SIG500V2))

/* ISDN-ETSI -> ISUP-G500V0 */
#define CC_IW_SIG500V0_TO_INETSI (((CC_SIG500V0) << 16) | (CC_INETSI))
#define CC_IW_INETSI_TO_SIG500V0 (((CC_INETSI) << 16) | (CC_SIG500V0))

/* ISDN-ETSI -> ISUP-BRAZIL */
#define CC_IW_SIBRAZIL_TO_INETSI (((CC_SIBRAZIL) << 16) | (CC_INETSI))
#define CC_IW_INETSI_TO_SIBRAZIL (((CC_INETSI) << 16) | (CC_SIBRAZIL))

/* ISDN-ETSI -> ISUP-INDIA */
#define CC_IW_SIINDIA_TO_INETSI (((CC_SIINDIA) << 16) | (CC_INETSI))
#define CC_IW_INETSI_TO_SIINDIA (((CC_INETSI) << 16) | (CC_SIINDIA))

/* ISDN-ETSI -> ISUP-VENEZUELA */
#define CC_IW_SIVENEZUELA_TO_INETSI (((CC_SIVENEZUELA) << 16) | (CC_INETSI))
#define CC_IW_INETSI_TO_SIVENEZUELA (((CC_INETSI) << 16) | (CC_SIVENEZUELA))

/* ISDN-ETSI -> ISUP-GERMANY */
#define CC_IW_SIGERMANY_TO_INETSI (((CC_SIGERMANY) << 16) | (CC_INETSI))
#define CC_IW_INETSI_TO_SIGERMANY (((CC_INETSI) << 16) | (CC_SIGERMANY))

/* ISDN-ETSI -> ISUP-POLAND */
#define CC_IW_SIPOLAND_TO_INETSI (((CC_SIPOLAND) << 16) | (CC_INETSI))
#define CC_IW_INETSI_TO_SIPOLAND (((CC_INETSI) << 16) | (CC_SIPOLAND))

/* ISDN-ETSI -> ISUP-CHINA */
#define CC_IW_SICHINA_TO_INETSI (((CC_SICHINA) << 16) | (CC_INETSI))
#define CC_IW_INETSI_TO_SICHINA (((CC_INETSI) << 16) | (CC_SICHINA))

/* ISDN-ETSI -> ISUP-COSTARICA */
#define CC_IW_SICOSTARICA_TO_INETSI (((CC_SICOSTARICA) << 16) | (CC_INETSI))
#define CC_IW_INETSI_TO_SICOSTARICA (((CC_INETSI) << 16) | (CC_SICOSTARICA))

/* ISDN-ETSI -> ISUP-VEMOVILNET */
#define CC_IW_SIVEMOVILNET_TO_INETSI (((CC_SIVEMOVILNET) << 16) | (CC_INETSI))
#define CC_IW_INETSI_TO_SIVEMOVILNET (((CC_INETSI) << 16) | (CC_SIVEMOVILNET))

/* ISDN-ETSI -> ISUP-ARGENTINA */
#define CC_IW_SIARGENTINA_TO_INETSI (((CC_SIARGENTINA) << 16) | (CC_INETSI))
#define CC_IW_INETSI_TO_SIARGENTINA (((CC_INETSI) << 16) | (CC_SIARGENTINA))

/* ISDN-ETSI -> ISUP-KOREAN */
#define CC_IW_SIKOREAN_TO_INETSI (((CC_SIKOREAN) << 16) | (CC_INETSI))
#define CC_IW_INETSI_TO_SIKOREAN (((CC_INETSI) << 16) | (CC_SIKOREAN))

/* ISDN-ETSI -> ISUP-KPNV2 */
#define CC_IW_SIKPNV2_TO_INETSI (((CC_SIKPNV2) << 16) | (CC_INETSI))
#define CC_IW_INETSI_TO_SIKPNV2 (((CC_INETSI) << 16) | (CC_SIKPNV2))

/* ISDN-ETSI -> ISUP-KPNNL2 */
#define CC_IW_SIKPNNL2_TO_INETSI (((CC_SIKPNNL2) << 16) | (CC_INETSI))
#define CC_IW_INETSI_TO_SIKPNNL2 (((CC_INETSI) << 16) | (CC_SIKPNNL2))

/* ISDN-ETSI -> ISUP-UK */
#define CC_IW_SIUK_TO_INETSI (((CC_SIUK) << 16) | (CC_INETSI))
#define CC_IW_INETSI_TO_SIUK (((CC_INETSI) << 16) | (CC_SIUK))

/* ISDN-ETSI -> ISUP-RUSSIA */
#define CC_IW_SIRUSSIA_TO_INETSI (((CC_SIRUSSIA) << 16) | (CC_INETSI))
#define CC_IW_INETSI_TO_SIRUSSIA (((CC_INETSI) << 16) | (CC_SIRUSSIA))

/* ISDN-ETSI -> ISUP-THAILAND */
#define CC_IW_SITHAILAND_TO_INETSI (((CC_SITHAILAND) << 16) | (CC_INETSI))
#define CC_IW_INETSI_TO_SITHAILAND (((CC_INETSI) << 16) | (CC_SITHAILAND))

/* ISDN-ETSI -> ISUP-SPAIN */
#define CC_IW_SISPAIN_TO_INETSI (((CC_SISPAIN) << 16) | (CC_INETSI))
#define CC_IW_INETSI_TO_SISPAIN (((CC_INETSI) << 16) | (CC_SISPAIN))

/* ISDN-ETSI -> ISUP-NICARAGUA */ 
#define CC_IW_SINICARAGUA_TO_INETSI (((CC_SINICARAGUA) << 16) | (CC_INETSI)) 
#define CC_IW_INETSI_TO_SINICARAGUA (((CC_INETSI) << 16) | (CC_SINICARAGUA))

/* ISDN-ETSI <-> ISUP-HK */
#define CC_IW_SIHK_TO_INETSI (((CC_SIHK) << 16) | (CC_INETSI))
#define CC_IW_INETSI_TO_SIHK (((CC_INETSI) << 16) | (CC_SIHK))

/* ISDN-ETSI <-> ISUP-PE */
#define CC_IW_SIPE_TO_INETSI (((CC_SIPE) << 16) | (CC_INETSI))
#define CC_IW_INETSI_TO_SIPE (((CC_INETSI) << 16) | (CC_SIPE))

/* ISDN-ETSI <-> ISUP-UA */
#define CC_IW_SIUA_TO_INETSI (((CC_SIUA) << 16) | (CC_INETSI))
#define CC_IW_INETSI_TO_SIUA (((CC_INETSI) << 16) | (CC_SIUA))

/* ISDN-ETSI <-> ISUP-ITU88 */
#define CC_IW_SIITU88_TO_INETSI (((CC_SIITU88) << 16) | (CC_INETSI))
#define CC_IW_INETSI_TO_SIITU88 (((CC_INETSI) << 16) | (CC_SIITU88))

/* ISDN-ETSI <-> ISUP-VN */
#define CC_IW_SIVN_TO_INETSI (((CC_SIVN) << 16) | (CC_INETSI))
#define CC_IW_INETSI_TO_SIVN (((CC_INETSI) << 16) | (CC_SIVN))

/* ISDN-ETSI <-> ISUP-ITU99 */
#define CC_IW_SIITU99_TO_INETSI (((CC_SIITU99) << 16) | (CC_INETSI))
#define CC_IW_INETSI_TO_SIITU99 (((CC_INETSI) << 16) | (CC_SIITU99))


/* BISUP-ITU -> BISUP-ITU */
#define CC_IW_BIITU_TO_BIITU (((CC_BIITU) << 16) | (CC_BIITU))

/* ISDN-QSIG -> ISDN-ETSI */
#define CC_IW_INETSI_TO_INQSIG (((CC_INETSI) << 16) | (CC_INQSIG))
#define CC_IW_INQSIG_TO_INETSI (((CC_INQSIG) << 16) | (CC_INETSI))

/* ISUP-ATT -> ISUP-ATT767 */
#define CC_IW_SIATT_TO_SIATT767 (((CC_SIATT)    << 16) | (CC_SIATT767))
#define CC_IW_SIATT767_TO_SIATT (((CC_SIATT767) << 16) | (CC_SIATT))

/* ISUP-ATT -> SIPT-ATT767 */
#define CC_IW_SIPTATT_TO_SIATT767 (((CC_SIPTATT)      << 16) | (CC_SIATT767))
#define CC_IW_SIATT767_TO_SIPTATT (((CC_SIATT767) << 16) | (CC_SIPTATT))

/* ISUP-ATT -> ISUP-ATT767 */
#define CC_IW_SIATT_TO_SIANSI92 (((CC_SIATT)    << 16) | (CC_SIANS92))
#define CC_IW_SIANSI92_TO_SIATT (((CC_SIANS92) << 16) | (CC_SIATT))

/* SIPT-ANS92 -> ISDN-NI2  */
#define CC_IW_SIPTANS92_TO_INNI2 (((CC_SIPTANS92) << 16) | (CC_INNI2))
#define CC_IW_INNI2_TO_SIPTANS92 (((CC_INNI2) << 16) | (CC_SIPTANS92))

/* SIPT-ANS92 -> ISDN-NI2  */
#define CC_IW_SIPTATT_TO_INNI2 (((CC_SIPTATT) << 16) | (CC_INNI2))
#define CC_IW_INNI2_TO_CC_SIPTATT (((CC_INNI2) << 16) | (CC_SIPTATT))

/* BASE PROTOCOL MAPPING INDEX */
#define CC_IW_SIPT_TO_SIPT     (((((U32) CC_SIPT) << 16) | (CC_SIPT)))
#define CC_IW_SI_TO_SIPT       (((((U32) CC_SI  ) << 16) | (CC_SIPT)))
#define CC_IW_SIPT_TO_SI       (((((U32) CC_SIPT) << 16) | (CC_SI)))


/* .................Protocol interworking  Ends  .......................*/
#endif /* End of CC_FID_1950_1773 */

/* Currently ISDN allows only 16 bit connection-ids, therefore */
/* the maximum possible SuConnId is conditionally defined */
#ifndef CLOUD
#define CC_MAXSUCONNID  0x00FFFFFF  /* Maximum possible SuConnId */
#else
#define CC_MAXSUCONNID  0x003FFFFF  /* Maximum possible SuConnId */
#endif
#define CC_CONNID_MASK  0xFFFFFFFE  /* Mask to distinguish between */
#define CC_GEO_AUD_CON_FLG  0x10000000  /* Mask to distiguish suConnId for Geo audit Calls */


#define CC_MAXTRANSID 0x00FFFFFF  /* Maximum possible TransId */
#define MAX_SURROGATE 3
#define MAX_TONERSCID 50
#define CC_MAXSUCTXID 0x000FFFFF  /* Maximum possible SuCtxId */
                                    /* outgoing and incoming suConnIds */

#define CCMAXINTFCKEYLEN  5         /* maximum length of interface key */
#define ST_DIGIT       0x0f         /* ST-DIGIT */

#define MAXTRANSID_IN_AUDITPACKET   128

/* packing interfaces */

#define MAXCCLICCT 11   /* ..and BICC and SIP and GR303 and MGCP makes 11  */

#define MAXCCLIMGA      5       /* Geo Audit */

#define MAXCCLIRMT 3
#define MAXCCLISFT 2
#define MAXCCLIAUDREQ 1

/* maximum lengths(in bytes) of a native  E164 format ATM address */
#define MAXE164ADDRLEN   8
#define CC_DUMMYPORTID1      0x7FFFFFFD
#define CC_DUMMYPORTID2      0x7FFFFFFE
#define CC_INVALID_PORTIDX   0xFF

/* CC cause codes for reattempt/cranking back a call (in case of PNNI routing) */
#define CC_CRANKBACK_CALL   1
#define CC_REATTEMPT_CALL   2
#define CC_RELEASE_CALL     3

/* CC Release codes */
#define CCSWTCHCONG  42
/* BIT CnStReq/Ind evntType */

#define CC_BI_ACM            0x00   /* Address complete */
#define CC_BI_CPG            0x02   /* Call progress */
#define CC_BI_FOT            0x04   /* Forward Transfer */
#define CC_BI_IAA            0x05   /* Intial Address Ack */
#define CC_BI_IAR            0x06   /* Intial Address Reject */
#define CC_BI_NRM            0x08   /* Network Resource Management */
#define CC_BI_SAM            0x0c   /* Subsequent address */
#define CC_BI_EXM            0x1c   /* ATF2 Exit message */
 
#define CC_M_EXIT            0xed   /* ATF2 Exit message code */

/* Message mapping */
#define IAM2IAM     0  /* Intial Address to Intial Address */
#define ACM2ACM     1  /* Address complete to Address complete */
#define CPG2CPG     2  /* Call progress to Call progress */
#define ANM2ANM     3  /* Answer to Answer */
#define SUS2SUS     4  /* Suspend to Suspend */
#define RES2RES     5  /* Resume to Resume */
#define SAM2SAM     6  /* Subsequent address to Subsequent address */
#define FOT2FOT     7  /* Forward Transfer to Forward Transfer */
#define NRM2NRM     8  /* Network Resource Mgmt to Network Resource Mgmt */
#define REL2REL     9  /* Release to Release */
#define USR2USR    10  /* User to user to User to user */
#define FAR2USR    11  /* Facility request to User to user */
#define FAA2USR    12  /* Facility acknowledge to User to user */
#define FRJ2USR    13  /* Facility reject to User to user */
#define EXM2EXM    14  /* Exit to Exit */
#define IAR2REL    15  /* Intial Address Reject to Release */
#define USR2FAR    16  /* User to user to Facility request */
#define USR2FAA    17  /* User to user to Facility acknowledge */
#define USR2FRJ    18  /* User to user to Facility reject */
#define INR2INF    19  /* Information Req to Information */
#define INF2INJ    20  /* Information  to ?? */
#define IDR2IRJ    21  /* Identity Request  to Identity Reject */
#define FAR2FAR    11  /* Facility request to Facility Request */
#define FAA2FAA    12  /* Facility acknowledge to Facility acknowledge */
#define FRJ2FRJ    13  /* Facility reject to Facility reject */
#define INR2INR    15  /* Information Req to Information */
#define INF2INF    16  /* Information  to Information */
#define IDR2IDR    17  /* Identity Request  to Identity Request */
#define MCP2MCP    18  /* Malicious call print  */
#define CRG2CRG    19  /* Charge Info  */
#define TCM2TCM    20  /* Tariff Change  */
#define CAK2CAK    21  /* Charge Acknowledge  */
#define COM2COM    22  /* Call Offering  */
#define LPR2LPR    23  /* Loop Prevention  */
#define CHG2CHG    24  /* GT_FTZ charging message */
#define CHGE2CHGE  25  /* GT_FTZ charging extended message */
#define CHGEA2CHGEA  26  /* GT_FTZ charging extended ack msg */
#define FIN2FIN    27  /* FTZ Facility information message */
#define EHZA2EHZA  28  /* FTZ Hangup indication of A-Tln message */
#define UIN2UIN    29  /* FTZ User information message */
#define NANA2NANA  30  /* FTZ National  message */
#define FAC2FAC    31  /* Facility message */
#define IDP2IDP    32  /* Identification Response message */
#define CONFWDNOTF2CONFWDNOTF    33
#define CONNECTED2CONNECTED      34
#define HOLD2HOLD                35
#define HOLDRSP2HOLDRSP          36
#define XFER2XFER                37
#define XFERRSP2XFERRSP          38
#define XFERQRY2XFERQRY          39
#define XFERQRYRESP2XFERQRYRESP  40
#define NOIND2NOIND              41
#define PSALNG2PSALNG            42
#define PREREL2PREREL            43 /* Pre-release Message */
#define APPTRAN2APPTRAN          44 /* Application Transport Message */
#define COC2COC                  45 /* Mexican Call Offering Cancellation message */
#define RCL2RCL                  46 /* Mexican Recall Message */
#define FAN2FAN                  47 /* Mexican False Answer Message */
#define UMSG2UMSG                48 /* ISUP Unrecognised Message */
#define DRS2DRS                  49 /* Delayed Release Message */
#define CCL2CCL                  50 /* China CCL message */
#define OPR2OPR                  51 /* China and Costa Rica OPR message */
#define MPM2MPM                  52 /* China MPM Message */
#define NMINFO2NMINFO            53 /* China ISUP internal NMINFO message from ISUP to GCC */
#define MCI2MCI                  54 /* Argentina and Spain MCI Message */
#define RNG2RNG                  55 /* Russia ISUP Ringing message */
#define FLA2FLA                  56 /* Thailand FLA Message */
#define OFR2OFR                  57 /* Thailand OFR Message */
#define SIPTUPDATE2BICCORSIPTUPDATE  58 /*SIPT UPDATE to SIPT UPDATE*/
#define BICCORSIPTUPDATERSP2BICCORSIPTUPDATERSP  59 /*SIPT UPDATERSP to SIPT UPDATERSP*/
#define SIPTACK2BICCORSIPTACK     60 /*SIPT ACK to SIPT ACK */
#define SIPTPRACK2BICCORSIPTPRACK 61 /*SIPT PRACK to SIPT PRACK */
#define SIPTPRACKRSP2SIPTPRACKRSP 62 /*SIPT PRACK RSP to SIPT PRACK RSP */
#define CMR2CMR                   63 /* ITU88 Call Modification Request Message */
#define SDM2SDM                   64 /* ITU99 Subsequent Directory Number Message */ 
#define COF2COF                   65 /* Italian ISUP call offering message */
#define VPN2VPN                   66 /* Brunei ISUP VPN facility message */
#define ITX2ITX                   67 /* Belgium ISUP ITX message */
#define TXA2TXA                   68 /* Belgium ISUP TXA message */
#define ITXB2ITXB                 69 /* Belgium ISUP ITXB message */
#define TXAB2TXAB                 70 /* Belgium ISUP TXAB message */
#define CFN2CPG                   71 /* BUG:82657 SIPT 183+CFN */
#define SIDTMFINFO2SIDTMFINFO     72 /* FID 16465.0 */
#define SIDTMFINFORSP2SIDTMFINFORSP    73 /* FID 16465.0 */

#define CC_MAX_BBTOBBMSG 11
#define CC_MAX_MSG    (CFN2CPG+1)
#define CC_MAX_NBTONBMSG (SIDTMFINFORSP2SIDTMFINFORSP+1) /* FID 16465.0 */ 

#if 0 /* cc019.15 : change - old code */
#define CC_MAX_INTOINMSG 15
#else /* cc019.15 : change - new code */
#define CC_MAX_INTOINMSG 16
#endif /* cc019.15 : change */
#define CCIGNOREMAPPING 100  /* This value is returned if no mapping */
                             /* is to be performed */
#ifdef CC_FID_2138_2167_2168_2169_2171
#define CCRELCALLFROMMAPPING 101 /* This is returned if core mapping 
                                    need to release the call*/
#endif 
/* ISDN Message mapping */
#define INSETUPTOINSETUP       0  /* Setup to Setup */
#define INALERT2INALERT        1  /* Alerting to Alerting */
#define INPROG2INPROG          2  /* Call progress to Call progress */
#define INCONTOINCON           3  /* Answer to Answer */
#define INSUS2INNOTIF          4  /* Suspend to Suspend */
#define INRES2INNOTIF          5  /* Resume to Resume */
#define ININFOTOININFO         6  /* Information to Informatiom */
#define INUSR2INUSR            7  /* User-User to User-User */
#define INNOTIF2INNOTIF        8  /* Notification to Notification */
#define INREL2INREL            9  /* Release to Release */
#define INHOLD2INNOTIF        10  /* Hold to Hold */
#define INRETRIEVE2INNOTIF    11  /* Retrieve to Retrieve */
#define INFACIL2INFACIL       12  /* Facility request to Facility Request */
#define INFACILACK2INFACILACK 13  /* Facility Ack to Facility Ack */
#define INFACILREJ2INFACILREJ 14  /* Facility Reject to Facility Reject */
#if 1 /* cc019.15 : addition */
#define INCALLPROC2INCALLPROC 15  /* Call Proceeding to Call Proceeding */
#endif /* cc019.15 : addition */

/* Message compatibility */
 
#define CC_ACM_MSG           0x83   /* Address Complete Message */
#define CC_ANM_MSGC          0x83   /* Answer Message */
#define CC_BLO_MSGC          0xA9   /* Block Message */
#define CC_BLA_MSGC          0xA9   /* Block AcknowledgeMessage */
#define CC_CPG_MSGC          0x90   /* Call Progress Message */
#define CC_CFN_MSGC          0x91   /* Confusion Message */
#define CC_FOT_MSGC          0x84   /* Forward Transfer Message */
#define CC_EXM_MSGC          0x94   /* Exit Message */
#define CC_IAA_MSGC          0xC3   /* Inital Address Acknowledge Message */
#define CC_IAR_MSGC          0xC3   /* Initial Address Reject Message */
#define CC_IAM_MSGC          0xC3   /* Initial Address Message Message */
#define CC_REL_MSGC          0xA9   /* Release Message */
#define CC_RLC_MSGC          0xA9   /* Release Complete Message */
#define CC_RSM_MSGC          0xA9   /* Reset Message */
#define CC_RAM_MSGC          0xA9   /* Reset Acknowledge Message */
#define CC_RES_MSGC          0x94   /* Resume Message */
#define CC_SUS_MSGC          0x94   /* Suspend Message */
#define CC_UBL_MSGC          0xA9   /* Unblock Message */
#define CC_UBA_MSGC          0xA9   /* Unblock Acknowledge Message */
#define CC_NRM_MSGC          0x90   /* Network Resource Management Message */
#define CC_USR_MSGC          0x90   /* User to User Message */

/* Message mapping from ISDN to ISUP */
#define INSETUP2SIIAM       0   /* SETUP to Initial Address message */
#define ININFORMATION2SISAM 1   /* INFORMATION to Subsequent Address message */
#define INALERTING2SIXXX    2   /* ALERTING to ACM or CPG message */
#define INPROGRESS2SIACM    3   /* PROGRESS to Address complete message */
#define INCONNECT2SIXXX     4   /* CONNECT to ANM or CON message */
#define INDISCONNECT2SIREL  5   /* DISCONNECT, RELEASE, REL COMPLETE to REL*/
#define INHOLD2SICPG        6   /* HOLD to Calll progress */
#define INRETRIEVE2SICPG    7   /* RETRIEVE to Call progress */
#define INSUSPEND2SISUS     8   /* SUSPEND to Suspend */
#define INRESUME2SIRES      9   /* RESEUME to Resume */
#define INNOTIFY2SICPG     10   /* NOTIFY to Call progress */
#define INUSRINFO2SIUSR    11   /* USER INFORMATION to USR */
#define INCALLPROC2SIACM   12   /* CALL PROCEEDING to ACM */
#define INFAC2SIFAC      13   /* FACILITY to FACILITY FID 16367.0 */
#define INFAC2SICCBSINFO 14   /* ISDN FACILITY to BICC CCBSINFO FID 16970.0*/ 
#define CC_MAX_IN2SI_MSG (INFAC2SICCBSINFO+1)

/* message mapping from ISUP to ISDN */ 
#define SIIAM2INSETUP       0     /* Initial Address message to SETUP */
#define SIACM2INXXX         1     /* ACM message to CALL PROC, PROGRESS, 
                                     ALERTING and DISCONNECT */
#define SICPG2INXXX         2     /* Call progress to DISCONNECT, ALERTING, 
                                     PROGRESS, and NOTIFY */
#define SIANM2INXXX         3     /* ANM to CONNECT or DISCONNECT */
#define SIREL2INXXX         4     /* REL to DISCONNECT, FACILITY, or RELEASE */
#define SISUS2INNOTIFY      5     /* Suspend to NOTIFY */ 
#define SIRES2INNOTIFY      6     /* Resume to NOTIFY */
#define SIUSR2INUSRINFO     7     /* USR to USER INFOMATION */
#define SISAM2ININFO        8     /* Subsequent Address message to Info */
#define SIINR2INXXX         9     /* ISUP INR message mapping */
#define BICCIN_CONFWDNOTF2CONNECTED 10 
#define BICCXFER2INNOTHING  11
#define BICCHOLD2INNOTHING  12
#define BICCQUERY2INNOTHING 13
/*BUG:47083*/
#define SIIDR2INNOTHING     14     
#define SICCL2INNOTHING     15     
#define BICCUPDATE2INNOTHING   16     
#define BICCPRACK2INNOTHING    17 /* SIP-T/BICC PRACK to Nothing */
#define BICCCCBSINFO2REGISTER 18  /*FID 16970.0 */
#define BICCCCBSINFO2FACILITY 19  /*FID 16970.0 */
#define CC_MAX_SI2IN_MSG (BICCCCBSINFO2FACILITY+1)
/*End of BUG:47083*/ 

/* Message mapping from PNNI to ISUP */
#define AMSETUP2SIIAM       0   /* SETUP to Initial Address message */
#define AMCALLPROC2SIACM    1   /* CALL PROCEEDING to ACM */
#define AMALERTING2SICPG    2   /* ALERTING to CPG message */
#define AMPROGRESS2SICPG    3   /* PROGRESS to Address complete message */
#define AMCONNECT2SIANM     4   /* CONNECT to ANM message */
#define AMNOTIFY2SICPG      5   /* NOTIFY to Call progress */
#define AMREL2SIREL         6   /* RELEASE to REL*/
#define CC_MAX_AM2SI_MSG (AMREL2SIREL+1)

/* message mapping from ISUP to PNNI */
#define SIIAM2AMSETUP       0     /* Initial Address message to SETUP */
#define SIACM2AMALERT       1     /* ACM message to ALERTING */
#define SICPG2AMXXX         2     /* Call progress to ALERTING, PROGRESS */
#define SIANM2AMCONNECT     3     /* ANM to CONNECT */
#define SIREL2AMREL         4     /* REL to RELEASE */
#define CC_MAX_SI2AM_MSG (SIREL2AMREL+1)

/* hash define for GAT parameter index */
#define AM_MI_GAT        0xff  

/* message mapping from Q.93B to Q.93B */ 
#define AMSETUPTOAMSETUP       0  /* Setup to Setup */
#define AMALERT2AMALERT        1  /* Alerting to Alerting */
#define AMPROG2AMPROG          2  /* Call progress to Call progress */
#define AMINFOTOAMINFO         3  /* Information to Informatiom */
#define AMNOTIF2AMNOTIF        4  /* Notification to Notification */
#define AMUSR2AMUSR            5  /* User-User to User-User */
#define AMCONTOAMCON           6  /* Answer to Answer */
#define AMREL2AMREL            7  /* Release to Release */
#define CC_MAX_AMTOAMMSG  8

/* message mapping from ISDN to XM */ 
#define INSETUP2XMSETUP       0
#define INCON2XMCON           1
#define INREL2XMREL           2
#define CC_MAX_INTOXMMSG (INREL2XMREL+1)

/* message mapping from XM to ISDN */ 
#define XMSETUP2INSETUP       0
#define XMCON2INCON           1
#define XMREL2INREL           2
#define CC_MAX_XMTOINMSG (XMREL2INREL+1)

/* message mapping from Q.93B to XM */ 
#define AMSETUP2XMSETUP       0
#define AMCON2XMCON           1
#define AMREL2XMREL           2
#define CC_MAX_AMTOXMMSG (AMREL2XMREL+1)

/* message mapping from XM to Q.93B */ 
#define XMSETUP2AMSETUP       0
#define XMCON2AMCON           1
#define XMREL2AMREL           2
#define CC_MAX_XMTOAMMSG (XMREL2AMREL+1)

/* message mapping from ISUP to XM */ 
#define SIIAM2XMSETUP         0
#define SIANM2XMCON           1
#define SIREL2XMREL           2
#define CC_MAX_SITOXMMSG (SIREL2XMREL+1)

/* message mapping from XM to ISUP */ 
#define XMSETUP2SIIAM         0
#define XMCON2SIANM           1
#define XMREL2SIREL           2
#define CC_MAX_XMTOSIMSG (XMREL2SIREL+1)

/* CAS message mapping */
#define CSADDRCFM2CSCONREQ    0
#define CSCONCFM2CSCONRSP     1
#define CSONHOOK2CSONHOOK     2
#define CSOFFHOOK2CSOFFHOOK   3
#if 1 /*FGD*/
#define CSACM2CSSTART         4
#define CSSTART2CSSTART       5
#define CC_MAX_CS2CS_MSG      (CSSTART2CSSTART+1)
#endif /*FGD*/

/* message mapping from CAS to ISDN */
#define CSADDRCFM2INSETUP     0
#define CSRINGING2INALERTING  1
#define CSCONCFM2INCON        2
#define CSONHOOK2INRELEASE    3
#define CC_MAX_CS2IN_MSG      (CSONHOOK2INRELEASE+1)

/* message mapping from ISDN to CAS */
#define INSETUP2CSCONREQ      0
#define INALERTING2CSRINGING  1
#define INCONTOCSCONRESP      2
#define INRELEASE2CSRELREQ    3
#define CC_MAX_IN2CS_MSG      (INRELEASE2CSRELREQ+1)

/* message mapping from CAS to ISUP */
#define CSADDRCFM2SIIAM       0
#define CSACM2SIACM           1  /* Trunks */
#define CSRINGING2SIACM       1  /* Lines */
#define CSCONCFM2SIANM        2
#define CSONHOOK2SIREL        3
#define CSSUS2SISUS           4
#define CSRES2SIRES           5
#if 1 /*FGD*/
#define CSADDRCFM2SICRM       6
#define CSRELIND2SIREL        7
#define CC_MAX_CS2SI_MSG      (CSRELIND2SIREL+1)
#endif /*FGD*/

/* message mapping from ISUP to CAS */
#define SIIAM2CSCONREQ        0
#define SIACM2CSRINGING       1
#define SIANM2CSCONRSP        2
#define SIREL2CSRELREQ        3
#define SICRA2CSSTART         4
#define BICCCS_CONFWDNOTF2CONNECTED 5
#define SISUSP2CSONHK        6
#define SIRES2CSOFFHK        7
#define BICCXFER2CSNOTHING   8
#define BICCHOLD2CSNOTHING   9
#define BICCQUERY2CSNOTHING  10
#define SIINR2CSNOTHING      11 /* ISUP INR to CAS Mapping Nothing. Info is sent back to ISUP */
#define BICCUPDATE2CSNOTHING 12 /* SIPt/BICC Update to Nothing */
#define SIPRACK2CSNOTHING    13 /* SIP-T/BICC PRACK to Nothing */
#define SICPG2CSNOTHING      14 /* SIP-T/BICC CPG to Nothing */
#define CC_MAX_SI2CS_MSG      (SICPG2CSNOTHING+1)


/* ISUP to SIP */
#define SIIAM2SIPINVITE      0
#define SIACM2SIP18X         1
#define SICPG2SIP183         2
#define SIANM2SIP200OK       3
#define SIREL2SIPBYE         4
#define BICCSIP_CONFWDNOTF2CONNECTED         5
#define BICCXFER2SIPXFER     6
#define BICCHOLD2SIPHOLD     7
#define BICCQURY2SIPQURY     8
#define BICCXFRRSP2SIPXFRRSP 9
#define BICCHLDRSP2SIPHLDRSP 10
#define BICCQRYRSP2SIPQRYRSP 11
#define SIINR2SIPXXX         12 /* ISUP INR to SIP Nothing, INFO is sent back to ISUP */
/*Feature 1789 - German ISUP*/
#define SIIDR2SIPXXX         13 /* ISUP IDR to SIP Nothing, IRS is sent back to ISUP */
#define SIMCI2SIPXXX         14 /* Argentine ISUP MCI to SIP Nothing, record call info */
#define SICRG2SIP183ORINFO   15 /* Netherlands ISUP: CRG to 183 or INFO */
#define SIFAR2SIPREFER       16
#define SIFAA2SIPNOTIFY      17
#define SIFRJ2SIPNOTIFY      18
#define SICCL2SIPBYE         19 /* Russia SIP/SIPT IW */
#define BICCORSIPTUPDATE2SIPUPDATE        20 /* FID-2112 */
#define BICCORSIPTUPDATERSP2SIPUPDATERSP  21 /* FID-2112 */
#define BICCORSIPTACK2SIPACK              22 /* FID-2112 */
#define BICCORSIPTPRACK2SIPPRACK          23 /* FID-2112 */
#define SIPTPRACKRSP2SIPPRACKRSP          24
#define SIAPM2SIP18XORINFO                25 /* FID 14840 AoC */
#define SISAM2SIPINFOOVERLAP              26 /* FID 15174.0 */
#define SICFN2SIP183                      27 /* BUG:82657 */
#define SIFAC2SIP302                      28 /* FID 16367.0 */
#define SIDTMFINFO2SIPDTMFINFO            29 /* FID 16465.0 */
#define SIDTMFINFORSP2SIPDTMFINFORSP      30 /* FID 16465.0 */
#define BICCCCBSINFO2SIPNOTIFY            31 /* FID 16970.0 */
#define CC_MAX_SI2SIP_MSG    (BICCCCBSINFO2SIPNOTIFY+1)

/* SIP to ISUP */
#define SIPINVITE2SIIAM      0
#define SIP18X2SIACM         1
#define SIP200OK2SIANM       2
#define SIPBYE2SIREL         3
#define SIPXFER2BICCXFER     4
#define SIPHOLD2BICCHOLD     5
#define SIPQRY2BICCQRY       6
#define SIPXFRRSP2BICCXFRRSP 7
#define SIPHLDRSP2BICCHLDRSP 8
#define SIPQRYRSP2BICCQRYRSP 9
#define SIPUPD2BICCUPD       10
#define SIPUPDRSP2BICCUPDRSP 11
#define SIPREFER2SIFAR       12
#define SIPNOTIFY2SIFAAORFRJ 13
#define SIPACK2BICCORSIPTACK     14
#define SIPPRACK2BICCORSIPTPRACK 15
#define SIPPRACKRSP2SIPTPRACKRSP 16
#define SIPINFO2SIAPM        17 /* FID 14840 AoC */
#define SIPINFOOVERLAP2SAM   18
#define SIPDTMFINFO2SIDTMFINFO         19   /* FID 16465.0 */
#define SIPDTMFINFORSP2SIDTMFINFORSP   20   /* FID 16465.0 */
#define SIPSUBSCRIBE2SICCBSINFO        21   /* FID 16970.0 */

#define CC_MAX_SIP2SI_MSG    (SIPSUBSCRIBE2SICCBSINFO+1)

/* ISDN to SIP */
#define INSETUP2SIPINVITE    0
#define INALERTING2SIP18X    1
#define INPROGRESS2SIP183    2
#define INCONNECT2SIP200OK   3
#define INDIS2SIPBYE         4
#define INCALLPROC2SIP183    5
#define INCALLINDPFAC2SIPMSG 6   /*FID 16206.0*/
#define INCALLFAC2SIP302     7   /*FID 16367.0*/
#define INCALLFAC2SIPNOTIFY  8   /*FID 16970.0*/
#define INCALLREG2SIPSUB     9   /*FID 16970.0*/
#define INCALLREL2SIPSUB     10  /*FID 16970.0*/
#define CC_MAX_IN2SIP_MSG    (INCALLREL2SIPSUB+1)

/* SIP to ISDN */
#define SIPINVITE2INSETUP    0
#define SIP18X2INALERTING    1
#define SIP200OK2INCONNECT   2
#define SIPBYE2INDIS         3
#define SIPXFER2INNOTHING    4
#define SIPHOLD2INNOTHING    5
#define SIPQUERY2INNOTHING   6
#define SIPUPDATE2INNOTHING  7
#define SIPPRACK2INNOTHING   8
#define SIP200OKMSG2ININDPFACILITY  9   /*FID 16206.0*/
#define SIPMSG2ININDPFACILITY 10        /*FID 16206.0*/
#define SIPINFOINFACILITY     11        /* FID 16706 INFO to FACILITY */
#define SIP200OKBYEINREL      12        /* FID 16706 200 OK BYE to RELEASE w/FACILITY */
#define SIPSUBCRIBE2INREGISTER 13       /* FID 16970.0 SUBSCRIBE to REGISTER */
#define SIPNOT2INFACILITY     14        /*FID 16970.0*/
#define CC_MAX_SIP2IN_MSG    (SIPNOT2INFACILITY+1)

/* CASLN to SIP */
#define CSLNADDRCFM2SIPINVITE  0
#define CSLNRINGING2SIP18X     1 /* Lines  */
#define CSLNCONCFM2SIP200OK    2
#define CSLNONHOOK2SIPBYE      3
#define CSLNHOOKFLASH2SIPINFO_HT 4 /* hook flash to Info */
#define CC_MAX_CSLN2SIP_MSG    (CSLNHOOKFLASH2SIPINFO_HT+1)

/* SIP to CASLN */
#define SIPINVITE2CSLNCONREQ   0
#define SIP18X2CSLNRINGING     1
#define SIP200OK2CSLNCONRSP    2
#define SIPBYE2CSLNRELREQ      3
#define SIPXFER2CSLNNOTHING    4
#define SIPHOLD2CSLNNOTHING    5
#define SIPQUERY2CSLNNOTHING   6
#define SIPUPDATE2CSLNNOTHING  7 /* 56999 */
#define SIPPRACK2CSLNNOTHING   8
#define CC_MAX_SIP2CSLN_MSG  (SIPPRACK2CSLNNOTHING+1)

/* CASTG to SIP */
#define CSTGADDRCFM2SIPINVITE  0
#define CSTGACM2SIP18X         1 /* Trunks */
#define CSTGCONCFM2SIP200OK    2
#define CSTGONHOOK2SIPBYE      3
#define CC_MAX_CSTG2SIP_MSG    (CSTGONHOOK2SIPBYE+1)

/* SIP to CASTG */
#define SIPINVITE2CSTGCONREQ   0
#define SIP18X2CSTGRINGING     1
#define SIP200OK2CSTGCONRSP    2
#define SIPBYE2CSTGRELREQ      3
#define SIPXFER2CSTGNOTHING    (SIPXFER2CSLNNOTHING)
#define SIPHOLD2CSTGNOTHING    (SIPHOLD2CSLNNOTHING)
#define SIPQUERY2CSTGNOTHING   (SIPQUERY2CSLNNOTHING)
#define SIPUPDATE2CSTGNOTHING  (SIPUPDATE2CSLNNOTHING)
#define SIPPRACK2CSTGNOTHING   (SIPPRACK2CSLNNOTHING)
#define CC_MAX_SIP2CSTG_MSG    (SIPPRACK2CSLNNOTHING+1)

/* SIP to SIP */
#define SIPINVITE2SIPINVITE    0
#define SIP18X2SIP18X          1
#define SIP200OK2SIP200OK      2
#define SIPBYE2SIPBYE          3
#define SIPXFER2SIPXFER        4
#define SIPHOLD2SIPHOLD        5
#define SIPQURY2SIPQRYR        6
#define SIPXFRRSP2SIPXFRRSP    7
#define SIPHLDRSP2SIPHLDRSP    8
#define SIPQRYRSP2SIPQRYRSP    9
#define SIPUPD2SIPUPD         10
#define SIPUPDRSP2SIPUPDRSP   11
#define SIPREFER2SIPREFER      12
#define SIPNOTIFY2SIPNOTIFY    13
#define SIPACK2SIPACK          14
#define SIPPRACK2SIPPRACK      15
#define SIPPRACKRSP2SIPPRACKRSP 16
#define SIPINFO2SIPINFO        17 /* FID 14840 AoC */
#define SIPINFOOVERLAP2SIPINFOOVLAP   18 /* FID 15174.0 */
#define SIPDTMFINFO2SIPDTMFINFO       19 /* FID 16465.0 */
#define SIPDTMFINFORSP2SIPDTMFINFORSP 20 /* FID 16465.0 */
#define SIPINFOCPG2SIPINFOCPG   21 /* Bug 93116: Bouygues issue: it is for INFO Generic, not just CPG */

#define CC_MAX_SIP2SIP_MSG     (SIPINFOCPG2SIPINFOCPG+1)

#ifdef CC_CALEA
#define LCLCONINDTOSIIAM       0
#define LCLRELTOSIREL          1 
#define  CC_MAX_LCLSWT2SI_MSG  (LCLRELTOSIREL+1)
#endif

/* MGCP message mapping */
#define MGCPADDRCFM2MGCPCONREQ    0
#define MGCPCONCFM2MGCPCONRSP     1
#define MGCPONHOOK2MGCPONHOOK     2
#define MGCPOFFHOOK2MGCPOFFHOOK   3
#define MGCPALERTING2MGCPMDCX     4
#define CC_MAX_MGCP2MGCP_MSG      (MGCPALERTING2MGCPMDCX+1)

/* message mapping from MGCP to ISUP */
#define MGCPADDRCFM2SIIAM       0
#define MGCPALERTING2SIACM      1
#define MGCPCONCFM2SIANM        2
#define MGCPXFERRSP2BICCXFERRSP 3
#define MGCPHOLDRSP2BICCHOLDRSP 4
#define MGCPQRYRSP2BICCQRYRSP   5
#define CC_MAX_MGCP2SI_MSG      (MGCPQRYRSP2BICCQRYRSP+1)

/* message mapping from ISUP to MGCP */
#define SIIAM2MGCPCONREQ        0
#define SIACM2MGCPMDCX          1
#define SIANM2MGCPCONRSP        2
#define SIREL2MGCPRELREQ        3
#define BICCMGCP_CONFWDNOTF2CONNECTED        4
#define BICCXFER2MGCPXFER       5
#define BICCHOLD2MGCPHOLD       6
#define BICCQRY2MGCPQRY         7
#define SIINR2MGCPXXX           8 /* ISUP INR to MGCP Nothing, INFO is sent back to ISUP */
#define CC_MAX_SI2MGCP_MSG      (SIINR2MGCPXXX+1)

/* message mapping from MGCP to ISDN */
#define MGCPADDRCFM2INSETUP     0
#define MGCPALERTING2INALERTING 1
#define MGCPCONCFM2INCONNECT    2
#define MGCPONHOOK2INDIS        3
#define CC_MAX_MGCP2IN_MSG      (MGCPONHOOK2INDIS+1)

/* message mapping from ISDN to MGCP */
#define INSETUP2MGCPCONREQ      0
#define INALERTING2MGCPMDCX     1
#define INCONNECT2MGCPCONRSP    2
#define INDIS2MGCPRELREQ        3
#define CC_MAX_IN2MGCP_MSG      (INDIS2MGCPRELREQ+1)

/* message mapping from MGCP to CAS */
#define MGCPADDRCFM2CSCONREQ    0
#define MGCPALERTING2CSRINGING  1
#define MGCPCONCFM2CSCONRSP     2
#define MGCPONHOOK2CSRELREQ     3
#define CC_MAX_MGCP2CS_MSG      (MGCPONHOOK2CSRELREQ+1)

/* message mapping from CAS to MGCP */
#define CSADDRCFM2MGCPCONREQ    0
#define CSACM2MGCPMDCX          1
#define CSRINGING2MGCPMDCX      1
#define CSCONCFM2MGCPCONRSP     2
#define CSONHOOK2MGCPRELREQ     3
#define CSSUS2MGLNSUS           4
#define CSRES2MGLNRES           5
#define CC_MAX_CS2MGCP_MSG      (CSRES2MGLNRES+1)

/* message mapping from MGCP to SIP */
#define MGCPADDRCFM2SIPINVITE   0
#define MGCPALERTING2SIP18X     1
#define MGCPCONCFM2SIP200OK     2
#define MGCPONHOOK2SIPBYE       3
#define MGCPXFERRSP2SIPXFERRSP  4
#define MGCPHOLDRSP2SIPHOLDRSP  5
#define MGCPQRYRSP2SIPQRYRSP    6
#define CC_MAX_MGCP2SIP_MSG     (MGCPQRYRSP2SIPQRYRSP+1)

/* message mapping from SIP to MGCP */
#define SIPINVITE2MGCPCONREQ    0
#define SIP18X2MGCPMDCX         1
#define SIP200OK2MGCPCONRSP     2
#define SIPBYE2MGCPRELREQ       3
#define SIPXFER2MGCPXFER        4
#define SIPHOLD2MGCPHOLD        5
#define SIPQRY2MGCPQRY          6
#define CC_MAX_SIP2MGCP_MSG     (SIPQRY2MGCPQRY+1)

/* Begin: FID 14221 */
/* message mapping from IVT to SIP */
#define IVT2SIPCONRSP           0
#define CC_MAX_IVT2SIP_MSG      (IVT2SIPCONRSP+1)

/* message mapping from IVT to BICC-SIPT-SI */
#define IVT2SICONRSP           0
#define CC_MAX_IVT2SI_MSG      (IVT2SICONRSP+1)

/* message mapping from BICC-SIPT-SI to IVT */
#define SI2IVTREL              0
#define SI2IVTXFER             1
#define SI2IVTQUERY            2
#define SI2IVTHOLD             3
#define SI2IVTUPDATE           4
#define CC_MAX_SI2IVT_MSG      (SI2IVTUPDATE+1)

/* message mapping from SIP to IVT */
#define SIP2IVTREL              0
#define SIP2IVTXFER             1
#define SIP2IVTQUERY            2
#define SIP2IVTHOLD             3
#define SIP2IVTUPDATE           4
#define CC_MAX_SIP2IVT_MSG      (SIP2IVTUPDATE+1)

/* message mapping from SIP to IVT */

/* End: FID 14221 */

/* FID 15310.1  + */
/* message mapping from ICA to ISUP */
#define ICA2SIIAM            0
#define CC_MAX_ICA2SI_MSG    (ICA2SIIAM + 1)
/* message mapping from ICA to SIP */
#define ICA2SIPINVITE        0
#define CC_MAX_ICA2SIP_MSG   (ICA2SIPINVITE + 1)

/* Message mapping from SIP to ICA */
#define SIPINVITE2ICAINVITE  0
#define SIPXFER2ICAXFER      1
#define SIPHOLD2ICAHOLD      2
#define SIPQRY2ICAQRY        3
#define SIPUPD2ICAUPD        4
#define SIPREFER2ICAREFER    5
#define SIPPRACK2ICAPRACK    6
#define CC_MAX_SIP2ICA_MSG   (SIPPRACK2ICAPRACK + 1)

/* message mapping from ICA to ISDN */
#define ICA2INSETUP          0
#define CC_MAX_ICA2ISDN_MSG    (ICA2INSETUP + 1)
/* FID 15310.1  - */

#ifndef CLOUD
#define CC_MAXCOMNUM         0x00FFFFFF  /* FID15083.0 Maxmum number for CIN */
#else
#define CC_MAXCOMNUM         0x007FFFFF  /* FID15083.0 Maxmum number for CIN */
#endif

#define CC_SUSP_REQUIRED      1
#define CC_SUSP_NOT_REQUIRED  0

#define CC_NO_TMD_RELEASE_REQUIRED     0
#define CC_IC_TMD_RELEASE              1
#define CC_OG_TMD_RELEASE              2
#define CC_CHECK_TMD_RELEASE           3 /* bug#14799 */
#define CC_IC_TMD_RELEASE_CPDI         4


#define CC_CODEC_LIST_MAX     6


/* Parameter compatibility */
 
#define CC_AALPARAM_PCM_H    0x40  /* AAL Parameters High */
#define CC_AALPARAM_PCM_L    0x83  /* AAL Parameters Low */
#define CC_ATMCELLRATE_PCM_H 0x03  /* ATM Cell Rate High */
#define CC_ATMCELLRATE_PCM_L 0x82  /* ATM Cell Rate Low */
#define CC_ACONGLEV_PCM_H    0x51  /* Automatic Congestion Level High */
#define CC_ACONGLEV_PCM_L    0x83  /* Automatic Congestion Level Low */
#define CC_BWDNBIWKIND_PCM_H 0x44  /* Bwd N-band Interworking Indicator High */
#define CC_BWDNBIWKIND_PCM_L 0x80  /* Bwd N-band Interworking Indicator Low */
#define CC_BBEARCAP_PCM_H    0x03  /* Broadband Bearer Capability High */
#define CC_BBEARCAP_PCM_L    0x82  /* Broadband Bearer Capability Low */
#define CC_BHILYRINFO_PCM_H  0x40  /* Broadband High Layer Information High */
#define CC_BHILYRINFO_PCM_L  0x83  /* Broadband High Layer Information Low */
#define CC_BLOLYRINFO_PCM_H  0x00  /* Broadband Low Layer High */
#define CC_BLOLYRINFO_PCM_L  0x83  /* Broadband Low Layer Low */
#define CC_CDPTYNUM_PCM_H    0x43  /* Called Party Number High */
#define CC_CDPTYNUM_PCM_L    0x82  /* Called Party Number Low */
#define CC_CDPTYSAD_PCM_H    0x44  /* Called Party Subaddress High */
#define CC_CDPTYSAD_PCM_L    0x83  /* Called Party Subaddress Low */
#define CC_CDPTYIND_PCM_H    0x40  /* Called Party Indicators High */
#define CC_CDPTYIND_PCM_L    0x82  /* Called Party Indicators Low */
#define CC_CGPTYNUM_PCM_H    0x44  /* Calling Party Number High */
#define CC_CGPTYNUM_PCM_L    0x80  /* Calling Party Number Low */
#define CC_CGPTYSAD_PCM_H    0x44  /* Calling Party Subaddress High */
#define CC_CGPTYSAD_PCM_L    0x83  /* Calling Party Subaddress Low */
#define CC_CGPTYCAT_PCM_H    0x41  /* Calling Party Category High */
#define CC_CGPTYCAT_PCM_L    0x82  /* Calling Party Category Low */
#define CC_CAUSEIND_PCM_H    0x41  /* Cause Indicators High */
#define CC_CAUSEIND_PCM_L    0x80  /* Cause Indicators Low */
#define CC_CNELMTID_PCM_H    0x03  /* Connection Element Identifier High */
#define CC_CNELMTID_PCM_L    0x82  /* Connection Element Identifier Low */
#define CC_DSTSIGID_PCM_H    0x03  /* Destination Signalling Identifier High */
#define CC_DSTSIGID_PCM_L    0x82  /* Destination Signalling Identifier Low */
#define CC_ECHOCTL_PCM_H     0x44  /* Echo Control Information High */
#define CC_ECHOCTL_PCM_L     0x80  /* Echo Control Information Low */
#define CC_FWDNBIWKI_PCM_H   0x44  /* Fwd N-band Interworking Indicator High */
#define CC_FWDNBIWKI_PCM_L   0x80  /* Fwd N-band Interworking Indicator Low
*/
#define CC_NBNDBEARCAP_PCM_H 0x03  /* Narrowband Bearer Capability High */
#define CC_NBNDBEARCAP_PCM_L 0x82  /* Narrowband Bearer Capability Low */
#define CC_NBNDHILYR_PCM_H   0x44  /* Narrowband High Layer Info High */
#define CC_NBNDHILYR_PCM_L   0x80  /* Narrowband High Layer Info Low */
#define CC_NBNDLOLRIN_PCM_H  0x04  /* Narrowband Low Layer Info High */
#define CC_NBNDLOLRIN_PCM_L  0x80  /* Narrowband Low Layer Info Low */
#define CC_NTLINTCLIND_PCM_H 0x44  /* National/internat Call Indicator High */
#define CC_NTLINTCLIND_PCM_L 0x80  /* National/internat Call Indicator Low */
#define CC_NOTIF_PCM_H       0x44  /* Notification */
#define CC_NOTIF_PCM_L       0x80  /* Notification High */
#define CC_ORGCDNUM_PCM_H    0x44  /* Original Called Number Low High */
#define CC_ORGCDNUM_PCM_L    0x80  /* Original Called Number Low Low */
#define CC_ORIGSIGID_PCM_H   0x03  /* Originating Signalling Identifier High */
#define CC_ORIGSIGID_PCM_L   0x82  /* Originating Signalling Identifier Low */
#define CC_PROGINDIC_PCM_H   0x44  /* Progress Indicator High */
#define CC_PROGINDIC_PCM_L   0x80  /* Progress Indicator Low */
#define CC_REDGNUM_PCM_H     0x44  /* Redirecting Number High */
#define CC_REDGNUM_PCM_L     0x80  /* Redirecting Number Low */
#define CC_REDINFO_PCM_H     0x44  /* Redirection Information */
#define CC_REDINFO_PCM_L     0x80  /* Redirection Information */
#define CC_SUSPRSMIND_PCM_H  0x44  /* Suspend/Resume Indicators High */
#define CC_SUSPRSMIND_PCM_L  0x80  /* Suspend/Resume Indicators Low */
#define CC_USR2USRIND_PCM_H  0x44  /* User to User Indicators High */
#define CC_USR2USRIND_PCM_L  0x80  /* User to User Indicators Low */
#define CC_USR2USRINFO_PCM_H 0x44  /* User to User Information High */
#define CC_USR2USRINFO_PCM_L 0x80  /* User to User Information Low */
#define CC_AATMCELLR_PCM_H   0x41  /* Additional ATM Cell Rate High */
#define CC_AATMCELLR_PCM_L   0x83  /* Additional ATM Cell Rate Low */
#define CC_QUALOFSERV_PCM_H  0x41  /* Quality of Service High */
#define CC_QUALOFSERV_PCM_L  0x83  /* Quality of Service Low */
#define CC_ORGLINEIN_PCM_H   0x44  /* T1.113.3 Originating Line Info High */
#define CC_ORGLINEIN_PCM_L   0x80  /* T1.113.3 Originating Line Info Low
*/
#define CC_INBNDINFO_PCM_H   0x40  /* In-band Information Indicator */
#define CC_INBNDINFO_PCM_L   0x83  /* In-band Information Indicator */
#define CC_SUBSEQNUM_PCM_H   0x40  /* subsequent number */
#define CC_SUBSEQNUM_PCM_L   0x83  /* subsequent number */
#define CC_RESIND_PCM_H      0x03
#define CC_RESIND_PCM_L      0x82
#define CC_PRDLYCNTR_PCM_H   0x03  /* Propagation Delay Counter */
#define CC_PRDLYCNTR_PCM_L   0x82  /* Propagation Delay Counter */
#define CC_PROP_PCM_H        0x40  /* Proprietary Parameters */
#define CC_PROP_PCM_L        0x83  /* Proprietary Parameters */

#ifdef SS7_SIANS2BIITUN
#define CC_GENADDR_PCM_H     0x40  /* Generic Address */
#define CC_GENADDR_PCM_L     0x83  /* Generic Address */
#define CC_GENDIGITS_PCM_H   0x40  /* Generic Digits */
#define CC_GENDIGITS_PCM_L   0x83  /* Generic Digits */
#define CC_CARRIERID_PCM_H   0x40  /* Carrier identification */
#define CC_CARRIERID_PCM_L   0x83  /* Carrier identification */
#define CC_CARSELINF_PCM_H   0x40  /* Carrier Select Information */
#define CC_CARSELINF_PCM_L   0x83  /* Carrier Select Information */
#define CC_CHARGENUM_PCM_H   0x40  /* Charge Number */
#define CC_CHARGENUM_PCM_L   0x83  /* Charge Number */
#define CC_JURISINF_PCM_H    0x40  /* Jurisdiction Information */
#define CC_JURISINF_PCM_L    0x83  /* Jurisdiction Information */

#define CC_USRNWIWINDR_PCM_H 0x40  /* User Network Interwork Indr */
#define CC_USRNWIWINDR_PCM_L 0x83  /* User Network Interworking Indr */
#endif

#if ( defined(SS7_SIITU2BIITU) || defined(SS7_SI7672BIITU) )
#define CC_ORGISPTECDE_PCM_H     0x44  /* Origination ISC point code High */
#define CC_ORGISPTECDE_PCM_L     0x80  /* Origination ISC point code High */
#define CC_ADDCNNUM_PCM_H        0x44  /* Additional Connected Number */
#define CC_ADDCNNUM_PCM_L        0x80  /* Additional Connected Number */
#define CC_MLPPPREC_PCM_H        0x44  /* MLPP Precedence */
#define CC_MLPPPREC_PCM_L        0x80  /* MLPP Precedence */
#define CC_CHARGEIND_PCM_H       0x44  /* Charge Indicator */
#define CC_CHARGEIND_PCM_L       0x80  /* Charge Indicator */
#define CC_CUGINFO_PCM_H         0x44  /* CUG Information */
#define CC_CUGINFO_PCM_L         0x80  /* CUG Information */
#define CC_CNLNIDREQ_PCM_H       0x44  /* Connected Line ID Request */
#define CC_CNLNIDREQ_PCM_L       0x80  /* Connected Line ID Request */
#define CC_REDIRNUM_PCM_H        0x44  /* Redirection Number */         
#define CC_REDIRNUM_PCM_L        0x80  /* Redirection Number */         
#define CC_REDIRREST_PCM_H       0x44  /* Redirection Number Restriction*/         
#define CC_REDIRREST_PCM_L       0x80  /* Redirection Number Restriction*/         
#define CC_CLDVRSNINFO_PCM_H     0x44  /* Call Diversion Information */
#define CC_CLDVRSNINFO_PCM_L     0x80  /* Call Diversion Information */
#define CC_CLDVRSNMO_PCM_H       0x44  /* Call Diversion May Occur */
#define CC_CLDVRSNMO_PCM_L       0x80  /* Call Diversion May Occur */
#define CC_MLPPUSRINFO_PCM_H     0x44  /* MLPP User Information */
#define CC_MLPPUSRINFO_PCM_L     0x80  /* MLPP User Information */
#define CC_ADDCGPTYNUM_PCM_H     0x44  /* Additional Calling Party Number */
#define CC_ADDCGPTYNUM_PCM_L     0x80  /* Additional Calling Party Number */
#define CC_CNPTYNUM_PCM_H        0x44  /* Connected Number */
#define CC_CNPTYNUM_PCM_L        0x80  /* Connected Number */
#define CC_CONNSAD_PCM_H         0x44  /* Connected Party Subaddress High */
#define CC_CONNSAD_PCM_L         0x83  /* Connected Party Subaddress Low */
#define CC_LOCNUM_PCM_H          0x44  /* Location Number */
#define CC_LOCNUM_PCM_L          0x80  /* Location Number */
#define CC_ACCDELINFO_PCM_H      0x44  /* Access Delivery Information */
#define CC_ACCDELINFO_PCM_L      0x80  /* Access Delivery Information */
#define CC_CLLHISTRY_PCM_H      0x44  /* Call History Information */
#define CC_CLLHISTRY_PCM_L      0x80  /* Call History Information */
#endif /* defined(SS7_SIITU2BIITU) || defined(SS7_SI7672BIITU) ) */
#define ME_AUTOCONGLVL    0x27   /* Automatic congestion */
#define ME_TRANNETSEL     0x23   /* Transit Network Selection */
#define ME_CALREF         0x01   /* Call reference */
#define ME_REDIRGNUM      0x0b   /* Redirecting number */
#define ME_CHARGENUM      0xeb   /* Charge number */
#define ME_GENDIGITS      0xc1   /* Generic Digits */
#define ME_SERVCODE       0xec   /* service code indicator */
#define ME_SERVACTA       0xe2   /* Service Activation */
#define ME_BUSINESSGRP    0xc6   /* Business group */
#define ME_CARRIERID      0xc5   /* Carrier Idenification */
#define ME_CARSELINF      0xee   /* Carrier Selection Information */
#define ME_EGRESS         0xc3   /* Egress Service */
#define ME_GENADDR        0xc0   /* Generic Adress */
#define ME_JURISINF       0xc4   /* Jurisdiction Information */
#define ME_NETTRANS       0xef   /* Network Transport */
#define ME_SPECPROCREQ    0xed   /* Special Processing Request */
#define ME_TRANSREQ       0xe3   /* Transaction Request */
#define ME_OPBACKCALLIND  0x29   /* Optional backward call indicator */
#define ME_NBNHLINFO      0x7D   /* Narrowdband High Layer Compatibility */
#define ME_NBNLLINFO      0x7C   /* Narrowdband Low Layer Compatibility */
#define ME_USR2USR        0x7E   /* User-user */
#define ME_PROGIND        0x1E   /* Progress Indicator */
#define ME_CDPTYSAD       0x71   /* Called Party Subaddress  */
#define ME_CONSAD         0x4d   /* Connected Subaddress  */
#define ME_CGPTYSAD       0x6D   /* Calling Party Subaddress  */
#define ME_NBBEARCAP      0x04   /* Narrowdband Bearer Capability */
#define ME_ORGLINEINF     0xea   /* Original line information */
#define ME_NATCONIND      0x06   /* Nature of connection indicators */
#define ME_FWDCALLIND     0x07   /* Forward call indicator */
#define ME_CALGPARTCAT    0x09   /* Calling party category */
#define ME_USRSERVINFO    0x1d   /* User service information */
#define ME_CALDPARTNUM    0x04   /* Called party's number */
#define ME_CALGPARTNUM    0x0a   /* Calling party number */
#define ME_ORIGCALDNUM    0x28   /* Original Called Number */
#define ME_ORIGCALDINNUM  0x7f   /* Original Called IN Number */
#define ME_USR2USRINFO    0x20   /* User to User information */
#define ME_REDIRINFO      0x13   /* Redirection information */
#define ME_ORISCCDE       0x29   /* originating ISC point code */
#define ME_GENNMB         0xc0   /* generic number */
#define ME_MLPPPREC       0x3a   /* MLPP precedence */
#define ME_USR2USRIND     0x2a   /* User to User indicators */
#define ME_OPFWDCALLIND   0x08   /* Optional foreward call indicator */
#define ME_USRSERVINFOPR  0x30   /* User service information prime */
#define ME_CLSDUGRPINTCDE 0x1a   /* Closed user group interlock code */
#define ME_LOCNMB         0x3f   /* location number */
#define ME_PROPDLYCTR     0x31   /* propagation delay counter */
#define ME_BACKCALLIND    0x11   /* Backward call indicators */
#define ME_NOTIFIND       0xe1   /* Notification Indicator */
#define ME_CAUSIND        0x12   /* Cause indicator */
#define ME_REDIRNUM       0x0c   /* Redirection number */
#define ME_REDIRRESTR     0x40   /* redirection restriction */
#define ME_CLLDIVERS      0x36   /* call Diversion information */
#define ME_TRANSMEDUSD    0x35   /* Transmission media used */
#define ME_TRANSMEDREQ    0x02   /* Transmission medium requirement */
#define ME_ACCDELINFO     0x2e   /* access delivery information */
#define ME_EVNTINFO       0x24   /* Event inforamtion */
#define ME_CONNUMB        0x21   /* Connected number */
#define ME_CLLHISTORY     0x2d   /* call history information */
#define  ME_MOREDATA      0xa0   /* More Data */


#if ( defined(SS7_SIITU2BIITU) || defined(SS7_SI7672BIITU) )
#define  ME_CONNSAD         0x4d   /* Connected Subaddress */
#define  ME_ECHOCNTRL       0x37   /* Echo Control */
#endif /* defined(SS7_SIITU2BIITU) || defined(SS7_SI7672BIITU) ) */

/* Information Element ID indecies - used in cc_mf.c/cc_db.c */
#define MEI_MOREDATA       0x06   /* more data */
#define MEI_ACCTPORT      0x03   /* Access transport */
#define MEI_AUTOCONGLVL   0x04   /* Automatic congestion level */
#define MEI_BACKCALLIND   0x05   /* Backward call indicators */
#define MEI_CALLMODIND    0x06   /* Call modification indicators */
#define MEI_CALREF        0x07   /* Call reference */
#define MEI_CALDPARTNUM   0x08   /* Called party's number */
#define MEI_CALGPARTNUM   0x09   /* Calling party number */
#define MEI_CALGPARTCAT   0x0a   /* Calling party category */
#define MEI_CAUSIND       0x0b   /* Cause indicator */
#define MEI_CGRPSUPMTYPIND 0x0c  /* Circuit group supervision msg type */
#define MEI_CIRCSTEIND    0x0d   /* Cicuit state indicator */
#define MEI_CLSDUGRPINTCDE 0x0e  /* Closed user group interlock code */
#define MEI_CONNUMB       0x0f   /* Connected number */
#define MEI_CONREQ        0x10   /* Connection request */
#define MEI_CONTIND       0x11   /* Continuity indicators */
#define MEI_ENDOP         0x12   /* End of optional parameters */
#define MEI_EVNTINFO      0x13   /* Event inforamtion */
#define MEI_FACIND        0x14   /* Facility indicator */
#define MEI_FWDCALLIND    0x15   /* Forward call indicator */
#define MEI_INFOIND       0x16   /* Information indicators */
#define MEI_INFOREQIND    0x17   /* Information request indicators */
#define MEI_NATCONIND     0x18   /* Nature of connection indicators */
#define MEI_OPBACKCALLIND 0x19   /* Optional backward call indicator */
#define MEI_OPFWDCALLIND  0x1a   /* Optional foreward call indicator */
#define MEI_ORIGCALDNUM   0x1b   /* Original Called Number */
#define MEI_RANGSTAT      0x1c   /* Range and Status */
#define MEI_REDIRGNUM     0x1d   /* Redirecting number */
#define MEI_REDIRINFO     0x1e   /* Redirection information */
#define MEI_REDIRNUM      0x1f   /* Redirection number */
#define MEI_SIGPTCDE      0x20   /* Signal point code */
#define MEI_SUBSEQNUM     0x21   /* Subsequent number */
#define MEI_SUSPRESIND    0x22   /* Suspend/Resume indicators */
#define MEI_TRANNETSEL    0x23   /* Transit network selection */
#define MEI_TRANSMEDREQ   0x24   /* Transmission medium reuirement */
#define MEI_USRSERVINFO   0x25   /* User service information */
#define MEI_USR2USRIND    0x26   /* User to User indicators */
#define MEI_USR2USRINFO   0x27   /* User to User information */

#define MEI_ACCDELINFO    0x28   /* access delivery information */
#define MEI_CLLDIVERS     0x29   /* call Diversion information */
#define MEI_CLLHISTORY    0x2a   /* call history information */
#define MEI_ECHOCNTRL     0x2b   /* echo control */
#define MEI_NOTIFINDC     0x2c   /* generic nnotification */
#define MEI_GENNMB        0x2d   /* generic number */
#define MEI_MCIDREQ       0x2e   /* MCID request indicators */
#define MEI_MCIDRSP       0x2f   /* MCID response indicators */
#define MEI_MSGCOMP       0x30   /* message compatibility info */
#define MEI_MLPPPREC      0x31   /* MLPP precedence */
#define MEI_NETSPFAC      0x32   /* network specific facility */
#define MEI_ORISCCDE      0x33   /* originating ISC point code */
#define MEI_PARCOMPIN     0x34   /* parameter compatibility info */
#define MEI_PROPDLYCTR    0x35   /* propagation delay counter */
#define MEI_REDIRRESTR    0x36   /* redirection restriction */
#define MEI_REMOTOPER     0x37   /* remote operations */
#define MEI_TRANSMEDPRM   0x38   /* Transmission media prime */
#define MEI_TRANSMEDUSD   0x39   /* Transmission media used */
#define MEI_USRSERVINFOPR 0x3a   /* User service information prime */

#define MEI_CHARGENUM     0x3b  /* Charge number */
#define MEI_CGRPCHARIND   0x3c  /* Circuit group characteristic ind. */
#define MEI_CIRIDNAME     0x3d  /* Circuit ID name */
#define MEI_CIRVALRSPIND  0x3e  /* Circuit validation response ind. */
#define MEI_CLSDUGRPCHKRSP 0x3f  /* Closed user group check response */
#define MEI_CLLI           0x40  /* Common language location ind. */
#define MEI_FACINFIND      0x41  /* Facility information indicator */
#define MEI_INDEX          0x42  /* Index */
#define MEI_ORGLINEINF     0x43  /* Original line information */
#define MEI_OUTGTRKGRPNUM  0x44  /* Outgoing trunk group number */

/* Information Element ID indecies - ANSI 92 specific */

#define MEI_BUSINESSGRP    0x45  /* Business group */
#define MEI_CARRIERID      0x46  /* Carrier Idenification */
#define MEI_CARSELINF      0x47  /* Carrier Selection Information */
#define MEI_EGRESS         0x48  /* Egress Service */
#define MEI_GENADDR        0x49  /* Generic Adress */
#define MEI_GENDIGITS      0x4a  /* Generic Digits */
#define MEI_JURISINF       0x4b  /* Jurisdiction Information */
#define MEI_NETTRANS       0x4c  /* Network Transport */
#define MEI_NOTIFIND       0x4d  /* Notification Indicator */
#define MEI_SERVACTA       0x4e  /* Service Activation */
#define MEI_SPECPROCREQ    0x4f  /* Special Processing Request */
#define MEI_TRANSREQ       0x50  /* Transaction Request */
#define MEI_SERVCODE       0x68   /* service code indicator */

/* Information Element ID tags - SINGTEL specific */

#define MEI_CHRGEINFO      0x51   /* Call charge info */
#define MEI_CHRGERTEINFO   0x52   /* Charge rate info */
#define MEI_TRNKOFF        0x53   /* Trunk offering info */

/* Information Element ID tags - Q767 Italy specific */

#define MEI_FWDVAD         0x54   /* Forward vad indicator */
#define MEI_BACKVAD        0x55   /* Backward vad indicator */

/* Information Element ID tags - ETSI specific */

#define MEI_CALLTRNSFRNMB  0x56   /* Call Transfer Number */
#define MEI_CALLTRNSFRREF  0x57   /* Call Transfer Reference */
#define MEI_CCBSPARAM      0x58   /* CCBS Parameter */
#define MEI_LOOPPRVNTIND   0x59   /* Loop Prevention Indicator */
#define MEI_FREEPHIND      0x5a   /* Free Phone Indicator */
#define MEI_USRTSRVINFO    0x5b   /* User Teleservice Information */

/* ITU Element ID tag */

#define MEI_SERVACT        0x5c   /* Service Activation */

/* Information Element ID tags - GT_FTZ specific */

#define MEI_GTNPSSCPRICLS  0x5d   /* Nat. Par.: subscriber priority class */
#define MEI_GTNATPARFF     0x5e   /* Nat. Par.: FF (multitype) */
#define MEI_GTNPCHRGINF    0x5f   /* Nat. Par.: charging information */
#define MEI_GTNPSSP        0x60   /* Nat. Par.: service switching point */
#define MEI_GTNPINCDPNO    0x61   /* Nat. Par.: intelligent network CdPNo */
#define MEI_GTNPSPV        0x62   /* Nat. Par.: semipermanent connection */
#define MEI_GTNPEXCHTRKID  0x63   /* Nat. Par.: exchange and trung ID */
#define MEI_GTNPTLN2TLNSG  0x64   /* Nat. Par.: Tln to Tln signalling */
#define MEI_GTNPCHGPTYID   0x65   /* Nat. Par.: charged party identification */
#define MEI_GTFACINDINFOR  0x66   /* Facility Indicator Information */
#define MEI_GTNPUKK        0x67   /* Nat. Par.: originating party information */

/* Info Element ID tags */
#define MEI_LOCNMB         0x68   /* location number */
/* last 0x69 */

#define MEI_ORIGCALDINNUM  0xb2   /* Original Called IN Number */

#define CC_SI_MAX_SWITCH      8  /* maximum number of ISUP protocol switches */

#define BIME_DSTSIGID      0x03   /* Destination Signalling Identifier */
#define BIME_CGPTYNM       0x0a   /* Calling Party Number  */
#define BIME_CUGINFO       0x1a   /* CUG Information  */
#define BIME_LOCNUM        0x3f   /* Location Number */
#define BIME_MAXETEDLAY    0x07   /* Maximum End-to-end Transit Delay */
#define BIME_MLPPPRECED    0x3a   /* MLPP Precedence */
#define BIME_OAMTRFCDSCR   0x48   /* OAM Traffic Descriptor */
#define BIME_ORGISCPTCDE   0x2b   /* Original ISC Point Code */
#define BIME_PRDLYCNTR     0x31   /* Propagation Delay Counter */
#define BIME_SEGMIND       0x3e   /* Segmentation Indicator */
#define BIME_TRANNETSEL    0x23   /* Transit Network Selection */
#define BIME_ACCDELINFO    0x2e   /* Access Delivery Info */
#define BIME_CLDVRINFO     0x36   /* Call Diversion Info  */
#define BIME_CLDVRPSBL     0x26   /* Call Diversion May Occur  */
#define BIME_CHGEIND       0x19   /* Charge Indicators  */
#define BIME_INBNDINFO     0x1f   /* In-band Information Indicator */
#define BIME_MLPPUSRINFO   0x49   /* MLPP User Info */
#define BIME_REDIRNUMRST   0x40   /* Redirection Number Restriction */
#define BIME_SEGMIND       0x3e   /* Segmentation Indicator */
#define BIME_ADDCNPTYNM    0x11   /* Additional Connected Number */
#define BIME_CLHSTINFO     0x2d   /* Call History Info  */
#define BIME_CNNUM         0x21   /* Connected Number */
#define BIME_CNSAD         0x24   /* Connected Subaddress */
#define BIME_OLI           0xea   /* T1.113.3 - Originating Line Information */
#define BIME_REDIRNUM      0x0c   /* Redirection Number */
#define BIME_NBNHLINFO     0x34   /* Narrowdband High Layer Compatibility */
#define BIME_NBNLLINFO     0x25   /* Narrowdband Low Layer Compatibility */
#define BIME_PROGIND       0x35   /* Progress Indicator */
#define BIME_CDPTYSAD      0x15   /* Called Party Subaddress  */
#define BIME_CGPTYSAD      0x16   /* Calling Party Subaddress  */
#define BIME_NBBEARCAP     0x1d   /* Narrowdband Bearer Capability */
#define BIME_USRNWIWINDR   0xc9   /* User Network Interworking Indicator */

/* information element id's */

/* message information elements */

#define AM_ME_NBEARCAP        0x04   /* Narrowband Bearer Capability */
#define AM_ME_PROGIND         0x1e   /* Progress Indicator */
#define AM_ME_CGPTYSAD        0x6d   /* Calling Party Sub Address */
#define AM_ME_CDPTYSAD        0x71   /* Called Party Sub Address */
#define AM_ME_NLOLYRCOMP      0x7c   /* Narrowband Lower Layer Compatibility */
#define AM_ME_NHILYRCOMP      0x7d   /* Narrowband Higher Layer Compatibility */

/* message element indices */

#define AM_MEI_CGPTYSAD       0x11   /* Calling Party Sub Address */
#define AM_MEI_CDPTYSAD       0x13   /* Called Party Sub Address */
#define AM_MEI_NBEARCAP       0x16   /* Narrowband Bearer Capability */
#define AM_MEI_NHILYRCOMP     0x17   /* Narrowband Higher Layer Compatibility */
#define AM_MEI_NLOLYRCOMP     0x18   /* Narrowband Lower Layer Compatibility */
#define AM_MEI_PROGIND        0x1a   /* Progress Indicator */

#define AM_INV_ME_LEN            0xff /* invalid Message element length */
#define AM_CGPTYSAD_MINLEN         1    /* Calling party subaddress */
#define AM_CGPTYSAD_MAXLEN        21    /* Calling party subaddress */

#define AM_CDPTYSAD_MINLEN         1    /* Called party subaddress */
#define AM_CDPTYSAD_MAXLEN        21    /* Called party subaddress */

#define AM_PROGIND_MINLEN          1    /* Progress indicator */
#define AM_PROGIND_MAXLEN          2    /* Progress indicator */

#define AM_NBEARCAP_MINLEN         2    /* Narrowband bearer capability */
#define AM_NBEARCAP_MAXLEN        10    /* Narrowband bearer capability */

#define AM_NHILYRCOMP_MINLEN       1    /* Narrowband high layer compatibility */
#define AM_NHILYRCOMP_MAXLEN       2    /* Narrowband high layer compatibility */

#define AM_NLOLYRCOMP_MINLEN       2    /* Narrowband low layer compatibility */
#define AM_NLOLYRCOMP_MAXLEN      15    /* Narrowband low layer compatibility */

#define AM_PROGIND_MINLEN          1    /* Progress indicator */
#define AM_PROGIND_MAXLEN          2    /* Progress indicator */

/* UNI information element identifier */
#define UNI_NHILYRINFO     0x7d   /* Narrowband Higher Layer Compatibility */
#define UNI_NLOLYRINFO     0x7c   /* Narrowband Lower Layer Compatibility */
#define UNI_PROGIND        0x1e   /* Progress Indicator */
#define UNI_NBBEARCAP      0x04   /* Narrowband Bearer Capability */


/* defines for the default traffic descriptors */
/* ATM Cell Rate */
#define CC_ACR_FPCR_ID0         0x02
#define CC_ACR_BPCR_ID0         0x03
#define CC_ACR_FPCR_ID1         0x04
#define CC_ACR_BPCR_ID1         0x05

/* Additional and Alternative ATM cell rate values (Q.2723.1 and Q.2725.1) */

#define CC_AACR_FPCR_ID0         0x01
#define CC_AACR_BPCR_ID0         0x02
#define CC_AACR_FPCR_ID1         0x04
#define CC_AACR_BPCR_ID1         0x05

#define CC_AACR_FSCR_ID0         0x08
#define CC_AACR_BSCR_ID0         0x09
#define CC_AACR_FSCR_ID1         0x10
#define CC_AACR_BSCR_ID1         0x11

#define CC_AACR_FMBS_ID0         0x20
#define CC_AACR_BMBS_ID0         0x21
#define CC_AACR_FMBS_ID1         0x30
#define CC_AACR_BMBS_ID1         0x31

/* Additional ATM Cell rate Values (Q.2723.3 and Q.2723.4) only */
#define CC_AACR_FAMCR_ID1        0x12  /* Q.2723.3 Forward ABR min Cell (0+1)*/
#define CC_AACR_BAMCR_ID1        0x13  /* Q.2723.3 Backward ABR min Cell (0+1)*/
#define CC_AACR_FRMPCR_ID        0x40  /* Q.2723.4 Fwd Resource Mngmt PCR Id */
#define CC_AACR_BRMPCR_ID        0x41  /* Q.2723.4 Bkwd Resource Mngmt PCR Id*/

/* defines for transmission medium requirement in traffic descriptor */
#define TMR_SPEECH      0x00
#define TMR_64KBITS     0x02
#define TMR_31KHZ       0x03
#define TMR_2X64KBITS   0x07
#define TMR_384KBITS    0x08
#define TMR_1536KBITS   0x09
#define TMR_1920KBITS   0x0a

/* define for event type for encoding/decoding access transport */
#define CC_EVCONN        0        /* Connection Event */
#define CC_EVCNST        1        /* Connection Status Event */
#define CC_EVINFO        2        /* Information Event */

/* Hash defines for the values of Notification Indicator information elements */
#define CC_USRSUSP       0x00    /* user suspend */
#define CC_USRRESM       0x01    /* user resume */
#define CC_CONFEST       0x42    /* conference established */
#define CC_CONFCLRD      0x43    /* conference cleared */
#define CC_OTHPTYADD     0x44    /* other party added */
#define CC_ISOLATED      0x45    /* isolated */
#define CC_REATTCHD      0x46    /* reattached */
#define CC_OTHPTYISLT    0x47    /* other party isolated */
#define CC_OTHPTYREATCH  0x48    /* other party reattached */
#define CC_OTHPTYSPLT    0x49    /* other party split */
#define CC_OTHPTYDISC    0x4a    /* other party disconnected */
#define CC_CONFFLTING    0x4b    /* conference floating */
#define CC_3PTYRMTHLD    0x7b    /* remote holding for 3PTY call */
#define CC_CALLDIVT      0x7b    /* call is diverting */
#define CC_WAITGCALL     0x60    /* call is waiting */
#define CC_CALLTRANACT   0x6a    /* call transfer active */
#define CC_RMTEHOLD      0x79    /* remote hold */
#define CC_RMTERETR      0x7a    /* remote retrieve */
/* Progress Descriptor, should be move to sit.h later */
#define PD_ITWOCC        0x05    /* interworking occured for progress indicator */

/* Hash Defines required in ISUP, Should be moved to sit.h later */
#define COLPREQ          1       /* COLP requested */
/* Hash define for Access Delivery I.E. to be moved to sit.h later */
#define SETUPGENRTD      0       /* SETUP generated for access delivery info */ 
/* Hash define for Calling party Category, to be moved to sit.h later */
#define CAGCAT_NOIND     0
/* Hash define for Inband information indicator in BckCall Indicator
   to be moved to sit.h later */
#define INBAND_NOIND     0
/* Hash define for Connected Line Id request Indicator in Option forward
   indicator, to be moved into sit.h later */
#define CLIDREQ_NOIND    0

/* Hash defines for default coding standard and IE instruction field value
   for PNNI parameters */
#define AM_CGSTD_IEINSTRUCT_DEF_VALUE 0x99

/* Continuity check related hsh defines */

/* Hash defines for xchg_typ in CcEchoCntrl strucutre */
#define  CC_INVALID_XCHG         0
#define  CC_ORIG_XCHG            1
#define  CC_TERM_XCHG            2
#define  CC_INTER_XCHG           3

/* Hash defines for echo control flag */
#define CC_OG_ECHOCNTRL          0x01
#define CC_IC_ECHOCNTRL          0x02
#define CC_IC_SIMECHO            0x04
#define CC_IC_ITU92ECHO          0x08
#define CC_OG_SIMECHO            0x10
#define CC_OG_ITU92ECHO          0x20
#define CC_IC_ECHO_REQ           0x40
#define CC_OG_ECHO_REQ           0x80
#define CC_TMR_T37               0x100
#define CC_IC_ECHO_INCL          0x200
#define CC_OG_ECHO_INCL          0x400
#define CC_ACM_SENT              0x800

/* Propagation delay allowed for echo control
 * puposes, this hash define should be changed in
 * case different delay value is required before echo
 * control kicks in */
#define CC_PROP_DELAY_ALLWD      25

#define CC_CONTINUE              0x01
#define CC_PROC                  0x02
#define CC_DROP                  0x03

#ifdef CC_AIN
/* Bug# 10273 */
/* #defines for the structure CcNewEchoParam */
/* xchg_typ takes values defined above, such as CC_INVALID_XCHG,
 * CC_ORIG_XCHG , CC_TERM_XCHG , CC_INTER_XCHG */ 

/* For the field IngressEchoVal */
#define CC_OG_HALF_ECOCNTRL_DEV_ENAB 0x01 /* Valid for Orig, Inter and Term Exch */
#define CC_IC_IAM_ECHO_IND_SET       0x02 /* Valid for Inter and Term exchange */
#define CC_OG_BKWD_ECHO_IND_SET      0x04 /* Valid for Inter and Term Exchange */ 
#define CC_INGRESS_ECHO_RSC_AVAL     0x08 /* Valid for Orig, Inter Exchange */

/* For the field  EgressEchoVal */
#define CC_IC_HALF_ECOCNTRL_DEV_ENAB 0x01 /* Valid for Orig, Inter and Term Exch */
#define CC_OG_IAM_ECHO_IND_SET       0x02 /* Valid for Orig. and Inter exchange */ 
#define CC_IC_BKWD_ECHO_IND_SET      0x04 /* Valid for Orig. and Inter exchange */ 
#define CC_EGRESS_ECHO_RSC_AVAL      0x08 /* Valid for Term. and Inter exchange */

/* For the field EnhEchoLogicVal, A.2.1/Q.115 */
#define CC_ECIF_O_I           0x00000001  /* 0:o.n.i., 1:o.i. */
#define CC_ECIF_O_R           0x00000002  /* 0:o.n.r., 1:o.r. */
#define CC_ECIB_I_I           0x00000004  /* 0:i.n.i., 1:i.i. */
#define CC_ECRF_I_R           0x00000008  /* 0:i.n.r., 1:i.r. */
#define CC_ECRF_O_R           0x00000010  /* 0:o.n.r., 1:o.r. */
#define CC_ECRB_I_R           0x00000020  /* 0:i.n.r., 1:i.r. */
#define CC_ECRB_O_R           0x00000040  /* 0:o.n.r., 1:o.r. */
#define CC_ECIFA_O_A          0x00000080  /* 0:o.n.a., 1:o.a. */
#define CC_ECIBA_I_A          0x00000100  /* 0:i.n.a., 1:i.a. */
#define CC_ECRRF_I_X          0x00000200  /* 0:i.n.x., 1:i.x. */
#define CC_ECRRF_O_X          0x00000400  /* 0:o.n.x., 1:o.x. */
#define CC_ECRRB_I_X          0x00000800  /* 0:i.n.x., 1:i.x. */
#define CC_ECRRB_O_X          0x00001000  /* 0:o.n.x., 1:o.x. */
#define CC_EC_INVOKE          0x00002000  /* TMR = speech/3.1khz */
#define CC_INGRESS_MGI_MODIFY 0x00004000  /* Orig ccs needs to send MODIFY to MGI */
#define CC_EGRESS_MGI_MODIFY  0x00008000  /* Term ccs needs to send MODIFY to MGI */

#define CC_ECHCDEV_NOINFO      0x00
#define CC_ECHCDEV_NOTINCL     0x01
#define CC_ECHCDEV_INCL        0x02
#define CC_ECHDEV_AVAILNOTINCL 0x03

#define CC_DEVREQ_NOINFO       0x00
#define CC_DEVREQ_REQ          0x01

#endif

/* define for max digits to seize for open numbered route */
#define CC_MAX_DGTS_SEIZE        16
/* Maximum cause values */
#define CC_MAX_CAUSE_VAL         127

#define CC_IGNORE_CAUSE_MAP  0xFF  /* ignore cause map */


#define MAX_DIGITS_IN_INTEGER 20

#define PRESUBIND_PRESUBSCRIBED  0x01
#define PRESUBIND_CASUAL         0x02  
#define PRESUBIND_BOTH           0x00
#define CARR_CLASS_INTERLATA     0x01
#define CARR_CLASS_INTRALATA     0x02
#define CARR_CLASS_BOTH          0x00
#define CARR_OPER_SUP_YES        0x01
#define CARR_OPER_SUP_NO         0x00

#define INVALID_NUMBER           0x10
#define INVALID_ONE_PREFIX       0x11
#define INVALID_CIC_SPECIFIED    0x12
#define INVALID_CATEGORY_CIC     0x13
#define NEED_ONE_PREFIX          0x14
#define INVALID_LOCAL_NUMBER     0x15
#define INVALID_ROUTE            0x16
#define UNALLOCATED_NUMBER       0x17
#define CALL_REJECTED            0x18
#define INVALID_ZERO_PREFIX      0x19
#define CC_VACANT_CODE           0x1a
#define INCOMPLETE_NUMBER        0x1b
#define PRESUB_CARRIER_NEEDED    0x1c
#define NO_ROUTE_DESTINATION     0x1d
#define NA_TG_FAILED             0x1e

#define INTERNATIONAL_NUMBER     0x20
#define SUBSCRIBER_EMERGENCY      0x23
#define SUBSCRIBER_DIR_ASSISTANCE 0x24
#define SUBSCRIBER_N11            0x25
#define SUBSCRIBER_CAC_CALL       0x26
#define SUBSCRIBER_INTER_LATA     0x27
#define SUBSCRIBER_INTRA_LATA     0x28
#define SUBSCRIBER_LOCAL_NUMBER   0x29
#define ON_SWITCH_NUMBER          0x2a
#define TOLL_FREE_NUMBER          0x2b
#define SUBSCRIBER_555_DIR_ASSISTANCE 0x2c
#define SUBSCRIBER_VSC_CODE       0x2d
#define SUBSCRIBER_AIN_CODE       0x2e
#define LOCAL_RATE_NUMBER         0x2f
#define PREMIUM_RATE_CALL         0x30
#define CALLTYPE_UNKNOWN          0x31
#define TOLL_DIR_ASSIST           0x32
#define NO_NUMBER_OPERATOR        0x40
#define INTER_LATA_OPERATOR       0x41
#define INTRA_LATA_OPERATOR       0x42
#define LOCAL_NUMBER_OPERATOR     0x44
#define INTERNATIONAL_OPERATOR    0x45
#define NATIONAL_NUMBER           0x46
#define OPR_TOLL_FREE_NUMBER      0x47
#define CUSTOMIZED_DIAL_PLAN      0x48
#define NATIONAL_OPERATOR         0x49
#define INTER_LATA_CARRIER_OPERATOR 0x4a
#define SUB_DIR_ASSISTANCE_OPREQ  0x4b
#define SUBSCRIBER_555_DIR_ASSISTANCE_OPREQ 0x4c


/* TELICA-VKG-06/08/2001-BUG#4572 */
#define IVR_ROUTE_CATEGORY        0x50
/* End-VKG */

#define NO_NUMBER_CUT_THROUGH     0x43

#define TON_NONMMBR_OPERATOR      0x30
#define TON_INT_OPERATOR          0x31
#define TON_NAT_OPERATOR          0x32
#define TON_950_NUMBER            0x33
#define TON_SUBS_OPERATOR         0x34


#define ACTIVE_SUBSCRIBER         0x52
#define NUMBER_CHANGED            0x53

#define EQUIPMENT_TROUBLE         0x55

#define RT_EQUIPMENT_TROUBLE      0x60
#define RT_INVALID_SUBSCRIBER     0x61
#define RT_FAILED                 0x62
#define RT_NUMBER_CHANGED         0x64
#define RT_NUMBER_INVALID         0x65
#define RT_INVALID_1PREFIX        0x66
#define RT_INVALID_CAC            0x67
#define RT_CAC_INVALID_FOR_CALL   0x68
#define RT_SHOULD_HAVE_1PREFIX    0x69

#define SWITCH_TYPE_INC           0x10
#define SWITCH_TYPE_AT            0x11
#define SWITCH_TYPE_IXC           0x12
#define SWITCH_TYPE_EO            0x13

/* #define ANI_SCREEN_SERVICE        0x14*/

/* BD bug 3690 5/1/2001 */
#define PRESUB_NO_INDICATION      0x00
#define PRESUB_NO_INPUT           0x01
#define PRESUB_INPUT              0x02
#define CASUAL_INPUT              0x03

/* defines/bitMask for deallocInd from ISDN */
#define CC_IC_RSC_HOLD            0x01 /* bit -1 */
#define CC_OG_RSC_HOLD            0x02 /* bit -2 */

/* #define values for the Surrogate Notification 
 * bitmap used for replication */ 

#define O_DISCONNECT_NOTFN        0x01
#define O_ANSWER_NOTFN            0x02

/* defines for condump */
#define STARTCONLOG               0x01
#define DUMPCONLOG                0x02

/*Bug# 17825 */
/* Values from services/times10/scripts/ssCreate.sql */
#define RECALL_DIAL_TONE          1
#define CONFIRMATION_TONE         3
#define REORDER_TONE              4  
#define DIAL_TONE                 11
#define BUSY_TONE                 19
/* new in 5-1-1 - CAP_WARN_TONE in ssCreate.sql - not using identical defn. */
#define PRE_PAID_WARNING_TONE     20
#define REORDER_ANNOUNCEMENT_TONE 112

/* Values for Ephemeral Service Change Indication */
#define MODULEID_WILDCARD         0xFF
#define IPADDR_WILDCARD           0xFFFFFFFF
#define PKTRSCID_WILDCARD         0xFFFFFFFF

#define CC_MAX_CLEANUP_COUNTER    0x00000080

/* Announcements played from GCC */
#define RO_ANN_313                313 /* We're sorry, all circuits are busy now. 
                                         Will you please try again later*/
#define RO_ANN_315                315 /* We're sorry, your call did not go through. 
                                         Will you please try your call again */
#define RO_ANN_369                369 /* Your Call did not go through.
                                         Please hang up and try your call again */
#define RO_ANN_370                370 /* If you are trying to make a call
                                         Please hang up and try your call again.
                                         If you need help, hang up and dial your operator*/

/* extern */
#define MAP_ALL       0xFF  /* ISUP */
#define MAP_SI        0x01  /* ISUP */
#define MAP_IN        0x02  /* ISDN */
#define MAP_CS        0x04  /* CAS Line and/or Trunk */
#define MAP_SIP       0x08  /* SIP */
#define MAP_MGCP      0x10  /* MGCP Line and/or Trunk */


#define CC_SUBSCRIBER   1
#define CC_TRUNK        2

#define CC_NO_MGI_ACTION        0
#define CC_MGI_ACTION_ADD       1
#define CC_MGI_ACTION_MODIFY    2
#define CC_MGI_ACTION_SFM_ADD   3

/* Definition for JIP LRN Length */
#define MAX_JIP_LEN   6

/* Macros */

#define MAP_DP(mask, fmtStr, args...) \
    if ((mask) & ccIwDbgMask)  DP(fmtStr, ## args)

#define COPYTKN(dst, src)         \
{                                 \
   (dst).pres = (src).pres;       \
   if ((dst).pres == PRSNT_DEF)   \
      (dst).pres = PRSNT_NODEF;   \
   (dst).val = (src).val;         \
}
 
#define COPYSTR(dst, src, maxLen)                   \
{                                                   \
   U8   indexStr;                                   \
   (dst).pres = (src).pres;                         \
   if ((dst).pres == PRSNT_DEF)                     \
      (dst).pres = PRSNT_NODEF;                     \
   (dst).len = (src).len;                           \
   if ((dst).len > (maxLen))                        \
      (dst).len = (maxLen);                         \
   for(indexStr=0; indexStr<(dst).len; indexStr++)  \
      (dst).val[indexStr] = (src).val[indexStr];    \
}
 
#define SETPCOMP(pcomp, hi, lo)          \
{                                        \
   pcomp = PutLoByte(pcomp, lo);         \
   pcomp = PutHiByte(pcomp, hi);         \
}

#if SS7_SIITU2BIITU
#define SETPCOMP1(pcomp, hi, lo, parmId, parmCom)        \
{                                                        \
  if ((parmCom).eh.pres)                                 \
     OverrideDefPComp(&(pcomp), parmId, lo, &(parmCom)); \
  else                                                   \
  {                                                      \
     pcomp = PutLoByte(pcomp, lo);                       \
     pcomp = PutHiByte(pcomp, hi);                       \
  }                                                      \
}
#else /* SS7_SIITU2BIITU */
#define SETPCOMP1(pcomp, hi, lo, parmId, parmCom)        \
{                                        \
   pcomp = PutLoByte(pcomp, lo);         \
   pcomp = PutHiByte(pcomp, hi);         \
}
#endif /* SS7_SIITU2BIITU */

#define TRACECC(con, ev)                                \
{                                                       \
   if ((con != NULLP) && con->bbDpc)                    \
   {                                                    \
      if (con->bbDpc->trc)                              \
         ccDoTrace(con, ev);                            \
   }                                                    \
}

/* Macro for AIN Debug */
#define AIN_DEBUG
#ifdef AIN_DEBUG
#define AIN_DBG_LEVEL_0 0
#define AIN_DBG_LEVEL_1 1
#define AIN_DBG_LEVEL_2 2
#define AIN_DBG_LEVEL_3 3
#define AIN_DBG_LEVEL_4 4
#define AIN_DBG_LEVEL_5 5
#define CCDP(LEVEL, STR, args...)\
{\
    if(LEVEL>=inDbgMsk)\
    {\
      gitDebugPrint(__FILE__, __LINE__, STR, ## args);\
    }\
}
#define CCDBGON(LEVEL)   (LEVEL>=inDbgMsk)
#endif

#define MAPTOFICBUSYSTATUS(busyStatus, fic_busyStatus)\
{\
  switch(busyStatus)\
  {\
    case RMT_RES_IDLE:\
      fic_busyStatus = SubscriberIdle;\
    break;\
    case RMT_RES_CP_BUSY:\
      fic_busyStatus = SubscriberBusy;\
    break;\
    case RMT_RES_MNTC_BUSY:\
    case RMT_RES_PERM_BUSY:\
      fic_busyStatus = SubscriberFaulty;\
    break;\
    case RMT_RES_INVALID:\
    default:\
      fic_busyStatus = SubscriberNoValue;\
    break;\
  }\
}

#define CCFILLFICPROTOCOLTYPE(ccProtType, ficProtType) \
{\
  switch (ccProtType)\
  {\
    case CC_SI:\
      ficProtType = FIC_ISUP;\
      break;\
    case CC_BICC:\
      ficProtType = FIC_BICC;\
      break;\
    case CC_IN:\
      ficProtType = FIC_ISDN;\
      break;\
    case CC_CS_LN:\
      ficProtType = FIC_CAS;\
      break;\
    case CC_CS_TG:\
      ficProtType = FIC_CAS_TG;\
      break;\
    case CC_SIP:\
      ficProtType = FIC_SIP;\
      break;\
    case CC_SIPT:\
      ficProtType = FIC_SIP;\
      break;\
    case CC_GR303:\
      ficProtType = FIC_GR303;\
      break;\
    case CC_MGCP_LN:\
      ficProtType = FIC_MGCP_LN;\
      break;\
    case CC_MGCP_TG:\
      ficProtType = FIC_MGCP_TG;\
      break;\
    case CC_LOCAL_SWT_PROT:\
      ficProtType = FIC_LOCAL_SWT_PROT;\
      break;\
    case CC_EXT_BICC:\
      ficProtType = FIC_EXT_BICC;\
      break;\
    case CC_ICA:\
      ficProtType = FIC_ICA;\
      break;\
  }\
}

#define CCFILLGCCPROTOCOLTYPEFROMFIC(ficProtType, ccProtType)                  \
{                                                                              \
    switch(ficProtType)                                                        \
    {                                                                          \
        case FIC_SIP:                                                          \
            ccProtType = CC_SIPGM;                                             \
            break;                                                             \
        case FIC_CAS:                                                          \
            ccProtType = CC_CS_LN;                                             \
            break;                                                             \
        case FIC_GR303:                                                        \
            ccProtType = CC_GR303;                                             \
            break;                                                             \
    }                                                                          \
}

#define CCFILLSAPFROMPROTTYPE(protType, sap)                                   \
{                                                                              \
    switch(protType)                                                           \
    {                                                                          \
        case CC_SIPGM:                                                         \
            sap = sipwSap;                                                     \
            break;                                                             \
        case CC_CS_LN:                                                         \
            sap = casSap;                                                      \
            break;                                                             \
        case CC_GR303:                                                         \
            sap = gr3Sap;                                                      \
            break;                                                             \
    }                                                                          \
}

#define CCIGNORECOT(protType)    \
   (((protType == CC_SIG500V0) ||\
     (protType == CC_BICCG500V0)) ? TRUE : FALSE)

#define CCFILLCONTTYPE(con)                                                    \
{                                                                              \
  ProtType icPtType = 0;                                                             \
  Bool ignoreCot = CCIGNORECOT(con->icProtType);                               \
  con->ccrIAMflg = FALSE;                                                      \
  if ((con->ccEvntHold) && (con->evntHeld == CCE_CONIND) && (!ignoreCot))      \
  {                                                                            \
    icPtType = CC_GET_PROTOCOL(con->icProtType);                               \
    switch (icPtType)                                                          \
    {                                                                          \
      case CC_BICC:                                                            \
        if ((con->ccEvntHold->m.ccConEvnt.m.biccConEvnt.natConInd.contChkInd.  \
           pres == PRSNT_NODEF) && ((con->ccEvntHold->m.ccConEvnt.m.           \
           biccConEvnt.natConInd.contChkInd.val == CONTCHK_PREV) ||            \
           (con->ccEvntHold->m.ccConEvnt.m.biccConEvnt.natConInd.contChkInd.val\
           == CONTCHK_REQ)))                                                   \
          con->cotBlock.contType = CC_IAM_INITIATED;                           \
      case CC_SI:                                                              \
      case CC_EXT_BICC:                                                        \
        if ((con->ccEvntHold->m.ccConEvnt.m.siConEvnt.natConInd.contChkInd.pres\
           == PRSNT_NODEF) && ((con->ccEvntHold->m.ccConEvnt.m.siConEvnt.      \
           natConInd.contChkInd.val == CONTCHK_PREV) || (con->ccEvntHold->m.   \
           ccConEvnt.m.siConEvnt.natConInd.contChkInd.val == CONTCHK_REQ)))    \
          con->cotBlock.contType = CC_IAM_INITIATED;                           \
      default:                                                                 \
        break;                                                                 \
    }                                                                          \
  }                                                                            \
  CCDP(AIN_DBG_LEVEL_0, "contType = %d %s\n",                                  \
       con->cotBlock.contType, (ignoreCot)?"ignored":"");                      \
}

#define CCFILLCOTSTAT(con, type, action)                                       \
{                                                                              \
    switch (type)                                                              \
    {                                                                          \
    case CC_IC_COT_ATTEMPT:                                                    \
        con->callDtlInfo.cm.cotStats.icCotAttmpt++;                            \
        break;                                                                 \
    case CC_OG_COT_ATTEMPT:                                                    \
        if (action == CC_ACTION_INCREMENT)                                     \
            con->callDtlInfo.cm.cotStats.ogCotAttmpt++;                        \
        else if (action == CC_ACTION_DECREASE)                                 \
        {                                                                      \
           if((con->callDtlInfo.cm.cotStats.ogCotAttmpt > 0) &&                \
              (con->reattemptFlag == FALSE))                                   \
               {con->callDtlInfo.cm.cotStats.ogCotAttmpt--;}                   \
        }                                                                      \
        break;                                                                 \
    case CC_IC_COT_SUCCESS:                                                    \
        con->callDtlInfo.cm.cotStats.icCotSucc++;                              \
        break;                                                                 \
    case CC_OG_COT_SUCCESS:                                                    \
        if (action == CC_ACTION_INCREMENT)                                     \
            con->callDtlInfo.cm.cotStats.ogCotSucc++;                          \
        else if ((action == CC_ACTION_DECREASE) &&                             \
                 (con->callDtlInfo.cm.cotStats.ogCotSucc > 0 ))                \
        {                                                                      \
            con->callDtlInfo.cm.cotStats.ogCotSucc--;                          \
            if(con->reattemptFlag == TRUE)                                     \
            {con->callDtlInfo.cm.cotStats.ogCotAttmpt--;}                      \
        }                                                                      \
        break;                                                                 \
    case CC_IC_COT_FAIL:                                                       \
        con->callDtlInfo.cm.cotStats.icCotFail++;                              \
        break;                                                                 \
    case CC_OG_COT_FAIL:                                                       \
        con->callDtlInfo.cm.cotStats.ogCotFail++;                              \
        break;                                                                 \
    default:                                                                   \
        break;                                                                 \
    }                                                                          \
    CCDP(AIN_DBG_LEVEL_0, "icCotAttmpt: %d, ogCotAttmpt:%d, icCotSucc:%d,"     \
        "ogCotSucc:%d, icCotFail:%d, ogCotFail:%d\n",                          \
        con->callDtlInfo.cm.cotStats.icCotAttmpt,                              \
        con->callDtlInfo.cm.cotStats.ogCotAttmpt,                              \
        con->callDtlInfo.cm.cotStats.icCotSucc,                                \
        con->callDtlInfo.cm.cotStats.ogCotSucc,                                \
        con->callDtlInfo.cm.cotStats.icCotFail,                                \
        con->callDtlInfo.cm.cotStats.ogCotFail);                               \
}

#define CCFILLTONEANNCSTAT(con, type)                                          \
{                                                                              \
    switch (type)                                                              \
    {                                                                          \
    case CC_TONE_SETUP_ATTEMPT:                                                \
        con->callDtlInfo.cm.toneAnncStats.toneSetupAttmpt++;                   \
        break;                                                                 \
    case CC_TONE_SETUP_SUCCESS:                                                \
        con->callDtlInfo.cm.toneAnncStats.toneSetupSucc++;                     \
        break;                                                                 \
    case CC_ANNC_SETUP_ATTEMPT:                                                \
        con->callDtlInfo.cm.toneAnncStats.anncSetupAttmpt++;                   \
        break;                                                                 \
    case CC_ANNC_SETUP_SUCCESS:                                                \
        con->callDtlInfo.cm.toneAnncStats.anncSetupSucc++;                     \
        break;                                                                 \
    default:                                                                   \
        break;                                                                 \
    }                                                                          \
    CCDP(AIN_DBG_LEVEL_0, "toneSetupAttmpt:%d, toneSetupSucc:%d,"              \
                          "anncSetupAttmpt:%d, anncSetupSucc:%d\n",            \
      con->callDtlInfo.cm.toneAnncStats.toneSetupAttmpt,                       \
      con->callDtlInfo.cm.toneAnncStats.toneSetupSucc,                         \
      con->callDtlInfo.cm.toneAnncStats.anncSetupAttmpt,                       \
      con->callDtlInfo.cm.toneAnncStats.anncSetupSucc);                        \
}

#define CC_FIND_CONTCHKIND_VAL(con, val)                                       \
{                                                                              \
  ProtType icPtType = 0;                                                             \
  if ((con->ccEvntHold) && (con->evntHeld == CCE_CONIND))                      \
  {                                                                            \
    icPtType = CC_GET_PROTOCOL(con->icProtType);                                 \
    switch (icPtType)                                                          \
    {                                                                          \
      case CC_BICC:                                                            \
        if (con->ccEvntHold->m.ccConEvnt.m.biccConEvnt.natConInd.contChkInd.   \
           pres == PRSNT_NODEF)                                                \
          val = con->ccEvntHold->m.ccConEvnt.m.biccConEvnt.natConInd.          \
                contChkInd.val;                                                \
        else                                                                   \
          val = CONTCHK_NOTREQ;                                                \
        break;                                                                 \
      case CC_SI:                                                              \
        if (con->ccEvntHold->m.ccConEvnt.m.siConEvnt.natConInd.contChkInd.     \
           pres == PRSNT_NODEF)                                                \
          val = con->ccEvntHold->m.ccConEvnt.m.siConEvnt.natConInd.            \
                contChkInd.val;                                                \
        else                                                                   \
          val = CONTCHK_NOTREQ;                                                \
        break;                                                                 \
      default:                                                                 \
        val = CONTCHK_NOTREQ;                                                  \
        break;                                                                 \
    }                                                                          \
  }                                                                            \
  else                                                                         \
    val = CONTCHK_NOTREQ;                                                      \
}

#define MODIFYCCIDTOCC2ORCC6(con)\
{\
  if (con->icBcm.cc)\
  {\
    if (con->icBcm.cc->ccId == CiThreePartySetup)\
    {\
      con->icBcm.cc->ccId = CiPartyOnHold;\
    }\
    else if (con->icBcm.cc->ccId == CiOriginatingSetup)\
    {\
      con->icBcm.cc->ccId = CiStable2Party;\
    }\
  }\
}

#define CCLOGERR(STR, args...) \
    gitDebugPrint(__FILE__, __LINE__, STR, ## args)

#define UPDATE_OPIC(con, opic) con->icBcm.pic = opic

#define UPDATE_TPIC(con, tpic) con->ogBcm.pic = tpic

#define UPDATE_CCID(pBcm, ccid) pBcm->cc->ccId = ccid

#define CCLAYERNAME    "CALL CONTROL"
#define  DBGMASK_CC (DBGMASK_LYR << 0)
/* Macro for debug */
/* Macro for debug */
#define CC_DP(mask, con, fmtStr, args...) \
    if ( ((mask) & ccInit.dbgMask) || ((con)->captIndex) ||\
          (logCallsWithBuffer && (con)->eventBuffered))    \
    {                                \
        DP(fmtStr, ## args);         \
    }


#define CCDBGP(_msgClass, _arg) \
        DBGP(&ccInit, CCLAYERNAME, _msgClass, _arg)

#ifdef CC_FID_1950_1773
#define CCGET_INTFCKEY(_intfcKey, _intfcType, _intfcId) \
{                                                       \
   _intfcKey[0] = GetHiByte(_intfcType);                 \
   _intfcKey[1] = GetHiByte(GetHiWord(_intfcId));        \
   _intfcKey[2] = GetLoByte(GetHiWord(_intfcId));        \
   _intfcKey[3] = GetHiByte(GetLoWord(_intfcId));        \
   _intfcKey[4] = GetLoByte(GetLoWord(_intfcId));        \
}
#endif

/* cc028.15 : add boundary check to compute obsIdx flag */
#define CC_COMPUTE_OBS_FLAG(_con)                                    \
{                                                                    \
   U8  _i, _j;                                                       \
   U8 _k, _m;                                                        \
   _con->obsFlag = 0;                                                \
   for (_i = 0; _i < NUM_IC_CRITERIA; _i++)                          \
   {                                                                 \
      _k = _con->rowObsIdx[_i]; \
      if (_con->rowObsIdx[_i] >= ccCp.genCfg.obsTblDim) \
      { \
         _k = 0; \
      } \
      for (_j = 0; _j < NUM_OG_CRITERIA; _j++)                       \
      {                                                              \
         _m = _con->colObsIdx[_j]; \
         if (_con->colObsIdx[_j] >= ccCp.genCfg.obsTblDim) \
         { \
            _m = 0; \
         } \
         _con->obsFlag |=                                            \
            ccObsTablePtr[_k][_m]; \
      }                                                              \
   }                                                                 \
}

#define CC_FILL_DEF_ECHOIND(pEcho) {\
   pEcho->eh.pres = PRSNT_NODEF;\
   pEcho->outEchoRsp.pres = PRSNT_NODEF;\
   pEcho->outEchoRsp.val = ECHCDEV_NOINFOINCL;\
   pEcho->incEchoRsp.pres = PRSNT_NODEF;\
   pEcho->incEchoRsp.val = ECHCDEV_NOINFOINCL;\
   pEcho->outEchoReq.pres = PRSNT_NODEF;\
   pEcho->outEchoReq.val = ECHCDEV_NOINFOINCL;\
   pEcho->incEchoReq.pres = PRSNT_NODEF;\
   pEcho->incEchoReq.val = ECHCDEV_NOINFOINCL;\
}

#if 1 /* TELICA - Bugzilla-3723 - taraveti */
#define LNP_NA                  0
#define LNP_TRANS_HERE          1
#define LNP_PORTD_HERE          2
#define LNP_TRANS_SOMEWHERE     3
#endif /* TELICA - Bugzilla-3723 - taraveti */

/* TELICA-jwh-10/05/2001:  Add GCC fsm profiling */
#ifdef CC_FSM_PROFILE
#define CC_STATE_CHANGE(con, newState)                         \
{                                                              \
    if (ccFsmProfileStatus != CC_FSM_PROFILE_ACTIVE)           \
    {                                                          \
        (con)->lastFile = __FILE__;                            \
        (con)->lastLine = __LINE__;                            \
        CC_DP(DBGMASK_CC, (con), "CCFSM: State:%2d->%2d,"      \
           " icSu:0x%08X, ogSu:0x%08X\n",                      \
               (int) (con)->state, (int)newState,              \
               (unsigned int)(con)->icSuConnId,                \
               (unsigned int)(con)->ogSuConnId);               \
        (con)->state = (newState);                             \
        if(((newState) == CCS_ANSWERED))                       \
        {                                                      \
           ccPegStndCalls(con, CC_INCREASE);                   \
        }                                                      \
    }                                                          \
    else                                                       \
    {                                                          \
        ccFsmProfile((con), __FILE__, __LINE__, (newState));   \
    }                                                          \
    CC_TRACE_STATE (con, newState);                            \
}

#define CC_PROFILE_EVT_START(startTicksCpu, startTicksClk)  \
    if (ccFsmProfileStatus == CC_FSM_PROFILE_ACTIVE)        \
    {                                                       \
        ccProfileEvtStart(startTicksCpu, startTicksClk);    \
    }

#define CC_PROFILE_EVT_END(startTicksCpu, startTicksClk, evntType, oldstate)    \
    if (ccFsmProfileStatus == CC_FSM_PROFILE_ACTIVE)                            \
    {                                                                           \
        ccProfileEvtEnd(startTicksCpu, startTicksClk, evntType, oldstate);      \
    }

#else

#define CC_STATE_CHANGE(con, newState)                         \
{                                                              \
    (con)->state = (newState);                                 \
    CC_TRACE_STATE (con, newState);                            \
}

#define CC_PROFILE_EVT_START(startTicksCpu, startTicksClk)
#define CC_PROFILE_EVT_END(startTicksCpu, startTicksClk, evntType, oldstate)

#endif /* CC_FSM_PROFILE */



#define CC_SET_G3CONNID(CONNID, IFID, CRVID)             \
{                                                         \
   G3ConnId g3ConnId;                                     \
   g3ConnId.split.intfcId = IFID;                      \
   g3ConnId.split.crvId   = CRVID;                        \
   CONNID = g3ConnId.connId;                            \
}
#define CCDP_RELCOND (DBGMASK_LYR << 1)

#define GCC_SET_RELCOND(con, condition)                           \
{                                                                 \
   (con)->gccInitiatedRel = TRUE;                                 \
   ccCp.sts.relCondition[(U8)(condition)]++;                      \
   /*(con)->callDtlInfo.cm.relOrign = LCC_RELORGN_INTERNAL;*/         \
   CC_DP(CCDP_RELCOND, (con), "CCRELCOND:%ld, icSp:0x%08X, icSu:0x%08X, ogSp:0x%08X, ogSu:0x%08X\n", \
        (condition), \
        (unsigned int)(con)->icSpConnId, (unsigned int)(con)->icSuConnId, \
        (unsigned int)(con)->ogSpConnId, (unsigned int)(con)->ogSuConnId); \
}

#define GCC_SET_RELCAUSE(con, sap, protType, cause) \
{                                                   \
  if ((con)->gccInitiatedRel == TRUE)               \
  {                                                 \
   (sap)->sts.relCause[(U8)(cause)]++;              \
   CC_DP(CCDP_RELCOND, (con), "CCRELCAUSE:%d, protType:%d, icSp:0x%08X, icSu:0x%08X, ogSp:0x%08X, ogSu:0x%08X\n", \
        (cause), (protType),\
        (unsigned int)(con)->icSpConnId, (unsigned int)(con)->icSuConnId, \
        (unsigned int)(con)->ogSpConnId, (unsigned int)(con)->ogSuConnId); \
  } \
}


EXTERN U8 billingDbgMsk;

#define BILLING_DBG_LEVEL_1 1
#define BILLING_DBG_LEVEL_2 2
#define BILLING_DBG_ALWAYS  4

#define BDBG(LEVEL, STR, args...) \
    if(LEVEL>=billingDbgMsk)\
      gitDebugPrint(__FILE__, __LINE__, "BDBG: " STR, ## args)

#define IFBDBG(LEVEL) if(LEVEL>=billingDbgMsk)

/* Bukucu - Bug - 16867 get unique call id for stats
 * since CC_MAXSUCONNID is 0x00FFFFFF, and the highest byte is
 * not used for icSuConnId, it's used for ownCcsId (Bug# 52681 )
*/
#ifndef CLOUD
#define CC_GET_STATS_CALLID(callId, icSuConnId, ownCcsId)       \
{                                                               \
    callId = (icSuConnId | (ownCcsId << 24));                   \
}
#else
#define CC_GET_STATS_CALLID(callId, icSuConnId, ownCcsId)       \
{                                                               \
    callId = (icSuConnId | (ownCcsId << 23));                   \
}
#endif

/* Bug 93819 - In order to support global MGs, the CcsId needs
 * to be included when getting the MGCT transaction ID.
 */
#define CC_GET_MGCT_TRANSID(transId, suCtxId, ownCcsId)         \
{                                                               \
    transId = ((suCtxId) | ((ownCcsId) << 20));                 \
}

/* Bug 93819 - In order to support global MGs, the CcsId needs to be
 * removed from the MGCT transaction ID to get the SU context ID.
 */
#define CC_GET_SUCTXID(suCtxId, transId)                        \
{                                                               \
    suCtxId = ((transId) & (CC_MAXSUCTXID));                    \
}

/* Variable Announcement support.  
*  Macro returns True if the provided announcement has Variable Args
* returns False if no variable Args
* Called with pointer to char str  &annDgtBlk->intAnncBlk.lst[0].val[0]
*
*/

#define VARIABLE_ANNC(AnncPtr) ({int a1 = (int)((char*)AnncPtr)[2]; \
 int a2 = (int)((char*)AnncPtr)[3];                                 \
((a1 == 0) && ( (a2 > 0) && a2 < SIGEVTDGT_V2_MAX_ANNC_VARS) ? 1 : 0); })

#define VAR_ANNC_COUNT(AnncPtr) (AnncPtr->lst[0].val[3])

#define CC_STR_RESOURCE_ID(AnncPtr)                                         \
        (AnncPtr->lst[0].anncId)

#define CC_FREE_EVNT_HOLD(con)                                              \
{                                                                           \
  ProtType protType = ((CC_GET_EVNTHOLD_DIR((con)->directionHeld) == INCTOOUT) ?                     \
      CC_GET_PROTOCOL((con)->icProtType) :                 \
      CC_GET_PROTOCOL((con)->ogProtType));                 \
  if ((con)->ccEvntHold != NULLP)                                         \
    {                                                                       \
        ccFreeSipIdInEvntHold(__FILE__, __LINE__, (con)->evntHeld,\
            (con)->evntTypeHeld, (PTR)((con)->ccEvntHold), protType);\
        ccFreeSipPstnXmlInEvntHold(__FILE__, __LINE__, (con)->evntHeld,\
              (con)->evntTypeHeld, (PTR)((con)->ccEvntHold), protType);\
        ccFreeSipUuiDataInEvntHold(__FILE__, __LINE__, (con)->evntHeld,\
              (con)->evntTypeHeld, (PTR)((con)->ccEvntHold), protType);\
        if((con)->evntHeld == CCE_CONIND)                                   \
        {                                                                   \
          ccFreeHistInfoEntriesInTheEvnt(__FILE__, __LINE__,(con)->evntHeld,\
              (con)->evntTypeHeld, (PTR)((con)->ccEvntHold), protType);     \
          ccFreeSipReqUriStrInEvntHold(__FILE__, __LINE__,(con)->evntHeld,\
              (con)->evntTypeHeld, (PTR)((con)->ccEvntHold), protType);     \
        }                                                                   \
        SPutSBuf(ccInit.region, ccInit.pool, (con)->ccEvntHold, \
                 (Size)(sizeof(CcAllSdus)));                                \
        (con)->ccEvntHold = NULLP;                                          \
    }                                                                       \
                                                                            \
    if ((con)->icConEvntHold != NULLP)                                      \
    {                                                                       \
      ccFreeSipIdInEvntHold(__FILE__, __LINE__,CCE_CONIND, \
      (con)->evntTypeHeld, (PTR)((con)->icConEvntHold), CC_GET_PROTOCOL((con)->icProtType)); \
      ccFreeSipPstnXmlInEvntHold(__FILE__, __LINE__, CCE_CONIND,\
      (con)->evntTypeHeld, (PTR)((con)->icConEvntHold), CC_GET_PROTOCOL((con)->icProtType)); \
      ccFreeSipUuiDataInEvntHold(__FILE__, __LINE__, CCE_CONIND,\
      (con)->evntTypeHeld, (PTR)((con)->icConEvntHold), CC_GET_PROTOCOL((con)->icProtType)); \
      ccFreeHistInfoEntriesInTheEvnt(__FILE__, __LINE__, CCE_CONIND,\
      (con)->evntTypeHeld, (PTR)((con)->icConEvntHold), CC_GET_PROTOCOL((con)->icProtType)); \
      ccFreeSipReqUriStrInEvntHold(__FILE__, __LINE__, CCE_CONIND,\
      (con)->evntTypeHeld, (PTR)((con)->icConEvntHold), CC_GET_PROTOCOL((con)->icProtType)); \
      SPutSBuf(ccInit.region, ccInit.pool, (con)->icConEvntHold, \
                 (Size)(sizeof(CcAllSdus)));                                \
        (con)->icConEvntHold = NULLP;                                       \
    }                                                                       \
                                                                            \
    if ((con)->ccRelEvntHold != NULLP)                                      \
    {                                                                       \
        SPutSBuf(ccInit.region, ccInit.pool, (con)->ccRelEvntHold, \
                 (Size)(sizeof(CcAllSdus)));                                \
        (con)->ccRelEvntHold = NULLP;                                       \
    }                                                                       \
}

#define CC_FREE_REL_EVNT_HOLD(con)                                          \
    if ((con)->ccRelEvntHold != NULLP)                                      \
    {                                                                       \
        SPutSBuf(ccInit.region, ccInit.pool, (con)->ccRelEvntHold, \
                 (Size)(sizeof(CcAllSdus)));                                \
        (con)->ccRelEvntHold = NULLP;                                       \
    }

#define CC_FREE_ROUTE_EVNT(con)                                             \
{                                                                           \
  if ((con)->routeEvnt != NULLP)                                            \
  {                                                                         \
    SPutSBuf(ccInit.region, ccInit.pool, (con)->routeEvnt,         \
             (Size)(sizeof(RtRteCfmEvnt1)));                                \
    (con)->routeEvnt = NULLP;                                               \
  }                                                                         \
}

#define CC_IS_CON_IN_EVNT_HOLD(con) ((((con)->evntHeld == CCE_CONIND) || ((con)->evntHeld == CCE_ADDRCFM)) && (con->ccEvntHold)) 
/* BUG:21331 used to identify RElIND */
#define CC_IS_REL_IN_EVNT_HOLD(con) (((con)->evntHeld == CCE_RELIND) && con->ccEvntHold)

/* Bug90322 */
#define CC_IS_ICREL_IN_EVNT_HOLD(con) (((con)->evntHeld == CCE_RELIND) && (con->ccEvntHold) && (con->directionHeld == INCTOOUT))
#define CC_IS_OGREL_IN_EVNT_HOLD(con) (((con)->evntHeld == CCE_RELIND) && (con->ccEvntHold) && (con->directionHeld == OUTTOINC))

#define CC_IS_SVC_IND_IN_EVNT_HOLD(con) (((con)->evntHeld == CCE_SVC_IND) &&\
                                         (con)->ccEvntHold)
#define CC_IS_CNSTIND_IN_EVNT_HOLD(con) (((con)->evntHeld == CCE_CNSTIND) &&\
                                         (con)->ccEvntHold)

#define CC_IS_CCID_CC10_AND_ABOVE(pBcm) \
        (((pBcm)->cc) && (((pBcm)->cc->ccId >= CiStableMParty) && \
                          ((pBcm)->cc->ccId <= CiRemoteTransferSetup)))

#define CC_IS_CON_IN_FORWARD(pBcm)                                \
        ((pBcm) && ((pBcm)->con) && ((pBcm)->con->surrogate))

#define IS_CLEAR_CHANNEL_CODEC(codec) \
        ((codec == SDP_CODEC_CISCO_CL) ||\
         (codec == SDP_CODEC_X_CCD)    ||\
         (codec == SDP_CODEC_CL_MODE))

#define CC_CLEAN_FIC_TXN(pBcm)                                                \
{                                                                             \
  Bool           noOperation            = FALSE;                              \
  U8             ccCleanFixTxnNelIndex  = 0;                                  \
                                                                              \
  if ((pBcm)->bcmType == BtOBcm)                                              \
    ccCloseOBcmTransaction((pBcm)->con);                                      \
  else if ((pBcm)->bcmType == BtTBcm)                                         \
    ccCloseTBcmTransaction((pBcm)->con);                                      \
  else                                                                        \
  {                                                                           \
    CCLOGERR("Error:Invalid bcmType:txnId:%ld, icSu:%ld, ogSu:%ld, bcmType:%d\n",\
             (pBcm)->svcHeader.transId, (pBcm)->con->icSuConnId,              \
             (pBcm)->con->ogSuConnId, (pBcm)->bcmType);                       \
    noOperation = TRUE;                                                       \
  }                                                                           \
  if (!noOperation)                                                           \
    for (ccCleanFixTxnNelIndex = 0; ccCleanFixTxnNelIndex < EReqMax; ccCleanFixTxnNelIndex++) \
      (pBcm)->nel[ccCleanFixTxnNelIndex] = NEL_UNARMED;                       \
}

#define CC_SET_BEARER_IP_PORT(bearerIpPort, ipVal, portVal) \
{                                                           \
    (bearerIpPort)->pres    = ((ipVal)?TRUE:FALSE);         \
    (bearerIpPort)->ip = (ipVal);                           \
    (bearerIpPort)->port = (portVal);                       \
}
#define CC_RELRSP_NEEDED_FOR_DISCONNECT(pBcm, protType)     \
                                                            \
   (!((ccIsLEGSubscriber(pBcm)) || (protType == CC_CS_TG) ||\
      ((pBcm)->con->suspendDirection != DIR_UNKNOWN)))      \


#if 1  /*T38_ENABLE*/ /*T.38 Changes* Begin*/
#define CC_SDP_TO_BEARER_IP_PORT(bearerIpPort, sdp)         \
({                                                           \
   MediaStream_t *msPtr;                                    \
   mgctGetValidMediaStream((sdp), &msPtr);                  \
    if ((bearerIpPort) && (msPtr) &&                        \
        (msPtr->address.ip.port.pres) &&                    \
        (msPtr->address.ip.binary.pres))                    \
    {                                                       \
        (bearerIpPort)->pres = TRUE;                        \
        (bearerIpPort)->ip = msPtr->address.ip.binary.val;  \
        (bearerIpPort)->port = msPtr->address.ip.port.val;  \
    }                                                       \
    if (((bearerIpPort)->ip.ipVersion == AF_INET))          \
    {                                                       \
        CCDP(AIN_DBG_LEVEL_1, "CC_SDP_TO_BEARER_IP_PORT:  pres = %d, ip =0x%x, port = %d\n", \
             (bearerIpPort)->pres, (bearerIpPort)->ip.ip_ipv4, (bearerIpPort)->port); \
    }                                                       \
    else                                                    \
    {                                                       \
        CCDP(AIN_DBG_LEVEL_1, "CC_SDP_TO_BEARER_IP_PORT:  pres = %d, ip =%s, port = %d\n", \
             (bearerIpPort)->pres, ip_ntoa(&(bearerIpPort)->ip), (bearerIpPort)->port); \
    }                                                       \
 })
#else /*T.38 Changes End*/
#define CC_SDP_TO_BEARER_IP_PORT(bearerIpPort, sdp)  ({     \
   MediaStream_t *msPtr;                                    \
   cmGetValidMediaStream((sdp), &msPtr);                    \
    if ( (bearerIpPort) && (msPtr) &&                       \
        (msPtr->address.ip.port.pres) &&                    \
        (msPtr->address.ip.binary.pres) )                   \
    {                                                       \
        (bearerIpPort)->pres = TRUE;                        \
        (bearerIpPort)->ip = msPtr->address.ip.binary.val;  \
        (bearerIpPort)->port = msPtr->address.ip.port.val;  \
    }                                                       \
    if (((bearerIpPort)->ip.ipVersion == AF_INET)           \
    {                                                       \
        CCDP(AIN_DBG_LEVEL_1, "CC_SDP_TO_BEARER_IP_PORT:  pres = %d, ip =0x%lx, port = %d\n", \
             (bearerIpPort)->pres, (bearerIpPort)->ip.ip_v4, (bearerIpPort)->port); \
    }                                                       \
    else                                                    \
    {                                                       \
        CCDP(AIN_DBG_LEVEL_1, "CC_SDP_TO_BEARER_IP_PORT:  pres = %d, ip =%s, port = %d\n", \
             (bearerIpPort)->pres, ip_ntoa(&(bearerIpPort)->ip), (bearerIpPort)->port); \
    }                                                       \
 })
#endif /*T.38 Changes End*/

#define CC_SDP_TO_CODEC(cdc, sdp)                              \
    if ( (sdp) && (mgctGetMediaStream(((Sdp*)sdp),SDP_MTYPE_AUDIO)) && \
           (mgctGetMediaStream(((Sdp*)sdp),SDP_MTYPE_AUDIO)->m.voip.numCodec) )                    \
    {                                                            \
           cdc = mgctGetMediaStream(((Sdp*)sdp),SDP_MTYPE_AUDIO)->m.voip.codec[0];               \
           cdc.codecExist = PRSNT_NODEF;                        \
    }

#define CC_PUT_BCM_INTO_TWO_PARTY(pBcm, otherlegId)             \
{                                                               \
    (pBcm)->cc        = NULLP;                                  \
    SGetSBuf(ccInit.region, ccInit.pool, (Data **) &((pBcm)->cc), \
             (Size) sizeof(AinCallConfig_t));                   \
    cmMemset((U8 *)((pBcm)->cc), 0, sizeof(AinCallConfig_t));  \
    (pBcm)->cc->ccId    = CiStable2Party;                       \
    (pBcm)->cc->legId01 = (pBcm);                               \
    (pBcm)->cc->legId1  =  otherlegId;                          \
}

#define CC_BREAK_IF_DISC_BUFFERED(con)                                     \
{                                                                          \
  U8         _icEvntType         = 0;                                      \
  U8         _ogEvntType         = 0;                                      \
  if ((ccCheckRelOrOnHookInBuffer(&((con)->icBcm), &_icEvntType) == ROK) ||\
      (ccCheckRelOrOnHookInBuffer(&((con)->ogBcm), &_ogEvntType) == ROK))  \
  {                                                                        \
    CCDP(AIN_DBG_LEVEL_3, "Rel-Dis bufferered:icSu = %ld, ogSu = %ld\n",     \
         con->icSuConnId, con->ogSuConnId);                                \
    ccPostEventToSelf (con);                                               \
    RETVALUE(ROK);                                                         \
  }                                                                        \
}

/* for cc_bdy8.c functions */
#define CC_BEARER_CONNECT_ANSWER_FLAG  0x01
#define CC_BEARER_CONNECT_MGISVC_CHG   0x02

/* Added for CPG hold and retrieval */
#define CC_BEARER_CONNECT_HOLD_FLAG    0x04
#define CC_BEARER_CONNECT_XFER_FLAG    0x08
#define CC_BEARER_CONNECT_HOLD_1W_FLAG 0x10
#define CC_BEARER_CONNECT_XFER_1W_FLAG 0x20
/* euysal - 03/15/2006 - BUG:45236 - Problem 3 Dual Call Hold */
#define CC_BEARER_CONNECT_SNDCPG_FLAG  0x40

#define CC_CLR_ANSWER_CALL_FLAG(bitMask)                     \
  (bitMask) &= ~CC_BEARER_CONNECT_ANSWER_FLAG
#define CC_SET_ANSWER_CALL_FLAG(bitMask)                     \
  (bitMask) |= CC_BEARER_CONNECT_ANSWER_FLAG
#define CC_CLR_MGISVC_CHG_FLAG(bitMask)                      \
  (bitMask) &= ~CC_BEARER_CONNECT_MGISVC_CHG
#define CC_SET_MGISVC_CHG_FLAG(bitMask)                      \
  (bitMask) |= CC_BEARER_CONNECT_MGISVC_CHG

/* Added for ISUP - SIP interworking, HOLD and RETRIEVAL */
#define CC_CLR_CALL_FLAG(bitMask,flag)                     \
  (bitMask) &= ~flag
#define CC_SET_CALL_FLAG(bitMask,flag)                     \
  (bitMask) |= flag
#define CC_IS_CALL_FLAG_SET(bitMask,flag)                  \
  ((bitMask) & flag)

#define CC_CALL_ON_HOLD_OR_XFER(con)                     \
  (((con->ccBearerConnectData.bitMask & CC_BEARER_CONNECT_HOLD_FLAG) || \
   (con->ccBearerConnectData.bitMask & CC_BEARER_CONNECT_HOLD_1W_FLAG) || \
   (con->ccBearerConnectData.bitMask & CC_BEARER_CONNECT_XFER_FLAG) || \
   (con->ccBearerConnectData.bitMask & CC_BEARER_CONNECT_XFER_1W_FLAG)) ? TRUE : FALSE)


#define CC_IS_REL_CAUSE_NETWRK_BSY(relCause)                            \
(                                                                       \
  ((relCause == CCNORTTODEST) ||     /*  3  */                          \
   (relCause == CCEXCHRTGERR) ||     /* 25  */                          \
   (relCause == CCDESTOUTORD) ||     /* 27  */                          \
   (relCause == CCFACREJ)     ||     /* 29  */                          \
   (relCause == CCNOCIRCUIT)  ||     /* 34  */                          \
   (relCause == CCNETAOL)     ||     /* 38  */                          \
   (relCause == CCTMPFAIL)    ||     /* 41  */                          \
   (relCause == CCSWTCHCONG)  ||     /* 42  */                          \
   (relCause == CCPREEMPT)    ||     /* 47  */                          \
   (relCause == CCINVMSG)     ||     /* 95  */                          \
   (relCause == CCTMRRECOV)   ||     /* 102 */                          \
   (relCause == CCPROTERR)    ||     /* 111 */                          \
   (relCause == CCINTRWRK))          /* 127 */                          \
)

#define CC_ROUTE_ADVANCE(con)                                           \
  ((con) && (con->routeEvnt) &&                                         \
   ((con->routeEvnt->intFlags == FC_ROUTE_ADV) ||                       \
    (con->routeEvnt->intFlags == FC_ROUTE_ADV_MULTI_RR)))               \

#define CC_ROUTE_ADVANCE_NOT_MULTIRR(con)                               \
  ((con) && (con->routeEvnt) &&                                         \
   (con->routeEvnt->intFlags != FC_ROUTE_ADV_MULTI_RR))                 \

/* FID 14716.0 */
#define CC_GR_OVRFLW_PRES(con)                                          \
  ((con) && ((con)->routeEvnt) &&                                       \
   ((con)->routeEvnt->infoBits & FC_RL_GR_OVRFLW_PRES))                 \

/* FID 16766.0 */
#define CC_GR_OVRFLW_PRES_AND_UNANCHORED(con)                           \
   (CC_GR_OVRFLW_PRES(con) &&						\
    (CC_GET_PROTOCOL(con->icProtType) != CC_EXT_BICC) &&		\
    ccIsIPResource(&con->icBcm) &&					\
    (!ccIsBearerNATIngress(con)))

/* modified when SIP UT, add (con)->routeEvnt check*/
#define NMBRCATEGORY_IVR(con)                                           \
 (((con)->routeEvnt) &&                                                 \
  (con->routeInfo->nmbrCategoryPres) &&                                 \
      (con->routeInfo->nmbrCategory == IVR_ROUTE_CATEGORY))

#define NMBRCATEGORY_NOT_IVR(con)                                       \
 !((con->routeInfo->nmbrCategoryPres) &&                                \
      (con->routeInfo->nmbrCategory == IVR_ROUTE_CATEGORY))

#define CC_GEN_BILLING(con,CdrStatus,genCause)                          \
{                                                                       \
  U8 oldCdrStatus = 0;                                                  \
  oldCdrStatus = con->callDtlInfo.cm.cdrStatus;                         \
  BDBG (BILLING_DBG_LEVEL_1,                                            \
            "oldCdrStatus:%d, CdrStatus:%d\n",oldCdrStatus,CdrStatus);  \
  con->callDtlInfo.cm.cdrStatus = CdrStatus;                            \
  if (procInfo.ActiveSP)                                                \
  {                                                                     \
    if ((ccBillingSystem.amaGeneration == AmaGeneration_ON) ||          \
        (ccStSt.state == C_STATS_IN_SERVICE))                           \
    {                                                                   \
      ccGenAcntgInd (con, genCause);                      \
    }                                                                   \
    else                                                                \
    {                                                                   \
      BDBG (BILLING_DBG_LEVEL_1,                                        \
            "No Billing/Stats InService, skip ccGenAcntgInd\n");        \
    }                                                                   \
  }                                                                     \
  con->callDtlInfo.cm.cdrStatus = oldCdrStatus;                         \
}

#define CC_LOG_FRAUD_REL_DETAILS(con)                                   \
{                                                                       \
  if (procInfo.ActiveSP)                                                \
  {                                                                     \
    ccStopFraudTmr(con, SCR_FRAUD_TMR);                                 \
    ccStopFraudTmr(con, ANS_FRAUD_TMR);                                 \
    if (con->fraudPrflbmap & SET_SCR_LOGGING_FLAG)                      \
    {                                                                   \
      if (con->fraudPrflbmap & SET_SCR_SWITCH_INITREL_FLAG)             \
        ccLogFraudCallReleaseDetails(con, CC_SWITCH_INITIATED, TRUE);   \
      else if ((con->fraudPrflbmap & SET_ANS_SWITCH_INITREL_FLAG) ==0 ) \
        ccLogFraudCallReleaseDetails(con, CC_SUBSCRIBER_INITIATED, TRUE);\
    }                                                                   \
    if (con->fraudPrflbmap & SET_ANS_LOGGING_FLAG)                      \
    {                                                                   \
      if (con->fraudPrflbmap & SET_ANS_SWITCH_INITREL_FLAG)             \
        ccLogFraudCallReleaseDetails(con, CC_SWITCH_INITIATED, FALSE);  \
    }                                                                   \
  }                                                                     \
  ccCleanFraudCallInfo(con);                                            \
}                                                                        

#if 0
#define CC_CONGESTION ((gdiCngLvl >= GDICONG_LEVEL_FOUR) ||                 \
                       (mgi_congestion_level == MGI_CONGESTION_LEVEL_TWO) ||\
                       (SChkDmndQCong_V2(&(ccDmndQCong_V2))))
#else
#define CC_CONGESTION (ccCongCb.ccDqExtCngCb.ccCongActnLvl != CCCONG_LEVEL_NORMAL)
#endif
/* FID 14457.2 */
#define CC_CONGESTION_LEVEL1 (ccCongCb.ccDqExtCngCb.ccCongActnLvl == CCCONG_LEVEL_ONE)
#define CC_CONGESTION_LEVEL2 (ccCongCb.ccDqExtCngCb.ccCongActnLvl == CCCONG_LEVEL_TWO)
#define CC_CONGESTION_LEVEL3 (ccCongCb.ccDqExtCngCb.ccCongActnLvl == CCCONG_LEVEL_THREE)
#define CC_CONGESTION_LEVEL4 (ccCongCb.ccDqExtCngCb.ccCongActnLvl == CCCONG_LEVEL_FOUR)
#define GET_GCC_ACTN_CONG_LEVEL (ccCongCb.ccDqExtCngCb.ccCongActnLvl)
#define GET_GCC_RLY_ACTN_CONG_LEVEL(_ccRlyCngCb) (_ccRlyCngCb->ccRCongActnLvl)
#define GET_GCC_MEGAA_ACTN_CONG_LEVEL(_ccMegaCngCb) (_ccMegaCngCb->ccMegaCngActnLvl)
#define PRIORITY_CALLS_REL_LOG_COUNT 100
#define NORMAL_CALLS_REL_LOG_COUNT   1000
#define CC_RLY_CONGESTION_LEVEL4(_ccRlyCngCb) (_ccRlyCngCb->ccRCongActnLvl == L4_CONGESTION)
#define CC_RLY_CONGESTION_LEVEL3(_ccRlyCngCb) (_ccRlyCngCb->ccRCongActnLvl == L3_CONGESTION)
#define CC_RLY_CONGESTION_LEVEL2(_ccRlyCngCb) (_ccRlyCngCb->ccRCongActnLvl == L2_CONGESTION)
#define CC_RLY_CONGESTION_LEVEL1(_ccRlyCngCb) (_ccRlyCngCb->ccRCongActnLvl == L1_CONGESTION)
#define CC_RLY_CONGESTION_NORMAL(_ccRlyCngCb) (_ccRlyCngCb->ccRCongActnLvl == NO_CONGESTION)
#define CC_RLY_CNG_CALL_RELEASEPCT ccDmndQCong_V2.callReleasePct
#define CC_MEGAA_CNG_CALL_RELEASEPCT ccDmndQCong_V2.callReleasePct

/* FID 2005*/
#define CC_MG_CONGESTION (gMgCong.currMgCong)

#define CC_MEGAA_CONGESTED (ccCongCb.ccMegaCngCb.ccMegaCngActnLvl)
#define CC_MEGAA_CONGESTION_LEVEL4 (ccCongCb.ccMegaCngCb.ccMegaCngActnLvl == MEGA_CONG_LVL4)

#define CC_GENTMR_TQNUMENT   32    /* Generic timer que */
#define CC_PACING_TIMER       2    /* Time used to pace releases */ 
                                   /* Between 1 to 2 sec */
#define CC_GENTMR_MAX        24    /* Number of Service Indication GCC gets,
                                      Right now, the max GCC can receive is
                                      the number of VOIP (PNA/ENA/Voice Server)
                                      cards it has + FID 7804 */
#define CC_MG_OVR_MEASURE_TMR  10  /* FID 7804 : 10 second timer to
                                      calculate the Current MG Overload */
#define CC_MIN_HCPL             0  /* FID 7804: Minimum value of HCPL */
#define CC_MAX_HCPL             1  /* FID 7804: Maximum value of HCPL */
#define cmHashListInsert(a, b, c, d) cmHashListInsert_check(a, b, c, d)

#ifndef CLOUD
#define BICC_PROXY_CCSID            0xFF
#else
#define BICC_PROXY_CCSID            0xFFFF
#endif
#define FRAUD_MAX_LOG_ENTRIES       10000
#define FRAUD_MAX_LOG_QUEUE_ENTRIES 10000

/* #defines for  con->fraudPrflbmap */
/* SCREEN DIGIT FRAUD CALL */
/* BIT 0 */
#define SET_SCR_REL_FLAG              0x01
#define RESET_SCR_REL_FLAG            0xFE

/* BIT 1 */
#define SET_SCR_ZERODUR_CDR_FLAG      0x02
#define RESET_SCR_ZERODUR_CDR_FLAG    0xFD

/* BIT 2 */
#define SET_SCR_LOGGING_FLAG          0x04
#define RESET_SCR_LOGGING_FLAG        0xFB

/* BIT 3 */
#define SET_SCR_SWITCH_INITREL_FLAG   0x08 
#define RESET_SCR_SWITCH_INITREL_FLAG 0xF7 

/* Answer Supervision Fraud Call */ 
/* BIT 4 */ 
#define SET_ANS_REL_FLAG              0x10 
#define RESET_ANS_REL_FLAG            0xEF

/* BIT 5 */ 
#define SET_ANS_ZERODUR_CDR_FLAG      0x20
#define RESET_ANS_ZERODUR_CDR_FLAG    0xDF

/* BIT 6 */
#define SET_ANS_LOGGING_FLAG          0x40
#define RESET_ANS_LOGGING_FLAG        0xBF

/* BIT 7 */
#define SET_ANS_SWITCH_INITREL_FLAG   0x80
#define RESET_ANS_SWITCH_INITREL_FLAG 0x7F 

/* Bug 44172 - return UNKNOWN when Type of Number is TON_UNK for AUS variant */
/* Bug 43914 - return UNKNOWN when Type of Number is TON_UNK for ITU97 variant */
/* Bug 80792 - return UNKNOWN when Type of Number is TON_UNK any variant */
#define CC_INCDPTY_2_SICDPTY(si, in, ret, prot)\
{\
   ret = ROK;\
   if ((in)->cdPrtyId.eh.pres)\
   {\
      (si)->cdPtyNum.eh.pres = PRSNT_NODEF;\
      (si)->cdPtyNum.innInd.pres = PRSNT_NODEF;\
      (si)->cdPtyNum.innInd.val = INN_NOTALLOW;\
      if ((in)->cdPrtyId.nmbPlanId.pres)\
      {\
         (si)->cdPtyNum.numPlan.pres = PRSNT_NODEF;   \
         switch((in)->cdPrtyId.nmbPlanId.val)\
         {\
            case NP_UNK:\
               (si)->cdPtyNum.numPlan.val = NP_UNK;\
               break;\
            case NP_ISDN:\
               (si)->cdPtyNum.numPlan.val = ISDNNUM;\
               break;\
            case NP_DATA:\
               (si)->cdPtyNum.numPlan.val = DATANUM;\
               break;\
            case NP_TELEX:\
               (si)->cdPtyNum.numPlan.val = TELEXNUM;\
               break;\
            case NP_PRIVATE:\
               (si)->cdPtyNum.numPlan.val = PRIVATENUMPLAN;\
               break;\
            case NP_NATIONAL:\
            case NP_EXT:\
            default:\
               ret = RFAILED;\
               DP("**%s: nmbPlanId not supported:%d\n",\
                   __FUNCTION__, (in)->cdPrtyId.nmbPlanId.val);\
         }\
      }\
      else\
      {\
         DP("*******%s: nmbPlanId not present :%d*********\n",\
             __FUNCTION__, (in)->cdPrtyId.nmbPlanId.pres);\
         ret = RFAILED;\
      }\
      if ((in)->cdPrtyId.typeNmb0.pres)\
      {\
         (si)->cdPtyNum.natAddrInd.pres = PRSNT_NODEF;   \
         switch((in)->cdPrtyId.typeNmb0.val)\
         {\
            case TON_INT:\
               (si)->cdPtyNum.natAddrInd.val = NA_INTNATNUM;\
               break;\
            case TON_NAT:\
               (si)->cdPtyNum.natAddrInd.val = NA_NATNUM;\
               break;\
            case TON_SUB:\
               (si)->cdPtyNum.natAddrInd.val = NA_SUBSNUM;\
               break;\
            case TON_NONMMBR_OPERATOR:\
               (si)->cdPtyNum.natAddrInd.val = NA_NONUMOPRREQ;\
               break;\
            case TON_INT_OPERATOR:\
               (si)->cdPtyNum.natAddrInd.val = NA_INTNATNUMOPRREQ;\
               break;\
            case TON_NAT_OPERATOR:\
               (si)->cdPtyNum.natAddrInd.val = NA_NATNUMOPRREQ;\
               break;\
            /* ANSI92, BELLCORE, ANSI95: 950+ call */\
            case TON_NAI118:\
               (si)->cdPtyNum.natAddrInd.val = NA_NAI118;\
               break;\
            /* KPN: in-ported number */\
            case TON_NAI125:\
               (si)->cdPtyNum.natAddrInd.val = NA_NAI125;\
               break;\
            /* KPN: For Number Portability; UK: For UKSPECIFIC */\
            case TON_NAI126:\
               (si)->cdPtyNum.natAddrInd.val = NA_NAI126;\
               break;\
            case TON_SUBS_OPERATOR:\
               (si)->cdPtyNum.natAddrInd.val = NA_SUBSNUMOPRREQ;\
               break;\
            case TON_NONUMCUTTHRU:\
               (si)->cdPtyNum.natAddrInd.val = NA_NONUMCUTTHRU;\
               break;\
            case TON_ABB:\
               (si)->cdPtyNum.natAddrInd.val = NA_RNNATNUM;\
                break;\
            case TON_CON:\
               (si)->cdPtyNum.natAddrInd.val = NA_RNCDN;\
                break;\
            case TON_ATTAPNNMBR:\
               (si)->cdPtyNum.natAddrInd.val = NA_ATTAPNNUM;\
                break;\
            case TON_NRP:\
               (si)->cdPtyNum.natAddrInd.val = NA_NRP;\
               break;\
            case TON_NAI112:\
               (si)->cdPtyNum.natAddrInd.val = NA_NATDEFUNK;\
               break;\
            case TON_RES:\
               (si)->cdPtyNum.natAddrInd.val = NA_NETSPNUM;\
               break;\
            case TON_EXT:\
               (si)->cdPtyNum.natAddrInd.val = NA_NETRTNUMSPEC;\
               break;\
            case TON_NETSPEC:\
               ret = RFAILED;\
               DP("**%s: typeNmb0 not supported:%d\n",\
                     __FUNCTION__, (in)->cdPrtyId.typeNmb0.pres);\
            case TON_UNK:\
               (si)->cdPtyNum.natAddrInd.val = NA_UNKNOWN;\
               break;\
            default:\
               if ((in)->cdPrtyId.nmbDigits.len > 9)\
               (si)->cdPtyNum.natAddrInd.val = NA_NATNUM;\
               else\
               (si)->cdPtyNum.natAddrInd.val = NA_SUBSNUM;\
               DP("**%s: typeNmb0.val= %d, natAddrInd.val= %d \n",\
                        __FUNCTION__, (in)->cdPrtyId.typeNmb0.val, (si)->cdPtyNum.natAddrInd.val);\
               break;\
         }\
      }\
      else\
      {\
         ret = RFAILED;\
         DP("*******%s: typeNmb0 not present :%d*********\n",\
             __FUNCTION__, (in)->cdPrtyId.typeNmb0.pres);\
      }\
      in2siAddrsMap(&(si)->cdPtyNum.oddEven, (TknStrM *)&(in)->cdPrtyId.nmbDigits,\
                    &(si)->cdPtyNum.addrSig, 0);\
   }\
}

#define FORCED_ECHO(con)                               \
  (((con->icIntfcCb) && (con->icIntfcCb->forcedEcho == TRUE)) ||     \
      ((con->ogIntfcCb) && (con->ogIntfcCb->forcedEcho == TRUE)))    \

/* FID 14677.0 */
#define ENHANCED_ECHO(con)                                              \
  ((con->icIntfcCb) &&                                                  \
   ((con->icIntfcCb->echoCntrlVariants == CC_ENH_ECHO_CNTRL_TYPE1) ||   \
    (con->icIntfcCb->echoCntrlVariants == CC_ENH_ECHO_CNTRL_TYPE2)))    \

#define IS_ECHO_ENH_CNTRL_TYPE_1(con)                                   \
  ((con->icIntfcCb) &&                                                  \
   (con->icIntfcCb->echoCntrlVariants == CC_ENH_ECHO_CNTRL_TYPE1))      \

#define IS_ECHO_ENHTYPE1_AND_INVOKE(con)                                \
  ((con->icIntfcCb) &&                                                  \
   (con->icIntfcCb->echoCntrlVariants == CC_ENH_ECHO_CNTRL_TYPE1) &&    \
   (con->echoParam.EnhEchoLogicVal & CC_EC_INVOKE))                     \

#define IS_ECHO_BWDMSG(con)                                             \
  ((con->icEvntType == CC_ET_ACM) ||                                    \
   (con->icEvntType == CC_ET_CPG) ||                                    \
   (con->icEvntType == CC_ET_NRESM) ||                                  \
   (con->icEvntType == CCSIP_ET_180_RINGING) ||                         \
   (con->icEvntType == CCSIP_ET_183_CALLPRG))                           \

#define SET_IC_AND_OG_MODE_FOR_EC(con, currentIcPkgId, currentIcSignal, \
   currentIcMode, currentOgPkgId, currentOgSignal, currentOgMode,       \
   icMode, icSignal, ogMode, ogSignal)                                  \
{                                                                       \
   ccCheckMgContext(&con->icBcm, &currentIcPkgId, &currentIcSignal,     \
       &currentIcMode);                                                 \
   icMode = currentIcMode;                                              \
   icSignal = currentIcSignal;                                          \
   ccCheckMgContext(&con->ogBcm, &currentOgPkgId, &currentOgSignal,     \
       &currentOgMode);                                                 \
   ogMode = currentOgMode;                                              \
   ogSignal = currentOgSignal;                                          \
} 

/* 
*** 
*/
#define IS_OG_LEG_DMS(con) ({ccIsLegDMS(&con->ogRsc.intfc, con->ogIntfcCb);})
#define IS_IC_LEG_DMS(con) ({ccIsLegDMS(&con->icRsc.intfc, con->icIntfcCb);})
#define IS_IC_LEG_DMS_FOR_INT_BICC(con) ({ccIsLegDMS(&con->icIngressCcsRscIntfc, \
                                         con->icIntfcCb);})

#define MINNOSWTCOUNT 1
#define MAXNOSWTCOUNT 0xFFFFFFFF 

#define CC_SET_CGPTYNMB_NIIND(siOut)                   \
{                                                      \
  if ((siOut)->cgPtyNum.eh.pres == PRSNT_NODEF)        \
  {                                                    \
    if ((siOut)->cgPtyNum.niInd.pres != PRSNT_NODEF)   \
    {                                                  \
      (siOut)->cgPtyNum.niInd.pres = PRSNT_NODEF;      \
      (siOut)->cgPtyNum.niInd.val = NBMCMLTE;          \
    }                                                  \
  }                                                    \
}

#define CC_DISC_FLAG(con) (con->ainInfo.disconnectLegFlag)

#define CC_REORIGINATE_FLAG(con) (ccReOriginateFlagSet(con))

#define CC_MERGE_FLAG(con) (ccMergeFlagSet(con))

#define CC_INGRESS_INTER_CCS_CALL(con)   ((con) && (con->icBcm.interCcsCall))
#define CC_EGRESS_INTER_CCS_CALL(con)    ((con) && (con->ogBcm.interCcsCall))

#define CC_BICC_PROXY_INTFC_ID 0x00
#define CC_BICC_LOCAL_INTFC_ID 0x01

/* Bug 83726 ++ */
#define CC_GET_EVNTHOLD_DIR(direction)          ((direction)&0x0F)
#define CC_GET_RELEVNTHOLD_DIR(direction)       (((direction)&0xF0)>>4)
/* Bug 83726 -- */

/* BUG:24582-mqin */
#define CHK_OPERATOR_FOR_ISDN(con, in) \
{ \
   if (in->cdPtyNmb.eh.pres && \
       in->cdPtyNmb.typeNmb0.pres && \
       (in->cdPtyNmb.typeNmb0.val == TON_NONMMBR_OPERATOR)) \
   { \
      in->cdPtyNmb.nmbDigits.pres = PRSNT_NODEF; \
      if (con->routeInfo->addrInd == CC_CDPTY && \
          !in->cdPtyNmb.nmbDigits.len) \
      { \
         in->cdPtyNmb.nmbDigits.len = 1; \
         in->cdPtyNmb.nmbDigits.val[0] = '0'; \
         in->cdPtyNmb.nmbDigits.val[1] = '\0'; \
      } \
      else if ((con->routeInfo->addrInd == CC_TRANNET) && \
               !in->cdPtyNmb.nmbDigits.len) \
      { \
         in->cdPtyNmb.nmbDigits.len = 2; \
         in->cdPtyNmb.nmbDigits.val[0] = '0'; \
         in->cdPtyNmb.nmbDigits.val[1] = '0'; \
         in->cdPtyNmb.nmbDigits.val[2] = '\0'; \
      } \
   } \
}

/* MCAO_DEV WILTEL */
#define CC_CDPTYNMB_UNKNOWN       0
#define CC_CDPTYNMB_WILTEL_CDR    1

/* Feature 2361 */
#define CC_OUTPULSENMB_NOA_NPI    0

#define CC_GET_PHYDS0(ifType, ds0, rsc, skip)   \
{                                               \
    (skip) = FALSE;                             \
                                                \
    switch(ifType)                              \
    {                                           \
        /* TDM */                               \
    case CC_SI_INTFC:                           \
        (ds0) = (rsc).t.nbRsc.phyDs0;           \
        break;                                  \
    case CC_IN_INTFC:                           \
        (ds0) = (rsc).t.dss1Rsc.phyDs0;         \
        break;                                  \
    case CC_G3_INTFC:                           \
    case CC_G3_SUBIF:                           \
        (ds0) = (rsc).t.gr3Rsc.ds0Rsc;          \
        break;                                  \
    case CC_CS_LN_INTFC:                        \
    case CC_CS_TG_INTFC:                        \
        (ds0) = (rsc).t.ds0Rsc;                 \
         break;                                 \
    case CC_AM_INTFC:                           \
    case CC_BI_INTFC:                           \
    case CC_BICC_INTFC:                         \
        (skip) = TRUE;                          \
        break;                                  \
        /* Packets */                           \
    case CC_SIP_INTFC:                          \
    case CC_MGCP_LN_INTFC:                      \
    case CC_MGCP_TG_INTFC:                      \
    default:                                    \
        (skip) = TRUE;                          \
        break;                                  \
    }                                           \
}

/* BUG:36821-mqin */
#define  CC_GET_IN_VARIANT(pSap, inVar) \
{ \
  if (pSap && pSap->suId) \
  { \
     switch (pSap->suId) \
     { \
      case 14: \
         inVar = CC_INVARNI2; \
         break; \
      case 14 + 1: \
         inVar = CC_INVARDMS100; \
         break; \
      case 14 + 4: \
         inVar = CC_INVAR5E; \
         break; \
      case 14 + 5: \
         inVar = CC_INVARITU; \
         break; \
      case 14 + 6: \
         inVar = CC_INVARETSI; \
         break; \
      default: \
         inVar = CC_INVARNI2; \
         break; \
     } \
  } \
  else \
     inVar = CC_INVARNI2; \
}

/* #-defines for RLT/REFER code */
#define CC_RMT_XFER_INPROGRESS  0
#define CC_RMT_XFER_FINAL       1

 /* MCAO_Bug:25860 converting ASCII to BCD */
 #define CM_ASCII_TO_BCD(ch, dig) \
 switch(ch) \
{ \
   case '0'...'9': \
      dig = ((ch) - '0') & 0x0F; \
      break; \
   case 'a'...'e': \
      dig = ((ch) - 'a' + 0xa) & 0x0F; \
      break; \
   case 'A'...'E': \
      dig = ((ch) - 'A' + 0xa) & 0x0F; \
      break; \
   default: \
      dig = 0; \
      CCLOGERR("unrecognized digits!\n"); \
      break; \
}

#define MAX_CALEALEGS_PER_SUB 5

/* + FID 17163.1 */
#define CC_IS_PARALLEL_FORK_CUTTHRU_18X(con)                        \
       ((ccIsParallelForkCutThru(con)) &&                           \
        ((con->icEvntType == CCSIP_ET_180_RINGING) ||               \
         (con->icEvntType == CCSIP_ET_181_FORWDNG) ||               \
         (con->icEvntType == CCSIP_ET_182_QUEUED) ||                \
         (con->icEvntType ==  CCSIP_ET_183_CALLPRG)) &&             \
        (CCT_IS_SIP_PARALLEL_FORK_SUPPORTED(con->ogSipMethodSupMask)))

#define CC_IS_PARALLEL_FORK_CUTTHRU_18X_NOT180(con)                 \
       ((ccIsParallelForkCutThru(con)) &&                           \
        ((con->icEvntType == CCSIP_ET_181_FORWDNG) ||               \
         (con->icEvntType == CCSIP_ET_182_QUEUED) ||                \
         (con->icEvntType ==  CCSIP_ET_183_CALLPRG)) &&             \
        (CCT_IS_SIP_PARALLEL_FORK_SUPPORTED(con->ogSipMethodSupMask)))
/* -  FID 17163.1 */

/* + FID 16118.0 */
#define CC_IS_AGNOSTIC_CANDIDATE_WITH_NO_OFFER_CALL(con)              \
        ((con->icIntfcCb) && (con->ogIntfcCb) &&                      \
         ((CC_GET_PROTOCOL(con->icProtType) == CC_SIP) ||             \
          (CC_GET_PROTOCOL(con->icProtType) == CC_SIPT)) &&           \
         ((CC_GET_PROTOCOL(con->ogProtType) == CC_SIP) ||             \
          (CC_GET_PROTOCOL(con->ogProtType) == CC_SIPT)) &&           \
         (CC_IS_CALL_FLAG_SET(con->flag,CC_FLAG_IP_CALL_NO_INITIAL_SDP))&&\
         (!CC_IS_X3_ALLOWED(con))                       &&            \
         (((con->ccmCb.ccn == CCN_0) &&                               \
           (ccCheckEnableMediaAgnostic(con, 0, TRUE) == ROK)) ||            \
          ((con->ccmCb.ccn == CCN_1) &&                               \
          (ccCheckEnableMediaAgnostic(con, CC_EGRESS_MG, TRUE) == ROK))))
/* - FID 16118.0 */

#define CC_IS_CONTEXT_PRESENT(con) ccIsContextPresent(con)

/* FID 15261.0 + */
#define CC_IS_BCM_IN_CONTEXT(bcm)                                 \
         ((bcm) && ((bcm)->mgCtx) &&                              \
          ((bcm)->mgCtx->contextId > 0) &&                        \
          ((bcm)->mgCtx->contextId != MGCT_CONTEXT_CHOOSE) &&     \
          ((bcm)->mgCtx->contextId != MGCT_CONTEXT_ALL))

#define CC_IS_CALL_MULTI_MG(_con)                                            \
         (((_con)->ccmCb.ccn == CCN_1) || ((_con)->ccmCb.ccn == CCN_3) ||    \
          ((_con)->ccmCb.ccn == CCN_4) || ((_con)->ccmCb.ccn == CCN_5))

#define CC_OPERATING_INGRESS_MG(_con)                                        \
         (CC_EGRESS_INTER_CCS_CALL(_con) || (CC_IS_CALL_MULTI_MG(_con) &&    \
          ((_con)->ccmCb.state == CC_INGRESS_MG)))

#define CC_OPERATING_EGRESS_MG(_con)                                          \
         (CC_INGRESS_INTER_CCS_CALL(_con) || (CC_IS_CALL_MULTI_MG(_con) &&    \
          ((_con)->ccmCb.state == CC_EGRESS_MG)))

#define CC_OPERATING_LAST_MG(_con)                                                      \
         ((((_con)->direction == INCTOOUT) && ((_con)->ccmCb.state == CC_EGRESS_MG)) || \
          (((_con)->direction == OUTTOINC) && ((_con)->ccmCb.state == CC_INGRESS_MG)))

#define CC_OPERATING_FIRST_MG(_con)                                                      \
         ((((_con)->direction == INCTOOUT) && ((_con)->ccmCb.state == CC_INGRESS_MG)) || \
          (((_con)->direction == OUTTOINC) && ((_con)->ccmCb.state == CC_EGRESS_MG)))

#define CC_IS_LOCAL_MG_USED(_con)                                           \
         ((((_con)->ccmCb.ccn == CCN_0) || ((_con)->ccmCb.ccn == CCN_2)) && \
          ((_con)->icBcm.mgSpId == ((CcGenSap *)(*ccSfSapLstPtr))->spId))   \

#define CC_IS_EGRESS_MGLIST_USED(_con)                                      \
         ((CC_OPERATING_EGRESS_MG(_con)) ||                                 \
          (((_con)->ccmCb.ccn == CCN_5) &&                                  \
           ((_con)->icBcm.mgSpId == (_con)->ogBcm.mgSpId) &&                \
           (CC_OPERATING_INGRESS_MG(_con))) ||                              \
          ((((_con)->ccmCb.ccn == CCN_0) ||                                 \
           ((_con)->ccmCb.ccn == CCN_2)) &&                                 \
          (ccIsBearerNATEgress(_con) ||                                     \
           (ccIsTranscodingReq(_con) &&                                     \
            !CC_IS_ICMG_LIST_POPULATED(_con) &&                             \
            !ccIsBearerNATIngress(_con))) &&                                \
          (CC_IS_OGMG_LIST_POPULATED(_con))))

#define CC_IS_INGRESS_MGLIST_USED(_con)                                     \
         ((CC_OPERATING_INGRESS_MG(_con)) ||                                \
          ((((_con)->ccmCb.ccn == CCN_0) ||                                 \
            ((_con)->ccmCb.ccn == CCN_2)) &&                                \
           (ccIsBearerNATIngress(_con) ||                                   \
            (ccIsTranscodingReq(_con) &&                                    \
             !ccIsBearerNATEgress(_con))) &&                                \
           (CC_IS_ICMG_LIST_POPULATED(_con))))

#define CC_IS_ICMG_LIST_POPULATED(_con)                  \
         ((_con) && (_con->routeEvnt) &&                 \
          (((_con)->routeEvnt->inCacInfo.numMgwInfo) ||  \
           ((_con)->routeEvnt->inCacInfo.listMgScreened)))

#define CC_IS_MGW_AVAIL_ICMG_LIST(_con)                  \
         ((_con) && (_con->routeEvnt) &&                 \
          (_con->routeEvnt->inCacInfo.currentMgw + 1) <  \
          (_con->routeEvnt->inCacInfo.numMgwInfo))

#define CC_NUM_MGW_ON_ICMG_LIST(_con)                  \
         (((_con) && (_con->routeEvnt))?(_con->routeEvnt->inCacInfo.numMgwInfo):0)

#define CC_NUM_MGW_ON_OGMG_LIST(_con)                                                \
         (((_con) && (_con->routeEvnt) && (_con->routeEvnt->currentInterface >= 0))? \
           (_con->routeEvnt->cacInfo[CC_EGRESS_INTER_CCS_CALL(_con)?                 \
                                     _con->routeEvnt->currentInterface:              \
                                     (_con->routeEvnt->currentInterface - 1)].numMgwInfo):0)

#define CC_IS_OGMG_LIST_POPULATED(_con)                                                     \
         ((_con) && (_con->routeEvnt) && (_con->routeEvnt->currentInterface >= 0) &&        \
          ((_con->routeEvnt->cacInfo[CC_EGRESS_INTER_CCS_CALL(_con)?                        \
                                     _con->routeEvnt->currentInterface:                     \
                                     (_con->routeEvnt->currentInterface - 1)].numMgwInfo) ||\
           (_con->routeEvnt->cacInfo[CC_EGRESS_INTER_CCS_CALL(_con)?                       \
                                     _con->routeEvnt->currentInterface:                     \
                                     (_con->routeEvnt->currentInterface - 1)].listMgScreened)))

#define CC_LOCATE_RMT_SDP(_con, _rmtSdp) \
{ \
  _rmtSdp = NULLP; \
  if (_con) \
  { \
    if CC_IS_CALL_MULTI_MG(_con) \
    { \
      if ((_con)->direction == OUTTOINC) \
      { \
        if ((_con)->ccmCb.state == CC_EGRESS_MG) \
        { \
          _rmtSdp = (_con)->ogIpParam.rmtDesc; \
        } \
        else /* CC_INGRESS_MG or CC_Y_MG */ \
        { \
          if ((_con)->ccmCb.state == CC_INGRESS_MG) \
          { \
            if ((_con)->icLiAdjIpParam) \
              _rmtSdp = (_con)->icLiAdjIpParam->locDesc; \
            else if ((_con)->ogAdjIpParam) \
              _rmtSdp = (_con)->ogAdjIpParam->locDesc; \
          } \
          else if ((_con)->ccmCb.state == CC_Y_MG) \
          { \
            if ((_con)->ogAdjIpParam) \
              _rmtSdp = (_con)->ogAdjIpParam->locDesc; \
          } \
        } \
      } \
      else \
      { \
        if ((_con)->ccmCb.state == CC_INGRESS_MG) \
        { \
          _rmtSdp = (_con)->icIpParam.rmtDesc; \
        } \
        else if ((_con)->ccmCb.state == CC_EGRESS_MG) \
        { \
          if ((_con)->ogLiAdjIpParam) \
            _rmtSdp = (_con)->ogLiAdjIpParam->locDesc; \
          else if ((_con)->icAdjIpParam) \
            _rmtSdp = (_con)->icAdjIpParam->locDesc; \
        } \
        else if ((_con)->ccmCb.state == CC_Y_MG) \
        { \
          if (_con->icAdjIpParam) \
            _rmtSdp = (_con)->icAdjIpParam->locDesc; \
        } \
      } \
    } \
    else \
    { \
      if ((_con)->direction == OUTTOINC) \
       {\
        if ((_con) &&((_con)->icConCurBcm))\
        {\
          if ((_con)->ccmCb.state == CC_EGRESS_MG)\
          {\
              _rmtSdp = (_con)->icIpParam.rmtDesc; \
          }\
          else if ((_con)->ccmCb.state == CC_INGRESS_MG)\
          {\
               _rmtSdp = (_con)->icConCurIpParam->rmtDesc; \
          }\
        }\
        else \
          {\
          _rmtSdp = (_con)->ogIpParam.rmtDesc; \
          }\
       }\
      else \
       {\
        if ((_con) &&((_con)->icConCurBcm))\
        {\
          if ((_con)->ccmCb.state == CC_INGRESS_MG)\
          {\
              _rmtSdp = (_con)->icIpParam.rmtDesc; \
          }\
          else if ((_con)->ccmCb.state == CC_EGRESS_MG)\
          {\
               _rmtSdp = (_con)->icConCurIpParam->rmtDesc; \
          }\
        }\
        else \
        {\
          _rmtSdp = (_con)->icIpParam.rmtDesc; \
        }\
       }\
    } \
  } \
}

#define CC_DERIVE_IPPARAM(_bcm, _ipParam) \
{ \
  _ipParam = NULLP; \
  if (_bcm) \
  { \
    if ((_bcm)->bcmType == BtOBcm) \
    { \
      _ipParam = &(_bcm)->con->icIpParam; \
    } \
    else if ((_bcm)->bcmType == BtTBcm) \
    { \
      _ipParam = &(_bcm)->con->ogIpParam; \
    } \
    else if ((_bcm)->bcmType == BtOAdjBcm) \
    { \
      if ((_bcm)->con->icAdjIpParam) \
        _ipParam = (_bcm)->con->icAdjIpParam; \
    } \
    else if ((_bcm)->bcmType == BtTAdjBcm) \
    { \
      if ((_bcm)->con->ogAdjIpParam) \
        _ipParam = (_bcm)->con->ogAdjIpParam; \
    } \
    else if ((_bcm)->bcmType == BtOLiBcm) \
    { \
      if ((_bcm)->con->icLiIpParam) \
        _ipParam = (_bcm)->con->icLiIpParam; \
    } \
    else if ((_bcm)->bcmType == BtTLiBcm) \
    { \
      if ((_bcm)->con->ogLiIpParam) \
        _ipParam = (_bcm)->con->ogLiIpParam; \
    } \
    else if ((_bcm)->bcmType == BtOLiAdjBcm) \
    { \
      if ((_bcm)->con->icLiAdjIpParam) \
        _ipParam = (_bcm)->con->icLiAdjIpParam; \
    } \
    else if ((_bcm)->bcmType == BtTLiAdjBcm) \
    { \
      if ((_bcm)->con->ogLiAdjIpParam) \
        _ipParam = (_bcm)->con->ogLiAdjIpParam; \
    } \
  } \
}

#define CC_DERIVE_BCMS(_con, _pTmpIcBcm, _pTmpOgBcm) \
{ \
  if (CC_IS_CALL_MULTI_MG(_con)) \
  { \
    if (_con->ccmCb.state == CC_INGRESS_MG) \
    { \
      _pTmpIcBcm = &(_con)->icBcm; \
      _pTmpOgBcm = (_con)->icAdjBcm; \
    } \
    else if (_con->ccmCb.state == CC_EGRESS_MG) \
    { \
      _pTmpIcBcm = (_con)->ogAdjBcm; \
      _pTmpOgBcm = &(_con)->ogBcm; \
    }\
    else if (_con->ccmCb.state == CC_Y_MG) \
    { \
      _pTmpIcBcm = (_con)->icLiAdjBcm;\
      _pTmpOgBcm = (_con)->ogLiAdjBcm; \
    }\
  } \
  else \
  { \
    if(CC_IS_STEROMONO_CALL(_con) && (_con->ccmCb.state == CC_EGRESS_MG))\
     {\
        if (con->direction == INCTOOUT) \
        {\
        _pTmpIcBcm = (_con)->icConCurBcm; \
        _pTmpOgBcm = &(_con)->ogBcm; \
        }\
        else \
        {\
           _pTmpIcBcm = &(_con)->icBcm; \
           _pTmpOgBcm = &(_con)->ogBcm; \
        }\
     }\
     else if(CC_IS_STEROMONO_CALL(_con) && (_con->ccmCb.state == CC_INGRESS_MG))\
     {\
        if (con->direction == OUTTOINC) \
        {\
        _pTmpIcBcm = (_con)->icConCurBcm; \
        _pTmpOgBcm = &(_con)->ogBcm; \
        }\
        else \
        {\
           _pTmpIcBcm = &(_con)->icBcm; \
           _pTmpOgBcm = &(_con)->ogBcm; \
        }\
     }\
    else \
     {\
        _pTmpIcBcm = &(_con)->icBcm; \
        _pTmpOgBcm = &(_con)->ogBcm; \
     }\
  } \
}

/* Context Configuration Numbers */
#define CCN_0  0
#define CCN_1  1
#define CCN_2  2
#define CCN_3  3
#define CCN_4  4
#define CCN_5  5

/* Context Configuration States */
#define CC_MG_NULL     0
#define CC_INGRESS_MG  1
#define CC_Y_MG        2
#define CC_EGRESS_MG   3

/* Context Configuration Flags */
#define CC_MG_SUB_ERROR  0x1
#define CC_STR_MG_LIST   0x2
#define CC_FAX_TRIGGER   0x4
#define CC_IW_TDM        0x8
#define CC_TDM_HOLD      0x10
#define CC_FAX_REV_VOICE 0x20 /* BUG88977 fax revert to voice */
/* FID 15261.0 - */

#define CC_FIND_MGL_VAR_CTX(mgCtx)\
{\
  if((mgCtx == NULLP) || (mgCtx->mgSapCb == NULLP))\
  {\
    CCLOGERR("Error:  mgCtx or its mgSapCb is NULLP! ");  \
    RETVALUE(NULLP);                               \
  }\
  else\
  {\
    RETVALUE(mgCtx->mgSapCb.mglVarCtx);            \
  }\
}\

#define IS_CALEA_TBCM(pBcm) ((pBcm->bcmType == BtTBcm) &&\
    (pBcm->con->icProtType == CC_LOCAL_SWT_PROT))

#define CC_SET_IAM_ECHO_IND(con)\
{ \
  if (con->ccEvntHold->m.ccConEvnt.m.siConEvnt.fwdCallInd.intInd.pres)\
    con->iamInterwrkgInd = \
       con->ccEvntHold->m.ccConEvnt.m.siConEvnt.fwdCallInd.intInd.val;\
    if (con->ccEvntHold->m.ccConEvnt.m.siConEvnt.natConInd.\
        echoCntrlDevInd.pres)\
    { \
      if (con->ccEvntHold->m.ccConEvnt.m.siConEvnt.natConInd.\
          echoCntrlDevInd.val == ECHOCDEV_INCL) \
      { \
        con->echoParam.IngressEchoVal |= CC_IC_IAM_ECHO_IND_SET;\
      }\
    }\
}

/* RV Suresh, Propagation Value changes in GCC */

#define CC_TDM_TDM_DELAY_VAL 2
#define CC_TDM_IP_DELAY_VAL 32
#define CC_DELAY_ADDED_BY_CLAM 10


#define UNPACK_1BYTE_OR_2BYTE_LEN(tag) ((tag == BICC_INT_FCBILLINFO) ||\
        (tag == BICC_INT_AINBILLINFO) || (tag == BICC_INT_SIPHISTINFO) || (tag == BICC_INT_CDPTYHOSTNAME) \
        || ( tag == BICC_INT_RSCPRIORITY) ||(tag == BICC_INT_SIPTRANSHDR) || (tag == BICC_INT_OGBILLINGSDP)\
        ||(tag == BICC_INT_PSTNXML_INFO) || (tag == BICC_INT_DESTCODE) ||\
        (tag == BICC_INT_CALLRRTINFO) || (tag == BICC_INT_CCBSCALLINFO) || (tag == BICC_INT_CCBSFACINFO) || \
         (tag == BICC_INT_SIPSURI))

/* Added for Configurable Network Suspend timer, 
 *
 * Changes made for TELSTRA and Mexico, RV Suresh 5.0 */
#define CC_FILL_SUSPEND_TIMER(icProtType,tmr)\
{ \
      /* incoming protocol is CAS Trunk */\
      if (icProtType == CC_CS_TG)\
      {\
          tmr.tSUSP.enb = tmr.tSUSPCas.enb;\
          tmr.tSUSP.val = tmr.tSUSPCas.val;\
      }\
      /* incoming protocol is ISUP. For Telstra, this is configure as 0.\
       * Bug 64193 for SIP-T to ISUP call, the SUS timer should be Tsuspisup */\
      else if(( icProtType == CC_SI)||(icProtType == CC_SIPT))\
      {\
          tmr.tSUSP.enb = tmr.tSUSPIsup.enb;\
          tmr.tSUSP.val = tmr.tSUSPIsup.val;\
      }\
      /* incoming protocol is Line Side access */\
      else\
      {\
          tmr.tSUSP.enb = tmr.tSUSPOrig.enb;\
          tmr.tSUSP.val = tmr.tSUSPOrig.val;\
      }\
}

#define CC_IS_PROT_TYPE_ANSI_ISUP(protType)\
   ((((protType) == CC_SIANS92) || ((protType) == CC_BICCANS92) ||\
     ((protType) == CC_SIANS95) || ((protType) == CC_BICCANS95) ||\
     ((protType) == CC_SIPT) || ((protType) == CC_SIPTANS92) || \
     ((protType) == CC_SIPTANS95)) ? TRUE : FALSE)

/* FID 16747.0 */
#define CC_IS_PROT_TYPE_ANSI_BICC(_intfcCb)\
   (((_intfcCb) && (CC_GET_PROTOCOL(_intfcCb->protType) == CC_EXT_BICC) &&\
     (CC_OPT_IWBASEVARIANT(_intfcCb) == CC_BASE_ISUPANS92))? TRUE : FALSE)

#define CC_IS_PROT_TYPE_MEXICO(protType)\
   (((protType == CC_SIMEXICO) ||\
     (protType == CC_BICCMEXICO) ||\
     (protType == CC_SIPTMEXICO)) ? TRUE : FALSE)

#define CC_IS_PROT_TYPE_G500(protType)\
   (((protType == CC_SIG500V2) ||\
     (protType == CC_SIG500V0) ||\
     (protType == CC_BICCG500V2) ||\
     (protType == CC_BICCG500V0) ||\
     (protType == CC_SIPTG500V2) ||\
     (protType == CC_SIPTG500V0)) ? TRUE : FALSE)

#define CC_IS_PROT_TYPE_G500V0(protType)\
    (((protType == CC_SIG500V0) ||\
      (protType == CC_BICCG500V0) ||\
      (protType == CC_SIPTG500V0)) ? TRUE : FALSE)

#define CC_IS_PROT_TYPE_TELSTRA(protType)\
   (((protType == CC_SITELSTRA) ||\
     (protType == CC_BICCTELSTRA) ||\
     (protType == CC_SIPTTELSTRA)) ? TRUE : FALSE)

#define CC_IS_PROT_TYPE_AUSTRALIA(protType)\
   (((CC_IS_PROT_TYPE_TELSTRA(protType)) ||\
     (CC_IS_PROT_TYPE_G500(protType))) ? TRUE : FALSE)

/* BUG: 43914 */
#define CC_IS_PROT_TYPE_ITU97(protType)\
   (((protType == CC_SIITU97) ||\
     (protType == CC_BICCITU97)) ? TRUE : FALSE)

#define CC_IS_VARIANT_OPTIONAL_BCKWARD_TYPE_Q_BASED(protType)\
   (((protType == CC_SIMEXICO) || (protType == CC_BICCMEXICO) || (protType == CC_SIPTMEXICO) ||\
     (protType == CC_SI76792) || (protType == CC_BICC76792) || (protType == CC_SIPT76792) ||\
     (protType == CC_SINTT) || (protType == CC_BICCNTT) || (protType == CC_SIPTNTT) ||\
     (protType == CC_SIVENEZUELA) || (protType == CC_BICCVENEZUELA) || (protType == CC_SIPTVENEZUELA) ||\
     (protType == CC_SISPAIN) || (protType == CC_BICCSPAIN) || (protType == CC_SIPTSPAIN) ||\
     (protType == CC_SIBRAZIL) || (protType == CC_BICCBRAZIL) || (protType == CC_SIPTBRAZIL) ||\
     (protType == CC_SITHAILAND) || (protType == CC_BICCTHAILAND) || (protType == CC_SIPTTHAILAND) ||\
     (protType == CC_SIPOLAND) || (protType == CC_BICCPOLAND) || (protType == CC_SIPTPOLAND)) ? TRUE : FALSE)

#define CC_IS_PROT_TYPE_SPAIN(protType)\
   (((protType == CC_SISPAIN) ||\
     (protType == CC_BICCSPAIN) ||\
     (protType == CC_SIPTSPAIN)) ? TRUE : FALSE)

#define CC_IS_PROT_TYPE_VENEZUELA(protType)\
   (((protType == CC_SIVENEZUELA) ||\
     (protType == CC_BICCVENEZUELA) ||\
     (protType == CC_SIPTVENEZUELA)) ? TRUE : FALSE)

#define CC_IS_PROT_TYPE_POLAND(protType)\
   (((protType == CC_SIPOLAND) ||\
     (protType == CC_BICCPOLAND) ||\
     (protType == CC_SIPTPOLAND)) ? TRUE : FALSE)

#define CC_IS_PROTOCOL_HANDLING_UBUF(protocol)\
      (((protocol == CC_SI) ||\
       (protocol == CC_EXT_BICC) ||\
       (protocol == CC_BICC) ||\
       (protocol == CC_IN) ||\
       (protocol == CC_SIPT)) ? TRUE : FALSE)

#define CC_IS_PROT_TYPE_GERMANY(protType)\
   (((protType == CC_SIGERMANY) ||\
     (protType == CC_BICCGERMANY) ||\
     (protType == CC_SIPTGERMANY)) ? TRUE : FALSE)

#define CC_IS_PROT_TYPE_CHINA(protType)\
   (((protType == CC_SICHINA) ||\
     (protType == CC_BICCCHINA)||\
     (protType == CC_SIPTCHINA)) ? TRUE : FALSE)

#define CC_IS_PROT_TYPE_RUSSIA(protType)\
   (((protType == CC_SIRUSSIA) ||\
     (protType == CC_BICCRUSSIA)||\
     (protType == CC_SIPTRUSSIA)) ? TRUE : FALSE)

#define CC_IS_PROT_TYPE_COSTARICA(protType)\
   (((protType == CC_SICOSTARICA) ||\
     (protType == CC_BICCCOSTARICA) ||\
     (protType == CC_SIPTCOSTARICA)) ? TRUE : FALSE)

#define CC_IS_PROT_TYPE_VEMOVILNET(protType)\
   (((protType == CC_SIVEMOVILNET) ||\
     (protType == CC_BICCVEMOVILNET)||\
     (protType == CC_SIPTVEMOVILNET)) ? TRUE : FALSE)

#define CC_IS_PROT_TYPE_ARGENTINA(protType)\
   (((protType == CC_SIARGENTINA) ||\
     (protType == CC_BICCARGENTINA)||\
     (protType == CC_SIPTARGENTINA)) ? TRUE : FALSE)

 /* Feature 2170 */
#define CC_IS_PROT_TYPE_KOREAN(protType)\
   (((protType == CC_SIKOREAN) ||\
     (protType == CC_BICCKOREAN) ||\
     (protType == CC_SIPTKOREAN)) ? TRUE : FALSE)
/* Feature 2170 */

#define CC_IS_PROT_TYPE_KPNV2(protType)\
    (((protType == CC_SIKPNV2) ||\
     (protType == CC_BICCKPNV2) ||\
     (protType == CC_SIPTKPNV2)) ? TRUE : FALSE)

#define CC_IS_PROT_TYPE_KPNNL2(protType)\
    (((protType == CC_SIKPNNL2) ||\
     (protType == CC_BICCKPNNL2) ||\
     (protType == CC_SIPTKPNNL2)) ? TRUE : FALSE)

#define CC_IS_PROT_TYPE_INDIA(protType)\
    (((protType == CC_SIINDIA) ||\
     (protType == CC_BICCINDIA) ||\
     (protType == CC_SIPTINDIA)) ? TRUE : FALSE)

#define CC_IS_PROT_TYPE_UK(protType)\
    (((protType == CC_SIUK) ||\
     (protType == CC_BICCUK) ||\
     (protType == CC_SIPTUK)) ? TRUE : FALSE)

#define CC_IS_PROT_TYPE_THAILAND(protType)\
    (((protType == CC_SITHAILAND) ||\
     (protType == CC_BICCTHAILAND)||\
     (protType == CC_SIPTTHAILAND)) ? TRUE : FALSE)

#define CC_IS_PROT_TYPE_NICARAGUA(protType)\
    (((protType == CC_SINICARAGUA) ||\
     (protType == CC_BICCNICARAGUA)||\
     (protType == CC_SIPTNICARAGUA)) ? TRUE : FALSE)

#define CC_IS_PROT_TYPE_HK(protType)\
   (((protType == CC_SIHK) ||\
     (protType == CC_BICCHK) ||\
     (protType == CC_SIPTHK)) ? TRUE : FALSE) 

#define CC_IS_PROT_TYPE_PE(protType)\
    (((protType == CC_SIPE) ||\
      (protType == CC_BICCPE) ||\
      (protType == CC_SIPTPE)) ? TRUE : FALSE)

#define CC_IS_PROT_TYPE_UA(protType)\
    (((protType == CC_SIUA) ||\
      (protType == CC_BICCUA) ||\
      (protType == CC_SIPTUA)) ? TRUE : FALSE)

#define CC_IS_PROT_TYPE_ITU88(protType)\
    (((protType == CC_SIITU88) ||\
      (protType == CC_BICCITU88) ||\
      (protType == CC_SIPTITU88)) ? TRUE : FALSE)

#define CC_IS_PROT_TYPE_VN(protType)\
   (((protType == CC_SIVN) ||\
     (protType == CC_BICCVN) ||\
     (protType == CC_SIPTVN)) ? TRUE : FALSE)

#define CC_IS_PROT_TYPE_ITU99(protType)\
    (((protType == CC_SIITU99) ||\
      (protType == CC_BICCITU99) ||\
      (protType == CC_SIPTITU99)) ? TRUE : FALSE)

/* FID 17143.0 */
#define CC_IS_PROT_TYPE_NTT(protType)\
    (((protType == CC_SINTT) ||\
      (protType == CC_BICCNTT) ||\
      (protType == CC_SIPTNTT)) ? TRUE : FALSE)

#define CC_IS_COLP_ON(intfcCb)\
    ((intfcCb && intfcCb->ccTgCb && intfcCb->ccTgCb->tgAtt.colpForSIP)? TRUE:FALSE)

/* FID 17026.0 */
#define CC_SIP3XX_HANDLING_PASSTHRU(intfcCb)\
    (((intfcCb && intfcCb->ccTgCb && intfcCb->ccTgCb->tgAtt.sipPrflPtr &&\
       intfcCb->ccTgCb->tgAtt.sipPrflPtr->sip3xxHandling == LCC_3XX_PASSTHRU) ||\
      (intfcCb && intfcCb->ccTgCb && intfcCb->ccTgCb->tgAtt.sipTPrflPtr &&\
       intfcCb->ccTgCb->tgAtt.sipTPrflPtr->sip3xxHandling == LCC_3XX_PASSTHRU))? TRUE:FALSE)
    
#define CC_IS_SIP3XX_PASSTHRU(_con) (_con->sip3xxContact.pres)

/* FID 14883.0 DDI */
#define CC_DDI_COMPAT_ON(intfcCb)                                          \
      (((intfcCb) && ((CC_OPT_ISUPCOMPAT(intfcCb) == COMPAT_ITU92)||       \
      (CC_OPT_ISUPCOMPAT(intfcCb) == COMPAT_TYPEA)||                       \
      (CC_OPT_ISUPCOMPAT(intfcCb) == COMPAT_Q767)))?TRUE:FALSE) 

#define CC_HOPIWTO767(con)                                                 \
    ((((con->direction == INCTOOUT) && (con->ogProtType == CC_SI76792))||  \
      ((con->direction == OUTTOINC) && (con->icProtType == CC_SI76792)))? TRUE:FALSE)

#define CC_HOPIWFROM767(con)                                               \
    ((((con->direction == INCTOOUT) && (con->icProtType == CC_SI76792))||\
      ((con->direction == OUTTOINC) && (con->ogProtType == CC_SI76792)))? TRUE:FALSE)

/* Feature 1839 - compatibility */
/* Check whether compatibility is supported */
/*Bug:42295---11--02--2005*/
/*Add support for DEV:2276-pengding*/
#if 0
#define CC_IS_COMPAT_SUPPORTED(protType, direction) \
    (((protType) == CC_SIGERMANY) && \
     ((direction) == INCTOOUT) ? TRUE : FALSE)
#else
/*leon: bug 56739 Add support fro G5002002*/
/* yugangz: bug 70278 Add support for Telstra */
#define CC_IS_COMPAT_SUPPORTED(IntfcCb, protType) \
    (((protType) == CC_SIGERMANY)||\
     ((protType) == CC_SICHINA)||\
     ((protType) == CC_SIBRAZIL)||\
     ((protType) == CC_SIINDIA)||\
     ((protType) == CC_SIETSI)||\
     ((protType) == CC_SIVENEZUELA)||\
     ((protType) == CC_SICOSTARICA)||\
     ((protType) == CC_SIARGENTINA)||\
     ((protType) == CC_SIKOREAN)||\
     ((protType) == CC_SIKPNV2)||\
     ((protType) == CC_SIKPNNL2)||\
     ((protType) == CC_SIVEMOVILNET)||\
     ((protType) == CC_SIPOLAND) || \
     ((protType) == CC_SIITU92) || \
     ((protType) == CC_SIETSIV3)||\
     ((protType) == CC_SIITU97)||\
     ((protType) == CC_SIG500V0)||\
     ((protType) == CC_SIG500V2)||\
     ((protType) == CC_SITELSTRA) || \
     ((protType) == CC_SIUK)||\
     ((protType) == CC_SIRUSSIA)||\
     ((protType) == CC_SITHAILAND)||\
     ((protType) == CC_SISPAIN)||\
     ((protType) == CC_SINICARAGUA)||\
     ((protType) == CC_SIHK)||\
     ((protType) == CC_SIPE)||\
     ((protType) == CC_SIUA)||\
     ((protType) == CC_SIVN)||\
     ((protType) == CC_SIITU99)||\
     ((protType) == CC_SI76792) ? TRUE :\
      (((CC_GET_PROTOCOL(protType)==CC_SI)||(CC_GET_PROTOCOL(protType)==CC_EXT_BICC)) &&\
       (CC_DDI_COMPAT_ON(IntfcCb))))
#endif
/*Bug:42295---11--02--2005*/
/*Bug :41665*/
#define CC_VALID_NOA(value) \
        ((((value) == NA_UNKNOWN) \
        || ((value) ==  NA_INTNATNUM) \
        || ((value) ==   NA_NATNUM)   \
        || ((value) ==  NA_NONE)      \
        || ((value) ==  NA_SUBSNUM)   \
        || ((value) ==  NA_SUBSNUMOPRREQ) \
        || ((value) ==  NA_NATNUMOPRREQ) \
        || ((value) ==  NA_INTNATNUMOPRREQ) \
        || ((value) ==  NA_NONUMOPRREQ) \
        || ((value) ==  NA_NONUMCUTTHRU) \
        || ((value) ==  NA_CAINVPN )) ? TRUE : FALSE)
/*Bug :41665*/
/*DEV:2276-pengding*/
#define CC_IS_PROT_TYPE_Q767(protType)\
   (((protType == CC_SI76792) ||\
     (protType == CC_BICC76792) ||\
     (protType == CC_SIPT76792)) ? TRUE : FALSE)

#define CC_VALID_NOA_Q767(value) \
        ((((value) == NA_NONE) \
        || ((value) ==  NA_INTNATNUM))? TRUE : FALSE)
/*DEV:2223--pengding*/
#define CC_IS_PROT_TYPE_ITU92(protType)\
   (((protType == CC_SIITU92) ||\
     (protType == CC_BICCITU92) ||\
     (protType == CC_SIPTITU92)) ? TRUE : FALSE)

#define CC_VALID_NOA_ITU92(value) \
        ((((value) == NA_UNKNOWN) \
        || ((value) ==  NA_INTNATNUM) \
        || ((value) ==   NA_NATNUM)   \
        || ((value) ==  NA_SUBSNUM))? TRUE : FALSE)
/*Bug87055 +*/
#define CC_IS_PROT_TYPE_ITU(con)\
   (CC_IS_PROT_TYPE_ITU88(con->icProtType) ||\
    CC_IS_PROT_TYPE_ITU92(con->icProtType) ||\
    CC_IS_PROT_TYPE_ITU97(con->icProtType) ||\
    CC_IS_PROT_TYPE_ITU99(con->icProtType) ||\
    CC_IS_PROT_TYPE_Q767(con->icProtType)  ||\
    CC_OPT_IWBASEVARIANT(con->icIntfcCb) == CC_BASE_ISUPITU88 ||\
    CC_OPT_IWBASEVARIANT(con->icIntfcCb) == CC_BASE_ISUPITU92 ||\
    CC_OPT_IWBASEVARIANT(con->icIntfcCb) == CC_BASE_ISUPITU99 ||\
    CC_OPT_IWBASEVARIANT(con->icIntfcCb) == CC_BASE_ISUPQ767)
/*Bug87055 -*/
/* 
 * check if the Generic Number parameter is not supported for a given variant
 * new variants that do not support the GenNmb parameter must be added here
 */
#define CC_IS_GENNMB_NOT_SUPPORTED(protType) \
        (CC_IS_PROT_TYPE_HK(protType)  || \
         CC_IS_PROT_TYPE_SPAIN(protType)  || \
         CC_IS_PROT_TYPE_G500V0(protType) || \
         CC_IS_PROT_TYPE_VEMOVILNET(protType) || \
         CC_IS_PROT_TYPE_ARGENTINA(protType))
            
/*pengding*/
/* Porting code from 5.2 */
#define CC_DEFAULT_MAX_FORWARDS_FACTOR   5

#define CC_MAX_HOP_COUNTER               31

#define EGRESS_SIZE_TKNSTR               48

#define CC_MAX_FORWARDS_FACTOR_SIP(ccSipPrflCb) \
          ((( (ccSipPrflCb) != NULL) && ( (ccSipPrflCb)->maxFwdHopCntrRatio)) ?\
          (ccSipPrflCb)->maxFwdHopCntrRatio : CC_DEFAULT_MAX_FORWARDS_FACTOR)
          
#define CC_MAX_FORWARDS_FACTOR_SIPT(ccSipTPrflCb) \
          ((( (ccSipTPrflCb) != NULL) && ( (ccSipTPrflCb)->maxFwdHopCntrRatio)) ?\
          (ccSipTPrflCb)->maxFwdHopCntrRatio : CC_DEFAULT_MAX_FORWARDS_FACTOR)
          
/* end of porting code from 5.2 */
#define CC_IS_PROT_TYPE_BRAZIL(protType)\
   (((protType == CC_SIBRAZIL) ||\
     (protType == CC_BICCBRAZIL) ||\
     (protType == CC_SIPTBRAZIL)) ? TRUE : FALSE)

#define CC_IS_PROT_TYPE_INDIA(protType)\
   (((protType == CC_SIINDIA) ||\
     (protType == CC_BICCINDIA) ||\
     (protType == CC_SIPTINDIA)) ? TRUE : FALSE)

#define CC_TMR_OFFSET 1

#define CC_MAX_DIGIT_LEN 31

#define CC_IS_BEARCAP_UNRDIG(itcCap) (itcCap == ITC_UNRDIG)

#define CC_ISUP_MAX_REDIRCNT 7

/* Bug 34323 - Counters to prevent too many error logs */
#define CC_INC_ERROR_COUNTER(errType,errString,stats)\
{\
    if (errType < CC_ERR_MAX_ERRTYPE)\
    {\
       ccErrorCounter[errType].counter++;\
       ccErrorCounter[errType].param = stats;\
    }\
    else\
    {\
      CCLOGERR("%s = %lx\n", errString, stats);\
    }\
}

/* For testing release map */
#define CC_PRINT_REL(relEvnt)           \
{                                        \
    CCDP(AIN_DBG_LEVEL_0,"Cause:{eh:{pres:%d},cdeStand:{pres:%d,val:%d},recommend:{pres:%d,val:%d},location:{pres:%d,val:%d},causeVal:{pres:%d,val:%d},dgnVal:{pres:%d,val:%s}}\n",(relEvnt)->causeDgn.eh.pres,(relEvnt)->causeDgn.cdeStand.pres,(relEvnt)->causeDgn.cdeStand.val,(relEvnt)->causeDgn.recommend.pres,(relEvnt)->causeDgn.recommend.val,(relEvnt)->causeDgn.location.pres,(relEvnt)->causeDgn.location.val,(relEvnt)->causeDgn.causeVal.pres,(relEvnt)->causeDgn.causeVal.val,(relEvnt)->causeDgn.dgnVal.pres,(relEvnt)->causeDgn.dgnVal.val);\
}

#define CC_PRINT_SIP_REL(relEvnt) \
{                                                                                  \
    if (((relEvnt)->sipReason.text.len) >= sizeof((relEvnt)->sipReason.text.val))  \
       (relEvnt)->sipReason.text.val[sizeof((relEvnt)->sipReason.text.val)-1] = 0; \
    else                                                                           \
       (relEvnt)->sipReason.text.val[(relEvnt)->sipReason.text.len] = 0;           \
    CCDP(AIN_DBG_LEVEL_0,"sipReason:{pres:%d,cause:%d,prtcl:%d,test:{pres:%d,len:%d,val:%s}}\n",(relEvnt)->sipReason.pres,(relEvnt)->sipReason.cause,(relEvnt)->sipReason.prtcl,(relEvnt)->sipReason.text.pres,(relEvnt)->sipReason.text.len,(relEvnt)->sipReason.text.val); \
    CCDP(AIN_DBG_LEVEL_0,"sipStatus:{pres:%d,val:%d}\n",(relEvnt)->sipStatus.pres,(relEvnt)->sipStatus.val); \
    CCDP(AIN_DBG_LEVEL_0,"sipWarnCode:{pres:%d,val:%d}\n",(relEvnt)->sipWarnCode.pres,(relEvnt)->sipWarnCode.val);\
}

#define CC_STORE_UBUF(con)\
{\
    ccDropUBuf(&con->tmpUbuf);\
    con->tmpUbuf = con->uBuf;\
    con->uBuf = NULLP;\
}

#define CC_RESTORE_UBUF(con)\
{\
    ccDropUBuf(&con->uBuf);\
    con->uBuf = con->tmpUbuf;\
    con->tmpUbuf = NULLP;\
}

#define GCC_INC_CONCNT(MGBLOCK)\
{                              \
   (MGBLOCK)->conCnt++;          \
   CCDP(AIN_DBG_LEVEL_0, "INC con count to %d. id=%ld, %lx\n", (MGBLOCK)->conCnt, \
   ((MGBLOCK)->mgCtx ? (MGBLOCK)->mgCtx->suCtxId : 0), (U32)(MGBLOCK));\
}

#define GCC_DEC_CONCNT(MGBLOCK)   \
{                                 \
   if ((MGBLOCK)->conCnt)           \
   {                              \
      (MGBLOCK)->conCnt--;          \
      if (((MGBLOCK)->mgCtx) && (MGBLOCK)->mgCtx->suCtxId && (MGBLOCK)->conCnt <= 0) \
      {                            \
         DP("GCC_DEC_CONCNT: **Sending a sub before trashing the memory:%lx\n", (MGBLOCK)->mgCtx->suCtxId); \
         ccSendSubAndCleanupMgCtx((MGBLOCK)); \
         if ((MGBLOCK)->con)\
           CC_TRACE_DUMP((MGBLOCK)->con);\
      }                            \
   }                              \
   else                           \
   {                              \
      if ((MGBLOCK)->con)\
      {\
         CCLOGERR("ERROR: decrementing conCnt beyond zero 0x%lx\n", (MGBLOCK)->con->icSuConnId); \
         CC_TRACE_DUMP((MGBLOCK)->con);\
      }\
      ABORT_DEBUG;                \
   }                              \
}

#define GCC_MNG_MGBLOCK_CNTR(DST,SRC,CONBLOCK_DST) \
{                                             \
   CcMgBlock_t *dst = DST, *src=SRC;          \
   if ((dst) != (src))                        \
   {                                          \
   if (((dst) != NULLP) && ((src) == NULLP))  \
   {\
      GCC_DEC_CONCNT((dst));                  \
   }\
   if (((dst) == NULLP) && ((src) != NULLP))  \
      GCC_INC_CONCNT((src));                  \
   if (((dst) != NULLP) && ((src) != NULLP))  \
   {                                          \
      GCC_DEC_CONCNT((dst));                  \
      GCC_INC_CONCNT((src));                  \
   }                                          \
   (DST)=(SRC);                               \
   }                                          \
}

#define FREE_MGBLOCK(MGBLOCK)                 \
{                                             \
  if ((MGBLOCK) && ((MGBLOCK)->conCnt <= 1))  \
  {                                           \
     if (((MGBLOCK)->mgCtx) && (MGBLOCK)->mgCtx->suCtxId)            \
     {                                        \
       DP("FREE_MGBLOCK: **Sending a sub before trashing the memory:%lx\n", (MGBLOCK)->mgCtx->suCtxId); \
       ccSendSubAndCleanupMgCtx((MGBLOCK)); \
     }                                        \
     SPutSBuf(ccInit.region, ccInit.pool, (MGBLOCK), (Size)(sizeof(CcMgBlock_t)));\
  }                                           \
  else                                        \
  {                                           \
     CCLOGERR("ERROR: attempted to release mgblock %ld when another con is using it\n", \
     ((MGBLOCK)->mgCtx ? (MGBLOCK)->mgCtx->suCtxId : 0)); \
     GCC_DEC_CONCNT(MGBLOCK);                 \
     if ((MGBLOCK)->con)                      \
        CC_TRACE_DUMP((MGBLOCK)->con);        \
     (MGBLOCK) = NULLP;                       \
     ABORT_DEBUG;                             \
  }                                           \
}

#ifdef CC_FID_1950_1773 

#define MAX_IW_ENTRIES 4

/* Common GCC protocols used in indirect mapping method */
#define GCC_COMMON_ISUP_PROTOCOL CC_SI76792 /* Common ISUP protocol used for indirect method*/
#define GCC_COMMON_SIPT_PROTOCOL CC_SIPT76792 /* Common SIPT protocol used for indirect method*/

/*  #defines for the mappingCategory field of CcIntwPath_t  structure */
#define MAP_INVALID       0x00
#define MAP_I             0x01
#define MAP_O             0x02
#define MAP_B             0x04
#define MAP_V             0x08
      
#define MAP_I_O          ((MAP_I  << 8)  |  MAP_O)
#define MAP_I_B          ((MAP_I  << 8)  |  MAP_B)
#define MAP_B_O          ((MAP_B  << 8)  |  MAP_O)
#define MAP_I_V          ((MAP_I  << 8)  |  MAP_V)
#define MAP_V_O          ((MAP_V  << 8)  |  MAP_O) 
#define MAP_V_V          ((MAP_V  << 8)  |  MAP_V)
#define MAP_B_V          ((MAP_B  << 8)  |  MAP_V)

/* Macros to determine the mapping categories */
#define IS_MAP_CAT_ANY_DIRECT(_mappingCategory)                               \
        ( ((_mappingCategory) == MAP_I_O) ||                                  \
          ((_mappingCategory) == MAP_I_B) ||                                  \
          ((_mappingCategory) == MAP_B_O) )

#define IS_MAP_CAT_ANY_I(_mappingCategory)                                    \
        ( (_mappingCategory) & (MAP_I << 8) )

#define IS_MAP_CAT_ANY_O(_mappingCategory)                                    \
        ( (_mappingCategory) & MAP_O )

#define IS_MAP_CAT_ANY_B(_mappingCategory)                                    \
        ( ((_mappingCategory) & (MAP_B << 8)) ||                              \
          ((_mappingCategory) & MAP_B) )

#define IS_MAP_CAT_ANY_I_OR_ANY_B(_mappingCategory)                           \
        ( ((_mappingCategory) & (MAP_I << 8)) ||                              \
          ((_mappingCategory) & (MAP_B << 8)) )

#define IS_MAP_CAT_ANY_O_OR_1ST_HOP_INDIR(_mappingCategory)                   \
        ( ((_mappingCategory) & MAP_O) ||                                     \
          ((_mappingCategory) == MAP_I_V) ||                                  \
          ((_mappingCategory) == MAP_B_V) )

#define IS_MAP_CAT_ANY_I_ANY_O_ANY_B(_mappingCategory)                        \
        ( IS_MAP_CAT_ANY_I(_mappingCategory) ||                               \
          IS_MAP_CAT_ANY_O(_mappingCategory) ||                               \
          IS_MAP_CAT_ANY_B(_mappingCategory) )

#define IS_MAP_CAT_ANY_O_OR_INGRESS_CCS(_mappingCategory)                     \
        ( ((_mappingCategory) & MAP_O) ||                                     \
          ((_mappingCategory) == MAP_I_B) )

#define CC_INSERT_MAPTBL(protocol1, protocol2)                                \
{                                                                             \
    CcMapEntry_t     *mapEntry     = NULL;                                    \
    CcIntwPath_t     *intwPath     = NULL;                                    \
    S16              ret           = RFAILED;                                 \
    U32              protKey       = 0;                                       \
                                                                              \
    protKey = protocol1;                                                      \
    protKey = ((protKey << 16) | (protocol2));                                \
                                                                              \
    ret = cmHashListFind(&ccMapTbl, (U8 *)&(protKey), (U16)sizeof(U32),       \
                        (U16)0, (PTR*) &mapEntry);                            \
    if (ret != ROK)                                                           \
    {/* No entry found */                                                     \
      mapEntry = NULL;                                                        \
      ret = SGetSBuf(ccInit.region, ccInit.pool, (Data **)&mapEntry,          \
                   (Size) sizeof(CcMapEntry_t));                              \
      if (ret != ROK)                                                         \
      {                                                                       \
        CCLOGERR("Error:  SGetSBuf failed for mapping Table entry.\n");       \
        ccGenAlarm(LCM_CATEGORY_RESOURCE, LCC_EVENT_MEMALOC_FAILED,           \
                   LCC_CAUSE_MALLOC_FAIL, (U8 *) &ret);                       \
        RETVOID;                                                              \
      }                                                                       \
      cmMemset((U8 *)mapEntry, 0, sizeof(CcMapEntry_t));                      \
      mapEntry->protKey = protKey;                                            \
                                                                              \
      cmQInit(&mapEntry->intwPath, 0, CCOFFSETOF(CcIntwPath_t, entry));       \
      ret = SGetSBuf(ccInit.region, ccInit.pool, (Data **) &intwPath,         \
                   (Size) sizeof(CcIntwPath_t));                              \
      if (ret != ROK)                                                         \
      {                                                                       \
        CCLOGERR("Error:  SGetSBuf failed for CcIntwPath_t queue entry\n");   \
        ccGenAlarm(LCM_CATEGORY_RESOURCE, LCC_EVENT_MEMALOC_FAILED,           \
                   LCC_CAUSE_MALLOC_FAIL, (U8 *) &ret);                       \
        SPutSBuf (ccInit.region, ccInit.pool, mapEntry,              \
                  (Size) sizeof (CcMapEntry_t));                              \
        RETVOID;                                                              \
      }                                                                       \
      cmMemset((U8 *) intwPath, 0, sizeof(CcIntwPath_t));                     \
      intwPath->intwProt1 = protocol1;                                        \
      intwPath->intwProt2 = protocol2;                                        \
      if (CC_GET_PROTOCOL(protocol2) == CC_BICC)                              \
        intwPath->mappingCategory = MAP_I_B;                                  \
      else if (CC_GET_PROTOCOL(protocol1) == CC_BICC)                         \
        intwPath->mappingCategory = MAP_B_O;                                  \
      else                                                                    \
        intwPath->mappingCategory = MAP_I_O;                                  \
                                                                              \
                                                                              \
      ret = cmHashListInsert(&ccMapTbl, (PTR)mapEntry, (U8 *)&mapEntry->protKey,\
                             (U16)sizeof(U32));                                 \
      if (ret != ROK)                                                           \
      {                                                                         \
        CCLOGERR("Error:ccMapTbl insert failed for prot1 = 0x%x,"               \
                 " prot2 = 0x%x, ret = %d\n", protocol1, protocol2, ret);       \
        SPutSBuf (ccInit.region, ccInit.pool, intwPath,                         \
                 (Size) sizeof (CcIntwPath_t));                                 \
        SPutSBuf (ccInit.region, ccInit.pool, mapEntry,                         \
                  (Size) sizeof (CcMapEntry_t));                                \
        RETVOID;                                                                \
      }                                                                         \
      cmQInsert(&mapEntry->intwPath, intwPath);                                 \
      DP("Inserted entry for icProt = 0x%x to ogProt = 0x%x into ccMapTbl\n",  \
         protocol1, protocol2);                                                \
    }                                                                          \
    else                                                                       \
    { /* Entry already present */                                              \
      DP("ccMapTbl already has an entry for icProt = 0x%x to ogProt = 0x%x\n", \
         protocol1, protocol2);                                                \
    }                                                                          \
}

#define CC_INSERT_NATISUP(protocol1, IsupProtocol2)                           \
{                                                                             \
    CcNativeIsupBlock_t     *natIsup      = NULL;                             \
    S16                      ret          = RFAILED;                          \
    ProtType                 icProtType   = protocol1;                        \
                                                                              \
    ret = cmHashListFind(&ccNatIsupTbl, (U8 *)&(icProtType),                  \
                        (U16)sizeof(ProtType), (U16)0, (PTR*) &natIsup);      \
    if (ret != ROK)                                                           \
    {/* No entry found */                                                     \
      natIsup = NULL;                                                         \
      ret = SGetSBuf(ccInit.region, ccInit.pool, (Data **)&natIsup,           \
                     (Size) sizeof(CcNativeIsupBlock_t));                     \
      if (ret != ROK)                                                         \
      {                                                                       \
        CCLOGERR("Error:  SGetSBuf failed for Native ISUP Table entry.\n");   \
        ccGenAlarm(LCM_CATEGORY_RESOURCE, LCC_EVENT_MEMALOC_FAILED,           \
                   LCC_CAUSE_MALLOC_FAIL, (U8 *) &ret);                       \
        RETVOID;                                                              \
      }                                                                       \
      cmMemset((U8 *)natIsup, 0, sizeof(CcNativeIsupBlock_t));                \
      natIsup->protocol     = protocol1;                                      \
      natIsup->isupProtocol = IsupProtocol2;                                  \
      ret = cmHashListInsert(&ccNatIsupTbl, (PTR)natIsup,                     \
                             (U8 *)&natIsup->protocol, (U16)sizeof(U16));     \
      if (ret != ROK)                                                         \
      {                                                                       \
        CCLOGERR("Error:ccNatIsupTbl insert failed for prot1 = 0x%x,"         \
                   " prot2 = 0x%x, ret = %d\n", protocol1, IsupProtocol2, ret);\
        SPutSBuf (ccInit.region, ccInit.pool, natIsup,                         \
                  (Size) sizeof (CcNativeIsupBlock_t));                        \
        RETVOID;                                                               \
      }                                                                        \
    }                                                                          \
    else                                                                       \
    {/* Entry already present */                                               \
      DP("ccNatIsupTbl already has an entry for protocol1=0x%x\n", protocol1); \
    }                                                                          \
}

#define CC_INSERT_IW_ENTRY_INTO_LIST(intwQList, protocol1, protocol2, mapCategory)\
{                                                                                 \
  intwQList->mapIwQ[intwQList->count].intwProt1 = protocol1;                      \
  intwQList->mapIwQ[intwQList->count].intwProt2 = protocol2;                      \
  intwQList->mapIwQ[intwQList->count].mappingCategory = mapCategory;              \
  intwQList->count++;                                                             \
} 

#define CC_DOES_BASE_MATCH(prot, base) ((prot & BASEMASK) == base)

#define CC_INTW_LAST_HOP(con) ((con) && (con->intwBlock.lastHop))

#define INTW_CAS_GR303_LN        0
#define INTW_CAS_TG              1
#define INTW_MGCP_LN             2
#define INTW_SIPT_SI_BICC        3
#define INTW_SIP                 4
#define INTW_ISDN                5
#define INTW_LCLSWT              6
#define INTW_IVT                 7
#define INTW_ICA                 8

#define MAX_BASE_INTW_PROT       9

#define CAS_GR303_LN_TO_CAS_GR303_LN_MAX 4
#define CAS_GR303_LN_TO_CAS_TG_MAX       4
#define CAS_GR303_LN_TO_MGCP_LN_MAX      6
#define CAS_GR303_LN_TO_SIPT_SI_BICC_MAX 6
#define CAS_GR303_LN_TO_SIP_MAX          5
#define CAS_GR303_LN_TO_ISDN_MAX         4
#define CAS_GR303_LN_TO_LCLSWT_MAX       1
#define CAS_GR303_LN_TO_IVT_MAX          1

#define CAS_TG_TO_CAS_GR303_LN_MAX       4
#define CAS_TG_TO_CAS_TG_MAX             6
#define CAS_TG_TO_MGCP_LN_MAX            6
#define CAS_TG_TO_SIPT_SI_BICC_MAX       8
#define CAS_TG_TO_SIP_MAX                4
#define CAS_TG_TO_ISDN_MAX               4
#define CAS_TG_TO_LCLSWT_MAX             1
#define CAS_TG_TO_IVT_MAX                1

#define MGCP_LN_TO_CAS_GR303_LN_MAX      4
#define MGCP_LN_TO_CAS_TG_MAX            4
#define MGCP_LN_TO_MGCP_LN_MAX           5
#define MGCP_LN_TO_SIPT_SI_BICC_MAX      6
#define MGCP_LN_TO_SIP_MAX               7
#define MGCP_LN_TO_ISDN_MAX              4
#define MGCP_LN_TO_LCLSWT_MAX            1
#define MGCP_LN_TO_IVT_MAX               1

#define SIPT_SI_BICC_TO_CAS_GR303_LN_MAX (CC_MAX_SI2CS_MSG)
#define SIPT_SI_BICC_TO_CAS_TG_MAX       (CC_MAX_SI2CS_MSG)
#define SIPT_SI_BICC_TO_MGCP_LN_MAX      9
#define SIPT_SI_BICC_TO_SIPT_SI_BICC_MAX (CC_MAX_NBTONBMSG)
#define SIPT_SI_BICC_TO_SIP_MAX          (CC_MAX_SI2SIP_MSG)
#define SIPT_SI_BICC_TO_ISDN_MAX         (CC_MAX_SI2IN_MSG)
#define SIPT_SI_BICC_TO_LCLSWT_MAX       1
#define SIPT_SI_BICC_TO_IVT_MAX          (CC_MAX_SI2IVT_MSG)

#define SIP_TO_CAS_GR303_LN_MAX          (CC_MAX_SIP2CSLN_MSG)
#define SIP_TO_CAS_TG_MAX                (CC_MAX_SIP2CSTG_MSG) 
#define SIP_TO_MGCP_LN_MAX               7
#define SIP_TO_SIPT_SI_BICC_MAX          (CC_MAX_SIP2SI_MSG)
#define SIP_TO_SIP_MAX                   (CC_MAX_SIP2SIP_MSG)
#define SIP_TO_ISDN_MAX                  (CC_MAX_SIP2IN_MSG) 
#define SIP_TO_LCLSWT_MAX                1
#define SIP_TO_IVT_MAX                   (CC_MAX_SIP2IVT_MSG)

#define ISDN_TO_CAS_GR303_LN_MAX         4
#define ISDN_TO_CAS_TG_MAX               4
#define ISDN_TO_MGCP_LN_MAX              4
#define ISDN_TO_SIPT_SI_BICC_MAX         15
#define ISDN_TO_SIP_MAX                  CC_MAX_IN2SIP_MSG 
#define ISDN_TO_ISDN_MAX                 16
#define ISDN_TO_LCLSWT_MAX               1
#define ISDN_TO_IVT_MAX                  1

#define LCLSWT_TO_CAS_GR303_LN_MAX       1
#define LCLSWT_TO_CAS_TG_MAX             1
#define LCLSWT_TO_MGCP_LN_MAX            1
#define LCLSWT_TO_SIPT_SI_BICC_MAX       2
#define LCLSWT_TO_SIP_MAX                1
#define LCLSWT_TO_ISDN_MAX               1
#define LCLSWT_TO_LCLSWT_MAX             1
#define LCLSWT_TO_IVT_MAX                1

#define IVT_TO_CAS_GR303_LN_MAX          1
#define IVT_TO_CAS_TG_MAX                1
#define IVT_TO_MGCP_LN_MAX               1
#define IVT_TO_SIPT_SI_BICC_MAX          CC_MAX_IVT2SI_MSG
#define IVT_TO_SIP_MAX                   (CC_MAX_IVT2SIP_MSG)
#define IVT_TO_ISDN_MAX                  1
#define IVT_TO_LCLSWT_MAX                1
#define IVT_TO_IVT_MAX                   1

#define ICA_TO_SIPT_SI_BICC_MAX          (CC_MAX_ICA2SI_MSG)
#define ICA_TO_SIP_MAX                   (CC_MAX_ICA2SIP_MSG)
#define ICA_TO_ISDN_MAX                  (CC_MAX_ICA2ISDN_MSG)
#define ICA_TO_XX_MAX                    1
#define XX_TO_ICA_MAX                    1
#define SIP_TO_ICA_MAX                   (CC_MAX_SIP2ICA_MSG)

/*pengding--BUG:50755--send protocol type to ISUP for ISUP<-->ISDN*/
#define CC_COPY_TYPE_OF_XCHANGE(CON)\
{\
  if ((CON))\
  {\
    switch (CC_GET_PROTOCOL((CON)->icProtType))\
    {\
      case CC_SI :\
        switch (CC_GET_PROTOCOL((CON)->ogProtType))\
        {\
          case CC_SI :\
          case CC_EXT_BICC:\
            COPYTKN((CON)->ogEvnt->m.ccConEvnt.m.siConEvnt.optElmnt.isup.typeOfXchg,\
                    (CON)->icEvnt->m.ccConEvnt.m.siConEvnt.optElmnt.isup.typeOfXchg);\
            COPYTKN((CON)->ogEvnt->m.ccConEvnt.m.siConEvnt.optElmnt.isup.passOnFlag,\
                    (CON)->icEvnt->m.ccConEvnt.m.siConEvnt.optElmnt.isup.passOnFlag);\
            (CON)->ogEvnt->m.ccConEvnt.m.siConEvnt.optElmnt.isup.typeOfProtocol = \
                    (CON)->icEvnt->m.ccConEvnt.m.siConEvnt.optElmnt.isup.typeOfProtocol;\
            break;\
          case CC_BICC :\
            COPYTKN((CON)->ogEvnt->m.ccConEvnt.m.siConEvnt.optElmnt.bicc.typeOfXchg,\
                    (CON)->icEvnt->m.ccConEvnt.m.siConEvnt.optElmnt.isup.typeOfXchg);\
            COPYTKN((CON)->ogEvnt->m.ccConEvnt.m.siConEvnt.optElmnt.bicc.passOnFlag,\
                    (CON)->icEvnt->m.ccConEvnt.m.siConEvnt.optElmnt.isup.passOnFlag);\
            break;\
          case CC_SIPT :\
            COPYTKN((CON)->ogEvnt->m.ccConEvnt.m.siConEvnt.optElmnt.sipt.typeOfXchg,\
                    (CON)->icEvnt->m.ccConEvnt.m.siConEvnt.optElmnt.isup.typeOfXchg);\
            COPYTKN((CON)->ogEvnt->m.ccConEvnt.m.siConEvnt.optElmnt.sipt.passOnFlag,\
                    (CON)->icEvnt->m.ccConEvnt.m.siConEvnt.optElmnt.isup.passOnFlag);\
            break;\
        }\
        break;\
      case CC_BICC :\
        switch (CC_GET_PROTOCOL((CON)->ogProtType))\
        {\
          case CC_SI :\
            COPYTKN((CON)->ogEvnt->m.ccConEvnt.m.siConEvnt.optElmnt.isup.typeOfXchg,\
                    (CON)->icEvnt->m.ccConEvnt.m.siConEvnt.optElmnt.bicc.typeOfXchg);\
            COPYTKN((CON)->ogEvnt->m.ccConEvnt.m.siConEvnt.optElmnt.isup.passOnFlag,\
                    (CON)->icEvnt->m.ccConEvnt.m.siConEvnt.optElmnt.bicc.passOnFlag);\
            break;\
          case CC_BICC :\
            COPYTKN((CON)->ogEvnt->m.ccConEvnt.m.siConEvnt.optElmnt.bicc.typeOfXchg,\
                    (CON)->icEvnt->m.ccConEvnt.m.siConEvnt.optElmnt.bicc.typeOfXchg);\
            COPYTKN((CON)->ogEvnt->m.ccConEvnt.m.siConEvnt.optElmnt.bicc.passOnFlag,\
                    (CON)->icEvnt->m.ccConEvnt.m.siConEvnt.optElmnt.bicc.passOnFlag);\
            break;\
          case CC_SIPT :\
            COPYTKN((CON)->ogEvnt->m.ccConEvnt.m.siConEvnt.optElmnt.sipt.typeOfXchg,\
                    (CON)->icEvnt->m.ccConEvnt.m.siConEvnt.optElmnt.bicc.typeOfXchg);\
            COPYTKN((CON)->ogEvnt->m.ccConEvnt.m.siConEvnt.optElmnt.sipt.passOnFlag,\
                    (CON)->icEvnt->m.ccConEvnt.m.siConEvnt.optElmnt.bicc.passOnFlag);\
            break;\
        }\
        break;\
      case CC_SIPT :\
        switch (CC_GET_PROTOCOL((CON)->ogProtType))\
        {\
          case CC_SI :\
            COPYTKN((CON)->ogEvnt->m.ccConEvnt.m.siConEvnt.optElmnt.isup.typeOfXchg,\
                    (CON)->icEvnt->m.ccConEvnt.m.siConEvnt.optElmnt.sipt.typeOfXchg);\
            COPYTKN((CON)->ogEvnt->m.ccConEvnt.m.siConEvnt.optElmnt.isup.passOnFlag,\
                    (CON)->icEvnt->m.ccConEvnt.m.siConEvnt.optElmnt.sipt.passOnFlag);\
            break;\
          case CC_BICC :\
            COPYTKN((CON)->ogEvnt->m.ccConEvnt.m.siConEvnt.optElmnt.bicc.typeOfXchg,\
                    (CON)->icEvnt->m.ccConEvnt.m.siConEvnt.optElmnt.sipt.typeOfXchg);\
            COPYTKN((CON)->ogEvnt->m.ccConEvnt.m.siConEvnt.optElmnt.bicc.passOnFlag,\
                    (CON)->icEvnt->m.ccConEvnt.m.siConEvnt.optElmnt.sipt.passOnFlag);\
            break;\
          case CC_SIPT :\
            COPYTKN((CON)->ogEvnt->m.ccConEvnt.m.siConEvnt.optElmnt.sipt.typeOfXchg,\
                    (CON)->icEvnt->m.ccConEvnt.m.siConEvnt.optElmnt.sipt.typeOfXchg);\
            COPYTKN((CON)->ogEvnt->m.ccConEvnt.m.siConEvnt.optElmnt.sipt.passOnFlag,\
                    (CON)->icEvnt->m.ccConEvnt.m.siConEvnt.optElmnt.sipt.passOnFlag);\
            break;\
        }\
        break;\
    }\
  }\
}

#define CC_COPY_TYPE_OF_XCHANGE_FOR_ACM(CON)\
{\
  if ((CON))\
  {\
    switch (CC_GET_PROTOCOL((CON)->icProtType))\
    {\
      case CC_SI :\
        switch (CC_GET_PROTOCOL((CON)->ogProtType))\
        {\
          case CC_SI :\
            COPYTKN((CON)->ogEvnt->m.ccCnStEvnt.m.siCnStEvnt.optElmnt.isup.typeOfXchg,\
                    (CON)->icEvnt->m.ccCnStEvnt.m.siCnStEvnt.optElmnt.isup.typeOfXchg);\
            COPYTKN((CON)->ogEvnt->m.ccCnStEvnt.m.siCnStEvnt.optElmnt.isup.passOnFlag,\
                    (CON)->icEvnt->m.ccCnStEvnt.m.siCnStEvnt.optElmnt.isup.passOnFlag);\
            (CON)->ogEvnt->m.ccCnStEvnt.m.siCnStEvnt.optElmnt.isup.typeOfProtocol = \
                    (CON)->icEvnt->m.ccCnStEvnt.m.siCnStEvnt.optElmnt.isup.typeOfProtocol;\
            break;\
          case CC_BICC :\
            COPYTKN((CON)->ogEvnt->m.ccCnStEvnt.m.siCnStEvnt.optElmnt.bicc.typeOfXchg,\
                    (CON)->icEvnt->m.ccCnStEvnt.m.siCnStEvnt.optElmnt.isup.typeOfXchg);\
            COPYTKN((CON)->ogEvnt->m.ccCnStEvnt.m.siCnStEvnt.optElmnt.bicc.passOnFlag,\
                    (CON)->icEvnt->m.ccCnStEvnt.m.siCnStEvnt.optElmnt.isup.passOnFlag);\
            break;\
          case CC_SIPT :\
            COPYTKN((CON)->ogEvnt->m.ccCnStEvnt.m.siCnStEvnt.optElmnt.sipt.typeOfXchg,\
                    (CON)->icEvnt->m.ccCnStEvnt.m.siCnStEvnt.optElmnt.isup.typeOfXchg);\
            COPYTKN((CON)->ogEvnt->m.ccCnStEvnt.m.siCnStEvnt.optElmnt.sipt.passOnFlag,\
                    (CON)->icEvnt->m.ccCnStEvnt.m.siCnStEvnt.optElmnt.isup.passOnFlag);\
            break;\
        }\
        break;\
      case CC_BICC :\
        switch (CC_GET_PROTOCOL((CON)->ogProtType))\
        {\
          case CC_SI :\
            COPYTKN((CON)->ogEvnt->m.ccCnStEvnt.m.siCnStEvnt.optElmnt.isup.typeOfXchg,\
                    (CON)->icEvnt->m.ccCnStEvnt.m.siCnStEvnt.optElmnt.bicc.typeOfXchg);\
            COPYTKN((CON)->ogEvnt->m.ccCnStEvnt.m.siCnStEvnt.optElmnt.isup.passOnFlag,\
                    (CON)->icEvnt->m.ccCnStEvnt.m.siCnStEvnt.optElmnt.bicc.passOnFlag);\
            break;\
          case CC_BICC :\
            COPYTKN((CON)->ogEvnt->m.ccCnStEvnt.m.siCnStEvnt.optElmnt.bicc.typeOfXchg,\
                    (CON)->icEvnt->m.ccCnStEvnt.m.siCnStEvnt.optElmnt.bicc.typeOfXchg);\
            COPYTKN((CON)->ogEvnt->m.ccCnStEvnt.m.siCnStEvnt.optElmnt.bicc.passOnFlag,\
                    (CON)->icEvnt->m.ccCnStEvnt.m.siCnStEvnt.optElmnt.bicc.passOnFlag);\
            break;\
          case CC_SIPT :\
            COPYTKN((CON)->ogEvnt->m.ccCnStEvnt.m.siCnStEvnt.optElmnt.sipt.typeOfXchg,\
                    (CON)->icEvnt->m.ccCnStEvnt.m.siCnStEvnt.optElmnt.bicc.typeOfXchg);\
            COPYTKN((CON)->ogEvnt->m.ccCnStEvnt.m.siCnStEvnt.optElmnt.sipt.passOnFlag,\
                    (CON)->icEvnt->m.ccCnStEvnt.m.siCnStEvnt.optElmnt.bicc.passOnFlag);\
            break;\
        }\
        break;\
      case CC_SIPT :\
        switch (CC_GET_PROTOCOL((CON)->ogProtType))\
        {\
          case CC_SI :\
            COPYTKN((CON)->ogEvnt->m.ccCnStEvnt.m.siCnStEvnt.optElmnt.isup.typeOfXchg,\
                    (CON)->icEvnt->m.ccCnStEvnt.m.siCnStEvnt.optElmnt.sipt.typeOfXchg);\
            COPYTKN((CON)->ogEvnt->m.ccCnStEvnt.m.siCnStEvnt.optElmnt.isup.passOnFlag,\
                    (CON)->icEvnt->m.ccCnStEvnt.m.siCnStEvnt.optElmnt.sipt.passOnFlag);\
            break;\
          case CC_BICC :\
            COPYTKN((CON)->ogEvnt->m.ccCnStEvnt.m.siCnStEvnt.optElmnt.bicc.typeOfXchg,\
                    (CON)->icEvnt->m.ccCnStEvnt.m.siCnStEvnt.optElmnt.sipt.typeOfXchg);\
            COPYTKN((CON)->ogEvnt->m.ccCnStEvnt.m.siCnStEvnt.optElmnt.bicc.passOnFlag,\
                    (CON)->icEvnt->m.ccCnStEvnt.m.siCnStEvnt.optElmnt.sipt.passOnFlag);\
            break;\
          case CC_SIPT :\
            COPYTKN((CON)->ogEvnt->m.ccCnStEvnt.m.siCnStEvnt.optElmnt.sipt.typeOfXchg,\
                    (CON)->icEvnt->m.ccCnStEvnt.m.siCnStEvnt.optElmnt.sipt.typeOfXchg);\
            COPYTKN((CON)->ogEvnt->m.ccCnStEvnt.m.siCnStEvnt.optElmnt.sipt.passOnFlag,\
                    (CON)->icEvnt->m.ccCnStEvnt.m.siCnStEvnt.optElmnt.sipt.passOnFlag);\
            break;\
        }\
        break;\
    }\
  }\
}
#endif /* End of CC_FID_1950_1773 */

/* BUG 42713 */
extern S32 ccLastExcessMgCtxts;
#define GCC_CHECK_EXCESS_MG_CTXTS \
{\
  S32 conCount = ccSuInstTbl.nmbEnt;\
  S32 ctxtCount = ccMgctInsTbl.nmbEnt;\
  /* FID 15261.0 - one Con can now have up to 3 contexts */\
  conCount *= 3;\
  if ((conCount < ctxtCount) && !((ctxtCount-conCount) % 50) &&\
      ((ctxtCount-conCount) != ccLastExcessMgCtxts))\
  {\
    DP("**Detected EXCESSIVE MG CONTEXTS (%ld) in GCC. Total calls are %ld\n",\
        ctxtCount, conCount);\
    ccLastExcessMgCtxts = ctxtCount - conCount;\
    if ((ctxtCount - conCount) >= 400)\
      ABORT_IF_PROT;\
  }\
}
#if 1 /* FID 14457.2 - Set a common DQ threshold across platforms */
#define CC_DFLT_LVL4_DQCONG_ENTER (3500)
#define CC_DFLT_LVL4_DQCONG_EXIT  (3300)
//#define CC_DFLT_LVL3_DQCONG_ENTER (2500)
//#define CC_DFLT_LVL3_DQCONG_EXIT  (2300)
#define CC_DFLT_LVL3_DQCONG_ENTER (1800)
#define CC_DFLT_LVL3_DQCONG_EXIT  (1600)
//#define CC_DFLT_LVL2_DQCONG_ENTER (1600)
//#define CC_DFLT_LVL2_DQCONG_EXIT  (1400)
#define CC_DFLT_LVL2_DQCONG_ENTER (1100)
#define CC_DFLT_LVL2_DQCONG_EXIT  (900)
#define CC_DFLT_LVL1_DQCONG_ENTER (800)
#define CC_DFLT_LVL1_DQCONG_EXIT  (600)
#else /* FID 14457.2 */
/* bug 77426 */
#ifdef ATCA
#define CC_DFLT_LVL4_DQCONG_ENTER (1900)
#define CC_DFLT_LVL4_DQCONG_EXIT  (1800)
#define CC_DFLT_LVL3_DQCONG_ENTER (1500) /* bug88508: changed from 2000->1500 */
#define CC_DFLT_LVL3_DQCONG_EXIT  (1300) /* bug88508: changed from 1900->1300 */
#define CC_DFLT_LVL2_DQCONG_ENTER (1200)
#define CC_DFLT_LVL2_DQCONG_EXIT  (1100)
#define CC_DFLT_LVL1_DQCONG_ENTER (1000)
#define CC_DFLT_LVL1_DQCONG_EXIT  (900)
#if 0
#define CC_DFLT_LVL2_DQCONG_ENTER (2000)
#define CC_DFLT_LVL2_DQCONG_EXIT  (1900)
#define CC_DFLT_LVL1_DQCONG_ENTER (1200)
#define CC_DFLT_LVL1_DQCONG_EXIT  (1100)
#endif
#else
#define CC_DFLT_LVL4_DQCONG_ENTER (3200)
#define CC_DFLT_LVL4_DQCONG_EXIT  (3000)
#define CC_DFLT_LVL3_DQCONG_ENTER (2200)
#define CC_DFLT_LVL3_DQCONG_EXIT  (2000)
#define CC_DFLT_LVL2_DQCONG_ENTER (1300)
#define CC_DFLT_LVL2_DQCONG_EXIT  (1100)
#define CC_DFLT_LVL1_DQCONG_ENTER (500)
#define CC_DFLT_LVL1_DQCONG_EXIT  (300)
#if 0
/* BUG 42713 */
#define CC_DFLT_LVL2_DQCONG_ENTER (1500)
#define CC_DFLT_LVL2_DQCONG_EXIT  (1400)
#define CC_DFLT_LVL1_DQCONG_ENTER (1000)
#define CC_DFLT_LVL1_DQCONG_EXIT  (900)
#endif
#endif
#endif /* FID 14457.2 */

#define CC_DFLT_NORMAL_REL_PCT    0
#define CC_DFLT_LVL1_REL_PCT      20
#define CC_DFLT_LVL2_REL_PCT      80
#define CC_DFLT_LVL3_REL_PCT      100
#define CC_DFLT_LVL4_REL_PCT      100

#if 1 /* Do not remove these counters! */
extern PUBLIC  U8  logFirstConInd;
extern PUBLIC  U8  logFirstConReq;
extern PUBLIC  U8  logFirstConCfm;
extern PUBLIC  U8  logFirstConRsp;
extern PUBLIC  U16 logFirstRelInd;
extern PUBLIC  U16 logFirstRelReq;
extern PUBLIC  U8 logRelCause;
extern PUBLIC  U8 logRelType;
extern PUBLIC  int CcExtRelCause[];
extern PUBLIC  int CcIntRelCause[];

extern PUBLIC  U16 logFailCndId; /* bug 69143 */
extern PUBLIC  U16 logFailcndNum;

/* FID 15319.0 External BICC CIC is U32 */
#define  CC_PRINT_CALL_INFO(CON)\
{\
  if ((CON))\
  {\
    switch (CC_GET_PROTOCOL((CON)->icProtType))\
    {\
      case CC_SI :\
        DP("** icRscVal=%d, icTg=%d, icProtType=%d, CIC=%d **\n",\
        (CON)->icRscVal,  (CON)->icRsc.intfc.t.tgrpDesc.tgrpId,\
        (CON)->icProtType, (CON)->icRsc.t.nbRsc.cic);\
        break;\
      case CC_BICC :\
        DP("** icRscVal=%d, icCCS=%d, icProtType=%d, biccCic=%lX **\n",\
        (CON)->icRscVal,  (CON)->icRsc.intfc.t.biccDesc.dstCcsId,\
        (CON)->icProtType, (CON)->icRsc.t.biccRsc.cirId);\
        break;\
      case CC_SIP :\
      case CC_SIPT :\
        DP("** icRscVal=%d, icTg=%d, icProtType=%d, CallId=%s **\n",\
        (CON)->icRscVal,  (CON)->icRsc.intfc.t.sipDesc.tgrpId,\
        (CON)->icProtType, (CON)->callDtlInfo.un.icSipId.val);\
        break;\
      case CC_EXT_BICC:\
        DP("** icRscVal=%d, icTg=%d, icProtType=%d, CIC=%lX **\n",\
           (CON)->icRscVal, (CON)->icRsc.intfc.t.tgrpDesc.tgrpId,\
           (CON)->icProtType, (CON)->icRsc.t.biccRsc.extCic);\
        break;\
      default:\
        DP("** icRscVal=%d, icProtType=%d **\n",\
        (CON)->icRscVal, (CON)->icProtType);\
        break;\
    }\
    switch (CC_GET_PROTOCOL((CON)->ogProtType))\
    {\
      case CC_SI :\
        DP("** ogRscVal=%d, ogTg=%d, ogProtType=%d, CIC=%d **\n",\
        (CON)->ogRscVal,  (CON)->ogRsc.intfc.t.tgrpDesc.tgrpId,\
        (CON)->ogProtType, (CON)->ogRsc.t.nbRsc.cic);\
        break;\
      case CC_BICC :\
        DP("** ogRscVal=%d, ogCCS=%d, ogProtType=%d, biccCic=%lX **\n",\
        (CON)->ogRscVal,  (CON)->ogRsc.intfc.t.biccDesc.dstCcsId,\
        (CON)->ogProtType, (CON)->ogRsc.t.biccRsc.cirId);\
        break;\
      case CC_SIP :\
      case CC_SIPT :\
        DP("** ogRscVal=%d, ogTg=%d, ogProtType=%d, CallId=%s **\n",\
        (CON)->ogRscVal,  (CON)->ogRsc.intfc.t.sipDesc.tgrpId,\
        (CON)->ogProtType, (CON)->callDtlInfo.un.ogSipId.val);\
        break;\
      case CC_EXT_BICC:\
        DP("** ogRscVal=%d, ogTg=%d, ogProtType=%d, CIC=%lX **\n",\
           (CON)->ogRscVal,  (CON)->ogRsc.intfc.t.tgrpDesc.tgrpId,\
           (CON)->ogProtType, (CON)->ogRsc.t.biccRsc.extCic);\
        break;\
      default:\
        DP("** ogRscVal=%d, ogProtType=%d **\n",\
        (CON)->ogRscVal, (CON)->ogProtType);\
        break;\
    }\
    CC_TRACE_DUMP((CON));\
  }\
}

#define CC_TRACE_EVENTS(CON, LOG, EVENT)\
{\
  if (LOG)\
  {\
    (LOG)--;\
    switch(EVENT)\
    {\
      case CCE_CONIND :\
        DP("** RCVD CONNECTION INDICATION FROM LAYER-3 **\n");\
        break;\
      case CCE_CONCFM :\
        DP("** RCVD CONNECTION CONFIRM FROM LAYER-3 **\n");\
        break;\
      case CCE_RELIND :\
        DP("** RCVD RELEASE INDICATION FROM LAYER-3 **\n");\
        break;\
      case CCE_CONREQ :\
        DP("** SENDING CONNECT REQUEST TO LAYER-3 **\n");\
        break;\
      case CCE_CONRSP :\
        DP("** SENDING CONNECT RESPONSE TO LAYER-3 **\n");\
        break;\
      case CCE_RELREQ :\
        DP("** SENDING RELEASE REQUEST TO LAYER-3 **\n");\
        break;\
    }\
    CC_PRINT_CALL_INFO((CON));\
  }\
}
#define CC_UPDATE_PIC(bcm, pc)                                                 \
{                                                                              \
    if (bcm)                                                                   \
       (bcm)->pic = pc;                                                        \
}

#define CC_TRACE_EVENTS_ID(ID, LOG, EVENT)\
{\
  CcConCb *con37962 = ccFindConFromSuConnId(suConnId);\
  CC_TRACE_EVENTS(con37962, LOG, EVENT);\
}

#define CC_INC_REL_CAUSE_CNTR(CAUSE, INT_EXT)\
{\
  if ((INT_EXT) & 0xF0) CcExtRelCause[(CAUSE)]++;\
  else CcIntRelCause[(CAUSE)]++;\
}

#define CC_DUMP_REL_CAUSE_CNTR()\
{\
  int i;\
  for (i=0; i<256; i++)\
    if (CcExtRelCause[i])\
      DP("Ext Rel Cause %d - %d times\n", i, CcExtRelCause[i]);\
  DP("End of Ext Rel Cause Dump\n");\
  for (i=0; i<256; i++)\
    if (CcIntRelCause[i])\
      DP("Int Rel Cause %d - %d times\n", i, CcIntRelCause[i]);\
  DP("End of Int Rel Cause Dump\n");\
}

#define CC_RESET_REL_CAUSE_CNTR()\
{\
  int i;\
  for (i=0; i<256; i++)\
  {\
    CcExtRelCause[i]=CcIntRelCause[i]=0;\
  }\
}

#define IS_THIS_ANSI_CALL_EMERGENCY(con) \
(CC_INGRESS_INTER_CCS_CALL(con) && \
(CC_IS_PROT_TYPE_ANSI_ISUP(con->icProtType) || CC_IS_PROT_TYPE_ANSI_BICC(con->icIntfcCb)) &&\
(con->ccEvntHold) && (con->ccEvntHold->m.ccConEvnt.m.siConEvnt.cgPtyCat.eh.pres == PRSNT_NODEF) &&\
(con->ccEvntHold->m.ccConEvnt.m.siConEvnt.cgPtyCat.cgPtyCat.pres == PRSNT_NODEF) &&\
((con->ccEvntHold->m.ccConEvnt.m.siConEvnt.cgPtyCat.cgPtyCat.val == CAT_EMERGCY) ||\
(con->ccEvntHold->m.ccConEvnt.m.siConEvnt.cgPtyCat.cgPtyCat.val == CAT_HIPRIEMRCY)))

#define IS_THIS_ANSI_CALL_PRIORITY_CALL(con) \
((CC_IS_PROT_TYPE_ANSI_ISUP(con->icProtType) || CC_IS_PROT_TYPE_ANSI_BICC(con->icIntfcCb)) &&\
(con->ccEvntHold) && (con->ccEvntHold->m.ccConEvnt.m.siConEvnt.cgPtyCat.eh.pres == PRSNT_NODEF) &&\
(con->ccEvntHold->m.ccConEvnt.m.siConEvnt.cgPtyCat.cgPtyCat.pres == PRSNT_NODEF) &&\
((con->ccEvntHold->m.ccConEvnt.m.siConEvnt.cgPtyCat.cgPtyCat.val == CAT_EMERGCY) ||\
(con->ccEvntHold->m.ccConEvnt.m.siConEvnt.cgPtyCat.cgPtyCat.val == CAT_NSEP) ||\
(con->ccEvntHold->m.ccConEvnt.m.siConEvnt.cgPtyCat.cgPtyCat.val == CAT_PRIOR) ||\
(con->ccEvntHold->m.ccConEvnt.m.siConEvnt.cgPtyCat.cgPtyCat.val == CAT_HIPRIEMRCY)))

#endif /* End Debug */

#define CC_TREE_MAX_LEAFS        24
#define CC_KEY_MAX_LEN           48       /* max length of key digit sequence */
#define CC_PRFLID_PADDEDLEN      4 
#define CC_PROTTYPE_PADDEDLEN    4
#define CC_RELCAUSE_PADDEDLEN    3
#define CC_RELCAUSE_NOTPRSNT     257
#define CC_RELCAUSE_PRSNTDEF     258
#define CC_RELSTND_PADDEDLEN     3
#define CC_RELSTND_NOTPRSNT      257
#define CC_RELLOC_PADDEDLEN      3
#define CC_RELLOC_NOTPRSNT       257
#define CC_SIPCAUSE_PADDEDLEN    3
#define CC_SIPCAUSE_NOTPRSNT     257
#define CC_SIPCAUSE_PRSNTDEF     258
#define CC_SIPSTATUS_PADDEDLEN   3 
#define CC_SIPSTATUS_NOTPRSNT    700 
#define CC_SIPSTATUS_PRSNTDEF    701

#if 1 /*EARLY_ACM*/
#define IC_EARLY_ACM_TMR_ENABLED(con) ((con->icIntfcCb) &&(con->icIntfcCb->ccTgCb)\
&& (con->icIntfcCb->ccTgCb->tgAtt.acmTmr.enb) && \
(con->icIntfcCb->ccTgCb->tgAtt.acmTmr.val > 0))

/* FID-2575: LMSD UPDATE existing Macro to use new define, and rename macro */
#define IC_RINGBACKTONE_LOCAL(con) ((con->icIntfcCb) && (con->icIntfcCb->ccTgCb)\
&& (con->icIntfcCb->ccTgCb->tgAtt.ringBackTone == LCC_RINGBACKTONE_LOCAL))

/* FID-2575: LMSD + */
#define IC_RINGBACKTONE_UPSTREAM(con) ((con->icIntfcCb) && (con->icIntfcCb->ccTgCb)\
&& (con->icIntfcCb->ccTgCb->tgAtt.ringBackTone == LCC_RINGBACKTONE_UPSTREAM))
/* FID-2575: LMSD - */

#define CC_SET_ACM_SENT_FLAG(con)\
{\
  ccStopConTmr(con, CC_TMR_EARLYACM);\
  con->isACMSent = TRUE;\
} 

/* FID-2575: LMSD UPDATE existing Macro to use new define */
#define IS_EARLY_ACM_RINGBACK_ON(con) (CC_IS_CONTEXT_PRESENT(con) && \
                                       IC_RINGBACKTONE_LOCAL(con) && con->mgPlayingTone && \
                                       (con->mgToneType == MGCT_SIG_ID_CG_RT))

#define CC_RESET_ANY_EARLY_ACM_FLAGS(con)\
{\
  if (IS_EARLY_ACM_RINGBACK_ON(con))\
  {\
    con->mgPlayingTone = FALSE;\
    con->mgToneType    = MGCT_ITEM_ID_CG_MAX;\
  }\
}
#endif

/* CRBT */
#define SWAPTYPE_OANSWER 0
#define SWAPTYPE_OTERMSEIZED 1

#define CC_IS_ASSOCBLOCK_PRESENT(con)     \
            ((con)->assocBlock != NULLP)

#define CC_SET_SENDRELEASE(con,val)        \
  ((con)->assocBlock->sendRelease=val)

#define CC_IS_SENDRELEASE_ENABLED(con)      \
  ((con)->assocBlock->sendRelease == TRUE)

#define CC_SET_ASSOCPARENTSUCONNID(con,SuConnId)   \
  ((con)->assocBlock->assocParentSuConnId=SuConnId)

#define CC_GET_ASSOCPARENTSUCONNID(con)   \
  ((con)->assocBlock->assocParentSuConnId)

#define CC_IS_PREANSTIMEOUT_PRESENT(con)     \
            ((con)->ainInfo.preAnsTimeout != 0)

#define CC_GET_PREANSTIMEOUT(con)     \
            ((con)->ainInfo.preAnsTimeout)

#define CC_SET_SWAPTYPE_OANSWER(con)   \
  ((con)->assocBlock->swapType=SWAPTYPE_OANSWER)

#define CC_SET_SWAPTYPE_OTERMSEIZED(con)   \
  ((con)->assocBlock->swapType=SWAPTYPE_OTERMSEIZED)

#define CC_GET_SWAPTYPE(con)  \
  ((con)->assocBlock->swapType)

#define CC_IS_SWAPTYPE_OANSWER(con)  \
  ((con)->assocBlock->swapType == SWAPTYPE_OANSWER)

#define CC_IS_SWAPTYPE_OTERMSEIZED(con)  \
  ((con)->assocBlock->swapType == SWAPTYPE_OTERMSEIZED)

#define CC_SET_ASSOCSTATE(con)  \
  ((con)->assocBlock->state=con->state)

#define CC_GET_ASSOCSTATE(con)  \
  ((con)->assocBlock->state)

#define CC_IS_STATE_POST_INT_SWAP(con) \
  (((con)->state != CCS_ANSWERED) && \
   ((con)->state != CCS_AWTANS) && \
   ((con)->state != CCS_AWTRSPODISCONNECT) && \
   ((con)->state != CCS_AWTMGCTNOTIFY1))

#define CC_IS_STATE_POST_INT_MOVE(con) \
  (((con)->state != CCS_AWTANS)) 

#define CC_CHECK_STOP_TMR_PREANM(con)\
{\
  ProtType   ogProtType = CC_GET_PROTOCOL(con->ogProtType);\
  if  ((con->direction == OUTTOINC) &&\
      (((ogProtType == CC_SI)  && (con->icEvntType == CC_ET_ACM))             ||\
      ((ogProtType == CC_SIP)  && (con->icEvntType == CCSIP_ET_183_CALLPRG))  ||\
      ((ogProtType == CC_SIP)  && (con->icEvntType == CCSIP_ET_180_RINGING))  ||\
      ((ogProtType == CC_IN)   && (con->icEvntType == CCIN_ET_ALERTING))      ||\
      ((ogProtType == CC_BICC) && (con->icEvntType == CC_ET_ACM))             ||\
      ((ogProtType == CC_SIPT) && (con->icEvntType == CC_ET_ACM))             ||\
      ((ogProtType == CC_EXT_BICC) && (con->icEvntType == CC_ET_ACM)) ))\
  {\
      ccStopConTmr(con, CC_TMR_PREANM);\
  }\
}

/* End CRBT */
     
#define CC_GET_NTP_TIME_STAMP() ((U32)time(NULLP)+2208988800ul)
extern PUBLIC Bool reEntryFlag;

#define CC_IS_REMOTE_REL(relType) (relType & 0x10)

/* 47414 for FID 1950 */
/* 5.1.2, 5.3 and 6.2.0 ProtType was U8 */
/* 6.2.1 also re-defined ProtType */

#if 1
#define EXTRACT_ROUTE_INFO_V0(con, route, plen, mBuf)\
{\
  if ((SRemPreMsgRevMult((Data*) &route, (MsgLen) plen, mBuf)) != ROK)\
  {\
    CCDP(AIN_DBG_LEVEL_3,"%s():: unpacking of Route failed\n",\
         __FUNCTION__);\
    RETVALUE(RFAILED);\
  }\
  CCDP(AIN_DBG_LEVEL_0,"%s():: plen = %d\n", __FUNCTION__, plen);\
  if ( CC_GET_MSB_INTFCTYPE(route.ogInterface.intfType) == 0 ) \
  {\
  if ( (plen == sizeof(BiccRouteInfo_V1)) ||  (plen == sizeof(BiccRouteInfo_V2)))\
  {\
    cmCcMapOldProtTypeToNewProtType_v_1_1( route.ogPrtclType, con->routeEvnt->ogPrtclType[0]);\
    cmMemcpy((U8*)&(con->routeEvnt->ogInterface[0]), (U8*)&(route.ogInterface),\
       sizeof(RmInterface_V0));\
    cmCcMapOldIntfcTypeToNewIntfcType_v_1_1( route.ogInterface.intfType, con->routeEvnt->ogInterface[0].intfType);\
  }\
  else if (plen == sizeof(BiccRouteInfo_V3))\
  {\
    cmCcMapOldProtTypeToNewProtType_v_3_1( route.ogPrtclType, con->routeEvnt->ogPrtclType[0]);\
    cmMemcpy((U8*)&(con->routeEvnt->ogInterface[0]), (U8*)&(route.ogInterface),\
       sizeof(RmInterface_V0));\
    cmCcMapOldIntfcTypeToNewIntfcType_v_3_1( route.ogInterface.intfType, con->routeEvnt->ogInterface[0].intfType);\
  }\
  }\
  else if ( (plen == sizeof(BiccRouteInfo_V4)))\
  {\
    con->routeEvnt->ogPrtclType[0] = route.ogPrtclType;\
    cmMemcpy((U8*)&(con->routeEvnt->ogInterface[0]), (U8*)&(route.ogInterface),\
       sizeof(RmInterface_V0));\
  }\
  else if ( (plen == sizeof(BiccRouteInfo_V5)))\
  {\
    con->routeEvnt->ogPrtclType[0] = route.ogPrtclType;\
  }\
  else if ( (plen == sizeof(BiccRouteInfo_V6)))\
  {\
     con->routeEvnt->ogPrtclType[0] = route.ogPrtclType;\
     cmMemcpy((U8*)&(con->routeEvnt->ogInterface[0]), (U8*)&(route.ogInterface),\
      sizeof(RmInterface_V1));\
  }\
  else\
  {\
     CCDP(AIN_DBG_LEVEL_3,"%s():: unsupported BICC version plen=%d\n",\
         __FUNCTION__, plen);\
    RETVALUE(RFAILED);\
  }\
  con->routeEvnt->ainFlags[0] = route.ainFlags;\
  cmMemcpy((U8*)&(con->routeEvnt->routeIdx[0]), (U8*)&(route.routeIdx),\
           sizeof(U32));\
  cmMemcpy((U8*)&(con->routeEvnt->outPulseNmb),(U8*)&(route.outPulseNmb),\
           sizeof(CdPtyNmb));\
  con->routeEvnt->ccsId[0] = ccCp.genCfg.ownCcsId;\
  con->routeEvnt->numOfOgInterfaces++;\
}
#endif

#define EXTRACT_ROUTE_INFO_V1(con, route, plen, mBuf)\
{\
  EXTRACT_ROUTE_INFO_V0(con, route, plen, mBuf);\
  \
  if (con->routeInfo != NULL)\
  {\
    con->routeInfo->nmbrCategoryPres = route.ccRouteInfoNumCatPres ;\
    con->routeInfo->nmbrCategory     = route.ccRouteInfoNumCat ;\
    CCDP(AIN_DBG_LEVEL_0,"%s():: routeInfo.nmbrCategoryPres = %d, "\
         "routeInfo.nmbrCategory = %d\n", __FUNCTION__,\
         con->routeInfo->nmbrCategoryPres, con->routeInfo->nmbrCategory);\
  }\
  else\
    CCDP(AIN_DBG_LEVEL_0,"%s():: WARNING! con->routeInfo == NULL, "\
        "cannot get ccRouteInfoNumCat\n", __FUNCTION__) ;\
}

#define EXTRACT_ROUTE_INFO_V2(con, route, plen, mBuf)\
{\
  EXTRACT_ROUTE_INFO_V1(con, route, plen, mBuf);\
\
  con->countryCode = route.countryCode ;\
  CCDP(AIN_DBG_LEVEL_0,"%s():: countryCode:(pres,val):(%d,%s)\n", __FUNCTION__,\
       con->countryCode.pres, con->countryCode.val) ;\
  cmMemcpy((U8 *)&con->ainInfo.tns, (U8 *)&(route.tns), sizeof(FcTrnstNtw)) ;\
  CCDP(AIN_DBG_LEVEL_0,"%s():: ainInfo.tns.netId:(pres,val):(%d,%s), "\
       "tns.ozz:(%d,%d)\n", __FUNCTION__, con->ainInfo.tns.netId.pres,\
       con->ainInfo.tns.netId.val, con->ainInfo.tns.ozz.pres,\
       con->ainInfo.tns.ozz.val);\
} 

#define EXTRACT_ROUTE_INFO_V3(con, route, plen, mBuf)\
{\
  EXTRACT_ROUTE_INFO_V2(con, route, plen, mBuf);\
\
  cmMemcpy((U8 *)&con->ainInfo.MdpTrunkGroup, (U8 *)&(route.MdpTrunkGroup), sizeof(FcTrnkGrpFmt)) ;\
  CCDP(AIN_DBG_LEVEL_0,"%s():: route.MdpTrunkGroup.eh:(pres):(%d),\n "\
       "       MdpTrunkGroup.trnkGrpNum:(%d,%ld)\n "\
       "       MdpTrunkGroup.trnkId:(%d,%ld)\n",\
       __FUNCTION__, route.MdpTrunkGroup.eh.pres,\
       route.MdpTrunkGroup.trnkGrpNum.pres, route.MdpTrunkGroup.trnkGrpNum.val,\
       route.MdpTrunkGroup.trnkId.pres, route.MdpTrunkGroup.trnkId.val) ;\
}

#define EXTRACT_ROUTE_INFO_V4(con, route, plen, mBuf)\
{\
  EXTRACT_ROUTE_INFO_V3(con, route, plen, mBuf);\
\
}

#define EXTRACT_ROUTE_INFO_V5(con, route, plen, mBuf)\
{\
  EXTRACT_ROUTE_INFO_V4(con, route, plen, mBuf);\
  cmMemcpy((U8 *)&(con->routeEvnt->cacInfo[0].localMaxBw), (U8 *)&(route.localMaxBw), sizeof(TknU32));\
}

#define EXTRACT_ROUTE_INFO_V6(con, route, plen, mBuf)\
{\
  EXTRACT_ROUTE_INFO_V5(con, route, plen, mBuf);\
}

/* AAG */
#define CC_ENDPOINT_CALL(_con)                                                 \
(                                                                              \
    ((_con) &&                                                                 \
     ((CC_GET_PROTOCOL(_con->icProtType) == CC_CS_LN) ||                         \
      (CC_GET_PROTOCOL(_con->ogProtType) == CC_CS_LN) ||                         \
      (CC_GET_PROTOCOL(_con->icProtType) == CC_GR303) ||                         \
      (CC_GET_PROTOCOL(_con->ogProtType) == CC_GR303)))                          \
)
#define CC_ENDPOINT_BCM(_bcm)                                                  \
(                                                                              \
    ((_bcm) && ((_bcm)->con) &&                                                \
     (((_bcm)->bcmType == BtOBcm) ?                                            \
           ((CC_GET_PROTOCOL((_bcm)->con->icProtType) == CC_CS_LN) ||          \
             (CC_GET_PROTOCOL((_bcm)->con->icProtType) == CC_GR303)):          \
           ((CC_GET_PROTOCOL((_bcm)->con->ogProtType) == CC_CS_LN) ||          \
             (CC_GET_PROTOCOL((_bcm)->con->ogProtType) == CC_GR303))))         \
)

#define CC_MAP_ALERTINFO_TO_CTL_LEG_TREAT(_pAlertInfo, _con)                   \
{                                                                              \
    if ((_pAlertInfo) && (_pAlertInfo->pres == PRSNT_NODEF))                   \
    {                                                                          \
        switch(_pAlertInfo->val)                                               \
        {                                                                      \
            case(ALERT_INFO_RING_SPLASH):                                      \
                _con->ainInfo.ControllingLegTreatment =                        \
                                   CltAlertingOnPattern4ReminderRing;          \
                CCDP(AIN_DBG_LEVEL_0, "Filled ControllingLegTreatment = %d\n", \
                    _con->ainInfo.ControllingLegTreatment);                    \
                break;                                                         \
            default:                                                           \
                break;                                                         \
        }                                                                      \
        CCDP(AIN_DBG_LEVEL_0, "_pAlertInfo->pres:%d, _pAlertInfo->val:%d\n",   \
             _pAlertInfo->pres, _pAlertInfo->val);                             \
    }                                                                          \
}
#define CC_FILL_BUSY_CAUSE(_busyCause, _cause, _sipCause, _sipStatus)          \
{                                                                              \
    Bool _fill = FALSE;                                                        \
    if (_cause.eh.pres == PRSNT_NODEF)                                         \
    {                                                                          \
        if (_cause.location.pres == PRSNT_NODEF)                               \
        {                                                                      \
            _busyCause.location = _cause.location;                             \
            _fill = TRUE;                                                      \
        }                                                                      \
        if (_cause.cdeStand.pres == PRSNT_NODEF)                               \
        {                                                                      \
            _busyCause.cdeStand = _cause.cdeStand;                             \
            _fill = TRUE;                                                      \
        }                                                                      \
        if (_cause.causeVal.pres == PRSNT_NODEF)                               \
        {                                                                      \
            _busyCause.causeVal = _cause.causeVal;                             \
            _fill = TRUE;                                                      \
        }                                                                      \
        if (_cause.dgnVal.pres == PRSNT_NODEF)                                 \
        {                                                                      \
            _busyCause.dgnVal = _cause.dgnVal;                                 \
            _fill = TRUE;                                                      \
        }                                                                      \
        if (_sipStatus.pres == PRSNT_NODEF)                                    \
        {                                                                      \
            _busyCause.statusCode.pres = PRSNT_NODEF;                          \
            _busyCause.statusCode.val = _sipStatus.val;                        \
            _fill = TRUE;                                                      \
        }                                                                      \
        if (_fill == TRUE)                                                     \
            _busyCause.eh = _cause.eh;                                         \
    }                                                                          \
}
/* AAG */
/* T.38 ++*/

#define T38_UDP_SHIFT_LEVEL          5
#define T38_TCP_SHIFT_LEVEL          4
#define T38_UDP_TCP_SHIFT_LEVEL      3
#define T38_TCP_UDP_SHIFT_LEVEL      2
#define AUDIO_VBD_ONLY_SHIFT_LEVEL   1
#define AUDIO_ONLY_SHIFT_LEVEL       0

/* FID 14844 -*/
#define T38_UDP_SHIFT_LEVEL_MASK           32
#define T38_TCP_SHIFT_LEVEL_MASK           16
#define T38_UDP_TCP_SHIFT_LEVEL_MASK       8
#define T38_TCP_UDP_SHIFT_LEVEL_MASK       4
#define AUDIO_VBD_ONLY_SHIFT_LEVEL_MASK    2
#define AUDIO_ONLY_SHIFT_LEVEL_MASK        1

#define LOCAL_V21_FLAG_DETECTED      1
#define REMOTE_REQUEST_FOR_T38       2
#define REMOTE_REJECT_T38_SWITCH     3
#define LOCAL_MODEM_FLAG_DETECTED    4
#define LOCAL_TEXT_FLAG_DETECTED     5
#define LOCAL_PI_EOF_FLAG_DETECTED   6  /*FID 16229.0*/

/*FID 16087.0 +*/
#define LOCAL_HIGH_MODEM_FLAG_DETECT    0x01
#define LOCAL_LOW_FAX_FLAG_DETECT       0x02
#define LOCAL_LOW_MODEM_FLAG_DETECT     0x03

#define MODEM_FAX_EVENT_SHIFT_LEVEL     6
#define MODEM_FAX_EVENT_SHIFT_LEVEL_MASK  0xc0
#define GET_MODEM_FAX_EVENT(flag)  \
(((flag)&MODEM_FAX_EVENT_SHIFT_LEVEL_MASK)>>MODEM_FAX_EVENT_SHIFT_LEVEL)
#define SET_MODEM_FAX_EVENT(flag, event)  \
{  \
   (flag)=(flag)&(~MODEM_FAX_EVENT_SHIFT_LEVEL_MASK);  \
   (flag)=(flag)|(((event)<<MODEM_FAX_EVENT_SHIFT_LEVEL)&MODEM_FAX_EVENT_SHIFT_LEVEL_MASK);  \
}

#define SDP_ECAN_DIR_SHIFT_LEVEL        4
#define SDP_ECAN_DIR_SHIFT_LEVEL_MASK   0x30
#define GET_SDP_ECAN_DIR(flag)  \
(((flag)&SDP_ECAN_DIR_SHIFT_LEVEL_MASK)>>SDP_ECAN_DIR_SHIFT_LEVEL)
#define SET_SDP_ECAN_DIR(flag, event) \
{  \
   (flag)=(flag)&(~SDP_ECAN_DIR_SHIFT_LEVEL_MASK);  \
   (flag)=(flag)|(((event)<<SDP_ECAN_DIR_SHIFT_LEVEL)&SDP_ECAN_DIR_SHIFT_LEVEL_MASK);  \
}
/*
The following values defined in cm_sdp.h will be reused.
#define  SDP_ECAN_DIR_F       0x01
#define  SDP_ECAN_DIR_B       0x02
#define  SDP_ECAN_DIR_FB      0x03
*/

#define SDP_ECAN_ENB_SHIFT_LEVEL        2
#define SDP_ECAN_ENB_SHIFT_LEVEL_MASK   0x0c
#define GET_SDP_ECAN_ENB(flag)  \
(((flag)&SDP_ECAN_ENB_SHIFT_LEVEL_MASK)>>SDP_ECAN_ENB_SHIFT_LEVEL)
#define SET_SDP_ECAN_ENB(flag, event)  \
{  \
  (flag)=(flag)&(~SDP_ECAN_ENB_SHIFT_LEVEL_MASK);  \
  (flag)=(flag)|(((event)<<SDP_ECAN_ENB_SHIFT_LEVEL)&SDP_ECAN_ENB_SHIFT_LEVEL_MASK);  \
}
/*
The following values defined in cm_sdp.h will be resued.
#define  SDP_ECAN_ENB_NONE       0x00
#define  SDP_ECAN_ENB_ON         0x01
#define  SDP_ECAN_ENB_OFF        0x02
*/

#define SDP_ECAN_TP_SHIFT_LEVEL       0
#define SDP_ECAN_TP_SHIFT_LEVEL_MASK  0x03
#define GET_SDP_ECAN_TP(flag)  \
(((flag)&SDP_ECAN_TP_SHIFT_LEVEL_MASK)>>SDP_ECAN_TP_SHIFT_LEVEL)
#define SET_SDP_ECAN_TP(flag, event)  \
{  \
  (flag)=(flag)&(~SDP_ECAN_TP_SHIFT_LEVEL);  \
  (flag)=(flag)|(((event)<<SDP_ECAN_TP_SHIFT_LEVEL)&SDP_ECAN_TP_SHIFT_LEVEL_MASK); \
}
/*
The following values defined in cm_sdp.h will be resued.
#define  SDP_ECAN_TP_NONE        0x00
#define  SDP_ECAN_TP_G165        0x01
#define  SDP_ECAN_TP_G168        0x02
*/
/*FID 16087.0 -*/

/* T.38 -- */

/* FID 15319.9 CSD --*/
#define FILLSDP_NORMAL                0
#define FILLSDP_ONLY_CLEARMODE        1
#define FILLSDP_CLEARMODE_FIRSTSPEECH 2

#define CC_IS_WAITMORE_INFO(waitForMoreInfo)  (((waitForMoreInfo) & 0x07)? TRUE : FALSE)
#define CC_IS_COTBUFFERED(con)    ((con->waitForMoreInfo & 0x08)? TRUE : FALSE)
#define CC_IS_SAMBUFFERED(con)    ((con->waitForMoreInfo & 0x10)? TRUE : FALSE)
#define CC_IS_INFBUFFERED(con)    ((con->waitForMoreInfo & 0x20)? TRUE : FALSE)
#define CC_SET_IN_WAITMORE_INFO(moreInfo) (con->waitForMoreInfo = ((con->waitForMoreInfo & 0xf8) | ((moreInfo) & 0x07)))
#define CC_IS_IN_PLAYDIALTONE(moreInfo)  ((con->waitForMoreInfo & 0x04)? TRUE : FALSE) /* Kavon */
#define CC_MARK_COTBUFFERED(con)  (con->waitForMoreInfo |= 0x08)
#define CC_MARK_SAMBUFFERED(con)  (con->waitForMoreInfo |= 0x10)
#define CC_MARK_INFBUFFERED(con)  (con->waitForMoreInfo |= 0x20)
#define CC_CLEAR_COTBUFFERED(con)  (con->waitForMoreInfo &= 0xF7)
#define CC_CLEAR_SAMBUFFERED(con)  (con->waitForMoreInfo &= 0xEF)
#define CC_CLEAR_INFBUFFERED(con)  (con->waitForMoreInfo &= 0xDF)
/* FID 15174.0 */
#define CC_IS_AWAIT_SAM(con)    ((con->waitForMoreInfo & 0x02)? TRUE : FALSE)
#define CC_MARK_AWAIT_SAM(con)  (con->waitForMoreInfo |= 0x02)
#define CC_CLEAR_AWAIT_SAM(con)  (con->waitForMoreInfo &= ~(0x02))
#define CC_SET_WAITMORE_INFO(moreInfo) (con->waitForMoreInfo = ((con->waitForMoreInfo & 0xfa) | ((moreInfo) & 0x05)))

#define CC_IS_ICOVERLAP_DISABLED(con) \
    ((con) && (con->icIntfcCb) && \
     (con->icIntfcCb->overlapCfg.icOverlap == TGP_ICOVERLAP_DISABLED))

#define CC_IS_ICOVERLAP_ENABLED(con) \
    ((con) && (con->icIntfcCb) && \
     (con->icIntfcCb->overlapCfg.icOverlap != TGP_ICOVERLAP_DISABLED))

#define CC_IS_SI_BICC_ICOVERLAP(con) \
    ((con) && (con->icIntfcCb) && \
     ((con->icIntfcCb->overlapCfg.icOverlap == TGP_ICOVERLAP_ENABLED) || \
      (con->icIntfcCb->overlapCfg.icOverlap == TGP_ICOVERLAP_SENDACM)))

#define CC_IS_ICOVERLAP_SENDACM(con) \
    ((con) && (con->icIntfcCb) && \
     (con->icIntfcCb->overlapCfg.icOverlap == TGP_ICOVERLAP_SENDACM))

#define CC_IS_SIP_ICOVERLAP_INDIALOG(con) \
    ((con) && (con->icIntfcCb) && \
     (con->icIntfcCb->overlapCfg.icOverlap == TGP_ICOVERLAP_INDIALOG))

#define CC_IS_SIP_ICOVERLAP(con) \
    (CC_IS_SIP_ICOVERLAP_INDIALOG(con) || \
     (CCT_IS_SIP_OVERLAP_IN_DIALOG_SUPPORTED(con->icSipMethodSupMask)))

#define CC_IS_SIPT_ICOVERLAP(con) \
    ((con) && (con->icIntfcCb) && \
     (con->icIntfcCb->overlapCfg.icOverlap == TGP_ICOVERLAP_MULTIINVITE))

#define CC_IS_OGOVERLAP_DISABLED(con) \
    ((con) && (con->ogIntfcCb) && (!(con->ogIntfcCb->overlapCfg.ogOverlap)))

#define CC_IS_OGOVERLAP_ENABLED(con) \
    ((con) && (con->ogIntfcCb) && (con->ogIntfcCb->overlapCfg.ogOverlap))

#define CC_IS_OVERLAPTOENBLOC_REQED(con) \
    ((con)&& CC_IS_CALL_FLAG_SET(con->overlapToEnblocFlag,CC_OVERLAPTOENBLOC_REQ_RECVED))
	
#define CC_IS_ENBLOC_OVERLAP(con) \
    (CC_IS_ICOVERLAP_ENABLED(con) && \
     ((ccGenPrfl.overlapToEnbloc == TRUE) || CC_IS_OVERLAPTOENBLOC_REQED(con) || CC_IS_OGOVERLAP_DISABLED(con)))

#define CC_STARTTMR_EARLYACM(con)                                              \
{                                                                              \
    if((con->EarlyACMSt != EARLY_ACM_HAS_BEEN_SENT) &&                         \
       (IC_EARLY_ACM_TMR_ENABLED(con)))                                        \
    {                                                                          \
        ccStartConTmr(CC_TMR_EARLYACM, con, (PTR)&ccCp.genCfg);                \
    }                                                                          \
}

#define CC_STARTTMR_MINDIGIT(con)                                              \
{                                                                              \
    Bool bRet1 = FALSE;                                                        \
    CC_MARK_AWAIT_SAM(con);                                                    \
    if (CC_IS_DDI_VARIANT(con->icProtType))                                    \
    {                                                                          \
        bRet1 = ccMatchTestCallTimer(CC_TMR_MINDIGIT,                          \
                  &(con->icIntfcCb->ccDDIOption.optChkTestCall),               \
                  &(con->ccEvntHold->m.ccConEvnt.m.siConEvnt.cgPtyCat));       \
    }                                                                          \
    if (bRet1 == FALSE)                                                        \
    {                                                                          \
        ccStartConTmr(CC_TMR_MINDIGIT, con, (PTR)&ccCp.genCfg);                \
    }                                                                          \
    if(con->state != CCS_AWTMOREINFO)                                          \
    {                                                                          \
        con->lastState = con->state;                                           \
        CC_STATE_CHANGE(con, CCS_AWTMOREINFO);                                 \
    }                                                                          \
    RETVALUE(ROK);                                                             \
}

#define CC_CHECK_SI_MINDIGITS(con, cdLen)                                      \
{                                                                              \
    Bool bRet2 = TRUE;                                                         \
    if ((con) && (con->icIntfcCb) &&                                           \
        (con->icIntfcCb->overlapCfg.minDigits >0))                             \
    {                                                                          \
        bRet2 = CC_GOT_TG_MINDIGITS(cdLen, con->icIntfcCb->overlapCfg);        \
    }                                                                          \
    else                                                                       \
    {                                                                          \
        bRet2 = CC_GOT_DR_MINDIGITS(cdLen,con);                                \
    }                                                                          \
    if(bRet2 == FALSE)                                                         \
    {                                                                          \
        CC_STARTTMR_MINDIGIT(con);                                             \
    }                                                                          \
}

#define CC_CHECK_SI_MAXDIGITS(con, cdLen)                                      \
{                                                                              \
    if (CC_GOT_MAXDIGITS(cdLen, con->icIntfcCb->overlapCfg))                   \
    {                                                                          \
        con->STDigitRecd = TRUE;                                               \
        CC_CLEAR_AWAIT_SAM(con);                                               \
    }                                                                          \
    else                                                                       \
    {                                                                          \
        CC_MARK_AWAIT_SAM(con);                                                \
        ccStartConTmr(CC_TMR_MAXDIGIT, con, (PTR)&ccCp.genCfg);                \
        if(con->state != CCS_AWTMOREINFO)                                      \
        {                                                                      \
            con->lastState = con->state;                                       \
            CC_STATE_CHANGE(con, CCS_AWTMOREINFO);                             \
        }                                                                      \
        RETVALUE(ROK);                                                         \
    }                                                                          \
}

#define CC_CHECK_SIP_MAXDIGITS(con, cdLen)                                     \
{                                                                              \
    if (CC_GOT_MAXDIGITS(cdLen, con->icIntfcCb->overlapCfg))                   \
    {                                                                          \
        con->STDigitRecd = TRUE;                                               \
        ccSendOrSkipInfoCollected(con);                                        \
    }                                                                          \
    else                                                                       \
    {                                                                          \
        ccStartConTmr(CC_TMR_MAXDIGIT, con, (PTR)&ccCp.genCfg);                \
        RETVALUE(ROK);                                                         \
    }                                                                          \
}

#define CC_DISCARD_SAM(con)                                                    \
{                                                                              \
    if(((CC_GET_PROTOCOL(con->icProtType) == CC_SI)||                          \
        (CC_GET_PROTOCOL(con->icProtType) == CC_EXT_BICC)) &&                  \
       (CC_IS_ICOVERLAP_DISABLED(con) || (con->STDigitRecd)))                  \
    {                                                                          \
        CCDP(AIN_DBG_LEVEL_0, "discard SAM message\n");                        \
        RETVALUE(ROK);                                                         \
    }                                                                          \
}

#define CC_GET_MINDIGITS(con,dest)                                             \
{                                                                              \
    if ((con) && (con->icIntfcCb) &&                                           \
        (con->icIntfcCb->overlapCfg.minDigits >0))                             \
    {                                                                          \
        dest = con->icIntfcCb->overlapCfg.minDigits;                           \
    }                                                                          \
    else                                                                       \
    {                                                                          \
        dest = con->minDigits;                                                 \
    }                                                                          \
}

#define CC_SET_MINDIGITS_TO_SIPW(con,cause,relEvnt)                            \
{                                                                              \
   if(cause == CCADDRINCOMP)                                                   \
   {                                                                           \
     if(CC_IS_SIPT_ICOVERLAP(con) &&                                           \
        (CC_GET_PROTOCOL(con->icProtType) == CC_SIPT))                         \
     {                                                                         \
       CC_GET_MINDIGITS(con, relEvnt.m.siRelEvnt.optElmnt.m.sipt.minDigits);   \
       CCDP(AIN_DBG_LEVEL_2,"sipt.minDigits=%d\n",                             \
            relEvnt.m.siRelEvnt.optElmnt.m.sipt.minDigits);                    \
     }                                                                         \
     else if(CC_IS_SIP_ICOVERLAP(con) && (con->icProtType == CC_SIP))          \
     {                                                                         \
       CC_GET_MINDIGITS(con, relEvnt.m.sipRelEvnt.minDigits);                  \
       CCDP(AIN_DBG_LEVEL_2,"sip.minDigits=%d\n",                              \
            relEvnt.m.sipRelEvnt.minDigits);                                   \
     }                                                                         \
   }                                                                           \
}

#define CC_IS_GEO_AUDIT_NOT_ALLOWED(mgSpId) ccIsGeoAuditNotAllowed(mgSpId)
#define CC_IS_CCDBGMASK_ON (ccInit.dbgMask != 0)

#define CC_MG_CONG_LEVEL_0       0
#define CC_MG_CONG_TIMER_RES     1  /*100ms*/
#define CC_MG_CONG_TIMER_1       1
#define CC_MG_CONG_TIMER_MAX     (CC_MG_CONG_TIMER_1 + 1)
#define CC_MG_CONG_LEVEL_STEP    10
#define CC_MG_CONG_TIMER_WAIT    300 /*30s*/
#define CC_MG_CONG_LEVEL1_THRESH 75
#define CC_MG_CONG_LEVEL2_THRESH 100

#define IS_THIS_CALL_EMERGENCY(con) \
		(((con) && (con->routeInfo) && \
		 (con->routeInfo->nmbrCategoryPres) && \
		 (con->routeInfo->nmbrCategory == SUBSCRIBER_EMERGENCY)) || \
		 ((con) && (con->specialCall & SPECIAL_CALL_EMERGENCY)))

#define IS_THIS_CALL_PRIORITY(con) \
               ((con) && (con->specialCall & SPECIAL_CALL_PRIORITY))

/* FID: 14457.0 GETS */
#define IS_THIS_CALL_GETS(con) \
                ((con) && (con->specialCall & SPECIAL_CALL_GETS))

#define CC_IS_GETS_UNSUPP(con,sipStatus) \
                ((IS_THIS_CALL_GETS(con) && \
		 ((CC_GET_PROTOCOL(con->ogProtType) == CC_SIP) || \
                  (CC_GET_PROTOCOL(con->ogProtType) == CC_SIPT)) &&  \
                 (sipStatus.pres == PRSNT_NODEF) &&  \
                 ((sipStatus.val == SIPCAUSE_420_BADEXT)   ||  \
                  (sipStatus.val == SIPCAUSE_417_UNKNOWNRP))))
                 
#define CC_RPH_DOWNGRADE 0x80
#define CC_HPCQ_AR       0x40

#define CC_CLR_RPH_DOWNGRADE_FLAG(con,flag)                     \
  (con->specialCall) &= ~flag
#define CC_SET_RPH_DOWNGRADE_FLAG(con,flag)                     \
  (con->specialCall) |= flag
#define CC_IS_RPH_DOWNGRADE_FLAG_SET(con,flag)                  \
  ((con->specialCall) & flag)

#define IS_THIS_CALL_SPECIAL_CALL(con) \
                ((IS_THIS_CALL_PRIORITY(con))  || \
                 (IS_THIS_CALL_EMERGENCY(con)) || \
                 (IS_THIS_CALL_GETS(con)))

#define CC_INC_XFER_COUNT(con)                                                 \
{                                                                              \
    con->outStandingXferCount++;                                               \
}

#define CC_DEC_XFER_COUNT(_con)                                                \
{                                                                              \
    if (_con->outStandingXferCount > 0)                                        \
    {                                                                          \
        _con->outStandingXferCount--;                                          \
    }                                                                          \
    else                                                                       \
    {                                                                          \
        ABORT_DEBUG;                                                           \
        CC_TRACE_DUMP((_con));                                                 \
    }                                                                          \
}

/* 52162 + */
#define CC_IS_OBCM_PIC_ACTIVE(CONN) (((CONN)->icBcm.pic == PicOActive) ||\
                                     ((CONN)->icBcm.pic == PicOSuspended))
#define CC_IS_TBCM_PIC_ACTIVE(CONN) (((CONN)->ogBcm.pic == PicTActive) ||\
                                     ((CONN)->ogBcm.pic == PicTSuspended))
/* 52162 - */

/* FID-2575: LMSD + */
#define CC_IS_IC_LMSD_AI(con) \
         ((con->icIntfcCb) && (con->icIntfcCb->ccTgCb) &&\
          (CC_GET_PROTOCOL(con->icIntfcCb->protType) == CC_SIPT) &&\
          (con->icIntfcCb->ccTgCb->tgAtt.sipTPrflPtr) &&\
          (con->icIntfcCb->ccTgCb->tgAtt.sipTPrflPtr->emHdrType == LCC_EMHDRTYPE_LMSD_AI))

#define CC_IS_INGRESS_CCS_LMSD_AI(con)\
        ((con->icIntfcCb) && (con->icIntfcCb->ccTgCb) && (CC_INGRESS_INTER_CCS_CALL(con)) &&\
         (CC_GET_PROTOCOL(con->icIntfcCb->protType) == CC_SIPT) &&\
         (con->icIntfcCb->ccTgCb->tgAtt.sipTPrflPtr) &&\
         (con->icIntfcCb->ccTgCb->tgAtt.sipTPrflPtr->emHdrType == LCC_EMHDRTYPE_LMSD_AI))

#define CC_IS_OG_LMSD_AI(con) \
        ((con->ogIntfcCb) && (con->ogIntfcCb->ccTgCb) &&\
         (CC_GET_PROTOCOL(con->ogIntfcCb->protType) == CC_SIPT) &&\
         (con->ogIntfcCb->ccTgCb->tgAtt.sipTPrflPtr) &&\
         (con->ogIntfcCb->ccTgCb->tgAtt.sipTPrflPtr->emHdrType == LCC_EMHDRTYPE_LMSD_AI))

#define CC_IS_EGRESS_CCS_LMSD_AI(con)\
        ((con->ogIntfcCb) && (con->ogIntfcCb->ccTgCb) && (CC_EGRESS_INTER_CCS_CALL(con)) &&\
         (CC_GET_PROTOCOL(con->ogIntfcCb->protType) == CC_SIPT) &&\
         (con->ogIntfcCb->ccTgCb->tgAtt.sipTPrflPtr) &&\
         (con->ogIntfcCb->ccTgCb->tgAtt.sipTPrflPtr->emHdrType == LCC_EMHDRTYPE_LMSD_AI))


#define CC_IS_IC_BCM_2_PARTY_CALL(con) \
        (((con)->icRscVal) && \
        (((con)->icBcm.cc == NULL) || \
         (((con)->icBcm.cc)&& \
          ((con)->icBcm.cc->ccId >= CiNull)&& \
          ((con)->icBcm.cc->ccId <= CiStable2Party))))

#define CC_IS_OG_BCM_2_PARTY_CALL(con) \
        (((con)->ogRscVal) && \
        (((con)->ogBcm.cc == NULL) || \
         (((con)->ogBcm.cc)&& \
          ((con)->ogBcm.cc->ccId >= CiNull)&& \
          ((con)->ogBcm.cc->ccId <= CiTerminatingSetup)&& \
          ((con)->ogBcm.cc->ccId != CiOriginatingSetup))))

#define CC_IS_LMSD_AI(alertInfo,val) (alertInfo==val)

#define CC_IS_LMSD_AI_LCP(alertInfo) \
        ((alertInfo==AI_RT) || (alertInfo==AI_BT) ||\
         (alertInfo==AI_CT) || (alertInfo==AI_CR) ||\
         (alertInfo==AI_CMFT))

#define CC_IS_LMSD_AI_RCP(alertInfo) \
        (alertInfo==AI_NULL)

#define CC_IS_LMSD_AI_LCP_OR_RCP(alertInfo) \
        ((alertInfo==AI_RT) || (alertInfo==AI_BT) ||\
         (alertInfo==AI_CT) || (alertInfo==AI_CR) ||\
         (alertInfo==AI_CMFT) || (alertInfo==AI_NULL))

/* Bug 80155 */
#define CC_IS_MG_FOR_NAT(con) \
        (CC_IS_CALL_FLAG_SET(con->flag,CC_FLAG_NAT) && \
         (((ccIsIPResource(&(con->icBcm)) && (CC_GET_PROTOCOL(con->icProtType) != CC_EXT_BICC)) && \
           (ccIsIPResource(&(con->ogBcm)) && (CC_GET_PROTOCOL(con->ogProtType) != CC_EXT_BICC))) || \
          (CC_IS_CALL_MULTI_MG(con) && \
           (((con->ccmCb.state == CC_INGRESS_MG) && ccIsIPResource(&(con->icBcm)) && \
              (CC_GET_PROTOCOL(con->icProtType) != CC_EXT_BICC)) || \
            ((con->ccmCb.state == CC_EGRESS_MG) && ccIsIPResource(&(con->ogBcm))  && \
              (CC_GET_PROTOCOL(con->ogProtType) != CC_EXT_BICC)) || \
            (con->ccmCb.state == CC_Y_MG)))))

/* BUG 84329 - Add CC_IS_X3_ALLOWED */
#define CC_IS_MG_FOR_SORM(con) \
        ((CC_IS_CALL_TAPPED(con) && (CC_IS_X3_ALLOWED(con)))&& \
         ((ccIsIPResource(&(con->icBcm)) && ccIsIPResource(&(con->ogBcm))) || \
          (CC_IS_CALL_MULTI_MG(con) && \
           (((con->ccmCb.state == CC_INGRESS_MG) && ccIsIPResource(&(con->icBcm))) || \
            ((con->ccmCb.state == CC_EGRESS_MG) && ccIsIPResource(&(con->ogBcm))) || \
            (con->ccmCb.state == CC_Y_MG)))))

/* FID 14341:IPV6 */
#define CC_IS_MG_FOR_IPV6(con)\
        (CC_IS_CALL_FLAG_SET(con->flag, CC_FLAG_IPV_CONVERSION) && \
        ((ccIsIPResource(&(con->icBcm)) && ccIsIPResource(&(con->ogBcm))) || \
        (CC_IS_CALL_MULTI_MG(con) && \
         (((con->ccmCb.state == CC_INGRESS_MG) && ccIsIPResource(&(con->icBcm))) || \
            ((con->ccmCb.state == CC_EGRESS_MG) && ccIsIPResource(&(con->ogBcm))) || \
            (con->ccmCb.state == CC_Y_MG)))))


#define CC_IS_MG_NEEDED_IN_IP_CALL(con) \
        (((CC_IS_MG_FOR_NAT(con)) || \
         (CC_IS_MG_FOR_IPV6(con)) || \
         (CC_IS_CALL_FLAG_SET(con->flag,CC_FLAG_TRANSCODING)) || \
         (CC_IS_CALL_FLAG_SET(con->flag,CC_FLAG_IW_GAP)) || \
         (CC_IS_MG_FOR_SORM(con)) || \
         (CC_IS_CALL_FLAG_SET(con->flag,CC_FLAG_EARLYMEDIA_IW))) && \
         (CC_IS_IC_BCM_2_PARTY_CALL(con)) && \
         (CC_IS_OG_BCM_2_PARTY_CALL(con)))

/* comfort tone is not supported by LNG, play silence */
#define MAP_AI_TO_CG_SIGNAL(signal,alertInfo) \
{ \
   switch(alertInfo) \
   { \
      case AI_RT: \
         signal = MGCT_SIG_ID_CG_RT; \
         break; \
      case AI_BT: \
         signal = MGCT_SIG_ID_CG_BT; \
         break; \
      case AI_CT: \
         signal = MGCT_SIG_ID_CG_CT; \
         break; \
      case AI_CR: \
         signal = MGCT_SIG_ID_CG_CR; \
         break; \
      case AI_CMFT: \
         signal = MGCT_ITEM_ID_CG_NULL; \
         break; \
      case AI_NULL: \
         signal = MGCT_ITEM_ID_CG_NULL; \
         break; \
      case AI_NULL_SIM: /* BUG:70759 + */  \
         signal = MGCT_ITEM_ID_CG_NULL;    \
         break;         /* BUG:70759 - */  \
      default: \
         signal = MGCT_ITEM_ID_CG_MAX; \
         break; \
   } \
}

/* FID-2575: LMSD - */

/* FID 15045.1 + */
#define CC_IS_SSP_RELAY_MODE(con) \
        (CC_IS_CALL_FLAG_SET(con->flag,CC_FLAG_RELAY_MODE))

#define CC_IS_OG_VALID_SRP_PROTOCOL(con) \
        ((con->ogIntfcCb) && (con->ogIntfcCb->ccTgCb) &&\
         (CC_GET_PROTOCOL(con->ogIntfcCb->protType) == CC_SI) &&\
         ((CC_IS_PROT_TYPE_ANSI_ISUP(con->ogIntfcCb->protType)) != TRUE) &&\
         ((CC_IS_PROT_TYPE_ANSI_ISUP(con->ogProtType)) != TRUE) &&\
         (CC_IS_PROT_TYPE_ANSI_BICC(con->ogIntfcCb) != TRUE))

#define CC_IS_MSG_RCV_FROM_SRP(con) \
        ((CC_IS_SSP_RELAY_MODE(con)) && (con->direction == OUTTOINC) &&\
         (!CC_INGRESS_INTER_CCS_CALL(con)) &&\
         (con->ogIntfcCb) && (con->ogIntfcCb->ccTgCb) &&\
         (CC_GET_PROTOCOL(con->ogIntfcCb->protType) == CC_SI))

#define CC_IS_CANCEL_IN_PROGRESS(con) \
        (CC_IS_CALL_FLAG_SET(con->flag,CC_FLAG_CANCEL))

/* FID 15045.1 - */


/* FID-14209: Enhanced Transcoding + */
#define CC_IS_CODEC_DIRECT(_codec1, _codec2) \
         ((_codec1) == (_codec2))

/* FID-14209: Enhanced Transcoding - */

/* FID 15681 - Setting Transcoding Policies */

#define	CC_MAX_CODEC_PAIR	6	

/* See if Codec Pair (_codec1, _codec2) satisfies Transcoding Policy Rule */
#define CC_IS_CODECPAIR_VALID(_codec1,_codec2,_tp_codec1,_tp_codec2) \
  (((((_codec2 == _tp_codec2)  || (_tp_codec2 == SDP_CODEC_ASTERISK)) && \
  ((_codec1 == _tp_codec1) || (_tp_codec1 == SDP_CODEC_ASTERISK))) ||    \
    (((_codec1 == _tp_codec2) || (_tp_codec2 == SDP_CODEC_ASTERISK)) &&  \
    ((_codec2 == _tp_codec1) || (_tp_codec1 == SDP_CODEC_ASTERISK))))    \
     ? TRUE: FALSE)     

/* FID 15681 - Get Transcoding Policies for ingress and egress trunks */
#define CC_GET_TRANSCODE_POLICY_PAIR(con, _transcodePolicy1, _transcodePolicy2) \
{\
      if (con->icIntfcCb && con->icIntfcCb->ccTgCb)			\
      { 								\
         if (CC_GET_PROTOCOL(con->icProtType) == CC_SIP)		\
         { 								\
	    if (con->icIntfcCb->ccTgCb->tgAtt.sipPrflPtr)		\
               _transcodePolicy1 = con->icIntfcCb->ccTgCb->tgAtt.sipPrflPtr->transcodePolicy;								\
         } else if ((CC_GET_PROTOCOL(con->icProtType) == CC_SIPT)) {	\
            if (con->icIntfcCb->ccTgCb->tgAtt.sipTPrflPtr)              \
               _transcodePolicy1 = con->icIntfcCb->ccTgCb->tgAtt.sipTPrflPtr->transcodePolicy; 								\
         }  								\
      }									\
      if (con->ogIntfcCb && con->ogIntfcCb->ccTgCb)			\
      { 								\
         if (CC_GET_PROTOCOL(con->ogProtType) == CC_SIP)		\
         {								\
            if (con->ogIntfcCb->ccTgCb->tgAtt.sipPrflPtr)               \
               _transcodePolicy2 = con->ogIntfcCb->ccTgCb->tgAtt.sipPrflPtr->transcodePolicy; 								\
         } else if((CC_GET_PROTOCOL(con->ogProtType) == CC_SIPT)) {	\
            if (con->ogIntfcCb->ccTgCb->tgAtt.sipTPrflPtr)               \
               _transcodePolicy2 = con->ogIntfcCb->ccTgCb->tgAtt.sipTPrflPtr->transcodePolicy; 								\
         }								\
      }									\
}

/* + FID 15083.0 */

/* bitMask vaules */
#define IcSipIriCfm       0x1     /* Incoming SIP/SIPT confirmation for IRI_REPORT      */
#define LiAddIc           0x2     /* Waiting for Li IC add rsp */
#define LiSub             0x4     /* Waiting for Li Sub        */
#define ManualInvokeSub   0x8     /* FID 15083.1 Set if manual invoked sub request for this call */ 
#define ManualInvokeAdd   0x10    /* FID 15083.1 Set if manual invoked add request for this call */ 
#define OgSipIriCfm       0x20    /* Outgoing SIP/SIPT confirmation for IRI_REPORT     */
#define LiAddOg           0x40    /* Waiting for Li OG Add Rsp */
#define ManualInvokeEnb   0x80    /* FID 15083.1 Set if manual invoked is enabled for this call  */ 
#define SwtchOnlyForLi    0x100   /* Switching done because of Li only */
#define NeedPopCriForLi   0x200   /* Need to Populate Cri for Li Bcm */
#define Li64KDataCall     0x400   /* FID 15083.1 Set if 64K Unrestricted ITU Data Call Detected  */ 
#define LiTermsAdded      0x800   /* FID 15083.1 Set if LI Terms Added in Context                */
#define IcSdpNew          0x1000  /* FID 15083.1 Set if Ic SDP has been updated                  */
#define OgSdpNew          0x2000  /* FID 15083.1 Set if Og SDP has been updated                  */
#define IcTarget1OpenSent 0x4000  /* FID 15083.1 & 15041.2 Set if Open sent on incoming target 1*/
#define IcTarget2OpenSent 0x8000  /* FID 15083.1 & 15041.2 Set if Open sent on incoming target 2*/
#define OgTargetOpenSent  0x10000 /* FID 15083.1 Set if Open sent on outgoing target             */
#define Sorm81Call        0x20000 /* BUG: 84166 HUG - Indicates an 8.1 replicated call           */
#define IcTarget3OpenSent 0x40000 /* FID 17002.0 */
/* - FID 15083.0 */

/* FID 2373 */
#define CC_MLPP_SD0     0x40024B
#define CC_MLPP_SD1     0x40024C
#define CC_MLPP_SD2     0x40024D
#define CC_MLPP_SD3     0x40024E
#define CC_MLPP_SD4     0x40024F
#define CC_LFB_NA	2		/* Look ahead for busy not allowed */

/* Bug# 62290 */

#define CC_IS_TDM_PROTOCOL(protType) ((protType == CC_SI) || (protType == CC_IN) || \
            (protType == CC_CS_TG) || (protType == CC_CS_LN) || (protType == CC_IVT))

/* FID 14556.0 + */
/*********************************************/
/* G726-XY translation from ITU I 366.2 to RFC 3551 format */
/*********************************************/
#define CONVT_G726XY_R3551_TO_I3662(srcCodec, srcPayload, msCodec)          \
   {                                                                        \
     if (srcPayload != SDP_PTYPE_INVALID)                                   \
     {                                                                      \
       switch(srcCodec)                                                     \
       {                                                                    \
         case SDP_CODEC_G726_16:                                            \
           (msCodec) = SDP_CODEC_AAL2_G726_16;                              \
           CCDP(AIN_DBG_LEVEL_2, "Map G726-16 to AAL2-G726-16\n");          \
           break;                                                           \
         case SDP_CODEC_G726_24:                                            \
           (msCodec) = SDP_CODEC_AAL2_G726_24;                              \
           CCDP(AIN_DBG_LEVEL_2, "Map G726-24 to AAL2-G726-24\n");          \
           break;                                                           \
         case SDP_CODEC_G726:                                               \
         case SDP_CODEC_G726_32:                                            \
           (msCodec) = SDP_CODEC_AAL2_G726_32;                              \
           CCDP(AIN_DBG_LEVEL_2, "Map G726 or G726-32 to AAL2-G726-32\n");  \
           break;                                                           \
         case SDP_CODEC_G726_40:                                            \
           (msCodec) = SDP_CODEC_AAL2_G726_40;                              \
           CCDP(AIN_DBG_LEVEL_2, "Map G726-40 to AAL2-G726-40 Codec\n");    \
           break;                                                           \
         default:                                                           \
           break;                                                           \
       }                                                                    \
     }                                                                      \
   }

/*********************************************/
/* G726-XY translation from ITU I 366.2 to RFC 3551 format */
/*********************************************/
#define CONVT_G726XY_I3662_TO_R3551(srcCodec, srcPayload, msCodec)          \
   {                                                                        \
     if (srcPayload != SDP_PTYPE_INVALID)                                   \
     {                                                                      \
       switch(srcCodec)                                                     \
       {                                                                    \
         case SDP_CODEC_AAL2_G726_16:                                       \
           (msCodec) = SDP_CODEC_G726_16;                                   \
           CCDP(AIN_DBG_LEVEL_2, "Map AAL2-G726-16 to G726-16\n");          \
           break;                                                           \
         case SDP_CODEC_AAL2_G726_24:                                       \
           (msCodec) = SDP_CODEC_G726_24;                                   \
           CCDP(AIN_DBG_LEVEL_2, "Map AAL2-G726-24 to G726-24\n");          \
           break;                                                           \
         case SDP_CODEC_AAL2_G726_32:                                       \
           (msCodec) = SDP_CODEC_G726_32;                                   \
           CCDP(AIN_DBG_LEVEL_2, "Map AAL2-G726-32 to G726-32\n");          \
           break;                                                           \
         case SDP_CODEC_AAL2_G726_40:                                       \
           (msCodec) = SDP_CODEC_G726_40;                                   \
           CCDP(AIN_DBG_LEVEL_2, "Map AAL2-G726-40 to G726-40\n");          \
           break;                                                           \
         default:                                                           \
           break;                                                           \
       }                                                                    \
     }                                                                      \
   }

/* FID 14556.0 - */
/* FID 14732 */

  #define CC_ALLOCATE_SIPSIPTTUNNEL_INFO(_con) \
  {\
    S16 ret = ROK;\
    if (_con->sipSiptTunnelInfo == NULL)\
    {\
       /* Alloccate the memory */\
      ret = SGetSBuf(ccInit.region, ccInit.pool, (Data **)&(_con->sipSiptTunnelInfo),\
                   (Size) (sizeof(CcSipSiptTunnelInfo)));\
      if (ret == RFAILED)\
      {\
         DP("Memory allocation failed for con->sipSiptTunnelInfo\n");\
         RETVALUE(RFAILED);\
      }\
      cmMemset((U8 *)_con->sipSiptTunnelInfo, 0, sizeof(CcSipSiptTunnelInfo));  \
      CCDP(AIN_DBG_LEVEL_0,"%s() Allocated con->sipSiptTunnelInfo = 0x%lx \n", __FUNCTION__, (U32)con->sipSiptTunnelInfo);\
    }\
  }

#define IS_IT_INVITE_SIPMBUF(_sipMBufInfo) ((_sipMBufInfo.direction == INCTOOUT) && (_sipMBufInfo.evntType == CCE_CONIND) && (_sipMBufInfo.sipMBuf))

#define IS_IT_SIPT_OR_BICC_IAM_SIPMBUF IS_IT_INVITE_SIPMBUF

#define IS_IT_18X_SIPMBUF(_sipMBufInfo) ((_sipMBufInfo.direction == OUTTOINC) && (_sipMBufInfo.evntType == CCE_CNSTIND) && \
            ((_sipMBufInfo.subEvntType == CCSIP_ET_180_RINGING) || (_sipMBufInfo.subEvntType == CCSIP_ET_181_FORWDNG) || \
             (_sipMBufInfo.subEvntType == CCSIP_ET_182_QUEUED) || (_sipMBufInfo.subEvntType == CCSIP_ET_183_CALLPRG)) && (_sipMBufInfo.sipMBuf))

#define IS_IT_SIPT_OR_BICC_ACM_SIPMBUF(_sipMBufInfo) ((_sipMBufInfo.direction == OUTTOINC) && (_sipMBufInfo.evntType == CCE_CNSTIND) && \
            (_sipMBufInfo.subEvntType == CC_ET_ACM) && (_sipMBufInfo.sipMBuf))

#define IS_IT_SIPT_OR_BICC_CPG_SIPMBUF(_sipMBufInfo) ((_sipMBufInfo.direction == OUTTOINC) && (_sipMBufInfo.evntType == CCE_CNSTIND) && \
            (_sipMBufInfo.subEvntType == CC_ET_CPG) && (_sipMBufInfo.sipMBuf))

#define IS_IT_200OK_SIPMBUF(_sipMBufInfo) ((_sipMBufInfo.direction == OUTTOINC) && (_sipMBufInfo.evntType == CCE_CONCFM) && (_sipMBufInfo.sipMBuf))

#define IS_IT_SIPT_OR_BICC_ANM_SIPMBUF IS_IT_200OK_SIPMBUF

#define IS_IT_BYE_SIPMBUF(_sipMBufInfo, _direction) ((_sipMBufInfo.direction ==_direction) && (_sipMBufInfo.evntType == CCE_RELIND) && (_sipMBufInfo.sipMBuf))

#define IS_IT_SIPT_OR_BICC_REL_SIPMBUF IS_IT_BYE_SIPMBUF

#define IS_SIPT_BICC_MISC_CNSTIND_SIPMBUF(_sipMBufInfo, _direction, _subEvntType) ((_sipMBufInfo.direction == _direction) && (_sipMBufInfo.evntType == CCE_CNSTIND) && (_sipMBufInfo.subEvntType == _subEvntType) && (_sipMBufInfo.sipMBuf))

#define IS_IT_XFER_SIPMBUF(_sipMBufInfo,_direction) ((_sipMBufInfo.direction ==_direction) && (_sipMBufInfo.evntType == CCE_CNSTIND) && \
              (_sipMBufInfo.subEvntType == CCSIP_ET_XFER) && (_sipMBufInfo.sipMBuf))

#define IS_IT_XFERRSP_SIPMBUF(_sipMBufInfo,_direction) ((_sipMBufInfo.direction ==_direction) && (_sipMBufInfo.evntType == CCE_CNSTIND) && \
              (_sipMBufInfo.subEvntType == CCSIP_ET_XFER_RSP) && (_sipMBufInfo.sipMBuf))

#define IS_IT_HOLD_SIPMBUF(_sipMBufInfo,_direction) ((_sipMBufInfo.direction ==_direction) && (_sipMBufInfo.evntType == CCE_CNSTIND) && \
              (_sipMBufInfo.subEvntType == CCSIP_ET_HOLD) && (_sipMBufInfo.sipMBuf))

#define IS_IT_HOLDRSP_SIPMBUF(_sipMBufInfo,_direction) ((_sipMBufInfo.direction ==_direction) && (_sipMBufInfo.evntType == CCE_CNSTIND) && \
              (_sipMBufInfo.subEvntType == CCSIP_ET_HOLD_RSP) && (_sipMBufInfo.sipMBuf))

#define IS_IT_QRY_SIPMBUF(_sipMBufInfo,_direction) ((_sipMBufInfo.direction ==_direction) && (_sipMBufInfo.evntType == CCE_CNSTIND) && \
              (_sipMBufInfo.subEvntType == CCSIP_ET_XFER_QRY) && (_sipMBufInfo.sipMBuf))

#define IS_IT_QRYRSP_SIPMBUF(_sipMBufInfo,_direction) ((_sipMBufInfo.direction ==_direction) && (_sipMBufInfo.evntType == CCE_CNSTIND) && \
              (_sipMBufInfo.subEvntType == CCSIP_ET_XFER_QRY_RSP) && (_sipMBufInfo.sipMBuf))

#define IS_IT_UPDATE_SIPMBUF(_sipMBufInfo,_direction) ((_sipMBufInfo.direction ==_direction) && (_sipMBufInfo.evntType == CCE_CNSTIND) && \
              (_sipMBufInfo.subEvntType == CCSIP_ET_UPDATE) && (_sipMBufInfo.sipMBuf))

#define IS_IT_UPDATERSP_SIPMBUF(_sipMBufInfo,_direction) ((_sipMBufInfo.direction ==_direction) && (_sipMBufInfo.evntType == CCE_CNSTIND) && \
              (_sipMBufInfo.subEvntType == CCSIP_ET_UPDATE_RSP) && (_sipMBufInfo.sipMBuf))

#define IS_IT_ACK_SIPMBUF(_sipMBufInfo,_direction) ((_sipMBufInfo.direction ==_direction) && (_sipMBufInfo.evntType == CCE_CNSTIND) && \
              (_sipMBufInfo.subEvntType == CCSIP_ET_ACK) && (_sipMBufInfo.sipMBuf))

#define IS_IT_PRACK_SIPMBUF(_sipMBufInfo,_direction) ((_sipMBufInfo.direction ==_direction) && (_sipMBufInfo.evntType == CCE_CNSTIND) && \
              (_sipMBufInfo.subEvntType == CCSIP_ET_PRACK) && (_sipMBufInfo.sipMBuf))

#define IS_IT_PRACKRSP_SIPMBUF(_sipMBufInfo,_direction) ((_sipMBufInfo.direction ==_direction) && (_sipMBufInfo.evntType == CCE_CNSTIND) && \
              (_sipMBufInfo.subEvntType == CCSIP_ET_PRACK_RSP) && (_sipMBufInfo.sipMBuf))

#define CC_GET_INGRESS_CCS_GENERIC_ICPROTTYPE(con, _icProtType) \
{\
   if (con->icIntfcCb && (CC_INGRESS_INTER_CCS_CALL(con)))\
   {\
      _icProtType = CC_GET_PROTOCOL(con->icIntfcCb->protType); \
      CCDP(AIN_DBG_LEVEL_1,"%s(), INGRESS_CCS_GENERIC_ICPROTTYPE=%d\n", __FUNCTION__, _icProtType);\
   }\
   else\
   {\
      if ((con->icIntfcCb == NULL) && ((CC_INGRESS_INTER_CCS_CALL(con))))\
      {\
        _icProtType = CC_GET_PROTOCOL(con->realLegProtType);\
       CCDP(AIN_DBG_LEVEL_1,"%s() con->ogIntfcCb is NULL, con->realLegProtType=%d\n", __FUNCTION__, con->realLegProtType);\
      }\
      else\
      {\
        _icProtType = 0;\
      }\
   }\
}

#define CC_GET_INGRESS_CCS_VAR_ICPROTTYPE(con, _icProtType) \
{\
   if (con->icIntfcCb && (CC_INGRESS_INTER_CCS_CALL(con)))\
   {\
      _icProtType = con->icIntfcCb->protType; \
      CCDP(AIN_DBG_LEVEL_1,"%s(), INGRESS_CCS_VAR_ICPROTTYPE=%d\n", __FUNCTION__, _icProtType);\
   }\
   else\
   {\
      if ((con->icIntfcCb == NULL) && ((CC_INGRESS_INTER_CCS_CALL(con))))\
      {\
        _icProtType = con->realLegProtType;\
       CCDP(AIN_DBG_LEVEL_1,"%s() con->ogIntfcCb is NULL, con->realLegProtType=%d\n", __FUNCTION__, con->realLegProtType);\
      }\
      else\
      {\
        _icProtType = 0;\
      }\
   }\
}

#define CC_GET_EGRESS_CCS_GENERIC_OGPROTTYPE(con, _ogProtType) \
{\
   if (con->ogIntfcCb && (CC_EGRESS_INTER_CCS_CALL(con)))\
   {\
      _ogProtType = CC_GET_PROTOCOL(con->ogIntfcCb->protType); \
      CCDP(AIN_DBG_LEVEL_1,"%s(), EGRESS_CCS_GENERIC_OGPROTTYPE=%d\n", __FUNCTION__, _ogProtType);\
   }\
   else\
   {\
      if ((con->ogIntfcCb == NULL) && ((CC_EGRESS_INTER_CCS_CALL(con))))\
      {\
        _ogProtType = CC_GET_PROTOCOL(con->realLegProtType);\
       CCDP(AIN_DBG_LEVEL_1,"%s() con->ogIntfcCb is NULL, con->realLegProtType=%d\n", __FUNCTION__, con->realLegProtType);\
      }\
      else\
      {\
        _ogProtType = 0;\
      }\
   }\
}

#define CC_GET_EGRESS_CCS_VAR_OGPROTTYPE(con, _ogProtType) \
{\
   if (con->ogIntfcCb && (CC_EGRESS_INTER_CCS_CALL(con)))\
   {\
     _ogProtType = con->ogIntfcCb->protType; \
      CCDP(AIN_DBG_LEVEL_1,"%s(), EGRESS_CCS_VAR_OGPROTTYPE=%d\n", __FUNCTION__, _ogProtType);\
   }\
   else\
   {\
      if ((con->ogIntfcCb == NULL) && ((CC_EGRESS_INTER_CCS_CALL(con))))\
      {\
        _ogProtType = con->realLegProtType;\
       CCDP(AIN_DBG_LEVEL_1,"%s() con->ogIntfcCb is NULL, con->realLegProtType=%d\n", __FUNCTION__, con->realLegProtType);\
      }\
      else\
      {\
        _ogProtType = 0;\
      }\
   }\
}


#define CC_FREE_SIPMBUF(_sipMBufInfo)\
{\
   MsgLen    length = 0;\
\
   if (_sipMBufInfo.sipMBuf)\
   {\
     if (SFndLenMsg ((Buffer *)_sipMBufInfo.sipMBuf, &length) != ROK)\
     {\
       DP("%s():: SFndLenMsg failed for sipMBuf\n", __FUNCTION__);\
     }\
\
     CCDP(AIN_DBG_LEVEL_0,\
        "%s():: Releasing sipMBuf=0x%lx of Size=%d, direction=%d, "\
        "evntType=%d, subEvntType=%d\n", __FUNCTION__, (U32)_sipMBufInfo.sipMBuf,\
        length, _sipMBufInfo.direction,\
        _sipMBufInfo.evntType, _sipMBufInfo.subEvntType);\
\
     SPutMsg(_sipMBufInfo.sipMBuf);\
     cmMemset((U8*)&_sipMBufInfo, 0, sizeof(CcSipSiptMBufInfo));\
   }\
}

#define IS_IT_INTER_CCS_CALL(_con) (CC_EGRESS_INTER_CCS_CALL(_con) || CC_INGRESS_INTER_CCS_CALL(_con))

#define IS_IT_INTRA_CCS_CALL(_con) (!(CC_EGRESS_INTER_CCS_CALL(_con) || CC_INGRESS_INTER_CCS_CALL(_con)))

#define IS_IT_TOWARDS_BICC(_con) (((_con->direction == INCTOOUT) && (CC_GET_PROTOCOL(_con->ogProtType) == CC_BICC))\
                               || ((_con->direction == OUTTOINC) && (CC_GET_PROTOCOL(_con->icProtType) == CC_BICC)))

/* FID 14732 */


/* + FID 14732.0 */
#define CC_IS_MG_ADVANCE(_con)                                                 \
(                                                                              \
    (_con) && (_con->routeEvnt) && (_con->routeEvnt->currentInterface>= 0) &&  \
    (_con->routeEvnt->cacInfo[CC_EGRESS_INTER_CCS_CALL(con)?                   \
                         _con->routeEvnt->currentInterface:                    \
                     (_con->routeEvnt->currentInterface -1)].mgwAdvance)       \
)

#define CC_IS_MG_ADVANCE_COND_MET(_failCdn)                                    \
(                                                                              \
    (_failCdn == FC_NO_DSP_RSC) ||                                             \
    (_failCdn == FC_BANDWITH_LIMITED) ||                                       \
    (_failCdn == FC_MG_NOT_READY)     ||                                       \
    (_failCdn == FC_RESOURCE_LIMITED) ||   /* FID 14457.0 Err 412 */           \
    (_failCdn == FC_MG_OOS)  /* BUG: 71477 */                                  \
)

#define CC_IS_MAX_MG_REROUTE(_con)                                             \
(                                                                              \
    (_con) && (_con->routeEvnt) && (_con->routeEvnt->currentInterface >= 0) && \
    (_con->routeEvnt->cacInfo[CC_EGRESS_INTER_CCS_CALL(con)?                   \
     _con->routeEvnt->currentInterface:(_con->routeEvnt->currentInterface -1)]. \
                      mgwCount >= 1) /* Bug# 72702 :: max reroute is just 2 */ \
)

#define CC_IS_MGW_AVAIL_MG_LIST(_con)                                          \
(                                                                              \
    (_con) && (_con->routeEvnt) && (_con->routeEvnt->currentInterface >= 0) && \
    (_con->routeEvnt->cacInfo[CC_EGRESS_INTER_CCS_CALL(_con)?                  \
                              _con->routeEvnt->currentInterface:               \
                              (_con->routeEvnt->currentInterface -1)].         \
            currentMgw + 1 < (_con->routeEvnt->cacInfo[                        \
                                   CC_EGRESS_INTER_CCS_CALL(con)?              \
                                   con->routeEvnt->currentInterface:           \
                                   (con->routeEvnt->currentInterface -1)].     \
                             numMgwInfo)) /* */                                \
)

#define CC_POPULATE_REAL_LEG_PROTTYPE(_con)\
{\
  if (con->icIntfcCb && CC_INGRESS_INTER_CCS_CALL(_con))\
  {\
    _con->realLegProtType = con->icIntfcCb->protType;\
  }\
  else if (con->ogIntfcCb && CC_EGRESS_INTER_CCS_CALL(_con))\
  {\
    _con->realLegProtType = con->ogIntfcCb->protType;\
  }\
}

/* - FID 14732.0 */

/* + FID 14457.0 GETS */
/* Ingress MG Advance is new with FID 14457.0, but it is general, not limited to GETS/HPC calls*/
#define CC_IS_ICMG_ADVANCE(_con)                                               \
(                                                                              \
    (CC_IS_ICMG_LIST_POPULATED(_con)) &&                                       \
    ((_con)->ccmCb.ingMgAdvance)                                               \
)

#define CC_IS_MAX_ICMG_REROUTE(_con)                                           \
(                                                                              \
    (_con) && (_con->routeEvnt) && (_con->routeEvnt->currentInterface >= 0) && \
    (_con->ccmCb.ingMgCount >= 1)               /* max reroute is just 2 */    \
)

#define CC_IS_MAX_HPC_MG_RETRIES(_con)                                         \
(                                                                              \
    (ccCp.mgRetryAttempts == 0 ) || (ccCp.mgRetryTmr == 0 ) ||                 \
    ((_con) && (_con->ccmCb.hpcMgRetries >= ccCp.mgRetryAttempts))             \
)
/* - FID 14457.0 GETS */

/* FID 14840 AoC */
/* FID 16706 AOC XML - Rename AOC to SCI XML */
#define CC_INGRESS_TG_SUPPORT_SCI_XML(con) \
(                                      \
    ((con->icIntfcCb != NULLP) &&      \
     (con->icIntfcCb->ccTgCb != NULLP) && \
     (con->icIntfcCb->intfc.intfType != CC_IN_INTFC) && \
     (con->icIntfcCb->ccTgCb->tgAtt.aoc) && \
    ((con->icIntfcCb->ccTgCb->tgAtt.SignalingType != TGP_SIGTYPE_SIP) || \
    ((con->icIntfcCb->ccTgCb->tgAtt.SignalingType == TGP_SIGTYPE_SIP) && \
     (con->icIntfcCb->ccTgCb->tgAtt.sipPrflPtr != NULL) && \
     (con->icIntfcCb->ccTgCb->tgAtt.sipPrflPtr->sciXML))) \
    )?TRUE:FALSE                         \
)

/* FID 16706 BUG 91702 */
#define CC_EGRESS_TG_SUPPORT_SCI_XML(con) \
(                                      \
    ((con->ogIntfcCb != NULLP) &&      \
     (con->ogIntfcCb->ccTgCb != NULLP) &&    \
     (con->ogIntfcCb->ccTgCb->tgAtt.aoc) &&   \
     (con->ogIntfcCb->intfc.intfType != CC_IN_INTFC) && \
    ((con->ogIntfcCb->ccTgCb->tgAtt.SignalingType != TGP_SIGTYPE_SIP) || \
    ((con->ogIntfcCb->ccTgCb->tgAtt.SignalingType == TGP_SIGTYPE_SIP) && \
     (con->ogIntfcCb->ccTgCb->tgAtt.sipPrflPtr != NULL) && \
     (con->ogIntfcCb->ccTgCb->tgAtt.sipPrflPtr->sciXML))) \
    )?TRUE:FALSE                            \
)

/* FID 16706 BUG 91702 */
#define CC_BOTH_TG_SUPPORT_SCI_XML(con)    \
(                                          \
    ((CC_INGRESS_TG_SUPPORT_SCI_XML(con)) && \
     (CC_EGRESS_TG_SUPPORT_SCI_XML(con))     \
    )?TRUE:FALSE                           \
)


/* FID 15318 IOI */                          
#define CC_INGRESS_TG_SUPPORT_IOI(con) \
(                                      \
    ((con->icIntfcCb) &&      \
     (con->icIntfcCb->ccTgCb) && \
     (con->icIntfcCb->ccTgCb->tgAtt.ioi.pres) \
    )?TRUE:FALSE                         \
)                                              
                                               
/* FID 15318 IOI */                            
#define CC_EGRESS_TG_SUPPORT_IOI(con) \
(                                      \
    ((con->ogIntfcCb) &&      \
     (con->ogIntfcCb->ccTgCb) && \
     (con->ogIntfcCb->ccTgCb->tgAtt.ioi.pres) \
    )?TRUE:FALSE                         \
) 

#define CC_SET_AOCINFO_TO(con, aocinfo) \
{\
    if (!CC_INGRESS_INTER_CCS_CALL(con)) \
    { \
       if (CC_GET_PROTOCOL(con->icProtType) == CC_SIPT) \
       { \
           aocinfo = &(con->ogEvnt->m.ccCnStEvnt.m.siCnStEvnt.optElmnt.sipt.aocInfo); \
       } \
       else \
       { \
           aocinfo = &(con->ogEvnt->m.ccCnStEvnt.m.siCnStEvnt.optElmnt.isup.aocInfo); \
       } \
    } \
    else \
    { \
       aocinfo = &(con->ogEvnt->m.ccCnStEvnt.m.siCnStEvnt.optElmnt.bicc.aocInfo); \
    } \
}

#define CC_SET_AOCINFO_FROM(con, aocinfo) \
{\
    if (!CC_EGRESS_INTER_CCS_CALL(con)) \
    { \
       if (CC_GET_PROTOCOL(con->ogProtType) == CC_SIPT) \
       { \
           aocinfo = &(con->icEvnt->m.ccCnStEvnt.m.siCnStEvnt.optElmnt.sipt.aocInfo); \
       } \
       else \
       { \
           aocinfo = &(con->icEvnt->m.ccCnStEvnt.m.siCnStEvnt.optElmnt.isup.aocInfo); \
       } \
    } \
    else \
    { \
       aocinfo = &(con->icEvnt->m.ccCnStEvnt.m.siCnStEvnt.optElmnt.bicc.aocInfo); \
    } \
}

#define CC_ALLOC_SIPXMLINFO(con) \
{\
  if (con != NULLP) \
  {\
    if (con->sipXmlInfo == NULLP) \
    {\
      if (SGetSBuf(ccInit.region, ccInit.pool, \
          (Data **) &(con->sipXmlInfo), \
          (Size) sizeof(SipXmlInfo)) != ROK) \
      {\
         CCDP(AIN_DBG_LEVEL_2, "FID 14840 Failed to allocate memory for con->sipXmlInfo \n"); \
         RETVALUE(RFAILED); \
      }\
      /* Bug 83184 */ \
      cmMemset((U8 *)con->sipXmlInfo, 0, sizeof(SipXmlInfo));\
    }\
    else if (con->sipXmlInfo->xmlBody.val != NULLP) \
    {\
        cmFreeTknStrVar(&(con->sipXmlInfo->xmlBody)); \
        CCDP(AIN_DBG_LEVEL_2, "FID 14840 con->sipXmlInfo->xmlBody.val exist %s, free it \n",__FUNCTION__); \
    } \
  }\
}

/* XML is case sensitive becase the cost of case monocasing
 * is huge and impact the performace too much
 * define macros for the AOC elements
 */
#define CC_XML_AOC_EMPTY ""
#define CC_XML_AOC_ADDONCHARGE "addOnCharge"
#define CC_XML_AOC_ADDONCHARGECURRENCY "addOnChargeCurrency"
#define CC_XML_AOC_ADDONCHARGEPULSE "addOnChargePulse"
#define CC_XML_AOC_AOCRG "aocrg"
#define CC_XML_AOC_CALLATTEMPTCHARGECURRENCY "callAttemptChargeCurrency"
#define CC_XML_AOC_CALLATTEMPTCHARGEPULSE "callAttemptChargePulse"
#define CC_XML_AOC_CALLSETUPCHARGECURRENCY "callSetupChargeCurrency"
#define CC_XML_AOC_CALLSETUPCHARGEPULSE "callSetupChargePulse"
#define CC_XML_AOC_CHARGEUNITTIMEINTERVAL "chargeUnitTimeInterval"
#define CC_XML_AOC_CHARGINGCONTROLINDICATORS "chargingControlIndicators"
#define CC_XML_AOC_CHARGINGTARIFF "chargingTariff"
#define CC_XML_AOC_COMMUNICATIONCHARGESEQUENCECURRENCY "communicationChargeSequenceCurrency"
#define CC_XML_AOC_COMMUNICATIONCHARGESEQUENCEPULSE "communicationChargeSequencePulse"
#define CC_XML_AOC_CRGT "crgt"
#define CC_XML_AOC_CURRENCY "currency"
#define CC_XML_AOC_CURRENCYFACTOR "currencyFactor"
#define CC_XML_AOC_CURRENCYFACTORSCALE "currencyFactorScale"
#define CC_XML_AOC_CURRENCYSCALE "currencyScale"
#define CC_XML_AOC_CURRENTTARIFFCURRENCY "currentTariffCurrency"
#define CC_XML_AOC_CURRENTTARIFFPULSE "currentTariffPulse"
#define CC_XML_AOC_DELAYUNTILSTART "delayUntilStart"
#define CC_XML_AOC_DESTINATIONIDENTIFICATION "destinationIdentification"
#define CC_XML_AOC_IMMEDIATECHANGEOFACTUALLYAPPLIEDTARIFF "immediateChangeOfActuallyAppliedTariff"
#define CC_XML_AOC_MESSAGETYPE "messageType"
#define CC_XML_AOC_NETWORKIDENTIFICATION "networkIdentification"
#define CC_XML_AOC_NEXTTARIFFCURRENCY "nextTariffCurrency"
#define CC_XML_AOC_NEXTTARIFFPULSE "nextTariffPulse"
#define CC_XML_AOC_ORIGINATIONIDENTIFICATION "originationIdentification"
#define CC_XML_AOC_PULSEUNITS "pulseUnits"
#define CC_XML_AOC_REFERENCEID "referenceID"
#define CC_XML_AOC_SUBTARIFFCONTROL "subTariffControl"
#define CC_XML_AOC_TARIFFCONTROLINDICATORS "tariffControlIndicators"
#define CC_XML_AOC_TARIFFCURRENCY "tariffCurrency"
#define CC_XML_AOC_TARIFFDURATION "tariffDuration"
#define CC_XML_AOC_TARIFFPULSE "tariffPulse"
#define CC_XML_AOC_TARIFFSWITCHCURRENCY "tariffSwitchCurrency"
#define CC_XML_AOC_TARIFFSWITCHOVERTIME "tariffSwitchOverTime"
#define CC_XML_AOC_TARIFFSWITCHPULSE "tariffSwitchPulse"

/* all the element type would coreponding with a element ID as defined in the enum
 */
#define CC_AOC_EM_UNUSE 0
#define CC_AOC_ADDONCHARGE 1
#define CC_AOC_ADDONCHARGECURRENCY 2
#define CC_AOC_ADDONCHARGEPULSE 3
#define CC_AOC_AOCRG 4
#define CC_AOC_CALLATTEMPTCHARGECURRENCY 5
#define CC_AOC_CALLATTEMPTCHARGEPULSE 6
#define CC_AOC_CALLSETUPCHARGECURRENCY 7
#define CC_AOC_CALLSETUPCHARGEPULSE 8
#define CC_AOC_CHARGEUNITTIMEINTERVAL 9
#define CC_AOC_CHARGINGCONTROLINDICATORS 10
#define CC_AOC_CHARGINGTARIFF 11
#define CC_AOC_COMMUNICATIONCHARGESEQUENCECURRENCY 12
#define CC_AOC_COMMUNICATIONCHARGESEQUENCEPULSE 13
#define CC_AOC_CRGT 14
#define CC_AOC_CURRENCY 15
#define CC_AOC_CURRENCYFACTOR 16
#define CC_AOC_CURRENCYFACTORSCALE 17
#define CC_AOC_CURRENCYSCALE 18
#define CC_AOC_CURRENTTARIFFCURRENCY 19
#define CC_AOC_CURRENTTARIFFPULSE 20
#define CC_AOC_DELAYUNTILSTART 21
#define CC_AOC_DESTINATIONIDENTIFICATION 22
#define CC_AOC_IMMEDIATECHANGEOFACTUALLYAPPLIEDTARIFF 23
#define CC_AOC_MESSAGETYPE 24
#define CC_AOC_NETWORKIDENTIFICATION 25
#define CC_AOC_NEXTTARIFFCURRENCY 26
#define CC_AOC_NEXTTARIFFPULSE 27
#define CC_AOC_ORIGINATIONIDENTIFICATION 28
#define CC_AOC_PULSEUNITS 29
#define CC_AOC_REFERENCEID 30
#define CC_AOC_SUBTARIFFCONTROL 31
#define CC_AOC_TARIFFCONTROLINDICATORS 32
#define CC_AOC_TARIFFCURRENCY 33
#define CC_AOC_TARIFFDURATION 34
#define CC_AOC_TARIFFPULSE 35
#define CC_AOC_TARIFFSWITCHCURRENCY 36
#define CC_AOC_TARIFFSWITCHOVERTIME 37
#define CC_AOC_TARIFFSWITCHPULSE 38

#define CC_AOC_MAXAOCID 39
#define CC_AOC_MAXVALLEN 64
#define CC_AOC_MAXXMLLEN 4096
#define CC_AOC_MAXCURRENCYID 28

/* Tarrif currency enums */
#define CC_AOC_NOINDICATION        0x00
#define CC_AOC_AUSTRALIANDOLLAR    0x01
#define CC_AOC_AUSTRIANSCHILLING   0x02
#define CC_AOC_BELGIANFRANC        0x03
#define CC_AOC_BRITISHPOUND        0x04
#define CC_AOC_CZECHKORUNA         0x05
#define CC_AOC_DANISHKRONE         0x06
#define CC_AOC_DUTCHGUILDER        0x07
#define CC_AOC_EURO                0x08
#define CC_AOC_FINNISHMARKKA       0x09
#define CC_AOC_FRENCHFRANC         0x0a
#define CC_AOC_GERMANMARK          0x0b
#define CC_AOC_GREEKDRACHMA        0x0c
#define CC_AOC_HUNGARIANFORINT     0x0d
#define CC_AOC_IRISHPUNT           0x0e
#define CC_AOC_ITALIANLIRA         0x0f
#define CC_AOC_JAPANESEYEN         0x10
#define CC_AOC_LUXEMBOURGIANFRANC  0x11
#define CC_AOC_NORWEGIANKRONE      0x12
#define CC_AOC_POLISHZLOTY         0x13
#define CC_AOC_PORTUGESEESCUDO     0x14
#define CC_AOC_RUSSIANROUBLE       0x15
#define CC_AOC_SLOVAKKORUNA        0x16
#define CC_AOC_SPANISHPESETA       0x17
#define CC_AOC_SWEDISHKRONE        0x18
#define CC_AOC_SWISSFRANC          0x19
#define CC_AOC_TURKISHLIRA         0x1a
#define CC_AOC_USDOLLAR            0x1b

/* Tarrif currency symbols list */
#define CC_NOINDICATION        ""
#define CC_AUSTRALIANDOLLAR    "AUD"
#define CC_AUSTRIANSCHILLING   "ATS"
#define CC_BELGIANFRANC        "BEF"
#define CC_BRITISHPOUND        "GBP"
#define CC_CZECHKORUNA         "CZK"
#define CC_DANISHKRONE         "DKK"
#define CC_DUTCHGUILDER        "NLG"
#define CC_EURO                "EUR"
#define CC_FINNISHMARKKA       "FIM"
#define CC_FRENCHFRANC         "FRF"
#define CC_GERMANMARK          "DEM"
#define CC_GREEKDRACHMA        "GRD"
#define CC_HUNGARIANFORINT     "HUF"
#define CC_IRISHPUNT           "IEP"
#define CC_ITALIANLIRA         "ITL"
#define CC_JAPANESEYEN         "JPY"
#define CC_LUXEMBOURGIANFRANC  "LUF"
#define CC_NORWEGIANKRONE      "NOK"
#define CC_POLISHZLOTY         "PLN"
#define CC_PORTUGESEESCUDO     "PTE"
#define CC_RUSSIANROUBLE       "RUB"
#define CC_SLOVAKKORUNA        "SKK"
#define CC_SPANISHPESETA       "ESP"
#define CC_SWEDISHKRONE        "SEK"
#define CC_SWISSFRANC          "CHF"
#define CC_TURKISHLIRA         "TRY"
#define CC_USDOLLAR            "USD"

/* + FID 14811.0 +
 * The following max length is based on the real need.
 * To encode PSDN from ISDN to SIP, the buffer can contain at most one BC, one HLC, 
 * one LLC and five PIs, and each para has its own max length as following:
 * the max BC length is 1480
 * the max HLC length is 463
 * the max LLC length is 1827
 * the max PI length is 215
 */
#define CC_PSTN_MAXXMLLEN      5120
#define CC_PSTN_MAXBCLEN       1536
#define CC_PSTN_MAXHLCLEN      512
#define CC_PSTN_MAXLLCLEN      2048
#define CC_PSTN_MAXPILEN       256

#define CC_BOTH_SUPPORT_PSTN_XML(con)       \
     ccBothSupportPstnXml(con)

/* - FID 14811.0 */

/* +FID16706.0 */
/* SIP XML definitions 3GPP TS 24.647 */
#define CC_AOC_SIPXML_AOC                       "aoc"
#define CC_AOC_SIPXML_AOC_D                     "aoc-d"
#define CC_AOC_SIPXML_AOC_E                     "aoc-e"
#define CC_AOC_SIPXML_CHARGINGINFO              "charging-info"
#define CC_AOC_SIPXML_RECORDEDCHARGES           "recorded-charges"
#define CC_AOC_SIPXML_BILLING_ID                "billing-id"
#define CC_AOC_SIPXML_CURRENCY_ID               "currency-id"
#define CC_AOC_SIPXML_CURRENCY_AMOUNT           "currency-amount"
#define CC_AOC_SIPXML_RECORDED_CURRENCY_UNITS   "recorded-currency-units"
#define CC_AOC_SIPXML_FREE_CHARGE               "free-charge"
#define CC_AOC_SIPXML_NOT_AVAILABLE             "not-available"

/* Element IDs */
#define CC_AOC_AOCMSG                             1
#define CC_AOC_AOCTYPE_D                          2
#define CC_AOC_AOCTYPE_E                          3
#define CC_AOC_CHARGINGINFO                       4
#define CC_AOC_RECORDEDCHARGES                    5
#define CC_AOC_RECORDED_CURRENCY_UNITS            6
#define CC_AOC_RECORDEDCHARGES_CURRENCY_ID        8
#define CC_AOC_RECORDEDCHARGES_CURRENCY_AMOUNT    9
#define CC_AOC_RECORDEDCHARGES_FREECHARGE         10
#define CC_AOC_RECORDEDCHARGES_NOTAVAILABLE       11
#define CC_AOC_BILLING_ID                         12

/* defines for ChargingInfoType*/
#define AOC_SIPXML_CHARGINGINFOTYPE_TOTAL          "total"
#define AOC_SIPXML_CHARGINGINFOTYPE_SUBTOTAL       "subtotal"

/* defines for billingIdtype */
#define AOC_SIPXML_BILLINGIDTYPE_NORMAL_CHARGING   "normal-charging"
#define AOC_SIPXML_BILLINGIDTYPE_REVERSE_CHARGING  "reverse-charging"
#define AOC_SIPXML_BILLINGIDTYPE_CREDIT_CARD       "credit-card"      
#define AOC_SIPXML_BILLINGIDTYPE_CFU               "cfu"            
#define AOC_SIPXML_BILLINGIDTYPE_CFB               "cfb"            
#define AOC_SIPXML_BILLINGIDTYPE_CFNR              "cfnr"
#define AOC_SIPXML_BILLINGIDTYPE_CD                "cd"
#define AOC_SIPXML_BILLINGIDTYPE_CT                "ct"

/* Element Values */

/* charging-info-type */
#define CC_AOC_CHARGINGINFOTYPE_TOTAL             1
#define CC_AOC_CHARGINGINFOTYPE_SUBTOTAL          2

/*  billing-id */
#define CC_AOC_BILLINGIDTYPE_NORMAL_CHARGING      1
#define CC_AOC_BILLINGIDTYPE_REVERSE_CHARGING     2
#define CC_AOC_BILLINGIDTYPE_CREDIT_CARD          3
#define CC_AOC_BILLINGIDTYPE_CFU                  4
#define CC_AOC_BILLINGIDTYPE_CFB                  5
#define CC_AOC_BILLINGIDTYPE_CFNR                 6
#define CC_AOC_BILLINGIDTYPE_CD                   7
#define CC_AOC_BILLINGIDTYPE_CT                   8

/* max limits for currencyAmount */
#define MAX_AOC_CURRENCY_AMOUNT         (16777215)
#define MAX_AOC_CURRENCY_AMT_LEN        63  /* TknStr64 */


/* defines used in processing currency amount */
#define AOC_LEN_OF_MAXAMOUNT        8  /* length of 16777215 */
#define AOC_MAX_BUFFER_LEN         40
#define AOC_MAX_DGT_SHIFT           3 
#define AOC_DIGITSHIFT_DIRN_LEFT    1  
#define AOC_DIGITSHIFT_DIRN_RIGHT   2 
#define AOC_INITSTR(x) memset(x,0,AOC_MAX_BUFFER_LEN); 

typedef unsigned long AocAmnt;
typedef unsigned int AocMultiplier;

#define AOC_MIN(a,b)    ((a) < (b))?(a):(b);

/* -FID16706.0 */

#define CC_0_9_TO_HEX_HI(src, dst, i) \
{\
    /* for '0'(48)-'9'(57) */\
    *(dst + i) = (((*(src + i*2) - 48) << 4) & 0xf0);\
}

#define CC_0_9_TO_HEX_LO(src, dst, i) \
{\
    /* for 0-9 */\
    *(dst + i) |= ((*(src + i*2 + 1) - 48) & 0x0f);\
}

#define CC_A_F_TO_HEX_HI(src, dst, i) \
{\
    /* for 'A'-'F', 'A'=65 */\
    *(dst + i) = (((*(src + i*2) - 55) << 4) & 0xf0);\
}

#define CC_A_F_TO_HEX_LO(src, dst, i) \
{\
    /* for 'A'-'F', 'A'=65 */\
    *(dst + i) |= ((*(src + i*2 + 1) - 55) & 0x0f);\
}

#define CC_a_f_TO_HEX_HI(src, dst, i) \
{\
    /* for 'a'-'f''a'=97  */\
    *(dst + i) = (((*(src + i*2) - 87) << 4) & 0xf0);\
}

#define CC_a_f_TO_HEX_LO(src, dst, i) \
{\
    /* for 'a'-'f''a'=97  */\
    *(dst + i) |= ((*(src + i*2 + 1) - 87) & 0x0f);\
}

#define CC_IS_EGRESS_SIPSIPT(con, egressCCS_OgProtType) \
( \
    ((CC_GET_PROTOCOL(con->ogProtType) == CC_SIPT) || \
     (egressCCS_OgProtType == CC_SIPT) || \
     (egressCCS_OgProtType == CC_SIP))?TRUE:FALSE \
)

#define CC_IS_INGRESS_SIPSIPT(con, ingressCCS_IcProtType) \
( \
    ((CC_GET_PROTOCOL(con->icProtType) == CC_SIPT) || \
     (ingressCCS_IcProtType == CC_SIPT) || \
     (ingressCCS_IcProtType == CC_SIP))?TRUE:FALSE \
)

/* 73287 make sure PRACK is sent to OG TG */
#define CC_SEND_PRACK_OG(con, prackSent)\
{\
   if (ccIsRcvdSdpReliable(con, con->ogProtType, CCE_CNSTIND, con->icEvntType, con->icEvnt) && !prackSent) \
   { \
      ccSendPrack(&con->ogBcm); \
      CCDP(AIN_DBG_LEVEL_0, "%s: sent PRACK before CCIGNOREMAPPING \n", __FUNCTION__); \
   } \
}

/* 73287 make sure PRACK is sent to IC TG */
#define CC_SEND_PRACK_IC(con, prackSent)\
{\
  if (ccIsRcvdSdpReliable(con, con->icProtType, CCE_CNSTIND, con->icEvntType, con->icEvnt) && !prackSent) \
  { \
     ccSendPrack(&con->icBcm); \
     CCDP(AIN_DBG_LEVEL_0, "%s: sent PRACK before CCIGNOREMAPPING \n", __FUNCTION__); \
  } \
}

/* End of FID 14840 AoC */

/* FID 14844 - VBD changes */
#define CC_SET_VBD_NEGOTIATED(_conFlag, _vbdFlag) (_conFlag |= _vbdFlag)
#define CC_IS_VBD_NEGOTIATED(_conFlag, _vbdFlag) (_conFlag & _vbdFlag)
#define CC_RESET_VBD_NEGOTIATED(_conFlag, _vbdFlag) (_conFlag &= ~_vbdFlag)

/* FID 15913.0 - PSEUDO VBD */
#define CC_SET_PSEUDO_VBD(_conFlag, _pseudoVbdFlag) (_conFlag |= _pseudoVbdFlag)
#define CC_IS_PSEUDO_VBD(_conFlag, _pseudoVbdFlag) (_conFlag & _pseudoVbdFlag)
#define CC_RESET_PSEUDO_VBD(_conFlag, _pseudoVbdFlag) (_conFlag &= ~_pseudoVbdFlag)

/* End of FID 14844 changes */ 

/* FID 16259.0 */
#define CC_SET_CALL_FLAG2(_con, _flag) (_con->flag2 |= _flag)
#define CC_CLR_CALL_FLAG2(_con, _flag) (_con->flag2 &= ~_flag)
#define CC_IS_CALL_FLAG2(_con, _flag) (_con->flag2 & _flag)


#define CC_IS_PROT_TYPE_CASORGR303(OgProtType) \
( \
    ((CC_GET_PROTOCOL(OgProtType) == CC_CS_TG) || \
     (CC_GET_PROTOCOL(OgProtType) == CC_CS_LN) || \
     (CC_GET_PROTOCOL(OgProtType) == CC_GR303))?TRUE:FALSE \
)


/* + FID 14941.0 */

/* return different MultiMediaCb for different CCN */
/* Add more ?: expression chain if necessary */
#define MULTIMEDIACB(CON) \
(((CON)->ccmCb.ccn == CCN_1)?(MULTIMEDIACB_4_CCN1(CON)):((CON)->MultiMediaCb))

#define MULTIMEDIACB_4_CCN1(CON) \
(((CON)->ccmCb.state == CC_INGRESS_MG)?((CON)->ingressMultiMediaCb):((CON)->egressMultiMediaCb))

#define MULTIMEDIACB_4_BCM(BCM) \
(((BCM)->con->ccmCb.ccn == CCN_1)?(MULTIMEDIACB_4_BCM_CCN1(BCM)):((BCM)->con->MultiMediaCb))

#define MULTIMEDIACB_4_BCM_CCN1(BCM) \
((((BCM)->bcmType == BtOBcm)||((BCM)->bcmType == BtOAdjBcm))?((BCM)->con->ingressMultiMediaCb):((BCM)->con->egressMultiMediaCb))


/* return different MultiMediaCb for different CCN */
/* GCC returns error for &MULTIMEDIACB, thus add this macro */
#define PTR_MULTIMEDIACB(CON) \
(((CON)->ccmCb.ccn == CCN_1)?(PTR_MULTIMEDIACB_4_CCN1(CON)):(&((CON)->MultiMediaCb)))

#define PTR_MULTIMEDIACB_4_CCN1(CON) \
(((CON)->ccmCb.state == CC_INGRESS_MG)?(&((CON)->ingressMultiMediaCb)):(&((CON)->egressMultiMediaCb)))

#define LOCATE_MULTIMEDIACB_ADDR_4_BCM(BCM, PtrMultiMediaCb) \
{ \
  if ((BCM)->con->ccmCb.ccn == CCN_1) \
  { \
    if (((BCM)->bcmType == BtOBcm)||((BCM)->bcmType == BtOAdjBcm)) \
    { \
      PtrMultiMediaCb = & (BCM)->con->ingressMultiMediaCb; \
    } \
    else \
    { \
      PtrMultiMediaCb = & (BCM)->con->egressMultiMediaCb; \
    } \
  } \
  else \
  { \
    PtrMultiMediaCb = & (BCM)->con->MultiMediaCb; \
  } \
};

#define IS_MULTIMEDIA(CON) \
((CON)&&(MULTIMEDIACB(CON)))


#define IS_MULTIMEDIA_4_BCM(BCM) \
((BCM)&&(MULTIMEDIACB_4_BCM(BCM)))

#define IS_SINGLE_AWARE_STREAM_4_BCM(BCM) \
((BCM)&& \
 ((!MULTIMEDIACB_4_BCM(BCM))|| \
  ((MULTIMEDIACB_4_BCM(BCM))&& \
   (IS_MULTIMEDIA_WITH_MEDIA_AWARE_4_BCM(BCM))&& \
   ((MULTIMEDIACB_4_BCM(BCM)->StreamCount- \
     MULTIMEDIACB_4_BCM(BCM)->disabledStreamCount)==1))))

#define IS_1ST_SINGLE_AWARE_STREAM_4_BCM(BCM) \
((BCM)&& \
 ((!MULTIMEDIACB_4_BCM(BCM))|| \
  ((MULTIMEDIACB_4_BCM(BCM))&& \
   (IS_MULTIMEDIA_WITH_MEDIA_AWARE_4_BCM(BCM))&& \
   (MULTIMEDIACB_4_BCM(BCM)->MediaAwareStreamID == 1)&& \
   ((MULTIMEDIACB_4_BCM(BCM)->StreamCount- \
     MULTIMEDIACB_4_BCM(BCM)->disabledStreamCount)==1))))


#define CHECK_REPLACE_SDP(CON, rmtSdp) \
{ \
   if (IS_MULTIMEDIA_WITH_MEDIA_AWARE(CON)) \
   { \
     cmCopy1MStoNewSdp(ccInit.region, ccInit.pool, &MULTIMEDIACB(CON)->tempSdp, rmtSdp, \
                       MEDIA_AWARE_MS_ID(CON)-1); \
     rmtSdp = MULTIMEDIACB(CON)->tempSdp; \
   } \
}

#define CHECK_REPLACE_SDP_4_BCM(BCM, rmtSdp) \
{ \
   if (IS_MULTIMEDIA_WITH_MEDIA_AWARE_4_BCM(BCM)) \
   { \
     cmCopy1MStoNewSdp(ccInit.region, ccInit.pool, &MULTIMEDIACB_4_BCM(BCM)->tempSdp, rmtSdp, \
                       MEDIA_AWARE_MS_ID_4_BCM(BCM)-1); \
     rmtSdp = MULTIMEDIACB_4_BCM(BCM)->tempSdp; \
   } \
}

#define CONDENSE_MULTIMEDIACB(CON) \
{ \
  if ((CON)->MultiMediaCb) \
  { \
    if ((CON)->MultiMediaCb->tempSdp) \
    { \
      cmFreeSdp(ccInit.region, ccInit.pool, &(CON)->MultiMediaCb->tempSdp); \
    } \
  } \
  if ((CON)->ingressMultiMediaCb) \
  { \
    if ((CON)->ingressMultiMediaCb->tempSdp) \
    { \
      cmFreeSdp(ccInit.region, ccInit.pool, &(CON)->ingressMultiMediaCb->tempSdp); \
    } \
  } \
  if ((CON)->egressMultiMediaCb) \
  { \
    if ((CON)->egressMultiMediaCb->tempSdp) \
    { \
      cmFreeSdp(ccInit.region, ccInit.pool, &(CON)->egressMultiMediaCb->tempSdp); \
    } \
  } \
}

#define LOCATE_SDPOFFER(CON, rmtSdp) \
{ \
  if (((CON)->ccmCb.ccn == CCN_1)&&((CON)->ccmCb.state == CC_EGRESS_MG)) \
  { \
    rmtSdp = (CON)->icAdjIpParam->locDesc; \
  } \
  else \
  { \
    rmtSdp = (CON)->icIpParam.rmtDesc; \
  } \
}

#define LOCATE_RMTSDP_4_BCM(BCM, rmtSdp) \
{ \
  if ((BCM)->bcmType == BtOBcm) \
  { \
    rmtSdp  = &(BCM)->con->icIpParam.rmtDesc; \
  } \
  else if ((BCM)->bcmType == BtTBcm) \
  { \
    rmtSdp  = &(BCM)->con->ogIpParam.rmtDesc; \
  } \
  else if (((BCM)->bcmType == BtOAdjBcm)&&((BCM)->con->ogAdjIpParam)) \
  { \
    rmtSdp  = &(BCM)->con->ogAdjIpParam->locDesc; \
  } \
  else if (((BCM)->bcmType == BtTAdjBcm)&&((BCM)->con->icAdjIpParam)) \
  { \
    rmtSdp  = &(BCM)->con->icAdjIpParam->locDesc; \
  } \
}

#define LOCATE_LOCSDP_4_BCM(BCM, locSdp) \
{ \
  if ((BCM)->bcmType == BtOBcm) \
  { \
    locSdp  = &(BCM)->con->icIpParam.locDesc; \
  } \
  else if ((BCM)->bcmType == BtTBcm) \
  { \
    locSdp  = &(BCM)->con->ogIpParam.locDesc; \
  } \
  else if (((BCM)->bcmType == BtOAdjBcm)&&((BCM)->con->icAdjIpParam)) \
  { \
    locSdp  = &(BCM)->con->icAdjIpParam->locDesc; \
  } \
  else if (((BCM)->bcmType == BtTAdjBcm)&&((BCM)->con->ogAdjIpParam)) \
  { \
    locSdp  = &(BCM)->con->ogAdjIpParam->locDesc; \
  } \
}

#define LOCATE_MODE_4_BCM(BCM, MODE) \
{ \
  if (IS_MULTIMEDIA_4_BCM(BCM)) \
  { \
    if ((BCM)->con->ccmCb.ccn == CCN_1) \
    { \
      if ((BCM)->bcmType == BtOBcm) \
      { \
        MODE  = (BCM)->con->ingressMultiMediaCb->icMode; \
      } \
      else if ((BCM)->bcmType == BtTBcm) \
      { \
        MODE  = (BCM)->con->egressMultiMediaCb->ogMode; \
      } \
      else if ((BCM)->bcmType == BtOAdjBcm) \
      { \
        MODE  = (BCM)->con->ingressMultiMediaCb->ogMode; \
      } \
      else if ((BCM)->bcmType == BtTAdjBcm) \
      { \
        MODE  = (BCM)->con->egressMultiMediaCb->icMode; \
      } \
    } \
    else if ((BCM)->con->ccmCb.ccn == CCN_0) \
    { \
      if ((BCM)->bcmType == BtOBcm) \
      { \
        MODE  = (BCM)->con->MultiMediaCb->icMode; \
      } \
      else if ((BCM)->bcmType == BtTBcm) \
      { \
        MODE  = (BCM)->con->MultiMediaCb->ogMode; \
      } \
    } \
  } \
}

#define LOCATE_LOC_IPPORT_4_BCM(BCM, IPPort) \
{ \
  if (IS_MULTIMEDIA_4_BCM(BCM)) \
  { \
    if ((BCM)->con->ccmCb.ccn == CCN_1) \
    { \
      if ((BCM)->bcmType == BtOBcm) \
      { \
        IPPort  = (BCM)->con->ingressMultiMediaCb->icIpPort; \
      } \
      else if ((BCM)->bcmType == BtTBcm) \
      { \
        IPPort  = (BCM)->con->egressMultiMediaCb->ogIpPort; \
      } \
      else if ((BCM)->bcmType == BtOAdjBcm) \
      { \
        IPPort  = (BCM)->con->ingressMultiMediaCb->ogIpPort; \
      } \
      else if ((BCM)->bcmType == BtTAdjBcm) \
      { \
        IPPort  = (BCM)->con->egressMultiMediaCb->icIpPort; \
      } \
    } \
    else if ((BCM)->con->ccmCb.ccn == CCN_0) \
    { \
      if ((BCM)->bcmType == BtOBcm) \
      { \
        IPPort  = (BCM)->con->MultiMediaCb->icIpPort; \
      } \
      else if ((BCM)->bcmType == BtTBcm) \
      { \
        IPPort  = (BCM)->con->MultiMediaCb->ogIpPort; \
      } \
    } \
  } \
  else \
  { \
    if ((BCM)->con->ccmCb.ccn == CCN_1) \
    { \
      if ((BCM)->bcmType == BtOBcm) \
      { \
        IPPort  = &(BCM)->con->icIpParam.locBearerIpPort; \
      } \
      else if ((BCM)->bcmType == BtTBcm) \
      { \
        IPPort  = &(BCM)->con->ogIpParam.locBearerIpPort; \
      } \
      else if ((BCM)->bcmType == BtOAdjBcm) \
      { \
        IPPort  = &(BCM)->con->icAdjIpParam->locBearerIpPort; \
      } \
      else if ((BCM)->bcmType == BtTAdjBcm) \
      { \
        IPPort  = &(BCM)->con->ogAdjIpParam->locBearerIpPort; \
      } \
    } \
    else if ((BCM)->con->ccmCb.ccn == CCN_0) \
    { \
      if ((BCM)->bcmType == BtOBcm) \
      { \
        IPPort  = &(BCM)->con->icIpParam.locBearerIpPort; \
      } \
      else if ((BCM)->bcmType == BtTBcm) \
      { \
        IPPort  = &(BCM)->con->ogIpParam.locBearerIpPort; \
      } \
    } \
  } \
}

#define LOCATE_REMOVEALLMS_4_BCM(BCM, RemoveAllMs) \
{ \
  if (IS_MULTIMEDIA_4_BCM(BCM)) \
  { \
    if ((BCM)->con->ccmCb.ccn == CCN_1) \
    { \
      if ((BCM)->bcmType == BtOBcm) \
      { \
        RemoveAllMs  = &(BCM)->con->ingressMultiMediaCb->icRemoveAllMs; \
      } \
      else if ((BCM)->bcmType == BtTBcm) \
      { \
        RemoveAllMs  = &(BCM)->con->egressMultiMediaCb->ogRemoveAllMs; \
      } \
      else if ((BCM)->bcmType == BtOAdjBcm) \
      { \
        RemoveAllMs  = &(BCM)->con->ingressMultiMediaCb->ogRemoveAllMs; \
      } \
      else if ((BCM)->bcmType == BtTAdjBcm) \
      { \
        RemoveAllMs  = &(BCM)->con->egressMultiMediaCb->icRemoveAllMs; \
      } \
    } \
    else if ((BCM)->con->ccmCb.ccn == CCN_0) \
    { \
      if ((BCM)->bcmType == BtOBcm) \
      { \
        RemoveAllMs  = &(BCM)->con->MultiMediaCb->icRemoveAllMs; \
      } \
      else if ((BCM)->bcmType == BtTBcm) \
      { \
        RemoveAllMs  = &(BCM)->con->MultiMediaCb->ogRemoveAllMs; \
      } \
    } \
  } \
}



#define LOCATE_COMP_RMTSDP_4_BCM(BCM, comp_rmtSdp) \
{ \
  if ((BCM)->con->ccmCb.ccn == CCN_0) \
  { \
    if ((BCM)->bcmType == BtOBcm) \
    { \
      comp_rmtSdp = (BCM)->con->ogIpParam.rmtDesc; \
    } \
    else if ((BCM)->bcmType == BtTBcm) \
    { \
      comp_rmtSdp = (BCM)->con->icIpParam.rmtDesc; \
    } \
  } \
  else if ((BCM)->con->ccmCb.ccn == CCN_1) \
  { \
    if (((BCM)->bcmType == BtOBcm)&&((BCM)->con->ogAdjIpParam)) \
    { \
      comp_rmtSdp = (BCM)->con->ogAdjIpParam->locDesc; \
    } \
    else if (((BCM)->bcmType == BtTBcm)&&((BCM)->con->icAdjIpParam)) \
    { \
      comp_rmtSdp = (BCM)->con->icAdjIpParam->locDesc; \
    } \
    else if ((BCM)->bcmType == BtOAdjBcm) \
    { \
      comp_rmtSdp = (BCM)->con->icIpParam.rmtDesc; \
    } \
    else if ((BCM)->bcmType == BtTAdjBcm) \
    { \
      comp_rmtSdp = (BCM)->con->ogIpParam.rmtDesc; \
    } \
  } \
}

#define IS_REQ(BCM) \
((ccIsXferRcvd(CC_GET_PROTOCOL((BCM)->con->icProtType), \
               CC_GET_PROTOCOL((BCM)->con->ogProtType), \
               (BCM)->con->icEvntType, \
               (BCM)->con->direction)) || \
 (ccIsUpdateRcvd(CC_GET_PROTOCOL((BCM)->con->icProtType), \
                 CC_GET_PROTOCOL((BCM)->con->ogProtType), \
                 (BCM)->con->icEvntType, \
                 (BCM)->con->direction))|| \
 (ccIsQueryRspRcvd(CC_GET_PROTOCOL((BCM)->con->icProtType), \
                   CC_GET_PROTOCOL((BCM)->con->ogProtType), \
                   (BCM)->con->icEvntType, \
                   (BCM)->con->direction))|| \
 (ccIsHoldRcvd(CC_GET_PROTOCOL((BCM)->con->icProtType), \
               CC_GET_PROTOCOL((BCM)->con->ogProtType), \
               (BCM)->con->icEvntType, \
               (BCM)->con->direction))|| \
 (((ccIsPrackRcvd(CC_GET_PROTOCOL((BCM)->con->icProtType), \
                CC_GET_PROTOCOL((BCM)->con->ogProtType), \
                (BCM)->con->icEvntType, \
                (BCM)->con->direction))&&((BCM)->con->direction == INCTOOUT)) && \
                (!(((con->ccmCb.ccn == CCN_1) && \
                  (con->ccmCb.state == CC_INGRESS_MG) && \
                  (CC_OFFANS_ICRMTANS_PENDING(con))) || \
                 ((con->ccmCb.ccn == CCN_1) && \
                  (con->ccmCb.state == CC_EGRESS_MG) && \
                  (CC_OFFANS_ICOFFANS_STATE_NULL(con))) || \
                 ((con->ccmCb.ccn == CCN_0) && \
                  (CC_OFFANS_ICRMTANS_PENDING(con)))))))

#define IS_RSP(BCM) \
((ccIsUpdateRspRcvd(CC_GET_PROTOCOL((BCM)->con->icProtType), \
                    CC_GET_PROTOCOL((BCM)->con->ogProtType), \
                    (BCM)->con->icEvntType, \
                    (BCM)->con->direction)) || \
 (ccIsPrackRspRcvd(CC_GET_PROTOCOL((BCM)->con->icProtType), \
                   CC_GET_PROTOCOL((BCM)->con->ogProtType), \
                   (BCM)->con->icEvntType, \
                   (BCM)->con->direction))|| \
 (ccIsXferRspRcvd(CC_GET_PROTOCOL((BCM)->con->icProtType), \
                  CC_GET_PROTOCOL((BCM)->con->ogProtType), \
                  (BCM)->con->icEvntType, \
                  (BCM)->con->direction))|| \
 (ccIsAckRcvd(CC_GET_PROTOCOL((BCM)->con->icProtType), \
              CC_GET_PROTOCOL((BCM)->con->ogProtType), \
              (BCM)->con->icEvntType, \
              (BCM)->con->direction))|| \
 (ccIsHoldRspRcvd(CC_GET_PROTOCOL((BCM)->con->icProtType), \
                  CC_GET_PROTOCOL((BCM)->con->ogProtType), \
                  (BCM)->con->icEvntType, \
                  (BCM)->con->direction)) || \
 (((ccIsPrackRcvd(CC_GET_PROTOCOL((BCM)->con->icProtType), \
                CC_GET_PROTOCOL((BCM)->con->ogProtType), \
                (BCM)->con->icEvntType, \
                (BCM)->con->direction))) && \
                (((con->ccmCb.ccn == CCN_1) && \
                  (con->ccmCb.state == CC_INGRESS_MG) && \
                  (CC_OFFANS_ICRMTANS_PENDING(con))) || \
                 ((con->ccmCb.ccn == CCN_1) && \
                  (con->ccmCb.state == CC_EGRESS_MG) && \
                  (CC_OFFANS_ICOFFANS_STATE_NULL(con))) || \
                 ((con->ccmCb.ccn == CCN_0) && \
                  (CC_OFFANS_ICRMTANS_PENDING(con))))))
#define IS_MULTISTREAM(CON) \
((CON)&&(MULTIMEDIACB(CON))&&((MULTIMEDIACB(CON)->StreamCount-MULTIMEDIACB(CON)->disabledStreamCount)>1))

#define IS_MULTISTREAM_4_BCM(BCM) \
((BCM)&&(MULTIMEDIACB_4_BCM(BCM))&&((MULTIMEDIACB_4_BCM(BCM)->StreamCount-MULTIMEDIACB_4_BCM(BCM)->disabledStreamCount)>1))


#define IS_MULTIMEDIA_WITH_MEDIA_AWARE(CON) \
((CON)&&(IS_MULTIMEDIA(CON))&&(MULTIMEDIACB(CON)->MediaAwareStreamID))

#define IS_MULTIMEDIA_WITH_MEDIA_AWARE_4_BCM(BCM) \
((BCM)&&(IS_MULTIMEDIA_4_BCM(BCM))&&(MULTIMEDIACB_4_BCM(BCM)->MediaAwareStreamID))


#define IS_MULTIMEDIA_NO_MEDIA_AWARE(CON) \
((CON)&&(IS_MULTIMEDIA(CON))&&(!MULTIMEDIACB(CON)->MediaAwareStreamID))

#define IS_MULTIMEDIA_NO_MEDIA_AWARE_4_BCM(BCM) \
((BCM)&&(IS_MULTIMEDIA_4_BCM(BCM))&&(!MULTIMEDIACB_4_BCM(BCM)->MediaAwareStreamID))


#define MEDIA_AWARE_MS_ID(CON) \
(MULTIMEDIACB(CON)->MediaAwareStreamID)

#define MEDIA_AWARE_MS_ID_4_BCM(BCM) \
(MULTIMEDIACB_4_BCM(BCM)->MediaAwareStreamID)

#if 0
#define HOLD_STATE(CON) \
((IS_MULTIMEDIA_WITH_MEDIA_AWARE(CON))? \
  (&MULTIMEDIACB(CON)->holdState[MEDIA_AWARE_MS_ID(CON)-1]): \
  (&(CON)->holdState))
  
#define HOLD_FLAG(CON) \
((IS_MULTIMEDIA_WITH_MEDIA_AWARE(CON))? \
  (&MULTIMEDIACB(CON)->holdFlag[MEDIA_AWARE_MS_ID(CON)-1]): \
  (&(CON)->holdFlag))
#else

#define HOLD_STATE(CON) \
((&(CON)->holdState))
  
#define HOLD_FLAG(CON) \
((&(CON)->holdFlag))

#endif
#define SET_E2EMEDIAPATHBROKEN(CON) \
{ \
  if ((CON)&&(MULTIMEDIACB(CON))) \
  { \
    MULTIMEDIACB(CON)->E2EMediaPathBroken = TRUE; \
  } \
}

#define SET_E2EMEDIAPATHBROKEN_4_BCM(BCM) \
{ \
  if ((BCM)&&(MULTIMEDIACB_4_BCM(BCM))) \
  { \
    MULTIMEDIACB_4_BCM(BCM)->E2EMediaPathBroken = TRUE; \
  } \
}

#define RESET_E2EMEDIAPATHBROKEN(CON) \
{ \
  if ((CON)&&(MULTIMEDIACB(CON))) \
  { \
    MULTIMEDIACB(CON)->E2EMediaPathBroken = FALSE; \
  } \
}

#define RESET_E2EMEDIAPATHBROKEN_4_BCM(BCM) \
{ \
  if ((BCM)&&(MULTIMEDIACB_4_BCM(BCM))) \
  { \
    MULTIMEDIACB_4_BCM(BCM)->E2EMediaPathBroken = FALSE; \
  } \
}


#define IS_E2EMEDIAPATHBROKEN(CON) \
((CON)&&(MULTIMEDIACB(CON))&&(MULTIMEDIACB(CON)->E2EMediaPathBroken == TRUE))

#define IS_E2EMEDIAPATHBROKEN_4_BCM(BCM) \
((BCM)&&(MULTIMEDIACB_4_BCM(BCM))&&(MULTIMEDIACB_4_BCM(BCM)->E2EMediaPathBroken == TRUE))


#define SET_AGNOSTIC_CAC_BW(cacEvent, mediaType, bandwidth) \
{ \
  switch (mediaType) \
  { \
    case SDP_MTYPE_AUDIO: \
      cacEvent.BW4AudioAgnostic += bandwidth; \
      break; \
    case SDP_MTYPE_IMAGE: \
      cacEvent.BW4FaxAgnostic += bandwidth; \
      break; \
    case SDP_MTYPE_VIDEO: \
      cacEvent.BW4VideoAgnostic += bandwidth; \
      break; \
    /* Add MSRP later */ \
    default: \
      break; \
  } \
}

/* Macros for CcMultiMediaCb variables */
/* Different MultiMediaCb would be located for different CCN */

#define STREAM_COUNT(CON) \
(MULTIMEDIACB(CON)->StreamCount)

#define DISABLED_STREAM_COUNT(CON) \
(MULTIMEDIACB(CON)->disabledStreamCount)

#define IC_IP_PARM(CON) \
(MULTIMEDIACB(CON)->icIpParam)

#define OG_IP_PARM(CON) \
(MULTIMEDIACB(CON)->ogIpParam)

#define SKIP_SAVE_SDP_4_COMP(CON) \
(MULTIMEDIACB(CON)->skipSaveSdp4Comp)

#define SAVED_SDP_4_COMP(CON) \
(MULTIMEDIACB(CON)->savedSdp4Comp)

#define SAVED_SDP_4_RE_EST(CON) \
(MULTIMEDIACB(CON)->savedSdp4ReEst)

#define TEMP_SDP(CON) \
(MULTIMEDIACB(CON)->tempSdp)

#define IC_REMOVE_ALL_MS(CON) \
(MULTIMEDIACB(CON)->icRemoveAllMs)

#define OG_REMOVE_ALL_MS(CON) \
(MULTIMEDIACB(CON)->ogRemoveAllMs)

#define E2E_MEDIA_PATH_BROKEN(CON) \
(MULTIMEDIACB(CON)->E2EMediaPathBroken)
  
/* - FID 14941.0 */

/* FID 14989.3 */
#define ENCAPIAM_CLI  (0x01)  /* encapsulated IAM contains CLI  */
#define CC_IS_ENCAPIAM_CLI(encapIsupParamMask) ((encapIsupParamMask & ENCAPIAM_CLI)? TRUE : FALSE)
#define CC_IS_ICFEND_TRUSTED(con) (((con->icIntfcCb)&&(con->icIntfcCb->ccTgCb))?(con->icIntfcCb->ccTgCb->tgAtt.fendTrustDomain):(FALSE))
#define CC_ICINTFCCB_IS_SIUK(con)                                              \
    ((con) && (con->icIntfcCb != NULL) && (con->icIntfcCb->protType == CC_SIUK))    
#define CC_OGINTFCCB_IS_SIUK(con)                                              \
    ((con) && (con->ogIntfcCb != NULL) && (con->ogIntfcCb->protType == CC_SIUK))    

#define CC_SET_APRI_UKGNPN(con, cgpn, gnpn)                                    \
{                                                                              \
    if (CC_OGINTFCCB_IS_SIUK(con))                                             \
    {                                                                          \
        if ((cgpn.presRest.pres != NOTPRSNT) &&                                \
            (cgpn.presRest.val != PRESREST_NW))                                \
        {                                                                      \
            gnpn.presRest.val = cgpn.presRest.val;                             \
        }                                                                      \
        else                                                                   \
        {                                                                      \
            gnpn.presRest.val = PRESALLOW;                                     \
        }                                                                      \
    }                                                                          \
}

/* FID14989.0-0897, Table28/Q.1912.5 of ND1017:2006/07 */
/* FID 15319.0 TS29.163 following the same action as Q19125 */
#define CC_INSERT_CCODE_TO_FROM(con, from, nmb)                                \
{                                                                              \
    if((CC_ICINTFCCB_IS_SIUK(con) &&                                           \
       ((con->ogProtType == CC_SIP) || (con->ogProtType == CC_SIPTUK))) ||     \
       ((con->ogIntfcCb != NULL) &&                                            \
        (CC_GET_PROTOCOL(con->icIntfcCb->protType) == CC_EXT_BICC)))           \
    {                                                                          \
        ccInsertCcodeToSipFrom(from, &(nmb->natAddrInd.val), con);             \
    }                                                                          \
}

/* FID14989.0-0897, Table27c/Q.1912.5 of ND1017:2006/07 */
#define CC_SET_IGNORECGPNAPRI(con)                                             \
{                                                                              \
    if(CC_ICINTFCCB_IS_SIUK(con))                                              \
    {                                                                          \
      if(con->ogProtType == CC_SIP)                                            \
      {                                                                        \
        con->ogEvnt->m.ccConEvnt.m.sipConEvnt.ignoreCgpnAPRI=1;                \
      }                                                                        \
      else if (con->ogProtType == CC_SIPTUK)                                   \
      {                                                                        \
        con->ogEvnt->m.ccConEvnt.m.siConEvnt.optElmnt.sipt.ignoreCgpnAPRI=1;   \
      }                                                                        \
    }                                                                          \
}

#define CC_SET_UK_NTWKCGPN(prflPtr, cgPtyNum, con)                             \
{                                                                              \
  cgPtyNum.eh.pres         = PRSNT_NODEF;                                      \
  cgPtyNum.natAddrInd.pres = PRSNT_NODEF;                                      \
  cgPtyNum.natAddrInd.val  = CC_NA_NATNUM;                                     \
  cgPtyNum.numPlan.pres    = PRSNT_NODEF;                                      \
  cgPtyNum.numPlan.val     = ISDNNUM;                                          \
  cgPtyNum.niInd.pres      = PRSNT_NODEF;                                      \
  cgPtyNum.niInd.val       = NBMCMLTE;                                         \
  cgPtyNum.scrnInd.pres    = PRSNT_NODEF;                                      \
  cgPtyNum.scrnInd.val     = NETPROV;                                          \
  cgPtyNum.presRest.pres   = PRSNT_NODEF;                                      \
  cgPtyNum.presRest.val    = PRESREST_NW;                                      \
  cgPtyNum.oddEven.pres    = PRSNT_NODEF;                                      \
  cgPtyNum.oddEven.val     = 0;                                                \
  cgPtyNum.addrSig.pres    = PRSNT_NODEF;                                      \
  cgPtyNum.addrSig.len     = 0;                                                \
  cgPtyNum.addrSig.val[0]  = '\0';                                             \
  if ((prflPtr != NULL) && (prflPtr->ntwkCgpn.pres))                           \
  {                                                                            \
    U8 ntwkCgpnAsc[MF_SIZE_TKNSTR];                                            \
    U8 ntwkCgpnBcd[MF_SIZE_TKNSTR];                                            \
    U8 ascLen = 0;                                                             \
    U8 bcdLen = 0;                                                             \
    U8 numDigits = 0;                                                          \
    INITEVENT(ntwkCgpnAsc, MF_SIZE_TKNSTR);                                    \
    INITEVENT(ntwkCgpnBcd, MF_SIZE_TKNSTR);                                    \
    /* add country code */                                                     \
    if (ccIsOgTgIntlIndInternational(con) &&                                   \
        (con->tempAuthTerm) &&                                                 \
        (con->tempAuthTerm->homeCc.len != 0))                                  \
    {                                                                          \
      memcpy((U8 *)&ntwkCgpnAsc[0],                                            \
             (U8 *)&con->tempAuthTerm->homeCc.val[0],                          \
             con->tempAuthTerm->homeCc.len);                                   \
      ascLen = con->tempAuthTerm->homeCc.len;                                  \
      cgPtyNum.natAddrInd.val  = NA_INTNATNUM;                                 \
    }                                                                          \
    memcpy((U8 *)&ntwkCgpnAsc[ascLen],                                         \
           (U8 *)&prflPtr->ntwkCgpn.val[0],                                    \
           prflPtr->ntwkCgpn.len);                                             \
    ascLen = ascLen + prflPtr->ntwkCgpn.len;                                   \
    ntwkCgpnAsc[ascLen]='\0';                                                  \
    /* Convert from ASCII to BCD */                                            \
    if (ccAsciiToBcd(&ntwkCgpnAsc[0],ascLen,&ntwkCgpnBcd[0],&bcdLen,&numDigits) == RFAILED)                                                                              \
    {                                                                          \
        MAP_DP((MAP_SI|MAP_SIP), "%s: ascii to bcd conversion failed\n",       \
               __FUNCTION__);                                                  \
    }                                                                          \
    else                                                                       \
    {                                                                          \
        cgPtyNum.addrSig.len = bcdLen;                                         \
        memcpy((U8 *)&cgPtyNum.addrSig.val[0],                                 \
               (U8 *)&ntwkCgpnBcd[0], bcdLen);                                 \
        cgPtyNum.addrSig.val[cgPtyNum.addrSig.len] = '\0';                     \
        cgPtyNum.oddEven.val = (numDigits&1);                                  \
        for( i = 0; i < cgPtyNum.addrSig.len; i++)                             \
        {                                                                      \
            MAP_DP((MAP_SIP|MAP_SI),"%s:cgPtyNum.addrSig.val[%d]=%d\n",        \
                    __FUNCTION__, i, cgPtyNum.addrSig.val[i]);                 \
        }                                                                      \
    }                                                                          \
  }                                                                            \
}

#define CC_PROCESS_VBD_ANSWER_SDP(_rmtSdp, _con, _Vbddirection, _needVbdModify)\
{\
  if ((_rmtSdp != NULL) && !(ccIsVBDPresent(_rmtSdp)) &&\
      (CC_IS_VBD_NEGOTIATED(_con->flag, _Vbddirection)) &&\
      ((ccIsIPResource(&_con->icBcm) && ccIsIPResource(&_con->ogBcm))))\
  {\
    ccSetVBDFlagsInCon(_con, _rmtSdp);\
    _needVbdModify = TRUE;\
  }\
}

#define IS_VBD_CFG_ON_BOTH_LEGS(_con) (IsVBDConfigured(&_con->icBcm) && IsVBDConfigured(&_con->ogBcm))

/* FID 15261.0 Add macro to get the hash key */
#define HASH_KEY_4_GRCTX(mgSpId, contextId, key) \
{                                                \
  key = mgSpId;                                  \
  key = key << 32;                               \
  key = key | contextId;                         \
}

/* FID 16094.0 Add macro to check if a MG suppots BearerNAT (IBCF) call, currently based on MgVariant */
#define CC_IS_BEARERNAT_SUPPORTED(_mgVarCtx) (((_mgVarCtx)&&(_mgVarCtx)->mgVariant == MGL_VAR_ALU7510)?TRUE:FALSE)


/* + FID-14341:IPV6 */
#define IPV4               0
#define IPV6               1
#define BOTH_IPV4_6        2
#define INVALID_IPV        255 

#define CC_IS_IPV6_SUPPORTED(_mgSapCb) (_mgSapCb->ipv6Supported)
#define CC_GCC_IPV(_sdpIpv)  ((_sdpIpv==AF_INET)?IPV4:IPV6)
#define CC_INET_IPV(_gccIpv) ((_gccIpv==IPV4)?AF_INET:AF_INET6)

#define BCM_DFLT_REALMID(bcm, _realmId)  \
{  \
    CcMgSapCb* __mgSapCb = NULLP;  \
    S16 __ret = RFAILED;  \
    __ret = cmHashListFind(&ccMgSapTbl, (U8*)&(bcm->mgSpId), (U16)sizeof(bcm->mgSpId), (U16)0, (PTR *)&__mgSapCb);  \
    if((__ret != ROK) || (__mgSapCb == NULLP))  \
    {  \
        CCLOGERR("%s(): Find mgSapCb Failed, Set realmId to 0.\n", __FUNCTION__);  \
        _realmId = 0;  \
    }else{  \
        _realmId = __mgSapCb->dfltRealmId;  \
    }  \
}

#define BCM_IPVERSION(bcm, _ipVersion)\
{\
  if((bcm)->bcmType == BtOBcm)\
  {\
    _ipVersion = (bcm)->con->icIpParam.locBearerIpPort.ip.ipVersion;\
  }else if((bcm)->bcmType == BtTBcm)\
  {\
    _ipVersion = (bcm)->con->ogIpParam.locBearerIpPort.ip.ipVersion;\
  }else if((bcm)->bcmType == BtOAdjBcm)\
  {\
    _ipVersion = (bcm)->con->icAdjIpParam->locBearerIpPort.ip.ipVersion;\
  }else if((bcm)->bcmType == BtTAdjBcm)\
  {\
    _ipVersion = (bcm)->con->ogAdjIpParam->locBearerIpPort.ip.ipVersion;\
  }else if ((bcm)->bcmType == BtOLiAdjBcm)\
  {\
    _ipVersion = (bcm)->con->icLiAdjIpParam->locBearerIpPort.ip.ipVersion;\
  }else if ((bcm)->bcmType == BtTLiAdjBcm)\
  {\
    _ipVersion = (bcm)->con->ogLiAdjIpParam->locBearerIpPort.ip.ipVersion;\
  }else\
  {\
    ipVersion = AF_INET;\
  }\
}

#define BCM_IPVERSION_MM(bcm, streamId, _ipVersion) \
{ \
  if (IS_MULTIMEDIA_4_BCM(bcm)) \
  { \
    if ((bcm)->con->ccmCb.ccn == CCN_1) \
    { \
      if ((bcm)->bcmType == BtOBcm) \
      { \
        _ipVersion  = (bcm)->con->ingressMultiMediaCb->icIpPort[streamId].ip.ipVersion; \
      } \
      else if ((bcm)->bcmType == BtTBcm) \
      { \
        _ipVersion  = (bcm)->con->egressMultiMediaCb->ogIpPort[streamId].ip.ipVersion; \
      } \
      else if ((bcm)->bcmType == BtOAdjBcm) \
      { \
        _ipVersion  = (bcm)->con->ingressMultiMediaCb->ogIpPort[streamId].ip.ipVersion; \
      } \
      else if ((bcm)->bcmType == BtTAdjBcm) \
      { \
        _ipVersion  = (bcm)->con->egressMultiMediaCb->icIpPort[streamId].ip.ipVersion; \
      } \
    } \
    else if ((bcm)->con->ccmCb.ccn == CCN_0) \
    { \
      if ((bcm)->bcmType == BtOBcm) \
      { \
        _ipVersion  = (bcm)->con->MultiMediaCb->icIpPort[streamId].ip.ipVersion; \
      } \
      else if ((bcm)->bcmType == BtTBcm) \
      { \
        _ipVersion  = (bcm)->con->MultiMediaCb->ogIpPort[streamId].ip.ipVersion; \
      } \
    } \
  }else{ \
    _ipVersion = AF_INET; \
  } \
}

#define SDP_SET_LOOPBACK_FOR_STREAM(sdp, strm, ipVer) \
{ \
  MediaStream_t *__msPtr; \
  ip_addr_t __ip; \
  __msPtr = (MediaStream_t*)&(sdp)->p[strm]; \
  cmMemset((U8*)&__ip,0,sizeof(__ip)); \
  if(ipVer==AF_INET6) \
  { \
    ip_pton("::1",&__ip); \
  }else \
  { \
    ip_pton("127.0.0.1",&__ip); \
  } \
  __msPtr->address.ip.binary.pres = PRSNT_NODEF; \
  __msPtr->address.ip.binary.val = __ip; \
  __msPtr->address.ip.port.pres = PRSNT_NODEF; \
  __msPtr->address.ip.port.val = 0; \
}

#define IC_MIXED_IPVERSION(mediaCb, _ipVersion)  \
{  \
  int i;  \
  _ipVersion = IPV6;  \
  for (i=0; i<mediaCb->StreamCount; i++)  \
  {  \
    if(mediaCb->icIpPort[i].ip.ipVersion == AF_INET)  \
    {  \
      _ipVersion = IPV4;  \
      break;  \
    }   \
  }  \
}

#define OG_MIXED_IPVERSION(mediaCb, _ipVersion)  \
{  \
  int i;  \
  _ipVersion = IPV6;  \
  for (i=0; i<mediaCb->StreamCount; i++)  \
  {  \
    if(mediaCb->ogIpPort[i].ip.ipVersion == AF_INET)  \
    {  \
      _ipVersion = IPV4;  \
      break;  \
    }   \
  }  \
}

/* - FID-14341:IPV6 */

/* FID 14341.0 IPv6 check if IP address is zero, whether IPv4 or IPv6 */
/* CC_IS_ZERO_IP(ip_addr_t _ipaddr)                                   */
#define CC_IS_ZERO_IP(_ipaddr)                   \
        IP_IS_ADDR_UNSPECIFIED(_ipaddr)

/* FID 14341.0 IPv6 get ipVersion from CcBearerIpport_t * */
#define CC_GET_BEARER_IP_VERSION(_bearerIpPort) \
{ \
   ((_bearerIpPort)?((_bearerIpPort)->ip.ipVersion):0); \
}
/* FID 14341.0 IPv6 set ipVersion for CcBearerIpport_t * */
#define CC_SET_BEARER_IP_VERSION(_bearerIpPort,_ipVersion) \
{ \
  if (_bearerIpPort) (_bearerIpPort)->ip.ipVersion = (_ipVersion); \
}
/* FID 14341.0 IPv6 set ip/port to zero without touching ipVersion for CcBearerIpport_t */
#define CC_SET_ZERO_BEARER_IP_PORT(_bearerIpPort) \
{ \
  if (_bearerIpPort) \
  { \
    (_bearerIpPort)->pres = NOTPRSNT; \
    cmMemset((U8 *)&((_bearerIpPort)->ip.ip),0,sizeof(ip6_addr_t)); \
    (_bearerIpPort)->port = 0; \
  } \
}

#define CC_DEALLOC_OG_ADJ_ROUTE_INTFC(_con)\
{\
  if (_con->ogRscVal)\
  {\
    ccDeallocateResource(_con, CC_OUTGOING);\
    _con->ogRscVal = FALSE;\
  }\
\
}

#define CC_IS_SAME_INTFC(_aIntfc, _bIntfc)  \
(\
   (_aIntfc.intfType == _bIntfc.intfType)&& \
   ( \
    ((_aIntfc.intfType == CC_SIP_INTFC) ? (_aIntfc.t.sipDesc.tgrpId == _bIntfc.t.sipDesc.tgrpId) \
     :(((_aIntfc.intfType == CC_SI_INTFC) || (_aIntfc.intfType == CC_EXT_BICC_INTFC)) ? \
       (_aIntfc.t.tgrpDesc.tgrpId == _bIntfc.t.tgrpDesc.tgrpId) \
       : (_aIntfc.t.intfcId == _bIntfc.t.intfcId)))) \
)
/* FID15310.1 */
#define CC_IS_ICA_CALL(_CON)  ((_CON && ((CC_GET_PROTOCOL(_CON->icProtType) == CC_ICA)))?TRUE:FALSE)
#define CC_CUT_MGCTX_CNNCT(mgCtx, con)                                                           \
{                                                                                                \
  CcMgTerm_t  *_term;                                                                            \
  _term = (mgCtx)? (mgCtx)->term : NULLP;                                                        \
  while(_term)                                                                                   \
  {                                                                                              \
    if(_term->bcm == &((con)->icBcm) ||                                                          \
        _term->bcm == &((con)->ogBcm))                                                           \
    {                                                                                            \
      _term->bcm = NULLP;                                                                        \
    }                                                                                            \
    _term = _term->next;                                                                         \
  }                                                                                              \
}


/* FID 15919.0 ++ */
#define CALL_CG_REL_NONE                0
#define CALL_CG_REL_ALERT                1
/* call is released by calling party after INVITE/IAM and before alerting, no matter original domain */
#define CALL_CG_REL_POST_ALERT          2  
/* call is released by calling party after alerting(180/ACM) before answered, no matter original domain */  
/* FID 15919.0 -- */

#endif /* __CCH__ */

/* FID 15649.0 + */
#define NOTOVFLOWEDCALL 0 /* not overflowed call */
#define OVFLOWEDCALL 2 /* overflowed call */
#define OVFLOWEDCALLID3 3 /* overflowed IG-767/ITU1992 call bug 85956 */
#define OVFLOWEDCALLID4 4 /* overflowed IG-IG call bug 85956 */

#define CC_IS_IC_ORI_SUPPORTED(con) \
     ((con->icIntfcCb && \
       (con->icIntfcCb->ccDDIOption.optORISupported == CC_OPTS_ORI_TW)) ? TRUE : FALSE)  

#define CC_IS_OG_ORI_SUPPORTED(con) \
     ((con->ogIntfcCb && \
      (con->ogIntfcCb->ccDDIOption.optORISupported == CC_OPTS_ORI_TW)) ? TRUE : FALSE)  

#define CC_IS_ORI_SUPPORTED(con) \
     (CC_IS_IC_ORI_SUPPORTED(con) || CC_IS_OG_ORI_SUPPORTED(con))

/* FID 14525.0 ++ */
#define CC_IS_IC_CALLREFA(con) \
     ((con->icIntfcCb && \
       (con->icIntfcCb->ccDDIOption.optCallRefHeader == CC_OPTS_CALLREF_FR)) ? TRUE : FALSE)

#define CC_IS_OG_CALLREFA(con) \
     ((con->ogIntfcCb && \
      (con->ogIntfcCb->ccDDIOption.optCallRefHeader == CC_OPTS_CALLREF_FR)) ? TRUE : FALSE)

#define CC_IS_CALLREFA_REQ(con) \
     (CC_IS_IC_CALLREFA(con) || CC_IS_OG_CALLREFA(con))

#define CC_IS_IC_SFRCPGIW(con) \
     ((con->icIntfcCb && \
       (con->icIntfcCb->ccDDIOption.optSiptSFRCpgIw == CC_OPTS_CALLREF_FR)) ? TRUE : FALSE)

#define CC_IS_OG_SFRCPGIW(con) \
     ((con->ogIntfcCb && \
      (con->ogIntfcCb->ccDDIOption.optSiptSFRCpgIw == CC_OPTS_CALLREF_FR)) ? TRUE : FALSE)

#define CC_IS_SFRCPGIW_REQ(con) \
     (CC_IS_IC_SFRCPGIW(con) || CC_IS_OG_SFRCPGIW(con))
/* FID 14525.0 -- */

#define CC_IS_PROT_Q767(protType) \
   (((protType == CC_SI76792) || \
     (protType == CC_SIPT76792)) ? TRUE : FALSE)

#define CC_IS_PROT_ITU92(protType) \
   (((protType == CC_SIITU92) || \
     (protType == CC_SIPTITU92)) ? TRUE : FALSE)

#define SWAPU16(word) (((U16)((((U16)word&0x00ff))<<8))|((U16)((((U16)word&0xff00))>>8))) /* bug 85956 */

#define CC_SET_ORI(con, ori) \
{ \
   SiOgRteId* ogRteId = (SiOgRteId*) ori; \
   ogRteId->eh.pres = PRSNT_NODEF; \
   CC_SET_ROUTE_TYPE(con, ogRteId); \
   ogRteId->trkGrpId.pres = PRSNT_NODEF; \
   if (CC_GET_PROTOCOL(con->ogProtType) == CC_SIPT) \
      ogRteId->trkGrpId.val = SWAPU16(con->ogRsc.intfc.t.sipDesc.tgrpId); /* bug 85956 */\
   else if(CC_GET_PROTOCOL(con->ogProtType) == CC_SI) \
      ogRteId->trkGrpId.val = SWAPU16(con->ogRsc.intfc.t.tgrpDesc.tgrpId); /* bug 85956 */\
   else if(CC_GET_PROTOCOL(con->ogProtType) == CC_SIP) /* bug 86934 */\
      ogRteId->trkGrpId.val = SWAPU16(con->ogRsc.intfc.t.sipDesc.tgrpId); \
   else \
      ogRteId->trkGrpId.val = 0; \
   ogRteId->gatewayId.pres = PRSNT_NODEF; \
   ogRteId->gatewayId.val = fcCp.switchCfg.gatewayId; \
}

#define CC_RMV_ORI(ori) \
{ \
   SiOgRteId* ogRteId = (SiOgRteId*) ori; \
   ogRteId->eh.pres = NOTPRSNT; \
}

#define CC_IS_ORI_MISS(ogRteId) \
   ((((SiOgRteId*)ogRteId)->eh.pres == NOTPRSNT) ? TRUE : FALSE)

#define CC_STORE_ORI_TO_CDR(con, ori) \
{ \
   CcUniqueCallDtlInfo    *un = NULLP; \
   SiOgRteId* ogRteId = (SiOgRteId*) ori; \
   un = &con->callDtlInfo.un; \
   if (ogRteId && (ogRteId->eh.pres == PRSNT_NODEF)) \
   { \
      un->ovFlowId = OVFLOWEDCALL; \
      un->gatewayId = ogRteId->gatewayId.val; \
      un->trkGrpId = SWAPU16(ogRteId->trkGrpId.val); /* bug 85956 */\
      un->routType = ogRteId->routType.val; \
   } \
}

#define CC_STORE_ORI_TO_CDR_DEFAULT(con) \
{ \
   CcUniqueCallDtlInfo    *un = NULLP; \
   un = &con->callDtlInfo.un; \
   un->ovFlowId = OVFLOWEDCALL; \
   un->gatewayId = 0; \
   un->trkGrpId = 0; \
   un->routType = ORI_NATRT; \
}

/* Bug 85956 + */
#define CC_SET_CDR_ORI_OVFLW_ID(con, overflowId) \
{ \
   con->callDtlInfo.un.ovFlowId = overflowId; \
}
/* Bug 85956 - */

#define CC_SEND_ORI_MISS_EVNT(con) \
{ \
   if (CC_GET_PROTOCOL(con->ogProtType) == CC_SIPT) \
   { \
     ccGenAlarm(LCM_CATEGORY_PROTOCOL, LCC_EVENT_ORI_MISS, LCC_CAUSE_ORI_MISS, (U8*)&con->ogRsc.intfc.t.sipDesc.tgrpId);\
     MAP_DP(MAP_SI,"%s: Gen Alarm on SIPT TrkgrpId=%d\n", __FUNCTION__, con->ogRsc.intfc.t.sipDesc.tgrpId); \
   } \
   else if(CC_GET_PROTOCOL(con->ogProtType) == CC_SI) \
   { \
     ccGenAlarm(LCM_CATEGORY_PROTOCOL, LCC_EVENT_ORI_MISS, LCC_CAUSE_ORI_MISS, (U8*)&con->ogRsc.intfc.t.tgrpDesc.tgrpId); \
     MAP_DP(MAP_SI,"%s: Gen Alarm on ISUP TrkgrpId=%d\n", __FUNCTION__, con->ogRsc.intfc.t.tgrpDesc.tgrpId); \
   } \
}

/* 
 * Macro CC_SET_ROUTE_TYPE is to set the value of Route Type in ori base on the following factors:
 *          cdPtyNum->natAddrInd
 *          ogIntfcCb->ccTgCb->tgAtt.intlInd
 *          ogIntfcCb->ccTgCb->tgAtt.farEndArea
 *          cdPtyNum->addrSig.val
 *          con->tempAuthTerm->homeCc
 */
#define CC_SET_ROUTE_TYPE(con, ogRteId) \
{ \
   SiCdPtyNum *tmpCdPtyNum = NULLP; \
   U8 ccLen = 0; \
   U8 asciiCdPtyNumVal[MF_SIZE_TKNSTR]; \
   ogRteId->routType.pres = PRSNT_NODEF; \
   ogRteId->routType.val = ORI_NATRT; \
   if (CC_IS_CON_IN_EVNT_HOLD(con)) \
   { \
     tmpCdPtyNum = &(con->ccEvntHold->m.ccConEvnt.m.siConEvnt.cdPtyNum); \
   } else \
   { \
     tmpCdPtyNum = &(con->icConEvntHold->m.ccConEvnt.m.siConEvnt.cdPtyNum); \
   } \
   if ((tmpCdPtyNum->eh.pres == PRSNT_NODEF) && \
       (tmpCdPtyNum->natAddrInd.pres == PRSNT_NODEF) && \
       (tmpCdPtyNum->natAddrInd.val == CC_NA_INTNATNUM)) \
   { \
     if (con->ogIntfcCb->ccTgCb->tgAtt.intlInd == TGP_INTL_IND_INTL) \
     { \
       ogRteId->routType.val = ORI_TRANSRT; \
       if ((con->ogIntfcCb->ccTgCb->tgAtt.farEndArea.pres == PRSNT_NODEF) && \
           (tmpCdPtyNum->addrSig.pres == PRSNT_NODEF) && \
           con->ogIntfcCb->ccTgCb->tgAtt.farEndArea.len && \
           tmpCdPtyNum->addrSig.len) \
       { \
            INITEVENT(asciiCdPtyNumVal, MF_SIZE_TKNSTR); \
            if (ccBcdToAscii((U8*)tmpCdPtyNum->addrSig.val, \
                             tmpCdPtyNum->addrSig.len,  \
                             (U8*)asciiCdPtyNumVal, &ccLen,  \
                             tmpCdPtyNum->oddEven.val, FALSE) == RFAILED) \
            { \
                MAP_DP(MAP_SI,"%s: BCD to ASCII conversion failed\n", __FUNCTION__); \
            } \
            if (ccLen >= con->ogIntfcCb->ccTgCb->tgAtt.farEndArea.len) \
            { \
               if (!strncmp(con->ogIntfcCb->ccTgCb->tgAtt.farEndArea.val, \
                            (U8*)asciiCdPtyNumVal, con->ogIntfcCb->ccTgCb->tgAtt.farEndArea.len)) \
               { \
                   ogRteId->routType.val = ORI_TERMRT; \
               } \
            } \
       } \
     } \
     else \
     { \
         if ((con->ogIntfcCb->ccTgCb->tgAtt.farEndArea.pres == PRSNT_NODEF) && \
             (con->tempAuthTerm->homeCc.pres == PRSNT_NODEF) && \
             con->ogIntfcCb->ccTgCb->tgAtt.farEndArea.len && \
             con->tempAuthTerm->homeCc.len) \
         { \
              if (con->tempAuthTerm->homeCc.len == con->ogIntfcCb->ccTgCb->tgAtt.farEndArea.len) \
              { \
                 if (!strncmp(con->ogIntfcCb->ccTgCb->tgAtt.farEndArea.val, \
                             con->tempAuthTerm->homeCc.val, con->tempAuthTerm->homeCc.len)) \
                 { \
                     ogRteId->routType.val = ORI_TERMRT; \
                 } \
              } \
         } \
     } \
  } \
}
/* FID 15649.0 - */

/* FID 15903.0 + */
/* 
 * Macro CC_COPY_CON_PASSTHRU_PARAM to copy ips_addr, callHandle, sessionId, hdrPassBitMask
 * from incoming event to outgoing event
 */
#define CC_COPY_CON_PASSTHRU_PARAM(con) \
{ \
 SipOpt *icConSipOpt = NULL;\
 SipOpt *ogConSipOpt = NULL;\
 SiptOpt *icConSiptOpt = NULL;\
 SiptOpt *ogConSiptOpt = NULL;\
 if (CC_GET_PROTOCOL(con->icProtType) == CC_SIP) \
 { \
   if(CC_GET_PROTOCOL(con->ogProtType) == CC_SIP) \
   { \
     icConSipOpt = &(con->icEvnt->m.ccConEvnt.m.sipConEvnt.sipOpt);\
     ogConSipOpt = &(con->ogEvnt->m.ccConEvnt.m.sipConEvnt.sipOpt);\
     ogConSipOpt->ips_addr = icConSipOpt->ips_addr;\
     ogConSipOpt->callHandle = icConSipOpt->callHandle;\
     ogConSipOpt->sessionId = icConSipOpt->sessionId;\
     ogConSipOpt->hdrPassBitMask = icConSipOpt->hdrPassBitMask;\
     MAP_DP(MAP_SI,"%s: ips_addr=%u callHandle=%u sessionId=%u hdrPassBitMask=%d\n",\
                    __FUNCTION__, (unsigned int)icConSipOpt->ips_addr,\
                    (unsigned int)icConSipOpt->callHandle,\
                    (unsigned int)icConSipOpt->sessionId,\
                    icConSipOpt->hdrPassBitMask);\
   } \
   else if (CC_GET_PROTOCOL(con->ogProtType) == CC_SIPT) \
   { \
    if (con->direction == INCTOOUT) /* SIP->SIPT */ \
    {\
     icConSipOpt = &(con->icEvnt->m.ccConEvnt.m.sipConEvnt.sipOpt);\
     ogConSiptOpt = &(con->ogEvnt->m.ccConEvnt.m.siConEvnt.optElmnt.sipt);\
     ogConSiptOpt->ips_addr = icConSipOpt->ips_addr;\
     ogConSiptOpt->callHandle = icConSipOpt->callHandle;\
     ogConSiptOpt->sessionId = icConSipOpt->sessionId;\
     ogConSiptOpt->hdrPassBitMask = icConSipOpt->hdrPassBitMask;\
     MAP_DP(MAP_SI,"%s: ips_addr=%u callHandle=%u sessionId=%u hdrPassBitMask=%d\n",\
                    __FUNCTION__, (unsigned int)icConSipOpt->ips_addr,\
                    (unsigned int)icConSipOpt->callHandle,\
                    (unsigned int)icConSipOpt->sessionId,\
                    icConSipOpt->hdrPassBitMask);\
    }\
    else /* SIPT-SIP */ \
    {\
     icConSiptOpt = &(con->icEvnt->m.ccConEvnt.m.siConEvnt.optElmnt.sipt);\
     ogConSipOpt = &(con->ogEvnt->m.ccConEvnt.m.sipConEvnt.sipOpt);\
     ogConSipOpt->ips_addr = icConSiptOpt->ips_addr;\
     ogConSipOpt->callHandle = icConSiptOpt->callHandle;\
     ogConSipOpt->sessionId = icConSiptOpt->sessionId;\
     ogConSipOpt->hdrPassBitMask = icConSiptOpt->hdrPassBitMask;\
     MAP_DP(MAP_SI,"%s: ips_addr=%u callHandle=%u sessionId=%u hdrPassBitMask=%d\n",\
                    __FUNCTION__, (unsigned int)icConSiptOpt->ips_addr,\
                    (unsigned int)icConSiptOpt->callHandle,\
                    (unsigned int)icConSiptOpt->sessionId,\
                    icConSiptOpt->hdrPassBitMask);\
    }\
   } \
 } \
 else if(CC_GET_PROTOCOL(con->icProtType) == CC_SIPT) \
 { \
   if(CC_GET_PROTOCOL(con->ogProtType) == CC_SIP) \
   { \
    if (con->direction == INCTOOUT) /* SIPT->SIP */ \
    {\
     icConSiptOpt = &(con->icEvnt->m.ccConEvnt.m.siConEvnt.optElmnt.sipt);\
     ogConSipOpt = &(con->ogEvnt->m.ccConEvnt.m.sipConEvnt.sipOpt);\
     ogConSipOpt->ips_addr = icConSiptOpt->ips_addr;\
     ogConSipOpt->callHandle = icConSiptOpt->callHandle;\
     ogConSipOpt->sessionId = icConSiptOpt->sessionId;\
     ogConSipOpt->hdrPassBitMask = icConSiptOpt->hdrPassBitMask;\
     MAP_DP(MAP_SI,"%s: ips_addr=%u callHandle=%u sessionId=%u hdrPassBitMask=%d\n",\
                    __FUNCTION__, (unsigned int)icConSiptOpt->ips_addr,\
                    (unsigned int)icConSiptOpt->callHandle,\
                    (unsigned int)icConSiptOpt->sessionId,\
                    icConSiptOpt->hdrPassBitMask);\
    }\
    else /* SIP-SIPT */ \
    {\
     icConSipOpt = &(con->icEvnt->m.ccConEvnt.m.sipConEvnt.sipOpt);\
     ogConSiptOpt = &(con->ogEvnt->m.ccConEvnt.m.siConEvnt.optElmnt.sipt);\
     ogConSiptOpt->ips_addr = icConSipOpt->ips_addr;\
     ogConSiptOpt->callHandle = icConSipOpt->callHandle;\
     ogConSiptOpt->sessionId = icConSipOpt->sessionId;\
     ogConSiptOpt->hdrPassBitMask = icConSipOpt->hdrPassBitMask;\
     MAP_DP(MAP_SI,"%s: ips_addr=%u callHandle=%u sessionId=%u hdrPassBitMask=%d\n",\
                    __FUNCTION__, (unsigned int)icConSipOpt->ips_addr,\
                    (unsigned int)icConSipOpt->callHandle,\
                    (unsigned int)icConSipOpt->sessionId,\
                    icConSipOpt->hdrPassBitMask);\
    }\
   } \
   else if (CC_GET_PROTOCOL(con->ogProtType) == CC_SIPT) \
   { \
     icConSiptOpt = &(con->icEvnt->m.ccConEvnt.m.siConEvnt.optElmnt.sipt);\
     ogConSiptOpt = &(con->ogEvnt->m.ccConEvnt.m.siConEvnt.optElmnt.sipt);\
     ogConSiptOpt->ips_addr = icConSiptOpt->ips_addr;\
     ogConSiptOpt->callHandle = icConSiptOpt->callHandle;\
     ogConSiptOpt->sessionId = icConSiptOpt->sessionId;\
     ogConSiptOpt->hdrPassBitMask = icConSiptOpt->hdrPassBitMask;\
     MAP_DP(MAP_SI,"%s: ips_addr=%u callHandle=%u sessionId=%u hdrPassBitMask=%d\n",\
                    __FUNCTION__, (unsigned int)icConSiptOpt->ips_addr,\
                    (unsigned int)icConSiptOpt->callHandle,\
                    (unsigned int)icConSiptOpt->sessionId,\
                    icConSiptOpt->hdrPassBitMask);\
   } \
 } \
}

/* 
 * Macro CC_COPY_CNST_PASSTHRU_PARAM to copy ips_addr, sessionId
 * from incoming event to outgoing event
 * bug-92456 Add NULL check for con->icEvnt for cases whete it is called out
 * of the state machine to internally generate a response - ccSendSiptXferRsp
 */
#define CC_COPY_CNST_PASSTHRU_PARAM(con) \
{ \
 SipOpt *icCnStSipOpt = NULL;\
 SipOpt *ogCnStSipOpt = NULL;\
 SiptOpt *icCnStSiptOpt = NULL;\
 SiptOpt *ogCnStSiptOpt = NULL;\
 if (con->icEvnt != NULLP) \
 { \
   if (CC_GET_PROTOCOL(con->icProtType) == CC_SIP) \
   { \
     if(CC_GET_PROTOCOL(con->ogProtType) == CC_SIP) \
     { \
	 icCnStSipOpt = &(con->icEvnt->m.ccCnStEvnt.m.sipCnStEvnt.sipOpt);\
	 ogCnStSipOpt = &(con->ogEvnt->m.ccCnStEvnt.m.sipCnStEvnt.sipOpt);\
	 ogCnStSipOpt->ips_addr   = icCnStSipOpt->ips_addr;\
	 MAP_DP(MAP_SI,"%s: ips_addr=%u \n",\
			__FUNCTION__, (unsigned int)icCnStSipOpt->ips_addr);\
     } \
     else if (CC_GET_PROTOCOL(con->ogProtType) == CC_SIPT) \
     { \
	if (con->direction == INCTOOUT) /* SIP->SIPT */\
	{\
	 icCnStSipOpt = &(con->icEvnt->m.ccCnStEvnt.m.sipCnStEvnt.sipOpt);\
	 ogCnStSiptOpt = &(con->ogEvnt->m.ccCnStEvnt.m.siCnStEvnt.optElmnt.sipt);\
	 ogCnStSiptOpt->ips_addr = icCnStSipOpt->ips_addr;\
	 MAP_DP(MAP_SI,"%s: ips_addr=%u \n",\
			__FUNCTION__, (unsigned int)icCnStSipOpt->ips_addr);\
	}\
	else /* SIPT-SIP */ \
	{\
	 icCnStSiptOpt = &(con->icEvnt->m.ccCnStEvnt.m.siCnStEvnt.optElmnt.sipt);\
	 ogCnStSipOpt = &(con->ogEvnt->m.ccCnStEvnt.m.sipCnStEvnt.sipOpt);\
	 ogCnStSipOpt->ips_addr = icCnStSiptOpt->ips_addr;\
	 MAP_DP(MAP_SI,"%s: ips_addr=%u \n",\
			__FUNCTION__, (unsigned int)icCnStSiptOpt->ips_addr);\
	}\
     } \
   } \
   else if(CC_GET_PROTOCOL(con->icProtType) == CC_SIPT) \
   { \
       if(CC_GET_PROTOCOL(con->ogProtType) == CC_SIP) \
       { \
	if (con->direction == INCTOOUT) /* SIPT->SIP */\
	{\
	 icCnStSiptOpt = &(con->icEvnt->m.ccCnStEvnt.m.siCnStEvnt.optElmnt.sipt);\
	 ogCnStSipOpt = &(con->ogEvnt->m.ccCnStEvnt.m.sipCnStEvnt.sipOpt);\
	 ogCnStSipOpt->ips_addr = icCnStSiptOpt->ips_addr;\
	 MAP_DP(MAP_SI,"%s: ips_addr=%u \n",\
			__FUNCTION__, (unsigned int)icCnStSiptOpt->ips_addr);\
	}\
	else /* SIP-SIPT */\
	{\
	 icCnStSipOpt = &(con->icEvnt->m.ccCnStEvnt.m.sipCnStEvnt.sipOpt);\
	 ogCnStSiptOpt = &(con->ogEvnt->m.ccCnStEvnt.m.siCnStEvnt.optElmnt.sipt);\
	 ogCnStSiptOpt->ips_addr = icCnStSipOpt->ips_addr;\
	 MAP_DP(MAP_SI,"%s: ips_addr=%u \n",\
			__FUNCTION__, (unsigned int)icCnStSipOpt->ips_addr);\
	}\
       } \
       else if (CC_GET_PROTOCOL(con->ogProtType) == CC_SIPT) \
       { \
	 icCnStSiptOpt = &(con->icEvnt->m.ccCnStEvnt.m.siCnStEvnt.optElmnt.sipt);\
	 ogCnStSiptOpt = &(con->ogEvnt->m.ccCnStEvnt.m.siCnStEvnt.optElmnt.sipt);\
	 ogCnStSiptOpt->ips_addr = icCnStSiptOpt->ips_addr;\
	 MAP_DP(MAP_SI,"%s: ips_addr=%u \n",\
			__FUNCTION__, (unsigned int)icCnStSiptOpt->ips_addr);\
       } \
   } \
 } \
}

/* 
 * Macro CC_COPY_REL_PASSTHRU_PARAM to copy ips_addr
 * from incoming event to outgoing event
 */
#define CC_COPY_REL_PASSTHRU_PARAM(con) \
{ \
 SipRelOpt *icRelSipOpt = NULL;\
 SipRelOpt *ogRelSipOpt = NULL;\
 SiptRelOpt *icRelSiptOpt = NULL;\
 SiptRelOpt *ogRelSiptOpt = NULL;\
 if (CC_GET_PROTOCOL(con->icProtType) == CC_SIP) \
 { \
   if(CC_GET_PROTOCOL(con->ogProtType) == CC_SIP) \
   { \
     icRelSipOpt = &(con->icEvnt->m.ccRelEvnt.m.sipRelEvnt.sipOpt);\
     ogRelSipOpt = &(con->ogEvnt->m.ccRelEvnt.m.sipRelEvnt.sipOpt);\
     ogRelSipOpt->ips_addr   = icRelSipOpt->ips_addr;\
     MAP_DP(MAP_SI,"%s: ips_addr=%d \n",\
                    __FUNCTION__, (unsigned int)icRelSipOpt->ips_addr);\
   } \
   else if (CC_GET_PROTOCOL(con->ogProtType) == CC_SIPT) \
   { \
    if (con->direction == INCTOOUT) /* SIP->SIPT */\
    {\
     icRelSipOpt = &(con->icEvnt->m.ccRelEvnt.m.sipRelEvnt.sipOpt);\
     ogRelSiptOpt = &(con->ogEvnt->m.ccRelEvnt.m.siRelEvnt.optElmnt.m.sipt);\
     ogRelSiptOpt->ips_addr = icRelSipOpt->ips_addr;\
     MAP_DP(MAP_SI,"%s: ips_addr=%d \n",\
                    __FUNCTION__, (unsigned int)icRelSipOpt->ips_addr);\
    }\
    else /* SIPT-SIP */\
    {\
     icRelSiptOpt = &(con->icEvnt->m.ccRelEvnt.m.siRelEvnt.optElmnt.m.sipt);\
     ogRelSipOpt = &(con->ogEvnt->m.ccRelEvnt.m.sipRelEvnt.sipOpt);\
     ogRelSipOpt->ips_addr = icRelSiptOpt->ips_addr;\
     MAP_DP(MAP_SI,"%s: ips_addr=%d \n",\
                    __FUNCTION__, (unsigned int)icRelSiptOpt->ips_addr);\
    }\
   } \
 } \
 else if(CC_GET_PROTOCOL(con->icProtType) == CC_SIPT) \
 { \
   if(CC_GET_PROTOCOL(con->ogProtType) == CC_SIP) \
   { \
    if (con->direction == INCTOOUT) /* SIPT->SIP */\
    {\
     icRelSiptOpt = &(con->icEvnt->m.ccRelEvnt.m.siRelEvnt.optElmnt.m.sipt);\
     ogRelSipOpt = &(con->ogEvnt->m.ccRelEvnt.m.sipRelEvnt.sipOpt);\
     ogRelSipOpt->ips_addr = icRelSiptOpt->ips_addr;\
     MAP_DP(MAP_SI,"%s: ips_addr=%d \n",\
                    __FUNCTION__, (unsigned int)icRelSiptOpt->ips_addr);\
    }\
    else /* SIP-SIPT */\
    {\
     icRelSipOpt = &(con->icEvnt->m.ccRelEvnt.m.sipRelEvnt.sipOpt);\
     ogRelSiptOpt = &(con->ogEvnt->m.ccRelEvnt.m.siRelEvnt.optElmnt.m.sipt);\
     ogRelSiptOpt->ips_addr = icRelSipOpt->ips_addr;\
     MAP_DP(MAP_SI,"%s: ips_addr=%d \n",\
                    __FUNCTION__, (unsigned int)icRelSipOpt->ips_addr);\
    }\
   } \
   else if (CC_GET_PROTOCOL(con->ogProtType) == CC_SIPT) \
   { \
     icRelSiptOpt = &(con->icEvnt->m.ccRelEvnt.m.siRelEvnt.optElmnt.m.sipt);\
     ogRelSiptOpt = &(con->ogEvnt->m.ccRelEvnt.m.siRelEvnt.optElmnt.m.sipt);\
     ogRelSiptOpt->ips_addr = icRelSiptOpt->ips_addr;\
     MAP_DP(MAP_SI,"%s: ips_addr=%d \n",\
                    __FUNCTION__, (unsigned int)icRelSiptOpt->ips_addr);\
   } \
 } \
}
/* FID 15903.0 - */
/* +  FID 14841.0 */
#define  IS_ADID_NEEDED(mgSapCb, cmdType, bcm)                    \
  ( ((mgSapCb)->adid) &&                                          \
    ((cmdType == MGCT_CMD_ADD)||(cmdType == MGCT_CMD_MODIFY)) &&  \
    (ccIsADIDSupported(bcm) == TRUE)                              \
  )
#define HNDL_MGCT_NOT_FOR_ADID(kind, con, event, bcm)              \
  if((kind) == MGCT_NOTIFY_KIND_ADID_IPSTOP)               \
  {                                                                \
     CCDP(AIN_DBG_LEVEL_0, "%s: receive ADID notification from MGI, in state %d\n", __FUNCTION__, (con)->state);   \
     if ((((bcm)->bcmType == BtOBcm) && CC_IS_OBCM_PIC_ACTIVE((con))) ||                                          \
         (((bcm)->bcmType == BtTBcm) && CC_IS_TBCM_PIC_ACTIVE((con))))                                            \
     {                                                                                                            \
      ccHandleMgctNotifyForADID((con), (MgctTxnInd *) (event), (bcm));                                            \
     }                                                                                                            \
     else                                                                                                         \
     {                                                                                                            \
       CCDP(AIN_DBG_LEVEL_0, "%s: Ignore it.\n", __FUNCTION__);                                                   \
     }                                                                                                            \
     RETVALUE(ROK);                                                                                               \
  }

#define CC_SET_HOLD_BY_ZERO_IP(con) \
  ((con->flag) |= (con->direction == INCTOOUT ? CC_FLAG_IC_HOLD_BY_ZERO_IP : CC_FLAG_OG_HOLD_BY_ZERO_IP))
#define CC_RESET_HOLD_BY_ZERO_IP(con) \
  ((con->flag) &= ~(con->direction == INCTOOUT ? CC_FLAG_IC_HOLD_BY_ZERO_IP : CC_FLAG_OG_HOLD_BY_ZERO_IP))
#define CC_IS_IC_HOLD_BY_ZERO_IP(con) ((con->flag) & (CC_FLAG_IC_HOLD_BY_ZERO_IP))
#define CC_IS_OG_HOLD_BY_ZERO_IP(con) ((con->flag) & (CC_FLAG_OG_HOLD_BY_ZERO_IP))

/* The threshold of times that ADID notification is received, which when hit will trigger call release. */
#define MAX_ADID_NOT_RCVD 2 
/* -  FID 14841.0 */
/* FID 16205 + */
#define IS_SUB_MAP_NEEDED(con)                                                        \
  ((con->icIntfcCb) && (con->ogIntfcCb) &&                                            \
   (((con->icIntfcCb->ccTgCb)&&(con->icIntfcCb->ccTgCb->tgAtt.sipPrflPtr)  &&         \
     (con->icIntfcCb->ccTgCb->tgAtt.sipPrflPtr->sipIsubMap) &&                          \
     (con->ogIntfcCb->sipIsubMap))   ||                                                 \
    ((con->icIntfcCb->sipIsubMap)   &&                                                 \
     (con->ogIntfcCb->ccTgCb)&&(con->ogIntfcCb->ccTgCb->tgAtt.sipPrflPtr) &&           \
     (con->ogIntfcCb->ccTgCb->tgAtt.sipPrflPtr->sipIsubMap))))                        \
/* FID 16205 - */

/* + FID 15801.0 */

#define MAX_SIP3XX_REDIR 15

#define CC_IS_SIP_3XX_REDIRECT(_sipStatus) ((_sipStatus.pres == PRSNT_NODEF) &&\
((_sipStatus.val == SIPCAUSE_300_MCHOICES) || (_sipStatus.val == SIPCAUSE_301_MOVEDP)\
 || (_sipStatus.val == SIPCAUSE_302_MOVEDT) || (_sipStatus.val == SIPCAUSE_305_USEPROXY)))

#define CC_FREE_SIP3XX_BLOCK(_con)\
{\
  if (_con->sip3xxCb)\
  {\
    if (_con->sip3xxCb->sip3xxBlock.numHeaders && _con->sip3xxCb->sip3xxBlock.contHdr)\
    {\
      CCDP(AIN_DBG_LEVEL_0,"%s():: Releasing sip3xxBlock mem size=%ld, Addr=0x%lx,numHdr=%d\n",\
           __FUNCTION__, (Size)(_con->sip3xxCb->sip3xxBlock.numHeaders * sizeof(SipContHdr)),\
           (U32)_con->sip3xxCb->sip3xxBlock.contHdr,_con->sip3xxCb->sip3xxBlock.numHeaders);\
      SPutSBuf(ccInit.region, ccInit.pool, _con->sip3xxCb->sip3xxBlock.contHdr,\
             (Size)(_con->sip3xxCb->sip3xxBlock.numHeaders * sizeof(SipContHdr)));\
    }\
    CCDP(AIN_DBG_LEVEL_0,"%s():: Releasing sip3xxCb mem size=%ld, Addr=0x%lx\n",__FUNCTION__,\
         (Size)sizeof(CcSip3xxCb), (U32)_con->sip3xxCb);\
    SPutSBuf(ccInit.region, ccInit.pool, _con->sip3xxCb, (Size)sizeof(CcSip3xxCb));\
    _con->sip3xxCb = NULL;\
  }\
}

#define CC_IS_SIP3XXCALL(_con) (_con->sip3xxCb)

#define CC_IS_SIP3XXFAILCND(_errorCode) ((_errorCode == FC_NOCKTAVAIL) || (_errorCode == FC_INTFC_UNAVAIL)\
                            || (_errorCode == FC_INTFC_UNAVAIL_COST) || (_errorCode == FC_INTFC_UNAVAIL_BCAP)\
                            || (_errorCode == FC_NOROUTDST))

#define CC_IS_SIP_CONTACTURI_AVAIL(_con) (_con && _con->sip3xxCb && \
                                         ((_con->sip3xxCb->currIndex + 1) < _con->sip3xxCb->sip3xxBlock.numHeaders)\
                                         && (_con->sip3xxCb->sip3xxRedirCnt < MAX_SIP3XX_REDIR)) /* LGP-78 */

#define CC_ADVANCE_CONTACT_URI_LIST(_con, _ret)\
{\
\
   ccRestoreEvntHold(_con);\
  _ret = ccRestoreOrigParamFor3xxCall(_con);\
  if (_ret == ROK) \
  { \
    _con->sip3xxCb->currIndex++;\
    CCDP(AIN_DBG_LEVEL_0,"%s():: ReRoute - Current URI index=%d\n",\
         __FUNCTION__, _con->sip3xxCb->currIndex);\
                                \
    _ret = ccProcessORedirect(&(_con->icBcm));\
  } \
}

#define COPY_OCN_FROM_3XXCDPN(_ocn, _con)                                                \
{                                                                                        \
    S16 ret;                                                                             \
    SiCdPtyNum siCdPty;                                                                  \
    CdPtyNmb   inCdPty;                                                                  \
                                                                                         \
   siCdPty.eh.pres = NOTPRSNT;                                                           \
   inCdPty.eh.pres = NOTPRSNT;                                                           \
    switch(CC_GET_PROTOCOL(_con->icProtType))                                            \
    {                                                                                    \
    case CC_SIP:                                                                         \
    case CC_SIPT:                                                                        \
    case CC_SI:                                                                          \
    case CC_EXT_BICC:                                                                    \
        ccGetCdPtyNumFrom3xxCdPtyNum(_con, &siCdPty, NULLP);                             \
        if(siCdPty.eh.pres != NOTPRSNT)                                                  \
        {                                                                                \
            _ocn.eh.pres = siCdPty.eh.pres;                                              \
            COPYTKN(_ocn.oddEven, siCdPty.oddEven);                                      \
            COPYTKN(_ocn.natAddr, siCdPty.natAddrInd);                                   \
            COPYTKN(_ocn.numPlan, siCdPty.numPlan);                                      \
            COPYSTR(_ocn.addrSig, siCdPty.addrSig, siCdPty.addrSig.len);                 \
        }                                                                                \
    break;                                                                               \
                                                                                         \
    case CC_IN:                                                                          \
        ccGetCdPtyNumFrom3xxCdPtyNum(_con, NULLP, &inCdPty);                             \
        if(inCdPty.eh.pres != NOTPRSNT)                                                  \
        {                                                                                \
            _ocn.eh.pres = inCdPty.eh.pres;                                              \
                                                                                         \
            if (inCdPty.nmbPlanId.pres)                                                  \
            {                                                                            \
                _ocn.numPlan.pres = PRSNT_NODEF;                                         \
                ret = ccCnvtNumPlanIDIsdn2ss7(inCdPty.nmbPlanId.val, &_ocn.numPlan.val); \
                if (ret != ROK)                                                          \
                {                                                                        \
                    CCLOGERR("ccCnvtNumPlanIDIsdn2ss7 failed \n");                       \
                    RETVALUE(RFAILED);                                                   \
                }                                                                        \
            }                                                                            \
            if (inCdPty.typeNmb0.pres)                                                   \
            {                                                                            \
                _ocn.natAddr.pres = PRSNT_NODEF;                                         \
                ret = ccCnvtTypeNmbIsdn2ss7(inCdPty.typeNmb0.val, &(_ocn.natAddr.val));  \
                if (ret != ROK)                                                          \
                {                                                                        \
                    CCLOGERR("ccCnvtTypeNmbIsdn2ss7 failed \n");                         \
                    RETVALUE(RFAILED);                                                   \
                }                                                                        \
            }                                                                            \
        }                                                                                \
    break;                                                                               \
                                                                                         \
    default:                                                                             \
    break;                                                                               \
    }                                                                                    \
} /* - US79 */

#define CC_POPULATE_CDR_FOR_SIP3XX(_con)\
{\
  if (_con->sip3xxCb)\
  {\
    S16        ret         = RFAILED;\
    CcAllSdus  *ccEvntHold = NULL;\
    TknStrE    *sip3xxReqUriStr  = NULL;\
    TknStrVar  *sipReqUriStr = NULL;\
    SiNetCallRef *ncr      = NULL;\
\
     _con->callDtlInfo.cm.sip3xxRedirCall = TRUE;\
\
     ncr = &_con->sip3xxCb->sip3xxBlock.contHdr[_con->sip3xxCb->currIndex].ncr;\
     if(ncr)\
     {\
       _con->callDtlInfo.cm.netCallRef.pres = ncr->eh.pres;\
       _con->callDtlInfo.cm.netCallRef.callId = ncr->callId;\
       _con->callDtlInfo.cm.netCallRef.pntCde = ncr->pntCde;\
     }\
     ncr = NULL;\
\
     sip3xxReqUriStr = &_con->sip3xxCb->sip3xxBlock.contHdr[_con->sip3xxCb->currIndex].reqUriStr;\
     if (sip3xxReqUriStr && sip3xxReqUriStr->pres)\
     {\
        ret = ccGetNewMemoryForSipVar(&_con->callDtlInfo.cm.termReqUri, sip3xxReqUriStr->len);\
        if (ret != ROK )\
          RETVALUE(RFAILED);\
        cmMemcpy((U8*)_con->callDtlInfo.cm.termReqUri.val, (U8*)sip3xxReqUriStr->val,\
            sip3xxReqUriStr->len);\
        _con->callDtlInfo.cm.termReqUri.val[sip3xxReqUriStr->len] = '\0';\
     }\
\
     if (CC_IS_CON_IN_EVNT_HOLD(_con))\
        ccEvntHold = _con->ccEvntHold;\
     else if (con->icConEvntHold != NULL)\
        ccEvntHold = _con->icConEvntHold;\
\
    if (ccEvntHold)\
    {\
      switch(CC_GET_PROTOCOL(_con->icProtType))\
      {\
        case CC_SIP:\
          sipReqUriStr = &ccEvntHold->m.ccConEvnt.m.sipConEvnt.reqUriStr;\
          break;\
        case CC_SIPT:\
          sipReqUriStr = &ccEvntHold->m.ccConEvnt.m.siConEvnt.reqUriStr;\
        default:\
          break;\
      }\
      if (sipReqUriStr && sipReqUriStr->pres)\
      {\
            ret = ccGetNewMemoryForSipVar(&_con->callDtlInfo.cm.origReqUri,\
                  sipReqUriStr->len);\
            if (ret != ROK )\
              RETVALUE(RFAILED);\
\
            cmMemcpy((U8*)_con->callDtlInfo.cm.origReqUri.val,\
                   (U8*) sipReqUriStr->val, sipReqUriStr->len);\
            _con->callDtlInfo.cm.origReqUri.val[sipReqUriStr->len] = '\0';\
      }\
    }\
  }\
}
/* - FID 15801.0 */
/* + FID 16371.0 */
#define CC_CHKSIPPRIVACY(val, privacy)       \
{                                            \
   if((cmStrcmp(val,"id") == 0) ||           \
      (cmStrcmp(val,"header") == 0) ||       \
      (cmStrcmp(val,"user") == 0))           \
   {                                         \
      privacy = TRUE;                        \
   }                                         \
}
/* Map calling party number IE refer to TS183036 */
#define CC_CLG_TB2(cgPtyNmb)                    \
{                                               \
   cgPtyNmb.eh.pres = PRSNT_NODEF;              \
   cgPtyNmb.typeNmb1.pres = PRSNT_NODEF;        \
   cgPtyNmb.typeNmb1.val = TON_UNK;             \
   cgPtyNmb.nmbPlanId.pres = PRSNT_NODEF;       \
   cgPtyNmb.nmbPlanId.val = NP_UNKN;            \
   cgPtyNmb.presInd0.pres = PRSNT_NODEF;        \
   cgPtyNmb.presInd0.val  = PI_NOTAVAIL;        \
   cgPtyNmb.screenInd.pres = PRSNT_NODEF;       \
   cgPtyNmb.screenInd.val  = SI_NET;            \
   cgPtyNmb.nmbDigits.pres = NOTPRSNT;          \
}

#define CC_CLG_TB3(cgPtyNmb)                    \
{                                               \
   cgPtyNmb.eh.pres = PRSNT_NODEF;              \
   cgPtyNmb.typeNmb1.pres = PRSNT_NODEF;        \
   cgPtyNmb.typeNmb1.val = TON_UNK;             \
   cgPtyNmb.nmbPlanId.pres = PRSNT_NODEF;       \
   cgPtyNmb.nmbPlanId.val = NP_UNKN;            \
   cgPtyNmb.presInd0.pres = PRSNT_NODEF;        \
   cgPtyNmb.presInd0.val  = PI_RESTRICT;        \
   cgPtyNmb.screenInd.pres = PRSNT_NODEF;       \
   cgPtyNmb.screenInd.val  = SI_NET;            \
   cgPtyNmb.nmbDigits.pres = NOTPRSNT;          \
}
#define CC_NOT_MAP_TB   0
#define CC_MAP_TB2      1
#define CC_MAP_TB3      2
#define CC_MAP_TB4EQ    3
#define CC_MAP_TB4NOTEQ 4
#define CC_MAP_TB4AND5  5

/*FID 14811.0 +*/
#define PI_ST_NULL        0
#define PI_ST_1_OR_2_SENT 1
#define PI_ST_4_SENT      2

#define PI_STATE_CHANGE(con, newState) \
{                                      \
    CCDP(AIN_DBG_LEVEL_0, "PIFSM: State:%2d->%2d\n",(int) (con)->piSentState, (int)newState);\
    (con)->piSentState = (newState);   \
}                                      
/*FID 14811.0 -*/

/* FID 16379.0 */
#define CC_IS_COLP_SUPPORTED(IntfcCb) \
    ((IntfcCb && \
    (IntfcCb->ccDDIOption.optCOLPSupported)) ? TRUE : FALSE)

#define CC_REDIRINFO_CHECK(IntfcCb) \
    ((IntfcCb && \
    (IntfcCb->ccDDIOption.optRedirInfo)) ? TRUE : FALSE)
/* FID 16379.0 end*/

/* FID15317.3 */
#define CC_IS_PEM_PRSNT(con) (((con)->pEarlyMediaVal) && ((con)->pEarlyMediaVal->cnt > 0))
#define CC_GET_PEM_VALUE_BY_MEDIA_INDEX(_INDEX, _PEM, _VALUE)     \
{                                                                 \
   (_VALUE) = SIP_PEM_NONE;                                       \
                                                                  \
   if ((_PEM) && (_PEM)->cnt > 0)                                 \
   {                                                              \
      if ((_PEM)->cnt > (_INDEX))                                 \
      {                                                           \
         (_VALUE) = (_PEM)->params[_INDEX];                       \
      }                                                           \
      else if ((_PEM)->params[(_PEM)->cnt - 1] != SIP_PEM_GATED)  \
      {                                                           \
         (_VALUE) = (_PEM)->params[(_PEM)->cnt - 1];              \
      }                                                           \
      else if ((_PEM)->cnt > 1)                                   \
      {                                                           \
         (_VALUE) = (_PEM)->params[(_PEM)->cnt - 2];              \
      }                                                           \
      else                                                        \
      {                                                           \
         CCLOGERR("Invalid PEM\n");                               \
      }                                                           \
   }                                                              \
                                                                  \
   if (((_VALUE) != SIP_PEM_SENDRECV) &&                          \
         ((_VALUE) != SIP_PEM_SENDONLY) &&                        \
         ((_VALUE) != SIP_PEM_RECVONLY) &&                        \
         ((_VALUE) != SIP_PEM_INACTIVE))                          \
   {                                                              \
      (_VALUE) = SIP_PEM_NONE;                                    \
      CCLOGERR("Invalid PEM\n");                                  \
   }                                                              \
}


/* FID 16466.0 */
#define CC_COPY_CON_OLI_PARAM(con) \
{ \
    SipConEvnt *icSip  = NULL;\
    SipConEvnt *ogSip  = NULL;\
    SiConEvnt  *icSi  = NULL;\
    SiConEvnt  *ogSi  = NULL;\
    if (CC_GET_PROTOCOL(con->icProtType) == CC_SIP) \
    { \
        if(CC_GET_PROTOCOL(con->ogProtType) == CC_SIP) \
        { \
            icSip = &(con->icEvnt->m.ccConEvnt.m.sipConEvnt);\
            ogSip = &(con->ogEvnt->m.ccConEvnt.m.sipConEvnt);\
            if( icSip->origLineInf.eh.pres )\
            {\
                ogSip->origLineInf.eh.pres = PRSNT_NODEF; \
                ogSip->origLineInf.lineInfo.pres = PRSNT_NODEF; \
                ogSip->origLineInf.lineInfo.val = icSip->origLineInf.lineInfo.val;\
                MAP_DP(MAP_SIP,"ogSip->origLineInf.lineInfo.val =%d\n",\
                        ogSip->origLineInf.lineInfo.val);\
            }\
        } \
    } \
    else if(CC_GET_PROTOCOL(con->icProtType) == CC_SIPT) \
    { \
        if(CC_GET_PROTOCOL(con->ogProtType) == CC_SIP) \
        { \
            icSi  = &(con->icEvnt->m.ccConEvnt.m.siConEvnt); \
            ogSip = &(con->ogEvnt->m.ccConEvnt.m.sipConEvnt);\
            if (icSi->origLineInf.eh.pres)\
            {\
                ogSip->origLineInf.eh.pres = PRSNT_NODEF;\
                ogSip->origLineInf.lineInfo.pres = PRSNT_NODEF;\
                ogSip->origLineInf.lineInfo.val = icSi->origLineInf.lineInfo.val;\
                MAP_DP((MAP_SIP|MAP_SI),"ogSip->origLineInf.lineInfo.val =%d\n",\
                        ogSip->origLineInf.lineInfo.val);\
            }\
        } \
        else if (CC_GET_PROTOCOL(con->ogProtType) == CC_SIPT) \
        { \
            icSi = &(con->icEvnt->m.ccConEvnt.m.siConEvnt); \
            ogSi = &(con->ogEvnt->m.ccConEvnt.m.siConEvnt); \
            if (icSi->origLineInf.eh.pres)\
            {\
                ogSi->origLineInf.eh.pres = PRSNT_NODEF;\
                    ogSi->origLineInf.lineInfo.pres = PRSNT_NODEF;\
                    ogSi->origLineInf.lineInfo.val = icSi->origLineInf.lineInfo.val;\
                    MAP_DP((MAP_SI),"ogSi->origLineInf.lineInfo.val =%d\n",\
                            ogSi->origLineInf.lineInfo.val);\
            }\
        } \
    } \
}

/* FID 16204.0 */
#define SIP_UUI_LEN   258
#define SIP_BELUUI_LEN 516        /* 93118 */
#define CC_BOTH_SUPPORT_UUI(con)       \
     ccBothSupportUui(con)

#define CC_BOTH_SUPPORT_ALERTINFO(con)    \
        ccBothSupportAlertInfo(con)

/* + FID 14457.2 */
/* Consider Memory congestion into calculation every 6 seconds to avoid OOM triggers */
#define MEMORY_CONGESTION_CALC_INTERVAL 60
#define CC_CONG_TIMER_RES     1  /*100ms*/
#define CC_CONG_TIMER_1       1  /* 20 sec timer for GCC DQ congestion alm */
#define CC_CONG_TIMER_2       2  /* 20 sec timer for GCC to SIPA Relay congestion alm */
#define CC_CONG_SAMP_TIMER    3  /* 60 sec timer for CCS memory usage 1st and 2nd sampling */
#define CC_CONG_TIMER_MAX     (CC_CONG_SAMP_TIMER + 1)
#define CC_CONG_TIMER_WAIT    200 /*20s*/
#define CC_CONG_SAMP_WAIT     600 /* 60s */
#define CCMAXRLYCHNLMON       1   /* Maximum No. of GCC to Modx Relay Channels monitored*/

#define CC_ALARM_TIMER_RES               1  /*100ms*/
#define CC_FRAUD_ALARM_THRESH_TIMER      1          /* FID16259.0 threshold alarm */
#define CC_ALARM_TIMER_MAX               (CC_FRAUD_ALARM_THRESH_TIMER + 1)

#define CC_FRAUD_ALARM_THRESH_WAIT       3*60*10         /* FID16259.0 3min */

#define CC_FIND_GCC_DQ_CNG_ALM_VAL(_dqCngLvl) ((_dqCngLvl==DQCONG_LEVEL_FOUR)?LCM_CAUSE_GCC_CONG_LVL4:((_dqCngLvl==DQCONG_LEVEL_THREE)?LCM_CAUSE_GCC_CONG_LVL3:((_dqCngLvl==DQCONG_LEVEL_TWO)?LCM_CAUSE_GCC_CONG_LVL2:((_dqCngLvl==DQCONG_LEVEL_ONE)?LCM_CAUSE_GCC_CONG_LVL1:LCM_CAUSE_GCC_CONG_CLR))))

#define CC_FIND_GCC_RLY_CNG_ALM_VAL(_rlyCngLvl) ((_rlyCngLvl==L4_CONGESTION)?LCM_CAUSE_GCC_RELAY_CONG_LVL4:((_rlyCngLvl==L3_CONGESTION)?LCM_CAUSE_GCC_RELAY_CONG_LVL3:((_rlyCngLvl==L2_CONGESTION)?LCM_CAUSE_GCC_RELAY_CONG_LVL2:((_rlyCngLvl==L1_CONGESTION)?LCM_CAUSE_GCC_RELAY_CONG_LVL1:LCM_CAUSE_GCC_RELAY_CONG_CLR))))

#define CC_INIT_CONG_ACTN_CNTS \
{\
  ccCongCb.ccDqExtCngCb.currAttemptCnt = 0;\
  ccCongCb.ccDqExtCngCb.currBlockCnt = 0;\
  ccCongCb.ccDqExtCngCb.nrmCallsDroppedSoFar = 0;\
  ccCongCb.ccDqExtCngCb.prioCallsDroppedSoFar = 0;\
}

#define IS_CONG_DUE_TO_GDI(_dqFlag, _gdiFlag, _rtFlag, _memFlag) ((_gdiFlag) && !(_dqFlag) && !(_rtFlag) && !(_memFlag)) 
#define IS_CONG_DUE_TO_MEM(_dqFlag, _gdiFlag, _rtFlag, _memFlag) (!(_gdiFlag) && !(_dqFlag) && !(_rtFlag) && (_memFlag)) 

#define CC_LOG_GCC_CONG_ACTN_INFO(_newActnLvl, _dqFlag, _gdiFlag, _rtFlag, _memFlag)\
{\
    if (ccCongCb.ccDqExtCngCb.ccCongActnLvl != _newActnLvl)\
    {\
      DP("%s(), In Congestion Level %d, Attempted %ld calls and Dropped:: %ld NormalCalls , %ld PrioCalls\n",\
          __FUNCTION__, ccCongCb.ccDqExtCngCb.ccCongActnLvl, ccCongCb.ccDqExtCngCb.currAttemptCnt, \
          ccCongCb.ccDqExtCngCb.nrmCallsDroppedSoFar, ccCongCb.ccDqExtCngCb.prioCallsDroppedSoFar);\
      DP("%s(), GCC Congestion Level Changes from %d to %d due to %s\n",\
        __FUNCTION__, ccCongCb.ccDqExtCngCb.ccCongActnLvl, _newActnLvl,\
         ((dqFlag)?"GCC DQ CONG":((gdiFlag)?"GDI CONG":((rtFlag)?"ROUTER CONG":(_memFlag)?"MEMORY CONG":"NO CONGESTION"))));\
      ccCongCb.ccDqExtCngCb.ccCongActnLvl = _newActnLvl;\
      CC_INIT_CONG_ACTN_CNTS;\
    }\
}

#define CC_INIT_RLY_CONG_ACTN_CNTS(_ccRlyCngCb) \
{\
  _ccRlyCngCb->currAttemptCnt = 0;\
  _ccRlyCngCb->currBlockCnt = 0;\
  _ccRlyCngCb->nrmCallsDroppedSoFar = 0;\
  _ccRlyCngCb->prioCallsDroppedSoFar = 0;\
  _ccRlyCngCb->nrmCallsReroutedSoFar = 0;\
  _ccRlyCngCb->prioCallsReroutedSoFar = 0;\
}

#define CC_INIT_RLY_CONG_CTRL_BLOCK(_ccRlyCngCb)\
{\
  _ccRlyCngCb->ccRlyChnlInfo.destModule[0] = 0;\
  _ccRlyCngCb->ccRlyChnlInfo.channelNo = 0;\
  _ccRlyCngCb->ccRCongActnLvl = CCCONG_LEVEL_NORMAL;\
  _ccRlyCngCb->currCCRlyCngLvl = CCCONG_LEVEL_NORMAL;\
  _ccRlyCngCb->latestRlycngLvl = CCCONG_LEVEL_NORMAL;\
  CC_INIT_RLY_CONG_ACTN_CNTS(_ccRlyCngCb);\
}

#define CC_INIT_MEGAA_CONG_ACTN_CNTS(_ccMegaCngCb) \
{\
  _ccMegaCngCb->currAttemptCnt = 0;\
  _ccMegaCngCb->currBlockCnt = 0;\
  _ccMegaCngCb->nrmCallsDroppedSoFar = 0;\
  _ccMegaCngCb->prioCallsDroppedSoFar = 0;\
}

#define CC_INIT_MEGAA_CONG_CTRL_BLOCK(_ccMegaCngCb)\
{\
  _ccMegaCngCb->ccMegaCngActnLvl = MEGA_CONG_CLR;\
  CC_INIT_MEGAA_CONG_ACTN_CNTS(_ccMegaCngCb);\
}

/* - FID 14457.2 */

/* FID16251.1 + */
#define SIPT_MAX_UUI_LEN   262
#define CC_IS_ISUB_BOTH_SUPPORT_SIPSIPT(sipPrflCb, siptPrflCb) \
 ((sipPrflCb) && ((sipPrflCb)->sipIsubMap) && (siptPrflCb) && ((siptPrflCb)->sipIsubMap))
/* FID16521.1 - */

/* + FID 16521.0 + */
#define CC_MAP_BC_TO_USI(BC, USI) \
{ \
    (USI).infoTranCap.pres = (BC).infoTranCap.pres; \
    (USI).infoTranCap.val = (BC).infoTranCap.val; \
    (USI).cdeStand.pres = (BC).codeStand0.pres; \
    (USI).cdeStand.val = (BC).codeStand0.val; \
    (USI).infoTranRate0.pres = (BC).infoTranRate0.pres; \
    (USI).infoTranRate0.val = (BC).infoTranRate0.val; \
    (USI).tranMode.pres = (BC).tranMode.pres; \
    (USI).tranMode.val = (BC).tranMode.val; \
    (USI).rateMultiplier.pres = (BC).rateMultiplier.pres;   \
    (USI).rateMultiplier.val = (BC).rateMultiplier.val;     \
    (USI).usrInfLyr1Prot.pres = (BC).usrInfoLyr1Prot.pres; \
    (USI).usrInfLyr1Prot.val = (BC).usrInfoLyr1Prot.val; \
    (USI).lyr1Ident.pres = (BC).lyr1Ident.pres; \
    (USI).lyr1Ident.val = (BC).lyr1Ident.val; \
    (USI).usrRate.pres = (BC).usrRate.pres; \
    (USI).usrRate.val = (BC).usrRate.val; \
    (USI).negot.pres = (BC).negot.pres; \
    (USI).negot.val = (BC).negot.val; \
    (USI).syncAsync.pres = (BC).syncAsync.pres; \
    (USI).syncAsync.val = (BC).syncAsync.val; \
    (USI).flcOnRx.pres = (BC).flcOnRx.pres; \
    (USI).flcOnRx.val = (BC).flcOnRx.val; \
    (USI).flcOnTx.pres = (BC).flcOnTx.pres; \
    (USI).flcOnTx.val = (BC).flcOnTx.val; \
    (USI).niClkOnRx.pres = (BC).nicOnRx.pres; \
    (USI).niClkOnRx.val = (BC).nicOnRx.val; \
    (USI).niClkOnTx.pres = (BC).nicOnTx.pres; \
    (USI).niClkOnTx.val = (BC).nicOnTx.val; \
    (USI).interRate.pres = (BC).interRate.pres; \
    (USI).interRate.val = (BC).interRate.val; \
    (USI).inOutBandNeg.pres = (BC).inOutBandNegot.pres; \
    (USI).inOutBandNeg.val = (BC).inOutBandNegot.val; \
    (USI).asgnrAsgne.pres = (BC).asgnrAsgne.pres; \
    (USI).asgnrAsgne.val = (BC).asgnrAsgne.val; \
    (USI).logLnkNegot.pres = (BC).logLnkNegot.pres; \
    (USI).logLnkNegot.val = (BC).logLnkNegot.val; \
    (USI).mode.pres = (BC).mode.pres; \
    (USI).mode.val = (BC).mode.val; \
    (USI).multiFrm.pres = (BC).multiFrm.pres; \
    (USI).multiFrm.val = (BC).multiFrm.val; \
    (USI).hdrNohdr.pres = (BC).hdrNoHdr.pres; \
    (USI).hdrNohdr.val = (BC).hdrNoHdr.val; \
    (USI).parity.pres = (BC).parity.pres; \
    (USI).parity.val = (BC).parity.val; \
    (USI).nmbDatBits.pres = (BC).nmbDatBits.pres; \
    (USI).nmbDatBits.val = (BC).nmbDatBits.val; \
    (USI).nmbStpBits.pres = (BC).nmbStopBits.pres; \
    (USI).nmbStpBits.val = (BC).nmbStopBits.val; \
    (USI).modemType.pres = (BC).modemType.pres; \
    (USI).modemType.val = (BC).modemType.val; \
    (USI).duplexMode.pres = (BC).duplexMode.pres; \
    (USI).duplexMode.val = (BC).duplexMode.val; \
    (USI).usrInfLyr2Prot.pres = (BC).usrInfoLyr2Prot0.pres; \
    (USI).usrInfLyr2Prot.val = (BC).usrInfoLyr2Prot0.val; \
    (USI).lyr2Ident.pres = (BC).lyr2Ident.pres; \
    (USI).lyr2Ident.val = (BC).lyr2Ident.val; \
    (USI).usrInfLyr3Prot.pres = (BC).usrInfoLyr3Prot0.pres; \
    (USI).usrInfLyr3Prot.val = (BC).usrInfoLyr3Prot0.val; \
    (USI).lyr3Ident.pres = (BC).lyr3Ident0.pres; \
    (USI).lyr3Ident.val = (BC).lyr3Ident0.val; \
}

#define CC_MAP_USI_TO_BC(USI, BC) \
{ \
    (BC).infoTranCap.pres = (USI).infoTranCap.pres; \
    (BC).infoTranCap.val = (USI).infoTranCap.val; \
    (BC).codeStand0.pres = (USI).cdeStand.pres; \
    (BC).codeStand0.val = (USI).cdeStand.val; \
    (BC).infoTranRate0.pres = (USI).infoTranRate0.pres; \
    (BC).infoTranRate0.val = (USI).infoTranRate0.val; \
    (BC).tranMode.pres = (USI).tranMode.pres; \
    (BC).tranMode.val = (USI).tranMode.val; \
    (BC).rateMultiplier.pres = (USI).rateMultiplier.pres;   \
    (BC).rateMultiplier.val = (USI).rateMultiplier.val;     \
    (BC).usrInfoLyr1Prot.pres = (USI).usrInfLyr1Prot.pres; \
    (BC).usrInfoLyr1Prot.val = (USI).usrInfLyr1Prot.val; \
    (BC).lyr1Ident.pres = (USI).lyr1Ident.pres; \
    (BC).lyr1Ident.val = (USI).lyr1Ident.val; \
    (BC).usrRate.pres = (USI).usrRate.pres; \
    (BC).usrRate.val = (USI).usrRate.val; \
    (BC).negot.pres = (USI).negot.pres; \
    (BC).negot.val = (USI).negot.val; \
    (BC).syncAsync.pres = (USI).syncAsync.pres; \
    (BC).syncAsync.val = (USI).syncAsync.val; \
    (BC).flcOnRx.pres = (USI).flcOnRx.pres; \
    (BC).flcOnRx.val = (USI).flcOnRx.val; \
    (BC).flcOnTx.pres = (USI).flcOnTx.pres; \
    (BC).flcOnTx.val = (USI).flcOnTx.val; \
    (BC).nicOnRx.pres = (USI).niClkOnRx.pres; \
    (BC).nicOnRx.val = (USI).niClkOnRx.val; \
    (BC).nicOnTx.pres = (USI).niClkOnTx.pres; \
    (BC).nicOnTx.val = (USI).niClkOnTx.val; \
    (BC).interRate.pres = (USI).interRate.pres; \
    (BC).interRate.val = (USI).interRate.val; \
    (BC).inOutBandNegot.pres = (USI).inOutBandNeg.pres; \
    (BC).inOutBandNegot.val = (USI).inOutBandNeg.val; \
    (BC).asgnrAsgne.pres = (USI).asgnrAsgne.pres; \
    (BC).asgnrAsgne.val = (USI).asgnrAsgne.val; \
    (BC).logLnkNegot.pres = (USI).logLnkNegot.pres; \
    (BC).logLnkNegot.val = (USI).logLnkNegot.val; \
    (BC).mode.pres = (USI).mode.pres; \
    (BC).mode.val = (USI).mode.val; \
    (BC).multiFrm.pres = (USI).multiFrm.pres; \
    (BC).multiFrm.val = (USI).multiFrm.val; \
    (BC).hdrNoHdr.pres = (USI).hdrNohdr.pres; \
    (BC).hdrNoHdr.val = (USI).hdrNohdr.val; \
    (BC).parity.pres = (USI).parity.pres; \
    (BC).parity.val = (USI).parity.val; \
    (BC).nmbDatBits.pres = (USI).nmbDatBits.pres; \
    (BC).nmbDatBits.val = (USI).nmbDatBits.val; \
    (BC).nmbStopBits.pres = (USI).nmbStpBits.pres; \
    (BC).nmbStopBits.val = (USI).nmbStpBits.val; \
    (BC).modemType.pres = (USI).modemType.pres; \
    (BC).modemType.val = (USI).modemType.val; \
    (BC).duplexMode.pres = (USI).duplexMode.pres; \
    (BC).duplexMode.val = (USI).duplexMode.val; \
    (BC).usrInfoLyr2Prot0.pres = (USI).usrInfLyr2Prot.pres; \
    (BC).usrInfoLyr2Prot0.val = (USI).usrInfLyr2Prot.val; \
    (BC).lyr2Ident.pres = (USI).lyr2Ident.pres; \
    (BC).lyr2Ident.val = (USI).lyr2Ident.val; \
    (BC).usrInfoLyr3Prot0.pres = (USI).usrInfLyr3Prot.pres; \
    (BC).usrInfoLyr3Prot0.val = (USI).usrInfLyr3Prot.val; \
    (BC).lyr3Ident0.pres = (USI).lyr3Ident.pres; \
    (BC).lyr3Ident0.val = (USI).lyr3Ident.val; \
}

#define PI_ACTION_EXCLUDE 0     
#define PI_ACTION_INCLUDE 1

#define PI_FILTER_LOCATION 0
#define PI_FILTER_CODESTD  1
#define PI_FILTER_PD       2

/* - FID 16521.0 - */

/* FID 15046.0 + */
#define CC_COLPNOFRMCHG 0x00
/* 0000 0000 do not support from-change tag */

#define CC_COLPFRMCHG  SIP_COLP_SUPPORT_FROMCHG
/* 0000 0001 support from-change tag */

#define CC_COLPFRMCHGTMR  (SIP_COLP_WAIT_UPDATE | SIP_COLP_SUPPORT_FROMCHG)
/* 0000 0011 support from-change and need to start timer*/

#define CC_TRIGGERUPDATE  (SIP_COLP_SEND_UPDATE | SIP_COLP_SUPPORT_FROMCHG)
/* 0000 0101 notify SIPA to trigger UPDATE */

#define CC_COLPFRMCHGUPD  (SIP_COLP_UPDATE_FROM_HDR | SIP_COLP_SUPPORT_FROMCHG)
/* 0000 1001 support from-change tag and use UPDATE from hdr to mapping */

#define CC_COLP_18XPAID  0x10
/* 0001 0000 use PAID/Privacy in 18X */

#define CC_COLPPRIVACY_PAID 0x20
/* 0010 0000 Privacy=id/header, PAID pres in 200OK */

#define CC_COLPPRIVACY_NOPAID 0x40
/* 0100 0000 Privacy=id/header, PAID not pres in 200 OK */

#define CC_BOTH_SUPPORT_COLP(con)       \
           ccBothSupportColp(con)

#define CC_ISOFCIPRES(siConEvnt) \
           (((siConEvnt) && (siConEvnt->opFwdCalInd.eh.pres) &&  \
           (siConEvnt->opFwdCalInd.conLineIdReqInd.pres) && \
           (siConEvnt->opFwdCalInd.conLineIdReqInd.val == CONNADDRREQ)) ? TRUE : FALSE)
/* FID 15046.0 - */

/* FID 16204.1 + */
#define UUS_SERVICE_1       1
#define UUS_SERVICE_2       2
#define UUS_SERVICE_3       3

#define LOCAL_VALUE  1
#define GLOBAL_VALUE 2
/* FID 16204.1 - */

/* FID 16206.0 +*/
#define UNKPTYNUM    1
#define PUBPTYNUM    2
#define NSAPENCNUM   3
#define DATAPTYNUM   4
#define TELPTYNUM    5
#define PRIPTYNUM    6
#define NATSTDPTYNUM 7

/* FID16647.0 + */
#define FCI_BIT_M_MASK  0x01
#define FCI_BIT_N_MASK  0x02
#define CC_GET_FCI_BIT_N(transCallNInd) \
            (((transCallNInd.val) & FCI_BIT_N_MASK) >> 1) 
#define CC_GET_FCI_BIT_M(transCallNInd) \
            ((transCallNInd.val) & FCI_BIT_M_MASK)  
#define CC_CLR_FCI_BIT_M(transCallNInd) \
            ((transCallNInd.val) &= ~FCI_BIT_M_MASK) 
#define CC_SET_FCI_BIT_M(transCallNInd) \
            ((transCallNInd.val) |= FCI_BIT_M_MASK) 
/* FID16647.0 - */

#define ALLNUMBERS  0x02
#define INDNUMBER   0x01

#define EXPIRESVAL 69   /* expires value, used for sip Msg Req */

#define CC_IS_FRWDTOADDR_VALID(cfxDiv)   \
         ((cfxDiv.eh.pres) && \
                (cfxDiv.frwdToAddr.eh.pres) &&   \
                (cfxDiv.frwdToAddr.ptyNum.type.pres ))

#define CC_IS_SERUSRNR_VALID(cfxDiv)   \
         ((cfxDiv.eh.pres) && \
                (cfxDiv.serUsrNr.type.pres) &&   \
                (cfxDiv.serUsrNr.type.val == INDNUMBER) && \
                (cfxDiv.serUsrNr.u.indNum.type.pres))
#define RINVSEVUSRNR 2
#define CC_DIV_MAXXMLLEN 1024
#define CC_CFX_MAXVALLEN 32

#define CC_BOTH_SUPPORT_CFX(con)  \
            ccBothSupportCfx(con)
#define CFX_TS183036 1

#define RSNTP_Q931 1
#define RSNTP_ASN  2

/* Toll Free judgement by BTS instead of GCC due to requirement change*/
#if 0
/* FID 16469.2 + */
#define TOLL_FREE_TGRP_LEN  4
#define TOLL_FREE_TGRP_1ST_DIGIT 0x34
/* FID 16469.2 - */
#endif

#define CC_FILLREDIRGNUM(redirgNum, typNumDiv, divNr, presInd1)  \
{    \
   redirgNum.eh.pres = PRSNT_NODEF;               \
   if(presInd1.pres)                                    \
   {                                                   \
      redirgNum.presInd.pres = PRSNT_NODEF;            \
      if(presInd1.val == PSNT_TYPE_ALOW)                              \
      {                                                \
         redirgNum.presInd.val = PRESALLOW;            \
      }                                                \
      else                                             \
      {                                                \
         redirgNum.presInd.val = PRESREST;             \
      }                                                \
   }                                                   \
   else                                                \
   {                                                   \
      redirgNum.presInd.pres = PRSNT_NODEF;            \
      redirgNum.presInd.val = PRESREST;                \
   }                                                   \
  if(divNr.pres)                                      \
   {                                                   \
   redirgNum.nmbPlanId.pres = PRSNT_NODEF;             \
   redirgNum.nmbPlanId.val = NP_ISDN;                  \
      redirgNum.typeNmb.pres = PRSNT_NODEF;               \
      redirgNum.typeNmb.val = typNumDiv.val;              \
   redirgNum.nmbDigits.pres = PRSNT_NODEF;             \
   cmMemcpy((U8 *)(&redirgNum.nmbDigits), (U8 *)&divNr, sizeof(TknStrS)); \
   if(divNr.len>= MF_SIZE_TKNSTRS + 2)                 \
   {                                                   \
      redirgNum.nmbDigits.len = MF_SIZE_TKNSTRS + 1;   \
   }                                                   \
   redirgNum.nmbDigits.val[redirgNum.nmbDigits.len] = '\0';\
   }                                                       \
   else                                                    \
   {                                                       \
      INITEVENT(&(redirgNum.nmbPlanId),sizeof(TknU8));     \
      INITEVENT(&(redirgNum.typeNmb), sizeof(TknU8));     \
      INITEVENT(&(redirgNum.nmbDigits),sizeof(TknStrS));  \
   }                                                       \
}

#define INITDIVLENINFO2(facInfo)\
{ \
   facInfo.protPrfl.pres = PRSNT_NODEF;        \
   facInfo.protPrfl.val = PROT_PRFL_RMT_OPRA;  \
   facInfo.compType.pres = PRSNT_NODEF;        \
   facInfo.compType.val = FAC_COMP_INVOKE;     \
   facInfo.facComp.invokeCompLeg2.eh.pres = PRSNT_NODEF;             \
   facInfo.facComp.invokeCompLeg2.invCompInvId.pres = PRSNT_NODEF;   \
   facInfo.facComp.invokeCompLeg2.invCompInvId.val = 1;              \
   facInfo.facComp.invokeCompLeg2.invCompOperation.type.pres = PRSNT_NODEF;      \
   facInfo.facComp.invokeCompLeg2.invCompOperation.type.val = LOCAL_VALUE;       \
   facInfo.facComp.invokeCompLeg2.invCompOperation.u.locVal.pres = PRSNT_NODEF;  \
   facInfo.facComp.invokeCompLeg2.invCompOperation.u.locVal.val = DIVLEGINFO2;   \
   facInfo.facComp.invokeCompLeg2.divLegInfo2.eh.pres = PRSNT_NODEF;             \
   facInfo.facComp.invokeCompLeg2.divLegInfo2.divRsn.pres = PRSNT_NODEF;         \
   facInfo.facComp.invokeCompLeg2.divLegInfo2.divCnt.pres = PRSNT_NODEF;         \
}
/*FID 16970.0 ++*/
#define INITCCBSTREQ(facInfo)\
{ \
   facInfo.protPrfl.pres = PRSNT_NODEF;        \
   facInfo.protPrfl.val = PROT_PRFL_RMT_OPRA;  \
   facInfo.compType.pres = PRSNT_NODEF;        \
   facInfo.compType.val = FAC_COMP_INVOKE;     \
   facInfo.facComp.invokeCompCCBSTReq.eh.pres = PRSNT_NODEF;             \
   facInfo.facComp.invokeCompCCBSTReq.invCompInvId.pres = PRSNT_NODEF;   \
   facInfo.facComp.invokeCompCCBSTReq.invCompInvId.val = 1;              \
   facInfo.facComp.invokeCompCCBSTReq.invCompOperation.type.pres = PRSNT_NODEF;      \
   facInfo.facComp.invokeCompCCBSTReq.invCompOperation.type.val = GLOBAL_VALUE;       \
   SETCCBSTREQ(facInfo.facComp.invokeCompCCBSTReq.invCompOperation.u.glVal);\
}
#define INITCCBSTCALL(facInfo)\
{ \
   facInfo.protPrfl.pres = PRSNT_NODEF;        \
   facInfo.protPrfl.val = PROT_PRFL_RMT_OPRA;  \
   facInfo.compType.pres = PRSNT_NODEF;        \
   facInfo.compType.val = FAC_COMP_INVOKE;     \
   facInfo.facComp.invokeCompCCBSTRelated.eh.pres = PRSNT_NODEF;             \
   facInfo.facComp.invokeCompCCBSTRelated.invCompInvId.pres = PRSNT_NODEF;   \
   facInfo.facComp.invokeCompCCBSTRelated.invCompInvId.val = 1;              \
   facInfo.facComp.invokeCompCCBSTRelated.invCompOperation.type.pres = PRSNT_NODEF;      \
   facInfo.facComp.invokeCompCCBSTRelated.invCompOperation.type.val = GLOBAL_VALUE;       \
   SETCCBSTCALL(facInfo.facComp.invokeCompCCBSTRelated.invCompOperation.u.glVal);\
}

#define INITCCBSTSUSPEND(facInfo)\
{ \
   facInfo.protPrfl.pres = PRSNT_NODEF;        \
   facInfo.protPrfl.val = PROT_PRFL_RMT_OPRA;  \
   facInfo.compType.pres = PRSNT_NODEF;        \
   facInfo.compType.val = FAC_COMP_INVOKE;     \
   facInfo.facComp.invokeCompCCBSTRelated.eh.pres = PRSNT_NODEF;             \
   facInfo.facComp.invokeCompCCBSTRelated.invCompInvId.pres = PRSNT_NODEF;   \
   facInfo.facComp.invokeCompCCBSTRelated.invCompInvId.val = 1;              \
   facInfo.facComp.invokeCompCCBSTRelated.invCompOperation.type.pres = PRSNT_NODEF;      \
   facInfo.facComp.invokeCompCCBSTRelated.invCompOperation.type.val = GLOBAL_VALUE;       \
   SETCCBSTSUSPEND(facInfo.facComp.invokeCompCCBSTRelated.invCompOperation.u.glVal);\
}
#define INITCCBSTRESUME(facInfo)\
{ \
   facInfo.protPrfl.pres = PRSNT_NODEF;        \
   facInfo.protPrfl.val = PROT_PRFL_RMT_OPRA;  \
   facInfo.compType.pres = PRSNT_NODEF;        \
   facInfo.compType.val = FAC_COMP_INVOKE;     \
   facInfo.facComp.invokeCompCCBSTRelated.eh.pres = PRSNT_NODEF;             \
   facInfo.facComp.invokeCompCCBSTRelated.invCompInvId.pres = PRSNT_NODEF;   \
   facInfo.facComp.invokeCompCCBSTRelated.invCompInvId.val = 1;              \
   facInfo.facComp.invokeCompCCBSTRelated.invCompOperation.type.pres = PRSNT_NODEF;      \
   facInfo.facComp.invokeCompCCBSTRelated.invCompOperation.type.val = GLOBAL_VALUE;       \
   SETCCBSTRESUME(facInfo.facComp.invokeCompCCBSTRelated.invCompOperation.u.glVal);\
}
#define INITCCBSTREUSRFREE(facInfo)\
{ \
   facInfo.protPrfl.pres = PRSNT_NODEF;        \
   facInfo.protPrfl.val = PROT_PRFL_RMT_OPRA;  \
   facInfo.compType.pres = PRSNT_NODEF;        \
   facInfo.compType.val = FAC_COMP_INVOKE;     \
   facInfo.facComp.invokeCompCCBSTRelated.eh.pres = PRSNT_NODEF;             \
   facInfo.facComp.invokeCompCCBSTRelated.invCompInvId.pres = PRSNT_NODEF;   \
   facInfo.facComp.invokeCompCCBSTRelated.invCompInvId.val = 1;              \
   facInfo.facComp.invokeCompCCBSTRelated.invCompOperation.type.pres = PRSNT_NODEF;      \
   facInfo.facComp.invokeCompCCBSTRelated.invCompOperation.type.val = GLOBAL_VALUE;       \
   SETCCBSTREUSRFREE(facInfo.facComp.invokeCompCCBSTRelated.invCompOperation.u.glVal);\
}
#define INITCCBSTAVA(facInfo)\
{ \
   facInfo.protPrfl.pres = PRSNT_NODEF;        \
   facInfo.protPrfl.val = PROT_PRFL_RMT_OPRA;  \
   facInfo.compType.pres = PRSNT_NODEF;        \
   facInfo.compType.val = FAC_COMP_INVOKE;     \
   facInfo.facComp.invokeCompCCBSTRelated.eh.pres = PRSNT_NODEF;             \
   facInfo.facComp.invokeCompCCBSTRelated.invCompInvId.pres = PRSNT_NODEF;   \
   facInfo.facComp.invokeCompCCBSTRelated.invCompInvId.val = 1;              \
   facInfo.facComp.invokeCompCCBSTRelated.invCompOperation.type.pres = PRSNT_NODEF;      \
   facInfo.facComp.invokeCompCCBSTRelated.invCompOperation.type.val = GLOBAL_VALUE;       \
   SETCCBSTAVA(facInfo.facComp.invokeCompCCBSTRelated.invCompOperation.u.glVal);\
}
/*FID 16970.0 --*/
#define CC_FILLDFLTREDIRN(redirNmb)      \
{                                        \
redirNmb.eh.pres = PRSNT_NODEF;          \
redirNmb.nmbPlanId.pres = PRSNT_NODEF;   \
redirNmb.nmbPlanId.val = NP_UNK;         \
redirNmb.typeNmb.pres = PRSNT_NODEF;     \
redirNmb.typeNmb.val = TON_UNK;          \
redirNmb.presInd.pres = PRSNT_NODEF;     \
redirNmb.presInd.val = PI_NOTAVAIL;      \
redirNmb.screenInd.pres = PRSNT_NODEF;   \
redirNmb.screenInd.val = SI_USRNOTSCRN;  \
redirNmb.rsnRedirect.pres = PRSNT_NODEF; \
redirNmb.rsnRedirect.val = RR_UNKNOWN;   \
redirNmb.nmbDigits.pres = NOTPRSNT;      \
}
/* FID 16206.0 -*/
/* FID 15461.0 +*/
/* FID 16766.0 +*/
#define CC_EGRESS_GRMGC(con)   \
           ((con) &&  \
            ((con->icIntfcCb) && (con->icIntfcCb->ccTgCb) && \
            (con->icIntfcCb->ccTgCb->tgAtt.grInterMgc == TRUE)))
/* FID 16766.0 -*/
/* FID 15461.0 -*/

/* Bug 89343 fix the issue of wild pointer con->icEvnt due to following
 * reason: A new memory management framework was checked into 
 * mainline and BRANCH-9-0-0 via bug88779 as a part of GET-2 feature where
 * in memory allocated by an mtss(trillium) based application is returned
 * back to OS at certain times. Before this framework change the memory 
 * allocated by such applications via SGetSBuf never used to get returned 
 * back to OS.  Wherever in the code there used to be access of freed up 
 * memory, although illegal and may point to bad coding, would not result in core dump.  
 * Since the heap memory that is freed by applications is now returned back to OS faster, 
 * the bad code may result in faults more easily.
 */
#define CC_INIT_CON_ICEVNT_PTR(con) \
{\
   con->icEvnt = NULLP;\
}

/*FID 16696.0 +*/
#define CC_OVERLAPTOENBLOC_STARTED    0x01  /*indicate overlap to en bloc conversion is already started*/
#define CC_OVERLAPTOENBLOC_REQ_RECVED 0x02  /*indicate overlap to en bloc conversion req is received from RS*/
/*FID 16696.0 -*/

/*FID 16642.0 +*/
#define CC_NON_OAC                    0x00  /*indicate the call is not an operator assisted call*/
#define CC_OAC                        0x01  /*indicate the call is an operator assisted call*/
#define CC_OAC_OFRSENT                0x02  /*OFR has been sent for an operator assisted call*/
#define CC_OAC_OFRSENT_RECVONLY       0x03  /*receive the SDP with recvonly mode after OFR was sent*/
#define CC_OAC_OFRSENT_SENDRECV       0x04  /*recevie the SDP with sendrecv mode after OFR was sent and reverse hold*/
#define CC_OAC_CANSENT                0x05  /*CAN has been sent for an operator assisted call*/

#define CC_IS_CPC_OPERATOR(cgPtyCat) \
    ((cgPtyCat == CAT_OPLANGENG)|| \
     (cgPtyCat == CAT_OPLANGFR) || \
     (cgPtyCat == CAT_OPLANGGER) || \
     (cgPtyCat == CAT_OPLANGRUS) || \
     (cgPtyCat == CAT_OPLANGSP))

/*FID 16642.0 -*/

/* FID 16925.0 + */
#define CC_IS_CADSTAT_SUBFREE(bckCallInd) \
        ((bckCallInd.eh.pres) && \
         (bckCallInd.cadPtyStatInd.pres) && \
         (bckCallInd.cadPtyStatInd.val == CADSTAT_SUBFREE))
#define CC_IS_INBND_AVAIL(optBckCalInd) \
        ((optBckCalInd.eh.pres) && \
         (optBckCalInd.inbndInfoInd.pres) && \
         (optBckCalInd.inbndInfoInd.val == INBND_AVAIL))
#define CC_IS_INBANDINFOIW(sipPrflPtr) \
        ((sipPrflPtr) && \
         (sipPrflPtr->inbandInfoIw == LCC_SIP_INBND_INBANDAVIL))
/* FID 16925.0 - */
/* FID16883.0 + */
#define CC_SET_VBDFALLBACK_NEEDED(_conFlag, _vbdFallBackFlag)   (_conFlag |= _vbdFallBackFlag)
#define CC_IS_VBDFALLBACK_NEEDED(_conFlag, _vbdFallBackFlag)    (_conFlag  & _vbdFallBackFlag)
#define CC_RESET_VBDFALLBACK_NEEDED(_conFlag, _vbdFallBackFlag) (_conFlag &= ~_vbdFallBackFlag)
/* FID16883.0 */

/* Bug90648 + */
#define CC_IS_SIPT_IT(con) \
        ((CC_GET_PROTOCOL(con->ogProtType) == CC_SIPT) && \
         (con->ogIntfcCb) && \
         (con->ogIntfcCb->ccDDIOption.optCDDNSupported))
/* Bug90648 - */

/* Bug91440 + */
#define CC_PRINT_BCD_ADDR(oddEven, addrSig)\
{\
   U8 tmpAddrSig[(MF_SIZE_TKNSTR+1)*2];\
   for (i=0; i<addrSig.len; i++)\
   {\
      tmpAddrSig[2*i] = ccBcdToAscHex ((U8)(addrSig.val[i] & 0x0f));\
      tmpAddrSig[2*i + 1] = ccBcdToAscHex ((U8)((addrSig.val[i] >> 4) & 0x0f));\
   }\
   if (oddEven.val == NMB_ODD)\
   {\
      DP("BCD Address Signal: %.*s\n", 2*i-1, tmpAddrSig);\
   }\
   else\
   {\
      DP("BCD Address Signal: %.*s\n", 2*i, tmpAddrSig);\
   }\
}
/* Bug91440 - */
/* FID 15703.0 */
#define TERMIOISRC_TRKGRP 0
#define TERMIOISRC_CDPN   1
#define TERMIOISRC_MAX    2

/* FID 16367.0 + */
#define CC_IS_ISDN_CALLRRT(con)\
        (con && con->callRrtInfo && \
         (con->callRrtInfo->invokeCompCallRrt.eh.pres != NOTPRSNT) && \
         (con->callRrtInfo->invokeCompCallRrt.callRrt.eh.pres != NOTPRSNT))
  
/* FID 16706 BUG 91702 + */
#define CC_INGRESS_TG_SUPPORT_AOC_XML(con) \
(                                      \
    ((con->icIntfcCb != NULLP) &&      \
     (con->icIntfcCb->intfc.intfType == CC_IN_INTFC) && \
     (con->icIntfcCb->aocIsdn) \
    )?TRUE:FALSE                         \
)

#define CC_EGRESS_TG_SUPPORT_AOC_XML(con)\
( \
    ((con->ogIntfcCb != NULLP) &&      \
     (con->ogIntfcCb->ccTgCb != NULLP) &&    \
     (con->ogIntfcCb->ccTgCb->tgAtt.aoc) &&   \
     (con->ogIntfcCb->intfc.intfType == CC_SIP_INTFC) && \
     (con->ogIntfcCb->ccTgCb->tgAtt.SignalingType == TGP_SIGTYPE_SIP) && \
     (con->ogIntfcCb->ccTgCb->tgAtt.sipPrflPtr != NULL) && \
     (con->ogIntfcCb->ccTgCb->tgAtt.sipPrflPtr->aocXML) \
    )?TRUE:FALSE                         \
)

#define CC_BOTH_TG_SUPPORT_AOC_XML(con)         \
( \
    ((CC_INGRESS_TG_SUPPORT_AOC_XML(con)) &&    \
     (CC_EGRESS_TG_SUPPORT_AOC_XML(con))        \
    )?TRUE:FALSE                             \
)


/* Initialize the AOCDCurrency operation - chargeNotAvailable CHOICE */
#define INITAOCDCURRENCY(facInfo)\
{ \
   facInfo.protPrfl.pres = PRSNT_NODEF;        \
   facInfo.protPrfl.val = PROT_PRFL_RMT_OPRA;  \
   facInfo.compType.pres = PRSNT_NODEF;        \
   facInfo.compType.val = FAC_COMP_INVOKE;     \
   facInfo.facComp.invokeCompAOCDCurrency.eh.pres = PRSNT_NODEF;             \
   facInfo.facComp.invokeCompAOCDCurrency.invCompInvId.pres = PRSNT_NODEF;   \
   facInfo.facComp.invokeCompAOCDCurrency.invCompInvId.val = 1;              \
   facInfo.facComp.invokeCompAOCDCurrency.invCompOperation.type.pres = PRSNT_NODEF;             \
   facInfo.facComp.invokeCompAOCDCurrency.invCompOperation.type.val = LOCAL_VALUE;              \
   facInfo.facComp.invokeCompAOCDCurrency.invCompOperation.u.locVal.pres = PRSNT_NODEF;         \
   facInfo.facComp.invokeCompAOCDCurrency.invCompOperation.u.locVal.val = AOCDCURRENCY;         \
   facInfo.facComp.invokeCompAOCDCurrency.aOCDCurrency.type.pres = PRSNT_NODEF;                 \
   facInfo.facComp.invokeCompAOCDCurrency.aOCDCurrency.type.val = CH_AOCDCURRENCY_CHARGENOTAVAILABLE; \
   facInfo.facComp.invokeCompAOCDCurrency.aOCDCurrency.u.chargeNotAvailable.pres = PRSNT_NODEF;   \
}

/* Initialize the AOCDChargingUnit operation - chargeNotAvailable CHOICE */
#define INITAOCDCHARGINGUNIT(facInfo)\
{ \
   facInfo.protPrfl.pres = PRSNT_NODEF;        \
   facInfo.protPrfl.val = PROT_PRFL_RMT_OPRA;  \
   facInfo.compType.pres = PRSNT_NODEF;        \
   facInfo.compType.val = FAC_COMP_INVOKE;     \
   facInfo.facComp.invokeCompAOCDChargingUnit.eh.pres = PRSNT_NODEF;             \
   facInfo.facComp.invokeCompAOCDChargingUnit.invCompInvId.pres = PRSNT_NODEF;   \
   facInfo.facComp.invokeCompAOCDChargingUnit.invCompInvId.val = 1;              \
   facInfo.facComp.invokeCompAOCDChargingUnit.invCompOperation.type.pres = PRSNT_NODEF;         \
   facInfo.facComp.invokeCompAOCDChargingUnit.invCompOperation.type.val = LOCAL_VALUE;          \
   facInfo.facComp.invokeCompAOCDChargingUnit.invCompOperation.u.locVal.pres = PRSNT_NODEF;     \
   facInfo.facComp.invokeCompAOCDChargingUnit.invCompOperation.u.locVal.val = AOCDCHARGINGUNIT; \
   facInfo.facComp.invokeCompAOCDChargingUnit.aOCDChargingUnit.type.pres = PRSNT_NODEF;         \
   facInfo.facComp.invokeCompAOCDChargingUnit.aOCDChargingUnit.type.val = CH_AOCDCHARGINGUNIT_CHARGENOTAVAILABLE; \
   facInfo.facComp.invokeCompAOCDChargingUnit.aOCDChargingUnit.u.chargeNotAvailable.pres = PRSNT_NODEF; \
}

/* Initialize the AOCECurrency operation - chargeNotAvailable CHOICE */
#define INITAOCECURRENCY(facInfo)\
{ \
   facInfo.protPrfl.pres = PRSNT_NODEF;        \
   facInfo.protPrfl.val = PROT_PRFL_RMT_OPRA;  \
   facInfo.compType.pres = PRSNT_NODEF;        \
   facInfo.compType.val = FAC_COMP_INVOKE;     \
   facInfo.facComp.invokeCompAOCECurrency.eh.pres = PRSNT_NODEF;             \
   facInfo.facComp.invokeCompAOCECurrency.invCompInvId.pres = PRSNT_NODEF;   \
   facInfo.facComp.invokeCompAOCECurrency.invCompInvId.val = 1;              \
   facInfo.facComp.invokeCompAOCECurrency.invCompOperation.type.pres = PRSNT_NODEF;             \
   facInfo.facComp.invokeCompAOCECurrency.invCompOperation.type.val = LOCAL_VALUE;              \
   facInfo.facComp.invokeCompAOCECurrency.invCompOperation.u.locVal.pres = PRSNT_NODEF;         \
   facInfo.facComp.invokeCompAOCECurrency.invCompOperation.u.locVal.val = AOCECURRENCY;         \
   facInfo.facComp.invokeCompAOCECurrency.aOCECurrency.type.pres = PRSNT_NODEF;                 \
   facInfo.facComp.invokeCompAOCECurrency.aOCECurrency.type.val = CH_AOCECURRENCY_CHARGENOTAVAILABLE; \
   facInfo.facComp.invokeCompAOCECurrency.aOCECurrency.u.chargeNotAvailable.pres = PRSNT_NODEF;   \
}

/* Initialize the AOCEChargingUnit operation - chargeNotAvailable */
#define INITAOCECHARGINGUNIT(facInfo)\
{ \
   facInfo.protPrfl.pres = PRSNT_NODEF;        \
   facInfo.protPrfl.val = PROT_PRFL_RMT_OPRA;  \
   facInfo.compType.pres = PRSNT_NODEF;        \
   facInfo.compType.val = FAC_COMP_INVOKE;     \
   facInfo.facComp.invokeCompAOCEChargingUnit.eh.pres = PRSNT_NODEF;             \
   facInfo.facComp.invokeCompAOCEChargingUnit.invCompInvId.pres = PRSNT_NODEF;   \
   facInfo.facComp.invokeCompAOCEChargingUnit.invCompInvId.val = 1;              \
   facInfo.facComp.invokeCompAOCEChargingUnit.invCompOperation.type.pres = PRSNT_NODEF;         \
   facInfo.facComp.invokeCompAOCEChargingUnit.invCompOperation.type.val = LOCAL_VALUE;          \
   facInfo.facComp.invokeCompAOCEChargingUnit.invCompOperation.u.locVal.pres = PRSNT_NODEF;     \
   facInfo.facComp.invokeCompAOCEChargingUnit.invCompOperation.u.locVal.val = AOCECHARGINGUNIT; \
   facInfo.facComp.invokeCompAOCEChargingUnit.aOCEChargingUnit.type.pres = PRSNT_NODEF;     \
   facInfo.facComp.invokeCompAOCEChargingUnit.aOCEChargingUnit.type.val = CH_AOCECHARGINGUNIT_CHARGENOTAVAILABLE; \
   facInfo.facComp.invokeCompAOCEChargingUnit.aOCEChargingUnit.u.chargeNotAvailable.pres = PRSNT_NODEF; \
}

/* FID 16706 BUG 91702 - */

#define CC_IS_STEROMONO_CALL(con) \
         (con && con->icConCurBcm)
/* FID 15068.0 + */
#define PRECND_OTHER 0x0
#define PRECND_QOS 0x01

#define PRECND_STATUS_E2E 0x0
#define PRECND_STATUS_LOCRMT 0x01

#define PRECND_SHIFT_NUM 0
#define PRECND_MASK 0x03
#define PRECND_STATUS_SHIFT_NUM 2
#define PRECND_STATUS_MASK 0x0c

#define GET_PRECND_TYPE(var) (((var)&PRECND_MASK)>>PRECND_SHIFT_NUM)
#define SET_PRECND_TYPE(var,type) \
  (var) = ((var)&~PRECND_MASK)|(((type)<<PRECND_SHIFT_NUM)&PRECND_MASK)

#define GET_PRECND_STATUS_TYPE(var) (((var)&PRECND_STATUS_MASK)>>PRECND_STATUS_SHIFT_NUM)
#define SET_PRECND_STATUS_TYPE(var,type) \
  (var) = ((var)&~PRECND_STATUS_MASK)|(((type)<<PRECND_STATUS_SHIFT_NUM)&PRECND_STATUS_MASK)

#define PRECND_DIR_NONE 0x04
#define PRECND_DIR_SEND 0x05
#define PRECND_DIR_RECV 0x06
#define PRECND_DIR_SENDRECV 0x07

#define PRECND_CUR_LOCAL_DIR_SHIFT_NUM 4
#define PRECND_CUR_LOCAL_DIR_MASK 0x70
#define PRECND_CUR_REMOTE_DIR_SHIFT_NUM 7
#define PRECND_CUR_REMOTE_DIR_MASK 0x380
#define PRECND_CONF_DIR_SHIFT_NUM 10
#define PRECND_CONF_DIR_MASK 0x1c00
#define PRECND_OFFER_ATTR_SHIFT_NUM 13
#define PRECND_OFFER_ATTR_MASK 0x2000
#define PRECND_OFFER_ATTR 0x1

#define GET_PRECND_CUR_LOCAL_DIR(var) (((var)&PRECND_CUR_LOCAL_DIR_MASK)>>PRECND_CUR_LOCAL_DIR_SHIFT_NUM)
#define SET_PRECND_CUR_LOCAL_DIR(var,dir) \
  (var) = ((var)&~PRECND_CUR_LOCAL_DIR_MASK)|(((dir)<<PRECND_CUR_LOCAL_DIR_SHIFT_NUM)&PRECND_CUR_LOCAL_DIR_MASK)

#define GET_PRECND_CUR_REMOTE_DIR(var) (((var)&PRECND_CUR_REMOTE_DIR_MASK)>>PRECND_CUR_REMOTE_DIR_SHIFT_NUM)
#define SET_PRECND_CUR_REMOTE_DIR(var,dir) \
   (var) = ((var)&~PRECND_CUR_REMOTE_DIR_MASK)|(((dir)<<PRECND_CUR_REMOTE_DIR_SHIFT_NUM)&PRECND_CUR_REMOTE_DIR_MASK);

#define GET_PRECND_CONF_DIR(var) (((var)&PRECND_CONF_DIR_MASK)>>PRECND_CONF_DIR_SHIFT_NUM)
#define SET_PRECND_CONF_DIR(var,dir) \
   (var) = ((var)&~PRECND_CONF_DIR_MASK)|(((dir)<<PRECND_CONF_DIR_SHIFT_NUM)&PRECND_CONF_DIR_MASK)

#define IS_PRECND_OFFER_ATTR(var) ((((var)&PRECND_OFFER_ATTR_MASK)>>PRECND_OFFER_ATTR_SHIFT_NUM)==PRECND_OFFER_ATTR)
#define SET_PRECND_OFFER_ATTR(var) \
  (var) = ((var)&~PRECND_OFFER_ATTR_MASK)|(((PRECND_OFFER_ATTR)<<PRECND_OFFER_ATTR_SHIFT_NUM)&PRECND_OFFER_ATTR_MASK)
#define CLEAR_PRECND_OFFER_ATTR(var) (var) = ((var)&~PRECND_OFFER_ATTR_MASK)


/*For desire line*/
#define PRECND_STRENGTH_UNKNOWN 0x01
//#define PRECND_STRENGTH_FAILURE 0x03
#define PRECND_STRENGTH_NONE 0x02
#define PRECND_STRENGTH_OPTIONAL 0x04
#define PRECND_STRENGTH_MANDATORY 0x08

#define PRECND_LOCAL_SEND_STRENGTH_SHIFT_NUM 0
#define PRECND_LOCAL_SEND_STRENGTH_MASK 0x0f
#define PRECND_LOCAL_RECV_STRENGTH_SHIFT_NUM 4
#define PRECND_LOCAL_RECV_STRENGTH_MASK 0xf0
#define PRECND_REMOTE_SEND_STRENGTH_SHIFT_NUM 8
#define PRECND_REMOTE_SEND_STRENGTH_MASK 0x0f00
#define PRECND_REMOTE_RECV_STRENGTH_SHIFT_NUM 12
#define PRECND_REMOTE_RECV_STRENGTH_MASK 0xf000

#define GET_PRECND_DESIRE_LOCAL_SEND_STRENGTH(var) (((var)&PRECND_LOCAL_SEND_STRENGTH_MASK)>>PRECND_LOCAL_SEND_STRENGTH_SHIFT_NUM)
#define SET_RRECND_DESIRE_LOCAL_SEND_STRENGTH(var,strength) \
  (var) = ((var)&~PRECND_LOCAL_SEND_STRENGTH_MASK)|(((strength)<<PRECND_LOCAL_SEND_STRENGTH_SHIFT_NUM)&PRECND_LOCAL_SEND_STRENGTH_MASK)

#define GET_PRECND_DESIRE_LOCAL_RECV_STRENGTH(var) (((var)&PRECND_LOCAL_RECV_STRENGTH_MASK)>>PRECND_LOCAL_RECV_STRENGTH_SHIFT_NUM)
#define SET_RRECND_DESIRE_LOCAL_RECV_STRENGTH(var,strength) \
  (var) = ((var)&~PRECND_LOCAL_RECV_STRENGTH_MASK)|(((strength)<<PRECND_LOCAL_RECV_STRENGTH_SHIFT_NUM)&PRECND_LOCAL_RECV_STRENGTH_MASK)

#define GET_PRECND_DESIRE_REMOTE_SEND_STRENGTH(var) (((var)&PRECND_REMOTE_SEND_STRENGTH_MASK)>>PRECND_REMOTE_SEND_STRENGTH_SHIFT_NUM)
#define SET_RRECND_DESIRE_REMOTE_SEND_STRENGTH(var,strength) \
  (var) = ((var)&~PRECND_REMOTE_SEND_STRENGTH_MASK)|(((strength)<<PRECND_REMOTE_SEND_STRENGTH_SHIFT_NUM)&PRECND_REMOTE_SEND_STRENGTH_MASK)

#define GET_PRECND_DESIRE_REMOTE_RECV_STRENGTH(var) (((var)&PRECND_REMOTE_RECV_STRENGTH_MASK)>>PRECND_REMOTE_RECV_STRENGTH_SHIFT_NUM)
#define SET_RRECND_DESIRE_REMOTE_RECV_STRENGTH(var,strength) \
  (var) = ((var)&~PRECND_REMOTE_RECV_STRENGTH_MASK)|(((strength)<<PRECND_REMOTE_RECV_STRENGTH_SHIFT_NUM)&PRECND_REMOTE_RECV_STRENGTH_MASK)

#define CC_PRECND_HAS_MANDATORY(var)  ((var)& \
  ((PRECND_STRENGTH_MANDATORY << PRECND_LOCAL_SEND_STRENGTH_SHIFT_NUM)| \
   (PRECND_STRENGTH_MANDATORY << PRECND_LOCAL_RECV_STRENGTH_SHIFT_NUM)| \
   (PRECND_STRENGTH_MANDATORY << PRECND_REMOTE_SEND_STRENGTH_SHIFT_NUM)| \
   (PRECND_STRENGTH_MANDATORY << PRECND_REMOTE_RECV_STRENGTH_SHIFT_NUM)))

#define PRECND_STATE_COMPLETED 0x01

#define PRECND_STATE_SHIFT_NUM 7
#define PRECND_STATE_MASK 0x80

#define GET_PRECND_STATE(var) (((var)&PRECND_STATE_MASK)>>PRECND_STATE_SHIFT_NUM)
#define SET_PRECND_STATE(var,state) \
  (var) = ((var)&~PRECND_STATE_MASK)|(((state)<<PRECND_STATE_SHIFT_NUM)&PRECND_STATE_MASK)

#define IS_PRECND_COMPLETED(var) (GET_PRECND_STATE(var)==PRECND_STATE_COMPLETED)
#define SET_PRECND_COMPLETED(var) SET_PRECND_STATE(var,PRECND_STATE_COMPLETED)
#define CLEAR_PRECND_COMPLETED(var) SET_PRECND_STATE(var,0x0)

#define PRECND_OG18XRCVD 0x01
#define PRECND_OG18XRCVD_SHIFT_NUM 0 
#define PRECND_OG18XRCVD_MASK 0x01

#define IS_PRECND_OG18XRCVD(var) ((((var)&PRECND_OG18XRCVD_MASK)>>PRECND_OG18XRCVD_SHIFT_NUM)==PRECND_OG18XRCVD)
#define SET_PRECND_OG18XRCVD(var) \
  (var) = ((var)&~PRECND_OG18XRCVD_MASK)|((PRECND_OG18XRCVD)<<PRECND_OG18XRCVD_SHIFT_NUM)
#define CLEAR_PRECND_OG18XRCVD(var) (var) = ((var)&~PRECND_OG18XRCVD_MASK)

#define PRECND_SIP2SIPREROUTE 0x01
#define PRECND_SIP2SIPREROUTE_SHIFT_NUM 1
#define PRECND_SIP2SIPREROUTE_MASK 0x02

#define IS_PRECND_SIP2SIPREROUTE(var) ((((var)& PRECND_SIP2SIPREROUTE_MASK)>>PRECND_SIP2SIPREROUTE_SHIFT_NUM)==PRECND_SIP2SIPREROUTE)
#define SET_PRECND_SIP2SIPREROUTE(var) \
  (var) = ((var)&~PRECND_SIP2SIPREROUTE_MASK)|((PRECND_SIP2SIPREROUTE)<<PRECND_SIP2SIPREROUTE_SHIFT_NUM)
#define CLEAR_PRECND_SIP2SIPREROUTE(var) (var) = ((var)&~PRECND_SIP2SIPREROUTE_MASK)

#define PRECND_INMGACTION 0x01
#define PRECND_INMGACTION_SHIFT_NUM 2 
#define PRECND_INMGACTION_MASK 0x04

#define IS_PRECND_INMGACTION(var) ((((var)&PRECND_INMGACTION_MASK)>>PRECND_INMGACTION_SHIFT_NUM)==PRECND_INMGACTION)
#define SET_PRECND_INMGACTION(var) \
  (var) = ((var)&~PRECND_INMGACTION_MASK)|((PRECND_INMGACTION)<<PRECND_INMGACTION_SHIFT_NUM)
#define CLEAR_PRECND_INMGACTION(var) (var) = ((var)&~PRECND_INMGACTION_MASK) 

#define PRECND_RMV_ISUP   0x1
#define PRECND_RMV_ISUP_SHIFT_NUM 3 
#define PRECND_RMV_ISUP_MASK 0x08
#define IS_PRECND_RMV_ISUP(var) ((((var)&PRECND_RMV_ISUP_MASK)>>PRECND_RMV_ISUP_SHIFT_NUM)==PRECND_RMV_ISUP)
#define SET_PRECND_RMV_ISUP(var) \
  (var) = ((var)&~PRECND_RMV_ISUP_MASK)|((PRECND_RMV_ISUP)<<PRECND_RMV_ISUP_SHIFT_NUM)
#define CLEAR_PRECND_RMV_ISUP(var) (var) = ((var)&~PRECND_RMV_ISUP_MASK) 

#define PRECND_INACTION 0x01
#define PRECND_INACTION_SHIFT_NUM 4 
#define PRECND_INACTION_MASK 0x10

#define IS_PRECND_INACTION(var) ((((var)&PRECND_INACTION_MASK)>>PRECND_INACTION_SHIFT_NUM)==PRECND_INACTION)
#define SET_PRECND_INACTION(var) \
  (var) = ((var)&~PRECND_INACTION_MASK)|((PRECND_INACTION)<<PRECND_INACTION_SHIFT_NUM)
#define CLEAR_PRECND_INACTION(var) (var) = ((var)&~PRECND_INACTION_MASK) 

/* FID17373.0 */
#define PRECND_181_NON_PRECND 0x01
#define PRECND_181_NON_PRECND_SHIFT_NUM 5 
#define PRECND_181_NON_PRECND_MASK 0x20

#define IS_PRECND_181_NON_PRECND(var) ((((var)&PRECND_181_NON_PRECND_MASK)>>PRECND_181_NON_PRECND_SHIFT_NUM)==PRECND_181_NON_PRECND)
#define SET_PRECND_181_NON_PRECND(var) \
  (var) = ((var)&~PRECND_181_NON_PRECND_MASK)|((PRECND_181_NON_PRECND)<<PRECND_181_NON_PRECND_SHIFT_NUM)
#define CLEAR_PRECND_181_NON_PRECND(var) (var) = ((var)&~PRECND_181_NON_PRECND_MASK) 

/* BUG93330 inter-MG to provide RBT */
#define PRECND_RBT_MULTI_MG 0x01
#define PRECND_RBT_MULTI_MG_SHIFT_NUM 6 
#define PRECND_RBT_MULTI_MG_MASK 0x40

#define IS_PRECND_RBT_MULTI_MG(var) ((((var)&PRECND_RBT_MULTI_MG_MASK)>>PRECND_RBT_MULTI_MG_SHIFT_NUM)==PRECND_RBT_MULTI_MG)
#define SET_PRECND_RBT_MULTI_MG(var) \
  (var) = ((var)&~PRECND_RBT_MULTI_MG_MASK)|((PRECND_RBT_MULTI_MG)<<PRECND_RBT_MULTI_MG_SHIFT_NUM)
#define CLEAR_PRECND_RBT_MULTI_MG(var) (var) = ((var)&~PRECND_RBT_MULTI_MG_MASK) 

/* FID 15068.0 + */

/* + FID 17373.0 */
#define PRECND_FLAG_PRACK_SENT          0x0002
#define PRECND_FLAG_PRACK_RSP_RECVED    0x0004
#define PRECND_FLAG_UPDT_SENT           0x0008
#define PRECND_FLAG_NO_UPDT             0x0010
#define PRECND_FLAG_181_BUFFERED        0x0020

#define PRECND_SET_CALL_FLAG2(_precndCb, _flag) (_precndCb->flag2 |= _flag)
#define PRECND_CLR_CALL_FLAG2(_precndCb, _flag) (_precndCb->flag2 &= ~_flag)
#define PRECND_IS_CALL_FLAG2(_precndCb, _flag) (_precndCb->flag2 & _flag)

#define CC_IS_COT_RCVD_EXT_BICC(CON) \
  ((CON) && (CC_GET_PROTOCOL((CON)->icProtType) == CC_EXT_BICC) &&\
   ((CON)->cotBlock.contType == CC_IAM_INITIATED) && \
   ((CON)->cotBlock.contResult == CONT_CHKSUCC))

#define CC_SET_COT_RCVD_EXT_BICC(CON) \
  (CON)->cotBlock.contResult = CONT_CHKSUCC
/* - FID 17373.0 */

#define CC_IS_CUST_SET(sip, flag) \
        ((sip->custFlag) & flag)

/*FID16690.0++*/
#define CC_CHARGEBCIMAP_NULL 0
#define CC_CHARGEBCIMAP_ACCEPT 1
#define CC_CHARGEBCIMAP_ACCEPTNOCHARGE 2
#define CC_CHARGEBCIMAP_ALWAYS 3
#define CC_CHARGEBCIMAP_ALWAYSNOCHARGE 4

#define CC_CHRG_ANM  1
#define CC_NOCHRG_ANM 2
#define CC_CHRG_200OK 3
#define CC_NOCHRG_200OK 4
#define CC_CHRG_ACM  5
#define CC_NOCHRG_ACM 6

/* + FID 16970.0 + */
#define CCBS_ISDN_STATE_CHANGE(ccbsIsdnCallInfo, newState)                                                        \
{                                                                                                    \
    CCDP(AIN_DBG_LEVEL_0, "CCBSISDNFSM: State:%2d->%2d\n",(int) (ccbsIsdnCallInfo)->ccbsIsdnState, (int)newState);\
    (ccbsIsdnCallInfo)->ccbsIsdnState = (newState);                                                               \
}
/* save state before suspend */
#define CCBS_ISDN_SAVE_STATE(ccbsIsdnCallInfo)                                                \
{                                                                                              \
    CCDP(AIN_DBG_LEVEL_0, "CCBSISDNFSM: State:%2d\n",(int) (ccbsIsdnCallInfo)->ccbsIsdnState); \
    (ccbsIsdnCallInfo)->stateBefSusOrResu = (ccbsIsdnCallInfo)->ccbsIsdnState;                   \
}
/* - FID 16970.0 - */
#define CC_CHRG_18X  7
#define CC_NOCHRG_18X 8
/*FID16690.0--*/

/* FID 16465.0 */
#define DTMF_INFO_RECV 0x01
#define DTMF_INFO_SEND 0x02

#define DTMF_TONE_ID_MASK 0xFFFF
#define DTMF_INFO_DURATION  100   /* default value to 100 ms */

#define CC_DTMF_INFO_INGRESS(CON) ((((CON)->dtmfInfo) >> 8) & 0xff)
#define CC_DTMF_INFO_EGRESS(CON) (((CON)->dtmfInfo) & 0xff)

#define CC_IS_DTMF_INFO_CALL(CON) ((CON) && ((CON)->dtmfInfo != 0))

#define DTMF_INFO_DD_ARM_INGRESS 0x01
#define DD_ARM_INGRESS_SHIFT_NUM 10
#define DD_ARM_INGRESS_MASK 0x0400

#define DTMF_INFO_DD_ARM_EGRESS 0x01
#define DD_ARM_EGRESS_SHIFT_NUM 2
#define DD_ARM_EGRESS_MASK 0x04

#define IS_DTMF_INFO_INGRESS_ARM(var) ((((var)&DD_ARM_INGRESS_MASK)>>DD_ARM_INGRESS_SHIFT_NUM)==DTMF_INFO_DD_ARM_INGRESS)
#define SET_DTMF_INFO_INGRESS_ARM(var) \
  (var) = ((var)&~DD_ARM_INGRESS_MASK)|((DTMF_INFO_DD_ARM_INGRESS)<<DD_ARM_INGRESS_SHIFT_NUM)
#define CLEAR_DTMF_INFO_INGRESS_ARM(var) (var) = ((var)&~DD_ARM_INGRESS_MASK)

#define IS_DTMF_INFO_EGRESS_ARM(var) ((((var)&DD_ARM_EGRESS_MASK)>>DD_ARM_EGRESS_SHIFT_NUM)==DTMF_INFO_DD_ARM_EGRESS)
#define SET_DTMF_INFO_EGRESS_ARM(var) \
  (var) = ((var)&~DD_ARM_EGRESS_MASK)|((DTMF_INFO_DD_ARM_EGRESS)<<DD_ARM_EGRESS_SHIFT_NUM)
#define CLEAR_DTMF_INFO_EGRESS_ARM(var) (var) = ((var)&~DD_ARM_EGRESS_MASK)


#define DTMF_INFO_GEN_INGRESS 0x01
#define GEN_INGRESS_SHIFT_NUM 11
#define GEN_INGRESS_MASK 0x0800

#define DTMF_INFO_GEN_EGRESS 0x01
#define GEN_EGRESS_SHIFT_NUM 3
#define GEN_EGRESS_MASK 0x08

#define IS_DTMF_INFO_INGRESS_GEN(var) ((((var)&GEN_INGRESS_MASK)>>GEN_INGRESS_SHIFT_NUM)==DTMF_INFO_GEN_INGRESS)
#define SET_DTMF_INFO_INGRESS_GEN(var) \
  (var) = ((var)&~GEN_INGRESS_MASK)|((DTMF_INFO_GEN_INGRESS)<<GEN_INGRESS_SHIFT_NUM)
#define CLEAR_DTMF_INFO_INGRESS_GEN(var) (var) = ((var)&~GEN_INGRESS_MASK)

#define IS_DTMF_INFO_EGRESS_GEN(var) ((((var)&GEN_EGRESS_MASK)>>GEN_EGRESS_SHIFT_NUM)==DTMF_INFO_GEN_EGRESS)
#define SET_DTMF_INFO_EGRESS_GEN(var) \
  (var) = ((var)&~GEN_EGRESS_MASK)|((DTMF_INFO_GEN_EGRESS)<<GEN_EGRESS_SHIFT_NUM)
#define CLEAR_DTMF_INFO_EGRESS_GEN(var) (var) = ((var)&~GEN_EGRESS_MASK)

#define DTMF_INFO_DET_INGRESS 0x01
#define DET_INGRESS_SHIFT_NUM 12
#define DET_INGRESS_MASK 0x1000

#define DTMF_INFO_DET_EGRESS 0x01
#define DET_EGRESS_SHIFT_NUM 4
#define DET_EGRESS_MASK 0x10

#define IS_DTMF_INFO_INGRESS_DET(var) ((((var)&DET_INGRESS_MASK)>>DET_INGRESS_SHIFT_NUM)==DTMF_INFO_DET_INGRESS)
#define SET_DTMF_INFO_INGRESS_DET(var) \
  (var) = ((var)&~DET_INGRESS_MASK)|((DTMF_INFO_DET_INGRESS)<<DET_INGRESS_SHIFT_NUM)
#define CLEAR_DTMF_INFO_INGRESS_DET(var) (var) = ((var)&~DET_INGRESS_MASK)

#define IS_DTMF_INFO_EGRESS_DET(var) ((((var)&DET_EGRESS_MASK)>>DET_EGRESS_SHIFT_NUM)==DTMF_INFO_DET_EGRESS)
#define SET_DTMF_INFO_EGRESS_DET(var) \
  (var) = ((var)&~DET_EGRESS_MASK)|((DTMF_INFO_DET_EGRESS)<<DET_EGRESS_SHIFT_NUM)
#define CLEAR_DTMF_INFO_EGRESS_DET(var) (var) = ((var)&~DET_EGRESS_MASK)

/* FID 17174 Annc Support via External MRF + */

#define HASH_KEY_4_MRFTBL(listId, entryId, key) \
{                                                \
  key = listId;                                  \
  key = key << 16;                               \
  key = key | entryId;                         \
}
#define MRF_CALL 0x01
#define MRF_PLAYING_ANNC 0x02
#define MRF_183_ANNC 0x04
#define MRF_200_ANNC 0x08
#define MRF_SEND_ACK 0x10


#define CC_SET_MRF(_conFlag, _mrfFlag)   (_conFlag |= _mrfFlag)
#define CC_IS_MRF(_conFlag, _mrfFlag)    (_conFlag  & _mrfFlag)
#define CC_RESET_MRF(_conFlag, _mrfFlag) (_conFlag &= ~_mrfFlag)

/* FID 17174 Annc Support via External MRF - */

/* FID 14343.6 support 2 prfl-cccip for each LI account + */
#define HASH_KEY_4_CCCIP(mdId, prflId, key)\
{                                          \
  key = mdId;                              \
  key = key << 8;                          \
  key = key | prflId;                      \
}
/* FID 14343.6 support 2 prfl-cccip for each LI account - */

/* FID17378.0 */
#define CC_IS_ACM_CPG_WITH_INBAND(con, evntType, ccCnStEvnt) \
        (((con)->direction == OUTTOINC) &&\
         ((con)->ogIntfcCb) && \
         ((CC_GET_PROTOCOL((con)->ogIntfcCb->protType) == CC_SI) ||  \
          (CC_GET_PROTOCOL((con)->ogIntfcCb->protType) == CC_EXT_BICC))&&\
       ((((evntType) == CC_ET_ACM) &&\
         ((ccCnStEvnt)->m.siCnStEvnt.bckCallInd.eh.pres) &&\
         ((ccCnStEvnt)->m.siCnStEvnt.bckCallInd.cadPtyStatInd.pres) &&\
         ((ccCnStEvnt)->m.siCnStEvnt.bckCallInd.cadPtyStatInd.val == CADSTAT_NOIND) &&\
         (ccIfInbandInfoAvail(&((ccCnStEvnt)->m.siCnStEvnt), (con)->ogIntfcCb->protType))) ||\
        (((evntType) == CC_ET_CPG) &&\
         ((ccCnStEvnt)->m.siCnStEvnt.evntInfo.eh.pres) &&\
         ((ccCnStEvnt)->m.siCnStEvnt.evntInfo.evntInd.pres) &&\
         ((((ccCnStEvnt)->m.siCnStEvnt.evntInfo.evntInd.val == EV_PROGRESS) &&\
         (ccIfInbandInfoAvail(&((ccCnStEvnt)->m.siCnStEvnt), (con)->ogIntfcCb->protType))) ||\
         ((ccCnStEvnt)->m.siCnStEvnt.evntInfo.evntInd.val == EV_INBAND)))))
#define CC_IS_IC_ICS_CALL(con)\
        (CCT_IS_SIP_FEATURE_CAPS_SUPPORTED((con)->icSipMethodSupMask) &&\
         ((con)->ogIntfcCb) &&\
         (CC_GET_PROTOCOL((con)->ogIntfcCb->protType) == CC_SI || \
          CC_GET_PROTOCOL((con)->ogIntfcCb->protType) == CC_EXT_BICC))
#define CC_IS_OG_ICS_CALL(con)\
        (CCT_IS_SIP_FEATURE_CAPS_SUPPORTED((con)->ogSipMethodSupMask) &&\
         ((con)->icIntfcCb) &&\
         (CC_GET_PROTOCOL((con)->icIntfcCb->protType) == CC_SI || \
          CC_GET_PROTOCOL((con)->icIntfcCb->protType) == CC_EXT_BICC))

/* FID17434.0 */
#define ISDN_SIPHDRCOLPMAP_NONE                  0
#define ISDN_SIPHDRCOLPMAP_MAP                   1
#define ISDN_SIPHDRCOLPMAP_MAPSCRNORIG           2
#define ISDN_SIPHDRCOLPMAP_MAPSCRNTERM           3

/* FID17066.0 */
#define CC_SRTP_CRYPTO_INFO_MASK                 1
#define CC_SET_SRTP_CRYPTO_INFO_MASK(mask, streamId) \
    ((mask) |= (CC_SRTP_CRYPTO_INFO_MASK << (streamId)))
#define CC_IS_SRTP_CRYPTO_INFO(mask, streamId)       \
    ((mask) & (CC_SRTP_CRYPTO_INFO_MASK << (streamId)))
/********************************************************************30**
  
         End of file: cc.h 1.5  -  12/21/99 09:49:59
  
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
1.3+         cc001.13 mg   1. Added macro CCGET_INTFCKEY for generation 
                              of the interface key.
             cc001.13 zy   2. Changes for supporting ISUP Q.767, ETSI.
                              ISDN Q.SIG, NI2 interworking.

1.4          ---      mg   1. Changes for ICC 1.4 release.
1.5          ---      zy   1. Changes for ICC 1.5 release.
             cc007.15 zy   1. Add defines for CCIGNORE_CAUSE_MAP. 
             cc014.15 zy   1. Adding support for ISUP ANS92-ITU92 
                              interworking
             cc018.15 zy   1. Adding define of INV_IF_SWTCH.
             cc019.15 tej  1. Added the mapping index to map CALLPROC
                              message.
             cc021.15 zy   1. Adding support for ISUP ANS92-ITU Q.767
                              interworking.
             cc028.15 zy   1. Adding boundary check in computing observation
                              index flag.
1.6          ---      ak   1. New Macro CCOFFSETOF.
1.7          ---      ak   1. Added few #define for AIN
1.8          ---      ak   1. Added new #define for AIN UPDATE_OPIC/TPIC.
*********************************************************************91*/
