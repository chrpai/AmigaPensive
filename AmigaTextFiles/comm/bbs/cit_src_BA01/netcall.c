/*
*       netcall.c
*
* Networking call functions.
*/
/*
*       history
*
* 86Aug20 HAW  History not maintained due to space problems.
*/
#include "ctdl.h"
#include "math.h"
/*
*       contents
*
*/
char                    inReceive;
NetInfo     NetStyle;
extern char   RecMassTransfer;
extern char             *SR_Sent;
extern char             *pollCall;
extern FILE             *netMisc;
extern FILE             *netLog;
extern AN_UNSIGNED      RecBuf[SECTSIZE + 5];
extern int              counter;
extern int              callSlot;
extern label            callerName, callerId;
char                    checkNegMail;
extern char             processMail;
extern char   MassTransferSent;
char             normId(), getNetMessage();
AN_UNSIGNED      inp();
extern CONFIG    cfg;            /* Lots an lots of variables    */
extern logBuffer logBuf;         /* Person buffer                */
extern logBuffer logTmp;         /* Person buffer                */
extern aRoom     roomBuf;        /* Room buffer                  */
extern rTable    *roomTab;
extern PROTO_TABLE Table[];
extern int TransProtocol;
extern MessageBuffer   msgBuf;
extern NetBuffer netBuf, netTemp;
extern NetTable  *netTab;
/* extern SListBase FwdAliasii; */
extern int       thisNet;
extern char      onConsole;
extern char      loggedIn;       /* Is we logged in?             */
extern char      outFlag;        /* Output flag                  */
extern char      haveCarrier;    /* Do we still got carrier?     */
extern char      modStat;        /* Needed so we don't die       */
extern char      TrError;
extern int       thisRoom;
extern char      netDebug;
extern char      logNetResults;
extern char  *DomainFlags;
extern long ByteCount, EncCount;
char *chMailTemplate = "chkMail.$$$";
extern long  char_in, char_out; /* send/recieve character counts*/
long start_time;                /* total time of session */
/*
* caller()
*
* This function is called when we've been called and have to handle the
* caller.  We have to stabilize the call and then manage all requests
* the caller makes of us.
*/
void caller()
  {
  ITL_InitCall();             /* initialize the ITL layer */
  RecMassTransfer = FALSE;
  memset(SR_Sent, 0, SHARED_ROOMS);
  inReceive = FALSE;
  processMail = FALSE;
  checkNegMail = FALSE;
  SpecialMessage("Status:Net Carrier");
  if( logNetResults && netDebug )splitF(netLog, "Have Carrier\n");
  caller_stabilize();
  if (!haveCarrier) return ;  /* Abort */
  if( logNetResults && netDebug )splitF(netLog, "Stabilized\n");
  SpecialMessage("Status:Net Session");
  char_in = char_out = 0;
  start_time = Set_Timer(0);
  sendId();
  if (!haveCarrier) return ;  /* Abort */
  if (!netBuf.nbflags.MassTransfer)
  ITL_optimize(TRUE);   /* try for better protocol */
  sendStuff(FALSE, TRUE);
  startTimer(WORK_TIMER);
  while (gotCarrier() && chkTimeSince(WORK_TIMER) < 10) ;
  killConnection();
  if( logNetResults )splitF(netLog, "\nFinished with %s @%s\n", netBuf.netName, Current_Time());
  Compute_Data(netBuf.netName);
  doResults();
  SpecialMessage("Status:Net Completed");

  }
/*
* sendStuff()
*
* This function handles being the sender of information (sending role).
*/
void sendStuff(char reversed, char SureDoIt)
  {
  extern int RouteToDirect;
  if (SureDoIt && callSlot != ERROR)
    {
    MassTransferSent = FALSE;
    SendPwd();
    if (!haveCarrier) return ;  /* Abort */
    if (netBuf.nbflags.HasRouted)    RouteOut();
    if (!haveCarrier) return ;  /* Abort */
    if (!netBuf.nbflags.Stadel) DomainOut(FALSE);
    if (!haveCarrier) return ;  /* Abort */
    if (netBuf.nbflags.normal_mail ||
    RouteToDirect != -1 ||
    (netBuf.nbflags.Stadel && DomainFlags[thisNet]))
      {
      sendMail();
      if (!haveCarrier) return ;  /* Abort */
      checkMail();

      }
    if (!haveCarrier) return ;  /* Abort */
    if (!HasPriorityMail(thisNet))
      {
      if (netBuf.nbflags.room_files)
      askFiles();
      if (!haveCarrier) return ;  /* Abort */
      sendSharedRooms();
      if (!haveCarrier) return ;  /* Abort */
      if (netBuf.nbflags.send_files)
      doSendFiles();
      if (!haveCarrier) return ;  /* Abort */
      roleReversal(reversed);

      }

    }
  sendHangUp();
  pollCall[thisNet]--;        /* Don't set polled flag unless stable call */

  }
