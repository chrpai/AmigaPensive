/*
 *                              va.c
 *
 * Virtual Room Administrator for the C-86 bulletin board system.
 */

/*
 *                              history
 *
 * 88Apr19 HAW  1.2: Fix for checking PEONS against local msgs.
 * 88Apr08 HAW  1.1: Fix for not turning off a room's inuse flag.
 * 88Jan19 HAW  Created.
 */

#define V_ADMIN 1

#include "ctdl.h"
#include "math.h"
void VReName(void);
int  mPrintf(char *format, ...) {return 0; }  /* stub to quiet the linker */

/*
 *                              contents
 *
 */

FILE            *VaLog = NULL;
char            *VersionString = "V3.42";
char            RunMode;
VirtualRoom     *VRoomTab = NULL;
int             VirtSize;
VirtNet         *VirtNetList = NULL;

extern MessageBuffer   msgBuf;          /* The -sole- message buffer    */
extern CONFIG    cfg;                   /* Configuration variables      */
extern aRoom     roomBuf;               /* Room buffer                  */
extern rTable    *roomTab;
extern NetTable  *netTab;
extern NetBuffer netBuf;
extern FILE      *netfl, *roomfl;

extern FILE *msgfl, *msgfl2;

extern int       thisRoom;              /* Current room */
extern int       thisNet;               /* Current node in use */

/*
 * main()
 *
 * This is the main manager.
 */
int main(int , char **);
int main(argc, argv)
char **argv;
int  argc;
{
    cfg.weAre = UTILITY;
    if (!readSysTab(TRUE, TRUE)) {
        exit(1);
    }

    if (access(LOCKFILE, 0) != ERROR) {
        printf("Please do not run VirtAdmn using Outside Commands.\n");
        writeSysTab();
        exit(1);
    }

    GenInit();
    InitVirtual();      /* kludge ... ? */
    AnalyzeArguments(argc, argv);
    splitIt("Citadel-86 Virtual Room Administrator %s\n%s\n", VersionString,
        COPYRIGHT);
    splitIt("\nNode is %s\n", cfg.nodeName + cfg.codeBuf);
    switch (RunMode) {
        case BATCH:
                splitIt("Batch mode.\n");
                BatchMode();
                break;
        case NORMAL:
                splitIt("\n\n\n");
                InteractMode();
                break;
        default: splitIt("very bad value for run mode!");
    }
    writeSysTab();
    UpdVirtStuff();             /* kludge ... ? */
return 0;
}

/*
 * GenInit()
 *
 * This does general initialization.
 */
void GenInit()
{
    SYS_FILE fn;

    initNetBuf(&netBuf);
    makeSysName(fn, "ctdlnet.sys", &cfg.netArea);
    openFile(fn, &netfl);

    initRoomBuf(&roomBuf);
    makeSysName(fn, "ctdlroom.sys", &cfg.roomArea);
    openFile(fn, &roomfl);

    makeSysName(fn, "ctdlmsg.sys", &cfg.msgArea);
    openFile(fn, &msgfl);
}

/*
 * InitVirtual()
 *
 * This will read in the virtual stuff, create if necessary.
 */
void InitVirtual()
{
    FILE        *fd, *safeopen();
    SYS_FILE    fn;
    int         i, j;
    long        len;
    extern char *R_W_ANY, *W_R_ANY;

    ChkVirtArea();

    makeVASysName(fn, "ctdlvrm.sys");
    if ((fd = safeopen(fn, R_W_ANY)) == NULL) {
        splitIt("%s missing, creating it.\n", fn);
        if ((fd = safeopen(fn, W_R_ANY)) == NULL) {
            splitIt("Couldn't create the virtual room table!");
            exit(1);
        }
        VirtSize = 0;
    }
    else {
        totalBytes(&len, fd);
        VRoomTab = (VirtualRoom *) GetDynamic((unsigned) len);
        fread(VRoomTab, (int) len, 1, fd);
        VirtSize = (int) ((int) len / sizeof *VRoomTab);
    }
    fclose(fd);

    VirtNetList = (VirtNet *)
                        GetDynamic(cfg.netSize * (sizeof *VirtNetList));
    makeVASysName(fn, "ctdlvnet.sys");
    if ((fd = safeopen(fn, R_W_ANY)) == NULL) {
        splitIt("%s missing, creating it.\n", fn);
        if ((fd = safeopen(fn, W_R_ANY)) == NULL) {
            splitIt("Couldn't create the network's virtual list (%s)!", fn);
            exit(1);
        }
        for (i = 0; i < cfg.netSize; i++) {
            for (j = 0; j < VIRT_LIMIT; j++) {
                VirtNetList[i].VirtList[j].WhichVirt = -1;      /* not in use */
            }
        }
        fwrite(VirtNetList, sizeof *VirtNetList, cfg.netSize, fd);
    }
    else fread(VirtNetList, cfg.netSize * (sizeof *VirtNetList), 1, fd);
    fclose(fd);
}

