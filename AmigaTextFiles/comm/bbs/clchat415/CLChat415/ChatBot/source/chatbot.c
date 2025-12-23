#include <proto/exec.h>
#include <proto/dos.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <dos/dostags.h>
#include <dos/stdio.h>

#include <cl/lists.h>
#include <cl/chatserver.h>
#include "rev.h"

char version[] = { "$VER: CLChat-ChatBot " VERTAG };

static int Done;
static time_t starttime;
static BPTR serverfh;

static char logoffmsg[ 128 ] = {"Terminating..."};

static struct MsgPort *serverport, *myreplyport, *myport, *ipcport;
static APTR myid;

static char fn_cmds[ 128 ], fn_patterns[ 128 ], fn_dat[ 128 ], fn_users[ 128 ], fn_msg[ 128 ];

struct myargs {
	char *host;
	ULONG *port;
	char *nick;
	char *realname;
	char *channel;
	char *cfgdir;
} args;

static void initfn( void )
{
	strcpy( fn_cmds, args.cfgdir );
	AddPart( fn_cmds, "ChatBot.Cmds", 128 );
	strcpy( fn_patterns, args.cfgdir );
	AddPart( fn_patterns, "ChatBot.Patterns", 128 );
	strcpy( fn_dat, args.cfgdir );
	AddPart( fn_dat, "ChatBot.Dat", 128 );
	strcpy( fn_users, args.cfgdir );
	AddPart( fn_users, "ChatBot.Users", 128 );
	strcpy( fn_msg, args.cfgdir );
	AddPart( fn_msg, "ChatBot.Msg", 128 );
}

struct ipcmsg {
	struct Message m;
	char data[ 256 ];
};

struct nicksave {
	char nick[ 40 ];
	char logoffmsg[ 128 ];
	ULONG count;
	time_t lastlogoff;
	char userhost[ 128 ];
	time_t created;			// When was this entry created
	time_t onlineaccu;
	time_t lastlogin;
	ULONG valid;			// the above 3 entries are valid (0xAC1DAFFE)
	ULONG accucount;
	char reserved[ 108 ];
};

long __stack = 8192;

struct usernode {
	struct MinNode n;
	char username[ 256 ];	// Already ParsedPattern
	char nick[ 32 ];	// Current nick
	int pri;
	ULONG autoop;
	char comment[ 256 ];
};

struct nsn {
	struct MinNode n;
	char nick[ 40 ];
};

struct cmd {
	struct MinNode n;
	char cmd[ 40 ];
	int pri;
	char exec[ 256 ];
};

struct pat {
	struct MinNode n;
	char pat[ 256 ];
	char exec[ 256 ];
	time_t lastused;
};

struct MinList userlist;
struct MinList nickscanlist;
struct MinList cmdlist;
struct MinList patlist;

static char *mynick;

static int shutup;

static UWORD fmtfunc[] = { 0x16c0, 0x4e75 };
void __stdargs sprintf( char *to, char *fmt, ... )
{
	RawDoFmt( fmt, &fmt + 1, (APTR)fmtfunc, to );
}

static void __stdargs sendmsg( char *fmt, ... )
{
	if( !serverfh )
	{
		struct chatmsg *svm;

		svm = malloc( sizeof( *svm ) );
		svm->cmd = CM_DATA;
		svm->id = myid;
		svm->m.mn_ReplyPort = myreplyport;
		
		RawDoFmt( fmt, &fmt + 1, (APTR)fmtfunc, svm->data );
		PutMsg( serverport, (struct Message*)svm );
	}
	else
	{
		VFPrintf( serverfh, fmt, &fmt + 1 );
		FPutC( serverfh, '\n' );
	}
}

static int __stdargs expandtemplate( char *from, char *to, int maxlen, ... )
{
	ULONG *idp;
	char **datap;
	int ch;
	char *oldto = to;

	while( *from )
	{
		if( *from != '%' )
		{
			*to++ = *from++;
			continue;
		}
		from++;
		ch 		= *from++;
		idp 	= ( ULONG * ) &maxlen + 1;
		datap 	= ( char ** ) &idp[ 1 ];
		while( *idp )
		{
			/* Replacement char gefunden */
			if( *idp == ch )
			{
				if( *datap )
					strcpy( to, *datap );
				to = strchr( to, 0 );
				break;
			}
			idp++;
			idp++;
			datap++;
			datap++;
		}
		/* Nicht gefunden, Zeichen 1:1 übernehmen */
		if( !*idp )
			*to++ = ch;
	}
	*to = 0;

	return( (int)strlen( oldto ) );
}

