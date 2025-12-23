/*
*       rooma.c
*
* room code for Citadel bulletin board system.
*/
/*
*       history
*
* SEE THE INCREM.* FILES FOR FURTHER HISTORICAL NOTES
* 84Jul12 JLS & HAW  gotoRoom() and dumpRoom() modified for <S>kip.
* 84Apr04 HAW  Start 1.50a update
* 83Feb24      Insert check for insufficient RAM, externs too low.
* 82Dec06 CrT  2.00 release.
* 82Nov05 CrT  main() splits off to become citadel.c
*/
#include "ctdl.h"
#include "math.h"
#include "dos.h"
/*
*       Contents
*
* CheckForSkippedMsgs() check for skipped msgs (Mail)
* CountMsgs()   counts the messages in the room.
* DateSearch()    analyzes for date specs
* dumpRoom()    tells us # new messages etc
* fillMailRoom()    set up Mail> from log record
* gotoRoom()    handles "g(oto)" command for menu
* GotoNamedRoom()   goto the named room, if possible.
* initCitadel()   system startup initialization
* KnownRoom()   is room known?
* knowRoom()    does some user know of specified room?
* legalMatch()    Looks for partial matches.
* listRooms()   lists known rooms
* partialExist()    partial roomname matcher.
* retRoom()   handle Ungoto command
* roomCheck()   returns slot# of named room else ERROR
* roomExists()    returns slot# of named room else ERROR
* searchRooms()   searches room list for matching string
* setUp()     setup the log buffer vars correctly
* SkippedNewRoom()  worker function
* systat()    shows current system status
* tableRunner()   Applies some function to all the rooms.
* UngotoMaintain()  Maintains the Ungoto list.
*/
char        *baseRoom, BadMessages[40];
int       UngotoStack[UN_STACK];
char        remoteSysop = FALSE;      /* Is current user a sysop      */
char        shownHidden;
SListBase     Moderators =
  {
  NULL, ChkNtoStr, NULL, FreeNtoStr, EatNMapStr

  };
int       *lPtrTab;     /* For .Ungoto    */
char        BpsSet = FALSE;
SListBase     BadWords =
  {
  NULL, FindIcky, NULL, NULL, EatIcky

  };
SListBase     BadPeople =
  {
  NULL, FindIckyPeople, NULL, NULL, EatIckyPeople

  };
extern CONFIG    cfg;   /* A buncha variables   */
extern LogTable  *logTab; /* RAM index of pippuls   */
extern MessageBuffer   msgBuf;
extern logBuffer logBuf;  /* Pippul buffer    */
extern logBuffer logTmp;  /* Pippul buffer    */
extern NetBuffer netBuf;
extern NetBuffer netTemp;
extern struct floor     *FloorTab;
extern FILE      *logfl;    /* log file descriptor    */
extern FILE      *netfl;    /* Net file   */
extern rTable    *roomTab;  /* RAM index of rooms   */
extern aRoom     roomBuf; /* room buffer    */
extern FILE      *roomfl; /* file descriptor for rooms    */
extern int       thisRoom;  /* room currently in roomBuf    */
extern char      loggedIn;  /* Are we logged in?    */
extern char      PrintBanner;
extern char      echo;    /* output flag    */
extern char      prevChar;  /* Last char out    */
extern char      onConsole; /* on console?    */
extern char      whichIO; /* where is the I/O?    */
extern int       thisSlot;  /* Current log slot   */
extern char      outFlag;
extern char      nextDay; /* System up before bailout?    */
extern char      heldMess;
extern label     oldTarget; /* Room to move messages to     */
extern char      ShowNew;
extern char      JustChecking;
/*
* DateSearch()
*
* This function analyzes for possible"[<|>] <date-spec>" in string.
*/
char *DateSearch(char *str, long *before, long *after)
  {
  long *which;
  while (*str && *str != '>' && *str != '<')
  str++;
  switch (*str)
    {
    case '>':
    which = after; break;
    case '<':
    which = before; break;
    default: return NULL;

    }
  /* part of our duties is to put EOS here */
  *str++ = 0;
  while (*str && *str == ' ') str++;
  if (*which == -1l)
    {
    /* if not set yet */
    if (strlen(str))
    ReadDate(str, which);
    else
    *which = logBuf.lblaston;

    }
  /* now we need to find the possible location of the next date spec */
  /* first, skip over current date. */
  while (*str && *str != ' ' && *str != '>' && *str != '<') str++;
  return str;

  }
