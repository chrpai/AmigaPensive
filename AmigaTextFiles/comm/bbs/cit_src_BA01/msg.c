/*
*                               msg.c
*
* Message handling for Citadel bulletin board system.
*/
/*
*                               history
*
* 86Aug15 HAW  Large chunk of History deleted due to space problems.
* 84Mar29 HAW Start upgrade to BDS C 1.50a, identify _spr problem.
* 83Mar03 CrT & SB   Various bug fixes...
* 83Feb27 CrT  Save private mail for sender as well as recipient.
* 83Feb23       Various.  transmitFile() won't drop first char on WC...
* 82Dec06 CrT  2.00 release.
* 82Nov05 CrT  Stream retrieval.  Handles messages longer than MAXTEXT.
* 82Nov04 CrT  Revised disk format implemented.
* 82Nov03 CrT  Individual history begun.  General cleanup.
*/
#include "ctdl.h"
/* #define TEST_SYS */
/*
*                               contents
*
*       AddNetMail()            manage adding mail to a net system
*       aideMessage()           saves auto message in Aide>
*       canRespond()            respond on the net checker
*       CheckForwarding()       forward mail to another system?
*       dGetWord()              reads a word off disk
*       doActualWrite()         to allow two message files for bkp
*       doFlush()               writes out to specified msg file
*       DoRespond()             respond to mail.
*       fakeFullCase()          converts uppercase message to mixed case
*       flushMsgBuf()           wraps up message-to-disk store
*       getRecipient()          get recipient for the message
*       getWord()               gets one word from message buffer
*       hldMessage()            handles held messages
*       mAbort()                checks for user abort of typeout
*       makeMessage()           menu-level message-entry routine
*       mFormat()               formats a string to modem and console
*       mPeek()                 sysop debugging tool--shows ctdlmsg.sys
*       msgToDisk()             puts a message to the given disk file
*       noteMessage()           enter message into current room
*       noteAMessage()          noteMessage() local
*       printMessage()          prints a message on modem & console
*       pullIt()                sysop special message-removal routine
*       putLong()               puts a long integer to file
*       putMessage()            write message to disk
*       putMsgChar()            writes successive message chars to disk
*       putWord()               writes one word to modem & console
*       replyMessage()          reply to a Mail> message
*       showMessages()          menu-level show-roomful-of-messages fn
*       ShowReply()             reply tracing function
*/
extern char      netDebug;
extern char      logNetResults;
extern FILE         *msgfl;    /* file descriptor for the msg file      */
extern FILE         *msgfl2;   /* disk based backup msg file            */
AN_UNSIGNED crtColumn; /* current position on screen            */
char pullMessage = FALSE;/* true to pull current message*/
char        journalMessage = FALSE;
SECTOR_ID   pulledMLoc;/* loc of pulled message         */
MSG_NUMBER  pulledMId = 0l;    /* id of message to be pulled   */
label       oldTarget;
char        jrnlFile[100] = "";
char        *NoNetRoomPrivs = "You do not have net privileges";
char        outFlag = OUTOK;   /* will be one of the above     */
char        heldMess;
char        PrintBanner = FALSE;
SListBase   SysList =
  {
  NULL, ChkCC, NULL, free, strdup

  };
SListBase   FwdVortex =
  {
  NULL, ChkCC, NULL, free, NULL

  };
char        EndWithCR = TRUE;
int         AnonMsgCount;
int         IckyCount, IckyLevel=1000;
char        DisVandals;
static char ReturnAddress[(2 * NAMESIZE) + 10];
static char ArchiveMail;
static char EOP = FALSE;
char ReverseMessage;
char MsgStreamEnter = FALSE;
char Showing  = WHATEVER;
char DiskHeld;
int  ParanoiaLimit = 1000;
int  ParanoiaCount;
extern MessageBuffer   msgBuf;  /* The message buffer   */
extern MessageBuffer   tempMess;        /* For held messages    */
extern struct mBuf mFile1, mFile2;
extern CONFIG    cfg;           /* Configuration variables      */
extern logBuffer logBuf;        /* Buffer for the pippuls       */
extern logBuffer logTmp;        /* Buffer for the pippuls       */
extern aRoom     roomBuf;       /* Room buffer                  */
extern rTable    *roomTab;
extern NetBuffer netBuf, netTemp;
extern FILE      *upfd;
extern int       thisRoom;      /* Current room         */
extern int       thisNet;       /* Current node in use    */
extern int       thisLog;       /* Current log position  */
extern int       outPut;
extern NetTable  *netTab;
extern char      *strFile;
extern char      exChar;
extern char      echo;  /* Output flag  */
extern char      echoChar;
extern char      loggedIn;       /* Logged in flag      */
extern char      whichIO;        /* Who gets output?    */
extern char      prevChar;       /* Output's evil purposes       */
extern char      inNet;
extern int       TransProtocol;  /* Flag        */
extern char      haveCarrier;    /* Flag        */
extern char      onConsole;      /* Flag        */
extern char      remoteSysop;
extern FILE      *netLog;
/*
* aideMessage()
*
* This function saves an auto message in Aide>.
*/
void aideMessage(char *name, char noteDeletedMessage)
  {
  int ourRoom, target;
  ourRoom = thisRoom;
  if (cfg.BoolFlags.debug) splitF(netLog, "Current Rooms(a) are:%d/%d\n",ourRoom,thisRoom);
  /* message is already set up in msgBuf.mbtext */
  putRoom(thisRoom);
  if (cfg.BoolFlags.debug) splitF(netLog, "Current Rooms(b) are:%d/%d\n",ourRoom,thisRoom);
  if (name == NULL || (target = roomExists(name)) == ERROR)target = AIDEROOM;
  getRoom(target);
  strCpy(msgBuf.mbauth, "Citadel");
  msgBuf.mbto[0]    = '\0';
  msgBuf.mboname[0] = '\0';
  putMessage(&logBuf);
  if (cfg.BoolFlags.debug) splitF(netLog, "Current Rooms(c) are:%d/%d\n",ourRoom,thisRoom);
  if (noteDeletedMessage)
    {
    if (cfg.BoolFlags.debug) splitF(netLog, "Current Rooms(d) are:%d/%d\n",ourRoom,thisRoom);
    noteAMessage(roomBuf.msg, MSGSPERRM, pulledMId, pulledMLoc);
    if (cfg.BoolFlags.debug) splitF(netLog, "Current Rooms(e) are:%d/%d\n",ourRoom,thisRoom);

    }
  putRoom(target);
  if (cfg.BoolFlags.debug) splitF(netLog, "Current Rooms(f) are:%d/%d\n",ourRoom,thisRoom);
  noteRoom();
  if (cfg.BoolFlags.debug) splitF(netLog, "Current Rooms(g) are:%d/%d\n",ourRoom,thisRoom);
  getRoom(ourRoom);
  if (cfg.BoolFlags.debug) splitF(netLog, "Current Rooms(h) are:%d/%d\n",ourRoom,thisRoom);

  }
/*
* canRespond()
*
* Can we set up an auto-response on the net?  This includes domain mail.
*/
char canRespond()
  {
  int   cost, result;
  char  dup;
  label temp;
  label domain;
  if (inNet != NON_NET)
  return FALSE;
  ReturnAddress[0] = 0;
  if (msgBuf.mborig[0] == 0 &&  /* i.e. is local mail      */
  msgBuf.mboname[0] == 0)
  return TRUE;
  if (!logBuf.lbflags.NET_PRIVS)
  return FALSE;
  normId(msgBuf.mborig, temp);
  domain[0] = 0;
  if ((result = searchNet(temp, &netBuf)) == ERROR ||
  (!netBuf.nbflags.local && !netBuf.nbflags.RouteLock))
    {
    if (msgBuf.mbdomain[0] != 0)
      {
      strCpy(domain, msgBuf.mbdomain);

      }
    else if (!SystemInSecondary(msgBuf.mboname, domain, &dup) || dup)
      {
      if (result == ERROR) return FALSE;

      }

    }
  if (strLen(domain) != 0)
  sPrintf(ReturnAddress, "%s _ %s", msgBuf.mboname, domain);
  else
  strCpy(ReturnAddress, netBuf.netName);
  if (domain[0] == 0 && netBuf.nbflags.local)
  return TRUE;
  if (domain[0] == 0)
    {
    if (msgBuf.mbdomain[0] != 0)
    cost = FindCost(msgBuf.mbdomain);
    else
    cost = !netBuf.nbflags.local;

    }
  else cost = FindCost(domain);
  if (logBuf.credit >= cost)
  return TRUE;
  /* Sysop always has enough credits kludge */
  if (HalfSysop())
    {
    logBuf.credit += cost;
    return TRUE;

    }
  mPrintf("\n Not enough LD credit.\n ");
  return FALSE;

  }
/*
* deleteMessage()
*
* This function deletes a message for pullIt().
*/
char deleteMessage(int m)
  {
  char auth[129];
  /* record vital statistics for possible insertion elsewhere: */
  DelMsg(TRUE, m);
  strCpy(auth, msgBuf.mbauth);
  ZeroMsgBuffer(&msgBuf);
  /* note in Aide>: */
  sPrintf(msgBuf.mbtext, "Following message from %s deleted by %s:",
  (auth[0]) ? auth : "<anonymous>", logBuf.lbname);
  aideMessage(NULL, /* noteDeletedMessage== */ TRUE);
  return TRUE;

  }
