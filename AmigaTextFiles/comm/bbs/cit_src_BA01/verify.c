/*  Citadel Verify Database */
#include <stdio.h>
#include <string.h>
#include "ctdl.h"
#include "c68door.h"
extern CONFIG      cfg;            /* A buncha variables           */
extern logBuffer   logBuf;         /* Pippul buffer                */
extern FILE        *logfl;         /* log file descriptor          */
extern LogTable    *logTab;
extern int         thisLog;
extern FILE            *netfl, *roomfl;
extern NetBuffer   netBuf;
extern NetTable    *netTab;
extern rTable      *roomTab;         /* RAM index of rooms              */
extern aRoom        roomBuf;         /* room buffer    */
extern EVENT       *EventTab;        /* Events Table */
extern int          TopFloor;        /* limit on floor */
extern struct floor  *FloorTab;
FILE            *netLog=stdout;

#define max(x,y)        ((x) > (y) ? (x) : (y))

void Process_Msg(MessageBuffer  *msg);
void Do_Room_Stats(long total);
void Do_Message(MessageBuffer  *);
void Verify_Messages(void);
int MsgLen(MessageBuffer  *);
void ShowHash(void);
void showlog(int);
char *LastOn(long lastdate, char s);
void Do_Boolean(char *,short,short);
void Verify_Config(void);
void Verify_Log(void);
void Verify_Room(void);
void Verify_Floor(void);
void Verify_Events(void);
void crashout(char *);
void Check_codeBuf(char *,long,short);
void Check_Shared(void);
void Do_nflags(struct nflags *);
void Do_Multi_Net_Data(MULTI_NET_DATA);
void Do_Shared_Room(SharedRoom *);
void Do_Door_Data(void);
void Do_Net_Data(void);
void Do_NetBuf_Data(int slot);
void Process_Other_Room_Data(int Index);

short Vflag;   /* TRUE - Verbose output( config) */
short Lflag;   /* TRUE - Verbose output( user log) */
short Nflag;   /* TRUE - Verbose output( ctdlnet.sys ) */
short Rflag;   /* TRUE - Verbose output( Room data ) */
short Dflag;   /* TRUE - Verbose output(Doors data ) */
short Mflag;   /* TRUE - Verbose output(Message data ) */
short Eflag;   /* TRUE - Process Event data */
short Fflag;   /* TRUE - Process Floor Table */
short sfflag;  /* TRUE - Message Dump is Multiple line */

char MControl;    /* selective dump control */
int  mPrintf(char *format, ...) {return 0; }  /* stub to quiet the linker */


void crashout(str)
char *str;
  {
  printf(str);
  exit(10);

  }
int  main(int,char **);
int  main(argc,argv)
int  argc;
char **argv;
  {
  SYS_FILE fn;
  /* Process CONFIG  */
  cfg.weAre = UTILITY;
  sfflag = TRUE;
  if( argc > 1 )
    {
    Fflag = Eflag = Dflag = Rflag = Vflag = Lflag = Mflag = Nflag = FALSE;
    MControl = '\0';
    }
  else Fflag =  Eflag = Dflag = Rflag = Vflag = Lflag = Mflag = Nflag = TRUE;  /* default is all */
  printf("Citadel Verify Database Utility Version %s\n", VERSION_NAME);
  for( ;--argc>0; )
    {
    if( *argv[argc] == '-' )
      {
      switch ( argv[argc][1])
        {
        case 'a' :
        case 'A' : Fflag = Mflag = Eflag = Dflag = Lflag = Vflag = Rflag = Nflag = TRUE;break;
        case 'd' :
        case 'D' : Dflag = TRUE;break;
        case 'e' :
        case 'E' : Eflag = TRUE;break;
        case 'f' :
        case 'F' : Fflag = TRUE;break;
        case 'l' :
        case 'L' : Lflag = TRUE;break;
        case 'm' :
        case 'M' :
          Mflag = TRUE;
          if( argv[argc][2] == 'a' ||  argv[argc][2] == 'A')
            MControl = 'A';
          if( argv[argc][2] == 'p' ||  argv[argc][2] == 'P')
            MControl = 'P';
          break;
        case 'n' :
        case 'N' : Nflag = TRUE;break;
        case 'r' :
        case 'R' : Rflag = TRUE;break;
        case 'v' :
        case 'V' : Vflag = TRUE;break;
        case 's' :
        case 'S' : sfflag = FALSE; break;
        default:
        printf("illegal option ignored:%s\n",argv[argc]);
        printf(" Valid options are:\n");
        printf("   -L User Log data  -V config data\n");
        printf("   -R room data      -M Messages data\n");
        printf("   -N net data       -E Event data        -S one line message data\n");
        printf("   -D Door Data      -F Floor data        -A All of the Above\n");
        };

      };

    };
  if (!readSysTab(FALSE, TRUE)) exit(100);
  Verify_Config();             /* Verify the data in the main database */
  /* Process the User Log */
  sprintf(fn, "%sctdllog.sys", &cfg.logArea);
  if ((logfl = fopen(fn, "rb")) == NULL)
    {
    printf("Can't open the Citadel log!\n");
    exit(1);

    }
  initLogBuf(&logBuf);
  Verify_Log();
  if( cfg.BoolFlags.netParticipant )
    {
    /* process network information */
    sprintf(fn, "%sctdlnet.sys", &cfg.netArea);
    openFile(fn, &netfl);
    initNetBuf(&netBuf);

    };
  /* open room file */
  sprintf(fn, "%sctdlroom.sys", &cfg.roomArea);
  openFile(fn, &roomfl);
  initRoomBuf(&roomBuf);
  Verify_Room();
  fclose(roomfl);
  /* close room file */
  Verify_Floor();
  Do_Door_Data();
  Verify_Messages();    /* process the messages */
  Verify_Events();      /* Event Data processing */
  return 0;
  }

