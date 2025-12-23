/*
 *                              Recover2.c
 *
 * Rebuilds trashed CTDLROOM.SYS.
 */

#include "ctdl.h"    /* header file  */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fctype.h>
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
 * 87Oct11 HAW  V3 update.
 * 85Nov16 HAW  Modified for MS-DOS libraries.
 * 85Apr15 HAW  Move to MS-DOS.
 * 84Aug16 HAW  Now no need to reconfigure afterwards, gives stats.
 * 84Aug15 HAW  Documented.
 * 84Aug15 HAW  Created (In the wee hours).
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
 *      XzapRoomFile()          Zaps room file for you.
 */

extern CONFIG   cfg;    /* The main variable to be saved        */
extern MessageBuffer msgBuf;    /* The -sole- message buffer    */
extern FILE     *msgfl, *msgfl2; /* file descriptor for the msg file    */
extern FILE             *upfd;
extern rTable   *roomTab;       /* RAM index of rooms   */
extern aRoom    roomBuf;        /* room buffer  */
extern FILE     *roomfl;        /* file descriptor for rooms    */
extern int      thisRoom;       /* room currently in roomBuf    */
FILE            *netLog=stdout;

FILE *fopen();
char handle(MSG_NUMBER mess);
void init(void);
void XzapRoomFile(void);
void doRest(void);
int  mPrintf(char *format, ...) {return 0; }  /* stub to quiet the linker */

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
 * doRest()
 *
 * This function loops thru the msg file until finished, reconstructing the
 * rooms and their contents from the messages found.
 */
void doRest()
{
    MSG_NUMBER message, firstMessage;
    int total, mailCount;       /* For stat keeping. */
    extern struct mBuf mFile1;

    startAt(msgfl, &mFile1, 0, 0);
    getMessage(getMsgChar, FALSE, FALSE, TRUE);
    message = atol(msgBuf.mbId);
    printf("%ld\n", message);
    firstMessage = message;
    mailCount = 0;
    if (handle(message)) mailCount++;
    getMessage(getMsgChar, FALSE, FALSE, TRUE);
    message = atol(msgBuf.mbId);
    total = 1;
    while (message != firstMessage) {
        total++;
        printf("%ld\n", message);
        if (handle(message)) mailCount++;
        getMessage(getMsgChar, FALSE, FALSE, TRUE);
        message = atol(msgBuf.mbId);
    }
    printf("We have %u messages, with %u of them being in the mail.\n",
                                                total, mailCount);
}

/*
 * handle()
 *
 * This function handles setting up and saving room indexes, etc, based on the
 * current message.
 */
char handle(mess)
MSG_NUMBER mess;
{
    int  i, j, k;

    if (msgBuf.mbroom[0] == '\0') return FALSE; /* Can't handle */
    printf("Message for %s\n", msgBuf.mbroom);
    for (i = 0; i < MAXROOMS; i++)
        if (roomTab[i].rtname[0] == 0) break;
        else if (strCmpU(roomTab[i].rtname, msgBuf.mbroom) == 0) break;

    if (i == MAXROOMS) return FALSE;    /* If no room left in room      */
    if (i == MAILROOM) return TRUE;     /* file or room is Mail don't do*/
                                        /* anything.                    */
    printf("Being put in slot %d\n", i);
    getRoom(i);                         /* Get appropriate slot */
    strcpy(roomTab[i].rtname, msgBuf.mbroom);   /* Copy whether needs it*/
    strcpy(roomBuf.rbname, msgBuf.mbroom);
    if (roomBuf.rbflags.INUSE == FALSE)
        for (j = 0; j < MSGSPERRM - 1; j++)
            roomBuf.msg[j].rbmsgNo = 0l;
    roomBuf.rbflags.INUSE = TRUE;
    for (k = 0; roomBuf.msg[k+1].rbmsgNo < mess && k < MSGSPERRM-1; k++)
        ;
    for (j = 0; j < k; j++) {   /* Move msgs up 1.      */
        roomBuf.msg[j].rbmsgLoc = roomBuf.msg[j+1].rbmsgLoc;
        roomBuf.msg[j].rbmsgNo  = roomBuf.msg[j+1].rbmsgNo;
    }
    roomBuf.msg[k].rbmsgNo  = mess;/* And now insert msg    */
    roomBuf.msg[k].rbmsgLoc = msgBuf.mbheadSector;
    putRoom(i); /* And save our folly. */
    return FALSE;
}

/*
 * indexRooms()
 *
 * This will build a RAM index to CTDLROOM.SYS, and delete empty rooms.
 */
void indexRooms()
{
    int  goodRoom, m, roomCount, slot;

    roomCount = 0;
    for (slot = 0;  slot < MAXROOMS;  slot++) {
        getRoom(slot);
        if (roomBuf.rbflags.INUSE == 1) {
            roomBuf.rbflags.INUSE = 0;          /* clear "inUse" flag */
            for (m = 0, goodRoom = FALSE; m < MSGSPERRM && !goodRoom; m++) {
                if (roomBuf.msg[m].rbmsgNo > cfg.oldest) {
                    goodRoom    = TRUE;
                }
            }
            if (goodRoom   || roomBuf.rbflags.PERMROOM == 1)   {
                roomBuf.rbflags.INUSE = 1;
            }

            if (roomBuf.rbflags.INUSE == 1)
                roomCount++;
            else {
                roomBuf.rbflags.INUSE    = 0;
                roomBuf.rbflags.ISDIR   = 0;
                roomBuf.rbflags.PERMROOM = 0;
                roomBuf.rbflags.INUSE    = 0;
                putRoom(slot);
            }
        }
        noteRoom();
    }
}

