
/*  Trion BBS 'Additional Clidoor Support' system header file           */
/*                                                                      */
/*  V 1.00   25 - 11 - 1995                                             */
/*  V 1.10    7 -  6 - 1997                                             */
/*                                                                      */
/*  (c) Copyright 1995-98 by Paul Spijkerman                            */



#include <exec/types.h>
#include <exec/ports.h>
#include <exec/memory.h>
#include <dos/dos.h>
#include <clib/exec_protos.h>
#include <clib/alib_protos.h>
#include <clib/dos_protos.h>
#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <stdarg.h>


#define CTRL(x)             (x - 'a' + 1)



struct ADoorMessage {
   struct Message    ADS_Msg;
   UBYTE             ADS_Command;
   UBYTE             ADS_SubCommand;

   BYTE              ADS_Result;
   UBYTE             ADS_Carrier;

   ULONG             ADS_Data_long;
   UWORD             ADS_Data_word;   

   void             *ADS_P;
   char              ADS_String[80];
};


/* In the new 'ADS' system there is a Result field for simplicity and  */
/* to avoid the confusion when the Result was in the Command field     */
/* If Result is smaller then zero the operation failed                 */
/*    -2   ..  Can't find BBS node (not stored in the result field)    */
/*    -1   ..  means the command doesn't exists                        */
/*     0   ..  Command exited succesfully                              */




struct ADS_Data  {                      /* ADS_DATA */
   struct ADoorMessage    *BBSMsg;
   struct MsgPort         *BBSreplyport;
   struct SharedData      *SharedData;      /* <- Pointer to Paths */
   int                    node;
   int                    quiet;
   int                    IDMode;
};



struct DoorIO {             /*   'Pure' Door :))   */
   BPTR              ConInput;
   BPTR              ConOutput;
   int               carrier;

   /* extra for color, position */

   int   mfc  , mbc;        /* Foreground, Background colour memory  */
   int   mx   , my;         /* Cursor pos. memory                    */
   int   xmax , ymax;       /* Screen size                           */
   int   mhigh;             /* Bold attribute, high intensity colors */

   /* UnGetCharacter data */

   UBYTE                  kar;
   UBYTE                  unused;   /* (padding or reserved for source) */
};





#define     ADSC_DROP_CARRIER                   ( 16)
#define     ADSC_STRING_TO_LOG                  ( 17)
#define     ADSC_TYPE_FILE                      ( 18)
#define     ADSC_GET_TIME_ONLINE                ( 19)
#define     ADSC_CHANGE_TIME_ONLINE             ( 20)

#define     ADSC_MARK_DOS_FILE                  ( 32)
#define     ADSC_UNMARK_ALL_DOSFILES            ( 33)
#define     ADSC_DOWNLOAD_DOS_FILES             ( 34)

#define     ADSC_UPLOAD                         ( 40)

#define     ADSC_PAGE_SYSOP                     ( 48)
#define     ADSC_PLAY_SAMPLE                    ( 49)
#define     ADSC_CHECK_SYSOP_AVAILABLE          ( 50)
#define     ADSC_SPLIT_CHAT_MODE                ( 51)

#define     ADSC_GET_USER_DATA                  ( 64)
#define     ADSC_CHANGE_USER_DATA               ( 65)
#define     ADSC_GET_SYSTEM_DATA                ( 66)


#define     ADSC_GET_MESSAGE_SECTION            ( 80)
/* ADS_Data_word = section  */
#define     ADSC_SET_MESSAGE_SECTION            ( 81)
/* ADS_Data_word = section,  ADS_SubCommand = 0-view, 2-read, 3-write */
#define     ADSC_CHECK_MESSAGE_SECTION          ( 82)
/* ? */


#define     ADSC_GET_FILE_SECTION               (100)
/* ADS_Data_word = section  */
#define     ADSC_SET_FILE_SECTION               (101)
/* ADS_Data_word = section,  ADS_SubCommand = 0-view, 1-list, 2-download, 3-upload */
#define     ADSC_CHECK_FILE_SECTION             (102)
/* ? */


#define     ADSC_FLUSH_BUFFER                   (126)
#define     ADSC_PUSHINTOBUFFER                 (127)

/* debug functions .. do not use !! */

#define     ADSC_PRINT                          (128)
#define     ADSC_GETCHAR                        (129)
#define     ADSC_MAYGETCHAR                     (130)
#define     ADSC_NO_OPERATION                   (131)    /* check carrier */




