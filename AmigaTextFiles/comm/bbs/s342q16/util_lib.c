#include "ctdl.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
CONFIG    cfg;                   /* A buncha variables           */
LogTable  *logTab;               /* RAM index of pippuls         */
NetTable  *netTab;               /* RAM index of nodes           */
rTable    *roomTab;              /* RAM index of rooms           */
EVENT     *EventTab = NULL;
char             *indexTable = "ctdlTabl.sys";
struct floor     *FloorTab;
int              TopFloor;
extern char *R_W_ANY;

logBuffer logBuf;                       /* Log buffer of a person       */
logBuffer logTmp;                       /* Useful global buffer         */
int       thisLog;                      /* entry currently in logBuf    */
FILE      *logfl;                       /* log file descriptor          */
static struct
  {
  int checkMark;                      /* rudimentary integrity        */
  int cfgSize;                        /* sizeof cfg                   */
  int logTSize;                       /* logtab size                  */
  int endMark;                        /* another integrity check      */

  }  integrity;
extern char *R_W_ANY;
void *FindServes();
SListBase Serves =
  {
  NULL, FindServes, NULL, free, NULL

  };

#define CHKM    7       /* major release        */
#define ENDM    8

void getLog(logBuffer *lBuf, int n)
{
    long int s, r;

    if (lBuf == &logBuf)   thisLog      = n;

    r = LB_TOTAL_SIZE;                  /* To get away from overflows   */
    s = n * r;                          /* This should be offset        */

    fseek(logfl, s, 0);

    if (fread((char *)lBuf, LB_SIZE, 1, logfl) != 1)
      {
      printf("getLog: Could not read User Log entry: %d(pos = $ld, size=%ld)\n",n, s, r);
      printf("        position = %ld, size of read = %ld\n",s,r);
      }
    else
      {
      crypte(lBuf, LB_SIZE, n*3);           /* decode buffer    */
      if (fread((char *)lBuf->lbgen, GEN_BULK, 1, logfl) != 1)
        {
        printf("getLog: read fail reading general data(pos = %ld, size=%ld)\n",(s+r), (long)GEN_BULK);
        };
      if (fread((char *)lBuf->lbMail, MAIL_BULK, 1, logfl) != 1)
        {
        printf("getLog: read fail reading mail data(pos = %ld, size = %ld)\n", (s+r+GEN_BULK), (long)MAIL_BULK);
        };
      };
  }
