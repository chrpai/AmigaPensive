CONSOLE			Equ	0
SERIAL			Equ	1
PRINTER			Equ	2

EDOS_READ		Equ	(1<<0)
EDOS_WRITE		Equ	(1<<1)

; GetLine Flags

GL_BOX			Equ	(1<<0)	; Draws An ANSI Box (length of max input)
GL_IN			Equ	(1<<1)	; Passing Something Into Input, goes in ID->IR_Buffer
GL_FLUSH		Equ	(1<<2)	; Flush the Buffers
GL_UPPER		Equ	(1<<3)	; Upper Case Only
GL_CAP			Equ	(1<<4)	; Captialize First Letter Only?
GL_HIDE			Equ	(1<<5)	; Print 'X', good for Passwords
GL_NUM			Equ	(1<<6)	; Numbers and  -,(,)
GL_ALPHA		Equ	(1<<7)	; Letters Only
GL_SYM			Equ	(1<<8)	; NO SYMBOLS!
GL_DOS			Equ	(1<<9)	; Anything Can Be Entered
GL_EDITOR		Equ	(1<<10)	; We're in editor mode
GL_DIGIT		Equ	(1<<11)	; Digits Only (0-9)
GL_NOANSI		Equ	(1<<12)	; for times we wish to ignore ANSI movements
GL_NOIUMS		Equ	(1<<13)	; No IUMs will be delivered
GL_WRAP			Equ	(1<<14)	; Word Wrap
GL_CHAT			Equ	(1<<15)	; Chat mode
GL_NOCMDSTACK		Equ	(1<<16)	; Turns off Command Stacking
GL_RETURN_DEL		Equ	(1<<17)	; Return if bspace and col 0 (for editor) 
GL_CONFERENCE		Equ	(1<<18)	; We are in a conference, report accordingly
GL_NOHISTORY		Equ	(1<<19)	; No history buffer
GL_SPECIAL		Equ	(1<<20)	; use IPL
GL_COMMODE		Equ	(1<<21)	; COM: mode
GL_HANDLE		Equ	(1<<22)	; Enter a handle only
GL_NORETURN		Equ	(1<<23)	; Do not print the return when hit
GL_LINEONE		Equ	(1<<24)	; Private CB flag


; IO Progress/Process Flags

IO_ASREAD		Equ	(1<<0)	; ASync serial IO read pending
IO_ASWRITE		Equ	(1<<1)	; ASync serial IO write pending
IO_ACREAD		Equ	(1<<2)	; ASync console IO read pending
IO_ACWRITE		Equ	(1<<3)	; ASync console IO read pending
IO_SERACT		Equ	(1<<4)	; Serial device open
IO_BAR			Equ	(1<<5)	; Stat bar on
IO_CARRIER		Equ	(1<<6)	; Set when carrier is detect is true
IO_ANSI			Equ	(1<<7)	; Is the user in ANSI mode?
IO_SHUTDOWN		Equ	(1<<8)	; We're being told to SHUTDOWN NOW!!!
IO_XPRABORT		Equ	(1<<9)	; Last transfer aborted
IO_BEEP			Equ	(1<<10)	; Beep for sysop
IO_NOTIFY		Equ	(1<<11)	; Notify SysOp after transfer
IO_CONNECTED		Equ	(1<<12)	; Someone is connected


; Bits for each node

