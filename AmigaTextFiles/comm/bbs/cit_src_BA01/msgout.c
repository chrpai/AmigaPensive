/*
*                               msgout.c
*
* External Message writer.  For use with external OtherNet parsers.
*/
/*
*                               history
*
* 92Jan17 HAW  1.4 - Hard-wire room name to msgs; handle domain field.
* 91Mar26 HAW  1.3 - Virtual rooms.
* 89Sep25 HAW  1.2 - update for Route Mail.
* 88Nov17 HAW  Created.
*/
#include <stdio.h>
#include <string.h>
#include "ctdl.h"
#include "2ndfmt.h"
#include "stdarg.h"
#include "dos.h"
#include "math.h"
/*
*                               contents
*
*/
#define TITLE           "C86Net Message Exporter "
#define NO_ERROR        0
#define BAD_ARGS        1
#define BAD_TABLE       2
#define NO_NODE         3
#define FATAL           4
#define BAD_OUT_FILE    5
#define LF_ERROR        6

char onConsole=1, remoteSysop;

long char_in, char_out;         /* send/recieve character counts*/
long start_time;                /* total time of session */


FILE            *outfile;
FILE            *netLog=stdout;
extern FILE *upfd;
extern CONFIG      cfg;                 /* Configuration variables      */
extern MessageBuffer msgBuf;    /* The -sole- message buffer    */
extern aRoom       roomBuf;             /* Room buffer */
extern logBuffer   logBuf;
extern FILE        *roomfl, *logfl;
extern int         thisRoom;    /* Current room    */
extern rTable      *roomTab;
extern struct mBuf mFile1, mFile2;
extern SListBase Serves;
extern NetTable    *netTab;
extern NetBuffer   netBuf, netTemp;
extern FILE        *netfl;
extern LogTable    *logTab;
extern FILE        *msgfl, *msgfl2;
FILE       *GlobalFd, *netMisc;
static int RCount, SCount;
extern int         thisNet;        /* Current node in use          */
char *R_SH_MARK =  "&&";
char *NON_LOC_NET= "%%";
char *LOC_NET =    "++";
char inNet = ANYTIME_NET;

void NetField(int field, char *value);
void MsgOutGenInit(void);
void MODoVirtuals(void);
void Process(void);
int  FindNet(label nm);
int ThrowAll(int which, char *distance, MSG_NUMBER start, MSG_NUMBER end,
char *room);
void UtilRoomSend(int rover, char *send1, char *send2, char *send3);
void HandleMessage(char *addr1, char *addr2, char *addr3);
int  RoutePath(char *rp, char *str);
char FindMessage(SECTOR_ID loc, MSG_NUMBER id);
void NetFormat(void);
void NowRouteMail(void);
void MoutCC(char *);
void MoutForeign(char *);
int ReadRoutedDest(int c);
int ReadRouted(void);
void Intel32ToMotorola(UNS_32 *);

int  mPrintf(char *format, ...) {return 0; }  /* stub to quiet the linker */

/**
  some statistics
**/
int total_msgs = 0;  /* total messages output */

/*
* crashout()
*
* Crash exit handler.
*/
void crashout(str)
char *str;
  {
  printf(str);
  writeSysTab();
  exit(FATAL);

  }