int   SetopPort      (struct ADS_Data *p, int node, int quiet);
int   RemovePort     (struct ADS_Data *p);


int   DropCarrier    (struct ADS_Data *p);
int   PrintToLog     (struct ADS_Data *p, char *string);
int   TypeFile       (struct ADS_Data *p, char *string);
int   GetTimeOnline  (struct ADS_Data *p);
int   IncreaseTimeOnline(struct ADS_Data *p, int time);
int   DecreaseTimeOnline(struct ADS_Data *p, int time);

int   Download       (struct ADS_Data *p);
int   MarkFile       (struct ADS_Data *p, char *str);
int   UnmarkAllFiles (struct ADS_Data *p);

int   Upload         (struct ADS_Data *p, char *str);

int   PageSysop      (struct ADS_Data *p);
int   PlaySample     (struct ADS_Data *p, char *sample);
int   PlaySampleNum  (struct ADS_Data *p, char *sample, int num);
int   CheckSysopAvailable(struct ADS_Data *p);
int   SplitChatMode  (struct ADS_Data *p, int mode);

int   GetUserStringField(struct ADS_Data *p, int field, char *string);
int   GetUserNumericalField(struct ADS_Data *p, int field, ULONG *var);
int   ChangeUserStringField(struct ADS_Data *p, int field, char *string);
int   ChangeUserNumericalField(struct ADS_Data *p, int field, ULONG var);

int   GetBaudRate    (struct ADS_Data *p, UWORD *local, ULONG *baudrate);
int   GetSharedData  (struct ADS_Data *p);

int   FlushBuffer    (struct ADS_Data *p);
/* int   PushIntoBuffer (struct ADS_Data *p, char *str); */
int   PushIntoBuffer(struct ADS_Data *p, char *str, int source);



void  SetOpIO        (struct DoorIO *p);
void  RemoveIO       (struct DoorIO *p);
UBYTE GetCharacter   (struct DoorIO *p);
UBYTE MayGetCharacter(struct DoorIO *p);
int   GetString      (struct DoorIO *p, char *str, int strsize);
int   Carrier        (struct DoorIO *p);
int   nprintf        (struct DoorIO *p, char *fmt, ...);
int   brk            (void);




/* voor en achtergrond kleuren  */
/* voor voorgrond kleuren wordt het BOLD atribute uitgezet ( esc[0m code ) */

#define BLACK     0x00     /* blue    in cli tot V1.3  */
#define RED       0x01     /* white   */
#define GREEN     0x02     /* black   */
#define YELLOW    0x03     /* orange  */
#define BLUE      0x04     /* blue    */
#define MAGENTA   0x05     /* white   */
#define CYAN      0x06     /* black   */
#define WHITE     0x07     /* orange  */

/* high intensity voorgrond kleuren                         */
/* hierbij wordt het BOLD atribute aangezet ( esc[1m code ) */

#define HBLACK    0x08     /* blue    in cli tot V1.3  */
#define HRED      0x09     /* white   */
#define HGREEN    0x0a     /* black   */
#define HYELLOW   0x0b     /* orange  */
#define HBLUE     0x0c     /* blue    */
#define HMAGENTA  0x0d     /* white   */
#define HCYAN     0x0e     /* black   */
#define HWHITE    0x0f     /* orange  */

#define NONE        0x00    /* geen atrribute aan  */
#define BOLD        0x01
#define FAINT       0x02
#define ITALIC      0x03
#define UNDERSCORE  0x04
#define BLINK       0x05
#define INVERS      0x07
#define SUBSCRIPT   0x30    /* decimaal 48 */
#define SUPERSCRIPT 0x31    /* decimaal 49 */
#define INVISIBLE   0x08


void cursxy  (struct DoorIO *p, int x,int y);
void col     (struct DoorIO *p, int fc,int bc);
void fcol    (struct DoorIO *p, int fc);
void bcol    (struct DoorIO *p, int bc);
void atr     (struct DoorIO *p, int atr);
void norm    (struct DoorIO *p);
void cls     (struct DoorIO *p);



/* User Data String Fields  */
/* size includes terminating '\0' character */

