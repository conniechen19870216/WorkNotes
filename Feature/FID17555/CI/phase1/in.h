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
  
     Name:     Q.93x
  
     Type:     C include file
  
     Desc:     Defines required by the Q.93x.
  
     File:     in.h

     Sid:      in.h 3.7  -  09/29/99 14:26:52
  
     Prg:      na
  
*********************************************************************21*/
  
#ifndef __INH__
#define __INH__
  
#include "git_class.h"  /* gitDebugPrint prototype */

/************************************************************************
                    DEFINES
************************************************************************/

/* global defines */
/* TELICA-MCAO-11/05/01: define num screen for CD/CG PTYNUM */
#define IN_NUMSCREEN 1          /* screen calling/called party number in ISDN */
/* TELICA-MCAO-11/13/01: add restart logic for DMS100P */
#define DMS100P_RST              /* Restart solution for DMS100P variant */

#define MAX_SRVREQ_CNT  1        /* Max Number of Service Requests sent */
#define MAX_SRVBCHANRQ_CNT  2    /* Max Number of B Channel Service Requests*/
#define IN_MAX_INTRETRY     2    /* Max Number of Bind Requests sent */


#define TQNUMENT         512      /* timing queue number of entries */
#define MOD64            511      /* modulo 64 mask */

#define MAX_SEGS          8       /* maximum number of segments */
#define MAX_HDR           8       /* maximum header */
#define LEN_SEGIE         4       /* length of segment information element */
#define FRST_SEG          0x80    /* first segment */

#define MAXENTPROC        10      /* maximum entries per process */
#if 0 /* deletion : in014.310 */
#define MAXBCHTIMER       2       /* maximum number of simultaneous b chan 
                                     timers */
#else /* addition : in014.310 */
#define MAXBCHTIMER       3       /* maximum number of simultaneous b chan 
                                     timers */
#endif /* change : in014.310 */
#define NOTUSED           0       /* not used */
#define MAXSIMTIMER       4       /* maximum number of simultaneous timers */
#define MAXDCHANCON       16      /* maximum D channel connections */
#define MAXSEGCNT         0x7FFFFFFF   /* maximum segment count */

#define CNTR_16           16      /* counter 16 */
#define CNTR_32           32      /* counter 32 */
#define CNTR_256          256     /* counter 256 */
#define MAXREPTIMES       1       /* maximum restarts of timer T320 */

#define TSAP              1       /* Transport SAP Type */
#define DLSAP             2       /* Data Link SAP Type */
#define LINKSAP           3       /* Link SAP Type */

#define BRDCST_CES     0x7f       /* broadcast ces */
#define CONN_CES          0       /* connection ces */
 
#define ORIGINUNK         0       /* unknown */
#define ORIGINATOR        1       /* originator */
#define DESTINATION       2       /* destination */


#define ST_IS             0       /* in service */
#define ST_STB            1       /* stand-by */
#define ST_MB             2       /* maintanance busy */
#define ST_OOS            3       /* out of service */
#define ST_MOOS           4       /* out of service maintenance */
#define ST_BSY            5       /* busy D-channel */

#define NOBCHAN           0xff    /* no B channel selected */

#define NOT_INIT          0       /* Endpoint state Not Initialized */
#define W_INIT            1       /* Endpoint state Waiting for Initialization
                                     reply */
#define P_INIT            2       /* Endpoint state Point-to-Point Init'lzed */
#define M_INIT            3       /* Endpoint state Multipoint Initialized */
#define E_INIT            4       /* Endpoint state Initialized */

#define MAX_SPID_INIT     2       /* maximum number of Term Inits for NTDMS100B */
#define CALLREFLEN_B      1       /* call reference length for basic intrfce */
#define CALLREFLEN_P      2       /* call reference length for primary 
                                     interface */
#define GLOBAL            0       /* global call reference */
#define INTTMRVAL         0x05    /* internal timer value */

#define ONE_BYTE          1       /* one-byte length of call reference */
#define TWO_BYTE          2       /* two-byte length of call reference */

#define BC303_SUFX        3       /* Bellcore suffix. */
#define BC303_ULMT     2048       /* BC303 upper limit. */

/* #define  IN_CHKRST        1   */    /* TELICA-MCAO-11/01/01 : Bug 7145  don't pick
				     channel in restart states */

    
/* link states */

#define CBST0CON           0x00   /* Connect State */
#define CBST1XFER          0x01   /* Event Transfer State */
#define CBST2RST           0x02   /* Restarting State */
#define CBST3DOWN          0x03   /* Down State */

/* Bind States */
#define IN_UNBND            0x00   /* State : Bound */
#define IN_WAIT_BNDCFM     0x01   /* State : Waiting for Bnd Cfm */
#define IN_BND             0x02   /* State : Bound */

#define NMB_LNK_ST         0x04   /* number of link states */
#define ACK1               0x01
#define ACK2               0x02


/* connection states */

#define ST_NULL            0x00   /* U00, N00 - null                     - c, p, u */
#define ST_CALLINIT        0x01   /* U01, N01 - call initiated           - c, p, u */
#define ST_OVLAPSND        0x02   /* U02, N02 - overlap sending          - c, u */
#define ST_OUTPROC         0x03   /* U03, N03 - outgoing call proceeding - c, p, u */
#define ST_CALLDEL         0x04   /* U04, N04 - call delivered           - c, u */
#define ST_CALLPRES        0x06   /* U06, N06 - call present             - c, p, u */
#define ST_CALLRECV        0x07   /* U07, N07 - call received            - c, p */
#define ST_CONNRQ          0x08   /* U08, N08 - connect request          - c, p, u */
#define ST_INPROC          0x09   /* U09, N09 - incoming call proceeding - c, p, u */
#define ST_ACTVE           0x0A   /* U10, N10 - active call              - c, p, u */
#define ST_DISRQ           0x0B   /* U11, N11 - disconnect request       - c, p */
#define ST_DISCIN          0x0C   /* U12, N12 - disconnect indictation   - c, p */
#define ST_WAITRELC        0x0D   /* U13, N13 - Wait for RLC             - c, p */
#define ST_SUSRQ           0x0F   /* U15, N15 - suspend request          - c */
#define ST_RSMRQ           0x11   /* U17, N17 - resume request           - c */
#define ST_RELRQ           0x13   /* U19, N19 - release request          - c, p, u */
#define ST_CALLABRT        0x16   /* ---, N22 - call abort               - c, p, u */
#if (ISDN_303TMC || ISDN_303CSC)
#define ST_PERMSIGN        0x18   /* U24, --- - permanent signal         - c, u */
#endif
#define ST_OVLAPRCV        0x19   /* U25, N25 - overlap receiving        - c, u */

