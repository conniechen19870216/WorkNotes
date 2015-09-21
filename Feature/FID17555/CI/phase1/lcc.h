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
  
        Name:     layer management - call control
    
        Type:     C include file
  
        Desc:     Defines required by the call control layer management .
 
        File:     lcc.h

        Sid:      lcc.h 1.5  -  12/21/99 11:54:44
  
        Prg:      rs
  
*********************************************************************21*/
 
#ifndef __LCCH__
#define __LCCH__


/*
 *      This software may be combined with the following TRILLIUM
 *      software:
 *
 *      part no.                      description
 *      --------    ----------------------------------------------
 *    
 */

  
/* defines */

/* Mngmt.hdr.elmId.elmnt */

/* #define STGEN             1              (see gen.h) general */
#define STCCPSSAP            2           /* Protocol Spec. SAP Configuration */
#define STCCRMSAP            3           /* Resource Manager SAP Configuration*/
#define STCCRTSAP            4           /* Router SAP configuration */
#define STCCSFSAP            5           /* SF Manager SAP configuration */
#define STCCINTFC            6           /* Interface SAP configuration */
#define STCCPROF             7           /* N/B Circuit Profile configuration */
#define STCCSDP              8           /* SDPProfile configuration */
#define STCCBILLINGPRFL      9           /* TG Billing Profile configuration */
#define STCCSIPPRFL          10          /* SIP profile */
#define STCCPROTRELMAPTBL    11          /* Protocol relase 2 FAILCND Map Tbl*/
#define STCCSIPTPRFL         12          /* SIPT profile */
#define STCCRAPROTRELTBL     13          /* auto reroute prot rel tbl */

#define STPSSAP              36          /* SAP statitstics */
#define STCCOBS              37          /* Observation Trigger Table 
                                          * configuration 
                                          */
#define STCCCDR              38          /* Call Detail Record */
#define STCCVINTFC           39          /* Virtual Interface configuration */
#define STCCTSTCALL          40          /* Continuity check test call */
#define STCCGRPSSAP          41          /* Group PSIF SAP */
#define STCCGRRMSAP          42          /* Group RM SAP */
#define STCCGRRTSAP          43          /* Group RT SAP */
#define STCCGRSFSAP          44          /* Group SF SAP */
#define STCCGRALL            45          /* Group All SAP */
#define STCCTGNOLI           46          /* TGN-OLI Table*/
#define STCCSWTCFG           47          /* SWITCH Configration */
#define STCCBILLINGSYS       48          /* Billing System */
#define STCCIVRSTATE         49          /* IVR State */
#define STCCSTST             50          /* STAT STATE*/
#define STCCIVRANNCID        51          /* IVR Announcement Id */
#define STCCPRTCLRELTBL      52          /* Protocol Release table */
#define STCCFAILCNDTBL       53          /* Failure condition table */
#define STCCPARAMSUPPTBL     54          /* Param Suppression table */
#define STCCPARAMDFLTTBL     55          /* Param Default     table */
#define STCCFRAUDPRFLTBL     56          /* Fraud trap profile table */
#define STCCEXTANNCID        57          /* External (Send_To_Resource) announcement */
#define STCCFRAUDCALLINFO    58          /* Retrieve Fraud Call Info */
/* BUG:25011 RDT 30Sep03 */
#define STCCDGSSRVLIST       59          /* Billing/Stats DGS server list */
#define STCCDGSENVCFG        60          /* GDI env configuration */
/*Bug: 37702 -- Begin*/
#define STMGNAMECFG          61          /* MG Name config */
/*Bug: 37702 -- End*/
/* Bug:45879 +*/
#define STCCMGCMGAUDITCFG    62          /* MGC-MG audit config */
/* Bug:45879 -*/
/* FID 14841.2 */
#define STCCREALMMAPADDR     63          /* Realm Map Address Config */
/* FID 14841.2 */
/* + FID15083.0 */
#define STCCCCCIPPRFL        64          /* LI CCC IP profile */
/* - FID15083.0 */
/* FID 15261.0 + */
#define STCCMGMAPADDR        65          /* MG Map Address Config */
#define STCCMEGAABINDCNTRL   66          /* GCC<->MEGAA bind*/
/* FID 15261.0 - */

/* FID15169.0 */
#define STCCMGCPCCFG         67          /* MCG PC Config */

#define STCCMCGCODE          68          /* MCG code config */
/* FID 14457.0 */
#define STCCHPCSYSCFG        69          /* GETS HPC Sys config */

/* FID 15737 */
#define STCCMDJRSDCTN        70          /* MDJRSDCTN config */
#define STCCMD               71          /* Prfl-MD config */
#define STCCLISYSCFG         72          /* LI-SYS config to GCC*/

/* FID 14716.0 */
#define STCCGRCNTRLCFG       73          /* GR control mode */

/* FID 14457.0 - Bug 88508 */
#define STCCGETSDNCFG        74          /* GETS DN config */

/*  FID16584.0 */
#define STCCENUMDOMAIN       75          /* ENUM DOMAIN config */

/* FID: 14708.0 */
#define STCCCCBSSYSCFG       76         /* CCBS Sys Config */

/* FID17174.0 */
#define STCCMRFLISTCFG       77         /* MRF-LIST */
#define STCCMRFANNCCFG       78         /* MRF-ANNC */
#define STCCSIPTGPST         79         /* SIP TGP STATE */

/* FID17121.0 */
#define STGDIFLUSH           80         /* GDI Flush event upon dlt-mgc-mgassoc*/

/* Observation Table Configuration Type */
#define LCC_OBS_ROW           0 /* Configure a row in the 
                                 * observation trigger table 
                                 */
#define LCC_OBS_COL           1 /* Configure a column in the 
                                 * observation trigger table 
                                 */
#define LCC_OBS_ELMNT         2 /* Configure a column in the 
                                 * observation trigger table 
                                 */

/* Maximum size for the Observation Trigger Table */
#define LCC_MAX_OBS_TBLSZ     50 /* Observation Table Size */

/* Observation Trigger Levels Bitmaps */
#define LCC_TRIG_STATMC    0x01 /* Trigger state transition */
#define LCC_TRIG_CCT_LOG   0x02 /* Trigger protocol events */
#define LCC_TRIG_MSG_DUMP  0x04 /* Trigger protocol events 
                                 * with event dumps */
#define LCC_TRIG_RMT_LOG   0x08 /* Trigger RMT events */
#define LCC_TRIG_RTT_LOG   0x10 /* Trigger RTT events */
#define LCC_TRIG_SFT_LOG   0x20 /* Trigger SFT events */
#define LCC_TRIG_TMR_LOG   0x40 /* Trigger Timer events */

/* Release Origin */
#define LCC_RELORGN_IN          1  /* incoming -> outgoing */
#define LCC_RELORGN_OUT         2  /* outgoing -> incoming */
#define LCC_RELORGN_INTERNAL    3  /* gcc -> incoming . gcc -> outgoing */
#define LCC_RELORGN_INTERNAL_MGW_FACILITY_PROBLEM_TDM 4 /* FID 14382.8 MG facility failure */
#define LCC_RELORGN_INTERNAL_MGW_FACILITY_PROBLEM_IP 5 /* FID 14382.8 MG facility failure */
#define LCC_RELORGN_INVALID    0xff  /* gcc -> incoming . gcc -> outgoing */

/* Cause value coding standards */
#define CCSTD_CCITT     0x00  /* CCITT standards */
#define CCSTD_INT       0x01  /* Other International Standards */
#define CCSTD_NAT       0x02  /* National Standard */
#define CCSTD_NET       0x03  /* Network Standard */

/* Cause value location */
#define CCLOC_USER       0x00  /* user */
#define CCLOC_PRIVNETLU  0x01  /* private network serving local user */
#define CCLOC_PUBNETLU   0x02  /* public network serving local user */
#define CCLOC_TRANNET    0x03  /* transit network */
#define CCLOC_PUBNETRU   0x04  /* public network serving the remote user */
#define CCLOC_PRIVNETRU  0x05  /* private network serving the remote user */
#define CCLOC_LOCALIF    0x06  /* local interface controlled by this signalling link */ /*bug52901*/
#define CCLOC_INTNET     0x07  /* international network */
#define CCLOC_NETINTER   0x0a  /* network beyond internetworking point */
#define CCLOC_NOINFOAV   0x0f  /* no info concerning origin location */

/* Cause value recommendation */
#define CCREC_Q931       0x00  /* CCITT Recommendation Q.931 */
#define CCREC_X21        0x03  /* CCITT Recommendation X.21 */
#define CCREC_X25        0x04  /* CCITT Recommendation X.25 */


/* SAP states */

#define CC_UNBND             0           /* unbound */
#define CC_BND               1           /* bound */
#define CC_BND_CONT          2           /* Waiting for Bind Confirm */
#define CC_DISGRC            3           /* undergoing graceful disable */
#define CC_UNBND_VER         4           /* unbinded due to version issue FID15261.0 */
/* maximum number of RTSAPs per interface */
#define MAXRTSAP 2


/* Alarms */