NODE_INVISIBLE		Equ	(1<<0)	; Is the node invisible to others
NODE_MONITOR		Equ	(1<<1)	; Is the node monitoring log on/offs
NODE_WATCH		Equ	(1<<2)	; Is the node watching another node
NODE_MUTE		Equ	(1<<3)	; Is the node 'muted' from others
NODE_SHUTDOWN		Equ	(1<<4)	; Shutdown node upon user logoff
NODE_GLNOCHAR		Equ	(1<<5)	; Force an IUM upon the user
NODE_IUMREADY		Equ	(1<<6)	; An IUM is waiting, fall out
NODE_LOCALMODE		Equ	(1<<7)	; We are in local mode, no SERIAL I/O
NODE_DUMPUSER		Equ	(1<<8)	; Dump the user off the system - IMMEDIATE!
NODE_CHATMODE		Equ	(1<<9)	; We are in SysOp Chat
NODE_INTERLACE		Equ	(1<<10)	; Are we in interlace?
NODE_2COLOR		Equ	(1<<11)	; We are in 2 color
NODE_4COLOR		Equ	(1<<12)	; We are in 4 color
NODE_8COLOR		Equ	(1<<13)	; We are in 8 color
NODE_16COLOR		Equ	(1<<14)	; We are in 16 color
NODE_WORKBENCH		Equ	(1<<15)	; We are on the workbench screen
NODE_PRINTER		Equ	(1<<16)	; Printer is active
NODE_MESS_READ		Equ	(1<<17)	; Reading a message, make ANSI window
NODE_TRANSFER		Equ	(1<<18)	; Someone is transfering right now
NODE_NOIUMS		Equ	(1<<19)	; Dont send an IUM, WAIT!
NODE_TIMERET		Equ	(1<<20)	; Return from a prompt if Timer 3 went off
NODE_ISFEEDBACK		Equ	(1<<21)	; This is a feedback message
NODE_NOIPL		Equ	(1<<22)	; No IPL will come across until a prompt of EOF
NODE_NOSYSOP		Equ	(1<<23)	; No Sysop Commands
NODE_NEWUSER		Equ	(1<<24)	; This is a new user letter
NODE_IBMCOLOR		Equ	(1<<25)	; Uses IBM color Palette
NODE_RELOGON		Equ	(1<<26)	; User is relogging on
NODE_CONSOLE		Equ	(1<<27)	; This is a console session
NODE_SPLIT		Equ	(1<<28)	; Split screen
NODE_USERBUFFER		Equ	(1<<29)	; Auto-Buffer to disk is active!
NODE_CON_BUFFER		Equ	(1<<30)	; Console buffer is on!
NODE_TEMPSYSOP		Equ	(1<<31)	; A temporary sysop until log off or cancellation

MNODE_NORMALOFF		Equ	(1<<0)	; The user manually logged off
MNODE_SUSPENDED		Equ	(1<<1)	; The Time has been suspended (ie transfers)
MNODE_SYSOPPAGE		Equ	(1<<2)	; Someone has paged the sysop
MNODE_CKABORT		Equ	(1<<3)	; Aborted Transfer
MNODE_NOIUC		Equ	(1<<4)	; We are at a point where we can not accept IUC requests or grab chats
MNODE_IUC		Equ	(1<<5)	; We are now in a Inter-User Chat -  This flag MAY be set when a user IS NOT in chat! Internal use only!!
MNODE_NOWRAPOUT		Equ	(1<<6)	; No Word Wrap on the output
MNODE_NOMORE		Equ	(1<<7)	; No more prompt
MNODE_NOABORT		Equ	(1<<8)	; Cannot abort message
MNODE_ISXMODEM		Equ	(1<<9)	; ??
MNODE_STARTEDIUC	Equ	(1<<9)	; We're the port that started the IUC
MNODE_OFFQUICK		Equ	(1<<10)	; Logoff Quick Requested by User
MNODE_EDITOR		Equ	(1<<11)	; We're currently in the editor
MNODE_DOQUICK		Equ	(1<<12)	; We're in quick mode
MNODE_ISDOWNLOAD	Equ	(1<<13)	; Current Transfer is a DOWNLOAD
MNODE_QUOTEDOALL	Equ	(1<<14)	; Quoting - Quote ALL!
MNODE_INT_EDITOR	Equ	(1<<15)	; We are currently in the editor
MNODE_POSTING		Equ	(1<<16)	; We are posting
MNODE_RESPONDING	Equ	(1<<17)	; We are responding
MNODE_UUCP_POST		Equ	(1<<18)	; We are posting public uucp
MNODE_UUCP_MAIL		Equ	(1<<19)	; We are posting private uucp
MNODE_CONFERENCE	Equ	(1<<20)	; We are in the conference
MNODE_CONF_READY	Equ	(1<<21)	; Ok to print conference messages
MNODE_SPECIAL_ED	Equ	(1<<22)	; We are in a special ed env
MNODE_NO_PARMARK	Equ	(1<<23)	; Do NOT show markers for CR/LF's
MNODE_WARN_LOWTIME	Equ	(1<<24)	; Warn User of low time
MNODE_WARNING_SET	Equ	(1<<25)	; User has been warned!
MNODE_ANSWERMODE	Equ	(1<<26)	; AutoAnswer Mode 
MNODE_ODU_REQ		Equ	(1<<27)	; Another process wants the serial.device
MNODE_AUTOOFF		Equ	(1<<28)	; Log the user off after transfer
MNODE_OPENAFTERT	Equ	(1<<29)	; Open Screen After Transfer
MNODE_RETURNMOUSE	Equ	(1<<30)	; Return mouse buttons every mouse click
MNODE_MOUSECLICK	Equ	(1<<31)	; A mouse click, return from GetCH()