#define ST_TONEACTV        0x12   /* U18, N18 - tone active              - c, u */
#define ST_CALLINDEP       0x1f   /* U31, N31 - call independent         - c, p, u */
#define ST_WAITCPN1        0x1A   /* ---, NS1 - wait for cpn/bn          - c, u */
#define ST_WAITCPN2        0x1B   /* ---, NS2 - wait for cpn/bn          - c, u */
#define ST_WAITCPN3        0x1C   /* ---, NS3 - wait for cpn/bn          - c, u */
#define ST_WAITCONT        0x1D   /* ---, NS4 - wait for continuity      - c, u */

#define ST_WAITNET1        0x1E   /* US1, --- - wait for network         - c, u */
#define ST_WAITNET2        0x1F   /* US2, --- - wait for network         - c, u */

#define NMB_CON_ST         0x20   /* number of connection states */  /* FID 16970.0 */


#ifdef NETSIDE
/*
 * constants related ISDN Point to multipoint on network side.
 */
#ifdef ISDN_PTMPT

#define MAXNTCNUM           2    /* Max number if NTCs in PCB list. */
#define MAXCBTIMERBLOCK     2    
#define MAXSIMINTTIMER      2    /* Max number of simultaneaous timers. */
#define MAXCBCESNUM         8    /* Maximum possible  CESs. */

#define NO_VALID_RESPONSE   0    /* No Valid response received. */


#define INTSTATE_0NULL      0    /* Internal state null. */
#define INTSTATE_7CALLRECV  1    /* Internal State Call Received. */
#define INTSTATE_8CONNRQ    2    /* Internal State Connect Request */
#define INTSTATE_9INPROC    3    /* Internal State Incoming Call Proc. */
#define INTSTATE_19RELREQ   4    /* Internal State Release Request. */
#define INTSTATE_25OVLAPSND 5    /* Internal State Overlap Sending. */


#endif /* ISDN_PTMPT */
#endif /* NETSIDE */



/* timer events */

#define TMR_T301           0x01   /* U    - alert received */
#define TMR_T302           0x02   /* N, U - setup ack sent */
#define TMR_T303           0x03   /* N, U - setup sent */
#define TMR_T304           0x04   /* N, U - setup ack received or info sent */
#define TMR_T305           0x05   /* N, U - disc w/ prog ind sent or disc sent */
#define TMR_T306           0x06   /* N    - disc w/o prog ind sent */
#define TMR_T307           0x07   /* N    - susp ack sent */
#define TMR_T308           0x08   /* N, U - rel sent */
#define TMR_T310           0x09   /* N, U - call proc received */
#define TMR_T312           0x0a   /* N    - setup sent on broadcast */
#define TMR_T313           0x0b   /* U    - conn sent */
#define TMR_T318           0x0c   /* U    - resume sent */
#define TMR_T319           0x0d   /* U    - suspend sent */
#define TMR_T322           0x0e   /* N, U - stat enq sent */
#define TMR_T330           0x0f   /* N, U - congestion with upper */
#define TMR_T331           0x10   /* N, U - congestion with lower */
#define TMR_TANS           0x11   /* U - waiting for Connect Message */
#define TMR_T396           0x12   /* U - waiting for Connect Message */
#define TMR_T397           0x13   /* U - waiting for Connect Message */
#define TMR_TPROG          0x14   /* N - waiting for Alert or Connect Message*/
#if ISDN_ETSI
#define TMR_TMIN           0x15   /* N - waiting for minimum digits -- 2111 overlap */
#endif
#define TMR_THOLDSETUP     0x16   /* BUG:37899-mqin */
#define TMR_THOLDSETUP_VAL 20

#define NMB_UTMR         0x14    /* number of timers */

#define NMB_TMR          0x11    /* number of timers */
#define TMR_T332         0x12    /* N, U - service request sent */ 
#define TMR_T316         0x01    /* restart */
#define TMR_CON          0x02    /* connecting */
#define TMR_DISC         0x03    /* disconnecting */
#define TMR_T314         0x04    /* segmenting */
#define TMR_T332I        0x05    /* service D channel */
#define TMR_T316C        0x06    /* restart b channel */
#define TMR_TRST         0x07    /* restart interface */
#define TMR_TRSTACK      0x09    /* restart ack for all interfaces */
#if (ISDN_NI1 || ISDN_NT || ISDN_ATT)
#define TMR_SPID         0x08    /* terminal initialization timer */
#endif
#define TMR_TINT         0x0a    /* Bind Confirm Timer */
#if 1 /* addition : in014.310 */
#ifdef NI2
#ifdef NI2_TREST
#define TMR_TREST         0x0b    /* Restart Timer */
#endif /* NI2_TREST */
#endif /* NI2 */
#endif /* addition : in014.310 */

/* TELICA-jwh-08/14/2001:  Fix for bug#5497:  Release Avalanche  */
#if 1   /* Start of code change */
#define EVTININTEARDOWN (0x01)
#define EVTININCLEARALLCALLS (0x02)
#endif  /* End of code change */


/* TELICA-jwh-12/03/2000:  Add D channel service message retransmission timer */
#if 1   /* Start of code change */
#define TMR_TSERV         0x0c    /* D channel service message retransmit timer */
#define TMR_TSERV_VAL     30
#endif  /* End of code change */

/* TELICA-MCAO-10/29/2001:  BUG 6469: add additional timer to avoid flood of STAENQ. */
#if 1   /* Start of code change */
#define TMR_STAENQFLOODPROOF         0x0d    /* flood-proof of STAENQ  timer */
#define TMR_STAENQFLOODPROOF_VAL     30
#endif  /* End of code change */

#define NMB_RST_T303        2    /* number restart T303 */


#define ORG_0            0x00    /* call originated here */
#define ORG_8            1       /* call did not originate here */


/* channel types */

#define PRIM_US_RATE      24      /* number of B channel, PRI, US */
#define PRIM_EUR_RATE     32      /* number of B channels, PRI, Europe */

#define H0GROUP            6      /* group of slots for H0 channels */
#define H11GROUP          24      /* H11 group - all B channels */
#define BCHANTYPE          1      /* B channel type */
#define DCHANTYPE          2      /* D channel type */
#define INTERFTYPE         3      /* interface type - all B chans and D */
#define ALLINTERF          4      /* all interfaces type */
#define B1CHAN             1      /* B1 channel */
#define B2CHAN             2      /* B2 channel */
#define USDCHAN           24      /* US D channel channel */
#define EURDCHAN          16      /* European D channel channel */

#define ALL_CHAN          33      /* All channels */
#define ALL_CHAN_BCHANONLY_RSTOPT   34      /* All channels, but with rstOpt == BchanOnly */


#define RSTCHAN            1      /* restarting channel */
#define RSTINTERF          2      /* restarting interface */
#define RSTCHANSND         4      /* restart channel sent */
#define RSTINTERFSND       8      /* restart interface sent */

