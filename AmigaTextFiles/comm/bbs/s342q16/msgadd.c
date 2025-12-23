/*
 *                              msgadd.c
 *
 * External Message handler.  For use with external OtherNet parsers.
 */

/*
 *                              history
 *
 * 96Sep01 AFP  V1.5 -- support vortex checking and archiving by month & year
 * 91Mar31 HAW  v1.4 -- support for virtual rooms.
 * 90Aug13 HAW  v1.3 -- support for room archiving.
 * 89Oct23 HAW  v1.2 -- support for incoming route mail.
 * 88Nov05 HAW  Created.
 */
#include <stdio.h>
#include <string.h>
#include "ctdl.h"
#include "math.h"
/*
 *                              contents
 *
 */

#define TITLE           "C86Net Message Adder(Amiga)"
#define VERSION         "V3.42"

#define NO_ERROR        0
#define BAD_ARGS        1
#define BAD_TABLE       2
#define NO_NODE         3
#define FATAL           4
#define LF_ERROR        5

int crtColumn;
extern CONFIG      cfg;         /* Configuration variables      */
extern MessageBuffer     msgBuf;        /* The -sole- message buffer    */
extern aRoom       roomBuf;     /* Room buffer  */
extern logBuffer   logBuf;
extern FILE     *roomfl, *logfl;
extern int      thisRoom;       /* Current room */
extern rTable      *roomTab;
extern struct mBuf mFile1, mFile2;
extern NetTable    *netTab;
extern NetBuffer   netBuf;
extern NetBuffer   netTemp;
extern FILE     *netfl;
extern LogTable    *logTab;
extern int      thisNet;        /* Current node in use  */
extern char     *APPEND_ANY;
extern char     *APPEND_TEXT;
extern FILE     *msgfl, *msgfl2;
FILE    *GlobalFd;
int     RouteSlot;
extern FILE *upfd;
char inNet = NORMAL_NET;

FILE      *netLog = NULL;
char logNetResults = 1;
char netDebug = 1;


SListBase MailForward = { NULL, NULL, NULL, NULL, NULL };
SListBase  Arch_base = { NULL, ChkNtoStr, NULL, FreeNtoStr, EatNMapStr };
static char EOP = FALSE;
char *R_SH_MARK =  "&&";
char *NON_LOC_NET= "%%";
char *LOC_NET =    "++";

#ifdef ANSI_PROTOTYPING

void GenInit(void);
void Process(char *fn);
int  GetOtherNetChar(void);
int VirtualShared(int NetNo, label name);
int VirtualExists(label name);
void SaveIt(void);
int  FindPerson(label nm, logBuffer *lbuf);
int  FindRoom(label nm);
int  FindNet(label nm, NetBuffer *netBuf);
void AddVirtualField(char field, char *contents);
void PutMessage(void);
void HandleRouted(void);
void AddField(int field, char *fValue);
int  FindPos(void);
void VirtualHandle(void);
int MARecVirtualRoom(int VirtIndex);
void TranslateFilename(char *realfn, char *fn);
void netResult(char *);
#endif

int  mPrintf(char *format, ...) {return 0; }  /* stub to quiet the linker */


/*
 * crashout()
 *
 * Big error handler.
 */
void crashout(str)
char *str;
{
    printf(str);
    exit(FATAL);
}

/*
 * main()
 *
 * This is the main manager.
 */
int  main(int, char **);
int  main(argc, argv)
char **argv;
int  argc;
{
    int rover;

    printf("%s %s\n%s\n\n", TITLE, VERSION, COPYRIGHT);

    /* not enough arguments?  Explain. */
    if (argc < 3) {
        printf("usage: MSGADD nodename file [ file ... ]\n");
        printf("\nEach file may contain one or more messages in C86Net format.\n");
        exit(BAD_ARGS);
    }

    cfg.weAre = UTILITY;
    if (!readSysTab(TRUE, TRUE)) {
        exit(BAD_TABLE);
    }

    if (access(LOCKFILE, 0) != -1) {
        printf("Please do not run MsgAdd using Outside Commands.\n");
        writeSysTab();
        exit(LF_ERROR);
    }

    GenInit();

    VirtInit();

    if (FindNet(argv[1], &netBuf) == ERROR) {
        writeSysTab();
        printf("Could not find node %s.\n", argv[1]);
        exit(NO_NODE);
    }

    for (rover = 2; rover < argc; rover++)
        Process(argv[rover]);

    UpdVirtStuff();
    writeSysTab();
  return 0;
  }

/*
 * GenInit()
 *
 * This is a general initialization routine.
 */
