/*
 *                              AddUser.C
 *
 * Adds a user to a C86 installation.
 */

/*
 *                              History
 *
 * 90Dec28  HAW V1 Created.
 */

/*
 *                              Contents
 *
 *      main()                  main manager
 *      ShowArguments()         Usage stuff
 *      ParseFile()             parses the input file
 *      OnOff()                 parses an On/Off parameter
 *      AddAccount()            adds a new account to the system
 *      slideLTab()             inserts account into logTab
 *      storeLog()              stores the current log record
 *      crashout()              fatal error handler
 */

#include "ctdl.h"    /* header file  */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <time.h>
#include <proto/exec.h>
#include <dos/dos.h>
#include <pragmas/dos_pragmas.h>
#include "exec/memory.h"
#include "exec/ports.h"
#include "exec/exec.h"

#define AU_COPYRIGHT "Copyright (c) 1990, 1992 by Hue, Jr."

extern CONFIG    cfg;                   /* A buncha variables */
extern logBuffer logBuf;                /* Pippul buffer */
extern logBuffer logTmp;                /* Pippul buffer */
extern FILE      *logfl;                /* log file descriptor */
extern LogTable  *logTab;
extern int       thisLog;
extern rTable    *roomTab;              /* RAM index of rooms */

void ShowArguments(void);
void ParseFile(void);
void AddAccount(void);
char OnOff(char *where);

FILE *fd;
char onConsole, remoteSysop;

/*
 * main()
 *
 * This is the main manager for adduser.
 */
int main(int, char **);
int main(argc, argv)
int  argc;
char **argv;
{
        SYS_FILE tempName;

        printf("AddUser V3.41\n%s\n", AU_COPYRIGHT);

        if (argc != 2) {
                ShowArguments();
                exit(1);
        }

        if ((fd = fopen(argv[1], "r")) == NULL) {
                printf("Couldn't open %s.\n", argv[1]);
                exit(1);
        }

        if (!readSysTab(FALSE, TRUE)) exit(1);

        makeSysName(tempName, "ctdllog.sys",  &cfg.logArea);
        openFile(tempName, &logfl );

        initLogBuf(&logBuf);
        initLogBuf(&logTmp);

        ParseFile();

        AddAccount();

        writeSysTab();

        return 0;
}

/*
 * ShowArguments()
 *
 * This function shows the arguments for adduser.
 */
void ShowArguments()
{
        printf("Usage: adduser <filename>\n\n");
  printf("The format of the file should be as follows\n\n");
  printf("User name (string)\n");
  printf("Password (string)\n");
  printf("Screen width (numeric, in columns)\n");
  printf("Expert setting (ON or OFF)\n");
  printf("Floor setting (ON or OFF)\n");
  printf("Linefeeds setting (ON or OFF)\n");
  printf("Display Time on messages (ON or OFF)\n");
  printf("Last Old on New toggle (ON or OFF)\n");
  printf("\t<last line of required data>\n");
  printf("\t<following data is optional>\n");
  printf("Aide setting (ON or OFF) (default to OFF)\n");
  printf("Number of NULLS (numeric) (default 0)\n");
  printf("Number of LD credits (numeric) (default 0)\n");
  printf("Net privileges (ON or OFF) (default OFF)\n");
  printf("Door privileges (ON or OFF) (default OFF)\n");
  printf("Download privileges (ON or OFF) (default OFF)\n");
  printf("Permanent account (ON or OFF) (default OFF)\n");
  printf("Half Duplex setting (ON or OFF) (default OFF)\n");
  printf("Twit setting (ON or OFF) (default OFF)\n");
  printf(".ECD setting (numeric) (default 0)\n");
  printf(".ECZ setting (ON or OFF) (defaults to OFF)\n");
}

char Required = TRUE;
/*
 * ParseFile()
 *
 * This function parses the input file.
 */
