/* MicroBBS - fuer alle Bloedel, die einen Mailer ancallen... */

#define DEBUG

#include    "MicroBBS.h"

//+ Consts

const char *DefaultFont     = "IBM.font";

#ifdef DEBUG
const char  *Version        = "$VER: MicroBBS 1.21ß (05.05.96)";
#else
const char  *Version        = "$VER: MicroBBS 1.21 (05.05.96)";
#endif

const char  *RD_Template    =
{
    "D=DEVICE,"
    "U=UNIT/N,"
    "B=BAUD/N,"
    "N=NODE/N,"
    "L=LOCAL/S,"
    "C=CONNECT,"
    "O=CONFIG,"
    "E=EXCLUSIVE/S"
};
//-
//+ Variables
char    UserName[ STRING_LENGTH ]        = "Gast";
char    TempString[ STRING_LENGTH ];
char    FullString[ COMMAND_LENGTH];
char    FullString2[ COMMAND_LENGTH];
char    SpawnPortName[ STRING_LENGTH ];

char    LogFileName[ STRING_LENGTH ]    = "BBS:MicroBBS.log";
char    ScreenName[ STRING_LENGTH ]     = "MicroBBS";
char    ConsolePubScreenName[ STRING_LENGTH ];

char    *StatsFileName  = "BBS:MicroBBS.stats";
char    *ConfigFileName = "BBS:MicroBBS.cfg";
char    *DeviceName     = "serial.device";
char    *Prompt         = "[1A[1E%H:%M:%S> ";

char    *SerialColor[ 20 ];
char    *ConsoleColor[ 20 ];

char    SerialChar;
char    SerialReadChar;
char    LastSerialChar;
char    ConsoleChar;
char    ConsoleReadChar;

char    InputChar;

time_t  OTime;

struct Library  *MetaXprBase    = NULL;
struct Library  *TimerBase      = NULL;

struct MsgPort      *SpawnPort  = NULL;
struct SpawnMessage *SpawnMessage;

struct MsgPort  *SerialReadMP  = NULL;
struct IOExtSer *SerialReadIO  = NULL;
struct MsgPort  *SerialWriteMP = NULL;
struct IOExtSer *SerialWriteIO = NULL;

struct MsgPort      *ConsoleReadMP  = NULL;
struct IOStdReq     *ConsoleReadIO  = NULL;
struct MsgPort      *ConsoleWriteMP = NULL;
struct IOStdReq     *ConsoleWriteIO = NULL;

struct MsgPort      *TimerMP = NULL;
struct timerequest  *TimerIO = NULL;

struct Screen       *Screen         = NULL;
struct Screen       *AlienPubScreen = NULL;
struct Window       *ConsoleWindow  = NULL;

struct TextFont     *ConsoleFont    = NULL;
struct TextAttr     ConsoleFontAttr =
{
    DefaultFont,
    8, 0, 0
};

struct RDArgs   *RD_Struct  = NULL;

struct UserStruct   User;
struct StatsStruct  Stats;

struct List *ConfigList = NULL;
struct List *UserList   = NULL;
struct List *StatsList  = NULL;

struct Process          *MyTask;
struct Window           *OldWindowPtr;

struct FileInfoBlock    *ConfigFileInfoBlock    = NULL;
struct FileInfoBlock    *KeyFileInfoBlock       = NULL;

long    Node    = 0;
long    Unit    = 0;
long    Baud    = 38400;

long    SerialDevice    = TRUE;
long    ConsoleDevice   = TRUE;
long    TimerDevice     = TRUE;

long    ConsoleLeft   = 50;
long    ConsoleTop    = 50;
long    ConsoleWidth  = 480;
long    ConsoleHeight = 300;

long    ScreenWidth  = 640;
long    ScreenHeight = 480;
long    ScreenDepth  = 4;
long    ScreenID     = 0;

