/*
*       nodedit.c
*
* Networking List Editor.
*/
#include "ctdl.h"

extern CONFIG    cfg;
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

char    *SR_Sent;
char    ErrBuf[100];    /* General buffer for error messages */
FILE    *netLog, *netMisc, *netMsg;
static char     UsedNetMsg;
int   callSlot;
label   normed, callerName, callerId;
logBuffer       *lBuf;
int   PriorityMail = 0;
char    *pollCall;
int   LD_Delay = 60;

int Request_String(char *string, char *buffer, int size);

void ExplainNeed(int i, MULTI_NET_DATA x);
void freeUNS();

SListBase UntilNetSessions =
  {
  NULL, ChkTwoNumbers, NULL, freeUNS, NULL

  };


int Request_String(char *string, char *buffer, int size)
  {
  /**
    Description:  Prompt with "string", and get up to size
    characters into buffer.
    return TRUE if we got something, FALSE if not
  **/
  printf("\n%s",string);
  buffer[0] = '\0';
  fgets(buffer,size, fp);
  return ( strlen(buffer) > 0 ? TRUE : FALSE );
  }

/*
* Main Menu Processing
*/
void netStuff()
  {
  extern char *who_str;
  extern char ForceNet;
  TwoNumbers  tmp;
  char  work[50];
  label       who;
  int   logNo;
  long  Redials, duration;

  int flag;
  printf(" Node Editor Version 1.0\n");
  cfg.weAre = UTILITY;
  if (!readSysTab(TRUE, TRUE))
    {
    printf("Unable to load CTDLTABL.SYS data\n");
    exit(100);
    };
  cfg.weAre = UTILITY;

  if (!cfg.BoolFlags.netParticipant)
    {
    printf( "Networking is disabled on this installation.\n ");
    printf( "You may not use this utility.\n");
    exit(0);
    };
  mvToHomeDisk(&cfg.homeArea);

  /* process network information */
  makeSysName(fn, "ctdlnet.sys", &cfg.netArea);
  openFile(fn, &netfl);
  initNetBuf(&netBuf);

  /* open room file */
  sprintf(fn, "%sctdlroom.sys", &cfg.roomArea);
  openFile(fn, &roomfl);


  for(;;)   /* do forever */
    {
    printf(" A - Add a Node       E - Edit a Node     R - request a file\n");
    printf(" L - List Local Nodes V - View Node List  N - Set Net Privileges \n");
    printf(" S - Send a file      X - Exit            Option:");
    switch ( getchar() )
      {
      case 'x':  /* Exit */
      case 'X':
        Save_System_Data();
        printf("Data Saved, program exited\n");
        exit(0);
        break;
      case 'R':   /* File requests */
      case 'r':
        Request_String("Enter System Name:", who, sizeof who);
        if (!ReqNodeName(NULL, who, NULL, FALSE, FALSE, FALSE,FALSE, TRUE, &netBuf)) break;
        fileRequest();
        break;
      case 'S':   /* File transmissions */
      case 's':
        Request_String("Enter System Name", who, sizeof who);
        if (!ReqNodeName(NULL, who, NULL, FALSE, FALSE, FALSE,FALSE, TRUE, &netBuf))      break;
        getSendFiles(id, who);
        break;
      case 'V':   /* View the net list. */
      case 'v':
        writeNet(TRUE, FALSE);
        if (NeedSysopInpPrompt()) modIn();
        break;
      case 'L':  /* local only list
      case 'l':
        break;
      case 'A':   /* Add a new node to the list */
        addNetNode();
        break;
      case 'E':   /* Edit a node that is on the list */
        Request_String("Enter Node Name", who, sizeof who);
        if (ReqNodeName(NULL, who, NULL, FALSE, TRUE, FALSE, TRUE, TRUE, &netBuf)) editNode();
        break;
      }
    };
  }

