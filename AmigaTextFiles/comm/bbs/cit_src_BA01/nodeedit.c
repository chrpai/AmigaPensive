#include "ctdl.h"

#define R_BTM		12
#define R_TOP		8
#define R_RGT		64
#define R_LEFT		16

#define SetColors(x, y)		textattr((y << 4) + x)

#define ROUTE_WARNING   "WARNING: Outgoing route mail will be lost!"
#define ROUTE_CONFIRM   "Confirm killing %s? "
#define CARRYING_ROUTED "Node is carrying routed mail, cannot reroute."

extern CONFIG    cfg;
extern NetBuffer netBuf, netTemp;
extern FILE      *netfl;
extern int       thisNet;
extern NetTable  *netTab;
extern aRoom     roomBuf;		/* room buffer			*/
extern FILE      *roomfl;		/* file descriptor for rooms	*/
extern int       thisRoom;		/* room currently in roomBuf	*/
extern rTable	 *roomTab;		/* RAM index of rooms		*/
extern VirtualRoom *VRoomTab;
extern VirtNet     *VirtNetList;
extern char VirtualInUse;


void Error(char *str);
int  DoMN(void);
int  DoRoomSharing(void);
void SetUpMN(char *str);
void EatMN(void);
char Confirm(char *str);
int ValNewSystem(void);
void RunEdit(char clear);
char ShowIt(char *title, char *str, char *accept);
char KillNode();
char DirectRoute(NetBuffer *n);
void DispNetName();
void DispRoomName();
int  CmpNodes();

typedef int DataElement;

DataElement *AccessElement;
#define KeyStroke (0)
/*int  Keys[] = { INS, 0 }; */
DisplayList NetList = { { NULL, NULL, CmpNodes, free, NULL },
			" Node Selection ",
			NULL, KillNode,
			1, 80, 1, 25,
			0, 0, 0, 0,
			NAMESIZE + 4, DispNetName, NULL, KeyStroke };

typedef struct {
	label name;
	label id;
	int num;
	char cond[4];
} NetRec;

#define NORMAL_ROOM	0
#define VIRTUAL_ROOM	1
typedef struct {
	label name;
	char mode;
	union {
		SharedRoom ShareInfo;
		VirtPoint  VirtRoom;
	} Room;
} OneSharedRoom;

void RoomInfo(OneSharedRoom *data);
void freeNList(SListBase *t);
char ChName(char *str);
char ChId(char *str);
char ChShort(char *str);
char ChRoute(char *str);
char ChBaud(int i);
char WeSpine(char *c);
char IsSpine(char *c);
void NetDisp(), NodeInfo();


SListBase c1 = { NULL, NULL, NULL, free, NULL };
SListBase c2 = { NULL, NULL, NULL, free, NULL };

char *Rates[] = {
	"300", "1200", "2400", "4800", "9600", "14400", "19200", "38400", "57600" ""
};

char is_spine, spine;
DataElement *SpEl, *IsEl;

#define MN_LEFT		20
#define MN_RIGHT	60
#define MN_TOP		5
#define MN_BOTTOM       24

DataElement *AccessElements;

Screen MNets = { " M e m b e r   N e t s ", NULL,
		{ NULL, NULL, NULL, freeNList, NULL },
		  MN_LEFT, MN_TOP, MN_RIGHT, MN_BOTTOM,
		  WHITE, BLACK, BLACK, LIGHTGRAY,
		 0, 0, 0, 0,
		 KeyStroke,
		 FULL_BORDER
};

char mn[32], MNval[20];

SListBase cc1 = { NULL, NULL, NULL, NoFree, NULL };
SListBase cc2 = { NULL, NULL, NULL, NoFree, NULL };

char extdialer, oldext;

label Router;
DataElement *Baud;

char ExtWarn();

