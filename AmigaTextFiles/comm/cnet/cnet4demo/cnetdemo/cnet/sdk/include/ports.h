
#ifndef CNET_PORTS
   #define CNET_PORTS

   #ifndef CNETPORT
      #define CNETPORT "cnetport"
   #endif

#define MAILTASKPORT "cnmail_serv"
#define FILETASKPORT "cnfile_serv"
#define YANKTASKPORT "cnyank_serv"
#define NEWSTASKPORT "cnnews_serv"
#define USEREDPORT   "cnusered_port"
#define MIMEEDPORT   "mimeed_port"


#define TASK_REPOSICON_MSG      0x01 // CMessage.command sent to #?-task to tell them to reposition icon
#define CONTROL_REPOSICON_MSG   0x06 // CMessage.command sent to Control to tell it to reposition icon


#include "ranges.h"
#include "users.h"
#include "doors.h"
#include "mail.h"
#include "lists.h"
#include "links.h"
#include "telnet.h"

// 9/11/95 Length = 140

struct CNetPort {
   char   UserName[32];
   char   From[32];
   char   Area[32];
   char   Action[32];

   ULONG   Baud;
   ULONG   Logon;

   USHORT   Port;
   USHORT   Access;
};


#define PORTRIGHT_HIDDEN   0x0001
#define PORTRIGHT_MUFFLED   0x0002
#define PORTRIGHT_CHATENABLE   0x0004
#define PORTRIGHT_NOTHIDDEN   0x0008
#define PORTRIGHT_NOTMUFFLED   0x0010
#define PORTRIGHT_CHATDISABLE   0x0020

struct SerPort {
   long   unit;
   long   flags;

   short   idlebaud;
   short   escape;
   short   answerpause;
   short   seconds;

   char   name[21];
   char   init1[50];
   char   ppass[11];
   char   init2[50];

   ULONG   TrapHours;
   ULONG   MailHours;

   UBYTE   extra[3];

   char   hangup[21];
   char   dialout[21];
   char   answer[21];
   char   offhook[21];
   char   terminal[50];
   char   callerid[11];
   char   ring[9];
   char   connect[11];

   char   termlink[21];

   UBYTE   other[2];

   UBYTE   null;      /* local terminal, no clear line */
};


// revised 22-Jan-97!
struct SerPort4 {
   long   unit;
   long   flags;

   short   idlebaud;
   short   escape;
   short   answerpause;
   short   seconds;

   char   name[40];
   char   init1[50];
   char   ppass[11];
   char   init2[50];

   ULONG   TrapHours;
   ULONG   MailHours;

   char   hangup[21];
   char   dialout[21];
   char   answer[21];
   char   offhook[21];
   char   terminal[50];
   char   callerid[11];
   char   ring[9];
   char   connect[11];

   char   termlink[21];
   UBYTE   null;        // local terminal, no clear line

   char IdleWho[21];    // 18-Feb-97 - text displayed in the "Handle/Name" area of who for idle port(s)

   long   portflags;    // 23-Apr-97
                        // bits: &1 = show port on WHO display.  if bit not set, do not show port on WHO
                        //       (other bits currently unused)

   BYTE expansion[103];
};

struct PortData {                   // each opened ports global vars
   struct   IsDate   Today;         // 0 what time is it right now?
   struct   IsDate   VConnectDate;  // 6 Virtual connect...eg 12am

   short   id,                      // 12 current user1 account #
      InPort,                       // 14 which port are we in?? 
      Age,                          // 16 how old am I ?
      CurrentCPS,                   // 18 CPS rate.  240 == 2400 baud
      OnType,                       // 20 type of logon 
      OffType,                      // 22
      CallMessages,                 // 24
      CallFeedbacks,                // 26
      TimeLeft,                     // 28 TENTHS of minutes left
      TimeIdle,                     // 30 TENTHS of minutes without activity
      TimeOnLine,                   // 32 TENTHS of minutes since logon
      TimeCredit,                   // 34 TENTHS awarded for uploads, etc
      LastTSR,                      // 36
      Carrier;                      // 38 Is there a carrier or Local?

   char   *AName;                   // 40 ptr to access group name
   long   SysMaint,                 // 44
      LocalTime,                    // 48 current area sur-time minutes
      LocalCharge,                  // 52 current area sur-charge per minute
      LastTime,                     // 56
      ServLen,                      // 60 seconds in-between Service() calls
      Caller;                       // 64 which caller am I?

