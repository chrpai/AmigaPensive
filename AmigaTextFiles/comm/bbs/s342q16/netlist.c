/*
 * Generate the NetLink Listing of Shared Rooms.
 */

#include "ctdl.h"       /*
                         * header file
                         */
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
 *      main()                  Main controller for this program
 *      openFile()              opens a .sys file
 */

extern rTable *roomTab;           /*

                                   * RAM index of rooms
                                   */
extern CONFIG cfg;                /*

                                   * A buncha variables
                                   */
extern FILE *netfl;
extern NetBuffer netBuf;
extern NetTable *netTab;
extern VirtualRoom *VRoomTab;
extern char VirtualInUse;
extern VirtNet *VirtNetList;
extern int thisNet,
          VirtSize,
          VNetSize;
FILE     *netLog;

void CheckNet(int i, int roomSlot);
void CheckVirt(int TheVirt, int NetRover);

/*
 * crashout()
 *
 * This function handles an irrecoverable error.
 */
void
crashout(str)
  char     *str;
{
  exit(printf(str));
}

/*
 * main()
 *
 * This is the main controller.
 */
int       main(int, char **);
int
main(argc, argv)
  int       argc;
  char    **argv;
{
  SYS_FILE  netFile;
  int       Index, NetRover;
  cfg.weAre = UTILITY;
  if (readSysTab(FALSE, TRUE))
    {
      mvToHomeDisk(&cfg.homeArea);
      makeSysName(netFile, "ctdlnet.sys", &cfg.netArea);
      openFile(netFile, &netfl);
      initNetBuf(&netBuf);
      printf("#!netlinks %s %s\n", cfg.codeBuf + cfg.nodeName, cfg.codeBuf + cfg.nodeId);
      for (Index = AIDEROOM + 1;        /*
                                         * First 3 never shared
                                         */
           Index < MAXROOMS; Index++)
        {
          if (roomTab[Index].rtflags.INUSE &&
              roomTab[Index].rtflags.SHARED)
            {
              printf(" room=%20s\n", roomTab[Index].rtname);
              for( NetRover = 0; NetRover < cfg.netSize; NetRover++)
                {
                CheckNet(NetRover, Index);
                };
            };
        };
    };
  VirtInit();
  if (VirtualInUse)
    {
      for (Index = 0; Index < VirtSize; Index++)
        {
          if (strLen(VRoomTab[Index].vrName) != 0)
            {
              printf(" room=%20s", VRoomTab[Index].vrName);
              for (NetRover = 0; NetRover < cfg.netSize; NetRover++)
                 CheckVirt(Index, NetRover);
            };
        };
    };
  printf("end_data\n");
}

/*
 * CheckNet()
 *
 * This checks to see if the given room is shared with this node and does
 * the requisite print.
 */
void CheckNet(int i, int roomSlot)
{
    int j;

    if (netTab[i].ntflags.in_use)
      {
      for (j = 0; j < SHARED_ROOMS; j++)
        {
        if (isSharedRoom(i, j))
          {
          if (netTabRoomSlot(i, j) == roomSlot &&
              roomTab[roomSlot].rtgen == netGen(i, j))
            {
            getNet(i, &netBuf);
            printf(" share=%20s %s\n", netBuf.netName, netBuf.netId);
            return;
            }
          }
        }
      }
}

/*
 * CheckVirt()
 *
 * This processes a virtual room.
 */
void CheckVirt(int TheVirt, int NetRover)
{
    int i;
    for (i = 0; i < VIRT_LIMIT; i++) {
        if (VirtNetList[NetRover].VirtList[i].WhichVirt == TheVirt) {
            getNet(NetRover, &netBuf);
            if (netBuf.nbflags.in_use) {
                printf(" share=%s %s\n", netBuf.netName, netBuf.netId);
            }
        }
    }
}
