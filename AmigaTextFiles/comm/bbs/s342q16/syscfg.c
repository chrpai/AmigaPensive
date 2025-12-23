/************************************************************************/
/*                              syscfg.c                                */
/*      configuration program for Citadel bulletin board system.  This  */
/* file contains the system dependent code!                             */
/************************************************************************/
#define SYSTEM_DEPENDENT
#define CONFIGURE
#include "ctdl.h"
#include "c68door.h"
#include "stdarg.h"
#include "dos.h"
#include "fcntl.h"
/************************************************************************/
/*                              History                                 */
/*                                                                      */
/* 89Oct17 HAW  Amiga.                                                  */
/* 89Jan15 HAW  Major release update: colors, etc....                   */
/* 87Jan19 HAW  Created.                                                */
/************************************************************************/
/************************************************************************/
/*                              Contents                                */
/*                                                                      */
/* #    dirExists()             check to see if directory exists        */
/* #    doCommon()              handles common stuff                    */
/*      GetDoorData()           reads in some piece of the door data    */
/*      initSysSpec()           initialization for system dependencies  */
/*      NextNonWhite()          finds next piece of non-whitespace      */
/*      NextWhite()             finds next piece of white space         */
/*      StoreDoor()             manages the reading a door              */
/*      SysDepIntegrity()       makes necessary checks for integrity    */
/*      sysSpecs()              System-dependent code for configure     */
/*                                                                      */
/*              # means local for this implementation                   */
/************************************************************************/
/************************************************************************/
/*              Statics                                                 */
/************************************************************************/

char onConsole=TRUE, remoteSysop=TRUE; /* dummies */


static int  necessary[MAX_NEEDED]   =
  {
  0, 0, 0, 0, 0, 0, 0, 0,   /* currently 16 fields */
  0, 0, 0, 0, 0, 0, 0, 0,
  };
static char curDir[80];
void *DoorName(DoorData *d, DoorData *s);
static SListBase DoorList  =
  {
  NULL, DoorName, NULL, free, NULL

  };
FILE *doorfl;
void totalBytes(long *size, FILE *fd);
/************************************************************************/
/*                External variable declarations in CONFG.C             */
/************************************************************************/
/***** THESE ARE REQUIRED DEFINITIONS! ******/
char *R_W_ANY     = "r+";
char *W_R_ANY     = "w+";
char *READ_ANY    = "r+";
char *READ_TEXT   = "r";
char *WRITE_ANY   = "w";
char *WRITE_TEXT  = "w";
/*****  done  *****/
extern CONFIG cfg;       /* The configuration variable           */
extern MessageBuffer   msgBuf;
extern rTable *roomTab;
void WrtDoor(DoorData *d);
/************************************************************************/
/*      dirExists() check to see if the directory exists                */
/************************************************************************/
void dirExists(char *theDir)
  {
  char cx;
  extern char FirstInit, ReInit;
  strCpy(msgBuf.mbtext, theDir);
  /* if (strLen(theDir) == 0) return ; */
  if (msgBuf.mbtext[strLen(msgBuf.mbtext)-1] == '/')
  msgBuf.mbtext[strLen(msgBuf.mbtext)-1] = '\0';
  #ifdef READY
  if (doch(msgBuf.mbtext) == EOF)
    {
    sPrintf(msgBuf.mbtext, "%s is not a directory!", theDir);
    illegal(msgBuf.mbtext);

    }
  #else
  if (chdir(msgBuf.mbtext) == 0)
    {
    chdir(curDir);
    return;

    }
  if (access(msgBuf.mbtext, 0) == 0)
    {
    sPrintf(msgBuf.mbtext, "%s is not a directory!", theDir);
    illegal(msgBuf.mbtext);

    }
  printf("\nThe directory '%s' doesn't exist.  Create it? ",
  msgBuf.mbtext);
  if (FirstInit)  printf("Y\n ");
  cx = simpleGetch();
  if (ReInit || FirstInit || toUpper(cx) == 'Y')
  if (mkdir(msgBuf.mbtext) != 0)  illegal("Couldn't make directory!");
  printf("Finished calling mkdir\n");
  #endif
  #ifdef READY
  else
    {
    printf("\nThe directory '%s' doesn't exist.  Create it? ",
    msgBuf.mbtext);
    if (FirstInit)
    printf("Y\n ");
    if (ReInit || FirstInit || toUpper(simpleGetch()) == 'Y')
    if (mkdir(msgBuf.mbtext) == BAD_DIR)
    illegal("Couldn't make directory!");

    }
  #endif
  #ifdef READY
  doch(curDir);
  #endif

  }