/*
* dumpRoom()
*
* This will tell us # new messages etc.
*/
void dumpRoom(char ShowFloor)
  {
  extern char HasSkipped;
  int   count, newCount;
  CountMsgs(&count, &newCount);
  if (!loggedIn && thisRoom == MAILROOM)      /* Kludge for new users */
  newCount = count = 1;     /* So they see intro.   */
  if (ShowFloor)Output_Citadel_Message("FLOORN",(long)FloorTab[thisFloor].FlName,NULL,NULL);
  Output_Citadel_Message("NUMMSG", count, NULL, NULL);
  if (newCount > 0 && !PrintBanner)
  Output_Citadel_Message("NUMNEW", newCount, NULL, NULL);
  if (thisRoom == LOBBY)
    {
    HasSkipped = FALSE;
    if (tableRunner(NSRoomHasNew, TRUE) != ERROR)
    return ;
    if (tableRunner(RoomHasNew, TRUE) == ERROR)
    return ;
    if (HasSkipped)
      {
      if (FloorMode)
      FSkipped();
      else
        {
        Output_Citadel_Message("SKPDRM", NULL, NULL, NULL);
        ShowNew = TRUE;
        JustChecking = FALSE;
        tableRunner(SkippedNewRoom, TRUE);

        }

      }

    }

  }
/*
* CountMsgs()
*
* This function counts the messages in the room, total and new.
*/
void CountMsgs(int *count, int *newCount)
  {
  int i;
  MSG_NUMBER msgNo;
  for (*newCount = *count = i = 0;
  i < ((thisRoom == MAILROOM) ? MAILSLOTS : MSGSPERRM);   i++)
    {
    /* Msg is still in system?  Count it.   */
    msgNo = roomBuf.msg[i].rbmsgNo & S_MSG_MASK;
    if (msgNo >= cfg.oldest)
      {
      (*count)++;
      /* don't boggle -- just checking against newest as of */
      /* the last time we were  in this room - also against */
      /* msg skip bit.            */
      if ((msgNo > logBuf.lbvisit[ logBuf.lbgen[thisRoom] & CALLMASK ] &&
      msgNo <= cfg.newest) || msgNo != roomBuf.msg[i].rbmsgNo)
        {
        (*newCount)++;

        }

      }

    }

  }
/*
* SkippedNewRoom()
*
* This is used for calls to tableRunner(), clears room's SKIP flag, if
* ShowNew is TRUE it prints the room's name, else just return TRUE.
*/
int SkippedNewRoom(int i)
  {
  if (roomTab[i].rtflags.SKIP == 1 && RoomHasNew(i))
    {
    roomTab[i].rtflags.SKIP = 0;  /* Clear. */
    if (ShowNew) mPrintf(" %s ", formRoom(i, TRUE, TRUE));
    if (JustChecking) return TRUE;

    }
  return FALSE;

  }
/*
* fillMailRoom()
*
* This fills up the Mail room.
*/
void fillMailRoom()
  {
  memcpy(roomBuf.msg, logBuf.lbMail, (long)MAIL_BULK);
  noteRoom();

  }
