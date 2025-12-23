/*
 * EXCELSIOR! AmigaDOS C flag bits include file.
 * 
 * EXCELSIOR! BBS © 1992 Sycom Design Software, All Rights Reserved 
 *                       Ronald Kushner, Thomas Dietz
 *
 *
 * This file is covered by the EXCELSIOR! program license agreement. 
 * Unauthorized distribution is prohibited.
 * 
 $Header: Work:excelsior/h/ex_flags.h,v 1.22 1992/08/24 20:01:05 rkushner Exp $
 *
 $Log: ex_flags.h,v $
 * Revision 1.22  1992/08/24  20:01:05  rkushner
 * *** empty log message ***
 *
 *
 * Revision 1.16  1992/07/28  10:43:05  tdietz
 * Cleaned up flags, organized and rewrote  
 *
 * Revision 1.15  1992/07/11  12:24:41  rkushner
 * Revision 1.14  1992/07/09  23:42:06  rkushner
 * Revision 1.13  1992/07/03  14:25:48  rkushner
 *
 */


#define CONSOLE		0
#define SERIAL		1
#define PRINTER		2


/*
**	GetLine Flags 
*/

#define GL_BOX 				(1<<0)  /* Draws An ANSI Box (length of max input) */
#define GL_IN				(1<<1)  /* Passing Something Into Input, goes in ID->IR_Buffer */
#define GL_FLUSH    		(1<<2)  /* Flush the Buffers */
#define GL_UPPER    		(1<<3)  /* Upper Case Only */
#define GL_CAP				(1<<4)	/* Captialize First Letter Only? */
#define GL_HIDE				(1<<5)	/* Print 'X', good for Passwords */
#define GL_NUM				(1<<6)	/* Numbers and  -,(,) */
#define GL_ALPHA        	(1<<7)	/* Letters Only */
#define GL_SYM          	(1<<8)	/* NO SYMBOLS! */
#define GL_DOS          	(1<<9)	/* Anything Can Be Entered */
#define GL_EDITOR       	(1<<10)	/* We're in editor mode */
#define GL_DIGIT			(1<<11)	/* Digits Only (0-9) */
#define GL_NOANSI       	(1<<12)	/* for times we wish to ignore ANSI movements */
#define GL_NOIUMS       	(1<<13)	/* No IUMs will be delivered */
#define GL_WRAP				(1<<14)	/* Word Wrap */
#define GL_CHAT				(1<<15)	/* Chat mode */
#define GL_NOCMDSTACK		(1<<16)	/* Turns off Command Stacking */
#define GL_RETURN_DEL		(1<<17) /* Return if bspace and col 0 (for editor) */ 
#define GL_CONFERENCE		(1<<18) /* We are in a conference, report accordingly */
#define GL_NOHISTORY		(1<<19) /* No history buffer */
#define GL_SPECIAL			(1<<20) /* no ipl, #, ?, ~ */
#define GL_COMMODE			(1<<21)	/* COM: mode */
#define GL_HANDLE			(1<<22)	/* Enter a handle only */

/*
**	IO Progress/Process Flags 
*/

#define	IO_ASREAD		(1<<0)	/* ASync serial IO read pending */
#define IO_ASWRITE		(1<<1)	/* ASync serial IO write pending */
#define IO_ACREAD		(1<<2)  /* ASync console IO read pending */
#define IO_ACWRITE		(1<<3)  /* ASync console IO read pending */
#define IO_SERACT		(1<<4)	/* Serial device open */
#define IO_BAR			(1<<5)	/* Stat bar on */
#define IO_CARRIER		(1<<6)	/* Set when carrier is detect is true */
#define IO_ANSI			(1<<7)	/* Is the user in ANSI mode? */
#define IO_SHUTDOWN		(1<<8)	/* We're being told to SHUTDOWN NOW!!! */
#define IO_XPRABORT		(1<<9)	/* Last transfer aborted */
#define IO_BEEP			(1<<10) /* Beep for sysop */
#define	IO_NOTIFY		(1<<11)	/* Notify SysOp after transfer */
#define IO_CONNECTED    (1<<12) /* Someone is connected */


/*
**		 BITS FOR EACH NODE 
*/

