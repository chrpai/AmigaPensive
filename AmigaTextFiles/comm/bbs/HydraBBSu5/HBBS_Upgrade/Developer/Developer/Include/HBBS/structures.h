/*


  to add

  *none*

  to remove

  anything with *R* next to it!

*/

struct ScreenInfoData
{
    struct Screen *Scr;
    V_BIGNUM ScrModeID;
    V_BIGNUM ScrWidth;
    V_BIGNUM ScrHeight;
    V_BIGNUM ScrDepth;
    V_STRING PubScreenName;
    V_BOOL UsePubScreen;
    UWORD *ScrPens;
};


// right, this is Soooo cool, basically a door calls the MenuPrompt() function
// like this:
// DOOR_MenuPrompt("Fruit [O]ranges, [B]ananas, [A]pples",'B')
// where the B means that Banana's will be selected if the user just
// presses return..
//
// then using the data in BBSCols we can change the output of the prompt
// passed to MenuPrompt() before we display it, here's an example of
// what each setting might be set to..


struct BBSColsData
{
  V_STRING MenuTextANSI;            // default: [0;37m    "white"
  V_STRING MenuOpenBracket;         // default: [0;34m[   "blue + ["
  V_STRING MenuCloseBracket;        // default: [0;34m]   "blue + ]"
  V_STRING MenuHighlightANSI;       // default: [0;33m    "yellow"
  V_STRING MenuDefaultOptANSI;      // default: [1;37m    "bold + white"
  V_STRING MenuPromptANSI;          // default: [0;34m:   "blue + :"
};

struct BBSStringsData
{
  V_STRING PausePrompt;       // default: "Press [Return] to continue!"
  V_STRING OutOfTime;         // default: "Sorry you've run out of time"
  V_STRING NoLoginsAllowed;   // default: "No logins are allowed at the moment, sysop has restricted access\r\n"
  V_STRING InactivityTimeout; // default: "\r\n\r\nInactivity Timeout!\r\n\r\n"

  // *C* add more and implement!
};

// using the defaults above with the example above, the string that gets
// printed looks like this:
//
// "[0;37mFruit [0;34m[[0;33mO[0;34m][0;37mranges, [0;34m[[1;37mB[0;34m][0;37mananas, [0;34m[[0;33mA[0;34m][0;37mpples"
//
// as you can see, there is quite alot of scope for cool menu designs!
// and you can have different settings for difference conferences and different nodes too !!


struct BoolNode
{
  struct Node node;
  V_BOOL Boolean;
};

struct ConfAcsData
{
  V_STRING Name;
  struct List *See; // Lists of BoolNode's
  struct List *Access;  // Lists of BoolNode's
};


// when a user joins a conf first copy the correct access settings from
// the list of access settings in BBSGlobal->AcsList (HBBS:Access/Level_XX
// to Node->User.Acs then overwrite this data Using UpdateAcs(dest,new)
// with the settings for the Node->Acs, (NodePath/Access/Level_XX)
// then override these settings with the access settings from
// the conference itself (ConfPath/Access/Level_XX), and finally
// override these settings with the users own access settings.


struct ConfData
  {
    // conference data is a linked list (see BBSGlobal->ConfList)
    struct Node   node;

    // the conference's name is stored in node->ln_Name and loaded from the
    // ConfName in the ConfConfig config file.
    // makes it cool for intuition based utils  (nukers ?)
    // MAXLEN: 25

    V_BIGNUM      ConfNum;    // filled in by Control, ranges from 1 to BBSGlobal->Conferences

    V_BOOL        ConfActive; // if set then you may use the conference
                              // possible use is for cd-rom conferences, you could write
                              // a door to de-activeate a conference if the cd was taken
                              // out (but make sure there are NO users in that conference
                              // when you modify this value.
    V_SMALLNUM    ConfAccess; // access required to enter conference

    // Public : Path to conference (initialised from ConfList and NOT ConfConfig)
    V_STRING      ConfPath;

    V_STRING      HoldFileList;   // path and filename of file that holds the list
                              // of privately uploaded files
    V_STRINGLIST  HoldFiles;  // path(s) to above files

    V_STRING      BadFileList; // same as hold but for bad/corrupt files
    V_STRINGLIST  BadFiles;

    V_STRINGLIST  PartUpload; // path for uploaded files that were not completed

    V_STRINGLIST  FileList;   // file listings
    V_BIGNUM      FileLists;  // contains amount of file lists..

    V_STRINGLIST  Upload;     // paths for uploaded files to go to..
    V_STRINGLIST  Download;   // paths to download files from

    V_SMALLNUM    MaxDIZLines; // max lines for the file description

    V_STRING      MenuPrompt;  // prompt for the menu

    V_BOOL        AutoMailScan; // automatic mail scan ?

    V_STRING      ConfPassWD; // conference password

    V_STRINGLIST  UserAllowed; // list of users allowed, or NULL
                               // if you want to allow every user access to the conf

    V_BOOL        KeepDIZWithFile; // if set it'll copy the FILE_ID.DIZ to the same
                                   // directory as the file itself is stored.
                                   // it's still kept in the archive too tho!
    V_STRINGLIST  NeedDIZ; // path for uploaded files that need file_id.diz's

    V_STRINGLIST  ListDesc;   // file listings
      };


