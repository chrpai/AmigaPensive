#ifndef NOVIA_DOS_H
#define NOVIA_DOS_H
#define NOVIA_DOS_H_VERSION "$VER: 0.59 (23.10.1999)"
#define NOVIA_DOS_VER 59
// (c) Copyright 1996-1999 by Thorsten Gehler
// All rights reserved. Read license.readme for copyright informations.
// VER 0.13 (09.08.1998)
// VER 0.14 (11.08.1998)
// VER 0.15 (17.08.1998)
// VER 0.54 (27.02.1999)
// VER 0.55 (07.03.1999)
// VER 0.56 (18.04.1999)
// VER 0.57 (04.09.1999)
// VER 0.58 (09.10.1999)
// VER 0.59 (23.10.1999)

#ifndef EXEC_LISTS_H
#include <exec/lists.h>
#endif

#ifndef EXEC_PORTS_H
#include <exec/ports.h>
#endif

#ifndef DOS_DOS_H
#include <dos/dos.h>
#endif

#ifndef NOVIA_DATE_H
#include <novia/novia_date.h>
#endif


#define ITEMTYPE_NULL			0
#define ITEMTYPE_SUBBOARD		1
#define ITEMTYPE_MESSAGE		2
#define ITEMTYPE_FILEARCHIV	3
#define ITEMTYPE_EMAIL			4
#define ITEMTYPE_COMMAND		10

#define GENDER_BOTH			0
#define GENDER_FEMALE		'F'
#define GENDER_MALE			'M'

struct ItemInfoBlock
{
	ItemInfoBlock *ln_Succ;		// 4	new since V0.15 (vorher IDNumber)
	ItemInfoBlock *ln_Pred;		// 8	new since V0.15 (vorher IPNumber)
	ULONG	res1;						// 12
	ULONG	res2;						// 16
	ULONG	res3;						// 20
	ULONG	res4;						// 24
	ULONG	IDNumber64;				// 28	offset changed since V0.15, oldoffset was 0
	ULONG	IDNumber;				// 32	reserverd for 64Bit-Filesystem 
	ULONG	IPNumber64;				// 36	offset changed since V0.15, oldoffset was 4
	ULONG	IPNumber;				// 40	

	ULONG	ItemType;				// 44	type of item

	struct	Date ReservedDate;// 56
	struct	Date LastChange;	// 68
	struct	Date CreateDate;	// 80	true writing date

	ULONG	Size64;					// 84	sizeof File or size of files in subboard
	ULONG	Size;						// 88	for 64Bit Filesystems or soft-dirs (FTP-Server)

	ULONG	NumBlocks64;			// 92	copy of FileInfoBlock->fib_NumBlocks
	ULONG	NumBlocks;				// 96	for 64Bit Filesystem (not yet supported)

	ULONG	Protection;				// 100  DOS-Protection Bits

	UWORD	UID;						// 102  UUCP-UserID
	UWORD	GID;						// 104	UUCP-GroupID

	ULONG	AccessLevel;			// 108	min Access

	ULONG	PostAccess;				// 112 groups which may Post
	ULONG	UploadAccess;			// 116 ... Upload/Respong access
	ULONG	DownloadAccess;		// 120 ... Download/Read access
	ULONG	ComputerTypes;			// 124

	UBYTE	NoItems;					// 125 user can't add a item/reply to subboard/item
	UBYTE	itsme1;					// 126 for future
	UBYTE	itsme2;					// 127 for future
	UBYTE	itsme3;					// 128 for future
	ULONG	ReplyAccess;			// 132
	UBYTE	Deleted;					// 133 Item is deleted
	UBYTE	Closed;					// 134 Closed

	UWORD	MinEnter;				// 136 minutes must online before enter of read subboard/item

	UBYTE	Youngest;				// 137
	UBYTE	Oldest;					// 138
	UBYTE	Gender;					// 139
	UBYTE	DefDropped;				// 140

	UWORD	MaxMins;					// 142 User can't read/enter item/subboard after ...
	UWORD	FreeDays;				// 144 files are free in ... days

	UWORD	DownBytes;				// 146 x of download bytes to subtract
	UWORD	DownFiles;				// 148 # of files to subtract per download
	UWORD	TimeCredit;				// 150 % of time to give back for uploads
	UBYTE	ArcTransform;			// 151 Transform to archiveformat
	UBYTE	RealNames;				// 152 use real names instead of Handles? 
	UBYTE	PrivateMails;			// 153
	UBYTE	NoSignatures;			// 154
	UBYTE	Anonymous;				// 155
	UBYTE	Override;				// 156 how to set the use defaults flags in newly uploaded items 

