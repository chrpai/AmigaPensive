
/*
 * FAME User Structs
 * ~~~~~~~~~~~~~~~~~
 *
 * $VER: FAMEUserStructs.h v1.5
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

//#include <exec/types.h>
//#include <stdio.h>

#ifndef FAME_FAMEUSERSTRUCTS_H
#define FAME_FAMEUSERSTRUCTS_H

/*
 * -----------------------------------------------------------------------------
 */

/*
 * Do not use Expansion structures.
 * They are only defined for future usage.
 */

struct  FAMEUserExpansion
{
  long Empty;
};

struct  FAMEUserConfExpansion
{
  long Empty;
};

/*
 * -----------------------------------------------------------------------------
 */

/*
 * Main FAME user structure.
 *
 * Default filename will be "User.data"
 * Default location will be "FAME:UserData/User.data"
 *
 * Filename and location can be changed by the SysOp
 * with the SystemEditor.
 *
 * IMPORTANT:
 * ----------
 *
 * "struct FAMEUser" is combined with "struct FAMEUserKeys".
 *
 * All changes to a user have to be done in *BOTH* structures
 * as long as the changed data field is also contending in
 * the other structure !!!
 */

struct  FAMEUser
{
  char      UserName[32],              /*XUserName                              */
            Password[22],              /*XPassword                              */
            UserLocation[32],          /*XLocation                              */
            UserFrom[32],              /*XFrom group                            */
            UserPhone[16],             /*XTelphone number                       */
            ConfAccess[22],            /*XUsers conference access               */
            Menuprompt[202],           /*XUsers menu prompt                     */
            Birthday[10],              /*XUsers birthday                        */
            DefCryptMePW[12],          /* Default krypt pw messages             */
            UserFBasePW[12],           /* Users FileBase password               */
            CursorUp[8],               /* Cursor up code                        */
            CursorDown[8],             /* Cursor down code                      */
            CursorRight[8],            /* Cursor left code                      */
            CursorLeft[8],             /* Cursor right code                     */
            StrNotUsed[30];
  struct    FAMEUserExpansion *UserExp;/* Expansion pointer                     */
  short int MADRedDefPub,
            MADDefPub,
            MADRedDefPriv,
            MADDefPriv,
            MADRedDefALL,
            MADDefALL,
            MADRedDefEALL,
            MADDefEALL,
            MADRedDefUEALL,
            MADDefUEALL;
  ULONG     ULONGNotUsed1,
            ULONGNotUsed2,
            BytesUpHi,                 /* Bytes uploaded  - 64 bit higher ULONG */
            BytesDownHi,               /* Bytes downloaded - 64 bit higher ULONG*/
            Calls,                     /*XNumber of calls                       */
            Uploads,                   /*xNumber of files uploaded              */
            BytesUpload,               /*xNumber of bytes uploaded              */
            Downloads,                 /*xNumber of files downloaded            */
            BytesDownload,             /*xNumber of bytes downloaded            */
            MessageWrite,              /*XNumber of messages written            */
            MessageRead,               /*XNumber of messages read               */
            DailyByteLimit,            /*xDaily download byte limit 0 = DISABLED*/
            DailyFileLimit,            /*xDaily download file limit 0 = DISABLED*/
            DailyByteBonus,            /*xByte bonus for the current day        */
            DailyFileBonus,            /*xFile bonus for the current day        */
            DailyByteDl,               /*xNumber of bytes downloaded today      */
            DailyFileDl,               /*xNumber of files downloaded today      */
            LastConf,                  /*XThe conf in which the user was last on*/
            CnfReJoin,                 /*XThe autojoin conference on logon sel. */
                                       /* by the user. 0 = join into LastConf   */
            BytesNuked,                /*xNuked bytes                           */
            FilesNuked,                /*xNuked files                           */
            NToNComFlag1,              /*XNode to Node communication flag 1     */
            NToNComFlag2,              /*XNode to Node communication flag 2     */
            NumberOfChats,             /*XNumber of chats                       */
            DayRelogins,               /*XNumber of relogins per day            */
            ShellNumLines;             /*XNumber of lines in FAME - Shell       */
  long      LastTime,                  /*XLast call                             */
            TimeLimit,                 /*XUsers time limit per day              */
            TimeUsed,                  /*XUsers time used this day              */
            FirstCall,                 /*XFirst call                            */
            HighCPSDown,               /*XHighest download CPS                  */
            HighCPSUP,                 /*XHighest upload CPS                    */
            BAUD,                      /*XBaudrate on logon                     */
            TimeBonus,                 /*xAdditional time after upload (daily)  */
            ChatTime,                  /* Chat time                             */
            ChatTimeUsed,              /* Chat time used                        */
            LowCPSDown,                /*XLowest download CPS                   */
            LowCPSUP,                  /*XLowest upload CPS                     */
            TimeTotal;                 /*XOnline time total, actual counted     */
  ULONG     UserFlags1,                /*-User flags 1                          */
            UserFlags2;                /* User flags 2                          */
  long      LongNotUsed1,
            LongNotUsed2;
  short int DefaultProt,               /*XDefault xfer protocol                 */
            Deleted_Or_Not,            /*X0 = valid, 1 = deleted, 2 = inactive  */
            UploadProt,                /*XSelected upload protocol              */
            Userlevel,                 /*XUser level                            */
            DownloadProt,              /*XSelected download protocol            */
            Ratio,                     /*xRatio. 0-n. 0 = disabled ratio        */
            MsgUploadProt,             /*XSelected message upload protocol      */
            RatioType,                 /*xRatiotype. 0 = Byte 1 = B/F 2 = File  */
            MsgDownloadProt,           /*XSelected message download protocol    */
            Ansi_On_Off,               /*X0 = ansi off, 1 = ansi on, 2 = prompt */
            DoorUploadProt,            /*XSelected door upload protocol         */
            NewScan;                   /*X0 = no mail/file scan, 1 = mail/file  */
                                       /* scan, 2 = mail scan, 3 = file scan,   */
                                       /* 4 = prompt                            */
  long      AllHacks,                  /* Number of hacks overall               */
            LastHacks;                 /* Last number of hacks. This can be     */
                                       /* reseted and can be used to let FAME   */
                                       /* start some actions defined by sysop,  */
                                       /* like inactivate this user and more... */
  short int DoorDownloadProt,          /*XSelected door download protocol       */
            NumLines,                  /*XNumber of lines                       */
            UserBlockSize,             /*XUsers selected xpr block size         */
            CompiType,                 /*XUsers selected computer type          */
            MADLvlOvrPub,
            ModemType,                 /*XUsers selected modem type             */
            MADLvlOvrPriv,
            Extension,                 /*XUsers selected text extension         */
            MADLvlOvrALL,
            Language,                  /*xUsers selected language               */
            MADLvlOvrEALL,
            ZoomType,                  /* Users selected zoom type              */
            MADLvlOvrUEALL,
            FreeForUsage;
  long      LostCarrier;               /* Number of lost carriers               */
  short int FreeFromShort14,
            FileIdDiz,                 /* Off/auto/ask for fileid tools         */
            FreeFromShort15,
            DoMsgCrypt;                /* Off/auto/ask for auto crypting msgs   */
  long      NumberOfPages;             /* Number of pages overall               */
  USHORT    UserNumber,                /*XUser number                           */
            Editor,                    /* 0 = line ed, 1 = full ed, 2 = external*/
                                       /* ed, 3 = prompt for ed                 */
            DayPages;                  /* Number of pages today                 */
};

