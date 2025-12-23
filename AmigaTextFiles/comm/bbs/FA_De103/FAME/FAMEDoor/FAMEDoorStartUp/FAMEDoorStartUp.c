
/*
 * $VER: FAME DoorStartUp 1.3
 *
 * DoorStartup for FIM Doors - can be compiled resident.
 *
 */

/*
 * Includes:
 */

#include <FAME/FAMEDoorProto.h>
#include <libraries/fame.h>

/*
 * FIM Defines:
 */

#define	FIM_MINBUFSIZE				1024L
#define	NC_CHECK							0
#define	NC_NOCHECK						1
#define	FE_QUIET							1
#define	FE_NOTQUIET						0

/*
 * Global variables:
 */

struct MsgPort			*DoorControlPort	= NULL,
										*DoorReplyPort		= NULL;
struct FAMEDoorMsg	*MyFAMEDoorMsg		= NULL;

char					FAMEDoorPort[30],
							*GlobBuffer							= NULL,
							ErrorText[200];
unsigned long	Signals,
							PortSig									= 0UL,
							GlobFIMFlags						= 0UL;

#ifdef FIM_USERSIGMODE

unsigned long	GlobUserSigs						= 0UL;

#endif

/*
 * Prototypes:
 */

long				FIMStart( long BufSize, ULONG FIMFlags );
long				FIMHandlePort( long FIMCommand, ULONG UserSigs, short int NoErrCheck );
void				FIMEnd( long ReturnCode );
STATIC void	FIMFree( long ReturnCode,short int Quiet );
STATIC void	FIMPrintError( long Error );

/*
 * Prototypes for Support-Functions
 */

long GetString( char*, long );
long PutString( char*, long );
long NC_PutString( char*, long );
long PutStringFormat( char *ctl, ... );
long NC_PutStringFormat( char *ctl, ... );
long PutCommand( char*, long, long, ULONG, long );
long GetCommand( char*, long, long, ULONG, long );

void Center( char *stri, long lf, long col );
void SPrintf( char *Buffer, char *ctl, ... );
long FAMEAtol( char *Buffer );

/*
 * FUNCTION: long FIMStart( long BufSize, ULONG FIMFlags );
 *
 *  PURPOSE: Allocates Memory for DoorStruct and start Doorport communication
 *
 *    INPUT: BufSize = Memory used for Global String Pointer, 1024 bytes is
 *                     the minimum amount for this pointer.
 *           FIMFlags = NULL or FIMF_<...> flags (defined in FAMEDefine.h).
 *                      FIMF_USERSIGMODE to enable the user signal check.
 *                      FIMF_NOCHECK to disable the error check of FIM
 *                      commands send to the door.
 *
 *   RETURN:  0 = If no error occures during initializing.
 *           -1 = No memory for Global String Pointer
 *           -2 = No memory for FAMEDoorMsg
 *           -3 = DoorReplyPort couldn't be created
 *           -4 = Communication with FAME failed
 *
 * See FIMHandlePort() for more informations about errorcode -4!
 *
 * If any error occure you may repeat the initialization process as all vars
 * used by this function are freed !
 */

