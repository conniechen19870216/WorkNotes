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
  
     Desc:     Structures, variables and typedefs required by the
               Q.93x.
  
     File:     in.x

     Sid:      in.x 3.8  -  09/29/99 14:29:12
 
     Prg:      na
  
*********************************************************************21*/
  
#ifndef __INX__
#define __INX__

/* TELICA-mg-01/07/00: Define to trace Q.931 to signaling.out */
#if 0 
#include "git_class.h"
#undef TRC1
#undef TRC2
#undef TRC3 
#define TRC1(a)  gitDebugPrint(__FILE__, __LINE__, "TRACE1:" #a "\n");
#define TRC2(a)  gitDebugPrint(__FILE__, __LINE__, "TRACE2:" #a "\n");
#define TRC3(a)  gitDebugPrint(__FILE__, __LINE__, "TRACE3:" #a "\n");
#endif /* TELICA */
  

/* public variable declarations */
typedef struct calId         /* call id */
{
   struct calId *prev;       /* previous entry */
   struct calId *next;       /* next entry */
   Addrs callId;             /* call Identity */
   struct inNtc *ntc;        /* pointer to NTC */
} CalId;

typedef struct calRefTbl     /* call reference table */
{
   struct calRefTbl *prev;   /* previous entry */
   struct calRefTbl *next;   /* next entry */
   CalRef callRef;           /* call reference */
   struct inNtc *ntc;        /* pointer to NTC */
   U32 spare;
} CalRefTbl;

/* FID 16779.0 */
typedef enum DigitMapState_Tag
{
    DMS_INACTIVE = 0,
    DMS_ACTIVE,
    DMS_FAILED_MATCH,
    DMS_PARTIAL_MATCH,
    DMS_FULL_MATCH
} DigitMapState;

typedef struct digitMapCandidateInfo
{
    CmListEnt       list;
    DigitMapState   state;
    U8              index;
    U8              matchLength;
} DigitMapCandidateInfo;

typedef struct digitMapCandidate
{
    CmListEnt       list;
    TknStr          pattern;
    U16             candidateId;
    U8              typeNmb;
} DigitMapCandidate;

typedef struct inDigitMap
{
    CmHashListEnt   hlEnt;
    CmListEnt       candidateList[IN_MAX_TON + 1];
    LngAddrs        digitMapName;
    U16             numberCandidates;
} InDigitMap;

 


/************************************************************************
                    Q.93X Control Block 
************************************************************************/


/* PTMPT007 */
#ifdef NETSIDE
#ifdef ISDN_PTMPT

typedef struct cesTimerCB
{
   S16   index;
   PTR   ntc; 
   PTR   dCb; 
   CmTimer timers[MAXSIMINTTIMER];
   U16   timerCnt;
} CesTimerCb;


#endif  /* NETSIDE */
#endif  /* ISDN_PTMPT */

typedef struct inCB          /* Q.93X Control Block */
{
   CmHashListCp tcrTbl;      /* hash table for tCallRef */
   SuId suId;                /* Service User id */
   SpId spId;                /* Service Provider id */
   Ces ces;                  /* CES */ 
   S16 sapType;              /* SAP Type */
   S16 state;                /* State */
   U8 bndState;              /* Upper SAP Bind State */
   Pst pst;                  /* Post structure */
   Buffer *mBuf;             /* pointer to the received message */
   Buffer *facMBuf;          /* Pointer to the facility message 
                                to be transmitted. */
   AllPdus *msg;             /* pointer to decoded message */
   Bool flc;                 /* Flow Control Flag */
   S16 rstCnt;               /* Restart Counter */
   S16 intType;              /* Interface Type */
   Bool inRstJobFlg;         /* Restart Job Flag */
   Bool inUnBndFlg;          /* Unbind CB flag */
   U8 prefix[MAXPREFDIG];    /* Address Prefix */
   struct inSAPCfg sapCfg;   /* configuration */
   struct inCBCfg cbCfg;     /* configuration */
   CalRefTbl *inNtcPtr;      /* table of call references using this sap */
   U16 numCalRef;            /* number of active call references */
   U8 numCalId;              /* number of suspended calls */
   CalId *callIdTbl;         /* pointer to call IDs table */ 
#ifdef ISDN_SEGMENT
   CalRef segCallRefVal;     /* segment call reference value */
   U8 numSegs;               /* number of segments */
   U8 remSegs;               /* remaining segments */
   Buffer *segMsg;           /* segment message */
   Bool segFlg;              /* segment flag */
#endif
   Bool rstCfmSnt;           /* restart confirm sent flag */
   Bool trc;                 /* trace enabled flag */
   U8 sigState;              /* signalling state */
   U8 endPointState;         /* Endpoint state */
   U8 tid;                   /* Terminal Identifier */
   U8 usid;                  /* User Select Identifier */
   U32 lstRefUsd;            /* last call reference value used */
#ifdef ATT
   U8 srvState;              /* Endpoint service state */
   SuId user;                /* Service User ID */
#endif
   MfCfgProf mfCfgProf;      /* message profile */
   MfMsgCtl mfMsgCtl;        /* message control structure */
   Origin callRefFlag;       /* call Reference Flag */
   S16 tmrCnt;               /* Timer Count */
   CmTimer timers[MAXSIMTIMER];/* Timer structure */

#ifdef NETSIDE
#ifdef ISDN_PTMPT
   CesTimerCb cesTimerCb[MAXCBTIMERBLOCK];   
#endif /* ISDN_PTMPT */
#endif /* NETSIDE */
   U32 dbgMask;		     /* debug tools */	
#ifdef IN_ACC /* TELICA-BUG:18735-mqin */
   U32 incCallCntr;          /* incoming call counter */
#endif /* BUG:18735 */
   Bool rel_force;           /* FID14382.8 */
   Bool nfas_gr; /* Bug 71341 */
   Bool gr_lnkup; /*Bug 81522 */
   Bool gr_relnk; /* Bug 90003: force link isdn link in GR failover */
   U32 rSuId;     /* FID16537.0: real suId */
} InCb;


   

/************************************************************************
                     Bearer channel allocation table
************************************************************************/

typedef struct bChanAllTbl
{
   U8 nmbOfChn;                    /* number of channels */
   U8 bChan[PRIM_EUR_RATE];        /* B Channels allocated */
} BChanAllTbl;


  
/************************************************************************
             Network Layer Connection (one per connection)
************************************************************************/
typedef struct inNtc         /* Q.93X Network Layer Connection */
{
   CmHashListEnt thl;        /* hash list pointers tCallRef */
   UConnId tCallRef;          /* Call reference used by transport */
   InCb *tCallCb;            /* Pointer to transport Control Block */
   InCb *dCallCb;            /* Pointer to data link layer Control Block */
   AllPdus *pduSp;           /* Pointer to PDU message structure */
   AllSdus *sduSp;           /* Pointer to SDU message structure */
   Buffer *mBuf;             /* Pointer to received message - user info */
   Buffer *setUp;            /* Pointer to received setup message */
   CalRef dCallRef;          /* Call reference used by data link layer */
   UConnId suInstId;         /* Service user instance id */
   U8 state;                 /* Network Level Matrix State */
#if 0 /* Deletion : in001.310 */
   U8 dFlc;                  /* Lower Layer Flow Control State */
#endif /* Deletion : in001.310 */
#if 0 /* Deletion : in002.310 */
   U8 tFlc;                  /* Upper Layer Flow Control State */
#endif /* Deletion : in002.310 */
   Bool bChanAs;             /* Indication of B channel assignment */
   BChanAllTbl bChanSel;     /* B channels selected */
   U8 toBeAlctdBChanlDir;    /* To be allocated B channel direction */
   U8 dChanAs;               /* Indication of D channel assignment */
   U8 dConNum;               /* D channel connection number */
   Origin origin;            /* Originator */
   U32 callType;             /* Type of call - circuit, packet, user to user */
   U8 evntType;              /* event type - proceeding, progress, setup ack, 
                                alerting, info */
#if 0 /* Deletion : in001.310 */
   Bool tFlcTx;              /* indication of accepting flc from uppr intrfc */
   Bool dFlcTx;              /* indication of accepting flc from lowr intrfc */
#endif /* Deletion : in001.310 */
   Bool exclFlg;             /* indication of exclusive channel ID */
   Queue tTxQueue;           /* Transmit Queue Control Block */

#if 0 /* Deletion : in001.310 */
   Queue dTxQueue;           /* Transmit Queue Control Block */
#endif /* Deletion : in001.310 */

   CdPtyNmb srcAdr;          /* Source Address */
   CdPtyNmb dstAdr;          /* Destination Address */
   Ticks calDura;            /* call duration */
   Bool susFlg;              /* suspend flag */
   Bool callRedir;           /* call redirected indication */
   U8 redirReason;           /* reason for call redirection */
   U8 redirNmbPlan;          /* redirected number numbering plan */
   U8 redirTypeNmb;          /* redirected number type of number */
   U8 alertCnt;              /* number of alert messages received */
   U8 progCnt;               /* number of progress messages received */
   Bool cmpltAdr;            /* complete address flag */
   TknStrS callId;           /* call Id if call is suspended */
   CauseDgn causeDgn;        /* cause and diagnostic */
   CauseDgn causeDgnUp;      /* cause and diagnostic to upper layer */
   S16 tmrCnt;               /* Timer Count */
   CmTimer timers[MAXSIMTIMER];/* Timer structure */
   U8 numAdrDig;             /* number of address digits received in overLap */
   U8 infoCnt;               /* Information event count */
   U8 bearCap;               /* Bearer capability of the call */
   Bool permUUI;             /* allows UUI with connect response  */
   Bool hlInsrt;             /* ntc is inserted in the hash list  */
   struct inCtldPcb *ctldPcb;       /* Physical control block */
   U8 cid;
#ifdef MNT_REL
   U8 discRcvd;              /* combination of both RelInd and RelReq */
#else
   Bool discRcvd;
#endif
#ifdef ISDN_HOLD
   U8 auxilState;            /* auxliary state */
#endif
   /* PTMPT009 */
#ifdef NETSIDE
#ifdef ISDN_PTMPT
   InCb *cesProcessLst[MAXCBCESNUM];   /* Ces Procesess for this call. */
   U8 cesState[MAXCBCESNUM];
   U8   causeValue;          /* Saved CauseValue if a Release Complete was */
                             /* in response to a Setup. */
   U8   nmbValidCes;         /* Number of currently valid ces. */
   Queue infoMsgQueue;       /* List of information messages received. */
   Bool setupBCast;          /* Set if setup was broadcast.            */
   Bool t312Running;         /* Better than looping to check its presence. */
   Bool dCbPreSelected;      /* Some equipment has responded, but its not yet
                              * been awarded the call */
#endif /* ISDN_PTMPT */
#endif /* NETSIDE */
#ifdef ZQ
  U8 updStatus;              /* ntc status on the standby */
#endif /* ZQ */
#if 1 /* CON_CHANID */
    U8  rscCfmed; 	     /* ntc connection cfm */
#endif
   /* 26758 */
    U8 relLoc;                /* mark the release location for GCC */ 
#ifdef ETSI
    U8  waitForMoreInfo;      /* bit 1 : waiting for INFORMATION */
    U8  nmbCdPty;             /* number of called party digits */
#endif /* 2111 overlap */

   ConEvnt *conEvnt;         /* BUG:37899-mqin */
   U8  numPL_max;            /* FID:15070 */
   Bool check_info;          /* Information message need to check or not */
   U8   lastTypeNmb;         /* Type of number received last time */
   CdPtyNmb cdAdr;           /* Called party digits recieved */
   FacilityStr *facilityStr1;    /* Pointer to Facility Element */
   CallTrcId callTrcId;   /* FID 14984.10 */
   U32       logMask;     /* FID 17167.0 */
   U8        checkRegCfm; /* FID 16970.0 */
   U8        maxDigitMapMatchLength;
   CmListEnt digitMapCandidateInfoList[2];
   Bool      firstChanId;    /* FID 17524.0 */
} InNtc;




