
/* FAME Public Structs
 *
 * $VER:FAMEPublicStructs.h v1.3
 *
 * Used tab size: 2
 */

#ifndef FAME_FAMEPUBLICSTRUCTS_H
#define FAME_FAMEPUBLICSTRUCTS_H

/* DoorPort Structure (FIM) Fame Interface Module Type, version 1.
 */

struct	FAMEDoorMsg {
struct	Message	fdom_Msg;						/* MessagePort Structure (System)				*/
char						fdom_IOString[202];	/* In/OutPut String											*/
STRPTR					fdom_StringPtr;			/* string pointer												*/
long						fdom_Command,				/* Command Node<->Door									*/
								fdom_Data1,					/* Data 1 to transfer data							*/
								fdom_Data2;					/* Data 2 to transfer data							*/
ULONG						fdom_Data3;					/* Data 3 to transfer data							*/
long						fdom_ReturnCode,		/* Returncode														*/
								fdom_Node;					/* NodeNumber														*/
ULONG						fdom_InternalBits;	/* FAME internal data bit flags					*/
APTR						fdom_StructDummy1,	/* APTR 1 for SystemStructs							*/
								fdom_StructDummy2,	/* APTR 2 for SystemStructs							*/
								fdom_StructDummy3;	/* APTR 3 for SystemStructs							*/
STRPTR					fdom_StringPtr2;		/* A second string pointer							*/
ULONG						fdom_Data4,					/* Data 4 to transfer data (future use)	*/
								fdom_BitFlags;			/* A bit mask to give the door infos		*/
struct	MsgPort	*fdom_ExternalPort;	/* External AbortIO port								*/
};

/*
Usage of fdom_BitFlags:

byte 0:
  bit0:		FBF_OLMSAVAIL							// New olm(s) available
  bit1:
  bit2:
  bit3:
  bit4:
  bit5:
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


Usage of fdom_InternalBits:

byte 0:
  bit0:		FIBF_NODOORDROP						// Forbid the Drop Door possibility
  bit1:
  bit2:
  bit3:
  bit4:
  bit5:
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

/* Definition of the ReturnCode:

ReturnCode=5      Comand aborted
ReturnCode=4      Comand not implemented untill now
ReturnCode=3      Comand denied
ReturnCode=2      Comand doesn't exists
ReturnCode=1      Comand not successfull executed
ReturnCode=0      Comand successfull executed
ReturnCode=-1     Abort of the Door requested   -> Door must end now!
ReturnCode=-2     Carrier lost                  -> Door must end now!
ReturnCode=-3     Unknown error                 -> Door must end now!

Defines out of FAMEDefine.h:

 /* FIM DoorPort return codes. */

#define	FIM_USERERROR								10
#define	FIM_CMDABORTED							5
#define	FIM_CMDNOTIMPLEMENTED				4
#define	FIM_CMDDENIED								3
#define	FIM_CMDDOESNOTEXISTS				2
#define	FIM_CMDNOTSUCCESSFULL				1
#define	FIM_CMDSUCCESSFULL					0
#define	FIM_DOORABORTREQUESTED			-1
#define	FIM_CARRIERLOST							-2
#define	FIM_UNKNOWNERROR						-3

On negative Returncodes the Door *MUST* close *IMMEDIATELY* with the last
command: MC_ShutDown !!!

ReturnCode -1 means the SysOp or an internal procedure of FAME demands an
abort of the Door.
The SysOp can do this with the IconifyWindow for example. If he clicks on
the upper left closegadget on the nodescreen this IconifyWindow appears.
If he now clicks on "DropUser or Exit Node/Door" the Doors receives at the
first time the ReturnCode -1 and has to call MC_ShutDown and must close down.
If the Door doesn't note this, maybe it's a wrong Interface Module Type
(XIM/REXX/...) and the SysOp clicks the second time on this gadget the
Door will be kicked and the Node goes back to the Menu or previous Door.
The same can also be done under FAME on XIM/REXXDOOR/REXXEXEC and Paragon
Doors! (FIM/XIM/AIM/CIM/TIM)


#define AFO_FAMEDoorMsg 0            // FAMEDoorMsg structure

Note: Allocations via FAME.library:

      struct  FAMEDoorMsg *MyFAMEDoorMsg = FAMEAllocObject(FOBJ_FAMEDoorMsg);

      and deallocations via FAME.library:

      FAMEFreeObject(FOBJ_FAMEDoorMsg,MyFAMEDoorMsg);

      APTR FAMEAllocObject(ULONG);
      VOID FAMEFreeObject(ULONG,APTR);

*/


/* External CoProcess Port Structure, version 0.
 *
 * NOTE: FAMEToolPort and CoProcess stuff isn't available yet!!!
 *       Do not use it!!!
 */