// defines all data needed for the device for the node

struct DeviceData
  {
    V_BOOL SysopNode; // gets set to  TRUE if the Device file does not
                      // exist in the nodes/nodex/ direcotry..
                      // this determines wether we open the serial port

    // following settings are defined in nodex/device

    V_STRING DeviceName;  // as in name of modem rather than amiga .device!
    V_STRING SerialDevice; // name/path of .device file..
    V_BIGNUM SerialUnit;
    V_BIGNUM SerialBaud;  // baud rate
    V_BOOL ModemDebug;   // show inout/output to and from modem commands
    V_BOOL ModemLog;  // log all input/output of modem commands

    V_SMALLNUM   EchoRetries;
    V_SMALLNUM   ReOpenRetries;
    V_BIGNUM     ReOpenDelay;
    V_STRING     LockUpScript;

    V_BIGNUM     MaxCommandWait;
    V_SMALLNUM   CommandRetries;
    V_BIGNUM     DelayBetweenCmds;
    V_BIGNUM     TildeDelay;

    V_STRINGLIST TurnOnEcho;
    V_BIGNUM     TurnOnEchoDelay;

    V_STRINGLIST ModemInit;

    V_BOOL       StrictConnect;
    V_STRINGLIST StrictConnectStr;
    V_STRINGLIST RelaxedConnectStr;

    V_STRINGLIST CommandModeString;
    V_BOOL       DropDTRHangup;
    V_STRING     HangUpString;
    V_STRING     OffHookString;
    V_STRING     Incoming;
    V_STRING     ImmediateAnswer;
    V_BOOL       NullModemCable;   // show inout/output to and from modem commands
    V_BOOL       DumpModem;   // show inout/output to and from modem commands
    V_BOOL       UseCRLF;   // show inout/output to and from modem commands
  };

struct NodeSettingsData   // these settings are always in memory, even when
                          // a node is closed (for stuff like lastcallers utils to examine)
  {
    V_STRING   SysopAccount;
    V_STRING   NodeName;
    V_STRING   NodePhone;
    V_STRING   NodeConnectScript;

    V_BOOL     AskUserPW;

    V_BOOL     UseSysPW;
    V_STRING   SysPW;
    V_STRING   SysPWScript;
    V_STRING   SysPWPrompt;

    V_BOOL     UseNodePW;
    V_STRING   NodePW;
    V_STRING   NodePWScript;
    V_STRING   NodePWPrompt;

    V_STRING   UserNamePrompt;
    V_STRING   UserPWPrompt;

    V_STRING   NodeLogFile;
    V_STRING   ModemLogFile;
    V_STRING   CallersLogFile;
    V_STRING   DoorLogFile;
    V_BOOL     DoorLog;
    V_BOOL     UploadLog;
    V_BOOL     DownloadLog;

    V_BOOL     ChatFlag;
    V_BOOL     Iconified;
    V_BOOL     UseOwnScreen;
    V_BOOL     OpenWatch;

    V_BIGNUM   ScrModeID;
    V_BIGNUM   ScrWidth;
    V_BIGNUM   ScrHeight;
    V_BIGNUM   ScrDepth;

    V_SMALLNUM AccessRequired;

    V_BOOL     UseDevice;
    V_BOOL     AllowNewUsers; // set to true if you want to allow new users to join.

    V_STRING   NodePlayPen;
    V_SMALLNUM MaxDIZLines; // default max diz lines.  it gets copied to N_ND->MaxDizLines

    V_BOOL InactivityDisconnectOverride;
    V_BIGNUM InactivitySeconds;

    V_BOOL     CheckNodeScreens;

    // MUCH more to come...

    // update stuff in source/common/shared.c
  };

// struct for time access data, see BBSGlobal->TimesList for linked list of these...


struct TimeAccessData
  {
    struct Node node;  // ln_Name contains the filename from which the data
                       // was loaded, so that the filename in the users
                       // user.data is matched to ln_Name to find the right one,
                       // rather than searching/loading it everytime a user logs on..

    V_BOOL     Times[7][24];
  };