/*
* main()
*
* Main manager
*/
int  main(int, char **);
int  main(argc, argv)
char **argv;
int  argc;
  {
  extern char *WRITE_ANY;
  printf("%s %s\n%s\n\n", TITLE, VERSION_NAME, COPYRIGHT);
  /* not enough arguments?  Explain. */
  if (argc < 3)
    {
    printf("usage: MSGOUT nodename file\n");
    exit(BAD_ARGS);

    }
  char_in = char_out = 0;
  start_time = Set_Timer(0);

  cfg.weAre = UTILITY;
  if (!readSysTab(TRUE, TRUE))
    {
    exit(BAD_TABLE);

    }
  if (access(LOCKFILE, 0) != ERROR)
    {
    printf("Please do not run MsgOut using Outside Commands.\n");
    exit(LF_ERROR);

    }
  MsgOutGenInit();
  VirtInit();
  if (FindNet(argv[1]) == ERROR)
    {
    writeSysTab();
    printf("Could not find node %s.\n", argv[1]);
    exit(NO_NODE);

    }
  if ((outfile = fopen(argv[2], WRITE_ANY)) == NULL)
    {
    writeSysTab();
    printf("Couldn't open output file %s.\n", argv[2]);
    exit(BAD_OUT_FILE);

    }
  Process();
  putNet(thisNet, &netBuf);
  writeSysTab();
  printf(" %d total messages output for %s\n", total_msgs, argv[1]);
  Compute_Data(netBuf.netName);
  return 0;
  }
/*
* MsgOutGenInit()
*
* This handles general initialization.
*/
void MsgOutGenInit()
  {
  SYS_FILE fn;
  initNetBuf(&netBuf);
  initNetBuf(&netTemp);
  makeSysName(fn, "ctdlnet.sys", &cfg.netArea);
  openFile(fn, &netfl);
  initRoomBuf(&roomBuf);
  makeSysName(fn, "ctdlroom.sys", &cfg.roomArea);
  openFile(fn, &roomfl);
  InitMsgBase();

  }
/*
* Process()
*
* This is the main processor.
*/
void Process()
  {
  char          *send1, *send2, *send3;
  label         temp;
  SYS_FILE              fn;
  FILE          *mail;
  int                   rover;
  extern char           *READ_ANY;
  struct netMLstruct    buf;
  /* first we handle Mail>. */
  if (netBuf.nbflags.normal_mail)
    {
    sPrintf(temp, "%d.ml", thisNet);
    makeSysName(fn, temp, &cfg.netArea);
    if ((mail = fopen(fn, READ_ANY)) == NULL)
      {
      printf("WARNING: Couldn't open %s for mail delivery to %s.\n",
      fn, netBuf.netName);

      }
    else
      {
      while (getMLNet(mail, buf))
      if (FindMessage(buf.ML_loc, buf.ML_id))
        {
        strCpy(msgBuf.mbroom, "Mail");
        NetFormat();

        }
      fclose(mail);
      unlink(fn);               /* kill mail file */

      }
    netBuf.nbflags.normal_mail = FALSE;

    }
  NowRouteMail();               /* now handle any route mail */
  /* now we handle the shared rooms */
  for (rover = 0; rover < SHARED_ROOMS; rover++)
    {
    /* if we share this room, check for new msgs. */
    if (isSharedRoom(thisNet, rover) && roomValidate(thisNet, rover))
      {
      getRoom(netRoomSlot(rover));
      send1 = R_SH_MARK;
      send2 = send3 = "guh";
      switch (roomBuf.rbShareType)
        {
        case REG_HOST:
        printf("WARNING: Please do not use Regional Host settings.\n");
        printf("\nThey are obsolete.\n");
        case PEON:
        break;
        case BACKBONE:
        switch (netBuf.netRooms[rover].mode)
          {
          case PEON:
          send2  = NON_LOC_NET;
          break;
          case ACTIVE_BACKBONE:
          case PASS_BACKBONE:
          case REG_HOST:
          send2 = NON_LOC_NET;
          send3 = LOC_NET;
          break;
          default: crashout("shared rooms: #2");

          }
        break;
        default: crashout("shared rooms: #1");

        }
      UtilRoomSend(rover, send1, send2, send3);

      }

    }
  MODoVirtuals();
  UpdVirtStuff();

  }
