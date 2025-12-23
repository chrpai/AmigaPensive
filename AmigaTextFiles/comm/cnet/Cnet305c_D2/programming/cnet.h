#include <exec/types.h>

#define CNET_VERSION "$VER: CNet PRO 3.05"

#define TOP_STAT 51
#define TOP_NOSTAT 11

#define C_BLACK  0
#define C_RED    4
#define C_GREEN  2
#define C_YELLOW 6
#define C_BLUE   1
#define C_PURPLE 5
#define C_CYAN   3
#define C_WHITE  7

#define GRN_BLK 0x20
#define YEL_BLK 0x60
#define BLU_BLU 0x11
#define CYN_BLK 0x30
#define CYN_BLU 0x31
#define WHT_BLK 0x70
#define WHT_RED 0x74
#define WHT_BLU 0x71
#define WHT_PUR 0x75

#define EVER ;;

#define EDHANDLE_FLAG		0x01	/* bit defs for ABits2 of struct Privs */
#define EDREALNAME_FLAG		0x02
#define EDADDRESS_FLAG		0x04
#define EDVOICE_FLAG		0x08
#define EDDATA_FLAG		0x10
#define BANNER_FLAG		0x20
#define NOT_USED_FLAG		0x40
#define PORTMONITOR_FLAG	0x80
#define VOTEMAINT_FLAG		0x0100
#define ALARMSYSOP_FLAG		0x0200
#define OPENSCREEN_FLAG		0x0400
#define OPENCAPTURE_FLAG	0x0800
#define NETMAIL_FLAG		0x1000
#define FREQSEND_FLAG		0x2000
#define NETCOSTEXEMPT_FLAG	0x4000
#define EXPANSION1_FLAG		0x8000
#define EXPANSION2_FLAG		0x010000
#define EXPANSION3_FLAG		0x020000
#define EXPANSION4_FLAG		0x040000
#define UUCPMAIL_FLAG		0x080000
#define NETCOSTCREDITS_FLAG	0x100000
#define HOLDANDCRASH_FLAG	0x200000
#define EXPANSION8_FLAG		0x400000
#define BROADCAST_FLAG		0x800000
#define EXPANSION10_FLAG	0x01000000

#define EMAIL_FLAG		0x01	/* bit defs for ABits of struct Privs */
#define	PFILE_FLAG		0x02
#define GFILE_FLAG		0x04
#define	ULIST_FLAG		0x08
#define	SYSOP_FLAG		0x10
#define	REWARDS_FLAG		0x20	/* may receive download rewards? */
#define	FCREDEX_FLAG		0x40
#define BCREDEX_FLAG		0x80
#define	EXPIREMAIL_FLAG		0x0100
#define	BULK_FLAG		0x0200
#define PARTY_FLAG		0x0400
#define	URGENT_FLAG		0x0800
#define	TIMESBP_FLAG		0x1000
#define	FILEADD_FLAG		0x2000
#define	READANY_FLAG		0x4000
#define	DELETEANY_FLAG		0x8000
#define XXXXXXXXX_FLAG		0x010000	/* not used */
#define	XXXXXXXXX4_FLAG		0x020000
#define AUTOVALID_FLAG		0x040000
#define	XXXXXXXXX2_FLAG		0x080000	/* not used */
#define	SEEANON_FLAG		0x100000
#define	XXXXXXXXX3_FLAG		0x200000	/* not used */
#define	CONF_FLAG		0x400000
#define CONFCON_FLAG		0x800000
#define MCI1_FLAG		0x01000000
#define MCI2_FLAG		0x02000000
#define	RELOGON_FLAG		0x04000000
#define RECEIVEMAIL_FLAG 	0x08000000
#define FORWARD_FLAG   		0x10000000
#define VOTETOPIC_FLAG  	0x20000000
#define VOTECHOICE_FLAG 	0x40000000
#define NOLOCKS_FLAG		0x80000000

struct RangeContext {
	long	nparts;
	long	parts[10][2];
	long	set;
	long	element;
};

struct DrawElement {
	UBYTE	x;		/* x screen position */
	UBYTE	y;		/* y screen position */
	UBYTE	att;		/* the character's attribute (colors) */
	UBYTE	att2;		/* the character's attribute (others) */
	UBYTE	c;		/* the character */
	UBYTE	att0;		/* Was the attribute (underneath) */
	UBYTE	att20;		/* Was the attribute (underneath) */
	UBYTE	c0;		/* Was the character (underneath) */
};

struct VDEentry {
	char	text[48];

	short	xpos,		/* where to print these things */
		ypos;

	long	min,		/* for numerix */
		max;		/* for numerix */

	short	length,		/* for string entry, field size */
				/* for bit boolean, which bit? */

		type;		/* 0 = UBYTE 0/1
				   1 = ULONG BIT 0/1
				   2 = Text (char *)
				   3 = BYTE numeric
				   4 = short numeric
				   5 = USHORT numeric
				   6 = long numeric
				   7 = date
				   8 = flags
				   9 = ordered BBSTEXT list
				   10= ordered BBSMENU list
				   11= char **
				   12= UBYTE BIT 0/1 */

	long	offset;		/* position in structure */

	short	screen0,	/* belongs-to screen */
		screen2;	/* goes-to screen, -1 for exit */

	short	goup,		/* VDEentry for movement */
		godown,
		goleft,
		goright;

	UWORD	ghost;
};

struct TermLink {
	char	dial[32];
	long	baud;
	char	ports[40];
	long	access;
	short	rate;
	char	name[38];
	short	databits;
};

struct LogType {
	char	name  [10];
	short	id;

	char	custom[10];

	UBYTE	flag;
	UBYTE	other;
};

struct	FidoType {
	char	Name[25];

	USHORT	ZoneB,  Zone,
		NetB,   Net,
		NodeB,  Node,
		PointB, Point;

	USHORT	ZoneC, NetC, NodeC, PointC;

	char	EchoOrigin[78];
	USHORT	LoZone;

	char	NetOrigin[76];
	USHORT	Pointnet;
	USHORT	HiZone;
};

struct	ProtoType {
	char	Name[16];
	char	Env[36];
	short	Flags;
	char	One[2];
	char	Path[22];
};

struct	ArcType {
	char	Suffix[8];
	char	Keyword[16];
	char	View[64];
	char	Test[64];
	char	Pack[64];
	char	Extract[64];

	long	Row;
	long	Column;
};

struct	EditorType {
	char	name[20];
	char	path[32];
	long	flags;
};

struct	Config1 {
	char	MySystemName[26];
	char	MySysopName[26];
	char	MyLocation[26];
	char	MyPhoneNumber[20];
	char	MyBBSID[10];

	char	FontName[40];
	long	DefBalance;
	long	DefaultColor;
	long	BlankTicks;
	long	BlankBright;

	long	nUDBASEx;		/* highest UDBASE partition */
	long	MaxShortLines;		/* Short description lines */
	long	MaxOpenPfiles;		/* stuff from BBSCONFIG */
	long	maxUserAccounts;
	long	NumRooms;
	long	nLinkPorts;		/* max link ID            */
	long	nsub;			/* */
	long	BListPurgeDays;
	long	nselect;		/* */
	long	nupload;
	long	nlist;
	long	maxYankTasks;
	long	maxYankSize;
	long	maxYankDays;
	long	maxLogonAttempts;
	long	maxLogonTime;
	long	maxYanksPerUser;

	long	DefNetCredits;
	long	DefByteCredits;
	long	DefFileCredits;
	char	MyCountry[4];
	char	MyAreaCode[12];
	char	DefDefProtocol[4];
	char	Yprefix[4];		/* Yank identifier */
	long	DefDoorPoints;
	long	DefTimeForm;

	UBYTE	MyLinkID;

	UBYTE	LogonFeedback;
	UBYTE	LogonSearch;
	UBYTE	GuestUsers;
	UBYTE	HideStatus;
	UBYTE	ConfProfile;
	UBYTE	MailFeedback;
	UBYTE	SeparateTexts;
	UBYTE	IndentSpaces;
	UBYTE	CIDvalidate;
	UBYTE	CIDhandleskip;
	UBYTE	SkipIdlePorts;

	char	MyLinkPass[8];

	char	OLMpath[40],
		ZIPpath[40],
		EXTRACTpath[40],
		YANKwork[40],
		RAMpath[40],
		TERMpath[40],
		LocalEditor[40],
		CDROMpath[40],
		DictPath[40];

	char	FKey [10][80];
	char	TFKey[10][80];

	USHORT	DColors2[2];
	USHORT	DColors4[4];
	USHORT	DColors8[8];
	USHORT	DColors16[16];

	long	narc;
	long	ned;
	long	nproto;
	long	nfido;
	long	nlog;

	struct	ArcType	   arc   [20];
	struct	EditorType ed     [8];
	struct	ProtoType  proto [16];
	struct	FidoType   fido  [12];
	struct	LogType	   log   [88];

