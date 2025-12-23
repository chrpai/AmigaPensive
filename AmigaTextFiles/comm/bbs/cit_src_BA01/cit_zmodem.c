#include <exec/types.h>
#include <dos/dos.h>
#include <devices/timer.h>
#include <exec/memory.h>
#include <ctype.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <exec/execbase.h>
#include <math.h>
#include <devices/timer.h>
#include <devices/serial.h>
#include "xproto.h"
#include <exec/memory.h>
#include "ctdl.h"
#include "sysdep.h"
#include "xproto.h"
#include "zmodem.h"
#include "xprzmodem.h"

struct Library *TimerBase;
static struct XPR_IO xio;
static struct timerequest treq;
static struct MsgPort *tport;
static char timeropen;

extern CONFIG    cfg;            /* Lots an lots of variables    */

extern struct IOExtSer *mySerReadMsg;
extern struct MsgPort *mySerReadPort;
struct IOExtSer *xpr_serio;
struct MsgPort *smp;

long xpr_sread(char *buffer, long size, long timeout);
long xpr_swrite(char *buffer, long size);
long xpr_sflush(void);
long xpr_ffirst(char *buffer, char *pattern);
long xpr_fnext(long OldState, char *buffer, char *pattern);
long xpr_finfo(char *name, long type);
long xpr_gets(char *b, char *p);


long xpr_chkabort(void);
void init_xpr ( struct XPR_IO *IO );
void plog(char *string, long arg1, long arg2  );


long   display_update (struct XPR_UPDATE *xu );
long   Do_Zmodem(char *filename, int sending);

void close_xpr ( void );

char *filelist;   /* initial file list from Citadel */

long
Do_Zmodem(filename, sending)
char   *filename;            /* file(s) to  send/recieve  */
int     sending;             /* TRUE - sending, FALSE - receive */
  {
  long status;
  plog("Files:%s Direction:%s",(long)filename, (long)(sending ? "TRUE" : "FALSE") );
  xpr_serio = mySerReadMsg;
  smp       = mySerReadPort;
  init_xpr(&xio);
  status = TRUE;
  if (!xpr_chkabort())
    {
    filelist         = filename;
    xio.xpr_filename = strtok(filelist," ");  /* Citadel delimits with a space*/
    if (!sending)
      {
      status = XProtocolSend(&xio);
      plog("XProtocolSend returned: %08.8lX",status,0L);
      }
    else
      {
      status = XProtocolReceive(&xio);
      plog("XProtocolReceive returned: %08.8lX",status,0L);
      };
    };
  XProtocolCleanup(&xio);
  return  status;
  }
void
init_xpr(struct XPR_IO *IO)
  {
  plog("init_xpr: XPR_IO =%08.8lX", (long)IO, 0L);
  IO->xpr_filename  = NULL;
  IO->xpr_data      = NULL;
  }

void
plog(char *string, long arg1, long arg2)
  {
  long    t;
    t = time(0);
    splitF(NULL,"%24.24s : ", ctime(&t));
    splitF(NULL, string, arg1, arg2);
    splitF(NULL, "\n", 1);

  }

char the_protocol[20];  /* the current protocol name */
char the_file[30];      /* the current filename */
long the_size;          /* size of current file */

long   display_update (struct XPR_UPDATE *xu )
  {
  int flag = FALSE;
  if( !xu)
    {
    plog(" XPR_UPDATE is NULL",0L, 0L);
    return 0;
    }
  if( XPRU_PROTOCOL && xu->xpru_updatemask)
    {
    if( xu->xpru_protocol)strncpy(the_protocol,xu->xpru_protocol,19);
    the_protocol[19] = '\0';
    };
  if( XPRU_FILENAME && xu->xpru_updatemask)
    {
    if( xu->xpru_filename)strncpy(the_file,xu->xpru_filename,29);
    the_protocol[19] = '\0';
    flag = TRUE;
    };
  if( XPRU_FILESIZE && xu->xpru_updatemask)the_size = xu->xpru_filesize;
  if( XPRU_MSG && xu->xpru_updatemask)
    plog("XPRU_MSG:%s",(long)xu->xpru_msg,0L);
  if( XPRU_ERRORMSG && xu->xpru_updatemask)
    plog("XPRU_ERRORMSG:%s on file %s",(long)xu->xpru_errormsg,(long)the_file);
  if( XPRU_ERRORS && xu->xpru_updatemask)
    {
    if( xu->xpru_errors != 0 )
      plog("XPRU_ERRORS:%ld on file %s",(long)xu->xpru_errors,(long)the_file);
    };
  if( flag )
    {
    plog("Protocol:%s File:%s",(long)the_protocol,(long)the_file);
    };
  return 1;
  }

long    xpr_chkabort(void)
  {
  return (gotCarrier()? 0L : -1L);

  }
long  opentimer(void)
  {
  plog("opentimer", 0L, 0L);
  tport = CreatePort(0, 0);
  if (!tport)  return (-1);
  treq.tr_node.io_Message.mn_ReplyPort = tport;
  if (OpenDevice("timer.device", UNIT_VBLANK, (struct IORequest *) &treq, 0))
  return (-1);
  TimerBase = (struct Library *)treq.tr_node.io_Device;
  timeropen = 1;
  return (0);

  }
void  closetimer(void)
  {
  plog("closetimer", 0L, 0L);
  if (timeropen)
  CloseDevice((struct IORequest *) &treq);
  if (tport)
  DeletePort(tport);
  TimerBase = NULL;
  }
void  qtimer(long micros)
  {
  long    secs = 0;
  plog("qtimer: micros = %ld", micros, 0L);
  if (micros > 1000000)
    {
    secs = micros / 1000000;
    micros = micros % 1000000;

    }
  treq.tr_time.tv_micro = micros;
  treq.tr_time.tv_secs = secs;
  treq.tr_node.io_Command = TR_ADDREQUEST;
  /* printf("QTIMER  * s=%ld  * m=%ld\n",secs,micros); */
  SendIO((struct IORequest *) &treq);

  }
/* the * finfo-function  */
long
xpr_finfo(char * FileName,LONG InfoType)
  {
  struct FileInfoBlock    *FileInfo;
  LONG                     Size=0;
  plog("finfo:%s, InfoType:%ld", 0L, 0L);
  switch(InfoType)
    {
    case 1:       /* Return the file size. */
      if(FileInfo = (struct FileInfoBlock *)AllocMem(sizeof(struct FileInfoBlock), MEMF_CLEAR))
        {
        BPTR    FileLock;
        if(FileLock = Lock(FileName,ACCESS_READ))
          {
          if(Examine(FileLock,FileInfo))
            {
            if(FileInfo -> fib_DirEntryType < 0)Size = FileInfo->fib_Size;
            };
          UnLock(FileLock);
          };
        FreeMem(FileInfo, sizeof(struct FileInfoBlock));
        };
      break;
    case 2:         /* Return the file transfer mode. */
      Size = 1; /* always return Binary */
    break;
   }
return Size;
}


/* the * serwrite  * function  */
long
xpr_swrite(char *buffer, long size)
  {
  plog("swrite:%08.8lX of size %ld", (long)buffer, size);
  xpr_serio->IOSer.io_Length  = size;
  xpr_serio->IOSer.io_Data    = buffer;
  xpr_serio->IOSer.io_Command = CMD_WRITE;
  DoIO((struct IORequest *) xpr_serio);
  if( xpr_serio->IOSer.io_Error )
    plog("  Error:%08.8lx", xpr_serio->IOSer.io_Error, 0L);
  return ((long) xpr_serio->IOSer.io_Error);

  }
/* the * serread  * function  */
long
xpr_sread(char *buffer, long size, long timeout)
  {
  long    flag = 1 << xpr_serio->IOSer.io_Message.mn_ReplyPort->mp_SigBit;
  long    len,
  nflag;
  plog("sread: size %ld with timeout of %ld", size, timeout);
  SetSignal(0, flag);
  if (timeout)
    {
    flag |= 1 << tport->mp_SigBit;
    /* printf("Starting  * timer...\n"); */
    qtimer(timeout);

    }
  else
    {
    xpr_serio->IOSer.io_Command = SDCMD_QUERY;
    DoIO((struct IORequest *) xpr_serio);
    if (!(len = xpr_serio->IOSer.io_Actual))
    return (0);
    else
      {
      if (len > size)
      len = size;
      xpr_serio->IOSer.io_Command = CMD_READ;
      xpr_serio->IOSer.io_Data = buffer;
      xpr_serio->IOSer.io_Length = len;
      DoIO((struct IORequest *) xpr_serio);
      /* printf("Fastread  * return  * * * *
      * %ld\n",xpr_serio->IOSer.io_Actual); */
      return ((long) xpr_serio->IOSer.io_Actual);

      }

    }
  SetSignal(0, flag);
  xpr_serio->IOSer.io_Command = CMD_READ;
  xpr_serio->IOSer.io_Data = buffer;
  xpr_serio->IOSer.io_Length = size;
  SendIO((struct IORequest *) xpr_serio);
  nflag = Wait(flag);
  if (nflag)
    {
    AbortIO((struct IORequest *) &treq);
    AbortIO((struct IORequest *) xpr_serio);
    WaitIO((struct IORequest *) &treq);
    WaitIO((struct IORequest *) xpr_serio);
    return (-1);

    }
  if (nflag & (1 << xpr_serio->IOSer.io_Message.mn_ReplyPort->mp_SigBit))
    {
    AbortIO((struct IORequest *) &treq);
    WaitIO((struct IORequest *) xpr_serio);
    WaitIO((struct IORequest *) &treq);
    return ((long) xpr_serio->IOSer.io_Actual);

    }
  plog("  Timeout occured", 0L, 0L);
  AbortIO((struct IORequest *) xpr_serio);
  WaitIO((struct IORequest *) &treq);
  WaitIO((struct IORequest *) xpr_serio);
  return ((long) xpr_serio->IOSer.io_Actual);

  }
/* sflush  * - * flushes  * serial  * port  */
long
xpr_sflush(void)
  {
  plog("sflush", 0L, 0L);
  xpr_serio->IOSer.io_Command = CMD_CLEAR;
  DoIO((struct IORequest *) xpr_serio);
  if( xpr_serio->IOSer.io_Error )
    plog("  Error:%08.8lx", xpr_serio->IOSer.io_Error, 0L);
  return ((long) xpr_serio->IOSer.io_Error);

  }
/**
  ffirst() -- setup first filename and return it
**/
long
xpr_ffirst(char *buffer, char *pattern)
  {
  char *token;
  plog("ffirst",0L,0L);
  token = strtok(NULL," ");  /* Citadel delimits filenames with a space*/
  if( token != NULL )
    {
    plog("  filename:%s", (long)token, 0L);
    strcpy(buffer,token);
    return(1);
    }
  return (0);
  }

long
xpr_fnext(long OldState,
          char *buffer,
          char *pattern)
  {
  char *token;
  plog("fnext",0L,0L);
  token = strtok(NULL," ");  /* Citadel delimits filenames with a space*/
  if( token != NULL )
    {
    plog("  filename:%s", (long)token, 0L);
    strcpy(buffer,token);
    return(1);
    }
  return (0);
  }


void
close_xpr(void)
  {
  plog("close_xpr",0L, 0L);
  closetimer();

  }
/**********************************************************************
 *   Z M . C
 *    ZMODEM protocol primitives
 *    01-19-87  Chuck Forsberg Omen Technology Inc
 *
 * 05-Apr-95: Converted to become a part of Citadel
 *
 * 29 July 89:
 * Major overhaul by Rick Huebner for adaptation to Amiga XPR protocol spec
 *
 * 28 October 89:
 * Converted to Lattice C 5.04
 *
 * 15 November 1991
 * Code added to support CRC-32 by William M. Perkins.
 *
 * 28 June 1993
 * Added run-length encoding code from original ZModem sources.
 **********************************************************************/

/*
 * Fast table-driven CRC functions for ZMODEM
 * * extracted from RBSB.C by Chuck Forsberg, Omen Technology, Inc.
 * *
 * * crctab calculated by Mark G. Mendel, Network Systems Corporation
 */

static unsigned short crctab[256] =
{
  0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
  0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
  0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
  0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
  0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
  0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
  0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
  0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
  0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
  0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
  0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
  0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
  0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
  0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
  0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
  0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
  0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
  0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
  0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
  0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
  0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
  0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
  0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
  0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
  0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
  0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
  0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
  0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
  0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
  0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
  0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
  0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0
};

/*
 * updcrc macro derived from article Copyright (C) 1986 Stephen Satchell.
 *  NOTE: First srgument must be in range 0 to 255.
 *        Second argument is referenced twice.
 *
 * Programmers may incorporate any or all code into their programs,
 * giving proper credit within the source. Publication of the
 * source routines is permitted so long as proper credit is given
 * to Stephen Satchell, Satchell Evaluations and Chuck Forsberg,
 * Omen Technology.
 */

#define updcrc(cp, crc) ( crctab[((crc >> 8) & 255)] ^ (crc << 8) ^ cp)

/*
 * Copyright (C) 1986 Gary S. Brown.  You may use this program, or
 * code or tables extracted from it, as desired without restriction.
 */

