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

     Name:     Q.93x - body 5

     Type:     C source file

     Desc:     C source code for Q.93x Upper Layer, Lower Layer,
               System Service and Layer Management.

               Part 5: Network Matrix (1 of 2)

     File:     qn_bdy5.c

     Sid:      qn_bdy5.c 3.9  -  09/29/99 14:36:54

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
#include "cc_asn.h"       /*ISDN Facility element decode header  */

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
#include "cm_lib.x"
#include "cm_pftha.x"      /* common PSF */
#include "lzq.x"           /* ISDN PSF management */
#include "zq.x"            /* ISDN PSF */
#include "cm_queue.h"
#include "cm_tree.x"
#include "trc_call.h"


/* local defines */

#define F FALSE
#define T TRUE

/* local typedefs */
  
/* local externs */
extern U8 callDir;

/* forward references */


/* public variable declarations */

/* matrices for checking message compatibility with call state */
/* TR6 and AUS matrices not defined, as network side is not supported for them */


/* Following are the call states -

       U00, N00 - null                     - c, p, u 
       U01, N01 - call initiated           - c, p, u 
       U02, N02 - overlap sending          - c, u 
       U03, N03 - outgoing call proceeding - c, p, u 
       U04, N04 - call delivered           - c, u 
       U05, N05 - not defined              - c, p, u 
       U06, N06 - call present             - c, p, u 
       U07, N07 - call received            - c, p 
       U08, N08 - connect request          - c, p, u 
       U09, N09 - incoming call proceeding - c, p, u 
       U10, N10 - active call              - c, p, u 
       U11, N11 - disconnect request       - c, p 
       U12, N12 - disconnect indictation   - c, p 
       U13, N13 - wait on release complete - c, p, u 
       U14, N14 - not defined              - c, p, u 
       U15, N15 - suspend request          - c 
       U16, N16 - not defined              - c, p, u 
       U17, N17 - resume request           - c 
       U18, N18 - tone active              - c 
       U19, N19 - release request          - c, p, u 
       U20, N20 - not defined              - c, p, u 
       U21, N21 - not defined              - c, p, u 
       ---, N22 - call abort               - c, p, u 
       U23, N23 - not defined              - c, p, u 
       U24, N24 - not defined              - c, p, u 
       U25, N25 - overlap receiving        - c, u 
       U26, N26 - not defined              - c, p, u
       U27, N27 - not defined              - c, p, u
       U28, N28 - not defined              - c, p, u
       U29, N29 - not defined              - c, p, u
       U30, N30 - not defined              - c, p, u
       U31, N31 - call independent         - c, p, u

   Following are the compatibility values -

       F - FALSE, message is not compatible with this call state
       T - TRUE,  message is compatible with this call state
*/


/* ccitt: network side matrix for checking msg compatibility with call state */

Bool inNetMsgComp1[][NMB_CON_ST] =
{
   /* Alerting */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, T, F, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, T, F, F, F, F, F, F},

   /* Call Proceeding */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, T, F, F, F, F, F, F},

   /* Connect */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, T, T, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, T, F, F, F, F, F, F},

   /* Connect Acknowledge */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Progress */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, T, T, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, T, F, F, F, F, F, F},

   /* Setup */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Setup Acknowledge */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Resume */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Resume Acknowledge */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Resume Reject */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Suspend */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Suspend Acknowledge */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Suspend Reject */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* User Information */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, T, T, T, T, F, T, T, T, T, T, T, T, F, F, T, F, T, T, T, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Disconnect */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, T, T, T, T, F, T, T, T, T, T, F, T, F, F, T, F, T, F, F, F, F, F, F, F, T, F, F, F, F, F, F},

   /* Release */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, T, T, T, T, F, T, T, T, T, T, T, T, F, F, T, F, T, F, T, F, F, F, F, F, T, F, F, F, F, F, F},

   /* Release Complete */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   T, T, T, T, T, F, T, T, T, T, T, T, T, F, F, T, F, T, F, T, F, F, F, F, F, T, F, F, F, F, F, F},

   /* Congestion Control */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, T, F, F, T, F, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Information */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   T, F, T, T, T, F, F, T, T, T, T, T, T, F, F, F, F, F, F, F, F, F, F, F, F, T, F, F, F, F, F, F},

   /* Information */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   T, F, T, T, T, F, F, T, T, T, T, T, T, F, F, F, F, F, F, F, F, F, F, F, F, T, F, F, F, F, F, F},

   /* Notify */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, T, T, T, T, F, T, T, T, T, T, T, T, F, F, T, F, T, T, T, F, F, F, F, F, T, F, F, F, F, F, F},

   /* Status */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   T, T, T, T, T, F, T, T, T, T, T, T, T, F, F, T, F, T, T, T, F, F, F, F, F, T, F, F, F, F, F, F},

   /* Status Enquiry */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   T, T, T, T, T, F, T, T, T, T, T, T, T, F, F, T, F, T, T, T, F, F, F, F, F, T, F, F, F, F, F, F},


   /* Facility */
 
/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, F, F, F, F, F, F},
 
   /* Register */ /* FID 16970.0 */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},
 
   /* Facility Ack */
 
/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, F, F, F, F, F, F},
 
   /* Facility Reject */
 
/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, F, F, F, F, F, F},
};
/* end of ccitt network compatibility matrix */


#ifdef ATT

/* att5eb: network side matrix for checking msg compatibility with call state */

Bool inNetMsgComp2[][NMB_CON_ST] =
{
   /* Alerting */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, T, T, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Call Proceeding */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Connect */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, T, T, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Connect Acknowledge */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Progress */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, T, T, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Setup */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Setup Acknowledge */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Resume */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Resume Acknowledge */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Resume Reject */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Suspend */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Suspend Acknowledge */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{  F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Suspend Reject */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* User Information */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, T, T, T, T, F, T, T, T, T, T, T, T, F, F, T, F, T, T, T, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Disconnect */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, T, T, T, T, F, T, T, T, T, T, F, T, F, F, T, F, T, T, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Release */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, T, T, T, T, F, T, T, T, T, T, T, T, F, F, T, F, T, T, T, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Release Complete */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   T, T, T, T, T, F, T, T, T, T, T, T, T, F, F, T, F, T, T, T, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Congestion Control */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, T, F, F, T, F, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Information */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   T, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Information */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{  T, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Notify */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, T, T, T, T, F, T, T, T, T, T, T, T, F, F, T, F, T, T, T, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Status */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   T, T, T, T, T, F, T, T, T, T, T, T, T, F, F, T, F, T, T, T, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Status Enquiry */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   T, T, T, T, T, F, T, T, T, T, T, T, T, F, F, T, F, T, T, T, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Facility */
 
/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, F, F, F, F, F, F},
   
   /* Register */ /* FID 16970.0 */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},
 
   /* Facility Ack */
 
/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, F, F, F, F, F, F},
 
   /* Facility Reject */
 
/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, F, F, F, F, F, F},
};
/* end of att5eb network compatibility matrix */


/* att5ep: network side matrix for checking msg compatibility with call state */

Bool inNetMsgComp3[][NMB_CON_ST] =
{
   /* Alerting */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, T, T, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Call Proceeding */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Connect */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, T, T, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Connect Acknowledge */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Progress */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, T, T, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Setup */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Setup Acknowledge */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Resume */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Resume Acknowledge */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Resume Reject */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Suspend */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Suspend Acknowledge */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Suspend Reject */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{  F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* User Information */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, T, T, T, T, F, T, T, T, T, T, T, T, F, F, T, F, T, T, T, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Disconnect */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, T, T, T, T, F, T, T, T, T, T, F, T, F, F, T, F, T, T, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Release */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, T, T, T, T, F, T, T, T, T, T, T, T, F, F, T, F, T, T, T, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Release Complete */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   T, T, T, T, T, F, T, T, T, T, T, T, T, F, F, T, F, T, T, T, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Congestion Control */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, T, F, F, T, F, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Information */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Information */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Notify */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, T, T, T, T, F, T, T, T, T, T, T, T, F, F, T, F, T, T, T, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Status */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   T, T, T, T, T, F, T, T, T, T, T, T, T, F, F, T, F, T, T, T, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Status Enquiry */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   T, T, T, T, T, F, T, T, T, T, T, T, T, F, F, T, F, T, T, T, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Facility */
 
/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, F, F, F, F, F, F},

  /* Register */ /* FID 16970.0 */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},
 
   /* Facility Ack */
 
/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, F, F, F, F, F, F},
 
   /* Facility Reject */
 
/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, F, F, F, F, F, F},
};
/* end of att5ep network compatibility matrix */


/* att4e: network side matrix for checking msg compatibility with call state */

Bool inNetMsgComp4[][NMB_CON_ST] =
{
   /* Alerting */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, T, T, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Call Proceeding */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Connect */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, T, T, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Connect Acknowledge */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Progress */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, T, T, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Setup */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Setup Acknowledge */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Resume */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Resume Acknowledge */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Resume Reject */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Suspend */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Suspend Acknowledge */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Suspend Reject */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* User Information */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, T, T, T, T, F, T, T, T, T, T, T, T, F, F, T, F, T, T, T, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Disconnect */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, T, T, T, T, F, T, T, T, T, T, F, T, F, F, T, F, T, T, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Release */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, T, T, T, T, F, T, T, T, T, T, T, T, F, F, T, F, T, T, T, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Release Complete */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   T, T, T, T, T, F, T, T, T, T, T, T, T, F, F, T, F, T, T, T, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Congestion Control */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, T, F, F, T, F, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Information */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Information */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Notify */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, T, T, T, T, F, T, T, T, T, T, T, T, F, F, T, F, T, T, T, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Status */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   T, T, T, T, T, F, T, T, T, T, T, T, T, F, F, T, F, T, T, T, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Status Enquiry */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   T, T, T, T, T, F, T, T, T, T, T, T, T, F, F, T, F, T, T, T, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Facility */
 
/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, F, F, F, F, F, F},
   
   /* Register */ /* FID 16970.0 */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},
 
   /* Facility Ack */
 
/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, F, F, F, F, F, F},
 
   /* Facility Reject */
 
/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, F, F, F, F, F, F},
};
/* end of att4e network compatibility matrix */
#endif


#ifdef NT

/* ntdms100b: network side matrix for checking msg compatibility with call state */

Bool inNetMsgComp5[][NMB_CON_ST] =
{
   /* Alerting */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, T, T, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Call Proceeding */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Connect */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, T, T, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Connect Acknowledge */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Progress */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, T, T, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Setup */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Setup Acknowledge */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   T, F, F, F, F, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Resume */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Resume Acknowledge */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Resume Reject */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Suspend */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Suspend Acknowledge */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Suspend Reject */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* User Information */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, T, T, T, T, F, T, T, T, T, T, T, T, F, F, T, F, T, T, T, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Disconnect */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, T, T, T, T, F, T, T, T, T, T, F, T, F, F, T, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Release */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, T, T, T, T, F, T, T, T, T, T, T, T, F, F, T, F, T, F, T, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Release Complete */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   T, T, T, T, T, F, T, T, T, T, T, T, T, F, F, T, F, T, F, T, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Congestion Control */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, T, F, F, T, F, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Information */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   T, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Information */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   T, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Notify */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, T, T, T, T, F, T, T, T, T, T, T, T, F, F, T, F, T, T, T, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Status */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   T, T, T, T, T, F, T, T, T, T, T, T, T, F, F, T, F, T, T, T, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Status Enquiry */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   T, T, T, T, T, F, T, T, T, T, T, T, T, F, F, T, F, T, T, T, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Facility */
 
/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, F, F, F, F, F, F},
    
   /* Register */ /* FID 16970.0 */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},
 
   /* Facility Ack */
 
/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, F, F, F, F, F, F},
 
   /* Facility Reject */
 
/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, F, F, F, F, F, F},
};
/* end of ntdms100b network compatibility matrix */


/* ntdms100p: network side matrix for checking msg compatibility with call state */

Bool inNetMsgComp6[][NMB_CON_ST] =
{
   /* Alerting */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, T, T, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Call Proceeding */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Connect */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, T, T, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Connect Acknowledge */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Progress */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, T, T, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Setup */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Setup Acknowledge */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Resume */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Resume Acknowledge */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Resume Reject */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Suspend */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Suspend Acknowledge */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Suspend Reject */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* User Information */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, T, T, T, T, F, T, T, T, T, T, T, T, F, F, T, F, T, T, T, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Disconnect */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, T, T, T, T, F, T, T, T, T, T, F, T, F, F, T, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Release */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   T, T, T, T, T, F, T, T, T, T, T, T, T, F, F, T, F, T, F, T, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Release Complete */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   T, T, T, T, T, F, T, T, T, T, T, T, T, F, F, T, F, T, F, T, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Congestion Control */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, T, F, F, T, F, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Information */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Information */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Notify */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, T, T, T, T, F, T, T, T, T, T, T, T, F, F, T, F, T, T, T, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Status */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   T, T, T, T, T, F, T, T, T, T, T, T, T, F, F, T, F, T, T, T, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Status Enquiry */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   T, T, T, T, T, F, T, T, T, T, T, T, T, F, F, T, F, T, T, T, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Facility */
 
/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, F, F, F, F, F, F},
    
   /* Register */ /* FID 16970.0 */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},
 
   /* Facility Ack */
 
/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, F, F, F, F, F, F},
 
   /* Facility Reject */
 
/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, F, F, F, F, F, F},
};
/* end of ntdms100p network compatibility matrix */
#endif







#ifdef ETSI                       /* TELICA-dsolanki-INTMGCDEV */

/* etsi: network side matrix for checking msg compatibility with call state */

Bool inNetMsgComp14[][NMB_CON_ST] =
{
   /* Alerting */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, T, F, F, T, F, F, F, F, F, F, F, F, F, F, F, F, T, F, F, T, F, F, F, F, F, F},

   /* Call Proceeding */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, T, F, F, T, F, F, F, F, F, F},

   /* Connect */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, T, T, F, T, F, F, F, F, F, F, F, F, F, F, F, F, T, F, F, T, F, F, F, F, F, F},

   /* Connect Acknowledge */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Progress */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, T, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, T, F, F, F, F, F, F},

   /* Setup */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Setup Acknowledge */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, T, F, F, F, F, F, F, F, F, F},

   /* Resume */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Resume Acknowledge */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Resume Reject */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Suspend */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Suspend Acknowledge */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Suspend Reject */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* User Information */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, T, T, T, T, F, T, T, T, T, T, T, T, F, F, T, F, T, T, T, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Disconnect */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, T, T, T, T, F, T, T, T, T, T, F, T, F, F, T, F, T, F, T, F, F, F, F, F, T, F, F, F, F, F, F},

   /* Release */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, T, T, T, T, F, T, T, T, T, T, T, T, T, F, T, F, T, F, T, F, F, T, F, F, T, F, F, F, F, F, T},

   /* Release Complete */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   T, T, T, T, T, F, T, T, T, T, T, T, T, T, F, T, F, T, F, T, F, F, T, F, F, T, F, F, F, F, F, T},

   /* Congestion Control */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, T, F, F, T, F, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Information */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   T, F, T, T, T, F, F, T, T, T, T, T, T, F, F, F, F, F, F, T, F, F, F, F, F, T, F, F, F, F, F, F},

   /* Information */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   T, F, T, T, T, F, F, T, T, T, T, T, T, F, F, F, F, F, F, T, F, F, F, F, F, T, F, F, F, F, F, F},

   /* Notify */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, T, T, T, T, F, T, T, T, T, T, T, T, F, F, T, F, T, T, T, F, F, F, F, F, T, F, F, F, F, F, F},

   /* Status */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   T, T, T, T, T, F, T, T, T, T, T, T, T, F, F, T, F, T, T, T, F, F, F, F, F, T, F, F, F, F, F, F},

   /* Status Enquiry */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   T, T, T, T, T, F, T, T, T, T, T, T, T, F, F, T, F, T, T, T, F, F, F, F, F, T, F, F, F, F, F, F},

   /* Facility */
 
/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, T, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, T},

   /* Register */ /* FID 16970.0 */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},
 
   /* Facility Ack */
 
/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, F, F, F, F, F, F},
 
   /* Facility Reject */
 
/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, F, F, F, F, F, F},
};
/* end of etsi network compatibility matrix */
#endif                                  /* TELICA-dsolanki-INTMGCDEV */



#ifdef NTDMS250

/* ntdms250p: network side matrix for checking msg compatibility with call state */

Bool inNetMsgComp17[][NMB_CON_ST] =
{
   /* Alerting */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, T, T, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Call Proceeding */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Connect */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, T, T, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Connect Acknowledge */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Progress */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, T, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Setup */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Setup Acknowledge */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Resume */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Resume Acknowledge */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Resume Reject */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Suspend */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Suspend Acknowledge */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Suspend Reject */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* User Information */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Disconnect */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, T, F, T, T, F, T, T, T, T, T, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Release */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, T, T, F, T, T, T, T, T, F, T, F, F, F, F, F, F, T, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Release Complete */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   T, T, F, T, T, F, T, T, T, T, T, T, T, F, F, F, F, F, F, T, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Congestion Control */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Information */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Information */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Notify */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, T, F, T, T, F, T, T, T, T, T, T, T, F, F, F, F, F, F, T, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Status */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   T, T, F, T, T, F, T, T, T, T, T, T, T, F, F, F, F, F, F, T, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Status Enquiry */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   T, T, F, T, T, F, T, T, T, T, T, T, T, F, F, F, F, F, F, T, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Facility */
 
/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, T, F, T, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},
    
   /* Register */ /* FID 16970.0 */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},
 
   /* Facility Ack */
 
/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},
 
   /* Facility Reject */
 
/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},
};
/* end of ntdms250p network compatibility matrix */
#endif





#ifdef NI2

Bool inNetMsgComp19[][NMB_CON_ST] =
{
   /* Alerting */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, T, F, F, T, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Call Proceeding */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, T, F, F, F, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Connect */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, T, T, F, T, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Connect Acknowledge */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, T, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Progress */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, T, F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Setup */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   T, T, F, T, T, F, T, T, T, T, T, T, T, F, F, F, F, F, F, T, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Setup Acknowledge */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Resume */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Resume Acknowledge */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Resume Reject */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Suspend */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Suspend Acknowledge */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Suspend Reject */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* User Information */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Disconnect */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, T, F, T, T, F, T, T, T, T, T, T, T, F, F, F, F, F, F, T, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Release */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, T, F, T, T, F, T, T, T, T, T, T, T, F, F, F, F, F, F, T, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Release Complete */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, T, F, T, T, F, T, T, T, T, T, T, T, F, F, F, F, F, F, T, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Congestion Control */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

/* Information */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Information - TR 6 */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Notify */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Status */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   T, T, F, T, T, F, T, T, T, T, T, T, T, F, F, F, F, F, F, T, F, F, F, F, F, F, F, F, F, F, F, F},

   /* Status Enquiry */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, T, F, T, T, F, T, T, T, T, T, T, T, F, F, F, F, F, F, T, F, F, F, F, F, F, F, F, F, F, F, F}, 

   /* Facility */
 
/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, T, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},
    
   /* Register */ /* FID 16970.0 */

/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},
 
   /* Facility Ack */
 
/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},
 
   /* Facility Reject */
 
/* 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
{   F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F},
};

#endif

/* ANSI T1.607 SDL's are used to fill this matrix */
/* ntmci: network side matrix for checking msg compatibility with call state */


/* ntni: network side matrix for checking msg compatibility with call state */


/* private variable declarations */

/* connection matrix - acting as network */