long FIMStart( long BufSize, ULONG FIMFlags )
{
	/*
	 * Make the FIMFlags global:
	 */

	GlobFIMFlags = FIMFlags;

	/*
	 * Clear the buffer:
	 */

	*ErrorText = '\0';

	/*
	 * BufSize must be >= FIM_MINBUFSIZE (1024 bytes):
	 */

	if(BufSize < FIM_MINBUFSIZE)
		BufSize = FIM_MINBUFSIZE;

	/*
	 * Allocate the GlobBuffer:
	 */

	if( ! (GlobBuffer = AllocVec(BufSize,MEMF_PUBLIC | MEMF_CLEAR)))
	{
		FIMFree( FIM_DOORABORTREQUESTED, FE_QUIET );
		return(-1);
	}

	/*
	 * Use the FAME.library to allocate the FAMEDoorMsg structure:
	 */

	if( ! (MyFAMEDoorMsg = (struct FAMEDoorMsg *)FAMEAllocObject(FOBJ_FAMEDoorMsg)))
	{
		FIMFree( FIM_DOORABORTREQUESTED, FE_QUIET );
		return(-2);
	}

	/*
	 * We need a reply port:
	 */

  if( ! (DoorReplyPort = CreatePort(NULL,0L)))
	{
		FIMFree( FIM_DOORABORTREQUESTED, FE_QUIET );
		return(-3);
	}

	/*
	 * Rember the signal of our reply port:
	 */

	PortSig = 1<<DoorReplyPort->mp_SigBit;

	/*
	 * Fill some message things and set the MC_DoorStart command:
	 *
	 * Note: mn_Node.ln_Type = NT_MESSAGE isn't needed, because exec's
	 *       PutMsg() will do it anyway (See AutoDoc: exec.library/PutMsg).
	 */

	MyFAMEDoorMsg->fdom_Msg.mn_Length			= sizeof(struct FAMEDoorMsg);
	MyFAMEDoorMsg->fdom_Msg.mn_ReplyPort	= DoorReplyPort;
	MyFAMEDoorMsg->fdom_Command						= MC_DoorStart; /* Register Door */

	/*
	 * Let's call FIMHandlePort() to register with the Node:
	 */

	if(FIMHandlePort(0, NULL, NC_CHECK ) < 0)
	{
		FIMFree( FIM_DOORABORTREQUESTED, FE_NOTQUIET );
		return(-4);
	}

	/*
	 * From now on the user can use FIMHandlePort() or the support functions
	 * to communicate with the Node till FIMEnd() is called.
	 *
	 * Return to the Door:
	 */

	return(0);

} // long FIMStart( long BufSize, ULONG FIMFlags );

/*
 * FUNCTION: void FIMEnd( long ReturnCode );
 *
 *  PURPOSE: Closes DoorPort Communication and also calls user supplied
 *           function 'ShutDown()' to allow the deallocation of all memory
 *           used by the door.
 *
 *    INPUT: Userdefined ReturnCode. Allowed range for usererrors are from
 *           (FIM_USERERROR + 1) to (ERROR_NO_FREE_STORE - 1), which means
 *           the range from 11 to 102. All values > 102 are for the DOS
 *           error codes and will be used to print the according error as
 *           localized errorstring (See include/dos/dos.h for further infos
 *           about DOS error codes).
 *
 *   RETURN: Nothing.
 */

void FIMEnd( long ReturnCode )
{
	/*
	 * Is there any problem with the Door?
	 */

	if(ReturnCode)
  {
		/*
		 * User (Door) related problem ?
		 */

		if(ReturnCode > FIM_USERERROR && ReturnCode < ERROR_NO_FREE_STORE)
		{
			SPrintf(ErrorText,"\n\r[37mDoor failed with returncode [35m%ld [m!\n\n\r",ReturnCode);
		}
		else
		{
			Fault(ReturnCode,"\n\r",ErrorText,199);
			strcat(ErrorText,"\n\n\r");
		}
  }

	/*
	 * Free the resources of the FAMEDoorStartUp code:
	 */

	FIMFree( FIM_USERERROR, FE_NOTQUIET );

} // void FIMEnd( long ReturnCode );

/*
 * FUNCTION: STATIC void FIMFree( long ReturnCode, short int Quiet );
 *           !!! PRIVATE !!!
 *
 *  PURPOSE: Really shutdown the Doorport and free all allocated memory.
 *           This function will call the user supplied function 'ShutDown()'
 *           after freeing all allocated memory!
 *
 *    INPUT: - Returncode which indicates if there is any doorport error.
 *             They are defined in FAMEDefine.h under FIM_<...>.
 *           - Quiet. If FE_QUIET no MC_ShutDown command will be send
 *             (it's for the case where the port isn't set up completely).
 *
 *   RETURN: Nothing.
 *
 *     NOTE: THIS FUNCTION SHOULD NOT BE USED FROM DOORS, AS THIS WILL ONLY
 *           BE CALLED HERE INSIDE THIS DOORSTARTUP-CODE !!
 */