void GenInit()
{
    SYS_FILE fn;

    initNetBuf(&netBuf);
    initNetBuf(&netTemp);
    makeSysName(fn, "ctdlnet.sys", &cfg.netArea);
    openFile(fn, &netfl);

    initRoomBuf(&roomBuf);
    makeSysName(fn, "ctdlroom.sys", &cfg.roomArea);
    openFile(fn, &roomfl);

    InitMsgBase();

    initLogBuf(&logBuf);
    makeSysName(fn, "ctdllog.sys", &cfg.logArea);
    openFile(fn, &logfl);

    makeSysName(fn, "ctdlarch.sys", &cfg.roomArea);
    MakeList(&Arch_base, fn, NULL);
}

/*
 * Process()
 *
 * This will process a file - read a file for all messages and stuff them
 * into the message base.
 */
void Process(fn)
char *fn;
{
    extern char *READ_ANY;

    if ((GlobalFd = fopen(fn, READ_ANY)) == NULL) {
        printf("ERROR: Could not open %s.\n", fn);
        return;
    }

    while (getMessage(GetOtherNetChar, TRUE, TRUE, TRUE))
        SaveIt();

    fclose(GlobalFd);
}

/*
 * GetOtherNetChar()
 *
 * This gets a character for getMessage.
 */
int GetOtherNetChar()
{
    int c;

    c = getc(GlobalFd);
    if (c == EOF) return -1;
    return c;
}

/*
 * SaveIt()
 *
 * This is charged with saving the message in the database.
 *
 * 1. If mail, must do recipient validation.
 * 2. Must validate room.
 */
void SaveIt()
{
    int LogSlot, RoomSlot, place;
    extern char *NON_LOC_NET, *LOC_NET;
    char *fn, *realfn;
    char header[200];

#ifdef VIEWING
    printf("mbauth=-%s-\n", msgBuf.mbauth);
    printf("mbroom=-%s-\n", msgBuf.mbroom);
    printf("mbdate=-%s-\n", msgBuf.mbdate);
    printf("mbtime=-%s-\n", msgBuf.mbtime);
    printf("mboname=-%s-\n", msgBuf.mboname);
    printf("mborig=-%s-\n", msgBuf.mborig);
    printf("mbto=-%s-\n", msgBuf.mbto);
    printf("mbsrcId=-%s-\n", msgBuf.mbsrcId);
    printf("mbtext=-%s-\n", msgBuf.mbtext);
    printf("\n");
#endif

    if (VirtualExists(msgBuf.mbroom) != ERROR) {
        VirtualHandle();
        return ;
    }

    if (strLen(msgBuf.mbaddr) != 0) {
        HandleRouted();
    }
    else if (strCmpU(msgBuf.mbroom, "mail") == SAMESTRING) {
        if ((LogSlot = FindPerson(msgBuf.mbto, &logBuf)) == ERROR) {
            printf("Could not deliver Mail to '%s', does not exist.\n",
                                                        msgBuf.mbto);
            return ;
        }
        noteAMessage(logBuf.lbMail, MAILSLOTS, cfg.newest+1, cfg.catSector);
        putLog(&logBuf, LogSlot);
    }
    else {
        if ((RoomSlot = FindRoom(msgBuf.mbroom)) == ERROR) {
            printf("Message meant for non-existent room '%s' not incorporated.\n",
                                                msgBuf.mbroom);
            return ;
        };
        if( ! NotVortex() )
          {
          printf("Message is a duplicate, not inorporated.\n");
          printf("Author:%20s ", msgBuf.mbauth);
          printf("Room:%s\n", msgBuf.mbroom);
          printf("  date:%20s ", msgBuf.mbdate);
          printf("time:%s\n", msgBuf.mbtime);
          printf(" Oname:%20s ", msgBuf.mboname);
          printf("orig:%s\n", msgBuf.mborig);
          printf("    to:%20s ", msgBuf.mbto);
          printf(" src:%s\n", msgBuf.mbsrcId);
          return;
          };
        noteAMessage(roomBuf.msg, MSGSPERRM, cfg.newest+1, cfg.catSector);
        roomTab[RoomSlot].rtlastMessage = cfg.newest + 1;
        putRoom(RoomSlot);
        if ((place = FindPos()) == ERROR)
            printf("WARNING: msg for %s is not formally shared.\n",
                                                        msgBuf.mbroom);

        if (roomBuf.rbShareType != PEON &&
                                netBuf.netRooms[place].mode != PEON)
            strCpy(msgBuf.mbaddr, NON_LOC_NET);
        else
            strCpy(msgBuf.mbaddr, LOC_NET);

        if (roomBuf.rbflags.ARCHIVE == 1) {
            fn = SearchList(&Arch_base, NtoStrInit(thisRoom, "", 0, TRUE));
            realfn = GetDynamic(strlen(fn) + 15);
            TranslateFilename(realfn,fn);
            if ((upfd = fopen(fn, APPEND_TEXT)) != NULL) {
                header[0] = 0;
                if (msgBuf.mbdate[ 0])
                    sPrintf(lbyte(header), "   %s ", msgBuf.mbdate);
                if (msgBuf.mbtime[ 0] && sendTime)
                    sPrintf(lbyte(header), "%s ", msgBuf.mbtime);
                if (msgBuf.mbauth[ 0]) {
                    sPrintf(lbyte(header), "from %s",    msgBuf.mbauth );
                }
                NormStr(msgBuf.mboname);
                if (msgBuf.mboname[0]) {
                    sPrintf(lbyte(header), " @ %s", msgBuf.mboname);
                    if (msgBuf.mbdomain[0])
                        sPrintf(lbyte(header), cfg.DomainDisplay,
                                                        msgBuf.mbdomain);
                }

                if (msgBuf.mbto[   0]) {
                    sPrintf(lbyte(header), " to %s", msgBuf.mbto);
                }
                fprintf(upfd, "%s\n", header);
                crtColumn = 1;
                mFormat(msgBuf.mbtext);
                fprintf(upfd, "\n");
                fclose(upfd);
            }
        }
    }
    strCpy(msgBuf.mborig, netBuf.netId);
    PutMessage();
    cfg.newest++;

    cfg.catSector   = mFile1.thisSector;
    cfg.catChar     = mFile1.thisChar;
}