/* Reason */
#define LCC_REASON_INVALID_RMSAP 301      /* Invalid RM SAP */
#define LCC_REASON_INVALID_RTSAP 302      /* Invalid RT SAP */
#define LCC_REASON_INVALID_SFSAP 303      /* Invalid SF SAP */
#define LCC_REASON_INVALID_HDRPARM 304    /* Invalid Header paramter */
#define LCC_REASON_INVALID_INTFC     305  /* Invalid Interface problem */
#define LCC_REASON_INVALID_ACTION    306  /* Invalid action requested */
#define LCC_REASON_INVALID_OBSTYPE   307  /* Invalid Observation Type */
#define LCC_REASON_INTFC_NOTDISABLED 308  /* Interface not disabled */
#define LCC_REASON_INVALID_PSSAP     309  /* Invalid PS SAP */
#define LCC_REASON_PSSAP_NOTDISABLED 310  /* PS SAP not disabled before ADEL */
#define LCC_REASON_RMSAP_NOTDISABLED 311  /* RM SAP not disabled before ADEL */
#define LCC_REASON_RTSAP_NOTDISABLED 312  /* RT SAP not disabled before ADEL */
#define LCC_REASON_SFSAP_NOTDISABLED 313  /* SF SAP not disabled before ADEL */
#define LCC_REASON_PSSAP_NOTCFGD     314  /* PS SAP not configured */
#define LCC_REASON_RMSAP_NOTCFGD     315  /* RM SAP not configured */
#define LCC_REASON_RTSAP_NOTCFGD     316  /* RT SAP not configured */
#define LCC_REASON_SFSAP_NOTCFGD     317  /* SF SAP not configured */
#define LCC_CAUSE_PSSAPNOTBND        318  /* PS SAP not bound */
#define LCC_CAUSE_RMSAPNOTBND        319  /* RM SAP not bound */
#define LCC_CAUSE_RTSAPNOTBND        320  /* RT SAP not bound */
#define LCC_CAUSE_SFSAPNOTBND        321  /* SF SAP not bound */

/* Event */
#define LCC_EVENT_HASHING_FAILED 301      /* Hashing falied */
#define LCC_EVENT_INV_DESTSAP    302      /* Invalid Destination SAP id */
#define LCC_EVENT_INV_RMSAP      303      /* Invalid RM SAP id */
#define LCC_EVENT_INV_RTSAP      304      /* Invalid RT SAP id */
#define LCC_EVENT_INV_SFSAP      305      /* Invalid SF SAP id */
#define LCC_EVENT_MAPPING_FAILED 306      /* Mapping falied */
#define LCC_EVENT_MEMALOC_FAILED 307      /* memory allocation falied */
#define LCC_EVENT_CONTCHK_PASSED 308      /* Test call passed */
#define LCC_EVENT_CONTCHK_FAILED 309      /* Test call failed */
#define LCC_EVENT_FLOWCONTRL_ERR 310      /* Problem with msg flow control */

#ifdef CC_GEO_R
#define LCC_EVENT_GEO_AUDIT_MG_CONNECTED  311 /* GCC CNTD to MG after Geo FO */
#define LCC_EVENT_GEO_AUDIT_MG_COMPLETE   312 /* GCC Geo Audit complete */
#endif
#define LCC_EVENT_ORI_MISS                313 /* FID 15649.0 ORI missing */
#define LCC_EVENT_APP_DATA_INACT_DETC     314 /* FID 14841.0 */

/* Cause */
#define LCC_CAUSE_INV_INTERFACE    301    /* Invalid interface */
#define LCC_CAUSE_MALLOC_FAIL      302    /* Buffer allocation failed */
#define LCC_CAUSE_SUINSTTBL_INS    303    /* SuInstTable hash insert failed */
#define LCC_CAUSE_SPINSTTBL_INS    304    /* SpInstTable hash insert failed */
#define LCC_CAUSE_SUINSTTBL_FIND   305    /* SuInstTable hash find failed */
#define LCC_CAUSE_SPINSTTBL_FIND   306    /* SpInstTable hash find failed */
#define LCC_CAUSE_INTFCSAPTBL_FIND 307    /* SpInstTable hash find failed */
#define LCC_CAUSE_MAPFAIL_NBBB     308    /* Mapping Failure NB-BB */
#define LCC_CAUSE_MAPFAIL_BBNB     309    /* Mapping Failure BB-NB */
#define LCC_CAUSE_MAPFAIL_NBNB     310    /* Mapping Failure NB-NB */
#define LCC_CAUSE_MAPFAIL_BBBB     311    /* Mapping Failure BB-BB */
#define LCC_CAUSE_RELEASETMR_EXP   312    /* Release Timer Expired */
#define LCC_CAUSE_SETUPTMR_EXP     313    /* Setup Timer Expired */
#define LCC_CAUSE_MAPFAIL_ININ     314    /* Mapping Failure IN-IN */
#define LCC_CAUSE_MAPFAIL_SIIN     315    /* Mapping Failure SI-IN */
#define LCC_CAUSE_MAPFAIL_INSI     316    /* Mapping Failure IN-SI */
#define LCC_CAUSE_PSSAPBNDTMREXP   317    /* PSIF SAP BIND CFM timer expired */
#define LCC_CAUSE_RMSAPBNDTMREXP   318    /* RM SAP BIND CFM timer expired */
#define LCC_CAUSE_RTSAPBNDTMREXP   319    /* RT SAP BIND CFM timer expired */
#define LCC_CAUSE_SFSAPBNDTMREXP   320    /* SF SAP BIND CFM timer expired */
#define LCC_CAUSE_MAPFAIL_AMAM     321    /* Mapping Failure AM-AM */
#define LCC_CAUSE_MAPFAIL_AMSI     322    /* Mapping Failure AM-SI */
#define LCC_CAUSE_MAPFAIL_SIAM     323    /* Mapping Failure SI-AM */
#define LCC_CAUSE_DEALOC_IND       324    /* Deallocate Inidication received
                                           * from Resource Manager 
                                           */
#define LCC_CAUSE_RMTALOC_FAILED   325    /* Resource allocation failed */
#define LCC_CAUSE_SFTCON_FAILED    326    /* switching failed */
#define LCC_CAUSE_PEER_FAILED      327    /* No continuity established */
#if 1 /* Bugzilla 6060:taraveti */
#define LCC_REASON_NO_STNDG_CALL   328    /* No standing call (for call trace feature) */
#endif /* Bugzilla 6060:taraveti */
/* TELICA-BUG:9128-mqin */
#define LCC_REASON_LAST_ENTRY      329    /* Last entry retrieved in the
                                             criteria table */
#define LCC_CAUSE_RECONNECTTMR_EXP 330    /* Reconnect Timer Expired */
#define LCC_CAUSE_CALEALEGTMR_EXP  331    /* CALEA LEG Timer Expiry */
#define LCC_CAUSE_INVALID_FRAUD_INFO 332  /* Invalid fraud Call Info */ 
#define LCC_CAUSE_PREANMTMR_EXP    333    /* PreAnswer Timer Expired */
/* BUG:9128 */
#define LCC_CAUSE_ORI_MISS         334    /* FID 15649.0 ORI missing */
#define LCC_CAUSE_APP_DATA_INACT_DETC     335    /* FID 14841.0 */

#define MAX_CODEC_NUM              8              /* MAX Supported CODEC type in a SDP profile */
#define MAX_PTYPE_NUM              MAX_CODEC_NUM  /* MAX Supported payload type in a SDP profile */

#define MAX_VBD_CODEC_NUM          2              /* MAX Supported VBD CODEC type in a SDP profile */
#define MAX_VBD_PTYPE_NUM          MAX_VBD_CODEC_NUM /* MAX Supported VBD payload type in a SDP profile */

/* stack manager events */
#define EVTLCCCFGREQ     0x3c             /* Configuration request */
#define EVTLCCUNCFGREQ   0x3d             /* unConfiguration request */
#define EVTLCCSTAREQ     0x40             /* Status request */
#define EVTLCCSTACFM     0x41             /* Status confirm */
#define EVTLCCSTAIND     0x42             /* Status indication */
#define EVTLCCSTSREQ     0x44             /* Statistics request */
#define EVTLCCSTSCFM     0x45             /* Statistics confirm */
#define EVTLCCTRCIND     0x48             /* Trace indication */
#define EVTLCCCNTRLREQ   0x4c             /* Control request */
#define EVTLCCCNTRLCFM   0x4d             /* Control confirm */
#define EVTLCCUNCFGCFM   0x4e             /* unConfig confirm */
#define EVTLCCCFGCFM     0x50             /* Config confirm */
#define EVTLCCACNTIND    0x51             /* Accounting Indication */
#ifdef TELICA
#define EVTLCCSIGDBG     0x52             /* Debug Command */
#endif /* TELICA */
#if 1 /* Bugzilla 6060:taraveti */
#define EVTLCCTRCCALLREQ 0x53             /* Debug Command */
#define EVTLCCTRCCALLCFM 0x54             /* Debug Command */
#define SS7CALLTRC 1
#define ISDNCALLTRC 2
/* TELICA-BUG:10477-mqin */
#define CASCALLTRC 3
/* BUG:10477 */
#define IVTCALLTRC 4                      /* FID 14221.0    */
#define EXTBICCCALLTRC 5                  /* FID 15319.0 */
#endif /* Bugzilla 6060:taraveti */
/* TELICA-BUG:9128-mqin */
#define EVTLCCENTCRITREQ 0x55             /* Enter call trace criteria */
#define EVTLCCENTCRITCFM 0x56             /* Enter call trace criteria cfm */
#define EVTLCCDLTCRITREQ 0x57             /* Delete call trace criteria */
#define EVTLCCDLTCRITCFM 0x58             /* Delete call trace criteria cfm */
#define EVTLCCRTRVCRIT   0x59             /* Retrieve call trace criteria */
#define EVTLCCTRCMSG     0x5a             /* Call trace message */
#define MAX_TRC_CRIT  128                 /* Maximum entries in the criteria
                                             table */
