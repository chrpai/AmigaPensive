/***************************************************************************

 © copyright 1995-96 by DLG Development
   All rights reserved

*****************************************************************************/

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif

struct MenuHeader
{
  char    name[61];     /* menu name to be displayed on menu                  */
  char    columns;      /* number of columns for menu                         */
  char    prompt[256];  /* configurable prompt for menu                       */
  UBYTE   numentry;     /* routine that gets called when a number is entered  */
  char    action[81];   /* Activity string associated with the menu           */
  char    module[16];   /* Menu name                                          */
  char    unused[20];   /* Future expansion                                   */
};


struct MenuEntry
{
  char command;            /* Menu Letter                            */
  char program[71];        /* Command to execute                     */
  char description[35];    /* Command description                    */
  char helpfile[17];       /* Help file (menu/letter) for entry      */
  UBYTE type;              /* Type of command (See below)            */
  UBYTE llev,ulev;         /* User level required                    */
  UBYTE visibility;        /* (see below)                            */
  UBYTE flags;             /* Flags (see below)                      */
  UBYTE logvalue;          /* Log entry value                        */
  char action[81];         /* Activity string associated with entry  */
  SHORT rate;              /* Cost/min                               */
  SHORT cost;              /* Base cost                              */
  SHORT maxtime;           /* Max minutes in menu entry              */
  char priority;           /* Task priority                          */
  char unused[36];         /* Room to grow                           */
};

/* Menu types */

#define MENU_EXE       1       /* Executeable File    */
#define MENU_BATCH     2       /* Batch File          */
#define MENU_SUB       3       /* SubMenu             */
#define MENU_DLGBATCH  4       /* Dialog Batch File   */
#define MENU_STACK     5       /* Command stack       */
#define MENU_AREXX     6       /* ARexx script        */
#define MENU_BUILTIN   7       /* Built-in function   */

/* Menu Visibility */

#define MENU_HIDDEN   1
#define MENU_GHOST    2

/* Menu Flags */

#define MENU_CHAIN    1
#define MENU_BCPEND   2
#define MENU_CLI      4
#define MENU_RETURN   8
#define MENU_ASK      16


struct NewShortMenu
{
  char  name[21];
  UBYTE status;
};

struct onecoldata {
  char number[3];
  char title[14];
  char data[60];
  char dummy;
};

struct twocoldata {
  char number[3];
  char title[14];
  char data[24];
  char dummy;
};

struct threecoldata {
  char number[3];
  char title[14];
  char data[8];
  char dummy;
};

struct ShortMenu {
  char    command;
  char    description[15];
  char    status;
  char    dummy;
};

struct MenuSet
{
  unsigned char number; /* number designating the character set (1-127)    */
  char    name[60];     /* name of the character set as displayed online   */
  unsigned char level;  /* user level required to use character set        */
  char    language[22]; /* language file used by the menu set              */
  char    filler[2];    /* reserved for future use                         */
};

#define MENUNOTFOUND  -10