/*
* gotoRoom()
*
* This is the menu fn to travel to a new room.
* returns TRUE if room is Lobby>, else FALSE.
*/
int gotoRoom(char *nam, char mode)
  {
  int  i, foundit, roomNo, s;
  int  lRoom, oldFloor;
  char NewFloor = FALSE;
  extern int ParanoiaCount;
  lRoom = thisRoom;
  if (strLen(nam) == 0)
    {
    foundit = FALSE;  /* leaves us in Lobby> if nothing found */
    if (mode != 'S')
      {
      SetKnown(-1, 0, thisRoom, &logBuf);
      roomTab[thisRoom].rtflags.SKIP = CheckForSkippedMsgs();

      }
    if (!FloorMode)
      {
      for (i = 0; i<MAXROOMS  &&  !foundit; i++)
        {
        s = knowRoom(&logBuf, i);
        if (
        (s == KNOW_ROOM || s == WRITE_PRIVS ||
        (s != DEAD_ROOM && aide && cfg.BoolFlags.aideSeeAll &&
        (!roomTab[i].rtflags.INVITE || SomeSysop())))
        &&
        !roomTab[i].rtflags.SKIP
        )
          {
          if (roomTab[i].rtlastMessage >
          logBuf.lbvisit[logBuf.lbgen[i] & CALLMASK] &&
          roomTab[i].rtlastMessage >= cfg.oldest)
            {
            if (i != thisRoom)
              {
              foundit  = i;

              }

            }

          }

        }
      getRoom(foundit);
      mPrintf("%s\n ", roomBuf.rbname);

      }
    else
      {
      NewFloor = NewRoom();
      foundit = thisRoom;

      }
    UngotoMaintain(lRoom);

    }
  else
    {
    foundit = 0;
    oldFloor = thisFloor;
    if ((roomNo = GotoNamedRoom(nam, mode)) == ERROR)
        Output_Citadel_Message("NOROOM", (long)nam, NULL, NULL);
    else
      {
      foundit = roomNo;
      if (FloorMode) NewFloor = !(oldFloor == thisFloor);

      }

    }
  setUp(FALSE);
  dumpRoom(NewFloor);
  /* in case recover1 gets a room back for a non-existent floor */
  if (!FloorTab[roomBuf.rbFlIndex].FlInuse)
  roomBuf.rbFlIndex = 0;
  if (thisRoom != lRoom) ParanoiaCount = 0;
  return foundit;

  }
/*
* GotoNamedRoom()
*
* This function will goto the named room, if possible.
*/
int GotoNamedRoom(char *name, char mode)
  {
  int roomNo;
  if ((roomNo = RealGNR(name, roomExists)) == ERROR &&
  (roomNo = RealGNR(name, partialExist)) == ERROR)
  return ERROR;
  if (roomNo != thisRoom)
    {
    if (mode != 'S')
      {
      SetKnown(-1, 0, thisRoom, &logBuf);
      roomTab[thisRoom].rtflags.SKIP = CheckForSkippedMsgs();

      }
    UngotoMaintain(thisRoom);
    getRoom(roomNo);
    /* if may have been unknown... if so, note it:      */
    if (!KnownRoom(thisRoom))
      {
      SetKnown(0, MAXVISIT - 1, thisRoom, &logBuf);

      }

    }
  return roomNo;

  }
/*
* RealGNR()
*
* This function does the real work of checking to see if a .Goto is legal.
*/
int RealGNR(char *nam, int (*func)(char *room))
  {
  int roomNo;
  /* non-empty room name, so now we look for it: */
  if ((roomNo = roomCheck(func, nam)) == ERROR ||
  roomTab[roomNo].rtflags.INVITE && !SomeSysop() &&
  roomTab[roomNo].rtgen != (logBuf.lbgen[roomNo] >> GENSHIFT) &&
  abs(roomTab[roomNo].rtgen - (logBuf.lbgen[roomNo] >> GENSHIFT))
  != RO_OFFSET)
    {
    return ERROR;

    }
  else
    {
    return roomNo;

    }

  }