struct	FAMECoProcessMsg {
struct	Message	fcpm_Msg;						/* MessagePort Structure (System)				*/
ULONG						fcpm_Registration;	/* To identify this structure						*/
STRPTR					fcpm_CoProcName,		/* The name of the CoProc								*/
																		/* (NewBroker like)											*/
								fcpm_CoProcTilte,		/* The title of the CoProc							*/
																		/* (NewBroker like)											*/
								fcpm_CoProcDesc,		/* The description of the CoProc				*/
																		/* (NewBroker like)											*/
								fcpm_CoProcAction;	/* Text msg showen on Server						*/
long						fcpm_Command,				/* The command to the Server						*/
								fcpm_Data1,					/* long based datafield 1								*/
								fcpm_Data2,					/* long based datafield 2								*/
								fcpm_Node,					/* NodeNumber for CoProc usage (ident.)	*/
								fcpm_ReturnCode;		/* Returncode from Server								*/
ULONG						fcpm_CoProNum,			/* CoProcessNumber											*/
								fcpm_Data3,					/* unsigned long based datafield 3			*/
								fcpm_Data4,					/* unsigned long based datafield 4			*/
								fcpm_UnUsed,
								fcpm_BitFlags,			/* A bit mask for future use						*/
								fcpm_IdentifyCode;	/* To identify the CoProcess						*/
STRPTR					fcpm_StringPtr;			/* string pointer												*/
APTR						fcpm_StructDummy1;	/* typeless ptr for structs and more		*/
};



/* Messagebase mail header structure, version 0.
 */

struct	FAMEMailHeader {
char		fmah_ToName[32],			/* To user name  Name/ALL/EALL/UEALL					*/
				fmah_FromName[32],		/* From user name															*/
				fmah_Subject[32],			/* Subject																		*/
				fmah_ToNet[12],				/* From Net																		*/
				fmah_FromNet[12],			/* To Net																			*/
				fmah_CryptPW[12],			/* KryptPW																		*/
				fmah_StrNotUsed1[12],
				fmah_StrNotUsed2[12],
				fmah_StrNotUsed3[12];
long		fmah_MsgNum,					/* Message number															*/
				fmah_MsgDate,					/* Message date																*/
				fmah_Crypt,						/* Crypted Y/N 0=no														*/
				fmah_UFromLevel,			/* Level from writer													*/
				fmah_Lines,						/* Number of lines for read testing						*/
															/* if more than 100 lines...									*/
				fmah_AktMsgNum,				/* Needed for SYSCMD: MAILHEADER							*/
				fmah_LongNotUsed2,
				fmah_Priority,				/* Priority																		*/
				fmah_DeleteInDays,		/* Delete in days															*/
				fmah_Received;				/* Times read																	*/
ULONG		fmah_ULongNotUsed1,
				fmah_ULongNotUsed2,
				fmah_ULongNotUsed3,
				fmah_ULongNotUsed4;
BOOL		fmah_MsgStatus,				/* State: deleted? 1=valid 0=deleted					*/
				fmah_AFileStat,				/* Attached file(s)?													*/
				fmah_NotUsed,
				fmah_Private,					/* Private msg?																*/
				fmah_ToNameRec;				/* Received by fmah_ToName?										*/
SHORT		fmah_HeadNumLines,		/* Number of lines skipping because of header */
				fmah_ShortNotUsed2,
				fmah_ShortNotUsed3,
				fmah_ShortNotUsed4,
				fmah_ShortNotUsed5;
};



/* MsgBase/Editor list structure
 */

struct	MsgList {
struct	MsgList	*Next,
								*Prev;
char						LineText[100];
long						LineNumber;
};



/* External editor structure, version 1.
 */

struct	FAMEExternEditor {
struct	FAMEMailHeader	*feed_EdMailHeader;	/* Pointer to filled structure	*/
struct	MsgList					*feed_MsgListStamm;	/* Replylist										*/
STRPTR									feed_FilePath;			/* MsgBasePath									*/
long										feed_MsgType,				/* 0 External file, 1 Enter new	*/
																						/* Msg, 2 Reply Msg, 3 Part Msg	*/
																						/* 4 OLM, 5 Edit Msg,						*/
																						/* 6 Script Mode								*/
												feed_Data,					/* Dummy Max lines on MsgType=4	*/
												feed_StartLine,			/* StartLine, if 0 then no			*/
																						/* lines to reply!							*/
												feed_EndLine;				/* EndLine, if 0 then no				*/
																						/* lines to reply!												*/
};