/*
* DelMsg()
*
* This does actual work of deleting a msg from current room.
*/
void DelMsg(char killit, int m)
  {
  int SaveRoom;
  int i;
  pulledMLoc = roomBuf.msg[m].rbmsgLoc;
  #ifdef NORMAL_MESSAGES
  pulledMId  = roomBuf.msg[m].rbmsgNo ;
  #else
  pulledMId  = (roomBuf.msg[m].rbmsgNo & S_MSG_MASK);
  #endif
  if (thisRoom == AIDEROOM || !killit)   return ;
  /* return emptied slot: */
  for (i = m;  i > 0;  i--)
    {
    roomBuf.msg[i].rbmsgLoc      = roomBuf.msg[i - 1].rbmsgLoc;
    roomBuf.msg[i].rbmsgNo       = roomBuf.msg[i - 1].rbmsgNo ;

    }
  roomBuf.msg[0].rbmsgNo   = 0l;         /* mark new slot at end as free */
  roomBuf.msg[0].rbmsgLoc  = 0;  /* mark new slot at end as free */
  /* store revised room to disk before we forget...   */
  SaveRoom = thisRoom;
  if (cfg.BoolFlags.debug) splitF(netLog, "Current Rooms(a) are:%d/%d\n",SaveRoom,thisRoom);
  noteRoom();
  if (cfg.BoolFlags.debug) splitF(netLog, "Current Rooms(b) are:%d/%d\n",SaveRoom,thisRoom);
  putRoom(thisRoom);
  if (cfg.BoolFlags.debug) splitF(netLog, "Current Rooms(c) are:%d/%d\n",SaveRoom,thisRoom);
  getRoom(SaveRoom);
  if (cfg.BoolFlags.debug) splitF(netLog, "Current Rooms(d) are:%d/%d\n",SaveRoom,thisRoom);

  }
/*
* dGetWord()
*
* This function fetches one word from current message, off disk.  It returns
* TRUE if more words follow, else FALSE.
*/
char dGetWord(char *dest, int lim)
  {
  int  c;
  --lim;         /* play it safe */
  /* pick up any leading blanks: */
  for (c = getMsgChar(); (c == '\n' || c == ' ')  &&  c && lim;
  c = getMsgChar())
    {
    if (lim)
      {
      *dest++ = c;   lim--;

      }

    }
  /* step through word: */
  for (; c != '\n' && c != ' ' && c && lim;   c = getMsgChar())
    {
    if (lim)
      {
      *dest++ = c;   lim--;

      }

    }
  if (*(dest - 1) != '\n')
  /* trailing blanks: */
  for (;   c == ' ' && c && lim;   c = getMsgChar())
    {
    if (lim)
      {
      *dest++ = c;   lim--;

      }

    }
  if (c)  unGetMsgChar(c);    /* took one too many    */
  *dest = '\0';         /* tie off string        */
  return (char) c;

  }
/*
* doActualWrite()
*
* This is used to help mirror a message on both disk and in RAM disk (two
* message files).
*/
char doActualWrite(FILE *whichmsg, struct mBuf *mFile, char c)
  {
  MSG_NUMBER  temp;
  int           toReturn = 0;
  if (mFile->sectBuf[mFile->thisChar] == 0xFF)
    {
    /* obliterating a msg   */
    toReturn = 1;

    }
  mFile->sectBuf[mFile->thisChar]   = c;
  mFile->thisChar    = ++mFile->thisChar % MSG_SECT_SIZE;
  if (mFile->thisChar == 0)
    {
    /* time to write sector out and get next: */
    temp = mFile->thisSector;
    temp *= MSG_SECT_SIZE;
    fseek(whichmsg, temp, 0);
    crypte(mFile->sectBuf, MSG_SECT_SIZE, 0);
    if (fwrite(mFile->sectBuf, MSG_SECT_SIZE, 1, whichmsg) != 1)
      {
      crashout("?putMsgChar-write fail");

      }
    mFile->thisSector = ++mFile->thisSector % cfg.maxMSector;
    temp = mFile->thisSector;
    temp *= MSG_SECT_SIZE;
    fseek(whichmsg, temp, 0);
    if (fread(mFile->sectBuf, MSG_SECT_SIZE, 1, whichmsg) != 1)
      {
      crashout("?putMsgChar-read fail");

      }
    crypte(mFile->sectBuf, MSG_SECT_SIZE, 0);

    }
  return  (char)toReturn;

  }
/*
* doFlush()
*
* This does actual write for specified msg file.
*/
void doFlush(FILE *whichmsg, struct mBuf *mFile)
  {
  long int s;
  s = mFile->thisSector;
  s *= MSG_SECT_SIZE;
  fseek(whichmsg, s, 0);
  crypte(mFile->sectBuf, MSG_SECT_SIZE, 0);
  if (fwrite(mFile->sectBuf, MSG_SECT_SIZE, 1, whichmsg) != 1)
    {
    crashout("?ctdlmsg.sys write fail");

    }
  crypte(mFile->sectBuf, MSG_SECT_SIZE, 0);
  fflush(whichmsg);

  }
/*
* DoRespond()
*
* Does the user want to respond to mail or even skip the mail?
*/
int DoRespond()
  {
  int toReturn = -2;
  for (doCR(); onLine() && toReturn == -2; )
    {
    outFlag = IMPERVIOUS;
    mPrintf("respond? (Y/N/Skip): ");
    switch (toUpper(iChar()))
      {
      case 'Y': toReturn = TRUE; break;
      case 'N': toReturn = FALSE; break;
      case 'S': mPrintf("kip message"); toReturn = ERROR; break;

      }
    doCR();

    }
  outFlag = OUTOK;
  return toReturn;

  }
/*
* fakeFullCase()
*
* This function converts a message in uppercase-only to a reasonable mix.  It
* can't possibly make matters worse...
*
* Algorithm: First alphabetic after a period is uppercase, all others are
* lowercase, excepting pronoun "I" is a special case.  We assume an imaginary
* period preceding the text.
*/
void fakeFullCase(char *text)
  {
  char *c;
  char lastWasPeriod;
  char state;
  for(lastWasPeriod=TRUE, c=text;   *c;  c++)
    {
    if (
    *c != '.'
    &&
    *c != '?'
    &&
    *c != '!'
    )
      {
      if (isAlpha(*c))
        {
        if (lastWasPeriod)      *c       = toUpper(*c);
        else if (*c != 'l')     *c       = toLower(*c);
        lastWasPeriod   = FALSE;

        }

      }
    else
      {
      lastWasPeriod      = TRUE ;

      }

    }
  /* little state machine to search for ' i ': */
  #define NUTHIN          0
  #define FIRSTBLANK     1
  #define BLANKI          2
  for (state=NUTHIN, c=text;  *c;  c++)
    {
    switch (state)
      {
      case NUTHIN:
      if (isSpace(*c))    state   = FIRSTBLANK;
      else              state   = NUTHIN    ;
      break;
      case FIRSTBLANK:
      if (*c == 'i')     state   = BLANKI    ;
      else              state   = NUTHIN    ;
      break;
      case BLANKI:
      if (isSpace(*c))    state   = FIRSTBLANK;
      else              state   = NUTHIN    ;
      if (!isAlpha(*c))   *(c-1)  = 'I';
      break;

      }

    }

  }
/*
* flushMsgBuf()
*
* This function wraps up writing a message to disk, takes into account 2nd
* msg file if necessary.
*/
void flushMsgBuf()
  {
  doFlush(msgfl, &mFile1);
  if (cfg.BoolFlags.mirror)
  doFlush(msgfl2, &mFile2);

  }
/*
* getWord()
*
* This function fetches one word from current message.
*/
int getWord(char *dest, char *source, int offset, int lim)
  {
  int i, j;
  /* skip leading blanks if any */
  for (i = 0; (source[offset+i] == '\n' || source[offset+i] == ' ') &&
  i < lim - 1;  i++);
  /* step over word */
  for (;
  source[offset+i]   != ' '     &&
  source[offset+i]   != '\n'     &&
  i               <  lim - 1 &&
  source[offset+i]   != 0;
  i++
  );
  if (source[offset + i - 1] != '\n')
  /* pick up any trailing blanks */
  for (;  source[offset+i]==' ' && i<lim - 1;  i++);
  /* copy word over */
  for (j = 0; j < i; j++)  dest[j] = source[offset+j];
  dest[j] = 0;  /* null to tie off string */
  return(offset+i);

  }
/*
* mAbort()
*
* This function returns TRUE if the user has aborted typeout.
*
* Globals modified:      outFlag
*/
char mAbort()
  {
  extern SListBase Moderators;
  char c, toReturn = FALSE, oldEcho;
  /* Check for abort/pause from user */
  if (outFlag == IMPERVIOUS || outFlag == NET_CALL || outPut == DISK)
    {
    toReturn    = FALSE;

    }
  else if (!BBSCharReady())
    {
    if (haveCarrier && !gotCarrier())
      {
      modIn();      /* Let modIn() report the problem    */
      toReturn  = TRUE;

      }
    else if (!onConsole && KBReady() && !PrintBanner)
      {
      if (!SurreptitiousChar(getCh()))
        {
        outFlag     = OUTSKIP;
        toReturn    = TRUE;

        }

      }
    else toReturn       = FALSE;

    }
  else
    {
    oldEcho  = echo;
    echo     = NEITHER;
    echoChar = 0;
    c = toUpper(modIn());   /* avoid the filter */
    switch (c)
      {
      case XOFF:
      while (iChar() != XON && (gotCarrier() || onConsole))
      ;
      toReturn = FALSE;
      break;
      case ' ':                             /* Allow space to pause*/
      case 'P':                             /*  pause:   */
      c = iChar();                          /* wait to resume */
      toReturn   = FALSE;
      if (     toUpper(c) == 'D' &&
      Showing == MSGS &&
      (aide ||
      (strCmpU(logBuf.lbname, AskForNSMap(&Moderators,
      thisRoom)) == SAMESTRING
      && strLen(logBuf.lbname) != 0)))
      pullMessage = TRUE;
      else if (toUpper(c) == 'J' && HalfSysop())
      journalMessage = TRUE;
      /*
      * We do things this way to avoid getting caught in a
      * recursive trap which I don't really care to trace down
      * at the moment involving mPrintf accidentally "calling"
      * message entry routines when it really shouldn't.  Not
      * only can that get messy, we could also run into some stack
      * overflow problems.  So instead we'll use this "kludge"
      * which may actually yield a better behavior for us -- it
      * lets us reprint the interrupted message after message
      * composition.  Note we allow this option in Mail but it acts
      * entirely differently -- see showMessage().  Also note we track
      * READ-ONLY rooms here, too, rather than in showMessage().
      */
      else if (toUpper(c) == 'E' && Showing == MSGS && HasWritePrivs())
        {
        MsgStreamEnter = TRUE;
        outFlag     = OUTSKIP;
        toReturn = TRUE;

        }
      break;
      case 'J':                            /* jump paragraph:*/
      outFlag     = OUTPARAGRAPH;
      toReturn    = FALSE;
      break;
      case 'N':                            /* next:       */
      outFlag     = OUTNEXT;
      toReturn    = TRUE;
      break;
      case 'S':                            /* skip:       */
      outFlag     = OUTSKIP;
      toReturn    = TRUE;
      break;
      case 'R':
      if (Showing == MSGS)
        {
        pause(50);
        ReverseMessage = TRUE;

        }
      toReturn    = FALSE;      /* so we don't stop output */
      break;
      case 7: /* anytime net indicator */
      case 68: /* stroll indicator */
      if (PrintBanner &&
      ((c == 7 && cfg.BoolFlags.netParticipant) ||
      c == 68))
        {
        if (c == 7)
          {
          if (CheckForSpecial(13, 69))
            {
            outFlag  = NET_CALL;
            toReturn = TRUE;

            }

          }
        else
          {
          if (CheckForSpecial(79, 35))
            {
            outFlag  = STROLL_DETECTED;
            toReturn = TRUE;

            }

          }
        break;

        }
      default:
      toReturn    = FALSE;
      break;

      }
    echo    = oldEcho;

    }
  return toReturn;

  }
