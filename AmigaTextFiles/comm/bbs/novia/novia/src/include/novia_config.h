#ifndef NOVIA_CONFIG_H
#define NOVIA_CONFIG_H
#define NOVIA_CONFIG_H_VERSION "$VER: 0.08 (06.12.1998)"
// (c) Copyright 1996-1998 by Thorsten Gehler
// All rights reserved. Read license.readme for copyright informations.

// V0.06 (10.06.1998) (c) Copyright 1996-1998 by Thorsten Gehler
// V0.07 (10.10.1998) (c) Copyright 1998 by Thorsten Gehler
// V0.08 (06.12.1998) (c) Copyright 1998 by Thorsten Gehler


#ifndef NOVIA_PORTDATA_H
#include <novia/novia_portdata.h>
#endif

#ifndef NOVIA_DOS_H
#include <novia/novia_dos.h>
#endif

#define SERVER_INVALID	0
#define SERVER_ADDED		1
#define SERVER_RUN		2
#define SERVER_READY		3
#define SERVER_WAIT		4
#define SERVER_EXCEPT	5
#define SERVER_REMOVED	6

#define USER_INVALID		0
#define USER_ADDED		1
#define USER_RUN			2
#define USER_READY		3
#define USER_WAIT			4
#define USER_EXCEPT		5
#define USER_REMOVED		6

struct INet_Config					// New V0.07
{
	ULONG	MaxRemoteNodes;			// 4
	ULONG	RemoteIPAddress;		// 8  Default Remote Admin IP-Address
	UWORD	RemotePort;				// 10 Default Port
	char	LocalHost[80];			// 90 Default Local Hostname/Address
	char	RemoteAdmin;			// 91 Allow remote administration
	char	Summertime;				// 92
};

struct SystemDirs {
	char	sysdir[80];				// 80
	char	userdir[80];			// 160
	char	sysdata[80];			// 240
	char	systext[80];			// 320
	char	newuser[80];			// 400
};

struct MainPortConfig
{
	char	SystemName[81];			// 81
	char	RegID[20];					// 101
	char	RegName[21];				// 122
	char	RegLocation[81];			// 203
	char	RegVoicenumber[41];		// 244
	char	RegModem[41];				// 285
	char	RegEMail[41];				// 326
	char	MainPortNumber[25];		// 351
	char	UUCPDomain[81];			// 432

	ULONG	SysopAccountNo;			// 436
	ULONG	UserIPCounter;				// 440 Serial IP Counter 
	ULONG	BaseIPCounter;				// 444
	ULONG	UserAccounts;				// 448 Number of Useracounts 

	ULONG	Numberofclients;			// 452

	char	DefaultAreaCode[4];		// 456
	char	DefaultLocationNo[10];	// 466
	char	DefaultCountryNo[5];		// 471
	char	ReservedDefault[181];	// 652
	ULONG EMailIPCounter64;			// 656
	ULONG EMailIPCounter;			// 660
	ULONG	MessageIPCounter64;		// 664
	ULONG	MessageIPCounter;			// 668

	char	UDBaseClosed;				// 669
	char	MsgBaseClosed;				// 670
	char	reservedClosed[18];		// 688
	ULONG	GlobalBalance;				// 692
	ULONG	ScreenMode;					// 696
	BYTE	CH_Main;						// 697
	BYTE	LoadGUI;						// 698
	BYTE	resBOOL[2];					// 700
	ULONG	Colors;						// 704 Sorry this is the screendepth not the colors !!! if Colors=8, Screen have 256 Colors
	char	FontName[32];				// 736 Screen and window Titles 
	ULONG 	Fontsize;				// 740 
	char	TermFontName[32];			// 772 Console Font
	ULONG	TermFontsize;				// 776
	struct	SystemDirs sysdirs;	// 1176
	ULONG	numberofmci;				// 1180 Number of mci's
	struct	mcicode *mcicmd;		// 1184 MCI Table
	struct	MinList	ClientList;	// 1196
	ULONG	rexxtempcounter;			// 1200
	UBYTE	guiloaded;					// 1201
	UBYTE	resflagsgsgs;				// 1202
	UWORD	nrc_ip_counter;			// 1204
	MinList	LocalMailServerList;	// 1216
	MinList POP3ServerList;			// 1228
	INet_Config INetConfig;			// 1320
	MinList	EventList;				// 1332
	Directory *rootdir;				// 1336
	char	ReservedOther[152];		// 1488
};


#define INET_MAX_USER_LENGTH		64
#define INET_MAX_DOMAIN_LENGTH	64


