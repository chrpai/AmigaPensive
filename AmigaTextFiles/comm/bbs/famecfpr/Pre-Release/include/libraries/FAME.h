#ifndef LIBRARIES_FAME_H
#define LIBRARIES_FAME_H TRUE

/*
**
**  $VER: FAME.h 6.0
**
**  Standard C definitions for FAME.library
**
**  (C) Copyright 1996-1998 BLOODROCK/tRSi/F-Innovation
**      All Rights Reserved
**
*/

#ifndef EXEC_LIBRARIES_H
#include <exec/libraries.h>
#endif

#ifndef DOS_DOS_H
#include <dos/dos.h>
#endif


#define  FAMENAME     "FAME.library"
#define  FAMEVERSION  6


/* FAME Objects allocated with FAMEAllocObject() */

        /* FAME Public Structs */

#define FOBJ_PS_FAMEDoorMsg            1
#define FOBJ_PS_MailHeader             2
#define FOBJ_PS_MailStat               3
#define FOBJ_PS_FAMEToolPortMsg        4
#define FOBJ_PS_FAMECoProcessMsg       5
#define FOBJ_PS_ExternEditor           6
#define FOBJ_PS_ToolNotifyMsg          7
#define FOBJ_PS_FAMECoProcRequest     13
#define FOBJ_PS_DestNodes             14
#define FOBJ_PS_InfoList              15
#define FOBJ_PS_OnlineMsg             16
#define FOBJ_PS_OLMString             17
#define FOBJ_PS_MsgList               93
#define FOBJ_PS_UserProfile           94
#define FOBJ_PS_FAMEToolPortSemaphore 95
#define FOBJ_PS_NumFlagList           96
#define FOBJ_PS_FAMESemaphore         97

        /* FAME User Structs */

#define FOBJ_US_UserExpansion          8
#define FOBJ_US_UserConfExpansion      9
#define FOBJ_US_User                  10
#define FOBJ_US_UserKeys              11
#define FOBJ_US_UserConf              12

        /* FAME Intern SetStructs */

#define FOBJ_SS_System                 0
#define FOBJ_SS_Server                31
#define FOBJ_SS_Conf                  32
#define FOBJ_SS_Levels                33
#define FOBJ_SS_ConfAccess            34
#define FOBJ_SS_Preset                35
#define FOBJ_SS_Activities            36
#define FOBJ_SS_Error                 37
#define FOBJ_SS_Doors                 38
#define FOBJ_SS_InternDoors           39
#define FOBJ_SS_Screens               40
#define FOBJ_SS_Node                  41
#define FOBJ_SS_Serial                42
#define FOBJ_SS_Modem                 43
#define FOBJ_SS_Screen                44
#define FOBJ_SS_Checker               45
#define FOBJ_SS_TextLang              46
#define FOBJ_SS_LangLocale            47
#define FOBJ_SS_DoorConfig            48
#define FOBJ_SS_UserHandler           49
#define FOBJ_SS_FameModules           50
#define FOBJ_SS_ComputerTypes         51
#define FOBJ_SS_ModemTypes            52
#define FOBJ_SS_Misc                  53
#define FOBJ_SS_Protocol              54
#define FOBJ_SS_BaudTimes             55
#define FOBJ_SS_UserButtons           56
#define FOBJ_SS_FameHydra             57
#define FOBJ_SS_FameHydraNode         58
#define FOBJ_SS_FameSmodem            59
#define FOBJ_SS_FameDebug             60
#define FOBJ_SS_ColPenDat             61
#define FOBJ_SS_AwayMsg               62

        /* obsolete Object defines (no longer used) */

