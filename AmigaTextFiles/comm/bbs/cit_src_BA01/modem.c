/*
*       modem.c
*
* modem code for Citadel bulletin board system
* NB: this code is rather machine-dependent:  it will typically
* need some twiddling for each new installation.
*       82Nov05 CrT
*
* now this file is mostly for upper layer modem handling and the protocols,
* so it may need no fiddling.
*       88May07 HAW
*/
/*
*       history
*
* 85Nov09 HAW  Warning bell before timeout.
* 85Oct27 HAW  Cermetek support eliminated.
* 85Oct18 HAW  2400 support.
* 85Sep15 HAW  Put limit in ringSysop().
* 85Aug17 HAW  Update for gotCarrier().
* 85Jul05 HAW  Insert fix code (Brian Riley) for 1200 network.
* 85Jun11 HAW  Fix readFile to recognize loss of carrier.
* 85May27 HAW  Code for networking time out.
* 85May06 HAW  Code for daily timeout.
* 85Mar07 HAW  Stick in Sperry PC mods for MSDOS.
* 85Feb22 HAW  Upload/download implemented.
* 85Feb20 HAW  IMPERVIOUS flag implemented.
* 85Feb17 HAW  Baud change functions installed.
* 85Feb09 HAW and Sr.  Chat bug analyzed by Sr.
* 85Jan16 JLS  fastIn modified for CR being first character from modem.
* 85Jan04 HAW  Code added but not tested for new WC functions.
* 84Sep12 HAW  Continue massacre of portability -- bye, pMIReady.
* 84Aug30 HAW  Wheeee!!  MS-DOS time!!
* 84Aug22 HAW  Compilation directive for 8085 chips inserted.
* 84Jul08 JLS & HAW ReadFile() fixed for the 255 rollover.
* 84Jul03 JLS & HAW All references to putCh changed to putChar.
* 84Jun23 HAW & JLS Local unused variables zapped.
* 84Mar07 HAW  Upgrade to BDS 1.50a begun.
* 83Mar01 CrT  FastIn() ignores LFs etc -- CRLF folks won't be trapped.
* 83Feb25 CrT  Possible fix for backspace-in-message-entry problem.
* 83Feb18 CrT  fastIn() upload mode cutting in on people.  Fixed.
* 82Dec16 dvm  modemInit revised for FDC-1, with kludge for use with
*   Big Board development system
* 82Dec06 CrT  2.00 release.
* 82Nov15 CrT  readfile() & sendfile() borrowed from TelEdit.c
* 82Nov05 CrT  Individual history file established
*/
#include "ctdl.h"
void DumpToFile(int LastReceived, int BufSize, CRC_TYPE tc, CRC_TYPE oc);
/*
*       Contents
*
* BBSCharReady()    returns true if user input is ready
* ClearWX()   finishes a WXMODEM transmission
* CommonPacket()    reads a block
* CommonWrite()   writes a block to wherever
* FlowControl()   flow control handler for WXMODEM
* GenTrInit()   general init for individual protocols
* getMod()    bottom-level modem-input filter
* iChar()     top-level user-input function
* initTransfers()   initial data buffers of protocols
* interact()    chat mode
* JumpStart()   gets protocol reception going
* MIReady()   check MS-DOS interrupt for data
* modIn()     returns a user char
* modemInit()   top-level initialize-all-modem-stuff
* oChar()     top-level user-output function
* Reception()   receive data via protocol
* recWX()     receive data via WXMODEM
* recWXchar()   receive a WXMODEM char, stripped of DLE
* recXYmodem()    receive data via X or Y MODEM
* ringSysop()   signal chat-mode request
* SendCmnBlk()    sends a WX/X/Y/MODEM block
* sendWCChar()    send file with WC-protocol handshaking
* sendWXchar()    send a char for WXMODEM
* sendWXModem()   send data with WXMODEM protocol
* sendYMChar()    send file with YMODEM protocol
* SummonSysop()   rings bell for ^T
* SurreptitiousChar() process a console character in MODEM
* Transmission()    handles protocol transmission
* WXResponses()   handles NAK/ACK XON/XOFF for WXMODEM
* XYBlock()   common routine for X & Y modem
* XYClear()   finished X or Y MODEM transmission
* YMHdr()     YMODEM BATCH header handler
*/
void ReActivate_Window(void);  /* allow window to be activated */
char justLostCarrier = FALSE;   /* Modem <==> rooms connection  */
char newCarrier = FALSE;   /* Just got carrier  */
char onConsole;     /* Who's in control?!?  */
int  outPut = NORMAL;
char modStat;     /* Whether modem had carrier LAST time  */
/* you checked. */
char CallSysop = FALSE;   /* Call sysop on user logout  */
char whichIO = CONSOLE;   /* CONSOLE or MODEM */
#ifdef NEED_VISIBLE
char visibleMode;   /* make non-printables visible? */
#endif
static char captureOn = FALSE;
static FILE *cptFile;
char haveCarrier;   /* set if DCD == TRUE */
char textDownload = FALSE;  /* read host files, TRUE => ASCII */
char echo;      /* Either NEITHER, CALLER, or BOTH  */
char echoChar;      /* What to echo with if echo == NEITHER */
int  TransProtocol;   /* Transfer protocol value  */
int  upDay;     /* Day system was brought up  */
char nextDay;     /* Come down tomorrow rather than today?*/
int  timeCrash;
char anyEcho = TRUE;
char warned;
long netBytes;
char ChatMode;
char NoConsoleBanner;   /* defaults to FALSE */
long TempBytes;
char ConOnly = FALSE;
static char PB = 0;
/* Block transfer variables */
TransferBlock Twindow[4];
#define blk Twindow[0]
int  CurWindow,   /* For sequence #, init to 1 */
TrBlock,    /* For block #, init to 1 */
StartWindow,  /* First block of current Twindow, init = 1 */
TrCount,    /* Byte accumulator counter, init to 0 */
LastSent,   /* Last seq block sent to receiver, regardless of ACK */
CurYBufSize,  /* Size of current YMODEM block to send */
GlobalHeader; /* Starting char of transfer (YMODEM) */
char DoCRC,   /* True if doing CRC */
DLinkError, /* For conveying errors during WXMODEM */
TrError,    /* True only on fatal error */
DLEsignal;    /* True only when an WXMODEM read involved a DLE */
AN_UNSIGNED *DataBuf;
int      TrCksm;  /* Checksum variable for transmissions */
static char *msg[] =
  {
  "NO ERROR",
  "BAD DLE",
  "EARLY SYN",
  "DATA TIMEOUT",
  "BAD CRC",
  "BAD CHECKSUM",
  "BAD SECTOR COMPLEMENT",
  "SYNCH ERROR",
  "WRITE ERROR",
  "CARRIER LOSS"

  };
