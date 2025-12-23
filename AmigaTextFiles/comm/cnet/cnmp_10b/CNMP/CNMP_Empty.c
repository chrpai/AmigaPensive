/*
 *  .------------------------------------------------------------------.
 *  | CNetMultiPort v1.0 COPYRIGHT © 1995 METAL SOFTWARE AKA MetalSoft |
 *  `------------------------------------------------------------------'
 *        Original EMPTY.C copyright © 1993	Perspective Software
 */


#include <exec/ports.h>
#include <exec/nodes.h>
#include <exec/exec.h>
#include <ctype.h>
#include <time.h>
#include <devices/timer.h>
#include <libraries/easyrexx.h>
#include <cnmp/cnmp.h>
#include <clib/cnmp_protos.h>

/**************************************************************************
 * REQUIRED CNetMultiPort global variables
 **************************************************************************/
UBYTE *vers="$VER:v1.0 YourPrgName (DD-MON-YR)";

struct	CnmpCluster		*MyCnmpCluster=NULL;		/* the meat of CNMP! - see
                                                   CNMP.H for structure
                                                   members */

BOOL							PORTPERMISSION=TRUE;		/* portpermission defaults to FALSE */



/**************************************************************************
 * CNMP ARexx support! - SEE CNMP_AREXX.DOC for more info on
 * ALTERING/ADDING/REMOVING/HANDLING AREXX COMMANDS.
 **************************************************************************/
char							RexxPortName[40];			/* must be at least CNMP_BASEPORT + 10 */
struct	Library			*EasyRexxBase=NULL;
struct	ARexxContext	*RexxContext=NULL;

/* define a context number for each ARexx operation you need */
#define AREXX_COMMAND   500

struct ARexxCommandTable rxtbl[] =
	{
	AREXX_COMMAND,			"SOMECOMMAND",		NULL,			NULL,
	TABLE_END,
	};
/**************************************************************************
 * END OF AREXX IMPLEMENTATION - SEE CNMP_AREXX.DOC for more info on
 * ALTERING/ADDING/REMOVING/HANDLING AREXX COMMANDS.
 **************************************************************************/




/************************************************************************
 *                         CNet Global Symbols                          *
 ************************************************************************/
struct	MsgPort  *replyp;
struct	CPort    *cport;
struct	CMessage  cmess;
struct	MainPort  *myp;
struct	PortData  *z;
char		**bm;
struct	Library *CNetBase = NULL;
struct	SignalSemaphore *SEM;


/************************************************************************
 *                         Entry/Start routine                          *
 ************************************************************************/
void main( int argc, char **argv )
{
	BOOL done=FALSE;
	char PortRange[50];
	struct RemotePort *TempRemotePort=NULL;
	struct CNMPMsg *UserCNMPMsg=NULL;
	LONG MsgMask=0;
	char OtherPortName[20];


	/* ----- Cnet comm stuff ----- */
	if( argc<2 || !(cport = (struct CPort *)FindPort( argv[1] )) )
		{
		printf("This is a CNet C program.\n");
		exit(0);
		};
	if( !(replyp = CreatePort( 0,0 )))
		{
		exit(0);
		};
	cmess.cn_Message.mn_ReplyPort   = replyp;
	cmess.cn_Message.mn_Length      = sizeof( struct CMessage );
	cmess.cn_Message.mn_Node.ln_Name= "cstuff";
	if( cport->ack != 30 )
		{
		cport->ack = 1;
		LoadError();
		};
	cport->ack	=	0;
	z				=	cport->zp;
	myp			=	cport->myp;
	SEM			=	myp->SEM;
	bm				=	z->bm;
	if( !(CNetBase = OpenLibrary( "cnet.library",0 )) )
		GetOut("", 0);



	/**************************************************************************
	 * CNMP(tm) initialization begins here!                                   *
	 *     ***** DO NOT CHANGE ANYTHING IN THIS SECTION OF CODE!! *****       *
	 * See below for details!                                                 *
	 **************************************************************************/
	if( ! (MyCnmpCluster=InitCNMP(z, "CNMPConf", sizeof(struct PortVars))) )
		{
		GetOut("", 0);
		}

	/* Create the EasyRexx port */
	if(EasyRexxBase=OpenLibrary(EASYREXXNAME, EASYREXXVERSION))
		{
		/* set the arexx port name to CNMPTESTREXXn where n=port number */
		sprintf(RexxPortName, "%sREXX%d", MyCnmpCluster->CNMP_BASEPORT, z->InPort);
		RexxContext = AllocARexxContext(ER_Portname, RexxPortName,
 	                                   ER_CommandTable, rxtbl,
 	                                   TAG_DONE) ;
		}
	else
		{
		PutText("ERROR 504: arexx port not opened!\n");
		}
/**************************************************************************
 *** end of CNMP initalization. DO NOT CHANGE ANY CODE ABOVE THIS POINT ***
 **************************************************************************/


	/* User code begins here... */


	/* get back to CNet BBS - MUST CALL GetOut() on exit!!! */
	GetOut("", 0);
}