/*
   times between 1:00am and 12:59pm
        AM          PM
        1         111         11
        212345678901212345678901         a + here means a user can login during that
    Mon +++++++++---------++++++         hour above the +, a - means the user is
 d  Tue +++++++++---------++++++         NOT allowed to login..
 a  Wed +++++++++---------++++++
 y  Thu +++++++++---------++++++         so in this example the user would be able
    Fri +++++++++---------++++++         to login between 12:00am and 8:59am and
    Sat ++++++++++++++++++++++++         between 6:00pm and 11:59pm except on the
    Sun ++++++++++++++++++++++++         weekend where they can login at anytime..
        NIGHT     DAY      NIGHT
                                         cool huh ?
*/

struct AccessData
  {
    V_SMALLNUM AccessLevel;
    V_CHAR Data[MAX_ACCESSSETTINGS];  // currently 4096, see defines.h and access.h
  };

// this is the file format of the userdata.hbbs file... :-) no more hunting
// and hacking to find it out! YEah!!!!

// when memory is allocated for the userdata it should be MEMF_CLEAR'd
// so that when it is written to the end or back to the userdata.hbbs
// file it does not contain any garbage characters that would appear otherwise..

struct UserData
  {
    // note: users DON'T have any form of ID based upon position in the file
    // but they do have ID, which is thier unique number that will always
    // be the same even if the userdata file is tieded.



    V_BIGNUM   UserID;   // When a user joins the system they are given
                         // a number, this number is BBSGlobal->LastUserNum+1
                         // this number is garunteed to be unique
                         // as this number goes up one everytime a new user
                         // is added to the user data file, but never goes down
                         // when users are removed.
                         // The lastusernum itself is stored in
                         // HBBS:System/Data/Private

    // see the flags USER_#? defined in <hbbs/defines.h>

    V_CHAR     Status;   // 'N' for new user      - the user is a new user..
                         // 'V' for validated     - the sysop has validated the user
                         // 'D' for deleted       - the sysop deleted the account
                         // 'I' for inactive      - the account became inactive
                         // 'L' for logins denied - user is not allowed to login.
                         // 'O' for overwritable  - set if you want this account to
                         //                         be erased by new accounts

    V_SMALLNUM Access;   // defines the users access level..

    // all arrays of V_CHAR are +1 for null terminator and should be allocated
    // with a MEMF_CLEAR flag...

    V_CHAR     Handle[LEN_HANDLE+1];          // all obvious definitions..
    V_CHAR     RealName[LEN_REALNAME+1];
    V_CHAR     Group[LEN_GROUP+1];
    V_CHAR     GeoLocation[LEN_GEOLOCATION+1];
    V_CHAR     Country[LEN_COUNTRY+1];
    V_CHAR     PhoneNumber[LEN_PHONENUMBER+1];
    V_CHAR     Password[LEN_PASSWORD+1];    // not encrypted.. as if i did
                                // encrypt it some bugger would write a
                                // password hacker anyway!
    V_CHAR     ComputerType[LEN_COMPUTERTYPE+1];  // note: this is a string, not a number!

    V_CHAR     SentBy[LEN_SENTBY+1]; // sentby string!  New, for Alpha A9

    V_CHAR     ConfAcsDataFile[LEN_CONFACCESSFILE+1];  // filename pointer to data for the access to
                                                       // conferences, e.g. "NewUser"
               // we look in HBBS:System/Data/ConfAcs for the above filename

               // note: this might be blank, if so the confjoiner program must check the access
               // level of the conference the user wants to join.

               // this file is normally only used to disable/enable access to conferences
               // without changing their access levels (which would enable more ACS_ commands)


    V_CHAR     LeechAccDataFile[LEN_LEECHACCESSFILE+1];
               // we look in HBBS:System/Data/LeechAcs for the above filename

    // below are the values the nukers should play with and all the doors
    // should actually use to figure out stuff like weektops/conf tops/charts...

    // these are also used to work out the ratios and wether the user is over the
    // limit or not..

    V_HUGENUM  UploadBytes;
    V_BIGNUM  UploadFiles;
    V_HUGENUM  DownloadBytes;
    V_BIGNUM  DownloadFiles;

    // these numbers below are the ACTUAL ammount of bytes the user has transferred
    // (via file transfer protocols like Zmodem) no doors should EVER change the
    // values in these variables, treat them as is they were readonly..

    // they should be used for programs that work out the amount of bytes that
    // have been uploaded to the system in a year, how many bytes have been nuked
    // etc etc...

    V_HUGENUM  ActualUploadBytes;
    V_BIGNUM  ActualUploadFiles;
    V_HUGENUM  ActualDownloadBytes;
    V_BIGNUM  ActualDownloadFiles;

    // dates for last upload and when the user last called.. great for spotting
    // users who have uploaded loads and then turned into a leech!
    // also so that doors can work out how many days since user last uploaded
    // great for creating lists of lamers..

    // you could write a door that tells the user their account will expire in N
    // days if they do not upload! cooool!

    LONG       LastUploadDate;
    LONG       LastCalledDate;

    // some vars for bulletin/top chart makers! :-) hope this helps!

    V_BIGNUM   BestCPSUp;
    V_BIGNUM   BestCPSDown;
    V_BIGNUM   CallsMade;
    V_BIGNUM   PagesMade;
    V_BIGNUM   MessagesWritten;


    V_SMALLNUM BytesRatio;   // as in N:1...
    V_SMALLNUM FilesRatio;

    V_BIGNUM   LastConf;  // last conf user was in when they logged off..
    V_BIGNUM   PreferredConf;// users preferred conference, if this is not set to 0
                             // then join this conference instead of the conference
                             // the user was last in...
                             // joinconference door must check that this is a valid conf
                             // number and that the user has access to it before
                             // joining it upon login.

    V_SMALLNUM LinesPerScreen; // obvious!

    V_SMALLNUM Protocol;    // number of protocol to use, if set to 0 then
                            // the the bbs will ask the user what
                            // protocol the user wishes to use for EVERY transfer

    V_SMALLNUM Editor;      // same as above except for editing text files...

    V_SMALLNUM ScreenType;  // use for enabling/disabling ansi colour..

    V_CHAR     TimeAccessFile[LEN_TIMEACCESS+1];
                            // number of TimeAccess to use.. (defines when a user
                            // can and can't login..
                            // looks in HBBS:System/Data/TimeAcs for this file..

                            // note: match THIS name to the name in the BBSGlobal->TimeAcsList
                            // nodes's ln_Name field rahter than loading each time as all access
                            // data is loaded at control startup time.

    V_BIGNUM   ExtraTimeLimit;
                            // if this is set then this number will be added to the
                            // amount of time a user is allowed on-line
                            // which is defined for each access level.. *I*mplement

    V_HUGENUM   ExtraBytesLimit;
                            // this number is added to the amount of bytes the user
                            // is allowed to download in one day which is defined
                            // for each access level...

    V_BIGNUM   ExtraCallsLimit;
                            // same as above except for calls a user is allowed to
                            // make in one day
    V_BIGNUM   ExtraChatLimit;
                            // same as above except for chat time user allowed to
                            // have per day

    V_BIGNUM   BytesAllowed;
                            // amount of bytes user is allowed to download in one day.
                            // if set to 0 then the user has unlimited
                            // see TimeAllowed for some rules..
    V_BIGNUM   TimeAllowed;
                            // amount of time user is allowed in one day.
                            // see

    V_BIGNUM   ChatAllowed;
                            // amount of chat time user is allowed in one day.

    V_BIGNUM   CallsAllowed;
                            // amount of calls a user is allowed to make in one day.


    V_BIGNUM   TimeUsed;
                            // total amount of time used today..
                            // (so, to determine when to log off a user get the amount
                            // of time a user is allowed per day from the access settings
                            // for that level, then add the ExtraTimeLimit to that, then
                            // take off the TimeUsed value and if it's <= 0 then log em off!

                            // The BBS Resets this value at 00:00am to 0 when the user is    *I*
                            // online, the frontend must check the last called date with
                            // todays date to determine wether or not to reset this
                            // at logon time..

    V_HUGENUM   BytesUsed;
                            // amount of bytes downloaded today. see timeused also.

    V_BIGNUM   CallsUsed;
                            // amount of calls made today. see timeused also.

    V_BIGNUM   ChatUsed;
                            // amount of chat time used today. see timeused also.

    V_CHAR     UserType;    // currently set to either
                            // USERTYPE_NORMAL or USERTYPE_EXPERT
                            // this is used by the CmdPrompt door to
                            // decide wether or not to display the menu options
                            // availiable to the user.  can also be used by
                            // other programs

    V_BIGNUM   Language;    // Number of LanguageName/Extn to be used. (index starts at 1)

  }; // don't forget to update HBBSCommon.c/HBBS_InitUserData()