struct  namelist
  {
  char *GenName;
  int  GenVal;

  };

typedef struct namelist GenList;

  /***  INDENT-OFF ***/
#define MAXDAYS  (8)
#define MAXTYPES (3)

static GenList EvnDays[MAXDAYS] =
  {
    {    "Sun", SUNDAYS     },    {    "Mon", MONDAYS     },
    {    "Tue", TUESDAYS    },    {    "Wed", WEDNESDAYS  },
    {    "Thu", THURSDAYS   },    {    "Fri", FRIDAYS     },
    {    "Sat", SATURDAYS   },    {    "All", ALL_DAYS    }
  };

static GenList EvnTypes[MAXTYPES] =
  {
    {    "preempt",     TYPREEMPT  },    {    "non-preempt", TYNON      },
    {    "quiet",       TYQUIET    }
  };


static GenList EvCls[EVENT_CLASS_COUNT] =
  {
    {    "network",             CLNET        },
    {    "external",            CLEXTERN     },
    {    "relative",            CLREL        },
    {    "dl-time",             CL_DL_TIME   },
    {    "anytime-net",         CL_ANYTIME_NET   },
    {    "door-limit",          CL_DOOR_TIME },
    {    "autodoor",            CL_AUTODOOR  },
    {    "chat-on",             CL_CHAT_ON   },
    {    "chat-off",            CL_CHAT_OFF  },
    {    "redirect",            CL_REDIRECT  },
    {    "newusers-allowed",    CL_NEWUSERS_ALLOWED   },
    {    "newusers-disallowed", CL_NEWUSERS_DISALLOWED   },
    {    "until-done-net",      CL_UNTIL_NET  },
    {    "netcache",            CL_NETCACHE   },
    };

 /**  INDENT-ON **/
char *fake = "Unknown nnn  ";

char *Look_Up(int value,  GenList list[], int limit )
 {
 int i;
 for( i=0; i<limit; i++) if( list[i].GenVal == value ) return list[i].GenName;
 sprintf(&fake[8],"%3d",value);
 return fake;
 }

void Verify_Floor()
  {
  int rover;
  for ( rover=0; rover < TopFloor; rover++)
    {
    if( FloorTab[rover].FlInuse )
      {
      printf(" Floor slot[%d] in use  Name:%20s Moderator:%s\n",
        rover, FloorTab[rover].FlName,
       ( FloorTab[rover].FlModerator[0] != '\0' ? FloorTab[rover].FlModerator : "No Moderator") );
      }
    else  printf(" Floor slot[%d] not in use\n",rover);

    };
  }