; Extended Bits

EXT_CONSOLE_CHAT	Equ	(1<<0)	; We are in a console chat
EXT_LEFT_CON_CHAT	Equ	(1<<1)	; We left console chat with close gad
EXT_IBMCOLOR		Equ	(1<<2)	; IBM Color Emulation mode
EXT_NEWUSER_MAIL	Equ	(1<<3)	; We are reading new user app's
EXT_UUCP_RESPONDING	Equ	(1<<4)	; We are responding to UUCP messages
EXT_COLORIFIC		Equ	(1<<5)	; We are in colorific mode
EXT_DELAY_CHAR		Equ	(1<<6)	; We are in delay char mode
EXT_EXIT_CHAT		Equ	(1<<7)	; We just left chat mode
EXT_FILE_MAIL		Equ	(1<<8)	; We are sending file mail
EXT_RAW_MODE		Equ	(1<<9)	; We are in RAW: mode for AUX:
EXT_RESUME_MODE		Equ	(1<<10)	; Upload resume mode
EXT_COM_ACTIVE		Equ	(1<<11)	; We are in COM mode
EXT_GETLINE		Equ	(1<<12)	; We are in GetLine
EXT_ASYNCREAD		Equ	(1<<13)	; Async ECOM: Read
EXT_CALLBACK		Equ	(1<<14)	; We are in callback mode
EXT_WATCHMODE		Equ	(1<<15)	; Watching a node
EXT_BEINGWATCHED	Equ	(1<<16)	; We are being watched
EXT_STOPTHEWATCH	Equ	(1<<17)	; In one way or another, this node is in use
EXT_PROCMD		Equ	(1<<18)	; We want to process a command
EXT_PRINTWATCH		Equ	(1<<19)	; We want to process a command
EXT_DOSDOOR_OUTPUT	Equ	(1<<20)	; dos door output only, NO IPL!
EXT_CURSOR_ON		Equ	(1<<21)	; The cursor is on!
EXT_SCANALL		Equ	(1<<22)	; Scan all mode
EXT_NOTIFYARCS		Equ	(1<<23)	; Notify user they cannot upload archive
EXT_NOCHATMODE		Equ	(1<<24)	; No chat mode right now
EXT_NOTITLEBAR		Equ	(1<<25)	; No title bar mode
EXT_BATCH_QWK		Equ	(1<<26)	; Batch QWK files
EXT_QWK_UPLOAD		Equ	(1<<27)	; Uploading QWK files
EXT_IMPORT_QWK		Equ	(1<<28)	; Currently Importing QWK Files
EXT_PARAGON_DOOR	Equ	(1<<29)	; A paragon door is active
EXT_NO_IUMS		Equ	(1<<30)	; No IUMs for the time being
EXT_MOUSEHIT		Equ	(1<<31)	; Mouse Click was hit


; Exp Bits

