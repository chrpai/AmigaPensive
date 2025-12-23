#include <exec/types.h>

short ReplaceText( char *line, char *from, char *with );
long GiveAmigaDays( struct IsDate *date );
void FormDate( struct IsDate *date, long *parts );
void AlterDate( struct IsDate *date, long i );
void UltimateFindParts( char *buffer, long min, long max, long *nparts, long *parts );
void ExpandFlags( long def, char *where );
long ConvertAccess( char *s );
void OneLessUser( struct NewSubboardType *s );
UBYTE OneMoreUser( struct NewSubboardType *s, UBYTE lock );
long SaveFree( struct NewSubboardType *s );
long FreeText( struct NewSubboardType *s, long pos, long length );
long AllocText( struct NewSubboardType *s, long length );
void LockAccount( short acc );
void UnLockAccount( short acc );
long FindPhone ( short *n, char *phone, short id );
long FindHandle( short *n, char *name,  short id );
void FormatVanilla( BPTR in, BPTR out );
void MakeDateZone( struct IsDate *date, char *whereto,
		   char **text, BYTE tz, BYTE form  );
void ZPutItem( struct ItemType3 *Item0, struct ItemHeader *ihead0,
	       struct NewSubboardType *s, short c );
void ZGetItem( struct ItemType3 *Item0, struct ItemHeader *ihead0,
	       struct NewSubboardType *s, short c );
UBYTE ZAddItem( struct ItemType3 *Item0, struct ItemHeader *ihead0,
	        struct NewSubboardType *s );
void ZAddMessage( struct MessageType3 *Message0, struct NewSubboardType *s );
void DelayAddItems( struct NewSubboardType *s );
void FlushAddItems( struct NewSubboardType *s );

#pragma libcall CNetBase ReplaceText       1e a9803
#pragma libcall CNetBase GiveAmigaDays     24 801
#pragma libcall CNetBase FormDate          2a 9802
#pragma libcall CNetBase AlterDate         30 0802
#pragma libcall CNetBase UltimateFindParts 36 a910805
#pragma libcall CNetBase ExpandFlags       3c 8002
#pragma libcall CNetBase ConvertAccess     42 801
#pragma libcall CNetBase OneLessUser       48 801
#pragma libcall CNetBase OneMoreUser       4e 0802
#pragma libcall CNetBase SaveFree          54 801
#pragma libcall CNetBase FreeText          5a 10803
#pragma libcall CNetBase AllocText         60 0802
#pragma libcall CNetBase LockAccount       66 001
#pragma libcall CNetBase UnLockAccount     6c 001
#pragma libcall CNetBase FindPhone         72 09803
#pragma libcall CNetBase FindHandle        78 09803
#pragma libcall CNetBase FormatVanilla     7e 9802
#pragma libcall CNetBase MakeDateZone	   84 10a9805
#pragma libcall CNetBase ZGetItem	   8a 0a9804
#pragma libcall CNetBase ZPutItem	   90 0a9804
#pragma libcall CNetBase ZAddItem	   96 a9803
#pragma libcall CNetBase ZAddMessage	   9c 9802
#pragma libcall CNetBase DelayAddItems     a2 801
#pragma libcall CNetBase FlushAddItems     a8 801

short SkipDrop( short d, short stop, UBYTE quiet );
char *RexxModem( char *s );
void UploadQWK( void );
UBYTE HasMaint( void );
void ManageFile0( void );
void UnManageFile0( void );
void CopyCDROM( struct SelectType *select, UBYTE create );
void RequestFiles( void );
void ListMembers( void );
void DirectFileLoop( void (*dfunct)( char *name ) );
void PostNetwork( void );
void SetHeaderDates( void );
UBYTE CanPostUUCP( void );
void FormatFidoEd( struct FidoNet *fidonet, struct FidoType *net, short sig, char *area );
void FormatVanillaEd( char *group, char *subject, char *dist, char *organ );
void ReadUUCP( void );
short FindItem( long number );
UBYTE TempMoveToSub( short sub, long item, UBYTE show );
void ReturnToSub( short list0, short br0 );
void VDEclose( void );
void VDEapply( void *data0, void *data1 );
long DoEvents( struct Event **e0, long *last );
void AutoJoinLink( char *dial );
void JoinLink( void );
void DropDTR( void );
UBYTE SaveAccount( struct UserData *user, short id );
UBYTE ReadAccountLock( short id, struct UserData *user );
UBYTE ReadAccount( short id, struct UserData *user );
struct PortData *IsOnLine( short acc );

