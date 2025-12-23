/*
*                               calllog.c
*
* handles call log of Citadel-86
*/
#include "ctdl.h"
/*
*                               history
*
* 88Sep02 HAW  Download/Upload log.
* 86Mar07 HAW  New users and .ts signals.
* 86Feb09 HAW  System up and down times.
* 86Jan22 HAW  Set extern var so entire system knows baud.
* 85Dec08 HAW  Put blank lines in file.
* 85Nov?? HAW  Created.
*/
/*
*                               Contents
*
*       logMessage()            Put out str to file.
*       fileMessage()           Handles file messages
*/
struct timeData
  {
  int   y, d, h, m;
  char  month[5];
  label person;
  char  newuser;
  char  evil;
  char  chat;

  };
struct timeData lgin;
extern CONFIG      cfg;
extern logBuffer   logBuf;
extern PROTO_TABLE Table[];
extern aRoom       roomBuf;
extern long byteRate;
extern MessageBuffer     msgBuf;
extern char        *WRITE_ANY, *READ_ANY;
extern char lastuser[28];              /* save for console display */
static char CallFn[80] = "";    /* can't use SYS_FILE here I fear */
static char CallCrash = FALSE;
static char *curBaud = NULL;
char FileTransStat;           /* status of file transgers */

/*
* logMessage()
*
* This function puts messages in the CALLLOG.SYS file depending on a number
* of different events and conditions.
*/
void logMessage(int val,char *str,char sig)
  {
  static int oldDay = 0;
  FILE *fd;
  int  yr, dy, hr, mn;
  char *mon, buf[100];
  char *format = "%s %s @ %d:%02d%s";
  char *SaveName = "callsave.sys";
  if (cfg.BoolFlags.debug)
    splitF(NULL,"logMessage(%d, %s, %x)\n",val,str,sig);

  if (CallCrash) return;
  if (val == BAUD || val == DOOR_RETURN)
    {
    byteRate = atoi(str) / 10;

    }
  if (cfg.Audit == 0) return;
  makeAuditName(CallFn, "calllog.sys");
  getCdate(&yr, &mon, &dy, &hr, &mn);
  switch (val)
    {
    case FIRST_IN:
    if (cfg.BoolFlags.IsDoor)break;
    oldDay = dy;
    sPrintf(buf, format, "System brought up", formDate(), hr, mn,
    "");
    CallMsg(CallFn, buf);
    break;
    #ifndef NO_DOORS
    case DOOR_RETURN:
    oldDay  = dy;
    curBaud = str;
    if ((fd = safeopen(SaveName, READ_ANY)) != NULL)
      {
      fread(&lgin, sizeof lgin, 1, fd);
      fclose(fd);
      unlink(SaveName);

      }
    else printf("No luck with %s.", SaveName);
    break;
    #endif
    case CRASH_OUT:
    case LAST_OUT:
    if (cfg.BoolFlags.IsDoor)return;
    sPrintf(buf, format, "System brought down", formDate(), hr, mn,
    (val == CRASH_OUT) ? " (crash exit!)" : "");
    CallMsg(CallFn, buf);
    return;
    #ifndef NO_DOORS
    case DOOR_OUT:
    if ((fd = safeopen(SaveName, WRITE_ANY)) != NULL)
      {
      fwrite(&lgin, sizeof lgin, 1, fd);
      fclose(fd);

      }
    else printf("No luck with %s.", SaveName);
    break;
    #endif
    case BAUD:
    curBaud     = str;
    lgin.person[0] = 0;
    goto datestuff;             /* ACK!  ACK!  ACK! */
    case L_IN:
    lgin.newuser = sig;
    strCpy(lgin.person, str);
    lgin.evil  = ' ';
    lgin.chat  = ' ';
    case INTO_NET:
    datestuff:
    lgin.y = yr;
    lgin.d = dy;
    lgin.h = hr;
    lgin.m = mn;
    strcpy(lgin.month, mon);
    break;
    case TRIED_CHAT:
    lgin.chat  = 'C';
    break;
    case EVIL_SIGNAL:
    lgin.evil  = 'E';
    break;
    case BADWORDS_SIGNAL:
    lgin.evil  = 'B';
    break;
    case CARRLOSS:
    homeSpace();        /* back to our regular lair, don't break! */
    case L_OUT:
    /*
    * curBaud's length 0 means the user is on the system console.
    * So this code means "If no person is logged in and
    * Anonymous session logging is off OR the anonymous user
    * was at the system console ..."
    */
    if( curBaud == NULL ) curBaud  = "";  /* string of length 0 */
    if (!lgin.person[0] &&
    !(cfg.BoolFlags.AnonSessions && strLen(curBaud) != 0))
      {
      curBaud = "";
      break;

      }
    if (oldDay != dy)
    CallMsg(CallFn, "");
    sPrintf(buf,
    "%-22s: %2d%s%02d %2d:%02d - %2d:%02d (%s)",
    (strLen(lgin.person)) ? lgin.person : "<No Login>",
    lgin.y, lgin.month, lgin.d, lgin.h, lgin.m,
    hr, mn, (curBaud == NULL || strLen(curBaud) == 0) ?
    "sysConsole" : curBaud);
    if (lgin.newuser) sPrintf(lbyte(buf), " %c", lgin.newuser);
    if (sig != 0) sPrintf(lbyte(buf), " %c", sig);
    if (lgin.evil != ' ') sPrintf(lbyte(buf), " %c", lgin.evil);
    if (lgin.chat != ' ') sPrintf(lbyte(buf), " %c", lgin.chat);
    CallMsg(CallFn, buf);
    if( strLen(lgin.person) )strcpy(lastuser,lgin.person);
    lgin.person[0] = 0;
    oldDay = dy;
    if (val == CARRLOSS) curBaud = NULL;
    goto datestuff;
    break;
    case OUTOF_NET:
    if (cfg.Audit == 1)
      {
      sPrintf(buf,
      "System in network mode: %d%s%02d %2d:%02d - %2d:%02d",
      lgin.y, lgin.month, lgin.d, lgin.h, lgin.m, hr, mn);
      CallMsg(CallFn, buf);

      }
    break;
    default:
    splitF(NULL," Bad Call to logMessage(%ld, %s,%d)\n",val,str,(int)sig);

    }

  }
