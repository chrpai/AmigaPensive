/*
*       log.c
*
* userlog code for Citadel bulletin board system.
*/
/*
*       history
*
* 85Nov15 HAW  MS-DOS library implemented.
* 85Aug31 HAW  Fix <.ep> problem.
* 85Aug17 HAW  Update to onLine().
* 85Aug10 HAW  Fix so system doesn't go out to disk on short pwds.
* 85Jul26 HAW  Kill noteLog(), insert anti-hack code in newPW().
* 85Jun13 HAW  Tweak code for networking stuff.
* 85Mar13 HAW  Moved zapLogFile() and logInit and logSort into confg.c.
* 85Jan19 HAW  Fix terminate() so room prompt isn't tossed at modem.
* 85Jan19 HAW  New Users are now directed to type ".help POLICY".
* 85Jan19 HAW  Move findPerson() into file.
* 84Dec15 HAW  Fix bug that allowed discovery of private rooms.
* 84Aug30 HAW  Now we roll into the 16-bit world.
* 84Jun23 HAW&JLS  Eliminating unused local variables using CRF.
* 84Jun19 JLS  Fixed terminate so that Mail> doesn't screw up SYSOP.
* 84Apr04 HAW  Started upgrade to BDS C 1.50a.
* 83Feb27 CrT  Fixed login-in-Mail> bug.
* 83Feb26 CrT  Limited # new messages for new users.
* 83Feb18 CrT  Null pw problem fixed.
* 82Dec06 CrT  2.00 release.
* 82Nov03 CrT  Began local history file & general V1.2 cleanup
*/
#include "ctdl.h"
/*
*       contents
*
*      doInviteDisplay()       displays invited users
*      GetUser()    gets a user record from user
*      login()      is menu-level routine to log caller in
*      newPW()      is menu-level routine to change a PW
*      newUser()    menu-level routine to log a new caller
*      PWSlot()   returns CTDLLOG.buf slot password is in
*      slideLTab()    support routine for sorting logTab
*      storeLog()   store data in log
*      strCmpU()    strcmp(), but ignoring case distinctions
*      terminate()    menu-level routine to exit system
*/
int   thisSlot;   /* logTab slot logBuf found via */
char    loggedIn = FALSE;      /* Global have-caller flag      */
char    prevChar;   /* for EOLN/EOParagraph stuff   */
static char      pwChangeCount;   /* Anti-hack variable   */
int   logTries;
extern logBuffer logBuf;    /* Log buffer of a person       */
extern logBuffer logTmp;    /* Log buffer of a person       */
extern LogTable *logTab;    /* RAM index of pippuls   */
LogTable *delogTab = NULL;    /* RAM index of pippuls   */
extern CONFIG cfg;    /* Configuration variables      */
extern rTable *roomTab;   /* RAM index of rooms   */
extern aRoom  roomBuf;    /* Room buffer    */
extern MessageBuffer   tempMess, msgBuf;
extern char   heldMess;
extern FILE   *logfl;   /* log file descriptor    */
extern int    thisLog;    /* entry currently in logBuf    */
extern char   outFlag;    /* Output skip flag   */
extern char   whichIO;    /* Where IO's going...    */
extern char   haveCarrier;       /* Do we still got carrier?     */
extern char   echo;   /* Who gets what    */
extern char   onConsole;    /* Where we get stuff from      */
extern int    thisRoom;   /* The room we're in    */
extern int    exitValue;
extern char      DiskHeld;

static int Bad_String(char *string, char *filename);


