/*
 *    Connectline 4.x/5.0 Datenstrukturen
 *
 *    $VER: 4.3
 */


#ifndef _TIME_H
#include <time.h>
#endif

#include <exec/ports.h>


#define CL_DEFAULT 0

/* ZConnect-Flags */

#define ZCARC_ARC 1
#define ZCARC_ARJ 2
#define ZCARC_LHARC 4
#define ZCARC_LHA 8
#define ZCARC_ZOO 16
#define ZCARC_ZIP 32
#define ZCARC_ZIP2 64
#define ZCARC_COMPRESS 128
#define ZCARC_TAR_COMPRESS 256
#define ZCARC_NONE 0
#define ZCCRYPT_NONE 0
#define ZCCRYPT_DES 1
#define ZCCRYPT_PGP 2
#define ZCTRANS_V21 1
#define ZCTRANS_V22 2
#define ZCTRANS_V22BIS 4
#define ZCTRANS_V32 8
#define ZCTRANS_V32BIS 16
#define ZCTRANS_PEP 32
#define ZCTRANS_HST 64
#define ZCTRANS_V110 128
#define ZCTRANS_BUNDLE 256
#define ZCTRANS_X75SLP 512
#define ZCTRANS_HDLC 1024
#define ZCTRANS_BITTRANSP 2048
#define ZCTRANS_SNA_SDLC 4096
#define ZCTRANS_X75BTX 8192
#define ZCTRANS_X25 16384
#define ZCTRANS_MNP 32768
#define ZCTRANS_MNP5 (1<<17)
#define ZCTRANS_V42 (1<<18)
#define ZCTRANS_V42BIS (1<<19)
#define ZCTRANS_T70NL (1<<20)
#define ZCTRANS_ISO8208 (1<<21)
#define ZCTRANS_T90 (1<<22)
#define ZCTRANS_TRANSPARENT 0

/* User-Flags */

#define BEFEHL_LOGFILE 1
#define BEFEHL_MITSCHNITT 2
#define BEFEHL_UNSICHTBAR 4
#define BEFEHL_NURBATCH 8
#define BEFEHL_HIGHPRIO 16
#define BEFEHL_LOCKED 32

#define NETZ_NURANRUF 1
#define NETZ_NURDIAL 2

#define USER_INVISIBLE 1
#define USER_NETWORKENABLED 2
#define USER_FRAS 4
#define USER_ADRENABLED 64
#define USER_FORBIDSPANNERSYSOP 128
#define USER_FORBIDDIALOG 256
#define USER_NO_MESSAGE 512
#define USER_NO_CHAT 1024
#define USER_BACKWARDS 4096
#define USER_AUTOAUSTRAG 8192
#define USER_VERTRETER_UMLEITEN 16384
#define USER_DOMAINROUTING 32768

#define SCR_INTERLACE 1
#define SCR_DHOEHE 2


#define MSG_NR 1
#define MSG_REALADR 2   //ZC
#define MSG_BETREFF 4
#define MSG_ABSENDER 8
#define MSG_KOPIEEMPFAENGER 16 //ZC
#define MSG_ABSENDERDATUM 32
#define MSG_AUTOCLEAR 64
#define MSG_ROUTZEIT 128
#define MSG_ROUTWEG 256 //ZC
#define MSG_LAENGE 512
#define MSG_KOMMENTAR 1024 //EXT
#define MSG_GELESEN 2048
#define MSG_CLS 4096
#define MSG_ABFRAGE 8192
#define MSG_SKOMMENTAR 16384
#define MSG_MESSAGEID 32768
#define MSG_FILETYP 65536 //ZC
#define MSG_FILENAME 131072  //ZC
#define MSG_BEZUG 262144  //ZC
#define MSG_ORIGEMPF 524288 //ZC
#define MSG_ORIGABS 1048576 //ZC
#define MSG_GUELTIGAB 2097152 //ZC
#define MSG_MAILER 4194304 //ZC
#define MSG_ZART 8388608 //ZC