SListBase FwdAliasii;   /* keeps some other stuff happy */
char onConsole = FALSE, remoteSysop = FALSE;
int  callSlot = ERROR;
/*
 * HandleRouted()
 *
 * This should handle mail routing incoming.
 */
void HandleRouted()
{
    char     *AltName;
    label    Name, Id, temp;
    SYS_FILE fn;
    extern void (*NetPrintTarget)(char *fmt, ...);
    extern int  (*ToFileWork)();

    /* no find? */
    if ((RouteSlot = FindNet(msgBuf.mbaddr, &netTemp)) == ERROR) {
        return;
    }

    /* yes, found, data in netTemp - will we do the routing? */
    if (!cfg.BoolFlags.RouteMail || !netTemp.nbflags.RouteTo) {
        return;
    }

    /* yes, we'll do the routing.  Now to figure it out. */
    strCpy(Name, netTemp.netName);
    strCpy(Id, netTemp.netId);

    if ((AltName = UseNetAlias(Name, TRUE)) != NULL)
        strCpy(Name, AltName);

    if (FindRouteSlot() == ERROR) {
        return;
    }

        /* Ugly ugly kludge until we figure out how nbHiRouteInd is wrong */
    if (netTemp.nbHiRouteInd < 0) netTemp.nbHiRouteInd = 0;
    sPrintf(temp, "R%d.%d", RouteSlot, netTemp.nbHiRouteInd++);

    makeSysName(fn, temp, &cfg.netArea);

    if ((upfd = safeopen(fn, APPEND_ANY)) == NULL) {
        return;
    }

    fprintf(upfd, "%-20s", Id);
    putc(0, upfd);
    fprintf(upfd, "%-20s", Name);
    putc(0, upfd);

    NetPrintTarget = ToFile;
    StartEncode(putFLChar);
    ToFileWork = Encode;
    prNetStyle(TRUE, Encode, FALSE, "");
    StopEncode();
    fclose(upfd);
    netTemp.nbflags.HasRouted = TRUE;

    putNet(RouteSlot, &netTemp);
}

/*
 * FindPerson()
 *
 * This loads the log record for the named person.
 * RETURNS: ERROR if not found, else log record #
 * (stolen from LOG.C)
 */
int FindPerson(name, lBuf)
char      *name;
logBuffer *lBuf;
{
    int  h, i, foundIt, logNo;

    if (strCmpU(name, "Citadel") != SAMESTRING) {
        h   = hash(name);
        for (foundIt = i = 0;  i < cfg.MAXLOGTAB && !foundIt;  i++) {
            if (logTab[i].ltnmhash == h) {
                getLog(lBuf, logNo = logTab[i].ltlogSlot);
                if (lBuf->lbflags.L_INUSE &&
                                strCmpU(name, lBuf->lbname) == SAMESTRING) {
                    foundIt = TRUE;
                }
            }
        }
    }
    else foundIt = FALSE;
    if (!foundIt)    return ERROR;
    else        return logNo;
}

/*
 * findRoom()
 *
 * This function should find the named room.  Return ERROR if not found.
 */
int FindRoom(nm)
label nm;
{
    int rover;

    for (rover = 0; rover < MAXROOMS; rover++)
        if (strCmpU(roomTab[rover].rtname, nm) == SAMESTRING) {
            getRoom(rover);
            return rover;
        }

    return ERROR;
}

/*
 * noteAMessage()
 *
 * This function notes a message in a message array.  Stolen from MSG.C.
 */
