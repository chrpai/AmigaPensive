#pragma libcall CLLBase CLL_DelLog 24 0
#pragma libcall CLLBase CLL_RenameLog 2a 801
#pragma libcall CLLBase CLL_NewOpenLogWindow 42 9802
#pragma libcall CLLBase CLL_NewCloseLogWindow 48 801
#pragma libcall CLLBase CLL_GetIWin 4e 0
/* v6 temporary interface*/
#pragma libcall CLLBase CLL_AddLogA 54 A98004
#pragma tagcall CLLBase CLL_AddLog 54 A98004

extern struct Library *CLLBase;

void CLL_AddLogA( ULONG, STRPTR, STRPTR, APTR );
void CLL_AddLog( ULONG, STRPTR, STRPTR, ... );
void CLL_DelLog( void );
void CLL_RenameLog( char * );

struct MsgPort * CLL_NewOpenLogWindow( APTR, STRPTR );
void CLL_NewCloseLogWindow( APTR );

struct Window *CLL_GetIWin( void );
