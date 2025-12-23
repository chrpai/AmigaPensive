/*
*                               libtabl.c
*
* Code to handle CTDLTABL.SYS
*/
/*
*                               history
*
* 87Jan20 HAW  Integrity stuff for portability.
* 86Apr24 HAW  Modified for fwrite() and fread().
* 85Nov15 HAW  Created.
*/
#include "ctdl.h"
/*
*                               Contents
*
*       readSysTab()            restores system state from ctdltabl.sys
*       common_read()           bottleneck for reading
*       writeSysTab()           saves state of system in CTDLTABL.SYS
*       GetDynamic()            allocation bottleneck
*/
CONFIG          cfg;                    /* A buncha variables   */
LogTable        *logTab;                /* RAM index of pippuls */
NetTable        *netTab;                /* RAM index of nodes   */
rTable          *roomTab;               /* RAM index of rooms   */
EVENT           *EventTab = NULL;
char            *indexTable = "ctdlTabl.sys";
struct floor    *FloorTab;
int             TopFloor;

static struct
  {
  int checkMark;                        /* rudimentary integrity */
  int cfgSize;                  /* sizeof cfg   */
  int logTSize;                 /* logtab size  */
  int endMark;                  /* another integrity check      */

  }
integrity;
extern char *R_W_ANY;
static void *FindServes(char *name, char *target);
SListBase Serves =
  {
  NULL, FindServes, NULL, free, NULL

  };
/**
  These are the "markers" for the version id of the
  current configuration.  If you change the internals
  of the ctdltbl.sys file, you should increment both
  of these to ensure that everyone is using the
  correct versions of CTDL and CONFG.
**/
#define CHKM    8       /* major release        */
#define ENDM    9
/*
* readSysTab()
*
* This function restores the state of system from CTDLTABL.SYS
* returns:              TRUE on success, else FALSE
* destroys CTDLTABL.TAB after read, to prevent erroneous re-use
* in the event of a crash.
*
* MS-DOS fun: Here's the map --
* Word 1 == sizeof cfg
* Word 2 == sizeof logTab
* Word 3 == sizeof roomTab
* Word 4 -- thru x == cfg contents
* x -- y == logTab
* y -- z == roomTab
* z -- a == netTab
* EOF
*/
char readSysTab(char kill, char showMsg)
  {
  FILE  *fd;
  extern char *READ_ANY;
  int           rover;
  long  bytes;
  SYS_FILE    name;
  char  caller;
  label       temp;
  caller = cfg.weAre;
  if ((fd = fopen(indexTable, READ_ANY)) == NULL)
    {
    if (showMsg)
      {
      perror("fopen");
      poserr("fopen");
      printf("%s not found or read protected!", indexTable);    /* Tsk, tsk! */
      };
    return(FALSE);

    }
  if (fread(&integrity, sizeof integrity, 1, fd) != 1)
    {
    if (showMsg)
      {
      perror("fread");
      poserr("fread");
      printf("Improper size of %s, unable to process.\n",indexTable);
      };
    return FALSE;

    }
  if (     integrity.checkMark != CHKM ||
  integrity.endMark != ENDM ||
  integrity.cfgSize != sizeof cfg)
    {
    if (showMsg) printf("Improper integrity information, you need the CONFG and CTDL\n");
    return(FALSE);

    }
  if (!common_read(&cfg, (sizeof cfg), 1, fd, showMsg))  return FALSE;
  /* Allocations for dynamic parameters */
  logTab = (LogTable *) GetDynamic(integrity.logTSize);
  roomTab = (rTable *) GetDynamic(MAXROOMS * (sizeof (*roomTab)));
  if (cfg.netSize)
  netTab = (NetTable *) GetDynamic(sizeof (*netTab) * cfg.netSize);
  else
  netTab = NULL;
  if (cfg.EvNumber) EventTab  = (EVENT *)GetDynamic(sizeof (*EventTab) * cfg.EvNumber);
  /* "- 1" is kludge */
  if (integrity.logTSize != sizeof (*logTab) * cfg.MAXLOGTAB)
    {
    if (showMsg) printf("Improper size of Log Table in integrity check\n");
    return(FALSE);

    }
  if (!common_read(logTab, integrity.logTSize, 1, fd, showMsg))
  return FALSE;
  if (!common_read(roomTab, (sizeof (*roomTab)) * MAXROOMS, 1, fd, showMsg))
  return FALSE;
  if (cfg.netSize)
    {
    for (rover = 0; rover < cfg.netSize; rover++)
      {
      if (!common_read(&netTab[rover], NT_SIZE, 1, fd, showMsg)) return FALSE;
      netTab[rover].netTRooms = (SharedRoom *) GetDynamic(SR_BULK);
      if (!common_read(netTab[rover].netTRooms, SR_BULK, 1, fd, showMsg)) return FALSE;
      };

    }
  if (cfg.EvNumber)
    {
    if (!common_read(EventTab, (sizeof(*EventTab) * cfg.EvNumber), 1, fd, showMsg))
    return FALSE;

    };
  for (rover = 0; rover < cfg.DomainHandlers; rover++)
    {
    if (!common_read(temp, NAMESIZE, 1, fd, showMsg))
      {
      return FALSE;

      }
    AddData(&Serves, strdup(temp), NULL, FALSE);

    }
  fclose(fd);
  makeSysName(name, "ctdlflr.sys", &cfg.floorArea);
  if ((fd = fopen(name, R_W_ANY)) == NULL)
    {
    if (caller != CONFIGUR)
      {
      if (showMsg)
        {
        perror("fopen");
        poserr("fopen");
        printf("No floor table file found!\n");
        };
      return FALSE;

      }

    }
  else
    {
    totalBytes(&bytes, fd);
    FloorTab = (struct floor *) GetDynamic((int) bytes);
    if (fread(FloorTab, (int) bytes, 1, fd) != 1)
      {
      if (showMsg)
        {
        perror("fread");
        poserr("fread");
        printf("error reading floor tab file\n");
        };

      fclose(fd);
      if (caller != CONFIGUR) return FALSE;

      }
    else
      {
      fclose(fd);
      TopFloor = (int) bytes/sizeof(*FloorTab);

      }

    }
  if (kill) unlink(indexTable);
  crypte(cfg.sysPassword, sizeof cfg.sysPassword, 0);
  return(TRUE);

  }