/* feed_MsgType is also the returncode:
 *
 *  0 = Ok.
 *  1 = MainPart shall upload file(s) to this message.
 * -2 = Mail aborted.
 */



/* Messagebase mail status structure, version 0.
 */

struct	FAMEMailStat {
long		fmas_HighMsgNum;
};



/* CoProcess structure. This one is needed to tell the Server of your
 * CoProcess requests.
 *
 * NOTE: FAMEToolPort and CoProcess stuff isn't available yet!!!
 *       Do not use it!!!
 */

struct	FAMECoProcess {
char		fcop_CoPrStartPath[32],		/* (Path)/Name for the CoProcess						*/
				fcop_CoPrArgs[32];				/* Args for CoProcess												*/
LONG		fcop_StartDate1,					/* Start at 1. Date													*/
				fcop_StartDate2,					/* Start at 2. Date													*/
				fcop_StartDate3,					/* Start at 3. Date													*/
				fcop_StartDate4,					/* Start at 4. Date													*/
				fcop_StartDate5,					/* Start at 5. Date													*/
				fcop_StartDate6,					/* Start at 6. Date													*/
				fcop_StartDate7,					/* Start at 7. Date													*/
				fcop_StartDate8,					/* Start at 8. Date													*/
				fcop_StartDate9,					/* Start at 9. Date													*/
				fcop_StartDate10;					/* Start at 10. Date												*/
ULONG		fcop_Flag1,								/* Flags 1																	*/
				fcop_Reserved;						/* Reserved!																*/
};

/*
Usage of Flag1:             (Flag1 is unused!)

byte 0:
  bit0:															// Activated y/n
  bit1:
  bit2:
  bit3:
  bit4:
  bit5:
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




/* ToolPort message structure to let the Server receive requests from tools,
 * version 0.
 *
 * NOTE: FAMEToolPort isn't available yet!!! Do not use it!!!
 */

struct	FAMEToolPortMsg {
struct	Message	ftpm_Msg;						/* MessagePort Structure (System)				*/
ULONG						ftpm_Registration;	/* To identify this structure						*/
STRPTR					ftpm_ToolName,			/* The name of the Tool									*/
																		/* (NewBroker like)											*/
								ftpm_ToolTilte,			/* The title of the Tool								*/
																		/* (NewBroker like)											*/
								ftpm_ToolDesc,			/* The description of the Tool					*/
																		/* (NewBroker like)											*/
								ftpm_ToolAction;		/* Text msg shown on Server							*/
long						ftpm_Command,				/* The command to the Server Tool Port	*/
								ftpm_Data1,					/* long based datafield 1								*/
								ftpm_Data2,					/* long based datafield 2								*/
								ftpm_ReturnCode,		/* Returncode from Server								*/
								ftpm_Node;					/* NodeNummer for Tool usage (identify)	*/
ULONG						ftpm_Data3,					/* unsigned long based datafield 3			*/
								ftpm_Data4,					/* unsigned long based datafield 4			*/
								ftpm_UnUsed,
								ftpm_BitFlags,			/* A bit mask for future use						*/
								ftpm_BitRequests1,	/* A bit mask for requests 1						*/
								ftpm_BitRequests2,	/* A bit mask for requests 2						*/
								ftpm_BitRequests3,	/* A bit mask for requests 3						*/
								ftpm_BitRequests4,	/* A bit mask for requests 4						*/
								ftpm_IdentifyCode;	/* To identify the Tool. DO NOT TOUCH !	*/
STRPTR					ftpm_StringPtr1,		/* string pointer 1											*/
								ftpm_StringPtr2;		/* string pointer 2											*/
APTR						ftpm_StructDummy1,	/* typeless ptr for structs and more 1	*/
								ftpm_StructDummy2;	/* typeless ptr for structs and more 2	*/
};

/* Returncodes:
 *               9 You are already registered.
 *               8 You have to use FAMETP_Register first.
 *               7 ftpm_IdentifyCode not valid.
 *               6 ftpm_ToolName not given.
 *               5 Comand aborted.
 *               4 Comand not implemented yet.
 *               3 Comand denied.
 *               2 Comand doesn't exists.
 *               1 Comand not successfull executed.
 *               0 Comand successfull executed.
 *              -1 Fatal error. Remove your tool immediately.
 *              -2 You are not the Tool you are trying to be.
 *              -3 Unknown error.
 *              -4 Server has closed the FAMEToolPort.
 *                 Message direct reply.
 */