/*
 * First, the polynomial itself and its table of feedback terms.  The
 * polynomial is
 * X^32+X^26+X^23+X^22+X^16+X^12+X^11+X^10+X^8+X^7+X^5+X^4+X^2+X^1+X^0
 * Note that we take it "backwards" and put the highest-order term in
 * the lowest-order bit.  The X^32 term is "implied"; the LSB is the
 * X^31 term, etc.  The X^0 term (usually shown as "+1") results in
 * the MSB being 1.
 *
 * Note that the usual hardware shift  implementation, which
 * is what we're using (we're merely optimizing it by doing eight-bit
 * chunks at a time) shifts bits into the lowest-order term.  In our
 * implementation, that means shifting towards the right.  Why do we
 * do it this way?  Because the calculated CRC must be transmitted in
 * order from highest-order term to lowest-order term.  UARTs transmit
 * characters in order from LSB to MSB.  By storing the CRC this way,
 * we hand it to the UART in the order low-byte to high-byte; the UART
 * sends each low-bit to hight-bit; and the result is transmission bit
 * by bit from highest- to lowest-order term without requiring any bit
 * shuffling on our part.  Reception works similarly.
 */

/*
 * The feedback terms table consists of 256, 32-bit entries.  Notes:
 *
 *     The table can be generated at runtime if desired; code to do so
 *     is shown later.  It might not be obvious, but the feedback
 *     terms simply represent the results of eight shift/xor opera-
 *     tions for all combinations of data and CRC  values.
 *
 *     The values must be right-shifted by eight bits by the "updcrc"
 *     logic; the shift must be unsigned (bring in zeroes).  On some
 *     hardware you could probably optimize the shift in assembler by
 *     using byte-swap instructions.
 */

static unsigned long cr3tab[] =   /*
                                   * CRC polynomial 0xedb88320
                                   */
{
  0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f,
  0xe963a535, 0x9e6495a3, 0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
  0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91, 0x1db71064, 0x6ab020f2,
  0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
  0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9,
  0xfa0f3d63, 0x8d080df5, 0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
  0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b, 0x35b5a8fa, 0x42b2986c,
  0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
  0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423,
  0xcfba9599, 0xb8bda50f, 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
  0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d, 0x76dc4190, 0x01db7106,
  0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
  0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d,
  0x91646c97, 0xe6635c01, 0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
  0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457, 0x65b0d9c6, 0x12b7e950,
  0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
  0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7,
  0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
  0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9, 0x5005713c, 0x270241aa,
  0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
  0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81,
  0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
  0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683, 0xe3630b12, 0x94643b84,
  0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
  0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb,
  0x196c3671, 0x6e6b06e7, 0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
  0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5, 0xd6d6a3e8, 0xa1d1937e,
  0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
  0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55,
  0x316e8eef, 0x4669be79, 0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
  0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 0xc5ba3bbe, 0xb2bd0b28,
  0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
  0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f,
  0x72076785, 0x05005713, 0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
  0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21, 0x86d3d2d4, 0xf1d4e242,
  0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
  0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69,
  0x616bffd3, 0x166ccf45, 0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
  0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db, 0xaed16a4a, 0xd9d65adc,
  0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
  0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693,
  0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
  0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
};

#define UPDC32(b, c) (cr3tab[((int)c ^ b) & 0xff] ^ ((c >> 8) & 0x00FFFFFF))

static char *frametypes[] =
{
  "CARRIER_LOST",                 /*
                                   * -3
                                   */
  "TIMEOUT",                      /*
                                   * -2
                                   */
  "ERROR",                        /*
                                   * -1
                                   */
#define FTOFFSET 3
  "ZRQINIT",
  "ZRINIT",
  "ZSINIT",
  "ZACK",
  "ZFILE",
  "ZSKIP",
  "ZNAK",
  "ZABORT",
  "ZFIN",
  "ZRPOS",
  "ZDATA",
  "ZEOF",
  "ZFERR",
  "ZCRC",
  "ZCHALLENGE",
  "ZCOMPL",
  "ZCAN",
  "ZFREECNT",
  "ZCOMMAND",
  "ZSTDERR",
  "xxxxx"
#define FRTYPES 22                /*
                                   * Total number of frame types in this
                                   * array
                                   */
                        /*
                         * not including psuedo negative entries
                         */
};

/**********************************************************
 *      void zsbhdr(struct Vars *v, USHORT type)
 *
 * Send ZMODEM binary header hdr of type type
 **********************************************************/
void
zsbhdr(struct Vars *v, USHORT type)
{
  UBYTE    *hdr = v->Txhdr;
  short     n;
  USHORT    crc;
  ULONG     crc32;

  xsendline(v, ZPAD);
  xsendline(v, ZDLE);

  if (v->Crc32t = v->Txfcs32)   /*
                                 * zsbh32()
                                 */
    {
      xsendline(v, ZBIN32);
      zsendline(v, (UBYTE) type);

      crc32 = 0xFFFFFFFFL;
      crc32 = UPDC32(type, crc32);

      for (n = 4; --n >= 0; ++hdr)
        {
          crc32 = UPDC32((0377 & *hdr), crc32);
          zsendline(v, *hdr);
        }
      crc32 = ~crc32;
      for (n = 4; --n >= 0;)
        {
          zsendline(v, (int) crc32);
          crc32 >>= 8;
        }
    }
  else
    {
      xsendline(v, ZBIN);
      zsendline(v, (UBYTE) type);

      crc = updcrc(type, 0);
      for (n = 4; --n >= 0;)
        {
          zsendline(v, *hdr);
          crc = updcrc(((USHORT) (*hdr++)), crc);
        }

      crc = updcrc(((USHORT) 0), crc);
      crc = updcrc(((USHORT) 0), crc);
      zsendline(v, (UBYTE) (crc >> 8));
      zsendline(v, (UBYTE) crc);
    }
}       /*
         * End of void zsbhdr()
         */

/**********************************************************
 *      void zshhdr(struct Vars *v, USHORT type)
 *
 * Send ZMODEM HEX header hdr of type type
 **********************************************************/
void
zshhdr(struct Vars *v, USHORT type)
{
  UBYTE    *hdr = v->Txhdr;
  short     n;
  USHORT    crc;

  sendline(v, ZPAD);
  sendline(v, ZPAD);
  sendline(v, ZDLE);
  sendline(v, ZHEX);
  zputhex(v, (UBYTE) type);
  v->Crc32t = 0;

  crc = updcrc(type, 0);
  for (n = 4; --n >= 0;)
    {
      zputhex(v, *hdr);
      crc = updcrc(((USHORT) (*hdr++)), crc);
    }

  crc = updcrc(((USHORT) 0), crc);
  crc = updcrc(((USHORT) 0), crc);
  zputhex(v, (UBYTE) (crc >> 8));
  zputhex(v, (UBYTE) crc);

/*
 * Make it printable on remote machine
 */
  sendline(v, '\r');
  sendline(v, '\n');
/*
 * Uncork the remote in case a fake XOFF has stopped data flow
 */
  if (type != ZFIN)
    sendline(v, ZM_XON);
}       /*
         * End of void zshhdr()
         */

/**********************************************************
 *      void zsdata() and void zsda32()
 *
 * Send binary array buf of length length, with ending
 * ZDLE sequence frameend
 **********************************************************/

void
zsdata(struct Vars *v, short length, USHORT frameend)
{
  UBYTE    *buf;

  buf = v->Pktbuf;

  if (v->Crc32t)
    {
      short     c;
      ULONG     crc;

      crc = 0xFFFFFFFFL;
      for (; --length >= 0; ++buf)
        {
          c = *buf;
          if (c & 0140)
            xsendline(v, v->Lastzsent = c);
          else
            zsendline(v, c);
          crc = UPDC32(c, crc);
        }
      xsendline(v, ZDLE);
      xsendline(v, frameend);
      crc = UPDC32(frameend, crc);

      crc = ~crc;
      for (c = 4; --c >= 0;)
        {
          zsendline(v, (int) crc);
          crc >>= 8;
        }
    }
  else
    {
      unsigned short crc;

      crc = 0;
      for (; --length >= 0; ++buf)
        {
          zsendline(v, *buf);
          crc = updcrc(*buf, crc);
        }
      xsendline(v, ZDLE);
      xsendline(v, frameend);
      crc = updcrc(frameend, crc);

      crc = updcrc(0, updcrc(0, crc));
      zsendline(v, crc >> 8);
      zsendline(v, crc);
    }
  if (frameend == ZCRCW)
    {
      xsendline(v, ZM_XON);
    }
}       /*
         * End of void zsdata()
         */

/**********************************************************
 *      short zrdata(struct Vars *v, UBYTE *buf, short length)
 *
 * Receive array buf of max length with ending ZDLE sequence
 * and CRC-16.  Returns the ending character or error code.
 **********************************************************/
short
zrdata(struct Vars *v, UBYTE *buf, short length)
{
  short     c,
            d;
  USHORT    crc;

  if (v->Rxframeind == ZBIN32)
    return zrdat32(v, buf, length);

  crc = v->Rxcount = 0;
  for (;;)
    {
      if ((c = zdlread(v)) & ~0xFF)
        {
        crcfoo:
          switch (c)
            {
              case GOTCRCE:
              case GOTCRCG:
              case GOTCRCQ:
              case GOTCRCW:
                crc = updcrc(((d = c) & 0xFF), crc);
                if ((c = zdlread(v)) & ~0xFF)
                  goto crcfoo;
                crc = updcrc(c, crc);
                if ((c = zdlread(v)) & ~0xFF)
                  goto crcfoo;
                crc = updcrc(c, crc);
                if (crc & 0xFFFF)
                  {
                    strcpy(v->Msgbuf, "MSG_BAD_DATA_CRC16");
                    return ZM_ERROR;
                  }
                return d;
              case GOTCAN:
                strcpy(v->Msgbuf, "MSG_SENDER_CANCELED");
                return ZCAN;
              case TIMEOUT:
                strcpy(v->Msgbuf, "MSG_DATA_PACKET_TIMEOUT_TXT");
                return c;
              case RCDO:
                return c;
              default:
                strcpy(v->Msgbuf, "MSG_UNRECOGNIZABLE_DATA_PACKET_TXT");
                return c;
            }
        }
      if (--length < 0)
        {
          strcpy(v->Msgbuf, "MSG_DATA_PACKET_TOO_LONG_TXT");
          return ZM_ERROR;
        }
      ++v->Rxcount;
      *buf++ = c;
      crc = updcrc(c, crc);
      continue;
    }
}       /*
         * End of short zrdata()
         */

/**********************************************************
 *      short zrdat32(struct Vars *v, UBYTE *buf, short length)
 *
 * Receive array buf of max length with ending ZDLE sequence
 * and CRC-32.  Returns the ending character or error code.
 **********************************************************/
short
zrdat32(struct Vars *v, UBYTE *buf, short length)
{
  short     c,
            d;
  ULONG     crc32;

  crc32 = 0xFFFFFFFFL;
  v->Rxcount = 0;

  for (;;)
    {
      if ((c = zdlread(v)) & ~0xFF)
        {
        crcfoo:
          switch (c)
            {
              case GOTCRCE:
              case GOTCRCG:
              case GOTCRCQ:
              case GOTCRCW:
                d = c;
                c &= 0xFF;
                crc32 = UPDC32(c, crc32);
                if ((c = zdlread(v)) & ~0xFF)
                  goto crcfoo;
                crc32 = UPDC32(c, crc32);
                if ((c = zdlread(v)) & ~0xFF)
                  goto crcfoo;
                crc32 = UPDC32(c, crc32);
                if ((c = zdlread(v)) & ~0xFF)
                  goto crcfoo;
                crc32 = UPDC32(c, crc32);
                if ((c = zdlread(v)) & ~0xFF)
                  goto crcfoo;
                crc32 = UPDC32(c, crc32);
                if (crc32 != 0xDEBB20E3)
                  {
                    strcpy(v->Msgbuf, "MSG_BAD_DATA_CRC32_TXT");
                    return ZM_ERROR;
                  }
                return d;
              case GOTCAN:
                strcpy(v->Msgbuf, "MSG_SENDER_CANCELED_TXT");
                return ZCAN;
              case TIMEOUT:
                strcpy(v->Msgbuf, "MSG_DATA_PACKET_TIMEOUT_TXT");
                return c;
              case RCDO:
                return c;
              default:
                strcpy(v->Msgbuf, "MSG_UNRECOGNIZABLE_DATA_PACKET_TXT");
                return c;
            }
        }
      if (--length < 0)
        {
          strcpy(v->Msgbuf, "MSG_DATA_PACKET_TOO_LONG_TXT");
          return ZM_ERROR;
        }
      ++v->Rxcount;
      *buf++ = c;
      crc32 = UPDC32(c, crc32);
      continue;
    }
}       /*
         * End of short zrdat32()
         */

/**********************************************************
 *      short zgethdr(struct Vars *v)
 *
 * Read a ZMODEM header to hdr, either binary or hex.
 *  On success return type of header.
 *  Otherwise return negative on error.
 **********************************************************/