/*
* SendPwd()
*
* This function sends the system password if necessary.
*/
void SendPwd()
  {
  struct cmd_data cmds;
  if (netBuf.TheirPwd[0] != 0)
    {
    /* only send if need to -- gets */
    zero_struct(cmds);              /* us around a bug in pre net 1.10*/
    strCpy(cmds.fields[0], netBuf.TheirPwd);        /* versions     */
    cmds.command = SYS_NET_PWD;
    sendNetCommand(&cmds, "system pwd");

    }

  }
/*
* roleReversal()
*
* This function handles the role reversal request.
*/
void roleReversal(char reversed)
  {
  struct cmd_data cmds;
  if (reversed) return ;
  if (!netBuf.nbflags.local && !netBuf.nbflags.spine) return ;
  if( logNetResults && netDebug )splitF(netLog, "Reversing roles\n");
  zero_struct(cmds);
  cmds.command = ROLE_REVERSAL;
  if (!sendNetCommand(&cmds, "role reversal"))
  return;
  rcvStuff(TRUE);
  pause(50);   /* wait a second */
  if (gotCarrier())
  reply(GOOD, "");/* this replies GOOD to the HANGUP terminating the */
  /* role reversal.  NOTE: STadel doesn't follow the */
  /* spec in this regard - it just dumps carrier     */

  }
/*
* caller_stabilize()
*
* This function tries to stabilize the call -- baud is already set.
*/
void caller_stabilize()
  {
  int tries, x1, x2, x3;
  extern char hst;
  /* regrettable initialization */
  for (tries = 0; tries < SHARED_ROOMS; tries++) resetNeedsProcessing(tries);
  putNet(thisNet, &netBuf);
  pause(50);                                  /* delay for a little bit */
  while (MIReady())   inp();                  /* Clear garbage        */
  startTimer(USER_TIMER);                     /* this is safe */
  x3 = 0;
  for (tries = 0; (chkTimeSince(USER_TIMER) < 20l || tries < 40) &&
  gotCarrier() ; tries++)
    {
    outMod(7);
    outMod(13);
    outMod(69);
    for (  startTimer(WORK_TIMER); chkTimeSince(WORK_TIMER) < 2l && !MIReady();) ;
    if (MIReady())
      {
      x1 = receive(2);
      x2 = receive(2);
      if (x2 != ERROR) x3 = receive(2);
      if (x1 == 248 && x2 == 242 && x3 == 186)
        {
        outMod(ACK);
        /* ok, we've seen at high speed some overrun problems, so ... */
        do
          {
          x1 = receive(2);

          }
        while (x1 == 248 || x1 == 242 || x1 == 186);
        ModemPushBack(x1);
        return;

        }
      else
        {
        if ( ( x1 == 242 || x1 == 186 || x1 == 248 )
        || ( x2 == 242 || x2 == 186 || x2 == 248 )
        || ( x3 == 242 || x3 == 186 || x3 == 248 ) )  /* all combinations */
          {
          /* real close, so let's catch our breath */
          while (receive(1) != ERROR) ;

          };

        }

      }
    else
      {
      tries ++;  /* if we get nothing back, don't try as much, try half the number of times */

      }

    }
  if( logNetResults && netDebug )splitF(netLog, "Call not stabilized:tries=%d \n",tries);
  killConnection();

  }
/*
* sendId()
*
* This function sends ID to the receiver.
*/
void sendId()
  {
  if (!ITL_Send(STARTUP))
    {
    no_good("Couldn't transfer ID to %s at startup!", TRUE);
    return;

    }
  mTrPrintf("%s", cfg.codeBuf + cfg.nodeId  );
  mTrPrintf("%s", cfg.codeBuf + cfg.nodeName);
  if (!ITL_Send(FINISH))
    {
    no_good("Couldn't transfer ID to %s at finish!", TRUE);
    return;

    }

  }