   struct   UserData   user1,       // 68
            user2;                  // 740 actual user structs

   short   MyMail;                  // 1412 counters for user currently online
   short   MyNewMail;               // 1414
   short   bn;                      // 1416 current sub #

   char   CallerID [18];            // 1418 Received from Supra, prev connect
   short   CIDmatch [ 8];           // Accounts which MATCH, [0] will be current user 

   UBYTE InFeedback;                // 1436 TRUE (1) if user is sending feedback - used to skip mail send flag prompt

   UBYTE NoClearIBuff;              // 1437 TRUE = add '/' to input buffer rather than use for buffer clearing operation 
   UBYTE NoSpaceAbort;              // 1438 TRUE = add SPACE to input stream rather than use as abort character
   UBYTE NoCaseConvert;             // 1439 TRUE = do not convert input from OneKey() to uppercase
   UBYTE   expansion[14];           // 1439

   UBYTE   PAccess,                 // 1470
      RAccess,
      DAccess,
      UAccess;

   char   fsuff[8],                 // 1474 more header stuff
      fcore[32],                    // 1482
      Title[80],                    // 1514 Screen/Window Title
      lastby_NOT[36];               // 1594 moved to struct PortDataExtension

   UBYTE   IUCBits[100];            // 1630 Hidden, Muffled, Monitor, ChatReq

   char   *MyDoing;                 // 1730
   struct   Room *CRoom;            // 1734 which room am I in ?
   struct   RoomUser *CUser;        // 1738 where is my RoomUser struct

   UBYTE   Dumped,                  // 1742 auto-kickoff flag
      OLMWaiting,                   // 1743 OLM waiting   
      HideAll,                      // 1744 Hidden from ALL?
      MuffAll,                      // 1745  Muffling ALL?
      MonitorAll,                   // 1746 Monitoring ALL?
      OnLine,                       // 1747
      nPdepth,                      // 1748 How many pfiles a port is running
      ChatCode,                     // 1749 who's chatting with whom
      WantToOpen,                   // 1750
      getout,                       // 1751 flag to auto-quit port
      loaded,                       // 1752 Is this port loaded?
      other[3];                     // 1753

   long   LastEvent,                // 1756 keep track of the last event
      LastCut,                      // 1760 time in these 4 catagories
      LastHold,                     // 1764
      LastOnLine;                   // 1768

   long   TempSysMaint;
   short   ConfIdle;

   struct NewMailFolder *Folders;   // 1778 - linked list of mail folders - may not be loaded until MailRead or MailSend
   struct NewMailFolder *Folder0;   // 1782 - pointer to folder currently in use

   struct MailHeader4 *rmhead;      // 1786 - linked list of mail header pointers allocated during mail read - mail only allocated for the currently "open" folder
   struct MailAlias *Aliases;       // 1790 - linked list of mail aliases - may not be loaded until MailRead/MailSend

   ULONG   DOIO_MASK;               // 1794
   struct   IOStdReq    *DoReq;     // 1798
   struct   MsgPort      *DoPort;   // 1802

   long   uz[10];                   // 1806 for C programmer use.  You can
                                    // (typecast) them as you need to

   struct   Task      *ThisTask;
   struct   MainPort   *cnp;

   struct   Window      *Window, *CWindow, *SWindow, *TWindow,
            *TTWindow, *WBWindow;
   struct   Screen      *Screen;
   struct   RastPort   *wrp, *trp;
   struct   TextFont   *BlueFont,
            *StandardFont;
   struct   IOStdReq    *UTReq, *ServReq, *PrintReq, *ConReq;
   struct   IOExtSer   *SerRdReq, *SerWrReq, *SerIOReq;
   struct   MsgPort      *SerRdPort,*SerWrPort,*SerIOPort, *ConPort,
            *UTPort,   *ServPort, *PrintPort, *cpreplyp;

   struct   FileInfoBlock   *fib;   // for reading directories
   struct   InfoData   *IData;      // for getting free disk space, etc
   struct   AnchorPath   *ap;
   struct   MessageType3    *Message;
   struct   BaseUser   *buser;
   struct   SelectType      *Select;
   struct   BaseUser   *brec;
   struct   SelectType   *Upload;
   struct   ListItem        *fpt;
   struct   ItemHeader   **ih, *ihead_obs;
   struct   ItemType3   **ip;
   char   **EdBuffer;               // pointers to editor lines
   char   *EdBase;                  // pointer to editor memory
   UBYTE   *CR;                     // Is there a Carriage Return? ea lin
   char   **mm;                     // pointers to bbsmenu lines
   char   **bm;                     // pointers to bbstext lines
   short   *menustart;
   short   *menulen;

