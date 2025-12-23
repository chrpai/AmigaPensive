/*
*       netmisc.c
*
* Networking functions of miscellaneous type.
*/
/*
*       history
*
* 91Aug17 HAW  New comment style.
* 86Aug20 HAW  History not maintained due to space problems.
*/
#include "ctdl.h"
/*
*       contents
*
*/
/* #define NET_DEBUG 1  */
/*
*   External variable declarations in NET.C
*/



char    *SR_Sent;
char    ErrBuf[100];    /* General buffer for error messages */
label   HomeId;
int   AnyIndex = 0;  /* tracks who to call between net sessions */
FILE    *netLog, *netMisc, *netMsg;
static char     UsedNetMsg;
char    *nMsgTemplate = "netMsg.$$$";
char    logNetResults = FALSE;
char    inNet = NON_NET;
AN_UNSIGNED     RecBuf[SECTSIZE + 5];
int   callSlot;
label   normed, callerName, callerId;
logBuffer       *lBuf;
int   PriorityMail = 0;
char    *pollCall;
int   LD_Delay = 60;
static char *SupportedBauds[] =
  {
  "300", "3/12", "3/24", "3/48", "3/96", "3/14.4", "3/19.2", "3/38.4", "3/57.6"

  };
/*
* UntilNetSessions
*
* This is used to maintain a list of Until-Done net sessions as requested by
* the sysop.  Each element contains information concerning which member net
* is involved (only one can be specified) and how long the session should
* last.
*/
void ExplainNeed(int i, MULTI_NET_DATA x);
void freeUNS();
SListBase UntilNetSessions =
  {
  NULL, ChkTwoNumbers, NULL, freeUNS, NULL

  };
/*
*   External variable definitions for NET.C
*/
extern CONFIG    cfg;   /* Lots an lots of variables    */
extern NetBuffer netTemp;
extern logBuffer logBuf;  /* Person buffer    */
extern logBuffer logTmp;  /* Person buffer    */
extern aRoom     roomBuf; /* Room buffer      */
extern rTable    *roomTab;
extern MessageBuffer   msgBuf;
extern MessageBuffer   tempMess;
extern NetBuffer netBuf;
extern NetTable  *netTab;
extern int       thisNet;
extern char      onConsole;
extern char      loggedIn;  /* Is we logged in?   */
extern char      outFlag; /* Output flag      */
extern char      haveCarrier; /* Do we still got carrier?     */
extern char      modStat; /* Needed so we don't die       */
extern char      WCError;
extern int       thisRoom;
extern int       thisLog;
extern char      *confirm;
extern char      heldMess;
extern char      netDebug;
extern char      *AssignAddress;
extern char      remoteSysop;
extern char     *DomainFlags;
/*
* called_stabilize()
*
* This function attempts to stabilize communication on the receiver end.
*/
char called_stabilize()
  {
  char retVal;
  retVal = getNetBaud();  /* has to handle stroll, too. */
  if (!gotCarrier())
    {
    killConnection();
    retVal = FALSE;

    }
  return retVal;

  }
static int table[2][3] =
  {
    { 7, 13, 69 },
    {68, 79, 35}

  };
/*
* check_for_init()
*
* This function looks for the networking initialization sequence.
*/
char check_for_init(char mode)
  {
  int index;
  int count, timeOut;
  AN_UNSIGNED thisVal, lastVal;
  index = (inNet == STROLL_CALL) ? 1 : 0;
  lastVal = (mode) ? table[index][0] : 0;
  timeOut = (INTERVALS / 2) * (25);
  for (count = 0; count < timeOut; count++)
    {
    if (MIReady())
      {
      thisVal = inp();
      if (thisVal == table[index][0])
      lastVal = table[index][0];
      else if (thisVal == table[index][1])
        {
        if (lastVal == table[index][0]) lastVal = table[index][1];
        else  lastVal = 0;

        }
      else if (thisVal == table[index][2])
        {
        if (lastVal == table[index][1])
          {
          lastVal = AckStabilize(index);
          if (lastVal == ACK) return TRUE;
          else if (lastVal == table[index][2])
          return(char) (AckStabilize(index) == ACK);
          else if (lastVal != table[index][0] &&
          lastVal != table[index][1])
          return (char)FALSE;

          }

        }

      }
    else pause(2);

    }
  return FALSE;

  }
/*
* AckStabilize()
*
* This function tries to stabilize with net caller.
*/
int AckStabilize(int index)
  {
  int temp;
  outMod(~(table[index][0]));
  outMod(~(table[index][1]));
  outMod(~(table[index][2]));
  temp = receive(1);
  if (cfg.BoolFlags.debug) splitF(netLog, "AckStabilize=%d\n",temp);
  return temp;

  }
/*
* AddNetMsgs()
*
* This function integrates messages into the data base.  Options include
* adding the net area or not to the filename and specifying the processing
* function rather than being stuck with a standard processing function.
* Usually the processing function will integrate messages into the message
* data base, although it may do negative mail checking instead.
*/
int AddNetMsgs(char *base, void (*procFn)(void), char zap, int roomNo,
char AddNetArea)
  {
  char tempNm[80];
  int count = 0;
  extern char *READ_ANY;
  if (AddNetArea)
  makeSysName(tempNm, base, &cfg.netArea);
  else
  strCpy(tempNm, base);
  if ((netMisc = safeopen(tempNm, READ_ANY)) == NULL)
    {
    return ERROR;

    }
  getRoom(roomNo);
  /* If reading for mail room, prepare a log buffer. */
  if (roomNo == MAILROOM)
  lBuf = &logTmp;
  else
  lBuf = NULL;
  while (getMessage(getNetChar, TRUE, TRUE, TRUE))
    {
    count++;
    if (strCmpU(cfg.nodeId + cfg.codeBuf, msgBuf.mborig) != SAMESTRING)
    (*procFn)();

    }
  fclose(netMisc);
  if (zap == 1) unlink(tempNm);
  else if (zap == 2 && count != 0) unlink(tempNm);
  return count;

  }
/*
* getNetChar()
*
* This function gets a character from a network temporary file.  The file
* should have been opened elsewhere.
*/
int getNetChar()
  {
  int c;
  c = fgetc(netMisc);
  if (c == EOF) return -1;
  return c;

  }
/*
* inMail()
*
* This function integrates a message into the message database.  It includes
* recognizing bangmail, vortex activation, and bad word scanning.
*/
void inMail()
  {
  extern SListBase BadPeople;
  extern SListBase BadWords;
  /* do we need any code here? */
  if (thisRoom == MAILROOM &&
  (strchr(msgBuf.mbto, '!') != NULL ||
  strchr(msgBuf.mbauth, '!') != NULL))
  MakeRouted();   /* Route.C */
  else if (NotVortex())
    {
    if (cfg.BoolFlags.NetScanBad)
      {
      if (thisRoom != MAILROOM && SearchList(&BadWords, msgBuf.mbtext) != NULL)
        {
        extern char BadMessages[];

        DiscardMessage( strlen(BadMessages)? BadMessages : "discard");

        sPrintf(msgBuf.mbtext,
        "Net message from %s @%s in %s discarded for decency reasons.",
        msgBuf.mbauth, msgBuf.mboname,
        (roomExists(msgBuf.mbroom)) ?
        formRoom(roomExists(msgBuf.mbroom), FALSE, FALSE) :
        msgBuf.mbroom);
        netResult(msgBuf.mbtext);
        return;

        }
      if (thisRoom != MAILROOM && SearchList(&BadPeople, &msgBuf) != NULL)
        {
        extern char BadMessages[];

        DiscardMessage( strlen(BadMessages)? BadMessages : "discard");

        sPrintf(msgBuf.mbtext,
        "Net message from %s @%s in %s discarded By Sysop Option.",
        msgBuf.mbauth, msgBuf.mboname,
        (roomExists(msgBuf.mbroom)) ?
        formRoom(roomExists(msgBuf.mbroom), FALSE, FALSE) :
        msgBuf.mbroom);
        netResult(msgBuf.mbtext);
        return;

        }

      }
    if (AssignAddress != NULL)
    strCpy(msgBuf.mbaddr, AssignAddress);
    putMessage(&logBuf);

    }
  else DiscardMessage("discard");

  }
static int GoodCount, BadCount;
/*
* inRouteMail()
*
* This function handles incoming route mail.
*/
void inRouteMail()
  {
  label oname, domain;
  if (RecipientAvail())
    {
    inMail();

    }
  if (BadCount)
    {
    sPrintf(lbyte(tempMess.mbtext), " on %s _ %s was undeliverable.",
    cfg.nodeName + cfg.codeBuf,
    cfg.codeBuf + cfg.nodeDomain);
    strCpy(tempMess.mbto, msgBuf.mbauth);
    strCpy(oname, msgBuf.mboname);
    strCpy(domain, msgBuf.mbdomain);
    strCpy(tempMess.mbauth, "Citadel");
    strCpy(tempMess.mbroom, "Mail");
    strCpy(tempMess.mbtime, Current_Time());
    strCpy(tempMess.mbdate, formDate());
    sPrintf(tempMess.mbId, "%lu", cfg.newest++ + 1);
    ZeroMsgBuffer(&msgBuf);
    MoveMsgBuffer(&msgBuf, &tempMess);
    netMailOut(TRUE, UseNetAlias(oname, FALSE), domain, FALSE, -1);

    }

  }