/*
* UtilRoomSend()
*
* Send stuff out.
*/
void UtilRoomSend(rover, send1, send2, send3)
int rover;
char *send1, *send2, *send3;
  {
  int i;
  for (i = 0; i < MSGSPERRM; i++)
    {
    if (roomBuf.msg[i].rbmsgNo > netBuf.netRooms[rover].lastMess)
      {
      if (FindMessage(roomBuf.msg[i].rbmsgLoc, roomBuf.msg[i].rbmsgNo))
        {
        strcpy(msgBuf.mbroom, roomBuf.rbname);
        HandleMessage(send1, send2, send3);

        }

      }

    }
  netBuf.netRooms[rover].lastMess = roomTab[thisRoom].rtlastMessage;
  netTab[thisNet].netTRooms[rover].lastMess =
  roomTab[thisRoom].rtlastMessage;

  }
/*
* HandleMessage()
*
* This decides if a message should be sent out.
*/
void HandleMessage(addr1, addr2, addr3)
char *addr1, *addr2, *addr3;
  {
  if ((strncmp(msgBuf.mbaddr, addr1, strLen(addr1)) == SAMESTRING  ||
  strncmp(msgBuf.mbaddr, addr2, strLen(addr2))      == SAMESTRING  ||
  strncmp(msgBuf.mbaddr, addr3, strLen(addr3))      == SAMESTRING) &&
  RoutePath(LOC_NET, msgBuf.mbaddr)     != thisNet &&
  RoutePath(NON_LOC_NET, msgBuf.mbaddr) != thisNet)
    {
    NetFormat();

    }

  }
/*
* RoutePath()
*
* This function returns the number of the node that routed this msg
* to here.  If the msg was not routed in from a BackBone, then
* return ERROR, which will never match another node's #.
* 88Oct13: Now simply check for msg origin, assume if one exists
* that it should be checked.  Don't remember why it is restricted
* to only BACKBONE-routed msgs.  Doesn't seem necessary.
*/
int RoutePath(rp, str)
char *str, *rp;
  {
  if (strncmp(rp, str, strLen(rp)) == SAMESTRING)
    {
    if (strLen(str) != strLen(rp)) /* prevent return of 0 */
    return atoi(str + 2);

    }
  return ERROR;

  }
/*
* FindNet()
*
* This function will find the named node.  Stolen from searchNameNet/NETMISC.
*/
int  FindNet(nm)
label nm;
  {
  int rover;
  for (rover = 0; rover < cfg.netSize; rover++)
    {
    if (netTab[rover].ntflags.in_use &&
    hash(nm) == netTab[rover].ntnmhash)
      {
      getNet(rover, &netBuf);
      if (strCmpU(netBuf.netName, nm) == SAMESTRING)
      return rover;

      }

    }
  return ERROR;

  }
/*********** These functions stolen & modified from MSG.C ***************/
/*
* FindMessage()
*
* This gets all set up to do something with a message.  We use this rather
* than the findMessage in libmsg.c so we can automatically read in the 'M'
* field.
*/
char FindMessage(loc, id)
SECTOR_ID  loc;         /* sector in message.buf */
MSG_NUMBER id;          /* unique-for-some-time ID# */
  {
  MSG_NUMBER here;
  startAt(msgfl, &mFile1, loc, 0);
  do
    {
    getMessage(getMsgChar, FALSE, TRUE, TRUE);
    here = atol(msgBuf.mbId);

    }
  while (here != id &&  mFile1.thisSector == loc);
  return (char) ((here == id));

  }