/*
* CheckForSpecial()
*
* This checks to see if a special call is incoming.  This can be a network
* call or a stroll call (not implemented, really).
*/
char CheckForSpecial(int second, int third)
  {
  if (receive(1) == second)
  if (receive(1) == third)
  return TRUE;
  return FALSE;

  }
/*
* getRecipient()
*
* This gets the recipient for the message (Mail> only, of course).
*/
char getRecipient()
  {
  label person;
  if (thisRoom != MAILROOM)
    {
    msgBuf.mbto[0] = 0;      /* Zero recipient   */
    return TRUE;

    }
  if (msgBuf.mbto[0] == 0)
    {
    if (!loggedIn || (!aide && cfg.BoolFlags.noMail) ||
    logBuf.lbflags.TWIT)
      {
      strCpy(msgBuf.mbto, "Sysop");
      mPrintf(" (private mail to 'sysop')\n ");
      return TRUE;

      }
    getNormStr("RECIPM", msgBuf.mbto, sizeof msgBuf.mbto, 0);
    if (strLen(msgBuf.mbto) == 0) return FALSE;

    }
  switch (SepNameSystem(msgBuf.mbto, person, msgBuf.mbaddr, &netBuf))
    {
    case IS_SYSTEM:
    if (!NetValidate(TRUE) || !netInfo(FALSE)) return FALSE;
    strCpy(msgBuf.mbto, person);
    /* strCpy(msgBuf.mbaddr, netBuf.netName); */
    case NOT_SYSTEM:
    break;
    default:
    mPrintf("Couldn't find target system.\n ");
    case SYSTEM_IS_US:
    return FALSE;

    }
  if (!msgBuf.mbaddr[0])
    {
    if (strCmpU(msgBuf.mbto, logBuf.lbname) == SAMESTRING)
      {
      Output_Citadel_Message("NOSELF",NULL, NULL, NULL);
      return FALSE;

      }
    if (PersonExists(msgBuf.mbto) == ERROR)
      {
      Output_Citadel_Message("NOPERS",(long)msgBuf.mbto,NULL, NULL);
      msgBuf.mbto[0] = 0;            /* Zero recipient   */
      return FALSE;

      }

    }
  return TRUE;

  }
/*
* replyMessage()
*
* This function will get a reply to a Mail> message.
*/
char replyMessage(MSG_NUMBER msgNo, SECTOR_ID Loc)
  {
  label who;
  char  other[O_NET_PATH_SIZE];
  if (heldMess)
    {
    if (strCmpU(msgBuf.mbauth, tempMess.mbto) == SAMESTRING)
      {
      return hldMessage(TRUE);

      }

    }
  strncpy(who, msgBuf.mbauth,sizeof(who)-1);
  strCpy(other, msgBuf.mbOther);
  ZeroMsgBuffer(&msgBuf);
  strCpy(msgBuf.mbto, who);
  strCpy(msgBuf.mbaddr, ReturnAddress);
  sPrintf(msgBuf.mbreply, "%u:%lu", Loc, msgNo);        /* back ptr */
  if (strLen(other) != 0)
    {
    mPrintf("%s address is '%s'.", netBuf.netName, other);
    if (!getYesNo("OKPROM"))
      {
      mPrintf("%s address", netBuf.netName);
      getNormStr("", msgBuf.mbOther, O_NET_PATH_SIZE, 0);

      }
    else
    strCpy(msgBuf.mbOther, other);

    }
  return (char)(procMessage(ASCII, TRUE) == TRUE);

  }
/*
* hldMessage()
*
* This function handles held messages
* TRUE indicates a message was added to the room
* FALSE indicates room not disturbed (either msg was re-held or aborted)
*/
char hldMessage(char IsReply)
  {
  int result;
  if (!heldMess)
    {
    mPrintf(" \n No message in the Hold buffer!\007\n ");
    return FALSE;

    }
  heldMess = FALSE;
  MoveMsgBuffer(&msgBuf, &tempMess);
  ZeroMsgBuffer(&tempMess);
  if (DiskHeld)
    {
    DiskHeld = FALSE;
    if ((result = roomExists(msgBuf.mbroom)) != ERROR)
      {
      if (KnownRoom(result))
        {
        mPrintf("\n Moving to %s.\n ", msgBuf.mbroom);
        getRoom(result);
        setUp(FALSE);
        if (thisRoom == MAILROOM)
        echo = CALLER;

        }

      }

    }
  if (roomTab[thisRoom].rtflags.SHARED == 0)
  msgBuf.mboname[0] = 0;
  else if (loggedIn && roomBuf.rbflags.SHARED &&
  roomBuf.rbflags.AUTO_NET &&
  (roomBuf.rbflags.ALL_NET || logBuf.lbflags.NET_PRIVS))
  netInfo(TRUE);
  /*
  * this indicates the user did a .eh in Mail> and
  * failed to type in a valid user name.  This saves the
  * message back to the held buffer, rather than losing
  * it (a nettlesome behavior).
  */
  if ((result = procMessage(ASCII, IsReply)) == ERROR)
    {
    MoveMsgBuffer(&tempMess, &msgBuf);
    heldMess = TRUE;
    return FALSE;               /* indicate what happened */

    }
  return (char)result;

  }
/*
* makeMessage()
*
* This is a menu-level routine to enter a message.
*
* Return: TRUE if message saved else FALSE.
*/
int makeMessage(char uploading)
  {
  if (!loggedIn && AnonMsgCount > 1)
  return FALSE;
  if (loggedIn && roomBuf.rbflags.SHARED &&
  roomBuf.rbflags.AUTO_NET &&
  (roomBuf.rbflags.ALL_NET || logBuf.lbflags.NET_PRIVS))
  return netMessage(uploading);
  ZeroMsgBuffer(&msgBuf);
  return procMessage(uploading, FALSE);

  }
/*
* idiotMessage()
*
* This checks for idiocy by the user.
*/
char idiotMessage()
  {
  int base, rover;
  if (DisVandals || loggedIn || thisRoom != MAILROOM)
  return FALSE;
  for (base = 0; msgBuf.mbtext[base]; base++)
    {
    for (rover = 0; rover < IDIOT_TRIGGER; rover++)
      {
      if (msgBuf.mbtext[base] != msgBuf.mbtext[base + rover] ||
      msgBuf.mbtext[base] == ' ')
      break;

      }
    if (rover == IDIOT_TRIGGER) return TRUE;    /* Jackass caught! */

    }
  if (cfg.AnonMailLength > 0 && strLen(msgBuf.mbtext) > cfg.AnonMailLength &&
  !onConsole)
    {
    HangUp(TRUE);
    sPrintf(tempMess.mbtext, "   %s @ %s\n%s\n", DisplayDate(formDate()), Current_Time(),
    msgBuf.mbtext);
    CallMsg("anonmail", tempMess.mbtext);
    strCpy(msgBuf.mbtext, "Indecently long anonymous Mail has been stored in ANONMAIL.");
    aideMessage(NULL,FALSE);
    return TRUE;

    }
  return FALSE;

  }
/*
* procMessage
*
* This is a menu-level routine to enter a message.
*/
int procMessage(char uploading, char IsReply)
  {
  char   *pc, allUpper;
  extern SListBase BadWords;
  extern char BadMessages[];
  extern char MsgEntryType;
  if (!HasWritePrivs())
    {
    mPrintf("This is a Read Only room.");
    return FALSE;

    }
  if (loggedIn)
  strCpy(msgBuf.mbauth, (roomBuf.rbflags.ANON) ? "" : logBuf.lbname);
  strCpy(msgBuf.mbroom, roomBuf.rbname);
  strCpy(msgBuf.mbdate, (roomBuf.rbflags.ANON) ? "****" :  formDate());
  if (!getRecipient())
    {
    return ERROR;

    }
  MsgEntryType = MSG_ENTRY;
  if (getText(uploading) == TRUE)
    {
    if (!getRecipient())
      {
      mPrintf("Unexpected internal error, please report it!\n ");
      return FALSE;

      }
    /* Asshole check */
    if (idiotMessage())
      {
      strCpy(msgBuf.mbtext, "Vandalism attempt.");
      aideMessage(NULL,FALSE);
      return FALSE;

      }
    if ( (!loggedIn || thisRoom != MAILROOM)
      &&  SearchList(&BadWords, msgBuf.mbtext) != NULL )
      {
      if (++IckyCount > IckyLevel && !aide)
        {
        logMessage(BADWORDS_SIGNAL, "", 'E');
        if (!onConsole) HangUp(TRUE);

        }
      if (!aide)
        {
        if (strLen(BadMessages) != 0)
          {
          if (redirect(BadMessages))
            {
            printMessage(1);
            undirect();

            }

          }
        return FALSE;

        }

      }
    for (pc=msgBuf.mbtext, allUpper=TRUE;   *pc && allUpper;  pc++)
      {
      if (toUpper(*pc) != *pc && *pc != 'l')   allUpper = FALSE;

      }
    if (allUpper)   fakeFullCase(msgBuf.mbtext);
    if (!logBuf.lbflags.TWIT)
    putMessage(&logBuf);
    AnonMsgCount++;
    if (!IsReply && ++ParanoiaCount > ParanoiaLimit && !aide)
      {
      logMessage(BADWORDS_SIGNAL, "", 'E');
      if (!onConsole) HangUp(TRUE);

      }
    return TRUE;

    }
  return FALSE;

  }
