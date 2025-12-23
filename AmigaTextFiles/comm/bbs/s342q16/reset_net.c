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
extern rTable  *roomTab;        /* RAM index of rooms */
extern aRoom   roomBuf;         /* room buffer */
extern FILE    *roomfl;         /* file descriptor for rooms    */
extern int     thisRoom;        /* room currently in roomBuf    */
extern NetTable  *netTab;       /* RAM index of nodes           */
extern FILE    *netfl;          /* file ctdlnet.sys */
extern NetBuffer   netBuf;      /* buffer for ctdlnet.sys */


char init(void);
int  mPrintf(char *format, ...) {return 0; }  /* stub to quiet the linker */

short fflag = FALSE;   /* TRUE = update net info */

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
 * init()
 *
 * This will set up the files for recovery.
 */
char init()
{
    SYS_FILE temp;

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

    /* open the net data file */
    makeSysName(temp, "ctdlnet.sys", &cfg.netArea);
    openFile(temp, &netfl);
    initNetBuf(&netBuf);

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
  int node, slot;
    if( argc > 1 )
      {
      if( strcmp(argv[1], "reset") == 0)fflag = TRUE;
      };
    printf("Reset Netted rooms V3.42\n%s\n", COPYRIGHT);
    if ( init() )
      {
      for(node = 0; node < cfg.netSize; node++)
        {
        if( !netTab[node].ntflags.in_use )continue;
        printf("\nProcessing :%c%c%c[%d]",
        (isprint(netTab[node].ntShort[0]) ? netTab[node].ntShort[0] : '*'),
        (isprint(netTab[node].ntShort[1]) ? netTab[node].ntShort[1] : '*'),
        (isprint(netTab[node].ntShort[2]) ? netTab[node].ntShort[2] : '*'),node);

        getNet(node,&netBuf);

        for( slot=0; slot < SHARED_ROOMS; slot++)
          {
          if( !roomTab[netTabRoomSlot(node, slot)].rtflags.SHARED)continue;
          printf("\n...Room:%s(%d) last:%ld NetTab.lastMess:%ld netBuf.lastMess:%ld",
          roomTab[netTabRoomSlot(node, slot)].rtname, slot,
          roomTab[netTabRoomSlot(node, slot)].rtlastMessage,
          netTab[node].netTRooms[slot].lastMess,
          netBuf.netRooms[slot].lastMess);

          if( fflag )
            {
            netTab[node].netTRooms[slot].lastMess = 0;
            netBuf.netRooms[slot].lastMess = 0;
            };
          };

        if( fflag )putNet(node,&netBuf);
        };

      };
    if( fflag ) writeSysTab();
    printf("\n...finished\n");
    return 0;
}