#define WindowFull()    IsSent(0) && IsSent(1) && IsSent(2) && IsSent(3)
#define WindowEmpty()   IsDone(0) && IsDone(1) && IsDone(2) && IsDone(3)
#define IsDone(x) (IsAcked(x) || NotUsed(x))
#define IsSent(x) (Twindow[x].status == SENT)
#define IsAcked(x)  (Twindow[x].status == ACKED)
#define NotUsed(x)  (Twindow[x].status == NOT_USED)
/*
* This table presupposes that XMODEM is defined as 1, YMODEM as 2, WXMODEM
* as 3.  Used only by CommonPacket().
*/
static int time_table[] =
  {
  0, 1, 4, 15

  };
long ByteCount;
extern MessageBuffer   msgBuf;  /* Message buffer */
extern CONFIG    cfg;   /* Configuration variables  */
extern logBuffer logBuf;  /* Log buffer of a person */
extern aRoom  roomBuf;  /* Room buffer  */
extern FILE *upfd;
extern char loggedIn; /* Is we logged in? */
extern char prevChar; /* previous char  */
extern char outFlag;  /* output flag  */
extern char ExitToMsdos;  /* Kill program flag  */
extern int  exitValue;
/* net stuff vars should go here */
extern char inNet;
extern FILE *netLog;
extern FILE *strollfd;
/* bloooooop! */
extern char netDebug;
extern PROTO_TABLE  Table[];
/*
* BBSCharReady()
*
* This returns TRUE if char is available from user.
* NB: user may be on modem, or may be sysop in CONSOLE mode.
*/
char BBSCharReady()
  {
  return (char) (PB || ((haveCarrier && whichIO == MODEM) && MIReady()) ||
  (whichIO == CONSOLE  &&   KBReady()));

  }
#ifdef WXMODEM_AVAILABLE
/*
* ClearWX()
*
* This finishes a WXMODEM transmission.
*/
int ClearWX()
  {
  int rover, BlockRover, TempSent, SendEOT;
  if (!gotCarrier())
  return CARR_LOSS;
  outMod(EOT);  /* Forces us to wait until output buffer is flushed */
  while (!WindowEmpty())
    {
    WXResponses();
    for (BlockRover = (LastSent + 1) % 4, TempSent = LastSent, SendEOT=0;
    BlockRover != LastSent; BlockRover = (BlockRover + 1) % 4)
      {
      if (Twindow[BlockRover].status != SECTOR_READY) break;
      SendEOT++;
      SendCmnBlk(WXMDM, Twindow + BlockRover, sendWXchar, SECTSIZE);
      Twindow[BlockRover].status = SENT;
      TempSent = BlockRover;

      }
    LastSent = TempSent;
    if (!gotCarrier())
    return CARR_LOSS;
    if (SendEOT) outMod(EOT);

    }
  for (rover = 1; rover < MAX_WX_ERRORS; rover++)
    {
    if (!gotCarrier())
    return CARR_LOSS;
    if (receive(3) != ACK) outMod(EOT);
    else return TRAN_SUCCESS;

    }
  return TRAN_FAILURE;

  }
#endif
/*
* CommonPacket()
*
* This reads a block of data (XMDM, YMDM, WXMDM).
*/
int CommonPacket(char type, int size, int (*recFn)(int t), int *Sector)
  {
  int  comp, cksm, i, c, hi, lo, time;
  CRC_TYPE crc;
  #ifdef NEED_NET_DEBUG_ERRORS
  CRC_TYPE oc;
  #endif
  /*
  * Format:
  *
  * <SOH | STX><Sec#><w Sec#><size bytes of data><checksum or CRC>
  *
  * SOH | STX has already been received by the caller.  type is used for
  * protocol specific problems, as follows.
  *
  * WXMDM:
  * 1. When SYN is detected without DLEsignal == TRUE, this indicates a bad
  *    packet problem, so that must be checked.
  */
  time = time_table[type];
  *Sector = (*recFn)(time);   /* Get Sector # */
  #ifdef WXMODEM_AVAILABLE
  if (type == WXMDM && *Sector == SYN && !DLEsignal) return EARLY_SYN;
  #endif
  comp = (*recFn)(time);  /* Get Sector #'s complement */
  #ifdef WXMODEM_AVAILABLE
  if (type == WXMDM && comp == SYN && !DLEsignal) return EARLY_SYN;
  #endif
  for (i = cksm = 0; i < size; i++)
    {
    /* Get data block */
    if ((c = (*recFn)(time)) == ERROR)
      {
      if (!gotCarrier())
      return CARR_LOSS;
      if (cfg.BoolFlags.debug)splitF(netLog, "TIMEOUT on byte %d\n", i);
      return DATA_TIMEOUT;

      }
    #ifdef WXMODEM_AVAILABLE
    if (  type == WXMDM &&
    c == SYN && !DLEsignal  )
      {
      return EARLY_SYN;

      }
    #endif
    DataBuf[i] = c;
    cksm = (c + cksm) & 0xFF;
    if (!gotCarrier()) return CARR_LOSS;

    }
  hi = (*recFn)(time);  /* Get cksm or hi byte of CRC */
  if (DoCRC)
    {
    lo = (*recFn)(time);    /* Get lo byte of CRC */
    crc = (hi << 8) + lo;
    if (*Sector + comp == 0xff) /* Validations... */
    if (crc != calcrc(DataBuf, size))
      {
      #ifdef NEED_NET_DEBUG_ERRORS
      splitF(netLog, "CRC error: we calc %x, they sent %x\n", oc=calcrc(DataBuf, size), crc);
      DumpToFile(*Sector, size, crc, oc);
      #endif
      return BAD_CRC;

      }

    }
  else
    {
    if (hi != cksm)
    return BAD_CKSM;

    }
  if (*Sector + comp != 0xFF)
    {
    /* Check this to make sure, too */
    #ifdef NEED_NET_DEBUG_ERRORS
    if (inNet != NON_NET && netDebug)
    splitF(netLog, "BT: %x %x\n", *Sector, comp);
    printf("\nBT: %x %x\n", *Sector, comp);
    #endif
    return BAD_SEC_COMP;

    }
  return NO_ERROR;

  }
/*
* CommonWrite()
*
* This function writes a block of data to wherever.
*/
int CommonWrite(int (*WriteFn)(int c), int size)
  {
  int i;
  for (i = 0; i < size; i++)
  if ((*WriteFn)(DataBuf[i]) == ERROR)
  return WRITE_ERROR;
  return NO_ERROR;

  }
#ifdef WXMODEM_AVAILABLE
/*
* FlowControl()
*
* This function handles XON/XOFF for WXMODEM.
*/
void FlowControl()
  {
  int val;
  startTimer(WORK_TIMER);
  do
  val = receive(1); /* Use receive in this instance */
  while (val != XON && chkTimeSince(WORK_TIMER) < 10);

  }
