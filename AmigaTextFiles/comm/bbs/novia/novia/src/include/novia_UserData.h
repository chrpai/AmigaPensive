#ifndef NOVIA_USERDATA_H
#define NOVIA_USERDATA_H
#define NOVIA_USERDATE_H_VERSION "$VER: 0.05 (02.08.1999)"

// V0.01 (08.05.1998) (c) Copyright 1996-1998 by Thorsten Gehler
// V0.02 (18.08.1998) (c) Copyright 1998 by Thorsten Gehler
// V0.03 (19.08.1998) (c) Copyright 1998 by Thorsten Gehler
// V0.04 (17.10.1998) (c) Copyright 1998 by Thorsten Gehler
// V0.05 (02.08.1999) (c) Copyright 1998 by Thorsten Gehler
// All rights reserved. Read license.readme for copyright informations.

#ifndef NOVIA_DATE_H
#include <novia/novia_date.h>
#endif

#ifndef NOVIA_TYPES_H
#include <novia/novia_types.h>
#endif

#ifndef NOVIA_USERLIST_H
#include <novia/novia_userlist.h>
#endif

struct UserData {			
	ULONG	IDNumber;				// 4		unique (serial) ID #
	ULONG	IPNumber;				// 8		real account number
	char	Handle[21];				// 29
	UBYTE	Status;					// 30		account in use/free/freeze...
	char	PassWord[21];			// 51
	char	PassWordEncrypt[21];	// 72
	ULONG	Access;					// 76
	ULONG	AccessGroup;			// 80
	char	RealName[41];			// 121
	char	Street[41];				// 162
	UWORD	Streetno;				// 164		!!! NOT IN USE SIENCE V0.02 
	char	City[31];				// 195
	char	ZipCode[11];			// 206

	UWORD	CountryCode;			// 208		!!! NOT IN USE SIENCE V0.04 
	char	Country[4];				// 212		GER,USA,FRA...

	char	AreaCode[8];			// 216		001,0049,0033....
	char 	Area[17];				// 237		!!! NOT IN USE SIENCE V0.04 
	char	PhoneNo[25];			// 262		Euro 8-7 format POSSIBLE!!
	char 	ModemNo[25];			// 287
	char	FaxNo[25];				// 312
	ULONG	ComputerType;			// 316
	struct	Date Birthday;			// 328
	struct	Date FirstCall;			// 340
	struct	Date LastCall;			// 352
	struct  Date Logondate;			// 364
	struct  Date reserveddate1;		// 376
	struct  Date reserveddate2;		// 388

	struct	Date ExpireDate;		// 400
	UWORD	ExpireDays;				// 402

	UBYTE	Sex;					// 403
	UBYTE	Language;				// 404

 	char	Banner[21];				// 425

	char	UUCP[31];				// 456
	char	Data_dir[31];			// 487
	
	UBYTE	CharSet;					// 488		Char Set (Unix, Amiga, Mac) or IBM
	UBYTE	MoreMode;				// 489
	UBYTE	TimeFormat;				// 490		12-Hrs, 24Hrs
	UBYTE	DateFormat;				// 491		us, europe
	UBYTE	TimeZone;				// 492
	UBYTE	TermLineFeeds;			// 493
	UBYTE	TermTabs;				// 494
	UBYTE	TermANSI;	
	UBYTE	TermColors;				// 496
	UBYTE	TermWidth;	
	UBYTE	TermLength;				// 498
	UBYTE TermType;	
	UBYTE	HelpLevel;				// 500
	UBYTE	DefProtocol;	
	UBYTE	MailBoxOpen;			// 502
	UBYTE	MailBoxOpenRes1;
	UBYTE	MailBoxOpenRes2;
	UBYTE	MailBoxOpenRes3;
	UBYTE	MailBoxOpenRes4;
	UBYTE	MailBoxOpenRes5;
	UBYTE	ResponsePausing;
	UBYTE YankEOLSequence;
	UBYTE	YankPacker;				// 510
	UBYTE	AutoHide;
	UBYTE	Laguage;
	UBYTE	HelpLevel2;				// 513
	UBYTE	PRealName;				// 514		private Flags
	UBYTE	PAddress;				// 515
	UBYTE	PPhoneno;				// 516
	UBYTE	PModemno;				// 517
	UBYTE PFaxno;					// 518
	UBYTE	PBirthday;				// 519
	UBYTE	PhoneVerification;		// 520
	ULONG	MailBoxForwardTo;		// 524
	ULONG	MailBoxCopyTo;			// 528
	UBYTE	reserverdBYTEs[2];		// 530
	UBYTE	MCILevel1;
	UBYTE	MCILevel2;