/*
 * UpdVirtStuff()
 *
 * This will update the disk about the virtual stuff.
 */
void UpdVirtStuff()
{
    FILE *fd, *safeopen();
    SYS_FILE fn;
    extern char *R_W_ANY;

    makeVASysName(fn, "ctdlvrm.sys");
    if ((fd = safeopen(fn, R_W_ANY)) == NULL)
        crashout("ctdlvrm.sys is missing!");

    fwrite(VRoomTab, VirtSize, sizeof *VRoomTab, fd);
    fclose(fd);

    makeVASysName(fn, "ctdlvnet.sys");
    if ((fd = safeopen(fn, R_W_ANY)) == NULL)
        crashout("ctdlvnet.sys is missing!!");

    fwrite(VirtNetList, cfg.netSize, sizeof *VirtNetList,  fd);
    fclose(fd);
}

/*
 * AnalyzeArguments()
 *
 * what are we to do???
 */
void AnalyzeArguments(argc, argv)
int  argc;
char **argv;
{
    int  rover;
    SYS_FILE    fn;
    extern char *APPEND_TEXT;

    RunMode = NORMAL;

    for (rover = 1; rover < argc; rover++) {
        if (strCmpU(argv[rover], "+log") == SAMESTRING) {
            makeSysName(fn, "netlog.sys", &cfg.netArea);
            VaLog = safeopen(fn, APPEND_TEXT);
        }
        else if (strCmpU(argv[rover], "+batch") == SAMESTRING) {
            RunMode = BATCH;
        }
    }
}

/*
 * InteractMode()
 *
 * This lets us talk to da sysop.
 */
void InteractMode()
{
    int answer;

    do {
        splitIt("\n\n%30cVirtual Administrator Menu\n\n", ' ');
        splitIt("A. Display Virtual Rooms.\n");
        splitIt("B. Add a Virtual Room.\n");
        splitIt("C. Modify a Virtual Room.\n");
        splitIt("D. Delete a Virtual Room.\n");
        splitIt("E. Convert a normal room to a virtual room.\n");
        splitIt("F. Rename room.\n");
        splitIt("Z. Exit the Virtual Room Administrator.\n");
        switch ((answer = toUpper(NEUtilGetch()))) {
        case 'A':
                Display();                      break;
        case 'B':
                AddRoom();                      break;
        case 'C':
                Modify();                       break;
        case 'D':
                Delete();                       break;
        case 'E':
                ConNorVa();                     break;
        case 'F':
                VReName();                      break;
        case 'Y':
                VADebug();                      break;
        case 'Z':
                                                break;
        default:
                printf("\007\n\n\n");           break;
        }
    } while (answer != EOF && answer != 'Z');
}

/*
 * AddRoom()
 *
 * This function will add a virtual room.
 */
void AddRoom()
{
    int   rover;
    label NewName;
    void  *realloc();

    if (GetConString("room name", NewName, NAMESIZE)) {
        if (roomExists(NewName) != ERROR) {
            splitIt("A real room by that name already exists.\n");
            return ;
        }
        if (VirtualExists(NewName) != ERROR) {
            splitIt("A virtual room by that name already exists.\n");
            return ;
        }

        rover = Add2Room(NewName);

        GetInputList(rover, "Nodes to be added to this virtual room's list",
                                                AddNodes);
    }
}

/*
 * Add2Room()
 *
 * This does the rest of adding a room.
 */
int Add2Room(label NewName)
{
    int rover;

    for (rover = 0; rover < VirtSize; rover++)
        if (!VRoomInuse(rover)) break;

    if (rover == VirtSize) {
        if (rover == 0)
            VRoomTab = (VirtualRoom *) GetDynamic(sizeof *VRoomTab);
        else
            VRoomTab = realloc(VRoomTab, (rover+1) * sizeof *VRoomTab);
        VirtSize++;
    }

    strCpy(VRoomTab[rover].vrName, NewName);

    VRoomTab[rover].vrHiLocal =
    VRoomTab[rover].vrHiLD    =
    VRoomTab[rover].vrLoLocal =
    VRoomTab[rover].vrLoLD    = 0l;
    SetVirtAreas(rover);

    return rover;
}

