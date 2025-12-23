/***************************************************************************

 © copyright 1990-93 by DLG Development
   All rights reserved

*****************************************************************************/


#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif

/* Global configuration - DLGCONFIG:Port/<name>.globals */

struct Global_Settings
{
  char    BBSName[36];     /* name of bbs                                     */
  char    GlobalScreen;    /* determines whether screen opens for users       */
  char    DefaultScreen;   /* determines between screen or window             */
  char    TimeOut;         /* idle timeout for port in seconds                */
  char    VerbPause;       /* selects between verbose pause or silent pause   */
  SHORT   Session_Limit;   /* not used                                        */
  SHORT   Daily_Limit;     /* not used                                        */
  char    ConnectDelay;    /* delay between connect and session start         */
  SHORT   NumUsers;        /* current number of users                         */
  char    NewUserLevel;    /* not used                                        */
  SHORT   DirLimit;        /* not used                                        */
  char    AutoAccess;      /* selects between auto access and private port    */
  char    NameList[20];    /* is the groupname for a private port             */
  char    DefMenu[18];     /* menu that is selected at login                  */
  char    CmdStack[10];    /* not used                                        */
  SHORT   MinBaud;         /* minimum baud rate for port                      */
  char    Ratio;           /* not used                                        */
  char    Language[22];    /* default language for this port                  */
  char    Origin[53];      /* not used                                        */
  unsigned char CharSet;   /* default characterset translation for this port  */
  unsigned char unused[3]; /* not used                                        */
  SHORT   Flags;           /* Flags                                           */
  char    ForcedStack[10]; /* command stack forced at login                   */
  SHORT   PrivateArea;     /* private message redirect area                   */
};

#define AUTO_ANSI       256     /* Do Auto-ANSI detect */


/* Port config - DLGCONFIG:Port/<name>.port */

struct Port
{
  char    Device[36];       /* normally serial.device       */
  char    Unit;             /* serial device unit number    */
  char    GlobalFile[36];   /* globals file for this port   */
  char    ModemFile[36];    /* modem file for this port     */
  char    DisplayFile[36];  /* display file for this port   */
  char    dummy;            /* not used                     */
};


/* Modem config - DLGCONFIG:Port/<name>.modem */

struct Modem
{
  char    Init[80];         /* modem init string                           */
  char    Hangup[10];       /* hangup string                               */
  char    Reset[10];        /* reset string                                */
  char    Lock;             /* lock mode                                   */
  char    HangupMethod;     /* selects between DTR and command hangup      */
  long    MaxBaud;          /* baud rate to send command to the modem at   */
  char    Answer[10];       /* command to answer the phone                 */
  char    AnswerMethod;     /* selects between auto answer and bbs answer  */
  char    Ring[10];         /* ring string                                 */
  char    CommandMode[10];  /* return to command mode                      */
};


/* Display config */

/* Window configuration */

struct WinStruct
{
  short x, y, width, height;  /* window dimensions          */
  char fontname[41];          /* font name (case sensitive) */
  UBYTE fontsize;             /* size of font               */
  UBYTE flags;                /* see display flags below    */
};

/* Screen configuration */

struct ScrStruct
{
  short width, height, depth; /* window dimensions                      */
  UBYTE hires, interlace;     /* screen modes                           */
  char fontname[41];          /* font name (case sensitive)             */
  UBYTE fontsize;             /* size of font                           */
  UBYTE flags;                /* see display flags below                */
  UWORD colortable[8];        /* array defining each of the 8 colours   */
};

/* Display flags */

#define DISP_BKGRND  1  /* Open window/screen in background */


/* The two above structures as found in DLGCONFIG:Port/<name>.display */

struct Displays
{
  struct ScrStruct Screen;
  struct WinStruct Window;
};