/************************************************************************/
/*      sysSpecs()  system specific configure stuff                     */
/************************************************************************/
int sysSpecs(char *line, int offset, char *status, FILE *fd)
  {
  char var[90], string[90];
  int  arg;
  *status = TRUE;
  if (sscanf(line, "%s %s ", var, string))
    {
    if (string[0] != '\"')
      {
      arg = atoi(string);

      }
    if (strCmpU(var, "#HELPAREA"  )    == SAMESTRING)
      {
      doAreaCommon(var, line, &cfg.homeArea, HELP);
      }
    else if (strCmpU(var, "#QWKWORKAREA" ) == SAMESTRING)
      {
      doAreaCommon(var, line, &cfg.QwkWorkArea,QWK1);
      }
    else if (strCmpU(var, "#BIOAREA" ) == SAMESTRING)
      {
      doAreaCommon(var, line, &cfg.bioArea,BIO_AREA);
      }
    else if (strCmpU(var, "#QWKLOCATION" ) == SAMESTRING)
      {
      readString(line, &cfg.QwkLocation.saDirname[0], FALSE);
      necessary[QWK1]++;
      }
    else if (strCmpU(var, "#QWKFILEAREA" ) == SAMESTRING)
      {
      doAreaCommon(var, line, &cfg.QwkFileArea,QWK2);
      }
    else if (strCmpU(var, "#QWKNAME" ) == SAMESTRING)
      {
      readString(line, (char *)&cfg.QwkName, FALSE);
      necessary[QWK3]++;
      }
    else if (strCmpU(var, "#QWKMAXROOM" ) == SAMESTRING)
      {
      cfg.QwkMaxRooms = arg;
      }
    else if (strCmpU(var, "#QWKMAXPACKET" ) == SAMESTRING)
      {
      cfg.QwkMaxPacket = arg;
      }
    else if (strCmpU(var, "#door"  )    == SAMESTRING)
      {
      StoreDoor(line, fd);

      }
    else if (strCmpU(var, "#LOGAREA"  )    == SAMESTRING)
      {
      doAreaCommon(var, line, &cfg.logArea, LOG);

      }
    else if (strCmpU(var, "#ROOMAREA"  )    == SAMESTRING)
      {
      doAreaCommon(var, line, &cfg.roomArea, ROOM);

      }
    else if (strCmpU(var, "#MSGAREA"  )    == SAMESTRING)
      {
      doAreaCommon(var, line, &cfg.msgArea, MSG);

      }
    else if (strCmpU(var, "#MSG2AREA" )    == SAMESTRING)
      {
      doAreaCommon(var, line, &cfg.msg2Area, MSG2);

      }
    else if (strCmpU(var, "#NETAREA"  )    == SAMESTRING)
      {
      doAreaCommon(var, line, &cfg.netArea, NET_STUFF);

      }
    else if (strCmpU(var, "#DOMAINAREA"  )    == SAMESTRING)
      {
      doAreaCommon(var, line, &cfg.domainArea, DOMAIN_STUFF);

      }
    else if (strCmpU(var, "#AUDITAREA"  )    == SAMESTRING)
      {
      doAreaCommon(var, line, &cfg.auditArea, CALL);
      cfg.Audit = 1;

      }
    else if (strCmpU(var, "#HOLDAREA"  )    == SAMESTRING)
      {
      doAreaCommon(var, line, &cfg.holdArea, HOLD);
      cfg.BoolFlags.HoldOnLost = TRUE;

      }
    else if (strCmpU(var, "#FLOORAREA" )    == SAMESTRING)
      {
      doAreaCommon(var, line, &cfg.floorArea, FLOORA);

      }
    else if (strCmpU(var, "#NET_RECEPT_AREA")   == SAMESTRING)
      {
      anyArea(var, line, cfg.receptArea.naDirname);

      }
    else if (strCmpU(var, "#EDITOR") == SAMESTRING)
      {
      cfg.BoolFlags.SysopEditor = TRUE;
      readString(line, cfg.DepData.Editor, FALSE);

      }
    else if (strCmpU(var, "#EDIT-AREA") == SAMESTRING)
      {
      readString(line, cfg.DepData.EditArea, FALSE);

      }
    else if (strCmpU(var, "#REINIT") == SAMESTRING)
      {
      readString(line, cfg.DepData.HiSpeedInit, TRUE);
      if (strLen(cfg.DepData.HiSpeedInit) >=
      sizeof cfg.DepData.HiSpeedInit)
      illegal("The #REINIT parameter is too long.  29 is the max.");

      }
    else if (strCmpU(var, "#LOCK-PORT") == SAMESTRING)
      {
      if (arg < 0 || arg > 8)
      illegal("Bad value for #LOCK-PORT: 0-8 valid only.");
      cfg.DepData.LockPort = arg;

      }
    else if (strCmpU(var, "#CLOCK"   ) == SAMESTRING)
      {
      cfg.DepData.Clock &= SER_7WIRE;  /* save serial  7 wire flag */
      if (     strCmpU(string, "none"  ) == SAMESTRING)
      cfg.DepData.Clock |= NO_CLOCK;
      else if (strCmpU(string, "inuse" ) == SAMESTRING)
      cfg.DepData.Clock |= BUSY_CLOCK;
      else if (strCmpU(string, "always") == SAMESTRING)
      cfg.DepData.Clock |= ALWAYS_CLOCK;
      else illegal("Didn't understand #CLOCK value.");

      }
    else if (strCmpU(var, "#modemSetup"   ) == SAMESTRING)
      {
      readString(line, cfg.DepData.ModemSetup, TRUE);
      strCat(cfg.DepData.ModemSetup, "\r");

      }
    else if (strCmpU(var, "#SCREENWIDTH") == SAMESTRING)
      {
      if (arg < 640 || arg > 740)
      illegal("The #SCREENWIDTH parameter is not between 640 and 740.");
      cfg.DepData.ScreenWidth = arg;

      }
    else if (strCmpU(var, "#SCREENHEIGHT") == SAMESTRING)
      {
      if (arg < 100 || (arg > 300 && arg < 400) || arg > 500)
      illegal("The #SCREENHEIGHT parameter is not valid.");
      cfg.DepData.ScreenHeight = arg;

      }
    else if (strCmpU(var, "#SCREENCOLOR0") == SAMESTRING)
      {
      cfg.DepData.Color0 = arg;

      }
    else if (strCmpU(var, "#SCREENCOLOR1") == SAMESTRING)
      {
      cfg.DepData.Color1 = arg;

      }
    else if (strCmpU(var, "#DISABLEECHO") == SAMESTRING)
      {
      cfg.DepData.StartUpEcho = FALSE;

      }
    else if (strCmpU(var, "#DIRECTTOCHIP") == SAMESTRING)
      {
      cfg.DepData.DirectToChip = TRUE;

      }
    else if (strCmpU(var, "#WBENCHWINDOW") == SAMESTRING)
      {
      cfg.DepData.ScreenDepth = TRUE;

      }
    else if (strCmpU(var, "#SERIALDEVICE") == SAMESTRING)
      {
      readString(line, cfg.DepData.DevName, FALSE);

      }
    else if (strCmpU(var, "#UNITNUMBER") == SAMESTRING)
      {
      cfg.DepData.UnitNumber = arg;

      }
    else if (strCmpU(var, "#SERIAL_7WIRE") == SAMESTRING)
      {
      cfg.DepData.Clock |= SER_7WIRE;

      }
    else *status = FALSE;

    }
  return offset;

  }