/************************************************************************
                            Routing Table
************************************************************************/

/* Routing Table structures */

typedef struct routEntry        /* routing entry */
{
   struct routEntry *prev;      /* previous entry */
   struct routEntry *next;      /* next entry */
   Addrs addr;                  /* route address */
   U16 sapId;                   /* service access point id */
   U8 sapType;                  /* sap type */
   struct dLId pLink;           /* physical link  and CES */
   Bool specCes;                /* specific CES flag */
} RoutEntry;

typedef struct root             /* routing root */
{
   RoutEntry *head;             /* head of the list */
   RoutEntry *tail;             /* tail of the list */
   U16 entries;                 /* number of entries in list */
} Root;               



/************************************************************************
                  D channel connections
************************************************************************/

typedef struct inDChn        /* D channel connections */
{
   S16 numCon;               /* number of connections */
   InNtc *ntc[MAXDCHANCON];  /* pointers to network connection */
} InDChn;

/************************************************************************
                      Bearer channel configuration
************************************************************************/

typedef struct inBearChn     /* bearer channel configuration */
{
   U8 status;                /* available */
   U16 intId;                /* interface Id */
   U16 chnNmb;               /* channel number */
   U8  chnDir;               /* channel direction */
   U8   rst;                 /* waiting for Restart Ack */
   S16 profile;              /* profile */
   SuId suId;                /* service user Id */
   Bool validate;            /* validate bearer capability */
   Bool waitSrvAck;          /* waiting for Service Ack */
   Bool dChan;               /* channel is used as a dChannnel */
   Bool suIdPres;            /* requested by the upper layer */
   U8 pendStat;              /* pending status */
   U8 rstCnt;                /* Number times restart has been sent */
#ifdef NI2_BCAS
   U16 srvCnt;               /* Number times service has been sent */
#endif /* NI2_BCAS */
   InNtc *ntc;               /* pointer to network connection */
   CmTimer timers[MAXBCHTIMER];/* Timer structure */
} InBearChn;



/************************************************************************
                  Controlled PCB Configuration 
************************************************************************/


typedef struct inCtldPcbCfg      
{
  Swtch swtch;                 /* switch */
  Bool clrGlr;                 /* clear B chan glare flag */
  Bool nfasInt;                /* NFAS interface */
  U16 intId;                    /* interface id */
  U16 sigInt;                   /* signalling interface id */
  U8 firstBChanNum;            /* first B channel number 0 or 1 */
  U16 nmbBearChan;             /* number of bearer channels */
  struct
  {              
    S16 profNmb;              /* profile number */
    Bool valid;               /* validate */
    U8 state;                  /* Provisioned/Not Provisioned */
  } bProf[MAXBCHNL];           /* b channel profiles */
  U32 rSigInt;
} InCtldPcbCfg;




/************************************************************************
                  Q.93X Controlled PCB
************************************************************************/

typedef struct inCtldPcb         /* Q.93X control block for physical link */
{
  CmHashListEnt hlEnt;      /* hash table header */
  SuId suId;                /* Service User id */
  SpId spId;                /* Service Provider id */
  S16 state;                /* State */
  InBearChn *bBearer;       /* Bearer channel configuration */
  InCtldPcbCfg cfg;         /* Configuration */ 
  CmTimer timers[MAXBCHTIMER];
  U8 rstCnt;                /* Number Rest has been sent */
  U32 rSuId;                /* FID16537.0: real suId */
} InCtldPcb;





/************************************************************************
                  Signalling PCB Configuration 
************************************************************************/


typedef struct inPcbCfg        
{
  Swtch swtch;                 /* switch */
  S16 intType;                 /* interface type - usr or network */
  S16 intCfg;                  /* point to point or multipoint */
  Bool tCbCfg;                 /* TSAP Id configured */
  U8 tCbId;                    /* TSAP Id */
  S16 n201;                    /* N201 */
  Bool nfasInt;                /* NFAS interface */
  U16 sigInt;                   /* signalling interface id */
  Bool buIntPr;                /* Backup interface present */
  U16 bupInt;                   /* backup interface id */
  U8 maxNmbNfasInt;             /* number of interfaces */
  Bool ackOpt;                 /* acknowledge option */
  U8 nmbRst;                   /* Number Restart  retransmitted */
  U8 numRstInd;                /* number simult disc indic to be sent for 
                                  restarting interface */
  Bool relOpt;                 /* release option if true release is generatd*/
  U8 facilityHandling;         /* Facility Handling: standard or GFT. */
  Bool statEnqOpt;             /* status enquiry option */
  S16 nmbCes;                  /* number of ces per link */
  U8 callRefLen;               /* call reference length */
  U8 teiAlloc;                 /* TEI Allocation */
  U8 minAdrDig;                /* Minimum number of address digits */
  Bool useSubAdr;              /* call routing on subaddress */
  Bool keyPad;                 /* call routing on keyPad */
  Bool wcRout;                 /* Wild Card Routing Flag */
  U8 wcMask[ADRLEN];           /* Wild Card Mask */
  Bool adrPref;                /* use of prefix for international calls */
  U8 nmbPrefDig;               /* number of digits used for prefix */
  U8 prefix[MAXPREFDIG];       /* address prefix */
  TknStrS callId;              /* Default Call Identity */
  Bool sidIns;                 /* SID insertion Flag */
  U8 sidTon;                   /* SID Type of Number */
  U8 sidNPlan;                 /* SID Numbering Plan */
  U8 sidPresInd;               /* default presentation indicator */
  Addrs sid;                   /* SID */
  Bool cndSubsc;               /* calling address delivery service 
                                   subscription */
  Bool redirSubsc;             /* subscription to call redirection */
  Bool forwSubsc;              /* programmed forwarding subscription */
  RedirNmb redirAdr;           /* redirect address */
  Bool srvOpt;                 /* service option */
  Bool rstOpt;                 /* restart option */
#ifdef ATT
  Bool mimOpt;                 /* management/maintenance option */
#endif
#ifdef NT
  Bool ringOff;                /* subscription to non ringing terminal */
#endif
  U8 dChannelNum;             /* Time slot assigned for D-Channel. */
  InTmrCfg tmr;                /* timer configuration */
#ifdef NI2_BCAS 
  Bool bcas;                /* B channel availability subscribed */
  U16 maxBSrvCnt;           /* Maximum number of service retransmission */
#endif   /* NI2_BCAS */
#if 1 /* TELICA-MCAO-PRJ-NI2-SRV_MSG-- begin of code change */
  Bool bcasOpt; 	
#endif  /* TELICA-MCAO-PRJ-NI2-SRV_MSG-- end of code change */
  S16 svcType;                 /* service type - network or user */
#ifdef ETSI
  Bool overlap;                /* Overlap */
  U8 minDigits;                /* Minimum digits */
  U8 maxDigits;                /* Maximum digits */
  U8 numPL;                    /* FID:15070 */
#endif /* 2111 overlap */
  Bool cfCfg;            /* FID: 16206.0 */
  U32 rSigInt;
  U32 rBupInt;
  Bool aoc;                 /* FID 16706 aoc */
  LngAddrs digitMapName;    /* FID 16779 digit map name */
  U8 connectDateTimeIE;     /* FID 17412.0 */
  Bool channelId;           /* FID 17524.0 */
  Bool playDialTone;        /* FID 17555.0 Kavon */
} InPcbCfg;


/************************************************************************
                  Q.93X Physical Control Block
************************************************************************/


typedef struct inPCB         /* Q.93X control block for physical link */
{
  CmHashListEnt hlEnt;      /* hash table header */
#ifdef NETSIDE
#ifdef ISDN_PTMPT  
  InNtc  *mptCallList[MAXNTCNUM];  /* Calls originated from a 
                                    * point to multipoint
                                    * Network side interface.*/
#endif /* ISDN_PTMPT */
#endif /* NETSIDE */
  SuId suId;                /* Service User id */
  SpId spId;                /* Service Provider id */
  U8 bndState;              /* Bind State */
#ifdef DAT2 
  U8 bndRetryCnt;              /* Current Bind Retry Counter */
#endif /* DAT2 */
  InPcbCfg cfg;             /* Configuration */
  Pst pst;                  /* Post structure */
  InSAPSts sts;             /* Statistics */
  InDChn dChan;             /* D channel connections */
  InCb **cesLst;            /* Pointers to CES Control Blocks */
  CmTimer timers[MAXBCHTIMER];/* Timer structure */
  U16 nmbCtldInt;           /* Number of Controlled Interfaces */
  U16 *ctldInt;             /* List of Controlled Interfaces */
  Bool rstAll;              /* all interfaces restarting */
  Bool flc;                 /* Flow Control Flag */
  U32 dbgMask;              /* PLM_IN: used for debug */
  Bool gr_bnd;              /* Bug 90003: force bind isdn link in GR failover */
  U32 rSuId;                /* FID16537.0: real suId */
  Bool tConRestarted;       /* Bug 90131: TCON retarted w/ standing call */
} InPCB;

typedef struct _inGlobalCb          /* IUA Global Control Block */
{
   CmHashListCp pcbHl;             /* interface CB hash list */
   CmHashListCp ctldPcbHl;
   CmHashListCp digitMapHl;        /* digit map hash list */
} InGlobalCb;

extern InGlobalCb inGlobalCb;

/************************************************************************
                 FTHA Support Structures
************************************************************************/


typedef struct inCesRst
{
  U16  intId;                /* interface Id */
  Ces  ces;                  /* channel number */
  U8 calls; 
}InCesRst;

typedef struct inCtldPSapRst
{
  SuId suId;   /* Service Provider id */
  U8 calls;
}InCtldPSapRst;

/* Geographic redundacny - start */
#define IN_GEO_CRI_LEN_MAX 120

#define IN_GEO_MAJOR_VER  1
#define IN_GEO_MINOR_VER  1

extern U8 InGeoCriMajVer;
extern U8 InGeoCriMinVer;

extern void inFillCriInfo (UConnId tCallRef, CctCriList *criList,
                           CctCriEnt *inCri, U8 *string);
extern S16 inDecodeCri    (CctCriList *list, UConnId *ref);
/* Geographic redundacny - end   */


/************************************************************************
             gdixit-12/09/2002-Bug#16931 
************************************************************************/
#ifdef TELICA /* gdixit-12/09/2002-Bug#16931 */ 
typedef struct _IN_MF_ERR_CB {
   U8       msgId;      /* last Message Id */
   U8       elmtId;     /* last element Id */
   U8       cause;      /* last cause code */
   U32      count;      /* how may times it occurred */
   time_t   time;       /* when last time it occurred ? */
   U16      time_diff;  /* after how many second, use DP */
} IN_MF_ERR_CB;

EXTERN IN_MF_ERR_CB inMfRxCb;
EXTERN IN_MF_ERR_CB inMfTxCb;
#endif /* BUG:16931-end */ 


/* 22802: MCAO */
typedef struct _IN_RPT_ALM_CB {
   char     file[20];
   int 	    line;
   U32      count;      /* how may times it occurred */
   time_t   time;       /* when last time it occurred ? */
   U16      time_diff;  /* after how many second, use DP */
} IN_RPT_ALM_CB;


/***************************************************************************
                          Q.93X Matrixes
****************************************************************************/


typedef U16  (*PFMF) ARGS((U8 c, TknElmtDef *tep, U8 * *cmsp));
typedef S16  (*PFSIM) ARGS((InNtc *ntc, InCb *dCb)); 
/* pointer to function return S16 */
typedef S16 (*PFSIEM) ARGS((InCb *cb));  
/* pointer to function return S16 */

