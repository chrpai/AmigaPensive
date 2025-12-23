/*
 *                              msgadd.c
 *
 * External Message handler.  For use with external OtherNet parsers.
 */
#define VIEWING (1)
/*
 *                              history
 *
 * 96Sep01 AFP  V1.5 -- support vortex checking and archiving by month & year
 * 91Mar31 HAW  v1.4 -- support for virtual rooms.
 * 90Aug13 HAW  v1.3 -- support for room archiving.
 * 89Oct23 HAW  v1.2 -- support for incoming route mail.
 * 88Nov05 HAW  Created.
 */
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "ctdl.h"
/*
 *                              contents
 *
 */

#define TITLE           "C86Net Message Importer"

int debug_flag=0;
int force_flag=0;

#define NO_ERROR        0
#define BAD_ARGS        1
#define BAD_TABLE       2
#define NO_NODE         3
#define FATAL           4
#define LF_ERROR        5

int       crtColumn;
extern CONFIG cfg;                /* Configuration variables      */
extern MessageBuffer msgBuf;      /* The -sole- message buffer    */
extern aRoom roomBuf;             /* Room buffer  */
extern logBuffer logBuf;
extern FILE *roomfl,
         *logfl;
extern int thisRoom;              /* Current room */
extern rTable *roomTab;
extern struct mBuf mFile1,
          mFile2;
extern NetTable *netTab;
extern NetBuffer netBuf;
extern NetBuffer netTemp;
extern FILE *netfl;
extern LogTable *logTab;
extern int thisNet;               /* Current node in use  */
extern char *APPEND_ANY;
extern char *APPEND_TEXT;
extern FILE *msgfl,
         *msgfl2;
FILE     *GlobalFd;
int       RouteSlot;
extern FILE *upfd;
char      inNet = NORMAL_NET;

FILE     *netLog = stderr;
char      logNetResults = TRUE;
extern char      netDebug;

SListBase MailForward =
{NULL, NULL, NULL, NULL, NULL};
SListBase Arch_base =
{NULL, ChkNtoStr, NULL, FreeNtoStr, EatNMapStr};
static char EOP = FALSE;
char     *R_SH_MARK = "&&";
char     *NON_LOC_NET = "%%";
char     *LOC_NET = "++";

/**
  Some statistics variables
**/
int total_msgs    = 0; /* total messages processed */
int vortex_msgs   = 0; /* rejected messages due to vortexes */
int added_msgs    = 0; /* messages added */
int rejected_msgs = 0; /* message  rejected for other reasons */

long char_in, char_out;         /* send/recieve character counts*/
long start_time;                /* total time of session */


void      GenInit (void);
void      Process (char *fn);
int       GetOtherNetChar (void);
int       VirtualShared (int NetNo, label name);
int       VirtualExists (label name);
void      SaveIt (void);
int       FindRoom (label nm);
int       FindNet (label nm, NetBuffer * netBuf);
void      AddVirtualField (char field, char *contents);
void      PutMessage (void);
void      HandleRouted (void);
void      AddField (int field, char *fValue);
int       FindPos (void);
void      VirtualHandle (void);
int       MARecVirtualRoom (int VirtIndex);
void      netResult (char *);
char      Check_Address(label mbaddr );
void      Fix_Name(char *out, char *in, int flag);
void      getCdate (int *year, char **month, int *day, int *hours, int *minutes);

int
mPrintf (char *format,...)
{
  return 0;
}       /* stub to quiet the linker */

/*
 * crashout()
 *
 * Big error handler.
 */
void
crashout (str)
  char     *str;
{
  printf (str);
  exit (FATAL);
}

/*
 * main()
 *
 * This is the main manager.
 */