/*
 * init()
 *
 * This function will set up the files for recovery.
 */
void init()
{
    char c;
    SYS_FILE temp;
    extern char *R_W_ANY, *W_R_ANY;

    cfg.weAre = UTILITY;
    if (!readSysTab(TRUE, TRUE)) crashout("\nDisaster!  I need CTDLTABL.SYS!");
    cfg.weAre = UTILITY;
    mvToHomeDisk(&cfg.homeArea);

    if (access(LOCKFILE, 0) != ERROR) {
        printf("Please do not run Recover2 using Outside Commands.\n");
        writeSysTab();
        exit(1);
    }
    /* open message file */

    InitMsgBase();

    makeSysName(temp, "ctdlroom.sys", &cfg.roomArea);
    /* open room file */
    if ((roomfl = fopen(temp, R_W_ANY)) == NULL) {
        if ((roomfl = fopen(temp, W_R_ANY)) == NULL)
            exit(printf("Hey! Can't create the room file?"));
    }
    initRoomBuf(&roomBuf);
    strcpy(roomTab[0].rtname, cfg.codeBuf + cfg.bRoom);
    strcpy(roomTab[1].rtname, "Mail");
    strcpy(roomTab[2].rtname, "Aide");
    for (c = 3; c < MAXROOMS; c++)
        roomTab[c].rtname[0] = 0;       /* Convenient way to zap roomTab */
}

/*
 * main()
 *
 * This directs salvage proceedings and collects profits.
 */
int main(void);
int main()
{
    printf("Citadel Room Recover II %s - %s\n\n", VERSION_NAME, COPYRIGHT);
    init();
    puts("No error msgs.  Go off and do something for an hour (or 2).\n");
    XzapRoomFile();
    doRest();
    indexRooms();
    writeSysTab();
    printf("Don't need to reconfigure.\n");
    return 0;
}

/*
 * noteRoom()
 *
 * This function will enter a room into RAM index array.
 */
void noteRoom()
{
    int   i;
    MSG_NUMBER last;

    last = 0l;
    for (i = 0;  i < MSGSPERRM;  i++)  {
        if (roomBuf.msg[i].rbmsgNo > last) {
            last = roomBuf.msg[i].rbmsgNo;
        }
    }
    roomTab[thisRoom].rtlastMessage = last      ;
    strCpy(roomTab[thisRoom].rtname, roomBuf.rbname) ;
    roomTab[thisRoom].rtgen     = roomBuf.rbgen  ;
    roomTab[thisRoom].rtflags.INUSE    = roomBuf.rbflags.INUSE;
    roomTab[thisRoom].rtflags.PUBLIC   = roomBuf.rbflags.PUBLIC;
    roomTab[thisRoom].rtflags.ISDIR    = roomBuf.rbflags.ISDIR;
    roomTab[thisRoom].rtflags.PERMROOM = roomBuf.rbflags.PERMROOM;
    roomTab[thisRoom].rtflags.SKIP      = roomBuf.rbflags.SKIP;
    roomTab[thisRoom].rtFlIndex = roomBuf.rbFlIndex;
}

/*
 * XzapRoomFile()
 *
 * This function erases and re-initializes ctdlroom.sys.
 */
void XzapRoomFile()
{
    int i;

    roomBuf.rbflags.PUBLIC   = TRUE;    /* This should be safe  */
    roomBuf.rbflags.INUSE    = FALSE;
    roomBuf.rbflags.ISDIR = FALSE;
    roomBuf.rbflags.PERMROOM = FALSE;
    roomBuf.rbgen       = 0;
    roomBuf.rbname[0]   = 0;    /* unnecessary -- but I like it...      */
    roomBuf.rbFlIndex   = 0;
    for (i=0;  i<MSGSPERRM;  i++) {
        roomBuf.msg[i].rbmsgNo = 0l;
        roomBuf.msg[i].rbmsgLoc = 0;
    }

    printf("maxrooms=%d\n", MAXROOMS);

    for (i=0;  i<MAXROOMS;  i++) {
        RoomSys(i);
        printf("clearing room %d\n", i);
        putRoom(i);
    }

    /* Lobby> always exists -- guarantees us a place to stand! */
    thisRoom    = 0     ;
    strcpy(roomBuf.rbname, cfg.codeBuf + cfg.bRoom)     ;
    roomBuf.rbflags.PERMROOM = TRUE;
    roomBuf.rbflags.PUBLIC   = TRUE;
    roomBuf.rbflags.INUSE    = TRUE;
    RoomSys(LOBBY);
    putRoom(LOBBY);

    /* Mail> is also permanent...       */
    thisRoom    = MAILROOM;
    strcpy(roomBuf.rbname, "Mail");
    RoomSys(MAILROOM);
    putRoom(MAILROOM);

    /* Aide> also...    */
    thisRoom    = AIDEROOM;
    strcpy(roomBuf.rbname, "Aide");
    roomBuf.rbflags.PUBLIC = FALSE;
    RoomSys(AIDEROOM);
    putRoom(AIDEROOM);
}
