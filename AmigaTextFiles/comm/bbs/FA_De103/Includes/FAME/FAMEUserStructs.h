
/* FAME User Structs
 *
 * $VER:FAMEUserStructs.h v1.3
 *
 * Used tab size: 2
 */

//#include <exec/types.h>
//#include <stdio.h>

#ifndef FAME_FAMEUSERSTRUCTS_H
#define FAME_FAMEUSERSTRUCTS_H

/* Do not use Expansion structures.
 * They are only defined for future usage.
 */

struct	Expansion {
long			Empty;
};

struct	CnfExpansion {
long			Empty;
};

struct	User {
char			UserName[32],				/*XUserName																		*/
					Password[22],				/*XPassword																		*/
					UserLocation[32],		/*XLocation																		*/
					UserFrom[32],				/*XFrom group																	*/
					UserPhone[16],			/*XTelphone number														*/
					ConfAccess[22],			/*XUsers conference access										*/
					Menuprompt[202],		/*XUsers menu prompt													*/
					Birthday[10],				/*XUsers birthday															*/
					DefCryptMePW[12],		/* Default krypt pw messages									*/
					UserFBasePW[12],		/* Users FileBase password										*/
					CursorUp[8],				/* Cursor up code															*/
					CursorDown[8],			/* Cursor down code														*/
					CursorRight[8],			/* Cursor left code														*/
					CursorLeft[8],			/* Cursor right code													*/
					StrNotUsed[30];
struct		Expansion				*UserExp;	/* Expansion pointer										*/
ULONG			ULONGNotUsed1,
					ULONGNotUsed2,
					ULONGNotUsed3,
					ULONGNotUsed4,
					ULONGNotUsed5,
					ULONGNotUsed6,
					ULONGNotUsed7,
					ULONGNotUsed8,
					ULONGNotUsed9,
					Calls,							/*XNumber of calls														*/
					Uploads,						/*xNumber of files uploaded										*/
					BytesUpload,				/*xNumber of bytes uploaded										*/
					Downloads,					/*xNumber of files downloaded									*/
					BytesDownload,			/*xNumber of bytes downloaded									*/
					MessageWrite,				/*XNumber of messages written									*/
					MessageRead,				/*XNumber of messages readen									*/
					DailyByteLimit,			/*xDaily download byte limit 0 = DISABLED			*/
					DailyFileLimit,			/*xDaily download file limit 0 = DISABLED			*/
					DailyByteBonus,			/*xByte bonus for the current day							*/
					DailyFileBonus,			/*xFile bonus for the current day							*/
					DailyByteDl,				/*xNumber of bytes downloaded today						*/
					DailyFileDl,				/*xNumber of files downloaded today						*/
					LastConf,						/*XThe conf in which the user was last on			*/
					CnfReJoin,					/*XThe autojoin conference on logon selected	*/
															/* by the user. 0 = join into LastConf				*/
					BytesNuked,					/*xNuked bytes																*/
					FilesNuked,					/*xNuked files																*/
					NToNComFlag1,				/*XNode to Node communication flag 1					*/
					NToNComFlag2,				/*XNode to Node communication flag 2					*/
					NumberOfChats,			/*XNumber of chats														*/
					DayRelogins,				/*XNumber of relogins per day									*/
					ShellNumLines;			/*XNumber of lines in FAME - Shell						*/
long			LastTime,						/*XLast call																	*/
					TimeLimit,					/*XUsers time limit per day										*/
					TimeUsed,						/*XUsers time used this day										*/
					FirstCall,					/*XFirst call																	*/
					HighCPSDown,				/*XHighest download CPS												*/
					HighCPSUP,					/*XHighest upload CPS													*/
					BAUD,								/*XBaudrate on logon													*/
					TimeBonus,					/*xAdditional time after upload (daily)				*/
					ChatTime,						/* Chat time																	*/
					ChatTimeUsed,				/* Chat time used															*/
					LowCPSDown,					/*XLowest download CPS												*/
					LowCPSUP,						/*XLowest upload CPS													*/
					TimeTotal;					/*XOnline time total, actual counted					*/
ULONG			UserFlags1,					/*-User flags 1																*/
					UserFlags2;					/* User flags 2																*/
long			LongNotUsed1,
					LongNotUsed2;
short	int	DefaultProt,
					Deleted_Or_Not,			/*X0 = valid user, 1 = deleted, 2 = inaktiv		*/
					UploadProt,					/*XSelected upload protocol										*/
					Userlevel,					/*XUser level																	*/
					DownloadProt,				/*XSelected download protocol									*/
					Ratio,							/*xRatio. 0-n. 0 = disabled ratio							*/
					MsgUploadProt,			/*XSelected message upload protocol						*/
					RatioType,					/*xRatiotype. 0 = Byte 1 = B/F 2 = File				*/
					MsgDownloadProt,		/*XSelected message download protocol					*/
					Ansi_On_Off,				/*X0 = ansi off, 1 = ansi on, 2 = prompt			*/
					DoorUploadProt,			/*XSelected door upload protocol							*/
					NewScan;						/*X0 = no mail/file scan, 1 = mail/file scan,	*/
															/* 2 = mail scan, 3 = file scan, 4 = prompt		*/
long			AllHacks,						/* Number of hacks overall										*/
					LastHacks;					/* Last number of hacks. This can be reseted	*/
															/* and can be used to let FAME start some			*/
															/* actions defined by sysop, like inactivate	*/
															/* this user and more...											*/
short	int	DoorDownloadProt,		/*XSelected door download protocol						*/
					NumLines,						/*XNumber of lines														*/
					UserBlockSize,			/*XUsers selected xpr block size							*/
					CompiType,					/*XUsers selected computer type								*/
					FreeFromShort9,
					ModemType,					/*XUsers selected modem type									*/
					FreeFromShort10,
					Extension,					/*XUsers selected text extension							*/
					FreeFromShort11,
					Language,						/*xUsers selected language										*/
					FreeFromShort12,
					ZoomType,						/* Users selected zoom type										*/
					FreeFromShort13,
					FreeForUsage;
long			LostCarrier;					/* Number of lost carriers										*/
short	int	FreeFromShort14,
					FileIdDiz,					/* Off/auto/ask for fileid tools							*/
					FreeFromShort15,
					DoMsgCrypt;					/* Off/auto/ask for auto crypting messages		*/
long			NumberOfPages;			/* Number of pages overall										*/
USHORT		UserNumber,					/*XUser number																*/
					Editor,							/* 0 = line ed, 1 = full ed, 2 = external ed,	*/
															/* 3 = prompt for ed													*/
					DayPages;						/* Number of pages today											*/
};

