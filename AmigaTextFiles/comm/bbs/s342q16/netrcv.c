/*
*       netrcv.c
*
*      Networking functions for reception.
*/
/*
*       history
*
* 86Aug20 HAW  History not maintained due to space problems.
*/
#include "ctdl.h"
/*
*       Contents
*
* called()    Handle being called.
* rcvStuff()    Manage receiving stuff.
* netPwd()    Check password.
* doResults()   Post-process results.
* getId()     Get nodeId and nodeName from caller.
* getNextCommand()  Get next command.
* grabCommand()   Extract network cmds from buffer.
* reply()     Sends a reply to caller.
* reqReversal()   Handle role reversal.
* reqCheckMail()    Check incoming mail.
* targetCheck()   Check for existence of recipients.
* CheckRecipient()  work fn for above.
* doNetRooms()    Manage integrating incoming messages.
* IntegrateRoom()   work fn for above.
* ReadNetRoomFile() work fn for above.
* getMail()   Handle incoming mail.
* reqSendFile()   Receive a sent file.
* netFileReq()    Senda a requested file.
* netRRReq()    Handle room sharing.
* recNetMessages()  Receive net messages.
* UpdateRecoveryFile()  Updates the network recovery file.
*   RoomRoutable()    Is this room routable?
* IsRoomRoutable()  Is this room routable?
* netMultiSend()    Send multiple files.
* RecoverNetwork()  Recover from network disaster.
*/
extern char logNetResults;
char        netDebug = FALSE;
char    processMail;
char    PosId;
char    *AssignAddress = NULL;
#define RECOVERABLE 1
extern char   *SR_Sent;
extern FILE   *netLog, *netMisc;
extern AN_UNSIGNED      RecBuf[];
extern int    counter;
extern int    callSlot;
extern char   checkNegMail;
extern char   inReceive;
extern label    normed, callerName, callerId;
extern char   RouteMailForHere;
extern char   *LOC_NET, *NON_LOC_NET;
char    normId(), getNetMessage();
char    callOut();
AN_UNSIGNED      inp();
char *netRoomTemplate = "room%d.$$$";
char *SharingRefusal[] =
  {
  "'%s' does not exist",
  "'%s' is not a networking room",
  "'%s' is not networking with you",
  "No can do for '%s'",

  };
extern char  RecMassTransfer;
extern CONFIG    cfg;   /* Lots an lots of variables    */
extern logBuffer logBuf;  /* Person buffer    */
extern logBuffer logTmp;  /* Person buffer    */
extern aRoom     roomBuf; /* Room buffer    */
extern rTable    *roomTab;
extern MessageBuffer   msgBuf;
extern NetBuffer netBuf;
extern NetTable  *netTab;
extern int       thisNet;
extern char      onConsole;
extern char      loggedIn;  /* Is we logged in?   */
extern char      outFlag; /* Output flag    */
extern char      haveCarrier; /* Do we still got carrier?     */
extern char      modStat; /* Needed so we don't die       */
extern char      WCError;
extern int       thisRoom;
extern int       thisLog;
extern char  *APPEND_TEXT, *WRITE_TEXT, *READ_TEXT;
extern char  *R_SH_MARK, *NON_LOC_NET, *LOC_NET;
extern long char_in, char_out, start_time;