short
zgethdr(struct Vars *v)
{
  short     c,
            cancount;
  long      n;

  n = v->Baud;  /*
                 * Max characters before start of frame
                 */
  cancount = 5;
again:
  v->Rxframeind = v->Rxtype = 0;
  switch (c = noxrd7(v))
    {
      case RCDO:
      case TIMEOUT:
        goto fifi;
      case ZM_CAN:
        if (--cancount <= 0)
          {
            c = ZCAN;
            goto fifi;
          }
      case ZPAD:        /*
                         * This is what we want.
                         */
        break;
      default:
      agn2:
        if (--n <= 0)
          {
            strcpy(v->Msgbuf, "MSG_HEADER_SEARCH_GARBAGE_COUNT_EXCEEDED_TXT");
            return ZM_ERROR;
          }
        if (c != ZM_CAN)
          cancount = 5;
        goto again;
    }
  cancount = 5;
splat:
  switch (c = noxrd7(v))
    {
      case ZPAD:
        goto splat;
      case RCDO:
      case TIMEOUT:
        goto fifi;
      default:
        goto agn2;
      case ZDLE:        /*
                         * This is what we want.
                         */
        break;
    }

  switch (c = noxrd7(v))
    {
      case RCDO:
      case TIMEOUT:
        goto fifi;
      case ZBIN:
        v->Rxframeind = ZBIN;
        v->Crc32 = FALSE;
        c = zrbhdr(v);
        break;
      case ZBIN32:
        v->Crc32 = v->Rxframeind = ZBIN32;
        c = zrbhdr32(v);
        break;
      case ZHEX:
        v->Rxframeind = ZHEX;
        v->Crc32 = FALSE;
        c = zrhhdr(v);
        break;
      case ZM_CAN:
        if (--cancount <= 0)
          {
            c = ZCAN;
            goto fifi;
          }
        goto agn2;
      default:
        goto agn2;
    }
  v->Rxpos = rclhdr(v);
fifi:
  switch (c)
    {
      case GOTCAN:
        c = ZCAN;
      case ZNAK:
      case ZCAN:
      case ZM_ERROR:
      case TIMEOUT:
      case RCDO:
        sprintf(v->Msgbuf, "%s %s ", frametypes[c + FTOFFSET],
                  (c >= 0) ? "MSG_HEADER" : "MSG_ERROR");
    }
  return c;
}       /*
         * End of short zgethdr()
         */

/**********************************************************
 *      short zrbhdr(struct Vars *v)
 *
 * Receive a binary style header (type and position)
 **********************************************************/
short
zrbhdr(struct Vars *v)
{
  UBYTE    *hdr = v->Rxhdr;
  short     c,
            n;
  USHORT    crc;

  if ((c = zdlread(v)) & ~0xFF)
    return c;
  v->Rxtype = c;
  crc = updcrc(c, 0);

  for (n = 4; --n >= 0;)
    {
      if ((c = zdlread(v)) & ~0xFF)
        return c;
      crc = updcrc(c, crc);
      *hdr++ = c;
    }
  if ((c = zdlread(v)) & ~0xFF)
    return c;
  crc = updcrc(c, crc);
  if ((c = zdlread(v)) & ~0xFF)
    return c;
  crc = updcrc(c, crc);
  if (crc & 0xFFFF)
    {
      strcpy(v->Msgbuf, "MSG_BAD_HEADER_CRC16_TXT");
      return ZM_ERROR;
    }
  return v->Rxtype;
}       /*
         * End of short zrbhdr()
         */

/**********************************************************
 *      short zrbhdr32(struct Vars *v)
 *
 * Receive a binary style header (type and position) with
 * 32 bit FCS
 **********************************************************/
short
zrbhdr32(struct Vars *v)
{
  UBYTE    *hdr = v->Rxhdr;
  short     c,
            n;
  ULONG     crc32;

  if ((c = zdlread(v)) & ~0xFF)
    return c;
  v->Rxtype = c;
  crc32 = 0xFFFFFFFFL;
  crc32 = UPDC32(c, crc32);

  for (n = 4; --n >= 0;)
    {
      if ((c = zdlread(v)) & ~0xFF)
        return c;
      crc32 = UPDC32(c, crc32);
      *hdr++ = c;
    }
  for (n = 4; --n >= 0;)
    {
      if ((c = zdlread(v)) & ~0xFF)
        return c;
      crc32 = UPDC32(c, crc32);
    }
  if (crc32 != 0xDEBB20E3)
    {
      strcpy(v->Msgbuf, "MSG_BAD_HEADER_CRC32_TXT");
      return ZM_ERROR;
    }
  return v->Rxtype;
}       /*
         * End of short zrbhdr32()
         */

/**********************************************************
 *      short zrhhdr(struct Vars *v)
 *
 * Receive a hex style header (type and position)
 **********************************************************/
short
zrhhdr(struct Vars *v)
{
  UBYTE    *hdr = v->Rxhdr;
  short     c,
            n;
  USHORT    crc;

  if ((c = zgethex(v)) < 0)
    return c;
  v->Rxtype = c;
  crc = updcrc(c, 0);

  for (n = 4; --n >= 0;)
    {
      if ((c = zgethex(v)) < 0)
        return c;
      crc = updcrc(c, crc);
      *hdr++ = c;
    }
  if ((c = zgethex(v)) < 0)
    return c;
  crc = updcrc(c, crc);
  if ((c = zgethex(v)) < 0)
    return c;
  crc = updcrc(c, crc);
  if (crc & 0xFFFF)
    {
      strcpy(v->Msgbuf, "MSG_BAD_HEADER_CRC_TXT");
      return ZM_ERROR;
    }
  if (readock(v, 1) == '\r')
    readock(v, 1);      /*
                         * Throw away possible cr/lf
                         */
  return v->Rxtype;
}       /*
         * End of short zrhhdr()
         */

/**********************************************************
 *      void zputhex(struct Vars *v, UBYTE c)
 *
 * Send a byte as two hex digits
 **********************************************************/
void
zputhex(struct Vars *v, UBYTE c)
{
  static char digits[] = "0123456789abcdef";

  sendline(v, digits[(c >> 4) & 0x0F]);
  sendline(v, digits[c & 0x0F]);
}       /*
         * End of void zputhex()
         */

/**********************************************************
 *      void zsendline(struct Vars *v, UBYTE c)
 *
 * Send character c with ZMODEM escape sequence encoding.
 * Escape ZDLE, real DLE, XON, XOFF, and CR following @ (Telenet net escape)
 **********************************************************/
void
zsendline(struct Vars *v, UBYTE c)
{
  static char actions[256] =
  {
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 1, 3, 3,
    2, 2, 3, 2, 3, 3, 3, 3, 0, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 1, 3, 3,
    2, 2, 3, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
  };

  switch (actions[c])
    {
      case 0:
        xsendline(v, ZDLE);
        xsendline(v, v->Lastzsent = (c ^ 0100));
        break;

      case 1:
        if (!v->Zctlesc && (v->Lastzsent & 0177) != '@')
          goto sendit;

   /*
    * **** FALL THRU TO ****
    */

      case 2:
        xsendline(v, ZDLE);
        c ^= 0100;

      sendit:xsendline(v, v->Lastzsent = c);
        break;

      case 3:
        if (v->Zctlesc && !(c & 0140))
          {
            xsendline(v, ZDLE);
            c ^= 0100;
          }

        xsendline(v, v->Lastzsent = c);
        break;
    }
}       /*
         * End of void zsendline()
         */

/**********************************************************
 *      short zgethex(struct Vars *v)
 *
 * Decode two lower case hex digits into an 8 bit byte value
 **********************************************************/
short
zgethex(struct Vars *v)
{
  short     c,
            n;

  if ((n = noxrd7(v)) < 0)
    return n;
  n -= '0';
  if (n > 9)
    n -= ('a' - ':');
  if (n & ~0xF)
    return ZM_ERROR;

  if ((c = noxrd7(v)) < 0)
    return c;
  c -= '0';
  if (c > 9)
    c -= ('a' - ':');
  if (c & ~0xF)
    return ZM_ERROR;

  return (short) (n << 4 | c);
}       /*
         * End of short zgethex()
         */

/**********************************************************
 *      short zdlread(struct Vars *v)
 *
 * Read a byte, checking for ZMODEM escape encoding
 * including CAN*5 which represents a quick abort.
 **********************************************************/
short
zdlread(struct Vars *v)
{
  short     c;

again:
/*
 * Quick check for non control characters
 */
  if ((c = readock(v, v->Rxtimeout)) & 0140)
    return c;
  switch (c)
    {
      case ZDLE:
        break;
      case 023:
      case 0223:
      case 021:
      case 0221:
        goto again;
      default:
        if (v->Zctlesc && !(c & 0140))
          {
            goto again;
          }
        return c;
    }
again2:
  if ((c = readock(v, v->Rxtimeout)) < 0)
    return c;
  if (c == ZM_CAN && (c = readock(v, v->Rxtimeout)) < 0)
    return c;
  if (c == ZM_CAN && (c = readock(v, v->Rxtimeout)) < 0)
    return c;
  if (c == ZM_CAN && (c = readock(v, v->Rxtimeout)) < 0)
    return c;
  switch (c)
    {
      case ZM_CAN:
        return GOTCAN;
      case ZCRCE:
      case ZCRCG:
      case ZCRCQ:
      case ZCRCW:
        return ((short) (c | GOTOR));
      case ZRUB0:
        return 0177;
      case ZRUB1:
        return 0377;
      case 023:
      case 0223:
      case 021:
      case 0221:
        goto again2;
      default:
        if (v->Zctlesc && !(c & 0140))
          {
            goto again2;
          }
        if ((c & 0140) == 0100)
          return ((short) (c ^ 0100));
        break;
    }
  strcpy(v->Msgbuf, "MSG_BAD_ZMODEM_ESC_TXT");
  return ZM_ERROR;
}       /*
         * End of short zdlread()
         */

/**********************************************************
 *      short noxrd7(struct Vars *v)
 *
 * Read a character from the modem line with timeout.
 * Eat parity, XON and XOFF characters.
 **********************************************************/
short
noxrd7(struct Vars *v)
{
  short     c;

  for (;;)
    {
      if ((c = readock(v, v->Rxtimeout)) < 0)
        return c;
      switch (c &= 0x7F)
        {
          case ZM_XON:
          case ZM_XOFF:
            continue;
          default:
            if (v->Zctlesc && !(c & 0140))
              continue;
          case '\r':
          case '\n':
          case ZDLE:
            return c;
        }
    }
}       /*
         * short noxrd7()
         */

/**********************************************************
 *      void stohdr(struct Vars *v, long pos)
 *
 * Store long integer pos in Txhdr
 **********************************************************/
void
stohdr(struct Vars *v, long pos)
{
  v->Txhdr[ZP0] = pos;
  pos >>= 8;
  v->Txhdr[ZP1] = pos;
  pos >>= 8;
  v->Txhdr[ZP2] = pos;
  pos >>= 8;
  v->Txhdr[ZP3] = pos;
}       /*
         * End of void stohdr()
         */

/**********************************************************
 *      long rclhdr(struct Vars *v)
 *
 * Recover a long integer from a header
 **********************************************************/
long
rclhdr(struct Vars *v)
{
  long      l;

  l = v->Rxhdr[ZP3];
  l = (l << 8) | v->Rxhdr[ZP2];
  l = (l << 8) | v->Rxhdr[ZP1];
  l = (l << 8) | v->Rxhdr[ZP0];
  return l;
}       /*
         * End of long rclhdr()
         */

/**
   Find the end of the string.
**/
char     *
find_end(char *buf)
{
  while (*buf)
    buf++;
  return (buf);
}

/**
 Convert octal number to decimal.
**/
unsigned long
from_octal(char *buf)
{
  unsigned long value = 0;
  UBYTE     c;

  while (c = *buf)
    {
      if (c == ' ' || c == ',' || c == '\t' || c == '\r' || c == '\n')
        buf++;
      else
        break;
    }

  while (c = *buf++)
    {
      if (c >= '0' && c <= '7')
        value = (value << 3) + (c - '0');
      else
        break;
    }

  return (value);
}

/*
 * Skip all blanks.
 */
char     *
skip_chars(char *buf)
{
  UBYTE     c;

  while (c = *buf)
    {
      if (c == ' ' || c == ',' || c == '\t' || c == '\r' || c == '\n')
        {
          buf++;

          while (c = *buf)
            {
              if (c == ' ' || c == ',' || c == '\t' || c == '\r' || c == '\n')
                buf++;
              else
                return (buf);
            }

          return (buf);
        }
      else
        buf++;
    }

  return (buf);
}

/**********************************************************
 *      long XProtocolReceive(struct XPR_IO *xio)
 *
 * Main file reception routine; called by comm program
 **********************************************************/
long
XProtocolReceive(struct XPR_IO *xio)
{
  struct SetupVars *sv;
  struct Vars *v;
  UBYTE     err = FALSE;

/*
 * Perform common setup and initializations
 */
  if (!(v = setup(xio)))
    return XPRS_FAILURE;

/*
 * Remember data direction for cps rate calculation.
 */
  v->Receiving = TRUE;
  v->Tryzhdrtype = ZRINIT;
  v->Rxtimeout = 100;

  sv = (void *) v->io.xpr_data;
  if (sv->bufpos)
    {
      v->Modemchar = v->Modembuf;
      if (sv->buflen > sizeof(v->Modembuf))
        sv->buflen = sizeof(v->Modembuf);
      memcpy(v->Modembuf, sv->bufpos, sv->buflen);
      v->Modemcount = sv->buflen;
    }

/*
 * Transfer the files
 */
  if (rcvbatch(v) == ZM_ERROR)
    {
      upderr(v, "MSG_DOWNLOAD_CANCELLED_OR_TIMED_OUT");
      err = TRUE;
    }
  else
    updmsg(v, "MSG_DONE");

/*
 * Clean up and return
 * If an error occured, it's quite likely that there is * garbage in the
 * receive buffer we don't want the user * to see.
 */
  xpr_sflush ();

  FreeMem(v->Filebuf, v->Filebufmax);
  FreeMem(v, (long) sizeof(struct Vars));

  return (err) ? XPRS_FAILURE : XPRS_SUCCESS;
}       /*
         * End of long XProtocolReceive()
         */

