/*
 * ConnectLine: Allgemeine Typen und Konstanten
 *
 * $RCSfile: Types.h $
 * $Revision: 1.1 $
 * $Date: 1994/02/19 17:05:14 $
 *
 * $Author: balzer $
 * $Locker: balzer $
 * $State: Exp $
 *
 * $Log: Types.h $
 * Revision 1.1  1994/02/19  17:05:14  balzer
 * Initial revision
 */

#ifndef _CL_Types_H
#define _CL_Types_H

#include <exec/types.h>
#include <exec/nodes.h>
#include <exec/lists.h>
#include <exec/ports.h>
#include <dos/dos.h>

typedef struct Node Node;
typedef struct List List;
typedef struct MinNode MinNode;
typedef struct MinList MinList;

typedef struct Message Message;

typedef LONG bool;
typedef ULONG ulong;
typedef USHORT ushort;
typedef USHORT uword;
typedef UBYTE ubyte;
typedef APTR aptr;
typedef BPTR bptr;

typedef struct IOExtSer IOExtSer;

typedef struct TagItem TagItem;
typedef struct Hook Hook;

typedef struct Window Window;
typedef struct WBArg WBArg;
typedef struct AppMessage AppMessage;
typedef struct FileInfoBlock FileInfoBlock;

#endif
