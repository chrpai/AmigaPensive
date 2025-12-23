
/* FAME Door Beginning Source
 *
 * $VER:TestDoor.c v1.3
 *
 * You need: Include:FAME/FAMEDefine.h
 *           Include:FAME/FAMEDoorCommands.h
 *           Include:FAME/FAMEDoorDefines.h
 *           Include:FAME/FAMEDoorExpansion.h
 *           Include:FAME/FAMEDoorProto.h
 *           Include:FAME/FAMEIncluding.h
 *           Include:FAME/FAMEPublic.h
 *           Include:FAME/FAMEPublicStructs.h
 *           Include:FAME/FAMEUserStructs.h
 *           Include:FAME/FAMEVersion.h
 *           FAMEDoorStartUp/FAMEDoorStartUp.c
 *           FAMEDoorStartUp/FAMEDoorStartUp.o
 *
 * Used tab size: 2
 */

/*
 * Includes:
 */

#include <FAME/FAMEPublic.h>

/*
 * Disable Lattice CTRL-C handling:
 */

#ifdef LATTICE
int CXBRK(void) {return(0);}
int chkabort(void) {return(0);}
#endif

/*
 * Define our version string for the "version" command:
 */

char *VerStr={"$VER: TestDoor.FIM 1.3 "__AMIGADATE__};

/*
 * _ProgramName contains our filename:
 */

extern char	*_ProgramName;

/*
 * Prototypes:
 */

				void	ShutDown( long ErrCode );
				void	UserSigHandle( ULONG UserSigs );
STATIC	long	OpenTimer( void );
STATIC	void	SetTimer( ULONG secs, ULONG micros );
STATIC	void	ClearTimer( void );

/*
 * No CON window please, quit immediately.
 */

void __autoopenfail(void) { _XCEXIT(0);}

/*
 * Variables for ReadArgs and Template.
 */

char 	ArgStr[]		= "NODENR/N/A";
long	ArgArray[]	= {0L};

/*
 * Global variables:
 */

struct FAMELibrary	*FAMEBase			= NULL;
struct timerequest	*TimerMsg			= NULL;
struct MsgPort			*TimerPort		= NULL;
short int						TimerRec			= 0;
unsigned long				TimerSig;

/*
 * void main( void );
 *
 * main() entry point:
 */