/**********************************************************
 *      short rcvbatch(struct Vars *v)
 *
 * Start the batch transfer
 **********************************************************/
short
rcvbatch(struct Vars *v)
{
  switch (tryz(v))
    {
      case ZCOMPL:
        return OK;
      case ZFILE:
        if (rzfiles(v) == OK)
          return OK;
    }
  canit(v);
  return ZM_ERROR;
}       /*
         * End of short rcvbatch()
         */

/**********************************************************
 *      short tryz(struct Vars *v)
 *
 * Negotiate with sender to start a file transfer
 **********************************************************/
short
tryz(struct Vars *v)
{
  short     n,
            errors = 0;
  for (n = v->ErrorLimit; --n >= 0;)
    {
 /*
  * Set max frame length and capability flags
  */
      stohdr(v, (long) v->Tframlen);
      v->Txhdr[ZF0] = CANFC32 | CANFDX | CANOVIO;

      if (v->Zctlesc)
        v->Txhdr[ZF0] |= TESCCTL;

      zshhdr(v, v->Tryzhdrtype);
      sendbuf(v);

      if (v->Tryzhdrtype == ZSKIP)      /*
                                         * Don't skip too far.
                                         */
        v->Tryzhdrtype = ZRINIT;        /*
                                         * CAF 8-21-87
                                         */

    again:
 /*
  * Check for abort from comm program
  */

          if ( xpr_chkabort() )
            {
              zshhdr(v, ZABORT);
              sendbuf(v);
              return ZM_ERROR;
            }

      switch (zgethdr(v))
        {
          case ZFILE:   /*
                         * File name and info packet
                         */
            v->Zconv = v->Rxhdr[ZF0];   /*
                                         * Suggested txt mode; ZCNL = text,
                                         */
       /*
        * ZCBIN = binary, 0 = don't know.
        */
            v->Zmanag = v->Rxhdr[ZF1];  /*
                                         * Suggested file management mode.
                                         */
            v->Ztrans = v->Rxhdr[ZF2];  /*
                                         * Suggested file transport mode.
                                         */
            v->Tryzhdrtype = ZRINIT;

            if (zrdata(v, v->Pktbuf, v->ksize) == GOTCRCW)
              return ZFILE;
            zshhdr(v, ZNAK);    /*
                                 * Packet mangled, ask for retry
                                 */
            sendbuf(v);
            goto again;
          case ZSINIT:          /*
                                 * Special attention-grabbing string to use
                                 * to
                                 */
            v->Zctlesc = TESCCTL & v->Rxhdr[ZF0];
       /*
        * interrupt sender
        */
            if (zrdata(v, v->Attn, ZATTNLEN) == GOTCRCW)
              zshhdr(v, ZACK);
            else
              zshhdr(v, ZNAK);
            sendbuf(v);
            goto again;
          case ZFREECNT:        /*
                                 * Sender wants to know how much room we've
                                 * got
                                 */
            stohdr(v, getfree(v));
            zshhdr(v, ZACK);
            sendbuf(v);
            goto again;
          case ZCOMMAND:        /*
                                 * Sender wants us to do remote commands,
                                 */
       /*
        * but we don't do requests.
        */

            if (zrdata(v, v->Pktbuf, v->ksize) == GOTCRCW)
              {
                sprintf(v->Msgbuf, "MSG_IGNORING_COMMAND", v->Pktbuf);
                upderr(v, v->Msgbuf);   /*
                                         * Ignore and report all uploaded
                                         * commands
                                         */
                stohdr(v, 0L);  /*
                                 * whilst telling sender they worked;
                                 */
                do
                  {
                    zshhdr(v, ZCOMPL);  /*
                                         * paranoia can be good for you...
                                         */
                    sendbuf(v);
                  }
                while (++errors < v->ErrorLimit && zgethdr(v) != ZFIN);
                ackbibi(v);
                return ZCOMPL;
              }
            else
              zshhdr(v, ZNAK);
            sendbuf(v);
            goto again;
          case ZCOMPL:
            goto again;
          case ZFIN:    /*
                         * Sender has ended batch
                         */
            ackbibi(v);
            return ZCOMPL;
          case ZCAN:
          case RCDO:
            upderr(v, v->Msgbuf);
            return ZM_ERROR;
        }
    }
  return ZM_ERROR;
}       /*
         * End of short tryz()
         */

/**********************************************************
 *      short rzfiles(struct Vars *v)
 *
 * Receive a batch of files
 **********************************************************/
short
rzfiles(struct Vars *v)
{
  struct SetupVars *sv;
  short     c;

/*
 * Keep receiving files until end of batch or error
 */
  while (TRUE)
    {
      switch (c = rzfile(v))
        {
          case ZEOF:
          case ZSKIP:
            switch (tryz(v))
              {
                case ZCOMPL:
                  return OK;
                default:
                  return ZM_ERROR;
                case ZFILE:
                  break;
              }
            break;
          default:
            bfclose(v);
            sv = (void *) v->io.xpr_data;
            if (*sv->option_k == 'N' )
              {
                updmsg(v, "MSG_DELETING_PARTIALLY_RECEIVED_FILE");
                unlink(v->Filename);
              }
            else
              updmsg(v, "MSG_KEEPING_PARTIALLY_RECEIVED_FILE");
            return c;
        }
    }
}       /*
         * End of short rzfiles()
         */

/**********************************************************
 *      short rzfile(struct Vars *v)
 *
 * Receive one file; file name packet already read into
 * Pktbuf by tryz()
 **********************************************************/
short
rzfile(struct Vars *v)
{
  short     c,
            n;
  long      result = 0,
            abort = 0;

/*
 * Process file name packet; either open file and prepare to receive, * or
 * tell us to skip this one.
 */
  if (procheader(v) == ZM_ERROR)
    return v->Tryzhdrtype = ZSKIP;

  n = v->ErrorLimit;
  v->Rxbytes = v->Strtpos;
  v->Eofseen = FALSE;

/*
 * Receive ZDATA frames until finished
 */
  while (TRUE)
    {
      stohdr(v, v->Rxbytes);    /*
                                 * Tell sender where to start frame
                                 */
      zshhdr(v, ZRPOS);
      sendbuf(v);
    nxthdr:
 /*
  * Check for abort from comm program
  */
          if (abort)
            {
              result = abort;
              abort = 0;
            };

     /*
      * Cancel transmission.
      */
          if (result || xpr_chkabort() )
            {
              zshhdr(v, ZABORT);
              sendbuf(v);
            };

      switch (c = zgethdr(v))   /*
                                 * Wait for frame header
                                 */
        {
          default:
            return ZM_ERROR;
          case ZFIN:
            zshhdr(v, ZCOMPL);
            sendbuf(v);
            return ZFIN;
          case ZNAK:
          case TIMEOUT:
            if (--n < 0)
              return ZM_ERROR;
            v->xpru.xpru_updatemask = XPRU_ERRORMSG | XPRU_TIMEOUTS;
            sprintf(find_end(v->Msgbuf), "MSG_AT %ld RETRIES %ld LEFT",
                      v->Rxbytes, (long) n);
            v->xpru.xpru_errormsg = (char *) v->Msgbuf;
            ++v->xpru.xpru_timeouts;
            display_update(&v->xpru);
            continue;
          case ZFILE:   /*
                         * Sender didn't see our ZRPOS yet; try again
                         */

            zrdata(v, v->Pktbuf, v->ksize);     /*
                                                 * Read and discard
                                                 * redundant
                                                 */
            continue;   /*
                         * filename packet
                         */
          case ZEOF:    /*
                         * End of file data
                         */
            if (v->Rxpos != v->Rxbytes)         /*
                                                 * We aren't in sync; go
                                                 * back
                                                 */
              {
                sprintf(v->Msgbuf, "MSG_BAD_EOF:%ld %ld",
                          v->Rxbytes, v->Rxpos);
                upderr(v, v->Msgbuf);
                continue;
              }
            bfclose(v);         /*
                                 * All done; close file
                                 */
            updmsg(v, "MSG_EOF_RECEIVED");
            return c;
          case ZM_ERROR:   /*
                         * Too much garbage while waiting for frame header
                         */
            if (--n < 0)
              return ZM_ERROR;
            v->xpru.xpru_updatemask = XPRU_ERRORMSG | XPRU_ERRORS;
            sprintf(find_end(v->Msgbuf), "MSG_AT %ld RETRIES %ld LEFT",
                      v->Rxbytes, (long) n);
            v->xpru.xpru_errormsg = (char *) v->Msgbuf;
            ++v->xpru.xpru_errors;
            display_update(&v->xpru);
            zmputs(v, v->Attn);
            continue;
          case ZSKIP:
            v->Modtime = 1;
            bfclose(v);
            upderr(v, "MSG_SKIP_COMMAND_RECEIVED");
            return (c);
          case ZDATA:   /*
                         * More file data packets forthcoming
                         */
            if (v->Rxpos != v->Rxbytes)         /*
                                                 * We aren't in sync; go
                                                 * back
                                                 */
              {
                if (--n < 0)
                  return ZM_ERROR;
                v->xpru.xpru_updatemask = XPRU_ERRORMSG | XPRU_ERRORS;
                sprintf(v->Msgbuf, "MSG_DATA_AT %ld BAD_POSITION %ld",
                          v->Rxbytes, v->Rxpos);
                v->xpru.xpru_errormsg = (char *) v->Msgbuf;
                ++v->xpru.xpru_errors;
                display_update(&v->xpru);
                zmputs(v, v->Attn);
                continue;
              }
       /*
        * Receive file data packet(s)
        */
          moredata:
       /*
        * Check for abort from comm program
        */
                if( (abort = xpr_chkabort()) )
                  {
                    zshhdr(v, ZABORT);
                    sendbuf(v);
                    upderr(v, "MSG_TRANSFER_CANCELED");
                    return ZM_ERROR;
                  };
            switch (c = zrdata(v, v->Pktbuf, v->ksize))
              {
                case ZCAN:
                case RCDO:
                  upderr(v, "MSG_TRANSFER_CANCELED");
                  return ZM_ERROR;
                case ZM_ERROR:     /*
                                 * CRC error or packet too long
                                 */
                  if (--n < 0)
                    return ZM_ERROR;
                  v->xpru.xpru_updatemask = XPRU_ERRORMSG | XPRU_ERRORS;
                  sprintf(find_end(v->Msgbuf), "MSG_AT %ld RETRIES %ld LEFT",
                            v->Rxbytes, (long) n);
                  v->xpru.xpru_errormsg = (char *) v->Msgbuf;
                  ++v->xpru.xpru_errors;
                  display_update(&v->xpru);
                  zmputs(v, v->Attn);
                  continue;
                case TIMEOUT:
                  if (--n < 0)
                    return ZM_ERROR;
                  v->xpru.xpru_updatemask = XPRU_ERRORMSG | XPRU_TIMEOUTS;
                  sprintf(find_end(v->Msgbuf), "MSG_AT %ld RETRIES %ld LEFT",
                            v->Rxbytes, (long) n);
                  v->xpru.xpru_errormsg = (char *) v->Msgbuf;
                  ++v->xpru.xpru_timeouts;
                  display_update(&v->xpru);
                  continue;
                case GOTCRCW:   /*
                                 * Sender says it's waiting for an ACK
                                 */
                  n = v->ErrorLimit;
                  if (putsec(v) == ZM_ERROR)
                    return ZM_ERROR;
                  stohdr(v, v->Rxbytes);
                  zshhdr(v, ZACK);
                  sendbuf(v);
                  goto nxthdr;
                case GOTCRCQ:   /*
                                 * Sender says it's not waiting,
                                 */
             /*
              * but ACK anyway (rarely used)
              */
                  n = v->ErrorLimit;
                  if (putsec(v) == ZM_ERROR)
                    return ZM_ERROR;
                  stohdr(v, v->Rxbytes);
                  zshhdr(v, ZACK);
                  sendbuf(v);
                  goto moredata;
                case GOTCRCG:   /*
                                 * Sender says keep receiving, there's more
                                 * coming
                                 */
                  n = v->ErrorLimit;
                  if (putsec(v) == ZM_ERROR)
                    return ZM_ERROR;
                  goto moredata;
                case GOTCRCE:   /*
                                 * Sender says this is the last packet
                                 */
                  n = v->ErrorLimit;
                  if (putsec(v) == ZM_ERROR)
                    return ZM_ERROR;
                  goto nxthdr;
              }
        }
    }
}       /*
         * End of short rzfile()
         */

/************************ZM_**********************************
 *      short procheader(struct Vars *v)
 *
 * Process file name & info packet; either open file and
 * prepare to receive, or return ZM_ERROR if we should skip
 * this one for some reason
 **********************************************************/
