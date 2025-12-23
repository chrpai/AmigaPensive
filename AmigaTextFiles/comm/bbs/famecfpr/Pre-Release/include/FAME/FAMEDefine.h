
/*
 * FAME Defines
 * ~~~~~~~~~~~~
 *
 * $VER: FAMEDefine.h v1.5
 *
 * Code by: David 'Strider/tRSi' Wettig
 * E-Mail:  strider@trsi.de
 * URL:     http://www.trsi.de/inno/strider/index.html
 *
 * Copyright note: FAME and all it's contents like programs,
 *                 tools, doors, documents, texts, sources,
 *                 standards like the FIM (FAME door port standard),
 *                 the FAMEToolPort and all depending things,
 *                 the FAMECoProc standard and (data) file formats
 *                 are copyrighted © 1993 - 1997 by David Wettig.
 *
 *                 The include file you are currently reading
 *                 is also copyrighted and you are only allowed
 *                 to use it to develop own programs for the
 *                 FAME BBS System and you are *NOT* allowed
 *                 to use it for own projects like Mailbox
 *                 programs or whatever without my permission.
 */

#ifndef FAME_FAMEDEFINE_H
#define FAME_FAMEDEFINE_H

/*
 * Header offset define.
 */

#define FAMECFG_HEADEROFFSET             61

/*
 * UL/DLSTAT Mode defines
 */

#define ULSTAT_UPLOAD_MSGBASE            -1  /* File attach to message          */
#define ULSTAT_UPLOAD_UL                 0    /* Upload local                    */
#define ULSTAT_UPLOAD_UG                 1    /* Upload global                  */
#define ULSTAT_UPLOAD_U                  2    /* Upload userdata depending      */
#define ULSTAT_UPLOAD_RZ                 3    /* Upload via Z-Modem identifying  */
#define ULSTAT_UPLOAD_PARTUL_RESUME      4    /* Resuming a part upload          */
#define ULSTAT_UPLOAD_DOOR               5    /* Door uses upload command        */
#define NORMAL_DL_XFER                   6    /* Normal download                */
#define ASL_DL_XFER                      7    /* Download from ASL req.          */
#define DOOR_DL_NORMAL                   8    /* Normal Door download            */
#define DOOR_DL_LIST                     9    /* Door download from list        */
#define DOOR_DL_MSG                      10  /* Download of msg files          */

/*
 * Rawkey Codes
 */

#define HELP_CODE                        95
#define RAMIGA_CODE                      103
#define CURSOR_UP_CODE                   76
#define CURSOR_DOWN_CODE                 77
#define CURSOR_RIGHT_CODE                78
#define CURSOR_LEFT_CODE                 79
#define LSHIFT_CODE                      96
#define RSHIFT_CODE                      97
#define SPACE_CODE                       64
#define F01_CODE                         80
#define F02_CODE                         81
#define F03_CODE                         82
#define F04_CODE                         83
#define F05_CODE                         84
#define F06_CODE                         85
#define F07_CODE                         86
#define F08_CODE                         87
#define F09_CODE                         88
#define F10_CODE                         89
#define KEY_0_CODE                       1
#define KEY_1_CODE                       2
#define KEY_2_CODE                       3
#define KEY_3_CODE                       4
#define KEY_4_CODE                       5
#define KEY_5_CODE                       6
#define KEY_6_CODE                       7
#define KEY_7_CODE                       8
#define KEY_8_CODE                       9
#define KEY_9_CODE                       10
#define KEY_A_CODE                       32
#define KEY_B_CODE                       53
#define KEY_C_CODE                       51
#define KEY_D_CODE                       34
#define KEY_E_CODE                       18
#define KEY_F_CODE                       35
#define KEY_G_CODE                       36
#define KEY_H_CODE                       37
#define KEY_I_CODE                       23
#define KEY_L_CODE                       40
#define KEY_M_CODE                       55
#define KEY_N_CODE                       54
#define KEY_O_CODE                       24
#define KEY_P_CODE                       25
#define KEY_R_CODE                       19
#define KEY_S_CODE                       33
#define KEY_T_CODE                       20
#define KEY_U_CODE                       22
#define KEY_Y_CODE                       49
#define KEY_QUESTMARK_CODE               11
#define KEY_ROOF_CODE                    43
#define KEY_DBLCROSS_CODE                43
#define KEY_ESC_CODE                     69
#define KEY_TAB_CODE                     66

/*
 * Console key codes
 */

#define CON_CURSOR_UP_CODE               65
#define CON_CURSOR_DOWN_CODE             66
#define CON_CURSOR_RIGHT_CODE            67
#define CON_CURSOR_LEFT_CODE             68
#define CON_CURSOR_SHIFT_UP_CODE         84
#define CON_CURSOR_SHIFT_DOWN_CODE       83
#define CON_F01_CODE                     48
#define CON_F02_CODE                     49
#define CON_F03_CODE                     50
#define CON_F04_CODE                     51
#define CON_F05_CODE                     52
#define CON_F06_CODE                     53
#define CON_F07_CODE                     54
#define CON_F08_CODE                     55
#define CON_F09_CODE                     56
#define CON_F10_CODE                     57
#define CON_HELP_CODE                    63
#define CON_SPACE_CODE                   32
#define CON_ESC_CODE                     27
#define CON_BRAKET_TO_RIGHT_CODE         91
#define CON_CSI_CODE                     155
#define CON_TILDE_CODE                   126
#define CON_SHIFTED_FKEY_CODE            49

/*
 * Other defines
 */

#define FOREVER                          for(;;)

/*
 * BackFlag defines
 */

#define TIMEOUT                          -3
#define NO_CARRIER                       -4
#define DOOR_TIMEOUT                     -5
#define GOODBYE                          -7
#define EXITCHAT                         -8
#define USERKICK                         10
#define USERKICK_NODEEXIT                110
#define USERKICK_NODEEXITOH              111
#define NODE_TIMEOUT                     1
#define USERLOGOFF                       2
#define USERTIMELIMIT                    3
#define RELOGIN                          4
#define ERROR_1                          201
#define ERROR_2                          202
#define ERROR_3                          203
#define ERROR_4                          204
#define ERROR_5                          205
#define ERROR_6                          206
#define ERROR_7                          207
#define ERROR_8                          208
#define ERROR_9                          209
#define ERROR_10                         210

/*
 * Where I am phases defines
 *
 * CalersUdLog defines & What User does defines & ENV defines & Server defines
 *
 * //<- means implemented!
 */

#define FAME_DROP_DTR                    -1  //<- whatlogon phases
#define FAME_AWAIT_CALL                  0    //<-
#define FAME_SYSOPLOGIN                  1    //<-
#define FAME_LOCALLOGIN                  2    //<-
#define FAME_MODEMLOGIN                  3    //<-
#define FAME_REMOTELOGIN                 4    //<-
#define FAME_TRAPDOORLOGIN               111