void ParseFile(void)
{
        char line[200], good;
        int  i, h;

        zero_struct(logBuf.lbflags);    /* zero all flags       */
        /* user name */
        if (GetAString(line, sizeof line, fd) == NULL)
                crashout("File ended abruptly while reading username.");

        if (strLen(line) == 0)
                crashout("Name is not long enough.");

        if (strLen(line) > 19)
                crashout("Name is too long.");

        if (PersonExists(line) != ERROR)
                crashout("Person already exists.");

        strCpy(logBuf.lbname, line);

        /* User password */
        if (GetAString(line, sizeof line, fd) == NULL)
                crashout("File ended abruptly while reading password.");

        if (strLen(line) < 3)
                crashout("Password is not long enough.");

        if (strLen(line) > 19)
                crashout("Password is too long.");

        h = hash(line);
        for (i = 0, good = TRUE; i < cfg.MAXLOGTAB && good; i++) {
                if (h == logTab[i].ltpwhash) good = FALSE;
        }

        if (!good)
                crashout("Password already exists.");

        strCpy(logBuf.lbpw, line);

        if (GetAString(line, sizeof line, fd) == NULL)
                crashout("File ended abruptly while reading screen width.");

        if (!isdigit(line[0]))
                crashout("The screen width doesn't seem to be a numeral.");

        logBuf.lbwidth = atoi(line);
        logBuf.lbflags.EXPERT = OnOff("Expert setting");
        logBuf.lbflags.FLOORS = OnOff("Floors setting");
        logBuf.lbflags.LFMASK = OnOff("Linefeeds setting");
        logBuf.lbflags.TIME = OnOff("Time setting");
        logBuf.lbflags.OLDTOO = OnOff("Old on New display setting");
        Required = FALSE;
        logBuf.lbflags.AIDE = OnOff("Aide setting");

        /* NULLs */
        if (GetAString(line, sizeof line, fd) != NULL) {

                if (!isdigit(line[0]))
                        crashout("The NULLs value doesn't seem to be a numeral.");
                logBuf.lbnulls = atoi(line);
        }
        else logBuf.lbnulls = 0;

        /* LD credits */
        if (GetAString(line, sizeof line, fd) != NULL) {

                if (!isdigit(line[0]))
                        crashout("The NULLs value doesn't seem to be a numeral.");
                logBuf.credit = atoi(line);
        }
        else logBuf.credit = 0;

        logBuf.lbflags.NET_PRIVS = OnOff("Net privs setting");
        logBuf.lbflags.DOOR_PRIVS = OnOff("Door privs setting");
        logBuf.lbflags.DL_PRIVS = OnOff("DL privs setting");
        logBuf.lbflags.PERMANENT = OnOff("Permanent account setting");
        logBuf.lbflags.HALF_DUP = OnOff("Half-duplex setting");
        logBuf.lbflags.TWIT = OnOff("Twit setting");

        /* Delay stuff */
        if (GetAString(line, sizeof line, fd) != NULL) {

                if (!isdigit(line[0]))
                        crashout("The delay value doesn't seem to be a numeral.");
                logBuf.lbdelay = atoi(line);
        }
        else logBuf.lbdelay = 0;
        logBuf.lbflags.ALT_RE = OnOff(".ECZ setting");
}

/*
 * OnOff()
 *
 * This handles parsing an On/Off parameter.  If interpretation fails it
 * crashes out.
 */
char OnOff(char *where)
{
        char line[200];

        if (GetAString(line, sizeof line, fd) == NULL) {
                sprintf(line, "File ended abruptly while reading %s.", where);
                crashout(line);
        }

        if (stricmp(line, "on") == SAMESTRING)
                return TRUE;
        if (stricmp(line, "off") == SAMESTRING)
                return FALSE;

        if (Required) {
                sprintf(line, "Malformed value for %s.", where);
                crashout(line);
        }

        return FALSE;
}

/*
 * AddAccount()
 *
 * This function adds a new account to the system.
 */