   long   plsize, nplanes;          // SkyStuff Stuff
   short   skyxpen, skyypen;
   PLANEPTR planePtr;
   struct   TmpRas      tmpRas;
   struct   BitMap     SkyBrush;

   long   FullMaint;

   long   numWE;                    // window-edit, current number
   long   WWidth;                   // 80
   long   WLength;                  // 24
   long   WLeft;                    // def   0
   long   WRight;                   // def 639
   long   WTop;                     // def  11
   long   WBottom;                  // def YMAX

   short   xc,                      // x coord, cursor pos
      yc,                           // y coord
      sxc,                          // storage for ANSI [s, [t
      syc,
      YMAX,                         // max y value
      CurrentMouseX,
      CurrentMouseY,
      countWE;                      // window-edit, # characters
      
   short   IsHalfScreen;
   short   TermLength;

   short   SWidth;
   short   SHeight;
   short   DWidth;
   UBYTE   io_bad;
   UBYTE   io_status;               // debugging Supra hangs

   char   *by0,                     // scan and header variables
      *by1,                         // for MCI configurability
      *to0,
      *to1,
      *date0,
      *organ,

      plus[2],
      star[2],
      fdate[8],
      fsize[8],
      ftime[12],
      *title0,
      *olm_default,
      *vis_subj,
      *vis_to,
      *user_default,
      *subj_default;

   short   Depth;
   short   DHeight;

   short   TextSet;                 // 0==use system-wide text/menu
   short   npitems;                 // number of items parsed
   char   pitem[8][61];             // last input parsed list

   long   txline;                   // number of lines in ed buffer
   char   GBuffer[16][81];          // General purpose 2-D text buffer
   UBYTE   GCR[16];

   ULONG   WaitFlags,
      SERVICE_MASK,
      RAWKEY_MASK;

   struct   JobType4 *pEvents;      // 4062 - list of waiting port events - queued events are Loaded when port is up and running

   struct SignalSemaphore pesem;    // 4066 - semaphore (46 bytes long) for locking pevents (port events) list
   struct SignalSemaphore mfsem;    // 4112 - semaphore (46 bytes long) for locking Folders list

   ULONG   eventtick;               // 4158 - events are ticking
   ULONG   nextevent;               // 4162 - time next event for this port - in seconds

   struct   SignalSemaphore sem;    // 4166
   LONG nothing171196;              // 4212 - ordered listing (pointers) of items in door/list areas

   struct   MailHeader4 *POMail0;   // 4216 - pointer into z->rmhead - current message being read, edited or verified
   struct   ChargeSet  *NowCharge;

   long   CTC[80];                  // users account, 62 items, Cost of This Call

   short   sorder[240];             // max subboards per subdir is 240
                                    // ^^ needs to be changed to use DYNAMIC allocation
                                    // so it can bypass limit of 240!

   char   *LastError;               // for file transfer WHY command

   char   RexxObject  [100],
         KBuff       [256],         // keyboard buffer!

         *ABuffer;                  // 15-Apr-97 -> ABuffer now allocated dynamically at startup
                                    // (size of ABuffer now CONFIGURABLE via Config/Limits!)

   BYTE AB_expansion[252];          // got some nice expansion room by making ABuffer dynamic!

   char InBuffer    [256];          // after a line input
      
   char   rbyname[40];              // 29-Aug-96 .. handle/name to look for if name in "read mess by handle/name" is not local

   char def_mailsubj[81];           // 30-Aug-96 .. default mail subject for new mails. Set using ARexx "SetMailSubject"

   char   rtoname[40];              // 22-Sep-96 .. handle/name to look for if name in "read mess to handle/name" is not local

   struct PortDataExtension *PDE;   // NEW extension to PortData!! 18-Oct-96 - to be added as structure type 3 in GETUSER list

   UBYTE DizShort;                  // 17-Feb-97 .. TRUE if using [W]rite to write file short description

   char  Expansion290896[197];      // last changed 18-Oct-96

