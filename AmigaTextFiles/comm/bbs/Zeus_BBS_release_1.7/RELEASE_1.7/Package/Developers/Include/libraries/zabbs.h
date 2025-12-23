/********************************************************************
**                                                                 **
**                 zabbs.library include file                      **
**                                                                 **
**        Copyright © 1995 CeCom. All rights reserved              **
**                                                                 **
********************************************************************/
#ifndef LIBRARIES_ZABBS_H
#define LIBRARIES_ZABBS_H

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif

#ifndef EXEC_SEMAPHORES_H
#include <exec/semaphores.h>
#endif

#ifndef EXEC_NODES_H
#include <exec/nodes.h>
#endif

#ifndef EXEC_LISTS_H
#include <exec/lists.h>
#endif

#define ZCA_STRUCTUSER 0x1
#define ZCA_NOCHKFLAG  0x2

/* --- Return codes for ZaFileAreaAccess() and ZaMsgAreaAccess() --- */
#define AXX_OK    0 // User got access
#define AXX_FLAGS 1 // Haven't got the required flags
#define AXX_LEVEL 2 // Too low/high access level
#define AXX_AGE   3 // User is too young
#define AXX_GENDER 4 // User has wrong gender

/* --- Next/Prev file area --- */
#define NPF_ALL         0x1
#define NPF_SIG         0x2
#define NPF_ZABBS_SIG   0x4

/* --- Bitmapped return code --- */
#define AMFR_OFFL_OK          0x1   // Offline flag set/cleared
#define AMFR_OFFL_NOFLAGS     0x2   // User got no offline flags in area
#define AMFR_OFFL_OUTOFMEMORY 0x4   // Out of memory
#define AMFR_OFFL_ILLEGALSZ   0x8   // Couldn't load access flags
#define AMFR_OFFL_ERRORSAVE   0x10  // Couldn't save access flags

#define AMFR_ONL_OK           0x20  // Online flag set/cleared
#define AMFR_ONL_NOUSERDATA   0x40  // Flags not found
#define AMFR_ONL_NOLINE       0x80  // No user online

#define AMFR_OK (AMFR_OFFL_OK|AMFR_ONL_OK)

/* Access flags */
#define MA_N_MASKEDOUT 0xfffffbff

/*
** Flags for ZaCheckMsgAccess
*/
#define CMA_FROMTO  0x1
#define CMA_DELETED 0x2
#define CMA_ALIAS   0x4

/* Return codes */
#define CMA_NOACCESS 0x1
#define CMA_FROMUSER 0x2
#define CMA_TOUSER   0x4

struct CountConference
{
   ULONG Low;
   ULONG High;
   ULONG User;
   ULONG Unread;
   ULONG ToUser;
};

#define ZCC_SCAN     0x1
#define ZCC_SCANPVT  0x2
#define ZCC_NOSETPNT 0x4

/*********************************************************************
**                         GENERAL CONFIG                           **
*********************************************************************/
#define ZACFG_FILE "ZEUS:Config/ZAbbs/ZAbbs.cfg"
#define ZA_VERSION 0
#define ZA_VARLEN  31

/*struct ZaLanguage
{
   struct MinNode nn;

   UBYTE *;
};*/

struct ZaViewConfCfg
{
   UBYTE FromUser;
   UBYTE ToUser;
   BOOL  Quick;
};

struct ZaConfigFlags
{
   ULONG StealthLocal : 1;
   ULONG LocalLocal   : 1;
   ULONG AvailLocal   : 1;
   ULONG SFAvail      : 1;
   ULONG Unused       : 28;
};

struct ZaConfig
{
   UWORD Version;

   ULONG UserInfoArea;
   ULONG FileInfoArea;

   UBYTE CommentName[41];

   UBYTE FileNameLen;
   UBYTE MaxLines;

   struct ZaConfigFlags Fl;

   UBYTE StealthVar[ZA_VARLEN];
   UBYTE LocalVar[ZA_VARLEN];
   UBYTE AvailVar[ZA_VARLEN];

   struct ZaViewConfCfg vcc;
};

struct HistNode
{
   struct MinNode nn;

   struct List    History;

   ULONG          LineNum;
   ULONG          NumLines;
};

struct ZaMsgNode
{
   struct MinNode nn;
   ULONG          Number;
};

struct ZaLine
{
   struct MinNode nn;
   ULONG          Number;

   struct SignalSemaphore hs;
   struct List            History;

   struct SignalSemaphore ms;
   struct List            Msgs;
   BOOL                   AllUnread;
};

struct ZaShared
{
   struct SignalSemaphore ss;    // Use this before access
   struct ZaConfig        zc;    // Configuration

   struct SignalSemaphore ls;
   struct List            lnls;
};

struct ZaWindowPos
{
   UWORD x;
   UWORD y;
};

#endif