void Verify_Events()
  {
  int i,j;
  int hrs,mins;
  long minutes;
  int day;
  if( !Eflag )return;  /* exit if not requested */
  printf("\n\nThere are %d events\n",cfg.EvNumber);
  for( i=0; i< cfg.EvNumber; i++)
    {
    hrs = EventTab[i].EvDur / 60;
    mins= EventTab[i].EvDur % 60;
    printf("\nEvent Number: %d Duration:%d(%d:%02.2d)   ",i,EventTab[i].EvDur, hrs, mins);
    printf("Event Type: %s\n", Look_Up(EventTab[i].EvType,EvnTypes,MAXTYPES) );
    printf("Event Warning:%s\n",( cfg.codeBuf+EventTab[i].EvWarn) );
    printf("Event Class:%s   ",Look_Up(EventTab[i].EvClass,EvCls,EVENT_CLASS_COUNT) );
    printf("Event EvExitVal:%08.8lX :",EventTab[i].EvExitVal);
    for (j = 0; j < 32; j++)
      {
      if (( 1l << j ) & EventTab[i].EvExitVal) printf("%d ", j + 1);
      };
    printf("\n");
    hrs = EventTab[i].EvDur / 60;
    mins= EventTab[i].EvDur % 60;
    printf("Event Minutes: %d(%d:%02.2d)\n",EventTab[i].EvMinutes, hrs, mins);
    day = 1;
    minutes = EventTab[i].EvMinutes;
    while( minutes >= 1440)
      {
      day <<= 1;
      minutes -= 1440;
      };
    hrs = minutes / 60;
    mins= minutes % 60;
    printf("Event Day: %s  Start Time:%ld(%d:%02.2d)\n",Look_Up(day,EvnDays, MAXDAYS), minutes, hrs, mins);
    switch (EventTab[i].EvClass)
      {
      case CL_ANYTIME_NET:
        printf("Anytime DeadTime: %ld  Anytime Duration: %d\n",
        EventTab[i].vars.Anytime.EvDeadTime, EventTab[i].vars.Anytime.EvAnyDur);
        break;
      case CL_AUTODOOR:
        printf("AutoDoor for User:%s\n", EventTab[i].vars.EvUserName);
        break;
      case CL_REDIRECT:
        printf("Redirect incoming: System:%s\n"
        ,EventTab[i].vars.Redirect.EvSystem);
        printf("Directory:%s Filename:%s\n",
        (EventTab[i].vars.Redirect.EvHomeDir + cfg.codeBuf)
        ,EventTab[i].vars.Redirect.EvFilename);
        break;
      };
    };
  }



void Verify_Room()
  {
  short Index;
  for (Index = 0; Index < MAXROOMS; Index++)
    {
    if( Rflag )printf("\nRoom Slot #%d:",Index);
    if(   roomTab[Index].rtflags.INUSE )
      {
      if( Index < 3 )
        {
        if( roomTab[Index].rtFlIndex != 0 )
          {
          printf("\nInitial Room not on Base Floor!\n");

          };

        };
      if( Rflag )
        {
        printf("%20s Last Message:%ld Last Netted message:%ld\n",
        roomTab[Index].rtname,roomTab[Index].rtlastMessage,roomTab[Index].rtlastNet);
        printf("Floor Index:%d\n", roomTab[Index].rtFlIndex);
        if( Index != MAILROOM)Process_Other_Room_Data(Index);
        };
      }
    else if( Rflag )printf(" Not in use at this time\n");

    };
  if( Nflag )Check_Shared();

  }
void Do_Net_Data()
  {
  printf("\nNode Name:%-22s  Node Id: %s\n",netBuf.netName, netBuf.netId);
  printf("Short Name:%c%c%c  Passwords: Our(%s) Their(%s)\n",
  (isprint(netBuf.nbShort[0]) ? netBuf.nbShort[0] : '*'),
  (isprint(netBuf.nbShort[1]) ? netBuf.nbShort[1] : '*'),
  (isprint(netBuf.nbShort[2]) ? netBuf.nbShort[2] : '*'),
  netBuf.OurPwd, netBuf.TheirPwd);
  printf("baudCode:%c nbGen: %c access:%s\n",
  netBuf.baudCode, netBuf.nbGen, netBuf.access);
  printf("nbRoute:%d nbRouteGen:%d nbHiRouteInd: %d nbLastConnect: %x\n",
  netBuf.nbRoute, netBuf.nbRouteGen, netBuf.nbHiRouteInd, netBuf.nbLastConnect);
  }

void Check_Shared()
  {
  short netrover;
  for( netrover=0; netrover < cfg.netSize; netrover++)
    {
    getNet(netrover,&netBuf);
    Do_NetBuf_Data(netrover);
    printf("\nnetTab[%d]:",netrover);
    if( netTab[netrover].ntflags.in_use )
      {
      printf(" In use\n");
      printf(" ntnmhash: %d  ntidhash: %d  Short name:%c%c%c Generation: %d\n",
      netTab[netrover].ntnmhash,netTab[netrover].ntidhash,
      (isprint(netTab[netrover].ntShort[0]) ? netTab[netrover].ntShort[0] : '*'),
      (isprint(netTab[netrover].ntShort[1]) ? netTab[netrover].ntShort[1] : '*'),
      (isprint(netTab[netrover].ntShort[2]) ? netTab[netrover].ntShort[2] : '*'),
      netTab[netrover].ntGen);
      Do_nflags(&netTab[netrover].ntflags);
      Do_Multi_Net_Data(netTab[netrover].ntMemberNets);
      Do_Shared_Room(netTab[netrover].netTRooms);
      Do_Net_Data();
      }
    else
      {
      printf("Not in use\n");

      };

    };

  }
