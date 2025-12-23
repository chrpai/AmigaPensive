                IFND    LIBRARIES_FAME_I
LIBRARIES_FAME_I        SET 1

**
**              $VER: FAME.i 6.0
**
**              Standard asm definitions for FAME.library
**
**              (C) Copyright 1996-1998 BLOODROCK/tRSi/F-Innovation
**                  All Rights Reserved
**

        IFND EXEC_LIBRARIES_I
                INCLUDE "exec/libraries.i"
        ENDC
        IFND DOS_DOS_I
                INCLUDE "dos/dos.i"
        ENDC
        IFND LIBRARIES_FAME_LIB_I
                INCLUDE "libraries/FAME_lib.i"
        ENDC


FAMENAME        Macro
                        dc.b    "FAME.library",0
                EndM

FAMEVERSION     EQU     6


** FAME Objecs allcated with FAMEAllocObject() **

     ** FAME Public Structs **

FOBJ_PS_FAMEDoorMsg           EQU  1
FOBJ_PS_MailHeader            EQU  2
FOBJ_PS_MailStat              EQU  3
FOBJ_PS_FAMEToolPortMsg       EQU  4
FOBJ_PS_FAMECoProcessMsg      EQU  5
FOBJ_PS_ExternEditor          EQU  6
FOBJ_PS_ToolNotifyMsg         EQU  7
FOBJ_PS_FAMECoProcRequest     EQU 13
FOBJ_PS_DestNodes             EQU 14
FOBJ_PS_InfoList              EQU 15
FOBJ_PS_OnlineMsg             EQU 16
FOBJ_PS_OLMString             EQU 17
FOBJ_PS_MsgList               EQU 93
FOBJ_PS_UserProfile           EQU 94
FOBJ_PS_FAMEToolPortSemaphore EQU 95
FOBJ_PS_NumFlagList           EQU 96
FOBJ_PS_FAMESemaphore         EQU 97

     ** FAME User Structs **

FOBJ_US_UserExpansion         EQU  8
FOBJ_US_UserConfExpansion     EQU  9
FOBJ_US_User                  EQU 10
FOBJ_US_UserKeys              EQU 11
FOBJ_US_UserConf              EQU 12

     ** FAME Intern SetStructs **

FOBJ_SS_System                EQU  0
FOBJ_SS_Server                EQU 31
FOBJ_SS_Conf                  EQU 32
FOBJ_SS_Levels                EQU 33
FOBJ_SS_ConfAccess            EQU 34
FOBJ_SS_Preset                EQU 35
FOBJ_SS_Activities            EQU 36
FOBJ_SS_Error                 EQU 37
FOBJ_SS_Doors                 EQU 38
FOBJ_SS_InternDoors           EQU 39
FOBJ_SS_Screens               EQU 40
FOBJ_SS_Node                  EQU 41
FOBJ_SS_Serial                EQU 42
FOBJ_SS_Modem                 EQU 43
FOBJ_SS_Screen                EQU 44
FOBJ_SS_Checker               EQU 45
FOBJ_SS_TextLang              EQU 46
FOBJ_SS_LangLocale            EQU 47
FOBJ_SS_DoorConfig            EQU 48
FOBJ_SS_UserHandler           EQU 49
FOBJ_SS_FameModules           EQU 50
FOBJ_SS_ComputerTypes         EQU 51
FOBJ_SS_ModemTypes            EQU 52
FOBJ_SS_Misc                  EQU 53
FOBJ_SS_Protocol              EQU 54
FOBJ_SS_BaudTimes             EQU 55
FOBJ_SS_UserButtons           EQU 56
FOBJ_SS_FameHydra             EQU 57
FOBJ_SS_FameHydraNode         EQU 58
FOBJ_SS_FameSmodem            EQU 59
FOBJ_SS_FameDebug             EQU 60
FOBJ_SS_ColPenDat             EQU 61
FOBJ_SS_AwayMsg               EQU 62

     ** obsolete Object defines (no longer used) **