/*
Usage of UserFlag1:             (UserFlag2 is unused!)

byte 0:
  bit0:		UD_SPY							// Capture user activities
  bit1:		UD_EXPERT						//X0 = menu text off else menue text on
  bit2:		UD_NEWUSER					//X0 = valid user else newuser
  bit3:		UD_MSGCLS						//XDo clear screens on messages
  bit4:		UD_FILECLS					//XDo clear screens on file lists
  bit5:		UD_UFLAG						//X0 = local upload else global upload
  bit6:		UD_DFLAG						//X0 = local download else global download
  bit7:		UD_UFBASE						// Allow UserFileBase
byte 1:
  bit0:		UD_HIDE							//XHidden for WHO etc...
  bit1:		UD_MSGROOM 					//XMessage rooming off/on
  bit2:		UD_STRED   					// String edit mode off/on
  bit3:		UD_SCRPAUSE					// Pause on screen numlines reached
  bit4:		UD_FSTGOODBYE				// Fast->No check for flags/partups/etc.
  bit5:		UD_VFLAG						// 0 = local else global file view
  bit6:		UD_ZFLAG						// 0 = local else global zippy search
  bit7:		UD_ASCIIMSG					// XASCII mode off/on on writing messages
byte 2:
  bit0:		UD_SWAPREDWHITE			// Red/white swap
  bit1:		UD_AUTOQUOTE				// Quote all automatic
  bit2:		UD_KEEPREPLYPRIV		// Keep private status on reply
  bit3:		UD_KEEPREPLYSUBJECT	// Keep message subject on reply
  bit4:		UD_FILECHECKPROCESS	// FileCheck process synchron or asynchron
  bit5:		UD_AUTOFORCEONWHO		// Automatic force on WHO
  bit6:		UD_BACKGROUNDPOST		// Background fileid extraction and file post
  bit7:		UD_FILENAMELENGTH		// AmigaDOS (30)/PC (12) file name length
byte 3:
  bit0:
  bit1:
  bit2:
  bit3:
  bit4:
  bit5:
  bit6:
  bit7:
*/

/*
Usage of NToNComFlag1:             (NToNComFlag2 is unused!)

byte 0:
  bit0:		NTN_DEFAULT
  bit1:		NTN_BBSHELP
  bit2:		NTN_BULLETINS
  bit3:		NTN_CHANGESTATS
  bit4:		NTN_ENTEREALL
  bit5:		NTN_FILELISTING
  bit6:		NTN_FLAGEDIT
  bit7:		NTN_JOINCONF
byte 1:
  bit0:		NTN_LOGGINGON
  bit1:		NTN_MULTICHAT
  bit2:		NTN_NEWFILES
  bit3:		NTN_PREPAREDOWNL
  bit4:		NTN_PREPAREUPL
  bit5:		NTN_READMAIL
  bit6:		NTN_SCANFILES
  bit7:		NTN_SCANMAIL
byte 2:
  bit0:		NTN_SEARCHFOR
  bit1:		NTN_SHELL
  bit2:		NTN_SYSOPCHAT
  bit3:		NTN_SYSOPCOMENT
  bit4:		NTN_VIEWFILES
  bit5:		NTN_VIEWTEXT
  bit6:		NTN_WAITFORKEY
  bit7:		NTN_WHO
byte 3:
  bit0:		NTN_WRITEMAIL
  bit1:
  bit2:
  bit3:
  bit4:
  bit5:
  bit6:
  bit7:
*/