/*
* initCitadel()
*
* This initializes system, returns TRUE if system is coming up normally,
* false if returning from a door call.
*/
char initCitadel()
  {
  SYS_FILE    tempName;
  extern char ExitToMsdos;
  extern long byteRate;
  extern char MeetDisabled;
  extern char *READ_TEXT, *VERSION, *SysVers;
  int   SysVal;
  char  fromDoor;
  extern SListBase Arch_base, MailForward;
  extern char justLostCarrier;
  extern FILE *upfd;
  extern int  IckyLevel;
  echo = BOTH;
  if (!readSysTab(TRUE, TRUE)) exit(CRASH_EXIT);/* No system table? Tacky, tacky*/
  cfg.weAre = CITADEL;
  if ((SysVal = systemInit()) != 0)
    {
    writeSysTab();
    systemShutdown(SysVal);
    exit(CRASH_EXIT);

    }
  Output_Citadel_Message("VERSIN",NULL,NULL,NULL);
  printf("This software is Public Domain, not Commercial and not Shareware.\n\n");
  printf("IF YOU PAID FOR THIS SOFTWARE, SOMEONE IS RIPPING YOU OFF.\n\n");
  if (access(LOCKFILE, 0) != -1)
    {
    printf("Lock File found!  Do you have Citadel already up?\n");
    writeSysTab();  /* Save it out just in case */
    systemShutdown(0);
    exit(RECURSE_EXIT);

    }
  SpecialMessage("Opening files         ");
  /* open message files: */
  InitMsgBase();
  InitEvents();
  initLogBuf(&logBuf);
  initLogBuf(&logTmp);
  initRoomBuf(&roomBuf);
  initNetBuf(&netBuf);
  initNetBuf(&netTemp);
  initTransfers();
  ReadCitInfo();
  lPtrTab = (int *) GetDynamic(MAXROOMS * sizeof (int));
  strCpy(oldTarget, "Aide");
  baseRoom = &cfg.codeBuf[cfg.bRoom];
  setUp(TRUE);
  makeSysName(tempName, "CtdlLog.SYS",  &cfg.logArea);
  openFile(tempName, &logfl );
  makeSysName(tempName, "CtdlRoom.SYS", &cfg.roomArea);
  openFile(tempName, &roomfl);
  makeSysName(tempName, "CtdlArch.SYS", &cfg.roomArea);
  MakeList(&Arch_base, tempName, NULL);
  /**
    Icky level is the first line of Badwords, filename is second line
  **/
  makeSysName(tempName, "BadWords.SYS", &cfg.roomArea);
  if ((upfd = safeopen(tempName, READ_TEXT)) != NULL)
    {
    if (GetAString(msgBuf.mbtext, MAXTEXT, upfd) != NULL)
    IckyLevel = atoi(msgBuf.mbtext);
    if (GetAString(msgBuf.mbtext, MAXTEXT, upfd) != NULL)
    strCpy(BadMessages, msgBuf.mbtext);
    MakeList(&BadWords, "", upfd);
    fclose(upfd);
    };
  /**
    Icky Stuff... BadPeople.sys
  **/
  makeSysName(tempName, "BadPeople.SYS", &cfg.roomArea);
  if ((upfd = safeopen(tempName, READ_TEXT)) != NULL)
    {
    MakeList(&BadPeople, "", upfd);
    fclose(upfd);
    };

  makeSysName(tempName, "CtdlModr.SYS", &cfg.roomArea);
  MakeList(&Moderators, tempName, NULL);
  if (cfg.BoolFlags.netParticipant)
    {
    makeSysName(tempName, "CtdlNet.SYS", &cfg.netArea);
    openFile(tempName, &netfl);
    NetInit();
    OpenForwarding();

    }
  getRoom(LOBBY);     /* load Lobby>  */
  SpecialMessage("Loading Lobby...     ");
  fromDoor = BackFromDoor();
  if (cfg.BoolFlags.IsDoor && !fromDoor && !BpsSet)
    {
    printf("This is a Door C-68K.\n");
    writeSysTab();
    exit(RECURSE_EXIT);

    }
  /* Now open the modem up */
  SpecialMessage("Modem Initialization...     ");
  ModemOpen((gotCarrier() && fromDoor) || cfg.BoolFlags.IsDoor);
  ExitToMsdos = !ModemSetup((fromDoor ||
  cfg.BoolFlags.IsDoor) && byteRate != 0);
  if (!cfg.BoolFlags.IsDoor) ExitToMsdos = FALSE;
  if ((fromDoor || BpsSet) && byteRate == 0)
    {
    DisableModem(FALSE);
    whichIO = CONSOLE;

    }
  else
  whichIO = MODEM;
  setUp(FALSE);
  #ifndef MAJOR_RELEASE
  if (!MeetDisabled)
    {
    InitBio();

    }
  #endif
  if (fromDoor && byteRate != 0 && !gotCarrier())
  justLostCarrier = TRUE;
  /* display a banner.      */
  SpecialMessage("Initialization completed....");
  return (char)!fromDoor;   /* if we come back from a door, don't   */
  }