void RunEdit(clear)
char clear;
{
	char on, s_files, r_files, local, stadel, n_mail, RT, RF, routelock;

		getNet(netBuf.nbRoute, &netTemp);
		if (netBuf.nbRouteGen == netTemp.nbGen)
			strCpy(Router, netTemp.netName);
		else {
			strCpy(Router, "<no route>");
			netBuf.nbRoute = -1;
		}
	}
	else strCpy(Router, "<no route>");

	on = netBuf.nbflags.OtherNet;
	spine = netBuf.nbflags.spine;
	is_spine = netBuf.nbflags.is_spine;
	s_files = netBuf.nbflags.send_files;
	r_files = netBuf.nbflags.room_files;
	local = netBuf.nbflags.local;
	stadel = netBuf.nbflags.Stadel;
	routelock = netBuf.nbflags.RouteLock;
	n_mail = netBuf.nbflags.normal_mail;
	RT = netBuf.nbflags.RouteTo;
	RF = netBuf.nbflags.RouteFor;
	extdialer = oldext = netBuf.nbflags.ExternalDialer;

	SetUpMN(MNval);

	AddData(&c1, MakeString("Name", 1, 3, netBuf.netName, NAMESIZE, ChName,
						TYPICAL, 0), NULL, FALSE);
	AddData(&c2, MakeString("ID", 40, 3, netBuf.netId, NAMESIZE, ChId,
						TYPICAL, 0), NULL, FALSE);
	AddData(&c2, MakeString("Condensed", 50, 5, netBuf.nbShort, 3,
					   ChShort, TYPICAL, 0), NULL, FALSE);
	AddData(&c1, (AccessElement = MakeString("Access", 1, 5, netBuf.access,
				40, NULL, TYPICAL, 0)), NULL, FALSE);
	AddData(&c1, MakeString("Our pwd", 1, 7, netBuf.OurPwd, NAMESIZE,
					   NULL, TYPICAL, 0), NULL, FALSE);
	AddData(&c2, MakeString("Their pwd", 40, 7, netBuf.TheirPwd, NAMESIZE,
					   NULL, TYPICAL, 0), NULL, FALSE);
	AddData(&c1, MakeBool("Local System", 1, 9, &local, NULL, TYPICAL, 0),
								NULL, FALSE);
	AddData(&c2, MakeBool("OtherNet System", 40, 9, &on, NULL, TYPICAL, 0),
								NULL, FALSE);
	AddData(&c1, (SpEl = MakeBool("We are spine", 1, 11, &spine, WeSpine,
						TYPICAL, 0)), NULL, FALSE);
	AddData(&c2, (IsEl = MakeBool("System is spine", 40, 11, &is_spine,
				       IsSpine, TYPICAL, 0)), NULL, FALSE);
	AddData(&c1, MakeBool("Route mail to", 1, 13, &RT, NULL, TYPICAL, 0),
								NULL, FALSE);
	AddData(&c2, MakeBool("Route mail from", 40, 13, &RF, NULL, TYPICAL, 0),
								NULL, FALSE);

	AddData(&c1, (Baud = MakeLim("Baud rate", 1, 15, Rates,
			netBuf.baudCode, 5, TYPICAL, 0)), NULL, FALSE);

	AddData(&c2, MakeUS("Member nets", 40, 15, MNval, DoMN, TYPICAL, 0),
						NULL, FALSE);

	AddData(&c1, MakeString("Routing node", 1, 17, Router, NAMESIZE,
					   ChRoute, TYPICAL, 0), NULL, FALSE);
	AddData(&c2, MakeBool("STadel", 40, 17, &stadel, NULL, TYPICAL,
							0), NULL, FALSE);
	AddData(&c1, MakeBool("Route Lock", 1, 19, &routelock, NULL, TYPICAL,
							0), NULL, FALSE);
	AddData(&c2, MakeBool("External Dialer", 40, 19, &extdialer, ExtWarn,
						TYPICAL, 0), NULL, FALSE);
	AddData(&c1, MakeUS("Room Sharing", 1, 21, " Hit space ", DoRoomSharing,
						TYPICAL, 0), NULL, FALSE);

	AddData(&Node.DataList, &c1, NULL, FALSE);
	AddData(&Node.DataList, &c2, NULL, FALSE);

	ScreenIt(&Node, clear);

	netBuf.nbflags.OtherNet = on;
	netBuf.nbflags.spine = spine;
	netBuf.nbflags.is_spine = is_spine;
	netBuf.nbflags.send_files = s_files;
	netBuf.nbflags.room_files = r_files;
	netBuf.nbflags.local = local;
	netBuf.nbflags.Stadel = stadel;
	netBuf.nbflags.RouteLock = routelock;
	/* netBuf.nbflags.normal_mail = n_mail; */
	netBuf.baudCode = Baud->val.A_Lim.index;
	netBuf.nbflags.RouteTo = RT;
	netBuf.nbflags.RouteFor = RF;
	netBuf.nbflags.ExternalDialer = extdialer;
	EatMN();

	KillList(&Node.DataList);
	KillList(&MNets.DataList);
}