/*
* called()
*
* We've been called, so let's handle it.
*/
void called()
  {
  ITL_InitCall();   /* Initialize the ITL layer */
  memset(SR_Sent, 0, SHARED_ROOMS);
  inReceive = TRUE;
  RecMassTransfer = FALSE;
  SpecialMessage("Status:Net Carrier");
  if( logNetResults || netDebug )splitF(netLog, "Carrier %s\n", Current_Time());
  processMail = checkNegMail = FALSE;
  if (!called_stabilize())
    {
    if (cfg.BoolFlags.debug)splitF(netLog," Not Stabilized...\n");
    return ;

    };
  if( logNetResults )splitF(netLog, "Stabilized\n");
  SpecialMessage("Status:Net Session");
  char_in = char_out = 0;
  start_time = Set_Timer(0);       /* initialize time of day */
  getId();
  if (!haveCarrier) return;
  rcvStuff(FALSE);
  if ( logNetResults )splitF(netLog, "Finished with %s @%s\n",  callerName, Current_Time());
  Compute_Data(callerName);
  pause(20);
  killConnection();
  doResults();
  if( logNetResults )splitF(netLog, "\n");
  SpecialMessage("Status:Net Completed");

  }
/*
* rcvStuff()
*
* This function manages receiving stuff.
*/
void rcvStuff(char reversed)
  {
  label     tempNm;
  struct cmd_data cmds;
  PosId = (callSlot == ERROR) ? FALSE : (netBuf.OurPwd[0] == 0 || (reversed && netBuf.nbflags.Stadel));
  RouteMailForHere = FALSE;
  do
    {
    getNextCommand(&cmds);
    switch (cmds.command)
      {
      case HANGUP:          break;
      case NORMAL_MAIL:    getMail();     break;
      case A_FILE_REQ:
      case R_FILE_REQ:     netFileReq(&cmds);   break;
      case NET_ROOM:       netRRReq(&cmds, FALSE);  break;
      case ROLE_REVERSAL:  reqReversal(reversed);   break;
      case CHECK_MAIL:     reqCheckMail();    break;
      case SEND_FILE:      reqSendFile(&cmds);    break;
      case NET_ROUTE_ROOM: netRRReq(&cmds, TRUE);   break;
      case SYS_NET_PWD:    netPwd(&cmds);     break;
      case ITL_PROTOCOL:   ITL_rec_optimize(&cmds);       break;
      case ITL_COMPACT:    ITL_RecCompact(&cmds);   break;
      case ROUTE_MAIL:     netRouteMail(&cmds);   break;
      case FAST_MSGS:  netFastTran(&cmds);    break;
      default:
      sprintf(tempNm, "'%d' unknown.", cmds.command);
      reply(BAD, tempNm);   break;

      }

    }
  while (gotCarrier() && cmds.command != HANGUP);

  }
/*
* netPwd()
*
* Check out the password sent to us, set flags appropriately.
*/
void netPwd(struct cmd_data *cmds)
  {
  if (callSlot != ERROR)
    {
    PosId = !strCmpU(cmds->fields[0], netBuf.OurPwd);
    if (!PosId)
      {
      if( logNetResults )splitF(netLog, "Bad pwd: -%s-\n", cmds->fields[0]);
      sPrintf(msgBuf.mbtext, "%s sent bad password -%s-.",
      callerName, cmds->fields[0]);
      netResult(msgBuf.mbtext);

      }

    }
  reply(GOOD, "");

  }
/*
* doResults()
*
* This function processes the results of receiving thingies and such.
*/
void doResults()
  {
  extern SListBase DomainMap;
  void HandleExistingDomain();
  int i;
  label temp;
  extern int RMcount;
  DisableModem(TRUE);
  InitVortexing();    /* handles all mail for here */
  if (processMail)
    {
    if (AddNetMsgs("tempmail.$$$", inMail, 2, MAILROOM, TRUE) == ERROR)
    no_good("No mail file for %s?", TRUE);

    }
  if (RouteMailForHere)
    {
    for (i = 0; ; i++)
      {
      sPrintf(temp, "rmail.%d", i);
      if (AddNetMsgs(temp, inRouteMail, 2, MAILROOM, TRUE) == ERROR)
      break;

      }
    RMcount = 0;

    }
  FinVortexing();   /* finish handling mail */
  if (callSlot == ERROR)
    {
    /* If didn't know this node, don't      */
    EnableModem(TRUE);
    return ;  /* bother with anything else  */

    }
  if (checkNegMail) readNegMail(TRUE);
  ReadFastFiles();    /* messages transferred in one big arc */
  doNetRooms();
  AdjustRoute();
  netBuf.nbLastConnect = CurAbsolute();
  putNet(thisNet, &netBuf);
  UpdVirtStuff(); /* Just in case. */
  RunList(&DomainMap, HandleExistingDomain);
  RationalizeDomains(); /* again just in case ... */
  EnableModem(TRUE);

  }
