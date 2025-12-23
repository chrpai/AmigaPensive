/*
*       Floors.c
*
* Floor handling code for Citadel-86.
*/
#include "ctdl.h"
/*
*       Contents
*/
char SelDirs = FALSE,
SelShared = FALSE,
SelPriv = FALSE,
SelRO = FALSE,
SelNew = FALSE,
SelAnon = FALSE;
char ShowNew;
char NotForgotten = TRUE;
char JustChecking;
char HasSkipped;
static char maim;
char FoundNew;
static int  GlobalFloor;
static int  MoveCount;
extern struct floor  *FloorTab;
extern rTable    *roomTab;
extern CONFIG    cfg;
extern aRoom     roomBuf;
extern logBuffer logBuf;
extern MessageBuffer   msgBuf;
extern char      remoteSysop;
extern int       thisRoom;
extern char      loggedIn;       /* Are we logged in?            */
extern int       TopFloor;
extern char      *confirm;
extern char      outFlag;
extern char      onConsole;
extern char      *baseRoom;
static int FlagCheck(int i);
int FindFloor(label name, char doPartial);
/*
* FloorRunner()
*
* This function runs through all the rooms on a floor, applying the given
* function to all rooms on that floor.
*/
int FloorRunner(int start, int (*func)(int r))
  {
  int rover, CurrentFloor;
  if (start == ERROR) return ERROR;
  CurrentFloor = roomTab[start].rtFlIndex;
  if (!FloorTab[CurrentFloor].FlInuse)
  CurrentFloor = 0;
  /* start with current room, go through table */
  for (rover = 0; rover < MAXROOMS; rover++)
    {
    /* deep breath ... should rewrite this, prime example of
    programming via accretion. */
    if (!FloorTab[roomTab[rover].rtFlIndex].FlInuse)
    roomTab[rover].rtFlIndex = 0;
    if ( roomTab[rover].rtFlIndex == CurrentFloor && (KnownRoom(rover) ||
    (NotForgotten && roomTab[rover].rtflags.INUSE &&
    (aide && (cfg.BoolFlags.aideSeeAll || onConsole) &&
    (!roomTab[rover].rtflags.INVITE || SomeSysop())))))
      {
      if ((*func)(rover)) return rover;

      }

    }
  return ERROR;

  }
/*
* NewRoom()
*
* This function gets next new room in system (like GotoRoom()) on the current
* floor.
*/
int NewRoom()
  {
  int CurrentFloor, OldFloor, roomNo;
  OldFloor = thisFloor;
  CurrentFloor = -1;
  while (   (roomNo = FloorRunner(thisRoom, NSRoomHasNew)) == ERROR &&
  CurrentFloor != TopFloor     )
    {
    thisRoom = FirstRoom(++CurrentFloor);

    }
  if (CurrentFloor == TopFloor)
  roomNo = 0;           /* no new-message rooms found */
  getRoom(roomNo);
  mPrintf("%s\n ", roomBuf.rbname);
  /* really ugly kludge.  someday locate this gunk in gotoRoom() */
  if (!KnownRoom(thisRoom))
    {
    SetKnown(0, MAXVISIT - 1, thisRoom, &logBuf);

    }
  return !(OldFloor == roomTab[thisRoom].rtFlIndex);

  }
/*
* FirstRoom()
*
* This function gets the first room on the specified floor.
*/
int FirstRoom(int FloorNo)
  {
  int rover;
  for (rover = 0; rover < MAXROOMS; rover++)
    {
    if (     roomTab[rover].rtflags.INUSE &&
    roomTab[rover].rtFlIndex == FloorNo    )
    return rover;

    }
  return ERROR;

  }