   char   Doing     [48],
      SPrompt     [128],            // subboard prompt
      LPrompt     [128],            // G/P/N list prompt
      ldiskloc    [160],            // diskloc for P/G/N
      myloc       [164],            // for Read-Mail
      filename    [116],
      ListArea     [40],
      rsearch      [64],
      myfile       [54],
      dirname      [32],
      CStack       [80],            // Command stacking
      ChatMsg      [84],            // current chat message
      sbuff        [48],
      outbuffer    [84],
      ansiout     [164],
      ConnectCode  [84],
      ChatBuff     [84],
      APrint       [36],            // for PrintAccess
      PName     [3][32],
      home         [36],
      qwk_to       [28],
      qwk_subject  [28],
      BackBuff [10][80],            // last 10 commands executed
      Pathx     [8][64];            // g/p/n subdir history

   long   bbnum;                    // pointer into D-Q

   long   warn1;

   long   TotalCTC;                 // TOTAL Cost of this Call (100ths

   short   bz,                      // # of subs on current List
      dz,                           // how deep in the tree are we?
      br,                           // item last used
      rn_obs,                       // how many items, current list

      nl,                           // number on g/p/n list
      allin_obs,                    // Are all items in memory?
      VTimeOnLine,                  // may reset at midnight
      PTimeOnLine,                  // in-current-pfile time counter

      mx,                           // highest item NOT killed
      listn,                        // current list subboard pointer
      parent0,
      gpos,                         // for global commands, last command #

      nsubs,                        // tracks # subs accessed in a Global cmd
      LinkPort;

   long   TermLinkBaud;

   char   *mygokeyword;             // go keyword for user's current location   - if 0, user is at main menu aka root object list

   long   rs,                       // were once SHORTS...moved here to
      cpos,                         // avoid conflicts with 32767 max  
      mstart,
      mend,
      new_i,                        // responses/item or per subboard 
      new_n,
      new_f,
      new_p,
      new_ri,
      t_new_i,                      // total new items
      t_new_n,                      // total new messages to you
      t_new_f,                      // total new files
      t_new_p,                      // total new posts
      t_new_ri;                     // total new response items

   USHORT   AParams,
      AParam[17];

   UBYTE   qwk;
   UBYTE   ylharc;
   UBYTE   yaddcr;
   UBYTE   ylocal;

   short   zmatches;
   UBYTE   zheader;
   UBYTE   readnew;

   long   nsets,
      rset,
      rcount,
      rtotal,
      set[10][2];

   UBYTE   runseen,
      resetIP,                      // flag, must re-order item pointers
      atbase,
      TermLink,

      rtome,
      rfree,
      rfavorite,
      runval,

      rprivate,
      rreverse,
      rbrandnew,
      rnresponse,

      rpass,
      rsince,
      rnew,
      rglobal,

      rdeftype,
      rchron,
      rbyme,
      runtil,

      rjustposts,
      rpreview,
      rbanner,
      rjustfiles;

   struct   IsDate   rdate,
         udate;

   short   pos,
      num,

      rdelta,                       // for/back n items

      NoSelected,                   // number of items in Select List
      NoUploaded,
      ModelType;                    // Controls .M in editor. 

   struct   RangeContext EdRange;

   long   tline, bit_mci;
   short   times, repmode, MaxLines, WScroll;

   long   warn2;

   char   keyin,
      edall_not,                    // allow enterline all chars
      serbuffer[2];

   UBYTE   mode,
      ZAlternate,
      CheckAllEvents,               // force check of TimeTilEvent's
      TrapDoor,

      AtPrompt,
      InShell,
      donormal,
      dtr,                          // modem device OPENED and ready

      sread,                        // read request pending ...
      modem,                        // modem exists ... allows BBSPORT0
      mio,                          // modem IO allowed
      isud_not,

      MaxColors,                    // # of colors on screen
      FlagCH,
      FlagED,
      Gflag,

      ConfMode,                     // Are we in conference?
      column,
      KB1,                          // DeQueue pointers
      KB2,

      KB3,                          // what inter-user chat sees
      CursorStatus,                 // is cursor on?
      AbortFlags,
      HourNow,

      LastHour,
      Indent,
      InANSI,
      GETarg,

      CMDarg,
      LocalMode,                    // BOOLS .. In Local Mode ?
      noflowcontrol,
      AtWaitScreen,                 // Are we at IDLE?

      InMCI,
      DoQuick,                      // ! at last input ?
      AutoLogon,
      TermOn,                       // in terminal?

      ANSIon,                       // can we type ansi? (ESC)
      PrintFlags,                   // space or ^C pressed
      CountUp,
      InputFlags,