/*
* common_read()
*
* This function reads in from file the important stuff.
* returns:      TRUE on success, else FALSE
*/
int common_read(void *block, int size, int elements, FILE *fd,
int showMsg)
  {
  if (size == 0) return TRUE;
  if (fread(block, size, elements, fd) != 1)
    {
    if (showMsg)
      {
      perror("fread");
      poserr("fread");
      printf("Unable to read common data %d elements of size %d\n",elements, size);
      };
    return FALSE;

    }
  return TRUE;

  }
static FILE *fd;
/*
* writeSysTab()
*
* This saves state of system in CTDLTABL.SYS
* returns:      TRUE on success, else ERROR
* See readSysTab() to see what the CTDLTABL.SYS map looks like.
*/
static void WriteServers(char *name);
int writeSysTab()
  {
  extern char   *WRITE_ANY;
  int   rover;
  if ((fd = fopen(indexTable, WRITE_ANY)) == NULL)
    {
    perror("writeSysTab");
    poserr("writeSysTab");
    printf("writeSysTab: cannot open %s for writing\n",indexTable);
    return(ERROR);

    }
  /* Write out some key stuff so we can detect bizarreness: */
  integrity.checkMark = CHKM;
  integrity.endMark = ENDM;
  integrity.cfgSize = sizeof cfg;
  integrity.logTSize = sizeof (*logTab) * cfg.MAXLOGTAB;

  if( 1 != fwrite((char *)&integrity, (sizeof integrity), 1, fd) )
    {
    perror("writeSysTab");
    poserr("writeSysTab");
    printf("writeSysTab: problem writing integrity data\n");
    return(ERROR);
    };

  crypte(cfg.sysPassword, sizeof cfg.sysPassword, 0);
  if( 1 != fwrite((char *)&cfg, (sizeof cfg), 1, fd) )
    {
    perror("writeSysTab");
    poserr("writeSysTab");
    printf("writeSysTab: problem writing configuration data\n");
    return(ERROR);
    };

  crypte(cfg.sysPassword, sizeof cfg.sysPassword, 0);
  if( 1 != fwrite((char *)logTab, (sizeof(*logTab) * cfg.MAXLOGTAB), 1, fd))
    {
    perror("writeSysTab");
    poserr("writeSysTab");
    printf("writeSysTab: problem writing User Log table\n");
    return(ERROR);
    };

  if( 1 != fwrite((char *)roomTab, (sizeof (*roomTab)) * MAXROOMS, 1, fd))
    {
    perror("writeSysTab");
    poserr("writeSysTab");
    printf("writeSysTab: problem writing room table\n");
    return(ERROR);
    };

   for (rover = 0; rover < cfg.netSize; rover++)
    {
    if( 1 != fwrite((char *)&netTab[rover], NT_SIZE, 1, fd))
      {
      perror("writeSysTab");
      poserr("writeSysTab");
      printf("writeSysTab: problem writing Net Table[%d]\n",rover);
      return(ERROR);
      };

    if( 1 != fwrite((char *)netTab[rover].netTRooms, SR_BULK, 1, fd))
      {
      perror("writeSysTab");
      poserr("writeSysTab");
      printf("writeSysTab: problem writing Net Table/Room[%d]\n",rover);
      return(ERROR);
      };

    };
  if (cfg.EvNumber)
  if( 1 != fwrite((char *)EventTab, (sizeof(*EventTab) * cfg.EvNumber), 1, fd))
    {
    perror("writeSysTab");
    poserr("writeSysTab");
    printf("writeSysTab: problem writing Event Table\n");
    return(ERROR);
    };

  RunList(&Serves, WriteServers);
  fclose(fd);
  return(TRUE);

  }
