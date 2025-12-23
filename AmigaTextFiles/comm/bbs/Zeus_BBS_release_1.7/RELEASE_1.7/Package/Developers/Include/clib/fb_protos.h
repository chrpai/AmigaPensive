#ifndef CLIB_FB_PROTOS_H
#define CLIB_FB_PROTOS_H

BOOL fbInitialise ( struct PF_Globals *pfg ) ;
BOOL fbInitBase ( ULONG area ) ;
void fbQuit ( void ) ;
APTR fbControl ( ULONG, APTR );
void fbGetInfo( struct FileBaseInfo *info );

struct fullFile *fbLoadFile( ULONG num, ULONG flags ) ;

LONG fbSaveFile( struct savefile *sf, ULONG flags );
LONG fbQuickSave( struct savefile *sf, ULONG flags );
LONG fbEditFile( struct file *file, UBYTE *description );
int  fbDeleteCatFile( ULONG num, ULONG flags );
APTR fbInitFileScanKey( struct ScanInfo *info );
ULONG fbScanFiles( APTR key, struct fullFile *ff );
void fbCloseFileScanKey( APTR key );
struct List *fbFindFile( UBYTE *name );
BOOL fbFileExists( UBYTE *name, ULONG area );
BOOL fbRenameCatFile( UBYTE *old_name, ULONG area, UBYTE *new_name, ULONG flags ) ; 
BOOL fbSearchFileName( int( *search )( UBYTE *, ULONG ) ) ;
void fbFileAreaStats( ULONG area, struct FileAreaStats *stats );
void fbFileLibraryStats( struct FileLibraryStats *stats );
BOOL fbConfiguration( struct Window *win, ULONG area );
LONG fbUpdate( ULONG area );
LONG fbRebuild( ULONG area );
LONG fbRecount( ULONG area );
LONG fbOptimise( ULONG area );
LONG fbTrim( ULONG area );
LONG fbConvert( ULONG area );
LONG fbDeleteArea( ULONG area );
LONG fbRenumberArea( ULONG old, ULONG new );


//struct fullFile * fbLoadFile ( ULONG, ULONG ) ;

BOOL fbGetFile ( ULONG, struct fileindex * ) ;
BOOL fbFileNumberToName ( ULONG, char * ) ;
ULONG fbFileNameToNumber ( char * ) ;
signed int fbGetIndexFileName ( char *, char * ) ;
BOOL fbGetFileNoExt ( char *, struct fileindex * ) ;

struct fullFile *fbLoadFileName( ULONG area, UBYTE *filename, ULONG flags ) ;
int fbDeleteCatFileName( UBYTE *filename, ULONG area, ULONG flags ) ;

#endif
