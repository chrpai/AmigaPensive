// defines for askmsg->flags for CONTROL program
#define ASK_BBSGLOBAL 1

// defines for askmsg->flags for NODE program
#define ASK_STATUS 20

// error messages for HBBS_LogError()

#define ERR_NO_ERROR 0
#define ERR_ERROR_OPENING 1
#define ERR_ERROR_READING 2
#define ERR_ERROR_WRITING 3
#define ERR_GENERAL 4
#define ERR_NODESETTINGS 5
#define ERR_DOORTIMEOUT 6
#define ERR_INFO 7

// if ya change any of the errors here don't forget to update the array
// of error msg strings in errors.h

#define TYPE_NONE 0
#define TYPE_FATAL 1
#define TYPE_CRITICAL 2
#define TYPE_IMPORTANT 3
#define TYPE_WARNING 4
#define TYPE_DONTCARE 5
#define TYPE_MEMORY 6 // a bit different to the above types...
#define TYPE_INFO 7

#define BIG_STR 1024 // ppplenty long enough.. and just to be on the (very) safe
                     // side of long sprintf's :-)


// if you update/change/add to this then update HBBS_GetSetting() in HBBSCommon.c with changes
#define VTYPE_TIME 1
#define VTYPE_SMALLNUM 2
#define VTYPE_BIGNUM 3
#define VTYPE_STRING 4
#define VTYPE_BOOL 5
#define VTYPE_STRINGLIST 6
#define VTYPE_FLAGS 7
#define VTYPE_PATH 8
#define VTYPE_PATHLIST 9
#define VTYPE_WORD 10
#define VTYPE_DATE 11
#define VTYPE_HUGENUM 12

#define OPT_SINGLE 0
#define OPT_MULTI 1

// possible node status's

#define STAT_CLOSED 0
#define STAT_LOADING 1
#define STAT_INITIALIZING 2
#define STAT_READY 3
#define STAT_ONLINE 4
#define STAT_CLOSING 5

// gets sent to Node Program when you want a node to shutdown..

#define STAT_REQUESTCLOSE 6

// send to node program if you want it to open or close it's window (NOT UserScreen tho!, just the node control window)
#define STAT_OPENWINDOW 7
#define STAT_CLOSEWINDOW 8
// send to node program if you want it to open or close it's UserScreen
#define STAT_OPENSCREEN 9
#define STAT_CLOSESCREEN 10

// login types..

#define LOGIN_NONE   0
#define LOGIN_REMOTE 1
#define LOGIN_LOCAL  2

// possible flags for displaying information in the ctrl window's status bar!

#define UPD_NODESTATUS 1

// define some string lengths

#define LEN_NODESTATNAME  10 // for use by Control, when allocation memory for node names for the control window
#define LEN_CURRENTLINE   1024  // should be PLENTY..
#define LEN_MAXDOORRETURN 255
#define LEN_DATESTR       11 // DD-MMM-YYYY
#define LEN_TIMESTR       8 // HH:MM:SS
#define LEN_XFERFILENAME  255 // used in XPR, HYDRA and the node structure.

// defines for request message types

#define REQ_UPDATEINFO 1 // tells control program to update the currently displayed
                         // list above the Information gadget (callers/dls/ul's....)


// message types

//#define mtype_ASK 1
#define mtype_STATUS 2
#define mtype_DOORACTIVITY 3
#define mtype_DOORIO 4
#define mtype_REQUEST 5
#define mtype_CHECK 6

// door status message types..

#define DMSG_DOORSTARTED 1
#define DMSG_DOORFINISHED 2

// door io message types

#define DOORIO_WRITECONSTR          1
#define DOORIO_WRITECONDATA         2
#define DOORIO_WRITESERSTR          3
#define DOORIO_WRITESERDATA         4
#define DOORIO_WRITESTR             5
#define DOORIO_GETLINE              6
#define DOORIO_SYSTEMDOOR           7
#define DOORIO_USERDOOR             8
#define DOORIO_HANGUP               9
#define DOORIO_DISPLAYSCREEN        10
#define DOORIO_DISPLAYSPECIALSCREEN 11
#define DOORIO_PAUSEPROMPT          12
#define DOORIO_ADDLASTUPLOAD        13
#define DOORIO_CONTINUEPROMPT       14
#define DOORIO_MENUPROMPT           15
#define DOORIO_CHECKRAW             16
#define DOORIO_CHANGECONSOLEMODE    17
#define DOORIO_ADDLASTDOWNLOAD      18
#define DOORIO_ACONTINUEPROMPT      19
#define DOORIO_CHECKCSI             20
#define DOORIO_ADDLASTPAGER         21
#define DOORIO_ADDLASTPWFAIL        22
#define DOORIO_ADDLASTCARRIER       23
#define DOORIO_DISPLAYBULLETIN      24