/*
* NetFormat()
*
* This function writes a message to disk.
*/
void NetFormat()
  {
  MSG_NUMBER val;
  if (!msgBuf.mborig[0])
  strCpy(msgBuf.mborig, cfg.nodeId + cfg.codeBuf);
  if (!msgBuf.mboname[0])
  strCpy(msgBuf.mboname, cfg.nodeName + cfg.codeBuf);
  if (!msgBuf.mbsrcId[0])
    {
    val = atol(msgBuf.mbId);
    sPrintf(msgBuf.mbsrcId, "%ld %ld",
    (val & 0xffff0000) >> 16, val & 0xffffl);

    }
  if (msgBuf.mbauth[0])   NetField('A', msgBuf.mbauth);
  if (msgBuf.mbdate[0])   NetField('D', msgBuf.mbdate);
  if (msgBuf.mbtime[0])   NetField('C', msgBuf.mbtime);
  if (msgBuf.mboname[0])  NetField('N', msgBuf.mboname);
  if (msgBuf.mbdomain[0]) NetField('X', msgBuf.mbdomain);
  if (msgBuf.mborig[0])   NetField('O', msgBuf.mborig);
  if (msgBuf.mbroom[0])   NetField('R', msgBuf.mbroom);
  if (msgBuf.mbsrcId[0])  NetField('S', msgBuf.mbsrcId);
  if (msgBuf.mbto[0])     NetField('T', msgBuf.mbto);
  if (msgBuf.mbOther[0])  NetField('P', msgBuf.mbOther);
  RunList(&msgBuf.mbCC, MoutCC);
  RunList(&msgBuf.mbForeign, MoutForeign);
  NetField('M', msgBuf.mbtext);
  total_msgs++;
  }
/*
* NetField()
*
* Work function to write out a field and its identifier.
*/
void NetField(field, value)
char *value;
int  field;
  {
  char_out += ( 2 + strlen(value));
  fprintf(outfile, "%c%s", field, value);
  putc(0, outfile);

  }
/*
* MoutCC()
*
* This handles the CC field of a message.
*/
void MoutCC(dd)
char *dd;
  {
  NetField('W', dd);

  }
/*
* MoutForeign()
*
* This handles the Foreign fields of a message.
*/
void MoutForeign(dd)
char *dd;
  {
  NetField(dd[0], dd + 1);

  }
static int  RWorkBuf[7];
/*
* NowRouteMail()
*
* This function handles outgoing route mail.
*/
void NowRouteMail()
  {
  int           rover;
  label temp;
  SYS_FILE      fn;
  extern char *READ_ANY, OverRides;
  if (!netBuf.nbflags.HasRouted)
  return;
  for (rover = 0; rover <= netBuf.nbHiRouteInd; rover++)
    {
    sPrintf(temp, "R%d.%d", thisNet, rover);
    makeSysName(fn, temp, &cfg.netArea);
    if ((netMisc = safeopen(fn, READ_ANY)) != NULL)
      {
      getMsgStr(getNetChar, temp, NAMESIZE);
      getMsgStr(getNetChar, temp, NAMESIZE);
      StartDecode(ReadRoutedDest);
      RCount = SCount = 0;
      while (getMessage(ReadRouted, TRUE, TRUE, TRUE))
        {
        strCpy(msgBuf.mbroom, "Mail");
        NetFormat();

        }
      fclose(netMisc);
      unlink(fn);

      }

    }
  netBuf.nbflags.HasRouted = FALSE;
  netBuf.nbHiRouteInd        = 0;

  }
/*
* ReadRoutedDest()
*
* A work function to read encrypted data.
*/
int ReadRoutedDest(int c)
  {
  RWorkBuf[RCount++] = c;
  return TRUE;

  }
/*
* ReadRouted()
*
* This function will read a routed char for getMessage().
*/
int ReadRouted()
  {
  int c;
  if (RCount != SCount)
  return RWorkBuf[SCount++];
  RCount = SCount = 0;
  while (SCount == RCount && (c = fgetc(netMisc)) != EOF)
  Decode(c);
  if (RCount != SCount)
  return RWorkBuf[SCount++];
  if (c == EOF) StopDecode();
  if (RCount != SCount)
  return RWorkBuf[SCount++];
  return -1;

  }
/*
* getNetChar()
*
* This function gets a character from a network temporary file.
*/
int getNetChar()
  {
  int c;
  c = fgetc(netMisc);
  if (c == EOF) return -1;
  return c;

  }
