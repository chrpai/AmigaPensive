
/*
 * FAME Intern Setting Structs (SUPPORTER ONLY)
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * $VER: FAMEInternSetStructs.h v1.5
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

#ifndef FAME_FAMEINTERNSETSTRUCTS_H
#define FAME_FAMEINTERNSETSTRUCTS_H

//#include <exec/types.h>
//#include <stdio.h>

/*
 * -----------------------------------------------------------------------------
 */

/* 
 * FAME:Settings/System.dat
 */

struct FAMESystem
{
  char      BBSName[42],               /* The BBS Name                          */
            BBSGroup[32],              /* The BBS group or location             */
            SysOpName[32],             /* The sysop name                        */
            BBSLocationPath[102],      /* The location of FAME: normaly FAME:   */
            FileDescULine[46],         /* FileDicriptionLine if                 */
                                       /* FileNumbers = TRUE                    */
            UserDataName[12],          /* The name of the User.data file        */
            UserKeysName[12],          /* The name of the User.keys file        */
            UserDaKePath[102],         /* Path for User.data and User.keys      */
            FileCryptPW[12],           /* File-Crypt-Password                   */
            EmergencyPW[12],           /* Emergency password                    */
            ASLLocalUlPath[22],        /* ASL Local Upload Path                 */
            ASLTextViewPath[22],       /* ASL Text View Path                    */
            ASLSendFilePath[22],       /* ASL Send File Path                    */
            ASLLocalDlPath[22],        /* ASL Local Download Path               */
            StrNotUsed4[98];
  ULONG     UserLimit;                 /* Max. users allowed                    */
  long      NumberOfConfs,             /* Number of conferences                 */
            FGPSBitPlanes,
            FGPSFontSize;
  ULONG     FGPSScreenID,
            ULongNotUsed4,
            ULongNotUsed5,
            ULongNotUsed6;
  short int FGPSAutoScroll,
            ShortIntNotUsed1;
  long      NumberNodes,               /* Number of nodes                       */
            NewUserLevel,              /* NewUser level                         */
            FAMEVers,
            BullTxtMinNumLines,        /* Bull.txt min. number of lines         */
            BullTxtMinBrkLvl,          /* Bull.txt min. number of lines         */
            DefaultNumLines,           /* Default number of lines               */
            MaxUserPWFail,             /* Max. number of false paswords         */
            MaxSysPWFail,              /* Max. number of false paswords         */
            MaxNupPWFail,              /* Max. number of false paswords         */
            MaxNameFail,               /* Max. number of false names            */
            MaxUserHacks,              /* Max. number of hackings               */
            CaptureBuffer,             /* Capturebuffer                         */
            UPWFailAction,             /* nothing/EnterMsgtoSysOp/              */
                                       /* EnterMsgto(Co)SysOps->UEALL Level     */
            UPWFailALevel,             /* The level of UEALL                    */
            SpyWindowTop,              /* Top of Spywindow                      */
            SpyWindowLines,            /* Number of lines for Spywindow         */
            MaxCopyBuffer,             /* Maximum copy buffer for filecopies    */
            MinFreeMB;                 /* Minimum free MB for upload volumes    */
  short int UserFileBase,              /* UserFileBereiche allow                */
            FileNumbers,               /* Filenumbers in file listings          */
            DateBytes,                 /* Date and bytes in file listings       */
            RightFileNames,            /* right formated file namenes in file l.*/
            MsgCryptPW,                /* MsgKryptPW in MsgHeader Y/N           */
            FileCryptPWFC,             /* FileKryptPW to file/comment Y/N       */
            TextWHO,                   /* MCI based WHO Y/N                     */
            MultiNodeUser,             /* The same user on different nodes Y/N  */
            DoFileInfos,               /* FileName/Filesize/Uploader/Downloads/ */
                                       /* ULDate/ULTime/Xfer-Time/CPS/Baud/     */
                                       /* ULNode/Downloads/FileIdDiz Y,N/       */
                                       /* File_id_DIZ                           */
            ShowBull,                  /* Show or show not Bull.txt             */
            SpyKeyIn,                  /* Allow key input from Spywindow        */
            SpyWindowLeft,
            SpyWindowSizeGad,
            SpyWindowWidth,
            AutoRestrict,              /* Auto restriction of FAME data files   */
            MaxFidLines,
            UploadHandle,
            InactiveWins,
            ConfAccounting,
            FGPSDefPublicScreen;
  char      FGPSScreenName[32],
            FGPSFontName[32],
            FGPSOtherPubScreen[102],
            FGPSFallBackPubScreen[102],
            StrNotUsed1[2004];
};

/*
 * -----------------------------------------------------------------------------
 */

/*
 * FAME:Settings/Server.dat
 */

struct FAMEServer
{
  char      StrNotUsed1[32],
            StrNotUsed2[32],
            StrNotUsed3[32],
            StrNotUsed4[86];
  short int NOLastUsers,
            NOLastLocations,
            NOLastActions,
            NOLastBaudCPS,
            NOLastUploads,
            NOLastDownloads,
            NOFlaggedFiles,
            FreeExit;
  ULONG     ULongNotUsed1,
            ULongNotUsed2,
            ULongNotUsed3,
            ULongNotUsed4,
            ULongNotUsed5,
            ULongNotUsed6,
            ULongNotUsed7,
            ULongNotUsed8;
  long      Priority,                  /* Server priority                       */
            IconifyLeftEdge,           /* Left edge if iconified                */
            IconifyTopEdge,            /* Top edge if iconified                 */
            FAMEVers,
            LeftEdge,                  /* Left edge if uniconified              */
            TopEdge,                   /* Top edge if uniconified               */
            LongNotUsed1;
  short int Iconified,                 /* Start server iconified                */
            ErrorWarnUser,             /* Warn on user error handling           */
            SmallBig,                  /* Gadget bank on/off                    */
            Idle,
            Synchron,
            ShortIntNotUsed4,
            MinLeftEdge,
            MinTopEdge,
            FreeStart,
            VerboseAction,
            VisibleNodes,
            ShortIntNotUsed10;
};

/*
 * -----------------------------------------------------------------------------
 */

/*
 * FAME:Settings/Conferences/Conf(x).dat
 */

