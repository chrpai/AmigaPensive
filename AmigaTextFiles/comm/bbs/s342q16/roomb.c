/**
*       roomb.c
*
* room code for Citadel bulletin board system
*       History
*
* 87Mar30 HAW  Fix for carrier loss in renameRoom, Invite only room
*   bug, add <L>ogin to novice menu for unlogged
* 86Aug16 HAW  Kill history in here due to space problems.
* 85Jan16 JLS  Fix getText so console starting CR creates blank msg.
* 84Jun28 JLS  Enhancement: Creator of a room is listed in Aide>.
* 84Apr04 HAW  Start upgrade to BDS 1.50a.
* 83Feb26 CrT  bug in makeRoom when out of rooms fixed.
* 83Feb26 CrT  matchString made caseless, normalizeString()
* 83Feb26 CrT  "]" directory prompt, user name before prompts
* 82Dec06 CrT  2.00 release.
* 82Nov02 CrT  Cleanup prior to V1.2 mods.
* 82Nov01 CrT  Proofread for CUG distribution.
* 82Mar27 dvm  conversion to v. 1.4 begun
* 82Mar25 dvm  conversion for TRS-80/Omikron test started
* 81Dec21 CrT  Log file...
* 81Dec20 CrT  Messages...
* 81Dec19 CrT  Rooms seem to be working...
* 81Dec12 CrT  Started.
*/
#include "ctdl.h"
/**
*       Contents
*
* CleanEnd()    cleans up end of msg
* editText()    handles the end-of-message-entry menu
* findRoom()    find a free room
* getNumber()   prompt user for a number, limited range
* getString()   read a string in from user
* getText()   reads a message in from user
* getYesNo()    prompts for a yes/no response
* givePrompt()    gives usual "THISROOM>" prompt
* indexRooms()    build RAM index to ctdlroom.sys
* initialArchive()  does initial archive of a room
* insertParagraph() inserts paragraph into message
* makeRoom()    make new room via user dialogue
* matchString()   search for given string
* noteRoom()    enter room into RAM index
* renameRoom()    sysop special to rename rooms
* replaceString()   string-substitute for message entry
* searchForRoom()   auxilary to addToList()
*
* # -- operating system dependent function.
*/
char     *public_str = " Public";
char     *private_str = " Private";
char     *perm_str = "Permanent";
char     *temp_str = "Temporary";
char      exChar = '?';
char     *on = "on";
char     *off = "off";
char     *no = "no";
char     *yes = "yes";
char      ShType;
char      MsgEntryType = MSG_ENTRY;
extern SListBase Arch_base;
extern aRoom roomBuf;             /** Room buffer */
extern rTable *roomTab;         /** RAM index   */
extern FILE *roomfl;              /** Room file descriptor   */
extern CONFIG cfg;                  /** Other variables   */
extern MessageBuffer msgBuf;        /** Message buffer   */
extern MessageBuffer tempMess;    /** For held messages   */
extern logBuffer logBuf;          /** Person buffer   */
extern logBuffer logTmp;          /** Person buffer   */
extern NetBuffer netBuf;
extern SListBase Moderators;
extern int masterCount,
          thisRoom,
          thisLog;
extern char remoteSysop;
extern char outFlag;              /** Output flag   */
extern char loggedIn;             /** Logged in?   */
extern char haveCarrier;          /** Have carrier?   */
extern char onConsole;            /** How about on Console?   */
extern char whichIO;              /** Where is I/O?   */
extern char *baseRoom;
extern char heldMess;
extern char echo;
extern char echoChar;
extern char *confirm;
extern int thisNet;
extern char *WRITE_TEXT;
extern FILE *netLog;
static char CCAddFlag;

/**
  editText()
  This funcion handles the end-of-message-entry menu.
  return TRUE  to save message to disk, FALSE to abort message, and
  ERROR if user decides to continue
**/
int
editText(char *buf, int lim)
{
  extern char *ALL_LOCALS,
           *WRITE_LOCALS;
  char      c,
           *oldRec;
  int       letter;
  char     *OtherEdit[] =
  {
    "Abort\n", "Continue", "Replace String\n", "Print Formatted\n",
    "Global Replace\n",
    " ", " ", " ", " ", " ", " ", " ", " ",
    " ", " ", " ", " ", " ", " ", " ", ""

  };
  static struct
    {
      char     *Save;
      char     *Menu;
      char     *Print;

    }
  Displays[] =
  {
    {
      "Saving message...\n", "edit.mnu", NULL

    }
    ,
    {
      "Saving file description...\n", "descedit.mnu", "File description"

    }
    ,
    {
      "Saving information...\n", "infoedit.mnu", "Information"

    }
    ,
    {
      "Saving biography...\n", "infoedit.mnu", "Biography"

    }
    ,

  };
  ExtraOption(OtherEdit, Displays[MsgEntryType].Save);
  if (MsgEntryType == MSG_ENTRY)
    {
      ExtraOption(OtherEdit, "Hold message for later\n");
      ExtraOption(OtherEdit, "Insert paragraph break\n");
      if (thisRoom == MAILROOM && loggedIn)
        ExtraOption(OtherEdit, "Who else\n");
      if (NetValidate(FALSE) && (thisRoom == MAILROOM ||
                                 roomBuf.rbflags.SHARED))
        ExtraOption(OtherEdit, "N");

    }
  else if (MsgEntryType == INFO_ENTRY || MsgEntryType == BIO_ENTRY)
    {
      ExtraOption(OtherEdit, "Insert paragraph break\n");

    }
  if (onConsole && cfg.BoolFlags.SysopEditor)
    ExtraOption(OtherEdit, "Outside Editor");
  OtherEditOptions(OtherEdit);
  RegisterThisMenu(Displays[MsgEntryType].Menu, OtherEdit);
  do
    {
      outFlag = IMPERVIOUS;
      doCR();
      if (MsgEntryType == MSG_ENTRY && !logBuf.lbflags.NoPrompt)
        Output_Citadel_Message("FLOORN", (long)roomBuf.rbname , NULL, NULL);
      mPrintf("Editor cmd: ");
      switch ((letter = GetMenuChar()))
        {
          case 'A':
            if (strLen(buf) == 0 || getYesNo("CONFRM"))
              {
                return FALSE;

              }
            break;
          case 'C':
            doCR();
            mPrintf("...%s", CleanEnd(buf));
            return ERROR;
          case 'I':
            insertParagraph(buf, lim);
            break;
          case 'W':
            if (thisRoom == MAILROOM)
              {
                CCAddFlag = TRUE;
                getList(CCAddDelete, "Other recipients", CC_SIZE, FALSE);
                CCAddFlag = FALSE;
                getList(CCAddDelete, "Users to take off Other recipients",CC_SIZE, FALSE);

              }
            break;
          case 'P':
            outFlag = OUTOK;
            if (Displays[MsgEntryType].Print != NULL)
              {
                doCR();
                mPrintf("   %s", Displays[MsgEntryType].Print);
                doCR();
                mFormat(buf);

              }
            else
              printMessage(TRUE);
            break;
          case 'R':
          case 'G':
            replaceString(buf, lim, (letter != 'R'));
            break;
          case 'S':
            if (MsgEntryType == MSG_ENTRY && roomBuf.rbflags.SHARED &&
                cfg.BoolFlags.netParticipant &&
                loggedIn &&
                strLen(msgBuf.mbaddr) == 0 &&
                logBuf.lbflags.NET_PRIVS)
              {
                if (getYesNo("SVNETM"))
                  if (!netInfo(TRUE))
                    break;

              }
            if (MsgEntryType == MSG_ENTRY && roomBuf.rbflags.ANON && loggedIn)
              {
                if (!getYesNo("SVANON"))
                  {
                    msgBuf.mbdate[0] = 0;
                    strCpy(msgBuf.mbauth, logBuf.lbname);

                  }

              }
            return TRUE;
          case 'H':
            if (heldMess)
              {
        Output_Citadel_Message("MSGHLD", NULL, NULL, NULL);
        break;
              }
            Output_Citadel_Message("HOLDIT", NULL, NULL, NULL);
            MoveMsgBuffer(&tempMess, &msgBuf);
            heldMess = TRUE;
            return FALSE;
          case '?':
            if ((MsgEntryType == MSG_ENTRY && roomBuf.rbflags.SHARED || thisRoom == MAILROOM)
                && cfg.BoolFlags.netParticipant &&
                loggedIn &&
                logBuf.lbflags.NET_PRIVS)
              mPrintf(" <N>et toggle\n ");
            if (cfg.BoolFlags.SysopEditor && onConsole)
              mPrintf(" <O>utside Editor\n ");
            if (MsgEntryType == MSG_ENTRY && thisRoom == MAILROOM && loggedIn)
              mPrintf(" <W>ho else (Carbon Copies)\n ");
            ShowOutsideEditors();
            break;
          case 'O':
            OutsideEditor();
            break;
          case 'N':
            if (msgBuf.mbaddr[0])
              {
                c = msgBuf.mbaddr[0];   /* need to remember this */
                msgBuf.mbaddr[0] = 0;   /* zero for getRecipient */
                mPrintf("\bormal message\n ");
                oldRec = strdup(msgBuf.mbto);   /* getRec might zero mbto */
                if (getRecipient())
                  {
                    msgBuf.mboname[0] = 0;
                    msgBuf.mbdomain[0] = 0;

                  }
                else
                  {
                    msgBuf.mbaddr[0] = c;       /* failed, restore address */
                    strCpy(msgBuf.mbto, oldRec);        /** restore recip        */

                  }
                free(oldRec);

              }
            else
              {
                mPrintf("\betwork message\n ");
                netInfo(TRUE);

              }
            break;
          default:
            RunRemoteEditor(letter);

        }

    }
  while (onLine());
  if (MsgEntryType == MSG_ENTRY && loggedIn)
    SaveInterrupted(&msgBuf);
  return FALSE;

}
/*
 * * CCAddDelete() * * This function handles adding or deleting Other
 * Recipients.
 */