#define WeServe(x)      SearchList(&Serves, x)
/*
* LocalName()
*
* This takes a string of form <system> _ <domain> and attempts to discover if
* this domain mapped system is actually a local.  This is used when we're
* sending mail and are trying to find out if a Who Else override needs to be
* generated.  Ugly kludge, but, hey, that's what programming's all about, eh?
*/
char *LocalName(char *system)
  {
  char *domain, *System;
  if ((domain = strchr(system, '_')) == NULL) return system;
  domain += 2;  /* always preceded by a space -- or so we assume */
  if (strCmpU(domain, cfg.codeBuf + cfg.nodeDomain) == SAMESTRING ||
  WeServe(domain) != NULL)
    {
    System = strdup(system);
    if ((domain = strchr(System, ' ')) == NULL)
    return system;      /* should never happen, though */
    *domain = NULL;
    if (searchNameNet(System, &netTemp) != ERROR)
      {
      free(System);
      return netTemp.netName;

      }
    free(System);

    }
  return system;

  }
/*
* SepNameSystem()
*
* This will parse an Other Recipient spec.
*/
char SepNameSystem(char *string, char *person, char *system, NetBuffer *buf)
  {
  char  *c;
  label domain;
  char dup, work[150];          /* should be sufficient */
  int   slot;
  strCpy(work, string);
  if ((c = strchr(work, '@')) == NULL)
    {
    if (strLen(work) >= NAMESIZE) return BAD_FORMAT;
    strCpy(person, string);
    return NOT_SYSTEM;

    }
  *c++ = 0;
  NormStr(work);
  NormStr(c);
  if (strLen(c) >= NAMESIZE * 2 || strLen(work) >= NAMESIZE)
  return BAD_FORMAT;
  strCpy(system, c);
  strCpy(person, work);
  if (buf == NULL) return IS_SYSTEM;    /* very minor cheat - see CTDL.C */
  if ((slot = searchNameNet(c, buf)) != ERROR)
    {
    /* try secondary lists */
    strCpy(system, buf->netName);       /* get "real" name */
    if (buf->nbflags.local)
      {
      return IS_SYSTEM;

      }

    }
  if (SystemInSecondary(c, domain, &dup))
    {
    if (dup)
      {
      /* oops */
      return (char) ( (slot == ERROR) ? NO_SYSTEM : IS_SYSTEM );

      }
    if (strCmpU(domain, cfg.nodeDomain + cfg.codeBuf) == SAMESTRING &&
    (strCmpU(c, cfg.nodeName + cfg.codeBuf) == SAMESTRING ||
    strCmpU(c, UseNetAlias(cfg.nodeName+cfg.codeBuf, TRUE))
    == SAMESTRING))
      {
      printf("Hey, that's this system!\n ");
      return (char)SYSTEM_IS_US;

      }
    sPrintf(system, "%s _ %s", c, domain);
    return (char)IS_SYSTEM;

    }
  return (char)( (slot == ERROR) ? NO_SYSTEM : IS_SYSTEM);

  }
static label SearchResult;
static char  *SearchTarget, GetAlias;
/*
* UseNetAlias()
*
* This will find a usenet alias or the converse.
*/
char *UseNetAlias(char *Name, char FindAlias)
  {
  void *EatTrans();
  SListBase Dummy =
    {
    NULL, NULL, NULL, NULL, EatTrans

    };
  SYS_FILE fn;
  char *c, *WorkName;
  WorkName = strdup(Name);      /* use a work buffer */
  SearchResult[0] = 0;
  SearchTarget = WorkName;
  if (!FindAlias) while ((c = strchr(WorkName, ' ')) != NULL) *c = '_';
  makeSysName(fn, "aliases.sys", &cfg.roomArea);
  GetAlias = FindAlias;
  MakeList(&Dummy, fn, NULL);   /* CHEAT!  WHEEEEEE! */
  free(WorkName);
  if (strLen(SearchResult) == 0) return Name;
  if (FindAlias) while ((c = strchr(SearchResult, '_')) != NULL) *c = ' ';
  return SearchResult;

  }