void freeNList(SListBase *t)
{
	KillList(t);
}

int CmpNodes(s1, s2)
NetRec *s1, *s2;
{
	return strCmpU(s1->name, s2->name);
}

void SetUpMN(str)
char *str;
{
	int i;
	char *t, Disabled = TRUE;
	long m = 1l;

	MNets.bg = cfg.DepData.ScreenColors.ScrBack;
	MNets.fg = cfg.DepData.ScreenColors.ScrFore;
	MNets.dbg = cfg.DepData.ScreenColors.StatBack;
	MNets.dfg = cfg.DepData.ScreenColors.StatFore;
	MNets.tfg = cfg.DepData.ScreenColors.StatFore;
	MNets.tbg = cfg.DepData.ScreenColors.StatBack;
	MNets.fcf = cfg.DepData.ScreenColors.ScrFore;
	MNets.fcb = cfg.DepData.ScreenColors.ScrBack;

	for (i = 0; i < 32; i++, m <<= 1) {
		mn[i] = !((netBuf.MemberNets & m) == 0);
		if (mn[i]) Disabled = FALSE;
	}

	strcpy(str, (Disabled) ? " Disabled " : " Active ");

	for (i = 0; i < 16; i++) {
		t = GetDynamic(10);
		sprintf(t, "%2d", i+1);
		AddData(&cc1, MakeBool(t, 13, i+3, mn+i, NULL, VALUE_AFTER, 0),
								NULL, FALSE);
	}

	for (i = 16; i < 32; i++) {
		t = GetDynamic(10);
		sprintf(t, "%2d", i+1);
		AddData(&cc2, MakeBool(t, 19, i+3-16, mn+i, NULL, 0, 0), NULL,
								FALSE);
	}
	AddData(&MNets.DataList, &cc1, NULL, FALSE);
	AddData(&MNets.DataList, &cc2, NULL, FALSE);
}

void EatMN()
{
	MULTI_NET_DATA m;
	int i;

	for (netBuf.MemberNets = 0l, m = 1l, i = 0; i < 32; i++, m <<= 1)
		if (mn[i]) netBuf.MemberNets |= m;
}

static char FoundDup, *check;
static NetRec *temp;

void CheckName();
void CheckId();

char ChName(char *str)
{
	char buf[NAMESIZE * 2];

	while (*str == ' ') str++;
	if (strLen(str) == 0)
		return FALSE;

	FoundDup = FALSE;
	check = str;
	RunList(&NetList.Data, CheckName);
	if (FoundDup) {
		sprintf(buf, "%s already in use.", str);
		Error(buf);
		return FALSE;
	}
	return TRUE;
}

void CheckName(d)
NetRec *d;
{
	if (d->num != thisNet) {
		if (strCmpU(d->name, check) == SAMESTRING) {
			FoundDup = TRUE;
			temp = d;
		}

		if (strCmpU(d->cond, check) == SAMESTRING) {
			FoundDup = TRUE;
			temp = d;
		}
	}
}