#define FOBJ_FAMEDoorMsg       FOBJ_PS_FAMEDoorMsg
#define FOBJ_FAMEMailHeader    FOBJ_PS_MailHeader
#define FOBJ_FAMEMailStat      FOBJ_PS_MailStat
#define FOBJ_FAMEToolPortMsg   FOBJ_PS_FAMEToolPortMsg
#define FOBJ_FAMECoProcessPort FOBJ_PS_FAMECoProcessMsg
#define FOBJ_FAMEExternEditor  FOBJ_PS_ExternEditor
#define FOBJ_FAMEToolNotifyMsg FOBJ_PS_ToolNotifyMsg
#define FOBJ_FAMEExpansion     FOBJ_US_UserExpansion
#define FOBJ_FAMECnfExpansion  FOBJ_US_UserConfExpansion
#define FOBJ_FAMEUser          FOBJ_US_User
#define FOBJ_FAMEUserKeys      FOBJ_US_UserKeys
#define FOBJ_FAMEUserConf      FOBJ_US_UserConf
#define FOBJ_FAMECoProcess     FOBJ_PS_FAMECoProcRequest
#define FOBJ_FAMEDestNodes     FOBJ_PS_DestNodes
#define FOBJ_FAMEInfoList      FOBJ_PS_InfoList
#define FOBJ_FAMEOnlineMsg     FOBJ_PS_OnlineMsg
#define FOBJ_FAMEOLMString     FOBJ_PS_OLMString

/* FAMEAllocObject() error codes */
/* (Use DOS/IoErr() to get the error code. May also be ERROR_NO_FREE_STORE) */

#define FAOB_UnknownObject  -1   /* Object type out of range */


/*
 * ========================== FAMEConfig Defines =============================
 */

#define FCDD_System                 0
#define FCDD_Server                 1
#define FCDD_Conf                   2
#define FCDD_Levels                 3
#define FCDD_ConfAccess             4
#define FCDD_Preset                 5
#define FCDD_Activities             6
#define FCDD_Error                  7
#define FCDD_Doors                  8
#define FCDD_InternDoors            9
#define FCDD_Screens                10
#define FCDD_Node                   11
#define FCDD_Serial                 12
#define FCDD_Modem                  13
#define FCDD_Screen                 14
#define FCDD_Checker                15
#define FCDD_TextLang               16
#define FCDD_LangLocale             17
#define FCDD_DoorConfig             18
#define FCDD_UserHandler            19
#define FCDD_FameModules            20
#define FCDD_ComputerTypes          21
#define FCDD_ModemTypes             22
#define FCDD_Misc                   23
#define FCDD_Protocol               24
#define FCDD_BaudTimes              25
#define FCDD_UserButtons            26
#define FCDD_FameHydra              27
#define FCDD_FameHydraNode          28
#define FCDD_FameSmodem             29
#define FCDD_FameDebug              30
// OBSOLETE #define FCDD_ColPenDat  31
#define FCDD_AwayMsg                32
#define FCDD_UserExpansion          33
#define FCDD_UserConfExpansion      34
#define FCDD_User                   35
#define FCDD_UserKeys               36
#define FCDD_UserConf               37

/*
 * Mode defines for fcr_ConfigMode of struct FAMEConfigRequest
 * and the FAMEReleaseConfig( ), FAMESaveConfig( ) and
 * extern FAMEModifyConfig( ) public functions.
 *
 * The mode which indicates if the tool just want to read out
 * of the received data structure or if the tool will modify it too.
 *
 * For FAMEReleaseConfig( ), FAMESaveConfig( ) and FAMEModifyConfig( )
 * the argument Modify has to be used with these defines to indicate 
 * if the tool has modified the received data structure though it has 
 * defined FAMECFG_MODE_READ in fcr_ConfigMode of struct FAMEConfigRequest
 * before. Needed for tools which don't know if they have to modify.
 */

#define FAMECFG_MODE_READ           0
#define FAMECFG_MODE_MODIFY         1

/*
 * Variable type defines for fcr_SearchType of struct FAMEConfigRequest.
 *
 * They are needed to indicate which variable type we get within
 * fcr_SearchValue of struct FAMEConfigRequest.
 */

#define FAMECFGVARTYPE_BYTE         0
#define FAMECFGVARTYPE_WORD         1
#define FAMECFGVARTYPE_LONG         2
#define FAMECFGVARTYPE_STRING       3

/*
 * Search operator for fcr_SearchOperator of struct FAMEConfigRequest.
 *
 * The operator indicates how we shall search for a match to be more flexible
 * on searching in every useful way.
 *
 * FAMECFGOP_LEFTSTR, FAMECFGOP_RIGHTSTR and FAMECFGOP_MIDSTR are only
 * possible with type FAMECFGVARTYPE_STRING. On using a not possible or
 * not existing operator always FAMECFGOP_EQUAL will be used as default.
 * With type FAMECFGVARTYPE_STRING all other available operators can
 * also be used to search for a match.
 */

