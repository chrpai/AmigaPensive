/*
*       confg2.c
*
* Event configuration program for Citadel bulletin board system.
*/
#define CONFIGURE
#include "ctdl.h"
/*
*       History
*
* 87Oct02 HAW  Created.
*/
/*
*       Contents
*
* checkList()   searches for a given string
* EatEvent()    Digests a #event
* FigureNets()    eats a comma separated list of nets
* getLVal()   gets a field value from a #event line
*/
int EvWCt    = 0;
#define PARA1 "\nWARNING: An event of Class dl-time should only use type quiet\n"
#define PARA2 "\nWARNING: An event of Class anytime-net should only use type quiet\n"
#define PARA3 "\nWARNING: Events of Class chat-on and chat-off should only use type quiet\n"
#define PARA4 "\nWARNING: Events of Class newusers-allowed and newusers-disallowed should only use type quiet\n"
#define PARA5 "Events of Class until-done-net cannot be of type quiet.\n"
#define PARA6 "Events of Class netcache cannot be of type quiet.\n"
typedef struct
  {
  char *GenName;
  int  GenVal;

  }
GenList;

  /***  INDENT-OFF ***/


static GenList EvnDays[] =
  {
    {    "Sun", SUNDAYS     },
    {    "Mon", MONDAYS     },
    {    "Tue", TUESDAYS    },
    {    "Wed", WEDNESDAYS  },
    {    "Thu", THURSDAYS   },
    {    "Fri", FRIDAYS     },
    {    "Sat", SATURDAYS   },
    {    "All", ALL_DAYS    }
  };

static GenList EvnTypes[] =
  {
    {    "preempt",     TYPREEMPT  },
    {    "non-preempt", TYNON      },
    {    "quiet",       TYQUIET    }
  };


static GenList EvCls[] =
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
  SListBase Events ={
  NULL, NULL, NULL, EvFree, NULL

  };
   /**  INDENT-ON **/
extern CONFIG cfg;      /* The configuration variable   */
extern EVENT  *EventTab;
typedef struct
  {
  EVENT *Evt;
  char  doorname[10];     /* inefficient?  who cares...   */

  }
