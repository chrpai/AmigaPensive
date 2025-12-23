/************************************************************************/
/*                           SysDoor.c                                  */
/*                                                                      */
/*      Handles user-interface for doors within Citadel.  Note this is  */
/*      a system dependent file, not necessarily portable.              */
/************************************************************************/
#define SYSTEM_DEPENDENT
/* #define NO_DOORS */
#include "ctdl.h"
#ifndef NO_DOORS
#include "c68door.h"
/************************************************************************/
/*                              History                                 */
/*                                                                      */
/* 92Sep18  AFP re-wrote validity checks                                */
/* 89Nov24  AP  Extensively modified for Amiga                          */
/* 89Jan30  HAW AutoDoors.                                              */
/* 89Jan21  HAW Controlled access via #events                           */
/* 88Dec16  HAW Created.                                                */
/************************************************************************/

char Legal_Door(DoorData *, int, int, int); /* door validity checker */
void Execute_Door(DoorData *,int);             /* execute current door  */
char MakeCmdLine(char *, char *, char *, int);
char ShowDoors(FILE *fd);
int BaudCode(int);

/************************************************************************/
/*      Local variables                                                 */
/************************************************************************/
static SListBase DoorTime =
  {
  NULL, ChkTwoNumbers, NULL, free, EatTwoNumbers

  };
static char      *BpsStr;
long      DoorsUsed = 0l;
extern int       *lPtrTab;
extern int       UngotoStack[];
extern CONFIG    cfg;
extern MessageBuffer   msgBuf;
extern aRoom     roomBuf;
extern logBuffer logBuf;
extern long      *DL_Total;
extern long      byteRate;
extern char      onConsole;
extern int       thisLog;
extern int       thisRoom;
extern char      ForceNet;
extern char      loggedIn;
extern char      remoteSysop;
extern char      CallSysop;
extern char      BpsSet;
extern int       SystemPort;
extern SListBase DL_List;
char line[120];               /* static line buffer */
/************************************************************************/
/*      doDoor() chief administrator for doors                          */
/************************************************************************/
char doDoor(char moreYet)
  {
  char        DoorId[6], match = FALSE;
  int         Count = -1;
  FILE        *fd;
  SYS_FILE    name;
  DoorData    DoorInfo;
  extern char *READ_ANY, *WRITE_ANY, outFlag;
  extern int  exitValue;
  extern char ExitToMsdos, *WRITE_ANY;
  extern FILE *upfd;
  extern long Door_Limit;
  /* First handle simple privilege check */
  if (!DoorPriv)
    {
    Output_Citadel_Message("NODORP", NULL, NULL, NULL);
    return GOOD_SELECT;

    }
  if (Door_Limit_On() && !HalfSysop() && DoorsUsed / 60 >= Door_Limit)
    {
    Output_Citadel_Message("NODORT", NULL, NULL, NULL);
    return GOOD_SELECT;

    }
  /* see if there are any doors present. */
  makeSysName(name, DOOR_DATA, &cfg.roomArea);
  if ((fd = safeopen(name, READ_ANY)) == NULL)
    {
    Output_Citadel_Message("NODORF", NULL, NULL, NULL);
    return GOOD_SELECT;

    }
  /*
  * If moreYet, then wait for name.  If not, then print out current
  * doors and prompt for name.
  */
  if (!moreYet)
  if (!ShowDoors(fd))
    {
    Output_Citadel_Message("NODORF", NULL, NULL, NULL);
    fclose(fd);
    return GOOD_SELECT;

    }
  if (getNormStr((moreYet)?"":"DOORID", DoorId, 6,
  ((moreYet) ? BS_VALID : 0) | QUEST_SPECIAL) == BACKED_OUT) return BACKED_OUT;
  if (DoorId[0] == '?')
    {
    ShowDoors(fd);
    fclose(fd);
    return GOOD_SELECT;

    }
  if(strLen(DoorId) != 0)
    {
    while (!match && fread((char *)&DoorInfo, sizeof DoorInfo, 1, fd) != 0)
      {
      if(match = (strCmpU(DoorId, DoorInfo.entrycode) == SAMESTRING))
        {
        match =  Legal_Door(&DoorInfo,FALSE,FALSE,FALSE);

        };
      Count++;

      }
    if( !match )
      {
      Output_Citadel_Message("NODRFD", (long)DoorId, NULL, NULL);

      }
    else
      {
      if( NoTimeForDoor(Count, &DoorInfo) )
        {
        Output_Citadel_Message("NODORT", NULL, NULL, NULL);

        }
      else
        {
        Execute_Door(&DoorInfo,Count);

        };

      };

    };
  fclose(fd);
  return GOOD_SELECT;

  }