/*
* doInviteDisplay()
*
* This function shows invited users.
*/
char doInviteDisplay()
  {
  int rover = 0, s;
  label who;
  if (getNormStr("", who, NAMESIZE, BS_VALID) == BACKED_OUT)
  return BACKED_OUT;
  if (strLen(who) == 0)
    {
    mPrintf("%s, ", logBuf.lbname);
    outFlag = OUTOK;
    for (; outFlag == OUTOK && rover < cfg.MAXLOGTAB; rover++)
      {
      if (rover != thisLog)
        {
        getLog(&logTmp, rover);
        if (logTmp.lbflags.L_INUSE &&
        (s = knowRoom(&logTmp, thisRoom)) == KNOW_ROOM ||
        s == WRITE_PRIVS)
          {
          mPrintf("%s", logTmp.lbname);
          if (s == WRITE_PRIVS && roomBuf.rbflags.READ_ONLY)
          mPrintf(" (w-p)");
          mPrintf(", ");

          }

        }

      }
    mPrintf("\b\b.");

    }
  else
    {
    if (findPerson(who, &logTmp) == ERROR)
    mPrintf("Who?\n ");
    else if ((s = knowRoom(&logTmp, thisRoom)) == KNOW_ROOM ||
    s == WRITE_PRIVS)
    mPrintf("Affirmative!\n ");
    else
    mPrintf("Negative!\n ");

    }
  return GOOD_SELECT;

  }
/*
* GetUser()
*
* This is a general function for requesting user name, getting user record,
* and doing error checking.  Returns ERROR on failure.
*/
int GetUser(label who, logBuffer *lBuf, char Menu)
  {
  int logNo;
  if (Menu && onConsole)
  SysopRequestString(NO_MENU, "WHOUSR", who, NAMESIZE, 0);
  else getNormStr("WHOUSR", who, NAMESIZE, 0);
  if (strLen(who)             == 0 )          return ERROR;
  if (strCmpU(who, "Citadel") == SAMESTRING ) return (int)cfg.MAXLOGTAB;
  if (strCmpU(who, "Sysop") == SAMESTRING)  strCpy(who, cfg.SysopName);
  logNo = findPerson(who, lBuf);
  if (logNo == ERROR)
    {
    if (Menu && onConsole)
    SysopError(NO_MENU, "No such person...\n ");
    else mPrintf("No such person...\n ");

    }
  return logNo;

  }
/*
* DoAllQuestion()
*
* This will ask if someone wants to apply an operation to all users.
*/
int DoAllQuestion(char *posprompt, char *negprompt)
  {
  if (SysopGetYesNo(NO_MENU, NULL, posprompt)) return TRUE;
  if (SysopGetYesNo(NO_MENU, NULL, negprompt)) return FALSE;
  return ERROR;

  }
