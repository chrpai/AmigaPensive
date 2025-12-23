extern struct Library *CLMailFidoBase;

/* private Funktionen!*/
#pragma libcall CLMailFidoBase CLMAIL_FIDO_RereadPrefs c6 0
#pragma libcall CLMailFidoBase CLMAIL_FIDO_Adr2Str cc 9802
#pragma libcall CLMailFidoBase CLMAIL_FIDO_Str2Adr d2 9802
#pragma libcall CLMailFidoBase CLMAIL_FIDO_FindAdr d8 801
#pragma libcall CLMailFidoBase CLMAIL_FIDO_GetBoxAdr de 09803
#pragma libcall CLMailFidoBase CLMAIL_FIDO_FindNet e4 9802
#pragma libcall CLMailFidoBase CLMAIL_FIDO_FindNetByLocalAdr ea 9802

void CLMAIL_FIDO_RereadPrefs( void );
void CLMAIL_FIDO_Adr2Str( fidoadr*, STRPTR );
void CLMAIL_FIDO_Str2Adr( STRPTR, fidoadr* );
struct Box * CLMAIL_FIDO_FindAdr( fidoadr * );
LONG CLMAIL_FIDO_FindNet( STRPTR boardname, struct fidonet *net );
LONG CLMAIL_FIDO_FindNetByLocalAdr( struct fidoadr *localadr, struct fidonet *net );