/************************************************************************/
/*      doAreaCommon() handles common stuff                             */
/************************************************************************/
void doAreaCommon(char *var, char *line, SYS_AREA *area, int which)
  {
  char *c;
  anyArea(var, line, area->saDirname);
  /**
    make sure the directory name has a trailing slash or colon
  **/
  c = area->saDirname;
  c += strlen(c) - 1;  /* get pointer to last character */
  if( *c != ':' && *c != '/' )
    {
    c[1] = '/';   /* add a trailing slash */
    c[2] = '\0';
    };
  necessary[which]++;

  }
/************************************************************************/
/*      anyArea() handles common stuff                                  */
/************************************************************************/
void anyArea(char *var, char *line, char *target)
  {
  readString(line, target, FALSE);

  }
/************************************************************************/
/*      initSysSpec() initialization for system dependencies            */
/************************************************************************/
void initSysSpec()
  {
  getcwd(curDir, sizeof curDir);
  cfg.DepData.Editor[0]      =      0;
  cfg.DepData.EditArea[0]    =      0;
  cfg.DepData.HiSpeedInit[0] =      0;
  cfg.DepData.LockPort       =     -1;
  cfg.DepData.Clock          = ALWAYS_CLOCK;
  cfg.DepData.ScreenWidth    =    640;
  cfg.DepData.ScreenHeight   =    200;
  cfg.DepData.ScreenDepth    =      0; /*WBENCHWINDOW*/
  cfg.DepData.Color0         = 0x0018;
  cfg.DepData.Color1         = 0x0FFF;
  cfg.DepData.StartUpEcho    =      1;
  cfg.DepData.DirectToChip   =      0;
  cfg.DepData.UnitNumber     =      0;
  cfg.QwkMaxRooms            =     20;
  cfg.QwkMaxPacket           =    200;
  strcpy(cfg.DepData.DevName,"serial.device");

  }