/************************************************************************/
/*      readSysTab() restores state of system from CTDLTABL.SYS         */
/*      returns:        TRUE on success, else FALSE                     */
/*      MS-DOS fun: Here's the map --                                   */
/*      Word 1 == sizeof cfg                                            */
/*      Word 2 == sizeof logTab                                         */
/*      Word 3 == sizeof roomTab                                        */
/*      Word 4 -- thru x == cfg contents                                */
/*      x -- y == logTab                                                */
/*      y -- z == roomTab                                               */
/*      z -- a == netTab                                                */
/*      EOF                                                             */
/************************************************************************/
char readSysTab(char kill, char showMsg)
  {
  FILE *fd;
  extern char *READ_ANY;
  int         rover;
  long        bytes;
  SYS_FILE    name;
  char        caller;
  label       temp;
  caller = cfg.weAre;
  if ((fd = fopen(indexTable, READ_ANY)) == NULL)
    {
    printf("readSysTab:  open failed on %s\n",indexTable);
    perror("readSysTab");
    poserr("readSysTab");
    return(FALSE);

    }
  if (fread((char *)&integrity, sizeof integrity, 1, fd) != 1)
    {
    printf("readSysTab: Unable to read the integrity data\n");
    perror("readSysTab");
    poserr("readSysTab");
    return FALSE;

    }
  if (     integrity.checkMark != CHKM ||
  integrity.endMark != ENDM ||
  integrity.cfgSize != sizeof cfg)
    {
    printf("readSysTab: Integrity mismatch.  Old ctdltabl.sys?\n");
    return(FALSE);

    }
  if (!common_read(&cfg, (sizeof cfg), 1, fd, showMsg))
  return FALSE;
  /* Allocations for dynamic parameters */
  logTab = (LogTable *) GetDynamic(integrity.logTSize);
  roomTab = (rTable *) GetDynamic(MAXROOMS * (sizeof (*roomTab)));
  if (cfg.netSize)
  netTab = (NetTable *) GetDynamic(sizeof (*netTab) * cfg.netSize);
  else
  netTab = NULL;
  if (cfg.EvNumber)
  EventTab  = (EVENT *)
  GetDynamic(sizeof (*EventTab) * cfg.EvNumber);
  /* "- 1" is kludge */
  if (integrity.logTSize != sizeof (*logTab) * cfg.MAXLOGTAB)
    {
    printf("readSysTab: Integrity check: size mismatch on logtable\n");
    printf("            Integrity size is %ld\n",integrity.logTSize);
    printf("            computed  size is %ld\n",sizeof (*logTab) * cfg.MAXLOGTAB);
    return(FALSE);

    }
  if (!common_read(logTab, integrity.logTSize, 1, fd, showMsg))
    {
    printf("readSysTab: Unable to load logTab\n");
    return(FALSE);
    };
  if (!common_read(roomTab, (sizeof (*roomTab)) * MAXROOMS, 1, fd, showMsg))
    {
    printf("readSysTab: Unable to load roomTab\n");
    return(FALSE);
    };
  if (cfg.netSize)
    {
    for (rover = 0; rover < cfg.netSize; rover++)
      {
      if (!common_read(&netTab[rover], NT_SIZE, 1, fd, showMsg))
        {
        printf("readSysTab: Unable to load netTab[%d](size = %ld)\n",rover, (long)NT_SIZE );
        return(FALSE);
        };
      netTab[rover].netTRooms =
      (SharedRoom *) GetDynamic(SR_BULK);
      if (!common_read(netTab[rover].netTRooms, SR_BULK, 1, fd, showMsg))
        {
        printf("readSysTab: Unable to load netTab[%d]/netTRooms(size = %ld)\n",rover, (long)NT_SIZE );
        return(FALSE);
        };

      }

    }
  if (cfg.EvNumber)
    {
    if (!common_read(EventTab, (sizeof(*EventTab) * cfg.EvNumber), 1, fd,
    showMsg))
      {
      printf("readSysTab: Unable to load EventTab(size = %ld)\n",(sizeof(*EventTab) * cfg.EvNumber) );
      return(FALSE);
      };

    }
  for (rover = 0; rover < cfg.DomainHandlers; rover++)
    {
    if (!common_read(temp, NAMESIZE, 1, fd, showMsg))
      {
      printf("readSysTab: Unable to load DomainHandles[%d](size = %ld)\n",rover, (long)NAMESIZE);
      return(FALSE);
      };
    AddData(&Serves, strdup(temp), NULL, FALSE);

    }
  fclose(fd);
  makeSysName(name, "ctdlflr.sys", &cfg.floorArea);
  if ((fd = fopen(name, R_W_ANY)) == NULL)
    {
    if (caller != CONFIGUR)
      {
      printf("readSysTab:  open failed on %s\n",name);
      perror("readSysTab");
      poserr("readSysTab");
      return(FALSE);
      };
    }
  else
    {
    totalBytes(&bytes, fd);
    FloorTab = (struct floor *) GetDynamic((int) bytes);
    if (fread((char *)FloorTab, (int) bytes, 1, fd) != 1)
      {
      printf("problem reading floor tab");
      printf("readSysTab: problem reading floor table(%s)\n",name);
      perror("readSysTab");
      poserr("readSysTab");
      fclose(fd);
      if (caller != CONFIGUR) return FALSE;

      }
    else
      {
      fclose(fd);
      TopFloor = (int) bytes/sizeof(*FloorTab);

      }

    }
  crypte(cfg.sysPassword, sizeof cfg.sysPassword, 0);
  return(TRUE);

  }
/************************************************************************/
/*      common_read() reads in from file the important stuff            */
/*      returns:        TRUE on success, else FALSE                     */
/************************************************************************/
static int common_read(void *block, int size, int elements, FILE *fd,
char showMsg)
  {
  if (size == 0) return TRUE;
  if (fread((char *)block, size, elements, fd) != 1)
    {
    printf("common_read: Unable to read data from file(%d records, %d size\n",elements, size);
    perror("readSysTab");
    poserr("readSysTab");
    return FALSE;

    };
  return TRUE;

  }

void *special_GetDynamic(unsigned size, char *file, int line)
  {
  void *temp;
  if (size == 0) return NULL; /* Simplify code                */
  if ((temp = (void *)malloc(size)) == NULL)
    {
    printf("Request for %u bytes failed.\n", size);
    exit(100);
    }
  memset(temp, 0, size);
  return temp;

  }

void crypte(void *buf, unsigned len, unsigned seed)
  {
  static AN_UNSIGNED *b;      /* Make this static for speed (I guess),*/
  static  int c, s;           /* since register variables not around  */
  if( cfg.cryptSeed == 0)return;
  seed        = (seed + cfg.cryptSeed) & 0xFF;
  b           = (AN_UNSIGNED *)buf;
  c           = len;
  s           = seed;
  for (;  c;  c--)
    {
    *b++   ^= s;
    s       = (s + CRYPTADD)  &  0xFF;

    }

  }