int       main (int, char **);
int
main (argc, argv)
  char    **argv;
  int       argc;
{
  char *node;
  int       rover;
  printf ("%s %s\n%s\n\n", TITLE, VERSION_NAME, COPYRIGHT);
  if (argc < 3)
    {
      printf ("usage: MSGADD <options>  nodename file [ file ... ]\n");
      printf (" options:  -f  == force adding of messages even if not shared\n");
      printf ("           -d  == debug information \n");
      printf ("\nEach file may contain one or more messages in C86Net format.\n");
      exit (BAD_ARGS);
    }
  /**
    First scan for any options.
  **/
  termWidth = 80;  /* set default line width to 80 characters */
  for( rover=1; rover < argc && argv[rover][0] == '-'; rover++)
    {
    if( argv[rover][1] == 'f' || argv[rover][1] == 'F' )force_flag = 1;
    if( argv[rover][1] == 'd' || argv[rover][1] == 'D' )debug_flag = 1;
    };

  char_in = char_out = 0;
  start_time = Set_Timer(0);


  cfg.weAre = UTILITY;
  if (!readSysTab (TRUE, TRUE))
    {
      exit (BAD_TABLE);
    }

  if (access (LOCKFILE, 0) != -1)
    {
      printf ("Please do not run MsgAdd using Outside Commands.\n");
      writeSysTab ();
      exit (LF_ERROR);
    }

  GenInit ();
  VirtInit ();
  VortexInit ();
  InitVortexing ();

  /**
    find the first non-option argument and use as
    the node name
  **/
  for( rover=1; rover < argc && argv[rover][0] == '-'; rover++)
    ;
  node = argv[rover];
  if (FindNet (argv[rover], &netBuf) == ERROR)
    {
      writeSysTab ();
      printf ("Could not find node %s.\n", argv[rover]);
      exit (NO_NODE);
    };

  for (rover++ ; rover < argc; rover++)
    {
    if( argv[rover][0] != '-' )
      {
      Process (argv[rover]);
      };
    };
  strcpy(msgBuf.mbtext,"No Vortex Problems found\n");
  FinVortexing ();      /* cleanup and report errors */
  printf(msgBuf.mbtext);
  UpdVirtStuff ();
  writeSysTab ();
  printf(" %d vortex, %d rejected, %d added messaged\n",
  vortex_msgs, rejected_msgs, added_msgs);
  printf(" %d total messages processed for %s\n", total_msgs, node);
  Compute_Data(netBuf.netName);
  return 0;
}

/*
 * GenInit()
 *
 * This is a general initialization routine.
 */
void
GenInit ()
{
  SYS_FILE  fn;

  initNetBuf (&netBuf);
  initNetBuf (&netTemp);
  makeSysName (fn, "ctdlnet.sys", &cfg.netArea);
  openFile (fn, &netfl);

  initRoomBuf (&roomBuf);
  makeSysName (fn, "ctdlroom.sys", &cfg.roomArea);
  openFile (fn, &roomfl);

  InitMsgBase ();

  initLogBuf (&logBuf);
  makeSysName (fn, "ctdllog.sys", &cfg.logArea);
  openFile (fn, &logfl);

  makeSysName (fn, "ctdlarch.sys", &cfg.roomArea);
  MakeList (&Arch_base, fn, NULL);
}

/*
 * Process()
 *
 * This will process a file - read a file for all messages and stuff them
 * into the message base.
 */
void
Process (fn)
  char     *fn;
{
  extern char *READ_ANY;

  if ((GlobalFd = fopen (fn, READ_ANY)) == NULL)
    {
      printf ("ERROR: Could not open %s.\n", fn);
      return;
    }

  while (getMessage (GetOtherNetChar, TRUE, TRUE, TRUE))
    SaveIt ();

  fclose (GlobalFd);
}

/*
 * GetOtherNetChar()
 *
 * This gets a character for getMessage.
 */
int
GetOtherNetChar ()
{
  int       c;

  c = getc (GlobalFd);
  char_in++;
  if (c == EOF)
    return -1;
  return c;
}

/*
 * SaveIt()
 *
 * This is charged with saving the message in the database.
 *
 * 1. If mail, must do recipient validation.
 * 2. Must validate room.
 */