	ULONG	CreateUserIP;			// 160
	ULONG	LastChangeUserIP;		// 164
	char	Title[128];				// 292 viewtitle
	MinList OpenList;				// 304 List of port of open this file
	char	Filename[108];			// 412 copy of FileInfoBlock->fib_FileName
	char	Comment[80];			// 492 copy of FileInfoBlock->fib_Comment
	char	*fullpath;				// 496 full filename
	ULONG	OpenCounter;			// 500 OpenCounter
	char	ReservedBytes[12];	// 512
};

struct Directory
{
	struct	ItemInfoBlock iib;// 512
	ULONG	MinFreeBytes;			// 516    to upload in this subboard 
	UBYTE	BufferType;				// 517	0 - no buffering
										//		1 - copy to ram before download
										//		2 - copy to harddisk before download

	UBYTE	ArcAvailable;			// 518  Transform to archiveformat
	UBYTE	QWKAvailable;			// 519 use real names instead of Handles? 

	UBYTE	Verification;			// 520
	UBYTE	ShowUnvalidated;		// 521
	UBYTE	NoMCI;					// 522

	UBYTE	PrivateArea;			// 523  0/1 No/Yes ... 2==FORCE 
	char	Password[11];			// 534
	char	Encrypt[11];			// 545
	UBYTE	TestFiles;				// 546  test at upload time 

	UBYTE	FileTransform;			// 547 run script at upload time 
	UBYTE	ChangeOwnItems;		// 548  Use inactive days against indiv responses 

	UWORD 	NetNumber;			// 550
	UWORD	UpFileRatio;			// 552 Which file uploads ratio 
	UWORD	UpFileCharge;			// 554 which set of charges to use?          
	UWORD	UpByteRatio;			// 556
	UWORD	UpByteCharge;			// 558
	UWORD	UseRate;					// 560  default download rate
	
	ULONG	Language;				// 564  which BBSARCS to transform 

	ULONG	RecordLog;				// 568
	ULONG	LogfileID;				// 572
	
	ULONG	ItemEntrys;				// 576
	ULONG	DirEntrys;				// 580
	UWORD	DelMails;				// 582
	UWORD	DelFiles;				// 584
	UBYTE	AddressMessages;		// 585
	UBYTE	DupeCheck;				// 586
	UBYTE	NoPostCharges;			// 587
	UBYTE	PurgeOld;				// 588
	UBYTE	InvitatonOnly;			// 589
	UBYTE	NewVotes;				// 590
	UBYTE	NetworkType;			// 591
	UBYTE	reserved06172_338956;// 592
	ULONG	HoursOpen;				// 596
	ULONG	MaxItems;				// 600 Maximum Dirs & Items... in this subboard, 0 = no maximum
	char	Scanfilter[4];			// 604
	UBYTE	NewFilesValidation;	// 605
	UBYTE	AMaintAdoptOrphans;	// 606
	UBYTE	KillOwnFiles;			// 607
	UBYTE	localdir;
	MinList entrylist;			// 628 new sience V0.57 !!!
	BPTR	fh_messages;			
	BPTR	fh_files;
	BPTR	fh_dirs;
	BPTR	fh_text;
	BPTR	fh_replys;
	MinList subdirlist;			// 652 List of opened Sub-Directorys
	Directory *parrentdir;		// 656 pointer of parrent Directory-structure
	char	*parrentdir_name;		// 660
};


struct ItemEntry {			
	struct  ItemInfoBlock iib;	// 512  
	Date	SendDate;				// 524  import date in database 
	Date	ExpirationDate;		// 536  auto-killing on ... 
	UWORD	ExpirationDays;		// 538	auto-killing in .. days 
	UBYTE	res;						// 539  reserved
	UBYTE	NetworkType;			// 540	>0 if uucp, fido or other mails
	ULONG	ItemType;				// 544	0 - mail
										//		   1 - archive

	ULONG	ToUser;					// 548  internal-items only
	ULONG	FromUser;				// 552  internal-items only
	ULONG	RealUser;				// 556  normaly a copy of FromUser. you can use this entry, if writers name is an alias or anonymous

	char	FromRealName[40];		// 676  for FIDO,Z-Net,SMTP (RFC 821,822) Messages or other imported messages
	char	From[80];				// 676  for FIDO,Z-Net,SMTP (RFC 821,822) Messages or other imported messages
	char	ToRealName[40];		// 796  for FIDO,Z-Net,SMTP (RFC 821,822) Messages or other imported messages
	char	To[80];					// 796  for FIDO,Z-Net,SMTP (RFC 821,822) Messages or other imported messages
	char	Alias[80];				// 876  alias name 

