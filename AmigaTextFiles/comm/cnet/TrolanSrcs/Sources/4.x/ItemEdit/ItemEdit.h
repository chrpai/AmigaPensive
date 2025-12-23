/*
** CNet/4 Item Editor
**
** Last updated: Sun Jun 28 13:15:01 1998
*/

#ifndef _ITEMEDIT_H
#define _ITEMEDIT_H

#include <libraries/bgui.h>
#include <libraries/bgui_macros.h>
#include <proto/bgui.h>

enum
{
   IE_LISTVIEW = 1,
   IE_TABS,
   IE_QUIT,
   IE_DELETE,
   IE_SAVE,
   IE_ABOUT,
   IE_TITLE,
   IE_SIZE,
   IE_PARTITION,
   IE_PARTITION_INT,
   IE_LAST_DL_DATE,
   IE_PURGE_DATE,
   IE_DL_AFTER_DATE,
   IE_DL_BEFORE_DATE,
   IE_DLS,
   IE_CPS,
   IE_FILE_CHARGE,
   IE_FILE_CREDITS,
   IE_BYTE_CHARGE,
   IE_BYTE_CREDITS,
   IE_OVERRIDE_CHECK,
   IE_FILE_PAYBACK,
   IE_BYTE_PAYBACK,
   IE_FILE_COST,
   IE_BYTE_COST,
   IE_PURGE,
   IE_ASCIIGRAB,
   IE_INTEGRETY,
   IE_OFFLINE,
   IE_XFORM,
   IE_VIRUS_CHECKED,
   IE_FREE_DL,
   IE_NOTIFY_UL,
   IE_DISABLE_RESPONSES,
   IE_FAVORITE,
   IE_VOTELINK,
   IE_DEL_PURGE,
   NUM_GADS
};

#define ACCESS_MIN 0
#define ACCESS_MAX 31

#define PARTITION_MIN 0
#define PARTITION_MAX 31

#endif /* _ITEMEDIT_H */
