#ifndef NOVIA_CONVERT_CNET_UDATA_H
#define NOVIA_CONVERT_CNET_UDATA_H

struct IsDate {		/* must LEAVE as BYTE */
	BYTE Year;
	BYTE Month;
	BYTE Date;
	BYTE Hour;
	BYTE Minute;
	BYTE Second;
};

struct cnetuser {			/* each user account */
	long	IDNumber;		/* 0 unique (serial) ID # */
	char	Handle[21];		/* 4 */
	char	RealName[26];	/* 25 */
	char	Address[31];	/* 51 */
	char	CityState[31];	/* 82 */
	char	ZipCode[11];	/* 113 */
	char	Country[8];		/* 124 */
	char	PhoneNo[17];	/* 132 Euro 8-7 format POSSIBLE!! */
	char	PassWord[15];	/* 149 */
	char	Comments[34];	/* 164 */
	char	Macro[3][36];	/* 198 user's defined macro keys */
	char	Organ[31];		/* 306 */
	UBYTE	PhoneVerified;	/* 337 */

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

#endif