#define CHN_B              1      /* channel - b */
#define CHN_H0             6      /* channel - h0 */

#define MAXINUI            4      /* maximum number of upper interfaces */

#define MAXINLI            4      /* maximum number of lower interfaces */

#define MAXINMI            3      /* maximum number of interfaces with the sm*/

#define MIN_LEN_MSG        4      /* min length of the message */

#define SEL_TCLD           3      /* tightly coupled LD */
#if 0 /* deletion : in012.310 */
#define SEL_TCBD           4      /* tightly coupled BD */
#else /* addition : in012.310 */
#define SEL_TCBD           2      /* tightly coupled BD */
#endif /* change : in012.310 */

#define SEL_TCIX           1      /* tightly coupled IX */
#define SEL_TCAN           2      /* tightly coupled AN */

#define CALL_IN            1      /* Incoming Call */
#define CALL_OUT           2      /* Outgoing Call */

#define SRV_MSG_RSTACK     1      /* sends service message on restart ack */
#define SRV_MSG_ON         2      /* enables service message capability */
  


#define NON_ACTIVE_CALLS   1
#define ALL_CALLS          2 

#if 1 /* addition : in010.310 */
#define NON_STABLE_NOIND   3      /* Clear non-stable calls without 
                                     sending a RelInd to call control */
#endif /* addition : in010.310 */
/* Addition - in036 */
#define ALL_CALLS_NOIND    4      /* Clear all calls without
                                     sending a RelInd to call control */

#if 1 /* addition : in011.310 */
#define NORMAL_CLEAR       0      /* clear all calls */
#define FTHA_CLEAR         1      /* clear non-active calls */
/* TELICA-jwh-09/06/2001:  Add clear type for paced clear */
#if 1   /* Start of code change */
#define NORMAL_CLEAR_PACE  6      /* clear all calls */
#endif  /* End of code change */

#endif /* addition : in011.310 */

#define NTC_NOTCREATED     0
#define NTC_CREATED        1
#define NTC_DELETED        2

#ifdef MNT_REL
#define NTC_GET_DISCREQ    0x01
#define NTC_GET_DISCIND    0x02
#define NTC_GET_RELIND     0x04
#endif


/* bug 54753, litovchick 05/29/07 */
/* congestion handling improvements */
#if SW_VERSION3
#define SW_VERSION_MULTIPLIER 16
#else
#define SW_VERSION_MULTIPLIER 10
#endif
/************************************************************************
                    MACROS
************************************************************************/


#ifdef TR6
#define INASSIGNCAUSE(str,cause)
#else
#define INASSIGNCAUSE(str,cause)   str.pres=PRSNT_NODEF;\
                                   str.val=cause;
#endif /* TR6  */


/*
 * Macro to set token header to present no def.
 */

#define INSETTOKHDR(tok_hdr)   tok_hdr.eh.pres = PRSNT_NODEF;
#define INUNSETTOK(token)      token.pres = NOTPRSNT;



/*
 * Macro to set a token value.
 */
#ifdef TST_DRIVER
#define IN_TST_ASSERT(msg)   SDisplay(0, "[Test:]  ");\
                            SDisplay(0, msg);
#else  /* Not TST_DRIVER */
#define IN_TST_ASSERT(msg)
#endif /*  Not TST_DRIVER */


#define INSETTOKVAL(token,value) token.pres = PRSNT_NODEF;\
                                 token.val = value; 

#define INSETTOKARRAY(token,leng,src) token.pres = PRSNT_NODEF;\
                                     token.len = leng;\
                                     for (i=0;i<leng;i++)\
                                     {\
                                       token.val[i]=src[i];\
                                     }

/*
 * Macro to set timer values.
 */

#define INSETTMRVAL(timer)  pcb->cfg.tmr.timer.enb =  cfg->t.cfg.s.inDLSAP.tmr.timer.enb;\
                            pcb->cfg.tmr.timer.val =  cfg->t.cfg.s.inDLSAP.tmr.timer.val;


#ifdef NT
#define IN_SWNT100B_GEN_PRG     if (pcb->cfg.swtch == SW_NTDMS100B) \
                                {\
                                    inGenPrgMsg(ntc, &ntc->causeDgn, dCb);\
                                }\
                                else\
                                {\
                                    inGenRelLw(ntc, &ntc->causeDgn, dCb);\
                                }
#else
#define IN_SWNT100B_GEN_PRG     inGenRelLw(ntc, &ntc->causeDgn, dCb);
#endif


#define IN_GET_BEARER(p_cb, i_dex, o_fset, b_chan)  \
                     b_chan = p_cb->bBearer +(i_dex - o_fset);

#if 1 /* TELICA - Bugzilla-4321 - Bukucu - 10.25.01 - added inUpdSts() */
#define SND_RESTART { \
                    if (((pcb->cfg.swtch == SW_NTDMS250) || \
                         (pcb->cfg.swtch == SW_NTDMS100P) || \
                         (pcb->cfg.swtch == SW_NTMCI)) && \
                         (pcb->cfg.rstOpt) && (pcb->cfg.intType == NETWORK)) \
                    {\
                       inInitPduHdr(PD_Q931, ORG_0, pcb->cfg.callRefLen, GLOBAL, M_RST, &pduHdr);\
                       MFINITPDU(&cb->mfMsgCtl, ret, (U8) 0, (U8) MI_RST, NULLP, \
                                 (ElmtHdr *) &msg, (U8) NOTPRSNT, pcb->cfg.swtch, (U32) MF_NA);\
                       msg.m.rst.rstInd.eh.pres = PRSNT_NODEF;\
                       msg.m.rst.rstInd.rstClass.pres = PRSNT_NODEF;\
                       msg.m.rst.rstInd.rstClass.val = CL_ALLINT;\
                       msg.m.rst.chanId.eh.pres = NOTPRSNT;\
                       inUpdSts(&pcb->sts.rstTx, pcb);\
                       cb->state = CBST2RST;\
                       inStartCtldPcbTmr(TMR_T316, ctldPcb);\
                       inGenPdu(cb, &pduHdr, &msg, pcb->cfg.swtch, (U32) MF_NA);\
                       rstSnd = TRUE;\
                    } \
                  }
