/*
 *				va2.c
 *
 * Virtual Room Administrator for the C-86, part 2.
 */

/*
 *				history
 *
 * 88Mar27 HAW  Created.
 */

#define V_ADMIN 1

#include "ctdl.h"

/*
 *				contents
 *
 */

extern FILE	   *VaLog;
extern VirtualRoom *VRoomTab;
extern int	   VirtSize;
extern VirtNet	   *VirtNetList;


extern MessageBuffer   msgBuf;	/* The -sole- message buffer	*/
extern CONFIG    cfg;		/* Configuration variables	*/
extern aRoom	 roomBuf;	/* Room buffer	*/
extern rTable    *roomTab;
extern NetTable  *netTab;
extern NetBuffer netBuf;
extern FILE	 *netfl, *roomfl, *msgfl;

extern int	 thisRoom;	/* Current room	*/
extern int	 thisNet;	/* Current node in use	*/


/*
 * NorToVirtual()
 *
 * This function moves msgs from msg base to virtual directory.
 */
int NorToVirtual(VirtNo, list)
int VirtNo;
TempData *list;
{
    int  rover, systems, i;
    char *dir, fn[100];
    MSG_NUMBER MsgNumber, val;
    FILE *fd, *safeopen();
    extern char *WRITE_ANY;

	/*
	 * Go through the messages
	 */
    for (rover = 0; rover < MSGSPERRM; rover++) {
	if (ReadMessage(rover)) {
	    if (strCmp(R_SH_MARK, msgBuf.mbaddr) == SAMESTRING ||
		strCmp(NON_LOC_NET, msgBuf.mbaddr) == SAMESTRING) {
		dir = LD_DIR;
		MsgNumber = ++VRoomTab[VirtNo].vrHiLD;
	    }
	    else if (strCmp(LOC_NET, msgBuf.mbaddr) == SAMESTRING) {
		dir = LOCAL_DIR;
		MsgNumber = ++VRoomTab[VirtNo].vrHiLocal;
	    }
	    else dir = NULL;
	    if (dir != NULL) {
		CreateVAName(fn, VirtNo, dir, MsgNumber);
/*	sPrintf(fn, "virtual\\%d\\%s\\%ld", VirtNo, dir, MsgNumber); */
		if ((fd = safeopen(fn, WRITE_ANY)) != NULL) {
	/* fill in local node in origin fields if local message: */
		    if (!msgBuf.mborig[ 0])
			strCpy(msgBuf.mborig,  cfg.nodeId + cfg.codeBuf  );
		    if (!msgBuf.mboname[0])
			strCpy(msgBuf.mboname, cfg.nodeName + cfg.codeBuf);

    /* Convert # to 8-bit Citadel style for compatibility   */
		    if (!msgBuf.mbsrcId[0]) {
			val = atol(msgBuf.mbId);
			sPrintf(msgBuf.mbsrcId, "%ld %ld",
					val & 0xFFFF0000l,val & 0xFFFFl);
		    }

    /* send header fields out: */
		    if (msgBuf.mbauth[ 0]) {
			fwrite("A", 1, 1, fd);
			fwrite(msgBuf.mbauth, strLen(msgBuf.mbauth) + 1, 1, fd);
		    }
		    if (msgBuf.mbdate[ 0]) {
			fwrite("D", 1, 1, fd);
			fwrite(msgBuf.mbdate, strLen(msgBuf.mbdate) + 1, 1, fd);
		    }
		    if (msgBuf.mbtime[ 0]) {
			fwrite("C", 1, 1, fd);
			fwrite(msgBuf.mbtime, strLen(msgBuf.mbtime) + 1, 1, fd);
		    }
		    if (msgBuf.mboname[0]) {
			fwrite("N", 1, 1, fd);
			fwrite(msgBuf.mboname, strLen(msgBuf.mboname)+1, 1, fd);
		    }
		    if (msgBuf.mborig[ 0]) {
			fwrite("O", 1, 1, fd);
			fwrite(msgBuf.mborig, strLen(msgBuf.mborig) + 1, 1, fd);
		    }
		    if (msgBuf.mbroom[ 0]) {
			fwrite("R", 1, 1, fd);
			fwrite(msgBuf.mbroom, strLen(msgBuf.mbroom) + 1, 1, fd);
		    }
		    if (msgBuf.mbsrcId[0]) {
			fwrite("S", 1, 1, fd);
			fwrite(msgBuf.mbsrcId, strLen(msgBuf.mbsrcId)+1, 1, fd);
		    }
		    if (msgBuf.mbto[   0]) {
			fwrite("T", 1, 1, fd);
			fwrite(msgBuf.mbto, strLen(msgBuf.mbto) + 1, 1, fd);
		    }

    /* send message text proper: */
		    for (i = 0; msgBuf.mbtext[i]; i++)
			if (msgBuf.mbtext[i] == '\n')
			    msgBuf.mbtext[i] = '\r';

		    fwrite(msgBuf.mbtext, strLen(msgBuf.mbtext) + 1, 1, fd);
		    fclose(fd);
		}
		else return ERROR;
		for (systems = 0; systems < cfg.netSize; systems++) {
		    if (list[systems].uses) {
			if (list[systems].mode == PEON &&
			    strCmp(dir, LOCAL_DIR) == SAMESTRING)
VirtNetList[systems].VirtList[list[systems].VirtIndex].LocSent = MsgNumber;
			else
			    if (strCmp(dir, LOCAL_DIR) == SAMESTRING &&
			list[systems].WhenNormal >= roomBuf.msg[rover].rbmsgNo)
VirtNetList[systems].VirtList[list[systems].VirtIndex].LocSent = MsgNumber;
			else
			    if (strCmp(dir, LD_DIR) == SAMESTRING &&
			list[systems].WhenNormal >= roomBuf.msg[rover].rbmsgNo)
VirtNetList[systems].VirtList[list[systems].VirtIndex].LDSent = MsgNumber;
		    }
		}
	    }
	}
    }
}