	char	MyUUCPName[44];
	char	OutboundPath[40];
	char	InboundPath[40];

	ULONG	TossInputSize;
	ULONG	TossOutputSize;
	USHORT	DupeTableSize;

	UBYTE	BadUnknown;
	UBYTE	Trapdoor174;
	UBYTE	BadDupes;
	UBYTE	ZoneGateAdjust;
	UBYTE	ShowKludges;
	UBYTE	ThreeD;
	UBYTE	DeleteUUCP;
	UBYTE	CIDsilence;
	UBYTE	TwoDotMsg;

	char	expansion[73];

	char	Nodelist[40];
};

struct LinkPortUser {
	short	tick;

	UBYTE	channel;
	char	name[21];
};

struct LinkPort {
	long	s;				/* serial number */

	short	ports;
	char	name[22];

	struct	LinkPortUser user[100];
};

struct IsDate {		/* must LEAVE as BYTE */
	BYTE Year;
	BYTE Month;
	BYTE Date;
	BYTE Hour;
	BYTE Minute;
	BYTE Second;
};

/* LONG aligned */

struct RoomConfig {
	long	Access;		/* which groups may enter */

	short	Creator;	/* who created this room (id#) */
	short	Youngest;
	short	Oldest;

	UBYTE	Doors;
	UBYTE	NameType;
	UBYTE	Gender;
	UBYTE	Quiet;
	UBYTE	Public;
	UBYTE	PermaRoom;

	char	Name[31];
	char	Topic[31];
	char	Entry[81];	/* Messages for entry, exit */
	char	Exit[81];

	UBYTE	Scribe;			/* are we writing a scribe? */
	UBYTE	MCIScribe;		/* remove MCI's ?? */
	char	SName[78];		/* name of current scribe file */

	UBYTE	Channel;
	UBYTE	LinkID;

	UBYTE	Invited[300];		/* 1 bit each for 2400 users */

	short	MaxUsers;
	UBYTE	Chaos;
	UBYTE	LurkTimeout;

	short	NoiseSet;
};

struct Room {
	struct	RoomConfig rc;

	long	Pointer;
	long	Users;			/* how many users are in this room */

	short	pSBuff;			/* pointer into Scribe Buffer */

	UBYTE	Buffer[15000];		/* Actual I/O buffer (Q) */

	UBYTE	SBuff[1024];		/* buffer for Scribe */
};

/* LONG aligned.  Total length == 304 */

struct RoomUser {
	long	MyPointer;		/* 0 */
	long	UID;			/* 4 */
	short	Room;			/* 8 */

	char	Entry[71];		/* 10 */
	char	Exit[71];
	char	Topic[31];
	char	Alias[21];
	char	Noise[71];
	UBYTE	Action;
	UBYTE	DefRoom;
	UBYTE	Quit;
	UBYTE	AllowReal;

	UBYTE	expansion[25];		/* 278 */
};

struct SelectType {			/* Select list */
	long	Size;

	char	Title   [32];
	char	Location[96];		/* physical path to file */
	short	Base;			/* physical subboard # */

	UBYTE	SDownFiles;		/* Which accounting schedule ? */
	UBYTE	SDownBytes;

	UBYTE	temp_rem;		/* DS from DS list!! */
	UBYTE	CDROM;			/* Copy to CDROMpath before download? */
	BYTE	FreeStuff;
	UBYTE	AutoKill;		/* 1==mark for amaint deletion when downloaded
					   2==Yank, 3==DELETE when unselected */

	long	ByteDownload;		/* credits to subtract when this */
	short	FileDownload;		/* item is downloaded ... */
					/* makes * faster by being here */
	struct	IsDate ADate;		/* utility ... AO adopt dates? */

	long	Number;			/* item number */
};

struct BaseUser {
	long	UID;			/* ==IDNumber if user's been here */

	BYTE	Joined;			/* -1 dropped, 0 sub.def., 1 joined  */
	BYTE	Invited;		/* -1 denied,  0 sub.def., 1 invited */
	BYTE	Sort;
	UBYTE	scan_flags;		/* used to 'mark' subboards */

	struct	IsDate LastMNew;
	struct	IsDate MNewDate;

	long	Created;		/* when sub was created */

	char	Alias[22];
	BYTE	Carbons;		/* -1 no, 0 sub.def., 1 yes */
	UBYTE	MNewSave;

	short	tome0;
	short	tome1;			/* while "online" */

	char	expansion[12];
};

struct HeaderType {
	struct	IsDate ShowDate;	/* for display */
	struct	IsDate EditDate;	/* last edited */
	struct	IsDate PostDate;

	short	ByAccount;

	char	NetAddress[52];		/* May be shortened */
	long	UUCPnumber;

	char	other[16];

	char	By[36];			/* Real name or alias */
	char	ByUser[24];		/* Handle */
	long	ByID;

	char	To[36];
	char	ToUser[24];
	long	ToID;

	UBYTE	ByNotParanoid;		/* allow showing real name? */
	UBYTE	ByAnonymous;
	UBYTE	ToAnonymous;
	UBYTE	Private;

	UBYTE	Imported;
	UBYTE	ByAlias;		/* is BY an alais for ByUser? */
	UBYTE	ToNotParanoid;		/* allow showing real name? */
	UBYTE	ToAlias;		/* is TO an alias for ToUser? */

	short	ToAccount;		/* for SendToMailBox() */

	ULONG	Magic;			/* to ensure alignment   */
	long	Number;			/* unique message number */

	long	Text;
	long	TextLen;

	char	Organ[31];

	UBYTE	Received;

	short	unknown;

	long	Next;
	long	Previous;
};

struct	OldMessageType {
	long	ItemNumber;
	long	ResponseNumber;
	long	ByID;
	long	ToID;

	struct	IsDate PostDate;

	UBYTE	Imported;
	UBYTE	IsFile;
};

struct	MessageType3 {
	long	ItemNumber;		/* item serial ID# */
	long	Seek;			/* position in text */
	long	ByID;
	long	ToID;
	long	Number;			/* serial ID# */

	struct	IsDate PostDate;

	UBYTE	Imported;
	UBYTE	IsFile;
};

struct	OldItemType {
	long	Number;
	long	Response1;		/*  1st valid response # */
	long	Responses;		/* last valid response # */

	char	Title[72];
	struct	IsDate PostDate;	/* new item ?      */
	struct	IsDate RespDate;	/* new responses ? */

	long	ByID;
	long	ToID;

	long	Size;
	long	Downloads;
	char	Info[84];
	struct	IsDate UsedDate;	/* new downloads ? */
	short	Part;			/* which partition? */

	short	fcredit;	/* Remember the Accounting credits */
	short	bcredit;	/* and File/Byte credits given for */
	UBYTE	fratio;		/* for this item so they can accurately */
	UBYTE	bratio;		/* be removed if un-validated or killed */

	UBYTE	Private;
	UBYTE	Killed;

	UBYTE	PleaseKill;
	UBYTE	Frozen;
	UBYTE	Free;
	UBYTE	Protected;

	UBYTE	Favorite;
	UBYTE	Validated;
	UBYTE	Finished;
	UBYTE	Described;

	UBYTE	Transformed;		/* has the script been ran? */
	UBYTE	MissingPost;
	UBYTE	MissingFile;
	BYTE	Integrity;		/* 0 == not tested
					   1 == passed
					  -1 == failed
					   2 == not testable */

	short	ByAccount;		/* for AddCredits */

	short	order;			/* for New Files Scan */
	short	base;
	UBYTE	WasMoved;

	BYTE	expand[57];
};

/* size = 34 */

struct	ItemHeader {
	long	Number;
	long	Size;			/* ==0-->POST, not file */
	long	Responses;		/* How many responses */

	struct	IsDate PostDate;	/* new item ?      */
	struct	IsDate RespDate;	/* new responses ? */

	UBYTE	TitleSort[9];
	UBYTE	Killed;
};

struct	ItemType3 {
	char	Title[42];		/* 0 */
	short	ByAccount;		/* 42 Uploader's account # */

	long	ByID;			/* 44 */
	long	ToID;			/* 48 */

	struct	IsDate UsedDate;	/* 52 new downloads ? */
	short	Part;			/* 58 which partition? */

	long	Downloads;		/* 60 */

	long	ByteCharges;		/* 64 Accounting system charges to the */
	short	FileCharges;		/* 68 uploader ... */

	UBYTE	Private;		/* 70 */
	UBYTE	DLnotifyULer;		/* 71 */

	UBYTE	PleaseKill_NOT;		/* 72 */
	UBYTE	Frozen;			/* 73 */
	UBYTE	Free;			/* 74 */
	UBYTE	delta;