static int findnick( char *nick, int remove )
{
	struct nsn *n;

	for( n = FIRSTNODE( &nickscanlist ); NEXTNODE( n ); n = NEXTNODE( n ) )
	{
		if( !stricmp( n->nick, nick ) )
		{
			if( remove )
			{
				REMOVE( n );
				free( n );
			}
			return( 1 );
		}
	}
	return( FALSE );
}

static void addnicktoscanlist( char *nick )
{
	struct nsn *nsn;

	if( findnick( nick, FALSE ) )
		return;
	
	nsn = malloc( sizeof( *nsn ) );
	strcpy( nsn->nick, nick );
	ADDTAIL( &nickscanlist, nsn );
}

//
//	Format der Userliste:
//  Username Pri AutoopYN Comment
//

static void clnl( char *s )
{
	s = stpbrk( s, "\r\n" );
	if( s )
		*s = 0;
}

static void loadusers( void )
{
	BPTR f = Open( fn_users, MODE_OLDFILE );
	char buffer[ 512 ], *p;
	struct usernode *un;

	NEWLIST( &userlist );

	if( !f )
		return;

	while( FGets( f, buffer, 256 ) )
	{
		clnl( buffer );
		if( buffer[ 0 ] == ';' || !buffer[ 0 ] || buffer[ 0 ] == '#' )
			continue;

		un = malloc( sizeof( *un ) );
		if( !un )
			break;

		memset( un, 0, sizeof( *un ) );

		p = strtok( buffer, " \t" );
		if( !p )
			continue;
		ParsePatternNoCase( p, un->username, 256 );

		ADDTAIL( &userlist, un );

		p = strtok( NULL, " \t" );
		if( !p )
			continue;
		un->pri = atoi( p );

		p = strtok( NULL, " \t" );
		if( !p )
			continue;
		if( *p == 'Y' || *p == 'y' )
			un->autoop = TRUE;

		p = strtok( NULL, "" );
		if( !p )
			continue;

		strcpy( un->comment, p );
	}
	Close( f );
}

static void loadpatterns( void )
{
	BPTR f = Open( fn_patterns, MODE_OLDFILE );
	char buffer[ 512 ], *p;
	struct pat *pn;

	NEWLIST( &patlist );

	if( !f )
		return;

	while( FGets( f, buffer, 512 ) )
	{
		clnl( buffer );
		if( buffer[ 0 ] == ';' || !buffer[ 0 ] )
			continue;

		pn = malloc( sizeof( *pn ) );
		if( !pn )
			break;

		memset( pn, 0, sizeof( *pn ) );

		p = strtok( buffer, "  \t\"" );
		if( !p )
			continue;
		ParsePatternNoCase( p, pn->pat, 256 );

		p = strtok( NULL, "·" );
		if( !p )
			continue;

		strcpy( pn->exec, p );
		ADDTAIL( &patlist, pn );
	}
	Close( f );
}


static void loadcmd( void )
{
	BPTR f = Open( fn_cmds, MODE_OLDFILE );
	char buffer[ 256 ], *p;
	struct cmd *un;

	NEWLIST( &cmdlist );

	if( !f )
		return;

	while( FGets( f, buffer, 256 ) )
	{
		clnl( buffer );
		if( buffer[ 0 ] == ';' || !buffer[ 0 ] || buffer[ 0 ] == '#' )
			continue;

		un = malloc( sizeof( *un ) );
		if( !un )
			break;

		memset( un, 0, sizeof( *un ) );

		p = strtok( buffer, "  \t" );
		if( !p )
			continue;
		strcpy( un->cmd, p );

		p = strtok( NULL, "  \t" );
		if( !p )
			continue;
		un->pri = atoi( p );

		p = strchr( p, 0 ) + 1;
		strcpy( un->exec, stpblk( p ) );

		ADDTAIL( &cmdlist, un );
	}
	Close( f );
}

static struct cmd *findcmd( char *name )
{
	struct cmd *cmd;

	for( cmd = FIRSTNODE( &cmdlist ); NEXTNODE( cmd ); cmd = NEXTNODE( cmd ) )
		if( !stricmp( cmd->cmd, name ) )
			return( cmd );
	return( NULL );
}