STATIC void FIMFree( long ReturnCode, short int Quiet )
{
	/*
	 * Only if the Door is registered with the Node we can send a MC_ShutDown
	 * command to it:
	 */

	if(Quiet == FE_NOTQUIET)
	{
		/*
		 * Positive return code? If we will print a reason else just ShutDown:
		 */

		if(ReturnCode > 0)
		{
			MyFAMEDoorMsg->fdom_Command = MC_ShutDownLastWords;
			strcpy(MyFAMEDoorMsg->fdom_IOString,ErrorText);
		}
		else
		{
			MyFAMEDoorMsg->fdom_Command = MC_ShutDown;
		}

		/*
		 * Place the request:
		 */

		FIMHandlePort(0,NULL, NC_CHECK );
	}

	/*
	 * Empty the reply port and delete it.
	 * Don't forget to NULL the pointer.
	 */

  if(DoorReplyPort)
	{
		while(GetMsg(DoorReplyPort));

		DeletePort(DoorReplyPort);
	}
	DoorReplyPort = NULL;

	/*
	 * Free the FAMEDoorMsg structure via FAME.ibrary.
	 * Don't forget to NULL the pointer.
	 */

	if(MyFAMEDoorMsg)
	{
		FAMEFreeObject(MyFAMEDoorMsg);
	}
	MyFAMEDoorMsg	= NULL;

	/*
	 * Free the GlobBuffer.
	 * Don't forget to NULL the pointer.
	 */

	if(GlobBuffer)
	{
		FreeVec(GlobBuffer);
	}
	GlobBuffer		= NULL;

	/*
	 * We are ready. Let's call the Doors's ShutDown() code now:
	 */

	ShutDown(ReturnCode);

} // STATIC void FIMFree( long ReturnCode, short int Quiet );

/*
 * FUNCTION: long FIMHandlePort( long FIMCommand, ULONG UserSigs, short int NoErrCheck );
 *
 *  PURPOSE: This is the main communication routine, it handles all the stuff
 *           required to send commands to FAME and also interprets errorcodes
 *           and ENDS THE DOOR if the according error occure! See the
 *           DoorCommands.Guide for more informations about these errorcodes
 *           and how to handle them!
 *
 *    INPUT: FIMCommand to send to FAME. If this field is 0, this function
 *           assumes the command to send in MyFAMEDoorMsg->fdom_Command. So
 *           make sure that you set the right command if you are using this
 *           function like FIMHandlePort(0,NULL, NC_CHECK ); !
 *
 *           UserSigs for other user signals from the door to wait for.
 *
 *           NoErrCheck to disable the error checking, and the door abort
 *           will not been done if NoErrCheck = TRUE.
 *
 *   RETURN:  0 = Everything okay, Command successfully executed.
 *           -1 = DoorControlPort not found !
 *           >0 = 1-10 for FAME defined Errorcodes (see DoorCommands.guide).
 *           <0 = -1 to -3 for FAME DoorPort errors (see FAMEDefine.h).
 *
 *     NOTE: This function is mainly used in the support functions, see above
 *           for further infos.
 */

long FIMHandlePort(long FIMCommand, ULONG UserSigs, short int NoErrCheck)
{
	ULONG	CombSigs	= 0UL;
	long	FailFlag	= 0L;

	/*
	 * Use the command from the argument?
	 */

	if(FIMCommand)
	{
		MyFAMEDoorMsg->fdom_Command = FIMCommand;
	}

	/*
	 * Build the signal mask to wait for, first the Door port:
	 */

	CombSigs		|= PortSig;

	/*
	 * Build user signal code into the FAMEDoorStartUp? If add their signals.
	 */

#ifdef FIM_USERSIGMODE

	if(GlobFIMFlags & FIMF_USERSIGMODE)
	{
		if(UserSigs)
		{
			CombSigs	|= UserSigs;
		}
		else if(GlobUserSigs)
		{
			CombSigs	|= GlobUserSigs;
		}
	}

#endif

	/*
	 * Try to find the port of the Node and send the command if found:
	 */

	Forbid();
	if(DoorControlPort = FindPort((UBYTE *)FAMEDoorPort))
	{
		PutMsg(DoorControlPort,(struct Message *)MyFAMEDoorMsg);
	}
  Permit();

	/*
	 * To keep the time of Forbid() as short as possible we check now
	 * if the port has been found or not:
	 */

	if(DoorControlPort)
	{
		/*
		 * Ok, port has been found and the command send, now read the replies:
		 */

		FOREVER
		{
			/*
			 * First let's wait for any signals:
			 */

			Signals = Wait(CombSigs);

			/*
			 * If the UserSig code is implemented check if there is any user signal
			 * in the mask:
			 */

#ifdef FIM_USERSIGMODE

			/*
			 * Enabled the UserSig check with FIMStart( 0, FIMF_USERSIGMODE ) flag?
			 */

			if(GlobFIMFlags & FIMF_USERSIGMODE)
			{
				if(UserSigs || GlobUserSigs)
				{
					/*
					 * Ok it's a user signal, call the Door's procedure UserSigHandle()
					 * to let the Door react on it:
					 */

					UserSigHandle(Signals);
				}
			}

#endif

			/*
			 * Is there a signal from the reply port?
			 */

			if(Signals & PortSig)
			{
				/*
				 * It is. Let's read the replies and react on the last one
				 * (just to be on the safe side), but if there is a negative
				 * return code from the Node we have to react in *ANY* case:
				 */

				while(GetMsg(DoorReplyPort))
				{
					if(MyFAMEDoorMsg->fdom_ReturnCode && FailFlag > -1)
					{
						FailFlag = MyFAMEDoorMsg->fdom_ReturnCode;
					}
				}
				break;
			}
		}
	}
	else
	{
		/*
		 * Doorport of the Node not found.
		 * Free resources and end the Door:
		 *
		 */

		Printf("\nUnable to find %s !\n\n",FAMEDoorPort);

		FIMFree( FIM_DOORABORTREQUESTED, FE_QUIET );

		return( FIM_DOORABORTREQUESTED );
	}

	/*
	 * Any problems with the command?
	 */

	if(FailFlag)
	{
		/*
		 * On positive return code print a text to the user:
		 */

		if(FailFlag > 0)
		{
			FIMPrintError(FailFlag);
		}

		/*
		 * Free resources and end the Door if the caller wants it or the
		 * FIMF_NOCHECK isn't set to disable this:
		 */

		if(NoErrCheck == NC_CHECK && !(GlobFIMFlags & FIMF_NOCHECK))
		{
			FIMFree(FailFlag, FE_NOTQUIET );
		}

		/*
		 * Return the error code to inform the Door:
		 */

		return(FailFlag);
	}

	/*
	 * Return to the Door, everything went well:
	 */

	return(0);

} // long FIMHandlePort( long FIMCommand, ULONG UserSigs, short int NoErrCheck );