/*
 * VirtualExists()
 *
 * This function will check for existence of a virtual room.
 */
int VirtualExists(name)
label name;
{
    int rover;

    for (rover = 0; rover < VirtSize; rover++)
        if (strCmpU(VRoomTab[rover].vrName, name) == SAMESTRING) return rover;

    return ERROR;
}

/*
 * Delete()
 *
 * This function will delete a room from the virtual room table.
 */
void Delete()
{
    label NewName;
    int   slot;

    if (GetConString("room name", NewName, NAMESIZE)) {
        if ((slot = VirtualExists(NewName)) != ERROR)
            if (getYesNo("confirm")) {
                VRoomKill(slot);
                KillVirtAreas(slot);
                ResetNodes(slot);
            }
    }
}

/*
 * ResetNodes()
 *
 * This function negates nodes attachments to a dead room.
 */
void ResetNodes(slot)
int slot;
{
    int rover, j;

    for (rover = 0; rover < cfg.netSize; rover++) {
        for (j = 0; j < VIRT_LIMIT; j++) {
            if (VirtNetList[rover].VirtList[j].WhichVirt == slot)
                VirtNetList[rover].VirtList[j].WhichVirt = -1;
        }
    }
}

/*
 * GetConString()
 *
 * This function will get info from console.
 */
char GetConString(prompt, buffer, length)
char *prompt, *buffer;
int length;
{
    int count = 0, c;

    if (strLen(prompt) != 0)
        splitIt("Enter %s (ESC to abort)", prompt);

    splitIt("\n : ");
    while (c = UtilGetch(), c != '\r' && c != '\n' && c != ESC) {
        if ((c != '\b' && count == length) ||
            (c == '\b' && count == 0)) {
            if (c != '\b') splitIt("\b \b\007");
            else           splitIt("\007");
        }
        else if (c != '\b') {
            buffer[count++] = c;
        }
        else {
            splitIt(" \b");
            count--;
        }
    }
    buffer[count] = 0;
    if (c == ESC || strLen(buffer) == 0)
        return FALSE;
    return TRUE;
}

/*
 * getYesNo()
 *
 * This gets a Y/N question of the user.
 */
char getYesNo(question)
char *question;
{
    int c;

    do {
        splitIt("\n%s (Y/N)? ", question);
        c = toUpper(UtilGetch());
    } while (c != 'Y' && c != 'N');
    return (char)( (c == 'Y') ? TRUE : FALSE);
}

/*
 * Modify()
 *
 * This function will add and kill nodes from a sharing list.
 */
void Modify()
{
    label NewName;
    int   slot, AddNodes(), RemoveNodes();

    if (GetConString("room name", NewName, NAMESIZE)) {
        if ((slot = VirtualExists(NewName)) == ERROR) {
            splitIt("'%s' does not exist.\n", NewName);
            return ;
        }
        GetInputList(slot, "Nodes to be added to this virtual room's list",
                                                AddNodes);
        GetInputList(slot, "Nodes to be taken off this virtual room's list",
                                                RemoveNodes);
        GetInputList(slot, "Nodes to be changed", ChgNodes);
    }
}

/*
 * GetInputList()
 *
 * This function will process lists entered by user.
 */
void GetInputList(index, prompt, func)
int index, (*func)(int index, label name);
char *prompt;
{
    label data;

    splitIt(prompt);
    while (GetConString("", data, NAMESIZE))
        if (!(*func)(index, data)) break;
}

/*
 * AddNodes()
 *
 * This adds a node to room's share list.
 */