short
procheader(struct Vars *v)
{
  struct SetupVars *sv;
  UBYTE    *p,
           *openmode,
            buff[PATHLEN];
  long      n;

  openmode = "w";
  v->Strtpos = 0;

/*
 * Extract expected filesize from file info packet, if given
 */
  v->Fsize = -1;
  p = find_end(v->Pktbuf) + 1;

  if (*p)
    {
      char     *index,
               *ptr;

      index = skip_chars(ptr = p);

      v->Fsize = atol(ptr);

      index = skip_chars(ptr = index);

      v->Modtime = from_octal(ptr);

      skip_chars(ptr = index);

      v->Bits = from_octal(ptr);

      v->SetAttributes = TRUE;
    }
  else
    {
      v->Modtime = 0;
      v->Bits = 0;
      v->SetAttributes = FALSE;
    }

/*
 * If option RY set, use full received file path
 */
  sv = (void *) v->io.xpr_data;
  if (*sv->option_r == 'Y')
    strcpy(v->Filename, v->Pktbuf);
  else
    {
 /*
  * else use the default directory path specified in the setup options
  */
      strcpy(v->Filename, sv->option_p);
      p = v->Filename + strlen(v->Filename) - 1;
      if (p >= v->Filename && *p != '/' && *p != ':')
        *++p = '/';
      *++p = '\0';
 /*
  * Append the filename from the file info packet; ignore anything before *
  * last /, \, or : in filename (received directory path is ignored)
  */
      p = find_end(v->Pktbuf);  /*
                                 * start at end and scan back
                                 */
 /*
  * to start of name
  */
      while (p >= v->Pktbuf && *p != '/' && *p != '\\' && *p != ':')
        --p;
      strcat(v->Filename, ++p);
    }

/*
 * Make sure we have room for file; skip it if not.
 */

  if (v->Fsize > getfree(v))
    {
      sprintf(v->Msgbuf, "MSG_INSUFFICIENT_DISK_SPACE Size:%ld Avail:%ld",
                v->Fsize, getfree(v));
      upderr(v, v->Msgbuf);
      v->Noroom = TRUE;
      return ZM_ERROR;
    }

/*
 * Display name of file being received for user
 */
  v->xpru.xpru_updatemask = XPRU_FILENAME;
  v->xpru.xpru_filename = (char *) v->Filename;
  display_update(&v->xpru);

/*
 * If a file with this name already exists, handle in * accordance with O
 * option
 */
  if (exist(v))
    {
      switch (*sv->option_o)
        {
          case 'N':     /*
                         * Don't overwrite; change name to prevent collision
                         */
            strcpy(buff, v->Filename);
            strcat(v->Filename, ".dup");
            n = 2;
            while (exist(v))
              {
                sprintf(v->Filename, "%s.dup%ld", buff, n);
                ++n;
              }
       /*
        * Update filename display to show new name
        */
            display_update(&v->xpru);
            break;
          case 'R':     /*
                         * Resume transfer from current end of file
                         */
            openmode = "a";
            v->Strtpos = xpr_finfo(v->Filename, 1L);
            break;
          case 'S':     /*
                         * Skip it
                         */
            upderr(v, "MSG_FILE_ALREADY_EXISTS");
            return ZM_ERROR;
       /*
        * Else 'Y', go ahead and overwrite it (openmode = w)
        */
        }
    }

/*
 * Set text/binary mode according to options before opening file
 */
  set_textmode(v);

/*
 * Figure out file translation mode to use; either binary (verbatim *
 * transfer) or ASCII (perform end-of-line conversions).  If user has *
 * specified a mode (TY or TN), that's what we use.  If user says use *
 * sender's suggestion (T?), set mode according to Zconv flag.  If neither *
 * side specifies, default to binary mode.
 */
  v->Thisbinary = v->Rxbinary || !v->Rxascii;
  if (!v->Rxbinary && v->Zconv == ZCNL)
    v->Thisbinary = FALSE;
  if (!v->Rxascii && v->Zconv == ZCBIN)
    v->Thisbinary = TRUE;

/*
 * Open the file (finally)
 */
  if (!(v->File = bfopen(v, openmode)))
    {
      ++v->Errcnt;
      upderr(v, "MSG_CANT_OPEN_FILE");
      return ZM_ERROR;
    }
  GetSysTime(&v->Starttime);
  v->BytesReceived = 0;

/*
 * Initialize comm program transfer status display
 */
  v->xpru.xpru_updatemask = XPRU_PROTOCOL | XPRU_FILESIZE | XPRU_MSG
    | XPRU_BLOCKS | XPRU_ERRORS | XPRU_TIMEOUTS | XPRU_BLOCKCHECK
    | XPRU_BYTES | XPRU_ELAPSEDTIME;
  v->xpru.xpru_protocol = "ZModem";
  v->xpru.xpru_filesize = v->Fsize;
  v->xpru.xpru_msg = (v->Thisbinary) ? "MSG_RECEIVING_BINARY_FILE"
    : "MSG_RECEIVING_TEXT_FILE";
  v->xpru.xpru_blocks = v->xpru.xpru_errors = v->xpru.xpru_timeouts = 0;
  v->xpru.xpru_blockcheck = v->Crc32 ? "CRC-32" : "CRC-16";
  v->xpru.xpru_bytes = v->Strtpos;
  v->xpru.xpru_blocksize = v->ksize;
  update_rate(v);
  display_update(&v->xpru);

  return OK;
}       /*
         * End of short procheader()
         */

/**********************************************************
 *      short putsec(struct Vars *v)
 *
 * Writes the received file data to the output file.
 * If in ASCII mode, stops writing at first ^Z, and converts all
 * \r\n pairs or solo \r's to \n's.
 **********************************************************/
short
putsec(struct Vars *v)
{
  char      nl = '\n';
  UBYTE    *p;
  short     n;

/*
 * If in binary mode, write it out verbatim
 */
  if (v->Thisbinary)
    {
      if (bfwrite(v, v->Pktbuf, (long) v->Rxcount) != v->Rxcount)
        goto diskfull;
 /*
  * If in text mode, perform end-of-line cleanup
  */
    }
  else
    {
      if (v->Eofseen)
        return OK;
      for (p = v->Pktbuf, n = v->Rxcount; --n >= 0; ++p)
        {
          if (*p == ZM_CPMEOF)
            {
              v->Eofseen = TRUE;
              return OK;
            }
          else if (*p != '\n' && v->Lastsent == '\r')
            {
              if (bfwrite(v, &nl, 1L) != 1)
                goto diskfull;
            }
          if (*p != '\r' && bfwrite(v, p, 1L) != 1)
            goto diskfull;
          v->Lastsent = *p;
        }
    }

/*
 * Update comm program status display
 */
  v->xpru.xpru_updatemask = XPRU_BLOCKS | XPRU_BLOCKSIZE | XPRU_BYTES
    | XPRU_ELAPSEDTIME | XPRU_BLOCKCHECK;
  ++v->xpru.xpru_blocks;
  v->xpru.xpru_blocksize = v->Rxcount;
  v->xpru.xpru_bytes = v->Rxbytes += v->Rxcount;
  v->xpru.xpru_blockcheck = v->Crc32 ? "CRC-32" : "CRC-16";
  update_rate(v);
  display_update(&v->xpru);

  return OK;

diskfull:
  upderr(v, "MSG_ERROR_WRITING_FILE");
  v->Noroom = TRUE;
  return ZM_ERROR;
}       /*
         * End of short putsec()
         */

/**********************************************************
 *      void ackbibi(struct Vars *v)
 *
 * End of batch transmission; disengage cleanly from sender
 **********************************************************/
void
ackbibi(struct Vars *v)
{
  short     n;

  stohdr(v, 0L);
  for (n = 4; --n;)
    {
      zshhdr(v, ZFIN);
      sendbuf(v);
      switch (readock(v, 100))
        {
          case 'O':
            readock(v, 1);      /*
                                 * Discard 2nd 'O'
                                 */
          case TIMEOUT:
          case RCDO:
            return;
        }
    }
}       /*
         * End of void ackbibi()
         */
/*
 * End of Receive.c source
 */
/**********************************************************************
 * Send.c: File transmission routines for xprzmodem.library;
 * Original Version 2.10, 12 February 1991, by Rick Huebner.
 * Based closely on Chuck Forsberg's sz.c example ZModem code,
 * but too pervasively modified to even think of detailing the changes.
 * Released to the Public Domain; do as you like with this code.
 *
 * Version 2.50, 15 November 1991, CRC-32 additions by William M. Perkins.
 * Version 2.51 29, January 1992, RX_timout fix by John Tillema
 * Version 2.52   6 March 1992, Very minor fix with compiled 020 library
 *               by William M. Perkins.
 * Version 2.53, 28 June 1993, several additions by Olaf `Olsen' Barthel
 **********************************************************************/

/*
 * Convert decimal to octal value.
 */
VOID
to_octal(STRPTR buf, ULONG value)
{
  UBYTE     buffer[20];
  short     i = 1;

  while (value)
    {
      buffer[i++] = '0' + (value & 7);

      value >>= 3;
    }

  do
    *buf++ = buffer[--i];
  while (i);

  *buf = 0;
}

/**********************************************************
 *      long XProtocolSend(struct XPR_IO *xio)
 *
 * Main file transmission routine; called by comm program
 **********************************************************/
long
XProtocolSend(struct XPR_IO *xio)
{
  struct Vars *v;
  short     err;

/*
 * Perform common setup and initializations
 */
  if (!(v = setup(xio)))
    return XPRS_FAILURE;

/*
 * was 600, set to 300 to fix so it uploads correctly
 */
  v->Rxtimeout = 300;
  v->Wantfcs32 = TRUE;
  v->Rxflags = 0;
  v->Receiving = FALSE;

/*
 * Transfer the files
 */
  zmputs(v, "rz\r");
  stohdr(v, 0L);
  zshhdr(v, ZRQINIT);
  sendbuf(v);
  if (getzrxinit(v) == ZM_ERROR)
    upderr(v, "MSG_UPLOAD_CANCELED_TXT");
  else
    sendbatch(v);

/*
 * Clean up and return
 */
  if (err = v->Errcnt)
    upderr(v, "MSG_FILES_SKIPPED_DUE_TO_ERRORS_TXT");
  else
    updmsg(v, "MSG_DONE_TXT");
  FreeMem(v->Filebuf, v->Filebufmax);
  FreeMem(v, (long) sizeof(struct Vars));

  return (err) ? XPRS_FAILURE : XPRS_SUCCESS;
}       /*
         * End of long XProtocolSend()
         */

/**********************************************************
 *      short getzrxinit(struct Vars *v)
 *
 * Negotiate with receiver to start a file transfer
 **********************************************************/
short
getzrxinit(struct Vars *v)
{
  short     n;

  for (n = v->ErrorLimit; --n >= 0;)
    {
 /*
  * Check for abort from comm program
  */
      if ( xpr_chkabort() )
        {
          canit(v);     /*
                         * Receiver does not respond to ZABORT.
                         */
          return ZM_ERROR;
        }
      switch (zgethdr(v))
        {
          case ZCHALLENGE:      /*
                                 * Echo receiver's challenge number
                                 */
            stohdr(v, v->Rxpos);
            zshhdr(v, ZACK);
            sendbuf(v);
            continue;
          case ZCOMMAND:        /*
                                 * They didn't see our ZRQINIT; try again
                                 */
            stohdr(v, 0L);
            zshhdr(v, ZRQINIT);
            sendbuf(v);
            continue;
          case ZRINIT:          /*
                                 * Receiver ready; get transfer parameters
                                 */
            v->Rxflags = 0xFF & v->Rxhdr[ZF0];
            v->Txfcs32 = (v->Wantfcs32 && (v->Rxflags & CANFC32));
            v->Zctlesc |= v->Rxflags & TESCCTL;
            v->Rxbuflen = ((USHORT) v->Rxhdr[ZP1] << 8) | v->Rxhdr[ZP0];
       /*
        * Use shortest of the two side's max frame lengths
        */
            if (v->Tframlen && (!v->Rxbuflen || v->Tframlen < v->Rxbuflen))
              v->Rxbuflen = v->Tframlen;
            return (sendzsinit(v));
          case ZCAN:
          case RCDO:
          case TIMEOUT:
            upderr(v, v->Msgbuf);
            return ZM_ERROR;
          case ZRQINIT:
            if (v->Rxhdr[ZF0] == ZCOMMAND)
              continue;
       /*
        * fallthrough...
        */
          default:
            zshhdr(v, ZNAK);
            sendbuf(v);
            continue;
        }
    }
  return ZM_ERROR;
}       /*
         * End of short getzrxinit()
         */

/*
 * Send send-init information
 */
short
sendzsinit(struct Vars *v)
{
  if (v->Attn[0] == '\0' && (!v->Zctlesc || (v->Rxflags & TESCCTL)))
    return OK;
  else
    {
      short     c,
                errors = 0;

      for (;;)
        {
          stohdr(v, 0L);
          if (v->Zctlesc)
            {
              v->Txhdr[ZF0] |= TESCCTL;
              zshhdr(v, ZSINIT);
            }
          else
            zsbhdr(v, ZSINIT);

          v->Outbuf[0] = 0;
          zsdata(v, 1, ZCRCW);

          c = zgethdr(v);
          switch (c)
            {
              case ZCAN:
                return ZM_ERROR;
              case ZACK:
                return OK;
              default:
                if (++errors > v->ErrorLimit)
                  return ZM_ERROR;
                continue;
            }
        }
    }
}       /*
         * End of short sendzsinit()
         */

/**********************************************************
 *      void sendbatch(struct Vars *v)
 *
 * Send a batch of files
 **********************************************************/