void Do_nflags(ntflags)
struct nflags *ntflags;
  {
  Do_Boolean("normal_mail   ",ntflags->normal_mail,FALSE);
  Do_Boolean("in_use        ",ntflags->in_use,FALSE);
  Do_Boolean("room_files    ",ntflags->room_files,TRUE);
  Do_Boolean("local         ",ntflags->local,FALSE);
  Do_Boolean("spine         ",ntflags->spine,FALSE);
  Do_Boolean("send_files    ",ntflags->send_files,TRUE);
  Do_Boolean("is_spine      ",ntflags->is_spine,FALSE);
  Do_Boolean("OtherNet      ",ntflags->OtherNet,FALSE);
  Do_Boolean("HasRouted     ",ntflags->HasRouted,TRUE);
  Do_Boolean("RouteFor      ",ntflags->RouteFor,FALSE);
  Do_Boolean("RouteTo       ",ntflags->RouteTo,FALSE);
  Do_Boolean("Stadel        ",ntflags->Stadel,TRUE);
  Do_Boolean("RouteLock     ",ntflags->RouteLock,FALSE);
  Do_Boolean("ExternalDialer",ntflags->ExternalDialer,FALSE);
  Do_Boolean("NoDL          ",ntflags->NoDL,TRUE);
  Do_Boolean("MassTransfer  ",ntflags->MassTransfer,FALSE);
  Do_Boolean("Zip           ",ntflags->Zip,FALSE);
  Do_Boolean("Zoo           ",ntflags->Zoo,TRUE);
  Do_Boolean("Arc           ",ntflags->Arc,FALSE);
  Do_Boolean("Lha           ",ntflags->Lha,FALSE);

  }
void Do_Multi_Net_Data(MN)
MULTI_NET_DATA MN;
  {
  short i;
  printf(" Member of Nets:");
  for(i=0; i<MAX_NET-1;i++) if( ((1<<i) & MN) !=0 )printf(" %d",i+1);
  printf("\n");
  if( (MN & 0x80000000) != 0)printf("Priority Mail\n");

  }
void Do_Shared_Room(nRms)
SharedRoom *nRms;
  {
  int slot;
  for(slot = 0; slot < SHARED_ROOMS; slot++)
    {
    if( (nRms->srgen & 0x8000) )
      {
      printf(" \nSR[%d]:  lM:%ld  Mode:",slot,  nRms->lastMess);
      switch (CGetMode(nRms->mode))
        {
        case            PEON: printf("Peon            "); break;
        case        REG_HOST: printf("Reg Host        "); break;
        case        BACKBONE: printf("Backbone        "); break;
        case   PASS_BACKBONE: printf("Passive Backbone"); break;
        case ACTIVE_BACKBONE: printf("Active Backbone "); break;
        default:              printf("Unknown");
        };
      printf("  F:%s srgen:%x srslot: %d",
      (GetFA(nRms->mode) != 0 ? "YES":"NO "),nRms->srgen,nRms->srslot);
      };
      nRms++;
    };
  }
void Check_codeBuf(str,data,control)
char *str;
long data;
short control;   /* TRUE - Newline if verbose operation */
  {
  if( data >= MAXCODE )
    {
    printf("\nInvalid values(%d) for offset:%s\n",data,str);

    }
  else
    {
    if(Vflag)
      {
      char *d;
      d = &cfg.codeBuf[0];
      printf("%-20s(%ld):%s%c",str,data,
      (data == 0 ? "Undefined" : &d[data]),(control ? '\n':' '));

      };

    };

  }
