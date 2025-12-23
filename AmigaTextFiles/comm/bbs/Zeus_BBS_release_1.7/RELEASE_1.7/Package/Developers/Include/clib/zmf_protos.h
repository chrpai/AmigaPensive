#ifndef CLIB_ZMF_PROTOS_H
#define CLIB_ZMF_PROTOS_H

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif
#ifndef ZFILE_H
#include <zfile.h>
#endif

void zeroize ( char *, int ) ;
struct Node * NumFindNode ( ULONG, struct List * ) ;
void MySort ( struct List *, int ( * ) () ) ;
void MakeUpper ( char *, int ) ;
UBYTE * LineWrap ( char *, ULONG ) ;
void CopySpaces   ( char *, int ) ;
void PrintSpaces ( int, ULONG ) ;
void InitParse ( struct parsekey *, char * ) ;
int ParseNum ( struct parsekey *, int ( * ) ( ), ULONG ) ;
int ParseChar ( struct parsekey *, int ( * ) ( ), ULONG ) ;
struct filearea * GetFileAreaData ( ULONG ) ;
int AreasLoad ( struct List * ) ;
struct PF_Globals * GetFileGlobals ( void ) ;
int IsPlFileThere ( void ) ;
int IsPlMsgThere ( void ) ;
void GetFileFlagList ( char *, struct filearea * ) ;
struct FileStats * GetFileStats  ( void ) ;
int TxtDisplay ( char *, int, ULONG ) ;
int XprProtosLoad ( struct List * ) ;
struct UserFileData * GetUserFileData ( ULONG ) ;
signed long FileSize ( char * ) ;
int RebuildFiles ( ULONG, BOOL, BOOL ) ;
struct XprProto * GetProtocol ( int ) ;
struct Editor * GetEditor ( int ) ;
struct BaudEntry * GetBaudRate ( int ) ;
int BaudRatesLoad ( struct List * ) ;
struct List *GetEditorList ( void ) ;
struct WholeMessage *LoadMessage ( ULONG, ULONG, ULONG ) ;
struct UserMsgData *GetUserMsgData ( ULONG ) ;
struct msgarea *GetMsgAreaData ( ULONG ) ;
struct MsgGlobals *GetMsgGlobals ( void ) ;
ULONG GetNumberOfMessages ( void ) ;
void FreeMessage ( struct WholeMessage * ) ;
struct List * GetArchiverList ( void ) ;
struct MsgScanKey * InitMsgScanKey ( struct ScanInfo * ) ;
void CloseMsgScanKey ( struct MsgScanKey * ) ;
ULONG ScanMessages ( struct MsgScanKey * , struct WholeMessage * ) ;
struct MsgIdx * GetPointers ( ULONG ) ;
int ProgramLog ( int, UWORD, char * ) ;
signed int MoveFile ( char *, char *, ULONG ) ;
struct List * ReadAddresses ( char *, char *, ULONG ) ;
void FreeAddresses ( struct List * ) ;
signed int CompareAddresses ( struct FidoAddress *, struct FidoAddress * ) ;
char * WriteAddresses ( struct List *, ULONG, int, char * ) ;
int LoadSigs ( struct List *, BOOL ) ;
LONG FindMessageFromDate ( ULONG, time_t *, int ) ;
LONG FindFileFromDate ( int, time_t *, int ) ;
int InsertNodeAlpha ( struct List *, APTR, signed int ( * ) ( ) ) ;
char * FormMessageText ( char *, ULONG, APTR ) ;
int MatchStr ( char *, char * ) ;
signed int SaveMessage ( struct WriteMsgStruct * ) ;
LONG GetHighPointer ( int, ULONG ) ;
APTR ReadWholeFile ( char *, ULONG * ) ;
struct List *GetSigList ( void ) ;
struct SigNode *GetSig ( ULONG ) ;
struct ConfigUpdate *GetConfigUpdate ( void ) ;
struct List *GetBaudList ( void ) ;
int EditorsLoad ( struct List * ) ;
signed int EditMessage ( struct Msg *, unsigned char * ) ;
signed int DeleteMessage ( int, ULONG ) ;
int SetPointers ( struct MsgIdx * ) ;
int ScanIndex ( char *, int (*)() ) ;
struct AreaNode * GetFileAreaNode ( int ) ;
int AddMsgBase ( struct msgarea * ) ;
struct List * GetMsgBaseLib ( void ) ;
struct MsgAreaNode * GetMsgAreaNode ( ULONG ) ;
BOOL ParseFidoAddr ( char *, struct FidoAddress * ) ;
ULONG FileAreaAccess ( ULONG, ULONG ) ;
ULONG MsgAreaAccess ( ULONG, ULONG ) ;
struct List * GetTemplateList ( void ) ;
ULONG SetFidoHeaders ( struct WriteMsgStruct * ) ;
struct List * LoadPaths ( struct List * ) ;
struct List * SavePaths ( struct List * ) ;
UBYTE * GetPath ( UBYTE *, struct filearea * ) ;
int AddFileArea ( struct filearea * ) ;
int AddMsgArea ( struct msgarea * ) ;
BOOL ScanMsgHdrs ( APTR, struct Msg * ) ;
BOOL LoadOrigin ( struct List * ) ;
UBYTE * GetOrigin ( struct msgarea * ) ;
BOOL LoadExecutables ( struct List * ) ;
struct Executable *GetExecutable ( struct msgarea * ) ;
int ReflowText ( UBYTE *, UBYTE *, UBYTE *, UBYTE, ULONG, ULONG ) ;
BOOL Validate ( struct file * ) ;
struct ListIndex * AllocIndex ( struct List *, ULONG * ) ;
BOOL BuildIndex ( struct List *, struct ListIndex *, ULONG ) ;
struct AreaNode * GetCloseFileArea ( ULONG ) ;
struct MsgAreaNode * GetCloseMsgArea ( ULONG ) ;
BOOL AddWaitingMail ( UBYTE *, ULONG, ULONG ) ;
signed int AddMarkedFile ( UBYTE *, UBYTE *, ULONG ) ;
LONG GetSigTag ( UBYTE * ) ;
struct SigNode * GetSigByDirectory ( ULONG, UBYTE * ) ;
void MakeFidoAddr ( struct FidoAddress *, UBYTE * ) ;
BOOL OutputHdrCodes ( struct User *, ULONG, UBYTE *, ULONG, struct Msg *, struct Line * ) ;
BOOL RegisterDock ( struct Task *task ) ;
struct Task *GetDock ( void ) ;
ULONG FindUnique ( ULONG ) ;
BOOL SnapshotMessageAreas ( struct List * ) ;
BOOL SnapshotFileAreas ( struct List * ) ;
BOOL BuildFileEvents ( APTR, struct List * ) ;
BOOL BuildMsgEvents ( APTR, struct List * ) ;
void FreeSnapshot ( APTR ) ;
BOOL SetMsgAccess ( ULONG, ULONG, ULONG ) ;
BOOL SetFileAccess ( ULONG, ULONG, ULONG ) ;
APTR InitAccessScan ( BOOL, struct User *, struct List * ) ;
void CloseAccessScan ( APTR ) ;
struct AreaAccess *ScanAccesses ( APTR ) ;
BOOL WriteAccesses ( struct User *, struct List *, ULONG ) ;
BOOL LoadAccesses( struct User *, struct List *, ULONG ) ;
void FreeAccessList ( struct List *list ) ;
BOOL UpdateOnlineUser ( struct User *user ) ;
int BulkAddFileArea ( struct filearea * ) ;
int BulkAddMsgArea ( struct msgarea * ) ;
BOOL BulkSaveMsgAreas ( void ) ;
BOOL BulkSaveFileAreas ( void ) ;
UBYTE *GetUploadPath( ULONG, ULONG ) ;
struct Archiver *ArchiverType ( UBYTE * ) ;
int LoadArchivers ( struct List * ) ;
int FreeArchivers ( struct List * ) ;
BOOL GetWindowPos( UBYTE *, ULONG *, ULONG *, ULONG *, ULONG * ) ;
BOOL SaveWindowPos( UBYTE *, ULONG, ULONG, ULONG, ULONG ) ;
void FreeFileBaseList ( struct List * ) ;
BOOL LoadFileBaseList ( struct List * ) ;
BOOL SaveFileBaseList ( struct List * ) ;