#define WhiteSpace(x)   ((x) == ' ' || (x) == '\t' || (x) == '\n')
/*
* #door <entrycode> <program name> <location> <who> <where> <how long>
* <description>
* <parameter list>
*/
/************************************************************************/
/*      StoreDoor() manages the reading and interpretation of a door    */
/************************************************************************/
void StoreDoor(char *line, FILE *fd)
  {
  char     *s;
  DoorData *DoorInfo;
  char     l2[100];
  char     temp[15];
  DoorInfo = (DoorData *) GetDynamic(sizeof *DoorInfo);
  zero_struct(*DoorInfo);
  line += 5;  /* now we point at the space before the entry code. */
  /* get entrycode */
  if ((line = GetDoorData(line, DoorInfo->entrycode, 6)) == NULL)
    illegal("Problem with parsing the entry code for a door!");
  /* get program name */
  if ((line = GetDoorData(line, DoorInfo->program, 14)) == NULL)
    illegal("Problem with parsing the program for a door!");
  /* get door preferred location */
  if ((line = GetDoorData(line, DoorInfo->location, 50)) == NULL)
    illegal("Problem with parsing the location of a door! (d1)");
  /* get door privilege */
  if ((line = GetDoorData(line, temp, 15)) == NULL)
    illegal("Problem with parsing the location of a door (d2)!");
  if (strCmpU(temp, "anyone") == SAMESTRING)
    DoorInfo->flags |= DOOR_ANYONE;
  else if (strCmpU(temp, "aide") == SAMESTRING)
    DoorInfo->flags |= DOOR_AIDE;
  else if (strCmpU(temp, "sysop") == SAMESTRING)
    DoorInfo->flags |= DOOR_SYSOP;
  else if (strCmpU(temp, "autodoor") == SAMESTRING)
    DoorInfo->flags |= DOOR_AUTO;
  else if (strCmpU(temp, "newusers") == SAMESTRING)
    DoorInfo->flags |= DOOR_NEWUSER;
  else illegal("Could not identify who is allowed to use this door!");
  /* get MODEM/CONSOLE/EITHER flag */
  if ((line = GetDoorData(line, temp, 15)) == NULL)
    illegal("Problem with parsing the 'where' field!");
  if (strCmpU(temp, "anywhere") == SAMESTRING)
    {
    DoorInfo->flags |= DOOR_CON;
    DoorInfo->flags |= DOOR_MODEM;

    }
  else if (strCmpU(temp, "console") == SAMESTRING)
  DoorInfo->flags |= DOOR_CON;
  else if (strCmpU(temp, "modem") == SAMESTRING)
  DoorInfo->flags |= DOOR_MODEM;
  else illegal("Could not identify the 'where' value!");
  /* get time limit of door */
  if ((line = GetDoorData(line, temp, 15)) == NULL)
  illegal("Problem with parsing the 'time' field!");
  if (strCmpU(temp, "unlimited") == SAMESTRING)
  DoorInfo->TimeLimit = -1;
  else
  DoorInfo->TimeLimit = atoi(temp);
  /* Now get optional room to tie this door to */
  if ((s = strrchr(line, '\n')) != NULL) *s = 0;
  strCpy(DoorInfo->RoomName, line);
  /* get description of door */
  if (fgets(l2, 100, fd) == NULL) illegal("Unexpected EOF in door handling!");
  if ((s = strrchr(l2, '\n')) != NULL) *s = 0;
  if (strLen(l2) >= 80)  illegal("Description too long!");
  strCpy(DoorInfo->description, l2);
  /* get parameters of door */
  if (fgets(l2, 100, fd) == NULL) illegal("Unexpected EOF in door handling!");
  if ((s = strrchr(l2, '\n')) != NULL) *s = 0;
  if (strLen(l2) >= 99) illegal("Parameters too long!");
  strCpy(DoorInfo->parameters, l2);
  AddData(&DoorList, DoorInfo, NULL, FALSE);

  }
