
/* bitdefs for mail "flags" */
#define MAIL_REALNAME	0x00000001
#define MAIL_COPYSELF	0x00000002
#define MAIL_LOWPRI		0x00000004
#define MAIL_HIGHPRI		0x00000008
#define MAIL_HOLD			0x00000010
#define MAIL_CRASH		0x00000020
#define MAIL_FSEND		0x00000040
#define MAIL_FREQ			0x00000080
#define MAIL_OPENED		0x00000100
#define MAIL_REPLIED		0x00000200
#define MAIL_RECEIPT		0x00000400
#define MAIL_SIGNATURE	0x00000800
#define MAIL_QUOTE		0x00001000

#define CNETOUTBOX		"mail:CNETOUTBOX"
#define CNETINBOX			"mail:CNETINBOX"

// Error codes produced when mail-task cannot deliver mail - added 16-Aug-96
// these codes decide what text is returned to the user who SENT the mail
// explaining the reason that mail was "bounced"
#define MAILERROR_NOUSER			0x01	// no user by this name
#define MAILERROR_DISKFULL 		0x02	// could not save data, disk full
#define MAILERROR_LOWBALANCE		0x04	// receiving user has low balance
#define MAILERROR_NORECIPIENT		0x08	// internal error.  MailToList structure
													// not included
#define MAILERROR_NOMHEAD			0x10	// internal error.  No MailHeader4 specified
#define MAILERROR_NOTEXT			0x20	// internal error.  No text in message
#define MAILERROR_NOUUCP			0x40	// internal error.  No UUCP name specified for local mail
#define MAILERROR_NODIR				0x80	// internal error.  Could not create user directory
#define MAILERROR_NOINETSCRIPT	0x100	// internal error.  Could not create/spawn internet mail script
#define MAILERROR_BADFIDOADDR		0x200	// bad/invalid fido address
#define MAILERROR_LOWNETBALANCE	0x400	// recipient does not have enough Net Credits to receive mail
#define MAILERROR_INVALIDMTYPE	0x800	// invalid MailType in struct MailToList

#define MAILERROR_BOXCLOSED      0x1000 // user's mailbox is closed!
#define MAILERROR_FILTERBOUNCE	0x2000 // system wide mailkill on received mail

#define MAILERROR_NOSOCKETLIB		0x4000 // (bsd)socket.library could not be opened


#ifndef CNET_MAIL
#define CNET_MAIL


// Fidonet - from nl.h
struct Addr
	{
	UWORD Zone;
	UWORD Net;
	UWORD Node;
	UWORD Point;
	};

// Fidonet - from nl.h
struct NodeDesc
	{
	struct Addr Node;					// node address
	ULONG Reserved0;

	UWORD Region;						// region (0 if none)
	UWORD Hub;							// node # of this node's HUB (0 if none)
	char *System;						// node name
	char *Sysop;						// sysop name
	char *Phone;						// phone number
	char *City;     					// city and state
	char *Passwd;						// password
	char *Flags;						// nodelist flags
	LONG Cost;							// cost
	UWORD BaudRate;					// baud rate
	LONG ID; 							// unique ID (offset into nodeindex)

	ULONG Reserved1;        		// New for version 5
	ULONG Reserved2;        		// For compatibility with nodelist.library
	ULONG Reserved3;

	UBYTE Type;             		// Entry type
	UBYTE Pad0;
	};


struct OldMailHeader {				// NEW 2.0 for mail, etc
	struct	IsDate  Date;
	struct	IsDate	EDate;		// for EXPIRATION setting
	short	EDays;
	char	Subject [81];				// Ahh, nice and roomy
	char	From    [27];

	ULONG	Number;

	short	ByAccount;
	long	ByID;

	char	To	[28];						// originals, if was Sent/forwarded
	long	ToID;
	short	ToAccount;

	UBYTE	Receipt_NOT;				// obsolete (was return-receipt)
	UBYTE	Return;						// return original message??

	UBYTE	HasOriginal;				// if TRUE, this item contains the recipient's original message also
	UBYTE	File;							// for File Mail
	UBYTE	Anonymous;					// From an anonymous user?

	char	Route	[41];					// for Bulk Mail, list of recips
	short	Recipients;					// number of recipients ...
	UBYTE	Party;						// Allow party responses?

	long	Length;
	long	CheckSum;					// must be 0

	long	Item;							// if this message is repeated
	long	Seek;							// somewhere on the system    
	short	Base;							// this tells where !!!       

	char	NetAddress[52];			// may be shortened

	UBYTE	Alias;
	UBYTE	Replied;
	UBYTE	Received;
	UBYTE	NoEdits;