void Squery( struct RangeContext *rc, char *command );
BPTR OpenAppend( char *name );
UBYTE TimedWaitIO( struct IORequest *areq, ULONG wbit, ULONG secs );
void LoadInfo( UBYTE close );

void YMarkRec( BPTR kp, long pos, struct HeaderType *head );
void SetDownloadCredits( void );
void AdoptQWK( void );
short IsAMember( char *name );
void GetBaseUser( short id, struct BaseUser *bp );
void PutBaseUser( short id, struct BaseUser *bp );

BPTR OpenPost( BPTR was );
void EditItems( void );
UBYTE EditOne( long r );
void QuickSearch( void );
UBYTE DupCheck( char *name );

char *RexxGetUser( char *arg );
char *RexxPutUser( char *arg );

UBYTE ReadHeader( BPTR fh, long pos, struct HeaderType *head );
void BaseSaveEditor( BPTR fh, short num );
long SizeOfMessage( short num );
long SaveText( long prev, short sig );
long SaveResponse( void );
long SaveItem( short sig );
long KillResponse( long pos );
long NewText( long pos, short sig );
long EditResponse( long pos );

void MarkReceived( char *path, long pos );
long LocateResponse( BPTR fh, long resp );
void GetAllItems( void );
void AdoptLocalDefaults( void );

void ReadM0( void );
void SetSubStuff( short base );
void DisplaySubPath( void );
void ShowGlobalPath( void );

void ReadMarked( void );
void ExtSetMinFree( long free );
UBYTE ExtSetProtocol( char a );
char *ExtDownload( char *args );
char *ExtUpload( char *args );

void FixCase( USHORT opts );
void MakePath( char *where );
void ChangeExchange( void );
void SetCapture( void );
void OpenCapture( char force );
void WriteSubboard( void );
short SetSigNumber( short num );
void OpenYank( short parm );
void ReadOne( void );
void GetMessage( long b );
void Rquery( char *command, UBYTE deftype );
UBYTE GoItem( UBYTE nof, UBYTE file );
UBYTE IsPrivate( void );
UBYTE IsUnvalidated( void );
UBYTE ReadJoined( struct BaseUser **p, short r );
UBYTE ChangeDate( struct IsDate *date );
void ChangeNew( void );
UBYTE Rsetup( UBYTE deftype, UBYTE chrono );
UBYTE Rnext( void );
void SToUpper( char *s );
void AMLogWrite( char *line );
void ArrangeItemPointers( void );
UBYTE SearchOne( void );

void MultiMail( struct MailHeader *rh );
UBYTE MailSetup( UBYTE type );
void MailCount( short i );
void ExitSubboard( UBYTE quiet );

void SendToMailBox( void );
void ScanForNewFiles( void );
void ScanForNewMessages( void );
void NewScanAtLogon( void );
short VisualDataEditor( char *file, void *data, long size );

short CountMail( char *filename, short *nnew );
void ShowMailHeader( struct MailHeader *m, short number, short of, short samv );
UBYTE SelectAndDownload( char *file, UBYTE now );

void LoadSelectList( void );
void SaveSelectList( void );
void SaveSelectListLock( void );

void LoadBaseUser( void );
void SaveBaseUser( void );

void LogOffUser( void );
void TermLink( void );
void DoPortMonitor( void );
void DoTermLink( void );
void DoRange( struct RangeContext *rc, long min, long max, long defmin, long defmax );
long GetNext( struct RangeContext *rc );

void SetByAndTo( void );
UBYTE existf( char *name );
void AdoptItemStuff( void );
UBYTE SetItemStuff( UBYTE qwk );
void TestOne( void );
void TestItems( void );
void TransformOne( void );
void TransformItems( void );

void AskAPlan( UBYTE time, UBYTE sysop );
void SafeFreeMem( void *b, int s );
void dispRexxPort( void );
void NormalizeScreen( void );
void CallXPR( void );
UBYTE ChangeDir( char *name );
void BrowseItems( void );
void EraseChat( void );
UBYTE *FilePart( UBYTE *a );
void GetSkyBrush( BPTR fd );
void LoadBBSCharges( void );
short LoadBBSText( char *loc, char **bm, char *name, long size );
void LoadXLation( short t );
void NormalizeColors( void );
void ResetSRead( UBYTE on );
void Rreset( void );
UBYTE RunParagon( char *name );
UBYTE RunC( char *name, char *args );
UBYTE RunCPfile( char *name );
UBYTE RunRexx( char *name );
void sendcanx( void );
void SetCreditStuff( void );
void SetSelect( UBYTE *a );
void SetUtilityClock( long mics );
void ShowAccess( void );
void ShowTimeLeft( void );
char WaitForInput( long mics );
UBYTE YesNo( UBYTE def );