UNS_16 hash(char *str)
{
    UNS_16  h, shift;

    for (h=shift=0;  *str;  shift=(shift+1)&7, str++) {
        h ^= (toUpper(*str)) << shift;
    }
    return h;
}
/************************************************************************/
/*      FindServes() find the server function                           */
/************************************************************************/
static void *FindServes(char *name, char *target)
  {
  return (strCmpU(name, target) == SAMESTRING) ? name : NULL;

  }
static FILE *fd;
/************************************************************************/
/*      writeSysTab() saves state of system in CTDLTABL.SYS             */
/*      returns:        TRUE on success, else ERROR                     */
/*      See readSysTab() to see what the CTDLTABL.SYS map looks like.   */
/************************************************************************/
int writeSysTab()
  {
  void WriteServers();
  extern char   *WRITE_ANY;
  int           rover;
  if ((fd = fopen(indexTable, WRITE_ANY)) == NULL)
    {
    printf("writeSysTab: problem writing %s\n",indexTable);
    perror("writeSysTab");
    poserr("writeSysTab");
    return(ERROR);
    };

  /* Write out some key stuff so we can detect bizarreness: */
  integrity.checkMark = CHKM;
  integrity.endMark = ENDM;
  integrity.cfgSize = sizeof cfg;
  integrity.logTSize = sizeof (*logTab) * cfg.MAXLOGTAB;
  if( 1 != fwrite((char *)&integrity, (sizeof integrity), 1, fd) )
    {
    printf("writeSysTab: problem writing integrity data\n");
    perror("writeSysTab");
    poserr("writeSysTab");
    return(ERROR);
    };

  crypte(cfg.sysPassword, sizeof cfg.sysPassword, 0);
  if( 1 != fwrite((char *)&cfg, (sizeof cfg), 1, fd) )
    {
    printf("writeSysTab: problem writing configuration data\n");
    perror("writeSysTab");
    poserr("writeSysTab");
    return(ERROR);
    };

  crypte(cfg.sysPassword, sizeof cfg.sysPassword, 0);
  if( 1 != fwrite((char *)logTab, (sizeof(*logTab) * cfg.MAXLOGTAB), 1, fd))
    {
    printf("writeSysTab: problem writing User Log table\n");
    perror("writeSysTab");
    poserr("writeSysTab");
    return(ERROR);
    };

  if( 1 != fwrite((char *)roomTab, (sizeof (*roomTab)) * MAXROOMS, 1, fd))
    {
    printf("writeSysTab: problem writing room table\n");
    perror("writeSysTab");
    poserr("writeSysTab");
    return(ERROR);
    };

  for (rover = 0; rover < cfg.netSize; rover++)
    {
    if( 1 != fwrite((char *)&netTab[rover], NT_SIZE, 1, fd))
      {
      printf("writeSysTab: problem writing Net Table[%d]\n",rover);
      perror("writeSysTab");
      poserr("writeSysTab");
      return(ERROR);
      };

    if( 1 != fwrite((char *)netTab[rover].netTRooms, SR_BULK, 1, fd))
      {
      printf("writeSysTab: problem writing Net Table/Room[%d]\n",rover);
      perror("writeSysTab");
      poserr("writeSysTab");
      return(ERROR);
      };

    };
  if (cfg.EvNumber)
  if( 1 != fwrite((char *)EventTab, (sizeof(*EventTab) * cfg.EvNumber), 1, fd))
    {
    printf("writeSysTab: problem writing Event Table\n");
    perror("writeSysTab");
    poserr("writeSysTab");
    return(ERROR);
    };

  RunList(&Serves, WriteServers);
  fclose(fd);
  return(TRUE);

  }
/************************************************************************/
/*      WriteServers() writes a server out to ctdltabl.sys              */
/************************************************************************/
static void WriteServers(char *name)
  {
  if( 1 != fwrite((char *)name, NAMESIZE, 1, fd))
    {
    printf("WriteServers: problem writing server data\n");
    perror("writeServers");
    poserr("writeServers");
    };
  }