int
CCAddDelete(char *name)
{
  label     person;
  char      buf[CC_SIZE],
            isdomain;
  char      system[(2 * NAMESIZE) + 10],
           *domain;
  int       result,
            cost;

  result = SepNameSystem(name, person, system, &netBuf);
  strCpy(buf, name);
  if (result == IS_SYSTEM)
    sPrintf(buf, "%s @%s", person, system);
  isdomain = (domain = strchr(system, '_')) != NULL;
  if (isdomain)
    domain += 2;        /*
                         * formats to "system _ domain", so we cheat
                         */
  if (!CCAddFlag)
    {
 /*
  * Take it off the list
  */
      KillData(&msgBuf.mbCC, buf);

    }
  else
    {
      switch (result)
        {
          case BAD_FORMAT:
      Output_Citadel_Message("NOTFND", (long)name , NULL, NULL);
            break;
          case SYSTEM_IS_US:
            break;      /*
                         * error message handled in SepNameSystem
                         */
          case NO_SYSTEM:
      Output_Citadel_Message("NOTFND", (long)name , NULL, NULL);
            break;
          case IS_SYSTEM:
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
                    break;

                  }

              }
            else if (!logBuf.lbflags.NET_PRIVS)
              {
        Output_Citadel_Message("NONETP", NULL, NULL, NULL);
        break;

              }
            AddData(&msgBuf.mbCC, strdup(buf), NULL, TRUE);
            break;
          case NOT_SYSTEM:
            if (PersonExists(name) == ERROR)
        Output_Citadel_Message("NOTFND", (long)name , NULL, NULL);
            else
              {
                if (strCmpU(name, logBuf.lbname) == SAMESTRING)
       Output_Citadel_Message("NOSELF", NULL, NULL, NULL);
                else if (!msgBuf.mbaddr[0] &&
                         strCmpU(name, msgBuf.mbto) == SAMESTRING)
       Output_Citadel_Message("NODUPT", NULL, NULL, NULL);
                else
                  AddData(&msgBuf.mbCC, strdup(name), NULL, TRUE);

              }
            break;

        }

    }
  return TRUE;

}
/*
 * * SepNameSystem() * * This will parse an Other Recipient spec.
 */
char
SepNameSystem(char *string, char *person, char *system, NetBuffer * buf)
{
  char     *c;
  label     domain;
  char      dup,
            work[150];            /*

                                   *
                                   * * should be sufficient
                                   */
  int       slot;

  strCpy(work, string);
  if (cfg.BoolFlags.debug)
    splitF(NULL, " SepNameSystem:%s\n", string);
  if ((c = strchr(work, '@')) == NULL)
    {
      if (strLen(work) >= NAMESIZE)
        return BAD_FORMAT;
      strCpy(person, string);
      return (char)NOT_SYSTEM;

    }
  *c++ = 0;
  NormStr(work);
  NormStr(c);
  if (cfg.BoolFlags.debug)
    splitF(NULL, "        person:%s\n", work);
  if (cfg.BoolFlags.debug)
    splitF(NULL, "        System:%s\n", c);
  if (strLen(c) >= NAMESIZE * 2 || strLen(work) >= NAMESIZE)
    return (char)BAD_FORMAT;
  strCpy(system, c);
  strCpy(person, work);
  if (buf == NULL)
    return (char)IS_SYSTEM;     /* very minor cheat - see CTDL.C */
  if (cfg.BoolFlags.debug)
    splitF(NULL, "    Search for:%s\n", system);
  if ((slot = searchNameNet(c, buf)) != ERROR)
    {
 /*
  * try secondary lists
  */
      strCpy(system, buf->netName);     /*
                                         * get "real" name
                                         */
      if (cfg.BoolFlags.debug)
        splitF(NULL, "       found:%s\n", system);
      if (buf->nbflags.local || buf->nbflags.RouteLock)
        {
          return (char)IS_SYSTEM;

        }

    }
  if (cfg.BoolFlags.debug)
    splitF(NULL, "  Still Search:%s\n", system);
  if (SystemInSecondary(c, domain, &dup))
    {
      if (dup)
        {
     /*
      * oops
      */
          if (slot == ERROR)
       Output_Citadel_Message("DUPDOM", (long)c, NULL, NULL);
          return (char) ((slot == ERROR) ? NO_SYSTEM : IS_SYSTEM);

        }
      if (strCmpU(domain, cfg.nodeDomain + cfg.codeBuf) == SAMESTRING &&
          (strCmpU(c, cfg.nodeName + cfg.codeBuf) == SAMESTRING ||
           strCmpU(c, UseNetAlias(cfg.nodeName + cfg.codeBuf, TRUE))
           == SAMESTRING))
        {
          mPrintf("Hey, that's this system!\n ");
          return (char) SYSTEM_IS_US;

        }
      sPrintf(system, "%s _ %s", c, domain);
      return (char)IS_SYSTEM;

    }
  if (cfg.BoolFlags.debug)
    splitF(NULL, "(%d)= SepNameSystem(%s,%s,%s,%x)\n ", slot, string, person, system, buf);
  return (char)((slot == ERROR) ? NO_SYSTEM : IS_SYSTEM);

}
/*
 * * findRoom() * * This function finds and returns the # of a free room
 * slot if possible, * else ERROR.
 */