void noteAMessage(base, slots, id, loc)
MSG_NUMBER  id;
SECTOR_ID   loc;
theMessages *base;
int     slots;
{
    int  i;

    /* store into current room: */
    /* slide message pointers down to make room for this one:       */
    for (i = 0;  i < slots - 1;  i++) {
        base[i].rbmsgLoc  = base[i+1].rbmsgLoc;
        base[i].rbmsgNo   = base[i+1].rbmsgNo ;
    }

    /* slot this message in:    */
    base[slots-1].rbmsgNo     = id ;
    base[slots-1].rbmsgLoc    = loc;
}

/*
 * FindNet()
 *
 * This function will find the named node.  Stolen from searchNameNet/NETMISC.
 */
int  FindNet(label nm, NetBuffer *nBuf)
{
    int rover;

    for (rover = 0; rover < cfg.netSize; rover++) {
        if (netTab[rover].ntflags.in_use &&
            hash(nm) == netTab[rover].ntnmhash) {
            getNet(rover, nBuf);
            if (strCmpU(nBuf->netName, nm) == SAMESTRING)
                return rover;
        }
    }
    return ERROR;
}

/*
 * FindPos()
 *
 * This finds the spot in the shared room array for the node that matches up
 * with the current room.
 */
int FindPos()
{
    int rover;

    for (rover = 0; rover < SHARED_ROOMS; rover++)
        if (isSharedRoom(thisNet, rover) &&
                        netRoomSlot(rover) == thisRoom &&
                                netGen(thisNet, rover)  == roomBuf.rbgen)
            return rover;

    return ERROR;
}

/*
 * VirtualHandle()
 *
 * This function will handle a message destined for a virt room.
 */
void VirtualHandle()
{
    int Vindex;

    if ((Vindex = VirtualShared(thisNet, msgBuf.mbroom)) == ERROR) {
        printf("Virtual room %s not shared with %s, message not saved.\n",
                                msgBuf.mbroom, netBuf.netName);
        return;
    }
    MARecVirtualRoom(Vindex);
}

/*********** These functions stolen & modified from VIRT2.C ***************/

extern VirtualRoom *VRoomTab;
extern VirtNet     *VirtNetList;
extern int  VirtSize, VNetSize;
/*
 * MARecVirtualRoom()
 *
 * This function receives a virtual room from another system.
 */
int MARecVirtualRoom(int VirtIndex)
{
    int         VirtNo;
    MSG_NUMBER  rover;
    char        *distance, fn[50];
    extern FILE *upfd;
    extern char *WRITE_ANY;

    VirtNo = VirtNetList[thisNet].VirtList[VirtIndex].WhichVirt;

    if (VirtNetList[thisNet].VirtList[VirtIndex].mode != PEON) {
        distance = LD_DIR;
        rover = VRoomTab[VirtNo].vrHiLD + 1l;
        VRoomTab[VirtNo].vrChanged |= LD_CHANGE;
    }
    else {
        distance = LOCAL_DIR;
        rover = VRoomTab[VirtNo].vrHiLocal + 1l;
        VRoomTab[VirtNo].vrChanged |= LOC_CHANGE;
    }
    CreateVAName(fn, VirtNo, distance, rover);

    if ((upfd = fopen(fn, WRITE_ANY)) != NULL) {
        if (msgBuf.mbauth[0])
            AddVirtualField('A', msgBuf.mbauth);
        if (msgBuf.mbdate[0])
            AddVirtualField('D', msgBuf.mbdate);
        if (msgBuf.mbtime[0])
            AddVirtualField('C', msgBuf.mbtime);
        if (msgBuf.mboname[0])
            AddVirtualField('N', msgBuf.mboname);
        if (msgBuf.mborig[0])
            AddVirtualField('O', msgBuf.mborig);
        if (msgBuf.mbroom[0])
            AddVirtualField('R', msgBuf.mbroom);
        if (msgBuf.mbsrcId[0])
            AddVirtualField('S', msgBuf.mbsrcId);
        if (msgBuf.mbto[0])
            AddVirtualField('T', msgBuf.mbto);
        if (msgBuf.mbOther[0])
            AddVirtualField('P', msgBuf.mbOther);
        if (msgBuf.mbdomain[0])
            AddVirtualField('X', msgBuf.mbdomain);
        if (msgBuf.mbtext[0])
            AddVirtualField('M', msgBuf.mbtext);
        fclose(upfd);
    }
    else
        printf("Unable to open %s!\n", fn);

    VirtSummary();
    return 0;
}

/*
 * AddVirtualField()
 *
 * This function adds a field to a virtual room message.
 */
void AddVirtualField(char field, char *contents)
{
    fprintf(upfd, "%c%s", field, contents);
    fputc(0, upfd);
}