/*
* RecipientAvail()
*
* This function checks to see if recipient is here.  This includes override
* handling.
*/
char RecipientAvail()
  {
  void RecAvWork(char *);
  GoodCount = BadCount = 0;
  if (msgBuf.mbdomain[0])
    {
    if (!HasOverrides(&msgBuf))
      {
      RecAvWork(msgBuf.mbto);

      }
    else
      {
      RunList(&msgBuf.mbOverride, RecAvWork);

      }
    return (char)GoodCount;

    }
  return TRUE;

  }
/*
* RecAvWork()
*
* This function does the real work of RecipientAvailable() - split out to
* better handle other recipients.
*/
void RecAvWork(char *name)
  {
  if (PersonExists(name) == ERROR &&
  strCmpU(msgBuf.mbauth, "Citadel") != SAMESTRING)
    {
    BadCount++;
    if( logNetResults )splitF(netLog, "No recipient: %s\n", name);
    if (BadCount == 1)
    sPrintf(tempMess.mbtext, "Your message to %s", name);
    else
    sPrintf(lbyte(tempMess.mbtext), ", %s", name);

    }
  else if (PersonExists(name) != ERROR ||
  strCmpU(msgBuf.mbauth, "Citadel") != SAMESTRING)
  GoodCount++;

  }
/*
* DiscardMessage()
*
* This function prints a message to a discard file.
*/
void DiscardMessage(char *filename)
  {
  extern int outPut;
  extern FILE *upfd;
  extern char *APPEND_TEXT;
  if (redirect(filename))
    {
    mPrintf("%s (%s)", formHeader(), msgBuf.mbsrcId);
    doCR();
    mFormat(msgBuf.mbtext);
    doCR();
    doCR();
    undirect();

    }

  }
char *kip = NULL;
/*
* netController()
*
* This is the main manager of a network session.  It is responsible for
* scheduling calls, noticing incoming calls, exiting network sessions due
* to timeouts or other events, forming error reports for the Aide> room,
* etc.
*/
#define unSetPoll()     free(pollCall)
void mTrPrintf(char *,...);
void netController(int NetStart, int NetLength, MULTI_NET_DATA whichNets,
char mode, UNS_16 flags)
  {
  int x;
  int searcher = 0, start, first;
  SYS_FILE AideMsg;
  long waitTime, InterCallDelay;
  extern char *WRITE_TEXT, *READ_TEXT, *APPEND_TEXT;
  extern void (*NetPrintTarget)();
  SpecialMessage("Status:Network Controller");
  if (cfg.BoolFlags.debug)
     splitF(netLog, "netController(%d,%d, %ld, %d,%d)\n", NetStart, NetLength,whichNets, (int)mode, flags);
  if (loggedIn) /* should only happen on mistake by sysop */
  terminate( /* hangUp == */ TRUE, TRUE);
  NetPrintTarget = mTrPrintf;
  outFlag = OUTOK;    /* for discarding messages correctly */
  inNet = mode;
  setPoll();
  switch (mode)
    {
    case ANYTIME_NET:
    case UNTIL_NET:
    if (!AnyCallsNeeded(whichNets))
      {
      inNet = NON_NET;
      unSetPoll();
      return;

      }
    searcher = AnyIndex;    /* so we don't always start at front */
    break;
    case ANY_CALL:
    while (MIReady()) inp();
    break;

    }
  InterCallDelay = (!(flags & LEISURELY)) ? 2l : 15l;
  SR_Sent = (char *) GetDynamic(SHARED_ROOMS);
  if (logNetResults)
    {
    makeSysName(AideMsg, "netlog.sys", &cfg.netArea);
    if ((netLog = safeopen(AideMsg, APPEND_TEXT)) == NULL)
    netResult("Couldn't open netLog");

    }
  else  netLog = NULL;
  loggedIn = FALSE;     /* Let's be VERY sure.  */
  thisLog = -1;
  splitF(netLog, "\nNetwork Session %s @ %s\n", formDate(), Current_Time());
  SpecialMessage("Network Session");
  logMessage(INTO_NET, "", 0);
  modStat = haveCarrier = FALSE;
  setTime(NetStart, NetLength);
  makeSysName(AideMsg, nMsgTemplate, &cfg.netArea);
  if ((netMsg = safeopen(AideMsg, WRITE_TEXT)) == NULL)
  splitF(netLog, "WARNING: Can't open %s!!!!\n", AideMsg);
  UsedNetMsg = FALSE;
  x = timeLeft();
  do
    {
    /* force at least one time through loop */
    waitTime = (cfg.catChar % 5) + 1;
    while (waitTime > minimum(5, (x/2) ) ) waitTime /= 2;
    if (flags & LEISURELY)
    for (startTimer(WORK_TIMER);
    chkTimeSince(WORK_TIMER) < (waitTime * 60) &&
    !KBReady();)
      {
      if (gotCarrier()) break;
      else BeNice(NET_PAUSE);

      }
    /* This will break us out of a network session if ESC is hit */
    if (KBReady())
    if (getCh() == SPECIAL) break;
    /*
    * In case someone calls while we're doing after-call processing.
    */
    while (gotCarrier())
      {
      modStat = haveCarrier = TRUE;
      called();
      /* CacheMessages(ALL_NETS, TRUE); */

      }
    /* ok, make calls */
    if (cfg.netSize != 0)
      {
      start = searcher;
      do
        {
        if (needToCall(searcher, whichNets))
          {
          ExplainNeed(searcher, whichNets);
          if (!HasPriorityMail(searcher))
          CacheSystem(searcher, FALSE);
          if (callOut(searcher))
            {
            caller();
            if( logNetResults )splitF(netLog, "(%s)\n", Current_Time());
            if (kip != NULL && netBuf.baudCode >= B_5)
              {
              moPuts(kip);
              outMod('\r');
              if( netDebug )splitF(netLog, "debug from modem:\n");
              for (startTimer(WORK_TIMER); chkTimeSince(WORK_TIMER) < 5l || MIReady();)
              splitF(netLog, "%c", inp());

              }
            /* CacheMessages(ALL_NETS, TRUE); */

            }
          for (startTimer(WORK_TIMER); !gotCarrier() &&
          chkTimeSince(WORK_TIMER) < InterCallDelay;)
          ;
          while (gotCarrier())
            {
            modStat = haveCarrier = TRUE;
            called();

            }
          if (whichNets == PRIORITY_MAIL)
            {
            getNet(thisNet, &netBuf);
            netBuf.MemberNets &= ~(PRIORITY_MAIL);
            putNet(thisNet, &netBuf);

            }

          }
        searcher = (searcher + 1) % cfg.netSize;
        if (mode == ANYTIME_NET && timeLeft() < 0 &&
        whichNets != PRIORITY_MAIL)
        break;      /* maintain discipline */

        }
      while (!KBReady() && searcher != start);

      }
    if (mode == ANYTIME_NET || mode == UNTIL_NET)
      {
      if (!AnyCallsNeeded(whichNets)) break;

      }

    }
  while ((x = timeLeft()) > 0);
  splitF(netLog, "\nOut of Networking Mode (%s)\n", Current_Time());
  for (x = 0; x < cfg.netSize; x++)
  if (netTab[x].ntMemberNets & PRIORITY_MAIL)
    {
    getNet(x, &netBuf);
    netBuf.MemberNets &= ~(PRIORITY_MAIL);
    putNet(x, &netBuf);

    }
  if (flags & REPORT_FAILURE)
    {
    if (AnyCallsNeeded(whichNets))
      {
      sPrintf(msgBuf.mbtext,
      "The following systems could not be reached: ");
      for (searcher = 0, first = 1; searcher < cfg.netSize; searcher++)
      if (needToCall(searcher, whichNets))
        {
        if (!first) strCat(msgBuf.mbtext,", ");
        first = FALSE;
        getNet(searcher, &netBuf);
        strCat(msgBuf.mbtext, netBuf.netName);

        }
      strCat(msgBuf.mbtext, ".");
      netResult(msgBuf.mbtext);

      }

    }
  if (inNet == ANYTIME_NET)
    {
    AnyIndex = searcher;    /* so we can start from here later */

    }
  fclose(netMsg);
  netMsg = NULL;
  /* Make the error and status messages generated into an Aide> msg */
  makeSysName(AideMsg, nMsgTemplate, &cfg.netArea);
  if (UsedNetMsg)
    {
    ZeroMsgBuffer(&msgBuf);
    if (access(AideMsg, 4) == -1)
      {
      sPrintf(msgBuf.mbtext, "Where did '%s' go???", AideMsg);
      aideMessage("Net Aide", FALSE);

      }
    else
      {
      ingestFile(AideMsg, &msgBuf);
      aideMessage("Net Aide", FALSE);

      }

    }
  unlink(AideMsg);
  modStat = haveCarrier = FALSE;
  inNet = NON_NET;
  if (logNetResults)
    {
    fclose(netLog);
    netLog = NULL;

    }
  unSetPoll();
  ITL_DeInit();
  free(SR_Sent);
  logMessage(OUTOF_NET, "", 0);
  startTimer(NEXT_ANYNET);      /* anytime net timer */
  getRoom(LOBBY);

  }
static int RunUntil;
/*
* setTime()
*
* This function sets up some global variables for the networker.
*/
void setTime(int NetStart, int NetLength)
  {
  int yr, hr, mins, dy, temp;
  char *mn;
  startTimer(NET_SESSION);
  if (NetLength == 0)
  RunUntil = 0;
  else
    {
    getCdate(&yr, &mn, &dy, &hr, &mins);
    temp = (hr * 60) + mins;
    RunUntil = 60 * (NetLength - abs(temp - NetStart));

    }

  }