#define FAME_AWAITCONNECT                5    //<- whereiam phases
#define FAME_LOGGINGON                   6    //<-
#define FAME_IDLE                        7    //<-
#define FAME_DOWNLOAD                    8    //<-
#define FAME_UPLOAD                      9    //<-
#define FAME_READMAIL                    10  //<-
#define FAME_WRITEMAIL                   11  //<-
#define FAME_USERSTATUS                  12  //<-
#define FAME_ACCOUNTEDIT                 13  //<-
#define FAME_ZOOM                        14
#define FAME_FILELISTING                 15  //<-
#define FAME_BULLETINS                   16  //<-
#define FAME_VIEWFILES                   17  //<-
#define FAME_LOGOFF                      18  //<-
#define FAME_SHELL                       19
#define FAME_JOINCONF                    20  //<-
#define FAME_SYSOPCHAT                   21  //<-
#define FAME_NOTACTIVE                   22
#define FAME_REQCHAT                     23
#define FAME_CONNECT                     24
#define FAME_SCANMAIL                    25  //<-
#define FAME_SCANFILES                   26  //<-
#define FAME_SHUTDOWN                    27
#define FAME_MULTICHAT                   28
#define FAME_SUSPEND                     29
#define FAME_USERRESERVE                 30
#define FAME_FLAGEDIT                    31  //<-
#define FAME_WAITFORKEY                  32  //<-
#define FAME_SYSOPCOMENT                 33  //<-
#define FAME_BBSHELP                     34  //<-
#define FAME_NEWFILES                    35  //<-
#define FAME_PAGESYSOP                   36  //<-
#define FAME_TIME                        37  //<-
#define FAME_CHANGESTATS                 38  //<-
#define FAME_SEARCHFOR                   39  //<-
#define FAME_VIEWEDCAL                   40  //<-
#define FAME_VIEWEDUDL                   41  //<-
#define FAME_VIEWEDDOL                   42  //<-
#define FAME_EDITFILELST                 43  //<-
#define FAME_EDITFILES                   44  //<-
#define FAME_ADOSLIST                    45  //<-
#define FAME_ADOSDIR                     46  //<-
#define FAME_ADOSINFO                    47  //<-
#define FAME_WHO                         48
#define FAME_OLM                         49
#define FAME_OLC                         50
#define FAME_OLD                         51
#define FAME_OLG                         52
#define FAME_OLS                         53
#define FAME_OLT                         54
#define FAME_VIEWSYSOP                   55
#define FAME_DOWNSYSOP                   56
#define FAME_MOVE                        57
#define FAME_NUKE                        58
#define FAME_NEWUSER                     59  //<-
#define FAME_PWFAIL                      60  //<-
#define FAME_ENDNODE                     61
#define FAME_STARTNODE                   62
#define FAME_4ONGO                       63
#define FAME_STAR                        64
#define FAME_LINE                        65
#define FAME_ON                          66
#define FAME_OFF                         67
#define FAME_NAME                        68
#define FAME_SYSFSTLGN                   69  //<-
#define FAME_SYSLGN                      70  //<-
#define FAME_LOCFSTLGN                   71  //<-
#define FAME_LOCFSTRELGN                 72  //<-
#define FAME_LOCLGN                      73  //<-
#define FAME_REMFSTLGN                   74  //<-
#define FAME_REMLGN                      75  //<-
#define FAME_AUTOREJN                    76  //<-
#define FAME_FREEDOWN                    77
#define FAME_FREEUP                      78
#define FAME_ULFAIL                      79
#define FAME_DLFAIL                      80
#define FAME_BIRTHDAY                    81
#define FAME_SYSPWFAIL                   82  //<-
#define FAME_ULSTATS                     83
#define FAME_DLSTATS                     84
#define FAME_ULMOVED                     85
#define FAME_ENTEREALL                   86  //<-
#define FAME_RELOGIN                     87  //<-
#define FAME_SUSPEND_LOGON               88  //<-
#define FAME_EXTUSERED                   89  //<-
#define FAME_EXTSYSTEMED                 90  //<-
#define FAME_RESUME                      91
#define FAME_ICONIFY                     92
#define FAME_UNICONIFY                   93
#define FAME_PREPAREDOWNL                94  //<-
#define FAME_PREPAREUPL                  95  //<-
#define FAME_SHUTDOWNOH                  96
#define FAME_SHUTDOWNUK                  97
#define FAME_SHUTDOWNUKOH                98
#define FAME_DROPUSER                    99
#define FAME_NUPPWFAIL                   100  //<-
#define FAME_LOSTCARRIER                 101
#define FAME_PARTUPLOAD                  102
#define FAME_VIEWTEXT                    103
#define FAME_REJOINCONF                  104
#define FAME_MULTINODEUS                 105
#define FAME_SCRIPT                      106
#define FAME_SEPARATOR                   107 //<-
#define FAME_HYDRA                       108 //<-
#define FAME_SMODEM                      109 //<-
#define FAME_CONFPWFAIL                  110

#define DOORS                            999  //<-

/*
 * ServerCommands
 */

#define SERVER_SYSTEM                    2000
#define SERVER_MISC                      2002
#define SERVER_PROTOCOL                  2003
#define SERVER_FAMEMODULES               2004
#define SERVER_ACTIVITIES                2005
#define SERVER_ERROR                     2006
#define SERVER_INTERNDOORS               2007
#define SERVER_LANGLOCALE                2008
#define SERVER_SCREENS                   2009
#define SERVER_NODE                      2010
#define SERVER_SERIAL                    2011
#define SERVER_MODEM                     2012
#define SERVER_SCREEN                    2013
#define SERVER_CONF_DATA                 2014
#define SERVER_GetServerCnf              2015
#define SERVER_FDEBUG                    2016

#define SERVER_QuietNode                 3000
#define SERVER_APTR1                     3001
#define SERVER_APTR2                     3002
#define SERVER_LastConf                  3101
#define SERVER_ForeGround                3102
#define SERVER_NodeIsReady               3103
#define SERVER_ChatFlag                  3104
#define SERVER_DoorName                  3105
#define SERVER_NamePrompt                3106
#define SERVER_InitModem                 3107
#define SERVER_TransferCPS               3108
#define SERVER_ServerCommand             3109
#define SERVER_NodeEnds                  3110
#define SERVER_NoShutDown                3111
#define SERVER_NewAction                 3112
#define SERVER_Complete                  3113
#define SERVER_SerialStart               3114
#define SERVER_SerialReady               3115
#define SERVER_BatchStart                3116
#define SERVER_BatchReady                3117
#define SERVER_UserReserve               3118
#define SERVER_SetColors                 3119
#define SERVER_OLC                       3120
#define SERVER_OLD                       3121
#define SERVER_OLG                       3122
#define SERVER_OLS                       3123
#define SERVER_OLT                       3124
#define SERVER_NewActionCPS              3125
#define SERVER_NewUserNamePrompt         3126
#define SERVER_DisplayTextToNode         3127
#define SERVER_DisplayAwaitTextNode      3128
#define SERVER_AwaitBulletinStart        3129
#define SERVER_AwaitBulletinStop         3130
#define SERVER_RequestAwaitBullMode      3131
#define SERVER_FIMCommand                3132

#define SERVER_StartNode                 4000
#define SERVER_SetSignal                 4001
#define SERVER_DropDTR                   4002

#define SERVER_BetaRequest               9999
#define SERVER_UpdateNodes               10000
#define SERVER_StringToNode              10001
#define SERVER_StartNodeCmd              10002
#define SERVER_BeADoorOnNode             10003
#define SERVER_OpenDoorPort              10004
#define SERVER_CloseDoorPort             10005
#define SERVER_DirStringToNd             10006
#define SERVER_DirStringToNds            10007
#define SERVER_NodeSpyStart              10008
#define SERVER_NodeSpyEnd                10009
#define SERVER_NodeSpyData               10010
#define SERVER_LastAction                10011
#define SERVER_FCheckResList             10012
#define SERVER_FCheckFileReady           10013
#define SERVER_FileCheckOk               10014
#define SERVER_DoCallerslog              10015
#define SERVER_PostFileReady             10016
#define SERVER_SendString                10017
#define SERVER_FGPSOpen                  10018

#define SERVER_CPTPStart                 20017
#define SERVER_CPTPKill                  20018
#define SERVER_TrapDoor                  20019
#define SERVER_RemoteStartNode           20020
#define SERVER_RemoteQuitNode            20021
#define SERVER_RemoteQuitNodeOH          20022
#define SERVER_CPTPRemoveCPReq           20023
#define SERVER_CPTPEditCPReq             20024
#define SERVER_CPTPActionCPReq           20025

/*
 * FIM command maximum defines
 */

#define MC_MAXIMUM                       9
#define NR_MAXIMUM                       199
#define NC_MAXIMUM                       399
#define CF_MAXIMUM                       599
#define SR_MAXIMUM                       699
#define SC_MAXIMUM                       799
#define AR_MAXIMUM                       899
#define AC_MAXIMUM                       999
#define RD_MAXIMUM                       99999

/*
 * HydraCommands
 */

#define FAMEHYDRA_OPENWINDOWS            1
#define FAMEHYDRA_CLOSEWINDOWS           2
#define FAMEHYDRA_ABORT                  3

/*
 * Server Info Window defines
 */

#define SVI_LASTUSERS                    1
#define SVI_LASTLOCATIONS                2
#define SVI_LASTACTIONS                  3
#define SVI_LASTBAUDCPS                  4
#define SVI_LASTUPLOADS                  5
#define SVI_LASTDOWNLOADS                6
#define SVI_FLAGGEDFILES                 7

/*
 * Misc. defines
 */

#define MILLION                          1000000

/*
 * A port signal mask
 */

#define PORTMASK(P)                      ( 1 << (((struct MsgPort *)( P )) -> mp_SigBit ))

/*
 * A handy signal macro
 */

#define ClrSignal(Signals)               SetSignal( 0, Signals )

/*
 * Macro to count the number of elements
 */

#define NumElements(s)                   ( sizeof( s ) / sizeof(( s )[ 0 ] ))

/*
 * Spread a byte across a long word
 */

#define SPREAD(v)                        ((ULONG)( v ) << 24 | (ULONG)( v ) << 16 | (ULONG)( v ) << 8 | ( v ))

/*
 * Spread a byte across a word
 */

#define SPREADW(v)                       ((ULONG)( v ) << 4 | ( v ))

/*
 * xpr opts Normaly unused! See: FAMECnf.ProtocolOptsBuffer
 */

