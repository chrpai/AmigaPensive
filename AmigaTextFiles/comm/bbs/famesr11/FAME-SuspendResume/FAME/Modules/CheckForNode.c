
/*
 * CheckForNode v1.1
 * ~~~~~~~~~~~~~~~~~
 *
 * $VER: CheckForNode.c v1.1
 *
 * Code by: David 'Strider/tRSi' Wettig
 * E-Mail:  strider@trsi.de
 * URL:     http://www.trsi.de/inno/strider/index.html
 *
 * Used tab size: 2
 */

/*
 * Includes:
 */

#include <stdio.h>
#include <strings.h>
#include <proto/exec.h>
#include <FAME/FAME.h>

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

char *VerStr = {"$VER: CheckForNode v1.1 "__AMIGADATE__};

/*
 * Prototypes:
 */

void SPrintf( char *Buffer, char *ctl, ... );
long FAMEAtol( STRPTR ConvertString );
void LoadMiscCnf( void );

/*
 * No CON window please, quit immediately.
 */

void __autoopenfail(void) { _XCEXIT(0);}

/*
 * Variables for ReadArgs and Template.
 */

char 	ArgStr[]		= "NODENR/K/N,LOCATION/K";
long	ArgArray[]	= {0L,0L};

/*
 * Global variables:
 */

struct mISC aKTmISC;

char	Buffer[256],
			BBSLocationPath[256],
			RexxPortName[52];

/*
 * void main( void );
 *
 * main() entry point:
 */

void main( void )
{
	struct	RDArgs	*rda		= NULL;
	BPTR						FilePtr	= NULL;
	long						Node		= 0,
									RetCode;

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
			Node = (*LDummy);
		}

		/*
		 * Location path of FAME:
		 */

		if(ArgArray[1])
		{
			strcpy(BBSLocationPath,(STRPTR)ArgArray[1]);
		}

		FreeArgs(rda);
	}
	else
	{
		/*
		 * Something failed with the argument, exit:
		 */

		SetIoErr( ERROR_REQUIRED_ARG_MISSING );
		PrintFault(IoErr(),"CheckForNode");

    exit( RETURN_FAIL );
  }

	if( ! BBSLocationPath[0])
	{
		strcpy(BBSLocationPath,"FAME:");
	}

	if(Node < 0)
	{
		Node = 0;
	}

	Printf("\nCheckForNode v1.1, FAME Node finder by David 'Strider' Wettig.\n\nChecking for Node %ld...",Node);

	aKTmISC.RexxPort[0] = '\0';

  SPrintf(Buffer,"%sSettings/Misc.dat",BBSLocationPath);

  if(FilePtr = Open(Buffer,MODE_OLDFILE))
	{
		Seek(FilePtr,61,OFFSET_BEGINNING);
    Read(FilePtr,&aKTmISC,sizeof(struct mISC));
    Close(FilePtr);
  }

	if( ! aKTmISC.RexxPort[0])
	{
		strcpy(aKTmISC.RexxPort,"FAMERexxPort.");
	}

	SPrintf(RexxPortName,"%s%ld",aKTmISC.RexxPort,Node);

	Forbid();
	if(FindPort(RexxPortName))
	{
		RetCode = RETURN_OK;
	}
	else
	{
		RetCode = RETURN_FAIL;
	}
	Permit();

	if(RetCode == RETURN_OK)
	{
		Printf("found !\n\nNode %ld found !",Node);
	}
	else
	{
		Printf("not found !\n\nNode %ld not found !",Node);
	}

	Printf("\n\n");

	exit( RetCode );

} // void main( void );

/*
 * void SPrintf( char *Buffer, char *ctl, ... );
 *
 * SPrintf via RawDoFmt:
 *
 * Note that you have to uses long aligned values means instead of only
 * using %d you have to use %ld! Also %lc ...
 */

void SPrintf( char *Buffer, char *ctl, ... )
{
	RawDoFmt(ctl, (long *)(&ctl + 1), (void (*))"\x16\xc0\x4e\x75",Buffer);

} // void SPrintf( char *Buffer, char *ctl, ... );

/*
 * long FAMEAtol( STRPTR ConvertString );
 *
 * Atol (Array to long) via StrToLong:
 */

long FAMEAtol( STRPTR ConvertString )
{
	long ConvertLong;

	StrToLong(ConvertString,&ConvertLong);
	return(ConvertLong);

} // long FAMEAtol( STRPTR ConvertString );