	struct	IsDate  ShowDate;
};


struct MailHeader4 {				// NEW 2.0 for mail, etc
	LONG HeaderSeek;				// dynamic, set by MailRead() and CheckMail() - location of header in _mhead4
	ULONG Date;						// date mail SENT - time_t value
	ULONG	EDate;					// for EXPIRATION setting - expire DATE (time_t value)
	short	EDays;					// expire days
	char	Subject [81];			// Ahh, nice and roomy
	char	From    [27];			// FROM user - Handle/name

	ULONG	Number;					// unique ID number (16 bit HEX time stamp)

	short	ByAccount;				// if a local user, account number
	long	ByID;						// ""        ""     IDNumber

	char	To	[200];				// originals, if was Sent/forwarded or multi/carbon
	long	ToID;						// ""     ""  IDNumber
	short	ToAccount;				// To account number

	UBYTE	Receipt;					// return-receipt ??
	UBYTE	Return;					// return original message??

	UBYTE	Anonymous;				// From an anonymous user??

	UBYTE	NoSAMCharge;			// v4.13c - do not alter SAM when this item is killed

	BOOL  mfattach;				// if set, "FileAttach" contains the name of a file
                              // which contains a LIST of files that are
                              // attached to this mail header/item.
                              // otherwise "FileAttach" contains the path/filename
                              // of the attached file.

	char	Expansion[126];		// future expansion
	short	Recipients;				// number of recipients ...

	long	Length;					// Length of MESSAGE body
	long	CheckSum;				// must be 0

	long	Item;						// if this message is repeated
	long	Seek;						// somewhere on the system    
	char	UniqueBase[75];		// this tells where - Same as subboard GO keyword !!! 
										// was 21 bytes long up to CNet v4.21

	// this is set by Mail-TASK on incoming mail
	char	NetAddress[80];		// Net address of the user this mail is from if a network mail

	UBYTE	Alias;					// did user use an alias??
	UBYTE	Replied;					// did user reply??
	UBYTE	Received;				// did user receive/read it yet??
	UBYTE	HasOriginal;			// user kept original message??
	UBYTE	NoEdits;					// mail not editable if TRUE

	ULONG ReadDate;				// Date message READ - same as "Date" unless read
	char FileAttach[128];		// full path/filename of attached file
	short set;						// which charge schedule to use - Physical number - Use NumToChargeSet() to get a pointer of type (struct ChargeSet *) to the actual ChargeSet
	ULONG flags;					// Mail flags (See bitdefs at the top of mail.h)

	char magic[13];				// Usually "1234CNET56789" for a VALID/non-corrupt header
                              // Mail recovery programs will search for and         
                              // test this to validate a "recovered" header!
										// CNet MailRead also tests for this to be correct.
                              // Automatic recovery should be active in CNet AMIGA  
                              // v4.13                                              
										// don't forget to get the TEXT from _mtext4 as well!!

	LONG text;						// Seek() value for text in _text4
	UBYTE Killed;					// TRUE if mail is marked for deletion - only used for moving mail to trashcan

	struct MailHeader4 *next;	// set internally - pointer to the next mail header in current folder
};


/* folder structure - use GotoMailFolder(UUCP_ID, FOLDER_NAME) to open a mail folder */
struct NewMailFolder {
	char OwnerID[10];					// UUCP ID of owner
	char path[128];					// path to this folder (usually mail:users/UUCP_ID)
	char Name[40];						// name of mail folder (ie. INBOX, OUTBOX, etc,..
	BPTR headerfh;						// file handle used to access this folder's mail HEADER file
	BPTR textfh;						// file handle used to access this folder's mail TEXT
	UBYTE temporary;					// TRUE if folder was opened on the fly and must be freed after current use
	
	struct NewMailFolder *NextFolder;	/* next folder allocated */
};


struct MailAlias4 {
	char Alias[24];					// 0 - alias to be entered when sending mail
	char Name[24];						// 24 - user's handle/name on the destination system
	char Address[80];					// 48 - destination address - blank if destination is local system
	UBYTE MailType;					// 128 - not currently used
	struct MailAlias4 *NextAlias;	// pointer to next mailalias
};



// the MailToList structure is filled in and returned by CMailIsUUCP,
// CMailIsFido, CMailIsLocal and CMailIsAlias functions           

// the first item written to outbox/#?.mtl is the name of the MailHeader4
// header/message file terminate by 0x0A - then 1 or more instances of   
// struct MailToList                                                     
struct MailToList
{
	char Name[100];	// full network name/address of user
	char UUCP[10];		// UUCP id if local user

