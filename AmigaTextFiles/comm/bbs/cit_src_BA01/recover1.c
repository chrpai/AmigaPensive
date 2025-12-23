/*
 *                              recover1.c
 *
 * Recovery program for recovering killed rooms.  This only works for room
 * slots which have only been killed; if the slot has been recycled (reclaimed
 * by makeRoom), a different program will be needed for recovery.
 */

/*
 *                              history
 *
 * 87Oct11 HAW  V3 update.
 * 85Nov16 HAW  Modified for MS-DOS libraries.
 * 85Apr10 HAW  Converted to MS-DOS.
 * 84Jun17 HAW  Seems to work correctly.
 * 84Jun17 HAW  Created.
 */

#include "ctdl.h"    /* header file  */
#include "fctype.h"
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
 *                              contents
 *
 *      crashout()              irrecoverable error
 *      main()                  main controller for this program
 *      noteRoom()              enter room into RAM index
 *      openFile()              opens a .sys file
 */

extern rTable *roomTab; /* RAM index of rooms   */
extern aRoom  roomBuf;  /* room buffer  */
extern FILE   *roomfl;  /* file descriptor for rooms    */
extern int    thisRoom; /* room currently in roomBuf    */
extern CONFIG cfg;      /* A buncha variables           */

FILE *fopen();
int UtilGetch(void);
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
 * main()
 *
 * The main controller.
 */
int main(void);
int main()
{
    SYS_FILE roomFile;
    int Index;
    char c;
    cfg.weAre = UTILITY;
    printf("Citadel Room Recover I %s - %s\n", VERSION_NAME, COPYRIGHT);
    printf("Recovering Killed rooms, Hang on a moment...\n");
    if (readSysTab(TRUE, TRUE)) {
        mvToHomeDisk(&cfg.homeArea);
        if (access(LOCKFILE, 0) != ERROR) {
            printf("Please do not run Recover1 using Outside Commands.\n");
            writeSysTab();
            exit(1);
        }
        makeSysName(roomFile, "ctdlroom.sys", &cfg.roomArea);
        openFile(roomFile, &roomfl);
        initRoomBuf(&roomBuf);

        for (Index = AIDEROOM + 1;      /* First 3 ALWAYS exist */
                Index < MAXROOMS; Index++) {
            if (!(roomTab[Index].rtflags.INUSE) && roomTab[Index].rtname[0]) {
                printf("The room named %s may be recoverable. ",
                                                roomTab[Index].rtname);
                printf("Do you want me to try? ");
    c = UtilGetch();
                if (toupper(c) == 'Y') {
                    getRoom(Index);
                    roomBuf.rbflags.INUSE = TRUE;
                    putRoom(Index);
                    noteRoom();
                }
                putchar('\n');
            }
        }
        printf("\n\nAll finished!  Saving... No need to reconfigure.");
        writeSysTab();
    }
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