/*
* legalMatch()
*
* This looks for partial matches, checks legalities.
*/
char legalMatch(int i, label target)
  {
  char  Equal, *endbuf;
  Equal = KnownRoom(i);
  if ((roomTab[i].rtflags.INUSE == 1) &&
  ((aide && cfg.BoolFlags.aideSeeAll &&
  !roomTab[i].rtflags.INVITE)
  || Equal))
    {
    endbuf = lbyte(roomTab[i].rtname);
    return (char)(matchString(roomTab[i].rtname, target, endbuf) != NULL);

    }
  return FALSE;

  }
/*
* listRooms()
*
* This function lists known rooms.
*/
void listRooms(char mode)
  {
  extern char SelDirs, SelShared, SelPriv, SelNew, Sel, SelAnon, NotForgotten;
  extern char SelRO, SelArch;
  shownHidden = FALSE;
  switch (mode)
    {
    case DR_SEL:   SelDirs   = TRUE; break;
    case SH_SEL:   SelShared = TRUE; break;
    case PR_SEL:   SelPriv   = TRUE; break;
    case ARCH_SEL: SelArch   = TRUE; break;
    case ANON_SEL: SelAnon   = TRUE; break;
    case READONLY: SelRO     = TRUE; break;
    case INT_EXPERT:
    case INT_NOVICE:
    case NOT_INTRO:
    SelNew = TRUE; break;
    case FORGOTTEN:
    NotForgotten = FALSE;
    SelNew = TRUE;
    break;

    }
  if (FloorMode)
    {
    FKnown(mode);

    }
  else
    {
    /* Else */
    if (SelNew && NotForgotten)
      {
      Output_Citadel_Message("RMUNRD", NULL, NULL, NULL);
      ShowNew = 1;

      }
    else if (mode == FORGOTTEN)
      {
      Output_Citadel_Message("FORGRM", NULL, NULL, NULL);
      ShowNew = 2;

      }
    tableRunner(DispRoom, TRUE);
    if (mode != INT_EXPERT && SelNew && NotForgotten)
      {
      Output_Citadel_Message("NOURMS", NULL, NULL, NULL);
      ShowNew = FALSE;
      tableRunner(DispRoom, TRUE);

      }

    }
  SelArch = SelDirs = SelShared = SelRO = SelPriv = SelNew = SelAnon = FALSE;
  NotForgotten = TRUE;

  }
/*
* tableRunner()
*
* This applies some function to all the rooms the user might know of.
*
* OnlyKnown: decides if every room is subject to the function call, or only
* those the current user knows of.
*/
int tableRunner(int (*func)(int rover), char OnlyKnown)
  {
  int rover;
  for (rover = 0; rover < MAXROOMS; rover++)
    {
    if (!OnlyKnown || KnownRoom(rover))
    if ((*func)(rover)) return rover;

    }
  return ERROR;

  }
/*
* KnownRoom()
*
* This is called by tableRunner, returns whether room is known.  External flag
* NotForgotten controls if we're doing a normal Known rooms or a list of
* ZForgotten rooms.
*/
int KnownRoom(int RoomNo)
  {
  extern char NotForgotten;
  int s;
  s = knowRoom(&logBuf, RoomNo);
  if (NotForgotten)
    {
    return (s == KNOW_ROOM || s == WRITE_PRIVS);

    }
  /* now checking for Forgotten rooms -- don't show if private room! */
  if (!roomTab[RoomNo].rtflags.PUBLIC) return FALSE;
  return (s == FORGOTTEN_ROOM);

  }
/*
* knowRoom()
*
* This will check to see if specified user knows given room.
*
* Return 0 if not know room, 2 if forgot room, 3 if know room and have write
* permission (r/o rooms), 1 otherwise.
*/
char knowRoom(logBuffer *lBuf, int i)
  {
  int difference;
  if (!roomTab[i].rtflags.INUSE) return DEAD_ROOM;
  difference = abs(roomTab[i].rtgen - (lBuf->lbgen[i] >> GENSHIFT));
  return (char)( ((difference == 0) ? KNOW_ROOM :
  (difference == RO_OFFSET) ? WRITE_PRIVS :
  ((difference == FORGET_OFFSET) ? FORGOTTEN_ROOM :
  (roomTab[i].rtflags.PUBLIC) ? KNOW_ROOM : UNKNOWN_ROOM)) );

  }