/*
* timeLeft()
*
* This function does a rough estimate of how much time left and returns it.
*/
int timeLeft()
  {
  int elapsed;
  elapsed = chkTimeSince(NET_SESSION);
  if (elapsed > RunUntil) return 0;
  return (((RunUntil - elapsed) / 60) + 1);

  }
/*
* callOut()
*
* This function attempts to call some other system.
*/
char callOut(int i)
  {
  getNet(callSlot = i, &netBuf);
  if( logNetResults )splitF(netLog, "Calling %s @ %s (%s): ",
  netBuf.netName, netBuf.netId, Current_Time());
  strCpy(normed, netBuf.netId);   /* Cosmetics */
  strCpy(callerId, netBuf.netId);
  strCpy(callerName, netBuf.netName);
  if (makeCall(TRUE, NO_MENU)) return modStat = haveCarrier = TRUE;
  killConnection();   /* Take SmartModem out of call mode   */
  if( logNetResults )splitF(netLog, "No luck.\n");
  return FALSE;

  }
/*
* moPuts()
*
* This function puts a string out to modem without carr check.
*/
void moPuts(char *s)
  {
  while (*s)
    {
    pause(5);
    outMod(*s++);

    }

  }
/*
* netMessage()
*
* This function will send message via net.  This is a userland function.
*/
int netMessage(int uploading)
  {
  if (!NetValidate(TRUE)) return FALSE;
  ZeroMsgBuffer(&msgBuf);
  if (!netInfo(TRUE)) return FALSE;
  return procMessage(uploading, FALSE);

  }
/*
* writeNet()
*
* This function writes nodes on the net to the screen.  Options include
* showing only local systems and with or without their ids.
*/
void writeNet(char idsAlso, char LocalOnly)
  {
  int rover, count = 0, len;
  int i;
  outFlag = OUTOK;
  mPrintf("Systems on the net:\n ");
  doCR();
  for (rover = 0; outFlag != OUTSKIP && rover < cfg.netSize; rover++)
    {
    if (netTab[rover].ntflags.in_use &&
    (!LocalOnly || netTab[rover].ntflags.local))
      {
      getNet(rover, &netBuf);
      if ((idsAlso || netBuf.MemberNets & ALL_NETS))
        {
        /* mPrintf("%-22s", netBuf.netName); */
        mPrintf("%s", netBuf.netName);
        if (idsAlso)
          {
          #ifdef TURBO_C_VSPRINTF_BUG
          SpaceBug(22 - strLen(netBuf.netName));   /* EEEESH */
          mPrintf("%-22s%-16s%-8s", netBuf.netId,
          (needToCall(rover, ALL_NETS)) ? "<need to call>" : "",
          SupportedBauds[netBuf.baudCode]);
          #else
          mPrintf("%*c%-22s%-16s%-8s", 22 - strLen(netBuf.netName),
          ' ', netBuf.netId,
          (needToCall(rover, ALL_NETS)) ? "<need to call>" : "",
          SupportedBauds[netBuf.baudCode]);
          #endif
          if (netBuf.nbflags.OtherNet) mPrintf("O");
          else if (!(netBuf.MemberNets & ALL_NETS))
          mPrintf("d");
          if (netBuf.nbflags.MassTransfer)
          mPrintf("F");
          mPrintf(" M:");
          for (i = 0; i < 32; i++)
            {
            if ((1l << i) & netBuf.MemberNets) mPrintf("%d ", i + 1);
            };
          doCR();

          }
        else
          {
          if (strLen(netBuf.nbShort) != 0)
            {
            mPrintf(" (%s)", netBuf.nbShort);
            len = strLen(netBuf.nbShort) + 3;

            }
          else len = 0;
          /* mPrintf(", "); */
          if (++count % 3 == 0)
            {
            count = 0;
            doCR();

            }
          else
            {
            #ifdef TURBO_C_VSPRINTF_BUG
            SpaceBug(28 - (len + strLen(netBuf.netName)));   /* EEEESH */
            #else
            mPrintf("%*c", 28 - (len + strLen(netBuf.netName)), ' ');
            #endif

            }

          }

        }

      }

    }
  if (!idsAlso)
  WriteDomainContents();

  }
/*
* netStuff()
*
* This function handles main net menu.
*/
void netStuff()
  {
  extern char *who_str;
  extern char ForceNet;
  TwoNumbers  tmp;
  char  work[50];
  label       who;
  int   logNo;
  MenuId  id, id2;
  long  Redials, duration;
  char *Net_Prompt = "\nNet Menu\n ";
  char  *NetMiscOpts[] =
    {
    "A(dd node to netlist) ", "C(redit setting)               ", "D(ial system)\n",
    "E(dit a node)         ", "I(nitiate Anytime Net Session) ", "L(ocal list)\n",
    "N(et privileges)      ", "P(riority Mail)                ", "R(equest File)\n",
    "S(end File)           ", "U(ntil Done Net Sessions)      ", "V(iew net list)\n",
    "X(Exit)               ", "Y(toggle NetLog)               ", "Z(toggle NetDebug\n",
    #ifdef ZNEEDED
    "Z",
    #endif
    ""

    };
  int AdminPriorityMail(char *line);
  /* If we don't net, don't allow this. */
  if (!cfg.BoolFlags.netParticipant)
    {
    SysopInfoReport(NO_MENU, "Networking is disabled on this installation.\n ");
    return ;

    }
  id = RegisterSysopMenu("netopt.mnu", NetMiscOpts,Net_Prompt);
  do
    {
    outFlag = OUTOK;
    RegisterThisMenu("netopt.mnu", NetMiscOpts);
    SysopMenuPrompt(id, "\n Net function: ");
    switch (GetSysopMenuChar(id))
      {
      case ERROR:
      case 'X':
      CloseSysopMenu(id);
      return;
      case 'Y':  /* toggle net log flag */
        logNetResults = ( logNetResults ) ? FALSE : TRUE;
        mPrintf("Network log is turned %s\n ",(logNetResults) ? "on ":"off");
        break;
      case 'Z':  /* toggle net debug flag */
        netDebug = ( netDebug ) ? FALSE : TRUE;
        mPrintf("Network Debug is turned %s\n ",(netDebug) ? "on ":"off");
        mPrintf("Network log is turned %s\n ",(logNetResults) ? "on ":"off");
        break;
      case 'P':
      getList(AdminPriorityMail, "Systems and Priority Mail",
      NAMESIZE, TRUE);
      break;
      case 'I':
      ForceAnytime();
      sPrintf(work, "now %s.\n ", ForceNet ? "ON" : "OFF");
      SysopInfoReport(id, work);
      break;
      case 'R':   /* File requests */
      SysopRequestString(id, "SYSTEM", who, sizeof who, 0);
      if (!ReqNodeName(NULL, who, NULL, FALSE, FALSE, FALSE,
      FALSE, TRUE, &netBuf))
      break;
      fileRequest();
      break;
      case 'S':   /* File transmissions */
      SysopRequestString(id, "SYSTEM", who, sizeof who, 0);
      if (!ReqNodeName(NULL, who, NULL, FALSE, FALSE, FALSE,
      FALSE, TRUE, &netBuf))
      break;
      getSendFiles(id, who);
      break;
      case 'C':   /* Set users' LD credits */
      if ((logNo = GetUser(who, &logTmp, TRUE)) == ERROR ||
      logNo == cfg.MAXLOGTAB) break;
      sPrintf(work, "Currently %d credits.", logTmp.credit);
      SysopInfoReport(id, work);
      logTmp.credit = (int) SysopGetNumber(id, "HWMYCR", 0l, 1000l);
      sPrintf(work, "Set to %d.", logTmp.credit);
      SysopInfoReport(id, work);
      if (loggedIn  &&  strCmpU(logBuf.lbname, who) == SAMESTRING)
      logBuf.credit = logTmp.credit;
      putLog(&logTmp, logNo);
      break;
      case 'D':   /* Primitive dial out ability.  Don't get excited */
      if (!onConsole) break;
      if (gotCarrier())
        {
        /* carrier already?  just jump in */
        CloseSysopMenu(id);
        interact(FALSE);
        id = RegisterSysopMenu("netopt.mnu", NetMiscOpts, Net_Prompt);
        break;

        }
      /* Get node to call, if none specified abort */
      SysopRequestString(id, "SYSTEM", who, sizeof who, 0);
      if (!ReqNodeName(NULL, who, NULL, FALSE, TRUE, FALSE, FALSE,
      TRUE, &netBuf))
        {
        break;

        }
      /* How many times should we try to call? */
      if ((Redials = SysopGetNumber(id, "REDIAL", 0l,
      65000l)) <= 0l)
      Redials = 1l;       /* allow empty C/R to generate 1. */
      /* Modem should be disabled since we're in CONSOLE mode. */
      id2 = SysopContinual(netBuf.netName, "DIALIT", 50, 13);
      EnableModem(FALSE);
      for (; Redials > 0l; Redials--)
        {
        /* if successful call, start chattin'! */
        if (makeCall(FALSE, id2))
          {
          SysopCloseContinual(id2);
          id2 = NO_MENU;
          CloseSysopMenu(id);
          mputChar(BELL);
          interact(FALSE);
          id = RegisterSysopMenu("netopt.mnu", NetMiscOpts,Net_Prompt);
          break;

          }
        /* This handles an abort from kbd */
        if (KBReady())
          {
          getCh();
          /* Hope this turns off modem */
          outMod(' '); pause(2);
          DisableModem(FALSE);
          break;

          }
        /* printf("Failed\n"); */
        /* Let modem stabilize for a moment. */
        for (startTimer(WORK_TIMER);chkTimeSince(WORK_TIMER) < 3l; )
        ;

        }
      SysopCloseContinual(id2);
      /*
      * If we don't have carrier disable the modem.  We have this
      * check in case sysop wants to perform download within
      * Citadel.
      */
      if (!gotCarrier())
        {
        DisableModem(FALSE);
        modStat = haveCarrier = FALSE;

        }
      break;
      case 'V':   /* View the net list. */
      CloseSysopMenu(id);
      doCR();
      writeNet(TRUE, FALSE);
      if (NeedSysopInpPrompt()) modIn();
      id = RegisterSysopMenu("netopt.mnu", NetMiscOpts, Net_Prompt);
      break;
      case 'L':
      CloseSysopMenu(id);
      writeNet(TRUE, TRUE);
      if (NeedSysopInpPrompt()) modIn();
      id = RegisterSysopMenu("netopt.mnu", NetMiscOpts, Net_Prompt);
      break;
      case 'A':   /* Add a new node to the list */
      addNetNode();
      break;
      case 'E':   /* Edit a node that is on the list */
      SysopRequestString(id, "EDITSY", who, sizeof who,0);
      if (!ReqNodeName(NULL, who, NULL, FALSE, TRUE, FALSE, TRUE, TRUE,
      &netBuf))
      break;
      CloseSysopMenu(id);
      editNode();
      id = RegisterSysopMenu("netopt.mnu", NetMiscOpts, Net_Prompt);
      break;
      case 'N':   /* Give someone net privileges. */
      NetPrivs(who);
      break;
      case 'U':
      id2 = SysopContinual(" Net Session ", "", 30, 4);
      SysopContinualString(id2, "MEMBER", work, 3, 0);
      tmp.first = atoi(work);
      if (tmp.first < 1 || tmp.first > MAX_NET - 1)
        {
        if (strLen(work) != 0)
        SysopError(id2, "Illegal Member Net");

        }
      else
        {
        if (SearchList(&UntilNetSessions, &tmp) != NULL)
          {
          SysopInfoReport(NO_MENU, "Net session deactivated.\n");
          KillData(&UntilNetSessions, &tmp);

          }
        else
          {
          SysopContinualString(id2, "DURATI", work, 4, 0);
          duration = atol(work);
          if (duration < 1l)
          SysopError(id2, "Illegal Duration");
          else
          AddData(&UntilNetSessions,MakeTwo(tmp.first,duration),
          NULL, FALSE);

          }

        }
      SysopCloseContinual(id2);
      break;
      #ifdef ZNEEDED
      case 'Z':
      inNet = NORMAL_NET;
      AddNetMsgs("tempmail.$$$", inMail, FALSE, MAILROOM, TRUE);
      inNet = NON_NET;
      break;
      #endif

      }

    }
  while (onLine());

  }