// conference flags for reading mail and scanning files are NOT in the user data
// as this is upto the mail/file system to handle...


struct NodeUser
  {
    BOOL Valid;  // if there is no user on-line then this will be FALSE
                 // it is critical that all doors check this before accessing
                 // any more data contained within this sructure.

    // *C* change these to pointers-to-structures for easy updates to the UserData structure in the future..

    struct UserData *NormalData;  // the user.data is loaded here first
    struct UserData *CallData;    // then copied to here. and THIS is the data
                                 // most of your doors/programs should use
                                 // to determine access and stuff as the sysop
                                 // might want to override the access level
                                 // settings for the call..

    struct TimeAccess *TimeAcs;  // only points to a TimeAccessData already loaded.
    struct AccessData Acs;       // current access settings (dependant on
                                 // Conference,Node and normal in that order..
    struct ConfAcsData ConfAcs;  // set-up when user logs in.

    V_BIGNUM FilesUploaded;      // during this logon...
    V_BIGNUM FilesDownloaded;
    V_HUGENUM BytesUploaded;
    V_HUGENUM BytesDownloaded;
    V_BIGNUM PagesMade;
    V_BIGNUM NukedFiles; // the amount of files other uses uploaded that the user has nuked
    V_BIGNUM MsgsRead;
    V_BIGNUM MsgsWritten;

  };