/*
* DoFloor()
*
* This function handles the fanout for floor commands.
*/
int DoFloors()
  {
  int  toReturn;
  char again, c[2];
  char *FloorOpts[] =
    {
    TERM "Aide", TERM "Z\bForget", TERM "Skip",
    TERM "Goto", TERM "Known Floors", TERM "C\b", ""

    };
  SListBase  FSelects =
    {
    NULL, FindSelect, NULL, NoFree, NULL

    };
  do
    {
    toReturn = GOOD_SELECT;
    if (CmdMenuList(FloorOpts, &FSelects, "floor.mnu", c, TRUE, FALSE)
    == BACKED_OUT) return GOOD_SELECT;
    again = FALSE;
    switch (c[0])
      {
      case 'A': toReturn = FAide();     break;
      case 'Z': toReturn = FForget();     break;
      case 'S':
      case 'G': toReturn = FGotoSkip(c[0]);   break;
      case 'K': toReturn = FKnown(ONLY_FLOORS);   break;
      case 'C': toReturn = FConfigure();      break;
      default:
      ;

      }
    if (toReturn == BACKED_OUT)
      {
      again = TRUE;
      PushBack('\b');

      }

    }
  while (again);
  return toReturn;

  }
/*
* FInvite()
*
* This function is used to invite a user to all rooms on a floor.
*/
void FInvite()
  {
  mPrintf("[%s]\n ", FloorTab[thisFloor].FlName);
  getList(UserToFloor, "Invited users", NAMESIZE, FALSE);

  }
static char YesInvite = TRUE;
/*
* FWithdraw()
*
* This function will withdraw invitations from a floor.
*/
void FWithdraw()
  {
  mPrintf("[%s]\n ", FloorTab[thisFloor].FlName);
  YesInvite = FALSE;
  getList(UserToFloor, "Kicked out users", NAMESIZE, FALSE);
  YesInvite = TRUE;

  }
extern logBuffer logTmp;
/*
* UserToFloor()
*
* This function will invite the named user to current floor.  This function
* is used in conjunction with getList().
*/
int UserToFloor(char *name)
  {
  int logNo;
  int InviteUserToFloor(int r);
  logNo = findPerson(name, &logTmp);
  if (logNo == ERROR)
    Output_Citadel_Message("NODRFD", (long)name,NULL,NULL);
  else
    {
    FloorRunner(thisRoom, InviteUserToFloor);
    putLog(&logTmp, logNo);

    }
  return TRUE;

  }
/*
* InviteUserToFloor()
*
* This function does the actual work of inviting a user.
*/
int InviteUserToFloor(int r)
  {
  if (r != AIDEROOM)
    {
    if (YesInvite)
    logTmp.lbgen[r] = (roomTab[r].rtgen << GENSHIFT) + MAXVISIT - 1;
    else
    logTmp.lbgen[r] = ((roomTab[r].rtgen + (MAXGEN-1)) % MAXGEN);

    }
  return FALSE;

  }
/*
* FSkipped()
*
* This function will show skipped rooms and floors.
*/
void FSkipped()
  {
  int rover, roomNo;
  JustChecking = FALSE;
  ShowNew = TRUE;
  Output_Citadel_Message("SKPRMF",(long) FloorTab[thisFloor].FlName , NULL, NULL);
  FloorRunner(thisRoom, SkippedNewRoom);
  ShowNew = FALSE;
  JustChecking = TRUE;
  Output_Citadel_Message("SKPFLR", NULL, NULL, NULL);
  for (rover = 1; rover < TopFloor; rover++)
    {
    roomNo = FirstRoom(rover);
    if (FloorRunner(roomNo, SkippedNewRoom) != ERROR)
      Output_Citadel_Message("FLRNNL",(long)FloorTab[rover].FlName , NULL, NULL);

    }
  JustChecking = FALSE;
  tableRunner(SkippedNewRoom, TRUE);

  }
/*
* FForget()
*
* This function will forget a floor.
*/
char FForget()
  {
  Output_Citadel_Message("FLOORN",(long)FloorTab[roomTab[thisRoom].rtFlIndex].FlName, NULL, NULL);
  if (!getYesNo("CONFRM")) return GOOD_SELECT;
  FloorRunner(thisRoom, Zroom);
  gotoRoom(baseRoom, 'S');
  return GOOD_SELECT;

  }
