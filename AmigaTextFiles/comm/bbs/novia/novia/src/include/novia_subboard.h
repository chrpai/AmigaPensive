#ifndef NOVIA_SUBBOARD_H
#define NOVIA_SUBBOARD_H
#define NOVIA_SUBBOARD_H_VERSION "$VER: 0.14 (17.01.1999)"
#define NOVIA_SUBBOARD_VER 14
// (c) Copyright 1996-1999 by Thorsten Gehler
// All rights reserved. Read license.readme for copyright informations.
// VER 0.13 (09.08.1998)
// VER 0.14 (11.08.1998)
// VER 0.15 (17.08.1998)

#ifndef NOVIA_DATE_H
#include <novia/novia_date.h>
#endif

#ifndef NOVIA_DOS_H
#include <novia/novia_dos.h>
#endif

/*

	Format of File: #?.index

	Offset	Type			Comment
	-------------------------------
	000-007	ULONGLONG	-   ID-Number of file			\____Header = 12Bytes
	008-011	ULONG		-	Entrys in this indexfile	/
	-------------------------------
	+0		ULONG		-	Offset in #?.data			\
	+4		ULONG		-	Reserved					 \
	+8		ULONG		-	Reserved					  \__Entry  = 20Bytes = struct IndexEntry 
	+12		UBYTE		-	type of data-structure		  /
	+13		7 Bytes		-	Reserved					 /
														/
*/

#define ITEMTYPE_NULL		0
#define ITEMTYPE_SUBBOARD	1
#define	ITEMTYPE_ITEM		2
#define	ITEMTYPE_FILEITEM	3

#define GENDER_BOTH			0
#define GENDER_FEMALE		'F'
#define GENDER_MALE			'M'

struct ItemHeader
{
	ItemHeader *ln_Succ;	// 4	// new since V0.15 (vorher IDNumber)
	ItemHeader *ln_Pred;	// 8	// new since V0.15 (vorher IPNumber)
	ULONG	IDNumber;		// 12	// offset changed since V0.15, oldoffset was 0
	ULONG	IPNumber;		// 16	// offset changed since V0.15, oldoffset was 4

	BYTE	ItemType;		// 17

	char	Title[80];		// 97
	char	Path[79];		// 176 SUBBOARD -> name of subdirectory from parentdir example: "base/"
							//     ITEM     -> name of prefix for .itemindex|.itemdata

	struct	Date LastChange;// 188
	struct	Date CreateDate;// 200 true writing date
	ULONG	AccessLevel;	// 204 min Access

	ULONG	PostAccess;		// 208 groups which may Post
	ULONG	UploadAccess;	// 212 ... Upload/Respong access
	ULONG	DownloadAccess;	// 216 ... Download/Read access
	ULONG	ComputerTypes;	// 220

	UBYTE	NoItems;		// 221 user can't add a item/reply to subboard/item
	UBYTE	itsme1;			// 222 for future
	UBYTE	itsme2;			// 223 for future
	UBYTE	itsme3;			// 224 for future
	ULONG	ReplyAccess;	// 228
	UBYTE	Deleted;		// 229 Item is deleted
	UBYTE	Closed;			// 230 Closed

	UWORD	MinEnter;		// 232 minutes must online before enter of read subboard/item

	UBYTE	Youngest;		// 233
	UBYTE	Oldest;			// 234
	UBYTE	Gender;			// 235
	UBYTE	DefDropped;		// 236

	UWORD	MaxMins;		// 238 User can't read/enter item/subboard after ...
	UWORD	FreeDays;		// 240 files are free in ... days

	UWORD	DownBytes;		// 242 x of download bytes to subtract
	UWORD	DownFiles;		// 244 # of files to subtract per download
	UWORD	TimeCredit;		// 246 % of time to give back for uploads
	UBYTE	ArcTransform;	// 247 Transform to archiveformat
	UBYTE	RealNames;		// 248 use real names instead of Handles? 
	UBYTE	PrivateMails;	// 249
	UBYTE	NoSignatures;	// 250
	UBYTE	Anonymous;		// 251
	UBYTE	Override;		// 252 how to set the use defaults flags in newly uploaded items 
	ULONG	IndexOffset;	// 256
};