void main( void )
{
	struct  RDArgs	*rda						= NULL;
	long						NodeNr;
	char						StrBuffer[256];

	/*
	 * Use ReadArgs() for arguments:
	 */

	if(rda = ReadArgs(ArgStr,ArgArray,NULL))
	{
		/*
		 * Read the Node number:
		 */

		if(ArgArray[0])
		{
			long *LDummy = (long *)ArgArray[0];
			NodeNr = (*LDummy);
		}

		FreeArgs(rda);
	}
	else
	{
		/*
		 * Something failed with the argument, exit the door:
		 */

    Printf("\nSorry, %s is a Door and must be called from FAME BBS!\n\n",_ProgramName);

		SetIoErr( ERROR_REQUIRED_ARG_MISSING );
		PrintFault(IoErr(),NULL);

    exit( RETURN_FAIL );
  }

	/*
	 * Now we have to open the FAME.library.
	 *
	 * From now on we have to call our ShutDown() function if something fails
	 * on our own resource initialisation where we have to release all our
	 * Door resources in ShutDown(). We are allowed to use ShutDown() just to
	 * the point where we call FIMStart(). From then we *HAVE* to call
	 * FIMEnd(0) to end the door!
	 *
	 * FIMEnd(0) will do our ShutDown() call then for us.
	 */

	if( ! (FAMEBase = (struct FAMELibrary *) OpenLibrary("FAME.library",0L)))
	{
		ShutDown( RETURN_FAIL );
	}

	/*
	 * To show the usage of the UserSigs we are opening now the timer.device
	 * and try to get all needed resources:
	 */

	if(OpenTimer())
	{
		Printf("Error opening " TIMERNAME "\n");

		ShutDown( RETURN_ERROR );
	}

	/*
	 * First we set up the TimerSig bit:
	 */

	TimerSig = 1L<<TimerPort->mp_SigBit;

	/*
	 * Now we combine (OR) it with out global user signal mask:
	 */

	GlobUserSigs |= TimerSig;

	/*
	 * Ok time to continue with the Door initialisation code.
	 *
	 * Now we have to build the MessagePort name string of the Door we
	 * are called from:
	 */

  SPrintf(FAMEDoorPort,"FAMEDoorPort%ld",NodeNr);

	/*
	 * Now we start the Door initialising code.
	 *
	 * Here we can define the buffer size for the Global String Pointer
	 * if we need more than the default of 1024 bytes. If we call FIMEnd()
	 * with a value < 1024 bytes (FIM_MINBUFSIZE) like here (0) FIMStart()
	 * will use automatically the default value of FIM_MINBUFSIZE (1024 bytes).
	 *
	 * With the second argument we can set some flags we might need for our
	 * Door to tell the FIM DoorStartUp code how to handle some things we
	 * need. Following flags are available (bit-flags you can combine them by
	 * separating them with the OR sign: |
	 * i.e.: FIMF_USERSIGMODE | FIMF_NOCHECK.
	 *
	 * - FIMF_USERSIGMODE to enable the user signal check.
	 * - FIMF_NOCHECK to disable the error check of FIM commands send to
	 *   the door. Some doors might need to continue work also if there is
	 *   a problem with the port or a command like a lost carrier.
	 *   If FIMF_NOCHECK is set *YOU* have to check the return code of the
	 *   support commands you are using yourself!
	 *
	 * Example: (FIM_CMDSUCCESSFULL = 0)
	 *
	 * if(GetString(StrBuffer,50) < FIM_CMDSUCCESSFULL)
	 * {
	 *   ...
	 *
	 *   FIMEnd(0);
	 * }
	 *
	 *
	 * From now on you *HAVE* to call FIMEnd(0) to end the door!
	 * FIMEnd(0) calls the from the door supported function ShutDown() itself.
	 * This is also the reason why if FIMStart() fails no longer ShutDown()
	 * will be called from here. FIMStart() has already called it if something
	 * failed during the Door initialisation so if FIMStart() fails just
	 * exit() is needed here:
	 */

  if(FIMStart( 0, FIMF_USERSIGMODE ))
	{
		exit( RETURN_ERROR );
	}

  /*
	 * Place your own code in here:
	 */

	// ****************************************************************

	/*
	 * Set a timeout of 10 seconds:
	 */

	SetTimer(10UL,0UL);

	/*
	 * Clear our buffer:
	 */

	StrBuffer[0] = '\0';

	/*
	 * Print a text to the user:
	 */

	PutString("\r\nTimer set to 10 seconds.",0);

	/*
	 * Print a prompt to the user:
	 */

	PutString("\r\nTell me something>: ",0);

	/*
	 * Wait for the input:
	 */

	GetString(StrBuffer,50);

	/*
	 * Print a formated text to the user incl. user's input:
	 */

	PutStringFormat("\r\n\nYou've said: %s\r\n",StrBuffer);

	/*
	 * If TimerRec is true we have received the 10 seconds timeout what means
	 * the user needed more than 10 seconds for the answer.
	 */

	if(TimerRec)
	{
		PutString("Timer signal received after 10 seconds!\r\n\n",0);
	}
	else
	{
		PutString("Timer signal not received! You was faster!\r\n\n",0);
	}

	// ****************************************************************

	/*
	 * End the Door:
	 */

	FIMEnd(0);

} // void main( void );

/*
 * STATIC long OpenTimer( void );
 *
 * Open the timer.device and the depending resources.
 */

STATIC long OpenTimer( void )
{
	if( ! (TimerPort	= CreateMsgPort()))
	{
		return(-1);
	}
	if( ! (TimerMsg		= (struct timerequest *)CreateIORequest(TimerPort,(long)sizeof(struct timerequest))))
	{
		return(-2);
	}

	TimerMsg -> tr_node . io_Message . mn_ReplyPort = TimerPort;

	return(OpenDevice(TIMERNAME,UNIT_VBLANK,(struct IORequest *)TimerMsg,0L));

} // STATIC long OpenTimer( void );

/*
 * STATIC void SetTimer( ULONG secs, ULONG micros );
 *
 * A procedure the send a timer request in seconds and micros.
 */