/************************************************************************/
/*      RunAutoDoor() does actual work of running a door                */
/************************************************************************/
char RunAutoDoor(int i, char ask)
  {
  FILE *fd;
  SYS_FILE name;
  DoorData   drBuf;
  if (i == -1) return FALSE;
  makeSysName(name, DOOR_DATA, &cfg.roomArea);
  if ((fd = safeopen(name, READ_ANY)) == NULL)
    {
    splitF(NULL,"Internal error:no %s door data file!\n ",name);
    return FALSE;

    };
  if (fseek(fd, sizeof drBuf * i, 0) != 0)
    {
    splitF(NULL,"Couldn't seek to %d in the doors file!\n", i);
    fclose(fd);
    return FALSE;

    };
  if (fread((char *)&drBuf, sizeof(drBuf), 1, fd) <= 0)
    {
    splitF(NULL,"Couldn't read from the doors file!\n");
    splitF(NULL," Filename:%s Position:%d size: %d byte offset: %ld\n",
    name,i, sizeof(drBuf),(long)i*sizeof(drBuf));
    fclose(fd);
    return FALSE;

    };
  fclose(fd);
  if( !Legal_Door(&drBuf,ask,TRUE,FALSE) )return TRUE;
  /*
  ** got a legal door now execute it
  */
  Execute_Door(&drBuf,i);
  return TRUE;
  }

int UnQueueSerRead(void);
int QueueSerRead(int);
int Jsystem(char *);

void Execute_Door(dr,i)
DoorData   *dr;
int i;
  {
  SYS_FILE    name;
  long *AlreadyUsed, Before, After;
  TwoNumbers  *temp;
  int  byr, bday, bhour, bmin;
  int  ayr, aday, ahour, amin;
  char *bmon, *amon;
  sPrintf(line, "%s ", dr->program);
  MakeCmdLine(lbyte(line), dr->parameters, "", sizeof line - strlen(line));
  /*
  ** if auditing enabled, collect statistics
  */
  if( cfg.Audit )
    {
    getCdate(&byr, &bmon, &bday, &bhour, &bmin);

    };
  chdir(dr->location);  /* set the directory */
  UnQueueSerRead();       /* clear the serial port */
  Before = CurAbsolute();
  SpecialMessage("Status:Door Execution");
  Jsystem(line);          /* execute the command line */
  After = CurAbsolute();
  QueueSerRead(6);        /* reset the serial port with a read */
  homeSpace();            /* reset to the home directory */
  if (cfg.Audit)
    {
    getCdate(&ayr, &amon, &aday, &ahour, &amin);
    sPrintf(line,
    "    %20s used %6s %2d%s%02d %d:%02d - %d:%02d (%ld:%02ld)",
    logBuf.lbname, dr->entrycode, byr, bmon, bday,
    bhour, bmin, ahour, amin, ((After - Before) / 60l),
    ((After - Before) % 60l));
    makeAuditName(name, "dooruse.sys");
    CallMsg(name, line);

    };
  temp = (TwoNumbers *) GetDynamic(sizeof *temp);
  temp->first = i;
  AlreadyUsed = (long *) SearchList(&DoorTime, temp);
  if (AlreadyUsed != NULL)
  temp->second = (*AlreadyUsed) + (CurAbsolute() - Before);
  else temp->second = CurAbsolute() - Before;
  AddData(&DoorTime, temp, NULL, TRUE);

  }
#ifdef REFERENCE
char  entrycode[6];         /* What user uses to specify a door     */
char  program[14];          /* name of program                      */
char  location[50];         /* door location                        */
char  description[80];      /* description of door                  */
char  flags;                /* sysop, aides, or everyone            */
char  parameters[100];      /* list of parameters.                  */
int   TimeLimit;            /* time limit for this door             */
label RoomName;             /* room name                            */
#endif
/************************************************************************/
/*      ShowDoors() Show the doors about                                */
/************************************************************************/
char ShowDoors(FILE *fd)
  {
  DoorData DoorInfo;
  int      DoorCount = 0;
  while (fread((char *)&DoorInfo, sizeof DoorInfo, 1, fd) != 0 && outFlag == OUTOK)
    {
    if( Legal_Door(&DoorInfo,FALSE,FALSE,FALSE) )
      {
      DoorCount++;
      doCR();
      mPrintf("%-10s: %s", DoorInfo.entrycode, DoorInfo.description);

      };

    }
  doCR();
  fseek(fd, 0l, 0);
  return (char)(DoorCount != 0);

  }