/*
* login()
*
* This is the menu-level routine to log someone in.
*/
void login(char *password)
  {
  int  foundIt;
  int  Adoor;
  extern char *READ_ANY, *LCHeld, shownHidden;
  extern long *DL_Total;
  extern SListBase DL_List;
  TwoNumbers *tmp;
  foundIt =    ((PWSlot(password, /*load = */TRUE)) != ERROR);
  pwChangeCount = 1;
  if (foundIt && *password)
    {
    /* update userlog entries: */
    heldMess     = FALSE;
    loggedIn     = TRUE;
    setUp(TRUE);
    /* recite caller's name, etc:    */
    Output_Citadel_Message("WELCOM",NULL, NULL, NULL);
    Update_Caller_Count();              /* update the stats */
    Output_Citadel_Message("CCPRIV",NULL, NULL, NULL);
    Adoor = CheckAutoDoor("Citadel");
    (void)RunAutoDoor(Adoor, FALSE);
    (void)RunAutoDoor(CheckAutoDoor(logBuf.lbname), FALSE);
    ScrNewUser();
    HelpIfPresent("Notice.PRE");
    if (!MultiBanner("Notice")) HelpIfPresent("Notice.BLB");
    HelpIfPresent("Notice.SFX");
    logMessage(L_IN, logBuf.lbname, FALSE);
    showMessages(NEWoNLY, FALSE,
    logBuf.lbvisit[logBuf.lbgen[thisRoom] & CALLMASK],
    OptionValidate);
    listRooms(expert ? INT_EXPERT : INT_NOVICE);
    if (shownHidden )
      Output_Citadel_Message("HIDDEN",NULL,NULL,NULL);
    /*
    * This code sets up to handle download time limits.  Users who
    * download are kept in a list -- user# + time spent in downloading.
    * The list is cleared whenever the system is brought down/up, or
    * a d-l time limit #event expires or initiates.  Here, we check to
    * see if this user has downloaded during the current period.  If so,
    * we simply point at the record so we know how much time has already
    * been spent downloading.  If not found, we initialize a new record.
    */
    if ((DL_Total = SearchList(&DL_List, (tmp = MakeTwo(thisLog, 0l))))
    == NULL)
      {
      DL_Total = &tmp->second;
      AddData(&DL_List, tmp, NULL, TRUE);

      }
    else free(tmp);
    outFlag = OUTOK;
    if (
    (
    logBuf.lbMail[MAILSLOTS-1].rbmsgNo >
    (logBuf.lbvisit[   logBuf.lbgen[MAILROOM] & CALLMASK   ])
    )
    &&
    (logBuf.lbMail[MAILSLOTS-1].rbmsgNo & S_MSG_MASK) > cfg.oldest
    &&
    thisRoom != MAILROOM
    )
      {
      Output_Citadel_Message("YRMAIL",NULL, NULL, NULL);
      }
    if (GetIntMessage())
      {
      outFlag = OUTOK;
      DiskHeld = TRUE;
      Output_Citadel_Message("HAVHLD",NULL, NULL, NULL);

      }

    }
  else
    {
    /* discourage password-guessing: */
    if (cfg.LoginAttempts > 0 && logTries++ >= cfg.LoginAttempts)
    if (whichIO == MODEM)
      {
      Output_Citadel_Message("HACKLG", NULL, NULL, NULL);
      HangUp(TRUE);
      return ;  /* skip the pause() farther on */

      }
    if (strLen(password) > 1 && whichIO == MODEM)
    if ((logTries-1) > 2 ) pause(2000);
    if (!cfg.BoolFlags.unlogLoginOk  &&  whichIO == MODEM)
      {
      if (!HelpIfPresent("UnLog.BLB"))
      Output_Citadel_Message("NOACCT",NULL, NULL, NULL);

      }
    else
      {
      if (getYesNo("NORCRE"))
        {
        SpecialMessage("Status:Creating New Account");
        newUser(&logBuf);
        Update_Caller_Count();              /* update the stats */
        }

      }

    }

  }
/*
* GetIntMessage()
*
* This will get an interrupted message from the Held dir.
*/
char GetIntMessage()
  {
  extern FILE *upfd;
  SYS_FILE funFile;
  label temp;
  /* if the user used Pause-E, we don't want to flush the buffer */
  if (!heldMess)
  ZeroMsgBuffer(&tempMess);
  if (cfg.BoolFlags.HoldOnLost)
    {
    sPrintf(temp, LCHeld, thisLog);
    makeSysName(funFile, temp, &cfg.holdArea);
    if ((upfd = safeopen(funFile, READ_ANY)) != NULL)
      {
      fread(&tempMess, STATIC_MSG_SIZE, 1, upfd);
      crypte(&tempMess, STATIC_MSG_SIZE, thisLog);
      fread(tempMess.mbtext, MAXTEXT, 1, upfd);
      crypte(tempMess.mbtext, MAXTEXT, thisLog);
      MakeList(&tempMess.mbCC, "", upfd);
      fclose(upfd);
      unlink(funFile);
      heldMess = TRUE;
      return TRUE;

      }

    }
  return FALSE;

  }
