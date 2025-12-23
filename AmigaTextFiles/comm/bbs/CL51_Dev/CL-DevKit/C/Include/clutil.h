/*
** "clutil.library" definitions
**
** $VER: 62.0
**
*/

#ifndef _CL_CLUTIL_H
#define _CL_CLUTIL_H

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif

#ifndef EXEC_MEMORY_H
#include <exec/memory.h>
#endif

#ifndef DOS_DOS_H
#include <dos/dos.h>
#endif

#ifndef _TIME_H
#include <time.h>
#endif

#ifndef _NO_PRAG
/*
 *  SAS/C pragmas
 */
#pragma libcall CLUtilBase CLU_ShowRequesterA 24 BA9804
#pragma tagcall CLUtilBase CLU_ShowRequester 24 BA9804
#pragma libcall CLUtilBase CLU_OpenLibrary 2A 0802
#pragma libcall CLUtilBase CLU_OpenFileTimeout 30 10803
#pragma libcall CLUtilBase CLU_DeleteDir 4E 801
#pragma libcall CLUtilBase CLU_CopyFile 54 9802
#pragma libcall CLUtilBase CLU_MoveFile 5A 9802
#pragma libcall CLUtilBase CLU_CreateFuelGauge 60 93210806
#pragma libcall CLUtilBase CLU_DeleteFuelGauge 66 801
#pragma libcall CLUtilBase CLU_DrawFuelGauge 6C 10803
#pragma libcall CLUtilBase CLU_SprintfA 72 A9803
#pragma tagcall CLUtilBase CLU_Sprintf 72 A9803
#pragma libcall CLUtilBase CLU_OpenLibraryCode 78 001
#pragma libcall CLUtilBase CLU_CRC16 90 0802
#pragma libcall CLUtilBase CLU_CRC32 96 0802
#pragma libcall CLUtilBase CLU_rstrchr 9C 0802
#pragma libcall CLUtilBase CLU_RefreshFuelGauge A2 801
#pragma libcall CLUtilBase CLU_Get8Font A8 0
#pragma libcall CLUtilBase CLU_Get8TextAttr AE 0
#pragma libcall CLUtilBase CLU_CloseScreenSafe B4 801
#pragma libcall CLUtilBase CLU_SetABPenDrMd BA 210804
#pragma libcall CLUtilBase CLU_OpenFileReally C0 801
#pragma libcall CLUtilBase CLU_clnl C6 801
#pragma libcall CLUtilBase CLU_CD CC 801
#pragma libcall CLUtilBase CLU_Examine D2 9802
#pragma libcall CLUtilBase CLU_GetFileLen D8 801
#pragma libcall CLUtilBase CLU_SetWaitPointer DE 801
#pragma libcall CLUtilBase CLU_LockWindow E4 801
#pragma libcall CLUtilBase CLU_UnlockWindow EA 801
#pragma libcall CLUtilBase CLU_ELGetNumEntries F0 801
#pragma libcall CLUtilBase CLU_ELGetEntryNum F6 9802
#pragma libcall CLUtilBase CLU_ELGetEntry FC 0802
#pragma libcall CLUtilBase CLU_DrawRLX 102 210804
#pragma libcall CLUtilBase CLU_DrawRLY 108 210804
#pragma libcall CLUtilBase CLU_strcpylw 10E 9802
#pragma libcall CLUtilBase CLU_strcpyup 114 9802
#pragma libcall CLUtilBase CLU_ObtainNamedSemaphore 11A 0802
#pragma libcall CLUtilBase CLU_FreeNamedSemaphoreName 120 801
#pragma libcall CLUtilBase CLU_FreeNamedSemaphore 126 801
#pragma libcall CLUtilBase CLU_GetNameVal 12C 801
#pragma libcall CLUtilBase CLU_GetSeqNum 132 0
#pragma libcall CLUtilBase CLU_GetSeqNumInc 138 001
#pragma libcall CLUtilBase CLU_MakeNumName 13E 821004
#pragma libcall CLUtilBase CLU_iso2ibm 144 001
#pragma libcall CLUtilBase CLU_ibm2iso 14A 001
#pragma libcall CLUtilBase CLU_siso2ibm 150 801
#pragma libcall CLUtilBase CLU_sibm2iso 156 801
#pragma libcall CLUtilBase CLU_MakePath 15C 801
#pragma libcall CLUtilBase CLU_AllocHH 162 0
#pragma libcall CLUtilBase CLU_DelHL 168 801
#pragma libcall CLUtilBase CLU_FreeHH 16E 801
#pragma libcall CLUtilBase CLU_AddHL 174 A9803
#pragma libcall CLUtilBase CLU_CloneHH 17A 801
#pragma libcall CLUtilBase CLU_DumpHH 180 09803
#pragma libcall CLUtilBase CLU_LoadHH 186 801
#pragma libcall CLUtilBase CLU_GetHL 18C 801
#pragma libcall CLUtilBase CLU_GetNextHL 192 801
#pragma libcall CLUtilBase CLU_FindHL 198 9802
#pragma libcall CLUtilBase CLU_FindNextHL 19E 801
#pragma libcall CLUtilBase CLU_GetHLData 1A4 801
#pragma libcall CLUtilBase CLU_GetHLHeader 1AA 801
#pragma libcall CLUtilBase CLU_DelHLName 1B0 9802
#pragma libcall CLUtilBase CLU_SetHLData 1B6 9802
#pragma libcall CLUtilBase CLU_AttemptNamedSemaphore 1BC 0802
#pragma libcall CLUtilBase CLU_OpenAsync 1C2 10803
#pragma libcall CLUtilBase CLU_CloseAsync 1C8 801
#pragma libcall CLUtilBase CLU_ReadAsync 1CE 09803
#pragma libcall CLUtilBase CLU_ReadCharAsync 1D4 801
#pragma libcall CLUtilBase CLU_WriteAsync 1DA 09803
#pragma libcall CLUtilBase CLU_WriteCharAsync 1E0 0802
#pragma libcall CLUtilBase CLU_SeekAsync 1E6 10803
#pragma libcall CLUtilBase CLU_CreatePool 1EC 21003
#pragma libcall CLUtilBase CLU_DeletePool 1F2 801
#pragma libcall CLUtilBase CLU_AllocPooled 1F8 0802
#pragma libcall CLUtilBase CLU_FreePooled 1FE 09803
#pragma libcall CLUtilBase CLU_AllocVecPooled 204 0802
#pragma libcall CLUtilBase CLU_FreeVecPooled 20A 9802
#pragma libcall CLUtilBase CLU_LoadHHfromMem 210 801
#pragma libcall CLUtilBase CLU_CRC16S 216 10803
#pragma libcall CLUtilBase CLU_CRC32S 21c 10803
#pragma libcall CLUtilBase CLU_FlushPort 222 801
#pragma libcall CLUtilBase CLU_StrDupPooled 228 9802
#pragma libcall CLUtilBase CLU_GetHL_Len 22e 801
#pragma libcall CLUtilBase CLU_DumpHHAsync 234 09803
#pragma libcall CLUtilBase CLU_ZCDatestringToDI 23a 9802
#pragma libcall CLUtilBase CLU_ZCDIToDatestring 240 9802
#pragma libcall CLUtilBase CLU_CopyData 246 09803
#pragma libcall CLUtilBase CLU_CopyDataAsync 24c 09803
#pragma libcall CLUtilBase CLU_ExpandTemplateA 252 A09804
#pragma tagcall CLUtilBase CLU_ExpandTemplate 252 A09804
#pragma libcall CLUtilBase CLU_FGetsAsync 258 09803
#pragma libcall CLUtilBase CLU_VFPrintfAsync 25e A9803
#pragma tagcall CLUtilBase CLU_FPrintfAsync 25e A9803
#pragma libcall CLUtilBase CLU_FtellAsync 264 801
#pragma libcall CLUtilBase CLU_MUI_MakeStringLinksA 26a 9802
#pragma tagcall CLUtilBase CLU_MUI_MakeStringLinks 26a 9802
#pragma libcall CLUtilBase CLU_AllocFIB 270 0
#pragma libcall CLUtilBase CLU_FreeFIB 276 801
#pragma libcall CLUtilBase CLU_MatchPattern 27c 9802
#pragma libcall CLUtilBase CLU_NOL_NewA 282 801
#pragma tagcall CLUtilBase CLU_NOL_New 282 801
#pragma libcall CLUtilBase CLU_NOL_Delete 288 801
#pragma libcall CLUtilBase CLU_NOL_Clone 28e 801
#pragma libcall CLUtilBase CLU_NOL_Save 294 9802
#pragma libcall CLUtilBase CLU_NOL_Load 29a 9802
#pragma libcall CLUtilBase CLU_NOL_Info 2a0 9802
#pragma libcall CLUtilBase CLU_NOL_FindEntry 2a6 9802
#pragma libcall CLUtilBase CLU_NOL_DeleteEntry 2ac 801
#pragma libcall CLUtilBase CLU_NOL_AddEntry 2b2 B0A9805
#pragma libcall CLUtilBase CLU_NOL_GetEntryName 2b8 801
#pragma libcall CLUtilBase CLU_NOL_GetEntryData 2be 801
#pragma libcall CLUtilBase CLU_NOL_GetNumEntries 2c4 801
#pragma libcall CLUtilBase CLU_NOL_GetNextEntry 2ca 801
#pragma libcall CLUtilBase CLU_NOL_GetPrevEntry 2d0 801
#pragma libcall CLUtilBase CLU_ScanDir 2d6 BA9804
#pragma libcall CLUtilBase CLU_OpenTempFile 2dc 8002
#pragma libcall CLUtilBase CLU_CloseTempFile 2e2 0802
#pragma libcall CLUtilBase CLU_MakeFileList 2e8 09803
#pragma libcall CLUtilBase CLU_FreeFileList 2ee 801
#pragma libcall CLUtilBase CLU_MUI_SetTextA 2f4 A9803
#pragma tagcall CLUtilBase CLU_MUI_SetText 2f4 A9803
#pragma libcall CLUtilBase CLU_MUI_InsertSingleStringA 2fa A90804
#pragma tagcall CLUtilBase CLU_MUI_InsertSingleString 2fa A90804
#pragma libcall CLUtilBase CLU_AllocVMem 300 1002
#pragma libcall CLUtilBase CLU_FreeVMem 306 801
#pragma libcall CLUtilBase CLU_MUI_RunCommandA 30c A9803
#pragma tagcall CLUtilBase CLU_MUI_RunCommand 30c A9803
#pragma libcall CLUtilBase CLU_NOL_SaveName 312 9802
#pragma libcall CLUtilBase CLU_NOL_LoadName 318 9802
#pragma libcall CLUtilBase CLU_Alert 31e 1002
#pragma libcall CLUtilBase CLU_NOL_GetFirstEntry 324 801
#pragma libcall CLUtilBase CLU_NOL_GetLastEntry 32a 801
#pragma libcall CLUtilBase CLU_NOL_Lock 330 0802
#pragma libcall CLUtilBase CLU_NOL_UnLock 336 801
#pragma libcall CLUtilBase CLU_NOL_SetEntryName 33c 9802
#pragma libcall CLUtilBase CLU_NOL_ChangeCounter 342 0802
#pragma libcall CLUtilBase CLU_NOL_SwapEntries 348 9802
#pragma libcall CLUtilBase CLU_CreatePort 354 0802
#pragma libcall CLUtilBase CLU_DeletePort 35a 801
#pragma libcall CLUtilBase CLU_Exists 360 801
#pragma libcall CLUtilBase CLU_GetScreenPubName 366 9802
#pragma libcall CLUtilBase CLU_ASprintfA 36c A9803
#pragma tagcall CLUtilBase CLU_ASprintf 36c A9803
#pragma libcall CLUtilBase CLU_SkipDot 372 801
#pragma libcall CLUtilBase CLU_UpdateFilename 378 801
#pragma libcall CLUtilBase CLU_CopyFileUpdateFilename 37e A9803
#pragma libcall CLUtilBase CLU_MoveFileUpdateFilename 384 A9803
#pragma libcall CLUtilBase CLU_FGets 38a 09803
#pragma libcall CLUtilBase CLU_SprintfnA 390 A90804
#pragma tagcall CLUtilBase CLU_Sprintfn 390 A90804
#pragma libcall CLUtilBase CLU_NOL_GetEntry 396 0802
#pragma libcall CLUtilBase CLU_RunRexxScript 39c 9802
#pragma libcall CLUtilBase CLU_UnGetCAsync 3a2 801
#pragma libcall CLUtilBase CLU_ReadArgsFromString 3a8 A9803
#pragma libcall CLUtilBase CLU_FreeArgs 3ae 801
#pragma libcall CLUtilBase CLU_OpenLibs 3b4 A9803
#pragma libcall CLUtilBase CLU_CloseLibsA 3ba 8002
#pragma tagcall CLUtilBase CLU_CloseLibs 3ba 8002
#pragma libcall CLUtilBase CLU_TimerBase 3c0 0
#pragma libcall CLUtilBase CLU_BeginTime 3c6 801
#pragma libcall CLUtilBase CLU_EndTime 3cc 801
#pragma libcall CLUtilBase CLU_AvgTime 3d2 0802
#pragma libcall CLUtilBase CLU_FmtTimeval 3d8 9802
#pragma libcall CLUtilBase CLU_Timecvt 3e4 8002
#pragma libcall CLUtilBase CLU_Datecvt 3ea 801
#pragma libcall CLUtilBase CLU_Timestring 3f0 8002
#pragma libcall CLUtilBase CLU_DebugNamedSemaphores 3f6 0
#pragma libcall CLUtilBase CLU_ctype 3fc 0
#pragma libcall CLUtilBase CLU_Copy 402 109804
#pragma libcall CLUtilBase CLU_TestHLValA 408 A9803
#pragma tagcall CLUtilBase CLU_TestHLVal 408 A9803
#pragma libcall CLUtilBase CLU_DelHLNameValA 40e A9803
#pragma tagcall CLUtilBase CLU_DelHLNameVal 40e A9803
#pragma libcall CLUtilBase CLU_CountHLName 414 9802
#pragma libcall CLUtilBase CLU_InsertRotHL 41a 9802
#pragma libcall CLUtilBase CLU_DeleteByPattern 420 9802
#pragma libcall CLUtilBase CLU_DoBackupQueue 426 09803
#pragma libcall CLUtilBase CLU_AddFileSizes 42c 9802
#pragma libcall CLUtilBase CLU_SplitDomainAdr 432 A9803
#pragma libcall CLUtilBase CLU_CalcCRC32File 438 801
#pragma libcall CLUtilBase CLU_time 43e 801
#pragma libcall CLUtilBase CLU_getft 444 801
#pragma libcall CLUtilBase CLU_GetHLDataLong 44a 801
#pragma libcall CLUtilBase CLU_GetHHCharset 450 801
#pragma libcall CLUtilBase CLU_Get_iso2ibm 456 0
#pragma libcall CLUtilBase CLU_Get_ibm2iso 45c 0
#pragma libcall CLUtilBase CLU_biso2ibm 462 0802
#pragma libcall CLUtilBase CLU_CreateProc 468 A9803
#pragma tagcall CLUtilBase CLU_CreateProcTags 468 A9803
#pragma libcall CLUtilBase CLU_AddHLSorted 46e 0A9804
#pragma libcall CLUtilBase CLU_utunpk 474 8002
#pragma libcall CLUtilBase CLU_utpack 47a 801
#pragma libcall CLUtilBase CLU_strnicmp 480 09803
#pragma libcall CLUtilBase CLU_stricmp 486 09803
#pragma libcall CLUtilBase CLU_FmtDateMMDD 48c 8002
#pragma libcall CLUtilBase CLU_bibm2iso 492 0802
#pragma libcall CLUtilBase CLU_CalcCRC16File 498 801
#pragma libcall CLUtilBase CLU_FindInStringTab 49e 9802
#pragma libcall CLUtilBase CLU_DebugSemaphore 4a4 801
#pragma libcall CLUtilBase CLU_GetKeyBase 4aa 0
#pragma libcall CLUtilBase CLU_ResetID 4b0 0
#endif