struct FAMEConf
{
  char      ConfName[32],              /* Conference name                       */
            ConfLocation[102],         /* Conference path                       */
            UploadPath[102],           /* Upload path 1                         */
            DownloadPath[102],         /* Download path 1                       */
            AddUlPaths[102],           /* Upload path 2                         */
            AddDlPaths[102],           /* Download path 2                       */
            ConfPassword[22],          /* Conference password                   */
            StrNotUsed1[80],
            StrNotUsed2[102],
            StrNotUsed3[102],
            StrNotUsed4[32];
  long      MBUsMinALL,
            MBUsMaxALL,
            MBUsDefALL,
            MBUsMinEALL,
            MBUsMaxEALL,
            MBUsDefEALL,
            MBUsMinUEALL,
            MBUsMaxUEALL,
            MBUsDefUEALL,
            MBUsMinPub,
            MBUsMaxPub,
            MBUsDefPub,
            MBUsMinPriv,
            MBUsMaxPriv,
            MBUsDefPriv;
  short int MBUsOvLvlALL,
            MBUsOvLvlEALL,
            MBUsOvLvlUEALL,
            MBUsOvLvlPub,
            MBUsOvLvlPriv;
  long      ConfNumber;                /* Conference number (x)                 */
  ULONG     ULongNotUsed1,
            ULongNotUsed2,
            ULongNotUsed3,
            ULongNotUsed4,
            ULongNotUsed5,
            ULongNotUsed6,
            ULongNotUsed7;
  long      NumberDirs,                /* Number of dirs                        */
            FreeUlLvl,                 /* Free upload level                     */
            FreeDlLvl,                 /* Free download level                   */
            ReadMsgLvl,                /* Msg write level                       */
            WriteMsgLvl,               /* Msg read level                        */
            UlLvl,                     /* Upload level                          */
            DlLvl,                     /* Download level                        */
            FilelstViewLvl,            /* Fileliste view level                  */
            GiveUlBytes,               /* Give upload bytes *                   */
            GiveDlBytes,               /* Give download bytes *                 */
            LongNotUsed5,
            BullTxtMinNumLines,        /* Bull.txt min. number of lines         */
            BullTxtMinBrkLvl,          /* Bull.txt min. abort level             */
            LongNotUsed6,
            MenuTxtMinNumLines,        /* Menu.txt min. number of lines         */
            MenuTxtMinBrkLvl,          /* Menu.txt min. abort level             */
            LongNotUsed7,
            FileHelpMinNumLines,       /* FileHelp min. number of lines         */
            FileHelpMinBrkLvl,         /* FileHelp min. abort level             */
            LongNotUsed8,
            UpMsgMinNumLines,          /* UpMsg min. number of lines            */
            UpMsgMinBrkLvl,            /* UpMsg min. abort level                */
            FAMEVers,
            DownMsgMinNumLines,        /* DownMsg min. number of lines          */
            DownMsgMinBrkLvl,          /* DownMsg min. abort level              */
            DelInDayMaxEALL,           /* Max. user delete EALL in days         */
            DelInDayDefEALL,           /* Default delete EALL in days           */
            DelInDayMaxPRIV,           /* Max. user delete PRIVATE in days      */
            DelInDayDefPRIV,           /* Default delete PRIVATE in days        */
            DelInDayMaxPUB,            /* Max. user delete PUBLIC in days       */
            DelInDayDefPUB,            /* Default delete PUBLIC in days         */
            WritePrivMsgLvl,           /* Private Msg write level               */
            WritePubMsgLvl,            /* Public Msg write level                */
            WriteUEALLMsgLvl,          /* UEALL Msg write level                 */
            WritePriorMsgLvl,          /* Priority Msg write level              */
            LongNotUsed1,
            LongNotUsed2,
            LongNotUsed3,
            LongNotUsed4;
  USHORT    UShortNotUsed5,
            FreeUlFlag,                /* Flag 0=OFF, 1=OR HIGHER, 2=OR LOWER   */
            FreeDlFlag,                /* Flag 0=OFF, 1=OR HIGHER, 2=OR LOWER   */
            ReadMsgLvlFlag,            /* Flag 0=OFF, 1=OR HIGHER, 2=OR LOWER   */
            WriteMsgLvlFlag,           /* Flag 0=OFF, 1=OR HIGHER, 2=OR LOWER   */
            UlLvlFlag,                 /* Flag 0=OFF, 1=OR HIGHER, 2=OR LOWER   */
            DlLvlFlag,                 /* Flag 0=OFF, 1=OR HIGHER, 2=OR LOWER   */
            FilelstViewLvlFlag,        /* Flag 0=OFF, 1=OR HIGHER, 2=OR LOWER   */
            WritePrivMsgLvlFlag,       /* Flag 0=OFF, 1=OR HIGHER, 2=OR LOWER   */
            WritePubMsgLvlFlag,        /* Flag 0=OFF, 1=OR HIGHER, 2=OR LOWER   */
            WriteUEALLMsgLvlFlag,      /* Flag 0=OFF, 1=OR HIGHER, 2=OR LOWER   */
            WritePriorMsgLvlFlag,      /* Flag 0=OFF, 1=OR HIGHER, 2=OR LOWER   */
            UShortNotUsed1,
            UShortNotUsed2,
            UShortNotUsed3,
            UShortNotUsed4;
  short int SentByFiles,               /* Do Sent By on files                   */
            UserSentByFiles,           /* Do user Sent By on files              */
            ShowMenu,
            ShowFileHelp,
            ShowUpMsg,
            ShowDownMsg,
            FirstPathsBehindA,
            AmigaDOSFileLen,
            CnfPWMFScanSkip,
            ShortIntNotUsed8,
            ShortIntNotUsed9,
            ShortIntNotUsed10;
};

/*
 * -----------------------------------------------------------------------------
 */

/*
 * FAME:Settings/Access/Levels/Level(x).dat
 */