void
SaveIt ()
{
  int       LogSlot,
            RoomSlot,
            place;
  extern char *NON_LOC_NET,
           *LOC_NET;
  char     *fn,
           *realfn;
  total_msgs++;
  if( debug_flag )
    {
    printf ("mbauth =-%s-\t", msgBuf.mbauth);
    printf ("mbroom =-%s-\n", msgBuf.mbroom);
    printf ("mbdate =-%s-\t", msgBuf.mbdate);
    printf ("mbtime =-%s-\n", msgBuf.mbtime);
    printf ("mboname=-%s-\t", msgBuf.mboname);
    printf ("mborig =-%s-\n", msgBuf.mborig);
    printf ("mbto   =-%s-\t", msgBuf.mbto);
    printf ("mbsrcId=-%s-\n", msgBuf.mbsrcId);
    };
  if (VirtualExists (msgBuf.mbroom) != ERROR)
    {
      VirtualHandle ();
      return;
    };

  if (Check_Address(msgBuf.mbaddr) != 0)
    {
      HandleRouted ();
    }
  else if (strCmpU (msgBuf.mbroom, "mail") == SAMESTRING)
    {
      if( strCmpU(msgBuf.mbto, "sysop") == SAMESTRING )
        {
        strcpy(msgBuf.mbto, cfg.SysopName);
        };
      if ((LogSlot = findPerson (msgBuf.mbto, &logBuf)) == ERROR)
        {
          printf ("Could not deliver Mail to '%s', does not exist.\n",
                  msgBuf.mbto);
          rejected_msgs++;
          return;
        }
      noteAMessage (logBuf.lbMail, MAILSLOTS, cfg.newest + 1, cfg.catSector);
      putLog (&logBuf, LogSlot);
    }
  else
    {
      if ((RoomSlot = FindRoom (msgBuf.mbroom)) == ERROR)
        {
          printf ("Message meant for non-existent room '%s' not incorporated.\n",
                  msgBuf.mbroom);
          rejected_msgs++;
          return;
        };
      if( debug_flag ) printf(" RoomSlot:%d\n",RoomSlot);
      /**
         Validate that the rooms is shared properly, ignored if we have force
      **/
      if ((place = FindPos ()) == ERROR)
        {
        printf ("WARNING: msg for %s is not formally shared, discarded.\n",
                msgBuf.mbroom);
        if( force_flag == 0 )
          {
          rejected_msgs++;
          return;
          };
        };
      /**
         Do vortex checking
      **/
      if (!NotVortex ())
        {
          printf ("Vortex detected, message is a duplicate, not incorporated.\n");
          printf ("Author:%20s   ", msgBuf.mbauth);
          printf ("Room:%s\n", msgBuf.mbroom);
          printf ("  date:%20s   ", msgBuf.mbdate);
          printf ("time:%s\n", msgBuf.mbtime);
          printf (" Oname:%20s   ", msgBuf.mboname);
          printf ("orig:%s\n", msgBuf.mborig);
          printf ("    to:%20s   ", msgBuf.mbto);
          printf (" src:%s\n\n", msgBuf.mbsrcId);
          vortex_msgs++;
          return;
        }
      else
        {
          if( msgBuf.mbsrcId[0] == '\0' )
             {
             printf ("Possible duplicate, Missing src id, added anyway.\n");
             printf ("Author:%20s   ", msgBuf.mbauth);
             printf ("Room:%s\n", msgBuf.mbroom);
             printf ("  date:%20s   ", msgBuf.mbdate);
             printf ("time:%s\n", msgBuf.mbtime);
             printf (" Oname:%20s   ", msgBuf.mboname);
             printf ("orig:%s\n", msgBuf.mborig);
             printf ("    to:%20s   ", msgBuf.mbto);
             printf (" src:%s\n\n", msgBuf.mbsrcId);
             };
        };

      noteAMessage (roomBuf.msg, MSGSPERRM, cfg.newest + 1, cfg.catSector);
      roomTab[RoomSlot].rtlastMessage = cfg.newest + 1;
      putRoom (RoomSlot);
      if( place != ERROR )
        {
        if (roomBuf.rbShareType != PEON && netBuf.netRooms[place].mode != PEON)
          strCpy (msgBuf.mbaddr, NON_LOC_NET);
        else
          strCpy (msgBuf.mbaddr, LOC_NET);
        };

      if (roomBuf.rbflags.ARCHIVE == 1)
        {
          fn = SearchList (&Arch_base, NtoStrInit (thisRoom, "", 0, TRUE));
          realfn = GetDynamic (strlen (fn) + 15);
          TranslateFilename (realfn, fn);
          if( debug_flag ) printf(" Filename;%s is now %s\n",fn,realfn);
          if ((upfd = fopen (realfn, APPEND_TEXT)) != NULL)
            {
              if (msgBuf.mbdate[0])fprintf (upfd, "   %s ", msgBuf.mbdate);
              if (msgBuf.mbtime[0] && sendTime)fprintf (upfd, "%s ", msgBuf.mbtime);
              if (msgBuf.mbauth[0]) fprintf (upfd, "from %s", msgBuf.mbauth);
              NormStr (msgBuf.mboname);
              if (msgBuf.mboname[0])
                {
                  fprintf (upfd, " @ %s", msgBuf.mboname);
                  if (msgBuf.mbdomain[0])fprintf (upfd, cfg.DomainDisplay, msgBuf.mbdomain);
                };

              if (msgBuf.mbto[0]) fprintf (upfd, " to %s", msgBuf.mbto);
              crtColumn = 1;
              mFormat (msgBuf.mbtext);
              fprintf (upfd, "\n");
              fclose (upfd);
            };
          free(realfn);
        }
    }
  /**  probably don't need this... strCpy (msgBuf.mborig, netBuf.netId); **/
  PutMessage ();
  cfg.newest++;
  added_msgs++;
  cfg.catSector = mFile1.thisSector;
  cfg.catChar = mFile1.thisChar;
}

SListBase FwdAliasii;             /* keeps some other stuff happy */
char      onConsole = FALSE,
          remoteSysop = FALSE;
