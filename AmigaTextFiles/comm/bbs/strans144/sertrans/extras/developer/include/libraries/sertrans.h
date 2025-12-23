#ifndef	SERTRANS_SERTRANS_H
#define	SERTRANS_SERTRANS_H

/*
**	$VER: sertrans.h V14 (02.07.96)
**
**	sertrans library definitions
**
**	(C) Copyright 1995-1996 Dirk Tietke
**	    All Rights Reserved
*/


struct SerTransPort {

     APTR	ST_Memory;	/* Memory includes all Information and     */
                                /* Config etc of SerTrans. This structure  */
                                /* is in moment still private              */
     APTR	Txt;		/* Pointer to the Texts. Please see below  */
                                /* for Positions of all Texts              */
     APTR	ST_DateTime;	/* Pointer to DateTime Struct.             */
     APTR	ST_dat_strday;	/* Pointer to the dat_xxxxx Strings, used  */
     APTR	ST_dat_strdate;	/* by DateToStr. Normally there should be  */
     APTR	ST_dat_strtime;	/* the aktual Day, Date and Time.          */
     STRPTR	PubName;	/* Pointer to the PublicScreeName what     */
                                /* SerTrans has. Normally it should be     */
                                /* 'SerTrans'!                             */
     APTR	ST_Config_Main;	/* Pointer to the Main Configuration       */
     APTR	ST_Config_Ext;	/* Pointer to the ExternalCmd Configs      */
     APTR	ST_Screen;	/* Pointer to SerTrans Screen              */
     APTR	ST_Window;	/* Pointer to SerTrans Window              */
     STRPTR	ProgrammName;	/* Pointer to the complete Path where 	   */
                                /* SerTrans is started from                */
     LONG	Private3;
     APTR	SerTransBase;	/* Pointer to Sertransbase                 */
     APTR	Aslbase;	/* Pointer to Aslbase                      */
     APTR	Diskfontbase;	/* Pointer to Diskfontbase                 */
     APTR	Gadtoolsbase;	/* Pointer to Gadtoolsbase                 */
     APTR	Workbenchbase;	/* Pointer to Workbenchbase                */
     APTR	Localebase;	/* Pointer to Localebase                   */
     APTR	Iconbase;	/* Pointer to Iconbase                     */
     APTR	Rexxsysbase;	/* Pointer to Rexxsysbase                  */
     APTR	ST_Task;	/* Pointer to SerTransTask                 */
     APTR	SerialStruct;	/* Pointer to the SerialStruct
     LONG	Private5;	/* Private  */
     UBYTE	Registered;	/* When the User of SerTrans is registered */
                                /* there will a stored a TRUE otherwise    */
                                /* a FALSE                                 */
     UBYTE	Connect;	/* Connect  = TRUE if User logged in       */
     UBYTE	ConLogin;	/* ConLogin = TRUE if Sysop is logged in   */
     UBYTE	Iconify;	/* Iconify  = TRUE if Console is closed    */
     STRPTR	RexxName;	/* Pointer to ArexxPortName                */
     STRPTR	PortName;	/* Pointer to PortName                     */
     STRPTR	PubName;	/* Pointer to PubScreenName                */
     STRPTR	WBScreen;	/* Pointer to WorkbenchName                */
     WORD	Version;	/* Version of SerTrans                     */
     WORD	Revision;	/* Revision of SerTrans                    */
     APTR	Coder0;		/* Here you can store your own Datas.      */
     APTR	Coder1;		/* i.e. The Pointer of the AreaStruct      */
     APTR	Coder2;		/* when you are coding a Module which      */
     APTR	Coder3;		/* is emulation a BBS with Areas.          */
     APTR	Coder4;		/* Also you can store here any other       */
     APTR	Coder5;		/* stuff. Maybe the Username of the        */
     APTR	Coder6;		/* User who's logged in...                 */
     APTR	Coder7;
     APTR	Coder8;
     APTR	Coder9;
     STRPTR	AssignName;	/* Name of Assign made by SerTrans. With   */
                                /* trailing ':' !                          */
};

struct	ExternalStruct {

     ULONG	ex_ID;			/* ID for Command see below        */
     char	ex_Name[20];		/* Name to Use                     */
     char	ex_PublicName[20];	/* Name to show User               */
     char	ex_ExecuteString[140];	/* Program to execute incl. Params */
     char	ex_Password[20];
     char	ex_Info[80];		/* i.e. Helptext ?!               */
     BOOL	ex_Visible;		/* Switch it in visible i.e. for  */
                                        /* UserLogin Module so they can't */
                                        /* execute a Login                */
     UBYTE	ex_Priority;		/* TaskPriority */
     BYTE	ex_Shortcut;		/* At least to enter */
     UBYTE	ex_Private;		/* HANDS OFF! */
};

;******************************** CommandDefinitions *************************/