void Verify_Config()
  {
  short i;
  if( Vflag )
    {
    printf("\nmaxMSector:%d ",cfg.maxMSector);
    printf("Message Old::%ld New:%ld\n",cfg.oldest,cfg.newest);

    };
  Check_codeBuf("Node Name",  cfg.nodeName,TRUE);
  Check_codeBuf("Node Id",    cfg.nodeId,FALSE);
  Check_codeBuf("Node Domain",cfg.nodeDomain,TRUE);
  Check_codeBuf("Node Title", cfg.nodeTitle,TRUE);
  Check_codeBuf("Main Floor", cfg.MainFloor,FALSE);
  Check_codeBuf("Base Room",  cfg.bRoom, TRUE);
  for(i=0;i<7;i++)
    {
    Check_codeBuf("Dial Prefix",cfg.DialPrefixes[i],TRUE);

    };
  Check_codeBuf("Dial Suffix",cfg.netSuffix,TRUE);
  if( Vflag )
    {
    printf("Max File:%d Area Size: %d receptArea:%s\n",
    cfg.maxFileSize, cfg.sizeArea, &cfg.receptArea);
    printf("Sysop:%s CryptSeed:%d InitColumns:%d LoginAttempts:%d Audit: %d(%s)\n",
    cfg.SysopName,cfg.cryptSeed,cfg.InitColumns,cfg.LoginAttempts,
    cfg.Audit,( cfg.Audit == 0 ? "None":(cfg.Audit == 1? "Normal":"No Net") ) );
    printf("  homeArea:%s\n",&cfg.homeArea);
    printf("   msgArea:%s\n",&cfg.msgArea);
    printf("  msg2Area:%s\n",&cfg.msg2Area);
    printf("   logArea:%s\n",&cfg.logArea);
    printf("  roomArea:%s\n",&cfg.roomArea);
    printf("   netArea:%s\n",&cfg.netArea);
    printf("domainArea:%s\n",&cfg.domainArea);
    printf(" auditArea:%s\n",&cfg.auditArea);
    printf(" floorArea:%s\n",&cfg.floorArea);
    printf("  holdArea:%s\n",&cfg.holdArea);
    printf("sysBaud:%d netSiz:%d DomainHandlers:%d\n"
    ,cfg.sysBaud,cfg.netSize,cfg.DomainHandlers);

    };
  Check_codeBuf("Mail Hub",cfg.MailHub,TRUE);
  if( Vflag )
    {
    printf("Domain display Format:%s\n",&cfg.DomainDisplay[0]);
    printf("EvNumber:%d weAre:%d paramVers:%d\n",cfg.EvNumber,cfg.weAre,cfg.paramVers);
    printf("MaxLog size:%d ",cfg.MAXLOGTAB);
    printf("Mail  Slots:%d ",cfg.MailSlots);
    printf("Msgs Per room:%d\n",cfg.MsgsPerrm);
    printf("Max Rooms:%d ",cfg.MaxRooms);
    printf("Shared Rooms:%d\n",cfg.SharedRooms);
    printf("Next msg slot: %d/%d Anon Mail Max Length:%d\n",
    cfg.catChar,cfg.catSector,cfg.AnonMailLength);
    printf("Log entries are %d in size\n",cfg.sizeLTentry);
    printf("Console Timeout:%d\n",cfg.ConTimeOut);
    printf("Sysop Archive:%s\n",cfg.SysopArchive);
    Do_Boolean("HoldOnLost    ", cfg.BoolFlags.HoldOnLost    ,FALSE);
    Do_Boolean("mirror        ", cfg.BoolFlags.mirror        ,FALSE);
    Do_Boolean("unlogEnterOk  ", cfg.BoolFlags.unlogEnterOk  ,TRUE);
    Do_Boolean("unlogReadOk   ", cfg.BoolFlags.unlogReadOk   ,FALSE);
    Do_Boolean("nonAideRoomOk ", cfg.BoolFlags.nonAideRoomOk ,FALSE);
    Do_Boolean("noMail        ", cfg.BoolFlags.noMail        ,TRUE);
    Do_Boolean("noChat        ", cfg.BoolFlags.noChat        ,FALSE);
    Do_Boolean("netParticipant", cfg.BoolFlags.netParticipant,FALSE);
    Do_Boolean("aideSeeAll    ", cfg.BoolFlags.aideSeeAll    ,TRUE);
    Do_Boolean("debug         ", cfg.BoolFlags.debug         ,FALSE);
    Do_Boolean("NetDft        ", cfg.BoolFlags.NetDft        ,FALSE);
    Do_Boolean("SysopEditor   ", cfg.BoolFlags.SysopEditor   ,TRUE);
    Do_Boolean("IsDoor        ", cfg.BoolFlags.IsDoor        ,FALSE);
    Do_Boolean("RouteMail     ", cfg.BoolFlags.RouteMail     ,FALSE);
    Do_Boolean("DoorDft       ", cfg.BoolFlags.DoorDft       ,TRUE);
    Do_Boolean("AnonSessions  ", cfg.BoolFlags.AnonSessions  ,FALSE);
    Do_Boolean("DL_Default    ", cfg.BoolFlags.DL_Default    ,FALSE);
    Do_Boolean("NetScanBad    ", cfg.BoolFlags.NetScanBad    ,TRUE);
    printf("SysOp Editor:%s\n",cfg.DepData.Editor);
    printf("SysOp Editor Area:%s\n",cfg.DepData.EditArea);
    printf("Sysdependant Modem:%s\n",cfg.DepData.ModemSetup);
    printf("Sysdependant Modem Init:%s\n",cfg.DepData.HiSpeedInit);
    printf("Sysdependant Modem Unit %d Device:%s\n"
    , cfg.DepData.UnitNumber, cfg.DepData.DevName);
    printf("Sysdependant clock:%d\n",cfg.DepData.Clock);
    printf("Sysdependant Lockport:%d\n",cfg.DepData.LockPort);
    printf("Sysdependant width: %d Height: %d Depth: %d\n",
    cfg.DepData.ScreenWidth,cfg.DepData.ScreenHeight,cfg.DepData.ScreenDepth);
    printf("Sysdependant Colors:%d %d\n",cfg.DepData.Color0, cfg.DepData.Color1);

    };

  }
