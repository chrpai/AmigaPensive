#ifndef _CL_ALERTS_H
#define _CL_ALERTS_H 1

#ifndef EXEC_ALERTS_H
#include <exec/alerts.h>
#endif

/*********************************************************************
*
*  Alert Codes of various Connectline Subsystems
*
*  Format of the alert error number:
*
*    +-+-------------+----------------+--------------------------------+
*    |D|0110101 ($35)|  Subsystem ID  |    SubSystem Specific Error    |
*    +-+-------------+----------------+--------------------------------+
*     1    7 bits          8 bits                  16 bits
*
*                        D:  DeadEnd alert flag (Not used!)
*                  $35:  AN_Unknown (Application Alert)
*         Subsystem ID:  which Connectline subsystem
*       Specific Error:  subsystem specific error code
*
*
*   Use clutil.library/CLU_Alert() to output alert codes. This
*   will generate a file report in "CONNECTLINE:alert.log"
*
**********************************************************************/

#define CLALERT_BASE AN_Unknown
#define CLALERT_MAKE_ID(subsys,specific) (CLALERT_BASE|((subsys)<<16)|(specific))

#define CLALERT_NOCLUTIL (AN_Unknown|AG_OpenLib)

/*
 *  Subsystem defines and specific error codes
 */

#define CLALERT_SUBSYS_CLUTIL 1

/*
 *  clutil.library specific error codes
 *
 *  0 - illegal InsertMode passed to CLU_NOL_AddEntry()
 *  1 - NULL NOL pointer encountered
 *  2 - NULL NOBJ pointer encountered
 *  3 - NOBJs are on different NOLS at CLU_NOL_SwapEntries()
 *
 */


#define CLALERT_SUBSYS_CL_PREFS 2

/*
 *  cl_prefs.library specific error codes
 *
 *  0 - illegal MsgPort passed to CLP_AddNotify()
 *
 */

#define CLALERT_SUBSYS_CL_ROUTE 3

/*
 *  cl_route.library specific error codes
 *
 *  0 - dead function CalcRouteFee has been called
 *
 */

#define CLALERT_SUBSYS_CLSYSTEM 4

/*
 *  clsystem.library specific error codes
 *
 *  0 - no memory for System list
 *
 */

#endif
