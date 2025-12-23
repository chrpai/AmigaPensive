/*
**	cl_ipc.library
**
**  Connectline Inter Process Message Support Library
**
*/


#ifndef _NO_PRAG
extern struct Library *CLIPCBase;
#pragma libcall CLIPCBase CLIPC_Setup 1e 9802
#pragma libcall CLIPCBase CLIPC_SetInfoA 24 A9803
#pragma tagcall CLIPCBase CLIPC_SetInfo 24 A9803
#pragma libcall CLIPCBase CLIPC_AddAccLog 2a 09803
#pragma libcall CLIPCBase CLIPC_GetPortDefs 30 0
#pragma libcall CLIPCBase CLIPC_SendMsg 36 98321006
#pragma libcall CLIPCBase CLIPC_SendOnlineMessage 3c A9803
#pragma libcall CLIPCBase CLIPC_BBSChatMsg 42 801
#endif

#ifndef _NO_PROTO
void CLIPC_Setup( APTR, APTR );
void CLIPC_SetInfoA( STRPTR, STRPTR, APTR );
void CLIPC_SetInfo( STRPTR, STRPTR, ... );
void CLIPC_AddAccLog( STRPTR portid, STRPTR name, UBYTE type );
struct portdef * CLIPC_GetPortDefs( void );
int CLIPC_SendMsg( int port, int cmd, int p1, int p2, STRPTR u1, STRPTR u2 );
int CLIPC_SendOnlineMessage( STRPTR touser, STRPTR text, STRPTR fromuser );
void CLIPC_BBSChatMsg( STRPTR msg );
#endif

struct ServerMsg
{
	struct Message ExecMessage;
	UWORD pad;
	int Command;
	int Params1;
	int Params2;
	char User1[ 128 ];
	char User2[ 128 ];
};

#define SERVERCOMMAND_BASE (0x10000)
#define SERVERCOMMAND_REVIEW (SERVERCOMMAND_BASE+1)