#ifndef _NO_PROTO
/*
 *
 * ANSI prototypes
 *
 */

LONG CLU_ShowRequesterA(STRPTR,STRPTR,STRPTR,APTR );
LONG CLU_ShowRequester(STRPTR,STRPTR,STRPTR,...);
struct Library *CLU_OpenLibrary(STRPTR,LONG);
BPTR CLU_OpenFileTimeout(STRPTR,LONG,LONG);
LONG CLU_DeleteDir(STRPTR );
LONG CLU_CopyFile(STRPTR,STRPTR );
LONG CLU_MoveFile(STRPTR,STRPTR );
APTR CLU_CreateFuelGauge(struct Window*,LONG,LONG,LONG,LONG,STRPTR );
void CLU_DeleteFuelGauge(APTR );
void CLU_DrawFuelGauge(APTR ,LONG,LONG);
void CLU_SprintfA(STRPTR,STRPTR,APTR );
void CLU_Sprintf(STRPTR,STRPTR,...);
void CLU_ASprintfA( STRPTR, STRPTR, APTR );
void CLU_ASprintf( STRPTR, STRPTR, ... );
struct Library *CLU_OpenLibraryCode(LONG);
LONG CLU_CRC16(APTR ,LONG);
LONG CLU_CRC32(APTR ,LONG);
STRPTR CLU_rstrchr(STRPTR,char);
void CLU_RefreshFuelGauge(APTR );
struct TextFont *CLU_Get8Font(void);
struct TextAttr *CLU_Get8TextAttr(void);
void CLU_CloseScreenSafe(struct Screen*);
void CLU_SetABPenDrMd(struct RastPort*rp,LONG,LONG,LONG);
BPTR CLU_OpenFileReally(STRPTR );
void CLU_clnl(STRPTR );
LONG CLU_CD(STRPTR );
LONG CLU_Examine(STRPTR,struct FileInfoBlock*);
LONG CLU_GetFileLen(STRPTR );
void CLU_SetWaitPointer(struct Window*);
void CLU_LockWindow(struct Window*);
void CLU_UnlockWindow(struct Window*);
/* v9 */
LONG CLU_ELGetNumEntries(struct List*);
LONG CLU_ELGetEntryNum(struct List*,struct Node*);
struct Node*CLU_ELGetEntry(struct List*,LONG);
void CLU_DrawRLX(struct RastPort *rp,LONG,LONG,LONG);
void CLU_DrawRLY(struct RastPort *rp,LONG,LONG,LONG);
void CLU_strcpylw(STRPTR,STRPTR );
void CLU_strcpyup(STRPTR,STRPTR );
APTR CLU_ObtainNamedSemaphore(STRPTR,LONG);
void CLU_FreeNamedSemaphoreName(STRPTR );
void CLU_FreeNamedSemaphore(APTR );
long CLU_GetNameVal(char *);
ULONG CLU_GetSeqNum(void);
ULONG CLU_GetSeqNumInc(ULONG);
void CLU_MakeNumName(ULONG,ULONG,ULONG,STRPTR );
ULONG CLU_iso2ibm(UBYTE);
ULONG CLU_ibm2iso(UBYTE);
void CLU_siso2ibm(STRPTR );
void CLU_sibm2iso(STRPTR );
LONG CLU_MakePath(STRPTR );
/* v13.2 */
APTR CLU_AttemptNamedSemaphore(STRPTR,LONG);
/* Interne Datenstrukturen */
typedef APTR HH; /* Headerhandle */
typedef APTR HL; /* Headerline */
HH CLU_AllocHH(void);
void CLU_DelHL(HL);
void CLU_FreeHH(HH);
HL CLU_AddHL(HH,STRPTR,STRPTR );
HH CLU_CloneHH(HH);
LONG CLU_DumpHH(HH,BPTR,ULONG);
HH CLU_LoadHH(BPTR);
HL CLU_GetHL(HH);
HL CLU_GetNextHL(HL);
HL CLU_FindHL(HH,STRPTR );
HL CLU_FindNextHL(HL);
STRPTR CLU_GetHLData(HL);
STRPTR CLU_GetHLHeader(HL);
LONG CLU_DelHLName(HH,STRPTR );
void CLU_SetHLData(HL,STRPTR );
/* AsyncIO (V14) */
struct AsyncFile * CLU_OpenAsync(const STRPTR fileName, UBYTE accessMode, LONG bufferSize);
LONG CLU_CloseAsync(struct AsyncFile *file);
LONG CLU_ReadAsync(struct AsyncFile *file, APTR buffer, LONG numBytes);
LONG CLU_ReadCharAsync(struct AsyncFile *file);
LONG CLU_WriteAsync(struct AsyncFile *file, APTR buffer, LONG numBytes);
LONG CLU_WriteCharAsync(struct AsyncFile *file, UBYTE ch);
LONG CLU_SeekAsync(struct AsyncFile *file, LONG position, BYTE mode);
/* V15 */
APTR  CLU_CreatePool( ULONG, ULONG, ULONG );
void CLU_DeletePool( APTR  );
APTR  CLU_AllocPooled( APTR , ULONG );
APTR  CLU_AllocVecPooled( APTR , ULONG );
void CLU_FreePooled( APTR , APTR , ULONG );
void CLU_FreeVecPooled( APTR , APTR  );
/* V16 */
HH CLU_LoadHHfromMem(STRPTR );
/* V17 */
LONG CLU_CRC16S( APTR , LONG, LONG );
LONG CLU_CRC32S( APTR , LONG, LONG );
void CLU_FlushPort( struct MsgPort * );
STRPTR CLU_StrDupPooled( APTR , STRPTR  );
/* V18 */
LONG CLU_GetHL_Len( HH );
int CLU_DumpHHAsync( HH, struct AsyncFile *tofile, ULONG flags );
/* V19 */
LONG CLU_ZCDatestringToDI( STRPTR, struct ZCDateInfo * );
LONG CLU_ZCDIToDatestring( STRPTR, struct ZCDateInfo * );
/* V20 */
LONG CLU_CopyData( BPTR, BPTR, ULONG );
LONG CLU_CopyDataAsync( struct AsyncFile *, struct AsyncFile *, ULONG );
/* V21 */
ULONG CLU_ExpandTemplateA( STRPTR, STRPTR, ULONG, APTR );
ULONG CLU_ExpandTemplate( STRPTR, STRPTR, ULONG, ... );
UBYTE *CLU_FGetsAsync( struct AsyncFile *, UBYTE *, LONG );
LONG CLU_VFPrintfAsync( struct AsyncFile *, STRPTR, APTR  );
LONG CLU_FPrintfAsync( struct AsyncFile *, STRPTR, ... );
/* V22 */
LONG CLU_FtellAsync( struct AsyncFile * );
/* v23 */
void CLU_MUI_MakeStringLinksA( APTR , APTR * );
void CLU_MUI_MakeStringLinks( APTR , ... );
struct FileInfoBlock *CLU_AllocFIB( void );
void CLU_FreeFIB( struct FileInfoBlock * );
BOOL CLU_MatchPattern( STRPTR, STRPTR );
/* Named Object Lists private Handles */
typedef APTR  NOL;
typedef APTR  NOBJ;
/* V24 */
NOL CLU_NOL_NewA( struct TagItem * );
NOL CLU_NOL_New( ... );
void CLU_NOL_Delete( NOL );
LONG CLU_NOL_Info( NOL,  struct NOL_Info * );
void CLU_NOL_DeleteEntry( NOBJ );
NOBJ   CLU_NOL_FindEntry( NOL,  STRPTR  );
NOBJ   CLU_NOL_AddEntry( NOL,  STRPTR , APTR, ULONG, NOBJ  );
STRPTR    CLU_NOL_GetEntryName( NOBJ );
APTR    CLU_NOL_GetEntryData( NOBJ );
LONG   CLU_NOL_GetNumEntries( NOL );
NOBJ   CLU_NOL_GetNextEntry( NOBJ );
NOBJ   CLU_NOL_GetPrevEntry( NOBJ );
NOL   CLU_NOL_Clone( NOL );
LONG   CLU_NOL_Save( NOL, BPTR );
NOL   CLU_NOL_Load( BPTR, struct NOL_Info * );
/* v26 */
LONG CLU_ScanDir( BPTR, STRPTR, BOOL __asm (*)(register __a1 struct FileInfoBlock *, register __a3 APTR ),  APTR );
/* v27 */
BPTR CLU_OpenTempFile( ULONG, STRPTR );
void CLU_CloseTempFile( BPTR, BOOL );
/* v29 */
struct clu_filelist *CLU_MakeFileList( BPTR, STRPTR, ULONG );
void CLU_FreeFileList( struct clu_filelist * );
/* v30 */
void CLU_MUI_SetText( APTR, STRPTR, ... );
void CLU_MUI_SetTextA( APTR, STRPTR, APTR );
/* v31 */
void CLU_MUI_InsertSingleString( APTR, ULONG, STRPTR, ... );
void CLU_MUI_InsertSingleStringA( APTR, ULONG, STRPTR, APTR );
APTR CLU_AllocVMem( ULONG, ULONG );
void CLU_FreeVMem( APTR );
/* v32 */
LONG CLU_MUI_RunCommandA( APTR, STRPTR, APTR );
LONG CLU_MUI_RunCommand( APTR, STRPTR, ... );
/* v33 */
LONG   CLU_NOL_SaveName( NOL,  STRPTR  );
NOL   CLU_NOL_LoadName( STRPTR, struct NOL_Info * );
/* v35 */
void CLU_Alert( UBYTE, UWORD );
NOBJ CLU_NOL_GetFirstEntry( NOL );
NOBJ CLU_NOL_GetLastEntry( NOL );
ULONG CLU_NOL_ChangeCounter( NOL, BOOL );
void CLU_NOL_Lock( NOL, BOOL );
void CLU_NOL_UnLock( NOL );
void CLU_NOL_SwapEntries( NOBJ, NOBJ );
BOOL CLU_NOL_SetEntryName( NOBJ, STRPTR );
void CLU_ELSwapNodes( struct Node *, struct Node * );
/* v36 */
struct MsgPort * CLU_CreatePort( STRPTR, LONG );
void CLU_DeletePort( struct MsgPort * );
/* v37 */
BOOL CLU_Exists( STRPTR filename );
/* v38 */
BOOL CLU_GetScreenPubName( struct Screen *scr, STRPTR namebuffer );
/* v39 */
STRPTR CLU_SkipDot( STRPTR string );
void CLU_UpdateFilename( STRPTR filenamearray );
long CLU_CopyFileUpdateFilename( STRPTR fromname, STRPTR toname, STRPTR newnamearray );
long CLU_MoveFileUpdateFilename( STRPTR fromname, STRPTR toname, STRPTR newnamearray );
APTR CLU_FGets( BPTR, APTR, ULONG );
/* v40 */
void CLU_SprintfnA( STRPTR buffer, ULONG maxlen, STRPTR formatstring, APTR args );
void CLU_Sprintfn( STRPTR buffer, ULONG maxlen, STRPTR formatstring, ... );
/* v41 */
NOBJ *CLU_NOL_GetEntry( NOL *nol, ULONG index );
/* v42 */
int CLU_RunRexxScript( STRPTR filename, STRPTR defaultport );
void CLU_UnGetCAsync( struct AsyncFile *file );
/* V43 */
struct RDArgs * CLU_ReadArgsFromString( STRPTR string, STRPTR template, APTR args );
void CLU_FreeArgs( struct RDArgs *rda );
/* V45 */
void CLU_OpenLibs( struct DosLibrary **DOSBase, struct IntuitionBase **IntuitionBase, struct Library **UtilityBase );
void CLU_CloseLibsA( ULONG num, APTR *libbasearray );
void CLU_CloseLibs( ULONG num, ... );
struct Library * CLU_TimerBase( void );
void CLU_BeginTime( struct timeval *tv );
void CLU_EndTime( struct timeval *tv );
void CLU_AvgTime( struct timeval *tv, ULONG number );
STRPTR CLU_FmtTimeval( struct timeval *tv, STRPTR buffer );
/* V45.1 */
void CLU_Timecvt( time_t t, struct DateStamp *to );
time_t CLU_Datecvt( struct DateStamp *datestamp );
STRPTR CLU_Timestring( time_t t, STRPTR buffer );
/* V45.2 */
void CLU_DebugNamedSemaphores( void );
APTR CLU_ctype( void );
LONG CLU_Copy( APTR fromobj, APTR toobj, LONG size, ULONG flags );
/* V48 */
ULONG CLU_TestHLValA( HH hlist, STRPTR header, STRPTR *valarray );
ULONG CLU_TestHLVal( HH hlist, STRPTR header, ... );
ULONG CLU_DelHLNameValA( HH hlist, STRPTR header, STRPTR *valarray );
ULONG CLU_DelHLNameVal( HH hlist, STRPTR header, ... );
ULONG CLU_CountHLName( HH hlist, STRPTR header );
ULONG CLU_InsertRotHL( HL hl, STRPTR what );
void CLU_DeleteByPattern( STRPTR dirname, STRPTR pattern );
LONG CLU_DoBackupQueue( STRPTR sourcefile, STRPTR destfile, ULONG gennum );
LONG CLU_AddFileSizes( STRPTR path, STRPTR pattern );
LONG CLU_SplitDomainAdr( STRPTR domainadr, STRPTR adrbuf, STRPTR realnamebuf );
ULONG CLU_CalcCRC32File( STRPTR filename );
/* V49 */
time_t CLU_time( time_t *write );
LONG CLU_getft( STRPTR filename );
LONG CLU_GetHLDataLong( HL hl );
LONG CLU_GetHHCharset( HH hh );
STRPTR CLU_Get_iso2ibm( void );
STRPTR CLU_Get_ibm2iso( void );
void CLU_biso2ibm( APTR block, ULONG len );
struct Process *CLU_CreateProc( APTR entry, struct CLU_RegisterInfo *regs, struct TagItem *tags );
struct Process *CLU_CreateProcTags( APTR entry, struct CLU_RegisterInfo *regs, ... );
/* V51 */
HL CLU_AddHLSorted( HH, STRPTR header, STRPTR data, LONG insertafter );
/* V52 */
void CLU_utunpk( time_t t, char *destarray );
time_t CLU_utpack( char *array );
int CLU_strnicmp( char *s1, char *s2, int len );
int CLU_stricmp( char *s1, char *s2 );
void CLU_FmtDateMMDD( time_t t, char *deststring );
void CLU_bibm2iso( STRPTR block, LONG blocklen );
/* V54 */
ULONG CLU_CalcCRC16File( STRPTR filename );
/* V55 */
int CLU_FindInStringTab( STRPTR *strtab, STRPTR str );
/* V58 */
void CLU_DebugSemaphore( struct SignalSemaphore *sem );
/* V62 */
APTR CLU_GetKeyBase( void );
void CLU_ResetID( void );
#endif