FOBJ_FAMEDoorMsg              EQU FOBJ_PS_FAMEDoorMsg
FOBJ_FAMEMailHeader           EQU FOBJ_PS_MailHeader
FOBJ_FAMEMailStat             EQU FOBJ_PS_MailStat
FOBJ_FAMEToolPortMsg          EQU FOBJ_PS_FAMEToolPortMsg
FOBJ_FAMECoProcessPort        EQU FOBJ_PS_FAMECoProcessMsg
FOBJ_FAMEExternEditor         EQU FOBJ_PS_ExternEditor
FOBJ_FAMEToolNotifyMsg        EQU FOBJ_PS_ToolNotifyMsg
FOBJ_FAMEExpansion            EQU FOBJ_US_UserExpansion
FOBJ_FAMECnfExpansion         EQU FOBJ_US_UserConfExpansion
FOBJ_FAMEUser                 EQU FOBJ_US_User
FOBJ_FAMEUserKeys             EQU FOBJ_US_UserKeys
FOBJ_FAMEUserConf             EQU FOBJ_US_UserConf
FOBJ_FAMECoProcess            EQU FOBJ_PS_FAMECoProcRequest
FOBJ_FAMEDestNodes            EQU FOBJ_PS_DestNodes
FOBJ_FAMEInfoList             EQU FOBJ_PS_InfoList
FOBJ_FAMEOnlineMsg            EQU FOBJ_PS_OnlineMsg
FOBJ_FAMEOLMString            EQU FOBJ_PS_OLMString

** FAMEAllocObject() error codes **
*  Use DOS/IoErr() to get the error code. May also be ERROR_NO_FREE_STORE.

FAOB_UnknownObject      EQU     -1  ; Object type out of range


**
** ========================== FAMEConfig Defines =============================
**

FCDD_System                 EQU 0
FCDD_Server                 EQU 1
FCDD_Conf                   EQU 2
FCDD_Levels                 EQU 3
FCDD_ConfAccess             EQU 4
FCDD_Preset                 EQU 5
FCDD_Activities             EQU 6
FCDD_Error                  EQU 7
FCDD_Doors                  EQU 8
FCDD_InternDoors            EQU 9
FCDD_Screens                EQU 10
FCDD_Node                   EQU 11
FCDD_Serial                 EQU 12
FCDD_Modem                  EQU 13
FCDD_Screen                 EQU 14
FCDD_Checker                EQU 15
FCDD_TextLang               EQU 16
FCDD_LangLocale             EQU 17
FCDD_DoorConfig             EQU 18
FCDD_UserHandler            EQU 19
FCDD_FameModules            EQU 20
FCDD_ComputerTypes          EQU 21
FCDD_ModemTypes             EQU 22
FCDD_Misc                   EQU 23
FCDD_Protocol               EQU 24
FCDD_BaudTimes              EQU 25
FCDD_UserButtons            EQU 26
FCDD_FameHydra              EQU 27
FCDD_FameHydraNode          EQU 28
FCDD_FameSmodem             EQU 29
FCDD_FameDebug              EQU 30
** OBSOLETE FCDD_ColPenDat  EQU 31
FCDD_AwayMsg                EQU 32
FCDD_UserExpansion          EQU 33
FCDD_UserConfExpansion      EQU 34
FCDD_User                   EQU 35
FCDD_UserKeys               EQU 36
FCDD_UserConf               EQU 37

** Mode defines for fcr_ConfigMode of struct FAMEConfigRequest
** and the FAMEReleaseConfig( ) and FAMESaveConfig( ) public functions.
**
** The mode which indicates if the tool just want to read out
** of the received data structure or if the tool will modify it too.
**
** For FAMEReleaseConfig( ) and FAMESaveConfig( ) the argument
** Modify has to be used with these defines to indicate if the
** tool has modified the received data structure though it has defined
** FAMECFG_MODE_READ in fcr_ConfigMode of struct FAMEConfigRequest
** before. Needed for tools which don't know if they have to modify.