#ifdef __cplusplus
extern "C" {
#endif /* _cplusplus */


extern PFSIEM inEv[NMB_EXTEVNT][NMB_LNK_ST];
extern CONSTANT PFSIM inConNet[NMB_CON_EVNT][NMB_CON_ST];
extern CONSTANT PFSIM inConUsr[NMB_CON_EVNT][NMB_CON_ST];


extern InGenCfg inCp;              /* isdn general configuration */
extern Root inTRoutRoot;           /* pointer to head of the routing tables for
                                      TSAP */
extern Root inLRoutRoot;           /* pointer to head of the routing tables for
                                      LSAP */
extern Bool inPCBBnd;              /* physical control blocks bound */
extern U16 inNumProf;              /* number of B channel profiles */
extern InBearCfg *inProfTbl;       /* pointer to B channel configuration 
                                      table */  
extern InCb **inSapLstPtr;         /* pointer to list of SAP structures */
extern InPCB **pcbLstPtr;          /* pointer to list of Control Blocks */
extern InCtldPcb **ctldPcbLstPtr;

#ifdef IN_ACC /* TELICA-BUG:18735-mqin */
extern SDmndQCong_V2 inDmndQCong;
extern U8 *q931Name;
#endif /* BUG:18735 */

/* timing queues */
 
extern S16 prCrtTqEnt;             /* process current timing queue entry */
extern S16 prCrtNtcTq;             /* process current Ntc timing queue entry */
extern S16 prCrtBChTq;             /* process current bearer channel timing 
                                      queue entry */
extern S16 prCrtPcbTq;             /* process current physical link timing 
                                      queue entry */

extern CmTqCp ntcTqCp;             /* network connection timing queue control 
                                      point */
  
extern CmTqCp cbTqCp;              /* control block timing queue cntrol point*/
  
extern CmTqCp bChTqCp;             /* bearer chan timing queue cntrl point */

extern CmTqCp pcbTqCp;            /* physical link timing queue cntrl point */

extern CmTqCp ctldPcbTqCp;        /* physical link timing queue cntrl point */

extern CmTqType ntcTq[TQNUMENT];
  
extern CmTqType cbTq[TQNUMENT];

extern CmTqType bChTq[TQNUMENT];

extern CmTqType pcbTq[TQNUMENT];

extern CmTqType ctldPcbTq[TQNUMENT];

extern CmTqType cbIntTq[TQNUMENT];
extern CmTqCp cbIntTqCp;

extern TskInit inInit;             /* initialization */

extern U8 inMfExt;
extern U8 inMfLen;
extern U8 inMfStk[];
extern U8 inMfSp;
extern U8 inMfOctet;
extern U16 inMfCodeSet;
extern U8 inMfShftLock;

/* user supplied functions */


/* forward references */

extern S16 inGetCesVal ARGS ((InPCB *pcb, Endpoint *ep, Ces *cesVal));
extern S16 inGenRelComplete ARGS ((InCb *dCb, InPCB *pcb, 
                                   CalRef callRef));
extern S16 inSetCauseDgn ARGS ((InCb *cb, CauseDgn *causeDgn, 
                                U8 causeval, U8 dgnlen, U8 dgnval));
extern  S16 inUnBndCb ARGS ((InPCB *pcb, InCb  *cb));
extern  S16 inUnBndTSAP ARGS((SpId spId));
extern  S16 inFreePcb ARGS((InPCB *pcb));
extern  S16 inFreeCtldPcb ARGS((InCtldPcb *ctldPcb));
extern  S16 inValChnlId ARGS((InNtc *ntc, ChanId *chanId));
extern  S16 inInsrtChnlId ARGS((InNtc *ntc, ChanId *chanId, InCtldPcb *ctldPcb));
extern  S16 inInitChan ARGS ((U16 intId, ChanId *chanId, U8 type, U8 chan));
extern  S16 inBldChnlId ARGS((InNtc *ntc, ChanId *chanId, InCtldPcb *ctldPcb));
extern  S16 inIsFirstChnlId ARGS((InNtc *ntc, ChanId *chanId, Bool chnlId)); /* FID 17524.0 */
extern  S16 inGetInstId ARGS((InCb *cb,U32 *ref));
extern  S16 inChkBearCap ARGS((S16 profile,BearCap *bearCap,
                               InNtc *ntc,InPCB *pcb));
extern  S16 inChkDatLnk ARGS((InCb *cb, InNtc *ntc));
extern  S16 inChkLlc ARGS((U8 profile, LlComp *llc, InNtc *ntc));
extern  S16 inChkHlc ARGS((U8 profile, HlComp *hlc, InNtc *ntc));
extern  S16 inGenDiscInd ARGS((InNtc *ntc, CauseDgn *cause));
extern  S16 inGenRelInd ARGS((InNtc *ntc, CauseDgn *cause));
extern  S16 inGetBChan ARGS((InNtc *ntc, ChanId *chanId,TknU16 *intId,
                             BearCap *bearCap,
        CauseDgn *causeDgn));
extern  U8 inStoreAdr ARGS((U8 *adr1,U8 len1,U8 *adr2,U8 len2,U8 *buf));
extern  S16 inGetPktChan ARGS((InNtc *ntc,ChanId *chanId,BearCap *bearCap,
        CauseDgn *causeDgn));
extern  U8 inRelBChan ARGS((InNtc *ntc,ChanId *chanId,TknU8 *intId,
        CauseDgn *causeDgn));
extern  S16 inGetCalRef ARGS((InCb *cb, U16 *ref));
extern  S16 inGenStaNoNtc ARGS((InCb *cb, CauseDgn *cause, CalRef callRef, 
                                Origin callRefFlg));
extern  S16 inSelectChan ARGS((InNtc *ntc,InCtldPcb *ctldPcb,U8 chnNmb,
                               U8 chnType,ChanId *chanId));
extern  S16 inGetChan ARGS((InNtc *ntc,ChanId *chanId,TknU16 *intId,
                            BearCap *bearCap,CauseDgn *causeDgn));
#if 1                                          /* TELICA-dsolanki-INTMGCDEV */
extern S16 inChkDummyRefValid ARGS((InPCB *pcb, U8    msgIdx));
extern S16 inProcDummyMsg  ARGS((InCb     *cb,AllPdus  *message, Buffer   *mBuf));
#endif                                        /* TELICA-dsolanki-INTMGCDEV */

#ifdef ATT
extern  S16 inGenMimRej ARGS((InCb *cb, U8 errCode));
extern  S16 inGenMimSrvRes ARGS((InCb *cb));
extern  S16 inGenMimSrvMsg ARGS((InCb *cb, U8 state));
extern  S16 inGenMimErr ARGS((InCb *cb, U8 errCode));
extern  S16 inGenMimInitReq ARGS((InCb *cb));
extern  S16 inGenMimAckUsid ARGS((InCb *cb));
extern  S16 inGenMimAssign ARGS((InCb *cb));
extern  Bool inCmpSpid ARGS((InCb *cb, TknStrS *spid));
extern  S16 inGenMimSrvEvnt ARGS((InCb *cb));
extern  S16 inGenMimSrvCfmEvnt ARGS((InCb *cb));
extern  S16 inMimGlobal ARGS((InCb *cb));
#endif

extern  S16 inGenRelCLw ARGS((InNtc *ntc, CauseDgn *cause, InCb *dCallCb));
#if 1 /* Bugzilla 11368:taraveti */
#define inRelCalRef(ntc) inRelCalRef_real(ntc, __FILE__, __LINE__)
extern  S16 inRelCalRef_real ARGS((InNtc *ntc, char *file, int line));
#else
extern  S16 inRelCalRef ARGS((InNtc *ntc));
#endif /* Bugzilla 11368:taraveti */
#define inNmReport(category, event, cause, type, link, chanNum) inNmReport_real(__FILE__, __LINE__, category, event, cause, type, link, chanNum)

/* TELICA-jwh-10/24/2000:  link is an suId so it needs to be U16 not U8. */
#if 0   /* Old code */
extern  S16 inNmReport_real ARGS((char *file, int line, U16 category,U16 event,U16 cause,
                             U8 type,U8 link,U8 chanNum));
#else   /* Start of code change */
extern  S16 inNmReport_real ARGS((char *file, int line, U16 category,U16 event,U16 cause,
                             U8 type,U16 link,U8 chanNum));
#endif  /* End of code change */


extern  S16 inIsANumber ARGS((S16 *p,U8 c,S16 base));
extern  S16 inAscAdrToBcd ARGS((U8 *inpBuf,U8 *bcdBuf));
extern  S16 inCmpAddr ARGS((U8 *adr1,U8 len1,U8 *adr2,U8 len2,U8 *mask));
extern  S16 inCmpAddr1 ARGS((U8 *adr1,U8 len1,U8 *adr2,U8 len2,U8 *mask));
extern  S16 inCmpAscAddr ARGS((U8 *adr1,U8 len1,U8 *adr2,U8 len2,U8 *mask));
extern  CalId *inGetCalEntry ARGS((InCb *cb, U16 index));
extern  CalId *inCallIdSearch ARGS((InCb *cb, TknStrS *id, U8 *causVal));
extern  CalRefTbl *inGetCalRefEntry ARGS((InCb *cb, U16 index));
extern  CalRefTbl *inCallRefSearch ARGS((InCb *cb, CalRef callRef));
extern  S16 inInsCallRef ARGS((InCb *cb, CalRefTbl *entry,CalRefTbl *callRef));
extern  S16 inInsCallRefEnt ARGS((InCb *cb, CalRef callRef, InNtc *ntc));
extern  S16 inInsCallRefEnt1 ARGS((InCb *cb, InNtc *ntc));
extern  S16 inRemCallRef ARGS((InCb *cb, CalRef callRef));
extern  S16 inGetSlot ARGS((BChanAllTbl *p, ChanId *chanId, CauseDgn *causeDgn,
        InCtldPcb *ctldPcb));
extern  S16 inFillSlotMap ARGS((BChanAllTbl *p, ChanId *chanId, 
                                InCtldPcb *ctldPcb));
extern  InNtc *inGetNtc ARGS((InCb *cb,UConnId callRef));
extern  InNtc *inFindNtc ARGS((InCb *tCb, UConnId suInstId, 
                               UConnId spInstId));
extern  InNtc *inSelectNtc ARGS((InCb *tcb,U32 suInstId,
                                 U32 spInstId,StaEvnt *staEvnt));
extern  InCb *inPrcGlblCnStReq ARGS((SuId dChnl,Ces ces,SpId spId,
                                     StaEvnt *staEvnt, CnStEvnt *cnStEvnt, 
                                     U8 event));

/* TELICA-jwh-01/09/2001:  BCHANSYNC: Add INT B channel status indication */
#if 1   /* Start of code change */
extern  S16 inGenBChanStaInd ARGS((char *file, int line, InBearChn *bBearer));
#endif  /* End of code change */

extern char* inGetSigStateStr ARGS((U8 sigState));

extern  S16 inGenStaCfm ARGS((InNtc *ntc, CauseDgn *cause));
extern  S16 inGenStaMsg ARGS((InNtc *ntc, CauseDgn *cause, InCb *dCb));
#if 1 /* TELICA-BUG:15265-mqin */
extern  S16 inGenStatusReq ARGS((InNtc *ntc, InCb *dCb));
#endif /* BUG:15265 */
extern  S16 inGenStaEnq ARGS((InNtc *ntc, InCb *dCallCb));
extern  S16 inGenPrgMsg ARGS((InNtc *ntc, CauseDgn *cause, InCb *dCallCb));
extern  S16 inInsCalEnt ARGS((InCb *cb, CalId *entry, CalId *callId));
extern  S16 inInitCalRedir ARGS((InNtc *ntc, AllSdus *ev, Swtch swtch));
extern  S16 inInsCallId ARGS((InCb *cb, TknStrS *id, InNtc *ntc));
extern  S16 inRemCallId ARGS((InCb *cb, CalId *entry));
extern  RoutEntry *inGetRtEntry ARGS((U16 index, Root *tmpRoot));
extern  RoutEntry *inRoutSearch ARGS((U8 *adr,U8 len,U8 *mask, U8 sapType));
extern  RoutEntry *inRoutSearch1 ARGS((U8 *adr,U8 len,U8 *mask, U8 sapType));
extern  S16 inInsRtAddr ARGS((RoutEntry *entry, RoutEntry *rout, U8 sapType));
extern  S16 inInsRout ARGS((InRoutCfg *rout));
extern  InCtldPcb *inSearchIntId ARGS((InPCB *pcb, U16 intId));
extern  S16 inClnNtc ARGS((InNtc *ntc));
extern  S16 inGenRelUpCLw ARGS((InNtc *ntc, CauseDgn *cause, InCb *dCallCb));
#ifdef TELICA /* gdixit-06/06/2002-BUG:9909 */
extern  S16 inGenRelUpCLw2 ARGS((InNtc *ntc, CauseDgn *cause, InCb *dCallCb,
                                                               U8 causeVal));
#endif /* TELICA-BUG:9909-end */
extern  S16 inGenDisUpLw ARGS((InNtc *ntc, CauseDgn *cause, InCb *dCallCb,
                                                               U8 causeVal));
extern  S16 inGenRelUpLw ARGS((InNtc *ntc, CauseDgn *cause, InCb *dCallCb));
extern  S16 inGenRelLw ARGS((InNtc *ntc, CauseDgn *cause, InCb *dCb));
extern  S16 inCalDur ARGS((InNtc *ntc));
#if 1 /* TELICA debug for bug 36407 */
#define inRelChan(ntc) inRelChan_real(ntc, __FILE__, __LINE__)
extern  S16 inRelChan_real ARGS((InNtc *ntc, char *file, int line));
#else
extern  S16 inRelChan ARGS((InNtc *ntc));
#endif
extern  S16 inAscMaskToBcd ARGS((U8 *mask, U8 *buf));
extern  S16 inGenBillNm ARGS((InNtc *ntc));
extern  S16 inGetAdr ARGS((InNtc *ntc,ConEvnt *conEvnt));
extern  S16 inNegChan ARGS((InNtc *ntc, ChanId *chanId, CauseDgn *causeDgn));
extern  U8 inGetRstAck ARGS((InCb *cb, InPCB *pcb, InNtc *ntc, U8 type));

#if (ISDN_NTNI || ISDN_NTDMS250 || ISDN_NT || ISDN_NTMCI)
extern  S16 inGenRstAckSndInd ARGS((InCb *cb, InPCB *pcb, RstAck *rstAck));
#endif /* (ISDN_NTNI || ISDN_NTDMS250 || ISDN_NT || ISDN_NTMCI) */
#if 1 /*Addition : in034.310  */
#ifdef NI2
extern  S16 inRstAck4Col ARGS((InCb *cb, InPCB *pcb, InCtldPcb *cltdPcb,
                                U8 type));
#endif /* ifdef NI2 */
#endif /* Addition : in034.310  */

extern  U8 inGetRedirDst ARGS((InCb *cb, InPCB *pcb, InNtc *ntc, 
           InCb **redirCb));
extern  U8 inGetDstSubAdr ARGS((InCb *cb, CdPtySad *sAdr,CdPtyNmb *adr));
extern  U8 inGetDstNmb ARGS((InCb *cb, CdPtyNmb *num,CdPtyNmb *adr));
extern  S16 inFillChanId ARGS(( InCb *cb, InCtldPcb *ctldPcb,ChanId *chanId));
extern  S16 inRstJob ARGS((InCb *cb, InPCB *pcb, U8 chnlType, S16 chnlNum, 
                           SuId suId));

extern  Void inNtcTmrEvnt ARGS((PTR cb,S16 event));
extern  Void inCbTmrEvnt ARGS((PTR cb,S16 event));
extern  Void inBChTmrEvnt ARGS((PTR cb,S16 event));
extern  Void inTrcBuf ARGS((SuId suId, U16 evnt, Buffer *mBuf));
extern  S16 inChkNfas ARGS((InPCB *pcb, InCb *cb));
extern  Void inPcbTmrEvnt ARGS((PTR cb,S16 event));
extern  Void inCtldPcbTmrEvnt ARGS((PTR cb,S16 event));
extern  S16 inPrcCbTq ARGS((void ));
extern  S16 inPrcPcbTq ARGS((void ));
extern  S16 inPrcCtldPcbTq ARGS((void ));
extern  S16 inPrcBChTq ARGS((void ));
extern  S16 inPrcNtcTq ARGS((void ));
extern  S16 inRmvNtcTq ARGS((InNtc *ntc,U8 tmrNum));
extern  S16 inRmvCbTq ARGS((InCb *cb,U8 tmrNum));
extern  S16 inRmvBChTq ARGS((InBearChn *bChan, U8 tmrNum));
extern  S16 inRmvPcbTq ARGS((InPCB *pcb, U8 tmrNum));
extern  S16 inRmvCtldPcbTq ARGS((InCtldPcb *ctldPcb, U8 tmrNum));
extern  S16 inChkCbTmr ARGS((S16 timer,InCb *cb));
#ifdef TELICA /* Bug 44121 */
extern  S16 inStartCbTmr_real ARGS((S16 timer,InCb *cb, char *file, int line));
#define inStartCbTmr(A, B) inStartCbTmr_real(A, B, __FILE__, __LINE__)
#else
extern  S16 inStartCbTmr ARGS((S16 timer,InCb *cb));
#endif
extern  S16 inStartPcbTmr ARGS((S16 timer,InPCB *pcb));
extern  S16 inStartCtldPcbTmr ARGS((S16 timer,InCtldPcb *ctldPcb));
#ifdef TELICA /* Bug 44121 */
extern  S16 inStartBChTmr_real ARGS((S16 timer,InBearChn *bChan, char *file, int line));
#define inStartBChTmr(A, B) inStartBChTmr_real(A, B, __FILE__, __LINE__)
#else
extern  S16 inStartBChTmr ARGS((S16 timer,InBearChn *bChan));
#endif
extern  InCb *inGetSigCb ARGS((InPCB *pcb));
extern  S16 inGetBkup ARGS((InPCB *pcb,InCb **cb));
extern  S16 inPrcTSpid ARGS((InCb *cb));
extern  S16 inStopCbTmr ARGS((S16 timer,InCb *cb));
extern  S16 inRestartCbTmr ARGS((S16 timer,InCb *cb));
extern  S16 inChkNtcTmr ARGS((S16 timer,InNtc *ntc));
#ifdef TELICA /* Bug 44121 */
extern  S16 inStartNtcTmr_real ARGS((S16 timer,InNtc *ntc,InCb *cb, char *file, int line));
#define inStartNtcTmr(A, B, C) inStartNtcTmr_real(A, B, C, __FILE__, __LINE__)
#else
extern  S16 inStartNtcTmr ARGS((S16 timer,InNtc *ntc,InCb *cb));
#endif
extern  S16 inStopNtcTmr ARGS((S16 timer,InNtc *ntc));
extern  S16 inRestartNtcTmr ARGS((S16 timer,InNtc *ntc,InCb *cb));
extern  S16 inDropData ARGS((Buffer *mBuf));
extern  S16 inUsrChkStateCmp ARGS((InPCB *pcb, U8 stateU, U8 stateN));

extern  S16 inByteCopy ARGS((U8 *dst, U8 *src, Size size));
extern  S16 inGenStaNoStateMsg ARGS((InNtc *ntc,CauseDgn *cause,
  InCb *dCallCb));
#ifdef ISDN_SEGMENT
extern  S16 inDiscSegMsg ARGS((InCb *cb));
extern  S16 inTxSegMsgChk ARGS((InCb *cb,Buffer *mBuf, MsgLen segIdx[], 
        S16 *numSegs, 
        S16 *hdrLen, Data msgHdr[]));
extern  S16 inRxSegMsgChk ARGS((InPCB *pcb,InCb *cb,Buffer **mBuf,
        U8 callRefLen, CalRef callRef,S16 *numSegs));
#endif

extern  S16 inNoAction ARGS((InCb *cb));
extern  S16 inIpe ARGS((InCb *cb));
extern  S16 inIse ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inRstReqSt1 ARGS((InCb *cb));
extern  S16 inSrvReqSt1 ARGS((InCb *cb));
extern  S16 inSrvReqSt2 ARGS((InCb *cb));
extern  S16 inSrvReqSt2 ARGS((InCb *cb));
extern  S16 inSrvAckSt1 ARGS((InCb *cb));
#ifdef NI2_BCAS
extern  S16 inSrvAckSt2 ARGS((InCb *cb));
#endif /* NI2_BCAS */
extern  S16 inGenSrvReq ARGS((InCb *cb, ChanId *chanId, U8 srvStat));
extern  S16 inGenSrvAck ARGS((InCb *cb, Srv *srv, U8 srvStat));
extern  S16 inPrcSrvReq ARGS((InCb *cb, InBearChn *bearChn, Srv *srv));
extern  S16 inPrcSrvEvnt ARGS((InCb *cb, SuId suId, InBearChn *bearChn, 
        Srv *srv));
extern  S16 inPrcSrvAck ARGS((InCb *cb, InBearChn *bearChn, SrvAck *srvAck));
#if 0 /* deletion : in015.310 */
extern  S16 inGenRstAck ARGS((InCb *cb, InPCB *pcb, InNtc *ntc, U8 type));
#else /* addition : in015.310 */
extern  S16 inGenRstAck ARGS((InCb *cb, InPCB *pcb, InCtldPcb *ctldRstPcb,
                              InNtc *ntc, U8 type));
#endif /* change : in015.310 */
extern  S16 inGenSrvEvnt ARGS((InCb *cb, Srv *srv));
extern  S16 inGenSrvCfmEnvt ARGS((SuId suId, Srv *srv, SuId dChan, Ces ces));
extern  S16 inRstReqSt2 ARGS((InCb *cb));
extern  S16 inRstReqSt4 ARGS((InCb *cb));
extern  S16 inRstAckSt1 ARGS((InCb *cb));
extern  S16 inRstAckSt2 ARGS((InCb *cb));
extern  S16 inRstAckSt3 ARGS((InCb *cb));
#ifdef NI2_BCAS
extern  S16 inNI2RstSrvReq ARGS((InCtldPcb *ctldPcb, InPCB *pcb, U8 type,
        ChanId *rstChanId, InCb *dCb, Bool change));
extern  S16 inPrcNI2SrvReq ARGS((InCb *cb, InBearChn *bearChn, Srv *srv,
                                 InCtldPcb *ctldPcb));
extern  S16 inPrcNI2SrvEvnt ARGS((InCb *cb, SuId suId, InBearChn *bearChn, 
                                  Srv *srv));
extern  S16 inPrcNI2SrvAck ARGS((InCb *cb, InBearChn *bearChn, SrvAck *srvAck));
extern  S16 inNI2Audit ARGS((InCb *cb, InBearChn *bearChn));
extern  S16 inNI2CauseValChk ARGS((InNtc *ntc, InCb *dCb));
#endif  /* NI2_BCAS */
extern  InCb *inGetSapPtr ARGS((SuId suId));
extern  InCb *inGetCBPtr ARGS((SuId suId,Ces ces));
extern  S16 inRmvAdrPref ARGS((U8 *adr,U8 adrLen,U8 prefLen,U8 type,U8 *buf));
extern  S16 inBndConCbs ARGS((Swtch swtch ));
extern  S16 inActvInt ARGS((InCb *cb));
extern  S16 inActNet ARGS((InCb *cb,InNtc *ntc,U8 event));
extern  S16 inActDat ARGS((InCb *cb,InNtc *ntc,U8 event));
extern  S16 inErrMapFunc ARGS((MfMsgCtl *msgCtlp,CauseDgn *causeDgn));
extern  U16 inMfInitErrSt ARGS((Swtch swtch,MfMsgCtl *errsp));
extern  S16 inInitPduHdr ARGS((U8 protDisc, Origin origin, U8 callRefLen, 
        U16 callRef, U8 msgType, PduHdr *pduHrd));
extern  S16 inGenPdu_real ARGS((char *file, int line, InCb *cb,PduHdr *pduHdr,AllPdus *allPdus,
        Swtch swtch,U32 flags));
#define inGenPdu(a,b,c,d,e) inGenPdu_real(__FILE__,__LINE__,a,b,c,d,e)
extern  S16 inBldMsg ARGS((InCb *cb, PduHdr *pduHdr, AllPdus *msg,
        Region region,Pool pool,
        Buffer **mBuf, Swtch swtch,U32 flags));
extern  S16 inSndMsg ARGS((InCb *cb,Buffer *mBuf));
extern  S16 inBldElmtBearCap ARGS((InBearCfg *bearCfg,BearCap *bearCap));
extern  S16 inAssnCause ARGS((InPCB *pcb,CauseDgn *causeDgn));
extern  S16 inChkComprehension ARGS((U8 infByte));

#ifdef ISDN_FACILITY
#ifdef QSIG
extern  S16 inPrcQSigFacReq ARGS ((InNtc *ntc, InCb *dCb, InPCB *pcb, FacEvnt *facEvnt));
#endif /* QSIG */
/* Change : in001.310 */
#if (ISDN_NT || ISDN_NTDMS250) 
extern S16 inGenNtFacReq ARGS ((InNtc *ntc, InCb *dCb, InPCB *pcb, 
                                FacEvnt *facEvnt, U8 evntType));
extern S16 inIssueNtFacInd ARGS ((InPCB *pcb, Facil *facil, 
                                  Bool globRef, U8 evntType, Ces ces)); 
#endif /* NT || NTDMS250 */
/* End Change : in001.310 */
#ifdef ETSI /* FID 16206.0 */
extern S16 inProcDumyFac ARGS ((InPCB *pcb, Facil *facil, U8 evntType, Ces ces));
#endif
#endif /* ISDN_FACILITY */

extern  S16 inChkDChan ARGS((MfMsgCtl *msgCtlp, U32 val));

extern  S16 mfPrntErr ARGS((MfMsgCtl *m));
extern  S16 inNetT301S07 ARGS((InNtc *ntc));
extern  S16 inNetT302S02 ARGS((InNtc *ntc));
extern  S16 inNetT303S06 ARGS((InNtc *ntc));
extern  S16 inNetT304S25 ARGS((InNtc *ntc));
extern  S16 inNetT305S12 ARGS((InNtc *ntc));
extern  S16 inNetT306S12 ARGS((InNtc *ntc));
extern  S16 inNetT306S18 ARGS((InNtc *ntc));
extern  S16 inNetT307S00 ARGS((InNtc *ntc));
extern  S16 inNetT308S19 ARGS((InNtc *ntc));
extern  S16 inNetT310S09 ARGS((InNtc *ntc));
extern  S16 inNetTMINS02 ARGS((InNtc *ntc));
#if 1 /* TELICA-BUG:5126-mqin - Telcordia ISDN Issue 119 */
extern  S16 inNetTPROGS09 ARGS((InNtc *ntc));
#endif /* BUG:5126 */
extern  S16 inNetT313S08 ARGS((InNtc *ntc));
extern  S16 inNetTCON ARGS((InCb *cb));
extern  S16 inNetT314 ARGS((InCb *cb));
extern  S16 inCbTmrRstAck ARGS((InCb *dCb));
extern  S16 inNetT316C ARGS((InBearChn *bChan));
#if 0 /* Deletion : in025.310 */
#if 1 /* addition : in014.310 */
#ifdef NI2
#ifdef NI2_TREST
extern  S16 inNetTREST ARGS((InBearChn *bChan));
#endif /* NI2_TREST */
#endif /* NI2 */
#endif /* addition : in014.310 */
#endif /* Deletion : in025.310 */
#if 1 /* Addition : in025.310 */
#ifdef NI2
#ifdef NI2_TREST
extern  S16 inNetTREST ARGS((InPCB *pcb));
extern  S16 inMkBChnAvailable ARGS((InPCB *pcb, InCtldPcb *ctldPcb, U8 intType));
#endif /* NI2_TREST */
#endif /* NI2 */
#endif /* Addition : in025.310 */
#if 1 /* Addition : in030.310   */
#ifdef ISDN_RLNK
extern  S16 inSndStaEnqDLFail ARGS((InCb *cb));
extern  S16 inPrcDLFailCal ARGS((CalRefTbl *entry));
#endif /* ISDN_RLNK */ 
#endif /* Addition : in030.310  */
#if 1  /* Addition : in026.310 */
extern S16 inGenRstInt ARGS((InCb *cb, InCtldPcb *ctldPcb, U8 intType));
extern S16 inGenRstBChan ARGS((InCb *cb, InCtldPcb *ctldPcb, 
                              InBearChn *bearChn));
extern S16 inChkRstState ARGS((InCb *cb));
#endif /* Addition : in026.310 */
extern  S16 inExpTINT ARGS((InPCB *pcb));
extern  S16 inNetT316 ARGS((InCtldPcb *ctldPcb));
extern  S16 inNetTRST ARGS((InCtldPcb *ctldPcb));
extern  S16 inNetTRSTACK ARGS((InCtldPcb *ctldPcb));
extern  S16 inNetT332 ARGS((InBearChn *bChan));
extern  S16 inNetT332I ARGS((InCb *cb));
extern  S16 inNetT322S00 ARGS((InNtc *ntc));
extern  S16 inNetT322S01 ARGS((InNtc *ntc));
extern  S16 inNetT330SND ARGS((InNtc *ntc));
extern  S16 inNetT331SND ARGS((InNtc *ntc));
extern  S16 inUsrT301S04 ARGS((InNtc *ntc));
extern  S16 inUsrT302S25 ARGS((InNtc *ntc));
extern  S16 inUsrT304S02 ARGS((InNtc *ntc));
extern  S16 inUsrT310S03 ARGS((InNtc *ntc));
extern  S16 inUsrT313S08 ARGS((InNtc *ntc));
extern  S16 inUsrT318S17 ARGS((InNtc *ntc));
extern  S16 inUsrT319S15 ARGS((InNtc *ntc));
extern  S16 inUsrT322S01 ARGS((InNtc *ntc));
extern  S16 inUsrTAnsSND ARGS((InNtc *ntc));
extern  S16 inUsrT396S01 ARGS((InNtc *ntc));
extern  S16 inUsrT397S00 ARGS((InNtc *ntc));
extern  S16 inIgnore ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inNetVioErr ARGS((InNtc *ntc, InCb *dCallCb));
extern  S16 inNetINVS00 ARGS((InNtc *ntc, InCb *dCallCb));
extern  S16 inNetEVTINV ARGS((InNtc *ntc, InCb *dCallCb));
extern  S16 inNetINES00 ARGS((InNtc *ntc, InCb *dCallCb));
extern  S16 inNetMSGINV ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inNetE00S06 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inNetE00S07 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inNetE00S09 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inNetE00S25 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inNetE01S06 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inNetE01S25 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inNetE02S06 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inNetE02S07 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inNetE02S09 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inNetE02S25 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inNetE03S08 ARGS((InNtc *ntc, InCb *dCb));
#if 1 /* TELICA-BUG:5126-mqin-11/19/01 - cause code Issue 76 */
extern  S16 inNetE03S10 ARGS((InNtc *ntc, InCb *dCb));
#endif /* BUG:5126 */
extern  S16 inNetE04S06 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inNetE04S07 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inNetE04S09 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inNetE04S25 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inNetE05S00 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inNetE05S24 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inNetE06S06 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inNetE07S00 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inNetE10S10 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inNetE13S04 ARGS((InNtc *ntc, InCb *dCallCb));
extern  S16 inNetE13DSC ARGS((InNtc *ntc, InCb *dCallCb));
extern  S16 inNetE13SND ARGS((InNtc *ntc, InCb *dCallCb));
extern  S16 inNetE13S19 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inNetE14S01 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inNetE14S09 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inNetE14SND ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inNetE14S12 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inNetE14S18 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inNetE15S00 ARGS((InNtc *ntc, InCb *dCallCb));
extern  S16 inNetE15SND ARGS((InNtc *ntc, InCb *dCallCb));
extern  S16 inNetE15S12 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inNetE15S19 ARGS((InNtc *ntc, InCb *dCallCb));
extern  S16 inNetE16S00 ARGS((InNtc *ntc, InCb *dCallCb));
extern  S16 inNetE16S06 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inNetE16SND ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inNetE18S10 ARGS((InNtc *ntc, InCb *dCallCb));
extern  S16 inNetE19S00 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inNetE19SND ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inNetE19S12 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inNetE19S02 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inNetE19S24 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inNetE20SND ARGS((InNtc *ntc, InCb *dCallCb));
extern  S16 inNetE21SND ARGS((InNtc *ntc, InCb *dCallCb));
extern  S16 inNetE21S07 ARGS((InNtc *ntc, InCb *dCallCb));
extern  S16 inNetE21S00 ARGS((InNtc *ntc, InCb *dCallCb));
extern  S16 inNetE21S19 ARGS((InNtc *ntc, InCb *dCallCb));
extern  S16 inNetE22SND ARGS((InNtc *ntc, InCb *dCallCb));
extern  S16 inNetE23S07 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inNetE23S09 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inNetE23S31 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inNetE24S00 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inNetE27S00 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inNetE27S11 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inNetE28S02 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inNetE28S01 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inNetE28S03 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inNetE29S01 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inNetE29S04 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inNetE29S07 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inNetE29SND ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inNetE29S10 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inNetE29S15 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inNetE30SND ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inNetE30S01 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inNetE30S03 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inNetE30S11 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inNetE31S00 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inNetE31SND ARGS((InNtc *ntc, InCb *dCallCb));
extern  S16 inNetE31S06 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inNetE32S10 ARGS((InNtc *ntc, InCb *dCallCb));
extern  S16 inNetE34S15 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inNetE36S17 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inNetE37S10 ARGS((InNtc *ntc, InCb *dCallCb));
extern  S16 inNetE38SND ARGS((InNtc *ntc, InCb *dCallCb));
extern  S16 inNetE31S13 ARGS((InNtc *ntc, InCb *dCallCb));
extern  S16 inNetE39S00 ARGS((InNtc *ntc, InCb *dCb));

extern  S16 inNetENDS08 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inNetE01S07 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inCallAbortAllStates ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inNetE06S07 ARGS((InNtc *ntc, InCb *dCb));

extern  S16 inUsrINVS00 ARGS((InNtc *ntc, InCb *dCallCb));
extern  S16 inUsrMSGINV ARGS((InNtc *ntc, InCb *dCallCb));
extern  S16 inUsrE00S01 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE00S02 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE00S03 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE00S04 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE00S12 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE01S01 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE01S02 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE02S01 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE02S02 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE02S03 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE02S04 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE03S08 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE03S10 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE04S01 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE04S02 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE04S03 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE04S04 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE04S10 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE05S00 ARGS((InNtc *ntc, InCb *dCb));
/*
 * the following function has been added for bellcore
 * this accepts setup in permanent signal state.
 */
extern  S16 inUsrE05S24 ARGS((InNtc *ntc, InCb *dCb));

extern  S16 inUsrE06S01 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE05S06 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE05S11 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE05SND ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE07S00 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE08S17 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE09S17 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE10S10 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE11S15 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE12S15 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE13S12 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE13S19 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE14SND ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE14S06 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE14S09 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE14S11 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE15S11 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE15S19 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE16S11 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE18S04 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE19S00 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE19S02 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE19S03 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE19S07 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE19S09 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE19S10 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE19S12 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE19S19 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE19S25 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE21S11 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE27S00 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE28S06 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE28S07 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE28S25 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE29S00 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE29S03 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE29S06 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE29S07 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE29S10 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE29SND ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE29S24 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE30SND ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE30S06 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE30S09 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE31S00 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE31S01 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE31S02 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE31S03 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE31SND ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE33S10 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE34S15 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE35S00 ARGS((InNtc *ntc, InCb *dCb));
extern  S16 inUsrE36S17 ARGS((InNtc *ntc, InCb *dCb));
       
extern  S16 inGetSId ARGS((SystemId *s));
extern  S16 inGenRstInd ARGS((InCb *cb, InPCB *pcb, U8 event, S16 chnlNum, 
                              Rst *rst));
extern  S16 inGenRstAllInt ARGS((InCb *cb));
extern  Void inGenSrvMsgAll ARGS((InCb *cb, InPCB *pcb));

extern  S16 inGenRstCfm ARGS((InCb *cb, InPCB *pcb, U8 event));
extern S16 inActvInit ARGS((Ent entity,Inst inst,Region region,Reason reason));
extern  S16 inActvTskNew ARGS((Pst *pst, Buffer *mBuf));
extern  S16 inActvTsk ARGS((Priority priority,Route route,Ent ent,Inst inst,
            Buffer *mBuf));
extern  S16 inInitialize ARGS((Ent entity,Inst inst,
                               Region region,Reason reason));
extern  S16 inInitEndPoint ARGS((InCb *cb, InPCB *pcb, AllPdus *message));
extern  S16 inInitExt ARGS((void ));
extern  S16 inChkMsgComp ARGS((InPCB *pcb, InNtc *ntc, U8 msgIdx));
#ifdef TELICA
extern  S16 inGenAlarm_real ARGS((U16 category,U16 event,U16 cause,SuId suId,char *file,int line));
#define inGenAlarm(A,B,C,D) inGenAlarm_real (A,B,C,D, __FILE__, __LINE__)
#else /* TELICA */
extern  S16 inGenAlarm ARGS((U16 category,U16 event,U16 cause,SuId suId));
#endif /* TELICA */
extern  S16 inGenInvCallRef ARGS((InCb *tCb,UConnId suInstId,
                                  UConnId spInstId, U8 cause));
extern  S16 inGenTCbError ARGS((InCb *tCb, UConnId suInstId));
extern  S16 inPrcNI2DatLnkFailure ARGS((CalRefTbl *entry));

extern S16 inChkETSIBearerVal ARGS((BearCap *bearCap));

#ifdef NI2
extern S16 inChkNI2BearerVal ARGS((BearCap *bearCap));
extern S16 inGetNtcState ARGS((U8 msgIdx,U8 *state));
#endif

extern S16 inPrcStndFacReq ARGS ((Pst *pst,SpId spId,
                                  UConnId suInstId,
                                  UConnId spInstId,FacEvnt  *facEvnt,
                                  U8 evntType,SuId dChan, Ces ces));

#if (ISDN_BC303TMC || ISDN_BC303CSC)
extern S16 inChkBC303CallRef ARGS ((CalRef callRef));
#endif /* (ISDN_BC303TMC || ISDN_BC303CSC) */

#ifdef NETSIDE
extern S16 inNetChkStateCmp ARGS((U8 stateN, U8 stateU));
#endif

#ifdef Q932
extern  S16 inGenCnStaInd ARGS((InCb *cb, InPCB *pcb, 
                                AllPdus *message));
#endif

#ifdef ISDN_HOLD
extern  S16 inProcHldRtrReq ARGS((InNtc *ntc, Action action));
extern  S16 inProcHldRtrRsp ARGS((InNtc *ntc, Action action));
extern  S16 inProcHoldRetrMsg ARGS((InNtc *ntc, Action action));
#endif

/*
 * The following is to check for Call Reference for BellCore 
 */
#if (ISDN_BC303TMC || ISDN_BC303CSC)
extern S16 inChkBC303CallRef ARGS ((CalRef callRef));
#endif /* (ISDN_BC303TMC || ISDN_BC303CSC) */

#ifdef ISDN_FACILITY
#ifdef QSIG
extern S16 inGenFacMessage ARGS((InCb *dCb, S16  callRefLen,
                                        U32  flags, U8   origin, 
                                        FacEvnt *facEvnt, 
                                        CalRef callRef, Buffer **mBuf,
                                        Swtch swtch));
extern S16 inIssueQSigFacInd ARGS ((InNtc *ntc, InPCB *pcb, 
                                    Facil *facil,Bool globRef));
#endif /* QSIG */
#endif /* ISDN_FACILITY */

#ifdef QSIG
extern S16 inChkQsigNotInd ARGS ((InCb *cb,AllPdus *allPdus));
#endif

#ifdef ISDN_SEGMENT
extern S16 inXmitSegMsg ARGS ((InCb *cb, Buffer *mBuf));
#endif /* ISDN_SEGMENT */


#ifdef ATT 
extern  S16 inBriDChanSrvReq ARGS((InCb *dCb, Srv *srvEvnt, SpId spId));
#endif /*ATT*/

#ifdef ISDN_PRI
extern  S16  inBChanSrvReq  ARGS((InCb *dCb, InCb *tCb, Srv *srvEvnt));
#endif /*ISDN_PRI*/

#if 1 /* addition : in011.310 */
#ifdef ISDN_NFAS
extern void inXferActvCalls ARGS((InCb *cb, InCb *sbCb));
#endif /* ISDN_NFAS */
#endif /* addition : in011.310 */

#ifdef IN_PBD
extern S16 inCtrldDChanSwtchOver ARGS((InPCB *sigPcb, 
				 InCtldPcb *sigCtldPcb, Ces ces));

#endif

#ifdef ISDN_SRV
extern  S16 inDChanSwtchOver ARGS((InCb *cb,Srv *srv,InPCB *pcb));

#ifdef ISDN_NFAS
extern S16 inDChanSrvAck ARGS((InPCB *pcb, InCb *cb, SrvAck *srvAck));
#if 0 /* deletion : in011.310 */
extern void inXferActvCalls ARGS((InCb *cb, InCb *sbCb));
#endif /* deletion : in011.310 */
extern S16 inReestablishLnk ARGS((InCb *sbCb));
#endif /* NFAS */

#if (ISDN_ATT || ISDN_NT || ISDN_NTDMS250 || ISDN_NTMCI || ISDN_NTNI || ISDN_QSIG || ISDN_NI2)


#ifdef ISDN_NFAS
extern S16 inGenSrvAckPdu ARGS((InCb *cb, InPCB *pcb, Srv *srv));
extern void inClearInactvCalls ARGS((InCb *cb));
#endif /* ISDN_NFAS */

#ifdef ISDN_PRI
extern S16 inBChanSrvMsg ARGS((Srv *srv, InCtldPcb *ctldPcb, InPCB *pcb, InCb *cb));
#endif /*ISDN_PRI*/

#endif /* (ISDN_ATT || ISDN_NT || ISDN_NTDMS250 || ISDN_NTMCI || ISDN_NTNI || ISDN_QSIG || ISDN_NI2)*/

#ifdef ISDN_PRI
extern S16 inBChanSrvAck ARGS((SrvAck *srvAck, InCb *cb, InPCB *pcb,InCtldPcb *ctldPcb));
#endif /*ISDN_PRI*/
#endif /* ISDN_SRV */

extern Void inGetActvDChan ARGS((SuId *actvDChan));
extern S16  inSendSrvInd ARGS((InCb *cb,InPCB *pcb, Srv *srv));
extern S16 inRstReqIndChan ARGS((InCtldPcb *ctldPcb, InPCB *pcb, InCb *tCb, 
                                 U8 reqType,Rst *rstEvnt, InCb *dCb));
extern S16 inRstReqBChan ARGS((InBearChn *bearChn, InCb *dCb, InPCB *pcb,InCb *tCb, 
                               InCtldPcb *ctldPcb, U8 reqType, Bool flgBasic));
extern S16 inRstReqIntBChan ARGS((InCtldPcb *ctldPcb, InPCB *pcb, InCb *dCb));

extern S16 inRstMsgIndChan ARGS((Rst *rst, InCb *cb, InPCB *pcb,
                                 InCtldPcb *sigCtldPcb)); 

extern S16 inRstReqSngInt ARGS((InCtldPcb *ctldPcb, InPCB *pcb, InCb *dCb, 
                                U8 reqType,Rst *rstEvnt));
extern S16 inRstReqAllInt ARGS((InPCB *pcb, InCb *dCb, U8 reqType));
extern S16 inRstBkup ARGS((InPCB  *pcb));
extern S16 inRstMsgAllInt ARGS((InPCB *pcb, InCb *cb, Rst *rst));
extern S16 inRstAckBChan ARGS((RstAck *rstAck, InCb *cb, InPCB *pcb, 
                               InCtldPcb *ctldPcb));
extern S16 inRlsNtcForRst ARGS((InBearChn *bChan, InPCB *pcb, U8 chnlType, 
                                Bool *bDisc, Bool *bRelease));
extern S16 inRstAckInt ARGS((U8 reqType, InPCB *pcb, InCtldPcb *ctldPcb,
                             InCb *cb, U8 intType));

extern S16 inRstMultChannels ARGS((InCb *cb, InPCB *pcb,SuId suId));
#if 1 /* addition : in013.310 */
extern S16 inChkRstMsg ARGS((InPCB *pcb, InCtldPcb *ctldPcb,
                             RstInd *rstInd, ChanId *chanId));
#endif /* addition : in013.310 */

#if 1 /* TELICA-MCAO-09/26/01: Telcodia Backup-D issue */
extern S16 inChkSrvMsg ARGS((InPCB *pcb, PTR msg, Bool isBchanSrvMsg));
#endif /* TELICA */

#ifdef IN_NUMSCREEN /* TELICA-MCAO-10/31/01: screen number format */
extern S16 inScreenCgPtyNum ARGS((Setup *setup));
extern S16 inScreenCdPtyNum ARGS((CdPtyNmb cdPtyNmb, InPCB *pcb));
#endif /* IN_NUMSCREEN */

#ifdef NETSIDE 
#ifdef ISDN_PTMPT 
extern S16 inHandleMptChannelCheck ARGS ((InNtc *ntc, InCb *dCb, 
                                          InPCB *pcb, ChanId *chanId, 
                                          U8 event));
extern S16 inIntGenReleaseComplete ARGS ((InNtc *ntc, InCb  *dCb));
extern S16 inIntCheckMessageCompatible ARGS ((InNtc *ntc,InCb *cb,Event event));
extern S16 inIntProcessState ARGS ((InNtc *ntc,InCb *dCb,Event event));
extern S16 inIntPrimRelReq ARGS ((InNtc *ntc,InCb *dCb,CauseDgn *cause));
extern S16 inIntPrimSetupAckReq ARGS ((InNtc *ntc,InCb  *dCb));
extern S16 inIntPrimCallProcReq ARGS ((InNtc *ntc,InCb *dCb));
extern S16 inIntPrimInfoReq ARGS ((InNtc *ntc,InCb *dCb,Buffer *mBuf));
extern S16 inPrcTimerT312 ARGS ((InNtc *ntc));
extern S16 inStartInternalCbTmr ARGS ((InNtc *ntc,InCb  *dCb,S16 timer));
extern Void inCbInternalTmrEvnt ARGS ((PTR cntrlBlk,S16 event));
extern S16 inIntProgressInd ARGS ((InNtc *ntc,InCb  *dCb));
extern S16 inIntInfoInd ARGS ((InNtc *ntc,InCb  *dCb));
extern S16 inIntAlertingInd ARGS ((InNtc *ntc,InCb  *dCb));
extern S16 inIntConnectInd ARGS ((InNtc *ntc,InCb  *dCb));
extern S16 inIntCallProcInd ARGS ((InNtc *ntc,InCb  *dCb));
extern S16 inIntReleaseInd ARGS ((InNtc *ntc,InCb  *dCb));
extern S16 inIntReleaseCmpltInd ARGS ((InNtc *ntc,InCb  *dCb));
extern S16 inInsertNtcInPCB ARGS ((InPCB *pcb,InNtc *ntc));
extern InNtc *inGetNtcInPCB ARGS ((InPCB *pcb,CalRef callRef));
extern S16 inDeleteNtcInPCB ARGS ((InPCB *pcb,CalRef callRef));
extern S16 inPrcReleaseAllCes ARGS ((InNtc *ntc));
extern S16 inPrcCallAbort ARGS ((InNtc *ntc));
extern Bool inCreateIntPrc ARGS ((U8 msgType));
extern S16 inDeleteCbIntTimers ARGS ((InNtc *ntc,InCb  *dCb));
extern S16 inDeleteCbIntTimer ARGS ((InNtc *ntc,InCb  *dCb, U8 tmrVal));
extern S16 inIntGenRelease ARGS ((InNtc *ntc, InCb *dCb, CauseDgn *cause));

#endif /* ISDN_PTMPT */
#endif /* NETSIDE */


/* Configuration Functions */
extern S16 inGenCfgHandler ARGS ((InMngmt *cfg));

extern S16 inTSAPCfgHandler ARGS ((InMngmt *cfg));

#ifdef PLM_IN
extern S16 inInitTCb ARGS((InCb *cb,InMngmt *cfg, SpId spId));
#else
extern S16 inInitTCb ARGS((InCb *cb,InMngmt *cfg));
#endif /* PLM_IN */

extern S16 inFillMgmtPst ARGS((InMngmt *cfg));
extern S16 inDLSAPCfgHandler ARGS ((InMngmt *cfg));
extern S16 inSigPcbCfgHandler ARGS((InMngmt *cfg,InCtldPcb *ctldPcb,
                                    InPCB *pcb,Bool initCfg,SuId sapId));
extern S16 inPrcChanCfg ARGS ((InMngmt *cfg,InCtldPcb *ctldPcb,Bool initCfg));
extern S16 inInitTmr ARGS ((InMngmt *cfg,InPCB *pcb));
extern S16 inFindOldInt ARGS ((InMngmt *cfg,Bool initCfg, SuId sapId));
extern S16 inInitCb ARGS ((InCb *cb, InPCB *pcb));
extern S16 inPrcSigPcbCfg ARGS ((InMngmt *cfg,InPCB *pcb,Bool initCfg));
#if 0 /* deletion : in012.310 */
extern S16 inInitPcb ARGS ((InPCB *pcb,InMngmt *cfg));
#else /* addition : in012.310 */
extern S16 inInitPcb ARGS ((InPCB *pcb,InMngmt *cfg, Bool initCfg));
extern S16 inPartialRecfgDLSAP ARGS ((InPCB *pcb, InCtldPcb *ctldPcb, 
                                 InMngmt *cfg));
#endif /* addition : in012.310 */
extern S16 inCtldSapCfgHandler ARGS ((InMngmt *cfg));
extern S16 inCtldPcbCfgHandler ARGS ((InMngmt *cfg,InCtldPcb *ctldPcb,
                                      Bool initCfg));
extern S16 inPrcCtldPcbCfg ARGS ((InCtldPcb *ctldPcb));
extern S16 inResetStsCntr ARGS ((InPCB *pcb));

extern S16 inDLCCfgHandler ARGS ((InMngmt *cfg));
extern S16 inCesCfg ARGS((InMngmt *cfg,InPCB *pcb,InCb *cb));

extern S16 inBearCfgHandler ARGS ((InMngmt *cfg));

#if 1 /* DEV_MGCBCH */
extern S16 inBchCfgHandler ARGS ((InMngmt *cfg));
#endif  
   
#ifdef ISDN_ROUTE
extern S16 inRouteCfgHandler ARGS ((InMngmt *cfg));
#endif

/* FID 14984.10 */
extern void inModReverse     ARGS ((U8 *ptr, U8 len));
extern S16 inCallTraceCfgHandler ARGS (( InMngmt *cfg));
extern S16 inCntrlCallTrace ARGS ((InMngmt *cntrl));
extern void inMatchCallTraceId ARGS ((CdPtyNmb *cdPtyNmb, CgPtyNmb *cgPtyNmb, InNtc *ntc));
extern void inSendCallTraceInfo ARGS ((CallTrcId *pTraceId, Buffer *mBuf, U8 msgtype));


extern S16 inDigitMapCfgHandler ARGS ((InMngmt *cfg));

#ifdef IN_LMINT3
extern S16 inFillReplyPst ARGS((Header *hdr,Pst *reqPst,Pst *cfmPst));
extern S16 inSendCfgCfm ARGS((Pst *cfmPst,InMngmt *cfmMsg,S16 reason));
extern S16 inSendCntrlCfm ARGS((Pst *cfmPst,InMngmt *cfmMsg,S16 reason));
#endif

/* Control Functions */
extern S16 inCntrlGen ARGS ((InMngmt *cntrl));
extern S16 inCntrlDLSAP ARGS((InMngmt *cntrl, SuId sapId, Ces ces));
extern S16 inCntrlTSAP ARGS((InMngmt *cntrl, SuId sapId));
extern S16 inCntrlBChan ARGS((InMngmt *cntrl,SuId sapId, Ces ces, U8 chn));
extern S16 inCntrlGrpDLSAP ARGS((InMngmt *cntrl));
extern S16 inCntrlGrpTSAP ARGS((InMngmt *cntrl));
extern S16 inCntrlAllSaps ARGS((InMngmt *cntrl));
extern S16 inCntrlDigitMap ARGS((InMngmt *cntrl));
extern S16 inGeoHandleBChannel( InMngmt *cntrl );
extern void inGeoPreCondBChannel(InCb *cb, InPCB *pcb );
#ifdef NI2_BCAS 
extern S16 inCntrlAudit ARGS((InMngmt *cntrl, SuId sapId, U8 chanNum));
#endif /* NI2_BCAS */

#ifdef PLM_IN
extern S16 inClearCtldCb ARGS((InPCB *sigPcb, InCtldPcb *ctldPcb, U8 clrCalls));
extern void inDisableCtldPcb ARGS((InPCB * pcb));
#endif

extern S16 inGetPcb ARGS((SuId suId,InCtldPcb **ctldPcb,InPCB **pcb));
extern S16 inGetCb ARGS((InCtldPcb *ctldPcb,InPCB *sigPcb,S16 ces,InCb **cb)); 
extern S16 inDisableNfasSap ARGS((InPCB *sigPcb,InCtldPcb *ctldPcb,InCb *cb,
                                  U8 action));
extern S16 inEnableSap ARGS((InCtldPcb *ctldPcb,InPCB *sigPcb,S16 ces));

/* TELICA-jwh-01/24/2001:  Add Q931 action to gracefully disable an interface. */
#if 1   /* Start of code change */
extern S16 inDisableInterface ARGS((InCtldPcb *ctldPcb,InPCB *sigPcb,S16 ces,
                              U8 action));
#endif  /* End of code change */

extern S16 inDisableDLSAP ARGS((InCtldPcb *ctldPcb,InPCB *sigPcb,S16 ces,
                              U8 action));
extern S16 inDeleteDLSAP ARGS((InCtldPcb *ctldPcb,InPCB *sigPcb,S16 ces));
/* PLM_IN_DBG */
extern S16 inBndEnableSap ARGS((InPCB *pcb, U8 action));

#if 0 /* deletion : in011.310 */
extern S16 inUBndDisDLSAP ARGS((InPCB *sigPcb));
#else /* addition : in011.310 */
extern S16 inUBndDisDLSAP ARGS((InPCB *sigPcb, InCtldPcb *ctldPcb, 
                                U8 clrCalls));
extern S16 inActivateBkup ARGS((InCb *cb, InCb *bkupCb, InPCB *sigPcb, 
                                InCtldPcb *ctldPcb));
#endif /* change : in011.310 */

extern S16 inClearBChanCalls ARGS((InCtldPcb *ctldPcb,InCb *cb, 
                                   InCb *dCb, U8 action));
extern S16 inClearAllCalls ARGS((InCb *cb,U8 callType));
extern S16 inDeleteTSAP ARGS((SuId sapId));
extern S16 inUBndDisTSAP ARGS((SuId sapId));
#if 0 /* deletion : in011.310 */
extern S16 inClearCb ARGS((InCb *cb, InPCB *sigPcb,InCtldPcb *ctldPcb));
#else /* addition : in011.310 */
extern S16 inClearCb ARGS((InPCB *sigPcb,InCtldPcb *ctldPcb, U8 clrCalls));
#endif /* change : in011.310 */

extern S16 inBndEnableAllSaps ARGS((void));
#if 0 /* deletion : in011.310 */
extern S16 inUBndDisableAllSaps ARGS((void));
extern S16 inHandleGrpDLSAP ARGS((ProcId dstProcId,U8 action));
#else /* addition : in011.310 */
extern S16 inUBndDisableAllSaps ARGS((U8 clrCalls));
extern S16 inHandleGrpDLSAP ARGS((ProcId dstProcId,U8 action, U8 clrCalls));
#endif /* change : in011.310 */

extern S16 inDisableGrpTSAP ARGS((ProcId dstProcId));

extern S16 inDelAddChan ARGS((InCtldPcb *ctldPcb,U8 chn,U8 action));

/* TELICA-jwh-01/04/2001:  BCHANSYNC:  Add logic to handle B channel maintenance request. */
#if 1  /* begin of code change */

#if 1 /* IN_PBD , H.Kim */
extern void inHandleBChanReq ARGS((InCtldPcb *ctldPcb, InPCB *sigPcb,
                              InBearChn *bearChn,U8 action, U8 mntStat));
#else

extern void inHandleBChanReq ARGS((InCtldPcb *ctldPcb, InPCB *sigPcb,
                              InBearChn *bearChn,U8 action));
#endif /* IN_PBD */

#endif  /* end of code change */

#if 1   /* TELICA-MCAO-PRJ-NI2-SRV_MSG-- begin of code change */

extern void inHandleBChanReqNoSrv ARGS((InCb *cb, InCb *tCb, InCtldPcb *ctldPcb, InPCB *sigPcb,
                              InBearChn *bearChn,U8 action));
#if 1 /* IN_PBD, H.Kim */
extern void inHandleBChanReqSrv ARGS((InCb *cb, InCb *tCb, InCtldPcb *ctldPcb, InPCB *sigPcb,
                              InBearChn *bearChn,U8 action, U8 mntStat));
#else
extern void inHandleBChanReqSrv ARGS((InCb *cb, InCb *tCb, InCtldPcb *ctldPcb, InPCB *sigPcb,
                              InBearChn *bearChn,U8 action));
#endif
extern S16 inNI2AuditbChan(InCb  *tCb, InPCB *pcb, InNtc *ntc);

#endif  /* TELICA-MCAO-PRJ-NI2-SRV_MSG-- end of code change */

#if 1 /* ISDN Persistent B/D Channel, H.Kim */
extern S16 inDisEnbChan ARGS((InCtldPcb *ctldPcb, InPCB *sigPcb, 
                              U8 chn,U8 action, U32 maintStat));
#else /* original code */
extern S16 inDisEnbChan ARGS((InCtldPcb *ctldPcb, InPCB *sigPcb, 
                              U8 chn,U8 action));
#endif /* IN_PBD */

extern S16 inRestartInt ARGS((InPCB *pcb,InCtldPcb *ctldPcb,Ces ces));
extern S16 inRestartChan ARGS((InPCB *pcb,InCtldPcb *ctldPcb,InCb *cb,
                               U8 chn));
extern S16 inRestartAllInt ARGS((SuId sapId,Ces ces));

extern S16 inShutDown ARGS((void));

extern S16 inStsReqHandler ARGS((InMngmt *sts,Action action));
extern S16 inSetStsCntr ARGS ((InMngmt *sts,InPCB *pcb));

/* TELICA-jwh-08/14/2001:  Fix for bug#5497:  Release Avalanche  */
#if 1   /* Start of code change */
extern void inUnpkKickIfTeardown  ARGS((Pst *pst, Buffer *mBuf));
extern void inUnpkKickClearAllCalls  ARGS((Pst *pst, Buffer *mBuf));
#endif  /* End of code change */
#if 1 /* TELICA - Bugzilla-4321 - Bukucu - 10.25.01 */
extern Void inUpdSts ARGS((Cntr *stsCntr, InPCB *pcb));
#endif /* Bugzilla-4321 */


#ifdef PLM_IN
extern S16 inCntrlCTRLDSAP ARGS((InMngmt *cntrl, SuId sapId, Ces ces));
extern S16 inEnableLink ARGS((InCtldPcb *ctldPcb,InPCB *sigPcb, S16 ces,
	                      InMngmt *cntrl));
extern S16 inDeleteLink ARGS((InCtldPcb *ctldPcb,InPCB *sigPcb,S16 ces));
extern S16 inDisableLink ARGS((InCtldPcb *ctldPcb,InPCB *sigPcb,S16 ces,
	                              U8 action));	
extern void inAssociateCtldPcb ARGS((SuId ctldInt, SuId sigInt, U32 rSigInt, SuId bupInt, U32
rBupInt));
extern void inDeAssociateCtldPcb ARGS((InCtldPcb *ctldPcb));
#endif /* PLM_IN */

#ifdef TELICA /* Bug#16149, rkhan */
typedef struct ISDNIFSTS_QUEUE_CB_TAG
{
    void *Next;
    SuId suId;
} ISDNIFSTS_QUEUE_t;

PUBLIC Void initInStsQ(Void);
PUBLIC Void addIsdnIfCbQ(InPCB *pcb);
PUBLIC Void remIsdnIfCbQ(InPCB *pcb);
S16 inUpdPeerSts(Void);
#endif

/* BUG:37899-mqin */
extern S16 inChkFacilMsg(InCb *cb, InPCB *pcb, InNtc *ntc);
extern S16 inChkFacilStr(TknStrE *facilityStr);
extern S16 inNetTHOLDSETUPS01(InNtc *ntc);

#ifdef __cplusplus
         }