struct	UserKeys {
char			UserName[32],
					StrNotUsed[32];
int				Deleted_Or_Not,			/*X0 = valid user, 1 = deleted, 2 = inaktiv		*/
					IntNotUsed;
USHORT		UserNumber;					/* User number																*/
BOOL			NewUser;
};

struct	UserConf {
char			UserName[32],				/* UserName																		*/
					ConfUserName[32],		/* UserName conf specific											*/
					UserSentLine[46],		/* User defined tag line for SentBy						*/
					StrNotUsed[98];
struct		CnfExpansion		*UserCnfExp;	/* Expansion pointer								*/
ULONG			ConfJoins,					/* Number of conference joins									*/
					Uploads,						/* Files uploaded in this conference					*/
					BytesUpload,				/* Bytes uploaded in this conference					*/
					Downloads,					/* Files downloaded in this conference				*/
					BytesDownload,			/* Bytes downloaded in this conference				*/
					MessageWrite,				/* Number of messages writen in this conf			*/
					MessageRead,				/* Number of messages readen in this conf			*/
					DailyByteLimit,			/* Daily download byte limit 0 = DISABLED			*/
					DailyFileLimit,			/* Daily download file limit 0 = DISABLED			*/
					DailyByteBonus,			/* Byte bonus for the current day							*/
					DailyFileBonus,			/* File bonus for the current day							*/
					DailyByteDl,				/* Number of bytes downloaded today						*/
					DailyFileDl,				/* Number of files downloaded today						*/
					BytesNuked,					/* Nuked bytes																*/
					FilesNuked,					/* Nuked files																*/
					ULongNotUsed1,
					ULongNotUsed2,
					ULongNotUsed3,
					ULongNotUsed4,
					ULongNotUsed5;
long			LastTime,						/* Last conference join												*/
					TimeLimit,					/* Time limit for this conference							*/
					TimeUsed,						/* Time used in this conference								*/
					FirstConf,					/* First conference join											*/
					HighCPSDown,				/*XHighest download CPS												*/
					HighCPSUP,					/*XHighest upload CPS													*/
					LastFileScan,				/* Last file scan															*/
					MsgRead,						/* Last message readen												*/
					EMsgRead,						/* Last EALL readen														*/
					MsgWritten,					/* Last message writen												*/
					MsgScanned,					/* Last new scan message readen								*/
					TimeBonus,					/* Additional time after upload (daily)				*/
					LowCPSDown,					/*Xlowest download CPS												*/
					LowCPSUP,						/*XLowest upload CPS													*/
					TimeTotal;					/* Online time total, actual counted					*/
ULONG			UserFlags1,					/* User flags 1																*/
					UserFlags2;					/* User flags 2																*/
long			LongNotUsed1,
					LongNotUsed2;
short int	NumOfConfPWFail,		/* Number of fails on entering the Conf PW.		*/
					Ratio,							/* Ratio. 0-n. 0 = disabled ratio							*/
					FreeFromShort2,
					RatioType,					/* Ratiotype. 0 = Byte 1 = B/F 2 = File				*/
															/* else GLOBAL ratio (user.data)							*/
					FreeFromShort3,
					ReplyMSGFlag,				/* Private,public/private,public/do private/	*/
															/* do public																	*/
					FreeFromShort4,
					IntNotUsed;
USHORT		UserNumber,					/* User number																*/
					UShortNotUsed;
};

/*
Usage of UserFlag1:             (UserFlag2 is unused!)

byte 0:
  bit0:		UC_NEWMSG						// 1 = scan for messages else no scan
  bit1:		UC_NEWFILES					// 1 = scan for files else no scan
  bit2:		UC_ZOOM							// Zooming off/on
  bit3:		UC_USERCNFXS				// 1 = user configurable access in conference
  bit4:		UC_OBSOLETE					// Obsolete
  bit5:		UC_USCNFDATA				// User conf datas or only user datas
  bit6:
  bit7:
byte 1:
  bit0:
  bit1:
  bit2:
  bit3:
  bit4:
  bit5:
  bit6:
  bit7:
byte 2:
  bit0:
  bit1:
  bit2:
  bit3:
  bit4:
  bit5:
  bit6:
  bit7:
byte 3:
  bit0:
  bit1:
  bit2:
  bit3:
  bit4:
  bit5:
  bit6:
  bit7:
*/

/*
void main()
{
printf("%d\n",sizeof(struct User));
printf("%d\n",sizeof(struct UserKeys));
printf("%d\n",sizeof(struct UserConf));
}
*/

#endif  /* FAME_FAMEUSERSTRUCTS_H */