void Do_Boolean(str,flag,control)
char *str;
short flag,control;
  {
  printf("%s: %s%c",str,(flag!=0 ? "Yes  " : "No   "),(control ? '\n' : ' ') );

  }
void Verify_Log()
  {
  short i;
  initLogBuf(&logBuf);
  for ( i = 0; i < cfg.MAXLOGTAB; i++ )
    {
    getLog(&logBuf,i);
    if( Lflag )printf("\nlog:%-4d",i);
    if (logBuf.lbflags.L_INUSE)
      {
      showlog(i);
      if( logBuf.lbwidth < 40 || logBuf.lbwidth > 132 )
        {
        printf("Log:%d,  %s terminal width = %d\n",i,logBuf.lbname,logBuf.lbwidth);

        };
      if( logBuf.lbnulls > 50 )
        {
        printf("Log:%d,  %s terminal nulls = %d\n",i,logBuf.lbname,logBuf.lbnulls);

        };
      if( logBuf.credit < 0 || logBuf.credit > 1000)
        {
        printf("Log:%d,  %s Long Distance credits = %d\n",i,logBuf.lbname,logBuf.credit);

        };

      }
    else
      {
      if( Lflag )printf(" Not in use at this time\n");

      };

    };

  }
void showlog(i)
int i;
  {
  if( Lflag )
    {
    printf("%4d: %-20s",i,logBuf.lbname);
    printf("%-20s",logBuf.lbpw);
    ShowHash();
    printf(" %d col. Last Call:%s\n",logBuf.lbwidth, LastOn(logBuf.lblaston, TRUE));
    Do_Boolean("AIDE Status       ",logBuf.lbflags.AIDE      ,FALSE);
    Do_Boolean("Expert User       ",logBuf.lbflags.EXPERT    ,FALSE);
    Do_Boolean("Network Privilege ",logBuf.lbflags.NET_PRIVS ,TRUE);
    Do_Boolean("Door Privilege    ",logBuf.lbflags.DOOR_PRIVS,FALSE);
    Do_Boolean("Download Privilege",logBuf.lbflags.DL_PRIVS  ,FALSE);
    Do_Boolean("Permanent Account ",logBuf.lbflags.PERMANENT ,TRUE);
    Do_Boolean("Floor mode        ",logBuf.lbflags.FLOORS    ,FALSE);
    Do_Boolean("TWIT User         ",logBuf.lbflags.TWIT      ,FALSE);
    Do_Boolean("RUGGIE User       ",logBuf.lbflags.RUGGIE    ,TRUE);
    Do_Boolean("Active Account    ",logBuf.lbflags.L_INUSE   ,FALSE);
    Do_Boolean("LF FLAG           ",logBuf.lbflags.LFMASK    ,FALSE);
    Do_Boolean("OLD STYLE         ",logBuf.lbflags.OLDTOO    ,TRUE);
    Do_Boolean("HALF DUPLEX       ",logBuf.lbflags.HALF_DUP  ,FALSE);
    Do_Boolean("ALT. Read Messages",logBuf.lbflags.ALT_RE    ,FALSE);
    Do_Boolean("Room Name Prompt  ",logBuf.lbflags.NoPrompt  ,TRUE);

    };

  }
void ShowHash()
  {
  int i;
  for (i = 0; i < cfg.MAXLOGTAB; i++)
  if (logTab[i].ltlogSlot == thisLog)
    {
    printf("ltab=%02d, hash=%02d\n ", logTab[i].ltpwhash, hash(logBuf.lbpw));

    }

  }
void Do_Door_Data()
  {
  FILE        *fd;
  SYS_FILE    name;
  DoorData    DoorInfo;
  extern char *READ_ANY;
  short flag;
  makeSysName(name, DOOR_DATA, &cfg.roomArea);
  if ((fd = safeopen(name, READ_ANY)) == NULL)
    {
    if( Dflag )printf("No doors appear to be available.\n ");
    return;

    };
  /* process each door */
  while ( fread((char *)&DoorInfo, sizeof DoorInfo, 1, fd) != 0)
    {
    if( Dflag)
      {
      printf("\nDoor Code:%s Program:%s Room:%s\n"
      ,DoorInfo.entrycode,DoorInfo.program,DoorInfo.RoomName);
      printf("Parameters:%s\n",DoorInfo.parameters);
      printf("Description:%s\n",DoorInfo.description);
      printf("Location:%s TimeLimit:%d\n",DoorInfo.location,DoorInfo.TimeLimit);
      flag = (DOOR_AIDE    & DoorInfo.flags) ? 1:0;
      Do_Boolean("   AIDE",flag,FALSE);
      flag = (DOOR_SYSOP   & DoorInfo.flags) ? 1:0;
      Do_Boolean("  SYSOP",flag,FALSE);
      flag = (DOOR_CON     & DoorInfo.flags) ? 1:0;
      Do_Boolean("Console",flag,TRUE);
      flag = (DOOR_MODEM   & DoorInfo.flags) ? 1:0;
      Do_Boolean("  Modem",flag,FALSE);
      flag = (DOOR_AUTO    & DoorInfo.flags) ? 1:0;
      Do_Boolean("   Auto",flag,FALSE);
      flag = (DOOR_NEWUSER & DoorInfo.flags) ? 1:0;
      Do_Boolean("NewUser",flag,TRUE);
      printf("\n");

      };

    };

  }