#define ZMDM_INIT                        "TN,OS,B32,F0,E10,AY,DN,KY,SN,RN,P"
#define ZMDM_INIT_RESUME                 "TN,OR,B32,F0,E10,AY,DN,KY,SN,RN,P"

#define BLOCK_1KB                        "M1024"
#define BLOCK_2KB                        "M2048"
#define BLOCK_3KB                        "M3072"
#define BLOCK_4KB                        "M4096"
#define BLOCK_5KB                        "M5120"
#define BLOCK_6KB                        "M6144"
#define BLOCK_7KB                        "M7168"
#define BLOCK_8KB                        "M8192"

/*
 * Door Upload and Download defines
 */

#define DOORUD_DONOTCOUNTBYTES           0
#define DOORUD_COUNTBYTES                100

/*
 * Number flag file list defines
 */

#define FNFL_OVERWRITE                   (1 << 0)

/*
 * File flag list flag defines
 */

#define FFFL_FREEFILE                    (1 << 0)
#define FFFL_NUMDELFLAG                  (1 << 1)
#define FFFL_NODOWNLOAD                  (1 << 2)
#define FFFL_NODELETE                    (1 << 3)

/*
 * Display case defines
 */

#define FAME_CONSER                      0
#define FAME_CON                         1
#define FAME_SER                         2

/*
 * MainPart DebugLog Types
 */

#define MPDLT_GLO_DEBUGLOG               0
#define MPDLT_FIM_DEBUGLOG               1
#define MPDLT_XIM_DEBUGLOG               2
#define MPDLT_AIM_DEBUGLOG               3
#define MPDLT_TIM_DEBUGLOG               4

/*
 * Interface Module defines
 */

#define IMT_DOORCONFIG                   -1
#define IMT_FIM                          0
#define IMT_XIM                          1
#define IMT_TIM                          2
#define IMT_AIM                          3
#define IMT_CIM                          4
#define IMT_RIM                          5
#define IMT_SIM                          6
#define IMT_TXT                          7
#define IMT_SND                          8
#define IMT_SPK                          9
#define IMT_GFM                          10

/*
 * XIM MultiComPort Chat Status defines
 */

#define CHAT_NONE                        1
#define CHAT_TEXT                        2
#define CHAT_IDLE                        3
#define CHAT_EXIT                        4
#define CHAT_ENTER                       5
#define CHAT_NTXT                        6

/*
 * XIM MultiComPort Node Range Check defines
 */

#define AEMCP_MINNODE                    -1
#define AEMCP_MAXNODE                    9

/*
 * XIM ENV defines
 */

#define ENV_INACTIVE                     -1
#define ENV_IDLE                         0
#define ENV_DOWNLOADING                  1
#define ENV_UPLOADING                    2
#define ENV_DOORS                        3
#define ENV_MAIL                         4
#define ENV_STATS                        5
#define ENV_ACCOUNT                      6
#define ENV_ZOOM                         7
#define ENV_FILES                        8
#define ENV_BULLETINS                    9
#define ENV_VIEWING                      10
#define ENV_ACCOUNTSEQ                   11
#define ENV_LOGOFF                       12
#define ENV_SYSOP                        13
#define ENV_SHELL                        14
#define ENV_EMACS                        15
#define ENV_JOIN                         16
#define ENV_CHAT                         17
#define ENV_NOTACTIVE                    18
#define ENV_REQ_CHAT                     19
#define ENV_CONNECT                      20
#define ENV_LOGGINGON                    21
#define ENV_AWAITCONNECT                 22
#define ENV_SCANNING                     23
#define ENV_SHUTDOWN                     24
#define ENV_MULTICHAT                    25
#define ENV_SUSPEND                      26
#define ENV_RESERVE                      27
#define ENV_OLM                          28
#define ENV_UNKNOWN                      29

/*
 * FAME (enter) Mail ddefines
 */

#define MSGTYPE_OLM                      -1
#define MSGTYPE_WRITE_EDIT_TXT           0
#define MSGTYPE_COMMENT_MAIL             1
#define MSGTYPE_NORMAL_MAIL              2
#define MSGTYPE_REPLY_MAIL               3
#define MSGTYPE_PART_MAIL                4
#define MSGTYPE_EDIT_MAIL                5
#define MSGTYPE_DOOR_MAIL                10

/*
 * FAME Mail Types
 */

#define MAILTYPE_PUBLIC                  0
#define MAILTYPE_PRIVATE                 1
#define MAILTYPE_ALL                     2
#define MAILTYPE_EALL                    3
#define MAILTYPE_UEALL                   4

/*
 * FAMEToolPort return codes (struct FAMEToolPortMsg -> ftpm_ReturnCode)
 */

#define FAMETPRC_NodeNotAvailable        12
#define FAMETPRC_FIMCommandInProgress    11
#define FAMETPRC_NoTaskAddressPointer    10
#define FAMETPRC_AlreadyRegistered       9
#define FAMETPRC_MustRegisterFirst       8
#define FAMETPRC_InvalidIdentifyCode     7
#define FAMETPRC_NoToolNameSet           6
#define FAMETPRC_CommandAborted          5
#define FAMETPRC_CommandNotImplemented   4
#define FAMETPRC_ComandDenied            3
#define FAMETPRC_CommandDoesNotExist     2
#define FAMETPRC_CommandNotSuccessful    1
#define FAMETPRC_CommandSuccessful       0
#define FAMETPRC_RemoveToolImmediately   -1
#define FAMETPRC_WrongToolIDOwner        -2
#define FAMETPRC_UnknownError            -3
#define FAMETPRC_FAMEToolPortClosed      -4
#define FAMETPRC_ServerEndQuitTools      -5

/*
 * FAMECoProc return codes (struct FAMECoProcessMsg -> fcpm_ReturnCode)
 */

#define FAMECPRC_CoProcNameActFlgUsed    7
#define FAMECPRC_NoCoProcNameSet         6
#define FAMECPRC_CommandAborted          5
#define FAMECPRC_CommandNotImplemented   4
#define FAMECPRC_ComandDenied            3
#define FAMECPRC_CommandDoesNotExist     2
#define FAMECPRC_CommandNotSuccessful    1
#define FAMECPRC_CommandSuccessful       0
#define FAMECPRC_RemoveToolImmediately   -1
#define FAMECPRC_WrongReqID              -2
#define FAMECPRC_UnknownError            -3
#define FAMECPRC_FAMEToolPortClosed      -4

/*
 * FAME Mail/File Scanner defines
 */

#define FAMESCAN_NOMAILFILESCAN          0
#define FAMESCAN_MAILFILESCAN            1
#define FAMESCAN_MAILSCAN                2
#define FAMESCAN_FILESCAN                3
#define FAMESCAN_PROMPT                  4

/*
 * FAME Message Auto Delete flag defines
 */

#define MAD_NEVER                        0
#define MAD_DEFAULT                      -1
#define MAD_MIN                          -2
#define MAD_MAX                          -3
#define MAD_PROMPT                       -4

/*
 * DiskLoadBase defines
 */

#define DLB_ADDDOWNLOADLIST              0
#define DLB_RESTRICTFILESLIST            1
#define DLB_NAMESNOTALLOWEDLIST          2
#define DLB_ADDUPLOADLIST                3
#define DLB_AVAILSYSOPSLIST              4

/*
 * FDCMP defines I
 *
 * (struct FAMEToolPortLst -> ftpl_BitRequests1)
 * (struct FAMEToolPortMsg -> ftpm_BitRequests1)
 * (struct FAMEToolNotifyMsg -> ftnm_BitRequests1)
 */