struct FAMELevels
{
  char      Name[32],
            StrNotUsed1[32],
            StrNotUsed2[32];
  ULONG     ULongNotUsed1,
            ULongNotUsed2,
            ULongNotUsed3,
            ULongNotUsed4;
  long      MaxPages,                  /* Max. SysOp pages per day              */
            FAMEVers,
            MaxRelogins,               /* Max. relogins per day                 */
            LongNotUsed2,
            LongNotUsed3,
            LongNotUsed4,
            LongNotUsed5,
            LongNotUsed6,
            LongNotUsed7,
            LongNotUsed8,
            LongNotUsed9;
  USHORT    Level,                     /* Level (x)                             */
            UShortNotUsed1,
            UShortNotUsed2;
  short int ReadMessages,              /*XRead messages                         */
            EnterMessage,              /*XWrite messages                        */
            CommentSysOp,              /*XComment to SysOp                      */
            PageSysOp,                 /*XPage SysOp                            */
            ZoomMail,                  /* Search new mails                      */
            ReadBulletins,             /*XRead Bulletins                        */
            WHO,                       /*XWHO                                   */
            UserStatus,                /*XUser status                           */
            ZippyTextSearch,           /*XText search                           */
            JoinConference,            /*XJoin conference                       */
            FileListing,               /*XFile lists                            */
            NewFiles,                  /*XNew files                             */
            Upload,                    /*XDownload                              */
            Download,                  /*XUpload                                */
            ViewFiles,                 /*XView files                            */
            FreeResuming,              /* Free resumeing                        */
            EditUserAccount,           /*XEdit user datas                       */
            EditPassword,              /*XEdit password                         */
            EditUserLocation,          /*XEdit location                         */
            EditUserCityState,         /*XEdit city/state                       */
            EditUserName,              /*XEdit username                         */
            EditPhonenumber,           /*XEdit telephone number                 */
            EditBirthday,              /*XEdit birthday                         */
            Relogin,                   /*XRelogin                               */
            UploadStatus,              /*XUpload status                         */
            PublicMsgFiles,            /* Public files with messages            */
            PrivateMsgFiles,           /* Privat files with messages            */
            AttachFile,                /* Files with messages                   */
            MCIMessages,               /* MCI comandos in messages              */
            BreakChat,                 /*XBreak chat with CTRL-C                */
            OverideChat,               /* Overide chat                          */
            OverideTime,               /* Overide timeout                       */
            EAllMessages,              /*XWrite EALL messages                   */
            EditFiles,                 /*XEdit files                            */
            EditDirs,                  /*XEdit file listings                    */
            DeleteMessages,            /* Delete foreign messages               */
            AccountEditing,            /*XUser editor                           */
            SysOpCommands,             /*XSysOp commands                        */
            EveryShell,                /*XShell                                 */
            SysOpRead,                 /*XRead all messages                     */
            SysOpView,                 /* Read all files from HD                */
            SysOpDownload,             /* Download every file from HD           */
            OverideDefaults,           /* Overide all defaults                  */
            GlobalUDVZ,                /*xGlobal U/D/V/Z allow                  */
            Hydra,                     /*XHydra protocol                        */
            Rip,                       /* Rip gfx                               */
            FastRelogin,               /*XFast relogins                         */
            ViewBiaery,                /* Binäry file view                      */
            FileListZoom,              /* File list packing + download          */
            NodeChat,                  /* Chat command                          */
            CryptMsg,                  /* Crypt messages yes/no                 */
            FastGoodbye,               /* Fast goodbye                          */
            DelInDayMsg,               /* Delete messages in days               */
            OverwriteFiles,            /* Overwrite files                       */
            EditMessages,              /* Edit foreign mail                     */
            KeepOtherMail,             /* Keep foreign mail                     */
            OthUsOnOtherMsg,           /* Divert foreign mail                   */
            SModem,                    /* SModem protocol                       */
            EditFileFlags,             /*XEdit file flags                       */
            BBSHelp,                   /*XBBS Help                              */
            QuietMode,                 /*XQuiet Mode on/off                     */
            ShortIntNotUsed11,
            ShortIntNotUsed12,
            OLM,                       /*xSend Line to On-Line User             */
            OLC,                       /*xStart Capture at On-Line User         */
            OLD,                       /*xDisconnect On-Line User               */
            OLG,                       /*xGet On-Line User into Chat            */
            OLS,                       /*xSpy on a On-Line User from Remote     */
            OLT,                       /*xChange an On-Line Users Time limit    */
                                       /* (+10/-10)                             */
            SysViewBinaery,            /* Binäry SysOp file view                */
            ViewEditCLog,              /*XView/edit CallersLog                  */
            ViewEditUD,                /*XView/edit UDLog                       */
            ViewEditD,                 /*XView/edit DoorsLog                    */
            DOSList,                   /*XAmigaDOS List                         */
            DOSDir,                    /*XAmigaDOS Dir                          */
            DOSInfo,                   /*XAmigaDOS Info                         */
            ConfXSEd,                  /*XConference access editor              */
            ForceWHO,                  /*XWHO on hidden Users                   */
            RealHidden,                /*XWHO types "Await Call..."             */
            RenewSubMis,               /*XRenewed submission yes/no             */
            PriorMsg,                  /* Priority messages?                    */
            Obsolete,
            ShortIntNotUsed18,
            ShortIntNotUsed19,
            ShortIntNotUsed20,
            ShortIntNotUsed21,
            ShortIntNotUsed22,
            ShortIntNotUsed23,
            ShortIntNotUsed24,
            ShortIntNotUsed25,
            ShortIntNotUsed26,
            ShortIntNotUsed27,
            ShortIntNotUsed28,
            ShortIntNotUsed29,
            ShortIntNotUsed30,
            ShortIntNotUsed31,
            ShortIntNotUsed32,
            ShortIntNotUsed33,
            ShortIntNotUsed34,
            ShortIntNotUsed35,
            ShortIntNotUsed36,
            ShortIntNotUsed37,
            ShortIntNotUsed38,
            ShortIntNotUsed39,
            ShortIntNotUsed40,
            ShortIntNotUsed41,
            ShortIntNotUsed42,
            ShortIntNotUsed43,
            ShortIntNotUsed44,
            ShortIntNotUsed45,
            ShortIntNotUsed46,
            ShortIntNotUsed47,
            ShortIntNotUsed48,
            ShortIntNotUsed49,
            ShortIntNotUsed50;
};

/*
 * -----------------------------------------------------------------------------
 */

/* 
 * FAME:Settings/Access/Conferences/(Conf Alias)
 */

struct FAMEConfAccess
{
  char      ConfAlias[22],
            ConfFlags[1002],
            StrNotUsed1[32],
            StrNotUsed2[32];
  ULONG     ULongNotUsed1,
            ULongNotUsed2,
            ULongNotUsed3,
            ULongNotUsed4;
  long      FAMEVers,
            LongNotUsed1;
  short int ShortIntNotUsed1,
            ShortIntNotUsed2,
            ShortIntNotUsed3,
            ShortIntNotUsed4,
            ShortIntNotUsed5;
};

/*
 * -----------------------------------------------------------------------------
 */

/*
 * FAME:Settings/Access/Presets/Preset(x)
 */

struct FAMEPreset
{
  char      ConfAlias[22],
            StrNotUsed1[32],
            StrNotUsed2[32];
  ULONG     DailyByteLimit,
            DailyFileLimit,
            ULongNotUsed1,
            ULongNotUsed2;
  long      TimeLimit,
            LongNotUsed1,
            LongNotUsed2,
            AccessLevel,
            Ratio,
            FAMEVers,
            LongNotUsed3,
            LongNotUsed4,
            LongNotUsed5;
  USHORT    Preset,
            RatioType,
            UShortNotUsed1,
            UShortNotUsed2;
  short int UserFileBase,
            ShortIntNotUsed1,
            ShortIntNotUsed2,
            ShortIntNotUsed3,
            ShortIntNotUsed4,
            ShortIntNotUsed5,
            ShortIntNotUsed6,
            ShortIntNotUsed7,
            ShortIntNotUsed8,
            ShortIntNotUsed9,
            ShortIntNotUsed10,
            ShortIntNotUsed11,
            ShortIntNotUsed12,
            ShortIntNotUsed13,
            ShortIntNotUsed14,
            ShortIntNotUsed15,
            ShortIntNotUsed16,
            ShortIntNotUsed17,
            ShortIntNotUsed18,
            ShortIntNotUsed19,
            ShortIntNotUsed20;
};