/*
 * SetNtoVList()
 *
 * Beats me.
 */
TempData *SetNtoVList(roomNo, VNo)
int roomNo, VNo;
{
    int rover, rooms, searcher;
    TempData *list;

    list = (TempData *) GetDynamic(cfg.netSize * sizeof *list);

    for (rover = 0; rover < cfg.netSize; rover++) {
	if (netTab[rover].ntflags.in_use) {
	    getNet(rover, &netBuf);
	    for (rooms = 0; rooms < SHARED_ROOMS; rooms++) {
		if (isSharedRoom(rover, rooms)) {
		    if (netRoomSlot(rooms) == roomNo) { /* Match! */
			list[rover].uses = TRUE;
			if (netBuf.netRooms[rooms].mode == REG_HOST)
			    list[rover].mode = PASS_BACKBONE;
			else
			    list[rover].mode = netBuf.netRooms[rooms].mode;

			list[rover].WhenNormal = netBuf.netRooms[rooms].lastMess;
			for (searcher = 0; searcher < VIRT_LIMIT; searcher++)
			    if (VirtNetList[rover].VirtList[searcher].WhichVirt
								== -1) break;
			if (searcher == VIRT_LIMIT) {
			    splitIt("Table for %s is full!\n", netBuf.netName);
			    list[rover].uses = FALSE;
			}
			else {
			    list[rover].VirtIndex = searcher;
			    VirtNetList[rover].VirtList[searcher].WhichVirt=VNo;
			    VirtNetList[rover].VirtList[searcher].mode=
							list[rover].mode;
			    splitIt("%s added.\n", netBuf.netName);
			}
			break;
		    }
		}
	    }
	    if (rooms == SHARED_ROOMS)
		list[rover].uses = FALSE;
	}
	else list[rover].uses = FALSE;
    }
    return list;
}

/*
 * ReadMessage()
 *
 * This function reads a message from the message base into the buffer.
 */
char ReadMessage(x)
int x;
{
    extern struct mBuf mFile1;
    MSG_NUMBER here;

    startAt(msgfl, &mFile1, roomBuf.msg[x].rbmsgLoc, 0);
    do {
	getMessage(getMsgChar, FALSE, TRUE, TRUE);
	here = atol(msgBuf.mbId);
    } while (here != roomBuf.msg[x].rbmsgNo &&
				mFile1.thisSector == roomBuf.msg[x].rbmsgLoc);

    if (here != roomBuf.msg[x].rbmsgNo)
	splitIt("Failure to find msg %ld!\n", roomBuf.msg[x].rbmsgNo);

    return (char)(here == roomBuf.msg[x].rbmsgNo);
}
