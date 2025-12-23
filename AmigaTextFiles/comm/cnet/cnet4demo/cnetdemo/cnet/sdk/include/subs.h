
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

	ULONG	Number;			/* item number */
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
	ULONG	Number;			/* unique message number */

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
	ULONG	ItemNumber;		/* item serial ID# */
	long	Seek;			/* position in text */
	long	ByID;
	long	ToID;
	ULONG	Number;			/* serial ID# */

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
	ULONG	Number;
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

	UBYTE	PleaseKill_obs;		/* 72 */
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

	short	order;			/*  92 on disk ...*/

	UBYTE	nada[5];		/*  94 reserved */
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

	UBYTE	Subdirectory;		// 1==dir, 2==text/door
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
	UBYTE	Marker;		/* 0 = subdir, 1 = Messages, 2 = Files,
                        4 = text/door area, 5=Text file, etc,
                        ^ same as "object class" during "AL"
                        &128 = Killed */

	long	Transforms;	/* 270 which BBSARCS to transform */

	long	ExportTo;	/* 274 */

	char	NothingNow[8];	/* 278 */

	ULONG	count;		/* 286 replaces the _COUNT file */
	ULONG	hiwater;	/* 290 */

	UBYTE	NetNumber;				// 294	->	0 = none, 1=Usenet/NNTP, 2=mailing list 
										//			->	type 2 (mailing list) added 25-Jun-97 v4.27c

	UBYTE	Locked;					// 295 Does AMAINT have a lock here?
	UBYTE	DLnotifyULer;			// 296

	char	TransformTo[5];		// 297

	UBYTE	QWKReplies;				// 302
	BYTE	SBaseUse;				// 303  which BBSCHARGE rate to use?
	UBYTE	CarbonCopy;				// 304
	UBYTE	CDROM;					// 305 copy to hard drive before download?

	struct	IsDate LastUpload;
	struct	IsDate LastMessage;

	short	Users;					//	how many people are in this sub
				   					//	REAL TIME (not # of members)

	long	Created;					// for membership determination

	short	Parent;					// pointers to other subboard #s
	short	Child;
	short	Next;

	short	TimeLock;				// 330 minutes online before available

	char	ZeroPath[48];			// 332
	char	Origin  [60];			// 380

	// if more room is needed, I could put all of this into a pointer to a 'sub-info' structure

	short	MaxItems;				// 440
	UBYTE	Persist;					// 442
	UBYTE	Delay;					// 443

	struct	ItemType3 *Item;
	struct	ItemHeader *ihead;
	struct	MessageType3 *NewMess;
	struct	SignalSemaphore *sem;

	long	nm;						// 460 # of _Messages
	short	rn;						// 464 # of _Items
	short	AllocItems;
	short	nNewMess;

	UBYTE	resetIP;					// 470 - set to TRUE to have CNet reset item pointers

	char	more_stuff[17];		// 471
};


struct SubboardType4 {
	char	Title[60];						// 0   - what user sees
	char	SubDirName[75];				// 60  - also functions as GO argument
	char	DataPath  [95];				// 135 - path to data files - may be different than SubDirName!
	long	Parts;							// 230 -  partitions 0-31 allowed

	long	SubOpIDs [6];					// 234 - unique user IDs of subbies

	short	MRewardBytes;					// 258 % of byte  size to award
	short	NRewardFiles;					// 260 # of file creds to award
					                     // uploader each time his file is
					                     // downloaded */

	struct	FreeType *free0;			// 262 pointer to free text in _text

	// added for v4.22 (beta)
	long	NNTPMaxDupes;					// 266 - maximum number of dupes to track for NNTP articles
	long	Nada2;							// 270 - expansion