/*
* HasWritePrivs()
*
* This checks to see if the write privileges are yes.
*/
char HasWritePrivs()
  {
  extern SListBase Moderators;
  if (roomBuf.rbflags.READ_ONLY && !aide &&
  knowRoom(&logBuf, thisRoom) != WRITE_PRIVS &&
  !(strCmpU(logBuf.lbname, AskForNSMap(&Moderators,
  thisRoom)) == SAMESTRING &&
  strLen(logBuf.lbname) != 0))
  return FALSE;
  else
  return TRUE;

  }
/*
* EatIcky()
*
* This function will eat an icky word for the MakeList handler.
*/
void *EatIcky(char *str)
  {
  if (strLen(str) != 0) return strdup(str);
  return NULL;

  }
/*
* FindIcky()
*
* This will check to see if a word exists in the current message.  This is
* used by the list handling functions.
*/
void *FindIcky(char *word, char *str)
  {
  char *c;
  if ((c = matchString(str, word, lbyte(str) - 1)) != NULL)
  splitF(netLog, "FindIcky found word -%s- in message.\n", word);
  return c;

  }
/**
* EatIckyPeople()
*
* This function will eat an icky person for the MakeList handler.
* See definition of User_Id for more info.
**/
void *EatIckyPeople(char *str)
  {
  char *item;
  struct User_Id *ptr;
  if (strLen(str) != 0)
    {
    ptr =  GetDynamic( sizeof( struct User_Id) );
    item = strtok(str,",");           /* name of person to suppress */
    if( item != NULL )
      {
      ptr->name = strdup(item);
      item      = strtok(NULL,",");   /* name of room to suppress */
      if( item != NULL )
        {
        ptr->room = strdup(item);
        item      = strtok(NULL,","); /* name of node to suppress */
        if( item != NULL )
          {
          ptr->system = strdup(item);
          if( strCmpU(ptr->room,  "*" ) == SAMESTRING &&
              strCmpU(ptr->name,  "*" ) == SAMESTRING &&
              strCmpU(ptr->system,"*" ) == SAMESTRING )
            {
            splitF(netLog, "Invalid room/name/system, cannot be all *");
            }
          else return (void *)ptr;
          }
        else splitF(netLog, "Invalid NodeName to EatIckyPeople:%s.\n",str);
        }
      else   splitF(netLog, "Invalid Room to EatIckyPeople:%s.\n", str);
       }
    else     splitF(netLog, "Invalid Name to EatIckyPeople:%s.\n", str);
    };
  return NULL;

  }
/*
* FindIckyPeople()
*
* This will check to see if a person/Room/System exists in the current message.
* This is used by the list handling functions.
*/
void *FindIckyPeople( struct User_Id *User_Record,  MessageBuffer *msg)
  {
  char *c;
  c = NULL;
  if( strCmpU(User_Record->name, msg->mbauth) == SAMESTRING
   || strCmpU(User_Record->name,         "*") == SAMESTRING )
    {
    if( strCmpU(User_Record->room, msg->mbroom) == SAMESTRING
     || strCmpU(User_Record->room,         "*") == SAMESTRING )
      {
      if( strCmpU(User_Record->system, msg->mboname) == SAMESTRING
       || strCmpU(User_Record->system,          "*") == SAMESTRING )
        {
        c = User_Record->name;
        splitF(netLog, "FindIckyPeople found -%s/%s/%s - in message.\n"
      , User_Record->name, User_Record->room, User_Record->system);
        };
      };
    };
  return c;

  }
/*
* mFormat()
*
* This function does the work of formatting a string to modem and console.
*/
void mFormat(char *string)
  {
  char wordBuf[MAXWORD];
  int  i;
  for (i = 0;  string[i] && (outFlag == OUTOK    ||
  outFlag == IMPERVIOUS ||
  outFlag == OUTPARAGRAPH);  )
    {
    i = getWord(wordBuf, string, i, MAXWORD);
    putWord(wordBuf);
    if (mAbort()) return;

    }

  }
/*
* moveMessage()
*
* This function moves a message for pullIt().
*/
char moveMessage(char which, int m, char *toReturn)
  {
  label blah;
  int   roomTarg, ourRoom;
  int   curRoom;
  char  tempauth[129];
  curRoom = thisRoom;
  if (!getXString("DESTRM", blah, 20, oldTarget, oldTarget))
  return FALSE;
  if ((roomTarg = roomCheck(roomExists, blah)) == ERROR)
    {
    if ((roomTarg = roomCheck(partialExist, blah)) == ERROR)
      {
      mPrintf("'%s' does not exist.", blah);
      return FALSE;

      }
    else
      {
      thisRoom = roomTarg;
      if (roomCheck(partialExist, blah) != ERROR)
        {
        thisRoom = curRoom;
        mPrintf("'%s' is not a unique string.", blah);
        return FALSE;

        }
      thisRoom = curRoom;

      }

    }
  strCpy(oldTarget, roomTab[roomTarg].rtname);
  ourRoom = thisRoom;
  DelMsg(which == 'M', m);
  getRoom(roomTarg);
  noteAMessage(roomBuf.msg, MSGSPERRM, pulledMId, pulledMLoc);
  putRoom(thisRoom);
  noteRoom();
  /* is message is going to a shared room ... */
  if (roomBuf.rbflags.SHARED)
    {
    /* if message was originally netted or if aide wants message shared */
    if (strLen(msgBuf.mboname) != 0 || getYesNo("MKENET"))
    MakeNetted(MSGSPERRM - 1);

    }
  getRoom(ourRoom);
  strCpy(tempauth, msgBuf.mbauth);
  ZeroMsgBuffer(&msgBuf);
  sPrintf(
  msgBuf.mbtext,
  "Following message from %s %sed from %s",
  (tempauth[0]) ? tempauth : "<anonymous>",
  (which == 'M') ? "mov" : "copi",
  formRoom(thisRoom, FALSE, FALSE));
  sPrintf(lbyte(msgBuf.mbtext), " to %s by %s",
  formRoom(roomTarg, FALSE, FALSE),
  logBuf.lbname
  );
  aideMessage(NULL, /* noteDeletedMessage == */ TRUE);
  *toReturn = (which == 'M') ? DELETED : NO_CHANGE;
  return TRUE;

  }
/*
* mPeek()
*
* This function dumps a sector in msgBuf.  sysop debugging tool.
*/
void mPeek()
  {
  #ifdef TEST_SYS
  char visible();
  char blup[50];
  DATA_BLOCK peekBuf;
  int  col, row;
  MSG_NUMBER r, s;
  s = getNumber("DUMPSC", 0l, (MSG_NUMBER) (cfg.maxMSector-1));
  r = s * MSG_SECT_SIZE;
  fseek(msgfl, r, 0);
  fread(peekBuf, MSG_SECT_SIZE, 1, msgfl);
  crypte(peekBuf, MSG_SECT_SIZE, 0);
  for (row = 0;  row < 2;  row++)
    {
    mPrintf("\n ");
    for (col = 0;  col < 64;  col++)
      {
      mPrintf("%c", visible(peekBuf[row*64 +col]));
      if (!isprint(peekBuf[row*64 +col]))
      mPrintf("(%x)", peekBuf[row*64 +col]);

      }

    }
  #else
  printf("Disabled\n");
  #endif

  }
/*
* msgToDisk()
*
* This puts a message to the given disk file.
*/
void msgToDisk(char *filename, char all, MSG_NUMBER id, SECTOR_ID loc,
UNS_16 size)
  {
  char *fn=NULL;
  long x;
  if( filename != NULL )
    {
    fn = GetDynamic(strLen(filename) + 5);
    strCpy(fn, filename);
    };
  id &= S_MSG_MASK;
  outFlag = OUTOK;
  if (redirect(filename))
    {
    if (!all)
      {
      if (findMessage(loc, id, TRUE))
        {
        if (size != 0)
          {
          totalBytes(&x, upfd);
          if (x > size * 1024)
            {
            undirect();
            FindNextFile(fn);
            rename(filename, fn);
            redirect(filename);

            }

          }
        printMessage(0);

        }
      else splitF(netLog, "bad findmessage!\n"    );

      }
    else
      {
      showMessages(OLDaNDnEW, FALSE, 0l, printMessage);

      }
    undirect();

    }
  free(fn);

  }
SListBase Errors =
  {
  NULL, NULL, NULL, free, NULL

  };