#endif
/*
* GenTrInit()
*
* General protocol initializations.
*/
void GenTrInit()
  {
  int i;
  for (i = 0; i < 4; i++) Twindow[i].status = NOT_USED;
  CurWindow  = TrBlock   = StartWindow = 1;
  TrCount    = LastSent  = TrCksm = 0;
  DLinkError = DLEsignal = FALSE;
  TrError    = TRAN_SUCCESS;
  DoCRC = TRUE;

  }
/*
* iChar()
*
* This is the top-level user-input function -- this is the function the rest
* of Citadel uses to obtain user input.
*/
char iChar()
  {
  char  c;
  extern AN_UNSIGNED crtColumn; /* current position on screen */
  if (justLostCarrier)   return 0;    /* ugly patch   */
  c = cfg.filter[modIn() & 0x7f];
  switch (echo)
    {
    case BOTH:
    if (haveCarrier)
      {
      if (c == '\n')
        {
        if (!HalfDup) doCR();

        }
      else
      if (!HalfDup) outMod(c);

      }
    mputChar(c);  /* Let putChar decide if it should go on console */
    crtColumn += (c == '\b') ? -1 : 1;
    break;
    case CALLER:
    if (whichIO == MODEM)
      {
      if (c == '\n')
        {
        if (!HalfDup) doCR();

        }
      else
        {
        if (!HalfDup) outMod(c);

        }

      }
    else
      {
      mputChar(c);

      }
    crtColumn += (c == '\b') ? -1 : 1;
    break;
    case NEITHER:
    if (echoChar != '\0')
      {
      if (whichIO == MODEM)
        {
        if (c == '\n') doCR();
        else if (c <= ' ') outMod(c);
        else  outMod(echoChar);

        }
      else
        {
        if (c == '\n') doCR();
        else if (c <= ' ') mputChar(c);
        else  mputChar(echoChar);

        }
      crtColumn += (c == '\b') ? -1 : 1;

      }
    break;

    }
  return(c);

  }
/*
* initTransfers()
*
* This initializes data buffers for protocol transfers.
*/
void initTransfers()
  {
  int i;
  for (i = 1; i < 4; i++)
    {
    Twindow[i].buf = GetDynamic(SECTSIZE);

    }
  DataBuf = blk.buf = GetDynamic(YM_BLOCK_SIZE);

  }
/*
* interact()
*
* Here we try to chat with the users.  Or at least the modem.
*/
void interact(char ask)
  {
  char CallerDumb;
  char last = 0;
  int  c = 0;
  extern char *APPEND_TEXT;
  ChatMode = TRUE;
  printf("\nDirect modem-interaction mode\n");
  if (ask)
    {
    ConOnly = TRUE;
    CallerDumb = getYesNo("DUMBCL");
    ConOnly = FALSE;
    if (!gotCarrier()) EnableModem(FALSE);

    }
  else
    {
    CallerDumb = FALSE;

    }
  printf("<ESC> to exit\n");
  /* incredibly ugly code.  Rethink sometime: */
  while (c != SPECIAL )
    {
    c = 0;
    BufferingOn();
    if (MIReady())
      {
      c = inp() & 0x7f;
      if( c == '\0')c = SPECIAL;
      if (c == SPECIAL && !ask) c = 0;
      if (c != '\r') c = cfg.filter[c];
      if (c != '\r')
        {
        if (CallerDumb && c != ESC && c != 0)   outMod(c);
        if (c != 0) interOut(c);

        }
      else
        {
        interOut('\n');
        if (CallerDumb)
          {
          outMod('\r');
          outMod('\n');

          }

        }

      }
    else if (KBReady())
      {
      BufferingOff();
      if ((c = getCh()) == '\r') c = '\n';
      if (c == CPT_SIGNAL)
        {
        /* capture input? */
        captureOn = !captureOn;
        if (captureOn)
          {
          if ((cptFile = safeopen("chat.txt", APPEND_TEXT)) == NULL)
            {
            printf("\nCOULDN'T OPEN/APPEND TO CHAT.TXT!\n");
            captureOn = FALSE;

            }
          else
            {
            printf("\nAppending to CHAT.TXT\n");
            mPrintf(
            "\n WARNING from Citadel: This chat is now being recorded.\n ");

            }

          }
        else
          {
          fclose(cptFile);
          printf("\nCapture finished\n");
          mPrintf(
          " \nMessage from Citadel: Chat no longer being captured.\n ");

          }

        }
      else if (c == 5 || ChatEat(c))
        {
        ChatGrab(TRUE);

        }
      else if (c == 6 || ChatSend(c))
        {
        ChatGrab(FALSE);

        }
      else if (c != NEWLINE)
        {
        if (CallerDumb)  interOut(c);
        if (c != ESC && c != '\\')   outMod(c);
        else
          {
          if (last == '\\')
            {
            outMod(c);
            c = 0;

            }

          }
        last = c;

        }
      else
        {
        outMod('\r');
        if (CallerDumb)
          {
          interOut('\n');
          outMod('\n');

          }

        }

      }
    else
      {
      BufferingOff();
      BeNice(CHAT_NICE);
      DoTimeouts();

      }

    }
  if (captureOn)
    {
    captureOn = FALSE;
    fclose(cptFile);

    }
  if (!gotCarrier() && whichIO == CONSOLE)
  DisableModem(FALSE);
  ChatMode = FALSE;
  BufferingOff();

  }
/*
* ChatGrab()
*
* This function implements downloading from Chat.
*/
void ChatGrab(char Up)
  {
  SListBase CSelects =
    {
    NULL, FindSelect, NULL, NoFree, NULL

    };
  char *Protocols[] =
    {
    TERM "Xmodem",
    #ifdef WXMODEM_AVAILABLE
    TERM "Wxmodem",
    #endif
    TERM "Ymodem",
    /* these are the external protocols */
    " ", " ", " ", " ", " ",
    " ", " ", " ", " ", " ",
    " ", " ", " ", " ", " ",
    ""

    };
  char  letter[2];
  int   protocol;
  label roomName;
  if (roomBuf.rbflags.ISDIR != 1)
    {
    printf("\nSorry this is not a directory room.\nNew room? ");
    ConOnly = TRUE;
    getNormStr("", roomName, NAMESIZE, 0);
    ConOnly = FALSE;
    if (strLen(roomName) == 0) return;
    gotoRoom(roomName, 'R');
    if (roomBuf.rbflags.ISDIR != 1)
      {
      printf("\nNor is this is a directory room.\n");
      return;

      }

    }
  printf((Up) ? "Grabbing file from other system (into %s)\n" :
  "Sending file to other system (from %s)\n", roomBuf.rbname);
  ConOnly = TRUE;
  AddExternProtocolOptions(Protocols, Up);
  printf("\nProtocol: ");
  CmdMenuList(Protocols, &CSelects, NULL, letter, FALSE, FALSE);
  switch (letter[0])
    {
    #ifdef WXMODEM_AVAILABLE
    case 'X':
    case 'W':
    case 'Y':
    protocol    = (letter[0] == 'W') ? WXMDM :
    (letter[0] == 'X') ? XMDM : YMDM;
    break;
    #else
    case 'X':
    case 'Y':
    protocol    = (letter[0] == 'X') ? XMDM : YMDM;
    break;
    #endif
    default:
    if ((protocol = FindProtocolCode(letter[0], Up)) == -1)
      {
      ConOnly = FALSE;
      /* KillList(&CSelects); */
      return ;

      }

    }
  if (Up) upLoad(protocol,NULL,FALSE);
  else    TranFiles(protocol, "");
  ConOnly = FALSE;
  /* KillList(&CSelects); */
  printf("\nBack in Chat.\n");

  }
