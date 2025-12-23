#ifndef NOVIA_MAIL_H
#define NOVIA_MAIL_H
#define NOVIA_MAIL_H_VERSION "$VER: 0.01 (08.05.1998)"
// (c) Copyright 1996-1998 by Thorsten Gehler
// All rights reserved. Read license.readme for copyright informations.

#ifndef NOVIA_DATE_H
#include <novia/novia_date.h>
#endif

#ifndef NOVIA_DOS_H
#include <novia/novia_dos.h>
#endif

#define MAILTYPE_NOVIA	1
#define MAILTYPE_UUCP	2
#define MAILTYPE_FIDO	3


#define MAIL_IMPORTANCE_LOW		-1
#define MAIL_IMPORTANCE_NORMAL	0
#define MAIL_IMPORTANCE_HIGH		1


struct MailEntry
{
	struct ItemInfoBlock iib;	// 512
	Date	SendDate;				// 524  import date in database 
	Date	ExpirationDate;		// 536  auto-killing on ... 
	UWORD	ExpirationDays;		// 538	auto-killing in .. days 
	UBYTE	importance;				// 539  priority of mail
	UBYTE	NetworkType;			// 540	>0 if uucp, fido or other mails
	ULONG	ItemType;				// 544	0 - mail
	ULONG	replys;					// 548
	ULONG ReplyToUser;			// 552
	ULONG ReplyToList;			// 556
	ULONG ReturnToUser;			// 560
	ULONG ReturnToList;			// 564
	UBYTE Receipt;					// 565
	UBYTE	Return;					// 566
	UBYTE copy_own;				// 567
	UBYTE	Anonymous;				// 568
										//			1 - archive
	ULONG	ToUser;					// 572  internal-items only
	ULONG ToUserList;				// 576
	ULONG	FromUser;				// 580  internal-items only
	ULONG	RealUser;				// 584  normaly a copy of FromUser. you can use this entry, if writers name is an alias or anonymous
	ULONG	CCUser;					// 588
	ULONG CCUserList;				// 592
	ULONG BCCUser;					// 596
	ULONG BCCUserList;			// 600

	char	FromRealName[40];		// 640  for FIDO,Z-Net,SMTP (RFC 821/822) Messages or other imported messages
	char	From[80];				// 720  for FIDO,Z-Net,SMTP (RFC 821/822) Messages or other imported messages
	char	ToRealName[40];		// 760  for FIDO,Z-Net,SMTP (RFC 821/822) Messages or other imported messages
	char	To[80];					// 840  for FIDO,Z-Net,SMTP (RFC 821/822) Messages or other imported messages
	char	Alias[80];				// 920  alias name 
	char	CC[80];					// 1000
	char  BCC[80];					// 1080

	ULONG	FileVersion;			// 1084 Version of file 
	ULONG	MailListID;				// 1088 MailList ID Number 

	char  *text;					// 1092
	ULONG headersize;				// 1096
	ULONG	text_seek;				// 1100 Startseek in [Userpath]/MailData 
	ULONG	length;					// 1104
	ULONG besttransfer;			// 1108
	ULONG	downloads;				// 1112
	ULONG DirIP;					// 1116
	ULONG	first_reply;			// 1120
	char  Organization[80];		// 1200
};

struct NewMail
{
	ULONG	PostAccess;			// 112 groups which may Post
	ULONG	UploadAccess;		// 116 ... Upload/Respong access
	ULONG	DownloadAccess;	// 120 ... Download/Read access
	ULONG	ComputerTypes;		// 124

	UBYTE	NoItems;				// 125 user can't add a item/reply to subboard/item
	ULONG	ReplyAccess;		// 132
	UBYTE	PrivateMail;		// 153
	UBYTE	NoSignatures;		// 154
	UBYTE	Anonymous;			// 155

	ULONG	CreateUserIP;		// 160
	char	subject[128];		// 292 viewtitle
	char	Filename[108];		// 412 copy of FileInfoBlock->fib_FileName
	char	Comment[80];		// 492 copy of FileInfoBlock->fib_Comment
	Date	SendDate;			// 524  import date in database 
	Date	ExpirationDate;	// 536  auto-killing on ... 
	UWORD	ExpirationDays;	// 538	auto-killing in .. days 
	UBYTE	MailType;			// 539  reserved
	UBYTE	NetworkType;		// 540	>0 if uucp, fido or other mails
	ULONG	ItemType;			// 544	0 - mail
									//		1 - archive

