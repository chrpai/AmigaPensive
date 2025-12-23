
/*
 * Version 1.0  : works!
 * Version 1.01 : checks for user name, executes something, show # items
 * Version 1.02 : final release? fixed text centre problem
 */

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
#include <dos/notify.h>
#include <proto/pipeline.h>
#include <clib/exec_protos.h>
#include <clib/dos_protos.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <pipeutil.h>
#include <nodes.h>

struct	Library		*PipelineBase;
struct	Library		*PipeUtilBase;
struct	IntuitionBase	*IntuitionBase;

struct	Gadget		*mailgad, *nomailgad;

signed 	long		 nummail;

char			 FileName[256];

#define			 NUMARGS	5

#define			 USER		0
#define			 EXECUTE	1
#define			 MAILCMD	2
#define			 XPOS		3
#define			 YPOS		4

UBYTE			 ver[] = "$VER: PostMan 1.0 (05.08.95)";

char			 icontitle[256];

UBYTE			 template[] = "USER/A,EXECUTE/A,MAILCMD,XPOS/N,YPOS/N";

struct	IntuiText	 mailtext;

signed long
FileSize ( char *file_id )
{
      	BPTR     		 lk;
      	struct	FileInfoBlock  	*f_info;
	signed 	long		 ret = 0;

	if ( f_info = AllocDosObject ( DOS_FIB, NULL ) )
	{
	      	if ( lk = Lock ( file_id, ACCESS_READ ) )
		{
	        	if ( Examine ( lk, f_info ) )
			{
			      	if ( f_info->fib_DirEntryType < 0 )
				{
				      	ret = f_info->fib_Size;
				}
				else
					ret = -3;
			}
			else
				ret = -2;
		      	UnLock( lk ) ;
		}
		else
			ret = -1;
		FreeDosObject ( DOS_FIB, f_info ) ;
	}
	return ( ret ) ;
}

void
CheckMail ( void )
{
	signed	long	sz;

	sz = FileSize ( FileName ) ;
	if ( sz > 0 )
	{
		nummail = (signed long) (sz / (signed long) sizeof ( struct WaitingMail )) ;
	}
	else
		nummail = 0;
}

void
DoIText ( char *icontitle )
{
	if ( nummail )
	{
		sprintf ( icontitle, "%ld Message", nummail ) ;
		if ( nummail > 1 ) strcat ( icontitle, "s" ) ;
	}
	else
		strcpy ( icontitle, "No Mail" ) ;
}