struct Subboard
{
	struct	ItemHeader itemheader;	// 256
	ULONG	MinFreeBytes;			// 260 to upload in this subboard 
	UBYTE	BufferType;				// 261	0 - no buffering
									//		1 - copy to ram before download
									//		2 - copy to harddisk before download

	UBYTE	ArcAvailable;			// 262 Transform to archiveformat
	UBYTE	QWKAvailable;			// 263 use real names instead of Handles? 

	UBYTE	Verification;			// 264
	UBYTE	ShowUnvalidated;		// 265
	UBYTE	NoMCI;					// 266

	UBYTE	PrivateArea;			// 267 0/1 No/Yes ... 2==FORCE 
	char	Password[11];			// 278
	char	Encrypt[11];			// 289
	UBYTE	TestFiles;				// 290 test at upload time 

	UBYTE	FileTransform;			// 291 run script at upload time 
	UBYTE	ChangeOwnItems;			// 292 Use inactive days against indiv responses 

	UWORD 	NetNumber;				// 294
	UWORD	UpFileRatio;			// 296 Which file uploads ratio 
	UWORD	UpFileCharge;			// 298 which set of charges to use?          
	UWORD	UpByteRatio;			// 300 
	UWORD	UpByteCharge;			// 302 
	UWORD	UseRate;				// 304 default download rate

	ULONG	Language;				// 308 which BBSARCS to transform 

	ULONG	RecordLog;				// 312
	ULONG	LogfileID;				// 316
	
	ULONG	ItemEntrys;				// 320
	UWORD	DelMails;				// 322
	UWORD	DelFiles;				// 324
	UBYTE	AddressMessages;		// 325
	UBYTE	DupeCheck;				// 326
	UBYTE	NoPostCharges;			// 327
	UBYTE	PurgeOld;				// 328
	UBYTE	InvitatonOnly;			// 329
	UBYTE	NewVotes;				// 330
	UBYTE	NetworkType;			// 331
	UBYTE	reserved06172_338956;	// 332

	ULONG	HoursOpen;				// 336
	ULONG	MaxItems;				// 340
	char	Scanfilter[4];			// 344
	UBYTE	NewFilesValidation;		// 345
	UBYTE	AMaintAdoptOrphans;		// 346
	UBYTE	KillOwnFiles;			// 347
	char	reserved[49];			// 396

	ULONG	Items;					// 400 Number of Items
};


struct Item {			
	struct  ItemHeader itemheader;	// 256
	ULONG	ToUser;					// 260 internal-items only 
	ULONG	FromUser;				// 264 internal-items only 
	Date	SendDate;				// 276 import date in database 
	ULONG	Size;					// 288
	ULONG	reserved1;				// 292
	ULONG	reserved2;				// 296
	Date	ExpirationDate;			// 300 auto-killing on ... 
	short	ExpirationDays;			// 302 auto-killing in .. days 

	UBYTE	ItemType;				// 303	0 - Message
									//		1-  File

	char	Name[80];				// 383
	char	RealName[41];			// 424 
	char	From[80];				// 504 or alias name 
	char	FromRealName[41];		// 545
	char	To[80];					// 625 
	char 	ToRealName[41];			// 666 
	char	Alias[80];				// 746 

	char	NoEdit;					// 747 User can't edit this mail 
	char	MailListMail;			// 748 MailList-Mail 
	char	AnonymousMail;			// 749 
	char	ReservedFlags[7];		// 756 
	char	Filename[40];			// 796 for fileitems only
	long	FileVersion;			// 800 Version of file 
	long	MailListID;				// 804 MailList ID Number 
	long	seek;					// 808 Startseek in [Userpath]/MailData 
	long	length;					// 812 
	ULONG	DirIP;					// 816 serial directory number				// New in V0.14
	ULONG	replys;					// 820
};

struct Reply {
	ULONG	IDNumber;				// 4
	ULONG	IPNumber;				// 8

	UBYTE	RealNames;				// 9 use real names instead of Handles? 
	UBYTE	PrivateMails;			// 10
	UBYTE	NoSignatures;			// 11
	UBYTE	Anonymous;				// 12
	UBYTE	Override;				// 13 how to set the use defaults flags in newly uploaded items 
	UBYTE	Deleted;				// 14 Item is deleted
	UBYTE	NoEdit;					// 15 User can't edit this mail 
	UBYTE	AnonymousMail;			// 16 
	UBYTE	MailListMail;			// 17 MailList-Mail 