      Relogon,
      Logoff,
      NewScanBROWSE,                // BROWSE at logon scan for new?
      AutoScroll,

      WWrap,
      DoubleBuffer,
      MoreRow,
      MoreAtRow,

      WScreen,
      InTransfer,
      NoMCI,
      FreezeTime,

      kolorific,
      mciflash,                     // actually, bold
      mciscore,
      mcireverse,

      intense,                      // color greater than 7?
      mciprint,
      mcispeed,
      mcicolor,                     // the requested MCI color 0-7

      pencolor,                     // actual screen pencolor
      mcinoabort,
      backcolor,                    // background color
      mciback,
   
      MCIenv,                       // current MCI environ flags
      MCIdefenv,
      MRinuse,                      // mail/post office in use status bits
                                    // eg. if((z->MRinuse & 1<<bit) == 1) to test.
                                    // bit 3 = In post office
                                    // bit 2 = in mailsend
                                    // bit 1 = in mailread

      OffLine,                      // skip answering the phone??

      cminute, 
      chour,
      FNewSave,                     // flag: update FNewDate at logoff
      intab;                        // for LoadLine()

   char   mcitest,                  // MCI stuff
      EditorOn,                     // is editor on?
      var5,
      ScreenWidth;

   long   warn3;

   long   swrite, LogoffFlag;       // file transfer stuff
   long   ResumeSize;
   short   ResumeNumber, StartUpTime;

   short   protonum;
   char   protobyte;
   char   edbuff;                   //   1 saved new text
                                    //   0 saved empty buffer
                                    //   -1 aborted


//   If you use QuoteType=2, you MUST set the following three variables
//   Note that QuoteFrom and QuoteDate are POINTERS ... do NOT copy into
//   them!

   char   *QuoteFrom;               // Who are we quoting?
   struct   IsDate *QuoteDate;      // When was that message written?
   long   QuoteEnd;                 // LAST BYTE position in file
                                    // Quoter will stop automatica
                                    //   if ^Z occurs first
   UBYTE   qwk_anonymous;
   UBYTE   qwk_private;
   short   xfercount;

   long   XferTime;
   long   LastCPS,
         cpsavecount;
   long   cpsavesum,
         LastXTime,
         StartByte,
         LastByte;

   short   ARunCount;               // depth of AREXX pfiles
   short   CRunCount;               // depth of C pfiles
   char   CSpawn[84];

   long   TrapBaud;
   long   TrapLock;

   UBYTE   QuoteType;               // 0==Mail, 1==Message b
                                    //   2==User defined.  See QuoteFrom,
                                    //   QuoteDate, etc., above */
   char   QuotePath[103];           // MESSAGE QUOTING in editor.
                                    // SETTING this ENABLES quoti
                                    //   Remember to QuotePath[0] = 0 after
                                    //   you use it.
   long   QuoteSeek;                // What's the starting byte pos?

   short   imode;                   // editor.  Insert mode?  If yes, which lin
   short   iline;

   long   abort;

   short   UrgentMail1;
   char   ExtTransfer;
   char   ExtProtocol;

   long   ExtMinFree;

   long   ALogon;
   long   BLogon;
   long   ATransfer;
   long   BDownload;
   long   BUpload;
   long   APfiles;
   long   BCharge;
   long   hiCharge;
   long   loCharge;

   long   rtoid;                    // to 'handle' searches

   long   TimeTilOnLine,
      TimeTilCutoff,                // Log user off first event
      TimeTilEvent,                 // Immediate (regular) event
      TimeTilHold;                  // hold until off-line event

   long   SERIAL_MASK,
      UTILITY_MASK;

   char   *xpr_filename;


   UBYTE   xpr_obsolete[124];


   struct   MsgPort *rexxPort;
   long   rexxPortBit;
   char   *extension;
   struct   rexxCommandList *globalrcl;

   UBYTE   callback,                // enable AUTOCALLBACK @ LOGON
      avalid,                       // which avalid set to use
      joinlink,                     // 0==off, 1==accept, 2==accept ONLY
      FreezeIdle;                   // special use -- stop idle timer

   long   rbyid;                    // by 'handle' searches

   long   SAMNow[20];               // users 'Last' SAM column

   struct   CMessage   cpmess;

   BYTE      RRM_trig;              // true if user performed Repair_mail
                                    // on this port and mail needs to rescan