#else /* old code */
#define SND_RESTART { \
                    if (((pcb->cfg.swtch == SW_NTDMS250) || \
                         (pcb->cfg.swtch == SW_NTDMS100P) || \
                         (pcb->cfg.swtch == SW_NTMCI)) && \
                         (pcb->cfg.rstOpt) && (pcb->cfg.intType == NETWORK)) \
                    {\
                       inInitPduHdr(PD_Q931, ORG_0, pcb->cfg.callRefLen, GLOBAL, M_RST, &pduHdr);\
                       MFINITPDU(&cb->mfMsgCtl, ret, (U8) 0, (U8) MI_RST, NULLP, \
                                 (ElmtHdr *) &msg, (U8) NOTPRSNT, pcb->cfg.swtch, (U32) MF_NA);\
                       msg.m.rst.rstInd.eh.pres = PRSNT_NODEF;\
                       msg.m.rst.rstInd.rstClass.pres = PRSNT_NODEF;\
                       msg.m.rst.rstInd.rstClass.val = CL_ALLINT;\
                       msg.m.rst.chanId.eh.pres = NOTPRSNT;\
                       pcb->sts.rstTx++;\
                       cb->state = CBST2RST;\
                       inStartCtldPcbTmr(TMR_T316, ctldPcb);\
                       inGenPdu(cb, &pduHdr, &msg, pcb->cfg.swtch, (U32) MF_NA);\
                       rstSnd = TRUE;\
                    } \
                  }

#endif /* Bugzilla-4321 */
/*
* Macro to stop all ntc timers.
 */

#define STOP_NTC_TIMERS   for(tmrNum=0;tmrNum<MAXSIMTIMER;tmrNum++)\
                          {\
                             if((ntc->timers[tmrNum].tmrEvnt != TMR_NONE)\
                                && (ntc->timers[tmrNum].tmrEvnt != TMR_T312))\
                             {\
                                 inRmvNtcTq(ntc, tmrNum);\
                             }\
                          }
 
#define STOP_NTC_TIMER(timer)  for(tmrNum=0;tmrNum<MAXSIMTIMER;tmrNum++)\
                               {\
                                   if(ntc->timers[tmrNum].tmrEvnt==timer)\
                                   {\
                                       inRmvNtcTq(ntc, tmrNum);\
                                       break;\
                                   }\
                               }
#define STOP_NTC_TIMER2(t1,t2) for(tmrNum=0;tmrNum<MAXSIMTIMER;tmrNum++)\
                               {\
                                   if((ntc->timers[tmrNum].tmrEvnt==t1)||\
                                      (ntc->timers[tmrNum].tmrEvnt==t2))\
                                   {\
                                       inRmvNtcTq(ntc, tmrNum);\
                                   }\
                               }
#ifdef ATT
#define IN_SWATT4E_SETNETFAC  if (pcb->cfg.swtch == SW_ATT4E)\
                             {\
                                if (((conEvnt->netFac[0].eh.pres)&&\
                                     (conEvnt->netFac[0].facCode.pres)) &&\
                                    ((conEvnt->netFac[0].facCode.val == SDN)||\
                                     (conEvnt->netFac[0].facCode.val == ACCUNET)) &&\
                                  ((ntc->callType == MF_PKT) && ntc->dChanAs))\
                                    ntc->callType |= MF_UTU;\
                             }\
                             else\
                                if ((ntc->callType == MF_PKT) && \
                                     ntc->dChanAs)\
                                {\
                                    ntc->callType |= MF_UTU;\
                                } 
#else
#define IN_SWATT4E_SETNETFAC  if ((ntc->callType == MF_PKT) \
                                  && ntc->dChanAs)\
                              {\
                                 ntc->callType |= MF_UTU;\
                              }
#endif



#ifdef NT
#define IN_SWNT100B_TONES    if (pcb->cfg.swtch == SW_NTDMS100B)\
                             {\
                                 INSETTOKHDR(allPdus.m.conn.signal);\
                                 INSETTOKVAL(allPdus.m.conn.signal.signal,\
                                             SIG_TONESOFF);\
                             }
#else
#define IN_SWNT100B_TONES
#endif


#define STOP_CB_TIMER(timer) for (tmrNum = 0; tmrNum < MAXSIMTIMER; tmrNum++)\
                             {\
                                 if (cb->timers[tmrNum].tmrEvnt == timer)\
                                 {\
                                    inRmvCbTq(cb, tmrNum);\
                                    break;\
                                 }\
                             }

#define STOP_CB_TIMERS   for(tmrNum=0;tmrNum<MAXSIMTIMER;tmrNum++)\
                          {\
                             if(cb->timers[tmrNum].tmrEvnt != TMR_NONE)\
                             {\
                                 inRmvCbTq(cb, tmrNum);\
                             }\
                          }\
                          cb->tmrCnt = 0; 

#define STOP_BCHAN_TIMERS   for(tmrNum=0;tmrNum<MAXBCHTIMER;tmrNum++)\
                          {\
                             if (bearChn->timers[tmrNum].tmrEvnt != TMR_NONE) \
                             {\
                                 inRmvBChTq(bearChn, tmrNum);\
                             }\
                          }\
                           bearChn->rst = bearChn->rstCnt = 0;
                           
#define STOP_PCB_TIMERS   for(tmrNum=0;tmrNum<MAXBCHTIMER;tmrNum++)\
                          {\
                             if (pcb->timers[tmrNum].tmrEvnt != TMR_NONE) \
                             {\
                                 inRmvPcbTq(pcb, tmrNum);\
                             }\
                          }\
                          pcb->rstAll = 0;

#if 1 /* Addition : in001.310 */
#define STOP_CTLDPCB_TIMERS   for(tmrNum=0;tmrNum<MAXBCHTIMER;tmrNum++)\
                          {\
                             if (ctldPcb->timers[tmrNum].tmrEvnt != TMR_NONE) \
                             {\
                                 inRmvCtldPcbTq(ctldPcb, tmrNum);\
                             }\
                          }\
                          ctldPcb->rstCnt = 0;

#endif /* Addition : in001.310 */

#define STOP_SPID_TIMERS {\
                            if((sigPcb->cfg.swtch == SW_NI1) || \
                               (sigPcb->cfg.swtch == SW_ATT5EB) || \
                               (sigPcb->cfg.swtch == SW_NTDMS100B)) \
                            {\
                               if((cb->endPointState == NOT_INIT) || \
                                  (cb->endPointState == W_INIT)) \
                                {\
                                 for (tmrNum = 0; tmrNum < MAXSIMTIMER;tmrNum++) \
                                 {\
                                   if (cb->timers[tmrNum].tmrEvnt == TMR_SPID)\
                                     inRmvCbTq(cb, tmrNum);\
                                 }\
                               }\
                            }\
                          }


/* TELICA-jwh-09/06/2001:  Make sure all timers are stopped before freeing memory */
#if 1   /* Start of code change */
#define STOP_TIMERS(pBlk, maxTimer, inRmvTqFunc) \
{ \
    U8 tmrNum; \
    for (tmrNum=0; tmrNum<(maxTimer); tmrNum++) \
    { \
        if ((pBlk)->timers[tmrNum].tmrEvnt != TMR_NONE) \
        { \
            inRmvTqFunc((pBlk), tmrNum); \
        } \
    } \
}
#endif  /* End of code change */