/*
* newPW()
*
* This is menu-level routine to change one's password.  Since some Citadel
* nodes run in public locations, we avoid displaying passwords on the console.
*/
void newPW()
  {
  char oldPw[NAMESIZE];
  char pw[NAMESIZE];
  int  goodPW;
  extern char *confirm;
  if (!loggedIn)
    {
    mPrintf("\n How?\n ");
    return ;

    }
  /* save password so we can find current user again: */
  strcpy(oldPw, logBuf.lbpw);
  storeLog();
  do
    {
    echo    = CALLER;
    getNormStr("UPASSW", pw, NAMESIZE, NO_ECHO);
    echo    = BOTH;
    if (strLen(pw) == 0 || strCmpU(pw, oldPw) == SAMESTRING)
      {
      PWSlot(oldPw, /*load = */TRUE);
      return ;

      }
    /* check that PW isn't already claimed: */
    goodPW = (PWSlot(pw,/* load = */TRUE) == ERROR  &&  strLen(pw) >= 5);
    if( goodPW && strcmp(logBuf.lbpw,pw)==0)goodPW = FALSE;
    if( goodPW && Bad_String(pw, "badpasswords.sys") )goodPW = FALSE;
    if (pwChangeCount == 0 && !onConsole && onLine())
      {
      Output_Citadel_Message("HANGON",NULL, NULL, NULL);
      pause(3000);      /* Discourage hacking       */
      }
    else pwChangeCount--;
    if (!goodPW)
      Output_Citadel_Message("POORCH",NULL, NULL, NULL);
    }
  while (!goodPW && (haveCarrier || whichIO==CONSOLE));
  doCR();
  PWSlot(oldPw, /*load = */TRUE);     /* reload old log entry */
  pw[NAMESIZE-1] = 0x00;    /* insure against loss of carrier:*/
  if (goodPW  &&  strLen(pw) > 1)
    {
    /* accept new PW:   */
    mPrintf("\n %s\n Password: ", logBuf.lbname);
    echo = CALLER;
    mPrintf("%s\n ", pw);
    echo = BOTH;
    if (getYesNo("CONFRM"))
      {
      strcpy(logBuf.lbpw, pw);
      logTab[0].ltpwhash      = hash(pw);
      storeLog();

      }

    }

  }