/************************************************************************/
/*      BackFromDoor() checks to see if we are returning from a door    */
/************************************************************************/
char BackFromDoor()
  {
  return FALSE;

  }
/************************************************************************/
/*  NoTimeForDoor() see if there is still time to run the door. */
/************************************************************************/
char NoTimeForDoor(int which, DoorData *DoorInfo)
  {
  TwoNumbers search;
  long        *found;
  if (DoorInfo->TimeLimit == -1) return FALSE;
  search.first = which;
  if ((found = (long *) SearchList(&DoorTime, &search)) != NULL)
  return (char)((*found) / 60l >= (long) DoorInfo->TimeLimit);
  return (char)FALSE;

  }
/************************************************************************/
/*  ClearDoorTimers() clear the door timers list.     */
/************************************************************************/
void ClearDoorTimers()
  {
  KillList(&DoorTime);

  }
/************************************************************************/
/*  Cumulate() Find out how much time has been used by current user */
/************************************************************************/
void Cumulate(TwoNumbers *temp)
  {
  DoorsUsed += temp->second;          /* cumulate it ... */

  }
/************************************************************************/
/*  DoorHelpListing() list the doors for the help system    */
/************************************************************************/
void DoorHelpListing(char *target)
  {
  DoorData DoorInfo;
  SYS_FILE name;
  int count = 0;
  FILE     *fd;
  /* see if there are any doors present. */
  makeSysName(name, DOOR_DATA, &cfg.roomArea);
  if ((fd = safeopen(name, READ_ANY)) == NULL)
    {
    sPrintf(target, "    There are no doors available.\n");
    return;

    }
  target[0] = 0;
  while (fread((char *)&DoorInfo, sizeof DoorInfo, 1, fd) != 0 && outFlag == OUTOK)
    {
    if( Legal_Door(&DoorInfo,FALSE,FALSE,FALSE) )
      {
      count++;
      sPrintf(lbyte(target), "   %-10s: %s\n", DoorInfo.entrycode, DoorInfo.description);

      }

    }
  fclose(fd);
  if (count == 0)sPrintf(target, "    There are no doors available.\n");

  }
/************************************************************************/
/*  ReadBps() read the bps off the command line.      */
/************************************************************************/
long Get_CPS(long, char *);
void ReadBps(char *str)
  {
  short i;
  extern char *rates[];   /* so we can generate the baud rate string. */
  BpsStr = str + 4;
  if (strCmpU(BpsStr, "LOCAL") == SAMESTRING ||
  strCmpU(BpsStr, "0") == SAMESTRING) byteRate = 0;
  else
    {
    byteRate = Get_CPS(atoi(BpsStr), "ReadBps");
    if( (i = BaudCode(byteRate)) == -1 )
      {
      if( ( i = BaudCode(byteRate/10)) == -1 )
        {
        printf("Unrecognized Baud/BPS rate on command line.\n");
        return;

        };

      };
    logMessage(BAUD, rates[i], FALSE);

    };

  }
/************************************************************************/
/*  SetUpPort() setup the port appropriately.     */
/************************************************************************/
int SetUpPort(int bps)
  {
  int code;
  byteRate = Get_CPS(bps,"SetUpPort:A");
  code = BaudCode(byteRate);
  if (code == -1)
    {
    printf("door error, unrecognizable bps.\n");
    code = 0;

    }
  CitadelBaudRate(code, "SetUpPort:B");
  return code;

  }
/************************************************************************/
/*  BaudCode() find out what our baud code is     */
/************************************************************************/
int BaudCode(int bps)
  {
  switch (bps)
    {
    case    0:   return (int)cfg.sysBaud;
    case   30:   return 0;
    case  120:   return 1;
    case  240:   return 2;
    case  480:   return 3;
    case  960:   return 4;
    case 1440:   return 5;
    case 1920:   return 6;
    case 3840:   return 7;
    case 5760:   return 8;
    default:
      if( bps > 1440 && bps < 3840 )return 7;
      if( bps > 3840 ) return 8;
      return -1;
    }
  }