/************************************************************************/
/*      GetDoorData() reads in some piece of the door data.             */
/************************************************************************/
char *GetDoorData(char *line, char *field, int size)
  {
  char *s;
  if ((line = NextNonWhite(line)) == NULL)
  return NULL;
  /* Now skip over field so we may zero out & copy */
  if ((s = NextWhite(line)) == NULL)
  return NULL;
  *s = 0;             /* zero whitespace */
  if (strLen(line) >= size)
    {
    printf("Value '%s' too long for door's data field.\n", line);
    return NULL;

    }
  strCpy(field, line);
  return s + 1;

  }
/************************************************************************/
/*      NextWhite() Finds next piece of white space on the line.        */
/************************************************************************/
char *NextWhite(char *line)
  {
  char *s;
  for (s = line; !WhiteSpace(*s) && *s; s++)
  ;
  if (!(*s)) return NULL;
  return s;

  }
/************************************************************************/
/*      NextNonWhite() Finds next piece of non-whitespace.              */
/************************************************************************/
char *NextNonWhite(char *line)
  {
  char *s;
  for (s = line; WhiteSpace(*s) && *s; s++)
  ;
  if (!(*s)) return NULL;
  return s;

  }
FILE *upfd;
/************************************************************************/
/*      WriteDoors() writes out the list of doors.                      */
/************************************************************************/
void WriteDoors()
  {
  SYS_FILE DoorFile;
  makeSysName(DoorFile, DOOR_DATA, &cfg.roomArea);
  if ((doorfl = fopen(DoorFile, WRITE_ANY)) == NULL)
    {
    sPrintf(msgBuf.mbtext, "Could not create %s.", DoorFile);
    illegal(msgBuf.mbtext);

    }
  RunList(&DoorList, WrtDoor);

  }
/************************************************************************/
/*      WrtDoor() writes out a single door.                             */
/************************************************************************/
void WrtDoor(DoorData *d)
  {
  if( fwrite(d, sizeof *d, 1, doorfl) != 1 )
    {
    printf("Write error: Door data file\n");
    };
  }
/************************************************************************/
/*      SysDepIntegrity() makes necessary checks for integrity          */
/************************************************************************/
static char Check_Flag(int flag, int orflag, char *message);
static char Check_Flag(int flag, int orflag, char *message)
  {
  if( !flag )
    {
    printf(" --%s parameters were not completely defined\n",message);
    orflag = TRUE;
    };
  return ( char )orflag;
  }

char SysDepIntegrity(int *offset)
  {
  char bad;
  bad = Check_Flag(necessary[HELP],    FALSE, "Help");
  bad = Check_Flag(necessary[MSG],       bad, "Message");
  bad = Check_Flag(necessary[FLOORA],    bad, "Floor");
  bad = Check_Flag(necessary[LOG],       bad, "User Log");
  bad = Check_Flag(necessary[ROOM],      bad, "Room");
  bad = Check_Flag(necessary[ BIO_AREA], bad, "Biography Area");

  if( necessary[QWK1] || necessary[QWK2] || necessary[QWK3] )
    {
    bad = Check_Flag(necessary[QWK1],   bad, "QWK Location");
    bad = Check_Flag(necessary[QWK2],   bad, "QWK packet");
    bad = Check_Flag(necessary[QWK3],   bad, "QWK file");
    if( !bad )cfg.BoolFlags.QwkMail = 1;
    }
  if( cfg.Audit !=0 )bad = Check_Flag(necessary[CALL], bad, "Audit");
  if( cfg.BoolFlags.mirror )
    bad = Check_Flag(necessary[MSG2],   bad, "Mirror Message");
  if( cfg.BoolFlags.netParticipant )
    {
    bad = Check_Flag(necessary[NET_STUFF],    bad, "Networking");
    bad = Check_Flag(necessary[DOMAIN_STUFF], bad, "Domain");
    };
  if( bad )  illegal("See the above error messages.");
  dirExists(cfg.homeArea.saDirname);
  dirExists(cfg.logArea.saDirname);
  dirExists(cfg.roomArea.saDirname);
  dirExists(cfg.msgArea.saDirname);
  dirExists(cfg.floorArea.saDirname);
  dirExists(cfg.bioArea.saDirname);
  if (cfg.BoolFlags.mirror)
    dirExists(cfg.msg2Area.saDirname);
  if (cfg.BoolFlags.netParticipant)
    {
    dirExists(cfg.netArea.saDirname);
    dirExists(cfg.receptArea.naDirname);
    dirExists(cfg.domainArea.saDirname);
    };
  if (cfg.Audit)
    dirExists(cfg.auditArea.saDirname);
  if (necessary[HOLD])
    dirExists(cfg.holdArea.saDirname);
  if( necessary[QWK1] || necessary[QWK3] )
    {
    dirExists(cfg.QwkWorkArea.saDirname);
    dirExists(cfg.QwkFileArea.saDirname);
    };
  WriteDoors();
  return TRUE;

  }