/*
 * -----------------------------------------------------------------------------
 */

/*
 * FAME:Settings/Activities.dat
 */

struct FAMEActivities
{
  char      StrNotUsed1[32],
            StrNotUsed2[32],
            StrNotUsed3[32],
            StrNotUsed4[102];
  ULONG     CallLogNumLines,
            UDLogNumLines,
            ULongNotUsed1,
            ULongNotUsed2,
            ULongNotUsed3,
            ULongNotUsed4,
            ULongNotUsed5,
            ULongNotUsed6;
  long      CLLine,
            UDLLine;
  ULONG     LongNotUsed7,
            LongNotUsed8,
            LongNotUsed9,
            LongNotUsed10;
  long      FAMEVers,
            LongNotUsed1,
            LongNotUsed2,
            LongNotUsed3,
            LongNotUsed4,
            LongNotUsed5;
  short int DoIncCallLog,
            ShortIntNotUsed21,
            DoSteCallLog,
            DoIncUDLog,
            ShortIntNotUsed22,
            DoSteUDLog,
            ShortIntNotUsed1,
            ShortIntNotUsed2,
            ShortIntNotUsed3,
            ShortIntNotUsed4,
            ShortIntNotUsed5,
            ShortIntNotUsed6,
            ShortIntNotUsed7,
            ShortIntNotUsed8,
            ShortIntNotUsed9,
            ShortIntNotUsed10,
            ShortIntNotUsed11,
            ShortIntNotUsed12,
            ShortIntNotUsed13,
            ShortIntNotUsed14,
            ShortIntNotUsed15,
            ShortIntNotUsed16,
            ShortIntNotUsed17,
            ShortIntNotUsed18,
            ShortIntNotUsed19,
            ShortIntNotUsed20;
};

/*
 * -----------------------------------------------------------------------------
 */

/* 
 * FAME:Settings/Error/Error.dat
 */

struct FAMEError
{
  char      Errors[102][4];
  ULONG     ULongNotUsed1,
            ULongNotUsed2;
  long      FAMEVers;
  short int ShortIntNotUsed1,
            ShortIntNotUsed2,
            ShortIntNotUsed3,
            ShortIntNotUsed4,
            ShortIntNotUsed5,
            ShortIntNotUsed6,
            ShortIntNotUsed7,
            ShortIntNotUsed8,
            ShortIntNotUsed9,
            ShortIntNotUsed10;
};

/*
 * -----------------------------------------------------------------------------
 */

/*
 * FAME:Settings/Doors/BBSCMD|CONF(x)CMD|SYSCMD/(Command name).dat
 */

struct FAMEDoors
{
  char      DoorCallName[22],          /* Door call name                        */
            DoorPath[102],             /* Path of the door                      */
            Password[22],              /* Password to call door                 */
            StrNotUsed1[32],
            InternalCMD[22],           /* Internal command                      */
            ServerName[22],            /* Name at Server display                */
            StrNotUsed2[58];
  ULONG     ULongNotUsed1,
            ULongNotUsed2;
  long      StackSize,                 /* Stack size                            */
            LongNotUsed1,
            LongNotUsed2,
            AccessLevel,               /* Access level                          */
            ConfNum,                   /* Number, if CONFCMD door than > 0 else */
                                       /* -1 = BBSCMD, -2 = SYSCMD, -3 = DOORCFG*/
            Priority,                  /* Priority                              */
            FAMEVers;
  USHORT    SingleMulti,               /* Single / multi node door              */
            IntfModType,               /* Interface module type                 */
            UShortNotUsed1,
            UShortNotUsed2;
  short int NR,                        /* Normal cmd retreive/send              */
            NC,                        /* Normal cmd change                     */
            CF,                        /* Call function                         */
            SR,                        /* System cmd retreive/send              */
            SC,                        /* System cmd change                     */
            AR,                        /* Additional cmd retreive/send          */
            AC,                        /* Additional cmd change                 */
            RD,                        /* Reserved                              */
            UNUSED,                    /* Not used yet!                         */
            Obsolete,                  /* Obsolete                              */
            Internal,                  /* Internal command                      */
            Menu,                      /* Menu command                          */
            ULDDrop,
            ShortIntNotUsed7,
            ShortIntNotUsed8,
            ShortIntNotUsed9,
            ShortIntNotUsed10;
};

/*
 * -----------------------------------------------------------------------------
 */

/* 
 * FAME:Settings/InternalDoors.dat
 */

struct FAMEInternDoors
{
  char      StrNotUsed1[32],
            StrNotUsed2[102];
  ULONG     ULongNotUsed1,
            ULongNotUsed2,
            ULongNotUsed3,
            ULongNotUsed4;
  long      FAMEVers,
            LongNotUsed1;
  short int Frontend,
            WHO,
            OLM,
            NODE,
            ShortIntNotUsed4,
            ShortIntNotUsed5,
            ShortIntNotUsed6,
            ShortIntNotUsed7,
            ShortIntNotUsed8,
            ShortIntNotUsed9,
            ShortIntNotUsed10,
            ShortIntNotUsed11,
            ShortIntNotUsed12,
            ShortIntNotUsed13,
            ShortIntNotUsed14,
            ShortIntNotUsed15,
            ShortIntNotUsed16,
            ShortIntNotUsed17,
            ShortIntNotUsed18,
            ShortIntNotUsed19,
            ShortIntNotUsed20;
};

/*
 * -----------------------------------------------------------------------------
 */

/* 
 * FAME:Settings/Screens/ScrNode(x).dat
 */

struct FAMEScreens
{
  char      StrNotUsed1[32],
            StrNotUsed2[102];
  ULONG     ULongNotUsed1,
            ULongNotUsed2,
            ULongNotUsed3,
            ULongNotUsed4;
  long      NewUserPWMinNumLines,
            PrivateMinNumLines,
            PrivateMinBrkLvl,
            NewUserPWMinBrkLvl,
            TitleMinNumLines,
            TitleMinBrkLvl,
            LogoffMinNumLines,
            LogonMinNumLines,
            LogonMinBrkLvl,
            LogoffMinBrkLvl,
            BullTxtMinNumLines,
            BullTxtMinBrkLvl,
            JoinConfMinNumLines,
            JoinConfMinBrkLvl,
            FAMEVers,
            LongNotUsed1,
            NodeNumber;
  USHORT    UShortNotUsed1;
  short int ShowPrivate,
            ShowTitle,
            ShowLogon,
            ShowBull,
            ShowJoinConf,
            ShowNewUserPW,
            ShowLogoff,
            ShortIntNotUsed8,
            ShortIntNotUsed9,
            ShortIntNotUsed10;
};

