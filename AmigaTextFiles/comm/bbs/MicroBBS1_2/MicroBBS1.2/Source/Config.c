// Config.c

#include    "Config.h"

extern char FullString[ COMMAND_LENGTH ];

extern struct RDArgs   *RD_Struct;
extern APTR   RD_Array[ RD_Size ];

extern char *DeviceName;
extern char *StatsFileName;
extern char *ConfigFileName;
extern char *Prompt;

extern BOOL Key;
extern BOOL DoScreen;

extern long Node;
extern long Unit;
extern long Baud;

extern struct FileInfoBlock    *ConfigFileInfoBlock;
extern APTR    ConfigFileBuffer;
extern BPTR    ConfigFileHandle;

extern struct List *ConfigList;

extern long    ConsoleLeft;
extern long    ConsoleTop;
extern long    ConsoleWidth;
extern long    ConsoleHeight;

extern long    ScreenWidth;
extern long    ScreenHeight;
extern long    ScreenDepth;
extern long    ScreenID;

extern struct TextAttr     ConsoleFontAttr;

extern char    LogFileName[ STRING_LENGTH ];
extern char    ScreenName[ STRING_LENGTH ];
extern char    ConsolePubScreenName[ STRING_LENGTH ];

extern char    *SerialColor[ 20 ];
extern char    *ConsoleColor[ 20 ];

//+ void CheckArgs()
void CheckArgs()
{
    if( RD_Array[ DEVICE ])         DeviceName = RD_Array[ DEVICE ];
    if( Key && RD_Array[ NODE ])    Node = *(( long *) RD_Array[ NODE ]);
    if( RD_Array[ UNIT ])           Unit = *(( long *) RD_Array[ UNIT ]);
    if( RD_Array[ BAUD ])           Baud = *(( long *) RD_Array[ BAUD ]);
} //-

//+ BOOL ReadConfig( char *ConfigFileName, struct List **ConfigList)
BOOL ReadConfig( char *ConfigFileName, struct List **ConfigList)
{
    if( !( *ConfigList))
    {
        if( *ConfigList = AllocVec( sizeof( struct List), MEMF_ANY|MEMF_CLEAR))
        {
            NewList( *ConfigList);
        }
        else return( FALSE);
    }

    if( ConfigFileHandle = Open( ConfigFileName, MODE_OLDFILE))
    {
        if( ConfigFileInfoBlock = ( long *) AllocVec( sizeof( struct FileInfoBlock), MEMF_ANY|MEMF_CLEAR))
        {
            if( ExamineFH( ConfigFileHandle, ConfigFileInfoBlock))
            {
                if( ConfigFileBuffer = ( long *) AllocVec( ConfigFileInfoBlock->fib_Size + 1, MEMF_ANY|MEMF_CLEAR))
                {
                    if( ConfigFileInfoBlock->fib_Size == Read( ConfigFileHandle, ConfigFileBuffer, ConfigFileInfoBlock->fib_Size))
                        ReadConfigSub( ConfigFileBuffer, ( char *) ConfigFileBuffer + ConfigFileInfoBlock->fib_Size, *ConfigList);
                    else SystemError();

                    FreeVec( ConfigFileBuffer);
                    ConfigFileBuffer = NULL;
                }
                else SystemError();
            }
            else SystemError();

            FreeVec( ConfigFileInfoBlock);
            ConfigFileInfoBlock = NULL;
        }
        else SystemError();

        Close( ConfigFileHandle);
        ConfigFileHandle = NULL;
    }
} //-
//+ void ReadConfigSub( char *SubStartPointer, char *SubEndPointer, struct List *SubList)
void ReadConfigSub( char *SubStartPointer, char *SubEndPointer, struct List *SubList)
{
    char    *AtomStartPointer;
    char    *AtomEndPointer;
    char    *TagStartPointer;
    char    *DataStartPointer;
    char    *DataEndPointer;
    char    *TempPointer;

    AtomStartPointer = SubStartPointer;

/*    Write( StdOutputHandle, SubStartPointer, SubEndPointer - SubStartPointer); */

    while( AtomStartPointer)
    {
        AtomStartPointer = FindAtom( AtomStartPointer, SubEndPointer, '(');
        AtomEndPointer   = FindAtomEnd( AtomStartPointer, SubEndPointer);

        TagStartPointer  = FindNextChar( AtomStartPointer, AtomEndPointer);
        TempPointer      = FindNextSpace( TagStartPointer, AtomEndPointer);
        DataStartPointer = FindNextChar( TempPointer, AtomEndPointer);

        if( TempPointer)
        {
            *TempPointer = 0x00;

            if( DataStartPointer)
            {
                if( *DataStartPointer == '(')
                {
                    ReadConfigSub( DataStartPointer, AtomEndPointer, AddHeadNode( TagStartPointer, SubList));
                }
                else
                {
                    if( *DataStartPointer == '"')
                    {
                        TempPointer = ++DataStartPointer;

                        while( TempPointer = FindAtom( TempPointer, AtomEndPointer, '"'))
                            DataEndPointer = TempPointer - 1;
                    }
                    else
                        DataEndPointer = FindNextSpace( DataStartPointer, AtomEndPointer);

                    if( DataEndPointer)
                    {
                        *DataEndPointer = 0x00;

                        AddNode( TagStartPointer, DataStartPointer, atol( DataStartPointer), SubList);
                    }
                }
            }
            else
            {
                AddNode( TagStartPointer, NULL, NULL, SubList);
            }
        }

        if( AtomEndPointer)
            AtomStartPointer = AtomEndPointer + 1;
        else
            AtomStartPointer = 0x00;
    }
} //-