CONSTANT PFSIM inConNet[NMB_CON_EVNT][NMB_CON_ST] =
{
   /* Alerting */

   {inNetINVS00,      /* U00, N00 - null                     - c, p, u */
   inNetMSGINV,      /* U01, N01 - call initiated           - c, p, u */
   inNetMSGINV,      /* U02, N02 - overlap sending          - c, u */
   inNetMSGINV,      /* U03, N03 - outgoing call proceeding - c, p, u */
   inNetMSGINV,      /* U04, N04 - call delivered           - c, u */
   inIse,            /* U05, N05 - not defined              - c, p, u */
   inNetE00S06,      /* U06, N06 - call present             - c, p, u */
   inNetE00S07,      /* U07, N07 - call received            - c, p */
#ifdef ISDN_PTMPT
   inNetENDS08,
#else
   inNetMSGINV,      /* U08, N08 - connect request          - c, p, u */
#endif 
   inNetE00S09,      /* U09, N09 - incoming call proceeding - c, p, u */
   inNetMSGINV,      /* U10, N10 - active call              - c, p, u */
   inIgnore,         /* U11, N11 - disconnect request       - c, p */
   inIgnore,         /* U12, N12 - disconnect indication    - c, p */
   inIse,            /* U13, N13 - wait on release complete - c, p, u */
   inIse,            /* U14, N14 - not defined              - c, p, u */
   inNetMSGINV,      /* U15, N15 - suspend request          - c */
   inIse,            /* U16, N16 - not defined              - c, p, u */
   inNetMSGINV,      /* U17, N17 - resume request           - c */
   inNetMSGINV,      /* U18, N18 - tone active              - c */
   inIgnore,         /* U19, N19 - release request          - c, p, u */
   inIse,            /* U20, N20 - not defined              - c, p, u */
   inIse,            /* U21, N21 - not defined              - c, p, u */
   inCallAbortAllStates,  /* ---, N22 - call abort               - c, p, u */
   inIse,            /* U23, N23 - not defined              - c, p, u */
   inIse,            /* U24, N24 - not defined              - c, p, u */
   inNetE00S25,      /* U25, N25 - overlap receiving        - c, u */
   inIse,            /* U26, N26 - not defined              - c, p, u */
   inIse,            /* U27, N27 - not defined              - c, p, u */
   inIse,            /* U28, N28 - not defined              - c, p, u */
   inIse,            /* U29, N29 - not defined              - c, p, u */
   inIse,            /* U30, N30 - not defined              - c, p, u */
   inIgnore},        /* U31, N31 - call independent         - c, p, u */ 

   /* Call Proceeding */

   {inNetINVS00,      /* U00, N00 - null                     - c, p, u */
   inNetMSGINV,      /* U01, N01 - call initiated           - c, p, u */
   inNetMSGINV,      /* U02, N02 - overlap sending          - c, u */
   inNetMSGINV,      /* U03, N03 - outgoing call proceeding - c, p, u */
   inNetMSGINV,      /* U04, N04 - call delivered           - c, u */
   inIse,            /* U05, N05 - not defined              - c, p, u */
   inNetE01S06,      /* U06, N06 - call present             - c, p, u */
#ifdef ISDN_PTMPT
   inNetE01S07,      /* U07, N07 - call received            - c, p */
#else  /* not point to multipoint */
   inNetMSGINV,
#endif 
#ifdef ISDN_PTMPT
   inNetENDS08,
#else
   inNetMSGINV,      /* U08, N08 - connect request          - c, p, u */
#endif 
#ifdef ISDN_PTMPT
   inNetE01S07,
#else
   inNetMSGINV,      /* U09, N09 - incoming call proceeding - c, p, u */
#endif
   inNetMSGINV,      /* U10, N10 - active call              - c, p, u */
   inIgnore,         /* U11, N11 - disconnect request       - c, p */
   inIgnore,         /* U12, N12 - disconnect indication    - c, p */
   inIse,            /* U13, N13 - wait on release complete - c, p, u */
   inIse,            /* U14, N14 - not defined              - c, p, u */
   inNetMSGINV,      /* U15, N15 - suspend request          - c */
   inIse,            /* U16, N16 - not defined              - c, p, u */
   inNetMSGINV,      /* U17, N17 - resume request           - c */
   inNetMSGINV,      /* U18, N18 - tone active              - c */
   inIgnore,         /* U19, N19 - release request          - c, p, u */
   inIse,            /* U20, N20 - not defined              - c, p, u */
   inIse,            /* U21, N21 - not defined              - c, p, u */
   inCallAbortAllStates, /* ---, N22 - call abort               - c, p, u */
   inIse,            /* U23, N23 - not defined              - c, p, u */
   inIse,            /* U24, N24 - not defined              - c, p, u */
   inNetE01S25,      /* U25, N25 - overlap receiving        - c, u */
   inIse,            /* U26, N26 - not defined              - c, p, u */
   inIse,            /* U27, N27 - not defined              - c, p, u */
   inIse,            /* U28, N28 - not defined              - c, p, u */
   inIse,            /* U29, N29 - not defined              - c, p, u */
   inIse,            /* U30, N30 - not defined              - c, p, u */
   inIgnore},        /* U31, N31 - call independent         - c, p, u */

   /* Connect */

   {inNetINVS00,      /* U00, N00 - null                     - c, p, u */
   inNetMSGINV,      /* U01, N01 - call initiated           - c, p, u */
   inNetMSGINV,      /* U02, N02 - overlap sending          - c, u */
   inNetMSGINV,      /* U03, N03 - outgoing call proceeding - c, p, u */
   inNetMSGINV,      /* U04, N04 - call delivered           - c, u */
   inIse,            /* U05, N05 - not defined              - c, p, u */
   inNetE02S06,      /* U06, N06 - call present             - c, p, u */
   inNetE02S07,      /* U07, N07 - call received            - c, p */
#ifdef ISDN_PTMPT
   inNetENDS08,
#else
   inIgnore,         /* U08, N08 - connect request          - c, p, u */
#endif
   inNetE02S09,      /* U09, N09 - incoming call proceeding - c, p, u */
   inIgnore,         /* U10, N10 - active call              - c, p, u */
   inIgnore,         /* U11, N11 - disconnect request       - c, p */
   inIgnore,         /* U12, N12 - disconnect indication    - c, p */
   inIse,            /* U13, N13 - wait on release complete - c, p, u */
   inIse,            /* U14, N14 - not defined              - c, p, u */
   inNetMSGINV,      /* U15, N15 - suspend request          - c */
   inIse,            /* U16, N16 - not defined              - c, p, u */
   inNetMSGINV,      /* U17, N17 - resume request           - c */
   inNetMSGINV,      /* U18, N18 - tone active              - c */
   inIgnore,         /* U19, N19 - release request          - c, p, u */
   inIse,            /* U20, N20 - not defined              - c, p, u */
   inIse,            /* U21, N21 - not defined              - c, p, u */
   inCallAbortAllStates,  /* ---, N22 - call abort               - c, p, u */
   inIse,            /* U23, N23 - not defined              - c, p, u */
   inIse,            /* U24, N24 - not defined              - c, p, u */
   inNetE02S25,      /* U25, N25 - overlap receiving        - c, u */
   inIse,            /* U26, N26 - not defined              - c, p, u */
   inIse,            /* U27, N27 - not defined              - c, p, u */
   inIse,            /* U28, N28 - not defined              - c, p, u */
   inIse,            /* U29, N29 - not defined              - c, p, u */
   inIse,            /* U30, N30 - not defined              - c, p, u */
   inIgnore},        /* U31, N31 - call independent         - c, p, u */

   /* Connect Acknowledge */

   {inNetINVS00,      /* U00, N00 - null                     - c, p, u */
   inNetMSGINV,      /* U01, N01 - call initiated           - c, p, u */
   inNetMSGINV,      /* U02, N02 - overlap sending          - c, u */
   inNetMSGINV,      /* U03, N03 - outgoing call proceeding - c, p, u */
   inNetMSGINV,      /* U04, N04 - call delivered           - c, u */
   inIse,            /* U05, N05 - not defined              - c, p, u */
   inNetMSGINV,      /* U06, N06 - call present             - c, p, u */
   inNetMSGINV,      /* U07, N07 - call received            - c, p */
   inNetE03S08,      /* U08, N08 - connect request          - c, p, u */
   inNetMSGINV,      /* U09, N09 - incoming call proceeding - c, p, u */
   /* TELICA-BUG:5126-mqin-11/19/01 - cause code Issue 76 */
   inNetE03S10,      /* U10, N10 - active call              - c, p, u */
   inIgnore,         /* U11, N11 - disconnect request       - c, p */
   inIgnore,         /* U12, N12 - disconnect indication    - c, p */
   inIse,            /* U13, N13 - wait on release complete - c, p, u */
   inIse,            /* U14, N14 - not defined              - c, p, u */
   inNetMSGINV,      /* U15, N15 - suspend request          - c */
   inIse,            /* U16, N16 - not defined              - c, p, u */
   inNetMSGINV,      /* U17, N17 - resume request           - c */
   inNetMSGINV,      /* U18, N18 - tone active              - c */
   inIgnore,         /* U19, N19 - release request          - c, p, u */
   inIse,            /* U20, N20 - not defined              - c, p, u */
   inIse,            /* U21, N21 - not defined              - c, p, u */
   inIse,            /* ---, N22 - call abort               - c, p, u */
   inIse,            /* U23, N23 - not defined              - c, p, u */
   inIse,            /* U24, N24 - not defined              - c, p, u */
   inNetMSGINV,      /* U25, N25 - overlap receiving        - c, u */
   inIse,            /* U26, N26 - not defined              - c, p, u */
   inIse,            /* U27, N27 - not defined              - c, p, u */
   inIse,            /* U28, N28 - not defined              - c, p, u */
   inIse,            /* U29, N29 - not defined              - c, p, u */
   inIse,            /* U30, N30 - not defined              - c, p, u */
   inIgnore},        /* U31, N31 - call independent         - c, p, u */

   /* Progress */

   {inNetINVS00,      /* U00, N00 - null                     - c, p, u */
   inNetMSGINV,      /* U01, N01 - call initiated           - c, p, u */
   inNetMSGINV,      /* U02, N02 - overlap sending          - c, u */
   inNetMSGINV,      /* U03, N03 - outgoing call proceeding - c, p, u */
   inNetMSGINV,      /* U04, N04 - call delivered           - c, u */
   inIse,            /* U05, N05 - not defined              - c, p, u */
   inNetE04S06,      /* U06, N06 - call present             - c, p, u */
   inNetE04S07,      /* U07, N07 - call received            - c, p */
   inNetMSGINV,      /* U08, N08 - connect request          - c, p, u */
   inNetE04S09,      /* U09, N09 - incoming call proceeding - c, p, u */
   inNetMSGINV,      /* U10, N10 - active call              - c, p, u */
   inIgnore,         /* U11, N11 - disconnect request       - c, p */
   inIgnore,         /* U12, N12 - disconnect indication    - c, p */
   inIse,            /* U13, N13 - wait on release complete - c, p, u */
   inIse,            /* U14, N14 - not defined              - c, p, u */
   inNetMSGINV,      /* U15, N15 - suspend request          - c */
   inIse,            /* U16, N16 - not defined              - c, p, u */
   inNetMSGINV,      /* U17, N17 - resume request           - c */
   inNetMSGINV,      /* U18, N18 - tone active              - c */
   inIgnore,         /* U19, N19 - release request          - c, p, u */
   inIse,            /* U20, N20 - not defined              - c, p, u */
   inIse,            /* U21, N21 - not defined              - c, p, u */
   inIse,            /* ---, N22 - call abort               - c, p, u */
   inIse,            /* U23, N23 - not defined              - c, p, u */
   inIse,            /* U24, N24 - not defined              - c, p, u */
   inNetE04S25,      /* U25, N25 - overlap receiving        - c, u */
   inIse,            /* U26, N26 - not defined              - c, p, u */
   inIse,            /* U27, N27 - not defined              - c, p, u */
   inIse,            /* U28, N28 - not defined              - c, p, u */
   inIse,            /* U29, N29 - not defined              - c, p, u */
   inIse,            /* U30, N30 - not defined              - c, p, u */
   inIgnore},        /* U31, N31 - call independent         - c, p, u */

   /* Setup */

   {inNetE05S00,      /* U00, N00 - null                     - c, p, u */
   inIgnore,         /* U01, N01 - call initiated           - c, p, u */
   inIgnore,         /* U02, N02 - overlap sending          - c, u */
   inNetVioErr,      /* U03, N03 - outgoing call proceeding - c, p, u */
   inNetVioErr,      /* U04, N04 - call delivered           - c, u */
   inIse,            /* U05, N05 - not defined              - c, p, u */
   inNetVioErr,      /* U06, N06 - call present             - c, p, u */
   inNetVioErr,      /* U07, N07 - call received            - c, p */
   inNetVioErr,      /* U08, N08 - connect request          - c, p, u */
   inNetVioErr,      /* U09, N09 - incoming call proceeding - c, p, u */
   inNetVioErr,      /* U10, N10 - active call              - c, p, u */
   inNetMSGINV,      /* U11, N11 - disconnect request       - c, p */
   inNetMSGINV,      /* U12, N12 - disconnect indication    - c, p */
   inIse,            /* U13, N13 - wait on release complete - c, p, u */
   inIse,            /* U14, N14 - not defined              - c, p, u */
   inNetMSGINV,      /* U15, N15 - suspend request          - c */
   inIse,            /* U16, N16 - not defined              - c, p, u */
   inNetMSGINV,      /* U17, N17 - resume request           - c */
   inNetMSGINV,      /* U18, N18 - tone active              - c */
   inNetMSGINV,      /* U19, N19 - release request          - c, p, u */
   inIse,            /* U20, N20 - not defined              - c, p, u */
   inIse,            /* U21, N21 - not defined              - c, p, u */
   inIse,            /* ---, N22 - call abort               - c, p, u */
   inIse,            /* U23, N23 - not defined              - c, p, u */
   inNetE05S24,      /* U24, N24 - not defined              - c, p, u */
   inIgnore,         /* U25, N25 - overlap receiving        - c, u */
   inIse,            /* U26, N26 - not defined              - c, p, u */
   inIse,            /* U27, N27 - not defined              - c, p, u */
   inIse,            /* U28, N28 - not defined              - c, p, u */
   inIse,            /* U29, N29 - not defined              - c, p, u */
   inIse,            /* U30, N30 - not defined              - c, p, u */
   inIgnore},        /* U31, N31 - call independent         - c, p, u */

   /* Setup Acknowledge */

   {inNetINVS00,      /* U00, N00 - null                     - c, p, u */
   inNetMSGINV,      /* U01, N01 - call initiated           - c, p, u */
   inNetMSGINV,      /* U02, N02 - overlap sending          - c, u */
   inNetMSGINV,      /* U03, N03 - outgoing call proceeding - c, p, u */
   inNetMSGINV,      /* U04, N04 - call delivered           - c, u */
   inIse,            /* U05, N05 - not defined              - c, p, u */
   inNetE06S06,      /* U06, N06 - call present             - c, p, u */
#ifdef ISDN_PTMPT 
   inNetE06S07,
#else /* Not point to multipoint. */
   inNetMSGINV,      /* U07, N07 - call received            - c, p */
#endif 
#ifdef ISDN_PTMPT
   inNetENDS08,
#else
   inNetMSGINV,      /* U08, N08 - connect request          - c, p, u */
#endif 
#ifdef ISDN_PTMPT
    inNetE06S07,
#else /* Not point to multipoint. */
   inNetMSGINV,      /* U09, N09 - incoming call proceeding - c, p, u */
#endif 
   inNetMSGINV,      /* U10, N10 - active call              - c, p, u */
   inNetMSGINV,      /* U11, N11 - disconnect request       - c, p */
   inIgnore,         /* U12, N12 - disconnect indication    - c, p */
   inIse,            /* U13, N13 - wait on release complete - c, p, u */
   inIse,            /* U14, N14 - not defined              - c, p, u */
   inNetMSGINV,      /* U15, N15 - suspend request          - c */
   inIse,            /* U16, N16 - not defined              - c, p, u */
   inNetMSGINV,      /* U17, N17 - resume request           - c */
   inNetMSGINV,      /* U18, N18 - tone active              - c */
   inIgnore,         /* U19, N19 - release request          - c, p, u */
   inIse,            /* U20, N20 - not defined              - c, p, u */
   inIse,            /* U21, N21 - not defined              - c, p, u */
   /* Marker PT_CHG#42 */
   inIse,            /* ---, N22 - call abort               - c, p, u */
   inIse,            /* U23, N23 - not defined              - c, p, u */
   inIse,            /* U24, N24 - not defined              - c, p, u */
#ifdef ISDN_PTMPT
   inNetE06S07,
#else
   inNetMSGINV,      /* U25, N25 - overlap receiving        - c, u */
#endif 
   inIse,            /* U26, N26 - not defined              - c, p, u */
   inIse,            /* U27, N27 - not defined              - c, p, u */
   inIse,            /* U28, N28 - not defined              - c, p, u */
   inIse,            /* U29, N29 - not defined              - c, p, u */
   inIse,            /* U30, N30 - not defined              - c, p, u */
   inIgnore},        /* U31, N31 - call independent         - c, p, u */

   /* Resume */

   {inNetE07S00,      /* U00, N00 - null                     - c, p, u */
   inNetMSGINV,      /* U01, N01 - call initiated           - c, p, u */
   inNetMSGINV,      /* U02, N02 - overlap sending          - c, u */
   inNetMSGINV,      /* U03, N03 - outgoing call proceeding - c, p, u */
   inNetMSGINV,      /* U04, N04 - call delivered           - c, u */
   inIse,            /* U05, N05 - not defined              - c, p, u */
   inNetMSGINV,      /* U06, N06 - call present             - c, p, u */
   inNetMSGINV,      /* U07, N07 - call received            - c, p */
   inNetMSGINV,      /* U08, N08 - connect request          - c, p, u */
   inNetMSGINV,      /* U09, N09 - incoming call proceeding - c, p, u */
   inNetMSGINV,      /* U10, N10 - active call              - c, p, u */
   inNetMSGINV,      /* U11, N11 - disconnect request       - c, p */
   inIgnore,         /* U12, N12 - disconnect indication    - c, p */
   inIse,            /* U13, N13 - wait on release complete - c, p, u */
   inIse,            /* U14, N14 - not defined              - c, p, u */
   inNetMSGINV,      /* U15, N15 - suspend request          - c */
   inIse,            /* U16, N16 - not defined              - c, p, u */
   inNetMSGINV,      /* U17, N17 - resume request           - c */
   inNetMSGINV,      /* U18, N18 - tone active              - c */
   inIgnore,         /* U19, N19 - release request          - c, p, u */
   inIse,            /* U20, N20 - not defined              - c, p, u */
   inIse,            /* U21, N21 - not defined              - c, p, u */
   inIse,            /* ---, N22 - call abort               - c, p, u */
   inIse,            /* U23, N23 - not defined              - c, p, u */
   inIse,            /* U24, N24 - not defined              - c, p, u */
   inNetMSGINV,      /* U25, N25 - overlap receiving        - c, u */
   inIse,            /* U26, N26 - not defined              - c, p, u */
   inIse,            /* U27, N27 - not defined              - c, p, u */
   inIse,            /* U28, N28 - not defined              - c, p, u */
   inIse,            /* U29, N29 - not defined              - c, p, u */
   inIse,            /* U30, N30 - not defined              - c, p, u */
   inIgnore},        /* U31, N31 - call independent         - c, p, u */

   /* Resume Acknowledge */

   {inNetINVS00,      /* U00, N00 - null                     - c, p, u */
   inNetMSGINV,      /* U01, N01 - call initiated           - c, p, u */
   inNetMSGINV,      /* U02, N02 - overlap sending          - c, u */
   inNetMSGINV,      /* U03, N03 - outgoing call proceeding - c, p, u */
   inNetMSGINV,      /* U04, N04 - call delivered           - c, u */
   inIse,            /* U05, N05 - not defined              - c, p, u */
   inNetMSGINV,      /* U06, N06 - call present             - c, p, u */
   inNetMSGINV,      /* U07, N07 - call received            - c, p */
   inNetMSGINV,      /* U08, N08 - connect request          - c, p, u */
   inNetMSGINV,      /* U09, N09 - incoming call proceeding - c, p, u */
   inNetMSGINV,      /* U10, N10 - active call              - c, p, u */
   inNetMSGINV,      /* U11, N11 - disconnect request       - c, p */
   inIgnore,         /* U12, N12 - disconnect indication    - c, p */
   inIse,            /* U13, N13 - wait on release complete - c, p, u */
   inIse,            /* U14, N14 - not defined              - c, p, u */
   inNetMSGINV,      /* U15, N15 - suspend request          - c */
   inIse,            /* U16, N16 - not defined              - c, p, u */
   inNetMSGINV,      /* U17, N17 - resume request           - c */
   inNetMSGINV,      /* U18, N18 - tone active              - c */
   inIgnore,         /* U19, N19 - release request          - c, p, u */
   inIse,            /* U20, N20 - not defined              - c, p, u */
   inIse,            /* U21, N21 - not defined              - c, p, u */
   inIse,            /* ---, N22 - call abort               - c, p, u */
   inIse,            /* U23, N23 - not defined              - c, p, u */
   inIse,            /* U24, N24 - not defined              - c, p, u */
   inNetMSGINV,      /* U25, N25 - overlap receiving        - c, u */
   inIse,            /* U26, N26 - not defined              - c, p, u */
   inIse,            /* U27, N27 - not defined              - c, p, u */
   inIse,            /* U28, N28 - not defined              - c, p, u */
   inIse,            /* U29, N29 - not defined              - c, p, u */
   inIse,            /* U30, N30 - not defined              - c, p, u */
   inIgnore},        /* U31, N31 - call independent         - c, p, u */

   /* Resume Reject */

   {inNetINVS00,      /* U00, N00 - null                     - c, p, u */
   inNetMSGINV,      /* U01, N01 - call initiated           - c, p, u */
   inNetMSGINV,      /* U02, N02 - overlap sending          - c, u */
   inNetMSGINV,      /* U03, N03 - outgoing call proceeding - c, p, u */
   inNetMSGINV,      /* U04, N04 - call delivered           - c, u */
   inIse,            /* U05, N05 - not defined              - c, p, u */
   inNetMSGINV,      /* U06, N06 - call present             - c, p, u */
   inNetMSGINV,      /* U07, N07 - call received            - c, p */
   inNetMSGINV,      /* U08, N08 - connect request          - c, p, u */
   inNetMSGINV,      /* U09, N09 - incoming call proceeding - c, p, u */
   inNetMSGINV,      /* U10, N10 - active call              - c, p, u */
   inNetMSGINV,      /* U11, N11 - disconnect request       - c, p */
   inIgnore,         /* U12, N12 - disconnect indication    - c, p */
   inIse,            /* U13, N13 - wait on release complete - c, p, u */
   inIse,            /* U14, N14 - not defined              - c, p, u */
   inNetMSGINV,      /* U15, N15 - suspend request          - c */
   inIse,            /* U16, N16 - not defined              - c, p, u */
   inNetMSGINV,      /* U17, N17 - resume request           - c */
   inNetMSGINV,      /* U18, N18 - tone active              - c */
   inIgnore,         /* U19, N19 - release request          - c, p, u */
   inIse,            /* U20, N20 - not defined              - c, p, u */
   inIse,            /* U21, N21 - not defined              - c, p, u */
   inIse,            /* ---, N22 - call abort               - c, p, u */
   inIse,            /* U23, N23 - not defined              - c, p, u */
   inIse,            /* U24, N24 - not defined              - c, p, u */
   inNetMSGINV,      /* U25, N25 - overlap receiving        - c, u */
   inIse,            /* U26, N26 - not defined              - c, p, u */
   inIse,            /* U27, N27 - not defined              - c, p, u */
   inIse,            /* U28, N28 - not defined              - c, p, u */
   inIse,            /* U29, N29 - not defined              - c, p, u */
   inIse,            /* U30, N30 - not defined              - c, p, u */
   inIgnore},        /* U31, N31 - call independent         - c, p, u */

   /* Suspend */

   {inNetINVS00,      /* U00, N00 - null                     - c, p, u */
   inNetMSGINV,      /* U01, N01 - call initiated           - c, p, u */
   inNetMSGINV,      /* U02, N02 - overlap sending          - c, u */
   inNetMSGINV,      /* U03, N03 - outgoing call proceeding - c, p, u */
   inNetMSGINV,      /* U04, N04 - call delivered           - c, u */
   inIse,            /* U05, N05 - not defined              - c, p, u */
   inNetMSGINV,      /* U06, N06 - call present             - c, p, u */
   inNetMSGINV,      /* U07, N07 - call received            - c, p */
   inNetMSGINV,      /* U08, N08 - connect request          - c, p, u */
   inNetMSGINV,      /* U09, N09 - incoming call proceeding - c, p, u */
   inNetE10S10,      /* U10, N10 - active call              - c, p, u */
   inNetMSGINV,      /* U11, N11 - disconnect request       - c, p */
   inIgnore,         /* U12, N12 - disconnect indication    - c, p */
   inIse,            /* U13, N13 - wait on release complete - c, p, u */
   inIse,            /* U14, N14 - not defined              - c, p, u */
   inNetMSGINV,      /* U15, N15 - suspend request          - c */
   inIse,            /* U16, N16 - not defined              - c, p, u */
   inNetMSGINV,      /* U17, N17 - resume request           - c */
   inNetMSGINV,      /* U18, N18 - tone active              - c */
   inIgnore,         /* U19, N19 - release request          - c, p, u */
   inIse,            /* U20, N20 - not defined              - c, p, u */
   inIse,            /* U21, N21 - not defined              - c, p, u */
   inIse,            /* ---, N22 - call abort               - c, p, u */
   inIse,            /* U23, N23 - not defined              - c, p, u */
   inIse,            /* U24, N24 - not defined              - c, p, u */
   inNetMSGINV,      /* U25, N25 - overlap receiving        - c, u */
   inIse,            /* U26, N26 - not defined              - c, p, u */
   inIse,            /* U27, N27 - not defined              - c, p, u */
   inIse,            /* U28, N28 - not defined              - c, p, u */
   inIse,            /* U29, N29 - not defined              - c, p, u */
   inIse,            /* U30, N30 - not defined              - c, p, u */
   inIgnore},        /* U31, N31 - call independent         - c, p, u */

   /* Suspend Acknowledge */

   {inNetINVS00,      /* U00, N00 - null                     - c, p, u */
   inNetMSGINV,      /* U01, N01 - call initiated           - c, p, u */
   inNetMSGINV,      /* U02, N02 - overlap sending          - c, u */
   inNetMSGINV,      /* U03, N03 - outgoing call proceeding - c, p, u */
   inNetMSGINV,      /* U04, N04 - call delivered           - c, u */
   inIse,            /* U05, N05 - not defined              - c, p, u */
   inNetMSGINV,      /* U06, N06 - call present             - c, p, u */
   inNetMSGINV,      /* U07, N07 - call received            - c, p */
   inNetMSGINV,      /* U08, N08 - connect request          - c, p, u */
   inNetMSGINV,      /* U09, N09 - incoming call proceeding - c, p, u */
   inNetMSGINV,      /* U10, N10 - active call              - c, p, u */
   inNetMSGINV,      /* U11, N11 - disconnect request       - c, p */
   inIgnore,         /* U12, N12 - disconnect indication    - c, p */
   inIse,            /* U13, N13 - wait on release complete - c, p, u */
   inIse,            /* U14, N14 - not defined              - c, p, u */
   inNetMSGINV,      /* U15, N15 - suspend request          - c */
   inIse,            /* U16, N16 - not defined              - c, p, u */
   inNetMSGINV,      /* U17, N17 - resume request           - c */
   inNetMSGINV,      /* U18, N18 - tone active              - c */
   inIgnore,         /* U19, N19 - release request          - c, p, u */
   inIse,            /* U20, N20 - not defined              - c, p, u */
   inIse,            /* U21, N21 - not defined              - c, p, u */
   inIse,            /* ---, N22 - call abort               - c, p, u */
   inIse,            /* U23, N23 - not defined              - c, p, u */
   inIse,            /* U24, N24 - not defined              - c, p, u */
   inNetMSGINV,      /* U25, N25 - overlap receiving        - c, u */
   inIse,            /* U26, N26 - not defined              - c, p, u */
   inIse,            /* U27, N27 - not defined              - c, p, u */
   inIse,            /* U28, N28 - not defined              - c, p, u */
   inIse,            /* U29, N29 - not defined              - c, p, u */
   inIse,            /* U30, N30 - not defined              - c, p, u */
   inIgnore},        /* U31, N31 - call independent         - c, p, u */

   /* Suspend Reject */

   {inNetINVS00,      /* U00, N00 - null                     - c, p, u */
   inNetMSGINV,      /* U01, N01 - call initiated           - c, p, u */
   inNetMSGINV,      /* U02, N02 - overlap sending          - c, u */
   inNetMSGINV,      /* U03, N03 - outgoing call proceeding - c, p, u */
   inNetMSGINV,      /* U04, N04 - call delivered           - c, u */
   inIse,            /* U05, N05 - not defined              - c, p, u */
   inNetMSGINV,      /* U06, N06 - call present             - c, p, u */
   inNetMSGINV,      /* U07, N07 - call received            - c, p */
   inNetMSGINV,      /* U08, N08 - connect request          - c, p, u */
   inNetMSGINV,      /* U09, N09 - incoming call proceeding - c, p, u */
   inNetMSGINV,      /* U10, N10 - active call              - c, p, u */
   inNetMSGINV,      /* U11, N11 - disconnect request       - c, p */
   inIgnore,         /* U12, N12 - disconnect indication    - c, p */
   inIse,            /* U13, N13 - wait on release complete - c, p, u */
   inIse,            /* U14, N14 - not defined              - c, p, u */
   inNetMSGINV,      /* U15, N15 - suspend request          - c */
   inIse,            /* U16, N16 - not defined              - c, p, u */
   inNetMSGINV,      /* U17, N17 - resume request           - c */
   inNetMSGINV,      /* U18, N18 - tone active              - c */
   inIgnore,         /* U19, N19 - release request          - c, p, u */
   inIse,            /* U20, N20 - not defined              - c, p, u */
   inIse,            /* U21, N21 - not defined              - c, p, u */
   inIse,            /* ---, N22 - call abort               - c, p, u */
   inIse,            /* U23, N23 - not defined              - c, p, u */
   inIse,            /* U24, N24 - not defined              - c, p, u */
   inNetMSGINV,      /* U25, N25 - overlap receiving        - c, u */
   inIse,            /* U26, N26 - not defined              - c, p, u */
   inIse,            /* U27, N27 - not defined              - c, p, u */
   inIse,            /* U28, N28 - not defined              - c, p, u */
   inIse,            /* U29, N29 - not defined              - c, p, u */
   inIse,            /* U30, N30 - not defined              - c, p, u */
   inIgnore},        /* U31, N31 - call independent         - c, p, u */

   /* User Information */

   {inNetINVS00,      /* U00, N00 - null                     - c, p, u */
   inNetE13DSC,      /* U01, N01 - call initiated           - c, p, u */
   inNetE13DSC,      /* U02, N02 - overlap sending          - c, u */
   inNetE13DSC,      /* U03, N03 - outgoing call proceeding - c, p, u */
   inNetE13SND,      /* U04, N04 - call delivered           - c, u */
   inIse,            /* U05, N05 - not defined              - c, p, u */
   inNetE13DSC,      /* U06, N06 - call present             - c, p, u */
   inNetE13SND,      /* U07, N07 - call received            - c, p */
   inNetE13DSC,      /* U08, N08 - connect request          - c, p, u */
   inNetE13DSC,      /* U09, N09 - incoming call proceeding - c, p, u */
   inNetE13SND,      /* U10, N10 - active call              - c, p, u */
   inNetE13DSC,      /* U11, N11 - disconnect request       - c, p */
   inNetE13DSC,      /* U12, N12 - disconnect indication    - c, p */
   inIse,            /* U13, N13 - wait on release complete - c, p, u */
   inIse,            /* U14, N14 - not defined              - c, p, u */
   inNetE13DSC,      /* U15, N15 - suspend request          - c */
   inIse,            /* U16, N16 - not defined              - c, p, u */
   inNetE13DSC,      /* U17, N17 - resume request           - c */
   inNetE13DSC,      /* U18, N18 - tone active              - c */
   inNetE13DSC,      /* U19, N19 - release request          - c, p, u */
   inIse,            /* U20, N20 - not defined              - c, p, u */
   inIse,            /* U21, N21 - not defined              - c, p, u */
   inIse,            /* ---, N22 - call abort               - c, p, u */
   inIse,            /* U23, N23 - not defined              - c, p, u */
   inIse,            /* U24, N24 - not defined              - c, p, u */
   inNetE13DSC,      /* U25, N25 - overlap receiving        - c, u */
   inIse,            /* U26, N26 - not defined              - c, p, u */
   inIse,            /* U27, N27 - not defined              - c, p, u */
   inIse,            /* U28, N28 - not defined              - c, p, u */
   inIse,            /* U29, N29 - not defined              - c, p, u */
   inIse,            /* U30, N30 - not defined              - c, p, u */
   inIgnore},        /* U31, N31 - call independent         - c, p, u */

   /* Disconnect */

   {inNetINVS00,      /* U00, N00 - null                     - c, p, u */
   inNetE14S01,      /* U01, N01 - call initiated           - c, p, u */
   inNetE14SND,      /* U02, N02 - overlap sending          - c, u */
   inNetE14SND,      /* U03, N03 - outgoing call proceeding - c, p, u */
   inNetE14SND,      /* U04, N04 - call delivered           - c, u */
   inIse,            /* U05, N05 - not defined              - c, p, u */
   inNetE14SND,      /* U06, N06 - call present             - c, p, u */
   inNetE14SND,      /* U07, N07 - call received            - c, p */
   inNetE14SND,      /* U08, N08 - connect request          - c, p, u */
   inNetE14S09,      /* U09, N09 - incoming call proceeding - c, p, u */
   inNetE14SND,      /* U10, N10 - active call              - c, p, u */
   inIgnore,         /* U11, N11 - disconnect request       - c, p */
   inNetE14S12,      /* U12, N12 - disconnect indication    - c, p */
   inIse,            /* U13, N13 - wait on release complete - c, p, u */
   inIse,            /* U14, N14 - not defined              - c, p, u */
   inNetE14SND,      /* U15, N15 - suspend request          - c */
   inIse,            /* U16, N16 - not defined              - c, p, u */
   inNetE14SND,      /* U17, N17 - resume request           - c */
   inNetE14S18,      /* U18, N18 - tone active              - c */
   inIgnore,         /* U19, N19 - release request          - c, p, u */
   inIse,            /* U20, N20 - not defined              - c, p, u */
   inIse,            /* U21, N21 - not defined              - c, p, u */
   inIse,            /* ---, N22 - call abort               - c, p, u */
   inIse,            /* U23, N23 - not defined              - c, p, u */
   inIse,            /* U24, N24 - not defined              - c, p, u */
   inNetE14SND,      /* U25, N25 - overlap receiving        - c, u */
   inIse,            /* U26, N26 - not defined              - c, p, u */
   inIse,            /* U27, N27 - not defined              - c, p, u */
   inIse,            /* U28, N28 - not defined              - c, p, u */
   inIse,            /* U29, N29 - not defined              - c, p, u */
   inIse,            /* U30, N30 - not defined              - c, p, u */
   inIgnore},     /* U31, N31 - call independent         - c, p, u */

   /* Release */

   {inNetE15S00,      /* U00, N00 - null                     - c, p, u */
   inNetE15SND,      /* U01, N01 - call initiated           - c, p, u */
   inNetE15SND,      /* U02, N02 - overlap sending          - c, u */
   inNetE15SND,      /* U03, N03 - outgoing call proceeding - c, p, u */
   inNetE15SND,      /* U04, N04 - call delivered           - c, u */
   inIse,            /* U05, N05 - not defined              - c, p, u */
   inNetE15SND,      /* U06, N06 - call present             - c, p, u */
   inNetE15SND,      /* U07, N07 - call received            - c, p */
   inNetE15SND,      /* U08, N08 - connect request          - c, p, u */
   inNetE15SND,      /* U09, N09 - incoming call proceeding - c, p, u */
   inNetE15SND,      /* U10, N10 - active call              - c, p, u */
   inNetE15SND,      /* U11, N11 - disconnect request       - c, p */
   inNetE15S12,      /* U12, N12 - disconnect indication    - c, p */
   inNetE15SND,      /* U13, N13 - wait on release complete - c, p, u */
   inIse,            /* U14, N14 - not defined              - c, p, u */
   inNetE15SND,      /* U15, N15 - suspend request          - c */
   inIse,            /* U16, N16 - not defined              - c, p, u */
   inNetE15SND,      /* U17, N17 - resume request           - c */
   inNetE15SND,      /* U18, N18 - tone active              - c */
   inNetE15S19,      /* U19, N19 - release request          - c, p, u */
   inIse,            /* U20, N20 - not defined              - c, p, u */
   inIse,            /* U21, N21 - not defined              - c, p, u */
   inCallAbortAllStates, /* ---, N22 - call abort               - c, p, u */
   inIse,            /* U23, N23 - not defined              - c, p, u */
   inIse,            /* U24, N24 - not defined              - c, p, u */
   inNetE15SND,      /* U25, N25 - overlap receiving        - c, u */
   inIse,            /* U26, N26 - not defined              - c, p, u */
   inIse,            /* U27, N27 - not defined              - c, p, u */
   inIse,            /* U28, N28 - not defined              - c, p, u */
   inIse,            /* U29, N29 - not defined              - c, p, u */
   inIse,            /* U30, N30 - not defined              - c, p, u */
   inNetE15SND},     /* U31, N31 - call independent         - c, p, u */

   /* Release Complete */

   {inNetE16S00,      /* U00, N00 - null                     - c, p, u */
   inNetE16SND,      /* U01, N01 - call initiated           - c, p, u */
   inNetE16SND,      /* U02, N02 - overlap sending          - c, u */
   inNetE16SND,      /* U03, N03 - outgoing call proceeding - c, p, u */
   inNetE16SND,      /* U04, N04 - call delivered           - c, u */
   inIse,            /* U05, N05 - not defined              - c, p, u */
   inNetE16SND,      /* U06, N06 - call present             - c, p, u */
   inNetE16SND,      /* U07, N07 - call received            - c, p */
   inNetE16SND,      /* U08, N08 - connect request          - c, p, u */
   inNetE16SND,      /* U09, N09 - incoming call proceeding - c, p, u */
   inNetE16SND,      /* U10, N10 - active call              - c, p, u */
   inNetE16SND,      /* U11, N11 - disconnect request       - c, p */
   inNetE16SND,      /* U12, N12 - disconnect indication    - c, p */
   inNetE16SND,      /* U13, N13 - wait on release complete - c, p, u */
   inIse,            /* U14, N14 - not defined              - c, p, u */
   inNetE16SND,      /* U15, N15 - suspend request          - c */
   inIse,            /* U16, N16 - not defined              - c, p, u */
   inNetE16SND,      /* U17, N17 - resume request           - c */
   inNetE16SND,      /* U18, N18 - tone active              - c */
   inNetE15S19,      /* U19, N19 - release request          - c, p, u */
   inIse,            /* U20, N20 - not defined              - c, p, u */
   inIse,            /* U21, N21 - not defined              - c, p, u */
   inCallAbortAllStates, /* ---, N22 - call abort               - c, p, u */
   inIse,            /* U23, N23 - not defined              - c, p, u */
   inIse,            /* U24, N24 - not defined              - c, p, u */
   inNetE16SND,      /* U25, N25 - overlap receiving        - c, u */
   inIse,            /* U26, N26 - not defined              - c, p, u */
   inIse,            /* U27, N27 - not defined              - c, p, u */
   inIse,            /* U28, N28 - not defined              - c, p, u */
   inIse,            /* U29, N29 - not defined              - c, p, u */
   inIse,            /* U30, N30 - not defined              - c, p, u */
   inNetE16SND},     /* U31, N31 - call independent         - c, p, u */

   /* Congestion Control */

   {inNetINVS00,      /* U00, N00 - null                     - c, p, u */
   inNetMSGINV,      /* U01, N01 - call initiated           - c, p, u */
   inNetMSGINV,      /* U02, N02 - overlap sending          - c, u */
   inNetMSGINV,      /* U03, N03 - outgoing call proceeding - c, p, u */
   inNetE18S10,      /* U04, N04 - call delivered           - c, u */
   inIse,            /* U05, N05 - not defined              - c, p, u */
   inNetMSGINV,      /* U06, N06 - call present             - c, p, u */
   inNetE18S10,      /* U07, N07 - call received            - c, p */
   inNetMSGINV,      /* U08, N08 - connect request          - c, p, u */
   inNetMSGINV,      /* U09, N09 - incoming call proceeding - c, p, u */
   inNetE18S10,      /* U10, N10 - active call              - c, p, u */
   inNetMSGINV,      /* U11, N11 - disconnect request       - c, p */
   inIgnore,         /* U12, N12 - disconnect indication    - c, p */
   inIse,            /* U13, N13 - wait on release complete - c, p, u */
   inIse,            /* U14, N14 - not defined              - c, p, u */
   inNetMSGINV,      /* U15, N15 - suspend request          - c */
   inIse,            /* U16, N16 - not defined              - c, p, u */
   inNetMSGINV,      /* U17, N17 - resume request           - c */
   inNetMSGINV,      /* U18, N18 - tone active              - c */
   inIgnore,         /* U19, N19 - release request          - c, p, u */
   inIse,            /* U20, N20 - not defined              - c, p, u */
   inIse,            /* U21, N21 - not defined              - c, p, u */
   inIse,            /* ---, N22 - call abort               - c, p, u */
   inIse,            /* U23, N23 - not defined              - c, p, u */
   inIse,            /* U24, N24 - not defined              - c, p, u */
   inNetMSGINV,      /* U25, N25 - overlap receiving        - c, u */
   inIse,            /* U26, N26 - not defined              - c, p, u */
   inIse,            /* U27, N27 - not defined              - c, p, u */
   inIse,            /* U28, N28 - not defined              - c, p, u */
   inIse,            /* U29, N29 - not defined              - c, p, u */
   inIse,            /* U30, N30 - not defined              - c, p, u */
   inIgnore},        /* U31, N31 - call independent         - c, p, u */

   /* Information */

   {inNetE19S00,      /* U00, N00 - null                     - c, p, u */
   inNetMSGINV,      /* U01, N01 - call initiated           - c, p, u */
   inNetE19S02,      /* U02, N02 - overlap sending          - c, u */
   inNetE19SND,      /* U03, N03 - outgoing call proceeding - c, p, u */
   inNetE19SND,      /* U04, N04 - call delivered           - c, u */
   inIse,            /* U05, N05 - not defined              - c, p, u */
   inNetMSGINV,      /* U06, N06 - call present             - c, p, u */
   inNetE19SND,      /* U07, N07 - call received            - c, p */
   inNetE19SND,      /* U08, N08 - connect request          - c, p, u */
   inNetE19SND,      /* U09, N09 - incoming call proceeding - c, p, u */
   inNetE19SND,      /* U10, N10 - active call              - c, p, u */
   inNetE19SND,      /* U11, N11 - disconnect request       - c, p */
   inNetE19S12,      /* U12, N12 - disconnect indication    - c, p */
   inIse,            /* U13, N13 - wait on release complete - c, p, u */
   inIse,            /* U14, N14 - not defined              - c, p, u */
   inNetMSGINV,      /* U15, N15 - suspend request          - c */
   inIse,            /* U16, N16 - not defined              - c, p, u */
   inNetMSGINV,      /* U17, N17 - resume request           - c */
   inNetMSGINV,      /* U18, N18 - tone active              - c */
   inIgnore,         /* U19, N19 - release request          - c, p, u */
   inIse,            /* U20, N20 - not defined              - c, p, u */
   inIse,            /* U21, N21 - not defined              - c, p, u */
   inIse,            /* ---, N22 - call abort               - c, p, u */
   inIse,            /* U23, N23 - not defined              - c, p, u */
   inNetE19S24,      /* U24, N24 - not defined              - c, p, u */
   inNetE19SND,      /* U25, N25 - overlap receiving        - c, u */
   inIse,            /* U26, N26 - not defined              - c, p, u */
   inIse,            /* U27, N27 - not defined              - c, p, u */
   inIse,            /* U28, N28 - not defined              - c, p, u */
   inIse,            /* U29, N29 - not defined              - c, p, u */
   inIse,            /* U30, N30 - not defined              - c, p, u */
   inIgnore},        /* U31, N31 - call independent         - c, p, u */

   /* Information  - TR6 */

   {inNetE19S00,      /* U00, N00 - null                     - c, p, u */
   inNetMSGINV,      /* U01, N01 - call initiated           - c, p, u */
   inNetE19S02,      /* U02, N02 - overlap sending          - c, u */
   inNetE19SND,      /* U03, N03 - outgoing call proceeding - c, p, u */
   inNetE19SND,      /* U04, N04 - call delivered           - c, u */
   inIse,            /* U05, N05 - not defined              - c, p, u */
   inNetMSGINV,      /* U06, N06 - call present             - c, p, u */
   inNetE19SND,      /* U07, N07 - call received            - c, p */
   inNetE19SND,      /* U08, N08 - connect request          - c, p, u */
   inNetE19SND,      /* U09, N09 - incoming call proceeding - c, p, u */
   inNetE19SND,      /* U10, N10 - active call              - c, p, u */
   inNetE19SND,      /* U11, N11 - disconnect request       - c, p */
   inNetE19S12,      /* U12, N12 - disconnect indication    - c, p */
   inIse,            /* U13, N13 - wait on release complete - c, p, u */
   inIse,            /* U14, N14 - not defined              - c, p, u */
   inNetMSGINV,      /* U15, N15 - suspend request          - c */
   inIse,            /* U16, N16 - not defined              - c, p, u */
   inNetMSGINV,      /* U17, N17 - resume request           - c */
   inNetMSGINV,      /* U18, N18 - tone active              - c */
   inIgnore,         /* U19, N19 - release request          - c, p, u */
   inIse,            /* U20, N20 - not defined              - c, p, u */
   inIse,            /* U21, N21 - not defined              - c, p, u */
   inIse,            /* ---, N22 - call abort               - c, p, u */
   inIse,            /* U23, N23 - not defined              - c, p, u */
   inNetE19S24,      /* U24, N24 - not defined              - c, p, u */
   inNetE19SND,      /* U25, N25 - overlap receiving        - c, u */
   inIse,            /* U26, N26 - not defined              - c, p, u */
   inIse,            /* U27, N27 - not defined              - c, p, u */
   inIse,            /* U28, N28 - not defined              - c, p, u */
   inIse,            /* U29, N29 - not defined              - c, p, u */
   inIse,            /* U30, N30 - not defined              - c, p, u */
   inIgnore},        /* U31, N31 - call independent         - c, p, u */

   /* Notify */

   {inNetINVS00,      /* U00, N00 - null                     - c, p, u */
   inNetE20SND,      /* U01, N01 - call initiated           - c, p, u */
   inNetE20SND,      /* U02, N02 - overlap sending          - c, u */
   inNetE20SND,      /* U03, N03 - outgoing call proceeding - c, p, u */
   inNetE20SND,      /* U04, N04 - call delivered           - c, u */
   inIse,            /* U05, N05 - not defined              - c, p, u */
   inNetE20SND,      /* U06, N06 - call present             - c, p, u */
   inNetE20SND,      /* U07, N07 - call received            - c, p */
   inNetE20SND,      /* U08, N08 - connect request          - c, p, u */
   inNetE20SND,      /* U09, N09 - incoming call proceeding - c, p, u */
   inNetE20SND,      /* U10, N10 - active call              - c, p, u */
   inNetE20SND,      /* U11, N11 - disconnect request       - c, p */
   inNetE20SND,      /* U12, N12 - disconnect indication    - c, p */
   inIse,            /* U13, N13 - wait on release complete - c, p, u */
   inIse,            /* U14, N14 - not defined              - c, p, u */
   inNetE20SND,      /* U15, N15 - suspend request          - c */
   inIse,            /* U16, N16 - not defined              - c, p, u */
   inNetE20SND,      /* U17, N17 - resume request           - c */
   inNetE20SND,      /* U18, N18 - tone active              - c */
   inNetE20SND,      /* U19, N19 - release request          - c, p, u */
   inIse,            /* U20, N20 - not defined              - c, p, u */
   inIse,            /* U21, N21 - not defined              - c, p, u */
   inIse,            /* ---, N22 - call abort               - c, p, u */
   inIse,            /* U23, N23 - not defined              - c, p, u */
   inIse,            /* U24, N24 - not defined              - c, p, u */
   inNetE20SND,      /* U25, N25 - overlap receiving        - c, u */
   inIse,            /* U26, N26 - not defined              - c, p, u */
   inIse,            /* U27, N27 - not defined              - c, p, u */
   inIse,            /* U28, N28 - not defined              - c, p, u */
   inIse,            /* U29, N29 - not defined              - c, p, u */
   inIse,            /* U30, N30 - not defined              - c, p, u */
   inIgnore},        /* U31, N31 - call independent         - c, p, u */

   /* Status */

   {inNetE21S00,      /* U00, N00 - null                     - c, p, u */
   inNetE21SND,      /* U01, N01 - call initiated           - c, p, u */
   inNetE21SND,      /* U02, N02 - overlap sending          - c, u */
   inNetE21SND,      /* U03, N03 - outgoing call proceeding - c, p, u */
   inNetE21SND,      /* U04, N04 - call delivered           - c, u */
   inIse,            /* U05, N05 - not defined              - c, p, u */
   inNetE21SND,      /* U06, N06 - call present             - c, p, u */
   inNetE21SND,      /* U07, N07 - call received            - c, p */
   inNetE21SND,      /* U08, N08 - connect request          - c, p, u */
   inNetE21SND,      /* U09, N09 - incoming call proceeding - c, p, u */
   inNetE21SND,      /* U10, N10 - active call              - c, p, u */
   inNetE21SND,      /* U11, N11 - disconnect request       - c, p */
   inNetE21SND,      /* U12, N12 - disconnect indication    - c, p */
   inIse,            /* U13, N13 - wait on release complete - c, p, u */
   inIse,            /* U14, N14 - not defined              - c, p, u */
   inNetE21SND,      /* U15, N15 - suspend request          - c */
   inIse,            /* U16, N16 - not defined              - c, p, u */
   inNetE21SND,      /* U17, N17 - resume request           - c */
   inNetE21SND,      /* U18, N18 - tone active              - c */
   inNetE21S19,      /* U19, N19 - release request          - c, p, u */
   inIse,            /* U20, N20 - not defined              - c, p, u */
   inIse,            /* U21, N21 - not defined              - c, p, u */
   inIse,            /* ---, N22 - call abort               - c, p, u */
   inIse,            /* U23, N23 - not defined              - c, p, u */
   /* inIse -> inNetE21SND */
   inNetE21SND,      /* U24, N24 - Perm signal              - c, p, u */
   inNetE21SND,      /* U25, N25 - overlap receiving        - c, u */
   inIse,            /* U26, N26 - not defined              - c, p, u */
   inIse,            /* U27, N27 - not defined              - c, p, u */
   inIse,            /* U28, N28 - not defined              - c, p, u */
   inIse,            /* U29, N29 - not defined              - c, p, u */
   inIse,            /* U30, N30 - not defined              - c, p, u */
   inIgnore},        /* U31, N31 - call independent         - c, p, u */

   /* Status Enquiry */

   {inNetE22SND,      /* U00, N00 - null                     - c, p, u */
   inNetE22SND,      /* U01, N01 - call initiated           - c, p, u */
   inNetE22SND,      /* U02, N02 - overlap sending          - c, u */
   inNetE22SND,      /* U03, N03 - outgoing call proceeding - c, p, u */
   inNetE22SND,      /* U04, N04 - call delivered           - c, u */
   inIse,            /* U05, N05 - not defined              - c, p, u */
   inNetE22SND,      /* U06, N06 - call present             - c, p, u */
   inNetE22SND,      /* U07, N07 - call received            - c, p */
   inNetE22SND,      /* U08, N08 - connect request          - c, p, u */
   inNetE22SND,      /* U09, N09 - incoming call proceeding - c, p, u */
   inNetE22SND,      /* U10, N10 - active call              - c, p, u */
   inNetE22SND,      /* U11, N11 - disconnect request       - c, p */
   inNetE22SND,      /* U12, N12 - disconnect indication    - c, p */
   inIse,            /* U13, N13 - wait on release complete - c, p, u */
   inIse,            /* U14, N14 - not defined              - c, p, u */
   inNetE22SND,      /* U15, N15 - suspend request          - c */
   inIse,            /* U16, N16 - not defined              - c, p, u */
   inNetE22SND,      /* U17, N17 - resume request           - c */
   inNetE22SND,      /* U18, N18 - tone active              - c */
   inNetE22SND,      /* U19, N19 - release request          - c, p, u */
   inIse,            /* U20, N20 - not defined              - c, p, u */
   inIse,            /* U21, N21 - not defined              - c, p, u */
   inIse,            /* ---, N22 - call abort               - c, p, u */
   inIse,            /* U23, N23 - not defined              - c, p, u */
/* inIse -> inNetE22SND */
   inNetE22SND,      /* U24, N24 - not defined              - c, p, u */
   inNetE22SND,      /* U25, N25 - overlap receiving        - c, u */
   inIse,            /* U26, N26 - not defined              - c, p, u */
   inIse,            /* U27, N27 - not defined              - c, p, u */
   inIse,            /* U28, N28 - not defined              - c, p, u */
   inIse,            /* U29, N29 - not defined              - c, p, u */
   inIse,            /* U30, N30 - not defined              - c, p, u */
   inIgnore},        /* U31, N31 - call independent         - c, p, u */

   /* Facility */
   
   {inIgnore,         /*U00, N00 - null                     - c, p, u */
   inIgnore,         /* U01, N01 - call initiated           - c, p, u */
   inIgnore,         /* U02, N02 - overlap sending          - c, u */
   inIgnore,         /* U03, N03 - outgoing call proceeding - c, p, u */
   inIgnore,         /* U04, N04 - call delivered           - c, u */
   inIgnore,         /* U05, N05 - not defined              - c, p, u */
   inIgnore,         /* U06, N06 - call present             - c, p, u */
   inNetE23S07,      /* U07, N07 - call received            - c, p */
   inIgnore,         /* U08, N08 - connect request          - c, p, u */
   inNetE23S09,      /* U09, N09 - incoming call proceeding - c, p, u */
   inIgnore,         /* U10, N10 - active call              - c, p, u */
   inIgnore,         /* U11, N11 - disconnect request       - c, p */
   inIgnore,         /* U12, N12 - disconnect indication    - c, p */
   inIgnore,         /* U13, N13 - wait on release complete - c, p, u */
   inIgnore,         /* U14, N14 - not defined              - c, p, u */
   inIgnore,         /* U15, N15 - suspend request          - c */
   inIgnore,         /* U16, N16 - not defined              - c, p, u */
   inIgnore,         /* U17, N17 - resume request           - c */
   inIgnore,         /* U18, N18 - tone active              - c */
   inIgnore,         /* U19, N19 - release request          - c, p, u */
   inIgnore,         /* U20, N20 - not defined              - c, p, u */
   inIgnore,         /* U21, N21 - not defined              - c, p, u */
   inIgnore,         /* ---, N22 - call abort               - c, p, u */
   inIgnore,         /* U23, N23 - not defined              - c, p, u */
   inIgnore,         /* U24, N24 - not defined              - c, p, u */
   inIgnore,         /* U25, N25 - overlap receiving        - c, u */
   inIse,            /* U26, N26 - not defined              - c, p, u */
   inIse,            /* U27, N27 - not defined              - c, p, u */
   inIse,            /* U28, N28 - not defined              - c, p, u */
   inIse,            /* U29, N29 - not defined              - c, p, u */
   inIse,            /* U30, N30 - not defined              - c, p, u */
   inNetE23S31},     /* U31, N31 - call independent         - c, p, u */

   /* Register */

   {inNetE24S00,        /* U00, N00 - null                     - c, p, u */
   inIgnore,            /* U01, N01 - call initiated           - c, p, u */
   inIgnore,            /* U02, N02 - overlap sending          - c, u */
   inIgnore,            /* U03, N03 - outgoing call proceeding - c, p, u */
   inIgnore,            /* U04, N04 - call delivered           - c, u */
   inIgnore,            /* U05, N05 - not defined              - c, p, u */
   inIgnore,            /* U06, N06 - call present             - c, p, u */
   inIgnore,            /* U07, N07 - call received            - c, p */
   inIgnore,            /* U08, N08 - connect request          - c, p, u */
   inIgnore,            /* U09, N09 - incoming call proceeding - c, p, u */
   inIgnore,            /* U10, N10 - active call              - c, p, u */
   inIgnore,            /* U11, N11 - disconnect request       - c, p */
   inIgnore,            /* U12, N12 - disconnect indication    - c, p */
   inIgnore,            /* U13, N13 - wait on release complete - c, p, u */
   inIgnore,            /* U14, N14 - not defined              - c, p, u */
   inIgnore,            /* U15, N15 - suspend request          - c */
   inIgnore,            /* U16, N16 - not defined              - c, p, u */
   inIgnore,            /* U17, N17 - resume request           - c */
   inIgnore,            /* U18, N18 - tone active              - c */
   inIgnore,            /* U19, N19 - release request          - c, p, u */
   inIgnore,            /* U20, N20 - not defined              - c, p, u */
   inIgnore,            /* U21, N21 - not defined              - c, p, u */
   inIgnore,            /* ---, N22 - call abort               - c, p, u */
   inIgnore,            /* U23, N23 - not defined              - c, p, u */
   inIgnore,            /* U24, N24 - not defined              - c, p, u */
   inIgnore,            /* U25, N25 - overlap receiving        - c, u */
   inIgnore,            /* U26, N26 - not defined              - c, p, u */
   inIgnore,            /* U27, N27 - not defined              - c, p, u */
   inIgnore,            /* U28, N28 - not defined              - c, p, u */
   inIgnore,            /* U29, N29 - not defined              - c, p, u */
   inIgnore,            /* U30, N30 - not defined              - c, p, u */
   inIgnore},           /* U31, N31 - call independent         - c, p, u */

   /* Connect Request */

   {inNetE27S00,      /* U00, N00 - null                     - c, p, u */
   inNetEVTINV,      /* U01, N01 - call initiated           - c, p, u */
   inNetEVTINV,      /* U02, N02 - overlap sending          - c, u */
   inNetEVTINV,      /* U03, N03 - outgoing call proceeding - c, p, u */
   inNetEVTINV,      /* U04, N04 - call delivered           - c, u */
   inIse,            /* U05, N05 - not defined              - c, p, u */
   inNetEVTINV,      /* U06, N06 - call present             - c, p, u */
   inNetEVTINV,      /* U07, N07 - call received            - c, p */
   inNetEVTINV,      /* U08, N08 - connect request          - c, p, u */
   inNetEVTINV,      /* U09, N09 - incoming call proceeding - c, p, u */
   inNetEVTINV,      /* U10, N10 - active call              - c, p, u */
   inNetEVTINV,      /* U11, N11 - disconnect request       - c, p */
   inNetEVTINV,      /* U12, N12 - disconnect indication    - c, p */
   inIse,            /* U13, N13 - wait on release complete - c, p, u */
   inIse,            /* U14, N14 - not defined              - c, p, u */
   inNetEVTINV,      /* U15, N15 - suspend request          - c */
   inIse,            /* U16, N16 - not defined              - c, p, u */
   inNetEVTINV,      /* U17, N17 - resume request           - c */
   inNetEVTINV,      /* U18, N18 - tone active              - c */
   inNetEVTINV,      /* U19, N19 - release request          - c, p, u */
   inIse,            /* U20, N20 - not defined              - c, p, u */
   inIse,            /* U21, N21 - not defined              - c, p, u */
   inIse,            /* ---, N22 - call abort               - c, p, u */
   inIse,            /* U23, N23 - not defined              - c, p, u */
   inIse,            /* U24, N24 - not defined              - c, p, u */
   inNetEVTINV,      /* U25, N25 - overlap receiving        - c, u */
   inIse,            /* U26, N26 - not defined              - c, p, u */
   inIse,            /* U27, N27 - not defined              - c, p, u */
   inIse,            /* U28, N28 - not defined              - c, p, u */
   inIse,            /* U29, N29 - not defined              - c, p, u */
   inIse,            /* U30, N30 - not defined              - c, p, u */
   inIse},           /* U31, N31 - call independent         - c, p, u */

   /* Connect Response */

   {inNetINES00,      /* U00, N00 - null                     - c, p, u */
   inNetE28S01,      /* U01, N01 - call initiated           - c, p, u */
   inNetE28S02,      /* U02, N02 - overlap sending          - c, u */
   inNetE28S03,      /* U03, N03 - outgoing call proceeding - c, p, u */
   inNetE28S03,      /* U04, N04 - call delivered           - c, u */
   inIse,            /* U05, N05 - not defined              - c, p, u */
   inNetEVTINV,      /* U06, N06 - call present             - c, p, u */
   inNetEVTINV,      /* U07, N07 - call received            - c, p */
   inNetEVTINV,      /* U08, N08 - connect request          - c, p, u */
   inNetEVTINV,      /* U09, N09 - incoming call proceeding - c, p, u */
   inNetEVTINV,      /* U10, N10 - active call              - c, p, u */
   inNetEVTINV,      /* U11, N11 - disconnect request       - c, p */
   inNetEVTINV,      /* U12, N12 - disconnect indication    - c, p */
   inIse,            /* U13, N13 - wait on release complete - c, p, u */
   inIse,            /* U14, N14 - not defined              - c, p, u */
   inNetEVTINV,      /* U15, N15 - suspend request          - c */
   inIse,            /* U16, N16 - not defined              - c, p, u */
   inNetEVTINV,      /* U17, N17 - resume request           - c */
   inNetEVTINV,      /* U18, N18 - tone active              - c */
   inNetEVTINV,      /* U19, N19 - release request          - c, p, u */
   inIse,            /* U20, N20 - not defined              - c, p, u */
   inIse,            /* U21, N21 - not defined              - c, p, u */
   inIse,            /* ---, N22 - call abort               - c, p, u */
   inIse,            /* U23, N23 - not defined              - c, p, u */
   inIse,            /* U24, N24 - not defined              - c, p, u */
   inNetEVTINV,      /* U25, N25 - overlap receiving        - c, u */
   inIse,            /* U26, N26 - not defined              - c, p, u */
   inIse,            /* U27, N27 - not defined              - c, p, u */
   inIse,            /* U28, N28 - not defined              - c, p, u */
   inIse,            /* U29, N29 - not defined              - c, p, u */
   inIse,            /* U30, N30 - not defined              - c, p, u */
   inIse},           /* U31, N31 - call independent         - c, p, u */

   /* Connection Status Request */

   {inNetINES00,      /* U00, N00 - null                     - c, p, u */
   inNetE29S01,      /* U01, N01 - call initiated           - c, p, u */
   inNetE29SND,      /* U02, N02 - overlap sending          - c, u */
   inNetE29SND,      /* U03, N03 - outgoing call proceeding - c, p, u */
   inNetE29S04,      /* U04, N04 - call delivered           - c, u */
   inIse,            /* U05, N05 - not defined              - c, p, u */
   inNetEVTINV,      /* U06, N06 - call present             - c, p, u */
   inNetE29S07,      /* U07, N07 - call received            - c, p */
   inNetE29S07,      /* U08, N08 - connect request          - c, p, u */
   inNetE29SND,      /* U09, N09 - incoming call proceeding - c, p, u */
   inNetE29S10,      /* U10, N10 - active call              - c, p, u */
   inNetE29S07,      /* U11, N11 - disconnect request       - c, p */
   inNetE29S07,      /* U12, N12 - disconnect indication    - c, p */
   inIse,            /* U13, N13 - wait on release complete - c, p, u */
   inIse,            /* U14, N14 - not defined              - c, p, u */
   inNetE29S15,      /* U15, N15 - suspend request          - c */
   inIse,            /* U16, N16 - not defined              - c, p, u */
   inNetEVTINV,      /* U17, N17 - resume request           - c */
   inNetEVTINV,      /* U18, N18 - tone active              - c */
   inNetEVTINV,      /* U19, N19 - release request          - c, p, u */
   inIse,            /* U20, N20 - not defined              - c, p, u */
   inIse,            /* U21, N21 - not defined              - c, p, u */
   inIse,            /* ---, N22 - call abort               - c, p, u */
   inIse,            /* U23, N23 - not defined              - c, p, u */
   inIse,            /* U24, N24 - not defined              - c, p, u */
   inNetE29SND,      /* U25, N25 - overlap receiving        - c, u */
   inIse,            /* U26, N26 - not defined              - c, p, u */
   inIse,            /* U27, N27 - not defined              - c, p, u */
   inIse,            /* U28, N28 - not defined              - c, p, u */
   inIse,            /* U29, N29 - not defined              - c, p, u */
   inIse,            /* U30, N30 - not defined              - c, p, u */
   inIse},           /* U31, N31 - call independent         - c, p, u */

   /* Disconnect Request */

   {inNetE31S00,     /* U00, N00 - null                     - c, p, u */
   inNetE30S01,      /* U01, N01 - call initiated           - c, p, u */
   inNetE30SND,      /* U02, N02 - overlap sending          - c, u */
   inNetE30S03,      /* U03, N03 - outgoing call proceeding - c, p, u */
   inNetE30SND,      /* U04, N04 - call delivered           - c, u */
   inIse,            /* U05, N05 - not defined              - c, p, u */
   inNetE30SND,      /* U06, N06 - call present             - c, p, u */
   inNetE30SND,      /* U07, N07 - call received            - c, p */
   inNetE30SND,      /* U08, N08 - connect request          - c, p, u */
   inNetE30SND,      /* U09, N09 - incoming call proceeding - c, p, u */
   inNetE30SND,      /* U10, N10 - active call              - c, p, u */
   inNetE30S11,      /* U11, N11 - disconnect request       - c, p */
   inIgnore,         /* U12, N12 - disconnect indication    - c, p */
   inIse,            /* U13, N13 - wait on release complete - c, p, u */
   inIse,            /* U14, N14 - not defined              - c, p, u */
   inNetE30SND,      /* U15, N15 - suspend request          - c */
   inIse,            /* U16, N16 - not defined              - c, p, u */
   inNetE30SND,      /* U17, N17 - resume request           - c */
   inNetE30SND,      /* U18, N18 - tone active              - c */
   inIgnore,         /* U19, N19 - release request          - c, p, u */
   inIse,            /* U20, N20 - not defined              - c, p, u */
   inIse,            /* U21, N21 - not defined              - c, p, u */
   inIse,            /* ---, N22 - call abort               - c, p, u */
   inIse,            /* U23, N23 - not defined              - c, p, u */
   inIse,            /* U24, N24 - not defined              - c, p, u */
   inNetE30SND,      /* U25, N25 - overlap receiving        - c, u */
   inIse,            /* U26, N26 - not defined              - c, p, u */
   inIse,            /* U27, N27 - not defined              - c, p, u */
   inIse,            /* U28, N28 - not defined              - c, p, u */
   inIse,            /* U29, N29 - not defined              - c, p, u */
   inIse,            /* U30, N30 - not defined              - c, p, u */
   inIgnore},        /* U31, N31 - call independent         - c, p, u */

   /* Release Request */

   {inNetE31S00,      /* U00, N00 - null                     - c, p, u */
   inNetE31SND,      /* U01, N01 - call initiated           - c, p, u */
   inNetE31SND,      /* U02, N02 - overlap sending          - c, u */
   inNetE31SND,      /* U03, N03 - outgoing call proceeding - c, p, u */
   inNetE31SND,      /* U04, N04 - call delivered           - c, u */
   inIse,            /* U05, N05 - not defined              - c, p, u */
   inNetE31S06,      /* U06, N06 - call present             - c, p, u */
   inNetE31SND,      /* U07, N07 - call received            - c, p */
   inNetE31SND,      /* U08, N08 - connect request          - c, p, u */
   inNetE31SND,      /* U09, N09 - incoming call proceeding - c, p, u */
   inNetE31SND,      /* U10, N10 - active call              - c, p, u */
   inNetE31SND,      /* U11, N11 - disconnect request       - c, p */
   inNetE31SND,      /* U12, N12 - disconnect indication    - c, p */
   inNetE31S13,      /* U13, N13 - wait on release complete - c, p, u */
   inIse,            /* U14, N14 - not defined              - c, p, u */
   inNetE31SND,      /* U15, N15 - suspend request          - c */
   inIse,            /* U16, N16 - not defined              - c, p, u */
   inNetE31SND,      /* U17, N17 - resume request           - c */
   inNetE31SND,      /* U18, N18 - tone active              - c */
   inIgnore,         /* U19, N19 - release request          - c, p, u */
   inIse,            /* U20, N20 - not defined              - c, p, u */
   inIse,            /* U21, N21 - not defined              - c, p, u */
   inIse,            /* ---, N22 - call abort               - c, p, u */
   inIse,            /* U23, N23 - not defined              - c, p, u */
   inIse,            /* U24, N24 - not defined              - c, p, u */
   inNetE31SND,      /* U25, N25 - overlap receiving        - c, u */
   inIse,            /* U26, N26 - not defined              - c, p, u */
   inIse,            /* U27, N27 - not defined              - c, p, u */
   inIse,            /* U28, N28 - not defined              - c, p, u */
   inIse,            /* U29, N29 - not defined              - c, p, u */
   inIse,            /* U30, N30 - not defined              - c, p, u */
   inNetE31SND},     /* U31, N31 - call independent         - c, p, u */

   /* Data Request */

   {inNetINES00,      /* U00, N00 - null                     - c, p, u */
   inNetEVTINV,      /* U01, N01 - call initiated           - c, p, u */
   inNetEVTINV,      /* U02, N02 - overlap sending          - c, u */
   inNetEVTINV,      /* U03, N03 - outgoing call proceeding - c, p, u */
   inNetE32S10,      /* U04, N04 - call delivered           - c, u */
   inIse,            /* U05, N05 - not defined              - c, p, u */
   inNetEVTINV,      /* U06, N06 - call present             - c, p, u */
   inNetE32S10,      /* U07, N07 - call received            - c, p */
   inNetEVTINV,      /* U08, N08 - connect request          - c, p, u */
   inNetEVTINV,      /* U09, N09 - incoming call proceeding - c, p, u */
   inNetE32S10,      /* U10, N10 - active call              - c, p, u */
   inNetEVTINV,      /* U11, N11 - disconnect request       - c, p */
   inIgnore,         /* U12, N12 - disconnect indication    - c, p */
   inIse,            /* U13, N13 - wait on release complete - c, p, u */
   inIse,            /* U14, N14 - not defined              - c, p, u */
   inNetEVTINV,      /* U15, N15 - suspend request          - c */
   inIse,            /* U16, N16 - not defined              - c, p, u */
   inNetEVTINV,      /* U17, N17 - resume request           - c */
   inNetEVTINV,      /* U18, N18 - tone active              - c */
   inNetEVTINV,      /* U19, N19 - release request          - c, p, u */
   inIse,            /* U20, N20 - not defined              - c, p, u */
   inIse,            /* U21, N21 - not defined              - c, p, u */
   inIse,            /* ---, N22 - call abort               - c, p, u */
   inIse,            /* U23, N23 - not defined              - c, p, u */
   inIse,            /* U24, N24 - not defined              - c, p, u */
   inNetEVTINV,      /* U25, N25 - overlap receiving        - c, u */
   inIse,            /* U26, N26 - not defined              - c, p, u */
   inIse,            /* U27, N27 - not defined              - c, p, u */
   inIse,            /* U28, N28 - not defined              - c, p, u */
   inIse,            /* U29, N29 - not defined              - c, p, u */
   inIse,            /* U30, N30 - not defined              - c, p, u */
   inIse},           /* U31, N31 - call independent         - c, p, u */

   /* Suspend/Halt Request */

   {inNetINES00,      /* U00, N00 - null                     - c, p, u */
   inNetEVTINV,      /* U01, N01 - call initiated           - c, p, u */
   inNetEVTINV,      /* U02, N02 - overlap sending          - c, u */
   inNetEVTINV,      /* U03, N03 - outgoing call proceeding - c, p, u */
   inNetEVTINV,      /* U04, N04 - call delivered           - c, u */
   inIse,            /* U05, N05 - not defined              - c, p, u */
   inNetEVTINV,      /* U06, N06 - call present             - c, p, u */
   inNetEVTINV,      /* U07, N07 - call received            - c, p */
   inNetEVTINV,      /* U08, N08 - connect request          - c, p, u */
   inNetEVTINV,      /* U09, N09 - incoming call proceeding - c, p, u */
   inNetEVTINV,      /* U10, N10 - active call              - c, p, u */
   inNetEVTINV,      /* U11, N11 - disconnect request       - c, p */
   inNetEVTINV,      /* U12, N12 - disconnect indication    - c, p */
   inIse,            /* U13, N13 - wait on release complete - c, p, u */
   inIse,            /* U14, N14 - not defined              - c, p, u */
   inNetEVTINV,      /* U15, N15 - suspend request          - c */
   inIse,            /* U16, N16 - not defined              - c, p, u */
   inNetEVTINV,      /* U17, N17 - resume request           - c */
   inNetEVTINV,      /* U18, N18 - tone active              - c */
   inNetEVTINV,      /* U19, N19 - release request          - c, p, u */
   inIse,            /* U20, N20 - not defined              - c, p, u */
   inIse,            /* U21, N21 - not defined              - c, p, u */
   inIse,            /* ---, N22 - call abort               - c, p, u */
   inIse,            /* U23, N23 - not defined              - c, p, u */
   inIse,            /* U24, N24 - not defined              - c, p, u */
   inNetEVTINV,      /* U25, N25 - overlap receiving        - c, u */
   inIse,            /* U26, N26 - not defined              - c, p, u */
   inIse,            /* U27, N27 - not defined              - c, p, u */
   inIse,            /* U28, N28 - not defined              - c, p, u */
   inIse,            /* U29, N29 - not defined              - c, p, u */
   inIse,            /* U30, N30 - not defined              - c, p, u */
   inIse},           /* U31, N31 - call independent         - c, p, u */

   /* Suspend/Hold Response */

   {inNetINES00,      /* U00, N00 - null                     - c, p, u */
   inNetEVTINV,      /* U01, N01 - call initiated           - c, p, u */
   inNetEVTINV,      /* U02, N02 - overlap sending          - c, u */
   inNetEVTINV,      /* U03, N03 - outgoing call proceeding - c, p, u */
   inNetEVTINV,      /* U04, N04 - call delivered           - c, u */
   inIse,            /* U05, N05 - not defined              - c, p, u */
   inNetEVTINV,      /* U06, N06 - call present             - c, p, u */
   inNetEVTINV,      /* U07, N07 - call received            - c, p */
   inNetEVTINV,      /* U08, N08 - connect request          - c, p, u */
   inNetEVTINV,      /* U09, N09 - incoming call proceeding - c, p, u */
   inNetEVTINV,      /* U10, N10 - active call              - c, p, u */
   inNetEVTINV,      /* U11, N11 - disconnect request       - c, p */
   inIgnore,         /* U12, N12 - disconnect indication    - c, p */
   inIse,            /* U13, N13 - wait on release complete - c, p, u */
   inIse,            /* U14, N14 - not defined              - c, p, u */
   inNetE34S15,      /* U15, N15 - suspend request          - c */
   inIse,            /* U16, N16 - not defined              - c, p, u */
   inNetEVTINV,      /* U17, N17 - resume request           - c */
   inNetEVTINV,      /* U18, N18 - tone active              - c */
   inNetEVTINV,      /* U19, N19 - release request          - c, p, u */
   inIse,            /* U20, N20 - not defined              - c, p, u */
   inIse,            /* U21, N21 - not defined              - c, p, u */
   inIse,            /* ---, N22 - call abort               - c, p, u */
   inIse,            /* U23, N23 - not defined              - c, p, u */
   inIse,            /* U24, N24 - not defined              - c, p, u */
   inNetEVTINV,      /* U25, N25 - overlap receiving        - c, u */
   inIse,            /* U26, N26 - not defined              - c, p, u */
   inIse,            /* U27, N27 - not defined              - c, p, u */
   inIse,            /* U28, N28 - not defined              - c, p, u */
   inIse,            /* U29, N29 - not defined              - c, p, u */
   inIse,            /* U30, N30 - not defined              - c, p, u */
   inIse},           /* U31, N31 - call independent         - c, p, u */

   /* Resume/Retrieve Request */

   {inNetINES00,      /* U00, N00 - null                     - c, p, u */
   inNetEVTINV,      /* U01, N01 - call initiated           - c, p, u */
   inNetEVTINV,      /* U02, N02 - overlap sending          - c, u */
   inNetEVTINV,      /* U03, N03 - outgoing call proceeding - c, p, u */
   inNetEVTINV,      /* U04, N04 - call delivered           - c, u */
   inIse,            /* U05, N05 - not defined              - c, p, u */
   inNetEVTINV,      /* U06, N06 - call present             - c, p, u */
   inNetEVTINV,      /* U07, N07 - call received            - c, p */
   inNetEVTINV,      /* U08, N08 - connect request          - c, p, u */
   inNetEVTINV,      /* U09, N09 - incoming call proceeding - c, p, u */
   inNetEVTINV,      /* U10, N10 - active call              - c, p, u */
   inNetEVTINV,      /* U11, N11 - disconnect request       - c, p */
   inNetEVTINV,      /* U12, N12 - disconnect indication    - c, p */
   inIse,            /* U13, N13 - wait on release complete - c, p, u */
   inIse,            /* U14, N14 - not defined              - c, p, u */
   inNetEVTINV,      /* U15, N15 - suspend request          - c */
   inIse,            /* U16, N16 - not defined              - c, p, u */
   inNetEVTINV,      /* U17, N17 - resume request           - c */
   inNetEVTINV,      /* U18, N18 - tone active              - c */
   inNetEVTINV,      /* U19, N19 - release request          - c, p, u */
   inIse,            /* U20, N20 - not defined              - c, p, u */
   inIse,            /* U21, N21 - not defined              - c, p, u */
   inIse,            /* ---, N22 - call abort               - c, p, u */
   inIse,            /* U23, N23 - not defined              - c, p, u */
   inIse,            /* U24, N24 - not defined              - c, p, u */
   inNetEVTINV,      /* U25, N25 - overlap receiving        - c, u */
   inIse,            /* U26, N26 - not defined              - c, p, u */
   inIse,            /* U27, N27 - not defined              - c, p, u */
   inIse,            /* U28, N28 - not defined              - c, p, u */
   inIse,            /* U29, N29 - not defined              - c, p, u */
   inIse,            /* U30, N30 - not defined              - c, p, u */
   inIse},           /* U31, N31 - call independent         - c, p, u */

   /* Resume/Retrieve Response */

   {inNetINES00,      /* U00, N00 - null                     - c, p, u */
   inNetEVTINV,      /* U01, N01 - call initiated           - c, p, u */
   inNetEVTINV,      /* U02, N02 - overlap sending          - c, u */
   inNetEVTINV,      /* U03, N03 - outgoing call proceeding - c, p, u */
   inNetEVTINV,      /* U04, N04 - call delivered           - c, u */
   inIse,            /* U05, N05 - not defined              - c, p, u */
   inNetEVTINV,      /* U06, N06 - call present             - c, p, u */
   inNetEVTINV,      /* U07, N07 - call received            - c, p */
   inNetEVTINV,      /* U08, N08 - connect request          - c, p, u */
   inNetEVTINV,      /* U09, N09 - incoming call proceeding - c, p, u */
   inNetEVTINV,      /* U10, N10 - active call              - c, p, u */
   inNetEVTINV,      /* U11, N11 - disconnect request       - c, p */
   inIgnore,         /* U12, N12 - disconnect indication    - c, p */
   inIse,            /* U13, N13 - wait on release complete - c, p, u */
   inIse,            /* U14, N14 - not defined              - c, p, u */
   inNetEVTINV,      /* U15, N15 - suspend request          - c */
   inIse,            /* U16, N16 - not defined              - c, p, u */
   inNetE36S17,      /* U17, N17 - resume request           - c */
   inNetEVTINV,      /* U18, N18 - tone active              - c */
   inNetEVTINV,      /* U19, N19 - release request          - c, p, u */
   inIse,            /* U20, N20 - not defined              - c, p, u */
   inIse,            /* U21, N21 - not defined              - c, p, u */
   inIse,            /* ---, N22 - call abort               - c, p, u */
   inIse,            /* U23, N23 - not defined              - c, p, u */
   inIse,            /* U24, N24 - not defined              - c, p, u */
   inNetEVTINV,      /* U25, N25 - overlap receiving        - c, u */
   inIse,            /* U26, N26 - not defined              - c, p, u */
   inIse,            /* U27, N27 - not defined              - c, p, u */
   inIse,            /* U28, N28 - not defined              - c, p, u */
   inIse,            /* U29, N29 - not defined              - c, p, u */
   inIse,            /* U30, N30 - not defined              - c, p, u */
   inIse},           /* U31, N31 - call independent         - c, p, u */

   /* Flow Control Request */

   {inNetINES00,      /* U00, N00 - null                     - c, p, u */
   inNetEVTINV,      /* U01, N01 - call initiated           - c, p, u */
   inNetEVTINV,      /* U02, N02 - overlap sending          - c, u */
   inNetEVTINV,      /* U03, N03 - outgoing call proceeding - c, p, u */
   inNetE37S10,      /* U04, N04 - call delivered           - c, u */
   inIse,            /* U05, N05 - not defined              - c, p, u */
   inNetEVTINV,      /* U06, N06 - call present             - c, p, u */
   inNetE37S10,      /* U07, N07 - call received            - c, p */
   inNetEVTINV,      /* U08, N08 - connect request          - c, p, u */
   inNetEVTINV,      /* U09, N09 - incoming call proceeding - c, p, u */
   inNetE37S10,      /* U10, N10 - active call              - c, p, u */
   inNetEVTINV,      /* U11, N11 - disconnect request       - c, p */
   inIgnore,         /* U12, N12 - disconnect indication    - c, p */
   inIse,            /* U13, N13 - wait on release complete - c, p, u */
   inIse,            /* U14, N14 - not defined              - c, p, u */
   inNetEVTINV,      /* U15, N15 - suspend request          - c */
   inIse,            /* U16, N16 - not defined              - c, p, u */
   inNetEVTINV,      /* U17, N17 - resume request           - c */
   inNetEVTINV,      /* U18, N18 - tone active              - c */
   inNetEVTINV,      /* U19, N19 - release request          - c, p, u */
   inIse,            /* U20, N20 - not defined              - c, p, u */
   inIse,            /* U21, N21 - not defined              - c, p, u */
   inIse,            /* ---, N22 - call abort               - c, p, u */
   inIse,            /* U23, N23 - not defined              - c, p, u */
   inIse,            /* U24, N24 - not defined              - c, p, u */
   inNetEVTINV,      /* U25, N25 - overlap receiving        - c, u */
   inIse,            /* U26, N26 - not defined              - c, p, u */
   inIse,            /* U27, N27 - not defined              - c, p, u */
   inIse,            /* U28, N28 - not defined              - c, p, u */
   inIse,            /* U29, N29 - not defined              - c, p, u */
   inIse,            /* U30, N30 - not defined              - c, p, u */
   inIse},           /* U31, N31 - call independent         - c, p, u */

   /* Status Request */

   {inNetE38SND,      /* U00, N00 - null                     - c, p, u */
   inNetE38SND,      /* U01, N01 - call initiated           - c, p, u */
   inNetE38SND,      /* U02, N02 - overlap sending          - c, u */
   inNetE38SND,      /* U03, N03 - outgoing call proceeding - c, p, u */
   inNetE38SND,      /* U04, N04 - call delivered           - c, u */
   inIse,            /* U05, N05 - not defined              - c, p, u */
   inNetE38SND,      /* U06, N06 - call present             - c, p, u */
   inNetE38SND,      /* U07, N07 - call received            - c, p */
   inNetE38SND,      /* U08, N08 - connect request          - c, p, u */
   inNetE38SND,      /* U09, N09 - incoming call proceeding - c, p, u */
   inNetE38SND,      /* U10, N10 - active call              - c, p, u */
   inNetE38SND,      /* U11, N11 - disconnect request       - c, p */
   inNetE38SND,      /* U12, N12 - disconnect indication    - c, p */
   inIse,            /* U13, N13 - not defined              - c, p, u */
   inIse,            /* U14, N14 - not defined              - c, p, u */
   inNetE38SND,      /* U15, N15 - suspend request          - c */
   inIse,            /* U16, N16 - not defined              - c, p, u */
   inNetE38SND,      /* U17, N17 - resume request           - c */
   inNetE38SND,      /* U18, N18 - tone active              - c */
   inNetE38SND,      /* U19, N19 - release request          - c, p, u */
   inIse,            /* U20, N20 - not defined              - c, p, u */
   inIse,            /* U21, N21 - not defined              - c, p, u */
   inIse,            /* ---, N22 - call abort               - c, p, u */
   inIse,            /* U23, N23 - not defined              - c, p, u */
   inNetE38SND,      /* U24, N24 - not defined              - c, p, u */
   inNetE38SND,      /* U25, N25 - overlap receiving        - c, u */
   inIse,            /* U26, N26 - not defined              - c, p, u */
   inIse,            /* U27, N27 - not defined              - c, p, u */
   inIse,            /* U28, N28 - not defined              - c, p, u */
   inIse,            /* U29, N29 - not defined              - c, p, u */
   inIse,            /* U30, N30 - not defined              - c, p, u */
   inIse},           /* U31, N31 - call independent         - c, p, u */

   /* Register Request */

   {inNetE39S00,     /* U00, N00 - null                     - c, p, u */
   inIse,            /* U01, N01 - call initiated           - c, p, u */
   inIse,            /* U02, N02 - overlap sending          - c, u */
   inIse,            /* U03, N03 - outgoing call proceeding - c, p, u */
   inIse,            /* U04, N04 - call delivered           - c, u */
   inIse,            /* U05, N05 - not defined              - c, p, u */
   inIse,            /* U06, N06 - call present             - c, p, u */
   inIse,            /* U07, N07 - call received            - c, p */
   inIse,            /* U08, N08 - connect request          - c, p, u */
   inIse,            /* U09, N09 - incoming call proceeding - c, p, u */
   inIse,            /* U10, N10 - active call              - c, p, u */
   inIse,            /* U11, N11 - disconnect request       - c, p */
   inIse,            /* U12, N12 - disconnect indication    - c, p */
   inIse,            /* U13, N13 - not defined              - c, p, u */
   inIse,            /* U14, N14 - not defined              - c, p, u */
   inIse,            /* U15, N15 - suspend request          - c */
   inIse,            /* U16, N16 - not defined              - c, p, u */
   inIse,            /* U17, N17 - resume request           - c */
   inIse,            /* U18, N18 - tone active              - c */
   inIse,            /* U19, N19 - release request          - c, p, u */
   inIse,            /* U20, N20 - not defined              - c, p, u */
   inIse,            /* U21, N21 - not defined              - c, p, u */
   inIse,            /* ---, N22 - call abort               - c, p, u */
   inIse,            /* U23, N23 - not defined              - c, p, u */
   inIse,            /* U24, N24 - not defined              - c, p, u */
   inIse,            /* U25, N25 - overlap receiving        - c, u */
   inIse,            /* U26, N26 - not defined              - c, p, u */
   inIse,            /* U27, N27 - not defined              - c, p, u */
   inIse,            /* U28, N28 - not defined              - c, p, u */
   inIse,            /* U29, N29 - not defined              - c, p, u */
   inIse,            /* U30, N30 - not defined              - c, p, u */
   inIse},           /* U31, N31 - call independent         - c, p, u */

};