/*
* getId()
*
* This gets nodeId and nodeName from caller.
*/
void getId()
  {
  char *secRunner;
  #ifdef NEED_THIS_DATA
  SYS_FILE fn;
  #endif
  int i;
  extern long byteRate;
  extern char *APPEND_ANY;
  if (!haveCarrier) return;
  ITL_Receive(NULL, FALSE, TRUE, putFLChar, fclose);
  if (!gotCarrier())
    {
    return ;

    }
  strncpy(callerId, RecBuf, NAMESIZE - 1);
  secRunner = RecBuf;
  while (*secRunner != 0) secRunner++;
  secRunner++;
  strncpy(callerName, secRunner, NAMESIZE - 1);
  normId(callerId, normed);
  if (strLen(callerName) == 0 || strLen(callerId) == 0)
    {
    if( logNetResults )splitF(netLog, "getId invalid data, dropping connection.\n\n");
    killConnection();
    #ifdef NEED_THIS_DATA
    makeSysName(fn, "getid.sys", &cfg.netArea);
    if ((upfd = safeopen(fn, APPEND_ANY)) != NULL)
      {
      fwrite(RecBuf, SECTSIZE, 1, upfd);
      fclose(upfd);

      }
    #endif

    }
  if ((callSlot = searchNet(normed, &netBuf)) == ERROR)
    {
    sPrintf(msgBuf.mbtext, "New caller: %s (%s)", callerName, callerId);
    netResult(msgBuf.mbtext);

    }
  else
    {
    for (i = 0; i < SHARED_ROOMS; i++)
    resetNeedsProcessing(i);
    putNet(callSlot, &netBuf);

    }
  if( logNetResults)splitF(netLog, "%s (%s) @ %ld\n", callerName, callerId, byteRate * 10L);

  }
/*
* getNextCommand()
*
* This gets next command (facility request) from the caller.
*/
void getNextCommand(struct cmd_data *cmds)
  {
  zero_struct(*cmds);
  ITL_Receive(NULL, FALSE, TRUE, putFLChar, fclose);
  if (!gotCarrier())
    {
    return ;

    }
  grabCommand(cmds, RecBuf);
  }
/*
* grabCommand()
*
* This pulls network cmds out of the specified buffer.
*/
void grabCommand(struct cmd_data *cmds, char *sect)
  {
  int fcount = 0;
  cmds->command = sect[0];
  sect++;
  while (sect[0] > 0 && fcount < 4)
    {
    strncpy(cmds->fields[fcount], sect, NAMESIZE - 1);
    cmds->fields[fcount][NAMESIZE - 1] = 0;
    fcount++;
    while (*sect != 0) sect++;
    sect++;

    }
  }
/*
* reply()
*
* This sends a full reply to the caller's request.
*/
void reply(char state, char *reason)
  {
  if (!ITL_Send(STARTUP))
    {
    no_good("Couldn't send reply to %s!", TRUE);
    return;

    }
  sendITLchar(state);
  if (state == BAD)
    {
    mTrPrintf("%s", reason);
    if (cfg.BoolFlags.debug) splitF(netLog, "Replying BAD: %s\n", reason);

    }
  sendITLchar(0);
  ITL_Send(FINISH);

  }