#define NODE_INVISIBLE  	(1<<0)	/* Is the node invisible to others */
#define NODE_MONITOR		(1<<1)	/* Is the node monitoring log on/offs */
#define NODE_WATCH			(1<<2)	/* Is the node watching another node */
#define NODE_MUTE			(1<<3)	/* Is the node 'muted' from others */
#define NODE_SHUTDOWN   	(1<<4)	/* Shutdown node upon user logoff */
#define NODE_GLNOCHAR   	(1<<5)	/* Force an IUM upon the user */
#define NODE_IUMREADY		(1<<6)	/* An IUM is waiting, fall out */
#define NODE_LOCALMODE  	(1<<7)	/* We are in local mode, no SERIAL I/O */
#define NODE_DUMPUSER   	(1<<8)	/* Dump the user off the system - IMMEDIATE! */
#define NODE_CHATMODE   	(1<<9)	/* We are in SysOp Chat */
#define NODE_INTERLACE  	(1<<10)	/* Are we in interlace? */
#define NODE_2COLOR     	(1<<11)	/* We are in 2 color */
#define NODE_4COLOR     	(1<<12)	/* We are in 4 color */
#define NODE_8COLOR     	(1<<13)	/* We are in 8 color */
#define NODE_16COLOR    	(1<<14)	/* We are in 16 color */
#define NODE_WORKBENCH  	(1<<15)	/* We are on the workbench screen */
#define NODE_PRINTER    	(1<<16)	/* Printer is active */
#define NODE_MESS_READ  	(1<<17)	/* Reading a message, make ANSI window */
#define NODE_TRANSFER   	(1<<18)	/* Someone is transfering right now */
#define NODE_NOIUMS     	(1<<19) /* Dont send an IUM, WAIT! */
#define NODE_TIMERET    	(1<<20)	/* Return from a prompt if Timer 3 went off */
#define NODE_ISFEEDBACK 	(1<<21) /* This is a feedback message */
#define NODE_NOIPL      	(1<<22) /* No IPL will come across until a prompt of EOF */
#define NODE_NOSYSOP    	(1<<23) /* No Sysop Commands */
#define NODE_NEWUSER    	(1<<24) /* This is a new user letter */
#define NODE_IBMCOLOR   	(1<<25) /* Uses IBM color Palette */
#define NODE_RELOGON    	(1<<26) /* User is relogging on */
#define NODE_CONSOLE		(1<<27)	/* This is a console session */
#define NODE_SPLIT      	(1<<28)	/* Split screen */
#define NODE_USERBUFFER 	(1<<29) /* Auto-Buffer to disk is active! */
#define NODE_CON_BUFFER 	(1<<30) /* Console buffer is on! */
#define NODE_TEMPSYSOP  	(1<<31) /* A temporary sysop until log off or cancellation */

#define MNODE_NORMALOFF 	(1<<0)  /* The user manually logged off */
#define MNODE_SUSPENDED 	(1<<1)  /* The Time has been suspended (ie transfers) */
#define MNODE_SYSOPPAGE 	(1<<2)  /* Someone has paged the sysop */
#define MNODE_CKABORT		(1<<3)	/* Aborted Transfer */
#define MNODE_NOIUC			(1<<4)	/* We are at a point where we can not accept */
									/* IUC requests or grab chats */
#define MNODE_IUC			(1<<5)	/* We are now in a Inter-User Chat */
									/* This flag MAY be set when a user IS NOT */
									/* in chat! Internal use only!!    */