static char AddedFiles;
/**
  Description;
  This will get the files from the sysop to send to another system.
**/
void getSendFiles(MenuId id, label sysName)
  {
  /**
    Description;
    This will get the files from the sysop to send to another system.
  **/
  SYS_FILE       sysFile;
  char     temp[10];
  extern char    *APPEND_ANY;
  char     *Files, list[126];
  struct fl_send sendWhat;

  sPrintf(temp, "%d.sfl", thisNet);
  makeSysName(sysFile, temp, &cfg.netArea);
  if ((upfd = safeopen(sysFile, APPEND_ANY)) == NULL)
    {
    printf(id, "Couldn't open %s for update?\n ", sysFile);
    return ;
    };
  printf("Files to send to %s:", sysName);
  fgets(list, sizeof list, stdin);

  getList(addSendFile, msgBuf.mbtext, 126, TRUE);

  Files = strtok(list, " ");
  if( Files )
    {
*****    sysGetSendFilesV2(, Files, &sendWhat))
*****    putSLNet(sendWhat, upfd);
    while( (Files = strtok(NULL, " ") ) != NULL )
      {
      if (sysGetSendFilesV2(GetListId, Files, &sendWhat))
        {
        putSLNet(sendWhat, upfd);
        };
      };
    netBuf.nbflags.send_files = TRUE;
    putNet(thisNet, &netBuf);
    };
  fclose(upfd);
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

void fileRequest()
  {
  /**
   Description:
   This handles the administration of requesting files from another system.
  **/
  struct fl_req file_data;
  label    data;
  char     loc[100], *c, *work;
  SYS_FILE fn;
  char     abort;
  FILE     *temp;
  int      place;
  extern char *APPEND_ANY;
  char     ambiguous, again;

  place = thisNet;    /* again, a kludge to be killed later */
  Request_String("Enter Room Name:", file_data.room, NAMESIZE);
  if (strLen(file_data.room) == 0)    return;

  Request_String("Enter the filename:", loc, sizeof loc);
  if (strLen(loc) == 0)    return;

  ambiguous = !(strchr(loc, '*') == NULL
             && strchr(loc, '?') == NULL
             && strchr(loc, ' ') == NULL);

  abort = !netGetAreaV2(id, loc, &file_data, ambiguous);
  if ( !abort )
    {
    sPrintf(data, "%d.rfl", place);
    makeSysName(fn, data, &cfg.netArea);
    if ((temp = safeopen(fn, APPEND_ANY)) == NULL)
      {
      printf("Couldn't open %s for append\n", fn);
      }
    else
      {
      work = loc;
      do
        {
        again = (c = strchr(work, ' ')) != NULL;
        if ( again ) *c = 0;
        strCpy(file_data.roomfile, work);
        if (ambiguous) strCpy(file_data.filename, work);
        fwrite(&file_data, sizeof (file_data), 1, temp);
        if (again) work = c + 1;
        }   while ( again );
      netBuf.nbflags.room_files = TRUE;
      putNet(place, &netBuf);
      fclose(temp);

      };
    };
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

void writeNet(char LocalOnly)
  {
  /**
  Description:
  This function writes nodes on the net to the screen.  Options include
  showing only local systems and with or without their ids.
  **/
  int rover;
  int i;
  printf("     %20s %22s TF Member Nets\n", "Systems", "Node Id");
  for (rover = 0; rover < cfg.netSize; rover++)
    {
    if (netTab[rover].ntflags.in_use
    && (!LocalOnly || netTab[rover].ntflags.local))
      {
      getNet(rover, &netBuf);
      printf("%3s%-20s %-22s "
      (strLen(netBuf.nbShort) != 0) ? &netBuf.nbShort : "   ",
      , netBuf.netName, netBuf.netId );
      printf("%-8s",SupportedBauds[netBuf.baudCode]);
      if (netBuf.nbflags.OtherNet)
        printf("O");
      else if (!(netBuf.MemberNets & ALL_NETS))
        printf("d");
      else printf(" ");
      printf("%c ".(netBuf.nbflags.MassTransfer) ? 'F' : ' ');
      for (i = 0; i < 32; i++)
        {
        if ((1l << i) & netBuf.MemberNets) printf("%d ", i + 1);
        };
       };
    };
  }