/*
 * FUNCTION: STATIC void FIMPrintError( long Error );
 *           !!! PRIVATE !!!
 *
 *  PURPOSE: Fills out global var 'ErrorText' with the supplied error number.
 *           The command which produces the error will be taken from the
 *           MyFAMEDoorMsg->fdom_Command field when a FIM_CDMDENIED error
 *           occures.
 *
 *    INPUT: Error indicates the errornumber from FAME (see FAMEDefines.h
 *           for further informations about these numbers).
 *
 *   RETURN: Nothing.
 *
 *     NOTE: This function is PRIVATE and will only be used inside this
 *           doorstartup-code from FIMHandlePort() in the case of an error!
 *
 */

STATIC void FIMPrintError( long Error )
{
	long		wrongcommand;
  char		*Button[] = {"NR","NC","CF","SR","SC","AR","AC","RD","UN"};
  USHORT	butnum = 0;

	/*
	 * Buffer the used Command:
	 */

	wrongcommand = MyFAMEDoorMsg->fdom_Command;

	/*
	 * Parse the error and inform the user:
	 */

  switch(Error)
	{
		case FIM_CMDABORTED:
			SPrintf(ErrorText,"\n\r[37mCommand [32m%ld [37maborted [m!\n\n\r",wrongcommand);
		break;
		case FIM_CMDNOTIMPLEMENTED:
			SPrintf(ErrorText,"\n\r[37mCommand [32m%ld [37mnot implemented until now in this Version of FAME [m!\n\n\r",wrongcommand);
		break;
		case FIM_CMDDOESNOTEXISTS:
			SPrintf(ErrorText,"\n\r[37mCommand [32m%ld [37mdoesn't exist [m!\n\n\r",wrongcommand);
		break;
		case FIM_CMDNOTSUCCESSFULL:
			SPrintf(ErrorText,"\n\r[37mCommand [32m%ld [37mcouldn't be executed successfully [m!\n\n\r",wrongcommand);
		break;
		default: // FIM_CMDDENIED:

			/*
			 * Was it just a forbidden command type?
			 */

			if(wrongcommand > 999)
			{
				butnum = 7; 							// RD
			}
			else if(wrongcommand > 899)
			{
				butnum = 6;								// AC
			}
			else if(wrongcommand > 799)
			{
				butnum = 5;								// AR
			}
			else if(wrongcommand > 699)
			{
				butnum = 4;								// SC
			}
			else if(wrongcommand > 599)
			{
				butnum = 3;								// SR
			}
			else if(wrongcommand > 399)
			{
				butnum = 2;								// CF
			}
			else if(wrongcommand > 199)
			{
				butnum = 1;								// NC
			}
			else if(wrongcommand > 9)
			{
				butnum = 0;								// NR
			}
      else
      {
				butnum = 8;							  // UN(used)
			}

			SPrintf(ErrorText,"\n\r[37mDoor uses [32m%s [37mCommand, please allow it in your SystemEditor [m!\n\n\r",Button[butnum]);
		break;
	}

} // STATIC void FIMPrintError( long Error );