int AddNodes(index, name)
int index;
label name;
{
    int rover, system, c, found;

    if ((system = searchNameNet(name, &netBuf)) == ERROR) {
        splitIt("No such system known.");
        return TRUE;
    }

    for (rover = 0, found = FALSE; rover < VIRT_LIMIT; rover++)
        if (index == VirtNetList[system].VirtList[rover].WhichVirt) {
            found = TRUE;
            break;
        }

    if (!found)
        for (rover = 0; rover < VIRT_LIMIT; rover++)
            if (VirtNetList[system].VirtList[rover].WhichVirt == -1) {
                break;
            }

    if (rover == VIRT_LIMIT) {
        splitIt("Sorry, no room.");
        return TRUE;
    }

    if (!getYesNo("Is this system a Peon")) {
        do {
            splitIt("\nWill we be a <P>assive or <A>ctive backbone? ");
            c = toUpper(UtilGetch());
        } while (c != 'A' && c != 'P');
        VirtNetList[system].VirtList[rover].mode =
        (c == 'A') ? ACTIVE_BACKBONE : PASS_BACKBONE;
    }
    else
        VirtNetList[system].VirtList[rover].mode = PEON;

    VirtNetList[system].VirtList[rover].WhichVirt = index;

    if (!found) {
        VirtNetList[system].VirtList[rover].LocSent   =
           VRoomTab[index].vrLoLocal;
        VirtNetList[system].VirtList[rover].LDSent    =
           VRoomTab[index].vrLoLD;
    }

    return TRUE;
}

/*
 * RemoveNodes()
 *
 * This function will remove a node from a room's share list.
 */
int RemoveNodes(index, name)
int index;
label name;
{
    int system, rover;

    if ((system = searchNameNet(name, &netBuf)) == ERROR) {
        splitIt("No such system known.\n");
        return TRUE;
    }
    for (rover = 0; rover < VIRT_LIMIT; rover++)
        if (index == VirtNetList[system].VirtList[rover].WhichVirt) {
            VirtNetList[system].VirtList[rover].WhichVirt = -1;
            return TRUE;
        }
    return TRUE;
}

/*
 * ChgNodes()
 *
 * This function will change a node's status.
 */
int ChgNodes(index, name)
int index;
label name;
{
    int system, rover;

    if ((system = searchNameNet(name, &netBuf)) == ERROR) {
        splitIt("No such system known.\n");
        return TRUE;
    }
    for (rover = 0; rover < VIRT_LIMIT; rover++)
        if (index == VirtNetList[system].VirtList[rover].WhichVirt) {
            splitIt("Currently, ");
            switch (VirtNetList[system].VirtList[rover].mode) {
            case PEON:
                splitIt("you think of %s as a Peon.", name); break;
            case ACTIVE_BACKBONE:
                splitIt("you are an Active Backbone for %s.", name); break;
            case PASS_BACKBONE:
                splitIt("you are a Passive Backbone for %s.", name); break;
            }

            splitIt("\nDo you wish to change the relationship to\n");
            splitIt("1. Peon\n2. You are Active Backbone\n");
            splitIt("3. Passive Backbone.\n? ");

            switch (NEUtilGetch()) {
            case '1':
            VirtNetList[system].VirtList[rover].mode = PEON; break;
            case '2':
            VirtNetList[system].VirtList[rover].mode = ACTIVE_BACKBONE; break;
            case '3':
            VirtNetList[system].VirtList[rover].mode = PASS_BACKBONE; break;
            }

            return TRUE;
        }
    return TRUE;
}

/*
 * crashout()
 *
 * This handles fatal errors.
 */
void crashout(str)
char *str;
{
    printf(str);
    writeSysTab();
    UpdVirtStuff();
    exit(1);
}

/*
 * BatchMode()
 *
 * This handles batch mode calls.
 */
