// Commands.c

#include    "Commands.h"

extern char    InputChar;

extern struct UserStruct   User;
extern struct StatsStruct  Stats;

extern char    UserName[ STRING_LENGTH ];

extern BPTR    TypeFileHandle;

extern struct List *ConfigList;

extern struct MsgPort      *TimerMP;
extern struct timerequest  *TimerIO;

extern struct SpawnMessage *SpawnMessage;
extern struct MsgPort      *SpawnPort;

extern long    Node;

//+ void Logon()
void Logon()
{
    char    Password[ STRING_LENGTH ];

    long    Count = 3;

    BOOL    OldUser;

    InputChar = 110;

    PutString( "\r\n");

    do
    {
        do
        {
            PutString( "Name> ");

            GetString( UserName);
        }
        while( strlen( UserName) < 3);

        OldUser = ReadUser( UserName);

        if( !( OldUser))
        {
            PutString( "\r\nUsername unbekannt. Bist du ein neuer User? [y/N]");

            do
            {
                GetChar( &InputChar);
                tolower( InputChar);
            }
            while( InputChar != 'y' && InputChar != 'n' && InputChar != 0x0a);
        }
    }
    while( !( OldUser) && InputChar != 'y');

    FPrintfLog( "Login: %s", UserName);

    if( OldUser)
    {
        do
        {
            PutString( "Passwort> ");

            GetStringHidden( Password);

            FPrintfLog( "Passwort: %s", Password);
        }
        while( StrCmpNoCase( Password, User.us_Password) && --Count);

        if( !( Count))
        {
            Type( "securityviolation.txt");

            FPutsLog( "Sicherheitsverletzung.");

            Close_All( FAIL);
        }
    }
    else
    {
        FPutsLog( "Neuer User!");

        Type( "newuser.txt");

        *User.us_Calls      = 0;
        *User.us_Files      = 0;
        *User.us_Level      = 0;
        *User.us_MaxLines   = 25;

        ChangePassword();
    }

    ( *User.us_Calls)++;
    WriteUser();
    ( *Stats.st_Calls)++;
    WriteStats();
}
//-
//+ void Logoff()
void Logoff()
{
    FPutsLog( "Logoff normal.");

    Close_All( NULL);
} //-
//+ BOOL Type( char *TypeFileID)
BOOL Type( char *TypeFileID)
{
    char    FullString[ COMMAND_LENGTH ];

    BPTR    TypeFileHandle;
    char    *TypeFileName = NULL;
    char    *TypeBuffer;
    char    *WriteBuffer;
    long    SizeRead;
    long    Count;
    long    LineCount = 0x00;

    struct ConfigNode   *ConfigNode;

    if( ConfigNode = FindNameNoCase(( struct List *) ConfigList, "texts"))
    {
        if( ConfigNode->cn_Node.ln_Type == CONFIGSUB_ID)
        {
            if( ConfigNode = FindNameNoCase(( struct List *) ConfigNode->cn_Value, TypeFileID))
            {
                #ifdef DEBUG
                FPrintfLog( "%ld: %s = %s", __LINE__, ConfigNode->cn_Node.ln_Name, ConfigNode->cn_String);
                #endif

                TypeFileName = ConfigNode->cn_String;
            }
        }
    }

    if( !( TypeFileName)) TypeFileName = TypeFileID;

    CompleteString( TypeFileName, FullString);

    if( TypeFileHandle = Open( FullString, MODE_OLDFILE))
    {
        if( TypeBuffer = ( long *) AllocVec( TYPEBUFFER_SIZE, MEMF_ANY))
        {
            while(( SizeRead = Read( TypeFileHandle, TypeBuffer, TYPEBUFFER_SIZE)) > 0)
            {
                WriteBuffer = TypeBuffer;
                Count = 0;

                while( SizeRead)
                {
                    if( WriteBuffer[ Count ] == 0x0a)
                    {
                        if( Count && ( WriteBuffer[ Count - 1 ] == 0x0d))  PutData( WriteBuffer, Count - 1);
                        else                                               PutData( WriteBuffer, Count);

                        PutChar( 0x0a);

                        if( *User.us_MaxLines)
                        {
                            if( ++LineCount == *User.us_MaxLines)
                            {
                                DoCommand( "waitreturn.bat", NULL);

                                LineCount = 0x00;
                            }
                        }

                        WriteBuffer += Count + 1;
                        Count = -1;
                    }

                    Count++;
                    SizeRead--;
                }

                PutData( WriteBuffer, Count);
            }

            FreeVec( TypeBuffer);
        }
        Close( TypeFileHandle);
    }
    else return( FALSE);

    return( TRUE);
} //-
//+ BOOL External( char *ExternalID)
BOOL External( char *ExternalID)
{
    char    FullString[ COMMAND_LENGTH ];
    struct ConfigNode   *ConfigNode;

    if( ConfigNode = FindNameNoCase(( struct List *) ConfigList, "externals"))
    {
        if( ConfigNode->cn_Node.ln_Type == CONFIGSUB_ID)
        {
            if( ConfigNode = FindNameNoCase(( struct List *) ConfigNode->cn_Value, ExternalID))
            {
                if( ConfigNode->cn_Node.ln_Type == CONFIGNODE_ID)
                {
                    CompleteString( ConfigNode->cn_String, FullString);
                    FPrintfLog( "External: %s", FullString);
                    SystemTagList( FullString, NULL);
                    FPutsLog( "...end.");
                }
                else return( FALSE);
            }
            else return( FALSE);
        }
        else return( FALSE);
    }
    else return( FALSE);
    return( TRUE);
}
//-
//+ BOOL Spawn( char *SpawnID)
BOOL Spawn( char *SpawnID)
{
    char    FullString[ COMMAND_LENGTH ];
    char    SpawnFile[ STRING_LENGTH ];

    BPTR    SpawnInHandle;
    BPTR    SpawnOutHandle;

    struct ConfigNode   *ConfigNode;

    long    Files        = 0;
    long    MinutesCount = 5;

    BOOL    SpawnDie = FALSE;

    if( ConfigNode = FindNameNoCase(( struct List *) ConfigList, "spawns"))
    {
        if( ConfigNode->cn_Node.ln_Type == CONFIGSUB_ID)
        {
            if( ConfigNode = FindNameNoCase(( struct List *) ConfigNode->cn_Value, SpawnID))
            {
                if( ConfigNode->cn_Node.ln_Type == CONFIGNODE_ID)
                {
                    sprintf( SpawnFile, "MicroBBS:%lu", Node);

                    ClearSpawnPort();

                    if( SpawnInHandle = Open( SpawnFile, MODE_READWRITE))
                    {
                        if( SpawnOutHandle = Open( SpawnFile, MODE_READWRITE))
                        {
                            long    SpawnSignal = 1L << SpawnPort->mp_SigBit;
                            long    TimerSignal = 1L << TimerMP->mp_SigBit;

                            CompleteString( ConfigNode->cn_String, FullString);
                            FPrintfLog( "Spawn: %s", FullString);
                            SystemTags( FullString, SYS_Asynch,  TRUE,
                                                    SYS_Input,   SpawnInHandle,
                                                    SYS_Output,  SpawnOutHandle,
                                                    TAG_DONE);

                            while( MinutesCount-- && !( SpawnDie))
                            {
                                TimerIO->tr_node.io_Command = TR_ADDREQUEST;
                                TimerIO->tr_time.tv_secs    = 60;
                                TimerIO->tr_time.tv_micro   = 0;

                                SendIO( TimerIO);

                                #ifdef DEBUG
                                FPrintfLog( "%ld: %x", __LINE__, Wait( SpawnSignal | TimerSignal));
                                #else
                                Wait( SpawnSignal | TimerSignal);
                                #endif

                                if( CheckIO( TimerIO))
                                {
                                    PutStringF( "\r\nKeine Spawn-Aktivitaet! %d Minuten bis zum Logoff.\r\n\a", MinutesCount);
                                }
                                else
                                {
                                    AbortIO( TimerIO);
                                    WaitIO( TimerIO);
                                    Wait( TimerSignal);

                                    while( SpawnMessage = GetMsg( SpawnPort))
                                    {
                                        #ifdef DEBUG
                                        FPutsLog( "Recieved packets:");
                                        #endif

                                        switch( SpawnMessage->sm_Type)
                                        {
//+                                           SM_SPAWN_DATA_IN
                                            case SM_SPAWN_DATA_IN :
                                            {
                                                long    ByteCount;

                                                #ifdef DEBUG
                                                FPrintfLog( "%ld:     Data_In (%ld):", __LINE__, SpawnMessage->sm_Mode );
                                                #endif

                                                if( ByteCount = SpawnMessage->sm_Length)
                                                {
                                                    if( SpawnMessage->sm_Mode == SM_RAW)
                                                    {
                                                        DGetChar( SpawnMessage->sm_Data);
                                                        SpawnMessage->sm_Length = 1;

                                                        #ifdef DEBUG
                                                        FPrintfLog( "%ld:         got %x", __LINE__, SpawnMessage->sm_Data[ 0 ]);
                                                        #endif
                                                    }
                                                    else
                                                    {
                                                        SpawnMessage->sm_Length = -1;

                                                        do
                                                        {
                                                            SpawnMessage->sm_Length++;
                                                            GetChar( SpawnMessage->sm_Data + SpawnMessage->sm_Length);

                                                            #ifdef DEBUG
                                                            FPrintfLog( "%ld:         got %x", __LINE__, SpawnMessage->sm_Data[ SpawnMessage->sm_Length ]);
                                                            #endif
                                                        }
                                                        while(( 0x03 > SpawnMessage->sm_Data[ SpawnMessage->sm_Length ] > 0x06) && ( SpawnMessage->sm_Data[ SpawnMessage->sm_Length ] != 0x0a) && ( --ByteCount));

//+                                                       check special codes
                                                        switch( SpawnMessage->sm_Data[ SpawnMessage->sm_Length ])
                                                        {
                                                            case 0x03 :
                                                            {
                                                                Signal( SpawnMessage->sm_Task, SIGBREAKF_CTRL_C);
                                                                break;
                                                            }
                                                            case 0x04 :
                                                            {
                                                                Signal( SpawnMessage->sm_Task, SIGBREAKF_CTRL_D);
                                                                break;
                                                            }
                                                            case 0x05 :
                                                            {
                                                                Signal( SpawnMessage->sm_Task, SIGBREAKF_CTRL_E);
                                                                break;
                                                            }
                                                            case 0x06 :
                                                            {
                                                                Signal( SpawnMessage->sm_Task, SIGBREAKF_CTRL_F);
                                                                break;
                                                            }

                                                            default :
                                                            {
                                                                SpawnMessage->sm_Length++;
                                                                break;
                                                            }
                                                        }
//-
                                                    }
                                                }

                                                #ifdef DEBUG
                                                FPrintfLog( "%ld:         sent %lu Bytes", __LINE__, SpawnMessage->sm_Length);
                                                #endif

                                                ReplyMsg( SpawnMessage);
                                                SpawnMessage = NULL;
                                            }
                                            break;
//-
//+                                           SM_SPAWN_DATA_OUT
                                            case SM_SPAWN_DATA_OUT :
                                            {
                                                #ifdef DEBUG
                                                FPrintfLog( "%ld:     Data_Out (%ld):", __LINE__, SpawnMessage->sm_Mode );
                                                FPrintfLog( "%ld:         got %lu Bytes", __LINE__, SpawnMessage->sm_Length);
                                                #endif

                                                char    *Data = SpawnMessage->sm_Data;
                                                long    Count = 0;

                                                while( SpawnMessage->sm_Length)
                                                {
                                                    if( Data[ Count ] == 0x0a)
                                                    {
                                                        if( Count && ( Data[ Count - 1 ] == 0x0d))  PutData( Data, Count - 1);
                                                        else                                        PutData( Data, Count);

                                                        PutChar( 0x0a);

                                                        Data  += Count + 1;
                                                        Count = -1;
                                                    }

                                                    Count++;
                                                    SpawnMessage->sm_Length--;
                                                }

                                                PutData( Data, Count);
                                            }
                                            break;
//-
//+                                           SM_SPAWN_OPEN
                                            case SM_SPAWN_OPEN :
                                            {
                                                Files++;

                                                #ifdef DEBUG
                                                FPrintfLog( "Open: %ld.", Files);
                                                #endif
                                            }
                                            break;
//-
//+                                           SM_SPAWN_CLOSE
                                            case SM_SPAWN_CLOSE :
                                            {
                                                #ifdef DEBUG
                                                FPrintfLog( "Close: %lu.", Files);
                                                #endif

                                                if( !( --Files))
                                                {
                                                    SpawnDie = TRUE;

                                                    #ifdef DEBUG
                                                    FPutsLog( "Die.");
                                                    #endif
                                                }
                                            }
                                            break;
//-
//+                                           SM_SPAWN_WAIT_CHAR
                                            case SM_SPAWN_WAIT_CHAR :
                                            {
                                                #ifdef DEBUG
                                                FPutsLog( "Wait_Char.");
                                                #endif

                                                WaitChar( SpawnMessage->sm_Length );
                                            }
                                            break;
//-

                                            default : break;
                                        }

                                        if( SpawnMessage)
                                        {
                                            if( SpawnMessage->sm_Data) FreeVec( SpawnMessage->sm_Data);

                                            FreeVec( SpawnMessage);
                                            SpawnMessage = NULL;
                                        }
                                    }

                                    MinutesCount = 5;
                                }
                            }

                            if( !( SpawnDie))
                            {
                                PutString( "\r\nAuf wiedersehen! Bis zum naechsten Anruf...\r\n");

                                FPutsLog( "Kicked out.");

                                Close_All( NULL);
                            }

                            ClearSpawnPort();
                            FPutsLog( "...end.");
                        }
                        else Close( SpawnInHandle);
                    }
                }
                else return( FALSE);
            }
            else return( FALSE);
        }
        else return( FALSE);
    }
    else return( FALSE);
    return( TRUE);
}
//-
//+ BOOL Menu( char *MenuID)
BOOL Menu( char *MenuID)
{
    struct ConfigNode   *ConfigNode;
    struct ConfigNode   *MenuNode;

    char    InputString[ 2 ] = "\0\0";
    char    *Password[ STRING_LENGTH ];

    if( ConfigNode = FindNameNoCase(( struct List *) ConfigList, "menus"))
    {
        if( ConfigNode->cn_Node.ln_Type == CONFIGSUB_ID)
        {
            if( MenuNode = FindNameNoCase(( struct List *) ConfigNode->cn_Value, MenuID))
            {
                if( MenuNode->cn_Node.ln_Type == CONFIGSUB_ID)
                {
                    long MenuLevel = 0;

                    if( ConfigNode = FindNameNoCase(( struct List *) MenuNode->cn_Value, "level"))
                        MenuLevel = ConfigNode->cn_Value;

                    if( *User.us_Level >= MenuLevel)
                    {
                        if( ConfigNode = FindNameNoCase(( struct List *) MenuNode->cn_Value, "password"))
                        {
                            #ifdef DEBUG
                            FPrintfLog( "%ld: %s = %s", __LINE__, ConfigNode->cn_Node.ln_Name, ConfigNode->cn_String);
                            #endif

                            PutString( "\r\nPasswort> ");

                            GetStringHidden( Password);

                            FPrintfLog( "Passwort: %s", Password);

                            if( StrCmpNoCase( Password, ConfigNode->cn_String)) return( TRUE);
                        }

                        if( ConfigNode = FindNameNoCase(( struct List *) MenuNode->cn_Value, "before"))
                        {
                            #ifdef DEBUG
                            FPrintfLog( "%ld: %s = %s", __LINE__, ConfigNode->cn_Node.ln_Name, ConfigNode->cn_String);
                            #endif

                            DoBatch( ConfigNode);
                        }

                        do
                        {
                            SetBBSPrompt();
                            DGetChar( &InputChar);

                            switch( InputChar)
                            {
                                case ' '  : break;
                                case 0x0a : break;

                                default   :
                                {
                                    InputString[ 0 ] = InputChar;

                                    if( ConfigNode = FindNameNoCase(( struct List *) MenuNode->cn_Value, "options"))
                                    {
                                        if( ConfigNode->cn_Node.ln_Type == CONFIGSUB_ID)
                                        {
                                            if( ConfigNode = FindNameNoCase(( struct List *) ConfigNode->cn_Value, InputString))
                                            {
                                                #ifdef DEBUG
                                                FPrintfLog( "%ld: %s = %s", __LINE__, ConfigNode->cn_Node.ln_Name, ConfigNode->cn_String);
                                                #endif

                                                if( InputChar == '-') InputChar = 0x00;

                                                if( DoBatch( ConfigNode))
                                                {
                                                    if( ConfigNode = FindNameNoCase(( struct List *) MenuNode->cn_Value, "after"))
                                                    {
                                                        #ifdef DEBUG
                                                        FPrintfLog( "%ld: %s = %s", __LINE__, ConfigNode->cn_Node.ln_Name, ConfigNode->cn_String);
                                                        #endif

                                                        DoBatch( ConfigNode);
                                                    }
                                                }
                                            }
                                        }
                                    }
                                    break;
                                }
                            }
                        }
                        while( InputChar != '-');

                        InputChar = 0x00;
                    }
                    else
                    {
                        Type( "leveltoolow.txt");
                        WaitReturn();
                    }
                }
                else return( FALSE);
            }
            else return( FALSE);
        }
        else return( FALSE);
    }
    else return( FALSE);

    return( TRUE);
} //-
//+ BOOL Batch( char *BatchID)
BOOL Batch( char *BatchID)
{
    struct ConfigNode   *ThisNode;

    if( ThisNode = FindNameNoCase(( struct List *) ConfigList, "batches"))
    {
        if( ThisNode->cn_Node.ln_Type == CONFIGSUB_ID)
        {
            #ifdef DEBUG
            FPrintfLog( "%ld: %s:", __LINE__, ThisNode->cn_Node.ln_Name);
            #endif

            if( ThisNode = FindNameNoCase(( struct List *) ThisNode->cn_Value, BatchID))
            {
                return( DoBatch( ThisNode));
            }
            else return( FALSE);
        }
        else return( FALSE);
    }
    else return( FALSE);

    return( TRUE);
} //-
//+ void WaitReturn()
void WaitReturn()
{
    char    BufferChar = 0x00;

    while( BufferChar != 0x0a) DGetChar( &BufferChar);

    PutChar( 0x0a);
} //-