UWORD ColorList[] =
{
    0x00, 0x0, 0x0, 0x0,
    0x01, 0xf, 0x0, 0x0,
    0x02, 0x0, 0xf, 0x0,
    0x03, 0xf, 0xf, 0x0,
    0x04, 0x0, 0x0, 0xf,
    0x05, 0xf, 0x0, 0xf,
    0x06, 0x0, 0xf, 0xf,
    0x07, 0xf, 0xf, 0xf,
    0x08, 0x6, 0x6, 0x6,
    0x09, 0xa, 0x0, 0x0,
    0x0a, 0x0, 0xa, 0x0,
    0x0b, 0xa, 0x6, 0x0,
    0x0c, 0x0, 0x0, 0xa,
    0x0d, 0xa, 0x0, 0xa,
    0x0e, 0x0, 0xa, 0xa,
    0x0f, 0xc, 0xc, 0xc,
      -1, 0x0, 0x0, 0x0
};

UWORD   ScreenPens[] =
{
    0x00,        /* compatible Intuition rendering pens  */
    0x00,        /* compatible Intuition rendering pens  */
    0x00,        /* text on background                   */
    0x07,        /* bright edge on 3D objects            */
    0x08,        /* dark edge on 3D objects              */
    0x0e,        /* active-window/selected-gadget fill   */
    0x00,        /* text over FILLPEN                    */
    0x0f,        /* always color 0                       */
    0x07,        /* special color text, on background    */
    0x00,        /* text/detail in screen-bar/menus */
    0x07,        /* screen-bar/menus fill */
    0x00,        /* trim under screen-bar */
    -1,                 -1
};

BOOL    Key      = FALSE;
BOOL    DoScreen = FALSE;

APTR    RD_Array[ RD_Size ];

APTR    ConfigFileBuffer    = NULL;
char    *KeyFileBuffer      = NULL;

BPTR    ConfigFileHandle    = NULL;
BPTR    KeyFileHandle       = NULL;
BPTR    LogFileHandle       = NULL;

BPTR    StdInputHandle;
BPTR    StdOutputHandle;
//-
//+ Declarations

void BatchDownload( char *);
void ChangeMaxLines();
void ChangePassword();
void Chatter();
void Close_All( int);
void FPrintfLog( char *, ...);
void FPutsLog( char *);
void GetLine( char *);
void GetLines( char *);
void main();
void MainChatter();
void Open_All();
void Upload();
//-