/*
* newUser()
*
* This is the add new user function.
*/
void newUser(logBuffer *lBuf)
  {
  logBuffer   l2;
  char  *temp;
  char  fullnm[NAMESIZE], tmp[30];
  SYS_FILE    checkHeld;
  char  pw[NAMESIZE], NewLogin;
  int   g, h, i, ourSlot, AttemptCount = 0;
  int   good; /* does double duty, keep it as int */
  MSG_NUMBER  low;
  TwoNumbers *ttemp;
  extern char *LCHeld/* , IsAutoMail*/;
  if (!NewUserDoor())
    {
    HangUp(TRUE);
    return;

    }
  NewLogin = (lBuf == &logBuf) ;
  if (NewLogin)
  HelpIfPresent("NewUser.BLB");
  zero_struct(lBuf->lbflags);     /* zero all flags */
  lBuf->lbflags.LFMASK = TRUE;    /* except we need linefeeds */
  if (!configure(lBuf, !NewLogin, TRUE))  /* configure correctly */
  return;
  initLogBuf(&l2);
  Output_Citadel_Message("PASSWD",NULL,NULL,NULL);
  do
    {
    /* get name and check for uniqueness... */
    if (AttemptCount++ > cfg.LoginAttempts)
      Output_Citadel_Message("MINDPW", NULL, NULL, NULL);
    if (AttemptCount > cfg.LoginAttempts + 2 && NewLogin)
    if (whichIO == MODEM) HangUp(TRUE);
    do
      {
      getNormStr("USRNAM", fullnm, NAMESIZE, 0);
      if ((temp = strchr(fullnm, '@')) != NULL)*temp='\0'; /* fix for bad user names */
      NormStr(fullnm);
      good = TRUE;
      if( findPerson(fullnm, &l2) != ERROR) good = FALSE;
      if( Bad_String(fullnm, "badnames.sys" ) ) good = FALSE;
      h = hash(fullnm);
      if (    h == 0    /* "HUH?" --HAW 84Aug31   */  )
        {
        good = FALSE;

        }
      /* lie sometimes -- hash collision !=> name collision */
      if (!good)
        {
        if (strLen(fullnm) == 0)
          {
          if (getYesNo("ABORTA"))
            {
            killLogBuf(&l2);
            setUp(NewLogin);
            logTries++;
            return ;

            }

          }
        else Output_Citadel_Message("NOTNEW", (long)fullnm, NULL, NULL);
        }

      }
    while (!good  &&  (haveCarrier || whichIO==CONSOLE));
    /* get password and check for uniqueness...     */
    do
      {
      echo  = CALLER;
      getNormStr("UPASSW",  pw, NAMESIZE, 0);
      good = TRUE;
      echo  = BOTH  ;
      if( strlen(pw) < 5)
        {
        good = FALSE;
        Output_Citadel_Message("2SHORT",NULL,NULL,NULL);
        };
      if( good &&  strCmpU(fullnm,pw) == 0 ) good = FALSE;
      if( good &&  Bad_String(pw, "badpasswords.sys" ) ) good = FALSE;
      h    = hash(pw);
      for (i = 0; i < cfg.MAXLOGTAB && good; i++)
        {
        if (h == logTab[i].ltpwhash) good = FALSE;

        }
      if (h == 0)   good = FALSE;
      if (!good)
        {
        if (strLen(pw) == 0)
          {
          if (getYesNo("ABORTA"))
            {
            killLogBuf(&l2);
            setUp(NewLogin);
            logTries++;
            return ;

            }

          }
        else Output_Citadel_Message("POORCH", NULL, NULL, NULL);
        }

      }
    while( !good  &&  (haveCarrier || whichIO==CONSOLE));
    mPrintf("\n Username: %s", fullnm);
    mPrintf("\n Password: ");
    echo = CALLER;
    mPrintf("%s\n ", pw);
    echo = BOTH;

    }
  while (!getYesNo("OKPROM") && (haveCarrier || whichIO==CONSOLE));
  if ((haveCarrier || whichIO == CONSOLE))
    {
    if (NewLogin)
    logMessage(L_IN, fullnm, '+');
    /* kick least recent temporary acct out of userlog and claim entry: */
    for (good = cfg.MAXLOGTAB-1; good >= 0; good--)
    if (!logTab[good].ltpermanent) break;
    if (good < 0) good = cfg.MAXLOGTAB - 1; /* too bad */
    ourSlot   = logTab[good].ltlogSlot;
    slideLTab((NewLogin) ? 0 : 1, good);
    logTab[NewLogin ? 0 : 1].ltlogSlot = ourSlot;
    if (NewLogin)
    thisLog = ourSlot;
    /* copy info into record:       */
    strcpy(lBuf->lbname, fullnm);
    strcpy(lBuf->lbpw, pw);
    lBuf->lbflags.L_INUSE    = TRUE;
    lBuf->lbflags.NET_PRIVS  = cfg.BoolFlags.NetDft;
    lBuf->lbflags.DOOR_PRIVS = cfg.BoolFlags.DoorDft;
    lBuf->lbflags.DL_PRIVS   = cfg.BoolFlags.DL_Default;
    lBuf->credit             = 0;  /* No L-D credit  */
    low                      = cfg.newest-50;
    if (cfg.oldest - low < 0x8000)   low = cfg.oldest;
    for (i=1;  i<MAXVISIT;  i++)   lBuf->lbvisit[i]= low;
    lBuf->lbvisit[0]         = cfg.newest;
    lBuf->lbvisit[(MAXVISIT-1)] = cfg.oldest;
    /* initialize rest of record:   */
    for (i = 0;  i < MAXROOMS;  i++)
      {
      if (roomTab[i].rtflags.PUBLIC == 1)
        {
        g = (roomTab[i].rtgen);
        lBuf->lbgen[i] = (g << GENSHIFT) + (MAXVISIT-1);

        }
      else
        {
        /* set to one less */
        g = (roomTab[i].rtgen + (MAXGEN-1)) % MAXGEN;
        lBuf->lbgen[i] = (g << GENSHIFT) + (MAXVISIT-1);

        }

      }
    memset(lBuf->lbMail, 0, MAIL_BULK);
    /* fill in logTab entries       */
    i = (NewLogin) ? 0 : 1;
    logTab[i].ltpwhash      = hash(pw)  ;
    logTab[i].ltnmhash      = hash(fullnm)    ;
    logTab[i].ltlogSlot     = ourSlot ;
    logTab[i].ltnewest      = lBuf->lbvisit[0];
    logTab[i].ltpermanent   = FALSE;  /* new accts not perm */
    /* special kludge for Mail> room, to signal no new mail:   */
    if (NewLogin)
      {
      loggedIn = TRUE;
      roomTab[MAILROOM].rtlastMessage =
      lBuf->lbMail[MAILSLOTS-1].rbmsgNo;

      }
    /* automatic mail for the new user. */
    makeSysName(checkHeld, (lBuf->lbflags.EXPERT) ? "ExpMail.BLB" :
    "NovMail.BLB", &cfg.homeArea);
    if (access(checkHeld, 0) == 0)
      {
      i = thisRoom;
      getRoom(MAILROOM);
      ZeroMsgBuffer(&msgBuf);
      ingestFile(checkHeld, &msgBuf);
      strCpy(msgBuf.mbauth, (strLen(cfg.SysopName)) ?
      cfg.SysopName : "Citadel");
      strCpy(msgBuf.mbto, lBuf->lbname);
      putMessage(lBuf);
      /* IsAutoMail = FALSE; */
      getRoom(i);

      }
    ClearBio(ourSlot);
    if (NewLogin)
      {
      storeLog();
      ScrNewUser();
      if (expert)
        {
        if (getYesNo("WRTBIO"))
        EditBio();

        }
      /*
      * Create a new download record.  See login() for more details.
      */
      AddData(&DL_List, ttemp = MakeTwo(thisLog, 0l), NULL, TRUE);
      DL_Total = &ttemp->second;

      }
    if (cfg.BoolFlags.HoldOnLost)
      {
      sPrintf(tmp, LCHeld, ourSlot);
      makeSysName(checkHeld, tmp, &cfg.holdArea);
      unlink(checkHeld);

      }
    if (NewLogin)
      {
      if (!MultiBanner("Notice"))
      HelpIfPresent("Notice.BLB");
      listRooms(expert ? INT_EXPERT : INT_NOVICE);
      Output_Citadel_Message("RDHELP",NULL, NULL, NULL);
      }
    else
      {
      logTab[1].ltnewest    = cfg.newest;
      lBuf->lbvisit[0]      = cfg.newest;
      lBuf->lbflags.DOOR_PRIVS = getYesNo("DRPRIV");
      lBuf->lbflags.DL_PRIVS  = getYesNo("DLPRIV");
      lBuf->lbflags.AIDE      = getYesNo("ADPRIV");
      if (cfg.BoolFlags.netParticipant)
      lBuf->lbflags.NET_PRIVS = getYesNo("NTPRIV");
      if( lBuf->lbflags.NET_PRIVS)
        lBuf->credit          = SysopGetNumber(NULL, "HWMYCR", 0l, 1000l);
      putLog(lBuf, ourSlot);

      }

    }
  killLogBuf(&l2);

  }