#endif /* _cplusplus */
#endif /* INX */

#ifdef ETSI
#if 0 /* Old code */
typedef struct mapDig
{
    U8 num_init[ADRLEN];
    U8 max;
} MapDig;

extern U8 inDigMapSrch(U8 *adr, U8 len);
extern S16 inCmpLdNmb(U8 *adr1, U8 len1, U8 *adr2, U8 len2);
#else /* Start of code change */
extern S16 inAddCandidateToList(InDigitMap *pDigitMap, LngAddrs *digitMapStr, U16 candidateId, U8 typeNmb);
extern S16 inFindCandidateInList(InDigitMap *pDigitMap, U16 candidateId, DigitMapCandidate **candidate);
extern S16 inDelCandidateFromList(InDigitMap *pDigitMap, DigitMapCandidate *pCandidate);

extern S16 inInitDigitMap(InDigitMap *pDigitMap, LngAddrs *pDigitMapName);
extern S16 inCreateDigitMapInfo(InNtc *ntc, InDigitMap *pDigitMap, U8 typeNmb);
extern S16 inResetDigitMapInfo(InNtc *ntc);
extern S16 inCandStateChange(InNtc *ntc, DigitMapCandidate *pCandidate, 
                             DigitMapCandidateInfo *pCandidateInfo, U8 matchFlag);
