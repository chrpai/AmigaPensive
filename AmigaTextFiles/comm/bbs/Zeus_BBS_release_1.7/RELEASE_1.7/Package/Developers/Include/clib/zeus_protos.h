#ifndef  CLIB_PIPELINE_PROTOS_H
#define  CLIB_PIPELINE_PROTOS_H

/*
** $VER: pipeline_protos.h 1.0 (12.01.94)
**
** C prototypes for the pipeline.library.
**
** (C) Copyright 1992-1994 Pipeline Developments.  Written by Alex May.
**     All rights reserved
*/

#ifndef DOS_DOS_H
#include "dos/dos.h"
#endif

#ifndef INTUITION_INTUITION_H
#include "intuition/intuition.h"
#endif

#ifndef _TIME_H
#include "time.h"
#endif

/*
** Misc functions
*/

void  ClearMem( APTR, ULONG );
UWORD CRC16Bit( UBYTE *, ULONG, UWORD );
ULONG CRC32Bit( UBYTE *, ULONG, ULONG );
UWORD *GetCRC16Tab( void );
ULONG *GetCRC32Tab( void );
void SortList( struct List * );
void Strip8Bit( char * );
void SpaceToUnder( char * );
void UnderToSpace( char * );
char *NiceName( char * );
int *NiceLine( char *, char *, int );
void GetConnectMode( UBYTE *, ULONG *, UWORD * );
APTR SearchArray( APTR, UWORD, UWORD, UBYTE *, ULONG * );

/*
** Fifo functions (plfunc.c)
*/

void AddLineFifo( long, long, long, long );
void RemLineFifo( long, void *, long, long );

/*
** Zeus functions
*/

void *GetShared( void );
int LockPipeline( void );
void UnLockPipeline( void );
void StoreShared( APTR );                       /* Private */
void ErrorLogWrite( char, char * );
long LoadSysCfg( UWORD );
void FreeSysCfg( void );

/*
** Function functions (!)
*/

APTR AddFDList( struct fdfunc *, int (*)(), int, int );
int RemFDList( APTR );
int ReplyFunction( int, int, char * );                      /* Do not use anymore */
void ReplyAsyncFunc( struct AsyncData *, long, UBYTE * );   /* Do not use anymore */
struct fdfunc *FindFunction( char * );                      /* Private */

/*
** Line functions
*/

struct Line *GetLine( int );
struct Line_Node *GetLineNode( int );           /* Private */
ULONG GetLineStatus( UWORD );
int KillMe( int );
int DontKillMe( void );
BOOL OwnIO( ULONG );
BOOL DisOwnIO( ULONG );
void LineLogWrite( UWORD, char, char * );
BOOL LineOutputMode( long, long );
BOOL LineKillMe( int, ULONG );
BOOL LineDontKillMe( int );
void SetStatus( long, UBYTE * );
long CheckMsgList( long );
BOOL GetLineMsg( long, UBYTE *, time_t * );

int NextRexxArg( int, char *, int );                  /* Do not use anymore */
LONG NextAsyncArg( UBYTE *, UBYTE *, LONG *, LONG );  /* Do not use anymore */
long ExpandCC( long, UBYTE *, UBYTE *, UWORD );
long ExpandCCStr( long, UBYTE *, UBYTE *, UWORD );

BOOL FindScript( struct Line *, UBYTE * );

/*
** Chat functions
*/

BOOL StartChat( UWORD, BOOL (*)(struct Line *, struct ChatChar *) );
BOOL ChatWrite( UWORD, APTR, ULONG );
void StopChat( UWORD );

/*
** Computer Type functions
*/

BOOL LoadCT( void );
void FreeCT( void );
BOOL GetCType( UWORD, char * );

/*
** Control functions
*/

struct SignalSemaphore *InitControl( struct Window *, void (*)(void) );
void FreeControl( void );
BOOL AddGad( struct Gadget * );
BOOL RemGad( struct Gadget * );
void AttachGads( void );
void DetachGads( void );
void LayoutGads( void );
struct Gadget *GetGad( UBYTE * );
void FreeGad( struct Gadget * );
void ChangeGadImage( struct Gadget *g, UBYTE *name );
struct Window *GetControlWindow( void );

/*
** User functions
*/

BOOL FindUserIdx( char *, struct UserIdx * );
LONG IsUserOnline( char * );
BOOL DoesUserExist( char * );
long DeleteUser( char * );
long InitUserBase( void );                      /* Private */
long QuitUserBase( void );                      /* Private */
struct UserKey *InitUserKey( UWORD );
BOOL NextUserKey( struct UserKey * );
void EndUserKey( struct UserKey * );
struct User *LoadUserFromKey( struct UserKey * );
struct User * LoadUser( char * );
int UnLoadUser( struct User * );
struct User * MakeNewUser( char * );
int SaveUser( struct User * );
long NumUsersOnline( void );
BOOL CheckBan( UBYTE * );
BOOL AddUserXData( struct User *, APTR, ULONG, ULONG );
BOOL RemUserXData( struct User *, ULONG );
ULONG GetUserXData( struct User *, APTR, ULONG, ULONG, ULONG );
ULONG GetUserXDataSize( struct User *, ULONG );
long LoadUserXData( struct User * );
BOOL TrimUserXData( struct User *, ULONG, ULONG, ULONG );

/*
** File transfer functions
*/

struct XferInfo *FileXfer( ULONG Mode, ULONG Protocol, char *FileNames, struct Line *line );
struct XferInfo *FileMarkXfer( ULONG, struct Mark *, struct Line *, ULONG );
void FreeXferInfo( struct XferInfo *info );

