
/*
** PostMsg v1.0 -- (c) 1995 Nick Loman
**
** This source is freely distributable and may be edited, added to, rewritten,
** compromised and molested in any way you see fit. I just hope it helps.
*/

/* TabSize 3 */

#include <exec/types.h>
#include <exec/memory.h>
#include <exec/semaphores.h>
#include <exec/nodes.h>
#include <exec/lists.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/intuition.h>
#include <dos/dos.h>
#include <dos/rdargs.h>
#include <dos/dosextens.h>
#include <dos/dostags.h>
#include <proto/pipeline.h>
#include <clib/exec_protos.h>
#include <clib/dos_protos.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <pipeutil.h>

#define NUMARGS		7

#define FROM			0
#define TO				1
#define SUBJECT		2
#define TEXTFILE		3
#define AREA			4
#define ADDRESS		5
#define PRIVATE		6

char						 Usage[] = "FROM/A,TO/A,SUBJECT/A,TEXTFILE/A,AREA/A/N,ADDRESS,PRIVATE/S";

struct	Library		*PipelineBase;
struct	Library		*PipeUtilBase;

void
ChangeEnd ( UBYTE *cp )
{
	int	 Len = 0;

	while ( *cp != '\0' )
	{
		if ( *cp == '\n' )
		{
			if ( Len > 75 )
			{
				cp++;
				if ( *cp == '\n' )
				{
					*cp = '\r';
					cp--;
					*cp = '\r';
					cp += 2;
				}
				else
				{
					cp--;
					*cp = ' ';
				}
			}
			else
			{
				cp++;
				if ( *cp == '\n' )
				{
					*cp = '\r';
					cp--;
					*cp = '\r';
					cp += 2;
				}
				else
				{
					cp--;
					*cp = '\r';
					cp++;
				}
			}
			Len = 0;
		}
		Len++;
		cp++;
	}
}

void
main ( int argc, char **argv )
{
	struct	RDArgs				*Args;
	long			 					 args[NUMARGS];
	struct	msgarea				*area;
	struct	MsgGlobals			*gl;
	BPTR			 					 fh;
	int			 					 i;
	UBYTE								*txt;
	struct	WriteMsgStruct		 wms;
	struct	Executable						*exe;
	ULONG								 mem;
	char								 Buffer[256];
	ULONG								 sz;

	for ( i = 0; i < NUMARGS; i ++ )
		args[i] = NULL;

	if ( Args = ReadArgs ( Usage, args, NULL ) )
	{
		if ( PipelineBase = OpenLibrary ( "zeus.library", 0L ) )
		{
			if ( PipeUtilBase = OpenLibrary ( "zmf.library", 0L ) )
			{
				if ( gl = GetMsgGlobals ( ) )
				{
					if ( area = GetMsgAreaData ( (ULONG) *(LONG *)args[AREA] ) )
					{
						ClearMem ( &wms, sizeof ( struct WriteMsgStruct ) ) ;

						strncpy ( wms.From,    (UBYTE *)args[FROM],    35 ) ;
						strncpy ( wms.To,      (UBYTE *)args[TO],      35 ) ;
						strncpy ( wms.Subject, (UBYTE *)args[SUBJECT], 71 ) ;

						memcpy ( &wms.FromAddress, &area->address, sizeof ( struct FidoAddress ) ) ;
						if ( area->basetype == BT_NETMAIL )
						{
							if ( ! ParseFidoAddr ( (UBYTE *)args[ADDRESS], &wms.DestAddress ) )
							{
								Printf ( "FidoNet address '%s' is invalid!\n", args[ADDRESS] ) ;
							}
						}
						if ( args[PRIVATE] )
						{
							wms.Flags = MI_PRIVATE;
						}
						wms.MessageArea = (ULONG) *(LONG *)args[AREA];

						if ( mem = SetFidoHeaders ( &wms ) )
						{
							if ( area->exe_cmd )
							{
								if ( exe = GetExecutable ( area ) )
								{
									sprintf ( Buffer, exe->Name, args[TEXTFILE] ) ;

									SystemTags ( Buffer, NP_StackSize, exe->Stack, NP_Priority, exe->Pri, TAG_DONE ) ;
								}
							}
							if ( ( sz = FileSize ( (char *) args[TEXTFILE] ) ) > 0 )
							{
								if ( txt = AllocMem ( sz + 1, MEMF_CLEAR ) )
								{
									if ( fh = Open ( (char *) args[TEXTFILE], MODE_OLDFILE ) )
									{
										if ( ( Read ( fh, txt, sz ) ) > 0 )
										{
											ChangeEnd ( txt ) ;
											wms.Text = txt;

											if ( ( SaveMessage ( &wms ) ) > 0 )
											{
												Printf ( "PostMessage: Saved Message from [%s] to [%s] Successfully!\n", args[FROM], args[TO] ) ;
											}
											else
												Printf ( "PostMessage: SaveMessage failed!\n" ) ;
										}
										Close ( fh ) ;
									}
									FreeMem ( txt, sz + 1 ) ;
								}
							}
							else
								Printf ( "Could not access message text file '%s'.\n", args[TEXTFILE] ) ;

							if ( wms.FidoText )
								FreeMem ( wms.FidoText, mem ) ;
						}
						else
							Printf ( "Out of Memory!\n" ) ;
					}
					else
						Printf ( "No such Message Area '%ld'.\n", *(LONG *)args[AREA] ) ;
				}
				else
					Printf ( "MsgControl is not running!\n" ) ;
			}
			else
				Printf ( "Cannot open zmf.library!\n" ) ;
		}
		else
			Printf ( "Cannot open zeus.library!\n" ) ;
		FreeArgs ( Args ) ;
	}
	else
		Printf ( "Usage: PostMsg %s\n", Usage ) ;
}
