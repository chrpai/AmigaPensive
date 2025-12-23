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
struct CNetCContext    *context      = NULL; // pointer to context data, containing
                                             // cnet mainport, current portdata, etc,.
                                             // see include/doors.h for
                                             // structure information
struct PortData        *z            = NULL; // pointer to be initialized later to
                                             // context->z
struct MainPort        *myp          = NULL; // pointer to be initialized later to
                                             // context->myp
char                   errtext[]     = "This is a CNetC door!\n";

/* --------------------- BEGINNING OF DOOR FUNCTIONS ----------------------- */

void GetOut(void)
{
	DoorExit();
	CNCL_DoorCleanup();
}


void main( void )
{
	LONG myargs[NUM_CNETC_ARGS];

   // CNCL_DoorInit() is in cnet.lib - you MUST link CNetC doors with cnet.lib
	if(!CNCL_DoorInit(myargs))
	{
		Printf(errtext);
		exit(0);
	}

	// execute your door code
   DoorMain(myargs[CNETC_MAINARG] ? (char *)myargs[CNETC_MAINARG]:NULL);

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

	return;
}



// -------------- YOUR DOOR CODE EXECUTION BEGINS IN THIS FUNCTION --------------
void DoorMain( char *DosArgs )
{
	// Your door code begins here.
	// DosArgs == NULL if no ** DOS ** args were passed to door from CNet

	return;
}