extern struct Library *CLUtilBase;

#define CLUTIL_NAME "clutil.library"
#define CLUTIL_VER 62
#define CLU_VERSION 62


/*
 *
 * Codes fuer CLU_OpenLibraryCode()
 *
 */

#define CLULC_CLPREFS 0
#define CLULC_CLBOARD 1
#define CLULC_CLSYSTEM 2
#define CLULC_CLLOG 3
#define CLULC_CLMID 4                   /* clmidb.library; v10 */
#define CLULC_CLMACRO 5                 /* clmacro.library; v10 */
#define CLULC_CL_PREFS 6                /* 5.x cl_prefs.library */
#define CLULC_CL_LOG 7                  /* 5.x cl_log.library */
#define CLULC_CL_MAIL 8                 /* 5.x cl_mail.library */
#define CLULC_CL_MESSAGE 9              /* OBSOLETE: 5.x cl_message.library */
#define CLULC_CL_MAIL_JANUS 10  		/* 5.x cl_mail.library */
#define CLULC_CL_MAIL_Z38 11    		/* 5.x cl_mail.library */
#define CLULC_CL_MAIL_RFC 12   			/* 5.x cl_mail.library */
#define CLULC_CL_ROUTE 13               /* 5.x cl_route.library */
#define CLULC_CL_BOARD 14               /* 5.x cl_board.library */
#define CLULC_CL_LINK 15                /* 5.x cl_link.library */
#define CLULC_CL_ANSI 16                /* 5.x cl_ansi.library */
#define CLULC_MUIMASTER 17              /* muimaster.library */
#define CLULC_CL_MUI 18                 /* 5.x cl_mui.library */
#define CLULC_CL_USER 19                /* 5.x cl_user.library */
#define CLULC_CL_PORT 20                /* 4.x cl-port.library */
#define CLULC_CL_MM 21                  /* 5.x cl_mailmaster.library */
#define CLULC_CL_XFER 22                /* 5.x cl_xfer.library */
#define CLULC_CL_TEXT 23				/* 5.x cl_text.library */
#define CLULC_CL_MSG 24					/* 5.x cl_msg.library */
#define CLULC_CL_MPROC 25				/* 5.x cl_mproc.library */
#define CLULC_CL_ACCOUNTS 26			/* 5.x cl_accounts.library */
#define CLULC_CL_IPC 27					/* 5.x cl_ipc.library */
#define CLULC_CL_SHELL 28				/* 5.x cl_shell.library */
#define CLULC_CLONLINE 29				/* 5.x cl_online.library */