	UBYTE	Favorite;		/* 76 */
	UBYTE	Validated;		/* 77 */
	UBYTE	Finished;		/* 78 */
	UBYTE	Described;		/* 79 */

	UBYTE	Transformed;		/* 80 has the script been ran? */
	UBYTE	PurgeKill;		/* 81 delete file when purged? */
	UBYTE	MissingFile;		/* 82 offline? */
	BYTE	Integrity;		/* 83	0 == not tested
						1 == passed
					       -1 == failed
						2 == not testable */

	UBYTE	AutoGrab;		/* 84 ASCII-Grab when read */
	UBYTE	PurgeStatus;		/* 85   0 == Auto
						1 == @DL
						2 == Query@DL
						3 == @Amaint
						4 == Protected */

	struct	IsDate ShowDate;

	UBYTE	nada[7];		/*  86 reserved */
	UBYTE	VirusChecked;		/*  99 */
	UBYTE	override;		/* 100 */
	UBYTE	loaded;			/* 101 is in memory? */

	short	FilePayBack;		/* 102 after each download */
	long	BytePayBack;		/* 104 Credits to award the uploader */

	long	VoteLink;		/* 108 vote serial# */
	long	SizeTemp;		/* 112 used privately by AT */

	long	ByteRewards;		/* 116 # of bytes awarded so far */
	long	FileRewards;		/* 120 # of files awarded so far */

	long	ByteDownload;		/* 124 Number of byte cred to download */
	short	FileDownload;		/* 128 Number of file cred to download */

	struct	IsDate DLableFrom;	/* 130 */
	struct	IsDate DLableTo;	/* 136 */
	struct	IsDate PurgeOn;		/* 142 */

	long	BestCPS;		/* 148 best download rate this file */

	long	First;			/* 152 pointer to post in _Text */
	long	Last;			/* 156 pointer to last response in _Text */

	long	InfoX;			/* 160 pointer into _Short */
	long	InfoLen;		/* 164 */
};

struct FreeType
{
	long	start;
	long	length;
	struct	FreeType *next;
};

/* Total length == 332 */

struct OldSubboardType {
	char	Title[31];		/* 0  */
	char	SubDirName[21];		/* 31 */
	char	Part0Path [40];		/* 52 */
	long	Parts;			/* 92 partitions 0-31 now allowed */

	long	SubOpIDs [6];		/* 96 */

	short	MRewardBytes;		/* 120 % of byte  size to award */
	short	NRewardFiles;		/* 122 # of file creds to award
					       uploader each time his file is
					       downloaded */

	struct	FreeType *free;		/* 124 */
	long	nf;			/* 128 */
	long	nfmax;			/* 132 */

	short	SubOpAccs[6];		/* 136 */

	char	Filler[4];		/* 148 shown during SCAN */

	short	fdelta;			/* 152 */
	short	fowner;			/* 154 current owner of "free" */

	UBYTE	Direct;			/* 156 Disk-exchange type */
	UBYTE	Vote;			/* 157 May non-sysops add topics? */

	short	FreeDays;		/* 158 when files are auto-free */

	UBYTE	Subdirectory;
	UBYTE	Closed;

	long	UnionFlags;		/* 162 Must have all of these flags */
	long	Access;			/* 166 Groups which may ENTER */
	long	PostAccess;		/* 170 Groups which may Post  */
	long	RespondAccess;		/* 174 ... Respond */
	long	UploadAccess;		/* 178 ... Upload */
	long	DownloadAccess;		/* 182 ... Download */
	long	ComputerTypes;		/* 186 */
	long	Hours;			/* 190 restricted entry hours */
	long	HourUnionFlags;		/* 194 */
	long	HourAccess;		/* 198 groups which may Enter during Hours*/
	long	Baud;			/* 202 minimum Baud rate to enter */
	long	BaudHours;		/* 206 hours to enforce min baud rate */

	UBYTE	Gender;			/* 210 0 or 'M', 'F' */
	UBYTE	AllowAliases;
	UBYTE	OnlyOnce;		/* 212 */
	UBYTE	Youngest;		/* 213 */

	UBYTE	EXTRA;

	UBYTE	Oldest;			/* 215 */

	struct	IsDate	Sent;		/* 216 FIDO to track new messages */

	short	MDownBytes;		/* 222 % of download bytes to subtract */
	short	NDownFiles;		/* 224 # of files to subtract per download */
	short	MTimeCredit;		/* 226 % of time to give back for uploads */

	long	MinFreeBytes;	/* 228 to upload in this subboard */
	short	SubSort;	/* 232 */
	short	InactiveDays;	/* 234 */

	UBYTE	RealNames;	/* 236 use real names instead of Handles? */
	UBYTE	Addressees;
	UBYTE	Anonymous;
	UBYTE	override;	/* 239 how to set the use defaults flags in
					newly uploaded items */

	UBYTE	Verification;	/* 240 */
	UBYTE	DupCheck;
	UBYTE	ShowUnvalidated;
	UBYTE	NoMCI;

	UBYTE	PrivateArea;	/* 244 0/1 No/Yes ... 2==FORCE */
	BYTE	SDownFiles;	/* Which download charge schedules? */
	BYTE	SDownBytes;
	UBYTE	TestImmediate;	/* test at upload time */

	UBYTE	TransImmediate;	/* 248 run script at upload time */
	UBYTE	AutoAdopt;	/* Adopt Orphans at a-maint */
	UBYTE	WeedResponses;	/* Use inactive days against indiv responses */
	UBYTE	NoSignatures;

	UBYTE	NoReadCharges;	/* 252 opt to short circuit accounting system */
	UBYTE	NoWriteCharges; /* 253 for reading/writing messages */

	long	Arcs;		/* 254 which Arcs in BBSARC are allowed uploaded*/

	short	UpRatioBytes;	/* 258 which set of file/byte ratios to use? */
	short	UpChargeBytes;	/* 260 which set of charges to use?          */
	short	UpRatioFiles;	/* 262 */
	short	UpChargeFiles;	/* 264 */

	UBYTE	Invitation;	/* 266 */
	UBYTE	UserMustJoin;
	UBYTE	DeleteOwn;
	UBYTE	Marker;		/* 0 = Base, 1 = UDBASE, &128 = Killed */

	long	Transforms;	/* 270 which BBSARCS to transform */

	char	NetPath[20];	/* 274 */
	UBYTE	NetNumber;	/* 294 */ 
	UBYTE	Locked;		/* 295 Does AMAINT have a lock here? */
	UBYTE	DLnotifyULer;	/* 296 */

	char	Transform[5];	/* 297 */

	UBYTE	QWKReplies;	/* 302 */
	BYTE	SBaseUse;	/* 303  which BBSCHARGE rate to use? */
	UBYTE	CarbonCopy;	/* 304 */
	UBYTE	CDROM;		/* 305 copy to hard drive before download? */

	struct	IsDate LastUpload;
	struct	IsDate LastMessage;

	short	Users;		/* how many people are in this sub
				   REAL TIME (not # of members) */

	long	Created;	/* for membership determination */

	short	Parent;		/* pointers to other subboard #s */
	short	Child;
	short	Next;

	short	TimeLock;	/* 330 minutes online before available */
};

struct NewSubboardType {
	char	Title[31];		/* 0  */
	char	SubDirName[21];		/* 31 */
	char	DataPath  [40];		/* 52 */
	long	Parts;			/* 92 partitions 0-31 now allowed */

	long	SubOpIDs [6];		/* 96 */

	short	MRewardBytes;		/* 120 % of byte  size to award */
	short	NRewardFiles;		/* 122 # of file creds to award
					       uploader each time his file is
					       downloaded */

	struct	FreeType *free0;	/* 124 */
	long	nf_obs;			/* 128 */
	long	nfmax_obs;		/* 132 */

	short	SubOpAccs[6];		/* 136 */

	char	Filler[6];		/* 148 shown during SCAN */

	UBYTE	fdelta;			/* 152 */
	UBYTE	floaded;		/* 154 */

	UBYTE	Direct;			/* 156 Disk-exchange type */
	UBYTE	Vote;			/* 157 May non-sysops add topics? */

	short	FreeDays;		/* 158 when files are auto-free */

	UBYTE	Subdirectory;
	UBYTE	Closed;

	long	UnionFlags;		/* 162 Must have all of these flags */
	long	Access;			/* 166 Groups which may ENTER */
	long	PostAccess;		/* 170 Groups which may Post  */
	long	RespondAccess;		/* 174 ... Respond */
	long	UploadAccess;		/* 178 ... Upload */
	long	DownloadAccess;		/* 182 ... Download */
	long	ComputerTypes;		/* 186 */
	long	Hours;			/* 190 restricted entry hours */
	long	HourUnionFlags;		/* 194 */
	long	HourAccess;		/* 198 groups which may Enter during Hours*/
	long	Baud;			/* 202 minimum Baud rate to enter */
	long	BaudHours;		/* 206 hours to enforce min baud rate */