#define FDCMP_FAME_AWAITCONNECT          (1 << 0)
#define FDCMP_FAME_LOGGINGON             (1 << 1)
#define FDCMP_FAME_IDLE                  (1 << 2)
#define FDCMP_FAME_DOWNLOAD              (1 << 3)
#define FDCMP_FAME_UPLOAD                (1 << 4)
#define FDCMP_FAME_READMAIL              (1 << 5)
#define FDCMP_FAME_WRITEMAIL             (1 << 6)
#define FDCMP_FAME_USERSTATUS            (1 << 7)
#define FDCMP_FAME_ACCOUNTEDIT           (1 << 8)
#define FDCMP_FAME_ZOOM                  (1 << 9)
#define FDCMP_FAME_FILELISTING           (1 << 10)
#define FDCMP_FAME_BULLETINS             (1 << 11)
#define FDCMP_FAME_VIEWFILES             (1 << 12)
#define FDCMP_FAME_LOGOFF                (1 << 13)
#define FDCMP_FAME_SHELL                 (1 << 14)
#define FDCMP_FAME_JOINCONF              (1 << 15)
#define FDCMP_FAME_SYSOPCHAT             (1 << 16)
#define FDCMP_FAME_NOTACTIVE             (1 << 17)
#define FDCMP_FAME_REQCHAT               (1 << 18)
#define FDCMP_FAME_CONNECT               (1 << 19)
#define FDCMP_FAME_SCANMAIL              (1 << 20)
#define FDCMP_FAME_SCANFILES             (1 << 21)
#define FDCMP_FAME_SHUTDOWN              (1 << 22)
#define FDCMP_FAME_MULTICHAT             (1 << 23)
#define FDCMP_FAME_SUSPEND               (1 << 24)
#define FDCMP_FAME_USERRESERVE           (1 << 25)
#define FDCMP_FAME_FLAGEDIT              (1 << 26)
#define FDCMP_FAME_WAITFORKEY            (1 << 27)
#define FDCMP_FAME_SYSOPCOMENT           (1 << 28)
#define FDCMP_FAME_BBSHELP               (1 << 29)
#define FDCMP_FAME_NEWFILES              (1 << 30)
#define FDCMP_FAME_PAGESYSOP             (1 << 31)

/*
 * FDCMP defines II
 *
 * (struct FAMEToolPortLst -> ftpl_BitRequests2)
 * (struct FAMEToolPortMsg -> ftpm_BitRequests2)
 * (struct FAMEToolNotifyMsg -> ftnm_BitRequests2)
 */

#define FDCMP_FAME_TIME                  (1 << 0)
#define FDCMP_FAME_CHANGESTATS           (1 << 1)
#define FDCMP_FAME_SEARCHFOR             (1 << 2)
#define FDCMP_FAME_VIEWEDCAL             (1 << 3)
#define FDCMP_FAME_VIEWEDUDL             (1 << 4)
#define FDCMP_FAME_VIEWEDDOL             (1 << 5)
#define FDCMP_FAME_EDITFILELST           (1 << 6)
#define FDCMP_FAME_EDITFILES             (1 << 7)
#define FDCMP_FAME_ADOSLIST              (1 << 8)
#define FDCMP_FAME_ADOSDIR               (1 << 9)
#define FDCMP_FAME_ADOSINFO              (1 << 10)
#define FDCMP_FAME_WHO                   (1 << 11)
#define FDCMP_FAME_OLM                   (1 << 12)
#define FDCMP_FAME_OLC                   (1 << 13)
#define FDCMP_FAME_OLD                   (1 << 14)
#define FDCMP_FAME_OLG                   (1 << 15)
#define FDCMP_FAME_OLS                   (1 << 16)
#define FDCMP_FAME_OLT                   (1 << 17)
#define FDCMP_FAME_VIEWSYSOP             (1 << 18)
#define FDCMP_FAME_DOWNSYSOP             (1 << 19)
#define FDCMP_FAME_MOVE                  (1 << 20)
#define FDCMP_FAME_NUKE                  (1 << 21)
#define FDCMP_FAME_NEWUSER               (1 << 22)
#define FDCMP_FAME_PWFAIL                (1 << 23)
#define FDCMP_FAME_STARTNODE             (1 << 24)
#define FDCMP_FAME_SYSFSTLGN             (1 << 25)
#define FDCMP_FAME_SYSLGN                (1 << 26)
#define FDCMP_FAME_LOCFSTLGN             (1 << 27)
#define FDCMP_FAME_LOCFSTRELGN           (1 << 28)
#define FDCMP_FAME_LOCLGN                (1 << 29)
#define FDCMP_FAME_REMFSTLGN             (1 << 30)
#define FDCMP_FAME_REMLGN                (1 << 31)

/*
 * FDCMP defines III
 *
 * (struct FAMEToolPortLst -> ftpl_BitRequests3)
 * (struct FAMEToolPortMsg -> ftpm_BitRequests3)
 * (struct FAMEToolNotifyMsg -> ftnm_BitRequests3)
 */

#define FDCMP_FAME_AUTOREJN              (1 << 0)
#define FDCMP_FAME_FREEDOWN              (1 << 1)
#define FDCMP_FAME_FREEUP                (1 << 2)
#define FDCMP_FAME_ULFAIL                (1 << 3)
#define FDCMP_FAME_DLFAIL                (1 << 4)
#define FDCMP_FAME_BIRTHDAY              (1 << 5)
#define FDCMP_FAME_SYSPWFAIL             (1 << 6)
#define FDCMP_FAME_NUPPWFAIL             (1 << 7)
#define FDCMP_FAME_ULSTATS               (1 << 8)
#define FDCMP_FAME_DLSTATS               (1 << 9)
#define FDCMP_FAME_ULMOVED               (1 << 10)
#define FDCMP_FAME_ENTEREALL             (1 << 11)
#define FDCMP_FAME_RELOGIN               (1 << 12)
#define FDCMP_FAME_SUSPEND_LOGON         (1 << 13)
#define FDCMP_FAME_RESUME                (1 << 14)
#define FDCMP_FAME_PREPAREDOWNL          (1 << 15)
#define FDCMP_FAME_PREPAREUPL            (1 << 16)
#define FDCMP_FAME_SHUTDOWNOH            (1 << 17)
#define FDCMP_FAME_LOSTCARRIER           (1 << 18)
#define FDCMP_FAME_PARTUPLOAD            (1 << 19)
#define FDCMP_FAME_VIEWTEXT              (1 << 20)
#define FDCMP_FAME_SCRIPT                (1 << 21)
#define FDCMP_DOORS                      (1 << 22)
#define FDCMP_SERVER_NodeIsReady         (1 << 23)
#define FDCMP_SERVER_SCREENS             (1 << 24)
#define FDCMP_SERVER_NODE                (1 << 25)
#define FDCMP_SERVER_SYSTEM              (1 << 26)
#define FDCMP_SERVER_MISC                (1 << 27)
#define FDCMP_SERVER_PROTOCOL            (1 << 28)
#define FDCMP_SERVER_FDEBUG              (1 << 29)
#define FDCMP_SERVER_SERIAL              (1 << 30)
#define FDCMP_SERVER_MODEM               (1 << 31)

/*
 * FDCMP defines IV
 *
 * (struct FAMEToolPortLst -> ftpl_BitRequests4)
 * (struct FAMEToolPortMsg -> ftpm_BitRequests4)
 * (struct FAMEToolNotifyMsg -> ftnm_BitRequests4)
 */

#define FDCMP_SERVER_SCREEN              (1 << 0)
#define FDCMP_SERVER_FAMEMODULES         (1 << 1)
#define FDCMP_SERVER_ACTIVITIES          (1 << 2)
#define FDCMP_SERVER_ERROR               (1 << 3)
#define FDCMP_SERVER_INTERNDOORS         (1 << 4)
#define FDCMP_SERVER_CONF_DATA           (1 << 5)
#define FDCMP_SERVER_GetServerCnf        (1 << 6)
#define FDCMP_SERVER_DoorName            (1 << 7)
#define FDCMP_SERVER_NewAction           (1 << 8)
#define FDCMP_SERVER_NewActionCPS        (1 << 9)
#define FDCMP_SERVER_ChatFlag            (1 << 10)
#define FDCMP_SERVER_NamePrompt          (1 << 11)
#define FDCMP_SERVER_NewUserNamePrompt   (1 << 12)
#define FDCMP_SERVER_TransferCPS         (1 << 13)
#define FDCMP_SERVER_InitModem           (1 << 14)
#define FDCMP_SERVER_NodeEnds            (1 << 15)
#define FDCMP_SERVER_NoShutDown          (1 << 16)
#define FDCMP_SERVER_Complete            (1 << 17)
#define FDCMP_SERVER_ServerCommand       (1 << 18)
#define FDCMP_SERVER_StartNode           (1 << 19)
#define FDCMP_SERVER_UpdateNodes         (1 << 20)
#define FDCMP_SERVER_StringToNode        (1 << 21)
#define FDCMP_SERVER_StartNodeCmd        (1 << 22)
#define FDCMP_SERVER_BeADoorOnNode       (1 << 23)
#define FDCMP_SERVER_OpenDoorPort        (1 << 24)
#define FDCMP_SERVER_CloseDoorPort       (1 << 25)
#define FDCMP_SERVER_DirStringToNd       (1 << 26)
#define FDCMP_SERVER_DirStringToNds      (1 << 27)
#define FDCMP_SERVER_SerialStart         (1 << 28)
#define FDCMP_SERVER_SerialReady         (1 << 29)
#define FDCMP_SERVER_BatchStart          (1 << 30)
#define FDCMP_SERVER_BatchReady          (1 << 31)