/*
* sendMail()
*
* This function sends normal mail to receiver.
*/
void sendMail()
  {
  struct cmd_data cmds;
  int             nor_mail;
  extern int      RouteToDirect;
  if (!gotCarrier())
    {
    modStat = haveCarrier = FALSE;
    return ;

    }
  if( logNetResults && netDebug )splitF(netLog, "Sending Mail ");
  zero_struct(cmds);
  cmds.command = NORMAL_MAIL;
  if (!sendNetCommand(&cmds, "normal mail"))
  return;
  if (!ITL_SendMessages())
    {
    no_good("Couldn't start Mail transfer to %s!", TRUE);
    killConnection();
    return;

    }
  nor_mail = s_m_n();            /* Send normal mail     */
  if (gotCarrier())
  nor_mail += SendRoutedAsLocal();
  if (gotCarrier() && netBuf.nbflags.Stadel)
  nor_mail += DomainOut(TRUE);
  ITL_StopSendMessages();
  RouteToDirect = -1;         /* This is just in case */
  if (gotCarrier())
    {
    if( logNetResults && netDebug )splitF(netLog, "(%d) (%ld => %ld bytes)\n",nor_mail,EncCount,ByteCount);
    netBuf.nbflags.normal_mail = FALSE;

    }

  }
/*
* checkMail()
*
* This function handles negative acknowledgement on netMail.
*/
void checkMail()
  {
  struct cmd_data cmds;
  SYS_FILE fileNm;
  extern char *WRITE_ANY;
  if (!gotCarrier())
    {
    return;

    }
  if( logNetResults && netDebug )splitF(netLog, "Check mail\n");
  makeSysName(fileNm, chMailTemplate, &cfg.netArea);
  zero_struct(cmds);
  cmds.command = CHECK_MAIL;
  if (!sendNetCommand(&cmds, "check mail"))
    {
    return;

    }
  if (ITL_Receive(fileNm, FALSE, TRUE, putFLChar, fclose) == ITL_SUCCESS)
  checkNegMail = TRUE;        /* Call readNegMail() later */

  }
/*
* readNegMail()
*
* This function reads and processes negative acks.
*/
void readNegMail(char talk)
  {
  label author, target, context;
  int whatLog;
  int  sigChar;
  SYS_FILE fileNm;
  extern char *READ_ANY;
  makeSysName(fileNm, chMailTemplate, &cfg.netArea);
  if ((netMisc = safeopen(fileNm, READ_ANY)) == NULL)
    {
    if (talk) no_good("Couldn't open negative ack file from %s.", FALSE);
    return ;

    }
  getRoom(MAILROOM);
  sigChar = fgetc(netMisc);
  while (sigChar != NO_ERROR && sigChar != EOF && sigChar != EOF)
    {
    ZeroMsgBuffer(&msgBuf);
    strCpy(msgBuf.mbauth, "Citadel");
    getMsgStr(getNetChar, author, NAMESIZE);
    getMsgStr(getNetChar, target, NAMESIZE);
    getMsgStr(getNetChar, context, NAMESIZE);
    switch (sigChar)
      {
      case NO_RECIPIENT:
      strCpy(msgBuf.mbto, author);
      if ((whatLog = PersonExists(author)) >= 0 &&
      whatLog < cfg.MAXLOGTAB)
        {
        sPrintf(msgBuf.mbtext,
        "Your netMail to '%s' (%s) failed because there is no such recipient on %s.",
        target, context, callerName);
        putMessage(&logBuf);
        break;

        }
      case UNKNOWN:
      ZeroMsgBuffer(&msgBuf);
      sPrintf(msgBuf.mbtext,
      "Unknown problems with netMail: author=-%s-, target=-%s-, context=-%s-.  System was %s.",
      author, target, context, netBuf.netName);
      netResult(msgBuf.mbtext);
      break;
      case BAD_FORM:
      sPrintf(msgBuf.mbtext, "Bad netMail sent to %s.", callerName);
      netResult(msgBuf.mbtext);
      break;
      default:
      sPrintf(msgBuf.mbtext, "Bad sigChar=%d.", sigChar);
      netResult(msgBuf.mbtext);
      break;

      }
    sigChar = fgetc(netMisc);

    }
  fclose(netMisc);
  unlink(fileNm);

  }
/*
* sendSharedRooms()
*
* This sends all shared rooms to receiver.
*/
static int SendRoom(SharedRoom *room, int system, int index, int roomslot,void *d);
void sendSharedRooms()
  {
  SendFastTransfer();
  EachSharedRoom(thisNet, SendRoom, SendVirtualRoom, NULL);

  }