//+ -Internal-
//+ -Chatter-
//+ void Chatter()
void Chatter()
{
    long    Count = 20;
    char    DummyChar;

    FPutsLog( "Calling for sysop...");

    PutString( "Calling for sysop");

    while( Count--)
    {
        if( WaitChar( 1000000))
        {
            ConsoleChar = 0;
            DGetChar( &DummyChar);

            if( ConsoleChar == DummyChar) MainChatter();

            return;
        }

        PutString( " .\a");
    }

    PutString( "\r\nSysop is not available.\r\n");
} //-
//+ void MainChatter()
void MainChatter()
{
    long    CharCount   = MAX_LINE_LENGTH;
    char    InputChar;

    FPutsLog( "Chat...");

    Type( "chatter.start.txt");

    if( !( ConsoleDevice) && !( SerialDevice))
    {
        do
        {
            ConsoleChar = 0;
            DGetChar( &InputChar);

            if( ConsoleChar) PutString( ConsoleColor);
            else             PutString( SerialColor);

            switch( InputChar)
            {
                case 0x03 : break;

                case 0x08 :
                {
                    if( CharCount < MAX_LINE_LENGTH)
                    {
                        CharCount++;

                        PutChar( 0x08);
                        PutChar( 0x20);
                        PutChar( 0x08);
                    }
                    else
                    {
                        PutStringF( "[1A[%ldC", MAX_LINE_LENGTH);
                        CharCount = 1;

                        PutChar( 0x20);
                        PutChar( 0x08);
                    }

                    break;
                }

                case 0x0a :
                {
                    CharCount = MAX_LINE_LENGTH;

                    PutString( "\r\n");

                    break;
                }

                default :
                {
                    if( !( CharCount--))
                    {
                        CharCount = MAX_LINE_LENGTH - 1;

                        PutString( "\r\n");
                    }

                    PutChar( InputChar);

                    break;
                }
            }
        }
        while( ConsoleChar != 0x03);

        PutString( "[0m\r\n");
    }
    else PutString( "Chat is not possible in local mode.\r\n");

    Type( "chatter.end.txt");
    WaitReturn();

    FPutsLog( "...end.");
} //-
//-
//+ -Transfer-
//+ void BatchDownload( char *FileName)
void BatchDownload( char *FileName)
{
    BPTR    FListFileHandle;

    struct  FileInfoBlock   *FListFileInfoBlock;

    struct  Screen      *TransferScreen;
    
    long    Result;
    long    FileCount = 0;

    char    *FListFileBuffer;
    char    *FileNamePointer;
    char    *FileNamePointers[ 256 ];

    FPutsLog( "Download:");

    PutString( "\r\nSending...\r\n");

    if( FListFileHandle = Open( FileName, MODE_OLDFILE))
    {
        if( FListFileInfoBlock = ( long *) AllocVec( sizeof( struct FileInfoBlock), MEMF_ANY))
        {
            if( ExamineFH( FListFileHandle, FListFileInfoBlock))
            {
                if( FListFileBuffer = ( long *) AllocVec( FListFileInfoBlock->fib_Size + 2, MEMF_ANY|MEMF_CLEAR))
                {
                    if( FListFileInfoBlock->fib_Size == Read( FListFileHandle, FListFileBuffer, FListFileInfoBlock->fib_Size))
                    {
                        FileNamePointer = FListFileBuffer;
        
                        while( *FileNamePointer)
                        {
                            FileNamePointers[ FileCount ] = FileNamePointer;

                            while( *FileNamePointer != 0x00 && *FileNamePointer != 0x0a)
                                FileNamePointer++;

                            *FileNamePointer++ = 0x00;

                            FPutsLog ( FileNamePointers[ FileCount ]);
                            FileCount++;
                        }

                        FileNamePointers[ FileCount] = 0x00;

                        if( FileCount)
                        {
                            if( !( SerialDevice))
                            {
                                AbortIO( SerialReadIO);
                                WaitIO( SerialReadIO);
                                AbortIO( SerialWriteIO);
                                WaitIO( SerialWriteIO);

                                SerialWriteIO->IOSer.io_Command = CMD_CLEAR;
                                DoIO( SerialWriteIO);

                                if( !( Result = TransferSetupShared( SerialReadIO, SerialWriteIO, "xprzmodem.library")))
                                {
                                    if( AlienPubScreen) TransferScreen = AlienPubScreen;
                                    else                TransferScreen = Screen;

                                    if( !( Result = SendMultipleFiles( FileNamePointers, 2L, TransferScreen)))
                                        FPrintfLog( "Transfer failed, error %d.", &Result);
                                    else
                                    {
                                        ( *User.us_Files)   += FileCount;
                                        WriteUser();
                                        ( *Stats.st_Files)  += FileCount;
                                        WriteStats();
                                    }
                                }
                                else
                                {
                                    FPrintfLog( "Transfersetup failed, error %d.", &Result);
                                }

                                SerialWriteIO->IOSer.io_Command = CMD_CLEAR;
                                DoIO( SerialWriteIO);

                                SerialReadIO->IOSer.io_Command  = CMD_READ;
                                SerialReadIO->IOSer.io_Length   = 1;
                                SerialReadIO->IOSer.io_Data     = &SerialReadChar;
                                SendIO( SerialReadIO);                              /* setup first request! Important! */
                            }
                        }
                    }
                    FreeVec( FListFileBuffer);
                }
            }
            FreeVec( FListFileInfoBlock);
        }
        Close( FListFileHandle);
    }

    FPutsLog( "...end.");
} //-
//+ void Upload()
void Upload()
{
    long    Result;

    struct  Screen      *TransferScreen;

    char *FileName = "Upload.tmp";

    CurrentDir( Lock( "BBS:T/", ACCESS_READ));

    FPutsLog( "Upload...");

    PutString( "\r\nUpload starten...\r\n");

    if( !( SerialDevice))
    {
        AbortIO( SerialReadIO);
        WaitIO( SerialReadIO);
        AbortIO( SerialWriteIO);
        WaitIO( SerialWriteIO);

        SerialWriteIO->IOSer.io_Command = CMD_CLEAR;
        DoIO( SerialWriteIO);

        if( !( Result = TransferSetupShared( SerialReadIO, SerialWriteIO, "xprzmodem.library")))
        {
            if( AlienPubScreen) TransferScreen = AlienPubScreen;
            else                TransferScreen = Screen;

            if( !( Result = ReceiveFile( FileName, 2L, TransferScreen)))
                FPrintfLog( "Transfer failed, error %d.", &Result);
            else
                FPutsLog( FileName);
        }
        else FPrintfLog( "Transfersetup failed, error %d.", &Result);

        SerialWriteIO->IOSer.io_Command = CMD_CLEAR;
        DoIO( SerialWriteIO);

        SerialReadIO->IOSer.io_Command  = CMD_READ;
        SerialReadIO->IOSer.io_Length   = 1;
        SerialReadIO->IOSer.io_Data     = &SerialReadChar;
        SendIO( SerialReadIO);                              /* setup first request! Important! */
    }

    CurrentDir( GetProgramDir());

    FPutsLog( "...end.");
}
//-
//-
//+ -Prefs-
//+ void ChangeMaxLines()
void ChangeMaxLines()
{
    PutString( "\r\nMaximale Zeilenzahl [0 = unendlich]: ");
    GetString( TempString);

    *User.us_MaxLines = atol( TempString);
    WriteUser();
} //-
//+ void ChangePassword()
void ChangePassword()
{
    char    Password[ STRING_LENGTH ];

    do
    {
        PutString( "\r\nNeues Password> ");

        do
        {
            GetStringHidden( User.us_Password);
        }
        while( strlen( User.us_Password) < 3);

        PutString( "\r\nNochmal> ");
        GetStringHidden( Password);
    }
    while( StrCmpNoCase( Password, User.us_Password));

    WriteUser();
} //-
//-
//+ -Input-
//+ void GetLine( char *FileName)
void GetLine( char *FileName)
{
    BPTR    FileHandle;

    char    String[ STRING_LENGTH ];

    if( FileHandle = Open( FileName, MODE_NEWFILE))
    {
        PutString( "> ");
        GetString( String);
        FPuts( FileHandle, String);
        Close( FileHandle);
    }
    else PutString( "\r\nSystemfehler - sorry...\r\n");
} //-
//+ void GetLines( char *FileName)
void GetLines( char *FileName)
{
    BPTR    FileHandle;

    char    String[ STRING_LENGTH ];

    if( FileHandle = Open( FileName, MODE_NEWFILE))
    {
        do
        {
            PutString( "> ");
            GetString( String);

            if( strlen( String))
            {
                FPuts( FileHandle, String);
                FPutC( FileHandle, 0x0a);
            }
        }
        while( strlen( String) && Key);

        Close( FileHandle);
    }
    else PutString( "\r\nSystemfehler - sorry...\r\n");
} //-
//-
//-