int
findRoom()
{
  int       roomRover;

  for (roomRover = 0; roomRover < MAXROOMS; roomRover++)
    {
      if (roomTab[roomRover].rtflags.INUSE == 0)
        return roomRover;

    }
  return ERROR;

}
/*
 * * getNumber() * * This prompts for a number in (bottom, top) range.
 */
long
getNumber(char *prompt, long bottom, long top)
{
  long      try;
  char      numstring[NAMESIZE];

  do
    {
      if (!gotCarrier())
        {
          haveCarrier = FALSE;
        };
      Output_Citadel_Message(prompt,NULL, NULL, NULL);
      getString("", numstring, NAMESIZE, 0);
      try = atol(numstring);
      if( try < bottom || try > top)
        Output_Citadel_Message("RANGEV",bottom, top, NULL);
    }
  while ((try < bottom || try > top) && onLine());
  return (long) try;

}
/*
 * * getString() * * This gets a string from the user.
 */
int
getString(char *prompt, char *buf, int lim, int Flags)
{
  int       toReturn;
  extern int crtColumn;
  char      oldEcho;

  outFlag = IMPERVIOUS;
  if( prompt != NULL )
    {
    if (strLen(prompt) > 0)
      {
      doCR();
      Output_Citadel_Message(prompt,NULL,NULL,NULL);
      };
    };
  oldEcho = echo;
  if (Flags & NO_ECHO)
    {
      echo = NEITHER;
      echoChar = 'X';

    }
  outFlag = OUTOK;
  Flags |= CR_ON_ABORT;
  if ((toReturn = BlindString(buf, lim, Flags, iChar, oChar, 0)) == BACKED_OUT)
    return toReturn;
  echo = oldEcho;
  crtColumn = 1;
  return toReturn;

}
/*
 * * BlindString() * * Gets a string blind to source and sink.
 */
int
BlindString(char *buf, int lim, int Flags,
            char      (*input) (void), void (*output) (char c), char Echo)
{
  char      c;
  int       i;

  i = 0;
  while (onLine() && (c = (*input) (), c != NEWLINE && c != '\r' && i < lim))
    {
      if (!gotCarrier())
        {
          haveCarrier = FALSE;
        };
      if (Echo)
        (*output) (c);
 /*
  * handle delete chars:
  */
      if (c == BACKSPACE)
        {
          (*output) (' ');
          (*output) (BACKSPACE);
          if (i > 0)
            i--;
          else if (Flags & BS_VALID)
            {
              return BACKED_OUT;

            }
          else
            {
              (*output) (' ');
              (*output) (BELL);

            }

        }
      else if (c)
        buf[i++] = c;
      if (i >= lim)
        {
          (*output) (BELL);
          (*output) (BACKSPACE);
          i--;

        }
 /*
  * kludge to return immediately on single '?':
  */
      if ((Flags & QUEST_SPECIAL) && *buf == exChar)
        {
          if ((Flags & CR_ON_ABORT))
            doCR();
     /*
      * i--;
      */
          break;

        }

    }
  buf[i] = '\0';
  return GOOD_SELECT;

}
/*
 * * getText() * * This manages reading a message from the user. * Returns
 * TRUE if user decides to save it, else FALSE (whether held or * aborted).
 */
char
getText(int uploading)
{
  extern PROTO_TABLE Table[];
  extern char *R_SH_MARK,
            EndWithCR;
  int       i,
            toReturn;

/*
 * msgBuf.mbtext[-1] = NEWLINE;
 */
  if (uploading == ASCII)
    {
    Output_Citadel_Message("UPLDAS", NULL, NULL, NULL);
      outFlag = OUTOK;
      if (msgBuf.mbtext[0])
        CleanEnd(msgBuf.mbtext);
      EndWithCR = FALSE;
      printMessage(TRUE);
      EndWithCR = TRUE;
      outFlag = OUTOK;

    }
  else
    {
      if (!expert)
        tutorial(Table[uploading].UpBlbName, TRUE);
      if (!getYesNo("RDYBEG"))
        return FALSE;
      masterCount = 0;
      if (uploading <= TOP_PROTOCOL)
        {
          if (Reception(uploading, putBufChar) != TRAN_SUCCESS)
            return FALSE;

        }
      else
        {
          if (EatExtMessage(uploading) != TRAN_SUCCESS)
            return FALSE;

        }

    }
  toReturn = GetBalance(uploading, msgBuf.mbtext, MAXTEXT);
  if (toReturn == TRUE)
    {
 /*
  * Filter null messages
  */
      toReturn = FALSE;
      for (i = 0; msgBuf.mbtext[i] != 0 && !toReturn; i++)
        toReturn = (msgBuf.mbtext[i] > ' ' && msgBuf.mbtext[i] < 127);

    }
  return (char) toReturn;

}
/*
 * * GetBalance() * * This function gets the balance of text for a message.
 */
char
GetBalance(int uploading, char *buf, int size)
{
  char      c,
            beeped = FALSE;
  int       i,
            toReturn;

  do
    {
      i = strLen(buf);
      if (uploading == ASCII)
        while (
                !(
                   (c = iChar()) == NEWLINE &&
                   (i == 0 || buf[i - 1] == NEWLINE)
                )
                && i < size - 1
                && onLine()
          )
          {
            if (c != BACKSPACE)
              {
                if (c == ESC)
                  {
                    if (iChar() == '[')
                      {
                        iChar();
                        continue;       /*
                                         * arrow key.
                                         */

                      }

                  }
                if (c != 0 && c != XOFF && c != XON)
                  buf[i++] = c;
                if (i > size - 80 && !beeped)
                  {
                    beeped = TRUE;
                    oChar(BELL);

                  }

              }
            else
              {
           /*
            * handle delete chars:
            */
                oChar(' ');
                oChar(BACKSPACE);
                if (i > 0 && buf[i - 1] != NEWLINE)
                  i--;
                else
                  oChar(BELL);

              }
            buf[i] = 0;         /*
                                 * null to terminate message
                                 */
            if (i == size - 1)
              {
                mPrintf(" buffer overflow\n ");
                while (receive(2) != ERROR)
                  ;

              }

          }
      if (i == size - 1)
        buf[--i] = 0;   /*
                         * fixes an odd buffer overflow problem
                         */
 /*
  * couldn't edit the message otherwise
  */
      toReturn = editText(buf, size - 1);
      uploading = ASCII;

    }
  while ((toReturn == ERROR) && onLine());
  return (char) toReturn;

}
/*
 * * getYesNo() * * This prompts for a yes/no response and gets it.
 */