/*
 *     support functions
 */


  
/*
*
*       Fun:   inNetChkStateCmp
*
*       Desc:  Check if User and Network calls are in compatible states
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  qn_bdy5.c
*
*/

#ifdef ANSI
PUBLIC S16 inNetChkStateCmp
(
  U8 stateN,
  U8 stateU
)
#else
PUBLIC S16 inNetChkStateCmp(stateN, stateU)
U8 stateN;
U8 stateU;
#endif
{                      
   TRC2(inNetChkStateCmp);


   if (stateU == stateN)
      RETVALUE (ROK);

   switch (stateN)
   {
   case ST_OUTPROC:
      if (stateU == ST_INPROC)
         RETVALUE(ROK);
      break;
   case ST_CALLDEL:
      if (stateU == ST_CALLRECV)
         RETVALUE(ROK);
      break;
   case ST_CALLRECV:
      if (stateU == ST_CALLDEL)
         RETVALUE(ROK);
      break;
   case ST_INPROC:
      if ((stateU == ST_OUTPROC) || (stateU == ST_CALLRECV))
         RETVALUE(ROK);
      break;
   default:
      RETVALUE(RFAILED);
}
 RETVALUE(RFAILED);
} /* end of inNetChkStateCmp */






  
/*
 *
 *       Fun:   inNetE00S06
 *
 *       Desc:  Input: Alerting
 *              State: Call Present
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
         File:  qn_bdy5.c
 *
*/

#ifdef ANSI
PUBLIC S16 inNetE00S06
(
InNtc *ntc,
InCb  *dCb
)
#else
PUBLIC S16 inNetE00S06(ntc, dCb)
InNtc *ntc;
InCb  *dCb;
#endif
{
   InCb *tCb;
   InPCB *pcb;
   AllSdus ev;
   U8 tmrNum;
   S16 ret;
   CctCriEnt inCri;
   U8 string[IN_GEO_CRI_LEN_MAX];


   TRC2(inNetE00S06);

   tCb = ntc->tCallCb;

   /* get physical link control block */
   if ((pcb = *(pcbLstPtr + ntc->dCallCb->suId)) == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      INLOGERROR(ERRCLS_DEBUG, EIN350, (ErrVal) ntc->dCallCb->rSuId,
                 "inNetE00S06() failed, unable to access DLSAP.");
#endif
      RETVALUE(RFAILED);
   }

   /* TELICA - Bugzilla-4321 - Bukucu - 10.25.01 - new imp. of cntrs */
   inUpdSts(&pcb->sts.alertingRx, pcb);

   if (!ntc->pduSp->m.alerting.chanId.eh.pres)
   {
      /* TELICA-MCAO-04/15/02: BUG 9627 modified solutions for ATT5EP only , NT variant need more investigation  */
      switch (pcb->cfg.swtch)
      {
         case SW_NI2:
            /* TELICA-BUG:5126-mqin-11/10/01 - Telcordia ISDN Issue 97,99 */
            cmMemset((U8 *)&(ntc->causeDgn), 0, sizeof(CauseDgn));
            ntc->causeDgn.eh.pres = PRSNT_NODEF;
            INSETTOKVAL(ntc->causeDgn.codeStand3, CSTD_CCITT);
            INSETTOKVAL(ntc->causeDgn.causeVal, CCTMPFAIL);
            INSETTOKVAL(ntc->causeDgn.location, LOC_PUBNETRU);
            if (tCb)
            {
               DiscEvnt discEvnt;
               /* Send Disconnect Indication to Upper */
               ntc->state = ST_RELRQ;
               MFINITSDU(&tCb->mfMsgCtl, ret, (U8)0, (U8)PM_DISCREQ,
                         NULLP, (ElmtHdr *) &discEvnt,
                         (U8) NOTPRSNT, pcb->cfg.swtch, ntc->callType);
               MFINITELMT(&tCb->mfMsgCtl, ret, (ElmtHdr *) &ntc->causeDgn,
                          (ElmtHdr *) &discEvnt.causeDgn[0], &meCauseDgn,
                          (U8) PRSNT_NODEF, pcb->cfg.swtch, ntc->callType);

               InUiIntDiscInd(&tCb->pst, tCb->suId, (InInstId)ntc->suInstId,
                              (InInstId)ntc->tCallRef, &discEvnt);
            }

            /* Send Release to Lower */
            INSETTOKVAL(ntc->causeDgn.causeVal, CCINFOELMSSG);
            INSETTOKVAL(ntc->causeDgn.location, LOC_PUBNETLU);
            ntc->causeDgn.dgnVal.pres = PRSNT_NODEF;
            ntc->causeDgn.dgnVal.len = 1;
            ntc->causeDgn.dgnVal.val[0] = ME_CHANID;
            inGenRelLw(ntc, &ntc->causeDgn, dCb);
            RETVALUE(ROK);
         case SW_NTDMS100P:
            INSETTOKVAL(ntc->causeDgn.causeVal, CCINFOELMSSG);
            inGenStaMsg(ntc, &ntc->causeDgn, dCb);
            RETVALUE(ROK);
         case SW_ATT5EP:
            INSETTOKVAL(ntc->causeDgn.causeVal, CCINFOELMSSG);
            inGenStaMsg(ntc, &ntc->causeDgn, dCb);
            RETVALUE(ROK);
         default:
            inInsrtChnlId(ntc, &ntc->pduSp->m.alerting.chanId, ntc->ctldPcb);
            break;
      }
   }

#ifdef ISDN_PTMPT 
   /*
    * Handle Mpt Check returns RFAILED if no further processing
    * needs to be done after the call of the function.
    * The return value of matrix functions does not matter since
    * it is not checked anywhere in the path.
    */   
   if ((ret = inHandleMptChannelCheck(ntc, dCb, pcb, 
                                      &ntc->pduSp->m.alerting.chanId, 
                                      MI_ALERTING)) != ROK)
   {
      RETVALUE(ROK);
   }

#endif /* ISDN_PTMPT */

   /* 
    * Stop timer 303 or 310 
    */
   for (tmrNum = 0; tmrNum < MAXSIMTIMER; tmrNum++)
      if ((ntc->timers[tmrNum].tmrEvnt == TMR_T303) || 
          (ntc->timers[tmrNum].tmrEvnt == TMR_T310))
         inRmvNtcTq(ntc, tmrNum);
   
   /* 
    * Channel Negotiation 
    */
   if (ntc->pduSp->m.alerting.chanId.intIdentPres.pres)
   {
      if ((ntc->pduSp->m.alerting.chanId.intIdentPres.val == IIP_EXPLICIT)
          && (!pcb->cfg.nfasInt))
         if (ntc->pduSp->m.alerting.chanId.intIdent.val != 
             ntc->ctldPcb->cfg.intId)
         {
            INSETTOKVAL(ntc->causeDgn.causeVal, CCCHANUNACPT);
            inGenRelUpLw(ntc, &ntc->causeDgn, dCb);
            RETVALUE(ROK);
         }
   }
   switch (pcb->cfg.swtch)
   {
      case SW_NTDMS100B:     /* switch - nt dms100 */
      case SW_NTDMS100P:     /* switch - nt dms100 */
         /* validate Channel */
         if ((ret = inValChnlId(ntc,&ntc->pduSp->m.alerting.chanId)) != ROK)
         {
            inGenRelUpLw(ntc, &ntc->causeDgn, dCb);
            RETVALUE(ROK);
         }
         break;

      default:
         if ((ret = inNegChan(ntc, &ntc->pduSp->m.alerting.chanId, 
                              &ntc->causeDgn)) != ROK)
         {
            inGenStaMsg(ntc, &ntc->causeDgn, dCb);
            RETVALUE(ROK);
         }
         if (ntc->causeDgn.causeVal.pres)
         {
            inGenRelUpLw(ntc, &ntc->causeDgn, dCb);
            RETVALUE(ROK);
         }
         ntc->causeDgn.eh.pres = PRSNT_NODEF;
         INSETTOKVAL(ntc->pduSp->m.alerting.chanId.prefExc, PE_EXCLSVE);
         break;
   }

   /* 
    * New State is Call Received (7) 
    */
   ntc->state = ST_CALLRECV;

   if (ntc->updStatus == NTC_NOTCREATED)
   {
      ret = zqRunTimeUpd(ZQ_NTC_CB, CMPFTHA_CREATE_REQ, (PTR)ntc);
      if (ret == ROK)
         ntc->updStatus = NTC_CREATED;
   }
   else
      zqRunTimeUpd(ZQ_NTC_CB, CMPFTHA_UPD_REQ, (PTR)ntc);


   /*
    * Start Timer T301 
    */
   inStartNtcTmr(TMR_T301, ntc, ntc->dCallCb);

   if ((pcb->cfg.swtch == SW_NTDMS100B) || (pcb->cfg.swtch == SW_NTDMS100P))
      if (ntc->callRedir)
      {
         inInitCalRedir(ntc, &ev, pcb->cfg.swtch);

         /* Send Status Indication to Upper */
         InUiIntCnStInd(&tCb->pst, tCb->suId, ntc->suInstId, ntc->tCallRef, 
                        &ev.m.cnStEvnt, MI_NOTIFY,pcb->suId, dCb->ces);

      }


   if (ntc->callType & MF_CKT)
   {
      MFINITSDU(&tCb->mfMsgCtl, ret, (U8) MI_ALERTING, (U8) PM_CNSTREQ, 
                (ElmtHdr *) &ntc->pduSp->m.alerting, (ElmtHdr *) &ev, 
                (U8) PRSNT_NODEF, tCb->sapCfg.swtch, ntc->callType);
     /* Fill CRI Information in CnStEvnt */
     INDBGP(DBGMASK_LI,"inNetE00S06: ****** tCallRef = %ld - dCallRef = %d *******\n",
                  ntc->tCallRef, ntc->dCallRef);
     inFillCriInfo(ntc->dCallRef, &ev.m.cnStEvnt.criList, &inCri, string);

      InUiIntCnStInd(&tCb->pst, tCb->suId, ntc->suInstId, ntc->tCallRef, 
                     &ev.m.cnStEvnt, MI_ALERTING,pcb->suId, dCb->ces);
   }

   zqUpdPeer();

   RETVALUE(ROK);
} /* end of inNetE00S06 */





  
/*
 *
 *       Fun:   inNetE00S07
 *
 *       Desc:  Input: Alerting
 *              State: Call Received
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
         File:  qn_bdy5.c
 *
*/