/** New 1.2 file functions **/

void GetInfo( struct FileBaseInfo *info ) ;
struct fullFile *LoadFileName( ULONG area, UBYTE *filename, ULONG flags );
struct fullFile *LoadFile( ULONG Number, ULONG flags );
UBYTE *SaveFile( struct savefile *sf, ULONG flags );
UBYTE *QuickSave( struct savefile *sf );
LONG EditFile( struct file *file, UBYTE *description );
int DeleteCatFileName( UBYTE *name, ULONG area, ULONG flags );
BOOL DeleteCatFile( ULONG number, ULONG flags );
APTR InitFileScanKey( struct ScanInfo *info );
ULONG ScanFiles( APTR key, struct fullFile *ff );
void CloseFileScanKey( APTR key );
struct List *FindFile( UBYTE *name, UBYTE *fblib );
BOOL FileExists( UBYTE *name, ULONG area );
BOOL SearchPattern( int( *search )( UBYTE *, ULONG ), UBYTE *fblib );
BOOL RenameCatFile( UBYTE *old_name, ULONG area, UBYTE *new_name, ULONG flags );
void FileAreaStats( ULONG area, struct FileAreaStats *stats );
void FileLibraryStats( UBYTE *fblib, struct FileLibraryStats *stats );
BOOL ConfigureFileBase( struct Window *win, ULONG area, UBYTE *fblib );
LONG UpdateFB( ULONG area );
LONG RebuildFB( ULONG area );
LONG RenumberFileArea( ULONG oldarea, ULONG newarea );
LONG OptimiseFB( ULONG area );
LONG TrimFB( ULONG area );
LONG ConvertFB( ULONG area );
LONG DeleteFileArea( ULONG area );
LONG RecountFB( ULONG area );
LONG MoveFileRecord( struct fullFile *file, ULONG destination_area );


ULONG FileNameToNumber( char *Name );
BOOL  FileNumberToName( ULONG Number, char *Buffer );


BOOL GetFile( ULONG number, struct fileindex *idx );
BOOL GetFileNoExt( char *Name, struct fileindex *newidx );
signed int GetIndexFileName( char *FileName, char *DiskName );
BOOL GetFileIndex( char *name, ULONG offset, struct fileindex *idx );


/**********************************************************************/
/*                         PRIVATE FUNCTIONS                          */
/*               Don't Even THINK About Calling These!                */
/**********************************************************************/

int Pvt1 ( void ) ;
void Pvt2 ( void ) ;
int Pvt3 ( APTR, BOOL ) ;
void Pvt4 ( void ) ;
int Pvt5 ( APTR, BOOL ) ;
void Pvt6 ( void ) ;

#endif /* CLIB_ZMF_PROTOS_H */