char
getYesNo(char *prompt)
{
  int       toReturn;
  extern char ConOnly;
  char      (*input) (void);

  input = ConOnly ? getCh : iChar;
  for (doCR(), toReturn = ERROR; toReturn == ERROR && onLine();)
    {
      outFlag = IMPERVIOUS;
      Output_Citadel_Message(prompt,NULL,NULL,NULL);
      switch (toUpper((*input) ()))
        {
          case 'Y':
            toReturn = TRUE;
            break;
          case 'N':
            toReturn = FALSE;
            break;
          case '\0':
            toReturn = TRUE;
            break;

        };
      doCR();

    }
  outFlag = OUTOK;
  return (char) toReturn;

}
/*
 * * givePrompt() * * This function simply prints the usual "CURRENTROOM>"
 * prompt -- not as simple * as it may seem.
 */
void
givePrompt()
{
  outFlag = IMPERVIOUS;
  doCR();
  ScreenUser();
  if (!loggedIn)
    Output_Citadel_Message("NOTLGN", NULL, NULL, NULL);
  Output_Citadel_Message("PROMPT"
         ,(long)( (thisRoom == MAILROOM || loggedIn ||cfg.BoolFlags.unlogReadOk) ?
             "<N>ew messages" : " ")
         ,(long)( (thisRoom == MAILROOM || loggedIn ||cfg.BoolFlags.unlogEnterOk)?
             "<E>nter" : " " )
         , NULL);

  if (!expert) doCR();
  if (roomBuf.rbflags.READ_ONLY)
    {
    Output_Citadel_Message("READOL", NULL, NULL, NULL);
    doCR();

    }
  mPrintf("%s ", formRoom(thisRoom, FALSE, TRUE));
  if (strCmp(roomBuf.rbname, roomTab[thisRoom].rtname) != SAMESTRING)
    {
      printf("thisRoom=%d, rbname=-%s-, rtname=-%s-\n", thisRoom,
             roomBuf.rbname, roomTab[thisRoom].rtname);
      crashout("Dependent variables mismatch!");

    }
  outFlag = OUTOK;

}
/*
 * * indexRooms() * * This function will try to find and free an empty room.
 */
void
indexRooms()
{
  int       goodRoom,
            slot;

  for (slot = 0; slot < MAXROOMS; slot++)
    {
      if (roomTab[slot].rtflags.INUSE == 1)
        {
          goodRoom = FALSE;
          if (roomTab[slot].rtlastMessage > cfg.oldest ||
              roomTab[slot].rtflags.PERMROOM == 1)
            {
              goodRoom = TRUE;

            }
          if (!goodRoom)
            {
              getRoom(slot);
              KillInfo(roomTab[slot].rtname);
              roomBuf.rbflags.INUSE = 0;
              putRoom(slot);
              strCat(msgBuf.mbtext, roomBuf.rbname);
              strCat(msgBuf.mbtext, "> ");
              noteRoom();

            }

        }

    }

}

/*
 * * insertParagraph() * * This inserts a paragraph (CR/Space) into a
 * message. * (By Jay Johnson of The Phoenix)
 */
void
insertParagraph(char *buf, int lim)
{
  char      oldString[2 * SECTSIZE];
  char     *loc,
           *textEnd;
  char     *pc;
  int       length;

  for (textEnd = buf, length = 0; *textEnd; length++, textEnd++) ;
  if (lim - length < 3)
    {
    Output_Citadel_Message("NORMMG", NULL, NULL, NULL);
    return;

    }
  getString("GETPAR", oldString, (2 * SECTSIZE), 0);
  if ((loc = matchString(buf, oldString, textEnd)) == NULL)
    {
    Output_Citadel_Message("NOTFND", NULL, NULL, NULL);
    return;

    }
  for (pc = textEnd; pc >= loc; pc--)
    {
      *(pc + 2) = *pc;

    }
  *loc++ = '\n';
  *loc = ' ';

}
/*
 * * makeRoom() * * This function constructs a new room via dialogue with
 * user.
 */
void
makeRoom()
{
  label     nm,
            oldName;
  int       CurrentFloor,
            oldRoom;

  CurrentFloor = thisFloor;
  oldRoom = thisRoom;
/*
 * update lastMessage for current room:
 */
  logBuf.lbgen[thisRoom] = roomBuf.rbgen << GENSHIFT;
  strCpy(oldName, roomBuf.rbname);
  if ((thisRoom = findRoom()) == ERROR)
    {
      indexRooms();     /*
                         * try and reclaim an empty room
                         */
      if ((thisRoom = findRoom()) == ERROR)
        {
    Output_Citadel_Message("NORMMG", NULL, NULL, NULL);
     /*
      * may have reclaimed old room, so:
      */
          if (roomExists(oldName) == ERROR)
            strCpy(oldName, baseRoom);
          getRoom(roomExists(oldName));
          return;

        }

    }
  getNormStr("ROOMNM", nm, NAMESIZE, 0);
  if (strLen(nm) == 0)
    {
      if (roomExists(oldName) == ERROR)
        strCpy(oldName, baseRoom);
      getRoom(roomExists(oldName));
      return;

    }
  if (roomExists(nm) >= 0)
    {
    Output_Citadel_Message("ALRDYE", (long)nm, NULL, NULL);
 /*
  * may have reclaimed old room, so:
  */
      if (roomExists(oldName) == ERROR)
        strCpy(oldName, baseRoom);
      getRoom(roomExists(oldName));
      return;

    }
  if (!expert)
    tutorial("newroom.blb", TRUE);
  zero_struct(roomBuf.rbflags);
  roomBuf.rbflags.INUSE = TRUE;
  if (getYesNo("MKRMPD"))
    roomBuf.rbflags.PUBLIC = TRUE;
  else
    roomBuf.rbflags.PUBLIC = FALSE;
  mPrintf("'%s', a %s room", nm,
          roomBuf.rbflags.PUBLIC == 1 ? "public" : "private"
    );
  if (!getYesNo("MKRMIS"))
    {
 /*
  * may have reclaimed old room, so:
  */
      if (roomExists(oldName) == ERROR)
        strCpy(oldName, baseRoom);
      getRoom(roomExists(oldName));
      return;

    }
  else if (roomExists(oldName) == ERROR)
    oldRoom = -1;       /*
                         * in case
                         */
  strCpy(roomBuf.rbname, nm);
  memset(roomBuf.msg, 0, MSG_BULK);     /*
                                         * mark all slots empty
                                         */
  roomBuf.rbgen = (roomTab[thisRoom].rtgen + 1) % MAXGEN;
  roomBuf.rbFlIndex = CurrentFloor;
  KillData(&Moderators, NtoStrInit(thisRoom, "", 0, TRUE));
  WriteAList(&Moderators, "ctdlmodr.sys", WrtNtoStr);
  KillData(&Arch_base, NtoStrInit(thisRoom, "", 0, TRUE));
  WriteAList(&Arch_base, "ctdlarch.sys", WrtArchRec);
  noteRoom();   /*
                 * index new room
                 */
  RoomSys(thisRoom);    /*
                         * initialize directory area
                         */
  putRoom(thisRoom);
  ZeroMsgBuffer(&msgBuf);
  Output_Citadel_Message("ENTINF",(long)roomBuf.rbname, NULL, NULL);
  EditInfo();   /*
                 * creator gets to set information
                 */
  msgBuf.mboname[0] = 0;        /*
                                 * icky kludge
                                 */
  if (strLen(msgBuf.mbtext) != 0 &&
      getYesNo("MKINFO"))
    procMessage(ASCII, FALSE);
/*
 * update logBuf:
 */
  logBuf.lbgen[thisRoom] = roomBuf.rbgen << GENSHIFT;
  ZeroMsgBuffer(&msgBuf);
  sPrintf(msgBuf.mbtext, "%s created by %s.", formRoom(thisRoom, FALSE, FALSE),
          logBuf.lbname);
  aideMessage(NULL, FALSE);
  roomTab[thisRoom].rtlastNet = 0l;
  if (oldRoom != -1)
    UngotoMaintain(oldRoom);

}
/*
 * * WriteAList() * * This writes a Num->String list to disk.
 */
