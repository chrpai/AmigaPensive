/*
 *                              popular.c
 *
 * Rough popularity estimator for rooms.
 */

#include "ctdl.h"
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

/*
 *                              history
 *
 * 90Apr27 HAW  Version 2.3 - sorted output.
 * 89Oct13 HAW  Version 2.2.
 * 86May12 HAW  Version 2.1.
 * 86Apr?? HAW  Version 2.0.
 * 86Apr07 HAW  Version 1.1.
 * 86Apr01 HAW  Created.
 */

/*
 *                              contents
 *
 *      crashout()              irrecoverable error
 *      main()                  Main controller for this program
 */
int  mPrintf(char *format, ...) {return 0; }  /* stub to quiet the linker */
FILE            *netLog=stdout;

#define LINE \
"------------------------------------------------------------------------------"

struct counts {
    int  forgot, num;
    int  messages;
    long bytes;
} *countTab;

int    messFlag = FALSE;
extern MessageBuffer  msgBuf; /* The -sole- message buffer      */
extern FILE *msgfl, *msgfl2;
extern FILE *upfd;  /* file descriptor for the msg file */
int PublicRoomCount = 0, ForgetCount = 0;
int activeLogs = 0, activeForgots = 0;

extern rTable *roomTab; /* RAM index of rooms   */
extern aRoom  roomBuf;  /* room buffer  */
extern FILE     *roomfl;        /* file descriptor for rooms    */
extern int      thisRoom;       /* room currently in roomBuf    */
extern logBuffer logBuf;        /* Log buffer of a person       */
extern int      thisLog;        /* entry currently in logBuf    */
extern FILE     *logfl; /* log file descriptor  */
extern CONFIG    cfg;   /* A buncha variables   */

void getCUdate(int *year, char **month, int *day, int *hours, int *minutes);
void flags(int argc, char *argv[]);
int mulchAcct(void);
void doMessages(void);
void display(void);
void handle(void);

/*
 * crashout()
 *
 * This handles the irrecoverable error.
 */
void crashout(str)
char *str;
{
    exit(printf(str));
}

/*
 * display()
 *
 * This function displays the results.
 */
void display()
{
    int rover;
    void ShowIt();
    int year, day, hours, minutes;
    char *month;
    int numcmp();
    static SListBase Sorted = { NULL, NULL, numcmp, NoFree, NULL };


    for (rover = 0;  rover < MAXROOMS;  rover++)
        if (roomTab[rover].rtflags.INUSE != 0 &&
            roomTab[rover].rtflags.PUBLIC != 0) {
            AddData(&Sorted, countTab + rover, NULL, FALSE);
            countTab[rover].num = rover;
        }

    getCUdate(&year, &month, &day, &hours, &minutes);
    printf("%d%s%02d @ %d:%02d\n\n", year, month, day, hours, minutes);

    printf(
"Out of a log of %d entries, %d are in use; %d (%d%%) have used <Z>Forget.\n\n",
 cfg.MAXLOGTAB, activeLogs, activeForgots, (100* activeForgots) / activeLogs);

    printf("%48s%15s\n", "Total", "Forgotten");
    printf("%-25s%-15s%-13s%s", "Room name", "# Forgotten", "Percentage",
                                                        "Percentage");
    if (messFlag) printf("%10s", "Messages");
    printf("\n%s\n", LINE);

    RunList(&Sorted, ShowIt);

    ForgetCount /= PublicRoomCount;
    printf("%s\n\n", LINE);
    printf("There are %d public rooms, ", PublicRoomCount);
    printf("an average %d forgetting them (%d%% and %d%%)\n\n",
                                                        ForgetCount,
    (activeLogs != 0) ? (100 * ForgetCount) / activeLogs : 0,
    (activeForgots != 0) ? (100 * ForgetCount) / activeForgots : 0);
}

/*
 * numcmp()
 *
 * This function will compare two counts and return who's higher.  This is
 * used in list sorting.
 */
int numcmp(struct counts *s, struct counts *d)
{
    return d->forgot - s->forgot;
}

/*
 * ShowIt()
 *
 * This will show a room with forgotten stats.  This is used in list handling.
 */
void ShowIt(struct counts *s)
{
    PublicRoomCount++;
    ForgetCount += s->forgot;
    printf("%-30s%-15d%-9d%3d", roomTab[s->num].rtname,
                                                        s->forgot,
   (activeLogs != 0) ? (100 * s->forgot) / activeLogs : 0,
   (activeForgots != 0) ? (100 * s->forgot) / activeForgots : 0);
    if (messFlag) printf("%11d (%ld)", s->messages,
                (s->messages == 0) ? 0 : s->bytes / (long) s->messages);
    printf("\n");
}

/*
 * doMessages()
 *
 * This function loops thru the msg file until finished.  It accumulates
 * statistics, etc.
 */