/*
* EachSharedRoom()
*
* This does something for each shared room.
*/
void EachSharedRoom(int system,
int (*func)(SharedRoom *room, int system, int index, int roomslot, void *d),
int (*virtfunc)(VirtualRoom *room, int sys, int index, int which, void *d),
void *data)
  {
  int rover;
  if( cfg.BoolFlags.debug)
    splitF(NULL, "EachSharedRoom(%d,%08lx,%08lx,%08lx)\n",system,func,virtfunc,data);
  if (!netTab[system].ntflags.in_use) return;
  if (func != NULL)
    {
    for (rover = 0; rover < SHARED_ROOMS; rover++)
      {
      if (isSharedRoom(system, rover) && roomValidate(system, rover))
        {
        if ((*func)(netTab[system].netTRooms + rover, system,
        rover, netTabRoomSlot(system, rover), data) == ERROR)
        return;

        }

      }

    }
  if (virtfunc != NULL) DoVirtuals(system, virtfunc, data);

  }
/*
* Addressing()
*
* This function is responsible for deciding what sort of addressing or routing
* flags should be checked for, and if the room should be sent if we are in
* a network session.
*/
void Addressing(int system, int index, char *commnd, char **send1, char **send2,
char **send3, char **name, char *doit)
  {
  extern char *R_SH_MARK, *NON_LOC_NET, *LOC_NET;
  /*
  * This is more than just a trivial efficiency.  This routine can be called
  * indirectly by the room editing functions.  If this happens then the
  * getRoom() call would overwrite the roomBuf being used for editing.
  * Therefore, we can only read in roomBuf if we don't have the right one
  * in place, otherwise we lose what we just changed.
  */
  if (thisRoom != netTabRoomSlot(system, index))
  getRoom(netTabRoomSlot(system, index));
  *doit = TRUE;
  *send1 = R_SH_MARK;
  *send2 = *send3 = "guh";
  switch (roomBuf.rbShareType)
    {
    case PEON:
    *commnd = NET_ROOM;
    *name = "Peon";
    break;
    case REG_HOST:          /* obsolete */
    case BACKBONE:
    switch (CGetMode(netTab[system].netTRooms[index].mode))
      {
      case PEON:
      *commnd = NET_ROOM;
      *send2 = NON_LOC_NET;
      *name = "Peon";
      break;
      case ACTIVE_BACKBONE:
      case REG_HOST:
      if (netTab[system].ntflags.local)
      *commnd = NET_ROOM;
      else
      *commnd = NET_ROUTE_ROOM;
      *send2 = NON_LOC_NET;
      *send3 = LOC_NET;
      *name = "Backbone - Active";
      break;
      case PASS_BACKBONE:
      if (!netTab[system].ntflags.local && !inReceive) *doit = FALSE;
      else if (netTab[system].ntflags.local)
      *commnd = NET_ROOM;
      else
      *commnd = NET_ROUTE_ROOM;
      *send2 = NON_LOC_NET;
      *send3 = LOC_NET;
      *name = "Backbone - Passive";
      break;
      default: crashout("shared rooms: #2");

      }
    break;
    default: crashout("shared rooms: #1");

    }
  }
/*
* SendRoom()
*
* Sends a room to the receiving system during netting.  It returns ERROR if
* carrier etc is lost.
*/
static int SendRoom(SharedRoom *room, int system, int index, int roomslot,
void *d)
  {
  char cmd;
  char doit, *s1, *s2, *s3, *name;
  if (!gotCarrier())
    {
    modStat = haveCarrier = FALSE;
    return ERROR;

    }
  Addressing(system, index, &cmd, &s1, &s2, &s3, &name, &doit);
  if (doit && SR_Sent[index] != 1 &&
  (roomTab[roomslot].rtlastNet >  netBuf.netRooms[index].lastMess ||
  GetFA(room->mode) ||
  (roomTab[roomslot].rtShareType == BACKBONE &&
  CGetMode(room->mode) != PEON &&  !netBuf.nbflags.local))
  )
    {
    /**
    if (cfg.BoolFlags.debug)
    splitF(netLog, "Why: %s && %d[%d] && ( %ld > %ld || %d || ( %s BACKBONE && %s PEON && %s LOCAL))\n",
    (doit ? "TRUE":"FALSE"), SR_Sent[index],index,
    roomTab[roomslot].rtlastNet,netBuf.netRooms[index].lastMess,GetFA(room->mode),
    (roomTab[roomslot].rtShareType == BACKBONE ? "" : "Not"),
    (CGetMode(room->mode) != PEON ? "Not" : ""),
    (netBuf.nbflags.local ? "" : "NOT"));
    **/
    ITL_optimize(TRUE);
    findAndSend((RecMassTransfer || chkNeedsProcessing(index)) ?
    NET_ROOM : cmd, s1, s2, s3, index,
    RoomSend, roomTab[roomslot].rtname, RoomReceive);

    }
  return TRUE;

  }