void
sendbatch(struct Vars *v)
{
  UBYTE     single,
            done = FALSE;
  long      fstate;

      single = FALSE;
      fstate = xpr_ffirst(v->Filename, v->io.xpr_filename);
      if (!fstate)
        {
          upderr(v, "MSG_NO_FILES_MATCH_TEMPLATE_TXT");
          return;
        }

/*
 * If using templates, keep getting names & sending until done
 */
  while (!done)
    {
      if (sendone(v) == ZM_ERROR)
        return;
      if (single)
        break;
      fstate = xpr_fnext(fstate, v->Filename, v->io.xpr_filename);
      done = !fstate;
    }

/*
 * End batch and return; if we never got started, just cancel receiver
 */
  if (v->Filcnt)
    saybibi(v);
  else
    canit(v);
}       /*
         * End of void sendbatch()
         */

/**********************************************************
 *      short sendone(struct Vars *v)
 *
 * Send the file named in v->Filename
 **********************************************************/
short
sendone(struct Vars *v)
{
  struct SetupVars *sv;

/*
 * Display name of file being sent for user
 */
  v->xpru.xpru_updatemask = XPRU_FILENAME;
  v->xpru.xpru_filename = v->Filename;
  display_update(&v->xpru);

/*
 * Set text/binary mode according to options before opening file
 */
  set_textmode(v);

/*
 * Open the file, if possible
 */
  if (!(v->File = bfopen(v, "r")))
    {
      ++v->Errcnt;
      upderr(v, "MSG_CANT_OPEN_FILE_TXT");
      return OK;        /*
                         * pass over it, there may be others
                         */
    }
  ++v->Filcnt;
  GetSysTime(&v->Starttime);
  v->BytesSent = 0;

/*
 * Kick off the file transfer
 */
  sv = (void *) v->io.xpr_data;
  switch (sendname(v))
    {
      case ZM_ERROR:
        ++v->Errcnt;
        return ZM_ERROR;
      case OK:
        bfclose(v);
   /*
    * File sent; if option DY, delete file after sending
    */
        if (*sv->option_d == 'Y' )
          {
            updmsg(v, "MSG_DELETING_FILE_AFTER_SEND_TXT");
            unlink(v->Filename);
          }
        break;
    }
  return OK;
}       /*
         * End of short sendone()
         */

/**********************************************************
 *      short sendname(struct Vars *v)
 *
 * Build file info block consisting of file name, length,
 * time, and mode
 **********************************************************/
short
sendname(struct Vars *v)
{
  struct SetupVars *sv;
  UBYTE    *p,
           *q,
            buff[32],
            mode[5];
  BPTR      FileLock;

/*
 * Initialize comm program transfer status display
 */
  v->Fsize = xpr_finfo(v->Filename, 1L);
  v->xpru.xpru_updatemask = XPRU_PROTOCOL | XPRU_FILESIZE | XPRU_MSG
    | XPRU_BLOCKS | XPRU_ERRORS | XPRU_TIMEOUTS | XPRU_BLOCKCHECK
    | XPRU_BYTES | XPRU_ELAPSEDTIME;
  v->xpru.xpru_protocol = "ZModem";
  v->xpru.xpru_filesize = v->Fsize;
  v->xpru.xpru_msg = (v->Lzconv == ZCNL) ? "MSG_SENDING_TEXT_FILE_TXT" :
    ((v->Lzconv == ZCBIN) ? "MSG_SENDING_BINARY_FILE_TXT" : "MSG_SENDING_FILE_TXT");
  v->xpru.xpru_blocks = v->xpru.xpru_errors = v->xpru.xpru_timeouts = 0;
  v->xpru.xpru_blockcheck = v->Crc32t ? "CRC-32" : "CRC-16";
  v->xpru.xpru_bytes = v->Strtpos = 0;
  v->xpru.xpru_blocksize = v->ksize;
  update_rate(v);
  display_update(&v->xpru);

  sv = (void *) v->io.xpr_data;
  if (*sv->option_s == 'Y')
    {
 /*
  * If "SY" option selected, send full path
  */
      strcpy(v->Pktbuf, v->Filename);
      p = v->Pktbuf + strlen(v->Pktbuf) + 1;
    }
  else
    {
 /*
  * else extract outgoing file name without directory path
  */
      for (p = v->Filename, q = v->Pktbuf; *p; ++p, ++q)
        if ((*q = *p) == '/' || *q == ':')
          q = v->Pktbuf - 1;
      *q = '\0';
      p = ++q;
    }

/*
 * Zero out remainder of file info packet
 */
  memset(p, 0, sizeof(v->Pktbuf) - (p - v->Pktbuf));

/*
 * Store file size, timestamp, and mode in info packet
 */
  if (v->FileAttributes & 1)
    {
      ULONG     Seconds = getsystime(NULL),
                Mode = 0000;      /*

                                   * owner = rwx, group = rwx, others = rwx
                                   */

 /*
  * See if we can lock it, this probably won't work for `term'.
  */
      if (FileLock = Lock(v->Filename, ACCESS_READ))
        {
          struct FileInfoBlock __aligned FileInfo;

     /*
      * Any info available?
      */
          if (Examine(FileLock, &FileInfo))
            {
         /*
          * Modification date.
          */
              Seconds = (  FileInfo.fib_Date.ds_Days * 24 * 60
                        +  FileInfo.fib_Date.ds_Minute) * 60
                        + (FileInfo.fib_Date.ds_Tick) / TICKS_PER_SECOND;

         /*
          * Take care of the owner bits.
          */
              if (FileInfo.fib_Protection & FIBF_EXECUTE)
                Mode |= 0100;

              if (FileInfo.fib_Protection & (FIBF_WRITE | FIBF_DELETE))
                Mode |= 0200;

              if (FileInfo.fib_Protection & FIBF_READ)
                Mode |= 0400;

         /*
          * Take care of the group bits.
          */
              if (!(FileInfo.fib_Protection & FIBF_GRP_EXECUTE))
                Mode |= 0010;

              if (!(FileInfo.fib_Protection & FIBF_GRP_WRITE) || !(FileInfo.fib_Protection & FIBF_GRP_DELETE))
                Mode |= 0020;

              if (!(FileInfo.fib_Protection & FIBF_GRP_READ))
                Mode |= 0040;

         /*
          * Take care of the other bits.
          */
              if (!(FileInfo.fib_Protection & FIBF_OTR_EXECUTE))
                Mode |= 0001;

              if (!(FileInfo.fib_Protection & FIBF_OTR_WRITE) || !(FileInfo.fib_Protection & FIBF_OTR_DELETE))
                Mode |= 0002;

              if (!(FileInfo.fib_Protection & FIBF_OTR_READ))
                Mode |= 0004;
            }

          UnLock(FileLock);
        }

      to_octal(buff, Seconds);
      to_octal(mode, Mode);
    }
  else
    {
      to_octal(buff, getsystime(NULL));
      to_octal(mode, 0000);
    }

/*
 * amiga.lib mysprintf() can't do %lo format, so we do it the hard way
 */
/*
 * Yes, octal; ZModem was originally done on Unix, and they like octal there
 */

  sprintf(p, "%ld %s %s", (v->Fsize < 0) ? 0L : v->Fsize, buff, mode);

/*
 * Send filename packet
 */
  return zsendfile(v, (short) (p - v->Pktbuf + strlen(p) + 1));
}       /*
         * End of short sendname()
         */

/**********************************************************
 *      short zsendfile(struct Vars *v, short blen)
 *
 * Send the filename packet and see if receiver will accept
 * file
 **********************************************************/
short
zsendfile(struct Vars *v, short blen)
{
  short     c;

  while (TRUE)
    {
      v->Txhdr[ZF0] = v->Lzconv;        /*
                                         * Text or Binary mode; from config
                                         * string
                                         */
      v->Txhdr[ZF1] = LZMANAG;  /*
                                 * Default file management mode
                                 */
      v->Txhdr[ZF2] = LZTRANS;  /*
                                 * Default file transport mode
                                 */
      v->Txhdr[ZF3] = 0;
      zsbhdr(v, ZFILE);
      zsdata(v, blen, ZCRCW);
      sendbuf(v);
    again:
 /*
  * Check for abort from comm program
  */
      if ( xpr_chkabort() )
        {
          bfclose(v);
          canit(v);     /*
                         * Receiver does not respond to ZABORT.
                         */
          return ZM_ERROR;
        };
      switch (c = zgethdr(v))
        {
          case ZRINIT:
            goto again;
          case ZCAN:
          case ZCRC:
          case RCDO:
          case TIMEOUT:
          case ZABORT:
          case ZFIN:
            upderr(v, v->Msgbuf);
            return ZM_ERROR;
          case ZSKIP:   /*
                         * Receiver doesn't want this one
                         */
            upderr(v, "MSG_SKIP_COMMAND_RECEIVED_TXT");
            bfclose(v);
            return c;
          case ZRPOS:   /*
                         * Receiver wants it; this is starting position
                         */
            bfseek(v, v->Rxpos);
            v->Strtpos = v->Txpos = v->Rxpos;
            xpr_sflush();
            v->Modemcount = 0;
            return zsendfdata(v);
        }
    }
}       /*
         * End of short zsendfile()
         */

/**********************************************************
 *      short zsendfdata(struct Vars *v)
 *
 * Send the file data
 **********************************************************/
short
zsendfdata(struct Vars *v)
{
  short     c,
            e,
            blklen,
            goodbytes = 0;
  USHORT    framelen,
            maxblklen,
            goodneeded = 512;

/*
 * Figure out max data packet size to send
 */
  maxblklen = v->ksize;
  if (v->Rxbuflen && maxblklen > v->Rxbuflen)
    maxblklen = v->Rxbuflen;
  blklen = (v->Baud < 1200) ? 256 : v->ksize;
  if (blklen > maxblklen)
    blklen = maxblklen;

/*
 * If an interruption happened, handle it; else keep sending data
 */
somemore:
  while (char_avail(v))
    {
 /*
  * Check for another incoming packet while discarding line noise
  */
      switch (readock(v, 1))
        {
          case ZM_CAN:
          case RCDO:
          case ZPAD:
            break;
          default:
            continue;
        }
    waitack:
      switch (c = getinsync(v))
        {
          default:
            upderr(v, "MSG_TRANSFER_CANCELED_TXT");
            bfclose(v);
            return ZM_ERROR;
          case ZSKIP:   /*
                         * Receiver changed its mind and wants to skip the
                         * file
                         */
            bfclose(v);
            return c;
          case ZACK:    /*
                         * ACK at end of frame; resume sending data
                         */
            break;
          case ZRPOS:   /*
                         * An error; resend data from last good point
                         */
            blklen >>= 2;
            if (blklen < MINBLOCK)
              blklen = MINBLOCK;
            if (goodneeded < MAXGOODNEEDED)
              goodneeded <<= 1;
            v->xpru.xpru_updatemask = XPRU_ERRORS;
            ++v->xpru.xpru_errors;
            display_update (&v->xpru);
            break;
          case ZRINIT:
            updmsg(v, "MSG_DONE_TXT");
            return OK;
        }
    }

/*
 * Transmit ZDATA frame header
 */
  framelen = v->Rxbuflen;
  stohdr(v, v->Txpos);
  zsbhdr(v, ZDATA);

/*
 * Keep sending data packets until finished or interrupted
 */
  do
    {
 /*
  * Read next chunk of file data
  */
      c = bfread(v, v->Pktbuf, (long) blklen);

 /*
  * Figure out how to handle this data packet
  */
      if (c < blklen)
        e = ZCRCE;      /*
                         * If end of file, this is last data packet
                         */
      else if (v->Rxbuflen && (framelen -= c) <= 0)
        e = ZCRCW;      /*
                         * If end of frame, ask for ACK
                         */
      else
        e = ZCRCG;      /*
                         * Else tell receiver to expect more data packets
                         */

      zsdata(v, c, e);  /*
                         * Send the packet
                         */
      sendbuf(v);

 /*
  * Update comm program status display
  */
      v->xpru.xpru_updatemask = XPRU_BLOCKS | XPRU_BLOCKSIZE | XPRU_BYTES
        | XPRU_ELAPSEDTIME | XPRU_BLOCKCHECK;
      ++v->xpru.xpru_blocks;
      v->xpru.xpru_blocksize = c;
      v->xpru.xpru_blockcheck = v->Crc32t ? "CRC-32" : "CRC-16";
      v->xpru.xpru_bytes = v->Txpos += c;
      update_rate(v);
      display_update(&v->xpru);

 /*
  * If we've been sending smaller than normal packets, see if it's * time to
  * bump the packet size up a notch yet
  */
      if (blklen < maxblklen && (goodbytes += c) >= goodneeded)
        {
          blklen <<= 1;
          if (blklen > maxblklen)
            blklen = maxblklen;
          goodbytes = 0;
        }
 /*
  * Check for abort from comm program
  */
          if( xpr_chkabort() )
            {
              canit(v);         /*
                                 * Receiver does not respond to ZABORT.
                                 */
              upderr(v, "MSG_TRANSFER_CANCELED_TXT");
              bfclose(v);
              return ZM_ERROR;
            }
 /*
  * If this was last packet in frame, go wait for ACK from receiver
  */
      if (e == ZCRCW)
        goto waitack;

 /*
  * * Check if receiver trying to interrupt us; look for incoming packet *
  * while discarding line noise
  */
      while (char_avail(v))
        {
          switch (readock(v, 1))
            {
              case ZM_CAN:
              case RCDO:
              case ZPAD:
           /*
            * Interruption detected; stop sending and process complaint
            */
                zsdata(v, 0, ZCRCE);
                sendbuf(v);
                goto waitack;
            }
        }
    }
  while (e == ZCRCG);   /*
                         * If no interruption, keep sending data packets
                         */

/*
 * Done sending file data; send EOF and wait for receiver to acknowledge
 */
  while (TRUE)
    {
      updmsg(v, "MSG_SENDING_EOF_TXT");
      stohdr(v, v->Txpos);
      zsbhdr(v, ZEOF);
      sendbuf(v);
      switch (c = getinsync(v))
        {
          case ZACK:
            continue;
          case ZRPOS:
            goto somemore;
          case ZRINIT:
            updmsg(v, "MSG_EOF_ACKNOWLEDGED_TXT");
            update_rate(v);
            v->xpru.xpru_updatemask = XPRU_ELAPSEDTIME;
            display_update(&v->xpru);
            return OK;
          case ZSKIP:
            bfclose(v);
            return c;
          default:
            upderr(v, "MSG_TRANSFER_CANCELED_TXT");
            bfclose(v);
            return ZM_ERROR;
        }
    }
}       /*
         * End of short zsendfdata()
         */