/*
* doMessages()
*
* This function loops thru the msg file until finished.  It accumulates
* statistics, etc.
*/
extern MessageBuffer  msgBuf; /* The -sole- message buffer      */
extern FILE *msgfl, *msgfl2;

struct counts
  {
  struct counts *next;
  char name[20];    /* room name */
  long  messages;   /* number of messages in the room */
  long bytes;       /* total space the message uses */

  };
struct counts *Header = NULL;

void Process_Msg(MessageBuffer  *msg)
  {
  struct counts *ptr;
/*
** User names Statistics
*/
  if( Header == NULL )
    {
    Header = ptr = malloc(sizeof(struct counts));
    strcpy(ptr->name, msg->mbauth);
    ptr->messages = 0;
    ptr->bytes    = 0;
    ptr->next     = NULL;
    }
  else
    {
    ptr = Header;
    while( ptr != NULL )
      {
      if( strcmp(msg->mbauth,ptr->name)== 0 )break;
      ptr = ptr->next;
      };
    if( ptr == NULL )
      {
      ptr = malloc(sizeof(struct counts));
      strcpy(ptr->name, msg->mbauth);
      ptr->messages = 0;
      ptr->bytes    = 0;
      ptr->next     = Header;
      Header        = ptr;
      };
    };

  ptr->messages++;
  ptr->bytes += MsgLen(msg);
/*
** Room Statistics
*/
  ptr = Header;
  while( ptr != NULL )
    {
    if( strcmp(msg->mbroom,ptr->name)== 0 )break;
    ptr = ptr->next;
    };
  if( ptr == NULL )
    {
    ptr = malloc(sizeof(struct counts));
    strcpy(ptr->name, msg->mbroom);
    ptr->messages = 0;
    ptr->bytes    = 0;
    ptr->next     = Header;
    Header        = ptr;
    };
  ptr->messages++;
  ptr->bytes += MsgLen(msg);

  }

void Do_Room_Stats(long total)
  {
  struct counts *ptr;
  printf("Total of %ld messages\n",total);
  ptr = Header;
  while( ptr )
    {
    if( ptr->messages <= 0)ptr->messages = 1;
    printf("Name:%20s Messages:%10ld Bytes:%10ld Aver:%10ld\n"
    ,ptr->name,ptr->messages, ptr->bytes, ptr->bytes/ptr->messages);
    ptr = ptr->next;
    };
  }

void Verify_Messages()
  {
  MSG_NUMBER msg, firstMessage;
  MSG_NUMBER total;     /* For stat keeping. */
  extern struct mBuf mFile1;
  if( !Mflag ) return;
  fprintf(stderr, "Mulching...\n");
  InitMsgBase();
  startAt(msgfl, &mFile1, 0, 0);
  getMessage(getMsgChar, FALSE, TRUE, TRUE);
  firstMessage = atol(msgBuf.mbId);
  Do_Message(&msgBuf);
  msg = firstMessage -1;
  total = 1;
  while (msg != firstMessage)
    {
    total++;
    getMessage(getMsgChar, FALSE, TRUE, TRUE);
    Do_Message(&msgBuf);
    msg = atol(msgBuf.mbId);

    }
  Do_Room_Stats(total);

  }
void Do_Message(MessageBuffer  *msg)
  {
  Process_Msg(msg);
  if( MControl == 'A')
    {
    printf("\nMsg: %6d Sector ID: %6d Author:%s To:%s "
    ,  msg->mbheadChar, msg->mbheadSector, msg->mbauth, msg->mbto);
    printf("Name:%s ID:%s Room:%s",  msg->mboname,  msg->mborig,  msg->mbroom);
    printf("\n");
    }
  else if( MControl == 'P' )
    {
    printf("Message %4d Sector ID: %5d Author:%16s",  msg->mbheadChar, msg->mbheadSector, msg->mbauth);
    if( sfflag ) printf("\n");
    printf(" Date:%8s Time:%8s Local Id:%10s",  msg->mbdate,  msg-> mbtime,  msg-> mbId);
    if( sfflag ) printf("\n");
    printf(" Human:%15s ID:%14s Room:%15s",  msg->mboname,  msg->mborig,  msg->mbroom);
    if( sfflag ) printf("\n");
    printf(" Origin:%s To:%s",  msg->mbsrcId,  msg->mbto);
    if( sfflag ) printf("\n");
    printf(" Route:%s", msg->mbaddr);
    if( sfflag ) printf("\n");
    printf(" OtherNet:%s",msg->mbOther);
    if( sfflag ) printf("\n");
    printf(" reply:%s Domain:%0s",msg->mbreply, msg->mbdomain);
    printf("\n");
    };
  }