/*
* interOut()
*
* This function actually implements chat capture and general interact()
* output.
*/
void interOut(char c)
  {
  mputChar(c);
  if (captureOn)
  fputc(c, cptFile);

  }
/*
* JumpStart()
*
* Generic jump start for Reception.
*/
char JumpStart(int tries, int timeout, int Starter, int t1, int t2,
char (*Method)(int (*wrt)(int c)), int (*WriteFn)(int c))
  {
  int StartTries;
  for (StartTries = 0; StartTries < tries; StartTries++)
    {
    if (!gotCarrier())
      {
      #ifdef NEED_NET_DEBUG_ERRORS
      if (inNet != NON_NET)
      splitF(netLog, "RecError: JS Carr Loss\n");
      #endif
      return CARR_LOSS;

      }
    outMod(Starter);
    GlobalHeader = receive(timeout);
    if (GlobalHeader == t1 || GlobalHeader == t2)
      {
      return (*Method)(WriteFn);

      }
    if (GlobalHeader == CAN)
      {
      #ifdef NEED_NET_DEBUG_ERRORS
      if (inNet != NON_NET)
      splitF(netLog, "RecError: JS JS received CAN\n");
      #endif
      return CANCEL;

      }
    if (GlobalHeader == EOT)
      {
      outMod(ACK);
      return TRAN_SUCCESS;  /* zero length data */

      }

    }
  return NO_LUCK;

  }
/*
* ModemSetup()
*
* This function will set up modem handling variables.
*/
char ModemSetup(char ShouldBeCarrier)
  {
  haveCarrier = modStat = gotCarrier();
  if (ShouldBeCarrier && !haveCarrier) return FALSE;
  if (!ShouldBeCarrier && haveCarrier)
    {
    HangUp(TRUE);
    haveCarrier = modStat = gotCarrier();

    }
  return TRUE;

  }
/**
* modIn()
*
* toplevel modem-input function.
*
* If DCD status has changed since the last access, reports carrier present or
* absent and sets flags as appropriate.  In case of a carrier loss, waits 20
* ticks and rechecks carrier to make sure it was not a temporary glitch.
* If carrier is newly received, returns newCarrier = TRUE;  if carrier lost
* returns 0.  If carrier is present and state has not changed, gets a
* character if present and returns it.  If a character is typed at the console,
* checks to see if it is keyboard interrupt character.  If so, prints
* short-form console menu and awaits next keyboard character.
*
* Globals modified:    carrierDetect   modStat    haveCarrier
*     justLostCarrier whichIO   ExitToMsDos
*     visibleMode
* Returns:  modem or console input character,
*   or above special values
**/
#define MAX_TIME  210l  /* Time out is 210 seconds  */
AN_UNSIGNED modIn()
  {
  AN_UNSIGNED logVal = 0;
  AN_UNSIGNED c;
  char *whatRate;
  char signal = FALSE;
  if (PB)
    {
    c = PB;
    PB = 0;
    return c;

    }
  if (!onLine() && CallSysop)SummonSysop();
  startTimer(WORK_TIMER);
  while (TRUE)
    {
    if ((whichIO==MODEM) && (c=gotCarrier()) != modStat)
      {
      /* carrier changed   */
      if (c)
        {
        /* carrier present   */
        if (Find_baud(&whatRate))
          {
          printf("Carr-detect (%s)\n", whatRate);
          warned      = FALSE;
          haveCarrier = TRUE;
          modStat     = c;
          newCarrier  = TRUE;
          justLostCarrier = FALSE;
          logMessage(BAUD, whatRate, FALSE);
          ScrNewUser();

          }
        else
        HangUp(TRUE);
        return(0);

        }
      else
        {
        pause(100);   /* confirm it's not a glitch */
        if (!gotCarrier())
          {
          /* check again */
          printf("Carr-loss\n");
          logMessage(CARRLOSS, "", logVal);
          HangUp(TRUE);
          modStat = haveCarrier = FALSE;
          justLostCarrier = TRUE;
          startTimer(NEXT_ANYNET);  /* start anytime net timer */
          return(0);

          }

        }

      }
    if (MIReady())
      {
      if (haveCarrier)
        {
        c = inp() & 0x7f;
        if (whichIO == MODEM)   return c;

        }

      }
    if (KBReady())
      {
      c = getCh();
      if (whichIO == CONSOLE) return(c);
      else
        {
        if (!SurreptitiousChar(c))
        return 0;

        }

      }
    if (DoTimeouts()) return 0;
    /* check for no input.  (Short-circuit evaluation, remember!) */
    if ((whichIO==MODEM  &&  haveCarrier  &&
    chkTimeSince(WORK_TIMER) >= MAX_TIME) ||
    (whichIO == CONSOLE && cfg.ConTimeOut != 0 &&
    chkTimeSince(WORK_TIMER) >= cfg.ConTimeOut))
      {
      mPrintf("Sleeping? Call again :-)");
      logVal = 't';
      if (whichIO == MODEM)
      HangUp(FALSE);
      else
        {
        logMessage(CARRLOSS, "", logVal);
        justLostCarrier = TRUE;
        onConsole = FALSE;
        return 0;

        }

      }
    else if ((whichIO == MODEM &&
    haveCarrier &&
    chkTimeSince(WORK_TIMER) == MAX_TIME - 10) ||
    (whichIO == CONSOLE && cfg.ConTimeOut != 0 &&
    chkTimeSince(WORK_TIMER) == cfg.ConTimeOut - 10))
      {
      if (!signal)
      oChar(BELL);
      signal = TRUE;

      }
    BeNice((haveCarrier || onConsole) ? INUSE_PAUSE : IDLE_PAUSE);

    }

  }