#define CCCRITNOMATCH  0                  /* trace index if no match is found */
/* BUG:9128 */
/* TELICA-BUG:12976-mqin : call capture - verbose */
#define EVTLCCENTCAPTREQ  0x5b            /* enter call capt criteria */
#define EVTLCCENTCAPTCFM  0x5c            /* enter call capt criteria cfm */
#define EVTLCCDLTCAPTREQ  0x5d            /* dlt call capt criteria */
#define EVTLCCDLTCAPTCFM  0x5e            /* dlt call capt criteria cfm */
#define EVTLCCRTRVCAPTREQ 0x5f            /* rtrv call capt criteria */
#define EVTLCCRTRVCAPTCFM 0x60            /* rtrv call capt criteria cfm */
#define EVTLCCCAPTMSG     0x61            /* call capt message */
#define EVTLCCPROTREL     0x62            /* call capt message */
#define MAX_CAPTV_CRIT    20              /* max entries of call capt */
/* BUG:12976 */

#if 1 /* Bugzilla 12526:taraveti */
#define EVTPCRSTART      0x62             /* to trigger the pcr timer */
                                          /*  in the pcr thread */
#endif
#define EVTLCCPCRIND     0x62             /* indicate PCR to gdi thread */

#define EVTCCCONDBGREQ      0x63             /* to trigger the cccon dump  */

/* other layer management */
#define LCC_STKSTRT          21            /* Stack Start Command */
#define LCC_CLEARCONN        22            /* Forced Connection clear command */
#define LCC_CDR              23            /* Call Detail Record Generation */
#define LCC_OBS              24            /* Signalling Conversion Analysis */
#define LCC_RFC3551          0x00
#define LCC_ITU_I_366_2      0x01

#ifdef TESTING
#define LCC_DISPCONS         25            /* Display Ids and state of all the 
                                              connections in the system */
#endif

#define LCC_GEO_SLCCS_AUDIT_START  26      /* CLAM indication to GCC to start the  GEO Audit */ 
#define LCC_GEO_SLCCS_AUDIT_ABORT  27      /* SLCCS:Abort Audit - CLCCS  indication to
					    * GCC to abort the geo audit */
#define LCC_GEO_SLCCS_AUDIT_DONE   28      /* SLCCS:Audit Done - CLCCS indication to
					    * GCC that geo Audit is done */
#define LCC_GEO_GCC_AUDIT_MG_CONNECT   29  /* GCC:MG IS - GCC indication to
					    * CLCCS that the MG is CONNected (IS) */
#define LCC_GEO_GCC_AUDIT_DONE   30        /* GCC:Audit COMPleted - GCC indication to
					    * CLCCS that geo Audit has Completed (done) */
#define LCC_GEO_GCC_AUDIT_STA_IND   31     /* GCC->CLCCS Notification via MiLccStaInd to
					    * post EVNT to CLCCS */
#define LCC_GEO_SLCCS_AUDIT_NOAUDIT   32   /* SLCCS No Audit CLCCS indication to GCC that 
                                            * NoAudit will be perform */
#define LCC_CONG_OVL_STA_IND       33      /* FID 7804. Congestion Overload Alarm */
#define LCC_GEO_SLCCS_AUDIT_PREP   34      /* FID 15261. CLAM indication to GCC to prepare the GEO Audit */
#define LCC_GEO_GCC_AUDIT_READY    35      /* GCC:Audit Ready - GCC indication to
					    * CLCCS that it's ready for Audit */
#define LCC_GEO_SLCCS_AUDIT_DONE_FAILOVER   36    /* SLCCS:Audit Done - CLCCS indication to
                                                   * GCC that geo Audit is done under failover case */
#define LCC_ORI_MISSING                     37    /* FID 15649.0 ORI missing */
#define LCC_APP_DATA_INACT_DETC             38    /* FID 14841.0 */
#define LCC_RELAY_CONG_STA_IND              39    /* FID 14457.2 */
#define LCC_FRAUD_ALARM_IND                 40    /* FID 16259.0 */

/* + FID 16259.0 */
#define   FRAUD_ALL         0x0
#define   FRAUD_EXPIRE      0x01
#define   FRAUD_NON_EXPIRE  0x02
/* - FID 16259.0 */

/* Event Type to be filled in trc.t.trc.evnt */
#define LCC_OBS_TRC          1            /* Signalling conversion analysis 
                                           * trace */

/* Obervation type in trc.t.trc.u.ccObsTrc.obsType */
#define LCC_CCT_EVNT    1 /* Event (received from/sent to)
                           * the protocol sap 
                           */
#define LCC_RMT_EVNT    2 /* Event (received from/sent to)
                           * the RMT interface 
                           */
#define LCC_RTT_EVNT    3 /* Event (received from/sent to)
                           * the RTT interface 
                           */ 
#define LCC_SFT_EVNT    4 /* Event (received from/sent to)
                           * the SFT interface 
                           */
#define LCC_TMR_EVNT    5 /* Event (received from/sent to)
                           * the Timer 
                           */
#define LCC_STMC_EVNT   6 /* State Machine Transitions */

/* Call state before being cleared */
#define LCC_CDR_UNANSWERED      0 /* Call Cleared before being answered */
#define LCC_CDR_ANSWERED        1 /* Call Cleared after being answered */

/* State of the call at the time of generating Call Detail Record */
#define LCC_CDR_CALLCLEARED     0  /* Call is being cleared */
#define LCC_CDR_CALLINPROGRESS  1  /* Call is still in progresss */

/* #define values forcedRelType */ 
#define CALL_REL_OPERATOR_FORCED   1 /* Call forcefully released by operator */
#define CALL_REL_FRAUD_SCR_TMR_EXP 2 /* Fraud Call released due to screen timer expiry */ 
#define CALL_REL_FRAUD_ANS_TMR_EXP 3 /* Fraud Call released due to Answer Supervision Timer Expiry */ 
#define CALL_REL_ERROR_FROM_SCP    4 /* Call forcefully released by FIC/SCP */

/* Hash defines for end-to-end information indicator for call Detail 
   Record */
#define CC_CDR_NBNHLINFO    0x01
#define CC_CDR_NBNLLINFO    0x02
#define CC_CDR_PROGIND      0x04
#define CC_CDR_CGPTYSAD     0x08
#define CC_CDR_CDPTYSAD     0x10
#define CC_CDR_NBBEARCAP    0x20
#define CC_CDR_MOREDATA     0x40
#define CC_CDR_CNPTYSAD     0x80
#define CC_CDR_ALLE2EINFO   0x8f

#define MAXFCI               5          /* FID 15045.2 support up to 5 FCI in one CDR */
#define MAXCCMI              3            /* Number of layer manager i/fs */
/* Maximum length of the country code prefix */
#define MAXCCODESIZE         4
/* Maximum length of the trunk prefix */
#define MAXTPREFSIZE         4

/* Call Control Connection Events filled in trc.t.trc.u.ccObsTrc.evntType */

