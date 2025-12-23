#ifndef CLIB_FAME_PROTOS_H
#define CLIB_FAME_PROTOS_H TRUE

/*
**    C prototypes for FAME.library, for use with 32 bit integers only.
*/

#ifndef  LIBRARIES_FAME_H
#include <libraries/FAME.h>
#endif   /* LIBRARIES_FAME_H */

STRPTR  FAMEStrStr (STRPTR Source, STRPTR MatchString);
LONG    FAMEStackReport (VOID);
STRPTR  FAMEStrChr (STRPTR Source, UBYTE MatchChar);
LONG    FAMEFileCopy (BPTR SourceFH, BPTR DestFH, ULONG SrcSize, ULONG MaxMem);
LONG    FAMEFSearch (STRPTR SearchString, BPTR SearchFH);
ULONG   FAMEIsNumStr (STRPTR String);
STRPTR  FAMEStrChrCase (STRPTR Source, UBYTE MatchChar);
STRPTR  FAMEStrFil (STRPTR FillBuffer, UBYTE FillChar, UWORD NumberOfChars);
LONG    FAMEStrMid (STRPTR Source, STRPTR Destination, LONG StartPos, LONG NumberOfChars);
STRPTR  FAMEStrStrCase (STRPTR Source, STRPTR MatchString);
APTR    FAMEAllocPooled(ULONG ByteSize, ULONG MemAttrs, APTR FAMEMemPool);
APTR    FAMECreatePool (ULONG PoolSize, ULONG PuddleSize, ULONG MemAttrs, struct TagItem *);
VOID    FAMEDeletePool (APTR FAMEMemPool);
VOID    FAMEFreePooled (APTR Memory);
VOID    FAMEResetPool (APTR FAMEMemPool);
VOID    FAMEFillMem (APTR Buffer, UBYTE FillByte, LONG Size);
STRPTR  FAMEChrCut (STRPTR String, UBYTE CutChar, ULONG MaxSearchRange);
STRPTR  FAMEChrCutCase (STRPTR String, UBYTE CutChar, ULONG MaxSearchRange);
STRPTR  FAMEStrCut (STRPTR String, STRPTR CutString, ULONG MaxSearchRange);
STRPTR  FAMEStrCutCase (STRPTR String, STRPTR CutString, ULONG MaxSearchRange);
STRPTR  FAMEStrCopy (STRPTR Source, STRPTR Destination, UWORD MaxLen);
VOID    FAMEPrivate1 (VOID);
VOID    FAMEPrivate2 (VOID);
VOID    FAMEPrivate3 (VOID);
VOID    FAMEMemSet (APTR FillBuffer, UBYTE FillChar, UWORD NumberOfChars);
VOID    FAMEPrivate4 (VOID);
VOID    FAMEPrivate5 (VOID);
VOID    FAMEPrivate6 (VOID);
STRPTR  FAMESwapRedWhite (STRPTR String);
APTR    FAMEAllocObject (ULONG Type);
VOID    FAMEFreeObject (APTR Object);
LONG    FAMENumToStr (ULONG Value, ULONG Flags, ULONG BufSize, STRPTR Buffer);
ULONG   FAMEReverseLong (ULONG Value);
UWORD   FAMEReverseWord (UWORD Value);
VOID    FAMEAdd64 (ULONG SrcHi, ULONG SrcLo, APTR Destination);
VOID    FAMESub64 (ULONG SrcHi, ULONG SrcLo, APTR Destination);
LONG    FAMEFreeDiskSpace (STRPTR Name);
APTR    FAMEGetDevInfoList (VOID);
VOID    FAMEFreeDevInfoList (APTR);
VOID    FAMEStartECTimer (struct timeval *dest);
VOID    FAMEStopECTimer (struct timeval *dest);
APTR    FAMELoadFile (STRPTR Name, ULONG MemAttr, ULONG MaxSize, ULONG Flags);
VOID    FAMEFreeFile (APTR FAMEFile);
LONG    FAMEAvailExe (STRPTR Name);
ULONG   FAMEOverallBytes (APTR UlPathList, ULONG MinFreeMB);
ULONG   FAMEDosMove (STRPTR Src, STRPTR Dest, ULONG MaxBuf, ULONG Flags);
APTR    FAMEPostFile (STRPTR Name, APTR ULPathList, ULONG MinFreeMB, ULONG BufSize, ULONG Flags);
STRPTR  FAMEStrToUpper (STRPTR String);
STRPTR  FAMEStrToLower (STRPTR String);
APTR    FAMEExecuteDir (BPTR DirLock, struct TagItem *, STRPTR Args);
VOID    FAMEFreeExecuteDirList (APTR ExecuteDirList);
APTR    FAMELoadFilePooled (STRPTR Name, ULONG MemAttr, ULONG MaxSize, ULONG Flags, APTR FAMEMemPool);
LONG    FAMENum64ToStr (ULONG ValueHi, ULONG ValueLo, ULONG Flags, ULONG BufSize, STRPTR Buffer);
STRPTR  FAMECutANSI (STRPTR String, ULONG Flags);
long    FAMEAtol(char * Buffer);
void    FAMEPrivate7 (void);
void    FAMEPrivate8 (void);
long    FAMEPrivate9 (void);
long    FAMEPrivate10 (void);
long    FAMEPrivate11 (void);
long    FAMEPrivate12 (void);
long    FAMEStrCat (STRPTR Source, STRPTR Destination);
long    FAMEPrivate13 (void);
long    FAMEPrivate14 (void);
long    FAMEPrivate15 (void);
void    FAMEPrivate16 (void);
long    FAMEIsEqual64 (ULONG SrcHi, ULONG SrcLo, ULONG DestHi, ULONG DestLo);
long    FAMEIsHigher64 (ULONG SrcHi, ULONG SrcLo, ULONG DestHi, ULONG DestLo);
long    FAMEIsLower64 (ULONG SrcHi, ULONG SrcLo, ULONG DestHi, ULONG DestLo);
long    FAMEMulU64L (ULONG SrcHi, ULONG SrcLo, ULONG * DestHi, ULONG * DestLo);
long    FAMENum64Comp (ULONG SrcHi, ULONG SrcLo, ULONG DestHi, ULONG DestLo);
long    FAMEStrTo64 (STRPTR String, ULONG * DestHi, ULONG * DestLo);
ULONG   FAMEGetObjectSize (ULONG Type);
void    FAMESPrintf (STRPTR Buffer, STRPTR FormatString, ...);
void    FAMESPrintfA (STRPTR Buffer, STRPTR FormatString, long * Argv);
long    FAMEDivU64L (ULONG SrcHi, ULONG SrcLo, ULONG * DestHi, ULONG * DestLo);
APTR    FAMEObtainConfig (struct FAMEConfigRequest *FAMEConfigRequest, long *Result);
long    FAMEReleaseConfig (APTR ConfigData, long Modify, struct Task *CfgUserTask);
VOID    FAMERequestReset (struct FAMEConfigRequest *FAMEConfigRequest);
long    FAMEReloadConfig (APTR ConfigData, ULONG Flags);
long    FAMESaveConfig (APTR ConfigData, long Modify);
long    FAMEModifyConfig (APTR ConfigData, long Modify);
long    FAMESaveAllConfig (long SpecialCase);
long    FAMELockConfig (APTR ConfigData);
long    FAMEAttemptLockConfig (APTR ConfigData);
long    FAMEUnLockConfig (APTR ConfigData);
VOID    FAMEHandleConfigResults (STRPTR Buffer, long Result, ULONG ConfigType);
void    FAMEPrivate17(void);
void    FAMEPrivate18(void);
void    FAMEPrivate19(void);
void    FAMEPrivate20(void);
#endif /* CLIB_FAME_PROTOS_H */