/*
 * Following macros used for Network side point to multipoint
 * operations.
 */
#ifdef NETSIDE  
#ifdef ISDN_PTMPT 

#define IN_SEND_BCASTMSG(c_blk, m_buf)       \
                             if (c_blk->trc)   \
                                inTrcBuf(c_blk->suId, TL3PKTTX, m_buf);\
          InLiDatUDatReq(&c_blk->pst, c_blk->spId, Q_930, BRDCST_CES, m_buf);



#define IN_INT_ISSUE_REL_REQ(ntc_blk, cb_blk, cause_ptr)     \
                            if ((pcb->cfg.intType == NETWORK) \
                                    && ((pcb->cfg.intCfg == INTCFG_MULTI) \
                                         && (ntc->setupBCast ==  TRUE))) \
                            {\
                              inIntIssuePrimRelReq(ntc, dCb, &ntc->causeDgn);\
                              RETVALUE(ROK);\
                            }

#define IN_PTMPT_ISSUE_STATUS(p_blk, n_blk, c_blk)   \
                     if ((p_blk->cfg.intCfg == INTCFG_MULTI)  \
                         && ((p_blk->cfg.intType == NETWORK)  \
                             && (n_blk->setupBCast == TRUE))) \
                     { \
                        if ((n_blk->state == ST_CALLRECV)     \
                            || ((n_blk->state == ST_CONNRQ)   \
                                || (n_blk->state == ST_INPROC) \
                                   || (n_blk->state == ST_OVLAPRCV))) \
                        {  \
                           U8 tmpState; \
                           tmpState = n_blk->state; \
                           n_blk->state = ST_CALLPRES; \
                           inNetMSGINV(n_blk, c_blk);  \
                           n_blk->state = tmpState;  \
                           RETVALUE(ROK); \
                        }  \
                     } 

#define ININT_ISSUE_CONN_REQ(callb, cesb)                  \
                                callb->cesState[cesb->ces] = INTSTATE_8CONNRQ;
    

#define inIntPrimConnReq(a, b)   ININT_ISSUE_CONN_REQ(a, b)


#define ININT_ISSUE_ALERTING_REQ(callb, cesb)                  \
                                callb->cesState[cesb->ces] = INTSTATE_7CALLRECV;
    

#define inIntPrimAlertingReq(a, b)   ININT_ISSUE_ALERTING_REQ(a, b)


#endif /* ISDN_PTMPT */
#endif /* NETSIDE */



#if 1 /* TELICA - Bugzilla-4321 - Bukucu - 10.25.01 - added inUpdSts() */
#define REST(pcb, allPdus, pduHdr,  cb) \
                  if (((pcb->cfg.swtch == SW_NTDMS250) || \
                       (pcb->cfg.swtch == SW_NTDMS100P) || \
                       (pcb->cfg.swtch == SW_NTMCI)) && \
                      (pcb->cfg.rstOpt) && (pcb->cfg.intType == NETWORK)) \
                   {\
                     inInitPduHdr(PD_Q931, ORG_0, pcb->cfg.callRefLen, GLOBAL, M_RST, &pduHdr); \
                     MFINITPDU(&cb->mfMsgCtl, ret, (U8) 0, (U8) MI_RST, NULLP, (ElmtHdr *) &allPdus, (U8) NOTPRSNT, pcb->cfg.swtch, (U32) MF_NA); \
                      allPdus.m.rst.rstInd.eh.pres = PRSNT_NODEF; \
                      allPdus.m.rst.rstInd.rstClass.pres = PRSNT_NODEF; \
                      allPdus.m.rst.rstInd.rstClass.val = CL_ALLINT; \
                      allPdus.m.rst.chanId.eh.pres = NOTPRSNT; \
                      inUpdSts(&pcb->sts.rstTx, pcb); \
                      cb->state = CBST2RST; \
                      inStartCtldPcbTmr(TMR_T316, ctldPcb); \
                      inGenPdu(cb, &pduHdr, &allPdus, pcb->cfg.swtch, (U32) MF_NA); \
                    }
#else /* old code */
#define REST(pcb, allPdus, pduHdr,  cb) \
                  if (((pcb->cfg.swtch == SW_NTDMS250) || \
                       (pcb->cfg.swtch == SW_NTDMS100P) || \
                       (pcb->cfg.swtch == SW_NTMCI)) && \
                      (pcb->cfg.rstOpt) && (pcb->cfg.intType == NETWORK)) \
                   {\
                     inInitPduHdr(PD_Q931, ORG_0, pcb->cfg.callRefLen, GLOBAL, M_RST, &pduHdr); \
                     MFINITPDU(&cb->mfMsgCtl, ret, (U8) 0, (U8) MI_RST, NULLP, (ElmtHdr *) &allPdus, (U8) NOTPRSNT, pcb->cfg.swtch, (U32) MF_NA); \
                      allPdus.m.rst.rstInd.eh.pres = PRSNT_NODEF; \
                      allPdus.m.rst.rstInd.rstClass.pres = PRSNT_NODEF; \
                      allPdus.m.rst.rstInd.rstClass.val = CL_ALLINT; \
                      allPdus.m.rst.chanId.eh.pres = NOTPRSNT; \
                      pcb->sts.rstTx++; \
                      cb->state = CBST2RST; \
                      inStartCtldPcbTmr(TMR_T316, ctldPcb); \
                      inGenPdu(cb, &pduHdr, &allPdus, pcb->cfg.swtch, (U32) MF_NA); \
                    }
#endif /* Bugzilla-4321 */

#if 0 /* deletion : in019.310 */
#define SENDSTATMSG(cause, val1, chanId) inSetCauseDgn(cb, &causeDgn,  (U8)CCINVINFOEL,\
                                         (U8)val1, (U8)chanId);\
                                         inGenStaNoNtc(cb, &causeDgn,\
                                         (CalRef)GLOBAL, (Origin)cb->callRefFlag);
#else /* addition : in019.310 */
#define SENDSTATMSG(cause, val1, chanId) inSetCauseDgn(cb, &causeDgn, cause,\
                                                       (U8)val1, (U8)chanId);\
                                         inGenStaNoNtc(cb, &causeDgn,\
                                                       (CalRef)GLOBAL, \
                                                     (Origin)cb->callRefFlag);
#endif /* change : in019.310 */

#define IN_GETINTERFACE(rst, sigCtldPcb, ctldPcb) if ((rst->chanId.intIdent.pres) && \
                                             (rst->chanId.intIdent.val != sigCtldPcb->cfg.intId)) \
                                         {\
                                            if ((!sigCtldPcb->cfg.nfasInt) ||\
                                                ((ctldPcb = inSearchIntId(pcb, rst->chanId.intIdent.val))\
                                                  == NULLP))\
                                            {\
                                               inSetCauseDgn(cb, &causeDgn, CCINVINFOEL, 1, ME_CHANID);\
                                               inGenStaNoNtc(cb, &causeDgn, GLOBAL, cb->callRefFlag);\
                                               RETVALUE(ROK); \
                                            }\
                                         }\
                                         else\
                                            ctldPcb = sigCtldPcb;