void PutError( char *what );
void GotoResponse( long n, BPTR fh );
short FindArc( char *name );
UBYTE CheckSysPassword( short x, char *banner );
void SilentUpload( char p );
void SilentDownload( char p );
UBYTE Unknown( void );
UBYTE CheckBBSTimes( void );
UBYTE ReadGraphics( char *f, char fnf );
void WriteLog( short n, char *text1, char *text2 );
void VerifyOff( void );
void WaitScreen( void );
void ModemCommand( char *s );
UBYTE WaitForCall( void );
void LogonScreen( void );
void ResetLine( void );
void ClearLine( void );
void UserStatus( struct UserData *user, short ii );
short FindAccount( char *text, struct UserData *user );
void UpdatePrivs( struct UserData *u );
void UpdateAccess( BYTE access );
void CheckChanges( void );
long CommonCommands( void );
UBYTE ReadLog( char *f );
void SendMail( BPTR fh, UBYTE qwk );
short FollowForward( short id, struct UserData *u );
short WriteMail( struct MailHeader *m, char type, BPTR fh,
	short id, struct UserData *u,
	UBYTE overrider, UBYTE saveopts );
UBYTE CheckUrgentMail( void );
void CheckMail( void );
void ReadMail( char *filename, short samv );
void CopyMessage( BPTR fh, BPTR kh );
void TimeDate( void );
void Feedback( void );
UBYTE LoginUser( void );
void SetColors( short off );
short CalculateAge( struct UserData *user );
void OpenChat( void );
void CloseChat( void );
void CloseDisplay( void );
UBYTE OpenDisplay( void );
void WriteSAMSummary( void );
void BadLogonReason( char *s );
void ParseCommandLine( UBYTE numargs );
short FindCommand( short menu );

UBYTE GetFilename( char *action );
void ReadDirectory( short parent );
void EntryPath( UBYTE exit );
void EditEntryFile( UBYTE exit );
void EntryFile( UBYTE exit );
void ListSubboards( void );
long ListSelected( UBYTE optdel );
short FindSelect( char *name );
short FindUpload( char *name );
void NothingSelected( void );
void NotValidated( void );
void Base( UBYTE base );
UBYTE OtherStuff( short r, UBYTE quiet, UBYTE enter );
void MoveToList( short r );
void MoveToSub( short r, UBYTE quiet );
void SetNewMessages( UBYTE quiet );
void SetThePrompt( void );
UBYTE NewItem( struct IsDate *date, struct IsDate *base );
int GetItemNumber( void );
void PostMessage( UBYTE qwk );
UBYTE OKFilename( char *name );
void UploadFiles( void );
UBYTE FileItem( void );
void NextItem( void );
void LastPost( void );
void NoNumber( void );
void OutScan( void );
void ScanItems( void );
char DoAnonymous( UBYTE to );
void AboutPost( UBYTE new );
UBYTE ValidateOne( UBYTE ud );
void ValidateItems( void );
void PutItem( short b );
void GetItem( short b );
void PrintOption( char *s, UBYTE *o );
void KillItems( void );
void RemovePost( UBYTE killit );
void SaveHeaders( void );
void GrabItems( void );
UBYTE GrabOne( void );
void GrabFile( char *name );
void DownloadFiles( void );
void PostHeader( void );
void MessageHeader( void );
void ReadItems( void );
void AskDescription( void );
void AddDescription( void );
UBYTE PromptFileNote( char *filename );
void AddFileNote( char *file );
void LimitEditor( char *loc );
UBYTE SetFrom( void );
UBYTE SetMessageStuff( UBYTE qwk );
void RespondOne( UBYTE qwk );
void RespondItems( void );
int ScanAll( void );
int ScanNew( UBYTE quiet );
int BetweenNew( void );
void ViewSubOps( void );
void DescribeOne( void );
void DescribeItems( void );
void SelectOne( UBYTE remove );
void SelectItems( UBYTE remove );
void SelectFile( UBYTE counttime, UBYTE remove );
void RemoveSelect( short i );
void RemoveUpload( short i );
void ExtractOne( void );
void ExtractItems( void );
void ExtractFile( char *name );
void SToLower( char *s );
void CheckString( char *string );
void SearchHere( void );
UBYTE SearchText( UBYTE filters );
void SearchEnd( void );
void TraverseAll( int (*Func1)( void ), int(*Func2)( void ) );
int TimeEstimate( int bytes );
void AdoptOrphans( void );
void DescribeBatch( char *shortfile );
UBYTE CheckPrivacy( struct HeaderType *h, UBYTE quiet );
UBYTE PrivateItem( void );
UBYTE VerifyKill( void );
void MakeLoc( short p );
void FindLoc( void );
long FindLargest( UBYTE quiet );