//+ void AnalyseConfig()
void AnalyseConfig()
{
    struct ConfigNode   *ConfigNode;
    struct ConfigNode   *ThisNode;
    struct ConfigNode   *NextNode;

    if( Key && ( ThisNode = FindNameNoCase(( struct List *) ConfigList, "configs")))
    {
        if( ThisNode->cn_Node.ln_Type == CONFIGSUB_ID)
        {
            ThisNode = ( struct Node *) ((( struct List *) ThisNode->cn_Value)->lh_Head);

            while( NextNode = ThisNode->cn_Node.ln_Succ)
            {
                if( ThisNode->cn_Node.ln_Type != CONFIGSUB_ID)
                {
                    CompleteString( ThisNode->cn_Node.ln_Name, FullString);
                    ReadConfig( FullString, &ConfigList);
                }

                ThisNode = NextNode;
            }
        }
    }

    if( Key && ( ThisNode = FindNameNoCase(( struct List *) ConfigList, "console")))
    {
        if( ThisNode->cn_Node.ln_Type == CONFIGSUB_ID)
        {
            if( ConfigNode = FindNameNoCase(( struct List *) ThisNode->cn_Value, "left"))   ConsoleLeft   = ConfigNode->cn_Value;
            if( ConfigNode = FindNameNoCase(( struct List *) ThisNode->cn_Value, "top"))    ConsoleTop    = ConfigNode->cn_Value;
            if( ConfigNode = FindNameNoCase(( struct List *) ThisNode->cn_Value, "width"))  ConsoleWidth  = ConfigNode->cn_Value;
            if( ConfigNode = FindNameNoCase(( struct List *) ThisNode->cn_Value, "height")) ConsoleHeight = ConfigNode->cn_Value;

            if( ConfigNode = FindNameNoCase(( struct List *) ThisNode->cn_Value, "fontsize"))  ConsoleFontAttr.ta_YSize = ConfigNode->cn_Value;
            if( ConfigNode = FindNameNoCase(( struct List *) ThisNode->cn_Value, "font"))      ConsoleFontAttr.ta_Name  = ConfigNode->cn_String;

            if( ConfigNode = FindNameNoCase(( struct List *) ThisNode->cn_Value, "pubscreen")) CompleteString( ConfigNode->cn_String, ConsolePubScreenName);
        }
    }

    if( Key && ( ThisNode = FindNameNoCase(( struct List *) ConfigList, "screen")))
    {
        DoScreen = TRUE;

        if( ThisNode->cn_Node.ln_Type == CONFIGSUB_ID)
        {
            if( ConfigNode = FindNameNoCase(( struct List *) ThisNode->cn_Value, "width"))  ScreenWidth  = ConfigNode->cn_Value;
            if( ConfigNode = FindNameNoCase(( struct List *) ThisNode->cn_Value, "height")) ScreenHeight = ConfigNode->cn_Value;
            if( ConfigNode = FindNameNoCase(( struct List *) ThisNode->cn_Value, "depth"))  ScreenDepth  = ConfigNode->cn_Value;
            if( ConfigNode = FindNameNoCase(( struct List *) ThisNode->cn_Value, "name")) CompleteString( ConfigNode->cn_String, ScreenName);

            if( ConfigNode = FindNameNoCase(( struct List *) ThisNode->cn_Value, "mode"))
            {
                struct NameInfo DisplayName;

                ScreenID = -1;

                while( StrCmpNoCase( ConfigNode->cn_String, DisplayName.Name) && (( ScreenID = NextDisplayInfo( ScreenID)) != -1))
                    GetDisplayInfoData( NULL, &DisplayName, sizeof( struct NameInfo), DTAG_NAME, ScreenID);
            }
        }
    }

    if( Key && ( ThisNode = FindNameNoCase(( struct List *) ConfigList, "chatter")))
    {
        if( ThisNode->cn_Node.ln_Type == CONFIGSUB_ID)
        {
            if( ConfigNode = FindNameNoCase(( struct List *) ThisNode->cn_Value, "serialcolor"))  sprintf( ConsoleColor, "[%lum", ConfigNode->cn_Value);
            if( ConfigNode = FindNameNoCase(( struct List *) ThisNode->cn_Value, "consolecolor")) sprintf( SerialColor, "[%lum", ConfigNode->cn_Value);
        }
    }

    if( Key && ( ConfigNode = FindNameNoCase(( struct List *) ConfigList, "Prompt"))) Prompt = ConfigNode->cn_String;
    if( ConfigNode = FindNameNoCase(( struct List *) ConfigList, "log")) CompleteString( ConfigNode->cn_String, LogFileName);
} //-