/**********************************************************
 *      short getinsync(struct Vars *v)
 *
 * Respond to receiver's complaint, get back in sync with
 * receiver
 **********************************************************/
short
getinsync(struct Vars *v)
{
  short     c;

  while (TRUE)
    {
      c = zgethdr(v);
      xpr_sflush();
      v->Modemcount = 0;
      switch (c)
        {
          case ZCAN:
          case ZABORT:
          case ZFIN:
          case RCDO:
          case TIMEOUT:
            upderr(v, v->Msgbuf);
            return ZM_ERROR;
          case ZRPOS:
            bfseek(v, v->Rxpos);
            v->Txpos = v->Rxpos;
            sprintf(v->Msgbuf, "MSG_RESENDING_FROM %ld", v->Txpos);
            upderr(v, v->Msgbuf);
            return c;
          case ZSKIP:
            upderr(v, "MSG_SKIP_COMMAND_RECEIVED");
       /*
        * fallthrough...
        */
          case ZRINIT:
            bfclose(v);
       /*
        * fallthrough...
        */
          case ZACK:
            return c;
          default:
            zsbhdr(v, ZNAK);
            sendbuf(v);
            continue;
        }
    }
}       /*
         * End of short getinsync()
         */

/**********************************************************
 *      void saybibi(struct Vars *v)
 *
 * End of batch transmission; disengage cleanly from receiver
 **********************************************************/
void
saybibi(struct Vars *v)
{
  while (TRUE)
    {
      stohdr(v, 0L);
      zsbhdr(v, ZFIN);
      sendbuf(v);
      switch (zgethdr(v))
        {
          case ZFIN:
            sendline(v, 'O');
            sendline(v, 'O');
            sendbuf(v);
       /*
        * fallthrough...
        */
          case ZCAN:
          case RCDO:
          case TIMEOUT:
            return;
        }
    }
}       /*
         * End of void saybibi()
         */
/**********************************************************************
 * Utils.c: Miscellaneous support routines for xprzmodem.library;
 * Version 2.10, 12 February 1991, by Rick Huebner.
 * Released to the Public Domain; do as you like with this code.
 *
 * Version 2.50, 15 November 1991, by William M. Perkins.  Added code
 * to update_rate() function in utils.c to avoid the Guru # 80000005
 * you would have gotten if you had decided to adjust the system clock
 * back during an upload or download.
 *
 * Mysprintf() function to replace sprintf() and proto code to use
 * libinit.o and linent.o library code was supplied by Jim Cooper of SAS.
 *
 * Version 2.54, 28 June 1993, by Olaf `Olsen' Barthel. Got rid of the
 * timer.c code, added OS 2.0 dependencies, rewrote update_rate() to
 * display only the information which was really available, added
 * control-code escaping & run-length encoding, changed the abort
 * routine to pay attention to the different abort levels, removed
 * SAS/C library dependencies (octal conversion, etc.), added
 * support for proper file creation date and access permission bits.
 **********************************************************************/

/*
 * Transfer options to use if XProtocolSetup not called
 */
struct SetupVars Default_Config =
{
  NULL, NULL, 0,
  {"C"},
  {"N"},
  {"16"},
  {"0"},
  {"10"},
  {"N"},
  {"N"},
  {"N"},
  {"Y"},
  {"N"},
  {"Y"},
  {""},
  {"1024"},
  {"S"}
};

/*
 * Convert a string to all upper case characters.
 */
VOID
string_upper(STRPTR buf)
{
  UBYTE     c;

  while (c = *buf)
    *buf++ = ToUpper(c);
}

/*
 * Skip blanks in a string.
 */
STRPTR
skip_blank(STRPTR buf)
{
  UBYTE     c;

  while (c = *buf)
    {
      if (c == ' ' || c == ',' || c == '\t' || c == '\r' || c == '\n')
        buf++;
      else
        break;
    }

  return (buf);
}


/*
 * Get the local time, convert it to GMT.
 */
ULONG
getsystime(struct timeval *tv)
{
  struct timeval Now;

  GetSysTime(&Now);
  if (tv)
    {
      tv->tv_secs = Now.tv_secs;
      tv->tv_micro = 0;
    }

  return (Now.tv_secs);
}

/**********************************************************
 *      long XProtocolCleanup(struct XPR_IO *xio)
 *
 * Called by comm program to give us a chance to clean
 * up before program ends
 **********************************************************/
long
XProtocolCleanup(struct XPR_IO *xio)
{
/*
 * Release option memory, if any
 */
  if (xio->xpr_data)
    {
      FreeMem(xio->xpr_data, (long) sizeof(struct SetupVars));

      xio->xpr_data = NULL;
    }

  return XPRS_SUCCESS;
}       /*
         * End of long XProtocolCleanup()
         */
/**********************************************************
 *      struct Vars *setup(struct XPR_IO *io)
 *
 * Perform setup and initializations common to both Send
 * and Receive routines
 **********************************************************/
struct Vars *
setup(struct XPR_IO *io)
{
  extern long BaudTable[];
  struct SetupVars *sv;
  struct Vars *v;
  long      origbuf;
/*
 * Hook in default transfer options if XProtocolSetup wasn't called
 */
  if (!(sv = (void *) io->xpr_data))
    {
      io->xpr_data = AllocMem((long) sizeof(struct SetupVars), MEMF_PUBLIC | MEMF_CLEAR);
      if (!(sv = (void *) io->xpr_data))
        {
          ioerr(io, "MSG_NOT_ENOUGH_MEMORY_TXT");
          return NULL;
        }
      *sv = Default_Config;
    }

/*
 * Allocate memory for our unshared variables, to provide reentrancy
 */
  if (!(v = AllocMem((long) sizeof(struct Vars), MEMF_PUBLIC | MEMF_CLEAR)))
    {
    nomem:
      ioerr(io, "MSG_NOT_ENOUGH_MEMORY_TXT");
      return NULL;
    }
  v->Modemchar = v->Modembuf;

/*
 * * Allocate memory for our file I/O buffer; if we can't get as much as *
 * requested, keep asking for less until we hit minimum before giving up
 */
  v->Filebufmax = origbuf = atol(sv->option_b) * 1024;
  while (!(v->Filebuf = AllocMem(v->Filebufmax, 0L)))
    {
      if (v->Filebufmax > 1024)
        v->Filebufmax -= 1024;
      else
        {
          FreeMem(v, (long) sizeof(struct Vars));

          goto nomem;
        }
    }

/*
 * If framelength was intended to match buffer size, stay in sync
 */
  v->Tframlen = atol(sv->option_f);
  if (v->Tframlen && v->Tframlen == origbuf)
    v->Tframlen = v->Filebufmax;

  v->ErrorLimit = atol(sv->option_e);

/*
 * If selected, enable control character escape mode.
 */
  if (sv->option_c[0] == 'Y')
    v->Zctlesc = 1;
  else
    v->Zctlesc = 0;

/*
 * Take care of the file attribute options.
 */
  if (sv->option_i[0] == 'S')
    v->FileAttributes = 1;
  else
    {
      if (sv->option_i[0] == 'R')
        v->FileAttributes = 2;
      else
        v->FileAttributes = 3;
    }

/*
 * Get the maximum allowed packet size.
 */
  if ((v->ksize = atol(sv->option_m)) < MINBLOCK)
    v->ksize = MINBLOCK;
  else
    {
      if (v->ksize > KSIZE)
        v->ksize = KSIZE;
    }

/*
 * Copy caller's io struct into our Vars for easier passing
 */
  v->io   = *io;
  v->Baud = BaudTable[cfg.sysBaud];  /* set the current baud rate */

  return v;
}       /*
         * End of struct Vars *setup()
         */

/**********************************************************
 *      void set_textmode(struct Vars *v)
 *
 * Set text/binary mode flags in accordance with T option
 * setting
 **********************************************************/
void
set_textmode(struct Vars *v)
{
  struct SetupVars *sv;
  long      i;

  sv = (void *) v->io.xpr_data;
  switch (*sv->option_t)
    {
      case 'Y': /*
                         * Force text mode on receive; suggest text mode on
                         * send
                         */
      TY:
        v->Rxascii = TRUE;
        v->Rxbinary = FALSE;
        v->Lzconv = ZCNL;
        break;
      case 'N': /*
                         * Force binary mode on receive; suggest binary mode
                         * on send
                         */
      TN:
        v->Rxascii = FALSE;
        v->Rxbinary = TRUE;
        v->Lzconv = ZCBIN;
        break;
      case 'C': /*
                         * Ask comm program for proper mode for this file
                         */
            i = xpr_finfo(v->Filename, 2L);
            if (i == 1)         /*
                                 * Comm program says use binary mode
                                 */
              goto TN;
            if (i == 2)         /*
                                 * Comm program says use text mode
                                 */
              goto TY;
   /*
    * xpr_finfo() not provided (or failed); default to T?
    */
      case '?':
        v->Rxascii = v->Rxbinary = FALSE;
        v->Lzconv = 0;
        break;
    }
}       /*
         * End of void set_textmode()
         */

/**********************************************************
 *      UBYTE *find_option(UBYTE *buf, UBYTE option)
 *
 * Search for specified option setting in string
 **********************************************************/
UBYTE    *
find_option(UBYTE *buf, UBYTE option)
{
  while (*buf)
    {
      buf = skip_blank(buf);

      if (*buf == option)
        return ++buf;

      buf = skip_chars(buf);
    }

  return NULL;
}       /*
         * End of UBYTE *find_option()
         */

/**********************************************************
 *      void canit(struct Vars *v)
 *
 * send cancel string to get the other end to shut up
 **********************************************************/
void
canit(struct Vars *v)
{
  static char canistr[] =
  {24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
   8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 0};

  zmputs(v, canistr);
}       /*
         * End of void canit()
         */

/**********************************************************
 *      void zmputs(struct Vars *v, UBYTE *s)
 *
 * Send a string to the modem, with processing for \336 (sleep 1 sec)
 * and \335 (break signal, ignored since XPR spec doesn't support it)
 **********************************************************/
void
zmputs(struct Vars *v, UBYTE *s)
{
  UBYTE     c;

  while (*s)
    {
      switch (c = *s++)
        {
          case '\336':
            Delay(TICKS_PER_SECOND);
          case '\335':
            break;
          default:
            sendline(v, c);
        }
    }
  sendbuf(v);
}       /*
         * End of void zmputs()
         */

/**********************************************************
 *      void xsendline(struct Vars *v, UBYTE c)
 *
 * Write one character to the modem
 **********************************************************/
void
xsendline(struct Vars *v, UBYTE c)
{
  v->Outbuf[v->Outbuflen++] = c;
  if (v->Outbuflen >= sizeof(v->Outbuf))
    sendbuf(v);
}       /*
         * End of void xsendline()
         */

/**********************************************************
 *      void sendbuf(struct Vars *v)
 *
 * Send any data waiting in modem output buffer
 **********************************************************/
void
sendbuf(struct Vars *v)
{
  if (v->Outbuflen)
    {
 /*
  * Do the bookkeeping.
  */
      v->BytesSent += v->Outbuflen;
      xpr_swrite(v->Outbuf, (long) v->Outbuflen);
      v->Outbuflen = 0;
    }
}       /*
         * End of void sendbuf()
         */

/**********************************************************
 *      short readock(struct Vars *v, short tenths)
 *
 * Get a byte from the modem;
 * return TIMEOUT if no read within timeout tenths of a
 * second, return RCDO if carrier lost or other fatal error
 * (sread returns -1).  Added in some buffering so we
 * wouldn't hammer the system with single-byte serial port
 * reads.  Also, the buffering makes char_avail() a lot
 * easier to implement.
 **********************************************************/