short GetWhichPort( UBYTE MustBeOn);
void SendOLM( void );
UBYTE FileOLM( short port, int nlines );
void ReadOLM( void );
long ConferenceWait( short r );
int EnterText( char firstkey, short max, short perline, short maxlines );
void WhosHere( void );
void HideOpts( char **text, UBYTE *all, UBYTE value );
void CheckTellMe( char *s );
void InterUserChat( short port );
void ChatRequest( void );
UBYTE CheckIUC( USHORT opts );

UBYTE RunDOS( char *name, char *arguments );
void AddToBuffer( char c );
long FileSize( char *path );
long GrabFileSize( UBYTE quiet );
long ListDir( UBYTE add, UBYTE counttime, struct IsDate *date );
UBYTE OkName( char *n );
void GetInfo( char *name, char *output );
long GetFree( char *where, UBYTE quiet );
void FindUnits( ULONG size, char *unit, ULONG chunk );

void EditorPrint( char *text );

void MakeEd( char *path );
void DeleteEd( void );
BPTR OpenEd( long mode );
void PrepEditor( BPTR fh );
void CallEditor( short max, short inlines );
void SaveEditor( BPTR fh, UBYTE eof );
char LineEditor( short max, UBYTE flags );
char ExitSaveEditor( void );
void DoMCIRead( void );
void DoZip( void );
UBYTE CJustify( char *prompt, void (*fn)() );
void PutString( short line, short ww );
void InsPoint( short line );
void InsChar( short line, short col, short n );
void DelChar( short line, short col, short n );
void DelPoint( short line );
void FunctLoop( void (*funct)(), short returns );
UBYTE EdFindParts( UBYTE d );
void FList( long line );
void FnRead( long line );
void FEdit( long line );
void FReplace( long line );
void FToggle( long line );
void CDelete( void );
void CBorder( void );
void FCopy( long line );
void CCopy( short *line );
void FLeft( long line );
void FRight( long line );
void FCenter( long line );
void FPacked( long line );
void FIndent( long line );
void FUnindent( long line );
void FExpand( long line );
void FFind( long line );
void CGet( void );
void CPut( void );
void MakePrompt( char *s );

void PrintAGraph( void );
void AddSAM( short a, long b );
void DelSAM( short a, long b );
void DisplaySAM( void );
void SaveSAM( void );
void EditAccount( void );
UBYTE KillAccount( short ii );
int EditHelp( UBYTE x );
void EditGroup( void );
void EditPrivs( struct Privs *p );
void EditPrivFlags( struct Privs *p );
void AccountSummary( UBYTE detail );
void PrintSigned( int n, char *s );
UBYTE CheckBalance( short n, short a );
UBYTE AddCharge( short n, short a );
void MyCredits( struct UserData *u );

short compstra( char *s, char *t );
void ResetServ( UBYTE reset, UBYTE time );
void AddKeyBuffer( char *c );
void ConGetChar( void );
void SerGetChar( void );
void ClearBuffer( void );
long GetABunch( char *buffer, long number );
void PutChar( char s );
char GetChar( ULONG mask );
void PrintAnything( char *s );
void DoPause( void );
void PrintSlow( char *wherefrom, short howmany );
void WaitModem( void );
void CursorOff( void );
void CursorOn( void );
void PrintWorkbench( char *where, short howmany );
void PrintPrinter( char *wherefrom, short howmany );
void PrintConsole( char *where, short howmany );
void PrintModem( char *where, short howmany );
char OneKey( void );
void ScreenBS( void );
void DoBackspace( short count );
void ScrollUp( void );
void DoReturn( void );
UBYTE ShowAbortKey( void );
void CheckFlowControl( void );
void PutText( char *wherefrom );
UBYTE PutQ( char *a );
void PutA( void );
void SetBaudRate( ULONG baud );
void Service( void );
void CheckCarrier( void );
void EnterPassword( UBYTE maxlen );
UBYTE DoANSIBox( UBYTE maxlen, USHORT opts, char *prompt, short len );
int EnterLine( UBYTE maxlen, USHORT opts, char *prompt );
UBYTE CheckInputChar( UBYTE gfx );
UBYTE ReadFile( char *filename, UBYTE fnf );
void ReadText( BPTR fh, char *buffer );
void ReadMessage( BPTR fh );
void ReadMessagePoint( char *name, long pos );
UBYTE LoadLine( BPTR fh, char *line, short w, short flags );
void EditMessage( char *file );
void EditMessageLines( short max, char *file );
void MakeDate( struct IsDate *date, char *whereto );
void GetDate( void );
void ResetTime( void );
void StatPrint( char *text, int XOffset, int YOffset, UBYTE colors );
void DisplayStatScreen( void );
void ShowCredits( void );
void LoAccess( void );
UBYTE NoMessages( void );
long EnterAccess( char *prompt, long def );
char *PrintAccess( int a, short n );
short DeadKeyConvert( struct IntuiMessage *msg );
void Print40( void );
void ResetEnviron( void );
void ResetFont( void );
void AttemptOpenDisplay( void );