// sent between from a node to the control program

struct StatusMsg
  {
    struct Message message;
    ULONG MsgType; // see nodemsg below..
    V_BIGNUM NodeNum; //see nodemsg below
    V_BIGNUM Status;
  };

struct RequestMsg     // Message sent FROM a node to the control program
  {
    struct Message message;
    ULONG MsgType; // see nodemsg below..
    V_BIGNUM NodeNum; //see nodemsg below
    ULONG Flags;
  };

// used for communicating between a door and a node..

struct DoorIOMsg
  {
    struct Message message;
    ULONG MsgType;
    V_BIGNUM NodeNum;
    ULONG Status;
    UBYTE *Data; // null terminated string or data
    ULONG DataLength; // length of data (above) in bytes
    ULONG Flags;
    ULONG ReturnVal;
    UBYTE *OptionStr; // for sting options such as parameters for doors..
    ULONG Num1;
    ULONG Num2;
  };

// tells the node when a door has started and when one has stopped.

struct DoorActivityMsg
  {
    struct Message message;
    ULONG MsgType;
    V_BIGNUM NodeNum;
    ULONG Status;
  };

// all messages sent to a CONTROL/NODE/DOOR program must contain these variables
// so we know what's happening (and to aid message trace programs)...

struct NodeMsg
  {
    struct Message message;
    ULONG MsgType;
    V_BIGNUM NodeNum; // these three fields are contained in all the message structures that are passed
                      // between the node and the control program, then depending on the TYPE field (see defines.h)
  };                  // they are typecasted onto a different structure and passed to the correct subroutine to deal
                      // with the message, for this reason these 3 fields MUST be present in any other messagetype
                      // for the node program to deal them correctly (system MAY hang if you send a buggered msg...)

struct DoorData
  {
    struct Node node;
    struct MsgPort *DoorPort;
    struct MsgPort *ReplyPort;
    UBYTE DoorPortName[20];
    UBYTE *SystemOptions;
  };

struct TimerData
  {
    struct Node node;
    struct timerequest TR;
  };

struct TimerSetupData
  {
    struct MsgPort *TimerPort;
    struct timerequest *BlankTimerIO;
    struct List *TimerList;
    V_BOOL TimerOpen;

  };



