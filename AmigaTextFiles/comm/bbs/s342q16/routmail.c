/*
*				RoutMail.c
*
* Reads in a net map for C86Net-style mail routing.
*/
/*
* NB: With the advent of domain mail routing most of this code has become
* irrelevant.  Don't get worked up about trying to figure out this code
* because this entire utility is probably used very very little by anyone.
*/
/*
*				History
*
* 89Nov30  HAW Route locking (1.10)
* 89Nov29  HAW Route optimization (1.9).
* 89Jul26  HAW Miscellaneous fixes (1.7).
* 89Jun13  HAW Make ReDoRouted() return TRUE at end (1.4).
* 89Jun07  HAW Support for encryption of mail files. (1.3).
* 89May03  HAW Integrity check on dead systems still on list (1.2).
* 89Apr04  HAW Fix for the virtual room capability (1.1).
* 89Jan--  HAW Initial creation finished, V1.
*/
/*
*				Contents
*
*	AddNewNode()		adds a new node to the node list.
*	BackToDirect()		return a route to direct connect
*	CheckAccessible()	Runs through map for routing.
*	FindHub()		Find a hub in the netmap.
*	FindPeon()		Find a peon in the netmap.
*	GetLine()		Gets a line from file.
*	HubHub()		Hubs talking to hubs.
*	InitSystem()		open up data base files.
*	main()			Main manager of program.
*	MakeMap()		Reads in the PEONs section of map.
*	ParseId()		Splits line into node ID and Name.
*	ProcessDead()		Process "dead" section of the netmap.
*	ReadArguments()		Read command line arguments.
*	ReadHubs()		Reads in the HUBS section of map.
*	SetHub()		Route path to a hub node.
*	SetNode()		Sets up routing path.
*	SetPeon()		Route path to a peon node.
*	SetUpHub()		Set up a hub for processing.
*	ShowArguments()		Inform user of valid arguments.
*	WeAreHub()		Handles us being a hub node.
*	WeArePeon()		Handles us being a peon node.
*/
#include "ctdl.h"
#include "citamap.h"
#define DEFAULT_NET	(1l << 14)
#define MAP_NAME	"ROUTEMAP"
/* Names of options for mapper. */
#define ADD_NODES	"+add"
#define KILL_NODES	"+kill"
#define DISCARD_MAIL	"+discard"
#define MANUAL_MODE	"+manual"
#define DOMAIN_MODE	"+domains"
/* Warning messages.	*/
#define UNKNOWN_HUB	     "WARNING: We are attached to a hub named %s (%s) of which we know nothing!\n"
#define SURPRISE_ROUTE   "SURPRISE!  We were routing Mail to a node we did not know: %s (%s)\n."
#define DISCARD_UNKNOWN  "We'll discard this Mail to the unknown node.\n"
#define DISCARD_KNOWN    "Discarding mail being routed to %s via %s.\n"
#define REROUTING	       "Rerouting mail for %s via %s.\n"
#define ERROR_LOCAL_MAIL "Couldn't open direct mail file for %s!\n"
#define MAIL_IN_TRANSIT	 "ERROR: Map says to delete %s, but it has mail in transit from:\n"
#define HAS_ROUTED	     "ERROR: %s is to be reached indirectly, but it has mail in transit!\n"
/* Explanations	*/
#define ADD_EXPLAIN	     "\t%s -- cause unknown nodes to be added to your node list.\n"
#define KILL_EXPLAIN     "\t%s -- cause nodes in DEAD section to be deleted from your node list.\n"
#define NEW_NODE	       "%s @%s added: non-local / %s / net 15.\n"
#define DISCARD	         "\t%s -- discard routed mail which can't be rerouted.\n\t\t(Use with caution.)\n"
#define MANUAL_EXPLAIN   "\t%s -- use the node editor.\n"
#define DOMAIN_EXPLAIN   "\t%s -- add domain handlers (found in ctdldmhd.sys) to CtdlNet.Sys.\n"
/* Exit error codes	*/
#define DEAD_IS_HUB	     "Node %s is present both as a hub and a dead node.\n"
#define DEAD_IS_PEON	   "Node %s is present both as a peon and a dead node.\n"
#define NO_ERRORS	      0
#define NO_FILE_NAMED   1
#define MAP_ERROR	      2
#define NO_SYS_TABLE    3
#define LF_ERROR	      4
#define SYSTEM_CRASH    10
char inNet = NORMAL_NET;
/*
Thoughts on topology map.
o Could organize it as a set of hubs.  Each hub could have: hubs to which
it connects, peons to which it connects.  Or do we need the explicit
recognition?   Can we handle just a "who does he connect with directly"
organization?  We don't* want to maintain differing netmaps for each
area of the net.  It seems we might need a path resolution algorithm
in the map reader.  Hell, we have code space since this'll be a utility
program ... hmmm.  Let's experiment with the idea.
o After some thought ... what if we make it a necessity that a system
running the route utility be on that map?  This may help us resolve
the situation.  If we can identify our own node on the list (above), then
we can also recognize our situation, as either a "hub" of some sort, or
a normal "peon".
- peons simply route everything non-local to the hub (or hubs) in their
area.  Ah, a rub: what if there's more than one hub, such as Images/
Test System, or UCC/KAOS?  Hmmmmmm.
o After writing some of the code and thinking about path resolution,
multiple backbones in one area, etc ... I think we need the map itself
to be more intelligent.  Let's divide it into two areas: HUBS and PEONS.
The HUB area would describe how the HUBS are configured -- who talks to
who.  The PEONS area is a listing what peons are connected to what HUBs.
o I also now think we need to model the topography in the resultant data
instantiation: we should be able to easily tell (from reading the netmap)
who talks to who.  More structures ...
*/
/*
* Map Data structure:
*
*  Two areas.  The second is a HUBS area, listing all hubs and who talks to
*  who.  The first is a PEONS list -- what peons are connected to what HUB.
*
*  Each area is a series of records.  Each record is separated from the next by
*  a blank line.  First item is the name of the system which connects directly
*  with the system(s) which follow it.
*
*  Systems are identified by <nodeId>:<nodeName>.
*	US 612 470 9635 : C-86 Test System
*
*  Perhaps it would make sense to require the start of the Node ID be in
*  the first column, otherwise it's a comment.
*
*  Later thought: A third section named "DOWN" might be an appropriate way
*  to get permanently down systems off the netlist.
*/
/*
* Routing optimization:
*   We need to do route optimization because the maps now may contain
* more than one path to a node, and sometimes the router will choose
* the "worse" path.  Therefore, for each distinct node in the map we
* will have to maintain a "hop count" to that node.  When we find a
* new path to any node, we first check our current hop count for this
* node and, if it's better (less) then we reset the path, otherwise
* not.
*/
int FindHopCount(char *Id);
NetBuffer nBuf1, nBuf2;
FILE *wfd;
char  AddNodes    = FALSE;
char  KillNodes   = FALSE;
char  DiscardMail = FALSE;
char  Manual	  = FALSE;
char  DoDomains   = FALSE;
extern FILE *upfd;
HUB  *Systems;
label OurId;
int   CurHubNet, callSlot, Pass;
SListBase FwdAliasii =
  {
  NULL, ChkNtoStr, NULL, FreeNtoStr, EatNMapStr

  };
SListBase DeadNodes  =
  {
  NULL, NULL, NULL, free, NULL

  };
SListBase AreaList   =
  {
  NULL, CheckArea, NULL, NULL, NULL

  };
extern SListBase Serves;
void *FindDomainH(), *EatDomainH();
static SListBase DomainHandlers =
  {
  NULL, FindDomainH, NULL, free, EatDomainH

  };
typedef struct
  {
  char *domain;
  char *name;
  char *nodeId;

  }