#define MNODE_NOWRAPOUT		(1<<6)	/* No Word Wrap on the output */
#define MNODE_NOMORE    	(1<<7)  /* No more prompt */
#define MNODE_NOABORT    	(1<<8)  /* Cannot abort message */
#define MNODE_ISXMODEM   	(1<<9)	/* ?? */
#define MNODE_STARTEDIUC	(1<<9)	/* We're the port that started the IUC */
#define MNODE_OFFQUICK	 	(1<<10)	/* Logoff Quick Requested by User */
#define MNODE_EDITOR	 	(1<<11)	/* We're currently in the editor */
#define MNODE_DOQUICK	 	(1<<12)	/* We're in quick mode */
#define MNODE_ISDOWNLOAD 	(1<<13)	/* Current Transfer is a DOWNLOAD */
#define MNODE_QUOTEDOALL 	(1<<14)	/* Quoting - Quote ALL! */
#define MNODE_INT_EDITOR 	(1<<15)	/* We are currently in the editor */
#define MNODE_POSTING	 	(1<<16)	/* We are posting */
#define MNODE_RESPONDING 	(1<<17) /* We are responding */
#define MNODE_UUCP_POST  	(1<<18) /* We are posting public uucp */
#define MNODE_UUCP_MAIL  	(1<<19) /* We are posting private uucp */
#define MNODE_CONFERENCE 	(1<<20) /* We are in the conference */
#define MNODE_CONF_READY 	(1<<21) /* Ok to print conference messages */
#define MNODE_SPECIAL_ED 	(1<<22) /* We are in a special ed env */
#define MNODE_NO_PARMARK  	(1<<23) /* Do NOT show markers for CR/LF's */
#define MNODE_WARN_LOWTIME	(1<<24)	/* Warn User of low time */
#define MNODE_WARNING_SET	(1<<25)	/* User has been warned! */
#define MNODE_ANSWERMODE	(1<<26)	/* AutoAnswer Mode */ 
#define MNODE_ODU_REQ		(1<<27)	/* Another process wants the serial.device */
#define MNODE_AUTOOFF		(1<<28) /* Log the user off after transfer */
#define MNODE_OPENAFTERT	(1<<29) /* Open Screen After Transfer */
#define MNODE_RETURNMOUSE	(1<<30) /* Return mouse buttons every mouse click */
#define MNODE_MOUSECLICK	(1<<31) /* A mouse click, return from GetCH() */


/* 
**	Extended Bits
*/
#define	EXT_CONSOLE_CHAT	(1<<0)	/* We are in a console chat */
#define EXT_LEFT_CON_CHAT	(1<<1)	/* We left console chat with close gad */
#define EXT_IBMCOLOR		(1<<2)	/* IBM Color Emulation mode */
#define EXT_NEWUSER_MAIL	(1<<3)	/* We are reading new user app's */
#define EXT_UUCP_RESPONDING (1<<4)	/* We are responding to UUCP messages */
#define EXT_COLORIFIC		(1<<5)  /* We are in colorific mode */
#define EXT_DELAY_CHAR		(1<<6)  /* We are in delay char mode */
#define EXT_EXIT_CHAT		(1<<7)  /* We just left chat mode */
#define EXT_FILE_MAIL		(1<<8)	/* We are sending file mail */
#define EXT_RAW_MODE		(1<<9)	/* We are in RAW: mode for AUX: */
#define EXT_RESUME_MODE		(1<<10) /* Upload resume mode */
#define EXT_COM_ACTIVE		(1<<11)	/* We are in COM mode */
#define EXT_GETLINE			(1<<12) /* We are in GetLine */
#define EXT_ASYNCREAD		(1<<13) /* Async ECOM: Read */
#define EXT_CALLBACK		(1<<14) /* We are in callback mode */
#define EXT_WATCHMODE		(1<<15) /* Watching a node */
#define EXT_BEINGWATCHED	(1<<16) /* We are being watched */
#define EXT_STOPTHEWATCH	(1<<17) /* In one way or another, this node is in use */
#define EXT_PROCMD          (1<<18) /* We want to process a command */
#define EXT_PRINTWATCH      (1<<19) /* We want to process a command */
#define EXT_DOSDOOR_OUTPUT  (1<<20) /* dos door output only, NO IPL! */
#define EXT_CURSOR_ON		(1<<21) /* The cursor is on! */
#define EXT_SCANALL			(1<<22) /* Scan all mode */
#define EXT_NOTIFYARCS		(1<<23) /* Notify user they cannot upload archive */
#define EXT_NOCHATMODE		(1<<24) /* No chat mode right now */
#define EXT_NOTITLEBAR		(1<<25) /* No title bar mode */
#define EXT_BATCH_QWK		(1<<26) /* Batch QWK files */
#define EXT_QWK_UPLOAD		(1<<27) /* Uploading QWK files */
#define EXT_IMPORT_QWK		(1<<28) /* Currently Importing QWK Files */
#define EXT_PARAGON_DOOR	(1<<29) /* A paragon door is active */
#define EXT_NO_IUMS			(1<<30) /* No IUMs for the time being */
#define EXT_MOUSEHIT		(1<<31) /* Mouse Click was hit */