/*
* reqReversal()
*
* This handles the role reversal command.
*/
void reqReversal(char reversed)
  {
  if ( netDebug ) splitF(netLog, "Role reversal\n");
  if (reversed)
    {
    reply(BAD, "Synch error on Reversal!");
    return ;

    }
  reply(GOOD, "");
  if (callSlot == ERROR)      /* Forces a "null" role reversal  */
  zero_struct(netBuf.nbflags);
  sendStuff(TRUE, PosId);

  }
/*
* reqCheckMail()
*
* This checks incoming mail and does negative acks where appropriate.
*/
void reqCheckMail()
  {
  if( logNetResults && netDebug )splitF(netLog, "checking Mail\n");
  if (!processMail)
    {
    reply(BAD, "No mail to check!");
    return ;

    }
  reply(GOOD, "");
  if (ITL_Send(STARTUP))
    {
    AddNetMsgs("tempmail.$$$", targetCheck, FALSE, MAILROOM, TRUE);
    sendITLchar(NO_ERROR);
    ITL_Send(FINISH);

    }

  }
/*
* targetCheck()
*
* This checks for existence of recipients.
*/
void targetCheck()
  {
  if (HasOverrides(&msgBuf))
    {
    RunList(&msgBuf.mbOverride, CheckRecipient);

    }
  else
    {
    CheckRecipient(msgBuf.mbto);

    }

  }
/*
* CheckRecipient()
*
* This will check to see if the recipient exists.
*/
void CheckRecipient(char *d)
  {
  char     sigChar;
  if (!d[0])
    {
    sigChar = BAD_FORM;

    }
  else if (strchr(d, '!') != NULL)
  return ;  /* STadel routed mail - don't try to check it here */
  else
    {
    if (PersonExists(d) != ERROR)
    return ;
    else
    sigChar = NO_RECIPIENT;

    }
  sendITLchar(sigChar);
  mTrPrintf("%s", msgBuf.mbauth);
  mTrPrintf("%s", d);
  mTrPrintf("%s @ %s", msgBuf.mbdate, msgBuf.mbtime);

  }
/*
* doNetRooms()
*
* This function integrates temporary files containing incoming messages into
* the data base.
*/
void doNetRooms()
  {
  SYS_FILE fileNm;
  int IntegrateRoom(SharedRoom *room, int system, int index, int roomslot,
  void *d);
  EachSharedRoom(thisNet, IntegrateRoom, NULL, NULL);
  makeSysName(fileNm, RECOVERY_FILE, &cfg.netArea);
  unlink(fileNm);

  }
/*
* IntegrateRoom()
*
* This function helps integrate incoming messages into a room.
*/
int IntegrateRoom(SharedRoom *room, int system, int index, int roomslot,
void *d)
  {
  if (chkNeedsProcessing(index))
    {
    ReadNetRoomFile(index, NULL);
    resetNeedsProcessing(index);

    }
  if (SR_Sent[index] == 1)
  netBuf.netRooms[index].lastMess =  roomTab[roomslot].rtlastMessage;
  return TRUE;

  }
/*
* ReadNetRoomFile()
*
* This function reads in a file of messages received on net.
* NB: the passed parameter is the index into the netBuf.netRooms
* pseudo-array, not the number of the room itself.  See the code.
*/
void ReadNetRoomFile(int rover, char *fn)
  {
  label temp2;
  if (cfg.BoolFlags.debug)splitF(netLog," ReadNetRoomFile(Rover:%d, fn:%s)\n",rover,(fn== NULL) ? "NULL":fn);
  if (fn == NULL) sPrintf(temp2, netRoomTemplate, netRoomSlot(rover));
  getRoom(netRoomSlot(rover));
  if (roomBuf.rbShareType != PEON &&
  GetMode(netBuf.netRooms[rover].mode) != PEON)
  AssignAddress = NON_LOC_NET;
  else
  AssignAddress = LOC_NET;
  InitVortexing();
  AddNetMsgs((fn == NULL) ? temp2 : fn, inMail, TRUE, netRoomSlot(rover),
  (fn == NULL));
  FinVortexing();
  AssignAddress = NULL;

  }