DomainHandler;
char *rates[] =
  {
  "300", "1200", "2400", "4800", "9600", "14400", "19200", "38400", "57600"

  };
/* this is here to satisfy a library */
char onConsole = FALSE, remoteSysop = FALSE;
FILE *netLog = NULL;
extern CONFIG    cfg;
extern NetBuffer netBuf, netTemp;
extern int	 thisNet;
extern FILE	 *netfl;
extern MessageBuffer   msgBuf;
extern NetTable  *netTab;
SListBase MailForward =
  {
  NULL, NULL, NULL, NULL, NULL

  };
/*
* AddNewNode()
*
* This function adds a new node to the internal node list we maintain.
*/
void AddNewNode(Name, Id, baud)
label Name, Id;
int baud;
  {
  int rover, gen;
  /*
  * This function adds a new node to the node list.
  * 1. Scavenge for unused node.  If none found, then we make new one.
  * 2. Zero it.
  * 3. Copy in Name & Id.
  * 4. Assume node is non-local & mark it so.  Announce to world.
  * 5. Assume node is 1200 baud, announce to world.
  * 6. Set node to member net 15 only.  Announce to world.
  * 7. Save node.
  */
  rover = GetNewSlot(&netBuf);
  gen = netBuf.nbGen;
  /*    zero the node -- watch out for pointer problems!  */
  killNetBuf(&netBuf);
  zero_struct(netBuf);
  initNetBuf(&netBuf);
  netBuf.nbGen = gen;
  strCpy(netBuf.netName, Name);
  strCpy(netBuf.netId, Id);
  netBuf.nbflags.in_use   = TRUE;    /* yes, we live!   */
  netBuf.nbflags.local    = FALSE;
  netBuf.nbflags.RouteFor = TRUE;
  netBuf.nbflags.RouteTo  = TRUE;
  netBuf.MemberNets	    = DEFAULT_NET;
  if (baud != -1)
  netBuf.baudCode	= baud;
  else
  netBuf.baudCode	= 1;
  netBuf.nbRoute	= -1;	/* No route.    */
  printf(NEW_NODE, Name, Id, rates[netBuf.baudCode]);
  putNet(rover, &netBuf);
  InitVNode(rover);

  }
/*
* GetNewSlot(n)
*
* This function gets a new slot for adding a new node.
*/
int GetNewSlot(n)
NetBuffer *n;
  {
  int rover;
  /* rover will be set right at the end of this. */
  for (rover = 0; rover < cfg.netSize; rover++)
  if (!netTab[rover].ntflags.in_use)
    {
    getNet(rover, n);
    break;

    }
  if (rover == cfg.netSize)
    {
    cfg.netSize++;
    n->nbGen = 0;
    if (cfg.netSize == 1)
    netTab = GetDynamic(sizeof *netTab);
    else
    netTab = realloc(netTab, sizeof (*netTab) * cfg.netSize);
    netTab[rover].netTRooms = GetDynamic(SR_BULK);

    }
  else n->nbGen = (n->nbGen + 1) % NET_GEN;
  return rover;

  }
/*
* BackToDirect()
*
* We are no longer routing to this node, but going direct.
*/
void BackToDirect(system, n)
int system;
NetBuffer *n;
  {
  label    Id, Name, temp, TarId;
  SYS_FILE fn, fn2;
  int i = 0, LastRoute;
  if (n->nbRoute == -1) return ;
  normId(n->netId, TarId);
  getNet(n->nbRoute, &nBuf1);
  LastRoute = FindRouteName(system, fn);
  do
    {
    sPrintf(temp, "R%d.%d", n->nbRoute, i);
    makeSysName(fn, temp, &cfg.netArea);
    if (IdRouteFile(fn, Id, Name))
      {
      if (strCmpU(Id, TarId) == SAMESTRING)
        {
        sprintf(temp, "R%d.%d", system, LastRoute++);
        makeSysName(fn2, temp, &cfg.netArea);
        rename(fn, fn2);
        CoverRouteHole(n->nbRoute, i);

        }
      else i++;

      }
    else break;

    }
  while (TRUE);
  if (i == 0)
  nBuf1.nbflags.HasRouted = FALSE;
  nBuf1.nbHiRouteInd = i;
  putNet(n->nbRoute, &nBuf1);
  if (LastRoute != 0) n->nbflags.HasRouted = TRUE;
  n->nbHiRouteInd = LastRoute - 1;

  }
/*
* CheckAccessible()
*
* This function helps run through the map, making routes.
*/
void CheckAccessible(HUB *HomeHub, HUB *CurHub, PEONS *Kr, int HopCount)
  {
  PEONS *Krover;
  LINKS *LinkRover;
  if (CurHub->Checked < Pass || IsLowerHopCount(CurHub->Id, HopCount))
    {
    /* So we don't circle the wagons on and on and on ... */
    CurHub->Checked = Pass;
    /* Set CurHub to be accessed via HomeHub. */
    SetHub(CurHub, HomeHub, HopCount);
    /*
    * For all nodes directly handled by this hub, set us up to
    * go through HomeHub to get to that node iff that node is
    * non-local.
    */
    for (Krover = CurHub->list; Krover != NULL; Krover = Krover->next)
    if (Krover != Kr)
    SetPeon(Krover, HomeHub, HopCount);
    /*
    * Now we need to go through every node on the list which we may
    * access and set it to be accessible via the hub node we are
    * currently using.  We do this by traversing the list of HUBs which
    * the current HUB may access, which is contained in the Hub field of
    * each hub in the list.  In order to avoid looping, we must check
    * each Hub's flag before processing it, and set it if not set yet.
    *
    * Fortunately, we can do this recursively, I think.
    */
    for (LinkRover = CurHub->Hubs; LinkRover != NULL;
    LinkRover = LinkRover->next)
    CheckAccessible(HomeHub, LinkRover->Hub, Kr, HopCount + 1);

    }

  }
/*
* CoverRouteHole()
*
* This function creates file names for routing files.  It must create them
* in an ascending numerical order of the form "r<num>.<num>", where the
* first number is the node index its associated with (routing mail to) and
* the second is the sequence number.
*/
void CoverRouteHole(system, start)
int system, start;
  {
  int i;
  label temp;
  SYS_FILE fn, fn2;
  for (i = start + 1; ; i++)
    {
    sPrintf(temp, "R%d.%d", system, i);
    makeSysName(fn, temp, &cfg.netArea);
    if (access(fn, 0) == -1) break;
    sPrintf(temp, "R%d.%d", system, i - 1);
    makeSysName(fn2, temp, &cfg.netArea);
    rename(fn, fn2);

    }

  }
/*
* crashout()
*
* This is the general crash routine.  Needed by getNet, etc.
*/
void crashout(str)
char *str;
  {
  printf(str);
  writeSysTab();
  exit(SYSTEM_CRASH);

  }
/*
* FindHub()
*
* This function will check for the existence of a node in the hub list.
*/
HUB *FindHub(base, id)
HUB *base;
label id;
  {
  HUB *rover;
  for (rover = base; rover != NULL; rover = rover->next)
  if (strCmp(id, rover->Id) == SAMESTRING)
  return rover;
  return NULL;

  }
/*
* FindGlobalPeon()
*
* This function finds a peon anywhere in the map.
*/
PEONS *FindGlobalPeon(base, id)
HUB *base;
label id;
  {
  HUB *rover;
  PEONS *toReturn = NULL;
  for (rover = base; rover != NULL; rover = rover->next)
  if ((toReturn = FindPeon(rover->list, id)) != NULL)
  break;
  return toReturn;

  }