/*
* AdminPriorityMail()
*
* This handles the administration of priority mail.
*/
int AdminPriorityMail(char *system)
  {
  if (searchNameNet(system, &netBuf) == ERROR)
    {
    SysopError(NO_MENU, "No such system\n");

    }
  else
    {
    if (!(netBuf.nbflags.normal_mail ||
    netBuf.nbflags.HasRouted ||
    DomainFlags[thisNet]))
      {
      SysopError(NO_MENU, "No outgoing mail.\n");
      return TRUE;

      }
    else if ((netBuf.MemberNets & PRIORITY_MAIL))
      {
      SysopInfoReport(NO_MENU, "Priority mail deactivated.\n");
      PriorityMail--;
      netBuf.MemberNets &= ~(PRIORITY_MAIL);

      }
    else
      {
      PriorityMail++;
      netBuf.MemberNets |= (PRIORITY_MAIL);

      }
    putNet(thisNet, &netBuf);

    }
  return TRUE;

  }
/*
* NetPrivs()
*
* This will setup net privs for someone.
*/
void NetPrivs(label who)
  {
  int logNo, result;
  if ((logNo = GetUser(who, &logTmp, TRUE)) == ERROR) return;
  if (logNo == cfg.MAXLOGTAB)
    {
    result = DoAllQuestion("GVNTPR","TANTPR");
    if (result == ERROR) return;
    for (logNo = 0; logNo < cfg.MAXLOGTAB; logNo++)
      {
      getLog(&logTmp, logNo);
      if (!onConsole) mPrintf(".");
      if (logTmp.lbflags.L_INUSE && logTmp.lbflags.NET_PRIVS != result)
        {
        logTmp.lbflags.NET_PRIVS = result;
        putLog(&logTmp, logNo);

        }

      }
    return;

    }
  Output_Citadel_Message((logTmp.lbflags.NET_PRIVS) ? "USRNNT" : "USRNET",
  (long)who, NULL, NULL);
  if (!SysopGetYesNo(NO_MENU, NULL, "CONFRM"))   return;
  logTmp.lbflags.NET_PRIVS = !logTmp.lbflags.NET_PRIVS;
  if (strCmpU(logTmp.lbname, logBuf.lbname) == SAMESTRING)
  logBuf.lbflags.NET_PRIVS = logTmp.lbflags.NET_PRIVS;
  putLog(&logTmp, logNo);

  }
static char AddedFiles;
/*
* getSendFiles()
*
* This will get the files from the sysop to send to another system.
*/
void getSendFiles(MenuId id, label sysName)
  {
  SYS_FILE       sysFile;
  char     temp[10];
  extern char    *APPEND_ANY;
  sPrintf(temp, "%d.sfl", thisNet);
  makeSysName(sysFile, temp, &cfg.netArea);
  if ((upfd = safeopen(sysFile, APPEND_ANY)) == NULL)
    {
    SysopPrintf(id, "Couldn't open %s for update?\n ", sysFile);
    return ;

    }
  sPrintf(msgBuf.mbtext, "Files to send to %s", sysName);
  AddedFiles = FALSE;
  getList(addSendFile, msgBuf.mbtext, 126, TRUE);
  fclose(upfd);
  if (AddedFiles)
    {
    netBuf.nbflags.send_files = TRUE;
    putNet(thisNet, &netBuf);

    }

  }
/*
* addSendFile()
*
* This is a work function, called indirectly by getList().
*/
int addSendFile(char *Files)
  {
  struct fl_send sendWhat;
  extern MenuId GetListId;
  if (sysGetSendFilesV2(GetListId, Files, &sendWhat))
    {
    putSLNet(sendWhat, upfd);
    AddedFiles = TRUE;

    }
  return TRUE;

  }
/*
* addNetNode()
*
* This adds a node to the net listing.
*/
void addNetNode()
  {
  int searcher, gen;
  char  goodAnswer, found;
  extern char *ALL_LOCALS;
  MenuId id;
  id = SysopContinual("", "", 2 * NAMESIZE, 5);
  for (searcher = 0; searcher < cfg.netSize; searcher++)
  if (netTab[searcher].ntflags.in_use == FALSE) break;
  if (searcher != cfg.netSize)
    {
    getNet(searcher, &netBuf);
    found = TRUE;
    gen = (netBuf.nbGen + 1) % NET_GEN;

    }
  else
    {
    found = FALSE;
    gen = 0;

    }
  killNetBuf(&netBuf);
  zero_struct(netBuf);  /* Useful initialization       */
  initNetBuf(&netBuf);
  /* Get a unique name */
  do
    {
    SysopContinualString(id, "SYSTEM", netBuf.netName, NAMESIZE, 0);
    if (strLen(netBuf.netName) == 0)
      {
      SysopCloseContinual(id);
      return;

      }
    if ((goodAnswer = strCmpU(ALL_LOCALS, netBuf.netName)) == 0)
    SysopError(id, "Sorry, reserved name\n ");
    else if ((goodAnswer = searchNameNet(netBuf.netName, &netTemp)==ERROR) == 0)
      {
      sPrintf(msgBuf.mbtext, "Sorry, %s is already in use.\n ",
      netBuf.netName);
      SysopError(id, msgBuf.mbtext);

      }
    else if (strchr(netBuf.netName, '_') != NULL)
      {
      goodAnswer = FALSE;
      SysopError(id, "Please don't use '_' in the system name.\n ");

      }

    }
  while (!goodAnswer);
  /* Get a unique ID */
  do
    {
    goodAnswer = TRUE;
    SysopContinualString(id, "SYSTID", netBuf.netId, NAMESIZE, 0);
    if (strLen(netBuf.netId) == 0)
      {
      SysopCloseContinual(id);
      return;

      }
    if (searchNet(netBuf.netId, &netTemp) != ERROR)
      {
      sPrintf(msgBuf.mbtext, "Sorry, %s is already in use.\n ",
      netBuf.netId);
      SysopError(id, msgBuf.mbtext);
      goodAnswer = FALSE;

      }

    }
  while (!goodAnswer);
  netBuf.baudCode = (int) SysopGetNumber(id, "BAUDST", 0l, 8l);
  netBuf.nbflags.local  = SysopGetYesNo(id, NULL, "ISSYSL");
  netBuf.nbflags.in_use = TRUE;
  netBuf.MemberNets   = 1;     /* Default */
  netBuf.nbGen    = gen;   /* Update generation #  */
  netBuf.nbRoute    = -1;
  netBuf.nbflags.RouteTo  = TRUE;
  netBuf.nbflags.RouteFor = TRUE;
  if (!found)
    {
    if (cfg.netSize != 0)
    netTab = (NetTable *)
    realloc(netTab, sizeof (*netTab) * ++cfg.netSize);
    else
    netTab = (NetTable *)
    GetDynamic(sizeof(*netTab) * ++cfg.netSize);
    searcher = cfg.netSize - 1;
    netTab[searcher].netTRooms = (SharedRoom *) GetDynamic(SR_BULK);

    }
  putNet(searcher, &netBuf);
  InitVNode(searcher);
  DomainInit(FALSE);    /* so we can redirect easily enough */
  SysopCloseContinual(id);

  }