/*
* getMail()
*
* This function Grabs mail from caller.
*/
void getMail()
  {
  SYS_FILE tempNm;
  if( netDebug && logNetResults) splitF(netLog, "Receiving Mail\n");
  makeSysName(tempNm, "tempmail.$$$", &cfg.netArea);
  if (ITL_StartRecMsgs(tempNm, TRUE, TRUE, NULL) == ITL_SUCCESS)
    {
    processMail = TRUE;

    }

  }
/*
* reqSendFile()
*
* This function handles receiving a sent file.  Note that it handles file
* redirection.
*/
void reqSendFile(struct cmd_data *cmds)
  {
  long  proposed;
  int count;
  char work[100], work1[100], *Dir;
  extern char *READ_ANY, *WRITE_ANY;
  static char *Reject = "File %s from %s rejected because %s.";
  /* don't accept files from rogues */
  if (!PosId)
    {
    reply(BAD, "No room for file.");
    return;

    }
  /* handle incoming file redirection */
  if ((Dir = RedirectFile(cmds->fields[0], netBuf.netName)) != NULL ||
  (Dir = RedirectFile(cmds->fields[0], netBuf.nbShort)) != NULL)
    {
    RedirectName(work1, Dir, "mm12"); /* temp file name */
    unlink(work1);  /* kill any prior backups (shouldn't be any ...) */
    RedirectName(work, Dir, cmds->fields[0]);
    if (access(work, 0) == 0)
    rename(work, work1);

    }
  else if (netSetNewArea(&cfg.receptArea))
    {
    proposed = atol(cmds->fields[2]);
    if (sysRoomLeft() < proposed ||
    proposed > ((long) cfg.maxFileSize) * 1024l)
      {
      reply(BAD, "No room for file.");
      sPrintf(msgBuf.mbtext, Reject, cmds->fields[0], callerName,
      proposed > ((long) cfg.maxFileSize) * 1024l ?
      "the file was larger than #MAX_NET_FILE" :
      "there was not enough room left in reception directory");
      netResult(msgBuf.mbtext);
      homeSpace();
      return;

      }
    count = 0;
    strCpy(work, cmds->fields[0]);
    while (access(work, 0) != -1)
      {
      sPrintf(work, "a.%d", count++);

      }

    }
  else
    {
    reply(BAD, "System error");
    return ;

    }
  reply(GOOD, NULL);
  if( logNetResults )splitF(netLog, "File Reception: %s\n", cmds->fields[0]);
  ITL_Receive(work, FALSE, TRUE, putFLChar, fclose);
  homeSpace();
  if (haveCarrier)
    {
    if (strCmp(work, cmds->fields[0]) == SAMESTRING || Dir != NULL)
    sPrintf(msgBuf.mbtext, "%s received from %s.", cmds->fields[0],
    callerName);
    else
    sPrintf(msgBuf.mbtext, "%s (saved as %s) received from %s.",
    cmds->fields[0], work, callerName);
    netResult(msgBuf.mbtext);
    if (Dir != NULL)  /* kill temporary bkp of redirected file */
    unlink(work1);

    }
  else if (Dir != NULL) /* failed transfer of redirected file */
  rename(work1, work);

  }