/*
* SetKnown()
*
* This sets up a known-room value.
*/
void SetKnown(int GenVal, int Index, int Room, logBuffer *lBuf)
  {
  int val;
  switch (GenVal)
    {
    case -2: val = (roomTab[Room].rtgen + (MAXGEN-1)) % MAXGEN;      break;
    case -1: val = (lBuf->lbgen[Room] >> GENSHIFT);                  break;
    case 0:  val = roomTab[Room].rtgen;                              break;
    case RO_OFFSET:val = (roomTab[Room].rtgen + RO_OFFSET) % MAXGEN; break;
    default:
      val = (roomTab[Room].rtgen + FORGET_OFFSET) % MAXGEN;
    };
  lBuf->lbgen[Room] = (val << GENSHIFT) + Index;

  }
/*
* partialExist()
*
* This roams the list looking for a partial match.
*/
int partialExist(label target)
  {
  int rover;
  for (rover = (thisRoom + 1) % MAXROOMS; rover != thisRoom;
  rover = (rover + 1) % MAXROOMS)
  if (legalMatch(rover, target)) return rover;
  return ERROR;

  }
/*
* retRoom()
*
* This is the menu Ungoto command.
*/
void retRoom(char *roomName)
  {
  int slot, OldFloor;
  OldFloor = thisFloor;
  if (strLen(roomName) == 0)
    {
    if (UngotoStack[0] == -1)
      {
      Output_Citadel_Message("NORMUN", NULL, NULL, NULL);
      return;

      }
    getRoom(UngotoStack[0]);
    mPrintf("%s\n ", roomBuf.rbname);
    logBuf.lbgen[thisRoom] = lPtrTab[thisRoom];
    /* Now pop that top element off the stack */
    memmove(UngotoStack, UngotoStack + 1, (UN_STACK - 1) * sizeof(int));
    UngotoStack[UN_STACK-1] = -1;   /* bottom of stack */

    }
  else
    {
    if (
    (slot = RealGNR(roomName, roomExists)) == ERROR &&
    (slot = RealGNR(roomName, partialExist)) == ERROR
    )
      {
      Output_Citadel_Message("NOROOM", (long)roomName, NULL, NULL);
      return;

      }
    UngotoMaintain(thisRoom);
    getRoom(slot);
    logBuf.lbgen[thisRoom] = lPtrTab[thisRoom];

    }
  setUp(FALSE);
  dumpRoom((char) FloorMode ? !(OldFloor == thisFloor) : FALSE);

  }
/*
* roomCheck()
*
* This returns slot# of named room else ERROR as determined by checker.
*/
int roomCheck(int (*checker)(char *name), char *nam)
  {
  int roomNo;
  if (
  (roomNo = (*checker)(nam)) == ERROR
  ||
  (roomNo==AIDEROOM  &&  !aide)
  ||
  (roomTab[roomNo].rtflags.PUBLIC == 0 && !loggedIn)
  )
  return ERROR;
  return roomNo;

  }
/*
* roomExists()
*
* This returns slot# of named room else ERROR.
*/
int roomExists(char *room)
  {
  int i;
  for (i = 0;  i < MAXROOMS;  i++)
    {
    if (
    roomTab[i].rtflags.INUSE == 1   &&
    strCmpU(room, roomTab[i].rtname) == SAMESTRING
    )
      {
      return(i);

      }

    }
  return(ERROR);

  }
/*
* searchRooms()
*
* This function searches for user string in list of rooms.
*/
void searchRooms(char *target)
  {
  int   i;
  Output_Citadel_Message("MATCHR", NULL, NULL, NULL);
  outFlag = OUTOK;
  for (i = 0; i < MAXROOMS;  i++)
    {
    if (legalMatch(i, target))
      {
      mPrintf(" %s ", formRoom(i, TRUE, TRUE));

      }

    }

  }