#define CCE_CONIND                0 /* Connect indication                    */
#define CCE_CONCFM                1 /* Connct confirm                        */
#define CCE_ADDRIND               2 /* Address Indication                    */
#define CCE_CNSTIND               3 /* Connection progress status indication */
#define CCE_RELIND                4 /* Connection release indication         */
#define CCE_RELCFM                5 /* Connection release confirmation       */
#define CCE_RSCCFM                6 /* Resouce Confirm                       */
#define CCE_REATIND               7 /* Status Indication - Reattempt         */
#define CCE_TIMEOUTTMREXP       	 8 /* Time Out Timer Expired                */
#define CCE_ALOCCFM               9 /* Resource Allocation Confirmation      */
#define CCE_DEALOCCFM            10 /* Resource Allocation Confirmation      */
#define CCE_MGCTTXNCFM           11 /* MGCT Transaction Confirmation         */
#define CCE_SVC_IND              12 /* Service Indication                    */
#define CCE_INITRELIND           13 /* Status Indication - Initiate Release  */
#define CCE_SETUPTMREXP          14 /* SETUP Timer Expired                   */
#define CCE_RLSTMREXP            15 /* RELEASE Timer Expired                 */
#define CCE_INTERDGTTMREXP       16 /* Inter Digit Timer Expired             */
#define CCE_INITLMRELREQ         17 /* Layer Management Initiated Release    */
#define CCE_HLDIND               18 /* Connection Hold indication            */
#define CCE_RTRIND               19 /* Connection Retrieve indication        */
#define CCE_PCC_MSG              20 /* Message from Peer GCC                 */
#define CCE_DEALOCIND            21 /* Resource Deallocation Indication      */
#define CCE_CALLDTLEXP           22 /* Call Detail Timer Expiry              */
#define CCE_CONTREP              23 /* Continuity Report                     */
#define CCE_CCR                  24 /* Initiate Continuity check             */
#define CCE_STA_STPCONTIN        25 /* Stop Continuity                       */
#define CCE_LOOPBACKACK          26 /* Loop back Acknowledgement             */
#define CCE_TMINDIGITEXPIRY      27 /* Overlap Dialing Support on ISUP       */
#define CCE_TMAXDIGITEXPIRY      28 /* Overlap Dialing Support on ISUP       */
#define CCE_TAWAITANSEXPIRY      29 /* Await Answer Timer Expiry             */
#define CCE_BUFFERTMREXP         30 /* Buffer Timer Expiry                   */
#define CCE_MGCTSVCCHGIND        31 /* MGCT Service Change Indication        */
#define CCE_CALLDTLEXP_STATS     32 /* Stats Call Detail Timer Expiry        */
#define CCE_TMR_REINVITE         33 /* Timer used for Glare(3 sec timer)     */
#define CCE_VIRTUALCALLSETUP     34 /* FID14234.0 Virtual Call Setup from FIC*/
#define CCE_ADDRCFM              35 /* address confirmation                  */
#define CCE_MIDCALLCFM           36 /* mid-call confirmation                 */
#define CCE_ANALYZE_ROUTE        37 /* Analyze Route                         */
#define CCE_AUTHORIZE_TERMINATE  38 /* Authorize Terminate                   */
#define CCE_CONTINUE             39 /* Continue                              */
#define CCE_DISCONNECT_LEG       40 /* Disconnect Leg                        */
#define CCE_FORWARD_CALL         41 /* Forward Call                          */
#define CCE_MERGE_CALL           42 /* Merge Call                            */
#define CCE_MOVE_LEG             43 /* Move Leg                              */
#define CCE_OFFER_CALL           44 /* Offer Call                            */
#define CCE_ORIGINATE_CALL       45 /* Originate Call                        */
#define CCE_RECONNECT            46 /* Reconnect Call                        */
#define CCE_SEND_TO_RESOURCE     47 /* Send To Resource                      */
#define CCE_TSUSPEXP             48 /* AwtResume-(Suspend) Timer Expiry      */
#define CCE_TTMDRELEXP           49 /* TimedRel Timer Expiry                 */
#define CCE_TNOANSEXP            50 /* T_NO_ANSWER Timer Expiry              */	
#define CCE_ONOANSEXP            51 /* O_NO_ANSWER Timer Expiry              */	
#define CCE_DISCONNECT           52 /* Disconnect the CSA                    */	
#define CCE_ORIGINATE_CALEA_CALL 53 /* Originate CALEA call                  */
#define CCE_DATAREQ              54 /* Data Req for Calea                    */
#define CCE_RECONNECTEXP         55 /* Reconnect Timer                       */
#define CCE_CALEALEGEXP          56
#define CCE_CREATECALL           57 /* Create Call From FIC                  */
#define CCE_NOTIFYTIMEREXP       58 /* Notify timer Expiry                   */
#define CCE_EVENTUSEDFORBUFF     59 /* Never use this event                  */
#define CCE_IGNOREFROMFIC        60
#define CCE_CRA                  61 /* Circuit Reserve ack from Peer         */
#define CCE_MGCTNOTIFY           62 /* MGI Notify of tone detect             */
#define CCE_STA_CRM              63 /* CRM from Peer                         */
#define CCE_NEGOTIATION_TMR      64 /* SDP negotiation timed out             */
#define CCE_FCTMWIREQ	         65 /* MWI Update request from FIC	        */
#define CCE_CLOSEFROMFIC         66 /* Close transaction from FIC	           */
#define CCE_KILLCALL             67 /* Kill the Call for a spec ctxId        */
#define CCE_CONGOVRLD_CFM        68 /* FID 15261.0 Congestion/Overload Confirm */
#define CCE_FRAUDCALLTMREXP      69 /* Fraud Call Timer expiry               */
#define CCE_ACKNOWLEDGE          70 /* generic Acknowledge                   */
#define CCE_CONNECT_TO_RESOURCE  71 /* generic Acknowledge                   */
#define CCE_FIC_ERROR            72 /* FIC ERROR message                     */
#define CCE_PAUSETMREXPIRY       73 /* Used for Annoucement framework        */
#define CCE_TOIW2TMREXPIRY       74 /* Feature 1789 - German : Toiw2 timer expire */
#define CCE_RELEASECALL          75 /* ReleaseCall from FIC (euysal 03/17/2005 - CAMEL Support */
#define CCE_EARLYACMTMREXP       76 /* Early ACM timer expiry event */
#define CCE_CAC                  77 /* Create Associated Call Message from FIC */
#define CCE_SWAPCALL             78 /* SWAP CALL message received from FIC */
#define CCE_MOVECALL             79 /* MOVE CALL message received from FIC */
#define CCE_INT_SWAPCALL         80 /* Internal SWAP CALL message received from GCC itself */
#define CCE_INT_MOVECALL         81 /* Internal MOVE CALL message received from GCC itself */
#define CCE_PREANMTMREXP         82 /* Pre-Answer timer expiry event */
#define CCE_CPG_RETRYTMREXP      83 /* FID-2112: CPG Retry Timer Event */
#define CCE_TESTLINE             84 /* Test Line */
#define CCE_TSTLNDSCNTMREXP      85 /* Test Line Disconnect timer expired */
#define CCE_TSTLNCDTMREXP        86 /* Test Line Cadence Disconnect timer expired */
#define CCE_SENDAPM              87 /* FID 14234.0 Send APM from FIC */
#define CCE_SENDACM              88 /* FID 14234.0 Send ACM from FIC */
#define CCE_MGID_RSP             89 /* FID 15083.0 RSP from FIC for MGI QRY */
#define CCE_SENDMSG              90 /* FID 15045.1 SendMsg Request from FIC */
#define CCE_CANCEL               91 /* FID 15045.1 Cancel Request from FIC */
#define CCE_TOIW3TMREXP          92 /* FID 15174.0 Toiw3 timer expiry event */
#define CCE_LIMANINV             93 /* FID 15083.1 LI Manual Invoke from FIC */
#define CCE_ICA                  94 /* FID 15310.1 InitialCallAttempt from FIC */
#define CCE_IACA                 95 /* FID 15310.1 InitialAssociatedCallAttempt from FIC */
#define CCE_CALLQUEUED           96 /* FID 14457.0 Call Queued Message from FIC */
#define CCE_HPCMGRETRYTMREXP     97 /* FID 14457.0 HPC call MG retry timer expiry event */
#define CCE_HPCDELAYREQ          98 /* FID 14457.0,BUG: 88025 HPC Delay Request*/
#define CCE_NCIND                99 /* FID 16206.0, Call Independent Facility Indication from ISDN*/
#define CCE_NCCFM                100 /* FID 16206.0, Out of dialog Message Confirm from SIP*/
#define CCE_CFXCALLINDPTMREXP    101 /* FID 16206.0, Call Independent Timer Expire*/

/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
/******PLEASE ADD CODE IN ccEventStr IN cc_bdy2.c when a new Event is added **/
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/


/* Max events should always be last event + 1. Be sure to update this 
 * when adding new events */
/* Maximum number of Connection Events */
#define CCMAXEVENTS      (CCE_CFXCALLINDPTMREXP + 1)  /*FID16206 change*/


#define CCE_CONREQ       CCMAXEVENTS
#define CCE_CONRSP       (CCMAXEVENTS +  1)
#define CCE_CNSTREQ      (CCMAXEVENTS +  2)
#define CCE_RELREQ       (CCMAXEVENTS +  3)
#define CCE_RELRSP       (CCMAXEVENTS +  4)
#define CCE_RSCRSP       (CCMAXEVENTS +  5)
#define CCE_MNTSTAREQ    (CCMAXEVENTS +  6)
#define CCE_MNTSTAIND    (CCMAXEVENTS +  7)
#define CCE_PROFIND      (CCMAXEVENTS +  8)
#define CCE_RTEREQ       (CCMAXEVENTS +  9)
#define CCE_RTEMNTSTAREQ (CCMAXEVENTS + 10)
#define CCE_RTERSP       (CCMAXEVENTS + 11)
#define CCE_RTERELREQ    (CCMAXEVENTS + 12)
#define CCE_ALOCREQ      (CCMAXEVENTS + 13)
#define CCE_DEALOCREQ    (CCMAXEVENTS + 14)
#define CCE_SWTCONREQ    (CCMAXEVENTS + 15)
#define CCE_SFRELREQ     (CCMAXEVENTS + 16)
#define CCE_ADDRREQ      (CCMAXEVENTS + 17)
#define CCE_MIDCALLREQ   (CCMAXEVENTS + 18)
#define CCE_ORIGATTEMPT   (CCMAXEVENTS + 19)
#define CCE_INFOCOLLECTED (CCMAXEVENTS + 20)
#define CCE_INFOANALYZED  (CCMAXEVENTS + 21)
#define CCE_TERMATTEMPT   (CCMAXEVENTS + 22)
#define CCE_TBUSY         (CCMAXEVENTS + 23)
#define CCE_OMIDCALL      (CCMAXEVENTS + 24)
#define CCE_TMIDCALL      (CCMAXEVENTS + 25)
#define CCE_TDISCONNECT   (CCMAXEVENTS + 26)
#define CCE_ODISCONNECT   (CCMAXEVENTS + 27)
#define CCE_OANSWER       (CCMAXEVENTS + 28)
#define CCE_ONOANSWER     (CCMAXEVENTS + 29)
#define CCE_TNOANSWER     (CCMAXEVENTS + 30)
#define CCE_TONEREQ      (CCMAXEVENTS + 31)
#if 1 /*FGD*/
#define CCE_ADDRCFM_IW     (CCMAXEVENTS + 32)  /* Special Addr Confirm to handle MF-SS7 IW */
#define CCE_STA_CIRRSRVREQ (CCMAXEVENTS + 33) /* Circuit Reserve request to Peer*/
#define CCE_STA_CIRRSRVACK (CCMAXEVENTS + 34) /* Circuit Reserve Ack sent to Peer */
#endif /*FGD*/
#define CCE_SVC_REQ        (CCMAXEVENTS + 35)
#define CCE_INV_EVENT    0xff

