/* diverse Definitionen fuer die MicroBBS und den Handler. */

#include    <devices/console.h>
#include    <devices/conunit.h>
#include    <devices/serial.h>
#include    <devices/timer.h>
#include    <dos/dos.h>
#include    <dos/dosextens.h>
#include    <dos/dostags.h>
#include    <dos/rdargs.h>
#include    <exec/exec.h>
#include    <graphics/text.h>
#include    <intuition/intuition.h>

#include    <clib/alib_protos.h>
#include    <clib/console_protos.h>
#include    <clib/diskfont_protos.h>
#include    <clib/dos_protos.h>
#include    <clib/exec_protos.h>
#include    <clib/graphics_protos.h>
#include    <clib/intuition_protos.h>
#include    <clib/macros.h>
#include    <clib/misc_protos.h>
#include    <clib/timer_protos.h>

#include    <signal.h>
#include    <stdio.h>
#include    <stdarg.h>
#include    <time.h>

#include    "Strings.h"
#include    "Input.h"
#include    "Output.h"
#include    "Misc.h"
#include    "MetaXpr.h"
#include    "Config.h"
#include    "Commands.h"
#include    "Spawn.h"
#include    "Support.h"