/*
* FindPeon()
*
* This finds if a given id is in a list of peons.
*/
PEONS *FindPeon(base, id)
PEONS *base;
label id;
  {
  PEONS *rover;
  for (rover = base; rover != NULL; rover = rover->next)
  if (strCmpU(id, rover->Id) == SAMESTRING)
  return rover;
  return NULL;

  }
/*
* GetLine()
*
* This function gets a line from file, strips some shit out.
*/
char *GetLine(line, s, fd)
char *line;
int  s;
FILE *fd;
  {
  int i;
  do
    {
    if (fgets(line, s, fd) == NULL)
    return NULL;
    /* Strip trailing blanks. */
    for (i = strLen(line) - 1; i >= 0; i--)
      {
      if (line[i] != '\n' && line[i] != '\t' && line[i] != ' ')
      break;  /* non-whitespace, so break     */
      line[i] = 0;

      }

    }
  while (line[0] == ' ' || line[0] == '\t');
  return line;

  }
/*
* HubHub()
*
* This handles the direct hub to hub and then the trace.  This is here to
* circumvent the "Checked" variable, just in case there's a loop in the net
* topology which would have us sending route mail on a long loop when we
* actually directly connect to the hub in question.  Note this is a poor
* man's attempt at net optimization, and consideration might be worth giving
* to doing true optimization in the future.
* 89Nov30 -- ok, with the hopcount stuff we're trying to optimize
*/
void HubHub(Us, Hub)
HUB *Us, *Hub;
  {
  char Connected;
  if ((CurHubNet = searchNet(Hub->Id, &netTemp)) == ERROR)
    {
    if (!AddNodes)
      {
      Connected = FALSE;
      printf(UNKNOWN_HUB, Hub->Name, Hub->Id);
      /* return ; */

      }
    else
      {
      Connected = TRUE;
      AddNewNode(Hub->Name, Hub->Id, Hub->baud);
      CurHubNet = thisNet;
      getNet(CurHubNet, &netTemp);

      }

    }
  else Connected = TRUE;
  if (Connected)
    {
    if (!DirectRoute(&netTemp))
    BackToDirect(CurHubNet, &netTemp);
    netTemp.nbRoute = -1;   /* Direct route. */
    putNet(CurHubNet, &netTemp);

    }
  Hub->Checked--;	/* Deactivate flag -- some more poor    */
  /* man optimization.                    */
  SetHopCount(Hub->Id, 0, NULL);
  CheckAccessible(Hub, Hub, NULL, 1);

  }
/*
* InitSystem()
*
* This function will open up appropriate data base files, and initialize
* buffers.
*/
void InitSystem()
  {
  SYS_FILE name;
  extern FILE *logfl;
  extern logBuffer logTmp, logBuf;
  /* Open database files. */
  makeSysName(name, "ctdlnet.sys", &cfg.netArea);
  openFile(name, &netfl);
  makeSysName(name, "ctdllog.sys", &cfg.logArea);
  openFile(name, &logfl);
  initLogBuf(&logTmp);
  initLogBuf(&logBuf);
  InitMsgBase();
  /* Now initialize data buffers. */
  initNetBuf(&netBuf);
  initNetBuf(&netTemp);
  initNetBuf(&nBuf1);
  initNetBuf(&nBuf2);
  makeSysName(name, "ctdlalsi.sys", &cfg.roomArea);
  MakeList(&FwdAliasii, name, upfd);
  VirtInit();

  }
/*
* KillMapNodes()
*
* This function kills a node if possible, in accordance with map directions.
*/
void KillMapNodes(Id)
label Id;
  {
  label    temp;
  SYS_FILE fn;
  if (FindHub(Systems, Id) != NULL)
    {
    printf(DEAD_IS_HUB, Id);
    exit(MAP_ERROR);

    }
  if (FindGlobalPeon(Systems, Id) != NULL)
    {
    printf(DEAD_IS_PEON, Id);
    exit(MAP_ERROR);

    }
  if (searchNet(Id, &netBuf) != ERROR)
    {
    if (netBuf.nbflags.HasRouted)
      {
      sprintf(temp, "R%d.0", thisNet);
      makeSysName(fn, temp, &cfg.netArea);
      if (access(fn, 0) == 0)
        {
        printf(MAIL_IN_TRANSIT, netBuf.netName);
        PrintRouteIds();
        return;

        }

      }
    printf("Deleting '%s'.\n", netBuf.netName);
    KillAuxFiles();	/* kill support files */
    netBuf.nbflags.in_use = FALSE;
    putNet(thisNet, &netBuf);

    }

  }
HUB  *Us;
/*
* main()
*
* The main manager for routemap eater.
*/
main(argc, argv)
int  argc;
char **argv;
  {
  char filename[100];
  printf(R_TITLE);
  if (argc < 2)
    {
    ShowArguments();
    exit(NO_FILE_NAMED);

    }
  if (!ReadArguments(argc, argv, filename))
    {
    printf("ERROR: No discernible file name found.\n\n");
    ShowArguments();
    exit(NO_FILE_NAMED);

    }
  if (!readSysTab(FALSE, TRUE))
    {
    printf("Couldn't read system table.\n");
    exit(NO_SYS_TABLE);

    }
  if (access(LOCKFILE, 0) != ERROR)
    {
    printf("Please do not run RoutMail using Outside Commands.\n");
    writeSysTab();
    exit(LF_ERROR);

    }
  InitSystem();	/* Open needed data files	*/
  if (Manual)
  DoEdit();
  else
    {
    if ((Systems = MakeMap(filename)) == NULL)
      {
      writeSysTab();
      exit(MAP_ERROR);

      }
    if (KillNodes)    RunList(&DeadNodes, KillMapNodes);
    /* normalize our id */
    normId(cfg.codeBuf + cfg.nodeId, OurId);
    SetHopCount(OurId, -1, NULL);
    /* Find ourselves */
    Pass = 1;
    if ((Us = FindHub(Systems, OurId)) != NULL)
    WeAreHub(Us, Systems);
    else
    WeArePeon(Systems);
    /* now do it again! */
    Pass = 2;
    if ((Us = FindHub(Systems, OurId)) != NULL)
    WeAreHub(Us, Systems);
    else
    WeArePeon(Systems);
    if (DoDomains) HandleDomains();

    }
  writeSysTab();
  exit(NO_ERRORS);
  return NO_ERRORS;

  }