/* General words for the usage of the ToolPort.
 *
 * First every new Tool must register before it can do anything.
 * To register the fields ftpm_ToolName, ftpm_ToolTilte and ftpm_ToolDesc
 * have to be filled (point to strings defined in your tool space).
 * ftpm_ToolName is a *must*, without no entry is allowed. If ftpm_ToolTilte
 * and ftpm_ToolDesc are not set the Server will point them to ftpm_ToolName.
 * Now you have to set the ftpm_Command to FAMETP_Register (define = 0), later
 * if you no longer need the ToolPort you have to use FAMETP_EndRegister define.
 * You also have to fill ftpm_Node if your Tool may run multiple at one time
 * if you fill it out you will get this number back over the ToolNotifyPort
 * you have to apply. You have always to allocate the structure via
 * FAMEAllocObject from FAME.library. Now you can send this message to the
 * Server and if everything went ok you will get in ftpm_IdentifyCode
 * your code for identifying which you *must* use from now on everytime
 * you send any request over this ToolPort to the Server. Means for the case
 * if you want to send more than one message to the ToolPort at one time where
 * you need to allocate with FAMEAllocObject new messages. It would be wise
 * to copy your first (registering) message into the new allocated one.
 * The Server will also give this code to you if you receive a message over
 * your ToolNotifyPort. Also if you use FAMETP_EndRegister to end your tool
 * you have to set ftpm_IdentifyCode to your received code, but the best is
 * to let the field ftpm_IdentifyCode like it is.
 * The Server will give and remove your ftpm_IdentifyCode itself.
 *
 * Note: Be sure that all the time you use the ToolPort you *must* leave
 *       ftpm_ToolName and ftpm_IdentifyCode untouched!!!
 *
 * ftpm_ToolName is the simple name of the tool, no version, no coder name
 * and no other stuff here! Add only the Node number or your process
 * address or something similar if your tool can be started multiple.
 *   (max. 40 chars long, try to use only 20 chars, because of displaying)
 * ftpm_ToolTilte is the title like tool name, version and coder name.
 *   (max. 40 chars long, try to use only 20 chars, because of displaying)
 * ftpm_ToolDesc is the description fo the tool, means what it does.
 *   (max. 40 chars long, try to use only 20 chars, because of displaying)
 * ftpm_ToolAction is the actual action/state of the tool similar
 * to Servers Action field on display.
 *   (max. 40 chars long, try to use only 20 chars, because of displaying)
 *
 *
 * NOTE: FAMEToolPort isn't available yet!!! Do not use it!!!
 *
 * #define	FAMETP_Register						0
 * #define	FAMETP_EndRegister				1
 * #define	FAMETP_InfoStructs				2
 * #define	FAMETP_FDCMP							3
 * #define	FAMETP_ToolAction					4
 */



/* Tool notify message structure to tell the tools using ToolPort of actions
 *
 * NOTE: FAMEToolPort isn't available yet!!! Do not use it!!!
 */

struct	FAMEToolNotifyMsg {
struct	Message	ftnm_Msg;						/* MessagePort Structure (System)				*/
long						ftnm_Notify,				/* The notify define										*/
								ftnm_Data1,					/* long based datafield 1								*/
								ftnm_ReturnCode,		/* Returncode from Tool									*/
								ftnm_Node;					/* NodeNummer for Tool usage (identify)	*/
ULONG						ftnm_Data2,					/* unsigned long based datafield 2			*/
								ftnm_UnUsed,
								ftnm_BitFlags,			/* A bit mask for future use						*/
								ftnm_BitRequests1,	/* A bit mask for requests 1						*/
								ftnm_BitRequests2,	/* A bit mask for requests 2						*/
								ftnm_BitRequests3,	/* A bit mask for requests 3						*/
								ftnm_BitRequests4,	/* A bit mask for requests 4						*/
								ftnm_IdentifyCode;	/* To identify the Tool									*/
STRPTR					ftnm_StringPtr;			/* string pointer												*/
APTR						ftnm_StructDummy1;	/* typeless ptr for structs and more		*/
};

/* Returncodes you can set for the Server:
 *                                         0 Everything ok, delete request
 *                                         1 Everything ok, hold request
 *
 * NOTE: FAMEToolPort isn't available yet!!! Do not use it!!!
 */



/* Number flag file list structure
 */

struct	NumFlagList {
struct	NumFlagList	*Next,
										*Prev;
char								FileName[102];
long								FileNumber;
ULONG								BitFlags;
};

/*
Usage of BitFlags:

byte 0:
  bit0:		FNFL_OVERWRITE
  bit1:
  bit2:
  bit3:
  bit4:
  bit5:
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



/* Structure for a list of nodes for some Doorcommands and more
 */

struct	FAMEDestNodes {
struct	FAMEDestNodes	*fndl_Next;
long									fndl_NodeNum;
};



/* FAME Info List structure for FAMESemaphore.
 */