int Dcount;
/************************************************************************/
/*      FindDoorSlot() looks for the specified door for autodoor stuff  */
/************************************************************************/
int FindDoorSlot(char *name)
  {
  int *c;
  DoorData temp;
  Dcount = 0;
  strCpy(temp.entrycode, name);
  if ((c = (int *) SearchList(&DoorList, &temp)) != NULL)
  return *c;

  }
/************************************************************************/
/*      DoorName() check to see if this door is here.                                   */
/************************************************************************/
void *DoorName(DoorData *d, DoorData *s)
  {
  if (strCmpU(d->entrycode, s->entrycode) == SAMESTRING)
  return &Dcount;
  Dcount++;
  return NULL;

  }
/*************************************************************************/
/*   simpleGetch() for AmigaDos version           */
/*************************************************************************/
int simpleGetch()
  {
  char inval;
  do
    {
    scanf("%c",&inval);
    inval = toupper(inval);

    }
  while( (inval != 'Y') && (inval != 'N') );
  return(inval);

  }
/************************************************************************/
/*              totalBytes() how many bytes in this here file?          */
/************************************************************************/
void totalBytes(long *size, FILE *fd)
  {
  *size = lseek(fileno(fd),-1l,2)+1;

  }
char bigbuffer[7500];
/************************************************************************/
/*              mPrintf()  can you say kludge?                          */
/************************************************************************/
int mPrintf(char *format, ...)
  {
  va_list argptr;
  va_start(argptr, format);
  vsprintf(bigbuffer, format, argptr);
  va_end(argptr);
  printf("%s",bigbuffer);
  return 0;
  }
/************************************************************************/
/*      sPrintf() write from format+args to supplied string             */
/************************************************************************/
int sPrintf(char *garp, const char *format, ...)
  {
  va_list argptr;
  va_start(argptr, format);
  vsprintf(garp, format, argptr);
  va_end(argptr);
  return 0;
  }
char sysArgs(char *str)
  {
  return FALSE;

  }
void *EatDirEntry(char *line);
char DirDeleted = FALSE;
SListBase DirRooms =
  {
  NULL, NULL, NULL, NULL, EatDirEntry

  };
FILE *Dirs;
/************************************************************************/
/*      FinalSystemCheck() final system dependent checks                */
/************************************************************************/
int FinalSystemCheck(char OnlyParams)
  {
  SYS_FILE fn;
  void WriteDir();
  /* Check for excess entries in ctdldir.sys */
  makeSysName(fn, "ctdldir.sys", &cfg.roomArea);
  MakeList(&DirRooms, fn, NULL);
  if (DirDeleted)
    {
    if ((Dirs = fopen(fn, WRITE_TEXT)) != NULL)
      {
      RunList(&DirRooms, WriteDir);
      fclose(Dirs);

      }

    }
  return TRUE;

  }
void WriteDir(char *l)
  {
  fprintf(Dirs, "%s\n", l);

  }
/************************************************************************/
/*      EatDirEntry() validates an entry from ctdldir.sys               */
/************************************************************************/
void *EatDirEntry(char *line)
  {
  char *space;
  int  room;
  /* discard garbled entries */
  if ((space = strchr(line, ' ')) == NULL)
    {
    DirDeleted = TRUE;
    printf("Garbled entry -%s- deleted from ctdldir.sys.\n", line);
    return NULL;

    }
  room = atoi(line);
  if (room >= MAXROOMS || room < 0)
    {
    DirDeleted = TRUE;
    printf("Out of range entry -%s- deleted from ctdldir.sys.\n", line);
    return NULL;

    }
  if (roomTab[room].rtflags.INUSE && roomTab[room].rtflags.ISDIR)
  return strdup(line);
  DirDeleted = TRUE;
  printf("Excess entry -%s- deleted from ctdldir.sys.\n", line);
  return NULL;

  }
/*  Stub routine for debug purposes */
void splitF(FILE *fd, char *x,...)  { }
void Do_Stack_Check(void);
void Do_Stack_Check(void) { }