//+ void WriteConfig( char *ConfigFilename, struct List *List)
void WriteConfig( char *ConfigFileName, struct List *List)
{
    BPTR    ConfigFileHandle;

    if( List)
    {
        if( ConfigFileHandle = Open( ConfigFileName, MODE_NEWFILE))
        {
            PutList( List, ConfigFileHandle, 0x00);
            Close( ConfigFileHandle);
        }
    }
} //-

//+ char *FindAtom( char *String, char *StringEnd, char Atom)
char *FindAtom( char *String, char *StringEnd, char Atom)
{
    if( String)
    {
        while( *String && *String != Atom && String != StringEnd)
            String++;

        if( *String && String != StringEnd)
            return( ++String);
    }
    return( NULL);
} //-
//+ char *FindAtomEnd( char *String, char *StringEnd)
char *FindAtomEnd( char *String, char *StringEnd)
{
    long    Count = 1;

    if( String)
    {
        while( Count--)
        {
            do
            {
                if( *String == '(')
                {
                    Count++;
                }
                String++;
            }
            while( *String && *String != ')');
        }

        if( *String && String != StringEnd)
            return( String);
    }
    return( NULL);
}//-
//+ char *FindNextChar( char *String, char *StringEnd)
char *FindNextChar( char *String, char *StringEnd)
{
    if( String)
    {
        while( *String && isspace( *String) && String != StringEnd)
            String++;

        if( *String && String != StringEnd)
            return( String);
    }
    return( NULL);
}//-
//+ char *FindNextSpace( char *String, char *StringEnd)
char *FindNextSpace( char *String, char *StringEnd)
{
    if( String && String != StringEnd)
    {
        while( *String && ( !( isspace( *String))) && String != StringEnd)
            String++;

        if( *String)
            return( String);
    }
    return( NULL);
}//-

//+ struct Node *FindNameNoCase( struct List *List, char *String)
struct Node *FindNameNoCase( struct List *List, char *String)
{
    struct Node     *ThisNode;
    struct Node     *NextNode;

    #ifdef COREDEBUG
    FPrintfLog( "%ld: FindNode->List->lh_Head: %ld", __LINE__, List->lh_Head);
    #endif

    if( ThisNode = List->lh_Head)
    {
        while( NextNode = ThisNode->ln_Succ)
        {
            #ifdef COREDEBUG
            FPrintfLog( "%ld: FindNode->ThisNode: %ld", __LINE__, ThisNode);
            #endif

            if( !( StrCmpNoCase( ThisNode->ln_Name, String))) return( ThisNode);

            ThisNode = NextNode;
        }
    }

    return( NULL);
}
//-

//+ struct ConfigNode *AddNode( char *ConfigID, char *ConfigString, long ConfigValue, struct List *ConfigList)
struct ConfigNode *AddNode( char *ConfigID, char *ConfigString, long ConfigValue, struct List *ConfigList)
{
    struct ConfigNode   *ConfigNode = NULL;

    if( !( ConfigNode = AllocVec( sizeof( struct ConfigNode), MEMF_ANY|MEMF_CLEAR)))
        SystemError();

    ConfigNode->cn_Node.ln_Type = CONFIGNODE_ID;
    ConfigNode->cn_Node.ln_Pri  = 0;
    ConfigNode->cn_Node.ln_Name = ConfigNode->cn_ID;