struct NodeData
  {
    // private:
    struct Node node; // for passing to list and for the ctrlwnd list view..

    // public:
    V_BIGNUM Status;
    V_SMALLNUM NodeNum;

    // private:
    V_BOOL AutoStart;

    // private:
    char PortName[15]; // port name for node, used to ask the node program stuff
                       // by doors..

    // private:
    char DoorStartPortName[25];

    // private:
    struct MsgPort *NodePort,
                   *ReplyPort,

                   *ConRPort,  // used by the console window for the node..
                   *ConWPort,

                   *SerPort; // only one port for serial device needed...

    // private:
    struct IOStdReq *ConRead,  // io requests for console window
                    *ConWrite;

    // private:
    struct IOExtSer *SerRead,  // io requests for serial port
                    *SerWrite;

    // private:
    struct TimerSetupData *NodeTimer; // timer io data for the node..

    // private:
    struct Screen *ConScr;  // screen for watching a user..
                            // note: this might be set to the same screen
                            // as CtrlScr.. or even workbench screen :-)
    // private:
    struct Window *ConWin;  // window for above! :-)
    // private:
    struct Window *NodeWnd;
    struct Window *InfoWnd;
    struct Window *SettingsWnd;

    V_BIGNUM ConX,ConY,ConW,ConH,NodeX,NodeY;

    // public:
    V_BOOL SettingsOpen;
    V_BOOL InformationOpen;

    // private:
    char WindowTitle[90];

    // public: will be set to true if console window for node is open!
    // so if it's not you can call HBBS_OpenWatchWindow()
    V_BOOL ConOK;


    // private:
    V_BOOL SerOK;


    // private: used by init/cleanup routines to determine wether to call CloseDevice()...
    V_BOOL SerOPEN;

    // private:
    char *ConBuffer;    // console data is read in here (most of the time!)
    ULONG ConBufferLen; // actual memory allocated to the console buffer
    ULONG ConBytes;     // bytes of data waiting in ConBuffer

    // private:
    char *SerBuffer;    // same as above but for the serial port!
    ULONG SerBufferLen;
    ULONG SerBytes;

    // private:
    char *IBuffer;      // InputBuffer is set to conbuffer or SerBuffer
                        // according to where input came from
    // private:
    ULONG IBytes;       // set to the amount of bytes awaiting to be parsed
                        // in the Input buffer

    // private:
    V_BOOL SerWaiting;  // if these flags are set then it means that an IORequest
    V_BOOL ConWaiting;  // has been sent, this must be aborted and waitio()'d
                        // before closing, and must be checked before
                        // you CheckIO() the request

    // private: used for csi's!
    char char1;


    // private: used for csi's!
    char csistring[256];


    // private:
    ULONG TimerSig,       // signals for input events..
          SerSig,
          InfoWinSig,
          SettingsWinSig,
          ConSig,
          ConWinSig,
          PortSig,
          WinSig;

    // public:  after you call DOOR_GetLine() this will contain the string
    // that was typed in..
    V_STRING CurrentLine;

    // public:  after you call DOOR_GetLine() with a GL_LINEWRAG flag this may
    // contain some data if the user was in the middle or writing a word
    // see the LineEditor for an example of it's usage.
    V_STRING CurrentLineWrap;

    // public: type of logon, (local, remote or none!)
    V_BIGNUM LoginType; // defaults to LOGIN_NONE


    // private:
    ULONG NodeFlags;   // some flags..  NFLG_#?


    // private:
    V_BOOL RequestShutdown;


    // public: actual path of the NODEx directory.
    UBYTE *NodeLocation; // path setting, should be initialized by CONTROL
                         // MUST and WILL always have a trailing : or / ....


    // public: options from the device file

    struct DeviceData NodeDevice; // *C* change to pointer!


    // public: the following options are defines in the nodeglobal or nodelocal files.

    struct NodeSettingsData NodeSettings;


    // public: info about the user who's online...
    // check N_ND->User.Valid before accessing any more data! in the NodeUser structure

    struct NodeUser *User;


    // private: Amount of doors the node program currently has spawned.
    ULONG DoorsRunning;


    // public: pointer to doordata structure of the active door, most if the time
    // this will point to some door of somekind.

    struct DoorData *ActiveDoor; // WARNING: NULL if door is not running,
                                 // for the name of door look at N_ND->ActiveDoor->node.ln_Name;

    // private: String returned by last active door.
    UBYTE  DoorReturn[LEN_MAXDOORRETURN];

    // private: List of doors

    struct List *DoorList; // as a door has the ability to spawn another door using GoDoor()
                           // we need to keep track of the loaded doors and the order in
                           // which we ran them, so the door at the START of the list
                           // (the lh_Head) if the active door (which was loaded last)
                           // the one at the end of the list if the one we loaded first.
                           // so we add new doors to the START of the list..
                           // (quicker to parse than having the active door at the end of the list..)

    // public: pointer to conference that user is in, WARNING: might be NULL if
    // user has not yet joined one..

    struct ConfData *CurrentConf;

    // private:
    // when a user logs into a node the access settings for that users level are
    // loaded here, DO NOT use these setting to check wether a user can access
    // things tho, use N_ND->User.Acs[X]

    // public:
    UBYTE ConnectBaud[MAX_CPSBAUD_LEN+1];

    // public: used by WHO doors..
    UBYTE Action[MAX_ACTION_LEN+1];

    V_BOOL TransferringFile;

    // public readable and door setable:  if a user is ONLINE (Via Local login or
    // remote login then this will be set to OS_ONLINE.  Otherwise it will be set
    // to OS_OFFLINE.  This variable is used by the node program to determine wether
    // or not to continue running the next door in the sequnence or to reset the
    // node...
    // see TransferReceiveName below

    V_BOOL OnlineStatus;  // OS_ONLINE || OS_OFFLINE

    V_STRINGLIST Last_Callers; // Implemented
    V_STRINGLIST Last_Uploads; // Implemented
    V_STRINGLIST Last_Downloads;
    V_STRINGLIST Last_Pagers;
    V_STRINGLIST Last_PWFails;
    V_STRINGLIST Last_Carrier;

    V_BIGNUM Max_Last_Callers,
             Max_Last_Uploads,
             Max_Last_Downloads,
             Max_Last_Pagers,
             Max_Last_PWFails,
             Max_Last_Carrier;

    V_BIGNUM Current_Last_Callers,
             Current_Last_Uploads,
             Current_Last_Downloads,
             Current_Last_Pagers,
             Current_Last_PWFails,
             Current_Last_Carrier;

    struct List *TaggedFileList; // list of struct TaggedFile's that is saved and
                                 // reloaded, by the node program, when a user
                                 // logs off and logs on respectivly
    V_BIGNUM TaggedFiles; // number of tagged files (check when logging out ?)
    V_SMALLNUM MaxDIZLines; // when a user is not in a conference then this is gained
                            // from N_ND->NodeSettings.MaxDIZLines, otherwise it is
                            // copied from N_ND->CurrentConf->MaxDIZLines
    V_BOOL DoorContinue;

    V_BOOL AllowLogins;

    V_STRING CharsAllowed; // fill this with the characters you want the user to
                           // be able to use when using DOOR_GetLine()
                           // and specify GL_USECHARS as one of the flags
                           // and when a user presses a key that's not in
                           // this string a BEEP will be sent to their
                           // system and the key will not be added to the currentline
                           // (sort of like an edit hook, but not!)

    ULONG CallsToday; // saved in file: HBBS:System/Data/Misc.CFG as Node_X_Calls=

    struct List *OLMList;
    V_BIGNUM OLMCount;     // also see NFLG_OLMSWAITING and "struct OLMNode"

    // private:
    struct MsgPort *OLMPort;  // send a message to this port to make the bbs check
                              // for OLM's!

    char Actions[LEN_ACTIONS+1]; // +1 for null terminator..
                               // array of actions, check out the ACT_#? defines
                               // in defines.h, basically it it set to a string
                               // full of spaces upon node initialisation
                               // and then actions are set if users run particular
                               // types of door, this is SOOOOO handy for writing
                               // lastcallers door,  this information is also
                               // written to the callers log when a user logs off.
    struct BBSColsData *BBSCols;
                               // for setting colours of stuff like menu's etc..

    UBYTE LastCalledDate[LEN_DATESTR+1];
    UBYTE LastCalledTime[LEN_TIMESTR+1];

    V_BOOL DoorLogOverride;
    V_BOOL RunningAwait;

    struct BBSStringsData *BBSStrings;

    V_BOOL ReservedNode;
    char ReservedHandle[LEN_HANDLE+1];


    UBYTE ConWinTitle[160]; // not gonna be bigger!
    UBYTE ConScreenName[30]; // as in, PubScreenName :-)

    struct TextFont *HBBSFont;
    struct TextAttr *HBBSTextAttr;
    struct ColorSpec *ScrCols;
    UWORD *DriPens;

    // public
    UBYTE LastCPS[MAX_CPSBAUD_LEN+1];
                   // if the user is transferring a file (i.e. N_ND->TransferringFile = TRUE)
                   // then the current CPS rate is held here transfer door codes must
                   // call DOOR_UpdateNodeStatus(UPD_CPSBAUD) before, during and after a transfer
                   // so do not write strings longer than MAX_CPSBAUD_LEN-1 to it.

    UBYTE TransferReceiveName[LEN_XFERFILENAME+1]; // stores the name of a file if
       // the user is uploading, see TransferringFile above.
       // this is used to determine wether or not to skip a file if a user on another node
       // wants to upload the same file
  };