struct INet_LocalMailServer							// New V0.07
{
	struct	INet_LocalMailServer *ln_Succ;		// 4		Next Server
	struct	INet_LocalMailServer *ln_Pred;		// 8		Pred Server
	struct	INet_LocalMailServer *backup_server;// 12		Pointer to MainPortServer New V0.08
	ULONG	IDNumber;										// 16		IDNumber
	ULONG	IPNumber;										// 20		Serial IP-Number
	char	Name[128];										// 60		ServerName
	char	Domain[80];										// 140	Domain
	char	SMTPHost[80];									// 220	Simple Mail Transfer Protocol, Hostaddress
	UWORD	SMTPPort;										// 222	SMTP-Port (Default 25)
	char	POP3Host[80];									// 302	Post of Office Protocol, Hostaddress
	UWORD	POP3Port;										// 304	POP3-Port (Default 110, POP2 = 109)
	char	Status;											// 305	Status of Server, new in V0.08
	char	Flag_Allow8Bit;								// 306	8-Bit Server
	char	Flag_UseAPOP;									// 307	Allow APOP-Login
	char	Flag_ExternalServer;							// 308	Don't Use Novia TCP/IP-Support
	char	InvalidMailSubject[80];						// 388
	char	InvalidAddress[80];							// 468	Invalid From:/ReplyTo: Address
	char	InvalidFilename[120];						// 588	Invalid Mail filename
	char	InboundPath[80];								// 668	
	char	OutboundPath[80];								// 748	
	MinList	Users;										// 760	UserList
};

struct INet_POP3Server									// New V0.07
{
	struct	INet_POP3Server *ln_Succ;				// 4
	struct	INet_POP3Server *ln_Pred;				// 8
	struct	INet_POP3Server *backup_server;		// 12		New V0.08
	ULONG	IDNumber;										// 16
	ULONG	IPNumber;										// 20
	char	Status;											// 21
	char	Name[40];										// 61		ServerName
	char	Domain[120];									// 181
	char	SMTPHost[80];									// 261	Simple Mail Transfer Protocol, Hostaddress
	char	POP3Host[80];									// 341	Post of Office Protocol, Hostaddress
	char	Username[80];									// 421	POP3 ID
	char	Password[80];									// 501	POP3 Password
	char	Flag_8Bit;										// 502	Server use 8-Bit
	char	Flag_UseAPOP;									// 503	APOP-Login
	char	Flag_ExternalServer;							// 504	Don't Use Novia TCP/IP-Support
	char	Flag_DeleteMails;								// 505	Delete Mails after Recieve
	char	InvalidFilename[120];						//	625	
	char	InvalidMailSubject[80];						// 705	
	char	InvalidAddress[80];							// 785	Invalid From:/ReplyTo: Address
	char	InboundPath[80];								// 865	
	char	OutboundPath[80];								// 945
	char	reserved;										// 946
};

#define INET_USERTYPE_BBS			0
#define INET_USERTYPE_EXTERNAL		1

struct INet_MailUser										// New V0.07
{
	struct	INet_MailUser *ln_Succ;					// 4
	struct	INet_MailUser *ln_Pred;					// 8
	struct	INet_MailUser *backup_user;			// 12		pointer to original Userdata
	ULONG	IDNumber;										// 16
	ULONG	IPNumber;										// 20
	char	UserType;										// 21		Type of User 0=BBS User, 1=External User (no BBS-Support)
	char	Name[40];										// 61
	char	POPID[40];										// 101	POP3 Username
	char	POPPassword[40];								// 141	Password
	char	Flag_UseBBSUserName;							//	142
	char	Flag_UseBBSPassword;							// 143
	char	Status;											// 144	New in V0.08
	UWORD	HoldMails;										// 146	HoldMails ... days
	UWORD	MaxEMails;										// 148	Maximum holding EMails in Mailbox
	ULONG	Space;											// 152	Harddisk-Space in MB
};

struct INet_RoutingUser									// New V0.07
{
	struct	INet_RoutingUser	*ln_Succ;			// 4
	struct	INet_RoutingUser	*ln_Pred;			// 8
	struct	INet_RoutingUser	*backup_user;		// 12
	ULONG		IDNumber;									// 16
	ULONG		IPNumber;									// 20
	char		Name[40];									// 60
	MinList	IncommingServerList;						// 72
	char		Status;										// 73		New in V0.08
	char		SearchNameType;							// 74		0 - Search in Username, 1 - Search in RealName
	char		SearchZeile;								// 75		0 - To:  1 - From:  2 - ReplyTo: 
	char		Flag_reserved;								//	76		
	ULONG		MoveToIPNumber;							// 80		To-Server Serial Number
	char		From[80];									//	160
	char		ReplyTo[80];								//	240
};

struct INet_Services										// New V0.07
{
	INet_Config		*config;
	MinList			LocalMailServerList;
	MinList			ExternalMailServerList;
};

struct INet_MailHeader
{
	INet_MailHeader	*ln_Succ;
	INet_MailHeader	*ln_Pred;
	ULONG					id64;
	ULONG					id;
	ULONG					ip64;
	ULONG					ip;
	ULONG					size;
	BPTR					fh;
	char					uid[80];
	char					*MessageID[80];
	char					filename[32];
};

struct INet_Mail
{
	INet_MailHeader	*ln_Succ;
	INet_MailHeader	*ln_Pred;
	char					*Username;
	char					*passwd;
	char					*from;
	MinList				to_list;
	char					*text;
	
};

#endif