/* 
**	Exp Bits
*/
#define EXP_FORCE_EXIT_IUC	(1<<0)	/* Force User out of IUC */
#define EXP_FORCE_EXIT_CB	(1<<1)  /* Force user out of CB */
#define EXP_WARN_CBIUC		(1<<2)	/* Warn user of low time in CB/IUC */
#define EXP_USED_TEMPLATE   (1<<3)  /* Template has been used */
#define EXP_LOGOFF_AFTER	(1<<4)	/* User is to log off after transfer */

/* 
**	System Bits
*/

#define SYS_NONEWUSERS 		(1<<0)	/* No new users */
#define SYS_NOFBASE    		(1<<1)	/* FBase is Closed */
#define SYS_NOMBASE    		(1<<2)	/* MBase is Closed */
#define SYS_NOPBASE    		(1<<3)	/* PBase is Closed */
#define SYS_NOTBASE 		(1<<4)	/* TBase is Closed */
#define SYS_NOFEEDS			(1<<5)	/* No UUCP Feeds */
#define SYS_SYSOPIN	 		(1<<6)	/* The sysop is in */


/*
**	PrintFile Flags 
*/

#define ERR_FNF  			(1<<0)	/* Prints A %s Not Found for Filenames */
#define NOABORT  			(1<<1)	/* User Can't Abort The File Display */
#define NO_IPL				(1<<2)	/* IPL Commands will be displayed */
#define SCR_CLR				(1<<3)  /* Clear screen before displaying */
#define NO_TT				(1<<4)  /* No term type processing */


/*
**		USER_INFO_BITS  (userinfo.User_Info_Bits)
*/

#define EI_MALE        	 	(1<<0)	/* Extended info, if set user is male */
#define EI_ANSI         	(1<<1)	/* Do we use ANSI? */
#define EI_EXPERT       	(1<<2)	/* Expert Help Level Flag */
#define EI_PRIVATE      	(1<<4)	/* Private Info? */
#define EI_MORE         	(1<<5)	/* More prompts active? */
#define EI_LINEFEEDS    	(1<<6)	/* Do we need linefeeds? */
#define EI_INT_TIME     	(1<<7)	/* International 24 hour time */
#define EI_LOCKOUT      	(1<<8)	/* Is user locked out of the system? */
#define EI_ACTIVE			(1<<9)	/* Is This Account Active? */
#define EI_ANSIED			(1<<10)	/* Use ANSI Full Screen Editor by default */
#define EI_ANSISCROLL		(1<<11)	/* Scroll messages (ANSI WINDOW) */
#define EI_CALLBACK			(1<<12) /* This user has been called and tested */
#define EI_NORMAL			(1<<13) /* Intermediate Help Level */
#define EI_NOVICE			(1<<14)	/* Nove help level */
#define EI_BATCH_CR			(1<<15) /* Add CR to batch files */
#define EI_RESPOND_PAUSE	(1<<16) /* Pause between responses */
#define EI_AUTO_SIG			(1<<17) /* If set, wont ask to add sig, else query */ 
#define EI_MAILCLOSED     	(1<<18)	/* Mail Box Open? */



/*
**		Extended Access Flags (userinfo.E_Access.AccBits)
*/