/* TELICA - Bugzilla-4321 - Bukucu - 10.25.01 */
/* below values are just for test 
 * they will be changed
*/
#define CCPM_MIN_UPDTHRSHLD_VAL		10
#define CCPM_DFLT_UPDTHRSHLD_VAL	30

typedef enum {
    CC_ALL,
    CC_UPDSTSTHRSHLD,
    CC_TOTCALL,
    CC_ANSCALL,
    CC_FROUTE, 
    CC_FROUTEDPC, 
    CC_FRES,
    CC_UNANSCALL,
    CC_STNDGCALL,
    CC_INSS7,
    CC_OUTSS7,
    CC_INISDN,
    CC_OUTISDN,
    CC_INCAS,
    CC_OUTCAS,
    CC_INGR3,
    CC_OUTGR3,
    CC_INSIP,
    CC_OUTSIP,
    CC_INBICC,
    CC_OUTBICC,
    CC_INMGCP,
    CC_OUTMGCP,
    CC_LASTINITTRMT,
    CC_LASTINITRCV,
    CC_LASTINITNA,
    CC_LNPQUERIES,
    CC_LNPFAILURES,
    CC_LNPVALIDRESPONSE,
    CC_LNPMISRTDCALLS,
    CC_LNPCALLFORPORTSUB,   /*FID 14382.8 */
    CC_LNPCALLFORPORTEDINNET,
    CC_LNPCALLSRELBYDONSWTCH,
    CC_LNPCALLSRELBYTHISSWTCH,
    CC_RMTXFERIN,
    CC_RMTXFEROUT,
    CC_TBXFEROUT,
    CC_CALEAATT,    /* FID 14382.15 */
    CC_CALEASUCC,   /* FID 14382.15 */
} CCPmMonTypes;
#define MAX_NUM_CCPM_MONTYPE	24
#define CCPM_NUM_ROWKEY		1
	
/* end of Bugzilla-4321 */

/* TELICA - Bugzilla-14864 -*/
typedef enum {
   RC_ALL,
   RC_NOT_APPLICABLE,
   RC_MEMORY_ALLOC,                     
   RC_INVALID_TID_INFO,
   RC_INFO_MISSING,     
   RC_DB_ACCESS,        
   RC_SRVC_TRAN_FAILURE,
   RC_INVALID_PARAM, 
   RC_INTERNAL,      
   RC_UNEXPECTED_MSG,
   RC_SCP_OPR_FAILED,                  
   RC_PROTERR,                         
   RC_RESUNAVAIL,                      
   RC_CKTCONG,                         
   RC_TMRRCVRY,                        
   RC_USRBUSY,                         
   RC_CALL_BLOCK,                      
   RC_BCAPNOTIMPL,                     
   RC_SUBIF_NOT_PROV,                  
   RC_NOCKTAVAIL,                      
   RC_LM_INITIATED_REL,                
   RC_ANS_SUPER_REL,                   
   RC_AUTH_FAILURE,                    
   RC_SCR_FRAUD_REL,                   
   RC_NO_DSP_RSC,    
   RC_CODEC_NEG_FAIL,
   RC_UNALLOC_NUMBER,
   RC_NOROUTTRANSNET,
   RC_VACANT_CODE,                     
   RC_INV0PREFIX,                      
   RC_INV1PREFIX,                      
   RC_HAVE1PREFIX,                     
   RC_INVALID_CARRIER,                 
   RC_NMBCHNG,                         
   RC_DSTOUTORD,                       
   RC_INVNMBFORM,                      
   RC_CGPTRY_DIS_OOS_CHG,              
   RC_CALLEDADDR_INCOMPLETE,
   RC_EQUIPMENT_TROUBLE,
   RC_RT_FAILED,        
   RC_INTFC_UNAVAIL,                   
   RC_INTFC_UNAVAIL_COST,              
   RC_INTFC_UNAVAIL_BCAP,              
   RC_NOROUTDST,                       
   RC_EXCG_RT_ERROR,
   RC_REQ_CHAN_NAVAIL,
   RC_WFS_USR_UNAVAIL,
   RC_WFS_UNALLOCNMB, 
   RC_INVALID_MSG,                     
   RC_MSG_UNSUPPORTED,                 
   RC_NOPARAMDISCMSG,                  
   RC_NORMAL_CLRG,                     
   RC_LNP_MISROUTE,
   RC_CASTG_RELIND,
   RC_ENUM_TIMEOUT,
   RC_ENUM_NOE164,
   RC_QUERY_LIMIT_EXCEEDED,
   RC_GMSC_MRKTSCREEN,
/*start bug 62997 add new enum values*/
    RC_ACC_DENIED,
    RC_CALLGAPED,
    RC_ACG_MAN_GAPED,
    RC_ACG_VAC_GAPED,
    RC_ACG_OOB_GAPED,
    RC_ACG_MC_GAPED,
    RC_PRIVACY_CRIT_ERR,
    RC_RSC_PRIORITY_ERROR,
    RC_INACTIVE,
    RC_TERMDENIED,
    RC_SRVCDENIED,
    RC_CASTG_ZERO_DIGITS_RX,
    RC_FACREJ_UUI,
    RC_T7EXPIRY,
    RC_SUB_REG_INACTIVE,
    RC_SUB_PRFL_NOT_FOUND,
    RC_SUB_OOS,
    RC_EMRGNCY_UNRCHABLE,
    RC_OVERLOAD,
    RC_RLT_REJECT,
   
   /*end bug 62997 add new enum values*/
   /*bug69394-FID14732*/
    RC_MG_NO_TERMID,
    RC_RESOURCE_LIMITED,
    RC_MG_UNEQUIPFORREQ,
    RC_MG_NO_ANNOCEMENT,
    RC_NO_SPACE_FOR_DGTMAP,
    RC_BANDWITH_LIMITED,
    RC_MG_RLIST_EXHAUST,
    RC_MG_NOT_READY,

   /*end bug69394-FID14732*/
   /*start FID 14972*/
    RC_CODEC_REORDER_FAIL, 
   /*end FID 14972*/
    
    /* FID 15319.0 */
    RC_SERVNOTIMP,

    /* FID 14457.0 */
    RC_HPC_QUEUE_TIMEOUT,

} RCPmMonTypesnew;

# define MAX_NUM_RCPM_MONTYPE 58

/* Call Control Connection states filled in trc.t.trc.u.ccObsTrc.crntCallState 
 */

#define CCMAXSTATES  (CCS_AWTSWTCFM_FOR_DIALINGTONE + 1) /* Maximum number of Connection States */ /* FID17555.0 change Kavon */

#define CCS_IDLE                       0 /* connection idle                   */
#define CCS_AWTROUTERSC                1 /* AWT resource to start routing     */
#define CCS_AWTROUTEDGT                2 /* awaiting digits to start routing  */
#define CCS_AWTRSPODISCONNECT          3 /* AWT for response of O_DISCONNECT   (far end) */
#define CCS_AWTRSCALOCCFM              4 /* AWT resource aloc confirmation    */
#define CCS_AWTENBLOCSND               5 /* AWT digits for enblock con req    */
#define CCS_AWTOGRSC                   6 /* awaiting outgoing resources       */
#define CCS_AWTANSSWT                  7 /* awaiting answer and switching cfm */
#define CCS_AWTSWTCFM                  8 /* awaiting switching cfm            */
#define CCS_AWTANS                     9 /* awaiting answer                   */
#define CCS_ANSWERED                  10 /* call established                  */
#define CCS_AWTRSPTIMEOUT        	  11 /* AWT for response of timeout       */
#define CCS_AWTRSCCFM_TOCLR           12 /* AWT resource aloc conf to clear   */
#define CCS_AWTOGRSC_TOCLR            13 /* AWT outgoing resource to clear    */
#define CCS_AWT_MGI_XFERQRYHOLD       14 /* AWT txnCfm for xfer/qry/hold      */
#define CCS_AWTRSCALLOCCFM_GLARE      15 /* awaiting resource allco cfm -     */
                                         /* to resolve internal glare - ISUP 
                                          * and GCC choose the same rsc for 2
                                          * diff calls                        */
