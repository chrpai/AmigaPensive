
/**********************************************************************/
/*                           ConvMsgToText                            */
/*                                                                    */
/*                            Version 1.0                             */
/*                                                                    */
/*             © 1994 Nick Loman & Pipeline Developments.             */
/*                                                                    */
/* This source is declared in the Public Domain. Please feel free to  */
/*         use this code as a part of your Pipeline program!          */
/**********************************************************************/


#include <exec/types.h>
#include <exec/memory.h>
#include <exec/lists.h>
#include <exec/nodes.h>

#include <dos/dos.h>
#include <dos/dosextens.h>
#include <dos/rdargs.h>

#include <rexx/storage.h>

#include <proto/exec.h>
#include <proto/dos.h>

#include <clib/exec_protos.h>
#include <clib/dos_protos.h>

#include <plfile.h>
#include <plmsg.h>
#include <pipeline.h>
#include <pipeutil.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define	 NUMARGS		6

#define  NUMBERS		0
#define	 AREA			1
#define  FILENAME		2
#define  NOHEAD			3
#define  NOTEXT			4
#define  NOKLUDGES		5

void		main 		( int, char ** ) ;
int __saveds	RangeFunc 	( ULONG, ULONG * ) ;
void		WriteMessage	( struct WholeMessage * ) ;
void		WriteStr 	( BPTR, ULONG, char * ) ;
void		WriteText 	( char * ) ;

char	 VersionString[]	= "$VER: 1.0";