void BatchMode()
{
    struct Table_Summary {
        MSG_NUMBER LoLocal, LoLD;
    } *Summary;
    int rover, checker, x;
    MSG_NUMBER msgrover;
    char buf[100];

    if (VirtSize == 0) return;

    Summary = (struct Table_Summary *) GetDynamic(VirtSize * sizeof *Summary);
/*
 * For each virtual room in use:
 *
 * 1. Determine which PEON-generated messages have been delivered to
 *    all BACKBONES and kill those messages.
 * 2. Update appropriate values.
 * 3. Determine which BACKBONE-generated messages have been delivered to
 *    other BACKBONES and PEONS, and kill them.
 * 4. Update appropriate values.
 *
 *   The code that accepts and places messages in the correct places resides
 * in the main source, not here.  Briefly:
 *
 * a. Each Virtual Room has a directory within the VIRTUAL\ subdirectory
 *    dedicated to it.  Mapping of Room to directory is to use the slot # of
 *    the Virtual room as a string for the directory name.  This will limit the
 *    system to, at best, 11 digits.  Even if the limit is 8 digits, however,
 *    that should be more than adequate.
 * b. Each Room's subdirectory consists of two more subdirectories, arbitrarily
 *    labeled.  Messages which arrive from BACKBONE systems
 *    are placed in the "ld" directory; messages from PEONS are placed in the
 *    "local" directory.
 * c. Message structure: Each message that is accepted from another system is
 *    NOT kept in a separate file.  Rather, each packet of messages is kept
 *    in a file.  This should work just as well, and save on clutter.
 * d. Each file kept in either directory will be given a name consisting of an
 *    integer generated by incrementing a counter associated with that
 *    directory.  Counters are independent of each other.
 * e. The Virtual net list is used to track who needs what, using these
 *    counters.  When messages are sent to a system, the number of the last
 *    packet sent is recorded.  Possible problem: a BACKBONE calling -- how
 *    do we keep from looping the messages back to the caller on the LD room
 *    share?  OK, looks like we'll have to put it in a temp file, and rename
 *    it later.  No, we can just remember to increment the number later, and
 *    just not resend it now.
 */
    for (rover = 0; rover < VirtSize; rover++)
        Summary[rover].LoLocal = Summary[rover].LoLD = 0xffffffffl;

    for (rover = 0; rover < cfg.netSize; rover++) {
        if (netTab[rover].ntflags.in_use)
        for (checker = 0; checker < VIRT_LIMIT; checker++) {
            if ((x = VirtNetList[rover].VirtList[checker].WhichVirt) >= 0 &&
                                x < VirtSize) {
/* splitIt("x is %d\n", x); */
/* if (x > VirtSize) splitIt("BINGO!\n"); */
                if (VirtNetList[rover].VirtList[checker].mode != PEON)
                    Summary[x].LoLocal =
      min(Summary[x].LoLocal, VirtNetList[rover].VirtList[checker].LocSent);
                Summary[x].LoLD =
      min(Summary[x].LoLD, VirtNetList[rover].VirtList[checker].LDSent);
            }
            else if (x >= VirtSize) {
                splitIt("x is high for system %d, correcting.\n", rover);
                VirtNetList[rover].VirtList[checker].WhichVirt = -1;
            }
        }
    }

    for (rover = 0; rover < VirtSize; rover++) {
        if (VRoomInuse(rover)) {

            if (Summary[rover].LoLocal == 0xffffffffl)
                Summary[rover].LoLocal = 0l;
            if (Summary[rover].LoLD == 0xffffffffl)
                Summary[rover].LoLD = 0l;

            for (msgrover = max(VRoomTab[rover].vrLoLocal, 1l);
                 msgrover <= Summary[rover].LoLocal; msgrover++) {
                CreateVAName(buf, rover, LOCAL_DIR, msgrover);
                splitIt("Unlinking %s\n", buf);
                unlink(buf);
            }
            VRoomTab[rover].vrLoLocal = msgrover;
            for (msgrover = max(VRoomTab[rover].vrLoLD, 1l);
                 msgrover <= Summary[rover].LoLD; msgrover++) {
                CreateVAName(buf, rover, LD_DIR, msgrover);
                splitIt("Unlinking %s\n", buf);
                unlink(buf);
            }
            VRoomTab[rover].vrLoLD = msgrover;
        }
    }

    free(Summary);
}

/*
 * roomExists()
 *
 * This will check for existence of a real room.
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

/*
 * Display()
 *
 * This will give a crude display of virtual rooms and nodes attached.
 */
void Display()
{
    int rover, i, len, netRover;
    char first;

    splitIt("\n\n\nVirtual Room Display\n\n");
    for (rover = 0; rover < VirtSize; rover++) {
        if (VRoomInuse(rover)) {
            splitIt("%s> ", VRoomTab[rover].vrName);
            for (i = strLen(VRoomTab[rover].vrName); i < 22; i++)
                splitIt(".");
            splitIt(" ");
            len = 44;   /* crude formatter */
            first = TRUE;
            for (netRover = 0; netRover < cfg.netSize; netRover++) {
                if (DoesShare(netRover, rover) != ERROR) {
                    getNet(netRover, &netBuf);
                    if (!first)
                        splitIt(", ");
                    else first = FALSE;
                    if (2 + len + strLen(netBuf.netName) > 80) {
                        splitIt("\n%24c", ' ');
                        len = 23;
                    }
                    splitIt("%s", netBuf.netName);
                    len += (2 + strLen(netBuf.netName));
                }
            }
            splitIt("\n");
        }
    }
}