extern S16 inCheckDigitMap(InNtc *ntc, InDigitMap *pDigitMap, DigitMapCandidate *pCandidate,
                           DigitMapCandidateInfo *pCandidateInfo, U8 digit);
extern S16 inSearchDigitMap(InNtc *ntc, InDigitMap *pDigitMap, TknStrM *nmbDigits, U8 typeNmb);
extern U8  inGetMaxDigits(InNtc *ntc, InDigitMap *pDigitMap, U8 typeNmb);
#endif /* End of code change */

PUBLIC Void inChkDepBtwTkns(InNtc *ntc, InPCB *pcb);
PUBLIC S16 inPreChkBldMsg(InCb *cb, AllPdus *msg);

/* Value used by pcb->cfg.numPL */
#define NONENP 0
#define NPPL 1
/* FID 17412.0 + */
PUBLIC S16 inFillDateTimeIE(DatTim  *datTim, U8 typeDateTime);
/* FID 17412.0 - */
#endif

  
/********************************************************************30**
  
         End of file: in.x 3.8  -  09/29/99 14:29:12
   
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

1.2.0.0         lc      1. change structures 

1.3.0.0         lc      1. change structures

1.4.0.0         lc      1. add elements to structures for
                           segmentation
                        2. text changes
 
1.5.0.0         jkm     1. change MF_MAX_MSG_ERRS to MF_MAX_REPELMT
                        2. change inMfCodeSet, inMfSetExt and inMfInitPdu
                           prototypes
                        3. add inPrntErr prototype
                        4. change elmtId in MfMsgCtl from U8 to U16

1.6.0.0         lc      1. added struct cbTimer to support multiple
                           simultaneous timers for CBs
                        2. added struct calRefTbl to support dynamic
                           call reference allocation
                        3. Control Block structure contains multiple
                           timers 
                        4. new elements waitSrvAck, pendStat and suId were
                           added to InBearChn type.
                        5. remove MSCDOS, MSCUNIX and not MSCDOS or MSCUNIX
                           prototypes and replace with single set of
                           prototypes using argument manipulation macro
                           for ansi or non ansi support to improve
                           portability

1.7             lc      1. added line number in MfMsgCtl structure for errors
                lc      2. added callRedir, redirReason, redirNmbPlan and
                           redirTypeNmb to inNtc structure  
                jrl     3. trillium development system checkpoint (dvs)
                           at version: 1.6.0.0

1.8             lc      1. extern allSduDefs has been moved to int.x

1.9             bn      1. changed calDura in inNtc from U32 to Ticks type

2.1             bn      1. added prototypes for inUsrMSGINV, inUsrE19SND,
                           inNetE13SND.

2.2             bn      1. added prototypes for inGenRelUpCLw, inUsrE05S12,
                           inUsrE15S19, inUsrE14S11.

2.3             bn      1. added prototypes for inGenStaCfm, inUsrE00S12,
                           inGenRelLw, inUsrE03S10, inUsrE14S09.
                        2. added Bool validate to inBearChn structure.

*********************************************************************71*/