	short	SubOpAccs[6];					/* 274 - subop ACCOUNT #s */
	char	Filler[6];						/* 286 shown during SCAN */
	UBYTE	fdelta;							/* 292 */
	UBYTE	floaded;							/* 293 */
	UBYTE	Direct;							/* 294 Disk-exchange type */
	UBYTE	Vote;								/* 295 May non-sysops add topics? */
	short	FreeDays;						/* 296 when files are auto-free */
	UBYTE	Subdirectory;  				/* 298 */
	UBYTE	Closed;							/* 299 */
	long	UnionFlags;						/* 300 Must have all of these flags */
	long	Access;							/* 304 Groups which may ENTER */
	long	PostAccess;						/* 308 Groups which may Post  */
	long	RespondAccess;					/* 312 ... Respond */
	long	UploadAccess;					/* 316 ... Upload */
	long	DownloadAccess;				/* 320 ... Download */
	long	ComputerTypes;					/* 324 */
	long	Hours;							/* 328 restricted entry hours */
	long	HourUnionFlags;				/* 332 */
	long	HourAccess;						/* 336 groups which may Enter during Hours*/
	long	Baud;								/* 340 minimum Baud rate to enter */
	long	BaudHours;						/* 344 hours to enforce min baud rate */
	UBYTE	Gender;							/* 348   0 (for unisex) or 'M', 'F' */
	UBYTE	AllowAliases;  				/* 349 */
	UBYTE	PurgeStatus;					/* 350 */
	UBYTE	Youngest;						/* 351 */
	UBYTE	EXTRA;							/* 352 */
	UBYTE	Oldest;							/* 353 */

	struct	IsDate	Sent;				/* 354 FIDO to track new messages */

	short	MDownBytes;						/* 360 % of download bytes to subtract */
	short	NDownFiles;						/* 362 # of files to subtract per download */
	short	MTimeCredit;					/* 364 % of time to give back for uploads */
	long	MinFreeBytes;					/* 366 to upload in this subboard */
	UBYTE	SubSort;							/* 370 */
	BYTE nothing_now;						/* 371 */
	short	InactiveDays;					/* 372 */

	UBYTE	RealNames;						/* 374 use real names instead of Handles? */
	UBYTE	Addressees; 					/* 375 */
	UBYTE	Anonymous;  					/* 376 */
	UBYTE	override;						/* 377 how to set the use defaults flags in
					                            newly uploaded items */

	UBYTE	Verification;					/* 378 */
	UBYTE	DupCheck;						/* 379 */
	UBYTE	ShowUnvalidated;				/* 380 */
	UBYTE	NoMCI;							/* 381 */

	UBYTE	PrivateArea;					/* 382 0/1 No/Yes ... 2==FORCE */
	BYTE	SDownFiles;						/* 383 Which download charge schedules? */
	BYTE	SDownBytes;						/* 384 */
	UBYTE	Test;								/* 385 test at upload time */

	UBYTE	Transform;						/* 386 run script at upload time */
	UBYTE	AutoAdopt;						/* 387 Adopt Orphans at a-maint */
	UBYTE	WeedResponses;					/* 388 Use inactive days against indiv responses */
	UBYTE	NoSignatures;					/* 389 */

	UBYTE	NoReadCharges;					/* 390 opt to short circuit accounting system */
	UBYTE	NoWriteCharges;				/* 391 for reading/writing messages */

	long	Arcs;								/* 392 which Arcs in BBSARC are allowed uploaded*/

	short	UpRatioBytes;					/* 396 which set of file/byte ratios to use? */
	short	UpChargeBytes;					/* 398 which set of charges to use?          */
	short	UpRatioFiles;					/* 400 */
	short	UpChargeFiles;					/* 402 */

	UBYTE	Invitation;						/* 404 */
	UBYTE	UserMustJoin; 					/* 405 */
	UBYTE	DeleteOwn;    					/* 406 */
	UBYTE	Marker;							/* 407     0 = Base, 1 = UDBASE, &128 = Killed */

	long	Transforms;						/* 408 which BBSARCS to transform */

	long	ExportTo;						/* 412 */

	UBYTE DIZAlphaNumOnly;				/* 416 - strip everything but alpha and numeric text */
	UBYTE UseDIZStripChars;				/* 417 - strip the characters specified in BBSTEXT line 2150 */
	UBYTE UseDIZStripText;				/* 418 - strip any text matching the contents of any line of sysdata:DIZStripText */
	UBYTE DIZStripCR;						/* 419 - strip carriage return/linefeed from DIZ */