#ifdef ANSI
PUBLIC S16 inNetE00S07
(
InNtc *ntc,
InCb *dCb 
)
#else
PUBLIC S16 inNetE00S07(ntc, dCb)
InNtc *ntc;
InCb *dCb;
#endif
{
   InCb *tCb = NULL;
   InPCB *pcb = NULL;
   S16 ret = 0;
   AllSdus ev;
   U8 tmrNum = 0;

   TRC2(inNetE00S07);
   UNUSED(ret);
   UNUSED(ev);

   tCb = ntc->tCallCb;
   /* 
    * get physical link control block 
    */
   if ((pcb = *(pcbLstPtr + dCb->suId)) == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      INLOGERROR(ERRCLS_DEBUG, EIN351, (ErrVal) dCb->rSuId,
                 "inNetE00S07() failed, unable to access DLSAP.");
#endif
      RETVALUE(RFAILED);
   }

   inUpdSts(&pcb->sts.alertingRx, pcb);

   switch (pcb->cfg.swtch)
   {
      case SW_ATT4E:
      case SW_ATT5EB:
      case SW_ATT5EP:
         if (ntc->alertCnt)
         {
            /* Send Status Event */
            INSETTOKVAL(ntc->causeDgn.causeVal,CCSERVUNAVAIL);
            inGenStaCfm(ntc, &ntc->causeDgn);
            RETVALUE(ROK);
         }
         else
         {
            ntc->alertCnt = 1;
            /* Stop T_303 or T_310 */
            for (tmrNum = 0; tmrNum < MAXSIMTIMER; tmrNum++)
               if ((ntc->timers[tmrNum].tmrEvnt == TMR_T303) || 
                   (ntc->timers[tmrNum].tmrEvnt == TMR_T310))
                  inRmvNtcTq(ntc, tmrNum);
         
            if (ntc->pduSp->m.alerting.chanId.eh.pres)
            {
               /* validate Channel */
               if ((ret = inValChnlId(ntc, &ntc->pduSp->m.alerting.chanId)) 
                   != ROK)
               {
                  inGenRelUpLw(ntc, &ntc->causeDgn, dCb);
                  RETVALUE(ROK);
               }
            }
            else
               /* insert channel Id  */
               inInsrtChnlId(ntc, &ntc->pduSp->m.alerting.chanId, ntc->ctldPcb);

            /* New State is Call Received (7) */
            ntc->state = ST_CALLRECV;

            /* Start T_301 */
            inStartNtcTmr(TMR_T301, ntc, ntc->dCallCb);
      
            if (ntc->callType & MF_CKT)
            {
               MFINITSDU(&tCb->mfMsgCtl, ret, (U8) MI_ALERTING, (U8)PM_CNSTREQ,
                         (ElmtHdr *) &ntc->pduSp->m.alerting, (ElmtHdr *) &ev, 
                         (U8) PRSNT_NODEF, tCb->sapCfg.swtch, ntc->callType);

               /* Send Status Indication to Upper */
               InUiIntCnStInd(&tCb->pst, tCb->suId, ntc->suInstId, 
                              ntc->tCallRef, &ev.m.cnStEvnt, MI_ALERTING, 
                              pcb->suId,dCb->ces);
            }
         }
         break;
#ifdef ETSI                   /* TELICA-dsolanki-INTMGCDEV */
      case SW_ETSI:          /* switch - etsi */
#ifdef NETSIDE
         /* PTMPT022 */
         /*
          * For point to mpt, if we receive an alerting in this state, we
          * need to issue a internal alerting request and return if the channel
          * is valid. else we need to issue a rel. req for invalid channel.
          * for point to point the processing remains the same.
          */
#ifdef ISDN_PTMPT
         if ((pcb->cfg.intCfg == IN_INTCFG_MULTI) &&
             (pcb->cfg.intType == NETWORK) &&
             (ntc->setupBCast == TRUE))
         {
            if (!ntc->pduSp->m.alerting.chanId.eh.pres)
               inInsrtChnlId(ntc, &ntc->pduSp->m.alerting.chanId, pcb, chan);

            if ((ret = inHandleMptChannelCheck(ntc, dCb, pcb,
                                               &ntc->pduSp->m.alerting.chanId,
                                               MI_ALERTING)) != ROK)
            {
               RETVALUE(ROK);
            }
         }
#endif /* ISDN_PTMPT */
#endif /* NETSIDE */ 

#endif /* ETSI */             /* TELICA-dsolanki-INTMGCDEV */


      case SW_CCITT:
         inNetMSGINV(ntc, dCb);
         break;

      case SW_NTDMS100B:     /* switch - nt dms100 */
         break;
      case SW_NTDMS100P:     /* switch - nt dms100 */
         if (ntc->callType & MF_CKT)
         {
            MFINITSDU(&tCb->mfMsgCtl, ret, (U8) MI_ALERTING, (U8) PM_CNSTREQ, 
                      (ElmtHdr *) &ntc->pduSp->m.alerting, (ElmtHdr *) &ev, 
                      (U8) PRSNT_NODEF, tCb->sapCfg.swtch, ntc->callType);
            /* Send Status Indication to Upper */

            InUiIntCnStInd(&tCb->pst, tCb->suId, ntc->suInstId, 
                           ntc->tCallRef, &ev.m.cnStEvnt, MI_ALERTING, 
                           pcb->suId,
                           dCb->ces);
         }
         break;

      case SW_NTDMS250:      /* switch - nt dms250 */
         if (ntc->callType & MF_CKT)
         {
            MFINITSDU(&tCb->mfMsgCtl, ret, (U8) MI_ALERTING, (U8) PM_CNSTREQ, 
                      (ElmtHdr *) &ntc->pduSp->m.alerting, (ElmtHdr *) &ev, 
                      (U8) PRSNT_NODEF, tCb->sapCfg.swtch, ntc->callType);
            /* Send Status Indication to Upper */

            InUiIntCnStInd(&tCb->pst, tCb->suId, ntc->suInstId, 
                           ntc->tCallRef, &ev.m.cnStEvnt, MI_ALERTING, 
                           pcb->suId,dCb->ces);
         }
         break;

      default:
#if (ERRCLASS & ERRCLS_DEBUG)
         INLOGERROR(ERRCLS_DEBUG, EIN352, (ErrVal) pcb->cfg.swtch,
                    "inNetE00S07() failed, invalid message.");
#endif
         inNetMSGINV(ntc, dCb);      
         break;
   }
   RETVALUE(ROK);
} /* end of inNetE00S07 */



  
/*
 *
 *       Fun:   inNetE00S09
 *
 *       Desc:  Input: Alerting 
 *              State: Incoming call proceeding
 *                     
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
         File:  qn_bdy5.c
 *
*/

#ifdef ANSI
PUBLIC S16 inNetE00S09
(
InNtc *ntc,
InCb *dCb 
)
#else
PUBLIC S16 inNetE00S09(ntc, dCb)
InNtc *ntc;
InCb *dCb;
#endif
{
   InCb *tCb;
   InPCB *pcb;
   AllSdus ev;
   U8 tmrNum;
   S16 ret;

   TRC2(inNetE00S09);

   tCb = ntc->tCallCb;

   /* get physical link control block */
   if ((pcb = *(pcbLstPtr + dCb->suId)) == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      INLOGERROR(ERRCLS_DEBUG, EIN353, (ErrVal) dCb->rSuId,
                 "inNetE00S09() failed, unable to access DLSAP.");
#endif
      RETVALUE(RFAILED);
   }

   inUpdSts(&pcb->sts.alertingRx, pcb);


#ifdef ISDN_PTMPT
   if (!ntc->pduSp->m.alerting.chanId.eh.pres)
      inInsrtChnlId(ntc, &ntc->pduSp->m.alerting.chanId, ntc->ctldPcb);
   if ((ret = inHandleMptChannelCheck(ntc, dCb, pcb, 
                                      &ntc->pduSp->m.alerting.chanId,
                                      MI_ALERTING)) != ROK)
      RETVALUE(ROK);
#endif /* ISDN_PTMPT */      

   /* Stop T310 */
   for (tmrNum = 0; tmrNum < MAXSIMTIMER; tmrNum++)
      if ((ntc->timers[tmrNum].tmrEvnt == TMR_T310)||
          (ntc->timers[tmrNum].tmrEvnt == TMR_TPROG)||
          (ntc->timers[tmrNum].tmrEvnt == TMR_T301))
         inRmvNtcTq(ntc, tmrNum);

   /* Start T301 */
   inStartNtcTmr(TMR_T301, ntc, ntc->dCallCb);

   /* Marker PT_CHG#18 */
   /* Change state to Call Received (7) */
   ntc->state = ST_CALLRECV;

   /* TELICA - Bugzilla-5126 - mqin - 8/14/01 - cause code Issue 7: send
                                              STATUS message */
   if (ntc->pduSp->m.alerting.chanId.eh.pres)
   {
      if (pcb->cfg.swtch == SW_NI2)
      {
         INSETTOKVAL(ntc->causeDgn.causeVal, CCINVINFOEL);
         INSETTOKVAL(ntc->causeDgn.location, LOC_PUBNETLU);
         ntc->causeDgn.dgnVal.pres = PRSNT_NODEF;
         ntc->causeDgn.dgnVal.len = 1;
         ntc->causeDgn.dgnVal.val[0] = ME_CHANID;

         inGenStaMsg(ntc, &ntc->causeDgn, dCb);
      }
   }

   if (ntc->updStatus == NTC_NOTCREATED)
   {
      ret = zqRunTimeUpd(ZQ_NTC_CB, CMPFTHA_CREATE_REQ, (PTR)ntc);
      if (ret == ROK)
         ntc->updStatus = NTC_CREATED;
   }
   else
      zqRunTimeUpd(ZQ_NTC_CB, CMPFTHA_UPD_REQ, (PTR)ntc);

   if (ntc->callType & MF_CKT)
   {
      MFINITSDU(&tCb->mfMsgCtl, ret, (U8) MI_ALERTING, (U8) PM_CNSTREQ, 
                (ElmtHdr *) &ntc->pduSp->m.alerting, (ElmtHdr *) &ev, 
                (U8) PRSNT_NODEF, tCb->sapCfg.swtch, ntc->callType);
      /* Send Alert Indication to Upper */
      InUiIntCnStInd(&tCb->pst, tCb->suId, ntc->suInstId, ntc->tCallRef, 
                     &ev.m.cnStEvnt, MI_ALERTING, pcb->suId, dCb->ces);
   }

   zqUpdPeer();

   RETVALUE(ROK);
} /* end of inNetE00S09 */

  
/*
 *
 *       Fun:   inNetE00S25
 *
 *       Desc:  Input: Alerting 
 *              State: overlap receiving
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
         File:  qn_bdy5.c
 *
*/

#ifdef ANSI
PUBLIC S16 inNetE00S25
(
InNtc *ntc,
InCb *dCb
)
#else
PUBLIC S16 inNetE00S25(ntc, dCb)
InNtc *ntc;
InCb *dCb;
#endif
{
   InCb *tCb = NULL;
   InPCB *pcb = NULL;
   AllSdus ev;
   U8 tmrNum = 0;
   S16 ret = 0;

   TRC2(inNetE00S25);
   UNUSED(ret);
   UNUSED(tmrNum);
   UNUSED(ev);
   UNUSED(pcb);
   UNUSED(tCb);
   UNUSED(ntc);
   UNUSED(dCb);

#ifdef ISDN_OVERLAP
   tCb = ntc->tCallCb;

   /* get physical link control block */
   if ((pcb = *(pcbLstPtr + dCb->suId)) == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      INLOGERROR(ERRCLS_DEBUG, EIN354, (ErrVal) dCb->rSuId,
                 "inNetE00S25() failed, unable to access DLSAP.");
#endif
      RETVALUE(RFAILED);
   }

   inUpdSts(&pcb->sts.alertingRx, pcb);

   switch (pcb->cfg.swtch)
   {
#ifdef ETSI                   /* TELICA-dsolanki-INTMGCDEV */
      case SW_ETSI:          /* switch - etsi */
#ifdef NETSIDE
#ifdef ISDN_PTMPT
         if ((pcb->cfg.intCfg == IN_INTCFG_MULTI) &&
             (pcb->cfg.intType == NETWORK) &&
             (ntc->setupBCast == TRUE))
         {
            if (!ntc->pduSp->m.alerting.chanId.eh.pres)
               inInsrtChnlId(ntc, &ntc->pduSp->m.alerting.chanId,
                             pcb, chan);
            if ((ret = inHandleMptChannelCheck(ntc, dCb, pcb,
                                               &ntc->pduSp->m.alerting.chanId,
                                               MI_ALERTING)) != ROK)
            {
               RETVALUE(ROK);
            }
         }
#endif /* ISDN_PTMPT */
#endif /* NETSIDE */
#endif                       /* TELICA-dsolanki-INTMGCDEV */

      case SW_CCITT:
         /* Stop T304 */
         for (tmrNum = 0; tmrNum < MAXSIMTIMER; tmrNum++)
            if (ntc->timers[tmrNum].tmrEvnt == TMR_T304) 
               inRmvNtcTq(ntc, tmrNum);
   
         /* validate Channel */
         if ((ret = inValChnlId(ntc, &ntc->pduSp->m.alerting.chanId)) != ROK)
         {
            inGenRelUpLw(ntc, &ntc->causeDgn, dCb);
            RETVALUE(ROK);
         }

         MFINITSDU(&tCb->mfMsgCtl, ret, (U8) MI_ALERTING, (U8)PM_CNSTREQ, 
                   (ElmtHdr *) &ntc->pduSp->m.alerting, (ElmtHdr *) &ev, 
                   (U8) PRSNT_NODEF, tCb->sapCfg.swtch, ntc->callType);
   
         /* Start T301 */
         inStartNtcTmr(TMR_T301, ntc, ntc->dCallCb);

         /* Change state to Call Received (7) */
         ntc->state = ST_CALLRECV;

         if (ntc->updStatus == NTC_NOTCREATED)
         {
            ret = zqRunTimeUpd(ZQ_NTC_CB, CMPFTHA_CREATE_REQ, (PTR)ntc);
            if (ret == ROK)
               ntc->updStatus = NTC_CREATED;
         }
         else
            zqRunTimeUpd(ZQ_NTC_CB, CMPFTHA_UPD_REQ, (PTR)ntc);

         /* Send Status Indication to Upper */
         InUiIntCnStInd(&tCb->pst, tCb->suId, ntc->suInstId, ntc->tCallRef, 
                        &ev.m.cnStEvnt, MI_ALERTING, pcb->suId, dCb->ces);

         break;

      default:
#if (ERRCLASS & ERRCLS_DEBUG)
         INLOGERROR(ERRCLS_DEBUG, EIN355,(ErrVal)pcb->cfg.swtch,
                    "inNetE00S25() failed, invalid message.");
#endif
         inNetMSGINV(ntc, dCb);
         break;
   }
   zqUpdPeer();
#endif
   RETVALUE(ROK);
} /* end of inNetE00S25 */





  
/*
 *
 *       Fun:   inNetE01S06
 *
 *       Desc:  Input: Call Proceeding
 *              State: call present
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None   
 *
         File:  qn_bdy5.c
 *
*/

#ifdef ANSI
PUBLIC S16 inNetE01S06
(
InNtc *ntc,
InCb  *dCb
)
#else
PUBLIC S16 inNetE01S06(ntc, dCb)
InNtc *ntc;
InCb  *dCb;
#endif
{
   U8 tmrNum;
   InCb *tCb;
   InPCB *pcb;
   AllSdus ev;
   S16 ret;
   CctCriEnt inCri;
   U8 string[IN_GEO_CRI_LEN_MAX];

   TRC2(inNetE01S06);

   tCb = ntc->tCallCb;

   /* get physical link control block */
   if ((pcb = *(pcbLstPtr + ntc->dCallCb->suId)) == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      INLOGERROR(ERRCLS_DEBUG, EIN356, (ErrVal) ntc->dCallCb->rSuId,
                 "inNetE01S06() failed, unable to access DLSAP.");
#endif
      RETVALUE(RFAILED);
   }

   inUpdSts(&pcb->sts.callProcRx, pcb);

   if (!ntc->pduSp->m.callProc.chanId.eh.pres)
   {
#if 1 /* bug 55464 */
      switch (pcb->cfg.swtch)
      {
      case SW_ETSI: /* user has accepted the chanId sent in SETUP */
         inInsrtChnlId(ntc, &ntc->pduSp->m.callProc.chanId, ntc->ctldPcb);
         break;
      default:
#endif
      ntc->causeDgn.eh.pres = PRSNT_NODEF;
      INSETTOKVAL(ntc->causeDgn.codeStand3, CSTD_CCITT);
      INSETTOKVAL(ntc->causeDgn.location, LOC_PUBNETLU);
      INSETTOKVAL(ntc->causeDgn.causeVal,
                  dCb->mfMsgCtl.ee[0].mfCauseDgn.causeVal.val);

      ntc->causeDgnUp = ntc->causeDgn;
      INSETTOKVAL(ntc->causeDgnUp.causeVal, CCTMPFAIL);
      INSETTOKVAL(ntc->causeDgnUp.location, LOC_PUBNETRU);

      inGenRelUpLw(ntc, &ntc->causeDgn, dCb);

      RETVALUE(ROK);
#if 1 /* bug 55464 */
      }
#endif
   }

#ifdef ISDN_PTMPT
   if ((ret = inHandleMptChannelCheck(ntc, dCb, pcb,
                                      &ntc->pduSp->m.callProc.chanId,
                                      MI_CALLPROC)) != ROK)
   {
      RETVALUE(ROK);
   }
#endif /* ISDN_PTMPT */

   /* 
    * Stop Timer T303 
    */

   STOP_NTC_TIMER(TMR_T303);

   if (ntc->pduSp->m.callProc.chanId.eh.pres)
   {
      if (ntc->pduSp->m.callProc.chanId.intIdentPres.pres)
      {
         if ((ntc->pduSp->m.callProc.chanId.intIdentPres.val == IIP_EXPLICIT)
             && (!pcb->cfg.nfasInt))
            if (ntc->pduSp->m.callProc.chanId.intIdent.val != 
                ntc->ctldPcb->cfg.intId)
            {
               INSETTOKVAL(ntc->causeDgn.causeVal, CCCHANUNACPT);
               ntc->state = ST_RELRQ;
               inGenRelUpLw(ntc, &ntc->causeDgn, dCb);
               RETVALUE(ROK);
            }
      }

      switch (pcb->cfg.swtch)
      {
         case SW_NTDMS100B:     /* switch - nt dms100 */
         case SW_NTDMS100P:     /* switch - nt dms100 */
            /* validate Channel */
            if ((ret = inValChnlId(ntc,&ntc->pduSp->m.callProc.chanId)) != ROK)
            {
               inGenRelUpLw(ntc, &ntc->causeDgn, dCb);
               RETVALUE(ROK);
            }
            break;
         default:
            if ((ret = inNegChan(ntc, &ntc->pduSp->m.callProc.chanId, 
                                 &ntc->causeDgn)) != ROK)
            {
               inGenStaMsg(ntc, &ntc->causeDgn, dCb);
               RETVALUE(ROK);
            }
            if (ntc->causeDgn.causeVal.pres)
            {
               inGenRelUpLw(ntc, &ntc->causeDgn, dCb);
               RETVALUE(ROK);
            }
            ntc->causeDgn.eh.pres = PRSNT_NODEF;

            INSETTOKVAL(ntc->pduSp->m.callProc.chanId.prefExc, PE_EXCLSVE);

            break;
      }
   }  /* chanId->pres.. */


   if ((pcb->cfg.swtch == SW_NTDMS100B) 
       || (pcb->cfg.swtch == SW_NTDMS100P))
   {
      if (ntc->callRedir)
      {
         inInitCalRedir(ntc, &ev, pcb->cfg.swtch);
         /* Send Status Indication to Upper */

         InUiIntCnStInd(&tCb->pst, tCb->suId, ntc->suInstId, ntc->tCallRef, 
                        &ev.m.cnStEvnt, MI_NOTIFY, pcb->suId, dCb->ces);
      }
   }

   MFINITSDU(&tCb->mfMsgCtl, ret, (U8) MI_CALLPROC, (U8) PM_CNSTREQ, 
             (ElmtHdr *) &ntc->pduSp->m.callProc, (ElmtHdr *)&ev, 
             (U8) PRSNT_NODEF, tCb->sapCfg.swtch, ntc->callType);

   /* 
    * New State is incoming call proceeding (9) 
    */
   ntc->state = ST_INPROC;


   /* 
    * Start Timer T310 
    */
   inStartNtcTmr(TMR_T310, ntc, ntc->dCallCb);

   /* Fill CRI Information in CnStEvnt */
   INDBGP(DBGMASK_LI,"inNetE01S06:****** tCallRef = %ld - dCallRef = %d *******\n",
                      ntc->tCallRef, ntc->dCallRef);
   inFillCriInfo(ntc->dCallRef, &ev.m.cnStEvnt.criList, &inCri, string);

   /* Send Connect Status Indication to Upper */
   InUiIntCnStInd(&tCb->pst, tCb->suId, ntc->suInstId, ntc->tCallRef, 
                  &ev.m.cnStEvnt, MI_CALLPROC, pcb->suId, dCb->ces);

   RETVALUE(ROK);
} /* end of inNetE01S06 */





  
/*
 *
 *       Fun:   inNetE01S25
 *
 *       Desc:  Input: Call Proceeding
 *              State: overlap receiving
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
         File:  qn_bdy5.c
 *
*/

#ifdef ANSI
PUBLIC S16 inNetE01S25
(
InNtc *ntc,
InCb *dCb
)
#else
PUBLIC S16 inNetE01S25(ntc, dCb)
InNtc *ntc;
InCb *dCb;
#endif
{
   InCb *tCb = NULL;
   InPCB *pcb = NULL;
   AllSdus ev;
   U8 tmrNum = 0;
   S16 ret = 0;

   TRC2(inNetE01S25);

   UNUSED(ret);
   UNUSED(tmrNum);
   UNUSED(ev);
   UNUSED(pcb);
   UNUSED(tCb);
   UNUSED(ntc);
   UNUSED(dCb);

#ifdef ISDN_OVERLAP
   tCb = ntc->tCallCb;

   /* get physical link control block */
   if ((pcb = *(pcbLstPtr + dCb->suId)) == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      INLOGERROR(ERRCLS_DEBUG, EIN357, (ErrVal) dCb->rSuId,
                 "inNetE01S25() failed, unable to access DLSAP.");
#endif
      RETVALUE(RFAILED);
   }

   inUpdSts(&pcb->sts.callProcRx, pcb);

   switch (pcb->cfg.swtch)
   {
#ifdef ETSI                  /* TELICA-dsolanki-INTMGCDEV */
      case SW_ETSI:          /* switch - etsi */
#ifdef NETSIDE
#ifdef ISDN_PTMPT
         if ((pcb->cfg.intCfg == IN_INTCFG_MULTI) &&
             (pcb->cfg.intType == NETWORK) &&
             (ntc->setupBCast == TRUE))
         {
            if (!ntc->pduSp->m.callProc.chanId.eh.pres)
               inInsrtChnlId(ntc, &ntc->pduSp->m.callProc.chanId,
                             pcb, chan);
            if ((ret = inHandleMptChannelCheck(ntc, dCb, pcb,
                                               &ntc->pduSp->m.callProc.chanId,
                                               MI_CALLPROC)) != ROK)
            {
               RETVALUE(ROK);
            }
         }
#endif /* ISDN_PTMPT */
#endif /* NETSIDE */

#endif                        /* TELICA-dsolanki-INTMGCDEV */

      case SW_CCITT:
         /* Stop T304 */
         for (tmrNum = 0; tmrNum < MAXSIMTIMER; tmrNum++)
            if (ntc->timers[tmrNum].tmrEvnt == TMR_T304) 
               inRmvNtcTq(ntc, tmrNum);
   
         /* validate Channel */
         if ((ret = inValChnlId(ntc, &ntc->pduSp->m.callProc.chanId)) != ROK)
         {
            /* Marker PT_CHG#35 */
            inGenRelUpLw(ntc, &ntc->causeDgn, dCb);
            RETVALUE(ROK);
         }
   
         MFINITSDU(&tCb->mfMsgCtl, ret, (U8) MI_CALLPROC, (U8) PM_CNSTREQ, 
                   (ElmtHdr *) &ntc->pduSp->m.callProc, (ElmtHdr *) &ev, 
                   (U8) PRSNT_NODEF, tCb->sapCfg.swtch, ntc->callType);
   
         /* Start T310 */
         inStartNtcTmr(TMR_T310, ntc, ntc->dCallCb);
   
         /* Change state to Incoming Call Proceeding (9) */
         ntc->state = ST_INPROC;

         /* Marker PT_CHG#38 */

         /* Send Proceeding Indication to Upper */
         InUiIntCnStInd(&tCb->pst, tCb->suId, ntc->suInstId, ntc->tCallRef, 
                        &ev.m.cnStEvnt, MI_CALLPROC, pcb->suId, dCb->ces);
         break;

      default:
#if (ERRCLASS & ERRCLS_DEBUG)
         INLOGERROR(ERRCLS_DEBUG, EIN358, (ErrVal) pcb->cfg.swtch,
                    "inNetE01S25() failed, message not valid.");
#endif
         inNetMSGINV(ntc, dCb);
         break;
   }
#endif
   RETVALUE(ROK);
} /* end of inNetE01S25 */

  
/*
 *
 *       Fun:   inNetE02S06
 *
 *       Desc:  Input: Connect
 *              State: call present
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
         File:  qn_bdy5.c
 *
*/

#ifdef ANSI
PUBLIC S16 inNetE02S06
(
InNtc *ntc,
InCb *dCb 
)
#else
PUBLIC S16 inNetE02S06(ntc, dCb)
InNtc *ntc;
InCb *dCb;
#endif
{
   InCb *tCb;
   InPCB *pcb;
   PduHdr pduHdr;
   AllSdus ev;
   U8 tmrNum;
   S16 ret;
   CctCriEnt inCri;
   U8 string[IN_GEO_CRI_LEN_MAX];

   TRC2(inNetE02S06);

   tCb = ntc->tCallCb;

   /* get physical link control block */
   if ((pcb = *(pcbLstPtr + dCb->suId)) == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      INLOGERROR(ERRCLS_DEBUG, EIN359, (ErrVal) dCb->rSuId,
                 "inNetE02S06() failed, unable to access DLSAP.");
#endif
      RETVALUE(RFAILED);
   }

   inUpdSts(&pcb->sts.connRx, pcb);

   if (!ntc->pduSp->m.conn.chanId.eh.pres)
   {

     /* TELICA-MCAO-04/15/02: BUG 9627 modified solutions for ATT5EP only , NT variant need more investigation  */
     switch (pcb->cfg.swtch)
      {
       case SW_NTDMS100P:
         INSETTOKVAL(ntc->causeDgn.causeVal, CCINFOELMSSG);
         inGenStaMsg(ntc, &ntc->causeDgn, dCb);
         RETVALUE(ROK);
       case SW_NI2:
        /* TELICA-BUG:5126-mqin-11/10/01 - Telcordia ISDN Issue 98,100 */
        cmMemset((U8 *)&(ntc->causeDgn), 0, sizeof(CauseDgn));
        ntc->causeDgn.eh.pres = PRSNT_NODEF;
        INSETTOKVAL(ntc->causeDgn.codeStand3, CSTD_CCITT); //bug 18022:taraveti
        INSETTOKVAL(ntc->causeDgn.causeVal, CCTMPFAIL);
        INSETTOKVAL(ntc->causeDgn.location, LOC_PUBNETRU);
        if (tCb)
        {
           DiscEvnt discEvnt;
           /* Send Disconnect Indication to Upper */
           ntc->state = ST_RELRQ;
           MFINITSDU(&tCb->mfMsgCtl, ret, (U8)0, (U8)PM_DISCREQ,
                     NULLP, (ElmtHdr *) &discEvnt,
                     (U8) NOTPRSNT, pcb->cfg.swtch, ntc->callType);
           MFINITELMT(&tCb->mfMsgCtl, ret, (ElmtHdr *) &ntc->causeDgn,
                     (ElmtHdr *) &discEvnt.causeDgn[0], &meCauseDgn,
                     (U8) PRSNT_NODEF, pcb->cfg.swtch, ntc->callType);

           InUiIntDiscInd(&tCb->pst, tCb->suId, (InInstId)ntc->suInstId,
                        (InInstId)ntc->tCallRef, &discEvnt);
        }

           /* Send Release to Lower */
        INSETTOKVAL(ntc->causeDgn.causeVal, CCINFOELMSSG);
        INSETTOKVAL(ntc->causeDgn.location, LOC_PUBNETLU);
        ntc->causeDgn.dgnVal.pres = PRSNT_NODEF;
        ntc->causeDgn.dgnVal.len = 1;
        ntc->causeDgn.dgnVal.val[0] = ME_CHANID;
        inGenRelLw(ntc, &ntc->causeDgn, dCb);
        RETVALUE(ROK);
       case SW_ATT5EP:
          INSETTOKVAL(ntc->causeDgn.causeVal, CCINFOELMSSG);
          inGenStaMsg(ntc, &ntc->causeDgn, dCb);
          RETVALUE(ROK);
       default:
            inInsrtChnlId(ntc, &ntc->pduSp->m.conn.chanId, ntc->ctldPcb);
          break;
       }
   }

#ifdef ISDN_PTMPT
   if ((ret = inHandleMptChannelCheck(ntc, dCb, pcb, 
                                      &ntc->pduSp->m.conn.chanId,
                                      MI_CONNECT)) != ROK)
   {
      RETVALUE(ROK);
   }
#endif /* ISDN_PTMPT */
   
   /* 
    * Stop T_301 or T_303 or T_310 
    */
   for (tmrNum = 0; tmrNum < MAXSIMTIMER; tmrNum++)
      if ((ntc->timers[tmrNum].tmrEvnt == TMR_T301) || 
          (ntc->timers[tmrNum].tmrEvnt == TMR_T303) ||
          (ntc->timers[tmrNum].tmrEvnt == TMR_T310))
         inRmvNtcTq(ntc, tmrNum);

   /* Channel Negotiation */
   if (ntc->pduSp->m.conn.chanId.eh.pres)
   {
      if (ntc->pduSp->m.conn.chanId.intIdentPres.pres)
         if ((ntc->pduSp->m.conn.chanId.intIdentPres.val == IIP_EXPLICIT)
             && (!pcb->cfg.nfasInt))
            if (ntc->pduSp->m.conn.chanId.intIdent.val != 
                ntc->ctldPcb->cfg.intId)
            {
               
               INSETTOKVAL(ntc->causeDgn.causeVal,CCCHANUNACPT);
               
               inGenRelUpLw(ntc, &ntc->causeDgn, dCb);
               RETVALUE(ROK);
            }
      switch (pcb->cfg.swtch)
      {
         case SW_NTDMS100B:     /* switch - nt dms100 */
         case SW_NTDMS100P:     /* switch - nt dms100 */
            /* validate Channel */
            if ((ret = inValChnlId(ntc, &ntc->pduSp->m.conn.chanId)) != ROK)
            {
               inGenRelUpLw(ntc, &ntc->causeDgn, dCb);
               RETVALUE(ROK);
            }
            break;
         default:
            if ((ret = inNegChan(ntc, &ntc->pduSp->m.conn.chanId, 
                                 &ntc->causeDgn)) != ROK)
            {
               inGenStaMsg(ntc, &ntc->causeDgn, dCb);
               RETVALUE(ROK);
            }
            if (ntc->causeDgn.causeVal.pres)
            {
               inGenRelUpLw(ntc, &ntc->causeDgn, dCb);
               RETVALUE(ROK);
            }
            ntc->causeDgn.eh.pres = PRSNT_NODEF;
            INSETTOKVAL(ntc->pduSp->m.conn.chanId.prefExc, PE_EXCLSVE);
            break;
      }
   }

   if ((pcb->cfg.swtch == SW_NTDMS100B) || (pcb->cfg.swtch == SW_NTDMS100P))
      if (ntc->callRedir)
      {
         inInitCalRedir(ntc, &ev, pcb->cfg.swtch);

         /* Send Status Indication to Upper */
         InUiIntCnStInd(&tCb->pst, tCb->suId, ntc->suInstId, ntc->tCallRef, 
                        &ev.m.cnStEvnt, MI_NOTIFY, pcb->suId, dCb->ces);
      } 

   MFINITSDU(&tCb->mfMsgCtl, ret, (U8) MI_CONNECT, (U8) PM_CNSTREQ, 
             (ElmtHdr *) &ntc->pduSp->m.conn, (ElmtHdr *) &ev, (U8) PRSNT_NODEF, 
             tCb->sapCfg.swtch, ntc->callType);

   /* 
    * Change State to active (10) 
    */

   /*
    * We should be changing state to CONNRQ and not to ACTVE.
    * Currently do this only for etsi, networkside multipoint config.
    */

   ntc->state = ST_ACTVE;

#ifdef ISDN_PTMPT
  if ((pcb->cfg.intCfg == INTCFG_MULTI)
           && (ntc->setupBCast == TRUE))
      ntc->state = ST_CONNRQ;

#endif /* ISDN_PTMPT */

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

   /* Send Connect Acknowledge message before passing 
    * InUiIntConCfm to layer 4
    */

   
   /*
    * Issue connect acknowledge only if we are not configured for
    * ETSI point to multipoint.
    */
   
#ifdef ISDN_PTMPT 
   if ((pcb->cfg.intCfg != INTCFG_MULTI)
           || (ntc->setupBCast != TRUE))
   {
#endif /* ISDN_PTMPT */
      if (pcb->cfg.ackOpt)
      {
         inUpdSts(&pcb->sts.connAckTx, pcb);

         /* prepare Pdu header */
         inInitPduHdr((U8) PD_Q931, ntc->origin, pcb->cfg.callRefLen, 
                      ntc->dCallRef, (U8) M_CONNACK, &pduHdr);
           
         MFINITPDU(&ntc->dCallCb->mfMsgCtl, ret, (U8) 0, 
                   (U8) MI_CONNACK, NULLP, 
                   (ElmtHdr *) ntc->pduSp, (U8) PRSNT_DEF, pcb->cfg.swtch, 
                   ntc->callType);

         if (pcb->cfg.swtch == SW_NTDMS100B)
         {
            ntc->pduSp->m.connAck.signal.eh.pres = PRSNT_NODEF;
            ntc->pduSp->m.connAck.signal.signal.pres = PRSNT_NODEF;
            ntc->pduSp->m.connAck.signal.signal.val = SIG_ALERTOFF;
         }
         inGenPdu(dCb, &pduHdr, ntc->pduSp, pcb->cfg.swtch, 
                  ntc->callType);
      }
#ifdef ISDN_PTMPT 
   }
#endif /* ISDN_PTMPT */

   /* Fill CRI Information in CnStEvnt */
   INDBGP(DBGMASK_LI,"inNetE02S06: ****** tCallRef = %ld - dCallRef = %d *******\n",
                       ntc->tCallRef, ntc->dCallRef);

   inFillCriInfo(ntc->dCallRef, &ev.m.cnStEvnt.criList, &inCri, string);


   InUiIntConCfm(&tCb->pst, tCb->suId, ntc->suInstId, ntc->tCallRef, 
                 &ev.m.cnStEvnt, pcb->suId, dCb->ces);

   if (inInit.acnt)
   {
      SGetSysTime(&ntc->calDura);
   }

   zqUpdPeer();

   RETVALUE(ROK);
} /* end of inNetE02S06 */






  
/*
 *
 *       Fun:   inNetE02S07
 *
 *       Desc:  Input: Connect
 *              State: Call Received
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
         File:  qn_bdy5.c
 *
*/