	UBYTE	Gender;			/* 210 0 or 'M', 'F' */
	UBYTE	AllowAliases;
	UBYTE	PurgeStatus;		/* 212 */
	UBYTE	Youngest;		/* 213 */

	UBYTE	EXTRA;

	UBYTE	Oldest;			/* 215 */

	struct	IsDate	Sent;		/* 216 FIDO to track new messages */

	short	MDownBytes;		/* 222 % of download bytes to subtract */
	short	NDownFiles;		/* 224 # of files to subtract per download */
	short	MTimeCredit;		/* 226 % of time to give back for uploads */

	long	MinFreeBytes;	/* 228 to upload in this subboard */
	short	SubSort;	/* 232 */
	short	InactiveDays;	/* 234 */

	UBYTE	RealNames;	/* 236 use real names instead of Handles? */
	UBYTE	Addressees;
	UBYTE	Anonymous;
	UBYTE	override;	/* 239 how to set the use defaults flags in
					newly uploaded items */

	UBYTE	Verification;	/* 240 */
	UBYTE	DupCheck;
	UBYTE	ShowUnvalidated;
	UBYTE	NoMCI;

	UBYTE	PrivateArea;	/* 244 0/1 No/Yes ... 2==FORCE */
	BYTE	SDownFiles;	/* Which download charge schedules? */
	BYTE	SDownBytes;
	UBYTE	Test;		/* test at upload time */

	UBYTE	Transform;	/* 248 run script at upload time */
	UBYTE	AutoAdopt;	/* Adopt Orphans at a-maint */
	UBYTE	WeedResponses;	/* Use inactive days against indiv responses */
	UBYTE	NoSignatures;

	UBYTE	NoReadCharges;	/* 252 opt to short circuit accounting system */
	UBYTE	NoWriteCharges; /* 253 for reading/writing messages */

	long	Arcs;		/* 254 which Arcs in BBSARC are allowed uploaded*/

	short	UpRatioBytes;	/* 258 which set of file/byte ratios to use? */
	short	UpChargeBytes;	/* 260 which set of charges to use?          */
	short	UpRatioFiles;	/* 262 */
	short	UpChargeFiles;	/* 264 */

	UBYTE	Invitation;	/* 266 */
	UBYTE	UserMustJoin;
	UBYTE	DeleteOwn;
	UBYTE	Marker;		/* 0 = Base, 1 = UDBASE, &128 = Killed */

	long	Transforms;	/* 270 which BBSARCS to transform */

	long	ExportTo;	/* 274 */

	char	NothingNow[8];	/* 278 */

	long	count;		/* 286 replaces the _COUNT file */
	long	hiwater;	/* 290 */

	UBYTE	NetNumber;	/* 294 */ 
	UBYTE	Locked;		/* 295 Does AMAINT have a lock here? */
	UBYTE	DLnotifyULer;	/* 296 */

	char	TransformTo[5];	/* 297 */

	UBYTE	QWKReplies;	/* 302 */
	BYTE	SBaseUse;	/* 303  which BBSCHARGE rate to use? */
	UBYTE	CarbonCopy;	/* 304 */
	UBYTE	CDROM;		/* 305 copy to hard drive before download? */

	struct	IsDate LastUpload;
	struct	IsDate LastMessage;

	short	Users;		/* how many people are in this sub
				   REAL TIME (not # of members) */

	long	Created;	/* for membership determination */

	short	Parent;		/* pointers to other subboard #s */
	short	Child;
	short	Next;

	short	TimeLock;	/* 330 minutes online before available */

	char	ZeroPath[48];	/* 332 */
	char	Origin  [60];	/* 380 */

/* if more room is needed, I could put all of this into a pointer to a
   'sub-info' structure */

	short	MaxItems;	/* 440 */
	UBYTE	Persist;	/* 442 */
	UBYTE	Delay;		/* 443 */

	struct	ItemType3 *Item;
	struct	ItemHeader *ihead;
	struct	MessageType3 *NewMess;
	struct	SignalSemaphore *sem;

	long	nm;		/* 460 # of _Messages */
	short	rn;		/* 464 # of _Items */
	short	AllocItems;
	short	nNewMess;

	UBYTE	resetIP;	/* 470 */

	char	more_stuff[17];	/* 471 */
};

/* LONG aligned.  Total length == 348 */

struct SerPort {
	long	unit;		/* 0 */
	long	flags;		/* 4 */

	short	idlebaud;	/* 8 */
	short	escape;
	short	answerpause;	/* 12 */
	short	seconds;

	char	name[21];	/* 16 */
	char	init1[50];	/* 37 */
	char	ppass[11];	/* 87 */
	char	init2[50];	/* 98 */

	ULONG	TrapHours;	/* 148 */
	ULONG	MailHours;

	UBYTE	extra[3];	/* 156 */

	char	hangup[21];	/* 159 */
	char	dialout[21];
	char	answer[21];
	char	offhook[21];
	char	terminal[50];
	char	callerid[11];
	char	ring[9];
	char	connect[11];

	char	termlink[21];	/* 324 */

	UBYTE	other[2];

	UBYTE	null;		/* local terminal, no clear line */
};

/* LONG aligned.  Total length == 20 */

struct	PortConfig {
	UBYTE	online;			/* port loaded or not */
	UBYTE	open;			/* 1==perm, 2==call only */

	UBYTE	check;			/* menus */
	UBYTE	idle;			/* clock, SAG, SAM ? */

	UBYTE	offline;		/* port loaded, but modem offline */
	UBYTE	not_used;

	UBYTE	bplanes;
	UBYTE	lace;			/* 0=none, 1=24 line, 2=49 line */

	USHORT	sizes[8];		/* workbench window & zoom size */
};

/* LONG aligned.  Total length==92 */

struct Privs {				/* each user&group has one */
	long	MBaseFlags;		/* 580 */
	long	FBaseFlags;		/* 584 Can user reach which subs ? */
	long	LBaseFlags;		/* 588 Gfiles/Pfiles flags */

	ULONG	ABits;			/* 592 see bitdefs above */

	long	DailyDownBytes;		/* 596 */
	long	DailyUpBytes;		/* 600 */
	long	XFreeBytes;		/* 604 !!! NOT USED ANYMORE !!! */
	long	XFreeFiles;		/* 608 SEE GLOBAL DEFAULT IN CONFIG1 */

	short	Calls;			/* 612 */
	short	CallMinutes;		/* 614 */
	short	DailyMinutes;		/* 616 */
	short	DailyDownloads;		/* 618 per 24 hr DAY */
	short	DailyUploads;		/* 620 same */
	short	Messages;		/* 622 */
	short	Feedbacks;		/* 624 */
	short	EditorLines;		/* 626 */
	short	Idle;			/* 628 */
	short	MaxMailKBytes;		/* 630 before mail-receive is limited */
	short	PurgeDays;		/* 632 */

	UBYTE	FileRatio;		/* 634 */
	UBYTE	ByteRatio;
	UBYTE	FileRatio2;
	UBYTE	ByteRatio2;
	UBYTE	FileRatio3;
	UBYTE	ByteRatio3;

	ULONG	ABits2;			/* 640 now gives 64 flags! */

	short	SigLines;		/* 644 how many lines per signature */

	UBYTE	AllowAliases;		/* 646 new tri-state variables */
	UBYTE	DeleteOwn;
	UBYTE	Anonymous;
	UBYTE	PrivateArea;

	short	DailyPfileMinutes;	/* 650 */
	short	LogToMail;		/* account # to send log */

	long	LogFlags;		/* 654 */

	UBYTE	CallBack;		/* 658 Tri-state */
	UBYTE	TermLink;		/* 659 */
	UBYTE	CallerID;		/* 660 */
	UBYTE	PageSysop;		/* 661 */

	char	exp[ 6 ];		/* 662 */

	short	Alias;			/* 668 */
	short	Dictionary;		/* 670 */
};

struct AccessGroup {
	char	Name[32];		/*   0 */
	struct	Privs DefPrivs;		/*  32 */

	short	ExpireDays;		/*  32+92 */
	UBYTE	ExpireAccess;		/*  32+94 */

	char	exp[29];
};

/* FILE: BBS.UKEYS3
   Alpha "key" for sortable data.  Phone# for caller-id, Handle/RealName
   for user addressing/mail/logon/etc.
*/

struct KeyElement3 {
	char	UUCP[8];
	long	IDNumber;		/* high order byte ALWAYS 0 */
	long	phone1, phone2;		/* data phone's area code & local # */
	char	Handle   [21];
	char	RealName [26];
	UBYTE	PName;
};

struct OldKeyElement {
	char	Handle[22];		/* actual user handle */
	short	HandleX;		/* account number where found */
};