/*
* addNetMem()
*
* This adds nets to this system's list.
*/
int addNetMem(char *netnum)
  {
  int num;
  MULTI_NET_DATA temp;
  num = atoi(netnum);
  if (num < 1 || num > MAX_NET - 1)
    {
    SysopError(NO_MENU, "There are only 31 nets to choose from.\n");
    return TRUE;

    }
  temp = 1l;
  temp <<= (num-1);
  netBuf.MemberNets |= temp;
  return TRUE;

  }
/*
* subNetMem()
*
* This takes nets from a system's list.
*/
int subNetMem(char *netnum)
  {
  int num;
  MULTI_NET_DATA temp;
  num = atoi(netnum);
  if (num < 1 || num > MAX_NET - 1)
    {
    SysopError(NO_MENU, "There are only 31 nets to choose from.");
    return TRUE;

    }
  temp = 1l;
  temp <<= (num-1);
  temp = ~temp;
  netBuf.MemberNets &= temp;
  return TRUE;

  }
/*
* editNode()
*
* This function will edit a net node.
*/
void editNode()
  {
  label  temp2;
  char   title[50], work[50], temp[NAMESIZE*3];
  int    place, compress;
  MenuId id;
  char   exttemp;
  char   *NetEditOpts[] =
    {
    "A(ccess setting)  ", "B(aud code change)    ", "C(ondensed name)\n",
    "D(ownload toggle) ", "E(xternal Dialer)     ", "F(ast Transfers)\n",
    "I(D change)       ", "K(ill node from list) ", "L(ocal setting)\n",
    "M(ember Nets)     ", "N(ame change)         ", "O(thernet toggle)\n",
    "P(asswords)       ", "R(ooms shared)        ", "S(pine settings)\n",
    "V(alues)          ", "X(Exit)\n",
    #ifdef NEEDED
    "ZKludge",
    #endif
    ""

    };
  place = thisNet;    /* this is really a kludge, but for now will serve */
  if (!NeedSysopInpPrompt())  /* rather icky, really.  fix someday? */
  NodeValues(NO_MENU);
  sPrintf(title, "\nEditing %s\n", netBuf.netName);
  id = RegisterSysopMenu("netedit.mnu", NetEditOpts, title);
  while (onLine())
    {
    outFlag = OUTOK;
    sPrintf(work, "\n (%s) edit fn: ", netBuf.netName);
    SysopMenuPrompt(id, work);
    switch (GetSysopMenuChar(id))
      {
      case ERROR:
      case 'X':
      putNet(place, &netBuf);
      CloseSysopMenu(id);
      return;
      case 'E':
      exttemp = netBuf.nbflags.ExternalDialer;
      if ((netBuf.nbflags.ExternalDialer =
      SysopGetYesNo(id, NULL, "USEDIL")))
        {
        SysopRequestString(id, "DIALER",netBuf.access,sizeof netBuf.access,0);
        }
      else if (exttemp) /* clear old information */
      netBuf.access[0] = 0;
      break;
      case 'A':
      SysopRequestString(id, "ACCESS", netBuf.access,
      sizeof netBuf.access, 0);
      break;
      case 'B':
      netBuf.baudCode = (int) SysopGetNumber(id, "BAUDST", 0l, 8l);
      break;
      case 'C':
      temp[0] = temp[1] = temp[2] = '\0';
      SysopRequestString(id, "CONSNM", temp, 3, 0);
      if (searchNameNet(temp, &netTemp) != ERROR)
        {
        sPrintf(work, "'%s' is already in use.", temp);
        SysopError(id, work);

        }
      else
      strCpy(netBuf.nbShort, temp);
      break;
      case 'D':
      sPrintf(work, "for %s %s.\n ",
      netBuf.netName, netBuf.nbflags.NoDL ? "ON" : "OFF");
      SysopInfoReport(id, work);
      netBuf.nbflags.NoDL = !netBuf.nbflags.NoDL;
      break;
      case 'F':
      netBuf.nbflags.MassTransfer = !netBuf.nbflags.MassTransfer;
      if (netBuf.nbflags.MassTransfer)
        {
        /* kludges - next major release make into char */
        netBuf.nbflags.Zoo = FALSE;
        netBuf.nbflags.Zip = FALSE;
        netBuf.nbflags.Lha = FALSE;
        netBuf.nbflags.Arc = FALSE;
        if ((compress = GetUserCompression()) == NO_COMP)
          {
          netBuf.nbflags.MassTransfer = FALSE;
          RegisterThisMenu("netedit.mnu", NetEditOpts);
          break;

          }
        switch (compress)
          {
          case ZIP_COMP: netBuf.nbflags.Zip = TRUE;break;
          case ZOO_COMP: netBuf.nbflags.Zoo = TRUE;break;
          case ARC_COMP: netBuf.nbflags.Arc = TRUE;break;
          case LHA_COMP: netBuf.nbflags.Lha = TRUE;break;

          }
        RegisterThisMenu("netedit.mnu", NetEditOpts);

        }
      sPrintf(work, "for %s %s.\n ", netBuf.netName,
      netBuf.nbflags.MassTransfer ? "ON" : "OFF");
      SysopInfoReport(id, work);
      if (netBuf.nbflags.MassTransfer)
        {
        MakeNetCacheName(temp, thisNet);
        mkdir(temp);

        }
      putNet(thisNet, &netBuf);
      /* more work here? */
      break;
      case 'R':
      CloseSysopMenu(id);
      EachSharedRoom(thisNet, DumpRoom, DumpVRoom, NULL);
      if (onConsole) modIn();
      sPrintf(title, " Editing %s ", netBuf.netName);
      id = RegisterSysopMenu("netedit.mnu", NetEditOpts, title);
      break;
      case 'N':
      SysopRequestString(id, "SYSTEM", temp, NAMESIZE, 0);
      if (strLen(temp) != 0) strCpy(netBuf.netName, temp);
      if (SysopGetYesNo(id, NULL, "NEWSYS"))
        {
        netBuf.nbGen = (netBuf.nbGen + 1) % NET_GEN;
        KillTempFiles(thisNet);
        ClearRoomSharing();

        }
      break;
      case 'I':
      SysopRequestString(id, "SYSTID", temp, NAMESIZE, 0);
      if (strLen(temp) != 0) strCpy(netBuf.netId, temp);
      if (SysopGetYesNo(id, NULL, "NEWSYS"))
        {
        netBuf.nbGen = (netBuf.nbGen + 1) % NET_GEN;
        KillTempFiles(thisNet);
        ClearRoomSharing();

        }
      break;
      case 'K':
      if (netBuf.nbflags.normal_mail)
        {
        sPrintf(work, "There is outgoing mail outstanding.\n ");
        SysopInfoReport(id, work);

        }
      if (netBuf.nbflags.room_files)
        {
        sPrintf(work, "There are file requests outstanding.\n ");
        SysopInfoReport(id, work);

        }
      if (SysopGetYesNo(id, NULL, "CONFRM"))
        {
        netBuf.nbflags.in_use = FALSE;
        putNet(place, &netBuf);
        KillTempFiles(thisNet);
        KillCacheFiles(thisNet);
        CloseSysopMenu(id);
        return;

        }
      break;
      case 'L':
      netBuf.nbflags.local = SysopGetYesNo(id, NULL, "ISSYSL");
      break;
      case 'P':
      CloseSysopMenu(id);
      Output_Citadel_Message("NTPSWO",
                             (long)netBuf.OurPwd,
                             (long)netBuf.TheirPwd, NULL);
      if (getXString("NTNPWO", temp2, NAMESIZE, "", ""))
      strCpy(netBuf.OurPwd, temp2);
      if (getXString("NTNPWT", temp2, NAMESIZE, "", ""))
      strCpy(netBuf.TheirPwd, temp2);
      id = RegisterSysopMenu("netedit.mnu", NetEditOpts, title);
      break;
      case 'M':
      getList(addNetMem, "Nets to add to this system's member list",
      5, TRUE);
      getList(subNetMem,"Nets to take off this system's member list",
      5, TRUE);
      break;
      case 'S':
      Output_Citadel_Message("ISPINE",(long)netBuf.netName,NULL,NULL);
      if (!(netBuf.nbflags.spine = SysopGetYesNo(id, NULL, "CONFRM")))
        {
        Output_Citadel_Message("SPINEU",(long)netBuf.netName,NULL,NULL);
        netBuf.nbflags.is_spine =
        SysopGetYesNo(id, NULL, "CONFRM");

        }
      else
      netBuf.nbflags.is_spine = FALSE;
      break;
      case 'O':
      Output_Citadel_Message((netBuf.nbflags.OtherNet) ? "NOTHER" : "OTHERN",NULL, NULL, NULL);
      netBuf.nbflags.OtherNet = !netBuf.nbflags.OtherNet;
      break;
      case 'V':
      NodeValues(id);
      break;
      #ifdef NEEDED
      case 'Z':
      netBuf.nbHiRouteInd = (int) getNumber("KLUDGE", 0l, 255l);
      netBuf.nbflags.HasRouted = TRUE;
      break;
      #endif

      }

    }

  }
