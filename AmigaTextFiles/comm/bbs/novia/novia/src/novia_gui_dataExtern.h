//extern ULONG ScreenmodeToStr( struct Hook *, void *, APTR );
#include <libraries/gadtools.h>
#define MUIA_HelpFile MUIA_Application_HelpFile

#define REG(x) register __ ## x
#define menu_aboutmui		0x05

#define ASM
#define SAVEDS

extern ULONG ScreenmodeToStr();
extern LONG DisplayListcport(); 
extern LONG DisplayListcportmin();
extern LONG DisplayListudata();
extern LONG DisplayListPOP3Server();
extern LONG DisplayListLocalMailServer();
extern LONG DisplayListMailserverUpdate();
extern ULONG DisplayListMailserverUser( struct Hook *, void *, APTR );
extern ULONG DisplayListMailserverChange( struct Hook *, void *, APTR );
extern ULONG DisplayListPOP3Update( struct Hook *, void *, APTR );
extern ULONG DisplayListBase( struct Hook *, void *, APTR );
extern ULONG DisplayListPOP3Remove( struct Hook *, void *, APTR );
extern ULONG DisplayListBaseRemove( struct Hook *, void *, APTR );
extern ULONG DisplayListEvent( struct Hook *, void *, APTR );
extern ULONG DisplayListEventRemove( struct Hook *, void *, APTR );
extern ULONG DisplayListEventUpdate( struct Hook *, void *, APTR );

