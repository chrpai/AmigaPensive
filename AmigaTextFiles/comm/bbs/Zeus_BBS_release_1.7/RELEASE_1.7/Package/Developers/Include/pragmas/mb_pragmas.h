#pragma libcall MsgLibBase mbInitialise 1e 801
#pragma libcall MsgLibBase mbGetInfo 24 801
#pragma libcall MsgLibBase mbInitBase 2a 801
#pragma libcall MsgLibBase mbLoadMessage 30 21003
#pragma libcall MsgLibBase mbSaveMessage 36 801
#pragma libcall MsgLibBase mbInitMsgScanKey 3c 801
#pragma libcall MsgLibBase mbCloseMsgScanKey 42 801
#pragma libcall MsgLibBase mbScanMessages 48 9802
#pragma libcall MsgLibBase mbEditMessage 4e 9802
#pragma libcall MsgLibBase mbDeleteMessage 54 1002
#pragma libcall MsgLibBase mbGetNumberOfMessages 5a 0
#pragma libcall MsgLibBase mbGetPointers 60 001
#pragma libcall MsgLibBase mbSetPointers 66 801
#pragma libcall MsgLibBase mbQuit 6c 0
/**/
/* New functions 01-Mar-95*/
/**/
#pragma libcall MsgLibBase mbUpdate 72 001
#pragma libcall MsgLibBase mbRescan 78 001
#pragma libcall MsgLibBase mbMaintenance 7e 001
#pragma libcall MsgLibBase mbTrim 84 001
/**/
/* 08-Mar-95*/
/**/
#pragma libcall MsgLibBase mbScanMsgHdrs 8a 9802
#pragma libcall MsgLibBase mbCustom1 90 801
#pragma libcall MsgLibBase mbCustom2 96 801
/**/
/* 02-Apr-95*/
/**/
#pragma libcall MsgLibBase mbInitialiseArea 9c 001
/**/
/* 08-Aug-95*/
/**/
#pragma libcall MsgLibBase mbCreateWaitingMail a2 001