struct MarkType {
	short	sub;			/* The new system can uniquely  */
	long	ItemNumber;		/* identify a message with this */
	long	Seek;			/* information.  Used for Z/ZG. */
	long	Number;
};

struct TransType {
	struct	TransType *next;	/* linked list */

	long	lines;
	long	bytes;

	UBYTE	**mm;			/* pointers to BBSMENU lines */
	UBYTE	**bm;			/* pointers to BBSTEXT lines */
	short	menustart[50];		/* */
	short	menulen  [50];
};

struct NoiseType {
	struct	NoiseType *next;

	long	lines;
	long	bytes;

	char	**nn;
	char	*args;
};

struct MainPort {			/* common, public to all ports */
	struct	MsgPort mp;		/*   0 standard EXEC message port  */
	char	portName[22];		/*  34 */

	char	LastOn[32];		/*  56 */

	long	Nums[5];		/*  88 [0] # of physical user accounts
					       [1] # of in-use user accounts
					       [2] last assigned unique ID Number
					       [3] # of calls to system TOTAL
					       [4] # of calls now logged */

	ULONG	SAG[2][72];		/* 108 activity graph    */
	long	SAM[5][15];		/* activity monitor  */
	struct	IsDate SAMDate[6];	/* */

	long	um[10];			/* for C programmer use.  You can */
					/* (typecast) them as you need to. */
					/* Initially at bootup == 0 */

	struct	Window		*PWindow;
	struct	TransType	*FirstTT;
	struct	NewSubboardType *Subboard;
	struct	LinkPort	*link;	/* all-linked port users! */
	struct	KeyElement3	*Key;	/* alpha order, etc. */
	short	*IName;			/* Indexs into KeyElement3 array */
	short	*IPhone;
	char	*SubTitle;
	char	**Noise_00;		/* Conference NOISE memory */
	char	*NArg_00;		/* */

	long	ns;			/* number of subs in memory */

	UBYTE	toss_up;
	UBYTE	server_up[7];

	long	YankCount;
	long	OpenPfiles;		/* */

	struct	NoiseType	*FirstNT;

	long	ClickMaint;		/* can we use local mouse maint? */
	long	Verbose;		/* do not redirect to NIL: for debug */

	long	s1;			/* version of CONTROL */
	long	s2;
	long	s3;			/* serial number of owner 0==DEMO */

	struct	BitMap  ClockBrush;	/* 40 bytes */
	struct	SignalSemaphore SEM[19];	/* for critical secs. */
	struct	AccessGroup     AGC[32];	/* BBS.ADATA stuff    */

	struct	Config1 gc;

	char	SysPassword[6][21];
	char	IDQ[6];			/* obs */

	struct	PortConfig  pc   [101];	/* For CONTROL, remembers things */
	struct	PortData   *PortZ[100];	/* pointers to each port's PortData struct */
	struct	Room       *CRoom[100];	/* pointers to ROOM structures  */

	struct	ULock *FirstULock;	/* Account Locking ... ! */
	short	LastTickHour;		/* control is ticking ... */
	short	LastTickMinute;

	short	HiPort;
	short	nPorts;

	char	regto[32], regco[32];
};

/* LONG Aligned, total length==672 */

struct UserData {			/* each user account */
	long	IDNumber;		/* 0 unique (serial) ID # */
	char	Handle[21];		/* 4 */
	char	RealName[26];		/* 25 */
	char	Address[31];		/* 51 */
	char	CityState[31];		/* 82 */
	char	ZipCode[11];		/* 113 */
	char	Country[8];		/* 124 */
	char	PhoneNo[17];		/* 132 Euro 8-7 format POSSIBLE!! */
	char	PassWord[15];		/* 149 */
	char	Comments[34];		/* 164 */
	char	Macro[3][36];		/* 198 user's defined macro keys */
	char	Organ[31];		/* 306 */
	UBYTE	PhoneVerified;		/* 337 */

	char	not_used[4];		/* 338 */

	struct	IsDate Birthdate;	/* 342 */
	struct	IsDate FirstCall;	/* 348 */
	struct	IsDate LastCall;	/* 354 */

	struct	IsDate NO_LONGER_USED;	/* 360 */

	struct	IsDate ConnectDate;	/* 366 */
	struct	IsDate YankDate;	/* 372 */

	BYTE	Access;			/* 378 */
	BYTE	ExpireAccess;		/* 379 */

	short	PfileTimeToday;

	short	not_used2;

	BYTE	TextSet;		/* 384 which BBSTEXT/MENU translation */
	BYTE	TimeForm;
	BYTE	MoreMode;
	BYTE	LineFeeds;
	BYTE	ObsParanoia;
	BYTE	TimeZone;
	BYTE	Colors;			/* can user display ANSI colors? */
	BYTE	Tabs;			/* can user do ANSI tabs? */
	BYTE	ANSI;			/* 392 */
	BYTE	Sex;
	BYTE	AutoMore;		/* More? and CLS between messages? */
	BYTE	TermType;
	BYTE	HelpLevel;		/* 396 */
	BYTE	CompType;
	BYTE	DefProtocol;		/* 398 */
	BYTE	TermWidth;
	BYTE	TermLength;		/* 400 */
	BYTE	MailBoxOpen;		/* 401 */
	short	MailForward;		/* 402 */

	long 	TotalCalls;		/* 404 */
	long	PubMessages;		/* 408 */
	long	PriMessages;		/* 412 */
	long	UpBytes;		/* 416 KILO bytes */
	long	UpFiles;		/* 420 */
	long	DownBytes;		/* 424 KILO bytes */
	long	DownFiles;		/* 428 */

	long	FileCredits;		/* 432 CREDITS used now! */
	long	ByteCredits;		/* 436 */
	long	TimeCredits;		/* 440 */

	long	Balance;		/* 444 */
	long	NetCredits;		/* 448 */
	long	DoorPoints;		/* 452 */

	long	DayUpBytes;		/* 456 */
	long	DayDownBytes;		/* 460 */
	short	DayUpFiles;		/* 464 */
	short	DayDownFiles;		/* 466 */

	short	CallsToday;		/* 468 */
	short 	TimeToday;		/* 470 */
	short	LogonAttempts;		/* 472 */

	USHORT	BaudRate;		/* 474 */
	USHORT	HighBaud;		/* 476 */

	char	UUCP[11];		/* 478 */

	UBYTE	PName;			/* 489 flags, are these data     */
	UBYTE	PAge;			/* items to be kept private? */
	UBYTE	PAddress;		/* street address only */
	UBYTE	PVoice;			/* phone numbers */
	UBYTE	PData;

	char	Banner[43];		/* 494 */

	char	VoiceNo[17];		/* 537 Euro 8-7 format POSSIBLE!! */

	short	YankCount;		/* 554 */

	struct	IsDate ExpireDate;	/* 556 */
	struct	IsDate FNewDate;	/* 562 */

	char	more[7];		/* 568 reserved */

	UBYTE	YankCR;			/* 575 add CR's to yank text? */
	UBYTE	YankArc;		/* which archiver to use? */
	BYTE	DefEditor;		/* 577 */
	BYTE	AutoHide;
	BYTE	PrivsSet_NOT;		/* 579 */
	struct	Privs MyPrivs;		/* 580 THESE ACTUALLY USED, not AGC */
};

struct OLMHeader {
	char	ByUser[26];		/* actual handle only */
	short	ByAccount;
	short	Port;

	UBYTE	system;
	UBYTE	broadcast;

	UBYTE	other[22];
};

/* length == 296 */

struct MailHeader {			/* NEW 2.0 for mail, etc */
	struct	IsDate  Date;
	struct	IsDate	EDate;		/* for EXPIRATION setting */
	short	EDays;
	char	Subject [81];		/* Ahh, nice and roomy */
	char	From    [27];

	long	Number;

	short	ByAccount;
	long	ByID;

	char	To	[28];		/* originals, if was Sent/forwarded */
	long	ToID;
	short	ToAccount;

	UBYTE	Receipt;		/* return-receipt ?? */
	UBYTE	Return;			/* return original message?? */

	UBYTE	HasOriginal;		/* is original attached to this one? */
	UBYTE	File;			/* for File Mail */
	UBYTE	Anonymous;		/* From an anonymous user? */

	char	Route	[41];		/* for Bulk Mail, list of recips */
	short	Recipients;		/* number of recipients ... */
	UBYTE	Party;			/* Allow party responses? */

	long	Length;
	long	CheckSum;		/* must be 0 */

	long	Item;			/* if this message is repeated */
	long	Seek;			/* somewhere on the system     */
	short	Base;			/* this tells where !!!        */

	char	NetAddress[52];		/* may be shortened */

	UBYTE	Alias;
	UBYTE	Replied;
	UBYTE	Received;
	UBYTE	NoEdits;

	struct	IsDate  ShowDate;
};

/* LONG aligned.  Length==172 */