struct TaggedFile
  {
    struct Node node;  // ln_Name is the name of the file that's tagged
    V_BIGNUM ConferenceNum; // the number of the conference that the file is in..
    V_HUGENUM FileSize; // size of file (must be filled in or set to 0, this number
                       // is used to determine if the user has enough creds to d/l and
                       // will be added to thier bytes uploaded/downloaded total..
    V_BOOL WarezFile; // if TRUE then bytes will be added to the ActualDownload#?
                      // and Download#?, if false then bytes will only be added
                      // to ActualDownload#?
                      // (i.e. user does not loose creds if a non warez file..)
    ULONG Flags; //see defines.h for the flag names.
  };

// config files are read into memory and line by line are loaded into a linked
// list of items and parameters, then to get a config option all we have to do
// is search the linked list of items for a matching item name and return the params
// in a desired format,  this works out much quicker than loading the config into mem
// and parsing it everytime you want an option.. (which is how i did it to start with.
// it's only when you try running it on a (lame) a500 that you realise how slow it
// is to read in about 50-60 options when you start a node using the older method!

struct CfgItemData
  {
    struct Node node; // ln_name contains item name
    UBYTE *Params;    // params contains the STRING of the parameter..
  };

struct CfgFileData
  {
    struct List *ItemsList; // list of CfgItemData nodes
    char *filename;
  };

// because of the way HBBS data is stored in memory via linked varaibles *ANY*
// program can access ALL DATA of the bbs and all of it's nodes at any time
// just by getting a pointer to the following structure..
// better than /X huh ? :-)

struct ProtocolNode
  {
    struct Node node;       // ln_Name is Protocol Name
    V_BIGNUM protocoltype;  // PTYPE_BIDIRECTIONAL | PTYPE_UNIDIRECTIONAL
    char *modulename;       // Name of door module to run
    char *moduleopts;       // door options supplied to protocol module door
    V_BOOL allow_ul;        // so you can specify a protocol is only availiable for
    V_BOOL allow_dl;        // up load or download or both!
    V_BOOL allow_batch;     // allow the transferring of more than one file at a time

  };