/*
* noteMessage()
*
* This function slots message into current room, delivers mail if necessary,
* handles net mail, Who Else stuff, mail forwarding, checkpointing, room
* archiving, etc etc etc etc etc.
*/
void noteMessage(logBuffer *lBuf)
  {
  int logRover, size = 0;
  char *fn, *realfn;
  CheckPoint Cpt;
  FILE *fd;
  extern SListBase Arch_base;
  extern char     *ALL_LOCALS, *WRITE_ANY;
  void AssembleMessage();
  KillList(&FwdVortex);
  ArchiveMail = FALSE;
  ++cfg.newest;
  if (lBuf == &logBuf)
  logBuf.lbvisit[0] = cfg.newest;
  if (thisRoom != MAILROOM)
    {
    noteAMessage(roomBuf.msg, MSGSPERRM, cfg.newest, cfg.catSector);
    /* write it to disk:            */
    putRoom(thisRoom);
    noteRoom();

    }
  else
    {
    /* when in Mail... */
    /*
    * First, we handle the origin of this message in Mail>.  Note that
    * checking loggedIn handles both anonymous and incoming NET mail --
    * loggedIn is always false when in net mode, or should be!
    */
    if (loggedIn || lBuf != &logBuf)
      {
      noteAMessage(lBuf->lbMail, MAILSLOTS, cfg.newest, cfg.catSector);
      if (lBuf == &logBuf)
      putLog(&logBuf, thisLog);
      ArchiveMail = (strCmpU(cfg.SysopName, logBuf.lbname) == SAMESTRING);

      }
    /*
    * If there are overrides on delivery target, process them in
    * preference to the mbto and mbCC fields.
    */
    if (HasOverrides(&msgBuf))
      {
      RunList(&msgBuf.mbOverride, AddMail);

      }
    else if (msgBuf.mbaddr[0] ||
    strCmpU(msgBuf.mbto, lBuf->lbname) != SAMESTRING)
      {
      /* kinda silly, but .. */
      if (msgBuf.mbaddr[0] && inNet == NON_NET)
        {
        if (strCmpU(msgBuf.mbaddr, ALL_LOCALS) != SAMESTRING)
        AddNetMail(msgBuf.mbaddr, TRUE);
        else
          {
          for (logRover = 0; logRover < cfg.netSize; logRover++)
            {
            /* this code could be better optimized (speed) */
            getNet(logRover, &netBuf);
            if (netBuf.nbflags.in_use && netBuf.nbflags.local &&
            (netBuf.MemberNets & ALL_NETS))
            AddNetMail("", TRUE);

            }

          }

        }
      else if ((logRover = PersonExists(msgBuf.mbto)) == ERROR)
        {
        mPrintf("Internal error in mail (-%s-)!\n ", msgBuf.mbto);
        return ;

        }
      else if (logRover == cfg.MAXLOGTAB)
        {
        /* special recipient */
        if (strCmpU(msgBuf.mbto, "Citadel") == SAMESTRING)
          {
          if (!msgBuf.mbaddr[0])
            {
            /* Not netward bound?? */
            for (logRover = 0; logRover < cfg.MAXLOGTAB;
            logRover++)
            if (logRover != thisLog)
              {
              printf("Log %d\r", logRover); /* Notify sysop */
              getLog(&logTmp, logRover);
              if (logTmp.lbflags.L_INUSE)
                {
                noteAMessage(logTmp.lbMail, MAILSLOTS,
                cfg.newest, cfg.catSector);
                putLog(&logTmp, logRover);

                }

              }

            }

          }
        else
          {
          AddMail(msgBuf.mbto);

          }

        }
      else
        {
        AddMail(msgBuf.mbto);

        }

      }
    if (inNet == NON_NET)
      {
      RunList(&msgBuf.mbCC, AddMail);

      }
    if (lBuf == &logBuf)
    fillMailRoom();                      /* update room also */

    }
  /* Finally, kill this list */
  KillList(&SysList);
  /* Checkpoint stuff - this HAS to be here before catSector is changed */
  Cpt.ltnewest = cfg.newest;
  Cpt.loc        = cfg.catSector;
  /* make message official:   */
  cfg.catSector   = mFile1.thisSector;
  cfg.catChar     = mFile1.thisChar;
  setUp(FALSE);
  if (roomBuf.rbflags.ARCHIVE == 1 ||
  (ArchiveMail && strLen(cfg.SysopArchive) != 0))
    {
    if (roomBuf.rbflags.ARCHIVE == 1)
    fn = SearchList(&Arch_base, NtoStrInit(thisRoom, "", 0, TRUE));
    else
    fn = cfg.SysopArchive;
    realfn = GetDynamic(strLen(fn) + 15);
    TranslateFilename(realfn, fn);
    if (fn == NULL)
      {
      sPrintf(msgBuf.mbtext, "Integrity problem with Archiving: %s.",
      roomBuf.rbname);
      aideMessage(NULL,FALSE);

      }
    else
      {
      if (roomBuf.rbflags.ARCHIVE == 1)
      size = GetArchSize(thisRoom);
      msgToDisk(realfn, FALSE, Cpt.ltnewest, Cpt.loc, size);

      }
    free(realfn);

    }
  msgBuf.mbaddr[0] = 0;
  msgBuf.mbto[0]   = 0;
  /* OK, so let's write out our checkpoint */
  if (inNet == NON_NET) /* this is strictly performance oriented */
  if ((fd = fopen(CHECKPT, WRITE_ANY)) != NULL)
    {
    fwrite(&Cpt, sizeof Cpt, 1, fd);
    fclose(fd);

    }
  if (GetFirst(&Errors) != NULL)
    {
    ZeroMsgBuffer(&msgBuf);
    RunList(&Errors, AssembleMessage);
    KillList(&Errors);
    CleanEnd(msgBuf.mbtext);
    aideMessage("Net Aide",FALSE);

    }

  }
/*
* AssembleMessage()
*
* This adds a submessage to a message. (?)
*/
void AssembleMessage(char *str)
  {
  sPrintf(lbyte(msgBuf.mbtext), "%s\n\n", str);

  }
/*
* AddMail()
*
* This function should deliver mail to the named person.
*/
void AddMail(char *DaPerson)
  {
  label person;
  char  system[(NAMESIZE * 2) + 10];
  int   slot;
  char *InternalError = "Internal error, couldn't identify '%s'\n ";
  if (cfg.BoolFlags.debug)
      splitF(netLog,"AddMail(%s)\n",DaPerson);
  switch (SepNameSystem(DaPerson, person, system, &netBuf))
    {
    case IS_SYSTEM:
    case SYSTEM_IS_US:
    if (!NetValidate(TRUE)) return;
    AddNetMail(system, TRUE);
    break;
    case BAD_FORMAT:
    if (inNet == NON_NET) mPrintf(InternalError, DaPerson);
    break;
    case NO_SYSTEM:
    if (inNet == NON_NET) mPrintf(InternalError, system);
    break;
    case NOT_SYSTEM:
    if (strCmpU(DaPerson, "sysop") == SAMESTRING)
      {
      ArchiveMail = TRUE;
      if ((slot = findPerson(cfg.SysopName, &logTmp)) == ERROR)
        {
        getRoom(AIDEROOM);
        /* enter in Aide> room -- 'sysop' is special */
        noteAMessage(roomBuf.msg, MSGSPERRM,
        cfg.newest, cfg.catSector);
        /* write it to disk:        */
        putRoom(AIDEROOM);
        noteRoom();
        getRoom(MAILROOM);

        }
      else
      MailWork(slot);

      }
    else if ((slot = findPerson(DaPerson, &logTmp)) == ERROR)
      {
      if (inNet == NON_NET)
      mPrintf(InternalError, DaPerson);
      else
      splitF(netLog, "No recipient: %s\n", DaPerson);

      }
    else
      {
      MailWork(slot);

      }

    }

  }
/*
* MailWork()
*
* This function is central to Mail delivery, and handles forwarding of both
* sorts.
*/
void MailWork(int slot)
  {
  logBuffer lBuf;
  if( logNetResults )
  if (inNet != NON_NET) splitF(netLog, "Mail for %s.\n", logTmp.lbname);
  noteAMessage(logTmp.lbMail, MAILSLOTS, cfg.newest, cfg.catSector);
  if (strCmpU(cfg.SysopName, logTmp.lbname) == SAMESTRING)
  ArchiveMail = TRUE;
  NetForwarding(&logTmp);
  putLog(&logTmp, slot);
  /* so we can't redeliver to this account */
  AddData(&FwdVortex, strdup(logTmp.lbname), NULL, FALSE);
  /* now check for forwarding to a local address */
  initLogBuf(&lBuf);
  LocalForwarding(FindLocalForward(logTmp.lbname), &lBuf);
  killLogBuf(&lBuf);

  }
/*
* NetForwarding()
*
* This handles network forwarding.
*/
void NetForwarding(logBuffer *lBuf)
  {
  int cost;
  ForwardMail *data;
  label domain;
  char *system;
  extern SListBase MailForward;
  /* Has forwarding address?  */
  if ((data = SearchList(&MailForward, lBuf->lbname)) != NULL &&
  lBuf->lbflags.NET_PRIVS)
    {
    system = strdup(data->System);
    if (ReqNodeName("", system, domain, FALSE, TRUE, FALSE, FALSE,
    FALSE, &netTemp))
      {
      if (domain[0] == 0)
      cost = !netTemp.nbflags.local;
      else
      cost = FindCost(domain);
      free(system);
      system = strdup(data->System);
      if (lBuf->credit >= cost)
        {
        AddData(&msgBuf.mbInternal, strdup(lBuf->lbname), NULL, FALSE);
        AddNetMail(system, FALSE);
        KillList(&msgBuf.mbInternal);

        }

      }
    free(system);

    }

  }
/*
* LocalForwarding()
*
* This handles forwarding to a local account.  Since multiple forwarding
* may* be setup (seems unlikely), this is recursive.  A list is kept of
* recipients of mail in order to avoid both duplicate deliveries and
* infinite forwarding vortexes.
*/
void LocalForwarding(char *name, logBuffer *workBuf)
  {
  int   slot;
  /* if this is NULL then there is no more forwarding to do */
  if (name == NULL) return;
  /* see if this account has already received the mail */
  if (SearchList(&FwdVortex, name) != NULL) return;
  if ((slot = findPerson(name, workBuf)) == ERROR)
  return;               /* implies an outofdate account */
  /* OK, save the message */
  noteAMessage(workBuf->lbMail, MAILSLOTS, cfg.newest, cfg.catSector);
  /* check the network forwarding for this account */
  NetForwarding(workBuf);
  putLog(workBuf, slot);
  AddData(&FwdVortex, strdup(name), NULL, FALSE);
  LocalForwarding(FindLocalForward(name), workBuf);

  }