	char	anonymous;				// 877
	char	MailListMail;			// 878  MailList-Mail 
	char	ReservedFlags[6];		// 884 

	char	Filename[128];			// 1012 for fileitems only
	ULONG	FileVersion;			// 1016 Version of file 
	ULONG	MailListID;				// 1020 MailList ID Number 

	ULONG downloads;				// 1024

	ULONG	length;					// 1028
	ULONG	DirIP;					// 1032 serial directory number				// New in V0.14
	ULONG	replys;					// 1036
	char  *text;					// 1040
	ULONG	besttransfer;			// 1044
	ULONG first_reply;			// 1048
	ULONG	text_seek;				// 1052 Startseek in [Userpath]/MailData 
	UBYTE	resered[148];			// 1200
};


struct CommandEntry
{
	struct  ItemInfoBlock iib;	// 512  
	Date	SendDate;				// 524  import date in database 
	Date	ExpirationDate;		// 536  auto-killing on ... 
	UWORD	ExpirationDays;		// 538	auto-killing in .. days 
	UBYTE	NetworkType;			// 539	>0 if uucp, fido or other mails
	UBYTE	internal;				// 540  internal command
	ULONG	CommandID;				// 544  internal command id
	char	FromRealName[40];		// 584  for FIDO,Z-Net,SMTP (RFC 821,822) Messages or other imported messages
	char	From[80];				// 664  for FIDO,Z-Net,SMTP (RFC 821,822) Messages or other imported messages
	char	Alias[80];				// 744  alias name 
	ULONG	Version;					// 748  Version of command 
	ULONG runs;						// 752
	char	reserved1[256];		// 1008 for fileitems only
	UBYTE	reserved2[192];		// 1200
};


struct ReplyEntry {
	ULONG	IDNumber;				// 4
	ULONG	IPNumber;				// 8

	UBYTE	RealNames;				// 9 use real names instead of Handles? fileinfoblock
	UBYTE	PrivateMails;			// 10
	UBYTE	NoSignatures;			// 11
	UBYTE	Anonymous;				// 12
	UBYTE	Override;				// 13 how to set the use defaults flags in newly uploaded items 
	UBYTE	Deleted;					// 14 Item is deleted
	UBYTE	NoEdit;					// 15 User can't edit this mail 
	UBYTE	AnonymousMail;			// 16 
	UBYTE	MailListMail;			// 17 MailList-Mail 

	UBYTE	Reserved;				// 18
	UBYTE	TxtAdded;				// 19
	UBYTE	FileMail;				// 20 if TRUE txt=distrution
	char	FileName[40];			// 60

	Date	LastChange;				// 72
	Date	CreateDate;				// 84 true writing date

	ULONG	ToUser;					// 88 internal-items only 
	ULONG	FromUser;				// 92 internal-items only 
	Date	ExpirationDate;		// 104 auto-killing on ... 
	short	ExpirationDays;		// 106 auto-killing in .. days 
	
	char	From[80];				// 186 or alias name 
	char	FromRealName[40];		// 226
	char	To[80];					// 306
	char 	ToRealName[40];		// 346
	char	Alias[80];				// 426
	UBYTE	Reservedbytes[8];		// 434
	ULONG	length;					// 438
	Date	SendDate;				// 450 // new since V0.15 (imported date)
	ULONG unknown;					// 454
	ULONG next_reply;				// 458
	ULONG	text_seek;				// 462
	UBYTE res[24];					// 486
};

struct OpenEntry					// for ItemInfoBlock->OpenList
{
	struct OpenEntry 	*ln_Succ;// next OpenEntry
	struct OpenEntry	*ln_Pred;// back OpenEntry
	struct PortData		*cport;// Pointer to cport
};


/* you must use this structure to add new items */

struct NewItem
{
	ULONG	ItemType;				// 4	type of item
	struct	Date CreateDate;	// 12	true writing date
	ULONG	Protection;				// 16   DOS-Protection Bits
	UWORD	UID;						// 18   UUCP-UserID		not yet used.
	UWORD	GID;						// 20	UUCP-GroupID    not yet used
	ULONG	CreateUserIP;			// 24
	char	Subject[80];			// 104  Subject of mail
	char	Filename[108];			// 212  copy of FileInfoBlock->fib_FileName
	char	FromRealName[40];		// 676  for FIDO,Z-Net,SMTP (RFC 821,822) Messages or other imported messages
	char	From[80];				// 676  for FIDO,Z-Net,SMTP (RFC 821,822) Messages or other imported messages
	char	ToRealName[40];		// 796  for FIDO,Z-Net,SMTP (RFC 821,822) Messages or other imported messages
	char	To[80];					// 796  for FIDO,Z-Net,SMTP (RFC 821,822) Messages or other imported messages
	char	Alias[80];				// 532  alias name 
	ULONG	ToUser;					// 536	for internal-items only
	ULONG	FromUser;				// 540  for internal-items only
	char	anonymous;				// 541
	char	MailListMail;			// 542  MailList-Mail 
	char	ReservedFlags[6];		// 548 
	ULONG	FileVersion;			// 552  Version of file 
	ULONG	MailListID;				// 556  MailList ID Number 
	char	*text;					// 560  pointer of mailtext
};