char ChId(char *str)
{
	char buf[80];
	label b;

	while (*str == ' ') str++;
	if (strLen(str) == 0)
		return FALSE;

	normId(str, b);
	FoundDup = FALSE;
	check = b;
	RunList(&NetList.Data, CheckId);
	if (FoundDup) {
		sprintf(buf, "%s already in use.", str);
		sprintf(buf, "%s already in use (%s).", str,
							temp->name);
		Error(buf);
		return FALSE;
	}
	ReDoAuxFiles(b);
	return TRUE;
}

void CheckId(d)
NetRec *d;
{
	if (d->num != thisNet)
		if (strCmpU(d->id, check) == SAMESTRING) {
			temp = d;
			FoundDup = TRUE;
		}
}

char IsSpine(char *c)
{
	if (spine) {
		spine = FALSE;
		SuddenUpdate(&Node, SpEl);
	}

	return TRUE;
}

char WeSpine(char *c)
{
	if (is_spine) {
		is_spine = FALSE;
		SuddenUpdate(&Node, IsEl);
	}

	return TRUE;
}

char ChRoute(char *name)
{
	char *s, buf[80], first[50];

	s = name;
	while (*name == ' ') name++;
	if (strLen(name) == 0) {
		strcpy(s, "<no route>");
		BackToDirect(thisNet, &netBuf);
		netBuf.nbRoute = -1;
		return TRUE;
	}
	else {
		FoundDup = FALSE;
		check = name;
		RunList(&NetList.Data, CheckName);
		if (!FoundDup) {
			sprintf(buf, "%s not found.", name);
			Error(buf);
			return FALSE;
		}
		getNet(temp->num, &netTemp);
		if (!DirectRoute(&netTemp)) {
			sprintf(buf, "we do not directly connect with %s",
								name);
			Error(buf);
			return FALSE;
		}

		if (!ReDoRouted(temp->num)) {
			sprintf(buf, CARRYING_ROUTED);
			Error(buf);
			return FALSE;
		}

		RouteLocalMail(temp->num);
		netBuf.nbRoute = temp->num;
		netBuf.nbRouteGen = netTemp.nbGen;
		sprintf(buf, "r%d.0", temp->num);
		makeSysName(first, buf, &cfg.netArea);
		netTemp.nbflags.HasRouted = (access(first, 0) == 0);
		putNet(temp->num, &netTemp);
	}
	return TRUE;
}

char ChShort(char *str)
{
	int i;
	char buf[80];

	for (i = 0; i < cfg.netSize; i++) {
		if (i != thisNet &&
			     strCmpU(str, netTab[i].ntShort) == SAMESTRING) {
			getNet(i, &netTemp);
			if (netTemp.nbflags.in_use) {
				sprintf(buf, "%s already in use (%s).", str,
							netTemp.netName);
				Error(buf);
				return FALSE;
			}
		}
	}

	return TRUE;
}

#define realen(s)	(strlen(s) + 8)
char ShowIt(title, str, accept)
char *str, *title, *accept;
{
	void *Save;
	int left, right, c;

	left = 40 - (realen(str) / 2) - 1;
	right = 40 + (realen(str) / 2) + 1;
	Save = BorderedMenu(left, R_TOP + 1, right, R_BTM + 1,
		cfg.DepData.ScreenColors.StatFore,
		cfg.DepData.ScreenColors.StatBack,
		cfg.DepData.ScreenColors.StatFore,
		cfg.DepData.ScreenColors.StatBack, title);
	gotoxy(2, 2);
	cprintf(str);
	if (accept != NULL) {
	    do {
		    c = toupper(KeyStroke());
	    } while (strchr(accept, c) == NULL);
	}
	else KeyStroke();

	FreeBorderedMenu(left, R_TOP + 1, right, R_BTM + 1, Save);
	return c;
}

void Error(str)
char *str;
{
	char work[100];

	ShowIt(" Error: ", str, NULL);
}