/*
* MakeMap()
*
* This reads in the PEONs section of the map.
*/
HUB *MakeMap(fn)
char *fn;
  {
  SYS_FILE temp;
  FILE  *fd;
  char  line[100], found = FALSE;
  HUB   base, *HubRover, *HubCur;
  PEONS kbase, *KnownRover, *KnownCur;
  extern char *READ_TEXT;
  if ((fd = fopen(fn, READ_TEXT)) == NULL)
    {
    printf("Couldn't open '%s'.\n", fn);
    return NULL;

    }
  while (!found)
    {
    if (GetLine(line, 100, fd) == NULL) break;
    if (strCmpU(line, "PEONS") == SAMESTRING) found = TRUE;

    }
  if (!found)
    {
    printf("ERROR: Never found PEONS heading.\n");
    return NULL;

    }
  printf("\nReading PEON lists.\n");
  base.next = NULL;
  HubCur = &base;
  do
    {
    /* get data */
    do
    if (GetLine(line, 100, fd) == NULL)
      {
      printf("ERROR: No HUBS section encountered.\n");
      return NULL;    /* where's the HUB section? */

      }
    while (strLen(line) == 0);	/* skip leading blank lines */
    if (strCmpU(line, "HUBS") == SAMESTRING)
    break;	    /* end of PEONS section     */
    /* now construct a hub node */
    HubRover = (HUB *) GetDynamic(sizeof base);
    HubRover->Hubs = NULL;	/* init */
    HubRover->Checked = 0;	/* init */
    if (!Parse(HubRover->Id, HubRover->Name, &HubRover->baud, line))
    return NULL;
    AddNewHop(HubRover->Id);
    if (FindHub(base.next, HubRover->Id) != NULL)
      {
      printf("Hub multiply defined! (%s)\n", HubRover->Id);
      return NULL;

      }
    HubCur->next = HubRover;
    HubRover->next = NULL;
    HubCur = HubRover;
    /* now construct the list of "known" nodes */
    kbase.next = NULL;
    KnownCur = &kbase;
    do
      {
      if (GetLine(line, 100, fd) == NULL)
        {
        printf("ERROR: No HUBS section encountered.\n");
        return NULL;

        }
      if (strLen(line) == 1 || strLen(line) == 0)
      break;    /* end of record indicator    */
      KnownRover = (PEONS *) GetDynamic(sizeof kbase);
      if (!Parse(KnownRover->Id, KnownRover->Name, &KnownRover->baud, line))
      return NULL;
      AddNewHop(KnownRover->Id);
      if (FindPeon(kbase.next, KnownRover->Id) != NULL)
        {
        printf("PEON %s multiply defined!\n", KnownRover->Id);
        return NULL;

        }
      KnownCur->next = KnownRover;
      KnownCur = KnownRover;
      KnownCur->next = NULL;

      }
    while (TRUE);
    HubRover->list = kbase.next;

    }
  while (TRUE);
  if (!ReadHubs(base.next, fd))
  return NULL;
  fclose(fd);
  if (DoDomains)
    {
    makeSysName(temp, "ctdldmhd.sys", &cfg.netArea);
    if (!MakeList(&DomainHandlers, temp, NULL))
    printf("Warning: %s was not found for processing!\n", temp);
    makeSysName(temp, "ctdldmhd.lcl", &cfg.netArea);
    MakeList(&DomainHandlers, temp, NULL);

    }
  return base.next;

  }
/*
* Parse()
*
* This function takes a line and splits it into node ID and Name.
*/
char Parse(Id, Name, baud, line)
char *line;
label Id, Name;
int *baud;
  {
  char *s, *t, *u;
  /* Guaranteed on entry that line is non-comment or separator */
  if ((s = strchr(line, ':')) == NULL)
  return FALSE;	/* No colon?  ERROR! */
  *s = 0;
  if (strLen(line) >= NAMESIZE)	/* Error checking!    */
  return FALSE;
  normId(line, Id);
  s++;
  /* Skip over leading white space after the colon, before the name. */
  while (*s && (*s == ' ' || *s == '\t')) s++;
  if ((u = t = strchr(s, '@')) != NULL)
    {
    *t = 0;
    for (u--; *u == ' ' || *u == '\t'; u--)
    ;
    u++;
    *u = 0;

    }
  if (strLen(s) >= NAMESIZE)	/* Error checking!    */
  return FALSE;
  strCpy(Name, s);
  if (t != NULL)
    {
    for (t++; *t == ' ' || *t == '\t'; t++)
    ;
    if (strLen(t))
      {
      switch (atoi(t))
        {
        case 300: *baud = 0; break;
        case 1200: *baud = 1; break;
        case 2400: *baud = 2; break;
        case 4800: *baud = 3; break;
        case 9600: *baud = 4; break;
        case 14400: *baud = 5; break;
        case 19200: *baud = 6; break;
        default: printf("Unknown baud rate %s\n", t);

        }

      }
    else *baud = -1;

    }
  else *baud = -1;
  return TRUE;

  }
/*
* ParseCh()
*
* This function parses a Change to a system's number or name.
*/
char ParseCh(OldId, NewId, Name, baud, line)
char *line;
label NewId, OldId, Name;
int *baud;
  {
  char *s, *t, *u;
  /* Guaranteed on entry that line is non-comment or separator */
  if ((s = strchr(line, ':')) == NULL)
  return FALSE;	/* No colon?  ERROR! */
  *s = 0;
  if (strLen(line) >= NAMESIZE)	/* Error checking!    */
  return FALSE;
  normId(line, OldId);
  s++;
  if ((t = strchr(s, ':')) == NULL)
  return FALSE;	/* No colon?  ERROR! */
  *t = 0;
  if (strLen(s) >= NAMESIZE)	/* Error checking!    */
  return FALSE;
  normId(s, NewId);
  t++;
  s = t;
  /* Skip over leading white space after the colon, before the name. */
  while (*s && (*s == ' ' || *s == '\t')) s++;
  if ((u = t = strchr(s, '@')) != NULL)
    {
    *t = 0;
    for (u--; *u == ' ' || *u == '\t'; u--)
    ;
    u++;
    *u = 0;

    }
  if (strLen(s) >= NAMESIZE)	/* Error checking!    */
  return FALSE;
  strCpy(Name, s);
  if (t != NULL)
    {
    for (t++; *t == ' ' || *t == '\t'; t++)
    ;
    if (strLen(t))
      {
      switch (atoi(t))
        {
        case 300: *baud = 0; break;
        case 1200: *baud = 1; break;
        case 2400: *baud = 2; break;
        case 4800: *baud = 3; break;
        case 9600: *baud = 4; break;
        case 14400: *baud = 5; break;
        case 19200: *baud = 6; break;
        default: printf("Unknown baud rate %s\n", t);

        }

      }
    else *baud = -1;

    }
  else *baud = -1;
  return TRUE;

  }
/*
* ProcessDead()
*
* This function should process the "dead" section of the netmap.
*/
char ProcessDead(fd)
FILE *fd;
  {
  char line[100];
  label Name, Id, *temp;
  int baud;
  /* Are we allowed to kill dead nodes? */
  if (!KillNodes)
  return TRUE;
  while (GetLine(line, 100, fd) != NULL)
    {
    if (strCmpU(line, "COMMENTARY") == SAMESTRING) break;
    if (!Parse(Id, Name, &baud, line))
    continue;    /* return to top of loop */
    temp = GetDynamic(sizeof *temp);
    strCpy(*temp, Id);
    AddData(&DeadNodes, temp, NULL, FALSE);

    }
  return TRUE;

  }
/*
* ReadArguments()
*
* This function reads command line arguments.
*/
char ReadArguments(argc, argv, filename)
int  argc;
char **argv, *filename;
  {
  int rover;
  for (rover = 1; rover < argc; rover++)
    {
    if (strCmpU(argv[rover], ADD_NODES) == SAMESTRING)
    AddNodes = TRUE;
    else if (strCmpU(argv[rover], KILL_NODES) == SAMESTRING)
    KillNodes = TRUE;
    else if (strCmpU(argv[rover], DISCARD_MAIL) == SAMESTRING)
    DiscardMail = TRUE;
    else if (strCmpU(argv[rover], DOMAIN_MODE) == SAMESTRING)
    DoDomains = TRUE;
    else if (strCmpU(argv[rover], MANUAL_MODE) == SAMESTRING)
      {
      Manual = TRUE;
      return TRUE;

      }
    else
      {
      strCpy(filename, argv[rover]);
      return TRUE;

      }

    }
  return FALSE;

  }