/*
 *  --- Support Functions (1.3) ---
 */

/*
 * FUNCTION: long GetString( char text[], long Data );
 *
 *  PURPOSE: Request a String from the user with a specific length.
 *
 *    INPUT: text[] = The string to be edited by the user. It also could
 *                    contain a default string.
 *             Data = Max. length of chars to be entered/edited by the user.
 *
 *   RETURN: If TRUE DoorPort is closed and ShutDown() was called during an
 *           error !
 *
 */

long GetString( char text[], long Data )
{
	long	ReturnCode;

  MyFAMEDoorMsg->fdom_Command	= NR_PromptChars;
  MyFAMEDoorMsg->fdom_Data1		= Data;
  MyFAMEDoorMsg->fdom_Data2		= FPC_NORMAL;
  strcpy(MyFAMEDoorMsg->fdom_IOString,text);

	ReturnCode = FIMHandlePort(0,NULL, NC_CHECK );

	if(text)
	{
	  strcpy(text,MyFAMEDoorMsg->fdom_IOString);
	}

  return(ReturnCode);

} // long GetString( char text[], long Data );

/*
 * FUNCTION: long PutString( char text[], long lf );
 *
 *  PURPOSE: Prints a String via AR_SendStr, so unlimited Strings are
 *           supported!
 *
 *    INPUT: text[] = Text to display.
 *               lf = 0 -> No LineFeed.
 *                    1 -> Print Linefeed (\r\n) after text.
 *
 *   RETURN: If TRUE DoorPort is closed and ShutDown() was called during an
 *           error !
 *
 *     NOTE: If the user has switched off the ANSI Colors, the string to
 *           display will be cutted from all ansicolors contained inside
 *           your string! During this operation the original string will
 *           be modified, so if you require the original string containing
 *           the ANSI colors, make first a copy before using this function!
 */

long PutString( char text[], long lf )
{
	MyFAMEDoorMsg->fdom_Command		= AR_SendStr;
	if(lf)
	{
		MyFAMEDoorMsg->fdom_Data1		= 1;
	}
	else
	{
		MyFAMEDoorMsg->fdom_Data1		= 0;
	}
	MyFAMEDoorMsg->fdom_StringPtr	= text;

  return(FIMHandlePort(0,NULL, NC_CHECK ));

} // long PutString( char text[], long lf );

/*
 * FUNCTION: long NC_PutString( char text[], long lf );
 *
 *  PURPOSE: Prints a String via AR_SendStr, so unlimited Strings are
 *           supported!
 *
 *    INPUT: text[] = Text to display.
 *               lf = 0 -> No LineFeed.
 *                    1 -> Print Linefeed (\r\n) after text.
 *
 *   RETURN: If TRUE DoorPort is closed and ShutDown() was called during an
 *           error !
 *
 *     NOTE: If the user has switched off the ANSI Colors, the string to
 *           display will be cutted from all ansicolors contained inside
 *           your string! During this operation the original string will
 *           be modified, so if you require the original string containing
 *           the ANSI colors, make first a copy before using this function!
 *
 *     NOTE: NC_PutString() is exactly the same as PutString(),
 *           but NC_PutString() does no error check and no door abort
 *           if something fails on printing a string to the Node's screen.
 *           Needfull if a prompting for key(s) follows where the check can
 *           be done also.
 */

long NC_PutString( char text[], long lf )
{
	MyFAMEDoorMsg->fdom_Command		= AR_SendStr;
	if(lf)
	{
		MyFAMEDoorMsg->fdom_Data1		= 1;
	}
	else
	{
		MyFAMEDoorMsg->fdom_Data1		= 0;
	}
	MyFAMEDoorMsg->fdom_StringPtr	= text;

  return(FIMHandlePort(0,NULL, NC_NOCHECK ));

} // long NC_PutString( char text[], long lf );