/*
* AddNetMail()
*
* This should manage adding mail to a net system.
*/
void AddNetMail(char *system, char CreditSender)
  {
  int cost, slot;
  logBuffer *lBuf;
  char isdomain = FALSE, *domain, *System;
  /*
  * sometimes system is mbaddr, which is not good, because later on down
  * the line we call findMessage, which will result in (unfortunately)
  * mbaddr being overwritten.  So we dup system.
  */
  if (cfg.BoolFlags.debug)
      splitF(netLog,"AddNetMail(%s,%s)\n"
             ,system,(CreditSender ? "TRUE" : "FALSE"));

  System = strdup(system);
  if (CreditSender) lBuf = &logBuf;
  else           lBuf = &logTmp;
  if (strLen(System))
    {
    isdomain = (domain = strchr(System, '_')) != NULL;
    if (!isdomain)
      {
      slot = searchNameNet(System, &netTemp);
      cost = !netTemp.nbflags.local;

      }
    else
      {
      *domain++ = 0;
      NormStr(domain);
      NormStr(System);
      cost = FindCost(domain);

      }

    }
  else
    {
    slot = thisNet;
    cost = 0;
    getNet(thisNet, &netTemp);  /* &L mail */
    system = netTemp.netName;

    }
  if (cost > lBuf->credit && !(lBuf == &logBuf && HalfSysop()))
    {
    free(System);
    return ;

    }
  lBuf->credit -= cost;
  if (SearchList(&SysList, system) == NULL)
    {
    AddData(&SysList, strdup(system), NULL, FALSE);
    if (netBuf.nbflags.OtherNet)
      {
      isdomain = FALSE;  /* override possible domain for other net */
      domain   = '\0';
      };
    netMailOut(isdomain, System, domain, TRUE, slot);

    }
  free(System);
  if (lBuf->credit < 0)
  lBuf->credit = 0;

  }
/*
* noteAMessage()
*
* This should add a message pointer to any room.
*/
void noteAMessage(theMessages *base, int slots, MSG_NUMBER id, SECTOR_ID loc)
  {
  int  i;
  /* store into current room: */
  /* slide message pointers down to make room for this one:      */
  for (i = 0;  i < slots - 1;  i++)
    {
    base[i].rbmsgLoc  = base[i+1].rbmsgLoc;
    base[i].rbmsgNo   = base[i+1].rbmsgNo ;

    }
  /* slot this message in:      */
  base[slots-1].rbmsgNo     = id ;
  base[slots-1].rbmsgLoc    = loc;

  }
/*
* printMessage()
*
* This prints the indicated message on modem & console.
*/
char printMessage(int status)
  {
  int  moreFollows;
  int  oldTermWidth;
  int  strip;
  extern char CCfirst, CCOutFlag;
  if( termWidth < 40 ) termWidth = 40;   /* force a semi-readable size */
  oldTermWidth = termWidth;
  if (outPut == DISK)
    {
    termWidth = 80;

    }
  doCR();
  mPrintf("%s", formHeader());
  doCR();
  /* Print out who is on the CC list for this message. */
  ShowCC(SCREEN);
  EOP = TRUE;
  if (status == 0)
    {
    if (outFlag != OUTSKIP && outFlag != OUTNEXT)
    while (1)
      {
      moreFollows     = dGetWord(msgBuf.mbtext, 150);
      /* strip control Ls out of the output                */
      for (strip = 0; msgBuf.mbtext[strip] != 0; strip++)
        if(msgBuf.mbtext[strip] == 0x0C ||
           msgBuf.mbtext[strip] == 0x0E ||
           msgBuf.mbtext[strip] == 0x0F ) msgBuf.mbtext[strip] = ' ';
      if (msgBuf.mbtext[strip] == SPECIAL && !logBuf.lbflags.ANSI) msgBuf.mbtext[strip] = ' ';
      putWord(msgBuf.mbtext);
      if (!(moreFollows  &&  !mAbort()))
        {
        if (outFlag == OUTNEXT)          /* If <N>ext, extra line */
        doCR();
        break;

        }

      }

    }
  else
    {
    mFormat(msgBuf.mbtext);

    }
  if (EndWithCR) doCR();
  termWidth = oldTermWidth;
  return TRUE;

  }
/*
* pullIt()
*
* This is a sysop special to remove or otherwise manipulate a message in a
* room.
*/
char pullIt(int m)
  {
  char  toReturn;
  char  answer;
  char *DelOpts[] =
    {
    "Delete message\n", "Move message\n", "Copy message\n",
    "Abort\n", " ", ""

    };
  /* confirm that we're removing the right one:  */
  outFlag = OUTOK;
  if (findMessage(roomBuf.msg[m].rbmsgLoc, roomBuf.msg[m].rbmsgNo, TRUE))
  printMessage(0);
  if (roomBuf.rbflags.SHARED && !msgBuf.mboname[0])
    {
    ExtraOption(DelOpts, "Net message");

    }
  RegisterThisMenu(NULL, DelOpts);
  do
    {
    outFlag = IMPERVIOUS;
    TellRoute();
    mPrintf("\n <D>elete <M>ove <C>opy <A>bort");
    if (roomBuf.rbflags.SHARED && !msgBuf.mboname[0])
    mPrintf(" <N>et");
    mPrintf("? (D/M/C/A%s) ",
    (roomBuf.rbflags.SHARED && !msgBuf.mboname[0]) ? "/N" : "");
    switch ((answer = GetMenuChar()))
      {
      case 'D':
      if (deleteMessage(m))
      return DELETED;
      break;
      case 'M':
      case 'C':
      if (moveMessage(answer, m, &toReturn))
      return toReturn;
      break;
      case 'A':
      return NO_CHANGE;
      case 'N':
      return MakeNetted(m);

      }

    }
  while (onLine());
  return DELETED;

  }
/*
* putMessage()
*
* This function stores a message to disk.
* Always called before noteMessage() -- newest not ++ed yet.
* Returns: TRUE on successful save, else FALSE
*/
char putMessage(logBuffer *lBuf)
  {
  char *s;
  extern char *ALL_LOCALS, *WRITE_LOCALS, CCOutFlag;
  extern char *R_SH_MARK, *LOC_NET, *NON_LOC_NET;
  void dLine();
  startAt(msgfl, &mFile1, cfg.catSector, cfg.catChar);
  /* tell putMsgChar where to write   */
  if (cfg.BoolFlags.mirror)
  startAt(msgfl2, &mFile2, cfg.catSector, cfg.catChar);
  putMsgChar(0xFF);              /* start-of-message             */
  /* write message ID */
  dPrintf("%lu", cfg.newest + 1);
  if (inNet != NON_NET ||
  (!roomBuf.rbflags.ANON || strCmp(msgBuf.mbdate, "****") != SAMESTRING))
    {
    /* write date:       */
    if (msgBuf.mbdate[0])
      {
      dPrintf("D%s", msgBuf.mbdate);

      }
    else
      {
      dPrintf("D%s", formDate());

      }
    /* write time:       */
    if (msgBuf.mbtime[0])
      {
      dPrintf("C%s", msgBuf.mbtime);

      }
    else
      {
      dPrintf("C%s", Current_Time());

      }
    /* write author's name out:  */
    if (msgBuf.mbauth[0])
      {
      dPrintf("A%s", msgBuf.mbauth);

      }

    }
  else
    {
    dPrintf("D****");

    }
  /* write room name out:            */
  dPrintf("R%s", msgBuf.mbroom[0] ? msgBuf.mbroom : roomBuf.rbname);
  if (msgBuf.mbto[0])
    {
    /* private message -- write addressee   */
    dPrintf("T%s", msgBuf.mbto);

    }
  if (msgBuf.mboname[0])
    {
    dPrintf("N%s", msgBuf.mboname);

    }
  if (msgBuf.mbdomain[0])
    {
    dPrintf("X%s", msgBuf.mbdomain);

    }
  if (msgBuf.mborig[0])
    {
    dPrintf("O%s", msgBuf.mborig);

    }
  /* this convolution lets us retrace routing for shared rooms */
  if (msgBuf.mbaddr[0])
    {
    /* net message routing */
    /* generated by user */
    if (inNet == NON_NET || (strCmp(msgBuf.mbaddr, LOC_NET) != SAMESTRING &&
    strCmp(msgBuf.mbaddr, NON_LOC_NET) != SAMESTRING))
    dPrintf("Q%s", wrNetId(msgBuf.mbaddr));
    else            /* saving a net message              */
    dPrintf("Q%s%d", wrNetId(msgBuf.mbaddr), thisNet);
    if (strCmpU(msgBuf.mbaddr, R_SH_MARK  ) == SAMESTRING ||
    strCmpU(msgBuf.mbaddr, NON_LOC_NET) == SAMESTRING)
    roomTab[thisRoom].rtlastNet = cfg.newest + 1;

    }
  if (msgBuf.mbsrcId[0])
    {
    dPrintf("S%s", msgBuf.mbsrcId);

    }
  if (msgBuf.mbOther[0])
    {
    dPrintf("P%s", msgBuf.mbOther);

    }
  if (msgBuf.mbreply[0])
  dPrintf("w%s", msgBuf.mbreply);               /* back ptr */
  /* This writes out the list of CC people to the message base. */
  /* Note we don't usually write Overrides to the message base. */
  CCOutFlag = MSGBASE;
  RunList(&msgBuf.mbCC, DisplayCC);
  /* save foreign fields */
  RunList(&msgBuf.mbForeign, dLine);
  /* write message text by hand because it would overrun dPrintf buffer: */
  putMsgChar('M');    /* M-for-message.  */
  for (s = msgBuf.mbtext;  *s;  s++) putMsgChar(*s);
  putMsgChar(0);         /* null to end text     */
  flushMsgBuf();
  noteMessage(lBuf);
  return  TRUE;

  }
/*
* dLine()
*
* This prints a line to the msg base, including the NULL byte.
*/
void dLine(char *garp)
  {
  do
  putMsgChar(*garp);
  while (*garp++);

  }