static void chknicklogin( char *nick, char *userhost );

static void scanuser( char *nick, char *uid )
{
	struct usernode *un;
	char *p2;
	int rc;

	if( !uid || ( *uid != '[' && *uid != '(' ) )
		return;

	uid++;

	p2 = strpbrk( uid, "\t  " );
	if( p2 )
		*p2++ = 0;

	chknicklogin( nick, uid );

	// Ok, uid enthält nun den usernamen zum nick
	for( un = FIRSTNODE( &userlist ); NEXTNODE( un ); un = NEXTNODE( un ) )
	{
		if( MatchPatternNoCase( un->username, uid ) )
		{
			strcpy( un->nick, nick );

			if( rc = findnick( un->nick, TRUE ) )
			{
				if( un->autoop )
					sendmsg( "/OP %s", nick );
				if( un->comment[ 0 ] )
					sendmsg( "%s", un->comment );
			}
			return;
		}
	}
}

static int getuserpri( char *nick )
{
	struct usernode *un;

	for( un = FIRSTNODE( &userlist ); NEXTNODE( un ); un = NEXTNODE( un ) )
	{
		if( !stricmp( nick, un->nick ) )
			return( un->pri );
	}
	return( 0 );
}

static void killnick( char *nick )
{
	struct usernode *un;

	for( un = FIRSTNODE( &userlist ); NEXTNODE( un ); un = NEXTNODE( un ) )
	{
		if( !stricmp( nick, un->nick ) )
		{
			un->nick[ 0 ] = 0;
			return;
		}
	}
}

static void __stdargs answer( char *user, int priv, char *fmt, ... )
{
	char buffer[ 512 ];

	if( user )
	{
		if( priv )
		{
			sprintf( buffer, "/QMSG %s ", user );
		}
		else
		{
			sprintf( buffer, "%s: ", user );
		}
	}
	else
		buffer[ 0 ] = 0;

	RawDoFmt( fmt, &fmt + 1, (APTR)fmtfunc, strchr( buffer, 0 ) );
	sendmsg( "%s", buffer );
}

static void exec( char *fromuser, int priv, char *exec, char *parms )
{
	char xb[ 512 ];
	BPTR ofh;

	if( parms && strpbrk( parms, "><`" ) )
		return;

	expandtemplate( exec, xb, 512,
		's', parms,
		'S', parms,
		'p', parms,
		'P', parms,
		'n', fromuser ? fromuser : "",
		'N', fromuser ? fromuser : "",
		NULL
	);

	ofh = Open( "T:cbtmp", MODE_NEWFILE );

	SystemTags( xb,
		SYS_Output, ofh,
		TAG_DONE
	);

	Seek( ofh, 0, OFFSET_BEGINNING );
	Flush( ofh );

	while( FGets( ofh, xb, 255 ) )
	{
		clnl( xb );
		answer( fromuser, priv, "%s", xb );
		Delay( 25 );
	}
	Close( ofh );
	//DeleteFile( "T:cbtmp" );
}

static int findnicklogin( char *nick, struct nicksave *to, BPTR f )
{
	while( FRead( f, to, sizeof( *to ), 1 ) == 1 )
	{
		if( !stricmp( to->nick, nick ) )
			return( TRUE );
	}
	return( FALSE );
}

static int findnicksave( char *nick, struct nicksave *to )
{
	BPTR f = Open( fn_dat, MODE_READWRITE );
	struct nicksave ns;

	if( !f )
		return( FALSE );

	SetVBuf( f, 0, BUF_FULL, 32 * sizeof( ns ) );

	if( findnicklogin( nick, to, f ) )
	{
		Close( f );
		return( TRUE );
	}
	Close( f );
	return( FALSE );
}

