/*
 *                              Clean.c
 *
 * Cleanses a room of a user.
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

/*
 *                              History
 *
 * 88Jun23 HAW  Created
 */

/*
 *                              Contents
 *
 *      crashout()              irrecoverable error
 *      doRest()                Read msgs.
 *      handle()                Inserts msg into room slot, etc.
 *      indexRooms()            build RAM index to ctdlroom.sys
 *      init()                  Open up files for recovery effort.
 *      main()                  Master control.
 *      noteRoom()              enter room into RAM index
 */

extern CONFIG  cfg;             /* The main variable to be saved */
extern MessageBuffer msgBuf;    /* The -sole- message buffer */
extern FILE    *msgfl, *msgfl2; /* file descriptor for the msg file */
FILE *netLog=stdout;
extern rTable  *roomTab;        /* RAM index of rooms */
extern aRoom   roomBuf;         /* room buffer */
extern FILE    *roomfl;         /* file descriptor for rooms    */
extern int     thisRoom;        /* room currently in roomBuf    */

struct bad {
    label name;
    struct bad *next;
} base;

void handle(MSG_NUMBER mess);
void getUtilString(char *prompt, char *buf, int lim);
char init(int argc, char **argv);
void doMsgs(void);
int UtilGetch(void);
int  mPrintf(char *format, ...) {return 0; }  /* stub to quiet the linker */

/*
 * crashout()
 *
 * Irrecoverable error?  Crash out of the program.
 */
void crashout(str)
char *str;
{
    exit(printf(str));
}

/*
 * doRest()
 *
 * This loops thru msg file until finished.
 */
void doMsgs()
{
    MSG_NUMBER message, firstMessage;
    extern struct mBuf mFile1;

    startAt(msgfl, &mFile1, 0, 0);
    getMessage(getMsgChar, FALSE, FALSE, TRUE);
    message = atol(msgBuf.mbId);
    printf("%ld\n", message);
    firstMessage = message;
    handle(message);
    getMessage(getMsgChar, FALSE, FALSE, TRUE);
    message = atol(msgBuf.mbId);
    while (message != firstMessage) {
        printf("%ld\n", message);
        handle(message);
        getMessage(getMsgChar, FALSE, FALSE, TRUE);
        message = atol(msgBuf.mbId);
    }
}

/*
 * handle()
 *
 * This function handles setting up and saving room indexes, etc.
 */
void handle(mess)
MSG_NUMBER mess;
{
    int k, j;
    struct bad *cur;

    if (strCmpU(msgBuf.mbroom, roomBuf.rbname) != SAMESTRING) return;
    for (cur = base.next; cur != NULL; cur = cur->next)
        if (strCmpU(cur->name, msgBuf.mbauth) == SAMESTRING) return;

    printf("Message from %s\n", msgBuf.mbauth);

    for (k = 0; roomBuf.msg[k+1].rbmsgNo < mess && k < MSGSPERRM-1; k++)
        ;
    for (j = 0; j < k; j++) {       /* Move msgs up 1.      */
        roomBuf.msg[j].rbmsgLoc = roomBuf.msg[j+1].rbmsgLoc;
        roomBuf.msg[j].rbmsgNo  = roomBuf.msg[j+1].rbmsgNo;
    }
    roomBuf.msg[k].rbmsgNo  = mess;/* And now insert msg    */
    roomBuf.msg[k].rbmsgLoc = msgBuf.mbheadSector;
}

/*
 * init()
 *
 * This will set up the files for recovery.
 */
char init(argc, argv)
int argc;
char *argv[];
{
    label RoomName, UserName;
    int  roomNo, count = 0, i;
    SYS_FILE temp;
    struct bad *cur;

    cfg.weAre = UTILITY;
    if (!readSysTab(TRUE, TRUE)) crashout("\nDisaster!  I need CTDLTABL.SYS!");

    if (access(LOCKFILE, 0) != -1) {
        printf("Please do not run Clean from Outside Commands.\n");
        return FALSE;
    }

    cfg.weAre = UTILITY;
    mvToHomeDisk(&cfg.homeArea);
    /* open message file */

    InitMsgBase();
    makeSysName(temp, "ctdlroom.sys", &cfg.roomArea);
    /* open room file */
    openFile(temp, &roomfl);

    initRoomBuf(&roomBuf);

    if (argc != 2) {
        getUtilString("room name", RoomName, NAMESIZE);
    }
    else strCpy(RoomName, argv[1]);

    printf("Check for room %s\n", RoomName);

    if ((roomNo = roomExists(RoomName)) == ERROR) crashout("No such room!");

    getRoom(roomNo);

    for (i = 0; i < MSGSPERRM; i++) {
        roomBuf.msg[i].rbmsgNo = 0l;
        roomBuf.msg[i].rbmsgLoc = 0;
    }

    cur = &base;

    printf("Type in bad users names, end with no name:\n");
    do {
        getUtilString("", UserName, NAMESIZE);
        if (strLen(UserName) != 0) {
            count++;
            cur->next = GetDynamic(sizeof *cur);
            cur = cur->next;
            strCpy(cur->name, UserName);
            cur->next = NULL;
        }
    } while (strLen(UserName) != 0);

    if (count == 0) {
        printf("No users specified!\n");
        return FALSE;
    }
    return TRUE;
}

/*
 * main()
 *
 * This directs salvage proceedings and collects profits.
 */
int main(int, char **);
int main(argc, argv)
int argc;
char **argv;
{
    printf("Citadel CLEAN a room %s\n%s\n", VERSION_NAME, COPYRIGHT);
    if (init(argc, argv)) {
        puts("No error msgs.  Go off and do something for an hour (or 2).\n");
        doMsgs();
        putRoom(thisRoom);
        printf("Don't need to reconfigure.\n");
    }
    writeSysTab();
    return 0;
}

/*
 * getUtilString()
 *
 * This function gets a string from the user.
 */
void getUtilString(prompt, buf, lim)
char *prompt;
char *buf;
int  lim;       /* max # chars to read */
{
    char c;
    int  i;

    if(strLen(prompt) > 0) {
        printf("Enter %s\n : ", prompt, lim);
    }

    i   = 0;
    while (
        c = UtilGetch(),

        c       != 13
        && i     <  lim
    ) {

        /* handle delete chars: */
        if (c == BACKSPACE) {
            putchar(' ');
            putchar(BACKSPACE);
            if (i > 0) i--;
            else  {
                putchar(' ');
                putchar(BELL);
            }
        } else   buf[i++] = c;

        if (i >= lim) {
            putchar(BELL);
            putchar(BACKSPACE); i--;
        }

    }
    buf[i]  = '\0';
    printf("\n");
}

/*
 * roomExists()
 *
 * This function returns slot# of named room else ERROR.
 */
int roomExists(room)
char *room;
{
    int i;

    for (i = 0;  i < MAXROOMS;  i++) {
        if (
            roomTab[i].rtflags.INUSE == 1   &&
            strCmpU(room, roomTab[i].rtname) == SAMESTRING
        ) {
            return(i);
        }
    }
    return(ERROR);
}