/*
* KillTempFiles()
*
* This eliminates unneeded temp files for dead node.
*/
void KillTempFiles(int which)
  {
  label    temp;
  SYS_FILE temp2;
  sPrintf(temp, "%d.ml", which);
  makeSysName(temp2, temp, &cfg.netArea);
  unlink(temp2);
  netBuf.nbflags.normal_mail = FALSE;
  sPrintf(temp, "%d.rfl", which);
  makeSysName(temp2, temp, &cfg.netArea);
  unlink(temp2);
  netBuf.nbflags.room_files = FALSE;
  sPrintf(temp, "%d.sfl", which);
  makeSysName(temp2, temp, &cfg.netArea);
  unlink(temp2);
  netBuf.nbflags.send_files = FALSE;
  sPrintf(temp, "%d.vtx", which);
  makeSysName(temp2, temp, &cfg.netArea);
  unlink(temp2);
  InitVNode(thisNet);

  }
/*
* ClearRoomSharing()
*
* This clears room sharing out completely for this node.
*/
void ClearRoomSharing()
  {
  int i;
  for (i = 0; i < SHARED_ROOMS; i++)
  netBuf.netRooms[i].srgen = 0;

  }
/*
* NodeValues()
*
* This function prints out the values for the current node.
*/
void NodeValues(MenuId id)
  {
  int i, first;
  MULTI_NET_DATA h;
  sPrintf(msgBuf.mbtext, "\n Node #%d: %s", thisNet, netBuf.netName);
  if (strLen(netBuf.nbShort))
  sPrintf(lbyte(msgBuf.mbtext), " (%s)", netBuf.nbShort);
  sPrintf(lbyte(msgBuf.mbtext), "\n Id: %s (%slocal @ %s)\n ",
  netBuf.netId,
  netBuf.nbflags.local ? "" : "non",
  SupportedBauds[netBuf.baudCode]);
  if (netBuf.nbflags.ExternalDialer)
  sPrintf(lbyte(msgBuf.mbtext), "External Dialer Information: %s\n ", netBuf.access);
  if (strLen(netBuf.access) != 0 && !netBuf.nbflags.ExternalDialer)
  sPrintf(lbyte(msgBuf.mbtext), "Access: %s\n ", netBuf.access);
  if (netBuf.nbflags.spine)
  sPrintf(lbyte(msgBuf.mbtext), "We are a spine for this system\n ");
  else if (netBuf.nbflags.is_spine)
  sPrintf(lbyte(msgBuf.mbtext), "This system is a spine\n ");
  if (netBuf.nbflags.OtherNet)
  sPrintf(lbyte(msgBuf.mbtext), "This system is designated as OtherNet.\n ");
  if (netBuf.nbflags.normal_mail)
  sPrintf(lbyte(msgBuf.mbtext), "There is outgoing Mail>.\n ");
  if (netBuf.nbflags.HasRouted)
  sPrintf(lbyte(msgBuf.mbtext), "There is outgoing RouteMail.\n ");
  if (DomainFlags[thisNet])
  sPrintf(lbyte(msgBuf.mbtext), "There is outgoing DomainMail.\n ");
  if (netBuf.nbflags.room_files)
  sPrintf(lbyte(msgBuf.mbtext), "There are file requests outstanding.\n ");
  if (netBuf.nbflags.send_files)
  sPrintf(lbyte(msgBuf.mbtext), "There are files to be sent.\n ");
  if (netBuf.nbflags.MassTransfer)
  sPrintf(lbyte(msgBuf.mbtext), "Fast Transfers on (using %s).\n ",
  GetCompEnglish(GetCompression(thisNet)));
  if (netBuf.MemberNets != 0l)
    {
    sPrintf(lbyte(msgBuf.mbtext), "This system is assigned to the following nets: ");
    for (i = 0, first = 1, h = 1l; i < MAX_NET; i++)
      {
      if (h & netBuf.MemberNets)
        {
        if (!first)
        sPrintf(lbyte(msgBuf.mbtext), ", ");
        else first = FALSE;
        sPrintf(lbyte(msgBuf.mbtext), "%d", i+1); /* Yes - +1. Number the bits starting with 1 */

        }
      h <<= 1;

      }
    sPrintf(lbyte(msgBuf.mbtext), ".\n ");

    }
  else sPrintf(lbyte(msgBuf.mbtext), "This system is currently disabled.\n ");
  sPrintf(lbyte(msgBuf.mbtext), "Last connected: %s\n", AbsToReadable(netBuf.nbLastConnect));
  SysopDisplayInfo(id, msgBuf.mbtext, " Values ");

  }
/*
* fileRequest()
*
* This handles the administration of requesting files from another system.
*/
void fileRequest()
  {
  struct fl_req file_data;
  label    data;
  char     loc[100], *c, *work;
  SYS_FILE fn;
  char     abort;
  FILE     *temp;
  int      place;
  extern char *APPEND_ANY;
  char     ambiguous, again;
  MenuId   id;
  place = thisNet;    /* again, a kludge to be killed later */
  id = SysopContinual("", "", 75, 10);
  SysopContinualString(id, "ROMNAM", file_data.room, NAMESIZE, 0);
  if (strLen(file_data.room) == 0)
    {
    SysopCloseContinual(id);
    return;

    }
  SysopContinualString(id, "EFILEN", loc, sizeof loc, 0);
  if (strLen(loc) == 0)
    {
    SysopCloseContinual(id);
    return;

    }
  ambiguous = !(strchr(loc, '*') == NULL && strchr(loc, '?') == NULL &&
  strchr(loc, ' ') == NULL);
  abort = !netGetAreaV2(id, loc, &file_data, ambiguous);
  if (!abort)
    {
    sPrintf(data, "%d.rfl", place);
    makeSysName(fn, data, &cfg.netArea);
    if ((temp = safeopen(fn, APPEND_ANY)) == NULL)
      {
      SysopPrintf(id, "Couldn't append to '%s'????", fn);

      }
    else
      {
      work = loc;
      do
        {
        again = (c = strchr(work, ' ')) != NULL;
        if (again) *c = 0;
        strCpy(file_data.roomfile, work);
        if (ambiguous) strCpy(file_data.filename, work);
        fwrite(&file_data, sizeof (file_data), 1, temp);
        if (again) work = c + 1;

        }
      while (again);
      netBuf.nbflags.room_files = TRUE;
      putNet(place, &netBuf);
      fclose(temp);

      }

    }
  SysopCloseContinual(id);

  }
/*
* roomsShared()
*
* This function returns TRUE if this system has a room with new data to share
* (orSomething).
*/
char roomsShared(int slot)
  {
  int ROutGoing(SharedRoom *room, int system, int index, int roomslot,void *d);
  char OutGoing;
  /* We only want to make one "successful" call per voluntary net session */
  if ((inNet == UNTIL_NET || inNet == NORMAL_NET || inNet == ANYTIME_NET ) &&
  pollCall[slot] <= 0)
  return FALSE;
  /*
  * Rules:
  * We check each slot of the shared rooms list for this node.  For
  * each one that is in use, we do the following:
  *   HOSTS ARE OBSOLETE!
  * a) if we are regional host for the room and other system is
  *    a backbone, then don't assume we need to call.
  * b) if we are backboning the room, check to see what status of this
  *    room for other system is.
  *    1) If we are Passive Backbone, then we need not call.
  *    2) If we are Active Backbone, then do call.
  *    3) The Regional Host looks screwy.  This may be a bug.
  * c) If none of the above applies, implies we are a simple Peon, so
  *    we simply check to see if we have outgoing messages, and if so,
  *    return TRUE indicating that we need to call; otherwise, continue
  *    search.
  *
  * LATER NOTE: now this is split up due to the use of EachSharedRoom.
  */
  OutGoing = FALSE;
  EachSharedRoom(slot, ROutGoing, VRNeedCall, &OutGoing);
  return OutGoing;

  }
/*
* ROutGoing()
*
* This decides if the system in question needs to be called due to the
* situation of the rooms.
*/
int ROutGoing(SharedRoom *room, int system, int index, int roomslot, void *d)
  {
  char *arg;
  arg = d;
  if (roomTab[roomslot].rtShareType != PEON)
    {
    if (GetMode(room->mode) == PASS_BACKBONE)
    return TRUE;
    else if (GetMode(room->mode) == REG_HOST ||
    GetMode(room->mode) == ACTIVE_BACKBONE)
      {
      if (inNet == NORMAL_NET)
        {
        *arg = TRUE;
        return ERROR;

        }

      }

    }
  if (roomTab[roomslot].rtlastNet > room->lastMess)
    {
    *arg = TRUE;
    return ERROR;

    }
  if (GetFA(room->mode))
    {
    *arg = TRUE;
    return ERROR;

    }
  return TRUE;

  }
/*
* DumpRoom()
*
* This dumps out information concerning a shared room, such as the status
* and the message stuff.
*/
int DumpRoom(SharedRoom *room, int system, int index, int roomslot, void *d)
  {
  char cmd, *s1, *s2, *s3, *name, doit;
  mPrintf("%-22sRelationship: ", roomTab[roomslot].rtname);
  Addressing(system, index, &cmd, &s1, &s2, &s3, &name, &doit);
  mPrintf(name);
  mPrintf(" (last sent=%ld, netlast=%ld)",
  room->lastMess, roomTab[roomslot].rtlastNet);
  if (GetFA(netTab[system].netTRooms[index].mode))
  mPrintf("*");
  mPrintf("\n ");
  return TRUE;

  }