DLK;
int checkList(char *ptr, GenList listing[], int elements);
/*
* EatEvent()
*
* This function assimilates event parameters.  Format:
*
*    #event <days> <time> <class> <type> <duration> <warning string> <dep>
*/
int EatEvent(char *line, int offset)
  {
  EVENT *EvBuf;
  DLK   *DataLk;
  char  *ptr, temp[10];
  int   rover, i, Day;
  int   EvHour, EvMin;
  EvBuf = (EVENT *) GetDynamic(sizeof *EvBuf);
  if ((ptr = strchr(line, '\n')) != NULL)
  *ptr = 0;
  rover   = 6;  /* starting index into line '#event ' ... */
  Day     = FigureDays(getLVal(line, &rover, ' '));
  EvHour    = atoi(getLVal(line, &rover, ':'));
  EvMin   = atoi(getLVal(line, &rover, ' '));
  EvBuf->EvMinutes  = EvHour * 60 + EvMin;
  ptr     = getLVal(line, &rover, ' ');
  EvBuf->EvClass  = checkList(ptr, EvCls, NumElems(EvCls));
  if (EvBuf->EvClass != CLREL)
    {
    if (EvHour > 23 || EvMin > 59)
    illegal("Bad time specified for an event!\n");
    EvBuf->EvMinutes = EvHour * 60 + EvMin;

    }
  ptr     = getLVal(line, &rover, ' ');
  EvBuf->EvType = checkList(ptr, EvnTypes, NumElems(EvnTypes));
  EvBuf->EvDur  = atoi(getLVal(line, &rover, ' '));
  if( EvBuf->EvDur < 1)
    {
    printf("Warning, duration is less than 1 minute\n");
    };
  EvBuf->EvWarn = GetStoreQuote(line, cfg.codeBuf, &rover, &offset);
  switch (EvBuf->EvClass)
    {
    case CL_ANYTIME_NET:
    EvBuf->vars.Anytime.EvDeadTime = atoi(getLVal(line, &rover, ' ')) * 60l;
    EvBuf->vars.Anytime.EvAnyDur   = atoi(getLVal(line, &rover, ' '));
    if (EvBuf->vars.Anytime.EvDeadTime < 5 )
      {
      printf(" Warning, Anytime-net event has deadtime(%ld) less than 5 minutes\n",EvBuf->vars.Anytime.EvDeadTime);
      };
    if (EvBuf->vars.Anytime.EvAnyDur < 1 )
      {
      printf(" Warning, Anytime-net event has duration(%d) less than 1 minutes\n",EvBuf->vars.Anytime.EvAnyDur);
      };
    /* no break */
    case CLNET:
    case CL_UNTIL_NET:
    EvBuf->EvExitVal = FigureNets(getLVal(line, &rover, ' '));
    if (EvBuf->EvClass == CL_ANYTIME_NET && EvBuf->EvType != TYQUIET)
      {
      printf(PARA2);
      EvBuf->EvType = TYQUIET;

      }
    if (EvBuf->EvClass == CL_UNTIL_NET && EvBuf->EvType == TYQUIET)
      {
      illegal(PARA5);

      }
    break;
    case CL_NETCACHE:
    if (EvBuf->EvType == TYQUIET)
      {
      illegal(PARA6);

      }
    EvBuf->EvExitVal = FigureNets(getLVal(line, &rover, ' '));
    break;
    case CL_AUTODOOR:
    i = 0;
    GetStoreQuote(line, EvBuf->vars.EvUserName, &rover, &i);
    strCpy(temp, getLVal(line, &rover, ' '));
    break;
    case CL_CHAT_ON:
    case CL_CHAT_OFF:
    if (EvBuf->EvType != TYQUIET)
      {
      printf(PARA3);
      EvBuf->EvType = TYQUIET;

      }
    break;
    case CL_NEWUSERS_ALLOWED:
    case CL_NEWUSERS_DISALLOWED:
    if (EvBuf->EvType != TYQUIET)
      {
      printf(PARA4);
      EvBuf->EvType = TYQUIET;

      }
    break;
    case CL_REDIRECT: /* filename targetdir systemname */
    strCpy(EvBuf->vars.Redirect.EvFilename, getLVal(line, &rover, ' '));
    EvBuf->vars.Redirect.EvHomeDir = offset;
    strCpy(cfg.codeBuf + offset, getLVal(line, &rover, ' '));
    while (cfg.codeBuf[offset++])
    ;
    strCpy(EvBuf->vars.Redirect.EvSystem, line + rover + 1);
    break;
    case CLREL:
    case CLEXTERN:
    case CL_DL_TIME:
    case CL_DOOR_TIME:
    EvBuf->EvExitVal = (MULTI_NET_DATA) atoi(getLVal(line, &rover, ' '));
    if (EvBuf->EvType != TYQUIET && EvBuf->EvClass == CL_DL_TIME)
      {
      printf(PARA1);
      EvBuf->EvType = TYQUIET;

      }
    if (EvBuf->EvClass != CL_DL_TIME &&
    EvBuf->EvClass != CL_DOOR_TIME)
      {
      if (EvBuf->EvExitVal >=0 && EvBuf->EvExitVal < 5)
      printf("\n\007WARNING: Event ERRORLEVEL value is "
      "between 0 and 4, all of which are used by "
      "Citadel.\007\n");

      }
    break;

    }
  /* now check to see which days this event is active */
  if (EvBuf->EvClass != CLREL)
    {
    for (rover = 0, i = 1; rover < 7; EvBuf->EvMinutes += 1440, rover++)
      {
      if (Day & i)
        {
        DataLk = (DLK *) GetDynamic(sizeof *DataLk);
        DataLk->Evt = (EVENT *) GetDynamic(sizeof *EvBuf);
        memcpy(DataLk->Evt, EvBuf, (long)sizeof *EvBuf);
        strCpy(DataLk->doorname, temp);
        AddData(&Events, DataLk, NULL, FALSE);
        cfg.EvNumber++;

        }
      i = i << 1;

      }

    }
  else
    {
    EvBuf->EvDur = EvHour * 60 + EvMin;
    DataLk = (DLK *) GetDynamic(sizeof *DataLk);
    DataLk->Evt = (EVENT *) GetDynamic(sizeof *EvBuf);
    memcpy(DataLk->Evt, EvBuf, (long)sizeof *EvBuf);
    AddData(&Events, DataLk, NULL, FALSE);
    cfg.EvNumber++;

    }
  return offset;

  }