/*
 *
 * Help me ;-)
 *
 */

#define rstrchr strrchr
//define sprintf CLU_Sprintf
//extern void __stdargs __clsprintf( STRPTR to, STRPTR fmt, ... );
//define sprintf __clsprintf

#define asprintf CLU_ASprintf
#define clnl CLU_clnl
#define CreatePort CLU_CreatePort
#define DeletePort CLU_DeletePort

/*
 *
 * asyncio 
 *
 */

#define ASMODE_READ   0  /* read an existing file                             */
#define ASMODE_WRITE  1  /* create a new file, delete existing file if needed */
#define ASMODE_APPEND 2  /* append to end of existing file, or create new     */

/* obsolete; use standard OFFSET_xxx instead */

#define ASMODE_START   -1   /* relative to start of file         */
#define ASMODE_CURRENT  0   /* relative to current file position */
#define ASMODE_END      1   /* relative to end of file         */

typedef struct AsyncFile {
 APTR dummy;
} AsyncFile;

/*
 *
 * Date-Info
 *
 */

typedef struct ZCDateInfo
{
        time_t  t;              /* GMT */
        UBYTE   zoneid; /* immer 0 */
        UBYTE   flags;
        SHORT   offset; /* relativ zur GMT in Minuten */
} ZCDateInfo;