char Confirm(char *str)
{
	char work[50];

	sprintf(work, "%s? (Y/N/C)", str);
	return ShowIt(" System Addition ", work, "YCN");
}

void RingBell() {}

void DoEdit()
{
	int ii, c, mode;
	NetRec *temp;
	void *Save;
	SYS_FILE roomFile;

	if (cfg.netSize == 0) {
		printf("No nets to play with!");
		return;
	}

	for (ii = 0; ii < cfg.netSize; ii++) {
		getNet(ii, &netBuf);
		if (netBuf.nbflags.in_use) {
			temp = GetDynamic(sizeof *temp);
			strCpy(temp->name, netBuf.netName);
			strCpy(temp->id, netBuf.netId);
			strCpy(temp->cond, netBuf.nbShort);
			temp->num = ii;
			AddData(&NetList.Data, temp, NULL, FALSE);
		}
	}

	makeSysName(roomFile, "ctdlroom.sys", &cfg.roomArea);
	openFile(roomFile, &roomfl);
	initRoomBuf(&roomBuf);

	NetList.bg = cfg.DepData.ScreenColors.ScrBack;
	NetList.fg = cfg.DepData.ScreenColors.ScrFore;
	NetList.dbg = cfg.DepData.ScreenColors.StatBack;
	NetList.dfg = cfg.DepData.ScreenColors.StatFore;
	temp = NULL;
	mode = DEL_ACTIVE;
	do {
		if ((temp = DispList(&NetList, temp, &c, mode)) != NULL) {
			Save = SaveImage(1, 1, 80, 25);
			switch (c) {
			case '\r':
			case '\n':
				getNet(temp->num, &netBuf);
				RunEdit(TRUE);
				putNet(temp->num, &netBuf);
				mode |= NO_REFRESH;
				break;
			case INS:
				ii = TRUE;
				killNetBuf(&netBuf);
				zero_struct(netBuf);
				initNetBuf(&netBuf);
				netBuf.nbRoute = -1;
				do {
					RunEdit(ii);
					ii = FALSE;
				} while (!ValNewSystem());
				mode &= ~(NO_REFRESH);
				break;
			}
			PutImage(1, 1, 80, 25, Save);
		}
	} while (temp != NULL);
	clrscr();
	printf(R_TITLE);
}

int ValNewSystem()
{
	int i;
	NetRec *temp;

	if (strLen(netBuf.netName) == 0) {
		Error("Need Name field.");
		return FALSE;
	}

	if (strLen(netBuf.netId)   == 0) {
		Error("Need Id field.");
		return FALSE;
	}

	switch (Confirm("Save")) {
	case 'Y':
		i = GetNewSlot(&netTemp);
		netBuf.nbGen = netTemp.nbGen;
		netBuf.nbflags.in_use = TRUE;
		putNet(i, &netBuf);
		InitVNode(i);
		temp = GetDynamic(sizeof *temp);
		strCpy(temp->name, netBuf.netName);
		strCpy(temp->id, netBuf.netId);
		temp->num = i;
		AddData(&NetList.Data, temp, NULL, FALSE);
		return TRUE;
	case 'C': return FALSE;
	case 'N': return TRUE;
	}
}

void DispNetName(NetRec *d)
{
	cprintf(d->name);
}

int DoMN()
{
	char *save, Disabled = TRUE;
	int  c, i;
	MULTI_NET_DATA m = 1l;

	switch ((c = KeyStroke())) {
	case ESC:
	case UP:
	case DOWN:
	case LEFT:
	case RIGHT:
		return c;
	case '\r':
		return DOWN;
	case '\b':
		return UP;
	}

	save = GetDynamic(2*(MN_RIGHT-MN_LEFT+1)*(MN_BOTTOM-MN_TOP+1));
	gettext(MN_LEFT, MN_TOP, MN_RIGHT, MN_BOTTOM, save);

	ScreenIt(&MNets, TRUE);

	puttext(MN_LEFT, MN_TOP, MN_RIGHT, MN_BOTTOM, save);
	free(save);

	for (i = 0; i < 32; i++, m <<= 1) {
		if (mn[i]) Disabled = FALSE;
	}

	strcpy(MNval, (Disabled) ? " Disabled " : " Active ");

	return 0;
}