struct	FAMEInfoList {
struct	FAMEInfoList	*fili_Next,				/* Ptr. to next struct FAMEInfoList	*/
											*fili_Prev;				/* Ptr. to prev struct FAMEInfoList	*/
struct	User					*fili_NodeRUser,	/* Ptr. to Nodes read user datas		*/
											*fili_NodeWUser;	/* Ptr. to Nodes write user datas		*/
struct	ConfList			*fili_ConfStamm,	/* Origin ptr. of the conf. list		*/
											*fili_FAMECnfLst;	/* Ptr. to the current conf. datas	*/
STRPTR								fili_SVUserName,	/* UserName (Server string)					*/
																				/* Note: Max. 22 bytes in size !		*/
											fili_SVLocation,	/* UserLocation (Server string)			*/
																				/* Note: Max. 22 bytes in size !		*/
											fili_SVAction,		/* Node action string (Server str)	*/
																				/* Note: Max. 22 bytes in size !		*/
											fili_VerStr,			/* Node version string							*/
																				/* Note: Max. 32 bytes in size !		*/
											fili_VersionStr,	/* Complete Node version string			*/
																				/* Note: Max. 62 bytes in size !		*/
											fili_ComputerType,/* Computer type as string					*/
																				/* Note: Max. 36 bytes in size !		*/
											fili_ModemType,		/* Modem type as string							*/
																				/* Note: Max. 36 bytes in size !		*/
											fili_Extension,		/* Extension for text files					*/
																				/* Note: Max. 12 bytes in size !		*/
											fili_Language,		/* Users selected language					*/
																				/* Note: Max. 32 bytes in size !		*/
											fili_ConnectMsg,	/* Connect message string						*/
																				/* Note: Max. 82 bytes in size !		*/
											fili_XferProt,		/* Transfer protocol name						*/
																				/* Note: Max. 32 bytes in size !		*/
											fili_FileUpload,	/* Name of file uploading/ed				*/
																				/* Note: Max. 32 bytes in size !		*/
											fili_FileDownload,/* Name if file downloading/ed			*/
																				/* Note: Max. 32 bytes in size !		*/
											fili_SerialDriver,/* Serial device driver name				*/
																				/* Note: Max. 102 bytes in size !		*/
					 						fili_PubScrName,	/* Node PublicScreen name						*/
																				/* Note: Max. 22 bytes in size !		*/
											fili_FiFoName,		/* Node FiFo name										*/
																				/* Note: Max. 22 bytes in size !		*/
											fili_ReservedUser,/* Name of reserved user						*/
																				/* Note: Max. 32 bytes in size !		*/
											fili_ASLDlPath,		/* ASL requester download path			*/
																				/* Note: Max. 102 bytes in size !		*/
											fili_ASLULPath,		/* ASL requester upload path				*/
																				/* Note: Max. 102 bytes in size !		*/
											fili_ASLTxtViewP;	/* ASL requester text view path			*/
																				/* Note: Max. 102 bytes in size !		*/
long									*fili_WhatLogon,	/* Logon type (defined)       (ptr) */
											*fili_InitBaud,		/* Baud rate on initializing  (ptr) */
											*fili_BaudRate,		/* Baud rate with no CPS      (ptr) */
											*fili_UploadCPS,	/* Upload CPS rate            (ptr) */
											*fili_DownloadCPS,/* Download CPS rate          (ptr) */
											*fili_TimeConnect,/* Time connected to the Node (ptr) */
											*fili_NumConfs;		/* Number of conferences      (ptr) */
SHORT									*fili_RelativCnfs,/* 0 = real else relative     (ptr) */
											*fili_ChatFlag;		/* 0 = not available else available	*/
long									fili_SVNodeNumber,/* The number of this Node					*/
											fili_SVBaudCps,		/* BaudRate or CPS (Transfer)				*/
											fili_SVActionCmd,	/* Defined action number						*/
											fili_DataFileVer,	/* Data file version								*/
											fili_DataFileRev,	/* Data file revision								*/
											fili_Version,			/* Nodes version										*/
											fili_Revision,		/* Nodes revision										*/
											fili_StartedTime;	/* Nodes start time									*/
SHORT									fili_SVNodeOnline,/* 0 = not started else online			*/
											fili_UserOnline;	/* 0=Await 1=Connect 2=Valid 3=Reset*/
ULONG									fili_BitFlags,		/* Bit flags for different things		*/
											fili_Reserved[20];/* Reserved fileds for future usage	*/
};