EXP_FORCE_EXIT_IUC	Equ	(1<<0)	; Force User out of IUC
EXP_FORCE_EXIT_CB	Equ	(1<<1)	; Force user out of CB
EXP_WARN_CBIUC		Equ	(1<<2)	; Warn user of low time in CB/IUC
EXP_USED_TEMPLATE	Equ	(1<<3)	; Template has been used
EXP_LOGOFF_AFTER	Equ	(1<<4)	; User is to log off after transfer
EXP_ALT_TABLE		Equ	(1<<5)	; Alternate translation table used
EXP_IUC_MODE		Equ	(1<<6)	; IUC Mode is active
EXP_EDDS_MODE		Equ	(1<<7)	; EDDS Mode is active
EXP_EDDS_REFUSED	Equ	(1<<8)	; We recieved a refuse packet
EXP_EDDS_STARTED	Equ	(1<<9)	; Packet accepted and calling out
EXP_EDDS_BUSY		Equ	(1<<10)	; Number was busy
EXP_EDDS_NOCARRIER	Equ	(1<<11)	; No connection or connection lost
EXP_EDDS_END		Equ	(1<<12)	; End this EDDS session
EXP_EDITOR_UPLOAD	Equ	(1<<13)	; Uploading to the editor			
EXP_MASS_MAILER		Equ	(1<<14)	; Mass mailer in progress
EXP_LEFT_CHAT		Equ	(1<<15)	; We left chat, reprint prompt
EXP_IGNORE_IDLE		Equ	(1<<16)	; Ignore idle time 				
EXP_LINKUP		Equ	(1<<17)	; Waiting for call, link up instead
EXP_LINKED_UP		Equ	(1<<18)	; We are now linked up 			
EXP_RETURN_PATH		Equ	(1<<19)	; Its on its return, quiet!		
EXP_SYSTEM_LINK		Equ	(1<<20)	; System is linked					
EXP_MANUAL_LINK		Equ	(1<<21)	; This was a manual link			
EXP_LINK_STARTUP	Equ	(1<<22)	; This link was loaded				
EXP_CALLBACK		Equ	(1<<23)	; We are in callback, dont exit wait()
EXP_CLOSE_LOGOFF	Equ	(1<<24)	; Close line after logoff
EXP_NEWSCAN		Equ	(1<<25)	; New scan/read in progress
EXP_IGNORE_CB		Equ	(1<<26)	; Ignore CB messages for the moment
EXP_COMMAND_USED	Equ	(1<<27)	; This command was used okay
EXP_EDIT_COMMENT	Equ	(1<<28)	; Edit comment in editor
EXP_SHUTTLE		Equ	(1<<29)	; Shuttle logon in progress
EXP_SCANNING		Equ	(1<<30)	; We are scanning files
EXP_OFF_HOOK		Equ	(1<<31)	; Go off hook


; System Bits

SYS_NONEWUSERS		Equ	(1<<0)	; No new users
SYS_NOFBASE		Equ	(1<<1)	; FBase is Closed
SYS_NOMBASE		Equ	(1<<2)	; MBase is Closed
SYS_NOPBASE		Equ	(1<<3)	; PBase is Closed
SYS_NOTBASE		Equ	(1<<4)	; TBase is Closed
SYS_NOFEEDS		Equ	(1<<5)	; No UUCP Feeds
SYS_SYSOPIN		Equ	(1<<6)	; The sysop is in


; PrintFile Flags

ERR_FNF			Equ	(1<<0)	; Prints A %s Not Found for Filenames
NOABORT			Equ	(1<<1)	; User Can't Abort The File Display
NO_IPL			Equ	(1<<2)	; IPL Commands will be displayed
SCR_CLR			Equ	(1<<3)	; Clear screen before displaying
NO_TT			Equ	(1<<4)	; No term type processing
IPL_SECURE		Equ	(1<<5)	; Show IPL, but Security!
CMD_FILE		Equ	(1<<6)	; This is a command file


; USER_INFO_BITS  (userinfo.User_Info_Bits)

EI_MALE			Equ	(1<<0)	; Extended info, if set user is male
EI_ANSI			Equ	(1<<1)	; Do we use ANSI?
EI_EXPERT		Equ	(1<<2)	; Expert Help Level Flag
EI_PRIVATE		Equ	(1<<4)	; Private Info?
EI_MORE			Equ	(1<<5)	; More prompts active?
EI_LINEFEEDS		Equ	(1<<6)	; Do we need linefeeds?
EI_INT_TIME		Equ	(1<<7)	; International 24 hour time
EI_LOCKOUT		Equ	(1<<8)	; Is user locked out of the system?
EI_ACTIVE		Equ	(1<<9)	; Is This Account Active?
EI_ANSIED		Equ	(1<<10)	; Use ANSI Full Screen Editor by default
EI_ANSISCROLL		Equ	(1<<11)	; Scroll messages (ANSI WINDOW)
EI_CALLBACK		Equ	(1<<12)	; This user has been called and tested
EI_NORMAL		Equ	(1<<13)	; Intermediate Help Level
EI_NOVICE		Equ	(1<<14)	; Nove help level
EI_BATCH_CR		Equ	(1<<15)	; Add CR to batch files
EI_RESPOND_PAUSE	Equ	(1<<16)	; Pause between responses
EI_AUTO_SIG		Equ	(1<<17)	; If set, wont ask to add sig, else query 
EI_MAILCLOSED		Equ	(1<<18)	; Mail Box Open?
EI_UUCP_FORWARD		Equ	(1<<19)	; Forward to UUCP
EI_FIDO_FORWARD		Equ	(1<<20)	; Forward to FIDO
EI_BROWSE		Equ	(1<<21)	; Enable browse prompt