/*
* EatTrans()
*
* This will eat a line of input for alias processing.
*/
void *EatTrans(char *line)
  {
  char *c;
  if ((c = strchr(line, ' ')) != NULL)
    {
    *c = 0;
    if (GetAlias)
      {
      /* check second field */
      if (strCmpU(c + 1, SearchTarget) == SAMESTRING)
        {
        strCpy(SearchResult, line);

        }

      }
    else
      {
      /* check first field */
      if (strCmpU(line, SearchTarget) == SAMESTRING)
        {
        strCpy(SearchResult, c + 1);

        }

      }

    }
  return NULL;

  }
/*
* SearchSecondary()
*
* This searches a secondary (domain) list for a system.
*/
static char SearchSecondary(char *secondary,char *Name,char *Domain,char *isdup)
  {
  FILE *fd;
  int  bucket;
  char found, *tab, *c, *tab2;
  char line[90];
  JumpInfo JumpTable[BUCKETCOUNT];
  if ((fd = fopen(secondary, READ_ANY)) == NULL)
  return FALSE;
  fread(line, VERS_SIZE + 1, 1, fd);
  fread(JumpTable, sizeof JumpTable, 1, fd);
  #ifdef IS_MOTOROLA
  for (bucket = 0; bucket < BUCKETCOUNT; bucket++)
  Intel32ToMotorola(&JumpTable[bucket].offset);
  #endif
  bucket = (isdigit(Name[0])) ? Name[0] - '0' :
  toUpper(Name[0]) - 'A' + 10;
  fseek(fd, JumpTable[bucket].offset, 0);
  found = FALSE;
  do
    {
    *isdup = FALSE;
    if (fgets(line, sizeof line, fd) == NULL) break;
    if ((tab2 = strchr(line, '\n')) != NULL)
    *tab2 = 0;
    if (strlen(line) == 0)
      {
      break;

      }
    if (line[0] <= ' ')
      {
      switch (line[0])
        {
        case DUP:
        *isdup = TRUE;
        break;
        default: printf("Ooop!");
        break;

        }
      c = line + 1;

      }
    else c = line;
    tab = strchr(c, '\t');
    *tab++ = 0;
    if (strCmpU(c, Name) == 0)
    found = TRUE;
    if (strCmpU(c, Name) > 0) break;

    }
  while (!found);
  if (found)
    {
    if ((tab2 = strchr(tab, '\t')) != NULL)
    *tab2++ = 0;
    strCpy(Domain, tab);
    if (tab2 != NULL)    /* alias?  Copy it into search string */
    strCpy(Name, tab2);

    }
  fclose(fd);
  return found;

  }
/*
* SystemInSecondary()
*
* This will look for a system in secondary lists.
*/
char SystemInSecondary(char *Name, char *Domain, char *dup)
  {
  int rover;
  char *sep;
  SYS_FILE secondary;
  label name;
  char WorkName[(NAMESIZE * 2) + 1];
  char SearchSecondary(char *secondary,char *Name,char *Domain,char *isdup);
  strCpy(WorkName, Name);
  /* is the domain specified already?  if so, parse it */
  if ((sep = strchr(WorkName, '_')) != NULL ||
  (sep = strchr(WorkName, '.')) != NULL)
    {
    *sep++ = 0;
    NormStr(WorkName);
    NormStr(sep);
    if (strchr(sep, '_') != NULL ||
    strchr(sep, '.') != NULL)
    return FALSE;       /* no subdomains */
    strCpy(Name, WorkName);
    strCpy(Domain, sep);
    *dup = FALSE;               /* by definition */
    return TRUE;

    }
  Domain[0] = 0;
  for (rover = 0; rover < 100; rover++)
    {
    sPrintf(name, "nodes%d.fst", rover);
    makeSysName(secondary, name, &cfg.netArea);
    if (access(secondary, 0) != 0) break;
    if (SearchSecondary(secondary, Name, Domain, dup)) break;

    }
  strCpy(WorkName, Name);
  /* make sure we found something and it's not us */
  return (char)(Domain[0] != 0 &&
  strCmpU(Name, cfg.codeBuf + cfg.nodeName) != SAMESTRING &&
  strCmpU(UseNetAlias(WorkName,FALSE), cfg.codeBuf + cfg.nodeName)
  != SAMESTRING);

  }
