
/*****************************************************************************
 *                                                                           *
 *                                    _      _ ______________________ __     *
 *     _____    __________ _____\__  (_)                              \//    *
 *   _|     \.__\_       //      \/_______                             ø     *
 *   \_      |   /      /.\       \      /         Tristar And        /      *
 *    |______|   \_______|________/______\       Red Sector Inc.             *
 *        /_______|-Mo!\aL-                      FAME-Innovation    /        *
 *                                                                           *
 *                 Tempest to FAME UserData Converter v1.00                  *
 *                             Used tab size: 2                              *
 *                                                                           *
 *****************************************************************************/

#include <stdio.h>
#include <strings.h>
#include <proto/exec.h>
#include <FAME/FAMEPublic.h>

char *VerStr = {"$VER: T2F 1.0 "__AMIGADATE__};
long AccessFile( char InStr[] );

/*
 * Tempest Userdata structure
 */

struct TUser
{
 UWORD   Slot_Number;            // (  2) Account#
 UBYTE   Status;                 // (  1) 0=Deleted, 1=Active, 2=New User, 3=Locked Out, 4=Created Account

 TEXT    Name[31],               // ( 31) Users Name
         RealName[26],           // ( 26) Real Name
         VoicePhone[21],         // ( 21) Voice Phone Number
         DataPhone[21],          // ( 21) Data  Phone Number
         Address[26],            // ( 26) Address
         City[26],               // ( 26) City
         State[4],               // (  4) State/Providence Code
         Country[4],             // (  4) Country Code
         MailRoute[15],          // ( 15) Zip Code / Mail Route
         ComputerType[21],       // ( 21) Computer Type (Possible Code Instead)
         Password[16],           // ( 16) Password
         Comment[31];            // ( 31) Comment String

 UBYTE   Security;               // (  1) Access Level
 UWORD   WeedDays,               // (  2) Days of inactivity until bbs auto deletes
         ReduceDays;             // (  2) Days Paid for by a payer.                        (NOT USED YET)
 UBYTE   ReducedAccess;          // (  1) Preset level to give to user after reduced days  (NOT USED YET)

 UBYTE   AnsiType,               // (  1) Ansi Type  0) Normal, 1) Amiga, 2) IBM, 3) RIP
         Protocol,               // (  1) Protocol Selected: 0:Zmodem 1:Hydra
         DEADProto,              // (  1) Not Used Anymore
         HelpLevel,              // (  1) Level of Help
         MSGHeader,              // (  1) Msg Header Type
         EditorLines,            // (  1) Max Lines allowed in Editor      (NOT USED YET)
         DescLines,              // (  1) File Listing lines, Descriptions
         ViewMode,               // (  1) File Listing View Mode
         WhoPrefs,               // (  1) Who Preferences
         MB_J,                   // (  1) Message Rejoin
         FB_J,                   // (  1) File rejoin
         Length,                 // (  1) Length of screen
         Width,                  // (  1) Width of screen
         Language,               // (  1)
         Translation,            // (  1) (NOT USED YET)
         TimeZone,               // (  1) Time Zone Offset (NOT USED YET)
         BirthDate_Day,          // (  1) DAY of birth
         BirthDate_Month,        // (  1) MONTH of birth
         BirthDate_Year,         // (  1) YEAR of birth
         IdleTime,               // (  1) Idle time allowed for no activity (NOT USED YET)
         NodeLastOn;             // (  1) Last node user was on

 UWORD   ForwardMailSlot,        // (  2) Slot Number of user to forward mail to! (NOT USED YET)
         PWF;                    // (  2) Number of Password Failures

 UWORD   Total_Calls,            // (  2) # of Calls Total
         Total_Posts,            // (  2) Msgs left total
         Total_PostsReceived,    // (  2) Msgs Received
         Total_ULFiles,          // (  2) # of Uploads
         Total_DLFiles,          // (  2) # of Downloads
         Total_TimeBank;         // (  2) Time Bank Value
 ULONG   Total_ULBytes,          // (  4) Total Uploaded Bytes
         Total_DLBytes;          // (  4) Total Downloaded Bytes

