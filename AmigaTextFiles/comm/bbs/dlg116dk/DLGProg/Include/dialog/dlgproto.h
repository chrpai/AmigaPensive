/***************************************************************************

 © copyright 1995-96 by DLG Development
   All rights reserved

*****************************************************************************/


struct DLGSerInfo
{
  char port[4];               /* Port info is about               */
  struct IOExtSer *read;      /* IOMessage for reading            */
  struct IOExtSer *write;     /* IOMessage for writing            */
  struct MsgPort *readbak;    /* Backup of handler's read port    */
  struct MsgPort *writebak;   /* Backup of handler's write port   */
  char titlebak[71];          /* Backup of screen/window title    */
  char title[71];             /* New title                        */
  unsigned char flags;        /* flags (as below)                 */
};

#define REFRESH_TITLE 1 /* Should the title be refreshed to
                           reflect new transfer information */