void putLog(logBuffer *lBuf, int n)
{
    long int s, r;

    r = LB_TOTAL_SIZE;
    s = n * r;

    crypte(lBuf, LB_SIZE, n*3);         /* encode buffer        */

    if (cfg.weAre != CONFIGUR)        /* No need if configuring         */
        fseek(logfl, s, 0);

    if (fwrite((char *)lBuf, LB_SIZE, 1, logfl) != 1)
       {
       printf("putLog: failed to write user log entry %d(pos = %ld, size=%ld)\n",n,s,r);
       perror("putLog");
       poserr("putLog");
       }

    if (fwrite((char *)lBuf->lbgen, GEN_BULK, 1, logfl) != 1)
        {
        printf("putLog: write fail on general data(pos = %ld, size=%ld)\n",(s+r), (long)GEN_BULK);
        };

    if (fwrite((char *)lBuf->lbMail, MAIL_BULK, 1, logfl) != 1)
        {
        printf("putLog: write fail on mail data(pos = %ld, size = %ld)\n", (s+r+GEN_BULK), (long)MAIL_BULK);
        };

    crypte(lBuf, LB_SIZE, n*3);         /* encode buffer        */

    fflush(logfl);
}
/************************************************************************/
/*      openFile() opens one of the .sys files.                         */
/************************************************************************/
void openFile(char *filename, FILE **fd)
  {
  /* We use fopen here rather than safeopen for link reasons */
  if ((*fd = fopen(filename, R_W_ANY)) == NULL)
    {
    printf("?no %s, cannot open it", filename);
    exit(SYSOP_EXIT);

    }

  }
/************************************************************************/
/*      PersonExists() check to see if the given name is valid for mail */
/************************************************************************/
int PersonExists(char *name)
{
    int result;

    result = findPerson(name, &logTmp);
    if (result != ERROR) strCpy(name, logTmp.lbname);
    return result;
}
/************************************************************************/
/*      findPerson() loads log record for named person.                 */
/*      RETURNS: ERROR if not found, else log record #                  */
/************************************************************************/
int findPerson(char *name, logBuffer *lBuf)
{
    int  h, i, foundIt, logNo;

    if (strLen(name) == 0) return ERROR;

    if (strCmpU(name, "Citadel") != SAMESTRING) {
        h   = hash(name);
        for (foundIt = i = 0;  i < cfg.MAXLOGTAB && !foundIt;  i++) {
            if (logTab[i].ltnmhash == h) {
                getLog(lBuf, logNo = logTab[i].ltlogSlot);
                if (lBuf->lbflags.L_INUSE &&
                        strCmpU(name, lBuf->lbname) == SAMESTRING) {
                    foundIt = TRUE;
                }
            }
        }
    }
    else foundIt = FALSE;
    if (!foundIt)    return ERROR;
    else             return logNo;
}

/*
* lbyte()
*
* This function finds the 0 byte of a string, returns pointer to it...
*/
char *lbyte(char *l)
  {
  while (*l) l++;
  return l;

  }
/*
* NormStr()
*
* This function Deletes leading trailing blanks etc.
*/
void NormStr(char *s)
  {
  char *pc;
  pc = s;
  /* find end of string   */
  while (*pc)
    {
    if (*pc < ' ')   *pc = ' ';   /* zap tabs etc... */
    pc++;

    }
  /* no trailing spaces: */
  while (pc>s  &&  isSpace(*(pc-1))) pc--;
  *pc = '\0';
  /* no leading spaces: */
  while (*s == ' ')
    {
    for (pc=s;  *pc;  pc++)    *pc = *(pc+1);

    }
  /* no double blanks */
  for (;  *s;)
    {
    if (*s == ' '   &&   *(s+1) == ' ')
      {
      for (pc=s;  *pc;  pc++)    *pc = *(pc+1);

      }
    else s++;

    }
 }
/*
* CleanEnd()
*
* This function cleans up a message trailer for later display via Continue or
* .EH.  Inspired by Glen Heinz (MacCitadel).
*/
char *CleanEnd(char *text)
  {
  char *ptr;
  int  wc, lc;  /* Word Count and Letter Count */
  if (strLen(text) == 0) return text;
  ptr = lbyte(text) - 1;      /* End of text of msg */
  /*
  * Strip trailing whitespace.  We structure the loop this
  * way to avoid any chance of accidentally accessing memory outside
  * of the memory area.
  */
  while (ptr != text - 1)
    {
    if (!(*ptr == ' ' || *ptr == NEWLINE || *ptr == TAB)) break;
    ptr--;

    }
  ptr++;        /* point at byte following last significant character */
  *ptr = 0;     /* tie it off with a NULL */
  /* Now we want to find a "preferred place" */
  for (wc = lc = 0, ptr--; wc < 4 && ptr > text && lc < 35; ptr--, lc++)
    {
    if (*ptr == ' ') wc++;
    if (*ptr == NEWLINE) break;     /* can't go beyond embedded NEWLINE */

    }
  if (ptr == text) return ptr;  /* if msg is empty or < 35 chars long */
  /* else */       return (ptr + 1);    /* else return "favored" spot */

  }