#define INH_NUMMER 1
#define INH_TYP 2
#define INH_KB 4
#define INH_KOSTEN 8
#define INH_DATUM 16
#define INH_ABSENDER 32
#define INH_BETREFF 64
#define INH_CLPMS 128
#define INH_BOLDNEW 256
#define INH_KURSMARK 512
#define INH_VRLESEN 1024
#define INH_AUTOCLS 2048
#define INH_KOMMENTAR 4096

#define BOX_NOEIL 1
//#define BOX_NOBLITZ 2 // Gibbet nicht mehr
#define BOX_NOROUT 4
#define BOX_MAPS 8
#define BOX_AUTOEINTRAG 16
//#define BOX_TELEFON 32        // Gibbet nicht mehr
//#define BOX_HIDDEN 64 // Gibbet nicht mehr
#define BOX_BACKUP 128
#define BOX_ZSWAP 256   // Crosspoint-Flag
#define BOX_EXTPROTO 512              
//#define BOX_REMAP 1024  // Gibbet nicht mehr
//#define BOX_POSTPACK 4096  // Gibbet nicht mehr
//#define BOX_DOMAINROUTING 8192 // Gibbet nicht mehr


#define DBK_MULTIUSER 1
#define DBK_LETOPEN 2
#define DBK_CLIPROG 4
#define DBK_READ 8
#define DBK_CALL 16
#define DBK_LOGIN 32
#define SER_DISABLE_CARRIER 1
#define SER_LOCKBAUD 2
#define ANRUFE 4
#define SER_TERMMODE 8
#define SER_XON 16

/* Mail-Flags */

#define MAIL_EIL 1
#define MAIL_BLITZ 2
#define MAIL_EMPFANGSBESTAETIGUNG 4
#define MAIL_READ_ONCE 8
#define MAIL_RUNDSCHREIBEN 16
#define MAIL_NO_RATIO 32
#define MAIL_SIGNUM 64

#define MT_TEXT 0
#define MT_BIN 1
#define MT_CLEARED 2
#define MT_NETMAIL 4
#define MT_ARCHIV 8
#define MT_BATCH 16
#define MT_ZENSUR 32
#define MT_UNZUSTELLBAR 64
 
/* Internal prefs of CLPrefs */

#define SYSOP_DA 1
#define AUTONETCALL 2

/* Boardflags */

#define FRASBRETT 1
#define BINARY_ONLY 2
#define AUTOAUSTRAG 4
#define MAPS_ACCESS 8
#define REALNAME 16
#define RATIO 32
#define RAMBO 64
#define TEXT_ONLY 128

/*-------------------------------------------------------------*/
/* Filename: PRGDATA:PREFS/PORTS */
/* Enthaelt IMMER die Daten fuer 8 Ports (Nr.9=Offline-Port)*/

struct PortInfo{
   char PortDeviceName[41];
   char Comment[41];          /*Sysopinfo*/
   char PortSysname[11]; /* Leer=Default-Systemname s.u. */
   char PortModemInit[64];
   char PortModemInit2[64];
   char PortModemInit3[64];
   char PortModemInit4[64];
   char PortModemWaehlen[41];
   char PortModemAuflegen[41]; /* Auflegebefehl ohne +++ ! */
   char PortModemConnect[41];
   char PortModemBusy[41];
   char PortFaxConnect[41];
   char PortFaxCmd[82];
   char PortModemNoCarrier[41];
   char PortModemTurnOnline[41]; /*=Besetzt schalten */   
   char OnlineUser[41];
   long PortBaud;
   long PortBuffer;
   long NumberOfCalls;
   short PortEscChar;
   short PortModemDelay;
   short PortUnitNumber;
   short PortSerialFlags;
   short NumberOfRings;
   short InternalSerflags;
   int WaitForConnect;
   int RedialDelay;
   long WaitQuiet; /* siehe flags */
   long supported_protos; /* siehe flags; wie transfer */
   char deftelefonnummer[76]; /* telefonnummer im zconnect-standard */
   UBYTE fax;			/* wenn 42, dann Fax vorhanden */
   UBYTE active_init;	/* Aktiver Modeminit */
   UBYTE reservedxxxx[ 2 ];

};