/*
* findAndSend()
*
* This function manages sending a room (virtual or normal) to the receiver,
* handling both normal and route rooms, via function pointers.
*/
void  findAndSend(int commnd, char *send1, char *send2, char *send3, int rover,
int (*MsgSender)(int r, char *d1, char *d2, char *d3),
label roomName, int (*MsgReceiver)(int r, char y))
  {
  struct cmd_data cmds;
  extern MessageBuffer tempMess;
  int  tempcount;
  extern char *netRoomTemplate, *WRITE_ANY;
  if (!gotCarrier()) return;
  zero_struct(cmds);
  cmds.command = commnd;
  strCpy(cmds.fields[0], roomName);
  if (commnd != ERROR)
  if (!sendNetCommand(&cmds, "shared rooms"))
    {
    if (commnd == NET_ROUTE_ROOM)
      {
      findAndSend(NET_ROOM, send1, send2, send3, rover, MsgSender,
      roomName, MsgReceiver); /* time to recurse */
      return ;

      }
    else
      {
      sPrintf(tempMess.mbtext, "%%s reports: %s (%s) commnd=%d", RecBuf+1,
      roomName,commnd);
      no_good(tempMess.mbtext, FALSE);
      return ;

      }

    }
  if (!ITL_SendMessages())
    {
    no_good("Couldn't start WC for room sharing: %s",
    FALSE);
    return;

    }
  tempcount = (*MsgSender)(rover, send1, send2, send3);
  ITL_StopSendMessages();
  if( logNetResults && netDebug )
    {
    if( EncCount > 0)
      splitF(netLog, "(%d) (%ld => %ld bytes)\n", tempcount, EncCount, ByteCount);
    else splitF(netLog,"\n");
    };
  if (commnd == NET_ROUTE_ROOM)
    {
    (*MsgReceiver)(rover, FALSE);

    }

  }
/*
* RoomReceive()
*
* This function receives messages for a room.
*/
int RoomReceive(int rover, char ReplyFirst)
  {
  recNetMessages(rover, roomBuf.rbname, netRoomSlot(rover), FALSE);
  return 0;

  }
/*
* RoomSend()
*
* This function sends messages for a room.
*/
int RoomSend(int rover, char *send1, char *send2, char *send3)
  {
  extern char PrTransmit;
  char work[10];
  int MsgCount = 0;
  if( logNetResults && netDebug )splitF(netLog, "Sending %s ", roomBuf.rbname);
  zero_struct(NetStyle);
  NetStyle.addr1 = send1;
  NetStyle.addr2 = send2;
  NetStyle.addr3 = send3;
  if (GetFA(netBuf.netRooms[rover].mode))
    {
    sPrintf(work, CACHE_END_NAME, netRoomSlot(rover));
    if (SendPrepAsNormal(work, &MsgCount))
    UnSetFA(netBuf.netRooms[rover].mode);

    }
  NetStyle.sendfunc = sendITLchar;
  /*   PrTransmit = FALSE; */
  MsgCount += showMessages(NEWoNLY, FALSE, netBuf.netRooms[rover].lastMess,
  NetRoute);
  if (TrError == TRAN_SUCCESS)
    {
    SetHighValues(rover);
    SR_Sent[rover] = 1;

    }
  else
    {
    MsgCount = 0;

    }
  return MsgCount;

  }
/*
* SetHighValues()
*
* This function sets the high message sent for a normal shared room after a
* successful net session has (apparently) taken place.
*/
void SetHighValues(int rover)
  {
  if (NetStyle.HiSent == 0l)
    {
    NetStyle.HiSent =
    max(netBuf.netRooms[rover].lastMess,roomTab[thisRoom].rtlastMessage);

    }
  netBuf.netRooms[rover].lastMess = NetStyle.HiSent;
  netTab[thisNet].netTRooms[rover].lastMess = NetStyle.HiSent;

  }
/*
* SendPrepAsNormal()
*
* This function sends files prepared for cache sending as normal message files,
* instead.
*/
char SendPrepAsNormal(char *work, int *MsgCount)
  {
  char tempNm[3*NAMESIZE];
  NetCacheName(tempNm, thisNet, work);
  if ((netMisc = safeopen(tempNm, READ_ANY)) != NULL)
    {
    while (getMessage(getNetChar, TRUE, TRUE, TRUE))
      {
      (*MsgCount)++;
      prNetStyle(0, sendITLchar, FALSE, "");

      }
    fclose(netMisc);

    }
  if (gotCarrier())
    {
    unlink(tempNm);
    return TRUE;

    }
  return FALSE;

  }