	UBYTE	Flag_supervisor;		// 533 
	UBYTE	Flag_send_email;		
	UBYTE	Flag_receive_email;		// 535
	UBYTE	Flag_set_mail_expiration;	
	UBYTE	Flag_add_maillist;		// 537
	UBYTE	Flag_msg_maillist;
	UBYTE	Flag_send_urgant_mail;	// 539
	UBYTE	Flag_reserverd[20];		// 559
	UBYTE	Flag_send_forward_mail;	// 560
	UBYTE	Flag_forward_mail;		// 561
	UBYTE	Flag_forward_emails;
	UBYTE	Flag_mailres2;
	UBYTE	Flag_mailres3;
	UBYTE	Flag_mailres4;			// 565
	UBYTE	Flag_mailres5;
	UBYTE	Flag_mailres6;
	UBYTE	Flag_mailres7;
	UBYTE	Flag_mailres8;
	UBYTE	Flag_use_pfiles;		// 570
	UBYTE	Flag_use_gfiles;		// 571
	UBYTE	Flag_use_userlist;		// 572
	UBYTE	Flag_use_olm;			// 573
	UBYTE	Flag_use_megaolms;		// 574
	UBYTE	Flag_use_chat;			// 575
	UBYTE	Flag_use_multichat;		// 576
	UBYTE	Flag_use_bc_olm;			// 577 broadcast olms
	UBYTE	Flag_use_olm_res2;		// 578
	UBYTE	Flag_use_olm_res3;		// 579
	UBYTE	Flag_bypas_bbsevents;	// 580
	UBYTE	Flag_alias_msg;
	UBYTE	Flag_adopt_orphans;		// 582
	UBYTE	Flag_read_priv_msg;
	UBYTE	Flag_edit_any_files;		// 584
	UBYTE	Flag_edit_own_files;
	UBYTE	Flag_write_anounym;		// 586
	UBYTE	Flag_trace_anounym;
	UBYTE	Flag_private_msg;			// 588
	UBYTE	Flag_irc_control;			// 589
	UBYTE	Flag_un_files;				// 590 unlimeted files
	UBYTE	Flag_un_bytes;				// 591
	UBYTE	Flag_autocallback;		// 592
	UBYTE	Flag_timelock;				// 593
	UBYTE	Flag_new_votes;			// 594
	UBYTE	Flag_new_choices;			// 595
	UBYTE	Flag_edit_votes;			// 596
	UBYTE	Flag_edit_handle;			// 597
	UBYTE	Flag_edit_name;			// 598
	UBYTE	Flag_edit_address;
	UBYTE	Flag_edit_voice;		// 600
	UBYTE	Flag_edit_data;
	UBYTE	Flag_edit_fax;			// 602
	UBYTE	Flag_allow_banner;
	UBYTE	Flag_use_termlink;		// 604
	UBYTE	Flag_monitor_port;
	UBYTE	Flag_alarm_sysop;		// 606
	UBYTE	Flag_open_screen;
	UBYTE	Flag_open_capture;		// 608
	UBYTE	Flag_page_sysop;		// 609
	UBYTE	Flag_skip_fvalid;		// 610
	UBYTE	Flag_relogin;			// 611
	UBYTE	Flag_multilogin;		// 612

	ULONG	UploadsToday;			// 616
	ULONG	UploadsMonths;			// 620
	ULONG	TotalUploads;			// 624