// flags for HBBS_DosCommand() (to be logical OR'ed together)

#define RDC_NONE          0
#define RDC_ASYNC         1
#define RDC_DEFAULTOUTPUT 2

// DOOR_GetLine() flags, see HBBSNode.library autodocs for more info.

#define GL_NONE         (1<<0)  // 0     default
#define GL_HISTORY      (1<<1)  // 1     if set history is enabled (up/down cursor keys)
#define GL_LINEWRAP     (1<<2)  // 2     if maxlen is reached then data back to the last ' ' character is copied to N_ND->CurrentLineWrap and returns immediately
#define GL_IMMEDIATE    (1<<3)  // 4     returns when maxlen is reached
#define GL_NORETURN     (1<<4)  // 8     does not print a cr+lf when a) reached maxlen or b) timeout or c) return press
#define GL_EDIT         (1<<5)  // 16    enables cursor left/right and backspace/delete
#define GL_DISPLAY      (1<<6)  // 32    display chars (don't specify for "Press [return] to contine" type prompts)
#define GL_SYSOP        (1<<7)  // 64    does not write to, or read from, the serial port
#define GL_USECHARS     (1<<8)  // 128   see N_ND->CharsAllowed
#define GL_NOBEEP       (1<<9)  // 256   use with GL_USECHARS if you don't want a beep when the presses a key not in N_ND->CharsAllowed
#define GL_NOOLM        (1<<10) // 512   (No OLM) if set olm's will not inturrpt the user
#define GL_NODISTURB    (1<<11) // 1024  Lets you use things like "Enter ID: [    ]" with out the ] moving
#define GL_CVTUPPER     (1<<12) // 2048  converts input chars to uppercase as the keys are pressed!
#define GL_NOINACTIVITY (1<<13) // 4096  Disables Inactivity checking (primarily for use with awaitconnect door)
#define GL_COUNTDOWN    (1<<14) // 8192  Displays a countdown if used with a timeout value!

// these are the result codes for DOOR_GetLine(), see the DOOR_GetLine() docs.

#define IN_NOTHING 0
#define IN_GOTLINE 1
#define IN_LOSSCARRIER 2
#define IN_LOGIN 3
#define IN_SHUTDOWN 4
#define IN_IMMEDIATE 5
#define IN_TERMINAL 6
#define IN_TIMEOUT 7
#define IN_ENDCHAT 8
#define IN_DISPLAYAWAIT 9
#define IN_DIALOUT 10
#define IN_PAGEANSWERED 11

// pass this to LoadNodeDataSettings() if you want the hbbs:nodes/nodeglobal file
#define NODE_GLOBAL 200000


// some defines used by Node and HBBSNode.library
#define IN_CONSOLE 1
#define IN_SERIAL 2

#define GET_ANYWHERE 0
#define GET_SERIAL 1
#define GET_CONSOLE 2

// note: these may be reallocated by the program to a different length,
// so check NodeData->Con/SerBufferLen for actual current length!
#define DEF_CONBUFLEN 2048  // default lenghts used to allocate NodeData->ConBuffer
#define DEF_SERBUFLEN 2048  // default lenghts used to allocate NodeData->SerBuffer



// these are used throughout the HBBSNode library and the node program.

#define DEF_WINSIG         (1L << NodeWnd->UserPort->mp_SigBit)
#define DEF_CONWINSIG      (1L << N_ND->ConWin->UserPort->mp_SigBit)
#define DEF_INFOWINSIG     (1L << InfoWin->UserPort->mp_SigBit)
#define DEF_SETTINGSWINSIG (1L << SettingsWin->UserPort->mp_SigBit)
#define DEF_CONSIG         (1L << N_ND->ConRPort->mp_SigBit)
#define DEF_SERSIG         (1L << N_ND->SerPort->mp_SigBit)
#define DEF_PORTSIG        (1L << N_ND->NodePort->mp_SigBit)
#define DEF_TIMERSIG       (1L << N_ND->NodeTimer->TimerPort->mp_SigBit)