/*
 * -----------------------------------------------------------------------------
 */

/*
 * FAME:Settings/Node(x)/Node.dat
 */

struct FAMENode
{
  char      NodeStPath[102],           /* Node start path                       */
            ScreenPath[102],           /* Screens path                          */
            FilesNotAll[102],          /* Files not allowed path                */
            LogoffBatch[102],          /* LogoffBatch path                      */
            ShutDBatch[102],           /* ShutDBatch path                       */
            SystemPW[22],              /* System password                       */
            SystemPWPrompt[256],       /* System password prompt                */
            NewUserPW[22],             /* Newuser password                      */
            NewUserPWPrompt[256],      /* Newuser password prompt               */
            SysOpChatCol[22],          /* SysOp chat color                      */
            UserChatCol[22],           /* User chat color                       */
            UserNamePrompt[256],       /* UserName prompt                       */
            UserPWPrompt[256],         /* User password prompt                  */
            PausePrompt[256],          /* Pause prompt                          */
            NewUserKey[32],            /* String like "Guest"                   */
            RipSuffix[12],             /* Suffix for RIP gfx                    */
            NETID[32],                 /* NetID                                 */
            NamesNotAll[102],          /* Names not allowed path                */
            MenuPrompt[512],           /* Default Menu prompt (with MCI)        */
            NodeName[128],             /* A SysOp definable name for the Node   */
            NodeTel[128],              /* A SysOp definable Node telephone nr.  */
            NodeNameShort[8],          /* A SysOp definable name for the Node   */
            StrNotUsed3[248],
            StrNotUsed4[256],
            StrNotUsed5[256];
  ULONG     ULongNotUsed1,
            ULongNotUsed2;
  long      Timeout1,                  /* Timeout warning                       */
            Timeout2,                  /* Timeout user kick                     */
            DoorTimeout,               /* Door timeout                          */
            BatchStackSize,            /* Batch stack size                      */
            NodePri,                   /* Node priority                         */
            BatchPriority,             /* Batch priority                        */
            BullTxtMinNumLines,        /* Bull.txt min. line length             */
            BullTxtMinBrkLvl,          /* Bull.txt min. break level             */
            NewUserStatus,             /* No newuser/ask for newuser/wait for   */
                                       /* NewUserKey                            */
            RipStatus,                 /* What to do on CON by RIP (Show        */
                                       /* nothing/say viewing rip gfx/view      */
                                       /* normal txt)                           */
            IfNoBaudTime,              /* What to do if BaudTime not found      */
            FAMEVers,
            ConfPreset,
            MinUserNameChars,
            MinUserPWChars,
            LongNotUsed1,
            LongNotUsed2;
  short int StealthMode,
            UnixMode,
            FreeDownloadNode,
            TrapDoorMode,
            ExitTrapDoor,
            SkipLocalCPS,
            ShortIntNotUsed4,
            ShortIntNotUsed5,
            ShortIntNotUsed6,
            ShortIntNotUsed7;
  ULONG     NewUserEntryFlags,
            ULongNotUsed4,
            ULongNotUsed5,
            ULongNotUsed6,
            ULongNotUsed7,
            ULongNotUsed8,
            ULongNotUsed9,
            ULongNotUsed10,
            ULongNotUsed11,
            ULongNotUsed12,
            ULongNotUsed13,
            ULongNotUsed14;
  long      NodeNumber;                /* Node number (x)                       */
  ULONG     UserEdType;                /* 0 = hidden ed, 1 = type to user,      */
                                       /* 2 = show user                         */
  short int IdleNode,                  /* Node idle (standby)                   */
            QuietNode,                 /* Node quiet (WHO)                      */
            DoCallersLog,              /* Do Callerslog                         */
            DoUDLog,                   /* Do UDLog                              */
            DoDoorLog,                 /* Do DoorLog                            */
            DoStartLog,                /* Do StartLog                           */
            DisQuickLogons,            /* No quick logons                       */
            NoTimeout,                 /* No timeout                            */
            NoMCIMsg,                  /* No MCI messages                       */
            FreeResume,                /* Free resumeing                        */
            BreakChat,                 /* Chat breakable with CTRL-C            */
            PWsInsensitive,            /* Passwords case type                   */
            ChatOn,                    /* Chat switched on                      */
            CapitolFiles,              /* All file names upper case             */
            SModem,                    /* SModem protocol                       */
            ANSIPrompt,                /* Ansi Prompt on/off                    */
            Hydra,                     /* Hydra protocol                        */
            Rip,                       /* Rip gfx                               */
            NoExtSysOpLg,              /* SysOp can't logon via modem           */
            SysOpCanRead,              /* SysOp can see passwords not '***'     */
            ShowBull,                  /* Show bull.txt                         */
            AsyncStartbat,             /* Async. Startbatches?                  */
            AsyncLogonbat,             /* Async. Logonbatches?                  */
            AsyncLogoffbat,            /* Async. Logoffbatches?                 */
            AsyncShutDownbat,          /* Async. ShutDownbatches?               */
            DispZeroBytes,             /* Display zero bytes                    */
            DefaultOH,                 /* Default off hook                      */
            ShanghaiWnd,               /* Shanghai windows?                     */
            RelativeConfs;             /* Relative conferences?                 */
};

/*
 * -----------------------------------------------------------------------------
 */

/*
 * FAME:Settings/Node(x)/Serial.dat
 */

struct FAMESerial
{
  char      SerialDriver[102],         /* Serial driver                         */
            StrNotUsed1[32],
            StrNotUsed2[102];
  ULONG     ULongNotUsed1,
            ULongNotUsed2;
  long      InitBaud,                  /* Init baudrate                         */
            LongNotUsed1,
            LongNotUsed2,
            SerialUnit,                /* Serial unit number                    */
            FAMEVers,
            LongNotUsed3,
            NodeNumber;                /* Node number (x)                       */
  USHORT    UShortNotUsed1;
  short int RADBoogie,
            XDisabled,
            SevenWire,
            Shared,
            UseOwnDevUnit,
            NotuseifShared,
            ReleaseOnline,
            DeviceReleaseKeepOpen,
            ShortIntNotUsed9,
            ShortIntNotUsed10;
};

/*
 * -----------------------------------------------------------------------------
 */

/* 
 * FAME:Settings/Node(x)/Modem.dat
 */

