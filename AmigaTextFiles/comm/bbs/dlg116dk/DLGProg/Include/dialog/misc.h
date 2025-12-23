/***************************************************************************

 © copyright 1995-96 by DLG Development
   All rights reserved

*****************************************************************************/

#ifndef DOS_DOS_H
#ifdef SAS5
#include <libraries/dos.h>
#else
#include <dos/dos.h>
#endif
#endif


/* Computer types - DLGCONFIG:Port/computertypes.bbs */

struct Computer_Type
{
  char    Number;    /* number representing the computer type (1-127)   */
  char    Name[36];  /* text describing the computer type               */
  char    dummy;     /* reserved for future use                         */
};


/* Groups - DLGCONFIG:Group/Group.bbs */

struct Group_Def
{
  char name[36];     /* name of the group       */
  char gop[36];      /* groupop of the group    */
  char status;       /* not currently used      */
  char filler[40];   /* reserved for future use */
  char dummy;        /* reserved for future use */
};

#define GAUTO   1
#define GVOL    2
#define GSPEC   3

/* DLGConfig:Group/#?.Group */

struct Group_Members
{
   char Name[36];
};


/* FidoNet settings - DLGCONFIG:Port/FidoNet.Settings */

struct fido
{
  SHORT zone;              /* zone number for this bbs                           */
  SHORT node;              /* node number for this bbs                           */
  SHORT net;               /* net number for this bbs                            */
  char origin[75];         /* default origin line for this bbs                   */
  SHORT flags;             /* these are the default fidonet flags for messages   */
  char nodelistpath[78];   /* path to the nodelist                               */
  SHORT point;             /* point for this bbs (zero if none)                  */
  char msgid[80];          /* not currently used                                 */
};


/* Editors - DLGCONFIG:Misc/Editors.bbs */

struct Editor
{
  char   number;           /* number representing the editor (1-127)       */
  SHORT   level;           /* level required to use editor                 */
  char   name[60];         /* text string describing the editor            */
  char   callstring[128];  /* string to call the editor                    */
  long   flags;            /* handler flag settings required to use editor */
};

/* Archivers - DLGConfig:Misc/Archivers.bbs */

struct Archiver
{
  char   number;              /* number representing archiver                       */
  SHORT  level;               /* user level required to use archiver                */
  char   name[60];            /* text string describing archiver                    */
  char   compress[128];       /* call string to compress                            */
  SHORT  compresssuccess;     /* result code returned by successful compression     */
  char   decompress[128];     /* call string to decompress                          */
  SHORT  decompresssuccess;   /* result code returned by successful decompression   */
  char   view[128];           /* call string to view                                */
  SHORT  viewsuccess;         /* result code returned by successful view            */
  char   integrity[128];      /* call string to integrity check                     */
  SHORT  integritysuccess;    /* result code returned by successful integrity check */
  char   extension[12];       /* archive extension                                  */
  char   compression;         /* average compression percentage                     */
  char   filler[36];          /* reserved for future use                            */
};

#define COMPRESS      1
#define DECOMPRESS    2
#define VIEW          4
#define INTEGRITY     8


/* Character sets */

struct CharSet
{
  char          number;       /* number representing character set         */
  char          inputflag;    /* used internally                           */
  char          name[12];     /* string describing character set           */
  char          netname[24];  /* not used                                  */
  char          font[36];     /* not used                                  */
  char          keymap[36];   /* not used                                  */
  unsigned char level;        /* used level required to use character set  */
  char          filler[36];   /* not used                                  */
  char          dummy;        /* not used                                  */
};


/* this structure is used by the DLG Search#?() commands  */

struct SearchCookie
{
  char *searchpat;
  BPTR searchlock;
  struct FileInfoBlock *searchfib;
  char currentsearchfile[36];
  char searchdone;
};


/* this is used internally */

struct Question
{
  char quest[61];
  char template[61];
};


struct ATime
{
  int sec, min, hour,
  mday, mon, year,
  wday, yday;
};