// the lengths of the strings in the user data file..
// update user editor's GUI string max chars values if you modify these..

#define LEN_HANDLE          25
#define LEN_REALNAME        30
#define LEN_GROUP           30
#define LEN_GEOLOCATION     20
#define LEN_COUNTRY         20
#define LEN_PHONENUMBER     20
#define LEN_PASSWORD        20
#define LEN_COMPUTERTYPE    40
#define LEN_SENTBY          48 // *C* needs to be 45
#define LEN_TIMEACCESS      32 // same as max AmigaDOS FFS filename length!
#define LEN_CONFACCESSFILE  32
#define LEN_LEECHACCESSFILE 32
#define MAX_ACCESSSETTINGS        4096       // 4k is not unreasonably large..
                                             // nor to small..  think about it,
                                             // are we really going to have
                                             // 4096 access flags ??

// tells node what to update.. *C* change to flags, so that one or more can be specified!

#define UPD_NAME     1 // (1<<1) *C*
#define UPD_GROUP    2 // (1<<2) *C*
#define UPD_ACTION   3 // (1<<3) *C*
#define UPD_CPSBAUD  4 // (1<<4) *C*

// string lengths for action information for node window

#define MAX_ACTION_LEN 29
#define MAX_CPSBAUD_LEN 8
#define MAX_GROUP_LEN 20
#define MAX_NAME_LEN 20

#define TIMEOUT_DOORLOAD 10 // ten seconds..


#define OS_ONLINE TRUE
#define OS_OFFLINE FALSE

// flags for UserData->Status

#define USER_NEW          'N'
#define USER_VALIDATED    'V'
#define USER_DELETED      'D' // may be reinstated
#define USER_INACTIVE     'I' // *N* users may login to inactive accounts at the moment,
                              // this setting just helps sysops find inactive accounts
#define USER_LOGINSDENIED 'L'
#define USER_OVERWRITABLE 'O' // all related mail stuff, files, stats should be gone if this is set..


// screen names,  use these when calling DOOR_DisplaySpecialScreen()

#define SSCREEN_PRIVATESYSTEM "PRIVATESYSTEM"
#define SSCREEN_PRIVATENODE "PRIVATENODE"
#define SSCREEN_BBSTITLE "BBSTITLE"
#define SSCREEN_NONEWAT_ALL "NONEWAT_ALL"
#define SSCREEN_NONEWAT_THISTIME "NONEWAT_THISTIME"
#define SSCREEN_GUESTLOGIN "GUESTLOGIN"
#define SSCREEN_JOINED "JOINED"
#define SSCREEN_LOGINLOGO "LOGINLOGO"
#define SSCREEN_BULLETIN "BULLETIN"
#define SSCREEN_CONFBULLETIN "CONFBULLETIN"

// user level types..

#define USERTYPE_NORMAL 'N'
#define USERTYPE_EXPERT 'X'

// protocol directional types

#define PTYPE_UNIDIRECTIONAL 1
#define PTYPE_BIDIRECTIONAL 2

// node flags             bits

#define NFLG_NONE         0
#define NFLG_BLOCKSERIAL  1
#define NFLG_PAGED        2
#define NFLG_CTRLC        4
#define NFLG_OLMSWAITING  8
#define NFLG_HANDLINGOLM  16

// flags for DOOR_ContinuePrompt()

#define DCP_NONE          (1<<0) // 0  accept internal defaults (subject to change at my descression)
#define DCP_DEFAULT_NONE  (1<<0) // 0  accept internal defaults (subject to change at my descression)
#define DCP_DEFAULT_YES   (1<<1) // 1  set the default to YES
#define DCP_DEFAULT_NO    (1<<2) // 2  set the default to NO
#define DCP_ADDYN         (1<<3) // 4  add's [Y/n] or [y/N] accordingly.
#define DCP_COUNTDOWN     (1<<4) // 8  adds a countdown timer
#define DCP_CLEAR         (1<<5) // 16 removes the prompt line after continuing.
#define DCP_NODISPLAY     (1<<6) // 32 does not display Y or N when the user presses a key.
#define DCP_COLOURIZE     (1<<7) // 64 passes the prompt string through MenuPrompt before displaying it