; Extended Access Flags (userinfo.E_Access.AccBits)

EA_SYSOP		Equ	(1<<0)	; SysOp Privileges?
EA_RELOGON		Equ	(1<<1)	; Can User Re-Logon?
EA_NOCHARGE		Equ	(1<<2)	; No charges for this account
EA_MAIL_FILE		Equ	(1<<3)	; Can User Send File Mail
EA_MAIL_ALIAS		Equ	(1<<4)	; Can User Post As An Alias
EA_MAIL_FORWARD		Equ	(1<<5)	; Can User Forward Mail
EA_MAIL_CARBON		Equ	(1<<6)	; Can user send Carbon Copy letters
EA_SHOW_ANON		Equ	(1<<7)	; Can User See Real Annoymous
EA_DEL_FILES		Equ	(1<<8)	; Can User Delete Any File
EA_DEL_OWN		Equ	(1<<9)	; Can User Delete OWN Files
EA_VALIDATE		Equ	(1<<10)	; Auto-Validate UL's
EA_UNRESTR_FILE		Equ	(1<<11)	; Unrestricted File Credits
EA_UNRESTR_BYTE		Equ	(1<<12)	; Unrestricted Byte Credits
EA_HAVE_ALIASES		Equ	(1<<13)	; Can user have aliases
EA_CONF_CTRL		Equ	(1<<14)	; Can User Control Conferences
EA_BROADCAST		Equ	(1<<15)	; Can User Send Public Broadcasts
EA_RECEIVE_BROD		Equ	(1<<16)	; Can User Receive Broadcast
EA_CHANGE_ALIAS		Equ	(1<<17)	; Can User Change Alias
EA_ENTER_UUCP		Equ	(1<<18)	; Enter A UUCP BASE
EA_SEND_UUCP		Equ	(1<<19)	; Send UUCP Public Posts
EA_ENTER_FIDO		Equ	(1<<20)	; Enter A FIDO BASE
EA_SEND_FIDO		Equ	(1<<21)	; Send FIDO Public Posts
EA_WATCH		Equ	(1<<22)	; Can user "watch" others
EA_OPENSCREEN		Equ	(1<<23)	; Open Screen Upon Logon										 	
EA_ANONYMOUS		Equ	(1<<24)	; Can User Post Annonymously
EA_PRIVATEAREA		Equ	(1<<25)	; Can have a private area
EA_BUFFER_OUT		Equ	(1<<26)	; Buffer Entire output!
EA_DOS_DOWNLOAD		Equ	(1<<27)	; User can enter direct path and filenames for downloading
NOT_IN_USE_USEME
NOT_IN_USE_USEME2
EA_FULLWATCH		Equ	(1<<30)	; User can type while in "WATCH"
EA_BYPASS_BAUD		Equ	(1<<31)	; User can bypass min bauds


; More Access Bits  (userinfo.E_Access.More_AccBits)

MA_NO_WHONOTES		Equ	(1<<0)	; User cannot have any Who Notes
MA_AUTO_INVIS		Equ	(1<<1)	; Auto Invisible
MA_UUCP_FEED		Equ	(1<<2)	; This is not a user, for uucp feeds
MA_NO_CHARGES		Equ	(1<<3)	; This user is exempt from charges
MA_DIALOUT		Equ	(1<<4)	; Able to Dial Out
MA_AUTO_MUTE		Equ	(1<<5)	; Auto mute upon login
MA_AUTO_WHO		Equ	(1<<6)	; Auto show who upon login
MA_LINK_ACCOUNT		Equ	(1<<7)	; Link account
MA_AUTO_MONITOR		Equ	(1<<8)	; Monitor at login
MA_GLOBAL_SUBOP		Equ	(1<<9)	; This user is a global sub-op
MA_BATCH_UUCP		Equ	(1<<10)	; User can batch UUCP messages
MA_BATCH_FIDO		Equ	(1<<11)	; User can batch FIDO messages
MA_CRASHMAIL		Equ	(1<<12)	; User can send Crash mail
MA_FILEREQUEST		Equ	(1<<13)	; File Request files
MA_MAIL_QUEUE		Equ	(1<<14)	; Send Mail Distribution Queue letters
MA_AUTO_BATCH		Equ	(1<<15)	; User can set an auto-batch time
MA_DEFINE_UUCPADDR	Equ	(1<<16)	; User can edit/create their UUCP net address
MA_DIALOUT_ANY		Equ	(1<<17)	; User can dial any number from the sys
MA_EDIT_PERSONAL	Equ	(1<<18)	; User can edit their personal data
MA_MASS_MAIL		Equ	(1<<19)	; User can send mass mail letters
MA_IGNORE_TLOCK		Equ	(1<<20)	; Timelocks do not affect user
MA_AUTOMUTE_ALL		Equ	(1<<21)	; Auto Mute Messages To All
MA_CALLBACK_ALWAYS	Equ	(1<<22)	; Callback user each call