void
main ( int argc, char **argv )
{
	struct	NotifyRequest 	 nr;
	struct	Gadget		*gad;
	ULONG			 success;
	BOOL			 done;
	ULONG			 Mask, Wake, IcnSB;
	struct	ConMsg		*cm;
	struct	MsgPort		*mp;
	struct	RDArgs		*Args;
	LONG			 args[NUMARGS];
	struct	SharedData	*shd;

	memset ( args, '\0', sizeof ( LONG ) * NUMARGS ) ;

	if ( Args = ReadArgs ( template, args, NULL ) )
	{
		if ( IntuitionBase = (struct IntuitionBase *) OpenLibrary ( "intuition.library", 37L ) )
		{
			if ( PipelineBase = OpenLibrary ( "zeus.library", 0L ) )
			{
				if ( PipeUtilBase = OpenLibrary ( "zmf.library", 0L ) )
				{
					if ( shd = GetShared ( ) ) 
					{
						sprintf ( FileName, "%s%s/WaitingMail.dat", shd->Paths->User, args[USER] ) ;
						SpaceToUnder ( FileName ) ;
						if ( mp = CreateMsgPort ( ) )
						{
							if ( mailgad = GetGad ( "Mail" ) )
							{
								if ( nomailgad = GetGad ( "NoMail" ) )
								{
									mailgad->Flags = GFLG_GADGHIMAGE | GFLG_GADGIMAGE | GFLG_LABELSTRING;
									mailgad->Activation = GACT_TOGGLESELECT | GACT_RELVERIFY;
									mailgad->UserData = (void *) mp;
									mailgad->GadgetText = &mailtext;
									if ( args[XPOS] && args[YPOS] )
									{
										mailgad->LeftEdge = *(LONG *)args[XPOS];
										mailgad->TopEdge = *(LONG *)args[YPOS];
									}
									else
									{
										mailgad->LeftEdge = 0;
										mailgad->TopEdge  = 0;
									}

									nomailgad->Flags = GFLG_GADGHIMAGE | GFLG_GADGIMAGE;
									nomailgad->Activation = GACT_TOGGLESELECT | GACT_RELVERIFY;
									nomailgad->UserData = (void *) mp;
									nomailgad->GadgetText = &mailtext;
									if ( args[XPOS] && args[YPOS] )
									{
										nomailgad->LeftEdge = *(LONG *)args[XPOS];
										nomailgad->TopEdge = *(LONG *)args[YPOS];
									}
									else
									{
										mailgad->LeftEdge = 0;
										mailgad->TopEdge  = 0;
									}

									CheckMail ( ) ;
					
									DetachGads ( ) ;
									if ( nummail ) 	gad = mailgad;
									else		gad = nomailgad;

									DoIText( icontitle ) ;
			
									ClearMem ( &mailtext, sizeof ( struct IntuiText ) ) ;
									mailtext.FrontPen = 1;
									mailtext.BackPen  = 2;
									mailtext.DrawMode = JAM1;
									mailtext.IText    = icontitle;

									success = AddGad ( gad ) ;
									
									mailgad->UserData = mp;
									nomailgad->UserData = mp;

									LayoutGads ( ) ;
									AttachGads ( ) ;

									if ( success )
									{
										done = FALSE;
										KillMe ( SIGBREAKF_CTRL_C ) ;
										IcnSB = ( 1L << mp->mp_SigBit ) ;
		
										Mask = SIGBREAKF_CTRL_C | IcnSB | SIGBREAKF_CTRL_E;
		
										ClearMem ( &nr, sizeof ( struct NotifyRequest ) ) ;
		
										nr.nr_Name 				= FileName;
										nr.nr_Flags 				= NRF_SEND_SIGNAL;
										nr.nr_stuff.nr_Signal.nr_Task		= FindTask ( NULL ) ;
										nr.nr_stuff.nr_Signal.nr_SignalNum	= SIGBREAKB_CTRL_E;
		
										if ( StartNotify ( &nr ) )
										{
											while ( ! done )
											{
												Wake = Wait ( Mask ) ;
												if ( Wake & IcnSB )
												{
													while ( cm = (struct ConMsg *)GetMsg ( mp ) )
													{
														switch ( cm->cm_Action )
														{
															case GAD_SELECT:
																SystemTags ( (STRPTR) args[EXECUTE], SYS_Input, NULL, SYS_Output, NULL, SYS_Asynch, TRUE, TAG_END ) ;
																break;
															case GAD_QUIT :
																done = TRUE;
																break;
														}
														ReplyMsg ( (struct Message *)cm ) ;
													}	
												}
												if ( Wake & SIGBREAKF_CTRL_C )
												{
													done = TRUE;
												}
												if ( Wake & SIGBREAKF_CTRL_E )
												{
													EndNotify ( &nr ) ;
													CheckMail ( ) ;
													StartNotify ( &nr ) ;
													DetachGads ( ) ;
													RemGad ( gad ) ;
													if ( nummail ) 	gad = mailgad;
													else		gad = nomailgad;

													DoIText( icontitle ) ;

													if ( args[XPOS] && args[YPOS] )
													{
														gad->LeftEdge = *(LONG *)args[XPOS];
														gad->TopEdge = *(LONG *)args[YPOS];
													}
													else
													{
														gad->LeftEdge = 0;
														gad->TopEdge  = 0;
													}
													ClearMem ( &mailtext, sizeof ( struct IntuiText ) ) ;
													mailtext.FrontPen = 1;
													mailtext.BackPen  = 2;
													mailtext.DrawMode = JAM1;
													mailtext.IText    = icontitle;

													success = AddGad ( gad ) ;
													gad->UserData = mp;
													LayoutGads ( ) ;
													AttachGads ( ) ;
													if ( args[MAILCMD] )
														SystemTags ( (STRPTR) args[MAILCMD], SYS_Input, NULL, SYS_Output, NULL, SYS_Asynch, TRUE, TAG_END ) ;
													if ( ! success ) done = TRUE;
												}
											}
											EndNotify ( &nr ) ;
										}
										else
											Printf ( "Could not add notification structure!\n" ) ;
			
										DontKillMe ( ) ;
										DetachGads ( ) ;
										RemGad ( gad ) ;
										LayoutGads ( ) ;
										AttachGads ( ) ;
									}
									else
										Printf ( "Could not add gadgets!\n" ) ;
									FreeGad ( nomailgad ) ;
								}
								else
									Printf ( "Could not get gadgets!\n" ) ;
								FreeGad ( mailgad ) ;
							}
							else
								Printf ( "Could not get gadgets!\n" ) ;
							DeleteMsgPort ( mp ) ;
						}
						else
							Printf ( "Could not create Message Port!\n" ) ;
					}
					else
						Printf ( "Could not get Zeus Shared Data!\n" ) ;
					CloseLibrary ( PipeUtilBase ) ;
				}
				else
					Printf ( "Could not open zmf.library!\n" ) ;
				CloseLibrary ( PipelineBase ) ;
			}
			else
				Printf ( "Could not open zeus.library!\n" ) ;
			CloseLibrary ( (struct Library *) IntuitionBase ) ;
		}
		else
			Printf ( "Could not open intuition.library!\n" ) ;
	}
	else
	{
		Printf ( "Usage: PostMan %s\n\n", template ) ;
		PutStr ( "   USER     - Name of user to check\n" ) ;
		PutStr ( "   EXECUTE  - Command to execute upon clicking on icon\n" ) ;
		PutStr ( "   MAILCMD  - Command to execute upon receiving mail\n" ) ;
		PutStr ( "   XPOS     - X Position to place icon\n" ) ;
		PutStr ( "   YPOS     - Y Position to place icon\n\n" ) ;
		exit ( 20 ) ;
	}
}