int DoRoomSharing()
{
	int  key, rover, count = 0, mode;
	void *Save;
	OneSharedRoom *data = NULL;
	static DisplayList Rooms = { { NULL, NULL, NULL, free, NULL },
			" Rooms Shared ",
			NULL, NULL,
			1, 80, 1, 25,
			0, 0, 0, 0,
			NAMESIZE + 4, DispRoomName, NULL, KeyStroke };

	Rooms.bg = cfg.DepData.ScreenColors.ScrBack;
	Rooms.fg = cfg.DepData.ScreenColors.ScrFore;
	Rooms.dbg = cfg.DepData.ScreenColors.StatBack;
	Rooms.dfg = cfg.DepData.ScreenColors.StatFore;

	if ((key = KeyStroke()) != ' ') return key;

	KillList(&Rooms.Data);
	for (rover = 0; rover < SHARED_ROOMS; rover++) {
		if (isSharedRoom(thisNet, rover) &&
					roomValidate(thisNet, rover)) {
			data = GetDynamic(sizeof *data);
			strcpy(data->name, roomTab[netRoomSlot(rover)].rtname);
			data->Room.ShareInfo = netBuf.netRooms[rover];
			AddData(&Rooms.Data, data, NULL, FALSE);
			count++;
			data->mode = NORMAL_ROOM;
		}
	}

	VirtInit();
	if (VirtualInUse) {
		for (rover = 0; rover < VIRT_LIMIT; rover++) {
			if (VirtNetList[thisNet].VirtList[rover].WhichVirt != -1) {
				key = VirtNetList[thisNet].VirtList[rover].WhichVirt;
				data = GetDynamic(sizeof *data);
				data->mode = VIRTUAL_ROOM;
				data->Room.VirtRoom = VirtNetList[thisNet].VirtList[rover];
				strcpy(data->name, VRoomTab[key].vrName);
				AddData(&Rooms.Data, data, NULL, FALSE);
				count++;
			}
		}
	}

	if (count == 0) {
		Save = BorderedMenu(R_LEFT, R_TOP, R_RGT, R_BTM,
			cfg.DepData.ScreenColors.StatFore,
			cfg.DepData.ScreenColors.StatBack,
			cfg.DepData.ScreenColors.StatFore,
			cfg.DepData.ScreenColors.StatBack, "");

		gotoxy(2, 2);
		cprintf("No rooms shared with %s.", netBuf.netName);
		KeyStroke();
		FreeBorderedMenu(R_LEFT, R_TOP, R_RGT, R_BTM, Save);
		return 0;
	}
	Save = SaveImage(1, 1, 80, 25);
	mode = 0;
	data = NULL;
	while ((data = DispList(&Rooms, data, &key, mode)) != NULL) {
		RoomInfo(data);
		mode |= NO_REFRESH;
	}
	PutImage(1, 1, 80, 25, Save);
	return 0; 	/* important */
}

void DispRoomName(OneSharedRoom *d)
{
	cprintf("%s", d->name);
}