; MAIL_Flag Bits

MAIL_RECEIPT		Equ	(1<<0)	; Send A Receipt if No Reply or Forward-Kept
MAIL_ORIGINAL		Equ	(1<<1)	; Return Original Message?
MAIL_FILE		Equ	(1<<2)	; Is this a file message?
MAIL_DELETE		Equ	(1<<3)	; If Its A File, Delete After
MAIL_URGENT		Equ	(1<<4)	; Is it Urgent Mail?
MAIL_FORWARDED		Equ	(1<<5)	; This is an anonymous message
MAIL_CARBONCOPY		Equ	(1<<6)	; Part of a Carbon Copy
MAIL_READ		Equ	(1<<7)	; The letter has been read
MAIL_KILL		Equ	(1<<8)	; The letter has been MARKED FOR DEATH
MAIL_UUCP_MAIL		Equ	(1<<9)	; This is a UUCP Netmail message
MAIL_FIDO		Equ	(1<<10)	; This is a FIDO Netmail message
MAIL_REPLYSENT		Equ	(1<<11)	; Reply has been sent
MAIL_QUEUED		Equ	(1<<12)	; This has been a mail queue
MAIL_MASS		Equ	(1<<13)	; This is a mass mail letter


; Config Bits

SYSTEM_PRIVATE		Equ	(1<<0)	; Is this a Private BBS? If so, prompt for passwd
FBACK_AT_LOGIN		Equ	(1<<1)	; After x attempts to login, ask to leave feedback?
OPT_USER_LETTER		Equ	(1<<2)	; Let users choose to leave new user letter
MAN_USER_LETTER		Equ	(1<<3)	; Manual new user letter
LOGIN_WHO		Equ	(1<<4)	; User can type Who at login
FILTER_ANSI		Equ	(1<<5)	; Filter out ansi for captures
EVERYTHING_FREE		Equ	(1<<6)	; Everything is free
NO_FILEBASE		Equ	(1<<7)	; File Base is closed
NO_MESSBASE		Equ	(1<<8)	; Message Base is closed
LOG_TRANSFERS		Equ	(1<<9)	; Record Transfers
SMART_PATHS		Equ	(1<<10)	; Use dir nest
SYSOP_AVAILABLE		Equ	(1<<11)	; Sysop Is available for chatting
SHOWBAR			Equ	(1<<12)	; Status Bar ON upon login
SHOWSTAT		Equ	(1<<13)	; Stats Window ON upon login
OPENSCREEN_ALL		Equ	(1<<14)	; Open Screen on ALL Logins
FBACK_MAILONE		Equ	(1<<15)	; Feedback goes to ID #1 (sysop)
CHECKARC_UPLOAD		Equ	(1<<16)	; Check arc's after upload 
CHECKARC_MAINT		Equ	(1<<17)	; Check arc's at maint
CALLBACK_ENABLE		Equ	(1<<18)	; Auto Callback Enabled
CALLBACK_TOLLSAVER	Equ	(1<<19)	; TollSaver Callback enabled
CONARC_UPLOAD		Equ	(1<<20)	; Convert Archives after upload
CONARC_MAINT		Equ	(1<<21)	; Convert Archives at maint
IMPORT_NETMAIL		Equ	(1<<22)	; Import netmail
RECEIVE_POLLS		Equ	(1<<23)	; Receive polls
USE_OWNDEVUNIT		Equ	(1<<24)	; Use owndevunit
USE_LOCALED		Equ	(1<<25)	; Use local editor
NO_DOORS		Equ	(1<<26)	; Door Area is closed
USE_REALNAMES		Equ	(1<<27)	; Use Real Names only
NO_CHARGING		Equ	(1<<28)	; No Charging to System
LOG_DOORS		Equ	(1<<29)	; Log doors
LOG_POSTS		Equ	(1<<30)	; Log posts
DETECT_ANSI		Equ	(1<<31)	; Auto Detect ANSI


