/*
*                               events.c
*
* Event handling code for Citadel.
*/
#include "ctdl.h"
/* #define EVENT_DEBUG */
/*
*                               history
*
* 91Aug16 HAW  New comment style.
* 89Jan23 HAW  Version 2 of event handling.
* 87Jun13 HAW  Created.
*/
/*
*                               contents
*
*      InitEvents()             Initialize event stuff
*      eventSort()              Sorts events
*      setPtrs()                Set up pointers
*      DoTimeouts()             Handles timeouts
*/
/*
* Event facility description
*
* Not available.
*/
/*
*                               Variables
*/
/* Internal housekeeping variables */
static int    ThisDay, ThisMinute;
static long   ThisSecond;
static char   warned = FALSE;
static long   ThisAbsolute;
/* These are lists of events */
typedef struct
  {
  SListBase List;
  long      NextAbs;
  long      LastAbs;
  int       toReturn;

  }
EventList;
static EventList Types [] =
  {
    { { NULL, NULL, eventSort, NULL, NULL   }, -1l, -1l, TRUE  },
    { { NULL, NULL, eventSort, NULL, NULL   }, -1l, -1l, FALSE },
    { { NULL, NULL, eventSort, NULL, NULL   }, -1l, -1l, FALSE }
  };
typedef struct
  {
  EVENT *Evt;
  long  finish;

  }
EvDoorRec;
static SListBase AutoDoors =
  {
  NULL, ChED, CmpED, free, NULL

  };
static SListBase Redirected =
  {
  NULL, ChRed, CmpED, free, NULL

  };
int ClassActive[EVENT_CLASS_COUNT];     /* which classes are active? */
#define NewUserAllowed()        (ClassActive[CL_NEWUSERS_ALLOWED])
#define NewUserDisAllowed()     (ClassActive[CL_NEWUSERS_DISALLOWED])
static SListBase EventEnds =
  {
  NULL, ChkTwoNumbers, CmpTwoLong, free, NULL

  };
/*
* This list helps solve the 'cheating downloader' problem.  We keep a list
* of downloaders and the amount of time spent downloading.  This list is
* cleared whenever the limit changes; thus, it is only effective during the
* duration of a specific download time limit #event.  We reference the list
* every time a user logs in (see LOG.C).
*/
SListBase DL_List =
  {
  NULL, ChkTwoNumbers, CmpTwoLong, free, EatTwoNumbers

  };
extern long *DL_Total;
char ResolveDls = TRUE;
char *DlMsgPtr = NULL;
static EVENT *Cur;
/* Externally needed variables */
char   ForceNet = FALSE;        /* True IFF ^A has been pressed         */
long   DeadTime;                /* Useful to keep this as a local       */
long   AnyNetLen;               /* Same here, tonto                     */
MULTI_NET_DATA AnyTimeNets;
long Door_Limit;
extern EVENT  *EventTab;
extern MessageBuffer msgBuf;
extern CONFIG cfg;
extern char   outFlag;
extern char   haveCarrier;
extern char   onConsole;
extern char   whichIO;          /* where is the I/O?            */
extern char   justLostCarrier;
extern char   ExitToMsdos;
extern int    exitValue;
extern long   Dl_Limit;    /* Yuck, but necessary */
void SetAbs(EventList *list, long LastAbs);

void ShowTW(TwoNumbers *);
void ExamineEvent(EVENT *);
static char *cl[] =
  {
  "network", "extern", "relative", "dl-time", "anytime-net",
  "doors-limit", "autodoor", "chat-on", "chat-off", "redirect",
  "new-users-allowed", "new-users-disallowed", "until-done"

  };
static char *ty[] =
  {
  "preempt", "non-preempt", "quiet"

  };
static char *dy[] =
  {
  "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"

  };