#define ZCDIF_DST 1     /* Daylightsavingtime (Sommerzeit) */


/*
 *    Named Object Lists (NOL)
 */

/*
 *   CLU_NOL_Info()
 */

typedef struct NOL_Info {
        ULONG nol_info_version; /* set to NOL_INFO_VERSION */
        ULONG nol_entrysize;    /* per entry size */
        ULONG nol_num_entries;  /* number of entries */
} NOL_Info;
#define NOL_INFO_VERSION 1

/*
 *  InsertModes and flags for CLU_NOL_AddEntry (V35)
 */

#define NOLIM_ADDHEAD 0
#define NOLIM_ADDTAIL 1
#define NOLIM_INSERT_BEFORE 2
#define NOLIM_INSERT_AFTER 3
#define NOLIM_INSERT_SORTED 4

#define NOLIMF_OVERWRITE (1<<16)        /* OR this into one of the InsertModes */

/*
 *   Tags for CLU_NOL_New() 
 */

#define NOLT_IGNORE     (TAG_USER+1)
#define NOLT_ENTRYSIZE (NOLT_IGNORE+1)  /* NOL entry size; default 0 */
#define NOLT_IGNORECASE (NOLT_IGNORE+2) /* Ignore name case; default FALSE */


/*
 *
 * CLU_MakeFileList() stuff 
 *
 */

