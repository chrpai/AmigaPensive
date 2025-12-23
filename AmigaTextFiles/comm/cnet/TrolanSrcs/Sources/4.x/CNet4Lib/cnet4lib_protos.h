/*
** CNet/4 Link Library Prototypes
** by Kelly Cochran
** Last updated: Tue Mar 24 00:57:56 1998
*/

#ifndef CNET4LIB_PROTOS_H
#define CNET4LIB_PROTOS_H

long  ListDir( UBYTE a, UBYTE b, struct IsDate *c );
void  SetDoing( char *what );
void  EditMessage( char *file );
UBYTE PutQ( char *a );
void  PrepEditor( BPTR fp );
void  LoadText( BPTR fh );
void  LoadError( void );
void  MakeEd( char *path );
char *ExtDownload( char *args );
short FindCommand( short num );
UBYTE ReadAccount( short id, struct UserData *user );
UBYTE SelectAndDownload( char *file, UBYTE flags );
UBYTE AddCharge( short n, short a );
UBYTE ReadFile( char *path, UBYTE flags );
void  SaveEditor( BPTR fp, UBYTE eof );
BYTE  WriteUKeys( BYTE getsem );
UBYTE Rnext( void );
void  CheckFlowControl( void );
void  CallHost( UBYTE c );
void  EnterPassword( UBYTE len );
short compstra( char *s, char *t );
UBYTE CheckBalance( short n, short a );
long  ConferenceWait( short a );
void  PutText( char *text );
int   EnterText( char firstchar, short maxchars, short perline, short maxlines );
void  CheckChanges( void );
void  InputSignalDispose( void );
UBYTE ReadGraphics( char *path, char flags );
BPTR  OpenEd( long mode );
char *ExtUpload( char *args );
void  ReadMessagePoint( char *a, long b );
void  DoReturn( void );
long  CGetFree( char *s, UBYTE q );
short VisualDataEditor( char *file, void *data, long size );
int   EnterLine( UBYTE len, ULONG flags, char *prompt );
void  ShutDown( char *spawn );
UBYTE ExtSetProtocol( char a );
void  ExtSetMinFree( long free );
short FindAccount( char *a, struct UserData *b, UBYTE c );
long  CommonCommands( void );
void  PutA( void );
void  DoANSIOut( UBYTE n);
void  MakeDate( struct IsDate *date, char *output );
long  ConvertAccess( char *s );
char  OneKey( void );
void  ParseCommandLine( UBYTE numargs );
UBYTE SaveAccount( struct UserData *user, short id );
char  WaitForInput( long mics );
void  DeleteEd( void );
void  DoANSI( UBYTE n, USHORT a, USHORT b);
void  CallEditor( short max, short inlines );
int   CalcAge(struct UserData *udata);
void  MakeOLM(char *msg, int id);
BPTR  OpenAppend(char *file);
void  FileDate(struct DateStamp *dstamp, struct IsDate *date);
void  checkcarrier(void);
BOOL  RemoveNode(struct List *list, int node);
BOOL  WhoIdleTime(struct PortData *z, char *output, int mintime);
BOOL  IsSubOp(struct UserData *udata, struct SubboardType4 *sub);
void  CNetPutMsg(char *port, UBYTE value);

struct List *init_list(void);
void         free_list(struct List **list);

#endif