struct Select
{
	struct Select *ln_Succ;
	struct Select *ln_Pred;
	ULONG			directoryIP64;
	ULONG			directoryIP;
	ULONG			itemIP64;
	ULONG			itemIP; 
	ULONG			replyIP64;
	ULONG			replyIP;
	UBYTE			downloadnow;	// download Item now.
	UBYTE			reserved[3];
	char			*dirpath;
	char			*filepath;
};

struct SelectListFileHeader
{
	ULONG			res[5];
	ULONG			entrys;
};

struct ItemEntryBlock
{
	ItemEntryBlock *ln_Succ;	// 4
	ItemEntryBlock *ln_Pred;	// 8
	ULONG	IDNumber64;				// 12
	ULONG	IDNumber;				// 16	reserverd for 64Bit-Filesystem 
	ULONG	IPNumber64;				// 20	offset changed since V0.15, oldoffset was 4
	ULONG	IPNumber;				// 24	

	ULONG	ItemType;				// 28	type of item

	ULONG	FileType;				// 32	0 - mail
										//		1 - archive

	struct	Date LastChange;	// 44
	struct	Date CreateDate;	// 56	true writing date

	ULONG	Size64;					// 60	sizeof File or size of files in subboard
	ULONG	Size;						// 64	for 64Bit Filesystems or soft-dirs (FTP-Server)

	ULONG	NumBlocks64;			// 68	copy of FileInfoBlock->fib_NumBlocks
	ULONG	NumBlocks;				// 72	for 64Bit Filesystem (not yet supported)

	ULONG	Protection;				// 76   DOS-Protection Bits

	UWORD	UID;						// 78   UUCP-UserID
	UWORD	GID;						// 80   UUCP-GroupID

	ULONG	AccessLevel;			// 84   min Access

	ULONG	PostAccess;				// 88   groups which may Post
	ULONG	UploadAccess;			// 92  ... Upload/Respong access
	ULONG	DownloadAccess;		// 96  ... Download/Read access
	ULONG	ComputerTypes;			// 100

	ULONG	ReplyAccess;			// 104

	ULONG	CreateUserIP;			// 108
	char	*Title;					// 112 viewtitle
	char	*Comment;				// 116 copy of FileInfoBlock->fib_Comment
	char	*filename;				// 124

	UBYTE	NetworkType;			// 125	>0 if uucp, fido or other mails
	UBYTE	selected;				// 126
	UBYTE	closed;					// 127
	UBYTE	res;						// 128

	ULONG	ItemEntrys;				// 132  if Directorys, number of Items, if Item, number of Replys
	ULONG	DirEntrys;				// 136  only used by Directorys
	ULONG	replys;					// 140  only Items

	char	FromRealName[40];		// 180  for FIDO,Z-Net,SMTP (RFC 821,822) Messages or other imported messages
	char	From[80];				// 260  for FIDO,Z-Net,SMTP (RFC 821,822) Messages or other imported messages
	char	ToRealName[40];		// 300  for FIDO,Z-Net,SMTP (RFC 821,822) Messages or other imported messages
	char	To[80];					// 380  for FIDO,Z-Net,SMTP (RFC 821,822) Messages or other imported messages

	ULONG	FileVersion;			// 384 Version of file 
	ULONG besttransfer;
	ULONG downloads;
};

struct ItemEntryList
{
	struct ItemEntryBlock	*lh_Head;
	struct ItemEntryBlock	*lh_Tail;
	struct ItemEntryBlock	*lh_TailPred;
	ULONG					dirs;
	ULONG					files;
	ULONG					size;
	ULONG					IDCounter;
	ULONG					res[20];
	
};