/*
* netResult()
*
* This will put a message to the net msg holder, building a message for the
* Aide room.
*/
void netResult(char *msg)
  {
  if (netMsg != NULL)
    {
    fprintf(netMsg, "(%s) %s\n\n", Current_Time(), msg);
    fflush(netMsg);
    UsedNetMsg = TRUE;

    }

  }
/*
* netInfo()
*
* This function acquires necessary info from the user when entering a message.
*/
char netInfo(char GetName)
  {
  int    cost;
  extern char *ALL_LOCALS;
  extern char *R_SH_MARK;
  label  domain = "";
  char   sys[NAMESIZE * 2];
  char   isdomain, *address;
  char   work[45];
  if (thisRoom == MAILROOM)
    {
    strCpy(sys, msgBuf.mbaddr);
    if (!ReqNodeName("SYSTSD", sys, domain, (char) aide,
    FALSE, GetName, FALSE, FALSE, &netBuf))
    return FALSE;
    isdomain = (domain[0] != 0);
    if (strCmpU(sys, ALL_LOCALS) != SAMESTRING)
      {
      if (strCmpU(domain, cfg.nodeDomain + cfg.codeBuf) == SAMESTRING &&
      (strCmpU(sys, cfg.nodeName + cfg.codeBuf) == SAMESTRING ||
      strCmpU(sys, UseNetAlias(cfg.nodeName+cfg.codeBuf, TRUE))
      == SAMESTRING))
        {
        mPrintf("Hey, that's this system!\n ");
        return FALSE;

        }
      cost = (isdomain) ? FindCost(domain) : !netBuf.nbflags.local;
      if (logBuf.credit < cost)
        {
        if (HalfSysop())
          {
          logBuf.credit += cost;

          }
        else
          {
          Output_Citadel_Message("NOCRDT", NULL, NULL, NULL);
          return FALSE;

          }

        }
      if (isdomain)
        {
        sPrintf(work, "%s _ %s", sys, domain);
        address = work;

        }
      else address = sys;
      if (!isdomain && netBuf.nbflags.OtherNet)
        {
        mPrintf("%s address", netBuf.netName);
        getNormStr("", msgBuf.mbOther, O_NET_PATH_SIZE, 0);
        if (strLen(msgBuf.mbOther) == 0) return FALSE;

        }

      }
    else
      {
      address = ALL_LOCALS;

      }

    }
  else
    {
    if (!roomBuf.rbflags.SHARED)
      {
      mPrintf("Sorry, this is not a network room\n ");
      return FALSE;

      }
    address = R_SH_MARK;
    strCpy(msgBuf.mboname, cfg.codeBuf + cfg.nodeName);
    strCpy(msgBuf.mbdomain, cfg.codeBuf + cfg.nodeDomain);

    }
  strCpy(msgBuf.mbaddr, address);
  return TRUE;

  }
/*
* killConnection()
*
* Zaps carrier for network.
*/
void killConnection()
  {
  HangUp(TRUE);
  modStat = haveCarrier = FALSE;
  while (MIReady()) inp();    /* Clear buffer of garbage */

  }
/*
* setPoll()
*
* This allows us to make sure we don't poll hosts too often during a net
* session.
*/
void setPoll()
  {
  int rover;
  pollCall = GetDynamic(cfg.netSize);
  for (rover = 0; rover < cfg.netSize; rover++)
    {
    pollCall[rover] = 1;

    }

  }
/*
* makeCall()
*
* This handles the actual task of dialing the modem.
*/
int makeCall(char EchoErr, MenuId id)
  {
  char  call[80];
  label blip1;
  int   bufc, result;
  char  buf[30], c, viable;
  char  ourArea[4], targetArea[4];
  while (MIReady()) inp();
  AreaCode(netBuf.netId, targetArea);
  AreaCode(cfg.nodeId + cfg.codeBuf, ourArea);
  if (!netBuf.nbflags.ExternalDialer)
    {
    setNetCallBaud(netBuf.baudCode, "makeCall");
    normId(netBuf.netId, blip1);
    strCpy(call, cfg.codeBuf + cfg.DialPrefixes[minimum(netBuf.baudCode, cfg.sysBaud)]);
    if (strLen(netBuf.access) != 0)
      {
      /* don't need to check extdial*/
      strCat(call, netBuf.access);

      }
    else if (!netBuf.nbflags.local)
      {
      strCat(call, "1");
      /* LD within same area code? (courtesy farokh irani) */
      if (strCmp(targetArea, ourArea) == SAMESTRING)
      strCat(call, blip1 + 5);
      else
      strCat(call, blip1 + 2);

      }
    else
      {
      /* local but different area codes?  (e.g., NYC) */
      /* again courtesy farokh irani */
      if (strCmp(targetArea, ourArea) != SAMESTRING)
      strCat(call, blip1 + 2);
      else
      strCat(call, blip1 + 5);

      }
    strCat(call, cfg.codeBuf + cfg.netSuffix);
    switch (RottenDial(call))
      {
      case FALSE: moPuts(call); break;
      case TRUE:  break;
      case ERROR: return FALSE;

      }
    for (startTimer(WORK_TIMER), bufc = 0, viable = TRUE;
    chkTimeSince(WORK_TIMER) < ((netBuf.nbflags.local) ? 40l : LD_Delay)
    && viable;)
      {
      if (gotCarrier()) break;
      /* Parse incoming string from modem -- call progress detection */
      if (KBReady()) viable = FALSE;
      if (MIReady())
        {
        if ((c = inp()) == '\r')
          {
          buf[bufc] = 0;
          switch ((result = ResultVal(buf)))
            {
            case R_NODIAL:
            case R_NOCARR:
            case R_BUSY:
            if( logNetResults )
              {
              if (EchoErr ) splitF(netLog, "(%s) ", buf);
              else SysopPrintf(id, "\n%s", buf);
              };
            viable = FALSE; break;
            case R_300:
            case R_1200:
            case R_2400:
            case R_4800:
            case R_9600:
            case R_14400:
            case R_19200:
            if ((minimum(netBuf.baudCode, cfg.sysBaud)) != result)
              {
              setNetCallBaud(result,"makeCall");
              if( logNetResults )splitF(netLog, "(Mismatch: %s, adjusting.)\n", buf);

              }
            break;

            }
          bufc = 0;

          }
        else
          {
          if (bufc > 28) bufc = 0;
          else if (c != '\n')
            {
            buf[bufc++] = c;

            }

          }

        }
      if( viable ) BeNice(INUSE_PAUSE);  /* give up some CPU time */

      }
    if (gotCarrier())
    return TRUE;

    }
  else
    {
    return DialExternal(&netBuf);

    }
  return FALSE;

  }
void ExplainNeed(int i, MULTI_NET_DATA x)
  {
  char c0,c1,c2;
  if (!cfg.BoolFlags.debug) return;
  c0 = netTab[i].ntShort[0];
  if( !isprint(c0))c0 = ' ';
  c1 = netTab[i].ntShort[1];
  c2 = netTab[i].ntShort[2];
  if( !isprint(c0))c0 = ' ';
  if( !isprint(c1))c1 = ' ';
  if( !isprint(c2))c2 = ' ';
  splitF(netLog," Networking with:( %c%c%c ) \n",c0,c1,c2);
  splitF(netLog, "slot %d%sin use is%sspine we are%sa spine \n",
  i, netTab[i].ntflags.in_use      ? " ":" not ",
  netTab[i].ntflags.is_spine       ? " ":" not ",
  netTab[i].ntflags.spine          ? " ":" not ");
  splitF(netLog, " MN%ld has%smail\n",  netTab[i].ntMemberNets & x,
  netTab[i].ntflags.normal_mail ? " ":" no ");
  splitF(netLog, "%sfile requests %ssend requests has%srooms to share\n",
  netTab[i].ntflags.room_files ? " ":" no ",
  netTab[i].ntflags.send_files ? " ":" no ",
  roomsShared(i) ? " ":" no ");
  splitF(netLog, " Domain Flags:%d has%srouted for %s mode \n",
  DomainFlags[i],
  netTab[i].ntflags.HasRouted ? " ":" no ",
  (inNet == NON_NET)    ? "NON_NET":
  (inNet == NORMAL_NET) ? "NORMAL_NET" :
  (inNet == ANYTIME_NET)? "ANYTIME_NET" :
  (inNet == UNTIL_NET)  ? "UNTIL_NET": "UNKNOWN");

  }
#define SpineSet(i) (netTab[i].ntflags.is_spine && !(netTab[i].ntMemberNets & PRIORITY_MAIL))
/*
* needToCall()
*
* This is responsible for checking to see if we need to call this system.
* Basically, here's what the rules are:
*
* Is this account in use?
* Is this account on one of the eligible net ('x' parameter)?
* Is this account not a spine and not an OtherNet system?
*
* If this system is a spine and we're not doing an anytime-net session
* and we haven't had a successful connection yet then call.
*
* If this system has normal mail, room file requests, send file requests,
* rooms that need to share (outgoing messages), domain mail outgoing, mail
* routing and hasn't been connected with yet, then call.
*
* I'm not entirely sure what the reference to Priority Mail signifies in
* this mess.
*/
int needToCall(int i, MULTI_NET_DATA x)
  {
  /* first check for permission to call   */
  if (netTab[i].ntflags.in_use &&     /* account in use   */
  (netTab[i].ntMemberNets & x) &&  /* system is member of net      */
  !SpineSet(i) &&
  !netTab[i].ntflags.OtherNet)
    {
    /* system not OtherNet    */
    /* check for requirement to call  */
    if (netTab[i].ntflags.spine &&
    !(netTab[i].ntMemberNets & PRIORITY_MAIL) &&
    (inNet == NON_NET || ((inNet == NORMAL_NET || inNet == UNTIL_NET) &&
    pollCall[i] == 1)))
    return TRUE;
    /* now check for need to call     */
    if (netTab[i].ntflags.normal_mail ||  /* normal outgoing mail?*/
    netTab[i].ntflags.room_files || /* request files ?  */
    netTab[i].ntflags.send_files || /* send files ?   */
    roomsShared(i) ||     /* rooms to share?  */
    DomainFlags[i] ||     /* domain mail to send? */
    (netTab[i].ntflags.HasRouted &&
    (inNet == NON_NET ||
    ((inNet == NORMAL_NET || inNet == ANYTIME_NET ||
    inNet == UNTIL_NET) &&
    pollCall[i] == 1))))
    return TRUE;

    }
  return FALSE;

  }