void
WriteAList(SListBase * base, char *fn, void (*func) ())
{
  SYS_FILE  name;
  extern FILE *upfd;

  makeSysName(name, fn, &cfg.roomArea);
  if ((upfd = safeopen(name, WRITE_TEXT)) != NULL)
    {
      RunList(base, func);
      fclose(upfd);

    }
  else
    Output_Citadel_Message("NOOPEN", (long)name, NULL, NULL);
}
/*
 * * matchString() * * This searches for match to given string.  Runs
 * backward through buffer so * we get most recent error first.  Returns loc
 * of match, else NULL.
 */
char     *
matchString(char *buf, char *pattern, char *bufEnd)
{
  char     *loc,
           *pc1,
           *pc2;
  char      foundIt;

  for (loc = bufEnd, foundIt = FALSE; !foundIt && --loc >= buf;)
    {
      for (pc1 = pattern, pc2 = loc, foundIt = TRUE; *pc1 && foundIt;)
        {
          if (!(toUpper(*pc1++) == toUpper(*pc2++)))
            foundIt = FALSE;

        }

    }
  return foundIt ? loc : NULL;

}
/*
 * * getNormStr() * * This function gets a string and deletes leading &
 * trailing blanks etc.
 */
int
getNormStr(char *prompt, char *s, int size, int Flags)
{
  int       toReturn;

  if ((toReturn = getString(prompt, s, size, Flags)) != BACKED_OUT)
    if (*s != exChar && *s != '\0')
      NormStr(s);
  return toReturn;

}
/*
 * * noteRoom() * * This will enter a room into RAM index array.
 */
void
noteRoom()
{
  int       i;
  MSG_NUMBER last;

  last = 0l;
#ifdef NORMAL_MESSAGES
  for (i = 0; i < ((thisRoom == MAILROOM) ? MAILSLOTS : MSGSPERRM); i++)
    {
      if (roomBuf.msg[i].rbmsgNo > last &&
          roomBuf.msg[i].rbmsgNo <= cfg.newest)
        {
          last = roomBuf.msg[i].rbmsgNo;

        }

    }
#else
  for (i = 0; i < ((thisRoom == MAILROOM) ? MAILSLOTS : MSGSPERRM); i++)
    {
      if ((roomBuf.msg[i].rbmsgNo & (~S_MSG_MASK)) &&
          (roomBuf.msg[i].rbmsgNo & S_MSG_MASK) > cfg.oldest)
        last = S_MSG_MASK;
      if ((roomBuf.msg[i].rbmsgNo & S_MSG_MASK) > last &&
          (roomBuf.msg[i].rbmsgNo & S_MSG_MASK) <= cfg.newest)
        {
          last = (roomBuf.msg[i].rbmsgNo & S_MSG_MASK);

        }

    }
#endif
  roomTab[thisRoom].rtlastMessage = last;
  strCpy(roomTab[thisRoom].rtname, roomBuf.rbname);
  roomTab[thisRoom].rtgen = roomBuf.rbgen;
  memcpy(&roomTab[thisRoom].rtflags, &roomBuf.rbflags,
        (long)sizeof roomBuf.rbflags);
  roomTab[thisRoom].rtShareType = roomBuf.rbShareType;
  roomTab[thisRoom].rtFlIndex = roomBuf.rbFlIndex;

}
char      DoWritePrivs;

/*
 * * renameRoom() * * This is a sysop special fn -- it handles all room
 * editing. * Returns:  TRUE on success else FALSE.
 */