/*
* NetRoute()
*
* This is a worker function, returns TRUE if message sent.
*/
char NetRoute(int status)
  {
  long temp;
  if ((strncmp(msgBuf.mbaddr, NetStyle.addr1, strLen(NetStyle.addr1))
  == SAMESTRING  ||
  strncmp(msgBuf.mbaddr, NetStyle.addr2, strLen(NetStyle.addr2))
  == SAMESTRING  ||
  strncmp(msgBuf.mbaddr, NetStyle.addr3, strLen(NetStyle.addr3))
  == SAMESTRING) &&
  RoutePath(LOC_NET, msgBuf.mbaddr)     != thisNet       &&
  RoutePath(NON_LOC_NET, msgBuf.mbaddr) != thisNet)
    {
    prNetStyle(FALSE, NetStyle.sendfunc, TRUE, "");
    temp = atol(msgBuf.mbId);
    NetStyle.HiSent = max(NetStyle.HiSent,temp);
    return TRUE;

    }
  return FALSE;

  }
/*
* RoutePath()
*
* This function returns the number of the node that routed this msg to here.
* If the msg was not routed in from a BackBone, then return ERROR, which will
* never match another node's #.
*
* 88Oct13: Now simply check for msg origin, assume if one exists that it
* should be checked.  Don't remember why it is restricted to only
* BACKBONE-routed msgs.  Doesn't seem necessary.
*/
int RoutePath(char *rp, char *str)
  {
  if (strncmp(rp, str, strLen(rp)) == SAMESTRING)
    {
    if (strLen(str) != strLen(rp)) /* prevent return of 0 */
    return atoi(str + 2);

    }
  return ERROR;

  }
/*
* doSendFiles()
*
* This function will send files to a victim.
*/
void doSendFiles()
  {
  extern char       *READ_ANY;
  struct   fl_send  theFiles;
  SYS_FILE          sdFile;
  char              temp[8];
  FILE              *fd;
  ITL_optimize(FALSE);    /* try for better protocol */
  sPrintf(temp, "%d.sfl", thisNet);
  makeSysName(sdFile, temp, &cfg.netArea);
  if ((fd = safeopen(sdFile, READ_ANY)) == NULL)
    {
    sPrintf(msgBuf.mbtext, "Couldn't open send file %s for %s!",
    sdFile,netBuf.netName);
    netResult(msgBuf.mbtext);
    netBuf.nbflags.send_files = FALSE;

    }
  else
    {
    while (getSLNet(theFiles, fd) && haveCarrier)
      {
      sysSendFiles(&theFiles);

      }
    fclose(fd);
    if (haveCarrier)
      {
      /* if no carrier, was an error during transmit */
      unlink(sdFile);
      netBuf.nbflags.send_files = FALSE;

      }

    }

  }
/*
* netSendFile()
*
* This function will send a file to another system via net.
*/
void netSendFile(DirEntry *fn)
  {
  extern char     *READ_ANY;
  struct cmd_data cmds;
  char            mess[140];
  if (!gotCarrier()) return ;
  if( logNetResults && netDebug )splitF(netLog, "Send File: %s\n", fn->unambig);
  zero_struct(cmds);
  cmds.command = SEND_FILE;
  strCpy(cmds.fields[0], fn->unambig);
  sPrintf(cmds.fields[1], "%ld", (fn->FileSize + SECTSIZE - 1) / SECTSIZE);
  sPrintf(cmds.fields[2], "%ld", fn->FileSize);
  if (!sendNetCommand(&cmds, "send file"))
    {
    if (haveCarrier)
      {
      strCpy(mess, "%s reports: ");
      strCat(mess, RecBuf + 1);
      no_good(mess, FALSE);

      }

    }
  else
    {
    SendHostFile(fn->unambig);
    if (haveCarrier)
      {
      sPrintf(msgBuf.mbtext, "%s sent to %s.", fn->unambig,
      netBuf.netName);
      netResult(msgBuf.mbtext);

      }

    }

  }