	ULONG	DownloadsToday;			// 628
	ULONG	DownloadsMonths;		// 632
	ULONG	TotalDownloads;			// 636
	
	ULONG	UploadsBytesToday;		// 640
	ULONG	UploadsBytesMonths;		// 644
	ULONG	TotalBytesUploads;		// 648

	ULONG	DownloadsBytesToday;	// 652
	ULONG	DownloadsBytesMonths;	// 656
	ULONG	TotalBytesDownloads;	// 660
	
	ULONG	ByteCredits;			// 664
	ULONG	FileCredits;			// 668
	UWORD	CallsToday;				// 670
	UWORD	CallsMonths;			// 672
	UWORD	CallsPerDay;			// 674   !!! new in V0.05 !!!
	UWORD CallsReserver;			// 676
	ULONG	TotalCalls;				// 680

	ULONG	TimeToday;				// 684
	ULONG	TimeMonths;				// 688
	ULONG	TotalTime;				// 692
	
	ULONG	CostsPerDay;			// 696
	ULONG	CostsPerMinute;			// 700
	ULONG	CostsPerKB;				// 704
	
	ULONG	TotalTelCosts;			// 708
	ULONG	TotalCredits;			// 712
	
	UWORD	PublicMsgsCall;			// 714
	UWORD	PublicMsgsDay;			// 716
	ULONG	PublicMsgsWeek;			// 720
	ULONG	PublicMsgsMonth;		// 724
	ULONG	TotalPosts;				// 728		!!! changed sience V0.02 TotalPublicMsgs > TotalPosts

	UWORD	PrivateMsgsCall;		// 730
	UWORD	PrivateMsgsDay;			// 732
	ULONG	PrivateMsgsWeek;		// 736
	ULONG	PrivateMsgsMonth;		// 740
	ULONG	TotalEMails;			// 744		!!! changed sience V0.02 TotalPublicMsgs > TotalEMails

	UWORD	OLMsCall;				// 746
	UWORD	OLMsDay;				// 748
	ULONG	OLMsWeek;				// 752
	ULONG	OLMsMonth;				// 756
	ULONG	TotalOLMs;				// 760

	ULONG	FileCredits1;			// 764		FileRation
	ULONG	ByteCredits1;			// 768		ByteRation
	ULONG	FileCredits2;			// 772
	ULONG	ByteCredits2;			// 776
	ULONG	FileCredits3;			// 780
	ULONG	ByteCredits3;			// 784
	ULONG	FileCredits4;			// 788
	ULONG	ByteCredits4;			// 792
	ULONG	FileCredits5;			// 796
	ULONG	ByteCredits5;			// 800

	UWORD	MinPerCall;				// 802
	UWORD	MinsIdle;				// 804
	ULONG	MsgPerCall;				// 808
	ULONG	FeedbackPerCall;		// 812
	UWORD	EditorLines;			// 814
	UWORD reserviert;				// 816
	ULONG	MaxEMail;				// 820
	ULONG	SendLogTo;				// 824
	UWORD	InactivityDays;			// 826
	UWORD	LinesPerSignature;		// 828
	UBYTE	DailyPfilesMinutes;		// 829
	char	reservedxy[40];			// 869
	
	char	LogonMacro[50];			// 919
	char	Control_E_macro[20];	// 939
	char	Control_F_macro[20];	// 959
	UBYTE	YankMethod;				// 960
	UBYTE CaseZone;				// 961
	char	res567567[3];			// 964

	ULONG	DailyDownBytes;			// 968
	ULONG	DailyUpBytes;			// 972

	ULONG	LogToMail;				// 976

	ULONG	NewMails;				// 980		New Mails his Mailbox.    !!! new sience V0.03
	ULONG	EMails;					// 984		Total Mails in Mailbox	 !!! bew sience V0.03
	char	uucp_forward_adr[64];// 1048		forwarding email-address for emails
	BYTE 	res[104];				// 1152		END OF STRUCT
};
#endif