/*
 * FDCMP defines V
 *
 * (struct FAMEToolPortLst -> ftpl_BitRequests5)
 * (struct FAMEToolPortMsg -> ftpm_BitRequests5)
 * (struct FAMEToolNotifyMsg -> ftnm_BitRequests5)
 */

#define FDCMP_SERVER_QuietNode           (1 << 0)
#define FDCMP_SERVER_UserReserve         (1 << 1)
#define FDCMP_SERVER_NodeSpyData         (1 << 2)
#define FDCMP_SERVER_SetColors           (1 << 3)
#define FDCMP_SERVER_LastAction          (1 << 4)
#define FDCMP_SERVER_DropDTR             (1 << 5)
#define FDCMP_SERVER_AwaitBulletinStart  (1 << 6)
#define FDCMP_SERVER_AwaitBulletinStop   (1 << 7)
#define FDCMP_SERVER_CPTPStart           (1 << 8)
#define FDCMP_SERVER_CPTPKill            (1 << 9)
#define FDCMP_SERVER_TrapDoor            (1 << 10)
#define FDCMP_SERVER_RemoteStartNode     (1 << 11)
#define FDCMP_SERVER_RemoteQuitNode      (1 << 12)
#define FDCMP_SERVER_RemoteQuitNodeOH    (1 << 13)
#define FDCMP_SERVER_FIMCommandReturn    (1 << 14)
#define FDCMP_SERVER_CPTPRemoveCPReq     (1 << 15)
#define FDCMP_SERVER_CPTPEditCPReq       (1 << 16)
#define FDCMP_SERVER_CPTPActionCPReq     (1 << 17)

/*
 * FDCMP define types
 */

#define FDCMPBitReq_1                    1
#define FDCMPBitReq_2                    2
#define FDCMPBitReq_3                    3
#define FDCMPBitReq_4                    4
#define FDCMPBitReq_5                    5
#define FDCMPBitReq_6                    6
#define FDCMPBitReq_7                    7
#define FDCMPBitReq_8                    8
#define FDCMPBitReq_9                    9
#define FDCMPBitReq_10                   10

/*
 * FDCMP - tool return codes
 */

#define FTNMRC_HOLD                      0
#define FTNMRC_REMOVE                    1

/*
 * FAME CoProc / ToolPort secondary Server process command defines
 */

#define CPTP_UpFAMECPList                1
#define CPTP_UpFAMETPList                2
#define CPTP_UpFAMECPListEntry           3
#define CPTP_UpFAMETPListEntry           4
#define CPTP_AddFAMECPListEntry          5
#define CPTP_AddFAMETPListEntry          6
#define CPTP_RemFAMECPListEntry          7
#define CPTP_RemFAMETPListEntry          8
#define CPTP_UpFAMECPTPLists             9
#define CPTP_UpFAMECPEdited              10

/*
 * FAME CoProc struct FAMECoProcRequest -> fcpr_Flag1 defines
 */

#define FCPR_ACTIVATED                   (1 << 0)
#define FCPR_SVINT_FLGDEL                (1 << 31)

/*
 * FAMEInfoStruct defines
 */

#define FIS_FAMEInfoList                 1

/*
 * Server About gadget type
 */

#define SVAbout_Default                  0
#define SVAbout_QuitAbout                1
#define SVAbout_MainCode                 2
#define SVAbout_DevelopTeam              3
#define SVAbout_SupportTeam              4
#define SVAbout_SupportBBS               5
#define SVAbout_BetaTesters              6
#define SVAbout_DistSites                7
#define SVAbout_NotUsed1                 8
#define SVAbout_CopyRContact             9
#define SVAbout_FAMEInfo                 10

/*
 * Server Gadget Bank defines
 */

#define SVGadBnk_SystemBank1             0
#define SVGadBnk_SystemBank2             1
#define SVGadBnk_UserBank1               2
#define SVGadBnk_UserBank2               3

/*
 * Last 10 Callers/Uploaders/Downloaders defines
 */

#define LTCUD_Callers                    0
#define LTCUD_Uploaders                  1
#define LTCUD_Downloaders                2

/*
 * Server Nodelist online Node defines
 */

#define SVLSTNO_STARTING                 -1
#define SVLSTNO_SHUTDOWN                 0
#define SVLSTNO_ONLINE                   1

/*
 * Server CP / TP defines
 */

#define CPTP_ISCP                        0
#define CPTP_ISTP                        1

/*
 * Server StartProcess defines
 */

#define STARTPROC_MinNode                0
#define STARTPROC_SystemEditor           -1
#define STARTPROC_UserEditor             -2
#define STARTPROC_ServerInfoTool         -3
#define STARTPROC_SystemEdNewNode        -4
#define STARTPROC_CoProcSVAction         -5
#define STARTPROC_CoProcSVEdit           -6
#define STARTPROC_UserBtnCmd0            -10
#define STARTPROC_UserBtnCmd1            -11
#define STARTPROC_UserBtnCmd2            -12
#define STARTPROC_UserBtnCmd3            -13
#define STARTPROC_UserBtnCmd4            -14
#define STARTPROC_UserBtnCmd5            -15
#define STARTPROC_UserBtnCmd6            -16
#define STARTPROC_UserBtnCmd7            -17
#define STARTPROC_UserBtnCmd8            -18
#define STARTPROC_UserBtnCmd9            -19
#define STARTPROC_UserBtnCmd10           -20
#define STARTPROC_UserBtnCmd11           -21
#define STARTPROC_UserBtnCmd12           -22
#define STARTPROC_UserBtnCmd13           -23
#define STARTPROC_UserBtnCmd14           -24
#define STARTPROC_UserBtnCmd15           -25
#define STARTPROC_UserBtnCmd16           -26
#define STARTPROC_UserBtnCmd17           -27
#define STARTPROC_UserBtnCmd18           -28
#define STARTPROC_UserBtnCmd19           -29
#define STARTPROC_UserBtnCmd20           -30
#define STARTPROC_UserBtnCmd21           -31
#define STARTPROC_UserBtnCmd22           -32
#define STARTPROC_UserBtnCmd23           -33
#define STARTPROC_UserBtnCmd24           -34
#define STARTPROC_UserBtnCmd25           -35
#define STARTPROC_UserBtnCmd26           -36
#define STARTPROC_UserBtnCmd27           -37

/*
 * FAME Batch Types
 */

#define FBT_LogonBatches                 0
#define FBT_LogoffBatches                1
#define FBT_StartBatches                 2
#define FBT_ShutDownBatches              3
#define FBT_LogoffBatch                  4
#define FBT_ShutDownBatch                5
#define FBT_ULDLogoffBatches             6

/*
 * FAME Protocol Types
 *
 * 1-20 reserved for max 20 variable by the SysOp configurable
 * XPR transfer protocols (xpr<name>.library).
 */

#define FPT_HYDRA                        21
#define FPT_SMODEM                       22

/*
 * FAME Protocol Offsets
 */

#define FPO_HYDRA                        1
#define FPO_SMODEM                       2
#define FPO_PROMPT                       3

/*
 * FAME Protocol cases
 */

#define FPC_ALLCASES                     1
#define FPC_DEFAULT                      2
#define FPC_BEGINUPLOAD                  3
#define FPC_BEGINDOWNLOAD                4
#define FPC_MESSAGEATTACHUL              5
#define FPC_MESSAGEATTACHDL              6
#define FPC_DOORFILEUPLOAD               7
#define FPC_DOORFILEDOWNLOAD             8

/*
 * FileChecker process defines
 */

#define FFCC_CHECKFILE                   1
#define FFCC_REQUESTINFOLIST             2
#define FFCC_POSTFILE                    3

#define FFCD_OUTPUT                      (1 << 0)

#define FFCM_SKIPDUPECHECK               (1 << 0)

#define FCSB_FILEPOSTED                  (1 << 0)
#define FCSB_HOLDFILE                    (1 << 1)

#define FFCP_RESULT_OK                   RETURN_OK
#define FFCP_RESULT_NO_INTUILIB          RETURN_FAIL + 1
#define FFCP_RESULT_NO_FAMELIB           RETURN_FAIL + 2
#define FFCP_RESULT_NO_FILEIDLIB         RETURN_FAIL + 3
#define FFCP_RESULT_NO_NODEMSG           RETURN_FAIL + 4
#define FFCP_RESULT_NO_REPLYPORT         RETURN_FAIL + 5
#define FFCP_RESULT_NO_FIPATHSTR         RETURN_FAIL + 6
#define FFCP_RESULT_NO_CUTFILESTR        RETURN_FAIL + 7
#define FFCP_RESULT_NO_TEMPSTR           RETURN_FAIL + 8
#define FFCP_RESULT_NO_FCHECKPORT        RETURN_FAIL + 9
#define FFCP_RESULT_NO_DATASTRUCT        RETURN_FAIL + 10