char
renameRoom()
{
  int       c,
            r;
  char     *buffer,
            wasRO,
            wasShared,
            wasAnon,
           *save,
            workbuf[200];
  extern char *APrivateRoom;
  char      doAideMessage = 0;    /*

                                   *
                                   * * Counter to determine if aide msg
                                   * needed
                                   */
  extern char *WRITE_TEXT;
  extern FILE *upfd;
  char     *RoomEditOpts[] =
  {
    "X\bExit room editing\n", "Name change\n", "Temporary room\n",
    "Private setting\n", "Lure users to room\n", "Only Invitational\n",
    "Innominate status\n", "Values\n", "Withdraw Invitations\n",
    "Edit information\n", "Read only\n",
    " ", " ", " ", " ", " ", " ", " ", " ", ""

  };

  if (thisRoom == LOBBY || thisRoom == MAILROOM || thisRoom == AIDEROOM)
    {
    Output_Citadel_Message("WARNSP", NULL, NULL, NULL);
    if (!getYesNo("ALLOWS"))
      return FALSE;

    };
  wasShared = roomBuf.rbflags.SHARED;
  wasAnon = roomBuf.rbflags.ANON;
  wasRO = roomBuf.rbflags.READ_ONLY;
  ZeroMsgBuffer(&msgBuf);
  sPrintf(msgBuf.mbtext, "%s, formerly ", formRoom(thisRoom, FALSE, FALSE));
  formatSummary(lbyte(msgBuf.mbtext), FALSE);
  buffer = strdup(msgBuf.mbtext);
  if (HalfSysop())
    {
      ExtraOption(RoomEditOpts, "Archive status\n");
      ExtraOption(RoomEditOpts, "Directory status\n");
      if (cfg.BoolFlags.netParticipant)
        {
          ExtraOption(RoomEditOpts, "Backbone setting\n");
          ExtraOption(RoomEditOpts, "Shared room\n");

        }
      if (roomBuf.rbflags.ISDIR)
        {
          ExtraOption(RoomEditOpts, "U");

        }
      if (cfg.BoolFlags.netParticipant && roomBuf.rbflags.ISDIR)
        {
          ExtraOption(RoomEditOpts, "Z\bNetwork Downloadable\n");

        }

    }
  if (aide)
    ExtraOption(RoomEditOpts, "Moderator setting\n");
  RegisterThisMenu(HalfSysop()? "rooms.mnu" : "rooma.mnu", RoomEditOpts);
  c = 0;        /*
                 * Init
                 */
  while (c != 'X' && onLine())
    {
      mPrintf("\n Room Editing Command: ");
      c = GetMenuChar();
      switch (c)
        {
          case 'X':
            break;
          case 'A':
            doAideMessage++;
            roomBuf.rbflags.ARCHIVE = getYesNo("ACTARC");
            if (roomBuf.rbflags.ARCHIVE)
              {
                getString("EFILEN", workbuf, (sizeof workbuf) - 1, 0);
                if (strLen(workbuf) == 0)
                  {
                    roomBuf.rbflags.ARCHIVE = FALSE;
                    break;

                  }
                c = (int) getNumber("ROLLOV", -1, 32000);
                AddData(&Arch_base, NtoStrInit(thisRoom, workbuf, c, FALSE),
                        NULL, TRUE      /*
                                         * kill duplicates
                                         */ );
                WriteAList(&Arch_base, "ctdlarch.sys", WrtArchRec);
                if (getYesNo("INITAR"))
                  initialArchive(workbuf);

              }
            break;
          case 'N':
            getNormStr("ROOMNM", workbuf, NAMESIZE, 0);
            r = roomExists(workbuf);
            if (r >= 0 && r != thisRoom)
              {
                mPrintf("A %s exists already!\n", workbuf);
                break;

              }
            else
              {
                ChangeInfoName(workbuf);
                strCpy(roomBuf.rbname, workbuf);        /*
                                                         * also in room
                                                         * itself
                                                         */
                doAideMessage++;

              }
            if (!getYesNo("EDIFRM"))
              break;
          case 'E':
            save = strdup(msgBuf.mbtext);
            EditInfo();
            RegisterThisMenu(SomeSysop()? "rooms.mnu" : "rooma.mnu", RoomEditOpts);
            strcpy(msgBuf.mbtext, save);
            free(save);
            break;
          case 'B':
            if (!roomBuf.rbflags.SHARED)
              {
        Output_Citadel_Message("NOTSHR", NULL, NULL, NULL);
                    break;

              }
            roomBuf.rbShareType = (getYesNo("SYSBKN")) ?
              BACKBONE : PEON;
            if (roomBuf.rbShareType == BACKBONE)
              {
                doCR();
                ShType = ACTIVE_BACKBONE;
                getList(knownHosts,
                        "Systems that you will be an Active Backbone for",NAMESIZE, FALSE);
                ShType = PASS_BACKBONE;
                getList(knownHosts,
                        "Systems that you will be a Passive Backbone for",NAMESIZE, FALSE);
                ShType = PEON;
                getList(knownHosts,
                        "Systems that should be returned to Peon status",NAMESIZE, FALSE);

              }
            break;
          case 'M':
            if (WhoIsModerator(workbuf))
              {
                if (strLen(workbuf))
                  AddData(&Moderators, NtoStrInit(thisRoom, workbuf, 0,
                                                  FALSE), NULL, TRUE);
                else
                  KillData(&Moderators, NtoStrInit(thisRoom, "", 0, TRUE));

              }
            WriteAList(&Moderators, "ctdlmodr.sys", WrtNtoStr);
            doAideMessage++;
            break;
          case 'D':
            doAideMessage++;
            roomBuf.rbflags.ISDIR = getYesNo("ACTDIR");
            if (roomBuf.rbflags.ISDIR)
              {
                if ((roomBuf.rbflags.ISDIR = getArea(&roomBuf)))
                  roomBuf.rbflags.PERMROOM = TRUE;
                else
                  break;

              }
            else
              break;
          case 'U':
            doAideMessage++;
            if (c == 'U') mPrintf("load/Download room\n ");
            roomBuf.rbflags.UPLOAD   = getYesNo("UPLDAL");
            roomBuf.rbflags.DOWNLOAD = getYesNo("DNLDAL");
            if (!roomBuf.rbflags.UPLOAD && !roomBuf.rbflags.DOWNLOAD)
        Output_Citadel_Message("STRNGE",NULL, NULL, NULL);
            break;
          case 'T':
            if (roomBuf.rbflags.ISDIR)
        Output_Citadel_Message("DIRRMP", NULL, NULL, NULL);
            else
              {
                roomBuf.rbflags.PERMROOM = !getYesNo("RMTEMP");
                doAideMessage++;

              }
            break;
          case 'P':
            doAideMessage++;
            roomBuf.rbflags.PUBLIC = !getYesNo("MKRMPR");
            if (!roomBuf.rbflags.PUBLIC)
              {
                if (getYesNo("CSNAUS"))
                  {
                    if (!wasShared || getYesNo("WRNSHR"))
                      {
                        roomBuf.rbgen = (roomBuf.rbgen + 1) % MAXGEN;
                        logBuf.lbgen[thisRoom] = (logBuf.lbgen[thisRoom]
                                                  & CALLMASK) +
                          (roomBuf.rbgen << GENSHIFT);
                        roomTab[thisRoom].rtgen = roomBuf.rbgen;

                      }

                  }

              }
            break;
          case 'S':
            roomBuf.rbflags.SHARED = getYesNo("NTSHRN");
            if (roomBuf.rbflags.SHARED)
              {
                if (!wasShared)
                  {
                    roomBuf.rbflags.PERMROOM = TRUE;
                    doAideMessage++;
               /*
                * cosmetic bug fix
                */
                    roomTab[thisRoom].rtflags.SHARED = TRUE;

                  }
                getList(addToList, wasShared ?
                        "Systems to add to the network list for this room" :
                  "Systems to network this room with", NAMESIZE, FALSE);
                if (wasShared)
                  getList(killFromList,
                  "Systems to take off the network list", NAMESIZE, FALSE);
                roomBuf.rbflags.AUTO_NET =
                  getYesNo("MSGSNT");
                if (roomBuf.rbflags.AUTO_NET)
                  roomBuf.rbflags.ALL_NET =
                    getYesNo("EVNNET");

              }
            else
              {
                if (wasShared)
                  doAideMessage++;
                if (!roomBuf.rbflags.ISDIR)
                  roomBuf.rbflags.PERMROOM = FALSE;

              }
            break;
          case 'L':
            getList(makeKnown, "Users to be invited", NAMESIZE, FALSE);
            break;
          case 'R':
            if ((roomBuf.rbflags.READ_ONLY = getYesNo("MKREAD")))
              {
                DoWritePrivs = TRUE;
                getList(WritePrivs, "Users needing write privileges",   NAMESIZE, FALSE);
                DoWritePrivs = FALSE;
                getList(WritePrivs, "Users losing write privileges",    NAMESIZE, FALSE);

              }
            if (roomBuf.rbflags.READ_ONLY != wasRO)
              doAideMessage++;
            break;
          case 'O':
            doAideMessage++;
            if ((roomBuf.rbflags.INVITE = getYesNo("MKINVT")))
              {
                roomBuf.rbflags.PUBLIC = FALSE;
                if (getYesNo("CSNAUS"))
                  {
                    if (!wasShared || getYesNo("WRNSHR"))
                      {
                        roomBuf.rbgen = (roomBuf.rbgen + 1) % MAXGEN;
                        logBuf.lbgen[thisRoom] = (logBuf.lbgen[thisRoom] &
                                                  CALLMASK) +
                          (roomBuf.rbgen << GENSHIFT);
                        roomTab[thisRoom].rtgen = roomBuf.rbgen;

                      }

                  }
                getList(makeKnown, "Users to be invited", NAMESIZE, FALSE);

              }
            break;
          case 'I':
            roomBuf.rbflags.ANON = getYesNo("MKANON");
            if (roomBuf.rbflags.ANON != wasAnon)
              doAideMessage++;
            break;
          case 'V':
            mPrintf("%s is %s.", roomBuf.rbname,
                    formatSummary(msgBuf.mbtext, TRUE));
            break;
          case 'W':
            getList(makeUnknown, "Users to be kicked out", NAMESIZE, FALSE);
            break;
          case 'Z':
            doAideMessage++;
            roomBuf.rbflags.NO_NET_DOWNLOAD =
              !getYesNo("ALOWNT");
            break;

        }

    }
  noteRoom();
  putRoom(thisRoom);
  if (doAideMessage)
    {
      ZeroMsgBuffer(&msgBuf);
      sPrintf(msgBuf.mbtext, "%s, has been edited to %s, ", buffer,
              formRoom(thisRoom, FALSE, FALSE));
      formatSummary(lbyte(msgBuf.mbtext), FALSE);
      sPrintf(lbyte(msgBuf.mbtext), ", by %s.", logBuf.lbname);
      aideMessage(NULL, FALSE);

    }
  free(buffer);
  return TRUE;

}
/*
 * * WhoIsModerator() * * This handles adding moderating-type people.
 */