/*
* PWSlot()
*
* This returns ctdllog.sys slot password is in, else ERROR.
* NB: we also leave the record for the user in logBuf.
*/
int PWSlot(char pw[NAMESIZE], char load)
  {
  int  h, i;
  int  foundIt, ourSlot;
  if (strLen(pw) < 2)   /* Don't search for these pwds    */
  return ERROR;
  h = hash(pw);
  /* Check all passwords in memory: */
  for(i = 0, foundIt = FALSE;  !foundIt && i < cfg.MAXLOGTAB;  i++)
    {
    /* check for password match here */
    /* If password matches, check full password */
    /* with current newUser code, password hash collisions should   */
    /* not be possible... but this is upward compatable & cheap     */
    if (logTab[i].ltpwhash == h)
      {
      ourSlot     = logTab[i].ltlogSlot;
      getLog(&logTmp, ourSlot);
      if (strCmpU(pw, logTmp.lbpw) == SAMESTRING)
        {
        /* found a complete match */
        thisSlot = i   ;
        foundIt  = TRUE;

        }

      }

    }
  if (foundIt)
    {
    if (load == TRUE)
      {
      copyLogBuf(&logTmp, &logBuf);
      thisLog = ourSlot;

      }
    return thisSlot;

    }
  return ERROR   ;

  }
/*
* slideLTab()
*
* This slides bottom N lots in logTab down.  For sorting.
*/
void slideLTab(int slot, int last)
  {
  int i;
  /* open slot up: (movmem isn't guaranteed on overlaps) */
  for (i = last - 1;  i >= slot;  i--)
    {
    memcpy(&logTab[i + 1], logTab + i,(long)cfg.sizeLTentry);

    }

  }