/*
 * SystemEditor window direct defines (JUMP argument)
 */

#define SYSED_WND_Main                   0
#define SYSED_WND_SystemDatas            1
#define SYSED_WND_MoreSystemDatas        2
#define SYSED_WND_ServerSettings         3
#define SYSED_WND_NodeEdit               4
#define SYSED_WND_SerModScr              5
#define SYSED_WND_MoreNodeEdit           6    // For compatibility reason
#define SYSED_WND_MoreNodeEditI          6
#define SYSED_WND_Conferences            7
#define SYSED_WND_AccessSecure           8
#define SYSED_WND_Levels                 9
#define SYSED_WND_MoreLevels             10
#define SYSED_WND_ConferenceAccess       11
#define SYSED_WND_Presets                12
#define SYSED_WND_Activities             13
#define SYSED_WND_Doors                  14
#define SYSED_WND_rESERVED1              15   // SortConferencesWnd
#define SYSED_WND_rESERVED2              16   // SortConfPerformWnd
#define SYSED_WND_InternalDoors          17
#define SYSED_WND_ComputerTypes          18
#define SYSED_WND_ModemTypes             19
#define SYSED_WND_Screens                20
#define SYSED_WND_FileCheckers           21
#define SYSED_WND_TextLanguage           22
#define SYSED_WND_ExtXferMods            23
#define SYSED_WND_FAMEModules            24
#define SYSED_WND_ExternModules          25
#define SYSED_WND_UserFileBase           26
#define SYSED_WND_Protocols              27
#define SYSED_WND_DoonError              28
#define SYSED_WND_DoorPortNames          29
#define SYSED_WND_DoorConfig             30
#define SYSED_WND_FAMEDebug              31
#define SYSED_WND_About                  32
#define SYSED_WND_MoreNodeEditII         33
#define SYSED_WND_UserHandler            34
#define SYSED_WND_MoreConferences        35
#define SYSED_WND_PalettePens            36
#define SYSED_WND_AwayMessages           37
#define SYSED_WND_AutoDeleteMails        38

/*
 * GoodFile flags (also for FileChecker doors)
 */

#define FAMEGoodFile_F_FAILED            -1
#define FAMEGoodFile_P_PASSED            0
#define FAMEGoodFile_N_NOTCHECKED        1
#define FAMEGoodFile_D_DELETED           2
#define FAMEGoodFile_O_OVERWRITTEN       3

/*
 * FAMEFileChecker return codes
 */

#define FAMEGOODFILE_OK                  0
#define FAMEGOODFILE_UNTESTED            101
#define FAMEGOODFILE_FAILED              102

/*
 * FIM DoorPort return codes
 */

#define FIM_USERERROR                    10
#define FIM_CMDABORTED                   5
#define FIM_CMDNOTIMPLEMENTED            4
#define FIM_CMDDENIED                    3
#define FIM_CMDDOESNOTEXISTS             2
#define FIM_CMDNOTSUCCESSFUL             1
#define FIM_CMDSUCCESSFUL                0
#define FIM_DOORABORTREQUESTED           -1
#define FIM_CARRIERLOST                  -2
#define FIM_UNKNOWNERROR                 -3

/*
 * Additional FameDoorStartUp return codes
 */

#define FIM_DOORPORTNOTOPEN              -10

/*
 * FIM DoorPort flags
 */

#define FIMF_USERSIGMODE                 (1 << 0)
#define FIMF_NOCHECK                     (1 << 1)
#define FIMF_NOEXITONTRUERETCODE         (1 << 2)

/*
 * Flags for NR_PromptChars
 */

#define FPC_SIMPLE                       0
#define FPC_CHATMODE                     1
#define FPC_MSGEDITOR                    2
#define FPC_BULLVIEW                     3
#define FPC_PASSWORD                     4
#define FPC_UNUSED                       5
#define FPC_NORMAL                       6
#define FPC_UNIX                         7

/*
 * Level defines
 */

#define FLVL_ReadMessages                1
#define FLVL_EnterMessage                2
#define FLVL_CommenttoSysOp              3
#define FLVL_PageSysOp                   4
#define FLVL_ZoomMail                    5
#define FLVL_ReadBulletins               6
#define FLVL_WHOisOnline                 7
#define FLVL_UserStatus                  8
#define FLVL_ZippyTextSearch             9
#define FLVL_JoinConference              10
#define FLVL_FileListing                 11
#define FLVL_NewFiles                    12
#define FLVL_Upload                      13
#define FLVL_Download                    14
#define FLVL_ViewFiles                   15
#define FLVL_FreeResuming                16
#define FLVL_EditUserAccount             17
#define FLVL_EditPassword                18
#define FLVL_EditUserLocation            19
#define FLVL_EditUserCityState           20
#define FLVL_EditUserName                21
#define FLVL_EditPhonenumber             22
#define FLVL_Relogin                     23
#define FLVL_UploadStatus                24
#define FLVL_PublicMessageFiles          25
#define FLVL_PrivateMessageFiles         26
#define FLVL_AttachFile                  27
#define FLVL_MCIMessages                 28
#define FLVL_BreakChat                   29
#define FLVL_OverideChat                 30
#define FLVL_OverideTime                 31
#define FLVL_EAllMessages                32
#define FLVL_EditFiles                   33
#define FLVL_EditDirs                    34
#define FLVL_DeleteMessages              35
#define FLVL_AccountEditing              36
#define FLVL_SysOpCommands               37
#define FLVL_Shell                       38
#define FLVL_SysOpRead                   39
#define FLVL_SysOpView                   40
#define FLVL_SysOpDownload               41
#define FLVL_OverideDefaults             42
#define FLVL_GlobalUD                    43
#define FLVL_FastRelogin                 44
#define FLVL_FastGoodbye                 45
#define FLVL_ViewBinaryFiles             46
#define FLVL_ZoomFilelist                47
#define FLVL_NodeChat                    48
#define FLVL_Hydra                       49
#define FLVL_RIPGfx                      50
#define FLVL_CryptMsgs                   51
#define FLVL_EditUserBirthday            52
#define FLVL_DeleteMsginDays             53
#define FLVL_OverwriteFiles              54
#define FLVL_EditMessages                55
#define FLVL_KeepotherMessages           56
#define FLVL_OthUsonotherMsg             57
#define FLVL_SModem                      58
#define FLVL_BBSHelp                     59
#define FLVL_QuietMode                   60
#define FLVL_OnLineMessageOLM            61
#define FLVL_CaptureOnlineUserOLC        62
#define FLVL_DisconOnlineUserOLD         63
#define FLVL_OnlineUserChatOLG           64
#define FLVL_SpyOnlineUserOLS            65
#define FLVL_OnlineUserTimeOLT           66
#define FLVL_SysOpBinaryView             67
#define FLVL_ViewEditCallersLog          68
#define FLVL_ViewEditDoorLog             69
#define FLVL_ViewEditUDLog               70
#define FLVL_AmigaDOSList                71
#define FLVL_AmigaDOSDir                 72
#define FLVL_AmigaDOSInfo                73
#define FLVL_ConfAccessEditor            74
#define FLVL_ForceWHO                    75
#define FLVL_RealHidden                  76
#define FLVL_RenewedSubmission           77
#define FLVL_PriorityMsg                 78
#define FLVL_Obsolete                    79
#define FLVL_EditFileFlags               80

/*
 * Sort/Delete Conference Perform action bits
 */

#define SDConfPerformWnd_Perform         (1 << 0)
#define SDConfPerformWnd_UpdateCASets    (1 << 1)
#define SDConfPerformWnd_RenameCANSets   (1 << 2)
#define SDConfPerformWnd_UserDataFit     (1 << 3)
#define SDConfPerformWnd_UserAbort	     (1 << 31)

/*
 * xpr serial bits
 */

#define ST_PARTYON                       (1 << 0)
#define ST_PARTYODD                      (1 << 1)
#define ST_7WIRE                         (1 << 2)
#define ST_QBREAK                        (1 << 3)
#define ST_RADBOOGIE                     (1 << 4)
#define ST_SHARED                        (1 << 5)
#define ST_EOFMODE                       (1 << 6)
#define ST_XDISABLED                     (1 << 7)
#define ST_PARTYMARKON                   (1 << 8)
#define ST_PARTYMARK                     (1 << 9)
#define ST_2BITS                         (1 << 10)
#define ST_READ7                         (1 << 11)
#define ST_WRITE7                        (1 << 12)

/*
 * Definitions to access the line signal setting functions supported by
 * some IO serial boards (namely the ASDG board)
 */