#define FAMECFGOP_EQUAL             1
#define FAMECFGOP_UNEQUAL           2
#define FAMECFGOP_SMALLER           3
#define FAMECFGOP_GREATER           4
#define FAMECFGOP_SMALLEREQUAL      5
#define FAMECFGOP_GREATEREQUAL      6
#define FAMECFGOP_LEFTSTR           7
#define FAMECFGOP_RIGHTSTR          8
#define FAMECFGOP_MIDSTR            9

/*
 * SpecialCase defines for FAMESaveAllConfig()
 */

#define FAMECFGSC_NOW               1

/*
 * Result defines for all Result returning functions.
 *
 * Negative values are really bad errors and in most cases it's not possible
 * to continue the work.
 * Positive ones are also errors which could cause trouble if they get not noticed
 * and the tool isn't reacting on them, but the tool will be able to solve the
 * problem and continue work often like doing a fallback or a retry.
 */

#define FAMECFGRES_CFGDATAISINVALID -9
#define FAMECFGRES_CFGDATAISNULL    -8
#define FAMECFGRES_HEADERCORRUPT    -7
#define FAMECFGRES_HEADERBUFFERFAIL -6
#define FAMECFGRES_LISTALLOCFAIL    -5
#define FAMECFGRES_PATHALLOCFAIL    -4
#define FAMECFGRES_CFGALLOCFAIL     -3
#define FAMECFGRES_CFGSEMAPHOREFAIL -2
#define FAMECFGRES_BASENOTAVAILABLE -1
#define FAMECFGRES_SUCCESS          0
#define FAMECFGRES_REQNOTVALID      1
#define FAMECFGRES_WRONGTYPE        2
#define FAMECFGRES_SEARCHNOTFOUND   3
#define FAMECFGRES_PATHERROR        4
#define FAMECFGRES_CFGOPENERROR     5
#define FAMECFGRES_CFGDATACORRUPT   6
#define FAMECFGRES_SEARCHIMPOSSIBLE 7
#define FAMECFGRES_DATAFILETOOLD    8
#define FAMECFGRES_DATAFILETONEW    9
#define FAMECFGRES_FORBID           10
#define FAMECFGRES_RELOADNOTALLOWED 11
#define FAMECFGRES_CFGALREADYLOCKED 12
#define FAMECFGRES_UNKNOWNCOMMAND   13
#define FAMECFGRES_CFGUSRTASKNOTSUP 14
#define FAMECFGRES_CFGUSRNAMENOTSUP 15

/*
 * Additional flags for FAMEReloadConfig().
 *
 * Forces the library to reload the Config data structure also if it's not
 * wanted, because the modify flag is set. This should be used with care
 * and only if it's really important and there is *NO* other way,
 * because made changes (mostly form other tools or FAME) will be lost then.
 */

#define FAMECFGRL_FORCE             (1 << 0)  /* Reload also if FAMECFGLST_MODIFY is set. */

/*
 * ========================== FAMEConfig Structures ==========================
 */

