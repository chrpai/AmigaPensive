/***************************************************************************

 © copyright 1995-96 by DLG Development
   All rights reserved

*****************************************************************************/


/* Daily log format - DLGCONFIG:TEXT/log.<day> */

struct Log_Entry
{
  unsigned char Code;
  long Long_Time;
  char Name[36];
  char Ext[4];
  char Info[36];
};


/* Definitions for Code field */

#define VIEW_INFO          1
#define VIEW_USER_STATS    2
#define USER_OPTION_MODULE 3
#define PAGED_SYSOP        4
#define VIEW_LOG           5
#define VIEW_TDIH          6
#define SYSOP_CONFIG       7
#define DROP_TO_DOS        8
#define READ_HELP          9
#define NORMAL_LOGOUT     10
#define WROTE_MESSAGE     11
#define KILLED_MESSAGE    12
#define EDITED_SIGNATURE  13
#define RE_EDITED_MESSAGE 14
#define WROTE_BULLETIN    15
#define BAD_PASSWORD      16
#define SUCCESSFUL_LOGIN  17
#define HAPPY_BIRTHDAY    18
#define UPLOADED_FILE     19
#define KILLED_FILE       20
#define RE_EDITED_FILE    21
#define DOWNLOADED_FILE   22
#define HURLED_FILE       23
#define NEW_USER          24
#define CONF              25
#define HURLED_MESSAGE    26
#define FORWARDED_MESSAGE 27
#define CONNECT           28
#define LOST_CARRIER      29

/* Custom log definitions - DLGCONFIG:Port/customlog.bbs */

struct Log
{
  unsigned char  number;   /* value from 128 and 255 representing the number  */
  char     desc[22];       /* log text                                        */
  unsigned char  level;    /* level of user that can view this log entry      */
};


/* Log Levels - DLGCONFIG:Port/LogLevels.bbs */

struct LogLevels
{
  unsigned char Log_Access[255];
  char dummy;
};
