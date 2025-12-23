
/*
 * FAME Door Defines
 * ~~~~~~~~~~~~~~~~~
 *
 * $VER: FAMEDoorDefines.h v1.5
 *
 * Some additional and helpfull functions which doesn't need extra
 * doorcommands. Normally macros.
 *
 * Code by: David 'Strider/tRSi' Wettig
 * E-Mail:  strider@trsi.de
 * URL:     http://www.trsi.de/inno/strider/index.html
 *
 * Copyright note: FAME and all it's contents like programs,
 *                 tools, doors, documents, texts, sources,
 *                 standards like the FIM (FAME door port standard),
 *                 the FAMEToolPort and all depending things,
 *                 the FAMECoProc standard and (data) file formats
 *                 are copyrighted © 1993 - 1997 by David Wettig.
 *
 *                 The include file you are currently reading
 *                 is also copyrighted and you are only allowed
 *                 to use it to develop own programs for the
 *                 FAME BBS System and you are *NOT* allowed
 *                 to use it for own projects like Mailbox
 *                 programs or whatever without my permission.
 */

#ifndef FAME_FAMEDOORDEFINES_H
#define FAME_FAMEDOORDEFINES_H

#include <FAME/FAMEDoorProto.h>

#define ClearFlagFiles(x) PutCommand("*",(x),0,0,NR_DeleteFlagFile);

/*
 * Clears all file flags in (x) given conference.
 * x < 1 means current conference.
 */

#endif  /* FAME_FAMEDOORDEFINES_H */