/*
* WriteServers()
*
* This function writes a domain server out to ctdltabl.sys.  See DOMAINS.C
* for more information on this list.
*/
static void WriteServers(char *name)
  {
  if( 1 != fwrite((char *)name, NAMESIZE, 1, fd))
    {
    perror("writeServers");
    poserr("writeServers");
    printf("WriteServers: problem writing server data\n");
    };
  }
/*
* GetDynamic()
*
* This does mallocs with error checking.
*/
void *special_GetDynamic(unsigned size, char *file, int line)
  {
  void *temp;
  char msg[80];
/**
  if (cfg.BoolFlags.debug)
    {
    splitF(NULL,"GetDynamic(%04.4x, %s, %d)\n",size,file,line);
    };
**/
  if (size == 0) return NULL; /* Simplify code  */
  temp = calloc(1,size);
  /* printf("Requested %d bytes, received address %p\n", size, temp); */
  if (temp == NULL)
    {
    printf("Request for %u bytes of memory failed.\n", size);
    sprintf(msg, "Asked for %u bytes, unable to get it.\n", size);
    crashout(msg);
    }
  return temp;

  }
/*
* openFile()
*
* This opens one of the .sys files.
*/
void openFile(char *filename, FILE **fd)
  {
  /* We use fopen here rather than safeopen for link reasons */
  if ((*fd = fopen(filename, R_W_ANY)) == NULL)
    {
    printf("?no %s, cannot open it", filename);
    exit(SYSOP_EXIT);

    }

  }
/*
* FindServes()
*
* This is a find the server function.  It's used in the list of domain servers
* to allow us to search for a domain server based on the name of the domain.
*/
static void *FindServes(char *name, char *target)
  {
  if (cfg.BoolFlags.debug)
    {
    splitF(NULL,"FindServes( %s, %s)\n",name, target);
    };
  return (strCmpU(name, target) == SAMESTRING) ? name : NULL;

  }