/*
* FConfigure()
*
* This function will change floor configuration value.
*/
char FConfigure()
  {
  FloorMode = !FloorMode;
  mPrintf("%s mode\n ", (FloorMode) ? "FLOOR" : "Normal");
  return GOOD_SELECT;

  }
/*
* FGotoSkip()
*
* This function will let us Skip an entire floor.
*/
char FGotoSkip(int mode)
  {
  label floorName;
  int   floorNo, newRoom, rover;
  char  dispbuf[40];
  outFlag = IMPERVIOUS;
  if (mode == 'S')
    {
    sPrintf(dispbuf, "[%s] goto ",
    FloorTab[roomTab[thisRoom].rtFlIndex].FlName);
    mPrintf("%s", dispbuf);

    }
  if (getNormStr("", floorName, NAMESIZE, BS_VALID) == BACKED_OUT)
    {
    if (mode == 'S')
    for (rover = 0; rover < strLen(dispbuf); rover++)
    mPrintf("\b \b");
    return BACKED_OUT;

    }
  if (strLen(floorName) != 0)
    {
    if ((floorNo = FindFloor(floorName, TRUE)) == ERROR)
      {
      Output_Citadel_Message("NODRFD", (long)floorName,NULL, NULL);
      return GOOD_SELECT;

      }
    if ((newRoom = FloorRunner(FirstRoom(floorNo), FindAny)) == ERROR)
      {
      Output_Citadel_Message("NOKNRM",(long) FloorTab[floorNo].FlName, NULL, NULL );
      return GOOD_SELECT;

      }

    }
  else
    {
    floorNo = thisFloor;
    for (rover = 0; rover < TopFloor; rover++)
    if (rover != floorNo && FloorTab[rover].FlInuse)
      {
      newRoom = FirstRoom(rover);
      if ((newRoom = FloorRunner(newRoom, NSRoomHasNew)) != ERROR)
      break;

      }
    if (rover == TopFloor)
    newRoom = 0;

    }
  if (mode == 'S') FloorRunner(thisRoom, FSroom);
  gotoRoom(roomTab[newRoom].rtname, mode);
  return GOOD_SELECT;

  }
/*
* FindFloor()
*
* This function returns index for the given floor name.
*/
int FindFloor(label name, char doPartial)
  {
  int rover;
  for (rover = 0; rover < TopFloor; rover++)
    {
    if (strCmpU(name, FloorTab[rover].FlName) == SAMESTRING &&
    FloorTab[rover].FlInuse)
      {
      return rover;

      }

    }
  if (doPartial)
  for (rover = 0; rover < TopFloor; rover++)
    {
    if (FloorTab[rover].FlInuse &&
    matchString(FloorTab[rover].FlName, name,
    lbyte(FloorTab[rover].FlName)) != NULL)
    return rover;

    }
  return ERROR;

  }
/*
* FAide()
*
* This function handles the floor-oriented aide commands.
*/
char FAide()
  {
  char *FloorAideOpts[] =
    {
    "Create Floor\n", "Delete empty floors\n", "Move rooms\n",
    "Kill Floor\n", "Rename Floor\n", "Floor moderator\n",
    "Invite users to floor", "Withdraw users from floor", "\b", ""

    };
  if (!aide)
    {
    if (loggedIn && strCmpU(logBuf.lbname, FloorTab[thisFloor].FlModerator) ==
    SAMESTRING)
      {
      RenameFloor();
      return GOOD_SELECT;

      }
    else
    return BAD_SELECT;    /* Indicates problem */

    }
  RegisterThisMenu("aideflr.mnu", FloorAideOpts);
  switch (GetMenuChar())
    {
    case 'C': CreateFloor();      break;
    case 'D': DeleteFloors();     break;
    case 'M': MoveRooms();        break;
    case 'K': KillFloor();        break;
    case 'R': RenameFloor();      break;
    case 'F': FlModerator();      break;
    case 'I': FInvite();        break;
    case 'W': FWithdraw();        break;
    case '\b': return BACKED_OUT;

    }
  return GOOD_SELECT;

  }