/*
* fileMessage()
*
* This function handles the upload/download file log.
*/
void fileMessage(char mode, char *fn, char IsDL, int protocol, long size)
  {
  long    work, hours, mins;
  static label  LastActive = "";
  char    logfn[100];
  int             yr, dy, hr, mn;
  char    *mon, *pr, buf[100];
  static struct timeData fData;
  if (protocol == ASCII || cfg.Audit == 0)
  return;
  makeAuditName(logfn, "filelog.sys");
  getCdate(&yr, &mon, &dy, &hr, &mn);
  switch (mode)
    {
    case FL_START:
    startTimer(USER_TIMER);
    fData.y = yr;
    fData.d = dy;
    fData.h = hr;
    fData.m = mn;
    strcpy(fData.month, mon);
    break;
    case FL_SUCCESS:
    case FL_FAIL:
        FileTransStat = mode;
    case FL_EX_END:
    if (strCmpU(LastActive, logBuf.lbname) != SAMESTRING)
      {
      sPrintf(buf, "\n%s on %d%s%02d @ %ld:", logBuf.lbname, yr, mon, dy,
      byteRate * 10);
      CallMsg(logfn, buf);
      strCpy(LastActive, logBuf.lbname);

      }
    work = chkTimeSince(USER_TIMER);
    hours = work / 3600;
    work -= (hours * 3600);
    mins  = work / 60;
    work -= (mins * 60);
    if (protocol > TOP_PROTOCOL)
    pr = FindProtoName(protocol);
    else
    pr = Table[protocol].GenericName;
    if (mode == FL_EX_END)
      {
      sPrintf(buf,
      "%2cFollowing files %c %s via %s %d:%02d - %d:%02d (%ld:%02ld:%02ld):",
      ' ', (IsDL) ? 'D' : 'U', roomBuf.rbname,
      pr,
      fData.h, fData.m, hr, mn,
      hours, mins, work);
      CallMsg(logfn, buf);
      CallMsg(logfn, msgBuf.mbtext);

      }
    else
      {
      sPrintf(buf,
      "%2c%s (%ld) %s %s: %d:%02d - %d:%02d (%ld:%02ld:%02ld) %s. %s",
      ' ', fn, size, (IsDL) ? "D" : "U", roomBuf.rbname,
      fData.h, fData.m, hr, mn,
      hours, mins, work,
      pr,
      (mode == FL_FAIL) ? "(FAILED)" : "");
      CallMsg(logfn, buf);

      }
    break;

    }

  }
/*
* CallMsg()
*
* This function is a generic message writing mechanism.  Given a filename, it
* appends the given message to the end of said file.
*/
void CallMsg(char *fn, char *str)
  {
  FILE  *fd;
  extern char *A_C_TEXT;
  if ((fd = safeopen(fn, A_C_TEXT)) != NULL)
    {
    fprintf(fd, "%s\n", str);
    fclose(fd);

    }
  else
    {
    CallCrash = TRUE;
    printf("Failure: filename is %s", fn);
    /*  crashout("CallMsg error!");  */

    }

  }