#define INIT_STAEVNT(msgCtlp, ret, staEvnt, swtch) \
       {\
        MFINITSDU(&msgCtlp, ret, (U8)0, (U8)PM_STATREQ, NULLP, \
             (ElmtHdr *)&staEvnt, (U8) PRSNT_DEF, swtch, \
             (U32) MF_UNK);\
        staEvnt.callSte.eh.pres = PRSNT_NODEF; \
        INSETTOKVAL(staEvnt.callSte.callGlblSte,ST_NULL); \
        staEvnt.causeDgn[0].eh.pres = PRSNT_NODEF; \
      }

#define INIT_TR_STAT(swtch, staEvnt) \
      {\
        if ((swtch == SW_TR6MPC) || (swtch == SW_TR6PBX)) \
        { \
          staEvnt.trCause.eh.pres = PRSNT_NODEF; \
          staEvnt.trCause.causeVal.pres = PRSNT_NODEF; \
          INSETTOKVAL(staEvnt.trCause.causeVal,CCLOCLPROCERR); \
        }\
      }

#ifdef ISDN_HOLD
/* auxiliary states */

#define AUXST_IDLE         0      /* idle */
#define AUXST_HLDREQ       1      /* hold request */
#define AUXST_CLLHLD       2      /* call held */
#define AUXST_RTRREQ       3      /* retrieve request */
#endif

#ifdef PLM_IN
enum
{
    IN_LNK_TYPE_UNKNOWN,
    IN_LNK_TYPE_B,
    IN_LNK_TYPE_D_PRI,
    IN_LNK_TYPE_D_NFAS,
    IN_LNK_TYPE_D1,
    IN_LNK_TYPE_D2
};
enum
{
  IN_CALL_TREATMENT_NONE,
  IN_CALL_TREATMENT_CLEAR_SINGLE,
  IN_CALL_TREATMENT_CLEAR_ALL,
  IN_CALL_TREATMENT_MAX
};

#endif /* PLM_IN */

/* mcao- 06/13/02: isdnsigdbg prints */
#define DBGMASK_IN        (DBGMASK_LYR)

/* Macro For Debug Printing */
/* Usage: 
   there are 2 levels of prints: layer level(higher priority) and link level
   layer level inInit.dbgMask, set through "sigdbg:::::set in dbgmask 0xffffffff;
   link level cb->dbgMask, set through "sigdbg:::::set in dbgmask x-y 0xffffffff; 
   some most used combinations:
    
   1. turn all prints on                e.g. inInit.dbgMask = 0xffffffff;
   2. monitor level oriented prints     e.g. inInit.dbgMask = 0x4 catch all UI interface prints; 
   3. monitor all levels link based prints           e.g. inInit.dbgMask = 0x0, cb->dbgMask = 0xffffffff;
   4. monitor particular level link based prints     e.g. inInit.dbgMask = 0x0, cb->dbgMask = 0x4;
   5. monitor other non-link related prints          e.g. inInit.dbgMask = 0x80000000, cb->dbgMask = 0x0;
   6. mix non-link related and particular level of indicated link:
                                                     e.g. inInit.dbgMask = 0x80000000, cb->dbgMask = 0x4;
 */

#define INDBGP_CB(level, fmtStr, args...) \
if (cb) \
{ \
   if ((level) & (inInit.dbgMask | cb->dbgMask)) \
    { \
        DP("[Q931]:" fmtStr, ## args); \
    } \
}

#define INDBGP(level, fmtStr, args...) \
{ \
    if ((level) & inInit.dbgMask) \
    { \
        DP("[Q931]:" fmtStr, ## args); \
    } \
}

#ifdef TELICA /* gdixit-12/09/2002-Bug#16931 */ 
/* following macro will be used for Rx msg
   see if msgType, element and cause are same, before return TRUE or FALSE
   may also check time elapsed ...
   and incraeament the counter */
#define IN_MF_RX_ERROR(msgCtlp, my_ret)  {                                       \
            if ((inMfRxCb.msgId == (msgCtlp)->msgType) &&                        \
                (inMfRxCb.elmtId == (msgCtlp)->ee[0].elmtId) &&                  \
                (inMfRxCb.cause == (msgCtlp)->ee[0].mfCauseDgn.causeVal.val) &&  \
                ((time(NULL) - inMfRxCb.time) <= inMfRxCb.time_diff))            \
            {                                                                    \
               my_ret = FALSE; /* return FALSE, we dont want DP */               \
            }                                                                    \
            else                                                                 \
            {                                                                    \
               inMfRxCb.msgId = (msgCtlp)->msgType;                              \
               inMfRxCb.elmtId = (msgCtlp)->ee[0].elmtId;                        \
               inMfRxCb.cause = (msgCtlp)->ee[0].mfCauseDgn.causeVal.val;        \
               inMfRxCb.time = time(NULL);                                       \
               my_ret = TRUE; /* we want to print incoming message */            \
               DP("IN_MF_RX_ERROR Count=%ld\n", inMfRxCb.count);                  \
            }                                                                    \
            inMfRxCb.count++;                                                    \
         }  /* end of MACRO, IN_MF_RX_ERROR */

/* following macro will be used for Tx msg
   see if msgType, element and cause are same, before return TRUE or FALSE
   and incraeament the counter */
#define IN_MF_TX_ERROR(msgCtlp, my_ret)  {                                       \
            if ((inMfTxCb.msgId == (msgCtlp)->msgType) &&                        \
                (inMfTxCb.elmtId == (msgCtlp)->ee[0].elmtId) &&                  \
                (inMfTxCb.cause == (msgCtlp)->ee[0].mfCauseDgn.causeVal.val) &&  \
                ((time(NULL) - inMfTxCb.time) <= inMfTxCb.time_diff))            \
            {                                                                    \
               my_ret = FALSE; /* return FALSE, we dont want DP */               \
            }                                                                    \
            else                                                                 \
            {                                                                    \
               inMfTxCb.msgId = (msgCtlp)->msgType;                              \
               inMfTxCb.elmtId = (msgCtlp)->ee[0].elmtId;                        \
               inMfTxCb.cause = (msgCtlp)->ee[0].mfCauseDgn.causeVal.val;        \
               inMfTxCb.time = time(NULL);                                       \
               my_ret = TRUE; /* we want to print message  error */              \
               DP("IN_MF_TX_ERROR Count=%ld\n", inMfTxCb.count);                  \
            }                                                                    \
            inMfTxCb.count++;                                                    \
         }  /* end of MACRO, IN_MF_TX_ERROR */