int       callSlot = ERROR;

/*
 * HandleRouted()
 *
 * This should handle mail routing incoming.
 */
void
HandleRouted ()
{
  char     *AltName;
  label     Name,
            Id,
            temp;
  SYS_FILE  fn;
  extern void (*NetPrintTarget) (char *fmt,...);
  extern int (*ToFileWork) ();

/* no find? */
  if ((RouteSlot = FindNet (msgBuf.mbaddr, &netTemp)) == ERROR)
    {
      return;
    }

/* yes, found, data in netTemp - will we do the routing? */
  if (!cfg.BoolFlags.RouteMail || !netTemp.nbflags.RouteTo)
    {
      return;
    }

/* yes, we'll do the routing.  Now to figure it out. */
  strCpy (Name, netTemp.netName);
  strCpy (Id, netTemp.netId);

  if ((AltName = UseNetAlias (Name, TRUE)) != NULL)
    strCpy (Name, AltName);

  if (FindRouteSlot () == ERROR)
    {
      return;
    }

/* Ugly ugly kludge until we figure out how nbHiRouteInd is wrong */
  if (netTemp.nbHiRouteInd < 0)
    netTemp.nbHiRouteInd = 0;
  sPrintf (temp, "R%d.%d", RouteSlot, netTemp.nbHiRouteInd++);

  makeSysName (fn, temp, &cfg.netArea);

  if ((upfd = safeopen (fn, APPEND_ANY)) == NULL)
    {
      return;
    }

  fprintf (upfd, "%-20s", Id);
  putc (0, upfd);
  fprintf (upfd, "%-20s", Name);
  putc (0, upfd);

  NetPrintTarget = ToFile;
  StartEncode (putFLChar);
  ToFileWork = Encode;
  prNetStyle (TRUE, Encode, FALSE, "");
  StopEncode ();
  fclose (upfd);
  netTemp.nbflags.HasRouted = TRUE;

  putNet (RouteSlot, &netTemp);
  added_msgs++;
}

/*
 * findRoom()
 *
 * This function should find the named room.  Return ERROR if not found.
 */
int
FindRoom (nm)
  label     nm;
{
  int       rover;

  for (rover = 0; rover < MAXROOMS; rover++)
    if (strCmpU (roomTab[rover].rtname, nm) == SAMESTRING)
      {
        getRoom (rover);
        return rover;
      }

  return ERROR;
}

/*
 * noteAMessage()
 *
 * This function notes a message in a message array.  Stolen from MSG.C.
 */
void
noteAMessage (base, slots, id, loc)
  MSG_NUMBER id;
  SECTOR_ID loc;
  theMessages *base;
  int       slots;
{
  int       i;

/* store into current room: */
/* slide message pointers down to make room for this one:       */
  for (i = 0; i < slots - 1; i++)
    {
      base[i].rbmsgLoc = base[i + 1].rbmsgLoc;
      base[i].rbmsgNo = base[i + 1].rbmsgNo;
    }

/* slot this message in:    */
  base[slots - 1].rbmsgNo = id;
  base[slots - 1].rbmsgLoc = loc;
}

/*
 * FindNet()
 *
 * This function will find the named node.  Stolen from searchNameNet/NETMISC.
 */
int
FindNet (label nm, NetBuffer * nBuf)
{
  int       rover;

  for (rover = 0; rover < cfg.netSize; rover++)
    {
      if (netTab[rover].ntflags.in_use &&
          hash (nm) == netTab[rover].ntnmhash)
        {
          getNet (rover, nBuf);
          if (strCmpU (nBuf->netName, nm) == SAMESTRING)
            return rover;
        }
    }
  return ERROR;
}

/*
 * FindPos()
 *
 * This finds the spot in the shared room array for the node that matches up
 * with the current room.
 */
int
FindPos ()
{
  int       rover;

  for (rover = 0; rover < SHARED_ROOMS; rover++)
    if (isSharedRoom (thisNet, rover) &&
        netRoomSlot (rover) == thisRoom &&
        netGen (thisNet, rover) == roomBuf.rbgen)
      return rover;

  return ERROR;
}

/*
 * VirtualHandle()
 *
 * This function will handle a message destined for a virt room.
 */
void
VirtualHandle ()
{
  int       Vindex;

  if ((Vindex = VirtualShared (thisNet, msgBuf.mbroom)) == ERROR)
    {
      printf ("Virtual room %s not shared with %s, message not saved.\n",
              msgBuf.mbroom, netBuf.netName);
      return;
    }
  MARecVirtualRoom (Vindex);
}

/*********** These functions stolen & modified from VIRT2.C ***************/