char
WhoIsModerator(char *buf)
{
  if (!getXString("MODRTR", buf, NAMESIZE, "no moderator", ""))
    return FALSE;
  if (strLen(buf) != 0)
    if (findPerson(buf, &logTmp) == ERROR)
      {
      Output_Citadel_Message("NOTFND", (long)buf, NULL, NULL);
      return FALSE;

      }
  return TRUE;

}
/*
 * * formatSummary() * * This is responsible for formatting a summary of the
 * current room.
 */
char     *
formatSummary(char *buffer, char NotFinal)
{
  char     *c;
  int       size;

  sPrintf(buffer, "a%s, ",
          roomBuf.rbflags.INVITE ? "n Invitation only" :
          roomBuf.rbflags.PUBLIC ? public_str : private_str);
  sPrintf(lbyte(buffer), "%s",
          roomBuf.rbflags.PERMROOM ? perm_str : temp_str);
  if (roomBuf.rbflags.ARCHIVE)
    {
      sPrintf(lbyte(buffer), ", Archived ('%s', ",
              AskForNSMap(&Arch_base, thisRoom));
      if ((size = GetArchSize(thisRoom)) == 0)
        strCat(buffer, "no rollover)");
      else
        sPrintf(lbyte(buffer), "rollover at %dK)", size);

    }
  if (roomBuf.rbflags.ANON)
    strCat(buffer, ", Anonymous");
  if (roomBuf.rbflags.READ_ONLY)
    strCat(buffer, ", Read-Only");
  if (roomBuf.rbflags.SHARED)
    {
      strCat(buffer, ", Shared");
      if (roomBuf.rbflags.AUTO_NET)
        {
          strCat(buffer, " (autonet for ");
          strCat(buffer, roomBuf.rbflags.ALL_NET ? "all users)" :
                 "net-priv users)");

        }

    }
  if (roomBuf.rbflags.ISDIR)
    {
      strCat(buffer, ", Directory (");
      if (NotFinal || !(roomBuf.rbflags.INVITE || !roomBuf.rbflags.PUBLIC))
        {
          dirString(lbyte(buffer), &roomBuf.rbArea);
          strCat(buffer, ", ");

        }
      if (roomBuf.rbflags.UPLOAD)
        strCat(buffer, "uploads, ");
      if (roomBuf.rbflags.DOWNLOAD)
        strCat(buffer, "downloads, ");
      sPrintf(lbyte(buffer), "%snet downloadable",
              roomBuf.rbflags.NO_NET_DOWNLOAD ? "not " : "");
      strCat(buffer, ")");

    }
  strCat(buffer, " room");
  if (strLen(c = AskForNSMap(&Moderators, thisRoom)) != 0)
    sPrintf(lbyte(buffer), " (Moderator is %s)", c);
  if (roomBuf.rbflags.SHARED && NotFinal)
    ParticipatingNodes(buffer);
  return buffer;

}
MenuId    GetListId;

/**
  getList()
  This will get a list of names and blindly process them.
**/
void
getList(int (*fn) (char *data), char *prompt, int size, char Sysop)
{
  char     *buffer;
  if (cfg.BoolFlags.debug)
    splitF(NULL, " getList(%08.8lx,%s,%d,%s)\n",fn,prompt,size, (Sysop) ? "TRUE" : "FALSE");
  buffer = GetDynamic(size + 1);
  GetListId = NO_MENU;
  mPrintf("\n %s (Empty line to end)...\n", prompt);
  do
    {
      if (Sysop)  SysopPrintf(GetListId, " : ");
      else        mPrintf(" : ");
      SysopContinualString(GetListId, "", buffer, size, 0);
      if (strLen(buffer) != 0) if (!(*fn) (buffer)) break;

    }
  while (strLen(buffer) != 0);
  if (Sysop)
    SysopCloseContinual(GetListId);
  free(buffer);
}
/*
 * * replaceString() * * This function corrects typos in message entry.
 */
#define REPLACE_SIZE    2000
void
replaceString(char *buf, int lim, char Global)
{
  char      oldString[2 * SECTSIZE];
  char     *newString;            /*

                                   *
                                   * * Eliminate stack overflows in Turbo c
                                   */
  char     *loc,
           *oldloc,
           *textEnd;
  char     *pc;
  int       incr,
            length,
            oldLen,
            newLen,
            maxLen;

/*
 * find terminal null
 */
  textEnd = lbyte(buf);
  length = strLen(buf);
  getString("GETSTR", oldString, (2 * SECTSIZE), 0);
  if ((oldLen = strLen(oldString)) == 0)
    return;
  if ((oldloc = loc = matchString(buf, oldString, textEnd)) == NULL)
    {
      mPrintf("Not found...\n ");
      return;

    }
/*
 * so we never have too long of a replacement string (unless Global R.)
 */
  maxLen = minimum(REPLACE_SIZE, (MAXTEXT - (length - oldLen)));
  newString = GetDynamic(REPLACE_SIZE);
  getString("GETREP", newString, maxLen, 0);
  newLen = strLen(newString);
  do
    {
      textEnd = lbyte(buf);
      length = textEnd - buf;
      if ((newLen - oldLen) >= lim - length)
        {
          mPrintf("Overflow!!\n ");
          free(newString);
          return;

        }
 /*
  * delete old string:
  */
      for (pc = loc,
           incr = strLen(oldString);
           *pc = *(pc + incr);  /*
                                 * Compiler generates a warning for this
                                 * line
                                 */
           pc++) ;
      textEnd -= incr;
 /*
  * make room for new string:
  */
      for (pc = textEnd, incr = strLen(newString); pc >= loc; pc--)
        {
          *(pc + incr) = *pc;

        }
 /*
  * insert new string:
  */
      for (pc = newString; *pc; *loc++ = *pc++) ;
      if (Global)
        oldloc = loc = matchString(buf, oldString, oldloc);

    }
  while (Global && oldloc != NULL);
  free(newString);

}
/*
 * * initialArchive() * * This function is responsible for doing an initial
 * archive of a room.
 */