struct FAMEModem
{
  char      ModemInit[102],            /* Modem init string                     */
            ModemReset[52],            /* Modem reset string                    */
            ModemRing[52],             /* Modem ring string                     */
            ModemAnswer[52],           /* Modem answer string                   */
            ModemOffHook[52],          /* Modem off hook string                 */
            StrNotUsed1[32],
            StrNotUsed2[102];
  ULONG     ULongNotUsed1,
            ULongNotUsed2,
            ULongNotUsed3,
            ULongNotUsed4;
  long      FAMEVers,
            LongNotUsed1,
            NodeNumber;                /* Node number (x)                       */
  USHORT    UShortNotUsed1;
  short int NumberOfRings,
            ShortIntNotUsed2,
            ShortIntNotUsed3,
            ShortIntNotUsed4,
            ShortIntNotUsed5,
            ShortIntNotUsed6,
            ShortIntNotUsed7,
            ShortIntNotUsed8,
            ShortIntNotUsed9,
            ShortIntNotUsed10;
};

/*
 * -----------------------------------------------------------------------------
 */

/* 
 * FAME:Settings/Node(x)/Screen.dat
 */

struct FAMEScreen
{
  char      FontName[32],              /* Font name                             */
            ScreenName[32],            /* Screen name                           */
            StrNotUsed1[32],
            StrNotUsed2[86];
  long      WndLeftEdge,               /* Window left edge                      */
            WndTopEdge,                /* Window top edge                       */
            WndWidth,                  /* Window width                          */
            WndHeight;                 /* Window height                         */
  ULONG     ScreenID,                  /* Selected screen                       */
            ULongNotUsed2;
  long      LongNotUsed1,
            CycFlag,                   /* Cyclegadget flag                      */
            LongNotUsed2,
            BitPlanes,                 /* Bitplanes                             */
            LeftEdge,                  /* Screen left edge                      */
            TopEdge,                   /* Screen top edge                       */
            Width,                     /* Screen width                          */
            Height,                    /* Screen height                         */
            FontSize,                  /* Font size                             */
            FAMEVers,
            LongNotUsed3,
            NodeNumber;                /* Node number (x)                       */
  USHORT    UShortNotUsed1;
  short int Iconified,                 /* Iconified                             */
            ScreenFront,               /* Screen to front                       */
            AutoScroll,                /* Auto scroll                           */
            WindowOnly,
            ShortIntNotUsed3,
            ShortIntNotUsed4,
            ShortIntNotUsed5,
            ShortIntNotUsed6,
            ShortIntNotUsed7,
            ShortIntNotUsed8,
            ShortIntNotUsed9,
            ShortIntNotUsed10;
};

/*
 * -----------------------------------------------------------------------------
 */

/* 
 * FAME:Settings/FileCheckers/(Extension).dat
 */

struct FAMEChecker
{
  char      Extension[12],             /* Extension name                        */
            CheckerPath[102],          /* Checker path                          */
            StrNotUsed1[102],
            Error1[82],                /* Error message 1                       */
            Error2[82],                /* Error message 2                       */
            Error3[82],                /* Error message 3                       */
            Error4[82],                /* Error message 4                       */
            Error5[82],                /* Error message 5                       */
            Options[102],              /* Options                               */
            StrNotUsed2[102],
            FileIdExtractor[102],      /* FileId extractor tool                 */
            StrNotUsed3[102],
            StrNotUsed4[102];
  ULONG     ULongNotUsed1,
            ULongNotUsed2;
  long      StackSize,                 /* Stack size                            */
            FidExStackSize,            /* Stack size for FileId extractor tool  */
            LongNotUsed1,
            Priority,                  /* Priority                              */
            CheckerOn,                 /* Checker on/off                        */
            FAMEVers,
            LongNotUsed3;
  USHORT    UShortNotUsed1,
            UShortNotUsed2;
  short int ShortIntNotUsed1,
            ShortIntNotUsed2,
            ShortIntNotUsed3,
            ShortIntNotUsed4,
            ShortIntNotUsed5,
            ShortIntNotUsed6,
            ShortIntNotUsed7,
            ShortIntNotUsed8,
            ShortIntNotUsed9,
            ShortIntNotUsed10;
};

/*
 * -----------------------------------------------------------------------------
 */

/* 
 * FAME:Settings/Language/TextLanguage.dat
 */

struct FAMETextLang
{
  char      TxtLanType[32],
            Extension[12];
  ULONG     ULongNotUsed1,
            ULongNotUsed2,
            ULongNotUsed3,
            ULongNotUsed4;
  long      FAMEVers,
            LongNotUsed1;
  USHORT    UShortNotUsed1,
            UShortNotUsed2;
  short int ShortIntNotUsed1,
            ShortIntNotUsed2,
            ShortIntNotUsed3,
            ShortIntNotUsed4,
            ShortIntNotUsed5,
            ShortIntNotUsed6,
            ShortIntNotUsed7,
            ShortIntNotUsed8,
            ShortIntNotUsed9,
            ShortIntNotUsed10;
};

/*
 * -----------------------------------------------------------------------------
 */

/* 
 * FAME:Settings/Language/LangLocale.dat
 */

struct FAMELangLocale
{
  char      ServerLocale[102],         /* Server locale catalog                 */
            MainPartLocale[102],       /* MainPart locale catalog               */
            SystemEdLocale[102],       /* SystemEd locale catalog               */
            UserEdLocale[102],         /* UserEd locale catalog                 */
            StrNotUsed1[32],
            StrNotUsed2[102];
  ULONG     ULongNotUsed1,
            ULongNotUsed2,
            ULongNotUsed3,
            ULongNotUsed4;
  long      FAMEVers,
            LongNotUsed1;
  USHORT    UShortNotUsed1,
            UShortNotUsed2;
  short int ShortIntNotUsed1,
            ShortIntNotUsed2,
            ShortIntNotUsed3,
            ShortIntNotUsed4,
            ShortIntNotUsed5,
            ShortIntNotUsed6,
            ShortIntNotUsed7,
            ShortIntNotUsed8,
            ShortIntNotUsed9,
            ShortIntNotUsed10;
};

/*
 * -----------------------------------------------------------------------------
 */

/*
 * FAME:Settings/DoorConfig/DoorConfig(x).dat
 */

struct FAMEDoorConfig
{
  char      DoorCnfName[22],
            DoorCnfPath[102],
            Password[22],
            Arguments[22],
            StrNotUsed2[112];
  ULONG     ULongNotUsed1,
            ULongNotUsed2;
  long      StackSize,
            Priority,
            AccessLevel,
            Number,
            FAMEVers,
            LongNotUsed1;
  USHORT    UShortNotUsed1,
            UShortNotUsed2;
  short int NR,
            NC,
            CF,
            SR,
            SC,
            AR,
            AC,
            RD,
            UNUSED,
            ShortIntNotUsed1,
            ShortIntNotUsed2;
};

/*
 * -----------------------------------------------------------------------------
 */