extern FILE *upfd;
/*
* askFiles()
*
* This function will ask for file(s) from caller.
*/
static void fl_req_free(struct fl_req *d);
void askFiles()
  {
  label    data2;
  SYS_FILE dataFl;
  char     mess[130];
  char     ambiguous;
  int      result = ITL_SUCCESS;
  FILE     *temp;
  struct   cmd_data cmds;
  struct   fl_req file_data, *list;
  SListBase Failed =
    {
    NULL, NULL, NULL, fl_req_free, NULL

    };
  extern char *READ_ANY, *WRITE_ANY;
  if (!gotCarrier())
    {
    modStat = haveCarrier = FALSE;
    return ;

    }
  sPrintf(data2, "%d.rfl", thisNet);
  makeSysName(dataFl, data2, &cfg.netArea);
  temp = safeopen(dataFl, READ_ANY);
  if (temp == NULL)
    {
    no_good("Couldn't open room request file for %s", FALSE);
    netBuf.nbflags.room_files = FALSE;

    }
  else
    {
    ITL_optimize(FALSE);    /* try for better protocol */
    while ( result == ITL_SUCCESS &&
    fread(&file_data, sizeof (file_data), 1, temp) == 1 &&
    gotCarrier() && result == ITL_SUCCESS)
      {
      if (netSetNewArea(&file_data.flArea))
        {
        zero_struct(cmds);
        ambiguous = !(strchr(file_data.roomfile, '*') == NULL &&
        strchr(file_data.roomfile, '?') == NULL);
        cmds.command = (!ambiguous) ? R_FILE_REQ : A_FILE_REQ;
        strCpy(cmds.fields[0], file_data.room);
        strCpy(cmds.fields[1], file_data.roomfile);
        if( logNetResults )splitF(netLog, "Requesting %s in %s\n", file_data.roomfile,
        file_data.room);
        if (!sendNetCommand(&cmds,
        (!ambiguous) ? "single file request" :
        "multiple file request"))
          {
          sPrintf(mess, "%%s reports %s for file %s in %s.", RecBuf+1,
          file_data.roomfile, file_data.room);
          no_good(mess, FALSE);

          }
        else
          {
          if (ambiguous)
          result = multiReceive(&file_data);
          else
            {
            if ((result = ITL_Receive(file_data.filename, FALSE,
            TRUE, putFLChar, fclose)) == ITL_SUCCESS)
              {
              sPrintf(msgBuf.mbtext,
              "File '%s' received from %s (stored in directory %s).",
              file_data.filename, netBuf.netName,
              prtNetArea(&file_data.flArea));
              netResult(msgBuf.mbtext);

              }

            }

          }

        }
      homeSpace();

      }
    if (gotCarrier())
      {
      fclose(temp);
      unlink(dataFl);
      netBuf.nbflags.room_files = FALSE;

      }
    else
      {
      haveCarrier = modStat = FALSE;
      /* Now find out what we didn't get and set up a new request queue */
      do
        {
        /* use do loop to get the one it failed in */
        list = GetDynamic(sizeof *list);
        copy_struct(file_data, (*list));
        AddData(&Failed, list, NULL, FALSE);

        }
      while (fread(&file_data, sizeof (file_data), 1, temp) == 1);
      fclose(temp);
      unlink(dataFl);
      upfd = fopen(dataFl, WRITE_ANY);
      KillList(&Failed);
      fclose(upfd);

      }

    }

  }
/*
* fl_req_free()
*
* This will write and free a file request.
*/
static void fl_req_free(struct fl_req *d)
  {
  if (upfd != NULL) fwrite(d, sizeof *d, 1, upfd);
  free(d);

  }
/*
* multiReceive()
*
* This function will receive multiple files.
*/
char multiReceive(struct fl_req *file_data)
  {
  char        first = 1;
  extern char *WRITE_ANY;
  sPrintf(msgBuf.mbtext,
  "Following files received from %s in response to request for %s from %s: ",
  netBuf.netName, file_data->roomfile, file_data->room);
  do
    {
    if (ITL_Receive(NULL, FALSE, TRUE, putFLChar, fclose) != ITL_SUCCESS ||
    !gotCarrier()) return ITL_BAD_TRANS;
    if (RecBuf[0] == 0)
      {
      /* Last file name       */
      sPrintf(lbyte(msgBuf.mbtext), " (stored in directory %s).",
      prtNetArea(&file_data->flArea));
      netResult(msgBuf.mbtext);
      return ITL_SUCCESS;

      }
    if (!first)
    strCat(msgBuf.mbtext, ", ");
    else
    first = FALSE;
    strCat(msgBuf.mbtext, RecBuf);
    if (ITL_Receive(RecBuf, FALSE, TRUE, putFLChar, fclose) != ITL_SUCCESS
    || !gotCarrier()) return ITL_BAD_TRANS;

    }
  while (1);
  return ITL_SUCCESS;

  }
