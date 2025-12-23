// Support.c

#include    "Support.h"

extern long    Node;
extern long    Unit;
extern long    Baud;

extern APTR    RD_Array[ RD_Size ];

extern char    UserName[ STRING_LENGTH ];

extern struct UserStruct   User;
extern struct StatsStruct  Stats;

extern char    *Prompt;
extern char    *StatsFileName;

extern struct List *StatsList;
extern struct List *UserList;

extern struct MsgPort      *SpawnPort;
extern struct SpawnMessage *SpawnMessage;

//+ BOOL DoCommand( char *CommandID, char *CommandArgs)
BOOL DoCommand( char *CommandID, char *CommandArgs)
{
    char FullString[ COMMAND_LENGTH];
    char FullString2[ COMMAND_LENGTH];

    CompleteString( CommandID, FullString);
    CompleteString( CommandArgs, FullString2);

         if( !( StrCmpNoCase( FullString, "Chat")))           Chatter();
    else if( !( StrCmpNoCase( FullString, "ChangeMaxlines"))) ChangeMaxLines();
    else if( !( StrCmpNoCase( FullString, "ChangePassword"))) ChangePassword();
    else if( !( StrCmpNoCase( FullString, "Echo")))           PutString( FullString2);
    else if( !( StrCmpNoCase( FullString, "Upload")))         Upload();
    else if( !( StrCmpNoCase( FullString, "BatchDownload")))  BatchDownload( FullString2);
    else if( !( StrCmpNoCase( FullString, "LogOn")))          Logon();
    else if( !( StrCmpNoCase( FullString, "LogOff")))         Logoff();
    else if( !( StrCmpNoCase( FullString, "WaitReturn")))     WaitReturn();
    else if( !( StrCmpNoCase( FullString, "GetLine")))        GetLine( FullString2);
    else if( !( StrCmpNoCase( FullString, "GetLines")))       GetLines( FullString2);
    else if( Menu( FullString));
    else if( Batch( FullString));
    else if( External( FullString));
    else if( Spawn( FullString));
    else if( Type( FullString));
    else return( FALSE);

    return( TRUE);
} //-
//+ BOOL DoBatch( struct ConfigNode *ConfigNode)
BOOL DoBatch( struct ConfigNode *ConfigNode)
{
    struct ConfigNode   *ThisNode;
    struct ConfigNode   *NextNode;

    BOOL Success = FALSE;

    if( ConfigNode->cn_Node.ln_Type == CONFIGNODE_ID)
    {
        #ifdef DEBUG
        FPrintfLog( "%ld: %s = %s", __LINE__, ThisNode->cn_Node.ln_Name, ThisNode->cn_String);
        #endif

        Success = DoCommand( ConfigNode->cn_String, NULL);
    }
    else
    {
        ThisNode = ( struct Node *) ((( struct List *) ConfigNode->cn_Value)->lh_Head);

        #ifdef DEBUG
        FPrintfLog( "%ld: %s = %s", __LINE__, ThisNode->cn_Node.ln_Name, ThisNode->cn_String);
        #endif

        while( NextNode = ThisNode->cn_Node.ln_Succ)
        {
            #ifdef DEBUG
            FPrintfLog( "%ld: %s = %s", __LINE__, ThisNode->cn_Node.ln_Name, ThisNode->cn_String);
            #endif

            if( ThisNode->cn_Node.ln_Type != CONFIGSUB_ID)
            {
                #ifdef DEBUG
                FPrintfLog( "%ld: %s = %s", __LINE__, ThisNode->cn_Node.ln_Name, ThisNode->cn_String);
                #endif

                if( DoCommand( ThisNode->cn_Node.ln_Name, ThisNode->cn_String)) Success = TRUE;
            }
            ThisNode = NextNode;
        }
    }

    return( Success);
} //-
//+ void CompleteString( char *String, char *FullString)
void CompleteString( char *String, char *FullString)
{
    /*
    %%
    %Node
    %Baud
    %User
    %Connect
    %Calls
    %Files
    %Level
    %AllCalls
    %AllFiles

    %Device
    %Unit
    */

    long    Count   = 0x00;
    char    TempString[ STRING_LENGTH ];
    char    HalfString[ COMMAND_LENGTH ];
    char    *HalfStringPos = HalfString;

    if( String)
    {
        while( HalfStringPos < HalfString + COMMAND_LENGTH && *String)
        {
            if(( *HalfStringPos = *String++) == '%')
            {
                if( !( StrCmpNoCaseOpen( "node", String)))
                {
                    sprintf( TempString, "%lu", Node);
                    String += strlen( "node");
                }
                else if( !( StrCmpNoCaseOpen( "unit", String)))
                {
                    sprintf( TempString, "%lu", Unit);
                    String += strlen( "unit");
                }
                else if( !( StrCmpNoCaseOpen( "baud", String)))
                {
                    sprintf( TempString, "%lu", Baud);
                    String += strlen( "baud");
                }
                else if( !( StrCmpNoCaseOpen( "connect", String)))
                {
                    if( RD_Array[ CONNECT ]) sprintf( TempString, "%s",  RD_Array[ CONNECT ]);
                    String += strlen( "connect");
                }
                else if( !( StrCmpNoCaseOpen( "device", String)))
                {
                    if( RD_Array[ DEVICE ]) sprintf( TempString, "%s",  RD_Array[ DEVICE ]);
                    String += strlen( "device");
                }
                else if( !( StrCmpNoCaseOpen( "user", String)))
                {
                    sprintf( TempString, "%s",  UserName);
                    String += strlen( "user");
                }
                else if( !( StrCmpNoCaseOpen( "calls", String)))
                {
                    sprintf( TempString, "%lu", *User.us_Calls);
                    String += strlen( "calls");
                }
                else if( !( StrCmpNoCaseOpen( "files", String)))
                {
                    sprintf( TempString, "%lu", *User.us_Files);
                    String += strlen( "files");
                }
                else if( !( StrCmpNoCaseOpen( "level", String)))
                {
                    sprintf( TempString, "%lu", *User.us_Level);
                    String += strlen( "level");
                }
                else if( !( StrCmpNoCaseOpen( "allcalls", String)))
                {
                    sprintf( TempString, "%lu", *Stats.st_Calls);
                    String += strlen( "allcalls");
                }
                else if( !( StrCmpNoCaseOpen( "allfiles", String)))
                {
                    sprintf( TempString, "%lu", *Stats.st_Files);
                    String += strlen( "allfiles");
                }
                else
                {
                    *TempString = 0x00;
                    HalfStringPos++;
                }

                strncpy( HalfStringPos, TempString, COMMAND_LENGTH - ( HalfString - HalfStringPos));

                HalfStringPos = HalfString + strlen( HalfString);
            }
            else HalfStringPos++;
        }
    }

    *HalfStringPos = 0x00;

    time_t OTime = time( NULL);
    strftime( FullString, COMMAND_LENGTH, HalfString, localtime( &OTime));
} //-
//+ void SystemError()
void SystemError()
{
    Type( "systemerror.txt");

    Close_All( FAIL);
} //-
//+ void SetBBSPrompt()
void SetBBSPrompt()
{
    char *PromptString[ STRING_LENGTH ];

    CompleteString( Prompt, PromptString);
    PutString( PromptString);
} //-
//+ void ReadUser( char *UserName)
BOOL ReadUser( char *UserName)
{
    char    UserFileName[ COMMAND_LENGTH ];

    struct ConfigNode   *ConfigNode;

    BOOL    Result = TRUE;

    sprintf( UserFileName, "BBS:Users/%s", PathPart( UserName));

    ClearList( UserList);
    UserList = NULL;

    ReadConfig( UserFileName, &UserList);

    if( !( ConfigNode = FindNameNoCase(( struct List *) UserList, "calls")))
        ConfigNode = AddNode( "calls", 0x00, 0x00, UserList);
    User.us_Calls = &ConfigNode->cn_Value;
    *ConfigNode->cn_String = NULL;

    if( !( ConfigNode = FindNameNoCase(( struct List *) UserList, "files")))
        ConfigNode = AddNode( "files", 0x00, 0x00, UserList);
    User.us_Files = &ConfigNode->cn_Value;
    *ConfigNode->cn_String = NULL;

    if( !( ConfigNode = FindNameNoCase(( struct List *) UserList, "maxlines")))
        ConfigNode = AddNode( "maxlines", 0x00, 0x25, UserList);
    User.us_MaxLines = &ConfigNode->cn_Value;
    *ConfigNode->cn_String = NULL;

    if( !( ConfigNode = FindNameNoCase(( struct List *) UserList, "level")))
        ConfigNode = AddNode( "level", 0x00, 0x00, UserList);
    User.us_Level = &ConfigNode->cn_Value;
    *ConfigNode->cn_String = NULL;

    if( !( ConfigNode = FindNameNoCase(( struct List *) UserList, "password")))
    {
        ConfigNode = AddNode( "password", 0x00, 0x00, UserList);
        Result = FALSE;
    }
    User.us_Password = ConfigNode->cn_String;

    return( Result);
} //-
//+ void WriteUser()
void WriteUser()
{
    char    UserFileName[ COMMAND_LENGTH ];

    sprintf( UserFileName, "BBS:Users/%s", PathPart( UserName));

    WriteConfig( UserFileName, UserList);
} //-
//+ void ReadStats()
void ReadStats()
{
    struct ConfigNode   *ConfigNode;

    ClearList( StatsList);
    StatsList = NULL;
    ReadConfig( StatsFileName, &StatsList);

    if( !( ConfigNode = FindNameNoCase(( struct List *) StatsList, "calls")))
        ConfigNode = AddNode( "calls", 0x00, 0x00, StatsList);
    Stats.st_Calls = &ConfigNode->cn_Value;
    *ConfigNode->cn_String = NULL;

    if( !( ConfigNode = FindNameNoCase(( struct List *) StatsList, "files")))
        ConfigNode = AddNode( "files", 0x00, 0x00, StatsList);
    Stats.st_Files = &ConfigNode->cn_Value;
    *ConfigNode->cn_String = NULL;
} //-
//+ void WriteStats()
void WriteStats()
{
    WriteConfig( StatsFileName, StatsList);
} //-
//+ void ClearSpawnPort()
void ClearSpawnPort()
{
    do
    {
        if( SpawnMessage)
        {
            if( SpawnMessage->sm_Type == SM_SPAWN_DATA_IN)
            {
                SpawnMessage->sm_Length = 0;
                ReplyMsg( SpawnMessage);
            }
            else
            {
                if( SpawnMessage->sm_Data) FreeVec( SpawnMessage->sm_Data);
                FreeVec( SpawnMessage);
            }
        }
    }
    while( SpawnMessage = GetMsg( SpawnPort));
} //-