static void handlecmd( char *fromuser, int priv, char *cmd )
{
	char *p = stpbrk( cmd, "  \t," );
	struct cmd *cmn;
	int pri = getuserpri( fromuser );

	if( !stricmp( fromuser, mynick ) )
		return;

	if( p )
	{
		*p++ = 0;
		p = stpblk( p );
	}
	else
		p = "";

	if( !stricmp( cmd, "STATUS" ) )
	{
		char x[ 6 ];
		struct nicksave ns;

		if( *p && pri >= 99 )
		{
			if( !findnicksave( p, &ns ) )
			{
				answer( fromuser, priv, "Sorry, no status for %s available.\n", p );
				return;
			}
			utunpk( ns.created, x );

			answer( fromuser, priv, "%s has logged in %ld times. I've first seen that user on %02ld-%02ld-%04ld. Since then, he has been online for %ld days, %ld hours and %ld mins. His average online time per login is ~%ld minutes, counting %ld logins i've tracked.", 
				p,
				ns.count,
				x[ 2 ], x[ 1 ], x[ 0 ] + 1970,
				ns.onlineaccu / ( 24 * 3600 ),
				( ns.onlineaccu / 3600 ) % 24,
				( ns.onlineaccu / 60 ) % 60,
				ns.accucount ? ( ( ( ns.onlineaccu / ns.accucount ) + 59 ) / 60 ) : 0,
				ns.accucount
			);
			return;
		}

		if( !findnicksave( fromuser, &ns ) || ns.count < 2 )
		{
			answer( fromuser, priv, "Your status is %ld. This is the first time that i have seen you.", pri, ns.count );
		}
		else
		{
			utunpk( ns.created, x );

			answer( fromuser, priv, "Your status is %ld. You've logged in %ld times. I've first seen you on %02ld-%02ld-%04ld. Since then, you have been online for %ld days, %ld hours and %ld mins. Your average online time per login is ~%ld minutes, counting %ld logins i've tracked.", 
				pri, 
				ns.count,
				x[ 2 ], x[ 1 ], x[ 0 ] + 1970,
				ns.onlineaccu / ( 24 * 3600 ),
				( ns.onlineaccu / 3600 ) % 24,
				( ns.onlineaccu / 60 ) % 60,
				ns.accucount ? ( ( ( ns.onlineaccu / ns.accucount ) + 59 ) / 60 ) : 0,
				ns.accucount
			);
		}
		return;
	}
	else if( !stricmp( cmd, "VERSION" ) || !stricmp( cmd, "*!VER" ) )
	{
		answer( fromuser, priv, "VERSION CLChat-ChatBot/AmigaOS " VERTAG, pri );
		return;
	}
	else if( !stricmp( cmd, "UPTIME" ) )
	{
		time_t t = time( NULL ) - starttime;
		int numuser = 0;
		BPTR f = Lock( fn_dat, SHARED_LOCK );
		if( f )
		{
			__aligned struct FileInfoBlock fib;
			Examine( f, &fib );
			UnLock( f );
			numuser = fib.fib_Size / sizeof( struct nicksave );
		}

		answer( fromuser, priv, "I'm up for %ldd, %ldh, %ldm, %lds. I know of %ld users.",
			t / ( 3600 * 24 ),
			( t / 3600 ) % 24,
			( t / 60 ) % 60,
			( t ) % 60,
			numuser
		);
	}
	else if( !stricmp( cmd, "UNKNOWN" ) )
	{
		return;	// Silent ignore...
	}
	else if( !stricmp( cmd, "SHUTUP" ) )
	{
		answer( fromuser, priv, "Ok, I'm quiet now." );
		shutup = TRUE;
	}
	else if( !stricmp( cmd, "TALK" ) )
	{
		answer( fromuser, priv, "Ok, I'm talking again." );
		shutup = FALSE;
	}
	else if( !stricmp( cmd, "MSG" ) )
	{
		char *p2 = stpbrk( p ? p : "", "\t  " );

		if( !p || !*p || !p2 )
		{
			answer( fromuser, priv, "Usage: MSG nick msgtext" );
		}
		else
		{
			char msgfn[ 256 ];
			BPTR f;

			*p2++ = 0;
			p2 = stpblk( p2 );

			if( stpbrk( p, ":/ \t@" ) )
			{
				answer( fromuser, priv, "Invalid nick \"%s\" specified.", p );
				return;
			}

			strcpy( msgfn, fn_msg );
			strcat( msgfn, p );
			f = Open( msgfn, MODE_READWRITE );
			if( !f )
			{
				answer( fromuser, priv, "Can't store MSG for %s.", p );
				return;
			}

			Seek( f, 0, OFFSET_END );
			utunpk( time( NULL ), msgfn );
			FPrintf( f, "** I've got a message for you from %s, stored %02ld:%02ld:%02ld on %02ld-%02ld-%02ld:\n",
				fromuser,
				msgfn[ 3 ], msgfn[ 4 ], msgfn[ 5 ],
				msgfn[ 2 ], msgfn[ 1 ], msgfn[ 0 ] + 70
			);
			FPrintf( f, "** %s\n", p2 );
			Close( f );
			answer( fromuser, priv, "MSG for %s stored.", p );
		}
	}
	else if( !stricmp( cmd, "SEEN" ) )
	{
		struct nicksave ns;

		if( !*p )
		{
			answer( fromuser, priv, "Usage: SEEN <nick>" );
			return;
		}

		if( !stricmp( p, mynick ) )
		{
			answer( fromuser, priv, "I don't have a mirror at hand and therefore can't see me. I'm here, anyway. Believe me!" );
			return;
		}
		if( !stricmp( p, fromuser ) )
		{
			answer( fromuser, priv, "Hm, sorry, i can't see you right now. Perhaps you'll come back later. Better ask yourself." );
			return;
		}

		if( findnicksave( p, &ns ) )
		{
			if( !ns.lastlogoff )
				answer( fromuser, priv, "%s (%s) is currently online.", p, ns.userhost );
			else if( ns.lastlogoff == ~0 )
				answer( fromuser, priv, "Sorry, I've missed %s's (%s) last logoff.", p, ns.userhost );
			else
			{
				time_t age = time( NULL ) - ns.lastlogoff;
				if( age > ( 24 * 3600 ) )
				{
					answer( fromuser, priv, "I haven't seen %s (%s) for %ld days.", p, ns.userhost, age / ( 24 * 3600 ) );
				}
				else if( age > 3600 )
				{
					answer( fromuser, priv, "%s (%s) logged out %ld hour(s) ago (%s).", p, ns.userhost, age / 3600, ns.logoffmsg );
				}
				else
				{
					answer( fromuser, priv, "%s (%s) logged out %ld minute(s) ago (%s).", p, ns.userhost, ( age + 59 ) / 60, ns.logoffmsg );
				}
			}
		}
		else
			answer( fromuser, priv, "I've never seen %s!", p );
	}
	else if( !stricmp( cmd, "QUIT" ) && pri > 90 )
	{
		if( p && *p )
			strcpy( logoffmsg, p );
		Done = TRUE;
		return;
	}
	else if( !stricmp( cmd, "RELOAD" ) && pri > 90 )
	{
		NEWLIST( &nickscanlist );
		sendmsg( "/USERS" );
		loadusers();
		loadcmd();
		loadpatterns();
		answer( fromuser, priv, "Reload of configuration complete!" );
		return;
	}
	else if( !stricmp( cmd, "EMIT" ) && pri >= 99 )
	{
		sendmsg( "%s", p );
		return;
	}
	else if( !strnicmp( cmd, "*!PING", 6 ) && priv )
	{
		sendmsg( "/QMSG %s *!PONG%s", fromuser, &cmd[ 6 ] );
		return;
	}
	else if( !strnicmp( cmd, "*!-CLFTREQ", 10 ) && priv )
	{
		sendmsg( "/QMSG %s You can't send files to me!", fromuser );
		return;
	}
	else if( !strnicmp( cmd, "\001DCC", 4 ) && priv )
	{
		sendmsg( "/QMSG %s I can't accept DCC requests!", fromuser );
		return;
	}
	else if( !stricmp( cmd, "?" ) || !stricmp( cmd, "HELP" ) )
	{
		char replybuffer[ 512 ];

		strcpy( replybuffer, "Status, Version, Uptime, Seen, MSG" );
		if( pri > 90 )
			strcat( replybuffer, ", Quit, Reload, Shutup, Talk" );
		if( pri >= 99 )
			strcat( replybuffer, ", Emit" );

		answer( fromuser, priv, "Commands available for status %ld:", pri );

		for( cmn = FIRSTNODE( &cmdlist ); NEXTNODE( cmn ); cmn = NEXTNODE( cmn ) )
		{
			if( cmn->pri > pri )
				continue;
			if( replybuffer[ 0 ] )
				strcat( replybuffer, ", " );
			strcat( replybuffer, cmn->cmd );
			if( strlen( replybuffer ) > 256 )
			{
				answer( fromuser, priv, replybuffer );
				replybuffer[ 0 ] = 0;
			}
		}
		if( replybuffer[ 0 ] )
			answer( fromuser, priv, replybuffer );
	}
	else
	{
		cmn = findcmd( cmd );
		if( !cmn || cmn->pri > pri )
		{
			if( stricmp( fromuser, cmd ) )
			{
				answer( fromuser, priv, "Unknown command \"%s\". Try \"/MSG %s HELP\" for a list of commands.",
					cmd, mynick
				);
			}
			return;

		}

		// Kommando ausführen
		exec( fromuser, priv, cmn->exec, p );
	}
}

