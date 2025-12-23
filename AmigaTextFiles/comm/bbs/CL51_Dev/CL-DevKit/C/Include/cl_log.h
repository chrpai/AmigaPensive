#ifndef _CL_LOG_H
#define _CL_LOG_H

/*
**   cl_log.library 3.0
**/

/* Loglevel */

#define LOGL_DEBUG		50
#define LOGL_DETAILED	40
#define LOGL_VERBOSE	30
#define LOGL_STANDARD	20
#define LOGL_ERROR		10

/* SAS/C pragmas */

extern struct Library *CLLogBase;
#pragma libcall CLLogBase CLLOG_CloseLog 1e 801
#pragma libcall CLLogBase CLLOG_DeleteLog 24 801
#pragma libcall CLLogBase CLLOG_AppendLogEntry 2a 9802
#pragma libcall CLLogBase CLLOG_UpdateLogPrefs 30 0
#pragma libcall CLLogBase CLLOG_AddLogA 36 A98004

/* ANSI prototypes */

void CLLOG_CloseLog( STRPTR logid );
void CLLOG_DeleteLog( STRPTR logid );
void CLLOG_AppendLogEntry( STRPTR logid, APTR logdata );
void CLLOG_UpdateLogPrefs( void );
void CLLOG_AddLogA( int loglevel, STRPTR logid, STRPTR text, APTR args );
void CLLOG_AddLog( int loglevel, STRPTR logid, STRPTR text, ... );

#endif