/*
* InitEvents()
*
* This function initializes events stuff.  This boils down to placing the
* various events into their types (implemented in SListBase structures), which
* automatically sorts them as they are added based on how close they are to
* the time they are supposed to occur.
*/
void InitEvents()
  {
  int      i;
  InitEvTimes();
  zero_array(ClassActive);              /* all classes off for now      */
  for (i = 0; i < cfg.EvNumber; i++)
    {
    if (EventTab[i].EvClass == CLREL)
      {
      EventTab[i].EvMinutes =
      (ThisMinute + EventTab[i].EvDur) % 10080;

      }
    AddData(&Types[EventTab[i].EvType].List, EventTab + i, NULL, FALSE);

    }
  SetAbs(&Types[0], -1l);
  SetAbs(&Types[1], -1l);
  SetAbs(&Types[2], -1l);

  }
/*
* InitEvTimes()
*
* This function initializes various time-based global variables for use by
* the event stuff.
*/
void InitEvTimes()
  {
  int      yr, dy, hr, mn, mon, secs, milli;
  long     temp;
  getRawDate(&yr, &mon, &dy, &hr, &mn, &secs, &milli);
  ThisDay = WhatDay();
  ThisMinute = (ThisDay * 1440) + (hr * 60) + mn;
  temp = (long) ThisMinute;
  ThisSecond = (temp * 60l) + secs;
  ThisAbsolute = CurAbsolute();

  }
/*
* eventSort()
*
* This helps sort a list of events (a type implementation) based on how soon
* each event is to occur.
*/
int eventSort(EVENT *s1, EVENT *s2)
  {
  if (during(s1) && during(s2))  return (int) (s1->EvMinutes - s2->EvMinutes);
  if (during(s1)) return -1;
  if (during(s2)) return 1;
  if ((passed(s1) && passed(s2)) ||
  (!passed(s1) && !passed(s2)))  return (int)(s1->EvMinutes - s2->EvMinutes);
  return (int) (s2->EvMinutes - s1->EvMinutes);

  }
/*
* during()
*
* Are we "during" this event?  This question is answered by checking to see
* if this event's time period (beginning to beginning+duration) encompasses
* the current time (within the week period).  The code is slightly more
* complex than might be otherwise expected due to the rollover point at the
* end of a week (Saturday).
*/
int during(EVENT *x)
  {
  int result;
  result = FALSE;
  if (ThisMinute >= x->EvMinutes && ThisMinute < x->EvMinutes + x->EvDur)
    {
    result = TRUE;
    }
  else
    {
    if (x->EvMinutes + x->EvDur > 10080
      && ThisMinute < (x->EvMinutes + x->EvDur) % 10080)
      {
      result = TRUE;
      };
    };
  return result;

  }
/*
* passed()
*
* This function determines if this event's start time has passed in terms of
* the week.
*/
int passed(EVENT *x)
  {
  if ((x->EvDur == 0 && x->EvMinutes <= ThisMinute) ||
  (x->EvDur != 0 && x->EvMinutes < ThisMinute))  return TRUE;
  return FALSE;

  }
/*
* SetAbs()
*
* This function calculates when the next event is supposed to occur for the
* given type.  This is calculated in terms of absolute time, making later
* comparisons much easier to deal with.  Since it's possible that this
* function will be called when we're actually already into some event's
* period, we have to take this into account.  That should explain the check
* for during() at the end of the function.
*/
void SetAbs(EventList *list, long LowLimit)
  {
  long temp, InSeconds;
  if ((Cur = (EVENT *) GetFirst(&list->List)) != NULL)
    {
    InitEvTimes();
    temp = (long) Cur->EvMinutes;
    InSeconds = temp * 60l;
    if (during(Cur))
    list->NextAbs = ThisAbsolute - WeekDiff(ThisSecond, InSeconds);
    else if (LowLimit != -1l)
      {
      if (ThisAbsolute - WeekDiff(ThisSecond, InSeconds) > LowLimit)
      list->NextAbs = ThisAbsolute - WeekDiff(ThisSecond, InSeconds);
      else
      list->NextAbs = ThisAbsolute + WeekDiff(InSeconds, ThisSecond);

      }
    else
    list->NextAbs = ThisAbsolute + WeekDiff(InSeconds, ThisSecond);

    }

  }