; Ext Flags for Master

CDROM_BUFFER		Equ	(1<<0)	; CD Rom download buffer
CUSTOM_SCREEN		Equ	(1<<1)	; Open Master Custom Screen
MASTER_REALNAMES	Equ	(1<<2)	; Show Real names in master
CNEWS_SUPPORT		Equ	(1<<3)	; CNews support
LOG_USERTRANSFERS	Equ	(1<<4)	; Log all transfers to user dir
USE_GUI			Equ	(1<<5)	; Use GUI
APPEND_RESPONSES	Equ	(1<<6)	; Append all responses to the end
USE_FILENOTE		Equ	(1<<7)	; Use filenotes for files ul'd
FILTER_LOG		Equ	(1<<8)	; Filter IPL from caller log
NOTIFY_UNVALIDATED	Equ	(1<<9)	; Notify SysOp unvalidated files
CONSOLE_LOCKED		Equ	(1<<10)	; The console is locked!
USE_DEFAULTS		Equ	(1<<11)	; User newuser defaults
USE_ASL			Equ	(1<<12)	; Use ASL requesters
ROUTE_BADMSGS		Equ	(1<<13)	; Route msgs to sender
DUPECHECK_ON		Equ	(1<<14)	; XMAIL! Dupe Checking is on

; For autobatcher
BATCH_ENABLED		Equ	(1<<7)	; The batch is enabled


; Semaphores

SEM_NEWUSER		Equ	0	; New User Semaphore
SEM_BASE_RW		Equ	1	; Message Base in write state
SEM_POST_DATA		Equ	2	; Semaphore on Post Data Structures
SEM_MAIL		Equ	3	; Use before writing
SEM_USERDATA		Equ	4	; For maintenance, no RW on users
SEM_UPNEW		Equ	5	; For updating newusers
SEM_UPFEED		Equ	6	; For updating feedbacks
SEM_BUMPRESPONSE	Equ	7	; Bumps the response
SEM_LOGFILE		Equ	8	; Semaphore for log files
SEM_NODE		Equ	9	; Semaphore for the nodes
SEM_COMMENTS		Equ	10	; Semaphore for the file comments
SEM_BBSTEXT		Equ	11	; Semaphore for all BBStext
SEM_SUBS		Equ	12	; Semaphore for the sub-boards
SEM_MENU		Equ	13	; Semaphore for the menu commands


; System Bits

BBS_RIP			Equ	(1<<0)	; Rip Mode is active
BBS_SEARCHBODY		Equ	(1<<1)	; Searching body for find()
BBS_LOGOFF		Equ	(1<<2)	; User is logging off
BBS_MOVETOFILES		Equ	(1<<3)	; Move to file base
BBS_MOVEITEM		Equ	(1<<4)	; We are moving an item
BBS_NOTEXT		Equ	(1<<5)	; Do not print any text, RIPmode
BBS_MUTE_TOALL		Equ	(1<<6)	; Mute messages sent to all
BBS_RIP_IUM		Equ	(1<<7)	; An IUM has been sent, redraw RIP
BBS_TERM		Equ	(1<<8)	; BBS Term mode
BBS_KEYBOARD		Equ	(1<<9)	; Away from keyboard
BBS_REXX_NOIUM		Equ	(1<<10)	; ARexx has requested no iums


; Post/File extended flag data

POST_ANONYMOUS		Equ	(1<<0)	; Annomouse ;-)
POST_RESTRICTED		Equ	(1<<1)	; UnValidated
POST_NORESPONSE		Equ	(1<<2)	; Responses are locked out
POST_PERSERVED		Equ	(1<<3)	; Will not be deleted by Maintenance
POST_INCOMPLETE		Equ	(1<<4)	; Half-cooked Z-Modem upload
POST_PRVT		Equ	(1<<5)	; Private Post
POST_NOCRED		Equ	(1<<6)	; A none credit penalty download
POST_FILE		Equ	(1<<7)	; This is a file!
POST_NOCHARGE		Equ	(1<<8)	; This is a NO-CHARGE (Accounting) file
POST_OFFLINE		Equ	(1<<9)	; offline file
POST_UPDATE		Equ	(1<<10)	; This file has been edited, update it!
POST_KILL		Equ	(1<<11)	; This post has been axed (schiched)
POST_UNTESTED		Equ	(1<<14)	; This archive has not been tested
POST_TESTED		Equ	(1<<15)	; This is a good archive
POST_FAILED		Equ	(1<<16)	; This archive has failed
POST_AGRAB		Equ	(1<<17)	; Auto-grab this item when read
POST_IMPORTED		Equ	(1<<18)	; This post has been imported
POST_EXPORTED		Equ	(1<<19)	; This post has been exported
POST_RECEIVED		Equ	(1<<20)	; This post has been received by user
POST_FAVORITE		Equ	(1<<21)	; This item is a sysop favorite
POST_LOCAL		Equ	(1<<22)	; This was a local FidoNet post
POST_FILEID		Equ	(1<<23)	; This archive has a file_id.diz file
POST_LONG_DESC		Equ	(1<<24)	; This file has a long desc