#define  UDSF_NAME             (64+ 0)      /* size=37  check for succes !! */
#define  UDSF_ALIAS            (64+ 1)      /* size=37  check for succes !! */
#define  UDSF_PASSWORD         (64+ 2)      /* size=17  */
#define  UDSF_PLAATS           (64+ 3)      /* size=40  */
#define  UDSF_VOICEPHONE       (64+ 4)      /* size=40  */
#define  UDSF_DATAPHONE        (64+ 5)      /* size=40  */

/* User Data Numerical Fields  */

#define  UDNF_NUMMER           (128+ 0)      /* LONG   Do not change field !! */
#define  UDNF_CALLS            (128+ 1)      /* ULONG  */
#define  UDNF_LEVEL            (128+ 2)      /* UBYTE  */
#define  UDNF_SCREENLENGTH     (128+ 3)      /* UBYTE  */
#define  UDNF_CALLSTODAY       (128+ 4)      /* UBYTE  */
#define  UDNF_CALLLIMITPERDAY  (128+ 5)      /* UBYTE  */
#define  UDNF_TIMELIMIT        (128+ 6)      /* UWORD  */
#define  UDNF_TIMEREMAINING    (128+ 7)      /* UWORD  Do not use !! */
#define  UDNF_EDITOR           (128+ 8)      /* UBYTE  */
#define  UDNF_PROTOCOL         (128+ 9)      /* UBYTE  */
#define  UDNF_FILERATIO        (128+10)      /* UBYTE  */
#define  UDNF_BYTERATIO        (128+11)      /* UBYTE  */
#define  UDNF_FILESDOWNLOADEDTODAY  (128+12) /* UBYTE  */
#define  UDNF_DOWNLOADS        (128+13)      /* UWORD  */
#define  UDNF_UPLOADS          (128+14)      /* UWORD  */
#define  UDNF_DOWNLOADBYTES    (128+15)      /* ULONG  */
#define  UDNF_UPLOADBYTES      (128+16)      /* ULONG  */
#define  UDNF_MAXBYTESPERDAY   (128+17)      /* ULONG  */
#define  UDNF_BYTESDOWNLOADEDTODAY  (128+18) /* ULONG  */
#define  UDNF_MAXMSGSPERDAY    (128+19)      /* UBYTE  */
#define  UDNF_MSGSWRITTENTODAY (128+20)      /* UBYTE  */
#define  UDNF_MESSAGES         (128+21)      /* ULONG  */
#define  UDNF_MENUSET          (128+22)      /* UBYTE  */

#define  UDNF_TOTDOWNLOADS     (128+23)      /* UWORD  */
#define  UDNF_TOTDOWNLOADBYTES (128+24)      /* ULONG  */


/* Note that DOWNLOADS and DOWNLOADBYTES refer to Downloads on ratio */
/* they do not include files marked FreeDownload                     */
/* TOTDOWNLOADS and TOTDOWNLOADBYTES do include freedownloads        */




/* When running a door you can get a pointer to 'SharedData' from  */
/* the BBS nodes with the paths to all BBS directories             */


#define  PATHLENGTH  60


struct Paths {
   UBYTE     Default   [ PATHLENGTH ];
   UBYTE     Config    [ PATHLENGTH ];
   UBYTE     Utils     [ PATHLENGTH ];
   UBYTE     Scripts   [ PATHLENGTH ];
   UBYTE     Users     [ PATHLENGTH ];
   UBYTE     Logs      [ PATHLENGTH ];
   UBYTE     Data      [ PATHLENGTH ];
   UBYTE     Icons     [ PATHLENGTH ];
   UBYTE     Samples   [ PATHLENGTH ];
   UBYTE     Doors     [ PATHLENGTH ];

   UBYTE     Menus     [ PATHLENGTH ];
   UBYTE     MenuText  [ PATHLENGTH ];
   UBYTE     Text      [ PATHLENGTH ];

   UBYTE     FileCat   [ PATHLENGTH ];
   UBYTE     Upload    [ PATHLENGTH ];
   UBYTE     Killed    [ PATHLENGTH ];

   UBYTE     Mail      [ PATHLENGTH ];
   UBYTE     Inbound   [ PATHLENGTH ];
   UBYTE     Outbound  [ PATHLENGTH ];
   UBYTE     NodeLists [ PATHLENGTH ];

   UBYTE     TempDir   [ PATHLENGTH ];
};



struct SharedData {
   struct Paths      Paths;

   /* rest of the structure is private */
};