/*
* WeekDiff()
*
* This function figures out difference in time between events, taking into
* account the week rollover stuff.  The difference is returned in seconds.
*/
long WeekDiff(long future, long now)
  {
  long diff;
  if( cfg.BoolFlags.debug )
    {
    splitF(NULL,"WeekDiff(future:%ld, now:%ld) =",future, now);
    };
  diff = (long)( ( now > future) ? (604800l - now + future ) : ( future - now) );
  if( cfg.BoolFlags.debug )
    {
    splitF(NULL,"%ld\n",diff);
    };
  return diff;
  }
/*
* ChkPreempt()
*
* This function will estimate whether the time to d/l will interfere with
* the next preemptive event.  If it does, then the warning string associated
* with this preemptive event is returned.  If there is no collision then
* NULL is returned.
*/
char *ChkPreempt(long estimated)
  {
  if ((Cur = GetFirst(&Types[0].List)) != NULL)
    {
    if (Types[0].NextAbs - CurAbsolute() < estimated)
    return (char *)(cfg.codeBuf + Cur->EvWarn);

    }
  return NULL;          /* No preemptive events to worry about */

  }
/*
* CheckAutoDoor()
*
* This function checks to see if the given login name has an autodoor that
* should be executed.  It returns -1 if no such autodoor exists, otherwise
* it returns the EvExitVal value associated with the autodoor entry.  This
* function resides in here rather than the door source because autodoors
* are implemented as #events.
*/
int CheckAutoDoor(char *name)
  {
  EvDoorRec rec;
  rec.Evt = GetDynamic(sizeof (EVENT)); /* get around possible bug */
  strCpy(rec.Evt->vars.EvUserName, name);
  if ((Cur = SearchList(&AutoDoors, &rec)) != NULL)
    {
    free(rec.Evt);
    return (int) Cur->EvExitVal;

    }
  free(rec.Evt);
  return ERROR;

  }
/*
* DoTimeouts()
*
* This is the function responsible for actual checking of timeouts.  It
* returns TRUE if you want modIn to break out, too.  It should only be called
* from modIn().
*/
char DoTimeouts()
  {
  int  yr, dy, hr, mn, temp, mon, secs, milli;
  #ifdef SYSTEM_CLOCK
  static int LastMinute = -1;
  #endif
  extern int PriorityMail;
  TwoNumbers *tmp;
  EvDoorRec  *evtmp;
  extern SListBase UntilNetSessions;
  getRawDate(&yr, &mon, &dy, &hr, &mn, &secs, &milli);
  #ifdef SYSTEM_CLOCK
  if (LastMinute != mn || ForceNet )
    {
    ScrTimeUpdate(hr, mn);
    LastMinute = mn;

    }
  else
    {
    BeNice(INUSE_PAUSE);  /* check for ARexx input and sleep */
    };
  #endif
  ThisMinute = (WhatDay() * 1440) + (hr * 60) + mn;
  ThisAbsolute = (long) ThisMinute;
  ThisSecond = ThisAbsolute * 60 + secs;
  ThisAbsolute = CurAbsolute();
  /* First we deal with events which are deactivating */
  if ((tmp = GetFirst(&EventEnds)) != NULL)
    {
    if (ThisAbsolute >= tmp->second)
      {
      /* event is ending! */
      ClassActive[tmp->first] = FALSE;
      KillData(&EventEnds, tmp);                /* take it off the list */
      if (tmp->first == CL_DL_TIME)
      ResolveDLStuff();

      }

    }
  if ((evtmp = GetFirst(&AutoDoors)) != NULL)
    {
    if (ThisAbsolute >= evtmp->finish)
      {
      /* autodoor finish */
      KillData(&AutoDoors, evtmp);

      }

    }
  if ((evtmp = GetFirst(&Redirected)) != NULL)
    {
    if (ThisAbsolute >= evtmp->finish)
      {
      /* redirection finish */
      KillData(&Redirected, evtmp);

      }

    }
  /* Next we deal with preemptive events, which are Type 0 */
  /* give a warning at T-5 */
  if ((Cur = GetFirst(&Types[0].List)) != NULL &&
  !warned && Types[0].NextAbs - ThisAbsolute < 300l && onLine())
    {
    temp = Cur->EvMinutes % 1440;
    warned = TRUE;
    outFlag = IMPERVIOUS;
    Output_Citadel_Message("ESYSDN",temp/60,temp%60,(long)Cur->EvWarn+(long)cfg.codeBuf);
    outFlag = OUTOK;
    return (char)FALSE;

    }
  else if (Cur != NULL && Types[0].NextAbs < ThisAbsolute)
    {
    if (onLine())
      {
      /* first boot off user, next time do event */
      outFlag = IMPERVIOUS;
      Output_Citadel_Message("EVHAPP",(long)Cur->EvWarn + (long)cfg.codeBuf, NULL, NULL);
      if (onConsole)
        {
        /* Ugly cheat */
        onConsole = FALSE;
        whichIO = MODEM;
        justLostCarrier = TRUE;
        EnableModem(FALSE);

        }
      else
      HangUp(FALSE);
      outFlag = OUTOK;
      return (char)TRUE;

      }
    return (char)FigureEvent(0);

    }
  if (!onLine() && (Cur = GetFirst(&Types[1].List)) != NULL &&
  Types[1].NextAbs < ThisAbsolute)
  return (char)FigureEvent(1);
  if ((Cur = GetFirst(&Types[2].List)) != NULL &&
  Types[2].NextAbs < ThisAbsolute)
  return (char)FigureEvent(2);
  /* check priority mail -- odd place for the check, but wotthehell */
  if (PriorityMail)
    {
    if (!onLine())
      {
      netController((hr*60) + mn, 0, PRIORITY_MAIL, ANYTIME_NET, 0);
      PriorityMail = 0;

      }

    }
  /* handle anytime netting here - is special type of thing */
  if (chkTimeSince(NEXT_ANYNET) > DeadTime || ForceNet)
    {
    if (ClassActive[CL_ANYTIME_NET])
      {
      if (!onLine())
        {
        netController((hr * 60) + mn, AnyNetLen,
        AnyTimeNets, ANYTIME_NET, 0);
        ScrNewUser();
        ForceNet = FALSE;

        }

      }
    else ForceNet = FALSE;
    startTimer(NEXT_ANYNET);

    }
  /*
  * now see if we have any other net sessions due to be run.  These are
  * sessions scheduled by the user from the Net menu. If so, run each of
  * them by killing the list (the kill function should run each).
  */
  if (!onLine() && GetFirst(&UntilNetSessions) != NULL)
    {
    KillList(&UntilNetSessions);

    }
  return (char)ExitToMsdos;

  }