/*
 * FAME:Settings/UserHandler/UserHandler(x).dat
 */

struct FAMEUserHandler
{
  char      UserHandlerName[22],
            UserHandlerPath[102],
            StrNotUsed1[32],
            StrNotUsed2[102];
  ULONG     ULongNotUsed1,
            ULongNotUsed2;
  long      StackSize,
            Priority,
            Number,
            FAMEVers,
            LongNotUsed1;
  USHORT    UShortNotUsed1,
            UShortNotUsed2;
  short int ShortIntNotUsed1,
            ShortIntNotUsed2;
};

/*
 * -----------------------------------------------------------------------------
 */

/*
 * FAME:Modules/FAMEModules.dat
 */

struct FAMEModules
{
  char      ServerLocation[102],       /* Server location path                  */
            MainPartLocation[102],     /* MainPart location path                */
            SystemEdLocation[102],     /* SystemEd location path                */
            UserEdLocation[102],       /* UserEd location path                  */
            ServerInfoLocation[102],   /* ServerInfo location path              */
            FileCheckerLocation[102],  /* FileChecher location                  */
            StrNotUsed1[102],
            StrNotUsed2[102],
            StrNotUsed3[102],
            StrNotUsed4[102],
            StrNotUsed5[102],
            StrNotUsed6[102],
            StrNotUsed7[102],
            StrNotUsed8[102],
            StrNotUsed9[102],
            StrNotUsed10[102],
            StrNotUsed11[102],
            StrNotUsed12[102],
            StrNotUsed13[102],
            StrNotUsed14[102],
            StrNotUsed15[102],
            StrNotUsed16[102],
            StrNotUsed17[102],
            StrNotUsed18[102],
            StrNotUsed19[102],
            StrNotUsed20[102];
  ULONG     ULongNotUsed1,
            ULongNotUsed2,
            ULongNotUsed3,
            ULongNotUsed4;
  long      FAMEVers,
            FileCheckerStack;          /* FileChecker stack                     */
  USHORT    UShortNotUsed1,
            UShortNotUsed2;
  short int FileCheckerPriority,       /* FileChecher priority                  */
            FileCheckerFlag,           /* FileChecher flag                      */
            ShortIntNotUsed3,
            ShortIntNotUsed4,
            ShortIntNotUsed5,
            ShortIntNotUsed6,
            ShortIntNotUsed7,
            ShortIntNotUsed8,
            ShortIntNotUsed9,
            ShortIntNotUsed10;
};

/*
 * -----------------------------------------------------------------------------
 */

/*
 * FAME:Settings/ComputerTypes.dat
 */

struct FAMEComputerTypes
{
  char      CompiType[36];             /* Computer Type                         */
};

/*
 * -----------------------------------------------------------------------------
 */

/*
 * FAME:Settings/ModemTypes.dat
 */

struct FAMEModemTypes
{
  char      ModemType[36];             /* Modem Type                            */
};

/*
 * -----------------------------------------------------------------------------
 */

/*
 * FAME:Settings/Misc.dat
 */

struct FAMEMisc
{
  char      RexxPort[32],              /* Rexx port name                        */
            XDoorPort[32],             /* XIM port name                         */
            PDoorPort[32],             /* Paragon port name                     */
            ServerPort[32],            /* Server port name                      */
            FAMEDoorPort[32],          /* FAME port name                        */
            FAMEToolPort[32],          /* FAME tool port name                   */
            StrNotUsed2[32],
            StrNotUsed3[32];
  ULONG     ULongNotUsed1,
            ULongNotUsed2,
            ULongNotUsed3,
            ULongNotUsed4,
            ULongNotUsed5,
            ULongNotUsed6,
            ULongNotUsed7,
            ULongNotUsed8,
            ULongNotUsed9,
            ULongNotUsed10;
  long      FAMEVers,
            LongNotUsed1,
            LongNotUsed2,
            LongNotUsed3,
            LongNotUsed4;
  USHORT    UShortNotUsed1,
            UShortNotUsed2,
            UShortNotUsed3,
            UShortNotUsed4,
            UShortNotUsed5;
  short int MultiComPort,
            ShortIntNotUsed2,
            ShortIntNotUsed3,
            ShortIntNotUsed4,
            ShortIntNotUsed5,
            ShortIntNotUsed6,
            ShortIntNotUsed7,
            ShortIntNotUsed8,
            ShortIntNotUsed9,
            ShortIntNotUsed10;
};

/*
 * -----------------------------------------------------------------------------
 */

/*
 * FAME:Settings/Protocol.dat
 */

struct FAMEProtocol
{
  char      XprLibrary[20][32],        /* Library name 1- 20                    */
            StrNotUsed1[32],
            StrNotUsed2[32],
            StrNotUsed3[32],
            StrNotUsed4[32],
            StrNotUsed5[32];
  ULONG     DefaultBuffer,             /* Default buffer size                   */
            MaxUserBlockSize,          /* Max user block size allowed           */
            DefaultBlockSize,          /* Default block size                    */
            ULongNotUsed2,
            ULongNotUsed3,
            ULongNotUsed4,
            ULongNotUsed5,
            ULongNotUsed6,
            ULongNotUsed7,
            ULongNotUsed8,
            ULongNotUsed9,
            ULongNotUsed10;
  long      LongNotUsed5,
            FAMEVers,
            LongNotUsed1,
            LongNotUsed2,
            LongNotUsed3,
            LongNotUsed4;
  USHORT    UShortNotUsed1,
            UShortNotUsed2,
            UShortNotUsed3,
            UShortNotUsed4,
            UShortNotUsed5;
  short int UserBlockSizeAllow,        /* Allowes user to set the buffer        */
            UserHydraAllow,            /* Allowes Hydra                         */
            UserSModemAllow,           /* Allowes SModem                        */
            ShortIntNotUsed2,
            ShortIntNotUsed3,
            ShortIntNotUsed4,
            ShortIntNotUsed5,
            ShortIntNotUsed6,
            ShortIntNotUsed7,
            ShortIntNotUsed8,
            ShortIntNotUsed9,
            ShortIntNotUsed10;
};

/*
 * -----------------------------------------------------------------------------
 */

/* 
 * FAME:Settings/BaudTimes/(Baud).dat
 */

struct FAMEBaudTimes
{
  char      StrNotUsed1[44];
  long      BaudRate;
  ULONG     StartHH,
            StartMM,
            EndHH,
            EndMM,
            ULongNotUsed;
  long      FAMEVers,
            LongNotUsed1,
            LongNotUsed2,
            LongNotUsed3,
            LongNotUsed4;
  short int ShortIntNotUsed1,
            ShortIntNotUsed2,
            ShortIntNotUsed3,
            ShortIntNotUsed4,
            ShortIntNotUsed5;
};