/* Filename: PRGDATA:PREFS/CL.prefs */

struct CLPrefs{
   char Systemname[10];
   char Logfile[256];
   char Mitschnitt[256];
   char Temp[256];
   char OfflineEditor[256];
   char Unzustellbar[41];
   char Userantraege[41];
   char Batch[41];
   char Hilfe[41];
   char Font[41];
   char FKey[10][81];
   char ExcludeBin[256];
   char ExcludeText[256];
   long SysPW; /* Trapdoor-Value */
   long LoginTimeout;
   long KeyTimeout;
   long NumberOfCallsGes;
   long NumberOfCallsTod;
   long Today;
   short Portanzahl;
   short InternalFlags;
   short HelpAtError;
   short LogoffAtError;
   short TickTime;      /* Minuten */
   short reserved[ 3 ];
   short NumberOfReadRetrys;
   short OldNumberOfBMessages;
   short OldNumberOfPMMessages;
   int MaxRoutBoard;
   int MaxRoutPM;
   int QuoteChar;
   long ScreenFlags;
   char StdBoardPath[256];
   char StdUserPath[256];
        char SysopName[ 40 ];
        LONG maxsmalltmp;
   char reserved2[256-44];
   char LUser[61];
   char LPoint[41];
   char LSystem[41];
   long LUserdate;
   long LPointdate;
   long LSystemdate;
   int LUserport;
   int LPointport;
   int LSystemport;
   int NR_Userantraege;
   int NR_Sysopmails;
   int Bitplanes;
   char zconn_koordinaten[40];
   char domain[40]; /* eigene domain */
   char zconn_postanschrift[128]; 
   char zconn_voicephone[80];

        ULONG   NumberOfBMessages;
        ULONG   NumberOfPMMessages;
};

/* Filename PRGDATA:PREFS/xprproto */

struct XprProto{
   char protoname[11];
   char protobetreff[41];
   char libname[41];
   char options[41];
   short effpercent;
   long params;
};

/* Filename PRGDATA:PREFS/keys */

struct Keypad{
   char name[11];
   char ae[5];
   char oe[5];
   char ue[5];
   char sz[5];
   char AE[5];
   char OE[5];
   char UE[5];
};

/* Filename: PRGDATA:PREFS/brettliste */

struct Brett
{
   char Brettname[41];
   char Pfad[81];
   char Betreff[41];
   char Netzboxen[256]; /*Netzboxen durch ein "!" getrennt */
   char Servername[41]; /* Name des Brettes in der Serverbox... */
   char Vertreter[41];
   char Zensor[41];    /* Wenn gesetzt, aktiv */
   char Priority;
   char Group[16];
   char PMBoard[41];
   long AccessKeyword; /* Wenn gesetzt, aktiv */
   long WriteKeyword;  /* ebenfalls */
   long LastUpdate;    /* Unix-Format */
   long Aufrufe;
   long MaxKB;         /* Wenn gesetzt, aktiv */
   long MaxDays;       /* "          "*/ 
   long MaxNet;                 /* maximale Routgroesse */
   short Boardflags;   
   short ZensurPrio;
   short Baudrate;     /*Mindestbaudrate*/
};


/*
 *
 *   CL-x.y-Brettstruktur
 *
 */