/*
* netMailOut()
*
* This should put the mail pointer and number into temp file for local mail,
* or will set up the temp file for routed mail.
*  SOMEDAY MOVE THIS INTO NETMISC!
*/
void netMailOut(char isdomain, char *system, char *domain, char MsgBase,
int slot)
  {
  FILE  *fd;
  label temp, id = "";
  int    result;
  DOMAIN_FILE fn;
  extern char *APPEND_ANY, *WRITE_ANY;
  struct        netMLstruct buf;
  if (cfg.BoolFlags.debug)
     splitF(netLog,"netMailOut(%s,%s,%s,%s.%d)\n"
            ,(isdomain ? "TRUE" : "FALSE")
            ,system, domain, (MsgBase ? "TRUE" : "FALSE"));
  if (isdomain)
    {
    if ((result = DomainMailFileName(fn, domain, id, system))==LOCALROUTE)
      {
      isdomain = FALSE;
      slot = searchNameNet(system, &netTemp);

      }

    }
  else
    {
    result = LOCALROUTE;
    if (!MsgBase) slot = searchNameNet(system, &netTemp);
    if (slot == ERROR)
      {
      splitF(netLog, "BUG!  Slot is -1 for %s.\n", system);
      return;

      }

    }
  if (cfg.BoolFlags.debug)
    {
    splitF(netLog, "isdomain:%s ", (isdomain ? "TRUE" : "FALSE") );
    splitF(netLog, "MsgBase:%d ", MsgBase   );
    splitF(netLog, "inNet==NON_NET:%s ", (inNet==NON_NET ? "TRUE" : "FALSE") );
    };
  if (!isdomain && MsgBase && DirectRoute(&netTemp) && inNet == NON_NET)
    {
    if (cfg.BoolFlags.debug) splitF(netLog, "local mail for slot:%d\n",slot);

    sPrintf(temp, "%d.ml", slot);
    makeSysName(fn, temp, &cfg.netArea);
    if ((fd = safeopen(fn, APPEND_ANY)) == NULL)
      {
      crashout("putMessage -- couldn't open direct mail file!");

      }
    buf.ML_id  = cfg.newest;
    buf.ML_loc = cfg.catSector;
    putMLNet(fd, buf);
    fclose(fd);
    netTemp.nbflags.normal_mail = TRUE;
    putNet(slot, &netTemp);

    }
  else
    {
    MakeIntoRouteMail(result, fn, isdomain, system, domain, MsgBase, slot);

    }

  }
/*
* MakeIntoRouteMail()
*
* This will set up a Rx.x file.
*/
void MakeIntoRouteMail(int result, DOMAIN_FILE fn
                       , char isdomain, char *system
                       , char *domain, char OriginIsMsgBase, int slot)
  {
  int    index, route;
  label temp;
  label name;
  char  For[(2 * NAMESIZE) + 10];
  void  (*TempPrint)(char *fmt, ...);
  extern void (*NetPrintTarget)(char *fmt, ...);
  extern int  (*ToFileWork)();
  extern char PrTransmit;

  if (cfg.BoolFlags.debug)
     splitF(netLog,"MakeIntoRouteMail(%d, %s, %s, %s, %s, %s, %d)\n"
            , result, fn, (isdomain ? "TRUE" : "FALSE")
            , system, domain, ( OriginIsMsgBase ? "TRUE" : "FALSE")
            , slot);


  route = (netTemp.nbRoute == -1) ? slot : netTemp.nbRoute;
  if (cfg.BoolFlags.debug) splitF(netLog, "route mail: system(%s) domain(%s) route(%d) \n",system,domain,route);
  if (result == LOCALROUTE)
    {
    strCpy(name, UseNetAlias(netTemp.netName, TRUE));
    index = FindRouteIndex(route);
    sPrintf(temp, "R%d.%d", route, index);
    makeSysName(fn, temp, &cfg.netArea);
    strCpy(For, netTemp.netName);

    }
  else sPrintf(For, "%s _ %s", system, domain);
  if (cfg.BoolFlags.debug) splitF(netLog, "route mail filename is %s for %s\n",fn, For);
  if ((upfd = safeopen(fn, WRITE_ANY)) == NULL)
    {
    splitF(netLog,"filename is -%s-\n", fn);
    crashout("couldn't open route mail file!");

    }
  ToFile("%-20s", (isdomain) ? " " : netTemp.netId);
  ToFile("%-20s", (isdomain) ? system : name);
  if (OriginIsMsgBase)
  findMessage(cfg.catSector, cfg.newest, FALSE);  /* we use false here */
  TempPrint = NetPrintTarget;
  NetPrintTarget = ToFile;
  StartEncode(putFLChar);
  ToFileWork = Encode;
  PrTransmit = FALSE;
  prNetStyle(!OriginIsMsgBase, Encode, OriginIsMsgBase, For);
  PrTransmit = TRUE;
  StopEncode();
  ToFileWork = putFLChar;
  NetPrintTarget = TempPrint;
  fclose(upfd);
  if (!isdomain)
    {
    if (inNet == NON_NET || thisNet != route)
      {
      getNet(route, &netTemp);
      netTemp.nbflags.HasRouted = TRUE;
      netTemp.nbHiRouteInd = index + 1;
      putNet(route, &netTemp);

      }
    else
      {
      netBuf.nbflags.HasRouted = TRUE;
      netBuf.nbHiRouteInd = index + 1;  /* saved by net stuff */

      }

    }
  else DomainFileAddResult(domain, system, "", DOMAIN_SUCCESS);

  }
/*
* putMsgChar()
*
* This writes successive message chars to disk.
*
* Globals:      thisChar=       thisSector=
* Returns:      ERROR if problems else TRUE
*/
int putMsgChar(int c)
  {
  int  toReturn;
  int  count1, count2;
  toReturn = TRUE;
  count1 = doActualWrite(msgfl, &mFile1, c);
  if (cfg.BoolFlags.mirror)
    {
    count2 = doActualWrite(msgfl2, &mFile2, c);
    if (count1 != count2) printf("Mirror msg count discrepancy!");

    }
  if (count1)
  logBuf.lbvisit[(MAXVISIT-1)]    = ++cfg.oldest;
  return toReturn;

  }
/*
* putWord()
*
* This function writes one word to modem & console.
*/
void putWord(char *st)
  {
  char *s;
  int  newColumn;
  if( termWidth < 40 ) termWidth = 40;   /* force a semi-readable size */
  for (newColumn = crtColumn, s = st;  *s; s++)
    {
    /* make special adjustment for escape sequences */
    if( *s == SPECIAL ) newColumn -= ( *(s+2) == '0' ? 4 : 8);
    if (*s != TAB)
      {
      if (*s == '\b') newColumn--;
      else if (*s == '\n')
        {
        if (*(s+1) == '\n' || *(s+1) == ' ')
        newColumn = 1;
        else ++newColumn;

        }
      else ++newColumn;

      }
    else            while (++newColumn % 8);

    }
  if (newColumn > termWidth)
    {
    doCR();
    if (*st == '\n' && *(st+1) != '\n' && *(st+1) != ' ' && *(st+1))
    st++;

    }
  BufferingOn();
  for (;  *st;  st++)
    {
    #ifdef OLD_STYLE
    if (*st != TAB) ++crtColumn;
    else            while (++crtColumn % 8);
    #else
    #ifdef NEEDED
    if (*st != TAB)
      {
      if (*st == '\b') crtColumn--;
      else ++crtColumn;

      }
    else            while (++crtColumn % 8);
    #endif
    #endif
    /* worry about words longer than a line:    */
    if( *st == SPECIAL ) crtColumn -= ( *(s+2) == '0' ? 4 : 8);
    if (crtColumn > termWidth) doCR();
    if (*st == '\n' && EOP)
    doCR();
    else if (prevChar!=NEWLINE  ||  (*st > ' '))
      {
      oChar(*st);
      if (*st > ' ') EOP = FALSE;

      }
    else
      {
      /* end of paragraph: */
      if (outFlag == OUTPARAGRAPH)
        {
        outFlag = OUTOK;

        }
      doCR();
      if (*st == '\n' && !EOP) doCR();
      else oChar(*st);
      EOP = TRUE;

      }

    }
  BufferingOff();

  }
/*
* showMessages()
*
* This function will try to print a roomfull of messages.
*/
char pause_whichMess;   /* kludge to avoid major problems */

