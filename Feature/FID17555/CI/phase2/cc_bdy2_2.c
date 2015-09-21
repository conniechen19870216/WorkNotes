/********************************************************************20**
  
        Name:     Call Control - body 2 extension
    
        Type:     C source file
 
        Desc:     Contains functions used in STATE TABLE of cc_bdy2.c 

        Sid:      cc_bdy2_2.c
  
        Prg:      Kumar 
  
                  of GCC.
*********************************************************************21*/
 

/*
 *      This software may be combined with the following TRILLIUM
 *      software:
 *
 *      part no.                      description
 *      --------    ----------------------------------------------
 *    
 */

  
/* header include files (.h) */
#include <unistd.h>
  
#include "envopt.h"             /* environment options */  
#include "envdep.h"             /* environment dependent */
#include "envind.h"             /* environment independent */

#include "gen.h"                /* general */
#include "ssi.h"                /* system services interface */
#include "cm_ss7.h"             /* general SS7 layer */
#include "cm_atm.h"             /* general ATM layer */
#include "cm_hash.h"            /* hash list structure */
#include "cm_llist.h"           /* linked list structure */
#include "cm_cc.h"              /* Common Call Control Hash Defs */
#include "sit.h"                /* PSIF interface*/
#include "int.h"                /* PSIF interface*/
#include "cst.h"
#include "sipt.h"
#include "mgcpt.h"
#include "tfsu_memmap.h"
#include "cct.h"                /* PSIF interface*/
#include "rtt.h"                /* router interface */
#include "rm.h"                 /* Resource Manager defines */
#include "rmt.h"                /* Resource Manager interface */
#include "sft.h"                /* Switching Fabric Manager interface */
#include "lcc.h"                /* layer management */
#include "mf.h"                 /* Message Decoding / Encoding Hdr */
#include "cm5.h"                /* timers */
#include "msg_enum_1299.h"      /* Common message types (CC and FIC) */
#include "fct.h"                /* FIC datastructures */
#include "fst.h"                /* FIC_NUM_CP_EVNTS defintion */
#include "fctcommon.h"          /* Close Error Codes  */
#include "cc.h"                 /* Call Control Private header file */
#include "cc_err.h"             /* Call Control Error Codes */
#ifdef ZC
#include "cm_pftha.h"      /* common PSF */
#include "lzc.h"           /* ZC LM defines */
#include "zc.h"            /* ZC defines */
#include "zc_err.h"        /* ZC error defines */
#endif /* ZC */
#include "cct.h"   
#include "lin.h"
#include "cm_sdp.h"             /* SDP */
#include "tb_cccfg.h"
#include "tb_sdp.h"


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
#include "cst.x"
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
#include "cc.x"                 /* Call Control Private header file */
#include "msg_enum_1299.h"      /* For enums */
#include "fctcommon.h"          /* Macros */
#ifdef ZC
#include "cm_pftha.x"      /* common PSF */
#include "lzc.x"           /* ZC LM */
#include "zc.x"            /* ZC typedefs */
#endif /* ZC */
#include "SoftSwitchDBApi.h"
#include "git_class.h"
#include "proc_info.h"
#include "telica_defs.h"
#include "billing_cmds.h"
#include "stdlib.h"
#include "tsmc_logutil.h"
#include "cm_pcrcdr.h"
#include "agc_EM_NE_enum.h"
#include "telica_errtreat.h"
#include "cc_treat.h"
#include "cc_peer.h"
#include "cc_overlap.h"
#include "cc_geo.x"
#include "cc_geo_cri.h"
#include "cc_geo_cri.x"
/* CPDI */
#include "telica_std.h"
#include "audio_annc.h"
#include "si.h"
#include "OalGen.h"

#include "cc_li.h"
#include "cc_li.x"
#include "bicct.h"
#include "cc_asn.h"   /*FID 16206.0*/
#include "cc_ccbs.x"  /*FID 16970.0*/


/* local defines */
PUBLIC  S16 ccBufE01SXX ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));
PUBLIC  S16 ccConE11S82 ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));

PUBLIC  S16 ccConE03S14 ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));
PUBLIC  S16 ccConE11S14 ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));
PUBLIC  S16 ccConE11S26 ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
				PTR event2));
/* Added for Bug 18422 */
PUBLIC  S16 ccConE63S00 ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));

PUBLIC  S16 ccConE14S57 ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));
PUBLIC  S16 ccConE14S58 ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));
PUBLIC  S16 ccConE14S72 ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));
PUBLIC  S16 ccConE66SXX ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));
PUBLIC  S16 ccConE48SBUF
                        ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                                       PTR event2));
PUBLIC  S16 ccConE50SBUF
                        ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                                       PTR event2));
PUBLIC  S16 ccConE51SBUF
                        ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                                       PTR event2));

PUBLIC  S16 ccConE67SXX ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));
PUBLIC  S16 ccConE67SFICRSP
                        ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                                       PTR event2));
PUBLIC  S16 ccConE67SBUF
                        ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                                       PTR event2));
PUBLIC  S16 ccConE67SMGI
                        ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                                       PTR event2));
PUBLIC  S16 ccConE03S61  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));

PUBLIC S16 ccConE71SBUF ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));
PUBLIC S16 ccConE52S37  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));
PUBLIC S16 ccConE03S_FICBUF
                         ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                                        PTR event2));
PUBLIC S16 ccConE04SBUF  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                                        PTR event2));
PUBLIC S16 ccConE37S62  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));
PUBLIC S16 ccConE37S138  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));

/* + 14841.2 */
PUBLIC S16 ccConE68S139  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
PUBLIC S16 ccConE10S139  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
/* - 14841.2 */

PUBLIC S16 ccConE39S62  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));
PUBLIC S16 ccConE47S62  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));
PUBLIC S16 ccConE52S62  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));
 
PUBLIC S16 ccConE37S45  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));
PUBLIC S16 ccConE39S44  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));
PUBLIC S16 ccConE39S45  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));
PUBLIC S16 ccConE47S45  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));
PUBLIC S16 ccConE52S45  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));

PUBLIC S16 ccConE37S86  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));
PUBLIC S16 ccConE39S86  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));
PUBLIC S16 ccConE47S86  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));
PUBLIC S16 ccConE52S86  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));

PUBLIC S16 ccConE11S87 ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));
PUBLIC S16 ccConE14S87 ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));
PUBLIC S16 ccConE15S03 ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                              PTR event2));

PUBLIC S16 ccConE69SXX ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                              PTR event2));
PUBLIC S16 ccConE37S09 ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                              PTR event2));
PUBLIC S16 ccConE37S10 ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
			      PTR event2));
PUBLIC S16 ccConE69SBUF ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));
PUBLIC S16 ccConE37SBUF ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                              PTR event2));
PUBLIC S16 ccConE37SCQBUF ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                              PTR event2)); /* FID 14457.0 */
#if 1 /* EARLY_ACM */
PUBLIC S16 ccConE76SBUF ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));
PUBLIC S16 ccConE38SBUF ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                              PTR event2));
PUBLIC S16 ccConE39SBUF ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                              PTR event2));
PUBLIC S16 ccConE47SBUF ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                              PTR event2));
PUBLIC S16 ccConE05SBUF ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                              PTR event2));
PUBLIC S16 ccConE06SBUF ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                              PTR event2));
PUBLIC S16 ccConE62SBUF ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                              PTR event2));
PUBLIC S16 ccHandleE76Exp ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                              PTR event2));
#endif
PUBLIC S16 ccConE14S18  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                              PTR event2));
PUBLIC  S16 ccConE39S79 ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));
PUBLIC  S16 ccConE40S79 ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));
PUBLIC  S16 ccConE41S79 ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));
PUBLIC  S16 ccConE42S79 ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));
PUBLIC  S16 ccConE43S79 ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));
PUBLIC  S16 ccConE44S79 ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));
PUBLIC  S16 ccConE45S79 ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));
PUBLIC  S16 ccConE46S79 ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));
PUBLIC  S16 ccConE50S79 ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));
PUBLIC  S16 ccConE51S79 ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));
PUBLIC  S16 ccConE52S79 ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));
PUBLIC  S16 ccConE31S32 ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));

/* CPDI prepaid */
//PUBLIC  S16 ccConE37S03  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
//                                PTR event2));
PUBLIC  S16 ccConE39S03  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
PUBLIC  S16 ccConE47SXX  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
/* euysal - 12/06/2005 - Bug:42382 */
PUBLIC  S16 ccConE47S09  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
PUBLIC  S16 ccConE52S03  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
PUBLIC  S16 ccConE71S03  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
PUBLIC  S16 ccConE62S09  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
PUBLIC  S16 ccConE62S51  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
PUBLIC  S16 ccConE08S10  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
PUBLIC  S16 ccConE70S10  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
/* euysal - 03/16/2005 - CAMEL Support */
#if 1
PUBLIC  S16 ccConE71S10  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
PUBLIC  S16 ccConE14S82  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
PUBLIC  S16 ccConE04S82  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
PUBLIC  S16 ccConE62S82  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm, 
                                PTR event2));   
EXTERN  S16 ccConE11S97  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));
EXTERN  S16 ccConE14S97  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
EXTERN  S16 ccConE31S97  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));
#endif
PUBLIC  S16 ccConE37S11  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
PUBLIC  S16 ccConE39S11  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
PUBLIC  S16 ccConE52S11  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
PUBLIC  S16 ccConE70S11  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
PUBLIC  S16 ccConE71S11  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
PUBLIC  S16 ccConE08S38  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
PUBLIC  S16 ccConE11S38  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
PUBLIC  S16 ccConE37S38  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
PUBLIC  S16 ccConE39S38  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
PUBLIC  S16 ccConE47S38  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
PUBLIC  S16 ccConE52SXX  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
PUBLIC  S16 ccConE62S52  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
PUBLIC  S16 ccConE62S53  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
PUBLIC  S16 ccConE03S82  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
PUBLIC  S16 ccConE11S82  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
PUBLIC  S16 ccConE31S82  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm, 
                                PTR event2));   
PUBLIC  S16 ccConE08SXX  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
PUBLIC  S16 ccConE31SYY  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));

/* end of CPDI state functions */
PUBLIC  S16 ccConE47S40 ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));
PUBLIC  S16 ccConE47S46 ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                              PTR event2));
EXTERN  S16 ccConE72S10  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                              PTR event2));
EXTERN  S16 ccConE72S09  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                              PTR event2));
EXTERN  S16 ccConE72SXX  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                              PTR event2));
EXTERN  S16 ccConE72SXY  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                              PTR event2));
EXTERN  S16 ccConE47S36  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                              PTR event2));
PUBLIC  S16 ccConE04S56  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
PUBLIC  S16 ccConE01S18  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
PUBLIC  S16 ccConE03S56  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
EXTERN  S16 ccConE11S57  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));
EXTERN  S16 ccConE31S57  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));
EXTERN  S16 ccConE11S58  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));
EXTERN  S16 ccConE14S58  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
EXTERN  S16 ccConE31S58  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));
EXTERN  S16 ccConE03S91  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
EXTERN  S16 ccConE04S91  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
EXTERN  S16 ccConE73S91  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
EXTERN  S16 ccConE03S89  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
EXTERN  S16 ccConE04S89  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
EXTERN  S16 ccConE14S89  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
EXTERN  S16 ccConE62S89  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
EXTERN  S16 ccConE03SMGIBUF
                         ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
EXTERN  S16 ccConE03SBUF ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
EXTERN  S16 ccConE03S2X  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
EXTERN  S16 ccConE14S90  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
EXTERN  S16 ccConE11S90  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
EXTERN  S16 ccConE14S98  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
EXTERN  S16 ccConE11S98  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
EXTERN  S16 ccConE11S100  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
EXTERN  S16 ccConE14S35  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
EXTERN  S16 ccConE62S10  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
EXTERN  S16 ccConE04S51  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
EXTERN  S16 ccConE24S00  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
EXTERN  S16 ccConE11S24  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
EXTERN  S16 ccConE14S24  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
EXTERN  S16 ccConE04S24  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
EXTERN  S16 ccConE23SBUF ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
EXTERN  S16 ccConE23S25  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
EXTERN  S16 ccConE14S25  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
EXTERN  S16 ccConE04S25  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
EXTERN  S16 ccConE11S26  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
EXTERN  S16 ccConE14S26  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
EXTERN  S16 ccConE00S27  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
EXTERN  S16 ccConE14S27  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2)); 
EXTERN  S16 ccConE04S27  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2)); 
EXTERN  S16 ccConE26S00  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
EXTERN  S16 ccConE26S09  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
EXTERN  S16 ccConE26SBUF ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
EXTERN  S16 ccConE11S29  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
EXTERN  S16 ccConE14S29  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
EXTERN  S16 ccConE14S16  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
EXTERN  S16 ccConE04S16  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
EXTERN  S16 ccConE62S30  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
EXTERN  S16 ccConE14S30  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
EXTERN  S16 ccConE04S30  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
EXTERN  S16 ccConE11S92  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
EXTERN  S16 ccConE14S92  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
EXTERN  S16 ccConE61S09  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
EXTERN  S16 ccConE23S01  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
EXTERN  S16 ccConE07S09 ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
EXTERN  S16 ccConE60S41  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
EXTERN  S16 ccConE07SXXBuf  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
EXTERN  S16 ccConE07SBUF    ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
EXTERN  S16 ccConE07S30     ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
EXTERN  S16 ccConE07S16  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
PUBLIC  S16 ccConE39S49  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
PUBLIC  S16 ccHandleContErr ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
PUBLIC S16 ccConE38S39  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                            PTR event2));
PUBLIC S16 ccUnexpEvent ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                 PTR event2));
PUBLIC S16 ccIgnoreEvent ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                 PTR event2));

PUBLIC S16 ccConE29SXX  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                            PTR event2));
/* euysal - 03/17/2005 - CAMEL Support */
PUBLIC S16 ccConE75SXX  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                            PTR event2));
/*euysal - 04/21/2005 - Support for O_Answer request */
PUBLIC S16 ccConE37S95  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                              PTR event2));
PUBLIC S16 ccConE39S95  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                              PTR event2));
EXTERN S16 ccConE40S95  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                              PTR event2));
PUBLIC S16 ccConE71S95  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                              PTR event2));
PUBLIC  S16 ccConE02S96  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
PUBLIC  S16 ccConE03S96  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
PUBLIC  S16 ccConE04S96  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
PUBLIC  S16 ccConE14S96  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
PUBLIC  S16 ccConE23S96  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
/* ACC */
PUBLIC S16 ccConE03S99  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                            PTR event2));
PUBLIC S16 ccConE39S99  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                            PTR event2));
PUBLIC S16 ccConE52S99  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                            PTR event2));
PUBLIC S16 ccConE71S99  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                            PTR event2));
/* euysal - 07/06/2005 - ACC Support */
PUBLIC S16 ccConE71S44  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                            PTR event2));
/* ACC End */

/* Feature 1789 - German ISUP, Toiw2 Timer Expiry */
EXTERN  S16 ccConE74S09  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                              PTR event2));

/* FID 15174.0 Support SIP ISUP Overlap Receiving and Sending */
EXTERN  S16 ccConE92S09  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                              PTR event2));

/* Delayed Release Process */
PUBLIC  S16 ccConE04S97  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
PUBLIC  S16 ccConE39S09  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));

/* FID-2112 SIP UPDATE + */
PUBLIC  S16 ccConE11SAckOrPrack  ARGS ((CcConCb *con, PTR event,
                                        AinBCM_t *bcm, PTR event2));
PUBLIC  S16 ccConE83S09           ARGS ((CcConCb *con, PTR event,
                                        AinBCM_t *bcm, PTR event2));
/* FID-2112 SIP UPDATE - */
/* FID-2575: LMSD + */
PUBLIC  S16 ccConE11S136  ARGS ((CcConCb *con, PTR event,
                                 AinBCM_t *bcm, PTR event2));
/* FID-2575: LMSD - */
/* FID-14715: Support INAP Operations for FP/PP/VPN with OSP + */
PUBLIC  S16 ccConE11S137  ARGS ((CcConCb *con, PTR event,
                                 AinBCM_t *bcm, PTR event2));
PUBLIC  S16 ccConE40S11  ARGS ((CcConCb *con, PTR event,
                                 AinBCM_t *bcm, PTR event2));
PUBLIC S16 ccDisconnectOgLeg     ARGS ((CcConCb *con));
/* FID-14715: Support INAP Operations for FP/PP/VPN with OSP - */
/* CRBT General */
/* Disconnect Call */
PUBLIC S16 ccConE52S123 ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));
/* CCE_CONTINUE from FIC */
PUBLIC S16 ccConE39S123 ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));
PUBLIC S16 ccConE39S124 ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));
/* Analyze Route */
PUBLIC S16 ccConE37S123 ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));
PUBLIC S16 ccProcessAR  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));
PUBLIC S16 ccConE60S95  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));
PUBLIC S16 ccProcessSwapRequest  ARGS ((CcConCb *con1, CcConCb *con2));
PUBLIC S16 ccProcessSwapType     ARGS ((CcConCb *con));
PUBLIC S16 ccProcessMoveSendRelease ARGS ((CcConCb *con1, CcConCb *con2));
PUBLIC S16 ccProcessMoveRequest  ARGS ((CcConCb *con1, CcConCb *con2));
PUBLIC Bool ccFindSwitchingReqForAssocCalls  ARGS ((CcConCb *con));

/* Handle MgctTxnCfm for SwapCall */
PUBLIC S16 ccConE11S121 ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));
/* Handle MgctTxnCfm for MoveCall */
PUBLIC S16 ccConE11S122 ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));
/* CCE_CAC */
PUBLIC S16 ccConE77S00  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));
/* CCE_SWAPCALL */
PUBLIC S16 ccConE78S95  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));
PUBLIC S16 ccConE78S124 ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));
/* CCE_MOVECALL */
PUBLIC S16 ccConE79S03  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));
PUBLIC S16 ccConE79S99  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));
PUBLIC S16 ccConE79S123 ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));
/* CCE_INT_SWAPCALL */
PUBLIC S16 ccConE80SXX  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));
PUBLIC S16 ccConE80SBUF ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));
/* CCE_INT_MOVECALL */
PUBLIC S16 ccConE81S09  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));
PUBLIC S16 ccConE81SBUF ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));
/* CCE_PREANMTMREXP */
PUBLIC S16 ccHandleE82Exp ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));
PUBLIC S16 ccConE82SBUF   ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                               PTR event2));

PUBLIC S16 ccConE84S37    ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));

PUBLIC S16 ccConE11S134   ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));

PUBLIC S16 ccConE36S134   ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));

PUBLIC S16 ccConE85S32    ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));

PUBLIC S16 ccConE85SBUF    ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));

PUBLIC S16 ccConE86S32    ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));
 
PUBLIC S16 ccConE11S135   ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));

PUBLIC S16 ccConE36S135   ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));

PUBLIC S16 ccConE62S32    ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));

PUBLIC S16 ccConE03S13X   ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));

EXTERN S16 ccBldAndSendTstLnSig (CcConCb *con, U32 sig, State state, U8 command);
/* bug64647 */                  
EXTERN  S16 ccConE62S58  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
                                PTR event2));

/* FID 14234.0 support RBWF for UK */
PUBLIC S16 ccConE34S00   ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm, PTR event2));
PUBLIC S16 ccConE87S37   ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm, PTR event2));
PUBLIC S16 ccConE87S44   ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm, PTR event2));
PUBLIC S16 ccConE87S124  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm, PTR event2));
PUBLIC S16 ccConE39S140  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm, PTR event2));
PUBLIC S16 ccConE87S140  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm, PTR event2));
PUBLIC S16 ccConE88S140  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm, PTR event2));
PUBLIC S16 ccConE04S140  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm, PTR event2));
/* End of FID 14234.0 */

/* FID 15045.1 + */
PUBLIC S16 ccConE90S10  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm, PTR event2));
PUBLIC S16 ccConE90SBUF ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm, PTR event2));
PUBLIC S16 ccConE91S89  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm, PTR event2));
PUBLIC S16 ccConE91S90  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm, PTR event2));
PUBLIC S16 ccConE91SBUF ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm, PTR event2));
PUBLIC S16 ccConE91SXX  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm, PTR event2));
/* FID 15045.1 - */
/* FID15319.0 + */
PUBLIC S16 ccConE01S148 ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm, PTR event2));
PUBLIC S16 ccConE03S147 ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm, PTR event2));
PUBLIC S16 ccConE03S148 ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm, PTR event2));
PUBLIC S16 ccConE04S147 ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm, PTR event2));
PUBLIC S16 ccConE11S148 ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm, PTR event2));
PUBLIC S16 ccConE14S147 ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm, PTR event2));
PUBLIC S16 ccConE14S148 ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm, PTR event2));
PUBLIC S16 ccConE23S147 ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm, PTR event2));
PUBLIC S16 ccConE62S148 ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm, PTR event2));
/* FID15319.0 - */

/* FID15310.1 + */
PUBLIC S16 ccConE94S00   ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm, PTR event2));
PUBLIC S16 ccConE95S00   ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm, PTR event2));
/* FID15310.1 - */

/* FID 14457.0 + */
PUBLIC S16 ccConE96S37  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm, PTR event2));
PUBLIC S16 ccConE04S86  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm, PTR event2));
PUBLIC S16 ccConE96S86  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm, PTR event2));
PUBLIC S16 ccConE04S123 ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm, PTR event2));
PUBLIC S16 ccConE96S123 ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm, PTR event2));
PUBLIC S16 ccConE37S89  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm, PTR event2));

PUBLIC S16 ccConE03S150 ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm, PTR event2));
PUBLIC S16 ccConE04S150 ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm, PTR event2));
PUBLIC S16 ccConE14S150 ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm, PTR event2));
PUBLIC S16 ccConE97S150 ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm, PTR event2));
/* FID 14457.0 - */

/* BUG: 88025 + */
PUBLIC S16 ccConE98S09  ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm, PTR event2));
PUBLIC S16 ccConE98SBUF ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm, PTR event2));
/* BUG: 88025 - */

/* + FID 15801.0 */
PUBLIC S16 ccConE37S149 ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
			      PTR event2));
PUBLIC S16 ccConE39S149 ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
			      PTR event2));
PUBLIC S16 ccConE47S149 ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
			      PTR event2));
PUBLIC S16 ccConE52S149 ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
			      PTR event2));
PUBLIC S16 ccConE14S149 ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm,
			      PTR event2));
/* - FID 15801.0 */

/* FID 16970.0 + */
EXTERN S16 ccConE04S151 ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm, PTR event2));
EXTERN S16 ccConE04S152 ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm, PTR event2));
EXTERN S16 ccConE30S151 ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm, PTR event2));
/* FID 16970.0 - */

/* bug86942 */
PUBLIC S16 ccConE14S68   ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm, PTR event2));

/* FID 17174 Annc Support via External MRF */
PUBLIC S16 ccConE01S155   ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm, PTR event2));
PUBLIC S16 ccConE03S155   ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm, PTR event2));
PUBLIC S16 ccConE04S155   ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm, PTR event2));
/* FID 17174 Annc Support via External MRF */

/* FID 17555.0 + Kavon */
PUBLIC S16 ccConE02S156   ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm, PTR event2));
PUBLIC S16 ccConE03S156   ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm, PTR event2));
PUBLIC S16 ccConE04S156   ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm, PTR event2));
PUBLIC S16 ccConE11S156   ARGS ((CcConCb *con, PTR event, AinBCM_t *bcm, PTR event2));
/* FID 17555.0 - */
/*
*
*      Fun:   ccBufE01SXX
*
*      Desc:  Connection state function
*             event - ANSWER
*             state - CCS_AWTRELCFM_KEEP_CON, CCS_AWTSWTCFMFORCC6CC11
*
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes:
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC  S16 ccBufE01SXX
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  TRC3(ccBufE01SXX)

  /* Store Connect confirm event */
  /* Bug#  19408 */
  DP("ccBufE01SXX : %ld\n", con->icSuConnId);

  ccBufferEventToQueue(con, bcm, event, CCE_CONCFM, sizeof(CcConEvnt), 0,
                       NULLP, 0);

  RETVALUE(ROK);
} /* ccBufE01SXX */

/*
*
*      Fun:   ccConE11S82 
*
*      Desc:  Connection state function 
*             event - Switching Connect Confirm
*             state - CCS_AWTANNCMPLT_CTR
*
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes:
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC  S16 ccConE11S82
(
CcConCb *con,                    /* connection */ 
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  MgctTxnCfm          *txnCfm  = NULLP;
  S16                  ret     = RFAILED;

  CCDP(AIN_DBG_LEVEL_1, "ccConE11S82:%ld, %ld\n", con->icSuConnId,
                        con->ogSuConnId);
  txnCfm   = (MgctTxnCfm*) event;
  ret = ccHandleMgResponse(con, txnCfm);
  if (ret == RFAILED) /* CPDI_FIX */
  {
    CCLOGERR( "ERROR: Response Handling FAILED\n");
    cleanUpMgQ(con);
    ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
    RETVALUE(RFAILED);
  }
  switch(con->relPend)
  {
    case PALYINGANNC:
      /* This is a Connect_To_Resource Case, no action ... */
      break;
    case PALYEDANNC:
      /* Announcement played, it's a response to remove rsc, we
	 should send CTR_Clear to FIC now */
        CC_STATE_CHANGE(con, CCS_ANSWERED);
      /* Fill ClearCause with ClcUserAbandon */
        con->ainInfo.ClearCause = ClcNormal; /* MCAO_08_21 */
        ccProcessCTRClear(&(con->icBcm));
        ccProcessCcChangeforX2SIG(con); /* FID 15083.0 */
      break;
    default:
      /* This is an error case, or a Continue case */
      ABORT_DEBUG;
      break;
  }
  RETVALUE(ROK);
}
/*
*
*      Fun:   ccConE63S00 
*
*      Desc:  Connection state function 
*             event - CCE_STA_CRM
*             state - Null
*
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes:
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC  S16 ccConE63S00
(
CcConCb *con,                    /* connection */ 
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
   CcStaEvnt     *staEvnt = NULLP;

   CCDP(AIN_DBG_LEVEL_1, "ccConE63S00:%lx, %lx\n", con->icSuConnId, con->ogSuConnId);

   /* Bug 45275 + */
   con->cotBlock.contType = CC_CRM_NOCONT;

   staEvnt = (CcStaEvnt *) event;

   /* Bug 45275 + */
   if((staEvnt->siStaEvnt.natConInd.eh.pres) &&
      (staEvnt->siStaEvnt.natConInd.contChkInd.pres))
   {
      if((staEvnt->siStaEvnt.natConInd.contChkInd.val == CONTCHK_REQ) ||
         (staEvnt->siStaEvnt.natConInd.contChkInd.val == CONTCHK_PREV))
      {
         con->cotBlock.contType = CC_CRM_INITIATED;
      }
   }

   ccStartConTmr(TMR_SETUP, con, (PTR)&ccCp.genCfg);
   CC_STATE_CHANGE(con, CCS_AWTROUTERSC);
   ccAllocateResource(con, (CcConEvnt *)NULLP, CC_INCOMING, RMT_RESERVE);
   RETVALUE(ROK);
}

/*
*
*      Fun:   ccConE14S57
*
*      Desc:  Connection state function 
*             event - Setup timer expiry
*             state - CCS_AWTMGCTCFMFORMODIFY0
*
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes:
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC  S16 ccConE14S57
(
CcConCb *con,                    /* connection */ 
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  CCLOGERR("Error: ccConE14S57: Setup Timer expired in state %d, for icSu %ld,"
           "ogSu %ld\n", con->state, con->icSuConnId, con->ogSuConnId);
  cleanUpMgQ(con);
  ccCleanUpMGCtx(con);
  ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
  RETVALUE(ROK);
} /* ccConE14S57 */

/*
*
*      Fun:   ccConE14S58 
*
*      Desc:  Connection state function 
*             event - Setup timer expiry
*             state - CCS_AWTMGCTCFMFORMODIFY1
*
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: 
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC  S16 ccConE14S58
(
CcConCb *con,                    /* connection */ 
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  CCLOGERR("Error: ccConE14S58: Setup Timer expired in state %d, for icSu %ld,"
           "ogSu %ld\n", con->state, con->icSuConnId, con->ogSuConnId);
  cleanUpMgQ(con);
  ccCleanUpMGCtx(con);
  ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
  RETVALUE(ROK);
} /* ccConE14S58 */

/* euysal - 06/01/2005 - ACC Support */
#if 1
/*
*
*      Fun:   ccConE14S97 
*
*      Desc:  Connection state function 
*             event - Setup timer expiry
*             state - CCS_AWTMGCTCFMFORMODIFY1_CTR
*
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: 
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC  S16 ccConE14S97
(
CcConCb *con,                    /* connection */ 
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  CCLOGERR("Error: ccConE14S97: Setup Timer expired in state %d, for icSu %ld,"
           "ogSu %ld\n", con->state, con->icSuConnId, con->ogSuConnId);
  cleanUpMgQ(con);
  ccCleanUpMGCtx(con);
  ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
  RETVALUE(ROK);
} /* ccConE14S97 */
#endif

/*
*
*      Fun:   ccConE14S72 
*
*      Desc:  Connection state function 
*             event - Setup timer expiry
*             state - CCS_AWT_PERMSIG_TREATMENT_CFM 
*
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes:
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC  S16 ccConE14S72
(
CcConCb *con,                    /* connection */ 
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  CCLOGERR("ccConE14S72 : Setup timer expired in state = %d, icSu = 0x%lx\n",
           con->state, con->icSuConnId);
  ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
  ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
  RETVALUE(ROK);
}


/*
*
*      Fun:   ccConE66SXX 
*
*      Desc:  Connection state function 
*             event - CCE_CLOSEFROMFIC
*             state - CCS_XXXX
*
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes:
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC  S16 ccConE66SXX
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  /*CID 15596*/
  if ((con == NULLP) || (bcm == NULLP))
  {
    CCLOGERR("**ERROR*** bcm or con is NULLP: con: %lx, bcm:%ld\n",
             (U32)con, event);
    RETVALUE(RFAILED);
  }
  /* Feature 1789 - German ISUP */
  /* stop the Toiw2 Timer if running */
  ccStopConTmr(con, CC_TMR_TOIW2);


  CCDP(AIN_DBG_LEVEL_1, "ccConE66SXX:icSu:%ld, ogSu:%ld, bcmType:%d,"
       " transId:%ld\n", con->icSuConnId, con->ogSuConnId,
       bcm->bcmType, bcm->svcHeader.transId);
  if (bcm->bcmType == BtOBcm)
  {
    if (con->icBcm.svcHeader.transId)
    cmHashListDelete(&ccicBCMInsTbl, (PTR)&(con->icBcm));

    ccDropIcBcmMember(con);
    con->icBcm.svcHeader.transId = 0;
    RETVALUE(ROK);
  }
  else if (bcm->bcmType == BtTBcm)
  {
    if (con->ogBcm.svcHeader.transId)
      cmHashListDelete(&ccogBCMInsTbl, (PTR)&(con->ogBcm));

    ccDropOgBcmMember(con);
    con->ogBcm.svcHeader.transId = 0;
    RETVALUE(ROK);
  }
  else
  {
    CCLOGERR("***Error:Invalid bcmType:%d, transId:%ld icSu:%ld,"
             " ogSu:%ld\n", bcm->bcmType, bcm->svcHeader.transId,
             con->icSuConnId, con->ogSuConnId);
    RETVALUE(RFAILED);
  }
}
/*
*
*      Fun:  ccConE48SBUF
*
*      Desc:  Connection state function
*             event - CCE_TSUSPEXP.
*             state - CCS_AWT_QRYRSP_ONSWITCH
*
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes:
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC  S16 ccConE48SBUF
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  CCDP(AIN_DBG_LEVEL_1, "ccConE48SBUF:icSu:%ld, ogSu:%ld\n",
       con->icSuConnId, con->ogSuConnId);

  ccBufferEventToQueue(con, bcm, event, CCE_TSUSPEXP, 0, con->ogSuConnId,
                       NULLP, 0);
  RETVALUE(ROK);
}

/*
*
*      Fun:   ccConE50SBUF
*
*      Desc:  Connection state function
*             event - T_NO_ANSWER.
*             state - CCS_XXXX
*
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes:
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC  S16 ccConE50SBUF
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  CCDP(AIN_DBG_LEVEL_1, "ccConE50SBUF:icSu:%ld, ogSu:%ld\n",
       con->icSuConnId, con->ogSuConnId);

/* euysal - 03/08/2005 - BUG:36186 We need to specify the connid explicity 
otherwise while trying to delete the event from buffer, if we cannot find an connid,
we look at the direction to search for the connid and the direction may have changed
since we buffered the event and we may end up searching with wrong connid */ 
#if 0
  ccBufferEventToQueue(con, bcm, event, CCE_TNOANSEXP, 0, 0,
                       NULLP, 0);
#else
  ccBufferEventToQueue(con, bcm, event, CCE_TNOANSEXP, 0, con->ogSuConnId,
                       NULLP, 0);
#endif  
  RETVALUE(ROK);
}

/* 
*  
*      Fun:   ccConE51SBUF
*  
*      Desc:  Connection state function
*             event - CCE_ONOANSEXP.
*             state - CCS_XXXX 
* 
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes:
*   
*      File:  cc_bdy2_2.c
*   
*/  
PUBLIC  S16 ccConE51SBUF
(   
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)   
{   
  CCDP(AIN_DBG_LEVEL_1, "ccConE51SBUF:icSu:%ld, ogSu:%ld\n", 
       con->icSuConnId, con->ogSuConnId);

/* euysal - 03/08/2005 - BUG:36186 We need to specify the connid explicity 
otherwise while trying to delete the event from buffer, if we cannot find an connid,
we look at the direction to search for the connid and the direction may have changed
since we buffered the event and we may end up searching with wrong connid */ 
#if 0
  ccBufferEventToQueue(con, bcm, event, CCE_ONOANSEXP, 0, 0, NULLP, 0);
#else
  ccBufferEventToQueue(con, bcm, event, CCE_ONOANSEXP, 0, con->icSuConnId, NULLP, 0);
#endif  
  RETVALUE(ROK);
}   


/*
*
*      Fun:   ccConE11S14
*
*      Desc:  Connection state function
*             event - MGI TxnConfirm
*             state - CCS_AWT_MGI_XFERQRYHOLD
*
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes:
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC  S16 ccConE11S14
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  S16            ret = RFAILED;
  MgctTxnCfm    *txnCfm = NULLP;
#ifdef ZC
  ZcUpdParam upd;
#endif /* ZC */
  Buffer        *uBuf           = NULLP;
  CcCnStEvnt      ogEvnt;
  U8             tmpDirection = DIR_UNKNOWN;
  /* Bug 62848 */
  Bool           isXferRcvd       = FALSE;
  Bool           isXferRspRcvd    = FALSE;
  Bool           isQryRcvd        = FALSE;
  Bool           isQryRspRcvd     = FALSE;
  Bool           isUpdateRcvd     = FALSE;
  Bool           isUpdateRspRcvd  = FALSE;
  Bool           repMgCtx      = FALSE;

  txnCfm   = (MgctTxnCfm*) event;

  /* bug 91532 */
  cmMemset((U8 *)&ogEvnt, 0, sizeof(ogEvnt));
  /* Bug 62783 */
  tmpDirection = con->direction;
  con->direction = CC_GET_EVNTHOLD_DIR(con->directionHeld);

  ret = ccHandleMgResponse(con, txnCfm);
  if(ret != ROK)
  {
    /* Bug# 49781 - Seeni - modifed the existing code below */
    if ((con->mgBlock) &&
        ((con->mgBlock->megacoError == TELICO_ERROR_INSUFFICIENT_RESOURCES) ||
         (con->mgBlock->megacoError == TELICO_ERROR_UNSUPPORTED_MEDIA_TYPE)) &&
        ((con->relPend == TRY_SUBADD_ON_ERR_IC) ||
         (con->relPend == TRY_SUBADD_ON_ERR_OG)))
    {
      CCDP(AIN_DBG_LEVEL_1, "MEGACO error - %d\n", con->mgBlock->megacoError);

      /* Reject Xfer */
      if (con->direction == INCTOOUT)
      {
        cmFreeSdp(ccInit.region, ccInit.pool, &con->icIpParam.locDesc);

        /* Bug 85824 */
        if ((con->faxCb.faxTrigger != 0) && CC_IS_CALL_MULTI_MG(con))
        {
          CC_SET_ZERO_BEARER_IP_PORT(&con->icIpParam.locBearerIpPort);
          if (con->icAdjIpParam)
            CC_SET_ZERO_BEARER_IP_PORT(&con->icAdjIpParam->locBearerIpPort);
          if (con->icLiAdjIpParam)
            CC_SET_ZERO_BEARER_IP_PORT(&con->icLiAdjIpParam->locBearerIpPort);
          if (con->ogLiAdjIpParam)
            CC_SET_ZERO_BEARER_IP_PORT(&con->ogLiAdjIpParam->locBearerIpPort);
        }

        if  (con->mgBlock->megacoError == TELICO_ERROR_UNSUPPORTED_MEDIA_TYPE)
        {
          if (CC_OFFANS_METHOD_USED(con) == CC_OFFANS_TYPE_UPDATE)
            ccSendUpdateRsp(&con->icBcm, PRSNT_NODEF, SIPCAUSE_415_MEDIA);
          else
            ccSendXferRsp(&con->icBcm, PRSNT_NODEF, SIPCAUSE_415_MEDIA);
        }
        else
        {
          if (CC_OFFANS_METHOD_USED(con) == CC_OFFANS_TYPE_UPDATE)
            ccSendUpdateRsp(&con->icBcm, NOTPRSNT, 0);
          else
            ccSendXferRsp(&con->icBcm, NOTPRSNT, 0);
        }
      }
      else
      {
        cmFreeSdp(ccInit.region, ccInit.pool, &con->ogIpParam.locDesc);

        /* Bug 85824 */
        if ((con->faxCb.faxTrigger != 0) && CC_IS_CALL_MULTI_MG(con))
        {
          CC_SET_ZERO_BEARER_IP_PORT(&con->ogIpParam.locBearerIpPort);
          if (con->ogAdjIpParam)
            CC_SET_ZERO_BEARER_IP_PORT(&con->ogAdjIpParam->locBearerIpPort);
          if (con->ogLiAdjIpParam)
            CC_SET_ZERO_BEARER_IP_PORT(&con->ogLiAdjIpParam->locBearerIpPort);
          if (con->icLiAdjIpParam)
            CC_SET_ZERO_BEARER_IP_PORT(&con->icLiAdjIpParam->locBearerIpPort);
        }

        if  (con->mgBlock->megacoError == TELICO_ERROR_UNSUPPORTED_MEDIA_TYPE)
        {
          if (CC_OFFANS_METHOD_USED(con) == CC_OFFANS_TYPE_UPDATE)
            ccSendUpdateRsp(&con->ogBcm, PRSNT_NODEF, SIPCAUSE_415_MEDIA);
          else
            ccSendXferRsp(&con->ogBcm, PRSNT_NODEF, SIPCAUSE_415_MEDIA);
        }
        else
        {
          if (CC_OFFANS_METHOD_USED(con) == CC_OFFANS_TYPE_UPDATE)
            ccSendUpdateRsp(&con->ogBcm, NOTPRSNT, 0);
          else
            ccSendXferRsp(&con->ogBcm, NOTPRSNT, 0);
        }
      }  /* end of BUG 49781 */

      con->relPend = 0;

      /* Back to Answered state */
      switch(con->lastState)
      {
      case CCS_AWTPAUSETIMEREXPIRY:
          ccStartConTmr(CC_PAUSETIMER, con, (PTR)&ccCp.genCfg);
          /* Fall through */
      case CCS_AWTANS:
      case CCS_AWTMGCTNOTIFY0:
      case CCS_AWT_RESUME:
          CC_STATE_CHANGE(con, con->lastState);
          break;
      default:
          CC_STATE_CHANGE(con, CCS_ANSWERED);
          ccProcessCcChangeforX2SIG(con); /* FID 15083.0 */
          break;
      }

      RETVALUE(ROK);
    }

    CCLOGERR("%s: ccHandleMgResponse : failed\n", __FUNCTION__);
    cleanUpMgQ(con);
    ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
    RETVALUE(ROK);
  }

  con->relPend = 0;

  /* Map the event in ccEvntHold */
  switch(con->lastState)
  {
  case CCS_AWTPAUSETIMEREXPIRY:
      ccStartConTmr(CC_PAUSETIMER, con, (PTR)&ccCp.genCfg);
      /* Fall through */
  case CCS_AWTMGCTNOTIFY0:
  case CCS_AWT_RESUME:
      CC_STATE_CHANGE(con, con->lastState);
      break;
  case CCS_AWTANS:
      CC_STATE_CHANGE(con, con->lastState);
      ccProcessCcChangeforX2SIG(con); /* bug 84005 */
      break;
  case CCS_AWT_PRECONDITION:  /* FID15068 */
      CC_STATE_CHANGE(con, con->lastState);
      break;
  case CCS_AWT_MRFRSP: /* BUG:93414 */
      CC_STATE_CHANGE(con, con->lastState);
      break;
  default:
      CC_STATE_CHANGE(con, CCS_ANSWERED);
      ccProcessCcChangeforX2SIG(con); /* FID 15083.0 */
      break;
  }

  /* Bug 62848 */
  isXferRcvd = ccIsXferRcvd(CC_GET_PROTOCOL(con->icProtType),
                            CC_GET_PROTOCOL(con->ogProtType),
                            con->evntTypeHeld, con->direction);
  isQryRcvd = ccIsXferQryRcvd(CC_GET_PROTOCOL(con->icProtType),
                              CC_GET_PROTOCOL(con->ogProtType),
                              con->evntTypeHeld, con->direction);
  isUpdateRcvd = ccIsUpdateRcvd(CC_GET_PROTOCOL(con->icProtType),
                              CC_GET_PROTOCOL(con->ogProtType),
                              con->evntTypeHeld, con->direction);
  isXferRspRcvd = ccIsXferRspRcvd(CC_GET_PROTOCOL(con->icProtType),
                                  CC_GET_PROTOCOL(con->ogProtType),
                                  con->evntTypeHeld, con->direction);
  isUpdateRspRcvd = ccIsUpdateRspRcvd(CC_GET_PROTOCOL(con->icProtType),
                                      CC_GET_PROTOCOL(con->ogProtType),
                                      con->evntTypeHeld, con->direction);
  isQryRspRcvd = ccIsQueryRspRcvd(CC_GET_PROTOCOL(con->icProtType),
                                  CC_GET_PROTOCOL(con->ogProtType),
                                  con->evntTypeHeld, con->direction);

  /* FID 15261.0 + */
  if (CC_IS_CALL_MULTI_MG(con) || CC_IS_STEROMONO_CALL(con))
  {
    if ((!CC_OPERATING_LAST_MG(con)) && ((con->ccmCb.flags & CC_IW_TDM) == 0))
    {
      Bool locally_handled = FALSE;

      /* See if this message is just being handled locally */
      if(!CC_IS_MG_NEEDED_IN_IP_CALL(con))
      {
        if (CC_IS_STEROMONO_CALL(con))
        {
           /* CC_IS_STEROMONO_CALL when Xfer/Update received need to
            * MODIFY three terminations!!!*/
        }
        else
          locally_handled = TRUE;
      }

      if (con->direction == INCTOOUT)
      {
          if ((con->ccmCb.ccn == CCN_5) && (con->ccmCb.state == CC_INGRESS_MG))
            con->ccmCb.state = CC_Y_MG;
          else
            con->ccmCb.state = CC_EGRESS_MG;
      }
      else
      {
          if ((con->ccmCb.ccn == CCN_5) && (con->ccmCb.state == CC_EGRESS_MG))
            con->ccmCb.state = CC_Y_MG;
          else
            con->ccmCb.state = CC_INGRESS_MG;
      }

      con->icEvntType = con->evntTypeHeld;

      if (locally_handled == TRUE)
      {
        ret = ROK;
      }
      else if (isXferRcvd)
      {
        ccHandleXfer(con, CC_GET_PROTOCOL(con->icProtType),
                     CC_GET_PROTOCOL(con->ogProtType), con->direction,
                     (PTR)con->ccEvntHold);
        ret = ROKIGNORE;
      }
      else if (isXferRspRcvd)
      {
        ccProcessXferRsp(con, CC_GET_PROTOCOL(con->icProtType),
                         CC_GET_PROTOCOL(con->ogProtType), con->ccEvntHold);
        ret = ROKIGNORE;
      }
      else if (isQryRcvd)
      {
        ret = ccHandleQryXferHold(con, CC_GET_PROTOCOL(con->icProtType),
                                  CC_GET_PROTOCOL(con->ogProtType), con->direction,
                                  (PTR)con->ccEvntHold);
      }
      else if (isQryRspRcvd)
      {
        ret = ccProcessQryRsp(con, CC_GET_PROTOCOL(con->icProtType),
                              CC_GET_PROTOCOL(con->ogProtType), con->ccEvntHold);
      }
      else if (isUpdateRcvd)
      {
        if (((con->direction == INCTOOUT) &&
              ((con->icBcm.pic >= PicOActive) ||
               (CCT_IS_100REL_SUPPORTED(con->ogSipMethodSupMask) &&
                CCT_IS_UPDATE_SUPPORTED(con->ogSipMethodSupMask)))) ||
             ((con->direction == OUTTOINC) &&
              ((con->ogBcm.pic >= PicTActive) ||
               (CCT_IS_100REL_SUPPORTED(con->icSipMethodSupMask) &&
                CCT_IS_UPDATE_SUPPORTED(con->icSipMethodSupMask)))))
        {
          ccProcessUpdate(con, CC_GET_PROTOCOL(con->icProtType),
                          CC_GET_PROTOCOL(con->ogProtType), con->ccEvntHold);
          ret = ROKIGNORE;
        }
        else
        {
          ret = ROK;
        }
      }
      else if (isUpdateRspRcvd)
      {
        ccProcessUpdateRsp(con, CC_GET_PROTOCOL(con->icProtType),
                           CC_GET_PROTOCOL(con->ogProtType), con->ccEvntHold);
        ret = ROKIGNORE;
      }

      if (ret != ROK)
        RETVALUE(ROK);
    }
    else
    {
      /* For FID17160 case, do not set the flag and since when goes
       * here, this is the final mg operation */
      if (((con->ccmCb.flags & CC_IW_TDM) == 0) && (!CC_IS_STEROMONO_CALL(con)))
      {
        if (con->direction == INCTOOUT)
        {
          /* FID 16458.0 */
          if (!ccIsIPResource(&con->ogBcm) || CC_GET_PROTOCOL(con->ogProtType)==CC_EXT_BICC)
            con->ccmCb.flags |= CC_IW_TDM;
        }
        else
        {
          /* FID 16458.0 */
          if (!ccIsIPResource(&con->icBcm) || CC_GET_PROTOCOL(con->icProtType)==CC_EXT_BICC)
            con->ccmCb.flags |= CC_IW_TDM;
        }
      }

      if (con->ccmCb.flags & CC_IW_TDM)
      {
        if (con->direction == INCTOOUT)
          con->direction = OUTTOINC;
        else
          con->direction = INCTOOUT;

        if (!CC_OPERATING_LAST_MG(con))
        {
          if (con->direction == INCTOOUT)
          {
            if ((con->ccmCb.ccn == CCN_5) && (con->ccmCb.state == CC_INGRESS_MG))
              con->ccmCb.state = CC_Y_MG;
            else
              con->ccmCb.state = CC_EGRESS_MG;
          }
          else
          {
            if ((con->ccmCb.ccn == CCN_5) && (con->ccmCb.state == CC_EGRESS_MG))
              con->ccmCb.state = CC_Y_MG;
            else
              con->ccmCb.state = CC_INGRESS_MG;
          }

          if (isQryRcvd)
          {
            ret = ccProcessQryRsp(con, CC_GET_PROTOCOL(con->icProtType),
                                  CC_GET_PROTOCOL(con->ogProtType), con->ccEvntHold);
	  }
          else
          {
            ccProcessXferRsp1OnSwitchPassThru(con, CC_GET_PROTOCOL(con->icProtType),
                                 CC_GET_PROTOCOL(con->ogProtType), con->ccEvntHold);
            CC_STATE_CHANGE(con, CCS_AWT_MGI_XFERQRYHOLD);
            ret = ROKIGNORE;
          }
        }

        if (ret != ROK)
          RETVALUE(ROK);
        else
        {
          con->ccmCb.flags &= ~CC_IW_TDM;
          con->direction =CC_GET_EVNTHOLD_DIR(con->directionHeld);

          if (con->direction == INCTOOUT)
            con->ccmCb.state = CC_EGRESS_MG;
          else
            con->ccmCb.state = CC_INGRESS_MG;
        }
      }
    }
  }
  /* FID 15261.0 - */

  /* FID17160.0 */
  if (CC_IS_STEROMONO_CALL(con))
  {
     con->direction =CC_GET_EVNTHOLD_DIR(con->directionHeld);

     if (con->direction == INCTOOUT)
        con->ccmCb.state = CC_EGRESS_MG;
     else
        con->ccmCb.state = CC_INGRESS_MG;
  }
  /* Bug 62848 + */
  if ((CC_IS_CONTEXT_PRESENT(con)) && (con->mgPlayingTone) && 
      ((isXferRcvd) || (isQryRcvd)) && 
      (CC_IS_IC_LMSD_AI(con) || CC_IS_OG_LMSD_AI(con)))
  {
        repMgCtx = TRUE;
  }
  /* Bug 62848 - */

  /* FID-2593: Transcoding + */
  if (CC_IS_MG_NEEDED_IN_IP_CALL(con))
  {
    if (isXferRcvd || isUpdateRcvd)
    {
      if ((con->direction == OUTTOINC) && (con->icIpParam.locDesc))
      {
        ccCodecReorderPostCfm(&con->icBcm, con->icIpParam.locDesc);  /* FID-14209 */

        /* In case we get 491 from ingress SIP UAC, we need to re-transmit
         * same SDP again. So, save it!
         */
         cmCopySdp(ccInit.region, ccInit.pool, &(con->ccHeldSdpCBI),
                   con->icIpParam.locDesc);
      }
      /* FID-14209: Enhanced Transcoding + */
      else if ((con->direction == INCTOOUT) && (con->ogIpParam.locDesc))
      {
        ccCodecReorderPostCfm(&con->ogBcm, con->ogIpParam.locDesc);
      }
      /* FID-14209: Enhanced Transcoding - */
    }
  }
  /* FID-2593: Transcoding - */
  /* FID-2575: LMSD + */
  else if (!CC_IS_CONTEXT_PRESENT(con))
  {
    /* Bug 62848 */
    if (isXferRcvd || isQryRcvd)
    {
        /* BUG 71151 - Free leftover SDP */
        if ((con->direction == OUTTOINC) && (con->icIpParam.locDesc))
        {
          cmFreeSdp(ccInit.region, ccInit.pool, &con->icIpParam.locDesc);
        }
        else if ((con->direction == INCTOOUT) && (con->ogIpParam.locDesc))
        {                                               
           cmFreeSdp(ccInit.region, ccInit.pool, &con->ogIpParam.locDesc);
        }  
        /* We just deleted the context so we have to go on-net */
        CC_INC_XFER_COUNT(con);
        CC_STATE_CHANGE(con, CCS_AWT_OFFNET_XFERRSP);
        repMgCtx = TRUE;
    }
  }
  /* FID-2575: LMSD - */

  /*FID 16087.0 +*/
  if(con->faxCb.subFaxTrigger)
  {
    if(isXferRcvd)
    {
      ProtType protType;
      if(con->direction == INCTOOUT)
      {
        protType = CC_GET_PROTOCOL(con->ogProtType);
        if(protType == CC_SI || protType == CC_IN)
        {
          updateSdpByModemFaxEvnt(&con->icBcm, TRUE);
          con->faxCb.subFaxTrigger = 0;
        }else if(protType == CC_SIP || protType == CC_SIPT || protType == CC_BICC)
        {
          if(isModemFaxNeededForSdp(&con->ogBcm, FALSE))
          {
            updateSdpByModemFaxEvnt(&con->ogBcm, TRUE);
          }
        }
      }else
      {
        protType = CC_GET_PROTOCOL(con->icProtType);
        if(protType == CC_SI || protType == CC_IN)
        {
          updateSdpByModemFaxEvnt(&con->ogBcm, TRUE);
          con->faxCb.subFaxTrigger = 0;
        }else if(protType == CC_SIP || protType == CC_SIPT || protType == CC_BICC)
        {
          if(isModemFaxNeededForSdp(&con->icBcm, FALSE))
          {
            updateSdpByModemFaxEvnt(&con->icBcm, TRUE);
          }
        }
      }
    }else if(isXferRspRcvd && ccIsIPResource(&con->icBcm) && ccIsIPResource(&con->ogBcm))
    {
      if(con->direction == INCTOOUT)
      {
        updateSdpByModemFaxEvnt(&con->ogBcm, TRUE);
        con->faxCb.subFaxTrigger = 0;
      }else
      {
        updateSdpByModemFaxEvnt(&con->icBcm, TRUE);
        con->faxCb.subFaxTrigger = 0;
      }
    }
  }
  /*FID 16087.0 -*/

  /* Bug: 62848 - Replicate or delete MG context on standby based on suCtxId */
#ifdef ZC
  if (repMgCtx && CC_IS_OBCM_PIC_ACTIVE(con))
  {
      cmMemset((U8*) &upd, 0, sizeof(ZcUpdParam));
      /* Bug# 30194 */
      upd.featMsg.suConnId      = ccGetCorrecticSuConnId(con);
      upd.featMsg.ccAinUpdType  = CC_CREATE_MG_CTX;
      upd.featMsg.DlegId        = 0;
      upd.featMsg.suCtxId1      = (con->icBcm.mgCtx ? con->icBcm.mgCtx->suCtxId : 0); 
      upd.featMsg.suCtxId2      = (con->ogBcm.mgCtx ? con->ogBcm.mgCtx->suCtxId : 0);
      upd.featMsg.contextId1     = (con->icBcm.mgCtx ? con->icBcm.mgCtx->contextId : 0);
      upd.featMsg.contextId2     = (con->ogBcm.mgCtx ? con->ogBcm.mgCtx->contextId : 0);
      upd.featMsg.mgToneType    = con->mgToneType;
      upd.featMsg.mgPlayingTone = con->mgPlayingTone; 
      upd.featMsg.mgSpId1       = con->icBcm.mgSpId;
      upd.featMsg.mgSpId2       = con->ogBcm.mgSpId;
      upd.featMsg.ccn           = con->ccmCb.ccn;  
      CC_FILL_FEATMSG_FOR_CCN5(upd.featMsg, con); /* FID 15083.0 */
      zcRunTimeUpd(ZC_FEAT, CMPFTHA_UPD_REQ, upd);
      zcUpdPeer();
  }
#endif /* ZC */

  con->icEvnt = (CcAllSdus *) con->ccEvntHold;
  con->ogEvnt = (CcAllSdus *) &ogEvnt;
  /* BUG:93368 If we are receive the UPDATE during playing annoucement,
   * we need to ignore this message map since no ogLeg there */

  if (con->state == CCS_AWTMGCTNOTIFY0 || con->state == CCS_AWTPAUSETIMEREXPIRY)
  {
     if (isUpdateRcvd)
     {
           if(con->direction == INCTOOUT)
              ccSendUpdateRsp(&con->icBcm, NOTPRSNT, 0);
           else
              ccSendUpdateRsp(&con->ogBcm, NOTPRSNT, 0);
     }
     if (isXferRcvd)
     {
        if (CC_IS_CONTEXT_PRESENT(con))
        {
           if(con->direction == INCTOOUT)
              ccSendXferRsp(&con->icBcm, NOTPRSNT, 0);
           else
              ccSendXferRsp(&con->ogBcm, NOTPRSNT, 0);
        }

     }
     ccDropUBuf(&con->uBuf);
     con->direction = tmpDirection;
     RETVALUE(ROK);

  }

  ret = ccMapEvent(con, CCE_CNSTIND, con->evntTypeHeld /* bug#27547, tfrederi */);
  if (ret == CCIGNOREMAPPING)
  {
    ccDropUBuf(&con->uBuf);
    con->direction = tmpDirection;
    RETVALUE(ROK);
  }
  else if (ret != ROK)
  {
    ccStopConTmr(con, CC_TMR_CALLDTL);
    ccStopConTmr(con, TMR_SETUP);
    ccStartConTmr(TMR_RELEASE, con, (PTR)&ccCp.genCfg);

    /* Release all connections */
    ccDropUBuf(&con->uBuf);
    if (CC_IS_CONTEXT_PRESENT(con))
    {
      /* Blow off the switching and Release the Call */
      ccQSubBasedOnCcn(con);  /* FID 15261.0 */

      ccStopConTmr(con, CC_TMR_CALLDTL);
      ccStartConTmr(TMR_RELEASE, con, (PTR)&ccCp.genCfg);

      con->lastState = con->state;
      con->relPend = BOTHREL;

      CC_STATE_CHANGE(con, CCS_AWTSWTCFM_SENDBOTHREL);
#ifdef ZC      
      cmMemset((U8*) &upd, 0, sizeof(ZcUpdParam));
      upd.icSuConnId = con->icSuConnId;
      zcRunTimeUpd(ZC_RELCON_CB, CMPFTHA_UPD_REQ, upd);
      zcUpdPeer();
#endif  
      con->mgBlock->con = con;
      ret = ccSendMgiQueue(0, con->mgBlock);
      if (ret != ROK)
      {
            CCLOGERR( "ccUpdateMgQForSubRsc() failed \n");
            cleanUpMgQ(con);
      }
      else
      {
            con->direction = tmpDirection;
            RETVALUE(ROK);
      }
    }
    else
    {
            CCLOGERR( "ERROR: No Switching Context Found\n");
            ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
            con->direction = tmpDirection;
            RETVALUE(ROK);
    }
  }

  /* FID-2593: Transcoding + */
  if (isXferRspRcvd || isUpdateRspRcvd)
  {
    /* Before we send this response, reset offAns state */
    CC_OFFANS_STATE_CHANGE(con, CC_OFFANS_STATE_NULL,
                           CC_OFFANS_STATE_NULL,
                           CC_OFFANS_SUBSTATE_NULL,
                           CC_OFFANS_TYPE_INVALID);
  }
  /* FID-2593: Transcoding - */

  uBuf = con->uBuf;
  con->uBuf = NULLP;
  if (con->direction == INCTOOUT)
  {
        ccSendCnStReq(con, con->ogSapCb->suId, &con->ogSapCb->pst,
                      con->ogSapCb->spId, con->ogSpConnId, con->ogSuConnId,
                      con->ogProtType, con->ogEvntType, &ogEvnt, uBuf);
  }
  else
  {
        ccSendCnStReq(con, con->icSapCb->suId, &con->icSapCb->pst,
                      con->icSapCb->spId, con->icSpConnId, con->icSuConnId,
                      con->icProtType, con->ogEvntType, &ogEvnt, uBuf);
  }

  con->direction = tmpDirection;
  RETVALUE(ROK);
} /* ccConE11S14 */

/* 
 *
 *      Fun:   ccConE67SXX 
 *
 *      Desc:  Connection state function
 *             event - Kill Call Event from GCC itself.
 *             state - 
 *      Ret:   ROK     - successful,
 *             RFAILED - unsuccessful
 *  
 *      Notes: None.
 *  
 *      File:  cc_bdy2_2.c 
 *  
*/  
PUBLIC  S16 ccConE67SXX
(   
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)   
{
#ifdef ZC
  ZcUpdParam upd;  
#endif /* ZC */

  CCDP(AIN_DBG_LEVEL_1, "ccConE67SXX:%ld,%ld,%ld\n", con->icSuConnId,
                        con->ogSuConnId, (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0));

  /* Feature 1789 - German ISUP */
  /* stop the Toiw2 Timer if running */
  ccStopConTmr(con, CC_TMR_TOIW2);

#ifdef ZC
  if (CC_IS_OBCM_PIC_ACTIVE(con))
  {
    cmMemset((U8 *)&upd, 0, sizeof(ZcUpdParam));
    upd.featMsg.suConnId     = con->icSuConnId;
    upd.featMsg.ccAinUpdType = CC_KILL_CALL;
    zcRunTimeUpd(ZC_FEAT, CMPFTHA_UPD_REQ, upd);
    zcUpdPeer();
  }
#endif
  ccKillCallOnThisCon(con);
  RETVALUE(ROK);
}


/*
 *
 *      Fun:   ccConE67SFICRSP
 *
 *             event:   - CCE_KILLCALL
 *             state:   - 
 *      Ret:   ROK     - successful,
 *             RFAILED - unsuccessful
 *
 *      Notes: None.
 *
 *      File:  cc_bdy2_2.c
 *
 */
PUBLIC  S16 ccConE67SFICRSP
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  DP("ccConE67SFICRSP : %ld\n", con->icSuConnId);

  CC_STATE_CHANGE(con, CCS_AWT_FIC_RSP_TO_KILL);
  RETVALUE(ROK);
}


PUBLIC S16 ccConE67SBUF
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  DP("ccConE67SBUF : %ld\n", con->icSuConnId);

  ccBufferEventToQueue(con, bcm, event, CCE_KILLCALL, 0, 0, NULLP, 0);
  RETVALUE(ROK);
}


PUBLIC S16 ccConE67SMGI
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
#ifdef ZC
  ZcUpdParam upd;
#endif /* ZC */

  DP("ccConE67SMGI : %ld\n", con->icSuConnId);
  ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
  ccRestoreCallStateOfAssociatedCalls(&(con->ogBcm));

  CCDP(AIN_DBG_LEVEL_1, "ccConE67SMGI:%ld,%ld,%ld\n", con->icSuConnId,
                        con->ogSuConnId, (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0));
#ifdef ZC
  if (CC_IS_OBCM_PIC_ACTIVE(con))
  {
    cmMemset((U8 *)&upd, 0, sizeof(ZcUpdParam));
    upd.featMsg.suConnId     = con->icSuConnId;
    upd.featMsg.ccAinUpdType = CC_KILL_CALL;
    zcRunTimeUpd(ZC_FEAT, CMPFTHA_UPD_REQ, upd);
    zcUpdPeer();
  }
#endif
  ccKillCallOnThisCon(con);
  RETVALUE(ROK);
}


/* TRFO */
/*
*
*      Fun:   ccConE71SBUF
*
*      Desc:  Connection state function
*             event - Connect To Resource
*             state - Various
*
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes:
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC  S16 ccConE71SBUF
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
    DP("%s : 0x%lx\n", __FUNCTION__, con->icSuConnId);

    ccBufferEventToQueue(con, bcm, event, CCE_CONNECT_TO_RESOURCE, 0, 0, NULLP,
                         0);
    RETVALUE(ROK);
}
/* TRFO */

/*
*
*      Fun:   ccConE03S61
*
*      Desc:  Connection state function
*             event - Connect Status Indication.
*             state - 61-CCS_NOTIFYRSPFORMSUB
*
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes:
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC  S16 ccConE03S61
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  if (ccIsForcedOnHookRcvd(con->icProtType, con->ogProtType, con->icEvntType,
          con->direction))
  {
    CCDP(AIN_DBG_LEVEL_0, "IOM failover has occured:%ld\n", con->icSuConnId);
    cleanUpMgQ(con);
    ccUpdateCallDtlInfoRelOrignForOnHook(con->icProtType,con->ogProtType,con, con->direction, con->icEvntType);
    ccCleanUpMGCtx (con);   
    ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
    RETVALUE(ROK);
  }
  else
  {
    CCLOGERR("Unexpected message :icSu:%ld, icEvntType:%d\n",
             con->icSuConnId, con->icEvntType);
    RETVALUE(RFAILED);
  }
}


/*  
*   
*      Fun:   ccConE52S37
*   
*      Desc:  Connection state function
*             event - Disconnect
*             state - CCS_AWTRSPFORDPE3
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*   
*      Notes: None.
*   
*      File:  cc_bdy2_2.c
*   
*/  
PUBLIC S16 ccConE52S37
(   
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)   
{  

  if ((con == NULLP) || (event == NULLP))
  {
    CCLOGERR("Critical ERROR: Null pointers:%lx, %ld\n", (U32)con, event);
    RETVALUE(RFAILED);
  }

  CCDP(AIN_DBG_LEVEL_1, "ccConE52S37:icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", con->icSuConnId, con->ogSuConnId,
       (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0), con->icBcm.svcHeader.transId);
  /*FID 16206 +*/
  if(con->dummyCallInfo)
  {
    ccCleanDummyCall(con, IN_FAC_COMP_NOTAVE);
    RETVALUE(ROK);
  }
  /*FID 16206 -*/

  ccDisconnectCallForIcBCM(con, event);
  RETVALUE(ROK);
}


/*
*
*      Fun:   ccConE03S_FICBUF
*
*      Desc:  Connection state function
*             event: 03 - CCE_CNST_IND
*             state - Awaiting FIC Response states.
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE03S_FICBUF
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{

  ProtType icProtType     = CC_GET_PROTOCOL(con->icProtType); /* Bug: 88025 */
  ProtType ogProtType     = CC_GET_PROTOCOL(con->ogProtType); /* Bug: 88025 */
  S16      ret            = ROK; /* Bug: 88025 */

  if (ccIsForcedOnHookRcvd(con->icProtType, con->ogProtType, con->icEvntType,
          con->direction))
  {
    ccUpdateCallDtlInfoRelOrignForOnHook(con->icProtType,con->ogProtType,con, con->direction, con->icEvntType);
    CCDP(AIN_DBG_LEVEL_0, "IOM failover has occured:%ld\n", con->icSuConnId);
    CC_STATE_CHANGE(con, CCS_AWT_FIC_RSP_TO_KILL);
    RETVALUE(ROK);
  }
 
  /* Bug: 88025 + */ 
  if ((con->state == CCS_AWTRSPFORDPE17) && 
      (ccIsPrackRcvd(icProtType, ogProtType, con->icEvntType, con->direction)) &&
      (con->EarlyACMSt == EARLY_ACM_HAS_BEEN_SENT))
  {
       CCDP(AIN_DBG_LEVEL_0, "%s: Early ACM, dropping the PRACK\n", __FUNCTION__);
       ret = ccProcessPrackOrAck (con, icProtType, ogProtType, (CcAllSdus*)event);
       if (ret == ROKIGNORE)
       {
          RETVALUE (ROK);
       }
       else
       {
          CCLOGERR("%s: ERROR: Unexpected return value from ccProcessPrackOrAck()\n", __FUNCTION__);
          ABORT_DEBUG;
          RETVALUE (ROK);
       }
  }
  /* Bug: 88025 - */ 

  DP("Buffered an Event :%ld, %d\n", con->icSuConnId, con->icEvntType);
  ccBufferEventToQueue(con, bcm, event, CCE_CNSTIND, sizeof(CcCnStEvnt), 0,
                       NULLP, 0);
  RETVALUE(ROK);
}


/*
*
*      Fun:   ccConE04SBUF
*
*      Desc:  Connection state function
*             event - Cct Release Indication
*             state - 
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE04SBUF
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  CCDP(AIN_DBG_LEVEL_1, "ccConE04SBUF: icSu = %ld, ogSu = %ld,"
       " T.transId = %ld\n", con->icSuConnId, con->ogSuConnId,
       con->ogBcm.svcHeader.transId);
/* Begin - Bug# 52113 */
  if ((ccCheckEventInBuffer(&(con->ogBcm), CCE_RELIND) && 
      (con->direction == OUTTOINC)) ||
      (ccCheckEventInBuffer(&(con->icBcm), CCE_RELIND) && 
      (con->direction == INCTOOUT)))
  {
    CCDP(AIN_DBG_LEVEL_1, "%s():: REL is already buffered , drop it(direction=%d)\n",
         __FUNCTION__, con->direction);
    RETVALUE(ROK);
  }
/* End - Bug# 52113 */ 
  ccBufferEventToQueue(con, bcm, event, CCE_RELIND, sizeof(CcRelEvnt), 0,
                       NULLP, 0);
  RETVALUE(ROK);
}

#if 1 /* EARLY_ACM */
/*  
*   
*      Fun:   ccConE38SBUF  
*   
*      Desc:  Connection state function
*             event - Authorize Termination  
*             state -        
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
* 
*      Notes: None.
* 
*      File:  cc_bdy2_2.c
*   
*/
PUBLIC S16 ccConE38SBUF      
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)   
{   
  CCDP(AIN_DBG_LEVEL_1, "ccConE38SBUF: icSu = %ld, ogSu = %ld,"
       " T.transId = %ld\n", con->icSuConnId, con->ogSuConnId,
       con->ogBcm.svcHeader.transId);
  ccBufferEventToQueue(con, bcm, event, CCE_AUTHORIZE_TERMINATE, sizeof(SvcHeader_t),
                       (bcm->bcmType == BtOBcm) ? con->icSuConnId:con->ogSuConnId, 
                       event2, sizeof(AuthorizeTermination_t));
  RETVALUE(ROK);             
}   

/*  
*   
*      Fun:   ccConE39SBUF  
*   
*      Desc:  Connection state function
*             event - Continue  
*             state -        
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
* 
*      Notes: None.
* 
*      File:  cc_bdy2_2.c
*   
*/
PUBLIC S16 ccConE39SBUF
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  CCDP(AIN_DBG_LEVEL_1, "ccConE39SBUF: icSu = %ld, ogSu = %ld,"
       " T.transId = %ld\n", con->icSuConnId, con->ogSuConnId,
       con->ogBcm.svcHeader.transId);
  ccBufferEventToQueue(con, bcm, event, CCE_CONTINUE, sizeof(SvcHeader_t),
                       (bcm->bcmType == BtOBcm) ? con->icSuConnId:con->ogSuConnId, 
                       event2, sizeof(Continue_t));
  RETVALUE(ROK);
}

/*                             
*   
*      Fun:   ccConE47SBUF     
*   
*      Desc:  Connection state function
*             event - SendToResource  
*             state -        
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
* 
*      Notes: None.
* 
*      File:  cc_bdy2_2.c
*   
*/
PUBLIC S16 ccConE47SBUF
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  CCDP(AIN_DBG_LEVEL_1, "ccConE47SBUF: icSu = %ld, ogSu = %ld,"
       " T.transId = %ld\n", con->icSuConnId, con->ogSuConnId,
       con->ogBcm.svcHeader.transId);
  ccBufferEventToQueue(con, bcm, event, CCE_SEND_TO_RESOURCE, sizeof(SvcHeader_t),
                       (bcm->bcmType == BtOBcm) ? con->icSuConnId:con->ogSuConnId,
                       event2, sizeof(SendToResource_t));
  RETVALUE(ROK);
}

/*                             
*   
*      Fun:   ccConE05SBUF     
*   
*      Desc:  Connection state function
*             event - ReleaseConfirm  
*             state -        
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
* 
*      Notes: None.            
* 
*      File:  cc_bdy2_2.c
*   
*/
PUBLIC S16 ccConE05SBUF       
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */ 
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{                             
  CCDP(AIN_DBG_LEVEL_1, "ccConE05SBUF: icSu = %ld, ogSu = %ld,"
       " T.transId = %ld\n", con->icSuConnId, con->ogSuConnId,
       con->ogBcm.svcHeader.transId);
  ccBufferEventToQueue(con, bcm, event, CCE_RELCFM, sizeof(CcRelEvnt), 0,
                       NULLP, 0);
  RETVALUE(ROK);
}

/*                             
*   
*      Fun:   ccConE06SBUF     
*   
*      Desc:  Connection state function
*             event - ReleaseConfirm  
*             state -        
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
* 
*      Notes: None.            
* 
*      File:  cc_bdy2_2.c
*   
*/
PUBLIC S16 ccConE06SBUF
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  CCDP(AIN_DBG_LEVEL_1, "ccConE06SBUF: icSu = %ld, ogSu = %ld,"
       " T.transId = %ld\n", con->icSuConnId, con->ogSuConnId,
       con->ogBcm.svcHeader.transId);
  ccBufferEventToQueue(con, bcm, event, CCE_RSCCFM, sizeof(RmRsc), 0,
                       NULLP, 0);
  RETVALUE(ROK);
}

/*                             
*   
*      Fun:   ccConE62SBUF     
*   
*      Desc:  Connection state function
*             event - ReleaseConfirm  
*             state -        
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
* 
*      Notes: None.            
* 
*      File:  cc_bdy2_2.c
*   
*/
PUBLIC S16 ccConE62SBUF
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  CCDP(AIN_DBG_LEVEL_1, "ccConE62SBUF: icSu = %ld, ogSu = %ld,"
       " T.transId = %ld\n", con->icSuConnId, con->ogSuConnId,
       con->ogBcm.svcHeader.transId);
  ccBufferEventToQueue(con, bcm, event, CCE_MGCTNOTIFY, sizeof(MgctTxnInd), 0,
                       NULLP, 0);
  RETVALUE(ROK);
}

#endif


/*
*
*      Fun:   ccConE37S62
*
*      Desc:  Connection state function
*             event - CCE_ANALYZE_ROUTE
*             state - CCS_AWTRSPFORDPE19
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2.c
*
*/
PUBLIC S16 ccConE37S62
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  SvcHeader_t        *arSvcHeader           = NULLP;
  AnalyzeRoute_t     *pAnalyzeRoute         = NULLP;
  S16                 mgret                 = RFAILED;
  /* bug 72537 */
  MgctPackageId              pkgId  = MGCT_PACKAGE_ID_MAX;
  MgctMode                   mode   = MGCT_MODE_MAX;
  MgctCallProgItemId         signal = MGCT_ITEM_ID_CG_MAX;
  /* BUG 78431 */
  ProtType                   icProtType = 0;

  if ((con == NULLP) || (event == NULLP))
  {
    CCLOGERR("Critical ERROR: Null pointers:%lx, %ld\n", (U32)con, event);
    RETVALUE(RFAILED);
  }

  /* BUG 78431 */
  icProtType = CC_GET_PROTOCOL(con->icProtType);

  CCDP(AIN_DBG_LEVEL_1, "ccConE37S62: icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", con->icSuConnId, con->ogSuConnId,
       (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0), con->icBcm.svcHeader.transId);

  pAnalyzeRoute = (AnalyzeRoute_t *) event2;
  arSvcHeader = (SvcHeader_t *) event;
  
  ccCutBillingRecordIfNeeded(con, pAnalyzeRoute->scpResponse, OUTTOINC);
  ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));

  /* CPDI need restore ConInd event before mapping AR routing info into it */
  ccRestoreEvntHold(con);
  ccExtractAnalyzeRteParam(pAnalyzeRoute, bcm);
  ccExtractSvcHeader(arSvcHeader, bcm, MSG_Analyze_Route);

  ccStopConTmr(con, AIN_O_NO_ANS_TMR);
  ccStopConTmr(con, AIN_T_NO_ANS_TMR);
  /* STOP the AWAIT ANSWER Timer if running
   *
   * changes made to Support Configurable Answer Timer in GCC, RV Suresh, 5.0 
   */
  ccStopConTmr(con, CC_TMR_AWAITANS); 
  /* bug 72540 */
  ccStopConTmr (con, TMR_RELEASE);
  /* Re-Start the Set-up timer */
  ccStopConTmr(con, TMR_SETUP);
  ccStartConTmr(TMR_SETUP, con, (PTR)&ccCp.genCfg);
  ccDeleteBufferedEvent(con, con->icSuConnId, CCE_ONOANSEXP);
  ccDeleteBufferedEvent(con, con->ogSuConnId, CCE_TNOANSEXP);
  /* Bug# 52113 - Remove the buffered Egress REL */
  ccDeleteBufferedEvent(con, con->ogSuConnId, CCE_RELIND);

  ccCleanAnsFraudCall(con);

  ccRestoreReleaseInEvntHold(con);

  if (arSvcHeader->lastTransactionFlag == TRUE)
    ccDeleteBcm(&(con->icBcm));

  con->ogBcm.closeCause = CscRmtBusy;
  ccCloseTBcmTransaction(con);

  /* BUG 78431 + */
  if ((!CC_IS_CONTEXT_PRESENT(con)) && (con->isAnsIndSent) &&
     ((icProtType == CC_SIP) || (icProtType == CC_SIPT)))
  {
     /* Ingress is complete with offer/answer, this will set renegSdpAfterAns and force
      * reneg of the ingress SDP 
      */
     SET_SDPSENTONICBCM(con);
  }
  /* BUG 78431 - */

  if (CC_IS_CONTEXT_PRESENT(con) && (ccCheckMgContext(&con->ogBcm, &pkgId, &signal, &mode) == ROK)) /* bug 72537 */
  {
    ccQueueSubOnOGCleanUp(con);
    ccChangeCallStateOfAssociatedCalls(&(con->icBcm), CCS_CANNOTPROCESSEVENTS);
    /* CPDI_FIX: need to decide whether received an RelInd from outgoing before, or
       it's a case need initiate a release internally */
    if (CC_IS_REL_IN_EVNT_HOLD(con))
       con->relPend = OUTRELRSPANDROUTE;
    else
    {
       if (con->ccRelEvntHold)
    con->relPend = OUTRELRSPANDROUTE;
       else
       {
          /* put default cause value here */
          con->tempHolderForReleaseCause = AM_CV_NORMCALLCLR;
          con->relPend = OUTRELANDROUTE;
       }
    }
    CCDP(AIN_DBG_LEVEL_1, "%s: icSu:%ld, ogSu:%ld, suCtxId:%ld,"
         " con->relPend:%d\n", __FUNCTION__, con->icSuConnId, con->ogSuConnId,
         con->mgBlock->mgCtx->suCtxId, con->relPend);
        
    CC_STATE_CHANGE(con, CCS_AWTSWTCFMFORDP);
    con->mgBlock->con = con;
    mgret = ccSendMgiQueue(0, con->mgBlock);
    if (mgret == ROK)
      RETVALUE(ROK);
    else
    {
      CCLOGERR("Error: MGI failed:icSu:%ld, ogSu:%ld, suCtxId:%ld,"
               " icTransId:%ld\n", con->icSuConnId, con->ogSuConnId,
               con->mgBlock->mgCtx->suCtxId, con->icBcm.svcHeader.transId);
      ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
      if (con->ogRscVal)
      {
        ccCloseTBcmTransaction(con);
        con->direction = OUTTOINC;
        if (con->noRspReqd == FALSE) ccSendReleaseRsp(con);
        ccDeallocateResource(con, CC_OUTGOING);
        con->ogRscVal = FALSE;
      }
      cleanUpMgQ(con);
      ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
      RETVALUE(ROK);
    }
  }

  /* Clean outgoing and ReRoute call */
  if (con->ogRscVal)
  {
    ccCloseTBcmTransaction(con);
    con->direction = OUTTOINC;
    if (con->noRspReqd == FALSE) ccSendReleaseRsp(con);
    ccDeallocateResource(con, CC_OUTGOING);
    con->ogRscVal = FALSE;
  }

  /* Route the call */
  initOgBcm(&con->ogBcm);
  ccRouteCall(con, FALSE);
  RETVALUE(ROK);
}


/*
*
*      Fun:   ccConE37S138
*
*      Desc:  Connection state function
*             event - CCE_ANALYZE_ROUTE
*             state - CCS_AWT_RSP_FOR_SO                    
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2.c
*
*/
PUBLIC S16 ccConE37S138
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  SvcHeader_t        *arSvcHeader           = NULLP;
  AnalyzeRoute_t     *pAnalyzeRoute         = NULLP;
  /* BUG 78431 */
  ProtType            icProtType = 0;

  if ((con == NULLP) || (event == NULLP))
  {
    CCLOGERR("Critical ERROR: Null pointers:%lx, %ld\n", (U32)con, event);
    RETVALUE(RFAILED);
  }

  /* BUG 78431 */
  icProtType = CC_GET_PROTOCOL(con->icProtType);

  CCDP(AIN_DBG_LEVEL_1, "ccConE37S138: icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", con->icSuConnId, con->ogSuConnId,
       (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0), con->icBcm.svcHeader.transId);

  pAnalyzeRoute = (AnalyzeRoute_t *) event2;
  arSvcHeader = (SvcHeader_t *) event;
  
  ccCutBillingRecordIfNeeded(con, pAnalyzeRoute->scpResponse, OUTTOINC);
  ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));

  /* CPDI need restore ConInd event before mapping AR routing info into it */
  ccRestoreEvntHold(con);
  /* BUG#72949 Regenerate the ccEvntHold only for the failover scenario */
  ccRegenerateEvntHold(con);
  ccExtractAnalyzeRteParam(pAnalyzeRoute, bcm);
  ccExtractSvcHeader(arSvcHeader, bcm, MSG_Analyze_Route);

  ccStopConTmr(con, AIN_O_NO_ANS_TMR);
  ccStopConTmr(con, AIN_T_NO_ANS_TMR);
  /* STOP the AWAIT ANSWER Timer if running
   *
   * changes made to Support Configurable Answer Timer in GCC, RV Suresh, 5.0 
   */
  ccStopConTmr(con, CC_TMR_AWAITANS); 
  ccDeleteBufferedEvent(con, con->icSuConnId, CCE_ONOANSEXP);
  ccDeleteBufferedEvent(con, con->ogSuConnId, CCE_TNOANSEXP);
  /* Bug# 52113 - Remove the buffered Egress REL */
  ccDeleteBufferedEvent(con, con->ogSuConnId, CCE_RELIND);

  ccCleanAnsFraudCall(con);

  ccRestoreReleaseInEvntHold(con);

  if (arSvcHeader->lastTransactionFlag == TRUE)
    ccDeleteBcm(&(con->icBcm));

  con->ogBcm.closeCause = CscRmtBusy;
  ccCloseTBcmTransaction(con);

  /* BUG 78431 + */
  if ((!CC_IS_CONTEXT_PRESENT(con)) && (con->isAnsIndSent) &&
      ((icProtType == CC_SIP) || (icProtType == CC_SIPT)))
  {
     /* Ingress is complete with offer/answer, this will set renegSdpAfterAns and force
      * reneg of the ingress SDP 
      */
     SET_SDPSENTONICBCM(con);
  }
  /* BUG 78431 - */

  /* Clean outgoing and ReRoute call */
  if (con->ogRscVal)
  {
    ccCloseTBcmTransaction(con);
    con->direction = OUTTOINC;
    if (con->noRspReqd == FALSE) ccSendReleaseRsp(con);
    ccDeallocateResource(con, CC_OUTGOING);
    con->ogRscVal = FALSE;
  }

  /* Route the call */
  initOgBcm(&con->ogBcm);
  ccRouteCall(con, FALSE);
  RETVALUE(ROK);
}


/* FID 15261.0 + */
/*
*
*      Fun:   ccConE68S139
*
*      Desc:  Connection state function
*             event - CCE_CONGOVRLD_CFM
*             state - CCS_AWT_CONGOVRLD_CFM                    
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE68S139
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                   /* bcm */
PTR     event2                   /* event */
)
{
  S16 *rejectCall = NULLP;
  S16 ret = RFAILED;
  
  rejectCall = (S16 *)event;
  
  if (rejectCall == NULLP)
  {
    RETVALUE(RFAILED);
  }
  
  CCDP(AIN_DBG_LEVEL_1,"%s: rejectCall=%d\n", __FUNCTION__, *rejectCall);
  
  if (*rejectCall == TRUE)
  {
    switch (con->ccmCb.state)
    {
      case CC_INGRESS_MG:
      {
        if (con->ccmCb.flags & CC_STR_MG_LIST)
        {
          ccSendToResource(&(con->icBcm));
        }
        else
        {
          if (con->ogRscVal)
          {
            ccDeallocateResource(con, CC_OUTGOING);
            con->ogRscVal = FALSE;
          }

          if (CC_IS_MGW_AVAIL_ICMG_LIST(con))
          {
            ccReRouteCall(con, CC_INGRESS_MG);
          }
          else
          {
            ccProcessFailCnd(con, FC_MG_RLIST_EXHAUST, TRUE);
          }
        }
        break;
      }
      case CC_Y_MG:
      {
        CcMgSapCb   *pMgSap = NULLP;

        ccReconfigureCCNPostLiSub(con, TRUE);
        CC_SET_X2ONLY(con);

        ret = cmHashListFind(&ccMgSapTbl, (U8 *)&(con->ogBcm.mgSpId),
                       (U16)sizeof(SpId), (U16)0, (PTR *)&pMgSap);
        if (ret != ROK)
        {
            CCLOGERR("%s - ERROR: MG SAP Table Not Found %lx\n",
                     __FUNCTION__, con->icSuConnId);
            ABORT_DEBUG;
            if (con->ogRscVal)
            {
              ccDeallocateResource(con, CC_OUTGOING);
              con->ogRscVal = FALSE;
            }
            ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
        }

        if ((pMgSap->congovrld || pMgSap->megaaCngLvl) && 
            !CC_IS_BCM_IN_CONTEXT(&con->ogBcm))
        {
            con->ccmCb.state = CC_EGRESS_MG;
            /* Note we may actually be posting to our self */
            ccSendCongOvrldReqMsgToCcs(con, pMgSap->ccsId, ccCp.genCfg.ownCcsId);
            RETVALUE(ROK);
        }

        con->ccmCb.state = CC_INGRESS_MG;
        ccProcessAuthTermSwtReqTrue (con);
        break;
      }
      case CC_EGRESS_MG:
      { 
        if (con->ogRscVal)
        {
          ccDeallocateResource(con, CC_OUTGOING);
          con->ogRscVal = FALSE;
        }

        if (CC_IS_MGW_AVAIL_MG_LIST(con))
        {
          ccReRouteCall(con, CC_EGRESS_MG);
        }
        else
        {
          ccProcessFailCnd(con, FC_MG_RLIST_EXHAUST, TRUE);
        }
        break;
      }
      default:
      {
        RETVALUE(RFAILED);
      }
    }
  }
  else
  {
    if (con->ccmCb.flags & CC_STR_MG_LIST)
    {
      if (con->ccmCb.strMgList)
      {
        SPutSBuf(ccInit.region, ccInit.pool, con->ccmCb.strMgList, (Size)(sizeof(FcMgwInfo)));
        con->ccmCb.strMgList = NULLP;
      }

      ccProcessInternalAnnoucement(con);
      RETVALUE(ROK);
    }

    switch (con->ccmCb.state)
    {
      case CC_INGRESS_MG:
      {
	CcMgSapCb   *pMgSap = NULLP;
	
    if (con->ccmCb.ccn == CCN_5)
    {
        SpId      tmpGwId = 0;

        tmpGwId = (SpId) con->liInfo->ygwId.val;
        ret = cmHashListFind(&ccMgSapTbl, (U8 *)&(tmpGwId),
                           (U16)sizeof(tmpGwId), (U16)0, (PTR *)&pMgSap);

	if (ret != ROK)
	{
            CCLOGERR("%s - ERROR: MG SAP Table Not Found %lx\n",
                     __FUNCTION__, con->icSuConnId);
            ABORT_DEBUG;
            if (con->ogRscVal)
            {
              ccDeallocateResource(con, CC_OUTGOING);
              con->ogRscVal = FALSE;
            }
            ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
        }

        if ((pMgSap->congovrld || pMgSap->megaaCngLvl) && 
            !CC_IS_BCM_IN_CONTEXT(con->icLiAdjBcm))
        {
            con->ccmCb.state = CC_Y_MG;
            /* Note we may actually be posting to our self */
            ccSendCongOvrldReqMsgToCcs(con, pMgSap->ccsId, ccCp.genCfg.ownCcsId);
            RETVALUE(ROK);
        }
    }
    else
	if (CC_IS_CALL_MULTI_MG(con))
	{
          ret = cmHashListFind(&ccMgSapTbl, (U8 *)&(con->ogBcm.mgSpId),
	                       (U16)sizeof(SpId), (U16)0, (PTR *)&pMgSap);
	  if (ret != ROK)
	  {
            CCLOGERR("%s - ERROR: MG SAP Table Not Found %lx\n",
                     __FUNCTION__, con->icSuConnId);
            ABORT_DEBUG;
            if (con->ogRscVal)
            {
              ccDeallocateResource(con, CC_OUTGOING);
              con->ogRscVal = FALSE;
            }
            ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
          }

          if ((pMgSap->congovrld || pMgSap->megaaCngLvl) && 
               !CC_IS_BCM_IN_CONTEXT(&(con->ogBcm)))
          {
            con->ccmCb.state = CC_EGRESS_MG;
            /* Note we may actually be posting to our self */
            ccSendCongOvrldReqMsgToCcs(con, pMgSap->ccsId, ccCp.genCfg.ownCcsId);
            RETVALUE(ROK);
          }
        }

        /* FID15310.1 Need to assign the mgBlock thing here ++ */
        /* Associate the mgBlock */
        if( CC_IS_ICA_CALL(con) &&
            con->icBcm.cc &&
            con->icBcm.cc->legId02 &&
            (con->icBcm.cc->legId01 == &(con->icBcm)))
        {
          AinBCM_t *pOtherBcm = NULLP;
          pOtherBcm = ccFindOtherBcm(&(con->icBcm));
          if(pOtherBcm && pOtherBcm->con)
          {
            /* BUG:84498 Fix the ICA +ETC+DFC+Connect */
            if (con->mgBlock &&
                pOtherBcm->con->mgBlock &&
                (con->mgBlock == pOtherBcm->con->mgBlock))
            {
              /* This could be possible for SCP->SRP get Re-Route after it answered */
              CCDP(AIN_DBG_LEVEL_1, "%s: con2 0x%lx mgBlock already be assocaited with con1 0x%lx\n",
                  __FUNCTION__, (U32)con, (U32)pOtherBcm->con);
            }
            else
            {
              if(con->mgBlock)
                FREE_MGBLOCK(con->mgBlock);
              GCC_MNG_MGBLOCK_CNTR(con->mgBlock, pOtherBcm->con->mgBlock, con);
            }
            con->mgBlock->con = con;

            CCDP(AIN_DBG_LEVEL_1, "%s: con2 - icSu:%ld, ogSu:%ld, suCtxId:%ld,"
                " icTransId:%ld\n", __FUNCTION__,con->icSuConnId, con->ogSuConnId,
                (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0), con->icBcm.svcHeader.transId);
            CCDP(AIN_DBG_LEVEL_1, "%s: con1 - icSu:%ld, ogSu:%ld, suCtxId:%ld,"
                " icTransId:%ld\n", __FUNCTION__,pOtherBcm->con->icSuConnId, pOtherBcm->con->ogSuConnId,
                (pOtherBcm->con->mgBlock->mgCtx ? pOtherBcm->con->mgBlock->mgCtx->suCtxId : 0),
                pOtherBcm->con->icBcm.svcHeader.transId);
          }
        }
        
        /* + FID 16118.0 */
        if (CC_IS_CALL_FLAG_SET(con->flag,CC_FLAG_AGNOSTIC_NO_OFFER_CANDIDATE))
        {
          /* If it is SIP(T) to SIP(T) call demanding agnostic for INVITE with 
             NO Offer, skip MG insertion before inter-working */
          ccProcessAuthTermSwtReqFalse (con);
        }
        else
        {
          ccProcessAuthTermSwtReqTrue (con);
        }
        /* - FID 16118.0 */
        break;
      }
      case CC_Y_MG:
      {
          CcMgSapCb   *pMgSap = NULLP;
          ret = cmHashListFind(&ccMgSapTbl, (U8 *)&(con->ogBcm.mgSpId),
                           (U16)sizeof(SpId), (U16)0, (PTR *)&pMgSap);
          if (ret != ROK)
          {
              CCLOGERR("%s - ERROR: MG SAP Table Not Found %lx\n",
                       __FUNCTION__, con->icSuConnId);
              ABORT_DEBUG;
              if (con->ogRscVal)
              {
                  ccDeallocateResource(con, CC_OUTGOING);
                  con->ogRscVal = FALSE;
              }
              ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
          }

          if ((pMgSap->congovrld || pMgSap->megaaCngLvl) && 
               !CC_IS_BCM_IN_CONTEXT(&(con->ogBcm)))
          {
                con->ccmCb.state = CC_EGRESS_MG;
                /* Note we may actually be posting to our self */
                ccSendCongOvrldReqMsgToCcs(con, pMgSap->ccsId, ccCp.genCfg.ownCcsId);
                RETVALUE(ROK);
          }

          con->ccmCb.state = CC_INGRESS_MG;
          ccProcessAuthTermSwtReqTrue (con);
          break;
      }
      case CC_EGRESS_MG:
      { 
        con->ccmCb.state = CC_INGRESS_MG;
        /* FID15310.1 Need to assign the mgBlock thing here ++ */
        /* Associate the mgBlock */
        if( CC_IS_ICA_CALL(con) &&
            con->icBcm.cc &&
            con->icBcm.cc->legId02 &&
            (con->icBcm.cc->legId01 == &(con->icBcm)))
        {
          AinBCM_t *pOtherBcm = NULLP;
          pOtherBcm = ccFindOtherBcm(&(con->icBcm));
          if(pOtherBcm && pOtherBcm->con)
          {
            /* BUG:84498 Fix the ICA +ETC+DFC+Connect */
            if (con->mgBlock &&
                pOtherBcm->con->mgBlock &&
                (con->mgBlock == pOtherBcm->con->mgBlock))
            {
              /* This could be possible for SCP->SRP get Re-Route after it answered */
              CCDP(AIN_DBG_LEVEL_1, "%s: con2 0x%lx mgBlock already be assocaited with con1 0x%lx\n",
                  __FUNCTION__, (U32)con, (U32)pOtherBcm->con);
            }
            else
            {
              FREE_MGBLOCK(con->mgBlock);
              GCC_MNG_MGBLOCK_CNTR(con->mgBlock, pOtherBcm->con->mgBlock, con);
            }
            con->mgBlock->con = con;

            CCDP(AIN_DBG_LEVEL_1, "%s: con2 - icSu:%ld, ogSu:%ld, suCtxId:%ld,"
                " icTransId:%ld\n", __FUNCTION__,con->icSuConnId, con->ogSuConnId,
                (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0), con->icBcm.svcHeader.transId);
            CCDP(AIN_DBG_LEVEL_1, "%s: con1 - icSu:%ld, ogSu:%ld, suCtxId:%ld,"
                " icTransId:%ld\n", __FUNCTION__,pOtherBcm->con->icSuConnId, pOtherBcm->con->ogSuConnId,
                (pOtherBcm->con->mgBlock->mgCtx ? pOtherBcm->con->mgBlock->mgCtx->suCtxId : 0),
                pOtherBcm->con->icBcm.svcHeader.transId);
          }
        }

        /* + FID 16118.0 */
        if (CC_IS_CALL_FLAG_SET(con->flag,CC_FLAG_AGNOSTIC_NO_OFFER_CANDIDATE))
        {
          /* If it is SIP(T) to SIP(T) call demanding agnostic for INVITE with 
             NO Offer, skip MG insertion before inter-working */
          ccProcessAuthTermSwtReqFalse (con);
        }
        else
        {
          ccProcessAuthTermSwtReqTrue (con);
        }
        /* - FID 16118.0 */
        break;
      }
      default:
      {
        RETVALUE(RFAILED);
      }
    }
  }
  RETVALUE(ROK);
}

/*
*
*      Fun:   ccConE10S139
*
*      Desc:  Connection state function
*             event - Resource Deallocation Confirm
*             state - CCS_AWT_CONGOVRLD_CFM                    
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE10S139
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  if (con->ogRscVal)
  {
    con->ogRscVal = FALSE;
  }
  else
  {
    /* Check - This is an error */
    RETVALUE(RFAILED);
  }
  RETVALUE(ROK);
}
/* FID 15261.0 - */

/* 
*  
*      Fun:   ccConE39S62
*  
*      Desc:  Connection state function
*             event - CONTINUE 
*             state - CCS_AWTRSPFORDPE19
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*  
*      Notes: None.
*  
*      File:  cc_bdy2_2.c
*  
*/ 
PUBLIC S16 ccConE39S62
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)  
{
  SvcHeader_t        *ctSvcHeader           = NULLP;
  S16                 mgret                 = RFAILED;
  Continue_t        *pContinue              = NULLP;
  S16                 relRstRet             = ROK;
  /* bug 72537 */
  MgctPackageId              pkgId  = MGCT_PACKAGE_ID_MAX;
  MgctMode                   mode   = MGCT_MODE_MAX;
  MgctCallProgItemId         signal = MGCT_ITEM_ID_CG_MAX;
   
  if ((con == NULLP) || (event == NULLP))
  {
    CCLOGERR("Critical ERROR: Null pointers:%lx, %ld\n", (U32)con, event);
    RETVALUE(RFAILED);
  }
  CCDP(AIN_DBG_LEVEL_1, "ccConE39S62:icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", con->icSuConnId, con->ogSuConnId,
       (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0), con->icBcm.svcHeader.transId);

  ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
  ccStopConTmr(con, AIN_O_NO_ANS_TMR);
  ccStopConTmr(con, AIN_T_NO_ANS_TMR);
  /* STOP the AWAIT ANSWER Timer if running
   *
   * changes made to Support Configurable Answer Timer in GCC, RV Suresh, 5.0 
   */
  ccStopConTmr(con, CC_TMR_AWAITANS); 
  ccDeleteBufferedEvent(con, con->icSuConnId, CCE_ONOANSEXP);
  ccDeleteBufferedEvent(con, con->ogSuConnId, CCE_TNOANSEXP);
#if 1 /* EARLY_ACM */
  ccStopConTmr(con, CC_TMR_EARLYACM); 
#endif

  /* CPDI_FIX 28351, 28730 */
  relRstRet = ccRestoreReleaseInEvntHold(con);

  ctSvcHeader = (SvcHeader_t *) event;
  pContinue   = (Continue_t *) event2;
  if(pContinue->errorCode != FC_NOT_APPLICABLE)
  {
    CCLOGERR("ERROR FROM FIC = %ld\n", pContinue->errorCode);
    con->tempHolderForReleaseCause = pContinue->errorCode;
  }
  if  (con->icProtType == CC_LOCAL_SWT_PROT)
    ccRemoveCaleaAssnWithTappedBcm(con);

  con->ainInfo.termInd = EXCEPTION_IND;
  con->ogBcm.closeCause = CscRmtBusy;
  ccCloseTBcmTransaction(con);

  /* BUG:84749 Fix the ETC failed for the SRP and no response get for the ETC failed */
  if (CC_IS_ICA_CALL(con) && con->icBcm.cc &&
      con->icBcm.cc->legId02 &&
      (con->icBcm.cc->legId01 == &(con->icBcm)))
  {
    CCDP(AIN_DBG_LEVEL_1, "ccConE39S62: ICA controller con\n");
    ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
    RETVALUE(ROK);
  }

  if (CC_IS_CONTEXT_PRESENT(con) && (ccCheckMgContext(&con->ogBcm, &pkgId, &signal, &mode) == ROK)) /* bug 72537 */
  {
#if 1 /* EARLY_ACM */
    /* Need to clean all the legs in the context */
    CC_RESET_ANY_EARLY_ACM_FLAGS(con);
#endif
    ccQueueSubOnOGCleanUp(con);
    con->relPend = OUTRELRSPANDINTONE;

    CC_STATE_CHANGE(con, CCS_AWTSWTCFM_SENDONEREL);
    con->mgBlock->con = con;
    mgret = ccSendMgiQueue(0, con->mgBlock);
    if (mgret == ROK)
      RETVALUE(ROK);
    else
    {
      CCLOGERR("Error: MGI failed:icSu:%ld, ogSu:%ld, suCtxId:%ld,"
               " icTransId:%ld\n", con->icSuConnId, con->ogSuConnId,
               con->mgBlock->mgCtx->suCtxId, con->icBcm.svcHeader.transId);
      cleanUpMgQ(con);
    }
  }

  if (relRstRet)
  {
     ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
     RETVALUE(ROK);
  }
  
  if (con->ogRscVal)
  {
    con->direction = OUTTOINC;
    if (con->noRspReqd == FALSE) ccSendReleaseRsp(con);
    ccDeallocateResource(con, CC_OUTGOING);
    con->ogRscVal = FALSE;
  }

  if (pContinue->errorCode != FC_NOT_APPLICABLE)
  {
    ccProcessFailCnd(con, pContinue->errorCode, TRUE);
  }
  else
  {
      CcCause cause;
      TknU16  sipCause;
      TknU16  sipStatus;


      CC_STATE_CHANGE(con, CCS_AWTRELCFM_IC);

      /* Get protocol release cause */
      ccGetRelCause(con, (con->direction == OUTTOINC?con->ogProtType:\
                     con->icProtType),
                    (CcRelEvnt *) con->ccEvntHold, &cause,
                    &sipCause, &sipStatus);

      if ((con->icProtType == CC_CS_TG) ||
          (ccIsLEGSubscriber(&(con->icBcm))))
      {
          ccStopConTmr(con, TMR_RELEASE);
          ccStartConTmr(TMR_SETUP, con, (PTR)&ccCp.genCfg);
          ccProcessFailCnd(con,
                           ccMapProtRel2FailCnd(con, &cause,
                                                &sipCause, &sipStatus),
                           TRUE);
          RETVALUE(ROK);
      }
      else
      {
          CcAllSdus  ogEvnt; 
          S16 ret = RFAILED;
          
          /* bug 91532 */
          cmMemset((U8 *)&ogEvnt, 0, sizeof(ogEvnt));
         
          con->icEvnt = (CcAllSdus *) con->ccEvntHold;
          con->ogEvnt = &ogEvnt;

          ret = ccMapEvent(con, CCE_RELIND, 0);
          if (ret != ROK)
          {
              ccSendRelease(con, con->icSapCb, con->icSpConnId,
                            con->icSuConnId,
                            con->icProtType, CCPROTERR,NULL);
              RETVALUE(ROK);
          }
          ccSendReleaseReq(con);
          RETVALUE(ROK);
      }
  }
  RETVALUE(ROK);
}

/*
*
*      Fun:   ccConE47S62
*
*      Desc:  Connection state function
*             event - Send_To_Resource
*             state - CCS_AWTRSPFORDPE19
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE47S62
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  SvcHeader_t       *srSvcHeader           = NULLP;
  S16                ret                   = RFAILED;
  S16                mgret                 = RFAILED;

  if ((con == NULLP) || (event == NULLP))
  {
    CCLOGERR("Critical ERROR: Null pointers:%lx, %ld\n", (U32)con, event);
    RETVALUE(RFAILED);
  }
  CCDP(AIN_DBG_LEVEL_1, "ccConE47S62:icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", con->icSuConnId, con->ogSuConnId,
       (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0), con->icBcm.svcHeader.transId);

  ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
  srSvcHeader = (SvcHeader_t *) event;

  ccStopConTmr(con, AIN_O_NO_ANS_TMR);
  ccStopConTmr(con, AIN_T_NO_ANS_TMR);
  /* STOP the AWAIT ANSWER Timer if running
   *
   * changes made to Support Configurable Answer Timer in GCC, RV Suresh, 5.0 
   */
  ccStopConTmr(con, CC_TMR_AWAITANS); 
  ccDeleteBufferedEvent(con, con->icSuConnId, CCE_ONOANSEXP);
  ccDeleteBufferedEvent(con, con->ogSuConnId, CCE_TNOANSEXP);
#if 1 /* EARLY_ACM */
  ccStopConTmr(con, CC_TMR_EARLYACM); 
#endif

  ccRestoreReleaseInEvntHold(con);

  if (srSvcHeader->lastTransactionFlag == TRUE)
    ccDeleteBcm(&(con->icBcm));

  con->ogBcm.closeCause = CscRmtBusy;
  ccCloseTBcmTransaction(con);

  con->lastState = con->state;
  con->anncBlock.stateForAnnc = con->state;
  if (CC_IS_CONTEXT_PRESENT(con))
  {
#if 1 /* EARLY_ACM */
    /* Need to clean all the legs in the context */
    CC_RESET_ANY_EARLY_ACM_FLAGS(con);
#endif
    ccQueueSubOnOGCleanUp(con);
    ccChangeCallStateOfAssociatedCalls(&(con->icBcm), CCS_CANNOTPROCESSEVENTS);
    
    /* CPDI_FIX: need to decide whether received an RelInd from outgoing before, or
       it's a case need initiate a release internally */
    if (CC_IS_REL_IN_EVNT_HOLD(con))
       con->relPend = OUTRELRSPANDPLAYANNC;
    else
    {
       if (con->ccRelEvntHold)
    con->relPend = OUTRELRSPANDPLAYANNC;
       else
       {
          /* put default cause value here */
          con->tempHolderForReleaseCause = AM_CV_NORMCALLCLR;
          con->relPend = OUTRELANDPLAYANNC;;
       } 
    }
    
    CC_STATE_CHANGE(con, CCS_AWTSWTCFMFORDP);
    con->mgBlock->con = con;
    mgret = ccSendMgiQueue(0, con->mgBlock);
    if (mgret == ROK)
      RETVALUE(ROK);
    else
    {
      CCLOGERR("Error: MGI failed:icSu:%ld, ogSu:%ld, suCtxId:%ld,"
               " icTransId:%ld\n", con->icSuConnId, con->ogSuConnId,
               con->mgBlock->mgCtx->suCtxId, con->icBcm.svcHeader.transId);
      ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
      if (con->ogRscVal)
      {
        con->direction = OUTTOINC;
        if (con->noRspReqd == FALSE) ccSendReleaseRsp(con);
        ccDeallocateResource(con, CC_OUTGOING);
        con->ogRscVal = FALSE;
      }
      cleanUpMgQ(con);
      ccHandleCleanUpConCb(con, TRUE, AM_CV_TMPFAIL);
      RETVALUE(ROK);
    }
  }

  if (con->ogRscVal)
  {
    con->direction = OUTTOINC;
    if (con->noRspReqd == FALSE) ccSendReleaseRsp(con);
    ccDeallocateResource(con, CC_OUTGOING);
    con->ogRscVal = FALSE;
  }

  initOgBcm(&con->ogBcm);
  ret = ccSendToResource(&(con->icBcm));
  if (ret != ROK)
  {
    CCLOGERR("Error: ccSendToResource failed:icSu:%ld, ogSu:%ld, icTrsId:%ld\n",
             con->icSuConnId, con->ogSuConnId, (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0));
    ccHandleCleanUpConCb(con, TRUE, AM_CV_TMPFAIL);
  }
  RETVALUE(ROK);
}


/*
*
*      Fun:   ccConE52S62
*
*      Desc:  Connection state function
*             event - Disconnect
*             state - CCS_AWTRSPFORDPE19
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE52S62
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{

  if ((con == NULLP) || (event == NULLP))
  {
    CCLOGERR("Critical ERROR: Null pointers:%lx, %ld\n", (U32)con, event);
    RETVALUE(RFAILED);
  }

  CCDP(AIN_DBG_LEVEL_1, "ccConE52S62:icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", con->icSuConnId, con->ogSuConnId,
       (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0), con->icBcm.svcHeader.transId);

  con->ogBcm.closeCause = CscRmtBusy;
  ccCloseTBcmTransaction(con);

  ccRestoreReleaseInEvntHold(con);
  ccDisconnectCallForIcBCM(con, event);
  RETVALUE(ROK);
}


/*
*
*      Fun:   ccConE37S45
*
*      Desc:  Connection state function
*             event - CCE_ANALYZE_ROUTE
*             state - 45-CCS_AWTRSPFORDPE33
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE37S45
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  SvcHeader_t        *arSvcHeader           = NULLP;
  AnalyzeRoute_t     *pAnalyzeRoute         = NULLP;
  U8                  relDisBoolOG          = RFAILED;
  U8                  evntType              = 0;
  S16                 mgret                 = RFAILED;
  S16                 ret    = RFAILED;
  MgctPackageId       pkgId  = MGCT_PACKAGE_ID_MAX;
  MgctCallProgItemId  signal = MGCT_ITEM_ID_CG_MAX;
  MgctMode            mode   = MGCT_MODE_MAX;

  if ((con == NULLP) || (event == NULLP))
  {
    CCLOGERR("Critical ERROR: Null pointers:%lx, %ld\n", (U32)con, event);
    RETVALUE(RFAILED);
  }

  CCDP(AIN_DBG_LEVEL_1, "ccConE37S45: icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", con->icSuConnId, con->ogSuConnId,
       (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0), con->icBcm.svcHeader.transId);
  /* Bug 91551 When the call goes unanswered and rolls to VM, the new call
   * to the VM platform uses the TLDN in the tempAT of the first call leg
   * rather than the route digits returned from the TTNR
   */
  if(con->tempAuthTerm != NULLP)
  {
    /* Bug 80641, reset con->tempAuthTerm but keep homeCc */
    TknStr4 tmpHomeCc = con->tempAuthTerm->homeCc;

    CCDP(AIN_DBG_LEVEL_1, "ccConE37S09: Free tempAuthTerm\n");
    if (con->tempAuthTerm->tempBuf.pres == PRSNT_NODEF)
    {
       ccDropUBuf((Buffer **)&con->tempAuthTerm->tempBuf.val);
    }
    cmMemset ((U8 *)con->tempAuthTerm, 0, sizeof(CcTempAuthTerm));
    con->tempAuthTerm->homeCc = tmpHomeCc; /*Bug 80641 */
  }


  ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
  ccDeleteBufferedEvent(con, con->ogSuConnId, CCE_TNOANSEXP);
  ccStopConTmr(con, AIN_T_NO_ANS_TMR);
  /* STOP the AWAIT ANSWER Timer if running
   *
   * changes made to Support Configurable Answer Timer in GCC, RV Suresh, 5.0 
   */
  ccStopConTmr(con, CC_TMR_AWAITANS); 
  ccCleanAnsFraudCall(con); 

  if (ccCheckEventInBuffer(&(con->ogBcm), CCE_CONCFM))
  {
    /* Drop the Analyze Route Response and process the Connect Confirm */
    DP("Dropping the AR and processing the ConCfm message:%ld, %ld\n",
       con->icSuConnId, con->icBcm.svcHeader.transId);
    CC_STATE_CHANGE(con, CCS_AWTANS);
    RETVALUE(ROK);
  }
  pAnalyzeRoute = (AnalyzeRoute_t *) event2;
  arSvcHeader   = (SvcHeader_t *) event;

  ccCutBillingRecordIfNeeded(con, pAnalyzeRoute->scpResponse, OUTTOINC);

  /* CPDI need restore ConInd event before mapping AR routing info into it */
  ccRestoreEvntHold(con);
  ccExtractAnalyzeRteParam(pAnalyzeRoute, bcm);
  ccExtractSvcHeader(arSvcHeader, bcm, MSG_Analyze_Route);

  if (arSvcHeader->lastTransactionFlag == TRUE)
    ccDeleteBcm(&(con->icBcm));

  con->ogBcm.closeCause = CscRmtTscTimeout;
  ret = ccCheckMgContext(&(con->ogBcm), &pkgId, &signal, &mode); 
  if (CC_IS_CONTEXT_PRESENT(con) && ret == ROK)
  {
    ccQueueSubOnOGCleanUp(con);
    con->relPend = OUTRELANDROUTE;
    CC_STATE_CHANGE(con, CCS_AWTSWTCFMFORDP);
    ccChangeCallStateOfAssociatedCalls(&(con->icBcm), CCS_CANNOTPROCESSEVENTS);
    con->mgBlock->con = con;
    mgret = ccSendMgiQueue(0, con->mgBlock);
    if (mgret == ROK)
      RETVALUE(ROK);
    else
    {
      CCLOGERR("Error: MGI failed:icSu:%ld, ogSu:%ld, suCtxId:%ld,"
               " icTransId:%ld\n", con->icSuConnId, con->ogSuConnId,
               con->mgBlock->mgCtx->suCtxId, con->icBcm.svcHeader.transId);
      ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
      cleanUpMgQ(con);
      ccHandleCleanUpConCb(con, TRUE, AM_CV_TMPFAIL);
      RETVALUE(ROK);
    }
  }

  /* Clean outgoing and ReRoute call */
  if (con->ogRscVal)
  {
    relDisBoolOG = ccCheckRelOrOnHookInBuffer(&(con->ogBcm), &evntType);
    if (relDisBoolOG == ROK)
    {
      con->ogBcm.closeCause = CscRmtBusy;
      ccCloseTBcmTransaction(con);
      con->direction = OUTTOINC;
      if (con->noRspReqd == FALSE) ccSendReleaseRsp(con);
      ccDeallocateResource(con, CC_OUTGOING);
      con->ogRscVal = FALSE;
      initOgBcm(&con->ogBcm);
      ccRouteCall(con, FALSE);
    }
    else
    {
      ccFreeTheRscFromCon(con, OUTTOINC); /* This will take care of REL_CFM */

      initOgBcm(&con->ogBcm);
      if(ROK == ccCheckForInterfaces(con))
        ccRouteCall(con, FALSE);
      else
      {
        CC_TRACE_DUMP(con);
        ccProcessFailCnd(con, FC_INTERNAL, TRUE);
      }
    }
    RETVALUE(ROK);
  }
  else
  {
    /* Route the call */
    CCLOGERR("Error: Unexpected condition, still going ahead:%ld\n",
             con->icSuConnId);
    initOgBcm(&con->ogBcm);
    ccRouteCall(con, FALSE);
    RETVALUE(ROK);
  }
}


/*
*
*      Fun:   ccConE39S44
*
*      Desc:  Connection state function
*             event - CONTINUE
*             state - 44-CCS_AWTRSPFORDPE9
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE39S44
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
    Continue_t        *pContinue      = NULLP;
    S16                ret            = RFAILED;
    CcRelEvnt          ogEvnt;

#ifdef ZC
    ZcUpdParam    upd;
#endif /* ZC */

    CCDP(AIN_DBG_LEVEL_1, "%s: icSu:0x%lx, ogSu:0x%lx, suCtxId:0x%lx,"
         " icTransId:0x%lx, con->state:%d\n", __FUNCTION__,
         (U32)con->icSuConnId, (U32)con->ogSuConnId,
         (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0), con->icBcm.svcHeader.transId,
         con->state);

    /* bug 91532 */
    cmMemset((U8 *)&ogEvnt, 0, sizeof(ogEvnt));
   
    pContinue   = (Continue_t *)  event2;

    ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));

    ccStopConTmr(con, AIN_O_NO_ANS_TMR);
    ccStopConTmr(con, AIN_T_NO_ANS_TMR);
    ccStopConTmr(con, CC_TMR_AWAITANS);
    ccStopConTmr(con, TMR_SETUP);
    ccStopConTmr(con, CC_TMR_EARLYACM);
    ccStopConTmr(con, CC_TMR_CALLDTL);
    ccDeleteBufferedEvent(con, con->icSuConnId, CCE_ONOANSEXP);
    ccDeleteBufferedEvent(con, con->ogSuConnId, CCE_TNOANSEXP);


    if (ccRestoreReleaseInEvntHold(con))
    {
       /* bug72209 */
       if(con->tempHolderForReleaseCause == CCTMRRECOV)
          ccHandleCleanUpConCb(con, TRUE, CCTMRRECOV);
       else
          ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
       RETVALUE(ROK);
    }

    if (pContinue && pContinue->errorCode != FC_NOT_APPLICABLE)
    {
        CCLOGERR("ERROR FROM FIC = %ld\n", pContinue->errorCode);
        if (isThisBcmInMPartyCall(&con->icBcm) || isThisBcmInMPartyCall(&con->ogBcm)) /*bug91115*/ 
        {
          ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL); /*Bug 90369*/
          RETVALUE(ROK);
        }
    }
#ifdef ZC
    /* REL CON */
    cmMemset((U8 *)&upd, 0, sizeof(ZcUpdParam));
    upd.icSuConnId = con->icSuConnId;
    zcRunTimeUpd(ZC_RELCON_CB, CMPFTHA_UPD_REQ, upd);
    zcUpdPeer();
#endif
    ccStartConTmr(TMR_RELEASE, con, (PTR)&ccCp.genCfg);

    if (CC_IS_CONTEXT_PRESENT(con))
    {
        ccQSubBasedOnCcn(con);  /* FID 15261.0 */

        ret = ccSendMgiQueue(0, con->mgBlock);
        if (ret != ROK)
        {
            CCLOGERR( "**** ERROR: can't do ccSendMgiQueue *******\n");
            cleanUpMgQ(con);
            ccCleanUpMGCtx(con);
        }
        else
        {
            CC_STATE_CHANGE(con, CCS_AWTSWTCFM_SENDONEREL);
            con->mgBlock->con = con;
            if(con->direction == OUTTOINC)
                con->relPend = OUTRELRSPANDINREL;
            else
            { 
                /* FID 16706.0 + */
                if (!CC_BOTH_TG_SUPPORT_AOC_XML(con))
                {
                  con->relPend = INRELRSPANDOUTREL;
                }
                {
                  con->relPend = INRELRSPDELAYANDOUTREL;;
                }
                /* FID 16706.0 - */
            }
            RETVALUE(ROK);
        }
    }
    /* bug 77255  ++ */
      if (con->direction == INCTOOUT)
      {
        CC_STATE_CHANGE(con, CCS_AWTRELCFM_OG);
        ccDeallocateResource(con, CC_INCOMING);
        con->icRscVal = FALSE;
      }
      else if (con->direction == OUTTOINC)
      {
        CC_STATE_CHANGE(con, CCS_AWTRELCFM_IC);
        ccDeallocateResource(con, CC_OUTGOING);
        con->ogRscVal = FALSE;
      }
    /* bug 77255  -- */


    con->icEvnt = (CcAllSdus *) con->ccEvntHold;
    con->ogEvnt = (CcAllSdus *) &ogEvnt;

    if (con->noRspReqd == FALSE) ccSendReleaseRsp(con);

    ret = ccMapEvent(con, CCE_RELIND, 0);
    if (ret != ROK)
    {
    /* bug 77255  ++ */
       if (con->direction == INCTOOUT)
       {
          ccSendRelease(con, con->ogSapCb, con->ogSpConnId, con->ogSuConnId,
                    con->ogProtType, con->tempHolderForReleaseCause,NULL);
       }
       else if (con->direction == OUTTOINC)
       {
            ccSendRelease(con, con->icSapCb, con->icSpConnId, con->icSuConnId,
                      con->icProtType, con->tempHolderForReleaseCause,NULL);
       }
    /* bug 77255  -- */
        RETVALUE(ROK);
    }
    ccSendReleaseReq(con);
    RETVALUE(ROK);
}


/*
*
*      Fun:   ccConE39S45
*
*      Desc:  Connection state function
*             event - CONTINUE
*             state - 45-CCS_AWTRSPFORDPE33
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE39S45
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  SvcHeader_t        *ctSvcHeader           = NULLP;
  Continue_t         *pContinue             = NULLP; 
  /* FID16901.0 */
  ProtType            ogProtType            = 0;
    
  if ((con == NULLP) || (event == NULLP))
  {
    CCLOGERR("Critical ERROR: Null pointers:%lx, %ld\n", (U32)con, event);
    RETVALUE(RFAILED);
  }

  ctSvcHeader = (SvcHeader_t *) event;
  pContinue   = (Continue_t *) event2;

  if(pContinue->errorCode != FC_NOT_APPLICABLE)
  {
    CCLOGERR("ERROR FROM FIC = %ld\n", pContinue->errorCode);
  }

  CCDP(AIN_DBG_LEVEL_1, "ccConE39S45:icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", con->icSuConnId, con->ogSuConnId,
       (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0), con->icBcm.svcHeader.transId);

  ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
  if (ccCheckEventInBuffer(&(con->ogBcm), CCE_CONCFM))
  {
    /* Drop the Continue Response and process the Connect Confirm */
    DP("Dropping the Continue and processing the ConCfm message:%ld, %ld\n",
       con->icSuConnId, con->icBcm.svcHeader.transId);
    CC_STATE_CHANGE(con, CCS_AWTANS);
    RETVALUE(ROK);
  }

  /* HACK FOR NEW SOUTH, NOT AS PER GR1298 */
  /* Because New South Does not have a ONoAnswer timeout field in the
     * SCP message */
  ccStopConTmr(con, AIN_O_NO_ANS_TMR);
  ccDeleteBufferedEvent(con, con->icSuConnId, CCE_ONOANSEXP);

  /* For Brazil which actually has the O_NO_ANSWER in SCP, after
   * O_NO_ANSWER is reported and continue is received, need to
   * clean_up the call */
  if (CC_EGRESS_INTER_CCS_CALL(con))
  {
     if (con->ogIntfcCb)
     {
        ogProtType = con->ogIntfcCb->protType;
     }
     else
     {
        ogProtType = con->realLegProtType;
     }
  }
  else
     ogProtType = con->ogProtType;

  ccProcessONoAnswer(&(con->icBcm));

  if ((con->lastState != CCS_AWTANS) ||
      (pContinue->errorCode != FC_NOT_APPLICABLE) ||
      ((con->direction == OUTTOINC) &&  CC_IS_PROT_TYPE_BRAZIL(ogProtType))) /* FID16901 */
  {
    CCDP(AIN_DBG_LEVEL_0, "icSu:%ld, icTransId:%ld, state:%d\n",
         con->icSuConnId, con->icBcm.svcHeader.transId, con->lastState);
    ccDeleteBufferedEvent(con, con->ogSuConnId, CCE_TNOANSEXP);
    ccHandleCleanUpConCb(con, TRUE, AM_CV_TMPFAIL);
    RETVALUE(RFAILED);
  }
  else
  {
    CC_STATE_CHANGE(con, con->lastState);
    RETVALUE(ROK);
  }
}


/*
*
*      Fun:   ccConE47S45
*
*      Desc:  Connection state function
*             event - Send_To_Resource
*             state - 45-CCS_AWTRSPFORDPE33
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE47S45
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  SvcHeader_t        *srSvcHeader           = NULLP;
  S16                 mgret                 = RFAILED;
  S16                 ret                   = RFAILED;

  if ((con == NULLP) || (event == NULLP))
  {
    CCLOGERR("Critical ERROR: Null pointers:%ld, %ld\n", (U32)con, (U32)event);
    RETVALUE(RFAILED);
  }
  CCDP(AIN_DBG_LEVEL_1, "ccConE47S45:icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", con->icSuConnId, con->ogSuConnId,
       (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0), con->icBcm.svcHeader.transId);

  ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
  srSvcHeader = (SvcHeader_t *) event;

  ccDeleteBufferedEvent(con, con->ogSuConnId, CCE_TNOANSEXP);
  ccStopConTmr(con, AIN_T_NO_ANS_TMR);
  /* STOP the AWAIT ANSWER Timer if running
   *
   * changes made to Support Configurable Answer Timer in GCC, RV Suresh, 5.0 
   */
  ccStopConTmr(con, CC_TMR_AWAITANS); 
#if 1 /* EARLY_ACM */
  ccStopConTmr(con, CC_TMR_EARLYACM);
#endif
  
  if (ccCheckEventInBuffer(&(con->ogBcm), CCE_CONCFM))
  {
    /* Drop the STR Response and process the Connect Confirm */
    DP("Dropping the STR and processing the ConCfm message:%ld, %ld\n",
       con->icSuConnId, con->icBcm.svcHeader.transId);
    CC_STATE_CHANGE(con, CCS_AWTANS);
    RETVALUE(ROK);
  }

  if (srSvcHeader->lastTransactionFlag == TRUE)
    ccDeleteBcm(&(con->icBcm));

  con->ogBcm.closeCause = CscRmtTscTimeout;
  con->lastState = con->state;
  con->anncBlock.stateForAnnc = con->state;
  if (CC_IS_CONTEXT_PRESENT(con))
  {
#if 1 /* EARLY_ACM */
    /* Need to clean all the legs in the context */
    CC_RESET_ANY_EARLY_ACM_FLAGS(con);
#endif
    ccQueueSubOnOGCleanUp(con);
    ccChangeCallStateOfAssociatedCalls(&(con->icBcm), CCS_CANNOTPROCESSEVENTS);
    con->relPend = OUTRELANDPLAYANNC;
    CC_STATE_CHANGE(con, CCS_AWTSWTCFMFORDP);
    con->mgBlock->con = con;
    mgret = ccSendMgiQueue(0, con->mgBlock);
    if (mgret == ROK)
      RETVALUE(ROK);
    else
    {
      CCLOGERR("Error: MGI failed:icSu:%ld, ogSu:%ld, suCtxId:%ld,"
               " icTransId:%ld\n", con->icSuConnId, con->ogSuConnId,
               con->mgBlock->mgCtx->suCtxId, con->icBcm.svcHeader.transId);
      ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
      cleanUpMgQ(con);
      ccHandleCleanUpConCb(con, TRUE, AM_CV_TMPFAIL);
      RETVALUE(ROK);
    }
  }

  if (con->ogRscVal)
  {
    ccFreeTheRscFromCon(con, OUTTOINC); /* This will take care of REL_CFM */
  }

  {
    initOgBcm(&con->ogBcm);
    ret = ccSendToResource(&(con->icBcm));
    if (ret != ROK)
    {
      CCLOGERR("Error: ccSendToResource failed:icSu:%ld,ogSu:%ld,icTrsId:%ld\n",
               con->icSuConnId, con->ogSuConnId, (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0));
        ccHandleCleanUpConCb(con, TRUE, AM_CV_TMPFAIL);
    }
    RETVALUE(ROK);
  }
}


/*
*
*      Fun:   ccConE52S45
*
*      Desc:  Connection state function
*             event - Disconnect
*             state - 45-CCS_AWTRSPFORDPE33
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE52S45
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{

  if ((con == NULLP) || (event == NULLP))
  {
    CCLOGERR("Critical ERROR: Null pointers:%lx, %ld\n", (U32)con, event);
    RETVALUE(RFAILED);
  }

  CCDP(AIN_DBG_LEVEL_1, "ccConE52S45:icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", con->icSuConnId, con->ogSuConnId,
       (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0), con->icBcm.svcHeader.transId);

  ccDeleteBufferedEvent(con, con->ogSuConnId, CCE_TNOANSEXP);

  if (ccCheckEventInBuffer(&(con->ogBcm), CCE_CONCFM))
  {
    /* Drop the Disconnect Response and process the Connect Confirm */
    DP("Dropping the DSC and processing the ConCfm message:%ld, %ld\n",
       con->icSuConnId, con->icBcm.svcHeader.transId);
    ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
    CC_STATE_CHANGE(con, CCS_AWTANS);
    RETVALUE(ROK);
  }
  ccDisconnectCallForIcBCM(con, event);
  RETVALUE(ROK);
}


/*
*
*      Fun:   ccConE37S86
*
*      Desc:  Connection state function
*             event - CCE_ANALYZE_ROUTE
*             state - 86-CCS_AWTRSPFORDPE17
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE37S86
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  SvcHeader_t        *arSvcHeader           = NULLP;
  AnalyzeRoute_t     *pAnalyzeRoute         = NULLP;
  S16                 mgret                 = RFAILED;

  if ((con == NULLP) || (event == NULLP))
  {
    CCLOGERR("Critical ERROR: Null pointers:%lx, %ld\n", (U32)con, event);
    RETVALUE(RFAILED);
  }

  CCDP(AIN_DBG_LEVEL_1, "ccConE37S86: icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", con->icSuConnId, con->ogSuConnId,
       (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0), con->icBcm.svcHeader.transId);

  pAnalyzeRoute = (AnalyzeRoute_t *) event2;
  arSvcHeader = (SvcHeader_t *) event;

  CC_CLR_CALL_FLAG(con->flag, CC_FLAG_CALLQUEUED); /* FID 14457.0 */
  
  ccCutBillingRecordIfNeeded(con, pAnalyzeRoute->scpResponse, OUTTOINC);
  ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
  /* CPDI need restore ConInd event before mapping AR routing info into it */
  ccRestoreEvntHold(con);
  ccExtractAnalyzeRteParam(pAnalyzeRoute, bcm);
  ccExtractSvcHeader(arSvcHeader, bcm, MSG_Analyze_Route);

  ccStopConTmr(con, AIN_O_NO_ANS_TMR);
  ccStopConTmr(con, AIN_T_NO_ANS_TMR);
  /* STOP the AWAIT ANSWER Timer if running
   *
   * changes made to Support Configurable Answer Timer in GCC, RV Suresh, 5.0 
   */
  ccStopConTmr(con, CC_TMR_AWAITANS); 
  ccDeleteBufferedEvent(con, con->icSuConnId, CCE_ONOANSEXP);
  ccDeleteBufferedEvent(con, con->ogSuConnId, CCE_TNOANSEXP);

  ccCleanAnsFraudCall(con);

  ccRestoreReleaseInEvntHold(con);

  if (arSvcHeader->lastTransactionFlag == TRUE)
    ccDeleteBcm(&(con->icBcm));

  con->ogBcm.closeCause = CscRouteNotAvail;
  ccCloseTBcmTransaction(con);
  if ((CC_IS_CONTEXT_PRESENT(con)) && (con->ogRscVal))
  {
    ccQueueSubOnOGCleanUp(con);
    ccChangeCallStateOfAssociatedCalls(&(con->icBcm), CCS_CANNOTPROCESSEVENTS);
    /* CPDI_FIX: need to decide whether received an RelInd from outgoing before, or
       it's a case need initiate a release internally */
    if (CC_IS_REL_IN_EVNT_HOLD(con))
    con->relPend = OUTRELRSPANDROUTE;
    else
    {
       if (con->ccRelEvntHold)
          con->relPend = OUTRELRSPANDROUTE;
       else
       {
          /* put default cause value here */
          con->tempHolderForReleaseCause = AM_CV_NORMCALLCLR;
          con->relPend = OUTRELANDROUTE;
       } 
    }
    CCDP(AIN_DBG_LEVEL_1, "%s: icSu:%ld, ogSu:%ld, suCtxId:%ld,"
         " con->relPend:%d\n", __FUNCTION__, con->icSuConnId, con->ogSuConnId,
         con->mgBlock->mgCtx->suCtxId, con->relPend);
    
    CC_STATE_CHANGE(con, CCS_AWTSWTCFMFORDP);
    con->mgBlock->con = con;
    mgret = ccSendMgiQueue(0, con->mgBlock);
    if (mgret == ROK)
      RETVALUE(ROK);
    else
    {
      CCLOGERR("Error: MGI failed:icSu:%ld, ogSu:%ld, suCtxId:%ld,"
               " icTransId:%ld\n", con->icSuConnId, con->ogSuConnId,
               con->mgBlock->mgCtx->suCtxId, con->icBcm.svcHeader.transId);
      ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
      if (con->ogRscVal)
      {
        ccCloseTBcmTransaction(con);
        con->direction = OUTTOINC;
        if (con->noRspReqd == FALSE) ccSendReleaseRsp(con);
        ccDeallocateResource(con, CC_OUTGOING);
        con->ogRscVal = FALSE;
      }
      cleanUpMgQ(con);
      ccHandleCleanUpConCb(con, TRUE, AM_CV_TMPFAIL);
      RETVALUE(ROK);
    }
  }

  /* Clean outgoing and ReRoute call */
  if (con->ogRscVal)
  {
    ccCloseTBcmTransaction(con);
    con->direction = OUTTOINC;
    if (con->noRspReqd == FALSE) ccSendReleaseRsp(con);
    ccDeallocateResource(con, CC_OUTGOING);
    con->ogRscVal = FALSE;
  }

  /* Route the call */
  initOgBcm(&con->ogBcm);
  ccRouteCall(con, FALSE);
  RETVALUE(ROK);
}


/*
*
*      Fun:   ccConE39S86
*
*      Desc:  Connection state function
*             event - CONTINUE
*             state - 86-CCS_AWTRSPFORDPE17
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE39S86
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  SvcHeader_t        *ctSvcHeader           = NULLP;
  S16                 mgret         = RFAILED, relRstRet = ROK;
  Continue_t        *pContinue              = NULLP;
  CcRelEvnt           ogEvnt;
  ProtType            icProtType    = 0; 
  CcCause             cause;
  TknU16              sipCause;
  TknU16              sipStatus;
  S16                 ret           = RFAILED;

  /* bug 91532 */
  cmMemset((U8 *)&ogEvnt, 0, sizeof(ogEvnt));
  
  if ((con == NULLP) || (event == NULLP))
  {
    CCLOGERR("Critical ERROR: Null pointers:%lx, %ld\n", (U32)con, event);
    RETVALUE(RFAILED);
  }
  /*CID 15594*/
  icProtType    = CC_GET_PROTOCOL(con->icProtType);
  CCDP(AIN_DBG_LEVEL_1, "ccConE39S86:icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", con->icSuConnId, con->ogSuConnId,
       (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0), con->icBcm.svcHeader.transId);

  ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
  ccStopConTmr(con, AIN_O_NO_ANS_TMR);
  ccStopConTmr(con, AIN_T_NO_ANS_TMR);
  /* STOP the AWAIT ANSWER Timer if running
   *
   * changes made to Support Configurable Answer Timer in GCC, RV Suresh, 5.0 
   */
  ccStopConTmr(con, CC_TMR_AWAITANS); 
  ccStopConTmr(con, TMR_SETUP);
  ccDeleteBufferedEvent(con, con->icSuConnId, CCE_ONOANSEXP);
  ccDeleteBufferedEvent(con, con->ogSuConnId, CCE_TNOANSEXP);
#if 1 /* EARLY_ACM */
  ccStopConTmr(con, CC_TMR_EARLYACM);
#endif


  /* CPDI_FIX 28351, 28730, need good way handling relrsp */
  relRstRet = ccRestoreReleaseInEvntHold(con);

  
  ctSvcHeader = (SvcHeader_t *) event;
  pContinue   = (Continue_t *) event2;
  /* Bug# 27816 */
  if(pContinue->errorCode != FC_NOT_APPLICABLE)
  {
    CCLOGERR("ERROR FROM FIC = %ld\n", pContinue->errorCode);

    /* FID: 14457.0 + */
    if ((ccCp.getsEnabled) && (IS_THIS_CALL_GETS(con)) && 
        (pContinue->errorCode == FC_HPC_QUEUE_TIMEOUT))
    {
      ccCp.getsSts.queuedTimeoutTx++;
    } /* FID 14457.0 - */

    con->tempHolderForReleaseCause = pContinue->errorCode;  
    if  (con->icProtType == CC_LOCAL_SWT_PROT)
      ccRemoveCaleaAssnWithTappedBcm(con);

    con->ainInfo.termInd = EXCEPTION_IND;
    con->ogBcm.closeCause = CscRouteNotAvail;
    ccCloseTBcmTransaction(con);

    con->failCndVal = pContinue->errorCode;
    if (CC_IS_CONTEXT_PRESENT(con))
    {
#if 1 /* EARLY_ACM */
      /* Need to clean all the legs in the context */
      CC_RESET_ANY_EARLY_ACM_FLAGS(con);
#endif
      ccQueueSubOnOGCleanUp(con);
      /* 28730 handle it differently */
      if (relRstRet == ROK)
      con->relPend = OUTRELRSPANDINFAILCOND;
      else
         con->relPend = OUTRELANDINFAILCOND;

      CC_STATE_CHANGE(con, CCS_AWTSWTCFM_SENDONEREL);
      con->mgBlock->con = con;
      mgret = ccSendMgiQueue(0, con->mgBlock);
      if (mgret == ROK)
        RETVALUE(ROK);
      else
      {
        CCLOGERR("Error: MGI failed:icSu:%ld, ogSu:%ld, suCtxId:%ld,"
                 " icTransId:%ld\n", con->icSuConnId, con->ogSuConnId,
                 con->mgBlock->mgCtx->suCtxId, con->icBcm.svcHeader.transId);
        cleanUpMgQ(con);
      }
    }
    if (con->ogRscVal)
    {
      con->direction = OUTTOINC;
      if (relRstRet == ROK)
      {
         ccSendReleaseRsp(con);
         ccDeallocateResource(con, CC_OUTGOING);
         con->ogRscVal = FALSE;
      }
      else /* 28730: there is nothing to respond, just request */
      {
        ccFreeTheRscFromCon(con, OUTTOINC); /* This will take care of REL_CFM */
        ccProcessFailCnd(con, con->failCndVal, TRUE);
        RETVALUE(ROK);
      }
    }
    ccProcessFailCnd(con, con->failCndVal, TRUE);
  }
  else
  { /* FIC asks GCC to go ahead with release using the stored relEvnt */ 
    con->icEvnt = (CcAllSdus *) con->ccEvntHold; 
    con->ogEvnt = (CcAllSdus *) &ogEvnt;
    /* 28351, 28730: balancing the conflicts */
    if (relRstRet != ROK)
    {
       /* there is no real event received, the following failcnd check make no sense */
       ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
       RETVALUE(ROK);
    }       
    /* Get protocol release cause */
    ccGetRelCause (con, (con->direction == OUTTOINC?con->ogProtType:con->icProtType),
                   (CcRelEvnt *)con->icEvnt, &cause, &sipCause, &sipStatus);

    if (con->mgBlock->mgCtx && (con->mgBlock->mgCtx->suCtxId != 0))
    {
      ccQSubBasedOnCcn(con);  /* FID 15261.0 */

      if ((con->direction == OUTTOINC) && ((con->reconnectBcm == OBCM)||(con->reconnectBcm == TBCM)) )
      {
        con->relPend = OUTRELRSPANDINREL;
      } 
      else if ((con->direction == OUTTOINC) &&
          ((ccIsLEGSubscriber(&(con->icBcm))) || ((icProtType == CC_CS_TG) &&
          ((CC_CAUSE_IS_VAL(&cause, CCNORMUNSPEC, CSTD_CCITT) == FALSE) &&
          (CC_CAUSE_IS_VAL(&cause, CCCALLCLR, CSTD_CCITT) == FALSE)))))
      {   
        con->relPend = OUTRELRSPANDINTONE;
      } 
      else if (con->direction == OUTTOINC)
      {
        con->relPend = OUTRELRSPANDINREL;
      }
      else
      {
        con->relPend = INRELRSPANDOUTREL;
      }

      CC_STATE_CHANGE(con, CCS_AWTSWTCFM_SENDONEREL);
      con->mgBlock->con = con;
      ccStopConTmr(con, TMR_SETUP);
      ccStartConTmr(TMR_RELEASE, con, (PTR)&ccCp.genCfg);
      ret = ccSendMgiQueue(0, con->mgBlock);
      if (ret != ROK)
      {
        CCLOGERR( "Error: ccSendMgiQueue failed\n");
      }
      else
        RETVALUE(ROK);
    }
    else
    {
      CCDP(AIN_DBG_LEVEL_0, "No Switching Ctx found:%d\n", con->ogProtType);
#ifdef CC_CALEA
      if (con->icProtType == CC_LOCAL_SWT_PROT)
      { 
        if ((con->icBcm.cc != NULLP) && (con->icBcm.cc->tappedLegId != NULLP))
        {
          CCDP(AIN_DBG_LEVEL_0,"Deswitching of CALEA  in Tapped con\n");
          ccProcessTappedLegForCaleaRel(con, con->icBcm.cc->tappedLegId, 
                                        event, CCE_RELIND, sizeof(CcRelEvnt));
          RETVALUE(ROK);
        }
        else
          CCLOGERR( "ERROR: con->ogBcm.cc is NULLP \n");
      }
#endif
    }
    if (con->noRspReqd == FALSE)
      ccSendReleaseRsp(con);

    if (con->direction == OUTTOINC)
    {
        con->failCndVal = ccMapProtRel2FailCnd (con, &cause, &sipCause, 
                                                &sipStatus);
    }

    if (con->direction == INCTOOUT)
    {
      /* Deallocate the IC Resources */
      CC_STATE_CHANGE(con, CCS_AWTRELCFM_OG);
#ifdef GUARD_TIMER
      ccGuardOrDeallocResource(con, CC_INCOMING);
#else
          ccDeallocateResource(con, CC_INCOMING);
#endif
          con->icRscVal = FALSE;
    }
    else if (con->direction == OUTTOINC)
    {
      CC_STATE_CHANGE(con, CCS_AWTRELCFM_IC);
#ifdef GUARD_TIMER
      ccGuardOrDeallocResource(con, CC_OUTGOING);
#else
      ccDeallocateResource(con, CC_OUTGOING);
#endif
      con->ogRscVal = FALSE;
    }
    /* if required, PLAY BUSY Tone or REORDER Tone */
    if (con->direction == OUTTOINC)
    {
        ccStopConTmr(con, TMR_RELEASE);
        ccStartConTmr(TMR_SETUP, con, (PTR)&ccCp.genCfg);
        ret = ccProcessFailCnd (con, con->failCndVal, FALSE);
        if (ret == ROK)
            RETVALUE (ROK);
    }

    ccStopConTmr(con, TMR_SETUP);
    ccStartConTmr(TMR_RELEASE, con, (PTR)&ccCp.genCfg);

    ret = ccMapEvent(con, CCE_RELIND, 0);
    if (ret == ROK)
        ccSendReleaseReq(con);
    else if (con->direction == INCTOOUT)
    {
        
        ccSendRelease(con, con->ogSapCb, con->ogSpConnId, con->ogSuConnId,
                      con->ogProtType, CCPROTERR,NULL);
    }
    else if (con->direction == OUTTOINC)
    {
        
        ccSendRelease(con, con->ogSapCb, con->ogSpConnId, con->ogSuConnId,
                      con->ogProtType, CCPROTERR,NULL);
    }
  }

  RETVALUE(ROK);
}


/* 
*  
*      Fun:   ccConE47S86
*  
*      Desc:  Connection state function
*             event - Send_To_Resource
*             state - 86-CCS_AWTRSPFORDPE17
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
* 
*      Notes: None.
*   
*      File:  cc_bdy2_2.c
*   
*/  
PUBLIC S16 ccConE47S86
(   
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)   
{
  SvcHeader_t       *srSvcHeader           = NULLP;
  S16                ret                   = RFAILED;
  S16                mgret                 = RFAILED;

  if ((con == NULLP) || (event == NULLP))
  {
    CCLOGERR("Critical ERROR: Null pointers:%lx, %ld\n", (U32)con, event);
    RETVALUE(RFAILED);
  }
  CCDP(AIN_DBG_LEVEL_1, "ccConE47S86:icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", con->icSuConnId, con->ogSuConnId,
       (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0), con->icBcm.svcHeader.transId);

  ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
  srSvcHeader = (SvcHeader_t *) event;

  ccStopConTmr(con, AIN_O_NO_ANS_TMR);
  ccStopConTmr(con, AIN_T_NO_ANS_TMR);
  /* STOP the AWAIT ANSWER Timer if running
   *
   * changes made to Support Configurable Answer Timer in GCC, RV Suresh, 5.0 
   */
  ccStopConTmr(con, CC_TMR_AWAITANS); 
  ccDeleteBufferedEvent(con, con->icSuConnId, CCE_ONOANSEXP);
  ccDeleteBufferedEvent(con, con->ogSuConnId, CCE_TNOANSEXP);
#if 1 /* EARLY_ACM */
  ccStopConTmr(con, CC_TMR_EARLYACM); 
#endif

  ccRestoreReleaseInEvntHold(con);

  if (srSvcHeader->lastTransactionFlag == TRUE)
    ccDeleteBcm(&(con->icBcm));

  con->ogBcm.closeCause = CscRouteNotAvail;
  ccCloseTBcmTransaction(con);

  con->lastState = con->state;
  con->anncBlock.stateForAnnc = con->state;

  if (CC_IS_CONTEXT_PRESENT(con))
  {
#if 1 /* EARLY_ACM */
    /* Need to clean all the legs in the context */
    CC_RESET_ANY_EARLY_ACM_FLAGS(con);
#endif

    ccQueueSubOnOGCleanUp(con);
    ccChangeCallStateOfAssociatedCalls(&(con->icBcm), CCS_CANNOTPROCESSEVENTS);
    /* CPDI_FIX: need to decide whether received an RelInd from outgoing before, or
       it's a case need initiate a release internally */
    if (CC_IS_REL_IN_EVNT_HOLD(con))
       con->relPend = OUTRELRSPANDPLAYANNC;
    else
    {
       if (con->ccRelEvntHold)
    con->relPend = OUTRELRSPANDPLAYANNC;
       else
       {
          /* put default cause value here */
          con->tempHolderForReleaseCause = AM_CV_NORMCALLCLR;
          con->relPend = OUTRELANDPLAYANNC;;
       } 
    }
    CC_STATE_CHANGE(con, CCS_AWTSWTCFMFORDP);
    con->mgBlock->con = con;
    mgret = ccSendMgiQueue(0, con->mgBlock);
    if (mgret == ROK)
      RETVALUE(ROK);
    else
    {
      CCLOGERR("Error: MGI failed:icSu:%ld, ogSu:%ld, suCtxId:%ld,"
               " icTransId:%ld\n", con->icSuConnId, con->ogSuConnId,
               con->mgBlock->mgCtx->suCtxId, con->icBcm.svcHeader.transId);
      ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
      con->direction = OUTTOINC;
      if (con->noRspReqd == FALSE) ccSendReleaseRsp(con);
      ccDeallocateResource(con, CC_OUTGOING);
      con->ogRscVal = FALSE;
      cleanUpMgQ(con);
      ccHandleCleanUpConCb(con, TRUE, AM_CV_TMPFAIL);
      RETVALUE(ROK);
    }
  }

  con->direction = OUTTOINC;
  if (con->noRspReqd == FALSE) ccSendReleaseRsp(con);
  ccDeallocateResource(con, CC_OUTGOING);
  con->ogRscVal = FALSE;

  initOgBcm(&con->ogBcm);
  ret = ccSendToResource(&(con->icBcm));
  if (ret != ROK)
  {
    CCLOGERR("Error: ccSendToResource failed:icSu:%ld, ogSu:%ld, icTrsId:%ld\n",
             con->icSuConnId, con->ogSuConnId, (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0));
    ccHandleCleanUpConCb(con, TRUE, AM_CV_TMPFAIL);
  }
  RETVALUE(ROK);
}


/*
*
*      Fun:   ccConE52S86
*
*      Desc:  Connection state function
*             event - Disconnect
*             state - 86-CCS_AWTRSPFORDPE33
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE52S86
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{

  if ((con == NULLP) || (event == NULLP))
  {
    CCLOGERR("Critical ERROR: Null pointers:%lx, %ld\n", (U32)con, event);
    RETVALUE(RFAILED);
  }

  CCDP(AIN_DBG_LEVEL_1, "ccConE52S86:icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", con->icSuConnId, con->ogSuConnId,
       (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0), con->icBcm.svcHeader.transId);

  con->ogBcm.closeCause = CscRouteNotAvail;
  ccCloseTBcmTransaction(con);

  ccRestoreReleaseInEvntHold(con);
  ccDisconnectCallForIcBCM(con, event);
  RETVALUE(ROK);
}


/*  
*   
*      Fun:   ccConE11S87    
*   
*      Desc:  Connection state function
*             event - MGI confirm.
*             state - 87-CCS_AWTSWTCFMFORDP
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*   
*      Notes: None.          
*   
*      File:  cc_bdy2_2.c        
*   
*/  
PUBLIC S16 ccConE11S87
(   
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)   
{
  MgctTxnCfm       *txnCfm         = NULLP;
  S16               ret            = RFAILED;
  U8                relDisBool     = RFAILED;
  U8                relDisBoolOG   = RFAILED;
  U8                evntType1      = 0;
  U8                evntType2      = 0;

  if ((con == NULLP) || (event == NULLP))
  {
    CCLOGERR("Critical ERROR: Null pointers:%lx, %ld\n", (U32)con, event);
    RETVALUE(RFAILED);
  }
  CCDP(AIN_DBG_LEVEL_1, "ccConE11S87:icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", con->icSuConnId, con->ogSuConnId,
       con->mgBlock->mgCtx->suCtxId, con->icBcm.svcHeader.transId);

  ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
  txnCfm   = (MgctTxnCfm*) event;
  ret = ccHandleMgResponse(con, txnCfm);

  /* FID 15261.0 + */
  if (CC_IS_CALL_MULTI_MG(con))
  {
    if (con->ccmCb.mgPend)
      con->ccmCb.mgPend--;

    if (ret != ROK)
    {
      con->ccmCb.flags |= CC_MG_SUB_ERROR;
    }

    if (con->ccmCb.mgPend)
    {
      RETVALUE(ROK);
    }
  }
  /* FID 15261.0 - */

  if ((ret != ROK) || (con->ccmCb.flags & CC_MG_SUB_ERROR))
  {
    CCLOGERR( "ccHandleResponse Failed:%ld\n", con->icSuConnId);
    cleanUpMgQ(con);
    ccHandleCleanUpConCb(con, TRUE, AM_CV_TMPFAIL);
    RETVALUE(ROK);
  }

  relDisBool = ccCheckRelOrOnHookInBuffer(&(con->icBcm), &evntType1);
  relDisBoolOG = ccCheckRelOrOnHookInBuffer(&(con->ogBcm), &evntType2);

  switch(con->relPend)
  {
    case OUTRELRSPANDROUTE:
      con->direction = OUTTOINC;
      if (con->noRspReqd == FALSE) ccSendReleaseRsp(con);
      ccDeallocateResource(con, CC_OUTGOING);
      /* BUG80669 ogLiInfo lost in rereoute call */
      if ((con->liInfo) && (con->liInfo->tmpLiInfo))
      {
        if ((con->liInfo) && (con->liInfo->ogLiInfo[0]))
        {
          CCLOGERR("Error: ogLiInfo was not NULLP\n");
          ABORT_DEBUG;
          ccCleanUpLiInfoLeg(con->liInfo->ogLiInfo, &con->ogBcm, con, LI_INVALID_LI_INDX);
        }
        CCDP(AIN_DBG_LEVEL_2,"Move back ogLiInfo \n");
        con->liInfo->ogLiInfo[0]  = con->liInfo->tmpLiInfo;
        con->liInfo->tmpLiInfo = NULLP;
        CC_CLEAR_LI_BITMASK(con,OgSipIriCfm);
      }
      con->ogRscVal = FALSE;
      initOgBcm(&con->ogBcm);
      ccRouteCall(con, FALSE);
      break;
    case OUTRELRSPANDROUTEFORMGADVANCE:
      con->direction = OUTTOINC;
      if (con->noRspReqd == FALSE) ccSendReleaseRsp(con);
      ccDeallocateResource(con, CC_OUTGOING);
      /* BUG80669 ogLiInfo lost in rereoute call */
      if ((con->liInfo) && (con->liInfo->tmpLiInfo))
      {
        if ((con->liInfo) && (con->liInfo->ogLiInfo[0]))
        {
          CCLOGERR("Error: ogLiInfo was not NULLP\n");
          ABORT_DEBUG;
          ccCleanUpLiInfoLeg(con->liInfo->ogLiInfo, &con->ogBcm, con, LI_INVALID_LI_INDX);
        }
        CCDP(AIN_DBG_LEVEL_2,"Move back ogLiInfo \n");
        con->liInfo->ogLiInfo[0]  = con->liInfo->tmpLiInfo;
        con->liInfo->tmpLiInfo = NULLP;
        CC_CLEAR_LI_BITMASK(con,OgSipIriCfm);
      }
      con->ogRscVal = FALSE;
      initOgBcm(&con->ogBcm);
      if ((con) && (con->routeEvnt))
      {
        con->routeEvnt->cacInfo[CC_EGRESS_INTER_CCS_CALL(con)? \
        /* BUG: 78856 */   con->routeEvnt->currentInterface:   \
                           con->routeEvnt->currentInterface -1].currentMgw++;
        /* + BUG: 71477 */
        if (con->failCndVal != FC_MG_OOS)
        {
            con->routeEvnt->cacInfo[CC_EGRESS_INTER_CCS_CALL(con)? \
        /* BUG: 78856 */       con->routeEvnt->currentInterface:   \
                               con->routeEvnt->currentInterface -1].mgwCount++;
        }
        /* - BUG: 71477 */
        ccRouteCall(con, TRUE);
      }
      else
      {
        CC_TRACE_DUMP(con);
        ccProcessFailCnd(con, FC_INTERNAL, TRUE);
      }
      break;
    /* FID 14457.0 +*/
    case OUTRELRSPANDROUTEFORICMGADVANCE:
      con->direction = OUTTOINC;
      if (con->noRspReqd == FALSE) ccSendReleaseRsp(con);
      ccDeallocateResource(con, CC_OUTGOING);
      con->ogRscVal = FALSE;
      initOgBcm(&con->ogBcm);
      if ((con) && (con->routeEvnt))
      {
        con->routeEvnt->inCacInfo.currentMgw++;
        /* FID 14457.0 Ingress MG list MgAdvance, increment ingress MgAdvance count */
        con->ccmCb.ingMgCount++;
        ccRouteCall(con, TRUE);
      }
      else
      {
        CC_TRACE_DUMP(con);
        ccProcessFailCnd(con, FC_INTERNAL, TRUE);
      }
      break;
    case OUTRELHPCMGRETRY:
      if (con->ogRscVal) 
      {
        ccDeallocateResource(con, CC_OUTGOING);
        con->ogRscVal = FALSE;
      }
      ccStartConTmr(CC_TMR_HPC_MG_RETRY, con, (PTR)&ccCp);
      CC_STATE_CHANGE(con, CCS_AWTHPCMGRETRYTIMEREXPRIY);
      RETVALUE(ROK);
      break;
    /* FID 14457.0 -*/
    case OUTRELRSPANDPLAYANNC:
      con->direction = OUTTOINC;
      if (con->noRspReqd == FALSE) ccSendReleaseRsp(con);
      ccDeallocateResource(con, CC_OUTGOING);
      con->ogRscVal = FALSE;
      initOgBcm(&con->ogBcm);
      ret = ccSendToResource(&(con->icBcm));
      if (ret != ROK)
      {
        CCLOGERR("Error: ccSendToResource failed:icSu:%ld,ogSu:%ld,icTrsId:%ld\n",
                 con->icSuConnId, con->ogSuConnId, 
                 (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0));
        ccHandleCleanUpConCb(con, TRUE, AM_CV_TMPFAIL);
      }
      break;
    case NORELANDPLAYANNC:
      initOgBcm(&con->ogBcm);
      ret = ccSendToResource(&(con->icBcm));
      if (ret != ROK)
      {
        CCLOGERR("Error: ccSendToResource failed:icSu:%ld,ogSu:%ld,icTrsId:%ld\n",
                 con->icSuConnId, con->ogSuConnId, 
                 (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0));
        ccHandleCleanUpConCb(con, TRUE, AM_CV_TMPFAIL);
      }
      break;
    case OUTRELANDROUTE:
      if (relDisBoolOG == ROK)
      {
        ccDeleteBufferedEvent(con, con->ogSuConnId, evntType2);
        con->direction = OUTTOINC;
        if (con->noRspReqd == FALSE) ccSendReleaseRsp(con);
        ccDeallocateResource(con, CC_OUTGOING);
        con->ogRscVal = FALSE;
        initOgBcm(&con->ogBcm);
        ccRouteCall(con, FALSE);
      }
      else
      {
        ccFreeTheRscFromCon(con, OUTTOINC); /* This will take care of REL_CFM */
        initOgBcm(&con->ogBcm);
        if(ROK == ccCheckForInterfaces(con))
        ccRouteCall(con, FALSE);
        else
        {
         CC_TRACE_DUMP(con);
         ccProcessFailCnd(con, FC_INTERNAL, TRUE);
        }
      }
      break;
    case OUTRELANDPLAYANNC:
      if (relDisBoolOG == ROK)
      {
        ccDeleteBufferedEvent(con, con->ogSuConnId, evntType2);
        con->direction = OUTTOINC;
        if (con->noRspReqd == FALSE) ccSendReleaseRsp(con);
        ccDeallocateResource(con, CC_OUTGOING);
        con->ogRscVal = FALSE;
        initOgBcm(&con->ogBcm);
        ret = ccSendToResource(&(con->icBcm));
        if (ret != ROK)
        {
          CCLOGERR("Error: ccSendToResource failed: icSu:%ld, ogSu:%ld,"
                   " icTrsId:%ld\n", con->icSuConnId, con->ogSuConnId,
                   (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0));
          ccHandleCleanUpConCb(con, TRUE, AM_CV_TMPFAIL);
        }
      }
      else
      {
        if (con->ogRscVal)/* BUG:92277. Don't free resource if it not allocataed */
          ccFreeTheRscFromCon(con, OUTTOINC); /* This will take care of REL_CFM */
        initOgBcm(&con->ogBcm);
        ret = ccSendToResource(&(con->icBcm));
        if (ret != ROK)
        {
          CCLOGERR("Error:ccSendToResource failed:icSu:%ld, icTrsId:%ld\n",
              con->icSuConnId, 
              (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0));
          ccHandleCleanUpConCb(con, TRUE, AM_CV_TMPFAIL);
        }
      }
      break;
    /* + FID !5083.0 */
    case CONTINUEWITHCALLWITHOUTLI:
      /* Lets retry without X3 */
      ccReconfigureCCNPostLiSub(con, TRUE); /*This will derive the CCN*/
      CCDP(AIN_DBG_LEVEL_1, "%s: set icIpParam & ogIpParam locBearerIpPort to 0\n",
                       __FUNCTION__);
      CC_SET_ZERO_BEARER_IP_PORT(&con->icIpParam.locBearerIpPort);
      CC_SET_ZERO_BEARER_IP_PORT(&con->ogIpParam.locBearerIpPort);

      ccCleanMgCtxFromBcm(&(con->icBcm));
      ccCleanMgCtxFromBcm(&(con->ogBcm));
      if (con->ccmCb.ccn == CCN_1)
      {
        ccInitAdj(BtOAdjBcm, &con->icAdjBcm, &con->icAdjIpParam, con, con->icBcm.mgSpId);
        ccInitAdj(BtTAdjBcm, &con->ogAdjBcm, &con->ogAdjIpParam, con, con->ogBcm.mgSpId);
      }
      ccProcessPostAuthorizeTermination(con);
      break;
    /* + FID 15083.0  BUG: 83045 */
    case CONTINUEWITHREATTEMPT:
      if ((CC_IS_CALL_TAPPED(con)) && (CC_IS_X3_ALLOWED(con)) &&/* BUG: 84409 */
          (CC_IS_ANY_LI_TARGET_STEREO(con)) && /* FID 15083.1 */
          ((con->ccmCb.ccn == CCN_4) || (con->ccmCb.ccn == CCN_2)))
      {
         ccClearLiTermPostLiSub(con);
      }
      ccProcessReAttempt(con, (PTR)con->ccEvntHold, NULLP, NULLP);
      break;
    case CONTINUEWITHREATTEMPT1:
      CC_STATE_CHANGE(con, CCS_AWTANS);

      if ((CC_IS_CALL_TAPPED(con)) && (CC_IS_X3_ALLOWED(con)) &&
          (CC_IS_ANY_LI_TARGET_STEREO(con)) && /* FID 15083.1 */
          ((con->ccmCb.ccn == CCN_4) || (con->ccmCb.ccn == CCN_2)))
      {
         ccProcessAddLi(con, (GccCallBakFunc)ccSendConReqForOgIpCalls, 
                        (PTR) con, con->state);
      }
      else
      {
         ccSendConReqForOgIpCalls(con);
      }
      break;
    /* - FID 15083.0  BUG: 83045 */
    default:
      CCLOGERR("Error: This relPend value is not handled:icSu:%ld, ogSu:%ld,"
               " suCtxId:%ld, icTransId:%ld, relPend:%d\n", con->icSuConnId,
               con->ogSuConnId, (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0),
               con->icBcm.svcHeader.transId, con->relPend);
      RETVALUE(RFAILED);
  }
  RETVALUE(ROK);
}


/*
*
*      Fun:   ccConE14S87
*
*      Desc:  Connection state function
*             event - Setup Timer expiry.
*             state - 87-CCS_AWTSWTCFMFORDP
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE14S87
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  CCLOGERR("Error: Setup Timer expired: ccConE14S87:icSu:%ld, ogSu:%ld\n",
           con->icSuConnId, con->ogSuConnId);

  ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));

  switch(con->relPend)
  {
    case OUTRELRSPANDROUTE:
    case OUTRELRSPANDPLAYANNC:
    if (con->noRspReqd == FALSE) ccSendReleaseRsp(con);
      ccDeallocateResource(con, CC_OUTGOING);
      con->ogRscVal = FALSE;
      break;
    case OUTRELANDROUTE:
    case OUTRELANDPLAYANNC:
      break;
    default:
      CCLOGERR("Error: Invalid relPend value = %d, icSu:%ld\n",
               con->relPend, con->icSuConnId);
    break;
  }
  ccHandleCleanUpConCb(con, TRUE, AM_CV_TMPFAIL);
  RETVALUE(ROK);
}


/*
*
*      Fun:   ccConE15S03
*
*      Desc:  Connection state function
*             event - Release Timer expiry.
*             state - 03-CCS_AWTRSPODISCONNECT
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.             
*
*      File:  cc_bdy2_2.c       
*
*/
PUBLIC S16 ccConE15S03
(                               
CcConCb *con,                    /* connection */ 
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
    CCLOGERR("Error: Release timer expired: ccConE15S03:icSu:%ld, ogSu:%ld\n",
             con->icSuConnId, con->ogSuConnId);

    if (con->ogRscVal)
    {
        con->direction = OUTTOINC;
        if (con->noRspReqd == FALSE) ccSendReleaseRsp(con);
        ccDeallocateResource(con, CC_OUTGOING);
        con->ogRscVal = FALSE;
    }
    ccHandleCleanUpConCb(con, TRUE, AM_CV_TMPFAIL);
    RETVALUE(ROK);
} /* ccConE15S03 */



/*
*
*      Fun:   ccConE39S79
*
*      Desc:  Connection state function
*             event - Continue
*             state - 79-CCS_AWT_FIC_RSP_TO_KILL
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.             
*
*      File:  cc_bdy2_2.c       
*
*/
PUBLIC S16 ccConE39S79
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
#ifdef ZC
  ZcUpdParam upd;
#endif /* ZC */
  CCDP(AIN_DBG_LEVEL_1, "ccConE39S79:%ld,%ld,%ld\n", con->icSuConnId,
                         con->ogSuConnId, (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0));
  ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
#ifdef ZC
  if (CC_IS_OBCM_PIC_ACTIVE(con))
  {
    cmMemset((U8 *)&upd, 0, sizeof(ZcUpdParam));
    upd.featMsg.suConnId     = con->icSuConnId; /*ACT Controlling leg connId*/
    upd.featMsg.ccAinUpdType = CC_KILL_CALL;
    zcRunTimeUpd(ZC_FEAT, CMPFTHA_UPD_REQ, upd);
    zcUpdPeer();
  }
#endif
  ccKillCallOnThisCon(con);
  RETVALUE(ROK);
}


/*
*
*      Fun:   ccConE40S79
*
*      Desc:  Connection state function
*             event - Disconnect Leg
*             state - 79-CCS_AWT_FIC_RSP_TO_KILL
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE40S79
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
#ifdef ZC
  ZcUpdParam upd;
#endif /* ZC */
  CCDP(AIN_DBG_LEVEL_1, "ccConE40S79:%ld,%ld,%ld\n", con->icSuConnId,
                         con->ogSuConnId, (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0));
  ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
#ifdef ZC
  if (CC_IS_OBCM_PIC_ACTIVE(con))
  {
    cmMemset((U8 *)&upd, 0, sizeof(ZcUpdParam));
    upd.featMsg.suConnId     = con->icSuConnId; /*ACT Controlling leg connId*/
    upd.featMsg.ccAinUpdType = CC_KILL_CALL;
    zcRunTimeUpd(ZC_FEAT, CMPFTHA_UPD_REQ, upd);
    zcUpdPeer();
  }
#endif
  ccKillCallOnThisCon(con);
  RETVALUE(ROK);
}
/*
*
*      Fun:   ccConE41S79
*
*      Desc:  Connection state function
*             event - Forward Call
*             state - 79-CCS_AWT_FIC_RSP_TO_KILL
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE41S79
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
#ifdef ZC
  ZcUpdParam upd;
#endif /* ZC */
  CCDP(AIN_DBG_LEVEL_1, "ccConE41S79:%ld,%ld,%ld\n", con->icSuConnId,
                         con->ogSuConnId, (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0));
  ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
#ifdef ZC
  if (CC_IS_OBCM_PIC_ACTIVE(con))
  {
    cmMemset((U8 *)&upd, 0, sizeof(ZcUpdParam));
    upd.featMsg.suConnId     = con->icSuConnId; /*ACT Controlling leg connId*/
    upd.featMsg.ccAinUpdType = CC_KILL_CALL;
    zcRunTimeUpd(ZC_FEAT, CMPFTHA_UPD_REQ, upd);
    zcUpdPeer();
  }
#endif
  ccKillCallOnThisCon(con);
  RETVALUE(ROK);
}

/*
*
*      Fun:   ccConE42S79
*
*      Desc:  Connection state function
*             event - Merge Call
*             state - 79-CCS_AWT_FIC_RSP_TO_KILL
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE42S79
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
#ifdef ZC
  ZcUpdParam upd;
#endif /* ZC */
  CCDP(AIN_DBG_LEVEL_1, "ccConE42S79:%ld,%ld,%ld\n", con->icSuConnId,
                         con->ogSuConnId, (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0));
  ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
#ifdef ZC
  if (CC_IS_OBCM_PIC_ACTIVE(con))
  {
    cmMemset((U8 *)&upd, 0, sizeof(ZcUpdParam));
    upd.featMsg.suConnId     = con->icSuConnId; /*ACT Controlling leg connId*/
    upd.featMsg.ccAinUpdType = CC_KILL_CALL;
    zcRunTimeUpd(ZC_FEAT, CMPFTHA_UPD_REQ, upd);
    zcUpdPeer();
  }
#endif
  ccKillCallOnThisCon(con);
  RETVALUE(ROK);
}

/*
*
*      Fun:   ccConE43S79
*
*      Desc:  Connection state function
*             event - Move Leg
*             state - 79-CCS_AWT_FIC_RSP_TO_KILL
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE43S79
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
#ifdef ZC
  ZcUpdParam upd;
#endif /* ZC */
  CCDP(AIN_DBG_LEVEL_1, "ccConE43S79:%ld,%ld,%ld\n", con->icSuConnId,
                         con->ogSuConnId, (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0));
  ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
#ifdef ZC
  if (CC_IS_OBCM_PIC_ACTIVE(con))
  {
    cmMemset((U8 *)&upd, 0, sizeof(ZcUpdParam));
    upd.featMsg.suConnId     = con->icSuConnId; /*ACT Controlling leg connId*/
    upd.featMsg.ccAinUpdType = CC_KILL_CALL;
    zcRunTimeUpd(ZC_FEAT, CMPFTHA_UPD_REQ, upd);
    zcUpdPeer();
  }
#endif
  ccKillCallOnThisCon(con);
  RETVALUE(ROK);
}

/*
*
*      Fun:   ccConE44S79
*
*      Desc:  Connection state function
*             event - Offer Call
*             state - 79-CCS_AWT_FIC_RSP_TO_KILL
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE44S79
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
#ifdef ZC
  ZcUpdParam upd;
#endif /* ZC */
  CCDP(AIN_DBG_LEVEL_1, "ccConE44S79:%ld,%ld,%ld\n", con->icSuConnId,
                         con->ogSuConnId, (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0));
  ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
#ifdef ZC
  if (CC_IS_OBCM_PIC_ACTIVE(con))
  {
    cmMemset((U8 *)&upd, 0, sizeof(ZcUpdParam));
    upd.featMsg.suConnId     = con->icSuConnId; /*ACT Controlling leg connId*/
    upd.featMsg.ccAinUpdType = CC_KILL_CALL;
    zcRunTimeUpd(ZC_FEAT, CMPFTHA_UPD_REQ, upd);
    zcUpdPeer();
  }
#endif
  ccKillCallOnThisCon(con);
  RETVALUE(ROK);
}

/*
*
*      Fun:   ccConE45S79
*
*      Desc:  Connection state function
*             event - Originate Call
*             state - 79-CCS_AWT_FIC_RSP_TO_KILL
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE45S79
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
#ifdef ZC
  ZcUpdParam upd;
#endif /* ZC */
  CCDP(AIN_DBG_LEVEL_1, "ccConE45S79:%ld,%ld,%ld\n", con->icSuConnId,
                         con->ogSuConnId, (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0));
  ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
#ifdef ZC
  if (CC_IS_OBCM_PIC_ACTIVE(con))
  {
    cmMemset((U8 *)&upd, 0, sizeof(ZcUpdParam));
    upd.featMsg.suConnId     = con->icSuConnId; /*ACT Controlling leg connId*/
    upd.featMsg.ccAinUpdType = CC_KILL_CALL;
    zcRunTimeUpd(ZC_FEAT, CMPFTHA_UPD_REQ, upd);
    zcUpdPeer();
  }
#endif
  ccKillCallOnThisCon(con);
  RETVALUE(ROK);
}

/*
*
*      Fun:   ccConE46S79
*
*      Desc:  Connection state function
*             event - Reconnect
*             state - 79-CCS_AWT_FIC_RSP_TO_KILL
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE46S79
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
#ifdef ZC
  ZcUpdParam upd;
#endif /* ZC */
  CCDP(AIN_DBG_LEVEL_1, "ccConE46S79:%ld,%ld,%ld\n", con->icSuConnId,
                         con->ogSuConnId, (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0));
  ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
#ifdef ZC
  if (CC_IS_OBCM_PIC_ACTIVE(con))
  {
    cmMemset((U8 *)&upd, 0, sizeof(ZcUpdParam));
    upd.featMsg.suConnId     = con->icSuConnId; /*ACT Controlling leg connId*/
    upd.featMsg.ccAinUpdType = CC_KILL_CALL;
    zcRunTimeUpd(ZC_FEAT, CMPFTHA_UPD_REQ, upd);
    zcUpdPeer();
  }
#endif
  ccKillCallOnThisCon(con);
  RETVALUE(ROK);
}

/*
*
*      Fun:   ccConE50S79
*
*      Desc:  Connection state function
*             event - T_NO_ANSWER Tmr Expiry
*             state - 79-CCS_AWT_FIC_RSP_TO_KILL
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE50S79
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
#ifdef ZC
  ZcUpdParam upd;
#endif /* ZC */
  CCDP(AIN_DBG_LEVEL_1, "ccConE50S79:%ld,%ld,%ld\n", con->icSuConnId,
                         con->ogSuConnId, (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0));
  ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
#ifdef ZC
  if (CC_IS_OBCM_PIC_ACTIVE(con))
  {
    cmMemset((U8 *)&upd, 0, sizeof(ZcUpdParam));
    upd.featMsg.suConnId     = con->icSuConnId; /*ACT Controlling leg connId*/
    upd.featMsg.ccAinUpdType = CC_KILL_CALL;
    zcRunTimeUpd(ZC_FEAT, CMPFTHA_UPD_REQ, upd);
    zcUpdPeer();
  }
#endif
  ccKillCallOnThisCon(con);
  RETVALUE(ROK);
}

/*
*
*      Fun:   ccConE51S79
*
*      Desc:  Connection state function
*             event - 0_NO_ANSWER Tmr Expiry
*             state - 79-CCS_AWT_FIC_RSP_TO_KILL
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE51S79
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
#ifdef ZC
  ZcUpdParam upd;
#endif /* ZC */
  CCDP(AIN_DBG_LEVEL_1, "ccConE51S79:%ld,%ld,%ld\n", con->icSuConnId,
                         con->ogSuConnId, (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0));
  ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
#ifdef ZC
  if (CC_IS_OBCM_PIC_ACTIVE(con))
  {
    cmMemset((U8 *)&upd, 0, sizeof(ZcUpdParam));
    upd.featMsg.suConnId     = con->icSuConnId; /*ACT Controlling leg connId*/
    upd.featMsg.ccAinUpdType = CC_KILL_CALL;
    zcRunTimeUpd(ZC_FEAT, CMPFTHA_UPD_REQ, upd);
    zcUpdPeer();
  }
#endif
  ccKillCallOnThisCon(con);
  RETVALUE(ROK);
}

/*
*
*      Fun:   ccConE52S79
*
*      Desc:  Connection state function
*             event - Disconnect
*             state - 79-CCS_AWT_FIC_RSP_TO_KILL
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE52S79
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
#ifdef ZC
  ZcUpdParam upd;
#endif /* ZC */
  CCDP(AIN_DBG_LEVEL_1, "ccConE52S79:%ld,%ld,%ld\n", con->icSuConnId,
                         con->ogSuConnId, (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0));
  ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
#ifdef ZC
  if (CC_IS_OBCM_PIC_ACTIVE(con))
  {
    cmMemset((U8 *)&upd, 0, sizeof(ZcUpdParam));
    upd.featMsg.suConnId     = con->icSuConnId; /*ACT Controlling leg connId*/
    upd.featMsg.ccAinUpdType = CC_KILL_CALL;
    zcRunTimeUpd(ZC_FEAT, CMPFTHA_UPD_REQ, upd);
    zcUpdPeer();
  }
#endif
  ccKillCallOnThisCon(con);
  RETVALUE(ROK);
}

/*
*
*      Fun:   ccConE31S32
*
*      Desc:  Connection state function
*             event - Mgi Svc Ind.
*             state - CCS_LOCALLYANSWERED
*
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2.c
*
*/
PUBLIC S16 ccConE31S32
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
#if 0 /* PENDING */
#ifdef ZC
  ZcUpdParam upd;
#endif /* ZC */

  CCDP(AIN_DBG_LEVEL_1, "ccConE31S32:%ld,%ld,%ld\n", con->icSuConnId,
                        con->ogSuConnId, con->mgBlock->mgCtx->suCtxId);
#ifdef ZC
  cmMemset((U8 *)&upd, 0, sizeof(ZcUpdParam));
  upd.featMsg.suConnId     = con->icSuConnId; /*ACT Controlling leg connId*/
  upd.featMsg.ccAinUpdType = CC_KILL_CALL;
  zcRunTimeUpd(ZC_FEAT, CMPFTHA_UPD_REQ, upd);
  zcUpdPeer();
#endif
  ccKillCallOnThisCon(con);
#endif /* PENDING */
  RETVALUE(ROK);
} /* ccConE31S10 */

/* CPDI prepaid */

/* 
*  
*      Fun:   ccConE03S82
*  
*      Desc:  Connection state function
*             event - Connection progress status indication
*             state - 
* 
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes:
*   
*      File:  cc_bdy2_2.c
*   
*/  
PUBLIC  S16 ccConE03S82
(   
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)   
{
  /* euysal - 05/27/2005 - ACC Support */
   S16       ret;
   AinBCM_t *bcmRsp = NULLP;
   
  CCDP(AIN_DBG_LEVEL_1, "ccConE03S82: event = %d, state=%d,icSu:%ld, ogSu:%ld\n", 
       con->evntType, con->state, con->icSuConnId, con->ogSuConnId);

  bcmRsp = (con->direction == INCTOOUT) ? (&con->icBcm) : (&con->ogBcm);

  /* euysal - 05/27/2005 - ACC Support */
  if (((ccIsOnHookRcvd(con->icProtType, con->ogProtType, con->icEvntType, con->direction)) && 
       (con->direction == OUTTOINC)) || 
      ((con->icEvntType == CC_ET_SUSP) && (con->direction == OUTTOINC)))
  {
#if 0     
     CC_UPDATE_PIC(&con->ogBcm, PicTSuspended);
     CC_UPDATE_PIC(&con->icBcm, PicOSuspended);
#endif     
     ret = ccProcessOSuspended (&(con->icBcm));
     if (ret == ROK)
     {
        CCDP(AIN_DBG_LEVEL_3, "OSuspended requested event sent to FIC\n");
        RETVALUE(ROK);
     }   
     else if ( ret == RFAILED)
     {
        CCDP(AIN_DBG_LEVEL_3, "ccProcessOSuspended failed\n");
     } 
     else
        CCDP(AIN_DBG_LEVEL_3, "OSuspended not armed as a requested event\n");
             
  }
  else if (ccIsXferRcvd(CC_GET_PROTOCOL(con->icProtType),
           CC_GET_PROTOCOL(con->ogProtType), con->icEvntType, con->direction))
  {
      ccSendXferRsp(bcmRsp, PRSNT_NODEF, SIPCAUSE_480_TEMPUNV);
      RETVALUE(ROK);
  }
  else if (ccIsXferQryRcvd(CC_GET_PROTOCOL(con->icProtType),
           CC_GET_PROTOCOL(con->ogProtType), con->icEvntType, con->direction))
  {
      ccSendQueryRsp(bcmRsp, PRSNT_NODEF, SIPCAUSE_480_TEMPUNV);
      RETVALUE(ROK);
  }
  else if (ccIsUpdateRcvd(CC_GET_PROTOCOL(con->icProtType),
           CC_GET_PROTOCOL(con->ogProtType), con->icEvntType, con->direction))
  {
      ccSendUpdateRsp(bcmRsp, PRSNT_NODEF, SIPCAUSE_480_TEMPUNV);
      RETVALUE(ROK);
  }
  ccBufferEventToQueue(con, bcm, event, CCE_CNSTIND, sizeof(CcCnStEvnt), 0, 
                       NULLP, 0);
  RETVALUE(ROK);
}


/* euysal - 05/26/2005 - ACC Support */
#if 1
/* 
*  
*      Fun:   ccConE04S82
*  
*      Desc:  Connection state function
*             event - Release indication
*             state - 
* 
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes:
*   
*      File:  cc_bdy2_2.c
*   
*/  
PUBLIC  S16 ccConE04S82
(   
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)   
{   
  /* euysal - 05/27/2005 - ACC Support */
   S16 ret;
   MgctEventsDescriptor emptyEvDsc;
   MgctSignalsDescriptor emptySigDsc;
   /* BUG 93121 */
   AinBCM_t     *pTmpIcBcm = NULLP;
   AinBCM_t     *pTmpOgBcm = NULLP;

   CCDP(AIN_DBG_LEVEL_1, "ccConE04S82: event = %d, state=%d,icSu:%ld, ogSu:%ld\n", 
        con->evntType, con->state, con->icSuConnId, con->ogSuConnId);
   
   /* if we have already received a RELEASE buffer the second release */
   if ((con->relPend == ABANDONEDANNC_IC) || (con->relPend == ABANDONEDANNC_OG))
   {   
      ccBufferEventToQueue(con, bcm, event, CCE_RELIND, sizeof(CcRelEvnt), 0,
                             NULLP, 0);
      RETVALUE(ROK);
   }
   /* Set the relPend value according to the side we received the release */
   if (con->direction == OUTTOINC)
      con->relPend = ABANDONEDANNC_OG;
   else
      con->relPend = ABANDONEDANNC_IC;

   /*Bug:46506: prepare Empty Event descriptor + */
   cmMemset((U8*) &emptyEvDsc, 0, sizeof(MgctEventsDescriptor));
   emptyEvDsc.pres = TRUE;
   cmMemset((U8*) &emptySigDsc, 0, sizeof(MgctSignalsDescriptor));
   emptySigDsc.pres = TRUE;
   /*Bug:46506: prepare Empty Event descriptor - */
   /* BUG 93121 */
   CC_DERIVE_BCMS(con, pTmpIcBcm, pTmpOgBcm);

   switch (con->anncCntr)
   {
      case ANNC_IC:
         ccUpdateMgQForAddOrModifyRsc(MGCT_MODE_SENDRECV, MGCT_PACKAGE_ID_MAX,
                                      MGCT_ITEM_ID_CG_MAX,
                                      MGCT_CMD_MODIFY, pTmpIcBcm,
                                      &emptyEvDsc, NULLP, &emptySigDsc, NULLP, NULLP,
                                      (pTmpIcBcm->mgCtx ? pTmpIcBcm->mgCtx->suCtxId : 0),
                                      (pTmpIcBcm->mgCtx ? pTmpIcBcm->mgCtx->contextId : 0), con);
         CC_STATE_CHANGE(con, CCS_AWTMGCTCFMFORMODIFY2_CTR);
         con->mgBlock->con = con;
         ccChangeCallStateOfAssociatedCalls(&(con->icBcm), CCS_CANNOTPROCESSEVENTS);
         ret = ccSendMgiQueue((pTmpIcBcm->mgCtx ? pTmpIcBcm->mgCtx->suCtxId : 0), con->mgBlock);
         if (ret == ROK)
         {
            RETVALUE(ROK);
         }
         else
         {
            CCLOGERR("Error: During switching icSu = 0x%lx\n", con->icSuConnId);
            ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
         }
         break;

      case ANNC_OG:
         ccUpdateMgQForAddOrModifyRsc(MGCT_MODE_SENDRECV, MGCT_PACKAGE_ID_MAX,
                                      MGCT_ITEM_ID_CG_MAX,
                                      MGCT_CMD_MODIFY, pTmpOgBcm,
                                      &emptyEvDsc, NULLP, &emptySigDsc, NULLP, NULLP,
                                      (pTmpOgBcm->mgCtx ? pTmpOgBcm->mgCtx->suCtxId : 0),
                                      (pTmpOgBcm->mgCtx ? pTmpOgBcm->mgCtx->contextId : 0), con);
         CC_STATE_CHANGE(con, CCS_AWTMGCTCFMFORMODIFY2_CTR);
         con->mgBlock->con = con;
         ccChangeCallStateOfAssociatedCalls(&(con->icBcm), CCS_CANNOTPROCESSEVENTS);
         ret = ccSendMgiQueue((pTmpOgBcm->mgCtx ? pTmpOgBcm->mgCtx->suCtxId : 0), con->mgBlock);
         if (ret == ROK)
         {
            RETVALUE(ROK);
         }
         else
         {
            CCLOGERR("Error: During switching icSu = 0x%lx\n", con->icSuConnId);
            ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
         }
         break;

      case ANNC_BOTH:
         ccUpdateMgQForAddOrModifyRsc(MGCT_MODE_SENDRECV, MGCT_PACKAGE_ID_MAX,
                                      MGCT_ITEM_ID_CG_MAX,
                                      MGCT_CMD_MODIFY, pTmpIcBcm,
                                      &emptyEvDsc, NULLP, &emptySigDsc, NULLP, NULLP,
                                      (pTmpIcBcm->mgCtx ? pTmpIcBcm->mgCtx->suCtxId : 0),
                                      (pTmpIcBcm->mgCtx ? pTmpIcBcm->mgCtx->contextId : 0), con); 
         
         ccUpdateMgQForAddOrModifyRsc(MGCT_MODE_SENDRECV, MGCT_PACKAGE_ID_MAX,
                                      MGCT_ITEM_ID_CG_MAX,
                                      MGCT_CMD_MODIFY, pTmpOgBcm,
                                      &emptyEvDsc, NULLP, &emptySigDsc, NULLP, NULLP,
                                      (pTmpOgBcm->mgCtx ? pTmpOgBcm->mgCtx->suCtxId : 0),
                                      (pTmpOgBcm->mgCtx ? pTmpOgBcm->mgCtx->contextId : 0), con);
         CC_STATE_CHANGE(con, CCS_AWTMGCTCFMFORMODIFY2_CTR);
         con->mgBlock->con = con;
         ccChangeCallStateOfAssociatedCalls(&(con->icBcm), CCS_CANNOTPROCESSEVENTS);
         ret = ccSendMgiQueue((pTmpIcBcm->mgCtx ? pTmpIcBcm->mgCtx->suCtxId : 0), con->mgBlock);
         if (ret == ROK)
         {
            RETVALUE(ROK);
         }
         else
         {
            CCLOGERR("Error: During switching icSu = 0x%lx\n", con->icSuConnId);
            ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
         }
         break;

      default:
         break;
   }   
   RETVALUE(ROK);
}   
#endif
/* 
*  
*      Fun:   ccConE08SXX
*  
*      Desc:  Connection state function
*             event - Release Indication
*             state - 
* 
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes:
*   
*      File:  cc_bdy2_2.c
*   
*/  
PUBLIC  S16 ccConE08SXX
(   
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)   
{   
  CCDP(AIN_DBG_LEVEL_1, "ccConE08SXX: event = %d, state=%d,icSu:%ld, ogSu:%ld\n", 
       con->evntType, con->state, con->icSuConnId, con->ogSuConnId);
   /* CPDI_FIX 28361 */
   if (zcCb.protState == ACTIVE)
  ccBufferEventToQueue(con, bcm, event, CCE_TIMEOUTTMREXP, sizeof(U8), 0, 
                       NULLP, 0);
  RETVALUE(ROK);
}   

/* 
*  
*      Fun:   ccConE52S03
*  
*      Desc:  Connection state function
*             event - Connection progress status indication
*             state - 
* 
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes:
*   
*      File:  cc_bdy2_2.c
*   
*/  
PUBLIC  S16 ccConE52S03
(   
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)   
{   
    Disconnect_t *pDisconnect = NULLP;

  if ((con == NULLP) || (event == NULLP))
    {
      CCLOGERR("Critical ERROR: Null pointers:%ld, %ld\n", (U32)con, (U32)event);
      RETVALUE(RFAILED);
  }
  
  CCDP(AIN_DBG_LEVEL_1, "ccConE52S03:icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", con->icSuConnId, con->ogSuConnId,
       (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0), con->icBcm.svcHeader.transId);

  pDisconnect = (Disconnect_t *)event2;

  if(pDisconnect->discCause.eh.pres && pDisconnect->discCause.causeVal.pres)
  {
      con->ainInfo.diconnectCause = pDisconnect->discCause.causeVal.val;
  }

  ccRestoreReleaseInEvntHold(con);
  ccDisconnectCallForIcBCM(con, event);
  
  RETVALUE(ROK);
}   

/* 
*  
*      Fun:   ccConE52S11
*  
*      Desc:  Connection state function
*             event - Connection progress status indication
*             state - 
* 
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes:
*   
*      File:  cc_bdy2_2.c
*   
*/  
PUBLIC  S16 ccConE52S11
(   
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)   
{   

  if ((con == NULLP) || (event == NULLP))
    {
      CCLOGERR("Critical ERROR: Null pointers:%ld, %ld\n", (U32)con, (U32)event);
      RETVALUE(RFAILED);
  }
  
  CCDP(AIN_DBG_LEVEL_1, "ccConE52S11:icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", con->icSuConnId, con->ogSuConnId,
       (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0), con->icBcm.svcHeader.transId);

  ccDisconnectCallForIcBCM(con, event);
  
  RETVALUE(ROK);
}   

/* 
*  
*      Fun:   ccConE52SXX
*  
*      Desc:  Connection state function
*             event - Connection progress status indication
*             state - 
* 
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes:
*   
*      File:  cc_bdy2_2.c
*   
*/  
#ifdef ANSI
PUBLIC  S16 ccConE52SXX
(   
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)   
#else
PUBLIC  S16 ccConE52SXX(con, event, bcm, event2)
CcConCb *con;                    /* connection */
PTR     event;                   /* event */
#endif
{   

  if ((con == NULLP) || (event == NULLP))
    {
      CCLOGERR("Critical ERROR: Null pointers:%ld, %ld\n", (U32)con, (U32)event);
      RETVALUE(RFAILED);
  }
  /* BUG:84505 
   * We used to treat Disconnect in S09 as unexpected event
   * but for the ICA call, we will map the ReleaseCall for the second IACA
   * call as Disconnect. In order not to impact the original design, we made a slight
   * change to let only the ICA call process the Disconnect in S09 while keep the original design there
   * Fix the ICA call DFC+RC received before the second IACA call get answered
   */
  if ((con->state == CCS_AWTANS) && !CC_IS_ICA_CALL(con))
  {
    CCDP(AIN_DBG_LEVEL_1, "ccConE52SXX:Disconnect received in S09 but not ICA call\n");
    ABORT_DEBUG;
    CCDBGP(DBGMASK_CC, (ccInit.prntBuf, " Received Unexpected Event \n"));
    RETVALUE(ROK);
  }

  CCDP(AIN_DBG_LEVEL_1, "ccConE52SXX:icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", con->icSuConnId, con->ogSuConnId,
       (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0), con->icBcm.svcHeader.transId);

  ccDisconnectCallForIcBCM(con, event);
  
  RETVALUE(ROK);
}   

/* 
*  
*      Fun:   ccConE31SYY
*  
*      Desc:  Connection state function
*             event - Connection progress status indication
*             state - 
* 
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes:
*   
*      File:  cc_bdy2_2.c
*   
*/  
#ifdef ANSI
PUBLIC  S16 ccConE31SYY
(   
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)   
#else
PUBLIC  S16 ccConE31SYY(con, event, bcm, event2)
CcConCb *con;                    /* connection */
PTR     event;                   /* event */
#endif
{
#if 0 /* CC_MGC PENDING */
   MgitInd    *mgitInd = NULLP;
  
  CCDP(AIN_DBG_LEVEL_1, "ccConE31SYY:icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", con->icSuConnId, con->ogSuConnId,
       con->mgBlock->mgCtx->suCtxId, con->icBcm.svcHeader.transId);
  
  mgitInd = (MgitInd *)event;

  if (!mgitInd)
  {
     CCLOGERR("ERROR!! void MgitInd! \n");
     RETVALUE(RFAILED);
  }
  
  /* only buffer SVC_CHANGE_IND or ignore ! */
  switch (mgitInd->type)
    {
    case MGIT_SVC_CHANGE_IND:
       ccBufferEventToQueue(con, bcm, event, CCE_SFEVTIND, sizeof(U8), 0,
                            NULLP, 0);
       break;
    default:
       /* Should never hit this because of the check above */
       break;  
    }
#endif  
  RETVALUE(ROK);
}

/* 
*  
*      Fun:   ccConE37S11
*  
*      Desc:  Connection state function
*             event - Connection progress status indication
*             state - 
* 
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes:
*   
*      File:  cc_bdy2_2.c
*   
*/  
PUBLIC  S16 ccConE37S11
(   
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)   
{   

  CCDP(AIN_DBG_LEVEL_1, "ccConE37S11: icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", con->icSuConnId, con->ogSuConnId,
       (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0), con->icBcm.svcHeader.transId);

  /* identical to E37S62 */
  ccConE37S62(con, event, bcm, event2);
 
  RETVALUE(ROK);
}   

/* 
*  
*      Fun:   ccConE47SXX
*  
*      Desc:  Connection state function
*             event - Send_To_Resource
*             state - CCS_AWTRSPTIMEOUT
* 
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: used in CPDI intermediate states
*   
*      File:  cc_bdy2_2.c
*   
*/  
PUBLIC  S16 ccConE47SXX
(   
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)   
{   
  SvcHeader_t        *srSvcHeader           = NULLP;
  S16                 mgret                 = RFAILED;
  S16                 ret                   = RFAILED;

  CCDP(AIN_DBG_LEVEL_1, "ccConE47SXX: icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", con->icSuConnId, con->ogSuConnId,
       (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0), con->icBcm.svcHeader.transId);

  ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
  srSvcHeader = (SvcHeader_t *) event;

  ccRestoreReleaseInEvntHold(con);

  if (srSvcHeader->lastTransactionFlag == TRUE)
    ccDeleteBcm(&(con->icBcm));

    
  con->ogBcm.closeCause = CscRmtFailure;
  con->lastState = con->state;
  con->anncBlock.stateForAnnc = con->state;
#if 1 /* EARLY_ACM */
  ccStopConTmr(con, CC_TMR_EARLYACM);
#endif


  
  if (CC_IS_CONTEXT_PRESENT(con))
  {
#if 1 /* EARLY_ACM */
    /* Need to clean all the legs in the context */
    CC_RESET_ANY_EARLY_ACM_FLAGS(con);
#endif
    ccQueueSubOnOGCleanUp(con);
    ccChangeCallStateOfAssociatedCalls(&(con->icBcm), CCS_CANNOTPROCESSEVENTS);
    con->relPend = OUTRELANDPLAYANNC;
    CC_STATE_CHANGE(con, CCS_AWTSWTCFMFORDP);
    con->mgBlock->con = con;
    mgret = ccSendMgiQueue(0, con->mgBlock);
    if (mgret == ROK)
      RETVALUE(ROK);
    else
    {
      CCLOGERR("Error: MGI failed:icSu:%ld, ogSu:%ld, suCtxId:%ld,"
               " icTransId:%ld\n", con->icSuConnId, con->ogSuConnId,
               con->mgBlock->mgCtx->suCtxId, con->icBcm.svcHeader.transId);
      ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
      cleanUpMgQ(con);
      ccHandleCleanUpConCb(con, TRUE, AM_CV_TMPFAIL);
      RETVALUE(ROK);
    }
  }

  if (con->ogRscVal)
  {
    ccFreeTheRscFromCon(con, OUTTOINC); /* This will take care of REL_CFM */
  }

  {
    initOgBcm(&con->ogBcm);
    ret = ccSendToResource(&(con->icBcm));
    if (ret != ROK)
    {
        CCLOGERR("Error: ccSendToResource fail:icSu:%ld,ogSu:%ld,icTrsId:%ld\n",
                 con->icSuConnId, con->ogSuConnId, 
                 (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0));
        ccHandleCleanUpConCb(con, TRUE, AM_CV_TMPFAIL);
    }
    RETVALUE(ROK);
  }
 
  RETVALUE(ROK);
}   

/* euysal - 12/06/2005 - Bug:42382 */
/* 
*  
*      Fun:   ccConE47S09
*  
*      Desc:  Connection state function
*             event - Send_To_Resource
*             state - CCS_AWTANS
* 
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: 
*   
*      File:  cc_bdy2_2.c
*   
*/  
PUBLIC  S16 ccConE47S09
(   
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)   
{   
  SvcHeader_t        *srSvcHeader           = NULLP;
  S16                 mgret                 = RFAILED;
  S16                 ret                   = RFAILED;
  U8                  relDisBoolOG          = RFAILED;
  U8                  evntType              = 0;

  CCDP(AIN_DBG_LEVEL_1, "ccConE47S00: icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", con->icSuConnId, con->ogSuConnId,
       (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0), con->icBcm.svcHeader.transId);

  ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
  srSvcHeader = (SvcHeader_t *) event;

  ccStopConTmr(con, AIN_O_NO_ANS_TMR);
  ccStopConTmr(con, AIN_T_NO_ANS_TMR);
  ccStopConTmr(con, CC_TMR_AWAITANS); 
  /* Stop the CC_TMR_PREANM timer if it is started */
  ccStopConTmr(con, CC_TMR_PREANM);

  ccDeleteBufferedEvent(con, con->icSuConnId, CCE_ONOANSEXP);
  ccDeleteBufferedEvent(con, con->ogSuConnId, CCE_TNOANSEXP);
  ccRestoreReleaseInEvntHold(con);

  if (srSvcHeader->lastTransactionFlag == TRUE)
    ccDeleteBcm(&(con->icBcm));

    
  con->ogBcm.closeCause = CscRmtFailure;
  con->lastState = con->state;

  /* After A-STR is complete, go into state awaiting for AR */ 
  con->anncBlock.stateForAnnc = CCS_AWTRSPFORDPE3;
#if 1 /* EARLY_ACM */
  ccStopConTmr(con, CC_TMR_EARLYACM);
#endif


  
  if (CC_IS_CONTEXT_PRESENT(con))
  {
#if 1 /* EARLY_ACM */
    /* Need to clean all the legs in the context */
    CC_RESET_ANY_EARLY_ACM_FLAGS(con);
#endif
    ccQueueSubOnOGCleanUp(con);
    ccChangeCallStateOfAssociatedCalls(&(con->icBcm), CCS_CANNOTPROCESSEVENTS);
    if (CC_IS_REL_IN_EVNT_HOLD(con))
       con->relPend = OUTRELRSPANDPLAYANNC;
    else
    {
       if (con->ccRelEvntHold)
          con->relPend = OUTRELRSPANDPLAYANNC;
       else
       {
          /* put default cause value here */
          con->tempHolderForReleaseCause = AM_CV_NORMCALLCLR;
          con->relPend = OUTRELANDPLAYANNC;;
       } 
    }
    CC_STATE_CHANGE(con, CCS_AWTSWTCFMFORDP);
    con->mgBlock->con = con;
    mgret = ccSendMgiQueue(0, con->mgBlock);
    if (mgret == ROK)
      RETVALUE(ROK);
    else
    {
      CCLOGERR("Error: MGI failed:icSu:%ld, ogSu:%ld, suCtxId:%ld,"
               " icTransId:%ld\n", con->icSuConnId, con->ogSuConnId,
               con->mgBlock->mgCtx->suCtxId, con->icBcm.svcHeader.transId);
      ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
      ccHandleCleanUpConCb(con, TRUE, AM_CV_TMPFAIL);
      RETVALUE(ROK);
    }
  }

  if (con->ogRscVal)
  {
    relDisBoolOG = ccCheckRelOrOnHookInBuffer(&(con->ogBcm), &evntType);
    if (relDisBoolOG == ROK)
    {
      ccCloseTBcmTransaction(con);
      con->direction = OUTTOINC;
      if (con->noRspReqd == FALSE) ccSendReleaseRsp(con);
      ccDeallocateResource(con, CC_OUTGOING);
      con->ogRscVal = FALSE;
    }
    else
    {
      ccFreeTheRscFromCon(con, OUTTOINC); /* This will take care of REL_CFM */
    }
  }

  initOgBcm(&con->ogBcm);
  ret = ccSendToResource(&(con->icBcm));
  if (ret != ROK)
  {
     CCLOGERR("Error: ccSendToResource fail:icSu:%ld,ogSu:%ld,icTrsId:%ld\n",
              con->icSuConnId, con->ogSuConnId, 
              (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0));
     ccHandleCleanUpConCb(con, TRUE, AM_CV_TMPFAIL);
  }
  RETVALUE(ROK);
}   


/* 
*  
*      Fun:   ccConE47S38
*  
*      Desc:  Connection state function
*             event - Send_To_Resource
*             state - CCS_AWTRSPTIMEOUT
* 
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes:
*   
*      File:  cc_bdy2_2.c
*   
*/  
PUBLIC  S16 ccConE47S38
(   
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)   
{   

  CCDP(AIN_DBG_LEVEL_1, "ccConE47S38: icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", con->icSuConnId, con->ogSuConnId,
       (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0), con->icBcm.svcHeader.transId);
#if 1 /* EARLY_ACM */
  ccStopConTmr(con, CC_TMR_EARLYACM);
#endif

  /* we already armed NEL list, here we simply turn the state to ANSWERED */
  CC_STATE_CHANGE(con, CCS_ANSWERED);
  ccProcessCcChangeforX2SIG(con); /* FID 15083.0 */
 
  RETVALUE(ROK);
}   

/* 
*  
*      Fun:   ccConE70S11
*  
*      Desc:  Connection state function
*             event - Connection progress status indication
*             state - CCS_AWTRSPTIMEOUT
* 
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes:
*   
*      File:  cc_bdy2_2.c
*   
*/  
PUBLIC  S16 ccConE70S11
(   
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)   
{
  CCDP(AIN_DBG_LEVEL_1, "ccConE70S11: icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", con->icSuConnId, con->ogSuConnId,
       (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0), con->icBcm.svcHeader.transId);

  /* stop obselete timeout timer first */
  ccStopConTmr(con, CC_TMR_TIMEOUT);

  /* restart timeout timer */
  if (((con->icBcm.nel[EReqTimeout] == NEL_NOTIFY) ||
       (con->icBcm.nel[EReqTimeout] == NEL_REQUEST)) &&
      con->ainInfo.TimeoutTimer.pres && con->ainInfo.TimeoutTimer.val > 0)
    {       
        /* start time out timer */
        ccCp.genCfg.tmr.tTimeout.enb = TRUE;
        ccCp.genCfg.tmr.tTimeout.val = con->ainInfo.TimeoutTimer.val;
        ccStartConTmr(CC_TMR_TIMEOUT, con, (PTR)&ccCp.genCfg);
        CCDP(AIN_DBG_LEVEL_0, "Started timeout timer val=%d\n", ccCp.genCfg.tmr.tTimeout.val);
    }
  
  /* CPDI 28361 replicate both BCM and TimeoutTimer before FIC interactions */
  if (CC_IS_OBCM_PIC_ACTIVE(con))
  {
     ZcUpdParam upd; 

     cmMemset((U8 *)&upd, 0, sizeof(ZcUpdParam));
     upd.featMsg.suConnId     = con->icSuConnId;
     upd.featMsg.ccAinUpdType = CC_UPDATE_BCM;
     upd.featMsg.nel          = con->icBcm.nel;
     zcRunTimeUpd(ZC_FEAT, CMPFTHA_UPD_REQ, upd);
     zcUpdPeer();
     /* use DlegId and suCtxId1 to carry tTimeout.enb & tTimeout.val */
     cmMemset((U8*) &upd, 0, sizeof(ZcUpdParam));
     upd.featMsg.suConnId     = con->icSuConnId;
     upd.featMsg.ccAinUpdType = CC_UPDATE_TMROUTTMR;
     upd.featMsg.DlegId       = ccCp.genCfg.tmr.tTimeout.enb;
     upd.featMsg.suCtxId1     = ccCp.genCfg.tmr.tTimeout.val;
     zcRunTimeUpd(ZC_FEAT, CMPFTHA_UPD_REQ, upd);
     zcUpdPeer();
  }
  /* we already armed NEL list, here we simply turn the state to ANSWERED */
  CC_STATE_CHANGE(con, CCS_ANSWERED);
  ccProcessCcChangeforX2SIG(con); /* FID 15083.0 */

  if (((!ccIsSwitchingLocallyDone(&con->icBcm)) && (!ccIsSwitchingLocallyDone(&con->ogBcm)) &&  
      (CC_IS_CONTEXT_PRESENT(con)) && (!CCT_IS_FCIM_SET_ACC(con->fciMSet))) || 
      (CCT_IS_FCIM_SET_ACC(con->fciMSet) && CC_IS_CALL_FLAG2(con, CC_FLAG_TRANSIENT_MG)))  /* FID 16692.0 */
  {
    CCDP(AIN_DBG_LEVEL_0, "%s: Re-Negotiating On-Net: icSu:0x%lx, ogSu:0x%lx"
         "suCtxId:0x%lx\n", __FUNCTION__, (U32)con->icSuConnId,
         (U32) con->ogSuConnId, (U32)con->mgBlock->mgCtx->suCtxId);
    ccCreateBearer(&(con->icBcm), &(con->ogBcm), NULLP, NULLP, con->state,
                   FALSE);
  }


  RETVALUE(ROK);
}

/* 
*  
*      Fun:   ccConE71S11
*  
*      Desc:  Connection state function
*             event - CCE_CONNECT_TO_RESOURCE
*             state - CCS_AWTRSPTIMEOUT
* 
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes:
*   
*      File:  cc_bdy2_2.c
*   
*/  
PUBLIC  S16 ccConE71S11
(   
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)   
{
  S16 ret = ROK;

  CCDP(AIN_DBG_LEVEL_1, "ccConE71S11:icSu:%ld, ogSu:%ld, icProt:%d\n", con->icSuConnId,
       con->ogSuConnId, con->icProtType);
  
  ret = ccProcessConnectToResource(&(con->icBcm));

  if (ret !=ROK)
    {
      CCDP(AIN_DBG_LEVEL_0, "FAILED to Play CPDI Annc thru MGI for annc-conf facility\n");
      RETVALUE(RFAILED);
    }
  else
    {
      CCDP(AIN_DBG_LEVEL_0, "Played CPDI Annc thru MGI for annc-conf facility\n");
    }
  
  RETVALUE(ROK);
}   

/* 
*  
*      Fun:   ccConE71S03
*  
*      Desc:  Connection state function
*             event - CCE_CONNECT_TO_RESOURCE
*             state - CCS_AWTRSPODISCONNECT
* 
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes:
*   
*      File:  cc_bdy2_2.c
*   
*/  
PUBLIC  S16 ccConE71S03
(   
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)   
{
  S16 ret = ROK;

  CCDP(AIN_DBG_LEVEL_1, "ccConE71S03:icSu:%ld, ogSu:%ld, icProt:%d\n", con->icSuConnId,
       con->ogSuConnId, con->icProtType);
  
  /* euysal - 02/14/2006 - Send CTRClear with Error Code ClcFailure in case of failure */
  con->lastState = con->state;
  ret = ccProcessConnectToResource(&(con->icBcm));

  if (ret !=ROK)
    {
      CCDP(AIN_DBG_LEVEL_0, "FAILED to Play CPDI Annc thru MGI for annc-conf facility\n");
      ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);

      RETVALUE(RFAILED);
    }
  else
    {
      CCDP(AIN_DBG_LEVEL_0, "Played CPDI Annc thru MGI for annc-conf facility\n");
    }
  
  RETVALUE(ROK);
}  

/* euysal - 03/10/2005 - CAMEL Support */
#if 1
/* 
*  
*      Fun:   ccConE71S10
*  
*      Desc:  Connection state function
*             event - CCE_CONNECT_TO_RESOURCE
*             state - CCS_ANSWERED
* 
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes:
*   
*      File:  cc_bdy2_2.c
*   
*/  
PUBLIC  S16 ccConE71S10
(   
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)   
{
  S16 ret = ROK;

  CCDP(AIN_DBG_LEVEL_1, "ccConE71S10:icSu:%ld, ogSu:%ld, icProt:%d\n", con->icSuConnId,
       con->ogSuConnId, con->icProtType);
  
  /* euysal - 02/14/2006 - Send CTRClear with Error Code ClcFailure in case of failure */
  con->lastState = con->state;
  ret = ccProcessConnectToResource(&(con->icBcm));

  if (ret !=ROK)
    {
      CCDP(AIN_DBG_LEVEL_0, "FAILED to Play CPDI Annc thru MGI for annc-conf facility\n");
      RETVALUE(RFAILED);
    }
  else
    {
      CCDP(AIN_DBG_LEVEL_0, "Played CPDI Annc thru MGI for annc-conf facility\n");
    }
  
  RETVALUE(ROK);
}   
#endif

/* 
*  
*      Fun:   ccConE39S03
*  
*      Desc:  Connection state function
*             event - Connection progress status indication
*             state - 
* 
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes:
*   
*      File:  cc_bdy2_2.c
*   
*/  
PUBLIC  S16 ccConE39S03
(   
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)   
{
    SvcHeader_t       *ctSvcHeader    = NULLP;
    Continue_t        *pContinue      = NULLP;
    S16                mgret          = RFAILED;
    S16                 ret           = RFAILED;
    ProtType            ogProtType    = CC_GET_PROTOCOL(con->ogProtType);
    CcRelEvnt           ogEvnt;
    CcCause cause;
    TknU16  sipCause;
    TknU16  sipStatus;
    /* bug 72537 */
    MgctPackageId              pkgId  = MGCT_PACKAGE_ID_MAX;
    MgctMode                   mode   = MGCT_MODE_MAX;
    MgctCallProgItemId         signal = MGCT_ITEM_ID_CG_MAX;

    cmMemset((U8 *)&cause, 0, sizeof(CcCause));

    /* bug 91532 */
    cmMemset((U8 *)&ogEvnt, 0, sizeof(ogEvnt));
    
    CCDP(AIN_DBG_LEVEL_1, "ccConE39S03:icSu:%ld, ogSu:%ld, suCtxId:%ld,"
         " icTransId:%ld, con->state= %d\n", con->icSuConnId, con->ogSuConnId,
         (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0),con->icBcm.svcHeader.transId, con->state);

    ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
  
    /* Bug# 37809 */
    ccStopConTmr(con, AIN_O_NO_ANS_TMR);
    ccStopConTmr(con, AIN_T_NO_ANS_TMR);

    ccStopConTmr(con, CC_TMR_AWAITANS);
    ccStopConTmr(con, TMR_SETUP);
    ccDeleteBufferedEvent(con, con->icSuConnId, CCE_ONOANSEXP);
    ccDeleteBufferedEvent(con, con->ogSuConnId, CCE_TNOANSEXP);
    /* End - 37809 */
#if 1 /* EARLY_ACM */
    ccStopConTmr(con, CC_TMR_EARLYACM);
#endif
  
    /* 28351: no special handling here, so should be fine */
    if (ccRestoreReleaseInEvntHold(con))
    {
       /* bug72209 */
       if(con->tempHolderForReleaseCause == CCTMRRECOV)
          ccHandleCleanUpConCb(con, TRUE, CCTMRRECOV);
       else
          ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
       RETVALUE(ROK);
    }
    if ((ogProtType == CC_SI) && (con->ccEvntHold) &&
        (con->ccEvntHold->m.ccRelEvnt.m.siRelEvnt.causeDgn.dgnVal.pres == PRSNT_NODEF) &&
        (con->ccEvntHold->m.ccRelEvnt.m.siRelEvnt.causeDgn.dgnVal.len == 0))
    {
       ABORT_DEBUG;
    }


    ctSvcHeader = (SvcHeader_t *) event;
    pContinue   = (Continue_t *)  event2;
    if(pContinue->errorCode != FC_NOT_APPLICABLE)
    {
        CCLOGERR("ERROR FROM FIC = %ld\n", pContinue->errorCode);
        con->tempHolderForReleaseCause = pContinue->errorCode;
       /* euysal 05/27/2005 - ACC Support */
      if  (con->icProtType == CC_LOCAL_SWT_PROT)
        ccRemoveCaleaAssnWithTappedBcm(con);

      con->ainInfo.termInd = EXCEPTION_IND;
      con->ogBcm.closeCause = CscRmtBusy;
      ccCloseTBcmTransaction(con);

      if (CC_IS_CONTEXT_PRESENT(con) && (ccCheckMgContext(&con->ogBcm, &pkgId, &signal, &mode) == ROK)) /* bug 72537 */
      {
        S16  ret = RFAILED;
        con->failCndVal = pContinue->errorCode;

        ccStartConTmr(TMR_RELEASE, con, (PTR)&ccCp.genCfg); /* Bug 38498 */
#if 1 /* EARLY_ACM */
        CC_RESET_ANY_EARLY_ACM_FLAGS(con);
#endif
        /* Subtract BCMs from MG context and then disconnect the legs */
        ccQueueSubOnOGCleanUp(con);

        ret = ccSendMgiQueue(0, con->mgBlock);
        if (ret != ROK)
        {
          CCLOGERR( "**** ERROR: can't do ccSendMgiQueue *******\n");
          cleanUpMgQ(con);
          ccCleanUpMGCtx(con);
        }
        else
        {
          CC_STATE_CHANGE(con, CCS_AWTSWTCFM_SENDONEREL);
          con->mgBlock->con = con;
          con->relPend = OUTRELRSPANDINFAILCOND;
          RETVALUE(ROK);
        }
      }

      if (con->ogRscVal)
      {
        con->direction = OUTTOINC;
        if (con->noRspReqd == FALSE) ccSendReleaseRsp(con);
          ccDeallocateResource(con, CC_OUTGOING);
        con->ogRscVal = FALSE;
      }

      ccProcessFailCnd(con, pContinue->errorCode, TRUE);
      RETVALUE(ROK);
    }
     else
        con->tempHolderForReleaseCause = AM_CV_TMPFAIL;
  
    /* in this stage, call should be released in all cases */
    if  (con->icProtType == CC_LOCAL_SWT_PROT)
      ccRemoveCaleaAssnWithTappedBcm(con);
    
    con->ainInfo.termInd = EXCEPTION_IND;
    con->ogBcm.closeCause = CscRmtBusy;
    ccCloseTBcmTransaction(con);

    ccStartConTmr(TMR_RELEASE, con, (PTR)&ccCp.genCfg); /* Bug 38498 */
    ccStopConTmr(con, CC_TMR_CALLDTL); /* Bug 38498 */

    if (CC_IS_CONTEXT_PRESENT(con) && (ccCheckMgContext(&con->ogBcm, &pkgId, &signal, &mode) == ROK)) /* bug 72537 */
    {
#if 1 /* EARLY_ACM */
        /* Need to clean all the legs in the context */
        CC_RESET_ANY_EARLY_ACM_FLAGS(con);
#endif
        ccQueueSubOnOGCleanUp(con);
        con->relPend = OUTRELRSPANDINTONE;
        
        CC_STATE_CHANGE(con, CCS_AWTSWTCFM_SENDONEREL);
        con->mgBlock->con = con;
        mgret = ccSendMgiQueue(0, con->mgBlock);
        if (mgret == ROK)
            RETVALUE(ROK);
        else
        {
          CCLOGERR("Error: MGI failed:icSu:%ld, ogSu:%ld, suCtxId:%ld,"
                   " icTransId:%ld\n", con->icSuConnId, con->ogSuConnId,
                   con->mgBlock->mgCtx->suCtxId, con->icBcm.svcHeader.transId);
          cleanUpMgQ(con);
          ccHandleCleanUpConCb(con, TRUE, AM_CV_TMPFAIL);
          RETVALUE(ROK);
        }
    }

    /* bug: 56917 */
    ccGetRelCause (con, con->ogProtType, (CcRelEvnt *)con->ccEvntHold,
             &cause, &sipCause, &sipStatus);

    if (con->direction == OUTTOINC)
    {
       con->failCndVal = ccMapProtRel2FailCnd (con, &cause, &sipCause, &sipStatus);
    }

    CC_STATE_CHANGE(con, CCS_AWTRELCFM_IC);

    if (con->ogRscVal) 
    {
       if (con->noRspReqd == FALSE)
          ccSendReleaseRsp(con);
#ifdef GUARD_TIMER          
       ccGuardOrDeallocResource(con, CC_OUTGOING);
#else
       ccDeallocateResource(con, CC_OUTGOING);
#endif
       con->ogRscVal = FALSE;
    }

    if (con->direction == OUTTOINC && CC_IS_CAUSEMAP_SUPPORTED
             (CC_GET_PROTOCOL(con->ogProtType)))
    {
       ccStopConTmr(con, TMR_RELEASE);
       ret =  ccProcessFailCnd (con, con->failCndVal, FALSE);
       if (ret == ROK)                
          RETVALUE(ROK);
    }

    con->icEvnt = con->ccEvntHold;
    con->ogEvnt = (CcAllSdus *) &ogEvnt;

    ret = ccMapEvent(con, CCE_RELIND, 0);
    if (ret == ROK)
    {
        ccSendReleaseReq(con);
    }
    else
    {
        ccSendRelease(con, con->icSapCb, con->icSpConnId, con->icSuConnId,
                      con->icProtType, CCPROTERR,NULL);
    }
    RETVALUE(ROK);

}
#if 0 /* bug 38872 */
/* 
*  
*      Fun:   ccConE39S44
*  
*      Desc:  Connection state function
*             event - Continue
*             state - O_disconnect
* 
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes:
*   
*      File:  cc_bdy2_2.c
*   
*/  
PUBLIC  S16 ccConE39S44
(   
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)   
{
    SvcHeader_t       *ctSvcHeader    = NULLP;
    Continue_t        *pContinue      = NULLP;
    S16                mgret          = RFAILED;
    S16                 ret           = RFAILED;
    CcAllSdus           ogEvnt;

    CCDP(AIN_DBG_LEVEL_1, "ccConE39S44:icSu:%ld, ogSu:%ld, suCtxId:%ld,"
         " icTransId:%ld, con->state= %d\n", con->icSuConnId, con->ogSuConnId,
           con->mgBlock->mgCtx->suCtxId, con->icBcm.svcHeader.transId, con->state);

      ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
    
      ccStopConTmr(con, AIN_O_NO_ANS_TMR);
      ccStopConTmr(con, AIN_T_NO_ANS_TMR);

      ccStopConTmr(con, CC_TMR_AWAITANS);
      ccStopConTmr(con, TMR_SETUP);
      ccDeleteBufferedEvent(con, con->icSuConnId, CCE_ONOANSEXP);
      ccDeleteBufferedEvent(con, con->ogSuConnId, CCE_TNOANSEXP);
      /* End - 37809 */
    
      /* 28351: no special handling here, so should be fine */
      if (ccRestoreReleaseInEvntHold(con))
      {
         ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
         RETVALUE(ROK);
      }

      ctSvcHeader = (SvcHeader_t *) event;
      pContinue   = (Continue_t *)  event2;
      if(pContinue->errorCode != FC_NOT_APPLICABLE)
      {
          CCLOGERR("ERROR FROM FIC = %ld\n", pContinue->errorCode);
          con->tempHolderForReleaseCause = pContinue->errorCode;
      }
      else
          con->tempHolderForReleaseCause = AM_CV_TMPFAIL;
    
      /* in this stage, call should be released in all cases */
      if  (con->icProtType == CC_LOCAL_SWT_PROT)
        ccRemoveCaleaAssnWithTappedBcm(con);
      
      ccStartConTmr(TMR_RELEASE, con, (PTR)&ccCp.genCfg); /* Bug 38498 */
      ccStopConTmr(con, CC_TMR_CALLDTL); /* Bug 38498 */

      if (CC_IS_CONTEXT_PRESENT(con))
      {
          ccQueueSubOnOGCleanUp(con);
          if (con->direction == INCTOOUT)
            con->relPend = INRELRSPANDOUTREL;
          else
            con->relPend = OUTRELRSPANDINREL;

          CC_STATE_CHANGE(con, CCS_AWTSWTCFM_SENDONEREL);
          con->mgBlock->con = con;
          mgret = ccSendMgiQueue(con->mgBlock->mgCtx->suCtxId, con->mgBlock);
          if (mgret == ROK)
              RETVALUE(ROK);
          else
          {
            CCLOGERR("Error: MGI failed:icSu:%ld, ogSu:%ld, suCtxId:%ld,"
                     " icTransId:%ld\n", con->icSuConnId, con->ogSuConnId,
                     con->mgBlock->mgCtx->suCtxId, con->icBcm.svcHeader.transId);
            ccHandleCleanUpConCb(con, TRUE, AM_CV_TMPFAIL);
            RETVALUE(ROK);
          }
      }

      if (con->noRspReqd == FALSE) ccSendReleaseRsp(con);

      if (con->direction == INCTOOUT)
      {
        /* Deallocate the IC Resources */
        CC_STATE_CHANGE(con, CCS_AWTRELCFM_OG);
#ifdef GUARD_TIMER
        ccGuardOrDeallocResource(con, CC_INCOMING);
#else
        ccDeallocateResource(con, CC_INCOMING);
#endif
        con->icRscVal = FALSE;
      }
      else if (con->direction == OUTTOINC)
      {
        CC_STATE_CHANGE(con, CCS_AWTRELCFM_IC);
#ifdef GUARD_TIMER
        ccGuardOrDeallocResource(con, CC_OUTGOING);
#else
        ccDeallocateResource(con, CC_OUTGOING);
#endif
        con->ogRscVal = FALSE;
      }

      con->icEvnt = con->ccEvntHold;
      con->ogEvnt = &ogEvnt;

    ret = ccMapEvent(con, CCE_RELIND, 0);
    if (ret != ROK)
    { 
       if (con->direction == INCTOOUT)
       {
          ccSendRelease(con, con->ogSapCb, con->ogSpConnId, con->ogSuConnId,
                    con->ogProtType, con->tempHolderForReleaseCause,NULL);
       }
       else if (con->direction == OUTTOINC)
       {
#ifdef CC_CALEA
          /* For CALEA when mapping fails just don't send Release to the other side */ 
          if (con->icProtType != CC_LOCAL_SWT_PROT)
          {
#endif
            ccSendRelease(con, con->icSapCb, con->icSpConnId, con->icSuConnId,
                      con->icProtType, con->tempHolderForReleaseCause,NULL);
#ifdef CC_CALEA
          }
#endif
      }
      RETVALUE(ROK);
    }
    ccSendReleaseReq(con);

    RETVALUE(ROK);
} /* E39S44 */
#endif /* 38872 */

/* 
* 
*      Fun:   ccConE62S09
*  
*      Desc:  Connection state function
*             event - CCE_NOTIFYFROMMGCT
*             state - CCS_AWTANS
* 
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes:
*   
*      File:  cc_bdy2_2.c
*   
*/  
PUBLIC  S16 ccConE62S09
(   
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)   
{
  MgctTxnInd        *mgctTxnInd    = NULLP;
  ProtType           ogProtType    = 0;
  U16                megacoErr     = 0;
  MgctNotifyKind_e   kind          = MGCT_NOTIFY_KIND_UNKNOWN;
  S16                ret           = RFAILED;

  /*CID 15595*/
  if ((con == NULLP) || (event == NULLP) || (bcm == NULLP))
  {
    CCLOGERR("Error: Null pointers con = 0x%lx, event = 0x%lx, bcm = 0x%lx\n",
             (U32)con, event, (U32)bcm);
    RETVALUE(RFAILED);
  }

  CCDP(AIN_DBG_LEVEL_1, "ccConE62S09 icSu = 0x%lx, ogSu = 0x%lx\n",
       con->icSuConnId, con->ogSuConnId);

  /* Feature 1789 - German ISUP */
  /* stop the Toiw2 Timer if running */
  ccStopConTmr(con, CC_TMR_TOIW2);

  /* We should not get the DTMF NOTIFY from the terminating side at S09 */
  /* Bug 90408 SIP invite w/o SDP route to announcement case where 183
   * is sent back to SIP side and Prack is responsed. E62S09 is called 
   * due to messed state machine. Null pointer is accesssed.
   */
  if(con->ogIntfcCb)
  {
      CCDP(AIN_DBG_LEVEL_1, "DBG code, bcmType:%d, icProtType:%d, ogProtType:%d.\n",
           bcm->bcmType, CC_GET_PROTOCOL(con->icIntfcCb->protType), CC_GET_PROTOCOL(con->ogIntfcCb->protType));
      if ((bcm->bcmType == BtOBcm) && (CC_GET_PROTOCOL(con->ogIntfcCb->protType) == CC_EXT_BICC))
      {
         CCDP(AIN_DBG_LEVEL_1, "Received a NOTIFY for SIP/ISUP interworking with BICC.\n");
         ccHandleMgctNotifyForBicc(con, (MgctTxnInd *) event, bcm);
         RETVALUE(ROK);
      }
  }
  mgctTxnInd = (MgctTxnInd *) event;
  ret = ccHandleMgctNotify(con, (MgctTxnInd *) event);
  if (ret != ROK)
  {
    CCLOGERR("Error: ccHandleMgctNotify failed icSu = 0x%lx,"
             "suCtxId 0x%lx, contextId = 0x%lx\n", con->icSuConnId,
              (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0), 
              (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->contextId : MGCT_CONTEXT_NULL));
    ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
    RETVALUE(ROK);
  }
  ret = mgctGetNotifyReqKind(mgctTxnInd, &megacoErr, &kind, NULLP);
  
 if (ret != ROKIGNORE)
 {
  if (kind == MGCT_NOTIFY_KIND_DIGIT_COMPLETION)
  {
    if ((con->ainInfo.ODTMFDigitString.pres == PRSNT_NODEF) &&
        (con->icBcm.nel[EReqODTMFEntered] == NEL_REQUEST))
    {
     /* CPDI_FIX: we need relinguish old rel event in the buffer */
          /* logic: whenever ccRelEvntHold != NULL, ccEvntHold isn't CCE_RELIND */ 
          CC_FREE_REL_EVNT_HOLD(con);
        ret = ccProcessODTMFEntered(&con->icBcm);
      if (ret != ROK)
      {
        CCLOGERR("Error: ccProcessODTMFEntered failed icSu = 0x%lx\n",
                 con->icSuConnId);
        ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
      }
      RETVALUE(ROK);
    }
    else if (ogProtType == CC_SIP)
    {
    }
    else
    {
      CCLOGERR("Error: DTMF detected for unknow application icSu 0x%lx\n",
               con->icSuConnId);
      ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
      RETVALUE(ROK);
    }
  }
  else if (kind == MGCT_NOTIFY_KIND_SIGNAL_COMPLETION)
  {
    CCDP(AIN_DBG_LEVEL_1, "This is a case of X-Resource Annc 0x%lx\n",
         con->icSuConnId);
    ccPostAnnoucementProcessingForConfCalls(con);
    RETVALUE(ROK);
  }
  else if((kind == MGCT_NOTIFY_KIND_G_CAUSE) &&
           ((CC_GET_PROTOCOL(con->icProtType) == CC_EXT_BICC) ||
            (CC_GET_PROTOCOL(con->ogProtType) == CC_EXT_BICC)))
  {
     /* FID 15319 possible Notify with failure from MG when MG is waitting for IPBCP info */
     MgctEvt_G_C_GeneralCause gcause = mgctTxnInd->u.notifyReq.observedEventsDesc.
                          observedEventList->observedEventParameterList->u.eventOther.u.gc.generalCause;

     if(gcause == MGCT_EVT_G_C_GEN_CAUSE_IW)
     {
         con->tempHolderForReleaseCause = CCINTRWRK;
     }
     else if(gcause == MGCT_EVT_G_C_GEN_CAUSE_FT)
     {
         con->tempHolderForReleaseCause = CCRESCUNAVAIL;
     }
     else if(gcause == MGCT_EVT_G_C_GEN_CAUSE_UR)
     {
         con->tempHolderForReleaseCause = CCRESCUNAVAIL;
     }

     CCDP(AIN_DBG_LEVEL_0, "%s: receive g/cause %d from MGI\n", __FUNCTION__,gcause);

     ccSendReleasesOnBothSidesWithSwitching(con, con->tempHolderForReleaseCause);
  }
  else if((kind == MGCT_NOTIFY_KIND_FAX_DETECT) || 
          (kind == MGCT_NOTIFY_KIND_MODEM_DETECT) ||
          (kind == MGCT_NOTIFY_KIND_TEXT_DETECT)) /* FID !4844 - Ignore TEXT event */ 

  {
    /* Ignore false FAX/Modem notifications from the MG -- Bug 53187*/
    CCLOGERR("Error: Ignoring Fax/Modem/Text event in Await Answer state\n");
  }
  else
  {
    /* FID 14841.0 */
    HNDL_MGCT_NOT_FOR_ADID(kind, con, event, bcm);
    CCLOGERR("Error: MGCT should never come back with NotifyType = %d\n", kind);
    ABORT_DEBUG;
  }
 }
  RETVALUE(ROK);
}


/* 
*  
*      Fun:   ccConE62S51
*  
*      Desc:  Connection state function
*             event - CCE_NOTIFYFROMMGCT
*             state - CCS_AWTBUSYREL_IC
* 
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes:
*   
*      File:  cc_bdy2_2.c
*   
*/
PUBLIC  S16 ccConE62S51
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  MgctTxnInd        *mgctTxnInd    = NULLP;
  U16                megacoErr     = 0;
  MgctNotifyKind_e   kind          = MGCT_NOTIFY_KIND_UNKNOWN;
  S16                ret           = RFAILED;

  CCDP(AIN_DBG_LEVEL_1, "ccConE62S51 icSu = 0x%lx, ogSu = 0x%lx\n",
       con->icSuConnId, con->ogSuConnId);

  if ((con == NULLP) || (event == NULLP) || (bcm == NULLP))
  {
    CCLOGERR("Error: Null pointers con = 0x%lx, event = 0x%lx, bcm = 0x%lx\n",
             (U32)con, event, (U32)bcm);
    RETVALUE(RFAILED);
  }
  mgctTxnInd = (MgctTxnInd *) event;
  ret = ccHandleMgctNotify(con, (MgctTxnInd *) event);
  if (ret != ROK)
  {
    CCLOGERR("Error: ccHandleMgctNotify failed icSu = 0x%lx,"
             "suCtxId 0x%lx, contextId = 0x%lx\n", con->icSuConnId,
             (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0), con->mgBlock->mgCtx->contextId);
    ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
    RETVALUE(ROK);
  }
  mgctGetNotifyReqKind(mgctTxnInd, &megacoErr, &kind, NULLP);
  if (kind == MGCT_NOTIFY_KIND_SIGNAL_COMPLETION)
  {
    CCDP(AIN_DBG_LEVEL_1, "This is a case of X-Resource Annc 0x%lx\n",
         con->icSuConnId);
    ccPostAnnoucementProcessingForConfCalls(con);
    RETVALUE(ROK);
  }
  else
  {
    /* FID 14841.0 */
    HNDL_MGCT_NOT_FOR_ADID(kind, con, event, bcm);
    CCLOGERR("Error: MGCT should never come back with NotifyType = %d\n", kind);
    ABORT_DEBUG;
  }
  RETVALUE(ROK);
}

/* 
*  
*      Fun:   ccConE39S11
*  
*      Desc:  Connection state function
*             event - Connection progress status indication
*             state - 
* 
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes:
*   
*      File:  cc_bdy2_2.c
*   
*/  
PUBLIC  S16 ccConE39S11
(   
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)   
{
    SvcHeader_t       *ctSvcHeader    = NULLP;
    Continue_t        *pContinue      = NULLP;
    S16   mgret = ROK;

    CCDP(AIN_DBG_LEVEL_1, "ccConE39S11:icSu:%ld, ogSu:%ld, suCtxId:%ld,"
         " icTransId:%ld, con->state= %d\n", con->icSuConnId, con->ogSuConnId,
         (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0), con->icBcm.svcHeader.transId, con->state);

    ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
  
    /* only handle error cases */
    ctSvcHeader = (SvcHeader_t *) event;
    pContinue   = (Continue_t *) event2;
    if(pContinue->errorCode != FC_NOT_APPLICABLE)
    {
        CCLOGERR("ERROR FROM FIC = %ld\n", pContinue->errorCode);
        con->tempHolderForReleaseCause = pContinue->errorCode;
  
        if  (con->icProtType == CC_LOCAL_SWT_PROT)
          ccRemoveCaleaAssnWithTappedBcm(con);
  
        con->ainInfo.termInd = EXCEPTION_IND;
        con->ogBcm.closeCause = CscRmtBusy;
        ccCloseTBcmTransaction(con);
        if (CC_IS_CONTEXT_PRESENT(con))
        {
            ccQueueSubOnOGCleanUp(con);
            con->relPend = OUTRELANDINFAILCOND; 

            if (con->lastState == CCS_AWTMGCTCFMFORMODIFY2_CTR)
            {
                /* Used for bug60709 */
                CC_STATE_CHANGE(con, CCS_AWTSWTCFM_SENDBOTHREL);
            }
            else
            {
            CC_STATE_CHANGE(con, CCS_AWTSWTCFM_SENDONEREL);
            }
            con->mgBlock->con = con;
            mgret = ccSendMgiQueue(0, con->mgBlock);
            if (mgret == ROK)
                RETVALUE(ROK);
            else
            {
              CCLOGERR("Error: MGI failed:icSu:%ld, ogSu:%ld, suCtxId:%ld,"
                       " icTransId:%ld\n", con->icSuConnId, con->ogSuConnId,
                     con->mgBlock->mgCtx->suCtxId, con->icBcm.svcHeader.transId);
              cleanUpMgQ(con);
              ccHandleCleanUpConCb(con, TRUE, AM_CV_TMPFAIL);
              RETVALUE(ROK);
            }
        }
        ccFreeTheRscFromCon(con, OUTTOINC);
        ccProcessFailCnd(con, pContinue->errorCode, TRUE);
        RETVALUE(ROK);
    }
    /* CPDI: MCAO-08/29/03 no error case, all event rearmed, only need reset the con->state */
    CC_STATE_CHANGE(con, CCS_ANSWERED);
    ccProcessCcChangeforX2SIG(con); /* FID 15083.0 */

    if ((!ccIsSwitchingLocallyDone(&con->icBcm)) &&
        (!ccIsSwitchingLocallyDone(&con->ogBcm)) && (CC_IS_CONTEXT_PRESENT(con)))
    {
        CCDP(AIN_DBG_LEVEL_0, "%s: Re-Negotiating On-Net: icSu:0x%lx,"
             " ogSu:0x%lx, suCtxId:0x%lx\n", __FUNCTION__, (U32)con->icSuConnId,
             (U32) con->ogSuConnId, (U32)con->mgBlock->mgCtx->suCtxId);
        ccCreateBearer(&(con->icBcm), &(con->ogBcm), NULLP, NULLP, con->state,
                       FALSE);
    }

    RETVALUE(ROK);
}   
/* 
*  
*      Fun:   ccConE39S38
*  
*      Desc:  Connection state function
*             event - Continue
*             state - CCS_AWTRSPFORDPE37
* 
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes:
*   
*      File:  cc_bdy2_2.c
*   
*/  
PUBLIC  S16 ccConE39S38
(   
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)   
{
  SvcHeader_t       *ctSvcHeader    = NULLP;
  AinBCM_t          *pBcm           = NULLP;
  S16                ret = ROK, mgret = ROK;
  Continue_t        *pContinue      = NULLP; 

  CCDP(AIN_DBG_LEVEL_1, "ccConE39S38:icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld, con->state= %d\n", con->icSuConnId, con->ogSuConnId,
       (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0), con->icBcm.svcHeader.transId, con->state);

  ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));

  ctSvcHeader = (SvcHeader_t *) event;
    pContinue   = (Continue_t *) event2;
    if(pContinue->errorCode != FC_NOT_APPLICABLE)
  {
        CCLOGERR("ERROR FROM FIC = %ld\n", pContinue->errorCode);
        con->tempHolderForReleaseCause = pContinue->errorCode;
  
    if  (con->icProtType == CC_LOCAL_SWT_PROT)
      ccRemoveCaleaAssnWithTappedBcm(con);
  
    con->ainInfo.termInd = EXCEPTION_IND;
    con->ogBcm.closeCause = CscRmtBusy;
    ccCloseTBcmTransaction(con);
    if (CC_IS_CONTEXT_PRESENT(con))
    {
#if 1 /* EARLY_ACM */
      /* Need to clean all the legs in the context */
      CC_RESET_ANY_EARLY_ACM_FLAGS(con);
#endif
      ccQueueSubOnOGCleanUp(con);
      con->relPend = OUTRELRSPANDINTONE;

      CC_STATE_CHANGE(con, CCS_AWTSWTCFM_SENDONEREL);
      con->mgBlock->con = con;
      mgret = ccSendMgiQueue(0, con->mgBlock);
      if (mgret == ROK)
        RETVALUE(ROK);
      else
      {
        CCLOGERR("Error: MGI failed:icSu:%ld, ogSu:%ld, suCtxId:%ld,"
                 " icTransId:%ld\n", con->icSuConnId, con->ogSuConnId,
                 con->mgBlock->mgCtx->suCtxId, con->icBcm.svcHeader.transId);
        cleanUpMgQ(con);
        ccHandleCleanUpConCb(con, TRUE, AM_CV_TMPFAIL);
        RETVALUE(ROK);
      }
    }
    if (con->ogRscVal)
    {
      con->direction = OUTTOINC;
      if (con->noRspReqd == FALSE) ccSendReleaseRsp(con);
      ccDeallocateResource(con, CC_OUTGOING);
      con->ogRscVal = FALSE;
    }
    ccProcessFailCnd(con, pContinue->errorCode, TRUE);

    RETVALUE(ROK);
  }

  /* examine whether to reactivate digit collection */
    pBcm = &con->icBcm;
  if ((con->ainInfo.ODTMFDigitString.pres) &&
        ((pBcm->nel[EReqODTMFEntered] == NEL_NOTIFY) ||
         (pBcm->nel[EReqODTMFEntered] == NEL_REQUEST)))
  {
    ret = ccReactivateDTMFDet(con);
    /* if failed to reactivate DTMFdet, it should back to state
       before report O_DTMF_Entered */
    if (ret == ROK)
      RETVALUE(ROK);
  }
  if (con->tmdRelApplied == CC_IC_TMD_RELEASE_CPDI)
  {
    ccStartConTmr(CC_TMR_TMDREL, con, (PTR)&ccCp.genCfg);
    CC_STATE_CHANGE(con, CCS_AWT_TMDRELEXP);
  }
  else if (con->icBcm.pic == PicOActive)
  {
    CC_STATE_CHANGE(con, CCS_ANSWERED);
    ccProcessCcChangeforX2SIG(con); /* FID 15083.0 */
  }
  else
  {
    CC_STATE_CHANGE(con, CCS_AWTANS);
  }
  RETVALUE(ROK);
}

/* 
*  
*      Fun:   ccConE11S38
*  
*      Desc:  Connection state function
*             event - Connection progress status indication
*             state - 
* 
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes:
*   
*      File:  cc_bdy2_2.c
*   
*/  
PUBLIC  S16 ccConE11S38
(   
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)   
{
  MgctTxnCfm          *txnCfm  = NULLP;
  S16                  ret     = RFAILED;

  CCDP(AIN_DBG_LEVEL_1, "ccConE11S38: icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", con->icSuConnId, con->ogSuConnId,
       (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0), con->icBcm.svcHeader.transId);
  txnCfm   = (MgctTxnCfm*) event;

  ret = ccHandleMgResponse(con, txnCfm);
  if (ret != ROK)
  {
      CCLOGERR( "ERROR: Response Handling FAILED\n");
  //  to be more fault-tolerant?  ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
  //  RETVALUE(RFAILED);
  }
  
  /* recover original state */
  CC_STATE_CHANGE(con, con->lastState);

  RETVALUE(ROK);
}

/* 
*  
*      Fun:   ccConE37S38
*  
*      Desc:  Connection state function
*             event - Connection progress status indication
*             state - 
* 
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes:
*   
*      File:  cc_bdy2_2.c
*   
*/  
PUBLIC  S16 ccConE37S38
(   
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)   
{
  SvcHeader_t        *arSvcHeader           = NULLP;
  AnalyzeRoute_t     *pAnalyzeRoute         = NULLP;
  S16                 mgret                 = RFAILED;

  if ((con == NULLP) || (event == NULLP))
  {
    CCLOGERR("Critical ERROR: Null pointers:%lx, %ld\n", (U32)con, event);
    RETVALUE(RFAILED);
  }

  CCDP(AIN_DBG_LEVEL_1, "ccConE37S38: icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", con->icSuConnId, con->ogSuConnId,
       (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0), con->icBcm.svcHeader.transId);

  pAnalyzeRoute = (AnalyzeRoute_t *) event2;
  arSvcHeader = (SvcHeader_t *) event;

  ccCutBillingRecordIfNeeded(con, pAnalyzeRoute->scpResponse, OUTTOINC);
  ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));

  /* CPDI need restore ConInd event before mapping AR routing info into it */
  ccRestoreEvntHold(con);
  ccExtractAnalyzeRteParam(pAnalyzeRoute, bcm);
  ccExtractSvcHeader(arSvcHeader, bcm, MSG_Analyze_Route);

  ccStopConTmr(con, AIN_O_NO_ANS_TMR);
  ccStopConTmr(con, AIN_T_NO_ANS_TMR);
  /* STOP the AWAIT ANSWER Timer if running
   *
   * changes made to Support Configurable Answer Timer in GCC, RV Suresh, 5.0 
   */
  ccStopConTmr(con, CC_TMR_AWAITANS); 
  ccDeleteBufferedEvent(con, con->icSuConnId, CCE_ONOANSEXP);
  ccDeleteBufferedEvent(con, con->ogSuConnId, CCE_TNOANSEXP);


  /* reroute call */
  if (arSvcHeader->lastTransactionFlag == TRUE)
    ccDeleteBcm(&(con->icBcm));

  con->ogBcm.closeCause = CscRmtBusy;
  ccCloseTBcmTransaction(con);

  if (CC_IS_CONTEXT_PRESENT(con))
  {
    ccQueueSubOnOGCleanUp(con);
    /* CPDI_FIX: need to decide whether received an RelInd from outgoing before, or
       it's a case need initiate a release internally */
    if (CC_IS_REL_IN_EVNT_HOLD(con))
       con->relPend = OUTRELRSPANDROUTE;
    else
    {
       if (con->ccRelEvntHold)
    con->relPend = OUTRELRSPANDROUTE;
       else
       {
          /* put default cause value here */
          con->tempHolderForReleaseCause = AM_CV_NORMCALLCLR;
          con->relPend = OUTRELANDROUTE;
       }
    }
    CCDP(AIN_DBG_LEVEL_1, "%s: icSu:%ld, ogSu:%ld, suCtxId:%ld,"
         " con->relPend:%d\n", __FUNCTION__, con->icSuConnId, con->ogSuConnId,
         con->mgBlock->mgCtx->suCtxId, con->relPend);    
    
    CC_STATE_CHANGE(con, CCS_AWTSWTCFMFORDP);
    con->mgBlock->con = con;
    ccChangeCallStateOfAssociatedCalls(&(con->icBcm), CCS_CANNOTPROCESSEVENTS);
    mgret = ccSendMgiQueue(0, con->mgBlock);
    if (mgret == ROK)
      RETVALUE(ROK);
    else
    {
      CCLOGERR("Error: MGI failed:icSu:%ld, ogSu:%ld, suCtxId:%ld,"
               " icTransId:%ld\n", con->icSuConnId, con->ogSuConnId,
               con->mgBlock->mgCtx->suCtxId, con->icBcm.svcHeader.transId);
      ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
      if (con->ogRscVal)
      {
        ccCloseTBcmTransaction(con);
        con->direction = OUTTOINC;
        if (con->noRspReqd == FALSE) ccSendReleaseRsp(con);
        ccDeallocateResource(con, CC_OUTGOING);
        con->ogRscVal = FALSE;
      }
      cleanUpMgQ(con);
      ccHandleCleanUpConCb(con, TRUE, AM_CV_TMPFAIL);
      RETVALUE(ROK);
    }
  }

  /* Clean outgoing and ReRoute call */
  if (con->ogRscVal)
  {
    ccCloseTBcmTransaction(con);
    con->direction = OUTTOINC;
    if (con->noRspReqd == FALSE) ccSendReleaseRsp(con);
    ccDeallocateResource(con, CC_OUTGOING);
    con->ogRscVal = FALSE;
  }

  /* re-Route the call to original party number*/
  initOgBcm(&con->ogBcm);
  ccRouteCall(con, FALSE);

  RETVALUE(ROK);
}

/* 
*  
*      Fun:   ccConE08S38
*  
*      Desc:  Connection state function
*             event - Connection progress status indication
*             state - 
* 
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes:
*   
*      File:  cc_bdy2_2.c
*   
*/  
PUBLIC  S16 ccConE08S38
(   
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)   
{   
  CCDP(AIN_DBG_LEVEL_1, "ccConE08S38:icSu:%ld, ogSu:%ld, icProt:%d \n", con->icSuConnId,
       con->ogSuConnId, con->icProtType);

  /* generate Timeout to Fic */
  RETVALUE(ROK);
}   


/* 
*  
*      Fun:   ccConE31S82
*  
*      Desc:  Connection state function
*             event - SF Connection progress status indication
*             state - 
* 
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes:
*   
*      File:  cc_bdy2_2.c
*   
*/  
PUBLIC  S16 ccConE31S82
(   
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)   
{
#if 0 /* PENDING */
  S16 ret  = ROK;
  MgitInd  *MgctTxnInd;
  
  CCDP(AIN_DBG_LEVEL_1, "ccConE31S82:icSu:%ld, ogSu:%ld, icProt:%d \n", con->icSuConnId,
       con->ogSuConnId, con->icProtType);

  /* the success of EvntInd */
  MgctTxnInd     = (MgitInd *) event;
  
  /* release conference reource if ANN has completed */
  if (MgctTxnInd->type == MGIT_ANN_COMPLETE_IND)
    {
      ccUpdateMgQForSubAnnRsc(con->mgBlock->mgCtx->suCtxId, con);
      
      con->lastState                 = con->state;
      con->relPend                   = PALYEDANNC;
      
      /* keep the same state, relPend will lead to next state */
      con->mgBlock->con = con;
      ret = ccSendMgiQueue(con->mgBlock->mgCtx->suCtxId, con->mgBlock);
      
      if (ret != ROK)
      {
          CCLOGERR( "ERROR: ccSendMgiQueue Failed\n");
          ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
          RETVALUE(RFAILED);
      } 
    }
  else if (MgctTxnInd->type == MGIT_SVC_CHANGE_IND)
  {
    CCDP(AIN_DBG_LEVEL_1, "ccConE31S82:%ld,%ld,%ld\n", con->icSuConnId,
                          con->ogSuConnId, con->mgBlock->mgCtx->suCtxId);
    /* Replication is not done, as this happens before we replicate
     * the CS2 to the standby, the relcon's and the other, disc leg,
     * and merge calls will take care of this */
    ccKillCallOnThisCon(con);
    RETVALUE(ROK);
  }
  else
  {
    CCLOGERR("Invalid MgctTxnInd->type:%d, %ld, %ld\n", MgctTxnInd->type,
                                    con->icSuConnId, con->ogSuConnId);
    RETVALUE(RFAILED);
  }
  
#endif /* PENDING */
  RETVALUE(ROK);
}   

/* 
*  
*      Fun:   ccConE08S10
*  
*      Desc:  Connection state function
*             event - Connection progress status indication
*             state - 
* 
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes:
*   
*      File:  cc_bdy2_2.c
*   
*/  
PUBLIC  S16 ccConE08S10
(   
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)   
{   
  SvcHeader_t       header;
  Timeout_t         timeout;

  CCDP(AIN_DBG_LEVEL_1, "ccConE08S10:icSu:%ld, ogSu:%ld, icProt:%d \n", con->icSuConnId,
       con->ogSuConnId, con->icProtType);
  /* CPDI_FIX 28361 */
  if (zcCb.protState != ACTIVE)
     RETVALUE(ROK);

  /* generate Timeout to Fic */
  /* Initializing the message buffers */
  cmMemset((U8 *)&header, 0, sizeof(SvcHeader_t));
  cmMemset((U8 *)&timeout, 0, sizeof(Timeout_t));
  
  /* CPDI_FIX: we need relinguish old rel event in the buffer */
  /* logic: whenever ccRelEvntHold != NULL, ccEvntHold isn't CCE_RELIND */ 
  CC_FREE_REL_EVNT_HOLD(con);
    
  ccFillSvcHeader(&header, &(con->icBcm), MSG_Timeout);
  CCDP(AIN_DBG_LEVEL_0," Filled SvcHeader\n");
  
  ccFillTimeoutParam(&timeout, &(con->icBcm));
  CCDP(AIN_DBG_LEVEL_0," Filled Timeout, TransID=%ld\n", header.spTransId);
  con->lastState = con->state;
  ccChangeCallStateOfAssociatedCalls(&(con->icBcm), CCS_CANNOTPROCESSEVENTS);
  CC_STATE_CHANGE(con, CCS_AWTRSPTIMEOUT);
  CCDP(AIN_DBG_LEVEL_3," Sending Timeout, TransID=%ld \n", header.spTransId);
  CcUiFctTimeout(&timeout, &header);
  
  RETVALUE(ROK);
}   

/* 
*  
*      Fun:   ccConE62S53
*  
*      Desc:  Connection state function
*             event - CCE_NOTIFYFROMMGCT
*             state - CCS_AWT_TMDRELEXP
* 
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes:
*   
*      File:  cc_bdy2_2.c
*   
*/  
PUBLIC  S16 ccConE62S53
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)   
{
  MgctTxnInd        *mgctTxnInd    = NULLP;
  ProtType           ogProtType    = 0;
  U16                megacoErr     = 0;
  MgctNotifyKind_e   kind          = MGCT_NOTIFY_KIND_UNKNOWN;
  S16                ret           = RFAILED;

  CCDP(AIN_DBG_LEVEL_1, "ccConE62S53 icSu = 0x%lx, ogSu = 0x%lx\n",
       con->icSuConnId, con->ogSuConnId);

  if ((con == NULLP) || (event == NULLP) || (bcm == NULLP))
  {
    CCLOGERR("Error: Null pointers con = 0x%lx, event = 0x%lx, bcm = 0x%lx\n",
             (U32)con, event, (U32)bcm);
    RETVALUE(RFAILED);
  }
  mgctTxnInd = (MgctTxnInd *) event;
  ret = ccHandleMgctNotify(con, (MgctTxnInd *) event);
  if (ret != ROK)
  {
    CCLOGERR("Error: ccHandleMgctNotify failed icSu = 0x%lx,"
             "suCtxId 0x%lx, contextId = 0x%lx\n", con->icSuConnId,
             (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0), con->mgBlock->mgCtx->contextId);
    ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
    RETVALUE(ROK);
  }
  mgctGetNotifyReqKind(mgctTxnInd, &megacoErr, &kind, NULLP);
  if (kind == MGCT_NOTIFY_KIND_DIGIT_COMPLETION)
  {
    if ((con->ainInfo.ODTMFDigitString.pres == PRSNT_NODEF) &&
        (con->icBcm.nel[EReqODTMFEntered] == NEL_REQUEST))
    {
      ccStopConTmr(con, CC_TMR_TMDREL);
      ret = ccProcessODTMFEntered(&con->icBcm);
      if (ret != ROK)
      {
        CCLOGERR("Error: ccProcessODTMFEntered failed icSu = 0x%lx\n",
                 con->icSuConnId);
        ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
      }
      RETVALUE(ROK);
    }
    else if (ogProtType == CC_SIP)
    {
    }
    else
    {
      CCLOGERR("Error: DTMF detected for unknow application icSu 0x%lx\n",
               con->icSuConnId);
      ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
      RETVALUE(ROK);
    }
  }
  else
  {
    /* FID 14841.0 */
    HNDL_MGCT_NOT_FOR_ADID(kind, con, event, bcm);
    CCLOGERR("Error: MGCT should never come back with NotifyType = %d\n", kind);
    ABORT_DEBUG;
  }

  RETVALUE(ROK);
}   



/* 
*  
*      Fun:   ccConE70S10
*  
*      Desc:  Connection state function
*             event - CCE_ACKOWNLEDGE
*             state - CCS_ANSWERED
* 
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes:
*   
*      File:  cc_bdy2_2.c
*   
*/  
PUBLIC  S16 ccConE70S10
(   
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)   
{   
  CCDP(AIN_DBG_LEVEL_1, "ccConE70S10:icSu:%ld, ogSu:%ld, icProt:%d \n", con->icSuConnId,
       con->ogSuConnId, con->icProtType);
  /* not necessary */
  RETVALUE(ROK);
}

PUBLIC S16 ccConE69SXX
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  U8 *fraudTmrType = NULLP; 
  S16 ret = RFAILED;
  CcFraudAlarm   fraudAlarm;

  CCDP(AIN_DBG_LEVEL_0,"Entered  ccConE69SXX\n");

  /* Feature 1789 - German ISUP */
  /* stop the Toiw2 Timer if running */
  ccStopConTmr(con, CC_TMR_TOIW2);

  if (zcCb.protState == ACTIVE)
  {
    fraudTmrType = (U8*) event;
    CCDP(AIN_DBG_LEVEL_0,"ccConE69SXX: fraudTmrType = %lx\n", (U32)*fraudTmrType);

    if (*fraudTmrType == SCR_FRAUD_TMR)
    {
      /* E69 event due to SCR_FRAUD_TMR */
      if (con->fraudPrflbmap & SET_SCR_REL_FLAG)
      {
        ccStopConTmr(con, TMR_SETUP); 

        con->fraudPrflbmap |= SET_SCR_SWITCH_INITREL_FLAG;
        con->callDtlInfo.cm.forcedRelType = CALL_REL_FRAUD_SCR_TMR_EXP;
        if (((con->icBcm.cc) && isThisBcmInMPartyCall(&con->icBcm)) ||
            ((con->ogBcm.cc) && isThisBcmInMPartyCall(&con->ogBcm)))
        { 
          ccHandleCleanUpConCb(con, FALSE, AM_CV_TMPFAIL);
        }
        else
        {
#if 0
          ccDisconnectCallForIcBCM(con, event); 
#endif
          ccStartConTmr(TMR_RELEASE, con, (PTR)&ccCp.genCfg);

          if (CC_IS_CONTEXT_PRESENT(con))
          {
            /* Blow off the switching and Release the Call */
            ccQSubBasedOnCcn(con);  /* FID 15261.0 */

            /* disconnect the outgoing leg but, continue processing
             * failure condition on incoming BCM */
            con->relPend = OUTRELANDSCRINFAILCOND;

            CC_STATE_CHANGE(con, CCS_AWTSWTCFM_SENDONEREL);
            con->mgBlock->con = con;
            ret = ccSendMgiQueue(0, con->mgBlock);
            if (ret != ROK)
            {
              CCLOGERR( "**** ERROR: can't do ccSendMgiQueue *******\n");
              cleanUpMgQ(con);
              ccHandleCleanUpConCb(con, TRUE, AM_CV_TMPFAIL);
            }
            else
              RETVALUE(ROK);
          }
          else
          {
            ccFreeTheRscFromCon(con, OUTTOINC);
            ccProcessFailCnd(con, FC_SCR_FRAUD_REL, TRUE);
            RETVALUE(ROK);
          }
        }
      }
      /* FID16259.0 generate alarm if alarm is set and not release call */
      else if (CC_IS_CALL_FLAG2(con, SET_SCR_ALARM_FLAG))
      {
        /* If Fraud alarm is not started, start it */
        if (glFraudAlarmCnt == 0 )
        {
          glFraudAlarmCnt++;
          con->flag2 |= CC_SCR_ALARM_GENERATED;

          CCDP(AIN_DBG_LEVEL_4,"Start FRAUD CALL Alarm count %ld, timer %d wait %d\n", 
               glFraudAlarmCnt, CC_FRAUD_ALARM_THRESH_TIMER, CC_FRAUD_ALARM_THRESH_WAIT);
          ccStartCcAlarmTimer(CC_FRAUD_ALARM_THRESH_TIMER, NULLP, CC_FRAUD_ALARM_THRESH_WAIT);

          CCDP(AIN_DBG_LEVEL_4,"FRAUD CALL Generating Alarm callAid %ld glFraudAlarmCnt %ld\n", 
               con->icSuConnId, glFraudAlarmCnt);
          cmMemset((U8 *)&fraudAlarm, 0, sizeof(CcFraudAlarm));
          fraudAlarm.ldcFraudCnt = glFraudAlarmCnt;

          ccGenAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_GCC_FRAUD_ALARM,
              LCM_CAUSE_GCC_FRAUD_ALARM, (U8 *) &fraudAlarm);
          glFraudAlarmCntChg = FALSE;
        }
        else
        {
          glFraudAlarmCnt++;
          con->flag2 |= CC_SCR_ALARM_GENERATED;
          glFraudAlarmCntChg = TRUE;
          CCDP(AIN_DBG_LEVEL_4,"FRAUD CALL CallAid %ld glFraudAlarmCnt %ld glFraudAlarmCntChg %d\n", 
               con->icSuConnId, glFraudAlarmCnt, glFraudAlarmCntChg);
        }
      }
      /*FID 16259.0*/
      con->flag2 |= CC_FRAUDCALL_EXPIRED;
      CCDP(AIN_DBG_LEVEL_4,"Set fraudcall expire flag, con->flag2=%lx\n", con->flag2);
    }
    else if (*fraudTmrType == ANS_FRAUD_TMR)
    {
      /* E68 due to ANS_FRAUD_TMR */
      /* This call is fraud , insert into ccFraudCallTbl hash list */

      ccSaveFraudCallInfo(con);

      if (con->fraudPrflbmap & SET_ANS_LOGGING_FLAG)
        ccLogFraudCallDetails(con, FALSE); 

      if (con->fraudPrflbmap & SET_ANS_REL_FLAG)
      {
        con->fraudPrflbmap |= SET_ANS_SWITCH_INITREL_FLAG; 
        con->callDtlInfo.cm.forcedRelType = CALL_REL_FRAUD_ANS_TMR_EXP;
      }
      else
      { 
        /* Bug# 26435 */
        /* Generate ZERO duration CDR only when it is not forcefully released */
        if (con->fraudPrflbmap & SET_ANS_ZERODUR_CDR_FLAG)
          ccGenZeroDurFraudCDR(con);
      }     

      if (con->fraudPrflbmap & SET_ANS_REL_FLAG)
      {
        ccStopConTmr(con, TMR_SETUP); 

        if (((con->icBcm.cc) && isThisBcmInMPartyCall(&con->icBcm)) ||
            ((con->ogBcm.cc) && isThisBcmInMPartyCall(&con->ogBcm)))
        {
          ccHandleCleanUpConCb(con, FALSE, AM_CV_TMPFAIL);
        }
        else
        {
#if 0
          ccDisconnectCallForIcBCM(con, event); 
#endif
          ccStartConTmr(TMR_RELEASE, con, (PTR)&ccCp.genCfg);

          if (CC_IS_CONTEXT_PRESENT(con))
          {
            /* Blow off the switching and Release the Call */
            ccQSubBasedOnCcn(con);  /* FID 15261.0 */

            /* disconnect the outgoing leg but, continue processing
             * failure condition on incoming BCM */
            con->relPend = OUTRELANDANSINFAILCOND;

            CC_STATE_CHANGE(con, CCS_AWTSWTCFM_SENDONEREL);
            con->mgBlock->con = con;
            ret = ccSendMgiQueue(0, con->mgBlock);
            if (ret != ROK)
            {
              CCLOGERR( "**** ERROR: can't do ccSendMgiQueue *******\n");
              cleanUpMgQ(con);
              ccHandleCleanUpConCb(con, FALSE, AM_CV_TMPFAIL);
            }
            else
              RETVALUE(ROK);
          }
          else
          {
            ccFreeTheRscFromCon(con, OUTTOINC);
            ccProcessFailCnd(con, FC_ANS_SUPER_REL, TRUE);
            RETVALUE(ROK);
          }
        }
      }
    }
    else
      CCLOGERR("**%s(), Invalid Fraud Timer Type:: icSu: %ld\n",
               __FUNCTION__, con->icSuConnId);
  }
  else
  {
    /* Screen Digit Fraud Call Timer expired on Standby */ 
    CCDP(AIN_DBG_LEVEL_4,"FRAUD  CALL TIMER  expired on STANDBY \n");
  }

  RETVALUE(ROK);
}


/*                            
*
*      Fun:   ccConE47S40
*
*      Desc:  Connection state function
*             event - Send_To_Resource
*             state - 40-CCS_AWTRSPFORDPE30
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE47S40
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  SvcHeader_t        *srSvcHeader           = NULLP;
  S16                 mgret                 = RFAILED;
  S16                 ret                   = RFAILED;

  if ((con == NULLP) || (event == NULLP))
  {
    CCLOGERR("Critical ERROR: Null pointers:%ld, %ld\n", (U32)con, (U32)event);
    RETVALUE(RFAILED);
  }
  CCDP(AIN_DBG_LEVEL_1, "ccConE47S40:icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", con->icSuConnId, con->ogSuConnId,
       (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0), con->icBcm.svcHeader.transId);

  ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
  srSvcHeader = (SvcHeader_t *) event;
  ccStopConTmr(con, AIN_O_NO_ANS_TMR);
  ccStopConTmr(con, AIN_T_NO_ANS_TMR);
  /* STOP the AWAIT ANSWER Timer if running
   *
   * changes made to Support Configurable Answer Timer in GCC, RV Suresh, 5.0 
   */
  ccStopConTmr(con, CC_TMR_AWAITANS); 
  ccDeleteBufferedEvent(con, con->icSuConnId, CCE_ONOANSEXP);
  ccDeleteBufferedEvent(con, con->ogSuConnId, CCE_TNOANSEXP);


  if (srSvcHeader->lastTransactionFlag == TRUE)
    ccDeleteBcm(&(con->ogBcm));

  con->ogBcm.closeCause = CscRmtBusy;
  ccCloseTBcmTransaction(con);
  con->lastState = con->state;
  con->anncBlock.stateForAnnc = con->state;
  if (CC_IS_CONTEXT_PRESENT(con))
  {
    ccQueueSubOnOGCleanUp(con);
    ccChangeCallStateOfAssociatedCalls(&(con->icBcm), CCS_CANNOTPROCESSEVENTS);
    con->relPend = NORELANDPLAYANNC;
    CC_STATE_CHANGE(con, CCS_AWTSWTCFMFORDP);
    con->mgBlock->con = con;
    mgret = ccSendMgiQueue(0, con->mgBlock);
    if (mgret == ROK)
      RETVALUE(ROK);
    else
    {
      CCLOGERR("Error: MGI failed:icSu:%ld, ogSu:%ld, suCtxId:%ld,"
               " icTransId:%ld\n", con->icSuConnId, con->ogSuConnId,
               con->mgBlock->mgCtx->suCtxId, con->icBcm.svcHeader.transId);
      ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
      cleanUpMgQ(con);
      ccHandleCleanUpConCb(con, TRUE, AM_CV_TMPFAIL);
      RETVALUE(ROK);
    }
  }

  initOgBcm(&con->ogBcm);
  ret = ccSendToResource(&(con->icBcm));
  if (ret != ROK)
  {
    CCLOGERR("Error: ccSendToResource failed:icSu:%ld,ogSu:%ld,icTrsId:%ld\n",
             con->icSuConnId, con->ogSuConnId, 
             (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0));
    ccHandleCleanUpConCb(con, TRUE, AM_CV_TMPFAIL);
  }
  RETVALUE(ROK);
}


/*                            
*
*      Fun:   ccConE47S46
*
*      Desc:  Connection state function
*             event - Send_To_Resource
*             state - 46-CCS_AWTRSPFORDPE32
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE47S46
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  SvcHeader_t        *srSvcHeader           = NULLP;
  CcConCb            *CS1con              = NULLP;
  S16                 mgret                 = RFAILED;
  S16                 ret                   = RFAILED;
  CcMidCallReqEvnt    midCallReq;

  if ((con == NULLP) || (event == NULLP))
  {
    CCLOGERR("Critical ERROR: Null pointers:%ld, %ld\n", (U32)con, (U32)event);
    RETVALUE(RFAILED);
  }
  CCDP(AIN_DBG_LEVEL_1, "ccConE47S46:icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", con->icSuConnId, con->ogSuConnId,
       (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0), con->icBcm.svcHeader.transId);

  ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
  srSvcHeader = (SvcHeader_t *) event;
  ccStopConTmr(con, AIN_O_NO_ANS_TMR);
  ccDeleteBufferedEvent(con, con->icSuConnId, CCE_ONOANSEXP);


  if (ccCheckEventInBuffer(&(con->ogBcm), CCE_CONCFM))
  {
    /* Drop the STR Response and process the Connect Confirm */
    DP("Dropping the STR and processing the ConCfm message:%ld, %ld\n",
       con->icSuConnId, con->ogBcm.svcHeader.transId);
    CC_STATE_CHANGE(con, CCS_AWTANS);
    RETVALUE(ROK);
  }

  con->ogBcm.closeCause = CscRmtBusy;
  if ((con->ogBcm.cc) && ((con->ogBcm.cc->ccId == CiCallWaiting) ||
                         (con->ogBcm.cc->ccId == CiCallWaitingComplement)))
  {
    ccBuildMidCalReqForMC_CANCEL_ALL(&midCallReq);
    CS1con = con->ogBcm.cc->legId02->con;
    if (CS1con == NULLP)
    {
      CCLOGERR("Error : CS1con is NULLP\n");
      ccHandleCleanUpConCb(con, TRUE, AM_CV_TMPFAIL);
      RETVALUE(ROK);
    }
    if (con->ogBcm.cc->legId02 ==  &(CS1con->icBcm))
    {
      if ((!(ccIsIPSubscriber(&(CS1con->icBcm)))) &&
          (con->icBcm.pic != PicOActive))
        CcLiCctMidCallReq(&CS1con->icSapCb->pst, CS1con->icSapCb->spId,
                          CS1con->icSpConnId, CS1con->icSuConnId,
                          CS1con->icProtType, &midCallReq);
      else
      {
        /* Send Cancel CWT tone for IP Subscriber */
      }
      CS1con->icBcm.cc->ccId    = CiStable2Party;
      con->ogBcm.cc->legId02 = NULLP;
      con->ogBcm.cc->legId2  = NULLP;
      con->ogBcm.cc->legId01 = &(CS1con->icBcm);
      con->ogBcm.cc          = NULLP;
    }
    else if (con->ogBcm.cc->legId02 == &(CS1con->ogBcm))
    {
      if ((!(ccIsIPSubscriber(&(CS1con->ogBcm)))) &&
          (con->icBcm.pic != PicOActive))
      {
        CcLiCctMidCallReq(&CS1con->ogSapCb->pst, CS1con->ogSapCb->spId,
                          CS1con->ogSpConnId, CS1con->ogSuConnId,
                          CS1con->ogProtType, &midCallReq);
      }
      else
      {
        /* Send Cancel CWT tone for IP Subscriber */
      }
      CS1con->ogBcm.cc->ccId    = CiStable2Party;
      con->ogBcm.cc->legId02 = NULLP;
      con->ogBcm.cc->legId2  = NULLP;
      con->ogBcm.cc->legId01 = &(CS1con->ogBcm);
      con->ogBcm.cc          = NULLP;
    }
    else
    {
      CCLOGERR("Error: Unable to find bcmType\n");
      RETVALUE(RFAILED);
    }
    /* This is needed, else the cc will be dropped */
    con->ogBcm.cc = NULLP;
    CC_CLEAN_FIC_TXN(&(con->ogBcm));
  }

  if (srSvcHeader->lastTransactionFlag == TRUE)
    ccDeleteBcm(&(con->ogBcm));

  con->lastState = con->state;
  con->anncBlock.stateForAnnc = con->state;
  if (CC_IS_CONTEXT_PRESENT(con))
  {
    ccQueueSubOnOGCleanUp(con);
    ccChangeCallStateOfAssociatedCalls(&(con->icBcm), CCS_CANNOTPROCESSEVENTS);
    if (con->ogRscVal)
      con->relPend = OUTRELANDPLAYANNC;
    else
      con->relPend = NORELANDPLAYANNC;

    CC_STATE_CHANGE(con, CCS_AWTSWTCFMFORDP);
    con->mgBlock->con = con;
    mgret = ccSendMgiQueue(0, con->mgBlock);
    if (mgret == ROK)
      RETVALUE(ROK);
    else
    {
      CCLOGERR("Error: MGI failed:icSu:%ld, ogSu:%ld, suCtxId:%ld,"
               " icTransId:%ld\n", con->icSuConnId, con->ogSuConnId,
               con->mgBlock->mgCtx->suCtxId, con->icBcm.svcHeader.transId);
      ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
      cleanUpMgQ(con);
      ccHandleCleanUpConCb(con, TRUE, AM_CV_TMPFAIL);
      RETVALUE(ROK);
    }
  }
  if (con->ogRscVal)
  {
    ccFreeTheRscFromCon(con, OUTTOINC);
  }

  initOgBcm(&con->ogBcm);
  ret = ccSendToResource(&(con->icBcm));
  if (ret != ROK)
  {
    CCLOGERR("Error: ccSendToResource failed:icSu:%ld,ogSu:%ld,icTrsId:%ld\n",
             con->icSuConnId, con->ogSuConnId, 
             (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0));
    ccHandleCleanUpConCb(con, TRUE, AM_CV_TMPFAIL);
  }
  RETVALUE(ROK);
}

/* 
*  
*      Fun:   ccConE69SBUF
*  
*      Desc:  Connection state function
*             event - CCE_FRAUDCALLTMREXP.
*             state - CCS_XXXX 
* 
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes:
*   
*      File:  cc_bdy2_2.c
*   
*/  
PUBLIC  S16 ccConE69SBUF
(   
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)   
{   
  CCDP(AIN_DBG_LEVEL_1, "ccConE69SBUF:icSu:%ld, ogSu:%ld\n", 
       con->icSuConnId, con->ogSuConnId);

  ccBufferEventToQueue(con, bcm, event, CCE_FRAUDCALLTMREXP, sizeof(U8), 0,
                       NULLP, 0);
  RETVALUE(ROK);
}   


/*
 *
 *      Fun:   ccConE72S10
 *
 *      Desc:  Connection state function
 *             event - Error message from FIC.
 *             state - ANSWERED
 *      Ret:   ROK     - successful,
 *             RFAILED - unsuccessful
 *
 *      Notes: None.
 *
 *      File:  cc_bdy2.c
 *
*/
PUBLIC  S16 ccConE72S10
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
#ifdef ZC
  ZcUpdParam upd;
#endif /* ZC */

  ccStopConTmr(con, CC_TMR_CALLDTL);

  DP("**%s: Disconnecting call (icSu:%lx-ogSu:%lx) on ERROR MSG from FIC\n",
      __FUNCTION__, con->icSuConnId, con->ogSuConnId);

#ifdef ZC
  /* REL CON */
  upd.icSuConnId = con->icSuConnId;
  zcRunTimeUpd(ZC_RELCON_CB, CMPFTHA_UPD_REQ, upd);
  zcUpdPeer();
#endif

  ccConE72S09(con, event, bcm, event2);

  RETVALUE(ROK);
} /* ccConE72S10 */


/*
 *
 *      Fun:   ccConE72S09
 *
 *      Desc:  Connection state function
 *             event - Error message from FIC.
 *             state - AWTANS
 *      Ret:   ROK     - successful,
 *             RFAILED - unsuccessful
 *
 *      Notes: None.
 *
 *      File:  cc_bdy2.c
 *
*/
PUBLIC  S16 ccConE72S09
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  S16 ret = RFAILED;
  Error_t   *pError = (Error_t *)event2;

 #if 1 /* EARLY_ACM */
  ccStopConTmr(con, CC_TMR_EARLYACM); 
 #endif

  /* Feature 1789 - German ISUP */
  /* stop the Toiw2 Timer if running */
  ccStopConTmr(con, CC_TMR_TOIW2);

  ccStopConTmr(con, TMR_SETUP);
  ccStartConTmr(TMR_RELEASE, con, (PTR)&ccCp.genCfg);

  DP("**%s: Disconnecting call (icSu:%lx-ogSu:%lx) on ERROR MSG from FIC\n",
      __FUNCTION__, con->icSuConnId, con->ogSuConnId);

  
  con->callDtlInfo.cm.forcedRelType = CALL_REL_ERROR_FROM_SCP;

  if(isThisBcmInMPartyCall(&con->icBcm) || isThisBcmInMPartyCall(&con->ogBcm))
  {
    ccHandleCleanUpConCb(con, FALSE, CCTMPFAIL);
  }
  else
  {
    con->failCndVal = pError->errorCode;

    CCDP(AIN_DBG_LEVEL_1, "%s:icSu:%ld, ogSu:%ld, errorCode=%ld\n",
        __FUNCTION__, con->icSuConnId, con->ogSuConnId, pError->errorCode);

    if (CC_IS_CONTEXT_PRESENT(con))
    {
      /* Blow off the switching and Release the Call */
      ccQSubBasedOnCcn(con);  /* FID 15261.0 */

      /* disconnect the outgoing leg but, continue processing
       * failure condition on incoming BCM */
      con->relPend = OUTRELANDINFAILCOND;

      CC_STATE_CHANGE(con, CCS_AWTSWTCFM_SENDONEREL);
      con->mgBlock->con = con;
      ret = ccSendMgiQueue(0, con->mgBlock);
      if (ret != ROK)
      {
        CCLOGERR( "**** ERROR: can't do ccSendMgiQueue *******\n");
        cleanUpMgQ(con);
        ccHandleCleanUpConCb(con, TRUE, AM_CV_TMPFAIL);
        RETVALUE(ROK);
      }
      else
        RETVALUE(ROK);
    }
    else
    {
      ccFreeTheRscFromCon(con, OUTTOINC); /* This will take care of REL_CFM */
      ccProcessFailCnd(con, con->failCndVal, TRUE);
      RETVALUE(ROK);
    }
  }

  RETVALUE(ROK);
} /* ccConE72S09 */


/*
*
*      Fun:   ccConE72SXX
*
*      Desc:  Connection state function
*             event - Error
*             state - 
*
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes:
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC  S16 ccConE72SXX
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  DP("ccConE72SXX : %ld\n", con->icSuConnId);

  ccBufferEventToQueue(con, bcm, event, CCE_FIC_ERROR, 0, 0,
                       event2, sizeof(Error_t));

  RETVALUE(ROK);
} /* ccConE72SXX */


/*
*
*      Fun:   ccConE72SXY
*
*      Desc:  Connection state function
*             event - Error
*             state - 
*
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes:
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC  S16 ccConE72SXY
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  Error_t   *pError = (Error_t *)event2;

  ccStopConTmr(con, TMR_SETUP);
  ccStartConTmr(TMR_RELEASE, con, (PTR)&ccCp.genCfg);

  DP("**%s: Disconnecting call (icSu:%lx-ogSu:%lx) on ERROR MSG from FIC\n",
      __FUNCTION__, con->icSuConnId, con->ogSuConnId);

  
  con->callDtlInfo.cm.forcedRelType = CALL_REL_ERROR_FROM_SCP;

  if(isThisBcmInMPartyCall(&con->icBcm) || isThisBcmInMPartyCall(&con->ogBcm))
  {
    ccHandleCleanUpConCb(con, FALSE, CCTMPFAIL);
  }
  else
  {
    CCDP(AIN_DBG_LEVEL_1, "%s:icSu:%ld, ogSu:%ld, errorCode=%ld\n",
        __FUNCTION__, con->icSuConnId, con->ogSuConnId, pError->errorCode);

    if(con->ogRscVal)
    {
      ccDeallocateResource(con, CC_OUTGOING);
      con->ogRscVal = FALSE;
    }
    con->relPend = 0;
    ccProcessFailCnd(con, pError->errorCode, TRUE);
  }

  RETVALUE(ROK);
} /* ccConE72SXY */

/* 
*  
*      Fun:   ccConE47S36
*
*      Desc:  Connection state function
*             event - CCE_SEND_TO_RESOURCE
*             state - CCS_AWTRSPFORDPE1
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2.c
*
*/
PUBLIC  S16 ccConE47S36
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
   Bool               noCloseMsgAndCloseTx  = FALSE;
   S16                ret                   = RFAILED;
   SvcHeader_t       *icSvcHeader           = NULLP;

   CCDP(AIN_DBG_LEVEL_1, "ccConE47S36: Rec SendToResource in AWTRSPFORDPE1\n");

   ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));

   icSvcHeader = (SvcHeader_t *) event;
   if (con->icBcm.cc != NULLP)
      if (con->icBcm.cc->ccId == CiThreePartySetup)
         noCloseMsgAndCloseTx   = TRUE;

   if (icSvcHeader->lastTransactionFlag == TRUE)
      ccDeleteBcm(&(con->icBcm));


   con->lastState = con->state;
   con->anncBlock.stateForAnnc = con->state;
   ret = ccSendToResource(&(con->icBcm));
   if (ret != ROK)
   {
     CCLOGERR("Error:ccSendToResource failed:icSu:%ld, ogSu:%ld, icTrsId:%ld\n",
              con->icSuConnId, con->ogSuConnId, 
              (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0));
     ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
   }
   RETVALUE(ROK);
} /* End of ccConE47S36 */


/*
 *
 *      Fun:   ccConE04S56
 *
 *      Desc:  Connection state function
 *             event -Release indication - 04
 *             state - CCS_AWTSWTCFM_SENDNOREL
 *
 *      Ret:   ROK     - successful,
 *             RFAILED - unsuccessful
 *
 *      Notes: None.
 *
 *      File:  cc_bdy2_2.c
 *
*/
PUBLIC  S16 ccConE04S56
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  S16 oldValue = 0;

  /* bug:24909 : to release the call by fore! */
  if((con->mntRelProc == CC_REL_FORCE_LOC) || (con->mntRelProc == CC_REL_FORCE_REM))

  {
    con->mntRelProc = 0;
    CCLOGERR(" *** Call is being terminated by force on req from Layer-3"
        ": icSu:%lx, ogSuc:%lx, relPend:%d, icPro:%d, ogPro:%d *****\n",
        con->icSuConnId, con->ogSuConnId, con->relPend, con->icProtType,
        con->ogProtType);
    /* plan: stop and start RELEASE timer to kick off release procedure
     * This should handle all cases of relPend in any state (55 or 56) */
    ccStopConTmr(con, TMR_RELEASE);
    oldValue = ccCp.genCfg.tmr.tRLS.val;
    ccCp.genCfg.tmr.tRLS.val = 1; /* force it to expire soon */
    ccStartConTmr(TMR_RELEASE, con, (PTR)&ccCp.genCfg);
    ccCp.genCfg.tmr.tRLS.val = oldValue; /* force it to oldValue */
  }
  /* end of bug:24909 */
  else if (con->lastState == CCS_AWTRSPFORDPE32)
  {
    ccBufferEventToQueue(con, bcm, event, CCE_RELIND, sizeof(CcRelEvnt), 0,
                         NULLP, 0);
    RETVALUE(ROK);
  }
  RETVALUE(ROK);
} /* ccConE04S56 */

/*
 *
 *      Fun:   ccConE01S18
 *
 *      Desc:  Connection state function
 *             event - ANswer indication - 04
 *             state - CCS_AWTRELCFM_OG or BOTH
 *
 *      Ret:   ROK     - successful,
 *             RFAILED - unsuccessful
 *
 *      Notes: None.
 *
 *      File:  cc_bdy2_2.c
 *
*/
PUBLIC  S16 ccConE01S18
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{

  /* FID 14732 */
  ccExtractSipwMBufFromConIndAndConCfm(con, CCE_CONCFM, (PTR) event);
  /* FID 14732 */

  if (con->direction == OUTTOINC)
  {
    ProtType ogProtType = CC_GET_PROTOCOL(con->ogProtType);

    switch (ogProtType)
    {
       case CC_MGCP_LN:
       case CC_MGCP_TG:
          ccExtractProtSdp(ccInit.region, ccInit.pool, con,
              &(((CcConEvnt *)event)->m.mgcpCctEvnt.sdp));
          break;

       case CC_BICC:
          ccExtractProtSdp(ccInit.region, ccInit.pool, con,
              &(((CcConEvnt *)event)->m.siConEvnt.optElmnt.bicc.sdp));
          break;

       case CC_SIPT:
          ccExtractProtSdp(ccInit.region, ccInit.pool, con,
                           &(((CcConEvnt *)event)->m.siConEvnt.optElmnt.sipt.sdp));

          break;

       case CC_SIP:
          ccExtractProtSdp(ccInit.region, ccInit.pool, con,
               &(((CcConEvnt *)event)->m.sipConEvnt.sdp));

#if 0 /* 35279 */
          if (((CcConEvnt *)event)->m.sipConEvnt.ogSipIdPres == PRSNT_NODEF)
          {
            /* Set the call ID */
            con->callDtlInfo.un.ogSipIdPres = PRSNT_NODEF;
            memcpy(con->callDtlInfo.un.ogSipId, ((CcConEvnt *)event)->m.sipConEvnt.ogSipId,
                   CC_SIP_ID_LENGTH);
          }
#endif /* 35279 */
          break;
    }
  }
  RETVALUE(ROK);
} /* ccConE01S18 */


/*
*
*      Fun:   ccConE37S09
*
*      Desc:  Connection state function
*             event - Analyze Route.
*             state - 09-CCS_AWTANS
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: This function is called, when FIC come back to GCC with a
*             Analyze Route, when it is awaiting Answer from far end.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE37S09
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  S16                 ret                 = RFAILED;

  if ((con == NULLP) || (event == NULLP))
  {
    CCLOGERR("Critical ERROR: Null pointers:%lx, %ld\n", (U32)con, event);
    RETVALUE(RFAILED);
  }

  CCDP(AIN_DBG_LEVEL_1, "ccConE37S09:icSu:%ld, ogSu:%ld, suCtxId:%ld\n",
       con->icSuConnId, con->ogSuConnId, 
       (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0));

  /* Bug 91414 When the call goes unanswered and rolls to VM, the new call
   * to the VM platform uses the TLDN in the tempAT of the first call leg
   * rather than the route digits returned from the TTNR
   */
  if(con->tempAuthTerm != NULLP)
  {
    /* Bug 80641, reset con->tempAuthTerm but keep homeCc */
    TknStr4 tmpHomeCc = con->tempAuthTerm->homeCc;

    CCDP(AIN_DBG_LEVEL_1, "ccConE37S09: Free tempAuthTerm\n");
    if (con->tempAuthTerm->tempBuf.pres == PRSNT_NODEF)
    {
       ccDropUBuf((Buffer **)&con->tempAuthTerm->tempBuf.val);
    }
    cmMemset ((U8 *)con->tempAuthTerm, 0, sizeof(CcTempAuthTerm));
    con->tempAuthTerm->homeCc = tmpHomeCc; /*Bug 80641 */
  }

  /* Bug 58536 */
  ccRestoreEvntHold(con);

  ret=ccProcessAR(con, event, bcm, event2);
  if (ret != ROK)
     RETVALUE(RFAILED);

  RETVALUE(ROK);
}

/*
*
*      Fun:   ccConE37SBUF
*
*      Desc:  Connection state function
*             event - Analyze Route.
*             state - Various states, waiting for Answer from far end.
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE37SBUF
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  DP("ccConE37SBUF:icSu:%ld, ogSu:%ld, suCtxId:%ld\n", con->icSuConnId,
      con->ogSuConnId, 
      (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0));
  ccBufferEventToQueue(con, bcm, event, CCE_ANALYZE_ROUTE, sizeof(SvcHeader_t),
                       con->icSuConnId, event2, sizeof(AnalyzeRoute_t));
  RETVALUE(ROK);
}

/*
 *
 *      Fun:   ccConE37S10
 *
 *      Desc:  Connection state function
 *             event - Analyze Route.
 *             state - 10 CCS_ANSWERED
 *      Ret:   ROK     - successful,
 *             RFAILED - unsuccessful
 *
 *      Notes: This function is called, when FIC come back to GCC with a
 *             Analyze Route, when the call is answered.
 *
 *      File:  cc_bdy2_2.c
 *
 */
PUBLIC S16 ccConE37S10
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  SvcHeader_t        *arSvcHeader           = NULLP;
  AnalyzeRoute_t     *pAnalyzeRoute         = NULLP;
  U8                  relDisBoolOG          = RFAILED;
  U8                  evntType              = 0;
  S16                 mgret                 = RFAILED;
#ifdef ZC
  ZcUpdParam          upd;
#endif 
  /* BUG 78431 */
  ProtType            icProtType = 0;
 /*CID 15592*/
  if ((con == NULLP) || (event == NULLP))
  {
    CCLOGERR("Critical ERROR: Null pointers:%lx, %ld\n", (U32)con, event);
    RETVALUE(RFAILED);
  }
  /* Feature 1789 - German ISUP */
  /* stop the Toiw2 Timer if running */
  ccStopConTmr(con, CC_TMR_TOIW2);


  /* BUG 78431 */
  icProtType = CC_GET_PROTOCOL(con->icProtType);

  CCDP(AIN_DBG_LEVEL_1, "ccConE37S10:icSu:%ld, ogSu:%ld, suCtxId:%ld\n",
       con->icSuConnId, con->ogSuConnId, 
       (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0));

  pAnalyzeRoute = (AnalyzeRoute_t *) event2;
  arSvcHeader = (SvcHeader_t *) event;

  CCDP(AIN_DBG_LEVEL_1, "ccConE37S10: scpResponse = %d\n", pAnalyzeRoute->scpResponse);
  ccCutBillingRecordIfNeeded(con, pAnalyzeRoute->scpResponse, OUTTOINC);
  ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));

  /* Bug 91414 When the call goes unanswered and rolls to VM, the new call
   * to the VM platform uses the TLDN in the tempAT of the first call leg
   * rather than the route digits returned from the TTNR
   */
  if(con->tempAuthTerm != NULLP)
  {
    /* Bug 80641, reset con->tempAuthTerm but keep homeCc */
    TknStr4 tmpHomeCc = con->tempAuthTerm->homeCc;

    CCDP(AIN_DBG_LEVEL_1, "ccConE37S10: Free tempAuthTerm\n");
    if (con->tempAuthTerm->tempBuf.pres == PRSNT_NODEF)
    {
       ccDropUBuf((Buffer **)&con->tempAuthTerm->tempBuf.val);
    }
    cmMemset ((U8 *)con->tempAuthTerm, 0, sizeof(CcTempAuthTerm));
    con->tempAuthTerm->homeCc = tmpHomeCc; /*Bug 80641 */
  }

  ccRestoreEvntHold(con);
  /* Regenerate the ccEvntHold only for the failover scenario */
  ccRegenerateEvntHold(con);
  ccExtractAnalyzeRteParam(pAnalyzeRoute, bcm);
  ccExtractSvcHeader(arSvcHeader, bcm, MSG_Analyze_Route);

  /*Bug 55191: Reset ogConReq and ogConCfm time stamps */
  if ((ccInit.acnt == TRUE)&&(con->keepGCCCallInfo == FALSE))
  {
    con->callDtlInfo.cm.tckOgConCfmRcvd.pres = NOTPRSNT;
    con->callDtlInfo.cm.tckOgConReqSent.pres = NOTPRSNT;

    /*Bug:55849: Reset the callState*/
    con->callDtlInfo.cm.callState = LCC_CDR_UNANSWERED;
  }

  /* FID 15461.0 */
  ccPegStndCalls(con, CC_DECREASE);
#ifdef ZC
  /* REL CON */
  upd.icSuConnId = con->icSuConnId;
  zcRunTimeUpd(ZC_RELCON_CB, CMPFTHA_UPD_REQ, upd);
  zcUpdPeer(); 

  if (zcCb.protState == ACTIVE)  
  {
      /* DEL CON */
      upd.icSuConnId = con->icSuConnId;
      zcRunTimeUpd(ZC_CON_CB, CMPFTHA_DELETE_REQ, upd);
      zcUpdPeer();
  }
#endif 

  /* BUG 78431 + */
  if ((!CC_IS_CONTEXT_PRESENT(con) || 
       (IS_MULTIMEDIA_NO_MEDIA_AWARE_4_BCM(&(con->icBcm)) && ccIsIPResource(&(con->ogBcm)))) /*bug91501*/
       && (con->isAnsIndSent) && ((icProtType == CC_SIP) || (icProtType == CC_SIPT)))
  {
     /* Ingress is complete with offer/answer, this will set renegSdpAfterAns and force
      * reneg of the ingress SDP 
      */
     CCDP(AIN_DBG_LEVEL_1, "SET_SDPSENTONICBCM \n"); 
     SET_SDPSENTONICBCM(con);
  }
  /* BUG 78431 - */

  /* Re-Start the Set-up timer */
  ccStopConTmr(con, TMR_SETUP);
  ccStartConTmr(TMR_SETUP, con, (PTR)&ccCp.genCfg);

  CCDP(AIN_DBG_LEVEL_1, "ccConE37S10: arSvcHeader->lastTransactionFlag = %d\n", arSvcHeader->lastTransactionFlag);
  if (arSvcHeader->lastTransactionFlag == TRUE)
    ccDeleteBcm(&(con->icBcm));

  con->ogBcm.closeCause = CscCallTerminated;
  if (CC_IS_CONTEXT_PRESENT(con))
  {
    CCDP(AIN_DBG_LEVEL_1, "ccConE37S10: Context present \n");
    ccQueueSubOnOGCleanUp(con);
    con->tempHolderForReleaseCause = CCCALLCLR;
    con->relPend = OUTRELANDROUTE;
    CC_STATE_CHANGE(con, CCS_AWTSWTCFMFORDP);
    ccChangeCallStateOfAssociatedCalls(&(con->icBcm), CCS_CANNOTPROCESSEVENTS);
    con->mgBlock->con = con;
    mgret = ccSendMgiQueue(0, con->mgBlock);
    if (mgret == ROK)
      RETVALUE(ROK);
    else
    {
      CCLOGERR("Error: MGI failed:icSu:%ld, ogSu:%ld, suCtxId:%ld,"
               " icTransId:%ld\n", con->icSuConnId, con->ogSuConnId,
               con->mgBlock->mgCtx->suCtxId, con->icBcm.svcHeader.transId);
      ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
      cleanUpMgQ(con);
      ccHandleCleanUpConCb(con, TRUE, AM_CV_TMPFAIL);
      RETVALUE(ROK);
    }
  }

  /* Clean outgoing and ReRoute call */
  if (con->ogRscVal)
  {
    relDisBoolOG = ccCheckRelOrOnHookInBuffer(&(con->ogBcm), &evntType);
    if (relDisBoolOG == ROK)
    {
      ccCloseTBcmTransaction(con);
      con->direction = OUTTOINC;
      if (con->noRspReqd == FALSE) ccSendReleaseRsp(con);
      ccDeallocateResource(con, CC_OUTGOING);
      con->ogRscVal = FALSE;
      initOgBcm(&con->ogBcm);
      ccRouteCall(con, FALSE);
    }
    else
    {
      ccFreeTheRscFromCon(con, OUTTOINC); /* This will take care of REL_CFM */
      initOgBcm(&con->ogBcm);
      if(ROK == ccCheckForInterfaces(con))
        ccRouteCall(con, FALSE);
      else
      {
        CC_TRACE_DUMP(con);
        ccProcessFailCnd(con, FC_INTERNAL, TRUE);
      }
    }
    RETVALUE(ROK);
  }
  else
  {
    /* Route the call */
    CCLOGERR("Error: Unexpected condition, still going ahead:%ld\n",
             con->icSuConnId);
    initOgBcm(&con->ogBcm);
    ccRouteCall(con, FALSE);
    RETVALUE(ROK);
  }
  RETVALUE(ROK);
}

/* FID 14457.0 + */
/*
*
*      Fun:   ccConE37SCQBUF
*
*      Desc:  Connection state function used for call queued
*             event - Analyze Route.
*             state - Various states, waiting for Answer from far end.
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE37SCQBUF
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  if (CC_IS_CALL_FLAG_SET(con->flag,CC_FLAG_CALLQUEUED))
  {
     DP("ccConE37SCQBUF:icSu:%ld, ogSu:%ld, suCtxId:%ld\n", con->icSuConnId,
         con->ogSuConnId, 
         (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0));
     ccBufferEventToQueue(con, bcm, event, CCE_ANALYZE_ROUTE, sizeof(SvcHeader_t),
                       con->icSuConnId, event2, sizeof(AnalyzeRoute_t));
  }
  else
  {
    ABORT_DEBUG;
    CCDBGP(DBGMASK_CC, (ccInit.prntBuf, " Received Unexpected Event \n"));
  }
  RETVALUE(ROK);
}
/* FID 14457.0 + */


/*
*
*      Fun:   ccConE14S18
*
*      Desc:  Connection state function
*             event - SETUP Timer Expired -14
*             state - CCS_AWTRELCFM_OG
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: This is a case of Call Forward No Answer scenario.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE14S18
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */ 
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  CCLOGERR("Error: ccConE14S18:icSu:%ld, ogSu:%ld, suCtxId:%ld\n",
           con->icSuConnId, con->ogSuConnId, 
           (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0));
  ccDeallocateResource(con, CC_OUTGOING);
  ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
  RETVALUE(ROK);
}


/*
*
*      Fun:   ccConE11S57
*
*      Desc:  Connection state function
*             event - Switching Connect Confirm
*             state - CCS_AWTMGCTCFMFORMODIFY0
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*          
*      File:  cc_bdy2_2.c
*        
*/
PUBLIC S16 ccConE11S57
(
CcConCb   *con,               /* connection */
PTR        event,             /* event */
AinBCM_t  *bcm,               /* bcm */
PTR        event2             /* event */
)
{
  MgctTxnCfm       *txnCfm         = NULLP;
  S16               ret            = RFAILED;
  SvcHeader_t       header;
  ResourceClear_t   resourceClear;
  AinBCM_t          *pBcm          = NULLP;

  if ((con == NULLP) || (event == NULLP))
  {
    CCLOGERR("Critical ERROR: Null pointers:%lx, %ld\n", (U32)con, event);
    RETVALUE(RFAILED);
  }
  CCDP(AIN_DBG_LEVEL_1, "ccConE11S57:icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", con->icSuConnId, con->ogSuConnId,
       con->mgBlock->mgCtx->suCtxId, con->icBcm.svcHeader.transId);

  ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));

  txnCfm   = (MgctTxnCfm*) event;
  ret = ccHandleMgResponse(con, txnCfm);
  if (ret != ROK)
  {
    CCLOGERR("ccHandleResponse Failed:%ld\n", con->icSuConnId);
    /*For Announcement + Digits, disconnectFlag is always set to FALSE*/
    if (con->anncBlock.anncType == CC_ANNC_DIGIT)
    {
      /*Always on icBcm*/
      pBcm = &(con->icBcm);

      /* Initializing the message buffers */
      cmMemset((U8 *)&header, 0, sizeof(SvcHeader_t));
      cmMemset((U8 *)&resourceClear, 0, sizeof(ResourceClear_t));

      /* Fill ClearCause with ClcUserAbandon */
      con->ainInfo.ClearCause = ClcResourceNotAvailable;

      /* Fill in parameters in the Service Header Structure */
      ccFillSvcHeader(&header, pBcm, MSG_Resource_Clear);
      CCDP(AIN_DBG_LEVEL_0," Filled SvcHeader\n");

      /* Fill in parameters in the Resource Clear structure */
      ccFillResourceClearParam(&resourceClear, pBcm);
      CCDP(AIN_DBG_LEVEL_0," Filled ResourceClear\n");

      CC_STATE_CHANGE(con, CCS_AWTRSPFORDPE3);

      /* Send the ResourceClear message to the FIC */
      CCDP(AIN_DBG_LEVEL_0,"Sending ResourceClear, TransID=%ld\n",
           header.spTransId);
      CcUiFctResourceClear(&resourceClear, &header);
    }
    else if ((CC_IS_CALL_FLAG_SET(con->flag,CC_FLAG_CALLQUEUED)) && 
             (con->anncBlock.anncType == CC_ANNCONLY))
    {
        CCDP(AIN_DBG_LEVEL_0, "%s: ccHandleResponse Failed:%ld, wait for AnalyzeRoute\n", __FUNCTION__,con->icSuConnId);
        CC_STATE_CHANGE(con, CCS_AWTRSPFORDPE3);
    }
    else
    {
      cleanUpMgQ(con);
      ccHandleCleanUpConCb(con, TRUE, AM_CV_TMPFAIL);
    }
    RETVALUE(ROK);
  }
  else
  {
    ccPlayInternalAnnoucement(con);
    RETVALUE(ROK);
  }
}


/*
 *
 *      Fun:   ccConE11S58
 *
 *      Desc:  Connection state function
 *             event - MGCT TXN CFM.
 *             state - CCS_AWTMGCTCFMFORMODIFY1 
 *      Ret:   ROK     - successful,
 *             RFAILED - unsuccessful
 *
 *      Notes: None.
 *
 *      File:  cc_bdy2.c
 *
 */
PUBLIC S16 ccConE11S58
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                   /* bcm */
PTR     event2                   /* event */
)
{
  MgctTxnCfm       *txnCfm         = NULLP;
  S16               ret            = RFAILED;
#ifdef ZC
  ZcUpdParam upd;
#endif /* ZC */
  SvcHeader_t       header;
  ResourceClear_t   resourceClear;
  AinBCM_t          *pBcm          = NULLP;

  if ((con == NULLP) || (event == NULLP))
  {
    CCLOGERR("Critical ERROR: Null pointers:%lx, %ld\n", (U32)con, event);
    RETVALUE(RFAILED);
  }
  CCDP(AIN_DBG_LEVEL_1, "ccConE11S58:icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", con->icSuConnId, con->ogSuConnId,
       con->mgBlock->mgCtx->suCtxId, con->icBcm.svcHeader.transId);

  ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));

  txnCfm   = (MgctTxnCfm*) event;
  ret = ccHandleMgResponse(con, txnCfm);
  if (ret != ROK)
  {
    CCLOGERR("ccHandleResponse Failed:%ld\n", con->icSuConnId);
    /*For Announcement + Digits, disconnectFlag is always set to FALSE*/
    if (con->anncBlock.anncType == CC_ANNC_DIGIT)
    {
      /*Always on icBcm*/
      pBcm = &(con->icBcm);

      /* Initializing the message buffers */
      cmMemset((U8 *)&header, 0, sizeof(SvcHeader_t));
      cmMemset((U8 *)&resourceClear, 0, sizeof(ResourceClear_t));

      /* Fill ClearCause with ClcUserAbandon */
      con->ainInfo.ClearCause = ClcResourceNotAvailable;

      /* Fill in parameters in the Service Header Structure */
      ccFillSvcHeader(&header, pBcm, MSG_Resource_Clear);
      CCDP(AIN_DBG_LEVEL_0," Filled SvcHeader\n");

      /* Fill in parameters in the Resource Clear structure */
      ccFillResourceClearParam(&resourceClear, pBcm);
      CCDP(AIN_DBG_LEVEL_0," Filled ResourceClear\n");

      CC_STATE_CHANGE(con, CCS_AWTRSPFORDPE3);

      /* Send the ResourceClear message to the FIC */
      CCDP(AIN_DBG_LEVEL_0,"Sending ResourceClear, TransID=%ld\n",
           header.spTransId);
      CcUiFctResourceClear(&resourceClear, &header);
    }
    else if ((CC_IS_CALL_FLAG_SET(con->flag,CC_FLAG_CALLQUEUED)) && 
             (con->anncBlock.anncType == CC_ANNCONLY))
    {
        CCDP(AIN_DBG_LEVEL_0, "%s: ccHandleResponse Failed:%ld, wait for AnalyzeRoute\n", __FUNCTION__,con->icSuConnId);
        CC_STATE_CHANGE(con, CCS_AWTRSPFORDPE3);
    }
    else
    {
      cleanUpMgQ(con);
      ccHandleCleanUpConCb(con, TRUE, AM_CV_TMPFAIL);
    }
    RETVALUE(ROK);
  }
  else
  {
    ccChangeCallStateOfAssociatedCalls(&(con->icBcm), CCS_CANNOTPROCESSEVENTS);

    if (CC_STR_RESOURCE_ID(con->anncBlock.annBlk) == ANNC_ID_MILLIWATT_INT_TONE)
    {
      /* MilliWatt Test Tone call */

      ccStopConTmr(con, TMR_SETUP);
      CC_STATE_CHANGE(con, CCS_LOCALLYANSWERED);
      ccProcessOAnswer(&(con->icBcm));
      ccUpdateBcm(con, PicOActive, CiStable2Party, BtOBcm);
#ifdef ZC
      cmMemset((U8*) &upd, 0, sizeof(ZcUpdParam));
      upd.con                  = con;
      upd.con->ogSapCb         = NULLP;
      upd.con->ogIntfcCb       = NULLP;
      zcRunTimeUpd(ZC_CON_CB, CMPFTHA_CREATE_REQ, upd);
      zcUpdPeer();
#endif /* ZC */
    }
    else
    {
      CC_STATE_CHANGE(con, CCS_AWTMGCTNOTIFY0);
    }

    RETVALUE(ROK);
  }
} /* ccConE11S58 */

/* euysal - 06/01/2005 - ACC Support */
#if 1

/*
 *
 *      Fun:   ccConE11S97
 *
 *      Desc:  Connection state function
 *             event - MGCT TXN CFM.
 *             state - CCS_AWTMGCTCFMFORMODIFY1_CTR 
 *      Ret:   ROK     - successful,
 *             RFAILED - unsuccessful
 *
 *      Notes: None.
 *
 *      File:  cc_bdy2.c
 *
 */
PUBLIC S16 ccConE11S97
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                   /* bcm */
PTR     event2                   /* event */
)
{
  MgctTxnCfm       *txnCfm         = NULLP;
  S16               ret            = RFAILED;

  if ((con == NULLP) || (event == NULLP))
  {
    CCLOGERR("Critical ERROR: Null pointers:%lx, %ld\n", (U32)con, event);
    RETVALUE(RFAILED);
  }
  CCDP(AIN_DBG_LEVEL_1, "ccConE11S97:icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", con->icSuConnId, con->ogSuConnId,
       con->mgBlock->mgCtx->suCtxId, con->icBcm.svcHeader.transId);

  ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));

  txnCfm   = (MgctTxnCfm*) event;
  ret = ccHandleMgResponse(con, txnCfm);
  if (ret != ROK)
  {
    CCLOGERR("ccHandleResponse Failed:%ld\n", con->icSuConnId);
    cleanUpMgQ(con);
    /* euysal - 02/14/2006 - Send back CTRClear with error ClcFailure instead of releasing the call */
    if (con->lastState == CCS_ANSWERED)
    {
       con->ainInfo.ClearCause = ClcFailure; 
       ccProcessCTRClear(&(con->icBcm));
    }
    else
    ccHandleCleanUpConCb(con, TRUE, AM_CV_TMPFAIL);
       
    con->lastState = CCS_IDLE;
    RETVALUE(ROK);
  }
  else
  {
    ccChangeCallStateOfAssociatedCalls(&(con->icBcm), CCS_CANNOTPROCESSEVENTS);
    con->lastState = CCS_IDLE;
    CC_STATE_CHANGE(con, CCS_AWTANNCMPLT_CTR);
    RETVALUE(ROK);
  }
} /* ccConE11S97 */

#endif


/*
*
*      Fun:   ccConE31S57
*
*      Desc:  Connection state function
*             event - SFT event indication
*             state - CCS_AWTMGCTCFMFORMODIFY0
*
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE31S57
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
#if 0 /* CC_MGC */
#ifdef ZC
  ZcUpdParam upd;
#endif /* ZC */

  CCDP(AIN_DBG_LEVEL_1, "ccConE31S57:%ld,%ld,%ld\n", con->icSuConnId,
                        con->ogSuConnId, con->mgBlock->mgCtx->suCtxId);
#ifdef ZC
  if (CC_IS_OBCM_PIC_ACTIVE(con))
  {
    cmMemset((U8 *)&upd, 0, sizeof(ZcUpdParam));
    upd.featMsg.suConnId     = con->icSuConnId; /*ACT Controlling leg connId*/
    upd.featMsg.ccAinUpdType = CC_KILL_CALL;
    zcRunTimeUpd(ZC_FEAT, CMPFTHA_UPD_REQ, upd);
    zcUpdPeer();
  }
#endif
  ccKillCallOnThisCon(con);
#endif /* CC_MGC */
  RETVALUE(ROK);
} /* ccConE31S57 */

/*
*
*      Fun:   ccConE31S58
*
*      Desc:  Connection state function
*             event - MGCT event indication
*             state - CCS_AWTMGCTCFMFORMODIFY1
*
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE31S58
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
#if 0 /* CC_MGC */
#ifdef ZC
  ZcUpdParam upd;
#endif /* ZC */

  CCDP(AIN_DBG_LEVEL_1, "ccConE31S58:%ld,%ld,%ld\n", con->icSuConnId,
                        con->ogSuConnId, con->mgBlock->mgCtx->suCtxId);
#ifdef ZC
  if (CC_IS_OBCM_PIC_ACTIVE(con))
  {
    cmMemset((U8 *)&upd, 0, sizeof(ZcUpdParam));
    upd.featMsg.suConnId     = con->icSuConnId; /*ACT Controlling leg connId*/
    upd.featMsg.ccAinUpdType = CC_KILL_CALL;
    zcRunTimeUpd(ZC_FEAT, CMPFTHA_UPD_REQ, upd);
    zcUpdPeer();
  }
#endif
  ccKillCallOnThisCon(con);
#endif /* CC_MGC */
  RETVALUE(ROK);
} /* ccConE31S58 */

/* euysal - 06/01/2005 - ACC Support */
#if 1
/*
*
*      Fun:   ccConE31S97
*
*      Desc:  Connection state function
*             event - MGCT event indication
*             state - CCS_AWTMGCTCFMFORMODIFY1_CTR
*
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE31S97
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
#if 0 /* CC_MGC */
#ifdef ZC
  ZcUpdParam upd;
#endif /* ZC */

  CCDP(AIN_DBG_LEVEL_1, "ccConE31S97:%ld,%ld,%ld\n", con->icSuConnId,
                        con->ogSuConnId, con->mgBlock->mgCtx->suCtxId);
#ifdef ZC
  if (CC_IS_OBCM_PIC_ACTIVE(con))
  {
    cmMemset((U8 *)&upd, 0, sizeof(ZcUpdParam));
    upd.featMsg.suConnId     = con->icSuConnId; /*ACT Controlling leg connId*/
    upd.featMsg.ccAinUpdType = CC_KILL_CALL;
    zcRunTimeUpd(ZC_FEAT, CMPFTHA_UPD_REQ, upd);
    zcUpdPeer();
  }
#endif
  ccKillCallOnThisCon(con);
#endif /* CC_MGC */
  RETVALUE(ROK);
} /* ccConE31S97 */
#endif


/*
*
*      Fun:   ccConE03S91
*
*      Desc:  Connection state function
*             event - CCE_CNST_IND 
*             state - CCS_AWTPAUSETIMEREXPIRY
*
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE03S91
(
CcConCb   *con,             /* connection */
PTR        event,           /* event */
AinBCM_t  *bcm,             /* bcm */
PTR        event2           /* event */
)
{
  S16                      ret           = RFAILED;
  S16                      sdpRet        = RFAILED;
  ProtType                 icProtType    = CC_GET_PROTOCOL(con->icProtType);
  ProtType                 ogProtType    = CC_GET_PROTOCOL(con->ogProtType);

  CCDP(AIN_DBG_LEVEL_1, "ccConE03S91 :0x%lx\n", con->icSuConnId);

  if (ccIsForcedOnHookRcvd(con->icProtType, con->ogProtType, con->icEvntType,
          con->direction))
  {
    CCDP(AIN_DBG_LEVEL_0, "IOM failover has occured:%ld\n", con->icSuConnId);
    cleanUpMgQ(con);
    ccCleanUpMGCtx(con);
    ccUpdateCallDtlInfoRelOrignForOnHook(con->icProtType,con->ogProtType,
                                         con, con->direction, con->icEvntType);
    ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
    RETVALUE(ROK);
  }
  else if(con->direction == OUTTOINC)
  {
    CCLOGERR("Error: Invalid direction icSu = 0x%lx\n", con->icSuConnId);
    RETVALUE(ROK);
  }
  else if (ccIsOnHookRcvd(con->icProtType, con->ogProtType, con->icEvntType,
          con->direction))
  {
    ret = ccProcessDisconnectEvent(con, event);
    if (con->surrogate != NULLP)
       ccProcessDisconnectNotfnForSurrogateLegs(con, event);
    if (ret == ROK)
    {
      CCDP(AIN_DBG_LEVEL_0,"ccProcessDisconnectEvent success,event detec.\n");
      RETVALUE(ROK);
    }
    else if (ret == RFAILED)
    {
      CCDP(AIN_DBG_LEVEL_3,"Failed to Check if (O/T)Disc event is armed\n");
    }
    CCDP(AIN_DBG_LEVEL_0, "O/T_Disconnect event is not Armed\n");

    ccStopConTmr(con, TMR_SETUP);
    ccStopConTmr(con, CC_PAUSETIMER);
    ccStartConTmr(TMR_RELEASE, con, (PTR)&ccCp.genCfg);
    if (icProtType != CC_LOCAL_SWT_PROT)
    {
      ret = ccProcessDisconnectNotfnForCaleaLegs(con, event);
      if (ret != ROK)
      {
        CCDP(AIN_DBG_LEVEL_4,
           "Critical Error!!ccProcessDisconnectNotfnForCaleaLegs FAILED\n");
      }
    }
    if (CC_IS_CONTEXT_PRESENT(con))
    {
      ccQSubBasedOnCcn(con);  /* FID 15261.0 */

      con->relPend = ICRELONLY;
      CC_STATE_CHANGE(con, CCS_AWTSWTCFM_SENDONEREL);
      con->mgBlock->con = con;
      ret = ccSendMgiQueue(0, con->mgBlock);
      if (ret == ROK)
        RETVALUE(ROK);
      else
      {
        CCLOGERR("Error: During switching icSu = 0x%lx\n", con->icSuConnId);
        cleanUpMgQ(con);     
        ccCleanUpMGCtx (con);
      }
    }
    CC_STATE_CHANGE(con, CCS_AWTRELCFM_IC);
    ccSendRelease(con, con->icSapCb, con->icSpConnId, con->icSuConnId,
                  con->icProtType, CCCALLCLR, NULL);
    RETVALUE(ROK);
  }
  else if (ccIsFlashRcvd(con->icProtType, con->ogProtType, con->icEvntType,
          con->direction))
  {
    if (con->icBcm.cc)
    {
      if (con->icBcm.cc->ccId < CiThreePartySetup)
      {
        CCDP(AIN_DBG_LEVEL_0, "Ignoring the hook Flash, it may be armed\n");
        RETVALUE(ROK);
      }
    }
    con->lastState = con->state;
    ret = ccProcessMidCallEvent(con);
    if (ret == ROK)
    {
      CCDP(AIN_DBG_LEVEL_0,"ccProcessMidCallEvent success, event detected\n");
      RETVALUE(ROK);
    }
    else if (ret == RFAILED)
    {
      CCLOGERR("MidCall Requested Event to FIC failed\n");
    }
    CCDP(AIN_DBG_LEVEL_0, "Hook Flash event is not armed\n");
    RETVALUE(ROK);
  }
#if 1  /* 51115 */
  else if((con->direction == INCTOOUT) &&
          ((icProtType == CC_SIP) || (icProtType == CC_BICC) || (icProtType == CC_SIPT)))
  {

      /* FID 14732 */ 
      ccExtractSipwMBufFromCnStInd(con, event);
      /* FID 14732 */

      /* Extract the SDP if rcvd from layer-3 */
      /* euysal - 05/31/2006 - BUG:46983 Need to check the retvalue */
      sdpRet = ccExtractSdpFromCnStInd(con, event);
      if (sdpRet != ROK)
      {
          CCLOGERR("Error: SDP parsing failed - %d\n", sdpRet);
          con->failCndVal = FC_PROTERR;

          if (CC_IS_CONTEXT_PRESENT(con))
          {
              ccQSubBasedOnCcn(con);  /* FID 15261.0 */

              ret = ccSendMgiQueue(0, con->mgBlock);
              if (ret != ROK)
              {
                  CCLOGERR( "**** ERROR: can't do ccSendMgiQueue *******\n");
                  cleanUpMgQ(con);
                  ccCleanUpMGCtx(con);
              }
              else
              {
                if(ccIsPrackRcvd(icProtType, ogProtType, con->icEvntType,
                                 con->direction))
                {
                  CC_STATE_CHANGE(con, CCS_AWTSWTCFM_SENDNOREL);
                  con->relPend = ONLYINFAILCND;
                }
                else
                {
                  CC_STATE_CHANGE(con, CCS_AWTSWTCFM_SENDONEREL);
                  con->relPend = OUTRELANDINFAILCOND;
                }
                con->mgBlock->con = con;
                RETVALUE(ROK);
              }
          }

          if(!ccIsPrackRcvd(icProtType, ogProtType, con->icEvntType,
                            con->direction))
          {
            ccFreeTheRscFromCon(con, OUTTOINC);
          }
          ccProcessFailCnd(con, con->failCndVal, TRUE);
          RETVALUE(ROK);
      }

      /* Handle  HOLD/QUERY/XFER and respective RSP for SIP/BICC calls */
      /* Note: MGCP will not send HOLD/QUERY/XFER to GCC */
      /* Bug 88230: Handle PRACK SDP Offers */

      /* Dual Call Hold -- XFER will be handling seperately */
      if((con->direction == INCTOOUT) &&
         (ccIsXferRcvd(CC_GET_PROTOCOL(con->icProtType),
          CC_GET_PROTOCOL(con->ogProtType), con->icEvntType, con->direction)))
      {
          ret = ccHandleXfer(con, icProtType, ogProtType,
                             con->direction, event);
      }
      else if ((ccIsAckRcvd(icProtType, ogProtType, con->icEvntType, con->direction)) ||
               (ccIsPrackRcvd(icProtType, ogProtType, con->icEvntType, con->direction))) /* BUG:93368 */
      {
          ret = ccProcessPrackOrAck(con, icProtType, ogProtType,
                                    (CcAllSdus*)event);

          if(ret == ROKIGNORE)
          {
            ccStopConTmr(con, CC_PAUSETIMER);
          }
      }
      else if ((con->direction == INCTOOUT) &&
               (ccIsUpdateRcvd(CC_GET_PROTOCOL(con->icProtType),
                CC_GET_PROTOCOL(con->ogProtType), con->icEvntType, con->direction))) /* BUG:93368 */
      {
         ret = ccProcessUpdate(con, CC_GET_PROTOCOL(con->icProtType),
                                               CC_GET_PROTOCOL(con->ogProtType), (CcAllSdus*)event);
         /* If this case, if MGI action is needed, will stop the pause
          * timer here, the best is to have ccProcessUpdate ret to be
          * ROKPENDING,but too many regressin! */
         if ((ret == ROKIGNORE) && (con->state == CCS_AWT_MGI_XFERQRYHOLD))
         {
            ccStopConTmr(con, CC_PAUSETIMER);
         }
      }
      else
      {
          ret = ccHandleQryXferHold(con, icProtType, ogProtType,
                                    con->direction, event);
      }

      if(ret == ROKPENDING) /* Save the event for later */
      {
	
          ccStopConTmr(con, CC_PAUSETIMER);

          /* FID 15261.0 + */
          if ((!CC_IS_CALL_MULTI_MG(con)) ||
              (CC_IS_CALL_MULTI_MG(con) && CC_OPERATING_FIRST_MG(con)))
          {
            /* we will interwork the event after MGI's response */
            ccStoreEvnt(con->evntType, con->icEvntType, con, (U8 *)event,
                        sizeof(CcCnStEvnt));
          }
          /* FID 15261.0 - */
          RETVALUE(ROK);
      }
      RETVALUE(ROK);
  }
#endif  /* 51115 */
  else
  {
    CCLOGERR("ccConE03S91(): unExpected evnt in state = %d, ProtocolType = %d,"
             " direction = %d, evnType =%d\n", con->state, con->icProtType,
             con->direction, con->icEvntType);
    RETVALUE(ROK);
  }
} /* ccConE03S91 */


/*
*      Fun:   ccConE04S91
*
*      Desc:  Connection state function
*             event - CCE_RELIND 
*             state - CCS_AWTPAUSETIMEREXPIRY
*
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE04S91
(
CcConCb   *con,             /* connection */
PTR        event,           /* event */
AinBCM_t  *bcm,             /* bcm */
PTR        event2           /* event */
)
{
  S16                      ret          = RFAILED;
  ProtType                 icProtType   = CC_GET_PROTOCOL(con->icProtType);

  CCDP(AIN_DBG_LEVEL_1, "ccConE04S91 :0x%lx\n", con->icSuConnId);

  if (con->direction != INCTOOUT)
  {
    CCLOGERR("Error: Invalid direction icSu = 0x%lx\n", con->icSuConnId);
    RETVALUE(ROK);
  }

  ret = ccProcessDisconnectEvent(con, event);
  if (con->surrogate != NULLP)
     ccProcessDisconnectNotfnForSurrogateLegs(con, event);

  if (ret == ROK)
  {
    CCDP(AIN_DBG_LEVEL_0,"ccProcessDisconnectEvent success,event detec.\n");
    RETVALUE(ROK);
  }
  else if (ret == RFAILED)
  {
    CCDP(AIN_DBG_LEVEL_3,"Failed to Check if (O/T)Disc event is armed\n");
  }
  CCDP(AIN_DBG_LEVEL_0, "O/T_Disconnect event is not Armed\n");

  ccStopConTmr(con, TMR_SETUP);
  ccStopConTmr(con, CC_PAUSETIMER);
  ccStartConTmr(TMR_RELEASE, con, (PTR)&ccCp.genCfg);
  ccUpdateCloseCause(con, CC_BOTH, CscCallerAbandon);

  /* CALL release before the Tapped subject goes to ANSWERED state */
  if (icProtType != CC_LOCAL_SWT_PROT)
  {
    ret = ccProcessDisconnectNotfnForCaleaLegs(con, event);
    if (ret != ROK)
    {
      CCDP(AIN_DBG_LEVEL_4,
           "Critical Error!!ccProcessDisconnectNotfnForCaleaLegs FAILED\n");
    }
  }

  /* FID 14732 */                      
  ccExtractSipwMBufFromRel(con, CCE_RELIND, (PTR) event);
  /* FID 14732 */

  if ((ret = ccStoreEvnt(con->evntType, con->icEvntType, con, (U8 *)event,
                         sizeof(CcRelEvnt))) != ROK)
  {
    CCLOGERR( "ERROR: ccStoreEvnt()failed. Continuing with call\n");
  }

  if (CC_IS_CONTEXT_PRESENT(con))
  {
    ccQSubBasedOnCcn(con);  /* FID 15261.0 */

    con->relPend = INRELRSPANDNOREL;
    CC_STATE_CHANGE(con, CCS_AWTSWTCFM_SENDNOREL);
    con->mgBlock->con = con;
    ret = ccSendMgiQueue(0, con->mgBlock);
    if (ret == ROK)
      RETVALUE(ROK);
    else
    {
      CCLOGERR("Error: During switching icSu = 0x%lx\n", con->icSuConnId);
      cleanUpMgQ(con);     
      ccCleanUpMGCtx (con);
    }
  }

  if (con->noRspReqd == FALSE) ccSendReleaseRsp(con);
  CC_STATE_CHANGE(con, CCS_AWTDEALOCCFM);
  ccDeallocateResource(con, CC_INCOMING);
  RETVALUE(ROK);
}


/*
*      Fun:   ccConE73S91
*
*      Desc:  Connection state function
*             event - CCE_PAUSETMREXPIRY
*             state - CCS_AWTPAUSETIMEREXPIRY
*
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE73S91
(
CcConCb   *con,             /* connection */
PTR        event,           /* event */
AinBCM_t  *bcm,             /* bcm */
PTR        event2           /* event */
)
{
  CCDP(AIN_DBG_LEVEL_1, "ccConE73S91 icSu:0x%lx\n", con->icSuConnId);
  ccBuildAndSendAnncReq(con);
  RETVALUE(ROK);
}


/*
*      Fun:   ccConE03S89
*
*      Desc:  Connection state function
*             event - CCE_CNSTREQ
*             state - CCS_AWTMGCTNOTIFY0
*
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE03S89
(
CcConCb   *con,             /* connection */
PTR        event,           /* event */
AinBCM_t  *bcm,             /* bcm */
PTR        event2           /* event */
)
{
  S16                      ret          = RFAILED;
  S16                      sdpRet          = RFAILED;
  ProtType                 icProtType   = CC_GET_PROTOCOL(con->icProtType);
  ProtType                 ogProtType   = CC_GET_PROTOCOL(con->ogProtType);
  SvcHeader_t              header;
  ResourceClear_t          resourceClear;
  AinBCM_t                *pBcm          = NULLP;

  CCDP(AIN_DBG_LEVEL_1, "ccConE03S89 icSu:0x%lx\n", con->icSuConnId);
  ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
  if (con->direction != INCTOOUT)
  {
    CCLOGERR("Error: Invalid Direction %d, icSu =0x%lx\n",
              con->direction, con->icSuConnId);
    RETVALUE(ROK);
  }
  else if (ccIsForcedOnHookRcvd(con->icProtType, con->ogProtType,
                                con->icEvntType, con->direction))
  {
    CCDP(AIN_DBG_LEVEL_0, "IOM failover has occured:%ld\n", con->icSuConnId);
    cleanUpMgQ(con);     
    ccCleanUpMGCtx (con);
    ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
    RETVALUE(ROK);
  }
  else if (ccIsOnHookRcvd(con->icProtType, con->ogProtType, con->icEvntType,
          con->direction))
  {
    if (!con->ainInfo.disconnectLegFlag)
    {
      if (con->anncBlock.stateForAnnc != CCS_AWTRSPFORDPE20)
        pBcm = &(con->icBcm);
      else
        pBcm = &(con->ogBcm);

      /* Initializing the message buffers */
      cmMemset((U8 *)&header, 0, sizeof(SvcHeader_t));
      cmMemset((U8 *)&resourceClear, 0, sizeof(ResourceClear_t));

      /* Fill ClearCause with ClcUserAbandon */
      con->ainInfo.ClearCause = ClcUserAbandon;

      /* Fill in parameters in the Service Header Structure */
      ccFillSvcHeader(&header, pBcm, MSG_Resource_Clear);
      CCDP(AIN_DBG_LEVEL_0," Filled SvcHeader\n");

      /* Fill in parameters in the Resource Clear structure */
      ccFillResourceClearParam(&resourceClear, pBcm);
      CCDP(AIN_DBG_LEVEL_0," Filled ResourceClear\n");

      /* Send the ResourceClear message to the FIC */
      CCDP(AIN_DBG_LEVEL_0,"Sending ResourceClear, TransID=%ld\n",
           header.spTransId);
      CcUiFctResourceClear(&resourceClear, &header);
    }

    ret = ccProcessDisconnectEvent(con, event);
    if (con->surrogate != NULLP)
       ccProcessDisconnectNotfnForSurrogateLegs(con, event);
    if (ret == ROK)
    {
      CCDP(AIN_DBG_LEVEL_0,"ccProcessDisconnectEvent success,event detec.\n");
      RETVALUE(ROK);
    }
    else if (ret == RFAILED)
    {
      CCDP(AIN_DBG_LEVEL_3,"Failed to Check if (O/T)Disc event is armed\n");
    }
    /* If the ccProcessDisconnectEvent returns ROKDNA, we continue */
    CCDP(AIN_DBG_LEVEL_0, "O/T_Disconnect event is not Armed\n");

    ccStopConTmr(con, TMR_SETUP);
    ccStartConTmr(TMR_RELEASE, con, (PTR)&ccCp.genCfg);

    /* CALL release before the Tapped subject goes to ANSWERED state */
    if (icProtType != CC_LOCAL_SWT_PROT)
    {
      /* Make sure ur in Tapped Con */
      ret = ccProcessDisconnectNotfnForCaleaLegs(con, event);
      if (ret != ROK)
      {
        CCDP(AIN_DBG_LEVEL_4,
           "Critical Error!!ccProcessDisconnectNotfnForCaleaLegs FAILED\n");
      }
    }

    if (CC_IS_CONTEXT_PRESENT(con))
    {
      ccQSubBasedOnCcn(con);  /* FID 15261.0 */

      con->relPend = ICRELONLY;
      CC_STATE_CHANGE(con, CCS_AWTSWTCFM_SENDONEREL);
      con->mgBlock->con = con;
      ret = ccSendMgiQueue(0, con->mgBlock);
      if (ret == ROK)
        RETVALUE(ROK);
      else
      {
        CCLOGERR("Error: During switching icSu = 0x%lx\n", con->icSuConnId);
        cleanUpMgQ(con);     
        ccCleanUpMGCtx (con);
      }
    }
    if (ret != ROK)
    {
      CC_STATE_CHANGE(con, CCS_AWTRELCFM_IC);
      ccSendRelease(con, con->icSapCb, con->icSpConnId, con->icSuConnId,
                    con->icProtType, CCCALLCLR, NULL);
      RETVALUE(ROK);
    }
    else
      RETVALUE(ROK);
  }
  else if (ccIsFlashRcvd(con->icProtType, con->ogProtType, con->icEvntType,
          con->direction))
  {
    if (con->icBcm.cc)
    {
      if (con->icBcm.cc->ccId < CiThreePartySetup)
      {
        CCDP(AIN_DBG_LEVEL_0, "Ignoring the hook Flash, it may be armed\n");
        RETVALUE(ROK);
      }
    }
    con->lastState = con->state;
    ret = ccProcessMidCallEvent(con);
    if (ret == ROK)
    {
      CCDP(AIN_DBG_LEVEL_0,"ccProcessMidCallEvent success, event detected\n");
      RETVALUE(ROK);
    }
    else if (ret == RFAILED)
    {
      CCLOGERR("MidCall Requested Event to FIC failed\n"); 
    }
    CCDP(AIN_DBG_LEVEL_0, "Hook Flash event is not armed\n");
    RETVALUE(ROK);
  }
#if 1  /* 51115 */
  else if((con->direction == INCTOOUT) &&
          ((icProtType == CC_SIP) || (icProtType == CC_BICC) || (icProtType == CC_SIPT)))
  {

      /* FID 14732 */ 
      ccExtractSipwMBufFromCnStInd(con, event);
      /* FID 14732 */

      /* Extract the SDP if rcvd from layer-3 */
      /* euysal - 05/31/2006 - BUG:46983 Need to check the retvalue */
      sdpRet = ccExtractSdpFromCnStInd(con, event);
      if (sdpRet != ROK)
      {
          CCLOGERR("Error: SDP parsing failed - %d\n", sdpRet);
          con->failCndVal = FC_PROTERR;

          if (CC_IS_CONTEXT_PRESENT(con))
          {
              ccQSubBasedOnCcn(con);  /* FID 15261.0 */

              ret = ccSendMgiQueue(0, con->mgBlock);
              if (ret != ROK)
              {
                  CCLOGERR( "**** ERROR: can't do ccSendMgiQueue *******\n");
                  cleanUpMgQ(con);
                  ccCleanUpMGCtx(con);
              }
              else
              {
                if(ccIsPrackRcvd(icProtType, ogProtType, con->icEvntType,
                                 con->direction))
                {
                  CC_STATE_CHANGE(con, CCS_AWTSWTCFM_SENDNOREL);
                  con->relPend = ONLYINFAILCND;
                }
                else
                {
                  CC_STATE_CHANGE(con, CCS_AWTSWTCFM_SENDONEREL);
                  con->relPend = OUTRELANDINFAILCOND;
                }
                con->mgBlock->con = con;
                RETVALUE(ROK);
              }
          }

          if(!ccIsPrackRcvd(icProtType, ogProtType, con->icEvntType,
                            con->direction))
          {
            ccFreeTheRscFromCon(con, OUTTOINC);
          }	
          ccProcessFailCnd(con, con->failCndVal, TRUE);
          RETVALUE(ROK);
      }

      /* Handle  HOLD/QUERY/XFER and respective RSP for SIP/BICC calls */
      /* Note: MGCP will not send HOLD/QUERY/XFER to GCC */

      /* Dual Call Hold -- XFER will be handling seperately */
      if((con->direction == INCTOOUT) &&
         (ccIsXferRcvd(CC_GET_PROTOCOL(con->icProtType),
          CC_GET_PROTOCOL(con->ogProtType), con->icEvntType, con->direction)))
      {
          ret = ccHandleXfer(con, icProtType, ogProtType,
                             con->direction, event);
      }
      else if(ccIsPrackRcvd(icProtType, ogProtType, con->icEvntType,
                            con->direction))
      {
          ret = ccProcessPrackOrAck(con, icProtType, ogProtType,
                                    (CcAllSdus*)event);
      }
      else if ((con->direction == INCTOOUT) &&
               ccIsUpdateRcvd(CC_GET_PROTOCOL(con->icProtType),
                              CC_GET_PROTOCOL(con->ogProtType), con->icEvntType, con->direction))
      {
         /* BUG:93368 Process UPDATE here ! */
          ret = ccProcessUpdate(con, CC_GET_PROTOCOL(con->icProtType),
                                CC_GET_PROTOCOL(con->ogProtType), (CcAllSdus*)event);
      }
      else
      {
          ret = ccHandleQryXferHold(con, icProtType, ogProtType,
                                    con->direction, event);
      }

      if(ret == ROKPENDING) /* Save the event for later */
      {
          /* FID 15261.0 + */
          if ((!CC_IS_CALL_MULTI_MG(con)) ||
              (CC_IS_CALL_MULTI_MG(con) && CC_OPERATING_FIRST_MG(con)))
          {
            /* we will interwork the event after MGI's response */
            ccStoreEvnt(con->evntType, con->icEvntType, con, (U8 *)event,
                        sizeof(CcCnStEvnt));
          }
          /* FID 15261.0 - */
          RETVALUE(ROK);
      }
      RETVALUE(ROK);
  }
#endif  /* 51115 */
  else
  {
    CCLOGERR("ccConE03S89(): unExpected evnt in state = %d, ProtocolType = %d,"
             " direction = %d, evnType =%d\n", con->state, con->icProtType,
             con->direction, con->icEvntType);
    RETVALUE(ROK);
  }
}


/*
*      Fun:   ccConE04S89
*
*      Desc:  Connection state function
*             event - CCE_RELREQ
*             state - CCS_AWTMGCTNOTIFY0
*
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE04S89
(
CcConCb   *con,             /* connection */
PTR        event,           /* event */
AinBCM_t  *bcm,             /* bcm */
PTR        event2           /* event */
)
{
  S16                      ret          = RFAILED;
  ProtType                 icProtType   = CC_GET_PROTOCOL(con->icProtType);
  SvcHeader_t              header;
  ResourceClear_t          resourceClear;
  AinBCM_t                *pBcm          = NULLP;

  CCDP(AIN_DBG_LEVEL_1, "ccConE04S89 :0x%lx\n", con->icSuConnId);
  ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));

  if (con->direction != INCTOOUT)
  {
    CCLOGERR("Error: Invalid direction icSu = 0x%lx\n", con->icSuConnId);
    RETVALUE(ROK);
  }


  /* FID 14457.0 Update Close Cause + */
  if (CC_IS_CALL_FLAG_SET(con->flag,CC_FLAG_CALLQUEUED))
  {
    ccUpdateCloseCause(con, CC_BOTH, CscCallerAbandon);
  }
  /* FID 14457.0 - */

  if ((!con->ainInfo.disconnectLegFlag) && 
      (!CC_IS_CALL_FLAG_SET(con->flag,CC_FLAG_CALLQUEUED))) /* FID 14457.0 GETS */
  {
    if (con->anncBlock.stateForAnnc != CCS_AWTRSPFORDPE20)
      pBcm = &(con->icBcm);
    else
      pBcm = &(con->ogBcm);

    /* Initializing the message buffers */
    cmMemset((U8 *)&header, 0, sizeof(SvcHeader_t));
    cmMemset((U8 *)&resourceClear, 0, sizeof(ResourceClear_t));

    /* Fill ClearCause with ClcUserAbandon */
    con->ainInfo.ClearCause = ClcUserAbandon;

    /* Fill in parameters in the Service Header Structure */
    ccFillSvcHeader(&header, pBcm, MSG_Resource_Clear);
    CCDP(AIN_DBG_LEVEL_0," Filled SvcHeader\n");

    /* Fill in parameters in the Resource Clear structure */
    ccFillResourceClearParam(&resourceClear, pBcm);
    CCDP(AIN_DBG_LEVEL_0," Filled ResourceClear\n");

    /* Send the ResourceClear message to the FIC */
    CCDP(AIN_DBG_LEVEL_0,"Sending ResourceClear, TransID=%ld\n",
         header.spTransId);
    CcUiFctResourceClear(&resourceClear, &header);
  }


  ret = ccProcessDisconnectEvent(con, event);
  if (con->surrogate != NULLP)
     ccProcessDisconnectNotfnForSurrogateLegs(con, event);

  if (ret == ROK)
  {
    CCDP(AIN_DBG_LEVEL_0,"ccProcessDisconnectEvent success,event detec.\n");
    RETVALUE(ROK);
  }
  else if (ret == RFAILED)
  {
    CCDP(AIN_DBG_LEVEL_3,"Failed to Check if (O/T)Disc event is armed\n");
  }
  CCDP(AIN_DBG_LEVEL_0, "O/T_Disconnect event is not Armed\n");

  ccStopConTmr(con, CC_TMR_CALLDTL);
  ccStartConTmr(TMR_RELEASE, con, (PTR)&ccCp.genCfg);

  /* CALL release before the Tapped subject goes to ANSWERED state */
  if (icProtType != CC_LOCAL_SWT_PROT)
  {
    ret = ccProcessDisconnectNotfnForCaleaLegs(con, event);
    if (ret != ROK)
    {
      CCDP(AIN_DBG_LEVEL_4,
           "Critical Error!!ccProcessDisconnectNotfnForCaleaLegs FAILED\n");
    }
  }

  /* FID 14732 */                      
  ccExtractSipwMBufFromRel(con, CCE_RELIND, (PTR) event);
  /* FID 14732 */

  if ((ret = ccStoreEvnt(con->evntType, con->icEvntType, con, (U8 *)event,
                         sizeof(CcRelEvnt))) != ROK)
  {
    CCLOGERR( "ERROR: ccStoreEvnt()failed. Continuing with call\n");
  }
  if (CC_IS_CONTEXT_PRESENT(con))
  {
    ccQSubBasedOnCcn(con);  /* FID 15261.0 */

    con->relPend = INRELRSPANDNOREL;
    CC_STATE_CHANGE(con, CCS_AWTSWTCFM_SENDNOREL);
    con->mgBlock->con = con;
    ret = ccSendMgiQueue(0, con->mgBlock);
    if (ret == ROK)
      RETVALUE(ROK);
    else
    {
      CCLOGERR("Error: During switching icSu = 0x%lx\n", con->icSuConnId);
      cleanUpMgQ(con);     
      ccCleanUpMGCtx (con);
    }
  }

  if (con->noRspReqd == FALSE) ccSendReleaseRsp(con);
  CC_STATE_CHANGE(con, CCS_AWTDEALOCCFM);
  ccDeallocateResource(con, CC_INCOMING);
  RETVALUE(ROK);
}


/*
*      Fun:   ccConE14S89
*
*      Desc:  Connection state function
*             event - Setup timer expiry
*             state - CCS_AWTMGCTNOTIFY0
*
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE14S89
(
CcConCb   *con,             /* connection */
PTR        event,           /* event */
AinBCM_t  *bcm,             /* bcm */
PTR        event2           /* event */
)
{
  CCLOGERR("ccConE14S89 : Setup timer expired in state = %d, icSu = 0x%lx\n",
           con->state, con->icSuConnId);
  ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
  ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
  RETVALUE(ROK);
}

/* euysal - 06/01/2005 - ACC Support */
#if 1

/*
*      Fun:   ccConE14S82
*
*      Desc:  Connection state function
*             event - Setup timer expiry
*             state - CCS_AWTANNCMPLT_CTR
*
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE14S82
(
CcConCb   *con,             /* connection */
PTR        event,           /* event */
AinBCM_t  *bcm,             /* bcm */
PTR        event2           /* event */
)
{
  CCLOGERR("ccConE14S82 : Setup timer expired in state = %d, icSu = 0x%lx\n",
           con->state, con->icSuConnId);
  ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
  ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
  RETVALUE(ROK);
}
#endif


/*
*      Fun:   ccConE62S89
*
*      Desc:  Connection state function
*             event - CCE_NOTIFYFROMMGCT
*             state - CCS_AWTMGCTNOTIFY0
*
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE62S89
(
CcConCb   *con,             /* connection */
PTR        event,           /* event */
AinBCM_t  *bcm,             /* bcm */
PTR        event2           /* event */
)
{
  S16                     ret           = RFAILED;
  MgctEventsDescriptor    evntDesc;
  MgctSignalsDescriptor   signalDesc;
  /* FID 14841.0 */
  U16                megacoErr     = 0;
  MgctNotifyKind_e   kind          = MGCT_NOTIFY_KIND_UNKNOWN;


  CCDP(AIN_DBG_LEVEL_1, "ccConE62S89 icSu:0x%lx\n", con->icSuConnId);
  ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));

  ret = ccHandleMgctNotify(con, (MgctTxnInd *) event);
  if (ret != ROK)
  {
    CCLOGERR("Error: ccHandleMgctNotify failed icSu = 0x%lx,"
             "suCtxId 0x%lx, contextId = 0x%lx\n", con->icSuConnId,
             (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0), con->mgBlock->mgCtx->contextId);
    ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
    RETVALUE(ROK);
  }

  /* FID 14841.0 */
  mgctGetNotifyReqKind((MgctTxnInd *) event, &megacoErr, &kind, NULLP);
  HNDL_MGCT_NOT_FOR_ADID(kind, con, event, bcm);

  if (CC_IS_CONTEXT_PRESENT(con))
  {
    cmMemset((U8 *)&evntDesc, 0, sizeof(MgctEventsDescriptor));
    cmMemset((U8 *)&signalDesc, 0, sizeof(MgctSignalsDescriptor));

    evntDesc.pres = PRSNT_NODEF;
    signalDesc.pres = PRSNT_NODEF;

    ccUpdateMgQForAddOrModifyRsc(MGCT_MODE_SENDRECV, MGCT_PACKAGE_ID_MAX,
                                 MGCT_ITEM_ID_CG_MAX,
                                 MGCT_CMD_MODIFY, &(con->icBcm),
                                 &evntDesc, NULLP, &signalDesc, NULLP, NULLP,
                                 con->mgBlock->mgCtx->suCtxId,
                                 con->mgBlock->mgCtx->contextId, con);
    CC_STATE_CHANGE(con, CCS_AWTMGCTCFMFORMODIFY2);
    con->mgBlock->con = con;
    ccChangeCallStateOfAssociatedCalls(&(con->icBcm), CCS_CANNOTPROCESSEVENTS);
    ret = ccSendMgiQueue(con->mgBlock->mgCtx->suCtxId, con->mgBlock);
    if (ret == ROK)
    {
      RETVALUE(ROK);
    }
    else
    {
      CCLOGERR("Error: During switching icSu = 0x%lx\n", con->icSuConnId);
      ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
    }
  }
  else
  {
    CCLOGERR("Error: No contextId for icSu = 0x%lx\n", con->icSuConnId);
  }
  cleanUpMgQ(con);
  ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
  RETVALUE(ROK);
}


/* euysal - 06/01/2005 - ACC Support */
#if 1

/*
*      Fun:   ccConE62S82
*
*      Desc:  Connection state function
*             event - CCE_NOTIFYFROMMGCT
*             state - CCS_AWTANNCMPLT_CTR
*
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE62S82
(
CcConCb   *con,             /* connection */
PTR        event,           /* event */
AinBCM_t  *bcm,             /* bcm */
PTR        event2           /* event */
)
{
#if 0  
  S16                  ret1;
  CmMglTermId_t        icTermId;
  CmMglTermId_t        ogTermId;
#endif
  S16                     ret           = RFAILED;
  MgctEventsDescriptor    evntDesc;
  MgctSignalsDescriptor   signalDesc;
  /* FID 14841.0 */
  U16                megacoErr     = 0;
  MgctNotifyKind_e   kind          = MGCT_NOTIFY_KIND_UNKNOWN;
  /* FID 16692.0 */
  AinBCM_t     *pTmpOgBcm = NULLP;   
  AinBCM_t     *pTmpIcBcm = NULLP;   
  ProtType     icProtType = CC_GET_PROTOCOL(con->icProtType);   
  ProtType     ogProtType = CC_GET_PROTOCOL(con->ogProtType);

  CCDP(AIN_DBG_LEVEL_1, "ccConE62S82 icSu:0x%lx\n", con->icSuConnId);
  ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));

  ret = ccHandleMgctNotify(con, (MgctTxnInd *) event);
  if (ret != ROK)
  {
    CCLOGERR("Error: ccHandleMgctNotify failed icSu = 0x%lx,"
             "suCtxId 0x%lx, contextId = 0x%lx\n", con->icSuConnId,
             (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0), con->mgBlock->mgCtx->contextId);
    ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
    RETVALUE(ROK);
  }

  /* FID 14841.0 */
  mgctGetNotifyReqKind((MgctTxnInd *) event, &megacoErr, &kind, NULLP);
  HNDL_MGCT_NOT_FOR_ADID(kind, con, event, bcm);

  if (con->relPend == PLAYINGANNCFORCONNECTTORSC)
  {
     if (bcm->bcmType == BtOBcm)
     {
        CCDP(AIN_DBG_LEVEL_0, "End of Announcement Notification received for the incoming leg\n");
        con->anncCntr &= ~(ANNC_IC); 
        CCDP(AIN_DBG_LEVEL_0, "End of Announcement Notification, anncCntr %d\n", con->anncCntr);
     }
     else if (bcm->bcmType == BtTBcm)
     {
        CCDP(AIN_DBG_LEVEL_0, "End of Announcement Notification received for the outgoing leg\n");
        con->anncCntr &= ~(ANNC_OG); 
     }
     else  
     {
         CCLOGERR("Error: ccHandleMgctNotify failed. Invalid bcmType(%d) icSu = 0x%lx,"
                 "suCtxId 0x%lx, contextId = 0x%lx\n", bcm->bcmType, con->icSuConnId,
                 (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0), con->mgBlock->mgCtx->contextId);
        ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
        RETVALUE(RFAILED);
     }        
           
#if 0
     ret1 = MGL_GREATER;
     cmMemset((U8 *)&icTermId, 0, sizeof(CmMglTermId_t));
     ccFindtermId(&(con->icBcm), &icTermId);
     cmMglTermCmp(ccCp.mgVarCtx, &(((MgctTxnInd *) event)->u.notifyReq.termId.termId), &icTermId, &ret1);
     if (ret1 == MGL_EQUAL)
     {
        CCDP(AIN_DBG_LEVEL_0, "End of Announcement Notification received for the incoming leg\n");
        con->anncCntr &= ~(ANNC_IC); 
     }
     else
     {
        cmMemset((U8 *)&ogTermId, 0, sizeof(CmMglTermId_t));
        ccFindtermId(&(con->ogBcm), &ogTermId);
        cmMglTermCmp(ccCp.mgVarCtx, &(((MgctTxnInd *) event)->u.notifyReq.termId.termId), &ogTermId, &ret1);
        if (ret1 == MGL_EQUAL)
        {
           CCDP(AIN_DBG_LEVEL_0, "End of Announcement Notification received for the outgoing leg\n");
           con->anncCntr &= ~(ANNC_OG); 
        }
     }
     if (ret1 != MGL_EQUAL)
     {   
        if (ccIsTermIdWildCard (&(((MgctTxnInd *) event)->u.notifyReq.termId)))
        {
           CCDP(AIN_DBG_LEVEL_0, "End of Announcement Notification received for both legs\n");
           con->anncCntr &= ~(ANNC_BOTH);  
        }        
     }  
#endif
     
     if ((con->anncCntr != ANNC_NONE) && (!(con->anncCntr & ANNC_IC)) &&
         (!(con->anncCntr & ANNC_OG)) && (!(con->anncCntr & ANNC_BOTH)))
     {
        CCLOGERR("Error: ccHandleMgctNotify failed. Invalid anncCntr(%d) icSu = 0x%lx,"
                 "suCtxId 0x%lx, contextId = 0x%lx\n", con->anncCntr, con->icSuConnId,
                 (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0), 
                 (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->contextId : 0));
        ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
        RETVALUE(RFAILED);
     }
     /* There is still another announcement being played. Wait until
        all the announcements finish */
     if (con->anncCntr != ANNC_NONE)
        RETVALUE(ROK);
     else
     {
        if (ccInit.acnt == TRUE)
        {
           con->callDtlInfo.cm.tckAnncCmpTime.pres = PRSNT_NODEF;
           (Void) SGetSysTime(&con->callDtlInfo.cm.tckAnncCmpTime.val);
           CCDP(AIN_DBG_LEVEL_0, "Populating tckAnncCmpTime:%ld \n",
                                  con->callDtlInfo.cm.tckAnncCmpTime.val);
        }
     }   
  } 

  if (CC_IS_CONTEXT_PRESENT(con))
  {
     cmMemset((U8 *)&evntDesc, 0, sizeof(MgctEventsDescriptor));
     cmMemset((U8 *)&signalDesc, 0, sizeof(MgctSignalsDescriptor));

     evntDesc.pres = PRSNT_NODEF;
     signalDesc.pres = PRSNT_NODEF;

     /* FID 16692.0 */
     if (CCT_IS_FCIM_SET_ACC(con->fciMSet) && CC_IS_CALL_MULTI_MG(con) &&
        ((icProtType == CC_SIP || icProtType == CC_SIPT) && 
         (ogProtType == CC_SIP || ogProtType == CC_SIPT)))
     {
        con->ccmCb.state = CC_EGRESS_MG;
        CCDP(AIN_DBG_LEVEL_0, "ACC inter-mg, change state to CC_EGRESS_MG for IP originating call\n");
     }

     CC_DERIVE_BCMS(con, pTmpIcBcm, pTmpOgBcm);

     if (((con->ainInfo.Resourcetype2.pres == NOTPRSNT) &&
          ((con->ainInfo.legId == 0) || (con->ainInfo.legId == 0xff))) || 
         (con->ainInfo.Resourcetype2.pres == PRSNT_NODEF))
        ccUpdateMgQForAddOrModifyRsc(MGCT_MODE_SENDRECV, MGCT_PACKAGE_ID_MAX,
                                     MGCT_ITEM_ID_CG_MAX,
                                     MGCT_CMD_MODIFY, pTmpIcBcm,
                                     &evntDesc, NULLP, &signalDesc, NULLP, NULLP,
                                     (pTmpIcBcm->mgCtx ? pTmpIcBcm->mgCtx->suCtxId : 0),
                                     (pTmpIcBcm->mgCtx ? pTmpIcBcm->mgCtx->contextId : 0), con);
     if (((con->ainInfo.Resourcetype2.pres == NOTPRSNT) &&
          ((con->ainInfo.legId == 1) || (con->ainInfo.legId == 0xff))) || 
         (con->ainInfo.Resourcetype2.pres == PRSNT_NODEF))
        ccUpdateMgQForAddOrModifyRsc(MGCT_MODE_SENDRECV, MGCT_PACKAGE_ID_MAX,
                                     MGCT_ITEM_ID_CG_MAX,
                                     MGCT_CMD_MODIFY, pTmpOgBcm,
                                     &evntDesc, NULLP, &signalDesc, NULLP, NULLP,
                                     (pTmpOgBcm->mgCtx ? pTmpOgBcm->mgCtx->suCtxId : 0),
                                     (pTmpOgBcm->mgCtx ? pTmpOgBcm->mgCtx->contextId : 0), con);
    
    CC_STATE_CHANGE(con, CCS_AWTMGCTCFMFORMODIFY2_CTR);
    con->mgBlock->con = con;
    ccChangeCallStateOfAssociatedCalls(&(con->icBcm), CCS_CANNOTPROCESSEVENTS);
    /* FID 16692.0 */
    ret = ccSendMgiQueue(pTmpIcBcm->mgCtx->suCtxId, con->mgBlock);
    
    if (ret == ROK)
    {
      RETVALUE(ROK);
    }
    else
    {
      CCLOGERR("Error: During switching icSu = 0x%lx\n", con->icSuConnId);
      ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
    }
  }
  else
  {
    CCLOGERR("Error: No contextId for icSu = 0x%lx\n", con->icSuConnId);
  }
  cleanUpMgQ(con);
  ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
  RETVALUE(ROK);
}
#endif

/* 
* 
*      Fun:   ccConE03SMGIBUF
*  
*      Desc:  Connection state function
*             event - CCE_CNST_IND
*             state - MGCT waiting state.
* 
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*   
*      File:  cc_bdy2_2.c
*   
*/
PUBLIC S16 ccConE03SMGIBUF
(
CcConCb   *con,             /* connection */
PTR        event,           /* event */
AinBCM_t  *bcm,             /* bcm */
PTR        event2           /* event */
)
{
  DP("ccConE03SMGIBUF: 0x%lx, 0x%lx\n", con->icSuConnId, con->ogSuConnId);

  if (ccIsForcedOnHookRcvd(con->icProtType, con->ogProtType, con->icEvntType,
          con->direction))
  {
    CCDP(AIN_DBG_LEVEL_0, "IOM failover has occured: %lx\n", con->icSuConnId);
    cleanUpMgQ(con);
    ccCleanUpMGCtx (con);
    ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
    RETVALUE(ROK);
  }

  if (ccIsFlashRcvd(con->icProtType, con->ogProtType, con->icEvntType,
          con->direction))
  {
    if (con->icBcm.cc)
    {
      if (con->icBcm.cc->ccId < CiThreePartySetup)
      {
        CCDP(AIN_DBG_LEVEL_0, "Ignoring the hook Flash, it may be armed\n");
        RETVALUE(ROK);
      }
    }
    else
    {
      CCDP(AIN_DBG_LEVEL_0, "Ignoring the hook Flash, it may be armed\n");
      RETVALUE(ROK);
    }
    ccBufferEventToQueue(con, bcm, event, CCE_CNSTIND, sizeof(CcCnStEvnt), 0,
                         NULLP, 0);
    RETVALUE(ROK);
  }
  else
  {
     ccBufferEventToQueue(con, bcm, event, CCE_CNSTIND, sizeof(CcCnStEvnt), 0,NULLP, 0);
    RETVALUE(ROK);
  }
  RETVALUE(ROK);
}

/*
*
*      Fun:   ccConE03SBUF
*
*      Desc:  Connection state function
*             event - CCE_CNSTIND
*             state - Various
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes:
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE03SBUF
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                   /* bcm */
PTR     event2                   /* event */
)
{
    ccBufferEventToQueue(con, bcm, event, CCE_CNSTIND, sizeof(CcCnStEvnt), 0, NULLP, 0);
    RETVALUE(ROK);
} /* End of ccConE03SBUF */


/*
*
*      Fun:   ccConE03S2X
*
*      Desc:  Connection state function
*             event - CCE_CNSTIND
*             state - S24/S26 
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes:
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE03S2X
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                   /* bcm */
PTR     event2                   /* event */
)
{
    CCDP(AIN_DBG_LEVEL_1, "ccConE03S2X: icSu = %ld, ogSu = %ld,"
         " T.transId = %ld\n", con->icSuConnId, con->ogSuConnId, con->ogBcm.svcHeader.transId);
    ccBufferEventToQueue(con, bcm, event, CCE_CNSTIND, sizeof(CcCnStEvnt), 0, NULLP, 0);

    if((CC_GET_PROTOCOL(con->icProtType) == CC_SI)&& 
       (con->direction == INCTOOUT)&&
       (con->icEvntType == CC_ET_INFO))   
    {
        /* mark the buffer bit for INF event */
        CC_MARK_INFBUFFERED(con);
    }

    RETVALUE(ROK);
} /* End of ccConE03S2X */



/*
*      Fun:   ccConE14S90
*
*      Desc:  Connection state function
*             event - Setup timer expiry
*             state - CCS_AWTMGCTCFMFORMODIFY2
*
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE14S90
(
CcConCb   *con,             /* connection */
PTR        event,           /* event */
AinBCM_t  *bcm,             /* bcm */
PTR        event2           /* event */
)
{
  CCLOGERR("ccConE14S90 : Setup timer expired in state = %d, icSu = 0x%lx\n",
           con->state, con->icSuConnId);
  ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
  ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
  RETVALUE(ROK);
}


/* euysal - 06/01/2005 - ACC Support */
#if 1

/*
*      Fun:   ccConE14S98
*
*      Desc:  Connection state function
*             event - Setup timer expiry
*             state - CCS_AWTMGCTCFMFORMODIFY2_CTR
*
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE14S98
(
CcConCb   *con,             /* connection */
PTR        event,           /* event */
AinBCM_t  *bcm,             /* bcm */
PTR        event2           /* event */
)
{
  CCLOGERR("ccConE14S98 : Setup timer expired in state = %d, icSu = 0x%lx\n",
           con->state, con->icSuConnId);
  ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
  ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
  RETVALUE(ROK);
}


/*
*      Fun:   ccConE11S98
*
*      Desc:  Connection state function
*             event - MGCT Txn Cfm.
*             state - CCS_AWTMGCTCFMFORMODIFY2_CTR
*
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE11S98
(
CcConCb   *con,             /* connection */
PTR        event,           /* event */
AinBCM_t  *bcm,             /* bcm */
PTR        event2           /* event */
)
{
  MgctTxnCfm       *txnCfm        = NULLP;
  S16              ret            = RFAILED;
  SvcHeader_t      header;
  CTRClear_t       ctrClear;
  BOOL             relDisBool     = FALSE;
  U8               evntType;
  U8               tmpDirection   = DIR_UNKNOWN;
  CcCause          cause;
  AinBCM_t         *pBcm          = NULLP;
  PTR              event1;
  TknU16           sipCause, sipStatus;
#ifdef ZC
  ZcUpdParam upd;  
#endif /* ZC */

  if ((con == NULLP) || (event == NULLP))
  {
    CCLOGERR("Critical ERROR: Null pointers:%lx, %ld\n", (U32)con, event);
    RETVALUE(RFAILED);
  }
  CCDP(AIN_DBG_LEVEL_1, "ccConE11S98:icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", con->icSuConnId, con->ogSuConnId,
       con->mgBlock->mgCtx->suCtxId, con->icBcm.svcHeader.transId);

  ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));

  txnCfm   = (MgctTxnCfm*) event;
  ret = ccHandleMgResponse(con, txnCfm);
  if (ret != ROK)
  {
    CCLOGERR("ccHandleResponse Failed:%ld\n", con->icSuConnId);
    cleanUpMgQ(con);
    ccHandleCleanUpConCb(con, TRUE, AM_CV_TMPFAIL);
    RETVALUE(ROK);
  }
  ccChangeCallStateOfAssociatedCalls(&(con->icBcm), CCS_CANNOTPROCESSEVENTS);
  /* euysal - 03/29/2005 - ACC Support */
  if (con->relPend == PLAYINGANNCFORCONNECTTORSC)
  {   
      /* Announcement played, it's a response to remove rsc, we
      should send CTR_Clear to FIC now */
     switch(con->anncBlock.anncType)
     {
        case CC_ANNCONLY:
           if (!CC_DISC_FLAG(con))
           {
             /* FID14715.0 */
             if(con->ainInfo.warnToneAC)
             {
               con->ainInfo.ClearCause = ClcSuccess; 
               ccProcessCTRClear(&(con->icBcm));
               RETVALUE(ROK);
             }

              CCLOGERR("Error: Disconnect Flag not set by CTR-PlayAnnoucement  0x%lx\n",
                       con->icSuConnId);
              RETVALUE(RFAILED);
           }
           else
           {
              if (CC_IS_PROT_TYPE_BRAZIL(con->icProtType) ||
                  CC_IS_PROT_TYPE_BRAZIL(con->ogProtType))
              {
                 ccHandleCleanUpConCb(con, TRUE, CCCALLCLR);
              }
              else
              {
                 ccHandleCleanUpConCb(con, TRUE, CCNORMUNSPEC);
              }
//              ccSendReleasesOnBothSidesWithSwitching(con, CCNORMUNSPEC);
              RETVALUE(ROK);
           }
        break;
     default:
        break;
     }   
        
     con->ainInfo.ClearCause = ClcSuccess; 
     ccProcessCTRClear(&(con->icBcm));
     RETVALUE(ROK);
  } 
  if ((con->relPend == ABANDONEDANNC_IC) ||
      (con->relPend == ABANDONEDANNC_OG))
  {   
     if (con->direction == INCTOOUT)
        relDisBool = ccCheckRelOrOnHookInBuffer(&(con->icBcm), &evntType);
     else
        relDisBool = ccCheckRelOrOnHookInBuffer(&(con->ogBcm), &evntType);

     CC_STATE_CHANGE(con, CCS_ANSWERED);
     ccProcessCcChangeforX2SIG(con); /* FID 15083.0 */
     /* Fill ClearCause with ClcUserAbandon */
     con->ainInfo.ClearCause = ClcUserAbandon;

      pBcm = &(con->icBcm);
      
      /* euysal - 09/14/2005 - BUG:40662 fixed a wrong pointer assignment */
#if 0     
     if (pBcm->svcHeader.transId != 0)
#else
     if ((pBcm) && (pBcm->svcHeader.transId != 0))
#endif        
     {
        /* Need to send an Notification Event to FIC ( CTR_Clear) */

        /* Initializing the message buffers */
        cmMemset((U8 *)&header, 0, sizeof(SvcHeader_t));
        cmMemset((U8 *)&ctrClear, 0, sizeof(CTRClear_t));

        /* Fill in parameters in the Service Header Structure */
        ccFillSvcHeader(&header, pBcm, MSG_CTRClear);

        /* Fill in parameters in the CTR_CLEAR Structure */
        ccFillCTRClearParam(&ctrClear, pBcm);
        cmMemset((U8 *)&ctrClear.clAddrInfo, 0, sizeof(FcClAddrInfo)); 
        CCDP(AIN_DBG_LEVEL_0," Filled CTR_Clear, TransID=%ld\n", header.spTransId);

        /* Send the OTermSeized message to the FIC */
        CcUiFctCTRClear(&ctrClear, &header);
     }
     else
     {
        CCLOGERR("Not able to notify ctrClear,still going ahead: %ld, %ld\n",
                 con->icSuConnId, pBcm->svcHeader.transId);
     }
      
     /* we need to put the state back to answered again 
        since it is changed in ccProcessCTRClear */
     CC_STATE_CHANGE(con, CCS_ANSWERED);
     ccProcessCcChangeforX2SIG(con); /* FID 15083.0 */
    
     /*need to restore rel event here */
     ccRestoreReleaseInEvntHold(con);
       
     /* if we have a buffered release event, that means 
        we received a second release event from the opposite 
        leg and hence the direction has changed, we need to restore the direction */
     if (relDisBool)
     {  
        tmpDirection = con->direction;
        if (con->relPend == ABANDONEDANNC_IC)
           con->direction = INCTOOUT;
        else if (con->relPend == ABANDONEDANNC_OG)   
           con->direction = OUTTOINC;
     }

     /* Get protocol release cause */
     ccGetRelCause(con, (con->direction == OUTTOINC?con->ogProtType:con->icProtType),
                   (CcRelEvnt *)con->ccEvntHold, &cause, &sipCause, &sipStatus);
     if ((cause.eh.pres == PRSNT_NODEF) && (cause.causeVal.pres == PRSNT_NODEF))
     {
        con->tempHolderForReleaseCause = cause.causeVal.val;
        con->ainInfo.diconnectCause    = cause.causeVal.val;
     }
     
     event1 = (PTR)con->ccEvntHold;
     
     ret = ccProcessDisconnectEvent(con, event1);
       
     if (ret == ROK)
     {
        CCDP(AIN_DBG_LEVEL_1, "Sent DisconnectEvent to FIC -- icSuConnId:%ld, ogSuConnId:%ld\n", con->icSuConnId,
             con->ogSuConnId);
        /* Now we can put the direction back to its previous value */
        if (relDisBool)
           con->direction = tmpDirection;
        RETVALUE(ROK);
     }
     else if (ret == RFAILED)
     {
        CCDP(AIN_DBG_LEVEL_1, "Failed to Send DisconnectEvent to FIC -- icSuConnId:%ld, ogSuConnId:%ld\n", 
             con->icSuConnId, con->ogSuConnId);
     }

#ifdef ZC
     /* REL CON */
     upd.icSuConnId = con->icSuConnId;
     zcRunTimeUpd(ZC_RELCON_CB, CMPFTHA_UPD_REQ, upd);
     zcUpdPeer();
#endif
     /* BUG 93121 
      * ACC service call, if receive a release indication during announcement from a
      * terminal, no need to send relreq to it again */
     if (relDisBool && !CCT_IS_FCIM_SET_ACC(con->fciMSet))
     {   
        /* Now we can put the direction back to its previous value */
        con->direction = tmpDirection;

        ccSendReleasesOnBothSidesWithSwitching(con, CCNORMUNSPEC);
        RETVALUE(ROK);
     }   

     if (CC_IS_CONTEXT_PRESENT(con))
     {
        /* Blow off the switching and Release the Call */
        ccQSubBasedOnCcn(con);  /* FID 15261.0 */

        if (con->direction == INCTOOUT)
           con->relPend = INRELRSPANDOUTREL;
        else
           con->relPend = OUTRELRSPANDINREL;

        CC_STATE_CHANGE(con, CCS_AWTSWTCFM_SENDONEREL);
        con->mgBlock->con = con;
        ret = ccSendMgiQueue(0, con->mgBlock);
        if (ret != ROK)
        {
           CCLOGERR( "ERROR: No Switching Ctx found\n");
           cleanUpMgQ(con);
           ccCleanUpMGCtx(con);
        }
        else
           RETVALUE(ROK);
     }
     else
     {
        if ((con->icProtType == CC_LOCAL_SWT_PROT) && (con->icBcm.cc != NULLP) &&
            (con->icBcm.cc->tappedLegId != NULLP))
        {
           CCDP(AIN_DBG_LEVEL_0,"Deswitching of CALEA  in Tapped con\n"); 
           ccProcessTappedLegForCaleaRel(con, con->icBcm.cc->tappedLegId, 
                                         event, CCE_RELIND, sizeof(CcRelEvnt));
           RETVALUE(ROK); 
        }
        else
           CCDP(AIN_DBG_LEVEL_0, "No Switching Ctx found \n");
     }
 
     /* De-Switching failed, go ahead and clear the call anyway */
     /* Send Resource Dealocation for initiating side */
     /* Send Release response and map the release    */

     if (con->noRspReqd == FALSE) ccSendReleaseRsp(con);

     if (con->direction == INCTOOUT)
     {
        /* Deallocate the IC Resources */
        CC_STATE_CHANGE(con, CCS_AWTRELCFM_OG);
#ifdef GUARD_TIMER
        ccGuardOrDeallocResource(con, CC_INCOMING);
#else
        ccDeallocateResource(con, CC_INCOMING);
#endif
        con->icRscVal = FALSE;
     }
     else if (con->direction == OUTTOINC)
     {
        CC_STATE_CHANGE(con, CCS_AWTRELCFM_IC);
#ifdef GUARD_TIMER
        ccGuardOrDeallocResource(con, CC_OUTGOING);
#else
        ccDeallocateResource(con, CC_OUTGOING);
#endif
        con->ogRscVal = FALSE;
     }

     ret = ccMapEvent(con, CCE_RELIND, 0);
     if (ret != ROK)
     { 
        if (con->direction == INCTOOUT)
        {
           ccSendRelease(con, con->ogSapCb, con->ogSpConnId, con->ogSuConnId,
                         con->ogProtType, con->tempHolderForReleaseCause,NULL);
        }
        else if (con->direction == OUTTOINC)
        {
#ifdef CC_CALEA
           /* For CALEA when mapping fails just don't send Release to the other side */ 
           if (con->icProtType != CC_LOCAL_SWT_PROT)
           {
#endif
              ccSendRelease(con, con->icSapCb, con->icSpConnId, con->icSuConnId,
                            con->icProtType, con->tempHolderForReleaseCause,NULL);
#ifdef CC_CALEA
           }
#endif
        }
        RETVALUE(ROK);
     }
     ccSendReleaseReq(con);
     RETVALUE(ROK);
  }
  RETVALUE(ROK);
}
#endif

/*
*      Fun:   ccConE11S90
*
*      Desc:  Connection state function
*             event - MGCT Txn Cfm.
*             state - CCS_AWTMGCTCFMFORMODIFY2
*
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE11S90
(
CcConCb   *con,             /* connection */
PTR        event,           /* event */
AinBCM_t  *bcm,             /* bcm */
PTR        event2           /* event */
)
{
  MgctTxnCfm       *txnCfm         = NULLP;
  S16               ret            = RFAILED;
  SvcHeader_t       header;
  ResourceClear_t   resourceClear;
  AinBCM_t          *pBcm          = NULLP;

  if ((con == NULLP) || (event == NULLP))
  {
    CCLOGERR("Critical ERROR: Null pointers:%lx, %ld\n", (U32)con, event);
    RETVALUE(RFAILED);
  }
  CCDP(AIN_DBG_LEVEL_1, "ccConE11S90:icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", con->icSuConnId, con->ogSuConnId,
       con->mgBlock->mgCtx->suCtxId, con->icBcm.svcHeader.transId);

  ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));

  txnCfm   = (MgctTxnCfm*) event;
  ret = ccHandleMgResponse(con, txnCfm);
  if (ret != ROK)
  {
    CCLOGERR("ccHandleResponse Failed:%ld\n", con->icSuConnId);
    /*For Announcement + Digits, disconnectFlag is always set to FALSE*/
    if (con->anncBlock.anncType == CC_ANNC_DIGIT)
    {
      /*Always on icBcm*/
      pBcm = &(con->icBcm);

      /* Initializing the message buffers */
      cmMemset((U8 *)&header, 0, sizeof(SvcHeader_t));
      cmMemset((U8 *)&resourceClear, 0, sizeof(ResourceClear_t));

      if (CC_IS_CANCEL_IN_PROGRESS(con))
      {
         /* FID 15045.1 - change clear cause, if cancel in progress */
         con->ainInfo.ClearCause = ClcCapabilityFailure;
         CC_CLR_CALL_FLAG(con->flag, CC_FLAG_CANCEL);
      }
      else
      {
         /* Fill ClearCause with ClcUserAbandon */
         con->ainInfo.ClearCause = ClcResourceNotAvailable;
      }

      /* Fill in parameters in the Service Header Structure */
      ccFillSvcHeader(&header, pBcm, MSG_Resource_Clear);
      CCDP(AIN_DBG_LEVEL_0," Filled SvcHeader\n");

      /* Fill in parameters in the Resource Clear structure */
      ccFillResourceClearParam(&resourceClear, pBcm);
      CCDP(AIN_DBG_LEVEL_0," Filled ResourceClear\n");

      CC_STATE_CHANGE(con, CCS_AWTRSPFORDPE3);

      /* Send the ResourceClear message to the FIC */
      CCDP(AIN_DBG_LEVEL_0,"Sending ResourceClear, TransID=%ld\n",
           header.spTransId);
      CcUiFctResourceClear(&resourceClear, &header);
    }
    else if ((CC_IS_CALL_FLAG_SET(con->flag,CC_FLAG_CALLQUEUED)) && 
             (con->anncBlock.anncType == CC_ANNCONLY))
    {
        CCDP(AIN_DBG_LEVEL_0, "%s: ccHandleResponse Failed:%ld, wait for AnalyzeRoute\n", __FUNCTION__,con->icSuConnId);
        CC_STATE_CHANGE(con, CCS_AWTRSPFORDPE3);
    }
    else
    {
      cleanUpMgQ(con);
      ccHandleCleanUpConCb(con, TRUE, AM_CV_TMPFAIL);
    }
    RETVALUE(ROK);
  }
  else
  {
    ccChangeCallStateOfAssociatedCalls(&(con->icBcm), CCS_CANNOTPROCESSEVENTS);
    ret = ccPostAnnoucementProcessing(con);
    if (ret != ROK)
    {
      CCLOGERR("Error: ccPostAnnoucementProcessing failed 0x%lx\n",
               con->icSuConnId);
      cleanUpMgQ(con);
      ccHandleCleanUpConCb(con, TRUE, AM_CV_TMPFAIL);
    }
    RETVALUE(ROK);
  }
}


/*
*      Fun:   ccConE14S35
*
*      Desc:  Connection state function
*             event - Setup timer expiry
*             state - CCS_AWT_MID_CALL_CFM
*
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE14S35
(
CcConCb   *con,             /* connection */
PTR        event,           /* event */
AinBCM_t  *bcm,             /* bcm */
PTR        event2           /* event */
)
{
  CCLOGERR("ccConE14S35 : Setup timer expired in state = %d, icSu = 0x%lx\n",
           con->state, con->icSuConnId);
  ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
  ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
  RETVALUE(ROK);
}


/*
*      Fun:   ccConE62S10
*
*      Desc:  Connection state function
*             event - MGI Event NOTIFY
*             state - ANSWERED
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE62S10
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  MgctTxnInd        *mgctTxnInd    = NULLP;
  ProtType           icProtType    = 0;
  ProtType           ogProtType    = 0;
  U16                megacoErr     = 0;
  MgctNotifyKind_e   kind          = MGCT_NOTIFY_KIND_UNKNOWN;
  S16                ret           = RFAILED;
  SipCnStEvnt        sipCnStEvnt;
  AinBCM_t           *pBcm         = NULLP;
  FaxData_t          faxData       = {0};
  Bool               vbdFlag       = FALSE;
  Bool               vbdEventSup   = FALSE;
  ModemFaxEvent_e    subKind       = MODEM_FAX_NONE_DETECT; /*FID 16087.0*/
  U8                 faxAction     = SDP_FAXACTION_NONE;/* FID16883.0 */

  CCDP(AIN_DBG_LEVEL_1, "ccConE62S10 icSu = 0x%lx, ogSu = 0x%lx\n",
       con->icSuConnId, con->ogSuConnId);

  if ((con == NULLP) || (event == NULLP) || (bcm == NULLP))
  {
    CCLOGERR("Error: Null pointers con = 0x%lx, event = 0x%lx, bcm = 0x%lx\n",
             (U32)con, event, (U32)bcm);
    RETVALUE(RFAILED);
  }

  icProtType = CC_GET_PROTOCOL (con->icProtType);
  ogProtType = CC_GET_PROTOCOL (con->ogProtType);

  mgctTxnInd = (MgctTxnInd *) event;

  /* FID16465.0 need to differentiate which bcm gets NOTIFY */
  if (con->dtmfInfo)
  {
    if ((IS_DTMF_INFO_INGRESS_ARM(con->dtmfInfo)) &&
        ((bcm->bcmType == BtOBcm) || (bcm->bcmType == BtTAdjBcm))) 
    {
      SET_DTMF_INFO_INGRESS_DET(con->dtmfInfo);
      CCDP(AIN_DBG_LEVEL_1, "%s ingress detect NOTIFY dtmfInfo 0x%x\n", __FUNCTION__, con->dtmfInfo);
    }
    else if ((IS_DTMF_INFO_EGRESS_ARM(con->dtmfInfo)) && (bcm->bcmType == BtTBcm)) 
    {
      SET_DTMF_INFO_EGRESS_DET(con->dtmfInfo);
      CCDP(AIN_DBG_LEVEL_1, "%s egress detect NOTIFY dtmfInfo 0x%x\n", __FUNCTION__, con->dtmfInfo);
    }
  }

  ret = ccHandleMgctNotify(con, (MgctTxnInd *) event);
  if (ret == RFAILED)
  {
    CCLOGERR("Error: ccHandleMgctNotify failed icSu = 0x%lx,"
             "suCtxId 0x%lx, contextId = 0x%lx\n", con->icSuConnId,
             (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0), con->mgBlock->mgCtx->contextId);
    ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
    RETVALUE(ROK);
  }

  /* FID16465.0 clear the tone detection bitmask */
  if (IS_DTMF_INFO_INGRESS_DET(con->dtmfInfo))
    CLEAR_DTMF_INFO_INGRESS_DET(con->dtmfInfo);
  if (IS_DTMF_INFO_EGRESS_DET(con->dtmfInfo))
    CLEAR_DTMF_INFO_EGRESS_DET(con->dtmfInfo);

  if (ret == ROKIGNORE) /* 47396 */
  {
    RETVALUE(ROK);
  }

  ret = mgctGetNotifyReqKind(mgctTxnInd, &megacoErr, &kind, &subKind); /*FID 16087.0*/
 if(ret != ROKIGNORE)
 {
  if (kind == MGCT_NOTIFY_KIND_DIGIT_COMPLETION)
  {
    if (((bcm->bcmType == BtOBcm) && (CC_GET_PROTOCOL(con->ogIntfcCb->protType) == CC_EXT_BICC)) ||
        ((bcm->bcmType == BtTBcm) && (CC_GET_PROTOCOL(con->icIntfcCb->protType) == CC_EXT_BICC)))
    {
      CCDP(AIN_DBG_LEVEL_1, "Received a NOTIFY for SIP/ISUP interworking with BICC.\n");
      ccHandleMgctNotifyForBicc(con, (MgctTxnInd *) event, bcm);
      RETVALUE(ROK);
    }

    if ((con->ainInfo.ODTMFDigitString.pres == PRSNT_NODEF) &&
        (con->icBcm.nel[EReqODTMFEntered] == NEL_REQUEST))
    {
      ret = ccProcessODTMFEntered(&con->icBcm);
      if (ret != ROK)
      {
        CCLOGERR("Error: ccProcessODTMFEntered failed icSu = 0x%lx\n",
                 con->icSuConnId);
        ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
      }
      RETVALUE(ROK);
    }
    else if (ogProtType == CC_SIP)
    {
        INITEVENT((U8 *)&sipCnStEvnt, sizeof(SipCnStEvnt));
        /* Send the connect request to sip indicating that we got the # key */
        ccPrepareSipCnStReqAndSend(con, CCSIP_ET_SIP_INFO_DTMF_DIGITS, con->direction, &sipCnStEvnt);
    }
    else
    {
      CCLOGERR("Error: DTMF detected for unknow application icSu 0x%lx\n",
               con->icSuConnId);
      ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
      RETVALUE(ROK);
    }
  }
  else if(kind == MGCT_NOTIFY_KIND_FAX_DETECT || kind == MGCT_NOTIFY_KIND_MODEM_DETECT 
         || kind == MGCT_NOTIFY_KIND_TEXT_DETECT)
  {
    /* FID 15913.0 */
    if((ccIsIPResource(&con->ogBcm)) &&
       (!ccIsIPResource(&con->icBcm)))
    {
      if (con->icIntfcCb)
        vbdEventSup = con->icIntfcCb->vbdEventSup;

      if (vbdEventSup == TRUE)
      {
        CCDP(AIN_DBG_LEVEL_1,"%s: vbdEventSup=%d ignore fax/modem/text event.\n",
         __FUNCTION__, vbdEventSup);
        RETVALUE(ROK);
      }
    }

    if(ccIsIPResource(&(con->icBcm)) || ccIsIPResource(&(con->ogBcm)))
    {
      if(kind == MGCT_NOTIFY_KIND_MODEM_DETECT)
      {
        /* Bug 70784 */
        if ((con->faxCb.ctypeCallType != CALLTYPE_DATA) &&
            (con->faxCb.ctypeCallType != CALLTYPE_FAX_DATA) &&
              /* FID 14844 -changes */
            (con->faxCb.ctypeCallType != CALLTYPE_DATA_TEXT) &&
            (con->faxCb.ctypeCallType != CALLTYPE_FAX_DATA_TEXT))
        {
          CCDP(AIN_DBG_LEVEL_1,
         "Ignoring MGCT_NOTIFY_KIND_MODEM_DETECT as DATA not armed!\n");
          RETVALUE(ROK);
        }
        if ((ccIsIPResource(&(con->icBcm)) && 
             CC_IS_VBD_NEGOTIATED(con->flag, CC_FLAG_IC_VBD_NEGOTIATED))
            ||
            (ccIsIPResource(&(con->ogBcm)) && 
             CC_IS_VBD_NEGOTIATED(con->flag, CC_FLAG_OG_VBD_NEGOTIATED)))
        {
          /* Drop the MODEM event if VBD was negotiated */
          CCDP(AIN_DBG_LEVEL_1, "Dropping MGCT_NOTIFY_KIND_MODEM_DETECT "
               "as VBD was negotiated.\n");
          RETVALUE(ROK);
        }

        CCDP(AIN_DBG_LEVEL_1, "MGCT_NOTIFY_KIND_MODEM_DETECT.\n");
        con->faxCb.faxLocalSdp |= (0x01 << AUDIO_ONLY_SHIFT_LEVEL);
        con->faxCb.faxTrigger = LOCAL_MODEM_FLAG_DETECTED;

        /*FID 16087.0*/
        if(checkModemFaxIndAndGetFaxAction(con, NULLP))
        {
          if(subKind == MODEM_FAX_HIGH_MODEM_DETECT)
          {
            CCDP(AIN_DBG_LEVEL_2, "%s(): store high modem and ecan info to subFaxTrigger\n",
                  __FUNCTION__);
            SET_MODEM_FAX_EVENT(con->faxCb.subFaxTrigger, LOCAL_HIGH_MODEM_FLAG_DETECT);
            SET_SDP_ECAN_DIR(con->faxCb.subFaxTrigger, SDP_ECAN_DIR_FB);
            SET_SDP_ECAN_ENB(con->faxCb.subFaxTrigger, SDP_ECAN_ENB_OFF);
            SET_SDP_ECAN_TP(con->faxCb.subFaxTrigger, SDP_ECAN_TP_NONE);
          }else if(subKind == MODEM_FAX_LOW_MODEM_DETECT)
          {
            CCDP(AIN_DBG_LEVEL_2, "%s(): store low modem and ecan info to subFaxTrigger\n",
                  __FUNCTION__);
            SET_MODEM_FAX_EVENT(con->faxCb.subFaxTrigger, LOCAL_LOW_MODEM_FLAG_DETECT);
            SET_SDP_ECAN_DIR(con->faxCb.subFaxTrigger, SDP_ECAN_DIR_FB);
            SET_SDP_ECAN_ENB(con->faxCb.subFaxTrigger, SDP_ECAN_ENB_ON);
            SET_SDP_ECAN_TP(con->faxCb.subFaxTrigger, SDP_ECAN_TP_NONE);
          }
        }

        /* FID 15913.0 */
        if (ccNeedToSetPseudoVBD(con))
        {
          CCDP(AIN_DBG_LEVEL_1, "Add Pseudo VBD.\n");
          CC_SET_PSEUDO_VBD(con->flag, CC_FLAG_NEED_PSEUDO_VBD);
        }
      }
      /* FID 14844 - Handle the Notify with TEXT evnt , Use PASSTHRU procedure 
         if VBD was not negotiated. Drop the event if the VBD wsa negotiated  */
      else if (kind == MGCT_NOTIFY_KIND_TEXT_DETECT)
      {
        if ((con->faxCb.ctypeCallType != CALLTYPE_TEXT) &&
            (con->faxCb.ctypeCallType != CALLTYPE_FAX_TEXT) &&
            (con->faxCb.ctypeCallType != CALLTYPE_DATA_TEXT) &&
            (con->faxCb.ctypeCallType != CALLTYPE_FAX_DATA_TEXT))
        {
          CCDP(AIN_DBG_LEVEL_1,
         "Ignoring MGCT_NOTIFY_KIND_TEXT_DETECT as TEXT was not armed!\n");
          RETVALUE(ROK);
        }

        if (con->faxCb.faxTrigger == LOCAL_TEXT_FLAG_DETECTED)
        {
          /* Drop the TEXT event as it is being received more than once */
          CCDP(AIN_DBG_LEVEL_1, "Dropping MGCT_NOTIFY_KIND_TEXT_DETECT "
               "as it is being received more than once.\n");
          RETVALUE(ROK);
        }

        con->faxCb.faxTrigger = LOCAL_TEXT_FLAG_DETECTED;

        /* TEXT event is detected only on the T-MGC, hence checking whether 
           the VBD is negotiated on the ic IP leg as egress is TDM */
        /* Bug# 79371 - Handling TDM-IP case */
        if ((ccIsIPResource(&(con->icBcm)) && 
             CC_IS_VBD_NEGOTIATED(con->flag, CC_FLAG_IC_VBD_NEGOTIATED))
            ||
            (ccIsIPResource(&(con->ogBcm)) && 
             CC_IS_VBD_NEGOTIATED(con->flag, CC_FLAG_OG_VBD_NEGOTIATED)))
        {
          /* Drop the TEXT event if VBD was negotiated */
          CCDP(AIN_DBG_LEVEL_1, "Dropping MGCT_NOTIFY_KIND_TEXT_DETECT "
               "as VBD was negotiated.\n");
          RETVALUE(ROK);
        }

        CCDP(AIN_DBG_LEVEL_1, "MGCT_NOTIFY_KIND_TEXT_DETECT.\n");
        con->faxCb.faxLocalSdp = 0;
        con->faxCb.faxLocalSdp |= (0x01 << AUDIO_ONLY_SHIFT_LEVEL);

        /* FID 15913.0 */
        if (ccNeedToSetPseudoVBD(con))
        {
          CCDP(AIN_DBG_LEVEL_1, "Add Pseudo VBD.\n");
          CC_SET_PSEUDO_VBD(con->flag, CC_FLAG_NEED_PSEUDO_VBD);
        }
      }
      else
      {
        /* Bug 70784 */
        if ((con->faxCb.ctypeCallType != CALLTYPE_FAX) &&
            (con->faxCb.ctypeCallType != CALLTYPE_FAX_DATA) &&
              /* FID 14844 -changes */
            (con->faxCb.ctypeCallType != CALLTYPE_FAX_TEXT) &&
            (con->faxCb.ctypeCallType != CALLTYPE_FAX_DATA_TEXT))
        {
          CCDP(AIN_DBG_LEVEL_1,
         "Ignoring MGCT_NOTIFY_KIND_FAX_DETECT as FAX not armed!\n");
          RETVALUE(ROK);
        }
        /* BUG86040 */
        if ((con->faxCb.faxTrigger == LOCAL_MODEM_FLAG_DETECTED) &&
            (kind == MGCT_NOTIFY_KIND_FAX_DETECT)) 
        {

           if ((ccIsIPResource(&(con->icBcm)) || ccIsIPResource(&(con->ogBcm))) &&
                 ccIs7510MgVariant(bcm))
           {
              /* FID16883.0 For ANS/ANSAM/ect reported before and V21flag
               * detected
               */
              /* Get the faxAction */
              checkModemFaxIndAndGetFaxAction(con, &faxAction);
              
              if ((faxAction == SDP_FAXACTION_VBDFALLBACKT38) &&
                   ((ccIsIPResource(&(con->icBcm)) && 
                     CC_IS_VBD_NEGOTIATED(con->flag, CC_FLAG_IC_VBD_NEGOTIATED)) ||
                    (ccIsIPResource(&(con->ogBcm)) && 
                     CC_IS_VBD_NEGOTIATED(con->flag, CC_FLAG_OG_VBD_NEGOTIATED)) || 
                     CC_IS_VBDFALLBACK_NEEDED(con->flag2, CC_FLAG_VBDFALLBACKNEEDED) ||
                     (CC_IS_PSEUDO_VBD(con->flag,CC_FLAG_NEED_PSEUDO_VBD)))
                 )
              {
                 /* VBD negotiated, ANS/ANSAM/ect received before and
                  * faxAction set to be VBD fallbck to T38, do not
                  * ignore this FAX event
                  */
                  CCDP(AIN_DBG_LEVEL_1, "MGCT_NOTIFY_KIND_FAX_DETECT and VBD negotiated before!\n");   
              }
              else
              {
                 /* Drop the MODEM event as it is being received more than once */
                 CCDP(AIN_DBG_LEVEL_1, "Dropping MGCT_NOTIFY_KIND_FAX_DETECT "
                       "as PsuedoVBD turned ON 7510 for Modem event.\n");
                 RETVALUE(ROK);
              }
          }
        }
        CCDP(AIN_DBG_LEVEL_1, "MGCT_NOTIFY_KIND_FAX_DETECT.\n");
      }

      /* Bug# 79371 - Handling TDM-IP case */
      if(ccIsIPResource(&(con->icBcm)) && (CC_GET_PROTOCOL(con->icProtType) != CC_EXT_BICC))
      {
         pBcm = &con->icBcm;
         vbdFlag = (CC_IS_VBD_NEGOTIATED(con->flag, CC_FLAG_IC_VBD_NEGOTIATED)?TRUE:FALSE);
      }
      else
      {
         pBcm = &con->ogBcm;
         vbdFlag = (CC_IS_VBD_NEGOTIATED(con->flag, CC_FLAG_OG_VBD_NEGOTIATED)?TRUE:FALSE);
      }

      if(ccGetFaxParams(pBcm, &faxData, TGP_FAX_MODEM_DET_NONE) == ROK)
      {
        /* FID 14844- Ignore FAX event if VBD was negotiated on ingress side 
           and faxAction is PASSTHRU */
        if ((kind == MGCT_NOTIFY_KIND_FAX_DETECT) &&
            (faxData.faxAction == SDP_FAXACTION_PASSTHRU) && vbdFlag)
        {
          /* Drop the V21 event if VBD was negotiated */
           CCDP(AIN_DBG_LEVEL_1, "Dropping MGCT_NOTIFY_KIND_TEXT_DETECT "
                "as VBD was negotiated.\n");
           RETVALUE(ROK);
        }

        CCDP(AIN_DBG_LEVEL_0,"%s(), con->flag =0x%lx, IsVbdNegotiated=%d\n",
           __FUNCTION__, con->flag, vbdFlag);

        /* FID16883.0 For ANS reported before and VBD negotiatied, needto
         *  process this FAX event
         */
        if ( (kind == MGCT_NOTIFY_KIND_FAX_DETECT) && 
              (con->faxCb.faxTrigger == LOCAL_MODEM_FLAG_DETECTED) &&
              (faxData.faxAction == SDP_FAXACTION_VBDFALLBACKT38) &&
              ((ccIsIPResource(&(con->icBcm)) &&
                CC_IS_VBD_NEGOTIATED(con->flag, CC_FLAG_IC_VBD_NEGOTIATED)) ||
               (ccIsIPResource(&(con->ogBcm)) &&
                CC_IS_VBD_NEGOTIATED(con->flag,CC_FLAG_OG_VBD_NEGOTIATED))  ||
               (CC_IS_VBDFALLBACK_NEEDED(con->flag2, CC_FLAG_VBDFALLBACKNEEDED))||
               (CC_IS_PSEUDO_VBD(con->flag, CC_FLAG_NEED_PSEUDO_VBD))))
        {
           CCDP(AIN_DBG_LEVEL_1,  "%s: V21 after ANS/ANSAM and faxAction to be VBDFALLBACKT38"
                 "set the localFaxTriggerto be %d\n",
                 __FUNCTION__, LOCAL_V21_FLAG_DETECTED);
           con->faxCb.faxTrigger = LOCAL_V21_FLAG_DETECTED;
        }
        ccTestFaxParamsForModifyOnNotify(&faxData, con, vbdFlag);
      }

      ccProcessFaxModemTextDetect((PTR)con, ROK);
    }
  }
  /*FID 16229.0*/
  else if (kind == MGCT_NOTIFY_KIND_FAX_EOF_OR_PI)  
  {
    con->faxCb.faxTrigger = LOCAL_PI_EOF_FLAG_DETECTED;
    ccProcessFaxModemTextDetect((PTR)con, ROK);
  }
#if 1 /* Bug# 62538 */
  else if (kind == MGCT_NOTIFY_KIND_SIGNAL_COMPLETION)
  {
    CCDP(AIN_DBG_LEVEL_1,
         "Ignore NOTIFY with MGCT_NOTIFY_KIND_SIGNAL_COMPLETION in state=10\n");
  }
#endif
  else
  {
    /* FID 14841.0 */
    HNDL_MGCT_NOT_FOR_ADID(kind, con, event, bcm);
    CCLOGERR("Error: Unsupported NOTIFY type %d\n", kind);
    ABORT_DEBUG;
  }
 }

  RETVALUE(ROK);
}

/*
*      Fun:   ccConE04S51
*
*      Desc:  Connection state function
*             event - Release Indication
*             state - CCS_AWTBUSYREL_IC
*
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE04S51
(
CcConCb   *con,             /* connection */
PTR        event,           /* event */
AinBCM_t  *bcm,             /* bcm */
PTR        event2           /* event */
)
{
  S16         ret        = RFAILED;
  ProtType    icProtType = CC_GET_PROTOCOL(con->icProtType);

  CCDP(AIN_DBG_LEVEL_1, "ccConE04S51 0x%lx\n", con->icSuConnId);
  ret = ccProcessDisconnectEvent(con, event);
  if (con->surrogate != NULLP)
    ccProcessDisconnectNotfnForSurrogateLegs(con, event);
  if (ret == ROK)
  {
    CCDP(AIN_DBG_LEVEL_0,"ccProcessDisconnectEvent success,event detec.\n");
    RETVALUE(ROK);
  }
  else if (ret == RFAILED)
  {
    CCDP(AIN_DBG_LEVEL_3,"Failed to Check if (O/T)Disc event is armed\n");
  }
  /* If the ccProcessDisconnectEvent returns ROKDNA, we continue */
  CCDP(AIN_DBG_LEVEL_0, "O/T_Disconnect event is not Armed\n");

  /* Stop the ToneTimer, infact it is the SETUP timer  */
  ccStopConTmr(con, TMR_SETUP);
  ccStartConTmr(TMR_RELEASE, con, (PTR)&ccCp.genCfg);

  if (icProtType != CC_LOCAL_SWT_PROT)
  {
    ret = ccProcessDisconnectNotfnForCaleaLegs(con, event);
    if (ret != ROK)
    {
      CCDP(AIN_DBG_LEVEL_4,
          "Critical Error!!ccProcessDisconnectNotfnForCaleaLegs FAILED\n");
    }
  }
  ret = RFAILED;
  if (CC_IS_CONTEXT_PRESENT(con))
  {
    ccQSubBasedOnCcn(con);  /* FID 15261.0 */

    con->relPend = INRELRSPANDNOREL;
    con->direction = INCTOOUT;
    CC_STATE_CHANGE(con, CCS_AWTSWTCFM_SENDNOREL);
    con->mgBlock->con = con;
    ret = ccSendMgiQueue(0, con->mgBlock);
    if (ret == ROK)
      RETVALUE(ROK);
  }
  if (ret != ROK)
  {
    CCLOGERR("ccUpdateMgQForSubRsc not sent:icSu:0x%lx, ctxId:0x%lx, ret:%d\n",
             con->icSuConnId, con->mgBlock->mgCtx->suCtxId, ret);
    if (con->noRspReqd == FALSE) ccSendReleaseRsp(con);
    CC_STATE_CHANGE(con, CCS_AWTDEALOCCFM);
    ccDeallocateResource(con, CC_INCOMING);
  }
  RETVALUE(ROK);
}


/*
*
*      Fun:   ccConE24S00
*
*      Desc:  Connection state function
*             event - CCE_CCR
*             state - CCS_IDLE
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE24S00
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  CCDP(AIN_DBG_LEVEL_1, "ccConE24S00 0x%lx\n", con->icSuConnId);

  /* FID2001 Set start time for opr-ccr cdr */
  if (ccInit.acnt == TRUE)
  {
    SwTimevalGet (&con->callDtlInfo.cm.timevalStart,
                  SW_TIMEVAL_IcConIndRcvd, NULL);
    con->callDtlInfo.cm.tckIcConIndRcvd.pres = PRSNT_NODEF;
    (Void) SGetSysTime(&con->callDtlInfo.cm.tckIcConIndRcvd.val);
  }

  ccStartConTmr(TMR_SETUP, con, (PTR)&ccCp.genCfg);
  con->cotBlock.contType = CC_CCR_INITIATED;
  CC_STATE_CHANGE(con, CCS_AWTROUTERSC);
  ccAllocateResource(con, (CcConEvnt *)NULLP, CC_INCOMING, RMT_RESERVE);
  RETVALUE(ROK);
}


/*
*
*      Fun:   ccConE11S24
*
*      Desc:  Connection state function
*             event - MGCT Confirm.
*             state - CCS_AWTMGCTCFMCOTIC0
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE11S24
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  MgctTxnCfm       *txnCfm         = NULLP;
  S16               ret            = RFAILED;

  if ((con == NULLP) || (event == NULLP))
  {     
    CCLOGERR("Critical ERROR: Null pointers:%lx, %ld\n", (U32)con, event);
    RETVALUE(RFAILED);
  } 
  CCDP(AIN_DBG_LEVEL_1, "ccConE11S24:icSu:0x%lx, ogSu:0x%lx, suCtxId:0x%lx,"
       " icTransId:0x%lx\n", con->icSuConnId, con->ogSuConnId,
       con->mgBlock->mgCtx->suCtxId, con->icBcm.svcHeader.transId);
  ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
  
  txnCfm   = (MgctTxnCfm*) event;
  ret = ccHandleMgResponse(con, txnCfm);
  if (ret != ROK)
  {
    CCLOGERR("ccHandleResponse Failed:%ld\n", con->icSuConnId);
    cleanUpMgQ(con);
    ccHandleCleanUpConCb(con, TRUE, AM_CV_TMPFAIL);
    RETVALUE(ROK);
  }
  else
  {
    /* China ISUP dev: change to new state CCS_AWTMOREINFO waitting for possible
     * COT/INF/SAM if con->waitForMoreInfo is true.
     */
    if(CC_IS_WAITMORE_INFO(con->waitForMoreInfo))
    {
        CCDP(AIN_DBG_LEVEL_0,"Got Mgct confirm and wait for more info con->waitForMoreInfo=0x%02x,"
             "con->cotBlock.contType=%2d\n",
             con->waitForMoreInfo, con->cotBlock.contType);
        con->lastState = con->state;
        CC_STATE_CHANGE(con, CCS_AWTMOREINFO);
    }
    else
    {
        CC_STATE_CHANGE(con, CCS_AWTCOT);
    }

    if (con->cotBlock.contType == CC_CCR_INITIATED)
    {
      CCDP(AIN_DBG_LEVEL_0, "Sending LoopBack Ack 0x%lx\n", con->icSuConnId);
      ccSendStaReq(con, CCE_LOOPBACKACK, 0, CC_INCOMING);
    }
  }
  RETVALUE(ROK);
}


/*
* 
*      Fun:   ccConE14S24
* 
*      Desc:  Connection state function
*             event - Setup Timer Expiry.
*             state - CCS_AWTMGCTCFMCOTIC0
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*   
*      Notes: None.          
*   
*      File:  cc_bdy2_2.c
*   
*/  
PUBLIC S16 ccConE14S24
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  CCLOGERR("Error: ccConE14S24: Setup Timer expired in state %d, icSu 0x%lx,"
           "ogSu 0x%lx\n", con->state, con->icSuConnId, con->ogSuConnId);
  cleanUpMgQ(con);
  ccCleanUpMGCtx(con);
  ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
  RETVALUE(ROK);
}


/*
* 
*      Fun:   ccConE23SBUF
* 
*      Desc:  Connection state function
*             event - Continuity Report (CCE_CONTREP).
*             state - CCS_AWTMGCTCFMCOTIC0
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*   
*      Notes: None.          
*   
*      File:  cc_bdy2_2.c
*   
*/
PUBLIC S16 ccConE23SBUF
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  CCDP(AIN_DBG_LEVEL_1, "ccConE23SBUF:icSu 0x%lx ogSu 0x%lx\n",
       con->icSuConnId, con->ogSuConnId);
  ccBufferEventToQueue(con, bcm, event, CCE_CONTREP, sizeof(CcStaEvnt),
                       con->icSuConnId, NULLP, 0);

  /* mark the buffer bit for COT Report event */
  CC_MARK_COTBUFFERED(con);

  RETVALUE(ROK);
}


/*
* 
*      Fun:   ccConE23S25
* 
*      Desc:  Connection state function
*             event - Continuity Report (CCE_CONTREP).
*             state - CCS_AWTCOT
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*   
*      Notes: None.
*   
*      File:  cc_bdy2_2.c
*   
*/
PUBLIC S16 ccConE23S25
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  S16               ret            = RFAILED;

  CCDP(AIN_DBG_LEVEL_1, "ccConE23S25:icSu 0x%lx ogSu 0x%lx\n",
       con->icSuConnId, con->ogSuConnId);
  CCFILLCOTSTAT(con, CC_IC_COT_SUCCESS, CC_ACTION_INCREMENT);

  if (CC_IS_CONTEXT_PRESENT(con))
  {
    ccUpdateMgQForSubRsc(NULLP, con->mgBlock->mgCtx->suCtxId, con);
    ccChangeCallStateOfAssociatedCalls(&(con->icBcm), CCS_CANNOTPROCESSEVENTS);
    CC_STATE_CHANGE(con, CCS_AWTMGCTCFMCOTIC1);
    con->mgBlock->con = con;
    ret = ccSendMgiQueue(con->mgBlock->mgCtx->suCtxId, con->mgBlock);
    if (ret == ROK)
      RETVALUE(ROK);
  }
  else
  {
        ccSendOrSkipInfoCollected(con);
    	RETVALUE(ROK);
  }
  if (ret != ROK)
  {
    CCLOGERR("ccUpdateMgQForSubRsc not sent:icSu:0x%lx, ctxId:0x%lx, ret:%d\n",
             con->icSuConnId, con->mgBlock->mgCtx->suCtxId, ret);
    cleanUpMgQ(con);
    ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
  }
  RETVALUE(ROK);
}


/*
* 
*      Fun:   ccConE14S25
*                               
*      Desc:  Connection state function 
*             event - Setup Timer Expiry.
*             state - CCS_AWTCOT
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*   
*      Notes: None.          
*   
*      File:  cc_bdy2_2.c
*   
*/  
PUBLIC S16 ccConE14S25
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  CCLOGERR("Error: ccConE14S25: Setup Timer expired in state %d, icSu 0x%lx,"
           "ogSu 0x%lx\n", con->state, con->icSuConnId, con->ogSuConnId);
  cleanUpMgQ(con);
  ccCleanUpMGCtx(con);
  ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
  RETVALUE(ROK);
}


/*
* 
*      Fun:   ccConE04S25
* 
*      Desc:  Connection state function
*             event - Release Indication.
*             state - CCS_AWTCOT
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*   
*      Notes: None.
*       
*      File:  cc_bdy2_2.c
*   
*/
PUBLIC S16 ccConE04S25
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
) 
{
  S16               ret            = RFAILED;
  
  CCDP(AIN_DBG_LEVEL_1, "ccConE04S25:icSu 0x%lx ogSu 0x%lx\n",
       con->icSuConnId, con->ogSuConnId);

  if ((event != NULLP) && (((CcRelEvnt *)event)->m.siRelEvnt.optElmnt.m.isup.COT_flag == INCOM_COT_FAIL))
      CCFILLCOTSTAT(con, CC_IC_COT_FAIL, CC_ACTION_INCREMENT);

  if (CC_IS_CONTEXT_PRESENT(con))
  {
    ccUpdateMgQForSubRsc(NULLP, con->mgBlock->mgCtx->suCtxId, con);
    con->relPend = INRELRSPANDNOREL;
    con->direction = INCTOOUT;
    CC_STATE_CHANGE(con, CCS_AWTSWTCFM_SENDNOREL);
    con->mgBlock->con = con;
    ret = ccSendMgiQueue(con->mgBlock->mgCtx->suCtxId, con->mgBlock);
    if (ret == ROK)
      RETVALUE(ROK);
  }
  if (ret != ROK)
  {
    CCLOGERR("ccUpdateMgQForSubRsc not sent:icSu:0x%lx, ctxId:0x%lx, ret:%d\n",
             con->icSuConnId, con->mgBlock->mgCtx->suCtxId, ret);
    if (con->noRspReqd == FALSE) ccSendReleaseRsp(con);
    CC_STATE_CHANGE(con, CCS_AWTDEALOCCFM);
    ccDeallocateResource(con, CC_INCOMING);
  }
  RETVALUE(ROK);
}


/*
*
*      Fun:   ccConE11S26
*
*      Desc:  Connection state function
*             event - MGCT Confirm.
*             state - CCS_AWTMGCTCFMCOTIC1
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE11S26
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  MgctTxnCfm            *txnCfm              = NULLP;
  S16                    ret                 = RFAILED;

  if ((con == NULLP) || (event == NULLP))
  {
    CCLOGERR("Critical ERROR: Null pointers:%lx, %ld\n", (U32)con, event);
    RETVALUE(RFAILED);
  }
  CCDP(AIN_DBG_LEVEL_1, "ccConE11S26:icSu:0x%lx, ogSu:0x%lx, suCtxId:0x%lx,"
       " icTransId:0x%lx\n", con->icSuConnId, con->ogSuConnId,
       con->mgBlock->mgCtx->suCtxId, con->icBcm.svcHeader.transId);
  ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));

  txnCfm   = (MgctTxnCfm*) event;
  ret = ccHandleMgResponse(con, txnCfm);
  if (ret != ROK)
  {
    CCLOGERR("ccHandleResponse Failed:%ld\n", con->icSuConnId);
    cleanUpMgQ(con);
    ccHandleCleanUpConCb(con, TRUE, AM_CV_TMPFAIL);
    RETVALUE(ROK);
  }
  else
  {
    /* China ISUP dev: change to new state CCS_AWTMOREINFO waitting for possible
     * INF/SAM if con->waitForMoreInfo is true or there are buffered events.
     */
    if(CC_IS_WAITMORE_INFO(con->waitForMoreInfo) || (CC_IS_SAMBUFFERED(con) || CC_IS_INFBUFFERED(con)))
    {
        CCDP(AIN_DBG_LEVEL_0, "ccConE11S26: wait for more info,con->waitForMoreInfo=0x%02x,con->cotBlock.contType=%2d\n",
         con->waitForMoreInfo, con->cotBlock.contType);
        con->lastState = con->state;
        CC_STATE_CHANGE(con, CCS_AWTMOREINFO);
        RETVALUE(ROK);
    }
    switch(con->cotBlock.contType)
    {
      case CC_IAM_INITIATED:
        	ccSendOrSkipInfoCollected(con);
      break;
      case CC_CRM_INITIATED:
      case CC_CCR_INITIATED:
        CC_STATE_CHANGE(con, COT_IC_DONE);
        break;
      default:
        CCLOGERR("Error: Invalid case statement %d, icSu = 0x%lx\n",
                 con->cotBlock.contType, con->icSuConnId);
        ccHandleCleanUpConCb(con, TRUE, AM_CV_TMPFAIL);
        break;
    }
    RETVALUE(ROK);
  }
}


/*
* 
*      Fun:   ccConE14S26
*                               
*      Desc:  Connection state function 
*             event - Setup Timer Expiry.
*             state - CCS_AWTMGCTCFMCOTIC1
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*   
*      Notes: None.          
*   
*      File:  cc_bdy2_2.c
*   
*/ 
PUBLIC S16 ccConE14S26
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  CCLOGERR("Error: ccConE14S26: Setup Timer expired in state %d, icSu 0x%lx,"
           "ogSu 0x%lx\n", con->state, con->icSuConnId, con->ogSuConnId);
  cleanUpMgQ(con);
  ccCleanUpMGCtx(con);
  ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
  RETVALUE(ROK);
}


/*
* 
*      Fun:   ccConE00S27
*                               
*      Desc:  Connection state function 
*             event - Connect indication.
*             state - COT_IC_DONE.
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*   
*      Notes: None.          
*   
*      File:  cc_bdy2_2.c
*   
*/
PUBLIC S16 ccConE00S27
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  S16 ret;
#if 0 /* Bug: 45275 */
  msgValCcIDEnum         temp_ccId           = CiInvalid;
  U8                     index               = 0;
  SvcHeader_t            header;
  InfoCollected_t        infoCollected;
#endif

  CCDP(AIN_DBG_LEVEL_1, "ccConE00S27:icSu:0x%lx, ogSu:0x%lx, suCtxId:0x%lx,"
       " icTransId:0x%lx\n", con->icSuConnId, con->ogSuConnId,
       (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0), con->icBcm.svcHeader.transId);

#if 0 /* Bug: 45275 */
  cmMemset((U8 *)&header, 0, sizeof(SvcHeader_t));
  cmMemset((U8 *)&infoCollected, 0, sizeof(InfoCollected_t));
  con->cotBlock.contType = CC_NULL_COTTYPE;

#if 1 /* EARLY_ACM */
  if (IC_EARLY_ACM_TMR_ENABLED(con))
  {
    ccStartConTmr(CC_TMR_EARLYACM, con, (PTR)&ccCp.genCfg);
  }
#endif

  if (con->icBcm.cc)
    temp_ccId = con->icBcm.cc->ccId;
  else
    temp_ccId = CiOriginatingSetup;

  ccUpdateBcm(con, PicCollectInformation, temp_ccId, BtOBcm);
  for (index = 0; index < EReqMax; index++)
  {
    con->icBcm.nel[index] = NEL_UNARMED;
  }
  CCDP(AIN_DBG_LEVEL_0," Fill InfoColl 0x%lx\n", con->icSuConnId);
  ccFillSvcHeader(&header, &(con->icBcm), MSG_Info_Collected);
  ccFillInfoColltdParam(&infoCollected, &(con->icBcm));
#endif /* Bug: 45275 */

  /*
   * SETUP (IAM) after CRM forms InfoCollected and sends it to FIC.
   * Incoming resource has been allocated by CRM already.
   *
   * Incorporate ccConE00S00 code where necessary.
   * This is because IAM after CRM does not go thru ccConE00S00.
   * Instead, the IAM is sent by ISUP in a StaInd to GCC.
   */
  if ((con->cotBlock.contType == CC_CRM_INITIATED) ||
      (con->cotBlock.contType == CC_CRM_NOCONT))
  {
     RmAlocReqEvnt   rscEvnt;
     ProtType icProtType = CC_GET_PROTOCOL(con->icProtType);

     /* Incorporate ccConE00S00. */
     noOfCalls++;
     if (ccInit.acnt == TRUE)
     {
#if 1 /* change from local time to UTC in billing */
       SwTimevalGet (&con->callDtlInfo.cm.timevalStart,
                     SW_TIMEVAL_IcConIndRcvd, NULL);
#else
       (Void) SGetDateTime(&con->callDtlInfo.cm.tmIcConIndRcvd);
#endif
       con->callDtlInfo.cm.tckIcConIndRcvd.pres = PRSNT_NODEF;
       (Void) SGetSysTime(&con->callDtlInfo.cm.tckIcConIndRcvd.val);
     }

     cmMemset ((U8 *)&con->carrierSelection, 0, sizeof(TknU8));
     if (!CM_IS_TMR_RUNNING(con->timers, MAXCCCONCBTIMER, TMR_SETUP))
          ccStartConTmr(TMR_SETUP, con, (PTR)&ccCp.genCfg);
     ccCp.sts.totalCalls++;

     if (ccIsSTDigitPresInEvent(con, event, CCE_CONIND))
     con->STDigitRecd = TRUE;

     /* NOTE: For CAS we do the copying of CdPrtyNmb into CollectAddrInfo only
      * after receiving addrCfm in ccConE35S34() */
     if ((con->icProtType != CC_CS_LN) &&
         (con->icProtType != CC_CS_TG) &&
         (con->icProtType != CC_MGCP_LN) &&
         (con->icProtType != CC_MGCP_TG) &&
         (con->icProtType != CC_GR303)) 
     {
       /* Populating the Called Party Address in the collected Address field */
       if (ccCopyCdPty2ClAddr(con) !=ROK)
       {
         CCDP(AIN_DBG_LEVEL_0," Populating the Called Party Address failed\n");
         ccStopConTmr(con, TMR_SETUP);
         ccStartConTmr(TMR_RELEASE, con, (PTR)&ccCp.genCfg);
         ccCp.sts.fRoutUnavail++;
         CC_STATE_CHANGE(con, CCS_AWTRELCFM_IC);
         ccSendRelease(con, con->icSapCb, con->icSpConnId, con->icSuConnId,
                       con->icProtType, CCTMPFAIL, NULL);
         RETVALUE(RFAILED);
       }
     }
     /* Initialize the icBcm field in the CcConCb */
     ret = SGetSBuf(ccInit.region, ccInit.pool, (Data **) &(con->icBcm.cc),
                    (Size) sizeof(AinCallConfig_t));
     if (ret != ROK)
     {
       ccGenAlarm(LCM_CATEGORY_RESOURCE, LCC_EVENT_MEMALOC_FAILED,
                  LCC_CAUSE_MALLOC_FAIL, (U8 *) &ret);
       RETVALUE(RFAILED);
     }
     cmMemset((U8 *) (con->icBcm.cc), 0, sizeof(AinCallConfig_t));

     /* Allocate transId  */
     con->icBcm.svcHeader.transId = ccGetNewtransId();
     CCDP(AIN_DBG_LEVEL_0," TransId= %ld\n", con->icBcm.svcHeader.transId);

     ret = ccFillBCMInfo(con, &(con->icBcm), con->icBcm.svcHeader.transId,
                         CiOriginatingSetup, PicONull, BtOBcm,
                         DP_ORIGINATION_ATTEMPT);
     con->icBcm.cc->legId01 = & (con->icBcm);

     /* Insert in icBCM Hash table */
     ret = cmHashListInsert(&ccicBCMInsTbl, (PTR) &(con->icBcm),
                            (U8 *)&(con->icBcm.svcHeader.transId),
                            (U16)sizeof(UConnId));
     if (ret != ROK)
     {
       CCDP(AIN_DBG_LEVEL_3," ccicBCMInsTbl Insertion failed\n");
       ccStopConTmr(con, TMR_SETUP);
       ccStartConTmr(TMR_RELEASE, con, (PTR)&ccCp.genCfg);
       CC_STATE_CHANGE(con, CCS_AWTRELCFM_IC);
       ccSendRelease(con, con->icSapCb, con->icSpConnId, con->icSuConnId,
                     con->icProtType, CCTMPFAIL, NULL);
       RETVALUE(RFAILED);
     }

     /* Bug# 51782 */
     if (((icProtType == CC_SI) || (icProtType == CC_SIPT)) && 
         (con->ccEvntHold != NULLP) && !(CC_INGRESS_INTER_CCS_CALL(con)))
     {                
       /* Set the global flag for interworkingInd field of FwdCallInd*/
       CC_SET_IAM_ECHO_IND(con);
     }

     if ((icProtType == CC_SI) || (icProtType == CC_BICC))
     {
        /* Bug 32680 - This is based on International Requirments, if the call is a CUG call
         * then Release the call with Release Cause = "92 Non_exisitent CUG"
         *
         * Therefore check if the siCugEvent is present in the ConEvnt. If so, 
         * Release the call
         * 
         * Changes made for International, RV Suresh. 5.0 
         */
        if ((con->ccEvntHold != NULLP) && (ccIsCUGCallForTelstra(con, event)))
        {
          CCDP(AIN_DBG_LEVEL_0," CUG Call Not Supported \n");
          ccStopConTmr(con, TMR_SETUP);
          ccStartConTmr(TMR_RELEASE, con, (PTR)&ccCp.genCfg);
          CC_STATE_CHANGE(con, CCS_AWTRELCFM_IC);
          ccSendRelease(con, con->icSapCb, con->icSpConnId, con->icSuConnId,
                        con->icProtType, CCCUGNOTEXIST, NULL);
          RETVALUE(RFAILED);
        }
     }

     ccGeoExtractProtCri(con, (CcAllSdus *)event, CCE_CONIND);

     ccExtractCallReferenceForIAM (con, event);
     ccExtractInfoFromConEvent(con, event);

     /* Bug 45275 + */
     ccExtractNBRscInfo((SiConEvnt *)event, &rscEvnt);
     cmMemcpy((U8 *)&con->tfcDsc, (U8 *)&rscEvnt.tfcDesc,
              sizeof(RmTfcDesc));
  }

  con->cotBlock.contType = CC_NULL_COTTYPE;

#if 1 /* Bug: 45275 */
    ccSendOrSkipInfoCollected(con);
#else
  CC_STATE_CHANGE(con, CCS_AWTRSPFORDPE3);
  ccChangeCallStateOfAssociatedCalls(&(con->icBcm), CCS_CANNOTPROCESSEVENTS);
  CcUiFctInfoCollected(&infoCollected, &header);
#endif
  RETVALUE(ROK);
}


/*
* 
*      Fun:   ccConE14S27
*                               
*      Desc:  Connection state function 
*             event - Setup Timer Expiry.
*             state - COT_IC_DONE.
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*   
*      Notes: None.          
*   
*      File:  cc_bdy2_2.c
*   
*/
PUBLIC S16 ccConE14S27
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  CCLOGERR("Error: ccConE14S27: Setup Timer expired in state %d, icSu 0x%lx,"
           "ogSu 0x%lx\n", con->state, con->icSuConnId, con->ogSuConnId);
  cleanUpMgQ(con);
  ccCleanUpMGCtx(con);
  ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
  RETVALUE(ROK);
}


/*
* 
*      Fun:   ccConE04S27
*                               
*      Desc:  Connection state function 
*             event - Release Indication.
*             state - COT_IC_DONE.
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*   
*      Notes: None.          
*   
*      File:  cc_bdy2_2.c
*   
*/
PUBLIC S16 ccConE04S27
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  CCDP(AIN_DBG_LEVEL_1, "ccConE04S27:icSu:0x%lx, ogSu:0x%lx, suCtxId:0x%lx,"
       " icTransId:0x%lx\n", con->icSuConnId, con->ogSuConnId,
       (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0), con->icBcm.svcHeader.transId);

  if (con->noRspReqd == FALSE) ccSendReleaseRsp(con);
  CC_STATE_CHANGE(con, CCS_AWTDEALOCCFM);
  ccDeallocateResource(con, CC_INCOMING);
  RETVALUE(ROK);
}


/*
* 
*      Fun:   ccConE26S00
*                               
*      Desc:  Connection state function 
*             event - Loop Back Acknowledge.
*             state - CCS_IDLE.
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*   
*      Notes: None.          
*   
*      File:  cc_bdy2_2.c
*   
*/
PUBLIC S16 ccConE26S00
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  /* FID2001 Set start time for opr-ccr cdr */
  if (ccInit.acnt == TRUE)
  {
    SwTimevalGet (&con->callDtlInfo.cm.timevalStart,
                  SW_TIMEVAL_IcConIndRcvd, NULL);
    con->callDtlInfo.cm.tckIcConIndRcvd.pres = PRSNT_NODEF;
    (Void) SGetSysTime(&con->callDtlInfo.cm.tckIcConIndRcvd.val);
  }

  con->cotBlock.contType = CC_CCR_INITIATED_OG;
  ccStartConTmr(TMR_SETUP, con, (PTR)&ccCp.genCfg); /* BUG 39652 */
  ccCp.sts.totalCalls++; /* BUG 39652 */

  CC_STATE_CHANGE(con, CCS_AWTROUTERSC);
  /* Use ALOC_TEST_CALL : It will allow blocked but non-cp-busy cics. It will
   * not check for direction : Bug:44936*/
  ccAllocateResource(con, NULLP, CC_INCOMING, RMT_ALOC_TEST_CALL);

#if 0
  MgctEventsDescriptor       eventDesc;
  MgctSignalsDescriptor      signalDesc;
  MgctSignalName             name;
  MgctEventsDescriptor      *ptr_eventDesc     = NULLP;
  MgctSignalsDescriptor     *ptr_signalDesc    = NULLP;
  S16                        ret               = RFAILED;

  CCDP(AIN_DBG_LEVEL_1, "ccConE26S00:icSu:0x%lx, ogSu:0x%lx, suCtxId:0x%lx,"
       " icTransId:0x%lx\n", con->icSuConnId, con->ogSuConnId,
       con->mgBlock->mgCtx->suCtxId, con->icBcm.svcHeader.transId);

  cmMemset((U8*) &eventDesc, 0, sizeof(MgctEventsDescriptor));
  cmMemset((U8*) &signalDesc, 0, sizeof(MgctSignalsDescriptor));
  cmMemset((U8 *)&name, 0, sizeof(MgctSignalName));
  con->icBcm.bcmType = BtOBcm;
  con->cotBlock.contType = CC_CCR_INITIATED;

  mgctSignalDesc(&con->ctl, PRSNT_NODEF, MGCT_SIG_ID_CT_CT,
                 MGCT_PACKAGE_ID_CT, SIG_SIGNAL_TYPE_TIMEOUT,
                 0, NULLP, NULLP, MGCT_KEEP_ACTIVE_MAX, &signalDesc);

  name.pkgId    = MGCT_PACKAGE_ID_CT;
  name.itemId.g = MGCT_EVT_ID_CT_CMP;
  mgctFormatSimpleEventsDesc(&(con->ctl), con->icSuConnId, name, &eventDesc);
  if (signalDesc.pres == PRSNT_NODEF)
    ptr_signalDesc = &signalDesc;
  if (eventDesc.pres == PRSNT_NODEF)
    ptr_eventDesc = &eventDesc;

  ccUpdateMgQForAddOrModifyRsc(MGCT_MODE_SENDRECV, MGCT_PACKAGE_ID_MAX,
                               MGCT_ITEM_ID_CG_MAX, MGCT_CMD_ADD,
                               NULLP, &(con->icBcm),
                               ptr_eventDesc, NULLP, ptr_signalDesc, NULLP,
                               (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0),
                               MGCT_CONTEXT_CHOOSE, con);
  ccChangeCallStateOfAssociatedCalls(&(con->icBcm),
                                     CCS_CANNOTPROCESSEVENTS);
  CC_STATE_CHANGE(con, CCS_AWTMGCTCFMCOTOG0);
  ret = ccSendMgiQueue(con->mgBlock->mgCtx->suCtxId, con->mgBlock);
  if (ret != ROK) 
  {
    CCLOGERR("ERROR: ccSendMgiQueue FAILED : icSu 0x%lx\n",
             con->icSuConnId);
    ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
  }
#endif
  RETVALUE(ROK);
}


/*
* 
*      Fun:   ccConE26S09
*                               
*      Desc:  Connection state function 
*             event - Loop Back Acknowledge.
*             state - CCS_AWTANS.
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*   
*      Notes: None.          
*   
*      File:  cc_bdy2_2.c
*   
*/
PUBLIC S16 ccConE26S09
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  MgctEventsDescriptor       eventDesc;
  MgctSignalsDescriptor      signalDesc;
  MgctSignalName             name;
  MgctEventsDescriptor      *ptr_eventDesc     = NULLP;
  MgctSignalsDescriptor     *ptr_signalDesc    = NULLP;
  MgctCmdType                cmdType           = MGCT_CMD_MAX;
  MgctPackageId              pkgId  = MGCT_PACKAGE_ID_MAX;
  MgctMode                   mode   = MGCT_MODE_MAX;
  MgctCallProgItemId         signal = MGCT_ITEM_ID_CG_MAX;

  S16                        ret               = RFAILED;
  /* Bug 32694 - Pass the COT timer value passed by ISUP, RV Suresh 5.0*/
  CcAllSdus                 *conEvent = NULLP;
  SiOptElmnt1               *siOptElmnt = NULLP;
  U32                       duration = 0;

  /*CID 15591*/
   if (con == NULLP)
   {
     CCLOGERR( "ERROR: ccConE26S09 CON is NULLP\n");
     RETVALUE(RFAILED);
   }
   /* Bug 79809: in case of stack overflow due to deep function calls,
    * post this event when it is the first time to be received.
    */
   if (reEntryFlag == TRUE)     
   {
      CCDP(AIN_DBG_LEVEL_1, "ccConE26S09: post CCE_LOOPBACKACK to GCC\n");
      ccBufferEventToQueue(con, bcm, event, CCE_LOOPBACKACK, sizeof(CcAllSdus), 0, NULLP, 0);
      ccPostEventToSelf(con);   
      RETVALUE(ROK);
   }                            
  /* Feature 1789 - German ISUP */
  /* stop the Toiw2 Timer if running */
  ccStopConTmr(con, CC_TMR_TOIW2);

  if (event == NULLP)
  {
    CCLOGERR( "ERROR: ccConE26S09 event is NULLP icSu = %ld, ogSu = %ld\n",
                 con->icSuConnId, con->ogSuConnId);
    RETVALUE(RFAILED);
  }
  CCDP(AIN_DBG_LEVEL_1, "ccConE26S09:icSu:0x%lx, ogSu:0x%lx, suCtxId:0x%lx,"
       " icTransId:0x%lx\n", con->icSuConnId, con->ogSuConnId,
       (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0), con->icBcm.svcHeader.transId);

  /* BUG: 61139 : Deleted ccProcessOverLoadControl */

  /* Bug 32694 - Pass the COT timer value passed by ISUP, RV Suresh 5.0*/
  conEvent = (CcAllSdus *) event;
  siOptElmnt = &(conEvent->m.ccStaEvnt.siStaEvnt.optElmnt);
  if (siOptElmnt)
  {
      if (siOptElmnt->isup.cotTimeout.pres)
      {
          duration = siOptElmnt->isup.cotTimeout.val;
      }
  }
  /* ISUP sends duration in deci-seconds whereas MGCT expects the timer in 0.01 seconds unit 
   * therefore multiply ISUP duration by 10 and send it MGCT 
   */
  duration = duration * 10;
  CCDP(AIN_DBG_LEVEL_0, "ccConE26S09 - COT duration value set by isup in E26 = %ld\n",duration);

  /* Bug 65436 */
  if (ccCheckMgContext(&con->ogBcm, &pkgId, &signal, &mode) == ROK)
     cmdType = MGCT_CMD_MODIFY;
  else
     cmdType = MGCT_CMD_ADD;
  
  cmMemset((U8*) &eventDesc, 0, sizeof(MgctEventsDescriptor));
  cmMemset((U8*) &signalDesc, 0, sizeof(MgctSignalsDescriptor));
  cmMemset((U8 *)&name, 0, sizeof(MgctSignalName));
  con->cotBlock.contType = CC_IAM_INITIATED;
  CCFILLCOTSTAT(con, CC_OG_COT_ATTEMPT, CC_ACTION_INCREMENT);

  mgctSignalDesc(&con->ctl, PRSNT_NODEF, MGCT_SIG_ID_CT_CT,
                 MGCT_PACKAGE_ID_CT, SIG_SIGNAL_TYPE_TIMEOUT,
                 duration, NULLP, NULLP, MGCT_KEEP_ACTIVE_MAX, &signalDesc);

  name.pkgId    = MGCT_PACKAGE_ID_CT;
  name.itemId.g = MGCT_EVT_ID_CT_CMP;
  mgctFormatSimpleEventsDesc(&(con->ctl), con->ogSuConnId, name, &eventDesc);
  if (signalDesc.pres == PRSNT_NODEF)
    ptr_signalDesc = &signalDesc;
  if (eventDesc.pres == PRSNT_NODEF)
    ptr_eventDesc = &eventDesc;
  /* FID 15261 Add to set the spId for ogBcm */
  con->ogBcm.mgSpId = ((CcGenSap *)(*ccSfSapLstPtr))->spId;
  ccUpdateMgQForAddOrModifyRsc(MGCT_MODE_SENDRECV, MGCT_PACKAGE_ID_MAX,
                               MGCT_ITEM_ID_CG_MAX, cmdType,
                               &(con->ogBcm),
                               ptr_eventDesc, NULLP, ptr_signalDesc, NULLP,
                               NULLP, (con->ogBcm.mgCtx ? con->ogBcm.mgCtx->suCtxId : 0),
                               MGCT_CONTEXT_CHOOSE, con);
  ccChangeCallStateOfAssociatedCalls(&(con->ogBcm),
                                     CCS_CANNOTPROCESSEVENTS);
  CC_STATE_CHANGE(con, CCS_AWTMGCTCFMCOTOG0);
  con->mgBlock->con = con;
  ret = ccSendMgiQueue((con->ogBcm.mgCtx ? con->ogBcm.mgCtx->suCtxId : 0), con->mgBlock);
  if (ret != ROK) 
  {
    CCLOGERR("ERROR: ccSendMgiQueue FAILED : icSu 0x%lx\n",
             con->icSuConnId);
    cleanUpMgQ(con);
    ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
  }
  RETVALUE(ROK);
} 

/*                             
*   
*      Fun:   ccConE26SBUF     
*   
*      Desc:  Connection state function
*             event - Loop Back Acknowledge 
*             state -        
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
* 
*      Notes: None.            
* 
*      File:  cc_bdy2_2.c
*   
*/
PUBLIC S16 ccConE26SBUF
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  CCDP(AIN_DBG_LEVEL_1, "%s: icSu = %ld, ogSu = %ld,"
       " T.transId = %ld\n", __FUNCTION__,con->icSuConnId, con->ogSuConnId,
       con->ogBcm.svcHeader.transId);
  ccBufferEventToQueue(con, bcm, event, CCE_LOOPBACKACK, sizeof(CcAllSdus), 0,
                       NULLP, 0);
  RETVALUE(ROK);
}


/*
* 
*      Fun:   ccConE11S29
*                               
*      Desc:  Connection state function 
*             event - MGCT Txn Confirm.
*             state - CCS_AWTMGCTCFMCOTOG0
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*   
*      Notes: None.          
*   
*      File:  cc_bdy2_2.c
*   
*/ 
PUBLIC S16 ccConE11S29
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  S16                    ret               = RFAILED;
  MgctTxnCfm            *txnCfm            = (MgctTxnCfm*) event;

  CCDP(AIN_DBG_LEVEL_1, "ccConE11S29:icSu:0x%lx, ogSu:0x%lx, suCtxId:0x%lx,"
       " icTransId:0x%lx\n", con->icSuConnId, con->ogSuConnId,
       con->mgBlock->mgCtx->suCtxId, con->icBcm.svcHeader.transId);

  ret = ccHandleMgResponse(con, txnCfm);
  if (ret != ROK)
  {
    CCLOGERR("ccHandleResponse Failed:%ld\n", con->icSuConnId);
    cleanUpMgQ(con);

    if (con->cotBlock.contType == CC_IAM_INITIATED)
    {
      ccSendStaReq(con, CCE_CONTREP, CONT_CHKFAIL, CC_OUTGOING);
      ccHandleCleanUpConCb(con, TRUE, AM_CV_TMPFAIL);
    }
    else /* it has to be CC_CCR_INITIATED_OG */
    {
      ccSendStaReq(con, CCE_CONTREP, CONT_CHKFAIL, CC_INCOMING);
      CC_STATE_CHANGE(con, COT_OG_DONE);
    }
    RETVALUE(ROK);
  }
  else
  {
    CC_STATE_CHANGE(con, CCS_AWTMGCTNOTIFY1);
  }
  RETVALUE(ROK);
}


/*
* 
*      Fun:   ccConE14S29
*                               
*      Desc:  Connection state function 
*             event - Setup Timer Expiry.
*             state - CCS_AWTMGCTCFMCOTOG0
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*   
*      Notes: None.          
*   
*      File:  cc_bdy2_2.c
*   
*/ 
PUBLIC S16 ccConE14S29
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  CCLOGERR("Error: ccConE14S29: Setup Timer expired in state %d, icSu 0x%lx,"
           "ogSu 0x%lx\n", con->state, con->icSuConnId, con->ogSuConnId);
  cleanUpMgQ(con);
  ccCleanUpMGCtx(con);
  ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
  RETVALUE(ROK);
}


/*
* 
*      Fun:   ccConE14S16
*                               
*      Desc:  Connection state function 
*             event - Setup Timer Expiry.
*             state - COT_OG_DONE
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*   
*      Notes: None.          
*   
*      File:  cc_bdy2_2.c
*   
*/
PUBLIC S16 ccConE14S16
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  CCLOGERR("Error: ccConE14S16: Setup Timer expired in state %d, icSu 0x%lx,"
           "ogSu 0x%lx\n", con->state, con->icSuConnId, con->ogSuConnId);
  cleanUpMgQ(con);
  ccCleanUpMGCtx(con);
  ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
  RETVALUE(ROK);
}


/*
*
*      Fun:   ccConE04S16
*
*      Desc:  Connection state function 
*             event - Release Indication.
*             state - COT_OG_DONE
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.          
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE04S16      
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  CCDP(AIN_DBG_LEVEL_1, "ccConE04S16:icSu:0x%lx, ogSu:0x%lx, suCtxId:0x%lx,"
       " icTransId:0x%lx\n", con->icSuConnId, con->ogSuConnId,
       (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0), con->icBcm.svcHeader.transId);
  if ((con->cotBlock.contType != CC_CCR_INITIATED) && 
      (con->cotBlock.contType != CC_CCR_INITIATED_OG) &&
      (con->direction != INCTOOUT))
  {
    CCLOGERR("ERROR: Unexpected call path contType %d, dir %d, icSu 0x%lx\n",
             con->cotBlock.contType, con->direction, con->icSuConnId);
    ABORT_DEBUG;
    ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
  }
  else
  {
    ccStopConTmr(con, TMR_SETUP);
    ccStartConTmr(TMR_RELEASE, con, (PTR)&ccCp.genCfg);
    ccUpdateCloseCause(con, CC_BOTH, CscCallerAbandon);
    if (con->noRspReqd == FALSE) ccSendReleaseRsp(con);
    CC_STATE_CHANGE(con, CCS_AWTDEALOCCFM);
    ccDeallocateResource(con, CC_INCOMING);
  }
  RETVALUE(ROK);  
}


/*
* 
*      Fun:   ccConE62S30
*                               
*      Desc:  Connection state function 
*             event - MGCT Notify.
*             state - CCS_AWTMGCTNOTIFY1
*      Ret:   ROK     - successful, 
*             RFAILED - unsuccessful
*   
*      Notes: None.           
*   
*      File:  cc_bdy2_2.c
*   
*/
PUBLIC S16 ccConE62S30
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  MgctTxnInd                  *mgctTxnInd    = NULLP;
  MgctObservedEvent           *observed      = NULLP;
  MgctObservedEventParameter  *oep           = NULLP;
  S16                         ret            = RFAILED;
  /* FID 14841.0 */
  U16                megacoErr     = 0;
  MgctNotifyKind_e   kind          = MGCT_NOTIFY_KIND_UNKNOWN;
  ProtType icProtType = 0;
  ProtType ogProtType = 0;


  CCDP(AIN_DBG_LEVEL_1, "ccConE62S30 icSu = 0x%lx, ogSu = 0x%lx\n",
       con->icSuConnId, con->ogSuConnId);

  if ((con == NULLP) || (event == NULLP) || (bcm == NULLP))
  {
    CCLOGERR("Error: Null pointers con = 0x%lx, event = 0x%lx, bcm = 0x%lx\n",
             (U32)con, event, (U32)bcm);
    RETVALUE(RFAILED);
  }

  icProtType = CC_GET_PROTOCOL(con->icProtType);
  ogProtType = CC_GET_PROTOCOL(con->ogProtType);

  mgctTxnInd = (MgctTxnInd *) event;
  ret = ccHandleMgctNotify(con, (MgctTxnInd *) event);
  if (ret != ROK)
  {
    CCLOGERR("Error: ccHandleMgctNotify failed icSu = 0x%lx,"
             "suCtxId 0x%lx, contextId = 0x%lx\n", con->icSuConnId,
             (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0), con->mgBlock->mgCtx->contextId);
    ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
    RETVALUE(ROK);
  }
  CCDP(AIN_DBG_LEVEL_1, "mgctTxnInd->u.notifyReq.error.pres is %d\n", 
      mgctTxnInd->u.notifyReq.error.pres);

  /* FID 14841.0 */
  mgctGetNotifyReqKind((MgctTxnInd *) event, &megacoErr, &kind, NULLP);
  HNDL_MGCT_NOT_FOR_ADID(kind, con, event, bcm);

  con->cotBlock.contResult = CONT_CHKFAIL;
  if (mgctTxnInd->u.notifyReq.error.pres == FALSE)
  {
      observed = mgctTxnInd->u.notifyReq.observedEventsDesc.observedEventList;

      if (observed && observed->observedEventParameterList)
      {
          oep = observed->observedEventParameterList;

          if (oep->type == MGCT_OBSERVED_EVENT_PARAM_OTHER)
          {
              if (oep->u.eventOther.type == MGCT_EV_OTHER_TYPE_CT_CMP_RES)
              {
                  if (oep->u.eventOther.u.cmpres.result == MGCT_EVT_CT_CMP_RES_SUCCESS)
                  {
                      con->cotBlock.contResult = CONT_CHKSUCC;
                      /* FID15068.0 if precnd call, send StaReq to stop T24*/
                      if((icProtType == CC_SIP||icProtType == CC_SIPT||icProtType == CC_BICC))
                      {
                        if(CC_IS_CALL_FLAG2(con,CC_FLAG_PRECONDITION) && (ogProtType == CC_SI))
                        {
                          ccSendStaReq(con, CCE_CONTREP, CONT_STOPT24, CC_OUTGOING);
                        }
                      }
                  }
                  else if (oep->u.eventOther.u.cmpres.result == MGCT_EVT_CT_CMP_RES_MAX)
                  {
                      CCLOGERR("Error: Received invalid COT result\n"); 
                      RETVALUE(RFAILED);
                  }
              }
          }
      }
  }

  if (con->cotBlock.contType == CC_IAM_INITIATED) /* 41920 */
  {
    
     if (CC_IS_CALL_FLAG2(con, CC_FLAG_PRECONDITION) && con->icIpParam.locDesc == NULLP)
     {
        AinBCM_t* prevBcm = NULLP;
        if(!CC_IS_CALL_MULTI_MG(con))
        {
            prevBcm = &(con->icBcm);
        }
        else
        {
            prevBcm = con->ogAdjBcm; 
        }

        if (prevBcm)
        {
          ccUpdateMgQForAddOrModifyRsc(MGCT_MODE_SENDRECV, MGCT_PACKAGE_ID_MAX,
                               MGCT_ITEM_ID_CG_NULL, MGCT_CMD_MODIFY,
                               prevBcm, NULLP, NULLP,
                               NULLP, NULLP, NULLP,
                               (prevBcm->mgCtx?prevBcm->mgCtx->suCtxId:0),
                               MGCT_CONTEXT_CHOOSE, con);
        }
     }
     /* Check if there is an assocBlock, must be a CRBT call, don't 
      * sub context, just remove signal 
      */
     if ((CC_IS_ASSOCBLOCK_PRESENT(con)) ||
         ((CC_IS_CALL_TAPPED(con) && (CC_IS_X3_ALLOWED(con)))) ||
         (CC_IS_CALL_FLAG2(con, CC_FLAG_PRECONDITION))) /* BUG: 86451 */
     {
         ccUpdateMgQForAddOrModifyRsc(MGCT_MODE_RECV_ONLY, MGCT_PACKAGE_ID_MAX,
                               MGCT_ITEM_ID_CG_NULL, MGCT_CMD_MODIFY,
                               &con->ogBcm, NULLP, NULLP,
                               NULLP, NULLP, NULLP,
                               (con->ogBcm.mgCtx ? con->ogBcm.mgCtx->suCtxId : 0), 
                               MGCT_CONTEXT_CHOOSE, con);
     }
     else
     {
        ccUpdateMgQForSubRsc(&con->ogBcm, (con->ogBcm.mgCtx ? con->ogBcm.mgCtx->suCtxId : 0), con);
     }
     ccChangeCallStateOfAssociatedCalls(&(con->icBcm), CCS_CANNOTPROCESSEVENTS);
     CC_STATE_CHANGE(con, CCS_AWTMGCTCFMCOTOG1);
     ret = ccSendMgiQueue((con->ogBcm.mgCtx ? con->ogBcm.mgCtx->suCtxId : 0), con->mgBlock);
  }
  else /* It has to be CC_CCR_INITIATED_OG */
  {
    ccUpdateMgQForSubRsc((PTR) NULLP, con->mgBlock->mgCtx->suCtxId, con);
    ccChangeCallStateOfAssociatedCalls(&(con->icBcm), CCS_CANNOTPROCESSEVENTS);
    CC_STATE_CHANGE(con, CCS_AWTMGCTCFMCOTOG1);
    ret = ccSendMgiQueue((con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0), con->mgBlock);
  }
  if (ret == ROK)
    RETVALUE(ROK);
  else
  {
    ABORT_DEBUG;
    CCLOGERR("Error: During switching icSu = 0x%lx\n", con->icSuConnId);
    cleanUpMgQ(con);
    ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
    RETVALUE(ROK);
  }
}


/*
* 
*      Fun:   ccConE14S30
*                               
*      Desc:  Connection state function 
*             event - Setup Timer Expiry.
*             state - CCS_AWTMGCTNOTIFY1
*      Ret:   ROK     - successful, 
*             RFAILED - unsuccessful
*
*      Notes: None.           
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE14S30
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  CCLOGERR("Error: ccConE14S30: Setup Timer expired in state %d, icSu 0x%lx,"
           "ogSu 0x%lx\n", con->state, con->icSuConnId, con->ogSuConnId);
  cleanUpMgQ(con);
  ccCleanUpMGCtx(con);
  ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
  RETVALUE(ROK);
}


/*
* 
*      Fun:   ccConE04S30
*                               
*      Desc:  Connection state function 
*             event - Release Indication.
*             state - CCS_AWTMGCTNOTIFY1
*      Ret:   ROK     - successful, 
*             RFAILED - unsuccessful
*   
*      Notes: None.           
*   
*      File:  cc_bdy2_2.c  
*   
*/  
PUBLIC S16 ccConE04S30
(   
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  S16            ret        = RFAILED;

  /* FID 14732 */                      
  ccExtractSipwMBufFromRel(con, CCE_RELIND, (PTR) event);
  /* FID 14732 */

  if ((ret = ccStoreEvnt(con->evntType, con->icEvntType, con, (U8 *)event,
                         sizeof(CcRelEvnt))) != ROK)
  { 
    CCLOGERR( "ERROR: ccStoreEvnt()failed. Continuing with call\n");
  }
 
#if 1 /* EARLY_ACM */
  ccStopConTmr(con, CC_TMR_EARLYACM);
#endif
 
  CCDP(AIN_DBG_LEVEL_1, "ccConE04S30 0x%lx, 0x%lx\n", con->icSuConnId,
       con->ogSuConnId);
  if (con->direction == INCTOOUT)
  {
    ccUpdateCloseCause(con, CC_BOTH, CscCallerAbandon);

    ccQSubBasedOnCcn(con);  /* FID 15261.0 */

    ccChangeCallStateOfAssociatedCalls(&(con->icBcm), CCS_CANNOTPROCESSEVENTS);
    if (con->cotBlock.contType == CC_IAM_INITIATED)
    {
      con->relPend = INRELRSPANDOUTREL;
      CC_STATE_CHANGE(con, CCS_AWTSWTCFM_SENDONEREL);
    }
    else
    {
      if ((event != NULLP) && (((CcRelEvnt *)event)->m.siRelEvnt.optElmnt.m.isup.COT_flag == OUTGO_COT_FAILT24))
          CCFILLCOTSTAT(con, CC_OG_COT_FAIL, CC_ACTION_INCREMENT);
      con->relPend = INRELRSPANDNOREL;
      CC_STATE_CHANGE(con, CCS_AWTSWTCFM_SENDNOREL);
    }
    ret = ccSendMgiQueue(0, con->mgBlock);
    if (ret != ROK)
    {
      CCLOGERR("ERROR: ccSendMgiQueue failed 0x%lx\n", con->icSuConnId);
      cleanUpMgQ(con);
      ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
    }
    RETVALUE(ROK);
  }
  else if (con->direction == OUTTOINC)
  {
    ccBufferEventToQueue(con, bcm, event, CCE_RELIND, sizeof(CcRelEvnt), 0,
                         NULLP, 0);
    RETVALUE(ROK);
  }
  else
  {
    CCLOGERR("Error: Unexpected direction %d, icSu 0x%lx\n", con->direction,
             con->icSuConnId);
    RETVALUE(ROK);
  }
  RETVALUE(ROK);
}

/*
*
*      Fun:   ccConE11S92
*
*      Desc:  Connection state function
*             event - MGCT Txn Confirm.
*             state - CCS_AWTMGCTCFMCOTOG1
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*   
*      File:  cc_bdy2_2.c
*   
*/
PUBLIC S16 ccConE11S92
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  MgctTxnCfm       *txnCfm         = NULLP;
  S16               ret            = RFAILED;
  U32              direction       = CC_INCOMING;
  ProtType icProtType = 0;
  ProtType ogProtType = 0;

  if ((con == NULLP) || (event == NULLP))
  {
    CCLOGERR("Critical ERROR: Null pointers:%lx, %ld\n", (U32)con, event);
    RETVALUE(RFAILED);
  }
  CCDP(AIN_DBG_LEVEL_1, "ccConE11S92:icSu:0x%lx, ogSu:0x%lx, suCtxId:0x%lx,"
       " icTransId:0x%lx\n", con->icSuConnId, con->ogSuConnId,
       con->mgBlock->mgCtx->suCtxId, con->icBcm.svcHeader.transId);
  
  icProtType = CC_GET_PROTOCOL(con->icProtType);
  ogProtType = CC_GET_PROTOCOL(con->ogProtType);


  ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
  txnCfm   = (MgctTxnCfm*) event;
  ret = ccHandleMgResponse(con, txnCfm);
  if (ret != ROK)
  {
    CCLOGERR("ccHandleResponse Failed:0x%lx\n", con->icSuConnId);
    if (con->cotBlock.contType == CC_IAM_INITIATED)
    {
      direction = CC_OUTGOING;
    }
    else
    {
      direction = CC_INCOMING;
    }
    if (con->cotBlock.contResult != CONT_CHKABORT)
    {
        ccSendStaReq(con, CCE_CONTREP, CONT_CHKFAIL, direction);
    }
    cleanUpMgQ(con);
    ccHandleCleanUpConCb(con, TRUE, AM_CV_TMPFAIL);
    RETVALUE(ROK);
  }
  else
  {
    /* FID 15068.0 */
    if (CC_IS_CALL_MULTI_MG(con) && CC_IS_CALL_FLAG2(con, CC_FLAG_PRECONDITION) &&
        (CC_GET_PROTOCOL(con->icProtType)==CC_SIPT || CC_GET_PROTOCOL(con->icProtType)==CC_SIP) &&
        (CC_GET_PROTOCOL(con->ogProtType)==CC_SI || CC_GET_PROTOCOL(con->ogProtType)==CC_IN) &&
        !IS_PRECND_COMPLETED(con->icPrecndCb->flag) &&
        (con->cotBlock.contResult==CONT_CHKSUCC))
    {
      if((con->ccmCb.flags&CC_IW_TDM)==0)
      {
        con->ccmCb.flags |= CC_IW_TDM;
        con->direction = OUTTOINC;
        con->ccmCb.state = CC_EGRESS_MG;
      }

      if (con->ccmCb.flags&CC_IW_TDM)
      {
        if (!CC_OPERATING_LAST_MG(con))
        {
          if ((con->ccmCb.ccn == CCN_5) && (con->ccmCb.state == CC_EGRESS_MG))
            con->ccmCb.state = CC_Y_MG;
          else
            con->ccmCb.state = CC_INGRESS_MG;

          ccProcessXferRsp1OnSwitchPassThru(con, CC_GET_PROTOCOL(con->icProtType),
                       CC_GET_PROTOCOL(con->ogProtType), con->ccEvntHold);
          CC_STATE_CHANGE(con, CCS_AWTMGCTCFMCOTOG1);
          return ROK;
        }else
        {
          con->ccmCb.flags &= ~CC_IW_TDM;
        }
      }
    }

    if((icProtType == CC_SIP||icProtType == CC_SIPT||icProtType == CC_BICC))
    {
      if(CC_IS_CALL_FLAG2(con,CC_FLAG_PRECONDITION) && 
         !IS_PRECND_COMPLETED(con->icPrecndCb->flag) &&
         (ogProtType==CC_SI) &&
         (con->cotBlock.contResult==CONT_CHKSUCC))
      {
        if (CC_OFFANS_ICRMTANS_PENDING(con))
        {
            CCDP(AIN_DBG_LEVEL_1, "%s: Reroute happened with incoming answer not received, release call!\n",__FUNCTION__);
            ccHandleCleanUpConCb(con, TRUE, CCPRECONDFAIL);
            RETVALUE(ROKIGNORE);
        }

        if (!CC_OFFANS_ICOFFANS_PENDING(con))
        {
          if((CC_IS_CALL_FLAG_SET(con->flag,CC_FLAG_IP_CALL_NO_INITIAL_SDP)) &&
             ccHasPrecndConfStatus(con->icPrecndCb))
          {
            ccEnablePrecndCb(con->icPrecndCb);
            ccTryToInsertPrecndConfStatus(con->icPrecndCb);
            ccSendUpdate(&con->icBcm);
            CC_OFFANS_STATE_CHANGE(con, CC_OFFANS_STATE_SENTOFFER,
                                           CC_OFFANS_STATE_NO_OP,
                                           CC_OFFANS_SUBSTATE_NULL,
                                           CC_OFFANS_TYPE_UPDATE);
          }
          CC_STATE_CHANGE(con, CCS_AWT_PRECONDITION);
          return ROK;
        }else if (CC_OFFANS_ICOFFER_PENDING(con))
        {
          Sdp* icLoc = NULLP;
          if(con && con->icIpParam.locDesc) 
          {
            CCDP(AIN_DBG_LEVEL_1, "%s: FID 15068 Sending 183 OFFER to incoming side\n",__FUNCTION__);

            icLoc = con->icIpParam.locDesc;
            ccUpdatePrecndStaTblIniNonPeer(con, con->icPrecndCb, PRECND_STRENGTH_MANDATORY, icLoc->numDecode);
            ccSend183ToInc(con);
            CC_OFFANS_STATE_CHANGE(con, CC_OFFANS_STATE_SENTOFFER,
                                           CC_OFFANS_STATE_NO_OP,
                                           CC_OFFANS_SUBSTATE_AWAITRMTANS_INVITENOSDP,
                                           CC_OFFANS_TYPE_REL18X_INVITENOSDP);

            CC_STATE_CHANGE(con, CCS_AWT_PRECONDITION);
            return ROK;
          }else
          {
            CCDP(AIN_DBG_LEVEL_1, "%s: FID 15068 incoming leg's sdp is NULL! \n",__FUNCTION__);
          }
        }
        else if(CC_OFFANS_ICANS_PENDING(con))
        {
          if (CC_OFFANS_OGOFFANS_STATE_NULL(con) && (IS_PRECND_COMPLETED(con->icPrecndCb->flag)))
          {
            CCDP(AIN_DBG_LEVEL_1, "%s: FID 15068 incoming precondition completed \n",__FUNCTION__);
          }
          else
          {
            U8 done = RFAILED; 
            ccEnablePrecndCb(con->icPrecndCb);
            done = ccTryToInsertPrecndConfStatus(con->icPrecndCb);
            ccSetPrecndLocalStatus(con->icPrecndCb, PRECND_DIR_SENDRECV);

            CCDP(AIN_DBG_LEVEL_1, "%s: FID 15068 Sending 183 ANSWER to incoming side\n",__FUNCTION__);

            ccSend183ToInc(con);
            CC_OFFANS_STATE_CHANGE(con, CC_OFFANS_STATE_NULL,
                                 CC_OFFANS_STATE_NO_OP,
                                 CC_OFFANS_GET_SUBSTATE(con),
                                 CC_OFFANS_METHOD_USED(con));
            if(done==ROK)  
            {
              CC_STATE_CHANGE(con, CCS_AWT_PRECONDITION);
              return ROK;
            }
            else
            {
              CCDP(AIN_DBG_LEVEL_1, "%s: FID 15068 Precondition has completed\n",__FUNCTION__);
              SET_PRECND_COMPLETED(con->icPrecndCb->flag);
              ccClearPrecndStatus(con->icPrecndCb);
            }

          }
        }
      }
    }

    if (con->cotBlock.contType == CC_IAM_INITIATED)
    {
      CC_STATE_CHANGE(con, CCS_AWTANS);
      direction = CC_OUTGOING;
    }
    else
    {
      CC_STATE_CHANGE(con, COT_OG_DONE);
      direction = CC_INCOMING;
    }
    if (con->cotBlock.contResult != CONT_CHKABORT)
        ccSendStaReq(con, CCE_CONTREP, con->cotBlock.contResult, direction);

    con->cotBlock.contResult = CONT_CHKABORT;
    RETVALUE(ROK);
  }
}


/*
*      Fun:   ccConE14S92
*
*      Desc:  Connection state function
*             event - Setup timer expiry
*             state - CCS_AWTMGCTCFMFORCOTOG1
*
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE14S92
(
CcConCb   *con,             /* connection */
PTR        event,           /* event */
AinBCM_t  *bcm,             /* bcm */
PTR        event2           /* event */
)
{
  CCLOGERR("ccConE14S92 : Setup timer expired in state = %d, icSu = 0x%lx\n",
           con->state, con->icSuConnId);
  ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
  ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
  RETVALUE(ROK);
}


/*
*      Fun:   ccConE61S09
*
*      Desc:  Connection state function
*             event - CCE_CRA
*             state - CCS_AWTANS
*
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE61S09
(
CcConCb   *con,             /* connection */
PTR        event,           /* event */
AinBCM_t  *bcm,             /* bcm */
PTR        event2           /* event */
)
{
  CcAddrReqEvnt          ccAddrReqEvnt;
  CCDP(AIN_DBG_LEVEL_1, "ccConE61S09  icSu = 0x%lx, ogSu = 0x%lx\n",
       con->icSuConnId, con->ogSuConnId);

  /* Feature 1789 - German ISUP */
  /* stop the Toiw2 Timer if running */
  ccStopConTmr(con, CC_TMR_TOIW2);

  if (ccMapEvent(con, CCE_CRA, 0) != ROK)
    CCLOGERR("ccConE61S09: Mapping failed 0x%lx\n", con->icSuConnId);

  ccSetDefaultAddrReq(con->icProtType, &ccAddrReqEvnt);
  ccUpdateAddrReq(con, &ccAddrReqEvnt);
  if (ccIsIPSubscriber(&(con->icBcm)))
    ccUpdateAddrReqForDMAPofIPSubscriber(con, &ccAddrReqEvnt);
  ccFillTgAttrForAddrReq(con, &ccAddrReqEvnt);
  CcLiCctAddrReq(&con->icSapCb->pst, con->icSapCb->spId,
                 con->icSpConnId, con->icSuConnId, con->icProtType,
                 &ccAddrReqEvnt);

  RETVALUE(ROK);
}


/*
*      Fun:   ccConE60S41
*
*      Desc:  Ignoring from FIC when GCC was waiting for a response after
*             sending OMidCall
*             event - CCE_IGNOREFROMFIC
*             state - CCS_AWTRSPFORDPE11
*
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE60S41
(
CcConCb   *con,             /* connection */
PTR        event,           /* event */
AinBCM_t  *bcm,             /* bcm */
PTR        event2           /* event */
)
{
    CCDP(AIN_DBG_LEVEL_1, "%s icSu:0x%lx, ogSu:0x%lx\n", __FUNCTION__,
         con->icSuConnId, con->ogSuConnId);

    CC_STATE_CHANGE(con, con->lastState);
    RETVALUE(ROK);
}




/*
 *
 *      Fun:   ccConE23S01
 *
 *      Desc:  Connection state function
 *             event - Continuity Report
 *             state - AWTROUTERSC
 *      Ret:   ROK     - successful,
 *             RFAILED - unsuccessful
 *
 *      Notes: None.
 *
 *      File:  cc_bdy2.c
 *
*/
PUBLIC S16 ccConE23S01
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
    /* This can happen when the resource is waiting to be dealocated who is
     * waitng for OG COT to be cleaned up. This is waitingCon(search on this) */

    ccStopConTmr(con, TMR_SETUP);
    ccStartConTmr(TMR_RELEASE, con, (PTR)&ccCp.genCfg);

    CC_STATE_CHANGE(con, CCS_AWTRELCFM_IC);
    ccSendRelease(con, con->icSapCb, con->icSpConnId, con->icSuConnId,
                  con->icProtType, CCTMPFAIL, NULLP);
    RETVALUE(ROK);
}


/*
*      Fun:   ccConE07S16
*
*      Desc:  Connection state function
*             event -  CCE_REATIND
*             state -  COT_OG_DONE
*
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE07S16
(
CcConCb   *con,             /* connection */
PTR       event,           /* event */
AinBCM_t  *bcm,             /* bcm */
PTR       event2           /* event */
)
{
  ccHandleCleanUpConCb(con, TRUE, CCCALLCLR);
  RETVALUE(ROK);
} /* ccConE07S16 */

/*
*      Fun:   ccConE07SXXBuf
*
*      Desc:  Connection state function
*             event -  CCE_REATIND
*             state -  
*
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/

PUBLIC S16 ccConE07SXXBuf
(
CcConCb   *con,             /* connection */
PTR       event,           /* event */
AinBCM_t  *bcm,             /* bcm */
PTR       event2           /* event */
)
{

  ccBufferEventToQueue(con, bcm, event, CCE_REATIND , sizeof(CcStaEvnt), 0,
                       NULLP, 0);

  RETVALUE(ROK);
}


/*
*      Fun:   ccConE07SBUF
*
*      Desc:  Connection state function
*             event -  CCE_REATIND
*             state -  In Outgoing COT States
*
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE07SBUF
(
CcConCb   *con,             /* connection */
PTR       event,           /* event */
AinBCM_t  *bcm,             /* bcm */
PTR       event2           /* event */
)
{
    con->cotBlock.contResult = CONT_CHKABORT;
    ccBufferEventToQueue(con, bcm, event, CCE_REATIND , sizeof(CcStaEvnt), 0,
                         NULLP, 0);
    RETVALUE(ROK);
}


/*
*      Fun:   ccConE07S30
*
*      Desc:  Connection state function
*             event -  CCE_REATIND
*             state -  CCS_AWTMGCTNOTIFY1
*
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE07S30
(
CcConCb   *con,             /* connection */
PTR       event,           /* event */
AinBCM_t  *bcm,             /* bcm */
PTR       event2           /* event */
)
{
    S16     ret  = RFAILED;
    U32     suCtxId = 0;       /* BUG:88040 which ContextId should we use, considring CCN_5. */

    /* Send a Subtract to the MG and then buffer the Re-attempt */

    con->cotBlock.contResult = CONT_CHKABORT;
    if (con->cotBlock.contType == CC_IAM_INITIATED)
    {
      suCtxId = con->ogBcm.mgCtx ? con->ogBcm.mgCtx->suCtxId : 0;
      ccUpdateMgQForSubRsc(&con->ogBcm, suCtxId, con);
    }
    else /* It has to be CC_CCR_INITIATED_OG */
    {
      suCtxId = con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0;
      ccUpdateMgQForSubRsc((PTR) NULLP, suCtxId, con);
    }
    ccChangeCallStateOfAssociatedCalls(&(con->icBcm), CCS_CANNOTPROCESSEVENTS);
    CC_STATE_CHANGE(con, CCS_AWTMGCTCFMCOTOG1);
    ret = ccSendMgiQueue(suCtxId, con->mgBlock);
    if (ret != ROK)
    {
      ABORT_DEBUG;
      CCLOGERR("Error: During switching icSu = 0x%lx\n", con->icSuConnId);
      cleanUpMgQ(con);
      ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
      RETVALUE(ROK);
    }

    ccBufferEventToQueue(con, bcm, event, CCE_REATIND , sizeof(CcStaEvnt), 0,
                       NULLP, 0);

    RETVALUE(ROK);
}


#ifdef ANSI
PUBLIC S16 ccConE39S49
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
#else
PUBLIC S16 ccConE39S49(con, event, bcm, event2)
CcConCb *con;                    /* connection */
PTR     event;                   /* event */
AinBCM_t *bcm;                    /* bcm */
PTR     event2;                   /* event */
#endif
{
  Continue_t        *pContinue      = NULLP;

  TRC3(ccConE39S49)

  pContinue   = (Continue_t *) event2;
  if (pContinue->errorCode != FC_NOT_APPLICABLE)
  {
    CCLOGERR("ERROR FROM FIC = %ld\n", pContinue->errorCode);
#ifdef CC_CALEA 
    if (con->icProtType != CC_LOCAL_SWT_PROT)
    {
       ccReleaseCaleaLegs(con);
       if (con->ogBcm.nel[EReqNetworkSignal] == NEL_NOTIFY)
       {
          /* This needed for sending NetworkSignal after playing Error Tone/Annoucement */
          con->ogBcm.keepTransactionOpen = TRUE;
       }

    }
    else if (con->icProtType == CC_LOCAL_SWT_PROT)
    {
       ccUpdateCloseCause(con, CC_BOTH, CscInternalFailure);
       ccRemoveCaleaAssnWithTappedBcm(con);
    }
#endif

    ccStopConTmr(con, AIN_O_NO_ANS_TMR);
    ccDeleteBufferedEvent(con, con->icSuConnId, CCE_ONOANSEXP);


    if(con->ogRscVal == TRUE)
    {
      ccDeallocateResource(con, CC_OUTGOING);
      con->ogRscVal = FALSE;
    }
    ccCloseTBcmTransaction(con);

#ifdef SND_NOT_IND
    /* Update termInd with exception for Termination Notification msg */
    con->ainInfo.termInd = EXCEPTION_IND;
#endif
    ccProcessFailCnd(con, pContinue->errorCode, TRUE);
    RETVALUE(ROK);
  }
  /* MAde changes for MGCP CALEA*/
  ccConE38S39(con, event, bcm, event2);
  RETVALUE(ROK);
}

/***********************************************************************************************
 *
 *      Fun:   ccConE29SXX
 *
 *      Desc:  Connection state function
 *             event - CC_TMR_AWAITANS Expiry
 *             state - AWTANS
 *      Ret:   ROK     - successful,
 *             RFAILED - unsuccessful
 *
 *      Notes: Awaiting Answer added for Answer Supervision Timer support for Telstra and 
 *             Mexico
 *
 *      File:  cc_bdy2_2.c
 *
*************************************************************************************************/

#ifdef ANSI
PUBLIC S16 ccConE29SXX
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
 )
#else
PUBLIC S16 ccConE29SXX(con, event, bcm, event2)
CcConCb *con;                    /* connection */
PTR     event;                   /* event */
AinBCM_t *bcm;                    /* bcm */
PTR     event2;                   /* event */
#endif
{
  S16                      ret            = RFAILED;
  CcConCb                 *CS1con         = NULLP;
  CcMidCallReqEvnt         midCallReq;
  Bool                     multiPartyCall = FALSE;
  ODisconnect_t            oDisconnect;
  SvcHeader_t              header;
  U8                       index          = 0;
  Bool                     notIndFlag     = FALSE; 

  CCDP(AIN_DBG_LEVEL_1, "ccConE29SXX: \n");
  CCLOGERR("Await Answer Timer Expired: %ld\n", con->icSuConnId);
  ccStopConTmr(con, CC_TMR_AWAITANS); 

  /* Feature 1789 - German ISUP */
  /* stop the Toiw2 Timer if running */
  ccStopConTmr(con, CC_TMR_TOIW2);
  /* BUG:84642 Fix the O_No_ANS_TMR issue */
  if(CC_IS_ICA_CALL(con))
  {
    ccStopConTmr(con, AIN_O_NO_ANS_TMR);
    ccDeleteBufferedEvent(con, con->icSuConnId, CCE_ONOANSEXP);
  }

  /* BUG: 62273 - jboyle - CDR Cause code not set for SIP originated calls */
  con->callDtlInfo.cm.relCause.causeVal.val = CCNOANSWR;

  /* bug82209 */
  if (con->icBcm.nel[EReqIDisconnect] == NEL_REQUEST)
  {
      ret=ccSendIDisconnect(&con->icBcm,FC_TMRRCVRY);
      if (ret != ROK)
      {
         CCDP(AIN_DBG_LEVEL_1,"IDisconnect failed to send to FIC\n");
         ABORT_DEBUG;
      }
      return(ROK);
  }

  if (con->icBcm.cc)
  {
    /* Bug# 18044  and Bug# 18131*/
    if (con->mgPlayingTone)
    {
       CCDP(AIN_DBG_LEVEL_0,
            "Busy Timer Expiry in MultiParty (TWC) call \n");
       if ((con->icBcm.nel[EReqODisconnect] == NEL_REQUEST) ||
           (con->icBcm.nel[EReqODisconnect] == NEL_NOTIFY))
       {
         if (con->icBcm.svcHeader.transId)
         {
           cmMemset((U8 *)&header, 0, sizeof(SvcHeader_t));
           cmMemset((U8 *)&oDisconnect, 0, sizeof(ODisconnect_t));
           /* Store legId */
           if (con->icBcm.cc)
           {
             if (con->icBcm.cc->ccId >= CiThreePartySetup)
               con->ainInfo.legId = 2;
             else
               con->ainInfo.legId = 1;
           }
           else
             con->ainInfo.legId = 1;

           /* Bug# 18104 */
           ccUpdateccId(&(con->icBcm.cc->ccId), con->ainInfo.legId);

           CCDP(AIN_DBG_LEVEL_2, "LegId disconnected:%d\n",
                              con->ainInfo.legId);
           ccFillSvcHeader(&header, &(con->icBcm), MSG_O_Disconnect);
           CCDP(AIN_DBG_LEVEL_0," Filled SvcHeader\n");

           notIndFlag = (con->icBcm.nel[EReqODisconnect] == NEL_REQUEST) ? 
                        FALSE: TRUE;

           ccFillODisconnectParam(&oDisconnect, &(con->icBcm), notIndFlag);
           CCDP(AIN_DBG_LEVEL_0," Filled O_Disconnect, TransID=%ld\n",
                header.spTransId);

           /* Bug# 11386 */
           if (con->icBcm.nel[EReqODisconnect] == NEL_REQUEST)
           {
              /* Cleaning up the NEL as per R5-241 GR1298 */
              for (index = 0; index < EReqMax; index++)
              {
                  con->icBcm.nel[index] = NEL_UNARMED;
              }

              /* Send the msg as it is not needed to be buffered */
              con->lastState = con->state;
              CC_STATE_CHANGE(con, CCS_AWTRSPFORDPE9);
              ccChangeCallStateOfAssociatedCalls(&(con->icBcm),
                                                 CCS_CANNOTPROCESSEVENTS);
              CcUiFctODisconnect(&oDisconnect, &header);
              RETVALUE(ROK);
           }
           else
           {
              CCDP(AIN_DBG_LEVEL_0, "Disconnect Notification:%d\n",
                   con->icBcm.nel[EReqODisconnect]);
              CcUiFctODisconnect(&oDisconnect, &header);
           }
         }
         else
         {
           CCLOGERR("Error: transId is zero:%ld,%ld\n", con->icSuConnId,
                                                      con->ogSuConnId);
         }
       }
       else
          CCLOGERR("BusyTimer expired and ODisconnect event is not armed\n");
    }

    if ((con->icBcm.cc->ccId >= CiThreePartySetup) && 
        (con->icBcm.cc->ccId <= CiRemoteTransferSetup))
      multiPartyCall = TRUE;
  }

  if (multiPartyCall)
  {
    ccHandleCleanUpConCb(con, TRUE, CCNOANSWR);
    RETVALUE(ROK);
  }

  /* Else Fall down and do the regular clean up */

  /* Initiate release in both directions */
  /* Initiate Deswitching */
  /* Release all connections */

  ccStartConTmr(TMR_RELEASE, con, (PTR)&ccCp.genCfg);

  /* Also Stop the CWT tone as we are going back to Two Party Call */
  ccBuildMidCalReqForMC_CANCEL_ALL(&midCallReq);

  /* Initiate De-switching */
  if (CC_IS_CONTEXT_PRESENT(con))
  {
    /* Blow off the switching and Release the Call */
    ccQSubBasedOnCcn(con);  /* FID 15261.0 */

    con->lastState = con->state;
    con->tempHolderForReleaseCause = CCNOANSWR;
    if (con->ogRscVal == FALSE)
    {
      if (con->ogBcm.cc)
      {
        if ((con->ogBcm.cc->ccId == CiCallWaiting) ||
            (con->ogBcm.cc->ccId == CiCallWaitingComplement))
        {
          CS1con = con->ogBcm.cc->legId02->con; 
          if (con->ogBcm.cc->legId02->bcmType == BtOBcm)
          {
            CcLiCctMidCallReq(&CS1con->icSapCb->pst, CS1con->icSapCb->spId,
                               CS1con->icSpConnId, CS1con->icSuConnId,
                               CS1con->icProtType, &midCallReq);
            CS1con->icBcm.cc->legId01 = &(CS1con->icBcm);
            CS1con->icBcm.cc->legId02 = NULLP;
            CS1con->icBcm.cc->legId2  = NULLP;
            CS1con->icBcm.cc->ccId    = CiStable2Party;
          }
          else if (con->ogBcm.cc->legId02->bcmType == BtTBcm)
          {
            CcLiCctMidCallReq(&CS1con->ogSapCb->pst,CS1con->ogSapCb->spId,
                               CS1con->ogSpConnId, CS1con->ogSuConnId,
                               CS1con->ogProtType, &midCallReq);
            CS1con->ogBcm.cc->legId01 = &(CS1con->ogBcm);
            CS1con->ogBcm.cc->legId02 = NULLP;
            CS1con->ogBcm.cc->legId2  = NULLP;
            CS1con->ogBcm.cc->ccId    = CiStable2Party;
          }
          else
          {
            CCLOGERR("Invalid bcmType: %d\n",
                                       con->ogBcm.cc->legId02->bcmType);
            RETVALUE(RFAILED);
          }

          con->ogBcm.cc             = NULLP;
          ccCloseTBcmTransaction (con);
        }
      }

      con->relPend = ICRELONLY;
      CC_STATE_CHANGE(con, CCS_AWTSWTCFM_SENDONEREL);
    }
    else if (CC_IS_ICA_CALL(con) && (con->icBcm.cc) && (con->icBcm.cc->legId02 == NULLP))
    {
      /*BUG:84642 This is the case where the first ICA call SCP->A call timed out */
      con->relPend = OGRELONLY;
      CC_STATE_CHANGE(con, CCS_AWTSWTCFM_SENDONEREL);
    }
    else
    {
      CC_STATE_CHANGE(con, CCS_AWTSWTCFM_SENDBOTHREL);
    }
    con->mgBlock->con = con;
    ret = ccSendMgiQueue(0, con->mgBlock);
    if (ret != ROK)
    {
      CCLOGERR( "ERROR occured \n");
      cleanUpMgQ(con);
      ccCleanUpMGCtx(con);
    }
    else
      RETVALUE(ROK);
  }
  else
  {
    CCDP(AIN_DBG_LEVEL_0, ": No Switching Context Found\n");
  }

  if (con->ogRscVal == FALSE)
  {
    /* Also Stop the CWT tone as we are going back to Two Party Call */
    if (con->ogBcm.cc)
    {
      if ((con->ogBcm.cc->ccId == CiCallWaiting) ||
          (con->ogBcm.cc->ccId == CiCallWaitingComplement))
      {
        CS1con = con->ogBcm.cc->legId02->con;
        if (con->ogBcm.cc->legId02->bcmType == BtOBcm)
        {
          CcLiCctMidCallReq(&CS1con->icSapCb->pst, CS1con->icSapCb->spId,
                             CS1con->icSpConnId, CS1con->icSuConnId,
                             CS1con->icProtType, &midCallReq);
          CS1con->icBcm.cc->legId01 = &(CS1con->icBcm);
          CS1con->icBcm.cc->legId02 = NULLP;
          CS1con->icBcm.cc->legId2  = NULLP;
          CS1con->icBcm.cc->ccId    = CiStable2Party;
        }
        else if (con->ogBcm.cc->legId02->bcmType == BtTBcm)
        {
          CcLiCctMidCallReq(&CS1con->ogSapCb->pst,CS1con->ogSapCb->spId,
                             CS1con->ogSpConnId, CS1con->ogSuConnId,
                             CS1con->ogProtType, &midCallReq);
          CS1con->ogBcm.cc->legId01 = &(CS1con->ogBcm);
          CS1con->ogBcm.cc->legId02 = NULLP;
          CS1con->ogBcm.cc->legId2  = NULLP;
          CS1con->ogBcm.cc->ccId    = CiStable2Party;
        }
        else
        {
          CCLOGERR("Invalid bcmType: %d\n", con->ogBcm.cc->legId02->bcmType);
          RETVALUE(RFAILED);
        }

        con->ogBcm.cc             = NULLP;
        ccCloseTBcmTransaction (con);
      }
    }
    else
    {
      CC_STATE_CHANGE(con, CCS_AWTRELCFM_IC);
      ccSendRelease(con, con->icSapCb, con->icSpConnId, con->icSuConnId,
                    con->icProtType, CCNOANSWR,NULL);
    }
  }
  else
  {
#ifdef CC_CALEA
    if (con->icProtType == CC_LOCAL_SWT_PROT)
    {
       ccUpdateCloseCause(con, CC_BOTH, CscRmtTscTimeout);
       ret = ccProcessTappedLegForCaleaRel(con, con->icBcm.cc->tappedLegId, 
                             event, CCE_TAWAITANSEXPIRY, 0 );
       RETVALUE(ROK);
    }
#endif

    /* Anyway go ahead and clean up the call */
    CC_STATE_CHANGE(con, CCS_AWTRELCFM_BOTH);
    ccSendRelease(con, con->icSapCb, con->icSpConnId, con->icSuConnId, 
                  con->icProtType, CCNOANSWR,NULL);
    ccSendRelease(con, con->ogSapCb, con->ogSpConnId, con->ogSuConnId, 
                  con->ogProtType, CCNOANSWR,NULL);
  }
  RETVALUE(ROK);
}
/* euysal - 03/17/2005 - CAMEL Support */
#if 1

/*
*      Fun:   ccConE75SXX
*
*      Desc:  Connection state function
*             event - CCE_RELEASECALL
*             state - CCS_ANSWERED
*
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
#ifdef ANSI
PUBLIC S16 ccConE75SXX
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
#else
PUBLIC S16 ccConE75SXX(con, event, bcm, event2)
CcConCb *con;                    /* connection */
PTR     event;                   /* event */
AinBCM_t *bcm;                    /* bcm */
PTR     event2;                   /* event */
#endif
{
   ReleaseCall_t            *pReleaseCall = NULLP;

   /* euysal - 08/18/2006 - GCC Support for Feature 2164 */
   if (con->state == CCS_AWTMGCTCFMFORMODIFY2) 
   {
      CCDP(AIN_DBG_LEVEL_1, "ccConE75SXX:icSu:%ld, ogSu:%ld, con->state:%d"  
          " Buffering Release Call from FIC until MG response is received\n",
           con->icSuConnId, con->ogSuConnId, con->state);

      ccBufferEventToQueue(con, bcm, event, CCE_RELEASECALL, sizeof(U8), 0,
                          NULLP, 0);
      RETVALUE(ROK);
   }
   /* Feature 1789 - German ISUP */
   /* stop the Toiw2 Timer if running */
   ccStopConTmr(con, CC_TMR_TOIW2);
   
   CCDP(AIN_DBG_LEVEL_1, "ccConE75SXX in STATE: %d --- icSu:0x%lx\n", con->state, con->icSuConnId);
   /* euysal - 06/20/2005 - BUG:38971 */  
   pReleaseCall = (ReleaseCall_t *) event2;

   CCDP(AIN_DBG_LEVEL_1, "Cause Value received in the ReleaseCall is %d for icSu:0x%lx\n", 
        pReleaseCall->relCause.causeVal.val, con->icSuConnId);
   CCDP(AIN_DBG_LEVEL_1, "relCause.eh.pres: %d --  relCause.causeVal.pres: %d --  for icSu:0x%lx\n", 
        pReleaseCall->relCause.eh.pres, pReleaseCall->relCause.causeVal.pres, con->icSuConnId); 
   CCDP(AIN_DBG_LEVEL_1, "protRelPrflId%d\n",  pReleaseCall->protRelPrflId);

   
   /* Bug80547 */
   if ((con->state == CCS_AWTRSPFORDPE9) &&
         (con->direction == INCTOOUT))
   {
      con->relPend = INRELRSPANDOUTREL;
      CCDP(AIN_DBG_LEVEL_1, "set relPend to INRELRSPANDOUTREL for ODisconnect\n");
   }

   /* FID16901.0 if there could be any ccFailCndId there */
   CcCause cause;
   cmMemset((U8 *)&cause, 0, sizeof(CcCause));

   cause.eh.pres = pReleaseCall->relCause.eh.pres;
   if(cause.eh.pres != NOTPRSNT)
   {
      cause.location.pres = pReleaseCall->relCause.location.pres;
      cause.location.val  = pReleaseCall->relCause.location.val;
      cause.cdeStand.pres = pReleaseCall->relCause.cdeStand.pres;
      cause.cdeStand.val  = pReleaseCall->relCause.cdeStand.val;
      cause.causeVal.pres = pReleaseCall->relCause.causeVal.pres;
      cause.causeVal.val  = pReleaseCall->relCause.causeVal.val;


      /* BUG92833 if no protRelPrflId, go through existing logic */
      if (pReleaseCall->protRelPrflId)
      {
          con->failCndVal = ccMapReleaseCallRel2FailCnd (con, &cause, pReleaseCall->protRelPrflId);
          if(con->failCndVal != FC_NOT_APPLICABLE)
          {
             ccProcessFailCnd(con, con->failCndVal, FALSE);
             RETVALUE(ROK);
          }
      }
   }
   /* release both legs here */
   if ((pReleaseCall != NULLP) &&
       (pReleaseCall->relCause.eh.pres != NOTPRSNT) && 
       (pReleaseCall->relCause.causeVal.pres != NOTPRSNT) && 
       (pReleaseCall->relCause.causeVal.val != 0))
   {   
      /* bug88826, update relCause in CDR */
      con->callDtlInfo.cm.relCause.eh.pres = PRSNT_NODEF; 
      con->callDtlInfo.cm.relCause.cdeStand  = pReleaseCall->relCause.cdeStand; 
      con->callDtlInfo.cm.relCause.location  = pReleaseCall->relCause.location; 
      con->callDtlInfo.cm.relCause.dgnVal  = pReleaseCall->relCause.dgnVal; 
      con->callDtlInfo.cm.relCause.causeVal  = pReleaseCall->relCause.causeVal;

      ccHandleCleanUpConCb(con, TRUE, pReleaseCall->relCause.causeVal.val);
      CCDP(AIN_DBG_LEVEL_1, "ccHandleCleanUpConCb called with cause: %d in ccConE75SXX\n",
           pReleaseCall->relCause.causeVal.val); 
   }   
   else
   {   
      /* bug72209 */
      if(con->tempHolderForReleaseCause == CCTMRRECOV)
      {
         ccHandleCleanUpConCb(con, TRUE, CCTMRRECOV);
         CCDP(AIN_DBG_LEVEL_1, "ccHandleCleanUpConCb called with cause: CCCALLCLR in ccConE75SXX\n");
      }
      else
      {
         ccHandleCleanUpConCb(con, TRUE, CCCALLCLR);
         CCDP(AIN_DBG_LEVEL_1, "ccHandleCleanUpConCb called with cause: CCCALLCLR in ccConE75SXX\n");
      }
   }   
  
  RETVALUE(ROK);
}

#endif

/*euysal - 04/21/2005 - Support for O_Answer request */
#if 1

/*
*
*      Fun:   ccConE37S95
*
*      Desc:  Connection state function
*             event - CCE_ANALYZE_ROUTE
*             state - 95-CCS_AWTRSPFORDPE08
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE37S95
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  SvcHeader_t        *arSvcHeader           = NULLP;
  AnalyzeRoute_t     *pAnalyzeRoute         = NULLP;
  S16                 mgret                 = RFAILED;
  U8                  evntType              = 0;

  if ((con == NULLP) || (event == NULLP))
  {
    CCLOGERR("Critical ERROR: Null pointers:%lx, %ld\n", (U32)con, event);
    RETVALUE(RFAILED);
  }

  /* If the AR is from the buffered queue it is not for S95, was buffered before 
   * we changed to S95, so buffer it again so it is processed in S10:BUG:65587 */
  if (con->bufferBlock.evntFromQ)
  {
    DP("%s: Bufered:icSu:%ld, ogSu:%ld, suCtxId:%ld\n", __FUNCTION__,
       con->icSuConnId, con->ogSuConnId, 
       (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0));

    ccBufferEventToQueue(con, bcm, event, CCE_ANALYZE_ROUTE, sizeof(SvcHeader_t),
                         con->icSuConnId, event2, sizeof(AnalyzeRoute_t));
    RETVALUE(ROK);
  }

  CCDP(AIN_DBG_LEVEL_1, "ccConE37S95: icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", con->icSuConnId, con->ogSuConnId,
       (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0), con->icBcm.svcHeader.transId);

  pAnalyzeRoute = (AnalyzeRoute_t *) event2;
  arSvcHeader = (SvcHeader_t *) event;
  
  ccCutBillingRecordIfNeeded(con, pAnalyzeRoute->scpResponse, OUTTOINC);
  ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
  /* CPDI need restore ConInd event before mapping AR routing info into it */
  ccRestoreEvntHold(con);
  ccExtractAnalyzeRteParam(pAnalyzeRoute, bcm);
  ccExtractSvcHeader(arSvcHeader, bcm, MSG_Analyze_Route);

  ccStopConTmr(con, AIN_O_NO_ANS_TMR);
  ccStopConTmr(con, AIN_T_NO_ANS_TMR);
  /* STOP the AWAIT ANSWER Timer if running
   *
   * changes made to Support Configurable Answer Timer in GCC, RV Suresh, 5.0 
   */
  ccStopConTmr(con, CC_TMR_AWAITANS); 
  ccDeleteBufferedEvent(con, con->icSuConnId, CCE_ONOANSEXP);
  ccDeleteBufferedEvent(con, con->ogSuConnId, CCE_TNOANSEXP);

  ccCleanAnsFraudCall(con);


  if (arSvcHeader->lastTransactionFlag == TRUE)
    ccDeleteBcm(&(con->icBcm));

  con->ogBcm.closeCause = CscRouteNotAvail;
  ccCloseTBcmTransaction(con);
  if ((CC_IS_CONTEXT_PRESENT(con)) && (con->ogRscVal))
  {
    ccQueueSubOnOGCleanUp(con);
    ccChangeCallStateOfAssociatedCalls(&(con->icBcm), CCS_CANNOTPROCESSEVENTS);
    /* CPDI_FIX: need to decide whether received an RelInd from outgoing before, or
       it's a case need initiate a release internally */
/*euysal - 04/21/2005 - Support for O_Answer request */
#if 1
    if (ccCheckRelOrOnHookInBuffer(&(con->ogBcm), &evntType) == ROK)
       con->relPend = OUTRELRSPANDROUTE;
    else   
       con->relPend = OUTRELANDROUTE;
#endif
    CCDP(AIN_DBG_LEVEL_1, "%s: icSu:%ld, ogSu:%ld, suCtxId:%ld,"
         " con->relPend:%d\n", __FUNCTION__, con->icSuConnId, con->ogSuConnId,
         con->mgBlock->mgCtx->suCtxId, con->relPend);
    
    CC_STATE_CHANGE(con, CCS_AWTSWTCFMFORDP);
    con->mgBlock->con = con;
    mgret = ccSendMgiQueue(0, con->mgBlock);
    if (mgret == ROK)
      RETVALUE(ROK);
    else
    {
      CCLOGERR("Error: MGI failed:icSu:%ld, ogSu:%ld, suCtxId:%ld,"
               " icTransId:%ld\n", con->icSuConnId, con->ogSuConnId,
               con->mgBlock->mgCtx->suCtxId, con->icBcm.svcHeader.transId);
      ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
      if (con->ogRscVal)
      {
        ccCloseTBcmTransaction(con);
        con->direction = OUTTOINC;
        if (con->noRspReqd == FALSE) ccSendReleaseRsp(con);
        ccDeallocateResource(con, CC_OUTGOING);
        con->ogRscVal = FALSE;
      }
      cleanUpMgQ(con);
      ccHandleCleanUpConCb(con, TRUE, AM_CV_TMPFAIL);
      RETVALUE(ROK);
    }
  }

  /* Clean outgoing  */
  if (con->ogRscVal)
  {
      ccFreeTheRscFromCon(con, OUTTOINC); /* This will take care of REL_CFM */
      initOgBcm(&con->ogBcm);
      if(ROK == ccCheckForInterfaces(con))
        ccRouteCall(con, FALSE);
      else
      {
        CC_TRACE_DUMP(con);
        ccProcessFailCnd(con, FC_INTERNAL, TRUE);
      }
     RETVALUE(ROK);
  }
  else
  {
      CCLOGERR("Error: Received O_Answer without Outgoing Resource:icSu:%ld, ogSu:%ld\n", con->icSuConnId, con->ogSuConnId);
      ABORT_DEBUG;
      ccHandleCleanUpConCb(con, TRUE, AM_CV_TMPFAIL);
      RETVALUE(ROK);
  }   
}


/*
*
*      Fun:   ccConE39S95
*
*      Desc:  Connection state function
*             event - CONTINUE
*             state - 95-CCS_AWTRSPFORDPE08
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE39S95
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  SvcHeader_t    *ctSvcHeader   = NULLP;
  Continue_t     *pContinue     = NULLP;
  S16             mgret         = RFAILED;
  S16             relRstRet     = ROK;
  SendApm_t       sendApm; /* FID 14234.0 */

  if ((con == NULLP) || (event == NULLP))
  {
    CCLOGERR("Critical ERROR: Null pointers:%lx, %ld\n", (U32)con, event);
    RETVALUE(RFAILED);
  }
  CCDP(AIN_DBG_LEVEL_1, "ccConE39S95:icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", con->icSuConnId, con->ogSuConnId,
       (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0), con->icBcm.svcHeader.transId);

  ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
  ccStopConTmr(con, AIN_O_NO_ANS_TMR);
  ccStopConTmr(con, AIN_T_NO_ANS_TMR);

  ccStopConTmr(con, CC_TMR_AWAITANS); 
  ccStopConTmr(con, TMR_SETUP);
  ccDeleteBufferedEvent(con, con->icSuConnId, CCE_ONOANSEXP);
  ccDeleteBufferedEvent(con, con->ogSuConnId, CCE_TNOANSEXP);

  ctSvcHeader = (SvcHeader_t *) event;
  pContinue   = (Continue_t *) event2;
  
  /* Begin FID 14234.0 */
  if((pContinue) && FCT_IS_CONTACT_SEND_APM(pContinue->action))
  {
      CCDP(AIN_DBG_LEVEL_1, "Populate one SENDAPM event and send out.\n");
      cmMemset((U8 *)&sendApm, 0, sizeof(SendApm_t));

      cmMemcpy((U8 *)&sendApm.app, (U8 *)&pContinue->app, sizeof(SiAppTrans));
      sendApm.direction = pContinue->apmDirection;

      ccSendApmReq(con, &sendApm);
  }
  /* End of FID 14234.0 */

  if(pContinue->errorCode != FC_NOT_APPLICABLE)
  {
     CCLOGERR("Received error in Continue from FIC: ErrorCode:%ld,icSu:%ld,ogSu:%ld,icTrsId:%ld\n",
              pContinue->errorCode, con->icSuConnId, con->ogSuConnId, 
              (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0));

     /* CPDI_FIX 28351, 28730, need good way handling relrsp */
     relRstRet = ccRestoreReleaseInEvntHold(con);

     con->tempHolderForReleaseCause = pContinue->errorCode;
     if  (con->icProtType == CC_LOCAL_SWT_PROT)
     {
         ccRemoveCaleaAssnWithTappedBcm(con);
     }

     con->ainInfo.termInd = EXCEPTION_IND;
     con->ogBcm.closeCause = CscRmtTscTimeout;
     ccCloseTBcmTransaction(con);

     con->failCndVal = pContinue->errorCode;
     if (CC_IS_CONTEXT_PRESENT(con))
     {
#if 1 /* EARLY_ACM */
         /* Need to clean all the legs in the context */
        CC_RESET_ANY_EARLY_ACM_FLAGS(con);
#endif

        ccQueueSubOnOGCleanUp(con);

        /* 28730 handle it differently */
        if (relRstRet == ROK)
            con->relPend = OUTRELRSPANDINFAILCOND;
        else
            con->relPend = OUTRELANDINFAILCOND;

        CC_STATE_CHANGE(con, CCS_AWTSWTCFM_SENDONEREL);
        con->mgBlock->con = con;
        mgret = ccSendMgiQueue(0, con->mgBlock);
        if (mgret == ROK)
            RETVALUE(ROK);
        else
        {
            CCLOGERR("Error: MGI failed:icSu:%ld, ogSu:%ld, suCtxId:%ld,"
                     " icTransId:%ld\n", con->icSuConnId, con->ogSuConnId,
                     con->mgBlock->mgCtx->suCtxId, con->icBcm.svcHeader.transId);
            cleanUpMgQ(con);
         }
     }

     if (con->ogRscVal)
     {
         con->direction = OUTTOINC;
         if (relRstRet == ROK)
         {
             ccSendReleaseRsp(con);
             ccDeallocateResource(con, CC_OUTGOING);
             con->ogRscVal = FALSE;
         }
         else /* 28730: there is nothing to respond, just request */
         {
             ccFreeTheRscFromCon(con, OUTTOINC); /* This will take care of REL_CFM */
         }
     }

     ccProcessFailCnd(con, con->failCndVal, TRUE);
     RETVALUE(ROK);
  }
  else
  { 
     ccAnswerCall(con, (PTR)con->ccEvntHold, NULLP);
  }

  RETVALUE(ROK);
}
/*
*
*      Fun:   ccConE40S95
*
*      Desc:  Connection state function
*             event - Disconnect Leg from FIC.
*             state - CCS_AWTRSPFORDPE08
*
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE40S95
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
    Bool               relDisBoolIC          = FALSE;
    U8                 evntType              = 0;

    CCDP(AIN_DBG_LEVEL_1, "ccConE40S95:0x%lx, 0x%lx, 0x%lx\n", con->icSuConnId,
                          con->ogSuConnId, (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0));
    ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));

    /* Send Notify to transferer */
    ccNotifyReferingLeg(con, INCTOOUT, CC_RMT_XFER_FINAL, SIP_200_OK);

    relDisBoolIC = ccCheckRelOrOnHookInBuffer(&(con->icBcm), &evntType);
    if (con->icBcm.cc == NULLP)
    {
        CCLOGERR("Error: cc Cannot be nullp icSu = 0x%lx\n", con->icSuConnId);
        ccHandleCleanUpConCb(con, TRUE, AM_CV_TMPFAIL);
    }
    else
    {
        /* bug 52785 */
        if (con->icBcm.cc->ccId == CiRemoteTransferSetup )
        {
           CcConCb *CSCon1 = con->icBcm.cc->legId02->con;
           if( CC_IS_CALL_FLAG_SET(CSCon1->flag, CC_FLAG_HOLDSENT))
           {
              if( ((con->icBcm.cc->legId02->bcmType == BtOBcm) 
                        && ((CC_GET_PROTOCOL(CSCon1->ogProtType) == CC_SI) ||
                           (CC_GET_PROTOCOL(CSCon1->ogProtType) == CC_EXT_BICC))
                        && (CC_GET_PROTOCOL(CSCon1->icProtType) == CC_SIP))
                    ||
                    ((con->icBcm.cc->legId02->bcmType == BtTBcm) 
                        && ((CC_GET_PROTOCOL(CSCon1->icProtType) == CC_SI) ||
                           (CC_GET_PROTOCOL(CSCon1->icProtType) == CC_EXT_BICC))
                        && (CC_GET_PROTOCOL(CSCon1->ogProtType) == CC_SIP)) )
              {
                 CcAllSdus          ogEvnt;
                 SiCnStEvnt  *si;                        
                 SiNotifInd *notifInd = NULLP;         
                         
                 cmMemset((U8*) &ogEvnt, 0, sizeof(CcAllSdus));
                 CSCon1->ogEvnt = (CcAllSdus *)&ogEvnt;
                 si  = &(ogEvnt.m.ccCnStEvnt.m.siCnStEvnt);
 
                 INITEVENT(si, sizeof(SiCnStEvnt)); 
                 CSCon1->ogEvntType = CC_ET_CPG;
 
                 si->evntInfo.eh.pres = PRSNT_NODEF;
                 si->evntInfo.evntInd.pres = PRSNT_NODEF;
                 si->evntInfo.evntInd.val = EV_PROGRESS;
                 si->evntInfo.evntPresResInd.pres = PRSNT_NODEF;
                 si->evntInfo.evntPresResInd.val = EVPR_PRESRES;  
                               
                 if(con->icBcm.cc->legId02->bcmType == BtOBcm)
                 {
                    /* FID 16747.0 */
                    if ((CSCon1->ogProtType == CC_SIANS92) ||(CSCon1->ogProtType == CC_BICCANS92) ||
                        CC_IS_PROT_TYPE_ANSI_BICC(con->ogIntfcCb))
                    {
                       notifInd = &(si->notifInd);
                    }
                    else
                    {
                       notifInd = &(si->notifInd1);
                    }
                    notifInd->eh.pres = PRSNT_NODEF;
                    notifInd->notifInd.pres = PRSNT_NODEF;
                    notifInd->notifInd.val = CC_RMTERETR;
 
                    ccSendCnStReq(CSCon1, CSCon1->ogSapCb->suId, &CSCon1->ogSapCb->pst,
                            CSCon1->ogSapCb->spId, CSCon1->ogSpConnId, CSCon1->ogSuConnId,
                            CSCon1->ogProtType, CSCon1->ogEvntType, &(ogEvnt.m.ccCnStEvnt), NULLP);
 
                 }
                 else
                 {
                    /* FID 16747.0 */
                    if ((CSCon1->icProtType == CC_SIANS92) ||(CSCon1->icProtType == CC_BICCANS92) ||
                        CC_IS_PROT_TYPE_ANSI_BICC(con->icIntfcCb))
                    {
                       notifInd = &(si->notifInd);
                    }
                    else
                    {
                       notifInd = &(si->notifInd1);
                    }
                    notifInd->eh.pres = PRSNT_NODEF;
                    notifInd->notifInd.pres = PRSNT_NODEF;
                    notifInd->notifInd.val = CC_RMTERETR;
 
                    ccSendCnStReq(CSCon1, CSCon1->icSapCb->suId, &CSCon1->icSapCb->pst,
                            CSCon1->icSapCb->spId, CSCon1->icSpConnId, CSCon1->icSuConnId,
                            CSCon1->icProtType, CSCon1->ogEvntType, &(ogEvnt.m.ccCnStEvnt), NULLP);
 
                 }
              }
              CC_CLR_CALL_FLAG(CSCon1->flag, CC_FLAG_HOLDSENT);
           }
        }
        ccAnswerCall(con,  (PTR)con->ccEvntHold, NULLP);

        /* Bug 58552 */
        if(!ccBillingSystem.genStartRecord)
        {
           ccGenACRBillingWrapper(con, C_CDRGEN_CAUSE_INTERIM);
        }

        if (relDisBoolIC == ROK)
        {
            /* bug 52387 */
            ccDeleteBufferedEvent(con, con->icSuConnId, CCE_RELIND);
            con->direction = INCTOOUT;
            ccSendReleaseRsp(con);
            ccProcessTransfer(con, con->icBcm.cc->legId02->con, &con->icBcm);
        }
        else
        {
          CcConCb  *newCon = NULLP;
#if 0  /* Bugs:31446,31661.*/
            ccChangeCallStateOfAssociatedCalls(&(con->icBcm),
                                               CCS_CANNOTPROCESSEVENTS);
            CC_STATE_CHANGE(con, CCS_AWTRELCFM_KEEP_CON);
            ccSendRelease(con, con->icSapCb, con->icSpConnId, con->icSuConnId,
                          con->icProtType, CCCALLCLR, NULL);
#else /* Bugs:31446,31661.*/
           /* Removed icBcm from "con" and run a half-call Bcm to track Layer-3
            * response and RM-state */
           /* This should allow the service to proceed without having to 
            * wait for CctRelCfm (and likes). */

            newCon = ccDisconnectBcmFromCon(con, INCTOOUT);
            if (!newCon)
            {
              ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
              RETVALUE(RFAILED);
            }
            ccProcessTransfer(con, con->icBcm.cc->legId02->con, &con->icBcm);

            /* Send RelReq to icBcm of newCon */
            CC_STATE_CHANGE(newCon, CCS_AWTRELCFM_IC);
            newCon->acnt = FALSE; /* Don't want to bill this call */
            ccStartConTmr(TMR_RELEASE, newCon, (PTR)&ccCp.genCfg);
            ccSendRelease(newCon, newCon->icSapCb, newCon->icSpConnId,
                newCon->icSuConnId, newCon->icProtType, CCCALLCLR, NULL);
#endif /* 0 */
        }
    }
    RETVALUE(ROK);
}



/* 
*  
*      Fun:   ccConE71S95
*  
*      Desc:  Connection state function
*             event - Connect_To_Resource
*             state - 95-CCS_AWTRSPFORDPE08
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
* 
*      Notes: None.
*   
*      File:  cc_bdy2_2.c
*   
*/  
PUBLIC S16 ccConE71S95
(   
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)   
{
  S16 ret = ROK;

  CCDP(AIN_DBG_LEVEL_1, "ccConE71S95:icSu:%ld, ogSu:%ld, icProt:%d\n", con->icSuConnId,
       con->ogSuConnId, con->icProtType);
  
  /* euysal - 02/14/2006 - Send CTRClear with Error Code ClcFailure in case of failure */
  con->lastState = con->state;
  ret = ccProcessConnectToResource(&(con->icBcm));

  if (ret !=ROK)
  {
     CCDP(AIN_DBG_LEVEL_0, "FAILED to Play CPDI Annc thru MGI for annc-conf facility\n");
     ccHandleCleanUpConCb(con, TRUE, AM_CV_TMPFAIL);
     RETVALUE(RFAILED);
  }
  else
  {
     CCDP(AIN_DBG_LEVEL_0, "Played CPDI Annc thru MGI for annc-conf facility\n");
  }
  
  RETVALUE(ROK);
}   
#endif /* euysal */

/***********************************************************************************************
 *
 *      Fun:   ccConE02S96
 *
 *      Desc:  Connection state function
 *             event - CCE_ADDRIND
 *             state - AWTMOREINFO
 *      Ret:   ROK     - successful,
 *             RFAILED - unsuccessful
 *
 *      Notes: Awaiting SAM message for routing.
 *
 *      Author: Kevin.D.xie
 *
 *      File:  cc_bdy2_2.c
 *
*************************************************************************************************/

#ifdef ANSI
PUBLIC S16 ccConE02S96
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                   /* bcm */
PTR     event2                   /* event */
 )
#else
PUBLIC S16 ccConE02S96(con, event, bcm, event2)
CcConCb *con;                    /* connection */
PTR     event;                   /* event */
AinBCM_t *bcm;                   /* bcm */
PTR     event2;                  /* event */
#endif
{
    SiCnStEvnt    *siInc;
    CnStEvnt      *inInc;
    ProtType      icProtType = CC_GET_PROTOCOL(con->icProtType);
    /* BUG:56154 */
    SiCdPtyNum    siCdNum;
    U8 i=0;
    /* BUG:56154 */
    Bool          noMoreSAM = FALSE;
    U8            cdLen = 0;
    SiCdPtyNum   *cdNum = NULLP;
    S16           ret   = RFAILED;

    TRC3(ccConE02S96)

    /* map connection status event */
    con->icEvnt = (CcAllSdus *)event;

    /* Update the con->waitForMoreInfo */
    if( icProtType == CC_IN)
    {
        inInc = &con->icEvnt->m.ccCnStEvnt.m.inCnStEvnt;

        /* 2111 ISDN overlap receiving */
        CCDP(AIN_DBG_LEVEL_0, "CCE_ADDRIND: con->waitForMoreInfo:0x%02x, waitForMoreInfo from ISDN:0x%02x\n",
                               con->waitForMoreInfo, inInc->optElmt.waitForMoreInfo);
        CC_SET_IN_WAITMORE_INFO(inInc->optElmt.waitForMoreInfo);

        if( (ccAddSubsqCalledDigits(con) == ROK) || (inInc->sndCmplt.eh.pres == PRSNT_NODEF) )
        {
            CCDP(AIN_DBG_LEVEL_0, "Kavon Tag 1\n");
            /* FID17555.0 Stop playing dialing tone first Kavon */
            if(CC_IS_CONTEXT_PRESENT(con))
            {
                CCDP(AIN_DBG_LEVEL_0, "Kavon Tag 2\n");
                con->lastState = con->state;
                ccUpdateMgQForSubRsc(&con->icBcm, con->icBcm.mgCtx->suCtxId, con);
                CC_STATE_CHANGE(con, CCS_AWTSWTCFM_FOR_DIALINGTONE);
                con->mgBlock->con = con;
                ret = ccSendMgiQueue(con->mgBlock->mgCtx->suCtxId, con->mgBlock);
                if (ret != ROK)
                {
                    CCLOGERR("ERROR: ccSendMgiQueue failed\n");
                    cleanUpMgQ(con);
                    ccCleanUpMGCtx(con);
                    ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
                    RETVALUE(ROK);
                }
            }
            else
            {
                if( con->waitForMoreInfo & 0x02 )
                {
                    RETVALUE(ROK);
                }
                else
                {
                    con->STDigitRecd = TRUE;
                    ccNoMoreInfoAndContinue(con);
                }
            }
        }
    }
    /* FID 15319.2 */
    else if((icProtType == CC_SI) || (icProtType == CC_EXT_BICC))
    {
        siInc = &con->icEvnt->m.ccCnStEvnt.m.siCnStEvnt;

        CCDP(AIN_DBG_LEVEL_0, "CCE_ADDRIND: con->waitForMoreInfo:0x%02x, waitForMoreInfo from ISUP:0x%02x\n",
                                con->waitForMoreInfo, siInc->optElmnt.isup.waitForMoreInfo);
#if 0
        CC_SET_WAITMORE_INFO(siInc->optElmnt.isup.waitForMoreInfo);
#endif

        /* clear the buffer bit for SAM msg */
        CC_CLEAR_SAMBUFFERED(con);

        /* FID 15174.0 */
        CC_DISCARD_SAM(con);

        ccStopConTmr(con, CC_TMR_MINDIGIT);
        ccStopConTmr(con, CC_TMR_MAXDIGIT);

        if(ccAddSubsqCalledDigits(con) == ROK)
        {
            CCDP(AIN_DBG_LEVEL_0, "After clear the SAM buffer bit, con->waitForMoreInfo:0x%02x\n",
                                con->waitForMoreInfo);
            /* BUG:56154 */
            if(con->ccEvntHold != NULLP)
            {
                siCdNum = con->ccEvntHold->m.ccConEvnt.m.siConEvnt.cdPtyNum;
                CCDP(AIN_DBG_LEVEL_3,"siCdNum.oddEven.val = %d\n", siCdNum.oddEven.val);
                for (i=0;i<siCdNum.addrSig.len;i++)
                {
                    CCDP(AIN_DBG_LEVEL_3,"siCdNum->addrSig.val[%d] = %x\n",
                        i,siCdNum.addrSig.val[i]);
                }
                con->STDigitRecd = CC_STDIGIT_PRESENT(siCdNum);
                CCDP(AIN_DBG_LEVEL_3, "con->STDigitRecd:%d\n",con->STDigitRecd);
            }

            /* FID 15174.0 */
            noMoreSAM = ccReadyToRoute(con);
            if(!noMoreSAM)
            {
                cdNum = &(con->ccEvntHold->m.ccConEvnt.m.siConEvnt.cdPtyNum);
                cdLen = CC_FIND_NMB_DIGITS(cdNum);
                /* FID 15319.2 */ /*FID 16696.0 */
                if((ccGenPrfl.overlapToEnbloc == TRUE)||
                   (CC_IS_OVERLAPTOENBLOC_REQED(con)) ||
                   (CC_IS_OGOVERLAP_DISABLED(con)) ||
                   ((icProtType == CC_EXT_BICC) && (con->ogIntfcCb)))
                {
                    CC_CHECK_SI_MAXDIGITS(con, cdLen);
                }
                else
                {
                    CC_CHECK_SI_MINDIGITS(con, cdLen);
                }
            }
            CC_CLEAR_AWAIT_SAM(con);

            /* BUG:56154 */
            if(CC_IS_WAITMORE_INFO(con->waitForMoreInfo))
            {
                RETVALUE(ROK);
            }
            else
            {
                ccNoMoreInfoAndContinue(con);
            }
        }
    }

    RETVALUE(ROK);
} /* End of ccConE02S96 */

/***********************************************************************************************
 *
 *      Fun:   ccConE03S96
 *
 *      Desc:  Connection state function
 *             event - CCE_CNST_IND
 *             state - AWTINFO
 *      Ret:   ROK     - successful,
 *             RFAILED - unsuccessful
 *
 *      Notes: Awaiting information requested to lower layer
 *
 *      File:  cc_bdy2_2.c
 *
*************************************************************************************************/

#ifdef ANSI
PUBLIC S16 ccConE03S96
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                   /* bcm */
PTR     event2                   /* event */
 )
#else
PUBLIC S16 ccConE03S96(con, event, bcm, event2)
CcConCb *con;                    /* connection */
PTR     event;                   /* event */
AinBCM_t *bcm;                   /* bcm */
PTR     event2;                  /* event */
#endif
{
    SiCnStEvnt *siInc;
    SipCnStEvnt *sipInc = NULLP; /* FID 15174.0 */
    SiCdPtyNum  *ccCdNum = NULLP;
    U8          i=0;
    U8          cdLen = 0;

    TRC3(ccConE03S96)
    
    if (con->icEvntType == CC_ET_INFO)
    {
      /* If INFO message is received, check if is solicited information 
       * and update the Con Event accordingly */

      /* map connection status event */
      con->icEvnt = (CcAllSdus *)event;
      siInc = &(con->icEvnt->m.ccCnStEvnt.m.siCnStEvnt);

      CCDP(AIN_DBG_LEVEL_0, "INF msg reaches GCC with con->waitForMoreInfo=0x%02x, waitForMoreInfo from ISUP:0x%02x\n",
           con->waitForMoreInfo, siInc->optElmnt.isup.waitForMoreInfo);

      /* Update the con->waitForMoreInfo */
      CC_SET_WAITMORE_INFO(siInc->optElmnt.isup.waitForMoreInfo);

      /* clear the buffer bit for SAM event */
      CC_CLEAR_INFBUFFERED(con);
      
#if 0
      if(CC_UNSOLICITED_INFO(siInc->infoInd) == FALSE)
      {
#endif
      CCDP(AIN_DBG_LEVEL_0, "After clear INF buffer bit, con->waitForMoreInfo=0x%02x\n",
              con->waitForMoreInfo);

         /* If solicited information is received, check for call reference and
          * other information in CnStEvent */
         if (con->callRefA.eh.pres == NOTPRSNT)
         {
            ccExtractCallReferenceFromInfo(con, event);
         }
         else
         {
           CCLOGERR("Error: ccConE03S96: Call Reference sent in IAM and INFO. Going with IAM icSu:%ld\n",
                     con->icSuConnId);
         }
         
         /* copy information from the received event */
         ccExtractInfoFromCnStEvnt(con, event);

#if 0
         CCFILLCONTTYPE(con);

         /* Request for alocation/verification of the incoming resource */
         CC_STATE_CHANGE(con, CCS_AWTROUTERSC);
         if ((!CC_IS_PROT_TYPE_G500V0(con->icProtType) && (con->routeInfo->cgCtgy == CC_CAT_TEST)) ||
             (CC_IS_PROT_TYPE_G500V0(con->icProtType) && (con->routeInfo->cgCtgy == CC_CAT_TESTCALL)))
         {
           ccAllocateResource(con, (CcConEvnt *)event, CC_INCOMING,
                              RMT_ALOC_TEST_CALL);
         }
         else
         {
           ccAllocateResource(con, (CcConEvnt *)event, CC_INCOMING, RMT_DONTCARE);
         }
      }
      else
      {
           CCLOGERR("Error: ccConE03S96: unsolicited Information received, ignoring. dumped icSu:%ld\n", con->icSuConnId);
      }
#endif
      /* FID 15174.0 */
      if(!CC_IS_WAITMORE_INFO(con->waitForMoreInfo))
      {
         ccNoMoreInfoAndContinue(con);
      }
    }
    else if(con->icEvntType == CC_ET_NMINFO)
    {
	/* Bug:47300 Maximum digit timer expired in protocol layer */
        if(con->waitForMoreInfo & 0x02)
        {
            con->maxDgtsTmrExp = TRUE;
        }
	/* 2111 -ISDN overlap receiving */
        if(CC_GET_PROTOCOL(con->icProtType) == CC_IN)
        {
             con->STDigitRecd = TRUE;
        }
        /* Bug:47300 */
        ccNoMoreInfoAndContinue(con);
    }
    /* FID 15174.0 SIP enbloc overlap */
    else if((con->icEvntType == CCSIP_ET_INFO_OVERLAP) && (con->direction == INCTOOUT))
    {
        CCDP(AIN_DBG_LEVEL_0, "CCE_CNSTIND: get CCSIP_ET_INFO_OVERLAP\n");

        if (!CC_IS_ENBLOC_OVERLAP(con))
        {
            CCDP(AIN_DBG_LEVEL_0, "Not a ebloc overlap, ignore CCSIP_ET_INFO_OVERLAP\n");
            return(ROK);
        }

        /* map connection status event */
        con->icEvnt = (CcAllSdus *)event;
        sipInc = &(con->icEvnt->m.ccCnStEvnt.m.sipCnStEvnt);

        ccStopConTmr(con, CC_TMR_MAXDIGIT);

        if(ccAddSIPSubsqCalledDigits(con, sipInc) == ROK)
        {
            if(con->ccEvntHold != NULLP)
            {
                ccCdNum = &(con->ccEvntHold->m.ccConEvnt.m.sipConEvnt.cdPtyNum);
                CCDP(AIN_DBG_LEVEL_3,"ccCdNum->oddEven.val = %d\n",
                     ccCdNum->oddEven.val);
                for (i=0;i<ccCdNum->addrSig.len;i++)
                {
                    CCDP(AIN_DBG_LEVEL_3,"ccCdNum->addrSig.val[%d] = %x\n",
                        i,ccCdNum->addrSig.val[i]);
                }
                cdLen = CC_FIND_NMB_DIGITS(ccCdNum);
                CC_CHECK_SIP_MAXDIGITS(con, cdLen);
            }
        }
        else
        {
            CCDP(AIN_DBG_LEVEL_0, " Failed to ccAddSIPSubsqCalledDigits, ignore it and restart CC_TMR_MAXDIGIT\n");
            ccStartConTmr(CC_TMR_MAXDIGIT, con, (PTR)&ccCp.genCfg);
            RETVALUE(RFAILED);
        }
    }
    /* FID 15174.0 SIPT enbloc overlap */
    else if((con->icEvntType == CCSIP_ET_MULTIINVITE) && (con->direction == INCTOOUT))
    {
        CCDP(AIN_DBG_LEVEL_0, "CCE_CNSTIND: get CCSIP_ET_MULTIINVITE\n");

        if (!CC_IS_ENBLOC_OVERLAP(con))
        {
            CCDP(AIN_DBG_LEVEL_0, "Not a ebloc overlap, ignore CCSIP_ET_MULTIINVITE\n");
            return(ROK);
        }

        /* map connection status event */
        con->icEvnt = (CcAllSdus *)event;
        siInc = &(con->icEvnt->m.ccCnStEvnt.m.siCnStEvnt);

        /* FID 15903.0 + */
        con->ccEvntHold->m.ccConEvnt.m.siConEvnt.optElmnt.sipt.ips_addr  =  ((CcCnStEvnt *)con->icEvnt)->m.siCnStEvnt.optElmnt.sipt.ips_addr;
        con->ccEvntHold->m.ccConEvnt.m.siConEvnt.optElmnt.sipt.sessionId  =  con->icSpConnId;
        /* FID 15903.0 - */

        ccStopConTmr(con, CC_TMR_MAXDIGIT);

        if(ccAddSIPTSubsqCalledDigits(con, siInc) == ROK)
        {
            if(con->ccEvntHold != NULLP)
            {
                ccCdNum = &(con->ccEvntHold->m.ccConEvnt.m.siConEvnt.cdPtyNum);
                CCDP(AIN_DBG_LEVEL_3,"ccCdNum->oddEven.val = %d\n",
                     ccCdNum->oddEven.val);
                for (i=0;i<ccCdNum->addrSig.len;i++)
                {
                    CCDP(AIN_DBG_LEVEL_3,"ccCdNum->addrSig.val[%d] = %x\n",
                        i,ccCdNum->addrSig.val[i]);
                }
                cdLen = CC_FIND_NMB_DIGITS(ccCdNum);
                CC_CHECK_SIP_MAXDIGITS(con, cdLen);
            }
        }
        else
        {
            CCDP(AIN_DBG_LEVEL_0, " Failed to ccAddSIPSubsqCalledDigits, ignore it and restart CC_TMR_MAXDIGIT\n");
            ccStartConTmr(CC_TMR_MAXDIGIT, con, (PTR)&ccCp.genCfg);
            RETVALUE(RFAILED);
        }
    }
    else
    {
     /* Bug: 31931 - Buffer E03 in this case  - Required for
      * Transiting Unrecognised message in International
      * RV Suresh, 5.0, International 
      */
       ccBufferEventToQueue(con, bcm, event, CCE_CNSTIND, sizeof(CcCnStEvnt), 0,
                       NULLP, 0);
    }
   
    RETVALUE(ROK);
}

/***********************************************************************************************
 *
 *      Fun:   ccConE04S96
 *
 *      Desc:  Connection state function
 *             event - Release Indication
 *             state - CCS_AWTMOREINFO
 *      Ret:   ROK     - successful,
 *             RFAILED - unsuccessful
 *
 *      Notes: None
 *
 *      File:  cc_bdy2_2.c
 *
*************************************************************************************************/

#ifdef ANSI
PUBLIC S16 ccConE04S96
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                   /* bcm */
PTR     event2                   /* event */
 )
#else
PUBLIC S16 ccConE04S96(con, event, bcm, event2)
CcConCb *con;                    /* connection */
PTR     event;                   /* event */
AinBCM_t *bcm;                   /* bcm */
PTR     event2;                  /* event */
#endif
{
  S16      ret   = RFAILED;

  CCDP(AIN_DBG_LEVEL_1, "ccConE04S96:icSu 0x%lx ogSu 0x%lx, con->waitForMoreInfo 0x%02x\n",
       con->icSuConnId, con->ogSuConnId, con->waitForMoreInfo);

  /* Wait for resource confirmation to clear the call */
  ccStopConTmr(con, TMR_SETUP);

  /* FID 15174.0 */
  ccStopConTmr(con, CC_TMR_MAXDIGIT);
  ccStopConTmr(con, CC_TMR_MINDIGIT);

  if (CC_IS_CONTEXT_PRESENT(con))
  {
    ccQSubBasedOnCcn(con);  /* FID 15261.0 */

    con->relPend = INRELRSPANDNOREL;
    con->direction = INCTOOUT;
    CC_STATE_CHANGE(con, CCS_AWTSWTCFM_SENDNOREL);
    con->mgBlock->con = con;
    ret = ccSendMgiQueue(0, con->mgBlock);
    if (ret == ROK)
      RETVALUE(ROK);
  }

  if (ret != ROK)
  {
    CCDP(AIN_DBG_LEVEL_0, "ccUpdateMgQForSubRsc not sent:icSu:0x%lx, ctxId:0x%lx, ret:%d, con->waitForMoreInfo:0x%02x\n",
         con->icSuConnId, (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0), ret,
         con->waitForMoreInfo);

    if (con->noRspReqd == FALSE)
    {
        ccSendReleaseRsp(con);
    }
    CC_STATE_CHANGE(con, CCS_AWTDEALOCCFM);
    ccDeallocateResource(con, CC_INCOMING);
  }

  RETVALUE(ROK);
} /* End of ccConE04S96 */

/***********************************************************************************************
 *
 *      Fun:   ccConE14S96
 *
 *      Desc:  Connection state function
 *             event - Setup Timer expiry
 *             state - CCS_AWTMOREINFO
 *      Ret:   ROK     - successful
 *             RFAILED - unsuccessful
 *
 *      Notes: None
 *
 *      File:  cc_bdy2_2.c
 *
*************************************************************************************************/

#ifdef ANSI
PUBLIC S16 ccConE14S96
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                   /* bcm */
PTR     event2                   /* event */
 )
#else
PUBLIC S16 ccConE14S96(con, event, bcm, event2)
CcConCb *con;                    /* connection */
PTR     event;                   /* event */
AinBCM_t *bcm;                   /* bcm */
PTR     event2;                  /* event */
#endif
{
  CCLOGERR("Error: ccConE14S96: Setup Timer expired in state %d, icSu 0x%lx, ogSu 0x%lx,"
           "con->waitForMoreInfo 0x%02x\n", con->state, con->icSuConnId, con->ogSuConnId, con->waitForMoreInfo);     


  /* FID 15174.0 */
  ccStopConTmr(con, CC_TMR_MAXDIGIT);
  ccStopConTmr(con, CC_TMR_MINDIGIT);
    
  cleanUpMgQ(con);
  ccCleanUpMGCtx(con);
  ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL); 
  RETVALUE(ROK);
} /* End of ccConE14S96 */

/***********************************************************************************************
 *
 *      Fun:   ccConE23S96
 *
 *      Desc:  Connection state function
 *             event - CCE_CONTREP
 *             state - AWTMOREINFO
 *      Ret:   ROK     - successful,
 *             RFAILED - unsuccessful
 *
 *      Notes: Awaiting COT message for routing.
 *
 *      Author: Kevin.D.xie
 *
 *      File:  cc_bdy2_2.c
 *
*************************************************************************************************/

#ifdef ANSI
PUBLIC S16 ccConE23S96
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                   /* bcm */
PTR     event2                   /* event */
 )
#else
PUBLIC S16 ccConE23S96(con, event, bcm, event2)
CcConCb *con;                    /* connection */
PTR     event;                   /* event */
AinBCM_t *bcm;                   /* bcm */
PTR     event2;                  /* event */
#endif
{
    S16               ret = RFAILED;
    SiStaEvnt         *siInc;

    TRC3(ccConE23S96)

    /* map connection status event */
    con->icEvnt = (CcAllSdus *)event;
    siInc = &(con->icEvnt->m.ccStaEvnt.siStaEvnt);

    CCDP(AIN_DBG_LEVEL_0, "COT msg reaches GCC with con->waitForMoreInfo:0x%02x, waitForMoreInfo from ISUP:0x%02x\n",
         con->waitForMoreInfo, siInc->optElmnt.isup.waitForMoreInfo);

    /* Update the con->waitForMoreInfo */
    CC_SET_WAITMORE_INFO(siInc->optElmnt.isup.waitForMoreInfo);

    /* clear the buffer bit for COTreport event */
    CC_CLEAR_COTBUFFERED(con);

    CCDP(AIN_DBG_LEVEL_0, "After clear the COT buffer bit, con->waitForMoreInfo:0x%02x\n", con->waitForMoreInfo);

    if(CC_IS_CONTEXT_PRESENT(con))
    {
        ccUpdateMgQForSubRsc(NULLP, con->mgBlock->mgCtx->suCtxId, con);
        ccChangeCallStateOfAssociatedCalls(&(con->icBcm), CCS_CANNOTPROCESSEVENTS);
        CC_STATE_CHANGE(con, CCS_AWTMGCTCFMCOTIC1);
        con->mgBlock->con = con;
        ret = ccSendMgiQueue(con->mgBlock->mgCtx->suCtxId, con->mgBlock);
        if(ret == ROK)
        {
            RETVALUE(ROK);
        }
    }
    else
    {
        /* FID 15174.0 */
        if(!CC_IS_WAITMORE_INFO(con->waitForMoreInfo)) 
        {
            ccNoMoreInfoAndContinue(con);
        }
        RETVALUE(ROK);
    }
    if(ret != ROK)
    {
        CCLOGERR("ccUpdateMgQForSubRsc not sent:icSu:0x%lx, ctxId:0x%lx, ret:%d\n",
                 con->icSuConnId, con->mgBlock->mgCtx->suCtxId, ret);
        ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
    }
    RETVALUE(ROK);
} /* End of ccConE23S96 */


#ifdef ANSI
PUBLIC  S16 ccHandleContErr
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
#else
PUBLIC  S16 ccHandleContErr(con, event, bcm, event2)
CcConCb *con;                    /* connection */
PTR     event;                   /* event */
AinBCM_t *bcm;                    /* bcm */
PTR     event2;                   /* event */
#endif
{
  Continue_t        *pContinue      = NULLP;
  TRC3(ccHandleContErr)

  pContinue   = (Continue_t *) event2;


  if (pContinue->errorCode != FC_NOT_APPLICABLE)
  {
    ccStopConTmr(con, TMR_SETUP);
    ccStartConTmr(TMR_RELEASE, con, (PTR)&ccCp.genCfg);

    DP("**%s: Disconnecting call (icSu:%lx-ogSu:%lx) on ERROR MSG from FIC\n",
        __FUNCTION__, con->icSuConnId, con->ogSuConnId);

    /* FID: 14457.0 + */
    if ((ccCp.getsEnabled) && (IS_THIS_CALL_GETS(con)) &&
        (pContinue->errorCode == FC_HPC_QUEUE_TIMEOUT))
    {
      ccCp.getsSts.queuedTimeoutTx++;
    } /* FID 14457.0 - */

    if(isThisBcmInMPartyCall(&con->icBcm) || isThisBcmInMPartyCall(&con->ogBcm))
    {
      ccHandleCleanUpConCb(con, FALSE, CCTMPFAIL);
    }
    else
    {
      CCDP(AIN_DBG_LEVEL_1, "%s:icSu:%ld, ogSu:%ld, errorCode=%ld\n",
          __FUNCTION__, con->icSuConnId, con->ogSuConnId, pContinue->errorCode);

      if(con->ogRscVal)
      {
        ccDeallocateResource(con, CC_OUTGOING);
        con->ogRscVal = FALSE;
      }
      con->relPend = 0;
      ccProcessFailCnd(con, pContinue->errorCode, TRUE);
    }
  }
  else if((con->state == CCS_AWTRELCFM_IC)||   /*Bug86264*/
          (con->state == CCS_AWTRELCFM_OG))
  {
    ccIgnoreEvent(con, event, bcm, event2); 
  }   
  else
    ccUnexpEvent(con, event, bcm, event2);

  RETVALUE(ROK);
}

/* ACC Start */
/* 
 *      Fun:   ccConE03S99 
 *
 *      Desc:
 *             event - Connection status indication (03)
 *             state - Await O_SUSPEND response (DPE12)
*/  
PUBLIC  S16 ccConE03S99
(   
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)   
{
  if((ccIsForcedOnHookRcvd(con->icProtType, con->ogProtType, con->icEvntType,
          con->direction)) ||
     (ccIsOnHookRcvd(con->icProtType, con->ogProtType, con->icEvntType,
          con->direction)))
  {
    ccBufferEventToQueue(con, bcm, event, CCE_CNSTIND, sizeof(CcCnStEvnt), 0,
                         NULLP, 0);
    RETVALUE(ROK);
  }

  /* FID 14732 */ 
  ccExtractSipwMBufFromCnStInd(con, event);
  /* FID 14732 */

  ccExtractSdpFromCnStInd(con, event);

  CCLOGERR("** Ignoring icEvntType = %d, from prot = %d/%d - dir = %d **\n",
      con->icEvntType, con->icProtType, con->ogProtType, con->direction);

  RETVALUE(ROK);
} /* End of ccConE03S99 */

/* 
 *      Fun:   ccConE39S99 
 *
 *      Desc:
 *             event - Continue (39)
 *             state - Await O_SUSPEND response (DPE12)
*/  
PUBLIC  S16 ccConE39S99
(   
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)   
{
    SvcHeader_t       *ctSvcHeader    = NULLP;
    Continue_t        *pContinue      = NULLP;

    ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
  
    ctSvcHeader = (SvcHeader_t *) event;
    pContinue   = (Continue_t *)  event2;

    /* in this stage, call should be released in all cases */
    if  (con->icProtType == CC_LOCAL_SWT_PROT)
      ccRemoveCaleaAssnWithTappedBcm(con);

    if(pContinue->errorCode != FC_NOT_APPLICABLE)
    {
      CCLOGERR("ERROR FROM FIC = %ld\n", pContinue->errorCode);
      ccStopConTmr(con, CC_TMR_CALLDTL); /* Bug 38498 */

      if (CC_IS_CONTEXT_PRESENT(con))
      {
        S16  ret = RFAILED;
        con->failCndVal = pContinue->errorCode;

        ccStartConTmr(TMR_RELEASE, con, (PTR)&ccCp.genCfg); /* Bug 38498 */

        /* Subtract BCMs from MG context and then disconnect the legs */
        ccQSubBasedOnCcn(con);  /* FID 15261.0 */

        ret = ccSendMgiQueue(0, con->mgBlock);
        if (ret != ROK)
        {
          CCLOGERR( "**** ERROR: can't do ccSendMgiQueue *******\n");
          cleanUpMgQ(con);
          ccCleanUpMGCtx(con);
        }
        else
        {
          CC_STATE_CHANGE(con, CCS_AWTSWTCFM_SENDONEREL);
          con->mgBlock->con = con;
          con->relPend = OUTRELANDINFAILCOND; /* will be used in state 88 */
          RETVALUE(ROK);
        }
      }

      if (con->ogRscVal)
      {
        CcConCb *discCon = NULLP;

        /* Send Release and wait for ReleaseConfirm from OgBcm */
//        discCon = ccDisconnectBcmFromCon(con, OUTTOINC, &con->ogBcm);
        if (discCon)
        {
          /* Send RelReq to icBcm of discCon */
          CC_STATE_CHANGE(discCon, CCS_AWTRELCFM_IC);
          discCon->acnt = FALSE; /* Don't want to bill this call */
          ccStartConTmr(TMR_RELEASE, discCon, (PTR)&ccCp.genCfg);
          ccSendRelease(discCon, discCon->icSapCb, discCon->icSpConnId,
                        discCon->icSuConnId, discCon->icProtType,
                        CCCALLCLR, NULL);
        }
      }

      ccProcessFailCnd(con, pContinue->errorCode, TRUE);
    }
    else
    {
      /* Continue "Suspending" the call */
       CCDP(AIN_DBG_LEVEL_0, "%s: suspending the call %ld, %ld\n",
           __FUNCTION__, con->icSuConnId, con->ogSuConnId);

       ccProcessSuspendForMPartyCall(con, (PTR) con->ccEvntHold);

       RETVALUE(ROK);
    }


  RETVALUE(ROK);
} /* End of ccConE39S99 */

/* 
 *      Fun:   ccConE52S99 
 *
 *      Desc:
 *             event - Disconnect (52)
 *             state - Await O_SUSPEND response (DPE12)
*/  
PUBLIC  S16 ccConE52S99
(   
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)   
{
  SvcHeader_t        *diSvcHeader    = NULLP;

  diSvcHeader = (SvcHeader_t *) event;

  ccDisconnectCallForIcBCM(con, event);

  RETVALUE(ROK);
} /* End of ccConE52S99 */

/* 
 *      Fun:   ccConE71S99 
 *
 *      Desc:
 *             event - Connect to Resource (71)
 *             state - Await O_SUSPEND response (DPE12)
*/  
PUBLIC  S16 ccConE71S99
(   
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)   
{
  S16  ret = RFAILED;

  CCDP(AIN_DBG_LEVEL_1, "ccConE71S99:icSu:%ld, ogSu:%ld, icProt:%d\n", con->icSuConnId,
       con->ogSuConnId, con->icProtType);
  
  /* euysal - 02/14/2006 - Send CTRClear with Error Code ClcFailure in case of failure */
  con->lastState = con->state;
  ret = ccProcessConnectToResource(&(con->icBcm));

  if (ret !=ROK)
  {
    CCDP(AIN_DBG_LEVEL_0,
        "FAILED to Play Annc thru MGI for annc-conf facility\n");
    ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);

    RETVALUE(RFAILED);
  }
  else
  {
      CCDP(AIN_DBG_LEVEL_0, "Played Annc thru MGI for annc-conf facility\n");
  }

  RETVALUE(ROK);
} /* End of ccConE71S99 */
/* 
 *      Fun:   ccConE71S44 
 *
 *      Desc:
 *             event - Connect to Resource (71)
 *             state - Await O_Disconnect response (DPE09)
*/  
PUBLIC  S16 ccConE71S44
(   
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)   
{
  S16  ret = RFAILED;

  CCDP(AIN_DBG_LEVEL_1, "ccConE71S44:icSu:%ld, ogSu:%ld, icProt:%d\n", con->icSuConnId,
       con->ogSuConnId, con->icProtType);
  
  /* euysal - 02/14/2006 - Send CTRClear with Error Code ClcFailure in case of failure */
  con->lastState = con->state;
  ret = ccProcessConnectToResource(&(con->icBcm));

  if (ret !=ROK)
  {
    CCDP(AIN_DBG_LEVEL_0,
        "FAILED to Play Annc thru MGI for annc-conf facility\n");
    ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);

    RETVALUE(RFAILED);
  }
  else
  {
      CCDP(AIN_DBG_LEVEL_0, "Played Annc thru MGI for annc-conf facility\n");
  }

  RETVALUE(ROK);
} /* End of ccConE71S44 */
/* ACC End */

/* ACC End */
#if 1 /* EARLY_ACM */
/* 
*  
*      Fun:   ccConE76SBUF
*  
*      Desc:  Connection state function
*             event - CCE_EARLYACMTMREXP.
*             state - 
* 
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes:
*   
*      File:  cc_bdy2_2.c
*   
*/
PUBLIC  S16 ccConE76SBUF
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  CCDP(AIN_DBG_LEVEL_1, "ccConE76SBUF:icSu:%ld, ogSu:%ld\n",
       con->icSuConnId, con->ogSuConnId);

  ccBufferEventToQueue(con, bcm, event, CCE_EARLYACMTMREXP, sizeof(U8), 0,
                       NULLP, 0);
  RETVALUE(ROK);
}


/* 
 *      Fun:   ccHandleE76Exp
 *
 *      Desc:
 *             event - CCE_EARLYACMTMREXP (76)
 *             state - 
*/
PUBLIC  S16 ccHandleE76Exp
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  ccProcessEarlyACMTimerExp(con);
  RETVALUE(ROK);
}

/* 
 *      Fun:   ccConE11S100 
 *
 *      Desc:
 *             event - MgctTxnCfm (11)
 *             state - CCS_AWTMGCTCFMFOREARLYACM (100) 
*/
PUBLIC  S16 ccConE11S100
(   
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)   
{
  MgctTxnCfm       *txnCfm         = NULLP;
  S16               ret            = RFAILED;

  if ((con == NULLP) || (event == NULLP))
  {
    CCLOGERR("Critical ERROR: Null pointers:%lx, %ld\n", (U32)con, event);
    RETVALUE(RFAILED);
  }
  CCDP(AIN_DBG_LEVEL_1, "ccConE11S99:icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", con->icSuConnId, con->ogSuConnId,
       con->mgBlock->mgCtx->suCtxId, con->icBcm.svcHeader.transId);

  ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));

  txnCfm   = (MgctTxnCfm*) event;
  ret = ccHandleMgResponse(con, txnCfm);
  if (ret != ROK)
  {
    CCLOGERR("ccHandleResponse Failed:%ld\n", con->icSuConnId);
    cleanUpMgQ(con);
    ccHandleCleanUpConCb(con, TRUE, AM_CV_TMPFAIL);
    RETVALUE(ROK);
  }

  /* Send Early ACM towards incoming */
  ccSendBackwardMessageForAnnoucement(con, RtPlayAnnouncements, TRUE, FALSE);
  
  CC_STATE_CHANGE(con, con->lastState);
  
  /* BUG 67115 */
  if (con->icBcm.pic < PicCollectInformation)
  {
    ccSendOrSkipInfoCollected(con);
  }

  RETVALUE(ROK);
} /* End of ccConE11S100*/
#endif


/* Feature 1789 - German ISUP */

/*
 *
 *      Fun:   ccConE74S09
 *
 *      Desc:  Connection state function
 *             event - Toiw2 timer expire
 *             state - AWTANS
 *      Ret:   ROK     - successful,
 *             RFAILED - unsuccessful
 *
 *      Notes: None.
 *
 *      File:  cc_bdy2_2.c
 *
*/
#ifdef ANSI
PUBLIC S16 ccConE74S09
(
CcConCb		*con,		/* connection */
PTR		event,		/* event      */
AinBCM_t	*bcm,		/* bcm        */
PTR		event2		/* event      */
)
#else
PUBLIC S16 ccConE74S09(con, event, bcm, event2)
CcConCb		*con;		/* connection */
PTR		event;		/* event      */
AinBCM_t	*bcm;		/* bcm        */
PTR		event2;		/* event      */
#endif
{
   S16                     ret            = RFAILED;
   ProtType                icProtType     = CC_GET_PROTOCOL(con->icProtType);
   ProtType                ogProtType     = CC_GET_PROTOCOL(con->ogProtType);

   U8                      mgiAction;
   U8                      icMode;
   U8                      ogMode;
   U8                      icSignal;
   U8                      ogSignal;
   MgctEventsDescriptor   *pIcEvDesc      = NULLP;
   MgctEventsDescriptor   *pOgEvDesc      = NULLP;
   Buffer                 *uBuf           = NULLP;
   MgctEventsDescriptor    icEvDesc;
   MgctEventsDescriptor    ogEvDesc;
   MgctMediaDesc           icMediaDesc, *pIcMediaDesc = NULLP;
   MgctMediaDesc           ogMediaDesc, *pOgMediaDesc = NULLP;
   TknU8                   DSVal;

  CCDP(AIN_DBG_LEVEL_0, "ccConE74S09:  icEvntType=%d, icProtType=%d,"
       " ogProtType=%d\n", con->icEvntType, icProtType, ogProtType);

  cmMemset((U8 *)(&DSVal), 0, sizeof(TknU8));
  
  /* stop Toiw2 Timer */
  ccStopConTmr(con, CC_TMR_TOIW2);

  /* for ISUP-SIP interworking call, when the Toiw2 expires, Ringing tone
   * and early ACM shall be sent back to the preceding switch */
  if(((icProtType == CC_SI) || (icProtType == CC_EXT_BICC)) && (ogProtType == CC_SIP))
  {
     mgiAction = ccFindSwitchingAction(con, CCE_TOIW2TMREXPIRY, con->icEvntType,
                        con->direction, &con->icBcm, &con->ogBcm, &icMode, &ogMode,
                        &icSignal, &ogSignal, event, FALSE);

     CCDP(AIN_DBG_LEVEL_3, "Action == %d, icMode == %d, icSignal == %d,"
                " ogMode == %d, ogSignal == %d\n", mgiAction, icMode, icSignal,
                ogMode, ogSignal);

     if(mgiAction != CC_NO_MGI_ACTION)
     {
        if(mgiAction == CC_MGI_ACTION_ADD) mgiAction = MGCT_CMD_ADD;
        else if(mgiAction == CC_MGI_ACTION_SFM_ADD) mgiAction = MGCT_CMD_ADD;
        else mgiAction = MGCT_CMD_MODIFY;

        if (mgiAction == MGCT_CMD_ADD)
        {
           CCDP(AIN_DBG_LEVEL_0, "Building EventsDesc icSu = 0x%lx, ogSu = 0x%lx"
                "state = %d\n", con->icSuConnId, con->ogSuConnId, con->state);
           ccBuildMgctEventsDescriptor(con, &(con->icBcm), &(con->ogBcm), &icEvDesc,
                                        &ogEvDesc);
        }

        /* if Hanging termination detect is on and timerx value is not zero */
        ccBuildHangtermEventDesc(con, mgiAction, &icEvDesc, &ogEvDesc);

        if ((icEvDesc.pres == PRSNT_NODEF) && (!((icProtType == CC_CS_LN) ||
                                             (icProtType == CC_GR303) ||
                                             (icProtType == CC_CS_TG))))
        pIcEvDesc = &icEvDesc;
        if ((ogEvDesc.pres == PRSNT_NODEF) && (!((ogProtType == CC_CS_LN) ||
                                             (ogProtType == CC_GR303) ||
                                             (ogProtType == CC_CS_TG))))
        pOgEvDesc = &ogEvDesc;

        cmMemset((U8*) &icMediaDesc, 0, sizeof(MgctMediaDesc));
        cmMemset((U8*) &ogMediaDesc, 0, sizeof(MgctMediaDesc));

        pIcMediaDesc = &icMediaDesc;
        pOgMediaDesc = &ogMediaDesc;

        mgctMediaDesc(&(con->ctl), NOTPRSNT, icMode, NOTPRSNT, 0,
                      TGP_NLP_NOT_PRESENT, TGP_EC_NOT_PRESENT, 0, FALSE, 0,
                      DSVal, NULLP, NULLP, NULLP, pIcMediaDesc);

        mgctMediaDesc(&(con->ctl), NOTPRSNT, ogMode,  NOTPRSNT, 0,
                      TGP_NLP_NOT_PRESENT, TGP_EC_NOT_PRESENT, 0, FALSE, 0,
                      DSVal, NULLP, NULLP, NULLP, pOgMediaDesc);

        ccProcessForAddModify(con, &(con->icBcm), &(con->ogBcm), mgiAction,
                                 icMode, ogMode, icSignal, ogSignal, pIcMediaDesc,
                                 pOgMediaDesc, pIcEvDesc, pOgEvDesc);

        /* Set EarlyACMSt to EARLY_ACM_TO_BE_SENT to indicate that the early ACM shall
         * be sent after getting the Switching Connect Confirm event */
        con->EarlyACMSt = EARLY_ACM_TO_BE_SENT;

        CC_STATE_CHANGE(con, CCS_AWTANSSWT);
        con->mgBlock->con = con;

        /* Send the Mg Queue. */
        ret = ccSendMgiQueue(con->mgBlock->mgCtx->suCtxId, con->mgBlock);
        if(ret != ROK)
        {
           CCLOGERR( "ERROR: ccSendMgiQueue FAILED: %ld\n", con->icSuConnId);
           cleanUpMgQ(con);
           ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
           RETVALUE(RFAILED);
        }
     }
     else
     {
        CCDP(AIN_DBG_LEVEL_0, "ccConE74S09: Build and Send early ACM to Ingress side\n");
        /* build the early ACM and send it backward */
        ccBuildSs7ACMOrCPGMsg(con, &(con->ogEvnt->m.ccCnStEvnt.m.siCnStEvnt), FALSE);
        ccSendCnStReq(con, con->icSapCb->suId, &con->icSapCb->pst,
                           con->icSapCb->spId, con->icSpConnId, con->icSuConnId,
                           con->icProtType, con->ogEvntType,
                           &(con->ogEvnt->m.ccCnStEvnt), uBuf);
     }
  }
  RETVALUE(ROK);
}/* end of ccConE74S09 */

/* FID 15174.0 Support SIP ISUP overlap receiving and sending */
/*
 *
 *      Fun:   ccConE92S09
 *
 *      Desc:  Connection state function
 *             event - Toiw3 timer expire
 *             state - AWTANS
 *      Ret:   ROK     - successful,
 *             RFAILED - unsuccessful
 *
 *      Notes: None.
 *
 *      File:  cc_bdy2_2.c
 *
 */
#ifdef ANSI
PUBLIC S16 ccConE92S09
(
CcConCb         *con,           /* connection */
PTR             event,          /* event      */
AinBCM_t        *bcm,           /* bcm        */
PTR             event2          /* event      */
)
#else
PUBLIC S16 ccConE92S09(con, event, bcm, event2)
CcConCb         *con;           /* connection */
PTR             event;          /* event      */
AinBCM_t        *bcm;           /* bcm        */
PTR             event2;         /* event      */
#endif
{

  CCDP(AIN_DBG_LEVEL_0, "ccConE92S09:  icEvntType=%d, icProtType=%d,"
       " ogProtType=%d\n", con->icEvntType, CC_GET_PROTOCOL(con->icProtType),
       CC_GET_PROTOCOL(con->ogProtType));

  /* for ISUP-SIP/SIPT interworking call, when the Toiw3 expires,
   * call will be released with cause value #28 Address Incomplete.
   */
  ccProcessFailCnd(con, FC_INVNMBFORM, TRUE);

  RETVALUE(ROK);
}

/***********************************************************************************************
 *
 *      Fun:   ccConE04S97
 *
 *      Desc:  Connection state function
 *             event - Release Indication
 *             state - AWTDELAYEDREL
 *      Ret:   ROK     - successful,
 *             RFAILED - unsuccessful
 *
 *      Notes: None
 *
 *      File:  cc_bdy2_2.c
 *
*************************************************************************************************/

#ifdef ANSI
PUBLIC S16 ccConE04S97
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                   /* bcm */
PTR     event2                   /* event */
 )
#else

PUBLIC S16 ccConE04S97(con, event, bcm, event2)
CcConCb *con;                    /* connection */
PTR     event;                   /* event */
AinBCM_t *bcm;                   /* bcm */
PTR     event2;                  /* event */
#endif
{

  TRC3(ccConE04S97)

  /* The "Release Indication" related process has already been executed
   * when I/C side sent RELIND. By now, GCC is waiting for another
   * RELIND from O/G side, as no RELREQ was sent to it (just DELAYEDREL
   * was sent to it).
   */
  
  /* check the direction before the relCfm is entertained! */
  if(con->direction == INCTOOUT)
    RETVALUE(ROK);

  if (con->noRspReqd == FALSE) 
     ccSendReleaseRsp(con); 
  
  CC_STATE_CHANGE(con, CCS_AWTDEALOCCFM);
  #ifdef CC_CALEA 
  if (con->icProtType != CC_LOCAL_SWT_PROT)
    ccReleaseCaleaLegs(con);
  #endif
  if(con->ogRscVal == TRUE)
  {
    ccDeallocateResource(con, CC_OUTGOING);
  }
  else
  {
    /* Can happen if Call fails during outgoing ISDN channel 
     * negotiation failure. Or any case where the ogRsc is not valid but
     * still are waiting for RelCfm from layer-3.*/
   
    /* this is the end (of Call !!!) */
    /* Connection shall be cleared by the state machine driver */
    CC_STATE_CHANGE(con, CCS_IDLE);
    ccRelCon(con);
  }

  RETVALUE(ROK);

} /* end of ccConE04S97 */

/*
 *
 *      Fun:   ccConE39S09
 *
 *      Desc:  Connection state function
 *             event - CONTINUE.
 *             state - AWTANS
 *      Ret:   ROK     - successful,
 *             RFAILED - unsuccessful
 *
 *      Notes: None.
 *
 *      File:  cc_bdy2.c
 *
*/
PUBLIC  S16 ccConE39S09
( 
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  S16               ret             = RFAILED;
  Continue_t        *pContinue      = NULLP;

  pContinue   = (Continue_t *) event2;

 #if 1 /* EARLY_ACM */
  ccStopConTmr(con, CC_TMR_EARLYACM);
 #endif

  ccStopConTmr(con, TMR_SETUP);
  ccStartConTmr(TMR_RELEASE, con, (PTR)&ccCp.genCfg);

  if (pContinue->errorCode != FC_NOT_APPLICABLE)
  {
    CCLOGERR("ERROR FROM FIC = %ld(icSu:%lx-ogSu:%lx)\n", pContinue->errorCode,
             con->icSuConnId, con->ogSuConnId);
#ifdef CC_CALEA 
    if (con->icProtType == CC_LOCAL_SWT_PROT)
    {
       ccUpdateCloseCause(con, CC_BOTH, CscInternalFailure);
       ccRemoveCaleaAssnWithTappedBcm(con);
    }
#endif

#ifdef SND_NOT_IND
    /* Update termInd with exception for Termination Notification msg */
    con->ainInfo.termInd = EXCEPTION_IND;
#endif

    ccStopConTmr(con, AIN_O_NO_ANS_TMR);
    ccDeleteBufferedEvent(con, con->icSuConnId, CCE_ONOANSEXP);

    if(isThisBcmInMPartyCall(&con->icBcm) || isThisBcmInMPartyCall(&con->ogBcm))
    {
      ccHandleCleanUpConCb(con, FALSE, CCTMPFAIL);
    }
    else
    {
      con->failCndVal = pContinue->errorCode;

      if (CC_IS_CONTEXT_PRESENT(con))
      {
        /* Blow off the switching and Release the Call */
        ccQSubBasedOnCcn(con);  /* FID 15261.0 */

        /* disconnect the outgoing leg but, continue processing
         * failure condition on incoming BCM */
        con->relPend = OUTRELANDINFAILCOND;

        CC_STATE_CHANGE(con, CCS_AWTSWTCFM_SENDONEREL);
        con->mgBlock->con = con;
        ret = ccSendMgiQueue(0, con->mgBlock);
        if (ret != ROK)
        {
          CCLOGERR( "**** ERROR: can't do ccSendMgiQueue *******\n");
          cleanUpMgQ(con);
          ccHandleCleanUpConCb(con, TRUE, AM_CV_TMPFAIL);
          RETVALUE(ROK);
        }
        else
          RETVALUE(ROK);
      }
      else
      {
        ccFreeTheRscFromCon(con, OUTTOINC); /* This will take care of REL_CFM */
        ccProcessFailCnd(con, con->failCndVal, TRUE);
        RETVALUE(ROK);
      }
    }
  }
  else
  {
    CCDBGP(DBGMASK_CC, (ccInit.prntBuf, " ccIgnoreEvent called \n")); 
  }

  RETVALUE(ROK);
} /* ccConE39S09 */


/* FID-2112 SIP UPDATE + */
/*
 *
 *      Fun:   ccConE11SAckOrPrack
 *
 *      Desc:  Connection state function
 *             event - MG Txn Cfm.
 *             state - CCS_AWTMGCTCFM_ACK
 *      Ret:   ROK     - successful,
 *             RFAILED - unsuccessful
 *
 *      Notes: None.
 *
 *      File:  cc_bdy2_2.c
 *
*/
PUBLIC S16 ccConE11SAckOrPrack
( 
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                   /* bcm */
PTR     event2                   /* event */
)
{
    /* FID-2575: LMSD + */
    S16              ret               = RFAILED;
    MgctTxnCfm      *txnCfm            = NULLP;
    CcCnStEvnt      ogEvnt;
    Buffer          *uBuf              = NULLP;
    AinBCM_t        *tmpBcm            = NULLP;
    AinBCM_t        *tmpCompBcm        = NULLP;
    U8              tmpDirection       = DIR_UNKNOWN;
    Bool            postAnswer         = FALSE;
    Bool            doRecursion        = TRUE;
    Bool            isAck              = FALSE;
    Bool            precnd             = FALSE;

    txnCfm   = (MgctTxnCfm*) event;
    
    /* bug 91532 */
    cmMemset((U8 *)&ogEvnt, 0, sizeof(ogEvnt));
    
    ret = ccHandleMgResponse(con, txnCfm);
    if (ret != ROK)
    {
      CCLOGERR("Error: %s: ccHandleMgResponse failed\n", __FUNCTION__);
      ccKillCallOnThisCon(con);
      RETVALUE(ROK);
    }
    else
    {
      ccProcessCcChangeforX2SIG(con); /* FID 15083.0 */
      if(CC_IS_MRF(con->mrfFlag, MRF_PLAYING_ANNC))
      {   
          CCDP(AIN_DBG_LEVEL_0, "%s: MRF Playing annc\n", __FUNCTION__);
          CC_STATE_CHANGE(con, CCS_AWT_MRFRSP);
      }
      else
      {
        switch (CC_OFFANS_METHOD_USED(con))
        {
          case CC_OFFANS_TYPE_REL18X_INVITENOSDP:
            /* Bug 90408 change back to S91 to continue processing announcement */
            switch(con->lastState)
            {
            case CCS_AWTPAUSETIMEREXPIRY:
                ccStopConTmr(con, CC_PAUSETIMER);
                ccStartConTmr(CC_PAUSETIMER, con, (PTR)&ccCp.genCfg);
                CC_STATE_CHANGE(con, con->lastState);
                break;
            default:
                CC_STATE_CHANGE(con, CCS_AWTANS);
                if (con->icBcm.pic >= PicOActive)
                {
                    CC_STATE_CHANGE(con, CCS_ANSWERED);
                 }
                break;
            }
            break;

          case CC_OFFANS_TYPE_200OKINVITE :
            switch (con->lastState)
            {
               case CCS_AWTPAUSETIMEREXPIRY:
                  /* BUG93368 for invite w/o sdp ansInd=Y, after ACK w/sdp
                   * respond with 200OK, should go back to continue annc
                   * playing */
                  if (isAnsIndPresent(con))
                  {
                     ccStopConTmr(con, CC_PAUSETIMER);
                     ccStartConTmr(CC_PAUSETIMER, con, (PTR)&ccCp.genCfg);
                     CC_STATE_CHANGE(con, con->lastState);
                  } 
                  else
                  {
                     ccStartConTmr(CC_PAUSETIMER, con, (PTR)&ccCp.genCfg);
                     CC_STATE_CHANGE(con, con->lastState);
                  }
                  break;
               default:
                  {
                     CC_STATE_CHANGE(con, CCS_ANSWERED);
                     if (con->icBcm.pic < PicOActive)
                     {
                        CC_STATE_CHANGE(con, CCS_AWTANS);
                     }
                  }
                  break;
            }
            break;

          default:
            if (con->icBcm.pic >= PicOActive)
            {
              postAnswer = TRUE;
              CC_STATE_CHANGE(con, CCS_ANSWERED);
            }
            else
            {
              switch(con->lastState)
              {
              case CCS_AWTPAUSETIMEREXPIRY:
                ccStartConTmr(CC_PAUSETIMER, con, (PTR)&ccCp.genCfg);
                /* Fall through */
              case CCS_AWTMGCTNOTIFY0:
              case CCS_AWTRSPFORDPE3:
                CC_STATE_CHANGE(con, con->lastState);
                break;
              default:
                CC_STATE_CHANGE(con, CCS_AWTANS);
                break;
              }
            }
            break;
        }
      }
      /* Bug 62783 */
      tmpDirection = con->direction;
      con->direction = CC_GET_EVNTHOLD_DIR(con->directionHeld);

      /* FID 15261.0 + */
      if (con->direction == INCTOOUT)
      {
        CC_DERIVE_BCMS(con, tmpBcm, tmpCompBcm);
      }
      else
      {
        CC_DERIVE_BCMS(con, tmpCompBcm, tmpBcm);
      }
      /* FID 15261.0 - */

      if (ccIsAckRcvd(CC_GET_PROTOCOL(con->icProtType),
                      CC_GET_PROTOCOL(con->ogProtType),
                      con->evntTypeHeld, con->direction))
      {
        isAck = TRUE;
      }

       precnd = (!CC_IS_CALL_FLAG2(con,CC_FLAG_PRECONDITION)  &&
                 ((con->icPrecndCb) && (con->ogPrecndCb) &&
                  ((CC_GET_PROTOCOL(con->icProtType)== CC_SIP || CC_GET_PROTOCOL(con->icProtType)==CC_SIPT) && 
                   (CC_GET_PROTOCOL(con->ogProtType)== CC_SIP || CC_GET_PROTOCOL(con->ogProtType)==CC_SIPT))));

      if (CC_OFFANS_GET_SUBSTATE(con) == CC_OFFANS_SUBSTATE_AWAITRMTANS_INVITENOSDP) 
      {
        /* INVITE w/o SDP and re-INVITE w/o SDP */
        if (con->direction == INCTOOUT)
        {
          if (CC_OFFANS_ICRMTANS_PENDING(con))
          {
            /* Free leftover SDP */
            if (con->icIpParam.locDesc)
            {
              cmFreeSdp(ccInit.region, ccInit.pool, &con->icIpParam.locDesc);
            }
          }

          if (precnd || CC_IS_CALL_FLAG2(con,CC_FLAG_PRECONDITION))  
          {
            CCDP(AIN_DBG_LEVEL_0, "%s: Call flow is changed by precondition\n", __FUNCTION__);
            if (!CC_IS_CALL_MULTI_MG(con) || 
                (CC_IS_CALL_MULTI_MG(con) && CC_OPERATING_LAST_MG(con)))
            {
              CC_OFFANS_STATE_CHANGE(con, CC_OFFANS_STATE_NULL,
                                   CC_OFFANS_STATE_NO_OP,
                                   CC_OFFANS_SUBSTATE_NULL,
                                   CC_OFFANS_TYPE_INVALID);
            }
          }
          else 
          {
            CC_OFFANS_STATE_CHANGE(con, CC_OFFANS_STATE_NULL,
                                   CC_OFFANS_STATE_NO_OP,
                                   CC_OFFANS_SUBSTATE_NULL,
                                   CC_OFFANS_TYPE_INVALID);
          }

          if ((!isAck) && ((!CC_OFFANS_OGOFFANS_PENDING(con)) ||
                           (CC_OFFANS_OGRMTANS_PENDING(con)))) /* BUG: 85868 */
            doRecursion = FALSE;            
        }
        else
        {
          if (CC_OFFANS_OGRMTANS_PENDING(con))
          {
            /* Free leftover SDP */
            if (con->ogIpParam.locDesc)
            {
              cmFreeSdp(ccInit.region, ccInit.pool, &con->ogIpParam.locDesc);
            }
          }

          if (precnd || CC_IS_CALL_FLAG2(con,CC_FLAG_PRECONDITION))  
          {
            CCDP(AIN_DBG_LEVEL_0, "%s: Call flow is changed by precondition\n", __FUNCTION__);
            if (!CC_IS_CALL_MULTI_MG(con) || 
                (CC_IS_CALL_MULTI_MG(con) && CC_OPERATING_LAST_MG(con)))
            {
              CC_OFFANS_STATE_CHANGE(con, CC_OFFANS_STATE_NO_OP,
                                 CC_OFFANS_STATE_NULL,
                                 CC_OFFANS_SUBSTATE_NULL,
                                 CC_OFFANS_TYPE_INVALID);
            }
          }
          else
          {
              CC_OFFANS_STATE_CHANGE(con, CC_OFFANS_STATE_NO_OP,
                                 CC_OFFANS_STATE_NULL,
                                 CC_OFFANS_SUBSTATE_NULL,
                                 CC_OFFANS_TYPE_INVALID);

          }
        }
      }

      if (isAck)
      {
        if (con->direction == INCTOOUT)
        {
	        if (!CC_OFFANS_OGOFFANS_PENDING(con))
 	        {
            /* Drop the ACK */
            CCDP(AIN_DBG_LEVEL_0, "%s: Dropping the ACK\n", __FUNCTION__);
            con->direction = tmpDirection;

            /* BUG:93368 */
            if ((con->state == CCS_AWTPAUSETIMEREXPIRY)&& (con->ogBcm.bcmType == BtMax))
            {
               CCDP(AIN_DBG_LEVEL_0, "%s: This is playannc case,just return !\n", __FUNCTION__);
               RETVALUE(ROK);
            }
            /* Bug 63314 */
            if ((!ccIsSwitchingLocallyDone(&con->icBcm)) &&
                (!ccIsSwitchingLocallyDone(&con->ogBcm)) &&
                (!CC_IS_CALL_FLAG_SET(con->flag,CC_FLAG_TRANSCODING)) &&
                (!CC_IS_CALL_FLAG_SET(con->flag,CC_FLAG_IPV_CONVERSION)) && /* FID 14341 - IPV6 */
                (!CC_IS_CALL_FLAG_SET(con->flag,CC_FLAG_NAT))) /* Bug# 71638 - FID 14732 */
            {
              CCDP(AIN_DBG_LEVEL_0, "%s: Removing the MG from bearer path\n", __FUNCTION__);
              ccCreateBearer(&(con->icBcm), &(con->ogBcm), NULLP, NULLP,
                             con->state, FALSE);
            }
            /* FID 15261.0 + */
            else if ((con->ccmCb.ccn == CCN_1) && (!ccIsBearerNATIngress(con)))
            {
              /* Need to remove the ingress transient MG */
             ccCreateAgnosticBearer(&(con->icBcm), &(con->ogBcm), NULLP, NULLP,
                                    con->state, FALSE);
            }
            /* FID 15261.0 - */
            else if (ccIsIPResource(&(con->icBcm))&&ccIsIPResource(&(con->ogBcm))&&
                     CC_IS_IC_BCM_2_PARTY_CALL(con)&&
                     CC_IS_OG_BCM_2_PARTY_CALL(con)&& 
                     ((CC_IS_CALL_FLAG_SET(con->flag,CC_FLAG_TRANSCODING))||
                      (CC_IS_CALL_FLAG_SET(con->flag,CC_FLAG_IPV_CONVERSION))|| /* FID 14341 - IPV6 */
                      (CC_IS_CALL_FLAG_SET(con->flag,CC_FLAG_NAT))))
            {
              if (((con->ccmCb.ccn == CCN_0)&&
                   (!IS_MULTIMEDIA(con))&&
                   (ccCheckEnableMediaAgnostic(con,0, FALSE) == ROK))||
                  ((con->ccmCb.ccn == CCN_1)&&
                   (!IS_MULTIMEDIA_4_BCM(&(con->icBcm)))&&
                (ccCheckEnableMediaAgnostic(con,CC_INGRESS_MG, FALSE) == ROK))||
                  ((con->ccmCb.ccn == CCN_1)&&
                   (!IS_MULTIMEDIA_4_BCM(&(con->ogBcm)))&&
                (ccCheckEnableMediaAgnostic(con,CC_EGRESS_MG, FALSE) == ROK)))
              {
                CCDP(AIN_DBG_LEVEL_1, "Call ccCreateAgnosticBearer\n");
                ccCreateAgnosticBearer(&(con->icBcm), &(con->ogBcm), NULLP, NULLP, con->state, FALSE);
                RETVALUE(ROK);
              }
            }
            RETVALUE(ROK);
          }

          ccIncVersionId(tmpCompBcm);
        }
        else
        {
          ccIncVersionId(tmpCompBcm);
        }
      }
      else
      {
        if (con->EarlyACMSt == EARLY_ACM_HAS_BEEN_SENT)
        {
          /* Don't interwork this PRACK */
          CCDP(AIN_DBG_LEVEL_0, "%s: Early ACM, dropping the PRACK\n", __FUNCTION__);
          con->EarlyACMSt = EARLY_ACM_NOT_AVAILABLE;
          con->direction = tmpDirection;
          RETVALUE(ROK);
        }

        /* Bug# 83948 */
        if (((CC_EGRESS_INTER_CCS_CALL(con) && !CC_IS_CALL_FLAG2(con,CC_FLAG_PRECONDITION)) ||
            (!CCT_IS_100REL_SUPPORTED(con->ogSipMethodSupMask))) &&
            (CC_OFFANS_METHOD_USED(con) != CC_OFFANS_TYPE_PRACK))
        {
          /* Drop the PRACK */
          CCDP(AIN_DBG_LEVEL_0, "%s: Dropping the PRACK\n", __FUNCTION__);

          con->direction = tmpDirection;
          RETVALUE(ROK);
        }

        if (CC_OFFANS_METHOD_USED(con) == CC_OFFANS_TYPE_PRACK)
        {
          if (CCT_IS_100REL_SUPPORTED(con->ogSipMethodSupMask))
          {	
            if ((!CC_OFFANS_OGOFFANS_PENDING(con)) && (!postAnswer))
            {

              if ((!CC_IS_CALL_MULTI_MG(con)) ||
                  (CC_IS_CALL_MULTI_MG(con) && CC_OPERATING_LAST_MG(con)))
	            {
                /* FID-14209: Enhanced Transcoding + */
                ccCodecReorderPostCfm(&con->ogBcm, con->ogIpParam.locDesc);
                /* FID-14209: Enhanced Transcoding - */
	
                ccIncVersionId(tmpCompBcm);

                CC_OFFANS_STATE_CHANGE(con, CC_OFFANS_STATE_NO_OP,
                                       CC_OFFANS_STATE_SENTOFFER,
                                       CC_OFFANS_SUBSTATE_NULL,
                                       CC_OFFANS_TYPE_PRACK);
              }
            }
            else
            {
              ccSendPrackRsp(&con->icBcm, NOTPRSNT, 0);
              doRecursion = FALSE;
            }
          }
          else
          {
            switch(con->lastState)
            {
            case CCS_AWTPAUSETIMEREXPIRY:
            case CCS_AWTMGCTNOTIFY0:
            case CCS_AWTRSPFORDPE3:
              /* This PRACK cannot be mapped */
              CCDP(AIN_DBG_LEVEL_0, "%s: Dropping the PRACK\n", __FUNCTION__);
              ccSendPrackRsp(&con->icBcm, NOTPRSNT, 0);

              con->direction = tmpDirection;
              RETVALUE(ROK);
            default:
              break;
            }
            doRecursion = FALSE;
          }
        }
      }

      /* FID 15261.0 + */
      if (CC_IS_CALL_MULTI_MG(con) && (doRecursion == TRUE))
      {
        if (!CC_OPERATING_LAST_MG(con))
        {
          if (con->direction == INCTOOUT)
          {
              if ((con->ccmCb.ccn == CCN_5) && (con->ccmCb.state == CC_INGRESS_MG))
                con->ccmCb.state = CC_Y_MG;
              else
                con->ccmCb.state = CC_EGRESS_MG;
          }
          else
          {
            if ((con->ccmCb.ccn == CCN_5) && (con->ccmCb.state == CC_EGRESS_MG))
              con->ccmCb.state = CC_Y_MG;
            else
              con->ccmCb.state = CC_INGRESS_MG;
          }

          con->icEvntType = con->evntTypeHeld;
          ccProcessPrackOrAck(con, CC_GET_PROTOCOL(con->icProtType),
                              CC_GET_PROTOCOL(con->ogProtType), con->ccEvntHold);
          con->direction = tmpDirection;
          RETVALUE(ROK);
        }
      }		
      /* FID 15261.0 - */

      if (con->direction == INCTOOUT)
      {
        if (CC_OFFANS_OGANS_PENDING(con))
        {
          CC_OFFANS_STATE_CHANGE(con, CC_OFFANS_STATE_NO_OP,
                                 CC_OFFANS_STATE_NULL,
                                 CC_OFFANS_GET_SUBSTATE(con),
                                 CC_OFFANS_METHOD_USED(con));
        }
      }
      else
      {
        if (CC_OFFANS_ICANS_PENDING(con))
        {
          CC_OFFANS_STATE_CHANGE(con, CC_OFFANS_STATE_NULL,
                                 CC_OFFANS_STATE_NO_OP,
                                 CC_OFFANS_GET_SUBSTATE(con),
                                 CC_OFFANS_METHOD_USED(con));
        }
      }

      con->icEvnt = (CcAllSdus *) con->ccEvntHold;
      con->ogEvnt = (CcAllSdus *) &ogEvnt;
      if (con->mrfFlag)
      {
        if(CC_IS_MRF(con->mrfFlag, MRF_183_ANNC))
        {
          /* send PRACK to MRF even OG offans complete for PRACK*/
          ccSendPrack(&con->ogBcm);
        }
        else if (CC_IS_MRF(con->mrfFlag, MRF_200_ANNC)&&
            CC_IS_MRF(con->mrfFlag, MRF_SEND_ACK))
        {
          ccSendAck(&con->ogBcm);
        }
        con->direction = tmpDirection;
        RETVALUE(ROK);
      }
      else
        ret = ccMapEvent(con, CCE_CNSTIND, con->evntTypeHeld);
      if (ret == CCIGNOREMAPPING)
      {
        ccDropUBuf(&con->uBuf);
        con->direction = tmpDirection;
        RETVALUE(ROK);
      }
      else if (ret != ROK)
      {
        ccDropUBuf(&con->uBuf);
        CCLOGERR( "ERROR: ccMapEvent failed: icProtType:%d, ogProtType:%d,"
                  " icSu:0x%lx, ogSu:0x%lx\n", con->icProtType,
                  con->ogProtType, con->icSuConnId, con->ogSuConnId);
        con->direction = tmpDirection;
        RETVALUE(ROK);
      }

      uBuf = con->uBuf;
      con->uBuf = NULLP;
      if (con->direction == INCTOOUT)     
      {
        ccSendCnStReq(con, con->ogSapCb->suId, &con->ogSapCb->pst,
                      con->ogSapCb->spId, con->ogSpConnId, con->ogSuConnId,
                      con->ogProtType, con->ogEvntType, &ogEvnt, uBuf);
      }
      else
      {
        ccSendCnStReq(con, con->icSapCb->suId, &con->icSapCb->pst,
                      con->icSapCb->spId, con->icSpConnId, con->icSuConnId,
                      con->icProtType, con->ogEvntType, &ogEvnt, uBuf);
      }
    }

  con->direction = tmpDirection;
  /* FID-2575: LMSD - */
  RETVALUE(ROK);
} /* ccConE11SAckOrPrack */

/*
 *
 *      Fun:   ccConE83S09
 *
 *      Desc:  Event - CPG Retry timer expiry (83)
 *             state - CCS_AWTANS (09)
 *      Ret:   ROK     - successful,
 *             RFAILED - unsuccessful
 *
 *      Notes:
 *             1) CPG retry is only applicable for ISUP->SIP call where
 *             the call is not answered and UPDATE (as a result of CPG) failed
 *             due to glare on egress SIP leg.
 *
 *      File:  cc_bdy2_2.c
 *
*/
PUBLIC S16 ccConE83S09
( 
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                   /* bcm */
PTR     event2                   /* event */
)
{
   HoldState_t newHoldState;
   S16         holdXferFlag;
   ProtType    icProtType     = CC_GET_PROTOCOL(con->icProtType);
   ProtType    ogProtType     = CC_GET_PROTOCOL(con->ogProtType);

  /* Lets retry the CPG processing all over again. */

   if (ROK == ccFindNewStateForDualCallHold(con->notifIndForCpgRetry,
                                   (*HOLD_STATE(con)),
                                   (*HOLD_FLAG(con)),
                                   INCTOOUT, /* Read Note: 1 above */
                                   &newHoldState,
                                   &holdXferFlag))
   {
     /* FID17378.0 */
     ccSetIcsHoldRetrieval(con, con->notifIndForCpgRetry, &newHoldState, &holdXferFlag);

     ccProcessDualCallHoldCpg(con, icProtType, ogProtType, newHoldState,
          holdXferFlag);

     RETVALUE(ROKIGNORE);
   }

   RETVALUE (ROK);
} /* End of ccConE83S09 */
/* FID-2112 SIP UPDATE - */
/* FID-2575: LMSD + */
/*
 *      Fun:   ccConE11S136
 *
 *      Desc:  Connection state function
 *             event - MG Txn Cfm
 *             state - CCS_AWT_MG_PRACKRSP
 *
 *      Ret:   ROK     - successful,
 *             RFAILED - unsuccessful
 *
 *      Notes: None.
 *
 *      File:  cc_bdy2_2.c
 */
PUBLIC S16 ccConE11S136
( 
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                   /* bcm */
PTR     event2                   /* event */
)
{
    S16              ret               = RFAILED;
    MgctTxnCfm      *txnCfm            = NULLP;
    U8              tmpDirection       = DIR_UNKNOWN;

    txnCfm   = (MgctTxnCfm*) event;
    ret = ccHandleMgResponse(con, txnCfm);
    if (ret != ROK)
    {
      CCLOGERR("Error: %s: ccHandleMgResponse failed\n", __FUNCTION__);
      ccKillCallOnThisCon(con);
      RETVALUE(ROK);
    }
    else
    {
      ccProcessCcChangeforX2SIG(con); /* FID 15083.0 */
      /* FID17373.0 */
     if (CC_IS_CALL_FLAG2(con,CC_FLAG_PRECONDITION) && (con->direction == OUTTOINC) &&
         (con->ogPrecndCb) && IS_PRECND_COMPLETED(con->ogPrecndCb->flag))
     {
        CC_STATE_CHANGE(con, CCS_AWTANS);
     }
     else
        CC_STATE_CHANGE(con, con->lastState);

      /* Bug 62783 */
      tmpDirection = con->direction;
      con->direction = CC_GET_EVNTHOLD_DIR(con->directionHeld);

      /* FID 15261.0 + */
      if (CC_IS_CALL_MULTI_MG(con))
      {
        if (!CC_OPERATING_LAST_MG(con))
        {
          /* + BUG: 85423 */
          if ((con->ccmCb.ccn == CCN_5) && (con->ccmCb.state == CC_EGRESS_MG))
            con->ccmCb.state = CC_Y_MG;
          else
            con->ccmCb.state = CC_INGRESS_MG;
          /* - BUG: 85423 */

          con->icEvntType = con->evntTypeHeld;

          ccProcessPrackRsp(con, CC_GET_PROTOCOL(con->icProtType),
                            CC_GET_PROTOCOL(con->ogProtType), con->ccEvntHold);
          con->direction = tmpDirection;
          RETVALUE(ROK);
        }
      }
      /* FID 15261.0 - */

      /* FID-2593: Transcoding + */
      if (CC_IS_MG_NEEDED_IN_IP_CALL(con))
      {
	CcCnStEvnt      ogEvnt;
        Buffer         *uBuf = NULLP;	

        /* bug 91532 */
        cmMemset((U8 *)&ogEvnt, 0, sizeof(ogEvnt));
        
        ccIncVersionId(&con->icBcm);

        CC_OFFANS_STATE_CHANGE(con, CC_OFFANS_STATE_NULL,
                               CC_OFFANS_STATE_NULL,
                               CC_OFFANS_SUBSTATE_NULL,
                               CC_OFFANS_TYPE_INVALID);
        con->icEvnt = (CcAllSdus *) con->ccEvntHold;
        con->ogEvnt = (CcAllSdus *) &ogEvnt;
        ret = ccMapEvent(con, CCE_CNSTIND, con->evntTypeHeld);
        if (ret == CCIGNOREMAPPING)
        {
          ccDropUBuf(&con->uBuf);
          con->direction = tmpDirection;
          RETVALUE(ROK);
        }
        else if (ret != ROK)
        {
          ccDropUBuf(&con->uBuf);
          CCLOGERR( "ERROR: ccMapEvent failed: icProtType:%d, ogProtType:%d,"
                    " icSu:0x%lx, ogSu:0x%lx\n", con->icProtType,
                    con->ogProtType, con->icSuConnId, con->ogSuConnId);
          con->direction = tmpDirection;
          RETVALUE(ROK);
        }

        uBuf = con->uBuf;
        con->uBuf = NULLP;
        ccSendCnStReq(con, con->icSapCb->suId, &con->icSapCb->pst,
                      con->icSapCb->spId, con->icSpConnId, con->icSuConnId,
                      con->icProtType, con->ogEvntType, &ogEvnt, uBuf);
      }
      /* FID-2593: Transcoding - */
      else
      {
        /* Something's wrong */
        ABORT_DEBUG;
      }
    }

  con->direction = tmpDirection;
  RETVALUE(ROK);
} /* ccConE11S136 */
/* FID-2575: LMSD - */

/* FID-14715: Support INAP Operations for FP/PP/VPN with OSP + */
/*
 *      Fun:   ccConE11S137
 *
 *      Desc:  Connection state function
 *             event - MG Txn Cfm
 *             state - CCS_AWT_MGI_CFM_FOR_DL2
 *
 *      Ret:   ROK     - successful,
 *             RFAILED - unsuccessful
 *
 *      Notes: None.
 *
 *      File:  cc_bdy2_2.c
 */
PUBLIC S16 ccConE11S137
( 
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                   /* bcm */
PTR     event2                   /* event */
)
{
  S16              ret               = RFAILED;
  MgctTxnCfm      *txnCfm            = NULLP;

  txnCfm   = (MgctTxnCfm*) event;
  ret = ccHandleMgResponse(con, txnCfm);
  
  /* BUG:80938 + */
  if (CC_IS_CALL_MULTI_MG(con))
  {
    if (con->ccmCb.mgPend)
      con->ccmCb.mgPend--;

    if (ret != ROK)
    {
      con->ccmCb.flags |= CC_MG_SUB_ERROR;
    }

    if (con->ccmCb.mgPend)
    {
      RETVALUE(ROK);
    } 
  }

  if ((ret != ROK) || (con->ccmCb.flags & CC_MG_SUB_ERROR))
  {
    CCLOGERR("Error: %s: ccHandleMgResponse failed\n", __FUNCTION__);
    cleanUpMgQ(con);
    ccSendFailureOutCome(&con->icBcm);
    ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
    RETVALUE(RFAILED);
  }
  else
  {
    ccDisconnectOgLeg(con);
  }

  RETVALUE(ROK);
} /* ccConE11S137 */


/*
*      Fun:   ccConE40S11
*
*      Desc:  Connection state function
*             event - CCE_DISCONNECT_LEG
*             state - CCS_AWTRSPTIMEOUT
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2.c
*
*/
PUBLIC S16 ccConE40S11
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  S16                  ret          = RFAILED;
  ProtType             icProtType   = 0;

  CCDP(AIN_DBG_LEVEL_1, "ccConE40S11: %ld\n", con->icSuConnId);

  icProtType = CC_GET_PROTOCOL(con->icProtType);  /*bug91501*/

  /* BUG:87481 Fix no close issue */
  if ((CC_IS_CALL_TAPPED(con)) && (CC_IS_X3_ALLOWED(con)))
    ccGenerateCcCloseForLi(con);

  if (CC_IS_CONTEXT_PRESENT(con))
  {
    /* BUG80938 + */
    if(CC_IS_CALL_MULTI_MG(con))
    {
      con->ccmCb.mgPend = 2;
      ccUpdateMgQForSubRsc((PTR) NULLP, con->ogBcm.mgCtx->suCtxId, con);
      ccUpdateMgQForSubRsc(con->icAdjBcm, con->icAdjBcm->mgCtx->suCtxId, con);
      CC_SUB_RSC_FOR_CCN5(con);
    }
    else
    {
      /*bug91501*/
      if (((icProtType == CC_SIP) || (icProtType == CC_SIPT)) && (IS_MULTIMEDIA_NO_MEDIA_AWARE(con)))
      {
         CCDP(AIN_DBG_LEVEL_1,"sub all for agnostic case\n");
         ccUpdateMgQForSubRsc((PTR) NULLP, con->mgBlock->mgCtx->suCtxId, con);
      }
      else
        ccUpdateMgQForSubRsc(&(con->ogBcm), con->mgBlock->mgCtx->suCtxId, con);

      /* BUG:87715 */
      if ((CC_IS_CALL_TAPPED(con)) && (CC_IS_X3_ALLOWED(con)) &&
          ((con->ccmCb.ccn == CCN_4) || (con->ccmCb.ccn == CCN_2)))
      {
        ccUpdateMgForSubRscForLi(con);
      }
    }
    /* BUG 80938 - */
    con->lastState = con->state;
    CC_STATE_CHANGE(con, CCS_AWT_MGI_CFM_FOR_DL2);
    con->mgBlock->con = con;
    ret = ccSendMgiQueue(0, con->mgBlock);
    if (ret != ROK)
    {
      CCLOGERR("ERROR: ccSendMgiQueue FAILED : icSu 0x%lx\n",
               con->icSuConnId);
      cleanUpMgQ(con);
      ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
      RETVALUE(ROK);
    }
  }
  else
  {
    ccDisconnectOgLeg(con);
  }
  RETVALUE(ROK);
}

/* 
 *      Fun:   ccDisconnectOgLeg 
 *
 *      Desc:  Split ogBcm to a new con to track the release of the ogBcm
 *             then send success out come to FIC.
 */
#ifdef ANSI
PUBLIC  S16 ccDisconnectOgLeg
(   
CcConCb *con                     /* connection */
)   
#else
PUBLIC S16 ccDisconnectOgLeg(con)
CcConCb *con;                    /* connection */
#endif
{
  SuccessOutcome_t     successOutCome;
  SvcHeader_t          header;
  AinBCM_t         *bcm            = NULLP;
  U8                relCause = CCCALLCLR;
  CcConCb          *discCon  = NULLP;

  if (!con)
  {
    CCLOGERR("%s: NULL con pointer\n", __FUNCTION__);
    RETVALUE(ROK);
  }

  CCDP(AIN_DBG_LEVEL_1, "%s:con - icSu:%ld, ogSu:%ld\n",
        __FUNCTION__,con->icSuConnId, con->ogSuConnId);
  bcm = &con->icBcm;

  if ((bcm->bcmType != BtOBcm) && (bcm->bcmType != BtTBcm))
  {
    CCLOGERR("Invalid bcmType:%d, %ld, %ld\n", bcm->bcmType,
              bcm->svcHeader.transId, bcm->con->icSuConnId);
    RETVALUE(ROK);
  }

  if (con->ainInfo.relCause.eh.pres && con->ainInfo.relCause.causeVal.pres)
  {
     relCause = con->ainInfo.relCause.causeVal.val;
  }

  CCDP(AIN_DBG_LEVEL_0, "%s: Rel Og with RelCause:%d\n",
        __FUNCTION__, relCause);

  /* Bug# 72251 Need to stop the timer here */
  ccStopConTmr(con, CC_TMR_TIMEOUT);
  /* split the ogBcm */
  discCon = ccDisconnectBcmFromCon(con, OUTTOINC);
    
  if (discCon)
  { 
    /* Send RelReq to icBcm of discCon */
    CC_STATE_CHANGE(discCon, CCS_AWTRELCFM_IC);

    discCon->callDtlInfo.cm.callState = LCC_CDR_ANSWERED;
    discCon->acnt = FALSE; /* Don't want to bill this call */
    ccStartConTmr(TMR_RELEASE, discCon, (PTR)&ccCp.genCfg);
    ccSendRelease(discCon, discCon->icSapCb, discCon->icSpConnId,
                  discCon->icSuConnId, discCon->icProtType, relCause, NULL);
  }       


  if (bcm->nel[EReqSuccessOutCome] == NEL_REQUEST)
  {
    /* Send Success OutCome on icBcm */
    CCDP(AIN_DBG_LEVEL_0, "%s: Sending Success OutCome:%ld\n", __FUNCTION__,con->icSuConnId);
    bcm->nel[EReqSuccessOutCome] = NEL_UNARMED;

    con->lastState = con->state;
    CC_STATE_CHANGE(con, CCS_AWT_RSP_FOR_SO);
    /* Send the msg as it is not needed to be buffered */
    ccChangeCallStateOfAssociatedCalls(bcm, CCS_CANNOTPROCESSEVENTS);
    cmMemset((U8 *)&header, 0, sizeof(SvcHeader_t));
    cmMemset((U8 *)&successOutCome, 0, sizeof(SuccessOutcome_t));


    /* Update bcm parameters */
    ccUpdateBcm(con, bcm->pic, bcm->cc->ccId, bcm->bcmType);

    /* Fill in parameters in the Service Header Structure */
    ccFillSvcHeader(&header, bcm, MSG_Success_Outcome);
    ccFillSuccessOutCome(&successOutCome, bcm);
    successOutCome.notInd = FALSE;
    CcUiFctSuccessOutcome(&successOutCome, &header);
    RETVALUE(ROK);
    
  }
  else
  {
    CCLOGERR("SuccessOutcome event is not armed as a request\n");
    ccSendFailureOutCome(&con->icBcm);
    ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
    RETVALUE(ROK);
  }

  RETVALUE(ROK);
} /* End of ccDisconnectOgLeg */
/* FID-14715: Support INAP Operations for FP/PP/VPN with OSP - */

/* 
 *      Fun:   ccConE52S123 
 *
 *      Desc:  CCE_DISCONNECT for CRBT, AWT FIC for I-Disconnect Response
 *             event - CCE_DISCONNECT (52)
 *             state - CCS_AWTIDISCONNECT (123) 
 */
#ifdef ANSI
PUBLIC  S16 ccConE52S123
(   
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                   /* bcm */
PTR     event2                   /* event */
)   
#else
PUBLIC S16 ccConE52S123(con, event, bcm, event2)
CcConCb *con;                    /* connection */
PTR     event;                   /* event */
AinBCM_t *bcm;                   /* bcm */
PTR     event2;                  /* event */
#endif
{
  if ((con == NULLP) || (event == NULLP))
  {
    CCLOGERR("Critical ERROR: Null pointers:%lx, %ld\n", (U32)con, event);
    RETVALUE(ROK);
  }

  CCDP(AIN_DBG_LEVEL_1, "%s: %ld\n", __FUNCTION__,con->icSuConnId);

  ccDisconnectCallForIcBCM(con, event);

  RETVALUE(ROK);

} /* End of ccConE52S123 */

/* 
 *      Fun:   ccConE39S123 
 *
 *      Desc:  CCE_CONTINUE, AWT FIC for I-Disconnect Response
 *             event - CCE_CONTINUE (39)
 *             state - CCS_AWTIDISCONNECT (123) 
 */
#ifdef ANSI
PUBLIC  S16 ccConE39S123
(   
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                   /* bcm */
PTR     event2                   /* event */
)   
#else
PUBLIC S16 ccConE39S123(con, event, bcm, event2)
CcConCb *con;                    /* connection */
PTR     event;                   /* event */
AinBCM_t *bcm;                   /* bcm */
PTR     event2;                  /* event */
#endif
{

  Continue_t        *pContinue      = NULLP;

  pContinue   = (Continue_t *) event2;

  if (con == NULLP)
  {
    CCLOGERR("Critical ERROR: Null pointer:%lx\n", (U32)con);
    RETVALUE(ROK);
  }

  CCDP(AIN_DBG_LEVEL_1, "%s entered with errorCode %ld, icSuConnId: %ld\n",
       __FUNCTION__,pContinue->errorCode,con->icSuConnId);

  if (pContinue->errorCode != FC_NOT_APPLICABLE)
  {

    /* FID: 14457.0 + */
    if ((ccCp.getsEnabled) && (IS_THIS_CALL_GETS(con)) &&
        (pContinue->errorCode == FC_HPC_QUEUE_TIMEOUT))
    {
      ccCp.getsSts.queuedTimeoutTx++;
    } /* FID 14457.0 - */

#if 1
    if (!(con->assocBlock))
    { /* BUG# 62478:: CONTINUE received on Con1 of the CRBT call. 
         Play the treatment to the incoming leg */
      ccStopConTmr(con, TMR_RELEASE);
#if 0 /* No need to start the setup timer as it is already running */
      ccStartConTmr(TMR_SETUP, con, (PTR)&ccCp.genCfg);
#endif
      ccStopConTmr(con, CC_TMR_PREANM);
      ccProcessFailCnd(con, pContinue->errorCode, TRUE);
      RETVALUE(ROK);
    }
    else
    {
        /* Stop the CC_TMR_PREANM timer if it is started */
        ccStopConTmr(con, CC_TMR_PREANM);
        CCLOGERR("ERROR::Continue with Error on Con2 for CRBT is not VALID\n");
    }
#endif
  }

  ccHandleCleanUpConCb(con,TRUE,CCTMPFAIL);

  RETVALUE(ROK);

} /* End of ccConE39S123 */

/* 
 *      Fun:   ccConE39S124 
 *
 *      Desc:  CCE_CONTINUE, AWT FIC for O-TermSeized Response
 *             event - CCE_CONTINUE (39)
 *             state - CCS_AWTRSPFORDPE7 (124) 
 */
#ifdef ANSI
PUBLIC  S16 ccConE39S124
(   
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                   /* bcm */
PTR     event2                   /* event */
)   
#else
PUBLIC S16 ccConE39S124(con, event, bcm, event2)
CcConCb *con;                    /* connection */
PTR     event;                   /* event */
AinBCM_t *bcm;                   /* bcm */
PTR     event2;                  /* event */
#endif
{
  S16               ret          = RFAILED;
  U8                mgiAction;
  U8                icMode;
  U8                ogMode;
  U8                icSignal;
  U8                ogSignal;
  Continue_t        *pContinue      = NULLP; /* FID 14234.0 */
  SendApm_t         sendApm;                 /* FID 14234.0 */

  if (con == NULLP)
  {
    CCLOGERR("Critical ERROR: Null pointer:%lx\n", (U32)con);
    RETVALUE(ROK);
  }

  CCDP(AIN_DBG_LEVEL_1, "%s: %ld\n", __FUNCTION__,con->icSuConnId);

  /* Begin FID 14234.0 */
  pContinue = (Continue_t *) event2;

  if(pContinue && FCT_IS_CONTACT_SEND_APM(pContinue->action))
  {
      CCDP(AIN_DBG_LEVEL_1, "Populate one SENDAPM event and send out.\n");
      cmMemset((U8 *)&sendApm, 0, sizeof(SendApm_t));

      cmMemcpy((U8 *)&sendApm.app, (U8 *)&pContinue->app, sizeof(SiAppTrans));
      sendApm.direction = pContinue->apmDirection;

      ccSendApmReq(con, &sendApm);
  }
  /* End of FID 14234.0 */
 

  /* Expect an AssocBlock to be present with Continue in this state
   * FID 14234 doens't care about AssocBlock.
   */
  if (((!FCT_IS_CONTACT_INTW_ACM(pContinue->action)) && 
       (!FCT_IS_CONTACT_SEND_APM(pContinue->action)) && 
       (!FCT_IS_CONTACT_HOLD_ACM(pContinue->action))) && 
        !CC_IS_ASSOCBLOCK_PRESENT(con))
  {
    CCLOGERR("Critical ERROR: Null pointer for assocblock\n");
    ABORT_DEBUG;
    ccHandleCleanUpConCb(con,TRUE,CCTMPFAIL);
    RETVALUE(ROK);
  }

  con->lastState = con->state;
  CC_STATE_CHANGE(con, CCS_AWTANS);

  mgiAction = ccFindSwitchingAction(con, CCE_CNSTIND, con->icEvntType,
              con->direction, &con->icBcm, &con->ogBcm, &icMode, &ogMode,
              &icSignal, &ogSignal, (PTR)con->ccEvntHold, TRUE);

  CCDP(AIN_DBG_LEVEL_3, "%s: Action == %d, icMode == %d, icSignal == %d,"
      " ogMode == %d, ogSignal == %d\n", __FUNCTION__,mgiAction, icMode, icSignal,
       ogMode, ogSignal);

  if(mgiAction != CC_NO_MGI_ACTION)
  {
      ret=ccProcessOAlertingWithSwitching(con, (PTR)con->ccEvntHold);
      if (ret!=ROK)
         RETVALUE(RFAILED);
  }
  else if (NMBRCATEGORY_NOT_IVR(con))
  {
      ret=ccProcessOAlertingNoSwitching(con,(PTR)con->ccEvntHold);
      if (ret!=ROK)
         RETVALUE(RFAILED);
  }
  RETVALUE(ROK);

} /* End of ccConE39S124 */


/* 
 *      Fun:   ccConE37S123 
 *
 *      Desc:  Analyze Route for CRBT, AWT FIC for I-Disconnect Response
 *             event - CCE_ANALYZE_ROUTE (37)
 *             state - CCS_AWTIDISCONNECT (123) 
 */
#ifdef ANSI
PUBLIC  S16 ccConE37S123
(   
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                   /* bcm */
PTR     event2                   /* event */
)   
#else
PUBLIC S16 ccConE37S123(con, event, bcm, event2)
CcConCb *con;                    /* connection */
PTR     event;                   /* event */
AinBCM_t *bcm;                   /* bcm */
PTR     event2;                  /* event */
#endif
{
  S16                 ret                 = RFAILED;

  if ((con == NULLP) || (event == NULLP))
  {
    CCLOGERR("Critical ERROR: Null pointers:%lx, %ld\n", (U32)con, event);
    RETVALUE(RFAILED);
  }

  CCDP(AIN_DBG_LEVEL_1, "%s:icSu:%ld, ogSu:%ld, suCtxId:%ld\n",
       __FUNCTION__,con->icSuConnId, con->ogSuConnId, (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0));

  CC_CLR_CALL_FLAG(con->flag, CC_FLAG_CALLQUEUED); /* FID 14457.0 */

  ret=ccProcessAR(con, event, bcm, event2);
  if (ret != ROK)
     RETVALUE(RFAILED);

  RETVALUE(ROK);
} /* End of ccConE37S123 */

/* 
 *      Fun:   ccConE60S95 
 *
 *      Desc:  Ignore from FIC, Awaiting dialing completion
 *             event - CCE_IGNOREFROMFIC (60)
 *             state - CCS_AWTRSPFORDPE08 (95) 
 */
#ifdef ANSI
PUBLIC  S16 ccConE60S95
(   
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                   /* bcm */
PTR     event2                   /* event */
)   
#else
PUBLIC S16 ccConE60S95(con, event, bcm, event2)
CcConCb *con;                    /* connection */
PTR     event;                   /* event */
AinBCM_t *bcm;                   /* bcm */
PTR     event2;                  /* event */
#endif
{
  S16               ret            = RFAILED;
  IgnoreEvent_t     *pIgnoreEvent  = NULLP;
  U8                tmrNum =0;
  Bool              allDtlTmd = FALSE, allDtlStatsTmd = FALSE;
  ProtType          icProtType;


  if ((con == NULLP) || (event == NULLP) || (event2 == NULLP))
  {
    CCLOGERR("Critical ERROR: Null pointers:%lx, %ld, %ld\n", (U32)con, event,event2);
    RETVALUE(ret);
  }

  pIgnoreEvent=(IgnoreEvent_t *)event2;
  icProtType = CC_GET_PROTOCOL(con->icProtType);

  CCDP(AIN_DBG_LEVEL_1, "%s:icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", __FUNCTION__,con->icSuConnId, con->ogSuConnId,
       (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0), con->icBcm.svcHeader.transId);

  CCDP(AIN_DBG_LEVEL_1, "%s:sendBackwardMsg:%d, isACMSent:%d, icProtType:0x%x, IS_SDPSENTONICBCM:%d,\n", 
       __FUNCTION__,pIgnoreEvent->sendBackwardMsg, con->isACMSent, icProtType, IS_SDPSENTONICBCM(con));


  /* Send backward message for announcement if instructed by FIC */
  if (pIgnoreEvent->sendBackwardMsg &&
     ((!con->isACMSent) || (icProtType == CC_SI) ||
      (((icProtType == CC_SIP) || (icProtType == CC_SIPT)) && 
        ((CC_IS_CONTEXT_PRESENT(con))?(con->icIpParam.locDesc != NULLP):(!IS_SDPSENTONICBCM(con))))))
      /* BUG: 57523, Send 183 with SDP for Ingress SIP/SIPT case to cut off ringing, 
       * where 180 was sent previously, or send CPG with inband info avail for ISUP
       */
  {
      ret = ccSendBackwardMessageForAnnoucement(con, con->ainInfo.Resourcetype.val, FALSE, TRUE);
      if (ret != ROK)
      {
          CCLOGERR("%s: Error, ccSendBackwardMessageForAnnoucement failed icSu=0x%lx\n",
                    __FUNCTION__,con->icSuConnId);
          ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
          RETVALUE(ret);
       }
  }
  ret=RFAILED; /* re-init ret to RFAILED */

  /* Only generate billing info if instructed by FIC */
  if (pIgnoreEvent->genBillingInfo)
  {
      for (tmrNum = 0; tmrNum < MAXCCCONCBTIMER; tmrNum++)
      {
           if (con->timers[tmrNum].tmrEvnt == CC_TMR_CALLDTL)
              allDtlTmd = TRUE;
           if (con->timers[tmrNum].tmrEvnt == CC_TMR_CALLDTL_STATS)
              allDtlStatsTmd = TRUE;
      }
      CCDP(AIN_DBG_LEVEL_3, "allDtlTmd = %d, allDtlStatsTmd = %d;\n",
           allDtlTmd, allDtlStatsTmd);
      if (!allDtlTmd)
          ccStartConTmr(CC_TMR_CALLDTL, con, (PTR)&ccCp.genCfg);
      if (!allDtlStatsTmd)
          ccStartConTmr(CC_TMR_CALLDTL_STATS, con, (PTR)&ccCp.genCfg);
      CCDP(AIN_DBG_LEVEL_3, "icBcm.nel[EReqTimeout] =  %d, timeout.pres =%d,"
           "val = %ld\n", con->icBcm.nel[EReqTimeout], con->ainInfo.TimeoutTimer.pres,
            (long)con->ainInfo.TimeoutTimer.val);
    
      (Void) SGetSysTime(&con->cdrStartTick);
    
      /* BD long duration call */
      con->callDtlInfo.cm.tckStartLongDur.pres = PRSNT_NODEF;
      (Void) SGetSysTime(&con->callDtlInfo.cm.tckStartLongDur.val);
    
      if ((con->icProtType != CC_LOCAL_SWT_PROT) &&
          (con->icIntfcCb != NULLP) && (con->ogIntfcCb != NULLP))
      {
         con->acnt = con->icIntfcCb->acnt | con->ogIntfcCb->acnt;
      }
      /* rkhan, bug#28856 */
      else if ((con->icBcm.cc != NULLP) && (con->icBcm.cc->ccId == CiTransfer)
               && (con->ogIntfcCb != NULLP))
      {
         con->acnt = con->ogIntfcCb->acnt;
      }
    
      if (ccInit.acnt == TRUE)
      {
         if ((con->callDtlInfo.cm.tckOgConCfmRcvd.pres == NOTPRSNT)&&
             (con->keepGCCCallInfo == FALSE))
         {
              con->callDtlInfo.cm.tckOgConCfmRcvd.pres = PRSNT_NODEF;
              (Void) SGetSysTime(&con->callDtlInfo.cm.tckOgConCfmRcvd.val);
         }
         if (con->keepGCCCallInfo == FALSE)
         {
         con->callDtlInfo.cm.callState = LCC_CDR_ANSWERED;  /* Bug 59032 */
       }
       }
       
       ccUpdateBillingInfo (con);

       if(ccBillingSystem.genStartRecord)
       {
          CCDP(AIN_DBG_LEVEL_0, "genStartRecord and genBillingInfo is set to YES, \n");
          CC_GEN_BILLING (con, LCC_CDR_CALLINPROGRESS, C_CDRGEN_CAUSE_SETUP);
       }
       else
          CCDP(AIN_DBG_LEVEL_0, "genStartRecord is set to NO \n");
 
   }
   CC_STATE_CHANGE(con, CCS_AWTANS); 
   RETVALUE(ROK);
} /* End of ccConE60S95 */


/* 
 *      Fun:   ccProcessAR 
 *
 *      Desc:  Analayze Route Processing 
 *             event - CCE_ANALYZE_ROUTE 
 *             state - CCS_AWTANS or CCS_AWTIDISCONNECT
 */
#ifdef ANSI
PUBLIC  S16 ccProcessAR
(   
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                   /* bcm */
PTR     event2                   /* event */
)   
#else
PUBLIC S16 ccProcessAR(con, event, bcm, event2)
CcConCb *con;                    /* connection */
PTR     event;                   /* event */
AinBCM_t *bcm;                   /* bcm */
PTR     event2;                  /* event */
#endif
{
  SvcHeader_t        *arSvcHeader           = NULLP;
  AnalyzeRoute_t     *pAnalyzeRoute         = NULLP;
  U8                  relDisBoolOG          = RFAILED;
  U8                  evntType              = 0;
  S16                 mgret                 = RFAILED;

  /*CID 15597*/
  if ((con == NULLP) || (event == NULLP))
  {
    CCLOGERR("Critical ERROR: Null pointers:%lx, %ld\n", (U32)con, event);
    RETVALUE(RFAILED);
  }

  /* Feature 1789 - German ISUP */
  /* stop the Toiw2 Timer if running */
  ccStopConTmr(con, CC_TMR_TOIW2);

 
  /* Stop the CC_TMR_PREANM timer if it is started */
  ccStopConTmr(con, CC_TMR_PREANM);

  CCDP(AIN_DBG_LEVEL_1, "%s:icSu:%ld, ogSu:%ld, suCtxId:%ld\n",
       __FUNCTION__,con->icSuConnId, con->ogSuConnId, (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0));

  pAnalyzeRoute = (AnalyzeRoute_t *) event2;
  arSvcHeader = (SvcHeader_t *) event;

  ccCutBillingRecordIfNeeded(con, pAnalyzeRoute->scpResponse, OUTTOINC);
  ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));

  if (CC_IS_CONTEXT_PRESENT(con))
  {
     /* BUG: 85554 Release cause has to be set prior to ccExtractAnalyzeRteParam() 
      * so that generated X2 signaling message IRI_END, 
      * doesn't use a default release cause
      */
     con->tempHolderForReleaseCause = CCCALLCLR;
  }

  ccExtractAnalyzeRteParam(pAnalyzeRoute, bcm);
  ccExtractSvcHeader(arSvcHeader, bcm, MSG_Analyze_Route);

  ccStopConTmr(con, AIN_O_NO_ANS_TMR);
  ccStopConTmr(con, AIN_T_NO_ANS_TMR);
  /* STOP the AWAIT ANSWER Timer if running
   *
   * changes made to Support Configurable Answer Timer in GCC, RV Suresh, 5.0 
   */
  ccStopConTmr(con, CC_TMR_AWAITANS); 
  ccDeleteBufferedEvent(con, con->icSuConnId, CCE_ONOANSEXP);
  ccDeleteBufferedEvent(con, con->ogSuConnId, CCE_TNOANSEXP);

  /* Re-Start the Set-up timer */
  ccStopConTmr(con, TMR_SETUP);
  ccStartConTmr(TMR_SETUP, con, (PTR)&ccCp.genCfg);

  if (arSvcHeader->lastTransactionFlag == TRUE)
    ccDeleteBcm(&(con->icBcm));

  con->ogBcm.closeCause = CscRmtTscTimeout;
  if ((CC_IS_CONTEXT_PRESENT(con)) && (con->ogRscVal))
  {
    ccQueueSubOnOGCleanUp(con);
    con->relPend = OUTRELANDROUTE;
    CC_STATE_CHANGE(con, CCS_AWTSWTCFMFORDP);
    ccChangeCallStateOfAssociatedCalls(&(con->icBcm), CCS_CANNOTPROCESSEVENTS);
    con->mgBlock->con = con;
    mgret = ccSendMgiQueue(0, con->mgBlock);
    if (mgret == ROK)
      RETVALUE(ROK);
    else
    {
      CCLOGERR("Error: MGI failed:icSu:%ld, ogSu:%ld, suCtxId:%ld,"
               " icTransId:%ld\n", con->icSuConnId, con->ogSuConnId,
               con->mgBlock->mgCtx->suCtxId, con->icBcm.svcHeader.transId);
      ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
      cleanUpMgQ(con);
      ccHandleCleanUpConCb(con, TRUE, AM_CV_TMPFAIL);
      RETVALUE(ROK);
    }
  }

  /* Clean outgoing and ReRoute call */
  if (con->ogRscVal)
  {
    relDisBoolOG = ccCheckRelOrOnHookInBuffer(&(con->ogBcm), &evntType);
    if (relDisBoolOG == ROK)
    {
      ccCloseTBcmTransaction(con);
      con->direction = OUTTOINC;
      if (con->noRspReqd == FALSE) ccSendReleaseRsp(con);
      ccDeallocateResource(con, CC_OUTGOING);
      con->ogRscVal = FALSE;
      initOgBcm(&con->ogBcm);
      ccRouteCall(con, FALSE);
    }
    else
    {
      ccFreeTheRscFromCon(con, OUTTOINC); /* This will take care of REL_CFM */
      initOgBcm(&con->ogBcm);
      if(ROK == ccCheckForInterfaces(con))
        ccRouteCall(con, FALSE);
      else
      {
        CC_TRACE_DUMP(con);
        ccProcessFailCnd(con, FC_INTERNAL, TRUE);
      }
    }
    RETVALUE(ROK);
  }
  else
  {
    /* Route the call */
    CCLOGERR("Error: Unexpected condition, still going ahead:%ld\n",
             con->icSuConnId);
    initOgBcm(&con->ogBcm);
    ccRouteCall(con, FALSE);
    RETVALUE(ROK);
  }
  RETVALUE(ROK);
} /* End of ccProcessAR */

/* 
 *      Fun:   ccProcessSwapRequest 
 *
 *      Desc:  Move Resource Processing CCE_SWAPCALL 
 *             con1 -> con2
 *             con1 - Represents the source CcConCb
 *             con2 - Represents the destination CcConCb 
 */
#ifdef ANSI
PUBLIC  S16 ccProcessSwapRequest
(   
CcConCb *con1,                    /* connection */
CcConCb *con2                     /* connection */
)   
#else
PUBLIC S16 ccProcessSwapRequest(con1, con2)
CcConCb *con1;                    /* connection */
CcConCb *con2;                    /* connection */
#endif
{
  S16               ret            = RFAILED;

  if ((con1 == NULLP) || (con2 == NULLP))
  {
    CCLOGERR("Critical ERROR: Null pointers:%lx, %lx\n", (U32)con1, (U32)con1);
    ABORT_DEBUG;
    if (con1 != NULLP)
        ccHandleCleanUpConCb(con1,TRUE,CCTMPFAIL);
    if (con2 != NULLP)
        ccHandleCleanUpConCb(con2,TRUE,CCTMPFAIL);
    RETVALUE(RFAILED);
  }

  CCDP(AIN_DBG_LEVEL_1, "%s:con1 - icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", __FUNCTION__,con1->icSuConnId, con1->ogSuConnId,
       (con1->mgBlock->mgCtx ? con1->mgBlock->mgCtx->suCtxId : 0), con1->icBcm.svcHeader.transId);

  CCDP(AIN_DBG_LEVEL_1, "%s:con2 - icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", __FUNCTION__,con2->icSuConnId, con2->ogSuConnId,
       (con2->mgBlock->mgCtx ? con2->mgBlock->mgCtx->suCtxId : 0), con2->icBcm.svcHeader.transId);

 
  /* Put con1 in state 68-CCS_CANNOTPROCESSEVENTS 
   */

  con1->lastState = con1->state;
  CC_STATE_CHANGE(con1, CCS_CANNOTPROCESSEVENTS);

  /* Copy remoteSdpSentOnIcBCM from con1 to con2 */
  con2->remoteSdpSentOnIcBCM = con1->remoteSdpSentOnIcBCM;

  if (CC_IS_CONTEXT_PRESENT(con1) && CC_IS_CONTEXT_PRESENT(con2)) 
  {
     /* Case 1, context present in con1 and con2, move operation is possible.
      * Move A from con1 to con2
      */
      U32 SsuCtxId;
      U32 DsuCtxId;

      if (con1->cacCb)
      {
        if (!con2->cacCb)
        {
          S16  ret          = RFAILED;
      
          ret = SGetSBuf(ccInit.region, ccInit.pool, (Data **) &(con2->cacCb),
                               (Size) sizeof(CcCacCb));
          if (ret != ROK)
          {
            CCLOGERR("**%s(): OUT of MEMORY origIcSuConnId=%ld ****\n",
                           __FUNCTION__, con2->origIcSuConnId);
            RETVALUE(RFAILED);
          }
          cmMemset((U8 *) (con2->cacCb), 0, sizeof(CcCacCb));
        }
        con2->cacCb->icswtchComCodec = con1->cacCb->icswtchComCodec;
        con2->cacCb->icNeedCACUpdate = con1->cacCb->icNeedCACUpdate;
        con2->cacCb->IcRealmCACCheckDone = con1->cacCb->IcRealmCACCheckDone;
        con2->cacCb->IngressRealmIdInBICC = con1->cacCb->IngressRealmIdInBICC;
        con2->cacCb->CurrentIcCodec = con1->cacCb->CurrentIcCodec;
      }

      SsuCtxId = (con1->icBcm.mgCtx ? con1->icBcm.mgCtx->suCtxId : 0);
      if (CC_IS_CALL_MULTI_MG(con2))
        DsuCtxId = (con2->icAdjBcm->mgCtx ? con2->icAdjBcm->mgCtx->suCtxId : 0);
      else
        DsuCtxId = (con2->ogBcm.mgCtx ? con2->ogBcm.mgCtx->suCtxId : 0);

      ccUpdateMgQForMoveRsc(MGCT_MODE_SENDRECV, NOTPRSNT, 0, TGP_NLP_MAX,
                            TGP_EC_MAX, &(con1->icBcm), &(con2->icBcm),
                            SsuCtxId, DsuCtxId,
                            con1, con2, NULLP,
                            NULLP,
                            CCDESTINATION, MGCT_ITEM_ID_CG_NULL);

      con2->lastState = con2->state;
      CC_STATE_CHANGE(con2, CCS_AWTTMGCTFORSWAPCALL);
      con2->mgBlock->con = con2;
      ret = ccSendMgiQueue(DsuCtxId, con2->mgBlock);
  }
  else if (!CC_IS_CONTEXT_PRESENT(con1) && CC_IS_CONTEXT_PRESENT(con2)) 
  {
      /* All IP, There is no SFEP 
       * Add A of CON2  
       */
      ProtType   icProtType;
      U32  suCtxId;
      U32  contextId;

      icProtType = CC_GET_PROTOCOL(con2->icProtType);
      
      if (((icProtType == CC_SIP)||
           (icProtType == CC_SIPT)||
           ((icProtType == CC_BICC)&&
            (con2->cacCb)&&
            (con2->cacCb->IngressRealmIdInBICC)))&&
            ((CC_IS_MG_NEEDED_IN_IP_CALL(con2))||
             (!ccIsIPResource(&con2->ogBcm)))&&
          (con2->callDtlInfo.cm.ingressRealmId))
      {
        if (!con2->cacCb)
        {
          S16  ret          = RFAILED;
        
          ret = SGetSBuf(ccInit.region, ccInit.pool, (Data **) &(con2->cacCb),
                               (Size) sizeof(CcCacCb));
          if (ret != ROK)
          {
            CCLOGERR("**%s(): OUT of MEMORY origIcSuConnId=%ld ****\n",
                           __FUNCTION__, con2->origIcSuConnId);
            RETVALUE(RFAILED);
          }
          cmMemset((U8 *) (con2->cacCb), 0, sizeof(CcCacCb));
        }
        
        con2->cacCb->icNeedCACUpdate = TRUE;
        con2->cacCb->CurrentIcCodec.val = 0;
        con2->cacCb->CurrentIcCodec.pres = NOTPRSNT;
      }

      if (CC_IS_CALL_MULTI_MG(con2))
      {
        suCtxId = (con2->icAdjBcm->mgCtx ? con2->icAdjBcm->mgCtx->suCtxId : 0);
        contextId = (con2->icAdjBcm->mgCtx ? con2->icAdjBcm->mgCtx->contextId : 0);
      }
      else
      {
        suCtxId = (con2->ogBcm.mgCtx ? con2->ogBcm.mgCtx->suCtxId : 0);
        contextId = (con2->ogBcm.mgCtx ? con2->ogBcm.mgCtx->contextId : 0);
      }
 
      ccUpdateMgQForAddOrModifyRsc(MGCT_MODE_SENDRECV, MGCT_PACKAGE_ID_MAX,
                                   MGCT_ITEM_ID_CG_MAX, MGCT_CMD_ADD,
                                   &(con2->icBcm), NULLP, NULLP,
                                   NULLP, NULLP, NULLP,
                                   suCtxId, contextId, con2);
      con2->renegSdpAfterAns = TRUE;
      con2->lastState = con2->state;
      CC_STATE_CHANGE(con2, CCS_AWTTMGCTFORSWAPCALL);
      con2->mgBlock->con = con2;
      ret = ccSendMgiQueue(suCtxId, con2->mgBlock);
  }
  else if (CC_IS_CONTEXT_PRESENT(con1) && !CC_IS_CONTEXT_PRESENT(con2)) 
  {
#if 0 /*This is possible, refer to Bug#86824*/
      /* Don't belive this case is possible, but want to catch it */
      ABORT_DEBUG;
#endif
      ccHandleCleanUpConCb(con1,TRUE,CCTMPFAIL);
      ccHandleCleanUpConCb(con2,TRUE,CCTMPFAIL);
      RETVALUE(ROK);
  }
  else
  {
      /* (!CC_IS_CONTEXT_PRESENT(con1) && !CC_IS_CONTEXT_PRESENT(con2)) */
      /* Complete processing of the SWAPCALL based on Swap Type
       */
      ccProcessSwapType(con2);
      RETVALUE(ROK);
  }

  /* There was an error with ccSendMgiQueue(), release con2 */
  if (ret != ROK)
  {
     CCLOGERR("Error: ccSendMgiQueue failed\n");
     cleanUpMgQ(con2);
     ccProcessSwapType(con2);
  }
  RETVALUE(ROK);
} /* End of ccProcessSwapRequest */

/* 
 *      Fun:   ccProcessSwapType 
 *
 *      Desc:  Complete processing of the SWAPCALL based on Swap Type 
 *             con    - Represents the Cb that received the SWAPCALL request
 */
#ifdef ANSI
PUBLIC  S16 ccProcessSwapType
(   
CcConCb *con2                     /* connection */
)   
#else
PUBLIC S16 ccProcessSwapType(con2)
CcConCb *con2;                    /* connection */
#endif
{
  S16               ret            = RFAILED;
  CcConCb           *con1          = NULLP;
  U8                mgiAction;
  U8                icMode;
  U8                ogMode;
  U8                icSignal;
  U8                ogSignal;
  UConnId           tempSuConnId   = 0;



  CCDP(AIN_DBG_LEVEL_1, "%s:con2 - icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", __FUNCTION__,con2->icSuConnId, con2->ogSuConnId,
       (con2->mgBlock->mgCtx ? con2->mgBlock->mgCtx->suCtxId : 0), con2->icBcm.svcHeader.transId);

  
  /* Check the swapType that is stored in the assocBlock
   */
   if (CC_IS_ASSOCBLOCK_PRESENT(con2))
   {

      /* Get associated call */
      /* Find Associated Connection Block based on assocTransId provided from FIC */
      ret = cmHashListFind(&ccSuInstTbl,(U8 *)&(CC_GET_ASSOCPARENTSUCONNID(con2)),
                          (U16)sizeof(UConnId), 0, (PTR *)&con1);
      if (ret != ROK)
      {
          CCLOGERR("%s - ERROR: Connection Block Not Found %lx\n", __FUNCTION__,CC_GET_ASSOCPARENTSUCONNID(con2));
          ABORT_DEBUG;
          ccHandleCleanUpConCb(con2,TRUE,CCTMPFAIL);
      }
      CCDP(AIN_DBG_LEVEL_1, "%s:con1 - icSu:%ld, ogSu:%ld, suCtxId:%ld,"
           " icTransId:%ld\n", __FUNCTION__,con1->icSuConnId, con1->ogSuConnId,
           (con1->mgBlock->mgCtx ? con1->mgBlock->mgCtx->suCtxId : 0), con1->icBcm.svcHeader.transId);

      con2->icSapCb         = con1->icSapCb;
      con2->uBuf            = con1->uBuf;
      con1->uBuf            = NULL; 
      ret = cmHashListDelete(&con1->icSapCb->spInstTbl, (PTR) con1);
      if (ret != ROK)
      {
         CCLOGERR("cmHashListDelete fail spInstTbl, %ld\n", con1->icSpConnId);
         ABORT_DEBUG;
         ccHandleCleanUpConCb(con1,TRUE,CCTMPFAIL);
         ccHandleCleanUpConCb(con2,TRUE,CCTMPFAIL);
         RETVALUE(ROK);
      }
      con2->icSpConnId      = con1->icSpConnId;
      con1->icSpConnId      = 0;
      ret = cmHashListInsert(&con2->icSapCb->spInstTbl, (PTR) con2,
                         (U8 *)&con2->icSpConnId, (U16) sizeof(UConnId));
      if (ret != ROK)
      {
         CCLOGERR("cmHashListInsert fail spInstTbl, %ld\n", con2->icSpConnId);
         ABORT_DEBUG;
         ccHandleCleanUpConCb(con1,TRUE,CCTMPFAIL);
         ccHandleCleanUpConCb(con2,TRUE,CCTMPFAIL);
         RETVALUE(ROK);
      }

      con1->icRscVal = FALSE;
      con2->icRscVal = TRUE;

      /* Move the local SDP */
      cmStoreSdp(ccInit.region, ccInit.pool, &con2->icIpParam.locDesc,
                 con1->icIpParam.locDesc);

      ret = cmHashListDelete(&ccSuInstTbl, (PTR) con1);
      if (ret != ROK)
      {
         CCLOGERR("cmHashListDelete fail ccSuInstTbl, %ld\n", con1->icSuConnId);
         ABORT_DEBUG;
         ccHandleCleanUpConCb(con1,TRUE,CCTMPFAIL);
         ccHandleCleanUpConCb(con2,TRUE,CCTMPFAIL);
         RETVALUE(ROK);
      }

      ret = cmHashListDelete(&ccSuInstTbl, (PTR) con2);
      if (ret != ROK)
      {
         CCLOGERR("cmHashListDelete fail ccSuInstTbl, %ld\n", con2->icSuConnId);
         ABORT_DEBUG;
         ccHandleCleanUpConCb(con1,TRUE,CCTMPFAIL);
         ccHandleCleanUpConCb(con2,TRUE,CCTMPFAIL);
         RETVALUE(ROK);
      }
 
      tempSuConnId          = con2->icSuConnId;
      con2->icSuConnId      = con1->icSuConnId;
      con1->icSuConnId      = tempSuConnId;
 
      ret = cmHashListInsert(&ccSuInstTbl, (PTR)con1, (U8 *)&con1->icSuConnId,
                            (U16)sizeof(UConnId));
      if (ret != ROK)
      {
         CCLOGERR("cmHashListInsert fail ccSuInstTbl, %ld\n", con1->icSuConnId);
         ABORT_DEBUG;
         ccHandleCleanUpConCb(con1,TRUE,CCTMPFAIL);
         ccHandleCleanUpConCb(con2,TRUE,CCTMPFAIL);
         RETVALUE(ROK);
      }
 
      ret = cmHashListInsert(&ccSuInstTbl, (PTR)con2, (U8 *)&con2->icSuConnId,
                            (U16)sizeof(UConnId));
      if (ret != ROK)
      {
         CCLOGERR("cmHashListInsert fail ccSuInstTbl, %ld\n", con2->icSuConnId);
         ABORT_DEBUG;
         ccHandleCleanUpConCb(con1,TRUE,CCTMPFAIL);
         ccHandleCleanUpConCb(con2,TRUE,CCTMPFAIL);
         RETVALUE(ROK);
      }
 
      CCDP(AIN_DBG_LEVEL_1, "%s:con1 Update - icSu:%ld, ogSu:%ld, suCtxId:%ld,"
           " icTransId:%ld\n", __FUNCTION__,con1->icSuConnId, con1->ogSuConnId,
           (con1->mgBlock->mgCtx ? con1->mgBlock->mgCtx->suCtxId : 0), con1->icBcm.svcHeader.transId);
      CCDP(AIN_DBG_LEVEL_1, "%s:con2 Update - icSu:%ld, ogSu:%ld, suCtxId:%ld,"
           " icTransId:%ld\n", __FUNCTION__,con2->icSuConnId, con2->ogSuConnId,
           (con2->mgBlock->mgCtx ? con2->mgBlock->mgCtx->suCtxId : 0), con2->icBcm.svcHeader.transId);

      if (CC_IS_SWAPTYPE_OANSWER(con2))
      {
         /* Swap type is OAnswer, Answer con2 call
          * State is changed to CCS_ANSWERED in ccAnswerCall
          */
          ccAnswerCall(con2, (PTR)con2->ccEvntHold, NULLP);
      }
      else
      {
          /* Swap type must be OTermSeized */

          /* Make sure we don't send ACM again if already sent 
           * on con1 for OTermSeized case 
           */
          CCDP(AIN_DBG_LEVEL_1, "%s: con1 isACMSent=%d,con2 isACMSent=%d\n", __FUNCTION__,con1->isACMSent, con2->isACMSent);
          con2->isACMSent = con1->isACMSent;

         /*
          * Restore state of con2 to CCS_AWTANS 
          */
          con2->lastState = con2->state;
          CC_STATE_CHANGE(con2, CCS_AWTANS);

          mgiAction = ccFindSwitchingAction(con2, CCE_CNSTIND, con2->icEvntType,
              con2->direction, &con2->icBcm, &con2->ogBcm, &icMode, &ogMode,
              &icSignal, &ogSignal, (PTR)con2->ccEvntHold, TRUE);

          CCDP(AIN_DBG_LEVEL_3, "%s: Action == %d, icMode == %d, icSignal == %d,"
              " ogMode == %d, ogSignal == %d\n", __FUNCTION__,mgiAction, icMode, icSignal,
              ogMode, ogSignal);

          if(mgiAction != CC_NO_MGI_ACTION)
          {
              ret=ccProcessOAlertingWithSwitching(con2, (PTR)con2->ccEvntHold);
              if (ret!=ROK)
                 RETVALUE(RFAILED);
          }
          else if (NMBRCATEGORY_NOT_IVR(con2))
          {
              ret=ccProcessOAlertingNoSwitching(con2,(PTR)con2->ccEvntHold);
              if (ret!=ROK)
                 RETVALUE(RFAILED);
          }

      }
      /* Cleanup unwanted buffered events. These events can be ignored since
       * call is going to be release.
       */
       ccDeleteBufferedEvent(con1, 0, CCE_MGCTNOTIFY);
       ccDeleteBufferedEvent(con1, 0, CCE_LOOPBACKACK);
       
      /* Kick off generic release process for con1 call 
       */
      CCDP(AIN_DBG_LEVEL_0, "Cleaning up the call: 0x%lx,0x%lx\n", con1->icSuConnId,
           con1->ogSuConnId);
      ccSendReleasesOnBothSidesWithSwitching(con1, CCNORMUNSPEC);

      /* Safe to kill the assocBlock */
      ccFreeConAssocBlock(con2);

      RETVALUE(ROK);
   }
   else
   {
      CCLOGERR("Critical ERROR: Null pointer for assocblock\n");
      ABORT_DEBUG;
      ccHandleCleanUpConCb(con2,TRUE,CCTMPFAIL);
      RETVALUE(ROK);
    } 

  RETVALUE(ROK);
} /* End of ccProcessSwapType */

/* 
 *      Fun:   ccProcessMoveRequest 
 *
 *      Desc:  Move Resource Processing CCE_MOVECALL
 *             con1 -> con2
 *             con1 - Represents the source CcConCb
 *             con2 - Represents the destination CcConCb 
 */
#ifdef ANSI
PUBLIC  S16 ccProcessMoveRequest
(   
CcConCb *con1,                    /* connection */
CcConCb *con2                     /* connection */
)   
#else
PUBLIC S16 ccProcessMoveRequest(con1, con2)
CcConCb *con1;                    /* connection */
CcConCb *con2;                    /* connection */
#endif
{
  S16                   ret            = ROK;
  S16                   tmpret         = RFAILED;
  MgctPackageId         pkgId          = MGCT_PACKAGE_ID_MAX;
  MgctCallProgItemId    signal         = MGCT_ITEM_ID_CG_MAX;
  MgctMode              mode           = MGCT_MODE_MAX;



  if ((con1 == NULLP) || (con2 == NULLP))
  {
    CCLOGERR("Critical ERROR: Null pointers:%lx, %lx\n", (U32)con1, (U32)con1);
    ABORT_DEBUG;
    if (con1 != NULLP)
        ccHandleCleanUpConCb(con1,TRUE,CCTMPFAIL);
    if (con2 != NULLP)
        ccHandleCleanUpConCb(con2,TRUE,CCTMPFAIL);
    RETVALUE(RFAILED);
  }

  CCDP(AIN_DBG_LEVEL_1, "%s:con1 - icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", __FUNCTION__,con1->icSuConnId, con1->ogSuConnId,
       (con1->mgBlock->mgCtx ? con1->mgBlock->mgCtx->suCtxId : 0), con1->icBcm.svcHeader.transId);

  CCDP(AIN_DBG_LEVEL_1, "%s:con2 - icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", __FUNCTION__,con2->icSuConnId, con2->ogSuConnId,
       (con2->mgBlock->mgCtx ? con2->mgBlock->mgCtx->suCtxId : 0), con2->icBcm.svcHeader.transId);

  /* Stop the Early ACM timer if it is started */
  ccStopConTmr(con1, CC_TMR_EARLYACM);

  /* Save Current state in assocBlock */
  CC_SET_ASSOCSTATE(con2);

  CCDP(AIN_DBG_LEVEL_1, "%s:con2's current state:%d\n", __FUNCTION__,con2->state);
 
  /* Put con2 in state 68-CCS_CANNOTPROCESSEVENTS 
   */

  con2->lastState = con2->state;
  CC_STATE_CHANGE(con2, CCS_CANNOTPROCESSEVENTS);

  /* Copy remoteSdpSentOnIcBCM from con1 to con2 */
  con2->remoteSdpSentOnIcBCM = con1->remoteSdpSentOnIcBCM;

  if (CC_IS_CONTEXT_PRESENT(con1) && CC_IS_CONTEXT_PRESENT(con2)) 
  {
     /* Case 1, context present in con1 and con2, move operation is possible.
      * Move A from con1 to con2
      */
      U32 SsuCtxId;
      U32 DsuCtxId;

      if (con1->cacCb)
      {
        if (!con2->cacCb)
        {
          S16  ret          = RFAILED;
      
          ret = SGetSBuf(ccInit.region, ccInit.pool, (Data **) &(con2->cacCb),
                               (Size) sizeof(CcCacCb));
          if (ret != ROK)
          {
            CCLOGERR("**%s(): OUT of MEMORY origIcSuConnId=%ld ****\n",
                           __FUNCTION__, con2->origIcSuConnId);
            RETVALUE(RFAILED);
          }
          cmMemset((U8 *) (con2->cacCb), 0, sizeof(CcCacCb));
        }
        con2->cacCb->icswtchComCodec = con1->cacCb->icswtchComCodec;
        con2->cacCb->icNeedCACUpdate = con1->cacCb->icNeedCACUpdate;
        con2->cacCb->IcRealmCACCheckDone = con1->cacCb->IcRealmCACCheckDone;
        con2->cacCb->IngressRealmIdInBICC = con1->cacCb->IngressRealmIdInBICC;
        con2->cacCb->CurrentIcCodec = con1->cacCb->CurrentIcCodec;
      }

      tmpret = ccCheckMgContext(&(con1->icBcm), &pkgId, &signal, &mode);

      SsuCtxId = (con1->icBcm.mgCtx ? con1->icBcm.mgCtx->suCtxId : 0);
      if (CC_IS_CALL_MULTI_MG(con2))
        DsuCtxId = (con2->icAdjBcm->mgCtx ? con2->icAdjBcm->mgCtx->suCtxId : 0);
      else
        DsuCtxId = (con2->ogBcm.mgCtx ? con2->ogBcm.mgCtx->suCtxId : 0);

      ccUpdateMgQForMoveRsc(mode, NOTPRSNT, 0, TGP_NLP_MAX, TGP_EC_MAX,
                            &(con1->icBcm), &(con2->icBcm),
                            SsuCtxId, DsuCtxId,
                            con1, con2, NULLP,
                            NULLP,
                            CCDESTINATION, signal);
      con2->lastState = con2->state;
      CC_STATE_CHANGE(con2, CCS_AWTTMGCTFORMOVECALL);
      con2->mgBlock->con = con2;
      ret = ccSendMgiQueue(DsuCtxId, con2->mgBlock);
  }
  else if (!CC_IS_CONTEXT_PRESENT(con1) && CC_IS_CONTEXT_PRESENT(con2)) 
  {
      /* All IP, There is no SFEP 
       * Add A of CON2  
       */
      ProtType   icProtType;
      U32  suCtxId;
      U32  contextId;

      icProtType = CC_GET_PROTOCOL(con2->icProtType);
      
      if (((icProtType == CC_SIP)||
           (icProtType == CC_SIPT)||
           ((icProtType == CC_BICC)&&
            (con2->cacCb)&&
            (con2->cacCb->IngressRealmIdInBICC)))&&
            ((CC_IS_MG_NEEDED_IN_IP_CALL(con2))||
             (!ccIsIPResource(&con2->ogBcm)))&&
          (con2->callDtlInfo.cm.ingressRealmId))
      {
        if (!con2->cacCb)
        {
          S16  ret          = RFAILED;
        
          ret = SGetSBuf(ccInit.region, ccInit.pool, (Data **) &(con2->cacCb),
                               (Size) sizeof(CcCacCb));
          if (ret != ROK)
          {
            CCLOGERR("**%s(): OUT of MEMORY origIcSuConnId=%ld ****\n",
                           __FUNCTION__, con2->origIcSuConnId);
            RETVALUE(RFAILED);
          }
          cmMemset((U8 *) (con2->cacCb), 0, sizeof(CcCacCb));
        }
        
        con2->cacCb->icNeedCACUpdate = TRUE;
        con2->cacCb->CurrentIcCodec.val = 0;
        con2->cacCb->CurrentIcCodec.pres = NOTPRSNT;
      }

      if (CC_IS_CALL_MULTI_MG(con2))
      {
        suCtxId = (con2->icAdjBcm->mgCtx ? con2->icAdjBcm->mgCtx->suCtxId : 0);
        contextId = (con2->icAdjBcm->mgCtx ? con2->icAdjBcm->mgCtx->contextId : 0);
      }
      else
      {
        suCtxId = (con2->ogBcm.mgCtx ? con2->ogBcm.mgCtx->suCtxId : 0);
        contextId = (con2->ogBcm.mgCtx ? con2->ogBcm.mgCtx->contextId : 0);
      }

      ccUpdateMgQForAddOrModifyRsc(MGCT_MODE_SENDRECV, MGCT_PACKAGE_ID_MAX,
                                   MGCT_ITEM_ID_CG_MAX, MGCT_CMD_ADD,
                                   &(con2->icBcm), NULLP, NULLP,
                                   NULLP, NULLP, NULLP,
                                   suCtxId, contextId, con2);
      con2->renegSdpAfterAns = TRUE;
      con2->lastState = con2->state;
      CC_STATE_CHANGE(con2, CCS_AWTTMGCTFORMOVECALL);
      con2->mgBlock->con = con2;
      ret = ccSendMgiQueue(suCtxId, con2->mgBlock);
  }
  else if (CC_IS_CONTEXT_PRESENT(con1) && !CC_IS_CONTEXT_PRESENT(con2)) 
  {
      /* Assume CON2 is in the process of setting up A-C, Sub CON1 */
      ccUpdateMgQForSubRsc(&con1->icBcm, con1->mgBlock->mgCtx->suCtxId, con1);
      con2->lastState = con2->state;
      CC_STATE_CHANGE(con2, CCS_AWTTMGCTFORMOVECALL);
      con1->mgBlock->con = con1;
      ret = ccSendMgiQueue(con1->mgBlock->mgCtx->suCtxId, con1->mgBlock);
  }
  else 
  {
       /* (!CC_IS_CONTEXT_PRESENT(con1) && !CC_IS_CONTEXT_PRESENT(con2)) */
       /* Complete processing of the MOVECALL based on sendRelease in assocBlock.
        */
        ccProcessMoveSendRelease(con1,con2);
        RETVALUE(ROK);
  }

  /* There was an error with ccSendMgiQueue(), release con2 */
  if (ret != ROK)
  {
     CCLOGERR("Error: ccSendMgiQueue failed\n");
     cleanUpMgQ(con2);
     ccProcessMoveSendRelease(con1,con2);
   }
   RETVALUE(ROK);

} /* End of ccProcessMoveRequest */

/* 
 *      Fun:   ccProcessMoveSendRelease
 *
 *      Desc:  Complete processing of the MoveCALL based on SendRelease in the assocBlock 
 *             con    - Represents the Cb that received the MOVECALL request
 */
#ifdef ANSI
PUBLIC  S16 ccProcessMoveSendRelease
(   
CcConCb *con1,                    /* connection */
CcConCb *con2                     /* connection */
)   
#else
PUBLIC S16 ccProcessMoveSendRelease(con1,con2)
CcConCb *con1;                    /* connection */
CcConCb *con2;                    /* connection */
#endif
{
  S16               ret            = ROK;
  UConnId           tempSuConnId   = 0;


  CCDP(AIN_DBG_LEVEL_1, "%s:con1 - icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", __FUNCTION__,con1->icSuConnId, con1->ogSuConnId,
       (con1->mgBlock->mgCtx ? con1->mgBlock->mgCtx->suCtxId : 0), con1->icBcm.svcHeader.transId);

  CCDP(AIN_DBG_LEVEL_1, "%s:con2 - icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", __FUNCTION__,con2->icSuConnId, con2->ogSuConnId,
       (con2->mgBlock->mgCtx ? con2->mgBlock->mgCtx->suCtxId : 0), con2->icBcm.svcHeader.transId);

  con2->icSapCb         = con1->icSapCb;
  con2->uBuf            = con1->uBuf;
  con1->uBuf            = NULL;

  ret = cmHashListDelete(&con1->icSapCb->spInstTbl, (PTR) con1);
  if (ret != ROK)
  {
     CCLOGERR("cmHashListDelete fail spInstTbl, %ld\n", con1->icSpConnId);
     ABORT_DEBUG;
     ccHandleCleanUpConCb(con1,TRUE,CCTMPFAIL);
     ccHandleCleanUpConCb(con2,TRUE,CCTMPFAIL);
     RETVALUE(ROK);
  }

  con2->icSpConnId      = con1->icSpConnId;
  con1->icSpConnId      = 0;

  ret = cmHashListInsert(&con2->icSapCb->spInstTbl, (PTR) con2,
                     (U8 *)&con2->icSpConnId, (U16) sizeof(UConnId));
  if (ret != ROK)
  {
     CCLOGERR("cmHashListInsert fail spInstTbl, %ld\n", con2->icSpConnId);
     ABORT_DEBUG;
     ccHandleCleanUpConCb(con1,TRUE,CCTMPFAIL);
     ccHandleCleanUpConCb(con2,TRUE,CCTMPFAIL);
     RETVALUE(ROK);
  }

  con1->icRscVal = FALSE;
  con2->icRscVal = TRUE;

  /* Move the local SDP */
  cmStoreSdp(ccInit.region, ccInit.pool, &con2->icIpParam.locDesc,
             con1->icIpParam.locDesc);


  ret = cmHashListDelete(&ccSuInstTbl, (PTR) con1);
  if (ret != ROK)
  {
     CCLOGERR("cmHashListDelete fail ccSuInstTbl, %ld\n", con1->icSuConnId);
     ABORT_DEBUG;
     ccHandleCleanUpConCb(con1,TRUE,CCTMPFAIL);
     ccHandleCleanUpConCb(con2,TRUE,CCTMPFAIL);
     RETVALUE(ROK);
  }

  ret = cmHashListDelete(&ccSuInstTbl, (PTR) con2);
  if (ret != ROK)
  {
     CCLOGERR("cmHashListDelete fail ccSuInstTbl, %ld\n", con2->icSuConnId);
     ABORT_DEBUG;
     ccHandleCleanUpConCb(con1,TRUE,CCTMPFAIL);
     ccHandleCleanUpConCb(con2,TRUE,CCTMPFAIL);
     RETVALUE(ROK);
  }

  tempSuConnId          = con2->icSuConnId;
  con2->icSuConnId      = con1->icSuConnId;
  con1->icSuConnId      = tempSuConnId;
  ret = cmHashListInsert(&ccSuInstTbl, (PTR)con1, (U8 *)&con1->icSuConnId,
                        (U16)sizeof(UConnId));
  if (ret != ROK)
  {
     CCLOGERR("cmHashListInsert fail ccSuInstTbl, %ld\n", con1->icSuConnId);
     ABORT_DEBUG;
     ccHandleCleanUpConCb(con1,TRUE,CCTMPFAIL);
     ccHandleCleanUpConCb(con2,TRUE,CCTMPFAIL);
     RETVALUE(ROK);
  }

  ret = cmHashListInsert(&ccSuInstTbl, (PTR)con2, (U8 *)&con2->icSuConnId,
                        (U16)sizeof(UConnId));
  if (ret != ROK)
  {
     CCLOGERR("cmHashListInsert fail ccSuInstTbl, %ld\n", con2->icSuConnId);
     ABORT_DEBUG;
     ccHandleCleanUpConCb(con1,TRUE,CCTMPFAIL);
     ccHandleCleanUpConCb(con2,TRUE,CCTMPFAIL);
     RETVALUE(ROK);
  }

  CCDP(AIN_DBG_LEVEL_1, "%s:con1 Update - icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", __FUNCTION__,con1->icSuConnId, con1->ogSuConnId,
      (con1->mgBlock->mgCtx ? con1->mgBlock->mgCtx->suCtxId : 0), con1->icBcm.svcHeader.transId);
  CCDP(AIN_DBG_LEVEL_1, "%s:con2 Update - icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", __FUNCTION__,con2->icSuConnId, con2->ogSuConnId,
      (con2->mgBlock->mgCtx ? con2->mgBlock->mgCtx->suCtxId : 0), con2->icBcm.svcHeader.transId);

  /* Restore state of con2 to what is stored in assocBlock 
   */

   con2->lastState = con2->state;
   CC_STATE_CHANGE(con2, CC_GET_ASSOCSTATE(con2));

  /* Cleanup unwanted buffered events. These events can be ignored since
   * call is going to be release.
   */
   ccDeleteBufferedEvent(con1, 0, CCE_MGCTNOTIFY);

   /* BUG81414 */
   /* Send Early ACM towards incoming */
   if(!con2->isACMSent)
       ccSendBackwardMessageForAnnoucement(con2, RtPlayAnnouncements, TRUE, FALSE);

  /* Check sendRelease in assocBlock, stored in con2
   * FALSE if coming from 03-CCS_AWTRSPODISCONNECT 
   *    o GCC needs to send a RLC to the outgoing side.
   * TRUE if coming from 123-CCS_AWTIDISCONNECT or 99-CCS_AWTRSPFORDPE12(OSUSPEND)
   *    o GCC needs to send a REL to the outgoing side.
   */
   if (!CC_IS_SENDRELEASE_ENABLED(con2))
   {
        /* 03-CCS_AWTRSPODISCONNECT case
         */
        if (CC_IS_CONTEXT_PRESENT(con1))
        {    
           /* Have to clean up context */
           ccUpdateMgQForSubRsc((PTR) NULLP, con1->mgBlock->mgCtx->suCtxId, con1);
           CC_STATE_CHANGE(con1, CCS_AWTSWTCFM_SENDNOREL);
           con1->mgBlock->con = con1;
           con1->relPend = OUTRELRSPANDNOREL;
           ret = ccSendMgiQueue(con1->mgBlock->mgCtx->suCtxId, con1->mgBlock);
           if (ret == ROK) 
           {
               /* Safe to kill the assocBlock */
               ccFreeConAssocBlock(con2);
               RETVALUE(ROK);
           }
        }
        if (ret != ROK)
        {
            CCLOGERR("%s - ERROR: ccSendMgiQueue Failed suCtxId=%lx\n", __FUNCTION__,con1->mgBlock->mgCtx->suCtxId);
            cleanUpMgQ(con1);
        }
        /* Safe to kill the assocBlock */
        ccFreeConAssocBlock(con2);
        con1->direction = OUTTOINC;
        ccSendReleaseRsp(con1);
        CC_STATE_CHANGE(con1, CCS_AWTDEALOCCFM);
        if(con1->ogRscVal == TRUE)
        {
          ccDeallocateResource(con1, CC_OUTGOING);
        }
        else
        {
          CC_STATE_CHANGE(con1, CCS_IDLE);
          ccRelCon(con1);
        }
        RETVALUE(ROK);
   }
   /* Kick off generic release process for con1 call
    * 123-CCS_AWTIDISCONNECT or 99-CCS_AWTRSPFORDPE12(OSUSPEND) case
    */
   CCDP(AIN_DBG_LEVEL_0, "Cleaning up the call: 0x%lx,0x%lx\n", con1->icSuConnId,
        con1->ogSuConnId);
   ccSendReleasesOnBothSidesWithSwitching(con1, CCNORMUNSPEC);

   /* Safe to kill the assocBlock */
   ccFreeConAssocBlock(con2);
#if 0                           /* BUG81414 */
   /* Send Early ACM towards incoming */
   ccSendBackwardMessageForAnnoucement(con2, RtPlayAnnouncements, TRUE, FALSE);
#endif
   RETVALUE(ROK);

} /* End of ccProcessMoveSendRelease */

/* 
 *      Fun:   ccFindSwitchingReqForAssocCalls 
 *
 *      Desc:  Called from the ccIsSwitchingReqOk only when con->assocBlock is present 
 *
 */
#ifdef ANSI
PUBLIC  Bool ccFindSwitchingReqForAssocCalls
(   
CcConCb *con                    /* connection */
)   
#else
PUBLIC Bool ccFindSwitchingReqForAssocCalls(con)
CcConCb *con;                    /* connection */
#endif
{
  S16               ret               = RFAILED;
  CcConCb           *con1             = NULLP;
  CcConCb           *con2             = NULLP;

  /* con2 has the assocBlock */
  con2=con;

  CCDP(AIN_DBG_LEVEL_1, "%s:con2 - icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", __FUNCTION__,con2->icSuConnId, con2->ogSuConnId,
       (con2->mgBlock->mgCtx ? con2->mgBlock->mgCtx->suCtxId : 0), con2->icBcm.svcHeader.transId);

  /* Get associated call */
  /* Find Associated Connection Block based on assocTransId provided from FIC */
  ret = cmHashListFind(&ccSuInstTbl,(U8 *)&(CC_GET_ASSOCPARENTSUCONNID(con2)),
                      (U16)sizeof(UConnId), 0, (PTR *)&con1);
  if (ret != ROK)
  {
     CCLOGERR("%s - ERROR: Connection Block Not Found %lx\n", __FUNCTION__,CC_GET_ASSOCPARENTSUCONNID(con2));
     ABORT_DEBUG;
     ccHandleCleanUpConCb(con2,TRUE,CCTMPFAIL);
  }
  CCDP(AIN_DBG_LEVEL_1, "%s:con1 - icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", __FUNCTION__,con1->icSuConnId, con1->ogSuConnId,
      (con1->mgBlock->mgCtx ? con1->mgBlock->mgCtx->suCtxId : 0), con1->icBcm.svcHeader.transId);

  /* RmMgStatus is being used to determine if this is a SFEP
   */
  if ((RmMgStatus == RM_MG_OOS) &&
      ccIsIPResource(&con1->icBcm) && ccIsIPResource(&con2->ogBcm))
  {
     /* SFEP case, where ic and og are both IP */
     RETVALUE(FALSE);
  }
  else
  {
    /* Always want to create a context to pass ringback/inband if A-B of con1 
     * fails to connect and we connect A-C, where C is in con2.
     */
     RETVALUE(TRUE);
  }
} /* End of ccFindSwitchingReqForAssocCalls */

/* 
 *      Fun:   ccConE77S00 
 *
 *      Desc:  Create Associated Call for CRBT
 *             event - CCE_CAC (77)
 *             state - CCS_AWTANS (09) 
 *             state - CCS_ANSWERED (10) 
 */
#ifdef ANSI
PUBLIC  S16 ccConE77S00
(   
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                   /* bcm */
PTR     event2                   /* event */
)   
#else
PUBLIC S16 ccConE77S00(con, event, bcm, event2)
CcConCb *con;                    /* connection */
PTR     event;                   /* event */
AinBCM_t *bcm;                   /* bcm */
PTR     event2;                  /* event */
#endif
{
  S16                  ret                    = RFAILED;
  CcConCb              *con2                  = NULLP;
  CcConCb              *con1                  = NULLP;
  AinBCM_t             *pBcm                  = NULLP;
  U8                   direction              = 0;
  msgValEDPRequestEnum index                  = 0;
  AnalyzeRoute_t       *pAnalyzeRoute         = NULLP;
  CreateAssocCall_t    *pCreateAssocCall;
  SvcHeader_t          *pSvcHeader;
  SvcHeader_t          header;
  InfoCollected_t      infoCollected;
  /* BUG#75722 */
  TknStrVar *callId = NULLP;
  TknStrVar *dstCallId = NULLP;
  ProtType    protType;
  /*Bug92218*/
  TknStrVar *srcReqUriStr = NULLP;
  TknStrVar *dstReqUriStr = NULLP; 
  /* Bug 92809 */
  TknStrVar *srcHistInfoEntries = NULLP;
  TknStrVar *dstHistInfoEntries = NULLP;

  if (event2 == NULLP)
  {
    /* con is verified to not be a NULLP on initial ccCallStateMachine */
    CCLOGERR("Critical ERROR: Null pointers: event2=%ld\n", event2);
    ABORT_DEBUG;
    RETVALUE(ret);
  }

  /* CCE_CAC Received on con2 */ 
  con2 = con;

  /* pSvcHeader needed for suTransid from FIC, spTransId is not valid  */
  pSvcHeader = (SvcHeader_t *) event;
  pCreateAssocCall = (CreateAssocCall_t *) event2;
  pAnalyzeRoute    = &(pCreateAssocCall->analyzeRoute);

  /* Find con1 Connection Block based on assocTransId provided from FIC */
  /* Search the transaction Id in BCM Instance table */
  pBcm = ccFindBCMFromTransId(pCreateAssocCall->assocTransId, direction);
  if (pBcm == NULLP)
  {
      CCLOGERR("**%s:DID NOT FIND THE BCM, transId = %ld\n",
               __FUNCTION__,pCreateAssocCall->assocTransId);
      ABORT_DEBUG;
      ccRelCon(con2);
      RETVALUE(RFAILED);
  }
  con1=pBcm->con;

  CCDP(AIN_DBG_LEVEL_1, "%s: con1 - icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", __FUNCTION__,con1->icSuConnId, con1->ogSuConnId,
       (con1->mgBlock->mgCtx ? con1->mgBlock->mgCtx->suCtxId : 0), con1->icBcm.svcHeader.transId);

  CCDP(AIN_DBG_LEVEL_1, "%s: con2 - icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", __FUNCTION__,con2->icSuConnId, con2->ogSuConnId,
       (con2->mgBlock->mgCtx ? con2->mgBlock->mgCtx->suCtxId : 0), con2->icBcm.svcHeader.transId);

  if (pSvcHeader->lastTransactionFlag == TRUE)
  {
    CCLOGERR("%s: Error, lastTransaction is TRUE for CreateAssocCall\n",__FUNCTION__);
    ABORT_DEBUG;
    ccRelCon(con2);
    ccHandleCleanUpConCb(con1,TRUE,CCTMPFAIL);
    RETVALUE(RFAILED);
  }

  /* Copy required data from CON1 into CON2 
   */

  /* Copy entire callDtlInfo from con1 into con2. 
   * Make ingressBillingSdp/egressBillingSdp NOTPRSNT to prevent double free 
   */
  cmMemcpy((U8 *)(&con2->callDtlInfo), (U8 *)(&con1->callDtlInfo),
         sizeof(CcCallDtlInfo));

  /* Bug 73684: These should be NOTPRSNT, make sure to prevent double free. */
  con2->callDtlInfo.un.icSipId.pres = NOTPRSNT;
  con2->callDtlInfo.un.ogSipId.pres = NOTPRSNT;

  /* FID 14485 */
  ccCopyTknStrVar(&con2->callDtlInfo.un.icSipId,&con1->callDtlInfo.un.icSipId);

  /* FID14485 the sip call-id GCC generated is within 32(CC_SIP_ID_LENGTH_OLD) bytes presently, 
   * if the call-id GCC generated is longer than 32  bytes,should change to CC_SIP_ID_LENGTH(128)
   * or more */
  /* get new sipCallId for og leg, Overwrite ogSipId from con1 in State Machine */
  ret = ccGetNewVarSipCallId (&(con2->callDtlInfo.un.ogSipId),
        CC_SIP_ID_LENGTH, con2->ogSuConnId,TRUE);
  if(ret != ROK)
     CCDP(AIN_DBG_LEVEL_3,"Error in getting new sip call-id!!!");

  /* Bug 61506 */
  con2->callDtlInfo.cm.ingressBillingSdp.pres = NOTPRSNT;
  con2->callDtlInfo.cm.egressBillingSdp.pres = NOTPRSNT;

  /* Must allocate new memory for ingressBillingSdp if it exists in con1. */
  if(con1->callDtlInfo.cm.ingressBillingSdp.pres == PRSNT_NODEF)
  {
    TknStrVar *icSdp;

    icSdp = &con2->callDtlInfo.cm.ingressBillingSdp;
    icSdp->region = ccInit.region;
    icSdp->pool = ccInit.pool;
    icSdp->size = con1->callDtlInfo.cm.ingressBillingSdp.len + 1;
    ret = SGetSBuf(icSdp->region, icSdp->pool,
                   (Data **)&(icSdp->val), icSdp->size);
    if (ret == ROK)
    {
      icSdp->pres = PRSNT_NODEF;
      icSdp->len = con1->callDtlInfo.cm.ingressBillingSdp.len;
      cmMemcpy(icSdp->val, con1->callDtlInfo.cm.ingressBillingSdp.val, icSdp->len);
      icSdp->val[icSdp->len] = 0;
    }
  }

  /* Bug 63050: Prevent double free  */
  con2->callDtlInfo.un.sipUriHostpartS.cdPtyNumHostname.pres = NOTPRSNT;
  con2->callDtlInfo.un.sipUriHostpartS.cgPtyNumHostname.pres = NOTPRSNT;
  con2->callDtlInfo.un.sipUriHostpartS.origCdNumHostname.pres = NOTPRSNT;
  con2->callDtlInfo.un.sipUriHostpartS.redirgNumHostname.pres = NOTPRSNT;

  if(con1->callDtlInfo.un.sipUriHostpartS.cdPtyNumHostname.pres == PRSNT_NODEF)
  {
    TknStrVar *cdPtyNumHostname;

    cdPtyNumHostname = &con2->callDtlInfo.un.sipUriHostpartS.cdPtyNumHostname;
    cdPtyNumHostname->region = ccInit.region;
    cdPtyNumHostname->pool = ccInit.pool;
    cdPtyNumHostname->size = con1->callDtlInfo.un.sipUriHostpartS.cdPtyNumHostname.len + 1;
    ret = SGetSBuf(cdPtyNumHostname->region, cdPtyNumHostname->pool,
                   (Data **)&(cdPtyNumHostname->val), cdPtyNumHostname->size);
    if (ret == ROK)
    {
      cdPtyNumHostname->pres = PRSNT_NODEF;
      cdPtyNumHostname->len = con1->callDtlInfo.un.sipUriHostpartS.cdPtyNumHostname.len;
      cmMemcpy(cdPtyNumHostname->val, con1->callDtlInfo.un.sipUriHostpartS.cdPtyNumHostname.val, 
               cdPtyNumHostname->len);
      cdPtyNumHostname->val[cdPtyNumHostname->len] = 0;
    }
  }
  if(con1->callDtlInfo.un.sipUriHostpartS.cgPtyNumHostname.pres == PRSNT_NODEF)
  {
    TknStrVar *cgPtyNumHostname;

    cgPtyNumHostname = &con2->callDtlInfo.un.sipUriHostpartS.cgPtyNumHostname;
    cgPtyNumHostname->region = ccInit.region;
    cgPtyNumHostname->pool = ccInit.pool;
    cgPtyNumHostname->size = con1->callDtlInfo.un.sipUriHostpartS.cgPtyNumHostname.len + 1;
    ret = SGetSBuf(cgPtyNumHostname->region, cgPtyNumHostname->pool,
                   (Data **)&(cgPtyNumHostname->val), cgPtyNumHostname->size);
    if (ret == ROK)
    {
      cgPtyNumHostname->pres = PRSNT_NODEF;
      cgPtyNumHostname->len = con1->callDtlInfo.un.sipUriHostpartS.cgPtyNumHostname.len;
      cmMemcpy(cgPtyNumHostname->val, con1->callDtlInfo.un.sipUriHostpartS.cgPtyNumHostname.val, 
               cgPtyNumHostname->len);
      cgPtyNumHostname->val[cgPtyNumHostname->len] = 0;
    }
  }
  if(con1->callDtlInfo.un.sipUriHostpartS.origCdNumHostname.pres == PRSNT_NODEF)
  {
    TknStrVar *origCdNumHostname;

    origCdNumHostname = &con2->callDtlInfo.un.sipUriHostpartS.origCdNumHostname;
    origCdNumHostname->region = ccInit.region;
    origCdNumHostname->pool = ccInit.pool;
    origCdNumHostname->size = con1->callDtlInfo.un.sipUriHostpartS.origCdNumHostname.len + 1;
    ret = SGetSBuf(origCdNumHostname->region, origCdNumHostname->pool,
                   (Data **)&(origCdNumHostname->val), origCdNumHostname->size);
    if (ret == ROK)
    {
      origCdNumHostname->pres = PRSNT_NODEF;
      origCdNumHostname->len = con1->callDtlInfo.un.sipUriHostpartS.origCdNumHostname.len;
      cmMemcpy(origCdNumHostname->val, con1->callDtlInfo.un.sipUriHostpartS.origCdNumHostname.val, 
               origCdNumHostname->len);
      origCdNumHostname->val[origCdNumHostname->len] = 0;
    }
  }
  if(con1->callDtlInfo.un.sipUriHostpartS.redirgNumHostname.pres == PRSNT_NODEF)
  {
    TknStrVar *redirgNumHostname;

    redirgNumHostname = &con2->callDtlInfo.un.sipUriHostpartS.redirgNumHostname;
    redirgNumHostname->region = ccInit.region;
    redirgNumHostname->pool = ccInit.pool;
    redirgNumHostname->size = con1->callDtlInfo.un.sipUriHostpartS.redirgNumHostname.len + 1;
    ret = SGetSBuf(redirgNumHostname->region, redirgNumHostname->pool,
                   (Data **)&(redirgNumHostname->val), redirgNumHostname->size);
    if (ret == ROK)
    {
      redirgNumHostname->pres = PRSNT_NODEF;
      redirgNumHostname->len = con1->callDtlInfo.un.sipUriHostpartS.redirgNumHostname.len;
      cmMemcpy(redirgNumHostname->val, con1->callDtlInfo.un.sipUriHostpartS.redirgNumHostname.val, 
               redirgNumHostname->len);
      redirgNumHostname->val[redirgNumHostname->len] = 0;
    }
  }

  /* Bug 63050: these should be NOTPRSNT, make sure to prevent double free  */
  con2->callDtlInfo.cm.networkStats.ic_pres  = NOTPRSNT;
  con2->callDtlInfo.cm.networkStats.og_pres  = NOTPRSNT;

  /* Bug 92455, set sipOrigIoi.pres to NOTPRSNT to avoid  wronlgy free con1
   * memory in ccCopyTknStrVar(), also apply on sipTermIoi and pAccessNetworkInfo  */
  con2->callDtlInfo.cm.sipOrigIoi.pres = NOTPRSNT;
  con2->callDtlInfo.cm.sipTermIoi.pres = NOTPRSNT;
  con2->callDtlInfo.cm.pAccessNetworkInfo.pres = NOTPRSNT; 

  /* Revsie code since bug 84027 & 84113 had changed the below params to dynamic structure 
   * start ++ */ 
  if(con1->callDtlInfo.cm.sipOrigIoi.pres == PRSNT_NODEF)
  {
   ccCopyTknStrVar(&con2->callDtlInfo.cm.sipOrigIoi, &con1->callDtlInfo.cm.sipOrigIoi);
  }
  if(con1->callDtlInfo.cm.sipTermIoi.pres == PRSNT_NODEF)
  {
   ccCopyTknStrVar(&con2->callDtlInfo.cm.sipTermIoi, &con1->callDtlInfo.cm.sipTermIoi);
  }
  if(con1->callDtlInfo.cm.pAccessNetworkInfo.pres == PRSNT_NODEF)
  {
   ccCopyTknStrVar(&con2->callDtlInfo.cm.pAccessNetworkInfo, &con1->callDtlInfo.cm.pAccessNetworkInfo);
  }
  /* end -- */

  /* Save con1's icSuConnId in con2's assocBlock->assocParentSuConnId */
  CC_SET_ASSOCPARENTSUCONNID(con2,con1->icSuConnId);

  /* Struct Copy, must be done before call to ccAssignSaps() */
  /* Set icRscVal once resource is moved */ 
  con2->icRsc = con1->icRsc;

  if (con1->cacCb)
  {
    if (!con2->cacCb)
    {
      S16  ret          = RFAILED;
    
      ret = SGetSBuf(ccInit.region, ccInit.pool, (Data **) &(con2->cacCb),
                           (Size) sizeof(CcCacCb));
      if (ret != ROK)
      {
        CCLOGERR("**%s(): OUT of MEMORY origIcSuConnId=%ld ****\n",
                       __FUNCTION__, con2->origIcSuConnId);
        RETVALUE(RFAILED);
      }
      cmMemset((U8 *) (con2->cacCb), 0, sizeof(CcCacCb));
    }
    con2->cacCb->icswtchComCodec = con1->cacCb->icswtchComCodec;
    con2->cacCb->icNeedCACUpdate = con1->cacCb->icNeedCACUpdate;
    con2->cacCb->IcRealmCACCheckDone = con1->cacCb->IcRealmCACCheckDone;
    con2->cacCb->IngressRealmIdInBICC = con1->cacCb->IngressRealmIdInBICC;
    con2->cacCb->CurrentIcCodec = con1->cacCb->CurrentIcCodec;
  }

  /* BUG: 92471, Copy icHeldIpVersion */
  con2->icHeldIpVersion = con1->icHeldIpVersion;


  /* BUG# 75722 We only care about CCE_CONIND and the icProtType */
  protType = con1->icProtType;
  
  /* We are only interested in moving the CC_CONIND event to con2 */
  if ((con1->evntHeld == CCE_CONIND) && (con1->ccEvntHold))
  {
     /* cmMemcpy the CCE_CONIND held event since ccEvntHold is allocated space 
      * on connection block creation for con2
      */ 
     CCDP(AIN_DBG_LEVEL_0,"%s: CCE_CONIND event found in con1->ccEvntHold con1->evntHeld=%d, con1->evntTypeHeld=%d\n", __FUNCTION__,con1->evntHeld,con1->evntTypeHeld);
     cmMemcpy ((U8 *) (con2->ccEvntHold), (U8 *) (con1->ccEvntHold),sizeof(CcAllSdus));
     con2->evntHeld        = con1->evntHeld;
     /* BUG#75722 Need to copy the call-id from con1 to con2 */
     if(CC_GET_PROTOCOL(protType) == CC_SIPT)
     {
       callId =
         &(con1->ccEvntHold->m.ccConEvnt.m.siConEvnt.optElmnt.sipt.callId);

       dstCallId =
         &(con2->ccEvntHold->m.ccConEvnt.m.siConEvnt.optElmnt.sipt.callId);

       if(callId && (callId->pres == PRSNT_NODEF) && (callId->len >0))
       {
         /* Reset to avoid double free */
         dstCallId->pres = NOTPRSNT;
         dstCallId->len  = 0;
         ccCopyTknStrVar(dstCallId, callId);
         CCDP(AIN_DBG_LEVEL_0,"Copy the call-id from con1 to con2\n");
       }

       /* BUG:92218 Need to copy the ReqUriStr from con1 to con2 */
       srcReqUriStr = &(con1->ccEvntHold->m.ccConEvnt.m.siConEvnt.reqUriStr);
       dstReqUriStr = &(con2->ccEvntHold->m.ccConEvnt.m.siConEvnt.reqUriStr);
       if(srcReqUriStr && (srcReqUriStr->pres == PRSNT_NODEF) && (srcReqUriStr->len >0))       
       {         
         dstReqUriStr->pres = NOTPRSNT;
         dstReqUriStr->len  = 0;
         ccCopyTknStrVar(dstReqUriStr, srcReqUriStr);
         CCDP(AIN_DBG_LEVEL_0,"Copy the reqUriStr from con1 to con2\n");
       }
       /* BUG:92809 copy the histInfoEntries from con1 to con2 */
       srcHistInfoEntries = &(con1->ccEvntHold->m.ccConEvnt.m.siConEvnt.optElmnt.sipt.sipHistInfo.otherEntries);
       dstHistInfoEntries = &(con2->ccEvntHold->m.ccConEvnt.m.siConEvnt.optElmnt.sipt.sipHistInfo.otherEntries);
       if(srcHistInfoEntries && (srcHistInfoEntries->pres == PRSNT_NODEF) && (srcHistInfoEntries->len >0))
       {
         cmCpyHistInfoEntries(dstHistInfoEntries, srcHistInfoEntries);
         CCDP(AIN_DBG_LEVEL_0,"Copy the HistInfoEntries from con1 to con2\n");
       }
     }
     if(CC_GET_PROTOCOL(protType) == CC_SIP)
     {
       callId = &(con1->ccEvntHold->m.ccConEvnt.m.sipConEvnt.callId);
       dstCallId = &(con2->ccEvntHold->m.ccConEvnt.m.sipConEvnt.callId);
       
       if(callId && (callId->pres == PRSNT_NODEF) && (callId->len >0))       
       {         
         /* Reset to avoid double free */
         dstCallId->pres = NOTPRSNT;
         dstCallId->len  = 0;
         ccCopyTknStrVar(dstCallId, callId);
         CCDP(AIN_DBG_LEVEL_0,"Copy the call-id from con1 to con2\n");
       }

       /* BUG:92218 Need to copy the ReqUriStr from con1 to con2 */
       srcReqUriStr = &(con1->ccEvntHold->m.ccConEvnt.m.sipConEvnt.reqUriStr);
       dstReqUriStr = &(con2->ccEvntHold->m.ccConEvnt.m.sipConEvnt.reqUriStr);

       if(srcReqUriStr && (srcReqUriStr->pres == PRSNT_NODEF) && (srcReqUriStr->len >0))       
       {         
         dstReqUriStr->pres = NOTPRSNT;
         dstReqUriStr->len  = 0;
         ccCopyTknStrVar(dstReqUriStr, srcReqUriStr);
         CCDP(AIN_DBG_LEVEL_0,"Copy the reqUriStr from con1 to con2\n");
       }
       /* BUG:92809 copy the histInfoEntries from con1 to con2 */
       srcHistInfoEntries = &(con1->ccEvntHold->m.ccConEvnt.m.sipConEvnt.sipHistInfo.otherEntries);
       dstHistInfoEntries = &(con2->ccEvntHold->m.ccConEvnt.m.sipConEvnt.sipHistInfo.otherEntries);
       if(srcHistInfoEntries && (srcHistInfoEntries->pres == PRSNT_NODEF) && (srcHistInfoEntries->len >0))
       {
         cmCpyHistInfoEntries(dstHistInfoEntries, srcHistInfoEntries);
         CCDP(AIN_DBG_LEVEL_0,"Copy the HistInfoEntries from con1 to con2\n");
       }
     }
  }
  else if (con1->icConEvntHold != NULL)
  {
     CCDP(AIN_DBG_LEVEL_0,"%s: CCE_CONIND event found in con1->icConEvntHold\n", __FUNCTION__);
     cmMemcpy ((U8 *) (con2->ccEvntHold), (U8 *) (con1->icConEvntHold),sizeof(CcAllSdus));
     con2->evntHeld = CCE_CONIND;

     /* BUG#75722 Need to copy the call-id from con1 to con2 */
     if(CC_GET_PROTOCOL(protType) == CC_SIPT)
     {
       callId =
         &(con1->icConEvntHold->m.ccConEvnt.m.siConEvnt.optElmnt.sipt.callId);

       dstCallId =
         &(con2->ccEvntHold->m.ccConEvnt.m.siConEvnt.optElmnt.sipt.callId);

       if(callId && (callId->pres == PRSNT_NODEF) && (callId->len >0))
       {
         /* Reset to avoid double free */
         dstCallId->pres = NOTPRSNT;
         dstCallId->len  = 0;
         ccCopyTknStrVar(dstCallId, callId);
         CCDP(AIN_DBG_LEVEL_0,"Copy the call-id from con1 to con2\n");
       }

       /* BUG:92218 Need to copy the ReqUriStr from con1 to con2 */
       srcReqUriStr = &(con1->icConEvntHold->m.ccConEvnt.m.siConEvnt.reqUriStr);
       dstReqUriStr = &(con2->ccEvntHold->m.ccConEvnt.m.siConEvnt.reqUriStr);
       if(srcReqUriStr && (srcReqUriStr->pres == PRSNT_NODEF) && (srcReqUriStr->len >0))       
       {         
         dstReqUriStr->pres = NOTPRSNT;
         dstReqUriStr->len  = 0;
         ccCopyTknStrVar(dstReqUriStr, srcReqUriStr);
         CCDP(AIN_DBG_LEVEL_0,"Copy the reqUriStr from con1 to con2\n");
       }
       /* BUG:92809 copy the histInfoEntries from con1 to con2 */
       srcHistInfoEntries = &(con1->icConEvntHold->m.ccConEvnt.m.siConEvnt.optElmnt.sipt.sipHistInfo.otherEntries);
       dstHistInfoEntries = &(con2->ccEvntHold->m.ccConEvnt.m.siConEvnt.optElmnt.sipt.sipHistInfo.otherEntries);
       if(srcHistInfoEntries && (srcHistInfoEntries->pres == PRSNT_NODEF) && (srcHistInfoEntries->len >0))
       {
         cmCpyHistInfoEntries(dstHistInfoEntries, srcHistInfoEntries);
         CCDP(AIN_DBG_LEVEL_0,"Copy the HistInfoEntries from con1 to con2\n");
       }
     }

     if(CC_GET_PROTOCOL(protType) == CC_SIP)
     {
       callId = &(con1->icConEvntHold->m.ccConEvnt.m.sipConEvnt.callId);
       dstCallId = &(con2->ccEvntHold->m.ccConEvnt.m.sipConEvnt.callId);
       
       if(callId && (callId->pres == PRSNT_NODEF) && (callId->len >0))       
       {         
         /* Reset to avoid double free */
         dstCallId->pres = NOTPRSNT;
         dstCallId->len  = 0;
         ccCopyTknStrVar(dstCallId, callId);
         CCDP(AIN_DBG_LEVEL_0,"Copy the call-id from con1 to con2\n");
       }

       /* BUG:92218 Need to copy the ReqUriStr from con1 to con2 */
       srcReqUriStr = &(con1->icConEvntHold->m.ccConEvnt.m.sipConEvnt.reqUriStr);
       dstReqUriStr = &(con2->ccEvntHold->m.ccConEvnt.m.sipConEvnt.reqUriStr);

       if(srcReqUriStr && (srcReqUriStr->pres == PRSNT_NODEF) && (srcReqUriStr->len >0))       
       {         
         dstReqUriStr->pres = NOTPRSNT;
         dstReqUriStr->len  = 0;
         ccCopyTknStrVar(dstReqUriStr, srcReqUriStr);
         CCDP(AIN_DBG_LEVEL_0,"Copy the reqUriStr from con1 to con2\n");
       }
       /* BUG:92809 copy the histInfoEntries from con1 to con2 */
       srcHistInfoEntries = &(con1->icConEvntHold->m.ccConEvnt.m.sipConEvnt.sipHistInfo.otherEntries);
       dstHistInfoEntries = &(con2->ccEvntHold->m.ccConEvnt.m.sipConEvnt.sipHistInfo.otherEntries);
       if(srcHistInfoEntries && (srcHistInfoEntries->pres == PRSNT_NODEF) && (srcHistInfoEntries->len >0))
       {
         cmCpyHistInfoEntries(dstHistInfoEntries, srcHistInfoEntries);
         CCDP(AIN_DBG_LEVEL_0,"Copy the HistInfoEntries from con1 to con2\n");
       }
     }
  }
  else
  {
     CCDP(AIN_DBG_LEVEL_0,"%s: CCE_CONIND event not found - con1->evntHeld=%d\n", __FUNCTION__,con1->evntHeld);
     ABORT_DEBUG;
     ccRelCon(con2);
     ccHandleCleanUpConCb(con1,TRUE,CCTMPFAIL);
     RETVALUE(RFAILED);
  }

  con2->icProtType      = con1->icProtType;
  con2->direction       = INCTOOUT; 
  con2->icBcm.bcmType   = BtOBcm; 

  /* This retrieves the suTransId, and last lastTransactionFlag 
   * from pSvcHeader and stores in icBcm, the suTransId will be overwritten.
   */

  CCDP(AIN_DBG_LEVEL_0,"%s: FIC - suTransId= %ld FIC - lastTransactionFlag=%d\n", __FUNCTION__,pSvcHeader->suTransId,pSvcHeader->lastTransactionFlag);

  ccExtractSvcHeader(pSvcHeader, &(con2->icBcm), MSG_Create_Assoc_Call);

  /* Allocate a new CC transId for use between CC and FIC  */
  con2->icBcm.svcHeader.transId = ccGetNewtransId();

  CCDP(AIN_DBG_LEVEL_0,"%s: Created New TransId= %ld\n", __FUNCTION__,con2->icBcm.svcHeader.transId);
 
  /* This allocates con2->icBcm.cc memory */
  /* This will also assign: con2->icBcm.cc->legId01 = & (con2->icBcm); */

  ret = ccFillBCMInfo(con2, &(con2->icBcm), con2->icBcm.svcHeader.transId,
                      CiOriginatingSetup, PicONull, BtOBcm,
                      DP_ORIGINATION_ATTEMPT);

  if (ret != ROK)
  {
       CCLOGERR("**%s:ccFillBCMInfo failed ret= %d\n",__FUNCTION__,ret);
       ABORT_DEBUG;
       ccRelCon(con2);
       ccHandleCleanUpConCb(con1,TRUE,CCTMPFAIL);
       RETVALUE(RFAILED);
  }

  /* Insert in icBCM Hash table with new transId */
  ret = cmHashListInsert(&ccicBCMInsTbl, (PTR) &(con2->icBcm),
                         (U8 *)&(con2->icBcm.svcHeader.transId),
                         (U16)sizeof(UConnId));
  if (ret != ROK)
  {
       CCLOGERR("**%s:ccicBCMInsTbl Insertion failed ret= %d\n",__FUNCTION__,ret);
       ABORT_DEBUG;
       ccRelCon(con2);
       ccHandleCleanUpConCb(con1,TRUE,CCTMPFAIL);
       RETVALUE(RFAILED);
  }

  if ((ret = ccAssignSaps(con2, &(con2->icRsc.intfc))) != ROK)
  {
    CCLOGERR("**%s: ccAssignSaps failed for con2, ret=%d\n", __FUNCTION__,ret);
    ABORT_DEBUG;
    ccRelCon(con2);
    ccHandleCleanUpConCb(con1,TRUE,CCTMPFAIL);
    RETVALUE(RFAILED);
  }

  /* Just copy pointer, well defined SAP interfaces */
  con2->icSapCb = con1->icSapCb;

  /* Struct Copy */
  con2->ainInfo = con1->ainInfo;
  /* Don't copy the preAnsTimeout, make sure it is 0 on con2 */
  con2->ainInfo.preAnsTimeout=0;

  /* Struct Copy */
  con2->genName = con1->genName;
  con2->jurisInfo = con1->jurisInfo;

  /* Copy remote SDP if it exists */

  if (con1->icIpParam.rmtDesc)
  {
      /* Allocated SDP for CON2 and copy from CON1 */
      cmGetCleanSdp(ccInit.region, ccInit.pool, &(con2->icIpParam.rmtDesc));
      cmCopySdp(ccInit.region, ccInit.pool, &(con2->icIpParam.rmtDesc),
                con1->icIpParam.rmtDesc);
  }

  /* FID-2593: Transcoding + */
  /* Copy icMsCb if it exists */
  if (con1->icMsCb)
  { 
      ccAllocMsCb(con2);
      cmMemcpy((U8 *)(con2->icMsCb), (U8 *)con1->icMsCb, sizeof(CcMediaStreams_t));
  } 
  /* FID-2593: Transcoding - */

  /* Extract AnalyzeRoute data from FIC into the icBcm to provide any updates
   * This extraction is done after copying AnalyzeRoute from con1 to con2
   */
  ccExtractAnalyzeRteParam(pAnalyzeRoute, &(con2->icBcm));
 
  /* Start the SETUP Timer on con2 */
  ccStartConTmr(TMR_SETUP, con2, (PTR)&ccCp.genCfg);
  ccCp.sts.totalCalls++;

  /* Send Success Outcome to FIC
   */

  /* Send Success OutCome on icBcm */
  CCDP(AIN_DBG_LEVEL_0, "%s: Sending Success OutCome:%ld\n", __FUNCTION__,con2->icSuConnId);
  ccSendSuccessOutCome(&con2->icBcm);

  /* Send Info Collected message to FIC and continue normal call processing
   */

  /* Send an InfoCollected  */
  /* Initializing the message buffers */
  cmMemset((U8 *)&header, 0, sizeof(SvcHeader_t));
  cmMemset((U8 *)&infoCollected, 0, sizeof(InfoCollected_t));

  /* Cleaning up the NEL as per R5-241 GR1298 */
  for (index = 0; index < EReqMax; index++)
  {
    con2->icBcm.nel[index] = NEL_UNARMED;
  }

  /* Fill in parameters in the Service Header Structure */
  ccFillSvcHeader(&header, &(con2->icBcm), MSG_Info_Collected);
  CCDP(AIN_DBG_LEVEL_1," Filled SvcHeader\n");

  /* Fill in parameters in the Info Collected Structure */
  ccFillInfoColltdParam(&infoCollected, &(con2->icBcm));
  CCDP(AIN_DBG_LEVEL_1," Filled InfoCollected\n");

  CC_STATE_CHANGE(con2, CCS_AWTRSPFORDPE3);
  
  CCDP(AIN_DBG_LEVEL_1,"%s: Sending InfoCollected\n",__FUNCTION__);
  CcUiFctInfoCollected(&infoCollected, &header);

  RETVALUE(ROK);
} /* End of ccConE77S00*/

/* 
 *      Fun:   ccConE78S95 
 *
 *      Desc:  Swap Call for CRBT, Waiting for OAnswer Response
 *             event - CCE_SWAPCALL (78)
 *             state - CCS_AWTRSPFORDPE08 (95) 
 */
#ifdef ANSI
PUBLIC  S16 ccConE78S95
(   
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                   /* bcm */
PTR     event2                   /* event */
)   
#else
PUBLIC S16 ccConE78S95(con, event, bcm, event2)
CcConCb *con;                    /* connection */
PTR     event;                   /* event */
AinBCM_t *bcm;                   /* bcm */
PTR     event2;                  /* event */
#endif
{
  SwapCall_t        *pSwapCall                = NULLP;
  CcConCb           *con1                     = NULLP;
  CcConCb           *con2                     = NULLP;
  AinBCM_t          *pBcm                     = NULLP;
  U8                direction                 = 0;


  if (event2 == NULLP)
  {
    CCLOGERR("Critical ERROR: Null pointers: event2=%ld\n", event2);
    ABORT_DEBUG;
    ccHandleCleanUpConCb(con,TRUE,CCTMPFAIL);
    RETVALUE(ROK);
  }

  /*  CCE_SWAPCALL received on con2 */ 
  con2 = con;

  CCDP(AIN_DBG_LEVEL_1, "%s: con2 - icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", __FUNCTION__,con2->icSuConnId, con2->ogSuConnId,
       (con2->mgBlock->mgCtx ? con2->mgBlock->mgCtx->suCtxId : 0), con2->icBcm.svcHeader.transId);

  pSwapCall = (SwapCall_t *) event2;


  /* Check and see if Move Occured and SWAP Event can be ignored 
   */
  
  if (con2->icRscVal)
  {
     CCDP(AIN_DBG_LEVEL_0, "%s: Move must of occured, ignore SWAP\n", __FUNCTION__);
     RETVALUE(ROK);
  }

  /* Extract Information into Call Control */ 
  ccExtractSwapCallParam(pSwapCall, &(con2->icBcm));

  /* Find Associated Connection Block based on assocTransId provided from FIC */
  /* Search the transaction Id in BCM Instance table */
  pBcm = ccFindBCMFromTransId(pSwapCall->assocTransId, direction);
  if (pBcm == NULLP)
  {
      CCLOGERR("%s - ERROR: BCM Not Found %lx\n", __FUNCTION__,pSwapCall->assocTransId);
      ABORT_DEBUG;
      ccHandleCleanUpConCb(con2,TRUE,CCTMPFAIL);
      RETVALUE(RFAILED);
  }
  con1            = pBcm->con;

  CCDP(AIN_DBG_LEVEL_1, "%s: con1 - icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", __FUNCTION__,con1->icSuConnId, con1->ogSuConnId,
       (con1->mgBlock->mgCtx ? con1->mgBlock->mgCtx->suCtxId : 0), con1->icBcm.svcHeader.transId);

  /* Stop the CC_TMR_PREANM timer if it is started */
  ccStopConTmr(con1, CC_TMR_PREANM);
  
  if (!CC_IS_ASSOCBLOCK_PRESENT(con2))
  {
     /* Assocation block not present */
     CCDP(AIN_DBG_LEVEL_0, "%s: assocBlock not present\n", __FUNCTION__);
     ABORT_DEBUG;
     ccHandleCleanUpConCb(con1,TRUE,CCTMPFAIL);
     ccHandleCleanUpConCb(con2,TRUE,CCTMPFAIL);
     RETVALUE(ROK);
  }

  /* Save swapType as OANSWER in assocBlock */
  CC_SET_SWAPTYPE_OANSWER(con2);
 
  /* Put con2 in state 68-CCS_CANNOTPROCESSEVENTS 
   */
  
  con2->lastState = con2->state;
  CC_STATE_CHANGE(con2, CCS_CANNOTPROCESSEVENTS);

  if (CC_IS_STATE_POST_INT_SWAP(con1)) 
  {
     /* Post CC_INT_SWAP_CALL to con1
      */
     ccPost_CC_INT_SWAP_CALL(con2->icSuConnId,con1->icSuConnId);
     RETVALUE(ROK);
  }

  /* Go ahead and do the move right here without posting to con1
   * Move A from con1 to con2 
   * o Change owner of resource, copy remoteSdpSentOnIcBCM, and
   *   do any MGCT requests as needed 
   * o If there are cntx, we will do post swap processing
   *   after MGCT work has been confirmed
   */
   ccProcessSwapRequest(con1,con2);
   RETVALUE(ROK);
} /* End of ccConE78S95 */

/* 
 *      Fun:   ccConE78S124 
 *
 *      Desc:  Swap Call for CRBT, Waiting for OTermSeized Response
 *             event - CCE_SWAPCALL (78)
 *             state - CCS_AWTRSPFORDPE7 (124) 
 */
#ifdef ANSI
PUBLIC  S16 ccConE78S124
(   
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                   /* bcm */
PTR     event2                   /* event */
)   
#else
PUBLIC S16 ccConE78S124(con, event, bcm, event2)
CcConCb *con;                    /* connection */
PTR     event;                   /* event */
AinBCM_t *bcm;                   /* bcm */
PTR     event2;                  /* event */
#endif
{
  SwapCall_t        *pSwapCall                = NULLP;
  CcConCb           *con1                     = NULLP;
  CcConCb           *con2                     = NULLP;
  AinBCM_t          *pBcm                     = NULLP;
  U8                direction                 = 0;

  if (event2 == NULLP)
  {
    CCLOGERR("Critical ERROR: Null pointers: event2=%ld\n", event2);
    ABORT_DEBUG;
    ccHandleCleanUpConCb(con,TRUE,CCTMPFAIL);
    RETVALUE(ROK);
  }

  /*  CCE_SWAPCALL received on con2 */ 
  con2 = con;

  CCDP(AIN_DBG_LEVEL_1, "%s: con2 - icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", __FUNCTION__,con2->icSuConnId, con2->ogSuConnId,
       (con2->mgBlock->mgCtx ? con2->mgBlock->mgCtx->suCtxId : 0), con2->icBcm.svcHeader.transId);

  pSwapCall = (SwapCall_t *) event2;


  /* Check and see if Move Occured and SWAP Event can be ignored 
   */
  
  if (con2->icRscVal)
  {
     CCDP(AIN_DBG_LEVEL_0, "%s: Move must of occured, ignore SWAP\n", __FUNCTION__);
     RETVALUE(ROK);
  }

  /* Extract Information into Call Control */
  ccExtractSwapCallParam(pSwapCall, &(con2->icBcm));
   
  /* Find Associated Connection Block based on assocTransId provided from FIC */
  /* Search the transaction Id in BCM Instance table */
  pBcm = ccFindBCMFromTransId(pSwapCall->assocTransId, direction);
  if (pBcm == NULLP)
  {
      CCLOGERR("%s - ERROR: BCM Not Found %lx\n", __FUNCTION__,pSwapCall->assocTransId);
      ABORT_DEBUG;
      ccHandleCleanUpConCb(con2,TRUE,CCTMPFAIL);
      RETVALUE(RFAILED);
  }
  con1            = pBcm->con;

  CCDP(AIN_DBG_LEVEL_1, "%s: con1 - icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", __FUNCTION__,con1->icSuConnId, con1->ogSuConnId,
       (con1->mgBlock->mgCtx ? con1->mgBlock->mgCtx->suCtxId : 0), con1->icBcm.svcHeader.transId);

  /* Stop the CC_TMR_PREANM timer if it is started */
  ccStopConTmr(con1, CC_TMR_PREANM);
  
  if (!CC_IS_ASSOCBLOCK_PRESENT(con2))
  {
     /* Assocation block not present */
     CCDP(AIN_DBG_LEVEL_0, "%s: assocBlock not present\n", __FUNCTION__);
     ABORT_DEBUG;
     ccHandleCleanUpConCb(con1,TRUE,CCTMPFAIL);
     ccHandleCleanUpConCb(con2,TRUE,CCTMPFAIL);
     RETVALUE(ROK);
  }

  /* Save swapType as OTERMSEIZED */
  CC_SET_SWAPTYPE_OTERMSEIZED(con2);
 
  /* Put con2 in state 68-CCS_CANNOTPROCESSEVENTS 
   */

  con2->lastState = con2->state;
  CC_STATE_CHANGE(con2, CCS_CANNOTPROCESSEVENTS);

  if (CC_IS_STATE_POST_INT_SWAP(con1)) 
  {
      /* Post CC_INT_SWAP_CALL to con1
       */
      ccPost_CC_INT_SWAP_CALL(con2->icSuConnId,con1->icSuConnId);
      RETVALUE(ROK);
  }

  /* Go ahead and do the move right here without posting to con1
   * Move A from con1 to con2 
   * o Change owner of resource, copy remoteSdpSentOnIcBCM, and
   *   do any MGCT requests as needed 
   * o If there are cntx, we will do post swap processing
   *   after MGCT work has been confirmed
   */
   ccProcessSwapRequest(con1,con2);
   RETVALUE(ROK);

} /* End of ccConE78S124 */


/* 
 *      Fun:   ccConE79S03 
 *
 *      Desc:  Move Call for CRBT, AWT for response of O_DISCONNECT   (far end) 
 *             event - CCE_MOVECALL (79)
 *             state - CCS_AWTRSPODISCONNECT (03) 
 */
#ifdef ANSI
PUBLIC  S16 ccConE79S03
(   
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                   /* bcm */
PTR     event2                   /* event */
)   
#else
PUBLIC S16 ccConE79S03(con, event, bcm, event2)
CcConCb *con;                    /* connection */
PTR     event;                   /* event */
AinBCM_t *bcm;                   /* bcm */
PTR     event2;                  /* event */
#endif
{
  S16               ret                       = RFAILED;
  MoveCall_t        *pMoveCall                = NULLP;
  CcConCb           *con1                     = NULLP;
  CcConCb           *con2                     = NULLP;
  AinBCM_t          *pBcm                     = NULLP;
  U8                direction                 = 0;

  if (event2 == NULLP)
  {
    CCLOGERR("Critical ERROR: Null pointers: event2=%ld\n", event2);
    ABORT_DEBUG;
    ccHandleCleanUpConCb(con,TRUE,CCTMPFAIL);
    RETVALUE(ret);
  }

  /*  CCE_MOVECALL received on con1 */ 
  con1 = con;

  CCDP(AIN_DBG_LEVEL_1, "%s: con1 - icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", __FUNCTION__,con1->icSuConnId, con1->ogSuConnId,
       (con1->mgBlock->mgCtx ? con1->mgBlock->mgCtx->suCtxId : 0), con1->icBcm.svcHeader.transId);
 
  pMoveCall = (MoveCall_t *) event2;

  /* Find Associated Connection Block based on assocTransId provided from FIC */
  /* Search the transaction Id in BCM Instance table */
  pBcm = ccFindBCMFromTransId(pMoveCall->assocTransId, direction);
  if (pBcm == NULLP)
  {
      CCLOGERR("%s - ERROR: BCM Not Found %lx\n", __FUNCTION__,pMoveCall->assocTransId);
      ABORT_DEBUG;
      ccHandleCleanUpConCb(con1,TRUE,CCTMPFAIL);
      RETVALUE(RFAILED);
  }
  con2            = pBcm->con;

  CCDP(AIN_DBG_LEVEL_1, "%s: con2 - icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", __FUNCTION__,con2->icSuConnId, con2->ogSuConnId,
       (con2->mgBlock->mgCtx ? con2->mgBlock->mgCtx->suCtxId : 0), con2->icBcm.svcHeader.transId);

  /* Stop the CC_TMR_PREANM timer if it is started */
  ccStopConTmr(con1, CC_TMR_PREANM);

  /* assocBlock is stored in CON2 */

  if (!CC_IS_ASSOCBLOCK_PRESENT(con2))
  {
     /* Assocation block not present */
     CCDP(AIN_DBG_LEVEL_0, "%s: assocBlock not present\n", __FUNCTION__);
     ABORT_DEBUG;
     ccHandleCleanUpConCb(con1,TRUE,CCTMPFAIL);
     ccHandleCleanUpConCb(con2,TRUE,CCTMPFAIL);
     RETVALUE(ret);
  }

  /* Clear sendRelease in assocBlock, ODISCONNECT */
  CC_SET_SENDRELEASE(con2,FALSE);

  if (con2->icRscVal)
  {
     CCDP(AIN_DBG_LEVEL_0, "%s: MOVE must of occured, ignore MOVE\n", __FUNCTION__);
     RETVALUE(ROK);
  }

  /* Extract Information into Call Control */
  ccExtractMoveCallParam(pMoveCall,&(con1->icBcm));

  /* Put con1 in state 68-CCS_CANNOTPROCESSEVENTS 
   */

  con1->lastState = con1->state;
  CC_STATE_CHANGE(con1, CCS_CANNOTPROCESSEVENTS);

  if (CC_IS_STATE_POST_INT_MOVE(con2)) 
  {
      /* Post CC_INT_MOVE_CALL to con2
       */
       ccPost_CC_INT_MOVE_CALL(con1->icSuConnId,con2->icSuConnId);
       RETVALUE(ROK);
  }

  /* Go ahead and do the move right here without posting to con2
   * Move A from con1 to con2 
   */

  /* If there are cntx, we will do post move processing based on
   * sendRelease in assocBlock after MGCT work has been confirmed
   *
   * o Change owner of resource, copy remoteSdpSentOnIcBCM, and
   *   do any MGCT requests as needed 
   */

   ccProcessMoveRequest(con1,con2);
   RETVALUE(ROK);
} /* End of ccConE79S03 */

/* 
 *      Fun:   ccConE79S99 
 *
 *      Desc:  Move Call for CRBT, Awaiting O_Suspend response - ACC 
 *             event - CCE_MOVECALL (79)
 *             state - CCS_AWTRSPFORDPE12 (99) 
 */
#ifdef ANSI
PUBLIC  S16 ccConE79S99
(   
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                   /* bcm */
PTR     event2                   /* event */
)   
#else
PUBLIC S16 ccConE79S99(con, event, bcm, event2)
CcConCb *con;                    /* connection */
PTR     event;                   /* event */
AinBCM_t *bcm;                   /* bcm */
PTR     event2;                  /* event */
#endif
{
  S16               ret                       = RFAILED;
  MoveCall_t        *pMoveCall                = NULLP;
  CcConCb           *con1                     = NULLP;
  CcConCb           *con2                     = NULLP;
  AinBCM_t          *pBcm                     = NULLP;
  U8                direction                 = 0;

  if (event2 == NULLP)
  {
    CCLOGERR("Critical ERROR: Null pointers: event2=%ld\n", event2);
    ABORT_DEBUG;
    ccHandleCleanUpConCb(con,TRUE,CCTMPFAIL);
    RETVALUE(ret);
  }

  /*  CCE_MOVECALL received on con1 */ 
  con1 = con;

  CCDP(AIN_DBG_LEVEL_1, "%s: con1 - icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", __FUNCTION__,con1->icSuConnId, con1->ogSuConnId,
       (con1->mgBlock->mgCtx ? con1->mgBlock->mgCtx->suCtxId :0), con1->icBcm.svcHeader.transId);
 
  pMoveCall = (MoveCall_t *) event2;

  /* Find Associated Connection Block based on assocTransId provided from FIC */
  /* Search the transaction Id in BCM Instance table */
  pBcm = ccFindBCMFromTransId(pMoveCall->assocTransId, direction);
  if (pBcm == NULLP)
  {
      CCLOGERR("%s - ERROR: BCM Not Found %lx\n", __FUNCTION__,pMoveCall->assocTransId);
      ABORT_DEBUG;
      ccHandleCleanUpConCb(con1,TRUE,CCTMPFAIL);
      RETVALUE(RFAILED);
  }
  con2            = pBcm->con;

  CCDP(AIN_DBG_LEVEL_1, "%s: con2 - icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", __FUNCTION__,con2->icSuConnId, con2->ogSuConnId,
       (con2->mgBlock->mgCtx ? con2->mgBlock->mgCtx->suCtxId : 0), con2->icBcm.svcHeader.transId);

  /* Stop the CC_TMR_PREANM timer if it is started */
  ccStopConTmr(con1, CC_TMR_PREANM);

  /* assocBlock is stored in CON2 */

  if (!CC_IS_ASSOCBLOCK_PRESENT(con2))
  {
     /* Assocation block not present */
     CCDP(AIN_DBG_LEVEL_0, "%s: assocBlock not present\n", __FUNCTION__);
     ABORT_DEBUG;
     ccHandleCleanUpConCb(con1,TRUE,CCTMPFAIL);
     ccHandleCleanUpConCb(con2,TRUE,CCTMPFAIL);
     RETVALUE(ret);
  }

  /* Set sendRelease in assocBlock, OSUSPEND case */
  CC_SET_SENDRELEASE(con2,TRUE);

  if (con2->icRscVal)
  {
     CCDP(AIN_DBG_LEVEL_0, "%s: MOVE must of occured, ignore MOVE\n", __FUNCTION__);
     RETVALUE(ROK);
  }

  /* Extract Information into Call Control */
  ccExtractMoveCallParam(pMoveCall,&(con1->icBcm));

  /* Put con1 in state 68-CCS_CANNOTPROCESSEVENTS 
   */

  con1->lastState = con1->state;
  CC_STATE_CHANGE(con1, CCS_CANNOTPROCESSEVENTS);

  if (CC_IS_STATE_POST_INT_MOVE(con2)) 
  {
      /* Post CC_INT_MOVE_CALL to con2
       */
       ccPost_CC_INT_MOVE_CALL(con1->icSuConnId,con2->icSuConnId);
       RETVALUE(ROK);
  }

  /* Go ahead and do the move right here without posting to con2
   * Move A from con1 to con2 
   */

  /* If there are cntx, we will do post move processing based on
   * sendRelease in assocBlock after MGCT work has been confirmed
   *
   * o Change owner of resource, copy remoteSdpSentOnIcBCM, and
   *   do any MGCT requests as needed 
   */
  ccProcessMoveRequest(con1,con2);
  RETVALUE(ROK);
} /* End of ccConE79S99 */

/* 
 *      Fun:   ccConE79S123 
 *
 *      Desc:  Move Call for CRBT, AWT FIC for I-Disconnect Response
 *             event - CCE_MOVECALL (79)
 *             state - CCS_AWTIDISCONNECT (123) 
 */
#ifdef ANSI
PUBLIC  S16 ccConE79S123
(   
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                   /* bcm */
PTR     event2                   /* event */
)   
#else
PUBLIC S16 ccConE79S123(con, event, bcm, event2)
CcConCb *con;                    /* connection */
PTR     event;                   /* event */
AinBCM_t *bcm;                   /* bcm */
PTR     event2;                  /* event */
#endif
{
  S16               ret                       = RFAILED;
  MoveCall_t        *pMoveCall                = NULLP;
  CcConCb           *con1                     = NULLP;
  CcConCb           *con2                     = NULLP;
  AinBCM_t          *pBcm                     = NULLP;
  U8                direction                 = 0;

  if (event2 == NULLP)
  {
    CCLOGERR("Critical ERROR: Null pointers: event2=%ld\n", event2);
    ABORT_DEBUG;
    ccHandleCleanUpConCb(con,TRUE,CCTMPFAIL);
    RETVALUE(ret);
  }

  /*  CCE_MOVECALL received on con1 */ 
  con1 = con;

  CCDP(AIN_DBG_LEVEL_1, "%s: con1 - icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", __FUNCTION__,con1->icSuConnId, con1->ogSuConnId,
       (con1->mgBlock->mgCtx ? con1->mgBlock->mgCtx->suCtxId : 0), con1->icBcm.svcHeader.transId);
 
  pMoveCall = (MoveCall_t *) event2;


  /* Find Associated Connection Block based on assocTransId provided from FIC */
  /* Search the transaction Id in BCM Instance table */
  pBcm = ccFindBCMFromTransId(pMoveCall->assocTransId, direction);
  if (pBcm == NULLP)
  {
      CCLOGERR("%s - ERROR: BCM Not Found %lx\n", __FUNCTION__,pMoveCall->assocTransId);
      ABORT_DEBUG;
      ccHandleCleanUpConCb(con1,TRUE,CCTMPFAIL);
      RETVALUE(RFAILED);
  }
  con2            = pBcm->con;

  CCDP(AIN_DBG_LEVEL_1, "%s: con2 - icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", __FUNCTION__,con2->icSuConnId, con2->ogSuConnId,
      (con2->mgBlock->mgCtx ?  con2->mgBlock->mgCtx->suCtxId : 0), con2->icBcm.svcHeader.transId);

  /* Stop the CC_TMR_PREANM timer if it is started */
  ccStopConTmr(con1, CC_TMR_PREANM);

  /* assocBlock is stored in CON2 */

  if (!CC_IS_ASSOCBLOCK_PRESENT(con2))
  {
     /* Assocation block not present */
     CCDP(AIN_DBG_LEVEL_0, "%s: assocBlock not present\n", __FUNCTION__);
     ABORT_DEBUG;
     ccHandleCleanUpConCb(con1,TRUE,CCTMPFAIL);
     ccHandleCleanUpConCb(con2,TRUE,CCTMPFAIL);
     RETVALUE(ret);
  }

  /* Set sendRelease in assocBlock, IDISCONNECT case */
  CC_SET_SENDRELEASE(con2,TRUE);

  /* Check and see if SWAP Occured and MOVE Event can be ignored 
   */

  if (con2->icRscVal)
  {
     CCDP(AIN_DBG_LEVEL_0, "%s: MOVE must of occured, ignore MOVE\n", __FUNCTION__);
     RETVALUE(ROK);
  }

  /* Extract Information into Call Control */
  ccExtractMoveCallParam(pMoveCall,&(con1->icBcm));

  /* Put con1 in state 68-CCS_CANNOTPROCESSEVENTS 
   */

  con1->lastState = con1->state;
  CC_STATE_CHANGE(con1, CCS_CANNOTPROCESSEVENTS);

  if (CC_IS_STATE_POST_INT_MOVE(con2)) 
  {
      /* Post CC_INT_MOVE_CALL to con2
       */
       ccPost_CC_INT_MOVE_CALL(con1->icSuConnId,con2->icSuConnId);
       RETVALUE(ROK);
  }

  /* Go ahead and do the move right here without posting to con2
   * Move A from con1 to con2 
   */

  /* If there are cntx, we will do post move processing based on
   * sendRelease in assocBlock after MGCT work has been confirmed
   *
   * o Change owner of resource, copy remoteSdpSentOnIcBCM, and
   *   do any MGCT requests as needed 
   */
   ccProcessMoveRequest(con1,con2);
   RETVALUE(ROK);

} /* End of ccConE79S123 */

/* 
 *      Fun:   ccConE80SXX 
 *
 *      Desc:  Internal Swap Call for CRBT, AWT for response of O_DISCONNECT (far end) 
 *             event - CCE_INT_SWAPCALL (80)
 *             state - CCS_AWTRSPODISCONNECT (03) 
 *             state - CCS_AWTANS (09)
 *             state - CCS_ANSWERED (10)
 *             state - CCS_AWTMGCTNOTIFY1 (30)
 */
#ifdef ANSI
PUBLIC  S16 ccConE80SXX
(   
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                   /* bcm */
PTR     event2                   /* event */
)   
#else
PUBLIC S16 ccConE80SXX(con, event, bcm, event2)
CcConCb *con;                    /* connection */
PTR     event;                   /* event */
AinBCM_t *bcm;                   /* bcm */
PTR     event2;                  /* event */
#endif
{
  S16               ret                       = RFAILED;
  CcConCb           *con1                     = NULLP;
  CcConCb           *con2                     = NULLP;
  InternalSwapCall_t *swapCallEvent;
  
  if (event == NULLP)
  {
    CCLOGERR("Critical ERROR: Null pointers: event=%ld\n", event);
    ABORT_DEBUG;
    ccHandleCleanUpConCb(con,TRUE,CCTMPFAIL);
    RETVALUE(ret);
  }

  swapCallEvent = (InternalSwapCall_t *)event;
  /* CCE_INT_SWAPCALL received on con1 */ 
  con1 = con;
  ret = cmHashListFind (&ccSuInstTbl, (U8 *)&swapCallEvent->suConnIdSender, (U16)sizeof(UConnId),
                         (U16)0, (PTR *) &con2);
  if (ret != ROK)
  {
      DP("Did not find the conSender\n");
      ABORT_DEBUG;
      ccHandleCleanUpConCb(con1,TRUE,CCTMPFAIL);
      RETVALUE(ret);
  }

  CCDP(AIN_DBG_LEVEL_1, "%s:state:%s, icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", __FUNCTION__,ccStateStr(con1->state),con1->icSuConnId, con1->ogSuConnId,
       (con1->mgBlock->mgCtx ? con1->mgBlock->mgCtx->suCtxId : 0), con1->icBcm.svcHeader.transId);
 
  /* Move A from con1 to con2 
   * o Change owner of resource, copy remoteSdpSentOnIcBCM, and
   *   do any MGCT requests as needed 
   * o If there are cntx, we will do post swap processing
   *   after MGCT work has been confirmed
   */
   ccProcessSwapRequest(con1,con2);
   RETVALUE(ROK);
} /* End of ccConE80SXX */

/* 
 *      Fun:   ccConE80SBUF 
 *
 *      Desc:  Internal Swap Call for CRBT, Buffer Event 
 *             event - CCE_INT_SWAPCALL (80)
 *             state - CCS_AWTANSSWT (7)
 *                     CCS_AWTSWTCFM (8)
 *                     CCS_AWTOGSWTCFM (28)
 *                     CCS_AWTMGCTCFMCOTOG0 (29)
 *                     CCS_AWTRSPFORDPE20 (39)
 *                     CCS_AWTMGCTCFMCOTOG1 (92) 
 */
#ifdef ANSI
PUBLIC  S16 ccConE80SBUF
(   
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                   /* bcm */
PTR     event2                   /* event */
)   
#else
PUBLIC S16 ccConE80SBUF(con, event, bcm, event2)
CcConCb *con;                    /* connection */
PTR     event;                   /* event */
AinBCM_t *bcm;                   /* bcm */
PTR     event2;                  /* event */
#endif
{
  S16               ret            = RFAILED;

  if ((con == NULLP) || (event == NULLP))
  {
    CCLOGERR("Critical ERROR: Null pointers:%lx, %ld\n", (U32)con, event);
    RETVALUE(ret);
  }

  CCDP(AIN_DBG_LEVEL_1, "%s:icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", __FUNCTION__,con->icSuConnId, con->ogSuConnId,
       (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0), con->icBcm.svcHeader.transId);

  ccBufferEventToQueue(con, bcm, event, CCE_INT_SWAPCALL, sizeof(InternalSwapCall_t),
                       0, NULLP, 0);
  RETVALUE(ROK);
} /* End of ccConE80SBUF */

/* 
 *      Fun:   ccConE81S09 
 *
 *      Desc:  Internal Move Call for CRBT, awaiting answer 
 *             event - CCE_INT_MOVECALL (81)
 *             state - CCS_AWTANS (9)
 */
#ifdef ANSI
PUBLIC  S16 ccConE81S09
(   
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                   /* bcm */
PTR     event2                   /* event */
)   
#else
PUBLIC S16 ccConE81S09(con, event, bcm, event2)
CcConCb *con;                    /* connection */
PTR     event;                   /* event */
AinBCM_t *bcm;                   /* bcm */
PTR     event2;                  /* event */
#endif
{
  S16               ret            = RFAILED;
  CcConCb           *con1                     = NULLP;
  CcConCb           *con2                     = NULLP;
  InternalMoveCall_t *moveCallEvent;

  if (event == NULLP)
  {
    CCLOGERR("Critical ERROR: Null pointers: event=%ld\n", event);
    ABORT_DEBUG;
    ccHandleCleanUpConCb(con,TRUE,CCTMPFAIL);
    RETVALUE(ret);
  }

  moveCallEvent = (InternalMoveCall_t *)event;
  /* CCE_INT_MOVECALL received on con2 */ 
  con2 = con;

  ret = cmHashListFind (&ccSuInstTbl, (U8 *)&moveCallEvent->suConnIdSender, (U16)sizeof(UConnId),
                         (U16)0, (PTR *) &con1);
  if (ret != ROK)
  {
      DP("Did not find the conSender\n");
      ABORT_DEBUG;
      ccHandleCleanUpConCb(con2,TRUE,CCTMPFAIL);
      RETVALUE(ret);
  }

  CCDP(AIN_DBG_LEVEL_1, "%s-con1:icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", __FUNCTION__,con1->icSuConnId, con1->ogSuConnId,
       (con1->mgBlock->mgCtx ? con1->mgBlock->mgCtx->suCtxId : 0), con1->icBcm.svcHeader.transId);

  /* 
   * Move A from con1 to con2 
   */

  /* Change owner of resource, copy remoteSdpSentOnIcBCM, and
   * do any MGCT requests as needed 
   */
  ccProcessMoveRequest(con1,con2);
  RETVALUE(ROK);
} /* End of ccConE81S09 */

/* 
 *      Fun:   ccConE81SBUF 
 *
 *      Desc:  Internal Move Call for CRBT, Buffer Event 
 *             event - CCE_INT_MOVECALL (81)
 *             state - CCS_AWTANSSWT (7)
 *                     CCS_AWTSWTCFM (8)
 *                     CCS_AWTOGSWTCFM (28)
 *                     CCS_AWTMGCTCFMCOTOG0 (29)
 *                     CCS_AWTRSPFORDPE20 (37)
 *                     CCS_AWTRSPFORDPE20 (39)
 *                     CCS_AWTMGCTCFMCOTOG1 (92) 
 */
#ifdef ANSI
PUBLIC  S16 ccConE81SBUF
(   
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                   /* bcm */
PTR     event2                   /* event */
)   
#else
PUBLIC S16 ccConE81SBUF(con, event, bcm, event2)
CcConCb *con;                    /* connection */
PTR     event;                   /* event */
AinBCM_t *bcm;                   /* bcm */
PTR     event2;                  /* event */
#endif
{
  S16               ret            = RFAILED;

  if ((con == NULLP) || (event == NULLP))
  {
    CCLOGERR("Critical ERROR: Null pointers:%lx, %ld\n", (U32)con, event);
    RETVALUE(ret);
  }

  CCDP(AIN_DBG_LEVEL_1, "%s:icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", __FUNCTION__,con->icSuConnId, con->ogSuConnId,
       (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0), con->icBcm.svcHeader.transId);
 
  ccBufferEventToQueue(con, bcm, event, CCE_INT_MOVECALL, sizeof(InternalMoveCall_t),
                       0, NULLP, 0);
  RETVALUE(ROK);
} /* End of ccConE81SBUF */

/* 
 *      Fun:   ccHandleE82Exp
 *
 *      Desc:
 *             event - CCE_PREANMTMREXP (82)
 *             state - 
*/
PUBLIC  S16 ccHandleE82Exp
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  ccProcessPreANMTimerExp(con);
  RETVALUE(ROK);
}

/* 
 *      Fun:   ccConE82SBUF
 *
 *      Desc:
 *             event - CCE_PREANMTMREXP (82)
 *             state - 
*/
PUBLIC  S16 ccConE82SBUF
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  CCDP(AIN_DBG_LEVEL_1, "%s:icSu:%ld, ogSu:%ld\n",
       __FUNCTION__,con->icSuConnId, con->ogSuConnId);

  ccBufferEventToQueue(con, bcm, event, CCE_PREANMTMREXP, sizeof(U8), 0,
                       NULLP, 0);
  RETVALUE(ROK);
}

/*
*
*      Fun:   ccConE11S121
*
*      Desc:  Connection state function
*             event - MGCT Txn Cfm
*             state - CCS_AWTTMGCTFORSWAPCALL
*
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes:
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC  S16 ccConE11S121
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
    S16              ret               = RFAILED;
    MgctTxnCfm      *txnCfm            = NULLP;
    CcConCb *con1;                    /* connection */
    CcConCb *con2;                    /* connection */

    txnCfm   = (MgctTxnCfm*) event;
    con2 = con;

    CCDP(AIN_DBG_LEVEL_1, "%s:icSu:%ld, ogSu:%ld\n",
       __FUNCTION__,con->icSuConnId, con->ogSuConnId);

    ret = ccHandleMgResponse(con2, txnCfm);
    if (ret != ROK)
    {
        CCLOGERR("Error: %s: ccHandleMgResponse failed\n", __FUNCTION__);
        if (CC_IS_ASSOCBLOCK_PRESENT(con2))
        {
           /* Get associated call */
           /* Find Associated Connection Block based on assocTransId provided from FIC */
           ret = cmHashListFind(&ccSuInstTbl,(U8 *)&(CC_GET_ASSOCPARENTSUCONNID(con2)),
                                (U16)sizeof(UConnId), 0, (PTR *)&con1);
           if (ret != ROK)
           {
              CCLOGERR("%s - ERROR: Connection Block Not Found %lx\n", __FUNCTION__,CC_GET_ASSOCPARENTSUCONNID(con2));
              ABORT_DEBUG;
              cleanUpMgQ(con2);
              ccHandleCleanUpConCb(con2,TRUE,CCTMPFAIL);
              RETVALUE(ROK);
           }
           cleanUpMgQ(con2);
           ccHandleCleanUpConCb(con2,TRUE,CCTMPFAIL);
           ccHandleCleanUpConCb(con1,TRUE,CCTMPFAIL);
        }
        else
        {
           CCLOGERR("Critical ERROR: Null pointer for assocblock\n");
           ABORT_DEBUG;
           cleanUpMgQ(con2);
           ccHandleCleanUpConCb(con2,TRUE,CCTMPFAIL);
           RETVALUE(ROK);
        }
    }
    else
    {
       /* Complete processing of the SWAPCALL based on Swap Type
        */
       ccProcessSwapType(con2);
    }
    RETVALUE(ROK);
}/* ccConE11S121 */

/*
*
*      Fun:   ccConE11S122
*
*      Desc:  Connection state function
*             event - MGCT Txn Cfm
*             state - CCS_AWTTMGCTFORMOVECALL
*
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes:
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC  S16 ccConE11S122
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
    S16              ret               = RFAILED;
    MgctTxnCfm      *txnCfm            = NULLP;
    CcConCb         *con1              = NULLP;
    CcConCb         *con2              = NULLP;

    con2 = con;

    txnCfm   = (MgctTxnCfm*) event;

    CCDP(AIN_DBG_LEVEL_1, "%s:icSu:%ld, ogSu:%ld\n",
       __FUNCTION__,con->icSuConnId, con->ogSuConnId);

    ret = ccHandleMgResponse(con2, txnCfm);
    if (ret != ROK)
    {
        if (CC_IS_ASSOCBLOCK_PRESENT(con2))
        {
            /* Get associated call, con1 */
            /* Find Associated Connection Block based on assocTransId provided from FIC */
            ret = cmHashListFind(&ccSuInstTbl,(U8 *)&(CC_GET_ASSOCPARENTSUCONNID(con2)),
                                (U16)sizeof(UConnId), 0, (PTR *)&con1);
            if (ret != ROK)
            {
                 CCLOGERR("%s - ERROR: Connection Block Not Found %lx\n", __FUNCTION__,CC_GET_ASSOCPARENTSUCONNID(con2));
                 ABORT_DEBUG;
                 cleanUpMgQ(con2);
                 ccHandleCleanUpConCb(con2,TRUE,CCTMPFAIL);
                 RETVALUE(ROK);
            }
            cleanUpMgQ(con2);
            ccHandleCleanUpConCb(con2,TRUE,CCTMPFAIL);
            ccHandleCleanUpConCb(con1,TRUE,CCTMPFAIL);
        }
        else
        {
           CCLOGERR("Critical ERROR: Null pointer for assocblock\n");
           ABORT_DEBUG;
           cleanUpMgQ(con2);
           ccHandleCleanUpConCb(con2,TRUE,CCTMPFAIL);
           RETVALUE(ROK);
        }
    }
    else
    {
        /* Make sure we have a valid pointer */
        if (CC_IS_ASSOCBLOCK_PRESENT(con2))
        {
            /* Get associated call, con1 */
            /* Find Associated Connection Block based on assocTransId provided from FIC */
            ret = cmHashListFind(&ccSuInstTbl,(U8 *)&(CC_GET_ASSOCPARENTSUCONNID(con2)),
                                (U16)sizeof(UConnId), 0, (PTR *)&con1);
            if (ret != ROK)
            {
                 CCLOGERR("%s - ERROR: Connection Block Not Found %lx\n", __FUNCTION__,CC_GET_ASSOCPARENTSUCONNID(con2));
                 ABORT_DEBUG;
                 ccHandleCleanUpConCb(con2,TRUE,CCTMPFAIL);
            }
            CCDP(AIN_DBG_LEVEL_1, "ccProcessSwapType:con1 - icSu:%ld, ogSu:%ld, suCtxId:%ld,"
                 " icTransId:%ld\n", con1->icSuConnId, con1->ogSuConnId,
                 (con1->mgBlock->mgCtx ? con1->mgBlock->mgCtx->suCtxId : 0), con1->icBcm.svcHeader.transId);

            /* Complete processing of the MOVECALL based on sendRelease in assocBlock.
             */
            ccProcessMoveSendRelease(con1,con2);
        }
        else
        {
            CCLOGERR("Critical ERROR: Null pointer for assocblock\n");
            ABORT_DEBUG;
            ccHandleCleanUpConCb(con2,TRUE,CCTMPFAIL);
            RETVALUE(ROK);
        } 
    }
    RETVALUE(ROK);
}/* ccConE11S122 */

/*
 *
 *      Fun:   ccConE84S37
 *
 *      Desc:  Connection state function
 *             event - CCE_TESTLINE
 *             state - CCS_AWTRSPFORDPE3 (AWT for RSP for MSG sent on DP E3)
 *      Ret:   ROK     - successful,
 *             RFAILED - unsuccessful
 *
 *      Notes: None.
 *
 *      File:  cc_bdy2_2.c
 *
 */
PUBLIC S16 ccConE84S37
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                   /* bcm */
PTR     event2                   /* event */
)
{
   S16			ret;
   Bool			saveAnsInd;
   ProtType		icProtType = CC_GET_PROTOCOL(con->icProtType);
   SvcHeader_t          *tlSvcHeader = NULLP;

#if 0
   U8                   tmrNum =0;
   Bool                 allDtlTmd = FALSE, allDtlStatsTmd = FALSE;
#endif
#ifdef ZC
   ZcUpdParam           upd;
#endif /* ZC */



   TRC3(ccConE84S37)
   CCDP(AIN_DBG_LEVEL_1, "ccConE84S37: Received TestLine, type:%d in AWTRSPFORDPE3,"
                         " con->icProtType=%d  icProtType =%d  direction=%d AnsInd = %d\n",
                          con->tstLnCb->testLineType, con->icProtType, icProtType, 
                          con->direction, con->ainInfo.AnswerIndicator);

   ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
   tlSvcHeader = (SvcHeader_t *) event;
   if (tlSvcHeader->lastTransactionFlag == TRUE)
   {
      ccDeleteBcm(&(con->icBcm));
   }

   ccDeriveCcn(con);  /* FID 15261.0 */

   switch(con->tstLnCb->testLineType)
   {
      case 108:
         con->tstLnCb->currentSignal = CC_TST_108_LOOPBACK;
         if((icProtType ==  CC_SI) || (icProtType ==  CC_IN))
         {
            /* Need to send an CnStInd (ACM) for ISUP */
            if(icProtType == CC_SI)
            {
               /* Save away the answer indicator, change it to No to 
                * force the connection status indication to be sent.
                */
               saveAnsInd = con->ainInfo.AnswerIndicator;
               con->ainInfo.AnswerIndicator = FALSE;

               ret = ccSendBackwardMessageForAnnoucement(con, RtPlayAnnouncements, FALSE, FALSE);
               if (ret != ROK)
               {
                   CCLOGERR("%s: Error, ccSendBackwardMessageForAnnoucement CnStInd (ACM)"
                            " failed icSu=0x%lx\n", __FUNCTION__,con->icSuConnId);
                   ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
                   RETVALUE(ret);
               }
               /* Restore the answer indicator */
               con->ainInfo.AnswerIndicator = saveAnsInd; 
            }

            ret = ccSendBackwardMessageForAnnoucement(con, RtPlayAnnouncements, FALSE, FALSE);
            /* Populate message for loopback signal */
            ccBldAndSendTstLnSig (con, CC_TST_108_LOOPBACK, CCS_AWT_MGCTCFM_LLTR, MGCT_CMD_ADD);
         }
         else /* CAS */
         {

            CCDP(AIN_DBG_LEVEL_1, "ccConE84S37 - Sending CctConRsp for CAS"
                                 " icSp:0x%lx,  icSu:0x%lx\n",con->icSpConnId, con->icSuConnId);
            ccBldAndSendConRsp(con);
            ccBldAndSendTstLnSig(con, CC_TST_108_LOOPBACK, CCS_AWT_MGCTCFM_LLTR, MGCT_CMD_MODIFY);
         }
         break;

      case 100:
      case 102:
         if((icProtType ==  CC_SI) || (icProtType ==  CC_IN))
         {
            /* Need to send an CnStInd (ACM) for ISUP */
            if(icProtType == CC_SI)
            {
               /* Save away the answer indicator, change it to No to 
                * force the connection status indication to be sent.
                */
               saveAnsInd = con->ainInfo.AnswerIndicator;
               con->ainInfo.AnswerIndicator = FALSE;

               ret = ccSendBackwardMessageForAnnoucement(con, RtPlayAnnouncements, FALSE, FALSE);
               if (ret != ROK)
               {
                   CCLOGERR("%s: Error, ccSendBackwardMessageForAnnoucement  CnStInd (ACM)"
                            " failed icSu=0x%lx\n", __FUNCTION__,con->icSuConnId);
                   ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
                   RETVALUE(ret);
               }
               /* Restore the answer indicator */
               con->ainInfo.AnswerIndicator = saveAnsInd; 
            }

            ret = ccSendBackwardMessageForAnnoucement(con, RtPlayAnnouncements, FALSE, FALSE);
            if (ret != ROK)
            {
               CCLOGERR("%s: Error, ccSendBackwardMessageForAnnouncement (ConRsp) failed"
                        " icSu=0x%lx\n", __FUNCTION__,con->icSuConnId);
               ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
               RETVALUE(ret);
            }
         }
         else  /* CAS */
         {
            CCDP(AIN_DBG_LEVEL_1, "ccConE84S37 - Sending CctConRsp for CAS"
                                 " icSp:0x%lx,  icSu:0x%lx\n",con->icSpConnId, con->icSuConnId);

            ccBldAndSendConRsp(con);

         }
         /* Administer timers */
         if (con->tstLnCb->testLineType == 102)
         {
            ccStartCon100Tmr(CC_TST_102_SLNC1_TMR, con);
            con->tstLnCb->currentSignal = CC_TST_102_SILENCE;
         }
         else
         {
            ccStartCon100Tmr(CC_TST_100_SLNC_TMR, con);
            con->tstLnCb->currentSignal = CC_TST_100_SILENCE;
         }
         ccStopConTmr(con, TMR_SETUP);
         ccStartConTmr(CC_TMR_TST_LN_DSC, con, (PTR)&ccCp.genCfg);
         CC_STATE_CHANGE(con, CCS_LOCALLYANSWERED);
         /* Since the Test Call is a one ended call, need to fill in ogRsc which
          * will be copied into termRsc of CcCallDtlRcrd to make billing 
          * happy to generate a record.
          */
         con->ogRsc.intfc.intfType = CC_LOCAL_SWT_PROT_INTFC;
         CCDP(AIN_DBG_LEVEL_1, "ccConE84S37: SETTING" 
               "con->ogRsc.intfc.intfType = %d\n",
                con->ogRsc.intfc.intfType);
         if ((con->callDtlInfo.cm.tckOgConCfmRcvd.pres != PRSNT_NODEF) &&
             (con->keepGCCCallInfo == FALSE))
         {
            con->callDtlInfo.cm.tckOgConCfmRcvd.pres = PRSNT_NODEF;
            (Void) SGetSysTime(&con->callDtlInfo.cm.tckOgConCfmRcvd.val);
            CCDP(AIN_DBG_LEVEL_1, "ccConE84S37: SETTING" 
              "con->callDtlInfo.cm.tckOgConCfmRcvd.pres=%d ,val=%ld\n", 
              con->callDtlInfo.cm.tckOgConCfmRcvd.pres,con->callDtlInfo.cm.tckOgConCfmRcvd.val);
         }
         CCDP(AIN_DBG_LEVEL_1, "ccConE84S37:callDtlInfo.cm.tckStartLongDur.pres =%d\n", 
              con->callDtlInfo.cm.tckStartLongDur.pres);
        if (con->callDtlInfo.cm.tckStartLongDur.pres != NOTPRSNT) 
        {
             con->callDtlInfo.cm.tckStartLongDur.pres = NOTPRSNT;
             CCDP(AIN_DBG_LEVEL_1, "ccConE84S37: Setting callDtlInfo.cm.tckStartLongDur.pres =%d\n", 
             con->callDtlInfo.cm.tckStartLongDur.pres);
         }

#ifdef ZC
         cmMemset((U8*) &upd, 0, sizeof(ZcUpdParam));
         upd.con                  = con;
         upd.con->ogSapCb         = NULLP;
         upd.con->ogIntfcCb       = NULLP;
         zcRunTimeUpd(ZC_CON_CB, CMPFTHA_CREATE_REQ, upd);
         zcUpdPeer();
#endif /* ZC */
         break;

      default:
         CCLOGERR( "ERROR: testLineType: %d is invalid. Not handling TestLine Message\n",
                    con->tstLnCb->testLineType);
         RETVALUE(RFAILED);
         break;
   } /* End switch on test line */

   RETVALUE(ROK);
} /* End func ccConE84S37 */

/*
 *
 *      Fun:   ccConE11S134 
 *
 *      Desc:  Connection state function 
 *             event - Switching Connect Confirm
 *             state - CCS_AWT_MGCTCFM_LLTR
 *
 *      Ret:   ROK     - successful,
 *             RFAILED - unsuccessful
 *
 *      Notes:
 *
 *      File:  cc_bdy2_2.c
 *
 */
PUBLIC  S16 ccConE11S134
(
CcConCb *con,                     /* connection */ 
PTR     event,                    /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
   MgctTxnCfm		*txnCfm  = NULLP;
   S16			ret = RFAILED;
#if 0
   U8                   tmrNum =0;
   Bool                 allDtlTmd = FALSE, allDtlStatsTmd = FALSE;
#endif
#ifdef ZC
   ZcUpdParam           upd;
#endif /* ZC */

   CCDP(AIN_DBG_LEVEL_1, "ccConE11S134:%ld, %ld\n", con->icSuConnId,
                        con->ogSuConnId);
   txnCfm   = (MgctTxnCfm*) event;
   ret = ccHandleMgResponse(con, txnCfm);
   if (ret == RFAILED) /* CPDI_FIX */
   {
      CCLOGERR( "ERROR: Response Handling FAILED\n");
      cleanUpMgQ(con);
      ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
      RETVALUE(RFAILED);
   }

   /* Administer timers and state */
   ccStopConTmr(con, TMR_SETUP);
   ccStartConTmr(CC_TMR_TST_LN_DSC, con, (PTR)&ccCp.genCfg);
   /* Bug 62665: Since the Test Call is a one ended call, need to fill 
    * in ogRsc which will be copied into termRsc of CcCallDtlRcrd to 
    * make billing happy to generate a record.
    */
   con->ogRsc.intfc.intfType = CC_LOCAL_SWT_PROT_INTFC;
   CCDP(AIN_DBG_LEVEL_1, "ccConE11S134: SETTING"
         "con->ogRcs.intfc.intfType = %d\n",
          con->ogRsc.intfc.intfType);


   if ((con->callDtlInfo.cm.tckOgConCfmRcvd.pres != PRSNT_NODEF)&&
       (con->keepGCCCallInfo == FALSE))
   {
      con->callDtlInfo.cm.tckOgConCfmRcvd.pres = PRSNT_NODEF;
      (Void) SGetSysTime(&con->callDtlInfo.cm.tckOgConCfmRcvd.val);
      CCDP(AIN_DBG_LEVEL_1, "ccConE11S134: SETTING "
        "con->callDtlInfo.cm.tckOgConCfmRcvd.pres=%d ,val=%ld\n", 
        con->callDtlInfo.cm.tckOgConCfmRcvd.pres,con->callDtlInfo.cm.tckOgConCfmRcvd.val);
   }

   CC_STATE_CHANGE(con, CCS_LOCALLYANSWERED);
   if (con->callDtlInfo.cm.tckStartLongDur.pres != NOTPRSNT) 
   {
       con->callDtlInfo.cm.tckStartLongDur.pres = NOTPRSNT;
       CCDP(AIN_DBG_LEVEL_1, "ccConE11S134: Setting "
            "callDtlInfo.cm.tckStartLongDur.pres =%d\n", 
            con->callDtlInfo.cm.tckStartLongDur.pres);
   }

#ifdef ZC
   cmMemset((U8*) &upd, 0, sizeof(ZcUpdParam));
   upd.con                  = con;
   upd.con->ogSapCb         = NULLP;
   upd.con->ogIntfcCb       = NULLP;
   zcRunTimeUpd(ZC_CON_CB, CMPFTHA_CREATE_REQ, upd);
   zcUpdPeer();
#endif /* ZC */


   RETVALUE(ROK);

} /* End ccConE11S134 */

/*
 *
 *      Fun:   ccConE36S134 
 *
 *      Desc:  Connection state function 
 *             event - CCE_MID_CALL_CFM
 *             state - CCS_AWT_MGCTCFM_LLTR
 *
 *      Ret:   ROK     - successful,
 *             RFAILED - unsuccessful
 *
 *      Notes: Handle a Mid-Call confirm while awaiting MGCT Line Test 
 *             Loopback confirm.
 *
 *      File:  cc_bdy2_2.c
 *
 */
PUBLIC  S16 ccConE36S134
(
CcConCb *con,                     /* connection */ 
PTR     event,                    /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
   CcMidCallCfmEvnt	*ccMidCallCfmEvnt    = NULLP;
   S16			ret = ROK;

#ifdef ZC
   ZcUpdParam upd;
#endif /* ZC */


   CCDP(AIN_DBG_LEVEL_1, "ccConE36S134:%ld, %ld\n", con->icSuConnId,
                        con->ogSuConnId);
   ccChangeCallStateOfAssociatedCalls(&(con->icBcm), CCS_CANNOTPROCESSEVENTS);

   if ((con == NULLP) || (event == NULLP))
   {
      CCLOGERR("Error: null pointers con 0x%lx, event 0x%lx\n", (U32)con, event);
      RETVALUE(ROK);
   }

   ccMidCallCfmEvnt = (CcMidCallCfmEvnt *) event;

   if (ccMidCallCfmEvnt->casCctEvnt.midCallStatus.pres == PRSNT_NODEF)
   {
      if (ccMidCallCfmEvnt->casCctEvnt.midCallStatus.val == MC_FAILURE)
      {
          CCLOGERR("Error: MidCallCfm returned a failure 0x%lx\n", con->icSuConnId);
          ret = RFAILED;
      }
   }
   else
   {
      CCLOGERR("Error: MidCallStatus parameter not present %d, icSu 0x%lx\n",
               ccMidCallCfmEvnt->casCctEvnt.midCallStatus.pres, con->icSuConnId);
      ret = RFAILED;
   }

   if (ret == RFAILED) 
   {
      CCLOGERR( "Error: Cleaning up from MidCallCfm failure\n");
      ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
      RETVALUE(ROK);
   }

   /* Administer timers and state */
   ccStopConTmr(con, TMR_SETUP);
   ccStartConTmr(CC_TMR_TST_LN_DSC, con, (PTR)&ccCp.genCfg);
   /* Since the Test Call is a one ended call, need to fill in ogRsc which
    * will be copied into termRsc of CcCallDtlRcrd to make billing 
    * happy to generate a record.
    */
   con->ogRsc.intfc.intfType = CC_LOCAL_SWT_PROT_INTFC;
   CCDP(AIN_DBG_LEVEL_1, "ccConE36S134: SETTING"   
         "con->ogRcs.intfc.intfType = %d\n",
          con->ogRsc.intfc.intfType);

   if ((con->callDtlInfo.cm.tckOgConCfmRcvd.pres != PRSNT_NODEF)&&
       (con->keepGCCCallInfo == FALSE))
   {
      con->callDtlInfo.cm.tckOgConCfmRcvd.pres = PRSNT_NODEF;
      (Void) SGetSysTime(&con->callDtlInfo.cm.tckOgConCfmRcvd.val);
      CCDP(AIN_DBG_LEVEL_1, "ccConE36S134: SETTING "
        "con->callDtlInfo.cm.tckOgConCfmRcvd.pres=%d ,val=%ld\n", 
        con->callDtlInfo.cm.tckOgConCfmRcvd.pres,con->callDtlInfo.cm.tckOgConCfmRcvd.val);
   }
   if (con->callDtlInfo.cm.tckStartLongDur.pres != NOTPRSNT) 
   {
       con->callDtlInfo.cm.tckStartLongDur.pres = NOTPRSNT;
       CCDP(AIN_DBG_LEVEL_1, "ccConE36S134: Setting "
            "callDtlInfo.cm.tckStartLongDur.pres=%d\n", 
            con->callDtlInfo.cm.tckStartLongDur.pres);
   }

   CC_STATE_CHANGE(con, CCS_LOCALLYANSWERED);
#ifdef ZC
   cmMemset((U8*) &upd, 0, sizeof(ZcUpdParam));
   upd.con                  = con;
   upd.con->ogSapCb         = NULLP;
   upd.con->ogIntfcCb       = NULLP;
   zcRunTimeUpd(ZC_CON_CB, CMPFTHA_CREATE_REQ, upd);
   zcUpdPeer();
#endif /* ZC */

   RETVALUE(ROK);

} /* End ccConE36S134 */

/*
 *
 *      Fun:   ccConE85S32
 *
 *      Desc:  Connection state function
 *             event - CCE_TSTLNDSCNTMREXP
 *             state - CCS_LOCALLYANSWERED
 *                   - CCS_AWT_MGCTCFM_TSTLNTONE
 *      Ret:   ROK     - successful,
 *             RFAILED - unsuccessful
 *
 *      Notes: Handle a Test Line Disconnect Timer Expiry from either the 
 *             Locally Answered state or awaiting MGCT confirm test line tone.
 *
 *      File:  cc_bdy2_2.c
 *
 */
PUBLIC S16 ccConE85S32
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                   /* bcm */
PTR     event2                   /* event */
)
{
   S16			ret;
   Bool       mgiSent   = FALSE;
#ifdef ZC
  ZcUpdParam upd;
#endif /* ZC */


   TRC3(ccConE85S32)
   CCDP(AIN_DBG_LEVEL_1, "ccConE85S32: Received a Disconnect Timer Expiry for testLineType:%d"
                         " in State:%d\n", con->tstLnCb->testLineType, con->state);
   /* Bug 63620 - If we are in a CM failover condition and MG resources are out of service.
    * restart the disconnect timer(shorter version for failover).
    */ 
   if ((con->tstLnCb->failOverFlag == TRUE) && 
       (con->tstLnCb->failOverTmrCount < CC_MAX_TSTLN_FO_TMR_EXP) &&
       (( CcLiRmtGetInterfaceSts(&con->rmSapCb->pst, con->rmSapCb->spId, &con->icRsc) == RM_INTFC_ST_DISABLE) ||
       (RmMgStatus == RM_MG_OOS)))
   {
      ccStartCon100Tmr(CC_TMR_TST_LN_FAILOVER, con);
      CCDP(AIN_DBG_LEVEL_1, "ccConE85S32: Count=%d Wait 500 msec more to drop call due to CM"
         " failover \n",con->tstLnCb->failOverTmrCount);
      con->tstLnCb->failOverTmrCount++;
      RETVALUE(ROK);
   }
   ccStopConTmr(con, CC_TMR_CALLDTL);
   ccStartConTmr(TMR_RELEASE, con, (PTR)&ccCp.genCfg);
   ccRmvCon100Tmr(con, CC_TST_100_SLNC_TMR);
   ccRmvCon100Tmr(con, CC_TST_102_SLNC1_TMR);
   ccRmvCon100Tmr(con, CC_TST_102_SLNC2_TMR);
   ccRmvCon100Tmr(con, CC_TMR_TST_LN_FAILOVER);
   SPutSBuf(ccInit.region, ccInit.pool, con->tstLnCb, (Size) sizeof(CcTstLnCb_t));
#ifdef ZC
  upd.icSuConnId = con->icSuConnId;
  zcRunTimeUpd(ZC_RELCON_CB, CMPFTHA_UPD_REQ, upd);
  zcUpdPeer();
#endif
   if (CC_IS_CONTEXT_PRESENT(con))
   {
      CCDP(AIN_DBG_LEVEL_1, "ccConE85S32: Sending Substract \n");
      ccUpdateMgQForSubRsc((PTR) NULLP, con->mgBlock->mgCtx->suCtxId, con);
      con->relPend = ICRELONLY;
      CC_STATE_CHANGE(con, CCS_AWTSWTCFM_SENDONEREL);
      con->mgBlock->con = con;
      ret = ccSendMgiQueue(con->mgBlock->mgCtx->suCtxId, con->mgBlock);
      if (ret == ROK)
      {
         mgiSent = TRUE;
      }
      else
      {
        CCLOGERR("Error: ccSendMgiQueue failed:suCtxId = %ld, icSu:%ld\n",
                 con->mgBlock->mgCtx->suCtxId, con->icSuConnId);
        cleanUpMgQ(con);
        ccCleanUpMGCtx (con);
      }
   }
   if (!mgiSent)
   {
      CC_STATE_CHANGE(con, CCS_AWTRELCFM_IC);
      CCDP(AIN_DBG_LEVEL_1, "ccConE85S32: Sending DeallocateResource\n");
      ccDeallocateResource(con, CC_INCOMING);

      CCDP(AIN_DBG_LEVEL_1, "ccConE85S32: Sending Relreq\n");
      ccSendRelease(con, con->icSapCb, con->icSpConnId, con->icSuConnId,
                    con->icProtType, CCCALLCLR, NULL);
   }
  
   RETVALUE(ROK);
} /* End func ccConE85S32 */

/* 
 *      Fun:   ccConE85SBUF
 *
 *      Desc:
 *             event - CCE_TSTLNDSCNTMREXP
 *             state - CCS_AWT_MGCTCFM_TSTLNTONE
 */
PUBLIC  S16 ccConE85SBUF
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  CCDP(AIN_DBG_LEVEL_1, "%s:icSu:%ld, ogSu:%ld\n",
       __FUNCTION__,con->icSuConnId, con->ogSuConnId);

  ccBufferEventToQueue(con, bcm, event, CCE_TSTLNDSCNTMREXP, sizeof(U8), 0,
                       NULLP, 0);
  RETVALUE(ROK);
} /* End func ccConE85SBUF */

/*
 *
 *      Fun:   ccConE86S32
 *
 *      Desc:  Connection state function
 *             event - CCE_TSTLNCDTMREXP
 *             state - CCS_LOCALLYANSWERED
 *      Ret:   ROK     - successful,
 *             RFAILED - unsuccessful
 *
 *      Notes: Handle a Test Line Cadence timer expiry for either the 100 or 102
 *             test line.  Will determine which silence timer expired and send tone. 
 *
 *      File:  cc_bdy2_2.c
 *
 */
PUBLIC S16 ccConE86S32
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                   /* bcm */
PTR     event2                   /* event */
)
{
   U8 *con100TmrType = NULLP;
   TRC3(ccConE86S32)
   CCDP(AIN_DBG_LEVEL_1, "ccConE86S32: Received a Cadence Timer Expiry for testLineType:%d"
                         " in Locally Answered state\n",con->tstLnCb->testLineType);

   con100TmrType = (U8*) event;
   
   /* Need to look at the event (timer type) and send a tone */
   switch(*con100TmrType)
   {
      case CC_TST_100_SLNC_TMR:
         con->tstLnCb->currentSignal = CC_TST_100_TONE;
         /* Populate message for tone */
         ccBldAndSendTstLnSig(con, CC_TST_100_TONE, CCS_AWT_MGCTCFM_TSTLNTONE, MGCT_CMD_ADD);
         break;

      case CC_TST_102_SLNC1_TMR:
         con->tstLnCb->currentSignal = CC_TST_102_TONE;

         /* Populate message for tone */
         ccBldAndSendTstLnSig(con, CC_TST_102_TONE, CCS_AWT_MGCTCFM_TSTLNTONE, MGCT_CMD_ADD);
         break;

      case CC_TST_102_SLNC2_TMR:
         con->tstLnCb->currentSignal = CC_TST_102_TONE;

         /* Populate message for tone */
         ccBldAndSendTstLnSig(con, CC_TST_102_TONE, CCS_AWT_MGCTCFM_TSTLNTONE, MGCT_CMD_MODIFY);

         break;

      default:
         CCLOGERR( "ERROR: con100TmrType: %d is invalid. Not handling Cadence Timer Expiry\n",
                    *con100TmrType);
         RETVALUE(RFAILED);
         break;
   } /* End switch on test line */

   RETVALUE(ROK);
} /* End func ccConE86S32 */

/*
 *
 *      Fun:   ccConE11S135 
 *
 *      Desc:  Connection state function 
 *             event - CCE_MGCTTXNCFM
 *             state - CCS_AWT_MGCTCFM_TSTLNTONE
 *
 *      Ret:   ROK     - successful,
 *             RFAILED - unsuccessful
 *
 *      Notes: Handle a MGCT transmit confirm - start the appropriate 
 *             cadence timer and update the state.
 *
 *      File:  cc_bdy2_2.c
 *
 */
PUBLIC  S16 ccConE11S135
(
CcConCb *con,                     /* connection */ 
PTR     event,                    /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
   MgctTxnCfm		*txnCfm  = NULLP;
   S16			ret = RFAILED;


   CCDP(AIN_DBG_LEVEL_1, "ccConE11S135: testLineType:%d, currentSignal:%d,"
                         " icSuConnId:%ld,ogSuConnId %ld\n", 
                          con->tstLnCb->testLineType, con->tstLnCb->currentSignal,
                          con->icSuConnId, con->ogSuConnId);
   txnCfm = (MgctTxnCfm*) event;
   ret = ccHandleMgResponse(con, txnCfm);
   if (ret == RFAILED) /* CPDI_FIX */
   {
      CCLOGERR( "ERROR: Response Handling FAILED\n");
      cleanUpMgQ(con);
      ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
      RETVALUE(RFAILED);
   }

   CC_STATE_CHANGE(con, CCS_LOCALLYANSWERED);

   RETVALUE(ROK);

} /* End ccConE11S135 */

/*
 *
 *      Fun:   ccConE36S135 
 *
 *      Desc:  Connection state function 
 *             event - CCE_MID_CALL_CFM
 *             state - CCS_AWT_MGCTCFM_TSTLNTONE
 *
 *      Ret:   ROK     - successful,
 *             RFAILED - unsuccessful
 *
 *      Notes: Handle a Mid-call confirm - start the appropriate 
 *             cadence timer and update the state.
 *
 *      File:  cc_bdy2_2.c
 *
 */
PUBLIC  S16 ccConE36S135
(
CcConCb *con,                     /* connection */ 
PTR     event,                    /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
   CcMidCallCfmEvnt	*ccMidCallCfmEvnt    = NULLP;
   S16			ret = ROK;


   CCDP(AIN_DBG_LEVEL_1, "ccConE36S135: testLineType:%d, currentSignal:%d,"
                         " icSuConnId:%ld,ogSuConnId %ld\n", 
                          con->tstLnCb->testLineType, con->tstLnCb->currentSignal,
                          con->icSuConnId, con->ogSuConnId);

   ccChangeCallStateOfAssociatedCalls(&(con->icBcm), CCS_CANNOTPROCESSEVENTS);

   if ((con == NULLP) || (event == NULLP))
   {
      CCLOGERR("Error: null pointers con 0x%lx, event 0x%lx\n", (U32)con, event);
      RETVALUE(ROK);
   }

   ccMidCallCfmEvnt = (CcMidCallCfmEvnt *) event;

   if (ccMidCallCfmEvnt->casCctEvnt.midCallStatus.pres == PRSNT_NODEF)
   {
      if (ccMidCallCfmEvnt->casCctEvnt.midCallStatus.val == MC_FAILURE)
      {
         CCLOGERR("Error: MidCallCfm returned a failure 0x%lx\n", con->icSuConnId);
         ret = RFAILED;
      }
   }
   else
   {
      CCLOGERR("Error: MidCallStatus parameter not present %d, icSu 0x%lx\n",
               ccMidCallCfmEvnt->casCctEvnt.midCallStatus.pres, con->icSuConnId);
      ret = RFAILED;
   }

   if (ret == RFAILED) 
   {
      CCLOGERR( "Error: Cleaning up from MidCallCfm failure\n");
      ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
      RETVALUE(ROK);
   }
   /* Verify this is a test line call start the appropriate timer */
   switch(con->tstLnCb->testLineType)
   {
      case 102:
         /* Start the silence timer and update current signal. */
         CCDP(AIN_DBG_LEVEL_1, "ccConE36S135 - 102 Test Line call - Tone just finished"
              " starting silence timer and updating current signal.\n");
      
         con->tstLnCb->currentSignal = CC_TST_102_SILENCE;
         ccStartCon100Tmr(CC_TST_102_SLNC2_TMR, con);
         break;
      case 100:
         /* NO silence timer to start for a 100 Test Line Call after 
          * the tone completes. 
          */
         CCDP(AIN_DBG_LEVEL_1, "ccConE36S135 - 100 Test Line call - Tone just finished"
              " nothing to do (e.g. no timers to start).\n");
         con->tstLnCb->currentSignal = CC_TST_100_SILENCE;
         break;
      case 108:
         /* Fall through - should not get here for a 108 Test Line Call. */
      default:
         CCLOGERR( "ERROR: ccConE36S135 - testLineType: %d is invalid. Not handling Notify"
                   " Message\n", con->tstLnCb->testLineType);
         RETVALUE(RFAILED);
         break;
   } /* End switch on testLineType */

   CC_STATE_CHANGE(con, CCS_LOCALLYANSWERED);

   RETVALUE(ROK);
} /* End ccConE36S135 */

/* 
 *  
 *      Fun:   ccConE62S32
 *  
 *      Desc:  Connection state function
 *             event - CCE_NOTIFYFROMMGCT
 *             state - CCS_LOCALLYANSWERED
 * 
 *      Ret:   ROK     - successful,
 *             RFAILED - unsuccessful
 *
 *      Notes: Handle a Notify while in the LOCALLYANSWERED.  This is expected
 *             to be from a milliwatt tone completion for a 100 or 102 Test Line
 *             call.
 *   
 *      File:  cc_bdy2_2.c
 *   
 */
PUBLIC  S16 ccConE62S32
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
   MgctTxnInd        *mgctTxnInd    = NULLP;
   U16                megacoErr     = 0;
   MgctNotifyKind_e   kind          = MGCT_NOTIFY_KIND_UNKNOWN;
   S16                ret           = RFAILED;

   CCDP(AIN_DBG_LEVEL_1, "ccConE62S32 icSu = 0x%lx, ogSu = 0x%lx\n",
        con->icSuConnId, con->ogSuConnId);

   if ((con == NULLP) || (event == NULLP) || (bcm == NULLP))
   {
      CCLOGERR("Error: Null pointers con = 0x%lx, event = 0x%lx, bcm = 0x%lx\n",
               (U32)con, event, (U32)bcm);
      RETVALUE(RFAILED);
   }
   mgctTxnInd = (MgctTxnInd *) event;
   ret = ccHandleMgctNotify(con, (MgctTxnInd *) event);
   if (ret != ROK)
   {
      CCLOGERR("Error: ccHandleMgctNotify failed icSu = 0x%lx,"
               "suCtxId 0x%lx, contextId = 0x%lx\n", con->icSuConnId,
               (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0), con->mgBlock->mgCtx->contextId);
      ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
      RETVALUE(ROK);
   }
   mgctGetNotifyReqKind(mgctTxnInd, &megacoErr, &kind, NULLP);
   if (kind == MGCT_NOTIFY_KIND_SIGNAL_COMPLETION)
   {
      /* Verify this is a test line call start the appropriate timer */
      switch(con->tstLnCb->testLineType)
      {
         case 102:
            /* Start the silence timer and update current signal. */
            CCDP(AIN_DBG_LEVEL_1, "ccConE62S32 - 102 Test Line call - Tone just finished"
                 " starting silence timer and updating current signal.\n");
         
            con->tstLnCb->currentSignal = CC_TST_102_SILENCE;
            ccStartCon100Tmr(CC_TST_102_SLNC2_TMR, con);
            break;
         case 100:
            /* NO silence timer to start for a 100 Test Line Call after 
             * the tone completes. 
             */
            CCDP(AIN_DBG_LEVEL_1, "ccConE62S32 - 100 Test Line call - Tone just finished"
                 " nothing to do (e.g. no timers to start).\n");
            con->tstLnCb->currentSignal = CC_TST_100_SILENCE;
            break;
         case 108:
            /* Fall through - should not get here for a 108 Test Line Call. */
         default:
            CCLOGERR( "ERROR: testLineType: %d is invalid. Not handling Notify Message\n",
                       con->tstLnCb->testLineType);
            RETVALUE(RFAILED);
            break;
       } /* End switch on testLineType */
   }
   else
   {
      /* FID 14841.0 */
      HNDL_MGCT_NOT_FOR_ADID(kind, con, event, bcm);
      CCLOGERR("Error: MGCT should never come back with NotifyType = %d\n", kind);
      ABORT_DEBUG;
   }
   CC_STATE_CHANGE(con, CCS_LOCALLYANSWERED);
   RETVALUE(ROK);
} /* End ccConE62S32 */

/*
 *
 *      Fun:   ccConE03S13X
 *
 *      Desc:  Connection state function
 *             event - CCE_CNST_IND 
 *             state - 134-CCS_AWT_MGCTCFM_LLTR
 *		     - 135-CCS_AWT_MGCTCFM_TSTLNTONE
 *
 *      Ret:   ROK     - successful,
 *             RFAILED - unsuccessful
 *
 *      Notes: None.
 *
 *      File:  cc_bdy2.c
 *
 */
PUBLIC S16 ccConE03S13X
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  S16         ret        = RFAILED;
  Bool        mgiSent    = FALSE;
  ProtType    icProtType = 0;
#ifdef ZC
  ZcUpdParam  upd;
#endif /* ZC */

  CCDP(AIN_DBG_LEVEL_1, "ccConE03S13X: %ld\n", con->icSuConnId);

  if(con->direction == OUTTOINC)
  {
    CCLOGERR("Error: Invalid direction :%d, icSu:%ld\n", con->direction,
             con->icSuConnId);
    RETVALUE(ROK);
  }

#ifdef ZC
  cmMemset((U8*) &upd, 0, sizeof(ZcUpdParam));
#endif /* ZC */
  icProtType = CC_GET_PROTOCOL(con->icProtType);

  if (ccIsForcedOnHookRcvd(con->icProtType, con->ogProtType, con->icEvntType,
          con->direction))
  {
    CCDP(AIN_DBG_LEVEL_0, "IOM failover has occured:%ld\n", con->icSuConnId);
    ccUpdateCallDtlInfoRelOrignForOnHook(con->icProtType,con->ogProtType,con, 
                                          con->direction, con->icEvntType);
    ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
    RETVALUE(ROK);
  }
  else if (ccIsOnHookRcvd(con->icProtType, con->ogProtType, con->icEvntType,
          con->direction))
  {
    ccStopConTmr(con, CC_TMR_TST_LN_DSC);
    ccRmvCon100Tmr(con, CC_TST_100_SLNC_TMR);
    ccRmvCon100Tmr(con, CC_TST_102_SLNC1_TMR);
    ccRmvCon100Tmr(con, CC_TST_102_SLNC2_TMR);
    ccStopConTmr(con, CC_TMR_CALLDTL);
    ccStartConTmr(TMR_RELEASE, con, (PTR)&ccCp.genCfg);

#ifdef ZC
     upd.icSuConnId = con->icSuConnId;
     zcRunTimeUpd(ZC_RELCON_CB, CMPFTHA_UPD_REQ, upd);
     zcUpdPeer();
#endif
    ccUpdateCallDtlInfoRelOrignForOnHook(con->icProtType,con->ogProtType,con, 
                                          con->direction, con->icEvntType);
    if (con->mgBlock->mgCtx && con->mgBlock->mgCtx->suCtxId)
    {
       /* FID 1915  Cleanup for a Test Line call */
      if (con->tstLnCb != NULLP)
      {
        CCDP(AIN_DBG_LEVEL_1, "ccConE03S13X:Administer testLineType:%d\n",
                               con->tstLnCb->testLineType);
        SPutSBuf(ccInit.region, ccInit.pool, con->tstLnCb,
                 (Size) sizeof(CcTstLnCb_t));
      } /* End if Test line */
      ccUpdateMgQForSubRsc((PTR) NULLP, con->mgBlock->mgCtx->suCtxId, con);
      con->relPend = ICRELONLY;
      CC_STATE_CHANGE(con, CCS_AWTSWTCFM_SENDONEREL);
      con->mgBlock->con = con;
      ret = ccSendMgiQueue(con->mgBlock->mgCtx->suCtxId, con->mgBlock);
      if (ret == ROK)
      {
        mgiSent = TRUE;
      }
      else
      {
        CCLOGERR("Error: ccSendMgiQueue failed:suCtxId = %ld, icSu:%ld\n",
                 con->mgBlock->mgCtx->suCtxId, con->icSuConnId);
        cleanUpMgQ(con);
        ccCleanUpMGCtx (con);
      }
    }
    if (!mgiSent)
    {
      CC_STATE_CHANGE(con, CCS_AWTRELCFM_IC);
      ccSendRelease(con, con->icSapCb, con->icSpConnId, con->icSuConnId,
                    con->icProtType, CCCALLCLR, NULL);
    }
    RETVALUE(ROK);
  }
  else
  {
    CCDP(AIN_DBG_LEVEL_1, "ccConE03S13X(): unExpected evnt in state = %d,"
         "ProtocolType = %d, direction = %d, evnType =%d\n",
         con->state, con->icProtType, con->direction, con->icEvntType);
    ccBufferEventToQueue(con, bcm, event, CCE_CNSTIND, sizeof(CcCnStEvnt), 0, NULLP, 0);
    RETVALUE(ROK);
  }
} /* ccConE03S13X */

/* Bug 64141 + */
/*
 *      Fun:   ccConE03S56
 *
 *      Desc:  Connection state function
 *             event - Connection Status Indication
 *             state - CCS_AWTSWTCFM_SENDNOREL
 *      Ret:   ROK     - successful,
 *             RFAILED - unsuccessful
 *
 *      Notes: None.
 *
 *      File:  cc_bdy2_2.c
 *
 */
PUBLIC S16 ccConE03S56
(
CcConCb  *con,
PTR      event,
AinBCM_t *bcm,
PTR      event2
)
{

  if (ccIsForcedOnHookRcvd(con->icProtType, con->ogProtType, con->icEvntType,
                           con->direction))
  {
    ccUpdateCallDtlInfoRelOrignForOnHook(con->icProtType,con->ogProtType,con, con->direction, con->icEvntType);

    cleanUpMgQ(con);
    ccCleanUpMGCtx (con);
    ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
    RETVALUE(ROK);
  }

  if (ccIsFlashRcvd(con->icProtType, con->ogProtType, con->icEvntType,
                    con->direction))
  {
    CCDP(AIN_DBG_LEVEL_0, "Flash ignored while waiting for Rsp from MGI\n");
    RETVALUE(ROK);
  }

  if (ccIsOnHookRcvd(con->icProtType, con->ogProtType, con->icEvntType,
                     con->direction))
  {
    /* Change state and reset relPend (wait for MGI rsp to do any call clearing) */
    con->relPend = ICRELONLY;
    CC_STATE_CHANGE(con, CCS_AWTSWTCFM_SENDONEREL);

    ccUpdateCallDtlInfoRelOrignForOnHook(con->icProtType,con->ogProtType,con, con->direction, con->icEvntType);

    RETVALUE(ROK);
  }

  /* Bug 88027 */
  if (ccIsPrackRcvd(CC_GET_PROTOCOL(con->icProtType), CC_GET_PROTOCOL(con->ogProtType), 
                    con->icEvntType, con->direction))
  {
     /* Drop Prack, it may come after 18x, but the call will be released */
     RETVALUE(ROK);
                
  }
    
  /* Drop event */
  CCLOGERR("%s: ERROR: Protocol %d Event %d is unexpected\n", __FUNCTION__, con->icProtType, con->icEvntType);

  RETVALUE(ROK);
}
/* Bug 64141 - */

/* Bug 64647 */
/*
*      Fun:   ccConE62S58
*
*      Desc:  Connection state function
*             event - CCE_NOTIFYFROMMGCT
*             state - CCS_AWTMGCTCFMFORMODIFY1
*
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE62S58
(
CcConCb   *con,             /* connection */
PTR        event,           /* event */
AinBCM_t  *bcm,             /* bcm */
PTR        event2           /* event */
)
{
  /* FID 14841.0 */
  U16                megacoErr     = 0;
  MgctNotifyKind_e   kind          = MGCT_NOTIFY_KIND_UNKNOWN;

  /* FID 14841.0 */
  mgctGetNotifyReqKind((MgctTxnInd *) event, &megacoErr, &kind, NULLP);
  HNDL_MGCT_NOT_FOR_ADID(kind, con, event, bcm);

  CCLOGERR("Error: received NOTIFY in S58, release the call\n");
  ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
  RETVALUE(ROK);
}


/* FID 14234.0 + */
/*
 *
 *      Fun:   ccConE34S00
 *
 *      Desc:  Connection state function
 *             event - CCE_VIRTUALCALLSETUP(Virtual Call Setup From FIC)
 *             state - CCS_IDLE
 *
 *      Ret:   ROK     - successful,
 *             RFAILED - unsuccessful
 *
 *      File:  cc_bdy2_2.c
 *
 */
PUBLIC S16 ccConE34S00
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
    S16                ret                      = RFAILED;
    msgValEDPRequestEnum index                  = 0;
    VirtualCallSetup_t   *pVirtualCallSetup;
    SvcHeader_t          *pSvcHeader;
    SvcHeader_t          header;
    InfoCollected_t      infoCollected;

    CCDP(AIN_DBG_LEVEL_1, "ccConE34S00:\n");

    pSvcHeader = (SvcHeader_t *) event;
    pVirtualCallSetup = (VirtualCallSetup_t *)event2;

    if (pSvcHeader->lastTransactionFlag == TRUE)
    {
        CCLOGERR("Error: lastTransaction is TRUE for VirtualCallSetup");
        ccDeleteBcm(&(con->ogBcm));
        ccRelCon(con);
        RETVALUE(ROK);
    }

    /* Initialize the icBcm field in the CcConCb */
    ret = SGetSBuf(ccInit.region, ccInit.pool, (Data **) &(con->icBcm.cc), (Size) sizeof(AinCallConfig_t));
    if (ret != ROK)
    {
        ccGenAlarm(LCM_CATEGORY_RESOURCE, LCC_EVENT_MEMALOC_FAILED, LCC_CAUSE_MALLOC_FAIL, (U8 *) &ret);
        ccRelCon(con);
        RETVALUE(RFAILED);
    }
    cmMemset((U8 *) (con->icBcm.cc), 0, sizeof(AinCallConfig_t));

    if (ccInit.acnt == TRUE)
    {
        SwTimevalGet(&con->callDtlInfo.cm.timevalStart, SW_TIMEVAL_IcConIndRcvd, NULL);
        con->callDtlInfo.cm.tckIcConIndRcvd.pres = PRSNT_NODEF;
        (Void) SGetSysTime(&con->callDtlInfo.cm.tckIcConIndRcvd.val);
    }

    /* Extract Information into Call Control */
    con->icBcm.bcmType = BtOBcm;
    ccExtractSvcHeader(pSvcHeader, &(con->icBcm), MSG_Virtual_Call_Setup);

    /* Allocate a new CC transId for use between CC and FIC */
    con->icBcm.svcHeader.transId = ccGetNewtransId();
    CCDP(AIN_DBG_LEVEL_0," TransId= %ld\n", con->icBcm.svcHeader.transId);

    /* FillBCMInfo       */
    ret = ccFillBCMInfo(con, &(con->icBcm), con->icBcm.svcHeader.transId,
                        CiOriginatingSetup, PicONull, BtOBcm, 
                        DP_ORIGINATION_ATTEMPT);

    /* Update AinCallConfig */
    con->icBcm.cc->legId01 = & (con->icBcm);

    /* Insert in icBCM Hash table */
    ret = cmHashListInsert(&ccicBCMInsTbl, (PTR) &(con->icBcm), (U8 *)&(con->icBcm.svcHeader.transId), (U16)sizeof(UConnId));
    if (ret != ROK)
    {
        CCLOGERR(" ccicBCMInsTbl Insertion failed:%ld\n", con->icSuConnId);
        ccRelCon(con);
        RETVALUE(RFAILED);
    }

    /* Extract info from FIC */
    ccExtractVirtualCallSetupParam(pVirtualCallSetup, &(con->icBcm));

    if(ccCopyCdPty2ClAddr(con)!= ROK)
    {
        CCDP(AIN_DBG_LEVEL_0," Populating the Called Party Address failed\n");
        ccRelCon(con);
        RETVALUE(RFAILED);
    }

    /* Skip RM */
    con->icRscVal = FALSE;

    /* FID 16970.0 */
    if ( (pVirtualCallSetup->intfcId.pres == PRSNT_NODEF) && 
         (pVirtualCallSetup->intfcId.val != 0) )
    {
		if (con->ccbsIsdnCallInfo == NULL)
	    {
            ret = SGetSBuf(ccInit.region, ccInit.pool, (Data **) &(con->ccbsIsdnCallInfo), 
                       (Size) sizeof(CcbsIsdnCallInfo));

            if (ret != ROK)
            {
                ccGenAlarm(LCM_CATEGORY_RESOURCE, LCC_EVENT_MEMALOC_FAILED,
                           LCC_CAUSE_MALLOC_FAIL, (U8 *) &ret);

                ccRelCon(con);
                RETVALUE(RFAILED);
            }

			cmMemset((U8 *)con->ccbsIsdnCallInfo, 0, sizeof(CcbsIsdnCallInfo));
	    }

        CCDP(AIN_DBG_LEVEL_3,"entering %s \n", __FUNCTION__);

        con->ccbsIsdnCallInfo->ccbsIsdnEvent = CCBS_ISDN_EVT_SUB_IND;
        con->ccbsIsdnCallInfo->caller = pVirtualCallSetup->caller;

        con->ccbsIsdnCallInfo->dstCcsId = pVirtualCallSetup->ccsId;
        con->ccbsIsdnCallInfo->srcCcsId = ccCp.genCfg.ownCcsId; 
        con->ccbsIsdnCallInfo->ccbsOgIntfcType = CC_IN_INTFC; 
        con->ccbsIsdnCallInfo->ccbsOgIntfcId.pres = PRSNT_NODEF;
        con->ccbsIsdnCallInfo->ccbsOgIntfcId.val = pVirtualCallSetup->intfcId.val;
        if(con->icIntfcCb)
            con->ccbsIsdnCallInfo->ccbsIcIntfcType = con->icIntfcCb->intfc.intfType;
        else
        {
            CCDP(AIN_DBG_LEVEL_3,"%s: con->icIntfcCb NULLP.\n",__FUNCTION__);
            ccRelCon(con);
            RETVALUE(RFAILED);
        }
        con->ccbsIsdnCallInfo->ccbsIcTrnkGrpId.pres = PRSNT_NODEF; 
        con->ccbsIsdnCallInfo->ccbsIcTrnkGrpId.val = pVirtualCallSetup->usrId.u.trnkGrpId.val;

        CCDP(AIN_DBG_LEVEL_3,"%s: own ccsId :%d , egress ccsId: %d. ccbsIcIntfcType %ld.\n", __FUNCTION__, 
             con->ccbsIsdnCallInfo->srcCcsId,con->ccbsIsdnCallInfo->dstCcsId,con->ccbsIsdnCallInfo->ccbsIcIntfcType);
        CCDP(AIN_DBG_LEVEL_3,"%s: ccbsOgIntfcId %ld, ccbsIcTrnkGrpId %d.\n",__FUNCTION__,
             con->ccbsIsdnCallInfo->ccbsOgIntfcId.val,con->ccbsIsdnCallInfo->ccbsIcTrnkGrpId.val);
        
        if (pVirtualCallSetup->srvcRetention == CCBS_SERVICE_RETENT_YES)
        {
            CCBS_ISDN_SET_RETENTION_SUPPORT_MASK(con->ccbsIsdnCallInfo->ccbsIsdnFlag);
        }
        else if (pVirtualCallSetup->srvcRetention == CCBS_SERVICE_RETENT_NO)
        {
            CCBS_ISDN_RESET_RETENTION_SUPPORT_MASK(con->ccbsIsdnCallInfo->ccbsIsdnFlag);
        }
        CCDP(AIN_DBG_LEVEL_3,"%s: get retention mask: %x, caller: %s \n", 
              __FUNCTION__,  (int)(con->ccbsIsdnCallInfo->ccbsIsdnFlag),
             (con->ccbsIsdnCallInfo->caller.addrSig.val));
        
        cmInitTimers(con->ccbsIsdnCallInfo->timer, CCBS_ISDN_MAX_RUN_TMR);
        /*FID 16970.0 inter ccs */
        if(con->ccbsIsdnCallInfo->dstCcsId != con->ccbsIsdnCallInfo->srcCcsId)
        {
            CCDP(AIN_DBG_LEVEL_3,"%s: inter ccs call.\n",__FUNCTION__);
            con->ogBcm.interCcsCall = TRUE;
            ret = ccbsIngressCcsInitCall(con,pVirtualCallSetup);
            if(ret != ROK)
            {
                CCDP(AIN_DBG_LEVEL_3,"%s: ccbsIngressCcsInitCall failed.\n",__FUNCTION__);
                RETVALUE(RFAILED);
            }
            RETVALUE(ROK);
        }
        ccbsIsdnCallStateMachine(con, (PTR)pVirtualCallSetup);
        /* Peg count for RBWF virtual call */
        //ccCp.sts.totalCalls++;
        //ccCp.sts.totalCCBSVirtual++;
        CCDP(AIN_DBG_LEVEL_1,"%s: CCBS virtual call counter %ld\n",__FUNCTION__, ccCp.sts.totalCCBSVirtual);
    }
    else
    {
        /* Start the SETUP Timer on con */
        ccStartConTmr(TMR_SETUP, con, (PTR)&ccCp.genCfg);

        /* Peg count for RBWF virtual call */
        ccCp.sts.totalCalls++;

        /* Bug 84781 Mark virtual call in order to avoid involving MG */
        con->callDtlInfo.un.telicaCDR_extention.RBWFCallInd = LCC_CDR_RBWF_VIRTUAL_CALL;

        /* Send Info Collected message to FIC and continue normal call processing */
        /* Initializing the message buffers */
        cmMemset((U8 *)&header, 0, sizeof(SvcHeader_t));
        cmMemset((U8 *)&infoCollected, 0, sizeof(InfoCollected_t));

        /* Cleaning up the NEL */
        for (index = 0; index < EReqMax; index++)
        {
            con->icBcm.nel[index] = NEL_UNARMED;
        }

        /* Fill in parameters in the Service Header Structure */
        ccFillSvcHeader(&header, &(con->icBcm), MSG_Info_Collected);
        CCDP(AIN_DBG_LEVEL_1," Filled SvcHeader\n");

        /* Fill in parameters in the Info Collected Structure */
        ccFillInfoColltdParam(&infoCollected, &(con->icBcm));
        CCDP(AIN_DBG_LEVEL_1," Filled InfoCollected\n");

        CC_STATE_CHANGE(con, CCS_AWTRSPFORDPE3);

        CCDP(AIN_DBG_LEVEL_1,"%s: Sending InfoCollected\n",__FUNCTION__);
        CcUiFctInfoCollected(&infoCollected, &header);
    }

    RETVALUE(ROK);
}

/*
 *
 *      Fun:   ccConE87S37
 *
 *      Desc:  Connection state function
 *             event - CCE_SENDAPM
 *             state - CCS_AWTRSPFORDPE3
 *
 *      Ret:   ROK     - successful,
 *             RFAILED - unsuccessful
 *
 *      File:  cc_bdy2_2.c
 *
 */
PUBLIC S16 ccConE87S37
(
 CcConCb *con,                    /* connection */
 PTR     event,                   /* event */
 AinBCM_t *bcm,                   /* bcm */
 PTR     event2                   /* event */
 )
{
    SvcHeader_t *icSvcHeader = NULLP;
    SendApm_t   *pSendApm    = NULLP;

    CCDP(AIN_DBG_LEVEL_1, "%s icSu:%ld, ogSu:%ld, suCtxId:%ld"
         "icTransId:%ld, con->state= %d\n",__FUNCTION__,
         con->icSuConnId, con->ogSuConnId,
         (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0),
         con->icBcm.svcHeader.transId, con->state);

    icSvcHeader = (SvcHeader_t *) event;
    pSendApm = (SendApm_t *) event2;

    ccExtractSvcHeader(icSvcHeader, bcm, MSG_Send_Apm);
    ccExtractReqRepBCMEvnt(&pSendApm->reqRepBCMEvnt, bcm);

    if(pSendApm->dpnssCallType.pres)
    {
        if(pSendApm->dpnssCallType.val == LCC_CDR_RBWF_VIRTUAL_CALL)
        {
            con->callDtlInfo.un.telicaCDR_extention.RBWFCallInd = LCC_CDR_RBWF_VIRTUAL_CALL;
            ccCp.sts.totalRBWFVirtualCall++;
            CCDP(AIN_DBG_LEVEL_2,"RBWF virtual call.\n");
        }
        else if(pSendApm->dpnssCallType.val == LCC_CDR_RBWF_REAL_CALL)
        {
            con->callDtlInfo.un.telicaCDR_extention.RBWFCallInd = LCC_CDR_RBWF_REAL_CALL;
            ccCp.sts.totalRBWFRealCall++;
            CCDP(AIN_DBG_LEVEL_2,"RBWF real call.\n");
        }
    }

    ccSendApmReq(con, pSendApm);

    RETVALUE(ROK);
}

/*
 *
 *      Fun:   ccConE87S44
 *
 *      Desc:  Connection state function. After report ODisconnect, 
 *             GCC get SendAPM with release call indication.
 *             event - CCE_SENDAPM
 *             state - CCS_AWTRSPFORDPE9
 *
 *      Ret:   ROK     - successful,
 *             RFAILED - unsuccessful
 *
 *      File:  cc_bdy2_2.c
 *
 */
PUBLIC S16 ccConE87S44
(
 CcConCb *con,                    /* connection */
 PTR     event,                   /* event */
 AinBCM_t *bcm,                   /* bcm */
 PTR     event2                   /* event */
 )
{
    SendApm_t   *pSendApm = NULLP;
    S16         mgret     = RFAILED;

    CCDP(AIN_DBG_LEVEL_1, "%s icSu:%ld, ogSu:%ld, suCtxId:%ld"
            "icTransId:%ld, con->state= %d\n",__FUNCTION__,
            con->icSuConnId, con->ogSuConnId,
            (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0),
            con->icBcm.svcHeader.transId, con->state);

    pSendApm = (SendApm_t *) event2;

    ccSendApmReq(con, pSendApm);

    if(pSendApm->releaseCall)
    {
        /* Following logic will be more like E39S44 */
        ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
        ccStopConTmr(con, AIN_O_NO_ANS_TMR);
        ccStopConTmr(con, AIN_T_NO_ANS_TMR);
        ccStopConTmr(con, CC_TMR_AWAITANS);
        ccStopConTmr(con, TMR_SETUP);
        ccDeleteBufferedEvent(con, con->icSuConnId, CCE_ONOANSEXP);
        ccDeleteBufferedEvent(con, con->ogSuConnId, CCE_TNOANSEXP);

        con->tempHolderForReleaseCause = CCCALLCLR;

        ccStartConTmr(TMR_RELEASE, con, (PTR)&ccCp.genCfg);
        ccStopConTmr(con, CC_TMR_CALLDTL);

        if (CC_IS_CONTEXT_PRESENT(con))
        {
            ccQueueSubOnOGCleanUp(con);
            if (con->direction == INCTOOUT)
                con->relPend = INRELRSPANDOUTREL;
            else
                con->relPend = OUTRELRSPANDINREL;

            CC_STATE_CHANGE(con, CCS_AWTSWTCFM_SENDONEREL);
            con->mgBlock->con = con;
            mgret = ccSendMgiQueue(con->mgBlock->mgCtx->suCtxId, con->mgBlock);
            if (mgret == ROK)
                RETVALUE(ROK);
            else
            {
                CCLOGERR("Error: MGI failed:icSu:%ld, ogSu:%ld, suCtxId:%ld,"
                         " icTransId:%ld\n", con->icSuConnId, con->ogSuConnId,
                         con->mgBlock->mgCtx->suCtxId, con->icBcm.svcHeader.transId);
                ccHandleCleanUpConCb(con, TRUE, AM_CV_TMPFAIL);
                RETVALUE(ROK);
            }
        }

        if (con->noRspReqd == FALSE) 
        {
            ccSendReleaseRsp(con);
        }

        if (con->direction == INCTOOUT)
        {
            /* Deallocate the incoming resources */
#ifdef GUARD_TIMER
            ccGuardOrDeallocResource(con, CC_INCOMING);
#else
            ccDeallocateResource(con, CC_INCOMING);
#endif
            con->icRscVal = FALSE;

            CC_STATE_CHANGE(con, CCS_AWTRELCFM_OG);

            ccSendRelease(con, con->ogSapCb, con->ogSpConnId, con->ogSuConnId,
                    con->ogProtType, con->tempHolderForReleaseCause, NULL);
        }
        else if (con->direction == OUTTOINC)
        {
            /* Deallocate the outgoing resources */
#ifdef GUARD_TIMER
            ccGuardOrDeallocResource(con, CC_OUTGOING);
#else
            ccDeallocateResource(con, CC_OUTGOING);
#endif
            con->ogRscVal = FALSE;

            CC_STATE_CHANGE(con, CCS_AWTRELCFM_IC);

            ccSendRelease(con, con->icSapCb, con->icSpConnId, con->icSuConnId,
                          con->icProtType, con->tempHolderForReleaseCause, NULL);
        }
    }

    RETVALUE(ROK);
}

/*
 *
 *      Fun:   ccConE87S124
 *
 *      Desc:  Connection state function
 *             event - CCE_SENDAPM
 *             state - CCS_AWTRSPFORDPE7
 *
 *      Ret:   ROK     - successful,
 *             RFAILED - unsuccessful
 *
 *      File:  cc_bdy2_2.c
 *
 */
PUBLIC S16 ccConE87S124
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
    SendApm_t   *pSendApm = NULLP;

    CCDP(AIN_DBG_LEVEL_1, "%s icSu:%ld, ogSu:%ld, suCtxId:%ld"
         "icTransId:%ld, con->state= %d\n",__FUNCTION__,
         con->icSuConnId, con->ogSuConnId,
         (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0),
         con->icBcm.svcHeader.transId, con->state);

    pSendApm = (SendApm_t *) event2;

    ccExtractReqRepBCMEvnt(&pSendApm->reqRepBCMEvnt, bcm);

    CC_STATE_CHANGE(con, CCS_AWTANS);

    ccSendApmReq(con, pSendApm);

    RETVALUE(ROK);
}   

/*
 *
 *      Fun:   ccConE39S140
 *
 *      Desc:  Connection state function
 *             event - CCE_CONTINUE
 *             state - CCS_AWTRSPOAPM
 *
 *      Ret:   ROK     - successful,
 *             RFAILED - unsuccessful
 *
 *      File:  cc_bdy2_2.c
 *
 */
PUBLIC S16 ccConE39S140
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                   /* bcm */
PTR     event2                   /* event */
)
{
      Continue_t       *pContinue      = NULLP; 
      SendApm_t         sendApm;               
      S16               ret          = RFAILED;
      U8                mgiAction;
      U8                icMode;
      U8                ogMode;
      U8                icSignal;
      U8                ogSignal;

      CCDP(AIN_DBG_LEVEL_0, "%s icSu:%ld, ogSu:%ld, suCtxId:%ld"
           "icTransId:%ld, con->state= %d\n",__FUNCTION__,
           con->icSuConnId, con->ogSuConnId,
           (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0),
           con->icBcm.svcHeader.transId, con->state);

      pContinue = (Continue_t *) event2;

      if(pContinue == NULLP)
      {
          RETVALUE(ROK);
      }

      if( FCT_IS_CONTACT_SEND_APM(pContinue->action))
      {
          CCDP(AIN_DBG_LEVEL_1, "Populate one SENDAPM event and send out.\n");

          /* Initializing the SendApm message buffers */
          cmMemset((U8 *)&sendApm, 0, sizeof(SendApm_t));

          /* Populate APP param and direction from FIC to SendApm_t structure */
          cmMemcpy((U8 *)&sendApm.app, (U8 *)&pContinue->app, sizeof(SiAppTrans));
          sendApm.direction = pContinue->apmDirection;

          /* Send out CC_ET_APPTRAN to ISUP base on direction from service layer */
          ccSendApmReq(con, &sendApm);
      }

      if( FCT_IS_CONTACT_HOLD_ACM(pContinue->action))
      {
          CCDP(AIN_DBG_LEVEL_1, "ACM is alredy stored in EvntHold before.\n");
      }
      if(FCT_IS_CONTACT_INTW_ACM(pContinue->action))
      {
          CCDP(AIN_DBG_LEVEL_1, "Restore ACM in EvntHold and continue the call processing.\n");
          con->icEvntType = con->evntTypeHeld;
          con->evntType   = con->evntHeld;
          con->direction  = CC_GET_EVNTHOLD_DIR(con->directionHeld);

          con->lastState = con->state;
          CC_STATE_CHANGE(con, CCS_AWTANS);

          mgiAction = ccFindSwitchingAction(con, CCE_CNSTIND, con->icEvntType,
                                            con->direction, &con->icBcm, &con->ogBcm, &icMode, &ogMode,
                                            &icSignal, &ogSignal, (PTR)con->ccEvntHold, TRUE);

          CCDP(AIN_DBG_LEVEL_1, "%s: Action == %d, icMode == %d, icSignal == %d,"
                                " ogMode == %d, ogSignal == %d\n", __FUNCTION__,
                                mgiAction, icMode, icSignal, ogMode, ogSignal);

          if(mgiAction != CC_NO_MGI_ACTION)
          {
              ret=ccProcessOAlertingWithSwitching(con, (PTR)con->ccEvntHold);
              if (ret!=ROK)
                  RETVALUE(RFAILED);
          }
          else 
          {
              ret=ccProcessOAlertingNoSwitching(con,(PTR)con->ccEvntHold);
              if (ret!=ROK)
                  RETVALUE(RFAILED);
          }
      }
      else
      {
          CC_STATE_CHANGE(con, CCS_AWTANS);
      }
      RETVALUE(ROK);
}   

/*
 *
 *      Fun:   ccConE87S140
 *
 *      Desc:  Connection state function
 *             event - CCE_SENDAPM
 *             state - CCS_AWTRSPOAPM
 *
 *      Ret:   ROK     - successful,
 *             RFAILED - unsuccessful
 *
 *      File:  cc_bdy2_2.c
 *
 */
PUBLIC S16 ccConE87S140
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                   /* bcm */
PTR     event2                   /* event */
)
{
    SendApm_t   *pSendApm = NULLP;
    S16         mgret     = RFAILED;

    CCDP(AIN_DBG_LEVEL_1, "%s icSu:%ld, ogSu:%ld, suCtxId:%ld"
                          "icTransId:%ld, con->state= %d\n",__FUNCTION__,
                          con->icSuConnId, con->ogSuConnId,
                          (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0),
                          con->icBcm.svcHeader.transId, con->state);

    pSendApm = (SendApm_t *) event2;

    ccExtractReqRepBCMEvnt(&pSendApm->reqRepBCMEvnt, bcm);

    if(pSendApm->app.eh.pres == PRSNT_NODEF)
    {
        ccSendApmReq(con, pSendApm);

        if(pSendApm->dpnssCallType.pres)
        {
            if(pSendApm->dpnssCallType.val == LCC_CDR_RBWF_VIRTUAL_CALL)
            {
                con->callDtlInfo.un.telicaCDR_extention.RBWFCallInd = LCC_CDR_RBWF_VIRTUAL_CALL;
                ccCp.sts.totalRBWFVirtualCall++;
                CCDP(AIN_DBG_LEVEL_2,"RBWF virtual call.\n");
            }
            else if(pSendApm->dpnssCallType.val == LCC_CDR_RBWF_REAL_CALL)
            {
                con->callDtlInfo.un.telicaCDR_extention.RBWFCallInd = LCC_CDR_RBWF_REAL_CALL;
                ccCp.sts.totalRBWFRealCall++;
                CCDP(AIN_DBG_LEVEL_2,"RBWF real call.\n");
            }
        }
    }

    if(pSendApm->releaseCall)
    {
        CCDP(AIN_DBG_LEVEL_2,"Begin to release call after Send APM\n");
        ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));

        ccStopConTmr(con, TMR_SETUP);
        ccStopConTmr(con, CC_TMR_CALLDTL);
        if(pSendApm->app.eh.pres == PRSNT_NODEF)
        {
           con->tempHolderForReleaseCause = CCCALLCLR;
        }
        else
        {
           con->tempHolderForReleaseCause = CCPROTERR;
        }   
        ccStartConTmr(TMR_RELEASE, con, (PTR)&ccCp.genCfg);

        /* Currently CCS_AWTRSPOAPM is only switched from CCS_AWTRSPFORDPE3(37) and CCS_AWTANS(09) */
        if(con->lastState == CCS_AWTRSPFORDPE3)
        {
            CC_STATE_CHANGE(con, CCS_AWTRELCFM_IC);
            ccSendRelease(con, con->icSapCb, con->icSpConnId, con->icSuConnId,
                          con->icProtType, con->tempHolderForReleaseCause, NULL);
        }
        else if(con->lastState == CCS_AWTANS)
        {
            ccStopConTmr(con, CC_TMR_AWAITANS);
            ccStopConTmr(con, AIN_O_NO_ANS_TMR);
            ccStopConTmr(con, AIN_T_NO_ANS_TMR);
            ccStopConTmr(con, CC_TMR_AWAITANS);
            ccDeleteBufferedEvent(con, con->icSuConnId, CCE_ONOANSEXP);
            ccDeleteBufferedEvent(con, con->ogSuConnId, CCE_TNOANSEXP);

            if (CC_IS_CONTEXT_PRESENT(con))
            {
                ccUpdateMgQForSubRsc((PTR) NULLP, con->mgBlock->mgCtx->suCtxId, con);
                con->lastState = con->state;
                CC_STATE_CHANGE(con, CCS_AWTSWTCFM_SENDBOTHREL);
                con->mgBlock->con = con;
                mgret = ccSendMgiQueue(con->mgBlock->mgCtx->suCtxId, con->mgBlock);
                if (mgret == ROK)
                    RETVALUE(ROK);
                else
                {
                    CCLOGERR("Error: MGI failed:icSu:%ld, ogSu:%ld, suCtxId:%ld,"
                             " icTransId:%ld\n", con->icSuConnId, con->ogSuConnId,
                             con->mgBlock->mgCtx->suCtxId, con->icBcm.svcHeader.transId);
                    ccHandleCleanUpConCb(con, TRUE, AM_CV_TMPFAIL);
                    RETVALUE(ROK);
                }
            }

            if (con->noRspReqd == FALSE) ccSendReleaseRsp(con);

            /* Bug 77029 fix DPNSS type 2 error for SIP to ISUP real call */
            if ((con->icRscVal) && (con->ogRscVal))
            {
                CC_STATE_CHANGE(con, CCS_AWTRELCFM_BOTH);
                ccSendRelease(con, con->icSapCb, con->icSpConnId,
                              con->icSuConnId, con->icProtType, CCNORMUNSPEC, NULL);
                ccSendRelease(con, con->ogSapCb, con->ogSpConnId,
                              con->ogSuConnId, con->ogProtType, con->tempHolderForReleaseCause, NULL);
                RETVALUE(ROK);
            }

            if (pSendApm->direction == INCTOOUT)
            {
                CC_STATE_CHANGE(con, CCS_AWTRELCFM_OG);
    
                ccSendRelease(con, con->ogSapCb, con->ogSpConnId, con->ogSuConnId,
                        con->ogProtType, con->tempHolderForReleaseCause, NULL);
            }
            else if (pSendApm->direction == OUTTOINC)
            {
                CC_STATE_CHANGE(con, CCS_AWTRELCFM_IC);
    
                ccSendRelease(con, con->icSapCb, con->icSpConnId, con->icSuConnId,
                              con->icProtType, con->tempHolderForReleaseCause, NULL);
            }
        }
    }
    else
    {
        CC_STATE_CHANGE(con,con->lastState);
    }
    RETVALUE(ROK);
}

/*
 *
 *      Fun:   ccConE88S140
 *
 *      Desc:  Connection state function
 *             event - CCE_SENDACM
 *             state - CCS_AWTRSPOAPM
 *
 *      Ret:   ROK     - successful,
 *             RFAILED - unsuccessful
 *
 *      File:  cc_bdy2_2.c
 *
 */
PUBLIC S16 ccConE88S140
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                   /* bcm */
PTR     event2                   /* event */
)
{
    SendAcm_t    *pSendAcm  = NULLP;

    CCDP(AIN_DBG_LEVEL_1, "%s icSu:%ld, ogSu:%ld, suCtxId:%ld"
         "icTransId:%ld, con->state= %d\n",__FUNCTION__,
         con->icSuConnId, con->ogSuConnId,
         (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0),
         con->icBcm.svcHeader.transId, con->state);

    pSendAcm = (SendAcm_t *) event2;

    ccExtractReqRepBCMEvnt(&pSendAcm->reqRepBCMEvnt, bcm);

    CC_STATE_CHANGE(con, CCS_AWTRSPFORDPE3);

    ccSendAcmReq(con, pSendAcm);

    /* Peg RBWF real call from ISUP to SIP */
    con->callDtlInfo.un.telicaCDR_extention.RBWFCallInd = LCC_CDR_RBWF_REAL_CALL;
    ccCp.sts.totalRBWFRealCall++;

    RETVALUE(ROK);
}   

/*
 *
 *      Fun:   ccConE04S140
 *
 *      Desc:  Connection state function
 *             event - CCE_RELEASE
 *             state - CCS_AWTRSPOAPM
 *
 *      Ret:   ROK     - successful,
 *             RFAILED - unsuccessful
 *
 *      File:  cc_bdy2_2.c
 *
 */
PUBLIC S16 ccConE04S140
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                   /* bcm */
PTR     event2                   /* event */
)
{
    S16           ret = RFAILED;
    CcCause       cause;
    TknU16        sipCause;
    TknU16        sipStatus;

    CCDP(AIN_DBG_LEVEL_1, "%s icSu:%ld, ogSu:%ld, suCtxId:%ld"
         "icTransId:%ld, con->state:%d\n",__FUNCTION__,
         con->icSuConnId, con->ogSuConnId,
         (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0),
         con->icBcm.svcHeader.transId, con->state);

    /* Bug 78309 */
    if(con->lastState == CCS_AWTANS)
    {
        ccBufferEventToQueue(con, bcm, event, CCE_RELIND, sizeof(CcRelEvnt), 0, NULLP, 0);
        RETVALUE(ROK);
    }

    cmMemset ((U8 *)&cause, 0, sizeof(CcCause));

    ccGetRelCause(con, (con->direction == OUTTOINC?con->ogProtType:con->icProtType),
                                (CcRelEvnt *)event, &cause, &sipCause, &sipStatus);

    if ((cause.eh.pres == PRSNT_NODEF) && (cause.causeVal.pres == PRSNT_NODEF))
    {
        con->tempHolderForReleaseCause = cause.causeVal.val;
        con->ainInfo.diconnectCause    = cause.causeVal.val;
    }
    else
    {
        con->tempHolderForReleaseCause = CCTMPFAIL;
        con->ainInfo.diconnectCause    = CCTMPFAIL;
    }

    ret = ccProcessDisconnectEvent(con, event);
    if (ret == ROK)
    {
        CCDP(AIN_DBG_LEVEL_0,"ccProcessDisconnectEvent success,event detec.\n");
        RETVALUE(ROK);
    }

    ccStopConTmr(con, TMR_SETUP);

    if (con->noRspReqd == FALSE)
    {
        ccSendReleaseRsp(con);
    }

    /* Bug 84842 */
    ccDeallocateResource(con, CC_INCOMING);
    con->icRscVal = FALSE;

    CC_STATE_CHANGE(con, CCS_IDLE);
    ccRelCon(con);

    RETVALUE(ROK);
}   
/* FID 14234.0 - */

/*+ FID 15083.0 */
/*
 *
 *      Fun:   ccProcessFaxModemTextDetect
 *
 *      Desc:  Process Fax Modem Text Detect
 *
 *      Ret:   S16
 *
 *      File:  cc_bdy2_2.c
 *
 */
PUBLIC S16 ccProcessFaxModemTextDetect
(
PTR               event,
S16               ret
)
{
    AinBCM_t               *pBcm       = NULLP;
    AinBCM_t               *pCompBcm   = NULLP;
    Sdp                   **rmtSdp     = NULLP;
    CcConCb                *con        = (CcConCb *)event;

    if(ccIsIPResource(&(con->icBcm)) || ccIsIPResource(&(con->ogBcm)))
    {
      /* Bug 82493 interaction caused by FID 15083.0 */
      if(ccIsIPResource(&(con->icBcm)) &&
        (CC_GET_PROTOCOL(con->icProtType) != CC_EXT_BICC))
      {
         /* FID 15261.0 + */
         if (CC_IS_CALL_MULTI_MG(con))
         {
           pBcm = con->ogAdjBcm;
           pCompBcm = &con->ogBcm;
         }
         else
         {
           pBcm = &con->icBcm;
           pCompBcm = &con->ogBcm;
         }
         /* FID 15261.0 - */
      }
      else
      {
         /* FID 15261.0 + */
         if (CC_IS_CALL_MULTI_MG(con))
         {
           pBcm = con->icAdjBcm;
           pCompBcm = &con->icBcm;
         }
         else
         {
           pBcm = &con->ogBcm;
           pCompBcm = &con->icBcm;
         }
      }

      CCDP(AIN_DBG_LEVEL_1, "Passing Empty remote to MGI.\n");

      if (pBcm->bcmType == BtOBcm)
      {
        rmtSdp = &pBcm->con->icIpParam.rmtDesc;

        CC_SET_ZERO_BEARER_IP_PORT(&pBcm->con->icIpParam.locBearerIpPort);
      }
      else if (pBcm->bcmType == BtTBcm)
      {
        rmtSdp = &pBcm->con->ogIpParam.rmtDesc;

        CC_SET_ZERO_BEARER_IP_PORT(&pBcm->con->ogIpParam.locBearerIpPort);
      }
      else if (pBcm->bcmType == BtOAdjBcm)
      {
        if (con->ccmCb.ccn == CCN_5)
          rmtSdp = &pBcm->con->icLiAdjIpParam->locDesc;
        else
          rmtSdp = &pBcm->con->ogAdjIpParam->locDesc;

        CC_SET_ZERO_BEARER_IP_PORT(&pBcm->con->icAdjIpParam->locBearerIpPort);
      }
      else if(pBcm->bcmType == BtTAdjBcm)
      {
        if (con->ccmCb.ccn == CCN_5)
          rmtSdp = &pBcm->con->ogLiAdjIpParam->locDesc;
        else
          rmtSdp = &pBcm->con->icAdjIpParam->locDesc;

        CC_SET_ZERO_BEARER_IP_PORT(&pBcm->con->ogAdjIpParam->locBearerIpPort);
      }
      else if (pBcm->bcmType == BtOLiAdjBcm)
      {
        rmtSdp = &pBcm->con->icAdjIpParam->locDesc;

        CC_SET_ZERO_BEARER_IP_PORT(&pBcm->con->icLiAdjIpParam->locBearerIpPort);
      }
      else /* BtTLiAdjBcm */
      {
        rmtSdp = &pBcm->con->ogAdjIpParam->locDesc;

        CC_SET_ZERO_BEARER_IP_PORT(&pBcm->con->ogLiAdjIpParam->locBearerIpPort);
      }

      if(*rmtSdp)
        cmFreeSdp(ccInit.region, ccInit.pool, rmtSdp);

      cmCreateSdp(ccInit.region, ccInit.pool, 0, 0, FALSE, 0, FALSE, 0, 0, 0,
                  pBcm->con, rmtSdp, 
                  AF_INET); /* FID 14341.0 */
      /* FID 14341.0 the above used IPV4, makes no difference, since it will be set to NOTPRSNT */
      SDP_SET_NOTPRSNT(*rmtSdp);
      (*rmtSdp)->eh.pres = PRSNT_NODEF;

      /* Initiate a switch to T.38 */
      ccCreateOnSwitchBearerWithModify(pBcm, pCompBcm, ccHandleXferRspFailure,
                                         (PTR)con, CCS_ANSWERED, FALSE);
    }
    RETVALUE(ROK);
}
/*- FID 15083.0 */
/* FID 15045.1 + */
/*
 *
 *      Fun:   ccConE90S10
 *
 *      Desc:  Connection state function
 *             event - CCE_SENDMSG
 *             state - CCS_ANSWERED
 *
 *      Ret:   ROK     - successful,
 *             RFAILED - unsuccessful
 *
 *      Notes: This function is used for FIC to send a USR message to the SRP.
 *
 *      File:  cc_bdy2_2.c
 *
 */
PUBLIC S16 ccConE90S10
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                   /* bcm */
PTR     event2                   /* event */
)
{
    SendMsg_t    *pSendMsg  = NULLP;
    ProtType     sendMsgProtType=0;
    S16       ret        = RFAILED;

    CCDP(AIN_DBG_LEVEL_1, "%s icSu:%ld, ogSu:%ld, suCtxId:%ld"
         "icTransId:%ld, con->state= %d\n",__FUNCTION__,
         con->icSuConnId, con->ogSuConnId,(con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0),
         con->icBcm.svcHeader.transId, con->state);

    if (event2 == NULLP)
    {
        CCLOGERR("%s: Critical ERROR: Null pointers: event2=%ld\n", __FUNCTION__,event2);
        ABORT_DEBUG;
        ccHandleCleanUpConCb(con,TRUE,CCTMPFAIL);
        RETVALUE(ret);
    }

    pSendMsg = (SendMsg_t *) event2;

    if (pSendMsg->pres != PRSNT_NODEF)
    { 
        CCLOGERR("%s: Critical ERROR: SendMsg->pres not present: SendMsg->pres=%d\n", __FUNCTION__,pSendMsg->pres);
        ABORT_DEBUG;
        RETVALUE(ret);
    }
    sendMsgProtType = CC_GET_PROTOCOL(pSendMsg->protType);

    switch(sendMsgProtType)
    {
        case CC_SI:
           CCDP(AIN_DBG_LEVEL_1,"%s: sendMsgProtType=%d\n", __FUNCTION__,sendMsgProtType);
           if (pSendMsg->msgType == CC_ET_U2U)
           {
              if (CC_IS_SSP_RELAY_MODE(con))
              {
                  ret=ccSendUsrReq(con, pSendMsg);
              }
              else
              {
                  CCDP(AIN_DBG_LEVEL_1,"%s: relayMode disabled, not sending USR to SRP\n", __FUNCTION__);
                  /* Catch an inconsistent state */
                  ABORT_DEBUG;
              }
           }
           else
           {
              CCDP(AIN_DBG_LEVEL_1,"%s: Unsupported message type, pSendMsg->msgType=0x%x\n", __FUNCTION__,pSendMsg->msgType);
           }
        break;
        default:
           CCDP(AIN_DBG_LEVEL_1,"%s: Invalid protocol type = %d\n",__FUNCTION__,sendMsgProtType);
        break;
    }
    RETVALUE(ret);
}   

/*
 *
 *      Fun:   ccConE90SBUF
 *
 *      Desc:  Connection state function
 *             event - CCE_SENDMSG
 *             state - List States 
 *
 *      Ret:   ROK     - successful,
 *             RFAILED - unsuccessful
 *
 *      File:  cc_bdy2_2.c
 *
 */
PUBLIC S16 ccConE90SBUF
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                   /* bcm */
PTR     event2                   /* event */
)
{
    S16       ret        = ROK;

    CCDP(AIN_DBG_LEVEL_1, "%s icSu:%ld, ogSu:%ld, suCtxId:%ld"
         "icTransId:%ld, con->state= %d\n",__FUNCTION__,
         con->icSuConnId, con->ogSuConnId,(con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0),
         con->icBcm.svcHeader.transId, con->state);

    if (event2 == NULLP)
    {
        CCLOGERR("%s: Critical ERROR: Null pointers: event2=%ld\n", __FUNCTION__,event2);
        ABORT_DEBUG;
        ccHandleCleanUpConCb(con,TRUE,CCTMPFAIL);
        RETVALUE(ret);
    }

    ccBufferEventToQueue(con, bcm, event, CCE_SENDMSG, sizeof(SvcHeader_t),
                             (bcm->bcmType == BtOBcm) ? con->icSuConnId:con->ogSuConnId,
                                                    event2, sizeof(SendMsg_t));
    RETVALUE(ret);
}   

/*
 *
 *      Fun:   ccConE91S89
 *
 *      Desc:  Connection state function
 *             event - CCE_CANCEL
 *             state - CCS_AWTMGCTNOTIFY0
 *
 *      Ret:   ROK     - successful,
 *             RFAILED - unsuccessful
 *
 *      Notes: This function handles the request from FIC to cancel an 
 *             in progress announcement 
 *
 *      File:  cc_bdy2_2.c
 *
 */
PUBLIC S16 ccConE91S89
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                   /* bcm */
PTR     event2                   /* event */
)
{
    Cancel_t    *pCancel         = NULLP;
    MgctEventsDescriptor         evntDesc;
    MgctSignalsDescriptor        signalDesc;
    AinBCM_t          *pBcm            = NULLP;
    SvcHeader_t       header;
    ResourceClear_t   resourceClear;

    S16         ret              = ROK;

    CCDP(AIN_DBG_LEVEL_1, "%s icSu:%ld, ogSu:%ld, suCtxId:%ld"
         "icTransId:%ld, con->state= %d\n",__FUNCTION__,
         con->icSuConnId, con->ogSuConnId,(con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0),
         con->icBcm.svcHeader.transId, con->state);

    if (event2 == NULLP)
    {
        CCLOGERR("%s: Critical ERROR: Null pointers: event2=%ld\n", __FUNCTION__,event2);
        ABORT_DEBUG;
        ccHandleCleanUpConCb(con,TRUE,CCTMPFAIL);
        RETVALUE(ret);
    }

    pCancel = (Cancel_t *) event2;
    ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));

    if (CC_IS_CONTEXT_PRESENT(con))
    {
       if (con->anncBlock.anncType != CC_ANNC_DIGIT)
       {
         /*Always on icBcm*/
         pBcm = &(con->icBcm);

         /* Initializing the message buffers */
         cmMemset((U8 *)&header, 0, sizeof(SvcHeader_t));
         cmMemset((U8 *)&resourceClear, 0, sizeof(ResourceClear_t));
     
         /* Fill ClearCause with ClcTaskRefused */
         con->ainInfo.ClearCause = ClcTaskRefused;
     
         /* Fill in parameters in the Service Header Structure */
         ccFillSvcHeader(&header, pBcm, MSG_Resource_Clear);
         CCDP(AIN_DBG_LEVEL_0," Filled SvcHeader\n");

         /* Fill in parameters in the Resource Clear structure */
         ccFillResourceClearParam(&resourceClear, pBcm);
         CCDP(AIN_DBG_LEVEL_0," Filled ResourceClear\n");

         /* Send the ResourceClear message to the FIC */
         CCDP(AIN_DBG_LEVEL_0,"Sending ResourceClear, TransID=%ld\n",
                header.spTransId);
         CcUiFctResourceClear(&resourceClear, &header);
         RETVALUE(ROK);
      }
      else
      {
         cmMemset((U8 *)&evntDesc, 0, sizeof(MgctEventsDescriptor));
         cmMemset((U8 *)&signalDesc, 0, sizeof(MgctSignalsDescriptor));

         evntDesc.pres = PRSNT_NODEF;
         signalDesc.pres = PRSNT_NODEF;

         ccUpdateMgQForAddOrModifyRsc(MGCT_MODE_SENDRECV, MGCT_PACKAGE_ID_MAX,
                                      MGCT_ITEM_ID_CG_MAX,
                                      MGCT_CMD_MODIFY, &(con->icBcm),
                                      &evntDesc, NULLP, &signalDesc, NULLP, NULLP,
                                      con->mgBlock->mgCtx->suCtxId,
                                      con->mgBlock->mgCtx->contextId, con);
         CC_STATE_CHANGE(con, CCS_AWTMGCTCFMFORMODIFY2);
         con->mgBlock->con = con;
         ccChangeCallStateOfAssociatedCalls(&(con->icBcm), CCS_CANNOTPROCESSEVENTS);
         ret = ccSendMgiQueue(con->mgBlock->mgCtx->suCtxId, con->mgBlock);
         if (ret == ROK)
         {
           /* Set flag that cancel annoucement is in progress */
           CC_SET_CALL_FLAG(con->flag, CC_FLAG_CANCEL);
           RETVALUE(ROK);
         }
         else
         {
           CCLOGERR("Error: During switching icSu = 0x%lx\n", con->icSuConnId);
           ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
         }
       }
     }
     else
     {
       CCLOGERR("Error: No contextId for icSu = 0x%lx\n", con->icSuConnId);
     }
     cleanUpMgQ(con);
     ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
     RETVALUE(ret);
}   

/*
 *
 *      Fun:   ccConE91S90
 *
 *      Desc:  Connection state function
 *             event - CCE_CANCEL
 *             state - CCS_AWTMGCTCFMFORMODIFY2
 *
 *      Ret:   ROK     - successful,
 *             RFAILED - unsuccessful
 *
 *      Notes: This function handles the request from FIC to cancel an 
 *             in progress announcement 
 *             The annoucement is in the final stages of complete so just  
 *             note a cancel in progress so that FIC is notified with the 
 *             expected cause value.
 *
 *      File:  cc_bdy2_2.c
 *
 */
PUBLIC S16 ccConE91S90
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                   /* bcm */
PTR     event2                   /* event */
)
{
    Cancel_t          *pCancel         = NULLP;
    S16               ret              = ROK;
    AinBCM_t          *pBcm            = NULLP;
    SvcHeader_t       header;
    ResourceClear_t   resourceClear;

    CCDP(AIN_DBG_LEVEL_1, "%s icSu:%ld, ogSu:%ld, suCtxId:%ld"
         "icTransId:%ld, con->state= %d\n",__FUNCTION__,
         con->icSuConnId, con->ogSuConnId,(con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0),
         con->icBcm.svcHeader.transId, con->state);

    if (event2 == NULLP)
    {
        CCLOGERR("%s: Critical ERROR: Null pointers: event2=%ld\n", __FUNCTION__,event2);
        ABORT_DEBUG;
        ccHandleCleanUpConCb(con,TRUE,CCTMPFAIL);
        RETVALUE(ret);
    }

    pCancel = (Cancel_t *) event2;

    if (CC_IS_CONTEXT_PRESENT(con))
    {
         if (con->anncBlock.anncType == CC_ANNC_DIGIT)
         {
            /* Set Cancel Flag, so that appropiate 
             * cause value is sent back to FIC 
             */
            CC_SET_CALL_FLAG(con->flag, CC_FLAG_CANCEL);
         }
         else
         {
            /*Always on icBcm*/
            pBcm = &(con->icBcm);

            /* Initializing the message buffers */
            cmMemset((U8 *)&header, 0, sizeof(SvcHeader_t));
            cmMemset((U8 *)&resourceClear, 0, sizeof(ResourceClear_t));
        
            /* Fill ClearCause with ClcTaskRefused */
            con->ainInfo.ClearCause = ClcTaskRefused;
        
            /* Fill in parameters in the Service Header Structure */
            ccFillSvcHeader(&header, pBcm, MSG_Resource_Clear);
            CCDP(AIN_DBG_LEVEL_0," Filled SvcHeader\n");

            /* Fill in parameters in the Resource Clear structure */
            ccFillResourceClearParam(&resourceClear, pBcm);
            CCDP(AIN_DBG_LEVEL_0," Filled ResourceClear\n");

            /* Send the ResourceClear message to the FIC */
            CCDP(AIN_DBG_LEVEL_0,"Sending ResourceClear, TransID=%ld\n",
                   header.spTransId);
            CcUiFctResourceClear(&resourceClear, &header);
         }
         RETVALUE(ROK);
    }
    else
    {
       CCLOGERR("Error: No contextId for icSu = 0x%lx\n", con->icSuConnId);
    }
    cleanUpMgQ(con);
    ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
    RETVALUE(ret);
}   

/*
 *
 *      Fun:   ccConE91SBUF
 *
 *      Desc:  Connection state function
 *             event - CCE_CANCEL
 *             state - List States 
 *
 *      Ret:   ROK     - successful,
 *             RFAILED - unsuccessful
 *
 *      Notes: Handle Buffering CANCEL 
 *
 *      File:  cc_bdy2_2.c
 *
 */
PUBLIC S16 ccConE91SBUF
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                   /* bcm */
PTR     event2                   /* event */
)
{
    S16       ret        = ROK;

    CCDP(AIN_DBG_LEVEL_1, "%s icSu:%ld, ogSu:%ld, suCtxId:%ld"
         "icTransId:%ld, con->state= %d\n",__FUNCTION__,
         con->icSuConnId, con->ogSuConnId,(con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0),
         con->icBcm.svcHeader.transId, con->state);

    if (event2 == NULLP)
    {
        CCLOGERR("%s: Critical ERROR: Null pointers: event2=%ld\n", __FUNCTION__,event2);
        ABORT_DEBUG;
        ccHandleCleanUpConCb(con,TRUE,CCTMPFAIL);
        RETVALUE(ret);
    }

    ccBufferEventToQueue(con, bcm, event, CCE_CANCEL, sizeof(SvcHeader_t),
                             (bcm->bcmType == BtOBcm) ? con->icSuConnId:con->ogSuConnId,
                                                    event2, sizeof(Cancel_t));
    RETVALUE(ret);
}   

/*
 *
 *      Fun:   ccConE91SXX
 *
 *      Desc:  Connection state function
 *             event - CCE_CANCEL
 *             state - List States 
 *
 *      Ret:   ROK     - successful,
 *             RFAILED - unsuccessful
 *
 *      Notes: Handle responding to FIC, where CANCEL requested in invalid state
 *
 *      File:  cc_bdy2_2.c
 *
 */
PUBLIC S16 ccConE91SXX
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                   /* bcm */
PTR     event2                   /* event */
)
{
    S16       ret        = ROK;
    AinBCM_t          *pBcm          = NULLP;
    SvcHeader_t       header;
    ResourceClear_t   resourceClear;

    CCDP(AIN_DBG_LEVEL_1, "%s icSu:%ld, ogSu:%ld, suCtxId:%ld"
         "icTransId:%ld, con->state= %d\n",__FUNCTION__,
         con->icSuConnId, con->ogSuConnId,(con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0),
         con->icBcm.svcHeader.transId, con->state);

    if (event2 == NULLP)
    {
        CCLOGERR("%s: Critical ERROR: Null pointers: event2=%ld\n", __FUNCTION__,event2);
        ABORT_DEBUG;
        ccHandleCleanUpConCb(con,TRUE,CCTMPFAIL);
        RETVALUE(ret);
    }

    /*Always on icBcm*/
    pBcm = &(con->icBcm);

    /* Initializing the message buffers */
    cmMemset((U8 *)&header, 0, sizeof(SvcHeader_t));
    cmMemset((U8 *)&resourceClear, 0, sizeof(ResourceClear_t));

    /* Fill ClearCause with ClcTemporaryFailure */
    con->ainInfo.ClearCause = ClcTemporaryFailure;

    /* Fill in parameters in the Service Header Structure */
    ccFillSvcHeader(&header, pBcm, MSG_Resource_Clear);
    CCDP(AIN_DBG_LEVEL_0," Filled SvcHeader\n");

    /* Fill in parameters in the Resource Clear structure */
    ccFillResourceClearParam(&resourceClear, pBcm);
    CCDP(AIN_DBG_LEVEL_0," Filled ResourceClear\n");

    /* Send the ResourceClear message to the FIC */
    CCDP(AIN_DBG_LEVEL_0,"Sending ResourceClear, TransID=%ld\n",
           header.spTransId);
    CcUiFctResourceClear(&resourceClear, &header);
    RETVALUE(ret);
}   

/*
 *
 *      Fun:   ccProcessOUui
 *
 *      Desc:  This is only detected as a Notification event.
 *             This function checks if OUui is armed in the
 *             NEL of the BCM. if yes, process and send message to FIC.
 *
 *      Ret:  ROK ----  Successful 
 *            RFAILED --- Failure
 *
 *      File: cc_bdy2.c
 *
 */
PUBLIC S16 ccProcessOUui
(
AinBCM_t          *pBcm
)
{
  CcConCb           *con            = NULLP;
  SvcHeader_t        header;
  RcvMsg_t           rcvmsg;

  if (pBcm == NULLP)
  {
    CCLOGERR("Error: pBcm is NULL\n");
    ABORT_DEBUG;
    RETVALUE(RFAILED);
  }
  CCDP(AIN_DBG_LEVEL_1, "%s:%ld,%ld\n",__FUNCTION__,
       pBcm->svcHeader.transId, pBcm->con->icSuConnId );
  if (pBcm->bcmType != BtOBcm)
  {
    CCLOGERR("Error: Invalid bcmType:%d,%ld\n", pBcm->bcmType,
              pBcm->con->icSuConnId );
    ABORT_DEBUG;
    RETVALUE(RFAILED);
  }
  con = pBcm->con;

  if (pBcm->nel[EReqOUui] == NEL_NOTIFY)
  {
    if (pBcm->svcHeader.transId != 0)
    {
      CCDP(AIN_DBG_LEVEL_0," TransId for OBCM :%ld\n", pBcm->svcHeader.transId );

      /* Initializing the message buffers */
      cmMemset((U8 *)&header, 0, sizeof(SvcHeader_t));
      cmMemset((U8 *)&rcvmsg, 0, sizeof(RcvMsg_t));

      /* Fill in parameters in the Service Header Structure */
      ccFillSvcHeader(&header, pBcm, MSG_RcvMsg);
      CCDP(AIN_DBG_LEVEL_0," Filled SvcHeader\n");

      /* Fill in User 2 User data from USR in the rcvmsg structure */
      ccFillRcvMsg(&rcvmsg, pBcm, FALSE);

      CCDP(AIN_DBG_LEVEL_0," Filled User 2 user data, TransID= %ld\n", header.spTransId);

      /* Send the RcvMsg Notification Event with U2U data to the FIC */
      CcUiFctRcvMsgInd(&rcvmsg,&header);

      CCDP(AIN_DBG_LEVEL_0,"%s: Sent CcUiFctRcvMsgInd(), with U2U Data\n", __FUNCTION__);
      RETVALUE(ROK);
    }
    else
    {
      CCLOGERR("Not able to send event: %ld, %ld\n",
                                   con->icSuConnId, pBcm->svcHeader.transId);
      ABORT_DEBUG;
      RETVALUE(RFAILED);
    }
  }
  else
  {
    CCDP(AIN_DBG_LEVEL_0,"EReqOUui event is NOT ARMED: %ld\n",
                          con->icSuConnId);
    RETVALUE(ROK);
  }
}
/* FID 15045.1 - */
/* FID15319.0 */
/*
 *
 *      Fun:   ccConE01S148
 *
 *      Desc:  Connection status indication function
 *             event - CCE_CONCFM
 *             state - CCS_BICC_AWT_MGCT_CFMNOT
 *       
 *      Ret:   ROK     - successful,
 *             RFAILED - unsuccessful
 *
 *      Notes: None. 
 *
 *      File:  cc_bdy2_2.c
 *
 */
PUBLIC S16 ccConE01S148
(   
 CcConCb *con,                    /* connection */
 PTR     event,                   /* event */
 AinBCM_t *bcm,                    /* bcm */
 PTR     event2                    /* event */
 )
{
   CCDP(AIN_DBG_LEVEL_0, "%s: evntType=%d, icEvntType=%d\n", 
         __FUNCTION__, con->evntType, con->icEvntType)

   /* ConCfm is received before bearer setup done */
   if((CC_GET_PROTOCOL(con->ogProtType) == CC_EXT_BICC) && (con->ogExtBiccCb) &&
      CC_IS_EXTBICC_BEARERSTATE_LESSTHAN(con,CC_OUTGOING,CC_BICC_STATE_SENT_CONNECTED))
   {
      /* release call with cause 111 */
      ccSendReleasesOnBothSidesWithSwitching(con, CCPROTERR);
   }
   RETVALUE(ROK);
} /* End of ccConE03S147 */

/*
 *
 *      Fun:   ccConE03S147
 *
 *      Desc:  Connection status indication function
 *             event - CCE_CNST_IND 
 *             state - CCS_BICC_AWTICBEARER 
 *       
 *      Ret:   ROK     - successful,
 *             RFAILED - unsuccessful
 *
 *      Notes: None. 
 *
 *      File:  cc_bdy2_2.c
 *
 */
PUBLIC S16 ccConE03S147
(   
 CcConCb *con,                    /* connection */
 PTR     event,                   /* event */
 AinBCM_t *bcm,                    /* bcm */
 PTR     event2                    /* event */
 )
{
    BiccApmUserInfo apmUserInfo;
    BiccApmEncData  *appInfo = NULLP;
    SiAppTrans      *appTrans  =  NULLP;
    CcCnStEvnt      *ccCnStEvnt = (CcCnStEvnt *)event;
    CcCnStEvnt      biccBearerDoneCnst; /* Bug 82786 */
    S16             ret = ROK;
    U8              cause = CCPROTERR;

    CCDP(AIN_DBG_LEVEL_0, "%s: evntType=%d, icEvntType=%d\n",
                 __FUNCTION__, con->evntType, con->icEvntType);

    /* FID 15319.2 */
    if((con->icEvntType == CCSIP_ET_INFO_OVERLAP) && (con->direction == INCTOOUT))
    {
       RETVALUE(ROK);
    }

    cmMemset((U8 *)&apmUserInfo, 0, sizeof(BiccApmUserInfo));
    appTrans = &ccCnStEvnt->m.siCnStEvnt.appTransParam;

    if ((CC_GET_PROTOCOL(con->icProtType) == CC_EXT_BICC) &&
          (con->direction == INCTOOUT) && 
          (con->icEvntType == CC_ET_APPTRAN) &&
          (appTrans->eh.pres == PRSNT_NODEF))
    {
        ret = ccDecodeAndCheckAppInfo(&apmUserInfo, appTrans, &cause);
        /* Bug 89925 */
        if((ret == ROK) && (CC_IS_CALL_MULTI_MG(con)))
        {
          CCDP(AIN_DBG_LEVEL_0, "con->ccmCb.state is %d\n", (con->ccmCb.state));
          con->ccmCb.state = CC_INGRESS_MG;
        }
    }
    else
    {
        CCDP(AIN_DBG_LEVEL_1, "%s: Unexpected message!\n", __FUNCTION__); 
        RETVALUE(ROK);
    }          

    if ((con->icExtBiccCb) && (ret == ROK))
    {
       CCDP(AIN_DBG_LEVEL_1, "icExtBicc bearer state=%x\n", con->icExtBiccCb->bearerState);

       appInfo = &apmUserInfo.data;
       if ( CC_IS_EXTBICC_BEARERSTATE(con,CC_INCOMING,CC_BICC_STATE_SENT_CONFWD_NOTFY))
       {
         if (((appInfo->actInd.pres == NOTPRSNT) || (appInfo->actInd.val == AI_NOIND)) && 
             (appInfo->bCntrlInfo.pres != NOTPRSNT) && strlen(appInfo->bCntrlInfo.data))
         {
            CCDP(AIN_DBG_LEVEL_2, "receive valid app user info\n");

            CC_SET_EXTBICC_BEARERSTATE(con,CC_INCOMING,CC_BICC_STATE_RCVD_IPBCPREQ);

            /* send TxnModReq */
            MgctEventsDescriptor  evtDesc  = {0};
            MgctSignalsDescriptor sigDesc    = {0};
            MgctSigOther          sigOther   = {0};

            CCDP(AIN_DBG_LEVEL_0, "Adding { BT/BIT {BIT = IPBCP Request} }\n");
            mgctSigOther(&(con->ctl), MGCT_SIG_OTHER_TYPE_BT, NULLP, appInfo->bCntrlInfo.length, 
                          (U8 *)&appInfo->bCntrlInfo.vce, &sigOther);

            mgctSignalDesc(&con->ctl, PRSNT_NODEF, MGCT_SIG_ID_BT_BIT, 
                           MGCT_PACKAGE_ID_BT, SIG_SIGNAL_TYPE_MAX, 0, 
                           &sigOther, NULLP, MGCT_KEEP_ACTIVE_MAX, &sigDesc) ;

            /* Bug 87950 */
            ccReBuildT38EventDescInMod(&con->icBcm, &evtDesc);
            /* Bug 89925 */
            if ((CC_GET_PROTOCOL(con->icProtType) == CC_EXT_BICC) &&
                ((!CC_IS_CALL_MULTI_MG(con)) || ((CC_IS_CALL_MULTI_MG(con)) && (con->ccmCb.state == CC_INGRESS_MG))))
            {
                ccBuildMgctEventsDescriptorForBicc(con, con->icSuConnId, MGCT_EVT_GB_BNCCHANGE_MAX, TRUE, FALSE, &evtDesc);
            }
            /*FID 16458.0*/
            ccUpdateMgQForAddOrModifyRsc(MGCT_MODE_SENDRECV, MGCT_PACKAGE_ID_BT, MGCT_ITEM_ID_CG_MAX, MGCT_CMD_MODIFY,
                                         &(con->icBcm), &evtDesc, NULLP, &sigDesc, NULLP, NULLP,
                                         con->icBcm.mgCtx->suCtxId,
                                         MGCT_CONTEXT_CHOOSE, con);

            con->lastState = con->state;
            CC_STATE_CHANGE(con,  CCS_BICC_AWT_MGCT_CFMNOT);
            con->mgBlock->con = con; 
            /*FID 16458.0*/
            ret = ccSendMgiQueue(con->icBcm.mgCtx->suCtxId, con->mgBlock);
            if (ret != ROK)  
            { 
                CCLOGERR("ccSendMgiQueue : failed during Modify: %ld\n", con->icSuConnId);
                ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
                cleanUpMgQ(con);
                ccHandleCleanUpConCb(con, TRUE, AM_CV_TMPFAIL);
                RETVALUE(ROK);
            }
        }
        else
        {
            CCDP(AIN_DBG_LEVEL_0, "%s, receive APM with invalid action or ipbcp\n", __FUNCTION__); 
            cause = CCPROTERR;
            ret = RFAILED;
        }
      }    
      else if (CC_IS_EXTBICC_BEARERSTATE(con,CC_INCOMING,CC_BICC_STATE_SENT_IPBCPACCEPT))
      {
        if ( (appInfo->actInd.pres != NOTPRSNT) && (appInfo->actInd.val == AI_CONNECTED) ) 
        {
            CCDP(AIN_DBG_LEVEL_2, "receive valid app user info\n");

            /* Bug 82786 Tell IC BICC layer bearer setup done via CC_ET_BICCBEARERDONE */
            cmMemset((U8 *)&biccBearerDoneCnst, 0, sizeof(CcCnStEvnt));
       
            con->ogEvntType = CC_ET_BICCBEARERDONE;
       
            ccSendCnStReq(con, con->icSapCb->suId, &con->icSapCb->pst,
                          con->icSapCb->spId, con->icSpConnId, con->icSuConnId,
                          con->icProtType, con->ogEvntType, &biccBearerDoneCnst, NULL);

            /* COT for BICC to SIP call */
            if ((con->cotBlock.contType == CC_IAM_INITIATED ) && (!CC_IS_CALL_FLAG2(con,CC_FLAG_PRECONDITION)))
            {
                CC_SET_EXTBICC_BEARERSTATE(con,CC_INCOMING,CC_BICC_STATE_WT_COT);
            }
            else
            {
                /*FID 16458.0*/
                if (con->ccmCb.ccn == CCN_1 && con->ccmCb.state == CC_INGRESS_MG &&
                    !CC_IS_BCM_IN_CONTEXT(&con->ogBcm)) /* BUG93337 not MODIFY if added before ConReq out*/
                {
                  CCDP(AIN_DBG_LEVEL_1,"%s():: CCN=%d, Need to process the egress MG now\n",
                       __FUNCTION__, con->ccmCb.ccn);
                  con->ccmCb.state = CC_EGRESS_MG;
                  ccProcessAuthTermSwtReqTrue(con);
                  RETVALUE(ROK);
                }

                /* FID 17373.0 */
                if (con->cotBlock.contType == CC_IAM_INITIATED)
                  CC_SET_EXTBICC_BEARERSTATE(con,CC_INCOMING,CC_BICC_STATE_WT_COT);

                ccSendConReqForExtBicc(con);
            }
        }
        else
        {
            CCDP(AIN_DBG_LEVEL_0, "%s, receive APM with invalid action (connected expected)\n", __FUNCTION__); 
            cause = CCPROTERR;
            ret = RFAILED;
        }
      }
       else
       {
          CCDP(AIN_DBG_LEVEL_2, "%s:Invalid icExtBicc bearer state=%x\n",
               __FUNCTION__, con->icExtBiccCb->bearerState);
       }
    }

    if ( ret != ROK )
    {
        CCDP(AIN_DBG_LEVEL_0, "%s: exception release call\n", __FUNCTION__); 
        /* release call here */
        ccStopConTmr(con, TMR_SETUP);
        ccStopConTmr(con, AIN_T_NO_ANS_TMR);
        ccDeleteBufferedEvent(con, con->ogSuConnId, CCE_TNOANSEXP);
        
        ccStartConTmr(TMR_RELEASE, con, (PTR)&ccCp.genCfg);
        ccDeallocateResource(con, CC_OUTGOING);
        con->ogRscVal = FALSE;

        if (CC_IS_CONTEXT_PRESENT(con))
        {
            ccQSubBasedOnCcn(con);
            ret = ccSendMgiQueue(0, con->mgBlock);
            if (ret != ROK)
            {
                 CCLOGERR( "**** ERROR: can't do ccSendMgiQueue *******\n");
                 cleanUpMgQ(con);
                 ccCleanUpMGCtx(con);
                 ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
            }
            else
            {
                 con->mgBlock->con = con;
                 con->relPend = ICRELONLY;
                 con->tempHolderForReleaseCause = cause;
                 CC_STATE_CHANGE(con, CCS_AWTSWTCFM_SENDONEREL);
                 RETVALUE(ROK);
            }
        }
        CC_STATE_CHANGE(con, CCS_AWTRELCFM_IC);
        ccSendRelease(con,con->icSapCb, con->icSpConnId, con->icSuConnId,
                      con->icProtType, cause, NULL);
    }
    RETVALUE(ROK);
} /* End of ccConE03S147 */

  /*
   *
   *      Fun:   ccConE03S148
   *
   *      Desc:  Connection status indication function
   *             event - CCE_CNST_IND 
   *             state - CCS_BICC_AWT_MGCT_CFMNOT 
   *       
   *      Ret:   ROK     - successful,
   *             RFAILED - unsuccessful
   *
   *      Notes: None. 
   *
   *      File:  cc_bdy2_2.c
   *
   */
PUBLIC S16 ccConE03S148
(   
 CcConCb *con,                    /* connection */
 PTR     event,                   /* event */
 AinBCM_t *bcm,                    /* bcm */
 PTR     event2                    /* event */
 )
{
    ProtType                icProtType     = CC_GET_PROTOCOL(con->icProtType);
    ProtType                ogProtType     = CC_GET_PROTOCOL(con->ogProtType);
    S16  ret;

    /* FID 15319.2 */
    if((con->icEvntType == CCSIP_ET_INFO_OVERLAP) && (con->direction == INCTOOUT))
    {
       RETVALUE(ROK);
    }

    if ( con->icEvntType == CC_ET_ACM )
    {
        CCDP(AIN_DBG_LEVEL_0, "%s: receive ACM before bearer setup finished n", __FUNCTION__);

        ccStopConTmr(con, CC_TMR_EARLYACM);
        ccStopConTmr(con, CC_TMR_TOIW2);
        ccStopConTmr(con, CC_TMR_PREANM);

        ccStartConTmr(TMR_RELEASE, con, (PTR)&ccCp.genCfg);
        con->lastState = con->state;
        CC_STATE_CHANGE(con, CCS_AWTSWTCFM_SENDBOTHREL);
        ccQSubBasedOnCcn(con);
        con->mgBlock->con = con;
        ret = ccSendMgiQueue(0, con->mgBlock);
        if (ret != ROK)
        {
             CCLOGERR( "ERROR occured \n");
             cleanUpMgQ(con);
             ccCleanUpMGCtx(con);
             ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
             RETVALUE(ROK);
        }

        /* Release call with cause value 31, SIP rsp code 480 is mapped
         * from cause 31.
         */
        con->tempHolderForReleaseCause = CCNORMUNSPEC;
        con->ainInfo.diconnectCause    = CCNORMUNSPEC;
    }
    else if(con->icEvntType == CC_ET_APPTRAN)
    {
        CCDP(AIN_DBG_LEVEL_0, "%s: dir:%d, icProtType:%d, ogProtType:%d\n",
                  __FUNCTION__,con->direction, icProtType, ogProtType );

        /* FID 15319.0, queue the signal APM msg as we are still waiting for the MGCT CFM */
        if(((con->direction == INCTOOUT) && (icProtType == CC_EXT_BICC)) ||
           ((con->direction == OUTTOINC) && (ogProtType == CC_EXT_BICC)))
        {
            ccBufferEventToQueue(con, bcm, event, CCE_CNSTIND, sizeof(CcConEvnt), 0, NULLP, 0);
            RETVALUE(ROK);
        }
        else
        {
            CCDP(AIN_DBG_LEVEL_0, "Rcvd CC_ET_APPTRAN for dir:%d, icProtType:%d, ogProtType:%d\n",
                  con->direction, icProtType, ogProtType );
        }

    }
    else
    {
        CCDP(AIN_DBG_LEVEL_0, "%s receive unexpected msg (%d) just ignore it \n", __FUNCTION__, con->icEvntType); 
    }
    RETVALUE(ROK);
} /* End of ccConE03S148 */


/*
 *
 *      Fun:   ccConE04S147
 *
 *      Desc:  Connection state function
 *             event - Release indication
 *             state - CCS_BICC_AWTICBEARER 
 *      Ret:   ROK     - successful,
 *             RFAILED - unsuccessful
 *
 *      Notes: None.
 *
 *      File:  cc_bdy2_2.c
 *
 */
PUBLIC S16 ccConE04S147
(
 CcConCb *con,                    /* connection */
 PTR     event,                   /* event */
 AinBCM_t *bcm,                    /* bcm */
 PTR     event2                    /* event */
 )
{
    S16 ret   = RFAILED;
    S16 mgret = RFAILED;

    ret = ccProcessDisconnectEvent(con, event);

    if (ret == ROK)
    {
      CCDP(AIN_DBG_LEVEL_0,"ccProcessDisconnectEvent success,event detec.\n");
      RETVALUE(ROK);
    }
    else if (ret == RFAILED)
    {
      CCLOGERR("Failed to Check if (O/T)Disc event is armed\n");
    }
    /* If the ccProcessDisconnectEvent returns ROKDNA, we continue */
    CCDP(AIN_DBG_LEVEL_0, "O/T_Disconnect event is not Armed\n");
    
    ccStopConTmr(con, TMR_SETUP);
    ccStopConTmr(con, AIN_O_NO_ANS_TMR);
    ccStopConTmr(con, AIN_T_NO_ANS_TMR);
    ccDeleteBufferedEvent(con, con->icSuConnId, CCE_ONOANSEXP);
    ccDeleteBufferedEvent(con, con->ogSuConnId, CCE_TNOANSEXP);

    ccStopConTmr(con, CC_TMR_TOIW2);

    if (CC_IS_CONTEXT_PRESENT(con))
    {
      ccQSubBasedOnCcn(con);

      con->mgBlock->con = con;
      mgret = ccSendMgiQueue(0, con->mgBlock);
      if(mgret != ROK)
      {
          CCLOGERR("ccHandleMgResponse or ccUpdateMgQForSubRsc or SendMgiQ failed \n");
          cleanUpMgQ(con);
          ccCleanUpMGCtx(con);

          if (con->noRspReqd == FALSE)
          {
              ccSendReleaseRsp(con);
          }

          if(con->ogRscVal == TRUE)
          {
              ccDeallocateResource(con, CC_OUTGOING);
          }
          con->ogRscVal = FALSE;

          CC_STATE_CHANGE(con, CCS_AWTDEALOCCFM);
          ccDeallocateResource(con, CC_INCOMING);
          RETVALUE(RFAILED);
       }
       con->relPend = INRELRSPANDNOREL;
       CC_STATE_CHANGE(con, CCS_AWTSWTCFM_SENDNOREL);
    }
    else
    {
       cleanUpMgQ(con);
       ccCleanUpMGCtx(con);
       ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
    }

    RETVALUE(ROK);
} /* End of ccConE04S147 */

  /*
   *
   *      Fun:   ccConE11S148
   *
   *      Desc:  Connection state function
   *             event - Switch Connect Confirm.
   *             state - CCS_BICC_AWT_MGCT_CFMNOT 
   *
   *      Ret:   ROK     - successful,
   *             RFAILED - unsuccessful
   *
   *      Notes: None.
   *
   *      File:  cc_bdy2_2.c
   *
   */
PUBLIC S16 ccConE11S148
(
 CcConCb *con,                    /* connection */
 PTR     event,                   /* event */
 AinBCM_t *bcm,                    /* bcm */
 PTR     event2                    /* event */
 )
{
    CcExtBiccCb             *pExtBiccCb = NULLP;
    AinBCM_t                *pBcm       = NULLP;
    CcCnStEvnt              apmEvnt;
    BiccApmUserInfo         apmUserInfo;
    Bool                    needSendCnStReq= FALSE;
    Bool                    needResetState = FALSE;
    S16                     mgret = RFAILED;
    U8                      extBiccLeg=CC_INCOMING;

    CCDP(AIN_DBG_LEVEL_1, "%s:%ld,%ld,%ld\n", __FUNCTION__, con->icSuConnId,
                         con->ogSuConnId, con->mgBlock->mgCtx->suCtxId);

    if ( con->direction == OUTTOINC )
    {
        pExtBiccCb = con->ogExtBiccCb;
        pBcm = &(con->icBcm);
        extBiccLeg = CC_OUTGOING;
    }
    else
    {
        pExtBiccCb = con->icExtBiccCb;
        pBcm = &(con->ogBcm);
        extBiccLeg = CC_INCOMING;
    }

    S16 ret = ccHandleMgResponse(con, (MgctTxnCfm*) event);

    if ( ret != ROK )
    {
        /* don't release the call when failed to handleMgResponse for DG/Dx */
        if (pExtBiccCb &&
            CC_IS_EXTBICC_BEARERSTATE(con,extBiccLeg,CC_BICC_DTMF_RCVD_STARTSIGNOT))
        {
            CCDP(AIN_DBG_LEVEL_0,"failed to handleMgResponse for DG/Dx.\n");
            /* Prepare for a reject to "start signal" apm */
            cmMemset((U8 *)&apmEvnt, 0, sizeof(CcCnStEvnt));
            cmMemset((U8 *)&apmUserInfo, 0, sizeof(BiccApmUserInfo));
            cmMemcpy((U8*)&apmUserInfo.hdr, (U8*)&pExtBiccCb->apmUsrInfoHdr, sizeof(BiccApmEncHdr));
            apmUserInfo.data.actInd.pres = PRSNT_NODEF;
            apmUserInfo.data.actInd.val = AI_STASIGREJ;
            ccSetBiccAppTransParam(&(apmEvnt.m.siCnStEvnt.appTransParam));
            ccSetAppApmUserInfo(&(apmEvnt.m.siCnStEvnt), &apmUserInfo);
            needSendCnStReq = TRUE;
            needResetState = TRUE;
            goto EGRESS;
        }
        else if (pExtBiccCb && 
                 CC_IS_EXTBICC_BEARERSTATE(con,extBiccLeg,CC_BICC_DTMF_RCVD_STARTSIG))
        {
            needResetState = TRUE;
            goto EGRESS;
        }


        CCDP(AIN_DBG_LEVEL_0, "%s:%ld,%ld,%ld failed to handleMgResponse, release call\n", __FUNCTION__, con->icSuConnId,
                         con->ogSuConnId, con->mgBlock->mgCtx->suCtxId);

        /* release call via FailCnd mapped from Megaco error */
        con->failCndVal  = ccGetMegacoFailCnd(con);

        ccQSubBasedOnCcn(con);

        con->mgBlock->con = con;
        mgret = ccSendMgiQueue(0, con->mgBlock);
        if(mgret != ROK)
        { 
            CCLOGERR("ccHandleMgResponse or ccUpdateMgQForSubRsc or SendMgiQ. failed \n");
            cleanUpMgQ(con);
            ccCleanUpMGCtx(con);
            ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
        }
        else
        {
            if(con->lastState == CCS_BICC_AWTICBEARER) 
            {
                CC_STATE_CHANGE(con, CCS_AWTSWTCFM_SENDNOREL);
                con->relPend = NORELANDINFAILCOND;
            }
            else 
            {
               CC_STATE_CHANGE(con, CCS_AWTSWTCFM_SENDONEREL);
               con->relPend = OUTRELANDINFAILCOND;
               con->tempHolderForReleaseCause = CCNORMUNSPEC;
            }
            con->lastState = CCS_BICC_AWT_MGCT_CFMNOT;
        } 
        RETVALUE(ROK);
    } 

    if ( pExtBiccCb )
    {
        CCDP(AIN_DBG_LEVEL_0,"The bearer state: %d.\n", pExtBiccCb->bearerState);
        switch ( pExtBiccCb->bearerState )
        {
            case CC_BICC_STATE_RCVD_IPBCPREQ:
            case CC_BICC_STATE_RCVD_CONFWD_NOTFY:
            case CC_BICC_STATE_RCVD_IPBCPACCEPT:
                CCDP(AIN_DBG_LEVEL_0, "Ignore the MgctTxnCfm");
                break;
            case CC_BICC_DTMF_RCVD_STARTSIGNOT:
                /* dtmf handling */
                cmMemset((U8 *)&apmEvnt, 0, sizeof(CcCnStEvnt));
                cmMemset((U8 *)&apmUserInfo, 0, sizeof(BiccApmUserInfo));
                cmMemcpy((U8*)&apmUserInfo.hdr, (U8*)&pExtBiccCb->apmUsrInfoHdr, sizeof(BiccApmEncHdr));
                apmUserInfo.data.actInd.pres = PRSNT_NODEF;
                apmUserInfo.data.actInd.val = AI_STASIGACK;
                ccSetBiccAppTransParam(&(apmEvnt.m.siCnStEvnt.appTransParam));
                ccSetAppApmUserInfo(&(apmEvnt.m.siCnStEvnt), &apmUserInfo);
                needSendCnStReq = TRUE;
                needResetState = TRUE;
                break;
            case CC_BICC_DTMF_RCVD_STARTSIG:
                needResetState = TRUE;
                break;
            default:
                CCDP(AIN_DBG_LEVEL_0, "unexptected, Ignore the MgctTxnCfm");
                break;
        }
    }

EGRESS:
    if ( needSendCnStReq )
    {
        if(con->direction == OUTTOINC)
        {
            con->ogEvntType = CC_ET_APPTRAN;
            ccSendCnStReq(con, con->ogSapCb->suId, &con->ogSapCb->pst,
                          con->ogSapCb->spId, con->ogSpConnId, con->ogSuConnId,
                          con->ogProtType, con->ogEvntType, &apmEvnt, NULLP);
        }
        else
        {
            con->ogEvntType = CC_ET_APPTRAN;
            ccSendCnStReq(con, con->icSapCb->suId, &con->icSapCb->pst,
                          con->icSapCb->spId, con->icSpConnId, con->icSuConnId,
                          con->icProtType, con->ogEvntType, &apmEvnt, NULLP);
        }
    }

    if ( needResetState )
    {
        CC_SET_EXTBICC_BEARERSTATE(con,extBiccLeg,CC_BICC_STATE_NULL);
        ccRestoreCallStateOfAssociatedCalls(pBcm);
        CC_STATE_CHANGE(con, con->lastState);
        con->lastState = CCS_BICC_AWT_MGCT_CFMNOT;
    }

    RETVALUE(ROK);
} /* End of ccConE11S148 */

  /*
   *
   *      Fun:   ccConE14S147
   *
   *      Desc:  Connection state function
   *             event - CCE_SETUPTMREXP
   *             state - CCS_BICC_AWTICBEARER 
   *      Ret:   ROK     - successful,
   *             RFAILED - unsuccessful
   *
   *      Notes: None.
   *
   *      File:  cc_bdy2_2.c
   *
   */
PUBLIC S16 ccConE14S147
(
 CcConCb *con,                    /* connection */
 PTR     event,                   /* event */
 AinBCM_t *bcm,                    /* bcm */
 PTR     event2                    /* event */
 )
{
    CCDP(AIN_DBG_LEVEL_1, "%s:%ld,%ld,%ld\n", __FUNCTION__, con->icSuConnId,
                         con->ogSuConnId, con->mgBlock->mgCtx->suCtxId);
    ccGenAlarm(LCM_CATEGORY_PROTOCOL, LCM_EVENT_INV_STATE, 
                LCC_CAUSE_SETUPTMR_EXP, (U8 *) &con->icSuConnId);

    ccStartConTmr(TMR_RELEASE, con, (PTR)&ccCp.genCfg);

    S16 mgret = RFAILED; 
    ccStopConTmr(con, AIN_O_NO_ANS_TMR);
    ccStopConTmr(con, AIN_T_NO_ANS_TMR);
    ccDeleteBufferedEvent(con, con->icSuConnId, CCE_ONOANSEXP);
    ccDeleteBufferedEvent(con, con->ogSuConnId, CCE_TNOANSEXP);

    ccQSubBasedOnCcn(con);

    con->mgBlock->con = con;
    mgret = ccSendMgiQueue(0, con->mgBlock);
    if(mgret != ROK)
    {
        CCLOGERR("ccHandleMgResponse or ccUpdateMgQForSubRsc or SendMgiQ. failed \n");
        cleanUpMgQ(con);
        ccCleanUpMGCtx(con);
        ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
    }
    else
    {
        con->relPend = ICRELONLY;
        CC_STATE_CHANGE(con, CCS_AWTSWTCFM_SENDONEREL);
        con->direction = INCTOOUT; 
    }

    RETVALUE(ROK);
} /* End of ccConE14S147 */

  /*
   *
   *      Fun:   ccConE14S148
   *
   *      Desc:  Connection state function
   *             event - CCE_SETUPTMREXP
   *             state - CCS_BICC_AWT_MGCT_CFMNOT 
   *      Ret:   ROK     - successful,
   *             RFAILED - unsuccessful
   *
   *      Notes: None.
   *
   *      File:  cc_bdy2_2.c
   *
   */
PUBLIC S16 ccConE14S148
(
 CcConCb *con,                    /* connection */
 PTR     event,                   /* event */
 AinBCM_t *bcm,                    /* bcm */
 PTR     event2                    /* event */
 )
{
    CCDP(AIN_DBG_LEVEL_1, "%s:%ld,%ld,%ld\n", __FUNCTION__, con->icSuConnId,
                         con->ogSuConnId, con->mgBlock->mgCtx->suCtxId);
    ccGenAlarm(LCM_CATEGORY_PROTOCOL, LCM_EVENT_INV_STATE, 
                LCC_CAUSE_SETUPTMR_EXP, (U8 *) &con->icSuConnId);

    cleanUpMgQ(con);            
    ccCleanUpMGCtx(con);
    ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
    RETVALUE(ROK);          
} /* End of ccConE14S148 */

/*
 *
 *      Fun:   ccConE23S147
 *
 *      Desc:  Connection state function
 *             event - Continuity Report
 *             state - CCS_BICC_AWTICBEARER 
 *      Ret:   ROK     - successful,
 *             RFAILED - unsuccessful
 *
 *      Notes: None.
 *
 *      File:  cc_bdy2_2.c
 *
 */
PUBLIC S16 ccConE23S147
(
 CcConCb *con,                    /* connection */
 PTR     event,                   /* event */
 AinBCM_t *bcm,                    /* bcm */
 PTR     event2                    /* event */
 )
{
    CCDP(AIN_DBG_LEVEL_1, "%s:%ld,%ld,%ld\n", __FUNCTION__, con->icSuConnId,
                         con->ogSuConnId, con->mgBlock->mgCtx->suCtxId);

    if ( con->icExtBiccCb && 
        CC_IS_EXTBICC_BEARERSTATE(con,CC_INCOMING,CC_BICC_STATE_WT_COT))
    {
        /* FID 17373.0: support incoming COT */
        CC_SET_COT_RCVD_EXT_BICC(con);
        if(CC_IS_CALL_FLAG2(con,CC_FLAG_PRECONDITION) && (con->ogPrecndCb) && (CC_GET_PROTOCOL(con->icProtType) == CC_EXT_BICC))
        {
            if (PRECND_IS_CALL_FLAG2(con->ogPrecndCb, PRECND_FLAG_PRACK_SENT) != TRUE)
            {
              PRECND_SET_CALL_FLAG2(con->ogPrecndCb, PRECND_FLAG_NO_UPDT);
              CCDP(AIN_DBG_LEVEL_2,"BICC2SIP received COT, set noUpdate\n");
            }
        }
        ccSendConReqForExtBicc(con);    
    }
    RETVALUE(ROK);
} /* End of ccConE23S147 */

/*
 *
 *      Fun:   ccConE62S148
 *
 *      Desc:  Connection state function
 *             event - MGCT Notify 
 *             state - CCS_BICC_AWT_MGCT_CFMNOT 
 *      Ret:   ROK     - successful,
 *             RFAILED - unsuccessful
 *
 *      Notes: None.
 *
 *      File:  cc_bdy2_2.c
 *
 */
PUBLIC S16 ccConE62S148
(
 CcConCb *con,                    /* connection */
 PTR     event,                   /* event */
 AinBCM_t *bcm,                    /* bcm */
 PTR     event2                    /* event */
 )
{
    MgctTxnInd       *mgctTxnInd = (MgctTxnInd *) event;
    CcCnStEvnt        biccBearerDoneCnst; /* Bug 82786 */
    CcExtBiccCb      *biccCb     = NULLP;
    MgctNotifyKind_e  kind       = MGCT_NOTIFY_KIND_UNKNOWN;
    U16               megacoErr  = 0;
    U16               cause      = CCTMPFAIL;
    S16               ret;
    Bool              isAnsiBicc = FALSE;

    CCDP(AIN_DBG_LEVEL_1, "%s:%ld,%ld,%ld\n", __FUNCTION__,
          con->icSuConnId,con->ogSuConnId, con->mgBlock->mgCtx->suCtxId);

    if((con->lastState == CCS_BICC_AWTICBEARER) &&
       (ccCheckEventInBuffer(&(con->icBcm), CCE_RELIND)))
    {
        /* Change back to laststate S147 to handle buffered REL in E04S147 */
        CC_STATE_CHANGE(con, CCS_BICC_AWTICBEARER);
        RETVALUE(ROK);
    }
    else if((con->lastState == CCS_AWTANS) && 
           (ccCheckEventInBuffer(&(con->ogBcm), CCE_RELIND)))
    {
       /* Change back to laststate S09 to handle buffered REL in E04S09 */
       CC_STATE_CHANGE(con, CCS_AWTANS);
       RETVALUE(ROK);
    }

    ret = ccHandleMgctNotify(con, (MgctTxnInd *) event);
    if (ret != ROK)
    {
        CCLOGERR("Error: ccHandleMgctNotify failed icSu = 0x%lx,"
                 "suCtxId 0x%lx, contextId = 0x%lx\n", con->icSuConnId,
                 con->mgBlock->mgCtx->suCtxId, con->mgBlock->mgCtx->contextId);
    }
    else 
    {
        /* BUG: 87494 in case E11 is not received in conner case
         * Handle E62 as if E11 had been received
         */
        cleanUpMgQ(con);
        /*End of 87494*/
        ret = mgctGetNotifyReqKind(mgctTxnInd, &megacoErr, &kind, NULLP);
        if  ( ret == RFAILED )
        {
            CCLOGERR("Error: mgctGetNotifyReqKind failed icSu = 0x%lx,"
                 "suCtxId 0x%lx, contextId = 0x%lx\n", con->icSuConnId,
                 con->mgBlock->mgCtx->suCtxId, con->mgBlock->mgCtx->contextId);
        }
        /* FID 14841.0 */
        HNDL_MGCT_NOT_FOR_ADID(kind, con, event, bcm);

        biccCb = ( bcm->bcmType == BtOBcm ) ? con->icExtBiccCb : con->ogExtBiccCb;
        if( !biccCb )
        {
            CCLOGERR("Error: %s:%ld,%ld extBiccCb not found\n", __FUNCTION__, con->icSuConnId, con->ogSuConnId);
            ret = RFAILED;
        }

        if( kind == MGCT_NOTIFY_KIND_G_CAUSE )
        {
            CCDP(AIN_DBG_LEVEL_0, "%s:%ld,%ld,%ld receive g/cause from MGI\n", __FUNCTION__, con->icSuConnId,
                         con->ogSuConnId, con->mgBlock->mgCtx->suCtxId);
            /* release call based on the general cause and failure cause */

            MgctEvt_G_C_GeneralCause gcause = mgctTxnInd->u.notifyReq.observedEventsDesc.
                          observedEventList->observedEventParameterList->u.eventOther.u.gc.generalCause;

            if ( gcause == MGCT_EVT_G_C_GEN_CAUSE_IW )
                cause = CCINTRWRK;
            else if ( gcause == MGCT_EVT_G_C_GEN_CAUSE_FT )
                cause = CCRESCUNAVAIL;
            else if ( gcause == MGCT_EVT_G_C_GEN_CAUSE_UR )
                cause = CCRESCUNAVAIL;
             
            ret = RFAILED;
        }
    }

    /* FID16747.0 */
    isAnsiBicc = (bcm->bcmType == BtOBcm) ?  CC_IS_PROT_TYPE_ANSI_BICC(con->icIntfcCb) : CC_IS_PROT_TYPE_ANSI_BICC(con->ogIntfcCb);

    if(ret == ROK)
    {
      CcCnStEvnt      apmEvnt;
      BiccApmUserInfo apmUserInfo;

      cmMemset((U8*)&apmEvnt, 0, sizeof(CcCnStEvnt));
      cmMemset((U8*)&apmUserInfo, 0, sizeof(BiccApmUserInfo));

      cmMemcpy((U8*)&apmUserInfo.hdr, (U8*)&biccCb->apmUsrInfoHdr, sizeof(BiccApmEncHdr));

      CCDP(AIN_DBG_LEVEL_2, "bicc cb's bearer state: %d\n", biccCb->bearerState);
      if ( biccCb->bearerState == CC_BICC_STATE_RCVD_IPBCPREQ )
      {
        if (  kind == MGCT_NOTIFY_KIND_BT_TIND  
            && mgctGetObservedEventPkgBT(mgctTxnInd,(U8 *)&apmUserInfo.data.bCntrlInfo.vce,
                                        &apmUserInfo.data.bCntrlInfo.length) == ROK )
        {
           CCDP(AIN_DBG_LEVEL_5, "receive valid BT TIND, len: %d\n", apmUserInfo.data.bCntrlInfo.length);
           apmUserInfo.data.bCntrlInfo.pres = PRSNT_NODEF;

           BiccApmEncData *appInfo = &apmUserInfo.data;
           Sdp *sdp = NULLP;
           MediaStream_t *msPtr = NULLP;
           U8 idx;
           U8 tgtCdcIdxVal;
           U8 orgId;
           U8 cdcType;
           U8 cdcTypeFlag;

           if ( biccCb->biccFlavor == CC_BICC_FLAVOR_FAST_FWD )
           {
              /* fill the Action Indicator based on profile & IAM's codec info */
              appInfo->actInd.pres = PRSNT_NODEF;

              /* CSD call backward 1st APM with or without single codec depends on codec list from IAM */
              if ((con->callDtlInfo.cm.tMR.pres == PRSNT_NODEF) &&
                  (con->callDtlInfo.cm.tMR.val == TMR_64KBITS))
              {
                 if (biccCb->apmUsrInfoData->cdcLst.pres == PRSNT_NODEF)
                 {
                    appInfo->sglCdc.pres = PRSNT_NODEF;
                    appInfo->sglCdc.orgId = OI_ITU_T;

                    if (isAnsiBicc)
                    {
                       appInfo->sglCdc.cdcInfo.cdcType = CT_G711_64K_M;
                    }
                    else
                    {
                       for (idx = 0; idx < MAX_CDC_LIST; idx++)
                       {
                          tgtCdcIdxVal = biccCb->tgtCdcIdx[idx];
                          orgId   = biccCb->apmUsrInfoData->cdcLst.cdc[tgtCdcIdxVal].orgId;
                          cdcType = biccCb->apmUsrInfoData->cdcLst.cdc[tgtCdcIdxVal].cdcInfo.cdcType;
                          CC_MAP_CT_FLAG(orgId, cdcType, cdcTypeFlag);
                          if (CC_IS_CT_FLAG_SET(biccCb->tgtCdcTypeFlag, cdcTypeFlag) != 0)
                          {
                             appInfo->sglCdc = biccCb->apmUsrInfoData->cdcLst.cdc[tgtCdcIdxVal];
                             break;
                          }
                       }
                    }
                    
                    if (CC_IS_EXTBICC_NOTIFY(con,CC_INCOMING))
                    {
                       appInfo->actInd.val = AI_CONFWDPNSC;
                    }
                    else
                    {
                       appInfo->actInd.val = AI_CONFWDNNSC;
                    }
                 }
                 else
                 {
                    appInfo->sglCdc.pres = NOTPRSNT;  
                    if(CC_IS_EXTBICC_NOTIFY(con,CC_INCOMING))
                    {
                       appInfo->actInd.val = AI_CONFWDPN;
                    }
                    else
                    {
                       appInfo->actInd.val = AI_CONFWDNN;
                    }
                 }
                 appInfo->cdcLst.pres = NOTPRSNT;
              }
              else
              {
                 if (CC_IS_EXTBICC_NOTIFY(con,CC_INCOMING))
                 {
                    appInfo->actInd.val = AI_CONFWDPNSC;
                 }
                 else
                 {
                    appInfo->actInd.val = AI_CONFWDNNSC;
                 }
    
                 /* fill the selected codec & availabel codec list if not existing */
                 appInfo->sglCdc.pres = PRSNT_NODEF;
                 appInfo->sglCdc.orgId = OI_ITU_T;
                 
                 appInfo->cdcLst.pres = PRSNT_NODEF;
                 appInfo->cdcLst.numCdcs = 1;
                 appInfo->cdcLst.cdc[0].orgId = OI_ITU_T;
                 if (isAnsiBicc)
                 {
                    appInfo->sglCdc.cdcInfo.cdcType = CT_G711_64K_M;
                    appInfo->cdcLst.cdc[0].cdcInfo.cdcType = CT_G711_64K_M;
                 }
                 else if (con->icIpParam.locDesc)
                 {
                    sdp = con->icIpParam.locDesc;
                    msPtr = (MediaStream_t *)&sdp->p[0];
                    appInfo->sglCdc.orgId = msPtr->m.voip.codecCfg[0].orgnId.val;
                    appInfo->sglCdc.cdcInfo.cdcType = msPtr->m.voip.codecCfg[0].codecInfo.val;
                    if (appInfo->sglCdc.cdcInfo.cdcType == CTE_UMTS_AMR2)                                     
                    {
                       if (msPtr->m.voip.codecCfg[0].bitMap.pres &&
                           strlen(msPtr->m.voip.codecCfg[0].bitMap.val) == 6)
                       {
                          ccStr2Hex(appInfo->sglCdc.cdcInfo.cdcCfg, 
                                    msPtr->m.voip.codecCfg[0].bitMap.val, 3, &appInfo->sglCdc.cdcInfo.length);
                       }
                    }
                    cmMemcpy((U8 *)&appInfo->cdcLst.cdc[0], (U8 *)&appInfo->sglCdc, sizeof(Codec));
                 }
              }
              appInfo->bCntrlTun.pres = PRSNT_NODEF;
              appInfo->bCntrlTun.indicator = 1;
              appInfo->bncChar.pres = PRSNT_NODEF;
              appInfo->bncChar.val = BC_IP_RTP;
              CCDP(AIN_DBG_LEVEL_0, "%s:actInd.val:%d bncChar.val:%d Send backward APM for Fast forward.\n", 
                    __FUNCTION__,appInfo->actInd.val,appInfo->bncChar.val);
            }
            else
            {
              appInfo->actInd.pres = PRSNT_NODEF;
              appInfo->actInd.val = AI_NOIND;

              appInfo->bCntrlTun.pres = PRSNT_NODEF;
              appInfo->bCntrlTun.indicator = 1;
            }
            biccCb->bearerState = CC_BICC_STATE_SENT_IPBCPACCEPT;
      
            /* SEND APM */
            con->ogEvntType = CC_ET_APPTRAN;
            ccSetBiccAppTransParam(&apmEvnt.m.siCnStEvnt.appTransParam);
              
            ccSetAppApmUserInfo(&apmEvnt.m.siCnStEvnt, &apmUserInfo); 

            ccSendCnStReq(con, con->icSapCb->suId, &con->icSapCb->pst,
                            con->icSapCb->spId, con->icSpConnId, con->icSuConnId,
                            con->icProtType, con->ogEvntType, &apmEvnt, NULL);

            if ( !biccCb->isNotifyNeed )
            {
                /* TL1 don't set ReqNotify as true, bearer setup already is completed here
                 * we send out connReq directly if no COT */

                /* Bug 82786 Tell IC BICC layer bearer setup done via CC_ET_BICCBEARERDONE */
                cmMemset((U8 *)&biccBearerDoneCnst, 0, sizeof(CcCnStEvnt));

                con->ogEvntType = CC_ET_BICCBEARERDONE;

                ccSendCnStReq(con, con->icSapCb->suId, &con->icSapCb->pst,
                              con->icSapCb->spId, con->icSpConnId, con->icSuConnId,
                              con->icProtType, con->ogEvntType, &biccBearerDoneCnst, NULL);

                /* BICC -> SIP  FID 16747.0 EXTBICC might terminate to ISDN, SIPT, internal BICC right now */

                /* FID 17373.0 support incoming COT. If precondition is found, send out INVITE to continue precondition first,
                 * Then handle COT. Whenevern COT is received during precondition, mark it as received in ogPrecndCb.
                 * After precondition is done, the COT should trigger a UPDATE to SIP side.
                 */
                if ( con->cotBlock.contType == CC_IAM_INITIATED && (!con->ogPrecndCb))
                {
                    biccCb->bearerState = CC_BICC_STATE_WT_COT;
                    CC_STATE_CHANGE(con,  CCS_BICC_AWTICBEARER);
                }
                else
                {
                    /* enter S09 after sending connReq in this func */
                  /* FID17373.0 */
                  if (con->cotBlock.contType == CC_IAM_INITIATED)
                    CC_SET_EXTBICC_BEARERSTATE(con,CC_INCOMING,CC_BICC_STATE_WT_COT);

                  ccSendConReqForExtBicc(con);
                }
            } 
            else
            {
                CC_STATE_CHANGE(con, CCS_BICC_AWTICBEARER);
            }

            RETVALUE(ROK);
        }
        else
        {
            CCDP(AIN_DBG_LEVEL_0, "%s:%ld,%ld,%ld BT/TIND IPBCP/Accept invalid\n", __FUNCTION__, con->icSuConnId,
                         con->ogSuConnId, con->mgBlock->mgCtx->suCtxId);
            /* req: orig0023 */
            cause = CCPROTERR;
            ret = RFAILED;
        }
      }
      else if ( biccCb->bearerState == CC_BICC_STATE_RCVD_CONFWD_NOTFY )
      {
        if (  kind == MGCT_NOTIFY_KIND_BT_TIND  
            && mgctGetObservedEventPkgBT(mgctTxnInd,(U8 *)&apmUserInfo.data.bCntrlInfo.vce,
                                        &apmUserInfo.data.bCntrlInfo.length) == ROK )
        {
            CCDP(AIN_DBG_LEVEL_5, "receive valid BT TIND, len:%d,ipbcp:%s\n", 
                 apmUserInfo.data.bCntrlInfo.length,apmUserInfo.data.bCntrlInfo.data);
            apmUserInfo.data.bCntrlInfo.pres = PRSNT_NODEF;

            BiccApmEncData * appInfo = &apmUserInfo.data;

            appInfo->actInd.pres = PRSNT_NODEF;
            appInfo->actInd.val = AI_NOIND;

            appInfo->bCntrlTun.pres = PRSNT_NODEF;
            appInfo->bCntrlTun.indicator = 1;

            appInfo->sglCdc.pres = NOTPRSNT;
            appInfo->cdcLst.pres = NOTPRSNT;

            biccCb->bearerState = CC_BICC_STATE_SENT_IPBCPREQ;
      
            /* SEND APM */
            con->ogEvntType = CC_ET_APPTRAN;
            ccSetBiccAppTransParam(&apmEvnt.m.siCnStEvnt.appTransParam);
              
            ccSetAppApmUserInfo(&apmEvnt.m.siCnStEvnt, &apmUserInfo); 

            ccSendCnStReq(con, con->ogSapCb->suId, &con->ogSapCb->pst,
                            con->ogSapCb->spId, con->ogSpConnId, con->ogSuConnId,
                            con->ogProtType, con->ogEvntType, &apmEvnt, NULL);

            CC_STATE_CHANGE(con, CCS_AWTANS);
        }
        else
        {
            CCDP(AIN_DBG_LEVEL_0, "%s:%ld,%ld,%ld BT/TIND IPBCP/Request invalid\n", __FUNCTION__, con->icSuConnId,
                         con->ogSuConnId, con->mgBlock->mgCtx->suCtxId);
            /* req: orig0023 */
            cause = CCPROTERR;
            ret = RFAILED;
        }
      }
      else if ( biccCb->bearerState == CC_BICC_STATE_RCVD_IPBCPACCEPT )
      {
        if ( kind == MGCT_NOTIFY_KIND_GB_BNCCHANGE
          && mgctTxnInd->u.notifyReq.observedEventsDesc.observedEventList->observedEventParameterList->u.eventOther.u.gbbncchange
                     ==  MGCT_EVT_GB_BNCCHANGE_EST )
        {
            CCDP(AIN_DBG_LEVEL_5, "receive valid BT TIND, len: %d\n", apmUserInfo.data.bCntrlInfo.length);
            if ( biccCb->isNotifyNeed )
            {
                BiccApmEncData * appInfo = &apmUserInfo.data;
                appInfo->actInd.pres = PRSNT_NODEF;
                appInfo->actInd.val = AI_CONNECTED; 

                appInfo->sglCdc.pres = NOTPRSNT;
                appInfo->cdcLst.pres = NOTPRSNT;

                appInfo->bCntrlInfo.pres = NOTPRSNT;
                appInfo->bCntrlTun.pres = NOTPRSNT;

                /* SEND APM */
                con->ogEvntType = CC_ET_APPTRAN;
                ccSetBiccAppTransParam(&apmEvnt.m.siCnStEvnt.appTransParam);
              
                ccSetAppApmUserInfo(&apmEvnt.m.siCnStEvnt, &apmUserInfo); 

                ccSendCnStReq(con, con->ogSapCb->suId, &con->ogSapCb->pst,
                            con->ogSapCb->spId, con->ogSpConnId, con->ogSuConnId,
                            con->ogProtType, con->ogEvntType, &apmEvnt, NULL);
            }
            biccCb->bearerState = CC_BICC_STATE_SENT_CONNECTED;

            /* FID 17373.0: If SIP precondition is needed, don't send BEARERDONE untill precondition is done */
            if (CC_IS_CALL_FLAG2(con,CC_FLAG_PRECONDITION) &&
                (CC_GET_PROTOCOL(con->ogProtType) == CC_EXT_BICC) &&
                (ccIsOgContChkRqstd(con)==CONTCHK_REQ) &&
                con->icIpParam.locDesc && 
                !CC_IS_CALL_MULTI_MG(con) &&
                (con->icPrecndCb) &&
                !IS_PRECND_COMPLETED(con->icPrecndCb->flag))
            {
               if (CC_OFFANS_ICOFFER_PENDING(con))
               {
                 Sdp* icLoc = NULLP;
                 if(con->icIpParam.locDesc)
                 {
                   CCDP(AIN_DBG_LEVEL_1, "%s: FID 17373 Sending 183 OFFER to incoming side\n",__FUNCTION__);

                   icLoc = con->icIpParam.locDesc;
                   ccUpdatePrecndStaTblIniNonPeer(con, con->icPrecndCb, PRECND_STRENGTH_MANDATORY, icLoc->numDecode);
                   ccSend183ToInc(con);
                   CC_OFFANS_STATE_CHANGE(con, CC_OFFANS_STATE_SENTOFFER,
                                             CC_OFFANS_STATE_NO_OP,
                                             CC_OFFANS_SUBSTATE_AWAITRMTANS_INVITENOSDP,
                                             CC_OFFANS_TYPE_REL18X_INVITENOSDP);


                   CC_STATE_CHANGE(con, CCS_AWT_PRECONDITION);
                   return ROK;
                 }else
                 {
                   CCDP(AIN_DBG_LEVEL_1, "%s: incoming leg's sdp is NULL! \n",__FUNCTION__);
                 }
               }
               else if(CC_OFFANS_ICANS_PENDING(con))
               {
                 U8 done = RFAILED;
                 ccEnablePrecndCb(con->icPrecndCb);
                 done = ccTryToInsertPrecndConfStatus(con->icPrecndCb);
                 ccSetPrecndLocalStatus(con->icPrecndCb, PRECND_DIR_SENDRECV);

                 CCDP(AIN_DBG_LEVEL_1, "%s: FID 17373 Sending 183 ANSWER to incoming side\n",__FUNCTION__);

                 ccSend183ToInc(con);
                 CC_OFFANS_STATE_CHANGE(con, CC_OFFANS_STATE_NULL,
                                   CC_OFFANS_STATE_NO_OP,
                                   CC_OFFANS_GET_SUBSTATE(con),
                                   CC_OFFANS_METHOD_USED(con));

                 if(done==ROK)  
                 {
                   CC_STATE_CHANGE(con, CCS_AWT_PRECONDITION);
                   return ROK;
                 }
                 else
                 {
                   CCDP(AIN_DBG_LEVEL_1, "%s: FID 15068 Precondition has completed\n",__FUNCTION__);
                   SET_PRECND_COMPLETED(con->icPrecndCb->flag);
                   ccClearPrecndStatus(con->icPrecndCb);

                   if (CC_GET_PROTOCOL(con->ogProtType) == CC_EXT_BICC) /* FID 17373.0 */
                   {
                       CCDP(AIN_DBG_LEVEL_2,"%s: contChkInd: %d.\n", __FUNCTION__, con->tfcDsc.nbTfcDesc.contChkInd.val);
                       /* Send Bearer done to BICC layer to trigger COT sending out */
                       if (ccIsOgContChkRqstd(con) == CONTCHK_REQ)
                       {
                          CCDP(AIN_DBG_LEVEL_2,"%s: Need to send out COT for EXT BICC in precondition.\n", __FUNCTION__);

                          CcCnStEvnt biccBearerDoneCnst;
                          cmMemset((U8 *)&biccBearerDoneCnst, 0, sizeof(CcCnStEvnt));

                          con->ogEvntType = CC_ET_BICCBEARERDONE;

                          ccSendCnStReq(con, con->ogSapCb->suId, &con->ogSapCb->pst,
                              con->ogSapCb->spId, con->ogSpConnId, con->ogSuConnId,
                              con->ogProtType, con->ogEvntType, &biccBearerDoneCnst, NULL);

                          CC_STATE_CHANGE(con, CCS_AWTANS);
                       }
                   }
                 }
               }
               else if (!CC_OFFANS_ICOFFANS_PENDING(con) && (con->icPrecndCb) && !IS_PRECND_COMPLETED(con->icPrecndCb->flag))
               {
                 if((CC_IS_CALL_FLAG_SET(con->flag,CC_FLAG_IP_CALL_NO_INITIAL_SDP)) &&
                   ccHasPrecndConfStatus(con->icPrecndCb))
                 {
                   ccEnablePrecndCb(con->icPrecndCb);
                   ccTryToInsertPrecndConfStatus(con->icPrecndCb);
                   ccSendUpdate(&con->icBcm);
                   CC_OFFANS_STATE_CHANGE(con, CC_OFFANS_STATE_SENTOFFER,
                                         CC_OFFANS_STATE_NO_OP,
                                         CC_OFFANS_SUBSTATE_NULL,
                                         CC_OFFANS_TYPE_UPDATE);
                 }
                 CC_STATE_CHANGE(con, CCS_AWT_PRECONDITION);
              }

            }
            else if (CC_IS_CALL_FLAG2(con,CC_FLAG_PRECONDITION) &&
                (CC_GET_PROTOCOL(con->ogProtType) == CC_EXT_BICC) &&
                (ccIsOgContChkRqstd(con)==CONTCHK_REQ) &&
                 CC_IS_CALL_MULTI_MG(con)) 
            {
               con->ccmCb.flags |= CC_IW_TDM;
               con->direction = OUTTOINC;

               if (con->ccmCb.flags&CC_IW_TDM)
               {
                 if (!CC_OPERATING_LAST_MG(con))
                 {
                   if ((con->ccmCb.ccn == CCN_5) && (con->ccmCb.state == CC_EGRESS_MG))
                     con->ccmCb.state = CC_Y_MG;
                   else
                     con->ccmCb.state = CC_INGRESS_MG;

                   ccProcessXferRsp1OnSwitchPassThru(con, CC_GET_PROTOCOL(con->icProtType),
                   CC_GET_PROTOCOL(con->ogProtType), con->ccEvntHold);
                   CC_STATE_CHANGE(con, CCS_AWTOGSWTCFM);

                   return ROK;

                 }else {
                   con->ccmCb.flags &= ~CC_IW_TDM;
                   con->direction = INCTOOUT;
                 }
               }
            }
            else
            {
               CCDP(AIN_DBG_LEVEL_1, "%s: FID 17373 send BICCBEARERDONE to BICC.\n",__FUNCTION__);
               CC_STATE_CHANGE(con, CCS_AWTANS);

               /* Bug 82786 Tell OG BICC layer bearer setup done via CC_ET_BICCBEARERDONE */
               cmMemset((U8 *)&biccBearerDoneCnst, 0, sizeof(CcCnStEvnt));

               con->ogEvntType = CC_ET_BICCBEARERDONE;

               ccSendCnStReq(con, con->ogSapCb->suId, &con->ogSapCb->pst,
                             con->ogSapCb->spId, con->ogSpConnId, con->ogSuConnId,
                             con->ogProtType, con->ogEvntType, &biccBearerDoneCnst, NULL);
            }
        }
        else
        {
            CCDP(AIN_DBG_LEVEL_0, "%s:kind=%d,gbbncchange=%d,%ld,%ld,%ld GB/BNCCHANGE invalid\n", __FUNCTION__, kind, 
                  mgctTxnInd->u.notifyReq.observedEventsDesc.observedEventList->observedEventParameterList->u.eventOther.u.gbbncchange,
                  con->icSuConnId,
                  con->ogSuConnId, 
                  con->mgBlock->mgCtx->suCtxId);
            /* req: orig0023 */
            cause = CCPROTERR;
            ret = RFAILED;
        }
      }
    }
    if ( ret != ROK )
    {
        /* release call here */
        con->tempHolderForReleaseCause = cause;

        S16 mgret = RFAILED; 
        ccStartConTmr(TMR_RELEASE, con, (PTR)&ccCp.genCfg);
        ccStopConTmr(con, AIN_O_NO_ANS_TMR);
        ccStopConTmr(con, AIN_T_NO_ANS_TMR);
        ccStopConTmr(con, CC_TMR_EARLYACM); 
        ccDeleteBufferedEvent(con, con->icSuConnId, CCE_ONOANSEXP);
        ccDeleteBufferedEvent(con, con->ogSuConnId, CCE_TNOANSEXP);

        ccQSubBasedOnCcn(con);

        con->mgBlock->con = con;
        mgret = ccSendMgiQueue(0, con->mgBlock);
        if(mgret != ROK)
        { 
            CCLOGERR("ccHandleMgResponse or ccUpdateMgQForSubRsc or SendMgiQ. failed \n");
            cleanUpMgQ(con);
            ccCleanUpMGCtx(con);
            ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
        }
        else
        {
            if ( con->icExtBiccCb &&
                CC_IS_EXTBICC_BEARERSTATE(con,CC_INCOMING,CC_BICC_STATE_RCVD_IPBCPREQ))
            {
                con->relPend = ICRELONLY;
                CC_STATE_CHANGE(con, CCS_AWTSWTCFM_TOCLR);
            }
            else if(con->ogExtBiccCb && 
                   (CC_IS_EXTBICC_BEARERSTATE(con,CC_OUTGOING,CC_BICC_STATE_RCVD_CONFWD_NOTFY) ||
                    CC_IS_EXTBICC_BEARERSTATE(con,CC_OUTGOING,CC_BICC_STATE_RCVD_IPBCPACCEPT)))
            {
                /* release both way */
                CC_STATE_CHANGE(con, CCS_AWTSWTCFM_SENDBOTHREL);
            }
        } 
    }
    RETVALUE(ROK);
}

/* FID 15310.1 + */
/*
 *
 *      Fun:   ccConE94S00
 *
 *      Desc:  Connection state function
 *             event - CCE_ICA(Initiate Call Attempt From FIC)
 *             state - CCS_IDLE
 *
 *      Ret:   ROK     - successful,
 *             RFAILED - unsuccessful
 *
 *      File:  cc_bdy2_2.c
 *
 */
PUBLIC S16 ccConE94S00
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
    S16                ret                      = RFAILED;
    msgValEDPRequestEnum index                  = 0;
    Ica_t        *pInitCallAtt = NULLP;
    SvcHeader_t          *pSvcHeader;
    SvcHeader_t          header;
    InfoCollected_t      infoCollected;

    CCDP(AIN_DBG_LEVEL_1, "ccConE94S00:\n");

    pSvcHeader = (SvcHeader_t *) event;
    pInitCallAtt = (Ica_t *)event2;

    if (pSvcHeader->lastTransactionFlag == TRUE)
    {
        CCLOGERR("Error: lastTransaction is TRUE for InitCallAttempt");
        ccRelCon(con);
        RETVALUE(ROK);
    }

    /* Initialize the icBcm field in the CcConCb */
    ret = SGetSBuf(ccInit.region, ccInit.pool, (Data **) &(con->icBcm.cc), (Size) sizeof(AinCallConfig_t));
    if (ret != ROK)
    {
        ccGenAlarm(LCM_CATEGORY_RESOURCE, LCC_EVENT_MEMALOC_FAILED, LCC_CAUSE_MALLOC_FAIL, (U8 *) &ret);
        ccRelCon(con);
        RETVALUE(RFAILED);
    }
    cmMemset((U8 *) (con->icBcm.cc), 0, sizeof(AinCallConfig_t));

    if (ccInit.acnt == TRUE)
    {
        SwTimevalGet(&con->callDtlInfo.cm.timevalStart, SW_TIMEVAL_IcConIndRcvd, NULL);
        con->callDtlInfo.cm.tckIcConIndRcvd.pres = PRSNT_NODEF;
        (Void) SGetSysTime(&con->callDtlInfo.cm.tckIcConIndRcvd.val);
    }
    /* Allocate a false icIntfcCb for ICA calls */
    /* allocate control block */
    ret = SGetSBuf(ccInit.region, ccInit.pool, (Data **) &(con->icIntfcCb),
        (Size) sizeof(CcIntfcSAPCb));
    if (ret != ROK)
    {
      ccGenAlarm(LCM_CATEGORY_RESOURCE, LCC_EVENT_MEMALOC_FAILED, LCC_CAUSE_MALLOC_FAIL, (U8 *) &ret);
      ccRelCon(con);
      RETVALUE(RFAILED);
    }

    con->icIntfcCb->nmbActvConn++;
    con->icIntfcCb->protType = CC_ICA;
    con->icIntfcCb->intfc.intfType = CC_ICA_INTFC;

    con->icIntfcCb->ccTgCb = NULLP;
    
    /* Allocate a new CC transId for use between CC and FIC */
    con->icBcm.svcHeader.transId = ccGetNewtransId();
    CCDP(AIN_DBG_LEVEL_0," TransId= %ld\n", con->icBcm.svcHeader.transId);

    /* FillBCMInfo       */
    ret = ccFillBCMInfo(con, &(con->icBcm), con->icBcm.svcHeader.transId,
                        CiOriginatingSetup, PicCollectInformation, BtOBcm, 
                        DP_ORIGINATION_ATTEMPT);

    /* Update AinCallConfig */
    con->icBcm.cc->legId01 = & (con->icBcm);

    /* Insert in icBCM Hash table */
    ret = cmHashListInsert(&ccicBCMInsTbl, (PTR) &(con->icBcm), 
                            (U8 *)&(con->icBcm.svcHeader.transId), (U16)sizeof(UConnId));
    if (ret != ROK)
    {
        CCLOGERR(" ccicBCMInsTbl Insertion failed:%ld\n", con->icSuConnId);
        ccRelCon(con);
        RETVALUE(RFAILED);
    }

    /* Skip RM */
    con->icRscVal = FALSE;

    /* Start the SETUP Timer on con */
    ccStartConTmr(TMR_SETUP, con, (PTR)&ccCp.genCfg);

    /* Peg count for RBWF virtual call */
    ccCp.sts.totalCalls++;

    /* Send Info Collected message to FIC and continue normal call processing */
    /* Initializing the message buffers */
    cmMemset((U8 *)&header, 0, sizeof(SvcHeader_t));
    cmMemset((U8 *)&infoCollected, 0, sizeof(InfoCollected_t));

    /* Cleaning up the NEL */
    for (index = 0; index < EReqMax; index++)
    {
        con->icBcm.nel[index] = NEL_UNARMED;
    }

    /* Fill in parameters in the Service Header Structure */
    ccFillSvcHeader(&header, &(con->icBcm), MSG_Info_Collected);
    CCDP(AIN_DBG_LEVEL_0," Filled SvcHeader\n");

    /* Fill in parameters in the Info Collected Structure */
    ccFillInfoColltdParam(&infoCollected, &(con->icBcm));
    CCDP(AIN_DBG_LEVEL_1," Filled InfoCollected\n");

    CC_STATE_CHANGE(con, CCS_AWTRSPFORDPE3);

    CCDP(AIN_DBG_LEVEL_1,"%s: Sending InfoCollected\n",__FUNCTION__);
    CcUiFctInfoCollected(&infoCollected, &header);

    RETVALUE(ROK);
}
/*
 *
 *      Fun:   ccConE95S00
 *
 *      Desc:  Connection state function
 *             event - CCE_IACA(Initial Associate Call Attempt From FIC)
 *             state - CCS_IDLE
 *
 *      Ret:   ROK     - successful,
 *             RFAILED - unsuccessful
 *
 *      File:  cc_bdy2_2.c
 *
 */
PUBLIC S16 ccConE95S00
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{

  S16                   ret                   = RFAILED;
  CcConCb              *con2                  = NULLP;
  CcConCb              *con1                  = NULLP;
  AinBCM_t             *pBcm                  = NULLP;
  U8                    direction             = 0;
  msgValEDPRequestEnum  index                 = 0;
  InitAssocCallAtt_t   *pInitAssocCallAtt     = NULLP;
  SvcHeader_t          *pSvcHeader            = NULLP;
  SvcHeader_t           header;
  InfoCollected_t       infoCollected;

  if (event2 == NULLP)
  {
    /* con is verified to not be a NULLP on initial ccCallStateMachine */
    CCLOGERR("Critical ERROR: Null pointers: event2=%ld\n", event2);
    ABORT_DEBUG;
    RETVALUE(ret);
  }

  /* CCE_ICA Received on con2 */ 
  con2 = con;

  /* pSvcHeader needed for suTransid from FIC, spTransId is not valid  */
  pSvcHeader        = (SvcHeader_t *) event;
  pInitAssocCallAtt = (InitAssocCallAtt_t *) event2;

  /* Find con1 Connection Block based on assocTransId provided from FIC */
  /* Search the transaction Id in BCM Instance table */
  pBcm = ccFindBCMFromTransId(pInitAssocCallAtt->assocTransId, direction);
  if (pBcm == NULLP)
  {
      CCLOGERR("**%s:DID NOT FIND THE BCM, transId = %ld\n",
               __FUNCTION__, pInitAssocCallAtt->assocTransId);
      ABORT_DEBUG;
      ccRelCon(con2);
      ccHandleCleanUpConCb(con1,TRUE,CCTMPFAIL);
      RETVALUE(RFAILED);
  }
  
  if (pSvcHeader->lastTransactionFlag == TRUE)
  {
    CCLOGERR("%s: Error, lastTransaction is TRUE for InitAssocCallAtt\n",__FUNCTION__);
    ABORT_DEBUG;
    ccRelCon(con2);
    ccHandleCleanUpConCb(con1,TRUE,CCTMPFAIL);
    RETVALUE(RFAILED);
  }

  con1=pBcm->con;

  CCDP(AIN_DBG_LEVEL_0, "%s: con1 - icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", __FUNCTION__,con1->icSuConnId, con1->ogSuConnId,
       (con1->mgBlock->mgCtx ? con1->mgBlock->mgCtx->suCtxId : 0), con1->icBcm.svcHeader.transId);

  /* As at this time con1's state is still waiting for rsp for O_Answer since we receive IACA change state back */
  ccRestoreCallStateOfAssociatedCalls(&(con1->icBcm));

  /*Set the cc for con1 and con2 */
  if(!con1->icBcm.cc)
  {
    /* Get memory for con1->icBcm's cc */
    /* Initialize the icBcm field in the CcConCb */
    ret = SGetSBuf(ccInit.region, ccInit.pool, (Data **) &(con1->icBcm.cc), (Size) sizeof(AinCallConfig_t));
    if (ret != ROK)
    {
      ccGenAlarm(LCM_CATEGORY_RESOURCE, LCC_EVENT_MEMALOC_FAILED, LCC_CAUSE_MALLOC_FAIL, (U8 *) &ret);
      ccHandleCleanUpConCb(con1,TRUE,CCTMPFAIL);
      ccRelCon(con2);
      RETVALUE(RFAILED);
    }
    cmMemset((U8 *) (con1->icBcm.cc), 0, sizeof(AinCallConfig_t));
  }

  /* Set the cc thing for both con1 and con2 */
  con1->icBcm.cc->legId01 = &(con2->icBcm);
  con1->icBcm.cc->legId2  = &(con2->ogBcm);
  con1->icBcm.cc->legId02 = &(con1->icBcm);
  con1->icBcm.cc->legId1  = &(con1->ogBcm);
  con1->icBcm.cc->ccId    = CiRemoteTransferSetup;
  
  con2->icBcm.cc = con1->icBcm.cc;

  if (ccInit.acnt == TRUE)
  {
    SwTimevalGet(&con2->callDtlInfo.cm.timevalStart, SW_TIMEVAL_IcConIndRcvd, NULL);
    con2->callDtlInfo.cm.tckIcConIndRcvd.pres = PRSNT_NODEF;
    (Void) SGetSysTime(&con2->callDtlInfo.cm.tckIcConIndRcvd.val);
  }

  /* Allocate a new CC transId for use between CC and FIC */
  con2->icBcm.svcHeader.transId = ccGetNewtransId();
  CCDP(AIN_DBG_LEVEL_0," TransId= %ld\n", con2->icBcm.svcHeader.transId);

  /* FillBCMInfo       */
  ret = ccFillBCMInfo(con2, &(con2->icBcm), con->icBcm.svcHeader.transId,
      CiRemoteTransferSetup, PicCollectInformation, BtOBcm, 
      DP_ORIGINATION_ATTEMPT);

  /* Update AinCallConfig */
  con2->icBcm.cc->legId01 = & (con2->icBcm);

  /* Insert in icBCM Hash table */
  ret = cmHashListInsert(&ccicBCMInsTbl, (PTR) &(con2->icBcm), 
      (U8 *)&(con2->icBcm.svcHeader.transId), (U16)sizeof(UConnId));
  if (ret != ROK)
  {
    CCLOGERR(" ccicBCMInsTbl Insertion failed:%ld\n", con->icSuConnId);
    ccRelCon(con2);
    ccHandleCleanUpConCb(con1,TRUE,CCTMPFAIL);
    RETVALUE(RFAILED);
  }

  /* Allocate a false icIntfcCb for ICA calls */
  ret = SGetSBuf(ccInit.region, ccInit.pool, (Data **) &(con2->icIntfcCb),
      (Size) sizeof(CcIntfcSAPCb));
  if (ret != ROK)
  {
    ccGenAlarm(LCM_CATEGORY_RESOURCE, LCC_EVENT_MEMALOC_FAILED, LCC_CAUSE_MALLOC_FAIL, (U8 *) &ret);
    ccRelCon(con2);
    ccHandleCleanUpConCb(con1,TRUE,CCTMPFAIL);
    RETVALUE(RFAILED);
  }

  con2->icIntfcCb->nmbActvConn++;
  con2->icIntfcCb->protType = CC_ICA;
  con2->icIntfcCb->ccTgCb   = NULLP;
  con2->icProtType          = CC_ICA;
  con2->icIntfcCb->intfc.intfType = CC_ICA_INTFC;

  /* Need to send out the SuccessOutCome back to service layer */
  if (con2->icBcm.nel[EReqSuccessOutCome] == NEL_NOTIFY)
  {
    /* Send Success OutCome on icBcm */
    CCDP(AIN_DBG_LEVEL_0, "%s: Sending Success OutCome:%ld\n", __FUNCTION__,con2->icSuConnId);
    con2->icBcm.nel[EReqSuccessOutCome] = NEL_UNARMED;

    SvcHeader_t    header;
    SuccessOutcome_t successOutCome;

    cmMemset((U8 *)&header, 0, sizeof(SvcHeader_t));
    cmMemset((U8 *)&successOutCome, 0, sizeof(SuccessOutcome_t));


    /* Update bcm parameters */
    ccUpdateBcm(con2, con2->icBcm.pic, con2->icBcm.cc->ccId, con2->icBcm.bcmType);

    /* Fill in parameters in the Service Header Structure */
    ccFillSvcHeader(&header, &con2->icBcm, MSG_Success_Outcome);
    ccFillSuccessOutCome(&successOutCome, &con2->icBcm);
    successOutCome.notInd = TRUE;
    CcUiFctSuccessOutcome(&successOutCome, &header);
    
  }
  /* Skip RM */
  con2->icRscVal = FALSE;

  /* Start the SETUP Timer on con */
  ccStartConTmr(TMR_SETUP, con2, (PTR)&ccCp.genCfg);

  /* Peg count for RBWF virtual call */
  ccCp.sts.totalCalls++;

  /* Send Info Collected message to FIC and continue normal call processing */
  /* Initializing the message buffers */
  cmMemset((U8 *)&header, 0, sizeof(SvcHeader_t));
  cmMemset((U8 *)&infoCollected, 0, sizeof(InfoCollected_t));

  /* Cleaning up the NEL */
  for (index = 0; index < EReqMax; index++)
  {
    con2->icBcm.nel[index] = NEL_UNARMED;
  }

  /* Fill in parameters in the Service Header Structure */
  ccFillSvcHeader(&header, &(con2->icBcm), MSG_Info_Collected);
  CCDP(AIN_DBG_LEVEL_0, " Filled SvcHeader\n");

  /* Fill in parameters in the Info Collected Structure */
  ccFillInfoColltdParam(&infoCollected, &(con2->icBcm));
  CCDP(AIN_DBG_LEVEL_1," Filled InfoCollected\n");

  CC_STATE_CHANGE(con2, CCS_AWTRSPFORDPE3);

  /* Change con1 state  */
  ccChangeCallStateOfAssociatedCalls(&(con2->icBcm),
                                     CCS_CANNOTPROCESSEVENTS);

  CCDP(AIN_DBG_LEVEL_1,"%s: Sending InfoCollected\n",__FUNCTION__);
  CcUiFctInfoCollected(&infoCollected, &header);

  RETVALUE(ROK);
}
/* FID 15310.1 - */

/* + FID 15801.0 */
/*
 *
 *      Fun:   ccConE37S149
 *
 *      Desc:  Connection state function
 *             event - AnalyzeRoute
 *             state - CCS_AWTOREDIRECT_RSP 
 *      Ret:   ROK     - successful,
 *             RFAILED - unsuccessful
 *
 *      Notes: None.
 *
 *      File:  cc_bdy2_2.c
 *
 */
PUBLIC S16 ccConE37S149
(
 CcConCb *con,                    /* connection */
 PTR     event,                   /* event */
 AinBCM_t *bcm,                    /* bcm */
 PTR     event2                    /* event */
 )
{
  SvcHeader_t        *arSvcHeader           = NULLP;
  AnalyzeRoute_t     *pAnalyzeRoute         = NULLP;
  S16                 mgret                 = RFAILED;

  if ((con == NULLP) || (event == NULLP))
  {
    CCLOGERR("Critical ERROR: Null pointers:%lx, %ld\n", (U32)con, event);
    RETVALUE(RFAILED);
  }

  CCDP(AIN_DBG_LEVEL_1, "%s():: icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", __FUNCTION__, con->icSuConnId, con->ogSuConnId,
       (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0), con->icBcm.svcHeader.transId);

  pAnalyzeRoute = (AnalyzeRoute_t *) event2;
  arSvcHeader = (SvcHeader_t *) event;

  /* LGP-68 story changes */
  if (con->sip3xxCb && con->sip3xxCb->currIndex)
  {
    /* CDR should be cut only when SIP 3xx URI call fails */
    pAnalyzeRoute->scpResponse = TRUE;
  }

  ccCutBillingRecordIfNeeded(con, pAnalyzeRoute->scpResponse, OUTTOINC);
  ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));

  ccRestoreEvntHold(con);
  ccExtractAnalyzeRteParam(pAnalyzeRoute, bcm);
  ccExtractSvcHeader(arSvcHeader, bcm, MSG_Analyze_Route);

  /* Populate the CDR info for SIP3xx call */
  CC_POPULATE_CDR_FOR_SIP3XX(con);

  ccStopConTmr(con, AIN_O_NO_ANS_TMR);
  ccStopConTmr(con, AIN_T_NO_ANS_TMR);
  /* STOP the AWAIT ANSWER Timer if running
   *
   * changes made to Support Configurable Answer Timer in GCC, RV Suresh, 5.0 
   */
  ccStopConTmr(con, CC_TMR_AWAITANS);
  ccDeleteBufferedEvent(con, con->icSuConnId, CCE_ONOANSEXP);
  ccDeleteBufferedEvent(con, con->ogSuConnId, CCE_TNOANSEXP);

  ccCleanAnsFraudCall(con);

  ccRestoreReleaseInEvntHold(con);

  if (arSvcHeader->lastTransactionFlag == TRUE)
    ccDeleteBcm(&(con->icBcm));

  con->ogBcm.closeCause = CscRouteNotAvail;
  ccCloseTBcmTransaction(con);
  if ((CC_IS_CONTEXT_PRESENT(con)) && (con->ogRscVal))
  {
    ccQueueSubOnOGCleanUp(con);
    ccChangeCallStateOfAssociatedCalls(&(con->icBcm), CCS_CANNOTPROCESSEVENTS);
    /* CPDI_FIX: need to decide whether received an RelInd from outgoing before, or
       it's a case need initiate a release internally */
    if (CC_IS_REL_IN_EVNT_HOLD(con))
       con->relPend = OUTRELRSPANDROUTE;
    else
    {
       if (con->ccRelEvntHold)
          con->relPend = OUTRELRSPANDROUTE;
       else
       {
          /* put default cause value here */
          con->tempHolderForReleaseCause = AM_CV_NORMCALLCLR;
          con->relPend = OUTRELANDROUTE;
       }
    }
    CCDP(AIN_DBG_LEVEL_1, "%s: icSu:%ld, ogSu:%ld, suCtxId:%ld,"
         " con->relPend:%d\n", __FUNCTION__, con->icSuConnId, con->ogSuConnId,
         con->mgBlock->mgCtx->suCtxId, con->relPend);

    CC_STATE_CHANGE(con, CCS_AWTSWTCFMFORDP);
    con->mgBlock->con = con;
    mgret = ccSendMgiQueue(0, con->mgBlock);
    if (mgret == ROK)
      RETVALUE(ROK);
    else
    {
      CCLOGERR("Error: MGI failed:icSu:%ld, ogSu:%ld, suCtxId:%ld,"
               " icTransId:%ld\n", con->icSuConnId, con->ogSuConnId,
               con->mgBlock->mgCtx->suCtxId, con->icBcm.svcHeader.transId);
      ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
      if (con->ogRscVal)
      {
        ccCloseTBcmTransaction(con);
        con->direction = OUTTOINC;
        if (con->noRspReqd == FALSE) ccSendReleaseRsp(con);
        ccDeallocateResource(con, CC_OUTGOING);
        con->ogRscVal = FALSE;
      }
      cleanUpMgQ(con);
      ccHandleCleanUpConCb(con, TRUE, AM_CV_TMPFAIL);
      RETVALUE(ROK);
    }
  }

  /* Clean outgoing and ReRoute call */
  if (con->ogRscVal)
  {
    ccCloseTBcmTransaction(con);
    con->direction = OUTTOINC;
    if (con->noRspReqd == FALSE) ccSendReleaseRsp(con);
    ccDeallocateResource(con, CC_OUTGOING);
    con->ogRscVal = FALSE;
  }

  /* Route the call */
  initOgBcm(&con->ogBcm);
  ccRouteCall(con, FALSE);
  RETVALUE(ROK);
} /* End of ccConE37S149 */


/*
 *
 *      Fun:   ccConE39S149
 *
 *      Desc:  Connection state function
 *             event - Continue
 *             state - CCS_AWTOREDIRECT_RSP 
 *      Ret:   ROK     - successful,
 *             RFAILED - unsuccessful
 *
 *      Notes: None.
 *
 *      File:  cc_bdy2_2.c
 *
 */
PUBLIC S16 ccConE39S149
(
 CcConCb *con,                    /* connection */
 PTR     event,                   /* event */
 AinBCM_t *bcm,                    /* bcm */
 PTR     event2                    /* event */
 )
{
  SvcHeader_t        *ctSvcHeader           = NULLP;
  S16                 mgret         = RFAILED, relRstRet = ROK;
  Continue_t        *pContinue              = NULLP;
  CcRelEvnt           ogEvnt;
  ProtType            icProtType    = 0;
  CcCause             cause;
  TknU16              sipCause;
  TknU16              sipStatus;
  S16                 ret           = RFAILED;

  /* bug 91532 */
  cmMemset((U8 *)&ogEvnt, 0, sizeof(ogEvnt));
  
  if ((con == NULLP) || (event == NULLP))
  {
    CCLOGERR("Critical ERROR: Null pointers:%lx, %ld\n", (U32)con, event);
    RETVALUE(RFAILED);
  }
  /*CID 15593*/
  icProtType    = CC_GET_PROTOCOL(con->icProtType);
  CCDP(AIN_DBG_LEVEL_1, "%s():::icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", __FUNCTION__, con->icSuConnId, con->ogSuConnId,
       (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0), con->icBcm.svcHeader.transId);

  ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
  ccStopConTmr(con, AIN_O_NO_ANS_TMR);
  ccStopConTmr(con, AIN_T_NO_ANS_TMR);
  /* STOP the AWAIT ANSWER Timer if running
   *
   * changes made to Support Configurable Answer Timer in GCC, RV Suresh, 5.0 
   */
  ccStopConTmr(con, CC_TMR_AWAITANS);
  ccStopConTmr(con, TMR_SETUP);
  ccDeleteBufferedEvent(con, con->icSuConnId, CCE_ONOANSEXP);
  ccDeleteBufferedEvent(con, con->ogSuConnId, CCE_TNOANSEXP);
#if 1 /* EARLY_ACM */
  ccStopConTmr(con, CC_TMR_EARLYACM);
#endif
  CC_POPULATE_CDR_FOR_SIP3XX(con);

  /* CPDI_FIX 28351, 28730, need good way handling relrsp */
  relRstRet = ccRestoreReleaseInEvntHold(con);


  ctSvcHeader = (SvcHeader_t *) event;
  pContinue   = (Continue_t *) event2;
  /* Bug# 27816 */
  if(pContinue->errorCode != FC_NOT_APPLICABLE)
  {
    CCDP(AIN_DBG_LEVEL_1, "ERROR FROM FIC = %ld\n", pContinue->errorCode);
    con->tempHolderForReleaseCause = pContinue->errorCode;
    if  (con->icProtType == CC_LOCAL_SWT_PROT)
      ccRemoveCaleaAssnWithTappedBcm(con);

    con->ainInfo.termInd = EXCEPTION_IND;
    con->ogBcm.closeCause = CscRouteNotAvail;
    ccCloseTBcmTransaction(con);

    con->failCndVal = pContinue->errorCode;
    if (CC_IS_CONTEXT_PRESENT(con))
    {
#if 1 /* EARLY_ACM */
      /* Need to clean all the legs in the context */
      CC_RESET_ANY_EARLY_ACM_FLAGS(con);
#endif
      ccQueueSubOnOGCleanUp(con);
      /* 28730 handle it differently */
      if (relRstRet == ROK)
      {
         /* FID 15801 (LGP74) */
         if (CC_IS_SIP3XXCALL(con) && CC_IS_SIP3XXFAILCND(pContinue->errorCode))  
         {
           con->relPend = OUTRELRSPANDADVANCEURI; 
         }
         else
         {
           con->relPend = OUTRELRSPANDINFAILCOND;
         }
      }
      else
      {
         if (CC_IS_SIP3XXCALL(con) && CC_IS_SIP3XXFAILCND(pContinue->errorCode))
         {
           con->relPend = OUTRELANDADVANCEURI; 
         }
         else
         {
           con->relPend = OUTRELANDINFAILCOND;
         }
      }

      CC_STATE_CHANGE(con, CCS_AWTSWTCFM_SENDONEREL);
      con->mgBlock->con = con;
      mgret = ccSendMgiQueue(0, con->mgBlock);
      if (mgret == ROK)
        RETVALUE(ROK);
      else
      {
        CCLOGERR("Error: MGI failed:icSu:%ld, ogSu:%ld, suCtxId:%ld,"
                 " icTransId:%ld\n", con->icSuConnId, con->ogSuConnId,
                 con->mgBlock->mgCtx->suCtxId, con->icBcm.svcHeader.transId);
        cleanUpMgQ(con);
      }
    }

    /* + FID 15801 (LGP74) */
    if (CC_IS_SIP3XXCALL(con)) 
    {
      if (CC_IS_SIP_CONTACTURI_AVAIL(con) && CC_IS_SIP3XXFAILCND(pContinue->errorCode))
      {
        CC_ADVANCE_CONTACT_URI_LIST(con, ret);
        
        if (ret == ROK)
        {
           CCDP(AIN_DBG_LEVEL_0,"Sending ORedirect after advancing to the next Contact URI"
               "icSu:%ld, ogSu:%ld, icTsId:%ld,currIndex=%d\n", con->icSuConnId, con->ogSuConnId,
               con->icBcm.svcHeader.transId, con->sip3xxCb->currIndex);
           RETVALUE(ROK);
        }
      }
      else if (CC_IS_SIP3XXCALL(con))
      {
        /* LGP-233:: Need to restore the original contents before SIP 3xx redirect as it 
           is needed for reroute due to default failCndId processing */
        ccRestoreEvntHold(con);
        ccRestoreOrigParamFor3xxCall(con);
      }

      /* We are done with all the Contact URIs, release the SIP3XXBLOCK */
      CC_FREE_SIP3XX_BLOCK(con);
    }
    /* - FID 15801 (LGP74) */

    if (con->ogRscVal)
    {
      con->direction = OUTTOINC;
      if (relRstRet == ROK)
      {
         ccSendReleaseRsp(con);
         ccDeallocateResource(con, CC_OUTGOING);
         con->ogRscVal = FALSE;
      }
      else /* 28730: there is nothing to respond, just request */
      {
        ccFreeTheRscFromCon(con, OUTTOINC); /* This will take care of REL_CFM */
        ccProcessFailCnd(con, con->failCndVal, TRUE);
        RETVALUE(ROK);
      }
    }
    ccProcessFailCnd(con, con->failCndVal, TRUE);
  }
  else
  { /* FIC asks GCC to go ahead with release using the stored relEvnt */
    con->icEvnt = (CcAllSdus *) con->ccEvntHold;
    con->ogEvnt = (CcAllSdus *) &ogEvnt;
    /* 28351, 28730: balancing the conflicts */
    if (relRstRet != ROK)
    {
       /* there is no real event received, the following failcnd check make no sense */
       ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
       RETVALUE(ROK);
    }
    /* Get protocol release cause */
    ccGetRelCause (con, (con->direction == OUTTOINC?con->ogProtType:con->icProtType),
                   (CcRelEvnt *)con->icEvnt, &cause, &sipCause, &sipStatus);

    if (con->mgBlock->mgCtx && (con->mgBlock->mgCtx->suCtxId != 0))
    {
      ccQSubBasedOnCcn(con);  /* FID 15261.0 */

      if ((con->direction == OUTTOINC) && ((con->reconnectBcm == OBCM)||(con->reconnectBcm == TBCM)) )
      {
        con->relPend = OUTRELRSPANDINREL;
      } 
      else if ((con->direction == OUTTOINC) &&
          ((ccIsLEGSubscriber(&(con->icBcm))) || ((icProtType == CC_CS_TG) &&
          ((CC_CAUSE_IS_VAL(&cause, CCNORMUNSPEC, CSTD_CCITT) == FALSE) &&
          (CC_CAUSE_IS_VAL(&cause, CCCALLCLR, CSTD_CCITT) == FALSE)))))
      {   
        con->relPend = OUTRELRSPANDINTONE;
      }
      else if (con->direction == OUTTOINC)
      {
        con->relPend = OUTRELRSPANDINREL;
      }
      else
      {
        con->relPend = INRELRSPANDOUTREL;
      }

      CC_STATE_CHANGE(con, CCS_AWTSWTCFM_SENDONEREL);
      con->mgBlock->con = con;
      ccStopConTmr(con, TMR_SETUP);
      ccStartConTmr(TMR_RELEASE, con, (PTR)&ccCp.genCfg);
      ret = ccSendMgiQueue(0, con->mgBlock);
      if (ret != ROK)
      {
        CCLOGERR( "Error: ccSendMgiQueue failed\n");
      }
      else
        RETVALUE(ROK);
    }
    else
    {
      CCDP(AIN_DBG_LEVEL_0, "No Switching Ctx found:%d\n", con->ogProtType);
#ifdef CC_CALEA
      if (con->icProtType == CC_LOCAL_SWT_PROT)
      { 
        if ((con->icBcm.cc != NULLP) && (con->icBcm.cc->tappedLegId != NULLP))
        {
          CCDP(AIN_DBG_LEVEL_0,"Deswitching of CALEA  in Tapped con\n");
          ccProcessTappedLegForCaleaRel(con, con->icBcm.cc->tappedLegId, 
                                        event, CCE_RELIND, sizeof(CcRelEvnt));
          RETVALUE(ROK);
        }
        else
          CCLOGERR( "ERROR: con->ogBcm.cc is NULLP \n");
      }
#endif
    }
    if (con->noRspReqd == FALSE)
      ccSendReleaseRsp(con);

    if (con->direction == OUTTOINC)
    {
        con->failCndVal = ccMapProtRel2FailCnd (con, &cause, &sipCause,
                                                &sipStatus);
    }

    if (con->direction == INCTOOUT)
    {
      /* Deallocate the IC Resources */
      CC_STATE_CHANGE(con, CCS_AWTRELCFM_OG);
#ifdef GUARD_TIMER
      ccGuardOrDeallocResource(con, CC_INCOMING);
#else
          ccDeallocateResource(con, CC_INCOMING);
#endif
          con->icRscVal = FALSE;
    }
    else if (con->direction == OUTTOINC)
    {
      CC_STATE_CHANGE(con, CCS_AWTRELCFM_IC);
#ifdef GUARD_TIMER
      ccGuardOrDeallocResource(con, CC_OUTGOING);
#else
      ccDeallocateResource(con, CC_OUTGOING);
#endif
      con->ogRscVal = FALSE;
    }
    /* if required, PLAY BUSY Tone or REORDER Tone */
    if (con->direction == OUTTOINC)
    {
        ccStopConTmr(con, TMR_RELEASE);
        ccStartConTmr(TMR_SETUP, con, (PTR)&ccCp.genCfg);
        ret = ccProcessFailCnd (con, con->failCndVal, FALSE);
        if (ret == ROK)
            RETVALUE (ROK);
    }

    ccStopConTmr(con, TMR_SETUP);
    ccStartConTmr(TMR_RELEASE, con, (PTR)&ccCp.genCfg);

    ret = ccMapEvent(con, CCE_RELIND, 0);
    if (ret == ROK)
        ccSendReleaseReq(con);
    else if (con->direction == INCTOOUT)
    {

        ccSendRelease(con, con->ogSapCb, con->ogSpConnId, con->ogSuConnId,
                      con->ogProtType, CCPROTERR,NULL);
    }
    else if (con->direction == OUTTOINC)
    {
        
        ccSendRelease(con, con->ogSapCb, con->ogSpConnId, con->ogSuConnId,
                      con->ogProtType, CCPROTERR,NULL);
    }
  }

  RETVALUE(ROK);
} /* End of ccConE39S149 */

/*
 *
 *      Fun:   ccConE47S149
 *
 *      Desc:  Connection state function
 *             event - Send To Resource
 *             state - CCS_AWTOREDIRECT_RSP 
 *      Ret:   ROK     - successful,
 *             RFAILED - unsuccessful
 *
 *      Notes: None.
 *
 *      File:  cc_bdy2_2.c
 *
 */
PUBLIC S16 ccConE47S149
(
 CcConCb *con,                    /* connection */
 PTR     event,                   /* event */
 AinBCM_t *bcm,                    /* bcm */
 PTR     event2                    /* event */
 )
{
  SvcHeader_t       *srSvcHeader           = NULLP;
  S16                ret                   = RFAILED;
  S16                mgret                 = RFAILED;
  
  if ((con == NULLP) || (event == NULLP))
  {
    CCLOGERR("Critical ERROR: Null pointers:%lx, %ld\n", (U32)con, event);
    RETVALUE(RFAILED);
  } 
  CCDP(AIN_DBG_LEVEL_1, "%s():icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", __FUNCTION__, con->icSuConnId, con->ogSuConnId,
       (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0), con->icBcm.svcHeader.transId);

  ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
  srSvcHeader = (SvcHeader_t *) event;

  ccStopConTmr(con, AIN_O_NO_ANS_TMR);
  ccStopConTmr(con, AIN_T_NO_ANS_TMR);
  /* STOP the AWAIT ANSWER Timer if running
   *
   * changes made to Support Configurable Answer Timer in GCC, RV Suresh, 5.0 
   */
  ccStopConTmr(con, CC_TMR_AWAITANS);
  ccDeleteBufferedEvent(con, con->icSuConnId, CCE_ONOANSEXP);
  ccDeleteBufferedEvent(con, con->ogSuConnId, CCE_TNOANSEXP);
#if 1 /* EARLY_ACM */
  ccStopConTmr(con, CC_TMR_EARLYACM);
#endif

  CC_POPULATE_CDR_FOR_SIP3XX(con);

  ccRestoreReleaseInEvntHold(con);

  if (srSvcHeader->lastTransactionFlag == TRUE)
    ccDeleteBcm(&(con->icBcm));

  con->ogBcm.closeCause = CscRouteNotAvail;
  ccCloseTBcmTransaction(con);

  con->lastState = con->state;
  con->anncBlock.stateForAnnc = con->state;

  if (CC_IS_CONTEXT_PRESENT(con))
  {
#if 1 /* EARLY_ACM */
    /* Need to clean all the legs in the context */
    CC_RESET_ANY_EARLY_ACM_FLAGS(con);
#endif

    ccQueueSubOnOGCleanUp(con);
    ccChangeCallStateOfAssociatedCalls(&(con->icBcm), CCS_CANNOTPROCESSEVENTS);
    /* CPDI_FIX: need to decide whether received an RelInd from outgoing before, or
       it's a case need initiate a release internally */
    if (CC_IS_REL_IN_EVNT_HOLD(con))
       con->relPend = OUTRELRSPANDPLAYANNC;
    else
    {
       if (con->ccRelEvntHold)
    con->relPend = OUTRELRSPANDPLAYANNC;
       else
       {
          /* put default cause value here */
          con->tempHolderForReleaseCause = AM_CV_NORMCALLCLR;
          con->relPend = OUTRELANDPLAYANNC;;
       }
    }
    CC_STATE_CHANGE(con, CCS_AWTSWTCFMFORDP);
    con->mgBlock->con = con;
    mgret = ccSendMgiQueue(0, con->mgBlock);
    if (mgret == ROK)
      RETVALUE(ROK);
    else
    {
      CCLOGERR("Error: MGI failed:icSu:%ld, ogSu:%ld, suCtxId:%ld,"
               " icTransId:%ld\n", con->icSuConnId, con->ogSuConnId,
               con->mgBlock->mgCtx->suCtxId, con->icBcm.svcHeader.transId);
      ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
      con->direction = OUTTOINC;
      if (con->noRspReqd == FALSE) ccSendReleaseRsp(con);
      ccDeallocateResource(con, CC_OUTGOING);
      con->ogRscVal = FALSE;
      cleanUpMgQ(con);
      ccHandleCleanUpConCb(con, TRUE, AM_CV_TMPFAIL);
      RETVALUE(ROK);
    }
  }

  con->direction = OUTTOINC;
  if (con->noRspReqd == FALSE) ccSendReleaseRsp(con);
  ccDeallocateResource(con, CC_OUTGOING);
  con->ogRscVal = FALSE;

  initOgBcm(&con->ogBcm);
  ret = ccSendToResource(&(con->icBcm));
  if (ret != ROK)
  {
    CCLOGERR("Error: ccSendToResource failed:icSu:%ld, ogSu:%ld, icTrsId:%ld\n",
             con->icSuConnId, con->ogSuConnId, (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0));
    ccHandleCleanUpConCb(con, TRUE, AM_CV_TMPFAIL);
  }

  RETVALUE(ROK);
} /* End of ccConE47S149 */

/*
 *
 *      Fun:   ccConE52S149
 *
 *      Desc:  Connection state function
 *             event - Disconnect 
 *             state - CCS_AWTOREDIRECT_RSP 
 *      Ret:   ROK     - successful,
 *             RFAILED - unsuccessful
 *
 *      Notes: None.
 *
 *      File:  cc_bdy2_2.c
 *
 */
PUBLIC S16 ccConE52S149
(
 CcConCb *con,                    /* connection */
 PTR     event,                   /* event */
 AinBCM_t *bcm,                    /* bcm */
 PTR     event2                    /* event */
 )
{
  if ((con == NULLP) || (event == NULLP))
  {
    CCLOGERR("Critical ERROR: Null pointers:%lx, %ld\n", (U32)con, event);
    RETVALUE(RFAILED);
  }

  CCDP(AIN_DBG_LEVEL_1, "%s()::icSu:%ld, ogSu:%ld, suCtxId:%ld,"
       " icTransId:%ld\n", __FUNCTION__, con->icSuConnId, con->ogSuConnId,
       (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0), con->icBcm.svcHeader.transId);
    
  con->ogBcm.closeCause = CscRouteNotAvail;
  ccCloseTBcmTransaction(con);

  CC_POPULATE_CDR_FOR_SIP3XX(con);
  
  ccRestoreReleaseInEvntHold(con);
  ccDisconnectCallForIcBCM(con, event);

  RETVALUE(ROK);
} /* End of ccConE52S149 */

/*
*   
*      Fun:   ccProcessORedirect
*     
*      Desc:  This function checks if there is an ORedirect event
*             armed.
*             If Armed sends a message to FIC.
*   
*      Ret:   ROK       - successful, Event detected and Message sent to FIC.
*             RFAILED   - failed
*             ROKDNA    - No event was armed
*      Notes: None.
*
*      File:  cc_bdy2.c
*
*/
PUBLIC S16 ccProcessORedirect
(
AinBCM_t       *pBcm
)
{
  CcConCb            *con         = NULLP;
  U8                 index        = 0;
  ORedirect_t        oRedirect; 
  SvcHeader_t        header;
  s16                ret          = RFAILED; 

  if (pBcm == NULL)
  {
    CCLOGERR("%s():: ERROR!! pBcm is NULL\n",__FUNCTION__); 
    RETVALUE(RFAILED);
  }
  
  con = pBcm->con;

  if (con == NULL)
  {
    CCLOGERR("%s():: ERROR!! con is NULL\n",__FUNCTION__); 
    RETVALUE(RFAILED);
  }

  if (con->sip3xxCb == NULL)
  {
    CCLOGERR("%s():: ERROR!! con->sip3xxCb is NULL\n",__FUNCTION__); 
    RETVALUE(RFAILED);
  }

  CCDP(AIN_DBG_LEVEL_1, "ccProcessORedirect:%ld, %ld\n",
                         con->icSuConnId, con->ogSuConnId);

  /* LGP-78 */
  if (con && con->sip3xxCb && (con->sip3xxCb->sip3xxRedirCnt == MAX_SIP3XX_REDIR))
  {
    /* This is for handling the corner case where the 15th redirection resulted
       in SIP 3XX */
    /* The calling function will process the default release procedure */
    RETVALUE(RFAILED);

  }

  if (pBcm->svcHeader.transId)
  {
    CCDP(AIN_DBG_LEVEL_0, "ORedirect is Armed Req Evnt:%ld\n",
                           con->icSuConnId);
    ccRestoreEvntHold(con);

    cmMemset((U8 *)&header, 0, sizeof(SvcHeader_t));
    cmMemset((U8 *)&oRedirect, 0, sizeof(ORedirect_t));

    /* Cleaning up the NEL as per R5-241 GR1298 */
    for (index = 0; index < EReqMax; index++)
      pBcm->nel[index] = NEL_UNARMED;

    /* Backing up ccEvntHold and ainInfo for URI processing */
    if ((pBcm->con) && (pBcm->con->sip3xxCb))
    {
      ccStoreOrigParamFor3xxCall(pBcm->con);
    } 

    CCDP(AIN_DBG_LEVEL_0," TransId for OBCM = %ld\n", pBcm->svcHeader.transId);
    ccFillSvcHeader(&header, pBcm, MSG_O_Redirect);
    CCDP(AIN_DBG_LEVEL_0," Filled SvcHeader\n");

    /* LGP-78 & LGP-384 changes */
    con->sip3xxCb->sip3xxRedirCnt++; /* This count is for individual SIP 3xx call */
    ccCp.sts.ccSip3xxSts.noOfSip3xxURIProcessed++; /* Count for the system */

    ret = ccFillORedirectParam(&oRedirect, pBcm);
    if (ret != ROK)
    {
      CCLOGERR("ERROR: ccFillOORedirectParam failed: %ld\n", con->icSuConnId);
      RETVALUE(RFAILED);
    }
    CCDP(AIN_DBG_LEVEL_0," Filled ORedirect,TransID= %ld\n", header.spTransId);

    con->lastState = con->state;
    CC_STATE_CHANGE (con, CCS_AWTOREDIRECT_RSP);
    ccChangeCallStateOfAssociatedCalls(&(con->icBcm), CCS_CANNOTPROCESSEVENTS);

    /* bug: 94143 */
    if(con->tempAuthTerm != NULLP)
    { 
       TknStr4      tmpHomeCc;    
       CCDP(AIN_DBG_LEVEL_1, "Free tempAuthTerm\n");  
       if (con->tempAuthTerm->tempBuf.pres == PRSNT_NODEF)
       { 
          ccDropUBuf((Buffer **)&con->tempAuthTerm->tempBuf.val);    
          con->tempAuthTerm->tempBuf.pres = NOTPRSNT; 
       }
       /* reset con->tempAuthTerm but keep homeCc */
       tmpHomeCc = con->tempAuthTerm->homeCc;
       cmMemset ((U8 *)con->tempAuthTerm, 0, sizeof(CcTempAuthTerm));
       con->tempAuthTerm->homeCc = tmpHomeCc;
    }

    CCDP(AIN_DBG_LEVEL_0," Sending ORedirect: %ld\n", con->icSuConnId);
    CcUiFctORedirect(&oRedirect, &header);
    RETVALUE(ROK);
  }

  CCDP(AIN_DBG_LEVEL_0, "ORedirect Event not armed:%ld\n", con->icSuConnId);
  RETVALUE(ROKDNA);
}

/*
*
*      Fun:   ccConE14S149
*
*      Desc:  Connection state function
*             event - SETUP Tmr Expiry
*             state - CCS_AWTOREDIRECT_RSP
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2.c
*
*/
PUBLIC S16 ccConE14S149
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  CCDP(AIN_DBG_LEVEL_1, "ccConE14S149: %ld\n", con->icSuConnId);

  /* Generate alarm */
  ccGenAlarm(LCM_CATEGORY_PROTOCOL, LCM_EVENT_INV_STATE,
             LCC_CAUSE_SETUPTMR_EXP, (U8 *) &con->icSuConnId);

  if (con->ogRscVal)
  {
    ccCloseTBcmTransaction(con);
    con->direction = OUTTOINC;
    if (con->noRspReqd == FALSE) ccSendReleaseRsp(con);
    ccDeallocateResource(con, CC_OUTGOING);
    con->ogRscVal = FALSE;
  }

  CC_POPULATE_CDR_FOR_SIP3XX(con);

  ccHandleCleanUpConCb(con, FALSE, CCTMPFAIL);
  RETVALUE(ROK);
} /* ccConE14S149 */

/* - FID 15801.0 */

/* FID 14457.0 + */

/* 
*  
*      Fun:   ccConE96S37
*
*      Desc:  Connection state function
*             event - CCE_CALLQUEUED
*             state - CCS_AWTRSPFORDPE3
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE96S37
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
   S16                ret                   = RFAILED;
   SvcHeader_t       *icSvcHeader           = NULLP;
   CallQueued_t      *pCallQueued           = NULLP;

   CCDP(AIN_DBG_LEVEL_1, "%s: Rec CallQueued in AWTRSPFORDPE3\n",__FUNCTION__);

   if (event2 == NULLP)
   {
       CCLOGERR("%s: Critical ERROR: Null pointers: event2=%ld\n", __FUNCTION__,event2);
       ABORT_DEBUG;
       ccHandleCleanUpConCb(con,TRUE,CCTMPFAIL);
       RETVALUE(ret);
   }

   pCallQueued = (CallQueued_t*)event2; 

   ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));

   icSvcHeader = (SvcHeader_t *) event;
   if (icSvcHeader->lastTransactionFlag == TRUE)
      ccDeleteBcm(&(con->icBcm));

   /* Update billing that call has been queued */
   if (ccInit.acnt == TRUE)
   {
      con->callDtlInfo.cm.callQueued=TRUE;
   }

   /* If no annoucement is to be played just send backward message
    * indicating queuing and stay in the same state
    */
    if (pCallQueued->resourceType.pres == PRSNT_NODEF)
    {
       if (pCallQueued->resourceType.val != RtPlayAnnouncements)
       {
           CCLOGERR("%s: Unexpected resourceType=%d\n", __FUNCTION__,pCallQueued->resourceType.val);
           ABORT_DEBUG;
           ccHandleCleanUpConCb(con,TRUE,CCTMPFAIL);
           RETVALUE(ret);
       }
       CC_SET_CALL_FLAG(con->flag, CC_FLAG_CALLQUEUED); /* Mark call queued */
       con->lastState = con->state;
       con->anncBlock.stateForAnnc = con->state;
       ret = ccSendToResource(&(con->icBcm));
       if (ret != ROK)
       {
         CCLOGERR("Error: ccSendToResource failed:icSu:%ld, ogSu:%ld, icTrsId:%ld\n",
                  con->icSuConnId, con->ogSuConnId, (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0));
         ccHandleCleanUpConCb(con, TRUE, AM_CV_TMPFAIL);
         CC_CLR_CALL_FLAG(con->flag, CC_FLAG_CALLQUEUED); /* Clear mark of call queued annc */
       }
    }
    else
    {
       CC_SET_CALL_FLAG(con->flag, CC_FLAG_CALLQUEUED);       /* Mark call queued */
       ccSendBackwardMessageForCallQueued(con);
    }
    RETVALUE(ROK);
} /* End of ccConE96S37 */

/* 
*  
*      Fun:   ccConE96S86
*
*      Desc:  Connection state function
*             event - CCE_CALLQUEUED
*             state - CCS_AWTRSPFORDPE17
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE96S86
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
   S16                ret                   = RFAILED;
   SvcHeader_t       *icSvcHeader           = NULLP;
   CallQueued_t      *pCallQueued           = NULLP;

   CCDP(AIN_DBG_LEVEL_1, "%s: Rec CallQueued in CCS_AWTRSPFORDPE17\n",__FUNCTION__);

   if (event2 == NULLP)
   {
       CCLOGERR("%s: Critical ERROR: Null pointers: event2=%ld\n", __FUNCTION__,event2);
       ABORT_DEBUG;
       ccHandleCleanUpConCb(con,TRUE,CCTMPFAIL);
       RETVALUE(ret);
   }

   pCallQueued = (CallQueued_t*)event2; 

   ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));

   icSvcHeader = (SvcHeader_t *) event;
   if (icSvcHeader->lastTransactionFlag == TRUE)
      ccDeleteBcm(&(con->icBcm));

   /* Update billing that call has been queued */
   if (ccInit.acnt == TRUE)
   {
      con->callDtlInfo.cm.callQueued=TRUE;
   }

   /* If no annoucement is to be played just send backward message
    * indicating queuing and stay in the same state
    */
    if (pCallQueued->resourceType.pres == PRSNT_NODEF)
    {
       if (pCallQueued->resourceType.val != RtPlayAnnouncements)
       {
           CCLOGERR("%s: Unexpected resourceType=%d\n", __FUNCTION__,pCallQueued->resourceType.val);
           ABORT_DEBUG;
           ccHandleCleanUpConCb(con,TRUE,CCTMPFAIL);
           RETVALUE(ret);
       }
       CC_SET_CALL_FLAG(con->flag, CC_FLAG_CALLQUEUED); /* Mark call queued */
       con->lastState = con->state;
       con->anncBlock.stateForAnnc = con->state;
       ret = ccSendToResource(&(con->icBcm));
       if (ret != ROK)
       {
         CCLOGERR("Error: ccSendToResource failed:icSu:%ld, ogSu:%ld, icTrsId:%ld\n",
                  con->icSuConnId, con->ogSuConnId, (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0));
         ccHandleCleanUpConCb(con, TRUE, AM_CV_TMPFAIL);
         CC_CLR_CALL_FLAG(con->flag, CC_FLAG_CALLQUEUED); /* Clear mark of call queued annc */
       }
    }
    else
    {
       CC_SET_CALL_FLAG(con->flag, CC_FLAG_CALLQUEUED);       /* Mark call queued */
       ccSendBackwardMessageForCallQueued(con);
    }
    RETVALUE(ROK);
} /* End of ccConE96S86 */
/*
*
*      Fun:   ccConE04S86
*
*      Desc:  Connection state function
*             event - Cct Release Indication
*             state - CCS_AWTRSPFORDPE17 
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE04S86
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
   S16            ret = RFAILED;

   CCDP(AIN_DBG_LEVEL_1, "%s: icSu = %ld, ogSu = %ld,"
       " T.transId = %ld\n", __FUNCTION__,con->icSuConnId, con->ogSuConnId,
       con->ogBcm.svcHeader.transId);

   /* FID 14457.0 GETS +  */
   if (CC_IS_CALL_FLAG_SET(con->flag,CC_FLAG_CALLQUEUED))
   {
      ccStopConTmr(con, TMR_SETUP);
      ccStopConTmr(con, CC_TMR_EARLYACM);
      ccStopConTmr(con, CC_TMR_CALLDTL);
      ccStartConTmr(TMR_RELEASE, con, (PTR)&ccCp.genCfg);
      ccUpdateCloseCause(con, CC_BOTH, CscCallerAbandon);

      /* FID 14732 */
      ccExtractSipwMBufFromRel(con, CCE_RELIND, (PTR) event);
      /* FID 14732 */

      if ((ret = ccStoreEvnt(con->evntType, con->icEvntType, con, (U8 *)event,
                            sizeof(CcRelEvnt))) != ROK)
      {
          CCLOGERR( "ERROR: ccStoreEvnt()failed. Continuing with call\n");
      }
      if (CC_IS_CONTEXT_PRESENT(con))
      {
         ccQSubBasedOnCcn(con);  /* FID 15261.0 */

         con->relPend = INRELRSPANDNOREL;
         CC_STATE_CHANGE(con, CCS_AWTSWTCFM_SENDNOREL);
         con->mgBlock->con = con;
         ret = ccSendMgiQueue(0, con->mgBlock);
         if (ret == ROK)
            RETVALUE(ROK);
         else
         {
            CCLOGERR("Error: During switching icSu = 0x%lx\n", con->icSuConnId);
            cleanUpMgQ(con);
            ccCleanUpMGCtx (con);
         }
      }

      if (con->noRspReqd == FALSE) 
      {                       
          ccSendReleaseRsp(con);
      }                       
      CC_STATE_CHANGE(con, CCS_AWTDEALOCCFM);
                                                                        
      if(con->icRscVal == FALSE)
      {
          CC_STATE_CHANGE(con, CCS_IDLE);
          ccRelCon(con);
      }                       
      else 
      {
          ccDeallocateResource(con, CC_INCOMING);
      }
      RETVALUE(ROK);
  }
   /* FID 14457.0 GETS - */
/* Begin - Bug# 52113 */
  if ((ccCheckEventInBuffer(&(con->ogBcm), CCE_RELIND) && 
      (con->direction == OUTTOINC)) ||
      (ccCheckEventInBuffer(&(con->icBcm), CCE_RELIND) && 
      (con->direction == INCTOOUT)))
  {
    CCDP(AIN_DBG_LEVEL_1, "%s():: REL is already buffered , drop it(direction=%d)\n",
         __FUNCTION__, con->direction);
    RETVALUE(ROK);
  }
/* End - Bug# 52113 */ 
  ccBufferEventToQueue(con, bcm, event, CCE_RELIND, sizeof(CcRelEvnt), 0,
                       NULLP, 0);
  RETVALUE(ROK);
}

/* 
*      Fun:   ccConE96S123
*
*      Desc:  Connection state function
*             event - CCE_CALLQUEUED
*             state - CCS_AWTIDISCONNECT
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE96S123
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
   S16                ret                   = RFAILED;
   SvcHeader_t       *icSvcHeader           = NULLP;
   CallQueued_t      *pCallQueued           = NULLP;

   CCDP(AIN_DBG_LEVEL_1, "%s: Rec CallQueued in CCS_AWTIDISCONNECT\n",__FUNCTION__);

   if (event2 == NULLP)
   {
       CCLOGERR("%s: Critical ERROR: Null pointers: event2=%ld\n", __FUNCTION__,event2);
       ABORT_DEBUG;
       ccHandleCleanUpConCb(con,TRUE,CCTMPFAIL);
       RETVALUE(ret);
   }

   pCallQueued = (CallQueued_t*)event2; 

   ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));

   icSvcHeader = (SvcHeader_t *) event;
   if (icSvcHeader->lastTransactionFlag == TRUE)
      ccDeleteBcm(&(con->icBcm));

   /* Update billing that call has been queued */
   if (ccInit.acnt == TRUE)
   {
      con->callDtlInfo.cm.callQueued=TRUE;
   }

   /* If no annoucement is to be played just send backward message
    * indicating queuing and stay in the same state
    */
    if (pCallQueued->resourceType.pres == PRSNT_NODEF)
    {
       if (pCallQueued->resourceType.val != RtPlayAnnouncements)
       {
           CCLOGERR("%s: Unexpected resourceType=%d\n", __FUNCTION__,pCallQueued->resourceType.val);
           ABORT_DEBUG;
           ccHandleCleanUpConCb(con,TRUE,CCTMPFAIL);
           RETVALUE(ret);
       }
       CC_SET_CALL_FLAG(con->flag, CC_FLAG_CALLQUEUED); /* Mark call queued */
       con->lastState = con->state;
       con->anncBlock.stateForAnnc = con->state;
       ret = ccSendToResource(&(con->icBcm));
       if (ret != ROK)
       {
         CCLOGERR("Error: ccSendToResource failed:icSu:%ld, ogSu:%ld, icTrsId:%ld\n",
                  con->icSuConnId, con->ogSuConnId, (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0));
         ccHandleCleanUpConCb(con, TRUE, AM_CV_TMPFAIL);
         CC_CLR_CALL_FLAG(con->flag, CC_FLAG_CALLQUEUED); /* Clear mark of call queued annc */
       }
    }
    else
    {
       CC_SET_CALL_FLAG(con->flag, CC_FLAG_CALLQUEUED);       /* Mark call queued */
       ccSendBackwardMessageForCallQueued(con);
    }
    RETVALUE(ROK);
} /* End of ccConE96S123 */

/*
*      Fun:   ccConE04S123
*
*      Desc:  Connection state function
*             event - Cct Release Indication
*             state - CCS_AWTIDISCONNECT
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE04S123
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
   S16            ret = RFAILED;

   CCDP(AIN_DBG_LEVEL_1, "%s: icSu = %ld, ogSu = %ld,"
       " T.transId = %ld\n", __FUNCTION__,con->icSuConnId, con->ogSuConnId,
       con->ogBcm.svcHeader.transId);

   /* FID 14457.0 GETS +  */
   if (CC_IS_CALL_FLAG_SET(con->flag,CC_FLAG_CALLQUEUED))
   {
      ccStopConTmr(con, TMR_SETUP);
      ccStopConTmr(con, CC_TMR_EARLYACM);
      ccStopConTmr(con, CC_TMR_CALLDTL);
      ccStartConTmr(TMR_RELEASE, con, (PTR)&ccCp.genCfg);
      ccUpdateCloseCause(con, CC_BOTH, CscCallerAbandon);

      /* FID 14732 */
      ccExtractSipwMBufFromRel(con, CCE_RELIND, (PTR) event);
      /* FID 14732 */

      if ((ret = ccStoreEvnt(con->evntType, con->icEvntType, con, (U8 *)event,
                            sizeof(CcRelEvnt))) != ROK)
      {
          CCLOGERR( "ERROR: ccStoreEvnt()failed. Continuing with call\n");
      }

      if (CC_IS_CONTEXT_PRESENT(con))
      {
         ccQSubBasedOnCcn(con);  /* FID 15261.0 */

         con->relPend = INRELRSPANDNOREL;
         CC_STATE_CHANGE(con, CCS_AWTSWTCFM_SENDNOREL);
         con->mgBlock->con = con;
         ret = ccSendMgiQueue(0, con->mgBlock);
         if (ret == ROK)
            RETVALUE(ROK);
         else
         {
            CCLOGERR("Error: During switching icSu = 0x%lx\n", con->icSuConnId);
            cleanUpMgQ(con);
            ccCleanUpMGCtx (con);
         }
      }

      if (con->noRspReqd == FALSE) 
      {                       
          ccSendReleaseRsp(con);
      }                       
      CC_STATE_CHANGE(con, CCS_AWTDEALOCCFM);
                                                                        
      if(con->icRscVal == FALSE)
      {
          CC_STATE_CHANGE(con, CCS_IDLE);
          ccRelCon(con);
      }                       
      else 
      {
          ccDeallocateResource(con, CC_INCOMING);
      }
      RETVALUE(ROK);
   }
   /* FID 14457.0 GETS - */

  /* Begin - Bug# 52113 */
  if ((ccCheckEventInBuffer(&(con->ogBcm), CCE_RELIND) && 
      (con->direction == OUTTOINC)) ||
      (ccCheckEventInBuffer(&(con->icBcm), CCE_RELIND) && 
      (con->direction == INCTOOUT)))
  {
    CCDP(AIN_DBG_LEVEL_1, "%s():: REL is already buffered , drop it(direction=%d)\n",
         __FUNCTION__, con->direction);
    RETVALUE(ROK);
  }
  /* End - Bug# 52113 */

  ccBufferEventToQueue(con, bcm, event, CCE_RELIND, sizeof(CcRelEvnt), 0,
                       NULLP, 0);
  RETVALUE(ROK);
} /* End of ccConE04S123 */

/* 
*  
*      Fun:   ccConE37S89
*
*      Desc:  Connection state function
*             event - CCE_ANALYZE_ROUTE
*             state - CCS_AWTMGCTNOTIFY0
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE37S89
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
   S16                    ret                   = RFAILED;
   MgctEventsDescriptor   evntDesc;
   MgctSignalsDescriptor  signalDesc;


   CCDP(AIN_DBG_LEVEL_1, "%s: Rec CCE_ANALYZE_ROUTE in CCS_AWTMGCTNOTIFY0\n",__FUNCTION__);

   if (CC_IS_CALL_FLAG_SET(con->flag,CC_FLAG_CALLQUEUED))
   {
      CCDP(AIN_DBG_LEVEL_1,"%s:icSu:%ld, ogSu:%ld, suCtxId:%ld\n", __FUNCTION__,con->icSuConnId,
          con->ogSuConnId, (con->mgBlock->mgCtx ? con->mgBlock->mgCtx->suCtxId : 0));
      ccBufferEventToQueue(con, bcm, event, CCE_ANALYZE_ROUTE, sizeof(SvcHeader_t),
                       con->icSuConnId, event2, sizeof(AnalyzeRoute_t));

      ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));

      if (CC_IS_CONTEXT_PRESENT(con))
      {
          cmMemset((U8 *)&evntDesc, 0, sizeof(MgctEventsDescriptor));
          cmMemset((U8 *)&signalDesc, 0, sizeof(MgctSignalsDescriptor));
   
          evntDesc.pres = PRSNT_NODEF;
          signalDesc.pres = PRSNT_NODEF;
   
          ccUpdateMgQForAddOrModifyRsc(MGCT_MODE_SENDRECV, MGCT_PACKAGE_ID_MAX,
                                       MGCT_ITEM_ID_CG_MAX,
                                       MGCT_CMD_MODIFY, &(con->icBcm),
                                       &evntDesc, NULLP, &signalDesc, NULLP, NULLP,
                                       con->mgBlock->mgCtx->suCtxId,
                                       con->mgBlock->mgCtx->contextId, con);
          CC_STATE_CHANGE(con, CCS_AWTMGCTCFMFORMODIFY2);
          con->mgBlock->con = con;
          ccChangeCallStateOfAssociatedCalls(&(con->icBcm), CCS_CANNOTPROCESSEVENTS);
          ret = ccSendMgiQueue(con->mgBlock->mgCtx->suCtxId, con->mgBlock);
          if (ret == ROK)
          {
            RETVALUE(ROK);
          }
          else
          {
            CCLOGERR("Error: During switching icSu = 0x%lx\n", con->icSuConnId);
            ccRestoreCallStateOfAssociatedCalls(&(con->icBcm));
          }
      }
      else
      {
        CCLOGERR("Error: No contextId for icSu = 0x%lx\n", con->icSuConnId);
      }
      cleanUpMgQ(con);
      ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
   }
   else
   {
      ABORT_DEBUG;
      CCDBGP(DBGMASK_CC, (ccInit.prntBuf, " Received Unexpected Event \n"));
   }
   RETVALUE(ret);
} /* End of ccConE96S37 */
/* FID 14457.0 -*/

/* FID 14457.0 GETS -- Connection State Functions for State 150 + */

/*
*      Fun:   ccConE97S150
*
*      Desc:  Connection state function
*             event - CCE_HPCMGRETRYTIMEREXPRIY
*             state - CCS_AWTHPCMGRETRYTIMEREXPRIY
*
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE97S150
(
CcConCb   *con,             /* connection */
PTR        event,           /* event */
AinBCM_t  *bcm,             /* bcm */
PTR        event2           /* event */
)
{
  CCDP(AIN_DBG_LEVEL_1, "ccConE97S150 icSu:0x%lx\n", con->icSuConnId);
  /* retry the same MG or MG list for the HPC call */
  if (con && (con->routeEvnt) && (!CC_IS_MAX_HPC_MG_RETRIES(con)))
  {
    /* update counts to keep track of MG retried */
    con->ccmCb.hpcMgRetries++;
    CCDP(AIN_DBG_LEVEL_1, "Retry for HPC the same MG %d time(s) icSu:0x%lx\n", 
         con->ccmCb.hpcMgRetries, con->icSuConnId);

    if (!CC_EGRESS_INTER_CCS_CALL(con))
        con->routeEvnt->currentInterface--;   /* keep the same route as last time */
    ccRouteCall(con, FALSE);
  }
  else  /* should never get here */
  {
    ABORT_DEBUG;
    ccProcessFailCnd(con, FC_INTERNAL, TRUE);
  }
  RETVALUE(ROK);
} /* ccConE97S150 */

/*
*
*      Fun:   ccConE14S150
*
*      Desc:  Connection state function
*             event - SETUP Tmr Expiry
*             state - CCS_AWTHPCMGRETRYTIMEREXPRIY
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE14S150
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  CCLOGERR("ccConE14S150: SET UP timer expiry. %ld\n", con->icSuConnId);

  /* Generate alarm */
  ccGenAlarm(LCM_CATEGORY_PROTOCOL, LCM_EVENT_INV_STATE,
             LCC_CAUSE_SETUPTMR_EXP, (U8 *) &con->icSuConnId);

  if (con->ogRscVal)
  {
    ccDeallocateResource(con, CC_OUTGOING);
    con->ogRscVal = FALSE;
  }

  ccPlayToneOrRelease(con, CCTMPFAIL);
  RETVALUE(ROK);
} /* ccConE14S150 */

/* 
* 
*      Fun:   ccConE03S150
*  
*      Desc:  Connection state function
*             event - CCE_CNST_IND
*             state - CCS_AWTHPCMGRETRYTIMEREXPRIY
* 
*      Ret:   ROK     - successful,
*
*      Notes: None.
*   
*      File:  cc_bdy2_2.c
*   
*/
PUBLIC S16 ccConE03S150
(
CcConCb   *con,             /* connection */
PTR        event,           /* event */
AinBCM_t  *bcm,             /* bcm */
PTR        event2           /* event */
)
{
  S16                      ret          = RFAILED;
  ProtType                 icProtType   = CC_GET_PROTOCOL(con->icProtType);

  CCDP(AIN_DBG_LEVEL_0, "ccConE03S150: 0x%lx, 0x%lx\n", con->icSuConnId, con->ogSuConnId);

  if (ccIsForcedOnHookRcvd(con->icProtType, con->ogProtType, con->icEvntType,
          con->direction))
  {
    CCDP(AIN_DBG_LEVEL_0, "IOM failover has occured: %lx\n", con->icSuConnId);
    cleanUpMgQ(con);
    ccCleanUpMGCtx (con);
    ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
    RETVALUE(ROK);
  }

  if(con->direction == OUTTOINC)
  {
    CCLOGERR("Error: Invalid direction :%d, icSu:%ld\n", con->direction,
              con->icSuConnId);
    RETVALUE(ROK);
  }

  if (ccIsOnHookRcvd(con->icProtType, con->ogProtType, con->icEvntType,
          con->direction))
  {
    ret = ccProcessDisconnectEvent(con, event);
    if (con->surrogate != NULLP)
       ccProcessDisconnectNotfnForSurrogateLegs(con, event);
    if (ret == ROK)
    {
      CCDP(AIN_DBG_LEVEL_0,"ccProcessDisconnectEvent success,event detec.\n");
      RETVALUE(ROK);
    }
    else if (ret == RFAILED)
    {
      CCDP(AIN_DBG_LEVEL_3,"Failed to Check if (O/T)Disc event is armed\n");
    }
    CCDP(AIN_DBG_LEVEL_0, "O/T_Disconnect event is not Armed\n");

    ccStopConTmr(con, TMR_SETUP);
    ccStopConTmr(con, CC_TMR_HPC_MG_RETRY);
    ccStartConTmr(TMR_RELEASE, con, (PTR)&ccCp.genCfg);
    if (icProtType != CC_LOCAL_SWT_PROT)
    {
      ret = ccProcessDisconnectNotfnForCaleaLegs(con, event);
      if (ret != ROK)
      {
        CCDP(AIN_DBG_LEVEL_4,
           "Critical Error!!ccProcessDisconnectNotfnForCaleaLegs FAILED\n");
      }
    }
    if (CC_IS_CONTEXT_PRESENT(con))
    {
      ccQSubBasedOnCcn(con); 

      con->relPend = ICRELONLY;
      CC_STATE_CHANGE(con, CCS_AWTSWTCFM_SENDONEREL);
      con->mgBlock->con = con;
      ret = ccSendMgiQueue(0, con->mgBlock);
      if (ret == ROK)
        RETVALUE(ROK);
      else
      {
        CCLOGERR("Error: During switching icSu = 0x%lx\n", con->icSuConnId);
        cleanUpMgQ(con);     
        ccCleanUpMGCtx (con);
      }
    }
    CC_STATE_CHANGE(con, CCS_AWTRELCFM_IC);
    ccSendRelease(con, con->icSapCb, con->icSpConnId, con->icSuConnId,
                  con->icProtType, CCCALLCLR, NULL);
    RETVALUE(ROK);
  }

  /* Bug 88027 */
  if (ccIsPrackRcvd(CC_GET_PROTOCOL(con->icProtType), CC_GET_PROTOCOL(con->ogProtType), 
                    con->icEvntType, con->direction))
  {
    ccBufferEventToQueue(con, bcm, event, CCE_CNSTIND, sizeof(CcCnStEvnt), 0,NULLP, 0);
    RETVALUE(ROK);
    
  }

  CCLOGERR("ccConE03S150(): unExpected evnt in state = %d, ProtocolType = %d,"
           " direction = %d, evnType =%d\n", con->state, con->icProtType,
           con->direction, con->icEvntType);

  RETVALUE(ROK);
} /* ccConE03S150 */

/*
*      Fun:   ccConE04S150
*
*      Desc:  Connection state function
*             event - CCE_RELIND 
*             state - CCS_AWTHPCMGRETRYTIMEREXPRIY
*
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE04S150
(
CcConCb   *con,             /* connection */
PTR        event,           /* event */
AinBCM_t  *bcm,             /* bcm */
PTR        event2           /* event */
)
{
  S16                      ret          = RFAILED;
  ProtType                 icProtType   = CC_GET_PROTOCOL(con->icProtType);

  CCDP(AIN_DBG_LEVEL_1, "ccConE04S150 :0x%lx\n", con->icSuConnId);

  if (con->direction != INCTOOUT)
  {
    CCLOGERR("Error: Invalid direction icSu = 0x%lx\n", con->icSuConnId);
    RETVALUE(ROK);
  }

  ret = ccProcessDisconnectEvent(con, event);
  if (con->surrogate != NULLP)
     ccProcessDisconnectNotfnForSurrogateLegs(con, event);

  if (ret == ROK)
  {
    CCDP(AIN_DBG_LEVEL_0,"ccProcessDisconnectEvent success,event detec.\n");
    RETVALUE(ROK);
  }
  else if (ret == RFAILED)
  {
    CCDP(AIN_DBG_LEVEL_3,"Failed to Check if (O/T)Disc event is armed\n");
  }
  CCDP(AIN_DBG_LEVEL_0, "O/T_Disconnect event is not Armed\n");

  ccStopConTmr(con, TMR_SETUP);
  ccStopConTmr(con, CC_TMR_HPC_MG_RETRY);
  ccStartConTmr(TMR_RELEASE, con, (PTR)&ccCp.genCfg);
  ccUpdateCloseCause(con, CC_BOTH, CscCallerAbandon);

  /* CALL release before the Tapped subject goes to ANSWERED state */
  if (icProtType != CC_LOCAL_SWT_PROT)
  {
    ret = ccProcessDisconnectNotfnForCaleaLegs(con, event);
    if (ret != ROK)
    {
      CCDP(AIN_DBG_LEVEL_4,
           "Critical Error!!ccProcessDisconnectNotfnForCaleaLegs FAILED\n");
    }
  }

  /* FID 14732 */                      
  ccExtractSipwMBufFromRel(con, CCE_RELIND, (PTR) event);
  /* FID 14732 */

  if ((ret = ccStoreEvnt(con->evntType, con->icEvntType, con, (U8 *)event,
                         sizeof(CcRelEvnt))) != ROK)
  {
    CCLOGERR( "ERROR: ccStoreEvnt()failed. Continuing with call\n");
  }

  if (CC_IS_CONTEXT_PRESENT(con))
  {
    ccQSubBasedOnCcn(con);  /* FID 15261.0 */

    con->relPend = INRELRSPANDNOREL;
    CC_STATE_CHANGE(con, CCS_AWTSWTCFM_SENDNOREL);
    con->mgBlock->con = con;
    ret = ccSendMgiQueue(0, con->mgBlock);
    if (ret == ROK)
      RETVALUE(ROK);
    else
    {
      CCLOGERR("Error: During switching icSu = 0x%lx\n", con->icSuConnId);
      cleanUpMgQ(con);     
      ccCleanUpMGCtx (con);
    }
  }

  if (con->noRspReqd == FALSE) ccSendReleaseRsp(con);
  CC_STATE_CHANGE(con, CCS_AWTDEALOCCFM);
  ccDeallocateResource(con, CC_INCOMING);
  RETVALUE(ROK);
} /* ccConE04S150 */

/* FID 14457.0 Connection State Functions for State 150 - */

/*
   *
   *      Fun:   ccConE14S68
   *
   *      Desc:  Connection state function
   *             event - CCE_SETUPTMREXP
   *             state - CCS_CANNOTPROCESSEVENTS 
   *      Ret:   ROK     - successful,
   *             RFAILED - unsuccessful
   *
   *      Notes: None.
   *
   *      File:  cc_bdy2_2.c
   *
   */
PUBLIC S16 ccConE14S68
(
 CcConCb *con,                    /* connection */
 PTR     event,                   /* event */
 AinBCM_t *bcm,                    /* bcm */
 PTR     event2                    /* event */
 )
{
    CCDP(AIN_DBG_LEVEL_1, "%s:%ld,%ld,%p\n", __FUNCTION__, con->icSuConnId,
                         con->ogSuConnId, con->assocBlock);
    ccGenAlarm(LCM_CATEGORY_PROTOCOL, LCM_EVENT_INV_STATE, 
                LCC_CAUSE_SETUPTMR_EXP, (U8 *) &con->icSuConnId);

    if(con->assocBlock)
    {
       CcConCb *con1;
       S16     ret = RFAILED;
       ret = cmHashListFind(&ccSuInstTbl,(U8 *)&(CC_GET_ASSOCPARENTSUCONNID(con)),
             (U16)sizeof(UConnId), 0, (PTR *)&con1);
       if(ret != ROK || con1 == NULLP)
       {
          CCLOGERR("%s - ERROR: CRBT call Con1 has gone!\n", __FUNCTION__);
          cleanUpMgQ(con);
          ccCleanUpMGCtx(con);
          ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
          RETVALUE(ROK);
       }
    }
    ccUnexpEvent(con, event, bcm, event2);
    RETVALUE(ROK);          
} /* End of ccConE14S68 */
/* BUG: 88025 + */
/* 
*  
*      Fun:   ccConE98S09
*
*      Desc:  Connection state function
*             event - CCE_HPCDELAYREQ
*             state - CCS_AWTANS
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE98S09
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
   CCDP(AIN_DBG_LEVEL_1, "%s: icSu:0x%lx\n", __FUNCTION__,con->icSuConnId);
   if (CC_EGRESS_INTER_CCS_CALL(con))
   {
      ccSendBackwardMessageForCallQueued(con);
   }
   RETVALUE(ROK);
} /* End of ccConE98S09 */

/* 
*  
*      Fun:   ccConE98SBUF
*
*      Desc:  Connection state function
*             event - CCE_HPCDELAYREQ
*             state - Various
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE98SBUF
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
   CCDP(AIN_DBG_LEVEL_1, "%s: icSu:0x%lx\n", __FUNCTION__,con->icSuConnId);

   if (CC_EGRESS_INTER_CCS_CALL(con))
   {
       ccBufferEventToQueue(con, bcm, event, CCE_HPCDELAYREQ, 0,
                            con->icSuConnId, NULLP, 0);
   }
   RETVALUE(ROK);
} /* End of ccConE98SBUF */
/* BUG: 88025 - */

/* FID 17174 Annc Support via External MRF + */
/* 
*  
*      Fun:   ccConE01S155
*
*      Desc:  Connection state function
*             event - CCE_CONCFM
*             state - CCS_AWT_MRFRSP 
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE01S155 
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  S16     ret = RFAILED;  

  U8                      mgiAction;
  U8                      icMode;
  U8                      ogMode;
  U8                      icSignal;
  U8                      ogSignal;
  ProtType                ogProtType     = CC_GET_PROTOCOL(con->ogProtType);
  Sdp*                    rmtSdp = NULLP;

  CCDP(AIN_DBG_LEVEL_1, "%s: icSu:0x%lx\n", __FUNCTION__,con->icSuConnId);

  if(con->direction == OUTTOINC)
  {
    if (CC_IS_MRF(con->mrfFlag, MRF_PLAYING_ANNC))
    {    
      /* Backward message already sent when get 183 */
      CCDP(AIN_DBG_LEVEL_1, "%s: ignore this 200OK but will set flag 200Ok-ANNC\n", __FUNCTION__);
      CC_SET_MRF(con->mrfFlag, MRF_200_ANNC);
    }
    else 
    {
      CCDP(AIN_DBG_LEVEL_1, "%s: get 200OK for MRF announcement\n", __FUNCTION__);  
      /* extract the SDP at first in case no SDP in backward message */
      if(ROK != ccExtractProtSdp(ccInit.region, ccInit.pool, con,
            &(((CcConEvnt *)event)->m.sipConEvnt.sdp)))
      {
        CCLOGERR("Error: SDP parsing failed \n");
        con->failCndVal = FC_PROTERR;
        /* Bug 78139 */
        if (con->keepGCCCallInfo == FALSE)
        {
          con->callDtlInfo.cm.callState = LCC_CDR_UNANSWERED;
        }

        if (CC_IS_CONTEXT_PRESENT(con))
        {
          ccQSubBasedOnCcn(con);  /* FID 15261.0 */

          ret = ccSendMgiQueue(0, con->mgBlock);
          if (ret != ROK)
          {
            CCLOGERR( "**** ERROR: can't do ccSendMgiQueue *******\n");
            cleanUpMgQ(con);
            ccCleanUpMGCtx(con);
          }
          else
          {
            CC_STATE_CHANGE(con, CCS_AWTSWTCFM_SENDONEREL);
            con->mgBlock->con = con;
            con->relPend = OUTRELANDINFAILCOND; /* will be used in state 88 */
            RETVALUE(ROK);
          }
        }
        ccFreeTheRscFromCon(con, OUTTOINC);
        ccProcessFailCnd(con, con->failCndVal, TRUE);
        RETVALUE(ROK);
      }

      /* FID-2575: LMSD + */
      ccExtractSipMethodSupport(con, OUTTOINC,
          ((CcConEvnt *)event)->m.sipConEvnt.headerSupportMask);
      /* MRF_200_ANNC will be set when get 200OK INVITE */
      CCDP(AIN_DBG_LEVEL_3, "mrfFlag set to MRF_200_ANNC\n");
      CC_SET_MRF(con->mrfFlag, MRF_200_ANNC);

      /* Add here to process angostic issue */
      if (CC_IS_CALL_MULTI_MG(con))
      {
        if (con->direction == OUTTOINC)
          con->ccmCb.state = CC_EGRESS_MG;
        else
          con->ccmCb.state = CC_INGRESS_MG;
      }

      CC_LOCATE_RMT_SDP(con, rmtSdp);

      if ((CC_IS_CALL_FLAG_SET(con->flag,CC_FLAG_AGNOSTIC_NO_OFFER_CANDIDATE)) &&
          (con->direction == OUTTOINC) && (rmtSdp) &&
          (!CC_IS_BCM_IN_CONTEXT(&con->ogBcm)))
      {
        ccAllocateMultiMediaCb(con);
        if (IS_MULTIMEDIA_4_BCM(&con->ogBcm))
        {
          /* FID 14941.0 */
          /* ccStoreMMBillingSdp is not invoked by ccExtractProtSdp for
             the initial offer */
          ccStoreMMBillingSdp(&con->ogBcm, FALSE, rmtSdp);

          MULTIMEDIACB_4_BCM(&con->ogBcm)->StreamCount = rmtSdp->numDecode;

          ccEnableMediaAwareStream_4_BCM (&con->ogBcm, rmtSdp);
        }

        if (IS_MULTIMEDIA_4_BCM(&con->icBcm))
        {
          MULTIMEDIACB_4_BCM(&con->icBcm)->StreamCount = rmtSdp->numDecode;
          ccEnableMediaAwareStream_4_BCM (&con->icBcm, rmtSdp);
        }

        if(IS_MULTIMEDIA_4_BCM(&con->ogBcm))
        {
          ccCheckToSkipStream(con, rmtSdp);
        }
        if (IS_MULTIMEDIA(con)) /* looks like MG is needed */
          ccFillIpVersion(con, CC_SWREQ_MAX);/* BUG:90566 */
      }

      /* set the OG OFFANS state whatever if there is MG action */
      if (CC_OFFANS_OGRMTOFFER_PENDING(con))
      {
          CC_OFFANS_STATE_CHANGE(con, CC_OFFANS_STATE_NO_OP,
                                         CC_OFFANS_STATE_RCVDOFFER,
                                         CC_OFFANS_GET_SUBSTATE(con),
                                         CC_OFFANS_METHOD_USED(con));
      }
      else if(CC_OFFANS_OGRMTANS_PENDING(con))
      {
          CC_OFFANS_STATE_CHANGE(con, CC_OFFANS_STATE_NO_OP,
                                      CC_OFFANS_STATE_NULL,
                                 CC_OFFANS_GET_SUBSTATE(con),
                                 CC_OFFANS_METHOD_USED(con));
      }

      mgiAction = ccFindSwitchingAction(con, CCE_CONCFM, 0,
              con->direction, &con->icBcm, &con->ogBcm, &icMode, &ogMode,
                  &icSignal, &ogSignal, event, TRUE);

      CCDP(AIN_DBG_LEVEL_3, "Action == %d, icMode == %d, icSignal == %d,"
              " ogMode == %d, ogSignal == %d\n", mgiAction, icMode, icSignal,
                  ogMode, ogSignal);

      CCDP(AIN_DBG_LEVEL_3,"%s: EnhEchoLogicVal=0x%lx\n",
           __FUNCTION__, con->echoParam.EnhEchoLogicVal);

      /* FID 14677.0 */
      /* Backward direction procedure */
      if(mgiAction != CC_NO_MGI_ACTION)
      {
         if ((ccProcessBackwrdDirEcho(con, (CcAllSdus *)event)) != ROK)
         {
            /* Bug 78139 */
            if (con->keepGCCCallInfo == FALSE)
            {
              con->callDtlInfo.cm.callState = LCC_CDR_UNANSWERED;
            }

            CCDP(AIN_DBG_LEVEL_3, "ERROR: ccProcessBackwrdDirEcho FAILED\n");
            RETVALUE(RFAILED);
         }
      }
      else if ((mgiAction == CC_NO_MGI_ACTION) &&
               (IS_ECHO_ENHTYPE1_AND_INVOKE(con)))
      {
         if (((!CC_INGRESS_INTER_CCS_CALL(con) && !CC_EGRESS_INTER_CCS_CALL(con) &&
               !CC_IS_CALL_MULTI_MG(con)) ||
              CC_OPERATING_EGRESS_MG(con)) &&
             ((ogProtType == CC_SI) || (ogProtType == CC_SIPT)))
         {
            if ((ccProcessBackwrdDirEcho(con, (CcAllSdus *)event)) != ROK)
            {
               /* Bug 78139 */
               if (con->keepGCCCallInfo == FALSE)
               {
                con->callDtlInfo.cm.callState = LCC_CDR_UNANSWERED;
               }

               CCDP(AIN_DBG_LEVEL_3, "ERROR: ccProcessBackwrdDirEcho FAILED\n");
               RETVALUE(RFAILED);
            }
         }
         if ((!CC_OPERATING_EGRESS_MG(con)) &&
             (con->echoParam.EnhEchoLogicVal & CC_INGRESS_MGI_MODIFY))
         {
            mgiAction = CC_MGI_ACTION_MODIFY;
         }
         if ((!CC_OPERATING_INGRESS_MG(con)) &&
             (con->echoParam.EnhEchoLogicVal & CC_EGRESS_MGI_MODIFY))
         {
            mgiAction = CC_MGI_ACTION_MODIFY;
         }
      }

      CCDP(AIN_DBG_LEVEL_3, "Action=%d, icMode=%d, icSignal=%d, ogMode=%d, ogSignal=%d\n",
           mgiAction, icMode, icSignal, ogMode, ogSignal);

      if(mgiAction != CC_NO_MGI_ACTION)
      {
        /* FID 15261.0 + */
        ret=ccProcessConCfmWithSwitching(con, (PTR)event);
        if (ret!=ROK)
        {
         /* Bug 78139 */
         if (con->keepGCCCallInfo == FALSE)
         {
           con->callDtlInfo.cm.callState = LCC_CDR_UNANSWERED;
         }
          RETVALUE(RFAILED);
        }
         RETVALUE(ROK);
      }
      /* MRF_PLAYING_ANNC will be set when no MG action or after success MG action */
      CC_SET_MRF(con->mrfFlag, MRF_PLAYING_ANNC);

      if (((con->remoteSdpSentOnIcBCM&&con->renegSdpAfterAns) ||con->isAnsIndSent) &&
          (CC_OFFANS_ICOFFANS_STATE_NULL(con))
         )
      {
        CCDP(AIN_DBG_LEVEL_1, "%s:send UPPDATE!\n",__FUNCTION__ );
        ccUpdateSession (&con->icBcm, MGCT_MODE_MAX, 0x00,
                         ccLcfUpdateCbFunc, (PTR)con, con->state);     
      }
      else
      {
        ret = ccSendBackwardMessageForAnnoucement(con, con->ainInfo.Resourcetype.val, FALSE, TRUE);
        if (ret != ROK)
        {
          CCLOGERR("%s: Error, ccSendBackwardMessageForAnnoucement failed icSu=0x%lx\n",
              __FUNCTION__,con->icSuConnId);
          ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
          RETVALUE(ret);
        }
      }
      CCDP(AIN_DBG_LEVEL_1, "%s:Reset the OFFANS state %x \n",__FUNCTION__,con->offAnsCb->state);
      if (CC_OFFANS_ICOFFER_PENDING(con))
      {
          CC_OFFANS_STATE_CHANGE(con, CC_OFFANS_STATE_SENTOFFER,
                                         CC_OFFANS_STATE_NO_OP,
                                         CC_OFFANS_SUBSTATE_AWAITRMTANS_INVITENOSDP,
                                         CC_OFFANS_TYPE_200OKINVITE);

      }
      else if(CC_OFFANS_ICANS_PENDING(con))
      {
          CC_OFFANS_STATE_CHANGE(con, CC_OFFANS_STATE_NULL,
                                 CC_OFFANS_STATE_NO_OP,
                                 CC_OFFANS_GET_SUBSTATE(con),
                                 CC_OFFANS_METHOD_USED(con));
      }
    }
  }
  else
  {
    CCLOGERR( "ERROR:Get 200OK from wrong direction\n");
  }
  RETVALUE(ROK);
} /* End of ccConE01S155 */

/* 
*  
*      Fun:   ccConE03S155
*
*      Desc:  Connection state function
*             event - CCE_CNSTIND 
*             state - CCS_AWT_MRFRSP 
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE03S155 
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
  S16     ret = RFAILED;  

  U8                      mgiAction;
  U8                      icMode;
  U8                      ogMode;
  U8                      icSignal;
  U8                      ogSignal;
  ProtType                icProtType     = CC_GET_PROTOCOL(con->icProtType);
  ProtType                ogProtType     = CC_GET_PROTOCOL(con->ogProtType);
  Sdp*                    rmtSdp = NULLP; 
  Buffer                  *uBuf  = NULLP;
  CcCnStEvnt               ogEvnt;

  CCDP(AIN_DBG_LEVEL_1, "%s: icSu:0x%lx\n", __FUNCTION__,con->icSuConnId);

  con->icEvnt = (CcAllSdus *)event;
  cmMemset((U8 *)&ogEvnt, 0, sizeof(ogEvnt));

  /* extract SDP at first */
  if(ROK != ccExtractSdpFromCnStInd(con, event))
  {
    CCLOGERR("Error: SDP parsing failed\n");
    con->failCndVal = FC_PROTERR;
    /* Bug 78139 */
    if (con->keepGCCCallInfo == FALSE)
    {
      con->callDtlInfo.cm.callState = LCC_CDR_UNANSWERED;
    }

    if (CC_IS_CONTEXT_PRESENT(con))
    {
      ccQSubBasedOnCcn(con);  /* FID 15261.0 */

      ret = ccSendMgiQueue(0, con->mgBlock);
      if (ret != ROK)
      {
        CCLOGERR( "**** ERROR: can't do ccSendMgiQueue *******\n");
        cleanUpMgQ(con);
        ccCleanUpMGCtx(con);
      }
      else
      {
        CC_STATE_CHANGE(con, CCS_AWTSWTCFM_SENDONEREL);
        con->mgBlock->con = con;
        con->relPend = OUTRELANDINFAILCOND; /* will be used in state 88 */
        RETVALUE(ROK);
      }
    }
    ccFreeTheRscFromCon(con, OUTTOINC);
    ccProcessFailCnd(con, con->failCndVal, TRUE);
    RETVALUE(ROK);
  } 
  ccExtractSipMethodSupport(con, OUTTOINC,
            ((CcCnStEvnt *)event)->m.sipCnStEvnt.headerSupportMask);

  if (CC_IS_CALL_MULTI_MG(con))
  {
    if (con->direction == OUTTOINC)
      con->ccmCb.state = CC_EGRESS_MG;
    else
      con->ccmCb.state = CC_INGRESS_MG;
  }

  CC_LOCATE_RMT_SDP(con, rmtSdp);
  if ((CC_IS_CALL_FLAG_SET(con->flag,CC_FLAG_AGNOSTIC_NO_OFFER_CANDIDATE)) &&
      (con->direction == OUTTOINC) && (rmtSdp) &&
      (!CC_IS_BCM_IN_CONTEXT(&con->ogBcm)))
  {
    ccAllocateMultiMediaCb(con);
    if (IS_MULTIMEDIA_4_BCM(&con->ogBcm))
    {
      /* FID 14941.0 */
      /* ccStoreMMBillingSdp is not invoked by ccExtractProtSdp for
         the initial offer */
      ccStoreMMBillingSdp(&con->ogBcm, FALSE, rmtSdp);
      
      MULTIMEDIACB_4_BCM(&con->ogBcm)->StreamCount = rmtSdp->numDecode;

      ccEnableMediaAwareStream_4_BCM (&con->ogBcm, rmtSdp);
    }

    if (IS_MULTIMEDIA_4_BCM(&con->icBcm))
    {
      MULTIMEDIACB_4_BCM(&con->icBcm)->StreamCount = rmtSdp->numDecode;
      ccEnableMediaAwareStream_4_BCM (&con->icBcm, rmtSdp);
    }

    if(IS_MULTIMEDIA_4_BCM(&con->ogBcm))
    {
      ccCheckToSkipStream(con, rmtSdp);
    }
    if (IS_MULTIMEDIA(con)) /* looks like MG is needed */
      ccFillIpVersion(con, CC_SWREQ_MAX);/* BUG:90566 */
  }

  if(con->direction == OUTTOINC)
  {  
    /* it should be always reliable 183 per requirement */
    if ( con->icEvntType == CCSIP_ET_183_CALLPRG )
    {
      /*ccIsRcvdSdpReliable == FALSE,ccFindSdpInMsg == NULLP*/
      if (rmtSdp == NULLP)
      {
      /* it should be always reliable 183 per requirement, ignore this 183 */
        DP("ERROR: remote SDP is not available, ignore this 183!!!\n");
        RETVALUE(ROK);
      }

      CC_SET_MRF(con->mrfFlag, MRF_183_ANNC);

      /* Get reliable 183, oprate MG action */
      mgiAction = ccFindSwitchingAction(con, CCE_CNSTIND, con->icEvntType,
                  con->direction, &con->icBcm, &con->ogBcm, &icMode, &ogMode, 
                  &icSignal, &ogSignal, event, TRUE);
         
      CCDP(AIN_DBG_LEVEL_3, "Action == %d, icMode == %d, icSignal == %d,"
          " ogMode == %d, ogSignal == %d\n", mgiAction, icMode, icSignal,
          ogMode, ogSignal);

      /* Backward direction procedure */
      if(mgiAction != CC_NO_MGI_ACTION)
      {
        if ((ccProcessBackwrdDirEcho(con, (CcAllSdus *)event)) != ROK)
        {
          CCDP(AIN_DBG_LEVEL_3, "ERROR: ccProcessBackwrdDirEcho FAILED\n");
          RETVALUE(RFAILED);
        }
      }
      else if ((mgiAction == CC_NO_MGI_ACTION) &&
          (IS_ECHO_BWDMSG(con)) &&
          (IS_ECHO_ENHTYPE1_AND_INVOKE(con)))
      {
         if (((!CC_INGRESS_INTER_CCS_CALL(con) && !CC_EGRESS_INTER_CCS_CALL(con) &&
               !CC_IS_CALL_MULTI_MG(con)) ||
              CC_OPERATING_EGRESS_MG(con)) &&
             ((con->icEvntType == CC_ET_NRESM) ||   
              ((ogProtType == CC_SI) || (ogProtType == CC_SIPT))))
         {
            if ((ccProcessBackwrdDirEcho(con, (CcAllSdus *)event)) != ROK)
            {
               CCDP(AIN_DBG_LEVEL_3, "ERROR: ccProcessBackwrdDirEcho FAILED\n");
               RETVALUE(RFAILED);
            }
         }
         if ((!CC_OPERATING_EGRESS_MG(con)) &&
             (con->echoParam.EnhEchoLogicVal & CC_INGRESS_MGI_MODIFY))
         {
            mgiAction = CC_MGI_ACTION_MODIFY;
         }
         if ((!CC_OPERATING_INGRESS_MG(con)) &&
             (con->echoParam.EnhEchoLogicVal & CC_EGRESS_MGI_MODIFY))
         {
            mgiAction = CC_MGI_ACTION_MODIFY;
         }
      }

      if(mgiAction != CC_NO_MGI_ACTION)
      {
        ret=ROKDRSIG; /* Set default that OTermSeized is not a requested event */
        if (con->icBcm.pic < PicOAlerting)
        {
           /* FID 14234.0 put APP in OTermseized if it's present */
           con->icEvnt = (CcAllSdus *)event;
           ret=ccProcessOTermSeized(&(con->icBcm));
        }
        if (ret == ROK)
        {
           /* OTermSeized is a requested event, move CcCnStEvnt to the 
            * ccEvntHold, and bail out 
            */
           if((ret = ccStoreEvnt(con->evntType, con->icEvntType, con, (U8 *)event, sizeof(CcCnStEvnt))) != ROK)
           {
               CCLOGERR( "ERROR: ccStoreEvnt FAILED: %ld\n", con->icSuConnId);
               ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
               RETVALUE(RFAILED);
           }     
           RETVALUE(ROK); 
        }
        else if (ret == ROKDRSIG)
        {
           ret=ccProcessOAlertingWithSwitching(con, (PTR)event);

           if (ret!=ROK)
              RETVALUE(RFAILED);
           else
              RETVALUE(ROK); /* FID17174 handle ccSendBackwardMessageForAnnoucement in E11SXX */
        }
        else if (ret == RFAILED)
        {
           CCLOGERR("ERROR: ccProcessOTermSeized FAILED: %ld\n", con->icSuConnId);
           ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
           RETVALUE(RFAILED);
        }
      } /* ADD or MODIFY - Done */

      CC_SET_MRF(con->mrfFlag, MRF_PLAYING_ANNC);

      /*set the OG OFFANS state if there is no MG action */
      /* for og OFF-ANS should done before PRACK, or ic should be after UPDATE*/
      if (CC_OFFANS_OGRMTOFFER_PENDING(con))
      {
          CC_OFFANS_STATE_CHANGE(con, CC_OFFANS_STATE_NO_OP,
                                         CC_OFFANS_STATE_RCVDOFFER,
                                         CC_OFFANS_GET_SUBSTATE(con),
                                         CC_OFFANS_METHOD_USED(con));
      }
      else if(CC_OFFANS_OGRMTANS_PENDING(con))
      {
          CC_OFFANS_STATE_CHANGE(con, CC_OFFANS_STATE_NO_OP,
                                      CC_OFFANS_STATE_NULL,
                                 CC_OFFANS_GET_SUBSTATE(con),
                                 CC_OFFANS_METHOD_USED(con));
      }
      /* Send PRACK to MRF if incoming do NOT support 100rel */
      if( (((CcCnStEvnt *)event)->m.sipCnStEvnt.reliable18X == CCT_SIP_REL18X_REL)&&
          ((icProtType == CC_SI)||
           (((icProtType == CC_SIP)||(icProtType == CC_SIPT))&&
            (!CCT_IS_100REL_SUPPORTED(con->icSipMethodSupMask) || (CC_OFFANS_OGOFFANS_STATE_NULL(con)))))
        )
      {
        CCDP(AIN_DBG_LEVEL_3, "get reliable 183, response with PRACK!\n");
        ccSendPrack(&con->ogBcm);
      }

      if (((con->remoteSdpSentOnIcBCM&&con->renegSdpAfterAns) ||con->isAnsIndSent) &&
          (CC_OFFANS_ICOFFANS_STATE_NULL(con))
         )
      { 
        CCDP(AIN_DBG_LEVEL_1,"will send update\n"); 
        ccUpdateSession (&con->icBcm, MGCT_MODE_MAX, 0x00,
                         ccLcfUpdateCbFunc, (PTR)con, con->state);

      }
      else
      { 
        ret = ccSendBackwardMessageForAnnoucement(con, con->ainInfo.Resourcetype.val, FALSE, TRUE);
        if (ret != ROK)
        {
          CCLOGERR("%s: Error, ccSendBackwardMessageForAnnoucement failed icSu=0x%lx\n",
              __FUNCTION__,con->icSuConnId);
          ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
          RETVALUE(ret);
        }
      }
      CCDP(AIN_DBG_LEVEL_1, "%s:Reset the OFFANS state %x for incoming \n",__FUNCTION__,con->offAnsCb->state);
      if (CC_OFFANS_ICOFFER_PENDING(con))
      {
          CC_OFFANS_STATE_CHANGE(con, CC_OFFANS_STATE_SENTOFFER,
                                         CC_OFFANS_STATE_NO_OP,
                                         CC_OFFANS_SUBSTATE_AWAITRMTANS_INVITENOSDP,
                                         CC_OFFANS_TYPE_REL18X_INVITENOSDP);
      }
      else if(CC_OFFANS_ICANS_PENDING(con))
      {
          CC_OFFANS_STATE_CHANGE(con, CC_OFFANS_STATE_NULL,
                                 CC_OFFANS_STATE_NO_OP,
                                 CC_OFFANS_GET_SUBSTATE(con),
                                 CC_OFFANS_METHOD_USED(con));
      }

      RETVALUE(ROK);
    }   
    else if (con->icEvntType == CCSIP_ET_180_RINGING)
    {
      if (((CcCnStEvnt *)event)->m.sipCnStEvnt.reliable18X == CCT_SIP_REL18X_REL)
      {
        CCDP(AIN_DBG_LEVEL_3, "get reliable 180, response with PRACK!\n");
        ccSendPrack(&con->ogBcm);
      }
      else
        CCLOGERR("%s: ignore this 180 message\n",__FUNCTION__);
      RETVALUE(ROK);
    }
    else if (ccIsUpdateRspRcvd(icProtType, ogProtType, con->icEvntType,
             con->direction))
    {
       CCDP(AIN_DBG_LEVEL_0,"Get UPDATE resp\n");
       ret = ccProcessUpdateRsp(con, icProtType, ogProtType, (CcAllSdus*)event);
       if (ret == ROKIGNORE)
       {
          RETVALUE (ROK);
       }
       if(ret == ROKPENDING) /* Save the event for later */
       {
          if ((!CC_IS_CALL_MULTI_MG(con)) ||
                (CC_IS_CALL_MULTI_MG(con) && CC_OPERATING_FIRST_MG(con)))
          {
             ccStoreEvnt(con->evntType, con->icEvntType, con, (U8 *)event,
                   sizeof(CcCnStEvnt));
          }
          RETVALUE(ROK);
       }

      /* For no mg switching action case, need to map this
       * update/re-invite out */
      con->ogEvnt = (CcAllSdus *) &ogEvnt;

      ret = ccMapEvent(con, CCE_CNSTIND, con->icEvntType);
     
      if (ret == CCIGNOREMAPPING)
      {
         ccDropUBuf(&con->uBuf);
         RETVALUE(ROK);
      }
      else if (ret != ROK)
      {
         ccStopConTmr(con, CC_TMR_CALLDTL);
         ccStopConTmr(con, TMR_SETUP);
         ccStartConTmr(TMR_RELEASE, con, (PTR)&ccCp.genCfg);

         /* Release all connections */
         ccDropUBuf(&con->uBuf);
         ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
         RETVALUE(ROK);
      }
      CC_OFFANS_STATE_CHANGE(con, CC_OFFANS_STATE_NULL,
            CC_OFFANS_STATE_NULL,
            CC_OFFANS_SUBSTATE_NULL,
            CC_OFFANS_TYPE_INVALID);
      uBuf = con->uBuf;
      con->uBuf = NULLP;
      if (con->direction == INCTOOUT)
      {
         ccSendCnStReq(con, con->ogSapCb->suId, &con->ogSapCb->pst,
               con->ogSapCb->spId, con->ogSpConnId, con->ogSuConnId,
               con->ogProtType, con->ogEvntType, &ogEvnt, uBuf);
      }
      else
      {
         ccSendCnStReq(con, con->icSapCb->suId, &con->icSapCb->pst,
               con->icSapCb->spId, con->icSpConnId, con->icSuConnId,
               con->icProtType, con->ogEvntType, &ogEvnt, uBuf);
      }

      RETVALUE(ROK);
    }
    else if (ccIsXferRspRcvd(icProtType, ogProtType, con->icEvntType,
             con->direction))
    {
       CCDP(AIN_DBG_LEVEL_0,"Get XFER resp\n");
       ret = ccProcessXferRsp(con, icProtType, ogProtType, (CcAllSdus*)event);

       if(con->state == CCS_AWT_MGI_XFERQRYHOLD) /* Save the event for later */
       {
          if ((!CC_IS_CALL_MULTI_MG(con)) ||
                (CC_IS_CALL_MULTI_MG(con) && CC_OPERATING_FIRST_MG(con)))
          {
             ccStoreEvnt(con->evntType, con->icEvntType, con, (U8 *)event,
                   sizeof(CcCnStEvnt));
          }
          RETVALUE(ROK);
       }

       /* For no mg switching action case, need to map this
        * update/re-invite out */
       if ((CC_OFFANS_METHOD_USED(con) == CC_OFFANS_TYPE_UPDATE) &&
             (((con->direction == INCTOOUT) &&
               CC_OFFANS_ICRMTANS_PENDING(con)) ||
              ((con->direction == OUTTOINC) &&
               CC_OFFANS_OGRMTANS_PENDING(con))))
       {
          ccConvertPostAnsUpdate(con);
       }

      con->ogEvnt = (CcAllSdus *) &ogEvnt;

      ret = ccMapEvent(con, CCE_CNSTIND, con->icEvntType);
     
      if (ret == CCIGNOREMAPPING)
      {
         ccDropUBuf(&con->uBuf);
         RETVALUE(ROK);
      }
      else if (ret != ROK)
      {
         ccStopConTmr(con, CC_TMR_CALLDTL);
         ccStopConTmr(con, TMR_SETUP);
         ccStartConTmr(TMR_RELEASE, con, (PTR)&ccCp.genCfg);

         /* Release all connections */
         ccDropUBuf(&con->uBuf);
         ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
         RETVALUE(ROK);
      }
      CC_OFFANS_STATE_CHANGE(con, CC_OFFANS_STATE_NULL,
            CC_OFFANS_STATE_NULL,
            CC_OFFANS_SUBSTATE_NULL,
            CC_OFFANS_TYPE_INVALID);
      uBuf = con->uBuf;
      con->uBuf = NULLP;
      if (con->direction == INCTOOUT)
      {
         ccSendCnStReq(con, con->ogSapCb->suId, &con->ogSapCb->pst,
               con->ogSapCb->spId, con->ogSpConnId, con->ogSuConnId,
               con->ogProtType, con->ogEvntType, &ogEvnt, uBuf);
      }
      else
      {
         ccSendCnStReq(con, con->icSapCb->suId, &con->icSapCb->pst,
               con->icSapCb->spId, con->icSpConnId, con->icSuConnId,
               con->icProtType, con->ogEvntType, &ogEvnt, uBuf);
      }

      RETVALUE(ROK);
    }
    else
    {
      CCLOGERR("%s: unexpect icEvnt %d\n",__FUNCTION__ ,con->icEvntType);
    }
  }
  else
  {
      CCDP(AIN_DBG_LEVEL_3, "Get icEvnt %d from incoming\n",con->icEvntType);  
      if (ccIsPrackRcvd(CC_GET_PROTOCOL(con->icProtType), CC_GET_PROTOCOL(con->ogProtType),
            con->icEvntType, con->direction)||
          (ccIsAckRcvd(CC_GET_PROTOCOL(con->icProtType), CC_GET_PROTOCOL(con->ogProtType),
            con->icEvntType, con->direction)))

      {
        CCDP(AIN_DBG_LEVEL_0,"PRACK/ACK following the 183/200.\n");

        if (!CC_IS_CONTEXT_PRESENT(con))
        {
          /* ic off-ans change */
          if ( CC_OFFANS_ICRMTANS_PENDING(con)&&rmtSdp)
          {
            /*  If ACK/PRACK contains sdp answer and we have sent sdp
             *  offer before */
            CC_OFFANS_STATE_CHANGE(con, CC_OFFANS_STATE_NULL,
                CC_OFFANS_STATE_NO_OP,
                CC_OFFANS_GET_SUBSTATE(con),
                CC_OFFANS_METHOD_USED(con));
          }

          if(CC_IS_MRF(con->mrfFlag, MRF_183_ANNC))
          {
            /* send PRACK to MRF even OG offans complete for PRACK*/
            ccSendPrack(&con->ogBcm);
          }
          else if (CC_IS_MRF(con->mrfFlag, MRF_200_ANNC)&&
              CC_OFFANS_OGANS_PENDING(con))
          {
            /* send ACK to MRF */
            ccSendAck(&con->ogBcm);
          }
          /* og off-ans change */
          if ((CC_OFFANS_OGRMTANS_PENDING(con) || CC_OFFANS_OGANS_PENDING(con)) && rmtSdp)
          {
            /*  If ACK/PRACK contains sdp answer and we have sent sdp
             *  offer before */
            CC_OFFANS_STATE_CHANGE(con, CC_OFFANS_STATE_NO_OP,
                CC_OFFANS_STATE_NULL,
                CC_OFFANS_GET_SUBSTATE(con),
                CC_OFFANS_METHOD_USED(con));
          }

        }
        else
        {
          if (CC_IS_MRF(con->mrfFlag, MRF_200_ANNC)&&
              CC_OFFANS_OGANS_PENDING(con))
          {
            /* Will send ACK after ccProcessPrackOrAck so set a new flag */
            CC_SET_MRF(con->mrfFlag, MRF_SEND_ACK);
          }
          ret = ccProcessPrackOrAck (con, con->icProtType, con->ogProtType, (CcAllSdus*)event);
          if (ret == ROKIGNORE)
          {
            RETVALUE (ROK);
          }
        }
      }
      if (ccIsUpdateRspRcvd(icProtType, ogProtType, con->icEvntType,
                            con->direction))
      {
        /* NOTE: This case will not be called normally 
         * since the UpdateRsp will be handled in E03S132 */
        CCDP(AIN_DBG_LEVEL_0,"Get UPDATE resp\n");

        if(CC_IS_MRF(con->mrfFlag, MRF_183_ANNC)) 
        {
          /* send PRACK to MRF according to UPDATE Resp(not only 200OK) */
          ccSendPrack(&con->ogBcm);
        }
        else if (CC_IS_MRF(con->mrfFlag, MRF_200_ANNC)&&
                 CC_OFFANS_OGANS_PENDING(con))
        {
          /* send ACK to MRF */
          ccSendAck(&con->ogBcm);
        }
        ret = ccProcessUpdateRsp(con, icProtType, ogProtType,
                                 (CcAllSdus*)event);
        if (ret == ROKIGNORE)
        {
          RETVALUE (ROK);
        }
      }
      /* BUG:93414 Let Re-Invite and Update can be handled */
      if ((con->direction == INCTOOUT) &&
          (ccIsUpdateRcvd(icProtType, ogProtType, con->icEvntType,con->direction) ||
           ccIsXferRcvd(icProtType,ogProtType, con->icEvntType, con->direction)))
      {
      if ((con->direction == INCTOOUT) && 
           ccIsUpdateRcvd(icProtType, ogProtType, con->icEvntType,
                            con->direction))
      {
         ret = ccProcessUpdate(con, icProtType,ogProtType, (CcAllSdus*)event);
      }
      if((con->direction == INCTOOUT) &&
         (ccIsXferRcvd(icProtType,ogProtType, con->icEvntType, con->direction)))
      {
         ret = ccHandleXfer(con, icProtType, ogProtType,
                            con->direction, event);
      }
      /* For ccProcessUpdate even there is mg action the retValue will
       * be ROKIGONORE and will store event;
       * while for Xfer, will be ROKPENDING and not store event*/
      if (con->state == CCS_AWT_MGI_XFERQRYHOLD)
      {
         if (ret == ROKPENDING)
         {
            if ((!CC_IS_CALL_MULTI_MG(con)) ||
                  (CC_IS_CALL_MULTI_MG(con) && CC_OPERATING_FIRST_MG(con)))
            {
               ccStoreEvnt(con->evntType, con->icEvntType, con, (U8 *)event,
                     sizeof(CcCnStEvnt));
            }
         }
         RETVALUE(ROK);
      }
      /* For no mg switching action case, need to map this
       * update/re-invite out */
      con->ogEvnt = (CcAllSdus *) &ogEvnt;
      if ((con->direction == INCTOOUT) &&
            ccIsUpdateRcvd(icProtType, ogProtType, con->icEvntType,
                           con->direction))
      {
         CCDP(AIN_DBG_LEVEL_1, "Will convert this UPDTE to Re-INVITE!\n");
         ccConvertPostAnsUpdate(con);
      }

      ret = ccMapEvent(con, CCE_CNSTIND, con->icEvntType);
     
      if (ret == CCIGNOREMAPPING)
      {
         ccDropUBuf(&con->uBuf);
         RETVALUE(ROK);
      }
      else if (ret != ROK)
      {
         ccStopConTmr(con, CC_TMR_CALLDTL);
         ccStopConTmr(con, TMR_SETUP);
         ccStartConTmr(TMR_RELEASE, con, (PTR)&ccCp.genCfg);

         /* Release all connections */
         ccDropUBuf(&con->uBuf);
         ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
         RETVALUE(ROK);
      }
      uBuf = con->uBuf;
      con->uBuf = NULLP;
      if (con->direction == INCTOOUT)
      {
         ccSendCnStReq(con, con->ogSapCb->suId, &con->ogSapCb->pst,
               con->ogSapCb->spId, con->ogSpConnId, con->ogSuConnId,
               con->ogProtType, con->ogEvntType, &ogEvnt, uBuf);
      }
      else
      {
         ccSendCnStReq(con, con->icSapCb->suId, &con->icSapCb->pst,
               con->icSapCb->spId, con->icSpConnId, con->icSuConnId,
               con->icProtType, con->ogEvntType, &ogEvnt, uBuf);
      }

      RETVALUE(ROK);
      }
  } 
 
  RETVALUE(ROK);
} /* End of ccConE03S155 */

/* 
*  
*      Fun:   ccConE04S155
*
*      Desc:  Connection state function
*             event - CCE_RELIND 
*             state - CCS_AWT_MRFRSP 
*      Ret:   ROK     - successful,
*             RFAILED - unsuccessful
*
*      Notes: None.
*
*      File:  cc_bdy2_2.c
*
*/
PUBLIC S16 ccConE04S155 
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                    /* bcm */
PTR     event2                    /* event */
)
{
   S16     ret = RFAILED;   
   CCDP(AIN_DBG_LEVEL_1, "%s: icSu:0x%lx\n", __FUNCTION__,con->icSuConnId);
   /*Need to first SUB the resource if needed  */
   ccStopConTmr(con, AIN_O_NO_ANS_TMR);
    ccStopConTmr(con, AIN_T_NO_ANS_TMR);
    /* STOP the AWAIT ANSWER Timer if running
     *
     * changes made to Support Configurable Answer Timer in GCC, RV Suresh, 5.0
     */
    ccStopConTmr(con, CC_TMR_AWAITANS);
    ccStopConTmr(con, TMR_SETUP);
    ccDeleteBufferedEvent(con, con->icSuConnId, CCE_ONOANSEXP);
    ccDeleteBufferedEvent(con, con->ogSuConnId, CCE_TNOANSEXP);
#if 1 /* EARLY_ACM */
    ccStopConTmr(con, CC_TMR_EARLYACM);
#endif
    CCDP(AIN_DBG_LEVEL_1,"mrfFlag%d\n", con->mrfFlag);

   if(con->direction == OUTTOINC) 
   {
    if ((ret = ccStoreEvnt(con->evntType, con->icEvntType, con, (U8 *)event,
            sizeof(CcRelEvnt))) != ROK)
    {
      CCLOGERR( "ERROR: ccStoreEvnt()failed. Continuing with call\n");
    }

    if (CC_IS_CONTEXT_PRESENT(con))
    {
      ccQSubBasedOnCcn(con);  /* FID 15261.0 */
      if (CC_IS_MRF(con->mrfFlag,MRF_PLAYING_ANNC) &&
          CC_IS_MRF(con->mrfFlag, MRF_200_ANNC)) /* 200OK received */
        con->relPend = OUTRELRSPANDINTONE;
      else
        con->relPend = OUTRELRSPANDINREL;

      CC_STATE_CHANGE(con, CCS_AWTSWTCFM_SENDONEREL);
      con->mgBlock->con = con;
      ret = ccSendMgiQueue(0, con->mgBlock);
      if (ret != ROK)
      {
        CCLOGERR( "Error: ccSendMgiQueue failed\n");
        cleanUpMgQ(con);
        ccCleanUpMGCtx(con);
        ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
        RETVALUE(RFAILED);
      }
      RETVALUE(ROK);
    }
    if ( CC_IS_MRF(con->mrfFlag,MRF_PLAYING_ANNC) &&
        CC_IS_MRF(con->mrfFlag, MRF_200_ANNC)) /* 200OK received */
    {
      /* Annc completed */
      CCDP(AIN_DBG_LEVEL_0,"MRF ANNC completed\n");
      ccChangeCallStateOfAssociatedCalls(&(con->icBcm), CCS_CANNOTPROCESSEVENTS);

      ccDeallocateResource(con, CC_OUTGOING);
      ccSendReleaseRsp(con);
      con->ogRscVal = FALSE;

      ret = ccPostAnnoucementProcessing(con);
      if (ret != ROK)
      {
        CCLOGERR("Error: ccPostAnnoucementProcessing failed 0x%lx\n",
            con->icSuConnId);
        cleanUpMgQ(con);
        ccHandleCleanUpConCb(con, TRUE, AM_CV_TMPFAIL);
#if 0
        /* Don't reset the mrfFlag for debug during error case?? */
        con->mrfFlag = 0;
#endif
        RETVALUE(RFAILED);
      }
      /* Since we are done with tha annoucement, need to clean the anncBlk */
      cmMemset((U8 *)&con->anncBlock, 0, sizeof(con->anncBlock));
    }
    else
    {
      /* Since we are done with tha annoucement, need to clean the anncBlk */
      cmMemset((U8 *)&con->anncBlock, 0, sizeof(con->anncBlock));
      /* release the call with 4xx-6xx  message */
      CCDP(AIN_DBG_LEVEL_0,"MRF response with non-200OK message\n");
      ccProcessFailCnd(con, FC_MRF_NOTPLAYANNC, TRUE);
      /*Need to ensure BYE after INVITE is handled correctly */
    }
    /* Please do NOT return without reset the mrfFlag to 0 */
    con->mrfFlag = 0;
   }
   else if(con->direction == INCTOOUT)
   {
       CCDP(AIN_DBG_LEVEL_0,"orignating party release call \n");

       con->mrfFlag = 0; /* reset the flag here */

       ccStopConTmr(con, TMR_SETUP);
       ccStopConTmr(con, CC_TMR_EARLYACM);
       ccStopConTmr(con, CC_TMR_CALLDTL);
       ccStartConTmr(TMR_RELEASE, con, (PTR)&ccCp.genCfg);

       /* Since we are done with tha annoucement, need to clean the anncBlk */
       cmMemset((U8 *)&con->anncBlock, 0, sizeof(con->anncBlock));
       /* FID 14732 */
       ccExtractSipwMBufFromRel(con, CCE_RELIND, (PTR) event);
       /* FID 14732 */

       if ((ret = ccStoreEvnt(con->evntType, con->icEvntType, con, (U8 *)event,
                             sizeof(CcRelEvnt))) != ROK)
       {
           CCLOGERR( "ERROR: ccStoreEvnt()failed. Continuing with call\n");
       }
       
       if (CC_IS_CONTEXT_PRESENT(con))
       {
          ccQSubBasedOnCcn(con);  /* FID 15261.0 */
          /* I think the relPend shuld be INRELRSPANDOUTREL */
          con->relPend = INRELRSPANDOUTREL;
          
          CC_STATE_CHANGE(con, CCS_AWTSWTCFM_SENDONEREL);
          con->mgBlock->con = con;
          ret = ccSendMgiQueue(0, con->mgBlock);
          if (ret == ROK)
             RETVALUE(ROK);
          else
          {
             CCLOGERR("Error: During switching icSu = 0x%lx\n", con->icSuConnId);
             cleanUpMgQ(con);
             ccCleanUpMGCtx (con);
          }
       }

       if (con->noRspReqd == FALSE) 
       {                       
           ccSendReleaseRsp(con);
       }                       
       CC_STATE_CHANGE(con, CCS_AWTDEALOCCFM);
       if(con->icRscVal == TRUE)
       {
           ccDeallocateResource(con, CC_INCOMING);
       }
       else if (con->ogRscVal == TRUE) 
       {
           ccDeallocateResource(con, CC_OUTGOING);
       }
       else if ((con->icRscVal == TRUE)&&(con->ogRscVal == TRUE))
       {
           ccDeallocateResource(con, CC_BOTH);    
       }
       else
       {
               CC_STATE_CHANGE(con, CCS_IDLE);
               ccRelCon(con);
       }
       RETVALUE(ROK);
   }
   else
   {
      CCLOGERR("Error: incorrect direction\n");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /* End of ccConE04S155 */


/* FID 17174 Annc Support via External MRF - */

/* FID 17555.0 + Kavon */
/*
 *
 *      Fun:   ccConE02S156
 *
 *      Desc:  Connection state function
 *             event - CCE_ADDRIND 
 *             state - AWTSWTCFM_FOR_DIALINGTONE
 *      Ret:   ROK     - successful,
 *             RFAILED - unsuccessful
 *
 *      Notes: None.
 *
 *      File:  cc_bdy2_2.c
 *
*/
PUBLIC S16 ccConE02S156
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                   /* bcm */
PTR     event2                   /* event */
)
{
   CCDP(AIN_DBG_LEVEL_2, "Entering ccConE02S156\n");

   if (con->icProtType == CC_INETSI)
   {
      ccBufferEventToQueue(con, bcm, event, CCE_ADDRIND, sizeof(CcCnStEvnt), 0,
                           NULLP, 0);
   }
   RETVALUE(ROK);
}

/*
 *
 *      Fun:   ccConE03S156
 *
 *      Desc:  Connection state function
 *             event - CCE_CNSTIND 
 *             state - AWTSWTCFM_FOR_DIALINGTONE
 *      Ret:   ROK     - successful,
 *             RFAILED - unsuccessful
 *
 *      Notes: None.
 *
 *      File:  cc_bdy2_2.c
 *
*/
PUBLIC S16 ccConE03S156
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                   /* bcm */
PTR     event2                   /* event */
)
{
   CCDP(AIN_DBG_LEVEL_2, "Entering ccConE03S156\n");

   if (con->icProtType == CC_INETSI &&
       con->icEvntType == CC_ET_NMINFO)
   {
      ccBufferEventToQueue(con, bcm, event, CCE_CNSTIND, sizeof(CcCnStEvnt), 0,
                           NULLP, 0);
   }
   RETVALUE(ROK);
}

/*
 *
 *      Fun:   ccConE04S156
 *
 *      Desc:  Connection state function
 *             event - CCE_RELIND 
 *             state - AWTSWTCFM_FOR_DIALINGTONE
 *      Ret:   ROK     - successful,
 *             RFAILED - unsuccessful
 *
 *      Notes: None.
 *
 *      File:  cc_bdy2_2.c
 *
*/
PUBLIC S16 ccConE04S156
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                   /* bcm */
PTR     event2                   /* event */
)
{
   CCDP(AIN_DBG_LEVEL_2, "Entering ccConE04S156\n");

   if (con->icProtType == CC_INETSI)
   {
      ccBufferEventToQueue(con, bcm, event, CCE_RELIND, sizeof(CcRelEvnt), 0,
                           NULLP, 0);
   }
   RETVALUE(ROK);
}

/*
 *
 *      Fun:   ccConE11S156
 *
 *      Desc:  Connection state function
 *             event - Switching Connect Confirm
 *             state - AWTSWTCFM_FOR_DIALINGTONE
 *      Ret:   ROK     - successful,
 *             RFAILED - unsuccessful
 *
 *      Notes: None.
 *
 *      File:  cc_bdy2_2.c
 *
*/
PUBLIC S16 ccConE11S156
(
CcConCb *con,                    /* connection */
PTR     event,                   /* event */
AinBCM_t *bcm,                   /* bcm */
PTR     event2                   /* event */
)
{
   S16           ret        = RFAILED;
   MgctTxnCfm   *txnCfm     = NULLP;

   CCDP(AIN_DBG_LEVEL_2, "Entering ccConE11S156\n");

   txnCfm = (MgctTxnCfm *)event;
   ret = ccHandleMgResponse(con, txnCfm);
   if (ret != ROK)
   {
      CCLOGERR("ERROR: Response Handling FAILED\n");
      cleanUpMgQ(con);
      ccHandleCleanUpConCb(con, TRUE, CCTMPFAIL);
      RETVALUE(RFAILED);
   }

   switch(con->lastState)
   {
      /* play dialing tone response */
      case CCS_AWTROUTERSC:
         CCDP(AIN_DBG_LEVEL_1, "playing dialing tone response\n");

         CC_STATE_CHANGE(con, CCS_AWTMOREINFO);
         if (ccIsRscRspReqd(con))
         {
            CcLiCctRscRsp(&con->icSapCb->pst, con->icSapCb->spId,
                           con->icSpConnId, con->icSuConnId, &con->icRsc,
                           con->STDigitRecd);
         }
         break;
      /* stop dialing tone response */
      case CCS_AWTMOREINFO:
         CCDP(AIN_DBG_LEVEL_1, "stop dialing tone response\n");

         con->lastState = CCS_AWTROUTERSC;
         if (con->waitForMoreInfo & 0x02)
         {
            CC_STATE_CHANGE(con, CCS_AWTMOREINFO);
            RETVALUE(ROK);
         }
         else
         {
            con->STDigitRecd = TRUE;
            ccNoMoreInfoAndContinue(con);
         }
         break;
      default:
         break;
   }

   RETVALUE(ROK);
}
/* FID 17555.0 - */
/********************************************************************60**
        Revision history:
*********************************************************************61*/

/********************************************************************90**
 $Log: cc_bdy2_2.c,v $
 Revision 1.602  2015/07/29 01:54:08  jfan
 BUG:94143

 Revision 1.601  2015/07/20 02:57:07  aliu002
 BUG:93414: FID17174.0 enhancement: Update and Re-INVITE not supported when using MRF for announcement playing

 Revision 1.600  2015/02/04 06:32:56  aliu002
 BUG:93493 Move the DP

 Revision 1.599  2015/01/20 05:38:15  aliu002
 BUG:93368 Enhance when play annc using local mg, re-invite/update can be processed

 Revision 1.598  2015/01/07 13:55:58  yuqinzhang
 bug:93241 FID17373 GCC reroute

 Revision 1.597  2015/01/06 07:57:37  yuqinzhang
 bug:93241 FID17373 GCC reroute

 Revision 1.596  2014/12/29 07:31:07  yuqinzhang
 bug:93241 FID17373 GCC reroute no COT

 Revision 1.595  2014/12/25 06:50:06  yuqinzhang
 bug:93241 FID17373 GCC reroute

 Revision 1.594  2014/12/22 08:48:58  yuqinzhang
 bug:93241 FID17373 GCC reroute

 Revision 1.593  2014/12/17 08:02:47  yuqinzhang
 bug:93241 FID17373 GCC inter-MG

 Revision 1.592  2014/12/17 08:00:51  yuqinzhang
 bug:93337 no additional MOD

 Revision 1.591  2014/11/26 11:28:31  yuqinzhang
 bug:93241 FID17373 GCC COT Dev

 Revision 1.590  2014/11/25 08:59:50  yuqinzhang
 bug:93241 FID17373 GCC COT Dev

 Revision 1.589  2014/11/25 08:28:07  yuqinzhang
 bug:93241 FID17373 GCC COT Dev

 Revision 1.588  2014/11/25 08:09:07  yuqinzhang
 bug:93241 FID17373 GCC COT Dev

 Revision 1.587  2014/11/25 07:21:56  yuqinzhang
 bug:93241 FID17373 GCC COT Dev

 Revision 1.586  2014/11/25 06:20:01  ashi
 BUG:93262

 Revision 1.585  2014/11/25 05:53:21  brjiang
 BUG:93241 COT related code change

 Revision 1.584  2014/11/05 08:58:24  kavonm
 BUG:93148

 Revision 1.583  2014/10/31 04:36:44  kavonm
 BUG:93148 FID17224 Dev

 Revision 1.582  2014/10/14 06:33:38  sahuang
 Bug:93121

 Revision 1.581  2014/10/14 06:01:36  sahuang
 Bug:93121

 Revision 1.580  2014/09/29 06:07:23  aliu002
 BUG:92866 FID17174.0 Fix the two MRF ansInd =y, first no MG then MG, 183 trigger no update

 Revision 1.579  2014/09/26 03:19:27  aliu002
 BUG:92866 Fix the agnostic no sdp issue

 Revision 1.578  2014/09/12 07:26:19  aliu002
 BUG:92866 FID17174.0 Fix the ACK issue

 Revision 1.577  2014/09/10 08:20:16  aliu002
 BUG:92866 Fix the pm counter issue

 Revision 1.576  2014/09/01 02:54:33  guohuaf
 BUG:92866 FID17174.0 ignore 200OK after reliable 183

 Revision 1.575  2014/08/15 07:16:48  aliu002
 BUG:92866 FID17174.0 Fix the PRACK and UPDATE issue

 Revision 1.574  2014/08/13 13:13:13  guohuaf
 BUG:92866 FID17174.0 ANNC with MRF

 Revision 1.573  2014/08/13 07:41:48  conniech
 BUG:92782 FID16970.0 PRA CCBS support on MGC-8

 Revision 1.572  2014/08/12 14:31:15  guohuaf
 BUG:92866 FID17174.0 send PRACK ACK UPDATE

 Revision 1.571  2014/08/12 10:10:44  ashi
 BUG:92782

 Revision 1.570  2014/08/12 02:58:53  guohuaf
 BUG:92866 FID17174.0 play annc via MRF

 Revision 1.569  2014/08/08 08:35:17  aliu002
 BUG:92866 FID17174.0 Fix the sdp off-ans and no rel to og issue

 Revision 1.568  2014/08/07 08:49:56  guohuaf
 BUG:92866 FID17174.0 play annc via MRF

 Revision 1.567  2014/08/07 02:36:34  guohuaf
 BUG:92866 FID17174.0 play annc via MRF

 Revision 1.566  2014/08/06 08:46:49  guohuaf
 BUG:92866 FID17174.0 play annc via MRF

 Revision 1.565  2014/08/06 03:35:04  guohuaf
 BUG:92866 FID17174.0 play annc via MRF

 Revision 1.564  2014/08/06 01:40:53  guohuaf
 BUG:92866 FID17174.0 play annc via MRF

 Revision 1.563  2014/08/05 15:24:46  guohuaf
 BUG:92866 FID17174.0 play annc via MRF

 Revision 1.562  2014/08/04 09:01:01  guohuaf
 BUG:92866 FID17174.0 play annc via MRF

 Revision 1.561  2014/08/04 02:31:26  guohuaf
 BUG:92866 FID17174.0 play annc via MRF

 Revision 1.560  2014/08/04 02:19:58  guohuaf
 BUG:92866 FID17174.0 play annc via MRF

 Revision 1.559  2014/08/01 12:27:43  guohuaf
 BUG:92866 FID17174.0 play annc via MRF

 Revision 1.558  2014/08/01 07:37:38  guohuaf
 BUG:92866 FID17174.0 play annc via MRF

 Revision 1.557  2014/07/31 13:35:51  guohuaf
 BUG:92866 FID17174.0 play annc via MRF

 Revision 1.556  2014/07/31 08:20:15  aliu002
 BUG:92866 FID17174.0 Fix the mg case

 Revision 1.555  2014/07/31 08:01:15  aliu002
 BUG:92866 FID17174.0 Fix the normal playannc with mg case

 Revision 1.554  2014/07/30 13:01:13  guohuaf
 BUG:92866 FID17174.0 play annc via MRF

 Revision 1.553  2014/07/30 12:56:44  guohuaf
 BUG:92866 FID17174.0 play annc via MRF

 Revision 1.552  2014/07/30 12:01:59  aliu002
 BUG:92866 FID17174.0 Fix the scenario MG inserted before play announcement

 Revision 1.551  2014/07/29 10:51:06  brjiang
 BUG:92782

 Revision 1.550  2014/07/29 10:20:27  brjiang
 BUG:92782 modification according to comments

 Revision 1.549  2014/07/28 10:17:05  guohuaf
 BUG:92866 FID17174.0 play annc via MRF

 Revision 1.548  2014/07/28 08:12:10  aliu002
 BUG:92866 FID17174.0 REL case handling

 Revision 1.547  2014/07/25 11:48:05  guohuaf
 BUG:92866 FID17174 0 play annc via MRF

 Revision 1.546  2014/07/25 06:48:13  aliu002
 BUG:92866 FID17174.0 play annc via MRF

 Revision 1.545  2014/07/24 08:41:42  brjiang
 Bug:92782 FID 16970.0

 Revision 1.544  2014/07/24 04:39:03  brjiang
 Bug:92782 FID 16970.0

 Revision 1.543  2014/06/30 01:45:26  yuqinzhang
 BUG:92780 diff detection leg

 Revision 1.542  2014/06/26 07:49:14  yuqinzhang
 BUG:92833 not send response

 Revision 1.541  2014/06/20 14:35:08  denburg
 BUG:92809

 Revision 1.540  2014/06/15 03:21:00  aliu002
 BUG:92355 FID16901

 Revision 1.539  2014/05/07 07:05:50  sahuang
 BUG:92163 FID16692.0

 Revision 1.538  2014/04/21 02:33:31  sahuang
 BUG:92163 FID16692.0

 Revision 1.537  2014/04/17 19:02:35  krt
 BUG:92471 Keep CRBT call inserted for SIP to BICC IP Coversion

 Revision 1.536  2014/04/17 07:11:52  aliu002
 BUG:92355 FID16901.0 Brazil INAP support - IN services SCM, Free-Phone, CTB

 Revision 1.535  2014/04/08 03:38:50  jfan
 BUG:92455

 Revision 1.534  2014/03/27 02:52:53  yuqinzhang
 BUG:92507 debug core for update in release

 Revision 1.533  2014/03/19 08:31:44  zhangbengchen
 BUG:92460 fix ccs cored when making SIP2ISUP precondition CRBT call

 Revision 1.532  2014/03/11 08:36:24  yuqinzhang
 BUG:91897 GCC codes for LI

 Revision 1.531  2014/03/10 08:51:47  yuqinzhang
 BUG:91897 GCC codes for PTA

 Revision 1.530  2014/03/06 05:03:49  guohuaf
 BUG:92277

 Revision 1.529  2014/02/02 07:58:32  yuqinzhang
 BUG:91897 GCC codes for re-route to IP

 Revision 1.528  2014/01/17 09:51:55  yuqinzhang
 BUG:91897 GCC codes for re-route

 Revision 1.527  2014/01/15 07:47:59  zhangbengchen
 BUG:92218 fix ccs cored when making SIP2ISUP CRBT call

 Revision 1.526  2014/01/09 09:58:17  yuqinzhang
 BUG:91897 GCC codes for inter-ccs and inter-mg

 Revision 1.525  2013/12/26 13:27:10  aliu002
 BUG:92023 FID17160.0 Support Stereo to mono conversion

 Revision 1.524  2013/12/24 09:43:34  yuqinzhang
 BUG:91897 GCC codes for bugFix&inter-ccs

 Revision 1.523  2013/12/13 07:33:46  yuqinzhang
 BUG:91897

 Revision 1.522  2013/12/05 03:09:38  yuqinzhang
 BUG:91897 GCC codes

 Revision 1.521  2013/11/22 06:05:45  yuqinzhang
 BUG:91897

 Revision 1.520  2013/11/22 04:42:10  yuqinzhang
 BUG:91897 GCC/RM codes

 Revision 1.519  2013/10/25 21:41:54  cmykulyn
 BUG: 91702

 Revision 1.518  2013/07/19 03:10:18  guohuaf
 BUG:91551

 Revision 1.517  2013/06/28 06:35:02  rowenw
 BUG:91532

 Revision 1.516  2013/06/18 06:17:03  jfan
 BUG:91501

 Revision 1.515  2013/05/16 14:59:50  nnaik
 bug:91414 On No-Ans Re-route clear the tempAT from previous ACTNTERMATTMPT

 Revision 1.514  2013/03/14 07:08:44  jfan
 BUG:91115

 Revision 1.513  2013/02/05 07:26:14  jfan
 BUG:90369

 Revision 1.512  2012/12/20 07:53:03  dxie
 BUG: 90482

 Revision 1.511  2012/12/18 09:52:01  yuqinzhang
 BUG:90482 FID16747.0 Early ACM support

 Revision 1.510  2012/12/12 07:30:40  yuqinzhang
 BUG:90482 FID16747.0 Set to PCMU in APM for Fast FWD

 Revision 1.509  2012/12/10 09:12:10  dxie
 BUG: 90482 procedure for bicc-isdn call

 Revision 1.508  2012/12/10 06:33:07  aliu002
 BUG:90363 FID16883 GCC part

 Revision 1.507  2012/10/19 01:54:42  dxie
 BUG: 90408 handle Prack of early 183 for announcement

 Revision 1.506  2012/08/23 07:38:34  rowenw
 BUG:89867 GCC code change for supporting rtrv-fraudcalls when alarms are not generated.

 Revision 1.505  2012/08/14 08:25:32  zhangbengchen
 BUG:90096 Don't perform state change action in function ccConE92S09

 Revision 1.504  2012/07/31 07:26:20  brjiang
 BUG:89905

 Revision 1.503  2012/07/27 05:52:05  yuqinzhang
 BUG:89867 - GCC Throttling fraud alarm comments

 Revision 1.502  2012/07/25 01:23:22  yuqinzhang
 BUG:89867 - GCC Generate/Clear/Throttling fraud alarm comments

 Revision 1.501  2012/07/20 08:08:27  yuqinzhang
 BUG:89867 - GCC Generate/Clear/Throttling fraud alarm

 Revision 1.500  2012/07/18 08:21:07  kajin
 BUG:89925 SIP2BICC involving two MGWs fails because BICC event is armed on internal IP termination

 Revision 1.499  2012/05/18 08:36:56  jx
 BUG: 89286 fix IAM handling issue after CRM

 Revision 1.498  2012/04/23 02:09:34  zhangbengchen
 BUG:88752

 Revision 1.497  2012/04/18 02:42:49  ejin
 BUG:88866 For FID16206.0: Service on PRA: CFx control procedure.

 Revision 1.496  2012/04/16 08:51:28  jianxueqd1
 Bug:88989: fix issue in coverity

 Revision 1.495  2012/04/12 13:59:51  kumarb
 BUG:88572, LGP-3838 checkin

 Revision 1.494  2012/04/12 08:55:17  rowenw
 BUG:88723 Fix 488 release call issue

 Revision 1.493  2012/03/30 08:12:10  hong
 BUG: 88924

 Revision 1.492  2012/03/13 02:12:56  ashi
 BUG:88826

 Revision 1.491  2012/03/01 05:35:29  rowenw
 BUG:88723 Fax revert to audio

 Revision 1.490  2012/02/28 02:22:52  pattonwa
 BUG:87956 backout FID:16446 codes

 Revision 1.489  2012/02/10 17:42:20  jjkott
 Bug:88275 CRBT Fix

 Revision 1.488  2012/02/07 18:52:18  jjkott
 Bug:88275 CRBT Fix

 Revision 1.487  2012/01/30 05:10:48  yuqinzhang
 BUG:88358 call flow fix

 Revision 1.486  2012/01/18 19:23:15  jjkott
 Bug:88230 PRACK SDP Offer Fix

 Revision 1.485  2012/01/16 01:04:09  rowenw
 BUG:88358 Re-invite support

 Revision 1.484  2012/01/10 12:37:33  aliu002
 BUG:87956 Fix the glare case for 491 issue

 Revision 1.483  2012/01/05 08:36:21  rowenw
 BUG:88358

 Revision 1.482  2011/12/29 05:17:51  aliu002
 BUG:87956 Remove VBD line

 Revision 1.481  2011/12/28 13:50:50  aliu002
 BUG:87956 Remove VBD line

 Revision 1.480  2011/12/26 13:36:00  aliu002
 BUG:87956 Fix two ccs two mg issue

 Revision 1.479  2011/12/20 12:06:40  aliu002
 BUG:87956 For LGP-2317 and LGP-2318

 Revision 1.478  2011/12/09 16:40:06  hollyj
 BUG:88204

 Revision 1.477  2011/12/07 20:35:57  hollyj
 BUG:88027

 Revision 1.476  2011/12/05 21:59:37  krt
 BUG:88025 Fix Internal Queue Indications

 Revision 1.475  2011/12/03 13:31:19  krt
 BUG:88025 Fix Queue Indications

 Revision 1.474  2011/11/30 08:17:12  valu
 BUG:87715 QD system SQA 8.3 FID 15737 After answer re-route call failed when intercepted by SORM3

 Revision 1.473  2011/11/29 03:42:41  weiweiqd1
 BUG:88040: COT handling for SORM call

 Revision 1.472  2011/11/21 16:15:01  hollyj
 BUG:88027 E03S56 drop prack

 Revision 1.471  2011/11/18 02:04:07  hollyj
 BUG:88027 E03S150 fix

 Revision 1.470  2011/11/10 04:30:34  dxie
 BUG: 87950

 Revision 1.469  2011/11/03 08:28:25  qmeng
 bug:87903 FID14841 0 adid package support

 Revision 1.468  2011/09/30 20:42:44  kumarb
 BUG:85490, LGP-233 story changes for default reroute scenario

 Revision 1.467  2011/09/30 17:21:56  kumarb
 BUG:87579, Memory leak fix for calls that result in cutting CDR prior to reroute

 Revision 1.466  2011/09/22 02:58:48  songleiwang
 Bug:87494

 Revision 1.465  2011/09/20 12:52:02  aliu002
 BUG:87481 No IRI_END

 Revision 1.464  2011/09/16 17:47:34  kumarb
 BUG:87485, Fix Klocworks issues in FID 15801.0 code

 Revision 1.463  2011/09/01 16:25:33  scrumbot
 BUG: 85490 -- scrumbot sprint 12 check-in

 Revision 1.462  2011/09/01 14:48:47  peizeng
 BUG: 86318 GET-PM

 Revision 1.461  2011/08/24 15:07:14  krt
 BUG:86318 FID 14457.0

 Revision 1.460  2011/08/22 22:58:15  hollyj
 BUG:86318 LGP1513 HPC MG Retry for TGW

 Revision 1.459  2011/08/15 17:36:48  scrumbot
 BUG: 85490 -- FID 15801 Sprint 11 check-in

 Revision 1.458  2011/08/12 13:20:42  krt
 BUG:86318 FID 14457.0 GETS

 Revision 1.457  2011/08/11 13:02:41  krt
 BUG:86318 FID 14457.0 GETS

 Revision 1.456  2011/08/09 22:15:34  hollyj
 BUG:86318 GCC HPC MG RETRY

 Revision 1.455  2011/08/02 05:10:32  ashi
 BUG:86942

 Revision 1.454  2011/07/30 06:00:35  hollyj
 BUG:86318 Ingress MG List Advance

 Revision 1.453  2011/07/29 12:53:07  krt
 BUG:86318 FID 14457.0 GETS

 Revision 1.452  2011/07/27 19:33:36  krt
 BUG:86318 FID 14457.0 GETS

 Revision 1.451  2011/07/26 18:56:00  krt
 BUG:86318 FID 14457.0 GETS

 Revision 1.450  2011/07/26 12:23:54  krt
 BUG:86318 GETS FID 14457.0

 Revision 1.449  2011/07/22 01:39:34  rowenw
 BUG:86824

 Revision 1.448  2011/07/14 17:47:19  scrumbot
 BUG: 85490 -- FID 15801 Sprint 9 lGP-74 checkin

 Revision 1.447  2011/07/06 21:20:21  krt
 BUG:86318 FID 14457.0 GETS

 Revision 1.446  2011/06/21 04:25:11  aliu002
 BUG:86481 Fix the COT issue for SORM

 Revision 1.445  2011/06/17 02:03:18  aliu002
 BUG:86481 Miss a checkin to send ogBcm's mgCtx mgCmdQ

 Revision 1.444  2011/06/14 01:28:21  aliu002
 BUG:86481 Fix SORM and FID15261 interaction issue for COT calls

 Revision 1.443  2011/06/10 17:38:35  ajayk
 BUG: 86451

 Revision 1.442  2011/06/09 18:59:22  ajayk
 BUG: 86355

 Revision 1.441  2011/06/07 14:00:07  ajayk
 BUG: 86363

 Revision 1.440  2011/06/02 08:29:41  zhangbengchen
 BUG:86264 Ignore the Continue event w/o err code in state CCS_AWTRELCFM_IC

 Revision 1.439  2011/05/25 01:21:18  yuqinzhang
 BUG:86040 ignore fax handling

 Revision 1.438  2011/05/24 07:09:58  yuqinzhang
 BUG:86040 ignore fax handling

 Revision 1.437  2011/05/23 17:17:34  ajayk
 BUG: 86014

 Revision 1.436  2011/05/18 02:36:01  yuqinzhang
 BUG:86040 ignore fax handling

 Revision 1.435  2011/05/12 17:01:08  jjkott
 Bug:85824 Fallback Fix

 Revision 1.434  2011/05/06 02:32:06  yuqinzhang
 BUG:86040 ignore fax handling

 Revision 1.433  2011/05/02 19:02:11  ajayk
 BUG: 85868

 Revision 1.432  2011/04/22 20:48:26  kumarb
 BUG: 85490

 Revision 1.431  2011/04/07 15:16:53  rowenw
 BUG:85615 Fix Unit Test issues

 Revision 1.430  2011/04/05 20:46:52  kumarb
 BUG:85490, FID 15801 - changing the name of the new state

 Revision 1.429  2011/04/05 20:28:27  kumarb
 BUG:85490, FID 15801 GCC internal data structure changes

 Revision 1.428  2011/03/31 16:55:40  rowenw
 BUG:85615 FID 16087.0 SDP Enhancement GCC code

 Revision 1.427  2011/03/31 15:01:29  ajayk
 BUG: 84921

 Revision 1.426  2011/03/31 12:42:00  yongz
 BUG: 85326 FID 15903.0 GCC Code

 Revision 1.425  2011/03/25 18:22:37  ajayk
 BUG: 84921

 Revision 1.424  2011/03/24 16:05:30  krt
 BUG:85554 Fix release cause in X2 IRI END for late call forwarding scenario

 Revision 1.423  2011/03/11 16:45:00  ajayk
 BUG: 85423

 Revision 1.422  2011/03/03 13:55:17  dxie
 BUG: 83948 rework on the fix for Prack offer

 Revision 1.421  2011/02/18 08:15:24  yuqinzhang
 BUG:84999 FID15913.0 GCC Codes

 Revision 1.420  2011/02/18 06:41:32  dayongww
 Bug:84113: fix a possiable issue in code

 Revision 1.419  2011/02/17 08:22:07  aliu002
 BUG:84498 Fix the new issue

 Revision 1.418  2011/02/11 08:52:00  aliu002
 BUG:84749 Fix the SRP busy and ETC failed with no response from SCP

 Revision 1.417  2011/01/30 15:52:19  dayongww
 Bug:83726: fix an issue caused double free of callId

 Revision 1.416  2011/01/25 07:08:05  chuanyiyu
 bug:84857

 Revision 1.415  2011/01/24 04:12:21  dxie
 BUG: 82493 there's interaction caused by 15083.0 on main

 Revision 1.414  2011/01/20 15:55:40  dxie
 BUG: 84842

 Revision 1.413  2011/01/19 02:48:46  dxie
 BUG: 84781 fix issue where virtual call involve MG

 Revision 1.412  2011/01/17 14:06:07  hong
 BUG: 83940 FID 15319.2

 Revision 1.411  2011/01/13 03:12:25  aliu002
 BUG:84642 Fix the O_No_Ans_Tmr issue

 Revision 1.410  2011/01/03 22:02:49  ajayk
 BUG: 84409

 Revision 1.409  2011/01/01 06:09:10  aliu002
 BUG:84505 Fix the DFC+ReleaseCall for SCP->SRP core

 Revision 1.408  2010/12/30 00:45:45  chuanyiyu
 bug:84005

 Revision 1.407  2010/12/22 19:36:58  zhangbengchen
 BUG: 84380 sends back the 200 ok Prack response.

 Revision 1.406  2010/12/22 01:43:14  chuanyiyu
 bug:84005

 Revision 1.405  2010/12/10 03:11:03  aliu002
 BUG:84155 Fix the standby ADCM fault issue for ICA call

 Revision 1.404  2010/12/05 10:14:42  nzhen001
 BUG:83591 FID15310.1 ICA Fix the ISDN setup message usrSrvcInfo issue

 Revision 1.403  2010/12/03 16:08:19  kumarb
 BUG:84026, IPV6 MG pruning for SIP to TDM + fixing 8.1 mgwAdvance issues

 Revision 1.402  2010/12/02 05:56:18  dxie
 BUG: 83948 drop Prack for inter-ccs

 Revision 1.401  2010/11/26 02:36:54  nzhen001
 BUG:83591 FID15310.1 ICA - CC part

 Revision 1.400  2010/10/29 03:17:33  zhangbengchen
 Bug: 83277 FID 15041.2 MGC-8 ETSI-LI Support for CHT-I

 Revision 1.399  2010/10/25 21:56:56  sunj
 BUG:80547 set relPend to NRELRSPANDOUTREL for ingress release case

 Revision 1.398  2010/10/22 20:45:33  hollyj
 BUG:81422 DT bug fix

 Revision 1.397  2010/10/15 07:32:54  dxie
 BUG: 82786

 Revision 1.396  2010/10/08 19:41:54  krt
 BUG:82408 Fix bug carryforward from 8.1 for SORM 2

 Revision 1.395  2010/10/08 17:06:01  ajayk
 BUG: 83045

 Revision 1.394  2010/10/07 20:24:26  krt
 BUG:82408 FID 15083.1 SORM 2

 Revision 1.393  2010/09/28 21:30:29  ajayk
 BUG: 82437

 Revision 1.392  2010/09/28 08:58:59  sui
 BUG:81414

 Revision 1.391  2010/09/28 04:14:40  dxie
 BUG: 82763

 Revision 1.390  2010/09/27 14:48:59  hollyj
 BUG:81422 IPv6 GCC 8.2

 Revision 1.389  2010/09/23 09:20:16  dxie
 BUG: 82763

 Revision 1.388  2010/09/22 09:50:58  dxie
 BUG: 82763

 Revision 1.387  2010/09/17 16:56:59  jjkott
 BUG:82746 Hold Fix

 Revision 1.386  2010/09/16 19:47:10  taraveti
 BUG:80474 dont clone con cb

 Revision 1.385  2010/09/09 13:20:28  jjkott
 BUG:82511 PRACK Offer Fix

 Revision 1.384  2010/09/08 07:15:46  yuqinzhang
 BUG:82520: ccs core for ISDN reroute call

 Revision 1.383  2010/09/03 07:23:10  ashi
 Bug:82209

 Revision 1.382  2010/09/01 07:25:06  yuqinzhang
 BUG:82372: call failure for SIP reroute call

 Revision 1.381  2010/08/27 18:28:35  jjkott
 BUG:82190 REFER Fix

 Revision 1.380  2010/08/24 18:22:07  ajayk
 BUG: 78856

 Revision 1.379  2010/08/17 17:52:40  jjkott
 BUG:82030 SDP Answer Fix

 Revision 1.378  2010/08/13 20:06:23  nahuja
 Bug:81414 play traditional ringtone for CBRT failure for ingress SIP

 Revision 1.377  2010/08/09 03:22:22  aliu002
 BUG:81691 Fix the sip refer for ccn change

 Revision 1.376  2010/08/05 21:05:08  kumarb
 BUG:81422, GCC code changes for IPV6 MG insertion

 Revision 1.375  2010/07/28 21:42:32  hollyj
 BUG:81422 IPV6 ph1 1st pass

 Revision 1.374  2010/07/28 06:00:32  weiwallz
 BUG:81552

 Revision 1.373  2010/07/27 14:14:09  krt
 BUG: 81389 - Fix real time updates for CC_CREATE_MG_CTX

 Revision 1.372  2010/07/21 03:19:52  weiwallz
 BUG:81268

 Revision 1.371  2010/07/16 08:56:59  sui
 BUG:81092

 Revision 1.370  2010/07/16 07:07:28  sui
 BUG:81092

 Revision 1.369  2010/07/16 04:40:01  aliu002
 BUG:80938 fix DisconnectLeg for multiple MG case

 Revision 1.368  2010/07/15 16:50:21  jjkott
 BUG:81112 Test Line Fix

 Revision 1.367  2010/07/13 05:56:01  yuqinzhang
 BUG:81068: ccs core

 Revision 1.366  2010/07/09 19:20:36  jjkott
 BUG:81004 PRACK Fix

 Revision 1.365  2010/07/09 02:47:24  aliu002
 BUG:80227 Fix the clock wise issue

 Revision 1.364  2010/07/08 17:00:04  jjkott
 BUG:80836 CRBT Fix

 Revision 1.363  2010/07/08 01:16:13  aliu002
 BUG:80910 Fix late call forward case

 Revision 1.362  2010/07/07 19:21:11  jjkott
 BUG:80930 PRACK Fix

 Revision 1.361  2010/07/07 02:56:06  yuqinzhang
 BUG:80669: ogLiInfo lost in reroute call

 Revision 1.360  2010/07/02 01:07:40  kelvinz
 bug:80525

 Revision 1.359  2010/06/29 19:34:15  jjkott
 BUG:80762 SDP Off/Ans Fix

 Revision 1.358  2010/06/29 03:26:38  yuqinzhang
 BUG:77242: CCN5 not work on LGP

 Revision 1.357  2010/06/28 13:04:52  aliu002
 BUG:80182 Fix Re-route issue

 Revision 1.356  2010/06/22 20:51:24  jjkott
 BUG:80091 re-INVITE Fix

 Revision 1.355  2010/06/08 08:18:52  jfan
 BUG:79809

 Revision 1.354  2010/06/03 08:11:52  dayongww
 Bug:79985: revise sipId generating method to improve performance

 Revision 1.353  2010/05/26 08:57:58  yuqinzhang
 BUG:79628 : YGW failure should not have impact on basic call

 Revision 1.352  2010/05/25 18:02:18  jjkott
 BUG:79717 MgCtx Fix

 Revision 1.351  2010/05/20 18:23:39  krt
 BUG: 78431 Fix Reroute After Answer Scenarios

 Revision 1.350  2010/05/13 01:55:01  aliu002
 BUG:77542 FID15261 Fix ISUP COT scenario bug

 Revision 1.349  2010/05/11 20:28:59  kumarb
 BUG:79371, Changes to process V21, Text, Modem events for PASSTHRU, VBD for TDM-IP call

 Revision 1.348  2010/05/07 05:35:59  yuqinzhang
 BUG:77242 FID15083.0 SORM GCC CcOpen & CcChange

 Revision 1.347  2010/05/06 20:57:58  jjkott
 BUG:77542 FID 15261.0

 Revision 1.346  2010/04/30 21:17:16  jjkott
 BUG:77542 FID 15261.0

 Revision 1.345  2010/04/30 02:43:04  yuqinzhang
 BUG:77242 FID15083.0 SORM GCC Fix UT bug

 Revision 1.344  2010/04/29 02:35:40  cswang
 BUG: 78676 FID 15174.0 New feature development

 Revision 1.343  2010/04/28 20:19:26  ajayk
 BUG: 79169

 Revision 1.342  2010/04/26 22:13:00  ajayk
 BUG: 77242

 Revision 1.341  2010/04/23 12:42:57  jjkott
 BUG:77542 FID 15261.0

 Revision 1.340  2010/04/22 23:40:39  jjkott
 BUG:77542 FID 15261.0

 Revision 1.339  2010/04/18 12:47:24  aliu002
 BUG:78108

 Revision 1.338  2010/04/16 21:54:56  jjkott
 BUG:77542 FID 15261.0

 Revision 1.337  2010/04/14 21:12:51  ajayk
 BUG: 77242

 Revision 1.336  2010/04/13 19:52:50  jjkott
 BUG:77542 FID 15261.0

 Revision 1.335  2010/04/07 13:50:54  weiwallz
 BUG: 78108 Comment handling

 Revision 1.334  2010/04/07 03:34:09  weiwallz
 BUG:78108 FID 14941\.0

 Revision 1.333  2010/04/02 07:17:49  dayongww
 BUG:78447 8-1 fid-15189 development

 Revision 1.332  2010/03/29 17:29:30  ajayk
 BUG: 77242

 Revision 1.331  2010/03/29 09:13:17  yuqinzhang
 BUG:77242 FID15083.0 SORM GCC Phase2 codes

 Revision 1.330  2010/03/26 22:54:52  aliu002
 BUG:77542 FID15261.0

 Revision 1.329  2010/03/23 20:34:20  jjkott
 BUG:77542 FID 15261.0

 Revision 1.328  2010/03/22 09:37:57  dxie
 BUG: 78309

 Revision 1.327  2010/03/18 18:56:47  krt
 BUG: 77380 FID 15045.1 ETSI-ITU INAP Support for SSP Relay Mode

 Revision 1.326  2010/03/18 07:40:20  chuanyiyu
 bug:77971 dev FID14841.5 on behalf of wally

 Revision 1.325  2010/03/16 17:58:49  aliu002
 BUG:77542 Pre-checkin for FID15083.0

 Revision 1.324  2010/03/16 14:19:49  jjkott
 BUG:77542 FID 15261

 Revision 1.323  2010/03/15 15:56:01  kumarb
 BUG:74483, unit testing

 Revision 1.322  2010/03/15 09:32:06  yuqinzhang
 BUG:77242 FID15083.0 SORM GCC Fix building error for CC_SORM

 Revision 1.321  2010/03/10 15:39:46  ajayk
 BUG: 77242

 Revision 1.320  2010/03/10 05:05:10  yuqinzhang
 BUG:77242 FID15083.0 SORM GCC X3 allow and SubLiForClearModeCall

 Revision 1.319  2010/03/07 20:00:35  aliu002
 BUG:77542 Pre-check-in for FID15083.0 change mgCtx to pointer in mgBlock

 Revision 1.318  2010/03/06 05:18:32  yuqinzhang
 BUG:77242 FID15083.0 SORM GCC CC_SORM codes

 Revision 1.317  2010/03/01 08:33:21  zhangbengchen
 BUG: 77029

 Revision 1.316  2010/02/26 01:34:21  yuqinzhang
 BUG:77242 FID15083.0 SORM cosmetic change for fax GccCallBakFunc

 Revision 1.315  2010/02/25 06:15:24  nzhen001
 BUG:77255

 Revision 1.314  2010/02/17 15:59:57  ajayk
 BUG: 77242

 Revision 1.313  2010/02/11 18:56:38  kumarb
 BUG:74483, GCC/CASMGC/GCC-LM Code changes for FID 14844 - VBD

 Revision 1.312  2010/02/02 14:53:57  nzhen001
 BUG:77087 fix the release cause problem when oapm has error

 Revision 1.311  2009/12/15 20:59:15  hollyj
 BUG:73456

 Revision 1.310  2009/12/11 15:09:59  weiwallz
 BUG:76141

 Revision 1.309  2009/12/10 03:24:31  weiwallz
 BUG:76056

 Revision 1.308  2009/12/04 08:35:05  zhangbengchen
 BUG: 75412

 Revision 1.307  2009/12/02 14:18:12  zhangbengchen
 BUG: 75412

 Revision 1.306  2009/11/27 07:34:52  aliu002
 BUG:75722 QD SQA 7.1 regr: FID 1967 CCS core when make a SIP originating CRBT call.

 Revision 1.305  2009/11/13 04:56:18  aliu002
 BUG:72949

 Revision 1.304  2009/10/23 01:03:50  aliu002
 BUG:71300 Code merge for FID14485.0 into 8-0-1

 Revision 1.303  2009/10/19 00:45:04  weiwallz
 BUG:74267

 Revision 1.302  2009/10/15 04:26:43  nzhen001
 BUG:73112

 Revision 1.301  2009/10/02 05:36:17  hollyj
 BUG:71663 merge 14972 to Main(8-0-1)

 Revision 1.300  2009/10/01 06:10:33  weiwallz
 BUG:73855
 Revision 1.287.2.28  2009/09/29 01:02:00  weiwallz
 bug:73928

 Revision 1.299  2009/09/29 08:46:32  weiwallz
 BUG: 70444 GCC Code Merge to 8.0.1
 Revision 1.287.2.27  2009/09/28 08:45:22  cswang
 BUG:73684 Fix icSipId field double free issue

 Revision 1.287.2.26  2009/09/18 00:52:09  weiwallz
 BUG:73519

 Revision 1.287.2.25  2009/09/14 01:07:38  weiwallz
 Revision 1.298  2009/09/14 01:45:06  weiwallz
 BUG:73248

 Revision 1.297  2009/09/08 07:10:03  ashi
 Revision 1.287.2.24  2009/09/03 06:01:02  ashi
 BUG:72209

 Revision 1.296  2009/08/27 16:52:10  jjkott
 BUG: 70784 Fax and Modem Fix

 Revision 1.295  2009/08/21 06:18:07  aliu002
 BUG:72251 A party can't be connected to C paty after a second Connect with AC

 Revision 1.294  2009/08/20 02:20:27  nzhen001
 Revision 1.287.2.22  2009/08/20 02:18:54  nzhen001
 BUG:72540

 Revision 1.293  2009/08/20 02:10:27  nzhen001
 BUG:72537

 Revision 1.292  2009/08/18 08:31:53  nzhen001
 BUG:71151
 Revision 1.287.2.20  2009/08/06 02:09:40  aliu002
 BUG:72251 A party can't be connected to C paty after a second Connect with AC

 Revision 1.291  2009/08/14 02:54:41  yuqinzhang
 Revision 1.287.2.19  2009/07/29 01:44:31  yuqinzhang
 BUG:71748 Fix SIP REFER core and ADD IP term with IP/PORT

 Revision 1.290  2009/08/13 17:47:44  kumarb
 BUG:71086,71257,71262,71356,71427,71589,71626,71707,71817,71980, Merging 7-0-1 fixes into mainLine
 Revision 1.287.2.18  2009/07/26 02:29:55  hollyj
 BUG:71663 GCC FID 14972.0

 Revision 1.287.2.17  2009/07/23 01:38:02  nzhen001
 BUG:71151

 Revision 1.287.2.16  2009/07/17 21:32:44  jjkott
 BUG: 70444 Realm Unavailable Fix

 Revision 1.287.2.15  2009/07/12 16:06:10  weiwallz
 BUG: 70444 Bug Fix

 Revision 1.287.2.14  2009/07/10 20:52:06  kumarb
 Revision 1.288.2.2  2009/07/10 20:49:04  kumarb
 BUG:71817, Not to remove egress BGW after processing ACK (FID 14732)

 Revision 1.289  2009/07/15 20:14:30  ajayk
 Revision 1.287.2.13  2009/07/10 10:16:34  weiwallz
 BUG: 70444 Bug Fix

 Revision 1.287.2.12  2009/07/10 07:55:33  weiwallz
 BUG: 70444 Bug Fix

 Revision 1.287.2.11  2009/07/10 02:18:02  weiwallz
 BUG: 70444 Bug Fix

 Revision 1.287.2.10  2009/07/08 01:11:50  weiwallz
 BUG: 70444 Do bandwidth accounting(Update) for Priority call

 Revision 1.287.2.9  2009/07/07 20:22:58  ajayk
 BUG: 71477

 Revision 1.287.2.8  2009/07/02 08:43:02  weiwallz
 BUG: 70444 Add robustness NULL check

 Revision 1.287.2.7  2009/07/02 08:35:52  weiwallz
 BUG: 70444 Add more logging

 Revision 1.287.2.6  2009/07/02 02:42:21  weiwallz
 BUG: 70444 use the 2 new fail conditions

 Revision 1.287.2.5  2009/06/30 06:05:10  weiwallz
 BUG: 70444 remove ABORT_DEBUG in ccConE68S139

 Revision 1.287.2.4  2009/06/30 00:44:52  weiwallz
 BUG: 70444 Bug fix calling ccCheckForInterfaces

 Revision 1.287.2.3  2009/06/29 01:02:23  aliu002
 BUG:71300 New Feature Development for FID14485.0

 Revision 1.287.2.2  2009/06/26 02:06:41  weiwallz
 BUG: 70444 Check in GCC and MEGAA change from Wally

 Revision 1.287.2.1  2009/05/20 09:05:09  nzhen001
 Revision 1.288  2009/05/15 02:23:58  ashi
 BUG:69986

 Revision 1.287  2009/05/10 13:31:03  ajayk
 BUG: 69394: FID 14732.0

 Revision 1.286  2009/05/07 10:45:33  chunyaji
 BUG:69986

 Revision 1.285  2009/05/05 14:30:58  nzhen001
 BUG:69986

 Revision 1.284  2009/02/08 08:31:13  weiwallz
 BUG: 67115 Fix regression issue as stated in bug 68089 and 67978

 Revision 1.283  2009/01/05 01:40:51  yuqinzhang
 BUG: 67115

 Revision 1.282  2008/11/20 15:50:23  hollyj
 Bug:66371 R7.0FID14426

 Revision 1.281  2008/09/19 19:36:42  ajayk
 BUG: 65587

 Revision 1.280  2008/09/08 18:08:25  jjkott
 BUG:65044 FID 14209

 Revision 1.279  2008/09/08 17:54:39  jjkott
 BUG:65436 COT Fix

 Revision 1.278  2008/08/08 16:40:14  krt
 BUG:64896 Fix CRBT case, where extra 180X w/o SDP on intra CCS scenario

 Revision 1.277  2008/08/01 01:02:59  ashi
 BUG:64647

 Revision 1.276  2008/07/01 12:29:15  jjkott
 BUG:64141 Unexpected CAS Onhook Fix

 Revision 1.275  2008/06/05 21:37:04  abpeterson
 BUG:63620 Correctly release trunk after CM failover for test line call

 Revision 1.274  2008/05/20 08:53:52  cli4
 BUG: 63445 Linux warning cleanup

 Revision 1.273  2008/05/15 14:54:03  ajay
 BUG: 61139

 Revision 1.272  2008/05/09 14:33:20  jjkott
 BUG:63314 MG Removal Fix

 Revision 1.271  2008/05/07 01:46:23  cli4
 BUG: 63291 CGE5 porting.

 Revision 1.270  2008/04/28 14:24:47  kbasavar
 BUG:62538, Changes to ccConE62S10() to Ignore NOTIFY with signalCompletion

 Revision 1.269  2008/04/25 14:45:58  krt
 BUG:63050 Fix CRBT for Billing

 Revision 1.268  2008/04/23 20:32:17  jjkott
 BUG:59032 Multiple Leg CDR Fix

 Revision 1.267  2008/04/18 18:30:59  kbasavar
 BUG:62478,Changes to remove starting of setup timer in ccConE39S123()

 Revision 1.266  2008/04/18 17:31:32  krt
 BUG:62849 Add replication of MG Context for LMSD reINVITE

 Revision 1.265  2008/04/15 01:17:56  yuqinzhang
 BUG:61196 Set cdrStatus to correct value when generating billing

 Revision 1.264  2008/04/14 13:05:56  jjkott
 BUG:62783 PRACK/18X Collision Fix

 Revision 1.263  2008/04/10 20:13:53  jjkott
 BUG:62783 PRACK/18X Collision Fix

 Revision 1.262  2008/04/08 16:08:39  abpeterson
 BUG:62665 Fix billing record generation for 108 ISUP/ISDN test call

 Revision 1.261  2008/04/03 16:31:10  edelk
 BUG: 52785 Fix Hold Flag check with CC_IS_CALL_FLAG_SET macro call in Main.

 Revision 1.260  2008/03/21 06:14:54  yushuanchen
 BUG:58176 Set start time value for the opr-ccr cdr

 Revision 1.259  2008/03/20 19:37:35  jjkott
 BUG:61385 FID 2575 Phase 2

 Revision 1.258  2008/03/19 18:29:47  jboyle
 Bug:62273 CC19 not set on SIP originated calls

 Revision 1.257  2008/03/13 14:56:42  jjkott
 BUG:61385 FID 2575 Phase 2

 Revision 1.256  2008/03/12 15:46:57  krt
 BUG:60869 Fix CRBT Outgoing COT

 Revision 1.255  2008/03/11 20:13:57  jjkott
 BUG:61385 FID 2575 Phase 2

 Revision 1.254  2008/03/10 16:17:37  jjkott
 BUG:61385 FID 2575 Phase 2

 Revision 1.253  2008/02/29 18:16:50  jjkott
 BUG:61506 FID 1924 Memory Usage Enhancement

 Revision 1.252  2008/02/26 19:08:53  krt
 BUG:61385 FID:2575 Phase 2 Coding

 Revision 1.251  2008/02/08 17:33:22  abpeterson
 BUG:57775 billing FID 1915 Trunk Test Line

 Revision 1.250  2008/02/06 08:17:40  chuanyiyu
 bug:61398

 Revision 1.249  2008/02/01 08:39:15  haiyan
 BUG:60709 A dial to WIN B, in raining day, B can not be released

 Revision 1.248  2008/01/30 16:19:22  jjkott
 BUG:61209 Transcoding and CRBT Interaction Fix

 Revision 1.247  2008/01/24 16:48:42  hollyj
 BUG:60393 MEGAA&GCC 14009

 Revision 1.246  2008/01/16 21:21:15  krt
 BUG:60704 FID 2575 - Unit Testing

 Revision 1.245  2008/01/12 02:12:25  edelk
 BUG: 60994 Fixing problem with CRBT and SIP Fork features

 Revision 1.244  2008/01/11 12:11:35  krt
 BUG: 60704 FID 2575 Unit Testing

 Revision 1.243  2008/01/03 19:16:57  krt
 BUG:60704 FID 2575 - LMSD Compliance for TrFO/RTO

 Revision 1.242  2007/12/10 08:22:52  yushuanchen
 BUG:59907 Delete definition and reference of subId/outSub

 Revision 1.241  2007/11/27 21:15:20  ajay
 BUG: 58916: FID:7804

 Revision 1.240  2007/11/08 06:12:46  cli4
 BUG: 60043 Cleanup Linux build warning.

 Revision 1.239  2007/10/26 15:28:56  jerri
 BUG: 59304

 Revision 1.238  2007/10/18 16:40:29  jjkott
 BUG:58816 FID 2593

 Revision 1.237  2007/10/12 15:35:33  jjkott
 BUG:58816 FID 2593

 Revision 1.236  2007/10/12 13:51:02  abpeterson
 BUG:57775 Merge for FID 1915 Trunk Test Line

 Revision 1.235  2007/10/10 08:29:31  smchi
 BUG:58176 FID 2001 development

 Revision 1.234  2007/10/01 20:59:30  ajay
 BUG:58916 FID:7804

 Revision 1.233  2007/09/27 18:03:26  krt
 BUG:57523 Send BW message on ignore event from FIC for CRBT case

 Revision 1.232  2007/09/10 01:08:41  edelk
 BUG: 58552 Fixing SIP Refer CDR problem

 Revision 1.231  2007/08/30 19:30:52  jjkott
 BUG:58536 Post-answer LCF Fix

 Revision 1.230  2007/08/28 20:15:06  edelk
 BUG: 52785 Porting fix to Main from 3.X for SIP-ISUP Refer Call

 Revision 1.229  2007/07/19 17:16:13  achrcanovic
 BUG:57550 Brazil-VIVO, on receiption of SUS, REL values are not correct

 Revision 1.228  2007/07/06 07:05:57  ashi
 BUG:56917

 Revision 1.227  2007/07/02 17:41:04  praveen
 Bug:53996

 Revision 1.226  2007/06/29 20:04:06  ajay
 BUG: 56977

 Revision 1.225  2007/06/27 02:09:55  dxie
 BUG: 56977 redefine specific event buffer function E03S2X

 Revision 1.224  2007/06/21 15:12:04  ajay
 BUG: 56977

 Revision 1.223  2007/06/15 19:22:47  ajay
 BUG: 56930

 Revision 1.222  2007/06/13 18:00:54  mbelkode
 BUG: 56663 For Ken Tishenkel

 Revision 1.221  2007/05/23 06:49:42  hong
 BUG: 56154 fix for ST digit

 Revision 1.220  2007/05/22 14:26:10  hxu
 bug:54536 - linux porting

 Revision 1.219  2007/05/18 17:22:10  gbonthu
 bug:56069: fix for BICC UPDATE_RSP\n

 Revision 1.218  2007/05/10 18:26:20  praveen
 Bug:55849

 Revision 1.217  2007/05/03 18:36:20  ajay
 BUG: 55605

 Revision 1.216  2007/05/02 18:27:49  ajay
 BUG: 55605

 Revision 1.215  2007/04/27 14:27:59  yourk
 BUG: 55039 KPN ALS70 CAS tones and signals

 Revision 1.214  2007/04/26 19:06:12  gbonthu
 bug:55534, 55568: fix for 2 diff bugs in FID-2112

 Revision 1.213  2007/04/25 15:39:25  krt
 BUG:54276 Customized Ring Back Tone (CRBT) 1967

 Revision 1.212  2007/04/19 17:11:27  praveen
 Bug:55191

 Revision 1.211  2007/04/18 23:22:41  krt
 BUG:54276 Customized Ring Back Tone (CRBT) 1967

 Revision 1.210  2007/04/18 18:48:19  krt
 BUG:54276 Customized Ring Back Tone (CRBT) 1967

 Revision 1.209  2007/04/17 17:44:36  gbonthu
 bug:54097: Added code to handle CPG retry timer expiry

 Revision 1.208  2007/04/13 19:17:14  gbonthu
 bug:54097: fix for bugs found during FID 2112 dev testing

 Revision 1.207  2007/04/13 13:04:06  jjkott
 BUG:54988

 Revision 1.206  2007/04/12 12:32:54  krt
 BUG:54276 Customized Ring Back Tone (CRBT) 1967

 Revision 1.205  2007/04/09 14:08:13  krt
 BUG:54276 Customized Ring Back Tone (CRBT) 1967

 Revision 1.204  2007/04/03 17:48:00  ajay
 BUG: 54218: FID2082

 Revision 1.203  2007/03/30 20:10:28  praveen
 Bug:54176, For Announcement + Digits, send Resource Clear to FIC on MG failures

 Revision 1.202  2007/03/28 14:40:07  krt
 BUG:54276 Customized Ring Back Tone (CRBT) 1967

 Revision 1.201  2007/03/28 13:35:48  gbonthu
 bug:54097: SIP UPDATE PRACK, ACK support FID 2112

 Revision 1.200  2007/03/21 12:43:47  krt
 BUG:54276 Handle CAC event from FIC in correct state

 Revision 1.199  2007/03/19 17:24:40  krt
 BUG:54276 Customized Ring Back Tone (CRBT) FID: 1967

 Revision 1.198  2007/03/15 13:34:43  gbonthu
 bug:54097: Support for initial INVITE with NO SDP

 Revision 1.197  2007/03/14 14:16:47  ajay
 BUG: 54218: FID2082

 Revision 1.196  2007/03/13 15:01:57  gbonthu
 bug:54097: clean-up old code

 Revision 1.195  2007/01/09 18:48:34  praveen
 Bug:53187, Ignore Fax/Modem events in S09

 Revision 1.194  2007/01/08 22:05:35  kbasavar
 BUG:50423,Early ACM changes for incoming SIPT

 Revision 1.193  2007/01/05 18:17:11  gurusamy
 BUG:49781 SQA - When a re-invite is received with a unsupported codec, the call is dropped (Feature 2075)

 Revision 1.192  2006/12/19 13:29:32  ajay
 BUG: 52908

 Revision 1.191  2006/12/18 07:29:35  smchi
 BUG:52061 regenerate ccEvntHold on the standby for failover scenario in E37S10

 Revision 1.190  2006/12/18 01:05:48  yunzhang
 BUG:52387

 Revision 1.189  2006/11/23 02:37:51  smchi
 BUG:52061 delete obsolete control block on the standby when receive 2nd AnalyzeRoute on answered call

 Revision 1.188  2006/11/22 16:13:05  jjkott
 BUG:52322

 Revision 1.187  2006/11/21 15:46:50  praveen
 Bug:52240

 Revision 1.186  2006/11/16 21:30:03  kbasavar
 BUG:52113, Changes to process REL during wait for ODisconnectCalled

 Revision 1.185  2006/11/15 20:40:43  gbonthu
 bug:52162: fix

 Revision 1.184  2006/11/03 04:18:45  smchi
 BUG:51214 ccConE37S10 process 2nd AR when call is answered

 Revision 1.183  2006/11/02 18:55:07  kbasavar
 BUG:51782, Setting of IAM echo bit in the IngressEchoVal for SIPT

 Revision 1.182  2006/10/27 12:54:48  jjkott
 BUG:51212

 Revision 1.181  2006/10/20 18:04:50  jjkott
 BUG:51115

 Revision 1.180  2006/10/17 17:05:43  jjkott
 BUG:51201

 Revision 1.179  2006/10/11 15:03:46  ajay
 BUG: 45545 49078

 Revision 1.178  2006/10/11 13:33:42  ajay
 BUG: 45545 49078

 Revision 1.177  2006/10/03 21:21:55  ajay
 BUG: 45545 49078

 Revision 1.176  2006/09/26 20:15:15  ajay
 BUG: 48783

 Revision 1.175  2006/09/22 15:54:52  spamidi
 BUG:47409 Changes for FID 2164

 Revision 1.174  2006/09/18 15:43:24  gbonthu
 bug:47396: SIP refer and other features

 Revision 1.173  2006/08/27 15:01:56  jjkott
 BUG:47398 FID 1950

 Revision 1.172  2006/08/11 17:37:29  gbonthu
 bug:47396: SIP Refer support

 Revision 1.171  2006/07/27 02:35:57  haiyan
 BUG:47398 Merge code from DEV to main

 Revision 1.170  2006/07/12 04:19:49  dxie
 BUG: 48272

 Revision 1.169  2006/07/11 20:10:06  ajay
 DEV: Merge from BRANCH-5-4 to main

 Revision 1.168  2006/07/10 06:34:35  dxie
 BUG: 48272

 Revision 1.167  2006/07/03 02:58:04  ykuang
 BUG:47300 Maximum digit timer expired in protocol layer

 Revision 1.166  2006/06/23 11:55:10  smchi
 BUG:41043 remove the unnecesasry code change of 41043

 Revision 1.165  2006/06/22 13:21:54  smchi
 BUG:41043 when LCS receive REL cause code31 release incoming side CAS TG with reorder tone

 Revision 1.164  2006/06/09 01:53:25  praveen
 Bug:47291, Fix to send empty remote in case of T.38

 Revision 1.163  2006/06/06 01:56:14  haiyan
 BUG:46944 fix bug 46944 for GCC part

 Revision 1.162  2006/06/01 15:32:05  ajay
 BUG: 45232

 Revision 1.161  2006/05/24 21:20:33  ajay
 BUG: 45232, 46873, 46902

 Revision 1.160  2006/05/16 19:06:50  ajay
 BUG: 45232

 Revision 1.159  2006/05/15 20:55:26  gbonthu
 bug:46506: fix

 Revision 1.158  2006/05/15 18:09:03  ajay
 BUG: 46412

 Revision 1.157  2006/05/12 13:30:48  ajay
 BUG: 46412

 Revision 1.156  2006/05/11 01:53:57  dxie
 BUG: 46188 merge ccConE02S01 to ccConE02S96.

 Revision 1.155  2006/04/28 01:00:18  samfang
 bug:45275 handle IAM after CRM

 Revision 1.154  2006/04/13 05:57:36  dxie
 DEV: missing merge code for China ISUP.

 Revision 1.153  2006/04/11 15:31:23  praveen
 Bug:43066

 Revision 1.152  2006/03/24 10:15:56  songleiwang
 BUG:45069 Rogers-ILEC ISUP cert - T1M1 M-1-7-1B failed

 Revision 1.151  2006/03/23 13:57:17  euysal
 BUG:45236 Fixed problems found with Dual Call Hold Feature

 Revision 1.150  2006/03/21 05:47:19  dxie
 DEV:merge china ISUP

 Revision 1.149  2006/03/14 23:03:44  taraveti
 BUG:45097 determine the direction before calling StaReq

 Revision 1.148  2006/03/09 16:33:05  gbonthu
 bug:44936: fix

 Revision 1.147  2006/03/08 21:26:38  euysal
 BUG:42382 Support for Asynchronous STR

 Revision 1.146  2006/03/08 18:24:08  gbonthu
 bug:44936: fix

 Revision 1.145  2006/03/08 18:04:53  ajay
 BUG: 41709 44385 44412 41421

 Revision 1.144  2006/03/06 15:56:15  euysal
 DEV: Modified call flow to send CTRClear with ClcFailure in case Announcement cannot be played

 Revision 1.143  2006/02/13 16:32:37  gbonthu
 dev:merge from BRANCH-DEV-6-2 to MAIN. This inlcudes GeoR and T.38 changes in CC and RM

 Revision 1.142  2006/01/28 23:21:17  gshores
 BUG: 4 Merging 5.3 -> 6.2 Phase2

 Revision 1.141  2006/01/24 13:25:48  rtperez
 BUG: 4 merge from BRANCH-5-3


 Revision 1.125.2.7.4.19.2.13.2.20  2006/01/25 08:44:08  smchi
 BUG:43151 memory leak for sdp buffer sent from SIP to GCC in ConCfm

 Revision 1.125.2.7.4.19.2.13.2.19  2005/12/31 09:03:08  haiyan
 BUG:43373

 Revision 1.125.2.7.4.19.2.13.2.18  2005/12/31 05:45:16  haiyan
 BUG:43373

 Revision 1.125.2.7.4.19.2.13.2.17  2005/12/06 16:57:36  peizeng
 BUG: 43049 Dual Call Hold

 Revision 1.125.2.7.4.19.2.13.2.16  2005/12/02 01:38:39  smchi
 BUG:42868

 Revision 1.125.2.7.4.19.2.13.2.15  2005/10/25 15:17:26  kbasavar
 BUG:42001

 Revision 1.125.2.7.4.19.2.13.2.14  2005/10/25 14:11:23  gbonthu
 bug:41920. fix

 Revision 1.125.2.7.4.19.2.13.2.13  2005/10/25 12:14:57  ajay
 BUG:41912 :  Stopping Setup timer for Milliwatt call

 Revision 1.125.2.7.4.19.2.13.2.12  2005/10/19 19:33:53  ajay
 BUG: 41154

 Revision 1.125.2.7.4.19.2.13.2.11  2005/10/19 19:26:56  ajay
 BUG: 41421

 Revision 1.125.2.7.4.19.2.13.2.10  2005/09/21 15:15:00  gbonthu
 bug:40996 fix

 Revision 1.125.2.7.4.19.2.13.2.9  2005/09/21 13:26:31  euysal
 BUG:40662 Fixed a wrong pointer assignment

 Revision 1.125.2.7.4.19.2.13.2.8  2005/09/14 20:05:22  bcouto
 DEV: Venezuelan ISUP - DRS Procedure

 Revision 1.125.2.7.4.19.2.13.2.7  2005/09/13 20:52:53  kbasavar
 BUG:40847, Handle ASYCH CONTINUE after ConReq

 Revision 1.125.2.7.4.19.2.13.2.6  2005/09/09 14:27:54  mbelkode
 DEV: Prfl protrel changes from 5-1-2

 Revision 1.125.2.7.4.19.2.13.2.5  2005/09/05 05:41:35  jinhuawang
 DEV: Feature 1839 compatibility for German ISUP variant.

 Revision 1.125.2.7.4.19.2.13.2.4  2005/09/02 14:54:05  ajay
 BUG: 40285: Implemented CC_UPDATE_PIC macro

 Revision 1.125.2.7.4.19.2.13.2.3  2005/08/31 18:26:04  euysal
 BUG:40634 Fixed a problem with CALLDLT timer

 Revision 1.125.2.7.4.19.2.13.2.2  2005/08/26 08:28:51  gshores
 BUG: 4 merge activity for BRANCH-5-3

 Revision 1.125.2.7.4.19.2.13.2.1  2005/08/19 19:07:14  spamidi
 BUG:40373 Fixing elapTime issue in BRANCH-5-3

 Revision 1.125.2.7.4.19.2.13  2005/08/09 18:46:35  gbonthu
 dev:removed state 88

 Revision 1.125.2.7.4.19.2.12  2005/08/09 17:48:31  gbonthu
 dev:Merge from BRANCH-SPR2-5-1-2

 Revision 1.125.2.7.4.19.2.11  2005/08/09 13:29:45  kbasavar
 DEV: Merge from SPR2-5-1-2

 Revision 1.125.2.7.4.19.2.10  2005/08/08 21:09:53  kbasavar
 DEV: EARLY ACM merge from SPR2-5-1-2

 Revision 1.125.2.7.4.19.2.7  2005/07/29 14:45:37  praveen
 Bug:39885

 Revision 1.125.2.7.4.19.2.6  2005/07/29 13:32:32  praveen
 Bug:39885

 Revision 1.125.2.7.4.19.2.5  2005/07/28 13:56:26  euysal
 DEV: Fixed a problem with O_ANSWER Request

 Revision 1.125.2.7.4.19.2.4  2005/07/26 17:55:00  euysal
 DEV: ACC Support

 Revision 1.125.2.7.4.19.2.3  2005/07/22 00:46:55  bcouto
 DEV: Brazilian/Indian ISUP merge

 Revision 1.125.2.7.4.19.2.2  2005/07/21 12:31:14  ajay
 BUG: 39287 : CscCallerAbandon sent in close message when the call is abandoned

 Revision 1.125.2.7.4.19.2.1  2005/07/20 23:43:07  gbonthu
 bug:39652. fix

 Revision 1.125.2.7.4.19  2005/07/11 21:41:58  euysal
 BUG:39440 Fixed a problem with ReleaseCall from FIC

 Revision 1.125.2.7.4.18  2005/07/11 13:12:49  euysal
 DEV: Added more logging to better trace  bug 38971

 Revision 1.125.2.7.4.17.4.7  2005/08/22 17:52:11  bcouto
 DEV: ISUP Poland changes

 Revision 1.125.2.7.4.17.4.6  2005/08/09 06:13:00  jinhuawang
 DEV: LYNX4. GCC 5.3 Germany ISUP

 Revision 1.125.2.7.4.17.4.5  2005/08/09 06:10:35  jinhuawang
 DEV: LYNX4. GCC 5.3 Germany ISUP

 Revision 1.125.2.7.4.17.4.4  2005/08/09 06:07:07  jinhuawang
 DEV: Rewind to version 1.125.2.7.4.17.4.1

 Revision 1.125.2.7.4.17.4.1  2005/07/23 20:39:15  rqbarros
 DEV: Changes for ISUPVENEZUELA variant.

 Revision 1.125.2.7.4.17  2005/06/24 17:56:15  gbonthu
 bug:38983. fix

 Revision 1.125.2.7.4.16  2005/06/22 18:03:07  gbonthu
 bug:38872. reverting changes

 Revision 1.125.2.7.4.15  2005/06/20 19:07:30  euysal
 BUG:38971 Fixed the problem with cause value in ReleaseCall from FIC

 Revision 1.125.2.7.4.14  2005/06/16 17:31:17  gbonthu
 bug:38872. fix for contiue in o_disconnect

 Revision 1.125.2.7.4.13  2005/06/10 14:42:04  ajay
 BUG: 38737: COT issue

 Revision 1.125.2.7.4.12  2005/06/01 16:10:21  gbonthu
 bug:38498. fix

 Revision 1.125.2.7.4.11  2005/05/25 14:29:50  euysal
 DEV: CAMEL/WIN Support

 Revision 1.125.2.7.4.10  2005/05/24 20:19:01  wrapose
 bug:38106 - clear con-billing.tckRelReq,Ind - set both pres to NOTPRSNT in after getting relcfm in e05s88

 Revision 1.125.2.7.4.9  2005/05/18 21:55:34  ajay
 BUG: 37659 : Mapping issue in E39S03

 Revision 1.125.2.7.4.8  2005/05/18 18:48:44  ajay
 BUG: 38038 : Adding LOCALLYANSWERED state to macro

 Revision 1.125.2.7.4.7  2005/05/17 19:24:44  ajay
 BUG: 37659 : Debug added for finding the bug

 Revision 1.125.2.7.4.6  2005/05/11 20:51:39  ajay
 BUG: 37659 : Handling Continue for ODisconnectCalled response

 Revision 1.125.2.7.4.5  2005/05/03 10:15:08  gshores
 BUG: 4 merge activity of BRANCH-5-1 into BRANCH-5-1-1

 Revision 1.125.2.9  2005/05/02 20:13:57  kbasavar
 BUG:37809, Stop ONoAnswer timer in E39S03

 Revision 1.125.2.8  2005/04/18 21:30:00  ajay
 BUG: 37540: Handle release timer in state 03

 Revision 1.125.2.7  2005/03/24 16:57:46  ajay
 BUG: 33018: code in E37S09 is corrected to look at scpResponse in Analyze Route

 Revision 1.125.2.6  2005/03/17 21:22:16  ajay
 BUG: 36764: Fixed code in not send release on the outgoing, where the resource is not present

 Revision 1.125.2.5  2005/03/08 21:24:55  euysal
 BUG:36186 Added suConnId while buffering CCE_ONOANSEXP and CCE_TNOANSEXP

 Revision 1.125.2.4  2005/02/08 21:29:36  ajay
 BUG: 35342 : Adding support for MilliWatt Tone

 Revision 1.125.2.3  2005/01/21 19:37:09  gbonthu
 dev:fix for call id

 Revision 1.125.2.2  2005/01/03 14:49:22  ajay
 BUG: 34594

 Revision 1.125.2.1  2004/12/22 17:19:10  rsuresh
 BUG: 34677 - CC_INCOMING is the direction in case of OPR-CCR

 Revision 1.125  2004/12/10 18:59:34  jhutchins
 bug:34610

 Revision 1.124  2004/12/02 19:03:26  rsuresh
 BUG: 34458 - Stop Await Answer timer before starting

 Revision 1.123  2004/11/30 22:19:17  ajay
 BUG: 34405

 Revision 1.122  2004/11/30 21:20:26  ajay
 BUG: 34346

 Revision 1.121  2004/11/17 20:35:32  rsuresh
 DEV: Merge GCC SIPT changes from 3-10-1 and ATT-NGBE

 Revision 1.120  2004/10/04 20:26:02  ajay
 BUG: 33270 : ccAnnouncementTreatment is deleted and ccProcessFailCnd is used instead

 Revision 1.119  2004/09/15 19:08:45  gbonthu
 bug:31058. fix for re-invite with SDP that has no local codec match

 Revision 1.118  2004/09/02 19:01:00  rsuresh
 BUG: 32694 - GCC passes the COT timer from ISUP to MGCT in E26S09

 Revision 1.117  2004/08/24 03:07:16  rsuresh
 BUG: 32651 - GCC buffers RE-ATTEMPT indication in S30 (AWT_MGCTNOTIFY)

 Revision 1.116  2004/08/12 18:05:02  rsuresh
 BUG: 32461 - GCC stops TAWAITANS timer in all cases

 Revision 1.115  2004/08/02 20:48:53  rsuresh
 BUG: 32215 - GCC fills cause code CCNOANSWR in case of E29S09

 Revision 1.114  2004/07/14 18:36:20  kbasavar
 BUG: 31132, Calea leg release check in ccDropIcBcm/ogBcm()

 Revision 1.113  2004/06/29 16:26:06  kparikh
 BUG: 30855

 Revision 1.112  2004/06/04 18:30:23  ajay
 BUG: 30573

 Revision 1.111  2004/06/04 14:36:37  ajay
 DEV: Found bug in COT testing

 Revision 1.110  2004/06/04 14:34:28  ajay
 DEV: Found bug in COT testing

 Revision 1.109  2004/05/30 15:48:57  kbasavar
 DEV: Correct Compilation error

 Revision 1.108  2004/05/28 21:18:43  rsuresh
 BUG: 30429 GCC releases the context on Await Answer Timer Expiry

 Revision 1.107  2004/05/21 13:36:31  amondal
 BUG:29725 backed out changes

 Revision 1.106  2004/05/21 13:19:23  amondal
 BUG:29725 No SUB ALL on error from MGI

 Revision 1.105  2004/05/21 13:11:27  kbasavar
 DEV: correct compilation error

 Revision 1.104  2004/05/20 19:43:30  ajay
 BUG: 30168

 Revision 1.103  2004/05/20 13:27:51  ajay
 BUG: 30134

 Revision 1.102  2004/05/17 17:33:23  tfrederi
 BUG:27547. Fix for SIP CFA transfer problem.

 Revision 1.101  2004/05/12 15:26:04  rsuresh
 BUG: 26758 GCC modifications for relOrign changes in CDR in mainline

 Revision 1.100  2004/05/07 02:52:23  rsuresh
 DEV: International Support of Network Suspend Timer and Answer Supervision timer in GCC

 Revision 1.99  2004/04/30 22:03:52  jjohn
 Bug:28895 rtrv-pm-rc aligned to RTRV-FAILCND

 Revision 1.98  2004/04/30 19:06:05  mcao
 Bug:28730

 Revision 1.97  2004/04/28 20:50:27  ajay
 DEV: Support for SIP and BICC  route call after annoucement

 Revision 1.96  2004/04/28 15:05:30  ajay
 DEV: MGCT error code fixes

 Revision 1.95  2004/04/14 14:01:26  ajay
 DEV: MGC development:
      Support for MG state changes in GCC.
      Handle MGCT_ACTION_EXECUTED_ERROR from MGCT.

 Revision 1.94  2004/04/01 19:27:27  ajay
 BUG: 28581

 Revision 1.93  2004/03/25 21:14:15  mbelkode
 DEV: FIC-GCC-Services API optimization changes

 Revision 1.92  2004/03/24 23:22:08  ajay
 BUG: 28518

 Revision 1.91  2004/03/23 01:36:13  mcao
 Bug:28351,28361,28403,28378

 Revision 1.90  2004/03/17 00:06:33  mcao
 Bug:28300,28192

 Revision 1.89  2004/03/15 03:25:32  mcao
 Bug:28192

 Revision 1.88  2004/03/11 19:25:51  rsuresh
 DEV: International ISDN/ISUP overlap support - new design

 Revision 1.87  2004/03/05 16:18:34  rsuresh
 DEV: INTL changes merged in for ISUP overlap support, GCC-BICC interworking & Variants

 Revision 1.86  2004/03/03 20:06:01  kbasavar
 BUG: 26062, changes to defer LEA release on FicErr for subject call

 Revision 1.85  2004/03/02 20:33:41  kbasavar
 BUG: 27816, Changes to E39S86() to handle continue with no error code

 Revision 1.84  2004/03/02 20:30:54  kbasavar
 BUG: 27816, Changes to E39S86() to handle continue with no error code

 Revision 1.83  2004/02/27 01:57:26  mcao
 Dev: CPDI fixes

 Revision 1.82  2004/02/18 15:45:52  ajay
 BUG:27756: Cleaning up call block stuck for release complete from GR303

 Revision 1.81  2004/01/30 18:48:01  gbonthu
 bug:27219: fix for problems encountered during MCI demo

 Revision 1.80  2004/01/30 15:28:25  gdixit
 BUG:25982, COT test using MEGACO COT package, These GCC changes are made by Ajay/Kumar

 Revision 1.79  2004/01/30 13:54:45  kbasavar
 DEV: Internal BICC call development

 Revision 1.78  2004/01/21 21:46:15  ajay
 DEV: MGC Development for supporting inter-CCS calls and also fixing SubAll error code

 Revision 1.77  2004/01/16 22:26:19  gbonthu
 bug:24322: fix for xfer

 Revision 1.76  2004/01/13 18:41:04  ajay
 DEV: inter-BICC call support

 Revision 1.75  2004/01/12 20:32:25  kbasavar
 BUG: 26435, Changes to ZERO duration CDR generation

 Revision 1.74  2004/01/08 17:09:15  ajay
 DEV: MGC COT support

 Revision 1.73  2004/01/06 16:06:26  gbonthu
 bug:26644: fix

 Revision 1.72  2004/01/06 14:41:28  ajay
 DEV: MGC COT support

 Revision 1.71  2004/01/05 13:51:48  darcari
 dev: added support for sip pound detect

 Revision 1.70  2003/12/31 21:49:15  ajay
 DEV: MGC COT support

 Revision 1.69  2003/12/30 22:36:54  ajay
 DEV: MGC COT support

 Revision 1.68  2003/12/29 18:55:48  kbasavar
 BUG: 26743, Answer Fraud Call

 Revision 1.67  2003/12/29 14:53:26  ajay
 DEV: MGC COT support

 Revision 1.66  2003/12/23 19:31:37  ajay
 DEV: MGC: CPDI and other DTMF detection fixes

 Revision 1.65  2003/12/16 22:23:40  gbonthu
 dev:post megre cleanUp of GCC

 Revision 1.64  2003/12/12 23:05:16  gbonthu
 dev:mergr from MGC

 Revision 1.63  2003/12/11 21:00:13  ajay
 DEV: Merge from BRANCH-MGCDEV-5-0-0-0-16 to Mainline

 Revision 1.62  2003/12/11 19:26:00  ajay
 DEV: Merge from BRANCH-MGCDEV-5-0-0-0-16 to Mainline

 Revision 1.61  2003/12/10 20:35:03  ajay
 DEV: Merge from BRANCH-MGCDEV-5-0-0-0-16 to Mainline

 Revision 1.60  2003/12/04 00:27:04  ajay
 BUG: 26355:

 Revision 1.59  2003/11/18 22:01:15  ajay
 DEV: Fix compile error

 Revision 1.58  2003/11/15 19:28:14  ajay
 DEV: Merge of Wireless support from BRANCH-3-8 to Mainline

 Revision 1.57  2003/11/04 16:03:18  gbonthu
 bug:25839. restore RLS timer's value.  ISUP needs 180 seconds time to recover from COT failure

 Revision 1.56  2003/10/08 13:25:23  gbonthu
 bug:25367:remove sleep from ccUnexpected. buffer CCR when waiting for MGitCfm

 Revision 1.55  2003/10/06 20:59:40  ajay
 BUG:25091  Restoring Con and Release event holds in the ccEvntHold

 Revision 1.54  2003/10/02 22:02:06  gbonthu
 bug:25303. SDP in 200 OK will be freed in GCC

 Revision 1.53  2003/09/29 15:50:41  bpolhemus
 DEV: T.38 merge

 Revision 1.52  2003/09/25 18:09:49  gbonthu
 bug:24909 fix

 Revision 1.51  2003/09/16 22:01:45  mcao
 DEV:CPDI changes

 Revision 1.50  2003/09/15 04:24:19  mcao
 DEV: CPDI changes and remove warnings

 Revision 1.49  2003/09/10 12:16:37  kbasavar
 DEV: correcting compilation warnings

 Revision 1.48  2003/09/08 20:13:18  spamidi
 DEV: Checking in all the CPDI code...CC files are modified by Meng but checked in from my sandbox.

 Revision 1.47  2003/09/08 19:58:20  ajay
 BUG:24460

 Revision 1.46  2003/09/02 15:57:04  ajay
 BUG:24437, 24409

 Revision 1.45  2003/08/27 21:53:17  kbasavar
 BUG: 24094 Call Wait involving fraud call

 Revision 1.44  2003/08/22 18:33:24  mcao
 Bug:23856 untabify code

 Revision 1.43  2003/08/22 02:20:27  mcao
 Bug:23840

 Revision 1.42  2003/08/21 21:01:47  ajay
 BUG:24073

 Revision 1.41  2003/08/21 20:54:59  vgupta
 DEV: removed all error conditions from fct.h, a unique file telica_errtreat.h is being used

 Revision 1.40  2003/08/21 00:12:36  mcao
 Bug:23682

 Revision 1.39  2003/08/18 18:53:11  mcao
 Bug:23832

 Revision 1.38  2003/08/18 04:36:15  mcao
 Bug:23841

 Revision 1.37  2003/08/14 14:01:02  gbonthu
 bug:23288. start handling send_to_resource in await DPE1, i.e after origination attempt

 Revision 1.36  2003/08/13 21:01:51  gbonthu
 bug:22771. added support for ERROR message from FIC

 Revision 1.35  2003/08/12 19:16:06  mcao
 Dev: warning remove

 Revision 1.34  2003/08/08 21:58:26  kbasavar
 DEV: CDR changes to handle forced call rel Type

 Revision 1.33  2003/08/08 18:03:36  mbelkode
 BUG: 21228 (For Ajay)

 Revision 1.32  2003/08/08 13:01:18  kbasavar
 DEV: Changes to fraud Call Handling feature

 Revision 1.31  2003/07/31 14:27:50  kbasavar
 DEV: incoming treatment handling for SCR and ANS_SUP fraud calls

 Revision 1.30  2003/07/30 19:47:29  gbonthu
 dev:correction to E04S88 code

 Revision 1.29  2003/07/29 22:19:39  mcao
 Dev:CPDI project

 Revision 1.28  2003/07/29 15:55:37  kbasavar
 DEV: merge from 39dev

 Revision 1.27  2003/07/28 22:46:01  kbasavar
 dev:39-DEV merge into MAIN

 Revision 1.26  2003/07/22 15:12:19  ajay
 DEV: Buffering code modified

 Revision 1.25  2003/07/20 23:39:02  ajay
 BUG:22771: temp work around for this bug

 Revision 1.24  2003/07/19 23:11:18  ajay
 BUG:22774: Error code bug, also checked in bug related to Milliwatt tone

 Revision 1.23  2003/07/11 21:08:07  ajay
 DEV: To support billing, at every Analyze Route from SCP

 Revision 1.22  2003/07/10 14:16:08  ajay
 BUG:22503, 22351: And also some code to fix memory leak of the routeEvnt

 Revision 1.21  2003/07/10 13:57:59  praveen
 BUG:22497

 Revision 1.20  2003/06/21 19:10:19  jjohn
 DEV: procLogMsg format cleanup

 Revision 1.19  2003/06/19 19:42:13  ajay
 DEV: Merge NewSouth changes from BRANCH-3-7 to MAIN(3.8)

 Revision 1.18  2003/06/05 15:35:36  jjohn
 Bug: 21720 added E15S57 & E15S58

 Revision 1.17  2003/05/23 18:41:50  ajay
 Bug:21445: Added code to handle forced onhook, while in between ACB processing

 Revision 1.16  2003/05/15 20:10:58  bpolhemus
 BUG: 21175 Add routines to handle setup timer expiry in state 57 CCS_AWTATDCFM

 Revision 1.15  2003/05/07 22:25:46  praveen
 BUG:20945, migrated bug fix from 3.7

 Revision 1.14  2003/05/02 20:22:44  ajay
 Bug:20526: Not stopping the O/T NoAnswer timers on Reconnect

 Revision 1.13  2003/04/24 15:16:18  ajay
 DEV: Replaced release cause : AM_CV_TMPFAIL with CCTMPFAIL

 Revision 1.12  2003/04/18 14:00:35  praveen
 BUG:19674

 Revision 1.11  2003/04/15 13:31:26  ajay
 DEV: Ephemeral Service Indication changes

 Revision 1.10  2003/04/14 20:18:12  ajay
 Bug:19866:

 Revision 1.9  2003/04/14 15:46:24  praveen
 BUG:19674

 Revision 1.8  2003/04/09 18:44:05  kbasavar
 BUG: 19408 E01 buffering for AOTC

 Revision 1.7  2003/04/04 19:37:11  mbelkode
 BUG:19126 CloseTrans changes for FIC and GCC

 Revision 1.6  2003/03/26 23:04:47  gbonthu
 bugs:19299, 19389, 19431, 19432 and 19437. Added a new state in GCC. Updated GCC-interworking code to handle all cases of Xfer-query-hold for all IP protocols

 Revision 1.5  2003/03/13 19:11:09  praveen
 Handle SUSP/RESUME for MF-SS7 IW, Handle E14S72

 Revision 1.4  2003/03/08 21:16:36  praveen
 Bug 18422

 Revision 1.3  2003/03/03 21:04:07  ajay
 OCdPty request event support for Post Switching

 Revision 1.2  2003/03/03 16:02:36  ajay
 Dev: Some OCdPty stuff

 Revision 1.1  2003/03/02 00:02:40  kbasavar
 Function to buffer E01 in S59 and S76

************************************************************************/