#ifdef ANSI
PUBLIC S16 inNetE02S07
(
InNtc *ntc,
InCb *dCb 
)
#else
PUBLIC S16 inNetE02S07(ntc, dCb)
InNtc *ntc;
InCb *dCb;
#endif
{
   InCb *tCb;
   InPCB *pcb;
   PduHdr pduHdr;
   AllSdus ev;
   U8 tmrNum;
   S16 ret;

   TRC2(inNetE02S07);

   tCb = ntc->tCallCb;

   /* get physical link control block */
   if ((pcb = *(pcbLstPtr + dCb->suId)) == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      INLOGERROR(ERRCLS_DEBUG, EIN360, (ErrVal) dCb->rSuId,
                 "inNetE02S07() failed, unable to access DLSAP.");
#endif
      RETVALUE(RFAILED);
   }

   inUpdSts(&pcb->sts.connRx, pcb);

#ifdef ISDN_PTMPT   
   /*
    * Insert channel if required.
    */
   if (!ntc->pduSp->m.conn.chanId.eh.pres)
      inInsrtChnlId(ntc, &ntc->pduSp->m.conn.chanId, ntc->ctldPcb);
   

   if ((ret = inHandleMptChannelCheck(ntc, dCb, pcb, 
                                      &ntc->pduSp->m.conn.chanId,
                                      MI_CONNECT)) != ROK)
   {
      RETVALUE(RFAILED);
   }
#endif /* ISDN_PTMPT */


   /* 
    * Stop Timer T301 
    */
   for (tmrNum = 0; tmrNum < MAXSIMTIMER; tmrNum++)
      if (ntc->timers[tmrNum].tmrEvnt == TMR_T301) 
         inRmvNtcTq(ntc, tmrNum);

   MFINITSDU(&tCb->mfMsgCtl, ret, (U8) MI_CONNECT, (U8) PM_CNSTREQ, 
             (ElmtHdr *) &ntc->pduSp->m.conn, (ElmtHdr *) &ev, 
             (U8) PRSNT_NODEF,tCb->sapCfg.swtch, ntc->callType);


   /* TELICA-BUG:5126-mqin-11/12/01 - cause code Issue 102 */
   if (ntc->pduSp->m.conn.chanId.eh.pres)
   {
      if (pcb->cfg.swtch == SW_NI2)
      {
         /* temporarily change call state to ST_CONNRQ(8) for STATUS */
         ntc->state = ST_CONNRQ;

         INSETTOKVAL(ntc->causeDgn.causeVal, CCINVINFOEL);
         INSETTOKVAL(ntc->causeDgn.location, LOC_PUBNETLU);
         ntc->causeDgn.dgnVal.pres = PRSNT_NODEF;
         ntc->causeDgn.dgnVal.len = 1;
         ntc->causeDgn.dgnVal.val[0] = ME_CHANID;
         inGenStaMsg(ntc, &ntc->causeDgn, dCb);
      }
   }

   /*
    * Change state to Active (10)
    */

   ntc->state = ST_ACTVE;

#ifdef ISDN_PTMPT
   if ((pcb->cfg.intCfg == INTCFG_MULTI)
           && (ntc->setupBCast == TRUE))
      ntc->state = ST_CONNRQ;

#endif /* ISDN_PTMPT */

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

   /* Send Connect Acknowledge message before passing 
    * InUiIntConCfm to layer 4
    */

      /* if ack option, send Connect Ack to Lower Layer */
#ifdef ISDN_PTMPT
      if ((pcb->cfg.intCfg != INTCFG_MULTI)
              || (ntc->setupBCast != TRUE))
      {
#endif /* ISDN_PTMPT */
         if (pcb->cfg.ackOpt)
         {
            inUpdSts(&pcb->sts.connAckTx, pcb);

            /* prepare Pdu header */
            inInitPduHdr((U8) PD_Q931, ntc->origin, pcb->cfg.callRefLen, 
                         ntc->dCallRef, (U8) M_CONNACK, &pduHdr);
           
            MFINITPDU(&ntc->dCallCb->mfMsgCtl, ret, (U8) 0, 
                      (U8) MI_CONNACK, NULLP, (ElmtHdr *) ntc->pduSp, 
                      (U8) PRSNT_DEF, pcb->cfg.swtch, ntc->callType);
            inGenPdu(dCb, &pduHdr, ntc->pduSp, pcb->cfg.swtch, 
                     ntc->callType);
         }
#ifdef ISDN_PTMPT 
      }
#endif /* ISDN_PTMPT */

   InUiIntConCfm(&tCb->pst, tCb->suId, ntc->suInstId, ntc->tCallRef, 
                 &ev.m.cnStEvnt, pcb->suId, dCb->ces);

   if (inInit.acnt)
   {
      SGetSysTime(&ntc->calDura);
   }

   zqUpdPeer();

   RETVALUE(ROK);
} /* end of inNetE02S07 */






  
/*
 *
 *       Fun:   inNetE02S09
 *
 *       Desc:  Input: Connect
 *              State: incoming call proceeding
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
         File:  qn_bdy5.c
 *
*/

#ifdef ANSI
PUBLIC S16 inNetE02S09
(
InNtc *ntc,
InCb *dCb 
)
#else
PUBLIC S16 inNetE02S09(ntc, dCb)
InNtc *ntc;
InCb *dCb;
#endif
{
   InCb *tCb;
   InPCB *pcb;
   PduHdr pduHdr;
   AllSdus ev;
   U8 tmrNum;
   S16 ret;

   TRC2(inNetE02S09);

   tCb = ntc->tCallCb;

   /* get physical link control block */
   if ((pcb = *(pcbLstPtr + dCb->suId)) == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      INLOGERROR(ERRCLS_DEBUG, EIN361, (ErrVal) dCb->rSuId,
                 "inNetE02S09() failed, unable to access DLSAP.");
#endif
      RETVALUE(RFAILED);
   }

   inUpdSts(&pcb->sts.connRx, pcb);


#ifdef ISDN_PTMPT
   if (!ntc->pduSp->m.conn.chanId.eh.pres)
      inInsrtChnlId(ntc, &ntc->pduSp->m.conn.chanId, ntc->ctldPcb);
    
   if ((ret = inHandleMptChannelCheck(ntc, dCb, pcb, 
                                      &ntc->pduSp->m.conn.chanId, 
                                      MI_CONNECT)) != ROK)
   {
      RETVALUE(ROK);
   }
#endif /* ISDN_PTMPT */

   /* 
    * Stop Timer T310 
    */
   for (tmrNum = 0; tmrNum < MAXSIMTIMER; tmrNum++)
      if ((ntc->timers[tmrNum].tmrEvnt == TMR_T310)||
          (ntc->timers[tmrNum].tmrEvnt == TMR_TPROG)||
          (ntc->timers[tmrNum].tmrEvnt == TMR_T301))
         inRmvNtcTq(ntc, tmrNum);

   MFINITSDU(&tCb->mfMsgCtl, ret, (U8) MI_CONNECT, (U8) PM_CNSTREQ, 
             (ElmtHdr *) &ntc->pduSp->m.conn, (ElmtHdr *) &ev, (U8)PRSNT_NODEF,
             tCb->sapCfg.swtch, ntc->callType);

   /* TELICA-BUG:5126-mqin-11/12/01 - cause code Issue 101 */
   if (ntc->pduSp->m.conn.chanId.eh.pres)
   {
      if (pcb->cfg.swtch == SW_NI2)
      {
         /* temporarily change call state to ST_CONNRQ(8) for STATUS */
         ntc->state = ST_CONNRQ;

         INSETTOKVAL(ntc->causeDgn.causeVal, CCINVINFOEL);
         INSETTOKVAL(ntc->causeDgn.location, LOC_PUBNETLU);
         ntc->causeDgn.dgnVal.pres = PRSNT_NODEF;
         ntc->causeDgn.dgnVal.len = 1;
         ntc->causeDgn.dgnVal.val[0] = ME_CHANID;
         inGenStaMsg(ntc, &ntc->causeDgn, dCb);
      }
   }

   /*
    * Change State to active (10)
    */

   ntc->state = ST_ACTVE;


#ifdef ISDN_PTMPT
   if ((pcb->cfg.intCfg == INTCFG_MULTI)
           && (ntc->setupBCast == TRUE))
      ntc->state = ST_CONNRQ;
#endif /* ISDN_PTMPT */

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

#ifdef ISDN_PTMPT
      if ((pcb->cfg.intCfg != INTCFG_MULTI)
              || (ntc->setupBCast != TRUE))
      {
#endif /* ISDN_PTMPT */
         if (pcb->cfg.ackOpt)
         {
            inUpdSts(&pcb->sts.connAckTx, pcb);

            /* prepare Pdu header */
            inInitPduHdr((U8) PD_Q931, ntc->origin, pcb->cfg.callRefLen, 
                         ntc->dCallRef,  (U8) M_CONNACK, &pduHdr);
           
            MFINITPDU(&ntc->dCallCb->mfMsgCtl, ret, (U8) 0, 
                      (U8) MI_CONNACK, NULLP, 
                      (ElmtHdr *) ntc->pduSp, (U8) PRSNT_DEF, pcb->cfg.swtch, 
                      ntc->callType);
            inGenPdu(dCb, &pduHdr, ntc->pduSp, pcb->cfg.swtch, 
                     ntc->callType);
         }
#ifdef ISDN_PTMPT 
      }
#endif /* ISDN_PTMPT */

   InUiIntConCfm(&tCb->pst, tCb->suId, ntc->suInstId, ntc->tCallRef, 
                 &ev.m.cnStEvnt, pcb->suId, dCb->ces);

   if (inInit.acnt)
   {
      SGetSysTime(&ntc->calDura);
   }

   zqUpdPeer();

   RETVALUE(ROK);
} /* end of inNetE02S09 */






  
/*
 *
 *       Fun:   inNetE02S25
 *
 *       Desc:  Input: Connect
 *              State: overlap receiving
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
         File:  qn_bdy5.c
 *
*/

#ifdef ANSI
PUBLIC S16 inNetE02S25
(
InNtc *ntc,
InCb *dCb 
)
#else
PUBLIC S16 inNetE02S25(ntc, dCb)
InNtc *ntc;
InCb *dCb;
#endif
{
   InCb *tCb = NULL;
   InPCB *pcb = NULL;
   PduHdr pduHdr;
   AllSdus ev;
   U8 tmrNum = 0;
   S16 ret = 0;
   
   TRC2(inNetE02S25);

   UNUSED(ret);
   UNUSED(tmrNum);
   UNUSED(ev);
   UNUSED(pcb);
   UNUSED(tCb);
   UNUSED(pduHdr);
   UNUSED(ntc);
   UNUSED(dCb);
 
#ifdef ISDN_OVERLAP
   tCb = ntc->tCallCb;
   /* get physical link control block */
   if ((pcb = *(pcbLstPtr + dCb->suId)) == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      INLOGERROR(ERRCLS_DEBUG, EIN362, (ErrVal) dCb->rSuId,
                 "inNetE02S25() failed, unable to access DLSAP.");
#endif
      RETVALUE(RFAILED);
   }

   inUpdSts(&pcb->sts.connRx, pcb);

   switch (pcb->cfg.swtch)
   {
#ifdef ETSI                   /* TELICA-dsolanki-INTMGCDEV */
      case SW_ETSI:          /* switch - etsi */
#ifdef NETSIDE
#ifdef ISDN_PTMPT
         if ((pcb->cfg.intCfg == IN_INTCFG_MULTI) &&
             (pcb->cfg.intType == NETWORK) &&
             (ntc->setupBCast == TRUE))
         {
            if (!ntc->pduSp->m.conn.chanId.eh.pres)
               inInsrtChnlId(ntc, &ntc->pduSp->m.conn.chanId, pcb, chan);

            if ((ret = inHandleMptChannelCheck(ntc, dCb, pcb,
                                               &ntc->pduSp->m.conn.chanId,
                                               MI_CONNECT)) != ROK)
            {
               RETVALUE(ROK);
            }
         }
#endif /* ISDN_PTMPT */
#endif /* NETSIDE */

#endif                       /* TELICA-dsolanki-INTMGCDEV */

      case SW_CCITT:
         /* Stop T304 */
         for (tmrNum = 0; tmrNum < MAXSIMTIMER; tmrNum++)
            if (ntc->timers[tmrNum].tmrEvnt == TMR_T304) 
               inRmvNtcTq(ntc, tmrNum);
   
         /* validate Channel */
         if ((ret = inValChnlId(ntc, &ntc->pduSp->m.conn.chanId)) != ROK)
         {
            /* Marker PT_CHG#35 */
            inGenRelUpLw(ntc, &ntc->causeDgn, dCb);
            RETVALUE(ROK);
         }
         MFINITSDU(&tCb->mfMsgCtl, ret, (U8) MI_CONNECT, (U8) PM_CNSTREQ, 
                   (ElmtHdr *) &ntc->pduSp->m.conn, (ElmtHdr *) &ev, 
                   (U8) PRSNT_NODEF, tCb->sapCfg.swtch, ntc->callType);
         pcb->sts.cons++;
         pcb->sts.aveCalls++;

         /* Change State to active (10) */
         ntc->state = ST_ACTVE;

         if (ntc->updStatus == NTC_NOTCREATED)
         {
            ret = zqRunTimeUpd(ZQ_NTC_CB, CMPFTHA_CREATE_REQ, (PTR)ntc);
            if (ret == ROK)
            ntc->updStatus = NTC_CREATED;
         }
         else
            zqRunTimeUpd(ZQ_NTC_CB, CMPFTHA_UPD_REQ, (PTR)ntc);


         /* Send Connect Acknowledge message before passing 
          * InUiIntConCfm to layer 4
          */
         if (pcb->cfg.ackOpt)
         {
            inUpdSts(&pcb->sts.connAckTx, pcb);

            /* prepare Pdu header */
            inInitPduHdr((U8) PD_Q931, ntc->origin, pcb->cfg.callRefLen, 
                         ntc->dCallRef, (U8) M_CONNACK, &pduHdr);

            MFINITPDU(&ntc->dCallCb->mfMsgCtl, ret, (U8) 0, (U8) MI_CONNACK, 
                      NULLP, (ElmtHdr *) ntc->pduSp, (U8) PRSNT_DEF, 
                      pcb->cfg.swtch, ntc->callType);
            inGenPdu(dCb, &pduHdr, ntc->pduSp, pcb->cfg.swtch, 
                     ntc->callType);
         }
         InUiIntConCfm(&tCb->pst, tCb->suId, ntc->suInstId, ntc->tCallRef, 
                       &ev.m.cnStEvnt, pcb->suId, dCb->ces);

         /* Generate Connect Response to Upper Layer */

         if (inInit.acnt)
         {
            SGetSysTime(&ntc->calDura);
         }
         break;

      default:
#if (ERRCLASS & ERRCLS_DEBUG)
         INLOGERROR(ERRCLS_DEBUG, EIN363, (ErrVal) pcb->cfg.swtch,
                    "inNetE02S25() failed, invalid message.");
#endif
         break;
   }
#endif

   zqUpdPeer();

   RETVALUE(ROK);
} /* end of inNetE02S25 */






/*
*
*       Fun:   inNetE03S08
*
*       Desc:  input: Connect Acknowledge
*              state: Connect Request
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  qn_bdy5.c
*
*/

#ifdef ANSI
PUBLIC S16 inNetE03S08
(
InNtc *ntc,
InCb *dCb 
)
#else
PUBLIC S16 inNetE03S08(ntc, dCb)
InNtc *ntc;
InCb *dCb;
#endif
{              
        
   RETVALUE(ROK);
} /* end of inNetE03S08 */


/* TELICA-BUG:5126-mqin-11/19/01 - cause code Issue 76 */
/*
*
*       Fun:   inNetE03S10
*
*       Desc:  input: Connect Acknowledge
*              state: Active
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  in_bdy5.c
*
*/

#ifdef ANSI
PUBLIC S16 inNetE03S10
(
InNtc *ntc,
InCb *dCb
)
#else
PUBLIC S16 inNetE03S10(ntc, dCb)
InNtc *ntc;
InCb *dCb;
#endif
{
#if 1 /*Bug 78505*/
   InPCB *pcb;
#endif
   TRC2(inNetE03S10);

#if 1 /*Bug 78505*/
   /* get physical link control block */
   if ((pcb = *(pcbLstPtr + dCb->suId)) == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      INLOGERROR(ERRCLS_DEBUG, EIN360, (ErrVal) dCb->rSuId,
            "inNetE03S10() failed, unable to access DLSAP.");
#endif
      RETVALUE(RFAILED);
   }
   /* add the counter for CONACK msg received */ 
   inUpdSts(&pcb->sts.connAckRx, pcb);
#endif

   if (!ntc->origin)
   {
      ntc->causeDgn.eh.pres = PRSNT_NODEF;
      INSETTOKVAL(ntc->causeDgn.causeVal, CCMSGNOTCMP);
      INSETTOKVAL(ntc->causeDgn.location, LOC_PUBNETLU);
      ntc->causeDgn.dgnVal.pres = PRSNT_NODEF;
      ntc->causeDgn.dgnVal.len = 1;
      ntc->causeDgn.dgnVal.val[0] = dCb->mfMsgCtl.msgType;

      inGenStaMsg(ntc, &ntc->causeDgn, dCb);
   }
   else
   {
      UNUSED(ntc);
      UNUSED(dCb);
   }

   RETVALUE(ROK);
} /* end of inNetE03S10 */


  
/*
 *
 *       Fun:   inNetE04S06
 *
 *       Desc:  Input: Progress
 *              State: call present
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
         File:  qn_bdy5.c
 *
*/

#ifdef ANSI
PUBLIC S16 inNetE04S06
(
InNtc *ntc,
InCb *dCb
)
#else
PUBLIC S16 inNetE04S06(ntc, dCb)
InNtc *ntc;
InCb *dCb;
#endif
{
   InCb *tCb;
   InPCB *pcb;
   AllSdus ev;
   S16 ret;
   U8 tmrNum;
   CctCriEnt inCri;
   U8 string[IN_GEO_CRI_LEN_MAX];

   TRC2(inNetE04S06);
   tCb = ntc->tCallCb;

   /* get physical link control block */
   if ((pcb = *(pcbLstPtr + dCb->suId)) == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      INLOGERROR(ERRCLS_DEBUG, EIN365, (ErrVal) dCb->rSuId,
                 "inNetE04S06() failed, unable to access DLSAP.");
#endif
      RETVALUE(RFAILED);
   }

   inUpdSts(&pcb->sts.progressRx, pcb);

   /* if this is the first response to setup message,
    *  then insert channel id, NOTE channel id being
    *  inserted in the event structure
    */
   if ((pcb->cfg.intType == NETWORK) && 
       (ntc->state == ST_CALLPRES))
      inInsrtChnlId(ntc, &ev.m.cnStEvnt.chanId,ntc->ctldPcb);


   switch (pcb->cfg.swtch)
   {
      case SW_NI2:
         /* Stop T303 */
         for (tmrNum = 0; tmrNum < MAXSIMTIMER; tmrNum++)
            if (ntc->timers[tmrNum].tmrEvnt == TMR_T303)  
               inRmvNtcTq(ntc, tmrNum);
         inStartNtcTmr(TMR_T310, ntc, ntc->dCallCb);
         MFINITSDU(&tCb->mfMsgCtl, ret, (U8) MI_PROGRESS, (U8) PM_CNSTREQ, 
                   (ElmtHdr *) &ntc->pduSp->m.progress, (ElmtHdr *) &ev, 
                   (U8) PRSNT_NODEF, tCb->sapCfg.swtch, ntc->callType);
         /* Fill CRI Information in CnStEvnt */
	 INDBGP(DBGMASK_LI,"inNetE04S06: ****** tCallRef = %ld - dCallRef = %d *******\n",
	              ntc->tCallRef, ntc->dCallRef);

         inFillCriInfo(ntc->dCallRef, &ev.m.cnStEvnt.criList, &inCri, string);
         /* Send Progress Indication to Upper */
         InUiIntCnStInd(&tCb->pst, tCb->suId, ntc->suInstId, ntc->tCallRef, 
                        &ev.m.cnStEvnt, MI_PROGRESS, pcb->suId, dCb->ces);
         break;
      case SW_ATT4E:
      case SW_ATT5EB:
      case SW_ATT5EP:
         /* Stop T303 */
         for (tmrNum = 0; tmrNum < MAXSIMTIMER; tmrNum++)
            if (ntc->timers[tmrNum].tmrEvnt == TMR_T303)  
               inRmvNtcTq(ntc, tmrNum);

         /* Channel Negotiation */
         if (ntc->pduSp->m.progress.chanId.eh.pres)
         {
            if ((ret = inNegChan(ntc, &ntc->pduSp->m.callProc.chanId, 
                                 &ntc->causeDgn)) != ROK)
            {
               inGenStaMsg(ntc, &ntc->causeDgn, dCb);
               RETVALUE(ROK);
            }
            if (ntc->causeDgn.causeVal.pres)
            {
               inGenRelUpLw(ntc, &ntc->causeDgn, dCb);
               RETVALUE(ROK);
            }
            ntc->causeDgn.eh.pres = PRSNT_NODEF;
            ntc->pduSp->m.progress.chanId.prefExc.val = PE_EXCLSVE;
         }
         else
            /* insert channel Id  */
            inInsrtChnlId(ntc, &ntc->pduSp->m.progress.chanId, ntc->ctldPcb);

         /* Change State to Call receieved */      
         ntc->state = ST_CALLRECV;

         if (ntc->updStatus == NTC_NOTCREATED)
         {
            ret = zqRunTimeUpd(ZQ_NTC_CB, CMPFTHA_CREATE_REQ, (PTR)ntc);
            if (ret == ROK)
               ntc->updStatus = NTC_CREATED;
         }
         else
            zqRunTimeUpd(ZQ_NTC_CB, CMPFTHA_UPD_REQ, (PTR)ntc);

         MFINITSDU(&tCb->mfMsgCtl, ret, (U8) MI_PROGRESS, (U8) PM_CNSTREQ, 
                   (ElmtHdr *) &ntc->pduSp->m.progress, (ElmtHdr *) &ev, 
                   (U8) PRSNT_NODEF, tCb->sapCfg.swtch, ntc->callType);
         /* Fill CRI Information in CnStEvnt */
         INDBGP(DBGMASK_LI,"inNetE04S06: ****** tCallRef = %ld - dCallRef = %d *******\n",
	                        ntc->tCallRef, ntc->dCallRef);

         inFillCriInfo(ntc->dCallRef, &ev.m.cnStEvnt.criList, &inCri, string);
         /* Send Progress Indication to Upper */
         InUiIntCnStInd(&tCb->pst, tCb->suId, ntc->suInstId, ntc->tCallRef, 
                        &ev.m.cnStEvnt, MI_PROGRESS, pcb->suId, dCb->ces);
         break;

      case SW_CCITT:
#ifdef ETSI                  /* TELICA-dsolanki-INTMGCDEV */
      case SW_ETSI:         /* switch - etsi */
#endif                      /* TELICA-dsolanki-INTMGCDEV */
         /* Stop T303 */
         for (tmrNum = 0; tmrNum < MAXSIMTIMER; tmrNum++)
            if (ntc->timers[tmrNum].tmrEvnt == TMR_T303)  
               inRmvNtcTq(ntc, tmrNum);
         break;

      case SW_NTDMS100B:     /* switch - nt dms100 */
      case SW_NTDMS100P:     /* switch - nt dms100 */
         MFINITSDU(&tCb->mfMsgCtl, ret, (U8) MI_PROGRESS, (U8) PM_CNSTREQ, 
                   (ElmtHdr *) &ntc->pduSp->m.progress, (ElmtHdr *) &ev, 
                   (U8) PRSNT_NODEF, tCb->sapCfg.swtch, ntc->callType);
         /* Fill CRI Information in CnStEvnt */
        INDBGP(DBGMASK_LI,"inNetE04S06: ****** tCallRef = %ld - dCallRef = %d *******\n",
	                 ntc->tCallRef, ntc->dCallRef);

         inFillCriInfo(ntc->dCallRef, &ev.m.cnStEvnt.criList, &inCri, string);
         /* Send Progress Indication to Upper */
         InUiIntCnStInd(&tCb->pst, tCb->suId, ntc->suInstId, ntc->tCallRef, 
                        &ev.m.cnStEvnt, MI_PROGRESS, pcb->suId, dCb->ces);
         break;
      default:
#if (ERRCLASS & ERRCLS_DEBUG)
         INLOGERROR(ERRCLS_DEBUG, EIN366, (ErrVal) pcb->cfg.swtch,
                    "inNetE04S06() failed, invalid message.");
#endif
         inNetMSGINV(ntc, dCb);
         break;
   }

   zqUpdPeer();

   RETVALUE(ROK);
} /* end of inNetE04S06 */





  
/*
 *
 *       Fun:   inNetE04S07
 *
 *       Desc:  Input: Progress
 *              State: call received
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
         File:  qn_bdy5.c
 *
*/

#ifdef ANSI
PUBLIC S16 inNetE04S07
(
InNtc *ntc,
InCb *dCb 
)
#else
PUBLIC S16 inNetE04S07(ntc, dCb)
InNtc *ntc;
InCb *dCb;
#endif
{
   InCb *tCb;
   InPCB *pcb;
   AllSdus ev;
   S16 ret;
   U8 tmrNum;

   TRC2(inNetE04S07);
   
   tCb = ntc->tCallCb;
   /* get physical link control block */
   if ((pcb = *(pcbLstPtr + dCb->suId)) == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      INLOGERROR(ERRCLS_DEBUG, EIN367, (ErrVal) dCb->rSuId,
                 "inNetE04S07() failed, unable to access DLSAP.");
#endif
      RETVALUE(RFAILED);
   }
   inUpdSts(&pcb->sts.progressRx, pcb);


#ifdef ISDN_PTMPT
   IN_PTMPT_ISSUE_STATUS(pcb, ntc, dCb);
#endif /* ISDN_PTMPT */

   switch (pcb->cfg.swtch)
   {
      case SW_ATT4E:
      case SW_ATT5EB:
      case SW_ATT5EP:
         if (ntc->progCnt)
         {
            /* Send Status Event */
            INSETTOKVAL(ntc->causeDgn.causeVal, CCSERVUNAVAIL);
            inGenStaCfm(ntc, &ntc->causeDgn);
            RETVALUE(ROK);
         }
         else
         {
            ntc->progCnt = 1;

            /* Stop T303 */
            for (tmrNum = 0; tmrNum < MAXSIMTIMER; tmrNum++)
               if (ntc->timers[tmrNum].tmrEvnt == TMR_T303)  
                  inRmvNtcTq(ntc, tmrNum);
      
            MFINITSDU(&tCb->mfMsgCtl, ret, (U8) MI_PROGRESS, (U8) PM_CNSTREQ, 
                      (ElmtHdr *) &ntc->pduSp->m.progress, (ElmtHdr *) &ev, 
                      (U8) PRSNT_NODEF, tCb->sapCfg.swtch, ntc->callType);

            /* Send Progress Indication to Upper */
            InUiIntCnStInd(&tCb->pst, tCb->suId, ntc->suInstId, 
                           ntc->tCallRef, &ev.m.cnStEvnt, MI_PROGRESS, 
                           pcb->suId, dCb->ces);

         }
         break;
      case SW_CCITT:
         break;

      case SW_NTDMS100B:     /* switch - nt dms100 */
      case SW_NTDMS100P:     /* switch - nt dms100 */
      case SW_NTDMS250:      /* switch - nt dms250 */
         MFINITSDU(&tCb->mfMsgCtl, ret, (U8) MI_PROGRESS, (U8) PM_CNSTREQ, 
                   (ElmtHdr *) &ntc->pduSp->m.progress, (ElmtHdr *) &ev, 
                   (U8) PRSNT_NODEF, tCb->sapCfg.swtch, ntc->callType);
         /* Send Progress Indication to Upper */
         InUiIntCnStInd(&tCb->pst, tCb->suId, ntc->suInstId, ntc->tCallRef,
                        &ev.m.cnStEvnt, MI_PROGRESS, pcb->suId, dCb->ces);

         break;

#ifdef ETSI
      case SW_ETSI:         /*switch - ETSI Bug:84046, follow NI2*/
#endif
      case SW_NI2:          /* switch - NI2 */
         {
            if ((ntc->pduSp->m.progress.progInd.eh.pres) &&
                (ntc->pduSp->m.progress.progInd.progDesc.val == PD_NOTETEISDN))
            {
               MFINITSDU(&tCb->mfMsgCtl, ret, (U8) MI_PROGRESS, (U8) PM_CNSTREQ, 
                         (ElmtHdr *) &ntc->pduSp->m.progress, (ElmtHdr *) &ev, 
                         (U8) PRSNT_NODEF, tCb->sapCfg.swtch, ntc->callType);

               /* Send Progress Indication to Upper */
               InUiIntCnStInd(&tCb->pst, tCb->suId, ntc->suInstId, 
                              ntc->tCallRef,&ev.m.cnStEvnt, MI_PROGRESS, 
                              pcb->suId, dCb->ces);

            }
            break;
         }

         /* End of Changed */

      default:
#if (ERRCLASS & ERRCLS_DEBUG)
         INLOGERROR(ERRCLS_DEBUG, EIN368, (ErrVal) pcb->cfg.swtch,
                    "inNetE04S07() failed, invalid message.");
#endif
         inNetMSGINV(ntc, dCb);
         break;
   }
   RETVALUE(ROK);
} /* end of inNetE04S07 */






  
/*
 *
 *       Fun:   inNetE04S09
 *
 *       Desc:  Input: Progress
 *              State: incoming call proceeding
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
         File:  qn_bdy5.c
 *
*/

#ifdef ANSI
PUBLIC S16 inNetE04S09
(
InNtc *ntc,
InCb *dCb
)
#else
PUBLIC S16 inNetE04S09(ntc, dCb)
InNtc *ntc;
InCb *dCb;
#endif
{
   InCb *tCb;
   InPCB *pcb;
   AllSdus ev;
   U8 tmrNum;
   S16 ret;


   TRC2(inNetE04S09);

   tCb = ntc->tCallCb;
   /* get physical link control block */
   if ((pcb = *(pcbLstPtr + dCb->suId)) == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      INLOGERROR(ERRCLS_DEBUG, EIN369, (ErrVal) dCb->rSuId,
                 "inNetE04S09() failed, unable to access DLSAP.");
#endif
      RETVALUE(RFAILED);
   }
   inUpdSts(&pcb->sts.progressRx, pcb);


#ifdef ISDN_PTMPT
   IN_PTMPT_ISSUE_STATUS(pcb, ntc, dCb);
#endif /* ISDN_PTMPT */   

   /* validate Channel */
   if ((ret = inValChnlId(ntc, &ntc->pduSp->m.progress.chanId)) != ROK)
   {
      inGenRelUpLw(ntc, &ntc->causeDgn, dCb);
      RETVALUE(ROK);
   }

   /* 
    * Stop Timer T310 
    */
   for (tmrNum = 0; tmrNum < MAXSIMTIMER; tmrNum++)
      if (ntc->timers[tmrNum].tmrEvnt == TMR_T310)  
         inRmvNtcTq(ntc, tmrNum);

   switch (pcb->cfg.swtch)
   {
      case SW_ATT4E:
      case SW_ATT5EB:
      case SW_ATT5EP:
         ntc->state = ST_CALLRECV;
         if (ntc->updStatus == NTC_NOTCREATED)
         {
            ret = zqRunTimeUpd(ZQ_NTC_CB, CMPFTHA_CREATE_REQ, (PTR)ntc);
            if (ret == ROK)
               ntc->updStatus = NTC_CREATED;
         }
         else
            zqRunTimeUpd(ZQ_NTC_CB, CMPFTHA_UPD_REQ, (PTR)ntc);
         break;
      case SW_NTDMS100B:    /* switch - nt dms100 */
      case SW_NTDMS100P:    /* switch - nt dms100 */
      case SW_NTDMS250:     /* switch - nt dms250 */
      case SW_CCITT:
      case SW_NI2:
#ifdef ETSI                 /* TELICA-dsolanki-INTMGCDEV */
      case SW_ETSI:         /* switch - etsi */
#endif                      /* TELICA-dsolanki-INTMGCDEV */
         break;

      default:
#if (ERRCLASS & ERRCLS_DEBUG)
         INLOGERROR(ERRCLS_DEBUG, EIN371, (ErrVal)pcb->cfg.swtch,
                    "inNetE04S09() failed, invalid switch.");
#endif
         RETVALUE(RFAILED);
   }

   MFINITSDU(&tCb->mfMsgCtl, ret, (U8) MI_PROGRESS, (U8) PM_CNSTREQ, 
             (ElmtHdr *) &ntc->pduSp->m.progress, (ElmtHdr *) &ev, 
             (U8) PRSNT_NODEF, tCb->sapCfg.swtch, ntc->callType);

   if (pcb->cfg.swtch == SW_NI2)
   {
      inStartNtcTmr(TMR_TPROG, ntc, ntc->dCallCb);
   }
   else
      /* 
       * Start T_310 Timer 
       */
     /* TELICA-MCAO-03/26/02: BUG 9953 start T301/T_PRG instead of T310 */
     inStartNtcTmr(TMR_T301, ntc, ntc->dCallCb);
      
   /* Send Progress Indication to Upper */
   InUiIntCnStInd(&tCb->pst, tCb->suId, ntc->suInstId, ntc->tCallRef, 
                  &ev.m.cnStEvnt, MI_PROGRESS, pcb->suId, dCb->ces);

   RETVALUE(ROK);
} /* end of inNetE04S09 */






  
/*
 *
 *       Fun:   inNetE04S25
 *
 *       Desc:  Input: Progress
 *              State: overlap receiving
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
         File:  qn_bdy5.c
 *
*/

#ifdef ANSI
PUBLIC S16 inNetE04S25
(
InNtc *ntc,
InCb *dCb 
)
#else
PUBLIC S16 inNetE04S25(ntc, dCb)
InNtc *ntc;
InCb *dCb;
#endif
{
   InCb *tCb = NULL;
   InPCB *pcb = NULL;
   AllSdus ev;
   U8 tmrNum = 0;
   S16 ret = 0;

   TRC2(inNetE04S25);
   UNUSED(ret);
   UNUSED(tmrNum);
   UNUSED(ev);
   UNUSED(pcb);
   UNUSED(tCb);
   UNUSED(ntc);
   UNUSED(dCb);


#ifdef ISDN_OVERLAP
   tCb = ntc->tCallCb;
   /* get physical link control block */
   if ((pcb = *(pcbLstPtr + dCb->suId)) == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      INLOGERROR(ERRCLS_DEBUG, EIN372, (ErrVal) dCb->rSuId,
                 "inNetE04S25() failed, unable to access DLSAP.");
#endif
      RETVALUE(RFAILED);
   }

   inUpdSts(&pcb->sts.progressRx, pcb);

   switch (pcb->cfg.swtch)
   {
#ifdef ETSI                  /* TELICA-dsolanki-INTMGCDEV */
      case SW_ETSI:          /* switch - etsi */
#ifdef NETSIDE
#ifdef ISDN_PTMPT
         IN_PTMPT_ISSUE_STATUS(pcb, ntc, dCb);
#endif /* ISDN_PTMPT */
#endif /* NETSIDE */

#endif                      /* TELICA-dsolanki-INTMGCDEV */
      case SW_CCITT:
         /* Stop T304 */
         for (tmrNum = 0; tmrNum < MAXSIMTIMER; tmrNum++)
            if (ntc->timers[tmrNum].tmrEvnt == TMR_T304) 
               inRmvNtcTq(ntc, tmrNum);
   
         /* Start T_304 Timer */
         inStartNtcTmr(TMR_T304, ntc, ntc->dCallCb);

         MFINITSDU(&tCb->mfMsgCtl, ret, (U8) MI_PROGRESS, (U8) PM_CNSTREQ, 
                   (ElmtHdr *) &ntc->pduSp->m.progress, (ElmtHdr *) &ev, 
                   (U8) PRSNT_NODEF, tCb->sapCfg.swtch, ntc->callType);
         /* Send Progress Indication to Upper */
         InUiIntCnStInd(&tCb->pst, tCb->suId, ntc->suInstId, ntc->tCallRef, 
                        &ev.m.cnStEvnt, MI_PROGRESS, pcb->suId, dCb->ces);
         break;

      default:
#if (ERRCLASS & ERRCLS_DEBUG)
         INLOGERROR(ERRCLS_DEBUG, EIN373, (ErrVal) pcb->cfg.swtch,
                    "inNetE04S25() failed, invalid message.");
#endif
         inNetMSGINV(ntc, dCb);
         break;
   }
#endif
   RETVALUE(ROK);
} /* end of inNetE04S25 */

  
/*
 *
 *       Fun:   inNetE05S00
 *
 *       Desc:  Input: Setup
 *              State: null
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
         File:  qn_bdy5.c
 *
*/

