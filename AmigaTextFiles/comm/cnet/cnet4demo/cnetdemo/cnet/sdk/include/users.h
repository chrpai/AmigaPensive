
#ifndef CNET_USERS
#define CNET_USERS

#include "dates.h"


struct Privs {				/* each user&group has one */
	long	MBaseFlags;		/* 580 */
	long	FBaseFlags;		/* 584 Can user reach which subs ? */
	long	LBaseFlags;		/* 588 Gfiles/Pfiles/Telnet/Other flags */

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
	short	MaxMailKBytes;		/* 630 */
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

	LONG	not_usedeither;

	char	exp[ 2 ];		/* 666 */

	short	Alias;			/* 668 */
	short	Dictionary;		/* 670 */
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

	struct	PortData *z;		/* for lock account */

	struct	IsDate Birthdate;	/* 342 */
	struct	IsDate FirstCall;	/* 348 */
	struct	IsDate LastCall;	/* 354 */

	ULONG	EMailFlags;		/* 360 */
	
	USHORT	InterfaceType;		// 364 - 0: CNet/4 Type -  number entered = read message x
                              //       1: CNet/3 Type - number entered = move to subboard x

	struct	IsDate ConnectDate;	/* 366 - date of connection */
	struct	IsDate YankDate;	/* 372 - date of last yank */

	BYTE	Access;			/* 378 - current access level */
	BYTE	ExpireAccess;		/* 379 - access level to expire to when "ExpireDate" is reached */

	short	PfileTimeToday;

	short	not_used2;		// expansion

	BYTE	TextSet;			// 384 - which BBSTEXT/MENU translation
	BYTE	TimeForm;		// which time form to use
	BYTE	MoreMode;		// more? mode
	BYTE	LineFeeds;		// user requires line feeds
	BYTE	FracBal;			// 1/100ths of a cent
	BYTE	TimeZone;		// time zone (relative to local system time)
	BYTE	Colors;			// can user display ANSI colors?
	BYTE	Tabs;				// can user do ANSI tabs?
	BYTE	ANSI;				// 392 - user's ANSI type
	BYTE	Sex;				// user's gender
	BYTE	AutoMore;		// More? and CLS between messages?
	BYTE	TermType;		// Terminal emulation
	BYTE	HelpLevel;		// 396 - novice, intermediate etc..
	BYTE	CompType;		// computer type - relative to top of section 30; in BBSMENU
	BYTE	DefProtocol;	// 398 - user's default txfer protocol
	BYTE	TermWidth;		// number of characters per line
	BYTE	TermLength;		// 400 - number of lines for full screen
	BYTE	MailBoxOpen;	// 401 - mailbox open or closed
	short	MailForward;	// 402 - mail forward to account stored here - if 0, no forward

	long 	TotalCalls;			/* 404 */
	long	PubMessages;		/* 408 */
	long	PriMessages;		/* 412 */
	long	UpBytes;				/* 416 KILO bytes */
	long	UpFiles;				/* 420 */
	long	DownBytes;			/* 424 KILO bytes */
	long	DownFiles;			/* 428 */

	long	FileCredits;		/* 432 CREDITS used now! */
	long	ByteCredits;		/* 436 */
	long	TimeCredits;		/* 440 */

	long	Balance;				/* 444 */
	long	NetCredits;			/* 448 */
	long	DoorPoints;			/* 452 */

	long	DayUpBytes;			/* 456 */
	long	DayDownBytes;		/* 460 */
	short	DayUpFiles;			/* 464 */
	short	DayDownFiles;		/* 466 */

	short	CallsToday;			/* 468 */
	short 	TimeToday;		/* 470 */
	short	LogonAttempts;		/* 472 */

	USHORT	BaudRate;		/* 474 */
	USHORT	HighBaud;		/* 476 */

	char	UUCP[11];			/* 478 - valid length of UUCP name is still 9 characters with one termination \0 to make 10 total */

