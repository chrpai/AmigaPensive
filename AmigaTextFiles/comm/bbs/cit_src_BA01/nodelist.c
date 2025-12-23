/*
 *                              nodelist.c
 *
 * Generates list of rooms shared with each node.
 */

/*
 *                              history
 *
 * 89Aug14 HAW  Handle virtual rooms as well.
 * 87Dec06 HAW  Created.
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

int CheckForShared(void);

/*
 *                              contents
 *
 *      crashout()              irrecoverable error
 *      main()                  Main controller for this program
 *      openFile()              opens a .sys file
 */

extern rTable *roomTab;         /* RAM index of rooms           */
extern CONFIG cfg;              /* A buncha variables           */
extern FILE     *netfl;
extern NetBuffer netBuf;
extern NetTable *netTab;
extern VirtualRoom *VRoomTab;
extern char VirtualInUse;
extern VirtNet  *VirtNetList;
extern int  thisNet, VirtSize, VNetSize;
FILE *netLog;

int  ListNodes(int alive);
int  ListByNodes(void);
void CheckNet(int i, int roomSlot);
void CheckVirt(int TheVirt, int NetRover);
void PrintNetRooms(void);
void PrintVirtuals(void);

/*
 * crashout()
 *
 * This function handles an irrecoverable error.
 */
void crashout(str)
char *str;
{
    exit(printf(str));
}

/*
 * main()
 *
 * This is the main controller.
 */