/*
* ReadHubs()
*
* This function reads in the HUBS section.
*/
char ReadHubs(base, fd)
HUB *base;
FILE *fd;
  {
  char line[100];
  label Name, Id;
  LINKS lbase, *lr, *clr;
  HUB   *Targ;
  int   baud;
  printf("\nReading HUBS.\n");
  /*
  * "HUBS" already encountered.
  */
  do
    {
    do
    if (GetLine(line, 100, fd) == NULL)
    return TRUE;
    while (strLen(line) == 0);	/* skip leading blank lines */
    if (strCmpU(line, "DEAD") == SAMESTRING)
    break;        /* end of HUBS section     */
    if (strCmpU(line, "CHANGES") == SAMESTRING)
    break;        /* end of HUBS section     */
    if (strCmpU(line, "COMMENTARY") == SAMESTRING)
    break;
    if (!Parse(Id, Name, &baud, line))
    return FALSE;
    printf("HUB %s\n", Name);
    AddNewHop(Id);
    if ((Targ = FindHub(base, Id)) == NULL)
      {
      printf("'%s' not found from PEONs section.\n", Name);
      return FALSE;

      }
    if (Targ->Hubs != NULL)
      {
      printf("'%s' already defined in HUBS section.\n",
      Name);
      return FALSE;

      }
    clr = &lbase;
    clr->next = NULL;
    do
      {
      if (GetLine(line, 100, fd) == NULL)
      break;
      if (strLen(line) == 1 || strLen(line) == 0)
      break;    /* end of record indicator    */
      lr = (LINKS *) GetDynamic(sizeof lbase);
      if (!Parse(Id, Name, &baud, line))
        {
        return FALSE;

        }
      AddNewHop(Id);
      if ((lr->Hub = (void *) FindHub(base, Id)) == NULL)
        {
        lr->Hub = (HUB *) GetDynamic(sizeof *Targ);
        strCpy(lr->Hub->Name, Name);
        strCpy(lr->Hub->Id, Id);
        lr->Hub->baud = baud;
        if (searchNet(Id, &netBuf) == ERROR)
        if (AddNodes) AddNewNode(Name, Id, baud);

        }
      clr->next = lr;
      clr = lr;

      }
    while (TRUE);
    clr->next = NULL;
    Targ->Hubs = lbase.next;    /* don't check for NULL */

    }
  while (TRUE);
  if (strCmpU(line, "DEAD") == SAMESTRING)
  return ProcessDead(fd);
  else if (strCmpU(line, "CHANGES") == SAMESTRING)
  return ProcChanges(fd);
  return TRUE;

  }
/*
* ProcChanges()
*
* This function will process the CHANGES section.
*/
char ProcChanges(fd)
FILE *fd;
  {
  char line[100];
  label Name, OldId, NewId;
  LINKS lbase, *lr, *clr;
  HUB   *Targ;
  int   baud;
  printf("\nReading CHANGES.\n");
  /*
  * "CHANGES" already encountered.
  */
  do
    {
    do
    if (GetLine(line, 100, fd) == NULL)
    return TRUE;
    while (strLen(line) == 0);	/* skip leading blank lines */
    if (strCmpU(line, "DEAD") == SAMESTRING)
    break;	/* end of CHANGES section	*/
    if (strCmpU(line, "COMMENTARY") == SAMESTRING)
    break;
    if (!ParseCh(OldId, NewId, Name, &baud, line))
    return FALSE;
    if (searchNet(NewId, &netBuf) == ERROR)
    if (searchNet(OldId, &netBuf) == ERROR)
    continue;
    ReDoAuxFiles(NewId);	/* changes aux files as needed */
    strCpy(netBuf.netId, NewId);
    strCpy(netBuf.netName, Name);
    /* if baud specified */
    if (baud != -1)
    netBuf.baudCode = baud;
    putNet(thisNet, &netBuf);

    }
  while (TRUE);
  if (strCmpU(line, "DEAD") == SAMESTRING)
  return ProcessDead(fd);
  return TRUE;

  }
/*
* SetHub()
*
* This sets up a route path to a hub node.
*/
void SetHub(HUB *CurHub, HUB *HomeHub, int HopCount)
  {
  SetNode(CurHub->Name, CurHub->Id,CurHub->baud, HomeHub->Id, HopCount, TRUE);

  }
/*
* SetPeon()
*
* This sets up a route path to a peon node.
*/
void SetPeon(PEONS *Peon, HUB *HomeHub, int HopCount)
  {
  SetNode(Peon->Name, Peon->Id, Peon->baud, HomeHub->Id, HopCount, TRUE);

  }
/*
* SetNode()
*
* This function sets up routing path.
*/
void SetNode(TargetName, TargetId, TargetBaud, RouteId, HopCount, CheckHops)
label TargetName, TargetId, RouteId;
int   TargetBaud, HopCount;
char CheckHops;
  {
  char New = FALSE, NotAlive = FALSE;
  /*
  * We check to make sure target and router aren't the same.  If they
  * are, then there's no reason to do anything further here.
  */
  if (strCmp(TargetId, RouteId) == SAMESTRING)
  return;
  /*
  * Check to see if already present in net list.
  */
  if (searchNet(TargetId, &netBuf) == ERROR)
    {
    if (!AddNodes)
      {
      NotAlive = TRUE;

      }
    else
      {
      AddNewNode(TargetName, TargetId, TargetBaud);
      New = TRUE;

      }

    }
  /*
  * If system is local, then we don't try to setup a route.  Let the
  * sysop do it manually, instead.
  */
  if (!NotAlive && netBuf.nbflags.local)
    {
    SetHopCount(TargetId, 0, NULL);	/* direct connect -- 0 hops */
    return;

    }
  /*
  * If the RouteLock flag is set, then don't do this.
  */
  if (!NotAlive && netBuf.nbflags.RouteLock)
  return;
  if (!NotAlive && (DirectRoute(&netBuf) ||
  (!DirectRoute(&netBuf) && netBuf.nbRoute != CurHubNet)))
    {
    if (!CheckHops || IsLowerHopCount(TargetId, HopCount))
      {
      if (!New)
      printf("Changing route for %s to use %s (%d).\n",netBuf.netName,
      netTemp.netName, HopCount);
      if (ReRoute(CurHubNet))
        {
        netBuf.nbRoute    = CurHubNet;
        netBuf.nbRouteGen = netTemp.nbGen;
        putNet(thisNet, &netBuf);
        SetHopCount(TargetId, HopCount, RouteId);

        }
      else if (Pass == 2)
      printf(HAS_ROUTED, netBuf.netName);

      }

    }
  else SetHopCount(TargetId, HopCount, RouteId);

  }
/*
* SetUpHub()
*
* This function will set up a hub for processing by/for a peon.
*/
char SetUpHub(Hr)
HUB *Hr;
  {
  if ((CurHubNet = searchNet(Hr->Id, &netTemp)) == ERROR)
    {
    if (!AddNodes)
      {
      printf(UNKNOWN_HUB, Hr->Name, Hr->Id);
      CurHubNet = -1;
      if (!DoDomains) return FALSE;

      }
    else
      {
      AddNewNode(Hr->Name, Hr->Id, Hr->baud);
      getNet(thisNet, &netTemp);
      CurHubNet = thisNet;

      }

    }
  if (CurHubNet != -1 && !DirectRoute(&netTemp))
    {
    printf("Changing %s to be a direct link.\n", netTemp.netName);
    BackToDirect(CurHubNet, &netTemp);

    }
  SetHopCount(Hr->Id, 0, NULL);	/* direct connect, now */
  netTemp.nbRoute = -1;   /* Force direct comm with this hub.	*/
  return TRUE;

  }
/*
* ShowArguments()
*
* Inform user of valid arguments.
*/
void ShowArguments()
  {
  printf("Usage: %s [options] <map name>\n\n", MAP_NAME);
  printf("Options:\n");
  printf(ADD_EXPLAIN, ADD_NODES);
  printf(KILL_EXPLAIN, KILL_NODES);
  printf(DISCARD, DISCARD_MAIL);
  printf(MANUAL_EXPLAIN, MANUAL_MODE);
  printf(DOMAIN_EXPLAIN, DOMAIN_MODE);

  }