	UBYTE	PName;				/* 489 flags, are these data     */
	UBYTE	PAge;					/* items to be kept private? */
	UBYTE	PAddress;			/* street address only */
	UBYTE	PVoice;				/* phone numbers */
	UBYTE	PData;

	char	Banner[43];			/* 494 */

	char	VoiceNo[17];		/* 537 Euro 8-7 format POSSIBLE!! */

	short	YankCount;			/* 554 */

	struct	IsDate ExpireDate;	/* 556 */
	struct	IsDate FNewDate;		/* 562 */

	char	more[7];				/* 568 reserved */

	UBYTE	YankCR;				/* 575 add CR's to yank text? */
	UBYTE	YankArc;				/* which archiver to use? */
	BYTE	DefEditor;			/* 577 */
	BYTE	AutoHide;
	BYTE	MailSort;			/* 579 */
	struct	Privs MyPrivs;		/* 580 THESE ACTUALLY USED, not AGC */
};


struct UserData426a {			/* each user account */
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

	struct	PortData *z;		/* for lock account */

	struct	IsDate Birthdate;	/* 342 */
	struct	IsDate FirstCall;	/* 348 */
	struct	IsDate LastCall;	/* 354 */

	ULONG EMailFlags;					// set before entering the editor to write mail.
	
	USHORT	InterfaceType;			// 364 - 0: CNet/4 Type -  number entered = read message x
                              	//       1: CNet/3 Type - number entered = move to subboard x

	struct	IsDate ConnectDate;	/* 366 - date of connection */
	struct	IsDate YankDate;	/* 372 - date of last yank */

	BYTE	Access;			/* 378 - current access level */
	BYTE	ExpireAccess;		/* 379 - access level to expire to when "ExpireDate" is reached */

	short	PfileTimeToday;

	short	not_used2;		// expansion

	BYTE	TextSet;			// 384 - which BBSTEXT/MENU translation
	BYTE	TimeForm;		// which time form to use
	BYTE	MoreMode;		// more? mode
	BYTE	LineFeeds;		// user requires line feeds
	BYTE	FracBal;			// 1/100ths of a cent
	BYTE	TimeZone;		// time zone (relative to local system time)
	BYTE	Colors;			// can user display ANSI colors?
	BYTE	Tabs;				// can user do ANSI tabs?
	BYTE	ANSI;				// 392 - user's ANSI type
	BYTE	Sex;				// user's gender
	BYTE	AutoMore;		// More? and CLS between messages?
	BYTE	TermType;		// Terminal emulation
	BYTE	HelpLevel;		// 396 - novice, intermediate etc..
	BYTE	CompType;		// computer type - relative to top of section 30; in BBSMENU
	BYTE	DefProtocol;	// 398 - user's default txfer protocol
	BYTE	TermWidth;		// number of characters per line
	BYTE	TermLength;		// 400 - number of lines for full screen
	BYTE	MailBoxOpen;	// 401 - mailbox open or closed
	short	MailForward;	// 402 - mail forward to account stored here - if 0, no forward

	long 	TotalCalls;			/* 404 */
	long	PubMessages;		/* 408 */
	long	PriMessages;		/* 412 */
	long	UpBytes;				/* 416 KILO bytes */
	long	UpFiles;				/* 420 */
	long	DownBytes;			/* 424 KILO bytes */
	long	DownFiles;			/* 428 */

	long	FileCredits;		/* 432 CREDITS used now! */
	long	ByteCredits;		/* 436 */
	long	TimeCredits;		/* 440 */

	long	Balance;				/* 444 */
	long	NetCredits;			/* 448 */
	long	DoorPoints;			/* 452 */

	long	DayUpBytes;			/* 456 */
	long	DayDownBytes;		/* 460 */
	short	DayUpFiles;			/* 464 */
	short	DayDownFiles;		/* 466 */

	short	CallsToday;			/* 468 */
	short 	TimeToday;		/* 470 */
	short	LogonAttempts;		/* 472 */

	USHORT	BaudRate;		/* 474 */
	USHORT	HighBaud;		/* 476 */

	char	UUCP[11];			/* 478 - valid length of UUCP name is still 9 characters with one termination \0 to make 10 total */