 UWORD   Limit_Calls,            // (  2) Calls ALLOWED per Day
         Limit_Posts,            // (  2) Messages Allowed p/d
         Limit_ULFiles,          // (  2) # of Uploads Per Period
         Limit_DLFiles,          // (  2) # of Downloads Per Period
         Limit_Time,             // (  2) Time Limit per day.
         Limit_TimeBank,         // (  2)
         Limit_ChatTime;         // (  2) Limit of Time user can be in chat door
 ULONG   Limit_ULBytes,          // (  4) Limit Upload Bytes Per Period
         Limit_DLBytes;          // (  4) Limit Download Bytes Per Period

 UBYTE   F_Ratio,                // (  1) File Ratio
         B_Ratio;                // (  1) Byte Ratio

 UBYTE   Period_Type;            // (  1) Mins, Hours, Days, Weeks, Months, Years    (NOT USED YET)
 UWORD   Period_Length,          // (  2) 24 hours or whatever! Set Limits by Period (NOT USED YET)
         Period_Calls,           // (  2) Calls made  Today
         Period_Posts,           // (  2) Todays Msgs Sent
         Period_ULFiles,         // (  2) Todays Ul's
         Period_DLFiles,         // (  2) Todays Dl's
         Period_ChatTime;        // (  2) Time user has been in chat this period
 ULONG   Period_ULBytes,         // (  4) Period Bytes Uploaded
         Period_DLBytes;         // (  4) Period Bytes Downloaded

 UWORD   Left_DLFiles,           // (  2) Files left available for download
         Left_ChatTime,          // (  2) Limit of Time user can be in chat door
         Left_Time;              // (  2) Time Left
 ULONG   Left_DLBytes;           // (  4) Bytes left for this period

 UWORD   DayCredits,             // (  2) For Payers
         FileCredits;            // (  2) Files Paid for by a payer.

 ULONG   ByteCredits,            // (  4) Bytes Paid for by a payer.
         PersonalBytes,          // (  4) Personal Area Bytes Limit
         LastBaudRate;           // (  4) Last Baud Rate of a user.

 LONG    Time_First_Called,      // (  4) Time First Logged On
         Time_Last_Logoff,       // (  4) Last Time LOGGED OFF
         Time_Last_Connect;      // (  4) Time last CONNECTED

LONGBITS UserBitsA,              // (  4)
         UserBitsB,              // (  4)
         UtilBits,               // (  4) 32 bits for Programmers to use as they wish for utils.
         LastCallFlags,          // (  4) Flags for activities during last call. Posted,UL,DL,etc (NOT USED YET)
         CatFlags,               // (  4) Flags for Custom File Listing (NOT USED YET)
         NFlags1,                // (  4) Normal Flags Part 1
         NFlags2,                // (  4) Normal Flags Part 2
         SFlags1,                // (  4) Sysop  Flags Part 1
         SFlags2;                // (  4) Sysop  Flags Part 2

 char    MsgBase[41],            // (1x41=41) MB 0-39 Flags N,Y,S   // Change to 2 LONGBITS
         FileBase[41];           // (1x41=41) FB 0-39 Flags N,Y,S   // one for normal, one for sysop

 UWORD   Last_Read_Message[40],  // (40x2=80) -position change- Last read pointer
         Last_Scan_Message[40];  // (40x2=80)

 BYTE    AnsiColor;              // (  1) Ansi Color
 BYTE    BExtras[29];            // (1x29=29)

 UWORD   CheckSum,               // (  2)
         SerialNumber;           // (  2) Unique seiral number for flagged files, etc. (NOT USED YET)

 SHORT   SExtras[3];             // (3x2=6)
 LONG    LExtras[3];             // (3x4=12)
};

/*
 * FAME:Settings/System.dat structure
 */