/* Note: The STRPTR sizes are including the zero-byte!
 *       Most sizes are two bytes longer as the real used text string,
 *       because of the zero-byte we need one byte more, but to align the
 *       sizes to word size there will always calculated with one byte more.
 *       This has the result that because of the zero-byte two bytes more
 *       are allocated instead only one byte. FAME mostly uses not the second
 *       byte, but it's ok if you fill the string completely up.
 *
 *       Example: fili_Extension has 12 bytes, 10 bytes max. string size
 *                the 11th byte is for the zero-byte and the 12th is not used,
 *                but it's not dangerous if the 12th byte is the zero-byte
 *                and the other 11 are used for the string.
 */

/*
Usage of fili_BitFlags:

byte 0:
  bit0:		FILI_USERSAVELOCK					// Lock user data saving on logout
  bit1:		FILI_QUIETNODE						// Node is in quiet mode.
  bit2:		FILI_USERHIDE							// User is hidden.
  bit3:		FILI_USERREALHIDE					// User is real hidden.
  bit4:
  bit5:
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

/* Note: fili_SV kinds are equal to the Server datas used to show informations
 *       on and/or about the Server.
 *
 * Note: This is a information structure, please do not write into the data
 *       elements! This is all READ-ONLY, else you might confuse or crash
 *       the system. In some cases it's possible to write into the dats,
 *       but please use for strings/string pointers (here STRPTR) Forbid()
 *       and Permit() around the string copy, else you can overwrite a
 *       zero-byte while the node or an other tool reads the same string.
 *       If you write please do not write more than the in the comment given
 *       string sizes - 1 byte for the zero-byte!!!
 *       Normally tools can't read/write strings at the same time as others
 *       do because of the Semaphore, but door commands and MainPart can.
 *       The STRPTRs are string pointers to MainPart's config data structure
 *       and this structure is not in a Semaphore!
 *       Because of this use also Forbid() and Permit() to read from a
 *       string/string pointer (STRPTR) or read only the in the comment given
 *       size limit (strncpy) and set the zero-byte on using strncpy!
 *
 * Note: fili_NodeRUser, fili_NodeWUser and fili_BitFlags are exceptions
 *       from the next note above. fili_NodeRUser and fili_NodeWUser are
 *       pointers to the both Node's user data structures of an online user.
 *       They are valid as long as a node is running, also if no user is
 *       online. You may ask why two structures. The first fili_NodeRUser
 *       is the read data structure and the other the structure which will
 *       be written back to disk if a user is loging off. MainPart reads
 *       and uses all datas out of fili_NodeRUser and writes datas into both
 *       structures, so if you use the pointers to change online user datas
 *       please do it in the same way:
 *
 *       fili_NodeRUser: To read and write datas.
 *       fili_NodeWUser: To write datas only.
 *
 *       Read example : If you want to know the username or level for
 *                      validations use ONLY "fili_NodeRUser" !
 *
 *       Write example: If you want to change the username or level of the
 *                      user use both "fili_NodeRUser" and "fili_NodeWUser" !
 *
 *       The reason for handling this in the above explained way is the
 *       internal user edior which allows "SysOp Access" what means the
 *       SysOp is able to give any online user a higher access which is
 *       only available as long as the user is actual online or the Sysop
 *       removes it before he is logging off.
 *       To realize this fili_NodeRUser is used to save the temporär changed
 *       datas on "SysOp access". fili_NodeWUser contains at this point not
 *       complete equal datas to fili_NodeRUser. fili_NodeRUser won't be
 *       saved on logoff, but the everytime "real" fili_NodeWUser.
 *
 * Note: Please read or change string contends in fili_NodeRUser and
 *       fili_NodeWUser only between Forbid() and Permit() !!!
 *
 * Note: fili_BitFlags first bit is used with "FILI_USERSAVELOCK".
 *       FILI_USERSAVELOCK is used to lock the back writings of the
 *       fili_NodeWUser user datas to disk. If you need to change user datas
 *       of an online user please use FILI_USERSAVELOCK to lock the
 *       user data to disk back writings of the node. But only lock it
 *       as short as you can, else it will disturb the node.
 *       The locking is needed, because if you change he datas while a
 *       user is logging off it can be that your changings will be made to
 *       late and they get lost.
 *       The node checks every second if it wants to write the user datas
 *       to disk if the bit is clear. After 60 tries (1 minute) it will
 *       clear the bit and writes the datas back to have no endless hanging
 *       nodes.
 *
 *       Please don't lock to long!!!
 *
 * Note: You also need to be sure that the node is online, else
 *       fili_NodeRUser and fili_NodeWUser are NULL !
 *       Use fili_SVNodeOnline for this.
 *
 * Note: You also need to be sure that a user is online, else your changing
 *       will take no effect. Use fili_SVUserOnline for this.
 *
 * Example for using fili_NodeRUser and fili_NodeWUser:
 *
 *	void ChangeDatas(struct FAMEInfoList *MyFAMEInfoList)
 *	{
 *		//-> Pointer valid ?
 *
 *		if(MyFAMEInfoList) {
 *
 *			//-> Is the Node online ?
 *
 *			if(MyFAMEInfoList -> fili_SVNodeOnline) {
 *
 *				//-> Is a user online ?
 *
 *				if(MyFAMEInfoList -> fili_SVUserOnline == 2) {
 *
 *					//-> Lock Node's user data savings:
 *					//->
 *					//-> "|=" is in asm: "BSET"
 *
 *					MyFAMEInfoList -> fili_BitFlags |= FILI_USERSAVELOCK;
 *
 *					if(MyFAMEInfoList -> fili_NodeRUser && MyFAMEInfoList -> fili_NodeWUser) {
 *
 *						char			UserName[32];
 *						short int	Userlevel;
 *
 *						//-> Read datas:
 *
 *						Forbid();
 *						strcpy(UserName,MyFAMEInfoList -> fili_NodeRUser . UserName);
 *						Permit();
 *
 *						Userlevel = MyFAMEInfoList -> fili_NodeRUser . Userlevel;
 *
 *						//-> Change datas:
 *
 *						Forbid();
 *						strcpy(MyFAMEInfoList -> fili_NodeRUser . UserName,UserName);
 *						strcpy(MyFAMEInfoList -> fili_NodeWUser . UserName,UserName);
 *						Permit();
 *
 *						MyFAMEInfoList -> fili_NodeRUser . Userlevel = Userlevel;
 *						MyFAMEInfoList -> fili_NodeWUser . Userlevel = Userlevel;
 *
 *					}
 *
 *					//-> Unlock Node's user data savings:
 *					//->
 *					//-> "&= ~" is in asm: "BCLR"
 *
 *					MyFAMEInfoList -> fili_BitFlags &= ~FILI_USERSAVELOCK;
 *
 *				}
 *			}
 *		}
 *	}
 *
 *  Source Note: It is also good to use Forbid and Permit around the check
 *               for a online node + user and locking with FILI_USERSAVELOCK.
 *               I have left this out here to keep the source small.
 *               Watch out for a complete example source also with
 *               getting the semaphore i will include to the FAME
 *               developer stuff.
 *
 *  Source Note: If you do any action like prompting for inputs or everything
 *               else between readings and writings please try to unlock
 *               and re-lock later. This could cause that the node writes
 *               and logges of a user. That's of course a problem if you
 *               want to write something back... you can do two things:
 *
 *               First : Check before lock and write if the user is still
 *                       online and lock! If the user is no longer online
 *                       load his account directly from the user datas and
 *                       change the datas and write them directly back.
 *               Second: The best way is to prompt first and after that
 *                       to read, change and write the datas back.
 */