; Subdirectory Bits -- For Use In Mess/Transfer Base

SUB_CLOSED		Equ	(1<<0)	; Area Closed, Only Sysops can enter
SUB_DOS			Equ	(1<<1)	; If Set, It is a dos sub (direct DOS)
SUB_UUCP		Equ	(1<<2)	; UUCP SubDirectory
SUB_FIDO		Equ	(1<<3)	; FIDO SubDirectory
SUB_DIRECTORY		Equ	(1<<4)	; This is JUST A SUBDIRECTORY - NO,MESS/FILES
SUB_NORMAL		Equ	(1<<5)	; A Normal Base/File Area
SUB_ALPHABETIZE		Equ	(1<<6)	; DEFAULT: List Items Alphabetically
SUB_NEWFIRST		Equ	(1<<7)	; DEFAULT: List Items Newest First
SUB_OLDFIRST		Equ	(1<<8)	; DEFAULT: List Items Oldest First
SUB_MALE		Equ	(1<<9)	; If Set, Only Males Can Enter
SUB_FEMALE		Equ	(1<<10)	; If Set, Only Females Can Enter
SUB_PSEUDO		Equ	(1<<11)	; Use Pseudo-filenames


; Extended Flags For Base & File Area

ESUB_ALL_ANON		Equ	(1<<0)	; Everything Is Anonymous
ESUB_VALIDATE		Equ	(1<<1)	; Auto-Validate All Uploads
ESUB_ANY_ANON		Equ	(1<<2)	; Can anything be anonymous
ESUB_SHOW_REAL		Equ	(1<<3)	; Show Users Real Names on UL's & Posts
ESUB_FREE		Equ	(1<<4)	; Free Area (no UL/DL credit change)
ESUB_ALT_RATIO		Equ	(1<<5)	; Use Alternate UL/DL Ratio
ESUB_ALT_CHARGS		Equ	(1<<6)	; Use Alternate Charge Template
ESUB_FILE_CHECK		Equ	(1<<7)	; Check For Duplicate Files Globally
ESUB_PASSWORD		Equ	(1<<8)	; User Must Enter Password To Gain Entry
ESUB_NOCHARGE		Equ	(1<<9)	; No charge for sub (ACCOUNTING) 
ESUB_ALL_UNVAL		Equ	(1<<10)	; All items are automatically UNVALIDATED
ESUB_NO_SIGS		Equ	(1<<11)	; No signatures added
ESUB_AUTO_ADD		Equ	(1<<12)	; AMaint will add all files in this directory automatically
ESUB_FORCE_JOIN		Equ	(1<<13)	; This acct is force join always
ESUB_AUTO_JOIN		Equ	(1<<14)	; This is auto join if never entered
ESUB_CD_ROM		Equ	(1<<15)	; This is a CD-ROM ARea
ESUB_MEMBS_ONLY		Equ	(1<<16)	; This is a members only area
ESUB_NOSMARTS		Equ	(1<<17)	; No smart paths for this sub-board
ESUB_LOG_ENTRY		Equ	(1<<18)	; Log all entries to the caller log


; Editor Return Flags

ED_SAVE			Equ	1	; Save post
ED_ABORT		Equ	2	; Aborted message


; WhatIs() Flags

WHATIS_USERDATA		Equ	1	; Returns a pointer to the users data
WHATIS_INPUTBUF		Equ	2	; Returns a pointer to Input Buffer
WHATIS_COMMONDATA	Equ	3	; Returns a pointer to Common_Data
WHATIS_DOS		Equ	4	; Private


; Extra Bits

DOWNLOAD_CDROM		Equ	(1<<8)	; This is a CDRom file