#define SIOCMD_SETCTRLLINES              (CMD_NONSTD + 7)
#define SIOB_RTSB                        0
#define SIOB_DTRB                        1
#define SIOB_RTSF                        (1L << SIOB_RTSB)
#define SIOB_DTRF                        (1L << SIOB_DTRB)

/*
 * UserFlag1 defines (UserData)
 */

#define UD_SPY                           (1 << 0)
#define UD_EXPERT                        (1 << 1)
#define UD_NEWUSER                       (1 << 2)
#define UD_MSGCLS                        (1 << 3)
#define UD_FILECLS                       (1 << 4)
#define UD_UFLAG                         (1 << 5)
#define UD_DFLAG                         (1 << 6)
#define UD_UFBASE                        (1 << 7)
#define UD_HIDE                          (1 << 8)
#define UD_MSGROOM                       (1 << 9)
#define UD_STRED                         (1 << 10)
#define UD_SCRPAUSE                      (1 << 11)
#define UD_FSTGOODBYE                    (1 << 12)
#define UD_VFLAG                         (1 << 13)
#define UD_ZFLAG                         (1 << 14)
#define UD_ASCIIMSG                      (1 << 15)
#define UD_SWAPREDWHITE                  (1 << 16)
#define UD_AUTOQUOTE                     (1 << 17)
#define UD_KEEPREPLYPRIV                 (1 << 18)
#define UD_KEEPREPLYSUBJECT              (1 << 19)
#define UD_FILECHECKPROCESS              (1 << 20)
#define UD_AUTOFORCEONWHO                (1 << 21)
#define UD_BACKGROUNDPOST                (1 << 22)
#define UD_FILENAMELENGTH                (1 << 23)

/*
 * UserFlag1 defines (ConfUserData)
 */

#define UC_NEWMSG                        (1 << 0)
#define UC_NEWFILES                      (1 << 1)
#define UC_ZOOM                          (1 << 2)
#define UC_USERCNFXS                     (1 << 3)
#define UC_OBSOLETE                      (1 << 4)
#define UC_USCNFDATA                     (1 << 5)

/*
 * NToNComFlag1 defines (UserData)
 */

#define NTN_DEFAULT                      (1 << 0)
#define NTN_BBSHELP                      (1 << 1)
#define NTN_BULLETINS                    (1 << 2)
#define NTN_CHANGESTATS                  (1 << 3)
#define NTN_ENTEREALL                    (1 << 4)
#define NTN_FILELISTING                  (1 << 5)
#define NTN_FLAGEDIT                     (1 << 6)
#define NTN_JOINCONF                     (1 << 7)
#define NTN_LOGGINGON                    (1 << 8)
#define NTN_MULTICHAT                    (1 << 9)
#define NTN_NEWFILES                     (1 << 10)
#define NTN_PREPAREDOWNL                 (1 << 11)
#define NTN_PREPAREUPL                   (1 << 12)
#define NTN_READMAIL                     (1 << 13)
#define NTN_SCANFILES                    (1 << 14)
#define NTN_SCANMAIL                     (1 << 15)
#define NTN_SEARCHFOR                    (1 << 16)
#define NTN_SHELL                        (1 << 17)
#define NTN_SYSOPCHAT                    (1 << 18)
#define NTN_SYSOPCOMENT                  (1 << 19)
#define NTN_VIEWFILES                    (1 << 20)
#define NTN_VIEWTEXT                     (1 << 21)
#define NTN_WAITFORKEY                   (1 << 22)
#define NTN_WHO                          (1 << 23)
#define NTN_WRITEMAIL                    (1 << 24)

/*
 * FAMECfgFlag1 defines (FAME Config)
 */

#define FAMECfg_AllowHistory             (1 << 0)
#define FAMECfg_NSTOGGLE                 (1 << 1)
#define FAMECfg_CHATIS                   (1 << 2)
#define FAMECfg_ASLIS                    (1 << 3)
#define FAMECfg_RoOm                     (1 << 4)
#define FAMECfg_KEY                      (1 << 5)
#define FAMECfg_SER                      (1 << 6)
#define FAMECfg_SERDRV                   (1 << 7)
#define FAMECfg_USERIP                   (1 << 8)
//#define FAMECfg_ANSI                   (1 << 9) Obsolete/Unused
#define FAMECfg_ASCII                    (1 << 10)
#define FAMECfg_QUIET                    (1 << 11)
#define FAMECfg_SYSOPXS                  (1 << 12)
#define FAMECfg_DOWNASL                  (1 << 13)
#define FAMECfg_WASUDLOGBEFORE           (1 << 14)
#define FAMECfg_SysOpCanRead             (1 << 15)
#define FAMECfg_DisTimeout               (1 << 16)
#define FAMECfg_WF_StatCmd               (1 << 17)
#define FAMECfg_WF_IconifyCmd            (1 << 18)
#define FAMECfg_WF_Trans                 (1 << 19)
#define FAMECfg_InitModem                (1 << 20)
#define FAMECfg_WF_DoorTool              (1 << 21)
#define FAMECfg_WF_Chat                  (1 << 22)
#define FAMECfg_FreeDlFile               (1 << 23)
#define FAMECfg_OkToInt                  (1 << 24)
#define FAMECfg_IconifyIs                (1 << 25)
#define FAMECfg_NodeSpyis                (1 << 26)
#define FAMECfg_FIRSTONHERE              (1 << 27)
#define FAMECfg_DoorUp                   (1 << 28)
#define FAMECfg_RESUME                   (1 << 29)
#define FAMECfg_GlobalDL                 (1 << 30)
#define FAMECfg_CloseIt                  (1 << 31)

/*
 * FAMECfgFlag2 defines (FAME Config)
 */

#define FAMECfg_SerInSetFlg              (1 << 0)
#define FAMECfg_SerOutSetFlg             (1 << 1)
#define FAMECfg_TransferAborted          (1 << 2)
#define FAMECfg_TransferError            (1 << 3)
#define FAMECfg_TransferFailed           (1 << 4)
#define FAMECfg_OkToScan                 (1 << 5)
#define FAMECfg_DoNotAsk                 (1 << 6)
#define FAMECfg_TransferIs               (1 << 7)
#define FAMECfg_WaitForXpr               (1 << 8)
#define FAMECfg_OffHook                  (1 << 9)
#define FAMECfg_HydraIs                  (1 << 10)
#define FAMECfg_HydraOpensWnds           (1 << 11)
#define FAMECfg_HydraWndsAreOpen         (1 << 12)
#define FAMECfg_HydraIconify             (1 << 13)
#define FAMECfg_HydraInvalidPort         (1 << 14)
#define FAMECfg_SerIn                    (1 << 15)
#define FAMECfg_SerOut                   (1 << 16)
#define FAMECfg_CHKLSTCHR                (1 << 17)
#define FAMECfg_NEWOPENED                (1 << 18)
#define FAMECfg_Suspend                  (1 << 19)
#define FAMECfg_AbortIOPort              (1 << 20)
#define FAMECfg_SaveUFlgs                (1 << 21)
#define FAMECfg_HydraNoTglIcnfy          (1 << 22)
#define FAMECfg_BackGroundCheck          (1 << 23)
#define FAMECfg_FileCheckRun             (1 << 24)
#define FAMECfg_ViewMailHeader           (1 << 25)
#define FAMECfg_RememberFlag             (1 << 26)
#define FAMECfg_NoMoreInput              (1 << 27)
#define FAMECfg_RWSwaped                 (1 << 28)
#define FAMECfg_VerboseAction            (1 << 29)
#define FAMECfg_FreeUlFile               (1 << 30)
#define FAMECfg_UserEdLock               (1 << 31)

/*
 * FAMECfgFlag3 defines (FAME Config)
 */

#define FAMECfg_OwnScreen                (1 << 0)
#define FAMECfg_OverrideXprDlFO          (1 << 1)
#define FAMECfg_ResumeFile               (1 << 2)

/*
 * _SvMsgInBitFlg1 defines (MainPart internal)
 */

#define SMIBF_SERVER_SYSTEM              (1 << 0)
#define SMIBF_SERVER_MISC                (1 << 1)
#define SMIBF_SERVER_PROTOCOL            (1 << 2)
#define SMIBF_SERVER_FAMEMODULES         (1 << 3)
#define SMIBF_SERVER_ACTIVITIES          (1 << 4)
#define SMIBF_SERVER_ERROR               (1 << 5)
#define SMIBF_SERVER_INTERNDOORS         (1 << 6)
#define SMIBF_SERVER_CONF_DATA           (1 << 7)
#define SMIBF_SERVER_SCREENS             (1 << 8)
#define SMIBF_SERVER_NODE                (1 << 9)
#define SMIBF_SERVER_SERIAL              (1 << 10)
#define SMIBF_SERVER_MODEM               (1 << 11)
#define SMIBF_SERVER_SCREEN              (1 << 12)
#define SMIBF_SERVER_GetServerCnf        (1 << 13)
#define SMIBF_SERVER_SerialReady         (1 << 14)
#define SMIBF_SERVER_BatchReady          (1 << 15)
#define SMIBF_SERVER_FCheckResList       (1 << 16)
#define SMIBF_SERVER_FCheckFileReady     (1 << 17)
#define SMIBF_SERVER_FileCheckOk         (1 << 18)
#define SMIBF_SERVER_PostFileReady       (1 << 19)
#define SMIBF_SERVER_FDEBUG              (1 << 20)