/*
 * FUNCTION: long PutStringFormat( char *ctl, ... );
 *
 *  PURPOSE: Prints a printf() style formatted string using the GlobBuffer
 *           and RawDoFmt() from exec.library, see autodocs for more infos
 *           about the supported formatcodes. The GlobBuffer depends on the
 *           size supplied when starting your door and has a minimum size
 *           of 1024 bytes. See FIMStart() for further infos about this.
 *
 *    INPUT: *ctl = formatstring. See Autodocs 'exec.library/RawDoFmt()'
 *                  for a detailed list of all supported formatcodes.
 *           ...  = All parameters required to fit your format string.
 *
 *   RETURN: If TRUE DoorPort is closed and ShutDown() was called during an
 *           error !
 */

long PutStringFormat( char *ctl, ... )
{
	RawDoFmt(ctl, (long *)(&ctl + 1), (void (*))"\x16\xc0\x4e\x75",GlobBuffer);
	MyFAMEDoorMsg->fdom_Data1			= 0;
	MyFAMEDoorMsg->fdom_Command		= AR_SendStr;
	MyFAMEDoorMsg->fdom_StringPtr = GlobBuffer;

  return(FIMHandlePort(0,NULL, NC_CHECK ));

} // long PutStringFormat( char *ctl, ... );

/*
 * FUNCTION: long NC_PutStringFormat( char *ctl, ... );
 *
 *  PURPOSE: Prints a printf() style formatted string using the GlobBuffer
 *           and RawDoFmt() from exec.library, see autodocs for more infos
 *           about the supported formatcodes. The GlobBuffer depends on the
 *           size supplied when starting your door and has a minimum size
 *           of 1024 bytes. See FIMStart() for further infos about this.
 *
 *    INPUT: *ctl = formatstring. See Autodocs 'exec.library/RawDoFmt()'
 *                  for a detailed list of all supported formatcodes.
 *           ...  = All parameters required to fit your format string.
 *
 *   RETURN: If TRUE DoorPort is closed and ShutDown() was called during an
 *           error !
 *
 *     NOTE: NC_PutStringFormat() is exactly the same as PutStringFormat(),
 *           but NC_PutStringFormat() does no error check and no door abort
 *           if something fails on printing a string to the Node's screen.
 *           Needfull if a prompting for key(s) follows where the check can
 *           be done also.
 */

long NC_PutStringFormat( char *ctl, ... )
{
	RawDoFmt(ctl, (long *)(&ctl + 1), (void (*))"\x16\xc0\x4e\x75",GlobBuffer);
	MyFAMEDoorMsg->fdom_Data1			= 0;
	MyFAMEDoorMsg->fdom_Command		= AR_SendStr;
	MyFAMEDoorMsg->fdom_StringPtr = GlobBuffer;

  return(FIMHandlePort(0,NULL, NC_NOCHECK ));

} // long NC_PutStringFormat( char *ctl, ... );

/*
 * FUNCTION: long PutCommand( char text[], long Data1, long Data2, ULONG Data3, long Command );
 *
 *  PURPOSE: Puts a command with specific parameters to the DoorPort to
 *           perform some action. The returnvalues from the according
 *           functions could be found in the MyFAMEDoorMsg structure fields.
 *
 *    INPUT:  text[] = This will be copied to fdom_IOString
 *             Data1 = This will be set to fdom_Data1
 *             Data2 = This will be set to fdom_Data2
 *             Data3 = This will be set to fdom_Data3
 *           Command = Command to perform (see FAMEDoorCommands.guide)
 *
 *   RETURN: If TRUE DoorPort is closed and ShutDown() was called during an
 *           error !
 *
 */

long PutCommand( char text[], long Data1, long Data2, ULONG Data3, long Command )
{
  MyFAMEDoorMsg->fdom_Command	= Command;
  MyFAMEDoorMsg->fdom_Data1		= Data1;
  MyFAMEDoorMsg->fdom_Data2		= Data2;
  MyFAMEDoorMsg->fdom_Data3		= Data3;
  strcpy(MyFAMEDoorMsg->fdom_IOString,text);

	return(FIMHandlePort(0,NULL, NC_CHECK ));

} // long PutCommand( char text[], long Data1, long Data2, ULONG Data3, long Command );