#define	ID_COMMAND_OFF		0x434f4646;
#define	ID_EXT_PRG		0x45585450;
#define	ID_ARCHIVE_PRG		0x41524348;
#define	ID_SEND_AREXX		0x52455858;
#define	ID_SHOW_MEM		0x41564149;
#define	ID_CHANGE_DIR		0x43444952;
#define	ID_CLEAR_SCREEN		0x434c5253;
#define	ID_DELETE_FILE		0x44454c45;
#define	ID_CHAT			0x43484154;
#define	ID_SIMPLE_DIR		0x53444952;
#define	ID_EXECUTE		0x45584543;
#define	ID_HYDRA		0x48594452;
#define	ID_DRIVE_INFO		0x494e464f;
#define	ID_DOWNLOAD		0x4c4f4144;
#define	ID_VERBOSE_DIR		0x56444952;
#define	ID_LOGOFF		0x4c4f474f;
#define	ID_MAKEDIR		0x4d444952;
#define	ID_MARK			0x4d41524b;
#define	ID_PROTECT		0x50524f54;
#define	ID_RUN_COMMAND		0x52554e43;
#define	ID_UPLOAD		0x53454e44;
#define	ID_SETUP		0x53455455;
#define	ID_SEARCH		0x53454152;
#define	ID_TYPE			0x54595045;
#define	ID_VERSION		0x56455253;
#define	ID_CURSOR_CD		0x43555253;
#define	ID_PARENT		0x50415245;
#define	ID_HELP			0x48454c50;


struct	SerialStruct {
     
     char	ss_Reset[10];
     char	ss_HangUp[10];
     char	ss_Answer[10];
     char	ss_MakeBusy[10];
     char	ss_Device[20];
     WORD	ss_Unit;
     BOOL	ss_Shared;
     BYTE	ss_AutoAnswer;		/* 0 is OFF / 1-n are number of Rings */
     char	ss_Init[60];
     char	ss_FaxConnect[20];
     char	ss_FaxProgram[80];
     BOOL	ss_KindOfHangUp;
     UBYTE	ss_Private;
     char	ss_OK[20];
     char	ss_ERROR[20];
     char	ss_BUSY[20];
     char	ss_RING
     char	ss_VOICE[20];
     char	ss_NO CARRIER[20];
     char	ss_NO DIALTONE[20];
     char	ss_CONNECT[20];
     LONG	ss_Baud;
     LONG	ss_LamerBaud;		/* Should be less than Baud ;) */
};

/******************* This Structure is used by the UserModule ****************/

struct UserStruct {

     char	us_Username[20];
     char	us_PointName10];
     char	us_Vorname[20];
     char	us_Nachname[20];
     char	us_Strasse[25];
     char	us_Ort[25];
     char	us_Telefon[20];
     char	us_GeburtsTag[20];
     char	us_Computer[20];
     char	us_Modem[20];
     char	us_Login[20];
     char	us_Passwort[20];
     char	us_SysopInfo[80];
     ULONG	us_DownloadFree;
     UWORD	us_Ratio;
     ULONG	us_Account;
     UWORD	us_TimeLimit;
     ULONG	us_CoderData;		/* Here you can store your own Data */
     BYTE	Private1;
     BYTE	us_Packer;
     BYTE	us_Protocol;
     BYTE	us_ChatType;
     BYTE	us_Lines;		/* Lines for Paging */
     BYTE	Private2;
     WORD	Private3;
     LONG	us_FirstLogin;
     LONG	us_LastLogin;
     LONG	us_LastPWChange;
     LONG	us_Calls;
     LONG	us_Flags;		/* For Flags see below */
     LONG	us_UploadedBytes;
     LONG	us_DownloadedBytes;
     LONG	us_WrittenAscii;
     LONG	us_ReadAscii;
     LONG	us_Baudrate;
     LONG	us_LBoxCalls;
     WORD	us_RestTimeToday;
     WORD	us_TopUpCPS;
     WORD	us_TopDoCPS;
     BYTE	us_Emulation;
     BYTE	us_Tarifzone;
     BYTE	us_Ä;
     BYTE	us_ä;
     BYTE	us_Ö;
     BYTE	us_ö;
     BYTE	us_Ü;
     BYTE	us_ü;
     BYTE	us_ß;
     BYTE	us_Mode;		/* GeoNet, Rip... */
     BYTE	us_Language;
     BYTE	us_Empty;
     LONG	us_Bezugsdatum;
     CHAR	us_ReplyTo[30];

 /*sizeof UserStruct (430) */

};

/************************ BitDefinitions for Userflags ***********************/

#define UF_DIRECTION		(1L<<0)	/* Direction to Show new Entries */
#define UF_PRIVATE1		(1L<<1)
#define UF_BELL			(1L<<2)
#define UF_PAGING		(1L<<3)
#define UF_PRIVATE2		(1L<<4)
#define UF_STATUSLINE		(1L<<5)
#define UF_PRIVATE3		(1L<<6)
#define UF_REALATPM		(1L<<7)
#define UF_INVISIBLE		(1L<<8)
#define UF_CLS_AT_PAGING	(1L<<9)
#define UF_POINT_ONLY		(1L<<10)
#define UF_FULL_ADR_IN_HDR	(1L<<11)
#define UF_NO_PASSWORD		(1L<<28)
#define UF_GASTMODE		(1L<<29)
#define UF_LOGFILE		(1L<<30)
#define UF_LOCKED		(1L<<31)

/******************* This Structure is used for CallerID ******************/

/* New for V14 */

struct CallerList {

     char	cl_NameOfCaller[20];
     char	cl_CallerPhoneNumber[20];
     byte	cl_Status;
     UBYTE	cl_Private1;		/* Kill Flag */
     UBYTE	cl_Additional;		/* (1L<<0) Protocol */
                              		/* (1L<<1) Message */
     ULONG	cl_Private2;
     ULONG	cl_Private3;


#endif	/*SERTRANS_SERTRANS_H*/

