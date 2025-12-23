/*****************************************************************************
 *                            CNetC library examples                         *
 *                         copyright © ZenMetal Software                     *
 *                              All rights reserved.                         *
 *                                                                           *
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

#include "csi:version.h"


/* ---------------------- DEFAULT FUNCTION PROTOTYPES ---------------------- */
void DoorExit ( void          );
void DoorMain ( char *DosArgs );

/* --------------------- GLOBAL VARIABLE/DATA SECTION ---------------------- */

// CNCL_DoorInit() requires that the following 5 variables are declared
struct RDArgs          *myrdargs     = NULL; // CLI args passed to door from CNet
struct Library         *CNetCBase    = NULL; // pointer to cnetc.library lib base - opened by CNCL_DoorInit()
struct CNetCContext    *context      = NULL; // pointer to CNetC context data, containing cnet mainport, current portdata, etc,. see include/doors.h for structure information
struct PortData        *z            = NULL; // pointer to be initialized by CNCL_DoorInit() later to context->z
struct MainPort        *myp          = NULL; // pointer to be initialized by CNCL_DoorInit() later to context->myp

char                   **bm;                 // pointer to bbstext lines - set by us in DoorMain()
struct Library         *CNet4Base    = NULL; // pointer to cnet4.library lib base - opened by us in DoorMain()
struct Library         *CNetBase     = NULL; // pointer to cnet.library lib base - opened by us in DoorMain()


// these were made globals to avoid subtle quirks..
// you should be able to move them as local variables as long as you use
// the __aligned C keyword to make sure they are LONGWORD aligned.
struct ItemHeader      MyHead;
struct ItemType3       MyItem;
struct SubboardType4   *sp     = NULL;
short                  physnum = 0;







// **********************************************************************************
// DO NOT ALTER ANYTHING WITHIN THESE LINES OR FUTURE COMPATIBILITY MAY BE SACRIFICED
// VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV
void GetOut(void)
{
	DoorExit();
	CNCL_DoorCleanup();
}


void main( void )
{
	LONG myargs[3];

	memset(myargs, 0, 4*3);

   // CNCL_DoorInit() is in cnet.lib - you MUST link CNetC doors with cnet.lib
	CNCL_DoorInit( myargs );

	// execute your door code
   DoorMain(myargs[1] ? (char *)myargs[1]:NULL);

   // exit back to CNet - exit trap automatically calls GetOut()
   exit(RETURN_OK);
}
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// DO NOT ALTER ANYTHING BETWEEN THESE LINES OR FUTURE COMPATIBILITY MAY BE SACRIFICED
// ***********************************************************************************







void SizeItems( struct SubboardType4 *sub )
{
	// only allow usage on file transfer and msg areas..
	if( (sub->Marker != MRK_FILE_TXFER) && (sub->Marker != MRK_MSG_BASE) )
		CNC_PutText(bm[2595]); // "command cannot be used on non-file areas"

	// make sure we don't attempt to do processing on a killed subboard..
	else if(sub->Marker & MRK_SUBBOARD_KILLED)
		{
		// this case should theoretically never happen but..  just in case..
		// Let's lie and say we never found it. 8^P
		sprintf(z->ABuffer, bm[2596], sub->SubDirName); // "could not find sub w/go key "%s"
		CNC_PutA();
		}

	// checks panned out..  attempt to "enter" subboard and do our thing..
	// note the requirement that OneMoreUser() MUST be called before ZGet* functions
	// can be used..
	else if(OneMoreUser(sub, FALSE))
		{
		long  j            = 0L;

		// loop through all subboard items..
		for( j=0; j<sub->rn; j++ )
		   {
		   // Get info for the current item..
		   // note that we could have made MyItem and MyHead regular
		   // non-pointer variables and used ZGetItem() but that's
		   // just a design decision..
		   ZGetItem( &z->Item0, &z->ihead0, sub, j );

			if(z->ihead0.Size)
				{
				// unval only applies to files

				// check item validation status..
				if(!z->Item0.Validated)
					{
					short oldbr=z->br;
					
					// output item information..
					z->br = j+1; // set internal item counter to current item number
					CNC_OutScan(); // display current item (z->Item0 & z->ihead0 packed with item) attributes
					z->br=oldbr; // restore internal item counter
					}
				}
		   }

		// "exit" subboard.
		OneLessUser(sub);
		}

	return;
}



// -------------------- YOUR DOOR EXIT/CLEANUP CODE GOES HERE -------------------
void DoorExit( void )
{
	// Place any resource cleanup code, such as freeing memory, closing libs
   // opened by your door (but not cnetc.library, which is already opened) here.
   // These will be freed by the door exit code whenever exit() is called


	// close the libs we opened in DoorMain()
	if(CNetBase)
		CloseLibrary(CNetBase);
	CNetBase = NULL;

	if(CNet4Base)
		CloseLibrary(CNet4Base);
	CNet4Base = NULL;

	return;
}



// -------------- YOUR DOOR CODE EXECUTION BEGINS IN THIS FUNCTION --------------
void DoorMain( char *DosArgs )
{
	// Your door code begins here.
	// DosArgs == NULL if no ** DOS ** args were passed to door from CNet

	bm=z->bm; // set our bbstext lines pointer

	// open required libs..
	// DO NOT open/close cnetc.library..  The door construct/destruct code does that for you..
	if(!(CNetBase = OpenLibrary(CNETLIBNAME, 4L)))
		{
		CNC_PutText("Could not open cnet.library\n");
		return;
		}
	if(!(CNet4Base = OpenLibrary(CNET4LIBNAME, 4L)))
		{
		CNC_PutText("Could not open cnet4.library\n");
		return;
		}
		
	// if we have AmigaDOS args or CNet args, parse them..
	if(DosArgs || (z->npitems > 1))
		{
		if( ((physnum = CNC_FindForeignArea( DosArgs ? DosArgs:z->pitem[1])) != INVALID_SUB_PHYSNUM) &&
          (physnum != MAIN_SUB_PHYSNUM))
			{
			sp = &myp->Subboard[physnum];
			}
		else
			{
			sprintf(z->ABuffer, bm[2596], DosArgs ? DosArgs:z->pitem[1]); // "Could not find subboard with go key of %s"
			CNC_PutA();
			return;
			}
		}
		
	// else check if we are IN a subboard now and default to that one..
	else if(z->sb)
		{
		sp = z->sb;
		}

	// else not in a subboard.. print verbose instruction/hint
	else
		{
		CNC_PutText(bm[2601]); // "You must supply a subboard GO arg or be in the subboard you wish to size"
		return;
		}

	// if all went well, proceed to final checks and size function.
	SizeItems(sp);

	return;
}