/*
 * DoesShare()
 *
 * This function will find out if the given node shares the given room.
 */
int DoesShare(NodeNo, RoomNo)
int NodeNo, RoomNo;
{
    int i;

    if (!netTab[NodeNo].ntflags.in_use) return ERROR;

    for (i = 0; i < VIRT_LIMIT; i++) {
        if (VirtNetList[NodeNo].VirtList[i].WhichVirt == RoomNo)
            return i;
    }
    return ERROR;
}

void VADebug()
{
    int i, j, x;

    for (i = 0; i < cfg.netSize; i++) {
        for (j = 0; j < VIRT_LIMIT; j++) {
            if ((x=VirtNetList[i].VirtList[j].WhichVirt) != -1) {
                if (VRoomInuse(x)) {
                    getNet(i, &netBuf);
                    printf("%s for %s: ", netBuf.netName,
                                VRoomTab[x].vrName);
                    printf("LocSent=%ld, LocHi=%ld, ",
                              VirtNetList[i].VirtList[j].LocSent,
                              VRoomTab[x].vrHiLocal);
                    printf("LDSent=%ld, LDHI=%ld\n",
                              VirtNetList[i].VirtList[j].LDSent,
                              VRoomTab[x].vrHiLD);
                }
            }
        }
    }
}

/*
 * ConNorVa()
 *
 * This function converts a normal room to a virtual
 * 1. Make new virtual room.
 * 2. Convert msg base messages to files, one per msg.  Route to correct
 *    directories
 * 3. Update VirtNetList.
 * 4. Delete old room.
 */
void ConNorVa()
{
    int   rover, roomNo;
    label Name;
    void  *realloc();
    TempData *List;

    if (GetConString("Normal room name", Name, NAMESIZE)) {
        if ((roomNo = roomExists(Name)) == ERROR) {
            splitIt("There is no room by that name.\n");
            return ;
        }

        if (!getYesNo("confirm")) return ;

        rover = Add2Room(Name);

        getRoom(roomNo);

        if (roomBuf.rbShareType == PEON)
            splitIt("Warning: %s is not being backboned at the present time.\n",
                                roomBuf.rbname);

        List = SetNtoVList(roomNo, rover);

        NorToVirtual(rover, List);

        roomBuf.rbflags.INUSE = FALSE;

        putRoom(roomNo);

        roomTab[roomNo].rtflags.INUSE = FALSE;

        writeSysTab();
    }
}

/*
 * VRename
 *
 * This function renames a virtual room.
 */
void VReName()
{
    label NewName, OldName;
    int slot;

    if (!GetConString("old room name", OldName, NAMESIZE)) return;

    if ((slot = VirtualExists(OldName)) != ERROR) {
        if (GetConString("new room name", NewName, NAMESIZE)) {
            if (roomExists(NewName) != ERROR) {
                splitIt("A real room by that name already exists.\n");
                return ;
            }
            if (VirtualExists(NewName) != ERROR) {
                splitIt("A virtual room by that name already exists.\n");
                return ;
            }
            strCpy(VRoomTab[slot].vrName, NewName);
        }
    }
    else splitIt("No such virtual room.\n");
}

#ifdef NEEDED
void LogSetup()
{
}

FILE *lfd = NULL;
extern LogTable    *logTab;
LogTable    *delogTab;
void LogCheck(char *x)
{
    int i;

    if (lfd != NULL) return;
    if (delogTab == NULL) {
        delogTab = (LogTable *) GetDynamic(sizeof (*logTab) * cfg.MAXLOGTAB);
        memcpy(delogTab, logTab, (long)sizeof (*logTab) * cfg.MAXLOGTAB);
    }
    for (i = 0; i < cfg.MAXLOGTAB; i++)
        if (logTab[i].ltlogSlot >= cfg.MAXLOGTAB ||
            logTab[i].ltlogSlot < 0 ||
            logTab[i].ltpwhash != delogTab[i].ltpwhash ||
            logTab[i].ltnmhash != delogTab[i].ltnmhash) {
            if ((lfd = fopen("debug", "a")) != NULL) {
                fprintf(lfd, "Blargh at -%s- in VIRTADMN for index %d!\n",
                                        x, i);
                fclose(lfd);
            }
            else printf("Blargh at -%s-!\n", x);
            break;
        }
}

void splitF(FILE *log, char *fmt, ...)
{
}
#endif