void ListItems( void );
void AddItem( void );
void SaveItemList( void );
void KillItem( short i, UBYTE quiet );
void EditItem( short i );
UBYTE MakeChanges( struct ListItem *f );
void CheckNews( void );
void DoItem( struct ListItem *f );
void DoFiles( short n );
UBYTE ObtainPfile( char *name, UBYTE restrict );
void ReleasePfile( void );
void AlphabetizeItems( void );

void DoMCI( char *string );
void PutNumber( ULONG n );
void SOHelp( short n );
void DoStillOn( void );
void DoCCG( UBYTE c );
void DoANSI( UBYTE n, USHORT a, USHORT b );
void DoANSIOut( UBYTE n );
void EraseLineEnd( void );
void EraseLineBegin( void );
void AScrollUp( short line, short num );
void AScrollDown( short line, short num );
void DoSky( short which );
void FreeSkyBrush( void );

void SaveAlpha( void );
void SaveNos( void );
UBYTE DoGeneral( short n );
UBYTE EditProfile( void );
UBYTE DoParams( void );
UBYTE EditParams( void );
void NewUser( void );
UBYTE CheckBadFiles( char *f, char *s, char *d );
short CheckHandle( void );
void EditGeneral( short i );
UBYTE EditPrefs( void );
UBYTE DisplayList( char **text, short n, char *prompt, short def );
void ListComp( void );
void AutoCBV( short force );

void ToggleBott( char mode );
void ToggleStat( char mode );
void DoWEdit( UBYTE c );
void SetDoing( char *what );
void InitMenu( void );
void ControlCommand( short c );

void Terminal( void );
void DoTermMenu( long code );
void AdjustPath( void );

void CarriageReturn( void );
void ClearStr( void );
short Findend( void );
void PutStr1( void );
void GetStr( void );
void Position( void );
void Position2( void );
void OutLine( short col );
void ClearToEnd( void );
char DoVisual( void );
char DrawEditor( void );
void DoInsLine( void );
void DoEOP( void );
void VInsChar( short n );
void SetOut( void );
void GoCorner( void );
void GoHome( UBYTE title );
void SendHome( void );
void GoLeft( void );
void GoRight( void );
void GoUp( void );
void SinsMany( short a );
void SdelMany( short a );
void GoDown( void );
void DoLeft( void );
void DoRight( void );
UBYTE DoBS( void );
void DoCR( void );
void VDelChar( short x );
UBYTE DoDelete( void );
void DoTab( void );
void DoILine( void );
void DoDLine( void );
void Pullups( void );
void RemoveMCI( char *b );

void zprprotocol( char *a );
void printsize( ULONG size );
void zprbytes( long val );
void zprblocks( USHORT b );
void zprblocksize( USHORT b );
void printerrors( long e );
void plotstatus( char *message );
void ploterrors( char *message );
void plotblockcheck( char *message );
void zprname( char *a );
void endxfer( void );
void AddDownload( void );
void AddCredits( UBYTE justadd, short m );
void DLoadSelect( UBYTE optdel );
void ULoadSelect( void );
void XAddFile( void );
UBYTE CheckTime( void );
UBYTE DCheckOK( struct SelectType *sp, short number );
UBYTE UCheckOK( UBYTE mode );
UBYTE OKProtocol( void );
UBYTE ChooseProtocol( struct UserData *u );
UBYTE MatchProtocol( UBYTE what );
UBYTE XOptions( char p );
void LogTransfer( char type, UBYTE log );
void AddTimeBenefit( void );
void CheckVerification( void );
void CloseTransfer( void );
UBYTE OpenTransfer( void );

void  CapsFirst( char *s );
short instr( char *s, char *t );
