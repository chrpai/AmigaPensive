/***************************************************************************

 © copyright 1995-96 by TelePro Technologies
   All rights reserved

The material within is the sole property of DLG Development.  It may
not be distributed outside of this archive, in whole or in part, without
express written permission from DLG Development.

*****************************************************************************/

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif

#define FREQ_AREA       2
#define VALIDATION_AREA 4
#define TEMP_DOWNLOAD   8

#define RATIO_AREA      2
#define DOWNLOAD        128

#define Upload_Priv     1
#define Ratio_Overide   32
#define Download_Priv   128

/* File header - First part of FILE:<number>/<nunber>.fd
   The header is followed by a null-terminated body of text */

struct  File_Header
{
  char    From[36];           /* The user who uploaded the file                  */
  char    Filename[36];       /* Filename of the file this is describing         */
  char    Date[20];           /* Text string of the date the file was uploaded   */
  SHORT   Times_Downloaded;   /* Number of downloads the file has had            */
  long    Size;               /* Size of the file in bytes                       */
  SHORT   Attribute;          /* See attributes below                            */
};

/* Flags for the attribute field of the file header */

#define NORATIO   1 /* No file ratio will be imposed on this file */
#define UNVALIDATED 2 /* This is an unvalidated file */


/* Quick file index

 The following structure must be written alphabetically on the first field
 into an index file called 'file.dat' in each public file area when a file is
 added.  Also must be deleted when the file is removed.  Applicable fields
 must also be modified in the index file when the filename, size, date, or
 description changes. Use AddStruct() if possible.

 All utilities that place a file into a public file area must be modified to
 update this file. (Private files need not worry about this index file).

*/

struct QuickFile
  {
  char    filename[36];
  long    date;         /* number of seconds since Jan 1, 1978 */
  SHORT   number;       /* File number (.fd file number)       */
  long    size;         /* size of file in bytes               */
  char    desc[60];     /* 60 character of the description     */
  };


/* Tagged file list - USER:<first>_<last>/Batch.File        */
/* Must be stored in alphabetical order based on filename   */

struct Batch
{
  char    filename[40]; /* Filename of the file                   */
  char    path[45];     /* full path to the filename              */
  char    free;         /* 1 = Free download  0 = Normal download */
  long    size;         /* Size of the file in bytes              */
};


/* File transfer protocol - DLGCONFIG:Misc/Protocols.bbs */

struct Protocol
{
  char     letter;         /* Single letter used to define the protocol          */
  SHORT    level;          /* User level required to use this protocol           */
  SHORT    flags;          /* See defines below                                  */
  char     name[41];       /* Text describing the name of the protocol           */
  char     sendstring[81]; /* Call string for sending a file                     */
  char     recstring[81];  /* Call string for receiving a file                   */
  char     sendbatch[81];  /* Call string for a batch send                       */
  UBYTE    sendrate;       /* Average send efficiency eg; 100 = 100%             */
  UBYTE    recrate;        /* Average receive efficiency eg; 100 = 100%          */
  SHORT    sendsuccess;    /* Code returned by the program for successful send   */
  SHORT    recsuccess;     /* Code returned by program for successful receive    */
  char     filler[128];    /* Reserved for future expansion                      */
};

#define PROTO_SEND   1  /* This protocol can send files              */
#define PROTO_REC    2  /* This protocol can receive files           */
#define PROTO_BATCH  4  /* This protocol has batch capabilities      */
#define PROTO_RESUME 8  /* This protocol can resume transfers        */
#define PROTO_FNAME  16 /* This protcol requires a supplied filename */


/* File transfer paths - DLGCONFIG:Misc/FilePaths.bbs */

struct path {char path[60];
            };