static void findpat( char *pat )
{
	struct pat *pn;

	if( shutup )
		return;

	for( pn = FIRSTNODE( &patlist ); NEXTNODE( pn ); pn = NEXTNODE( pn ) )
	{
		if( MatchPatternNoCase( pn->pat, pat ) )
		{
			time_t t;

			time( &t );

			if( t - pn->lastused > 10 )
			{
				exec( NULL, 0, pn->exec, pat );
				pn->lastused = time( NULL );
			}
			break;
		}
	}
}

static void initnicksave( void )
{
	BPTR f = Open( fn_dat, MODE_READWRITE );
	struct nicksave ns;

	SetVBuf( f, 0, BUF_FULL, 32 * sizeof( ns ) );

	while( FRead( f, &ns, sizeof( ns ), 1 ) == 1 )
	{
		if( !ns.lastlogoff )
		{
			ns.lastlogoff = ~0;
			Seek( f, -sizeof( ns ), OFFSET_CURRENT );
			FWrite( f, &ns, sizeof( ns ), 1 );
		}
	}
	Close( f );
}

static void chknicklogin( char *nick, char *userhost )
{
	BPTR f = Open( fn_dat, MODE_READWRITE );
	struct nicksave ns;

	if( !f )
		return;

	SetVBuf( f, 0, BUF_FULL, 32 * sizeof( ns ) );

	if( findnicklogin( nick, &ns, f ) )	
	{
		Seek( f, -sizeof( ns ), OFFSET_CURRENT );
	}
	else
	{
		memset( &ns, 0, sizeof( ns ) );
		strcpy( ns.nick, nick );
	}
	if( ns.valid != 0xAC1DAFFE )
	{
		ns.valid = 0xAC1DAFFE;
		ns.onlineaccu = 0;
		ns.accucount = 0;
		time( &ns.created );
	}

	time( &ns.lastlogin );
	ns.count++;
	ns.lastlogoff = NULL;
	strncpy( ns.userhost, userhost, 127 );
	ns.userhost[ 127 ] = 0;
	FWrite( f, &ns, sizeof( ns ), 1 );
	Close( f );
}

