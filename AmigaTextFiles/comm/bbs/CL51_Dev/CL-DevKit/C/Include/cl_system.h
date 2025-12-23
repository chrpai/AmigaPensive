/* cls v8.0 */

#ifndef _NO_PRAG
extern struct Library *CLSBase;
#pragma libcall CLSBase CLS_LoadList 1e 0
#pragma libcall CLSBase CLS_SaveList 24 0
#pragma libcall CLSBase CLS_GetEntry 2a 801
#pragma libcall CLSBase CLS_FindEntry 30 801
#pragma libcall CLSBase CLS_DeleteEntry 36 801
#pragma libcall CLSBase CLS_AddEntry 3c 801
/* semaphore locking*/
#pragma libcall CLSBase CLS_LockList 42 001
#pragma libcall CLSBase CLS_UnlockList 48 0
#pragma libcall CLSBase CLS_LastEntry 4e 801
/* "tag" support*/
#pragma libcall CLSBase CLS_AddTag 54 109804
#pragma libcall CLSBase CLS_GetTag 5a 0802
#pragma libcall CLSBase CLS_DelTag 60 0802
/* master rfc/zc rout parser (OBSOLETE!, use cl_route.library instead)*/
#pragma libcall CLSBase CLS_DoRouting 66 BA9804
/* multiple domain support*/
#pragma libcall CLSBase CLS_FindBox 6c 801
#pragma libcall CLSBase CLS_TestBoxDomain 72 9802
/* OBSOLETE, use cl_route.library instead!*/
#pragma libcall CLSBase CLS_IsThisMe 78 801
#pragma libcall CLSBase CLS_Private1 7e 0
/* new for v3*/
#pragma libcall CLSBase CLS_GetTagSize 84 801
/* new for v4*/
#pragma libcall CLSBase CLS_GetNumEntries 8a 0
/* new for V8*/
#pragma libcall CLSBase CLS_IsModified 90 0
#pragma libcall CLSBase CLS_GetBoxNumber 96 90803
/* new for V9 */
#pragma libcall CLSBase CLS_GetSystemNetData 9c 801
#pragma libcall CLSBase CLS_ResetSystemNetData a2 801
/* new for V10 */
#pragma libcall CLSBase CLS_FindPoint a8 801
#endif

#ifndef _NO_PROTO
int CLS_LoadList(void);
int CLS_SaveList(void);
struct Box *CLS_GetEntry(struct Box*);
struct Box *CLS_LastEntry(struct Box*);
void CLS_DeleteEntry(struct Box*);
void CLS_AddEntry(struct Box*);
void CLS_LockList(int);
void CLS_UnlockList(void);
long CLS_AddTag(struct Box*,void*,short,short);
void *CLS_GetTag(struct Box*,short);
long CLS_DelTag(struct Box*,short);
struct Box *CLS_FindBox(char*);
long CLS_TestBoxDomain(struct Box*,char*);
long CLS_GetTagSize(void*);
long CLS_GetNumEntries(void);
long CLS_IsModified( void );
void CLS_GetBoxNumber( struct Box *box, ULONG index, STRPTR phonebuffer );
APTR CLS_GetSystemNetData( struct Box *box );
void CLS_ResetSystemNetData( struct Box *box );
struct Box * CLS_FindPoint( STRPTR username );
#endif

#define CLS_NAME "clsystem.library"
#define CLS_VERSION 10

struct Box
{
   char boxname[41];			// Name
   char boxtext[41];			// Domain
   char outserver[41];			// Routesystem
   char netzname[29];           // gekürzt!

   ULONG valid_ports;           // gueltige Ports
   long rout_grundkosten;       // Wird bei Routmail an dieses System
                                // einmal berechnet
   long eil_grundkosten;        // Wird bei Eilmail an dieses System
                                // einmal berechnt
   UBYTE modeminit;				// Welches Modem-Init
   char reserved[10];    // Not in USE
   char xprprotosend[11];		
   char packerin[41];
   char packerout[41];
   char groups[16];
   char pointuser[41];
   char telefon[61];
   char boxinit[41];
   long priority;
   char password[41];
   long flags;
   long lastpoll;
   long eilkosten;              // gab es schon vorher, jetzt pro KB bei Mail
   long batchsize;
   long routkosten;             // wie eilkosten
   long maxpm;
   long ignoreme;				// OBSOLETE!
};

#define BOX_NOEIL 1
//#define BOX_NOBLITZ 2 // Gibbet nicht mehr
#define BOX_NOROUT 4
#define BOX_MAPS 8
#define BOX_AUTOEINTRAG 16
//#define BOX_TELEFON 32        // Gibbet nicht mehr
//#define BOX_HIDDEN 64 // Gibbet nicht mehr
#define BOX_BACKUP 128
//#define BOX_PACKDELAY 256   // Crosspoint-Flag
#define BOX_EXTPROTO 512              
//#define BOX_REMAP 1024  // Gibbet nicht mehr
//#define BOX_POSTPACK 4096  // Gibbet nicht mehr
//#define BOX_DOMAINROUTING 8192 // Gibbet nicht mehr