/**************************************************************************
 *       Routine called if Load error (wrong CNet version, etc,...        *
 **************************************************************************/
void LoadError( void )
{
	DeletePort( replyp );
	exit(0);
}


/************************************************************************
 *                          MetalSoft EXIT code                         *
 ************************************************************************/
void GetOut( char *text, short errornum )
{
	struct CNMPMsg *Msg = NULL;

	/* bring the rexx port down */
	if(RexxContext)
		FreeARexxContext(RexxContext);

	NotifyCNMPRemove(MyCnmpCluster);
	Forbid();

	if(MyCnmpCluster)
		{
		/*
       * get all messages that may be waiting and either free them if they
       * are NT_REPLYMSG or ReplyMsg() them if they are messages from
       * remote ports
       */
		while(Msg=(struct CNMPMsg *)GetMsg(MyCnmpCluster->cc_msgport))
			{
			if(Msg->Msg.mn_Node.ln_Type == NT_REPLYMSG)
				{
				FreeMem(Msg, sizeof(struct CNMPMsg));
				}
			else
				{
				ReplyMsg((struct Message *)Msg);
				}
			}
		}

	/* free linked list of ports */
	FreeCNMPList(MyCnmpCluster);
	if(MyCnmpCluster->cc_msgport)
		{
		RemPort(MyCnmpCluster->cc_msgport);
		DeleteMsgPort(MyCnmpCluster->cc_msgport);		/* delete our message port */
		}
	MyCnmpCluster->cc_msgport=NULL;
	Permit();
	CleanupTimeout(MyCnmpCluster);
	if(MyCnmpCluster->cc_iotr->tr_node.io_Device)
		{
		CloseDevice((struct IORequest *)MyCnmpCluster->cc_iotr);
		MyCnmpCluster->cc_iotr->tr_node.io_Device = NULL;
		}
	if(MyCnmpCluster->cc_iosink)
		{
		DeleteMsgPort(MyCnmpCluster->cc_iosink);
		MyCnmpCluster->cc_iosink = NULL;
		}

	FreeUserMsgs(MyCnmpCluster);

	if( (errornum > 0) || (strlen(text)) )
		{
		PutText(text);
		}

	SetDoing(oldDOING);
	ShutDown( NULL );
	DeletePort( replyp );
	exit(errornum);
}



void ShutDown( char *spawn )			/* another file to run? */
{
	if( spawn )
		strcpy( z->CSpawn, spawn );

	CallHost( 0 );
}



void CallHost( UBYTE c )
{
	cmess.command = c;
	PutMsg  ( (struct MsgPort *)cport, (struct Message *)&cmess );
	WaitPort( replyp );
	GetMsg  ( replyp );
}



void PutText( char *text )
{
	cmess.arg1 = (ULONG)text;	/* text to print		*/
	CallHost( 1 );
}



void PutA( void )
{
	PutText( z->ABuffer );
}




/**************************************************************************
 *		ENTERLINE FLAGS:                                                    *
 *		    1:	All capitalized                                            *
 *		    2:	FILENAME.  Don't allow =":;/ or asterisk                   *
 *		    4:	Begin with existing z.InBuffer                             *
 *		    8:	Chop leading spaces                                        *
 *		   16:	Force 1st letter of word caps                              *
 *		   32:	Force all others lower case                                *
 *		   64:	Numeric input only                                         *
 *		  128:	Print input box (.)                                        *
 *		  256:	DO allow MCI                                               *
 *		  512:	HANDLES/SPECIAL.  Don't allow ^_`{|}~@                     *
 *		 1024:	Exit for . or / at beginning of line                       *
 *		 2048:	Exit for backspace at beginning of line                    *
 *		 4096:	Do not allow OLM's to appear while editing                 *
 *		 8192:	Allow Chat break in at this prompt. COMMAND PROMPT.        *
 *		16384:	Don't allow SPACE, either                                  *
 *		32768:	DON'T ALLOW MOVEMENT                                       *
 **************************************************************************/