static void chknicklogoff( char *nick, char *msg )
{
	BPTR f = Open( fn_dat, MODE_READWRITE );
	struct nicksave ns;

	if( !f )
		return;

	SetVBuf( f, 0, BUF_FULL, 32 * sizeof( ns ) );

	if( findnicklogin( nick, &ns, f ) )	
	{
		Seek( f, -sizeof( ns ), OFFSET_CURRENT );
		time( &ns.lastlogoff );
		strncpy( ns.logoffmsg, msg, 127 );
		ns.logoffmsg[ 127 ] = 0;

		if( ( ns.lastlogoff - ns.lastlogin > 120 ) )
		{
			ns.onlineaccu += ns.lastlogoff - ns.lastlogin;
			ns.accucount++;
		}
		FWrite( f, &ns, sizeof( ns ), 1 );
	}
	Close( f );
}

static void chkmsgs( char *nick )
{
	char msgfn[ 256 ];
	char buffer[ 256 ];
	BPTR f;

	strcpy( msgfn, fn_msg );
	strcat( msgfn, nick );
	f = Open( msgfn, MODE_OLDFILE );
	if( !f )
		return;

	while( FGets( f, buffer, 256 ) )
	{
		clnl( buffer );
		sendmsg( "/QMSG %s %s", nick, buffer );
	}
	Close( f );
	DeleteFile( msgfn );
}