extern VirtualRoom *VRoomTab;
extern VirtNet *VirtNetList;
extern int VirtSize,
          VNetSize;

/*
 * MARecVirtualRoom()
 *
 * This function receives a virtual room from another system.
 */
int
MARecVirtualRoom (int VirtIndex)
{
  int       VirtNo;
  MSG_NUMBER rover;
  char     *distance,
            fn[50];
  extern FILE *upfd;
  extern char *WRITE_ANY;

  VirtNo = VirtNetList[thisNet].VirtList[VirtIndex].WhichVirt;

  if (VirtNetList[thisNet].VirtList[VirtIndex].mode != PEON)
    {
      distance = LD_DIR;
      rover = VRoomTab[VirtNo].vrHiLD + 1l;
      VRoomTab[VirtNo].vrChanged |= LD_CHANGE;
    }
  else
    {
      distance = LOCAL_DIR;
      rover = VRoomTab[VirtNo].vrHiLocal + 1l;
      VRoomTab[VirtNo].vrChanged |= LOC_CHANGE;
    }
  CreateVAName (fn, VirtNo, distance, rover);

  if ((upfd = fopen (fn, WRITE_ANY)) != NULL)
    {
      if (msgBuf.mbauth[0])
        AddVirtualField ('A', msgBuf.mbauth);
      if (msgBuf.mbdate[0])
        AddVirtualField ('D', msgBuf.mbdate);
      if (msgBuf.mbtime[0])
        AddVirtualField ('C', msgBuf.mbtime);
      if (msgBuf.mboname[0])
        AddVirtualField ('N', msgBuf.mboname);
      if (msgBuf.mborig[0])
        AddVirtualField ('O', msgBuf.mborig);
      if (msgBuf.mbroom[0])
        AddVirtualField ('R', msgBuf.mbroom);
      if (msgBuf.mbsrcId[0])
        AddVirtualField ('S', msgBuf.mbsrcId);
      if (msgBuf.mbto[0])
        AddVirtualField ('T', msgBuf.mbto);
      if (msgBuf.mbOther[0])
        AddVirtualField ('P', msgBuf.mbOther);
      if (msgBuf.mbdomain[0])
        AddVirtualField ('X', msgBuf.mbdomain);
      if (msgBuf.mbtext[0])
        AddVirtualField ('M', msgBuf.mbtext);
      fclose (upfd);
    }
  else
    printf ("Unable to open %s!\n", fn);

  VirtSummary ();
  return 0;
}

/*
 * AddVirtualField()
 *
 * This function adds a field to a virtual room message.
 */
void
AddVirtualField (char field, char *contents)
{
  fprintf (upfd, "%c%s", field, contents);
  fputc (0, upfd);
}

/*
 * VirtualExists()
 *
 * This function returns an index to given room, if exists.
 */
int
VirtualExists (label name)
{
  int       rover;

  for (rover = 0; rover < VirtSize; rover++)
    if (strCmpU (VRoomTab[rover].vrName, name) == SAMESTRING)
      return rover;

  return ERROR;
}

/*
 * VirtualShared()
 *
 * This function returns an index into current net's virtual index.
 */
int
VirtualShared (int NetNo, label name)
{
  int       rover,
            VirtNo;

  if ((VirtNo = VirtualExists (name)) == ERROR)
    return ERROR;

  for (rover = 0; rover < VIRT_LIMIT; rover++)
    if (VirtNetList[NetNo].VirtList[rover].WhichVirt == VirtNo)
      return rover;
  return ERROR;
}

/*********** These functions stolen & modified from MSG.C ***************/

/*
 * doActualWrite()
 *
 * This should allow automatic bkp of msg file from RAM.
 */
char
doActualWrite (FILE *whichmsg, struct mBuf *mFile, char c)
  {
  MSG_NUMBER temp;
  int       toReturn = 0;

  if (mFile->sectBuf[mFile->thisChar] == 0xFF)
    {
 /* obliterating a msg   */
      toReturn = 1;
    }

  mFile->sectBuf[mFile->thisChar] = c;

  mFile->thisChar = ++mFile->thisChar % MSG_SECT_SIZE;

  if (mFile->thisChar == 0)
    {   /* time to write sector out and get next: */
      temp = mFile->thisSector;
      temp *= MSG_SECT_SIZE;
      fseek (whichmsg, temp, 0);
      crypte (mFile->sectBuf, MSG_SECT_SIZE, 0);
      if (fwrite (mFile->sectBuf, MSG_SECT_SIZE, 1, whichmsg) != 1)
        {
          crashout ("?putMsgChar-write fail");
        }

      mFile->thisSector = ++mFile->thisSector % cfg.maxMSector;
      temp = mFile->thisSector;
      temp *= MSG_SECT_SIZE;
      fseek (whichmsg, temp, 0);
      if (fread (mFile->sectBuf, MSG_SECT_SIZE, 1, whichmsg) != 1)
        {
          crashout ("?putMsgChar-read fail");
        }
      crypte (mFile->sectBuf, MSG_SECT_SIZE, 0);
    }
  return (char) toReturn;
}