/*
 * VirtualExists()
 *
 * This function returns an index to given room, if exists.
 */
int VirtualExists(label name)
{
    int rover;

    for (rover = 0; rover < VirtSize; rover++)
        if (strCmpU(VRoomTab[rover].vrName, name) == SAMESTRING) return rover;

    return ERROR;
}

/*
 * VirtualShared()
 *
 * This function returns an index into current net's virtual index.
 */
int VirtualShared(int NetNo, label name)
{
    int rover, VirtNo;

    if ((VirtNo = VirtualExists(name)) == ERROR) return ERROR;

    for (rover = 0; rover < VIRT_LIMIT; rover++)
        if (VirtNetList[NetNo].VirtList[rover].WhichVirt == VirtNo)
            return rover;
    return ERROR;
}

/*********** These functions stolen & modified from MSG.C ***************/

/*
 * doActualWrite()
 *
 * This should allow automatic bkp of msg file from RAM.
 */
char doActualWrite(whichmsg, mFile, c)
FILE    *whichmsg;
struct mBuf *mFile;
char    c;
{
    MSG_NUMBER temp;
    int toReturn = 0;

    if (mFile->sectBuf[mFile->thisChar] == 0xFF)  {
        /* obliterating a msg   */
        toReturn = 1;
    }

    mFile->sectBuf[mFile->thisChar]   = c;

    mFile->thisChar    = ++mFile->thisChar % MSG_SECT_SIZE;

    if (mFile->thisChar == 0) { /* time to write sector out and get next: */
        temp = mFile->thisSector;
        temp *= MSG_SECT_SIZE;
        fseek(whichmsg, temp, 0);
        crypte(mFile->sectBuf, MSG_SECT_SIZE, 0);
        if (fwrite(mFile->sectBuf, MSG_SECT_SIZE, 1, whichmsg) != 1) {
            crashout("?putMsgChar-write fail");
        }

        mFile->thisSector = ++mFile->thisSector % cfg.maxMSector;
        temp = mFile->thisSector;
        temp *= MSG_SECT_SIZE;
        fseek(whichmsg, temp, 0);
        if (fread(mFile->sectBuf, MSG_SECT_SIZE, 1, whichmsg) != 1) {
            crashout("?putMsgChar-read fail");
        }
        crypte(mFile->sectBuf, MSG_SECT_SIZE, 0);
    }
    return (char) toReturn;
}

/*
 * doFlush()
 *
 * This will do actual writeup for specified msg file.
 */
void doFlush(whichmsg, mFile)
FILE *whichmsg;
struct mBuf *mFile;
{
    long int s;

    s = mFile->thisSector;
    s *= MSG_SECT_SIZE;
    fseek(whichmsg, s, 0);
    crypte(mFile->sectBuf, MSG_SECT_SIZE, 0);
    if (fwrite(mFile->sectBuf, MSG_SECT_SIZE, 1, whichmsg) != 1) {
        crashout("?ctdlmsg.sys write fail");
    }
    crypte(mFile->sectBuf, MSG_SECT_SIZE, 0);
    fflush(whichmsg);
}

/*
 * flushMsgBuf()
 *
 * This wraps up writing a message to disk, takes into account 2nd msg file if
 * necessary.
 */
void flushMsgBuf()
{
    doFlush(msgfl, &mFile1);
    if (cfg.BoolFlags.mirror)
        doFlush(msgfl2, &mFile2);
}

/*
 * putMessage()
 *
 * This function stores a message to disk.
 * Always called before noteMessage() -- newest not ++ed yet.
 * Returns: TRUE on successful save, else FALSE
 */