int EnterLine( UBYTE len, USHORT flags, char *prompt )
{
	cmess.arg1 = (ULONG)len;	/* how many chars max to input	*/
	cmess.arg2 = (ULONG)flags;	/* 1=UpperCase			*/
	cmess.arg3 = (ULONG)prompt;	/* text to print before input	*/
	CallHost( 2 );			/* result is in z->InBuffer	*/
	return( (int)strlen( z->InBuffer ));
}



char OneKey( void )			/* Stop until a key is pressed	*/
{
	CallHost( 3 );
	return( (char)cmess.result );
}



void EnterPassword( UBYTE len )
{
	cmess.arg1 = (ULONG)len;	/* max number of characters */
	CallHost( 4 );
}



long CommonCommands( void )		/* Check z->InBuffer for Chat, OLM, etc */
{
	CallHost( 5 );
	return( (long)cmess.result );
}



UBYTE ReadFile( char *path, UBYTE flags )
{
	cmess.arg1 = (ULONG)path;
	cmess.arg2 = (ULONG)flags;	/* 1 = print File Not Found	*/
	CallHost( 6 );
	return( (UBYTE)cmess.result );		/* FALSE if File Not Found	*/
}



void SetDoing( char *what )
{
	cmess.arg1 = (ULONG)what;
	CallHost( 7 );
}



void CallEditor( short max, short inlines )
{
	cmess.arg1 = (ULONG)max;	/* Maximum number of lines (250)*/
	cmess.arg2 = (ULONG)inlines;	/* TRUE/FALSE use existing _edbuff? */
	CallHost( 8 );
}



UBYTE ReadGraphics( char *path, char flags )
{
	cmess.arg1 = (ULONG)path;
	cmess.arg2 = (ULONG)flags;	/* 1 = print File Not Found	*/
	CallHost( 9 );
	return( (UBYTE)cmess.result );		/* FALSE if File Not Found	*/
}



void MakeDate( struct IsDate *date, char *output )
{
	cmess.arg1 = (ULONG)date;
	cmess.arg2 = (ULONG)output;
	CallHost( 10 );
}



UBYTE ReadAccount( short id, struct UserData *user )
{
	cmess.arg1 = (ULONG)id;
	cmess.arg2 = (ULONG)user;
	CallHost( 11 );
	return( (UBYTE)cmess.result );
}



UBYTE SaveAccount( struct UserData *user, short id )
{
	cmess.arg1 = (ULONG)user;
	cmess.arg2 = (ULONG)id;
	CallHost( 12 );
	return( (UBYTE)cmess.result );
}



UBYTE AddCharge( short n, short a )
{
	cmess.arg1 = (ULONG)n;
	cmess.arg2 = (ULONG)a;
	CallHost( 13 );
	return( (UBYTE)cmess.result );
}



UBYTE CheckBalance( short n, short a )
{
	cmess.arg1 = (ULONG)n;
	cmess.arg2 = (ULONG)a;
	CallHost( 14 );
	return( (UBYTE)cmess.result );
}



int EnterText( char firstchar, short maxchars, short perline, short maxlines )
{
	cmess.arg1 = (ULONG)firstchar;
	cmess.arg2 = (ULONG)maxchars;
	cmess.arg3 = (ULONG)perline;
	cmess.arg4 = (ULONG)maxlines;
	CallHost( 15 );
	return( (int)cmess.result );
}



long ConferenceWait( short a )
{
	cmess.arg1 = (ULONG) a;
	CallHost( 16 );
	return( (long)cmess.result );
}



void CheckChanges( void )
{
	CallHost( 17 );
}



long ConvertAccess( char *s )
{
	cmess.arg1 = (ULONG)s;
	CallHost( 18 );
	return( (long)cmess.result );
}



long GetFree( char *s, UBYTE q )
{
	cmess.arg1 = (ULONG)s;
	cmess.arg2 = (ULONG)q;
	CallHost( 19 );
	return( (long)cmess.result );
}



short FindAccount( char *a, struct UserData *b )
{
	cmess.arg1 = (ULONG)a;
	cmess.arg2 = (ULONG)b;
	CallHost( 20 );
	return( (short)cmess.result );
}



/**************************************************************************
 * forces CNet to update it's keyboard buffers etc,..
 **************************************************************************/
void CheckFlowControl( void )
{
	CallHost( 21 );
}