/*
* ResolveDLStuff()
*
* Resolves the DL list when a limit changes.  Basically, we clear the list of
* all users, but if someone is currently on, we wish to retain the user's
* record.  So, we make a copy of it, clear the list, and then restore the
* user's record to the list.  This code and related code elsewhere should
* help minimize the 'cheating user' problem.
*/
void ResolveDLStuff()
  {
  TwoNumbers  *tmp = NULL;
  extern int  thisLog;  /* entry currently in logBuf    */
  extern char loggedIn;
  if (!ResolveDls)
    {
    ResolveDls = TRUE;
    return;

    }
  if (loggedIn)         /* this makes a copy if someone is on */
  tmp = MakeTwo(thisLog, *DL_Total);
  KillList(&DL_List);           /* Now clear the list           */
  if (tmp != NULL)
    {
    /* If someone is on, re-add to the list */
    AddData(&DL_List, tmp, NULL, TRUE);
    DL_Total = &tmp->second;    /* And keep pointing    */

    }

  }
/*
* FigureEvent()
*
* This function handles an event becoming active and takes action.
*/
int FigureEvent(int index)
  {
  long ThisAbs, temp, InSeconds, EndIt = -1l, CalcEnd;
  EvDoorRec *EvDoor;
  if( cfg.BoolFlags.debug )
    {
    splitF(NULL," Event Type - %d", index);
    splitF(NULL," Class:%s Type:%s\n ", cl[Cur->EvClass], ty[Cur->EvType]);
    splitF(NULL,"%s -  ", dy[Cur->EvMinutes / 1440]);
    temp = Cur->EvMinutes % 1440;
    splitF(NULL,"%d:%02d %d %lx\n", temp/60, temp%60, Cur->EvDur, Cur->EvExitVal);
    };
  temp = (long) Cur->EvMinutes;
  InSeconds = temp * 60l;
  ThisAbs = ThisAbsolute - WeekDiff(ThisSecond, InSeconds);
  CalcEnd = ThisAbs + (Cur->EvDur * 60l);
  if( cfg.BoolFlags.debug )
    {
    splitF(NULL," Event ThisABS: %ld CalcEnd: %ld ThisSec:%ld Insec:%ld\n"
    ,ThisAbs, CalcEnd, ThisSecond, InSeconds);
    };
  switch (Cur->EvClass)
    {
    case CL_UNTIL_NET:
    case CLNET:
    if( cfg.BoolFlags.debug )
      {
      splitF(NULL," Event CL_UNTIL_NET/CLNET\n");
      };
    netController(Cur->EvMinutes % 1440, Cur->EvDur, Cur->EvExitVal,
    (Cur->EvClass == CLNET) ? NORMAL_NET : UNTIL_NET,
    REPORT_FAILURE | LEISURELY);
    warned = FALSE;
    startTimer(NEXT_ANYNET);
    break;
    case CLEXTERN:
    case CLREL:
    if( cfg.BoolFlags.debug )
      {
      splitF(NULL," Event CLEXTERN/CLREL\n");
      };
    ExitToMsdos = TRUE;
    exitValue = (int) Cur->EvExitVal;
    return TRUE;        /* force it */
    case CL_DL_TIME:
    if( cfg.BoolFlags.debug )
      {
      splitF(NULL," Event CL_DL_TIME\n");
      };
    EndIt = CalcEnd;
    Dl_Limit = Cur->EvExitVal;
    ResolveDLStuff();
    DlMsgPtr = cfg.codeBuf + Cur->EvWarn;
    break;
    case CL_ANYTIME_NET:
    if( cfg.BoolFlags.debug )
      {
      splitF(NULL," Event CL_ANYTIME_NET\n");
      };
    EndIt = CalcEnd;
    /* gets eligible nets */
    AnyTimeNets = Cur->EvExitVal;
    DeadTime    = Cur->vars.Anytime.EvDeadTime;
    AnyNetLen   = Cur->vars.Anytime.EvAnyDur;
    break;
    case CL_DOOR_TIME:
    if( cfg.BoolFlags.debug )
      {
      splitF(NULL," Event CL_DOOR_TIME\n");
      };
    EndIt = CalcEnd;
    Door_Limit = Cur->EvExitVal;
    break;
    case CL_AUTODOOR:
    case CL_REDIRECT:
    if( cfg.BoolFlags.debug )
      {
      splitF(NULL," Event CL_AUTODOOR/REDIRECT\n");
      };
    EvDoor = GetDynamic(sizeof *EvDoor);
    EvDoor->Evt = Cur;
    EvDoor->finish = CalcEnd;
    AddData((Cur->EvClass == CL_AUTODOOR) ? &AutoDoors : &Redirected,
    EvDoor, NULL, TRUE);
    break;
    case CL_CHAT_ON:
    case CL_CHAT_OFF:
    if( cfg.BoolFlags.debug )
      {
      splitF(NULL," Event CL_CHAT_ON/OFF\n");
      };
    cfg.BoolFlags.noChat = (Cur->EvClass != CL_CHAT_ON);
    ScrNewUser();
    break;
    case CL_NEWUSERS_ALLOWED:
    case CL_NEWUSERS_DISALLOWED:
    if( cfg.BoolFlags.debug )
      {
      splitF(NULL," Event CL_NEW_USERS_ALLOWED/DISALLOWED\n");
      };
    cfg.BoolFlags.unlogLoginOk = (Cur->EvClass == CL_NEWUSERS_ALLOWED);
    EndIt = CalcEnd;
    break;
    case CL_NETCACHE:
    if( cfg.BoolFlags.debug )
      {
      splitF(NULL," Event CL_NETCACHE\n");
      };
    CacheMessages(Cur->EvExitVal, FALSE);
    break;
    default:    /* do nothing */
    if( cfg.BoolFlags.debug )
      {
      splitF(NULL," Event:Unknown \n");
      };

    };
  if (EndIt != -1l)
    {
    AddData(&EventEnds, MakeTwo(Cur->EvClass, EndIt), NULL, TRUE);
    ClassActive[Cur->EvClass] = TRUE;       /* turn it on, baby! */

    };
  FrontToEnd(&Types[index].List);       /* put event on end of list */
  Types[index].LastAbs = ThisAbs;
  SetAbs(&Types[index], Types[index].LastAbs);
  return Types[index].toReturn;

  }