FAMECFG_MODE_READ           EQU 0
FAMECFG_MODE_MODIFY         EQU 1

** Variable type defines for fcr_SearchType of struct FAMEConfigRequest.
**
** They are needed to indicate which variable type we get within
** fcr_SearchValue of struct FAMEConfigRequest.

FAMECFGVARTYPE_BYTE         EQU 0
FAMECFGVARTYPE_WORD         EQU 1
FAMECFGVARTYPE_LONG         EQU 2
FAMECFGVARTYPE_STRING       EQU 3

** Search operator for fcr_SearchOperator of struct FAMEConfigRequest.
**
** The operator indicates how we shall search for a match to be more flexible
** on searching in every useful way.
**
** FAMECFGOP_LEFTSTR, FAMECFGOP_RIGHTSTR and FAMECFGOP_MIDSTR are only
** possible with type FAMECFGVARTYPE_STRING. On using a not possible or
** not existing operator always FAMECFGOP_EQUAL will be used as default.
** With type FAMECFGVARTYPE_STRING all other available operators can
** also be used to search for a match.

FAMECFGOP_EQUAL             EQU 1
FAMECFGOP_UNEQUAL           EQU 2
FAMECFGOP_SMALLER           EQU 3
FAMECFGOP_GREATER           EQU 4
FAMECFGOP_SMALLEREQUAL      EQU 5
FAMECFGOP_GREATEREQUAL      EQU 6
FAMECFGOP_LEFTSTR           EQU 7
FAMECFGOP_RIGHTSTR          EQU 8
FAMECFGOP_MIDSTR            EQU 9

** SpecialCase defines for FAMESaveAllConfig()

FAMECFGSC_NOW               EQU 1

** Result defines for all Result returning functions.
**
** Negative values are really bad errors and in most cases it's not possible
** to continue the work.
** Positive ones are also errors which could cause trouble if they get not noticed
** and the tool isn't reacting on them, but the tool will be able to solve the
** problem and continue work often like doing a fallback or a retry.

FAMECFGRES_CFGDATAISINVALID EQU -9
FAMECFGRES_CFGDATAISNULL    EQU -8
FAMECFGRES_HEADERCORRUPT    EQU -7
FAMECFGRES_HEADERBUFFERFAIL EQU -6
FAMECFGRES_LISTALLOCFAIL    EQU -5
FAMECFGRES_PATHALLOCFAIL    EQU -4
FAMECFGRES_CFGALLOCFAIL     EQU -3
FAMECFGRES_CFGSEMAPHOREFAIL EQU -2
FAMECFGRES_BASENOTAVAILABLE EQU -1
FAMECFGRES_SUCCESS          EQU 0
FAMECFGRES_REQNOTVALID      EQU 1
FAMECFGRES_WRONGTYPE        EQU 2
FAMECFGRES_SEARCHNOTFOUND   EQU 3
FAMECFGRES_PATHERROR        EQU 4
FAMECFGRES_CFGOPENERROR     EQU 5
FAMECFGRES_CFGDATACORRUPT   EQU 6
FAMECFGRES_SEARCHIMPOSSIBLE EQU 7
FAMECFGRES_DATAFILETOOLD    EQU 8
FAMECFGRES_DATAFILETONEW    EQU 9
FAMECFGRES_FORBID           EQU 10
FAMECFGRES_RELOADNOTALLOWED EQU 11
FAMECFGRES_CFGALREADYLOCKED EQU 12
FAMECFGRES_UNKNOWNCOMMAND   EQU 13
FAMECFGRES_CFGUSRTASKNOTSUP EQU 14
FAMECFGRES_CFGUSRNAMENOTSUP EQU 15

** Additional flags for FAMEReloadConfig().
**
** Forces the library to reload the Config data structure also if it's not
** wanted, because the modify flag is set. This should be used with care
** and only if it's really important and there is *NO* other way,
** because made changes (mostly form other tools or FAME) will be lost then.

