
/*
 * FAME Including
 * ~~~~~~~~~~~~~~
 *
 * $VER: FAMEIncluding.h v1.5
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

#ifndef FAME_INCLUDING_H
#define FAME_INCLUDING_H

/*
 * As FAME door coder you needn't all includes, for compiling FAME i will.
 * You can strip all not needed includes, but i would recommend you to
 * leave them all and generate a gst file which do you can use for every
 * compile actions not only for FAME doors.
 * A gst speeds up compiling extremely!
 */

#include <dos/dostags.h>
#include <proto/asl.h>
#include <proto/graphics.h>
#include <proto/gadtools.h>
#include <proto/rexxsyslib.h>
#include <proto/icon.h>
#include <proto/dos.h>
#include <proto/utility.h>
#include <proto/diskfont.h>
#include <proto/iffparse.h>
#include <proto/layers.h>
#include <proto/intuition.h>
#include <datatypes/textclass.h>
#include <clib/macros.h>
#include <clib/dos_protos.h>
#include <clib/exec_protos.h>
#include <clib/gadtools_protos.h>
#include <clib/graphics_protos.h>
#include <clib/utility_protos.h>
#include <clib/alib_protos.h>
#include <clib/intuition_protos.h>
#include <clib/icon_protos.h>
#include <clib/asl_protos.h>
#include <clib/timer_protos.h>
#include <Devices/Console.h>
#include <Devices/ConUnit.h>
#include <devices/serial.h>
#include <hardware/cia.h>
#include <exec/types.h>
#include <exec/semaphores.h>
#include <exec/memory.h>
#include <graphics/displayinfo.h>
#include <graphics/gfx.h>
#include <graphics/gfxbase.h>
#include <intuition/intuition.h>
#include <intuition/classes.h>
#include <intuition/classusr.h>
#include <intuition/imageclass.h>
#include <intuition/gadgetclass.h>
#include <pragmas/exec_pragmas.h>
#include <pragmas/intuition_pragmas.h>
#include <pragmas/graphics_pragmas.h>
#include <pragmas/utility_pragmas.h>
#include <stdlib.h>                /* Because of exit() is a must          */
#include <string.h>                /* Because of strcpy/strcat/strlen      */
#include <time.h>                  /* Because of time() i will change this  */

#endif  /* FAME_INCLUDING_H */