#define CLUMFLF_NODIRS 1
#define CLUMFLF_NOFILES 2
#define CLUMFLF_NOINFOS 4
#define CLUMFLF_SORT 8

typedef struct clu_fib
{
        struct clu_fib *next;
        struct FileInfoBlock fib;
} clu_fib;

typedef struct clu_filelist
{
        struct clu_fib *entries;        /* linked list of entries found */
        ULONG numentries;                       /* number of entries found */
        ULONG flags;                            /* flags used for scanning */
        APTR sexandfreebeer;            /* PRIVATE, do not touch! */
} clu_filelist;


/*
 *
 * VMem support
 *
 */

#define MEMF_VMEM (1L<<24) /* private flag for pool functions only! */
#define MEMF_PROTECT (1L<<25) /* protect memory for this task only */


/*
 *
 *  MAKE_ID -Makro for MUI and cl_prefs.library
 *
 *
 */

#ifndef MAKE_ID
#define MAKE_ID(a,b,c,d) ((ULONG) (a)<<24 | (ULONG) (b)<<16 | (ULONG) (c)<<8 | (ULONG) (d))
#endif

/*
 *
 *	Flags for CLU_Copy()
 *
 */

#define CLUCOPY_SOURCE_NAME 	(1L<<0)	/* Source is Name instead of Filehandle */
#define CLUCOPY_SOURCE_ASYNC 	(1L<<1)	/* Source really is AsyncFile */
#define CLUCOPY_DEST_NAME 		(1L<<2)	/* Dest is Name instead of Filehandle */
#define CLUCOPY_DEST_ASYNC 		(1L<<3)	/* Dest really is AsyncFile */
#define CLUCOPY_DEST_APPEND		(1L<<4)	/* Append to the end of dest */

#define CLUCOPY_ERR_OPENIN		(-1)
#define CLUCOPY_ERR_OPENOUT		(-2)
#define CLUCOPY_ERR_WRITE		(-3)

/*
 *
 *  Register setup structure for CLU_CreateProc()
 *
 */

struct CLU_RegisterInfo {
	ULONG d0, d1, d2, d3, d4, d5, d6, d7;
	ULONG a0, a1, a4, a5, a6;
};

#endif