/********************************************************************80**

  version    pat  init                   description
----------- ----- ----  ------------------------------------------------
2.4          ---  jrl   1. text changes
             ---  bn    2. replaced struct ... by typefs.

2.5          ---  bn    1. add prototype for inNetE29S10.
             ---  bn    2. remove prototype for inUsrE05S01.
             ---  bn    3. added prototypes for new timer routine functions.
             ---  bn    4. added prototypes for functions for nfas support.

2.6          ---  rg    1. removed second typedef for InPCB.
                        2. added prototypes for inUsrE19S10, inUsrE29S03,
                           inUsrE29S10, inNetE29S07 and inNetE29S15.

2.7          ---  bn    1. added prototype for inChkMsgComp.

2.8          ---  bn    1. change timer declarations to use common definitions.
             ---  bn    2. add support for tr 303 tmc.
             ---  bn    3. replaced tknStr by tknStrS in control blocks 
                           to reduce memory requrements.

2.9          ---  bn    1. add trace enable flag to InCB.
             ---  bn    2. add restart confirmation sent flag to InCB.

2.10         ---  bn    1. add auxilState to InNtc.
             ---  bn    2. removed hldFlg from InNtc.
             ---  bn    3. added prototype for inNetE29S00 and inNetE31S00.
             ---  bn    3. added prototype for inUsrE29S00 and inUsrE31S00.
             ---  bn    4. added prototype for inProcHldRtrReq,inProcHldRtrRsp
                           and inProcHldRtrMsg for Q932.
             ---  bn    5. surrounded by #ifdef SEGMENT parameters and function
                           prototypes used by segmentation functionality.

2.11         ---  bn    1. added prototype for inGenCnStaInd.
             ---  bn    2. added prototype for inUsrE16S11.
             ---  bn    3. added dChan to InBearChn structure.

2.12         ---  bn    1. added suIdPres to InBearChn structure.

2.13         ---  bn    1. changed #ifdef HOLD to #if ISDN_HOLD.
                        2. changed #ifdef SEGMENT to #if ISDN_SEGMENT.

3.1          ---  bn    1. Changed interfaces to include Pst structure
                           instead of BndCfg.
3.2          ---  krp   1. Added "inUnBndFlg" to the InCB structure.
             ---  krp   2. Added prototypes for functions inGetCesVal,
                           inSetCauseDgn, inUnBndCb, inFreePcb and 
                           inGenRelComplete.

3.3          ---  bn    1. miscellaneous changes

*********************************************************************81*/