/*
* netFileReq()
*
* This will handle requests for file transfers.
*/
void netFileReq(struct cmd_data *cmds)
  {
  int  roomSlot;
  extern char *READ_ANY;
  if( logNetResults)splitF(netLog, "File request: %s in %s\n", cmds->fields[1],
  cmds->fields[0]);
  /* allow disabling this feature on a system by system basis */
  if (PosId && netBuf.nbflags.NoDL)
    {
    reply(BAD, "Downloading disabled.");
    return;

    }
  if ((roomSlot = roomExists(cmds->fields[0])) == ERROR   ||
  !roomTab[roomSlot].rtflags.ISDIR  ||
  roomTab[roomSlot].rtflags.NO_NET_DOWNLOAD ||
  !roomTab[roomSlot].rtflags.DOWNLOAD)
    {
    sPrintf(msgBuf.mbtext, "Room %s does not exist.", cmds->fields[0]);
    reply(BAD, msgBuf.mbtext);
    return;

    }
  getRoom(roomSlot);
  if (!setSpace(&roomBuf))
    {
    reply(BAD, "Directory error");
    return;

    }
  if (cmds->command == A_FILE_REQ)
    {
    reply(GOOD, "");
    sPrintf(msgBuf.mbtext, "Following files sent to %s from %s: ",
    callerName, roomBuf.rbname);
    wildCard(netMultiSend, cmds->fields[1], FALSE, "", FALSE);
    if (ITL_Send(STARTUP))
      {
      mTrPrintf("");
      ITL_Send(FINISH);

      }

    }
  else
    {
    if (access(cmds->fields[1], 4) == -1)
      {
      sPrintf(msgBuf.mbtext, "There is no '%s' in %s.", cmds->fields[1],
      cmds->fields[0]);
      reply(BAD, msgBuf.mbtext);
      homeSpace();
      return;

      }
    reply(GOOD, "");
    SendHostFile(cmds->fields[1]);
    sPrintf(msgBuf.mbtext,
    "%s downloaded from %s by %s.",
    cmds->fields[1], formRoom(thisRoom, FALSE, FALSE), callerName);

    }
  homeSpace();
  netResult(msgBuf.mbtext);

  }
/*
* netRRReq()
*
* This function handles room sharing.  If SendBack is TRUE then this is a
* room routing (LD) request and requires we send the room's current contents
* back.
*/
void netRRReq(struct cmd_data *cmds, char SendBack)
  {
  RoomSearch arg;
  char reason[50];
  strCpy(arg.Room, cmds->fields[0]);
  if (!RoomRoutable(&arg))
    {
    sPrintf(reason, SharingRefusal[arg.reason], cmds->fields[0]);
    if( logNetResults )splitF(netLog, "Refusing to share %s (%s)\n", cmds->fields[0], reason);
    reply(BAD, reason);
    return;

    }
  if (!arg.virtual)
    {
    getRoom(arg.room);
    recNetMessages(arg.index, arg.Room, arg.room, TRUE);
    if (SendBack)
    findAndSend(ERROR, R_SH_MARK, LOC_NET,
    roomBuf.rbShareType == BACKBONE ? NON_LOC_NET : NULL, arg.index,
    RoomSend, roomBuf.rbname, RoomReceive);

    }
  else
    {
    RecVirtualRoom(arg.index, TRUE);
    if (SendBack)
    findAndSend(ERROR, NULL, NULL, NULL, arg.index, SendVirtual,
    arg.Room, RecVirtualRoom);

    }

  }
/*
* recNetMessages()
*
* This function receives net messages.  This is not the same as processing
* them.
*/
void recNetMessages(int arraySlot, char *name, int slot, char ReplyFirst)
  {
  SYS_FILE fileNm, temp;
  char reason[60];
  if( netDebug )splitF(netLog, "Receiving %s\n", name);
  sPrintf(temp, netRoomTemplate, slot);
  makeSysName(fileNm, temp, &cfg.netArea);
  switch (ITL_StartRecMsgs(fileNm, ReplyFirst, TRUE, NULL))
    {
    case ITL_SUCCESS:
    setNeedsProcessing(arraySlot);
    UpdateRecoveryFile(name);
    break;
    case ITL_NO_OPEN:
    sPrintf(reason, "Internal error for %s", name);
    reply(BAD, reason);
    break;
    case ITL_BAD_TRANS:
    break;

    }

  }
