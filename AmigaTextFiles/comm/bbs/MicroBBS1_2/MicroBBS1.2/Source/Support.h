// Support.h

#ifndef MBBS_SUPPORT_H
#define MBBS_SUPPORT_H

#include    <exec/types.h>

#include    <time.h>

#include    "Config.h"
#include    "Spawn.h"
#include    "Commands.h"

BOOL DoBatch( struct ConfigNode *);
BOOL DoCommand( char *, char *);
BOOL ReadUser( char *);

void ClearSpawnPort();
void CompleteString( char *, char *);
void ReadStats();
void SetBBSPrompt();
void SystemError();
void WriteStats();
void WriteUser();

#endif