	UBYTE	NNTPMarker;						/* 420 - 0 = no-import/disabled newsgroup
                                             1 = regular NNTPGet newsgroup - full import of header/body
                                             2 = mailing list pseudo-newsgroup
                                             3 = (not yet implemented) NNTPGet headers only - connect to remote host to retrieve body text
                                             4 = (not yet implemented) No NNTPGet - always connect to remote for article list and body text after scanning articles & presenting to user */

	UBYTE ReformatDIZ;					// 18-Feb-97 reformat DIZ - remove multiple spaces between words,
												//                sussessive newlines, tabs, etc,..

	UBYTE nada3[2];						// 421 nothing now

	ULONG	count;							/* 424 replaces the _COUNT file */
	ULONG	hiwater;							/* 428 */

	UBYTE	NetNumber;						/* 432 */ 
	UBYTE	Locked;							/* 433 Does AMAINT (or other processes) have a lock here? */
	UBYTE	DLnotifyULer;					/* 434 */

	char	TransformTo[5];				/* 435 */

	UBYTE	QWKReplies;						/* 440 */

	BYTE	SBaseUse;						/* 441  which BBSCHARGE rate to use? */
	UBYTE	CarbonCopy;						/* 442 */
	UBYTE	CDROM;							/* 443 copy to hard drive before download? */

	struct	IsDate LastUpload;		/* 444 */
	struct	IsDate LastMessage;		/* 450*/

	short	Users;							/* 456  how many people are in this sub
   				                             REAL TIME (not # of members) */

	long	Created;							/* 458  for membership determination */

	short									Parent,		/* 462 - PHYSICAL number of first subboard on the list to which this subboard belongs (all subboards in the ROOT directory should point to the very first physical subboard) */
											Child,		/* 464 - physnum of first subboard in the list to which this subdirectory houses (-1 if none) */
											Next;		   /* 466 - physnum of the next subboard on the current list (-1 if none) */

	short	TimeLock;		/* 468 */

	char	ZeroPath[90];	/* 470 */
	char	Origin  [60];  /* 560 */

	/* if more room is needed, I could put all of this into a pointer to a
	   'sub-info' structure */

	USHORT MaxItems;	/* 620 */
	UBYTE	Persist;		/* 622 */
	UBYTE	Delay;		/* 623 */

	struct	ItemType3 *Item;			/* 624 */
	struct	ItemHeader *ihead; 
	struct	MessageType3 *NewMess;
	struct	SignalSemaphore *sem;

	long	nm;
	USHORT	rn;
	USHORT	AllocItems;
	USHORT	nNewMess;

	UBYTE	resetIP;

	/* diz support! 11-Jun-96 */
	long	DizTypes;						/* 652 - which BBSARCs to Diz process (v4.20) */
	UBYTE DizSave;							/* 656 - 0=don't save back to archive, 1=save DIZ back to archive after edit/changed */
	UBYTE UseSentBy;						/* 657 - 0=don't tack on sentby's, 1=tack sentbys on uploads */
	UBYTE DizEdit;							/* 658 - allow diz'd discriptions to be altered/edited - maint users have override can be turned of in user prefs */

	LONG	NNTPLastImport;				/* 659 - last NNTP news import date/time */

	char	SubDoing[17];					/* 663 - String to be set for the Where/Action/DOING string in WHO display when use enters this subboard object */
	BYTE  more_stuff[15];
	};											/* total length = ??? */