int main(int, char **);
int main(argc, argv)
int argc;
char **argv;
{
    SYS_FILE netFile;
    int Index, NetRover;

    cfg.weAre = UTILITY;
    fprintf(stderr, "Citadel Node versus Room Cross Reference list %s\n%s\n"
    , VERSION_NAME, COPYRIGHT);
    if (readSysTab(FALSE, TRUE)) {
        mvToHomeDisk(&cfg.homeArea);
        makeSysName(netFile, "ctdlnet.sys", &cfg.netArea);
        openFile(netFile, &netfl);
        initNetBuf(&netBuf);

        if (argc != 1) {
            if (strCmp(argv[1], "-n") == SAMESTRING)
                return ListNodes(TRUE);
            if (strCmp(argv[1], "-nd") == SAMESTRING)
                return ListNodes(FALSE);
            if (strCmp(argv[1], "-s") == SAMESTRING)
                return ListByNodes();
        }

        printf("%s\n", cfg.codeBuf + cfg.nodeName);
        for (Index = AIDEROOM + 1;              /* First 3 never shared */
            Index < MAXROOMS; Index++) {
            if (  roomTab[Index].rtflags.INUSE &&
                        roomTab[Index].rtflags.SHARED) {
                printf("%s\n", roomTab[Index].rtname);
                for (NetRover = 0; NetRover < cfg.netSize; NetRover++) {
                    CheckNet(NetRover, Index);
                }
                printf("\n");
            }
        }
        VirtInit();
        if (VirtualInUse) {
            for (Index = 0; Index < VirtSize; Index++) {
                if (strLen(VRoomTab[Index].vrName) != 0) {
                    printf("%s (Virtual)\n", VRoomTab[Index].vrName);
                    for (NetRover = 0; NetRover < cfg.netSize; NetRover++) {
                        CheckVirt(Index, NetRover);
                    }
                    printf("\n");
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
                printf("        %s", netBuf.netName);
                switch (CGetMode(VirtNetList[NetRover].VirtList[i].mode)) {
                    case PEON: printf(" (Peon)\n"); break;
                    case ACTIVE_BACKBONE:
                        printf(" (Backbone - Active)\n"); break;
                    case PASS_BACKBONE:
                        printf(" (Backbone - Passive)\n"); break;
                }
            }
        }
    }
}

/*
 * CheckNet()
 *
 * This checks to see if the given room is shared with this node and does
 * the requisite print.
 */
void CheckNet(i, roomSlot)
int i, roomSlot;
{
    int j;

    if (netTab[i].ntflags.in_use) {
        for (j = 0; j < SHARED_ROOMS; j++) {
            if (isSharedRoom(i, j)) {
                if (netTabRoomSlot(i, j) == roomSlot &&
                        roomTab[roomSlot].rtgen == netGen(i, j)) {
                    getNet(i, &netBuf);
                    printf("        %s", netBuf.netName);
                    if (roomTab[netRoomSlot(j)].rtShareType == PEON)
                        printf(" (Peon)\n");
                    else switch (CGetMode(netTab[i].netTRooms[j].mode)) {
                    case PEON:
                        printf(" (Peon)\n");    break;
                    case ACTIVE_BACKBONE:
                        printf(" (Active Backbone)\n"); break;
                    case PASS_BACKBONE:
                        printf(" (Passive Backbone)\n");        break;
                    }
                    return;
                }
            }
        }
    }
}

/*
 * ListNodes()
 *
 * This just prints the list of nodes.  If alive is set, then only print those
 * nodes not disabled.
 */
int ListNodes(alive)
int alive;
{
    int i;
    static char *bauds[] = {
    "300", "1200", "2400", "4800", "9600", "14400", "19200", "38400", "57600"
    };

    for (i = 0; i < cfg.netSize; i++) {
        getNet(i, &netBuf);
        if (netBuf.nbflags.in_use &&
                (!alive || (netBuf.MemberNets & ALL_NETS))) {
            printf("%3d. %-21s%-21s%-11s%s\n", i, netBuf.netName, netBuf.netId,
                (netBuf.nbflags.local) ? "local" : "non-local",
                bauds[netBuf.baudCode]);
        }
    }
  return 0;
}

/*
 * ListByNodes()
 *
 * This prints out nodes and the rooms shared.
 */
int ListByNodes()
{
    int i;

    VirtInit();
    for (i = 0; i < cfg.netSize; i++) {
        getNet(i, &netBuf);
        if (netBuf.nbflags.in_use) {
            if (CheckForShared()) {
                printf("%s\n", netBuf.netName);
                PrintNetRooms();
                PrintVirtuals();
                printf("\n");
            }
        }
    }

    return 0;
}

/*
 * CheckForShared()
 *
 * This decides if this node has any shared rooms at all.
 */
int CheckForShared()
{
    int rover, x;

    for (rover = 0; rover < SHARED_ROOMS; rover++)
        if (isSharedRoom(thisNet, rover) && roomValidate(thisNet, rover))
            return TRUE;

    if (!VirtualInUse) return FALSE;

    for (rover = 0; rover < VIRT_LIMIT; rover++) {
        x = VirtNetList[thisNet].VirtList[rover].WhichVirt;
        if (x != -1 && (x >= VirtSize || x < 0 || !VRoomInuse(x)))
            VirtNetList[thisNet].VirtList[rover].WhichVirt = -1;

        if ((x = VirtNetList[thisNet].VirtList[rover].WhichVirt) != -1)
            return TRUE;
    }
    return FALSE;
}

/*
 * PrintNetRooms()
 *
 * This prints out all the net rooms for the current node.
 */
void PrintNetRooms()
{
    int rover;

    for (rover = 0; rover < SHARED_ROOMS; rover++)
        if (isSharedRoom(thisNet, rover) && roomValidate(thisNet, rover))
            printf("     %s\n", roomTab[netTabRoomSlot(thisNet, rover)].rtname);
}

/*
 * PrintVirtuals()
 *
 * This prints out all the virtual rooms for the current node.
 */
void PrintVirtuals()
{
    int rover, x;

    if (!VirtualInUse) return ;

    for (rover = 0; rover < VIRT_LIMIT; rover++) {
        x = VirtNetList[thisNet].VirtList[rover].WhichVirt;
        if (x != -1 && (x >= VirtSize || x < 0 || !VRoomInuse(x)))
            VirtNetList[thisNet].VirtList[rover].WhichVirt = -1;

        if ((x = VirtNetList[thisNet].VirtList[rover].WhichVirt) != -1)
            printf("     %s\n", VRoomTab[x].vrName);
    }
}