long ListDir( UBYTE a, UBYTE b, struct IsDate *c )
{
	cmess.arg1 = (ULONG)a;
	cmess.arg2 = (ULONG)b;
	cmess.arg3 = (ULONG)c;
	CallHost( 22 );
	return( (int)cmess.result );
}



UBYTE FileOLM( short a, int b )
{
	cmess.arg1 = (ULONG)a;
	cmess.arg2 = (ULONG)b;
	CallHost( 23 );
	return( (UBYTE)cmess.result );
}



UBYTE Rnext( void )
{
	CallHost( 24 );
	return( (UBYTE)cmess.result );
}



void ParseCommandLine( UBYTE numargs )
{
	cmess.arg1 = (ULONG)numargs;
	CallHost( 25 );
}



short FindCommand( short num )
{
	cmess.arg1 = (ULONG) num;
	CallHost( 26 );
	return( (short)cmess.result );
}



void ReadMessagePoint( char *a, long b )
{
	cmess.arg1 = (ULONG) a;
	cmess.arg2 = (ULONG) b;
	CallHost( 27 );
}



void EditMessage( char *file )
{
	cmess.arg1 = (ULONG) file;
	CallHost( 28 );
}



void LoadText( BPTR fh )
{
	cmess.arg1 = (ULONG) fh;
	CallHost( 29 );
}



char WaitForInput( long mics )
{
	/* 1000000 mics = 1 second */

	cmess.arg1 = (ULONG) mics;
	CallHost( 31 );
	return( (char)cmess.result );
}



void AddCredits( UBYTE justadd, short m )
{
	cmess.arg1 = (ULONG)justadd;
	cmess.arg2 = (ULONG)m;
	CallHost( 33 );
}



UBYTE SelectAndDownload( char *file, UBYTE now )
{
	cmess.arg1 = (ULONG)file;
	cmess.arg2 = (ULONG)now;
	CallHost( 39 );
	return( (UBYTE)cmess.result );
}



/************************************************************************
 *	file: the ".vde" filename, without the ".vde"!                       *
 *	data: pointer to the structure you are going to edit                 *
 *	size: structure length in bytes                                      *
 *                                                                      *
 *	returns:	TRUE  if structure has been changed                         *
 *				FALSE otherwise                                             *
 ************************************************************************/
short VisualDataEditor( char *file, void *data, long size )
{
	cmess.arg1 = (ULONG)file;
	cmess.arg2 = (ULONG)data;
	cmess.arg3 = (ULONG)size;
	CallHost( 40 );
	return( (short)cmess.result );
}



void WriteLog( short n, char *text1, char *text2 )
{
	cmess.arg1 = (ULONG)n;
	cmess.arg2 = (ULONG)text1;
	cmess.arg3 = (ULONG)text2;
	CallHost( 41 );
}



/************************************************************************
 *	In preparation for an ExtUpload, this function                       *
 *	sets the minimum number of free bytes to maintain on the             *
 *	drive.                                                               *
 ************************************************************************/ 
void ExtSetMinFree( long free )
{
	cmess.arg1 = (ULONG)free;
	CallHost( 42 );
}



/************************************************************************
 *	In preparation for an ExtDownload or an ExtUpload, this function     *
 *	sets the protocol to be used.  If you send NULL, it will allow the   *
 *	user to choose his OWN protocol.                                     *
 *                                                                      *
 *	Otherwise, you may select 'a' to be the first letter of a valid      *
 *	system protocol (from BBSPROTO file), such as 'x', 'z', etc.         *
 *                                                                      *
 *	TRUE will be returned if a protocol is selected and ready, FALSE     *
 *	if there is a problem.                                               *
 ************************************************************************/
UBYTE ExtSetProtocol( char a )
{
	cmess.arg1 = (ULONG)a;
	CallHost( 43 );
	return( (UBYTE)cmess.result );
}



/* 
	This routine allows the user to download the SINGLE file specified
	by the FULL PATH 'args'.

	Currently, NULL is always returned.
*/
char *ExtDownload( char *args )
{
	cmess.arg1 = (ULONG)args;
	CallHost( 44 );
	return( (char *)cmess.result );
}



/*
	This routine allows the user to upload the file specified by
	'args'.  The path for uploading will be taken from the path
	in 'args'.  If you do NOT specify a path, the file(s) will
	appear in the user's HOME directory.

	Note that with batch protocols like ZMODEM, the filename(s) are
	taken from the header packet information, and may NOT be the
	same as what you have requested the user upload.  For this reason,
	you should have uploads occur in a TEMP directory, and search that
	directory yourself for new files.

	Currently, NULL is always returned.
*/
char *ExtUpload( char *args )
{
	cmess.arg1 = (ULONG)args;
	CallHost( 45 );
	return( (char *)cmess.result );
}