#define COT_OG_DONE                   16 /* Used for COT Framework            */
#define CCS_AWTRELCFM_IC              17 /* AWT release cfm from incoming     */
#define CCS_AWTRELCFM_OG              18 /* AWT release cfm from outgoing     */
#define CCS_AWTRELCFM_BOTH            19 /* AWT release Cfm from both         */
#define CCS_AWTSWTCFM_TOCLR           20 /* AWT MGCT CFM to complete to clear */
#define CCS_AWTDEALOCCFM              21 /* AWT Dealloc confirmation          */
#define CCS_RETRIEVE                  22 /* Incoming Retrieve in progress     */
#define CCS_AWTALOCCFM_TOCLR          23 /* Resource All Cfm to clear         */
#define CCS_AWTMGCTCFMCOTIC0          24 /* Used for COT Framework            */
#define CCS_AWTCOT                    25 /* awaiting COT                      */
#define CCS_AWTMGCTCFMCOTIC1          26 /* Used for COT Framework            */
#define COT_IC_DONE                   27 /* Used for COT Framework            */
#define CCS_AWTOGSWTCFM               28 /* Outgoing MGCT CFM : NORM call     */
#define CCS_AWTMGCTCFMCOTOG0          29 /* Used for COT Framework            */
#define CCS_AWTMGCTNOTIFY1            30 /* Used for COT Framework            */
#define CCS_AWTDELFROMACT             31 /* Await DEL from ACT in FT/HA env   */
#define CCS_LOCALLYANSWERED           32 /* Call terminated on switch         */
#define CCS_AWTSWTCFMFORSVC           33 /* awaiting switching confirmation   */
#define CCS_AWT_ADDR_CFM              34 /* awaiting address confirmation     */
#define CCS_AWT_MID_CALL_CFM          35 /* awaiting address confirmation     */
#define CCS_AWTRSPFORDPE1             36 /* DP E1                             */
#define CCS_AWTRSPFORDPE3             37 /* DP E3                             */
#define CCS_AWTRSPFORDPE37     	     38 /* DP E37                            */
#define CCS_AWTRSPFORDPE20            39 /* DP E20                            */
#define CCS_AWTRSPFORDPE30            40 /* DP E30                            */
#define CCS_AWTRSPFORDPE11            41 /* DP E11                            */
#define CCS_AWTRSPFORDPE27            42 /* DP E27                            */
#define CCS_AWTRSPFORDPE28            43 /* DP E28                            */
#define CCS_AWTRSPFORDPE9             44 /* DP E9                             */
#define CCS_AWTRSPFORDPE33            45 /* DP E33                            */
#define CCS_AWTRSPFORDPE32            46 /* DP E32                            */
#define CCS_AWTRSPFORDPEXY_TOCLR      47 /* AWT FOR RSP for DP E[1,3,4,20]    */
#define CCS_AWT_REATTEMPT             48 /* Await re-attempt indication       */
#define CCS_AWTRSPFORDPE22            49 /* AWT for RSP for MSG sent on DP 22 */
#define CCS_AWTCALEALEGSWTCFM         50 /* AWT for MGCT for CALEA Leg        */
#define CCS_AWTBUSYREL_IC             51 /* awaiting for release on usrbusy   */
#define CCS_AWT_RESUME                52 /* AWT for resume/off-hook           */
#define CCS_AWT_TMDRELEXP             53 /* AWT for tmdRel-expiry/on-hook     */
#define CCS_AWTSWTCFM_SENDBOTHREL     54 /* AWT MGI rsp for sub, release both */
#define CCS_AWTSWTCFM_SENDONEREL      55 /* AWT MGI rsp for sub, release one  */
#define CCS_AWTSWTCFM_SENDNOREL       56 /* AWT MGI rsp for sub, no rel reqd. */
#define CCS_AWTMGCTCFMFORMODIFY0      57 /* State for Annoucement framework   */
#define CCS_AWTMGCTCFMFORMODIFY1      58 /* State for Annoucement framework   */
#define CCS_AWTRELCFM_KEEP_CON        59 /* is used for Call Transfer         */
#define CCS_AWTRSCCFMNOTIFY           60 /* ACB STUFF                         */
#define CCS_NOTIFYRSPFORMSUB          61 /* ACB STUFF                         */
#define CCS_AWTRSPFORDPE19            62 /* For OCALLED PARTY BUSY EVENT      */
#define CCS_AWTSWTCFMFORCC6CC10       63 /* For 3WC                           */
#define CCS_AWTRSPFAILOUT             64 /* For Failure OutCome               */
#define CCS_AWTSWTCFMFORHOLD          65 /* When SUB flashes to originate     */
#define CCS_AWTSWTCFMCONFTONE         66 /* Will be used for conf tone        */
#define CCS_AWTSWTCFMFORSPLIT3WC      67 /* Spliting 3WC                      */
#define CCS_CANNOTPROCESSEVENTS       68 /* State !process any event, buffer  */
#define CCS_AWTSWTCFMFORMERGEINCC4    69 /* Used for Merge Call in CC4        */
#define CCS_AWTSWTCFM_MERGEINCC6IC    70 /* Used for Merge Call in CC6 IC     */
#define CCS_AWTSWTCFM_MERGEINCC6OG    71 /* Used for Merge Call in CC6 OG     */
#define CCS_AWT_PERMSIG_TREATMENT_CFM 72 /* Perm-sig treatment to CAS line    */
#define CCS_AWT_QRYRSP                73 /* State for GCC sub-state m/c       */
#define CCS_AWT_XFRRSP_FROM_1         74 /* State for GCC sub-state m/c       */
#define CCS_AWT_XFRRSP_FROM_2         75 /* State for GCC sub-state m/c       */
#define CCS_AWTSWTCFMFORCC6CC11       76 /* FOR AOTC                          */
#define CCS_AWT_MGI_SUBCFM_TO_QRY     77 /* State for GCC sub-state m/c       */
#define CCS_AWT_RSP_TO_CLR_BEARER     78 /* State for GCC sub-state m/c       */
#define CCS_AWT_FIC_RSP_TO_KILL       79 /* When GCC receives MGI REL IND     */
#define CCS_AWT_QRYRSP_ONSWITCH       80 /* State for GCC sub-state m/c       */
#define CCS_AWT_XFRRSP_1_ONSWITCH     81 /* State for GCC sub-state m/c       */
#define CCS_AWTANNCMPLT_CTR           82 /* EVENT IND (ANN COMPLT) from MGI   */
#define CCS_AWT_XFRRSP_2_ONSWITCH     83 /* State for GCC sub-state m/c       */
#define CCS_AWTRSCALLOCCFM_MWI        84 /* AWT rsc cfm from RM for MWI       */
#define CCS_AWTMWIUPDCFM              85 /* AWT MWI update CFM from CAS	      */
#define CCS_AWTRSPFORDPE17            86 /* Network Busy                      */
#define CCS_AWTSWTCFMFORDP            87 /* Awt MGI CFM                       */
#define CCS_AWTFICRSP_2_XFER          88 /* Awaiting O/T-Transfer- X1         */
#define CCS_AWTMGCTNOTIFY0            89 /* Used for Annc framework           */
#define CCS_AWTMGCTCFMFORMODIFY2      90 /* Used for Annc framework           */
#define CCS_AWTPAUSETIMEREXPIRY       91 /* Used for Annc framework           */
#define CCS_AWTMGCTCFMCOTOG1          92 /* Used for COT Framework            */
/* International Development: For Overlap Digit Operation */
#define CCS_AWTMINDIGITS              93 /* awaiting min digits to route */
#define CCS_AWTMAXDIGITS              94 /* awaiting dialing completion  */
/*euysal - 04/21/2005 - Support for O_Answer request */
#define CCS_AWTRSPFORDPE08            95 /* awaiting dialing completion  */
#if 0
#define CCS_AWTINFO                   96 /* awaiting information requested    */
#else
#define CCS_AWTMOREINFO               96 /* awaiting more info from ISUP */
#endif
/* euysal - 06/15/2005 - ACC Support */
#define CCS_AWTMGCTCFMFORMODIFY1_CTR  97
#define CCS_AWTMGCTCFMFORMODIFY2_CTR  98 /* Used for Annc framework           */
#define CCS_AWTRSPFORDPE12            99 /* Awaiting O_Suspend response - ACC */
#define CCS_AWTMGCTCFMFOREARLYACM    100 /* Awaiting MGCT response for RingBack */
#define CCS_AWTDELAYEDREL            101 /* awaiting delayed release */
#define CCS_AWT_MGIT_SUBSCRIBE       102 /* Awaiting MGIT  - X2 */
#define CCS_AWT_MGIT_CREATE          103 /* Awaiting MGIT to Route/RLT- X4 */
#define CCS_AWT_RLT                  104 /* Awaiting RLT to complete  - X5 */