/*
 * -----------------------------------------------------------------------------
 */

/*
 * FAME:Settings/UserButtons.dat
 */

struct FAMEUserButtons
{
  char      ButtonName[28][12],
            ButtonCommand[28][102];

};

/*
 * -----------------------------------------------------------------------------
 */

/*
 * FAME:Settings/FAMEHydra.dat
 */

struct FAMEHydra
{
  char      StringNotUsed[1024];
  ULONG     ULongNotUsed1,
            ULongNotUsed2,
            ULongNotUsed3,
            ULongNotUsed4,
            ULongNotUsed5;
  long      LongNotUsed1,
            LongNotUsed2,
            LongNotUsed3,
            LongNotUsed4,
            LongNotUsed5;
  short int WriteLogFile,
            RememberWndPos,
            ShortIntNotUsed1,
            ShortIntNotUsed2,
            ShortIntNotUsed3,
            ShortIntNotUsed4,
            ShortIntNotUsed5;
};

/*
 * -----------------------------------------------------------------------------
 */

/*
 * FAME:Settings/Node<x>/FAMEHydra.dat
 */

struct FAMEHydraNode
{
  char      StringNotUsed[1024];
  ULONG     ULongNotUsed1,
            ULongNotUsed2,
            ULongNotUsed3,
            ULongNotUsed4,
            ULongNotUsed5;
  long      ReceiveWndLeft,
            ReceiveWndTop,
            ReceiveWndWidth,
            ReceiveWndHeight,
            SendWndLeft,
            SendWndTop,
            SendWndWidth,
            SendWndHeight,
            ChatReadWndLeft,
            ChatReadWndTop,
            ChatReadWndWidth,
            ChatReadWndHeight,
            ChatWriteWndLeft,
            ChatWriteWndTop,
            ChatWriteWndWidth,
            ChatWriteWndHeight,
            ZoomMinX,
            ZoomMinY,
            ZoomMaxX,
            ZoomMaxY,
            LongNotUsed;
  short int ShortIntNotUsed1,
            ShortIntNotUsed2,
            ShortIntNotUsed3,
            ShortIntNotUsed4,
            ShortIntNotUsed5;
};

/*
 * -----------------------------------------------------------------------------
 */

/*
 * FAME:Settings/FAMESModem.dat
 */

struct FAMESModem
{
  char      OtherPubScreen[102],
            FallBackPubScreen[102],
            StringNotUsed[1024];
  ULONG     ULongNotUsed1,
            ULongNotUsed2,
            ULongNotUsed3,
            ULongNotUsed4,
            ULongNotUsed5;
  long      LongNotUsed1,
            LongNotUsed2,
            LongNotUsed3,
            LongNotUsed4,
            LongNotUsed5;
  short int DefPublicScreen,
            WriteLogfile,
            CheckCarrier,
            RTSCTS,
            Encapsulation,
            AutoExit,
            RemoteYell,
            RemoteAutoDl,
            UlonnormalDl,
            UlonDoorDl,
            UlonMessageDl,
            UlonASLDl,
            ShortIntNotUsed1,
            ShortIntNotUsed2,
            ShortIntNotUsed3,
            ShortIntNotUsed4,
            ShortIntNotUsed5;
};

/*
 * -----------------------------------------------------------------------------
 */

/*
 * FAME:Settings/FAMEDebug.dat
 */

struct FAMEDebug
{
  char      FIMCmdDataLog[256],
            XIMCmdDataLog[256],
            AIMCmdDataLog[256],
            TIMCmdDataLog[256],
            DebugLog[256],
            StringNotUsed[4096];
  ULONG     ULongNotUsed1,
            ULongNotUsed2,
            ULongNotUsed3,
            ULongNotUsed4,
            ULongNotUsed5;
  long      FNode,
            NNode,
            LNode,
            BufferSize,
            LongNotUsed1,
            LongNotUsed2,
            LongNotUsed3,
            LongNotUsed4;
  short int FIMCmdActive,
            FIMDatActive,
            FIMStrActive,
            XIMCmdActive,
            XIMDatActive,
            XIMStrActive,
            AIMCmdActive,
            AIMDatActive,
            AIMStrActive,
            TIMCmdActive,
            TIMDatActive,
            TIMStrActive,
            DebugLogActive,
            FNodeActive,
            NNodeActive,
            LNodeActive,
            AllNodes,
            StripANSI,
            UseBuffer,
            ShortIntNotUsed1,
            ShortIntNotUsed2,
            ShortIntNotUsed3,
            ShortIntNotUsed4,
            ShortIntNotUsed5,
            ShortIntNotUsed6,
            ShortIntNotUsed7,
            ShortIntNotUsed8;
};

/*
 * -----------------------------------------------------------------------------
 */

/*
 * FAME:Settings/Node%ld/ColPen.dat | FAME:Settings/DefColPen.dat
 */

struct FAMEColPenDat
{
  char      StringNotUsed[4096];
  short int RWSwap,
            CONRWSwap,
            ShortIntNotUsed1,
            ShortIntNotUsed2,
            ShortIntNotUsed3,
            ShortIntNotUsed4,
            ShortIntNotUsed5,
            ShortIntNotUsed6,
            ShortIntNotUsed7,
            ShortIntNotUsed8,
            ShortIntNotUsed9,
            ShortIntNotUsed10;
  ULONG     ULongNotUsed1,
            ULongNotUsed2,
            ULongNotUsed3,
            ULongNotUsed4,
            ULongNotUsed5;
}; 
/* -> Additional pens and screen colors will be added (means struct is dynamically in size). */

/*
 * -----------------------------------------------------------------------------
 */

/*
 * FAME:Settings/AwayMsgs.dat
 */

struct FAMEAwayMsg
{
  char      Msg1[66],
            Msg2[66],
            Msg3[66],
            Msg4[66],
            Msg5[66],
            Msg6[66],
            Msg7[66],
            Msg8[66],
            Msg9[66],
            Msg10[66],
            VariableMsg[66],
            StringNotUsed[1024];
  STRPTR    SelectedMsg;
  ULONG     ULongNotUsed1,
            ULongNotUsed2,
            ULongNotUsed3,
            ULongNotUsed4,
            ULongNotUsed5;
  short int DefaultAwayMsg,
            ShortIntNotUsed1,
            ShortIntNotUsed2,
            ShortIntNotUsed3,
            ShortIntNotUsed4,
            ShortIntNotUsed5,
            ShortIntNotUsed6,
            ShortIntNotUsed7,
            ShortIntNotUsed8,
            ShortIntNotUsed9,
            ShortIntNotUsed10;
};

/*
 * -----------------------------------------------------------------------------
 */

#endif  /* FAME_FAMEINTERNSETSTRUCTS_H */