/*
* ActiveEvents()
*
* This puts together something vaguely resembling a useful list of currently
* active events.
*/
void ActiveEvents(char *buf)
  {
  int i;
  sPrintf(lbyte(buf), "\n   Active Event,s:\n ");
  if (Dl_Limit_On())                sPrintf(lbyte(buf), "D-L time limit of %ld minutes.\n ", Dl_Limit);
  if (Door_Limit_On())              sPrintf(lbyte(buf), "Door time limit of %ld minutes.\n ", Door_Limit);
  if (GetFirst(&Redirected) != NULL)sPrintf(lbyte(buf), "%d Redirect Files active.\n ",  RunList(&Redirected, NoFree));
  if (GetFirst(&AutoDoors) != NULL) sPrintf(lbyte(buf), "%d Auto Doors active.\n ", RunList(&AutoDoors, NoFree));
  if (NewUserAllowed())             sPrintf(lbyte(buf), "New Users Allowed event active.\n ");
  if (NewUserDisAllowed())          sPrintf(lbyte(buf), "New Users Disallowed event active.\n ");
  if (ClassActive[CL_ANYTIME_NET])
    {
    sPrintf(lbyte(buf), "Anytime net active for net(s) value:%08.8lX - ",AnyTimeNets);
    for (i = 0; i < 32; i++)
      {
      if ((1l << i) & AnyTimeNets) sprintf(lbyte(buf), "%d, ", i + 1);

      };
    buf[strLen(buf) - 2] = 0;
    strcat(buf, ".\n ");

    }

  }