/*
* WeAreHub()
*
* This function handles processing if we are a hub system.
*/
void WeAreHub(Us, Systems)
HUB *Us, *Systems;
  {
  LINKS *LinkRunner;
  PEONS *peons;
  /*
  * As a hub, we need to know which hub will take us to other peons
  * and other hubs.  We'll do this by traversing all the hubs we
  * can talk to, and all of their peons.  This should seem quite
  * a bit like we did for WeArePeon(), I hope.
  *
  * However, at the moment we do not attempt to process our own list of
  * peons.  We assume these are all set up correctly.  This could, of
  * course, be changed in the future.
  */
  Us->Checked = Pass;	/* So we don't process ourselves.	*/
  for (LinkRunner = Us->Hubs; LinkRunner != NULL;
  LinkRunner = LinkRunner->next)
  HubHub(Us, LinkRunner->Hub);
  for (peons = Us->list; peons != NULL; peons = peons->next)
    {
    if (searchNet(peons->Id, &netBuf) == ERROR)
    AddNewNode(peons->Name, peons->Id, peons->baud);
    if (!DirectRoute(&netBuf) && !netBuf.nbflags.RouteLock)
      {
      BackToDirect(thisNet, &netBuf);
      netBuf.nbRoute = -1;
      printf("Changing %s to be a direct link.\n", netBuf.netName);

      }
    SetHopCount(peons->Id, 0, NULL);
    putNet(thisNet, &netBuf);

    }

  }
/*
* WeArePeon()
*
* This function handles us being a peon node.
*/
void WeArePeon(Systems)
HUB *Systems;
  {
  HUB *Hr;
  PEONS *Kr, *Kkr;
  char found = FALSE;
  /*
  * If we are in this function, we have verified we are not a hub
  * for anyone.  This implies, due to the structure of the netmap, that
  * we may exist in more than one hub's peon listing.  Therefore, we must
  * traverse all hubs' peon listings, searching for ourselves.  For each
  * one we find ourselves in, we using that hub's listing of other hubs
  * it directly talks to in order to assign the route for those nodes
  * accessible via those hubs to the current hub (confusing? yeah).
  *
  * Since those hubs directly accessible via the current hub may talk to
  * yet more hubs, thus allowing us access to yet more nodes, both hub and
  * peon, CheckAccessible() has been recursively designed ("goddamned son
  * of a...") to follow those hubs, assigning the current hub (the one we
  * started with) as the routing node.
  */
  for (Hr = Systems; Hr != NULL; Hr = Hr->next)
    {
    for (Kr = Hr->list; Kr != NULL; Kr = Kr->next)
      {
      if (strCmp(Kr->Id, OurId) == SAMESTRING)
        {
        found = TRUE;
        if (SetUpHub(Hr))
          {
          CheckAccessible(Hr, Hr, Kr, 1);
          if (CurHubNet != -1) putNet(CurHubNet, &netTemp);

          }

        }

      }

    }
  if (!found)
    {
    printf("ERROR: We do not seem to be anywhere on the map.\n");

    }

  }
/*
* ReRoute()
*
* This function handles rerouting mail for nodes with changing routes.  If
* you're going to read this code, you must be familiar with how routed mail
* is being stored.
*/
char ReRoute(target)
int target;
  {
  /*
  * Here are the steps we need to take, and why, remembering that we
  * are seeing this node before its routing code has been changed:
  *
  * 1. If this system directly connected before, check to see if there
  *    is any outstanding mail.  If so, convert the local outstanding
  *    to route format and attach to the new route node's queue.
  *
  * 2. If this system directly connected before, check to see if there
  *    is any mail being routed via this node to other nodes.  If so,
  *    scan this mail to discover targets.  For each target, discover
  *    if we may reroute through another node.  If not, then we either
  *    abort the change for this node or discard the mail, depending
  *    on the command line arguments of the user.  If we abort, return
  *    FALSE.
  *
  * 3. If this system was not directly connected before, find the
  *    routed mail, using the old routing code in netBuf, and extract
  *    the routed Mail for rerouting.  We must also do a lot of renaming
  *    here.
  */
  if (DirectRoute(&netBuf))
    {
    /* first check for mail routing through this node. */
    if (!ChangeRouting(target)) return FALSE;
    /* now check local mail    */
    RouteLocalMail(target);

    }
  else
    {
    DoReRoute(target);

    }
  return TRUE;

  }
/*
* ReDoRouted()
*
* Dunno.
*/
char ReDoRouted(target)
int target;
  {
  SYS_FILE fn, fn2;
  label temp1, temp, Name, Id, TarId;
  int i, LastRoute;
  normId(netBuf.netId, TarId);
  if (!DirectRoute(&netBuf))
  DoReRoute(target);
  /* first check for truly routed mail */
  for (i = 0; ; i++)
    {
    sprintf(temp, "R%d.%d", thisNet, i);
    makeSysName(fn, temp, &cfg.netArea);
    if (IdRouteFile(fn, Id, Name))
      {
      if (strCmpU(TarId, Id) != SAMESTRING)
      return FALSE;

      }
    else break;

    }
  /* None?  Good.  Transfer to target, which is netTemp. */
  LastRoute = FindRouteName(target, fn2);
  for (i = 0; ; i++)
    {
    sprintf(temp, "R%d.%d", thisNet, i);
    makeSysName(fn, temp, &cfg.netArea);
    if (access(fn, 0) != 0) break;
    sprintf(temp, "R%d.%d", target, LastRoute++);
    makeSysName(fn2, temp, &cfg.netArea);
    rename(fn, fn2);

    }
  netBuf.nbflags.HasRouted = FALSE;
  netBuf.nbHiRouteInd = 0;
  if (LastRoute != 0) netTemp.nbflags.HasRouted = TRUE;
  netTemp.nbHiRouteInd = LastRoute - 1;
  return TRUE;

  }
/*
* DoReRoute()
*
* This retrieve routed mail for this node from the router and pass to the new
* router, in netTemp.
*/
void DoReRoute(target)
int target;
  {
  int		rover = 0, i, LastRoute;
  label	temp1, temp, Id, Name, netId, temp2;
  SYS_FILE	fn, fn2;
  char	finished = FALSE;
  LastRoute = FindRouteName(target, fn);
  getNet(netBuf.nbRoute, &nBuf1);
  normId(netBuf.netId, netId);
  do
    {
    sPrintf(temp, "R%d.%d", netBuf.nbRoute, rover);
    makeSysName(fn, temp, &cfg.netArea);
    if (!IdRouteFile(fn, Id, Name))
    finished = TRUE;
    else
      {
      if (strCmpU(Id, netId) == SAMESTRING)
        {
        sPrintf(temp, "R%d.%d", target, LastRoute++);
        makeSysName(fn2, temp, &cfg.netArea);
        rename(fn, fn2);
        CoverRouteHole(netBuf.nbRoute, rover);

        }
      else rover++;

      }

    }
  while (!finished);
  if (rover == 0) nBuf1.nbflags.HasRouted = FALSE;
  nBuf1.nbHiRouteInd = rover;
  putNet(netBuf.nbRoute, &nBuf1);
  if (LastRoute != 0) netTemp.nbflags.HasRouted = TRUE;
  netTemp.nbHiRouteInd = LastRoute - 1;

  }