/* FAMESemaphore with info datas for everyones usage.
 */

struct	FAMESemaphore {
struct	SignalSemaphore	fsem_Semaphore;			/* The Signal Semaphore					*/
struct	FAMEInfoList		*fsem_FirstNode;		/* Ptr. to first FAMEInfoList		*/
																						/* entry, in every case Node 0	*/
STRPTR									fsem_SVBBSLocPath,	/* BBS Location Path						*/
												fsem_SVBBSName,			/* BBS Name											*/
												fsem_SVBBSOrigin,		/* BBS Origin/Group							*/
												fsem_SVSysOpName;		/* SysOp Name										*/
long										fsem_SVNumOfNodes,	/* The number of nodes in list	*/
												fsem_SVVersion,			/* Servers version							*/
												fsem_SVRevision,		/* Servers revision							*/
												fsem_SVStartedTime;	/* Servers start time						*/
ULONG										fsem_Reserved[20];	/* Reserved fileds for future		*/
};

/* Note: fsem_SV kinds are equal to the Server datas used to show informations
 *       on and/or about the Server.
 */



/* OLM structure of dynamic allocated list entries to be as result the
 * complete OLM text.
 *
 * Note: You must free fols_OLMessage with FreeVec first before you
 *       free any list entry!!!
 *
 * Why a dynamic allocation? Because after the guide lines noone should
 * send more than 1kb and ideally no more than 256 characters per write
 * to the console. So you can have more shorter strings. MainPart needs
 * this also because of lists from editors used to write olms.
 *
 * NOTE: This OLM stuff isn't implemented yet!!! Do not use it!!!
 */

struct	FAMEOLMString {
struct	FAMEOLMString	*fols_Next,					/* Pointer to the Next list entry	*/
											*fols_Prev;					/* Pointer to the Prev list entry	*/
STRPTR								fols_OLMessage;			/* OLM message text. Note, it has	*/
																					/* been dynamic allocated via			*/
																					/* AllocVec and so you have to		*/
																					/* deallocate it via FreeVec			*/
																					/* before you begin to free one		*/
																					/* of the list entries!!!					*/
};