/*
* SurreptitiousChar()
*
* This function will process a console character when the system is in modem
* mode.
*/
int SurreptitiousChar(char c)
  {
  switch (toUpper(c))
    {
    case CON_NEXT:    /* ^T */
    if (onLine())
      {
      CallSysop = !CallSysop;
      printf("\nSysop call toggle is %s\n", CallSysop ? "ON" : "OFF");
      ScrNewUser();
      break;

      }
    /* yes, don't break here! */
    case SPECIAL:   /* ESC */
    printf("CONSOLE mode\n ");
    onConsole  = TRUE;
    whichIO    = CONSOLE;
    if (!gotCarrier())
      {
      DisableModem(FALSE);
      logMessage(BAUD, "", FALSE);

      }
    setUp(FALSE);
    if (!gotCarrier())
      {
      ScrNewUser();
      if (!NoConsoleBanner)
      newCarrier = TRUE;

      }
    warned = FALSE;
    return FALSE;
    case 1:     /* ^A */
    ForceAnytime();
    break;
    case 5:     /* ^E */
    anyEcho = !anyEcho;
    ScrNewUser();
    break;
    case CNTRLD:
    cfg.BoolFlags.debug  = !cfg.BoolFlags.debug;
    break;
    case CNTRLZ:
    cfg.BoolFlags.noChat = !cfg.BoolFlags.noChat;
    ScrNewUser();
    break;

    }
  return TRUE;

  }
/*
* oChar()
*
* This function is the top-level user-output routine.  It sends to modem port
* and console, does conversion to upper-case etc as necessary in "debug" mode,
* converts control chars to uppercase letters
* Globals modified: prevChar
*/
int delay_factor= -1;
void oChar(char c)
  {
  prevChar = c;     /* for end-of-paragraph code    */
  if (outFlag != OUTOK &&   /* s(kip) mode  */
  outFlag != IMPERVIOUS)
  return;
  if( !onConsole && logBuf.lbdelay > 0 && delay_factor == -1)
    {
    delay_factor = ( 256 - logBuf.lbdelay );
    };

  if (c == NEWLINE)   c = ' ';  /* doCR() handles real newlines */
  /* show on console  */
  if (outPut == DISK)
  putc(c, upfd);
  else
    {
    if (haveCarrier && !ConOnly)
    (*Table[TransProtocol].method)(c);
    if (TransProtocol == ASCII)
      {
      mputChar(c);
      if (!onConsole && logBuf.lbdelay > 0 )
         {
         if( delay_factor-- < 1 )
           {
           delay_factor = ( 256 - logBuf.lbdelay );
           MilliSecPause(3);
           };
         };

      }

    }
  crtColumn += (c == '\b') ? -1 : 1;

  }
/*
* PushBack()
*/
void PushBack(char c)
  {
  PB = c;

  }
/*
* Reception()
*
* This function reads data, trying to use specified protocol.  Note: This only
* handles XMODEM, WXMODEM, and YMODEM.  Due to the multiple authors of these
* protocols, this code is a mess.  I do da best I can, wid da leetle brain
* giben me.
*/
char Reception(int protocol, int (*WriteFn)(int c))
  {
  char RetVal;
  if (!gotCarrier())
  return CARR_LOSS;
  GenTrInit();
  #ifdef WXMODEM_AVAILABLE
  /* From P. Boswell's WXMODEM doc */
  if (protocol == WXMDM)
    {
    if ((RetVal = JumpStart(3, 3, 'W', SYN, SYN, recWX, WriteFn))
    != NO_LUCK)
    return RetVal;
    if ((RetVal = JumpStart(3, 3, 'C', SOH, SOH, recXYmodem, WriteFn))
    != NO_LUCK)
    return RetVal;
    DoCRC = FALSE;
    if ((RetVal = JumpStart(4, 3, NAK, SOH, SOH, recXYmodem, WriteFn))
    != NO_LUCK)
    return RetVal;

    }
  /* From Ward's XMODEM.DOC -- 10 seconds between start tries */
  else
  #endif
  if (protocol == XMDM)
    {
    if ((RetVal = JumpStart((inNet != NON_NET) ? 2 : 4, 10, 'C', SOH,
    SOH, recXYmodem, WriteFn)) != NO_LUCK)
    return RetVal;
    DoCRC = FALSE;
    if ((RetVal = JumpStart(6, 10, NAK, SOH, SOH, recXYmodem, WriteFn))
    != NO_LUCK)
    return RetVal;

    }
  /* Extrapolated from C. Forsberg's doc and WC's doc. */
  else if (protocol == YMDM)
  if ((RetVal = JumpStart(10, 10, 'C', SOH, STX, recXYmodem, WriteFn))
  != NO_LUCK)
  return RetVal;
  return NO_START;

  }
#ifdef WXMODEM_AVAILABLE
/*
* recWX()
*
* This function receives data via WXMODEM.
*/
char recWX(int (*WriteFn)(int c))
  {
  int CurChar,
  LastChar, /* Confirm that SOH follows a SYN */
  lastReceived = 0,
  Sector,
  SectorResult,
  LastError = NO_ERROR,
  LastNak = -1;
  /*
  * Assume that the startup part of the protocol has completed, which would
  * be indicated by a SYN -- already received by us.
  */
  CurChar = SYN;
  do
    {
    DLinkError = NO_ERROR;
    if (inNet == NON_NET) printf("Awaiting block %d\r", lastReceived +1);
    /*
    * Get starting SOH, discarding leading SYN and other characters.
    */
    do
      {
      LastChar = CurChar;
      CurChar = recWXchar(15);

      }
    while (!(CurChar == SOH && LastChar == SYN) && CurChar != EOT &&
    gotCarrier());
    if (CurChar != EOT)
      {
      SectorResult = CommonPacket(WXMDM, 128, recWXchar, &Sector);
      if (SectorResult == NO_ERROR)
        {
        if (Sector == (lastReceived + 1) % 256)
          {
          SectorResult = CommonWrite(WriteFn, SECTSIZE);
          lastReceived = Sector;

          }
        sendWXchar(ACK);
        sendWXchar(Sector & 3);

        }
      if (SectorResult != NO_ERROR)
        {
        /*
        * See Section 6.4.4 of the WXMODEM doc.
        */
        /*      if (!inNet) printf("Error: %s\n", msg[SectorResult]); */
        #ifdef NEED_NET_DEBUG_ERRORS
        splitF(netLog, "Error: %s\n", msg[SectorResult]);
        #endif
        if (SectorResult == CARR_LOSS)
          {
          return TRAN_FAILURE;

          }
        if (SectorResult == WRITE_ERROR)
          {
          sendWXchar(CAN);    /* Cancel transfer - fatal error */
          sendWXchar(CAN);
          sendWXchar(CAN);
          return TRAN_FAILURE;

          }
        if (LastError == NO_ERROR || Sector == LastNak)
          {
          sendWXchar(NAK);
          sendWXchar(Sector & 3);
          LastError = SectorResult;
          LastNak = Sector;

          }

        }
      else if (lastReceived == Sector)
        {
        LastError = NO_ERROR;   /* Clear signal */
        LastNak   = -1;

        }

      }

    }
  while (CurChar != EOT);
  sendWXchar(ACK);    /* Ack the EOT, indicates end of file */
  return TRAN_SUCCESS;

  }