#define EA_SYSOP        	(1<<0)	/* SysOp Privileges? */
#define EA_RELOGON      	(1<<1)	/* Can User Re-Logon? */
#define EA_NOCHARGE     	(1<<2)	/* No charges for this account */
#define EA_MAIL_FILE    	(1<<3)	/* Can User Send File Mail */
#define EA_MAIL_ALIAS   	(1<<4)	/* Can User Post As An Alias */
#define EA_MAIL_FORWARD 	(1<<5)	/* Can User Forward Mail */
#define EA_MAIL_CARBON  	(1<<6)	/* Can user send Carbon Copy letters */
#define EA_SHOW_ANON    	(1<<7)	/* Can User See Real Annoymous'*/
#define EA_DEL_FILES    	(1<<8)	/* Can User Delete Any File */
#define EA_DEL_OWN      	(1<<9)	/* Can User Delete OWN Files */
#define EA_VALIDATE     	(1<<10)	/* Auto-Validate UL's */
#define EA_UNRESTR_FILE 	(1<<11) /* Unrestricted File Credits */
#define EA_UNRESTR_BYTE 	(1<<12)	/* Unrestricted Byte Credits */
#define EA_HAVE_ALIASES		(1<<13) /* Can user have aliases */
#define EA_CONF_CTRL		(1<<14)	/* Can User Control Conferences */
#define EA_BROADCAST    	(1<<15)	/* Can User Send Public Broadcasts */
#define EA_RECEIVE_BROD 	(1<<16)	/* Can User Receive Broadcast */
#define EA_CHANGE_ALIAS 	(1<<17)	/* Can User Change Alias */
#define EA_ENTER_UUCP		(1<<18)	/* Enter A UUCP BASE */
#define EA_SEND_UUCP		(1<<19)	/* Send UUCP Public Posts */
#define EA_ENTER_FIDO		(1<<20)	/* Enter A FIDO BASE */
#define EA_SEND_FIDO		(1<<21)	/* Send FIDO Public Posts */
#define EA_WATCH			(1<<22)	/* Can user "watch" others */
#define EA_OPENSCREEN		(1<<23)	/* Open Screen Upon Logon */										 	
#define EA_ANONYMOUS    	(1<<24)	/* Can User Post Annonymously */
#define EA_PRIVATEAREA  	(1<<25)	/* Can have a private area */
#define EA_BUFFER_OUT   	(1<<26)	/* Buffer Entire output! */
#define EA_DOS_DOWNLOAD 	(1<<27)	/* User can enter direct path and filenames	for downloading */
#define NOT_IN_USE_USEME
#define NOT_IN_USE_USEME_TOO
#define EA_FULLWATCH		(1<<30) /* User can type while in "WATCH" */
#define EA_BYPASS_BAUD  	(1<<31) /* User can bypass min bauds */



/* 
**	More Access Bits  (userinfo.E_Access.More_AccBits) 
*/

#define MA_NO_WHONOTES		(1<<0)	/* User cannot have any Who Notes */
#define	MA_AUTO_INVIS		(1<<1)	/* Auto Invisible */
#define MA_UUCP_FEED		(1<<2)  /* This is not a user, for uucp feeds */
#define MA_NO_CHARGES		(1<<3)	/* This user is exempt from charges */
#define MA_DIALOUT			(1<<4)	/* Able to Dial Out */
#define MA_AUTO_MUTE		(1<<5)	/* Auto mute upon login */
#define MA_AUTO_WHO			(1<<6)	/* Auto show who upon login */
#define MA_LINK_ACCOUNT		(1<<7)	/* Link account */
#define MA_AUTO_MONITOR		(1<<8)  /* Monitor at login */
#define MA_GLOBAL_SUBOP		(1<<9)	/* This user is a global sub-op */
#define MA_BATCH_UUCP		(1<<10)	/* User can batch UUCP messages */
#define MA_BATCH_FIDO		(1<<11) /* User can batch FIDO messages */
#define MA_CRASHMAIL		(1<<12) /* User can send Crash mail */
#define MA_FILEREQUEST      (1<<13) /* File Request files */
#define MA_MAIL_QUEUE		(1<<14)	/* Send Mail Distribution Queue letters */
#define MA_AUTO_BATCH		(1<<15) /* User can set an auto-batch time */
#define MA_DEFINE_UUCPADDR	(1<<16)	/* User can edit/create their UUCP net address */


/* 
**	MAIL_Flag Bits 
*/