#define BTM     12
#define TOP     8
#define RGT     64
#define W_LEFT  16
char KillNode(NetRec *which)
{
	void *Save;
	int  c;
	char toReturn;

	Save = BorderedMenu(W_LEFT, TOP, RGT, BTM,
		cfg.DepData.ScreenColors.StatFore,
		cfg.DepData.ScreenColors.StatBack,
		cfg.DepData.ScreenColors.StatFore,
		cfg.DepData.ScreenColors.StatBack, "");

	gotoxy(2, 2);
	cprintf("Really kill %s? ", which->name);
	do
		c = toupper(KeyStroke());
	while (c != ESC && c != 'Y' && c != 'N');

	if (c == 'Y') {
		getNet(which->num, &netBuf);
		if (netBuf.nbflags.HasRouted) {
			clrscr();
			gotoxy(3, 2);
			cprintf(ROUTE_WARNING);
			gotoxy(3, 3);
			cprintf(ROUTE_CONFIRM, netBuf.netName);
			do
				c = toupper(KeyStroke());
			while (c != ESC && c != 'Y' && c != 'N');
			if (c == 'Y')
				toReturn = TRUE;
			else toReturn = FALSE;
		}
		else toReturn = TRUE;

		if (toReturn) {
			KillAuxFiles();
			netBuf.nbflags.in_use = FALSE;
			putNet(which->num, &netBuf);
			toReturn = TRUE;
		}
	}
	else toReturn = FALSE;

	FreeBorderedMenu(W_LEFT, TOP, RGT, BTM, Save);

	return toReturn;
}

void KillAuxFiles()
{
	SYS_FILE fn, fn2;
	label    temp, Id, OurId;
	int      i, j;
	extern FILE *upfd;
	extern NetBuffer nBuf1;

	sPrintf(temp, "%d.ml", thisNet);
	makeSysName(fn, temp, &cfg.netArea);
	unlink(fn);

	sPrintf(temp, "%d.rfl", thisNet);
	makeSysName(fn, temp, &cfg.netArea);
	unlink(fn);

	sPrintf(temp, "%d.sfl", thisNet);
	makeSysName(fn, temp, &cfg.netArea);
	unlink(fn);

	sPrintf(temp, "%d.vtx", thisNet);
	makeSysName(fn, temp, &cfg.netArea);
	unlink(fn);

	for (i = 0; ; i++) {
		sPrintf(temp, "R%d.%d", thisNet, i);
		makeSysName(fn, temp, &cfg.netArea);
		if (unlink(fn) != 0) break;
	}

	if (!DirectRoute(&netBuf)) {
		normId(netBuf.netId, OurId);
		getNet(netBuf.nbRoute, &nBuf1);
		for (i = 0; ; ) {
			sPrintf(temp, "R%d.%d", netBuf.nbRoute, i);
			makeSysName(fn, temp, &cfg.netArea);
			if ((upfd = fopen(fn, READ_ANY)) == NULL)
				break;
			getMsgStr(getNetChar, Id, NAMESIZE);
			fclose(upfd);
			NormStr(Id);
			if (strCmpU(OurId, Id) == SAMESTRING) {
				unlink(fn);
				for (j = i + 1; ; j++) {
					sPrintf(temp, "R%d.%d", netBuf.nbRoute,
						j);
					makeSysName(fn, temp, &cfg.netArea);
					if (access(fn, 0) != 0) break;
					sPrintf(temp, "R%d.%d", netBuf.nbRoute,
						j - 1);
					makeSysName(fn2, temp, &cfg.netArea);
					rename(fn, fn2);
				}
			}
			else i++;
		}
		if (i == 0)
			nBuf1.nbflags.HasRouted = FALSE;
		netBuf.nbHiRouteInd = i - 1;
		putNet(netBuf.nbRoute, &nBuf1);
	}
}

void ReDoAuxFiles(NewId)
label NewId;
{
    label ThisId;
    int AuxWork(int target, label ThisId, label NewId);

    normId(netBuf.netId, ThisId);
    AuxWork(netBuf.nbRoute, ThisId, NewId);
    AuxWork(thisNet, ThisId, NewId);
}

AuxWork(target, ThisId, NewId)
int target;
label ThisId, NewId;
{
    label temp, Name, Id;
    SYS_FILE fn;
    int     rover = 0;
    FILE *fd;
    extern char *R_W_ANY;

    if (strCmpU(NewId, ThisId) != SAMESTRING) {
	do {
	    sPrintf(temp, "R%d.%d", target, rover++);
	    makeSysName(fn, temp, &cfg.netArea);
	    if (IdRouteFile(fn, Id, Name)) {
		if (strCmpU(Id, ThisId) == SAMESTRING) {
		    fd = fopen(fn, R_W_ANY);
		    fprintf(fd, "%-20s", NewId);
		    fclose(fd);
		}
	    }
	    else break;
	} while (TRUE);
    }
}