/*
* recWXchar()
*
* This function reads a character, strips any leading DLE.
*/
int recWXchar(int ErrorTime)
  {
  int result;
  if ((result = receive(ErrorTime)) == ERROR)
  return ERROR;
  if (result == DLE)
    {
    DLEsignal = TRUE;
    if ((result = receive(ErrorTime)) == ERROR)
    return ERROR;
    result ^= 64;
    switch (result)
      {
      case DLE:
      case SYN:
      case XON:
      case XOFF: break;
      default:
      DLinkError = BAD_DLE;
      result  = ERROR;
      break;

      }

    }
  else DLEsignal = FALSE;
  return result;

  }
#endif
/*
* recXYmodem()
*
* This function receives data via YMODEM SINGLE.
*      (May also work with XMODEM...)
* Note: algorithm modified to ACK the transmitter before trying
* to write to disk (or wherever).  This should up performance, but
* at the expense code complexity.
*/
char recXYmodem(int (*WriteFn)(int c))  /* Supports YMODEM SINGLE mode only */
  {
  char AbortTransmission = FALSE, WriteError = FALSE;
  int  SectorResult,    /* Result of packet read */
  Sector,   /* Sector # received */
  LastReceived = 0, /* Last sector received */
  TotalErrors = 0,  /* Total errors for this transmission */
  CurrentErrors = 0,  /* Total errors for block */
  SOH_val,    /* SOH indicates 128, STX 1024 */
  BufSize;    /* In conjunction with SOH_val */
  /*
  * Assume that the initial SOH or STX has been received.  An initial EOT
  * should also be handled by the startup function.
  */
  SOH_val = GlobalHeader; /* Set by startup function, is global */
  do
    {
    BufSize = (SOH_val == SOH) ? SECTSIZE : YM_BLOCK_SIZE;
    SectorResult = CommonPacket(YMDM, BufSize, receive, &Sector);
    if (WriteError) SectorResult = WRITE_ERROR;
    if (SectorResult == NO_ERROR)
      {
      if ((Sector == (LastReceived + 1) % 256) ||
      Sector == LastReceived)
      outMod(ACK);
      if (Sector == (LastReceived + 1) % 256)
        {
        WriteError = (CommonWrite(WriteFn, BufSize) == WRITE_ERROR);
        LastReceived++;
        if (inNet == NON_NET)
        printf("Block %d received (try %d, %d total errors)\r",
        LastReceived,
        CurrentErrors,
        TotalErrors);

        }
      else if (Sector != LastReceived)
      SectorResult = SYNCH_ERROR;

      }
    if (SectorResult != NO_ERROR)
      {
      TotalErrors++;
      splitF(netLog, "Error on sector %d: %s (%d)\n", LastReceived+1,msg[SectorResult],CurrentErrors+1);
      switch (SectorResult)
        {
        case SYNCH_ERROR:
        case WRITE_ERROR:
        outMod(CAN);
        outMod(CAN);
        outMod(CAN);    /* Fatal error -- cancel transmission */
        case CARR_LOSS: /* Don't bother with CANs here */
        AbortTransmission = TRUE;
        splitF(netLog, "\nAborting reception due to %s error\n",
        msg[SectorResult]);
        break;
        default:  /* Some normal problem */
        outMod(NAK);
        if (CurrentErrors++ >= 9)
          {
          AbortTransmission = TRUE;
          splitF(netLog,
          "Aborting reception due to 10 consecutive errors\n");

          }

        }

      }
    else
      {
      CurrentErrors = 0;

      }
    if (!AbortTransmission)
      {
      do
      SOH_val = receive(10);
      while (SOH_val != EOT && SOH_val != SOH && SOH_val != CAN &&
      SOH_val != STX && gotCarrier());

      }

    }
  while (SOH_val != EOT && !AbortTransmission && SOH_val != CAN);
  if (AbortTransmission)
  splitF(netLog, "Leaving recXY loop due to AT\n");
  if (!AbortTransmission && SOH_val != CAN )
    {
    outMod(ACK);    /* Ack the EOT, indicates end of file */
    return TRAN_SUCCESS;

    }
  else return TRAN_FAILURE;

  }
/*
* ringSysop()
*
* This function signals a chat mode request.  Exits on input from modem or
* keyboard.
*/
#define RING_LIMIT 6
void ReActivate_Window(void);
void ringSysop()
  {
  char answered;
  int  i, ring;
  ReActivate_Window();
  mPrintf("\n Ringing sysop.\n ");
  answered = FALSE;
  for (ring = 0; ring < RING_LIMIT && !answered && gotCarrier(); ring++)
    {
    for (i=0; !BBSCharReady() && !KBReady(); i = ++i % 7)
      {
      /* play shave-and-a-haircut/two bits... as best we can: */
      oChar(BELL);
      pause(cfg.shave[i]);
      if (i == 6) ring++;

      }
    if (BBSCharReady() || KBReady()) answered = TRUE;

    }
  if (KBReady())
    {
    getCh();
    whichIO = CONSOLE;
    interact(TRUE);
    whichIO = MODEM;

    }
  else if (ring >= RING_LIMIT)
    {
    cfg.BoolFlags.noChat = TRUE;
    mPrintf("\n Sorry, Sysop not around...\n ");

    }
  else modIn();

  }
/*
* SendCmnBlk()
*
* This function sends a WX/X/Y Modem block.
*/
void SendCmnBlk(char type, TransferBlock *block,char (*SendFn)(int c), int size)
  {
  int rover;
  #ifndef NEED_NET_DEBUG_ERRORS
  if (inNet == NON_NET)
  printf("Sending block %d\r", block->ThisBlock);
  #else
  splitF(netLog, "Sending block %d, type %d, size %d\n", block->ThisBlock, type, size);
  #endif
  #ifdef WXMODEM_AVAILABLE
  if (type == WXMDM)
    {
    /* Thrust out some SYNs first */
    fastMod(SYN);
    fastMod(SYN);

    }
  else
  #endif
  BufferingOn();
  (*SendFn)((size == SECTSIZE) ? SOH : STX);
  (*SendFn)(block->ThisBlock & 0xFF);
  (*SendFn)(~(block->ThisBlock & 0xff));
  for (rover = 0; rover < size; rover++)
    {
    (*SendFn)(block->buf[rover]);
    #ifdef WXMODEM_AVAILABLE
    if (type == WXMDM) WXResponses();
    #endif
    if (!gotCarrier())
      {
      TrError = CARR_LOSS;
      splitF(netLog, "SendCmnBlk lost carrier\n");
      BufferingOff();
      return ;

      }

    }
  /*
  * Handle CRC/Checksum stuff.
  */
  if (DoCRC)
    {
    (*SendFn)(((block->ThisCRC & 0xff00) >> 8));

    }
  (*SendFn)(block->ThisCRC & 0xff);
  BufferingOff();

  }