/*
* storeLog()
*
* This function stores the current log record.
*/
void storeLog()
  {
  logTab[0].ltnewest    = cfg.newest;
  logBuf.lbvisit[0]     = cfg.newest;
  putLog(&logBuf, thisLog);

  }

/*
* terminate()
*
* This is the menu-level routine for a user to exit system.
*/
void terminate(int discon, int save)
  {
  extern char heldMess, *READ_TEXT;
  extern MessageBuffer  tempMess;
  extern int  exitValue;
  extern char ExitToMsdos, hst;
  int i;
  char  StillThere;
  outFlag    = IMPERVIOUS;
  DiskHeld   = FALSE;
  StillThere = onLine();
  /* this stops the .TS cheat of password hacking */
  if (!loggedIn) discon = TRUE;
  if (loggedIn && exitValue != DOOR_EXIT)
      Output_Citadel_Message("GOODBY",NULL, NULL, NULL);
  if (StillThere && exitValue != DOOR_EXIT)
    {
    HelpIfPresent("LoNotice.Pre");
    if (!MultiBanner("LoNotice"))
    HelpIfPresent("LoNotice.BLB");
    HelpIfPresent("LoNotice.SFX");

    }
  if (discon && exitValue != DOOR_EXIT)
    {
    switch (whichIO)
      {
      case MODEM:
      if (onLine() && !cfg.BoolFlags.IsDoor)
        {
        pause(50);
        HangUp(FALSE);
        modIn();    /* And now detect carrier loss  */
        while (MIReady()) inp();    /* clear buffer */

        }
      break;
      case CONSOLE:
      whichIO =  MODEM;
      EnableModem(FALSE);
      if (!ExitToMsdos)
        {
        ReInitModem();
        printf("\n System returned to MODEM MODE...\n ");

        }
      startTimer(NEXT_ANYNET);    /* start up anytime net timer */
      break;

      }
    /* if we're functioning as a door ... */
    if (cfg.BoolFlags.IsDoor)
      {
      ExitToMsdos = TRUE;
      exitValue   = SYSOP_EXIT;   /* Seems appropriate */

      }

    }
  Output_Citadel_Message("CHNGDT",NULL,NULL,NULL);
  if (loggedIn)
    {
    ReadDate(formDate(), &logBuf.lblaston);
    if (StillThere)
    SetKnown(-1, 0, thisRoom, &logBuf);
    if (save)       storeLog();
    logBuf.lbname[0] = 0; /* For screen display */
    if (heldMess)
    SaveInterrupted(&tempMess);

    }
  if (exitValue != DOOR_EXIT)
  logMessage(L_OUT, "", (discon) ? 0 : '-');
  loggedIn = FALSE;
  setUp(TRUE);
  for (i = 0; i < MAXROOMS; i++)      /* Clear skip bits */
  roomTab[i].rtflags.SKIP = 0;
  /*if (discon) CacheMessages(ALL_NETS, TRUE);*/
  outFlag = OUTOK;
  ScrNewUser();
  if (exitValue != DOOR_EXIT)
  getRoom(LOBBY);
  ClearDoorTimers();
  logBuf.lbflags.ANSI     = 0;
  }
/*
 * Bad_String()
 * This function will check to see if the requested file is available
 * If it is not, it just returns FALSE and assumes the SYSOP does not
 * have it or want it checked.
 * This function searches the stringfile to discover if the requested
 * string is acceptable or not.
 */
static int Bad_String(char *fullnm, char *stringfile)
{
    SYS_FILE name;
    char line[25], toReturn = FALSE;
    FILE *fd;
    makeSysName(name, stringfile, &cfg.roomArea);
    if ((fd = fopen(name, READ_TEXT)) != NULL)
      {
        while (GetAString(line, sizeof line, fd) != NULL && !toReturn)
              if (strCmpU(fullnm, line) == 0) toReturn = TRUE;
      fclose(fd);
      };
    return toReturn;
}

