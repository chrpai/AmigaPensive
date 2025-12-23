// Commands.h

#ifndef MBBS_COMMANDS_H
#define MBBS_COMMANDS_H

#include    <exec/types.h>
#include    <exec/memory.h>
#include    <exec/lists.h>

#include    <dos/dos.h>
#include    <dos/dosextens.h>
#include    <dos/dostags.h>

#include    "Misc.h"
#include    "Spawn.h"
#include    "Config.h"

void Logon();
void Logoff();
BOOL Type( char *);
BOOL External( char *);
BOOL Spawn( char *);
BOOL Menu( char *);
BOOL Batch( char *);
void WaitReturn();

#endif