struct sYSTEM
{
	char			BBSName[42],						/* The BBS Name														*/
						BBSGroup[32],						/* The BBS group or location							*/
						SysOpName[32],					/* The sysop name													*/
						BBSLocationPath[102],		/* The location of FAME: normaly FAME:		*/
						FileDescULine[46],			/* FileDicriptionLine if									*/
																		/* FileNumbers = TRUE											*/
						UserDataName[12],				/* The name of the User.data file					*/
						UserKeysName[12],				/* The name of the User.keys file					*/
						UserDaKePath[102],			/* Path for User.data and User.keys				*/
						FileCryptPW[12],				/* File-Crypt-Password										*/
						EmergencyPW[12],				/* Emergency password											*/
						ASLLocalUlPath[22],			/* ASL Local Upload Path									*/
						ASLTextViewPath[22],		/* ASL Text View Path											*/
						ASLSendFilePath[22],		/* ASL Send File Path											*/
						ASLLocalDlPath[22],			/* ASL Local Download Path								*/
						StrNotUsed4[98];
	ULONG			UserLimit;							/* Max. users allowed											*/
	long			NumberOfConfs,					/* Number of conferences									*/
						FGPSBitPlanes,
						FGPSFontSize;
	ULONG			FGPSScreenID,
						ULongNotUsed4,
						ULongNotUsed5,
						ULongNotUsed6;
	short int	FGPSAutoScroll,
						ShortIntNotUsed1;
	long			NumberNodes,						/* Number of nodes												*/
						NewUserLevel,						/* NewUser level													*/
						FAMEVers,
						BullTxtMinNumLines,			/* Bull.txt min. number of lines					*/
						BullTxtMinBrkLvl,				/* Bull.txt min. number of lines					*/
						DefaultNumLines,				/* Default number of lines								*/
						MaxUserPWFail,					/* Max. number of false paswords					*/
						MaxSysPWFail,						/* Max. number of false paswords					*/
						MaxNupPWFail,						/* Max. number of false paswords					*/
						MaxNameFail,						/* Max. number of false names							*/
						MaxUserHacks,						/* Max. number of hackings								*/
						CaptureBuffer,					/* Capturebuffer													*/
						UPWFailAction,					/* nothing/EnterMsgtoSysOp/								*/
																		/* EnterMsgto(Co)SysOps->UEALL Level			*/
						UPWFailALevel,					/* The level of UEALL											*/
						SpyWindowTop,						/* Top of Spywindow												*/
						SpyWindowLines,					/* Number of lines for Spywindow					*/
						MaxCopyBuffer,					/* Maximum copy buffer for filecopies			*/
						MinFreeMB;							/* Minimum free MB for upload volumes			*/
	short int	UserFileBase,						/* UserFileBereiche allow									*/
						FileNumbers,						/* Filenumbers in file listings						*/
						DateBytes,							/* Date and bytes in file listings				*/
						RightFileNames,					/* right formated file namenes in file l.	*/
						MsgCryptPW,							/* MsgKryptPW in MsgHeader Y/N						*/
						FileCryptPWFC,					/* FileKryptPW to file/comment Y/N				*/
						TextWHO,								/* MCI based WHO Y/N											*/
						MultiNodeUser,					/* The same user on different nodes Y/N		*/
						DoFileInfos,						/* FileName/Filesize/Uploader/Downloads/	*/
																		/* ULDate/ULTime/Xfer-Time/CPS/Baud/			*/
																		/* ULNode/Downloads/FileIdDiz Y,N/				*/
																		/* File_id_DIZ														*/
						ShowBull,								/* Show or show not Bull.txt							*/
						SpyKeyIn,								/* Allow key input from Spywindow					*/
						SpyWindowLeft,
						SpyWindowSizeGad,
						SpyWindowWidth,
						AutoRestrict,						/* Auto restriction of FAME data files		*/
						MaxFidLines,
						UploadHandle,
						InactiveWins,
						ConfAccounting;

/* FGPS and additional memory only in FAME 2.0 available.,

						FGPSDefPublicScreen;
	char			FGPSScreenName[32],
						FGPSFontName[32],
						FGPSOtherPubScreen[102],
						FGPSFallBackPubScreen[102],
						StrNotUsed1[2004];
*/

};