FAMECFGRL_FORCE             EQU (1 << 0)  ; Reload also if FAMECFGLST_MODIFY is set.

** ========================== FAMEConfig Structures ==========================
**
** Request structure for FAMEObtainConfig().
**
** This structure will be used only one time by the user tool and can be reused
** for as many requests as needed. It contains all needed information if it's
** filled for calling FAMEObtainConfig( ) to handle the wanted Config data
** structure in return to the Config data user (requestor in this case).
**
** fcr_CfgUserTask, fcr_CfgUserName, fcr_ConfigType, fcr_ConfigMode and
** depending on the fcr_ConfigType fcr_ConfigNum and/or fcr_ConfigNum2 and/or
** fcr_ConfigStr are *MUST* structure elements and have to be filled out before
** calling FAMEObtainConfig( ) with this structure.
**
** fcr_CfgUserTask    - has to be FindTask( NULL );
** fcr_CfgUserName    - should as short as possible contain the name of the user (tool).
** fcr_CfgUserDesc    - is not a must, but it would be nice to have a short description too.
** fcr_ConfigType     - is a FCDD_<xyz> define like FCDD_System to indicate your wanted
**                      Config data structure.
** fcr_ConfigMode     - is FAMECFG_MODE_READ or FAMECFG_MODE_MODIFY to indicate if you
**                      only read from the Config data structure or if you modify it too.
** fcr_ConfigNum      - is needed for multiple possible files on disk for the in
**                      fcr_ConfigType defined Config data structure if it has to be
**                      indentified by a number like Level200.dat.
**                      fcr_ConfigNum could also be the Node number for a Node depending
**                      Config data structure.
** fcr_ConfigNum2     - is needed for multiple possible files on disk for the in
**                      fcr_ConfigType defined Config data structure if it has to be
**                      indentified by two numbers like a user entry from a specific
**                      BBS-Conference (number) like UserCnf.data.
** fcr_ConfigStr      - is needed for multiple possible files on disk for the in
**                      fcr_ConfigType defined Config data structure if it has to be
**                      indentified by string (text/word) like the Conf Access for a user
**                      defined alternative by the user's name.
** fcr_SearchOffset   - is the offset of the structure element of the in fcr_ConfigType
**                      defined Config data structure which shall be used to search with.
**                      If for example the Config data structure exists of two LONGs
**                      and you want to search in the second long of a multiple entry
**                      file like the user data files are, then the offset will be the size
**                      of the first long means 4. For the first long it will be 0.
** fcr_SearchValue    - is a pointer to the data field like a BYTE, WORD, LONG or string
**                      (char array) which will be used to search for.
** fcr_SearchType     - defines the variable type you are pointing to with fcr_SearchValue.
**                      and which points with fcr_SearchOffset in the Config data structure.
** fcr_SearchOperator - is the operator which defines if you search for equal, unequal,
**                      greater, smaller and so on matches. Possible defines are
**                      FAMECFGOP_EQUAL, FAMECFGOP_UNEQUAL, FAMECFGOP_SMALLER,
**                      FAMECFGOP_GREATER, FAMECFGOP_SMALLEREQUAL, FAMECFGOP_GREATEREQUAL,
**                      FAMECFGOP_LEFTSTR, FAMECFGOP_RIGHTSTR and FAMECFGOP_MIDSTR.
**
** Note: To find out which Config data type needs what elements to be identified
**       read the autodoc.
**
** Note: On reusing the FAMEConfigRequest you *HAVE* to reset all not needed structure
**       elements before reusing FAMEConfigRequest by setting these elements to 0!
**       Especially if you have used the request before to search an entry, because
**       fcr_SearchValue and fcr_SearchOperator are indicating a search request if they
**       are not 0.
**       To made this reinitializing more compfortable to you the function
**       FAMERequestReset( ) exists.

    STRUCTURE FAMEConfigRequest,0
    APTR      fcr_CfgUserTask     ; The task pointer of the Config user.
    APTR      fcr_CfgUserName     ; Config user name.
    APTR      fcr_CfgUserDesc     ; Config user description.
    ULONG     fcr_ConfigType      ; The defined Config data number.
    LONG      fcr_ConfigMode      ; The Config data structure mode.
    LONG      fcr_ConfigNum       ; Numeric expansion.
    LONG      fcr_ConfigNum2      ; 2nd Numeric expansion.
    APTR      fcr_ConfigStr       ; String expansion.
    ULONG     fcr_SearchOffset    ; Data structure search offset.
    APTR      fcr_SearchValue     ; Data structure search value.
    LONG      fcr_SearchType      ; Data structure search mem type.
    LONG      fcr_SearchOperator  ; Data structure search operator.
    STRUCT    fcr_UnUsed,80       ; Dummy for future expansions.
    APTR      fcr_Private         ; Private.
    LABEL     fcr_SIZEOF


