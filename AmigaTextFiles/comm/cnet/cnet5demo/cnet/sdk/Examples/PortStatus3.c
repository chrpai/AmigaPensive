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
struct Library         *CNet4Base    = NULL; // pointer to cnet.library lib base
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

	LONG zzz=0;

   if( !(CNet4Base = OpenLibrary("cnet4.library", 4)) )
   	exit(RETURN_ERROR);

	sprintf(z->ABuffer, "   Your Handle: %s\n", z->user1.Handle);
	CNC_PutA();
	sprintf(z->ABuffer, "Your Birthdate: %d-%d-%d\n", z->user1.Birthdate.Year, z->user1.Birthdate.Month, z->user1.Birthdate.Date);
	CNC_PutA();
	
	sprintf(z->ABuffer, "z->NoUploaded = %d\n", z->NoUploaded);
	CNC_PutA();
	
	sprintf(z->ABuffer, "z->nupload = %d\n", z->nupload);
	CNC_PutA();

	sprintf(z->ABuffer, "myp->gc.nupload = %d\n", myp->gc.nupload);
	CNC_PutA();

	return;
}