struct FAMELibrary 	*FAMEBase = NULL;
struct RDArgs 			*rda 			= NULL;
extern char					*_ProgramName;

char 	ArgStr[]			= "TEMPESTUSERDATA/A";
long	ArgArray[]		= { 0L };

struct sYSTEM 			FAMESystem;
struct User					FAMEUser;
struct UserKeys			FAMEUserKeys;
struct TUser				TempestUser;

char	TempestUsDa[256],
			FAMEUsDa[256],
			FAMEUsKe[256],
			WorkBuffer[256];

void main( void )
{
	if( FAMEBase = (struct FAMELibrary *)OpenLibrary( FAMENAME, 4L ))
	{
		Printf( "\n[32mtRSi presents T2F - Tempest to FAME User Data converter "__AMIGADATE__"[31m\n\nAn  installed  FAME  including  FAME:   assign  must  be available to use that\nconverter.  Then start This tool with the argument <PathToTheTempestUserDatas>\n\nExample usage:  T2F Tempest:aCCOUNTS.dATA\n\n" );
		Printf( "T2F  will  then locate the FAME user data location itself and generate the new\nUser.data and User.Keys files there.  If old User Data files are existing they\nwill be renamed to <OldFileName>.RenamedByT2F.\n\nBe  sure that you delete all UserCnf.data files in your Conferences of FAME so\n" );
		Printf( "the  FAME  Server is able to create the new ones there.  Or just quit T2F now,\ndelete/rename  the  User.Data  and  User.Keys  files of your FAME BBS and then\nstart  the  Server  which  will then ask and delete all UserCnf.data files for\nyou. Then restart T2F.\n\n" );

    TempestUsDa[ 0 ] = '\0';

		if( rda = ReadArgs( ArgStr, ArgArray, NULL ))
		{
			if( ArgArray[ 0 ] ) strcpy( TempestUsDa, (STRPTR)ArgArray[ 0 ] );
			FreeArgs( rda );

  	 	if( TempestUsDa[ 0 ] && ! AccessFile( TempestUsDa ))
			{
				BPTR SysDatFH = NULL;

				if( SysDatFH = Open( "FAME:Settings/System.dat", MODE_OLDFILE ))
				{
					BPTR	TempestUsDaFH	= NULL,
								FAMEUsDaFH		= NULL,
								FAMEUsKeFH		= NULL;

					Seek( SysDatFH, 61, OFFSET_BEGINNING );
					Read( SysDatFH, &FAMESystem, sizeof( struct sYSTEM ));

					strcpy( FAMEUsDa, 	FAMESystem.UserDaKePath );
					AddPart( FAMEUsDa, 	FAMESystem.UserDataName, 255 );

					strcpy( FAMEUsKe, 	FAMESystem.UserDaKePath );
					AddPart( FAMEUsKe, 	FAMESystem.UserKeysName, 255 );

					Close( SysDatFH );

         	if( ! AccessFile( FAMEUsDa ))
					{
						strcpy( WorkBuffer, FAMEUsDa );
						strcat( WorkBuffer, ".RenamedByT2F" );

						Printf( "INFO: Renaming %s to %s !!!\n", FAMEUsDa, WorkBuffer );

						DeleteFile( WorkBuffer );

						Rename( FAMEUsDa, WorkBuffer );
					}

					if( ! AccessFile( FAMEUsKe ))
					{
						strcpy( WorkBuffer, FAMEUsKe );
						strcat( WorkBuffer, ".RenamedByT2F" );

						Printf( "INFO: Renaming %s to %s !!!\n", FAMEUsKe, WorkBuffer );

						DeleteFile( WorkBuffer );

						Rename( FAMEUsKe, WorkBuffer );
					}

          if( TempestUsDaFH  = Open( TempestUsDa, MODE_OLDFILE ))
					{

          	if( FAMEUsDaFH   = Open( FAMEUsDa, MODE_NEWFILE ))
						{
							/*
							 * Write header.
							 */

							Write( FAMEUsDaFH, "FAMEUSDA$VER:FAME v0.75 beta (31.10.97)                     ", 61 );

							if( FAMEUsKeFH = Open( FAMEUsKe, MODE_NEWFILE ))
							{
								ULONG UserSlot	= 0;
								WORD	whichchar	= 1;

								/*
							 	 * Write header.
							 	 */

								Write( FAMEUsKeFH, "FAMEUSKE$VER:FAME v0.75 beta (31.10.97)                     ", 61 );

								/*
								 * Read Tempest User Datas and convert them all.
								 */

								Printf( "\n" );

      					while( sizeof( struct TUser ) == Read( TempestUsDaFH, &TempestUser, sizeof( struct TUser )))
								{

									Printf( "Now converting...  " );

									switch( whichchar )
									{
										case -1:	break;
										case  1:  Printf( "[D|" );
															break;
										case  2:  Printf( "[D/" );
															break;
										case  3:  Printf( "[D-" );
                 	            break;
										default:  Printf( "[D\\" );
															whichchar = 0;
															break;
   	              }

									Flush( Output( ));
									whichchar++;

									/*
								 	 * New UserSlot/UserNumber creating.
									 */

									UserSlot++;

									/*
									 * Start converting now
									 */

          				/*
									 * Defaulting FAME user datas.
									 *
									 * All not here defaulted and converted datas are already 0 - NULL
									 * by using FAMEMemSet below and that's ok and default!
									 */

                  FAMEMemSet( &FAMEUser,			'\0', sizeof( struct User ));
									FAMEMemSet( &FAMEUserKeys,	'\0', sizeof( struct UserKeys ));

									FAMEUser.UserFlags1					|= UD_MSGCLS;
									FAMEUser.UserFlags1					|= UD_MSGROOM;
									FAMEUser.UserFlags1					|= UD_STRED;
									FAMEUser.UserFlags1					|= UD_SCRPAUSE;
									FAMEUser.UserFlags1					&= ~UD_SWAPREDWHITE;
									strcpy( FAMEUser.ConfAccess, "X___________________" );

									FAMEUser.Ansi_On_Off				= 2;
									FAMEUser.CnfReJoin          = 0UL;
									FAMEUser.RatioType					= 1;
									FAMEUser.LastConf						= 1UL;
									FAMEUser.Editor							= 3;

									FAMEUser.NewScan						= 4;
									FAMEUser.CompiType					= 1;
									FAMEUser.ModemType					= 1;
									FAMEUser.Extension					= 1;
									FAMEUser.Language						= 1;
									FAMEUser.FileIdDiz					= 2;

									/*
								   * Converting T -> F.
								   */

									FAMEUser.UserNumber					= UserSlot;
									FAMEUserKeys.UserNumber			= UserSlot;

									switch( TempestUser.Status )
									{
										case 0: // Deleted

										FAMEUser.Deleted_Or_Not 		= 1;
										FAMEUserKeys.Deleted_Or_Not	= 1;
										break;

										case 1: // Active

										FAMEUser.Deleted_Or_Not			= 0;
										FAMEUserKeys.Deleted_Or_Not	= 0;
										break;

										case 2: // New User
										case 4: // Created

										FAMEUser.UserFlags1					|= UD_NEWUSER;
										FAMEUser.Deleted_Or_Not			= 0;
										FAMEUserKeys.NewUser				= TRUE;
										FAMEUserKeys.Deleted_Or_Not	= 0;
										break;

										case 3: // LockedOut
										default:

										FAMEUser.Deleted_Or_Not			= 2;
										FAMEUserKeys.Deleted_Or_Not	= 2;
										break;
									}

									strcpy( FAMEUser.UserName, 			TempestUser.Name );
									strcpy( FAMEUserKeys.UserName, 	TempestUser.Name );

									FAMEStrCopy( TempestUser.DataPhone, FAMEUser.UserPhone, 15 );

									strcpy( FAMEUser.UserLocation, 	TempestUser.City );
									strcpy( FAMEUser.UserFrom, 			TempestUser.State );
									strcat( FAMEUser.UserFrom, 			" / " );
									strcat( FAMEUser.UserFrom, 			TempestUser.Country );
									strcpy( FAMEUser.Password, 			TempestUser.Password );

									FAMEUser.Userlevel 			= TempestUser.Security;
									FAMEUser.NumLines				= TempestUser. Length;

									if( TempestUser.BirthDate_Day != 0 && TempestUser.BirthDate_Month != 0 && TempestUser.BirthDate_Year != 0 )
									{
                  	sprintf( FAMEUser.Birthday, "%2.2d-%2.2d-%2.2d\0", TempestUser.BirthDate_Day,TempestUser.BirthDate_Month,TempestUser.BirthDate_Year );
									}

                  FAMEUser.AllHacks   		= TempestUser.PWF;
									FAMEUser.Calls					= TempestUser.Total_Calls;
									FAMEUser.MessageWrite		= TempestUser.Total_Posts;
                  FAMEUser.MessageRead		= TempestUser.Total_PostsReceived;

									FAMEUser.Uploads				= TempestUser.Total_ULFiles;
									FAMEUser.Downloads			= TempestUser.Total_DLFiles;

									FAMEUser.BytesUpload		= TempestUser.Total_ULBytes;
									FAMEUser.BytesDownload	= TempestUser.Total_DLBytes;

	       					FAMEUser.DailyFileLimit	= TempestUser.Limit_DLFiles;
									FAMEUser.TimeLimit			= TempestUser.Limit_Time;
									FAMEUser.ChatTime       = TempestUser.Limit_ChatTime;
									FAMEUser.DailyByteLimit	= TempestUser.Limit_DLBytes;
									FAMEUser.Ratio					= TempestUser.B_Ratio;

									FAMEUser.BAUD           = TempestUser.LastBaudRate;
                  FAMEUser.FirstCall			= TempestUser.Time_First_Called;

                 	Write( FAMEUsDaFH, &FAMEUser, sizeof( struct User ));
									Write( FAMEUsKeFH, &FAMEUserKeys, sizeof( struct UserKeys ));

   	 							Printf( " Slot %4.4lu( %-20.20s ) converted, done!\n[1F", UserSlot,TempestUser.Name );
								}
              	Close( FAMEUsKeFH );
  						}
							else
							{
								Printf( "\nError creating FAME User.Keys !!!" );
							}
							Close( FAMEUsDaFH );
						}
						else
						{
							Printf( "\nError creating FAME User.Data !!!" );
						}
						Close( TempestUsDaFH );
          }
					else
					{
						Printf( "\nError opening Tempest User Datas !!!" );
					}
				}
				else
				{
					Printf( "\nFAME:Settings/System.dat not found !!!" );
				}
			}
		 	else
			{
				Printf( "\nTempest User Data not found ( file: %s ) !!!",TempestUsDa );
			}
		}
		else
		{
    	Printf( "\nNo Arguments given !!!" );
		}
		CloseLibrary( (struct Library *)FAMEBase );
	}
	else
	{
		Printf( "\nError opening FAME.library v4 !!!" );
	}
	Printf( "\n\n" );
}

long AccessFile( char InStr[] )
{
	BPTR lock = NULL;

	if( lock = Lock( InStr, SHARED_LOCK ))
	{
		UnLock( lock );

		return( 0 );
	}
	else if( IoErr( ) == ERROR_OBJECT_IN_USE )
	{
		return( 0 );
	}

	return( 1 );
}
