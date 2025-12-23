/* Clib/MsgLibBase_protos.h */

#ifndef CLIB_MSGLIBBASE_H
#define CLIB_MSGLIBBASE_H

struct MsgBaseLink * mbInitialise ( struct MsgGlobals * ) ;
int mbGetInfo ( struct MsgInfo * ) ;
BOOL mbInitBase ( struct msgarea * ) ;
void mbQuit ( void ) ;

struct WholeMessage * mbLoadMessage ( ULONG, ULONG, ULONG ) ;
signed int mbSaveMessage ( struct WriteMsgStruct * ) ;
struct MsgScanKey * mbInitMsgScanKey ( struct ScanInfo * ) ;
ULONG mbScanMessages ( struct MsgScanKey *, struct WholeMessage * ) ;
void mbCloseMsgScanKey ( struct MsgScanKey * ) ;
signed int mbEditMessage ( struct Msg *, unsigned char * ) ;
signed int mbDeleteMessage ( ULONG, ULONG ) ;
ULONG mbGetNumberOfMessages ( void ) ;
struct MsgIdx * mbGetPointers ( ULONG ) ;
int mbSetPointers ( struct MsgIdx * ) ;

void mbInitialiseArea ( ULONG ) ;

long mbRescan( long );
long mbMaintenance( ULONG );
long mbUpdate( ULONG );
long mbCreateWaitingMail( ULONG );
long mbTrim( ULONG );

#endif

