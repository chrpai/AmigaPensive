/************************************************************************/
/*									*/
/*	CNet AMIGA 3.0  C language interface routines and examples	*/
/*									*/
/*	© 1993	Perspective Software ... this code may be freely	*/
/*		ditributed to and used by registered CNet owners	*/
/*		EXCLUSIVELY.  Other distribution is in violation 	*/
/*		of copyright laws.					*/
/*									*/
/************************************************************************/

void CallHost( UBYTE c );
void ShutDown( char *spawn );

struct MsgPort  *replyp;	/* Some commnunication details ...	*/
struct CPort    *cport;
struct CMessage  cmess;

struct	MainPort  *myp;		/* Pointer to CNet port--ALL info!	*/
struct	PortData  *z;

char	**bm;
struct	Library *CNetBase = NULL;
struct	SignalSemaphore *SEM;
				/* put your other GLOBALS here		*/


void main( int argc, char **argv )
{
	if( argc<2 || !(cport = (struct CPort *)FindPort( argv[1] )) ) {
		printf("This is a CNet C program.\n");
		exit(0);
	}

	if( !(replyp = CreatePort( 0,0 )))
		exit(0);

	cmess.cn_Message.mn_ReplyPort   = replyp;
	cmess.cn_Message.mn_Length      = sizeof( struct CMessage );
	cmess.cn_Message.mn_Node.ln_Name= "cstuff";

	if( cport->ack != 30 ) {	/* right CNet version running? */
	    cport->ack = 1;
	    goto err;
	}

	cport->ack = 0;

	z		=	cport->zp;
	myp		=	cport->myp;
	SEM		=	myp->SEM;
	bm		=	z->bm;

	if( !(CNetBase = OpenLibrary( "cnet.library",0 )) )
		goto err0;

	/* put your program here */






	/* get back to CNet */

err0:	ShutDown( NULL );

err:	DeletePort( replyp );

	if( CNetBase ) CloseLibrary( CNetBase );

	exit(0);
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

/*	flags are:
		    1:	All capitalized
		    2:	FILENAME.  Don't allow =":;/ or asterisk
		    4:	Begin with existing z.InBuffer
		    8:	Chop leading spaces
		   16:	Force 1st letter of word caps
		   32:	Force all others lower case
		   64:	Numeric input only
		  128:	Print input box (.)
		  256:	DO allow MCI
		  512:	HANDLES/SPECIAL.  Don't allow ^_`{|}~@
		 1024:	Exit for . or / at beginning of line
		 2048:	Exit for backspace at beginning of line
		 4096:	Do not allow OLM's to appear while editing
		 8192:	Allow Chat break in at this prompt. COMMAND PROMPT.
		16384:	Don't allow SPACE, either
		32768:  DON'T ALLOW MOVEMENT
*/

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
	return( (long)cmess.result );
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

/* file: the ".vde" filename, without the ".vde"!
   data: pointer to the structure you are going to edit
   size: structure length in bytes

   returns: TRUE  if structure has been changed
	    FALSE otherwise
*/

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

/* 
	In preparation for an ExtUpload, this function
	sets the minimum number of free bytes to maintain on the
	drive.
*/

void ExtSetMinFree( long free )
{
	cmess.arg1 = (ULONG)free;
	CallHost( 42 );
}

/*
	In preparation for an ExtDownload or an ExtUpload, this function
	sets the protocol to be used.  If you send NULL, it will allow the
	user to choose his OWN protocol.

	Otherwise, you may select 'a' to be the first letter of a valid
	system protocol (from BBSPROTO file), such as 'x', 'z', etc.

	TRUE will be returned if a protocol is selected and ready, FALSE
	if there is a problem.
*/

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

/* some commonly used functions in CNet */

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