/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
3.4          ---      sb   1. Miscellaneous changes
3.5          ---      sb   1. Added function prototypes for QSIG GFT.  
             ---      sb   1. Added hash list mptCallList in InPCB.
             ---      sb   2. Added hash list entry mptCallEntry in InNtc.
             ---      sb   3. Added structure cesTimerCb.
             ---      sb   4. Modified prototype for inChkMsgComp.

3.6          ---      sam  1. Added inChkBC303CallRef to check for Call Ref.
                              for BellCore.

3.7          ---      pk   1. Added inChkQsigNotInd prototype.
             ---      asa  2. Changed inCallIdSearch prototype.
                      pk   3. Changed ifdef ISDN_SEGMENT to if ISDN_SEGMENT
                              for declaration of function inXmitSegMsg.
             ---      pk   4. Added inAssnCause prototype.
             ---      sam  5. Added prototypes for some function
                              which were missing to remove warnings.
             ---      sam  6. Added prototypes for InIssueNtFacInd and
                              InGenNtFacReq.

3.8          ---      rsk  1. Added function prototypes for inFillChanId,
                              inGenSrvAck

3.9        in001.310   pk  1. Removed flow control queues, ttxQueue,
                              dTxQueue, and tFlc, dFlc flags.
                           2. Fixed Compile time warnings.
           in002.310   pk  3. Removed tFlc flag.
           in011.310   pk  4. Modified prototypes for functions 
                              inUBndDisDLSAP, inUBndDisableAllSaps
                              inClearCb, inHandleGrpDLSAP.
                           5. Added prototype for inActivateBkup
                       cy  6. Put the prototype for inXferActvCalls outside
                              the compile time flag ISDN_SRV.
           in012.310   cy  7. Change the prototype for inInitPcb.
                              Added the prototype for inPartialRecfgDLSAP.
           in013.310   cy  8. Added the prototype for inChkRstMsg.
           in014.310   mm  9. Added fuction prototype for inNetTREST for
                              NI2 variant.
           in015.310   cy 10. Changed prototype for inGenRstAck.
	   in025.310   bb 12. Changed definition of inNetTREST() and added
                              inMkBChnAvailable(). 
	   in026.310   bb 13. Added three new functions, inChkRstState(),
	                      inGenRstBChan() and inGenRstInt().
           in030.310   yz 14. Added two new functions, inSndStaEnqDLFail(),
                              inPrcDLFailCal().
           in034.310   yz 15. Added new function inRstAck4Col().	
           in036.310   bb 16. Added changes for CORE2 and TCR18 support
                              (TELICA:won't work, removed).
                          17. Added extern declaration for two new functions,
                              InMiShtCntrlReq and InMiShtCntrlCfm for TCR18
                              (TELICA:won't work,removed).
*********************************************************************91*/