** Possible return codes from FAMEAvailExe() **

FAE_RESIDENT            EQU     3
FAE_RESIDENTSYS         EQU     2
FAE_LOADFILE            EQU     1
FAE_NOMATCH             EQU     0
FAE_DATAFILE            EQU    -1
FAE_NOEBIT              EQU    -2
FAE_DIRECTORY           EQU    -3
FAE_ERROR               EQU    -4


** Flags for FAMECutANSI() **

        BITDEF  FCA,STYLECMDS,0 ; remove "ESC .. m" text style commands


** Flags for FAMENumToStr() and FAMENum64ToStr() **
** Note: flags marked with "(-)" are not available for FAMENum64ToStr(). **

        BITDEF  FNS,HEX,0           ; (-) Output hex string
        BITDEF  FNS,BIN,1           ; (-) Output binary string
        BITDEF  FNS,WORD,2          ; (-) Output size WORD
        BITDEF  FNS,BYTE,3          ; (-) Output size BYTE
        BITDEF  FNS,LEADINGZEROES,4 ; (+) Output leading zeroes
        BITDEF  FNS,LEADINGSIGN,5   ; (+) Add leading numeric system sign
        BITDEF  FNS,RIGHTFORMAT,6   ; (+) Right formatted output
        BITDEF  FNS,LEFTFORMAT,7    ; (+) Left formatted output
        BITDEF  FNS,CENTERFORMAT,8  ; (+) Center formatted output
        BITDEF  FNS,TENDLEFT,9      ; (+) Left tendence center formatting
        BITDEF  FNS,LOWERCASE,10    ; (-) Hex characters lowercase
        BITDEF  FNS,GROUPING,11     ; (+) Separate digit groups
        BITDEF  FNS,NUMLOCALE,12    ; (+) Use numeric Locale group separator
        BITDEF  FNS,MONLOCALE,13    ; (+) Use monetary Locale group separator
        BITDEF  FNS,SIGNED,14       ; (+) Handle value as signed number
        BITDEF  FNS,PLUSSIGN,15     ; (+) Use "+" char for signed pos. values
        BITDEF  FNS,SWAPSIGNS,16    ; (+) Swap "-/+/ " and "#/$/%" positions


** FAME DevInfoList structure **

        STRUCTURE       FAMEDevInfoList,0
        APTR    fdil_Next               ; next device
        STRUCT  fdil_Name,32            ; device name including ":"
        LONG    fdil_DiskType           ; InfoData->DiskType
        LONG    fdil_DiskState          ; InfoData->DiskState
        LONG    fdil_Size               ; total size in bytes
        LONG    fdil_BytesFree          ; free bytes
        LABEL   fdil_SIZEOF


** FAMEExecuteDir() list (the result you get) **

** This list only contains programs which have been successfully started.

        STRUCTURE       FAMEExecuteDirList,0
        APTR    fxdl_Next               ; next started program
        STRUCT  fxdl_FileName,256       ; name of the launched program
        ULONG   fxdl_ReturnCode         ; the DOS ReturnCode of the program.
        LABEL   fxdl_SIZEOF             ; If the SYS_Async Tag was specified,
                                        ; fxdl_ReturnCode is always NULL.

