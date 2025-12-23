/*
** CNet3/ Link Library Prototypes
** by Kelly Cochran
** Last Updated: Sun Sep 21 23:44:36 1997
*/

#ifndef CNET3LIB_PROTOS_H
#define CNET3LIB_PROTOS_H

void  CallEditor( short max, short inlines );
void  CallHost( UBYTE c );
UBYTE CheckBalance( short n, short a );
void  CheckChanges( void );
void  CheckFlowControl( void );
long  CommonCommands( void );
short compstra( char *s, char *t );
long  ConferenceWait( short a );
long  ConvertAccess( char *s );
void  DeleteEd( void );
void  DoReturn( void );
void  EditMessage( char *file );
void  EnterPassword( UBYTE len );
int   EnterText( char firstchar, short maxchars, short perline, short maxlines );
char *ExtDownload( char *args );
void  ExtSetMinFree( long free );
UBYTE ExtSetProtocol( char a );
char *ExtUpload( char *args );
short FindAccount( char *a, struct UserData *b );
short FindCommand( short num );
long  CGetFree( char *s, UBYTE q );
long  ListDir( UBYTE a, UBYTE b, struct IsDate *c );
void  LoadError( void );
void  LoadText( BPTR fh );
UBYTE AddCharge( short n, short a );
void  MakeDate( struct IsDate *date, char *output );
void  MakeEd( char *path );
char  OneKey( void );
BPTR  OpenEd( long mode );
void  ParseCommandLine( UBYTE numargs );
void  PrepEditor( BPTR fp );
void  PutA( void );
UBYTE PutQ( char *a );
void  PutText( char *text );
UBYTE ReadAccount( short id, struct UserData *user );
UBYTE ReadFile( char *path, UBYTE flags );
UBYTE ReadGraphics( char *path, char flags );
void  ReadMessagePoint( char *a, long b );
UBYTE Rnext( void );
UBYTE SaveAccount( struct UserData *user, short id );
void  SaveEditor( BPTR fp, UBYTE eof );
UBYTE SelectAndDownload( char *file, UBYTE flags );
void  SetDoing( char *what );
void  ShutDown( char *spawn );
short VisualDataEditor( char *file, void *data, long size );
char  WaitForInput( long mics );
int   EnterLine( UBYTE len, USHORT flags, char *prompt );
BOOL  FileExists(char *filename);
long  FileSize(char *filename);
BPTR  OpenAppend(char *file);
void  checkcarrier(void);
void  FileDate(struct DateStamp *dstamp, struct IsDate *date);
BOOL  RemoveNode(struct List *list, int node);

struct List *init_list(void);
void         free_list(struct List **list);
struct Node *FindOrdinal(struct List *list, int node);

#endif