/*
* setUp()
*
* This does the initial setup based on who's logged on.
*/
void setUp(char justIn)
  {
  int   g, i, j, ourSlot;
  extern long DoorsUsed;
  extern int  AnonMsgCount;
  extern int  IckyCount;
  echo = BOTH;  /* just in case */
  if (justIn)
    {
    for (i = 0; i < UN_STACK; i++)
    UngotoStack[i] = -1;
    heldMess = FALSE;
    IckyCount = 0;

    }
  if (!loggedIn)
    {
    remoteSysop = FALSE;
    prevChar  = ' ';
    termWidth = cfg.InitColumns;
    termLF    = TRUE;
    termNulls = 5;
    expert    = FALSE;
    aide    = FALSE;
    sendTime  = TRUE;
    oldToo    = FALSE;
    HalfDup   = FALSE;
    FloorMode = FALSE;
    DoorPriv  = FALSE;
    if (justIn)
      {
      /* set up logBuf so everything is new...  */
      AnonMsgCount = 0;
      for (i = 0; i < MAXVISIT;  i++)  logBuf.lbvisit[i] = cfg.oldest-1l;
      /* no mail for anonymous folks: */
      roomTab[MAILROOM].rtlastMessage = cfg.newest;
      for (i = 0; i < MAILSLOTS;  i++)
      logBuf.lbMail[i].rbmsgNo = 0l;
      logBuf.lbname[0] = 0;
      for (i = 0; i < MAXROOMS;  i++)
        {
        if (roomTab[i].rtflags.PUBLIC)
          {
          /* make public rooms known: */
          g       = roomTab[i].rtgen;
          logBuf.lbgen[i] = (g << GENSHIFT) + (MAXVISIT-1);

          }
        else
          {
          /* make private rooms unknown: */
          g       = (roomTab[i].rtgen + (MAXGEN-1)) % MAXGEN;
          logBuf.lbgen[i] = (g << GENSHIFT) + (MAXVISIT-1);

          }
        lPtrTab[i]  = logBuf.lbgen[i];

        }

      }

    }
  else
    {
    /* loggedIn: */
    if (justIn)
      {
      DoorsUsed = 0l;
      remoteSysop = FALSE;
      /* set gen on all unknown rooms  --  INUSE or no: */
      for (i = 0;  i < MAXROOMS;  i++)
        {
        j = abs(roomTab[i].rtgen - (logBuf.lbgen[i] >> GENSHIFT));
        if (!roomTab[i].rtflags.PUBLIC && !roomTab[i].rtflags.PUBLIC)
          {
          /* it is private -- is it unknown? */
          if (j != 0 && aide)
            {
            if (SomeSysop() || (cfg.BoolFlags.aideSeeAll &&
            (!roomTab[i].rtflags.INVITE || SomeSysop())))
            SetKnown(0, MAXVISIT - 1, i, &logBuf);

            }
          else if ((j != 0 && j != RO_OFFSET) ||
          (!aide && i == AIDEROOM)
          )
            {
            /* yes -- set   gen = (realgen-1) % MAXGEN */
            /* j = (roomTab[i].rtgen + (MAXGEN-1)) % MAXGEN; */
            SetKnown(-2, MAXVISIT - 1, i, &logBuf);

            }

          }
        else if ((logBuf.lbgen[i] >> GENSHIFT) != roomTab[i].rtgen)
          {
          /* newly created public room -- remember to visit it; */
          j = roomTab[i].rtgen - (logBuf.lbgen[i] >> GENSHIFT);
          if (j < 0)
          g = -j;
          else
          g = j;
          if (g != FORGET_OFFSET && g != RO_OFFSET)
            {
            SetKnown(0, 1, i, &logBuf);

            }

          }

        }
      /* special kludge for Mail> room, to signal new mail:   */
      roomTab[MAILROOM].rtlastMessage = 0l;
      for (i = 0;  i < MAILSLOTS;  i++)
      if ((logBuf.lbMail[i].rbmsgNo & (~S_MSG_MASK)) &&
      (logBuf.lbMail[i].rbmsgNo & S_MSG_MASK) > cfg.oldest)
      roomTab[MAILROOM].rtlastMessage = S_MSG_MASK;
      if (roomTab[MAILROOM].rtlastMessage != S_MSG_MASK)
      roomTab[MAILROOM].rtlastMessage =
      (logBuf.lbMail[MAILSLOTS-1].rbmsgNo & S_MSG_MASK);
      /* slide lbvisit array down and change lbgen entries to match: */
      for (i = (MAXVISIT - 2);  i;  i--)
        {
        logBuf.lbvisit[i] = logBuf.lbvisit[i-1];

        }
      logBuf.lbvisit[(MAXVISIT - 1)]    = cfg.oldest;
      for (i = 0;  i < MAXROOMS;  i++)
        {
        if ((logBuf.lbgen[i] & CALLMASK)  <  (MAXVISIT-2))
          {
          logBuf.lbgen[i]++;

          }
        lPtrTab[i]  = logBuf.lbgen[i];

        }
      /* Slide entry to top of log table: */
      ourSlot = logTab[thisSlot].ltlogSlot;
      slideLTab(0, thisSlot);
      logTab[0].ltpwhash      = hash(logBuf.lbpw);
      logTab[0].ltnmhash      = hash(logBuf.lbname);
      logTab[0].ltlogSlot     = ourSlot;
      logTab[0].ltnewest      = cfg.newest;

      }

    }
  logBuf.lbvisit[0]   = cfg.newest;
  onConsole   = (whichIO == CONSOLE);
  if (thisRoom == MAILROOM)   fillMailRoom();

  }