/*
 * CoProzess Flag1 defines
 */

#define COPR_ACTIVE                      (1 << 0)

/*
 * Hydra Data Flag defines
 */

#define FAMEHDF_Upload                   (1 << 0)
#define FAMEHDF_Download                 (1 << 1)

/*
 * FAMEDoorMsg fdom_BitFlag defines
 */

#define FBF_OLMSAVAIL                    (1 << 0)

/*
 * FAMEDoorMsg fdom_InternalBit defines
 */

#define FIBF_NODOORDROP                  (1 << 0)

/*
 * FAMEInfoList fili_BitFlags defines
 */

#define FILI_USERSAVELOCK                (1 << 0)
#define FILI_QUIETNODE                   (1 << 1)
#define FILI_USERHIDE                    (1 << 2)
#define FILI_USERREALHIDE                (1 << 3)
#define FILI_FORCEWHO                    (1 << 4)

/*
 * Remember String structure frst_BitFlags defines
 */

#define FRST_MCISTRING                   (1 << 0)

/*
 * Additional flags for FAMESaveSystemConfig() (FAME Config).
 */

#define FAMECFGSB_FORCE                  (1 << 0)  /* Save back also if FAMECFGLST_MODIFY is not set. */

/*
 * New User Entry Flags
 */

#define FAMENUE_Location                 (1 << 0)
#define FAMENUE_CityState                (1 << 1)
#define FAMENUE_PhoneNumber              (1 << 2)
#define FAMENUE_NumberofLines            (1 << 3)
#define FAMENUE_Computer                 (1 << 4)
#define FAMENUE_Modem                    (1 << 5)
#define FAMENUE_Language                 (1 << 6)
#define FAMENUE_MsgClearScreen           (1 << 7)
#define FAMENUE_FilelistClearScreen      (1 << 8)
#define FAMENUE_RedWhiteSwap             (1 << 9)
#define FAMENUE_XferProtOptions          (1 << 10)

/*
 * Header defines of FAME Datafiles
 */

#define FAMEUNKN    0   /* Unknown FAME DataType */
#define FAMEBAUD    1   /* BaudTimes means in which time array a connect rate
                           is allowed. 2400 baud is allowed from x to y */
#define FAMEPROT    2   /* Transfer Protocols holds the settings for the
                           xpr.library and hydra */
#define FAMEMISC    3   /* Miscellaneous holds many different settings */
#define FAMESYST    4   /* System holds the settings for the whole BBS
                           These datas are global for all nodes. */
#define FAMESERV    5   /* Server holds the settings for the Server */
#define FAMECONF    6   /* Conference holds the settings for each conference */
#define FAMELEVL    7   /* Levels holds the settings allowed for a level
                           This gives a user access to different commands
                           and actions in the BBS */
#define FAMECOXS    8   /* Conference Access holds flags which shows the node
                           to which conferences the user has access */
#define FAMEPRES    9   /* Presets are different basic settings which can
                           easy set datas for a normaly newuser account. */
#define FAMEACTI    10  /* Activities holds different settings to note the
                           nodes what they should so, like which logtypes
                           should be written, because there are different
                           log types avail, like Callers.log and
                           "steady Callers.log". */
#define FAMEERRO    11  /* Errors holds info's which are needed by the nodes
                           to show what the node should do if an error
                           occurs. */
#define FAMEDOOR    12  /* Doors holds the settings for every single door.
                           Doortype, stack, priority and many more... */
#define FAMEINDO    13  /* Internal Doors holds the settings for internal
                           Doors of the BBS. */
#define FAMESCRS    14  /* Screens (Textfiles) holds different settings
                           for screentexts */
#define FAMENODE    15  /* Node holds the individual settings of every single
                           node */
#define FAMESERI    16  /* Serial (Node) holds the (serial).device settings
                           and that for every single node */
#define FAMEMODM    17  /* Modem (Node) holds the modem settings
                           and that for every single node */
#define FAMESCRE    18  /* Screen (Node) holds the screen/window settings
                           and that for every single node */
#define FAMEFCHK    19  /* FileCheckers holds the settings for every single
                           filechecker type like lha,zip,... */
#define FAMETXLA    20  /* Text language holds the settings for the
                           available textsuffix types, like .txt .ger */
#define FAMELALO    21  /* Language Locale holds the settings for the
                           different locale.library's */
#define FAMEDOCO    22  /* Door Config holds the settings for Doorconfig
                           modules. This are additional Doortools for Doors
                           which let the user configure settings for a Door */
#define FAMEFMMO    23  /* FAME Modules are settings like paths for
                           FAME Modules like Server,MainPart,SystemEd,UserEd */
#define FAMECOTY    24  /* Computer Types holds the available Computertypes
                           listed to the user */
#define FAMEMOTY    25  /* Modem Types holds the available Modemtypes
                           listed to the user */
#define FAMEMAHE    26  /* MailHeader is the Headerfile for MsgBases */
#define FAMEMAST    27  /* MailStatus is the 2nd HeaderFile for MsgBases
                           which is needed for faster MsgBase Operations,
                           because it contains the number of the last written
                           message */
#define FAMEUSDA    28  /* User.data is the main userdata file */
#define FAMEUSKE    29  /* User.keys is the 2nd userdata file for faster
                           access */
#define FAMEUSCO    30  /* UserCnf.data is the additional userdata file
                           which is located for/in every conference */


// ---------- OBSOLETE ---------- //
#define FAMECOPR    31  /* xyz.dat located in Settings/CoProzesses/
                           to tell the Server which CoProzesses are avail
                           and their requests. This Data is a defined structure
                           for detail look out for: struct CoProzess in
                           GlobalBBSStructs.c */
// ---------- OBSOLETE ---------- //


#define FAMEPAMS    32  /* Msgheader and Message writen while a LossCarrier
                           happens */
#define FAMESCPT    33  /* FAME Sysop/System/User Scripts
                           Are all 3 script types like: Sysop scripts, are scripts,
                           maybe like doors, which user can decide if they want
                           them or not. System scripts are scripts like doors
                           (SYSCMD types) and they will be executed at different
                           places in the BBS. User scripts are self written
                           scripts from and for users, placed in an own directory
                           for each user. */
#define FAMEUSCA    34  /* User script commands allow
                           Is a datafile of bitflags which describes which
                           script commands are allowed in a user script and
                           which not. Also it holds additional datas to handle
                           all needed things for secure and so on.
                           This is an expansion structure of the original
                           user.data */
#define FAMESMSK    35  /* System script Mask
                           Is a datafile of bitflags which toggles every
                           system script at all their different places in the
                           BBS */
#define FAMEUSFB    36  /* User FileBase
                           Is a future used expansion structure of the original
                           user.data and includes all user datas for the own
                           user filearea like unix. A dir where user can do
                           waht they want with theri own files.. maybe only 4MB
                           for each user etc. */
#define FAMEUEXP    37  /* Is a future used expansion structure of the original
                           user.data and includes nothing yet */
#define FAMEUSBU    38  /* This datafield contains all the free definable user
                           buttons at the Server. */
#define FAMELOCK    39  /* MsgBases lock file. */
#define FAMEHYDR    40  /* FAME Hydra global config data file */
#define FAMEHYCO    41  /* FAME Hydra Node depending config data file */
#define FAMESMDM    42  /* SModem global config data file */
#define FAMEDBUG    43  /* FAME Debug global data file to let FAME generate
                           different debug logs and more */
#define FAMEUSHA    44  /* User data handler structure. Defines tools to
                           be startet if a username changes, a user gets
                           deleted or a new user validated. */
#define FAMECOPE    45  /* Color and Dripen definitions for a Node. */
#define FAMEAWAY    46  /* Away Message definitions for all Nodes. */
#define FAMEUSPR    47  /* User Profile for user settings import/export */
#define FAMEUCEX    48  /* Is a future used expansion structure of the original
                           UserCnf.data (the additional userdata file which is 
                           located for/in every conference) and includes nothing 
                           yet */

#endif /* FAME_FAMEDEFINE_H */

