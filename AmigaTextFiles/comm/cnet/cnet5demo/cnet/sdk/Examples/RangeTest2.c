/*****************************************************************************
 *                            CNetC library examples                         *
 *                         copyright © ZenMetal Software                     *
 *                              All rights reserved.                         *
 *                                                                           *
 *                 Special thanks to Kelly Cochran for his initial           *
 *                 conversion of empty.c code to a shared library.           *
 *****************************************************************************/

/* ------------------------- COMPILE INSTRUCTIONS --------------------------
 *
 * To compile CNetC doors, and link with cnet_reg.lib use:
 *
 *     SC PARMS=REG NOSTKCHK COMNEST OPT LINK SC SD GSTIMM GST=<path to cnet.gst>
 *
 * To compile CNetC doors, and link with cnet.lib use:
 *
 *     SC NOSTKCHK COMNEST OPT LINK SC SD GSTIMM GST=<path to cnet.gst>
 *
 * ------------------------------------------------------------------------- */



/* -------------------------- FUNCTION PROTOTYPES -------------------------- */
void DoorExit ( void          );
void DoorMain ( char *DosArgs );

/* --------------------- GLOBAL VARIABLE/DATA SECTION ---------------------- */

// CNCL_DoorInit() requires that the following variables are declared
struct RDArgs          *myrdargs     = NULL; // CLI args passed to door from CNet
struct Library         *CNetCBase    = NULL; // pointer to cnet.library lib base
struct Library         *CNet4Base    = NULL; // pointer to cnet4.library lib base
struct CNetCContext    *context      = NULL; // pointer to context data, containing
                                             // cnet mainport, current portdata, etc,.
                                             // see include/doors.h for
                                             // structure information
struct PortData        *z            = NULL; // pointer to be initialized later to
                                             // context->z
struct MainPort        *myp          = NULL; // pointer to be initialized later to
                                             // context->myp

/* --------------------- BEGINNING OF DOOR FUNCTIONS ----------------------- */

void GetOut(void)
{
	DoorExit();
	CNCL_DoorCleanup();
}


void main( void )
{
	LONG myargs[2];

   // CNCL_DoorInit() is in cnet.lib - you MUST link CNetC doors with cnet.lib
	CNCL_DoorInit( myargs );

	// execute your door code
   DoorMain(myargs[1] ? (char *)myargs[1]:NULL);

   // exit back to CNet - exit trap automatically calls GetOut()
   exit(RETURN_OK);
}


// *******************************************************************************
// DO NOT ALTER ANYTHING ABOVE THIS LINE OR FUTURE COMPATIBILITY MAY BE SACRIFICED
// *******************************************************************************



// -------------------- YOUR DOOR EXIT/CLEANUP CODE GOES HERE -------------------
void DoorExit( void )
{
	// Place any resource cleanup code, such as freeing memory, closing libs
   // opened by your door (but not cnetc.library, which is already opened) here.
   // These will be freed by the door exit code whenever exit() is called

   if(CNet4Base)
      {
      CloseLibrary(CNet4Base);
      CNet4Base=NULL;
      }

	return;
}



// -------------- YOUR DOOR CODE EXECUTION BEGINS IN THIS FUNCTION --------------
void DoorMain( char *DosArgs )
{
	// Your door code begins here.
	// DosArgs == NULL if no ** DOS ** args were passed to door from CNet

	struct RangeContext rc;
	LONG x;

   if( !(CNet4Base = OpenLibrary("cnet4.library", 4)) )
		exit(RETURN_ERROR);

	while(CNC_EnterLine(40, ELINE_INPUTBOX, "Enter a CNet range [RETURN=exit]: "))
		{

		// notice that the range limits are 1 to 100

		if( CNetFindRange(z->InBuffer, 1, 100, &rc) )
			{
			do
				{
				x=CNetNextRange( &rc );
				sprintf( z->ABuffer, "x=%ld\n", x );
				CNC_PutA();
				} while( !CNetEndOfRange(&rc) );
			}
		else
			CNC_PutText( "Bad range; ranges between 1 and 100 only!\n" );
		}


	// CNetNextRange() also returns -1 if there are no more elements in the range.
	// Therefore, the following method is another way to handle the above code.

	while( CNC_EnterLine(40, ELINE_INPUTBOX, "Enter a CNet range [RETURN=exit]: ") )
		{

		// notice that the range limits here are 10 to 300

		if( CNetFindRange(z->InBuffer, 10, 300, &rc) )
			{
			while( (x=CNetNextRange(&rc)) != -1 )
				{
				sprintf( z->ABuffer, "x=%ld\n", x );
				CNC_PutA();
				} 
			}
		else
			CNC_PutText( "Bad range; ranges between 10 and 300 only!\n" );
		}

	// CNetPrevRange() test
	while( CNC_EnterLine(40, ELINE_INPUTBOX, "Enter a CNet range [RETURN=exit]: ") )
		{

		// notice that the range limits here are 10 to 300

		if( CNetFindRange(z->InBuffer, 10, 300, &rc) )
			{
			UBYTE last=FALSE; // flag to break loop if we are at last element

			// do forward direction until x=50
			CNC_PutText("spanning forward in range until (x >= 50) is reached (if range exceeds 50)..\n");
			do
				{
				x=CNetNextRange(&rc);
				sprintf(z->ABuffer, "x=%ld", x);
				CNC_PutA();

				if(last=CNetEndOfRange(&rc))
					CNC_PutText(" is last element in range!\n");

				CNC_DoReturn();
				} while( x<50 && !last );

			CNC_PutText("Now traversing backwards in range from current element to first element..\n");

			// test CNetPrevRange()..
			while( (x=CNetPrevRange(&rc)) != -1 )
				{
				sprintf(z->ABuffer, "x=%ld", x);
				CNC_PutA();
				if(CNetStartOfRange(&rc))
					CNC_PutText(" is first element in range!\n");
				CNC_DoReturn();
				}

			}
		}	

	return;
}