#define MAIL_RECEIPT    	(1<<0)	/* Send A Receipt if No Reply or Forward-Kept */
#define MAIL_ORIGINAL   	(1<<1)	/* Return Original Message? */
#define MAIL_FILE			(1<<2)	/* Is this a file message? */
#define MAIL_DELETE			(1<<3)	/* If Its A File, Delete After */
#define MAIL_URGENT			(1<<4)	/* Is it Urgent Mail? */
#define MAIL_FORWARDED		(1<<5)	/* This is an anonymous message */
#define MAIL_CARBONCOPY		(1<<6)	/* Part of a Carbon Copy */
#define	MAIL_READ			(1<<7)	/* The letter has been read */
#define	MAIL_KILL			(1<<8)	/* The letter has been MARKED FOR DEATH */
#define MAIL_UUCP_MAIL  	(1<<9)	/* This is a UUCP Netmail message */
#define MAIL_FIDO			(1<<10) /* This is a FIDO Netmail message */
#define MAIL_REPLYSENT		(1<<11) /* Reply has been sent */



/* 
**	Config Bits 
*/

#define SYSTEM_PRIVATE  	(1<<0)	/* Is this a Private BBS? If so, prompt for passwd */
#define FBACK_AT_LOGIN  	(1<<1)	/* After x attempts to login, ask to leave feedback?*/
#define OPT_USER_LETTER 	(1<<2)	/* Let users choose to leave new user letter */
#define MAN_USER_LETTER 	(1<<3)	/* Manual new user letter */
#define LOGIN_WHO       	(1<<4)	/* User can type Who at login */
#define FILTER_ANSI     	(1<<5)	/* Filter out ansi for captures */
#define EVERYTHING_FREE		(1<<6)	/* Everything is free */
#define	NO_FILEBASE			(1<<7)	/* File Base is closed */
#define NO_MESSBASE			(1<<8)	/* Message Base is closed */
#define LOG_TRANSFERS   	(1<<9)	/* Record Transfers */
#define SMART_PATHS       	(1<<10)	/* Use dir nest */
#define SYSOP_AVAILABLE    	(1<<11)	/* Sysop Is available for chatting */
#define SHOWBAR         	(1<<12)	/* Status Bar ON upon login */
#define SHOWSTAT        	(1<<13)	/* Stats Window ON upon login */
#define OPENSCREEN_ALL  	(1<<14)	/* Open Screen on ALL Logins */
#define FBACK_MAILONE   	(1<<15)	/* Feedback goes to ID #1 (sysop) */
#define CHECKARC_UPLOAD		(1<<16) /* Check arc's after upload */ 
#define CHECKARC_MAINT		(1<<17) /* Check arc's at maint */
#define CALLBACK_ENABLE		(1<<18)	/* Auto Callback Enabled */
#define CALLBACK_TOLLSAVER	(1<<19) /* TollSaver Callback enabled */
#define	CONARC_UPLOAD		(1<<20) /* Convert Archives after upload */
#define CONARC_MAINT		(1<<21) /* Convert Archives at maint */
#define IMPORT_NETMAIL		(1<<22)	/* Import netmail */
#define RECEIVE_POLLS		(1<<23) /* Receive polls */
#define USE_OWNDEVUNIT		(1<<24) /* Use owndevunit */
#define USE_LOCALED			(1<<25) /* Use local editor */
#define NO_DOORS			(1<<26)	/* Door Area is closed */
#define USE_REALNAMES		(1<<27)	/* Use Real Names only */
#define NO_CHARGING         (1<<28) /* No Charging to System */
#define LOG_DOORS			(1<<29) /* Log doors */
#define LOG_POSTS			(1<<30) /* Log posts */
#define DETECT_ANSI			(1<<31) /* Auto Detect ANSI */

/*
**	Ext Flags for Master 
*/
#define CDROM_BUFFER		(1<<0)	/* CD Rom download buffer */
#define CUSTOM_SCREEN		(1<<1)  /* Open Master Custom Screen */
#define MASTER_REALNAMES	(1<<2)	/* Show Real names in master */
#define CNEWS_SUPPORT		(1<<3)	/* CNews support */
#define LOG_USERTRANSFERS	(1<<4)	/* Log all transfers to user dir */

#define SEM_NEWUSER			(0)		/*	New User Semaphore */
#define SEM_BASE_RW			(1)		/*	Message Base in write state */
#define SEM_POST_DATA		(2)		/*	Semaphore on Post Data Structures */
#define SEM_MAIL			(3)		/*  Use before writing */
#define SEM_USERDATA		(4)		/*  For maintenance, no RW on users */
#define SEM_UPNEW			(5)		/*  For updating newusers */
#define SEM_UPFEED			(6)		/*	For updating feedbacks */