	ULONG	ToUser;					// 548  internal-items only
	ULONG	FromUser;				// 552  internal-items only
	ULONG	RealUser;				// 556  normaly a copy of FromUser. you can use this entry, if writers name is an alias or anonymous
	ULONG CCUser;
	ULONG BCCUser;

	char	*FromRealName;		// 676  for FIDO,Z-Net,SMTP (RFC 821/822) Messages or other imported messages
	char	*From;				// 676  for FIDO,Z-Net,SMTP (RFC 821/822) Messages or other imported messages
	char	*ToRealName;		// 796  for FIDO,Z-Net,SMTP (RFC 821/822) Messages or other imported messages
	char	*To;						// 796  for FIDO,Z-Net,SMTP (RFC 821/822) Messages or other imported messages
	char	*Alias;				// 876  alias name 
	char  *CC;
	char	*BCC;

	char	MailListMail;			// 878 MailList-Mail 
	char	ReservedFlags[2];		// 880
	char	*Organization;

	ULONG	FileVersion;			// 1016 Version of file 
	ULONG	MailListID;				// 1020 MailList ID Number 
	char	*text;					// 1024 Startseek in [Userpath]/MailData 
	ULONG	length;					// 1028
	ULONG	DirIP;					// 1032 serial directory number				// New in V0.14
	ULONG	replys;					// 1036
	ULONG ReplyToUser;
	ULONG ReturnToUser;
	UBYTE Receipt;
	UBYTE	Return;
	UBYTE FileMail;
	UBYTE copy_own;
	BYTE	importance;
};

struct INet_Field_Entry
{
	struct INet_Field_Entry *ln_Succ;	// next user
	struct INet_Field_Entry *ln_Pred;	// next user
	ULONG	 fieldid;
	ULONG  fieldtype;
};


struct INet_User_Address
{
	INet_Field_Entry field;
	char		*RealName;
	char		*UserName;
};

struct INet_String
{
	INet_Field_Entry field;
	char		*string;	
};


#define INET_FIELD_TYPE_USER_ADDRESS						1
#define INET_FIELD_TYPE_STRING								2

#define INET_FIELD_ID_X_ENVELOPE_FROM						1
#define INET_FIELD_ID_X_ENVELOPE_TO							2
#define INET_FIELD_ID_FROM										3
#define INET_FIELD_ID_TO										4
#define INET_FIELD_ID_SENDER									5
#define INET_FIELD_ID_REPLY_TO								6
#define INET_FIELD_ID_CC										7
#define INET_FIELD_ID_BCC										8
#define INET_FIELD_ID_RETURN_RECEIPT_TO					9
#define INET_FIELD_ID_DISPOSITION_NOTIFICATION_TO		10
#define INET_FIELD_ID_MESSAGE_ID								11
#define INET_FIELD_ID_ORGANIZATION							12
#define INET_FIELD_ID_DATE										13
#define INET_FIELD_ID_X_MAILER								14
#define INET_FIELD_ID_SUBJECT									15

struct INet_EMail_Entry
{
	struct ItemInfoBlock iib;		// 512
	Date	SendDate;					// 524  import date in database 
	Date	ExpirationDate;			// 536  auto-killing on ... 
	UWORD	ExpirationDays;			// 538	auto-killing in .. days 
	UBYTE	Importance;					// 539  priority of mail
	UBYTE res;							// 540
	ULONG replys;						// 544
	ULONG	next_replyIP64;			// 548 
	ULONG next_replyIP;				// 552
	ULONG	textsize;					// 556
	ULONG headersize;					// 560
	ULONG	seek;							// 564 Startseek in [Userpath]/MailData 
	ULONG	xxxxxx;						// 568

	ULONG FieldEntrys;				// 572
	ULONG FirstField64;				// 576
	ULONG FirstField;					// 580
	ULONG FieldSize;					// 584
	UBYTE resdf[16];					// 600
};

struct INet_EMail
{
	struct ItemInfoBlock iib;	// 512
	Date	SendDate;				// 524  import date in database 
	Date	ExpirationDate;		// 536  auto-killing on ... 
	UWORD	ExpirationDays;		// 538	auto-killing in .. days 
	UBYTE	Importance;				// 539  priority of mail
	UBYTE res;						// 540
	ULONG replys;					// 544
	ULONG	next_replyIP64;		// 548 
	ULONG next_replyIP;			// 552
	ULONG	textsize;				// 556
	ULONG headersize;				// 560
	ULONG	seek;						// 564 Startseek in [Userpath]/MailData 
	ULONG	xxxxxx;					// 568

	MinList fieldlist;			// 580
	char  *text;					// 584
	char  *header;					// 588
};



#endif