/*
* FlModerator()
*
* This function handles the floor moderator.
*/
void FlModerator()
  {
  label buffer;
  if (WhoIsModerator(buffer))
    {
    strCpy(FloorTab[thisFloor].FlModerator, buffer);
    putFloor(thisFloor);
    ZeroMsgBuffer(&msgBuf);
    sPrintf(msgBuf.mbtext, "%s's moderator set to %s by %s.",
    FloorTab[thisFloor].FlName, buffer, logBuf.lbname);
    aideMessage(NULL,FALSE);

    }

  }
/*
* DeleteFloors()
*
* This function deletes empty floors.
*/
void DeleteFloors()
  {
  int rover, count = 0;
  ZeroMsgBuffer(&msgBuf);
  sPrintf(msgBuf.mbtext, "Following empty floors deleted by %s: ",
  logBuf.lbname);
  for (rover = 1; rover < TopFloor; rover++)
    {
    if (FloorTab[rover].FlInuse)
      {
      if (FirstRoom(rover) == ERROR)
        {
        count++;
        FloorTab[rover].FlInuse = FALSE;
        putFloor(rover);
        sPrintf(lbyte(msgBuf.mbtext), "[%s], ", FloorTab[rover].FlName);

        }

      }

    }
  if (count)
    {
    *(lbyte(msgBuf.mbtext) - 2) = '.';
    *(lbyte(msgBuf.mbtext) - 1) = 0;

    }
  aideMessage(NULL,FALSE);

  }
/*
* MoveRooms()
*
* This function moves a series of rooms into the current floor.
*/
void MoveRooms()
  {
  int   CurrentRoom;
  char  *end;
  CurrentRoom = thisRoom;
  GlobalFloor = thisFloor;
  MoveCount = 0;
  ZeroMsgBuffer(&msgBuf);
  sPrintf(msgBuf.mbtext, "Following rooms moved to floor %s by %s: ",
  FloorTab[thisFloor].FlName, logBuf.lbname);
  getList(MoveToFloor, "Rooms to move to this floor", NAMESIZE, FALSE);
  getRoom(CurrentRoom);       /* MoveToFloor changes thisRoom & roomBuf */
  if (MoveCount != 0)
    {
    end = lbyte(msgBuf.mbtext);
    *(end - 2) = '.';
    *(end - 1) = 0;
    aideMessage(NULL,FALSE);

    }

  }
/*
* MoveToFloor()
*
* This function will move a single room to a floor.
*/
int MoveToFloor(char *name)
  {
  int roomNo;
  if ((roomNo = roomExists(name)) == ERROR)
    {
    Output_Citadel_Message("NOROOM",(long)name,NULL,NULL);
    return TRUE;

    }
  if (   roomNo == LOBBY  ||  roomNo == MAILROOM  ||  roomNo == AIDEROOM  )
    {
    if (!getYesNo("WRMVSP")) return TRUE;

    }
  MoveCount++;
  getRoom(roomNo);
  roomBuf.rbFlIndex = GlobalFloor;
  putRoom(roomNo);
  noteRoom();
  sPrintf(lbyte(msgBuf.mbtext), "%s, ", formRoom(thisRoom, FALSE, FALSE));
  return TRUE;

  }