/************************************************************************/
/*  NewUserDoor() called as new user identifies himself    */
/************************************************************************/
char NewUserDoor()
  {
  FILE        *fd;
  SYS_FILE    name;
  DoorData    DoorInfo;
  int not_found;
  int more;
  extern char *READ_ANY, *WRITE_ANY, outFlag;
  /* get door data */
  makeSysName(name, DOOR_DATA, &cfg.roomArea);
  if ((fd = safeopen(name, READ_ANY)) == NULL)
    {
    Output_Citadel_Message("NODORF", NULL, NULL, NULL);
    }
  else
    {
    more = not_found = TRUE;
    while( more && not_found)
      {
      more = fread((char *)&DoorInfo, sizeof DoorInfo, 1, fd) != 0;
      if( more )
        {
        not_found =  ! Legal_Door(&DoorInfo,FALSE,FALSE,TRUE);

        };

      };
    fclose(fd);
    if( ! not_found )
      {
      sPrintf(line, "%s ", DoorInfo.program);
      MakeCmdLine(lbyte(line), DoorInfo.parameters, ""
      , sizeof line - strlen(line));
      UnQueueSerRead();
      chdir(DoorInfo.location);  /* set the directory */
      SpecialMessage("Status:NewUser Door Execution");
      Jsystem(line);
      QueueSerRead(6);
      homeSpace();

      };

    };
  return((char)TRUE);

  }
char Flag_Test(int,int);
char Flag_Test(flag,mask)
int flag,mask;
  {
  int result;
  result = flag & mask;
  if (cfg.BoolFlags.debug)
    {
    splitF(NULL,"flag:%x, mask: %x result: %x\n",flag,mask,result );

    };
  if( result == 0 )return TRUE;
  return FALSE;
  }

char Legal_Door(dr,verbose,autolegal,newuser)
DoorData *dr;
int verbose;    /* flag - give User a text reason */
int autolegal;  /* are we allowing an autodoor? */
int newuser;    /* are we allowing a new user door here?*/
  {
  char status;
  int  reason;
  status = TRUE;

  reason = 0;
  /* Checks to see if this is a legal door in this case  */
  if (!DoorPriv && !newuser)
    {
    if (cfg.BoolFlags.debug)
      {
      splitF(NULL,"No door priviledges\n");
      };
    reason |= 128;
    status = FALSE;
    };
  if( dr->flags == DOOR_ANYONE ) return status;  /* anyone can run */
  if( onConsole )
    {
    if(  Flag_Test(dr->flags, DOOR_CON) )
      {
      if (cfg.BoolFlags.debug)
        {
        splitF(NULL,"Not able to run door from console.\n");
        };
      reason |= 1;
      status = FALSE;
      };
    }
  else
    {
    if( Flag_Test(dr->flags, DOOR_MODEM) )
      {
      if (cfg.BoolFlags.debug)
        {
        splitF(NULL,"Not able to run door from modem.\n");
        };
      reason |= 2;
      status = FALSE;
      };
    };
  if( !aide )
    {
    if( ! Flag_Test(dr->flags, DOOR_AIDE) )
      {
      if(cfg.BoolFlags.debug || verbose )
        mPrintf("Sorry you must have AIDE privileges to run that door.\n");
      reason |= 4;
      status = FALSE;

      };

    };
  if( !SomeSysop() )
    {
    if( ! Flag_Test(dr->flags, DOOR_SYSOP) )
      {
      if( cfg.BoolFlags.debug || verbose )
        mPrintf("Sorry only the Sysop may run that door.\n");
      reason |= 8;
      status = FALSE;

      };

    };
  if( autolegal )
    {
    if( Flag_Test(dr->flags,DOOR_AUTO))
      {
      if(cfg.BoolFlags.debug || verbose )
        mPrintf("Error: That door is not an AutoDoor.\n");
      reason |= 16;
      status = FALSE;
      };
    }
  else
    {
    if( !Flag_Test(dr->flags,DOOR_AUTO) )
      {
      if( cfg.BoolFlags.debug ||  verbose )
        mPrintf("Sorry that is an AutoDoor.\n");
      reason |= 16;
      status = FALSE;

      };
    };
  if(  ( !newuser && !Flag_Test(dr->flags, DOOR_NEWUSER) )
  ||   (  newuser &&  Flag_Test(dr->flags, DOOR_NEWUSER) ) )
    {
    if( cfg.BoolFlags.debug || verbose )
      mPrintf("Sorry that is a new user door.\n");
    reason |= 32;
    status = FALSE;

    };
  if( dr->RoomName[0] != '\0'
  &&  strCmpU(dr->RoomName,roomBuf.rbname) != SAMESTRING )
    {
    if( verbose )mPrintf("Sorry you may only run that door from the %s room.\n",dr->RoomName);
    if (cfg.BoolFlags.debug)
      {
      splitF(NULL," Door Name: %s =?= Room Name:%s\n",dr->RoomName,roomBuf.rbname);

      };
    reason |= 64;
    status = FALSE;

    };
  if (cfg.BoolFlags.debug)
    {
    splitF(NULL,"%s  Legal Door:%s Reason %x\n",dr->program, ( reason ? " " : " NOT"),reason );

    };
  return status;

  }
