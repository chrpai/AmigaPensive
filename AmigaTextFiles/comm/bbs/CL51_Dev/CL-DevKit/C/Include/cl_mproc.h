#ifndef _CL_MPROC_H
#define _CL_MPROC_H

#ifndef _NO_PRAG
extern struct Library *CLMProcBase;
#pragma libcall CLMProcBase CLMProc_Process 1e 210BA9807
#pragma tagcall CLMProcBase CLMProc_ProcessTags 1e 210BA9807
#pragma libcall CLMProcBase CLMProc_Newfiles 24 001
#endif

#ifndef _NO_PROTO
LONG CLMProc_Process( STRPTR fromadr, STRPTR destadr, STRPTR subject, STRPTR msgdata, struct Box *frombox, HH headerlist, struct TagItems *tags );
LONG CLMProc_ProcessTags( STRPTR fromadr, STRPTR destadr, STRPTR subject, STRPTR msgdata, struct Box *frombox, HH headerlist, ... );
void CLMProc_Newfiles( ULONG settings );
#endif

#endif
