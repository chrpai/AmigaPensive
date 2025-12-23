// Input.h

#ifndef MBBS_INPUT_H
#define MBBS_INPUT_H

#include    <devices/serial.h>
#include    <devices/timer.h>
#include    <exec/types.h>

#include    "Misc.h"
#include    "Support.h"

BOOL WaitChar( long);
void DGetChar( unsigned char *);
void GetChar( unsigned char *);
BOOL GetString( char *);
BOOL GetStringHidden( char *);

#endif