/*
 * Request structure for FAMEObtainConfig().
 *
 * This structure will be used only one time by the user tool and can be reused
 * for as many requests as needed. It contains all needed information if it's
 * filled for calling FAMEObtainConfig( ) to handle the wanted Config data
 * structure in return to the Config data user (requestor in this case).
 *
 * fcr_CfgUserTask, fcr_CfgUserName, fcr_ConfigType, fcr_ConfigMode and
 * depending on the fcr_ConfigType fcr_ConfigNum and/or fcr_ConfigNum2 and/or
 * fcr_ConfigStr are *MUST* structure elements and have to be filled out before
 * calling FAMEObtainConfig( ) with this structure.
 *
 * fcr_CfgUserTask    - has to be FindTask( NULL );
 * fcr_CfgUserName    - should as short as possible contain the name of the user (tool).
 * fcr_CfgUserDesc    - is not a must, but it would be nice to have a short description too.
 * fcr_ConfigType     - is a FCDD_<xyz> define like FCDD_System to indicate your wanted
 *                      Config data structure.
 * fcr_ConfigMode     - is FAMECFG_MODE_READ or FAMECFG_MODE_MODIFY to indicate if you
 *                      only read from the Config data structure or if you modify it too.
 * fcr_ConfigNum      - is needed for multiple possible files on disk for the in
 *                      fcr_ConfigType defined Config data structure if it has to be
 *                      indentified by a number like Level200.dat.
 *                      fcr_ConfigNum could also be the Node number for a Node depending
 *                      Config data structure.
 * fcr_ConfigNum2     - is needed for multiple possible files on disk for the in
 *                      fcr_ConfigType defined Config data structure if it has to be
 *                      indentified by two numbers like a user entry from a specific
 *                      BBS-Conference (number) like UserCnf.data.
 * fcr_ConfigStr      - is needed for multiple possible files on disk for the in
 *                      fcr_ConfigType defined Config data structure if it has to be
 *                      indentified by string (text/word) like the Conf Access for a user
 *                      defined alternative by the user's name.
 * fcr_SearchOffset   - is the offset of the structure element of the in fcr_ConfigType
 *                      defined Config data structure which shall be used to search with.
 *                      If for example the Config data structure exists of two LONGs
 *                      and you want to search in the second long of a multiple entry
 *                      file like the user data files are, then the offset will be the size
 *                      of the first long means 4. For the first long it will be 0.
 * fcr_SearchValue    - is a pointer to the data field like a BYTE, WORD, LONG or string
 *                      (char array) which will be used to search for.
 * fcr_SearchType     - defines the variable type you are pointing to with fcr_SearchValue.
 *                      and which points with fcr_SearchOffset in the Config data structure.
 * fcr_SearchOperator - is the operator which defines if you search for equal, unequal,
 *                      greater, smaller and so on matches. Possible defines are
 *                      FAMECFGOP_EQUAL, FAMECFGOP_UNEQUAL, FAMECFGOP_SMALLER,
 *                      FAMECFGOP_GREATER, FAMECFGOP_SMALLEREQUAL, FAMECFGOP_GREATEREQUAL,
 *                      FAMECFGOP_LEFTSTR, FAMECFGOP_RIGHTSTR and FAMECFGOP_MIDSTR.
 *
 * Note: To find out which Config data type needs what elements to be identified
 *       read the autodoc.
 *
 * Note: On reusing the FAMEConfigRequest you *HAVE* to reset all not needed structure
 *       elements before reusing FAMEConfigRequest by setting these elements to 0!
 *       Especially if you have used the request before to search an entry, because
 *       fcr_SearchValue and fcr_SearchOperator are indicating a search request if they
 *       are not 0.
 *       To made this reinitializing more compfortable to you the function
 *       FAMERequestReset( ) exists.
 */

struct FAMEConfigRequest
{
  struct Task * fcr_CfgUserTask;    /* The task pointer of the Config user. */
  STRPTR        fcr_CfgUserName,    /* Config user name.                    */
                fcr_CfgUserDesc;    /* Config user description.             */
  ULONG         fcr_ConfigType;     /* The defined Config data number.      */
  long          fcr_ConfigMode,     /* The Config data structure mode.      */
                fcr_ConfigNum,      /* Numeric expansion.                   */
                fcr_ConfigNum2;     /* 2nd Numeric expansion.               */
  STRPTR        fcr_ConfigStr;      /* String expansion.                    */
  ULONG         fcr_SearchOffset;   /* Data structure search offset.        */
  APTR          fcr_SearchValue;    /* Data structure search value.         */
  long          fcr_SearchType,     /* Data structure search mem type.      */
                fcr_SearchOperator; /* Data structure search operator.      */
  ULONG         fcr_UnUsed[20];     /* Dummy for future expansions.         */
  APTR          fcr_Private;        /* Private.                             */
};


/* Possible return codes from FAMEAvailExe() */

#define FAE_RESIDENT      3
#define FAE_RESIDENTSYS   2
#define FAE_LOADFILE      1
#define FAE_NOMATCH       0
#define FAE_DATAFILE     -1
#define FAE_NOEBIT       -2
#define FAE_DIRECTORY    -3
#define FAE_ERROR        -4


/* Flags for FAMECutANSI() */

#define FCAF_STYLECMDS     (1L<<0)  /* remove "ESC .. m" text style commands */

#define FCAB_STYLECMDS      0       /* remove "ESC .. m" text style commands */


