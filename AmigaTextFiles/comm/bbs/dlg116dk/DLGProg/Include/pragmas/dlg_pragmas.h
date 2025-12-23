/* Cron routines*/
#pragma libcall DLGBase CronEvent 1e 81003
#pragma libcall DLGBase WhenEvent 24 801
/**/
/* Handler routines*/
#pragma libcall DLGBase TSetFlags 2a 8002
#pragma libcall DLGBase TUnSetFlags 30 8002
#pragma libcall DLGBase TBaud 36 8002
#pragma libcall DLGBase TColors 3c 9802
#pragma libcall DLGBase TCont 42 801
#pragma libcall DLGBase TDevQuery 48 9802
#pragma libcall DLGBase TFreeze 4e 801
#pragma libcall DLGBase TGetSer 54 9802
#pragma libcall DLGBase TGetTitle 5a 9802
#pragma libcall DLGBase TKill 60 801
#pragma libcall DLGBase TRecover 66 801
#pragma libcall DLGBase TScreen 6c 98003
#pragma libcall DLGBase TString 72 9802
#pragma libcall DLGBase TTimeDelay 78 8002
#pragma libcall DLGBase TTitle 7e 9802
#pragma libcall DLGBase TWindow 84 98003
#pragma libcall DLGBase TWinHeight 8a 9802
#pragma libcall DLGBase TInTrans 90 9802
#pragma libcall DLGBase TOutTrans 96 9802
#pragma libcall DLGBase TCheckCarrier 9c 801
#pragma libcall DLGBase TSendBreak a2 801
/**/
/* String comparison routines*/
#pragma libcall DLGBase Stricmp a8 9802
#pragma libcall DLGBase Strnicmp ae 09803
/**/
/* String formatting routines*/
#pragma libcall DLGBase Upper b4 801
#pragma libcall DLGBase Capitalize ba 801
#pragma libcall DLGBase UnderScore c0 801
#pragma libcall DLGBase DeScore c6 801
/**/
/* Formatting routines*/
#pragma libcall DLGBase AFormat cc B0A9805
#pragma libcall DLGBase XASPrintf d2 BA9804
#pragma libcall DLGBase XAFPrintf d8 BA9804
/**/
/* Time routines*/
#pragma libcall DLGBase MDate de 801
#pragma libcall DLGBase SMDate e4 8002
#pragma libcall DLGBase AmigaTime ea 0
/**/
/* Resource management routines*/
#pragma libcall DLGBase ResourceMsg f0 801
#pragma libcall DLGBase ActivatePort f6 9802
#pragma libcall DLGBase DeActivatePort fc 9802
#pragma libcall DLGBase LockPort 102 B0A9805
#pragma libcall DLGBase ImmedLockPort 108 B0A9805
#pragma libcall DLGBase TransferPortLock 10e 10BA9806
#pragma libcall DLGBase FreePort 114 9802
#pragma libcall DLGBase GetPortInfo 11a 801
#pragma libcall DLGBase FreePortInfo 120 801
#pragma libcall DLGBase LockArea 126 2198005
#pragma libcall DLGBase BorrowArea 12c 2198005
#pragma libcall DLGBase FreeArea 132 18003
#pragma libcall DLGBase EnterArea 138 1002
#pragma libcall DLGBase LeaveArea 13e 1002
#pragma libcall DLGBase GetAreaInfo 144 0802
#pragma libcall DLGBase FreeAreaInfo 14a 801
#pragma libcall DLGBase ListPorts 150 9802
#pragma libcall DLGBase LoadLang 156 9802
#pragma libcall DLGBase GetLang 15c 801
#pragma libcall DLGBase LockResource 162 10A9805
#pragma libcall DLGBase FreeResource 168 9802
#pragma libcall DLGBase GetResReport 16e 0
#pragma libcall DLGBase FreeResReport 174 801
#pragma libcall DLGBase LockMenu 17a 21BA09807
#pragma libcall DLGBase FreeMenu 180 A9803
#pragma libcall DLGBase PurgeMenu 186 9802
/**/
/* Structured File I/O routines*/
#pragma libcall DLGBase AddStruct 18c 109804
#pragma libcall DLGBase DeleteStruct 192 109804
#pragma libcall DLGBase GetStruct 198 109804
#pragma libcall DLGBase GetFirstStruct 19e 09803
#pragma libcall DLGBase FileSize 1a4 9802
#pragma libcall DLGBase Exists 1aa 801
#pragma libcall DLGBase AppendFile 1b0 9802
#pragma libcall DLGBase BinPos 1b6 A2190806
#pragma libcall DLGBase FileCopy 1bc 2109805
/**/
/* Broadcast routines*/
#pragma libcall DLGBase BroadCast 1c2 09803
#pragma libcall DLGBase BCPend 1c8 801
#pragma libcall DLGBase BCResume 1ce 801
#pragma libcall DLGBase BCGet 1d4 9802
#pragma libcall DLGBase HandleBCMsgs 1da 801
#pragma libcall DLGBase BCMsg 1e0 109804
#pragma libcall DLGBase Inform 1e6 0A9804
#pragma libcall DLGBase WaitingMail 1ec 21B0A9807
/**/
/* User access routines*/
#pragma libcall DLGBase GetDevName 1f2 801
#pragma libcall DLGBase ReadRam 1f8 9802
#pragma libcall DLGBase WriteRam 1fe 9802
#pragma libcall DLGBase ReadUser 204 A9803
#pragma libcall DLGBase WriteUser 20a 9802
#pragma libcall DLGBase GetComputerType 210 8002
#pragma libcall DLGBase Age 216 21003
#pragma libcall DLGBase CheckUser 21c 801
/**/
/* Logging routines*/
#pragma libcall DLGBase WriteLog 222 A98004
#pragma libcall DLGBase WriteEvent 228 9802
/**/
/* Routines for accessing groups*/
#pragma libcall DLGBase OpenGroup 22e 801
#pragma libcall DLGBase NextInGroup 234 9802
#pragma libcall DLGBase CloseGroup 23a 801
/**/
/* Translation routines*/
#pragma libcall DLGBase TranslateBuffer 240 1BA09806
#pragma libcall DLGBase Substitute 246 0BA9805
#pragma libcall DLGBase ScreenMsg 24c 109804
#pragma libcall DLGBase ScreenBuffer 252 A09804
/**/
/* Search routines*/
#pragma libcall DLGBase SearchStart 258 9802
#pragma libcall DLGBase SearchNext 25e 801
#pragma libcall DLGBase SearchEnd 264 801
#pragma libcall DLGBase DLGPatternMatch 26a 9802
#pragma libcall DLGBase DLGSearch 270 2109805
#pragma libcall DLGBase DLGBinSearch 276 2109805
/**/
/* Argument parsing routines*/
#pragma libcall DLGBase ArgParse 27c 09803
#pragma libcall DLGBase StripPath 282 801
/**/
/* Input Routines*/
#pragma libcall DLGBase DLGQuery 288 9802
#pragma libcall DLGBase GetChar 28e 0
#pragma libcall DLGBase ReadChar 294 001
#pragma libcall DLGBase PutChar 29a 8002
#pragma libcall DLGBase IntQuery 2a0 9210805
#pragma libcall DLGBase BoolQuery 2a6 90803
#pragma libcall DLGBase More 2ac 10803
#pragma libcall DLGBase Pause 2b2 0
/**/
/* Display Routines*/
#pragma libcall DLGBase DispBuffer 2b8 3B2A198008
#pragma libcall DLGBase DispMsg 2be 9802
#pragma libcall DLGBase DispForm 2c4 0BA9805
#pragma libcall DLGBase Clr 2ca 001
#pragma libcall DLGBase PrintSpace 2d0 10803
/**/
/* Message access routines*/
#pragma libcall DLGBase ReadArea 2d6 18003
#pragma libcall DLGBase SendPrivateMsg 2dc BA09805
#pragma libcall DLGBase SendPublicMsg 2e2 BA9804
#pragma libcall DLGBase ImportPublicMsg 2e8 BA9804
#pragma libcall DLGBase SendRawMsg 2ee A9803
#pragma libcall DLGBase KillMsg 2f4 981004
#pragma libcall DLGBase SendBulletin 2fa A9803
#pragma libcall DLGBase GetHiLowPointers 300 BA98005
#pragma libcall DLGBase PutHiLowPointers 306 9218005
/**/
/* DLG File tranfer protocol routines*/
#pragma libcall DLGBase DLGGetSer 30c 0802
#pragma libcall DLGBase DLGReleaseSer 312 801
#pragma libcall DLGBase DLGProtoStatus 318 910804
/**/
/* Debugging Routine*/
#pragma libcall DLGBase DB 31e 801
/**/
/* More Routines*/
#pragma libcall DLGBase UnpackTime 324 8002
/**/
/* File access Routines*/
#pragma libcall DLGBase GetHiLowFPointers 32a BA98005
#pragma libcall DLGBase PutHiLowFPointers 330 9218005
#pragma libcall DLGBase GetPath 336 A90804
#pragma libcall DLGBase ScreenPath 33c 801
#pragma libcall DLGBase ReceiveFile 342 10BA9806
#pragma libcall DLGBase SendFile 348 10BA9806
/**/
/* Additional New Routines*/
#pragma libcall DLGBase GetComment 34e 9802
#pragma libcall DLGBase GetFileDate 354 9802
#pragma libcall DLGBase DelDir 35a 9802
#pragma libcall DLGBase DirSize 360 801
#pragma libcall DLGBase CD 366 801
#pragma libcall DLGBase Cat 36c A9803
#pragma libcall DLGBase Copy 372 9802
#pragma libcall DLGBase SmartRename 378 9802
#pragma libcall DLGBase SendCtlMsg 37e 81003
#pragma libcall DLGBase ClearLine 384 0
#pragma libcall DLGBase Draw_Line 38a 001
#pragma libcall DLGBase SDraw_Line 390 0802
#pragma libcall DLGBase GetLevel 396 801
#pragma libcall DLGBase AddArea 39c 0802
#pragma libcall DLGBase DelArea 3a2 0802
#pragma libcall DLGBase ExistsGlobalArea 3a8 0802
#pragma libcall DLGBase StripSpaces 3ae 801
#pragma libcall DLGBase LogOut 3b4 BA9804
#pragma libcall DLGBase TimeUntilShutdown 3ba 801
#pragma libcall DLGBase SuspendTime 3c0 9802
#pragma libcall DLGBase ResumeTime 3c6 9802
#pragma libcall DLGBase GetOrigin 3cc 0BA9805
#pragma libcall DLGBase CallEditor 3d2 21B0A9807
#pragma libcall DLGBase ListAreas 3d8 109804
#pragma libcall DLGBase ListSIGS 3de 10803
/**/
/* Execute Routines*/
#pragma libcall DLGBase ChainProgram 3e4 9802
#pragma libcall DLGBase OverlayProgram 3ea 801
#pragma libcall DLGBase DialogBatch 3f0 BA9804
