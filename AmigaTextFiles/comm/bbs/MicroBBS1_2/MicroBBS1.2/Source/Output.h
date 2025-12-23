// Output.h

#ifndef MBBS_OUTPUT_H
#define MBBS_OUTPUT_H

#include    <devices/serial.h>

#include    <stdarg.h>

#include    "Support.h"

void PutChar( unsigned char );
void PutString( char *);
void PutStringF( char *, ...);
void PutData( char *, long);

#endif