extern VirtualRoom *VRoomTab;
extern VirtNet     *VirtNetList;
extern char VirtualInUse;
extern int  VirtSize, VNetSize;
/*
* MODoVirtuals()
*
* This sends rooms to another system, if needed.
*/
void MODoVirtuals()
  {
  int rover, x;
  if (!VirtualInUse) return ;
  for (rover = 0; rover < VIRT_LIMIT; rover++)
    {
    x = VirtNetList[thisNet].VirtList[rover].WhichVirt;
    if (x >= VirtSize || x < 0 || !VRoomInuse(x))
    VirtNetList[thisNet].VirtList[rover].WhichVirt = -1;
    if (VirtNetList[thisNet].VirtList[rover].WhichVirt != -1)
      {
      SendVirtual(rover, NULL, NULL, NULL);

      }

    }

  }
/*
* SendVirtual()
*
* This manages sending a room to another system.
*/
int SendVirtual(int VirtIndex, char *d1, char *d2, char *d3)
  {
  int   VirtNo, count;
  MSG_NUMBER StartMsg;
  VirtNo = VirtNetList[thisNet].VirtList[VirtIndex].WhichVirt;
  /* Send all the new LD messages received */
  StartMsg = VirtNetList[thisNet].VirtList[VirtIndex].LDSent;
  count = ThrowAll(VirtNo, LD_DIR, StartMsg, VRoomTab[VirtNo].vrHiLD,
  VRoomTab[VirtNo].vrName);
  VirtNetList[thisNet].VirtList[VirtIndex].LDSent =
  VRoomTab[VirtNo].vrHiLD;
  if (VirtNetList[thisNet].VirtList[VirtIndex].mode != PEON)
    {
    StartMsg = VirtNetList[thisNet].VirtList[VirtIndex].LocSent;
    count += ThrowAll(VirtNo, LOCAL_DIR, StartMsg,
    VRoomTab[VirtNo].vrHiLocal, VRoomTab[VirtNo].vrName);
    VirtNetList[thisNet].VirtList[VirtIndex].LocSent =
    VRoomTab[VirtNo].vrHiLocal;

    }
  VRoomTab[VirtNo].vrChanged |= SENT_DATA;
  return count;

  }
/*
* ThrowAll()
*
* This sends a virtual room to another system.
*/
int ThrowAll(int which, char *distance, MSG_NUMBER start, MSG_NUMBER end,
char *room)
  {
  MSG_NUMBER  rover;
  int           count=0;
  char  fn[100];
  extern char *READ_ANY;
  extern PROTO_TABLE Table[];
  extern int    TransProtocol;
  for (rover = start + 1; rover <= end; rover++)
    {
    CreateVAName(fn, which, distance, rover);
    if ((netMisc = safeopen(fn, READ_ANY)) != NULL)
      {
      while (getMessage(getNetChar, TRUE, TRUE, TRUE))
        {
        count++;
        strCpy(msgBuf.mbroom, room);
        NetFormat();

        }
      fclose(netMisc);

      }

    }
  return count;

  }
void Intel32ToMotorola(UNS_32 *val)
  {
  unsigned long temp;
  temp = *val;
  *val = ((ULONG)(temp & 0xff) << 24) + ((ULONG)(temp & 0xff00) << 8) +
  ((ULONG)(temp & 0xff0000) >> 8) + ((ULONG)(temp &0xff000000) >> 24);

  }