/*
 * Usage of UserFlag1:            (UserFlag2 is currently unused !)
 *
 * byte 0:
 *   bit0:    UD_SPY              -  Capture user activities
 *   bit1:    UD_EXPERT           - X0 = menu text off else menue text on
 *   bit2:    UD_NEWUSER          - X0 = valid user else newuser
 *   bit3:    UD_MSGCLS           - XDo clear screens on messages
 *   bit4:    UD_FILECLS          - XDo clear screens on file lists
 *   bit5:    UD_UFLAG            - X0 = local upload else global upload
 *   bit6:    UD_DFLAG            - X0 = local download else global download
 *   bit7:    UD_UFBASE           -  Allow UserFileBase
 * byte 1:
 *   bit0:    UD_HIDE             - XHidden for WHO etc...
 *   bit1:    UD_MSGROOM          - XMessage rooming off/on
 *   bit2:    UD_STRED            -  String edit mode off/on
 *   bit3:    UD_SCRPAUSE         -  Pause on screen numlines reached
 *   bit4:    UD_FSTGOODBYE       -  Fast->No check for flags/partups/etc.
 *   bit5:    UD_VFLAG            -  0 = local else global file view
 *   bit6:    UD_ZFLAG            -  0 = local else global zippy search
 *   bit7:    UD_ASCIIMSG         -  XASCII mode off/on on writing messages
 * byte 2:
 *   bit0:    UD_SWAPREDWHITE     -  Red/white swap
 *   bit1:    UD_AUTOQUOTE        -  Quote all automatic
 *   bit2:    UD_KEEPREPLYPRIV    -  Keep private status on reply
 *   bit3:    UD_KEEPREPLYSUBJECT -  Keep message subject on reply
 *   bit4:    UD_FILECHECKPROCESS -  FileCheck process synchron or asynchron
 *   bit5:    UD_AUTOFORCEONWHO   -  Automatic force on WHO
 *   bit6:    UD_BACKGROUNDPOST   -  Background fileid extraction and file post
 *   bit7:    UD_FILENAMELENGTH   -  AmigaDOS (30)/PC (12) file name length
 * byte 3:
 *   bit0:
 *   bit1:
 *   bit2:
 *   bit3:
 *   bit4:
 *   bit5:
 *   bit6:
 *   bit7:
 */