#ifdef ANSI
PUBLIC S16 inNetE05S00
(
InNtc *ntc,
InCb *dCb 
)
#else
PUBLIC S16 inNetE05S00(ntc, dCb)
InNtc *ntc;
InCb *dCb;
#endif
{
   InCb *tCb;
   InPCB *pcb;
   UConnId ref;
   TknU16 intId;
   AllSdus ev;
   PduHdr pduHdr;
   AllPdus allPdus;
#if 0       /* bug 54842 */
   Status status;
#endif
   U16 i;
   Bool overLap;
   S16 ret;
   RoutEntry *rout = NULL;
   CctCriEnt inCri;
   U8 string[IN_GEO_CRI_LEN_MAX];
   InDigitMap *pDigitMap;

   TRC2(inNetE05S00);

   UNUSED(rout);
   UNUSED(allPdus);
   UNUSED(pduHdr);

   overLap = FALSE;
   callDir = CALL_IN;

   /* get physical link control block */
   if ((pcb = *(pcbLstPtr + dCb->suId)) == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      INLOGERROR(ERRCLS_DEBUG, EIN374, (ErrVal) dCb->rSuId,
                 "inNetE05S00() failed, unable to access DLSAP.");
#endif
      RETVALUE(RFAILED);
 
   }

   /* initialize tCb */
   tCb = (InCb *) NULLP;

   /* Bug 92814 clean the stack to ensure no garbage data sent to GCC */
   cmMemset((U8 *)&ev, 0, sizeof(AllSdus));

   ntc->origin = ORIGINATOR;
   ntc->toBeAlctdBChanlDir = IN_BCHAN_INCOMING;

#ifdef IN_ACC /* TELICA-BUG:18735-mqin */
   ISDN_INC_INCOMING_CALL_CNTR();
   IN_CHKDQCONG(&inDmndQCong, in_curr_cong);
   if (in_curr_cong)
   {
      if (IN_ACC_SHOULD_REJECT(inDmndQCong))
      {
         /* release the call */
         DP("Rejecting a call due to congestion\n");
         INSETTOKVAL(ntc->causeDgn.location, LOC_PUBNETLU);
         INSETTOKVAL(ntc->causeDgn.causeVal, CCSWTCHCONG);
         inGenRelCLw(ntc, &ntc->causeDgn, dCb);
         RETVALUE(ROK);
      }
   }
#endif /* BUG:18735 */

#ifdef IN_NUMSCREEN /* TELICA-MCAO-10/31/01: BUG 7129, pre-screen cg/cd num */
   /* BUG:29212-mqin - screen calling/called party number seperately */
   if (inScreenCgPtyNum(&(ntc->pduSp->m.setup)) != ROK)
   {
      /* send STATUS, discard calling party number and proceed */
      INSETTOKVAL(ntc->causeDgn.causeVal, CCINVINFOEL);
      INSETTOKVAL(ntc->causeDgn.location, LOC_PUBNETLU);
      ntc->causeDgn.dgnVal.pres = PRSNT_NODEF;
      ntc->causeDgn.dgnVal.len = 1;
      ntc->causeDgn.dgnVal.val[0] = ME_CGPTYNMB;
      inGenStaMsg(ntc, &ntc->causeDgn, dCb);

      ntc->pduSp->m.setup.cgPtyNmb.eh.pres = NOTPRSNT;
      ntc->pduSp->m.setup.cgPtyNmb.nmbDigits.pres = NOTPRSNT;
   }

   if (inScreenCdPtyNum(ntc->pduSp->m.setup.cdPtyNmb, pcb) != ROK)
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
#endif /* IN_NUMSCREEN */

   /* gdixit-05/14/2002-Bug#11027 */
   if (pcb->cfg.swtch == SW_NI2)
   {
      if ((ntc->pduSp->m.setup.trnstNtw.eh.pres == PRSNT_NODEF) ||
          (ntc->pduSp->m.setup.operSysAcc.eh.pres == PRSNT_NODEF))
      {   
         if (ntc->pduSp->m.setup.cdPtyNmb.eh.pres &&    /*Bug67326*/
             (ntc->pduSp->m.setup.cdPtyNmb.nmbPlanId.val == NP_UNK) &&
             (ntc->pduSp->m.setup.cdPtyNmb.typeNmb0.val == TON_UNK))
         {
            INSETTOKVAL(ntc->causeDgn.location, LOC_PUBNETLU);
            INSETTOKVAL(ntc->causeDgn.causeVal, CCINVNMBFRM);
            inGenRelCLw(ntc, &ntc->causeDgn, dCb);
            RETVALUE(ROK);
         }
      }
   }

   inUpdSts(&pcb->sts.setupRx, pcb);

   /* Bug#15960, rkhan */
   /* Special implementation for RIO, Until the spec is provided which says that for DMS100
    * variant we will get a call with 011... with type of number as international number. */
   {
      extern char *dms100_011internationalnumber;
      if (dms100_011internationalnumber)
      {
         if (ntc->pduSp->m.setup.cdPtyNmb.eh.pres &&
             ntc->pduSp->m.setup.cdPtyNmb.nmbDigits.pres &&
             (ntc->pduSp->m.setup.cdPtyNmb.nmbDigits.len >= 3))
         {
            if ((ntc->pduSp->m.setup.cdPtyNmb.nmbDigits.val[0] == '0') &&
                (ntc->pduSp->m.setup.cdPtyNmb.nmbDigits.val[1] == '1') &&
                (ntc->pduSp->m.setup.cdPtyNmb.nmbDigits.val[2] == '1'))
            {
               if (ntc->pduSp->m.setup.cdPtyNmb.typeNmb0.val == TON_INT)
               {
                  ntc->pduSp->m.setup.cdPtyNmb.typeNmb0.val = TON_UNK;
               }
            }
         }
      }
   }

   /* bug 18709 */
   {
     extern char *monmouth_bearCap_adjust;

     if (monmouth_bearCap_adjust)
       {
     BearCap *bearCap = &ntc->pduSp->m.setup.bearCap[0];

     if (bearCap->eh.pres && 
         (bearCap->tranMode.pres) && (bearCap->tranMode.val == TM_CIRCUIT) &&
         (bearCap->infoTranCap.pres) && ((bearCap->infoTranCap.val == ITC_SPEECH) ||
                         (bearCap->infoTranCap.val == ITC_A31KHZ)) &&
         (!bearCap->usrInfoLyr1Prot.pres))
     {
       bearCap->usrInfoLyr1Prot.pres = PRSNT_NODEF;
       bearCap->usrInfoLyr1Prot.val = UIL1_G711ULAW;
       DP("bug 18709 monmouth_bearCap_adjust:  usrInfoLyr1Prot[pres:%d, val:%d]\n", 
          bearCap->usrInfoLyr1Prot.pres, bearCap->usrInfoLyr1Prot.val);
     }
       }
   }

#if 0    /* bug 54842 */
   SChkRes(pcb->pst.region, pcb->pst.pool, &status);

   if ((status < inCp.poolTrUpper) || (ntc->dCallCb->flc == TRUE))
   {

      INSETTOKVAL(ntc->causeDgn.causeVal, CCSWTCHCONG);
      DP("bug 54842 -below threshold, rejecting call \n");
      if (pcb->cfg.swtch == SW_NTDMS100B)
         inGenPrgMsg(ntc, &ntc->causeDgn, dCb);
      else
         inGenRelLw(ntc, &ntc->causeDgn, dCb);
      RETVALUE(ROK);
   }
#endif

   if (!ntc->pduSp->m.setup.sndCmplt.eh.pres)     /* overlap */
   {
      if (pcb->cfg.keyPad)    /* keyPad conveys call info */ 
      {
         if ((ntc->numAdrDig = ntc->pduSp->m.setup.keyFac.keyInfo.len) < 
             pcb->cfg.minAdrDig)
            overLap = TRUE;
      }
      else
      {
         if (ntc->pduSp->m.setup.cdPtyNmb.eh.pres)
         {
            if ((ntc->numAdrDig = 
                 ntc->pduSp->m.setup.cdPtyNmb.nmbDigits.len)
                < pcb->cfg.minAdrDig)
               overLap = TRUE;
         }
         else
            overLap = TRUE;
      }
   }

   if (pcb->cfg.tCbCfg)
   {
      if (!(tCb = inGetSapPtr(pcb->cfg.tCbId)))
      {
         INSETTOKVAL(ntc->causeDgn.causeVal, CCNORTTODEST);
         if (pcb->cfg.swtch == SW_NTDMS100B)
            inGenPrgMsg(ntc, &ntc->causeDgn, dCb);
         else
            inGenRelCLw(ntc, &ntc->causeDgn, dCb);
         RETVALUE(ROK);
      }
      if (inGetInstId(tCb, &ref) == RFAILED)
      {

         INSETTOKVAL(ntc->causeDgn.causeVal, CCRESCUNAVAIL);
         inGenRelCLw(ntc, &ntc->causeDgn, dCb);
         RETVALUE(ROK);
      }
   }
   else
   {
#ifdef ISDN_ROUTE
      if (ntc->pduSp->m.setup.trnstNtw.eh.pres)
      {
         ntc->cmpltAdr = TRUE;
         inAscAdrToBcd(&ntc->pduSp->m.setup.trnstNtw.netId.val[0], 
                       &ntc->dstAdr.nmbDigits.val[0]);
         if ((rout = inRoutSearch(&ntc->dstAdr.nmbDigits.val[0], 
                                  ntc->dstAdr.nmbDigits.len=ntc->pduSp->m.setup.trnstNtw.netId.len,
                                  &pcb->cfg.wcMask[0], TSAP)) == NULLP)
         {
            INSETTOKVAL(ntc->causeDgn.causeVal, CCNORTTOTSFNET);
            if (pcb->cfg.swtch == SW_NTDMS100B)
               inGenPrgMsg(ntc, &ntc->causeDgn, dCb);
            else
               inGenRelLw(ntc, &ntc->causeDgn, dCb);
            RETVALUE(ROK);
         }

         if (!(tCb = inGetSapPtr(rout->sapId)))
         {
            INSETTOKVAL(ntc->causeDgn.causeVal, CCNORTTODEST);
            if (pcb->cfg.swtch == SW_NTDMS100B)
               inGenPrgMsg(ntc, &ntc->causeDgn, dCb);
            else
               inGenRelCLw(ntc, &ntc->causeDgn, dCb);
            RETVALUE(ROK);
         }

         ntc->pduSp->m.setup.trnstNtw.eh.pres = NOTPRSNT;
      }
      else
         if (overLap)     /* overlap */
         {
            ntc->dstAdr.nmbDigits.len = 0;
            if ((pcb->cfg.swtch == SW_ATT4E) || (pcb->cfg.swtch == SW_ATT5EP))
            {
               INSETTOKVAL(ntc->causeDgn.causeVal,  CCINVNMBFRM);
               inGenRelLw(ntc, &ntc->causeDgn, dCb);
               RETVALUE(ROK);
            }

            if (pcb->cfg.swtch == SW_NTDMS100P)
            {
               INSETTOKVAL(ntc->causeDgn.causeVal, CCINVNMBFRM);
               inGenRelLw(ntc, &ntc->causeDgn, dCb);
               RETVALUE(ROK);
            }

            if (pcb->cfg.swtch == SW_NI2)
            {
               INSETTOKVAL(ntc->causeDgn.causeVal, CCINVNMBFRM);
               inGenRelCLw(ntc, &ntc->causeDgn, dCb);
               RETVALUE(ROK);
            }

            if (pcb->cfg.keyPad)    /* keyPad conveys call info */ 
               if (ntc->pduSp->m.setup.keyFac.eh.pres)
               {
                  ntc->dstAdr.nmbDigits.len = 
                    ntc->pduSp->m.setup.keyFac.keyInfo.len;
                  inAscAdrToBcd(&ntc->pduSp->m.setup.keyFac.keyInfo.val[0], 
                                &ntc->dstAdr.nmbDigits.val[0]);
               }

            if (pcb->cfg.useSubAdr)
            {
               if (ntc->pduSp->m.setup.cdPtySad.eh.pres)
                  if (!(inGetDstSubAdr(ntc->dCallCb, 
                                       &ntc->pduSp->m.setup.cdPtySad, &ntc->dstAdr)))
                  {
                     INSETTOKVAL(ntc->causeDgn.causeVal, CCNORTTODEST);
                     if (pcb->cfg.swtch == SW_NTDMS100B)
                        inGenPrgMsg(ntc, &ntc->causeDgn, dCb);
                     else
                        inGenRelLw(ntc, &ntc->causeDgn, dCb);
                     RETVALUE(ROK);
                  }
            }
            else           /* not a subaddress or keyPad */
               if (ntc->pduSp->m.setup.cdPtyNmb.eh.pres)
                  if (!(inGetDstNmb(ntc->dCallCb, 
                                    &ntc->pduSp->m.setup.cdPtyNmb, &ntc->dstAdr)))
                  {
                     INSETTOKVAL(ntc->causeDgn.causeVal, CCNORTTODEST); 
                     if (pcb->cfg.swtch == SW_NTDMS100B)
                        inGenPrgMsg(ntc, &ntc->causeDgn, dCb);
                     else
                        inGenRelLw(ntc, &ntc->causeDgn, dCb);
                     RETVALUE(ROK);
                  }
#ifdef ISDN_OVERLAP
            ntc->numAdrDig = ntc->dstAdr.nmbDigits.len;
            inGetBChan(ntc, &ntc->pduSp->m.setup.chanId, &intId,
                       &ntc->pduSp->m.setup.bearCap[0], &ntc->causeDgn);
            if (ntc->causeDgn.causeVal.pres)
            {
               if (ntc->pduSp->m.setup.bearCap[1].eh.pres)
                  inGetBChan(ntc, &ntc->pduSp->m.setup.chanId, &intId,
                             &ntc->pduSp->m.setup.bearCap[1], &ntc->causeDgn);
               else
               {
                  inGenRelCLw(ntc, &ntc->causeDgn, dCb);
                  RETVALUE(ROK);
               }
               if (ntc->causeDgn.causeVal.pres)
               {
                  inGenRelCLw(ntc, &ntc->causeDgn, dCb);
                  RETVALUE(ROK);
               }
            }
            ntc->causeDgn.eh.pres = PRSNT_NODEF;
            /* force exclusive flag */  
            ntc->pduSp->m.setup.chanId.prefExc.val = PE_EXCLSVE;
            ntc->exclFlg = TRUE;
 
            if (pcb->cfg.swtch == SW_ATT4E)
            {
               if ((ntc->pduSp->m.setup.netFac[0].eh.pres &&  
                    ntc->pduSp->m.setup.netFac[0].facCode.pres &&  
                    ((ntc->pduSp->m.setup.netFac[0].facCode.val == SDN) ||
                     (ntc->pduSp->m.setup.netFac[0].facCode.val == ACCUNET)) &&
                    (ntc->callType == MF_PKT) && ntc->dChanAs))
                  ntc->callType |= MF_UTU;   
            }
            else
               if ((ntc->callType == MF_PKT) && ntc->dChanAs)
                  ntc->callType |= MF_UTU;
         
            /* FID 17524.0 */
            inIsFirstChnlId(ntc, &ntc->pduSp->m.setup.chanId, pcb->cfg.channelId);
            ntc->state = ST_OVLAPSND;
            inStartNtcTmr(TMR_T302, ntc, ntc->dCallCb);
            /* Send Setup Acknowledge */
            /* prepare Pdu header */
            inInitPduHdr((U8) PD_Q931, ntc->origin, pcb->cfg.callRefLen, 
                         ntc->dCallRef, (U8) M_SETUPACK, &pduHdr);

            MFINITPDU(&ntc->dCallCb->mfMsgCtl, ret, (U8) MI_SETUP, 
                      (U8) MI_SETUPACK, (ElmtHdr *) &ntc->pduSp->m.setup, 
                      (ElmtHdr *) &allPdus, (U8) PRSNT_NODEF, pcb->cfg.swtch, 
                      ntc->callType);

            if (!ntc->dstAdr.nmbDigits.len)
               if (pcb->cfg.swtch == SW_ATT5EB)
               {
                  /* set signal to dial tone on */
                  allPdus.m.setupAck.signal.eh.pres = PRSNT_NODEF;
                  INSETTOKVAL(allPdus.m.setupAck.signal.signal, SIG_DIAL);
               } 

            /* set signal to 0 */
            /* set progress indicator to 8 */
            if (pcb->cfg.swtch == SW_NTDMS100B)
            {
               allPdus.m.setupAck.signal.eh.pres = PRSNT_NODEF;
               INSETTOKVAL(allPdus.m.setupAck.signal.signal, SIG_DIAL);

               allPdus.m.setupAck.progInd.eh.pres = PRSNT_NODEF;
               INSETTOKVAL(allPdus.m.setupAck.progInd.location, LOC_PUBNETLU);
               INSETTOKVAL(allPdus.m.setupAck.progInd.codeStand0, CSTD_CCITT);
               INSETTOKVAL(allPdus.m.setupAck.progInd.progDesc, PD_IBAVAIL);
            }

            inGenPdu(dCb, &pduHdr, &allPdus, pcb->cfg.swtch,
                     ntc->callType);
            RETVALUE(ROK);
#else
            INSETTOKVAL(ntc->causeDgn.causeVal, CCNORTTODEST);
            if (pcb->cfg.swtch == SW_NTDMS100B)
               inGenPrgMsg(ntc, &ntc->causeDgn, dCb);
            else
               inGenRelLw(ntc, &ntc->causeDgn, dCb);
            RETVALUE(ROK);
#endif
         }
         else
         {
            ntc->cmpltAdr = TRUE;

            if (pcb->cfg.keyPad)    /* keyPad conveys call info */ 
            {
               inAscAdrToBcd(&ntc->pduSp->m.setup.keyFac.keyInfo.val[0], 
                             &ntc->dstAdr.nmbDigits.val[0]);
               if ((rout = inRoutSearch(&ntc->dstAdr.nmbDigits.val[0], 
                                        ntc->dstAdr.nmbDigits.len = 
                                        ntc->pduSp->m.setup.keyFac.keyInfo.len,
                                        &pcb->cfg.wcMask[0], TSAP)) == NULLP)
               {
                  INSETTOKVAL(ntc->causeDgn.causeVal, CCNORTTOTSFNET);
                  if (pcb->cfg.swtch == SW_NTDMS100B)
                     inGenPrgMsg(ntc, &ntc->causeDgn, dCb);
                  else
                     inGenRelLw(ntc, &ntc->causeDgn, dCb);
                  RETVALUE(ROK);
               }
               if (!(tCb = inGetSapPtr(rout->sapId)))
               {
#if (ERRCLASS & ERRCLS_DEBUG)
                  INLOGERROR(ERRCLS_DEBUG, EIN375, (ErrVal) rout->sapId,
                             "inNetE05S00() failed, unable to access TSAP.");
#endif
                  RETVALUE(RFAILED);
               }
            }
            else
            {
               if (pcb->cfg.useSubAdr)
               {
                  if (!(inGetDstSubAdr(ntc->dCallCb, 
                                       &ntc->pduSp->m.setup.cdPtySad, &ntc->dstAdr)))
                  {

                     INSETTOKVAL(ntc->causeDgn.causeVal, CCNORTTODEST);
                     if (pcb->cfg.swtch == SW_NTDMS100B)
                        inGenPrgMsg(ntc, &ntc->causeDgn, dCb);
                     else
                        inGenRelLw(ntc, &ntc->causeDgn, dCb);
                     RETVALUE(ROK);
                  }
               }
               else           /* not a subaddress or keyPad */
                  if (!(inGetDstNmb(ntc->dCallCb,&ntc->pduSp->m.setup.cdPtyNmb,
                                    &ntc->dstAdr)))
                  {
                     INSETTOKVAL(ntc->causeDgn.causeVal, CCNORTTODEST);      
                     if (pcb->cfg.swtch == SW_NTDMS100B)
                        inGenPrgMsg(ntc, &ntc->causeDgn, dCb);
                     else
                        inGenRelLw(ntc, &ntc->causeDgn, dCb);
                     RETVALUE(ROK);
                  }
               if ((rout = inRoutSearch(ntc->dstAdr.nmbDigits.val, 
                                        ntc->dstAdr.nmbDigits.len, 
                                        &pcb->cfg.wcMask[0], 
                                        TSAP)) == NULLP)
               {

                  INSETTOKVAL(ntc->causeDgn.causeVal, CCNORTTODEST); 
                  if (pcb->cfg.swtch == SW_NTDMS100B)
                     inGenPrgMsg(ntc, &ntc->causeDgn, dCb);
                  else
                     inGenRelLw(ntc, &ntc->causeDgn, dCb);
                  RETVALUE(ROK);
               }
               if (!(tCb = inGetSapPtr(rout->sapId)))
               {
#if (ERRCLASS & ERRCLS_DEBUG)
                  INLOGERROR(ERRCLS_DEBUG, EIN376, (ErrVal) rout->sapId,
                             "inNetE05S00() failed, unable to access TSAP.");
#endif
                  RETVALUE(RFAILED);
               }
            }
         }
   
      if (tCb->sapCfg.swtch != pcb->cfg.swtch)
      {
         INSETTOKVAL(ntc->causeDgn.causeVal, CCINCOMPDEST);
         if (pcb->cfg.swtch == SW_NTDMS100B)
            inGenPrgMsg(ntc, &ntc->causeDgn, dCb);
         else
            inGenRelLw(ntc, &ntc->causeDgn, dCb);
         RETVALUE(ROK);
      }

      if (inGetInstId(tCb, &ref) == RFAILED)
      {
         INSETTOKVAL(ntc->causeDgn.causeVal, CCRESCUNAVAIL);
         if (pcb->cfg.swtch == SW_NTDMS100B)
            inGenPrgMsg(ntc, &ntc->causeDgn, dCb);
         else
            inGenRelLw(ntc, &ntc->causeDgn, dCb);
         RETVALUE(ROK);
      }
#else
      INSETTOKVAL(ntc->causeDgn.causeVal, CCRESCUNAVAIL);
      inGenRelCLw(ntc, &ntc->causeDgn, dCb);
      RETVALUE(ROK);
#endif
   }

   /* check if UTU information is permitted */
   if (ntc->pduSp->m.setup.usrUsr.eh.pres)
      ntc->permUUI = TRUE;

   if (tCb->state == CBST1XFER)
   {
      ntc->tCallCb = tCb;
      ntc->tCallRef = ref;
      inInsCallRefEnt1(ntc->tCallCb, ntc);
      inGetBChan(ntc, &ntc->pduSp->m.setup.chanId, &intId,
                 &ntc->pduSp->m.setup.bearCap[0], &ntc->causeDgn);
      if (ntc->causeDgn.causeVal.pres)
      {
         if (ntc->pduSp->m.setup.bearCap[1].eh.pres)
            inGetBChan(ntc, &ntc->pduSp->m.setup.chanId, &intId,
                       &ntc->pduSp->m.setup.bearCap[1], &ntc->causeDgn);
         else
         {
            inGenRelCLw(ntc, &ntc->causeDgn, dCb);
            RETVALUE(ROK);
         }
         if (ntc->causeDgn.causeVal.pres)
         {
            inGenRelCLw(ntc, &ntc->causeDgn, dCb);
            RETVALUE(ROK);
         }
      }
      ntc->causeDgn.eh.pres = PRSNT_NODEF;
      /* save prefered/exclusive flag */  
      ntc->exclFlg = ntc->pduSp->m.setup.chanId.prefExc.val;
      
      if ((ntc->pduSp->m.setup.netFac[0].eh.pres &&  
           ntc->pduSp->m.setup.netFac[0].facCode.pres &&  
           ((ntc->pduSp->m.setup.netFac[0].facCode.val == SDN) ||
            (ntc->pduSp->m.setup.netFac[0].facCode.val == ACCUNET)) &&
           (ntc->callType == MF_PKT) && ntc->dChanAs))
         ntc->callType |= MF_UTU;   
      else
         if ((ntc->callType == MF_PKT) && ntc->dChanAs)
            ntc->callType |= MF_UTU;
      
      switch (pcb->cfg.swtch)
      {
         case SW_NI2:          /* switch - NI2 */
            /* gdixit-01/23/2002-Bug#4754 */
            /* See TR-NWT-001268 Issue 1, Dec'91, Rev. 4, Mar'98 
               Section 3.2.1::Progress Indicator */
            if ( (ntc->pduSp->m.setup.progInd.progDesc.val == PD_NOTETEISDN) ||
                 (ntc->pduSp->m.setup.progInd.progDesc.val == PD_ORGNOTISDN) )
            {
               if ( (ntc->pduSp->m.setup.progInd.location.val == LOC_PRIVNETLU) ||
                    (ntc->pduSp->m.setup.progInd.location.val == LOC_PRIVNETRU) )
               {
                  ntc->pduSp->m.setup.progInd.location.val =  LOC_PRIVNETRU;
               }
               else
               {
                  ntc->pduSp->m.setup.progInd.location.val =  LOC_USER;
               }
               /* if bearer cap. is circuit-mode/speech, change it to
                  circuit-mode/3.1-kHz Audio */
               if ( (ntc->pduSp->m.setup.bearCap[0].infoTranCap.val == AM_ITC_SPEECH) &&
                    (ntc->pduSp->m.setup.bearCap[0].tranMode.val == TM_CIRCUIT) )
               {
                  ntc->pduSp->m.setup.bearCap[0].infoTranCap.val = AM_ITC_A31KHZ;
               }
            }

         case SW_NTDMS100B:     /* switch - nt dms100 */
         case SW_NTDMS100P:     /* switch - nt dms100 */
         case SW_NTDMS250:      /* switch - nt dms250 */
         case SW_CCITT:
#ifdef ETSI                    /* TELICA-dsolanki-INTMGCDEV */
         case SW_ETSI:         /* switch - etsi */
#endif                         /* TELICA-dsolanki-INTMGCDEV */
            ntc->state = ST_CALLINIT;
            break;
         case SW_ATT4E:
         case SW_ATT5EB:
         case SW_ATT5EP:
            if (pcb->cfg.tCbCfg)
               ntc->state = ST_CALLINIT;
            else
            {
               ntc->state = ST_OUTPROC;
               inUpdSts(&pcb->sts.callProcTx, pcb);
               ntc->exclFlg = TRUE;
            
               /* prepare Pdu header */
               inInitPduHdr((U8) PD_Q931, ntc->origin, 
                            pcb->cfg.callRefLen, ntc->dCallRef, 
                            (U8) M_CALLPROC,
                            &pduHdr);
            
               MFINITPDU(&ntc->dCallCb->mfMsgCtl, ret, (U8) 0, 
                         (U8) MI_CALLPROC, NULLP, (ElmtHdr *) &allPdus, 
                         (U8) PRSNT_DEF,  pcb->cfg.swtch, ntc->callType);
            
               MFINITELMT(&ntc->dCallCb->mfMsgCtl, ret, 
                          (ElmtHdr *) &ntc->pduSp->m.setup.chanId, 
                          (ElmtHdr *) &allPdus.m.callProc.chanId, &meChanId, 
                          (U8) PRSNT_NODEF, pcb->cfg.swtch, ntc->callType);
               /* force exclusive flag */  
               allPdus.m.callProc.chanId.prefExc.val = PE_EXCLSVE;
               inGenPdu(dCb, &pduHdr, &allPdus, pcb->cfg.swtch,
                        ntc->callType);
            }
            break;
         default:
#if (ERRCLASS & ERRCLS_DEBUG)
            INLOGERROR(ERRCLS_DEBUG, EIN377, (ErrVal) pcb->cfg.swtch,
                       "inNetE05S00() failed, invalid switch configured.");
#endif
            inNetMSGINV(ntc, dCb);
         
            break;
      }

      /* Bug 87471: Implement dependency check between tokens. */
      inChkDepBtwTkns(ntc, pcb);

      /* Send Setup Indication to Upper */
      MFINITSDU(&tCb->mfMsgCtl, ret, (U8) MI_SETUP, (U8) PM_CONREQ, 
                (ElmtHdr *) &ntc->pduSp->m.setup, (ElmtHdr *) &ev, 
                (U8) PRSNT_NODEF, pcb->cfg.swtch, ntc->callType);

      /* FID 16779.0 Replace NPPL with digit map */
      if ((pcb->cfg.swtch == SW_ETSI) && (pcb->cfg.overlap) &&
          (pcb->cfg.digitMapName.length > 0))
      {
         if ((ntc->pduSp->m.setup.cdPtyNmb.eh.pres)
            && (ntc->pduSp->m.setup.cdPtyNmb.nmbDigits.len > 0))
         {
            int ret_val;
            U8  typeNmb0;

            ret_val = cmStrncmp(ntc->pduSp->m.setup.cdPtyNmb.nmbDigits.val, "00", 2);     
            typeNmb0 = ntc->pduSp->m.setup.cdPtyNmb.typeNmb0.val;

            if ((pcb->cfg.numPL == NPPL) && ((typeNmb0 == TON_INT) ||
                (ret_val == 0 && typeNmb0 == TON_UNK)))
            {
               /* International call don't need to
                * make further check on information message
                */
               ntc->check_info = FALSE;
               INDBGP(DBGMASK_MI, "international call\n");
            }
            else
            {
               /* Search the digit map table to get the corresponding maxDigits
                * if match is found.
                */
#if 0 /* Old code */
               ntc->numPL_max = inDigMapSrch (ntc->pduSp->m.setup.cdPtyNmb.nmbDigits.val, 
                                              ntc->pduSp->m.setup.cdPtyNmb.nmbDigits.len);
#else /* Start of code change */
               if ((ret = cmHashListFind(&inGlobalCb.digitMapHl, pcb->cfg.digitMapName.strg,
                            pcb->cfg.digitMapName.length, (U16)0, (PTR *) &pDigitMap)) == ROK)
               {
                  /* Create digit map candidate info */
                  inCreateDigitMapInfo(ntc, pDigitMap, typeNmb0);
                  ntc->lastTypeNmb = typeNmb0;

                  /* Search digit map candidiate lists */
                  inSearchDigitMap(ntc, pDigitMap, &ntc->pduSp->m.setup.cdPtyNmb.nmbDigits, typeNmb0);

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
         }
      }

      /* 2111 overlap */
      if ((pcb->cfg.swtch == SW_ETSI) && pcb->cfg.overlap)
      {
         if (!ntc->pduSp->m.setup.sndCmplt.eh.pres)
         {
            U8 len;
            len = ntc->pduSp->m.setup.cdPtyNmb.nmbDigits.len;

            if (ntc->pduSp->m.setup.cdPtyNmb.eh.pres && len > 0)
            {

               if (ntc->pduSp->m.setup.cdPtyNmb.nmbDigits.val[len - 1] != '#')
               {
                  if(!IN_GOT_MAXDIGITS(len, IN_GOTMAX(ntc, pcb)))
                  {
                     ntc->nmbCdPty = ntc->pduSp->m.setup.cdPtyNmb.nmbDigits.len;
                     IN_SET_WAITINFO(ntc->waitForMoreInfo);
                     ev.m.conEvnt.optElmt.waitForMoreInfo = ntc->waitForMoreInfo;
                     ntc->cdAdr.nmbDigits.len 
                            = ntc->pduSp->m.setup.cdPtyNmb.nmbDigits.len;

                     /* Save digits received in setup message in ntc */
                     cmMemcpy(ntc->cdAdr.nmbDigits.val,
                            ntc->pduSp->m.setup.cdPtyNmb.nmbDigits.val,
                            ntc->pduSp->m.setup.cdPtyNmb.nmbDigits.len);
                  }
               }
            }
            else
            {
               /* FID17555.0 Kavon */
               if (pcb->cfg.playDialTone)
               {
                  IN_SET_PLAYDIALTONE(ntc->waitForMoreInfo);
                  INDBGP(DBGMASK_LI,"Kavon inNetE05S00 dial tone: ntc->waitForMoreInfo = %d\n",
                                    ntc->waitForMoreInfo);
               }
               IN_SET_WAITINFO(ntc->waitForMoreInfo);
               ev.m.conEvnt.optElmt.waitForMoreInfo = ntc->waitForMoreInfo;
               INDBGP(DBGMASK_LI, "Kavon inNetE05S00 final: ntc->waitForMoreInfo = %d, ev.m.conEvnt.optElmt.waitForMoreInfo = %d\n",
                                  ntc->waitForMoreInfo, ev.m.conEvnt.optElmt.waitForMoreInfo);
            }
         }
      }

      if (pcb->cfg.cndSubsc)
      {
         if (ntc->pduSp->m.setup.cgPtyNmb.eh.pres)
         {
            if (ntc->pduSp->m.setup.cgPtyNmb.presInd0.pres)
            {
               INSETTOKVAL(ev.m.conEvnt.cgPtyNmb.screenInd, SI_USRNOTSCRN);
            }
            else
            {
               INSETTOKVAL(ev.m.conEvnt.cgPtyNmb.presInd0, PI_ALLOW);
               INSETTOKVAL(ev.m.conEvnt.cgPtyNmb.screenInd, SI_USRNOTSCRN);
            }

#ifdef IN_NW_NMB
            /* Insert the network number, TR-NWT-000444 */
            ev.m.conEvnt.nwNmb.eh.pres = PRSNT_NODEF;
            INSETTOKVAL(ev.m.conEvnt.nwNmb.nmbPlanId, pcb->cfg.sidNPlan);
            INSETTOKVAL(ev.m.conEvnt.nwNmb.typeNmb1, pcb->cfg.sidTon);
            INSETTOKARRAY(ev.m.conEvnt.nwNmb.nmbDigits, 
                          pcb->cfg.sid.length, pcb->cfg.sid.strg);
            INSETTOKVAL(ev.m.conEvnt.nwNmb.screenInd, SI_NET);         
            INSETTOKVAL(ev.m.conEvnt.nwNmb.presInd0, pcb->cfg.sidPresInd);
#endif /* IN_NW_NMB */

         }
         else   /* calling party number is not supplied */
         {
            ev.m.conEvnt.cgPtyNmb.eh.pres = PRSNT_NODEF;
            
            INSETTOKVAL(ev.m.conEvnt.cgPtyNmb.nmbPlanId, pcb->cfg.sidNPlan);
            INSETTOKVAL(ev.m.conEvnt.cgPtyNmb.typeNmb1, pcb->cfg.sidTon);
            INSETTOKARRAY(ev.m.conEvnt.cgPtyNmb.nmbDigits, 
                          pcb->cfg.sid.length,
                          pcb->cfg.sid.strg);
            INSETTOKVAL(ev.m.conEvnt.cgPtyNmb.screenInd,SI_NET);         
            INSETTOKVAL(ev.m.conEvnt.cgPtyNmb.presInd0, pcb->cfg.sidPresInd);
         }
      }

      /* Fill CRI Information in ConEvnt */
      INDBGP(DBGMASK_LI,"inNetE05S00: ****** tCallRef = %ld - dCallRef = %d *******\n",
                        ntc->tCallRef, ntc->dCallRef);

      /* BUG:37899-mqin */
      if (pcb->cfg.swtch == SW_NI2 &&
          ntc->pduSp->m.setup.facilityStr.eh.pres &&
          ntc->pduSp->m.setup.facilityStr.facilityStr.pres)
      {
         /* check facility string to see if we need to hold the SETUP */
         ret = inChkFacilStr(&ntc->pduSp->m.setup.facilityStr.facilityStr);
 
         if (ret == ROK)
         {
            /* This is informationFollowing operation. Hold the SETUP and
             * wait for the Facility message
             */
            SGetSBuf(DFLT_REGION, DFLT_POOL, (Data **)&(ntc->conEvnt),
                           (Size) sizeof(ConEvnt));
            cmMemcpy((U8 *)(ntc->conEvnt), (U8 *)&ev.m.conEvnt,
                     sizeof(ConEvnt));
            inStartNtcTmr(TMR_THOLDSETUP, ntc, ntc->dCallCb);
            RETVALUE(ROK);
         } 
      }
      
      inFillCriInfo(ntc->dCallRef, &ev.m.conEvnt.criList, &inCri, string);

      /* FID 14984.10 */
      if((ntc->callTrcId.count >0) && (ntc->callTrcId.count < TRC_CALL_MAX_DUP_ENTRY + 1) && (ntc->callTrcId.idList))
      {
          ev.m.conEvnt.callTrcId.idList =  ntc->callTrcId.idList;
          ev.m.conEvnt.callTrcId.count  =  ntc->callTrcId.count;

          /* FID 17167.0 */
          ev.m.conEvnt.logMask = ntc->logMask;
          INDBGP(DBGMASK_LI,"inNetE05S00: logMask to GCC =%ld\n", ev.m.conEvnt.logMask);
      }
      else /* Bug 92164 make sure trace Id is empty if not present */
      {
          cmMemset((U8 *)&(ev.m.conEvnt.callTrcId), 0, sizeof(CallTrcId));
      }

      InUiIntConInd(&tCb->pst, tCb->suId, ntc->suInstId,ntc->tCallRef,
                    &ev.m.conEvnt, pcb->suId, dCb->ces);
   }
   else
   {
      INSETTOKVAL(ntc->causeDgn.causeVal,  CCDESTOUTORD);
      if (pcb->cfg.swtch == SW_NTDMS100B)
         inGenPrgMsg(ntc, &ntc->causeDgn, dCb);
      else
         inGenRelLw(ntc, &ntc->causeDgn, dCb);
      RETVALUE(ROK);
   }
   RETVALUE(ROK);
} /* end of inNetE05S00 */







  
/*
 *
 *       Fun:   inNetE05S24
 *
 *       Desc:  Input: Setup
 *              State: permanent signal
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
         File:  qn_bdy5.c
 *
*/

#ifdef ANSI
PUBLIC S16 inNetE05S24
(
InNtc *ntc,
InCb *dCb
)
#else
PUBLIC S16 inNetE05S24(ntc, dCb)
InNtc *ntc;
InCb *dCb;
#endif
{


   TRC2(inNetE05S24);

      inIse(ntc, dCb);
   RETVALUE(ROK);
} /* end of inNetE05S24 */






  
/*
 *
 *       Fun:   inNetE06S06
 *
 *       Desc:  Input: Setup Ack
 *              State: call present
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
         File:  qn_bdy5.c
 *
*/

#ifdef ANSI
PUBLIC S16 inNetE06S06
(
InNtc *ntc,
InCb  *dCb
)
#else
PUBLIC S16 inNetE06S06(ntc, dCb)
InNtc *ntc;
InCb  *dCb;
#endif
{
   InCb *tCb;
   InPCB *pcb;
   AllSdus ev;
   U8 tmrNum;
   S16 ret;

   TRC2(inNetE06S06);

   tCb = ntc->tCallCb;

   /* get physical link control block */
   if ((pcb = *(pcbLstPtr + dCb->suId)) == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      INLOGERROR(ERRCLS_DEBUG, EIN379, (ErrVal) dCb->rSuId,
                 "inNetE06S06() failed, unable to access DLSAP.");
#endif
      RETVALUE(RFAILED);
   }

   switch (pcb->cfg.swtch)
   {
      case SW_ATT4E:
      case SW_ATT5EB:
      case SW_ATT5EP:
         INSETTOKVAL(ntc->causeDgn.causeVal, CCINVMSG);
         inGenRelUpLw(ntc, &ntc->causeDgn, dCb);
         RETVALUE(ROK);

      case SW_NTDMS100P:     /* switch - nt dms100 */
         /* 
          * send status message with cause 97 
          */
         INSETTOKVAL(ntc->causeDgn.causeVal, CCNOMSGTYP);
         inGenStaMsg(ntc, &ntc->causeDgn, dCb);
         RETVALUE(ROK);

      case SW_NTDMS100B:     /* switch - nt dms100 */

         INSETTOKVAL(ntc->causeDgn.causeVal, CCINVMSG);
         inGenRelUpLw(ntc, &ntc->causeDgn, dCb);
         RETVALUE(ROK);

      case SW_CCITT:
#ifdef ETSI                   /* TELICA-dsolanki-INTMGCDEV */
      case SW_ETSI:          /* switch - etsi */
#endif                       /* TELICA-dsolanki-INTMGCDEV */
         break;

      default:
#if (ERRCLASS & ERRCLS_DEBUG)
         INLOGERROR(ERRCLS_DEBUG, EIN380, (ErrVal) pcb->cfg.swtch,
                    "inNetE06S06() failed, unknown switch.");
#endif
         RETVALUE(RFAILED);
   }

   inUpdSts(&pcb->sts.setupAckRx, pcb);

   /* Stop T303 */
   /* PTMPT019 */

   if (!ntc->pduSp->m.setupAck.chanId.eh.pres)
      inInsrtChnlId(ntc, &ntc->pduSp->m.setupAck.chanId, ntc->ctldPcb);

#ifdef ISDN_PTMPT
   /*
    * The function returns RFAILED if we need to return from here.
    */

   if ((ret = inHandleMptChannelCheck(ntc, dCb, pcb, 
                                      &ntc->pduSp->m.setupAck.chanId,
                                      MI_SETUPACK)) != ROK)
   {
      RETVALUE(RFAILED);
   }
#endif /* ISDN_PTMPT */

   STOP_NTC_TIMER(TMR_T303);

   if (ntc->pduSp->m.setupAck.chanId.intIdentPres.pres)
      if ((ntc->pduSp->m.setupAck.chanId.intIdentPres.val == IIP_EXPLICIT)
          && (!pcb->cfg.nfasInt))
         if (ntc->pduSp->m.setupAck.chanId.intIdent.val != 
             ntc->ctldPcb->cfg.intId)
         {
            INSETTOKVAL(ntc->causeDgn.causeVal, CCCHANUNACPT);
            inGenRelUpLw(ntc, &ntc->causeDgn, dCb);
            RETVALUE(ROK);
         }
   
   if ((ret = inNegChan(ntc, &ntc->pduSp->m.setupAck.chanId, 
                        &ntc->causeDgn)) != ROK)
   {
      inGenStaMsg(ntc, &ntc->causeDgn, dCb);
      RETVALUE(ROK);
   }


   if (ntc->causeDgn.causeVal.pres)
   {
      inGenRelUpLw(ntc, &ntc->causeDgn, dCb);
      RETVALUE(ROK);
   }
   ntc->causeDgn.eh.pres = PRSNT_NODEF;
   INSETTOKVAL(ntc->pduSp->m.setupAck.chanId.prefExc, PE_EXCLSVE);

#ifdef ISDN_PTMPT 
   if ((pcb->cfg.intType == NETWORK)
       && ((pcb->cfg.intCfg == INTCFG_MULTI)
           && (ntc->setupBCast ==  TRUE)))
   {
      inIntPrimSetupAckReq(ntc, dCb);
      
   } /* end of multipoint check.. */
#endif /* ISDN_PTMPT */


   MFINITSDU(&tCb->mfMsgCtl, ret, (U8) MI_SETUPACK, (U8) PM_CNSTREQ, 
             (ElmtHdr *) &ntc->pduSp->m.setupAck, (ElmtHdr *) &ev, 
             (U8) PRSNT_NODEF, tCb->sapCfg.swtch, ntc->callType);

   /* Start T304 */
   inStartNtcTmr(TMR_T304, ntc, ntc->dCallCb);

   /* Change state to Overlap Receiving (25) */
   ntc->state = ST_OVLAPRCV;

   /* Send Status Indication to Upper */
   InUiIntCnStInd(&tCb->pst, tCb->suId, ntc->suInstId, ntc->tCallRef, 
                  &ev.m.cnStEvnt, MI_SETUPACK, pcb->suId, dCb->ces);
   RETVALUE(ROK);
} /* end of inNetE06S06 */





  
/*
 *
 *       Fun:   inNetE07S00
 *
 *       Desc:  Input: Resume
 *              State: null
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
         File:  qn_bdy5.c
 *
*/

#ifdef ANSI
PUBLIC S16 inNetE07S00
(
InNtc *ntc,
InCb *dCb 
)
#else
PUBLIC S16 inNetE07S00(ntc, dCb)
InNtc *ntc;
InCb *dCb;
#endif
{
   InCb *tCb = NULL;
   InPCB *pcb = NULL;
   AllSdus ev;
   PduHdr pduHdr;
   AllPdus m;
   S16 ret = 0;

   TRC2(inNetE07S00);
   UNUSED(ret);
   UNUSED(m);
   UNUSED(pduHdr);
   UNUSED(ev);
   UNUSED(pcb);
   UNUSED(tCb);

#ifdef ISDN_SUSPEND
   tCb = ntc->tCallCb;
   /* get physical link control block */

   if ((pcb = *(pcbLstPtr + dCb->suId)) == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      INLOGERROR(ERRCLS_DEBUG, EIN381, (ErrVal) dCb->rSuId,
                 "inNetE07S00() failed, unable to access DLSAP.");
#endif
      RETVALUE(RFAILED);
   }

   if (!ntc->susFlg)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      INLOGERROR(ERRCLS_DEBUG, EIN382, (ErrVal) ntc->dCallCb->rSuId,
                 "inNetE07S00() failed, invalid state..");
#endif
      RETVALUE(RFAILED);
   }

   inUpdSts(&pcb->sts.rsmRx, pcb);

   MFINITSDU(&tCb->mfMsgCtl, ret, (U8) MI_RSM, (U8) PM_RMRTREQ, 
             (ElmtHdr *) &ntc->pduSp->m.rsm, (ElmtHdr *) &ev, (U8) PRSNT_NODEF,
             tCb->sapCfg.swtch, ntc->callType);

   /* Change state to Resume Request (17) */
   ntc->state = ST_RSMRQ;

   /* Send Resume Indication to Upper */
   InUiIntRmRtInd(&tCb->pst, tCb->suId, (InInstId)ntc->suInstId, 
                  (InInstId)ntc->tCallRef, &ev.m.rmRtEvnt, N_RESUME);

#endif
   RETVALUE(ROK);
} /* end of inNetE07S00 */





  
/*
 *
 *       Fun:   inNetE10S10
 *
 *       Desc:  Input: Suspend
 *              State: active
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
         File:  qn_bdy5.c
 *
*/

#ifdef ANSI
PUBLIC S16 inNetE10S10
(
InNtc *ntc,
InCb *dCb 
)
#else
PUBLIC S16 inNetE10S10(ntc, dCb)
InNtc *ntc;
InCb *dCb;
#endif
{
   InCb *tCb = NULL;
   InPCB *pcb = NULL;
   AllSdus ev;
   PduHdr pduHdr;
   S16 ret = 0;
   Action action = N_SUSPEND;
   TknStrS *id = NULL;
   CalId *entry = NULL;
   U8 i = 0;
#ifdef ISDN_SUSPEND
   U8 causVal = 0;
#endif

   TRC2(inNetE10S10);
   UNUSED(ret);
   UNUSED(pduHdr);
   UNUSED(ev);
   UNUSED(pcb);
   UNUSED(tCb);
   UNUSED(i);
   UNUSED(entry);
   UNUSED(id);
   UNUSED(action);

#ifdef ISDN_SUSPEND
   causVal=0;
   tCb = ntc->tCallCb;
   /* get physical link control block */
   if ((pcb = *(pcbLstPtr + dCb->suId)) == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      INLOGERROR(ERRCLS_DEBUG, EIN383, (ErrVal) dCb->rSuId,
                 "inNetE10S10() failed, unable to access DLSAP.");
#endif
      RETVALUE(RFAILED);
   }

   inUpdSts(&pcb->sts.suspRx, pcb);

#if 1 /* Esc 18680 for Poland Exatel, SUS should be rejected with cv=29 facility rejected*/
   if ((pcb->cfg.numPL == NPPL) && (ntc->state == ST_ACTVE))
   {
      INDBGP(DBGMASK_LI,
             "inNetE10S10(): for Poland, SUS message should be rejected if the call is active!\n");
      inUpdSts(&pcb->sts.suspRejTx, pcb);

      /* prepare Pdu header */
      inInitPduHdr((U8) PD_Q931, ntc->origin, pcb->cfg.callRefLen,
                   ntc->dCallRef, (U8) M_SUSPREJ, &pduHdr);

      MFINITPDU(&ntc->dCallCb->mfMsgCtl, ret, (U8) 0, (U8) MI_SUSPREJ, NULLP,
                (ElmtHdr *) ntc->pduSp, (U8) NOTPRSNT, pcb->cfg.swtch,
                ntc->callType);
      INSETTOKHDR(ntc->pduSp->m.suspRej.causeDgn[0]);
      INSETTOKVAL(ntc->pduSp->m.suspRej.causeDgn[0].causeVal,  CCFACREJ);

      INSETTOKVAL(ntc->causeDgn.causeVal, CCFACREJ);
      pcb->sts.lstTxCause = ntc->causeDgn.causeVal.val;
      pcb->sts.lstTxDgn = 0;
      MFINITELMT(&ntc->dCallCb->mfMsgCtl, ret, (ElmtHdr *) &ntc->causeDgn,
                 (ElmtHdr *) &ntc->pduSp->m.suspRej.causeDgn[0], &meCauseDgn,
                 (U8) PRSNT_NODEF, pcb->cfg.swtch, ntc->callType);
      inGenPdu(dCb, &pduHdr, ntc->pduSp, pcb->cfg.swtch, ntc->callType);
      RETVALUE(ROK);
   }
#endif /* Esc 18680 */

   if (ntc->pduSp->m.susp.callId.eh.pres && 
       ntc->pduSp->m.susp.callId.callIdent.pres &&
       ntc->pduSp->m.susp.callId.callIdent.len)
      id = &ntc->pduSp->m.susp.callId.callIdent;
   else
      id = &pcb->cfg.callId;

   if ((entry = inCallIdSearch(ntc->dCallCb, id, &causVal)))
   {
      inUpdSts(&pcb->sts.suspRejTx, pcb);

      /* prepare Pdu header */
      inInitPduHdr((U8) PD_Q931, ntc->origin, pcb->cfg.callRefLen,
                   ntc->dCallRef, (U8) M_SUSPREJ, &pduHdr);
 
      MFINITPDU(&ntc->dCallCb->mfMsgCtl, ret, (U8) 0, (U8) MI_SUSPREJ, NULLP,
                (ElmtHdr *) ntc->pduSp, (U8) NOTPRSNT, pcb->cfg.swtch,
                ntc->callType);
      INSETTOKHDR(ntc->pduSp->m.suspRej.causeDgn[0]);
      INSETTOKVAL(ntc->pduSp->m.suspRej.causeDgn[0].causeVal, causVal);
 
      INSETTOKVAL(ntc->causeDgn.causeVal, causVal);
      pcb->sts.lstTxCause = ntc->causeDgn.causeVal.val;
      pcb->sts.lstTxDgn = 0;
      MFINITELMT(&ntc->dCallCb->mfMsgCtl, ret, (ElmtHdr *) &ntc->causeDgn,
                 (ElmtHdr *) &ntc->pduSp->m.suspRej.causeDgn[0], &meCauseDgn,
                 (U8) PRSNT_NODEF, pcb->cfg.swtch, ntc->callType);
      inGenPdu(dCb, &pduHdr, ntc->pduSp, pcb->cfg.swtch, ntc->callType);
      RETVALUE(ROK);
   }

   ntc->susFlg = TRUE;

   INSETTOKARRAY(ntc->callId, id->len, id->val);

   if ((ret = inInsCallId(ntc->dCallCb, id, ntc)) != ROK)
   {
#if (ERRCLASS & ERRCLS_DEBUG) /* addition : in014.310 */
      INLOGERROR(ERRCLS_DEBUG, EIN384, (ErrVal) ret,
                 "inNetE10S10() failed, unable to insert entry.");
#endif /* addition : in014.310 */
      INSETTOKVAL(ntc->causeDgn.causeVal, CCPROTERR);
      inGenStaCfm(ntc, &ntc->causeDgn);
      RETVALUE(RFAILED);
   }

   /* Change state to Suspend Request (15) */
   ntc->state = ST_SUSRQ;

   MFINITSDU(&tCb->mfMsgCtl, ret, (U8) MI_SUSPEND, (U8) PM_SSHLREQ, 
             (ElmtHdr *) &ntc->pduSp->m.susp, (ElmtHdr *) &ev, (U8)PRSNT_NODEF,
             tCb->sapCfg.swtch, ntc->callType);
   action = N_SUSPEND;
   /* Send Suspend Indication to Upper */

   InUiIntSsHlInd(&tCb->pst, tCb->suId, (InInstId)ntc->suInstId, 
                  (InInstId)ntc->tCallRef, &ev.m.ssHlEvnt, action);
#endif
   RETVALUE(ROK);
} /* end of inNetE10S10 */






  
/*
 *
 *       Fun:   inNetE14S01
 *
 *       Desc:  Input: Disconnect
 *              State: call initiated
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
         File:  qn_bdy5.c
 *
*/

#ifdef ANSI
PUBLIC S16 inNetE14S01
(
InNtc *ntc,
InCb *dCb 
)
#else
PUBLIC S16 inNetE14S01(ntc, dCb)
InNtc *ntc;
InCb *dCb;
#endif
{
   InCb *tCb;
   InPCB *pcb;
   AllSdus ev;
   U8 tmrNum;
   S16 ret;

   TRC2(inNetE14S01);

   UNUSED(dCb);

   tCb = ntc->tCallCb;
   /* get physical link control block */

   if ((pcb = *(pcbLstPtr + ntc->dCallCb->suId)) == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      INLOGERROR(ERRCLS_DEBUG, EIN385, (ErrVal) ntc->dCallCb->rSuId,
                 "inNetE14S01() failed, unable to access DLSAP.");
#endif
      RETVALUE(RFAILED);
   }
   if (ntc->pduSp->m.disc.causeDgn[0].causeVal.pres)
      pcb->sts.lstRxCause = ntc->pduSp->m.disc.causeDgn[0].causeVal.val;

   if (ntc->pduSp->m.disc.causeDgn[0].dgnVal.pres)
      pcb->sts.lstRxDgn = ntc->pduSp->m.disc.causeDgn[0].dgnVal.val[0];
  
   /* Bug 45852: copy causeDgn from disconnect message to ntc */ 
   if (ntc->pduSp->m.disc.causeDgn[0].eh.pres)
   {
      ntc->causeDgn.eh.pres = PRSNT_NODEF;
      ntc->causeDgn.location.pres = ntc->pduSp->m.disc.causeDgn[0].location.pres;
      ntc->causeDgn.location.val = ntc->pduSp->m.disc.causeDgn[0].location.val;
      ntc->causeDgn.recommend.pres =ntc->pduSp->m.disc.causeDgn[0].recommend.pres;
      ntc->causeDgn.recommend.val = ntc->pduSp->m.disc.causeDgn[0].recommend.val;
      ntc->causeDgn.causeVal.pres = ntc->pduSp->m.disc.causeDgn[0].causeVal.pres;
      ntc->causeDgn.causeVal.val = ntc->pduSp->m.disc.causeDgn[0].causeVal.val;
      ntc->causeDgn.codeStand3.pres =
                     ntc->pduSp->m.disc.causeDgn[0].codeStand3.pres;
      ntc->causeDgn.codeStand3.val =
                        ntc->pduSp->m.disc.causeDgn[0].codeStand3.val;
      ntc->causeDgn.dgnVal.pres = ntc->pduSp->m.disc.causeDgn[0].dgnVal.pres; 
      if (ntc->causeDgn.dgnVal.pres)
      {
         ntc->causeDgn.dgnVal.len = ntc->pduSp->m.disc.causeDgn[0].dgnVal.len;
         cmMemcpy((U8 *)ntc->causeDgn.dgnVal.val, 
                    (U8 *)ntc->pduSp->m.disc.causeDgn[0].dgnVal.val, ntc->causeDgn.dgnVal.len);
      }
   }

   /* 
    * Stop All Timers 
    */
   STOP_NTC_TIMERS;

   ntc->tmrCnt = 0;

   /* 
    * Change state to Disconnect Request (11) 
    */
   ntc->state = ST_DISRQ;

   inUpdSts(&pcb->sts.discRx, pcb);
   pcb->sts.discs++;

   MFINITSDU(&tCb->mfMsgCtl, ret, (U8) MI_DISC, (U8) PM_DISCREQ, 
             (ElmtHdr *) &ntc->pduSp->m.disc, (ElmtHdr *) &ev.m.discEvnt, 
             (U8) PRSNT_NODEF, tCb->sapCfg.swtch, ntc->callType);

   /* 26758 */
   ntc->relLoc = CC_REL_NRML_REM;
   /* 
    * Send Disconnect Indication to Upper 
    */
   InUiIntDiscInd(&tCb->pst, tCb->suId, (InInstId)ntc->suInstId, 
                  (InInstId)ntc->tCallRef, &ev.m.discEvnt);
   RETVALUE(ROK);
} /* end of inNetE14S01 */






  
/*
 *
 *       Fun:   inNetE14SND
 *
 *       Desc:  Input: Disconnect
 *              State: any state
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
         File:  qn_bdy5.c
 *
*/

#ifdef ANSI
PUBLIC S16 inNetE14SND
(
InNtc *ntc,
InCb *dCb 
)
#else
PUBLIC S16 inNetE14SND(ntc, dCb)
InNtc *ntc;
InCb *dCb;
#endif
{
   InCb *tCb;
   InPCB *pcb;
   AllSdus ev;
   U8 tmrNum;
   S16 ret;

   TRC2(inNetE14SND);

   tCb = ntc->tCallCb;
   /* get physical link control block */
   INDBGP(DBGMASK_LI,"inNetE14SND: Entered - ntc = %p, ntc->state = %d\n", ntc, ntc->state);
   if ((pcb = *(pcbLstPtr + ntc->dCallCb->suId)) == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      INLOGERROR(ERRCLS_DEBUG, EIN386, (ErrVal) ntc->dCallCb->rSuId,
                 "inNetE14SND() failed, unable to access DLSAP.");
#endif
      RETVALUE(RFAILED);
   }
   if (ntc->pduSp->m.disc.causeDgn[0].causeVal.pres)
      pcb->sts.lstRxCause = ntc->pduSp->m.disc.causeDgn[0].causeVal.val;
   if (ntc->pduSp->m.disc.causeDgn[0].dgnVal.pres)
      pcb->sts.lstRxDgn = ntc->pduSp->m.disc.causeDgn[0].dgnVal.val[0];


   if ((ntc->state == ST_ACTVE) && !(ntc->origin))
      inGenBillNm(ntc);

#ifdef ISDN_PTMPT
   /*
    * The macro issues a status message in point to multipoint case
    * and returns.
    */
   IN_PTMPT_ISSUE_STATUS(pcb, ntc, dCb);
#endif /* ISDN_PTMPT */

   if (ntc->state != ST_DISRQ)
   {
      /* 
       * Stop all Timers 
       */
      STOP_NTC_TIMERS;

      ntc->tmrCnt = 0;

      /* Bug 45852: copy causeDgn from disconnect message to ntc */
      if (ntc->pduSp->m.disc.causeDgn[0].eh.pres)
      {
         ntc->causeDgn.eh.pres = PRSNT_NODEF;
         ntc->causeDgn.location.pres = ntc->pduSp->m.disc.causeDgn[0].location.pres;
         ntc->causeDgn.location.val = ntc->pduSp->m.disc.causeDgn[0].location.val;
         ntc->causeDgn.recommend.pres =ntc->pduSp->m.disc.causeDgn[0].recommend.pres;
         ntc->causeDgn.recommend.val = ntc->pduSp->m.disc.causeDgn[0].recommend.val;
         ntc->causeDgn.causeVal.pres = ntc->pduSp->m.disc.causeDgn[0].causeVal.pres;
         ntc->causeDgn.causeVal.val = ntc->pduSp->m.disc.causeDgn[0].causeVal.val;
         ntc->causeDgn.codeStand3.pres =
    		     ntc->pduSp->m.disc.causeDgn[0].codeStand3.pres;
         ntc->causeDgn.codeStand3.val =
     	         	ntc->pduSp->m.disc.causeDgn[0].codeStand3.val;
         ntc->causeDgn.dgnVal.pres = ntc->pduSp->m.disc.causeDgn[0].dgnVal.pres;
         if (ntc->causeDgn.dgnVal.pres)
         {
            ntc->causeDgn.dgnVal.len = ntc->pduSp->m.disc.causeDgn[0].dgnVal.len;
            cmMemcpy((U8 *)ntc->causeDgn.dgnVal.val, 
                       (U8 *)ntc->pduSp->m.disc.causeDgn[0].dgnVal.val, ntc->causeDgn.dgnVal.len);
         }
      }

      /* 
       * Change state to Disconnect Request (11) 
       */
      /* Bukucu - Bug-12959 decrement it only if it's greater than 0.
       * even if we count this counter correctly, we may have a case
       * where it goes below zero, after a failover
      */

      if ((ntc->state == ST_ACTVE) && (pcb->sts.aveCalls > 0))
         pcb->sts.aveCalls--;

      ntc->state = ST_DISRQ;

      inUpdSts(&pcb->sts.discRx, pcb);
      pcb->sts.discs++;

      MFINITSDU(&tCb->mfMsgCtl, ret, (U8) MI_DISC, (U8)PM_DISCREQ, 
                (ElmtHdr *) &ntc->pduSp->m.disc, (ElmtHdr *)&ev.m.discEvnt,
                (U8) PRSNT_NODEF, tCb->sapCfg.swtch, ntc->callType);

      /* 26758 */
      ntc->relLoc = CC_REL_NRML_REM;
      /* 
       * Send Disconnect Indication to Upper 
       */
      INDBGP(DBGMASK_LI,"inNetE14SND: calling InUiIntDiscInd, pst->selector = %d,"
               "suInstId = 0x%lx, tCallRef = %ld, discEvnt = %p\n",
               tCb->pst.selector, (InInstId)ntc->suInstId, 
               (InInstId)ntc->tCallRef, &ev.m.discEvnt);

      InUiIntDiscInd(&tCb->pst, tCb->suId, (InInstId)ntc->suInstId, 
                     (InInstId)ntc->tCallRef, &ev.m.discEvnt);
   }   
   RETVALUE(ROK);
} /* end of inNetE14SND */

  
/*
 *
 *       Fun:   inNetE14S09
 *
 *       Desc:  Input: Disconnect
 *              State: Incoming Call Proceeding
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
         File:  qn_bdy5.c
 *
*/

#ifdef ANSI
PUBLIC S16 inNetE14S09
(
InNtc *ntc,
InCb *dCb 
)
#else
PUBLIC S16 inNetE14S09(ntc, dCb)
InNtc *ntc;
InCb *dCb;
#endif
{
   InCb *tCb;
   InPCB *pcb;
   AllSdus ev;
   U8 tmrNum;
   S16 ret;


   TRC2(inNetE14S09);

   tCb = ntc->tCallCb;
   /* get physical link control block */

   if ((pcb = *(pcbLstPtr + dCb->suId)) == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      INLOGERROR(ERRCLS_DEBUG, EIN387, (ErrVal) dCb->rSuId,
                 "inNetE14S09() failed, unable to access DLSAP.");
#endif
      RETVALUE(RFAILED);
   }


   if (ntc->pduSp->m.disc.causeDgn[0].causeVal.pres)
      pcb->sts.lstRxCause = ntc->pduSp->m.disc.causeDgn[0].causeVal.val;
   if (ntc->pduSp->m.disc.causeDgn[0].dgnVal.pres)
      pcb->sts.lstRxDgn = ntc->pduSp->m.disc.causeDgn[0].dgnVal.val[0];

   /* Bug 45852: copy causeDgn from disconnect message to ntc */
   if (ntc->pduSp->m.disc.causeDgn[0].eh.pres)
   {
      ntc->causeDgn.eh.pres = PRSNT_NODEF;
      ntc->causeDgn.location.pres = ntc->pduSp->m.disc.causeDgn[0].location.pres;
      ntc->causeDgn.location.val = ntc->pduSp->m.disc.causeDgn[0].location.val;
      ntc->causeDgn.recommend.pres =ntc->pduSp->m.disc.causeDgn[0].recommend.pres;
      ntc->causeDgn.recommend.val = ntc->pduSp->m.disc.causeDgn[0].recommend.val;
      ntc->causeDgn.causeVal.pres = ntc->pduSp->m.disc.causeDgn[0].causeVal.pres;
      ntc->causeDgn.causeVal.val = ntc->pduSp->m.disc.causeDgn[0].causeVal.val;
      ntc->causeDgn.codeStand3.pres =
                     ntc->pduSp->m.disc.causeDgn[0].codeStand3.pres;
      ntc->causeDgn.codeStand3.val =
                     ntc->pduSp->m.disc.causeDgn[0].codeStand3.val;
      ntc->causeDgn.dgnVal.pres = ntc->pduSp->m.disc.causeDgn[0].dgnVal.pres;
      if (ntc->causeDgn.dgnVal.pres)
      {
         ntc->causeDgn.dgnVal.len = ntc->pduSp->m.disc.causeDgn[0].dgnVal.len;
         cmMemcpy((U8 *)ntc->causeDgn.dgnVal.val, 
                    (U8 *)ntc->pduSp->m.disc.causeDgn[0].dgnVal.val, ntc->causeDgn.dgnVal.len);
      }
   }

#ifdef ISDN_PTMPT   
   IN_PTMPT_ISSUE_STATUS(pcb, ntc, dCb);
#endif /* ISDN_PTMPT */

   /* 
    * Stop all Timers 
    */
   STOP_NTC_TIMERS;
   ntc->tmrCnt = 0;

   /* 
    * Change state to Disconnect Request (11) 
    */
   ntc->state = ST_DISRQ;

   inUpdSts(&pcb->sts.discRx, pcb);
   pcb->sts.discs++;

   if (pcb->cfg.swtch == SW_ATT5EB)     /* switch - ATT 5E */
   { 
      /* Generate Progress Event */
      MFINITSDU(&tCb->mfMsgCtl, ret, (U8) 0, (U8) PM_CNSTREQ, NULLP, 
                (ElmtHdr *) &ev, (U8) PRSNT_DEF, pcb->cfg.swtch, ntc->callType);
      MFINITELMT(&tCb->mfMsgCtl,ret,(ElmtHdr *)&ntc->pduSp->m.disc.causeDgn[0],
                 (ElmtHdr *) &ev.m.cnStEvnt.causeDgn[0], &meCauseDgn, 
                 (U8) PRSNT_NODEF, pcb->cfg.swtch, ntc->callType);
      MFINITELMT(&tCb->mfMsgCtl,ret,NULLP,(ElmtHdr*)&ev.m.cnStEvnt.causeDgn[1],
                 &meCauseDgn, (U8) NOTPRSNT, pcb->cfg.swtch, ntc->callType);
      /* set signal to 1 */
      /* set progress indicator to 8 */
      ev.m.cnStEvnt.signal.eh.pres = PRSNT_NODEF;
      INSETTOKVAL(ev.m.cnStEvnt.signal.signal, SIG_RNGBCK);

      ev.m.cnStEvnt.progInd.eh.pres = PRSNT_NODEF;
      INSETTOKVAL(ev.m.cnStEvnt.progInd.location, LOC_PUBNETLU);
      INSETTOKVAL(ev.m.cnStEvnt.progInd.codeStand0,CSTD_CCITT); 
      INSETTOKVAL(ev.m.cnStEvnt.progInd.progDesc, PD_IBAVAIL);

      InUiIntCnStInd(&tCb->pst, tCb->suId, ntc->suInstId, ntc->tCallRef, 
                     &ev.m.cnStEvnt, MI_PROGRESS, pcb->suId, dCb->ces);

   }

   MFINITSDU(&tCb->mfMsgCtl, ret, (U8)MI_DISC, (U8)PM_DISCREQ, 
             (ElmtHdr *) &ntc->pduSp->m.disc, (ElmtHdr *)&ev.m.discEvnt, 
             (U8) PRSNT_NODEF, tCb->sapCfg.swtch, ntc->callType);
   /* 26758 */
   ntc->relLoc = CC_REL_NRML_REM;
   /* 
    * Send Disconnect Indication to Upper 
    */
   InUiIntDiscInd(&tCb->pst, tCb->suId, (InInstId)ntc->suInstId, 
                  (InInstId)ntc->tCallRef, &ev.m.discEvnt);
   RETVALUE(ROK);
} /* end of inNetE14S09 */





  
/*
 *
 *       Fun:   inNetE14S12
 *
 *       Desc:  Input: Disconnect
 *              State: disconnect indication
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
         File:  qn_bdy5.c
 *
*/

#ifdef ANSI
PUBLIC S16 inNetE14S12
(
InNtc *ntc,
InCb *dCb 
)
#else
PUBLIC S16 inNetE14S12(ntc, dCb)
InNtc *ntc;
InCb *dCb;
#endif
{
   InCb *tCb;
   InPCB *pcb;
   U8 tmrNum;
   DiscEvnt discEvnt; 
   S16 ret;

   TRC2(inNetE14S12);

   /* get physical link control block */
   if ((pcb = *(pcbLstPtr + dCb->suId)) == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      INLOGERROR(ERRCLS_DEBUG, EIN388, (ErrVal) dCb->rSuId,
                 "inNetE14S12() failed, unable to access DLSAP.");
#endif
      RETVALUE(RFAILED);
   }
   tCb = ntc->tCallCb;

   inUpdSts(&pcb->sts.discRx, pcb);

   if (ntc->pduSp->m.disc.causeDgn[0].causeVal.pres)
      pcb->sts.lstRxCause = ntc->pduSp->m.disc.causeDgn[0].causeVal.val;
   if (ntc->pduSp->m.disc.causeDgn[0].dgnVal.pres)
      pcb->sts.lstRxDgn = ntc->pduSp->m.disc.causeDgn[0].dgnVal.val[0];

   /*
    * Stop Timer T305 or T306 
    */
   for (tmrNum = 0; tmrNum < MAXSIMTIMER; tmrNum++)
      if ((ntc->timers[tmrNum].tmrEvnt == TMR_T305) || 
          (ntc->timers[tmrNum].tmrEvnt == TMR_T306))
         inRmvNtcTq(ntc, tmrNum);

   /* generate Release to Lower */
   inGenRelLw(ntc, &ntc->pduSp->m.disc.causeDgn[0], dCb);

   /* TELICA-MCAO-07/26/01: Bug 5035, start code change */
   /* Send Disc Indication to Upper */
   MFINITSDU(&tCb->mfMsgCtl, ret, (U8) MI_DISC, (U8)PM_DISCREQ, 
         NULLP, (ElmtHdr *)&discEvnt,(U8) PRSNT_NODEF, pcb->cfg.swtch, 
         ntc->callType);
   MFINITELMT(&tCb->mfMsgCtl, ret, (ElmtHdr *) &ntc->pduSp->m.disc.causeDgn[0], 
              (ElmtHdr *) &discEvnt.causeDgn[0], &meCauseDgn, (U8) PRSNT_NODEF,
              pcb->cfg.swtch, ntc->callType);

   /* 26758 */
   ntc->relLoc = CC_REL_NRML_REM;
   /* TELICA-MCAO-07/26/01: Bug 5035, start of code change */
   InUiIntDiscInd(&tCb->pst, tCb->suId, (InInstId)ntc->suInstId, 
                 (InInstId)ntc->tCallRef, &discEvnt); 

   RETVALUE(ROK);
} /* end of inNetE14S12 */






  
/*
 *
 *       Fun:   inNetE14S18
 *
 *       Desc:  Input: Disconnect
 *              State: tone active
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
         File:  qn_bdy5.c
 *
*/

#ifdef ANSI
PUBLIC S16 inNetE14S18
(
InNtc *ntc,
InCb *dCb
)
#else
PUBLIC S16 inNetE14S18(ntc, dCb)
InNtc *ntc;
InCb *dCb;
#endif
{
   InCb *tCb;
   InPCB *pcb;
   AllSdus ev;
   U8 tmrNum;
   S16 ret;

   TRC2(inNetE14S18);

   tCb = ntc->tCallCb;
   /* get physical link control block */
   if ((pcb = *(pcbLstPtr + ntc->dCallCb->suId)) == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      INLOGERROR(ERRCLS_DEBUG, EIN389, (ErrVal) ntc->dCallCb->rSuId,
                 "inNetE14S18() failed, unable to access DLSAP.");
#endif
      RETVALUE(RFAILED);
   }
   if (ntc->pduSp->m.disc.causeDgn[0].causeVal.pres)
      pcb->sts.lstRxCause = ntc->pduSp->m.disc.causeDgn[0].causeVal.val;
   if (ntc->pduSp->m.disc.causeDgn[0].dgnVal.pres)
      pcb->sts.lstRxDgn = ntc->pduSp->m.disc.causeDgn[0].dgnVal.val[0];

   /* Bug 45852: copy causeDgn from disconnect message to ntc */
   if (ntc->pduSp->m.disc.causeDgn[0].eh.pres)
   {
      ntc->causeDgn.eh.pres = PRSNT_NODEF;
      ntc->causeDgn.location.pres = ntc->pduSp->m.disc.causeDgn[0].location.pres;
      ntc->causeDgn.location.val = ntc->pduSp->m.disc.causeDgn[0].location.val;
      ntc->causeDgn.recommend.pres =ntc->pduSp->m.disc.causeDgn[0].recommend.pres;
      ntc->causeDgn.recommend.val = ntc->pduSp->m.disc.causeDgn[0].recommend.val;
      ntc->causeDgn.causeVal.pres = ntc->pduSp->m.disc.causeDgn[0].causeVal.pres;
      ntc->causeDgn.causeVal.val = ntc->pduSp->m.disc.causeDgn[0].causeVal.val;
      ntc->causeDgn.codeStand3.pres =
                  ntc->pduSp->m.disc.causeDgn[0].codeStand3.pres;
      ntc->causeDgn.codeStand3.val =
                     ntc->pduSp->m.disc.causeDgn[0].codeStand3.val;
      ntc->causeDgn.dgnVal.pres = ntc->pduSp->m.disc.causeDgn[0].dgnVal.pres;
      if (ntc->causeDgn.dgnVal.pres)
      {
         ntc->causeDgn.dgnVal.len = ntc->pduSp->m.disc.causeDgn[0].dgnVal.len;
         cmMemcpy((U8 *)ntc->causeDgn.dgnVal.val, 
                    (U8 *)ntc->pduSp->m.disc.causeDgn[0].dgnVal.val, ntc->causeDgn.dgnVal.len);
      }
    }

   switch (pcb->cfg.swtch)
   {
      case SW_ATT4E:
      case SW_ATT5EP:
         if ((pcb->cfg.swtch == SW_ATT4E) || (pcb->cfg.swtch == SW_ATT5EP))
         {
            /* Stop all Timers */
            STOP_NTC_TIMERS;
      
            /* Change state to Disconnect Request (11) */
            ntc->state = ST_DISRQ;

            inUpdSts(&pcb->sts.discRx, pcb);
            pcb->sts.discs++;

            MFINITSDU(&tCb->mfMsgCtl, ret, (U8) MI_DISC, (U8) PM_DISCREQ, 
                      (ElmtHdr *) &ntc->pduSp->m.disc, (ElmtHdr *)&ev.m.discEvnt,
                      (U8) PRSNT_NODEF, tCb->sapCfg.swtch, ntc->callType);

         /* 26758 */
         ntc->relLoc = CC_REL_NRML_REM;         
            /* Send Disconnect Indication to Upper */
            InUiIntDiscInd(&tCb->pst, tCb->suId, (InInstId)ntc->suInstId, 
                           (InInstId)ntc->tCallRef, &ev.m.discEvnt);
         }
         break;
      case SW_ATT5EB:
         
      default:
#if (ERRCLASS & ERRCLS_DEBUG)
         INLOGERROR(ERRCLS_DEBUG, EIN390, (ErrVal) pcb->cfg.swtch,
                    "inNetE14S18() failed, invalid message.");
#endif
         inNetMSGINV(ntc, dCb);
         break;
   }
   RETVALUE(ROK);
} /* end of inNetE14S18 */





  
/*
 *
 *       Fun:   inNetE15S12
 *
 *       Desc:  Input: Release
 *              State: disconnect indication
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
         File:  qn_bdy5.c
 *
*/

#ifdef ANSI
PUBLIC S16 inNetE15S12
(
InNtc *ntc,
InCb *dCb 
)
#else
PUBLIC S16 inNetE15S12(ntc, dCb)
InNtc *ntc;
InCb *dCb;
#endif
{
   InCb *tCb;
   InPCB *pcb;
   RelEvnt relEvnt;
   U8 tmrNum;
   S16 ret;
   UConnId suInstId;
   UConnId tCallRef;

   TRC2(inNetE15S12);

   tCb = ntc->tCallCb;
   /* inititialize variables */
   suInstId = 0;
   tCallRef = 0;
   /* get physical link control block */
   if ((pcb = *(pcbLstPtr + ntc->dCallCb->suId)) == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      INLOGERROR(ERRCLS_DEBUG, EIN391, (ErrVal) ntc->dCallCb->rSuId,
                 "inNetE15S12() failed, unable to access DLSAP.");
#endif
      RETVALUE(RFAILED);
   }

   /* 
    * Stop Timer T305 and T306 
    */
   for (tmrNum = 0; tmrNum < MAXSIMTIMER; tmrNum++)
      if ((ntc->timers[tmrNum].tmrEvnt == TMR_T305) || 
          (ntc->timers[tmrNum].tmrEvnt == TMR_T306))
         inRmvNtcTq(ntc, tmrNum);
 
   inUpdSts(&pcb->sts.relRx, pcb);

   /*
    * FID 16706.0 - We have received a network response, so if a
    * facilityStr1 is stored in the NTC it should be removed so
    * we don't include it again in the subsequent Release Complete.
    */
   if (ntc->facilityStr1)
   {
      INDBGP(DBGMASK_LI, "%s freeing the facilityStr1 in NTC\n",__FUNCTION__);

      if ((ret = SPutSBuf(DFLT_REGION, DFLT_POOL,
           ntc->facilityStr1, sizeof(FacilityStr))) != ROK)
      {
#if (ERRCLASS & ERRCLS_DEBUG)
         INLOGERROR(ERRCLS_DEBUG, EIN399, (ErrVal) ret,
                    "inNetE15S12() error, unable to release memory.");
#endif
      }
      ntc->facilityStr1 = NULLP;
   }

   if (ntc->pduSp->m.rel.causeDgn[0].causeVal.pres)
      pcb->sts.lstRxCause = ntc->pduSp->m.rel.causeDgn[0].causeVal.val;


   if (ntc->pduSp->m.rel.causeDgn[0].dgnVal.pres)
      pcb->sts.lstRxDgn = ntc->pduSp->m.rel.causeDgn[0].dgnVal.val[0];

   if (tCb)
   {
      /* 
       * Send Release Indication to Upper 
       */
      MFINITSDU(&tCb->mfMsgCtl, ret, (U8) MI_RELEASE, (U8) PM_RELREQ, 
                (ElmtHdr *) &ntc->pduSp->m.rel, (ElmtHdr *) &relEvnt, 
                (U8)PRSNT_NODEF, pcb->cfg.swtch, ntc->callType);
      suInstId = ntc->suInstId;
      tCallRef = ntc->tCallRef;
   }

   /* MCAO-05/22/02 BUG 11372: send RelInd before release call control block */
   ntc->state = ST_NULL;
   if (tCb)
   {
      /* 26758 */
      ntc->relLoc = CC_REL_NRML_REM;
      /* 
       * Send Release Indication to Upper 
       */
      InUiIntRelInd(&tCb->pst, tCb->suId, (InInstId)suInstId, (InInstId)tCallRef, 
                    &relEvnt);
   }

   /* 
    * Send Release Complete to Lower 
    */
   inGenRelCLw(ntc, &ntc->pduSp->m.rel.causeDgn[0], dCb);

   RETVALUE(ROK);
} /* end of inNetE15S12 */






/* PTMPT023 */
/*
 *
 *       Fun:   inNetE01S07
 *
 *       Desc:  Input: Call Proceeding
 *              State: call delivered.
 *              The function implements the second branch on ETS 300 102-2.
 *              
 *       Ret:   ROK      - ok
 *
 *       Notes: None   
 *
         File:  qn_bdy5.c
 *
*/

#ifdef ANSI
PUBLIC S16 inNetE01S07
(
InNtc *ntc,
InCb  *dCb 
)
#else
PUBLIC S16 inNetE01S07(ntc, dCb)
InNtc *ntc;
InCb *dCb;
#endif
{
   S16 ret = 0;
   InPCB *pcb = NULL;

   TRC2(inNetE01S07);
   UNUSED(ret);
   UNUSED(pcb);

#ifdef ISDN_PTMPT
   if ((pcb = *(pcbLstPtr + ntc->dCallCb->suId)) == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
     INLOGERROR(ERRCLS_DEBUG, EIN392, (ErrVal) ntc->dCallCb->rSuId,
                "inNetE01S07() failed, unable to access DLSAP.");
#endif
     RETVALUE(RFAILED);
   }
   
   if ((ret = inHandleMptChannelCheck(ntc, dCb, pcb, 
                                      &ntc->pduSp->m.callProc.chanId,
                                      MI_CALLPROC)) != ROK)
   {
      RETVALUE(ROK);
   }


   if ((pcb->cfg.intCfg != INTCFG_MULTI)
       || (ntc->setupBCast != TRUE))
      inNetMSGINV(ntc, dCb);
       
#else /*  not ISDN_PTMPT */
   inNetMSGINV(ntc, dCb);
#endif /* ISDN_PTMPT */

    
   RETVALUE(ROK);
} /* inNetE01S07 */






/* PTMPT025 */

/*
*
*       Fun:   inNetE06S07
*
*       Desc:  Input: Call Proceeding
*              State: call delivered.
*              The function implements the second branch on ETS 300 102-2.
*              
*       Ret:   ROK      - ok
*
*       Notes: None   
*
*       File:  qn_bdy5.c
*
*/

#ifdef ANSI
PUBLIC S16 inNetE06S07
(
InNtc *ntc,
InCb  *dCb 
)
#else
PUBLIC S16 inNetE06S07(ntc, dCb)
InNtc *ntc;
InCb *dCb;
#endif
{
   S16 ret = 0;          /* General Return Value. */
#ifdef ISDN_PTMPT 
   InPCB *pcb = NULL;
#endif
   
   TRC2(inNetE06S07);
   UNUSED(ret);

#ifdef ISDN_PTMPT 

   if ((pcb = *(pcbLstPtr + ntc->dCallCb->suId)) == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
     INLOGERROR(ERRCLS_DEBUG, EIN393, (ErrVal) ntc->dCallCb->rSuId,
                "inNetE06S07() failed, unable to access DLSAP.");
#endif
     RETVALUE(RFAILED);
   }
   
   if ((pcb->cfg.intCfg == INTCFG_MULTI)
       && ((pcb->cfg.intType == NETWORK)
           && (ntc->setupBCast == TRUE)))
   {
      if (!ntc->pduSp->m.setupAck.chanId.eh.pres)
         inInsrtChnlId(ntc, &ntc->pduSp->m.setupAck.chanId, ntc->ctldPcb);
       
      if ((ret = inValChnlId(ntc, &ntc->pduSp->m.setupAck.chanId)) != ROK)
      {  
         /* 
          * Channel is not valid, issue internal release request.
          */       
        inIntPrimRelReq(ntc, dCb, &ntc->causeDgn);    
      }
      else  /* Valid channel */
      {
         QLen infoMsgQueueLen;
         U32  index;  /* Index to loop through infoMsgQueue */
         Buffer *infoBuf;
          
         inIntPrimSetupAckReq(ntc, dCb);
         /*
          * Check the state, if the state was overlap receiveing, then 
          * we need to issue multiple info requests to the CES.  These
          * info messages needs to be picked up from the queue that was
          * built up.
          */
         if (ntc->state == ST_OVLAPRCV)
         {
            SFndLenQueue(&ntc->infoMsgQueue, &infoMsgQueueLen);
            for (index = 0; index < infoMsgQueueLen; index++)
            {
               if ((ret = SExamQueue(&infoBuf, &ntc->infoMsgQueue,
                                     index)) == ROK)
               {
                  inIntPrimInfoReq(ntc, dCb, infoBuf);
               }
                
            }
         }
      }
       
   }
#endif /* ISDN_PTMPT */
    
   RETVALUE(ROK);
} /* inNetE06S07 */





  
/*
 *
 *       Fun:   inNetENDS08
 *
 *       Desc:  Input: Call Proceeding
 *              State: call delivered.
 *              The function implements the second branch on ETS 300 102-2.
 *              
 *       Ret:   ROK      - ok
 *
 *       Notes: None   
 *
         File:  qn_bdy5.c
 *
*/

#ifdef ANSI
PUBLIC S16 inNetENDS08
(
InNtc *ntc,
InCb  *dCb 
)
#else
PUBLIC S16 inNetENDS08(ntc, dCb)
InNtc *ntc;
InCb *dCb;
#endif
{
   CauseDgn cause;
#ifdef ISDN_PTMPT 
   InPCB *pcb;
#endif

   TRC2(inNetENDS08);
   UNUSED(cause);

#ifdef ISDN_PTMPT 

   if ((pcb = *(pcbLstPtr + ntc->dCallCb->suId)) == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
     INLOGERROR(ERRCLS_DEBUG, EIN394, (ErrVal) ntc->dCallCb->rSuId,
                "inNetENDS08() failed, unable to access DLSAP.");
#endif
     RETVALUE(RFAILED);
   }

   if ((pcb->cfg.intCfg == INTCFG_MULTI)
       && (ntc->setupBCast == TRUE))
   {
      INSETTOKHDR(cause);
      INSETTOKVAL(cause.location, LOC_PUBNETLU);
      INSETTOKVAL(cause.causeVal, CCMSGNOTCMP);
      INSETTOKVAL(cause.codeStand3, CSTD_CCITT);

      cause.recommend.pres = NOTPRSNT;
      cause.dgnVal.pres = NOTPRSNT;

      inDeleteCbIntTimers(ntc, dCb);
      ntc->cesState[dCb->ces] = INTSTATE_19RELREQ;
      inIntGenRelease(ntc, dCb, &cause);
   } /* pcb->cfg.intCfg == INTCFG_MULTI) */
   else
#endif /* ISDN_PTMPT */
      inNetMSGINV(ntc, dCb);

   RETVALUE(ROK);    
} /* End of Function PUBLIC S16 inNetENDS08(ntc, dCb) */





  
/*
 *
 *       Fun:   inNetE23S07
 *
 *       Desc:  Input: Facility
 *              State: Call Received
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
         File:  qn_bdy5.c
 *
*/

#ifdef ANSI
PUBLIC S16 inNetE23S07
(
InNtc *ntc,
InCb  *dCb
)
#else
PUBLIC S16 inNetE23S07(ntc, dCb)
InNtc *ntc;
InCb *dCb;
#endif
{
   InCb *tCb;
   InPCB *pcb;
   AllSdus ev;
   S16 ret;

   TRC2(inNetE23S07);

   tCb = ntc->tCallCb;

   /* get physical link control block */
   if ((pcb = *(pcbLstPtr + dCb->suId)) == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      INLOGERROR(ERRCLS_DEBUG, EIN364, (ErrVal) dCb->rSuId,
                 "inNetE23S07() failed, unable to access DLSAP.");
#endif
      RETVALUE(RFAILED);
   }  

   inUpdSts(&pcb->sts.facilRx, pcb);

   switch (pcb->cfg.swtch)
   {
#ifdef ETSI                  /* TELICA-dsolanki-INTMGCDEV */
      case SW_ETSI:          /* switch - etsi */
         if (ntc->callType & MF_CKT)
         {
            MFINITSDU(&tCb->mfMsgCtl, ret, (U8) MI_FACIL, (U8) PM_FAC, 
                      (ElmtHdr *) &ntc->pduSp->m.facil, (ElmtHdr *) &ev, 
                      (U8) PRSNT_NODEF, tCb->sapCfg.swtch, ntc->callType);
            /* Send Facility Indication to Upper */ 
            InUiIntFacInd(&tCb->pst, tCb->suId, ntc->suInstId, ntc->tCallRef,
                          &ev.m.facEvnt, MI_FACIL, pcb->suId, dCb->ces);
         }
         break;

#endif /* ETSI */            /* TELICA-dsolanki-INTMGCDEV */

      default:
#if (ERRCLASS & ERRCLS_DEBUG)
         INLOGERROR(ERRCLS_DEBUG, EIN370, (ErrVal) pcb->cfg.swtch,
                    "inNetE23S07() failed, invalid switch.");
#endif
         inNetMSGINV(ntc, dCb);      
         break;
   }

   RETVALUE(ROK);
} /* end of inNetE23S07 */





  
/*       
 *
 *       Fun:   inNetE23S09
 *
 *       Desc:  Input: Facility 
 *              State: Incoming Call Proceeding
 *                     
 *       Ret:   ROK      - ok
 *                         
 *       Notes: None  
 * 
         File:  qn_bdy5.c
 * 
*/ 
   
#ifdef ANSI
PUBLIC S16 inNetE23S09
(
InNtc *ntc,
InCb  *dCb 
)
#else
PUBLIC S16 inNetE23S09(ntc, dCb)
InNtc *ntc;
InCb *dCb;
#endif
{
   InCb *tCb;
   InPCB *pcb;
   AllSdus ev;
   S16 ret;

   TRC2(inNetE23S09);

   tCb = ntc->tCallCb;

   /* get physical link control block */
   if ((pcb = *(pcbLstPtr + dCb->suId)) == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      INLOGERROR(ERRCLS_DEBUG, EIN378, (ErrVal) dCb->rSuId,
                 "inNetE23S09() failed, unable to access DLSAP.");
#endif
      RETVALUE(RFAILED);
   }

   inUpdSts(&pcb->sts.facilRx, pcb);

   switch (pcb->cfg.swtch)
   {
#ifdef ETSI                  /* TELICA-dsolanki-INTMGCDEV */
      case SW_ETSI:          /* switch - etsi */
         if (ntc->callType & MF_CKT)
         {
            MFINITSDU(&tCb->mfMsgCtl, ret, (U8) MI_FACIL, (U8) PM_FAC,
                      (ElmtHdr *) &ntc->pduSp->m.facil, (ElmtHdr *) &ev,
                      (U8) PRSNT_NODEF, tCb->sapCfg.swtch, ntc->callType);
            /* Send Facility Indication to Upper */
            InUiIntFacInd(&tCb->pst, tCb->suId, ntc->suInstId, ntc->tCallRef,
                          &ev.m.facEvnt, MI_FACIL, pcb->suId, dCb->ces);
         }
         break;

#endif /* ETSI */            /* TELICA-dsolanki-INTMGCDEV */

      default:
#if (ERRCLASS & ERRCLS_DEBUG)
         INLOGERROR(ERRCLS_DEBUG, EIN398, (ErrVal) pcb->cfg.swtch,
                    "inNetE23S09() failed, invalid switch.");
#endif
         inNetMSGINV(ntc, dCb);
         break;
   }

   RETVALUE(ROK);
} /* end of inNetE23S09 */

/* FID 16970.0 + */
/*
 *
 *       Fun:   inNetE23S31
 *
 *       Desc:  Input: Facility
 *              State: call independent
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
         File:  qn_bdy5.c
 *
*/

#ifdef ANSI
PUBLIC S16 inNetE23S31
(
InNtc *ntc,
InCb  *dCb
)
#else
PUBLIC S16 inNetE23S31(ntc, dCb)
InNtc *ntc;
InCb *dCb;
#endif
{
   InCb *tCb;
   InPCB *pcb;
   AllSdus ev;
   S16 ret;
   TRC2(inNetE23S31);

   tCb = ntc->tCallCb;

   /* get physical link control block */
   if ((pcb = *(pcbLstPtr + dCb->suId)) == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      INLOGERROR(ERRCLS_DEBUG, EIN346, (ErrVal) dCb->rSuId,
                 "inNetE23S31() failed, unable to access DLSAP.");
#endif
      RETVALUE(RFAILED);
   }

   inUpdSts(&pcb->sts.facilRx, pcb);
   cmMemset((U8 *)&ev, 0, sizeof(AllSdus));

   switch (pcb->cfg.swtch)
   {
#ifdef ETSI                  /* TELICA-dsolanki-INTMGCDEV */
      case SW_ETSI:          /* switch - etsi */
         MFINITSDU(&tCb->mfMsgCtl, ret, (U8) MI_FACIL, (U8) PM_FAC,
                   (ElmtHdr *) &ntc->pduSp->m.facil, (ElmtHdr *) &ev,
                   (U8) PRSNT_NODEF, tCb->sapCfg.swtch, ntc->callType);
         /*Generate Status if facility information element contain an invalid protocol profile*/ 
         if(ev.m.facEvnt.facilityStr1.facilityStr.val[0] != PROT_PRFL_RMT_OPRA)
         {
             ntc->causeDgn.eh.pres = PRSNT_NODEF;
             ntc->causeDgn.causeVal.pres = PRSNT_NODEF;
             ntc->causeDgn.causeVal.val = CCINVINFOEL;
             ntc->causeDgn.location.pres = PRSNT_NODEF;
             ntc->causeDgn.location.val = PRSNT_NODEF;
             ntc->causeDgn.dgnVal.pres = NOTPRSNT;
             ntc->causeDgn.recommend.pres = NOTPRSNT;
             ntc->causeDgn.codeStand3.pres = PRSNT_NODEF;
             ntc->causeDgn.codeStand3.val = CSTD_CCITT;
             inGenStaMsg(ntc, &ntc->causeDgn, dCb);
             RETVALUE(RFAILED);
         }

         /* Send Facility Indication to Upper */
         InUiIntIndpFacInd(&tCb->pst, tCb->suId, ntc->suInstId, ntc->tCallRef,
                       &ev.m.facEvnt, pcb->suId, dCb->ces);
         break;

#endif /* ETSI */            /* TELICA-dsolanki-INTMGCDEV */

      default:
#if (ERRCLASS & ERRCLS_DEBUG)
         INLOGERROR(ERRCLS_DEBUG, EIN347, (ErrVal) pcb->cfg.swtch,
                    "inNetE23S31() failed, invalid switch.");
#endif
         inNetMSGINV(ntc, dCb);
         break;
   }

   RETVALUE(ROK);
} /* end of inNetE23S31 */

/*
 *
 *       Fun:   inNetE24S00
 *
 *       Desc:  Input: Register
 *              State: Null 
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
         File:  qn_bdy5.c
 *
*/

#ifdef ANSI
PUBLIC S16 inNetE24S00
(
InNtc *ntc,
InCb  *dCb
)
#else
PUBLIC S16 inNetE24S00(ntc, dCb)
InNtc *ntc;
InCb *dCb;
#endif
{
   InCb *tCb;
   InPCB *pcb;
   AllSdus ev;
   S16 ret;

   TRC2(inNetE24S00);
   /* get physical link control block */
   if ((pcb = *(pcbLstPtr + dCb->suId)) == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      INLOGERROR(ERRCLS_DEBUG, EIN348, (ErrVal) dCb->rSuId,
                 "inNetE24S00() failed, unable to access DLSAP.");
#endif
      RETVALUE(RFAILED);
   }

   if ((ntc->tCallCb = *(inSapLstPtr + pcb->cfg.tCbId)) == NULLP)
   {
      ntc->causeDgn.causeVal.pres = PRSNT_NODEF;
      ntc->causeDgn.causeVal.val = CCRESCUNAVAIL;
      inGenRelCLw(ntc, &ntc->causeDgn, dCb);
      RETVALUE(RFAILED);
   }
   /* get upper call reference */
   if (inGetInstId(ntc->tCallCb, &ntc->tCallRef) == RFAILED)
   {
      INSETTOKVAL(ntc->causeDgn.causeVal, CCRESCUNAVAIL);
      inGenRelCLw(ntc, &ntc->causeDgn, dCb);
      RETVALUE(RFAILED);
   }

   inInsCallRefEnt1(ntc->tCallCb, ntc);
   tCb = ntc->tCallCb;
   cmMemset((U8 *)&ev, 0, sizeof(AllSdus));
   inUpdSts(&pcb->sts.regRx, pcb);
   ntc->state = ST_CALLINDEP;
   ntc->checkRegCfm = IN_REG_IND;
   
   switch (pcb->cfg.swtch)
   {
#ifdef ETSI                  /* TELICA-dsolanki-INTMGCDEV */
      case SW_ETSI:          /* switch - etsi */
          MFINITSDU(&tCb->mfMsgCtl, ret, (U8) MI_REG, (U8) PM_REG,
                    (ElmtHdr *) &ntc->pduSp->m.reg, (ElmtHdr *) &ev,
                    (U8) PRSNT_NODEF, tCb->sapCfg.swtch, ntc->callType);
          /* Send Register Indication to Upper */
          InUiIntRegInd(&tCb->pst, tCb->suId, ntc->suInstId, ntc->tCallRef,
                        &ev.m.regEvnt, pcb->suId, dCb->ces);
          break;

#endif /* ETSI */            /* TELICA-dsolanki-INTMGCDEV */

      default:
#if (ERRCLASS & ERRCLS_DEBUG)
         INLOGERROR(ERRCLS_DEBUG, EIN349, (ErrVal) pcb->cfg.swtch,
                    "inNetE24S00() failed, invalid switch.");
#endif
         inNetMSGINV(ntc, dCb);
         break;
   }
   zqUpdPeer();

   RETVALUE(ROK);
} /* end of inNetE24S00 */
/* FID 16970.0 - */
 

  
/*
 *    Func: inCallAbortAllStates
 *    Desc: Input: Any valid message.
 *          State: call Abort
 *          This function implements call abort state for point to
 *          multipoint network side.
 *              
 *    Ret:   ROK      - ok
 *
 *    Notes: None   
 *
 File:  qn_bdy5.c
 *
 */

#ifdef ANSI
PUBLIC S16 inCallAbortAllStates
(
InNtc *ntc, 
InCb  *dCb
)
#else
PUBLIC S16 inCallAbortAllStates(ntc, dCb)
InNtc *ntc;
InCb  *dCb;
#endif
{
   CauseDgn cause;  /* Cause for release request. */
#ifdef ISDN_PTMPT 
   InPCB *pcb;
#endif

   TRC2(inCallAbortAllStates);
   UNUSED(cause);

#ifdef ISDN_PTMPT 

   if ((pcb = *(pcbLstPtr + ntc->dCallCb->suId)) == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
     INLOGERROR(ERRCLS_DEBUG, EIN395, (ErrVal) ntc->dCallCb->rSuId,
                "inNetENDS08() failed, unable to access DLSAP.");
#endif
     RETVALUE(RFAILED);
   }

   if ((pcb->cfg.intCfg != INTCFG_MULTI)
       || (pcb->cfg.swtch != SW_ETSI))
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      INLOGERROR(ERRCLS_DEBUG, EIN396, (ErrVal) pcb->cfg.intCfg,
                 "inCallAbortAllStates() failed, incorrect configuration."); 
#endif /* ERRCLASS & ERRCLS_DEBUG */
      RETVALUE(RFAILED);
   }

   ntc->state = ST_CALLABRT;

   INSETTOKHDR(cause);
   INSETTOKVAL(cause.location, LOC_PUBNETLU);
   INSETTOKVAL(cause.causeVal, CCTIMERECOVR);
   INSETTOKVAL(cause.codeStand3, CSTD_CCITT);
   
   cause.recommend.pres = NOTPRSNT;
   cause.dgnVal.pres = NOTPRSNT;
   /* 26758 */
   ntc->relLoc = CC_REL_NRML_REM;
   
   switch (dCb->mfMsgCtl.msgIdx)
   {
      case MI_ALERTING:
      case MI_CONNECT:
      case MI_CALLPROC:
      case MI_SETUPACK:
         inIntPrimRelReq(ntc, dCb, &cause);    
         break;
      case MI_RELEASE:
         inIntReleaseCmpltInd(ntc, dCb);
         break;
      case MI_RELCMPLT:
         break;
      default:
#if (ERRCLASS & ERRCLS_DEBUG)
         INLOGERROR(ERRCLS_DEBUG, EIN397, (ErrVal)dCb->mfMsgCtl.msgIdx,
                    "inCallAbortAllStates() failed, incorrect event. ");
#endif /* ERRCLASS & ERRCLS_DEBUG */
         break;
   } /* End of switch (dCb->mfMsgCtl.msgIdx) */
#endif  /* ISDN_PTMPT */

   RETVALUE(ROK);
} /* End of function inCallAbortAllStates */