/*
* CheckForSkippedMsgs()
*
* This function does a check for skipped msgs (Mail).
*/
char CheckForSkippedMsgs()
  {
  int i;
  for (i = 0;  i < ((thisRoom == MAILROOM) ? MAILSLOTS : MSGSPERRM);  i++)
  if ((roomBuf.msg[i].rbmsgNo & (~S_MSG_MASK)) &&
  (roomBuf.msg[i].rbmsgNo & S_MSG_MASK) >= cfg.oldest)
    {
    roomTab[thisRoom].rtlastMessage = S_MSG_MASK;
    return TRUE;

    }
  noteRoom();   /* just in case */
  return FALSE;

  }
/*
* systat()
*
* This function prints out current system status (.rs).
*/
void systat()
  {
  extern char *VERSION, *SysVers;
  int   i;
  long temp;
  extern long total_char_in, total_char_out;
  char  buffer1[15];
  char  buffer2[15];
  MSG_NUMBER  average, work;
  int   roomCount;
  for (roomCount = i = 0; i < MAXROOMS; i++)if (roomTab[i].rtflags.INUSE) roomCount++;
  Output_Citadel_Message("NODETL", NULL, NULL, NULL);
  Output_Citadel_Message("DTSTMP",NULL, NULL, NULL );
  if (loggedIn)
    {
    Output_Citadel_Message("WELCOM", NULL, NULL, NULL);
    if (logBuf.lbflags.NET_PRIVS)
      {
      Output_Citadel_Message("NETCRD", (long)logBuf.credit, NULL, NULL);
      };
    mPrintf("\n ");

    };
  temp = cfg.newest - cfg.oldest;
  if( temp < 0 ) temp = -temp;
  temp++;
  Output_Citadel_Message("MSGSNO"
  ,(long) PrintPretty(temp, buffer1)
  ,(long) PrintPretty(cfg.newest, buffer2)
  ,(long) cfg.maxMSector / (1024 / MSG_SECT_SIZE)  );
  Output_Citadel_Message("LOGSLR",(long)cfg.MAXLOGTAB, MAXROOMS, roomCount);
  Output_Citadel_Message("NETDAT",total_char_in, total_char_out, NULL);
  if (cfg.oldest > 1)
  work = cfg.maxMSector;
  else
  work = cfg.catSector;
  work *= MSG_SECT_SIZE;
  average = work / temp;
  Output_Citadel_Message("AVERML",average, NULL, NULL);
  Output_Citadel_Message("CCPRIV",NULL, NULL, NULL);
  }
/*
* UngotoMaintain()
*
* This function maintains the Ungoto list.
*/
void UngotoMaintain(int lRoom)
  {
  /* Move stack down 1 element */
  memmove(UngotoStack + 1, UngotoStack, (UN_STACK - 1) * sizeof(int));
  /* Add new element */
  UngotoStack[0] = lRoom;

  }