STATIC void SetTimer( ULONG secs, ULONG micros )
{
	TimerMsg -> tr_node . io_Command							= TR_ADDREQUEST;	/* add a new timer request */
	TimerMsg -> tr_time . tv_secs									= secs;						/* seconds */
	TimerMsg -> tr_time . tv_micro								= micros;					/* microseconds */
	TimerMsg -> tr_node . io_Message.mn_ReplyPort	= TimerPort;

	SendIO((struct IORequest *)TimerMsg);														/* post the request to the timer device */

} // STATIC void SetTimer( ULONG secs, ULONG micros );

/*
 * STATIC void ClearTimer( void );
 *
 * A procedure the cancel/abort the current timer request.
 */

STATIC void ClearTimer( void )
{
	if( ! CheckIO((struct IORequest *)TimerMsg))
	{
		AbortIO((struct IORequest *)TimerMsg);
		WaitIO((struct IORequest *)TimerMsg);
		ClrSignal(TimerSig);
	}

} // STATIC void ClearTimer( void );

/*
 * void ShutDown( long ErrCode );
 *
 * This function has to be supplied by the door and will be called if any
 * error occures. It's up to the door to exit or continue the work, but
 * remember that the DoorPort and therefor the communication to FAME is
 * CLOSED ! The errcode contains the errorcode supplied by FAME, you may
 * use it to your own requirements.
 */

void ShutDown( long ErrCode )
{
	/*
	 * Abort the last timer request (if any), close the timer.device
	 * and delete the IORequest of the timer.
	 *
	 * First check if the resource is really allocated:
	 */

	if(TimerMsg)
	{
		ClearTimer();

		CloseDevice((struct IORequest *)TimerMsg);
		DeleteIORequest((struct IORequest *)TimerMsg);
	}

	/*
	 * To NULL our pointers after freeing the resource of it is *VERY*
	 * important to avoid problems if ShutDown() gets called more the one time!
	 */

	TimerMsg	= NULL;

	/*
	 * Delete now the port used for the timer.
	 *
	 * First check if the resource is really allocated:
	 */

	if(TimerPort)
	{
		DeleteMsgPort(TimerPort);
	}

	/*
	 * To NULL our pointers after freeing the resource of it is *VERY*
	 * important to avoid problems if ShutDown() gets called more the one time!
	 */

	TimerPort	= NULL;

	/*
	 * Now close the FAME.library.
	 *
	 * First check if the resource is really allocated:
	 */

	if(FAMEBase)
	{
		CloseLibrary((struct Library *)FAMEBase);
	}

	/*
	 * To NULL our pointers after freeing the resource of it is *VERY*
	 * important to avoid problems if ShutDown() gets called more the one time!
	 */

	FAMEBase = NULL;

	/*
	 * Now do some standard AmigaOS error handling if needed:
	 */

	if(ErrCode < RETURN_OK )
	{
		SetIoErr( ERROR_OBJECT_NOT_FOUND );
		PrintFault(IoErr(),NULL);

		ErrCode = RETURN_ERROR;
	}

	/*
	 * Exit the Door with a standard and defined AmigaOS exit code:
	 */

	exit( ErrCode );

} // void ShutDown( long ErrCode );

/*
 * void UserSigHandle( ULONG UserSigs );
 *
 * This function has to be supplied by the door and will be called if any
 * user signal occures. User signals will be set via FIMHandlePort() in the
 * second argument "ULONG UserSigs". Use NULL here if you don't have any
 * signals to be supported. UserSigHandle() will be called if any user
 * signals are set and if FIMHandlePort() receives them. In this case
 * check the argument "UserSigs" in your UserSigHandle() function.
 *
 * You can also use the global variable "GlobUserSigs" to inform
 * FIMHandlePort() about your own additional signal bits.
 * In this case FIMHandlePort()'s second argument "ULONG UserSigs" has to
 * be always NULL, because this argument has the higher priority and
 * "GlobUserSigs" will not be used then.
 * "GlobUserSigs" is to handle your own signal bits more easy.
 * All support functions will pass NULL to FIMHandlePort() so usage of
 * "GlobUserSigs" is possible without doing any changes to the doorstartup
 * code.
 */

void UserSigHandle( ULONG UserSigs )
{
	/*
	 * If the UserSigs contains the TimerSig we have received the timer
	 * signal. There is no message with it, just set a flag to remmeber it:
	 */

	if(UserSigs & TimerSig)
	{
		TimerRec = 1;
	}

} // void UserSigHandle( ULONG UserSigs );