struct NewBrett
{
        char    Brettname[ 80 ];                                        // +
        char    Pfad[ 80 ];                                                     // *
        char    Zensor[ 32 ];                                           // -
        char    Vertreter[ 32 ];                                        // -
        char    Betreff[ 40 ];                                          // *
    UBYTE       Group[ 16 ];                                            // *
        ULONG   AccessKeyword, WriteKeyword;            // *
        ULONG   Aufrufe;                                                        // *
        time_t  LastUpdate;                                                     // *
        ULONG   MaxKB, MaxDays;                                         // *
        ULONG   MaxNet;                                                         // *
        ULONG   Baudrate;                                                       // +
        ULONG   Boardflags;                                                     // +
        UBYTE   Priority, ZensurPrio;                           // *

        UBYTE   _byte_reserved[ 2 ];                            // +
        ULONG   _longreserved[ 3 ];                                     // +
};

 

struct Terminal {
   char termname[41];
   char cls[41];
   char normal[41];
   char bold[41];
   char underline[41]; 
   char invers[41];
   char gotoxy[41];   /* %ld(x) %ld(y) */
   char colorx[41];   /* %ld(color) */
   char insertline[41];
   char deleteline[41];
   char insertchar[41];
   char deletechar[41];
   char Aup[41];
   char Adown[41];
   char Aleft[41];
   char Aright[41];
   char Aescchar;
};

struct Befehl{
   char hauptname[41];
   char type;
   char batchname[256];
   int number;
   int prio;
   int flags;
};



struct OldMail{
   char betreff[128];
   char absender[128];
   char routing[128]; /* Der Routstring ist bei lokalen Nachrichten der Kommentar ! */
   char kopieempfaenger[128];
   char messageid[31];
   char typ[4];
   char BodyBoard[41];
   char BodyID[31];
   char NextBoard[41];
   char NextID[31];
   int frasnumber;
   int kosten;
   int mailstart;
   int maillength;
   int commentstart;
   int commentlength;
   int numberofreads;
   short priority;
   long cleardate;
   long absdate;
   long incomdate;
   long flags;
};

struct Mail{
   char betreff[128];
   char absender[128];
   char kommentar[80]; 
   char messageid[128];
   unsigned char priority;
   unsigned short readcount;
   long typ;
   long mailstart,maillength;
   long commentstart,commentlength;
   long headerstart,headerlength;
   long cleardate,absdate,incomdate;  
   long kosten;
   long flags;
};

struct Netmail{
   char dest[128];
   char boxen[256];
   int portnr;
   struct Mail mail;
};

struct Oldbox
{
   char boxname[41];
   char boxtext[41];
   char outserver[41];
   char netzname[41];
   char xprprotoreceive[11];
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
   long eilkosten;
   long blitzkosten;
   long routkosten;
   long maxpm;
   long maxboard;
};

struct Netz
{
   char netzname[41];
   char netzdir[256];
   char outcall[41];
   char options[41];
   char extension[6];
   long flags;
};

struct Alias
{
   char input[81];
   char output[81];
   int priority;
};

struct Datenbank
{
   char name[41];
   char betreff[41];
   char dosaufruf[70];
   char parameter[70];
   char datei[70]; /* Parameter-Dateiname zur Übergabe der Parameter !*/
   char groups[16];
   int portnr;
   int baudrate;
   int password;
   int priority;
   long flags;
};   

struct DbkFile
{
   char username[41];
   char devicename[41];
   int unit;
   int priority;
   int konto;
   int portnr;
   APTR serread;
   APTR serwrite;
   APTR conread;
   APTR conwrite;
   APTR screen;
};

struct UserRexx{
   char befehl[80];
   char compfileto[80];
   int priority;
};

struct ClTimerReq{
   char name[41];
   char script[256];
   int day;
   int month;
   int hour;
   int minute;
   long flags;
};

struct Download{
   char pfad[256];
   char betreff[256];
   int msgnr;
};

struct headerline {
        struct headerline *next;
        char *header; /* Zeiger auf Header-String */
        char *data; /* Zeiger auf Parameter dazu */
};