/*
 * Usage of NToNComFlag1:            (NToNComFlag2 is currently unused !)
 *
 * byte 0:
 *   bit0:    NTN_DEFAULT         -  Default for everything not listed
 *   bit1:    NTN_BBSHELP         -  On BBS (DOOR) Help
 *   bit2:    NTN_BULLETINS       -  On reading bulletins
 *   bit3:    NTN_CHANGESTATS     -  On changing user stats
 *   bit4:    NTN_ENTEREALL       -  On entering EALL mails
 *   bit5:    NTN_FILELISTING     -  On viewing file listings
 *   bit6:    NTN_FLAGEDIT        -  On editing file flags
 *   bit7:    NTN_JOINCONF        -  On joining a conference
 * byte 1:
 *   bit0:    NTN_LOGGINGON       -  On logging on
 *   bit1:    NTN_MULTICHAT       -  On multichatting
 *   bit2:    NTN_NEWFILES        -  On newfiles scan
 *   bit3:    NTN_PREPAREDOWNL    -  On preparing for download
 *   bit4:    NTN_PREPAREUPL      -  On preparing for upload
 *   bit5:    NTN_READMAIL        -  On reading mails
 *   bit6:    NTN_SCANFILES       -  On scanning files (login or "MS" command)
 *   bit7:    NTN_SCANMAIL        -  On scanning mails (login or "MS" command)
 * byte 2:
 *   bit0:    NTN_SEARCHFOR       -  On searching for files
 *   bit1:    NTN_SHELL           -  On using the shell
 *   bit2:    NTN_SYSOPCHAT       -  On chatting with sysop
 *   bit3:    NTN_SYSOPCOMENT     -  On writing a sysop comment
 *   bit4:    NTN_VIEWFILES       -  On viewing files
 *   bit5:    NTN_VIEWTEXT        -  On viewing texts
 *   bit6:    NTN_WAITFORKEY      -  On prompts (except the menu)
 *   bit7:    NTN_WHO             -  On using who doors
 * byte 3:
 *   bit0:    NTN_WRITEMAIL       -  On writing a standard mail
 *   bit1:
 *   bit2:
 *   bit3:
 *   bit4:
 *   bit5:
 *   bit6:
 *   bit7:
 */

/*
 * -----------------------------------------------------------------------------
 */

/*
 * Main FAME user structure (short form).
 *
 * Default filename will be "User.Keys"
 * Default location will be "FAME:UserData/User.Keys"
 *
 * Filename and location can be changed by the SysOp
 * with the SystemEditor.
 *
 * "struct UserKeys" is used to do a fast scan (seek)
 * of users and the user state. The datas here are
 * just the really need datas to do a fast scan of users.
 *
 * The user entries here are completely parallel to the
 * ones in "struct User".
 *
 * With the seekpointer of "struct UserKeys" the seekpointer
 * for "struct User" will be known.
 *
 * IMPORTANT:
 * ----------
 *
 * "struct FAMEUserKeys" is combined with "struct FAMEUser".
 *
 * All changes to a user have to be done in *BOTH* structures
 * as long as the changed data field is also contending in
 * the other structure !!!
 */

struct  FAMEUserKeys
{
  char   UserName[32],                 /* UserName                              */
         StrNotUsed[32];
  int    Deleted_Or_Not,               /*X0 = valid user, 1 = deleted,          */
                                       /* 2 = inactive                          */
         IntNotUsed;
  USHORT UserNumber;                   /* User number                           */
  BOOL   NewUser;                      /* Newuser flag                          */
};

/*
 * -----------------------------------------------------------------------------
 */

/*
 * Local FAME user structure.
 *
 * Default filename will be "UserCnf.data"
 * Default location will be "<ConferenceLocation>/UserCnf.data"
 *
 * "struct FAMEUserCnf" contains the local user datas of a
 * specific conference. It's nearly the same data as in
 * "struct FAMEUser", but enhanced with datas like message base
 * (seek/read/scan) pointers and more.
 *
 * The user entries here are completely parallel to the
 * ones in "struct FAMEUser".
 *
 * With the seekpointer of "struct FAMEUserKeys" and/or "struct FAMEUser"
 * the seekpointer for "struct FAMEUser" will be known.
 */