/*
 * doFlush()
 *
 * This will do actual writeup for specified msg file.
 */
void
doFlush (whichmsg, mFile)
  FILE     *whichmsg;
  struct mBuf *mFile;
{
  long int  s;

  s = mFile->thisSector;
  s *= MSG_SECT_SIZE;
  fseek (whichmsg, s, 0);
  crypte (mFile->sectBuf, MSG_SECT_SIZE, 0);
  if (fwrite (mFile->sectBuf, MSG_SECT_SIZE, 1, whichmsg) != 1)
    {
      crashout ("?ctdlmsg.sys write fail");
    }
  crypte (mFile->sectBuf, MSG_SECT_SIZE, 0);
  fflush (whichmsg);
}

/*
 * flushMsgBuf()
 *
 * This wraps up writing a message to disk, takes into account 2nd msg file if
 * necessary.
 */
void
flushMsgBuf ()
{
  doFlush (msgfl, &mFile1);
  if (cfg.BoolFlags.mirror)
    doFlush (msgfl2, &mFile2);
}

/*
 * putMessage()
 *
 * This function stores a message to disk.
 * Always called before noteMessage() -- newest not ++ed yet.
 * Returns: TRUE on successful save, else FALSE
 */
void
PutMessage ()
{
  char     temp[150];
  extern char *ALL_LOCALS,
           *WRITE_LOCALS;
  extern char *R_SH_MARK,
           *LOC_NET,
           *NON_LOC_NET;
  char     *s;

  startAt (msgfl, &mFile1, cfg.catSector, cfg.catChar);
/* tell putMsgChar where to write   */
  if (cfg.BoolFlags.mirror)
    startAt (msgfl2, &mFile2, cfg.catSector, cfg.catChar);

  putMsgChar (0xFF);    /* start-of-message     */

/* write message ID */
  sPrintf (temp, "%lu", cfg.newest + 1);
  AddField (0, temp);

/* write date:      */
  if (msgBuf.mbdate[0])
    {
      AddField ('D', msgBuf.mbdate);
    }
  else
    {
      AddField ('D', "????");
    }

/* write time:      */
  if (msgBuf.mbtime[0])
    {
      AddField ('C', msgBuf.mbtime);
    }
  else
    {
      AddField ('C', "!!!!");
    }

/* write author's name out: */
  if (msgBuf.mbauth[0])
    {
      AddField ('A', msgBuf.mbauth);
    }

/* write room name out:     */
  AddField ('R', msgBuf.mbroom);

  if (msgBuf.mbto[0])
    {   /* private message -- write addressee   */
      AddField ('T', msgBuf.mbto);
    }

  if (msgBuf.mbaddr[0])
    {   /* net message routing  */
      if (strCmpU (msgBuf.mbaddr, R_SH_MARK) == SAMESTRING ||
          strCmpU (msgBuf.mbaddr, NON_LOC_NET) == SAMESTRING)
        {
          AddField ('N', msgBuf.mboname);
          if (msgBuf.mborig[0])
            {
              AddField ('O', msgBuf.mborig);
            }
          roomTab[thisRoom].rtlastNet = cfg.newest + 1;
        }
      else if (strCmpU (msgBuf.mbaddr, LOC_NET) == SAMESTRING)
        {
          AddField ('N', msgBuf.mboname);
          if (msgBuf.mborig[0])
            {
              AddField ('O', msgBuf.mborig);
            }
        }

      sPrintf (temp, "%s%d", msgBuf.mbaddr, thisNet);
      AddField ('Q', temp);
    }
  else
    {
      if (msgBuf.mboname[0])
        {
          AddField ('N', msgBuf.mboname);
        }
      if (msgBuf.mborig[0])
        {
          AddField ('O', msgBuf.mborig);
        }
    }

  if (msgBuf.mbdomain[0])
    {
      AddField ('X', msgBuf.mbdomain);
    }

  if (msgBuf.mbsrcId[0])
    {
      AddField ('S', msgBuf.mbsrcId);
    }

  if (msgBuf.mbOther[0])
    {
      AddField ('P', msgBuf.mbOther);
    }

/* write message text by hand because it would overrun AddField buffer: */
  putMsgChar ('M');     /* M-for-message.       */
  for (s = msgBuf.mbtext; *s; s++)
    putMsgChar (*s);

  putMsgChar (0);       /* null to end text     */
  flushMsgBuf ();

}