/*
 * FUNCTION: long GetCommand( char text[], long Data1, long Data2, ULONG Data3, long Command );
 *
 *  PURPOSE: Puts a command with specific parameters to the DoorPort to
 *           perform some action. The returnvalues from the according
 *           functions could be found in the MyFAMEDoorMsg structure fields,
 *           but if you supply a textbuffer the returnvalue found in IOString
 *           (if any) will be copied to this buffer.
 *
 *    INPUT:  text[] = This will be copied to fdom_IOString, after 
 *                     performing the command the IOString contents will be
 *                     copied back to this field, so don't supply here any
 *                     constant texts, instead supply a buffer with at least
 *                     202 Bytes in size or "" to supply no buffer.
 *             Data1 = This will be set to fdom_Data1
 *             Data2 = This will be set to fdom_Data2
 *             Data3 = This will be set to fdom_Data3
 *           Command = Command to perform (see FAMEDoorCommands.guide)
 *
 *   RETURN:  text[] = If you have supplied a buffer the contents of the
 *                     IOString will be copied to your buffer.
 *
 *            Function returns TRUE if DoorPort is closed and ShutDown()
 *            was called during an error !
 *
 */

long GetCommand( char text[], long Data1, long Data2, ULONG Data3, long Command )
{
	long	ReturnCode;

  MyFAMEDoorMsg->fdom_Command	= Command;
  MyFAMEDoorMsg->fdom_Data1		= Data1;
  MyFAMEDoorMsg->fdom_Data2		= Data2;
  MyFAMEDoorMsg->fdom_Data3		= Data3;
  strcpy(MyFAMEDoorMsg->fdom_IOString,text);

  ReturnCode = FIMHandlePort(0,NULL, NC_CHECK );

	if(text)
	{
		strcpy(text,MyFAMEDoorMsg->fdom_IOString);
	}

	return(ReturnCode);

} // long GetCommand( char text[], long Data1, long Data2, ULONG Data3, long Command );

/*
 * FUNCTION: void Center( char *stri, long lf, long col );
 *
 *  PURPOSE: Displays a string centered with given colors and additional
 *           linefeed if required.
 *
 *    INPUT: *stri = The String to be displayed centered, this string may
 *                   also contain ANSI codes. The string is limited to
 *                   the size of GlobBuffer, which you supply in FAMEStart()
 *                   and have a minimum of 1024 bytes.
 *              lf = 0 -> No LineFeed
 *                   1 -> Print Linefeed (\r\n) after the string
 *             col = The color to use for the string, but you may also
 *                   supply multicolor strings!
 *
 *   RETURN: Nothing.
 *
 *     NOTE: If any error occures the DoorPort will be closed and ShutDown()
 *           will be called !!
 */

void Center( char *stri, long lf, long col )
{
	long Middle = NULL;

  strcpy(GlobBuffer,stri);
	FAMECutANSI(GlobBuffer, FCAF_STYLECMDS );
	Middle = ((78 - strlen(GlobBuffer)) / 2);
	GlobBuffer[0] = '\0';

	SPrintf(GlobBuffer,"\r[%ldm[%ldC%s\0",col,Middle,stri);

	PutString(GlobBuffer,lf);

} // void Center( char *stri, long lf, long col );

/*
 * FUNCTION: void SPrintf( char *Buffer, char *ctl, ... );
 *
 *  PURPOSE: Builds a RawDoFmt() like string to a buffer.
 *
 *    INPUT: *Buffer = The Buffer to recieve the formatted String.
 *              *ctl = The Formatstring to use (see exec.library/RawDoFmt()
 *                     for further informations!).
 *               ... = The arguments to fit your formatstring.
 *
 *   RETURN: Nothing.
 */

void SPrintf( char *Buffer, char *ctl, ... )
{
	RawDoFmt(ctl, (long *)(&ctl + 1), (void (*))"\x16\xc0\x4e\x75",Buffer);

} // void SPrintf( char *Buffer, char *ctl, ... );

/*
 * FUNCTION: long FAMEAtol( char *Buffer );
 *
 *  PURPOSE: Converts a textstring to a long value using StrToLong() from
 *           dos.library.
 *
 *    INPUT: *Buffer = The string to convert.
 *
 *   RETURN: The converted long value (0 is returned if you supply a text
 *           without any valid digits!)
 */

long FAMEAtol( char *Buffer )
{
	long	Dummy;

	StrToLong(Buffer,&Dummy);

	return(Dummy);

} // long FAMEAtol( char *Buffer );

// ---EOF---