void __stdargs __main( char *thissiimplysucks )
{
	char buffer[ 512 ], oldbuffer[ 512 ], *p, *p2, *p3;
	char hostname[ 64 ], ipcportname[ 128 ];
	static struct chatmsg *loginmsg;
	int c;
	struct RDArgs *rda;
	ULONG dummy = 5555;

	Printf( "%s\n(C) 1995 Oliver Wagner (o.wagner@lsd.wupper.de), All Rights Reserved\n", &version[ 6 ] );

	NEWLIST( &nickscanlist );

	args.port = &dummy;
	args.cfgdir = "S:";
	if( !( rda = ReadArgs( "Host/K,Port/K/N,Nick/A,Realname/A,Channel=AutoJoin/K,Dir=CFGDir/K", &args, NULL ) ) )
	{
		PrintFault( IoErr(), "ChatBot" );
		exit( 20 );
	}

	initfn();

	if( args.host )
	{
		sprintf( hostname, "TCP:%s/%ld", args.host, *args.port );
		Printf( "Attempting connect to %s...\n", hostname );
		serverfh = Open( hostname, MODE_NEWFILE );
		if( !serverfh )
		{
			Printf( "ChatBot: unable to connect to %s, err %ld\n", hostname, IoErr() );
			exit( 10 );
		}
		//SetVBuf( serverfh, 0, BUF_NONE, 512 );
	}

	GetVar( "HOSTNAME", hostname, 64, 0 );

	if( !serverfh )
	{
		for( c = 0; c < 30; c++ )
		{
			serverport = FindPort( CPNAME );
			if( serverport )
				break;
			Delay( 50 );
		}
		if( !serverport )
		{
			PutStr( "Unable to find Chat Server.\n" );
			FreeArgs( rda );
			exit( 20 );
		}

		myreplyport = CreateMsgPort();
		myport = CreateMsgPort();

		loginmsg = malloc( sizeof( *loginmsg ) );
		sprintf( loginmsg->data, "%s %s@%s %s", mynick = args.nick, mynick, hostname, args.realname );
		loginmsg->id = myport;
		loginmsg->cmd = CM_LOGIN;
		loginmsg->m.mn_ReplyPort = myreplyport;
		PutMsg( serverport,  (struct Message*)loginmsg );
		WaitPort( myreplyport );
		GetMsg( myreplyport );
		if( loginmsg->cmd )
		{
			Printf( "Chat login failed: %ld\n", loginmsg->cmd );
			DeleteMsgPort( myreplyport );
			DeleteMsgPort( myport );
			FreeArgs( rda );
			exit( 10 );
		}
		myid = loginmsg->id;
		free( loginmsg );
	}
	else
	{
		mynick = args.nick;
		FPrintf( serverfh, "%s %s@%s %s\n", mynick, mynick, hostname, args.realname );
	}

	loadusers();
	loadcmd();
	loadpatterns();
	initnicksave();

	time( &starttime );

	if( args.channel )
		sendmsg( "/JOIN %s", args.channel );

	sprintf( ipcportname, "CHATBOT_%s", mynick );
	strupr( ipcportname );

	ipcport = CreatePort( ipcportname, -32 );

	while( !Done )
	{
		struct ipcmsg *ipcmsg;

		if( !serverfh )
		{
			while( loginmsg = (struct chatmsg*)GetMsg( myreplyport ) )
				free( loginmsg );
		}

		while( ipcmsg = GetMsg( ipcport ) ) 
		{
			sendmsg( "%s", ipcmsg->data );
			ReplyMsg( ipcmsg );
		}

		if( CheckSignal( SIGBREAKF_CTRL_C ) )
			break;

		if( !serverfh )
		{
			if( !( loginmsg = (struct chatmsg*)GetMsg( myport ) ) )
			{
				Wait( ( 1L<<myport->mp_SigBit ) | ( 1L << ipcport->mp_SigBit ) | ( 1L << myreplyport->mp_SigBit ) );
				continue;
			}

			if( loginmsg->cmd != CM_DATA )
			{
				ReplyMsg( loginmsg );
				break;
			}
			strcpy( buffer, loginmsg->data );
			ReplyMsg( loginmsg );
		}
		else
		{
			if( !WaitForChar( serverfh, 100000 ) )
				continue;

			if( !FGets( serverfh, buffer, 511 ) )
				break;
			clnl( buffer );
		}

		Printf( "%s\n", buffer );
		//clnl( buffer );
		if( !buffer[ 0 ] )
			continue;	// Tick

		// Parsen
		if( buffer[ 0 ] == '[' )
		{
			p = strchr( buffer, ']' );
			if( !p )
				continue;
			*p++ = 0;
			p = stpblk( p );
			p2 = stpbrk( p, "\t " );
			if( p2 )
			{
				*p2++ = 0;
				p3 = stpbrk( p2, "\t " );
				if( p3 )
					*p3++ = 0;
			}

			// Server-Cmd in Buffer
			if( !stricmp( &buffer[ 1 ], "USERS" ) )
			{
				// WHOIS schicken, um Userliste upzudaten
				if( *p == '!' || *p == '@' || *p == '>' )
				{
					if( !findnick( p + 1, FALSE ) )
					{
						addnicktoscanlist( p + 1 );
						sendmsg( "/WHOIS %s", p + 1 );
					}
					continue;
				}
			}
			else if( !stricmp( &buffer[ 1 ], "WHOIS" ) )
			{
				if( !stricmp( p, "Nick" ) )
				{
					scanuser( p2, p3 );
				}				
			}
			else if( !stricmp( &buffer[ 1 ], "JOIN" ) )
			{
				if( stricmp( mynick, p ) )
				{
					addnicktoscanlist( p );
					sendmsg( "/WHOIS %s", p );
				}
			}
			else if( !stricmp( &buffer[ 1 ], "ENTER" ) )
			{
				if( stricmp( mynick, p ) )
				{
					char *p4 = strrchr( p2, ')' );
					*p4 = 0;
					addnicktoscanlist( p );
					scanuser( p, p2 );
					chkmsgs( p );
				}
			}
			else if( !stricmp( &buffer[ 1 ], "EXIT" ) )
			{
				char *p4 = strchr( p3, '(' );
				if( p4 )
				{
					char *p5;

					p4++;
					p5 = strrchr( p4, ')' );
					if( p5 )
						*p5 = 0;
				}
				killnick( p );
				chknicklogoff( p, p4 ? p4 : "none" );
			}
			else if( !stricmp( &buffer[ 1 ], "NICK" ) )
			{
				char bf[ 128 ];

				p3 = strrchr( p3, ' ' );
				if( !p3 )
					continue;
				*p3++ = 0;
				p2 = strchr( p3, '.' );
				if( p2 )
					*p2 = 0;

				sprintf( bf, "renamed to %s", p3 );
				chknicklogoff( p, bf );

				sendmsg( "/WHOIS %s", p3 );
			}
		}
		else if( buffer[ 0 ] == '<' )	// Priv
		{
			p = strchr( buffer, '>' );
			if( !p )
				continue;
			*p++ = 0;
			handlecmd( &buffer[ 1 ], TRUE, stpblk( p ) );
		}
		else if( buffer[ 0 ] == 2 )
		{
			static char nickbf[ 40 ];
			strncpy( nickbf, &buffer[ 1 ], 39 );
			mynick = nickbf;
			p = stpbrk( mynick, " \t " );
			if( p )
				*p = 0;
		}
		else // Öffentlich, 
		{
			p = strchr( buffer, ':' );
			if( !p )
				continue;
			strcpy( oldbuffer, buffer );
			*p++ = 0;
			p = stpblk( p );

			if( !stricmp( p, "PING" ) )
			{
				sendmsg( "\002*PONG*" );
				continue;
			}

			p2 = stpbrk( p, "\t:,  " );
			if( !p2 )
			{
				if( stricmp( buffer, mynick ) )
					findpat( oldbuffer );
				continue;
			}
			*p2++ = 0;

			if( stricmp( p, mynick ) )
			{
				if( stricmp( buffer, mynick ) )
					findpat( oldbuffer );
				continue;
			}

			handlecmd( buffer, FALSE, stpblk( p2 ) );
		}
	}

	if( !serverfh )
	{
		loginmsg = malloc( sizeof( *loginmsg ) );
		loginmsg->id = myid;
		loginmsg->cmd = CM_LOGOFF;
		loginmsg->m.mn_ReplyPort = myreplyport;
		strcpy( loginmsg->data, logoffmsg );
		PutMsg( serverport,  (struct Message*)loginmsg ); 
		Delay( 50 );
		while( loginmsg = (struct chatmsg*)GetMsg( myport ) ) 
			ReplyMsg( loginmsg );
	}
	else
	{
		FPrintf( serverfh, "/QUIT Terminating...\n" );
		Close( serverfh );
	}

	DeleteMsgPort( myreplyport );
	DeleteMsgPort( myport );
	DeletePort( ipcport );
	FreeArgs( rda );
	exit( 0 );
}
