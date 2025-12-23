
#ifndef PROTOS_H
#define PROTOS_H

int		__asm	__saveds  mbInitBase 	( register __a0 struct msgarea * ) ;
BOOL
		__asm 	__saveds  mbInitialise 	( register __a0 struct MsgGlobals * ) ;
int		__asm	__saveds  mbGetInfo 		( register __a0 struct MsgInfo * ) ;
void		__asm	__saveds  mbQuit		( void ) ;

int				  FindPosition		( BPTR, ULONG, ULONG, ULONG ) ;
void				  MakeWaitingMail	( struct Msg * ) ;

struct WholeMessage * 	LIBFUNC mbLoadMessage 	( register __d0 int, register __d1 ULONG, register __d2 ULONG ) ;
signed int 		LIBFUNC mbSaveMessage 	( register __a0 struct WriteMsgStruct * ) ;
struct MsgScanKey * 	LIBFUNC mbInitMsgScanKey 	( register __a0 struct ScanInfo * ) ;
void			LIBFUNC mbCloseMsgScanKey 	( register __a0 struct MsgScanKey * ) ;
ULONG 			LIBFUNC mbScanMessages	( register __a0 struct MsgScanKey *, register __a1 struct WholeMessage * ) ;
signed int		LIBFUNC mbEditMessage 	( register __a0 struct Msg *, register __a1 unsigned char * ) ;
signed int		LIBFUNC mbDeleteMessage 	( register __d0 int, register __d1 ULONG ) ;
struct MsgIdx * 	LIBFUNC mbGetPointers 	( register __d0 ULONG ) ;
int			LIBFUNC mbSetPointers		( register __a0 struct MsgIdx * ) ;
ULONG			LIBFUNC mbGetNumberOfMessages ( void ) ;

#endif /* PROTOS_H */