/*
* sendWCChar()
*
* This function sends a file using Ward Christensen's protocol.
* (i.e., compatable with xModem, modem7, modem2, YAM, ... )
*/
int sendWCChar(int c)
  {
  if (TrError != TRAN_SUCCESS)
  return FALSE;
  blk.buf[TrCount++] = c;
  TrCksm = (TrCksm + c) & 0xFF;
  if (TrCount != SECTSIZE)
  return TRUE;
  blk.ThisBlock = TrBlock;
  blk.ThisCRC   = (DoCRC) ? calcrc(blk.buf, SECTSIZE) : TrCksm;
  return (XYBlock(XMDM, SECTSIZE));

  }
#ifdef WXMODEM_AVAILABLE
/*
* sendWXchar()
*
* This function sends a char via WXMODEM with appropriate leaders, etc.
*/
char sendWXchar(int data)
  {
  data &= 0xff;
  switch (data)
    {
    case DLE:
    case SYN:
    case XON:
    case XOFF:
    fastMod(DLE);
    fastMod(data ^ 64);
    break;
    default:
    fastMod(data);
    break;

    }
  return 0;

  }
/*
* sendWXModem()
*
* This function sends a character using WXMODEM protocol.
*/
int sendWXModem(int c)
  {
  int BlockRover, TempSent, LoopCount;
  /*
  * First, check for terminal failure.
  */
  if (TrError != TRAN_SUCCESS) return FALSE;
  /*
  * Process current character.
  */
  Twindow[CurWindow].buf[TrCount] = c;
  /*
  * Block is full, so clean up details in preparation for transmission.
  */
  if (++TrCount == SECTSIZE)
    {
    Twindow[CurWindow].ThisBlock = TrBlock;
    Twindow[CurWindow].ThisCRC = calcrc(Twindow[CurWindow].buf, SECTSIZE);
    Twindow[CurWindow].status = SECTOR_READY;   /* Set to go */
    /* TrBlock = (TrBlock + 1) % 256; line below should work -- see SendCmnBlk */
    TrBlock++;
    CurWindow = (CurWindow + 1) % 4;
    TrCount = 0;

    }
  /*
  * Check for responses from receiver.
  */
  WXResponses();
  /*
  * Now we need to send outstanding blocks.
  */
  for (    BlockRover = (LastSent + 1) % 4, TempSent = LastSent;
  BlockRover != LastSent; BlockRover = (BlockRover + 1) % 4)
    {
    if (Twindow[BlockRover].status != SECTOR_READY) break;
    SendCmnBlk(WXMDM, Twindow + BlockRover, sendWXchar, SECTSIZE);
    Twindow[BlockRover].status = SENT;
    TempSent = BlockRover;
    if (!gotCarrier())
      {
      break;

      }

    }
  LastSent = TempSent;
  /*
  * Now we need to check to see if the Twindow is "full"
  */
  for (LoopCount = 0; gotCarrier() && WindowFull() &&
  LoopCount < MAX_WX_ERRORS;
  LoopCount++)
    {
    startTimer(WORK_TIMER);
    do
    WXResponses();
    while (chkTimeSince(WORK_TIMER) < 10 && WindowFull());
    if (WindowFull())
    SendCmnBlk(WXMDM, Twindow + LastSent, sendWXchar, SECTSIZE);

    }
  /*
  * If the Twindow is still full at this point, then we've suffered some
  * sort of fatal error, and it's time to bomb out.
  */
  if (WindowFull() || !gotCarrier())
    {
    TrError = TRAN_FAILURE;
    return FALSE;

    }
  return TRUE;

  }
#endif
/*
* sendYMChar()
*
* This function sends a character using YMODEM protocol.
*/
int sendYMChar(int c)
  {
  if (TrError != TRAN_SUCCESS)
  return FALSE;
  blk.buf[TrCount++] = c;
  if (TrCount != CurYBufSize)
  return TRUE;
  return SendYBlk();

  }
/*
* SendYBlk()
*
* This function sends a YMODEM block.
*/
int SendYBlk()
  {
  blk.ThisBlock = TrBlock;
  blk.ThisCRC   = calcrc(blk.buf, CurYBufSize);
  return XYBlock(YMDM, CurYBufSize);

  }
/*
* SummonSysop()
*
* This function rings the sysop for ^T.
*/
void SummonSysop()
  {
  int i;
  CallSysop = FALSE;
  DisableModem(FALSE);
  ReActivate_Window();  /* allow window to be activated and to the front */
  printf("SYSOP: System available!  Hit space!\n");
  for (i = 0; i < 12 && !KBReady(); i++)
    {
    onConsole = TRUE;
    printf("%c", BELL);
    onConsole = FALSE;
    startTimer(WORK_TIMER);
    while (!KBReady() && chkTimeSince(WORK_TIMER) < 10l)
    ;

    }
  if (KBReady())
    {
    getCh();
    printf("CONSOLE mode\n ");
    whichIO = CONSOLE;
    setUp(FALSE);
    ScrNewUser();
    warned  = FALSE;
    logMessage(BAUD, "", FALSE);

    }
  else
    {
    printf("No answer.  System back on MODEM.\n");
    EnableModem(FALSE);

    }
  givePrompt();

  }