void PutMessage()
{
    label temp;
    extern char *ALL_LOCALS, *WRITE_LOCALS;
    extern char *R_SH_MARK, *LOC_NET, *NON_LOC_NET;
    char  *s;

    startAt(msgfl, &mFile1, cfg.catSector, cfg.catChar);
            /* tell putMsgChar where to write   */
    if (cfg.BoolFlags.mirror)
        startAt(msgfl2, &mFile2, cfg.catSector, cfg.catChar);

    putMsgChar(0xFF);   /* start-of-message     */

    /* write message ID */
    sPrintf(temp, "%lu", cfg.newest + 1);
    AddField(0, temp);

    /* write date:      */
    if (msgBuf.mbdate[0]) {
        AddField('D', msgBuf.mbdate);
    }
    else {
        AddField('D', "????");
    }

    /* write time:      */
    if (msgBuf.mbtime[0]) {
        AddField('C', msgBuf.mbtime);
    }
    else {
        AddField('C', "!!!!");
    }

    /* write author's name out: */
    if (msgBuf.mbauth[0]) {
        AddField('A', msgBuf.mbauth);
    }

    /* write room name out:     */
    AddField('R', msgBuf.mbroom);

    if (msgBuf.mbto[0]) {       /* private message -- write addressee   */
        AddField('T', msgBuf.mbto);
    }

    if (msgBuf.mbaddr[0]) {     /* net message routing  */
        if (strCmpU(msgBuf.mbaddr, R_SH_MARK  ) == SAMESTRING ||
                strCmpU(msgBuf.mbaddr, NON_LOC_NET) == SAMESTRING) {
            AddField('N', msgBuf.mboname);
            if (msgBuf.mborig[0])   {
                AddField('O', msgBuf.mborig);
            }
            roomTab[thisRoom].rtlastNet = cfg.newest + 1;
        }
        else if (strCmpU(msgBuf.mbaddr, LOC_NET    ) == SAMESTRING) {
            AddField('N', msgBuf.mboname);
            if (msgBuf.mborig[0])   {
                AddField('O', msgBuf.mborig);
            }
        }

        sPrintf(temp, "%s%d", msgBuf.mbaddr, thisNet);
        AddField('Q', temp);
    }
    else {
        if (msgBuf.mboname[0]) {
            AddField('N', msgBuf.mboname);
        }
        if (msgBuf.mborig[0])   {
            AddField('O', msgBuf.mborig);
        }
    }

    if (msgBuf.mbdomain[0])   {
        AddField('X', msgBuf.mbdomain);
    }

    if (msgBuf.mbsrcId[0])   {
        AddField('S', msgBuf.mbsrcId);
    }

    if (msgBuf.mbOther[0])   {
        AddField('P', msgBuf.mbOther);
    }

    /* write message text by hand because it would overrun AddField buffer: */
    putMsgChar('M');    /* M-for-message.       */
    for (s = msgBuf.mbtext;  *s;  s++) putMsgChar(*s);

    putMsgChar(0);      /* null to end text     */
    flushMsgBuf();

}

/*
 * AddField()
 *
 * This adds a field to the message base.
 */
void AddField(field, fValue)
int field;
char *fValue;
{
    if (field) putMsgChar(field);
    while (*fValue) putMsgChar(*fValue++);
    putMsgChar(0);      /* End field. */
}

/*
 * putMsgChar()
 *
 * This function writes successive message chars to disk.
 * Globals:     thisChar=       thisSector=
 * Returns:     ERROR if problems else TRUE.
 */
int putMsgChar(c)
int c;
{
    int  toReturn;
    int  count1, count2;

    toReturn = TRUE;
    count1 = doActualWrite(msgfl, &mFile1, c);
    if (cfg.BoolFlags.mirror) {
        count2 = doActualWrite(msgfl2, &mFile2, c);
        if (count1 != count2) printf("Mirror msg count discrepancy!");
    }
    if (count1)
        ++cfg.oldest;
    return toReturn;
}

static label SearchResult;
static char  *SearchTarget, GetAlias;
/*
 * UseNetAlias()
 *
 * This function will find a usenet alias or the converse.
 */
char *UseNetAlias(char *Name, char FindAlias)
{
    void *EatTrans();
    SListBase Dummy = { NULL, NULL, NULL, NULL, EatTrans };
    SYS_FILE fn;
    char *c;

    SearchResult[0] = 0;
    SearchTarget = Name;
    if (!FindAlias) while ((c = strchr(Name, ' ')) != NULL) *c = '_';
    makeSysName(fn, "st-alias.sys", &cfg.roomArea);
    GetAlias = FindAlias;
    MakeList(&Dummy, fn, NULL); /* CHEAT!  WHEEEEEE! */
    if (strLen(SearchResult) == 0) return NULL;
    return SearchResult;
}

/*
 * EatTrans()
 *
 * This function will eat a line of for the alias mapping.
 */
void *EatTrans(char *line)
{
    char *c;

    if ((c = strchr(line, ' ')) != NULL) {
        *c = 0;
        if (GetAlias) {         /* check second field */
            if (strCmpU(c + 1, SearchTarget) == SAMESTRING) {
                strCpy(SearchResult, line);
            }
        }
        else {                  /* check first field */
            if (strCmpU(line, SearchTarget) == SAMESTRING) {
                strCpy(SearchResult, c + 1);
            }
        }
    }
    return NULL;
}

/*
 * FindRouteSlot()
 *
 * This function will find the slot of routing node.  If found, netTemp will
 * contain the new slot, as will RouteSlot.
 */
int FindRouteSlot()
{
    /*
     * first check to see if we're the direct (final) link, either
     * expressly or because the route is outdated.
     */
    if (!DirectRoute(&netTemp)) /* expressly?   */
        getNet((RouteSlot = netTemp.nbRoute), &netTemp);

    return RouteSlot;
}

/*
 * DirectRoute()
 *
 * This will discover if we directly or indirectly talk to this node.
 */