/*
* UpdateRecoveryFile()
*
* This function updates the network recovery file.
*/
void UpdateRecoveryFile(char *val)
  {
  SYS_FILE fileNm;
  makeSysName(fileNm, RECOVERY_FILE, &cfg.netArea);
  if (access(fileNm, 0) != 0)
  CallMsg(fileNm, callerId);
  CallMsg(fileNm, val);

  }
/*
* RoomRoutable()
*
* Is this room routable?
*/
char RoomRoutable(RoomSearch *data)
  {
  if (!PosId)
    {
    data->reason = NO_PWD;
    return FALSE;

    }
  if (callSlot == ERROR)
    {
    data->reason = NOT_SHARING;
    return FALSE;

    }
  data->reason = NO_ROOM;
  data->virtual = FALSE;
  EachSharedRoom(thisNet, IsRoomRoutable, VirtRoomRoutable, data);
  return (char)(data->reason == FOUND);

  }
/*
* IsRoomRoutable()
*
* This function checks to see if the given shared room matches with the
* argument presented in the void pointer parameter.
*/
int IsRoomRoutable(SharedRoom *room, int system, int index, int roomslot,
void *d)
  {
  RoomSearch *arg;
  arg = d;
  if (strCmpU(roomTab[roomslot].rtname, arg->Room) == SAMESTRING)
    {
    if (roomTab[roomslot].rtflags.SHARED == 0)
    arg->reason = NOT_SHARING;
    else
    arg->reason = FOUND;
    arg->room = roomslot;
    arg->index = index;
    return ERROR;   /* stop searching */

    }
  return TRUE;

  }
/*
* netMultiSend()
*
* This function will send requested files via the net.
*/
void netMultiSend(DirEntry *fn)
  {
  long Sectors;
  if (!gotCarrier()) return ;
  strCat(msgBuf.mbtext, fn->unambig);
  strCat(msgBuf.mbtext, " ");
  Sectors     = ((fn->FileSize + 127) / SECTSIZE);
  if (ITL_Send(STARTUP))
    {
    mTrPrintf("%s", fn->unambig);
    mTrPrintf("%ld", Sectors);
    ITL_Send(FINISH);

    }
  SendHostFile(fn->unambig);

  }
/*
* RecoverNetwork()
*
* This function is called during system startup.  If a disaster hit during
* a network session, this will try to recover messages already transferred
* from files left in the network directory.
*/
void RecoverNetwork()
  {
  SYS_FILE fileNm;
  char line[50];
  label temp;
  int rover;
  FILE *fd;
  RoomSearch arg;
  extern char inNet;
  makeSysName(fileNm, RECOVERY_FILE, &cfg.netArea);
  if ((fd = safeopen(fileNm, READ_TEXT)) == NULL)
  return;   /* normal */
  inNet = NORMAL_NET;
  SpecialMessage("Network Cleanup");
  if (GetAString(line, sizeof line, fd) != NULL)
    {
    if (searchNet(line, &netBuf) != ERROR)
      {
      PosId = TRUE;
      while (GetAString(line, sizeof line, fd) != NULL)
        {
        if (strncmp(line, FAST_TRANS_FILE, strLen(FAST_TRANS_FILE))
        == SAMESTRING)
        RecoverMassTransfer(line);
        else
          {
          strCpy(arg.Room, line);
          if (RoomRoutable(&arg) && !arg.virtual)
            {
            printf("%s\n", line);
            ReadNetRoomFile(arg.index, NULL);

            }

          }

        }
      readNegMail(FALSE);
      AddNetMsgs("tempmail.$$$", inMail, 2, MAILROOM, TRUE);
      for (rover = 0; ; rover++)
        {
        sPrintf(temp, "rmail.%d", rover);
        if (AddNetMsgs(temp, inRouteMail, 2, MAILROOM, TRUE) == ERROR)
        break;

        }

      }

    }
  fclose(fd);
  unlink(fileNm);
  inNet = NON_NET;

  }