	UBYTE	PName;				/* 489 flags, are these data     */
	UBYTE	PAge;					/* items to be kept private? */
	UBYTE	PAddress;			/* street address only */
	UBYTE	PVoice;				/* phone numbers */
	UBYTE	PData;

	char	Banner[43];			/* 494 */

	char	VoiceNo[17];		/* 537 Euro 8-7 format POSSIBLE!! */

	short	YankCount;			/* 554 */

	struct	IsDate ExpireDate;	/* 556 */
	struct	IsDate FNewDate;		/* 562 */

	char	more[7];				/* 568 reserved */

	UBYTE	YankCR;				/* 575 add CR's to yank text? */
	UBYTE	YankArc;				/* which archiver to use? */
	BYTE	DefEditor;			/* 577 */
	BYTE	AutoHide;
	BYTE	MailSort;			/* 579 */
	struct	Privs MyPrivs;		/* 580 THESE ACTUALLY USED, not AGC */

	// 08-Mar-97
	char ReplyTo[128];		// user's return email address used in "Reply-To" fields

	BYTE expansion[128];		// expansion.
};


#define EDHANDLE_FLAG			0x01	/* bit defs for ABits2 of struct Privs */
#define EDREALNAME_FLAG			0x02
#define EDADDRESS_FLAG			0x04
#define EDVOICE_FLAG	     	 	0x08
#define EDDATA_FLAG				0x10
#define BANNER_FLAG				0x20
#define NOT_USED_FLAG			0x40
#define PORTMONITOR_FLAG		0x80
#define VOTEMAINT_FLAG			0x0100
#define ALARMSYSOP_FLAG			0x0200
#define OPENSCREEN_FLAG			0x0400
#define OPENCAPTURE_FLAG		0x0800
#define NETMAIL_FLAG				0x1000
#define FREQSEND_FLAG			0x2000
#define NETCOSTEXEMPT_FLAG		0x4000
#define EXPANSION1_FLAG			0x8000
#define EXPANSION2_FLAG			0x010000
#define EXPANSION3_FLAG			0x020000
#define EXPANSION4_FLAG			0x040000
#define UUCPMAIL_FLAG			0x080000
#define NETCOSTCREDITS_FLAG	0x100000
#define HOLDANDCRASH_FLAG		0x200000
#define EXPANSION8_FLAG			0x400000
#define BROADCAST_FLAG			0x800000
#define SUPERUSER_FLAG			0x01000000

#define EMAIL_FLAG			0x01	/* bit defs for ABits of struct Privs */
#define	PFILE_FLAG			0x02
#define GFILE_FLAG			0x04
#define	ULIST_FLAG			0x08
#define	SYSOP_FLAG			0x10
#define	REWARDS_FLAG		0x20	/* may receive download rewards? */
#define	FCREDEX_FLAG		0x40
#define BCREDEX_FLAG			0x80
#define	EXPIREMAIL_FLAG	0x0100
#define	BULKMAIL_FLAG		0x0200
#define XXXXXXXXX6_FLAG		0x0400
#define	URGENTMAIL_FLAG	0x0800
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

// obsolete

struct ULock {				/* account locking node!!! */
	short	Account;
	short	Wait;

	struct	SignalSemaphore Sem;

	struct	UserData user1;
	struct	UserData *u;

	struct	ULock *Next;
};

struct AccessGroup {
	char	Name[32];		/*   0 */
	struct	Privs DefPrivs;		/*  32 */

	short	ExpireDays;		/*  32+92 */
	UBYTE	ExpireAccess;		/*  32+94 */

	char	exp[29];
};

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

struct KeyElement4 {
	char	UUCP[11];			// 10 characters allowed in UUCP name - plus terminating 0 byte
	long	IDNumber;			/* high order byte ALWAYS 0 */
	long	phone1, phone2;	/* data phone's area code & local # */
	char	Handle   [21];
	char	RealName [26];
	UBYTE	PName;

	BYTE Access;	// user's access group!
};

#endif
