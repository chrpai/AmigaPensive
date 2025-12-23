#ifndef _CL_CL_USER_H
#define _CL_CL_USER_H

/*
**      User & UserProfile definitions
*/

typedef struct User
{
   char Username[ 31 ];
   char Priority;
   char Pfad[ 244 ];
   UBYTE pw[ 12 ];
   long Passwordval; /* Trapdoor-Value */
   char Groups[ 16 ];
} User;

typedef struct UserProfile
{
   char   RealName[41];                         // Realname
   char   RealAdr[41];
   char   RealTel[41];
   char   RealInfo[41];                         // Sysop-Info
   char   reserved[37];                         // unbenutzt
   char   InfoMessage[41];                      // User-Info
   char   CopyTo[82];                           /* Vertreter */
   long   Schreibschutz;                        /* Trapdoor-Value */
   char   Terminal[11];                         // ANSI/VT100/TTY
   char   Language[11];                         // Deutsch
   char   Keyboard[11];                         // IBM/ISO
   time_t LastLogin;                            // Ab wann Nachrichten als NEU angezeigt werden (irreführend)
   short  NumberOfCalls;
   short  Timeout;                                      // Sekunden
   long   AccountTalers;                        /* DM * 100 (wg. Pfennig) */
   long   CreditTalers;                         /*  "" */
   short  ShowLines;                            // Zeilenzahl
   long   Flags;
   short  UserTarif;                            // unbenutzt
   char   StdProto[41];                         // Default-Protokoll
   long   InhaltFlags;
   long   LesenFlags;                           // unbenutzt
   int    MaxKB;                                        // für Crunch
   int    MaxDays;                                      // für Crunch
   time_t LastCall;                                     // letzter Login
   long   Uploads;                                      // kb
   long   Downloads;                            // kb
   long   FreeFiles;                            // Freie Files
   long   Rate;                                         // D/L-Ratio
   ULONG  FreeKByte;                            // Freie KBytes
   ULONG  reserved2[ 32 ];                      // let's waste some memory...
} UserProfile;

/*
**      Flags
*/

#define USER_INVISIBLE          1               // User unsichtbar
#define USER_NETWORKENABLED 2           // Netzberechtigung
#define USER_DONTSAVE           4               // Userdaten nach Logoff nicht speichern
#define USER_AUTO                       8               // Automatisch eingetragen
#define USER_AUTOREMOVE         16              // Automatisch entfernen

#endif