//struct NewSubboardType4 {
//	char	Title[60];						/* 0   - what user sees */
//	char	SubDirName[75];				/* 60  - also functions as GO argument */
//	char	DataPath  [95];				/* 135 - path to data files - may be different than SubDirName! */
//	long	Parts;							/* 230 -  partitions 0-31 allowed */
//
//	long	SubOpIDs [6];					/* 234 - unique user IDs of subbies */
//
//	short	MRewardBytes;					/* 258 % of byte  size to award */
//	short	NRewardFiles;					/* 260 # of file creds to award
//					                      uploader each time his file is
//					                      downloaded */
//
//	struct	FreeType *free0;			/* 262 pointer to free text in _text */
//
//	// added for v4.22 (beta)
//	long	NNTPMaxDupes;					/* 266 - maximum number of dupes to track for NNTP articles */
//	long	Nada2;							/* 270 - expansion */
//
//	short	SubOpAccs[6];					/* 274 - subop ACCOUNT #s */
//	char	Filler[6];						/* 286 shown during SCAN */
//	UBYTE	fdelta;							/* 292 */
//	UBYTE	floaded;							/* 293 */
//	UBYTE	Direct;							/* 294 Disk-exchange type */
//	UBYTE	Vote;								/* 295 May non-sysops add topics? */
//	short	FreeDays;						/* 296 when files are auto-free */
//	UBYTE	Subdirectory;  				/* 298 */
//	UBYTE	Closed;							/* 299 */
//	long	UnionFlags;						/* 300 Must have all of these flags */
//	long	Access;							/* 304 Groups which may ENTER */
//	long	PostAccess;						/* 308 Groups which may Post  */
//	long	RespondAccess;					/* 312 ... Respond */
//	long	UploadAccess;					/* 316 ... Upload */
//	long	DownloadAccess;				/* 320 ... Download */
//	long	ComputerTypes;					/* 324 */
//	long	Hours;							/* 328 restricted entry hours */
//	long	HourUnionFlags;				/* 332 */
//	long	HourAccess;						/* 336 groups which may Enter during Hours*/
//	long	Baud;								/* 340 minimum Baud rate to enter */
//	long	BaudHours;						/* 344 hours to enforce min baud rate */
//	UBYTE	Gender;							/* 348   0 (for unisex) or 'M', 'F' */
//	UBYTE	AllowAliases;  				/* 349 */
//	UBYTE	PurgeStatus;					/* 350 */
//	UBYTE	Youngest;						/* 351 */
//	UBYTE	EXTRA;							/* 352 */
//	UBYTE	Oldest;							/* 353 */
//
//	struct	IsDate	Sent;				/* 354 FIDO to track new messages */
//
//	short	MDownBytes;						/* 360 % of download bytes to subtract */
//	short	NDownFiles;						/* 362 # of files to subtract per download */
//	short	MTimeCredit;					/* 364 % of time to give back for uploads */
//	long	MinFreeBytes;					/* 366 to upload in this subboard */
//	UBYTE	SubSort;							/* 370 */
//
//	BYTE	unused;
//
//	short	InactiveDays;					/* 372 */
//
//	UBYTE	RealNames;						/* 374 use real names instead of Handles? */
//	UBYTE	Addressees; 					/* 375 */
//	UBYTE	Anonymous;  					/* 376 */
//	UBYTE	override;						/* 377 how to set the use defaults flags in
//					                            newly uploaded items */
//
//	UBYTE	Verification;					/* 378 */
//	UBYTE	DupCheck;						/* 379 */
//	UBYTE	ShowUnvalidated;				/* 380 */
//	UBYTE	NoMCI;							/* 381 */
//
//	UBYTE	PrivateArea;					/* 382 0/1 No/Yes ... 2==FORCE */
//	BYTE	SDownFiles;						/* 383 Which download charge schedules? */
//	BYTE	SDownBytes;						/* 384 */
//	UBYTE	Test;								/* 385 test at upload time */
//
//	UBYTE	Transform;						/* 386 run script at upload time */
//	UBYTE	AutoAdopt;						/* 387 Adopt Orphans at a-maint */
//	UBYTE	WeedResponses;					/* 388 Use inactive days against indiv responses */
//	UBYTE	NoSignatures;					/* 389 */
//
//	UBYTE	NoReadCharges;					/* 390 opt to short circuit accounting system */
//	UBYTE	NoWriteCharges;				/* 391 for reading/writing messages */
//
//	long	Arcs;								/* 392 which Arcs in BBSARC are allowed uploaded*/
//
//	short	UpRatioBytes;					/* 396 which set of file/byte ratios to use? */
//	short	UpChargeBytes;					/* 398 which set of charges to use?          */
//	short	UpRatioFiles;					/* 400 */
//	short	UpChargeFiles;					/* 402 */
//
//	UBYTE	Invitation;						/* 404 */
//	UBYTE	UserMustJoin; 					/* 405 */
//	UBYTE	DeleteOwn;    					/* 406 */
//	UBYTE	Marker;							/* 407     0 = Base, 1 = UDBASE, &128 = Killed */
//
//	long	Transforms;						/* 408 which BBSARCS to transform */
//
//	long	ExportTo;						/* 412 */
//
//	UBYTE DIZAlphaNumOnly;				/* 416 - strip everything but alpha and numeric text */
//	UBYTE UseDIZStripChars;				/* 417 - strip the characters specified in BBSTEXT line 2150 */
//	UBYTE UseDIZStripText;				/* 418 - strip any text matching the contents of any line of sysdata:DIZStripText */
//	UBYTE DIZStripCR;						/* 419 - strip carriage return/linefeed from DIZ */
//
//	UBYTE	NNTPMarker;						/* 420 - 0 = no-import/disabled newsgroup
//                                             1 = regular NNTPGet newsgroup - full import of header/body
//                                             2 = mailing list pseudo-newsgroup
//                                             3 = (not yet implemented) NNTPGet headers only - connect to remote host to retrieve body text
//                                             4 = (not yet implemented) No NNTPGet - always connect to remote for article list and body text after scanning articles & presenting to user */
//
//	UBYTE ReformatDIZ;					// 18-Feb-97 reformat DIZ - remove multiple spaces between words,
//												//                sussessive newlines, tabs, etc,..
//
//	UBYTE nada3[2];						// 421 not used currently
//
//	ULONG	count;							/* 424 replaces the _COUNT file */
//	ULONG	hiwater;							/* 428 */
//
//	UBYTE	NetNumber;						/* 432 */ 
//	UBYTE	Locked;							/* 433 Does AMAINT (or other processes) have a lock here? */
//	UBYTE	DLnotifyULer;					/* 434 */
//
//	char	TransformTo[5];				/* 435 */
//
//	UBYTE	QWKReplies;						/* 440 */
//
//	BYTE	SBaseUse;						/* 441  which BBSCHARGE rate to use? */
//	UBYTE	CarbonCopy;						/* 442 */
//	UBYTE	CDROM;							/* 443 copy to hard drive before download? */
//
//	struct	IsDate LastUpload;		/* 444 */
//	struct	IsDate LastMessage;		/* 450*/
//
//	short	Users;							/* 456  how many people are in this sub
//   				                             REAL TIME (not # of members) */
//
//	long	Created;							/* 458  for membership determination */
//
//	short									Parent,		/* 462 - PHYSICAL number of first subboard on the list to which this subboard belongs (all subboards in the ROOT directory should point to the very first physical subboard) */
//											Child,		/* 464 - physnum of first subboard in the list to which this subdirectory houses (-1 if none) */
//											Next;		   /* 466 - physnum of the next subboard on the current list (-1 if none) */
//
//	short	TimeLock;		/* 468 */
//
//	char	ZeroPath[90];	/* 470 */
//	char	Origin  [60];  /* 560 */
//
//	/* if more room is needed, I could put all of this into a pointer to a
//	   'sub-info' structure */
//
//	USHORT MaxItems;	/* 620 */
//	UBYTE	Persist;		/* 622 */
//	UBYTE	Delay;		/* 623 */
//
//	struct	ItemType3 *Item;			/* 624 */
//	struct	ItemHeader *ihead; 
//	struct	MessageType3 *NewMess;
//	struct	SignalSemaphore *sem;
//
//	long	nm;
//	USHORT	rn;
//	USHORT	AllocItems;
//	USHORT	nNewMess;
//
//	UBYTE	resetIP;
//
//	/* diz support! 11-Jun-96 */
//	long	DizTypes;						/* 652 - which BBSARCs to Diz process (v4.20) */
//	UBYTE DizSave;							/* 656 - 0=don't save back to archive, 1=save DIZ back to archive after edit/changed */
//	UBYTE UseSentBy;						/* 657 - 0=don't tack on sentby's, 1=tack sentbys on uploads */
//	UBYTE DizEdit;							/* 658 - allow diz'd discriptions to be altered/edited - maint users have override can be turned of in user prefs */
//
//	LONG	NNTPLastImport;				/* 659 - last NNTP news import date/time */
//
//	char	SubDoing[17];					/* 663 - String to be set for the Where/Action/DOING string in WHO display when use enters this subboard object */
//	BYTE  more_stuff[15];
//	};												/* total length = ??? */