short compstra( char *s, char *t )
{
	for( ; tolower(*s) == tolower(*t); s++, t++)
		if( !*s ) return 0;

	return (short)( tolower(*s)-tolower(*t) );
}



UBYTE PutQ( char *a )
{
	PutText( a );
	return (UBYTE)(z->MCIcreg[0][0]=='1') ;
}



void DoReturn( void )
{
	PutText("\n");
}



void MakeEd( char *path )
{
	sprintf( path, "%s_edbuff%d", myp->gc.ZIPpath, z->InPort );
}



void DeleteEd( void )
{
	char	filename[80];

	MakeEd    ( filename ) ;
	DeleteFile( filename ) ;
}



BPTR OpenEd( long mode )
{
	char	filename[80];

	MakeEd( filename );

	return Open( filename, mode );
}



void PrepEditor( BPTR fp )
{
	BPTR	kp;
	char	buff[100];

	if( fp ) {
		if( kp = OpenEd( MODE_NEWFILE ) ) {
			while( FGets( fp, buff, 82 ) && buff[0]!=26 )
			       FPuts( kp, buff     ) ;

			Close( kp );
		}
	}
	else	DeleteEd();
}



void SaveEditor( BPTR fp, UBYTE eof )
{
	BPTR	kp;
	char	buff[100];

	if( kp = OpenEd( MODE_OLDFILE ) ) {
		while( FGets( kp, buff, 82 ) && buff[0]!=26 )
		       FPuts( fp, buff     ) ;

		Close( kp );

		DeleteEd();
	}

	if( eof ) FPuts( fp, "\032\n" );
}




/**************************************************************************
 * This function not part of CNMP - this is required for EasyRexx
 **************************************************************************/
void HandleRexx( void )
{
	UBYTE ResultString[255];
	LONG ResultLong=0;

	ResultString[0]='\0';
	ResultLong=0;

	if(GetARexxMsg(RexxContext))
		{
		switch(RexxContext->id)
			{
			case AREXX_COMMAND:
				/*
             * received message from ARexx - DON'T FORGET TO CHANGE
             * AREXX_COMMAND to your own Context - See CNMP.guide for details
             */
				break;
			}
		if(strlen(ResultString))
			ReplyARexxMsg(RexxContext, ER_ResultString, ResultString, TAG_DONE);
		else if(ResultLong)
			ReplyARexxMsg(RexxContext, ER_ResultLong, ResultLong, TAG_DONE);
		else
			ReplyARexxMsg(RexxContext, TAG_DONE);
		}
}




/**************************************************************************
 *
 *
 *             ******* BONUS METALSOFT FUNCTIONS BELOW *******
 *
 *
 **************************************************************************/




/**************************************************************************
 * CheckDigits() - Returns true if string is ALL digits, false otherwise  *
 **************************************************************************/
BOOL CheckDigits( char *str )
{
	BOOL status=TRUE;

	while(*str != '\0')
		{		
		if(!isdigit(*str))
			{
			/* if you want to speed the routine up, "return FALSE" here rather. */
			/* than just setting a flag. For me, speed isn't a concern here.    */
			status=FALSE;
			};
		str++;
		}
	return status;
}


/**************************************************************************
 * Note that this routine does not do sanity checking on the length of
 * the string buffer passed to it.  It shouldn't matter though because we
 * are attempting to SHORTEN it, not lengthen it.
 **************************************************************************/
void RemoveLeadingSpace( char *str )
{
	char *retstr;

	/* advance past whitespace */
	while( (isspace(*str)) && (*str != '\0') )
		str++;

	while(*str != '\0')
		{
		*retstr=*str;
		retstr++;
		str++;
		}
	*retstr='\0';
}




BOOL FileExists(char *Name)
{
   BPTR FileLock;

   if (FileLock = Lock(Name, SHARED_LOCK))
      {
         UnLock(FileLock);
         return TRUE;
      }

   return FALSE;
}

/**************************************************************************
 * Add keys to CNet keybuffer of current port
 **************************************************************************/
void AddKeyBuffer( char *c )
{
	while( *c )
		{
		z->KBuff[ z->KB2++ ] = *c;
		c++;
		}
}