struct  FAMEUserConf
{
  char      UserName[32],              /* UserName                              */
            ConfUserName[32],          /* UserName conf specific                */
            UserSentLine[46],          /* User defined tag line for SentBy      */
            StrNotUsed[98];
  struct    FAMEUserConfExpansion  *UserConfExp; /* Expansion pointer           */
  ULONG     ConfJoins,                 /* Number of conference joins            */
            Uploads,                   /* Files uploaded in this conference     */
            BytesUpload,               /* Bytes uploaded in this conference     */
            Downloads,                 /* Files downloaded in this conference   */
            BytesDownload,             /* Bytes downloaded in this conference   */
            MessageWrite,              /* Number of messages writen in this conf*/
            MessageRead,               /* Number of messages read in this conf  */
            DailyByteLimit,            /* Daily download byte limit 0 = DISABLED*/
            DailyFileLimit,            /* Daily download file limit 0 = DISABLED*/
            DailyByteBonus,            /* Byte bonus for the current day        */
            DailyFileBonus,            /* File bonus for the current day        */
            DailyByteDl,               /* Number of bytes downloaded today      */
            DailyFileDl,               /* Number of files downloaded today      */
            BytesNuked,                /* Nuked bytes                           */
            FilesNuked,                /* Nuked files                           */
            BytesUpHi,                 /* Bytes uploaded   - 64 bit higher ULONG*/
            BytesDownHi,               /* Bytes downloaded - 64 bit higher ULONG*/
            ULongNotUsed3,
            ULongNotUsed4,
            ULongNotUsed5;
  long      LastTime,                  /* Last conference join                  */
            TimeLimit,                 /* Time limit for this conference        */
            TimeUsed,                  /* Time used in this conference          */
            FirstConf,                 /* First conference join                 */
            HighCPSDown,               /*XHighest download CPS                  */
            HighCPSUP,                 /*XHighest upload CPS                    */
            LastFileScan,              /* Last file scan                        */
            MsgRead,                   /* Last message read                     */
            EMsgRead,                  /* Last EALL read                        */
            MsgWritten,                /* Last message writen                   */
            MsgScanned,                /* Last new scan message read            */
            TimeBonus,                 /* Additional time after upload (daily)  */
            LowCPSDown,                /*Xlowest download CPS                   */
            LowCPSUP,                  /*XLowest upload CPS                     */
            TimeTotal;                 /* Online time total, actual counted     */
  ULONG     UserFlags1,                /* User flags 1                          */
            UserFlags2;                /* User flags 2                          */
  long      LongNotUsed1,
            LongNotUsed2;
  short int NumOfConfPWFail,           /* Num of fails on entering the Conf PW. */
            Ratio,                     /* Ratio. 0-n. 0 = disabled ratio        */
            FreeFromShort2,
            RatioType,                 /* Ratiotype. 0 = Byte 1 = B/F 2 = File  */
                                       /* else GLOBAL ratio (user.data)         */
            FreeFromShort3,
            ReplyMSGFlag,              /* * OBSOLETE !!! *                      */
            FreeFromShort4,
            IntNotUsed;
  USHORT    UserNumber,                /* User number                           */
            UShortNotUsed;
};

/*
 * Usage of UserFlag1:              (UserFlag2 is currently unused !)
 *
 * byte 0:
 *   bit0:    UC_NEWMSG           -  1 = scan for messages else no scan
 *   bit1:    UC_NEWFILES         -  1 = scan for files else no scan
 *   bit2:    UC_ZOOM             -  Zooming off/on
 *   bit3:    UC_USERCNFXS        -  1 = user configurable access in conference
 *   bit4:    UC_OBSOLETE         -  Obsolete
 *   bit5:    UC_USCNFDATA        -  User conf datas or only user datas
 *   bit6:
 *   bit7:
 * byte 1:
 *   bit0:
 *   bit1:
 *   bit2:
 *   bit3:
 *   bit4:
 *   bit5:
 *   bit6:
 *   bit7:
 * byte 2:
 *   bit0:
 *   bit1:
 *   bit2:
 *   bit3:
 *   bit4:
 *   bit5:
 *   bit6:
 *   bit7:
 * byte 3:
 *   bit0:
 *   bit1:
 *   bit2:
 *   bit3:
 *   bit4:
 *   bit5:
 *   bit6:
 *   bit7:
 */

/*
 * -----------------------------------------------------------------------------
 */

/*
void main()
{
printf("%d\n",sizeof(struct FAMEUser));
printf("%d\n",sizeof(struct FAMEUserKeys));
printf("%d\n",sizeof(struct FAMEUserConf));
}
*/

#endif  /* FAME_FAMEUSERSTRUCTS_H */

