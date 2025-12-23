/***************************************************************************

 © copyright 1995-96 by DLG Development
   All rights reserved

*****************************************************************************/


#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif

/* User data structure - USER:First_Last/User.Data */

struct USER_DATA
{
  char    Password[12];     /* users password                              */
  char    Screen_Len;       /* length of screen                            */
  char    Screen_Width;     /* width of screen                             */
  UBYTE   Ansi_Flag;        /* ansi flag (see defines)                     */
  char    More_Flag;        /* more flag (1=YES 0=NO)                      */
  char    Help_Level;       /* 0=Novice 1=Intermediate 2=Expert            */
  char    Alias[36];        /* alias                                       */
  char    Computer_Type;    /* number designating computer type            */
  SHORT   Daily_Limit;      /* daily call limit in minutes                 */
  SHORT   Session_Limit;    /* single session limit in minutes             */
  SHORT   Daily_Used;       /* amount of time used on last date logged in  */
  char    Bulletin_Access;  /* user can write bulletins ((1=YES 0=NO)      */
  char    send;             /* send protocol number                        */
  char    receive;          /* receive protocol number                     */
  UBYTE   msgsig;           /* last message sig the user was in            */
  UBYTE   filesig;          /* last file sig the user was in               */
  char    archiver;         /* number of default archiver (0=none)         */
  char    menuset;          /* number of default menu set                  */
  char    fileorder;        /* last selection for file list order          */
  char    filesince;        /* last selection for file search item         */
  char    charset;          /* users selected character set                */
  char    Temp_Filler[7];   /* not used                                    */
  char    Birth_Year;       /* year of birth - 1900 (eg 56 = 1956)         */
  SHORT   Last_Area;        /* last message area user visited              */
  SHORT   User_Level;       /* users level                                 */
  char    Lex_Flag;         /* lex checking flag (1=YES 0=NO)              */
  char    Editor;           /* default message editor                      */
  char    Private_Flag;     /* not used                                    */
  char    Last_Login[27];   /* text string denoting last login date/time   */
  char    Ratio;            /* number denoting users upload download ratio */
  UBYTE   NetMail;          /* netmail access flag (see defines)           */
  char    ThreadMode;       /* last threadmode setting                     */
  char    Phone[15];        /* telephone number                            */
  char    City[24];         /* city                                        */
  char    Prov[6];          /* province/state                              */
  char    Pop_Screen;       /* pop screen for this user (1=YES 0=NO)       */
  char    Hot_Keys;         /* hot key flag (1=YES 0=NO)                   */
  long    Total_Calls;      /* total calls this user has made              */
  long    Online_Time;      /* total number of minutes spent online        */
  long    Messages_Entered; /* number of messages user has entered         */
  long    Messages_Read;    /* number of messages read online              */
  ULONG   Bytes_Uploaded;   /* number of bytes uploaded                    */
  ULONG   Bytes_Downloaded; /* number of bytes downloaded                  */
  long    Files_Uploaded;   /* number of files uploaded                    */
  long    Files_Downloaded; /* number of files downloaded                  */
  long    Sysop_Pages;      /* number of sysop pages                       */
  char    Address[26];      /* street address                              */
  char    Birth_Month;      /* month of birth                              */
  char    Birth_Day;        /* date of birth                               */
  SHORT   Last_File_Area;   /* last file area visited                      */
  char    stack[40];        /* login command stack                         */
  char    UUCP;             /* uucp flag (see below)                       */
  char    Country[25];      /* country of origin                           */
  ULONG   Long_Date;        /* manx encoded long of date/time of last/current login */
  SHORT   DirLimit;         /* number of bytes allowed in private directory */
  SHORT   credit;           /* amount of netmail credit                    */
  char    postal[8];        /* postal code                                 */
  char    protocol;         /* not used                                    */
  char    captureflags;     /* default capture flag settings               */
  long    DateJoined;       /* date of first application (manx encoded long) */
};

/* Flags for NetMail */

#define NET_WRITE 1
#define NET_SYSOP 2

/* Flags for UUCP */

#define UUCP_CLIENT     1   /* this account is a client (fake account)  */
#define USENET_ACCESS   2   /* this user has uucp access                */

/* Help levels */

#define EXPERT       2
#define INTERMEDIATE 1
#define NOVICE       0

/* Ansi settings */

#define ANSI_COLOR     1  /* colour selected                      */
#define ANSI_CLR       2  /* screen clears selected               */
#define ANSI_POS       4  /* ansi positioning selected            */
#define ANSI_SCROLL    8  /* partial screen scroll selected       */
#define ANSI_NOFREEZE 16  /* no message header freezing selected  */

/* Session information - T:<portname>.user */

struct Ram_File
{
  char      Name[36];           /* name of user on this port                  */
  long      Baud_Rate;          /* users baud rate                            */
  char      Command_Stack[256]; /* storage for command stack while switching  */
                                /* between modules                            */
  char      Local_Flag;         /* user is logged in locally                  */
  char      Action[81];         /* users current action                       */
  char      module;             /* module number user in in                   */
  short     area;               /* area number user is in (msg or file)       */
  char      CLI;                /* cli process number                         */
  char      menu[13];           /* menu name they left the MENU module using  */
  char      DefAction[81];      /* default action for current menu            */
  char      chainflag;          /* used internally                            */
  long      status;             /* See below                                  */
  ULONG     suspended;          /* time clock suspended                       */
  SHORT     time_left;          /* time left when suspended                   */
  SHORT     suspd_adj;          /* online time adjustment                     */
  char      unused[21];         /* not used                                   */
};

/* Bits for status field */

#define IN_AREXX  1

struct NameStruct
{
  char    name[36];
};