#define CCS_AWT_OFFNET_XFERRSP       105 /* Awaiting for XFER RSP for OFFNET */
#define CCS_COB_AWTXFERRSPBOTH       106 /*CreateOnSwitchBearer(COB)processin */
#define CCS_COB_AWTXFERRSPIN         107 /* COB Processing */
#define CCS_COB_AWTXFERRSPOUT        108 /* COB Processing */
#define CCS_COB_AWTMGCTADDICANDOUT   109 /* COB Processing */
#define CCS_COB_AWTMGCTSUBALL        110 /* COB Processing */
#define CCS_COB_AWTMGCTICSUBADD_AWTXFERRSPOUT 111 /* COB Processing */
#define CCS_COB_AWTMGCTICSUBADD               112 /* COB Processing */
#define CCS_COB_AWTXFERRSPINANDXFEROUT        113 /* COB Processing */
#define CCS_COB_AWTXFERROUT                   114 /* COB Processing */
#define CCS_COB_AWTMGCTOUTSUBADD_AWTXFERRSPIN 115 /* COB Processing */
#define CCS_COB_AWTMGCTTOREL                  116 /* COB Processing */
#define CCS_COB_AWTMGCTOUTSUBADD              117 /* COB Processing */
#define CCS_COB_AWTMGCTMODICANDOUT            118 /* COB Processing */
#define CCS_COB_AWTXFERRSPINANDOUTADD         119 /* COB Processing */
#define CCS_COB_AWTMGCTOUTADD                 120 /* COB Processing */
#define CCS_AWTTMGCTFORSWAPCALL               121 /* AWT MGI for SWAP CALL */
#define CCS_AWTTMGCTFORMOVECALL               122 /* AWT MGI for MOVE CALL */
#define CCS_AWTIDISCONNECT                    123 /* AWT FIC for I-Disconnect Response */
#define CCS_AWTRSPFORDPE7                     124 /* Waiting for OTermSeized Response */
#define CCS_CBI_AWTMGI1                       125 /* CBI Processing */
#define CCS_CBI_AWTOGQRYRSP                   126 /* CBI Processing */
#define CCS_CBI_AWTICXFERRSP1                 127 /* CBI Processing */
#define CCS_CBI_AWTOGXFER                     128 /* CBI Processing */
#define CCS_CBI_AWTSUB                        129 /* CBI Processing */
#define CCS_AWTMGCTCFM_ACK                    130 /* U0 UPDATE FID-2112 */
#define CCS_UPDSES_AWTMG_1                    131 /* U1 UPDATE FID-2112 */
#define CCS_UPDSES_AWTRSP                     132 /* U2 UPDATE FID-2112 */
#define CCS_UPDSES_AWTMG_2                    133 /* U3 UPDATE FID-2112 */
#define CCS_AWT_MGCTCFM_LLTR       134  /* AWT for MGCT confirm for line test loopback */
#define CCS_AWT_MGCTCFM_TSTLNTONE  135  /* AWT for MGCT confirm for line test tone */
#define CCS_AWT_MG_PRACKRSP                   136  /* AWT for MGI for PRACK RSP */
#define CCS_AWT_MGI_CFM_FOR_DL2               137  /* AWT for MGI CFM for Disconnect Leg 2 */
#define CCS_AWT_RSP_FOR_SO                    138  /* AWT for RSP FOR SuccessOutCome */
#define CCS_AWT_CONGOVRLD_CFM                 139  /* AWT congestion/overload Confirm */
#define CCS_AWTRSPOAPM                        140  /* AWT for OAPM RSP from FIC */
#define CCS_AWTCFM_FOR_LI                     141  /* AWT for TxnCfm of LI terminations */
#define CCS_AWT_MGID_RSP                      142  /* AWT for MGID Rsp from RS */
#define CCS_CMAB_AWT_IC_QRY_RESP              143  /* AWT IC Query Resp */
#define CCS_CMAB_AWT_MGI_CFM_IC_QRY_RESP      144  /* AWT MGI CFM post IC Query Resp */
#define CCS_CMAB_AWT_OG_XFER_RESP             145  /* AWT OG Xfer Response */
#define CCS_CMAB_AWT_MGI_CFM_OG_XFER_RESP     146  /* AWT MGI CFM post OG Xfer Response */
#define CCS_BICC_AWTICBEARER                  147  /* AWT IC ExtBICC bearer setup */
#define CCS_BICC_AWT_MGCT_CFMNOT              148  /* AWT MG confirm/notify for ExtBICC bearer*/
#define CCS_AWTOREDIRECT_RSP                     149  /* AWT FIC response for 3XX OREDIRECT msg */
#define CCS_AWTHPCMGRETRYTIMEREXPRIY          150  /* AWT HPC call MG retry timer expriy FID 14457.0 */
#define CCS_AWTNCCFM                         151  /* FID16206.0,AWT SIP Message Confirm*/
#define CCS_AWTNCIND                         152  /* FID16206.0,AWT SIP Message Indication*/
#define CCS_AWTSWTCFMFORSUBADD               153  /* BUG:90302, AWT MG CFM for Back2back SUB and ADD*/
#define CCS_AWT_PRECONDITION                 154  /* FID 15068.0, PRECONDIDION NEGOTIATION PROCESS*/
#define CCS_AWT_MRFRSP                       155  /* FID 17174.0, MRF play annc and wait for response */
#define CCS_AWTSWTCFM_FOR_DIALINGTONE        156  /* FID 17555.0, await switching cfm for playing/stopping dialing tone Kavon */

/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
/******PLEASE ADD CODE IN ccStateStr IN cc_bdy2.c when a new State is added **/
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/



/* Active call states */
#define CC_IS_ACTIVE_CALL_STATE(state) ( \
    ((state) == CCS_ANSWERED)        ||      \
    ((state) == CCS_LOCALLYANSWERED) ||      \
    ((state) == CCS_RETRIEVE))

#define CC_IS_CALL_WAIT4ACT2DEL(state) ((state) == CCS_AWTDELFROMACT)

/* Interface control block states */
#define CC_INTFC_ENABLED          0 /* Interface is enabled */
#define CC_INTFC_DISABLED         1 /* Interface is disabled */
#define CC_INTFC_DISGRC           2 /* Interface is in the process of being 
                                     * disabled 
                                     */
#define CC_INTFC_DELETED          3 /* Interface is deleted */ 

#define CC_TONE_TEST_ON           1 /* Enable Tone Test  */
#define CC_TONE_TEST_OFF          0 /* Disable Tone Test  */

#define SIP_PSTN_XML_ENABLED      1 /* FID 14811.0 */
#define SIP_PSTN_XML_DISABLED     0 

/* Test call control - detailed action */
#define LCC_CONTINUITY_CALL 0x01    /* Continuity test call */

#define MAX_FRAUD_CALL  18 /* Max no. of fraud Call enteries sent in StaCfm */ 

#define LINE_INTFC      0
#define TRKGRP_INTFC    1 

#define MAX_GAP_BMAP    8

#define MAX_AOCNETID_LEN  84  /* FID 14840 */

/* Error codes */
#define   ELCCBASE     0            /* reserved */
#define   ELCCXXX      ELCCBASE     /* reserved */

/* #defines for xferIn, xferOut */
#define LCC_XFER_NONE      0x00
#define LCC_XFER_INGRESS   0x01
#define LCC_XFER_EGRESS    0x02
#define LCC_XFER_BOTH      0x03

/* defines for uriType */
#define  LCC_URI_SIP       0x00
#define  LCC_URI_TEL       0x01
#define  LCC_URI_SIPS      0x02    /* FID 17364.0 */

/* #defines for privacy in SIP profile */
#define    LCC_SIP_PRIVACY_NONE        0x00
#define    LCC_SIP_PRIVACY_P_ASSERT    0x01
#define    LCC_SIP_PRIVACY_RMTPRTY_ID  0x02

/* #defines for reasonHdrPrtcl in SIP/SIPT profile */
#define    LCC_SIP_REASONHDRPRTCL_NONE   0x00
#define    LCC_SIP_REASONHDRPRTCL_Q850   0x01
#define    LCC_SIP_REASONHDRPRTCL_SIP    0x02

/* Added for feature 1833 requirement 4864 */
#define		NORMAL		0x00
#define		RELEASE		0x01	

/* #defines for SIP IW PROCEDURE Params in SIP profile */
#define    LCC_SIP_IW_PROC_Q1912_5     0x00
#define    LCC_SIP_IW_PROC_RFC3398     0x01
/* BUG: 41155 ACM TO 183 */
#define    LCC_SIP_IW_PROC_ACM_TO_183  0x02
#define    LCC_SIP_IW_PROC_T1679       0x03
#define    LCC_SIP_IW_PROC_183T0ACMSUBFREE 0x04 /* 53154 */
#define    LCC_SIP_IW_PROC_3GPPTS29_163 0x05 /* FID 15319.0 */
#define    LCC_SIP_IW_PROC_Q737_1      0x06 /* FID 16522.0 */
#define    LCC_SIP_IW_PROC_Q735_1      0x07 /* FID 16522.0 */
#define    LCC_SIP_IW_PROC_3GPPTS29_163_V12 0x08 /* FID 17378.0 */
#define    LCC_SIP_IW_PROC_3GPPTS29_163_V12_CMCC 0x09 /* FID 17378.0 */

/* Prefixes used for International/Operator calls */
#define LCC_SIP_INT_PREFIX         "011"
#define LCC_SIP_INT_OPER_PREFIX    "01"
#define LCC_SIP_NAT_OPER_PREFIX    "0"
#define LCC_SIP_MAX_STR_LEN        64