struct BBSGlobalData
  {

    struct NodeData *NodeGlobalData;
    struct List *NodeList;
    struct MsgPort *CtrlMainPort,*CtrlMainReplyPort;
    struct List *TimesList;
    struct ScreenInfoData *ScreenInfo; // *!* Changed to a pointer for V1

    // settings for bbs, all sturctures are sub structures of this..

    // public: linked list of all Conference Data (struct ConfData *)
    struct List *ConfList; // list of conferences.

    // public: amount of conferences (active or not) (initialised by Control)
    V_BIGNUM Conferences;


    // public: linked list of TimeAcs's (struct TimeAccessData *)
    struct List *TimeAcsList;

    // public: amount of TimeAcs's loaded
    V_BIGNUM TimeAccesses;

    // public: list of access levels
    V_STRINGLIST AcsLevelList;        // ln_Name contains a string of the access level number
    V_STRINGLIST AcsLevelNames;       // ln_Name contains a string of the access level name
    V_STRINGLIST AcsLevelConfAcsDef;  // ln_Name contains a string of the name of the default
                                      // conference access level settings file
    // public: amount of access levels
    V_BIGNUM AcsLevels;

    // public readonly: amount of users in the user database.
    V_BIGNUM TotalUsers;

    V_STRING ErrorLogFile;

    V_BIGNUM BBSNodes;
    V_STRING BBSName;
    V_BIGNUM BBSSerial;
    V_STRING BBSLocation;
    V_STRING BBSCountry;
    V_STRINGLIST BBSGroup;

    V_STRING SysopAccount; // the name of the sysop...

    V_STRINGLIST Drive;

    V_STRING BBSDrive;
    V_BIGNUM MinFreeSpace;
    V_STRING NoFreeSpaceScript;
    V_STRINGLIST ButtonName;
    V_STRINGLIST ButtonCMD;

    V_BOOL HideScreen;

    V_STRING UserDataFileName;

//    V_SMALLNUM NewUserAccessLevel;
    V_SMALLNUM MaxUsernameAttempts;
    V_SMALLNUM MaxPasswordAttempts;
    V_SMALLNUM MinPasswordLength;

//    V_BIGNUM NewUserConf;

    // readonly:  userid of the last user added to the system.
    V_BIGNUM LastUserNum;

    V_STRINGLIST LanguageName;
    V_STRINGLIST LanguageExtn;
    V_BIGNUM Languages;

    struct List *ProtocolList;  // List of protocol information
    int Protocols;              // Amount of protocols loaded

    V_STRING EditorCMD; // program to be used for editing text files!

    UBYTE LastCalledDate[LEN_DATESTR+1];
    UBYTE LastCalledTime[LEN_TIMESTR+1];

    V_BIGNUM CallsToday; // saved in file: HBBS:System/Data/Calls.CFG
                         // also saves Today=[datestr] in file
    V_BIGNUM CopyBufferSize;

    V_BIGNUM CallsEver; // total amount of calls bbs has received!

    V_BIGNUM Door_Timeout; // System waits this long for doors to load
                           // and activate, setting in in BBSGlobal

    V_BOOL LogConfig;        // if enabled all HBBS_*Config()'s are logged to
    V_STRING ConfigLogFile;  // the file specified here!

    V_BIGNUM CtrlX; // stores the window positions of the control gui
    V_BIGNUM CtrlY;

    struct Hook backfillhook; // use this with WA_Backfill when you open a window on the ctrl screen
    													// ONLY if ScreenInfo->UsePubScreen is FALSE

    V_BOOL UserDataLocked; // public, readonly
};


// an OLM door should check for the AccessLevel ACS_ALLOWOLM before a user is allowed
// to send an OLM..  this is not part of HBBS_AddOLM() as you might want other
// doors to send olm's (e.g. LogonAnnounce Doors..)

// (olm stands for OnLineMessage..)

struct OLMNode
{
  struct Node node;  // -> ln_Name contains the message, null terminated string, it *CAN* have \n's in it..
                     // -> use node.ln_Pri to set an OLM's priority!!!
                     // the higher the pri the nearer the top of the list

  UBYTE Handle[LEN_HANDLE+1]; // handle of the user who sent the olm
                              // handle does not have to be valid,
                              // but if you want to reply to an OLM
                              // then you MUST check to see if it's valid...
                              // reason: announce doors don't have handles :=)
  UBYTE Time[LEN_TIMESTR+1];  // time olm was sent is added here..
  UBYTE Date[LEN_DATESTR+1];  // date olm was sent is added here (in case it was sent at 11:59pm :-))

  UBYTE FromPRG[LEN_HANDLE+1]; // the name of the program that sent the door, note
                               // the length constraints.
  V_SMALLNUM FromNode;        // 0, for external program, or a N_ND->NodeNum+1


};