void
initialArchive(char *fn)
{
  char     *TmpMsg,
           *realfn;

  TmpMsg = strdup(msgBuf.mbtext);
  realfn = GetDynamic(strLen(fn) + 15);
  TranslateFilename(realfn, fn);
  mPrintf("Doing the initial archive...\n");
  msgToDisk(realfn, TRUE, 0l, 0l, 0);
  strCpy(msgBuf.mbtext, TmpMsg);
  free(TmpMsg);
  free(realfn);

}
/*
 * * knownHosts() * * This function handles setting systems as hosts.
 */
int
knownHosts(char *name)
{
  int       slot,
            i;

  if ((slot = CmnNetList(name, &i, ERROR, "does not share this room with you"))
      == ERROR)
    return TRUE;
  if (i == ERROR)
    if ((i = ListAsShared(name)) == ERROR)
      return TRUE;
  SetMode(netBuf.netRooms[i].mode, ShType);
  putNet(slot, &netBuf);
  return TRUE;

}
/*
 * * addToList() * * This function will Add a system to a room networking
 * list.
 */
int
addToList(char *name)
{
  int       slot,
            i;

  if ((slot = CmnNetList(name, &i, FALSE, "already networks this room with you"))
      != ERROR)
    {
      if (ListAsShared(name) != ERROR)
        {
          putNet(slot, &netBuf);

        }

    }
  return TRUE;

}
/*
 * * ListAsShared() * * This function will find an empty share slot.
 */
int
ListAsShared(char *name)
{
  int       i,
            temp,
            gen;

  for (i = 0; i < SHARED_ROOMS; i++)
    {
      if ((netBuf.netRooms[i].srgen & 0x8000) != 0)
        {
     /*
      * Salvage attempt
      */
          temp = netBuf.netRooms[i].srslot & 0x7fff;
          gen = netBuf.netRooms[i].srgen & 0x7fff;
          if (roomTab[temp].rtgen != gen ||     /*
                                                 * No longer exists!
                                                 */
              roomTab[temp].rtflags.SHARED == 0 ||      /*
                                                         * Not netting
                                                         */
              !roomTab[temp].rtflags.INUSE)
            {
         /*
          * no longer exists
          */
              break;

            }

        }
      else
        break;

    }
  if (i == SHARED_ROOMS)
    {
      mPrintf("Sorry, already sharing %d rooms with %s.\n", SHARED_ROOMS, name);
      return ERROR;

    }
  netBuf.netRooms[i].srslot = thisRoom;
  netBuf.netRooms[i].lastMess = cfg.newest;
  netBuf.netRooms[i].srgen = roomBuf.rbgen + (unsigned) 0x8000;
  SetMode(netBuf.netRooms[i].mode, PEON);
  return i;

}
/*
 * * searchForRoom() * * This function checks to see if the current room is
 * in the current node's * room sharing list.
 */
int
searchForRoom()
{
  RoomSearch data;

  strCpy(data.Room, roomBuf.rbname);
  EachSharedRoom(thisNet, IsRoomRoutable, NULL, &data);
  if (data.reason == FOUND)
    return data.index;
  return ERROR;

}
/*
 * * getXString()
 */
char
getXString(char *prompt, char *target, int targetSize, char *CR_str,
           char *dft)
{
  return getXInternal(NO_MENU, prompt, target, targetSize, CR_str, dft);

}
/*
 * * getXInternal() * * This is a sophisticated GetString().  Will return
 * TRUE and FALSE -- read * the code to figure it out.
 */
char
getXInternal(MenuId id, char *prompt, char *target, int targetSize,
             char *CR_str, char *dft)
{
  if (CR_str != NULL && strLen(CR_str) != 0)
    mPrintf( "C/R = '%s',", CR_str);
  mPrintf("(ESCape to abort)");
  exChar = ESC;
  SysopContinualString(id, prompt, target, targetSize, QUEST_SPECIAL);
  exChar = '?';
  if (!onLine())
    return FALSE;       /*
                         * Lost carrier
                         */
  if (target[0] == ESC)
    return FALSE;
  if (CR_str == NULL && target[0] == 0)
    return FALSE;
  else if (target[0] == 0)
    strCpy(target, dft);
  return TRUE;

}
/*
 * * makeKnown() * * This makes a user knowledgeable about this room.
 */
int
makeKnown(char *user)
{
  return doMakeWork(user, roomBuf.rbgen);

}
/*
 * * makeUnknown() * * This makes a user forget about this room.
 */
int
makeUnknown(char *user)
{
  return doMakeWork(user, (roomBuf.rbgen + (MAXGEN - 1)) % MAXGEN);

}
/*
 * * doMakeWork() * * This is a worker function.
 */
int
doMakeWork(char *user, int val)
{
  int       target;

  if ((target = findPerson(user, &logTmp)) == ERROR)
    mPrintf("'%s' not found.\n", user);
  else
    {
      logTmp.lbgen[thisRoom] = (val << GENSHIFT) + MAXVISIT - 1;
      putLog(&logTmp, target);

    }
  return TRUE;

}
/*
 * * killFromList() * * Kills systems from sharing a room.
 */
int
killFromList(char *sysName)
{
  int       i,
            slot;

  if ((slot = CmnNetList(sysName, &i, TRUE, "does not network this room with you"))
      == ERROR)
    return TRUE;
  netBuf.netRooms[i].srgen = 0;
  putNet(slot, &netBuf);
  return TRUE;

}
/*
 * * CmnNetList() * * This does general work on net stuff.
 */
int
CmnNetList(char *name, int *slot, char ShouldBeThere, char *errstr)
{
  extern int thisNet;

/*
 * This will do required getNet() if it returns true
 */
  if (!ReqNodeName("", name, NULL, FALSE, TRUE, FALSE, FALSE, FALSE, &netBuf))
    return ERROR;
  *slot = searchForRoom();
  if ((*slot == ERROR && ShouldBeThere == TRUE) ||
      (*slot != ERROR && ShouldBeThere == FALSE))
    {
      mPrintf("%s %s.", name, errstr);
      doCR();
      return ERROR;

    }
  return thisNet;

}
/*
 * * WritePrivs() * * Here we give or take away write privs.
 */
int
WritePrivs(char *user)
{
  return doMakeWork(user, (DoWritePrivs) ?
                 (((roomBuf.rbgen + RO_OFFSET)) % MAXGEN) : roomBuf.rbgen);

}