void
inFillCriInfo
(
UConnId tCallRef,
CctCriList *criList,
CctCriEnt *inCri,
U8 *string
)
{
  if (!criList || !inCri || !string) return;

  sprintf(string, "\"LAYER:%4s VER:%d-%d CRF:%ld\"", "ISDN",
      InGeoCriMajVer, InGeoCriMinVer, tCallRef);

  string[strlen(string)] = '\0';
  inCri->criEnt.pres = PRSNT_NODEF;
  inCri->criEnt.len = strlen(string);
  inCri->criEnt.size = 0; /* No Memory allocation involved */
  inCri->criEnt.val = string;
  inCri->qEnt.next = inCri->qEnt.prev = NULLP;

  cmQInsert(&criList->cp, (void*)inCri);
  criList->pres = TRUE;

  return;
}

S16
inDecodeCri
(
CctCriList  *cctCri,
UConnId *ref
)
{

  if (cctCri && ref && (cctCri->pres) && (cctCri->cp.count))
  {
    CctCriEnt *criEnt = NULLP;
    cmQGetNext(&cctCri->cp, NULLP, (void **)&criEnt);
    if (criEnt)
    {
      S32  numTok = 0;
      U32  verMaj = 0, verMin = 0, crf = 0;
      U8 *cri = criEnt->criEnt.val;
      U8 layer[10];

      numTok = sscanf(cri, "\"LAYER:%4s VER:%ld-%ld CRF:%ld", &layer[0], &verMaj, &verMin, &crf);

      if (numTok != 4)
      {
        DP("WRONG CRI being passed to ISDN %s\n", cri);
        return (RFAILED);
      }
      if (strncmp (layer, "ISDN", strlen ("ISDN")) != 0)
      {
        DP("WRONG CRI being passed to ISDN %s\n", cri);
        return (RFAILED);
      }
      if ((verMaj != InGeoCriMajVer) || (verMin != InGeoCriMinVer))
      {
        DP("WRONG CRI: Needs upgrade : %ld-%ld to %d-%d\n", verMaj, verMin,
            InGeoCriMajVer, InGeoCriMinVer);
        return (RFAILED);
      }
      *ref = crf;
      DP("CctCriList: CRI being passed to ISDN %s\n", cri);
    }
  }
  
  return (ROK);
}