/*
* checkList()
*
* This function searches for a given string in a list of arrays.  This is
* used for both classes and types.
*/
int checkList(char *ptr, GenList listing[], int elements)
  {
  int rover;
  char message[100];
  for (rover = 0; rover < elements; rover++)
  if (strCmpU(ptr, listing[rover].GenName) == SAMESTRING)
  return listing[rover].GenVal;
  sPrintf(message, "'%s' is not recognized!\n", ptr);
  illegal(message);
  return ERROR;

  }
/*
* getLVal()
*
* This function gets a field value from a #event line.
*/
char *getLVal(char *line, int *rover, char fin)
  {
  static char retVal[75];
  int         i;
  if (!line[*rover])
    {
    retVal[0] = 0;
    return retVal;

    }
  if (line[*rover] != '\n')
  (*rover)++;
  while (line[*rover] == ' ') (*rover)++;
  i = 0;
  while (line[*rover] != fin && line[*rover] != '\n' &&
  line[*rover] != 0       )
    {
    retVal[i++] = line[*rover];
    (*rover)++;

    }
  retVal[i] = 0;
  return retVal;

  }
/*
* FigureNets()
*
* This function takes a comma separated list of nets and eats it.
*/
MULTI_NET_DATA FigureNets(char *str)
  {
  MULTI_NET_DATA retVal, r;
  int temp;
  retVal = 0l;
  while (*str)
    {
    temp = atoi(str);
    if (temp < 1 || temp > 31) illegal("Bad member net value (0 < x < 32");
    r = 1l;
    retVal = retVal + (r << (temp - 1));
    while (*str != ',' && *str) str++;
    if (*str) str++;

    }
  return retVal;

  }
/*
* GetStoreQuote()
*
* This function reads in a quoted string and stores it in codeBuf.
*/
int GetStoreQuote(char *line, char *target, int *rover, int *offset)
  {
  int OldOffset;
  OldOffset = *offset;
  while (line[*rover] == ' ') (*rover)++;
  if (line[*rover] != '\"')
  illegal("Expecting a quote mark in event processor!\n");
  (*rover)++;
  if (line[*rover] == '\"')
    {
    (*rover)++;
    return ERROR;

    }
  while (line[*rover] != '\"' && line[*rover] != '\r')
    {
    target[(*offset)++] = line[*rover];
    (*rover)++;

    }
  target[(*offset)++] = 0;
  (*rover)++;
  return OldOffset;

  }
/*
* FigureDays()
*
* This function reads and interprets the <days> field.
*/
int FigureDays(char *vals)
  {
  int results, rover;
  char val[4];
  results = 0;
  while (*vals)
    {
    for (rover = 0; rover < 3; rover++)
    val[rover] = *vals++;
    val[3] = 0;
    results += checkList(val, EvnDays, NumElems(EvnDays));
    if (*vals) vals++;      /* bypass ',' */

    }
  return results;

  }
/*
* EvIsDoor()
*
* This function is used by RunList, etc.  It does final init on #events
* which control autodoors.
*/
void EvIsDoor(DLK *d)
  {
  if (d->Evt->EvClass == CL_AUTODOOR)
  d->Evt->EvExitVal = (MULTI_NET_DATA) FindDoorSlot(d->doorname);

  }
/*
* EvFree()
*
* This function frees part of a list of events.  Superfluous????
*/
void EvFree(DLK *d)
  {
  free(d->Evt);
  free(d);

  }
/*
* EventWrite()
*
* This function moves the element to the designated array position.
*/
void EventWrite(DLK *d)
  {
  memcpy(EventTab + EvWCt++, d->Evt,(long)sizeof (EVENT));

  }