/*
* ForceAnytime()
*
* This is an interface function for forcing anytime net.
*/
void ForceAnytime()
  {
  if (ClassActive[CL_ANYTIME_NET])
    {
    ForceNet = !ForceNet;
    ScrNewUser();

    }

  }
/*
* ChkTwoNumbers()
*
* check for equality of d1 vs d2.  This is used to search a list for a
* given value.
*/
void *ChkTwoNumbers(TwoNumbers *d1, TwoNumbers *d2)
  {
  if (d1->first == d2->first) return &d1->second;
  return NULL;

  }
/*
* MakeTwo()
*
* This creates a new record for addition to a list.
*/
TwoNumbers *MakeTwo(int First, long Second)
  {
  TwoNumbers *tmp;
  tmp = (TwoNumbers *) GetDynamic(sizeof *tmp);
  tmp->first  = First;
  tmp->second = Second;
  return tmp;

  }
/*
* CmpTwoLong()
*
* This functoin compares TwoNumbers in their long parts and returns a
* value suitable for use in sorting.
*/
int CmpTwoLong(TwoNumbers *d1, TwoNumbers *d2)
  {
  return (d1->second < d2->second) ? -1 : 1;

  }
/*
* ChED()
*
* This function is used to search for autodoors.  If the autodoor is found
* then the address to it is returned, otherwise NULL.
*/
void *ChED(EvDoorRec *d1, EvDoorRec *d2)
  {
  if (strCmpU(d1->Evt->vars.EvUserName, d2->Evt->vars.EvUserName) == SAMESTRING)
  return d1->Evt;
  return NULL;

  }
/*
* ChRed()
*
* This function is used to search to see if a given file is on the 'redirect'
* list.  Notice we check against both name and system origin.
*/
void *ChRed(EvDoorRec *d1, EvDoorRec *d2)
  {
  if (strCmpU(d1->Evt->vars.Redirect.EvFilename,
  d2->Evt->vars.Redirect.EvFilename) == SAMESTRING &&
  strCmpU(d1->Evt->vars.Redirect.EvSystem,
  d2->Evt->vars.Redirect.EvSystem) == SAMESTRING)
  return d1->Evt;
  return NULL;

  }