/*
* AnyCallsNeeded()
*
* Do we need to make any calls 'tall?
*/
char AnyCallsNeeded(MULTI_NET_DATA whichNets)
  {
  int searcher;
  for (searcher = 0; searcher < cfg.netSize; searcher++)
  if (needToCall(searcher, whichNets)) return TRUE;
  return FALSE;

  }
/*
* ReqNodeName()
*
* This function is a general request for node name from user.  It supports
* various options for prompting or not prompting for the name, allowing the
* input of '&L', allow display of nodelists, etc. (see the code).  The
* function will validate the choice, query again if appropriate, handle
* domains, and do a getNet of the node if necessary.
*/
char ReqNodeName(char *prompt, label target, label domain, char WideSpecValid,
char Once, char Ask, char Display, char SysopMenu,
NetBuffer *nBuf)
  {
  extern char *ALL_LOCALS;
  char sysname[2 * NAMESIZE], dup, work[2 * NAMESIZE];
  int  slot;
  do
    {
    slot = ERROR;
    if (domain != NULL) domain[0] = 0;
    /* Allows function to act as validator only */
    if (Ask)
      {
      getString(prompt, sysname, 2 * NAMESIZE, QUEST_SPECIAL);

      }
    else strCpy(sysname, target);
    NormStr(sysname);
    /* Empty line implies operation abort. */
    if (strLen(sysname) == 0)
      {
      strCpy(target, sysname);
      return FALSE;

      }
    /* If "&L" entered and is acceptable ... */
    if (WideSpecValid && strCmpU(sysname, ALL_LOCALS) == SAMESTRING)
      {
      strCpy(target, sysname);
      return TRUE;

      }
    /* Questioning frown */
    if (sysname[0] == '?')
      {
      writeNet(Display, FALSE);   /* write out available nets */
      if (WideSpecValid)
      mPrintf("'&L' == Local Systems Announcement\n ");

      }
    /* finally, must be real system name so seeeeearch for it! */
    else if ((slot = searchNameNet(sysname, nBuf)) != ERROR)
      {
      strCpy(target, nBuf->netName);  /* aesthetics */
      if (nBuf->nbflags.local || nBuf->nbflags.RouteLock)
        {
        return TRUE;    /* Yup */

        }

      }
    if (domain != NULL && SystemInSecondary(sysname, domain, &dup))
      {
      if (dup)
        {
        /* oops */
        if (slot != ERROR) return TRUE;
        /* do it as a double if, not claused */
        if (Ask)
          Output_Citadel_Message("DUPDOM", (long)sysname, NULL, NULL);

        }
      else
        {
        strCpy(target, sysname);  /* aesthetics */
        return TRUE;

        }

      }
    if (slot != ERROR) return TRUE;
    if (sysname[0] != '?')
      {
      sPrintf(work, "%s not listed.\n", sysname); /* Nope */
      if (SysopMenu) SysopError(NO_MENU, work);
      else     mPrintf(work);

      }

    }
  while (!Once && Ask); /* This controls if we ask repeatedly or only once */
  return FALSE;       /* And if we get here, we definitely are a failure */

  }
/*
* NetValidate()
*
* This will return TRUE if net privs are go, FALSE otherwise.
*/
char NetValidate(char talk)
  {
  if (!cfg.BoolFlags.netParticipant)
    {
    if (talk)
      Output_Citadel_Message("NTPTNT",NULL, NULL, NULL);
    return FALSE;

    }
  if( loggedIn && logBuf.lbflags.NET_PRIVS ) return TRUE;
  if( loggedIn && roomBuf.rbflags.AUTO_NET ) return TRUE;
  if( loggedIn && roomBuf.rbflags.ALL_NET  ) return TRUE;
  if (talk)
    Output_Citadel_Message("NTPTNT",NULL, NULL, NULL);
  return FALSE;
  }
/*
* FindRouteIndex()
*
* This will find the next route filename in sequence.
*/
int FindRouteIndex(int slot)
  {
  label temp;
  SYS_FILE newfn;
  sPrintf(temp, "R%d", slot);
  makeSysName(newfn, temp, &cfg.netArea);
  return FindNextFile(newfn);

  }
typedef struct
  {
  int count;
  char *str;

  }
SR_Arg;
/*
* ParticipatingNodes()
*
* This function prepares a string indicating who shares the current room with
* us.
*/
void ParticipatingNodes(char *target)
  {
  int    node;
  SR_Arg arg;
  char   *c;
  int ShowSharedRoomName(SharedRoom *room, int system, int index,
  int roomslot, void *arg);
  sPrintf(lbyte(target), ". This room is shared with: ");
  arg.count = 0;
  arg.str   = target;
  for (node = 0; node < cfg.netSize; node++)
  EachSharedRoom(node, ShowSharedRoomName, NULL, &arg);
  if (arg.count)
    {
    /* this eliminates the trailing comma */
    c = lbyte(target);
    c -= 2;
    *c = 0;

    }

  }
/*
* ShowSharedRoomName()
*
* This appends the name of this shared room to a string.
*/
int ShowSharedRoomName(SharedRoom *room, int system, int index, int roomslot,
void *d)
  {
  SR_Arg *arg;
  char *name;
  char commnd, *s1, *s2, *s3, doit;
  arg = d;
  if (roomslot == thisRoom)
    {
    getNet(system, &netBuf);
    arg->count++;
    Addressing(system, index, &commnd, &s1, &s2, &s3, &name, &doit);
    sPrintf(lbyte(arg->str), "%s (%s), ", netBuf.netName, name);
    return ERROR;

    }
  return TRUE;

  }
/*
* AreaCode()
*
* This function extracts the area code from the node id.
*/
void AreaCode(char *Id, char *Target)
  {
  int i, j;
  for (i = j = 0; j < 3 && Id[i]; i++)
  if (isdigit(Id[i]))
  Target[j++] = Id[i];
  Target[j] = 0;

  }
/*
* NetInit()
*
* This function does network initialization: Cache handling, network recovery
* (in case of crash during netting), etc...
*/
void NetInit()
  {
  int rover;
  char buf[2*NAMESIZE];
  SpecialMessage("Network Initialization");
  MakeNetCache(buf);
  if (access(buf, 0) != 0) mkdir(buf);
  VirtInit();
  VortexInit();
  /* we never need do this again */
  normId(cfg.codeBuf + cfg.nodeId, HomeId);
  DomainInit(TRUE);
  for (rover = 0; rover < cfg.netSize; rover++)
  if (netTab[rover].ntMemberNets & PRIORITY_MAIL) PriorityMail++;
  RecoverNetwork();
  SpecialMessage("");

  }
/*
* MakeNetted()
*
* This function will make a message into a net message.  This is userland
* code called when an aide wants to make a non-netted message into a netted
* message.
*/
char MakeNetted(int m)
  {
  if (findMessage(roomBuf.msg[m].rbmsgLoc, roomBuf.msg[m].rbmsgNo, TRUE))
    {
    getMsgStr(getMsgChar, msgBuf.mbtext, MAXTEXT);  /* get balance */
    strCpy(msgBuf.mboname, cfg.codeBuf + cfg.nodeName);
    strCpy(msgBuf.mbdomain, cfg.codeBuf + cfg.nodeDomain);
    strCpy(msgBuf.mbaddr, R_SH_MARK);
    DelMsg(TRUE, m);
    putMessage(NULL);
    return NETTED;

    }
  return NO_CHANGE;

  }
/*
* freeUNS()
*
* This function is purportedly a free function for a list.  In actuality,
* it also runs a net session for each.
*/
void freeUNS(TwoNumbers *netdata)
  {
  int yr, dy, hr, mn, mon, secs, milli;
  if (!onLine())
    {
    getRawDate(&yr, &mon, &dy, &hr, &mn, &secs, &milli);
    netController((hr * 60) + mn, (int) netdata->second,
    (1l << (netdata->first - 1)), UNTIL_NET, 0);

    }
  free(netdata);

  }
/*
* GetCompression()
*
* Gets the compression value.
*/
int GetCompression(int system)
  {
  if (netTab[system].ntflags.Arc) return ARC_COMP;
  if (netTab[system].ntflags.Zoo) return ZOO_COMP;
  if (netTab[system].ntflags.Zip) return ZIP_COMP;
  return LHA_COMP;

  }
/*
* HasOutgoing()
*
* This function checks to see if the given room as specified by the system,
* index pair has outgoing messages in either cache or message form.
*
* NB: This code may not be OK.
*/
char HasOutgoing(int system, int index)
  {
  if (GetFA(netTab[system].netTRooms[index].mode)) return TRUE;
  if (roomTab[netTabRoomSlot(system, index)].rtlastMessage >
  netTab[system].netTRooms[index].lastMess) return TRUE;
  return FALSE;

  }