int showMessages(char whichMess, char revOrder, MSG_NUMBER LastMsg,
char (*Style)(int i))
  {
  int           i, start, finish, increment, MsgCount = 0, result;
  MSG_NUMBER    lowLim, highLim, msgNo;
  char  pulled, PEUsed = FALSE, LoopIt;
  pause_whichMess = whichMess;
  setUp(FALSE);
  /* Don't need to check net status 'cuz netMail is sent differently. */
  if (thisRoom == MAILROOM && !loggedIn)
    {
    printHelp("POLICY.HLP");
    return 1;

    }
  if (TransProtocol == ASCII && inNet == NON_NET)
    Output_Citadel_Message("HOTHLP",NULL,NULL,NULL);
  /* This shouldn't bother the net. */
  if (whichIO != CONSOLE && thisRoom == MAILROOM) echo = CALLER;
  SetShowLimits(revOrder, &start, &finish, &increment);
  if (Showing == WHATEVER) Showing = MSGS;
  switch (whichMess)
    {
    case NEWoNLY:
    lowLim  = LastMsg + 1l;
    highLim = cfg.newest;
    if (inNet == NON_NET && !revOrder && TransProtocol == ASCII &&
    thisRoom != MAILROOM && oldToo)
      {
      for (i = MSGSPERRM - 1; i != -1; i--)
      if (lowLim > roomBuf.msg[i].rbmsgNo &&
      roomBuf.msg[i].rbmsgNo >= cfg.oldest)
      break;
      if (i != -1)
        {
        LoopIt = TRUE;
        while (i != -1 && LoopIt)
          {
          LoopIt = FALSE;
          findMessage(roomBuf.msg[i].rbmsgLoc,
          roomBuf.msg[i].rbmsgNo, TRUE);
          (*Style)(1);
          /* Pause-Enter for the last old on new feature */
          if (MsgStreamEnter)
            {
            LoopIt = TRUE;
            PEUsed = TRUE;
            if (InterruptMessage()) --i;

            }

          }

        }

      }
    break;
    case OLDaNDnEW:
    lowLim  = cfg.oldest;
    highLim = cfg.newest;
    break;
    case OLDoNLY:
    lowLim  = cfg.oldest;
    highLim = LastMsg;
    break;

    }
  /* stuff may have scrolled off system unseen, so: */
  if (cfg.oldest  > lowLim)
    {
    lowLim = cfg.oldest;

    }
  /*
  * We'll increment this loop at the end.  Doing so eases the job of
  * implementing the Pause-E option.
  */
  for (i = start; i != finish && (onLine() || inNet == NET_CACHE); )
    {
    if (outFlag != OUTOK)
      {
      if (outFlag == OUTNEXT || outFlag == OUTPARAGRAPH)
      outFlag = OUTOK;
      else if (outFlag == OUTSKIP)
        {
        echo = BOTH;
        return MsgCount;

        }

      }
    /* first get the REAL msgNo -- this is a kludge, replace next m. r. */
    msgNo = (roomBuf.msg[i].rbmsgNo & S_MSG_MASK);
    /*
    * Now check to see if msg is in "to be read" range, OR if we are
    * reading New AND the message is marked as SKIPPED (only happens in
    * Mail).  Note at the moment we're not going to worry about net
    * mode -- we don't use this loop for sending Mail, although we do
    * for other rooms.
    */
    if (
    (msgNo >= lowLim && highLim >= msgNo) ||
    (whichMess == NEWoNLY && msgNo != roomBuf.msg[i].rbmsgNo &&
    msgNo > cfg.oldest)
    )
      {
      if (findMessage(roomBuf.msg[i].rbmsgLoc, msgNo, TRUE))
        {
        ReverseMessage = FALSE;
        if ((*Style)(0))
          {
          /* successful print? */
          MsgCount++;
          /**
            Pull current message from room if flag set
          **/
          if (pullMessage)
            {
            pullMessage = FALSE;
            pulled = pullIt(i);
            outFlag = OUTOK;
            switch (pulled)
              {
              case NO_CHANGE:
              break;
              case DELETED:
              if (revOrder)   i++;
              break;
              case NETTED:
              if (!revOrder)   i--;
              break;

              }

            }
          else   pulled = FALSE;
          /**
            Reverse Order of Read
          **/
          if (ReverseMessage)
            {
            revOrder = !revOrder;
            SetShowLimits(revOrder, &start, &finish, &increment);

            }
          /**
            Journal Message
          **/
          if (journalMessage)
            {
            msgToDisk(NULL, FALSE, msgNo, roomBuf.msg[i].rbmsgLoc, 0);
            journalMessage = FALSE;

            }
          /**
            Pause-E option
          **/
          if (MsgStreamEnter)
            {
            if (thisRoom == MAILROOM)
              {
              ShowReply(i);
              MsgStreamEnter = FALSE;
              Showing = MSGS;
              outFlag = OUTOK;  /* so we can Pause later */

              }
            else
              {
              PEUsed = TRUE;
              if (InterruptMessage()) i--;

              }
            continue;   /* skip the increment - reprint msg */

            }
          if (
          Showing == MSGS         && outFlag != OUTSKIP  /* so we can <S>top Mail */
          && !pulled              && thisRoom  == MAILROOM
          && whichMess == NEWoNLY && canRespond()
          && (strCmpU(msgBuf.mbauth, logBuf.lbname) != SAMESTRING
              || msgBuf.mborig[0] != 0)  /* i.e. is not local mail */
          &&  strCmpU(msgBuf.mbauth, "Citadel") != SAMESTRING
          &&  msgBuf.mbauth[0] != 0   /* not anonymous mail> */
             )
            {
            if ((result = DoRespond()) != ERROR)
              {
              roomBuf.msg[i].rbmsgNo &= S_MSG_MASK;
              logBuf.lbMail[i].rbmsgNo &= S_MSG_MASK;
              if (result == TRUE)
                {
                if (replyMessage(msgNo,roomBuf.msg[i].rbmsgLoc))
                i--;
                if (whichIO != CONSOLE && thisRoom == MAILROOM)
                echo = CALLER;   /* Restore privacy. */
                outFlag = OUTOK;

                }

              }
            else
              {
              roomBuf.msg[i].rbmsgNo |= (~S_MSG_MASK);
              logBuf.lbMail[i].rbmsgNo |= (~S_MSG_MASK);

              }

            }

          }

        }

      }
    i += increment;

    }
  echo = BOTH;
  Showing = WHATEVER;
  if (heldMess && PEUsed)
    {
    givePrompt();
    mPrintf("Current Held Message\n ");
    hldMessage(FALSE);

    }
  return MsgCount;

  }
/*
* InterruptMessage()
*
* This handles Pause-Enter.
*/
char InterruptMessage()
  {
  char toReturn = FALSE;
  Showing = WHATEVER;
  if (heldMess)
    {
    if (hldMessage(FALSE)) toReturn = TRUE;

    }
  else
    {
    if (makeMessage(ASCII)) toReturn = TRUE;

    }
  MsgStreamEnter = FALSE;
  Showing = MSGS;
  outFlag = OUTOK;      /* so we can Pause later */
  return toReturn;

  }
/*
* ShowReply()
*
* This is the backlink tracer of a mail message.
*/
void ShowReply(int i)   /* i is index into roomBuf.msgs */
  {
  char *ptr, doit = FALSE;
  MSG_NUMBER msg;
  label author, rec;

  /* format of mbreply is loc:msgNo */
  MsgStreamEnter = FALSE;
  outFlag = OUTOK;
  /* make sure there's a return ptr */
  if (strLen(msgBuf.mbreply) != 0 &&
  (ptr = strchr(msgBuf.mbreply, ':')) != NULL)
    {
    msg = atol(ptr + 1);
    doit = findMessage(atoi(msgBuf.mbreply), msg, TRUE);
    for (; i >= 0; i--)
    if (msg == (roomBuf.msg[i].rbmsgNo & S_MSG_MASK)) break;

    }
  else
    {
    /* else do a manual scan */
    strncpy(author, msgBuf.mbauth, sizeof(author)-1);
    strCpy(rec, msgBuf.mbto);
    for (--i; i >= 0; i--)
      {
      doit = findMessage(roomBuf.msg[i].rbmsgLoc,
      roomBuf.msg[i].rbmsgNo & S_MSG_MASK, TRUE);
      if (!doit) i = 0;
      else if (strCmpU(author, msgBuf.mbto) == SAMESTRING &&
      strCmpU(rec, msgBuf.mbauth) == SAMESTRING) break;

      }

    }
  /* this will allow streaming along on the message chain */
  if (doit)
    {
    printMessage(0);
    if (MsgStreamEnter)
    ShowReply(i);       /* fix this argument someday */

    }

  }
/*
* SetShowLimits()
*
* Sets up the limits for showing messages.
*/
void SetShowLimits(char rev, int *start, int *finish, int *increment)
  {
  /* Allow for reverse retrieval: */
  if (!rev)
    {
    *start      = 0;
    *finish     = (thisRoom == MAILROOM) ? MAILSLOTS : MSGSPERRM;
    *increment   = 1;

    }
  else
    {
    *start      = (((thisRoom == MAILROOM) ? MAILSLOTS : MSGSPERRM) -1);
    *finish     = -1;
    *increment   = -1;

    }

  }
/*
* redirect()
*
* This function causes output to be redirected to a file.
*/
char redirect(char *name)
  {
  extern char *APPEND_TEXT;
  char fullFileName[100];
  if (name != NULL) strCpy(fullFileName, name);
  else fullFileName[0] = 0;
  if (strLen(fullFileName) != 0 || getXString("EFILEN", fullFileName, 100,
  (strLen(jrnlFile) == 0) ? NULL : jrnlFile, jrnlFile))
    {
    if ((upfd = safeopen(fullFileName, APPEND_TEXT)) == NULL)
      {
      if (inNet == NON_NET)
      mPrintf("ERROR: Couldn't open output file %s\n ", fullFileName);

      }
    else
      {
      outPut = DISK;
      if (name == NULL) strCpy(jrnlFile, fullFileName);
      return TRUE;

      }

    }
  return FALSE;

  }
/*
* undirect()
*
* This makes output go back to normal.
*/
void undirect()
  {
  fclose(upfd);
  outPut = NORMAL;

  }
#define UnknownRoute "\n Couldn't identify route (%s).", msgBuf.mbaddr
/*
* TellRoute()
*
* This will figure out where this message came from.
*/
void TellRoute()
  {
  extern char *LOC_NET, *NON_LOC_NET;
  int slot;
  if (strCmp(msgBuf.mboname, cfg.codeBuf + cfg.nodeName) != SAMESTRING &&
  strLen(msgBuf.mbaddr) != 0)
    {
    if (((slot = RoutePath(NON_LOC_NET, msgBuf.mbaddr)) != ERROR ||
    (slot = RoutePath(LOC_NET, msgBuf.mbaddr)) != ERROR) &&
    slot >= 0 && slot < cfg.netSize)
      {
      if (slot != thisNet)
      getNet(slot, &netBuf);
      if (netBuf.nbflags.in_use)
      mPrintf("\n Routed from %s.", netBuf.netName);
      else
      mPrintf(UnknownRoute);

      }
    else
    mPrintf(UnknownRoute);

    }

  }
/*
* FindNextFile()
*
* This finds next route filename in sequence.
*/
int FindNextFile(char *base)
  {
  int rover = 0;
  char *fn;
  if (cfg.BoolFlags.debug) splitF(netLog," FindNextFile(%s)",base);
  fn = GetDynamic(strLen(base) + 5);
  /* Find next unused file name */
  do
    {
    sPrintf(fn, "%s.%d", base, rover++);

    }
  while (access(fn, 0) != -1);
  strCpy(base, fn);
  free(fn);
  rover--;
  if (cfg.BoolFlags.debug) splitF(netLog,"=%d\n",rover);
  return rover;

  }