#endif /* BUG:16931-end */ 

#ifdef IN_ACC /* TELICA-BUG:18735-mqin */
#define MAX_ISDN_CALL_COUNTER   0xffffffff /* 2^32 , 2 to the power of 32 */
#define ISDN_INC_INCOMING_CALL_CNTR()\
    {\
       dCb->incCallCntr++;\
       dCb->incCallCntr %= MAX_ISDN_CALL_COUNTER;\
    }
/*
 * release first 20 new incoming calls out of 100.
 * that is , 20 % of calls during congestion.
 * By just changing this macro, you can implement
 * any kind of call reject policy.
 * -H.Kim-
 */
/* bug 54753, litovchick 04/02/07 */
/* congestion handling improvements */
/* release 2 out of 10 calls */
#   define IN_ACC_SHOULD_REJECT(arg_cong)\
       (((dCb->incCallCntr % 10) < (GET_RELEASE_PCT(&(arg_cong))/10)) ? 1 : 0)

/* mcao-05/05/2004 add new ISDN congestion control */
extern Bool in_curr_cong;
extern Bool in_old_cong;

/* mcao-08/13/03: Bug 22582 GDI Cnglvl is introduced to evaluate real congestion */
#define IN_GDI_CNG_MAPPING(inLvl) \
{ \
 switch(gdiCngLvl) \
 { \
 case GDICONG_LEVEL_ONE: \
 case GDICONG_LEVEL_TWO: \
   inLvl = DQCONG_LEVEL_ONE; \
   break; \
 case GDICONG_LEVEL_THREE: \
 case GDICONG_LEVEL_FOUR: \
   inLvl = DQCONG_LEVEL_TWO; \
   break; \
 case GDICONG_LEVEL_NORMAL: \
 default: \
   inLvl = DQCONG_LEVEL_NORMAL; \
   break; \
 } \
}

#define IN_CHKDQCONG(dQue, ret_val)                                                    \
               {                                                                       \
                 U8 inGdiCngLvl = 0;                                                   \
                                                                                       \
                  SChkDmndQCong_V2(dQue);                                                 \
                  ret_val = GET_DQCONG_LEVEL(dQue);                                          \
                  IN_GDI_CNG_MAPPING(inGdiCngLvl);                                     \
                  ret_val = (ret_val >= inGdiCngLvl)? ret_val: inGdiCngLvl;            \
                  if (ret_val != in_old_cong)                                          \
                  {                                                                    \
                     switch (ret_val)                                                  \
                     {                                                                 \
                       case DQCONG_LEVEL_NORMAL:                                       \
                          if (in_old_cong == DQCONG_LEVEL_ONE)                         \
                          {                                                            \
                              /* clear level1 alarm */                                 \
                              inGenAlarm(LCM_CATEGORY_RESOURCE, LCM_EVENT_THRSHLD,  \
                                         LCM_CAUSE_DQ_CONG_CLEAR, 0);                      \
                              DP("ISDN DQ Clear Congestion, lvl1 alarm cleared\n");    \
                          }                                                            \
                          else if (in_old_cong == DQCONG_LEVEL_TWO)                    \
                          {                                                            \
                              /* clear level2 alarm */                                 \
                              inGenAlarm(LCM_CATEGORY_RESOURCE, LCM_EVENT_THRSHLD,  \
                                          LCM_CAUSE_DQ_CONG_LVL2, 0);                     \
                              DP("ISDN DQ Clear Congestion, lvl2 alarm cleared\n");    \
                          }                                                            \
                          break;                                                       \
                       case DQCONG_LEVEL_ONE:                                          \
                          /* generate level1 alarm */                                  \
                          inGenAlarm(LCM_CATEGORY_RESOURCE, LCM_EVENT_THRSHLD,      \
                                     LCM_CAUSE_DQ_CONG_LVL1, 0);                          \
                          DP("ISDN DQ Congested, Lvl 1 alarm generated\n");            \
                          break;                                                       \
                       case DQCONG_LEVEL_TWO:                                          \
                          /* generate level2 alarm */                                  \
                          inGenAlarm(LCM_CATEGORY_RESOURCE, LCM_EVENT_THRSHLD,      \
                                     LCM_CAUSE_DQ_CONG_LVL2, 0);                          \
                          DP("ISDN DQ Congested, Lvl 2 alarm generated\n");            \
                          break;                                                       \
                       default:                                                        \
                          DP("ISDN Unknown Cong LVL=%d rcvd made lvl2\n", ret_val);    \
                          /* take safe side, and make it lvl2 */                       \
                          ret_val = DQCONG_LEVEL_TWO;                                  \
                       break;                                                          \
                     } /* switch */                                                    \
                     in_old_cong = ret_val;                                            \
                  }                                                                    \
                  /* set it to TRUE of FALSE before return */                          \
                  ret_val = (ret_val != DQCONG_LEVEL_NORMAL);                          \
               }
#endif /* 18735 */

#ifdef ETSI
#define IN_GOT_MINDIGITS(digitLen, minDigits) \
                        ((digitLen >= minDigits) ? \
                          TRUE : FALSE)

#define IN_GOT_MAXDIGITS(digitLen, maxDigits) \
                        ((digitLen >= maxDigits) ? \
                         TRUE : FALSE)
#define AWAIT_INFO       0x2
#define IN_CK_WAITINFO(waitForMoreInfo)      (((waitForMoreInfo) & AWAIT_INFO) ? TRUE : FALSE)
#define IN_SET_WAITINFO(waitForMoreInfo)     ((waitForMoreInfo) |= AWAIT_INFO)
#define IN_CLR_WAITINFO(waitForMoreInfo)     ((waitForMoreInfo) &= 0xF8)

/* Kavon FID17555.0 */
#define PLAY_DIAL_TONE   0x4
#define IN_CK_PLAYDIALTONE(waitForMoreInfo)  (((waitForMoreInfo) & PLAY_DIAL_TONE) ? TRUE : FALSE)
#define IN_SET_PLAYDIALTONE(waitForMoreInfo) ((waitForMoreInfo) |= PLAY_DIAL_TONE)

/* FID:15070 if ntc->numPL_max get maxDigits from
 * mapping table, use ntc->numPL_max as the maxDigits,
 * else use configured maxDigits in pcb->cfg.maxDigits
 */
#define IN_GOTMAX(ntc,pcb)\
        (((ntc->numPL_max) > 0)?\
        (ntc->numPL_max) : (pcb->cfg.maxDigits))

/* FID 16779.0 */
#define IN_MAX_TON            0x08
#define IN_CONVERT_TYPENMB_TO_LIST_IDX(typeNmb, listIdx) \
{ \
    if (typeNmb == TON_DEFAULT) \
    { \
        listIdx = IN_MAX_TON; \
    } \
    else \
    { \
        listIdx = typeNmb; \
    } \
}