/*
**	Editor Return Flags 
*/

#define ED_SAVE				1		/* Save post */
#define ED_ABORT			2		/* Aborted message */



/*
**	EXCELSIOR! Door Interface Communication Command Codes
*/


/*
 *
 *
 * Commands 0 to 100 are system private. No usable function is performed by
 * these commands for a door interface. Commands under 100 are also handled
 * differently than commands above 100, and you risk the possibility of 
 * a SYSTEM FAILURE if you attempt to use commands below 100.
 *
 *															-Ron
 *
 */

#define CMD_CCLOSE_DOWN		500		/* Shuts down the interface */

#define CMD_GETLINE			100		/* Prompts the program to get a line */
#define CMD_GETCH			101		/* Gets a character, will wait */
#define	CMD_IRGETCH			102		/* Gets a character, returns 0 if */
									/* a character isn't waiting. */
#define CMD_WHATIS			103		/* Returns a pointer to the variable */
									/* requested. All pointers returned by*/
									/* WhatIs() point to public memory */
#define CMD_PRINT_STRING	104		/* Prints out a null terminated string */
#define CMD_CON_WRITE		105		/* Prints out a null terminated string */
									/* to the console only (handy for debug)*/
									/* NO IPL ALLOWED TO CON_WRITE */
#define CMD_RWUBUF			106		/* R/W an account into the User Buffer */
#define CMD_ACTIVATE_EDITOR	107		/* Starts up the editor */
                                    /* Returns ED_SAVE for saving, and */
                                    /* ED_ABORT for aborting */
#define CMD_CHANGE_WHAT		108		/* Changes what the user is doing */
#define CMD_CHANGE_WHERE	109		/* Changes where the user is */
#define CMD_GLOBAL_COMMANDS	110		/* Execute a global command */
#define CMD_PRINT_FILE		111		/* Reads a file to the user */
#define CMD_GET_DATE		112		/* Fills a 'MyDateStamp' with the */
									/* current time and date */
#define CMD_PRINT_DATE		113		/* Returns a pointer to a null 
									   terminated string of a 'MyDateStamp'
									   passed to this function */
#define CMD_CONFIRM_ID		114		/* Returns the ID number of a user
									   by handle or ID number */
#define CMD_SEND_MAIL		115		/* Sends Mail to User ID */
#define CMD_EXPORT_FILE		116     /* A Simple ASCII dump of editor to file */
#define CMD_IMPORT_FILE		117		/* Load a file into editor */
#define CMD_IS_LOGOFF		118		/* Is the user logging off */
#define CMD_IS_SYSOP		119		/* Is the user a sysop */
#define CMD_PARSE_COMMAND   120     /* Parse string, return result code */
#define CMD_SER_WRITE		121		/* Serial Write Command                */
									/* Arg1 = buffer, Arg2 = buffer length */
									/* This command exists mainly for our  */
									/* AUX: handler */
#define CMD_WAIT_CHAR		122		/* Wait x number of micros for a char  */
									/* then return. Arg1 = micros, result1 */
									/* equals a character or a null char   */
#define CMD_ACTION_READ		123		/* Read from list - another AUX: handler*/
									/* command that is useless for most */
									/* doors. Arg1 is buffer to fill, Arg2 */
									/* is number of bytes to read          */
#define CMD_ACTION_SCMODE	124		/* Put the BBS in RAW: mode            */
#define CMD_DOOR_PORT		125		/* Assign address of ecom port */
#define CMD_QUOTE_MACHINE	126		/* Call the Quote Machine (For VisEd's */
#define CMD_START_CHATMODE	127		/* Start Chat Mode !!BECAREFUL!! */

#define WHATIS_USERDATA		1		/* Returns a pointer to the users data */
#define WHATIS_INPUTBUF		2		/* Returns a pointer to Input Buffer */
#define WHATIS_COMMONDATA	3		/* Returns a pointer to Common_Data */
#define WHATIS_DOS			4		/* Private */
