
/*
 * FAME Door Proto
 * ~~~~~~~~~~~~~~~
 *
 * $VER: FAMEDoorProto.h v1.5
 *
 * Code by: David 'Strider/tRSi' Wettig
 * E-Mail:  strider@trsi.de
 * URL:     http://www.trsi.de/inno/strider/index.html
 *
 * Copyright note: FAME and all it's contents like programs,
 *                 tools, doors, documents, texts, sources,
 *                 standards like the FIM (FAME door port standard),
 *                 the FAMEToolPort and all depending things,
 *                 the FAMECoProc standard and (data) file formats
 *                 are copyrighted © 1993 - 1997 by David Wettig.
 *
 *                 The include file you are currently reading
 *                 is also copyrighted and you are only allowed
 *                 to use it to develop own programs for the
 *                 FAME BBS System and you are *NOT* allowed
 *                 to use it for own projects like Mailbox
 *                 programs or whatever without my permission.
 */

#ifndef FAME_FAMEDOORPROTO_H
#define FAME_FAMEDOORPROTO_H

/*
 * Includes:
 */

#include <FAME/FAMEPublic.h>
#include <proto/dos.h>
#include <proto/exec.h>
#include <exec/types.h>
#include <exec/ports.h>
#include <exec/memory.h>
#include <dos/dos.h>
#include <clib/exec_protos.h>
#include <clib/dos_protos.h>
#include <clib/alib_protos.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 
 * Prototypes:
 */

extern long FIMStart( long BufSize, ULONG FIMFlags );
extern long FIMHandlePort( long FIMCommand, ULONG UserSigs, short int NoErrCheck );
extern void FIMEnd( long ReturnCode );

extern long GetString( char*, long );
extern long PutString( char*, long );
extern long NC_PutString( char*, long );
extern long PutStringFormat(char *ctl, ... );
extern long NC_PutStringFormat( char *ctl, ... );
extern long PutCommand( char*, long, long, ULONG, long );
extern long GetCommand( char*, long, long, ULONG, long );

extern void Center( char *stri, long lf, long col );
extern void SPrintf( char *Buffer, char *ctl, ... );
extern long FAMEAtol( char *Buffer );

/*
 * This function has to be supplied by the door and will be called if any
 * error occures. It's up to the door to exit or continue the work, but
 * remember that the DoorPort and therefor the communication to FAME is
 * CLOSED ! The errcode contains the errorcode supplied by FAME, you may
 * use it to your own requirements.
 */

extern void ShutDown( long ErrCode );

/*
 * This function has to be supplied by the door if the define FIM_USERSIGMODE
 * is set and will be called if any user signal occures. User signals will be
 * set via FIMHandlePort() in the second argument "ULONG UserSigs".
 * Use NULL here if you don't have any signals to be supported.
 * UserSigHandle() will be called if any user signals are set and if 
 * FIMHandlePort() receives them. In this case check the argument
 * "UserSigs" in your UserSigHandle() function.
 *
 * You can also use the global variable "GlobUserSigs" to inform
 * FIMHandlePort() about your own additional signal bits.
 * In this case FIMHandlePort()'s second argument "ULONG UserSigs" has to
 * be always NULL, because this argument has the higher priority and
 * "GlobUserSigs" will not be used then.
 * "GlobUserSigs" is to handle your own signal bits more easy.
 * All support functions will pass NULL to FIMHandlePort() so usage of
 * "GlobUserSigs" is possible without doing any changes to the doorstartup
 * code.
 *
 * Note: If you want to use the user signals feature you have to set the flag
 *       FIMF_USERSIGMODE in the function FIMStart().
 */

#ifdef FIM_USERSIGMODE

extern void UserSigHandle( ULONG UserSigs );

#endif

/*
 * Global variables:
 */

extern struct DosBase     *DosBase;
extern struct MsgPort     *DoorControlPort,
                          *DoorReplyPort;
extern struct FAMEDoorMsg *MyFAMEDoorMsg;  /* The DoorMsgPort structure for your usage */
extern struct FAMELibrary *FAMEBase;

extern char               FAMEDoorPort[30],
                          LastWords[200];
extern unsigned long      Signals,
                          PortSig,
                          GlobFIMFlags;

#ifdef FIM_USERSIGMODE

extern unsigned long      GlobUserSigs;

#endif

#endif  /* FAME_FAMEDOORPROTO_H */