/*
* RenameFloor()
*
* This function renames a floor.
*/
void RenameFloor()
  {
  label FloorName;
  int   ReturnNo;
  if (thisFloor == 0)
    {
    Output_Citadel_Message("USECFG",NULL ,NULL,NULL);
    return ;

    }
  if (!getXString("FLRNIT", FloorName, NAMESIZE, NULL, NULL))
  return ;
  if ((ReturnNo = FindFloor(FloorName, FALSE)) != ERROR)
    {
    if (ReturnNo != thisFloor)
      {
      Output_Citadel_Message("ALRYFR",(long)FloorName,NULL,NULL);
      return;

      }

    }
  ZeroMsgBuffer(&msgBuf);
  sPrintf(msgBuf.mbtext, "The floor %s renamed to %s by %s.",
  FloorTab[thisFloor].FlName, FloorName, logBuf.lbname);
  strCpy(FloorTab[thisFloor].FlName, FloorName);
  putFloor(thisFloor);
  aideMessage(NULL,FALSE);

  }
/*
* CreateFloor()
*
* This function creates a floor.
*/
void CreateFloor()
  {
  label FloorName;
  int   rover;
  if (  thisRoom == LOBBY ||
  thisRoom == MAILROOM ||
  thisRoom == AIDEROOM  )
    {
    Output_Citadel_Message("CCRFLR",NULL, NULL, NULL);
    return ;

    }
  if (!getXString("FLRNIT", FloorName, NAMESIZE, NULL, NULL))
  return ;
  if (FindFloor(FloorName, FALSE) != ERROR)
    {
    Output_Citadel_Message("ALRYFR",(long)FloorName,NULL,NULL);
    return;

    }
  for (rover = 1; rover < TopFloor; rover++)
  if (!FloorTab[rover].FlInuse) break;
  if (rover == TopFloor)
    {
    FloorTab = (struct floor *) realloc(FloorTab,
    sizeof *FloorTab * ++TopFloor);

    }
  roomBuf.rbFlIndex = rover;
  FloorTab[rover].FlInuse = TRUE;
  strCpy(FloorTab[rover].FlName, FloorName);
  memset(FloorTab[rover].FlModerator, '\0',sizeof(label));
  putFloor(rover);
  putRoom(thisRoom);
  noteRoom();
  ZeroMsgBuffer(&msgBuf);
  sPrintf(msgBuf.mbtext, "The floor %s created by %s.", FloorName,
  logBuf.lbname);
  aideMessage(NULL,FALSE);

  }
/*
* putFloor()
*
* This will put a floor record out to disk.
*/
void putFloor(int i)
  {
  SYS_FILE name;
  FILE *fd;
  long r;
  extern char *R_W_ANY;
  makeSysName(name, "ctdlflr.sys", &cfg.floorArea);
  if ((fd = safeopen(name, R_W_ANY)) == NULL)
  crashout("Couldn't open the floor file for update!");
  r = i * sizeof *FloorTab;
  fseek(fd, r, 0);
  if (fwrite(FloorTab + i, sizeof *FloorTab, 1, fd) != 1)
  crashout("?putFloor(): write failed!");
  fclose(fd);

  }
/*
* KillFloor()
*
* This function kills a floor.
*/
void KillFloor()
  {
  int CurrentFloor, CurrentRoom;
  if (roomBuf.rbFlIndex == 0)
    {
    Output_Citadel_Message("NOKILF",NULL ,NULL,NULL);
    return;

    }
  if (!getYesNo("CONFRM")) return;
  ZeroMsgBuffer(&msgBuf);
  sPrintf(msgBuf.mbtext, "Floor %s killed by %s.",
  FloorTab[thisFloor].FlName, logBuf.lbname);
  aideMessage(NULL,FALSE);
  CurrentFloor = thisFloor;   /* #define in CTDL.H */
  CurrentRoom  = thisRoom;
  maim = getYesNo("MVALLM");
  FloorRunner(thisRoom, MaimOrKill);
  FloorTab[CurrentFloor].FlInuse = FALSE;
  putFloor(CurrentFloor);
  /* due to behavior of MaimOrKill */
  getRoom((!maim) ? LOBBY : CurrentRoom);

  }