struct Subdirectory {
	long	SubOpIDs [10];
	short	SubOpAccs[10];

	long	Access;
	long	UnionFlags;

	long	Hours;
	long	HourUnionFlags;
	long	HourAccess;

	long	Baud;
	long	BaudHours;

	short	Youngest;
	short	Oldest;

/* when the Download command is used on a Gfile */

	short	NDownFiles;	/* # of download files to subtract */
	BYTE	SDownFiles;	/* Which download charge schedules? */
	BYTE	SDownBytes;

/* per-usage charge schedules from BBSCHARGESX.A files */

	long	unused;

	UBYTE	Gender;		/* 0 or 1, 2 */
	UBYTE	SBaseUse;	/* accounting schedule for this area */
	UBYTE	Closed;

	UBYTE	exp[61];

	long	ComputerTypes;

	short	TimeLock;	/* minutes online before available */
	short	MDownBytes;	/* % of download Bytes to subtract */
};

struct ListItem {
	short	Version;		/* should be 225 */
	char	Path       [54];	/*   2 */
	char	Description[64];	/*  56 */
	char	Arguments  [64];	/* 120 */

	long	Access;			/* 184 */
	long	UnionFlags;		/* 188 */

	struct	IsDate Date;		/* 192 */

	short	SCharge;		/* 198 accounting use-of-item charge */

	UBYTE	Subdirectory;		/* 200 */
	UBYTE	FifoFlags;		/* 201 1==RAW startup */
	UBYTE	ReadFlags;		/* internal only */
	UBYTE	Environ;

	UBYTE	DailyPfileTime;		/* 204 subtract from daily pfile time? */
	UBYTE	Disabled;		/* 205 */
	UBYTE	Type;			/* 206 not really used yet */
	UBYTE	PurgeKill;		/* 207 Delete item when purged? */

	UBYTE	Expansion[14];		/* 208 */

	struct	IsDate PurgeDate;	/* 222 */
};

/* Length== 176 */

struct OldListItem {
	char	Path       [51];	/* 0 */
	char	Description[61];	/* 51 */
	char	Arguments  [20];

	struct	IsDate Date;		/* 132 */

	long	Access;			/* 138 un-al */

	UBYTE	Subdirectory;		/* 142 */
	UBYTE	DaysToKeep;
	UBYTE	ReadFlags;		/* internal only */
	UBYTE	Environ;

	short	SCharge;		/* 146 override a subdir schedule */

	long	UnionFlags;

	UBYTE	DailyPfileTime;		/* subtract from daily pfile time? */

	UBYTE	Expansion[23];		/* 148 */
};

/* LONG aligned.  Length==48 */

struct CPort {				/* setup when a C pfile is ran */
	struct	MsgPort  mport;		/* 0 */
	short	align;
	struct	MainPort *myp;		/* 36 pointer to MainPort */
	struct	PortData *zp;		/* 40 pointer to my PortData */

	UBYTE	ack;			/* 44 tell CNet we've arrived */
	UBYTE	align2[3];
};

struct CMessage {			/* passed between C pfile and C-Net */
	struct	Message cn_Message;	/* 0 communication stuff */
	ULONG	arg1;			/* 20 */
	ULONG	arg2;
	ULONG	arg3;
	ULONG	arg4;
	ULONG	result;
	UBYTE	command;		/* 40 which command? see programming doc */
	UBYTE	align[3];
};

struct Event {
	long	Start;
	long	End;

	short	Event;
	char	Args[82];
	long	IntArgs;

	struct	Event *Next;
};

struct ConfigEvent {
	char	Ports[40];

	UBYTE	Days;		/* if this is 0, use Date */
	char	Date[7];

	short	Time,
		Duration,
		Interval,
		Iterations;

	short	Event;
	char	Args[81];

	UBYTE	Type;
};

/* all LONG's 4 byte aligned */

struct PortData {			/* each opened ports global vars */
	struct	IsDate	Today;		/* 0 what time is it right now? */
	struct	IsDate	VConnectDate;	/* 6 Virtual connect...eg 12am */

	short	id,			/* 12 current user1 account # */
		InPort,			/* 14 which port are we in??  */
		Age,			/* 16 how old am I ? */
		CurrentCPS,		/* 18 CPS rate.  240 == 2400 baud */
		OnType,			/* 20 type of logon  */
		OffType,		/* 22 */
		CallMessages,		/* 24 */
		CallFeedbacks,		/* 26 */
		TimeLeft,		/* 28 TENTHS of minutes left */
		TimeIdle,		/* 30 TENTHS of minutes without activity */
		TimeOnLine,		/* 32 TENTHS of minutes since logon */
		TimeCredit,		/* 34 TENTHS awarded for uploads, etc */
		LastTSR,		/* 36 */
		Carrier;		/* 38 Is there a carrier or Local? */

	char	*AName;			/* 40 ptr to access group name */
	long	SysMaint,		/* 44 */
		LocalTime,		/* 48 current area sur-time minutes */
		LocalCharge,		/* 52 current area sur-charge per minute */
		LastTime,		/* 56 */
		ServLen,		/* 60 seconds in-between Service() calls */
		Caller;			/* 64 which caller am I? */

	struct	UserData	user1,	/* 68 */
				user2;	/* 740 actual user structs */

	short	MyMail;			/* 1412 counters for user currently online */
	short	MyNewMail;		/* 1414 */
	short	bn;			/* 1416 current sub # */

	char	CallerID [18];		/* 1418 Received from Supra, prev connect */
	short	CIDmatch [ 8];		/* Accounts which MATCH, [0] will be current user  */

	UBYTE	expansion[18];		/* 1436 */

	UBYTE	PAccess,		/* 1470 */
		RAccess,
		DAccess,
		UAccess;

	char	fsuff[8],		/* 1474 more header stuff */
		fcore[32],		/* 1482 */
		Title[80],		/* 1514 Screen/Window */
		lastby[36];		/* 1594 */

	UBYTE	IUCBits[100];		/* 1630 Hidden, Muffled, Monitor, ChatReq */

	char	*MyDoing;		/* 1730 */
	struct	Room *CRoom;		/* 1734 which room am I in ? */
	struct	RoomUser *CUser;	/* 1738 where is my RoomUser struct */

	UBYTE	Dumped,			/* 1742 auto-kickoff flag	*/
		OLMWaiting,		/* 1743 OLM waiting		*/
		HideAll,		/* 1744 Hidden from ALL?	*/
		MuffAll,		/* 1745  Muffling ALL?	*/
		MonitorAll,		/* 1746 Monitoring ALL?	*/
		OnLine,			/* 1747 */
		nPdepth,		/* 1748 How many pfiles a port is running */
		ChatCode,		/* 1749 who's chatting with whom */
		WantToOpen,		/* 1750 */
		getout,			/* 1751 flag to auto-quit port */
		loaded,			/* 1752 Is this port loaded? */
		other[3];		/* 1753 */

	long	LastEvent,		/* 1756 keep track of the last event */
		LastCut,		/* 1760 time in these 4 catagories */
		LastHold,		/* 1764 */
		LastOnLine;		/* 1768 */

	long	TempSysMaint;
	short	ConfIdle;

	UBYTE	expansion2[16];		/* 1772 */

	ULONG	DOIO_MASK;		 /* 1794 */
	struct	IOStdReq 	*DoReq;	 /* 1798 */
	struct	MsgPort		*DoPort; /* 1802 */

	long	uz[10];			/* 1806 for C programmer use.  You can */
					/* (typecast) them as you need to */

	struct	Task		*ThisTask;
	struct	MainPort	*cnp;

	struct	Window		*Window, *CWindow, *SWindow, *TWindow,
				*TTWindow, *WBWindow;
	struct	Screen		*Screen;
	struct	RastPort	*wrp, *trp;
	struct	TextFont	*BlueFont,
				*StandardFont;
	struct	IOStdReq 	*UTReq, *ServReq, *PrintReq, *ConReq;
	struct	IOExtSer	*SerRdReq, *SerWrReq, *SerIOReq;
	struct	MsgPort		*SerRdPort,*SerWrPort,*SerIOPort, *ConPort,
				*UTPort,   *ServPort, *PrintPort, *cpreplyp;

	struct	FileInfoBlock	*fib;	/* for reading directories */
	struct	InfoData	*IData;	/* for getting free disk space, etc */
	struct	ItemType3       *Item_obs;
	struct	MessageType3    *Message;
	struct	BaseUser	*buser;
	struct	SelectType      *Select;
	struct	BaseUser	*brec;
	struct	SelectType	*Upload;
	struct	ListItem        *fpt;
	struct	ItemHeader	**ih, *ihead_obs;
	struct	ItemType3	**ip;
	char	**EdBuffer;		/* pointers to editor lines.*/
	char	*EdBase;		/* pointer to editor memory */
	UBYTE	*CR;			/* Is there a Carriage Return? ea line*/
	char	**mm;			/* pointers to bbsmenu lines */
	char	**bm;			/* pointers to bbstext lines */
	short	*menustart;
	short	*menulen;