#define IS_NOT_MATCH          0x00
#define IS_MATCH              0x01

#define IS_NUMBER(c)          ( ((c) >= '0') && ((c) <= '9') )
#define IS_A2F(c)             ( ((c) >= 'A') && ((c) <= 'F') )
#define IS_X(c)               ( (c) == 'X' )
#define IS_NOT_X(c)           ( (c) != 'X' )

#define IS_LEFT_BRACKET(c)    ( (c) == '[' )
#define IS_RIGHT_BRACKET(c)   ( (c) == ']' )
#define IS_SHORT_DASH(c)      ( (c) == '-' )

#define IS_EXACT_MATCH(c)     ( IS_NUMBER(c) || IS_A2F(c) )
#define IS_ANY_MATCH(c)       IS_X(c)
#define IS_RANGE_MATCH(c)     IS_LEFT_BRACKET(c)
#define IS_IN_RANGE(c,lo,hi)  ( ((c) >= (lo)) && ((c) <= (hi)) )
#endif /* 2111 overlap */

/* FID 17233.0 */
#define IN_BCHAN_BOTHWAY    0x00
#define IN_BCHAN_INCOMING   0x01
#define IN_BCHAN_OUTGOING   0x02

/* FID 16970.0 */
#define IN_REG_NONE        0x00
#define IN_REG_IND         0x01
#define IN_REG_CFM         0x02

#define IS_BCHAN_DIR_MATCH(bearChn, ntc) \
  ((((bearChn) != NULL) && ((ntc) != NULL) && \
    (((bearChn)->chnDir == IN_BCHAN_BOTHWAY) || \
     ((bearChn)->chnDir == ntc->toBeAlctdBChanlDir))) ? TRUE : FALSE)

#endif
/* FID 17412.0 + */
#define IN_CONNECT_WO_DATE_TIME                 0x00
#define IN_CONNECT_W_DATE_TIME_LOCAL            0x01
#define IN_CONNECT_W_DATE_TIME_UTC              0x02
#define IN_CONNECT_W_DATE_TIME_LOCALWOSECS      0x03
#define IN_CONNECT_W_DATE_TIME_UTCWOSECS        0x04
/* FID 17412.0 - */

  
  
/********************************************************************30**
  
         End of file: in.h 3.7  -  09/29/99 14:26:52
  
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
  
  version    initials                   description
-----------  ---------  ------------------------------------------------
1.1.0.0         lc      1. initial release.

1.2.0.0         lc      1. add information elements
                        2. add codeset support

1.3.0.0         lc      1. renumber message element indexes from relative
                           to absolute

1.4.0.0         lc      1. text changes

1.5.0.0         lc      1. change CALLREFLEN from 1 to 2
                        2. add message elements and message element indexes
                           for ATT
                        3. remove TF_SKIPF and add TF_PUSHF token flags
                        4. add TF_PEXTN0 token flag
                        5. delete MF_MAX_MSG_ERRS
  
1.6.0.0         lc      1. added EI_SRV event define
                        2. change NMB_EVNT from 0x0d to 0x0e for EI_SRV
                        3. added PM_SRV event define
                        4. change NMB_PRIM from 13 to 14 for PM_SRV
                        5. added OUT_SERVICE_NE, OUT_SERVICE_FE, MAINT_NE
                           and MAINT_FE defines
                        6. deleted OUT_SERVICE and MAINT_SERVICE defines
                        7. added TMR_T332 define
                        8. deleted MF_OCTET_MASK define

1.7             lc      1. changed ORG_8 to 1
                        2. added information elements for new switches
                        3. reorgonized information elements to put them
                           in acsending order
                        4. added new states 
                jrl     5. trillium development system checkpoint (dvs)
                           at version: 1.6.0.0

1.8             lc      1. PM_ event indexes have been moved to int.h
                        2. add MAXINUI and MAXINLI

1.9             bn      1. removed BASICRATE define.

1.10            bn      1. change TQNUMENT and MOD64 defines.

1.11            bn      1. added defines for BRDCST_CES and CONN_CES.

*********************************************************************71*/

/********************************************************************80**

  version    pat  init                   description
----------- ----- ----  ------------------------------------------------
1.12         ---  jrl   1. text changes

1.13         ---  bn    1. added defines for signalling states.
                        2. changed defines for control block states.

1.14         ---  rg    1. changed CALLREFLEN to CALLREFLEN_P, and
                           added new define for CALLREFLEN_B (1).
                        2. added TMR_TRSTACK.
             ---  bn    3. added support for ni1.

1.15         ---  bn    1. added defines for USDCHAN and EURDCHAN.
             ---  bn    2. removed define for TMR_NONE.
             ---  bn    3. add support for tr 303 tmc.

1.16         ---  bn    1. change IN_ to ISDN_

1.17         ---  bn    1. added auxiliary states for Q932.

1.18         ---  bn    1. changed ifdef Q932 to ifdef HOLD.

1.19         ---  bn    1. changed #ifdef HOLD to #if ISDN_HOLD.

3.1          ---  bn    1. added a define for MIN_LEN_MSG.
             ---  bn    2. changed interfaces 
 
3.2          ---  krp   1. Added MAX_SRVREQ_CNT definition.
             ---  krp   2. Added MAX_SPID_INIT definition
             ---  krp   3. Modified TMR_SPID definition to be included
                           for AT&T, DMS and NI1 switches.
*********************************************************************81*/

/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- -----------------------------------------------
3.3          ---      sb   1. Added Macros.

3.4          ---      sb   1. Added BC 303 call reference boundary 
                             constants. 
             ---      sb   2. Added macros for ISDN point to multipoint.

3.5          ---      pk   1. Added define for TMR_TPROG.

3.6          ---      asa  1. Changed macro IN_PTMPT_ISSUE_STATUS to
                              return after issuing STATUS message.
             ---      sam  2. Added macro IN_GET_BEARER.

3.7          ---      pk   1. Added NOTPROVISIONED (not provisioned) and 
                              INT_INV(invalid interface) defines.
                      rsk  2. Added RSTCHANSND and RSTINTERFSND defines

3.8        in001.310  pk   1. Added STOP_CTLDPCB_TIMERS.
           in010.310  cy   2. Added NON_ACTIVE_NOIND.
           in011.310  pk   3. Added defines NORMAL_CLEAR &  FTHA_CLEAR 
           in012.310  cy   4. Corrected the defined value for SEL_TCBD.
           in014.310  mm   5. Added TMR_TREST Timer event for NI2 variant.
           in019.310  cy   6. Corrected cause value in define SENDSTATMSG. 
           in036.310  bb   7. Added new hashdef ALL_CALLS_NOIND to release
                              calls without giving any indication to upper
                              layer.
*********************************************************************91*/