#define ACTION_NFS_CLOSEROOT			2000
#define ACTION_NFS_OPENROOT			2001
#define ACTION_NFS_MAKEDIR				2002
#define ACTION_NFS_PARENTDIR			2003
#define ACTION_NFS_CHANGEDIR			2004
#define ACTION_NFS_DELETE				2005
#define ACTION_NFS_ADDMAIL				2006
#define ACTION_NFS_ADDREPLY			2007
#define ACTION_NFS_LOADSELECTLIST	2008
#define ACTION_NFS_SAVESELECTLIST	2009
#define ACTION_NFS_GETLIST				2010
#define ACTION_NFS_ADDITEMENTRY		2011
#define ACTION_NFS_ADDITEMENTRY2DIR	2012
#define ACTION_NFS_ADDMAIL2DIR		2013
#define ACTION_NFS_CLOSEDIR			2014
#define ACTION_NFS_SENDMAIL			2015
#define ACTION_NFS_RENAME				2016
#define ACTION_NFS_GET_IEB				2017
#define ACTION_NFS_UUCP_IMPORT_MAIL	2018

#define ACTION_LOWLEVEL_MAKE_FILESYSTEM	3001
#define ACTION_LOWLEVEL_MAKE_ROOT			3002
#define ACTION_LOWLEVEL_ADD_DIRECTORY		3003
#define ACTION_LOWLEVEL_ADD_LINK				3004
#define ACTION_LOWLEVEL_ADD_COMMAND			3005


#define RESULT_NFS_TEXTNOTIFY			500

struct NFSPacket
{
	Message		nfs_msg;
	PortData 	*nfs_cport;
	LONG			nfs_type;
	LONG			nfs_arg1;
	LONG			nfs_arg2;
	LONG			nfs_arg3;
	LONG			nfs_arg4;
	LONG			nfs_arg5;
	LONG			nfs_arg6;
	LONG			nfs_result1;
	APTR			nfs_result2;
	UBYTE			nfs_inprocess;
};

struct NFSServer
{
	MsgPort		*nfs_port;
	MsgPort		*controlport;
	Task			*nfstask;
	UBYTE			CloseServer;
	UBYTE			ServerOpen;
};

#endif /* NOVIA_DOS_H */

/* Amiga DOS ERROR - Codes 
	
	include: dos/dos.h

	#define ERROR_NO_FREE_STORE				103
	#define ERROR_TASK_TABLE_FULL				105
	#define ERROR_BAD_TEMPLATE					114
	#define ERROR_BAD_NUMBER					115
	#define ERROR_REQUIRED_ARG_MISSING		116
	#define ERROR_KEY_NEEDS_ARG				117
	#define ERROR_TOO_MANY_ARGS				118
	#define ERROR_UNMATCHED_QUOTES			119
	#define ERROR_LINE_TOO_LONG				120
	#define ERROR_FILE_NOT_OBJECT				121
	#define ERROR_INVALID_RESIDENT_LIBRARY	122
	#define ERROR_NO_DEFAULT_DIR				201
	#define ERROR_OBJECT_IN_USE				202
	#define ERROR_OBJECT_EXISTS				203
	#define ERROR_DIR_NOT_FOUND				204
	#define ERROR_OBJECT_NOT_FOUND			205
	#define ERROR_BAD_STREAM_NAME				206
	#define ERROR_OBJECT_TOO_LARGE			207
	#define ERROR_ACTION_NOT_KNOWN			209
	#define ERROR_INVALID_COMPONENT_NAME	210
	#define ERROR_INVALID_LOCK					211
	#define ERROR_OBJECT_WRONG_TYPE			212
	#define ERROR_DISK_NOT_VALIDATED			213
	#define ERROR_DISK_WRITE_PROTECTED		214
	#define ERROR_RENAME_ACROSS_DEVICES		215
	#define ERROR_DIRECTORY_NOT_EMPTY		216
	#define ERROR_TOO_MANY_LEVELS				217
	#define ERROR_DEVICE_NOT_MOUNTED			218
	#define ERROR_SEEK_ERROR					219
	#define ERROR_COMMENT_TOO_BIG				220
	#define ERROR_DISK_FULL						221
	#define ERROR_DELETE_PROTECTED			222
	#define ERROR_WRITE_PROTECTED				223
	#define ERROR_READ_PROTECTED				224
	#define ERROR_NOT_A_DOS_DISK				225
	#define ERROR_NO_DISK						226
	#define ERROR_NO_MORE_ENTRIES				232
	#define ERROR_IS_SOFT_LINK					233
	#define ERROR_OBJECT_LINKED				234
	#define ERROR_BAD_HUNK						235
	#define ERROR_NOT_IMPLEMENTED				236
	#define ERROR_RECORD_NOT_LOCKED			240
	#define ERROR_LOCK_COLLISION				241
	#define ERROR_LOCK_TIMEOUT					242
	#define ERROR_UNLOCK_ERROR					243

*/