/*
* ChangeRouting()
*
* This function is called when the route to a node is changing, so reroute
* mail.
*/
char ChangeRouting(target)
int target;
  {
  SYS_FILE fn, newfn;
  label    temp, Id, Name;
  int	rover = 0, LastRoute;
  /* Loop through all of the mail queued for this system. */
  do
    {
    /* setup name */
    sPrintf(temp, "R%d.%d", thisNet, rover++);
    makeSysName(fn, temp, &cfg.netArea);
    if (IdRouteFile(fn, Id, Name))
      {
      if (strCmpU(OurId, Id) == SAMESTRING)
      ;	/* do nothing - sloppy programming, but is explicit */
      else if (searchNet(Id, &nBuf1) == ERROR)
        {
        printf(SURPRISE_ROUTE, Name, Id);
        if (DiscardMail)
          {
          printf(DISCARD_UNKNOWN);
          unlink(fn);

          }
        else return FALSE;    /* abort */

        }
      else return FALSE;

      }
    else break;	/* no more files found */

    }
  while (TRUE);
  /*
  * if we make it here, we just need to move the mail to the new
  * router
  */
  LastRoute = FindRouteName(target, fn);
  getNet(target, &nBuf1);
  rover = 0;
  do
    {
    /* setup name */
    sPrintf(temp, "R%d.%d", thisNet, rover++);
    makeSysName(fn, temp, &cfg.netArea);
    if (access(fn, 0) != 0) break;
    sPrintf(temp, "R%d.%d", target, LastRoute++);
    makeSysName(newfn, temp, &cfg.netArea);
    rename(fn, newfn);

    }
  while (TRUE);
  if (LastRoute != 0) nBuf1.nbflags.HasRouted = TRUE;
  nBuf1.nbHiRouteInd = LastRoute - 1;
  putNet(target, &nBuf1);
  netBuf.nbflags.HasRouted = FALSE;
  netBuf.nbHiRouteInd = 0;
  return TRUE;

  }
/*
* FindRouteName()
*
* This function finds the next route filename in sequence.
*/
int FindRouteName(slot, newfn)
int slot;
SYS_FILE newfn;
  {
  int rover = 0;
  label temp;
  /* Find next unused route name */
  do
    {
    sPrintf(temp, "R%d.%d", slot, rover++);
    makeSysName(newfn, temp, &cfg.netArea);

    }
  while (access(newfn, 0) != -1);
  return rover - 1;

  }
/*
* RouteLocalMail()
*
* This function takes direct mail to a node and makes it route mail. The
* kludginess of this mess makes me wonder if direct mail should be
* automatically generated into a file, rather than just using pointers as we
* do currently.  But then what would happen for small systems?  Ah, the fun
* of system constraints.
*/
void RouteLocalMail(target)
int target;
  {
  FILE *fd;
  label temp;
  SYS_FILE fn, rn;
  int rover;
  extern void (*NetPrintTarget)(), ToFile(char *fmt, ...);
  extern int (*ToFileWork)(int c);
  struct netMLstruct ml;
  if (!netBuf.nbflags.normal_mail) return;
  callSlot = thisNet;
  NetPrintTarget = ToFile;
  sPrintf(temp, "%d.ml", thisNet);
  makeSysName(fn, temp, &cfg.netArea);
  if ((fd = safeopen(fn, READ_ANY)) == NULL)
    {
    printf(ERROR_LOCAL_MAIL, netBuf.netName);
    netBuf.nbflags.normal_mail = FALSE;
    return ;

    }
  rover = FindRouteName(target, rn);
  sPrintf(temp, "R%d.%d", target, rover);
  makeSysName(rn, temp, &cfg.netArea);
  if ((wfd = safeopen(rn, WRITE_ANY)) == NULL)
    {
    printf("BAD ERROR: Couldn't open routing file '%s'!\n", rn);
    return;

    }
  ToFileWork = putRouteChar;
  ToFile("%-20s", netBuf.netId);
  ToFile("%-20s", netBuf.netName);
  ToFileWork = Encode;
  while (getMLNet(fd, ml))
    {
    if (findMessage(ml.ML_loc, ml.ML_id, TRUE))
      {
      StartEncode(putRouteChar);
      prNetStyle(0, Encode, FALSE, "");
      StopEncode();

      }

    }
  ToFileWork = putRouteChar;
  fclose(wfd);
  fclose(fd);
  unlink(fn);
  netBuf.nbflags.normal_mail = FALSE;

  }
/*
* putRouteChar()
*
* This function puts a character to a file.
*/
int putRouteChar(c)
int c;
  {
  putc(c, wfd);
  return TRUE;

  }
/*
* DirectRoute()
*
* This function determines if the given node is a direct or indirect route
* for us.
*/
char DirectRoute(n)
NetBuffer *n;
  {
  if (n->nbRoute == -1) return TRUE;
  if (n->nbRoute >= cfg.netSize) return TRUE;
  if (!netTab[n->nbRoute].ntflags.in_use) return FALSE;
  return (n->nbRouteGen != netTab[n->nbRoute].ntGen);

  }
/*
* getNetChar()
*
* This function gets a net file byte for us.
*/
int getNetChar()
  {
  int c;
  if ((c = getc(upfd)) == EOF) return -1;
  return c;

  }
/*
* SepNameSystem()
*
* This function handles a Who Else string for us.
*/
char SepNameSystem(string, person, system, buf)
NetBuffer *buf;
char *string, *person, *system;
  {
  char *c;
  int  n;
  char work[NAMESIZE * 3];	/* should be sufficient */
  strCpy(work, string);
  if ((c = strchr(work, '@')) == NULL)
  return NOT_SYSTEM;
  /* find leading spaces of "name @ system" format */
  for (n = 1; c[n] == ' ' && c[n]; n++)
  ;
  if (strLen(c + n) >= NAMESIZE)
  return BAD_FORMAT;
  strCpy(system, c + n);
  if (searchNameNet(c + n, buf) == ERROR)  /* bad - set signal of it */
  return NO_SYSTEM;
  *c = 0;
  CleanEnd(work);	/* kill trailing spaces */
  if (strLen(work) >= NAMESIZE)
  return BAD_FORMAT;
  strCpy(person, work);
  return IS_SYSTEM;

  }
#ifdef NEEDED
/************************************************************************/
/* CleanEnd() cleans up a message trailer for later display via    */
/*            Continue or .EH.  Inspired by Glen Heinz (MacCitadel)*/
/************************************************************************/
char *CleanEnd(text)
char *text;
  {
  char *ptr;
  int  wc, lc;
  if (strLen(text) == 0) return text;
  ptr = lbyte(text) - 1;      /* End of text of msg */
  /* Strip trailing whitespace */
  while ((*ptr == ' ' || *ptr == NEWLINE) && ptr != text)
  ptr--;
  ptr++;
  *ptr = 0;           /* tie it off with a NULL */
  /* Now we want to find a "preferred place" */
  for (wc = lc = 0, ptr--; wc < 4 && ptr != text && lc < 35; ptr--, lc++)
    {
    if (*ptr == ' ') wc++;
    if (*ptr == NEWLINE) break;     /* can't go beyond embedded NEWLINE */

    }
  if (ptr == text) return ptr;
  /* else */       return (ptr + 1);

  }
/************************************************************************/
/* lbyte() finds 0 byte of a string, returns pointer to it...      */
/************************************************************************/
char *lbyte(l)
char *l;
  {
  while (*l) l++;
  return l;

  }
#endif
/*
* PrintRouteIds()
*
* Prints something for us.
*/
void PrintRouteIds()
  {
  label temp, Name, Id;
  SYS_FILE fn;
  int i = 0;
  do
    {
    sprintf(temp, "R%d.%d", thisNet, i++);
    makeSysName(fn, temp, &cfg.netArea);
    if (!IdRouteFile(fn, Id, Name)) break;
    printf("%s (%s)\n", Name, Id);

    }
  while (TRUE);

  }
/*
* IdRouteFile()
*
* This tries to get the routing information for a piece of mail.
*/
int IdRouteFile(fn, Id, Name)
SYS_FILE fn;
label Id, Name;
  {
  label temp;
  if ((upfd = fopen(fn, READ_ANY)) == NULL) return FALSE;
  getMsgStr(getNetChar, temp, NAMESIZE);
  getMsgStr(getNetChar, Name, NAMESIZE);
  normId(temp, Id);
  NormStr(Name);
  fclose(upfd);
  return TRUE;

  }