** FAMEFile structure returned by FAMELoadFile() **

        STRUCTURE       FAMEFile,0
        APTR    ffil_File               ; the file image itself
        BPTR    ffil_FH                 ; the file's FileHandle
                                        ; (only valid if FLFF_KEEPFH was set)
        STRUCT  ffil_FIB,fib_SIZEOF     ; the file's FileInfoBlock
        LABEL   ffil_SIZEOF


** FAMEPoolFile structure returned by FAMELoadFilePooled() **

        STRUCTURE       FAMEPoolFile,0
        APTR    fpof_Next               ; initialized to NULL, always ignored.
                                        ; free to use by your program !
        APTR    fpof_Prev               ; initialized to NULL, always ignored.
                                        ; free to use by your program !
        APTR    fpof_File               ; the file image itself
        BPTR    fpof_FH                 ; the file's FileHandle
                                        ; (only valid if FLFF_KEEPFH was set)
        STRUCT  fpof_FIB,fib_SIZEOF     ; the file's FileInfoBlock
        LABEL   fpof_SIZEOF


** FAMEDosMove() flags **

        BITDEF  FDM,NODELETE,0          ; copy instead moving
        BITDEF  FDM,KEEPDATA,1          ; keep comment & protection bits
                                        ; (except the archive bit)


** FAMELoadFile() and FAMELoadFilePooled() flags **

        BITDEF  FLF,KEEPFH,0            ; don't close the file


** FAMEPostFile() flags **

        BITDEF  FPF,NODELETE,0  ; don't delete source file anyway
        BITDEF  FPF,REPLACE,1   ; delete/replace existing file(s)
        BITDEF  FPF,CHECKONLY,2 ; Only test if the file already exists
        BITDEF  FPF,KEEPDATA,3  ; Keep source file comment and protection
                                ; bits (except A,R,D)

** FAMEUlPathList structure **

        STRUCTURE       FAMEUlPathList,0
        APTR    fupl_Next       ; Ptr. to next UploadPathList entry
        APTR    fupl_Prev       ; Ptr. to previous UploadPathList entry
        STRUCT  fupl_Path,102   ; full path name
        ULONG   fupl_Internal   ; FAME.library internal; do not use!
        LABEL   fupl_SIZEOF


** Library base **

;  Fields declared as private ARE private. Do not use 'em. They contain
;  some internal data structures which don't contain anything you may
;  use to hack the BBS. It isn't useful for you anyway. Some BBS
;  functions give you informations from these data fields, if your
;  program may have use of it. So do not ever read these fields.
;  Also, *never* write anything to the library base.

 STRUCTURE FAMELibrary,LIB_SIZE

   * LIBRARY PRIVATE *

   UBYTE   FAME_Flags           ; see also:
   UBYTE   FAME_pad             ; "sample.library.asm", documented in the
   BPTR    FAME_SegList         ; Amiga ROM Kernel Reference Manual: Libraries

   * PUBLIC FIELDS *            ; may be READ from your program

   APTR    FAME_DOSBase         ; dos.library base V37
   APTR    FAME_SysBase         ; exec base for faster access
   APTR    FAME_LocaleBase      ; locale.library V38 base address

   * LIBRARY PRIVATE *

   LONG    FAME_Private2
   STRUCT  FAME_Private3,50
   WORD    FAME_Private4
   STRUCT  FAME_Private5,40

   * PUBLIC FIELDS *

   APTR    FAME_TimerBase       ; timer.device library base
   APTR    FAME_IconBase        ; icon.library V37 base address
   APTR    FAME_IntuiBase       ; intuition.library V37 base address
   APTR    FAME_UtilityBase     ; utility.library V37 base address

   * LIBRARY PRIVATE *

   APTR    FAME_Private6
   APTR    FAME_Private7
   APTR    FAME_Private8

   LABEL   FAME_SIZEOF

 ENDC   ; LIBRARIES_FAME_I