/*
* MaimOrKill()
*
* This will kill or move a room to the main floor.
*/
int MaimOrKill(int i)
  {
  getRoom(i);
  if (maim)
    {
    roomBuf.rbFlIndex = 0;

    }
  else
    {
    roomBuf.rbflags.INUSE = FALSE;

    }
  putRoom(i);
  noteRoom();
  return FALSE;

  }
/*
* FKnown()
*
* This function will handle the ticklish task of floor display.
*/
char FKnown(char mode)
  {
  int         rover, roomNo;
  extern int  DirAlign;
  extern char AlignChar;
  switch (mode)
    {
    case FORGOTTEN:
      Output_Citadel_Message("FORGFL",NULL,NULL,NULL);
    ShowNew = 2;
    SelNew = TRUE;
    RunDisplay();
    SelNew = FALSE;
    break;
    case INT_NOVICE:
    case INT_EXPERT:
    case NOT_INTRO:
    if (SelNew)
      Output_Citadel_Message("RMUNRF",(long)FloorTab[thisFloor].FlName,NULL,NULL);
    ShowNew = TRUE;
    FloorRunner(thisRoom, DispRoom);
    if (mode == INT_NOVICE || mode == NOT_INTRO)
      {
      Output_Citadel_Message("NOUNSN",NULL,NULL,NULL);
      ShowNew = FALSE;
      FloorRunner(thisRoom, DispRoom);

      }
    if (SelNew)
      {
      Output_Citadel_Message("OTFLUM",NULL,NULL,NULL);
      for (rover = 0; rover < TopFloor; rover++)
        {
        if (rover != roomBuf.rbFlIndex)
          {
          roomNo = FirstRoom(rover);
          if (FloorRunner(roomNo, RoomHasNew) != ERROR)
            Output_Citadel_Message("FLRNNL", (long)FloorTab[rover].FlName,NULL,NULL);

          }

        }

      }
    break;
    case DR_SEL:
    case SH_SEL:
    case PR_SEL:
    case ANON_SEL:
    case READONLY:
    RunDisplay();
    break;
    default:
    SelNew = TRUE;
    doCR();
    Output_Citadel_Message("FLRWUR", NULL,NULL,NULL);
    ShowNew = FoundNew = TRUE;
    RunDisplay();
    doCR();
    Output_Citadel_Message("FLRWNR", NULL,NULL,NULL);
    ShowNew = FoundNew = FALSE;
    RunDisplay();
    SelNew = FALSE;

    }
  return GOOD_SELECT;

  }
/*
* RunDisplay()
*
* This will do the actual work of displaying in Floor mode.
*/
void RunDisplay()
  {
  extern int  DirAlign;
  extern char AlignChar;
  int rover, roomNo;
  AlignChar = ' ';
  for (rover = 0; rover < TopFloor; rover++)
    {
    if (FloorTab[rover].FlInuse)
      {
      roomNo = FirstRoom(rover);
      if (FloorRunner(roomNo, CheckFloor) != ERROR)
        {
        DispFloorName(rover);
        FloorRunner(roomNo, DispRoom);
        DirAlign = 0;

        }

      }

    }

  }
/*
* DispFloorName()
*
* This function displays a floor name with periods, etc...
*/
void DispFloorName(int FloorNo)
  {
  int i;
  extern int DirAlign;
  extern char AlignChar;
  Output_Citadel_Message("FLRSNL", (long)FloorTab[FloorNo].FlName,NULL,NULL);
  if (termWidth >= 50)
    {
    DirAlign = 24;
    AlignChar = ' ';
    for (i = strLen(FloorTab[FloorNo].FlName); i < 21; i++)
    mPrintf(".");

    }
  else mPrintf("-");

  }
/*
* These functions are used as arguments to FloorRunner
*/
/*
* RoomHasNew()
*
* This returns TRUE if room has new messages for the current user.
*/
int RoomHasNew(int i)
  {
  return (    roomTab[i].rtlastMessage >
  logBuf.lbvisit[logBuf.lbgen[i] & CALLMASK] &&
  roomTab[i].rtlastMessage >= cfg.oldest     );

  }