char DirectRoute(NetBuffer *n)
{
    return (char) (n->nbRoute == -1 ||  /* expressly?   */
        !netTab[n->nbRoute].ntflags.in_use ||    /* outdated?    */
        netTab[n->nbRoute].ntGen != n->nbRouteGen);/* outdated?*/
}

/*
 * getNetChar()
 *
 * This gets a character from a network temporary file.
 */
int getNetChar()
{
    return -1;          /* actually, this should never be called.       */
}

/*
 * SepNameSystem()
 *
 * This will parse an Other Recipient spec.
 */
char SepNameSystem(char *string, char *person, char *system, NetBuffer *buf)
{
    char *c;
    int  n;
    char work[NAMESIZE * 3];    /* should be sufficient */

    strCpy(work, string);
    if ((c = strchr(work, '@')) == NULL)
        return NOT_SYSTEM;

    /* find leading spaces of "name @ system" format */
    for (n = 1; c[n] == ' ' && c[n]; n++)
        ;

    if (strLen(c + n) >= NAMESIZE)
        return BAD_FORMAT;

    strCpy(system, c + n);
    if (searchNameNet(c + n, buf) == ERROR)  /* bad - set signal of it */
        return NO_SYSTEM;

    *c = 0;
    CleanEnd(work);     /* kill trailing spaces */
    if (strLen(work) >= NAMESIZE)
        return BAD_FORMAT;

    strCpy(person, work);

    return IS_SYSTEM;
}

#define WeServe(x)      SearchList(&Serves, x)
extern SListBase Serves;
/*
 * LocalName()
 *
 * This takes a string of form <system> _ <domain> and attempts to discover if
 * this domain mapped system is actually a local.  This is used when we're
 * sending mail and are trying to find out if a Who Else override needs to be
 * generated.  Ugly kludge, but, hey, that's what programming's all about, eh?
 */
char *LocalName(char *system)
{
    char *domain, *System;

    if ((domain = strchr(system, '_')) == NULL) return system;
    domain += 2;        /* always preceded by a space -- or so we assume */

    if (strCmpU(domain, cfg.codeBuf + cfg.nodeDomain) == SAMESTRING ||
             WeServe(domain) != NULL) {
        System = strdup(system);
        if ((domain = strchr(System, ' ')) == NULL)
            return system;      /* should never happen, though */
        *domain = NULL;
        if (searchNameNet(System, &netTemp) != ERROR) {
            free(System);
            return netTemp.netName;
        }
        free(System);
    }
    return system;
}

/*
 * mFormat()
 *
 * This function formats a string to modem and console.
 */
void mFormat(char *string)
{
    char wordBuf[MAXWORD];
    int  i;

    for (i = 0;  string[i]; ) {
        i = getWord(wordBuf, string, i, MAXWORD);
        putWord(wordBuf);
    }
}

/*
 * getWord()
 *
 * This function fetches one word from current message.
 */
int getWord(char *dest, char *source, int offset, int lim)
{
    int i, j;

    /* skip leading blanks if any */
    for (i = 0;  source[offset+i] ==' ' && i < lim - 1;  i++);

    /* step over word */
    for (;

        source[offset+i]   != ' '     &&
        i       <  lim - 1 &&
        source[offset+i]   != 0;

        i++
    );

    if (source[offset + i - 1] != '\n')
    /* pick up any trailing blanks */
    for (;  source[offset+i]==' ' && i<lim - 1;  i++);

    /* copy word over */
    for (j = 0; j < i; j++)  dest[j] = source[offset+j];
    dest[j] = 0;        /* null to tie off string */

    return(offset+i);
}

/*
 * putWord()
 *
 * This function writes one word to modem & console.
 */
void putWord(char *st)
{
    char *s;
    int  newColumn;
    static char prevChar = 0;

    for (newColumn = crtColumn, s = st;  *s; s++)   {
        if (*s != TAB) {
            if (*s == '\b') newColumn--;
            else ++newColumn;
        }
        else    while (++newColumn % 8);
    }
    if (newColumn > termWidth) {
        fprintf(upfd, "\n");
        crtColumn = 1;
    }

    for (;  *st;  st++) {

        if (*st != TAB) {
            if (*st == '\b') crtColumn--;
            else ++crtColumn;
        }
        else    while (++crtColumn % 8);

        /* worry about words longer than a line:        */
        if (crtColumn > termWidth) {
            fprintf(upfd, "\n");
            crtColumn = 1;
        }

        if (*st == '\n' && EOP) {
            fprintf(upfd, "\n");
            crtColumn = 1;
        }
        else if (prevChar!=NEWLINE  ||  (*st > ' ')) {
            putc(*st, upfd); prevChar = *st;
            if (*st > ' ') EOP = FALSE;
        }
        else {
            fprintf(upfd, "\n");
            crtColumn = 1;
            if (*st == '\n' && !EOP) {
                fprintf(upfd, "\n");
                crtColumn = 1;
            }
            else putc(*st, upfd), prevChar = *st;
            EOP = TRUE;
        }
    }
}