/* OLM structure to send online messages.
 *
 * Note: You must free the list beginning with folm_First first completely
 *       before you free any list entry!!!
 *       Note also the note for the structure FAMEOLMString!!!
 *
 * NOTE: This OLM stuff isn't implemented yet!!! Do not use it!!!
 */


// files an olms???

struct	FAMEOnlineMsg {
struct	FAMEOnlineMsg	*folm_Next,					/* Pointer to the Next list entry	*/
											*folm_Prev;					/* Pointer to the Prev list entry	*/
struct	FAMEOLMString	*folm_FirstOLMStr;	/* The first entry of the dynamic	*/
																					/* allocated OLM text list				*/
char									folm_Sender[32],		/* The name of the user/tool			*/
																					/* who/which has send							*/
																					/* this olm												*/
											folm_Command[202];	/* Maybe a menu command, FAMEChat	*/
long									folm_NodeNr;				/* The node number of the sender	*/
ULONG									folm_BitFlags;			/* Bit flags for different things	*/
long									folm_Time,					/* The time the olm was sent			*/
											folm_Date;					/* The date the olm was sent			*/
};

/* OLD:
STRPTR								folm_OLMessage;			// OLM message text. Note, it has
																					// been dynamic allocated via
																					// AllocVec and so you have to
																					// deallocate it via FreeVec!
 */

/* folm_BitFlags: - Reply j/n. (LIST WORK ABORT)
 *                - Std. Header aus den struct Daten j/n (OLM from user x on node y)
 *
 * NOTE: This OLM stuff isn't implemented yet!!! Do not use it!!!
 */

/* (LIST WORK ABORT) bedeutet Empfänger verläßt Abarbeitungsfkt. der OLM liste
                     um z.B. zu replyen. Da beim reply oder im Chat neue
                     OLMs kommen können muß das sein!
 */

/* ToDo: MainPart      : - struct FAMEOnlineMsg *Ptr in FAMEConfig welcher den
                           Stamm der OLMS bildet.
         Listfunktionen: - FreeCompleteFOLMStr() liste der struct FAMEOLMString
                           komplett freigeben.
                         - FreeCompleteFOLM() liste der struct FAMEOnlineMsg
                           komplett freigeben mit check ob folm_First TRUE,
                           wenn ja dann an FreeCompleteFOLMStr() übergeben.
                         - SendOLM() um teilweise gefüllte struct FAMEOnlineMsg
                           an eine Node zu senden. Teilweise weil NodNr zB.
                           oder UserName kann diese Funktion selber machen.
                         - Fkt. die struct FAMEOnlineMsg an Stamm/Liste hängt.
                         - Fkt. die struct FAMEOLMString an Stamm/Liste hängt.
                         - NToN include anpassen, entweder an Liste oder OLM
                           gleich ausgeben und freigeben.

Senden: TempStammPtr der struct FAMEOLMString allozieren und ersten Eintrag
        Text allozieren und reinhängen... so weiter verfahren bist text
        komplett. OLMStammPtr der struct FAMEOnlineMsg allozieren den
        TempStammPtr reinsetzen restlichen Einträge ausfüllen und zur Node
        senden.

Empfangen: Wenn user gleich bereit zum Empfang ist dann an folm_First hängende
           liste ausgeben, ansonsten an stamm/liste der wartenen OLMS hängen.
           Wenn user bereit wird dann checken ob der *Ptr in FAMEConfig
           NULL ist, wenn nicht liste durchlaufen und texte ausgeben.
           Dazu in einer whileschleife laufen solange wie *Ptr nicht NULL
           und fkt. anspringen die erstes listenelement abhandelt, danach
           frei gibt und liste vorrückt. In der Zeit darf nix von den ports
           gelesen werden! Also abgeschloßene Fkt.! Besagte Fkt. gibt dann
           returncode zurück, ob die whileschliefe vorzeitig unterbrochen
           werden soll. Wodurch das ausgelöst wird: folm_BitFlags!
           Sobald irgendwas gemacht werden muß was die signalbits abfragt
           muß das außerhalb der schleife gemacht werden! Und da nach
           jeder angezeigten OLM zumindest ein eigener prompt zum anhalten
           gemacht werden sollte, muß die Schleife immer nach einer einzigen
           OLM verlassen werden! Das while dient also nur mehr zur Einsprungs-
           entscheidung. Mann kann also auch genauso if(*Ptr) machen.
 */

#endif  /* FAME_FAMEPUBLICSTRUCTS_H */