// legacy #define's for early code using ContinuePrompt()
#define DEFAULT_NONE DCP_DEFAULT_NONE
#define DEFAULT_YES DCP_DEFAULT_YES
#define DEFAULT_NO DCP_DEFAULT_NO

// used in N_ND->Actions, to set an access flag just use the follow:
//
// N_ND->Actions[ACTN_<action>]=ACTC_<action>;
//
// where <action> is one of the names below...
// if you add actions you must recomile the node program!

#define ACTC_NONE                ' '   // all ACTC's default to this

#define ACTN_HACK                0
#define ACTC_HACK                'H'   // user failed to enter a correct password
#define ACTN_CARRIERLOST         1
#define ACTC_CARRIERLOST         'L'   // user lost carrier!
#define ACTN_FILESCAN            2
#define ACTC_FILESCAN            'S'   // user scanned one or more filelists (FR,N,Z or F)
#define ACTN_WROTEMAIL           3
#define ACTC_WROTEMAIL           'W'   // user wrote some mail
#define ACTN_READMAIL            4
#define ACTC_READMAIL            'R'   // user read some mail
#define ACTN_PAGED               5
#define ACTC_PAGED               'P'   // user used the 'O' door to page the sysop
#define ACTN_UPLOAD              6
#define ACTC_UPLOAD              'U'   // user uploaded files
#define ACTN_FAILEDUPLOAD        7
#define ACTC_FAILEDUPLOAD        'u'   // user failed an upload transfer
#define ACTN_DOWNLOAD            8
#define ACTC_DOWNLOAD            'D'   // user downloaded a file
#define ACTN_FAILEDDOWNLOAD      9
#define ACTC_FAILEDDOWNLOAD      'd'   // user failed to dow3nload 1 or more files
#define ACTN_CHANGEDSETTINGS     10
#define ACTC_CHANGEDSETTINGS     'C'   // user uses a 'W' type door
#define ACTN_CHATTEDTOSYSOP      11
#define ACTC_CHATTEDTOSYSOP      '1'   // sysop chatted to the user!  (via SysopChat door)
#define ACTN_NUKEDFILES          12
#define ACTC_NUKEDFILES          'X'   // user used the nuker!
#define ACTN_NEWUSER             13
#define ACTC_NEWUSER             'N'   // user is a new user
#define ACTN_USEDSHELL           14
#define ACTC_USEDSHELL           '!'   // user used a bbs shell to access the bbs computer directly
#define ACTN_INACTIVITYTIMEOUT   15
#define ACTC_INACTIVITYTIMEOUT   'I'   // user used a bbs shell to access the bbs computer directly
#define ACTN_KICKEDBYSYSOP       16
#define ACTC_KICKEDBYSYSOP       'K'   // user was kicked off-line by the sysop

#define LAST_ACTION              17    // must be last ACTN_blah + 1
#define LEN_ACTIONS              150   // total number of actions, LEN_ACTIONS+1 is allocated N_ND->Actions[LEN_ACTIONS] is always 0 (null terminated string..)

// flags for LoadConfig()     bits

#define LCFG_NONE             0
#define LCFG_NOSTRIPCOMMENTS  1
#define LCFG_NOSTRIPSPACES    2

// flags for CheckRaw()       bits

#define CR_NONE               0
#define CR_NOSERIAL           1
#define CR_NOCONSOLE          2
#define CR_CHECKFUNC          4

// WARNING: Never wait on CheckRaw() using only CR_NOSERIAL when the login type
//          is LOGIN_LOCAL otherwise you'll end up in a permanent loop.

#define LEN_DIZWIDTH 45 // used to be 48 chars and not defined anywhere...


// for use with HBBSNode.library/CheckCSI()
#define KEY_NONE 0
#define KEY_ESCAPE 1
#define KEY_CSI 2

// for use with DisplaySpecialScreen()

#define DSS_NONE     0  // do nothing special
#define DSS_BULLETIN 1  // searches Screens/Bulletins instead of Screens/Special

// for use with HBBS_LoadUser()

#define LOADUSER_NONE             0
#define LOADUSER_SKIPOVERWRITABLE 1
#define LOADUSER_SKIPID           2

// returned by HBBS_OkToUpload()

#define SKIPF_NONE            0
#define SKIPF_ONANOTHERNODE   1
#define SKIPF_INADLPATH       2
#define SKIPF_INVALIDORNONAME 3

// for use with the tagged file lists.

#define TF_NONE          0
#define TF_FREEDOWNLOAD  1
