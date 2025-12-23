
#ifndef PRAGMAS_FAME_PRAGMAS_H
#define PRAGMAS_FAME_PRAGMAS_H TRUE

#ifndef CLIB_FAME_PROTOS_H
#include <clib/FAME_protos.h>
#endif

/* --- Version 6.0 --- **/
#pragma libcall FAMEBase FAMEStrStr 1E 9802
#pragma libcall FAMEBase FAMEStackReport 24 0
#pragma libcall FAMEBase FAMEStrChr 2A 0802
#pragma libcall FAMEBase FAMEFileCopy 30 321004
#pragma libcall FAMEBase FAMEFSearch 36 0802
#pragma libcall FAMEBase FAMEIsNumStr 3C 801
#pragma libcall FAMEBase FAMEStrChrCase 42 0802
#pragma libcall FAMEBase FAMEStrFil 48 10803
#pragma libcall FAMEBase FAMEStrMid 4E 019804
#pragma libcall FAMEBase FAMEStrStrCase 54 9802
#pragma libcall FAMEBase FAMEAllocPooled 5A 81003
#pragma libcall FAMEBase FAMECreatePool 60 321004
#pragma libcall FAMEBase FAMEDeletePool 66 901
#pragma libcall FAMEBase FAMEFreePooled 6C 901
#pragma libcall FAMEBase FAMEResetPool 72 901
#pragma libcall FAMEBase FAMEFillMem 78 10803
#pragma libcall FAMEBase FAMEChrCut 7E 10803
#pragma libcall FAMEBase FAMEChrCutCase 84 10803
#pragma libcall FAMEBase FAMEStrCut 8A 09803
#pragma libcall FAMEBase FAMEStrCutCase 90 09803
#pragma libcall FAMEBase FAMEStrCopy 96 09803
#pragma libcall FAMEBase FAMEPrivate1 9C 0
#pragma libcall FAMEBase FAMEPrivate2 A2 0
#pragma libcall FAMEBase FAMEPrivate3 A8 0
#pragma libcall FAMEBase FAMEMemSet AE 10803
#pragma libcall FAMEBase FAMEPrivate4 B4 0
#pragma libcall FAMEBase FAMEPrivate5 BA 0
#pragma libcall FAMEBase FAMEPrivate6 C0 0
#pragma libcall FAMEBase FAMESwapRedWhite C6 801
#pragma libcall FAMEBase FAMEAllocObject CC 001
#pragma libcall FAMEBase FAMEFreeObject D2 901
#pragma libcall FAMEBase FAMENumToStr D8 821004
#pragma libcall FAMEBase FAMEReverseLong  DE 001
#pragma libcall FAMEBase FAMEReverseWord  E4 101
#pragma libcall FAMEBase FAMEAdd64  EA 81003
#pragma libcall FAMEBase FAMESub64  F0 81003
#pragma libcall FAMEBase FAMEFreeDiskSpace  F6 801
#pragma libcall FAMEBase FAMEGetDevInfoList  FC 0
#pragma libcall FAMEBase FAMEFreeDevInfoList  102 901
#pragma libcall FAMEBase FAMEStartECTimer  108 801
#pragma libcall FAMEBase FAMEStopECTimer  10E 801
#pragma libcall FAMEBase FAMELoadFile  114 321004
#pragma libcall FAMEBase FAMEFreeFile  11A 901
#pragma libcall FAMEBase FAMEAvailExe  120 001
#pragma libcall FAMEBase FAMEOverallBytes  126 0802
#pragma libcall FAMEBase FAMEDosMove  12C 321004
#pragma libcall FAMEBase FAMEPostFile  132 3218005
#pragma libcall FAMEBase FAMEStrToUpper  138 801
#pragma libcall FAMEBase FAMEStrToLower  13E 801
#pragma libcall FAMEBase FAMEExecuteDir  144 18003
#pragma libcall FAMEBase FAMEFreeExecuteDirList  14A 901
#pragma libcall FAMEBase FAMELoadFilePooled  150 8321005
#pragma libcall FAMEBase FAMENum64ToStr 156 8321005
#pragma libcall FAMEBase FAMECutANSI 15C 0802
#pragma libcall FAMEBase FAMEAtol 162 801
#pragma libcall FAMEBase FAMEPrivate7 168 0
#pragma libcall FAMEBase FAMEPrivate8 16E 0
#pragma libcall FAMEBase FAMEPrivate9 174 0
#pragma libcall FAMEBase FAMEPrivate10 17A 0
#pragma libcall FAMEBase FAMEPrivate11 180 0
#pragma libcall FAMEBase FAMEPrivate12 186 0
#pragma libcall FAMEBase FAMEStrCat 18C 9802
#pragma libcall FAMEBase FAMEPrivate13 192 0
#pragma libcall FAMEBase FAMEPrivate14 198 0
#pragma libcall FAMEBase FAMEPrivate15 19E 0
#pragma libcall FAMEBase FAMEPrivate16 1A4 0
#pragma libcall FAMEBase FAMEIsEqual64 1AA 321004
#pragma libcall FAMEBase FAMEIsHigher64 1B0 321004
#pragma libcall FAMEBase FAMEIsLower64 1B6 321004
#pragma libcall FAMEBase FAMEMulU64L 1BC 981004
#pragma libcall FAMEBase FAMENum64Comp 1C2 321004
#pragma libcall FAMEBase FAMEStrTo64 1C8 A9803
#pragma libcall FAMEBase FAMEGetObjectSize 1CE 001
#pragma tagcall FAMEBase FAMESPrintf 1D4 A9803
#pragma libcall FAMEBase FAMESPrintfA 1D4 A9803
#pragma libcall FAMEBase FAMEDivU64L 1DA 981004
#pragma libcall FAMEBase FAMEObtainConfig 1E0 0802
#pragma libcall FAMEBase FAMEReleaseConfig 1E6 90803
#pragma libcall FAMEBase FAMERequestReset 1EC 801
#pragma libcall FAMEBase FAMEReloadConfig 1F2 0802
#pragma libcall FAMEBase FAMESaveConfig 1F8 0802
#pragma libcall FAMEBase FAMEModifyConfig 1FE 0802
#pragma libcall FAMEBase FAMESaveAllConfig 204 001
#pragma libcall FAMEBase FAMELockConfig 20A 801
#pragma libcall FAMEBase FAMEAttemptLockConfig 210 801
#pragma libcall FAMEBase FAMEUnLockConfig 216 801
#pragma libcall FAMEBase FAMEHandleConfigResults 21C 10803
#pragma libcall FAMEBase FAMEPrivate17 222 0
#pragma libcall FAMEBase FAMEPrivate18 228 0
#pragma libcall FAMEBase FAMEPrivate19 22E 0
#pragma libcall FAMEBase FAMEPrivate20 234 0

#endif  /*  PRAGMAS_FAME_PRAGMAS_H  */