	long	plsize, nplanes;	/* SkyStuff Stuff */
	short	skyxpen, skyypen;
	PLANEPTR planePtr;
	struct	TmpRas		tmpRas;
	struct	BitMap  	SkyBrush;

	long	FullMaint;

	long	numWE;			/* window-edit, current number */
	long	WWidth;		/* 80 */
	long	WLength;	/* 24 */
	long	WLeft;		/* def   0 */
	long	WRight;		/* def 639 */
	long	WTop;		/* def  11 */
	long	WBottom;	/* def YMAX */

	short	xc,			/* x coord, cursor pos */
		yc,			/* y coord */
		sxc,			/* storage for ANSI [s, [t */
		syc,
		YMAX,			/* max y value */
		CurrentMouseX,
		CurrentMouseY,
		countWE;		/* window-edit, # characters */

	short	IsHalfScreen;
	short	TermLength;

	short	SWidth;
	short	SHeight;
	short	DWidth;
	UBYTE	io_bad;
	UBYTE	io_status;		/* debugging Supra hangs */

	char	*by0,			/* scan and header variables */
		*by1,			/* for MCI configurability */
		*to0,
		*to1,
		*date0,
		*organ,

		plus[2],
		star[2],
		fdate[8],
		fsize[8],
		ftime[12],
		*title0,
		*olm_default,
		*vis_subj,
		*vis_to,
		*user_default,
		*subj_default;

	short	Depth;
	short	DHeight;

	short	TextSet;		/* 0==use system-wide text/menu */
	short	npitems;		/* number of items parsed */
	char	pitem[8][61];		/* last input parsed list */

	long	txline;			/* number of lines in ed buffer */
	char	GBuffer[16][81];	/* General purpose 2-D text buffer */
	UBYTE	GCR[16];

	ULONG	WaitFlags,
		SERVICE_MASK,
		RAWKEY_MASK;

	short	ACC[80],		/* accounting table currently in use */
		nowCharge;		/* schedule now in memory            */

	long	CTC[80];		/* users account, 62 items, Cost of This Call */

	short	sorder[240];		/* max subboards per subdir is 240 */

	char	*LastError;		/* for file transfer WHY command */

	char	RexxObject  [100],
		KBuff       [256],	/* keyboard buffer! */
		ABuffer     [256],	/* workspace */
		InBuffer    [256],	/* after a line input */
		FileNote    [364],
		Doing	     [48],
		SPrompt     [128],	/* subboard prompt */
		LPrompt     [128],	/* G/P/N list prompt */
		diskloc_obs  [80],
		ldiskloc     [80],	/* diskloc for P/G/N */
		myloc        [80],
		LastSubject_obs  [84],	/* for Read-Mail */
		filename    [116],
		ListArea     [40],
		rsearch      [64],
		myfile       [54],
		dirname      [32],
		CStack       [80],	/* Command stacking */
		ChatMsg      [84],	/* current chat message */
		sbuff        [48],
		outbuffer    [84],
		ansiout      [164],
		ConnectCode  [84],
		ChatBuff     [84],
		APrint       [36],	/* for PrintAccess */
		PName     [3][32],
		home         [36],
		qwk_to       [28],
		qwk_subject  [28],
		BackBuff [10][80],	/* last 10 commands executed */
		Pathx     [8][64];	/* g/p/n subdir history */

	long	bbnum;			/* pointer into D-Q */

	long	warn1;

	long	TotalCTC;		/* TOTAL Cost of this Call (100ths)*/

	short	bz,		/* # of subs on current List */
		dz,		/* how deep in the tree are we? */
		br,		/* item last used */
		rn_NOT,		/* how many items, current list */

		nl,		/* number on g/p/n list */
		allin_NOT,	/* Are all items in memory? */
		VTimeOnLine,	/* may reset at midnight */
		PTimeOnLine,	/* in-current-pfile time counter */

		mx,		/* highest item NOT killed */
		listn,		/* current list subboard pointer */
		parent0,
		gpos,		/* for global commands, last command # */

		nsubs,		/* tracks # subs accessed in a Global cmd */
		LinkPort;

	long	TermLinkBaud;

	long	nm_NOT,		/* were once SHORTS...moved here to*/
		rs,		/* avoid conflicts with 32767 max  */
		cpos,
		mstart,
		mend,
		new_i,		/* responses/item or per subboard  */
		new_n,
		new_f,
		new_p,
		new_ri,
		t_new_i,	/* total new items */
		t_new_n,	/* total new messages to you */
		t_new_f,	/* total new files */
		t_new_p,	/* total new posts */
		t_new_ri;	/* total new response items */

	USHORT	AParams,
		AParam[17];

	UBYTE	qwk;
	UBYTE	ylharc;
	UBYTE	yaddcr;
	UBYTE	ylocal;

	short	zmatches;
	UBYTE	zheader;
	UBYTE	readnew;

	long	nsets,
		rset,
		rcount,
		rtotal,
		set[10][2];

	UBYTE	runseen,
		resetIP,		/* flag, must re-order item pointers */
		atbase,
		TermLink,

		rtome,
		rfree,
		rfavorite,
		runval,

		rprivate,
		rreverse,
		rbrandnew,
		rnresponse,

		rpass,
		rsince,
		rnew,
		rglobal,

		rdeftype,
		rchron,
		rbyme,
		runtil,

		rjustposts,
		rpreview,
		rbanner,
		rjustfiles;

	struct	IsDate	rdate,
			udate;

	short	pos,
		num,

		rdelta,		/* for/back n items */

		NoSelected,	/* number of items in Select List */
		NoUploaded,
		ModelType;		/* Controls .M in editor.  */

	struct	RangeContext EdRange;

	long	tline, bit_mci;
	short	times, repmode, MaxLines, WScroll;

	long	warn2;

	char	keyin,
		edall_not,		/* allow enterline all chars */
		serbuffer[2];

	UBYTE	mode,
		ZAlternate,
		CheckAllEvents,		/* force check of TimeTilEvent's */
		TrapDoor,

		AtPrompt,
		InShell,
		donormal,
		dtr,		/* modem device OPENED and ready */

		sread,		/* read request pending ... */
		modem,		/* modem exists ... allows BBSPORT0 */
		mio,		/* modem IO allowed */
		isud,		/* when in base, is UD, not BASE */

		MaxColors,		/* # of colors on screen */
		FlagCH,
		FlagED,
		Gflag,

		ConfMode,		/* Are we in conference? */
		column,
		KB1,			/* DeQueue pointers */
		KB2,

		KB3,			/* what inter-user chat sees */
		CursorStatus,		/* is cursor on? */
		AbortFlags,
		HourNow,

		LastHour,
		Indent,
		InANSI,
		GETarg,

		CMDarg,
		LocalMode,		/* BOOLS .. In Local Mode ? */
		noflowcontrol,
		AtWaitScreen,		/* Are we at IDLE? */

		InMCI,
		DoQuick,		/* ! at last input ? */
		AutoLogon,
		TermOn,			/* in terminal? */

		ANSIon,			/* can we type ansi? (ESC) */
		PrintFlags,		/* space or ^C pressed */
		CountUp,
		InputFlags,

		Relogon,
		Logoff,
		NewScanBROWSE,		/* BROWSE at logon scan for new? */
		AutoScroll,

		WWrap,
		DoubleBuffer,
		MoreRow,
		MoreAtRow,

		WScreen,
		InTransfer,
		NoMCI,
		FreezeTime,

		kolorific,
		mciflash,		/* actually, bold */
		mciscore,
		mcireverse,

		intense,		/* color greater than 7? */
		mciprint,
		mcispeed,
		mcicolor,		/* the requested MCI color 0-7 */

		pencolor,		/* actual screen pencolor */
		mcinoabort,
		backcolor,		/* background color */
		mciback,

		MCIenv,			/* current MCI environ flags */
		MCIdefenv,
		MRinuse,
		OffLine,		/* skip answering the phone?? */

		cminute, 
		chour,
		FNewSave,		/* flag: update FNewDate at logoff */
		intab;			/* for LoadLine() */

	char	mcitest,		/* MCI stuff */
		EditorOn,		/* is editor on? */
		var5,
		ScreenWidth;

	long	warn3;

	long	swrite, LogoffFlag;	/* file transfer stuff */
	long	ResumeSize;
	short	ResumeNumber, StartUpTime;