/*
* MsgLen()
*
* This function figures out the byte usage of the message.
*/
int MsgLen(MessageBuffer  *msg)
  {
  return  (int) (       strlen(msg->mbtext)  + strlen(msg->mbauth) +
  strlen(msg->mbdate)  + strlen(msg->mbtime) +
  strlen(msg->mbId)    + strlen(msg->mboname) +
  strlen(msg->mborig)  + strlen(msg->mbroom) +
  strlen(msg->mbsrcId) + strlen(msg->mbto) +
  strlen(msg->mbaddr)  + strlen(msg->mbOther) );

  }
void Do_NetBuf_Data(int slot)
  {
  printf("\nSlot:%2d Node Name:%-22s  Node Id: %s\n",slot,netBuf.netName, netBuf.netId);
  printf("Short Name:%c%c%c  Passwords: Our(%s) Their(%s)\n",
  (isprint(netBuf.nbShort[0]) ? netBuf.nbShort[0] : '*'),
  (isprint(netBuf.nbShort[1]) ? netBuf.nbShort[1] : '*'),
  (isprint(netBuf.nbShort[2]) ? netBuf.nbShort[2] : '*'),
  netBuf.OurPwd, netBuf.TheirPwd);
  printf("baudCode:%d nbGen: %d access:%s\n",
  (int)netBuf.baudCode, (int)netBuf.nbGen, netBuf.access);
  printf("nbRoute:%d nbRouteGen:%d nbHiRouteInd: %d nbLastConnect: %x\n",
  netBuf.nbRoute, netBuf.nbRouteGen, netBuf.nbHiRouteInd, netBuf.nbLastConnect);
  Do_nflags(&netBuf.nbflags);
  Do_Multi_Net_Data(netBuf.MemberNets);
  Do_Shared_Room(netBuf.netRooms);

  }

void  Process_Other_Room_Data(int Index)
  {
  int i;
  int count;
  MSG_NUMBER msgNo;
  getRoom(Index);
  printf("Slot: %d %s\n ",Index, roomBuf.rbname);
  printf("     rbgen:%d  rbFlIndex: %d ShareType:");
  switch( roomBuf.rbShareType)
    {
    case             PEON: printf("PEON\n"); break;
    case         REG_HOST: printf("REG_HOST\n"); break;
    case         BACKBONE: printf("BACKBONE\n"); break;
    case    PASS_BACKBONE: printf("PASSIVE\n"); break;
    case  ACTIVE_BACKBONE: printf("ACTIVE\n"); break;
    default:               printf("Unknown\n");
    };
  printf("Room Area:");
  if (roomBuf.rbflags.ISDIR)
    {
    printf("%s\n", &roomBuf.rbArea);
    }
  else printf("No Directory\n");
  Do_Boolean("Public            ",roomBuf.rbflags.PUBLIC,         FALSE);
  Do_Boolean("Directory         ",roomBuf.rbflags.ISDIR,          FALSE);
  Do_Boolean("Permanent         ",roomBuf.rbflags.PERMROOM,       TRUE);
  Do_Boolean("Skipped           ",roomBuf.rbflags.SKIP,           FALSE);
  Do_Boolean("Uploadable        ",roomBuf.rbflags.UPLOAD,         FALSE);
  Do_Boolean("Downloadable      ",roomBuf.rbflags.DOWNLOAD,       TRUE);
  Do_Boolean("Shared            ",roomBuf.rbflags.SHARED,         FALSE);
  Do_Boolean("Archived          ",roomBuf.rbflags.ARCHIVE,        FALSE);
  Do_Boolean("Anonymous         ",roomBuf.rbflags.ANON,           TRUE);
  Do_Boolean("Net Downloadable  ",roomBuf.rbflags.NO_NET_DOWNLOAD,FALSE);
  Do_Boolean("Invitational      ",roomBuf.rbflags.INVITE,         FALSE);
  Do_Boolean("Automaticly Netted",roomBuf.rbflags.AUTO_NET,       TRUE);
  Do_Boolean("All Users Netted  ",roomBuf.rbflags.ALL_NET,        FALSE);
  Do_Boolean("Read Only         ",roomBuf.rbflags.READ_ONLY,      TRUE);

  count = 0;
  for (i = 0;  i < MSGSPERRM;   i++)
    {
    msgNo = roomBuf.msg[i].rbmsgNo & S_MSG_MASK;
    if (msgNo >= cfg.oldest) count++;
    };
  printf("There are %d messages in this room\n");
  }