   BYTE      was_SerPort[347];      // WAS SERPORT data. see above struct
                                    // moved to PortDataExtension!

   struct   BaseUser   xbrec;
   struct   ItemType3   Item0;
   struct   ItemHeader   ihead0;
   struct   HeaderType   Header0, LastHeader;
   struct   OldMailHeader   OMail0_NOT;
   struct   SelectType   Select0;
   struct   MessageType3   Message0;

   UBYTE   Subdir_NOT [172];

   struct   SubboardType4 *sb;

   char   rpattok[202];             // tokenized search pattern

   UBYTE   Subboard0_NOT [276];

   short   WBorTop;
   short   nupload;                 // # of upload allocated   
   short   nips_obs;

   short   Monitored;               // 128 + port number if monitored.
   short   ModelEnable;             // Controls .M in editor. 

   BPTR   MCIfile;
   long   mcilabel;
   long   nlabels;
   long   mlabel[20];
   long   mfseek[20];
   long   MCInreg[10];
   char   MCIcreg[ 5][100];

   BPTR   sf;                       // for _short scan
   long   sfin;

   long   VoteItem;                 // which vote? -1==New
   char   *VotePath;                // for Vote in Any Area!

   UBYTE   q_kolorific,             // MCI Q2/Q3 storage
      q_mciflash,
      q_mciscore,
      q_mcireverse,

      q_intense,
      q_mciprint,
      q_mcispeed,
      q_mcicolor,

      q_mciback,
      q_mcinoabort,
      q_MCIenv;

   UBYTE   SelectInMemory;          // is _select in memory or on disk?

   struct   Event   *FirstEvent,
         *FirstCutEvent,
         *FirstHoldEvent,
         *FirstOnLineEvent;

   struct   TermLink tl;

   char   JoinLinkOut[76];

   struct   VDEentry *vv;           // the currently loaded VDE file
   short   vns;                     // used with VDEapply featuers! 
   UBYTE   vmulti;                  // editing more than one?       
   UBYTE   iselect;                 // is the input SELECTED?       

   char   gmPath[36];               // global Read-Mail variables...
   ULONG   *gmPos;
   ULONG   *gmNum;                  // needed for Yank command .... 
   UBYTE   *gmFlags;

   UBYTE   *amem;                   // DrawEd variables
   UBYTE   *cmem;
   long   maxdraw;

   UBYTE   predraw;
   UBYTE   GiveCPR;                 // CPR request received in term
   UBYTE   hmmmm[2];

   UBYTE   *smem;

   long   inlen, edlength;

   UBYTE   invalid;
   char FileNote[750];              // temp storage for "short" descriptions, DIZ and filenotes
                                    // moved here to facilitate expansion - was 364 bytes previously

   // new for v4.12
   struct MailHeader4   OMail0;     // general purpose mailheader (not a pointer!) - used for outgoing/incoming mail and mailread
   struct SignalSemaphore masem;    // semaphore for locking Aliases list
   struct SignalSemaphore rmhsem;   // semaphore for locking the mail header list
};


////////////////////////////////////////////////////////////////////////////
//
//        PortData extensions!   (z->PDE)        ----      18-Oct-96
//
struct PortDataExtension {
   struct TNHostEntry *TNHost0;       // pointer to telnet host entry in use during list/scan/edit of sysop/pre-configured hosts
   struct TNHostEntry *CurrentTNHost; // pointer to telnet host entry currently connected to - NULL/0 if none/unique
   LONG *GoMatches;                   // pointer to array of GO matches used in parsing GO keys
   char *DynamicGoKey;                // go keyword currently displayed for GO arg listings
   char *DynamicGoTitle;              // Title of currently displayed subboard in Go arg listing
   LONG DynamicGoNum;                 // number (in current list) of Go arg
   UBYTE InMailFwd;                   // TRUE if we are currently forwarding mail.
   short def_part;                    // default partition to move files to (dynamic setting pertains to bbstext line 1778)..
   short MailArrived;                 // number of new mails attached to online user's mailbox - set by mail-task

   // 9-Jan-97
   char lastby[52];                   // moved here from previous GU offset
                                      // of 1594 in struct PortData - expanded
                                      // from 36 characters to 52 in length.

   struct SerPort4 sp;                // moved here from PortData!
};


// obsolete
#define HIDDEN_FLAG   0x01
#define MUFFLED_FLAG   0x02
#define MONITOR_FLAG   0x04
#define CHATREQ_FLAG   0x08

#endif