	UBYTE	Reserved;				// 18
	UBYTE	TxtAdded;				// 19
	UBYTE	FileMail;				// 20 if TRUE txt=distrution

	char	FileName[40];			// 60

	Date	LastChange;		// 72
	Date	CreateDate;		// 84 true writing date

	ULONG	ToUser;					// 88 internal-items only 
	ULONG	FromUser;				// 92 internal-items only 
	Date	ExpirationDate;			// 104 auto-killing on ... 
	short	ExpirationDays;			// 106 auto-killing in .. days 
	
	char	Name[80];				// 186
	char	RealName[41];			// 227 
	char	From[80];				// 307 or alias name 
	char	FromRealName[41];		// 348
	char	To[80];					// 428 
	char 	ToRealName[41];			// 469 
	char	Alias[80];				// 549
	char	reservdfgfdfd[3];		// 552

	ULONG	seek;					// 556
	ULONG	length;					// 560
	Date	SendDate;				// 572 // new since V0.15 (imported date)
	UBYTE	Reservedbytes[28];		// 600
};

struct ViewItem
{
	struct	ViewItem	*ln_Succ;	// 4	Forward Item Node
	struct	ViewItem	*ln_Pred;	// 8	Next	Item
	ULONG	ListID;					// 12	// Itemnumber of Display
	ULONG	IndexID;				// 16	// Real Itemnumber in indexfile
	ULONG	Seek;					// 20	// pos in .data       (copy of .index-entry)
	ULONG	Size;					// 24
	UBYTE	ItemType;				// 25	// Type of structure  (copy of .index-entry)
	char	Title[150];				// 175	// Titel
	ULONG	ItemIP;					// 176	// item serial number		// New in V0.14
	UBYTE	select;					// 180	// is item select ??		// New in V0.14
};

struct ViewList
{
	struct	ViewItem	*lh_Head;
	struct	ViewItem	*lh_Tail;
	struct	ViewItem	*lh_TailPred;
	ULONG	entrys;
};

struct ViewTable
{
	struct ViewItem *item;	// pointer to table
	ULONG  items;
	ULONG  maxitems;
};


struct IndexEntry
{
	ULONG		offset;				// 4 Offset in #?.data
	ULONG		reserved1;			// 8
	ULONG		IPNumber;			// 12	same as itemheader->IPNumber
	UBYTE		type;				// 13 Type of struture
	UBYTE		reserved3[7];		// 20
};

struct IndexHeader
{
	ULONG		IDNumber;			// Offset 0
	ULONG		IPNumber;			// Offset 4
	ULONG		idcounter;			// Offset 8
	ULONG		Entrys;				// Offset 12
	ULONG		reserved;			// Offset 16 end 20
};

/********************************************** New in V0.14 (11.08.1998) *******************************************/

struct ItemIndexHeader				// 
{

	ULONG		version;			// File version
	ULONG		entrys;				// Directory entrys
	ULONG		counter64;			// reserved for 64bit serials
	ULONG		counter;			// highest serial number (new serialnumber = counter+1)
};

struct ItemIndexEntry
{
	ULONG		checksum;
	ULONG		IPNumber64;			// reserved for future 64Bit serials
	ULONG		IPNumber;			// serial number of Item
	ULONG		seek;				// seek in /data/.data
};

struct DirectoryIndexHeader	
{

	ULONG		version;			// File version
	ULONG		entrys;				// Directory entrys
	ULONG		counter64;			// reserved for 64 bit serials
	ULONG		counter;			// highest serial number (new serialnumber = counter+1)
};

struct DirIndexEntry
{
	ULONG		checksum;
	ULONG		IPNumber64;			// reserved for future 64Bit serials
	ULONG		IPNumber;			// serial number of Directory
	ULONG		seek;				// seek in sysdata/sys.dirtable.names (real pathname)
	ULONG		dataseek;			// seek in sysdata/sys.dirtable.names (name of Directory-structure-path, without /data)
};

struct SelectItem
{
	struct SelectItem *ln_Succ;
	struct SelectItem *ln_Pred;
	ULONG		dirIP;
	ULONG		itemIP;
	ULONG		replyIP;
	UBYTE		downloadnow;		// download Item now.
	UBYTE		reserved[3];
};



#endif