/* Flags for FAMENumToStr() and FAMENum64ToStr() */
/* Note: flags marked with "(-)" are not available for FAMENum64ToStr(). */

#define FNSF_HEX           (1L<<0)  /* (-) Output hex string */
#define FNSF_BIN           (1L<<1)  /* (-) Output binary string */
#define FNSF_WORD          (1L<<2)  /* (-) Output size WORD */
#define FNSF_BYTE          (1L<<3)  /* (-) Output size BYTE */
#define FNSF_LEADINGZEROES (1L<<4)  /* (+) Output leading zeros */
#define FNSF_LEADINGSIGN   (1L<<5)  /* (+) Add leading numeric system sign */
#define FNSF_RIGHTFORMAT   (1L<<6)  /* (+) Right formatted output */
#define FNSF_LEFTFORMAT    (1L<<7)  /* (+) Left formatted output */
#define FNSF_CENTERFORMAT  (1L<<8)  /* (+) Center formatted output */
#define FNSF_TENDLEFT      (1L<<9)  /* (+) Left tendence center formatting */
#define FNSF_LOWERCASE     (1L<<10) /* (-) Hex characters lowercase */
#define FNSF_GROUPING      (1L<<11) /* (+) Separate digit groups */
#define FNSF_NUMLOCALE     (1L<<12) /* (+) Use numeric Locale group separator */
#define FNSF_MONLOCALE     (1L<<13) /* (+) Use monetary Locale group separator */
#define FNSF_SIGNED        (1L<<14) /* (+) Handle value as signed number */
#define FNSF_PLUSSIGN      (1L<<15) /* (+) Use "+" char for signed pos. values */
#define FNSF_SWAPSIGNS     (1L<<16) /* (+) Swap "-/+/ " and "#/$/%" positions */

#define FNSB_HEX            0 /* (-) Output hex string */
#define FNSB_BIN            1 /* (-) Output binary string */
#define FNSB_WORD           2 /* (-) Output size WORD */
#define FNSB_BYTE           3 /* (-) Output size BYTE */
#define FNSB_LEADINGZEROES  4 /* (+) Output leading zeros */
#define FNSB_LEADINGSIGN    5 /* (+) Add leading numeric system sign */
#define FNSB_RIGHTFORMAT    6 /* (+) Right formatted output */
#define FNSB_LEFTFORMAT     7 /* (+) Left formatted output */
#define FNSB_CENTERFORMAT   8 /* (+) Center formatted output */
#define FNSB_TENDLEFT       9 /* (+) Left tendence center formatting */
#define FNSB_LOWERCASE     10 /* (-) Hex characters lowercase */
#define FNSB_GROUPING      11 /* (+) Separate digit groups */
#define FNSB_NUMLOCALE     12 /* (+) Use numeric Locale group separator */
#define FNSB_MONLOCALE     13 /* (+) Use monetary Locale group separator */
#define FNSB_SIGNED        14 /* (+) Handle value as signed number */
#define FNSB_PLUSSIGN      15 /* (+) Use "+" char for signed pos. values */
#define FNSB_SWAPSIGNS     16 /* (+) Swap "-/+/ " and "#/$/%" positions */


/* FAME DevInfoList structure */

struct FAMEDevInfoList {
   APTR  fdil_Next;         /* next device */
   char  fdil_Name[32];     /* device name including ":" */
   LONG  fdil_DiskType;     /* InfoData->DiskType */
   LONG  fdil_DiskState;    /* InfoData->DiskState */
   LONG  fdil_Size;         /* total size in bytes */
   LONG  fdil_BytesFree;    /* free bytes */
};


/* FAMEExecuteDir() list (the result you get)

This list only contains programs which have been successfully started. */

struct FAMEExecuteDirList {
   APTR  fxdl_Next;           /* next started program */
   char  fxdl_FileName[256];  /* name of the launched program */
   ULONG fxdl_ReturnCode;     /* the DOS ReturnCode of the program. */
};                            /* If the SYS_Async Tag was specified, */
                              /* fxdl_ReturnCode is always NULL. */

/* FAMEFile structure returned by FAMELoadFile() */