/*
* CheckArea()
*
* This function checks to see if the area codes of two systems match.
*/
void *CheckArea(Hop *d, char *code)
  {
  return (strCmp(code, d->Area) == 0) ? d : NULL;

  }
/*
* AddNewHop()
*
* Dunno.
*/
void AddNewHop(char *Id)
  {
  Hop *ThisCode;
  HopNode *Node;
  char *ac;
  ac = GetDynamic(4);
  AreaCode(Id, ac);
  if ((ThisCode = SearchList(&AreaList, ac)) == NULL)
    {
    ThisCode = GetDynamic(sizeof *ThisCode);
    ThisCode->Area = ac;
    InitListValues(&ThisCode->Systems, HopSearch, NULL, NULL, NULL);
    AddData(&AreaList, ThisCode, NULL, FALSE);

    }
  else
    {
    free(ac);
    if ((Node = SearchList(&ThisCode->Systems, Id)) != NULL)
    return;

    }
  Node = GetDynamic(sizeof *Node);
  Node->Id = strdup(Id);
  Node->hopcount = 10000;
  AddData(&ThisCode->Systems, Node, NULL, FALSE);

  }
/*
* AreaCode()
*
* This function extracts the area code from the node ID.
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
* SetHopCount()
*
* This function sets a hop count on a node.
*/
void SetHopCount(char *Id, int hopcount, char *RouteId)
  {
  HopNode *Node;
  if ((Node = GetHopNode(Id)) == NULL)
    {
    printf("Failure in SetHopCount!: %s", Id);
    exit(1);

    }
  Node->hopcount = hopcount;
  Node->RouteId = RouteId;

  }
/*
* GetHopNode()
*
* This function searches for the specified hop node.
*/
HopNode *GetHopNode(char *Id)
  {
  char ac[5];
  Hop *ThisCode;
  HopNode *Node;
  AreaCode(Id, ac);
  if ((ThisCode = SearchList(&AreaList, ac)) == NULL)
    {
    printf("Failed at area search (%s)\n", ac);
    return NULL;

    }
  else
  return SearchList(&ThisCode->Systems, Id);

  }
/*
* IsLowerHopCount()
*
* This function tries to discover if the hop count for the specified node is
* lower than the specified hop count.
*/
char IsLowerHopCount(char *Id, int HopCount)
  {
  HopNode *Node;
  if ((Node = GetHopNode(Id)) == NULL)
    {
    printf("Failure in IsLowerHopCount: %s", Id);
    exit(1);

    }
  return HopCount < Node->hopcount;

  }
/*
* FindHopCount()
*
* This function tries to find the specified hop count.
*/
int FindHopCount(char *Id)
  {
  HopNode *Node;
  if ((Node = GetHopNode(Id)) == NULL)
    {
    printf("Failure in IsLowerHopCount: %s", Id);
    exit(1);

    }
  return Node->hopcount;

  }
/*
* HopSearch()
*
* This compares a hop node and an identifier.
*/
void *HopSearch(HopNode *d, char *id)
  {
  if (strcmp(id, d->Id) == 0) return d;
  return NULL;

  }
/*
* FindDomainH()
*
* This function will help find the named domain handler.
*/
static void *FindDomainH(DomainHandler *data, char *name)
  {
  return (strCmpU(data->domain, name) == SAMESTRING) ? data : NULL;

  }
/*
* EatDomainH()
*
* This function will eat a line from ctdldmhd.sys.
*/
static void *EatDomainH(char *line)
  {
  DomainHandler *data;
  char *c, *d;
  label temp;
  if ((c = strchr(line, '#')) != NULL) *c = 0;	/* kill '#' */
  if (strlen(line) <= 3) return NULL;
  if ((c = strchr(line, ' ')) == NULL)
    {
    printf("WARNING: badly formed ctdldmhd.sys.\n");
    return NULL;

    }
  *c++ = 0;
  if ((d = strchr(c, ':')) == NULL)
    {
    printf("WARNING: badly formed ctdldmhd.sys.\n");
    return NULL;

    }
  *d++ = 0;
  NormStr(c);
  data = GetDynamic(sizeof *data);
  data->domain = strdup(line);
  data->name = strdup(c);
  normId(d, temp);
  data->nodeId = strdup(temp);
  return data;

  }
/*
* HandleDomains()
*
* This function is charged with making sure we	know how to get to each of the
* domain handlers we know about.
*/
void HandleDomains()
  {
  void SetHandlers();
  RunList(&DomainHandlers, SetHandlers);

  }
/*
* SetHandlers()
*
* This function will set up the given handler if we don't know about it yet.
*/
void SetHandlers(DomainHandler *data)
  {
  HopNode *Node;
  HUB   *Hub;
  PEONS *Peon;
  int   slot;
  /* is this us?  Then forget it */
  if (strCmpU(data->nodeId, OurId) == SAMESTRING) return;
  /*
  * first, see if this domain handler is on our list.  If not, then
  * add him in.
  */
  if (searchNet(data->nodeId, &netBuf) == ERROR)
    {
    if (FindHub(Systems, data->nodeId) == NULL &&
    FindGlobalPeon(Systems, data->nodeId) == NULL)
      {
      printf("WARNING: Domain Handler %s _ %s is not on this routmail map!\n", data->name, data->domain);
      return;

      }
    /* looks cool, so add him in. */
    AddNewNode(data->name, data->nodeId, 1);

    }
  /* next check if we already have a route to him.  If so, then return */
  if (netBuf.nbflags.RouteLock) return;
  /*
  * next, see if our immediate path to him (that is, the node we connect
  * directly to) is on the net.  If we happen to connect directly to him,
  * the route id will be NULL.  We store the Route Id in the system's Hop
  * count record.
  */
  if ((Node = GetHopNode(data->nodeId)) == NULL)
    {
    printf("Failure in SetHandlers: %s", data->name);
    return ;

    }
  if (Node->RouteId != NULL)
    {
    if ((CurHubNet = searchNet(Node->RouteId, &netBuf)) == ERROR)
      {
      if ((Hub = FindHub(Systems, Node->RouteId)) == NULL &&
      (Peon = FindGlobalPeon(Systems, Node->RouteId)) == NULL)
        {
        printf("BUG: Our path node, %s, to Domain Handler %s _ %s is not on this routmail map!", Node->RouteId, data->name, data->domain);
        return;

        }
      /* looks cool, so add him in. */
      if (Hub != NULL)
      AddNewNode(Hub->Name, Hub->Id, 1);
      else
      AddNewNode(Peon->Name, Peon->Id, 1);
      CurHubNet = thisNet;

      }
    searchNet(Node->RouteId, &netTemp);
    SetNode(data->name, data->nodeId, 1, Node->RouteId, 0, FALSE);

    }

  }
/* stub */
void splitF(FILE *log, char *fmt, ...)
  {

  }
#define WeServe(x)	SearchList(&Serves, x)
/*
* LocalName()
*
* This function takes a string of form <system> _ <domain> and attempts to
* discover if this domain mapped system is actually a local.  This is used
* when we're sending mail and are trying to find out if a Who Else override
* needs to be generated.  Ugly	 * kludge, but, hey, that's what programming's
* all about, eh?
*/
char *LocalName(char *system)
  {
  char *domain, *System;
  if ((domain = strchr(system, '_')) == NULL) return system;
  domain += 2;	/* always preceded by a space -- or so we assume */
  if (strCmpU(domain, cfg.codeBuf + cfg.nodeDomain) == SAMESTRING ||
  WeServe(domain) != NULL)
    {
    System = strdup(system);
    if ((domain = strchr(System, ' ')) == NULL)
    return system;	/* should never happen, though */
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