/*
* Transmission()
*
* Starts protocols up.
* Note: This only handles XMODEM, WXMODEM, and YMODEM.
*/
char Transmission(int protocol, char mode)  /* Transmission handler */
  {
  int Errors, m;
  if (!gotCarrier() && protocol != ASCII)
    {
    #ifdef NEED_NET_DEBUG_ERRORS
    if (inNet != NON_NET)
    splitF(netLog, "Error: Transmission() got carrier loss\n");
    #endif
    return CARR_LOSS;

    }
  if (protocol == ASCII) return TRAN_SUCCESS;
  if (mode == STARTUP)
    {
    GenTrInit();
    ByteCount = 0l;
    for (Errors = 0; Errors < ERRORMAX; Errors++)
      {
      m = receive(MINUTE);
      switch (m)
        {
        case CAN:
        if (cfg.BoolFlags.debug) splitF(netLog, "Transmission() CANned\n");
        return CANCEL;
        case ERROR:
        if (cfg.BoolFlags.debug) splitF(netLog, "Transmission() did not start\n");
        return NO_START;
        case NAK:
        TransProtocol = XMDM;
        DoCRC = FALSE;
        return TRAN_SUCCESS;
        #ifdef WXMODEM_AVAILABLE
        case 'W':
        if (protocol == WXMDM)
          {
          TransProtocol = WXMDM;
          return TRAN_SUCCESS;

          }
        break;
        #endif
        case 'C':
        if (protocol == YMDM)
          {
          /* Kludge for floopy performance */
          CurYBufSize = 1024;
          TransProtocol = YMDM;
          return TRAN_SUCCESS;

          }
        TransProtocol = XMDM;
        return TRAN_SUCCESS;
        default:
        if (cfg.BoolFlags.debug) splitF(netLog, "TrStart: -%c (0x%x)-\n", m, m);

        }

      }
    if (cfg.BoolFlags.debug) splitF(netLog, "returning NO_START on loop exit.\n");
    return NO_START;    /* If we make it out of the loop, error */

    }
  else
    {
    if (inNet == NON_NET) printf("\n");
    if (TrError != TRAN_SUCCESS)
      {
      splitF(netLog, "TranFin found TrError was bad immediately\n");
      return TrError;

      }
    if (TransProtocol == YMDM && TrCount < SECTSIZE)
    CurYBufSize = SECTSIZE;
    while (TrCount != 0)
      {
      (*Table[TransProtocol].method)(' ');

      }
    if (TrError != TRAN_SUCCESS)
      {
      splitF(netLog, "TranFin found TrError after buffer fill\n");
      return TrError;

      }
    if (Table[TransProtocol].CleanUp != NULL)
      {
      return (char) (*Table[TransProtocol].CleanUp)();

      }
    TransProtocol = ASCII;  /* Return to normal */
    return TRAN_SUCCESS;

    }
  }
#ifdef WXMODEM_AVAILABLE
/*
* WXResponses()
*
* Handles ACK/NAK and XON/XOFF for WXMODEM.
*/
void WXResponses()
  {
  int rover, sig, old, SeqReceived;
  if (MIReady())
    {
    sig = recWXchar(1);
    switch (sig)
      {
      case XOFF:
      FlowControl();
      break;
      case ACK:
      case NAK:
      SeqReceived = recWXchar(5) & 0x03;
      if (sig == ACK)
        {
        rover = StartWindow;
        do
          {
          Twindow[rover].status = ACKED;
          old = rover;
          rover = (rover + 1) % 4;

          }
        while (old != SeqReceived);
        StartWindow = rover;    /* First block of Twindow */

        }
      else
        {
        /* NAK */
        LastSent = (SeqReceived == 0) ? 3 : (SeqReceived - 1);
        rover = SeqReceived;
        do
          {
          Twindow[rover].status = SECTOR_READY;
          rover = (rover + 1) % 4;

          }
        while (rover != CurWindow);

        }
      break;

      }

    }

  }
#endif
/*
* XYBlock()
*
* This function handles common work of XMODEM and YMODEM.
*/
char XYBlock(int mode, int size)
  {
  int i, m = CAN;
  #ifdef NEED_NET_DEBUG
  splitF(netLog, "In XYBlock\n");
  #endif
  ByteCount += size;
  for (i = 0; i < ERRORMAX; i++)
    {
    SendCmnBlk(mode, &blk, outMod, size);
    while (MIReady()) inp();  /* clear line */
    m = receive(10);  /* wait 10 seconds for return ACK/NAK */
    if (m == ACK || m == CAN || !gotCarrier()) break;
    #ifdef NEED_NET_DEBUG_ERRORS
    if (inNet != NON_NET)
    splitF(netLog, "Tr Resend (m=%c [%x])\n", m, m);
    #endif

    }
  TrCksm  = TrCount = 0;
  TrBlock++;
  if (m == ACK)
    {
    #ifdef NEED_NET_DEBUG_ERRORS
    if (inNet != NON_NET)
    splitF(netLog, "Ack on block\n");
    #endif
    return TRUE;

    }
  TrError = TRAN_FAILURE;
  if (inNet == NON_NET) printf("Aborting\n ");
  else splitF(netLog, "XYBlock aborting (%s)\n", (gotCarrier()) ? "10 NAKs" :
  "carrier loss");
  return FALSE;

  }
/*
* XYClear()
*
* This function finishes XMODEM and YMODEM transmission.
*/
int XYClear()
  {
  int i, m;
  for (i = 0; gotCarrier() && i < ERRORMAX; i++)
    {
    #ifdef NEED_NET_DEBUG
    splitF(netLog, "Sending EOT\n");
    #endif
    outMod(EOT);
    if ((m = receive(10)) == ACK)
    return TRAN_SUCCESS;
    if (m == CAN)
    return TRAN_FAILURE;

    }
  return TRAN_FAILURE;

  }
/*
* YMHdr()
*
* This function sends the header for a YMODEM BATCH transmission.
*/
int YMHdr(long fileSize, char *filename)
  {
  extern int (*ITLFunc)(int c);
  TrBlock    = 0; /* One a kludge, two a kludge, three a kludge .. */
  ITLFunc = sendYMChar;
  mTrPrintf("%s", filename);
  mTrPrintf("%ld", fileSize);
  if (TrCount < 128)
  CurYBufSize = 128;
  if (TrCount != 128)
  for (; TrCount != 0 && sendYMChar(0); )
  ;
  else
  SendYBlk();
  if (TrError != TRAN_SUCCESS) return FALSE;
  if (strLen(filename) != 0)
  Transmission(YMDM, STARTUP);  /* now restart protocol *sigh* */
  return TrError == TRAN_SUCCESS;

  }
#ifdef NEED_NET_DEBUG_ERRORS
void DumpToFile(int LastReceived, int BufSize, CRC_TYPE tc, CRC_TYPE oc)
  {
  int i, j;
  FILE *fd;
  extern NetBuffer netBuf;
  if (strCmpU(netBuf.netName, "Images") == 0)
    {
    if ((fd = fopen("badsector", "a")) != NULL)
      {
      fprintf(fd, "Error on sector %d.  Their crc is %x, our's %x. Contents:\n", LastReceived, tc, oc);
      for (i = 0; i < BufSize; i += 16)
        {
        fprintf(fd, "%2x:", i);
        for (j = i; j < i + 16; j++)
        fprintf(fd, " %02x", DataBuf[j]);
        fprintf(fd, "   ");
        for (j = i; j < i + 16; j++)
        fprintf(fd, "%c", isprint(DataBuf[j]) ? DataBuf[j] : '.');
        fprintf(fd, "\n");

        }
      fprintf(fd, "\n");
      fclose(fd);

      }

    }

  }
#endif