char *LastOn(long s, char d);

void NodeInfo()
{
	SetColors(cfg.DepData.ScreenColors.StatFore, cfg.DepData.ScreenColors.StatBack);
	gotoxy(2, 25);
	cprintf("Last contacted: %s", LastOn(netBuf.nbLastConnect, FALSE));
}

#define MSG		"Don't forget to set the Access field."
#define ERR_TOP		9
#define ERR_BOTTOM	(ERR_TOP + 4)
#define ERR_RIGHT	(42 + (strlen(MSG) / 2))
#define ERR_LEFT	(35 - (strlen(MSG) / 2))
char ExtWarn(char *newval)
{
	char *Save;

	if (*newval) {
		Save = BorderedMenu(ERR_LEFT, ERR_TOP, ERR_RIGHT, ERR_BOTTOM,
					WHITE, RED, WHITE, RED, "");
		gotoxy(3, 2);
		cprintf(MSG);
		KeyStroke();
		FreeBorderedMenu(ERR_LEFT,ERR_TOP, ERR_RIGHT, ERR_BOTTOM, Save);
	}
	else {
		netBuf.access[0] = 0;
		SuddenUpdate(&Node, AccessElement);
	}
	return TRUE;
}

#define ROOM_BTM		14
#define ROOM_TOP		8
#define ROOM_RGT		64
#define ROOM_LEFT		16
void RoomInfo(OneSharedRoom *data)
{
	void *Save;
	char moo[20];
	int roomno;

	Save = BorderedMenu(ROOM_LEFT, ROOM_TOP, ROOM_RGT, ROOM_BTM,
			cfg.DepData.ScreenColors.StatFore,
			cfg.DepData.ScreenColors.StatBack,
			cfg.DepData.ScreenColors.StatFore,
			cfg.DepData.ScreenColors.StatBack, "");

	if (data->mode == NORMAL_ROOM) {
		if ((roomno = roomExists(data->name)) == ERROR) {
			gotoxy(2, 2);
			cprintf("Ooops, internal bug.");
		}
		else {
			getRoom(roomno);
			gotoxy(2, 1);
			cprintf("%s", data->name);
			gotoxy(2, 3);
			if (roomBuf.rbShareType == PEON)
				cprintf("Peon relationship");
			else switch (data->Room.ShareInfo.mode) {
			case PEON:
				cprintf("Peon relationship");
				break;
			case PASS_BACKBONE:
				cprintf("Passive Backbone relationship");
				break;
			case ACTIVE_BACKBONE:
				cprintf("Active Backbone relationship");
				break;
			default:
				cprintf("OOPS: %d", data->Room.ShareInfo.mode);
			}
			gotoxy(2, 4);
			cprintf("Last message sent was %s.",
					PrintPretty(data->Room.ShareInfo.lastMess, moo));
		}
	}
	else {
		gotoxy(2, 1);
		cprintf("%s", data->name);
		gotoxy(2, 3);
		switch (data->Room.VirtRoom.mode) {
		case PEON:
			cprintf("Peon relationship");
			break;
		case PASS_BACKBONE:
			cprintf("Passive Backbone relationship");
			break;
		case ACTIVE_BACKBONE:
			cprintf("Active Backbone relationship");
			break;
		default:
			cprintf("OOPS: %d", data->Room.ShareInfo.mode);
		}
		gotoxy(2, 4);
		cprintf("A virtual room.");
	}
	KeyStroke();
	FreeBorderedMenu(ROOM_LEFT, ROOM_TOP, ROOM_RGT, ROOM_BTM, Save);
}

/************************************************************************/
/*	roomExists() returns slot# of named room else ERROR             */
/************************************************************************/
int roomExists(char *room)
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