//+ void Open_All()
void Open_All()
{
    long    Position1 = 0x00;
    long    Position2 = 0x00;

    long    CheckSum = 0x00;
    long    Count;

    MyTask = ( struct *Task) FindTask( NULL);

    OldWindowPtr = MyTask->pr_WindowPtr;
    MyTask->pr_WindowPtr = ( APTR) -1;

    if( !( StdInputHandle = Input()))
        Close_All( FAIL);

    if( !( StdOutputHandle = Output()))
        Close_All( FAIL);

    if( !( MetaXprBase = ( struct Library *) OpenLibrary( "metaxpr.library", 0L)))
    {
        FPutsLog( "can`t open metaxpr.library!");
        Close_All( FAIL);
    }

//+   Keyfile
    #ifdef DEBUG
    FPutsLog( "checking keyfile.");
    #endif

    if( KeyFileHandle = Open( "BBS:MicroBBS.key", MODE_OLDFILE))
    {
        if( KeyFileInfoBlock = ( long *) AllocVec( sizeof( struct FileInfoBlock), MEMF_ANY|MEMF_CLEAR))
        {
            if( ExamineFH( KeyFileHandle, KeyFileInfoBlock))
            {
                if( KeyFileBuffer = ( long *) AllocVec( KeyFileInfoBlock->fib_Size, MEMF_ANY|MEMF_CLEAR))
                {
                    if( KeyFileInfoBlock->fib_Size > 0x04)
                    {
                        if( KeyFileInfoBlock->fib_Size - 4 == Read( KeyFileHandle, KeyFileBuffer, KeyFileInfoBlock->fib_Size - 4))
                        {
                            Read( KeyFileHandle, &CheckSum, 4);

                            Count = KeyFileInfoBlock->fib_Size - 4;

                            while( Count--)
                            {
                                KeyFileBuffer[ Count ] = KeyFileBuffer[ Count ] ^ KeyFileBuffer[ Count + 1 ];
                                CheckSum -= KeyFileBuffer[ Count ];
                            }
                            if( !CheckSum) Key = TRUE;
                        }
                    }
                }
            }
            FreeVec( KeyFileInfoBlock);
            KeyFileInfoBlock = NULL;
        }
        Close( KeyFileHandle);
        KeyFileHandle = NULL;
    }
//-

    if( !( RD_Struct = ReadArgs( RD_Template, RD_Array, NULL)))
    {
        FPutsLog( "required argument missing!");
        Close_All( FAIL);
    }

    CheckArgs();

//+   SpawnPort
    #ifdef DEBUG
    FPutsLog( "opening spawnport.");
    #endif

    sprintf( SpawnPortName, "MicroBBS SpawnPort Unit %lu", Node);

    Forbid();

    if( FindPort( SpawnPortName))
    {
        Permit();

        FPrintf( StdOutputHandle, "port %lu is already running!\n", Node);
        Close_All( FAIL);
    }

    if( !( SpawnPort = CreateMsgPort()))
    {
        Permit();

        FPutsLog( "can't create spawnport!");
        Close_All( FAIL);
    }

    SpawnPort->mp_Node.ln_Name = SpawnPortName;
    AddPort( SpawnPort);

    Permit();
//-
//+   SerialDevice
    #ifdef DEBUG
    FPutsLog( "opening serial.device.");
    #endif

    if( !( SerialReadMP = CreateMsgPort()))
    {
        FPutsLog( "can`t create serialport!");
        Close_All( FAIL);
    }

    if( !( SerialWriteMP = CreateMsgPort()))
    {
        FPutsLog( "can`t create serialport!");
        Close_All( FAIL);
    }

    if( !( SerialReadIO = ( struct IOExtSer *) CreateExtIO( SerialReadMP, sizeof( struct IOExtSer))))
    {
        FPutsLog( "can`t create serialio!");
        Close_All( FAIL);
    }

    if( !( SerialWriteIO = ( struct IOExtSer *) CreateExtIO( SerialWriteMP, sizeof( struct IOExtSer))))
    {
        FPutsLog( "can`t create serialio!");
        Close_All( FAIL);
    }

    if( !( RD_Array[ LOCAL ]))
    {
        if( RD_Array[ EXCLUSIVE ])  SerialWriteIO->io_SerFlags = 0;
        else                        SerialWriteIO->io_SerFlags = SERF_SHARED;

        if( SerialDevice = OpenDevice(( char *) RD_Array[ DEVICE ], Unit, SerialWriteIO, 0))
        {
            FPrintf( StdOutputHandle, "can`t open %s unit %d!\n", RD_Array[ DEVICE ], Unit);
            Close_All( FAIL);
        }

        SerialWriteIO->IOSer.io_Command = SDCMD_SETPARAMS;
        SerialWriteIO->io_SerFlags     &= ~SERF_PARTY_ON;
        SerialWriteIO->io_SerFlags     |= SERF_XDISABLED;
        /* SerialWriteIO->io_SerFlags     |= SERF_RAD_BOOGIE; */
        SerialWriteIO->io_Baud          = Baud;

        if( !( RD_Array[ EXCLUSIVE ]))  SerialWriteIO->io_SerFlags  |= SERF_SHARED;

        DoIO( SerialWriteIO);

        CopyMem( SerialWriteIO, SerialReadIO, sizeof( struct IOExtSer));
        SerialReadIO->IOSer.io_Message.mn_ReplyPort = SerialReadMP;

        SerialReadIO->IOSer.io_Command  = CMD_READ;
        SerialReadIO->IOSer.io_Length   = 1;
        SerialReadIO->IOSer.io_Data     = &SerialReadChar;
        SendIO( SerialReadIO);                              /* setup first request! Important! */
    }
//-
//+   TimerDevice
    #ifdef DEBUG
    FPutsLog( "opening timer.device.");
    #endif

    if( !( TimerMP = CreateMsgPort()))
    {
        FPutsLog( "can`t create port!");
        Close_All( FAIL);
    }

    if( !( TimerIO = ( struct timerequest *) CreateExtIO( TimerMP, sizeof( struct timerequest))))
    {
        FPutsLog( "can`t create timerio!");
        Close_All( FAIL);
    }

    if( TimerDevice = OpenDevice( TIMERNAME, UNIT_VBLANK, TimerIO, 0))
    {
        FPrintf( StdOutputHandle, "can`t open %s unit %d!\n", TIMERNAME, UNIT_VBLANK);
        Close_All( FAIL);
    }

    TimerBase = ( struct Library *) TimerIO->tr_node.io_Device;
//-

    PutString( "\r\nWelcome to ");
    PutString( &Version[ 6 ]);

    if( Key)
    {
        Position1 = KeyFileBuffer[ 0 ] + 1;
        Position2 = Position1 + KeyFileBuffer[ Position1 ] + 1;
        KeyFileBuffer[ Position1 ] = 0x00;
        KeyFileBuffer[ Position2 ] = 0x00;

        PutString( "\r\nOwner: ");
        PutString( &KeyFileBuffer[ 1 ]);
        PutString( ". Serial: #");
        PutString( &KeyFileBuffer[ Position1 + 1 ]);
        PutString( ".\r\n");
    }
    else
    {
        PutString( " unregistered.\r\n");
        PutString( "\r\nPlease ask your sysop to register his shareware.\r\n");
        PutString( "\r\nSupporting shareware is like supporting yourself.\r\n");
    }

    PutString( "\r\nStarting BBS...\r\n");

    #ifdef DEBUG
    FPutsLog( "reading config.");
    #endif

    if( RD_Array[ CONFIG ]) ReadConfig(( long *) RD_Array[ CONFIG ], &ConfigList);
    else                    ReadConfig( ConfigFileName, &ConfigList);

    #ifdef DEBUG
    FPutsLog( "analysing config.");
    #endif

    AnalyseConfig();

//+   Screen
    #ifdef DEBUG
    FPutsLog( "opening screen.");
    #endif

    if( !( AlienPubScreen = LockPubScreen( ScreenName)))
    {
        if( Screen = OpenScreenTags( NULL,
                                     SA_Width,          ScreenWidth,
                                     SA_Height,         ScreenHeight,
                                     SA_Depth,          ScreenDepth,
                                     SA_DisplayID,      ScreenID,
                                     SA_AutoScroll,     TRUE,
                                     SA_PubName,        ScreenName,
                                     SA_Title,          Version + 5,
                                     SA_Pens,           ScreenPens,
                                     SA_Colors,         ColorList,
                                     SA_Behind,         TRUE,
                                     TAG_DONE))
        {
            PubScreenStatus( Screen, 0);
        }
    }
//-
//+   ConsoleDevice
    #ifdef DEBUG
    FPutsLog( "opening console.device.");
    #endif

    if( !( ConsoleReadMP = CreateMsgPort()))
    {
        FPutsLog( "can`t create consoleport!");
        Close_All( FAIL);
    }

    if( !( ConsoleWriteMP = CreateMsgPort()))
    {
        FPutsLog( "can`t create consoleport!");
        Close_All( FAIL);
    }

    if( !( ConsoleReadIO = ( struct IOStdReq *) CreateExtIO( ConsoleReadMP, sizeof( struct IOStdReq))))
    {
        FPutsLog( "can`t create consoleio!");
        Close_All( FAIL);
    }

    if( !( ConsoleWriteIO = ( struct IOStdReq *) CreateExtIO( ConsoleWriteMP, sizeof( struct IOStdReq))))
    {
        FPutsLog( "can`t create consoleio!");
        Close_All( FAIL);
    }

    if( !( ConsoleWindow = OpenWindowTags( NULL,
                                           WA_Left,                 ConsoleLeft,
                                           WA_Top,                  ConsoleTop,
                                           WA_Width,                ConsoleWidth,
                                           WA_Height,               ConsoleHeight,
                                           WA_MinWidth,             64,
                                           WA_MinHeight,            40,
                                           WA_MaxWidth,             -1,
                                           WA_MaxHeight,            -1,
                                           WA_Title,                Version + 5,
                                           WA_Flags,                WFLG_SIZEGADGET|WFLG_DRAGBAR|WFLG_DEPTHGADGET|WFLG_RMBTRAP|WFLG_SIMPLE_REFRESH,
                                           WA_PubScreenName,        ConsolePubScreenName,
                                           WA_PubScreenFallBack,    TRUE,
                                           WA_AutoAdjust,           TRUE,
                                           TAG_DONE)))
    {
        FPutsLog( "can't open window!");
        Close_All( FAIL);
    }

    if( ConsoleFont = OpenDiskFont( &ConsoleFontAttr)) SetFont( ConsoleWindow->RPort, ConsoleFont);

    ConsoleWriteIO->io_Data   = ( APTR) ConsoleWindow;
    ConsoleWriteIO->io_Length = sizeof( struct Window);

    if( ConsoleDevice = OpenDevice( CONSOLENAME, CONU_SNIPMAP, ConsoleWriteIO, CONFLAG_DEFAULT))
    {
        FPrintf( StdOutputHandle, "can`t open %s!\n", CONSOLENAME);
        Close_All( FAIL);
    }

    ConsoleReadIO->io_Device = ConsoleWriteIO->io_Device;
    ConsoleReadIO->io_Unit   = ConsoleWriteIO->io_Unit;

    ConsoleReadIO->io_Command   = CMD_READ;
    ConsoleReadIO->io_Length    = 1;
    ConsoleReadIO->io_Data      = &ConsoleReadChar;
    SendIO( ConsoleReadIO);     /* setup first request. Important! */
//-

    if( !( LogFileHandle = Open( LogFileName, MODE_READWRITE)))
    {
        FPutsLog( "can`t open logfile!");
        Close_All( FAIL);
    }

    Seek( LogFileHandle, 0, OFFSET_END);
    FPutC( LogFileHandle, 0x0a);
    FPutsLog( "Starting BBS...");

    #ifdef DEBUG
    FPutsLog( "party time!");
    #endif

    ReadStats();

    #ifdef DEBUG
    FPutsLog( "1");
    #endif

    ReadUser( UserName);

    #ifdef DEBUG
    FPutsLog( "2");
    #endif
} //-
//+ void main()
void main()
{
    struct ConfigNode   *ConfigNode;

    signal( SIGINT, SIG_IGN);

    Open_All();

    #ifdef DEBUG
    FPutsLog( "3");
    #endif

    while( TRUE)
    {
        #ifdef DEBUG
        FPutsLog( "4");
        #endif

        if( ConfigNode = FindNameNoCase(( struct List *) ConfigList, "startcommand"))
        {
            #ifdef DEBUG
            FPutsLog( "5");
            #endif

            if( !( DoCommand( ConfigNode->cn_String, NULL)))
            {
                FPutsLog( "No valid startcommand!");
                Close_All( FAIL);
            }
            #ifdef DEBUG
            FPrintfLog( "%ld: %s = %s", __LINE__, ConfigNode->cn_Node.ln_Name, ConfigNode->cn_String);
            #endif
        }
        else
        {
            #ifdef DEBUG
            FPutsLog( "6");
            #endif

            if( ! ( DoCommand( "main.menu", NULL)))
            {
                FPutsLog( "No main.menu or startcommand!");
                Close_All( FAIL);
            }
        }
    }
} //-
//+ void Close_All( int ExitError)
void Close_All( int ExitError)
{
    #ifdef DEBUG
    FPutsLog( "7");
    #endif

    #ifdef DEBUG
    FPutsLog( "let's go home...");
    #endif

    WriteConfig( StatsFileName, StatsList);

    FPutsLog( "BBS exit normal.");

    if( SpawnPort)
    {
        RemPort( SpawnPort);

        ClearSpawnPort();

        DeleteMsgPort( SpawnPort);
    }

    #ifdef DEBUG
    FPutsLog( "closing devices.");
    #endif

    if( !( SerialDevice))
    {
        AbortIO( SerialReadIO);
        WaitIO( SerialReadIO);

        CloseDevice( SerialReadIO);
    }

    if( !( ConsoleDevice))
    {
        AbortIO( ConsoleReadIO);
        WaitIO( ConsoleReadIO);

        CloseDevice( ConsoleReadIO);
    }

    if( !( TimerDevice))        CloseDevice( TimerIO);

    if( SerialReadIO)           DeleteExtIO( SerialReadIO);
    if( SerialReadMP)           DeleteMsgPort( SerialReadMP);
    if( SerialWriteIO)          DeleteExtIO( SerialWriteIO);
    if( SerialWriteMP)          DeleteMsgPort( SerialWriteMP);

    if( ConsoleReadIO)          DeleteExtIO( ConsoleReadIO);
    if( ConsoleReadMP)          DeleteMsgPort( ConsoleReadMP);
    if( ConsoleWriteIO)         DeleteExtIO( ConsoleWriteIO);
    if( ConsoleWriteMP)         DeleteMsgPort( ConsoleWriteMP);

    if( TimerIO)                DeleteExtIO( TimerIO);
    if( TimerMP)                DeleteMsgPort( TimerMP);

    #ifdef DEBUG
    FPutsLog( "closing screen.");
    #endif

    if( ConsoleWindow)          CloseWindow( ConsoleWindow);
    if( ConsoleFont)            CloseFont( ConsoleFont);

    if( Screen)                 while( !( CloseScreen( Screen))) Delay( 50L);
    if( AlienPubScreen)         UnlockPubScreen( NULL, AlienPubScreen);

    #ifdef DEBUG
    FPutsLog( "freeing buffers.");
    #endif

    if( ConfigFileBuffer)       FreeVec( ConfigFileBuffer);
    if( KeyFileBuffer)          FreeVec( KeyFileBuffer);
    if( ConfigFileInfoBlock)    FreeVec( ConfigFileInfoBlock);
    if( KeyFileInfoBlock)       FreeVec( KeyFileInfoBlock);

    if( RD_Struct)              FreeArgs( RD_Struct);

    ClearList( ConfigList);
    ClearList( UserList);
    ClearList( StatsList);

    if( ConfigFileHandle)       Close( ConfigFileHandle);
    if( KeyFileHandle)          Close( KeyFileHandle);

    if( MetaXprBase)            CloseLibrary( MetaXprBase);

    #ifdef DEBUG
    FPutsLog( "that's it.");
    #endif

    if( LogFileHandle)          Close( LogFileHandle);

    MyTask->pr_WindowPtr = OldWindowPtr;

    exit( ExitError);
} //-

//+ -Log-
//+ void FPutsLog( char *String)
void FPutsLog( char *String)
{
    char TimeString[ TIMESTRING_SIZE ];

    OTime = time( NULL);
    strftime( TimeString, TIMESTRING_SIZE, "%d-%m %H:%M:%S> ", localtime( &OTime));

    if( LogFileHandle)
    {
        FPuts( LogFileHandle, TimeString);
        FPuts( LogFileHandle, String);
        FPutC( LogFileHandle, 0x0a);

        Flush( LogFileHandle);
    }
    #ifndef DEBUG
    else
    #endif
    if( StdOutputHandle)
    {
        FPuts( StdOutputHandle, TimeString);
        FPuts( StdOutputHandle, String);
        FPutC( StdOutputHandle, 0x0a);

        Flush( StdOutputHandle);
    }
} //-
//+ void FPrintfLog( char *FString, ...)
void FPrintfLog( char *FString, ...)
{
    va_list Arguments;

    char    String[ 120 ];

    va_start( Arguments, FString);

    vsprintf( String, FString, Arguments);
    FPutsLog( String);

    va_end( Arguments);
} //-
//-