    strncpy( ConfigNode->cn_ID, ConfigID, STRING_LENGTH);

    if( ConfigString && *ConfigString)
    {
        strncpy( ConfigNode->cn_String, ConfigString, STRING_LENGTH);
        ConfigNode->cn_Value = atol( ConfigString);
    }
    else
    {
        sprintf( ConfigNode->cn_String, "%lu", ConfigValue);
        ConfigNode->cn_Value = ConfigValue;
    }

    #ifdef DEEPDEBUG
    FPrintfLog( "%ld: %s = %s", __LINE__, ConfigNode->cn_Node.ln_Name, ConfigNode->cn_String);
    #endif

    AddTail(( struct List *) ConfigList, ( struct Node *) ConfigNode);

    return( ConfigNode);
} //-
//+ struct List *AddHeadNode( char *ConfigID, struct List *ConfigList)
struct List *AddHeadNode( char *ConfigID, struct List *ConfigList)
{
    struct ConfigNode   *ConfigNode;
    struct List         *TempList;

    if( !( ConfigNode = FindNameNoCase(( struct List *) ConfigList, ConfigID)))
        if( !( ConfigNode = AllocVec( sizeof( struct ConfigNode), MEMF_ANY|MEMF_CLEAR)))
            SystemError();

    if( ConfigNode->cn_Node.ln_Type == CONFIGSUB_ID)
        return( ConfigNode->cn_Value);

    if( TempList = AllocVec( sizeof( struct List), MEMF_ANY|MEMF_CLEAR))
    {
        NewList( TempList);

        ConfigNode->cn_Node.ln_Pri  = 0;
        ConfigNode->cn_Value        = TempList;
        ConfigNode->cn_Node.ln_Name = ConfigNode->cn_ID;
        strncpy( ConfigNode->cn_ID, ConfigID, STRING_LENGTH);

        if( !( ConfigNode->cn_Node.ln_Type == CONFIGNODE_ID))
            AddTail(( struct List *) ConfigList, ( struct Node *) ConfigNode);

        ConfigNode->cn_Node.ln_Type = CONFIGSUB_ID;

        #ifdef DEEPDEBUG
        FPrintfLog( "%ld: %s:", __LINE__, ConfigNode->cn_Node.ln_Name);
        #endif

        return( ConfigNode->cn_Value);
    }
    else SystemError();
} //-
//+ void ClearList( struct List *List)
void ClearList( struct List *List)
{
    struct ConfigNode   *ThisNode;
    struct ConfigNode   *NextNode;

    if( List)
    {
        ThisNode = ( struct Node *) ( List->lh_Head);

        while( NextNode = ( struct Node *) ( ThisNode->cn_Node.ln_Succ))
        {
            if( ThisNode->cn_Node.ln_Type == CONFIGSUB_ID)
                ClearList(( struct ConfigNode *) ThisNode->cn_Value);

            FreeVec( ThisNode);
            ThisNode = NextNode;
        }

        FreeVec( List);
    }
} //-
//+ void PutList( struct List *List, BPTR ConfigFileHandle, long Iteration)
void PutList( struct List *List, BPTR ConfigFileHandle, long Iteration)
{
    struct ConfigNode   *ThisNode;
    struct ConfigNode   *NextNode;

    char    SpaceString[ 80 ] = "                                                                                ";
    char    ConfigString[ 120 ];

    if( List)
    {
        SpaceString[ Iteration * 4 ] = 0x00;

        ThisNode = ( struct Node *) ( List->lh_Head);

        while( NextNode = ThisNode->cn_Node.ln_Succ)
        {
            if( FPuts( ConfigFileHandle, SpaceString)) SystemError();

            if( ThisNode->cn_Node.ln_Type == CONFIGSUB_ID)
            {
                sprintf( ConfigString, "( %s\n", ThisNode->cn_Node.ln_Name);
                FPuts( ConfigFileHandle, ConfigString);

                PutList(( struct ConfigNode *) ThisNode->cn_Value, ConfigFileHandle, Iteration + 1);

                FPuts( ConfigFileHandle, SpaceString);
                FPuts( ConfigFileHandle,")\n");
            }
            else
            {
                if( *ThisNode->cn_String)
                    sprintf( ConfigString, "( %s \"%s\" )\n", ThisNode->cn_Node.ln_Name, ThisNode->cn_String);
                else
                    sprintf( ConfigString, "( %s %lu )\n", ThisNode->cn_Node.ln_Name, ThisNode->cn_Value);

                FPuts( ConfigFileHandle, ConfigString);
            }

            ThisNode = NextNode;
        }
    }
} //-