/* BUG:37899-mqin */
/* 
 *    Func: inChkFacilMsg
 *    Desc: This function checks the received Facility message and
 *          copy the facility string into ConEvnt before sending
 *          connnection indication to upper layer.
 *
 *    Ret:   ROK      - ok
 *
 *    File: in_bdy5.c
 *
 */
PUBLIC S16 inChkFacilMsg(InCb *cb, InPCB *pcb, InNtc *ntc)
{  
   InCb *tCb;
   U8 tmrNum;
   CctCriEnt inCri;
   U8 string[IN_GEO_CRI_LEN_MAX];

   /* initialize tCb */
   tCb = (InCb *) NULLP;
   
   if (pcb->cfg.tCbCfg)
   {  
      if (!(tCb = inGetSapPtr(pcb->cfg.tCbId)))
      {
         INSETTOKVAL(ntc->causeDgn.causeVal, CCNORTTODEST);
         inGenRelCLw(ntc, &ntc->causeDgn, cb);
         RETVALUE(ROK);
      }
   }
   else
   {
      INSETTOKVAL(ntc->causeDgn.causeVal, CCNORTTODEST);
      inGenRelCLw(ntc, &ntc->causeDgn, cb);
      RETVALUE(ROK);
   }

   STOP_NTC_TIMER(TMR_THOLDSETUP);

   if (ntc->pduSp->m.facil.facilityStr.eh.pres &&
       ntc->pduSp->m.facil.facilityStr.facilityStr.pres)
   { 
      if (ntc->pduSp->m.facil.facilityStr.facilityStr.val[5] > 0x08 &&
          ntc->pduSp->m.facil.facilityStr.facilityStr.val[9] == 0x02 &&
          ntc->pduSp->m.facil.facilityStr.facilityStr.val[11] == 0x0)
      {
         /* facilityStr contains callingName */
         cmMemcpy((U8 *)&(ntc->conEvnt->facilityStr),
                  (U8 *)&(ntc->pduSp->m.facil.facilityStr),
                  sizeof(FacilityStr));
      }
   }

   inFillCriInfo(ntc->dCallRef, &ntc->conEvnt->criList, &inCri, string);

   /* Send connection indication */
   InUiIntConInd(&tCb->pst, tCb->suId, ntc->suInstId, ntc->tCallRef,
                 ntc->conEvnt, pcb->suId, cb->ces);

   if (ntc->conEvnt)
   {
      SPutSBuf(DFLT_REGION, DFLT_POOL, ntc->conEvnt,
               (Size) sizeof(ConEvnt));
   }

   RETVALUE(ROK);
}


/*
 *    Func: inChkFacilStr
 *    Desc: This function checks the received Facility string to see
 *          if the operation is informationFollowing.
 *
 *    Ret:   ROK      - ok
 *
 *    File: in_bdy5.c
 *
 */
PUBLIC S16 inChkFacilStr(TknStrE *facilityStr)
{
   U8 CVlen, OVlen;

   /* check Component Value */
   if (facilityStr->val[4] != 0xA1)
   {
      /* this is not Invoke Compoent */
      RETVALUE(RFAILED);
   }

   /* get Component Value length */
   CVlen = facilityStr->val[5];

   /* the next 3 bytes should be Invoke Identifier, no need to check it */
   /* check Operation Value */
   if (facilityStr->val[9] == 0x06)
   {
      /* this is Object Identifier, get the length */
      OVlen = facilityStr->val[10];

      /* check the Component Value length */
      if (CVlen < OVlen + 5)
         RETVALUE(RFAILED);

      /* check the last byte of Operation Value */
      if (facilityStr->val[10 + OVlen] == 0x04)
      {
         /* the subidentifier is informationFollowing */
         RETVALUE(ROK);
      }
   }

   /* Operation Value is Integer, which is callingName */
   RETVALUE(RFAILED);
}
  
/********************************************************************30**
  
         End of file: qn_bdy5.c 3.9  -  09/29/99 14:36:54
  
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

1.6.0.0         lc       1. check for MF_CKT bit instead of value in
                            ntc->callType
                         2. initialize sdu as connect status request
                            instead of connect response
                         3. change order of assignment for tCallCb and
                            tCallRef inNetE05S00 function
                         4. assign ntc to tCallCb instead of dCallCb in
                            inNetE05S00 function                            
                         5. or MF_USR flag into ntc->callType instead
                            of just setting
                         6. pass setup pdu pointer to MFINITPDU instead
                            of NULLP in inUsrE05S00 function
                         7. change ConEvnt typedef to CnStEvnt typdef

1.7.0.0         lc       1. error codes were changed       
                         2. overlap procedure was changed
                         3. random numbers for call references
                         4. Connect Request state was implemented
                         5. statistic counter for Connecr Ack was added

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
                jrl     16. trillium development system checkpoint (dvs)
                            at version: 1.7.0.0

2.1             lc      1. changed calling convention for upper and lower
                           interfaces primitives.
                        2. conversion to second release of message generating
                           and processing functions universal to Trillium's 
                           implementation of communication protocols.
                        3. added INSNET variant.
                        4. added dChan parametr to CnStInd primitive.
                        5. add ANSI function declarations
                        6. add envopt.h include file
                        7. text changes

2.2             jrl     1. remove MSCDOS
 
*********************************************************************71*/

/********************************************************************80**

  version    pat  init                   description
----------- ----- ----  ------------------------------------------------
2.3          ---  jrl   1. text changes

2.4          ---  bn    1. add inNetE29S10 to the matrix.
                        2. add support for nfas.

2.5          ---  rg    1. added etsi variant.
                        2. text changes.
                        3. added arrays for checking msg compatibility
                           with call state, for network side.
                        4. in inNetE15S12, added check for tCb being Null
                           before sending Release Indication to layer 4.

2.6          ---  lc    1. miscelanneous changes

2.7          ---  bn    1. added missing } in inUsrE05S00.
                        2. removed extra } in inUsrE05S00.
                        3. changed compatability table to allow Connect 
                           Acknowledge message in active state.

2.8          ---  bn    1. text changes

2.9          ---  rg    1. added support for Facility related messages in 
                           message compatibility check matrices.
             ---  bn    2. corrected User Information entry in message
                           compatiblity check matrix for VN3.
             ---  bn    3. text changes
             ---  bn    4. change VN3 define to VN2 define
             ---  bn    5. change SW_VN3 define to SW_VN2

2.10         ---  bn    1. text changes

2.11         ---  bn    1. add include cm5.?

2.12         ---  bn    1. replaced return with RETVALUE.
             ---  bn    2. add nt dms-250.

2.13         ---  bn    1. added inNetE19S00 and inNetE31S00 for Q932.
             ---  bn    2. changed compatability entries to reflect Q932.

2.14         ---  bn    1. surrounded the code pertaining to specific options
                           by corresponding ifdef's

2.15         ---  bn    1. changed #ifdef SUSPEND to #if ISDN_SUSPEND.
             ---  bn    2. replaced ss_??.? by ssi.?

3.1          ---  bn    1. Changed interfaces to include Pst structure
                           instead of BndCfg.
             ---  bn    2. changed layer manager interface.
             ---  bn    3. miscellanious error correctios.
             ---  bn    4. removed unused events from connection matrix.
             ---  bn    5. added BC303.

3.2          ---  bn    1. IN3_1ID003, in012.31, in026.31 
             ---  krp   1. Modifications in inNetE05S00 to handle
                           NI2 network side.
             ---  krp   2. Modified inNetE05S00 to send RelComplete
                           message instead of Release message when
                           channel Glare occurs.

3.3          --- bn      1. miscellaneous change
*********************************************************************81*/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
3.4          ---      sb   1. Miscellaneous changes.

3.5          ---      sb   1. Added functions to support point to multipoint
                              feature.
                      sb   2. Modified state matrix functions to take in a
                              additional parameter (dCb).

3.6          ---      sam  1. Added network compatibility matrix for 
                              Australia Telecom.
             ---      sam  2. Modified error message in function inNetE05S00.
             ---      sam  3. Text Changes.

3.7          in002.37 sam  1. Modified network compatibility matrix for 
                              Austell: Connect Ack in Connect Req state set
                              to T.
             ---      pk   1. Included start of the TProg timer in 
                              inNetE04S09 when a progress message is 
                              received.
             ---      pk   2. Added a check for the progInd value of the
                              Progress message received in the incoming
                              call proceeding state,in function 
                              inNetE04S09
             ---      pk   3. Modified function inNetE00S09 and function
                              inNetE02S09 to stop TProg timer if present.
             ---      sam  2. Modified network compatibility matrix for
                              Austel - Disconnect, Release and Rel.Comp
                              messages accepted in various states.

3.8          in001.38 sam  1. Modified condition for ISDN_PTMPT in func.
                              inNetE02S09 to issue connect ack.
             in002.38 sam  1. Changed mesg. compatibility matrix for Austel
                              to disallow setup ack in state 6.
                           2. Changed mesg. compatibility matrix for Austel
                              to allow disconnect in state 6.
             in009.38 sam  3. Changed causeDgn.eh.pres to PRSNT_NODEF if
                              it was being reinitialized to PRSNT_DEF
             in010.38 asa  4. Changed to fill up ces and pcb->suId in
                              the last parameter in InUiIntConInd.
             in012.38 asa  5. Removed call for function inIntPrimRelReq
                              from inNetENDS08 (This func should not be
                              called in Connect-Req state).
                           6. Fill up Ces value in the last parameter of
                              InUiIntConCfm and InUiIntCnStInd primitives.
             in013.38 asa  7. Correct the parameters passed to
                              inCallIdSearch function.
                           8. Issue a Suspend Reject message in error
                              cases on getting a Suspend message.
             ---      sam  9. Modified compatibility matrix for NTDMS100P.
                              Release is T in State 0 and SetupAck is F
                              in State 6.
             ---      sam 10. Modified inNetE06S06 to issue status with
                              cause 97 for NTDMS100P.
             ---      sam 11. Added message compatibility matrix for
                              NTMCI.
             ---      sam 12. Added message compatibility matrix for
                              NTNI.
             ---      sam 13. Added processing for NTNI and NTMCI
                              variants.
             ---      sam 14. Pass both dChan and Ces as parameters to 
                              upper layer primitives.
             ---      sam 15. Removed #ifdef cm_gen.h and cm_gen.x.
             ---      sb  16. Text changes.

3.9          ---      rsk  1. Inserted Channel Id before sending 
                              InUiIntCnStInd to the upper layer, if this 
                              is the first response to a Setup message.
                      rsk  2. Reversed the order of issuing connect confirm
                              to the upper layer and then sending connect 
                              ack message 
                           3. Corrected call to InUiIntRelInd in inNetE15S12
                              to not use parameters from ntc

3.10        in001.310  pk  1. Changed NTMCI network side matrix to receive
                              Congestion Control and Facility messages.
            in003.310  pk  2. Addition of QW header files.
            in005.310  cy  3. Changed network side matrix for BC303TMC variant.
                           4. Changed connection matrix for Status Request 
                              in state 24.
            in007.310  cy  5. Added correction for counter pcd->sts.cons and
                              pcb->sts.aveCalls.
            in009.310  cy  6. Checked ntc->updStatus before calling
                              funciton zqRunTimeUpd for update/create ntc.
            in010.310  cy  7. Fix for ZQ.
                              Set ntc->updStatus flag to NTC_CREATED
                              after sending create request to the 
                              standby.
            in011.310  pk  8. Removed Check for ETSI while checking for
                              PTMPT.
            in011.310  bb  9. Changed to send Release message instead of
                              Release Comp in case of call present state
                              and channel negotiation fails. 
            in014.310  cy 10. Removed compile time warning.
                          11. Corrected compile time flag for INLOGERR.
            in015.310  cy 12. Fix for ZQ.
                              Checked ntc->updStatus before calling
                              funciton zqRunTimeUpd for update/create ntc.
            in018.310  cy 13. Fix for ZQ. Checked return value from 
                              zqRunTimeUpd before setting ntc->updStatus  
                              to NTC_CREATED.
            in019.310  cy 14. Changed suInstId to UConnId value in 
                              inNetE15SND.
            in021.310  cy 15. Saved network number (station Id) in conEvnt
                              in inNetE05S00. Refer to TR-NWT-000444.
            in029.310 scc 16. Remove define for ISDN_PRI as it should be
                              defined in envopt.h
                      scc 17. Initialize variables.
            in036.310 bb  18. Added changes for CORE2 support
                              (TELICA: won't work, removed).
*********************************************************************91*/