/*
* EatTwoNumbers()
*
* This function will eat a line of text from disk and make it into a record
* for placement on a disk.  It's simply a space separated string.
*/
void *EatTwoNumbers(char *line)
  {
  TwoNumbers *temp;
  char *space;
  if ((space = strchr(line, ' ')) == NULL)
  return NULL;
  temp = GetDynamic(sizeof *temp);
  temp->first = atoi(line);
  temp->second = atol(space + 1);
  return temp;

  }
/*
* WrtTwoNumbers()
*
* This function is used to write the contents of a TwoNumbers structure to
* disk.  This is used for saving information concerning a list while Citadel
* is temporarily down.
*/
void WrtTwoNumbers(TwoNumbers *d)
  {
  extern FILE *upfd;
  fprintf(upfd, "%d %d\n", d->first, d->second);

  }
/*
* CmpED()
*
* This function is used to sort a list of autodoors based on when they finish.
*/
int CmpED(EvDoorRec *d1, EvDoorRec *d2)
  {
  return (d1->finish < d2->finish) ? -1 : 1;

  }
/*
* RedirectFile()
*
* This function is used to discover if the given file should be redirected from
* the normal file reception area to somewhere else.
*
* If it should be then a pointer is returned to a string representing the new
* location; otherwise, NULL is returned.
*/
char *RedirectFile(char *filename, char *systemname)
  {
  EvDoorRec rec;
  rec.Evt = GetDynamic(sizeof (EVENT)); /* get around possible bug */
  /* prepare for search */
  strCpy(rec.Evt->vars.Redirect.EvFilename, filename);
  strCpy(rec.Evt->vars.Redirect.EvSystem, systemname);
  Cur = SearchList(&Redirected, &rec);
  free(rec.Evt);
  return(char *)( (Cur != NULL) ? cfg.codeBuf + Cur->vars.Redirect.EvHomeDir : NULL);

  }
/***********************************************************/
void EventShow()
  {
  int i;
  mPrintf("\nCurAbs=%ld\n ", CurAbsolute());
  mPrintf("ANYTIME NET is %d\n ", ClassActive[CL_ANYTIME_NET]);
  mPrintf("non-preempt nextabs is %ld, %ld seconds away\n ",
  Types[2].NextAbs, Types[2].NextAbs - CurAbsolute());
  mPrintf("Preemptive list (%ld):\n ", Types[0].NextAbs);
  RunList(&Types[0].List, ExamineEvent);
  modIn();
  mPrintf("Non-Preemptive list (%ld):\n ", Types[1].NextAbs);
  RunList(&Types[1].List, ExamineEvent);
  modIn();
  mPrintf("Quiet list (%ld):\n ", Types[2].NextAbs);
  RunList(&Types[2].List, ExamineEvent);
  modIn();
  mPrintf("Event ending list:\n ");
  RunList(&EventEnds, ShowTW);
  mPrintf("Anytime net is %s\n ", (ClassActive[CL_ANYTIME_NET]) ? "On" : "Off");
  if (ClassActive[CL_ANYTIME_NET])
    {
    msgBuf.mbtext[0] = 0;
    for (i = 0; i < 32; i++)
    if ((1l << i) & AnyTimeNets)
    sprintf(lbyte(msgBuf.mbtext), "%d, ", i + 1);
    if (strlen(msgBuf.mbtext))
    msgBuf.mbtext[strLen(msgBuf.mbtext) - 2] = 0;
    mPrintf("Anytime nets: %s\n ", msgBuf.mbtext);

    }

  }
void ShowTW(tw)
TwoNumbers *tw;
  {
  mPrintf("Class %s active, shuts down @%ld\n ", cl[tw->first],
  tw->second);

  }
void ExamineEvent(ev)
EVENT *ev;
  {
  int temp;
  mPrintf("%s %s ", cl[ev->EvClass], ty[ev->EvType]);
  mPrintf("%s ", dy[ev->EvMinutes / 1440]);
  temp = ev->EvMinutes % 1440;
  mPrintf("%d:%02d %d %lx\n ", temp/60, temp%60, ev->EvDur,
  ev->EvExitVal);

  }