/* #-defines for xferIn, xferOut */
#define LCC_XFER_NONE      0x00
#define LCC_XFER_INGRESS   0x01
#define LCC_XFER_EGRESS    0x02
#define LCC_XFER_BOTH      0x03

/* FID-2593: Transcoding + */
#define LCC_PREFCODEC_ANY     0x00
#define LCC_PREFCODEC_CODEC1  0x01
#define LCC_PREFCODEC_MG      0x02
#define LCC_PREFCODEC_TANDEM  0x03  /* FID-14209: Enhanced Transcoding */
/* FID-2593: Transcoding - */

/* FID 15681.0 - Setting Transcoding Policies */
#define LCC_TRANSCODE_POLICY_DEFAULT            1000
#define LCC_TRANSCODE_POLICY_DIRECT             1001
#define LCC_TRANSCODE_POLICY_ANY                1002
#define LCC_TRANSCODE_POLICY_CODEC_PAIR         1003
/* FID 15681.0 - Setting Transcoding Policies */

/* FID-2575: LMSD + */
/* CC defines for emHdrType from prfl-sipt */
#define LCC_EMHDRTYPE_NONE              0x00
#define LCC_EMHDRTYPE_LMSD_AI           0x01

/* FID 16521.0  */
/* CC defines for emHdrType for prfl-sip */
#define LCC_SIP_EMHDRTYPE_NONE          0x00
#define LCC_SIP_EMHDRTYPE_SALUD_AI      0x01

/* FID-13979: */
/* CC defines for divInfo from prfl-sipt */
#define LCC_DIVINFO_DIVHDR         0x00
#define LCC_DIVINFO_HISTHDR         0x01

/* CC defines for ringBackTone from trkgrp */
#define LCC_RINGBACKTONE_NONE      0x00
#define LCC_RINGBACKTONE_LOCAL     0x01
#define LCC_RINGBACKTONE_UPSTREAM  0x02
/* FID-2575: LMSD - */

/* FID-14209: Enhanced Transcoding + */
#define LCC_CODECREORDER_NONE  0x00
#define LCC_CODECREORDER_LMSD  0x01
#define LCC_CODECREORDER_PASSTHRU 0x02
#define LCC_CODECREORDER_PASSTHRUNOFLTR 0x03
#define LCC_CODECREORDER_AGNOSTIC 0x04
#define LCC_CODECREORDER_AGNOSTICTP 0x05

#define LCC_IWGAP_NOGAP        0x00
#define LCC_IWGAP_NOPRACK      0x01
#define LCC_IWGAP_PRACKONLY    0x02
#define LCC_IWGAP_PRACKUPDATE  0x03
/* FID-14209: Enhanced Transcoding - */

/* FID 14234.0 RBWF call indicator */
#define LCC_CDR_RBWF_VIRTUAL_CALL  0x01
#define LCC_CDR_RBWF_REAL_CALL     0x02

/* FID15083.0 */
#define LCC_TRKGW    /* Trunking Gateway */
#define LCC_BRDGW    /* Border Gateway */
#define LCC_CTBGW    /* Converged Trunk and Border Gateway */

/* + FID 15801.0 */
#define LCC_3XX_NORMAL                0x00
#define LCC_3XX_RELEASE               0x01
#define LCC_3XX_ADVANCED              0x02
#define LCC_3XX_PASSTHRU              0x03   /* FID 17026 */
/* - FID 15801.0 */

/* + FID 16382.0 */
#define    LCC_SIPIW_NONE            0x00
#define    LCC_SIPIW_ANON_AURION     0x01
#define    LCC_SIPIW_ANON_AURIOFF    0x02
#define    LCC_SIPIW_ANOFF_AURION    0x03
#define    LCC_SIPIW_ANOFF_AURIOFF   0x04
/* - FID 16382.0 */

/* + FID 14811.0 */
#define CON_EVENT                     0x01
#define CNST_EVENT                    0x02
/* - FID 14811.0 */

/* FID 16661.0 */
#define    LCC_HIST_INFO_FMT_DEFAULT   0x00
#define    LCC_HIST_INFO_FMT_29163_746 0x01
#endif /* __LCCH__ */

/* FID 16925.0 + */
#define    LCC_SIP_INBND_NULL        0x00
#define    LCC_SIP_INBND_INBANDAVIL  0x01
/* FID 16925.0 - */

/* FID16584.0 */
#define   LENUM_MAX_DOMAIN_STRING_LEN 65

/* FID 17180.0 */
#define   LCC_PEM_SENDRECV            0x00 
#define   LCC_PEM_SENDRECV_INACTIVE   0x01 

/* FID 16970.0 + */

/* CCBS ISDN call indicator */
#define LCC_CDR_CCBS_NORMARL_CALL  0x00
#define LCC_CDR_CCBS_REAL_CALL     0x01
#define LCC_CDR_CCBS_VIRTUAL_CALL  0x02

/* CCBS ISDN call mode */
#define   CCBS_ISDN_MODE_NOTIFIER     1
#define   CCBS_ISDN_MODE_SUBSCRIBER   2

/* CCBS ISND state */
#define   CCBS_ISDN_ST_IDLE                               0
#define   CCBS_ISDN_ST_WAIT_SUB200OK_ADD                  1
#define   CCBS_ISDN_ST_WAIT_NOTIFY_REQ_QUEUED             2
#define   CCBS_ISDN_ST_WAIT_NOTIFY_USR_FREE_FOR_RECALL    3
#define   CCBS_ISDN_ST_WAIT_NOTIFY_SEVICE_COMPLETE        4
#define   CCBS_ISDN_ST_WAIT_SUB200OK_SUSPEND              5
#define   CCBS_ISDN_ST_WAIT_SUB200OK_RESUME               6
#define   CCBS_ISDN_ST_WAIT_NOTIFY_SUSPEND_REQ_QUEUED     7
#define   CCBS_ISDN_ST_WAIT_NOTIFY_RESUME_REQ_QUEUED      8
#define   CCBS_ISDN_ST_WAIT_FACILITY_REQUEST_RR           9
#define   CCBS_ISDN_ST_WAIT_NOT200OK_REQ_QUEUED           10
#define   CCBS_ISDN_ST_WAIT_FACILIFY_REMOTE_USER_FREE     11
#define   CCBS_ISDN_ST_WAIT_NOT200OK_USER_FREE_FOR_RECALL 12
#define   CCBS_ISDN_ST_WAIT_RELEASE                       13
#define   CCBS_ISDN_ST_WAIT_FACILITY_RESUME               14
#define   CCBS_ISDN_ST_WAIT_SUBSCRIBE_RESUME              15
#define   CCBS_ISDN_ST_WAIT_NOT200OK_TERMINATE            16
#define   CCBS_ISDN_ST_WAIT_NOT200OK_SUSPEND_REQ_QUEUED   17
#define   CCBS_ISDN_ST_WAIT_NOT200OK_RESUME_REQ_QUEUED    18
#define   CCBS_ISDN_ST_WAIT_SUB200OK_TERMINATE            19
#define   CCBS_ISDN_ST_WAIT_NOTIFY_TERMINATE              20
#define   CCBS_ISDN_ST_WAIT_RELEASE_COMPLETE              21
#define   CCBS_ISDN_ST_WAIT_NOT200OK_SEVICE_COMPLETE      22
#define   CCBS_ISDN_ST_WAIT_RELCFM_BOTH                   23
#define   CCBS_ISDN_ST_MAX_STATE                          24 

/* CCBS ISDN event */
#define   CCBS_ISDN_EVT_SUB_IND        0
#define   CCBS_ISDN_EVT_SUB_CFM        1
#define   CCBS_ISDN_EVT_NOT_IND        2
#define   CCBS_ISDN_EVT_NOT_CFM        3
#define   CCBS_ISDN_EVT_TMR_EXP        4
#define   CCBS_ISDN_EVT_REG            5
#define   CCBS_ISDN_EVT_FAC            6
#define   CCBS_ISDN_EVT_REL            7
#define   CCBS_ISDN_EVT_REL_CFM        8
#define   CCBS_ISDN_EVT_MAX_EVENT      9
 
/* FID 16970.0 - */

/* FID 17210 + */
#define  LCC_SIP_IW_OCN_RFC3398        1
#define  LCC_SIP_IW_OCN_TS29163        2
/* FID 17210 - */

/* FID 17364 */
#define CDPTYSIPSURIIND 0x0001
#define CGPTYSIPURIIND  0x0002
#define ORIGCDSIPURIIND 0x0004
#define REDIRGSIPURIIND 0x0008

  
/********************************************************************30**
  
         End of file: lcc.h 1.5  -  12/21/99 11:54:44

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
             cc009.13 zy   1. Add more reasons
1.4          ---      mg   1. Changes for ICC 1.4 release.

1.5          ---      zy   1. Changes for ICC 1.5 Release.
1.6          ---      ak   1. Added new State and Event defination 
                              to the existing defination of state Machine
                              to support AIN.
1.7          ---      ak   1. Changed the name of the states to follow
                              the coding convention.
1.8          ---      ak   1. Added four more states for handling
                              releases when waiting for responses from
                              FIC. These are for handling the response
                              for triggers( 47 to 50).
                           2. Also changed the CCMAXSTATES to 51.
*********************************************************************91*/