/*
* TranslateFilename()
*
* This does translations on a filename.  This is used for embedding dates
* or numbers into a filename.
*/
void TranslateFilename(char *realfn, char *fn)
  {
  int year, day, hours, minutes;
  char *month;
  getCdate(&year, &month, &day, &hours, &minutes);
  do
    {
    *realfn = *fn;
    if (*fn == '%')
      {
      fn++;
      switch (*fn)
        {
        case 'm':
        case 'M':
        sPrintf(realfn, "%s", month);
        break;
        case 'y':
        case 'Y':
        sPrintf(realfn, "%d", year);
        break;
        default:
        sPrintf(realfn, "%c", *fn);
        break;

        }
      while (*(realfn + 1))
      realfn++;

      }
    realfn++;

    }
  while (*fn++);

  }

void netResult(char *msg)
  {
  printf("%s\n",msg);
  }

char *MonthTab[] =
    {
    "JANUARY", "FEBRUARY", "MARCH", "APRIL", "MAY", "JUNE",
    "JULY", "AUGUST", "SEPTEMBER", "OCTOBER", "NOVEMBER", "DECEMBER"

    };

char *KeyWords[] =
    {
    "TODAY", "YESTERDAY"

    };

char DayPMonth[] =
    {
    31,28,31,30,31,30,31,31,30,31,30,31

    };


/*
 * getCdate()
 *
 * This retrieves system date and returns in the parameters.
*/
  void getCdate(int *year, char **month, int *day, int *hours, int *minutes)
    {
    int mon, seconds, milli;
    getRawDate(year, &mon, day, hours, minutes, &seconds, &milli);
    *year -= 1900;
    *month = MonthTab[mon];

    }

void  Do_Stack_Check(void);


#define LeapYear(x)     ((x % 4) ? FALSE : ((year % 100) ? TRUE : FALSE))
/************************************************************************/
/*      ReadDate() interprets the string and returns it in seconds      */
/************************************************************************/
int ReadDate(char *date, long *RetTime)
  {
  int rover, found, keyword = -1;
  int   year, month, day, hours, minutes, seconds, milli;
  label mon;
  char *d = date;
  char  darray[6];              /* see format for utpack() */
  Do_Stack_Check();
  if (strLen(date) == 0) return FALSE;
  if (isdigit(date[0]))
    {
    darray[0] = (char) (atoi(date) + 1900 - 1970);
    while (isdigit(*date)) date++;

    }
  else
    {
    getRawDate(&year, &month, &day, &hours, &minutes,
    &seconds, &milli);
    year -= 1970;
    darray[0] = (char) year;
    darray[1] = (char) month;
    darray[2] = (char) day;

    }
  for (rover = 0; isalpha(*date); date++, rover++)mon[rover] = toUpper(*date);
  mon[rover] = 0;
  if (rover == 0)
    {
    if( isdigit(d[0]) )
      {
      *RetTime = CurAbsolute() - ( atol(d)*86400l);
      return TRUE;
      }
    return ERROR;
    };
  for (found = rover = 0; rover < NumElems(MonthTab); rover++)
  if (strncmp(mon, MonthTab[rover], strLen(mon)) == SAMESTRING)
    {
    found++;
    darray[1] = (char) (rover + 1);

    }
  if (found != 1)
    {
    for (keyword = -1, rover = 0; rover < NumElems(KeyWords); rover++)
    if (strncmp(mon, KeyWords[rover], strLen(mon)) == SAMESTRING)
      {
      keyword = rover;

      }
    if (keyword == -1)
    return ERROR;
    else
      {
      switch(keyword)
        {
        case 0: /* TODAY */
        break;
        case 1: /* YESTERDAY */
        --darray[2];
        if (!darray[2])
          {
          --darray[1];
          if (!darray[1])
            {
            darray[1] = 12;
            --darray[0];

            }
          darray[2] = DayPMonth[darray[1] - 1];
          if (darray[1] == 2 && LeapYear(darray[0]))
          darray[2] = 29;

          }
        break;

        }

      }

    }
  if ((keyword == -1) && ((darray[2] = (char) atoi(date)) == 0)) return ERROR;
  darray[3] = 0;
  darray[4] = 0;
  darray[5] = 0;
  *RetTime = utpack(darray);
  return TRUE;

  }

/************************************************************************/
/*      CurAbsolute() current time in absolute terms.                   */
/************************************************************************/
long CurAbsolute()
  {
  Do_Stack_Check();
  return time(NULL);

  }