/*
 * AddField()
 *
 * This adds a field to the message base.
 */
void
AddField (field, fValue)
  int       field;
  char     *fValue;
{
  int i=0;
  if( debug_flag )
    {
    if( field)
      printf("   %c:%s(",field, fValue);
    else
      printf("NULL:%s(",fValue);
    };
  if (field)
    putMsgChar (field);
  while (*fValue)
    {
    putMsgChar (*fValue++);
    i++;
    };
  putMsgChar (0);       /* End field. */
  if( debug_flag)printf("%d characters)\n", i);
}

/*
 * putMsgChar()
 *
 * This function writes successive message chars to disk.
 * Globals:     thisChar=       thisSector=
 * Returns:     ERROR if problems else TRUE.
 */
int
putMsgChar (c)
  int       c;
{
  int       toReturn;
  int       count1,
            count2;

  toReturn = TRUE;
  count1 = doActualWrite (msgfl, &mFile1, c);
  if (cfg.BoolFlags.mirror)
    {
      count2 = doActualWrite (msgfl2, &mFile2, c);
      if (count1 != count2)
        printf ("Mirror msg count discrepancy!");
    }
  if (count1)
    ++cfg.oldest;
  return toReturn;
}

static label SearchResult;
static char *SearchTarget,
          GetAlias;

/*
 * UseNetAlias()
 *
 * This function will find a usenet alias or the converse.
 */
char     *
UseNetAlias (char *Name, char FindAlias)
{
  void     *EatTrans ();
  SListBase Dummy =
  {NULL, NULL, NULL, NULL, EatTrans};
  SYS_FILE  fn;
  char     *c;

  SearchResult[0] = 0;
  SearchTarget = Name;
  if (!FindAlias)
    while ((c = (char *)strchr (Name, ' ')) != NULL)
      *c = '_';
  makeSysName (fn, "st-alias.sys", &cfg.roomArea);
  GetAlias = FindAlias;
  MakeList (&Dummy, fn, NULL);  /* CHEAT!  WHEEEEEE! */
  if (strLen (SearchResult) == 0)
    return NULL;
  return SearchResult;
}

/*
 * EatTrans()
 *
 * This function will eat a line of for the alias mapping.
 */
void     *
EatTrans (char *line)
{
  char     *c;

  if ((c = (char *)strchr (line, ' ')) != NULL)
    {
      *c = 0;
      if (GetAlias)
        {       /* check second field */
          if (strCmpU (c + 1, SearchTarget) == SAMESTRING)
            {
              strCpy (SearchResult, line);
            }
        }
      else
        {       /* check first field */
          if (strCmpU (line, SearchTarget) == SAMESTRING)
            {
              strCpy (SearchResult, c + 1);
            }
        }
    }
  return NULL;
}

/*
 * FindRouteSlot()
 *
 * This function will find the slot of routing node.  If found, netTemp will
 * contain the new slot, as will RouteSlot.
 */
int
FindRouteSlot ()
{
/*
 * first check to see if we're the direct (final) link, either
 * expressly or because the route is outdated.
 */
  if (!DirectRoute (&netTemp))  /* expressly?   */
    getNet ((RouteSlot = netTemp.nbRoute), &netTemp);

  return RouteSlot;
}

/*
 * DirectRoute()
 *
 * This will discover if we directly or indirectly talk to this node.
 */
char
DirectRoute (NetBuffer * n)
{
  return (char) (n->nbRoute == -1 ||    /* expressly?   */
                 !netTab[n->nbRoute].ntflags.in_use ||  /* outdated?    */
                 netTab[n->nbRoute].ntGen != n->nbRouteGen);    /* outdated? */
}

/*
 * getNetChar()
 *
 * This gets a character from a network temporary file.
 */
int
getNetChar ()
{
  return -1;    /* actually, this should never be called.       */
}

/*
 * SepNameSystem()
 *
 * This will parse an Other Recipient spec.
 */
char
SepNameSystem (char *string, char *person, char *system, NetBuffer * buf)
{
  char     *c;
  int       n;
  char      work[NAMESIZE * 3];   /* should be sufficient */

  strCpy (work, string);
  if ((c = strchr (work, '@')) == NULL)
    return NOT_SYSTEM;

/* find leading spaces of "name @ system" format */
  for (n = 1; c[n] == ' ' && c[n]; n++)
    ;

  if (strLen (c + n) >= NAMESIZE)
    return BAD_FORMAT;

  strCpy (system, c + n);
  if (searchNameNet (c + n, buf) == ERROR)      /* bad - set signal of it */
    return NO_SYSTEM;

  *c = 0;
  CleanEnd (work);      /* kill trailing spaces */
  if (strLen (work) >= NAMESIZE)
    return BAD_FORMAT;

  strCpy (person, work);

  return IS_SYSTEM;
}