void AddAccount()
{
        int good, ourSlot, i, g;
        MSG_NUMBER  low;
        char tmp[30];
        SYS_FILE    checkHeld;
        char   *LCHeld = "log%d.hld";

        for (good = cfg.MAXLOGTAB-1; good >= 0; good--)
                if (!logTab[good].ltpermanent) break;

        if (good < 0) good = cfg.MAXLOGTAB - 1; /* too bad */

        ourSlot = logTab[good].ltlogSlot;

        slideLTab(0, good);
        logTab[0].ltlogSlot = ourSlot;

        thisLog = ourSlot;

        /* copy info into record:       */
        logBuf.lbflags.L_INUSE    = TRUE;

        low = cfg.newest-50;
        if (cfg.oldest - low < 0x8000)   low = cfg.oldest;
        for (i=1;  i<MAXVISIT;  i++)   logBuf.lbvisit[i]= low;
        logBuf.lbvisit[ 0]= cfg.newest;
        logBuf.lbvisit[ (MAXVISIT-1)]= cfg.oldest;

        /* initialize rest of record:   */
        for (i = 0;  i < MAXROOMS;  i++) {
            if (roomTab[i].rtflags.PUBLIC == 1) {
                g = (roomTab[i].rtgen);
                logBuf.lbgen[i] = (g << GENSHIFT) + (MAXVISIT-1);
            } else {
                /* set to one less */
                g = (roomTab[i].rtgen + (MAXGEN-1)) % MAXGEN;
                logBuf.lbgen[i] = (g << GENSHIFT) + (MAXVISIT-1);
            }
        }
        memset(logBuf.lbMail, 0, MAIL_BULK);

        /* fill in logTab entries       */
        i = 0;
        logTab[i].ltpwhash      = hash(logBuf.lbpw)  ;
        logTab[i].ltnmhash      = hash(logBuf.lbname);
        logTab[i].ltlogSlot     = ourSlot       ;
        logTab[i].ltnewest      = logBuf.lbvisit[0];
        logTab[i].ltpermanent   = logBuf.lbflags.PERMANENT;

#ifdef MAIL_ALSO
        /* automatic mail for the new user. */
        makeSysName(checkHeld, (logBuf.lbflags.EXPERT) ? "expmail.blb" :
                                        "novmail.blb", &cfg.homeArea);
        if (access(checkHeld, 0) == 0) {
            i = thisRoom;
            getRoom(MAILROOM);
            ZeroMsgBuffer(&msgBuf);
            ingestFile(checkHeld, &msgBuf);
            strCpy(msgBuf.mbauth, (strLen(cfg.SysopName)) ?
                                    cfg.SysopName : "Citadel");
            strCpy(msgBuf.mbto, logBuf.lbname);
            putMessage(lBuf);
            getRoom(i);
        }
#endif

        storeLog();

        if (cfg.BoolFlags.HoldOnLost) {
                sprintf(tmp, LCHeld, ourSlot);
                makeSysName(checkHeld, tmp, &cfg.holdArea);
                unlink(checkHeld);
        }
}

/*
 * slideLTab()
 *
 * This function slides bottom N lots in logTab down.  For sorting.
 */
void slideLTab(int slot, int last)
{
    int i;

    /* open slot up: (movmem isn't guaranteed on overlaps) */
    for (i = last - 1;  i >= slot;  i--)  {
        memcpy(&logTab[i + 1], logTab + i,(long)cfg.sizeLTentry);
    }
}

/*
 * storeLog()
 *
 * This function stores the current log record.
 */
void storeLog()
{
    logTab[0].ltnewest    = cfg.newest;
    logBuf.lbvisit[0]     = cfg.newest;
    putLog(&logBuf, thisLog);
}

/*
 * crashout()
 *
 * Fatal error?  Out we go!
 */
void crashout(char *s)
{
        printf("Ooops: %s\n", s);
        exit(1);
}