	UBYTE MailType;   // type of mail - mailtask uses this to decide what
                     //                user name to use on outgoing mail
                     //                and which file-attach method to  
                     //                use and also how to send the mail
                     //                                                 
                     //              - 0=LOCAL REALNAME, 1=local HANDLE,
                     //                2=UUCP,           3=FIDO         
                     //                4=LOCAL-ALIAS,    5=Handle from  
                     //                                    FindAccount  
							//                6=FEEDBACK        7=NEWUSER      
							//                8="SYSOP"                        
	struct MailToList *next;
};


struct CNetNodeDesc	// possible argument to IsFido()
	{
	char UserName[30];			// User name
	ULONG Node[4];					// node address
	UWORD Region;					// region (0 if none)
	UWORD Hub;						// node # of this node's HUB (0 if none)
	char System[60];				// node name
	char Sysop[60];				// sysop name
	char Phone[30];				// phone number
	char City[60];   				// city and state
	char Flags[60];				// nodelist flags
	LONG Cost;						// cost
	UWORD BaudRate;				// baud rate
	UBYTE Type;                // Entry type
};



/**************************************************************************
 * structure used in mail:users/UUCP_ID/twitfilter
 * mail from user(s) matching the fields within will not be saved
 * and the mail will be bounced back to the originating user
 * stating that the recipient is blocking his messages
 **************************************************************************/
struct MailFilter
	{
	char Name[100];						// full name of user to filter - may be UUCP name, real name or handle
	ULONG Node[4];							// network address to filter
	BYTE expansion[100];					// future expansion
	struct MailFilter *NextFilter;	// internal use only
	};


// defines for CNAddressType() - see cnetfuncs.h
#define ADDRESSTYPE_FIDO     0x01
#define ADDRESSTYPE_INTERNET 0x02
#define ADDRESSTYPE_LOCAL    0x03
#define ADDRESSTYPE_UNKNOWN  0x04

#define MAILFILTER_NAMEADDR      0
#define MAILFILTER_SUBJECT       1



// For linked list of spooled mail files tracked by mail-task.
// Spool only used when "MailSend: Use internal SMTP routines" selected in
// Config/Options
struct MailSpoolEntry
	{
	char filename[128];
	struct MailSpoolEntry *next;
	};



// IMPStruct used for Intercept Mail Processing duties
struct IMPStruct
	{
	struct Node IMP_node;	// Exec Node for this instance of IMPStruct
	char Name[128];			// name/pattern to trap - IMP_node.ln_Name points here
	char ProcessName[128];	// script/executable/subboard GO arg used to process this name/pattern
	char ProcessReply[128];	// for mailing-list gating of subboard posts/replies, reply-to this address

	ULONG dflag;				// detect flag bits
									// 0 = detect apparent sender
									// 1 = detect apparent recipient
													// 2 = uumail file (not yet implemented

	ULONG pflag;				// processing type flag bits.
									// 0 = ignore
									// 1 = by script
									// 2 = mailing list gate
	};

#define IMPTYPE_NORMAL			0L
#define IMPTYPE_SCRIPT			(1L << 0)
#define IMPTYPE_MAILLIST		(1L << 1)

#define IMPDETECT_SENDER		0L
#define IMPDETECT_RECIPIENT	(1L << 0)


struct MIMEContentType
	{
	char type[64];		 // name of content (eg. application/octet-stream)

	char encoding[64]; // encoding type (eg. base64)

	UBYTE method;		 // 0-255; how to handle attachment
							 // current methods:   0=leave as email
							 //                    1=add to user mailbox as CNet FILE-attachment
							 //                    2=Save as VALIDATED file in base with GO KEY specified in ProcessArg1
							 //                    3=Save as UNVALIDATED file in base with GO KEY specified in MethodArg1
							 //                    4=save to >NIL: (eg. delete/lose it from the system)

	char methodarg[75]; // currently only functions as GO ARG for a subboard
							  // for method types 2 or 3.

	char decode[128]; // command to decode attachment.  Any %n used in this command
                        // will be replaced with the temp filename used for the
                        // attachment.  Any %t in this command will be replaced
								// with the DESTINATION (eg decoded) filename to be used
								// for this attachment.  If there is no decode command
                        // specified, the RAW attachment will be saved to the
                        // location associated with the Method for this type
								//
								// substitutions:  %f = FROM (temp) filename
								//                 %t = attachment filename if any. a unique filename otherwise.

	char encode[128]; // future use - to be used for encoding outgoing MIME
	                  // messages.

	struct MIMEContentType *next;	// pointer to next instance.
	};

#endif