void doMessages()
{
    MSG_NUMBER msg, firstMessage;
    MSG_NUMBER total;   /* For stat keeping. */
    extern struct mBuf mFile1;

    fprintf(stderr, "Mulching...\n");
    InitMsgBase();
    startAt(msgfl, &mFile1, 0, 0);
    getMessage(getMsgChar, FALSE, TRUE, TRUE);
    msg = atol(msgBuf.mbId);
    fprintf(stderr, "%ld\n", msg);
    firstMessage = msg;
    handle();
    getMessage(getMsgChar, FALSE, TRUE, TRUE);
    msg = atol(msgBuf.mbId);
    total = 1;
    while (msg != firstMessage) {
        total++;
        fprintf(stderr, "%ld\r", msg);
        handle();
        getMessage(getMsgChar, FALSE, TRUE, TRUE);
        msg = atol(msgBuf.mbId);
    }
}

/*
 * flags()
 *
 * This function analyzes the command line arguments.  Very primitive.
 */
void flags(argc, argv)
int argc;
char *argv[];
{
    int i;

    for (i = 0; i < MAXROOMS; i++) {
        countTab[i].forgot = 0;
        countTab[i].messages = 0;
    }

    while (argc != 1) {
        argc--;
        if (strCmpU("-M", argv[argc]) == 0)
            messFlag = TRUE;
    }
}

/*
 * getCUdate()
 *
 * This function retrieves the system date and returns in the parameters.
 */
void getCUdate(int *year, char **month, int *day, int *hours, int *minutes)
{
    int m;
    int seconds, milli;
    static char *monthTab[13] = {"", "Jan", "Feb", "Mar",
                                "Apr", "May", "Jun",
                                "Jul", "Aug", "Sep",
                                "Oct", "Nov", "Dec" };

    getRawDate(year, &m, day, hours, minutes, &seconds, &milli);
    *month = monthTab[m];
    *year -= 1900;
}

/*
 * handle()
 *
 * This function searches the room base for the current msg's room, increments
 * the appropriate counters when found.
 */
void handle()
{
    int rover;
    int MsgLen(void);

    for (rover = 0; rover < MAXROOMS; rover++)
        if (strCmpU(roomTab[rover].rtname, msgBuf.mbroom) == 0) {
            countTab[rover].messages++;
            countTab[rover].bytes += MsgLen();
            break;
        }
}

/*
 * MsgLen()
 *
 * This function figures out the byte usage of the message.
 */
int MsgLen()
{
    return  (int) (     strLen(msgBuf.mbtext)  + strLen(msgBuf.mbauth) +
                    strLen(msgBuf.mbdate)  + strLen(msgBuf.mbtime) +
                    strLen(msgBuf.mbId)    + strLen(msgBuf.mboname) +
                    strLen(msgBuf.mborig)  + strLen(msgBuf.mbroom) +
                    strLen(msgBuf.mbsrcId) + strLen(msgBuf.mbto) +
                    strLen(msgBuf.mbaddr)  + strLen(msgBuf.mbOther) );
}

/*
 * main()
 *
 * This is the main controller.
 */
int main(int, char **);
int main(argc, argv)
int argc;
char *argv[];
{
    SYS_FILE temp;
    int Index;

    cfg.weAre = UTILITY;
    printf("\nCitadel Room Popularity Estimator %s\n%s\n", VERSION_NAME, COPYRIGHT);
    fprintf(stderr, "Munching...\n");
    printf("\n");
    if (readSysTab(FALSE, TRUE)) {
        initRoomBuf(&roomBuf);
        countTab = (struct counts *) GetDynamic(MAXROOMS * sizeof *countTab);
        flags(argc, argv);
        mvToHomeDisk(&cfg.homeArea);
        makeSysName(temp, "ctdlroom.sys", &cfg.roomArea);
        openFile(temp, &roomfl);
        makeSysName(temp, "ctdllog.sys", &cfg.logArea);
        openFile(temp, &logfl);
        initLogBuf(&logBuf);

        for (Index = 0; Index < cfg.MAXLOGTAB; Index++) {
            getLog(&logBuf, Index);
            if (logBuf.lbflags.L_INUSE) {
                activeLogs++;
                activeForgots += mulchAcct();
            }
            fprintf(stderr, "%d\r", Index);
        }

        if (messFlag)
            doMessages();

        display();
    }
    return 0;
}

/*
 * mulchAcct()
 *
 * This goes through all the rooms, checking them against the current acct
 * for statistics gathering.
 */
int mulchAcct()
{
    int i, j, g, usedForget;

    usedForget = 0;
    for (i = 0;  i < MAXROOMS;  i++) {
        if (roomTab[i].rtflags.PUBLIC != 0) {
            if ((logBuf.lbgen[i] >> GENSHIFT) != roomTab[i].rtgen)  {
                j = roomTab[i].rtgen - (logBuf.lbgen[i] >> GENSHIFT);
                if (j < 0)
                    g = -j;
                else
                    g = j;
                if (g == FORGET_OFFSET) {
                    usedForget = 1;
                    countTab[i].forgot++;
                }
            }
        }
    }
    return usedForget;
}