struct FAMEFile {
   APTR  ffil_File;               /* the file image itself */
   BPTR  ffil_FH;                 /* the file's FileHandle
                                     (only valid if FLFF_KEEPFH was set) */
   struct FileInfoBlock ffil_FIB; /* the file's FileInfoBlock */
};


/* FAMEPoolFile structure returned by FAMELoadFilePooled() */

struct FAMEPoolFile {
   APTR  fpof_Next;               /* initialized to NULL, always ignored.
                                     free to use by your program !       */
   APTR  fpof_Prev;               /* initialized to NULL, always ignored.
                                     free to use by your program !       */
   APTR  fpof_File;               /* the file image itself */
   BPTR  fpof_FH;                 /* the file's FileHandle
                                     (only valid if FLFF_KEEPFH was set) */
   struct FileInfoBlock fpof_FIB; /* the file's FileInfoBlock */
};


/* FAMEDosMove() flags */

#define FDMF_NODELETE    (1l<<0)  /* copy instead moving */
#define FDMF_KEEPDATA    (1l<<1)  /* keep comment & protection bits */
                                  /* (except the archive bit) */

#define FDMB_NODELETE         0   /* copy instead moving */
#define FDMB_KEEPDATA         1   /* keep comment & protection bits */
                                  /* (except the archive bit) */

/* FAMELoadFile() and FAMELoadFilePooled() flags */

#define FLFF_KEEPFH      (1L<<0)  /* don't close the file */

#define FLFB_KEEPFH           0   /* don't close the file */


/* FAMEPostFile() flags */

#define FPFF_NODELETE   (1L<<0)  /* don't delete source file anyway */
#define FPFF_REPLACE    (1L<<1)  /* delete/replace existing file(s) */
#define FPFF_CHECKONLY  (1L<<2)  /* Only test if the file already exists */
#define FPFF_KEEPDATA   (1L<<3)  /* Keep source file comment and protection */
                                  /* bits (except A,R,D) */

#define FPFB_NODELETE         0   /* don't delete source file anyway */
#define FPFB_REPLACE          1   /* always Post file */
#define FPFB_CHECKONLY        2   /* Only test if the file already exists */
#define FPFB_KEEPDATA         3   /* Keep source file comment and protection */
                                  /* bits (except A,R,D) */

/* FAMEUlPathList structure */

   struct FAMEUlPathList {
  APTR  fupl_Next;      /* Ptr. to next UploadPathList entry */
  APTR  fupl_Prev;      /* Ptr. to previous UploadPathList entry */
  char  fupl_Path[102]; /* full path name */
  ULONG fupl_Internal;  /* FAME.library internal; do not use! */
};


/* Library base */

/* Fields declared as private ARE private. Do not use 'em. They contain
   some internal data structures which don't contain anything you may
   use to hack the BBS. It isn't useful for you anyway. Some BBS
   functions give you informations from these data fields, if your
   program may have use of it. So do not ever read these fields.
   Also, *never* write anything to the library base. */

struct FAMELibrary {
   struct Library LibNode; /* Standard library node */

   /* LIBRARY PRIVATE */

   UBYTE FAME_Flags;         /* see also: */
   UBYTE FAME_pad;           /* "sample.library.asm", documented in the */
   BPTR  FAME_SegList;       /* Amiga ROM Kernel Reference Manual: Libraries */

   /* PUBLIC FIELDS */       /* may be READ from your program */

   struct DosLibrary * FAME_DOSBase;  /* dos.library base V37 */
   APTR  FAME_SysBase;                /* exec base for faster access */
   APTR  FAME_LocaleBase;             /* locale.library V38 base address */

   /* LIBRARY PRIVATE */

   LONG  FAME_Private2;
   BYTE  FAME_Private3[50];
   WORD  FAME_Private4;
   BYTE  FAME_Private5[40];

   /* PUBLIC FIELDS */

   APTR  FAME_TimerBase;      /* timer.device library base */
   APTR  FAME_IconBase;       /* icon.library V37 base address */
   APTR  FAME_IntuiBase;      /* intuition.library V37 base address */
   APTR  FAME_UtilityBase;    /* utility.library V37 base address */

   /* LIBRARY PRIVATE */

   APTR  FAME_Private6;
   APTR  FAME_Private7;
   APTR  FAME_Private8;
};

#endif  /* LIBRARIES_FAME_H */