#define WeServe(x)      SearchList(&Serves, x)
extern SListBase Serves;

/*
 * LocalName()
 *
 * This takes a string of form <system> _ <domain> and attempts to discover if
 * this domain mapped system is actually a local.  This is used when we're
 * sending mail and are trying to find out if a Who Else override needs to be
 * generated.  Ugly kludge, but, hey, that's what programming's all about, eh?
 */
char     *
LocalName (char *system)
{
  char     *domain,
           *System;

  if ((domain = strchr (system, '_')) == NULL)
    return system;
  domain += 2;  /* always preceded by a space -- or so we assume */

  if (strCmpU (domain, (char *)(cfg.codeBuf + cfg.nodeDomain)) == SAMESTRING ||
      WeServe (domain) != NULL)
    {
      System = strdup (system);
      if ((domain = strchr (System, ' ')) == NULL)
        return system;  /* should never happen, though */
      *domain = NULL;
      if (searchNameNet (System, &netTemp) != ERROR)
        {
          free (System);
          return netTemp.netName;
        }
      free (System);
    }
  return system;
}

/*
 * mFormat()
 *
 * This function formats a string to modem and console.
 */
void
mFormat (char *string)
{
  char      wordBuf[MAXWORD];
  int       i;

  for (i = 0; string[i];)
    {
      i = getWord (wordBuf, string, i, MAXWORD);
      putWord (wordBuf);
    }
}

/*
 * getWord()
 *
 * This function fetches one word from current message.
 */
int
getWord (char *dest, char *source, int offset, int lim)
{
  int       i,
            j;

/* skip leading blanks if any */
  for (i = 0; source[offset + i] == ' ' && i < lim - 1; i++) ;

/* step over word */
  for (;

       source[offset + i] != ' ' &&
       i < lim - 1 &&
       source[offset + i] != 0;

       i++
    ) ;

  if (source[offset + i - 1] != '\n')

/* pick up any trailing blanks */
    for (; source[offset + i] == ' ' && i < lim - 1; i++) ;

/* copy word over */
  for (j = 0; j < i; j++)
    dest[j] = source[offset + j];
  dest[j] = 0;  /* null to tie off string */

  return (offset + i);
}

/*
 * putWord()
 *
 * This function writes one word to modem & console.
 */
void
putWord (char *st)
{
  char     *s;
  int       newColumn;
  static char prevChar = 0;

  for (newColumn = crtColumn, s = st; *s; s++)
    {
      if (*s != TAB)
        {
          if (*s == '\b')
            newColumn--;
          else
            ++newColumn;
        }
      else
        while (++newColumn % 8) ;
    }
  if (newColumn > termWidth)
    {
      fprintf (upfd, "\n");
      crtColumn = 1;
    }

  for (; *st; st++)
    {

      if (*st != TAB)
        {
          if (*st == '\b')
            crtColumn--;
          else
            ++crtColumn;
        }
      else
        while (++crtColumn % 8) ;

 /* worry about words longer than a line:        */
      if (crtColumn > termWidth)
        {
          fprintf (upfd, "\n");
          crtColumn = 1;
        }

      if (*st == '\n' && EOP)
        {
          fprintf (upfd, "\n");
          crtColumn = 1;
        }
      else if (prevChar != NEWLINE || (*st > ' '))
        {
          putc (*st, upfd);
          prevChar = *st;
          if (*st > ' ')
            EOP = FALSE;
        }
      else
        {
          fprintf (upfd, "\n");
          crtColumn = 1;
          if (*st == '\n' && !EOP)
            {
              fprintf (upfd, "\n");
              crtColumn = 1;
            }
          else
            putc (*st, upfd), prevChar = *st;
          EOP = TRUE;
        }
    }
}

void
netResult (char *msg)
{
  printf ("%s\n", msg);
}


void      Do_Stack_Check (void);

/**
  Check Routing Address

  This function will check to see if we have a node address
  that is not ourselve, or NULL.
**/

char Check_Address(label mbaddr )
  {
  char new[130];
  char ours[130];
  int i;
  if( (i=strlen(mbaddr)) == 0 ) return FALSE;
  /**
    Check to see if the node address is really ourselves
    or an alias...
  **/
  Fix_Name(new,  mbaddr,1);
  Fix_Name(ours, (char *)&cfg.codeBuf[cfg.nodeName],1);
  if( strncmp(new, ours, strlen(ours)) == 0 )return FALSE;
  return TRUE;
  }