/*
** Time Functions
*/

time_t Time( time_t * );
void GMTime( time_t, struct tm * );
time_t MkTime( struct tm * );

/*
** Pipe functions
*/

APTR OpenPipe( char *, LONG, ULONG );
void ClosePipe( APTR, ULONG );
long WritePipe( APTR, UBYTE *, LONG );
long ReadPipe( APTR, UBYTE **, long );
void RequestPipe( APTR, struct Message *, ULONG );
long PipeBufSize( APTR );
void SignalPipeReaders( APTR, ULONG );
void AddPipeSignal( APTR, struct PipeSig * );
void RemPipeSignal( APTR, struct PipeSig * );
long Clip2Pipe( APTR, long );
BOOL WaitPipe( APTR, UBYTE );

/*
** User/Line Message functions
*/

BOOL SendLineMsg( long, char *, BOOL );
BOOL WriteUserLog( char *, char * );

/*
** Editing functions
*/

APTR InitEdit( struct EditOpts * );
void FreeEdit( APTR );
long DoEdit( APTR, UBYTE );
long RedrawEdit( APTR );
void ResetEdit( APTR );

/*
** Bulletin functions
*/

void FreeBulletins( void );
LONG SaveBulletins( void );
LONG LoadBulletins( void );
LONG AddBulletin( UBYTE *, UBYTE *, UBYTE *, time_t );
LONG RenumberBulletins( void );
BOOL RemBulletin( UBYTE *, LONG );
struct List *ReadLockBltnList( void );
struct List *WriteLockBltnList( void );
void UnlockBltnList( void );

/*
** Door I/O and Text Functions
*/

BOOL  DisplayFile( UWORD, char *, ULONG );
BPTR  OpenTextFile( UWORD, char * );
LONG  WriteLine( int, APTR, int, int );
void  ReadLine( struct ReadVars * );
UBYTE GetHotChar( long, BOOL );
long  ReadLineTagList( long, UBYTE *, struct TagItem * );
BOOL  GetYesNo( long, BOOL );
BOOL  PreLoadText( UBYTE * );
void  FreeAllText( void );
BOOL  AddHistList( long, struct List * );
void  RemHistList( long );
void  ClearHistList( struct List * );
BOOL  SetCLIMore( long, UWORD );
long  BootUser( long );
long  LVPrintf( long, ULONG, UBYTE *, LONG * );
long  LPrintf( long, ULONG, UBYTE *, ... );

/*
** Global command functions
*/

WORD   LoadGCmds( void );
void   UnLoadGCmds( void );
UBYTE *FindGCmd( UBYTE * );
void   FreeGCmd( void );

/*
** Watcher functions
*/

APTR AddWatcher( long, struct MsgPort * );
void FreeWatcher( APTR );
BOOL SignalWatchers( long, ULONG, ULONG );
struct Screen *LockWatcherScreen( long );
void FunctionKey( long, struct FKey *, APTR, UBYTE *, BOOL );
APTR BuildZMenu( UBYTE *, struct NewMenu *, ULONG, struct Menu ** );
void FreeZMenu( APTR );
BOOL StartMenuProg( LONG, UBYTE *, UBYTE * );
void LoadWTitles( void );
void FreeWTitles( void );
UBYTE *GetWTitle( long );
void ProcSW( struct WatcherMsg * );

/*
** ANSI Music functions
*/

APTR InitMusic( void );
void FreeMusic( APTR );
BOOL PlayMusic( APTR, UBYTE * );

/*
** Language functions
*/

APTR LoadLanguage( UBYTE * );
void FreeLanguage( APTR );
UBYTE *GetLangStr( APTR, UBYTE * );
BOOL ReLoadLanguage( APTR );
struct Language *LoadLangExtra( UBYTE * );
void FreeLangExtra( struct Language * );

/*
** Last calls
*/

long AddLastCall( struct Line *le );
long LoadLastCalls( void );

/*
** Custom segment loading
*/

struct ZSegs *ZLoadSeg( UBYTE * );
void ZUnLoadSeg( struct ZSegs * );

/*
** Rexx functions
*/

long           rfNextArg( APTR, UBYTE *, long );
struct Line   *rfGetLine( APTR );
UBYTE         *rfGetArg( APTR, long );
void           rfReply( APTR, long, UBYTE * );
struct RDArgs *rfReadArgs( APTR, UBYTE *, LONG * );

struct Line_Node *rfGetLineNode( APTR );        /* Private */

void LLVPrintf( long, UBYTE, UBYTE *, APTR );
void ELVPrintf( UBYTE, UBYTE *, APTR );

void LLPrintf( long, UBYTE, UBYTE *, ... );
void ELPrintf( UBYTE, UBYTE *, ... );

/*
** User -> Address Mapping Functions
*/

BOOL LoadMapCfg( void );
void FreeMapCfg( void );
BOOL FindMapEntry( UBYTE *, UBYTE *, struct FidoAddress *, UBYTE * );
BOOL FindMapInternetEntry( UBYTE *, UBYTE *, struct FidoAddress *, UBYTE * );

/*
** InterNet configuration
*/

struct IN_Req *GetINetCfg( BOOL );
void FreeINetCfg( void );

/*
** Fido Product ID database functions
*/

void LoadFIDdb( void );
void FreeFIDdb( void );
BOOL GetFID( UWORD, struct fid * );

UWORD IOCtrl( long, UWORD );
void CFGtoIO( struct Dev_Cfg *, struct IOExtSer * );

#endif   /* CLIB_PIPELINE_PROTOS_H */