	short	protonum;
	char	protobyte;
	char	edbuff;			/*  1 saved new text
					    0 saved empty buffer
					   -1 aborted
					*/

/* If you use QuoteType=2, you MUST set the following three variables.
   Note that QuoteFrom and QuoteDate are POINTERS ... do NOT copy into
   them!
*/
	char	*QuoteFrom;		/* Who are we quoting? */
	struct	IsDate *QuoteDate;	/* When was that message written? */
	long	QuoteEnd;		/* LAST BYTE position in file */
					/* Quoter will stop automatically
					   if ^Z occurs first */
	UBYTE	qwk_anonymous;
	UBYTE	qwk_private;
	short	xfercount;

	long	XferTime;
	long	LastCPS, cpsavecount;
	long	cpsavesum, LastXTime, StartByte, LastByte;

	short	ARunCount;		/* depth of AREXX pfiles */
	short	CRunCount;		/* depth of C pfiles */
	char	CSpawn[84];

	long	TrapBaud;
	long	TrapLock;

	UBYTE	QuoteType;		/* 0==Mail, 1==Message base
					   2==User defined.  See QuoteFrom,
					      QuoteDate, etc., above */
	char	QuotePath[103];		/* MESSAGE QUOTING in editor. */
					/* SETTING this ENABLES quoting.
					   Remember to QuotePath[0] = 0 after
					   you use it. */
	long	QuoteSeek;		/* What's the starting byte pos? */

	short	imode;		/* editor.  Insert mode?  If yes, which line*/
	short	iline;

	long	abort;

	short	UrgentMail1;
	char	ExtTransfer;
	char	ExtProtocol;

	long	ExtMinFree;

	long	ALogon;
	long	BLogon;
	long	ATransfer;
	long	BDownload;
	long	BUpload;
	long	APfiles;
	long	BCharge;
	long	hiCharge;
	long	loCharge;

	long	rtoid;			/* to 'handle' searches */

	long	TimeTilOnLine,
		TimeTilCutoff,		/* Log user off first event */
		TimeTilEvent,		/* Immediate (regular) event */
		TimeTilHold;		/* hold until off-line event */

	long	SERIAL_MASK,
		UTILITY_MASK;

	char	*xpr_filename;


	UBYTE	xpr_obsolete[124];


	struct	MsgPort *rexxPort;
	long	rexxPortBit;
	char	*extension;
	struct	rexxCommandList *globalrcl;

	UBYTE	callback,		/* enable AUTOCALLBACK @ LOGON */
		avalid,			/* which avalid set to use */
		joinlink,		/* 0==off, 1==accept, 2==accept ONLY */
		FreezeIdle;		/* special use -- stop idle timer */

	long	rbyid;			/* by 'handle' searches */

	long	SAMNow[20];		/* users 'Last' SAM column */

	struct	CMessage	cpmess;
	struct	SerPort		sp;	/* BBSPORT data. see above struct */
	struct	BaseUser	xbrec;
	struct	ItemType3	Item0;
	struct	ItemHeader	ihead0;
	struct	HeaderType	Header0, LastHeader;
	struct	MailHeader	Mail0;
	struct	SelectType	Select0;
	struct	MessageType3	Message0;
	struct	Subdirectory	sub;

	struct	NewSubboardType *sb;

	char	rpattok[202];		/* tokenized search pattern */

	UBYTE	Subboard0_NOT [276];

	short	WBorTop;
	short	nupload;		/* # of upload allocated    */
	short	nips;			/* # of ip,ih now allocated */

	short	Monitored;		/* 128 + port number if monitored. */
	short	ModelEnable;		/* Controls .M in editor.  */

	BPTR	MCIfile;
	long	mcilabel;
	long	nlabels;
	long	mlabel[20];
	long	mfseek[20];
	long	MCInreg[10];
	char	MCIcreg[ 5][100];

	BPTR	sf;			/* for _short scan */
	long	sfin;

	long	VoteItem;		/* which vote? -1==New */
	char	*VotePath;		/* for Vote in Any Area! */

	UBYTE	q_kolorific,		/* MCI Q2/Q3 storage */
		q_mciflash,
		q_mciscore,
		q_mcireverse,

		q_intense,
		q_mciprint,
		q_mcispeed,
		q_mcicolor,

		q_mciback,
		q_mcinoabort,
		q_MCIenv;

	UBYTE	SelectInMemory;		/* is _select in memory or on disk? */

	struct	Event	*FirstEvent,
			*FirstCutEvent,
			*FirstHoldEvent,
			*FirstOnLineEvent;

	struct	TermLink tl;

	char	JoinLinkOut[76];

	struct	VDEentry *vv;		/* the currently loaded VDE file */
	short	vns;			/* used with VDEapply featuers!  */
	UBYTE	vmulti;			/* editing more than one?        */
	UBYTE	iselect;		/* is the input SELECTED?        */

	char	gmFile[40];		/* global Read-Mail variables... */
	long	*gmPos;			/* needed for Yank command ....  */
	UBYTE	*gmFlags;

	UBYTE	*amem;			/* DrawEd variables */
	UBYTE	*cmem;
	long	maxdraw;

	UBYTE	predraw;
	UBYTE	GiveCPR;		/* CPR request received in term */
	UBYTE	hmmmm[2];

	UBYTE	*smem;

	long	inlen, edlength;
};

#define HIDDEN_FLAG	0x01
#define MUFFLED_FLAG	0x02
#define MONITOR_FLAG	0x04
#define CHATREQ_FLAG	0x08

struct ULock {				/* account locking node!!! */
	short	Account;
	short	Wait;

	struct	SignalSemaphore Sem;

	struct	ULock *Next;
};

struct FidoNet {
	char from[36];
	char to  [36];
	char subject [72];
	char dateline[20];
	USHORT time;
	USHORT destnode;
	USHORT orignode;
	USHORT cost;
	USHORT orignet;
	USHORT destnet;
	USHORT destzone;
	USHORT orignzone;
	USHORT destpoint;
	USHORT orignpoint;
	USHORT replyto;
	USHORT attribute;
	USHORT nextrply;
};

struct PacketHeader {
	USHORT	orignode;
	USHORT	destnode;
	USHORT	year;
	USHORT	month;
	USHORT	day;
	USHORT	hour;
	USHORT	minute;
	USHORT	second;
	USHORT	baud;
	USHORT	two_oh;
	USHORT	orignet;
	USHORT	destnet;
	char	fill[34];
};

struct PackedMessage {
	USHORT	two_oh;
	USHORT	orignode;
	USHORT	destnode;
	USHORT	orignet;
	USHORT	destnet;
	USHORT	attribute;
	USHORT	cost;
	char	date[20];
};

struct NodeType {		/* a node that we export-to */
	char	*buffer;
	ULONG	buffpos;
	ULONG	buffsize;

	USHORT	zone;
	USHORT	net;
	USHORT	node;
	USHORT	point;

	USHORT	packer;
	USHORT	format;		/* 4d or 2d */
	USHORT	type;		/* normal, crash, hold */

	USHORT	flag;		/* 0-31 ... which subboards? */
	USHORT	domain;		/* which fido-net domain? */

	UBYTE	Access;
	UBYTE	Flags;

	USHORT	more[4];

	ULONG	incount;
	ULONG	outcount;

	char	Password[12];
};

struct RouteType {		/* a netmail routing pattern */
	USHORT	zone;		/* from ... -1's mean wildcard ... */
	USHORT	net;
	USHORT	node;
	USHORT	point;

	USHORT	zone2;		/* to */
	USHORT	net2;
	USHORT	node2;
	USHORT	point2;

	USHORT	domain;		/* which fido-net domain? */
	USHORT	type;		/* normal, crash, hold */

	USHORT	more[4];

	ULONG	outcount;
};

struct PacketType {
	struct	PacketType *next;

	char	filename[32];
};

struct ImportType {
	struct	ImportType *next;

	USHORT	zone0, net0, node0, point0;
	ULONG	data;

	long	seek;
	long	end;
	long	seen;		/* index */
	long	xoff;		/* index */

	struct	IsDate date;

	UBYTE	isdupe, isfull;
};

struct	DupeType {
	USHORT	zone;
	USHORT	net;
	USHORT	node;
	USHORT	point;
	ULONG	data;
};

struct	EchoType {
	char	Title[24];
	ULONG	ExportTo[4];		/* export to which node types */

	struct	ImportType *first;	/* during toss, list to import */
	struct	ImportType *last;

	USHORT	listn;			/* during toss, which local sub? */
	USHORT	domain;

	struct	DupeType *duptab;
	ULONG	incount;
	ULONG	outcount;

	USHORT	dupsize;

	UBYTE	dupdelta;
	UBYTE	Access;
	UBYTE	Flags;

	UBYTE	other[3];

	char	Info[48];
};

struct	BBSItem {
	long	ID;

	char	Phone[16];
	char	Title[26];
	char	Location[21];
	char	Baud[6];
	char	Comments[62];
	char	Country[4];
	char	Flags[10];

	UBYTE	Immortal;

	struct	IsDate Date;
};
