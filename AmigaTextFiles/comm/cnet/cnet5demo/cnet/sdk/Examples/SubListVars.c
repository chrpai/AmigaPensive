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

	return;
}



// -------------- YOUR DOOR CODE EXECUTION BEGINS IN THIS FUNCTION --------------
void DoorMain( char *DosArgs )
{
	// Your door code begins here.
	// DosArgs == NULL if no ** DOS ** args were passed to door from CNet

	LONG zzz=0;

	for( zzz=0 ; (zzz<myp->ns) && IsAbort() ; zzz++ )
		{
		// print subboard info!
		sprintf(z->ABuffer, "\nTITLE        : %su0\n", myp->Subboard[zzz].Title);
		CNC_PutA();
		sprintf(z->ABuffer, "SubDirName   : %s\n", myp->Subboard[zzz].SubDirName);
		CNC_PutA();
		sprintf(z->ABuffer, "DataPath     : %s\n", myp->Subboard[zzz].DataPath);
		CNC_PutA();
		sprintf(z->ABuffer, "Parts        : %ld\n", myp->Subboard[zzz].Parts);
		CNC_PutA();
		sprintf(z->ABuffer, "Filler       : %s\n", myp->Subboard[zzz].Filler);
		CNC_PutA();
		sprintf(z->ABuffer, "Direct       : %d\n", myp->Subboard[zzz].Direct);
		CNC_PutA();
		sprintf(z->ABuffer, "FreeDays     : %d\n", myp->Subboard[zzz].FreeDays);
		CNC_PutA();
		sprintf(z->ABuffer, "SubDirectory : %d\n", myp->Subboard[zzz].Subdirectory);
		CNC_PutA();
		sprintf(z->ABuffer, "Closed       : %d\n", myp->Subboard[zzz].Closed);
		CNC_PutA();
		sprintf(z->ABuffer, "Union Flags  : %d\n", myp->Subboard[zzz].UnionFlags);
		CNC_PutA();
		sprintf(z->ABuffer, "Access       : %d\n", myp->Subboard[zzz].Access);
		CNC_PutA();
		sprintf(z->ABuffer, "Post Access  : %d\n", myp->Subboard[zzz].PostAccess);
		CNC_PutA();
		sprintf(z->ABuffer, "RespondAccess: %d\n", myp->Subboard[zzz].RespondAccess);
		CNC_PutA();
		sprintf(z->ABuffer, "Upload Access: %d\n", myp->Subboard[zzz].UploadAccess);
		CNC_PutA();
		sprintf(z->ABuffer, "DLoad Access : %d\n", myp->Subboard[zzz].DownloadAccess);
		CNC_PutA();
		sprintf(z->ABuffer, "Comp Types   : %d\n", myp->Subboard[zzz].ComputerTypes);
		CNC_PutA();
		sprintf(z->ABuffer, "Zero Path    : %s\n", myp->Subboard[zzz].ZeroPath);
		CNC_PutA();
		sprintf(z->ABuffer, "Origin       : %s\n", myp->Subboard[zzz].Origin);
		CNC_PutA();
		sprintf(z->ABuffer, "Max Items    : %d\n", myp->Subboard[zzz].MaxItems);
		CNC_PutA();
		sprintf(z->ABuffer, "nm           : %d\n", myp->Subboard[zzz].nm);
		CNC_PutA();
		sprintf(z->ABuffer, "rn           : %d\n", myp->Subboard[zzz].rn);
		CNC_PutA();
		sprintf(z->ABuffer, "AllocItems   : %d\n", myp->Subboard[zzz].AllocItems);
		CNC_PutA();
		sprintf(z->ABuffer, "nNewMess     : %d\n", myp->Subboard[zzz].nNewMess);
		CNC_PutA();
		}

	return;
}