/*
* sendNetCommand()
*
* This sends a command to the receiver.
*/
char sendNetCommand(struct cmd_data *cmds, char *error)
  {
  char errMsg[100];
  int  count;
  if (cfg.BoolFlags.debug && netDebug )
    {
    splitF(netLog, "Sending Command: %d\n", cmds->command);
    splitF(netLog, " Field[0]: %s\n", cmds->fields[0]);
    splitF(netLog, " Field[1]: %s\n", cmds->fields[1]);
    splitF(netLog, " Field[2]: %s\n", cmds->fields[2]);
    splitF(netLog, " Field[3]: %s\n", cmds->fields[3]);
    };

  if (!ITL_Send(STARTUP))
    {
    sPrintf(errMsg, "Link failure for %s (system: %%s).", error);
    if (cmds->command != HANGUP) no_good(errMsg, TRUE);
    killConnection();
    return FALSE;

    }
  sendITLchar(cmds->command);
  for (count = 0; count < 4; count++)
    {
    if (cmds->fields[count][0])
      {
      mTrPrintf("%s", cmds->fields[count]);

      }

    }
  sendITLchar(0);
  ITL_Send(FINISH);
  if (cmds->command == HANGUP && !inReceive) return TRUE;
  ITL_Receive(NULL, FALSE, TRUE, putFLChar, fclose);
  if (RecBuf[0] == BAD || !gotCarrier()) return FALSE;
  return TRUE;

  }
/*
* sendHangUp()
*
* This sends the hangup command to receiver.
*/
void sendHangUp()
  {
  struct cmd_data cmds;
  if (!gotCarrier())
    {
    modStat = haveCarrier = FALSE;
    return ;

    }
  zero_struct(cmds);
  cmds.command = HANGUP;
  sendNetCommand(&cmds, "HANGUP");

  }
/*
* no_good()
*
* This handles error messages when something really bad happens.
*/
void no_good(char *str, char hup)
  {
  sPrintf(msgBuf.mbtext, str, netBuf.netName);
  if (hup)
    {
    killConnection();

    }
  netResult(msgBuf.mbtext);

  }
/*
* s_m_n()
*
* This sends mail normal (non-route mail).
*/
int s_m_n()
  {
  FILE     *ptrs;
  label    fntemp;
  SYS_FILE fn;
  int      messCount = 0;
  struct   netMLstruct buf;
  extern char *READ_ANY;
  sPrintf(fntemp, "%d.ml", thisNet);
  makeSysName(fn, fntemp, &cfg.netArea);
  if ((ptrs = safeopen(fn, READ_ANY)) == NULL)
    {
    if (netBuf.nbflags.normal_mail)
      {
      sPrintf(msgBuf.mbtext, "No mail file to send to %s?",
      netBuf.netName);
      netResult(msgBuf.mbtext);

      }
    return 0;

    }
  while (getMLNet(ptrs, buf) && TrError == TRAN_SUCCESS)
    {
    if (findMessage(buf.ML_loc, buf.ML_id, TRUE))
      {
      if (netDebug)
        {
        MessageBuffer *msg = &msgBuf;
        splitF(netLog, "Message %6d Sector ID: %6d Author:%s\n",  msg->mbheadChar, msg->mbheadSector, msg->mbauth);
        splitF(netLog, "Date:%20s Time:%20s Local Id:%20s\n",  msg->mbdate,  msg-> mbtime,  msg-> mbId);
        splitF(netLog, "Human:%20s ID:%20s Room:%20s\n",  msg->mboname,  msg->mborig,  msg->mbroom);
        splitF(netLog, "Origin:%20s To:%20s\n",  msg->mbsrcId,  msg->mbto);
        splitF(netLog, "Route:%s\n", msg->mbaddr);
        splitF(netLog, "OtherNet:%s\n",msg->mbOther);
        splitF(netLog, "reply:%20s Domain:%20s\n",msg->mbreply, msg->mbdomain);
        }
      prNetStyle(0, sendITLchar, TRUE, netBuf.netName);
      messCount++;

      }

    }
  fclose(ptrs);
  if (TrError == TRAN_SUCCESS)
    {
    unlink(fn);
    return messCount;

    }
  killConnection();
  if( logNetResults )splitF(netLog, "\nFailed transferring mail!\n");
  return 0;

  }
/*
* SendHostFile()
*
* This function will send a file to the other system.
*/
void SendHostFile(char *fn)
  {
  int  success;
  FILE *fd;
  extern int errno;
  extern char *READ_ANY;
  success = ((fd = safeopen(fn, READ_ANY)) != NULL);
  if (ITL_Send(STARTUP))
    {
    if (!success) mTrPrintf("System error failure, this is a bogus file.");
    else
      {
      SendThatDamnFile(fd, sendITLchar);

      }
    ITL_Send(FINISH);

    }
  if (!success)
    {
    sPrintf(msgBuf.mbtext, "System error! Couldn't open %s for %s, errno %d.",
    fn, netBuf.netName, errno);
    netResult(msgBuf.mbtext);

    }

  }