short
readock(struct Vars *v, short tenths)
{
  long      t;

/*
 * If there's data waiting in our buffer, return next byte
 */
  if (v->Modemcount)
    {
    gotdata:
      --v->Modemcount;
      return (short) (*v->Modemchar++);
    }
/*
 * Our buffer is empty; see if there's anything waiting in system buffer. *
 * If the caller is in a hurry, don't wait around, but if it can spare * a
 * half second, wait a bit and build up some input so we don't do as * many
 * sread() calls.
 */
  t = (tenths < 5) ? 0 : 500000;
  v->Modemcount = xpr_sread(v->Modembuf, (long) sizeof(v->Modembuf),
                                      t);
  if (v->Modemcount < 0)        /*
                                 * Carrier dropped or other fatal error;
                                 * abort
                                 */
    {
      v->Modemcount = 0;
      return RCDO;
    }
  else if (!v->Modemcount)      /*
                                 * Nothing in system buffer; try waiting
                                 */
    {
      t = tenths * 100000L - t;
      v->Modemcount = xpr_sread(v->Modembuf, 1L, t);
      if (v->Modemcount < 0)
        {
          v->Modemcount = 0;
          return RCDO;
        }
      else if (!v->Modemcount)  /*
                                 * Nothing received in time
                                 */
        return TIMEOUT;
    }

/*
 * Do the bookkeeping.
 */
  v->BytesReceived += v->Modemcount;

/*
 * Reset buffer pointer to start of data
 */
  v->Modemchar = v->Modembuf;
  goto gotdata;
}       /*
         * End of short readock()
         */

/**********************************************************
 *      char char_avail(struct Vars *v)
 *
 * Check if there's anything available to read from the
 * modem
 **********************************************************/
char
char_avail(struct Vars *v)
{
  if (v->Modemcount)
    return TRUE;

/*
 * No data in our buffer; check system's input buffer
 */
  v->Modemcount = xpr_sread(v->Modembuf, (long) sizeof(v->Modembuf), 0L);
  if (v->Modemcount < 1)        /*
                                 * Nothing in system buffer either
                                 */
    {
      v->Modemcount = 0;
      return FALSE;
    }
  else
    {
 /*
  * Do the bookkeeping.
  */
      v->BytesReceived += v->Modemcount;

 /*
  * System buffer had something waiting for us
  */
      v->Modemchar = v->Modembuf;
      return TRUE;
    }
}       /*
         * End of char char_avail()
         */

/**********************************************************
 *      void update_rate(struct Vars *v)
 *
 * Update the elapsed time, expected total time, and
 * effective data transfer rate values for status display
 **********************************************************/
void
update_rate(struct Vars *v)
{
  ULONG     elapsed,
            expect,
            throughput,
            time,
            oldrate = v->xpru.xpru_datarate;
  struct timeval Now;

/*
 * What time is it?
 */
  GetSysTime(&Now);

/*
 * Now calculate the time elapsed since the protocol started * transferring
 * the current file.
 */
  if (CmpTime(&Now, &v->Starttime) > 0)
    Now.tv_secs = Now.tv_micro = 0;
  else
    SubTime(&Now, &v->Starttime);
  elapsed = Now.tv_secs;

/*
 * Now take a look at the data that go through so far.
 */
  if (v->Receiving)
    throughput = v->BytesReceived;
  else
    throughput = v->BytesSent;

/*
 * Cut off the lowest 24 bits.
 */
  if (throughput > 0x00FFFFFF)
    throughput = 0xFFFFFF00;
  else
    throughput = throughput << 8;

/*
 * Now calculate the time.
 */
  if (elapsed > 0x00FFFFFF)
    time = 0xFFFFFF00 | (Now.tv_micro >> 12);
  else
    time = (elapsed << 8) | (Now.tv_micro >> 12);

/*
 * Finally, calculate the effective number of characters * transferred per
 * second.
 */
  if (time)
    {
      if (v->xpru.xpru_datarate = throughput / time)
        v->xpru.xpru_updatemask |= XPRU_DATARATE;
      else
        v->xpru.xpru_updatemask &= ~XPRU_DATARATE;
    }
  else
    {
      v->xpru.xpru_datarate = 0;
      v->xpru.xpru_updatemask &= ~XPRU_DATARATE;
    }

/*
 * Any changes?
 */
  if (v->xpru.xpru_datarate == oldrate)
    v->xpru.xpru_updatemask &= ~(XPRU_DATARATE | XPRU_EXPECTTIME);

/*
 * Compute expected total transfer time based on data rate so far
 */
  if (v->xpru.xpru_filesize <= 0 || !v->xpru.xpru_datarate)
    v->xpru.xpru_updatemask &= ~XPRU_EXPECTTIME;
  else
    {
      if ((expect = (v->xpru.xpru_filesize - v->Strtpos) / v->xpru.xpru_datarate) < elapsed)
        expect = elapsed;

      v->xpru.xpru_updatemask |= XPRU_EXPECTTIME;
    }

/*
 * Any data to display?
 */
  if (v->xpru.xpru_updatemask & XPRU_EXPECTTIME)
    {
      v->xpru.xpru_expecttime = (char *) v->Msgbuf;

      sprintf(v->xpru.xpru_expecttime, "%02ld:%02ld:%02ld", expect / (60 * 60), (expect / 60) % 60, expect % 60);

      v->xpru.xpru_elapsedtime = &v->xpru.xpru_expecttime[strlen(v->xpru.xpru_expecttime) + 1];
    }
  else
    v->xpru.xpru_elapsedtime = v->Msgbuf;

  sprintf(v->xpru.xpru_elapsedtime, "%02ld:%02ld:%02ld", elapsed / (60 * 60), (elapsed / 60) % 60, elapsed % 60);
}

/**********************************************************
 *      long bfopen(struct Vars *v, UBYTE *mode)
 *
 * Buffered file I/O fopen() interface routine
 **********************************************************/
FILE *
bfopen(struct Vars *v, UBYTE *mode)
{
/*
 * Initialize file-handling variables
 */
  v->Filebufpos = v->Filebuflen = v->Filebufcnt = 0;
  v->Fileflush = FALSE;
  v->Filebufptr = v->Filebuf;
/*
 * Open the file
 */
  return fopen (v->Filename, mode);
}

/**********************************************************
 *      void bfclose(struct Vars *v)
 *
 * Buffered file I/O fclose() interface routine
 **********************************************************/
void
bfclose(struct Vars *v)
{
  if (v->File)
    {
 /*
  * If bfwrite() left data in buffer, flush it out before closing
  */
      if (v->Fileflush)
        fwrite (v->Filebuf, 1L, v->Filebufcnt, v->File);

 /*
  * Close the file
  */
      fclose (v->File);

 /*
  * Let's see if we should muck with the file attributes.
  */
      if (v->SetAttributes && (v->FileAttributes & 2))
        {
          ULONG     Bits = 0,
                    Seconds;

            Seconds = 0;

     /*
      * Take care of the owner bits.
      */
          if (v->Bits & 0100)
            Bits |= FIBF_EXECUTE;

          if (v->Bits & 0200)
            Bits |= FIBF_WRITE | FIBF_DELETE;

          if (v->Bits & 0400)
            Bits |= FIBF_READ;

     /*
      * Now for the group bits.
      */
          if (!(v->Bits & 0010))
            Bits |= FIBF_GRP_EXECUTE;

          if (!(v->Bits & 0020))
            Bits |= FIBF_GRP_WRITE | FIBF_GRP_DELETE;

          if (!(v->Bits & 0040))
            Bits |= FIBF_GRP_READ;

     /*
      * Now for the other bits.
      */
          if (!(v->Bits & 0001))
            Bits |= FIBF_OTR_EXECUTE;

          if (!(v->Bits & 0002))
            Bits |= FIBF_OTR_WRITE | FIBF_OTR_DELETE;

          if (!(v->Bits & 0004))
            Bits |= FIBF_OTR_READ;

     /*
      * Try to change the attributes.
      */
          SetProtection(v->Filename, Bits);

     /*
      * Now for the modification date.
      */
          if (Seconds)
            {
              struct DateStamp __aligned Date;

              Date.ds_Days = Seconds / 86400;
              Date.ds_Minute = (Seconds % 86400) / 60;
              Date.ds_Tick = (Seconds % 60) * TICKS_PER_SECOND;

              SetFileDate(v->Filename, &Date);
            }
        }

      v->File = NULL;
    }
}       /*
         * End of void bfclose()
         */

/**********************************************************
 *      void bfseek(struct Vars *v, long pos)
 *
 * Buffered file I/O fseek() interface routine
 **********************************************************/
void
bfseek(struct Vars *v, long pos)
{
  long      offset;

/*
 * If new file position is within currently buffered section, reset pointers
 */
  if (pos >= v->Filebufpos && pos < v->Filebufpos + v->Filebuflen)
    {
      offset = pos - v->Filebufpos;
      v->Filebufptr = v->Filebuf + offset;
      v->Filebufcnt = v->Filebuflen - offset;
 /*
  * Otherwise, fseek() file & discard buffer contents to force new read
  */
    }
  else
    {
      fseek (v->File, pos, SEEK_SET);
      v->Filebuflen = v->Filebufcnt = 0;
      v->Filebufpos = pos;
    }
}       /*
         * End of void bfseek()
         */

/**********************************************************
 *      long bfread(struct Vars *v, UBYTE *buf, long length)
 *
 * Buffered file I/O fread() interface routine
 **********************************************************/
long
bfread(struct Vars *v, UBYTE *buf, long length)
{
  long      count,
            total = 0;

/*
 * Keep going until entire request completed
 */
  while (length > 0)
    {
 /*
  * Copy as much of the request as possible from the buffer
  */
      count = (length <= v->Filebufcnt) ? length : v->Filebufcnt;
      CopyMem(v->Filebufptr, buf, count);
      buf += count;
      total += count;
      length -= count;
      v->Filebufptr += count;
      v->Filebufcnt -= count;

 /*
  * If we've emptied the buffer, read next buffer's worth
  */
      if (!v->Filebufcnt)
        {
          v->Filebufpos += v->Filebuflen;
          v->Filebufptr = v->Filebuf;
          v->Filebufcnt = v->Filebuflen
            = fread (v->Filebuf, 1L, v->Filebufmax, v->File);
     /*
      * If we hit the EOF, return with however much we read so far
      */
          if (!v->Filebufcnt)
            break;
        }
    }
  return total;
}       /*
         * End of long bfread()
         */

/**********************************************************
 *      long bfwrite(struct Vars *v, UBYTE *buf, long length)
 *
 * Buffered file I/O fwrite() interface routine
 **********************************************************/
long
bfwrite(struct Vars *v, UBYTE *buf, long length)
{
  long      count,
            total = 0;

/*
 * Keep going until entire request completed
 */
  while (length > 0)
    {
 /*
  * Copy as much as will fit into the buffer
  */
      count = v->Filebufmax - v->Filebufcnt;
      if (length < count)
        count = length;
      CopyMem(buf, v->Filebufptr, count);
      buf += count;
      total += count;
      length -= count;
      v->Filebufptr += count;
      v->Filebufcnt += count;
      v->Fileflush = TRUE;

 /*
  * If we've filled the buffer, write it out
  */
      if (v->Filebufcnt == v->Filebufmax)
        {
          count = fwrite(v->Filebuf, 1L, v->Filebufcnt, v->File);
          if (count < v->Filebufcnt)
            return -1;
          v->Filebufptr = v->Filebuf;
          v->Filebufcnt = 0;
          v->Fileflush = FALSE;
        }
    }
  return total;
}       /*
         * End of long bfwrite()
         */

/**********************************************************
 *      void ioerr(struct XPR_IO *io, char *msg)
 *
 * Have the comm program display an error message for us,
 * using a temporary XPR_UPDATE structure; used to display
 * errors before Vars gets allocated
 **********************************************************/
void
ioerr(struct XPR_IO *io, char *msg)
{
  struct XPR_UPDATE xpru;

      xpru.xpru_updatemask = XPRU_ERRORMSG;
      xpru.xpru_errormsg = msg;
      display_update(&xpru);
}       /** End of void ioerr() */

/**********************************************************
 *      void upderr(struct Vars *v, char *msg)
 *
 * Have the comm program display an error message for us, using the
 * normal XPR_IO structure allocated in Vars
 **********************************************************/
void
upderr(struct Vars *v, char *msg)
{
  v->xpru.xpru_updatemask = XPRU_ERRORMSG;
  v->xpru.xpru_errormsg = (STRPTR) msg;
  if ((STRPTR) msg == v->Msgbuf)        /*
                                         * Ensure message length < 50
                                         */
    msg[48] = '\0';
  display_update (&v->xpru);
}

/**********************************************************
 *      void updmsg(struct Vars *v,char *msg)
 *
 * Have the comm program display a normal message for us
 **********************************************************/
void
updmsg(struct Vars *v, char *msg)
{
  v->xpru.xpru_updatemask = XPRU_MSG;
  v->xpru.xpru_msg = (STRPTR) msg;
  if ((STRPTR) msg == v->Msgbuf)        /*
                                         * Ensure message length < 50
                                         */
    msg[48] = '\0';
  display_update(&v->xpru);
}

/**********************************************************
 *      long getfree(void)
 *
 * Figure out how many bytes are free on the drive we're uploading to.
 * Stubbed out for now; not supported by XPR spec.
 **********************************************************/
long
getfree(struct Vars *v)
{
  LONG      Space = 0x7FFFFFFF;   /*
                                   * = Unknown
                                   */
  return (Space);

}

/**********************************************************
 *      char exist(struct Vars *v)
 *
 * Check whether file already exists; used to detect
 * potential overwrites
 **********************************************************/
char
exist(struct Vars *v)
{
  FILE *alock;
  alock = fopen(v->Filename,"R");
  if( alock )
    {
    fclose(alock);
    };
  return (char)(alock != NULL);
}