/*
* CheckFloor()
*
* This multipurpose function returns true of one of the combinations is
* true.
*/
int CheckFloor(int i)
  {
  #ifdef OLD_STYLE
  if (( SelDirs   &&   roomTab[i].rtflags.ISDIR   ) ||
  ( SelShared &&   roomTab[i].rtflags.SHARED  ) ||
  ( SelPriv   &&   !roomTab[i].rtflags.PUBLIC ) ||
  ( SelAnon   &&   roomTab[i].rtflags.ANON )    ||
  ( SelRO     &&   roomTab[i].rtflags.READ_ONLY )    ||
  ( SelNew && (ShowNew == 2 || (RoomHasNew(i) && FoundNew) ||
  (!RoomHasNew(i) && !FoundNew))))
  return TRUE;
  #else
  if (FlagCheck(i) ||
  ( SelNew && (ShowNew == 2 || (RoomHasNew(i) && FoundNew) ||
  (!RoomHasNew(i) && !FoundNew))))
  return TRUE;
  #endif
  return FALSE;

  }
/*
* NSRoomHasNew()
*
* TRUE if room has new messages and isn't skipped.
*/
int NSRoomHasNew(int i)
  {
  if (!roomTab[i].rtflags.SKIP && RoomHasNew(i))
  return TRUE;
  if (roomTab[i].rtflags.SKIP) /* Kludge this in -- ugly but useful */
  HasSkipped = TRUE;
  return FALSE;

  }
/*
* DispRoom()
*
* This function will display a room name.
*/
int DispRoom(int i)
  {
  char HasNew;
  extern char shownHidden;
  HasNew = RoomHasNew(i);
  if (FlagCheck(i) ||
  ( SelNew &&
  (ShowNew == 2 || (HasNew && ShowNew == 1) ||
  (!HasNew && !ShowNew))))
    {
    Output_Citadel_Message("DISPRM",(long)formRoom(i, TRUE, TRUE),NULL,NULL);
    if (!roomTab[i].rtflags.PUBLIC) shownHidden = TRUE;
    if (SelDirs && HalfSysop())
      {
      if (FindDirName(i) != NULL)
        Output_Citadel_Message("DISPDR",(long)FindDirName(i),NULL,NULL);
      else
        Output_Citadel_Message("DISPNF",NULL,NULL,NULL);

      }

    }
  return FALSE;

  }
/*
* Zroom()
*
* This lets us Zforget a room.  Used for ;Z.
*/
int Zroom(int i)
  {
  int r;
  if (     i == LOBBY    ||
  i == MAILROOM ||
  i == AIDEROOM      )
    {
    return FALSE;

    }
  r = (roomTab[i].rtgen + FORGET_OFFSET) % MAXGEN;
  logBuf.lbgen[i] = r << GENSHIFT;
  return FALSE;

  }
/*
* FSroom()
*
* This will skip a room on a floor.
*/
int FSroom(int i)
  {
  roomTab[i].rtflags.SKIP = 1;     /* Set bit */
  return FALSE;

  }
/*
* FindAny()
*
* This finds any known room on a floor.
*/
int FindAny(int i)
  {
  return TRUE;        /* My, that was easy... */

  }
static int FlagCheck(int i)
  {
  if (( SelDirs   &&   roomTab[i].rtflags.ISDIR   ) ||
  ( SelShared &&   roomTab[i].rtflags.SHARED  ) ||
  ( SelPriv   &&   !roomTab[i].rtflags.PUBLIC ) ||
  ( SelAnon   &&   roomTab[i].rtflags.ANON )    ||
  ( SelRO     &&   roomTab[i].rtflags.READ_ONLY ))
  return TRUE;
  return FALSE;

  }
